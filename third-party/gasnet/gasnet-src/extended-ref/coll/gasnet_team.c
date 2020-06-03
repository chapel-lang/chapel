/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_team.c $
 *
 * Description: GASNet generic team implementation for collectives 
 * Copyright 2009, The Regents of the University of California
 * Terms of use are as specified in license.txt 
 */

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_refcoll.h>

#include <coll/gasnet_team.h>
#include <coll/gasnet_autotune_internal.h>
#include <coll/gasnet_scratch.h>
#include <coll/gasnet_trees.h>

#ifdef GASNETE_COLL_TEAM_CONDUIT_DECLS
GASNETE_COLL_TEAM_CONDUIT_DECLS
#endif

/* #define DEBUG_TEAM */

static 
gasnete_hashtable_t *team_dir = NULL;

static
volatile uint32_t my_team_seq = 1;
static
volatile uint32_t new_team_id = 0; /* new_team_id is for communication
                                      between the AM handler
                                      (gasnete_coll_teamid_reqh) and
                                      the main thread, 0 means the new
                                      team id is not set. */

/*called by only one thread*/
static void initialize_team_fields(gasnete_coll_team_t team,  
                                   const gasnet_image_t images[], gex_Rank_t myrank, gex_Rank_t num_members,
                                   gasnet_seginfo_t * scratch_segments GASNETI_THREAD_FARG) {

  size_t image_size = num_members*sizeof(gasnet_image_t);
  int i;
  static size_t smallest_scratch_seg;

  team->sequence = 0xfffffff8;  // Intentionally near to wrap-around

  smallest_scratch_seg = scratch_segments[0].size;
  for (i = 0; i < num_members; ++i) {
    smallest_scratch_seg = MIN(smallest_scratch_seg, scratch_segments[i].size);
  }

#if GASNET_PAR && GASNET_DEBUG
  gasneti_mutex_init(&team->threads_mutex);
#endif
  
  team->tree_geom_cache_head = NULL;
  team->tree_geom_cache_tail = NULL;
  gasneti_mutex_init(&team->tree_geom_cache_lock);
  team->tree_construction_scratch = NULL;
  team->dissem_cache_head = NULL;
  team->dissem_cache_tail = NULL;
  gasneti_mutex_init(&team->dissem_cache_lock);
  team->myrank = myrank;
  team->total_ranks = num_members;
  team->scratch_segs = scratch_segments;
  team->smallest_scratch_seg = smallest_scratch_seg;
  team->autotune_info = gasnete_coll_autotune_init(team, smallest_scratch_seg GASNETI_THREAD_PASS);
  team->consensus_id = team->consensus_issued_id = 0xfffffff8;  // Intentionally near to wrap-around
  gasnete_coll_alloc_new_scratch_status(team);
  team->scratch_free_list = NULL;
  
#ifndef GASNETE_COLL_P2P_OVERRIDE
  gex_HSL_Init(&team->p2p_lock);
  team->p2p_freelist = NULL;
  for (i = 0; i < GASNETE_COLL_P2P_TABLE_SIZE; ++i) {
    team->p2p_table[i] = NULL;
  }
#endif
}

/* Helper for gasnete_coll_team_init() */
static int gasnete_node_pair_sort_fn(const void *a_p, const void *b_p) {
  const int a0 = ((const gex_Rank_t *)a_p)[0];
  const int b0 = ((const gex_Rank_t *)b_p)[0];
  const int d0 = (a0 - b0); /* sort first by supernode */
  if (d0) return d0;
  else {
    const int a1 = ((const gex_Rank_t *)a_p)[1];
    const int b1 = ((const gex_Rank_t *)b_p)[1];
    /* break ties by node - must be increasing order because
     * we use local rank to determine the active node
     */
    return (a1 - b1);
  }
}

void gasnete_coll_team_init(gasnet_team_handle_t team, 
                            uint32_t team_id, 
                            uint32_t total_ranks,
                            gex_Rank_t myrank,
                            gex_Rank_t *rel2act_map,
                            gasnet_seginfo_t *scratch_segs,
                            const gasnet_image_t images[] GASNETI_THREAD_FARG)
{
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_init: team %p, team_id %x, total_ranks %u, myrank %u\n", 
          team, team_id, total_ranks, myrank);
  fflush(stderr);
  if (myrank == 0) {
    PRINT_ARRAY(stderr, rel2act_map, total_ranks, "%u");
    fflush(stderr);
  }
#endif
  
  gex_Rank_t *supernodes = NULL;
  uint32_t i;
  initialize_team_fields(team, images, myrank, total_ranks, scratch_segs GASNETI_THREAD_PASS); 
  team->team_id = team_id;
  team->total_ranks = total_ranks;
  team->myrank = myrank;

  /* Build rel2act map (unless already constructed) */
  if (team->rel2act_map == NULL) {
    size_t alloc_size = total_ranks * sizeof(gex_Rank_t);
    team->rel2act_map = (gex_Rank_t *)gasneti_malloc(alloc_size);
    memcpy(team->rel2act_map, rel2act_map, alloc_size);
  }

  /* Build peer lists (unless already constructed) */
  if (total_ranks > 1 && !team->peers.num) {
    unsigned int count = 0;
    for (i=1; i<total_ranks; i*=2) ++count;
    team->peers.num = count;
    team->peers.fwd = gasneti_malloc(sizeof(gex_Rank_t) * count);
    for (i=0; i<count; i++) {
      unsigned int dist = 1 << i;
      team->peers.fwd[i] = rel2act_map[(myrank + dist) % total_ranks];
    }
  }

#if GASNET_PSHM
  /* Build supernode stats (unless already constructed) */
  if (!team->supernode.node_count) {
    gex_Rank_t *node_vector;
    int count, rank;

    /* A list with a representative for each supernode (needed by some barriers) */
    supernodes = gasneti_malloc(gasneti_nodemap_global_count * sizeof(gex_Rank_t));

    /* Created a sorted vector of (supernode,node) for members of this team
     * while finding size of and rank in local supernode in the same pass
     */
    count = 0; rank = -1;
    node_vector = gasneti_malloc(2 * total_ranks * sizeof(gex_Rank_t));
    for (i = 0; i < total_ranks; ++i) {
      gex_Rank_t n = rel2act_map[i];
      if (gasneti_pshm_jobrank_in_supernode(n)) {
        if (n == gasneti_mynode) rank = count;
        ++count;
      }
      node_vector[2*i+0] = gasneti_node2supernode(n);
      node_vector[2*i+1] = n;
    }
    qsort(node_vector, total_ranks, 2*sizeof(gex_Rank_t), &gasnete_node_pair_sort_fn);

    gasneti_assert((count >  0) && (count <= gasneti_nodemap_local_count));
    gasneti_assert((rank  >= 0) && (rank  <  gasneti_nodemap_local_count));
    team->supernode.node_count = count;
    team->supernode.node_rank  = rank;

    /* Count and enumerate unique supernodes and find my supernode's rank */
    count = 1; rank = 0;
    supernodes[0] = node_vector[1];
    for (i = 1; i < total_ranks; ++i) {
      if (node_vector[2*i] != node_vector[2*(i-1)]) {
        if (node_vector[2*i] == gasneti_nodemap_global_rank) rank = count;
        supernodes[count] = node_vector[2*i+1];
        ++count;
      }
    }
    gasneti_free(node_vector);

    gasneti_assert((count >  0) && (count <= gasneti_nodemap_global_count));
    gasneti_assert((rank  >= 0) && (rank  <  gasneti_nodemap_global_count));
    team->supernode.grp_count = count;
    team->supernode.grp_rank  = rank;

    /* Construct a list of log(P) representatives at distance +2^i */
    /* NOTE: 'count' and 'rank' are in the supernode space */
    {
      unsigned int len = 0;
      for (i=1; i<count; i*=2) ++len;
      team->supernode_peers.num = len;
      team->supernode_peers.fwd = gasneti_malloc(sizeof(gex_Rank_t) * len);
      for (i=0; i<len; i++) {
        unsigned int dist = 1 << i;
        team->supernode_peers.fwd[i] = supernodes[(rank + dist) % count];
      }
    }
  }
#endif

  /* lock the team directory (team_dir) */
  /* add the new team to the directory */
  if (team_dir == NULL) {
    team_dir = gasnete_hashtable_create(TEAM_DIR_SIZE);
    gasneti_assert(team_dir != NULL);
  }
  gasnete_hashtable_insert(team_dir, team_id, team);

#ifdef gasnete_coll_team_init_conduit
  /* conduit specific initialization for gasnet teams */
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_init: calling gasnete_coll_team_init_conduit.\n");
  fflush(stderr);
#endif
  gasnete_coll_team_init_conduit(team);
#endif
  /* unlock */

  if (team != GASNET_TEAM_ALL) {
    gasnete_coll_barrier_init(team, GASNETE_COLL_BARRIER_ENVDEFAULT,
                              rel2act_map, supernodes);
  }

#if GASNET_PSHM
  gasneti_free(supernodes);
#endif
}

void gasnete_coll_team_fini(gasnet_team_handle_t team)
{
  int i;
  gasneti_assert(team != NULL);
  /* free data members of the team, such as scratch space and etc. */
  gasneti_free(team->rel2act_map);
  gasneti_free(team->peers.fwd);
#if GASNET_PSHM
  gasneti_free(team->supernode_peers.fwd);
#endif

  gasneti_assert(team_dir != NULL);
  gasnete_hashtable_remove(team_dir, team->team_id, NULL);

#if !defined(GASNETE_COLL_P2P_OVERRIDE) && GASNET_DEBUG
  for (i = 0; i < GASNETE_COLL_P2P_TABLE_SIZE; ++i) {
    /* Check that table is actually empty */
    gasneti_assert(team->p2p_table[i] == NULL);
  }
#endif

#ifdef gasnete_coll_team_fini_conduit
  /* conduit specific initialization for gasnet teams */
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_fini: calling gasnete_coll_team_fini_conduit.\n");
  fflush(stderr);
#endif
  gasnete_coll_team_fini_conduit(team);
#endif
}

void gasnete_coll_teamid_reqh(gex_Token_t token,
                              gex_AM_Arg_t team_id)
{
  new_team_id=(uint32_t)team_id;
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_teamid_reqh: new_team_id %x\n", new_team_id);
  fflush(stderr);
#endif
}

/* collective function that should be called by all participating nodes */
gasnet_team_handle_t gasnete_coll_team_create(uint32_t total_ranks,
                                              gex_Rank_t myrank,
                                              gex_Rank_t *rel2act_map, gasnet_seginfo_t* scratch_segs GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team;
  gex_Rank_t team_lead = rel2act_map[0];
  uint32_t i;
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_create: team_lead %u, total_ranks %u, myrank %u\n", team_lead, total_ranks, myrank);
  fflush(stderr);
  if (myrank == 0) {
    PRINT_ARRAY(stderr, rel2act_map, total_ranks, "%u");
    fflush(stderr);
  }
#endif

  /* need to lock for thread safety */

  if (myrank == 0) {
    /* the team leader (rank 0) computes the new team_id */
    /* gasneti_atomic_increment(&(my_team_seq), GASNETI_ATOMIC_NONE); */
    my_team_seq++; /* need to be an atomic operation */
    /* limitation: each root node can only allocate team sequence id
       4096 times */
    gasneti_assert(my_team_seq < 0xfff);
    new_team_id = ((team_lead << 12) | (my_team_seq & 0xfff));
    
    /* send out team_id */
    for(i=1; i<total_ranks; i++) {
      gex_AM_RequestShort(gasneti_THUNK_TM, rel2act_map[i],
                                  gasneti_handleridx(gasnete_coll_teamid_reqh), 0,
                                  new_team_id);
    }
  } else {
    /* wait for team_id from the team leader */
    while (new_team_id == 0)
      gasneti_AMPoll();

#ifdef DEBUG_TEAM
    fprintf(stderr, "myrank %u, get new_team_id %x\n", myrank, new_team_id);
    fflush(stderr);
#endif
  }

  /* create the team locally */
  team = (gasnet_team_handle_t)gasneti_calloc(1,sizeof(struct gasnete_coll_team_t_));
  gasnete_coll_team_init(team, new_team_id, total_ranks, myrank, rel2act_map, scratch_segs, NULL GASNETI_THREAD_PASS);
  new_team_id = 0;
  
  /* unlock */
#ifdef DEBUG_TEAM
  gasnete_print_team(team, stderr);
#endif

  return team;
}

void gasnete_coll_team_free(gasnet_team_handle_t team)
{
  gasneti_assert(team != NULL);
  gasnete_coll_team_fini(team);
  gasneti_free(team);
}

typedef struct {
  int relrank;
  gex_Rank_t parent_rank;
} gasnete_coll_split_sort_t;

int gasnete_coll_split_sort_fn(const void *x, const void *y) {
  const gasnete_coll_split_sort_t *a = x;
  const gasnete_coll_split_sort_t *b = y;
  // Note that parent_ranks are distinct (never equal)
  if (a->relrank == b->relrank) return (a->parent_rank < b->parent_rank) ? -1 : 1;
  else return (a->relrank < b->relrank) ? -1 : 1;
}

gasnet_team_handle_t gasnete_coll_team_split(gasnet_team_handle_t team,
                                             int mycolor,
                                             int myrelrank,
                                             void *clientdata
                                             GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t newteam;
  gex_Rank_t new_total_ranks;
  gex_Rank_t new_myrank = GEX_RANK_INVALID;
  gex_Rank_t *rel2act_map;
  gasnet_seginfo_t *segments;
  gex_Rank_t i, j;
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_split: team rank %u, parent team handle %p, mycolor %u, myrank %u\n",
          team->myrank, team, mycolor, myrelrank);
  fflush(stderr);
#endif

  /* collect the arguments */
  struct {
    gex_Rank_t color;
    gex_Rank_t relrank;
    gasnet_seginfo_t segment;
  } my_args, *all_args;
  my_args.color = mycolor;
  my_args.relrank = myrelrank;
  my_args.segment = *(gasnet_seginfo_t*)clientdata;
  all_args = gasneti_malloc(sizeof(my_args) * team->total_ranks);
  gasnet_coll_gather_all(team, all_args, &my_args, sizeof(my_args),
                         GASNET_COLL_LOCAL|GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);

  /* short-circuit if excluded */
  if (mycolor == -1) {
    gasneti_free(all_args);
    gasnete_coll_consensus_barrier(team GASNETI_THREAD_PASS);
    gasnete_coll_consensus_barrier(team GASNETI_THREAD_PASS);
    return NULL;
  }

  /* pass 1: just count */
  new_total_ranks = 0;
  for (i=0; i<team->total_ranks; i++) {
    new_total_ranks += (mycolor == all_args[i].color);
  }

  /* pass 2: collect and rank the members */
  gasnete_coll_split_sort_t *members = gasneti_malloc(new_total_ranks*sizeof(gasnete_coll_split_sort_t));
  for (i=j=0; i<team->total_ranks; i++) {
    if (mycolor == all_args[i].color) {
      members[j].parent_rank = i;
      members[j].relrank = all_args[i].relrank;
      j += 1;
    }
  }
  gasneti_assert(j == new_total_ranks);
  qsort(members, new_total_ranks, sizeof(gasnete_coll_split_sort_t), &gasnete_coll_split_sort_fn);

  /* pass 3: collect jobrank and segments of sorted members */
  rel2act_map = (gex_Rank_t *)gasneti_malloc(new_total_ranks*sizeof(gex_Rank_t));
  segments = (gasnet_seginfo_t *)gasneti_malloc(new_total_ranks*sizeof(gasnet_seginfo_t));
  for (i=0; i < new_total_ranks; i++) {
    j = members[i].parent_rank;
    if (j == team->myrank) new_myrank = i;
    rel2act_map[i] = team->rel2act_map[j];
    segments[i] = all_args[j].segment;
  }
  gasneti_assert(new_myrank != GEX_RANK_INVALID);
  gasneti_free(members);
  gasneti_free(all_args);
  
  /* It would be better to add some sanity check for team correctness here. */
  
  /* create a team */
  new_team_id = 0;
  gasnete_coll_consensus_barrier(team GASNETI_THREAD_PASS);

#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_split: new_total_ranks %u, new_myrank %u.\n",
          new_total_ranks, new_myrank);
  PRINT_ARRAY(stderr, rel2act_map, new_total_ranks, "%u");
  fflush(stderr);
#endif

  newteam = gasnete_coll_team_create(new_total_ranks, new_myrank, rel2act_map, segments GASNETI_THREAD_PASS);
  
  gasneti_free(rel2act_map);
  gasnete_coll_consensus_barrier(team GASNETI_THREAD_PASS);
  return newteam;
}

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id) 
{
  gasnet_team_handle_t team;
  
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_lookup: team_id %x\n", team_id);
  fflush(stderr);
#endif

  if (team_id == 0) {
    team = GASNET_TEAM_ALL;
  } else {
    if (gasnete_hashtable_search(team_dir, team_id, (void **)&team))
      team = NULL; /* cannot find team_id the hash table */
  }
  
  return team;
}

void gasnete_print_team(gasnet_team_handle_t team, FILE *fp)
{
  int i;
  fprintf(fp, "team id %x, total ranks %u, my rank %u\n",
          (unsigned int)team->team_id, (unsigned int)team->total_ranks, (unsigned int)team->myrank);
  fprintf(fp, "rel2act_map:\n");
  for (i=0; i<team->total_ranks; i++) {
    fprintf(fp, "%u -> %u\n", i, (unsigned int)team->rel2act_map[i]);
  }
  fflush(fp);
}
