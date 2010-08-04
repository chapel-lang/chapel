/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_team.c,v $
 *     $Date: 2010/05/07 03:14:25 $
 * $Revision: 1.8.2.2 $
 *
 * Description: GASNet generic team implementation for collectives 
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt 
 */

#include <gasnet_internal.h>
#include <gasnet_coll.h>
#include <gasnet_coll_internal.h>
#include <gasnet_extended_refcoll.h>

#include <gasnet_coll_team.h>
#include <gasnet_coll_autotune_internal.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_trees.h>

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
                                   const gasnet_image_t images[], gasnet_node_t myrank, gasnet_node_t num_members, 
                                   gasnet_seginfo_t * scratch_segments GASNETE_THREAD_FARG) {

  size_t image_size = num_members*sizeof(gasnet_image_t);
  int i;
  static size_t smallest_scratch_seg;
#if GASNET_DEBUG
  static int team_all_made=0;
#endif
  
#if 0
  if(!allocating_team_all) {
    /*the space for team all has already been initialized in gasnete_init()*/

  }
#if GASNET_DEBUG
  else if(!team_all_made) {
    team = GASNET_TEAM_ALL;
    team_all_made = 1;
  } else {
    gasneti_fatalerror("TRYING TO RECONSTRUCT TEAM ALL (allocating_team_all variable can be set at most and should not be set for construction of non team all)\n");
  }
#endif
#endif
  
  team->sequence = 42;
  team->all_images = gasneti_malloc(image_size);
  team->all_offset = gasneti_malloc(image_size);
  if (images != NULL) {
    memcpy(team->all_images, images, image_size);
  } else  {
    for (i = 0; i < num_members; ++i) {
      team->all_images[i] = 1;
    }
  }
  team->total_images = 0;
  team->max_images = 0;
  team->fixed_image_count=1;
  smallest_scratch_seg = scratch_segments[0].size;
  for (i = 0; i < num_members; ++i) {
    team->all_offset[i] = team->total_images;
    team->total_images += team->all_images[i];
    team->max_images = MAX(team->max_images,team->all_images[i]);
    if(team->all_images[i] != team->all_images[0]) {
      team->fixed_image_count = 0;
    }
    smallest_scratch_seg = MIN(smallest_scratch_seg, scratch_segments[i].size);
    
  }
  team->my_images = team->all_images[myrank];
  team->my_offset = team->all_offset[myrank];
  
#if GASNET_PAR
  /*can only be allocating team all if PAR MODE*/
  
  if (!images) {
    team->multi_images = 0;
    team->multi_images_any = 0;
  } else if (team->my_images != 1) {
    team->multi_images = 1;
    team->multi_images_any = 1;
  } else {
    team->multi_images = 0;
    team->multi_images_any = 0;
    for (i = 0; i < gasneti_nodes; ++i) {
      if (team->all_images[i] > 1) {
        team->multi_images_any = 1;
        break;
      }
    }
  }
#endif
  
#if !GASNET_SEQ
  {
    gasnet_image_t j;
    team->image_to_node = gasneti_malloc(team->total_images * sizeof(gasnet_node_t));
    for (j = 0, i = 0; j < team->total_images; ++j) {
      if (j >= (team->all_offset[i] + team->all_images[i])) {
        i += 1;
      }
      team->image_to_node[j] = i;
    }
  }
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
  team->autotune_info = gasnete_coll_autotune_init(team, myrank, num_members, 
                                                   team->my_images, team->total_images,
                                                   smallest_scratch_seg GASNETE_THREAD_PASS);
  team->consensus_issued_id = 0;
  team->consensus_id = 0;
  gasnete_coll_alloc_new_scratch_status(team);
  gasneti_weakatomic_set(&team->num_multi_addr_collectives_started, 0, GASNETT_ATOMIC_WMB_PRE);
  if(!team->fixed_image_count && team->myrank ==0) {
    fprintf(stderr, "WARNING: Current collective implementation requires a constant number\n");
    fprintf(stderr, "WARNING: of threads per process for optimized collectives.\n");
  }
  
}

void gasnete_coll_team_init(gasnet_team_handle_t team, 
                            uint32_t team_id, 
                            uint32_t total_ranks,
                            gasnet_node_t myrank,
                            gasnet_node_t *rel2act_map,
                            gasnet_seginfo_t *scratch_segs,
                            const gasnet_image_t images[] GASNETE_THREAD_FARG)
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
  
  uint32_t i;
  initialize_team_fields(team, images, myrank, total_ranks, scratch_segs GASNETE_THREAD_PASS); 
  team->team_id = team_id;
  team->total_ranks = total_ranks;
  team->myrank = myrank;
  if (team->rel2act_map == NULL)
    team->rel2act_map = (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t)*total_ranks);

  for (i=0; i<total_ranks; i++) 
    team->rel2act_map[i] = rel2act_map[i];

  if(team!=GASNET_TEAM_ALL) {
    /*GASNET TEAM ALL already has a barrier attached to it*/
    gasnete_coll_barrier_init(team, GASNETE_COLL_BARRIER_ENVDEFAULT);
  }
  
  /* lock the team direcotry (team_dir) */
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
}

void gasnete_coll_team_fini(gasnet_team_handle_t team)
{
  gasneti_assert(team != NULL);
  /* free data members of the team, such as scratch space and etc. */
  gasneti_free(team->rel2act_map);
  gasneti_assert(team_dir != NULL);
  gasnete_hashtable_remove(team_dir, team->team_id, NULL);

#ifdef gasnete_coll_team_fini_conduit
  /* conduit specific initialization for gasnet teams */
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_fini: calling gasnete_coll_team_fini_conduit.\n");
  fflush(stderr);
#endif
  gasnete_coll_team_fini_conduit(team);
#endif
}

void gasnete_coll_teamid_reqh(gasnet_token_t token,
                              gasnet_handlerarg_t team_id)
{
  new_team_id=(uint32_t)team_id;
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_teamid_reqh: new_team_id %x\n", new_team_id);
  fflush(stderr);
#endif
}

/* collective function that should be called by all participating nodes */
gasnet_team_handle_t gasnete_coll_team_create(uint32_t total_ranks,
                                              gasnet_node_t myrank,
                                              gasnet_node_t *rel2act_map, gasnet_seginfo_t* scratch_segs GASNETE_THREAD_FARG)
{
  gasnet_team_handle_t team;
  gasnet_node_t team_lead = rel2act_map[0];
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
    
    /* create the team locally */
    team = (gasnet_team_handle_t)gasneti_calloc(1,sizeof(struct gasnete_coll_team_t_));
#if GASNET_PAR
    gasneti_fatalerror("can't call team_init in PAR Builds yet");
#endif
    gasnete_coll_team_init(team, new_team_id, total_ranks, myrank, rel2act_map, scratch_segs, NULL GASNETE_THREAD_PASS);
        
    /* send out team_id */
    for(i=1; i<total_ranks; i++) {
      GASNETI_SAFE(SHORT_REQ(1,1,(rel2act_map[i],
                                  gasneti_handleridx(gasnete_coll_teamid_reqh),
                                  new_team_id)));
    }
  } else {
    /* wait for team_id from the team leader */
    while (new_team_id == 0)
      gasneti_AMPoll();

#ifdef DEBUG_TEAM
    fprintf(stderr, "myrank %u, get new_team_id %x\n", myrank, new_team_id);
    fflush(stderr);
#endif
    /* create the team locally */
    team = (gasnet_team_handle_t)gasneti_calloc(1,sizeof(struct gasnete_coll_team_t_));
    gasnete_coll_team_init(team, new_team_id, total_ranks, myrank, rel2act_map, scratch_segs, NULL GASNETE_THREAD_PASS);
    new_team_id = 0;
  }
  
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

gasnet_team_handle_t gasnete_coll_team_split(gasnet_team_handle_t team,
                                             gasnet_node_t mycolor,
                                             gasnet_node_t myrelrank,
                                             void *clientdata
                                             GASNETE_THREAD_FARG)
{
  gasnet_team_handle_t newteam;
  uint32_t new_total_ranks;
  gasnet_node_t *colors; /* gasnet_image_t for PAR mode*/
  gasnet_node_t *relranks; /* gasnet_image_t for PAR mode */
  gasnet_node_t *rel2act_map;
  gasnet_seginfo_t *allsegs;
  uint32_t i;
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_split: team rank %u, parent team handle %p, mycolor %u, myrank %u\n",
          team->myrank, team, mycolor, myrelrank);
  fflush(stderr);
#endif

  colors = (gasnet_node_t *)gasneti_malloc(sizeof(mycolor)*team->total_ranks);
  gasneti_assert(colors != NULL);
  relranks = (gasnet_node_t *)gasneti_malloc(sizeof(myrelrank)*team->total_ranks);
  gasneti_assert(relranks != NULL);
  allsegs = (gasnet_seginfo_t*) gasneti_malloc(sizeof(gasnet_seginfo_t)*team->total_ranks);
 
  
  gasnet_coll_gather_all(team, allsegs, (gasnet_seginfo_t*) clientdata, 
                         sizeof(gasnet_seginfo_t), GASNET_COLL_LOCAL|GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);
  /* collect the color information */
  gasnet_coll_gather_all(team, colors, &mycolor, sizeof(gasnet_node_t), GASNET_COLL_LOCAL|GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);

  
  /* collect the relrank information */
  gasnet_coll_gather_all(team, relranks, &myrelrank, sizeof(gasnet_node_t), GASNET_COLL_LOCAL|GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);


  new_total_ranks = 0;
  rel2act_map = (gasnet_node_t *)gasneti_malloc(team->total_ranks*sizeof(gasnet_node_t));
  for (i=0; i<team->total_ranks; i++) {
    if (mycolor == colors[i]) {
      rel2act_map[relranks[i]] = team->rel2act_map[i];
      new_total_ranks++;
    }
  }
  
  /* It would be better to add some sanity check for team correctness here. */
  
  /* create a team */
  new_team_id = 0;
  gasnete_coll_teambarrier(team);

#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_split: new_total_ranks %u, myrelrank %u.\n",
          new_total_ranks, myrelrank);
  PRINT_ARRAY(stderr, rel2act_map, new_total_ranks, "%u");
  fflush(stderr);
#endif

  newteam = gasnete_coll_team_create(new_total_ranks, myrelrank, rel2act_map, allsegs GASNETE_THREAD_PASS);
  
  gasneti_free(rel2act_map);
  gasnete_coll_teambarrier(team);
  return newteam;
}

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id) 
{
  uint32_t rv;
  gasnet_team_handle_t team;
  
#ifdef DEBUG_TEAM
  fprintf(stderr, "gasnete_coll_team_lookup: team_id %x\n", team_id);
  fflush(stderr);
#endif

	if (team_id == 0)
    team = GASNET_TEAM_ALL;
  else {
    if (gasnete_hashtable_search(team_dir, team_id, (void **)&team))
      team = NULL; /* cannot find team_id the hash table */
  }
  
  return team;
}

gasnet_node_t gasnete_coll_team_rank2node(gasnete_coll_team_t team, int rank) 
{
  gasneti_assert(team != NULL);
  gasneti_assert(rank < team->total_ranks);
  return team->rel2act_map[rank];
}

gasnet_node_t gasnete_coll_team_node2rank(gasnete_coll_team_t team, gasnet_node_t node) 
{
  uint32_t i;
  gasneti_assert(team != NULL);
  for (i=0; i<team->total_ranks; i++)
    if (team->rel2act_map[i] == node)
      return i;
   
  gasneti_fatalerror("Cannot find node %u in team %p with id %x!\n", 
                     (unsigned int)node, team, (unsigned int)team->team_id);
  return (gasnet_node_t)(-1); /* NOT REACHED */
}

uint32_t gasnete_coll_team_id(gasnete_coll_team_t team) 
{
  return team->team_id;
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
