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

static 
gasnete_hashtable_t *team_dir = NULL;

#define GASNETE_COLL_TEAM_ID_BITS   12
#define GASNETE_COLL_TEAM_ID_COUNT  (1<<GASNETE_COLL_TEAM_ID_BITS)
#define GASNETE_COLL_TEAM_ID_MASK   (GASNETE_COLL_TEAM_ID_COUNT-1)
static struct {
  gasneti_weakatomic32_t map[GASNETE_COLL_TEAM_ID_COUNT/32];
  unsigned int hint;
} team_id = { { gasneti_weakatomic32_init(1), }, };

// Allocate per-leader portion of team_id
static uint32_t team_id_alloc(void) {
  const unsigned int count = GASNETE_COLL_TEAM_ID_COUNT/32;
  uint32_t readval, newval;
  unsigned int word, bit;
  do {
    // Scan all words (with wrap) starting from the word numbered `hint`
    // Performing mod here ensures valid value regardless of write races
    word = team_id.hint % count;
    unsigned int i;
    for (i = 0; i < count; ++i, word = (word+1) % count) {
      readval = gasneti_weakatomic32_read(&team_id.map[word], 0);
      bit = gasnete_coll_ctz_u32(~readval);
      if (bit == 32) continue; // no free bits - advance to next word, if any
      break;
    }
    if_pf (i == count) { // failure due to full map
      gasneti_fatalerror("Exhausted %d concurrent teams with leader %d",
                         GASNETE_COLL_TEAM_ID_COUNT, gasneti_mynode);
    }
    newval = readval ^ (1 << bit);
  } while (!gasneti_weakatomic32_compare_and_swap(&team_id.map[word], readval, newval, 0));
  team_id.hint = word + (bit == 31); // mod done when read
  uint32_t result = bit + word * 32;
  gasneti_assert(result < GASNETE_COLL_TEAM_ID_COUNT); // redundant. (i == count) should already catch
  return result;
}

static void team_id_free(uint32_t id) {
  if (gasneti_mynode != (id >> GASNETE_COLL_TEAM_ID_BITS)) return;
  id &= GASNETE_COLL_TEAM_ID_MASK;
  unsigned int word = id / 32;
  unsigned int bit = id % 32;
  gasneti_weakatomic32_t *p = &team_id.map[word];
  uint32_t readval, newval;
  do { // Atomic clear
    readval = gasneti_weakatomic32_read(p, 0);
    gasneti_assert(readval & (1 << bit));
    newval = readval ^ (1 << bit);
  } while (!gasneti_weakatomic32_compare_and_swap(p, readval, newval, 0));
  // If allocation hint is word we've written, advance to increase distance to reuse
  if (team_id.hint == word) team_id.hint = word + 1; // mod done when read
}

extern size_t gasnete_coll_auxseg_size;
extern size_t gasnete_coll_auxseg_offset;

/*called by only one thread*/
static void initialize_team_fields(
                        gasnete_coll_team_t team,  
                        size_t scratch_size,       // Single-valued
                        gex_Addr_t *scratch_addrs,
                        gex_Flags_t flags
                        GASNETI_THREAD_FARG)
{
  team->sequence = 0xfffffff8;  // Intentionally near to wrap-around

  if (team->team_id) {
    // Detect and optimize for storage in symmetric offset case
    uintptr_t symmetric_offset = 0;
    int is_symmetric = 0;
    if (scratch_size && (team->total_ranks > 1)) {
      const gasnet_seginfo_t *si = gasneti_seginfo + team->rel2act_map[0];
      symmetric_offset = (uintptr_t)scratch_addrs[0] - (uintptr_t)(si->addr);
      if (symmetric_offset < si->size) {
        is_symmetric = 1;
        for (gex_Rank_t r = 1; r < team->total_ranks; ++r) {
          si = gasneti_seginfo + team->rel2act_map[r];
          if (symmetric_offset != (uintptr_t)scratch_addrs[r] - (uintptr_t)(si->addr)) {
            is_symmetric = 0;
            break;
          }
        }
      }
    }

    team->scratch_size = scratch_size;
    if (is_symmetric) {
      team->scratch_segs = gasneti_seginfo;
      team->symmetric_scratch_offset = symmetric_offset;
      gasneti_assert(! team->scratch_addrs);
      gasneti_free(scratch_addrs);
      GASNETI_TRACE_PRINTF(W,("Team TM%x:%i scratch: size=%"PRIuSZ" symmetric_offset=%"PRIuPTR,
                              team->team_id, team->myrank, scratch_size, symmetric_offset));
    } else {
      team->scratch_addrs = scratch_addrs;
      gasneti_assert(! team->scratch_segs);
      gasneti_assert(! team->symmetric_scratch_offset);
      GASNETI_TRACE_PRINTF(W,("Team TM%x:%i scratch: size=%"PRIuSZ" non-symmetric",
                              team->team_id, team->myrank, scratch_size));
    }
  } else {
    gasneti_assert(team == GASNET_TEAM_ALL);
    gasneti_assert(gasnete_coll_auxseg_size);
    team->scratch_size = gasnete_coll_auxseg_size;
    gasneti_assert(! team->scratch_addrs);
    team->scratch_segs = gasneti_seginfo_aux;
    team->symmetric_scratch_offset = gasnete_coll_auxseg_offset;
    gasneti_assert(! team->rel2act_map);
    GASNETI_TRACE_PRINTF(W,("Team TM0:%i scratch: size=%"PRIuSZ" symmetric_offset=%"PRIuPTR" (auxseg)",
                            gasneti_mynode, scratch_size, gasnete_coll_auxseg_offset));
  }
  team->myscratch = team->scratch_size ? (void *)gasnete_coll_scratch_base(team, team->myrank) : NULL;

#if GASNET_PAR && GASNET_DEBUG
  gasneti_mutex_init(&team->threads_mutex);
#endif
#if GASNET_DEBUG
  gasneti_mutex_init(&team->barrier_lock);
#endif
  
  team->tree_geom_cache_head = NULL;
  team->tree_geom_cache_tail = NULL;
  gasneti_mutex_init(&team->tree_geom_cache_lock);
  team->tree_construction_scratch = NULL;
  team->dissem_cache_head = NULL;
  team->dissem_cache_tail = NULL;
  gasneti_mutex_init(&team->dissem_cache_lock);
  team->autotune_info = gasnete_coll_autotune_init(team GASNETI_THREAD_PASS);
  team->consensus_id = team->consensus_issued_id = 0xfffffff8;  // Intentionally near to wrap-around
  gasnete_coll_alloc_new_scratch_status(team);
  team->scratch_free_list = NULL;
  gex_HSL_Init(&team->child.lock);
  gex_HSL_Init(&team->rexchgv.lock);
  
#ifndef GASNETE_COLL_P2P_OVERRIDE
  gex_HSL_Init(&team->p2p_lock);
  team->p2p_freelist = NULL;
  for (int i = 0; i < GASNETE_COLL_P2P_TABLE_SIZE; ++i) {
    team->p2p_table[i] = NULL;
  }
  team->p2p_eager_buffersz = MAX(gasnete_coll_p2p_eager_min, team->total_ranks * gasnete_coll_p2p_eager_scale);
#endif
}

/* Helper for gasnete_coll_team_alloc() */
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
                            size_t scratch_size,
                            gex_Addr_t *scratch_addrs,
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG)
{
  initialize_team_fields(team, scratch_size, scratch_addrs, flags GASNETI_THREAD_PASS); 

  /* lock the team directory (team_dir) */
  /* add the new team to the directory */
  if (team_dir == NULL) {
    team_dir = gasnete_hashtable_create(TEAM_DIR_SIZE);
    gasneti_assert(team_dir != NULL);
  }
  gasnete_hashtable_insert(team_dir, team->team_id, team);

#ifdef gasnete_coll_team_init_conduit
  /* conduit specific initialization for gasnet teams */
  gasnete_coll_team_init_conduit(team);
#endif
  /* unlock */

  if (team->team_id) {
    gasnete_coll_barrier_init(team, GASNETE_COLL_BARRIER_ENVDEFAULT);
  }
}

void gasnete_coll_team_fini(gasnet_team_handle_t team)
{
  gasneti_assert(team != NULL);

  team_id_free(team->team_id);

  /* free data members of the team, such as scratch space and etc. */
  gasneti_free(team->rel2act_map);
  gasneti_free(team->peers.fwd);
#if GASNET_PSHM
  gasneti_free(team->supernode_peers.fwd);
#endif
  gasneti_free(team->scratch_addrs);

  gasnete_coll_autotune_free(team);
  gasnete_coll_free_scratch_status(team);
  gasnete_coll_local_tree_geom_purge(team);
  gasnete_coll_scratch_req_purge(team);
  gasnete_coll_p2p_purge(team);

  if (team->barrier_fini) team->barrier_fini(team);

  gasneti_assert(team_dir != NULL);
  gasnete_hashtable_remove(team_dir, team->team_id, NULL);

#if !defined(GASNETE_COLL_P2P_OVERRIDE) && GASNET_DEBUG
  for (int i = 0; i < GASNETE_COLL_P2P_TABLE_SIZE; ++i) {
    /* Check that table is actually empty */
    gasneti_assert(team->p2p_table[i] == NULL);
  }
#endif

#ifdef gasnete_coll_team_fini_conduit
  /* conduit specific initialization for gasnet teams */
  gasnete_coll_team_fini_conduit(team);
#endif
}

// Non-collective call to allocate local data and initialize some key fields
gasnet_team_handle_t gasnete_coll_team_alloc(
                        gex_Rank_t total_ranks,
                        gex_Rank_t myrank,
                        gex_Rank_t *rank_map)
{
  gasnet_team_handle_t team = gasneti_calloc(1,sizeof(struct gasnete_coll_team_t_));

  team->myrank = myrank;
  team->total_ranks = total_ranks;
  team->rel2act_map = rank_map;

  // Build peer lists
  if (total_ranks > 1) {
    unsigned int count = 0;
    for (gex_Rank_t i=1; i<total_ranks; i*=2) ++count;
    team->peers.num = count;
    team->peers.fwd = gasneti_malloc(sizeof(gex_Rank_t) * count * 2);
    team->peers.bwd = team->peers.fwd + count;
    for (gex_Rank_t i=0; i<count; i++) {
      unsigned int dist = 1 << i;
      team->peers.fwd[i] = rank_map[(myrank + dist) % total_ranks];
      team->peers.bwd[i] = rank_map[(myrank + total_ranks - dist) % total_ranks];
    }
  }

#if GASNET_PSHM
  // Build supernode stats
  {
    gex_Rank_t *node_vector, *supernodes;
    int count, rank;

    // A list with a representative for each supernode (for hierarchical comms)
    supernodes = gasneti_malloc(gasneti_nodemap_global_count * sizeof(gex_Rank_t));

    /* Created a sorted vector of (supernode,node) for members of this team
     * while finding size of and rank in local supernode in the same pass
     */
    count = 0; rank = -1;
    node_vector = gasneti_malloc(2 * total_ranks * sizeof(gex_Rank_t));
    for (gex_Rank_t i = 0; i < total_ranks; ++i) {
      gex_Rank_t n = rank_map[i];
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
    for (gex_Rank_t i = 1; i < total_ranks; ++i) {
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
      for (gex_Rank_t i=1; i<count; i*=2) ++len;
      team->supernode_peers.num = len;
      team->supernode_peers.fwd = gasneti_malloc(sizeof(gex_Rank_t) * len);
      for (gex_Rank_t i=0; i<len; i++) {
        unsigned int dist = 1 << i;
        team->supernode_peers.fwd[i] = supernodes[(rank + dist) % count];
      }
    }

    gasneti_free(supernodes);
  }
#endif

  return team;
}

// collective function that should be called by all ranks in the output team
// Consumes the rank_map
// Provides synchronization sufficient for use of output team
// Requires a parent-scoped entry barrier due to use of "subteam" collectives
gasnet_team_handle_t gasnete_coll_team_create(
                        gasnet_team_handle_t parent,
                        gex_Rank_t total_ranks,
                        gex_Rank_t myrank,
                        gex_Rank_t *rank_map,
                        size_t scratch_size,
                        gex_Addr_t *scratch_addrs,
                        gex_Flags_t flags
                        GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team = gasnete_coll_team_alloc(total_ranks, myrank, rank_map);

  gasnete_subteam_Init(parent, team GASNETI_THREAD_PASS);

  // Allocatate/communicate the new team's ID
  gasnete_subteam_ID(team GASNETI_THREAD_PASS);

  // Construct global scratch_addrs[] array
  gex_Addr_t *global_scratch_addrs = NULL;
  gex_Flags_t scratch_mask = GEX_FLAG_TM_GLOBAL_SCRATCH    |
                             GEX_FLAG_TM_LOCAL_SCRATCH     |
                             GEX_FLAG_TM_SYMMETRIC_SCRATCH |
                             GEX_FLAG_TM_NO_SCRATCH;
  switch (flags & scratch_mask) {
    case GEX_FLAG_TM_GLOBAL_SCRATCH:
      global_scratch_addrs = gasneti_malloc(total_ranks * sizeof(gex_Addr_t));
      memcpy(global_scratch_addrs, scratch_addrs, total_ranks * sizeof(gex_Addr_t));
      break;

    case GEX_FLAG_TM_LOCAL_SCRATCH:
      global_scratch_addrs = gasneti_malloc(total_ranks * sizeof(gex_Addr_t));
      gasnete_subteam_Exchange(team,
                               scratch_addrs, sizeof(gex_Addr_t), global_scratch_addrs
                               GASNETI_THREAD_PASS);
      break;

    case GEX_FLAG_TM_SYMMETRIC_SCRATCH:
      // TODO-EX: when implementing GEX_FLAG_SCRATCH_SEG_OFFSET, do not instatate an array
      global_scratch_addrs = gasneti_malloc(total_ranks * sizeof(gex_Addr_t));
      for (gex_Rank_t r = 0; r < total_ranks; ++r) {
        global_scratch_addrs[r] = scratch_addrs[0];
      }
      break;

    case GEX_FLAG_TM_NO_SCRATCH:
      scratch_size = 0;
      break;

    case 0:
      gasneti_fatalerror("No GEX_FLAG_TM_*_SCRATCH flags provided");
      break;

    default:
      gasneti_fatalerror("Multiple GEX_FLAG_TM_*_SCRATCH flags provided");
      break;
  }

#if GASNET_DEBUG
  // Verify single-valued scratch_size
  size_t leader_scratch_size = myrank ? 0xcafef00d : scratch_size;
  gasnete_subteam_Broadcast(team,
                            &leader_scratch_size, sizeof(leader_scratch_size)
                            GASNETI_THREAD_PASS);
  gasneti_assert_uint(scratch_size ,==, leader_scratch_size);
#endif

  gasnete_coll_team_init(team, scratch_size, global_scratch_addrs, flags GASNETI_THREAD_PASS);

  gasnete_subteam_Fini(team, 1 GASNETI_THREAD_PASS);

  return team;
}

int gasnete_coll_team_free(gasnet_team_handle_t team, gex_Memvec_t *scratch_p)
{
  int result = 0;
  gasneti_assert(team != NULL);
  if (scratch_p && team->scratch_size) { // TODO: distinguish scratch allocated by GASNet
    result = 1;
    scratch_p->gex_addr = team->myscratch;
    scratch_p->gex_len  = team->scratch_size;
  }
  gasnete_coll_team_fini(team);
  gasneti_free(team);
  return result;
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

gasnet_team_handle_t gasnete_coll_team_split(gasnet_team_handle_t parent,
                                             int mycolor,
                                             int myrelrank,
                                             size_t scratch_size,
                                             gex_Addr_t scratch_addr,
                                             gex_Flags_t flags
                                             GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t newteam;
  gex_Rank_t new_total_ranks;
  gex_Rank_t new_myrank = GEX_RANK_INVALID;
  gex_Rank_t i, j;
  
  /* collect the arguments */
  struct {
    gex_Rank_t color;
    gex_Rank_t relrank;
  } my_args, *all_args;
  my_args.color = mycolor;
  my_args.relrank = myrelrank;
  all_args = gasneti_malloc(sizeof(my_args) * parent->total_ranks);
  gasnet_coll_gather_all(parent, all_args, &my_args, sizeof(my_args),
                         GASNET_COLL_LOCAL|GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);

  /* short-circuit if excluded */
  if (mycolor == -1) {
    gasneti_free(all_args);
    return NULL;
  }

  /* pass 1: just count */
  new_total_ranks = 0;
  for (i=0; i<parent->total_ranks; i++) {
    new_total_ranks += (mycolor == all_args[i].color);
  }

  /* pass 2: collect and rank the members */
  gasnete_coll_split_sort_t *members = gasneti_malloc(new_total_ranks*sizeof(gasnete_coll_split_sort_t));
  for (i=j=0; i<parent->total_ranks; i++) {
    if (mycolor == all_args[i].color) {
      members[j].parent_rank = i;
      members[j].relrank = all_args[i].relrank;
      j += 1;
    }
  }
  gasneti_assert(j == new_total_ranks);
  qsort(members, new_total_ranks, sizeof(gasnete_coll_split_sort_t), &gasnete_coll_split_sort_fn);

  // pass 3: collect jobranks of sorted members
  gex_Rank_t *rank_map = (gex_Rank_t *)gasneti_malloc(new_total_ranks*sizeof(gex_Rank_t));
  for (i=0; i < new_total_ranks; i++) {
    j = members[i].parent_rank;
    if (j == parent->myrank) new_myrank = i;
    rank_map[i] = parent->team_id ? parent->rel2act_map[j] : j;
  }
  gasneti_assert(new_myrank != GEX_RANK_INVALID);
  gasneti_free(members);
  gasneti_free(all_args);

  /* It would be better to add some sanity check for team correctness here. */
  
  /* create a team */
  newteam = gasnete_coll_team_create(parent, new_total_ranks, new_myrank, rank_map,
                                     scratch_size, &scratch_addr, flags GASNETI_THREAD_PASS);
  
  return newteam;
}

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id) 
{
  gasnet_team_handle_t team;
  
  if (team_id == 0) {
    team = GASNET_TEAM_ALL;
  } else {
    if (gasnete_hashtable_search(team_dir, team_id, (void **)&team)) {
      // did not find team_id in the hash table
      gasneti_fatalerror("Collective operation on invalid (destroyed?) TM%x", (unsigned int)team_id);
    }
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

/* ------------------------------------------------------------------------------------ */
// AM-based Collective Operations over a SUBSET of an existing "parent" team
// Used in team construction
//
// Use must follow the pattern:
//  1. Call gasnete_subteam_Init(parent, child)
//     Preconditions (satisfied in any order):
//     A. There must be a barrier synchronization over the parent team since the
//        previous use of gasnete_subteam_Fini().  In the case of gex_TM_Spilt(),
//        the GatherAll of (color,rel_rank) is sufficient.
//  2. Call zero or more gasnete_subteam_*(child, ...) functions
//     Use the collectives as needed to construct the team.
//  3. Call gasnete_subteam_Fini(child, do_sync)
//     A non-zero value of 'do_sync' argument requests a barrier synchronization
//     to prevent use of the constructed child team prior to global completion
//     of its construction.  This may be omitted if such synchronization is
//     provided by other means.
//
//  All steps above are collective over the members of the child team.
//  However no such restriction exists over the parent team.
//
// TODO-EX: multi-EP, especially Create using EPs not in parent
// TODO-EX: remove THUNK_TM via gex_TM_Pair()
// TODO-EX: revisit use of phase[2] vs sequence numbers (or similar)
// TODO-EX: seek to remove requirement for an entry barrier over parent_tm
// TODO-EX: determine if one can remove memory fences from start/end of initiators

// BEGIN use of subteam collectives over a given parent
void gasnete_subteam_Init(
                        gasnete_coll_team_t parent,
                        gasnete_coll_team_t child
                        GASNETI_THREAD_FARG)
{
  gasneti_assert(child->early_parent == NULL);
  gasneti_assert(parent->child.phase == 0);
  child->early_parent = parent;
}

// END use of subteam collectives
// May include optional "exit barrier" over the child team
void gasnete_subteam_Fini(
                        gasnete_coll_team_t child,
                        int do_sync
                        GASNETI_THREAD_FARG)
{
  gasneti_assert(child->early_parent);
  if (do_sync) gasnete_subteam_Barrier(child GASNETI_THREAD_PASS);
  child->early_parent->child.phase = 0;
  child->early_parent = NULL;
}

static uint8_t*
gasnete_subteam_op_data(gasnete_coll_team_t parent, int phase, size_t size) {
  uint8_t *data = parent->child.data[phase];
  if_pf (! data) {
    gex_HSL_Lock(&parent->child.lock);
    data = parent->child.data[phase];
    if (! data) {
      data = gasneti_malloc(size);
      parent->child.data[phase] = data;
    }
    gex_HSL_Unlock(&parent->child.lock);
  }
  return data;
}

enum {
  gasnete_subteam_op_barrier,
  gasnete_subteam_op_broadcast,
  gasnete_subteam_op_exchange,
  gasnete_subteam_op_team_id,
};

#define GASNETE_SUBTEAM_OP_ARG0(op,phase,step,seq) \
  (gasnete_subteam_op_##op | ((phase)<<2) | ((step)<<3) | ((seq)<<8))

void gasnete_subteam_op_reqh(
                        gex_Token_t token, void *buf, size_t nbytes,
                        gex_AM_Arg_t parent_id, gex_AM_Arg_t nranks,
                        gex_AM_Arg_t arg0, gex_AM_Arg_t arg1)
{
    const int op    =  arg0       & 0x3;
    const int phase = (arg0 >> 2) & 1;
    const int step  = (arg0 >> 3) & 0x1f; // Max 2^5 steps            => 2^32 ranks
    const int seq   = (arg0 >> 8);        // Max 2^24 fragments * 512 => 8GB (and largest msg is elemsz*nranks/2)
    const int distance = (1 << step);
    gasneti_assert_uint(distance ,<, nranks);

    gasnete_coll_team_t parent = gasnete_coll_team_lookup(parent_id);
    switch (op) {
      case gasnete_subteam_op_barrier:
        // arg1 is unused
        gasneti_assert_uint(nbytes ,==, 0);
        gasneti_assert_int((int)gasneti_weakatomic32_read(&parent->child.step_rcvd[phase][step], 0) ,==, 0);
        gasneti_weakatomic32_set(&parent->child.step_rcvd[phase][step], 1, 0);
        break;

      case gasnete_subteam_op_broadcast: {
        // arg1 is size of one element
        uint8_t *data = gasnete_subteam_op_data(parent, phase, arg1);
        if (nbytes) { // not every message carries data.  some are synchronization
          gasneti_assert(nbytes == arg1);
          GASNETI_MEMCPY(data, buf, nbytes);
        }
        gasneti_assert_int((int)gasneti_weakatomic32_read(&parent->child.step_rcvd[phase][step], 0) ,==, 0);
        gasneti_weakatomic32_set(&parent->child.step_rcvd[phase][step], 1, GASNETI_ATOMIC_REL);
        break;
      }

      case gasnete_subteam_op_exchange: {
        // arg1 is size of one element
        uint8_t *data = gasnete_subteam_op_data(parent, phase, nranks*arg1);
        uint8_t *dest = data + (arg1 * distance) + (seq * gex_AM_LUBRequestMedium());
        gasneti_assert_ptr(dest + nbytes ,<=, data + arg1 * nranks);
        GASNETI_MEMCPY(dest, buf, nbytes);
        gasneti_weakatomic32_increment(&parent->child.step_rcvd[phase][step], GASNETI_ATOMIC_REL);
        break;
      }

      case gasnete_subteam_op_team_id: {
        // arg1 is the (non-zero) new team id
        gasneti_assert_uint(nbytes ,==, 0);
        gasneti_assert_uint(arg1 ,!=, 0);
        gasneti_assert_int((int)gasneti_weakatomic32_read(&parent->child.team_id, 0) ,==, 0);
        gasneti_weakatomic32_set(&parent->child.team_id, arg1, 0);
        break;
      }

      default: gasneti_unreachable_error(("Unknown op: %i",op));
    }
}

void gasnete_subteam_Barrier(
                        gasnete_coll_team_t child
                        GASNETI_THREAD_FARG)
{
  gasnete_coll_team_t parent = child->early_parent;
  gasneti_assert(parent != NULL);
  const int phase = parent->child.phase;
  gex_Rank_t nranks = child->total_ranks;
  gex_Rank_t myrank = child->myrank;
  gasneti_sync_reads();

  if (nranks == 1) goto out;

  // Dissemination barrier
  for (unsigned int step = 0, distance = 1; distance < nranks; ++step, distance *= 2) {
    gex_Rank_t jobrank = child->peers.fwd[step];

    // Send payload using 0-byte AMMedium due to sharing single handler
    gex_AM_Arg_t arg0 = GASNETE_SUBTEAM_OP_ARG0(barrier, phase, step, 0);
    gex_AM_RequestMedium(gasneti_THUNK_TM, jobrank, _hidx_gasnete_subteam_op_reqh,
                         NULL, 0, GEX_EVENT_NOW, 0,
                         parent->team_id, nranks, arg0, 0);

    // Poll until we have received for the current step
    GASNET_BLOCKUNTIL((int)gasneti_weakatomic32_read(&parent->child.step_rcvd[phase][step], 0) == 1);
    gasneti_weakatomic32_set(&parent->child.step_rcvd[phase][step], 0, 0); // reset
  }

out:
  gasneti_sync_writes();
  parent->child.phase ^= 1;
}

// Root is always rank 0
// In-place on root (and thus a single pointer argument)
//
// This implementation provides a full barrier synchronization required for the
// use of phase[], but this does NOT ensure all ranks have received the
// broadcast prior to return on any given ranks.
void gasnete_subteam_Broadcast(
                        gasnete_coll_team_t child,
                        void *ptr, size_t elemsz
                        GASNETI_THREAD_FARG)
{
  gasneti_assert_uint(elemsz ,<=, gex_AM_LUBRequestMedium());

  gasnete_coll_team_t parent = child->early_parent;
  gasneti_assert(parent != NULL);
  const int phase = parent->child.phase;
  gex_Rank_t nranks = child->total_ranks;
  gex_Rank_t myrank = child->myrank;
  gasneti_sync_reads();

  if (nranks == 1) goto out;

  uint8_t *data = gasnete_subteam_op_data(parent, phase, elemsz);

  if (!myrank) {
    GASNETI_MEMCPY(data, ptr, elemsz);
  }

  // Essentially an AllgatherV with len=elemsz on rank 0, and zero on all others
  gex_Rank_t recv_count = 0;
  for (unsigned int step = 0, distance = 1; distance < nranks; ++step, distance *= 2) {
    gex_Rank_t jobrank = child->peers.fwd[step];

    // Send payload using AMMedium
    size_t nbytes = (recv_count >= myrank) ? elemsz : 0; // non-zero if have recv'd data from rank 0
    gex_AM_Arg_t arg0 = GASNETE_SUBTEAM_OP_ARG0(broadcast, phase, step, 0);
    gex_AM_RequestMedium(gasneti_THUNK_TM, jobrank, _hidx_gasnete_subteam_op_reqh,
                         data, nbytes, GEX_EVENT_NOW, 0,
                         parent->team_id, nranks, arg0, (uint32_t)elemsz);

    // Poll until we have received for the current step
    GASNET_BLOCKUNTIL((int)gasneti_weakatomic32_read(&parent->child.step_rcvd[phase][step], 0) == 1);
    gasneti_weakatomic32_set(&parent->child.step_rcvd[phase][step], 0, 0); // reset

    recv_count += distance;
  }

  if (myrank) {
    GASNETI_MEMCPY(ptr, data, elemsz);
  }

  gasneti_free(data);
  parent->child.data[phase] = NULL;

out:
  gasneti_sync_writes();
  parent->child.phase ^= 1;
}

void gasnete_subteam_Exchange(
                        gasnete_coll_team_t child,
                        void *src, size_t elemsz, void *dst
                        GASNETI_THREAD_FARG)
{
  gasneti_assert(elemsz);

  gasnete_coll_team_t parent = child->early_parent;
  gasneti_assert(parent != NULL);
  const int phase = parent->child.phase;
  gex_Rank_t nranks = child->total_ranks;
  gex_Rank_t myrank = child->myrank;
  gasneti_sync_reads();

  if (nranks == 1) {
    GASNETI_MEMCPY_SAFE_IDENTICAL(dst, src, elemsz);
    goto out;
  }

  uint8_t *data = gasnete_subteam_op_data(parent, phase, nranks*elemsz);

  // copy in local contribution
  GASNETI_MEMCPY(data, src, elemsz);

  // Bruck's concatenation algorithm:
  for (unsigned int step = 0, distance = 1; distance < nranks; ++step, distance *= 2) {
    gex_Rank_t jobrank = child->peers.bwd[step];
    size_t nbytes = elemsz * MIN(distance, nranks - distance);
    size_t offset = 0;
    uint32_t seq = 0;

    // Send payload using AMMedium(s)
    do {
      const gex_AM_Arg_t arg0 = GASNETE_SUBTEAM_OP_ARG0(exchange, phase, step, seq);
      const size_t to_xfer = MIN(nbytes, gex_AM_LUBRequestMedium());
      gex_AM_RequestMedium(gasneti_THUNK_TM, jobrank, _hidx_gasnete_subteam_op_reqh,
                           data + offset, to_xfer, GEX_EVENT_NOW, 0,
                           parent->team_id, nranks, arg0, (uint32_t)elemsz);
      ++seq;
      offset += to_xfer;
      nbytes -= to_xfer;
    } while (nbytes);

    // Poll until we have received the same number of messages as we sent
    GASNET_BLOCKUNTIL((int)gasneti_weakatomic32_read(&parent->child.step_rcvd[phase][step], 0) >= (int)seq);
    gasneti_assert_int((int)gasneti_weakatomic32_read(&parent->child.step_rcvd[phase][step], 0) ,==, (int)seq);
    gasneti_weakatomic32_set(&parent->child.step_rcvd[phase][step], 0, 0); // reset
  }

  // Copy to final destination while performing the rotation
  const size_t a = elemsz * (nranks - myrank);
  const size_t b = elemsz * myrank;
  GASNETI_MEMCPY_SAFE_EMPTY(dst, data + a, b);
  GASNETI_MEMCPY_SAFE_EMPTY((uint8_t*)dst + b, data, a);
  gasneti_assert(!memcmp((uint8_t*)dst + myrank*elemsz, src, elemsz));

  gasneti_free(data);
  parent->child.data[phase] = NULL;

out:
  gasneti_sync_writes();
  parent->child.phase ^= 1;
}

void gasnete_subteam_ID(
                        gasnete_coll_team_t child
                        GASNETI_THREAD_FARG)
{
  gasnete_coll_team_t parent = child->early_parent;
  gasneti_assert(parent != NULL);
  gex_Rank_t nranks = child->total_ranks;
  gex_Rank_t myrank = child->myrank;
  uint32_t new_team_id;

  if (!myrank) {
    // the team leader (rank 0) allocates the new team_id
    uint32_t low_bits = team_id_alloc();
    new_team_id = low_bits | (gasneti_mynode << GASNETE_COLL_TEAM_ID_BITS);
  } else {
    // Non-leader polls until we have received the datum
    gasneti_polluntil (0 != (new_team_id = gasneti_weakatomic32_read(&parent->child.team_id, 0)));
    gasneti_weakatomic32_set(&parent->child.team_id, 0, 0);
  }

  // Send to children in a binomial tree
  gex_Rank_t subtree = myrank ? (myrank & -myrank) : nranks;
  if (subtree > 1) {
    for (int step = gasnete_coll_log2_rank(subtree - 1); step >= 0; --step) {
      if (myrank + (1 << step) >= nranks) continue;
      gex_Rank_t jobrank = child->peers.fwd[step];
      const gex_AM_Arg_t arg0 = GASNETE_SUBTEAM_OP_ARG0(team_id, 0, 0, 0);
      gex_AM_RequestMedium(gasneti_THUNK_TM, jobrank, _hidx_gasnete_subteam_op_reqh,
                           NULL, 0, GEX_EVENT_NOW, 0,
                           parent->team_id, nranks, arg0, new_team_id);
    }
  }

  child->team_id = new_team_id;
}

/* ------------------------------------------------------------------------------------ */

// Blocking Rotated, ExchangeV utility function
//
// Takes only local data and length, and computes (and returns) the total length.
// Returns data address via *dst_p
// Returns optional lengths array via *len_p, if non-NULL.
// Both arrays are dynamically allocated and the caller is responsible for freeing them.
// In the event of an "empty" result (returning 0) both pointers returns are NULL;
//
// In order to avoid the cost (time and space) of an in-memory rotation, this
// implementation does not return its result in the normal [0...nranks) order.
// Instead the data starts with the local contribution, followed by the remaining
// ranks in order with wrap-around:
//     myrank, (myrank+1)%nranks, (myrank+2)%nranks, ...
// A caller can index the data in "normal" order with some modular arithmetic.
//
// Boundaries between the variable contributions can be determined by examining
// the (optional) array of lengths available via len_p.  Note that this array
// is in the same rotated order as the data buffer.
//
// TODO-EX: use relevant gex_Coll_*() facilities, if any, when available
// TODO-EX: support for total_len > 2^32

static void *
gasnete_rexchgv_data(gasnete_coll_team_t team, int phase, size_t size) {
  uint8_t *data = team->rexchgv.data[phase];
  if_pf (! data) {
    gex_HSL_Lock(&team->rexchgv.lock);
    data = team->rexchgv.data[phase];
    if (! data) {
      data = gasneti_malloc(size);
      team->rexchgv.data[phase] = data;
    }
    gex_HSL_Unlock(&team->rexchgv.lock);
  }
  return data;
}

// It is not permissible to omit zero-length transfers from the second
// ExchangeV, because their synchronization side-effect is required.  However,
// adding nbytes==0 to rexchgv.rcvd[phase][step] does not provide any signal of
// the arrival.  So, we replace zero by an arbitrary non-zero value in the rcvd
// accounting.
#define gasnete_rexchgv_zero_recvd 42

void gasnete_rexchgv_reqh(
                        gex_Token_t token, void *buf, size_t nbytes,
                        gex_AM_Arg_t team_id, gex_AM_Arg_t arg1,
                        gex_AM_Arg_t total_len, gex_AM_Arg_t offset)
{
  gasnete_coll_team_t team = gasnete_coll_team_lookup(team_id);
  int phase = arg1 & 1;
  int step = arg1 >> 1;
  uint8_t *data = gasnete_rexchgv_data(team, phase, total_len);
  GASNETI_MEMCPY(data + (uint32_t)offset, buf, nbytes);
  size_t increment = nbytes ? nbytes : gasnete_rexchgv_zero_recvd;
  gasneti_weakatomic32_add(&team->rexchgv.rcvd[phase][step], increment, GASNETI_ATOMIC_REL);
}

size_t gasneti_blockingRotatedExchangeV(
                gex_TM_t    tm,
                const void *src,
                size_t      len,
                void      **dst_p,
                size_t    **len_p)
{
  GASNET_BEGIN_FUNCTION(); // TODO: remove this lookup

  gasnete_coll_team_t team = gasneti_import_tm_nonpair(tm)->_coll_team;
  uint32_t team_id = team->team_id;
  gex_Rank_t self = gex_TM_QueryRank(tm);
  gex_Rank_t team_sz = gex_TM_QuerySize(tm);
  int steps = 0; // ceil(log_2(team_sz));
  for (gex_Rank_t tmp = team_sz-1; tmp; tmp >>= 1) ++steps;
  // Without the following hint, some gcc versions warn about massive malloc sizes below
  gasneti_assume(steps <= 8*sizeof(gex_Rank_t));

  int phase = team->rexchgv.phase;
  gasneti_assert(phase == 0 || phase == 1);

  gex_Event_t event0 = GEX_EVENT_INVALID;
  gex_Event_t event1 = GEX_EVENT_INVALID;

  //
  // Step 1.  Exchange the lengths using Bruck's concatenation algorithm
  // The final rotation is omitted, saving time and space, as well as greatly
  // simplifying the index arithmetic in the next step.
  //
  size_t len_array_sz = team_sz * sizeof(len);
  uint8_t *data0 = gasnete_rexchgv_data(team, phase, len_array_sz);
  GASNETI_MEMCPY(data0, &len, sizeof(len));
  gex_NBI_BeginAccessRegion(0);
  for (unsigned int step = 0, distance = 1; step < steps; ++step, distance *= 2) {
    // Send data using stream of AMMediums
    gex_Rank_t dest_rank = (self + team_sz - distance) % team_sz;
    uint32_t offset = distance * sizeof(len);
    uint32_t nbytes = MIN(offset, len_array_sz - offset);
    uint32_t sent = 0;
    uint32_t arg1 = phase | (step << 1);
    size_t limit = gex_AM_MaxRequestMedium(tm,dest_rank,GEX_EVENT_GROUP,0,4);
    do {
      const uint32_t to_xfer = MIN(nbytes - sent, limit);
      gex_AM_RequestMedium4(tm, dest_rank, _hidx_gasnete_rexchgv_reqh,
                            data0 + sent, to_xfer, GEX_EVENT_GROUP, 0,
                            team_id, arg1, len_array_sz, offset + sent);
      sent += to_xfer;
    } while (sent < nbytes);

    // Wait to receive this step's data
    GASNET_BLOCKUNTIL(gasneti_weakatomic32_read(&team->rexchgv.rcvd[phase][step], 0) >= nbytes);
    gasneti_assert_uint(gasneti_weakatomic32_read(&team->rexchgv.rcvd[phase][step], 0) ,==, nbytes);
    gasneti_weakatomic32_set(&team->rexchgv.rcvd[phase][step], 0, 0); // reset
  }
  event0 = gex_NBI_EndAccessRegion(0);
  // reset/advance
  team->rexchgv.data[phase] = NULL;
  phase ^= 1;

  //
  // Step 2.  Compute sum and 2*log(P) partial sums
  // Indexing is nearly trivial due to omitting the rotation of the lengths array
  //
  size_t *len_array = (size_t *)data0; // NOTE: final rotation omitted
  // Total size:
  size_t total_len = 0;
  // Local received size and per-step partials:
  size_t l_sum = 0;
  size_t *l_sums = gasneti_malloc(sizeof(size_t) * (steps+1));
  // Remote received size and per-step partials:
  size_t r_sum = 0;
  size_t *r_sums = gasneti_malloc(sizeof(size_t) * steps);
  // Indexing from both ends:
  int fwd_idx = 0, bwd_idx = team_sz - 1;
  for (int i = 0; i < steps; ++i) {
    int step_sz = 1 << MAX(0,i-1); // 1,1,2,4,8,...
    for (int j = 0; j < step_sz && bwd_idx; ++j) {
      total_len += len_array[fwd_idx];
      r_sum += len_array[bwd_idx--];
      l_sum += len_array[fwd_idx++];
    }
    l_sums[i] = l_sum;
    r_sums[i] = r_sum;
  }
  while (fwd_idx < team_sz) {
    total_len += len_array[fwd_idx++];
  }
  l_sums[steps] = total_len;

  // This code is for slinging metadata, not user data.
  // As such, we've assumed 32-bit (single handler argument) sizes and offsets
  // are sufficient to this purpose.
  if_pf ((uint64_t)total_len > UINT32_MAX) {
    gasneti_fatalerror("blockingRotatedExchangeV size limit of 4GB exceeded: total_len=%"PRIuSZ, total_len);
  }

  if (! total_len) { // Empty!
    *dst_p = NULL;
    goto out_zero_len;
  }

  //
  // Step 3.  Bruck's concatenation algorithm generalized for variable lengths
  //
  uint8_t *data1 = gasnete_rexchgv_data(team, phase, total_len);
  GASNETI_MEMCPY_SAFE_EMPTY(data1, src, len);
  gex_NBI_BeginAccessRegion(0);
  for (unsigned int step = 0, distance = 1; step < steps; ++step, distance *= 2) {
    // Send data using stream of AMMediums
    gex_Rank_t dest_rank = (self + team_sz - distance) % team_sz;
    uint32_t offset = r_sums[step];
    uint32_t nbytes = MIN(l_sums[step], total_len - offset);
    uint32_t sent = 0;
    uint32_t arg1 = phase | (step << 1);
    size_t limit = gex_AM_MaxRequestMedium(tm,dest_rank,GEX_EVENT_GROUP,0,4);
    do { // Note: must not skip nbytes==0 case, since message is needed for synchronization
      const uint32_t to_xfer = MIN(nbytes - sent, limit);
      gex_AM_RequestMedium4(tm, dest_rank, _hidx_gasnete_rexchgv_reqh,
                            data1 + sent, to_xfer, GEX_EVENT_GROUP, 0,
                            team_id, arg1, total_len, offset + sent);
      sent += to_xfer;
    } while (sent < nbytes);

    // Wait to receive this step's data (if any)
    uint32_t to_recv = l_sums[step+1] - l_sums[step];
    if (!to_recv) to_recv = gasnete_rexchgv_zero_recvd; // a non-zero value used for zero-length recv
    GASNET_BLOCKUNTIL(gasneti_weakatomic32_read(&team->rexchgv.rcvd[phase][step], 0) >= to_recv);
    gasneti_assert_uint(gasneti_weakatomic32_read(&team->rexchgv.rcvd[phase][step], 0) ,==, to_recv);
    gasneti_weakatomic32_set(&team->rexchgv.rcvd[phase][step], 0, 0); // reset
  }
  event1 = gex_NBI_EndAccessRegion(0);
  // reset/advance
  team->rexchgv.data[phase] = NULL;
  phase ^= 1;

  *dst_p = data1;

out_zero_len:
  team->rexchgv.phase = phase;
  gasneti_free(r_sums);
  gasneti_free(l_sums);
  gex_Event_Wait(event0); // Source data in len_array
  if (total_len && len_p) {
    *len_p = len_array;
  } else {
    // Either len_array is empty, or caller didn't request it
    if (len_p) *len_p = NULL;
    gasneti_free(len_array);
  }
  gex_Event_Wait(event1); // Source data in data1

  return total_len;
}
/* ------------------------------------------------------------------------------------ */
