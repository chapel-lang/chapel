/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_autotune.c,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.25 $
 * Description: GASNet Autotuner Implementation
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


/* This is intended as a stub for the autotuner routines*/
#include <gasnet_coll_autotune_internal.h>


/*a small library to write and read XML style sheets for the collective tuner*/
#include <myxml/myxml.h>
#include <myxml/myxml.c>

/*this array is the maximum size of hte log2 array for fanouts*/


#define GASNETE_COLL_PRINT_TIMERS 0
static int gasnete_coll_print_autotuner_timers;

struct gasnet_coll_tuning_iterator_t_{
  uint32_t num_params;
  struct gasnet_coll_tuning_parameter_t params[GASNET_COLL_NUM_PARAM_TYPES];
  uint32_t param_space_size[GASNET_COLL_NUM_PARAM_TYPES];
  uint32_t curr_pos[GASNET_COLL_NUM_PARAM_TYPES]; /*a position into the parameter space for each parameter*/
  uint32_t max_idx;
  uint32_t curr_idx;
};

GASNETI_ALWAYS_INLINE(gasnete_coll_nextpower2)
size_t gasnete_coll_nextpower2(size_t n)
{
  size_t x;
  if(n==0) return 0;
  x=1;
  while(x < n) x<<=1;
  return x;
}

/*
 the following two functions to find the fast log2 of an int are adapted from:
 http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup (accessed July 10, 2008)
 */

static uint32_t fast_log2_64bit(uint64_t number) {
	
	static const char LogTable256[] = 
  {
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
  };
	
	uint64_t v=number; 
	uint32_t r;    
	uint64_t t, tt; 
	
	if ((tt = v>>48)) {
		r = ((t = tt>>8) ? 56 + LogTable256[t] : 48 + LogTable256[tt]); 
	} else if ((tt = v>>32)) {
		r = ((t = tt>>8) ? 40 + LogTable256[t] : 32 + LogTable256[tt]); 
	} else	if ((tt = v >> 16)) {
		r = ((t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt]);
	}
	else {
		r = ((t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v]);
	}
	
	return r;
	
}

static uint32_t fast_log2_32bit(uint32_t number) {
  
  static const char LogTable256[] = 
    {
      0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
    };
  
  uint32_t v=number; 
  uint32_t r;     
  uint32_t t, tt; 
  
  
  if ((tt = v >> 16)) {
    r = ((t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt]);
  }
  else {
    r = ((t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v]);
  }
  
  return r;
  
}

/*register teh collective algorithm
  optype is the type of collective op
  syncflags is an ored list of the valid sync flags for this colelctive
  requirements is an or'ed list of the required flags (i.e. DST/SRC in segment, etc)
  max_size is the maximum number of bytes this algorithm is valid for
  tree_alg indicates whether this is a tree based algorithm so that those tuning parameters can automaitcally be appended
  num_params is the number of params for the algorithm
 param_list is the paramter list
*/

int gasnete_coll_autotune_get_num_tree_types(gasnet_team_handle_t team) {
  /*for now only search over the FLAT, NARY, KNOMIAL, and RECURSIVE trees power of two fanouts and the FLAT TREE*/
  int log2_threads = fast_log2_32bit(MIN((uint32_t) team->total_ranks,128));
  
  return (team->autotune_info->allow_flat_tree ? 1 : 0) + /*flat_tree*/
    log2_threads * (GASNETE_COLL_NUM_PLATFORM_INDEP_TREE_CLASSES-1); /*num powers of two for each of the three tree types*/
}


gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type_idx(gasnet_team_handle_t team, int idx) {
  gasnete_coll_tree_type_t ret = gasnete_coll_get_tree_type();
  int log2_threads = fast_log2_32bit(MIN((uint32_t) team->total_ranks,128));
  int tree_class;
  int radix;
  gasneti_assert(idx < gasnete_coll_autotune_get_num_tree_types(team));
  if(team->autotune_info->allow_flat_tree) {
    if(idx == 0) {
      ret->tree_class = GASNETE_COLL_FLAT_TREE;
      return ret;
    }
    idx -=1;
  }
  
  tree_class = (idx / log2_threads)+1;
  radix = 1 << (1+(idx % log2_threads));
  return gasnete_coll_make_tree_type(tree_class, &radix, 1);
}


gasnete_coll_algorithm_t gasnete_coll_autotune_register_algorithm(gasnet_team_handle_t team, 
                                                                  gasnet_coll_optype_t optype, 
                                                                  uint32_t syncflags,
                                                                  uint32_t requirements,
                                                                  uint32_t n_requirements,
                                                                  size_t max_size,
                                                                  size_t min_size,
                                                                  uint32_t tree_alg,
                                                                  uint32_t num_params,
                                                                  struct gasnet_coll_tuning_parameter_t *param_list, 
                                                                  void *coll_fnptr,
                                                                  const char *name_str) {
  gasnete_coll_algorithm_t ret;
  int i;
  ret.tree_alg = tree_alg;
  ret.optype = optype;
  ret.syncflags = syncflags;
  ret.requirements = requirements;
  ret.n_requirements = n_requirements;
  ret.num_parameters = num_params+tree_alg;
  ret.max_num_bytes = max_size;
  ret.min_num_bytes = min_size;
  ret.name_str = name_str;
  /*create a deep copy of the param list*/
  gasneti_assert(tree_alg == 1 || tree_alg == 0);
  if(num_params > 0 || tree_alg) {
    ret.parameter_list = (struct gasnet_coll_tuning_parameter_t*) gasneti_malloc(sizeof(struct gasnet_coll_tuning_parameter_t)*(num_params+tree_alg));
    for(i=0; i<num_params; i++) {
      ret.parameter_list[i].tuning_param = param_list[i].tuning_param;
      ret.parameter_list[i].start = param_list[i].start;
      ret.parameter_list[i].end = param_list[i].end;
      ret.parameter_list[i].stride = param_list[i].stride;
      ret.parameter_list[i].flags = param_list[i].flags;
    }
    if(tree_alg) {
      /*always add the param as the last one*/
      ret.parameter_list[num_params].tuning_param = GASNET_COLL_TREE_TYPE;
      ret.parameter_list[num_params].start = 0;
      ret.parameter_list[num_params].end = gasnete_coll_autotune_get_num_tree_types(team)-1;
      ret.parameter_list[num_params].stride = 1;
      ret.parameter_list[num_params].flags = GASNET_COLL_TUNING_TREE_SHAPE | GASNET_COLL_TUNING_STRIDE_ADD;
    }
  
  } else {
    ret.parameter_list = NULL;
  }
  switch(optype) {
    case GASNET_COLL_BROADCAST_OP: ret.fn_ptr.bcast_fn = (gasnete_coll_bcast_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_BROADCASTM_OP: ret.fn_ptr.bcastM_fn = (gasnete_coll_bcastM_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_SCATTER_OP: ret.fn_ptr.scatter_fn = (gasnete_coll_scatter_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_SCATTERM_OP: ret.fn_ptr.scatterM_fn = (gasnete_coll_scatterM_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_GATHER_OP: ret.fn_ptr.gather_fn = (gasnete_coll_gather_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_GATHERM_OP: ret.fn_ptr.gatherM_fn = (gasnete_coll_gatherM_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_GATHER_ALL_OP: ret.fn_ptr.gather_all_fn = (gasnete_coll_gather_all_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_GATHER_ALLM_OP: ret.fn_ptr.gather_allM_fn = (gasnete_coll_gather_allM_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_EXCHANGE_OP: ret.fn_ptr.exchange_fn = (gasnete_coll_exchange_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_EXCHANGEM_OP: ret.fn_ptr.exchangeM_fn = (gasnete_coll_exchangeM_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_REDUCE_OP: ret.fn_ptr.reduce_fn = (gasnete_coll_reduce_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_REDUCEM_OP: ret.fn_ptr.reduceM_fn = (gasnete_coll_reduceM_fn_ptr_t) coll_fnptr; break;
    default: gasneti_fatalerror("not implemented yet");
  }
  return ret;
}

#define GASNETE_COLL_EVERY_IN_SYNC_FLAG GASNET_COLL_IN_NOSYNC | GASNET_COLL_IN_MYSYNC | GASNET_COLL_IN_ALLSYNC 
#define GASNETE_COLL_EVERY_OUT_SYNC_FLAG GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC 
#define GASNETE_COLL_EVERY_SYNC_FLAG GASNETE_COLL_EVERY_IN_SYNC_FLAG | GASNETE_COLL_EVERY_OUT_SYNC_FLAG

#define GASNETE_COLL_MAX_BYTES ((size_t) -1)
void gasnete_coll_register_broadcast_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch)  {
  
  /*first register all the broadcast algorithms*/
  /*all tuning parameters are inclusinve (i.e. iterations go from for(i=start; i<=end; i+=stride (or) i*=stride)*/
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_BROADCAST_NUM_ALGS);
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SINGLE, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcast_Put, "BROADCAST_PUT");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_SINGLE, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcast_Get, "BROADCAST_GET");
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNET_COLL_IN_NOSYNC | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_ALLSYNC,
                                           GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SINGLE, 0,
                                           gasnet_AMMaxLongRequest(), 0, 1,
                                           0, NULL,(void*)gasnete_coll_bcast_TreePut, "BROADCAST_TREE_PUT");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SCRATCH] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0,
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch), 0, 1,
                                           0,NULL,(void*)gasnete_coll_bcast_TreePutScratch, "BROADCAST_TREE_PUT_SCRATCH");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_SCATTERALLGATHER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags (scatter/allgather will pick their right implementations based on the actual flags)*/ 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcast_ScatterAllgather, "BROADCAST_SCATTERALLGATHER");
  {
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE, GASNET_COLL_MIN_PIPE_SEG_SIZE, MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch), 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    
    info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0,
                                             GASNET_COLL_MIN_PIPE_SEG_SIZE*GASNETE_COLL_MAX_NUM_SEGS, GASNET_COLL_MIN_PIPE_SEG_SIZE, 1,
                                             1,tuning_params,(void*)gasnete_coll_bcast_TreePutSeg, "BROADCAST_TREE_PUT_SEG");
    
    
  }
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           gasnete_coll_p2p_eager_min, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcast_Eager, "BROADCAST_EAGER");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           gasnete_coll_p2p_eager_min,0, 1,
                                           0,NULL,(void*)gasnete_coll_bcast_TreeEager, "BROADCAST_TREE_EAGER");
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVOUS] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           GASNETE_COLL_MAX_BYTES, /*works for all sizes*/ 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcast_RVous, "BROADCAST_RVOUS");
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, /*works for all sizes*/ 0, 0, 
                                           0,NULL,(void*)gasnete_coll_bcast_RVGet, "BROADCAST_RVGET");
  
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_DST_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES /*works for all sizes*/, 0, 1,
                                           0,NULL,(void*)gasnete_coll_bcast_TreeRVGet, "BROADCAST_TREE_RVGET");
  
  
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_BROADCASTM_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SINGLE | GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcastM_Get, "BROADCASTM_GET");
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SINGLE | GASNET_COLL_DST_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcastM_Put, "BROADCASTM_PUT");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNET_COLL_IN_NOSYNC | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_ALLSYNC,
                                           GASNET_COLL_SINGLE | GASNET_COLL_DST_IN_SEGMENT, 0,
                                           gasnet_AMMaxLongRequest(), 0, 1,
                                           0,NULL,(void*)gasnete_coll_bcastM_TreePut, "BROADCASTM_TREE_PUT");
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_PUT_SCRATCH] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0,
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch), 0, 1,
                                           0,NULL,(void*)gasnete_coll_bcastM_TreePutScratch,"BROADCASTM_TREE_PUT_SCRATCH");
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_SCATTERALLGATHER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags (scatter/allgather will pick their right implementations based on the actual flags)*/0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcastM_ScatterAllgather,"BROADCASTM_SCATTERALLGATHER");
  
  {
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE, GASNET_COLL_MIN_PIPE_SEG_SIZE, MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch), 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    
    info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0,
                                             GASNET_COLL_MIN_PIPE_SEG_SIZE*GASNETE_COLL_MAX_NUM_SEGS, GASNET_COLL_MIN_PIPE_SEG_SIZE, 1,
                                             1,tuning_params,(void*)gasnete_coll_bcastM_TreePutSeg, "BROADCASTM_TREE_PUT_SEG");
    
    
  }
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           gasnete_coll_p2p_eager_min, 0, 1,
                                           0,NULL,(void*)gasnete_coll_bcastM_TreeEager, "BROADCASTM_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           gasnete_coll_p2p_eager_min, 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcastM_Eager, "BROADCASTM_EAGER");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_RVOUS] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           GASNETE_COLL_MAX_BYTES, /*works for all sizes*/ 0, 0,
                                           0,NULL,(void*)gasnete_coll_bcastM_RVous, "BROADCASTM_RVOUS");
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, /*works for all sizes*/ 0, 0, 
                                           0,NULL,(void*)gasnete_coll_bcastM_RVGet, "BROADCASTM_RVGET");
  
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_DST_IN_SEGMENT, 0,
                                            GASNETE_COLL_MAX_BYTES /*works for all sizes*/, 0, 1,
                                           0,NULL,(void*)gasnete_coll_bcastM_TreeRVGet, "BROADCASTM_TREE_RVGET");
  
}

void gasnete_coll_register_scatter_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  info->collective_algorithms[GASNET_COLL_SCATTER_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_SCATTER_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_SINGLE,  0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_scat_Get, "SCATTER_GET");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT| GASNET_COLL_SINGLE, 0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_scat_Put, "SCATTER_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/(info->team->total_ranks), 0, 1,
                                           0, NULL, (void*)gasnete_coll_scat_TreePut, "SCATTER_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_NO_COPY] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/(info->team->total_ranks), 0, 1,
                                           0, NULL, (void*)gasnete_coll_scat_TreePutNoCopy, "SCATTER_PUT_NOCOPY");
  {
    size_t smallest_seg_size =  MIN(gasnet_AMMaxLongRequest(),GASNET_COLL_MIN_PIPE_SEG_SIZE)/(info->team->total_ranks);
    size_t largest_seg_size =  MIN(gasnet_AMMaxLongRequest(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch))/(info->team->total_ranks);
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE,smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             smallest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, smallest_seg_size, 1,
                                             1, tuning_params, (void*)gasnete_coll_scat_TreePutSeg, "SCATTER_PUT_SEG");
  }

  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gasnet_AMMaxMedium()), 0, 0,
                                           0,NULL,(void*)gasnete_coll_scat_Eager, "SCATTER_EAGER");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gasnet_AMMaxMedium()/info->team->total_images), 0, 1,
                                           0,NULL,(void*)gasnete_coll_scat_TreeEager, "SCATTER_TREE_EAGER");

  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_scat_RVGet, "SCATTER_RVGET");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_scat_RVous, "SCATTER_RVOUS");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_SCATTERM_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_SINGLE,  0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_scatM_Get, "SCATTERM_GET");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT| GASNET_COLL_SINGLE, 0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_scatM_Put, "SCATTERM_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/(info->team->total_ranks*info->team->my_images), 0, 1,
                                           0, NULL, (void*)gasnete_coll_scatM_TreePut, "SCATTERM_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_PUT_NO_COPY] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/(info->team->total_ranks*info->team->my_images), 0, 1,
                                           0, NULL, (void*)gasnete_coll_scatM_TreePutNoCopy, "SCATTERM_PUT_NOCOPY");
  {
    size_t smallest_seg_size =  MIN(gasnet_AMMaxLongRequest(),GASNET_COLL_MIN_PIPE_SEG_SIZE)/(info->team->total_ranks*info->team->my_images);
    size_t largest_seg_size =  MIN(gasnet_AMMaxLongRequest(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch))/(info->team->total_ranks*info->team->my_images);
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE,smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             smallest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, smallest_seg_size, 1,
                                             1, tuning_params, (void*)gasnete_coll_scatM_TreePutSeg, "SCATTERM_PUT_SEG");
  }
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gasnet_AMMaxMedium()/info->team->my_images), 0, 0,
                                           0,NULL,(void*)gasnete_coll_scatM_Eager, "SCATTERM_EAGER");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale,
                                               gasnet_AMMaxMedium()/info->team->total_images), 
					   0, 1,
					   0,NULL,(void*)gasnete_coll_scatM_TreeEager, "SCATTERM_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_scatM_RVGet, "SCATTERM_RVGET");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVOUS] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_scatM_RVous, "SCATTERM_RVOUS");
  
  
}

void gasnete_coll_register_gather_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  info->collective_algorithms[GASNET_COLL_GATHER_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_GATHER_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_SINGLE,  0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_gath_Get, "GATHER_GET");
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SINGLE,  0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_gath_Put, "GATHER_PUT");

  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT,  0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/info->team->total_ranks, 0, 1,
                                           0, NULL, (void*)gasnete_coll_gath_TreePut, "GATHER_TREE_PUT");

  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_NO_COPY] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT,  0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/info->team->total_ranks, 0, 1,
                                           0, NULL, (void*)gasnete_coll_gath_TreePutNoCopy, "GATHER_TREE_PUT_NO_COPY");
  
  {
    size_t smallest_seg_size =  MIN(gasnet_AMMaxLongRequest(),GASNET_COLL_MIN_PIPE_SEG_SIZE)/info->team->total_ranks;
    size_t largest_seg_size =  MIN(gasnet_AMMaxLongRequest(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch))/info->team->total_ranks;
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE,smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             smallest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, smallest_seg_size, 1,
                                             1, tuning_params, (void*)gasnete_coll_gath_TreePutSeg, "GATHER_TREE_PUT_SEG");
  }
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gasnet_AMMaxMedium()/info->team->total_images), 0, 1,
                                           0,NULL,(void*)gasnete_coll_gath_TreeEager, "GATHER_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_EAGER]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gasnet_AMMaxMedium()), 0, 0,
                                           0,NULL,(void*)gasnete_coll_gath_Eager, "GATHER_EAGER");
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVPUT]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_gath_RVPut, "GATHER_RVPUT");
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_gath_RVous, "GATHER_RVOUS");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_GATHERM_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_SINGLE,  0, 
                                           0, 0, 0,
                                           0, NULL, (void*)gasnete_coll_gathM_Get, "GATHERM_GET");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SINGLE,  0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*)gasnete_coll_gathM_Put, "GATHERM_PUT");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT,  0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/info->team->total_images, 0, 1,
                                           0, NULL, (void*)gasnete_coll_gathM_TreePut, "GATHERM_TREE_PUT");
  
  {
    size_t smallest_seg_size =  MIN(gasnet_AMMaxLongRequest(),GASNET_COLL_MIN_PIPE_SEG_SIZE)/info->team->total_images;
    size_t largest_seg_size =  MIN(gasnet_AMMaxLongRequest(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch))/info->team->total_images;
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE,smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             smallest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, smallest_seg_size, 1,
                                             1, tuning_params, (void*)gasnete_coll_gathM_TreePutSeg, "GATHERM_TREE_PUT_SEG");
  }
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_EAGER]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale/info->team->my_images,
                                               gasnet_AMMaxMedium()/info->team->total_images),
                                           0, 1,
                                           0,NULL,(void*)gasnete_coll_gathM_TreeEager, "GATHERM_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_EAGER]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           gasnete_coll_p2p_eager_scale/info->team->my_images, 0, 0,
                                           0,NULL,(void*)gasnete_coll_gathM_Eager, "GATHERM_EAGER");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVPUT]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_gathM_RVPut, "GATHERM_RVPUT");
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVOUS]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,(void*)gasnete_coll_gathM_RVous, "GATHERM_RVOUS");
  
}

void gasnete_coll_register_gather_all_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_GATHER_ALL_NUM_ALGS);

  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_GATH] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, /*works for every flag (gather will take care of figuiring out right algorithm)*/
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_Gath, "GATHER_ALL_GATH");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM_EAGER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           MIN(gasnete_coll_p2p_eager_scale,gasnet_AMMaxMedium()/info->team->total_images), 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_EagerDissem, "GATHER_ALL_EAGER_DISSEM");

  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/info->team->total_ranks, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_Dissem, "GATHER_ALL_DISSEM");

  
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM_NOSCRATCH] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SINGLE | GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           gasnet_AMMaxLongRequest()/info->team->total_ranks, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_DissemNoScratch, "GATHER_ALL_DISSEM_NO_SCRATCH");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_FLAT_PUT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SINGLE, 0, 
                                           gasnet_AMMaxLongRequest(), 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_FlatPut, "GATHER_ALL_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_FLAT_PUT_EAGER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           gasnete_coll_p2p_eager_scale, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_FlatEagerPut, "GATHER_ALL_FLAT_PUT_EAGER");
  

  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_FLAT_GET] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_SINGLE, 0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gall_FlatGet, "GATHER_ALL_FLAT_GET");

  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_GATHER_ALLM_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_GATH] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, /*works for every flag (gather will take care of figuiring out right algorithm)*/
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_Gath, "GATHER_ALLM_GATH");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_DISSEM_EAGER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           MIN(gasnete_coll_p2p_eager_scale/info->team->my_images,
                                               gasnet_AMMaxMedium()/info->team->total_images),
                                           0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_EagerDissem, "GATHER_ALLM_EAGER_DISSEM");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_DISSEM] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/info->team->total_images, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_Dissem, "GATHER_ALLM_DISSEM");

  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_DISSEM_NOSCRATCH] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SINGLE | GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           gasnet_AMMaxLongRequest()/info->team->total_images, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_DissemNoScratch, "GATHER_ALLM_DISSEM_NOSCRATCH");

  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_DISSEM_NOSCRATCH_SEG] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SINGLE | GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_DissemNoScratchSeg, "GATHER_ALLM_DISSEM_NOSCRATCH_SEG");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_FLAT_PUT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SINGLE, 0, 
                                           gasnet_AMMaxLongRequest()/info->team->my_images, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_FlatPut, "GATHER_ALLM_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_FLAT_PUT_EAGER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           gasnete_coll_p2p_eager_scale/info->team->my_images, 0, 0,
                                           0, NULL, (void*) gasnete_coll_gallM_FlatEagerPut, "GATHER_ALLM_FLAT_PUT_EAGER");
  
  
}

void gasnete_coll_register_exchange_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  gasnete_coll_dissem_info_t *dissem;
  info->collective_algorithms[GASNET_COLL_EXCHANGE_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_EXCHANGE_NUM_ALGS);
  
  {
    int radix = 2;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM2] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_Dissem2, "EXCHANGE_DISSEM2");
  }
  {
    int radix = 3;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM3] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_Dissem3, "EXCHANGE_DISSEM3");
  }
  {
    int radix = 4;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM4] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_Dissem4, "EXCHANGE_DISSEM4");
  }
  {
    int radix = 8;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM8] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_Dissem8, "EXCHANGE_DISSEM8");
  }
  {
    int radix = info->team->total_ranks;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_FLAT_SCRATCH] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_FlatScratch, "EXCHANGE_FLAT_SCRATCH");
  }
  {
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_PUT] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT|GASNET_COLL_SINGLE, 0, 
                                             GASNETE_COLL_MAX_BYTES, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_Put, "EXCHANGE_PUT");
  }
  {
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_RVPUT] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             GASNETE_COLL_MAX_BYTES, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_RVPut, "EXCHANGE_RVPUT");
  }
  {
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_GATH] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             GASNETE_COLL_MAX_BYTES, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchg_Gath, "EXCHANGE_GATH");
  }
  
  
  info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_EXCHANGEM_NUM_ALGS);
  {
    int radix = 2;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEM2] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_Dissem2, "EXCHANGEM_DISSEM2");
  }
  {
    int radix = 3;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEM3] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_Dissem3, "EXCHANGEM_DISSEM3");
  }
  {
    int radix = 4;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEM4] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_Dissem4, "EXCHANGEM_DISSEM4");
  }
  {
    int radix = 8;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEM8] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_Dissem8, "EXCHANGEM_DISSEM8");
  }
  {
    int radix = info->team->total_ranks;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gasnet_AMMaxLongRequest(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_FLAT_SCRATCH] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_FlatScratch, "EXCHANGEM_FLAT_SCRATCH");
  }
  {
    int radix = 2;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = smallest_scratch/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEMSEG2] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_DissemSeg2, "EXCHANGEM_DISSEMSEG2");
  }
  {
    int radix = 3;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = smallest_scratch/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEMSEG3] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_DissemSeg3, "EXCHANGEM_DISSEMSEG3");
  }
  {
    int radix = 4;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = smallest_scratch/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEMSEG4] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_DissemSeg4, "EXCHANGEM_DISSEMSEG4");
  }
  {
    int radix = 8;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = smallest_scratch/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEMSEG8] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_DissemSeg8, "EXCHANGEM_DISSEMSEG8");
  }
  {
    int radix = info->team->total_ranks;
    size_t max_size;
    gasnet_image_t tpn = info->team->my_images;
    gasnet_node_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = smallest_scratch/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_FLAT_SCRATCH_SEG] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_FlatScratchSeg, "EXCHANGEM_FLAT_SCRATCH_SEG");
  }

  {
    info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_GATH] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             GASNETE_COLL_MAX_BYTES, 0, 0,
                                             0, NULL, (void*) gasnete_coll_exchgM_Gath, "EXCHANGEM_GATH");
  }
}

void gasnete_coll_register_reduce_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  
  info->collective_algorithms[GASNET_COLL_REDUCE_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_REDUCE_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_REDUCE_OP][GASNETE_COLL_REDUCE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCE_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           gasnete_coll_p2p_eager_scale, 0, 0,
                                           0,NULL,(void*)gasnete_coll_reduce_Eager, "REDUCE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_REDUCE_OP][GASNETE_COLL_REDUCE_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCE_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           gasnete_coll_p2p_eager_scale, 0, 1,
                                           0,NULL,(void*)gasnete_coll_reduce_TreeEager, "REDUCE_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_REDUCE_OP][GASNETE_COLL_REDUCE_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCE_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch/info->team->total_ranks), 0, 1,
                                           0,NULL,(void*)gasnete_coll_reduce_TreePut, "REDUCE_TREE_PUT");
  
  
  info->collective_algorithms[GASNET_COLL_REDUCE_OP][GASNETE_COLL_REDUCE_TREE_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCE_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           smallest_scratch/info->team->total_ranks, 0, 1,
                                           0,NULL,(void*)gasnete_coll_reduce_TreeGet, "REDUCE_TREE_GET");
  
  {
    size_t smallest_seg_size = MIN(MIN(gasnet_AMMaxLongRequest(),smallest_scratch/info->team->total_ranks),GASNET_COLL_MIN_PIPE_SEG_SIZE);
    size_t largest_seg_size = MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch/info->team->total_ranks);
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE, smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    info->collective_algorithms[GASNET_COLL_REDUCE_OP][GASNETE_COLL_REDUCE_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCE_OP, 
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0,
                                             smallest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, 
                                             smallest_seg_size, 1,
                                             1,tuning_params,(void*)gasnete_coll_reduce_TreePutSeg, "REDUCE_TREE_PUT_SEG");
  }
  
  
  info->collective_algorithms[GASNET_COLL_REDUCEM_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_REDUCEM_NUM_ALGS);
  
  
  info->collective_algorithms[GASNET_COLL_REDUCEM_OP][GASNETE_COLL_REDUCEM_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCEM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           gasnete_coll_p2p_eager_scale, 0, 1,
                                           0,NULL,(void*)gasnete_coll_reduceM_TreeEager, "REDUCEM_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_REDUCEM_OP][GASNETE_COLL_REDUCEM_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCEM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnet_AMMaxLongRequest(),smallest_scratch/info->team->total_ranks), 0, 1,
                                           0,NULL,(void*)gasnete_coll_reduceM_TreePut, "REDUCEM_TREE_PUT");
  
  
  info->collective_algorithms[GASNET_COLL_REDUCEM_OP][GASNETE_COLL_REDUCEM_TREE_GET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCEM_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           smallest_scratch/info->team->total_ranks, 0, 1,
                                           0,NULL,(void*)gasnete_coll_reduceM_TreeGet, "REDUCEM_TREE_GET");
  
  {
    size_t smallest_seg_size = MIN(MIN(gasnet_AMMaxLongRequest(),smallest_scratch/info->team->total_ranks),GASNET_COLL_MIN_PIPE_SEG_SIZE);
    size_t largest_seg_size = MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch/info->team->total_ranks);
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNET_COLL_PIPE_SEG_SIZE, smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM}
    }; 
    info->collective_algorithms[GASNET_COLL_REDUCEM_OP][GASNETE_COLL_REDUCEM_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCEM_OP, 
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0,
                                             smallest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, 
                                             smallest_seg_size, 1,
                                             1,tuning_params,(void*)gasnete_coll_reduceM_TreePutSeg, "REDUCEM_TREE_PUT_SEG");
  }
}


void gasnete_coll_register_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  gasnete_coll_register_broadcast_collectives(info, smallest_scratch);
  gasnete_coll_register_scatter_collectives(info, smallest_scratch);
  gasnete_coll_register_gather_collectives(info, smallest_scratch);
  gasnete_coll_register_gather_all_collectives(info, smallest_scratch);
  gasnete_coll_register_exchange_collectives(info, smallest_scratch);
  gasnete_coll_register_reduce_collectives(info, smallest_scratch);

}



#define GASNETE_COLL_AUTOTUNE_WARM_ITERS_DEFAULT 5
#define GASNETE_COLL_AUTOTUNE_PERF_ITERS_DEFAULT 10
#define GASNETE_COLL_FLAT_TREE_LIMIT 64

static gasneti_lifo_head_t gasnete_coll_autotune_tree_node_free_list = GASNETI_LIFO_INITIALIZER;

gasnete_coll_autotune_tree_node_t *gasnete_coll_get_autotune_tree_node(void) {
  gasnete_coll_autotune_tree_node_t *ret;
  ret = gasneti_lifo_pop(&gasnete_coll_autotune_tree_node_free_list);
  if(!ret) {
    ret = (gasnete_coll_autotune_tree_node_t*) gasneti_malloc(sizeof(gasnete_coll_autotune_tree_node_t));
  }
  memset(ret, 0, sizeof(gasnete_coll_autotune_tree_node_t));
  return ret;
}

void gasnete_coll_free_autotune_tree_node(gasnete_coll_autotune_tree_node_t *in) {
  if(in) {
    gasneti_lifo_push(&gasnete_coll_autotune_tree_node_free_list, in);
  }
}

static int allow_conduit_collectives=1;
static char* gasnete_coll_team_all_tuning_file;
gasnete_coll_autotune_info_t* gasnete_coll_autotune_init(gasnet_team_handle_t team, gasnet_node_t mynode, gasnet_node_t total_nodes,
                                                         gasnet_image_t my_images, gasnet_image_t total_images, size_t min_scratch_size GASNETE_THREAD_FARG) {

  /* read all the environment variables and setup the defaults*/
  gasnete_coll_autotune_info_t* ret;
  char *default_tree_type;
  gasnet_node_t default_tree_fanout;
  size_t dissem_limit;
  size_t temp_size;
  size_t dissem_limit_per_thread;
	int i;
  gasnet_node_t fanout;
  
  ret = gasneti_calloc(1,sizeof(gasnete_coll_autotune_info_t));
  team->autotune_info = ret;
  ret->team = team;
  /* first read the environment variables for tree types*/
  default_tree_type = gasneti_getenv_withdefault("GASNET_COLL_ROOTED_GEOM", GASNETE_COLL_DEFAULT_TREE_TYPE_STR);
   
  /* now over-ride the defaults w/ the collective specific tree types in the environment*/
  ret->bcast_tree_type = gasnete_coll_make_tree_type_str(gasneti_getenv_withdefault("GASNET_COLL_BROADCAST_GEOM", default_tree_type));
  ret->scatter_tree_type = gasnete_coll_make_tree_type_str(gasneti_getenv_withdefault("GASNET_COLL_SCATTER_GEOM", default_tree_type));
  ret->gather_tree_type = gasnete_coll_make_tree_type_str(gasneti_getenv_withdefault("GASNET_COLL_GATHER_GEOM", default_tree_type));
  
  dissem_limit_per_thread = gasneti_getenv_int_withdefault("GASNET_COLL_GATHER_ALL_DISSEM_LIMIT_PER_THREAD", GASNETE_COLL_DEFAULT_DISSEM_LIMIT_PER_THREAD, 1);
  temp_size = gasnete_coll_nextpower2(dissem_limit_per_thread*my_images);
  dissem_limit = gasneti_getenv_int_withdefault("GASNET_COLL_GATHER_ALL_DISSEM_LIMIT", temp_size, 1);
  if(temp_size != dissem_limit) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting environment values for GASNET_COLL_GATHER_ALL_DISSEM_LIMIT (%ld) and GASNET_COLL_GATHER_ALL_DISSEM_LIMIT_PER_THREAD (%ld)\n", (long int) dissem_limit, (long int) dissem_limit_per_thread);
      fprintf(stderr, "WARNING: Using: %ld\n", (long int) MIN(dissem_limit, temp_size));
    }
  }
  ret->gather_all_dissem_limit = MIN(dissem_limit, temp_size);
  
  dissem_limit_per_thread = gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_LIMIT_PER_THREAD", GASNETE_COLL_DEFAULT_DISSEM_LIMIT_PER_THREAD, 1);
  temp_size = gasnete_coll_nextpower2(dissem_limit_per_thread*my_images*my_images);
  dissem_limit = gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_LIMIT", temp_size, 1);
  if(temp_size != dissem_limit) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting environment values for GASNET_COLL_EXCHANGE_DISSEM_LIMIT (%ld) and GASNET_COLL_EXCHANGE_DISSEM_LIMIT_PER_THREAD (%ld)\n", (long int) dissem_limit, (long int) temp_size);
      fprintf(stderr, "WARNING: Using: %ld\n", (long int) MIN(dissem_limit, temp_size));
    }
  }
  ret->exchange_dissem_limit = MIN(dissem_limit, temp_size);
  ret->exchange_dissem_radix = MIN(gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_RADIX", 2, 0),total_images);

  if(min_scratch_size < total_images) {
    gasneti_fatalerror("SCRATCH SPACE TOO SMALL Please set it to at least (%ld bytes) through the GASNET_COLL_SCRATCH_SIZE environment variable", (long int) total_images);
  }
  ret->pipe_seg_size = gasneti_getenv_int_withdefault("GASNET_COLL_PIPE_SEG_SIZE", MIN(min_scratch_size, gasnet_AMMaxLongRequest())/total_images, 1);
  /*  if(ret->pipe_seg_size == 0) {
   ret->pipe_seg_size = MIN(min_scratch_size, gasnet_AMMaxLongRequest())/total_images;
   } 
   */
  if(ret->pipe_seg_size*total_images > min_scratch_size) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting evnironment values for scratch space allocated (%d bytes) and GASNET_COLL_PIPE_SEG_SIZE (%d bytes)\n", (int) min_scratch_size, (int)ret->pipe_seg_size);
      fprintf(stderr, "WARNING: Using %d bytes for GASNET_COLL_PIPE_SEG_SIZE\n", (int)(min_scratch_size/total_images));
    } 
    ret->pipe_seg_size = min_scratch_size/(total_images);
  } 
  
  if(ret->pipe_seg_size*total_images > gasnet_AMMaxLongRequest()) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: GASNET_COLL_PIPE_SEG_SIZE (%d bytes) * total images (%d) has to be less than max size for an AMLong for this conduit (%ld)\n", 
              (int)ret->pipe_seg_size, (int)total_images, (long int) gasnet_AMMaxLongRequest());
      fprintf(stderr, "WARNING: Using %ld bytes for GASNET_COLL_PIPE_SEG_SIZE instead\n", (long int) gasnet_AMMaxLongRequest()/total_images);
      ret->pipe_seg_size = gasnet_AMMaxLongRequest()/total_images;
    }
    
  } 
  if(ret->pipe_seg_size == 0) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: GASNET_COLL_PIPE_SEG_SIZE has been set to 0 bytes\n");
      fprintf(stderr, "WARNING: Disabling Optimized Rooted Collectives\n");
    } 
    
  }
  
	/*initialize the autotune size array to 2 so we always get a binary tree*/
	for(i=0; i<GASNETE_COLL_AUTOTUNE_RADIX_ARR_LEN; i++) {
		ret->bcast_tree_radix_limits[i] = 3;
	}
  
  ret->warm_iters = gasneti_getenv_int_withdefault("GASNET_COLL_AUTOTUNE_WARM_ITERS", GASNETE_COLL_AUTOTUNE_WARM_ITERS_DEFAULT, 0);
  ret->perf_iters = gasneti_getenv_int_withdefault("GASNET_COLL_AUTOTUNE_PERF_ITERS", GASNETE_COLL_AUTOTUNE_PERF_ITERS_DEFAULT, 0);
  ret->allow_flat_tree = gasneti_getenv_int_withdefault("GASNET_COLL_AUTOTUNE_ALLOW_FLAT_TREE", 
                                                        (team->total_ranks <= GASNETE_COLL_FLAT_TREE_LIMIT ? 1 : 0), 0);

  gasnete_coll_register_collectives(ret, min_scratch_size);
#if GASNETE_COLL_CONDUIT_COLLECTIVES
  allow_conduit_collectives = gasneti_getenv_yesno_withdefault("GASNET_COLL_ALLOW_CONDUIT_COLLECTIVES", allow_conduit_collectives);
  if(allow_conduit_collectives) {
    gasnete_coll_register_conduit_collectives(ret);
  }
#endif

  if(team == GASNET_TEAM_ALL){
    gasnete_coll_team_all_tuning_file = gasneti_getenv_withdefault("GASNET_COLL_TUNING_FILE",NULL);
    gasnete_coll_print_autotuner_timers = gasneti_getenv_yesno_withdefault("GASNET_COLL_PRINT_AUTOTUNE_TIMER", GASNETE_COLL_PRINT_TIMERS);
  }
  
  ret->autotuner_defaults = NULL;
  ret->search_enabled = gasneti_getenv_yesno_withdefault("GASNET_COLL_ENABLE_SEARCH", 0);
  ret->profile_enabled = gasneti_getenv_yesno_withdefault("GASNET_COLL_ENABLE_PROFILE", 0);
  
  return ret;
}



#define GASNETE_COLL_AUTOTUNE_BARRIER(TEAM) do { \
    gasnet_coll_barrier_notify(TEAM, 0,GASNET_BARRIERFLAG_ANONYMOUS); \
    gasnet_coll_barrier_wait(TEAM, 0, GASNET_BARRIERFLAG_ANONYMOUS); \
} while (0)

gasnete_coll_tree_type_t gasnete_coll_autotune_get_bcast_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                                   gasnet_coll_optype_t op_type, 
                                                                   gasnet_node_t root, size_t nbytes, int flags) {
	gasnete_coll_tree_type_t ret;
	/*first check if we've seen this size*/
	/*find the log of the transfer size we are interested in*/
	uint32_t log2_nbytes;
#if PLATFORM_ARCH_32
	log2_nbytes = fast_log2_32bit(nbytes);
#else
	log2_nbytes = fast_log2_64bit(nbytes);
#endif
	
	if(autotune_info->bcast_tree_radix_limits[log2_nbytes] == -1) {
		int radix = 0; 
		/*perform search across fanouts*/
		/* do a barrier to ensure all threads have arrived*/
		GASNETE_COLL_AUTOTUNE_BARRIER(autotune_info->team);
    
		
		
		GASNETE_COLL_AUTOTUNE_BARRIER(autotune_info->team);
	} else {
    /*for larger arrays just use the maximum setting that we've already found*/
		ret = gasnete_coll_make_tree_type_str((char*) "KNOMIAL_TREE,2");
	}
	
	return ret;
  
}

gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                             gasnet_coll_optype_t op_type, 
                                                             gasnet_node_t root, size_t nbytes, int flags) {
  switch(op_type) {
	  case GASNET_COLL_BROADCAST_OP:
    case GASNET_COLL_BROADCASTM_OP: 
    case GASNET_COLL_REDUCE_OP:
    case GASNET_COLL_REDUCEM_OP:
      return autotune_info->bcast_tree_type;  
      
	  case GASNET_COLL_SCATTER_OP: 
    case GASNET_COLL_SCATTERM_OP:  
      return autotune_info->scatter_tree_type;
      
    case GASNET_COLL_GATHER_OP:
  	case GASNET_COLL_GATHERM_OP:
      return autotune_info->gather_tree_type;
      
  	default: gasneti_fatalerror("unknown tree based collective op type"); return autotune_info->bcast_tree_type;
  }
}


size_t gasnete_coll_get_dissem_limit(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags) {
  switch(op_type) {
    case GASNET_COLL_GATHER_ALL_OP:
    case GASNET_COLL_GATHER_ALLM_OP: 
      return autotune_info->gather_all_dissem_limit;
    case GASNET_COLL_EXCHANGE_OP: 
    case GASNET_COLL_EXCHANGEM_OP: 
      return autotune_info->exchange_dissem_limit;
    default:  gasneti_fatalerror("unknown dissem based collective op type"); return 0;
  }
}

int gasnete_coll_get_dissem_radix(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags) {
  switch(op_type) {
  case GASNET_COLL_EXCHANGE_OP: 
    case GASNET_COLL_EXCHANGEM_OP: 
      return autotune_info->exchange_dissem_radix;
  default: gasneti_fatalerror("op doesn't specify dissem radix");   return 0;
  }

}

size_t gasnete_coll_get_pipe_seg_size(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags){
  return autotune_info->pipe_seg_size;
}


int gasnet_coll_get_num_tree_classes(gasnete_coll_team_t team, gasnet_coll_optype_t optype) {
  return (int) GASNETE_COLL_NUM_TREE_CLASSES;
}



void gasnet_coll_set_tree_kind(gasnete_coll_team_t team, int tree_class, int fanout, gasnet_coll_optype_t optype) {
  
  switch(optype) {
    case GASNET_COLL_BROADCAST_OP: 
    case GASNET_COLL_BROADCASTM_OP: 
    case GASNET_COLL_REDUCE_OP:
    case GASNET_COLL_REDUCEM_OP:
      gasnete_coll_free_tree_type(team->autotune_info->bcast_tree_type);
      team->autotune_info->bcast_tree_type = gasnete_coll_make_tree_type(tree_class, &fanout,1); break;
    case GASNET_COLL_SCATTER_OP:
    case GASNET_COLL_SCATTERM_OP:
      gasnete_coll_free_tree_type(team->autotune_info->scatter_tree_type);
      team->autotune_info->scatter_tree_type = gasnete_coll_make_tree_type(tree_class, &fanout,1); break;
    case GASNET_COLL_GATHER_OP:
    case GASNET_COLL_GATHERM_OP:    
      gasnete_coll_free_tree_type(team->autotune_info->gather_tree_type);
      team->autotune_info->gather_tree_type = gasnete_coll_make_tree_type(tree_class, &fanout,1); break;
    default: gasneti_fatalerror("unknown tree based collective op");
  }
  return;
}

void gasnet_coll_set_dissem_limit(gasnete_coll_team_t team, size_t dissemlimit, gasnet_coll_optype_t optype) {
  switch(optype) {
    case GASNET_COLL_GATHER_ALL_OP:
    case GASNET_COLL_GATHER_ALLM_OP:
      team->autotune_info->gather_all_dissem_limit = dissemlimit; break;
    case GASNET_COLL_EXCHANGE_OP:
    case GASNET_COLL_EXCHANGEM_OP:
      team->autotune_info->exchange_dissem_limit = dissemlimit; break;
    default:  gasneti_fatalerror("unknown dissem based collective op type"); break;
  }
  return;
}


int gasnet_coll_get_num_params(gasnet_team_handle_t team, gasnet_coll_optype_t op, uint32_t algorithm_num) {
  return team->autotune_info->collective_algorithms[op][algorithm_num].num_parameters;
}
struct gasnet_coll_tuning_parameter_t gasnet_coll_get_param(gasnet_team_handle_t team, gasnet_coll_optype_t op, uint32_t algorithm_num, uint32_t param_idx){
  gasneti_assert(param_idx < team->autotune_info->collective_algorithms[op][algorithm_num].num_parameters);
  return team->autotune_info->collective_algorithms[op][algorithm_num].parameter_list[param_idx];
}



static gasneti_lifo_head_t gasnete_coll_impl_free_list = GASNETI_LIFO_INITIALIZER;
gasnete_coll_implementation_t gasnete_coll_get_implementation(void) {
  gasnete_coll_implementation_t ret;

  ret = gasneti_lifo_pop(&gasnete_coll_impl_free_list);
  if(!ret) {
    ret = (gasnete_coll_implementation_t) gasneti_malloc(sizeof(struct gasnete_coll_implementation_t_));
  }
  memset(ret, 0, sizeof(struct gasnete_coll_implementation_t_));
  return ret;
}

void gasnete_coll_free_implementation(gasnete_coll_implementation_t in){
  if(in!=NULL) {
    gasneti_lifo_push(&gasnete_coll_impl_free_list, in);
  }
}


static char* print_op_str(char *buf, gasnet_coll_optype_t op, int flags) {
  
  switch(op) {
  case GASNET_COLL_BROADCAST_OP:
    sprintf(buf, "BROADCAST SINGLE/");
    break;
  case GASNET_COLL_BROADCASTM_OP:
    sprintf(buf, "BROADCAST MULTI/");
    break;
  case GASNET_COLL_SCATTER_OP:
    sprintf(buf, "SCATTER SINGLE/");
    break;
  case GASNET_COLL_SCATTERM_OP:
    sprintf(buf, "SCATTER MULTI/");
    break;
    case GASNET_COLL_GATHER_OP:
      sprintf(buf, "GATHER SINGLE/");
      break;
    case GASNET_COLL_GATHERM_OP:
      sprintf(buf, "GATHER MULTI/");
      break;
    case GASNET_COLL_GATHER_ALL_OP:
      sprintf(buf, "GATHER_ALL SINGLE/");
      break;
    case GASNET_COLL_GATHER_ALLM_OP:
      sprintf(buf, "GATHER_ALL MULTI/");
      break;
    case GASNET_COLL_EXCHANGE_OP:
      sprintf(buf, "EXCHANGE SINGLE/");
      break;
    case GASNET_COLL_EXCHANGEM_OP:
      sprintf(buf, "EXCHANGE MULTI/");
      break;
    case GASNET_COLL_REDUCE_OP:
      sprintf(buf, "REDUCE SINGLE/");
      break;
    case GASNET_COLL_REDUCEM_OP:
      sprintf(buf, "REDUCE MULTI/");
      break;
      
  default:
    sprintf(buf, "FILLIN");
    
  }

  if(flags & GASNET_COLL_LOCAL) {
    strncat(buf, "LOCAL", 100);
  } else {
    strncat(buf, "SINGLE", 100);
  }
  return buf;
}

static char* print_flag_str(char *outstr, int flags) {
  
  if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    sprintf(outstr, "no/no");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    sprintf(outstr, "no/my");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    sprintf(outstr, "no/all");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    sprintf(outstr, "my/no");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    sprintf(outstr, "my/my");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    sprintf(outstr, "my/all");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    sprintf(outstr, "all/no");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    sprintf(outstr, "all/my");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    sprintf(outstr, "all/all");
  }
  return outstr;
}


#define STRINGS_MATCH(STR_A, STR_B) (strcmp(STR_A, STR_B)==0)
GASNETI_INLINE(get_syncmode_from_flags)
gasnete_coll_syncmode_t get_syncmode_from_flags(int flags) {
  
  if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    return GASNETE_COLL_NONO;
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    return GASNETE_COLL_NOMY;
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    return GASNETE_COLL_NOALL;
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    return GASNETE_COLL_MYNO;
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    return GASNETE_COLL_MYMY;
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    return GASNETE_COLL_MYALL;
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    return GASNETE_COLL_ALLNO;
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    return GASNETE_COLL_ALLMY;
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    return GASNETE_COLL_ALLALL;
  }
  return -1;
}

static gasnete_coll_syncmode_t get_syncmode_from_str(char *str) {
  if(STRINGS_MATCH(str, "no/no")) return GASNETE_COLL_NONO;
  else if(STRINGS_MATCH(str, "no/my")) return GASNETE_COLL_NOMY;
  else if(STRINGS_MATCH(str, "no/all")) return GASNETE_COLL_NOALL;
  else if(STRINGS_MATCH(str, "my/no")) return GASNETE_COLL_MYNO;
  else if(STRINGS_MATCH(str, "my/my")) return GASNETE_COLL_MYMY;
  else if(STRINGS_MATCH(str, "my/all")) return GASNETE_COLL_MYALL;
  else if(STRINGS_MATCH(str, "all/no")) return GASNETE_COLL_ALLNO;
  else if(STRINGS_MATCH(str, "all/my")) return GASNETE_COLL_ALLMY;
  else if(STRINGS_MATCH(str, "all/all")) return GASNETE_COLL_ALLALL;
  gasneti_fatalerror("unknown syncmode from str %s", str);
  return (gasnete_coll_syncmode_t)(-1); /* NOT REACHED */
}

static char* syncmode_to_str(char *buffer, gasnete_coll_syncmode_t mode) {
  switch (mode) {
    case GASNETE_COLL_NONO:
      sprintf(buffer, "no/no");
      break;
    case GASNETE_COLL_NOMY:
      sprintf(buffer, "no/my");
      break;
    case GASNETE_COLL_NOALL:
      sprintf(buffer, "no/all");
      break;
    case GASNETE_COLL_MYNO:
      sprintf(buffer, "my/no");
      break;
    case GASNETE_COLL_MYMY:
      sprintf(buffer, "my/my");
      break;
    case GASNETE_COLL_MYALL:
      sprintf(buffer, "my/all");
      break;
    case GASNETE_COLL_ALLNO:
      sprintf(buffer, "all/no");
      break;
    case GASNETE_COLL_ALLMY:
      sprintf(buffer, "all/my");
      break;
    case GASNETE_COLL_ALLALL:
      sprintf(buffer, "all/all");
      break;
    default:
      gasneti_fatalerror("unknown syncmode");
      break;
  }
  return buffer;
}

static gasnete_coll_addr_mode_t get_addrmode_from_str(char *str) { 
  if(STRINGS_MATCH(str, "single")) 
    return GASNETE_COLL_SINGLE_MODE;
  else if(STRINGS_MATCH(str, "local"))
    return GASNETE_COLL_LOCAL_MODE;
  else if(STRINGS_MATCH(str, "thread_local")) {
    return GASNETE_COLL_THREAD_LOCAL_MODE;
  }
  return -1;
}

static char * addrmode_to_str(char *buffer, gasnete_coll_addr_mode_t mode) {
  switch(mode){
    case GASNETE_COLL_SINGLE_MODE:
      sprintf(buffer, "single");
      break;
    case GASNETE_COLL_LOCAL_MODE:
      sprintf(buffer, "local");
      break;
    case GASNETE_COLL_THREAD_LOCAL_MODE:
      sprintf(buffer, "thread_local");
      break;
    default:
      gasneti_fatalerror("unknown address mode");
  }
  return buffer;
} 

GASNETI_INLINE(get_addrmode_from_flags)
gasnete_coll_addr_mode_t get_addrmode_from_flags(int flags) { 
  if(flags & GASNET_COLL_SINGLE) 
    return GASNETE_COLL_SINGLE_MODE;
  else if(flags & GASNET_COLL_LOCAL) {
#if GASNET_PAR
    if(flags & GASNETE_COLL_THREAD_LOCAL) {
      return GASNETE_COLL_THREAD_LOCAL_MODE;
    } else 
#endif
    {
      return GASNETE_COLL_LOCAL_MODE;
    }
  }
  
  return -1;
}

static gasnet_coll_optype_t get_optype_from_str(char *str) { 
 
  if(STRINGS_MATCH(str, "broadcast")) 
    return GASNET_COLL_BROADCAST_OP;
  else if(STRINGS_MATCH(str, "broadcastM"))
    return GASNET_COLL_BROADCASTM_OP;
  
  else if(STRINGS_MATCH(str, "scatter")) 
    return GASNET_COLL_SCATTER_OP;
  else if(STRINGS_MATCH(str, "scatterM"))
    return GASNET_COLL_SCATTERM_OP;
  
  else if(STRINGS_MATCH(str, "gather")) 
    return GASNET_COLL_GATHER_OP;
  else if(STRINGS_MATCH(str, "gatherM"))
    return GASNET_COLL_GATHERM_OP;
  
  else if(STRINGS_MATCH(str, "gather_all")) 
    return GASNET_COLL_GATHER_ALL_OP;
  else if(STRINGS_MATCH(str, "gather_allM"))
    return GASNET_COLL_GATHER_ALLM_OP;
  
  else if(STRINGS_MATCH(str, "exchange")) 
    return GASNET_COLL_EXCHANGE_OP;
  else if(STRINGS_MATCH(str, "exchangeM"))
    return GASNET_COLL_EXCHANGEM_OP;
  
  else if(STRINGS_MATCH(str, "reduce"))
    return GASNET_COLL_REDUCE_OP;
  else if(STRINGS_MATCH(str, "reduceM"))
    return GASNET_COLL_REDUCEM_OP;
  
  else gasneti_fatalerror("op %s not yet supported\n", str);
  return (gasnet_coll_optype_t)(-1); /* NOT REACHED */
}

static char * optype_to_str(char *buffer, gasnet_coll_optype_t op) {
  switch (op) {
    case GASNET_COLL_BROADCAST_OP:
      sprintf(buffer, "broadcast");
      break;
    case GASNET_COLL_BROADCASTM_OP:
      sprintf(buffer, "broadcastM");
      break;  
    case GASNET_COLL_SCATTER_OP:
      sprintf(buffer, "scatter");
      break;
    case GASNET_COLL_SCATTERM_OP:
      sprintf(buffer, "scatterM");
      break;
    case GASNET_COLL_GATHER_OP:
      sprintf(buffer, "gather");
      break;
    case GASNET_COLL_GATHERM_OP:
      sprintf(buffer, "gatherM");
      break;
    case GASNET_COLL_GATHER_ALL_OP:
      sprintf(buffer, "gather_all");
      break;
    case GASNET_COLL_GATHER_ALLM_OP:
      sprintf(buffer, "gather_allM");
      break;
    case GASNET_COLL_EXCHANGE_OP:
      sprintf(buffer, "gather_all");
      break;
    case GASNET_COLL_EXCHANGEM_OP:
      sprintf(buffer, "gather_allM");
      break;
    case GASNET_COLL_REDUCE_OP:
      sprintf(buffer, "reduce");
      break;
      
    case GASNET_COLL_REDUCEM_OP:
      sprintf(buffer, "reduceM");
      break;
    default:
      gasneti_fatalerror("unknown op type");
  }
  return buffer;
}

/************************/
/***LOAD THE TUNING FILE*/
/************************/

static gasnete_coll_autotune_index_entry_t *load_autotuner_defaults_helper(gasnete_coll_autotune_info_t *info, myxml_node_t *parent, const char **tag_strings, int level, int max_levels, gasnet_coll_optype_t optype) {
  int i;
 
  gasnete_coll_autotune_index_entry_t *array = gasneti_calloc(sizeof(struct gasnete_coll_autotune_index_entry_t_),MYXML_NUM_CHILDREN(parent));
  gasnete_coll_autotune_index_entry_t *temp = array;
  gasneti_assert(STRINGS_MATCH(MYXML_TAG(MYXML_CHILDREN(parent)[0]),tag_strings[level]));
  for(i=0; i<MYXML_NUM_CHILDREN(parent); i++) {
    myxml_node_t *child_node = MYXML_CHILDREN(parent)[i];
    temp->node_type = tag_strings[level];
    
    if(STRINGS_MATCH(tag_strings[level], "sync_mode")) {
      temp->start = get_syncmode_from_str(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else if(STRINGS_MATCH(tag_strings[level], "address_mode")) {
      temp->start = get_addrmode_from_str(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else if(STRINGS_MATCH(tag_strings[level], "collective")) {
      optype = temp->start = get_optype_from_str(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else if(STRINGS_MATCH(tag_strings[level], "size")) {
      temp->start = atoi(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else if(STRINGS_MATCH(tag_strings[level], "threads_per_node")) {
      temp->start = atoi(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else if(STRINGS_MATCH(tag_strings[level], "num_nodes")) {
      temp->start = atoi(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else if(STRINGS_MATCH(tag_strings[level], "root")) {
      temp->start = atoi(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
    } else {
      gasneti_fatalerror("unknown tag string\n");
    }
    if(level == max_levels-1) {
      int j;
      gasneti_assert(STRINGS_MATCH(MYXML_TAG(MYXML_CHILDREN(child_node)[0]), "Best_Alg"));
      gasneti_assert(STRINGS_MATCH(MYXML_TAG(MYXML_CHILDREN(child_node)[1]), "Best_Tree"));
      gasneti_assert(STRINGS_MATCH(MYXML_TAG(MYXML_CHILDREN(child_node)[2]), "Num_Params"));
      
      temp->end = atoi(MYXML_VALUE(MYXML_CHILDREN(child_node)[0]));
      temp->impl = gasnete_coll_get_implementation();
      temp->impl->fn_ptr = info->collective_algorithms[optype][atoi(MYXML_VALUE(MYXML_CHILDREN(child_node)[0]))].fn_ptr.generic_coll_fn_ptr;
      temp->impl->fn_idx = atoi(MYXML_VALUE(MYXML_CHILDREN(child_node)[0]));
      if(strlen(MYXML_VALUE(MYXML_CHILDREN(child_node)[1])) > 0) {
        temp->impl->tree_type = gasnete_coll_make_tree_type_str(MYXML_VALUE(MYXML_CHILDREN(child_node)[1]));
      }
      temp->impl->num_params = atoi(MYXML_VALUE(MYXML_CHILDREN(child_node)[2]));
      if(temp->impl->num_params > 0) {
        for(j=0; j<temp->impl->num_params; j++) {
          temp->impl->param_list[j] = atoi(MYXML_VALUE(MYXML_CHILDREN(child_node)[j+3]));
        }
      }
      /*read and allocate implementations*/
    } else {
      temp->subtree = load_autotuner_defaults_helper(info, MYXML_CHILDREN(parent)[i], tag_strings, level+1, max_levels, optype);
    }
    if(i==MYXML_NUM_CHILDREN(parent)-1) {
      temp->next_interval = NULL;
    } else {
      temp->next_interval = &array[i+1];
      temp = &array[i+1];
    }
  }
  return array;
}

gasnete_coll_autotune_index_entry_t *gasnete_coll_load_autotuner_defaults(gasnete_coll_autotune_info_t* autotune_info, myxml_node_t *tuning_data) {
  myxml_node_t *temp;
  
  gasnete_coll_autotune_index_entry_t *root;
  const char *tree_levels[8] = {"machine", "num_nodes", "threads_per_node", "sync_mode", "address_mode", "collective", "root", "size"};
  
  /*the root of the tree contains the GASNET config string*/
  /*throw a warning if the tree does not match the current tree*/
  if(STRINGS_MATCH(MYXML_TAG(tuning_data), "machine")) {
    if(!STRINGS_MATCH(MYXML_ATTRIBUTES(tuning_data)[0].attribute_value, GASNET_CONFIG_STRING)) {
      printf("warning! tuning data's config string: %s does not match current gasnet config string: %s\n", MYXML_ATTRIBUTES(tuning_data)[0].attribute_value, GASNET_CONFIG_STRING);
    } 
    root= load_autotuner_defaults_helper(autotune_info, tuning_data, tree_levels, 1, 8, -1);
  } else gasneti_fatalerror("exepected machine as the root of the tree");
  return root;
}

/****************************/
/****** RUN THE AUTOTUNER ***/
/****************************/

/*run the given op on the given arguments*/
/*and return the best one*/
int gasnete_coll_autotune_barrier(gasnete_coll_team_t team) {
  int ret;
  gasnet_coll_barrier_notify(team, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);
  ret = gasnet_coll_barrier_wait(team, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);
  gasneti_assert_always(ret == GASNET_OK);
  return ret;
}

#define PTHREAD_BARRIER(team, local_pthread_count)  \
  gasnete_coll_autotune_barrier(team)


static gasnett_tick_t run_collective_bench(gasnet_team_handle_t team, gasnet_coll_optype_t op,
                                           gasnet_coll_args_t coll_args, int flags,
                                           gasnete_coll_implementation_t impl, gasnet_coll_overlap_sample_work_t fnptr, void *sample_work_arg GASNETE_THREAD_FARG) {
  int iter;
  gasnett_tick_t start, total;
  gasnet_coll_handle_t handle;

  gasneti_assert(coll_args.rootimg < team->total_images);
  PTHREAD_BARRIER(team, team->my_images);

  for(iter=0; iter<team->autotune_info->warm_iters; iter++) {
    switch(op){
      case GASNET_COLL_BROADCAST_OP:
        handle = (*((gasnete_coll_bcast_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, 
                                                                   coll_args.src[0], coll_args.nbytes, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_BROADCASTM_OP:
        handle = (*((gasnete_coll_bcastM_fn_ptr_t) (impl->fn_ptr)))(team, (void * const *) coll_args.dst, coll_args.rootimg, 
                            coll_args.src[0], coll_args.nbytes, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
      case GASNET_COLL_SCATTER_OP:
        handle = (*((gasnete_coll_scatter_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, 
                                                                   coll_args.src[0], coll_args.nbytes, coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_SCATTERM_OP:
        handle = (*((gasnete_coll_scatterM_fn_ptr_t) (impl->fn_ptr)))(team, (void * const *) coll_args.dst, coll_args.rootimg, 
                                                                    coll_args.src[0], coll_args.nbytes, coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
      case GASNET_COLL_GATHER_OP:
        handle = (*((gasnete_coll_gather_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.rootimg, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                    coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_GATHERM_OP:
        handle = (*((gasnete_coll_gatherM_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.rootimg, coll_args.dst[0], (void * const*) coll_args.src, coll_args.nbytes,
                                                                    coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_ALL_OP:
        handle = (*((gasnete_coll_gather_all_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                        flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_ALLM_OP:
        handle = (*((gasnete_coll_gather_allM_fn_ptr_t) (impl->fn_ptr)))(team,   (void * const*) coll_args.dst, (void * const*) coll_args.src, coll_args.nbytes,
                                                                         flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
        
      case GASNET_COLL_EXCHANGE_OP:
        handle = (*((gasnete_coll_exchange_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                      flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_EXCHANGEM_OP:
        handle = (*((gasnete_coll_exchangeM_fn_ptr_t) (impl->fn_ptr)))(team,   (void * const*) coll_args.dst, (void * const*) coll_args.src, coll_args.nbytes,
                                                                       flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
        
        
      case GASNET_COLL_REDUCE_OP:
        handle = (*((gasnete_coll_reduce_fn_ptr_t) (impl->fn_ptr)))(team,  coll_args.rootimg, coll_args.dst[0], 
                                                                    coll_args.src[0], coll_args.src_blksz, coll_args.src_offset,
                                                                    coll_args.elem_size, coll_args.nbytes/coll_args.elem_size,
                                                                    coll_args.func, coll_args.func_arg, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
      case GASNET_COLL_REDUCEM_OP:
        handle = (*((gasnete_coll_reduceM_fn_ptr_t) (impl->fn_ptr)))(team,  coll_args.rootimg, coll_args.dst[0], 
                                                                    (void* const*)coll_args.src, coll_args.src_blksz, coll_args.src_offset,
                                                                    coll_args.elem_size, coll_args.nbytes/coll_args.elem_size,
                                                                    coll_args.func, coll_args.func_arg, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
        
      default:
        gasneti_fatalerror("collective not yet implemented");  
    }    
  }
  
  PTHREAD_BARRIER(team, team->my_images);

  start = gasnett_ticks_now();
  for(iter=0; iter<team->autotune_info->perf_iters; iter++) {
    switch(op){
      case GASNET_COLL_BROADCAST_OP:
        handle = (*((gasnete_coll_bcast_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, coll_args.src[0], 
                                                                   coll_args.nbytes, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
        
      case GASNET_COLL_BROADCASTM_OP:
        handle = (*((gasnete_coll_bcastM_fn_ptr_t) (impl->fn_ptr)))(team, (void * const *) coll_args.dst, coll_args.rootimg, coll_args.src[0], 
                                                                    coll_args.nbytes, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;  
        
      case GASNET_COLL_SCATTER_OP:
        handle = (*((gasnete_coll_scatter_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, 
                                                                     coll_args.src[0], coll_args.nbytes, coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
        
      case GASNET_COLL_SCATTERM_OP:
        handle = (*((gasnete_coll_scatterM_fn_ptr_t) (impl->fn_ptr)))(team, (void * const *) coll_args.dst, coll_args.rootimg, 
                                                                      coll_args.src[0], coll_args.nbytes, coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
      case GASNET_COLL_GATHER_OP:
        handle = (*((gasnete_coll_gather_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.rootimg, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                    coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_GATHERM_OP:
        handle = (*((gasnete_coll_gatherM_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.rootimg, coll_args.dst[0], (void * const*)coll_args.src, coll_args.nbytes,
                                                                     coll_args.dist, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_ALL_OP:
        handle = (*((gasnete_coll_gather_all_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                        flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_ALLM_OP:
        handle = (*((gasnete_coll_gather_allM_fn_ptr_t) (impl->fn_ptr)))(team,   (void * const*) coll_args.dst, (void * const*) coll_args.src, coll_args.nbytes,
                                                                         flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_EXCHANGE_OP:
        handle = (*((gasnete_coll_exchange_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                      flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
      case GASNET_COLL_EXCHANGEM_OP:
        handle = (*((gasnete_coll_exchangeM_fn_ptr_t) (impl->fn_ptr)))(team,   (void * const*) coll_args.dst, (void * const*) coll_args.src, coll_args.nbytes,
                                                                       flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break;
        
      case GASNET_COLL_REDUCE_OP:
        handle = (*((gasnete_coll_reduce_fn_ptr_t) (impl->fn_ptr)))(team,  coll_args.rootimg, coll_args.dst[0], 
                                                                    coll_args.src[0], coll_args.src_blksz, coll_args.src_offset,
                                                                    coll_args.elem_size, coll_args.nbytes/coll_args.elem_size,
                                                                    coll_args.func, coll_args.func_arg, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
        
      case GASNET_COLL_REDUCEM_OP:
        handle = (*((gasnete_coll_reduceM_fn_ptr_t) (impl->fn_ptr)))(team,  coll_args.rootimg, coll_args.dst[0], 
                                                                     (void* const*)coll_args.src, coll_args.src_blksz, coll_args.src_offset,
                                                                     coll_args.elem_size, coll_args.nbytes/coll_args.elem_size,
                                                                     coll_args.func, coll_args.func_arg, flags, impl, 0 GASNETE_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
        break; 
      default:
        gasneti_fatalerror("collective not yet implemented");  
    }    
  }

  PTHREAD_BARRIER(team, team->my_images);
  
  total = gasnett_ticks_now()-start;
  return total;

}


static void do_tuning_loop(gasnet_team_handle_t team, gasnet_coll_optype_t op,
                           gasnet_coll_args_t coll_args, int flags,
                           gasnet_coll_overlap_sample_work_t fnptr, void *sample_work_arg,
                           int alg_idx, gasnett_tick_t *best_time,  uint32_t *best_param_list, char *best_tree, int current_param_number, uint32_t *curr_idx_in  GASNETE_THREAD_FARG) {
  int idx;
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  /*no tuning parameters*/
  if(gasnet_coll_get_num_params(team, op, alg_idx)==0) {
    gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
    impl->fn_ptr = team->autotune_info->collective_algorithms[op][alg_idx].fn_ptr.generic_coll_fn_ptr;
    impl->fn_idx = alg_idx;
    *best_time = run_collective_bench(team, op, coll_args, flags, impl, fnptr, sample_work_arg GASNETE_THREAD_PASS);
    if(td->my_image==0 && gasnete_coll_print_autotuner_timers) {
      int i;
      char buf1[100];
      char buf2[100];
      
      printf("%d> %s alg: %s (%d) syncflags: %s nbytes: %d params:<", (int)td->my_image, print_op_str(buf1, op, flags), team->autotune_info->collective_algorithms[op][alg_idx].name_str, alg_idx,
             print_flag_str(buf2, flags), (int) coll_args.nbytes);
      
      
      for(i=0; i<impl->num_params; i++) {
        printf(" %d", (int)impl->param_list[i]);
      }
      printf(" > time: %g\n", (double)gasnett_ticks_to_us(*best_time)/team->autotune_info->perf_iters); 
    }
    gasnete_coll_free_implementation(impl);
    return;
  } else { /*algorithm has parameters*/
    struct gasnet_coll_tuning_parameter_t param = gasnet_coll_get_param(team, op, alg_idx, current_param_number);
    uint32_t *curr_idx= curr_idx_in;
    int needtofree_idx=0;
    if(curr_idx==NULL) {
      gasneti_assert(current_param_number == 0);
      needtofree_idx = 1;
      curr_idx=gasneti_malloc(sizeof(uint32_t)*gasnet_coll_get_num_params(team, op, alg_idx));
    }
                            
    idx = param.start;
    gasneti_assert(idx<=param.end);
    while (1) {
      /*if the tuning paramter is a size parameter and it already exceeds the value for our collective
        then skip this iteration*/
      if(!(team->autotune_info->collective_algorithms[op][alg_idx].parameter_list[current_param_number].flags & GASNET_COLL_TUNING_SIZE_PARAM && idx > coll_args.nbytes)) {
        
        if(current_param_number == team->autotune_info->collective_algorithms[op][alg_idx].num_parameters-1) {
          /*this is the last one so run the collective*/
          gasnett_tick_t curr_run;
          /*setup the collective information*/
          gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
          curr_idx[current_param_number]=idx;
          impl->fn_ptr = team->autotune_info->collective_algorithms[op][alg_idx].fn_ptr.generic_coll_fn_ptr;
          impl->fn_idx = alg_idx;
          impl->num_params = team->autotune_info->collective_algorithms[op][alg_idx].num_parameters;
          GASNETE_FAST_UNALIGNED_MEMCPY(impl->param_list, curr_idx, impl->num_params*sizeof(uint32_t));
          if(team->autotune_info->collective_algorithms[op][alg_idx].parameter_list[current_param_number].flags & GASNET_COLL_TUNING_TREE_SHAPE)
            impl->tree_type = gasnete_coll_autotune_get_tree_type_idx(team, idx);
          
          
          /*run the measurement iterations*/
          curr_run = run_collective_bench(team, op,coll_args, flags, impl, fnptr, sample_work_arg GASNETE_THREAD_PASS);

          if(td->my_image==0 && gasnete_coll_print_autotuner_timers) {
            char buf1[100];
            char buf2[100];
            int i;
            printf("%d> %s alg: %s (%d) syncflags: %s nbytes: %d params:<", (int)td->my_image, print_op_str(buf1, op, flags), team->autotune_info->collective_algorithms[op][alg_idx].name_str, alg_idx,
                   print_flag_str(buf2, flags), (int) coll_args.nbytes);
            for(i=0; i<impl->num_params; i++) {
              if(team->autotune_info->collective_algorithms[op][alg_idx].parameter_list[i].flags & GASNET_COLL_TUNING_TREE_SHAPE){  
                gasnete_coll_tree_type_to_str((char *) buf1, impl->tree_type);
                printf(" %s", buf1);
              }else {
                printf(" %d", (int)impl->param_list[i]);
              }       
            }
            printf(" > time: %g\n", (double)gasnett_ticks_to_us(curr_run)/team->autotune_info->perf_iters); 
          }
          /*if the time is less than the best set this one as the new best*/
          if(curr_run < *best_time) {
            *best_time = curr_run;
            GASNETE_FAST_UNALIGNED_MEMCPY(best_param_list, curr_idx, impl->num_params*sizeof(uint32_t));
            strcpy(best_tree, "");
            if(team->autotune_info->collective_algorithms[op][alg_idx].parameter_list[current_param_number].flags & GASNET_COLL_TUNING_TREE_SHAPE){
              gasnete_coll_tree_type_to_str(best_tree, impl->tree_type);
            } 
          }
          
          gasnete_coll_free_implementation(impl);
        } else {
          /*there are still more iterations to set so continue down the loop nest*/
          curr_idx[current_param_number]=idx;
          do_tuning_loop(team, op, coll_args, flags, fnptr, sample_work_arg, alg_idx, best_time, best_param_list, best_tree, current_param_number+1, curr_idx GASNETE_THREAD_PASS);
        }
      }
      if(param.flags & GASNET_COLL_TUNING_STRIDE_ADD) {
        idx+=param.stride;
      } else if(param.flags & GASNET_COLL_TUNING_STRIDE_MULTIPLY) {
        idx*=param.stride;
      }
      if(idx > param.end) break;
    }
    if(needtofree_idx) {
      gasneti_assert(current_param_number == 0);
      gasneti_free(curr_idx);
    }
  }
}


void gasnete_coll_tune_generic_op(gasnet_team_handle_t team, gasnet_coll_optype_t op, 
                                  gasnet_coll_args_t coll_args, int flags,
                                  gasnet_coll_overlap_sample_work_t fnptr, void *sample_work_arg,
                                  /*returned by the function*/
                                  uint32_t *best_algidx, uint32_t *num_params, uint32_t **best_param, char **best_tree GASNETE_THREAD_FARG)  {
  int algidx = 0;
  int num_algs;
  gasnett_tick_t curr_best_time=GASNETT_TICK_MAX, alg_best_time=GASNETT_TICK_MAX;
  int loc_num_params;
  uint32_t loc_best_param_list[GASNET_COLL_NUM_PARAM_TYPES];
  uint32_t sync_flags = (flags &  GASNET_COLL_SYNC_FLAG_MASK); /*strip the sync flags off the flags*/
  uint32_t req_flags = (flags & (~GASNET_COLL_SYNC_FLAG_MASK));
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  char *loc_best_tree;

  loc_best_tree = gasneti_calloc(1,sizeof(char)*100);
  

  switch (op) {
    case GASNET_COLL_BROADCAST_OP:
      num_algs = GASNETE_COLL_BROADCAST_NUM_ALGS;
      break;
    case GASNET_COLL_BROADCASTM_OP:
      num_algs = GASNETE_COLL_BROADCASTM_NUM_ALGS;
      break;
    case GASNET_COLL_SCATTER_OP:
      num_algs = GASNETE_COLL_SCATTER_NUM_ALGS;
      break;
    case GASNET_COLL_SCATTERM_OP:
      num_algs = GASNETE_COLL_SCATTERM_NUM_ALGS;
      break;
    case GASNET_COLL_GATHER_OP:
      num_algs = GASNETE_COLL_GATHER_NUM_ALGS;
      break;
    case GASNET_COLL_GATHERM_OP:
      num_algs = GASNETE_COLL_GATHERM_NUM_ALGS;
      break;
    case GASNET_COLL_GATHER_ALL_OP:
      num_algs = GASNETE_COLL_GATHER_ALL_NUM_ALGS;
      break;
    case GASNET_COLL_GATHER_ALLM_OP:
      num_algs = GASNETE_COLL_GATHER_ALLM_NUM_ALGS;
      break;
    case GASNET_COLL_EXCHANGE_OP:
      num_algs = GASNETE_COLL_EXCHANGE_NUM_ALGS;
      break;
    case GASNET_COLL_EXCHANGEM_OP:
      num_algs = GASNETE_COLL_EXCHANGEM_NUM_ALGS;
      break;
    case GASNET_COLL_REDUCE_OP:
      num_algs = GASNETE_COLL_REDUCE_NUM_ALGS;
      break;
    case GASNET_COLL_REDUCEM_OP:
      num_algs = GASNETE_COLL_REDUCEM_NUM_ALGS;
      break;
    default:
      gasneti_fatalerror("not yet supported");
      break;
  }
  
  *best_algidx = 0;
  PTHREAD_BARRIER(team, team->my_images);
  for (algidx=0; algidx<num_algs; algidx++) {
    
    int size_ok = (coll_args.nbytes <= team->autotune_info->collective_algorithms[op][algidx].max_num_bytes && 
		   coll_args.nbytes >= team->autotune_info->collective_algorithms[op][algidx].min_num_bytes);
    /*ensure that all the flags required by the algorithm are passed in through the flags*/
    int req_flags_ok = ((req_flags & team->autotune_info->collective_algorithms[op][algidx].requirements) == team->autotune_info->collective_algorithms[op][algidx].requirements);
    /*ensure that the synchronization flags exist in the list of possible synch flags for this algorithm*/
    int sync_flags_ok = ((sync_flags & team->autotune_info->collective_algorithms[op][algidx].syncflags) == sync_flags);
    int nreq_flags_ok = (!(req_flags & team->autotune_info->collective_algorithms[op][algidx].n_requirements));
#if GASNET_DEBUG
    if(!size_ok){if(td->my_image==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: size too large)\n", (int)gasneti_mynode, algidx);continue;}
    if(!req_flags_ok){if(td->my_image==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: all req flags are not present)\n", (int)gasneti_mynode, algidx);continue;}
    if(!nreq_flags_ok){if(td->my_image==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: one of the nreq flags is present)\n", (int)gasneti_mynode, algidx);continue;}
    if(!sync_flags_ok){if(td->my_image==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: not valid for this syncflag)\n", (int)gasneti_mynode, algidx);continue;}
    
#else
    if(!(size_ok && req_flags_ok && sync_flags_ok &&  nreq_flags_ok/*match!*/)) {
      continue;
    }
#endif
    
    PTHREAD_BARRIER(team, team->my_images);
     if((op == GASNET_COLL_BROADCASTM_OP && algidx == GASNETE_COLL_BROADCASTM_SCATTERALLGATHER) || 
       (op == GASNET_COLL_BROADCAST_OP && algidx == GASNETE_COLL_BROADCAST_SCATTERALLGATHER)) continue;
     if((op == GASNET_COLL_EXCHANGEM_OP && algidx == GASNETE_COLL_EXCHANGEM_GATH) || 
        (op == GASNET_COLL_GATHER_ALLM_OP && algidx == GASNETE_COLL_GATHER_ALLM_GATH)) continue;
     if((op == GASNET_COLL_SCATTERM_OP && algidx ==   GASNETE_COLL_SCATTERM_TREE_PUT_SEG) ||
	(op == GASNET_COLL_GATHERM_OP && algidx ==   GASNETE_COLL_GATHERM_TREE_PUT_SEG)) continue;
	
    alg_best_time = curr_best_time;

    do_tuning_loop(team, op, coll_args, flags, fnptr, sample_work_arg, 
                   algidx, &alg_best_time, loc_best_param_list, loc_best_tree, 0, NULL GASNETE_THREAD_PASS);
    
    /*if this tuning iteration pass has beaten the best we've seen so far set it to be the new best*/
    if(alg_best_time < curr_best_time) {
      *best_algidx = algidx;
      curr_best_time = alg_best_time;
      if(!team->autotune_info->collective_algorithms[op][algidx].tree_alg) {
        memset(loc_best_tree, 0, sizeof(char)*100);
      } else {
        gasneti_assert(strlen(loc_best_tree)>0);
      }
    }
  }
  /*take the best time that we've seen so far and then copy out the number of parameters to it*/
  /*the tuning loop will set the loc_best_param_list with the appropriate parameters so we just have to copy it out and return it*/
  *num_params = gasnet_coll_get_num_params(team, op, *best_algidx);
  *best_param = gasneti_malloc(sizeof(uint32_t)*gasnet_coll_get_num_params(team, op, *best_algidx));
  GASNETE_FAST_UNALIGNED_MEMCPY(*best_param, loc_best_param_list, sizeof(uint32_t)*(*num_params));
  *best_tree = gasneti_calloc(strlen(loc_best_tree)+1,sizeof(char));
  strcpy(*best_tree, loc_best_tree);
  gasneti_free(loc_best_tree);
}

/*************************/
/***LOAD THE OPERATIONS***/
/*************************/

GASNETI_INLINE(search_intervals)
gasnete_coll_autotune_index_entry_t *search_intervals(gasnete_coll_autotune_index_entry_t *idx, int search_value, int exact_match) {
  gasnete_coll_autotune_index_entry_t *temp = idx;
  gasnete_coll_autotune_index_entry_t *ret = temp;
  if(!exact_match) {
    if(search_value < temp->start) {
      return ret;
    } 
    while(temp!=NULL) {
      if(search_value >= temp->start) {
        ret = temp;
        temp = temp->next_interval;
        if(temp) continue;
        else return ret;
      } else {
        /*the pervious interval had the answer*/
        return ret;
      }
    }
  } else {
    while(temp!=NULL) {
      if(search_value == temp->start) {
        return temp;
      } else {
        temp = temp->next_interval;
      }
    }
  }
  return NULL;
}

static
gasnete_coll_implementation_t search_index(gasnet_coll_optype_t op, gasnete_coll_team_t team, uint32_t flags, size_t nbytes, gasnet_image_t rootimg, int exact_match) {

  gasnete_coll_autotune_index_entry_t *temp = team->autotune_info->autotuner_defaults;
  if(!temp) return NULL;

  
  /*first go through and pick out the right subtree for the threads per node*/
  temp = search_intervals(temp, team->total_ranks, exact_match);
  if(!temp) return NULL;
  
  temp = search_intervals(temp->subtree, team->my_images,exact_match);
  if(!temp) return NULL;
  
  /*next get the sync mode (need to find an exact match)*/
  temp = search_intervals(temp->subtree, get_syncmode_from_flags(flags),1);
  if(!temp) return NULL;
  
  /*lookup the address mode (need to find an exact match)*/
  temp = search_intervals(temp->subtree, get_addrmode_from_flags(flags),1);
  if(!temp) return NULL;
  
  /*loookup the op (need to find an exact match)*/
  temp = search_intervals(temp->subtree, op,1);
  if(!temp) return NULL;
  
  /*loookup the root*/
  temp = search_intervals(temp->subtree, rootimg,exact_match);
  if(!temp) return NULL;
  
  /*approximate match for size is ok*/
  temp = search_intervals(temp->subtree, nbytes,exact_match);
  if(!temp) return NULL;
  
  return temp->impl;
}

static
gasnete_coll_autotune_index_entry_t* add_interval(gasnete_coll_autotune_index_entry_t *list, uint32_t value, const char *node_type) {
  gasnete_coll_autotune_index_entry_t *current_head = list;
  gasnete_coll_autotune_index_entry_t *temp = list;
  
  if(!list) {
    current_head = gasneti_calloc(1, sizeof(gasnete_coll_autotune_index_entry_t));
    current_head->start = value;
    current_head->node_type = node_type;
    return current_head;
  } else {
    /*value is less than the current head so preappend*/
    if(current_head->start > value) {
      temp = gasneti_calloc(1, sizeof(gasnete_coll_autotune_index_entry_t));
      temp->start = value;
      temp->node_type = node_type;
      temp->next_interval = current_head;
      return temp;
    }
    while(current_head) {
      if(current_head->start == value) {
        /*nothing todo*/
        return list;
      } else if(current_head->next_interval) {
        /*check if the search value is between this and the next value*/
        /*if it is append and return it*/
        if(current_head->next_interval->start > value && current_head->start < value) {
          temp = gasneti_calloc(1, sizeof(gasnete_coll_autotune_index_entry_t));
          temp->start = value;
          temp->node_type = node_type;
          temp->next_interval = current_head->next_interval;
          current_head->next_interval = temp;
          return list;
        } else {
          current_head = current_head->next_interval;
        }
      } else {
        /*end of list and no where to be found*/
        temp = gasneti_calloc(1, sizeof(gasnete_coll_autotune_index_entry_t));
        temp->start = value;
        temp->node_type = node_type;
        current_head->next_interval = temp;
        return list;
      }
    }
  }
  return NULL;
}

static
gasnete_coll_autotune_index_entry_t *add_to_index(gasnet_coll_optype_t op, gasnete_coll_team_t team, uint32_t flags, 
                                                  size_t nbytes, gasnet_image_t rootimg, int profile_mode) {
  gasnete_coll_autotune_index_entry_t *idx;
  gasnete_coll_autotune_index_entry_t *temp; 
  gasnete_coll_implementation_t impl;
  
  if(profile_mode) {
    idx = team->autotune_info->collective_profile;
    team->autotune_info->collective_profile = idx = add_interval(idx, team->total_ranks, "num_nodes");
    temp = search_intervals(idx, team->total_ranks, 1);
    gasneti_assert(temp);
    
  } else {
    idx = team->autotune_info->autotuner_defaults;
    team->autotune_info->autotuner_defaults = idx = add_interval(idx, team->total_ranks, "num_nodes");
    temp = search_intervals(idx, team->total_ranks, 1);
    gasneti_assert(temp);
  }
  temp->subtree = add_interval(temp->subtree, team->my_images, "threads_per_node");
  temp = search_intervals(temp->subtree, team->my_images, 1);
  gasneti_assert(temp);
  
  temp->subtree = add_interval(temp->subtree,  get_syncmode_from_flags(flags), "sync_mode");
  temp = search_intervals(temp->subtree, get_syncmode_from_flags(flags), 1);
  gasneti_assert(temp);
  
  temp->subtree = add_interval(temp->subtree,  get_addrmode_from_flags(flags), "address_mode");
  temp = search_intervals(temp->subtree, get_addrmode_from_flags(flags), 1);
  gasneti_assert(temp);

  temp->subtree = add_interval(temp->subtree,  op, "collective");
  temp = search_intervals(temp->subtree, op, 1);
  gasneti_assert(temp);
  
  temp->subtree = add_interval(temp->subtree,  rootimg, "root");
  temp = search_intervals(temp->subtree, rootimg, 1);
  gasneti_assert(temp);

  temp->subtree = add_interval(temp->subtree,  nbytes, "size");
  temp = search_intervals(temp->subtree, nbytes, 1);
  gasneti_assert(temp);
  
  return temp;

}
void gasnete_coll_safe_broadcast(gasnete_coll_team_t team, void *dst, void *src, gasnet_image_t root, size_t nbytes, int nodes_only GASNETE_THREAD_FARG) {
  int flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC | GASNET_COLL_LOCAL |  GASNET_COLL_DISABLE_AUTOTUNE;
  if(nodes_only) 
    flags = flags | GASNET_COLL_NO_IMAGES;
  gasnet_coll_broadcast(team, dst, root, src, nbytes, flags);
}


static
int verify_algorithm(gasnete_coll_team_t team, gasnet_coll_optype_t op, uint32_t flags, size_t nbytes, gasnete_coll_implementation_t impl) {
  uint32_t sync_flags = (flags &  GASNET_COLL_SYNC_FLAG_MASK); /*strip the sync flags off the flags*/
  uint32_t req_flags = (flags & (~GASNET_COLL_SYNC_FLAG_MASK));
  int size_ok, req_flags_ok, sync_flags_ok;
  if(!impl) return 0;
  
  size_ok = (team->autotune_info->collective_algorithms[op][impl->fn_idx].max_num_bytes==0 || nbytes <= team->autotune_info->collective_algorithms[op][impl->fn_idx].max_num_bytes);
  /*ensure that all the flags required by the algorithm are passed in through the flags*/
  req_flags_ok = ((req_flags & team->autotune_info->collective_algorithms[op][impl->fn_idx].requirements) == team->autotune_info->collective_algorithms[op][impl->fn_idx].requirements);
  /*ensure that the synchronization flags exist in the list of possible synch flags for this algorithm*/
  sync_flags_ok = ((sync_flags & team->autotune_info->collective_algorithms[op][impl->fn_idx].syncflags) == sync_flags);
  return (size_ok && req_flags_ok && sync_flags_ok);
}

static gasnete_coll_implementation_t autotune_op(gasnet_team_handle_t team, gasnet_coll_optype_t op, gasnet_coll_args_t args, int flags GASNETE_THREAD_FARG) {
  gasnete_coll_implementation_t ret;
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  /* explicit request to not tune */
  if(flags & GASNET_COLL_DISABLE_AUTOTUNE) return NULL;
  
  /*if a tuning file has been specified for TEAM ALL and hasn't been yet loaded load it now*/
  if(team == GASNET_TEAM_ALL && gasnete_coll_team_all_tuning_file && !team->autotune_info->autotuner_defaults) {
    gasnete_coll_loadTuningState(gasnete_coll_team_all_tuning_file, team GASNETE_THREAD_PASS);
  }
  
  if(td->my_local_image == 0  && team->autotune_info->profile_enabled) {
    gasnete_coll_autotune_index_entry_t *idx;
    idx = add_to_index(op, team, flags, args.nbytes, args.rootimg, 1);
    idx->num_calls++;
  }
  
  if(team->autotune_info->autotuner_defaults  || team->autotune_info->search_enabled) {
    ret = search_index(op, team, flags, args.nbytes, args.rootimg, team->autotune_info->search_enabled);  
    /*make sure the returned algortithm can handle the cases*/
    if(verify_algorithm(team, op, flags, args.nbytes, ret)) {
      return ret;
    }    
  }
  /*we searched the index w/ no luck so need to run a tuning run (if enabled)*/
  if(team->autotune_info->search_enabled && (team==GASNET_TEAM_ALL || team->my_images==1)) {
    gasnete_coll_implementation_t temp = gasnete_coll_get_implementation();
    gasnete_coll_autotune_index_entry_t *idx;
    gasnet_image_t myrank;
    uint32_t best_algidx;
    uint32_t num_params;
    uint32_t *param_list;
    char flagstr[15];

    char best_tree[GASNETE_COLL_MAX_TREE_TYPE_STRLEN];
    char all_best_tree[GASNETE_COLL_MAX_TREE_TYPE_STRLEN];
    char *temp_tree_str;
    
    
    ret = gasnete_coll_get_implementation();
    myrank = (team == GASNET_TEAM_ALL ? td->my_image : team->myrank);
#if 0
    if(myrank == srcimage) {
      fprintf(stderr, "%d> starting autotune for %s %d byte broadcast\n", td->my_image, print_flag_str(flagstr, flags), (int)nbytes);
    }
#endif
    gasnete_coll_tune_generic_op(team, op, 
                                 args, flags, 
                                 NULL, NULL,
                                 /*returned by the algorithm*/
                                 &best_algidx, &num_params, &param_list, &temp_tree_str GASNETE_THREAD_PASS);
    /*until we have a solution for teams with srcimages*/
    if(myrank == args.rootimg) {
#if 0
      fprintf(stderr, "%d> finishing autotune for %s %d byte broadcast\n", td->my_image, print_flag_str(flagstr, flags), (int)nbytes);      
#endif
      temp->fn_idx = best_algidx;
      temp->num_params = num_params;
      GASNETE_FAST_UNALIGNED_MEMCPY(temp->param_list, param_list, sizeof(uint32_t)*num_params);
      if(strlen(temp_tree_str) > 0) {
        gasneti_assert(strlen(temp_tree_str)<(GASNETE_COLL_MAX_TREE_TYPE_STRLEN-1));
        strcpy(best_tree, temp_tree_str);
        gasneti_free(temp_tree_str);
      } else {
        memset(best_tree, 0, sizeof(char)*GASNETE_COLL_MAX_TREE_TYPE_STRLEN);
      }
    }
    /*have the root tell all other nodes in this team what the correct implementation is*/
    gasnete_coll_safe_broadcast(team, ret, temp, args.rootimg, sizeof(struct gasnete_coll_implementation_t_), 0 GASNETE_THREAD_PASS);
    memset(all_best_tree, 0, sizeof(char)*GASNETE_COLL_MAX_TREE_TYPE_STRLEN);
    gasnete_coll_safe_broadcast(team, all_best_tree, best_tree, args.rootimg, GASNETE_COLL_MAX_TREE_TYPE_STRLEN*sizeof(char), 0 GASNETE_THREAD_PASS);
    ret->fn_ptr = (void*) team->autotune_info->collective_algorithms[op][ret->fn_idx].fn_ptr.generic_coll_fn_ptr;
    if(strlen(all_best_tree) > 0) {
      ret->tree_type = gasnete_coll_make_tree_type_str(all_best_tree);
    }
    gasnete_coll_free_implementation(temp);
    /*insert ret into the search index*/
    PTHREAD_BARRIER(team, team->my_images); 
    if(td->my_local_image == 0) {
      idx = add_to_index(op, team, flags, args.nbytes, args.rootimg, 0);
      idx->impl = ret;
    }
    
    PTHREAD_BARRIER(team, team->my_images); 
    
    return ret;
  } else {
    return NULL;
  }
}

gasnete_coll_implementation_t gasnete_coll_autotune_get_bcast_algorithm(gasnet_team_handle_t team, void *dst, gasnet_image_t srcimage, void *src, size_t nbytes, uint32_t flags  GASNETE_THREAD_FARG) {
  
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_min, gasnet_AMMaxMedium());

  
  gasnete_coll_implementation_t ret;

  /*first try to search our gasnet autotuner index to see if we have anything for it*/
  /*if not then fall back to our orignal implementation*/

  {
    gasnet_coll_args_t args = {0};
    
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = srcimage;
    args.nbytes = nbytes;
    ret = autotune_op(team, GASNET_COLL_BROADCAST_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  /*autotuning is turned off or we need to fall back to the default*/
  ret = gasnete_coll_get_implementation();
  ret->need_to_free = 1;
  
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_BROADCASTM_OP, 
                                                       -1,nbytes, flags);

  
  /*for now encode the original decision tree*/
  if ((nbytes <= eager_limit) && 
      (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL))) {
    /* Small enough for Eager, which will eliminate any barriers for *_MYSYNC and
     * the need for passing addresses for _LOCAL
     * Eager is totally AM-based and thus safe regardless of *_IN_SEGMENT
     */
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_EAGER].fn_ptr.bcast_fn; 
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT ) {
    /* run the segmented broadcast code 
     function internally checks synch flags and SINGLE/LOCAL flags
     */
    /*this should also be part of the space*/
    if((nbytes > team->total_ranks) && !(flags & GASNETE_COLL_SUBORDINATE) && 0) {       
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_SCATTERALLGATHER].fn_ptr.bcast_fn;
    } else if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_BROADCAST_OP, flags)) {
      if (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL)) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SCRATCH].fn_ptr.bcast_fn;
      } else {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT].fn_ptr.bcast_fn;
      }
    } else if(nbytes<=team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG].max_num_bytes) {
      ret->num_params = 1;
      ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_BROADCAST_OP, flags);  
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG].fn_ptr.bcast_fn;
    } else if(flags & GASNET_COLL_SRC_IN_SEGMENT) {
      ret->num_params = 0;
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_RVGET].fn_ptr.bcast_fn;
    } else {
      ret->num_params = 0;
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVOUS].fn_ptr.bcast_fn;
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    if (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL)) {
      /* We can use Rendezvous+Get to eliminate any barriers for *_MYSYNC.
       * The Rendezvous is needed for _LOCAL.
       */
      ret->num_params = 0;
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVGET].fn_ptr.bcast_fn;
    } else {
      ret->num_params = 0;
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_GET].fn_ptr.bcast_fn;
    }
  }  else {
    /* If we reach here then neither src nor dst is in-segment */
    ret->num_params = 0;
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVOUS].fn_ptr.bcast_fn;
  }
  
  
  return ret;
}


gasnete_coll_implementation_t gasnete_coll_autotune_get_bcastM_algorithm(gasnet_team_handle_t team, void * const dstlist[],
                                                                         gasnet_image_t srcimage, void *src, size_t nbytes, uint32_t flags GASNETE_THREAD_FARG) {
  
  
  gasnete_coll_implementation_t ret;
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_min, gasnet_AMMaxMedium());
 
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)dstlist;
    args.src = (uint8_t**)&src;
    args.rootimg = srcimage;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_BROADCASTM_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  ret->need_to_free = 1;
  ret->num_params =0;

  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_BROADCASTM_OP, 
                                                       -1,nbytes, flags);
  
  /* Choose algorithm based on arguments */
  if ((nbytes <= eager_limit) &&
      (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL))) {
    /* Small enough for Eager, which will eliminate any barriers for *_MYSYNC and
     * the need for passing addresses for _LOCAL
     * Eager is totally AM-based and thus safe regardless of *_IN_SEGMENT
     */       
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_EAGER].fn_ptr.bcastM_fn; 
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_BROADCASTM_OP, flags)) {
      if (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL)) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_PUT_SCRATCH].fn_ptr.bcastM_fn; 
      } else {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_PUT].fn_ptr.bcastM_fn; 
      }
    } else if(nbytes<=team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG].max_num_bytes) {
      ret->num_params = 1;
      ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_BROADCASTM_OP, flags);  
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_PUT_SEG].fn_ptr.bcastM_fn; 
    } else if(flags & GASNET_COLL_SRC_IN_SEGMENT) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_TREE_RVGET].fn_ptr.bcastM_fn; 
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_RVOUS].fn_ptr.bcastM_fn; 
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    if (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL)) {
      /* We can use Rendezvous+Get to eliminate any barriers for *_MYSYNC.
       * The Rendezvous is needed for _LOCAL.
       */
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_RVGET].fn_ptr.bcastM_fn; 
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_GET].fn_ptr.bcastM_fn; 
    }
  } else {
    /* If we reach here then neither src nor dst is in-segment */
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCASTM_RVOUS].fn_ptr.bcastM_fn; 
  }
 
  return ret;
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_scatter_algorithm(gasnet_team_handle_t team, void *dst, gasnet_image_t srcimage, 
                                            void *src, size_t nbytes, size_t dist, uint32_t flags  GASNETE_THREAD_FARG) {
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale/team->my_images, gasnet_AMMaxMedium()/team->total_images);
  gasnete_coll_tree_type_t tree_type;
  gasnete_coll_implementation_t ret;

  {
    gasnet_coll_args_t args = {0};
    
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = srcimage;
    args.nbytes = nbytes;
    args.dist = dist;
    ret = autotune_op(team, GASNET_COLL_SCATTER_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(team, flags, 0, 0, dst, nbytes,
                                     1, srcimage, src, nbytes*gasneti_nodes);
  ret->need_to_free = 1;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                        GASNET_COLL_SCATTER_OP, 
                                                        srcimage, nbytes, flags);
  /* Choose algorithm based on arguments */
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if (nbytes <= eager_limit) {
      /* Small enough for Eager, which works for out-of-segment src and/or dst */
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_EAGER].fn_ptr.scatter_fn;
    } else if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_SCATTER_OP, flags)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_NO_COPY].fn_ptr.scatter_fn;
    } else {
      ret->num_params = 1;
      ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_SCATTER_OP, flags);
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_SEG].fn_ptr.scatter_fn;
    }
  } else if (nbytes <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_EAGER].fn_ptr.scatter_fn;

  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_GET].fn_ptr.scatter_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVGET].fn_ptr.scatter_fn;  
    }
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_PUT].fn_ptr.scatter_fn;
    } else {
      /* XXX: could do better since DST is in-segment */
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS].fn_ptr.scatter_fn;
    }
  } else {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS].fn_ptr.scatter_fn;
  }
  
  return ret;
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_scatterM_algorithm(gasnet_team_handle_t team, void * const dstlist[], gasnet_image_t srcimage, 
                                            void *src, size_t nbytes, size_t dist, uint32_t flags  GASNETE_THREAD_FARG) {

  gasnete_coll_tree_type_t tree_type;
  gasnete_coll_implementation_t ret;
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale/team->my_images, gasnet_AMMaxMedium()/team->total_images);
  {
    gasnet_coll_args_t args = {0};
    
    args.dst = (uint8_t**) dstlist;
    args.src = (uint8_t**)&src;
    args.rootimg = srcimage;
    args.nbytes = nbytes;
    args.dist = dist;
    ret = autotune_op(team, GASNET_COLL_SCATTERM_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  ret->need_to_free = 1;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                        GASNET_COLL_SCATTERM_OP, 
                                                        srcimage, nbytes, flags);
  /* Choose algorithm based on arguments */
  if(!team->fixed_image_count) {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVOUS].fn_ptr.scatterM_fn;
    return ret;
  }
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if(team->fixed_image_count) {
      if (nbytes*team->my_images <= eager_limit) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_EAGER].fn_ptr.scatterM_fn;
      } else if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_SCATTERM_OP, flags)) {
        /* require that all ndoes have the same number of GASNet images*/
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_PUT].fn_ptr.scatterM_fn;
      } else {
        ret->num_params = 1;
        ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_SCATTERM_OP, flags);
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_TREE_PUT_SEG].fn_ptr.scatterM_fn;
      }
    } else if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes*team->my_images <= eager_limit) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_EAGER].fn_ptr.scatterM_fn;
      } else {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVGET].fn_ptr.scatterM_fn;
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes*team->my_images <= eager_limit)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_EAGER].fn_ptr.scatterM_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_GET].fn_ptr.scatterM_fn;
    }
  } else if (nbytes*team->my_images <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_EAGER].fn_ptr.scatterM_fn;
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_GET].fn_ptr.scatterM_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVGET].fn_ptr.scatterM_fn;
    }
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_PUT].fn_ptr.scatterM_fn;
    } else {
      /* Could do better since dst is in-segment */
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVOUS].fn_ptr.scatterM_fn;
    }
  } else {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTERM_RVOUS].fn_ptr.scatterM_fn;
  }
  return ret;
  

}
  
gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_algorithm(gasnet_team_handle_t team,gasnet_image_t dstimage, void *dst, void *src, 
                                           size_t nbytes, size_t dist, uint32_t flags  GASNETE_THREAD_FARG) {
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale/team->my_images, gasnet_AMMaxMedium()/team->total_images);
  gasnete_coll_implementation_t ret;
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = dstimage;
    args.nbytes = nbytes;
    args.dist = dist;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_GATHER_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_GATHER_OP, 
                                                       dstimage, nbytes, flags);
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    if (nbytes <= eager_limit) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
    } else if(!(flags & GASNETE_COLL_NONROOT_SUBORDINATE)) {
      if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_GATHER_OP, flags)) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_NO_COPY].fn_ptr.gather_fn;
      } else {
        ret->num_params = 1;
        ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_GATHER_OP, flags);
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_SEG].fn_ptr.gather_fn;
      }
    } else if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes <= eager_limit) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
      } else {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVPUT].fn_ptr.gather_fn;
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes <= eager_limit)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_PUT].fn_ptr.gather_fn;
    }
  } else if (nbytes <=eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_PUT].fn_ptr.gather_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVPUT].fn_ptr.gather_fn;
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_GET].fn_ptr.gather_fn;
    } else {
      /* XXX: could do better since src is in-segment */
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS].fn_ptr.gather_fn;
    }
  } else {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS].fn_ptr.gather_fn;
  }
  return ret;
  
  
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gatherM_algorithm(gasnet_team_handle_t team,gasnet_image_t dstimage, void *dst, void * const srclist[], 
                                            size_t nbytes, size_t dist, uint32_t flags  GASNETE_THREAD_FARG) {
  gasnete_coll_implementation_t ret;
    const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale/team->my_images, gasnet_AMMaxMedium()/team->total_images);

  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)srclist;
    args.rootimg = dstimage;
    args.nbytes = nbytes;
    args.dist = dist;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_GATHERM_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_GATHERM_OP, 
                                                       dstimage, nbytes, flags);

   
  if(!team->fixed_image_count) {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVOUS].fn_ptr.gatherM_fn;
  } else if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if(!(flags & GASNETE_COLL_NONROOT_SUBORDINATE) && team->fixed_image_count) {
      /*XXX: with the current implementation of the scratch space it does not make sense to use this for
       all-gather and exchange since it will serialize the gathers which is not what we want
       */
      if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_GATHERM_OP, flags)) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_PUT].fn_ptr.gatherM_fn;
      } else {
        ret->num_params = 1;
        ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_GATHER_OP, flags);
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_PUT_SEG].fn_ptr.gatherM_fn;
      }
    } else if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes <= eager_limit) {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_EAGER].fn_ptr.gatherM_fn;
        
      } else {
        ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVPUT].fn_ptr.gatherM_fn;
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes <= eager_limit)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_EAGER].fn_ptr.gatherM_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_PUT].fn_ptr.gatherM_fn;
    }
  } else if (nbytes <= eager_limit && team->fixed_image_count) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_TREE_EAGER].fn_ptr.gatherM_fn;
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_PUT].fn_ptr.gatherM_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVPUT].fn_ptr.gatherM_fn;
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_GET].fn_ptr.gatherM_fn;
    } else {
      /* XXX: could do better since src is in-segment */
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVOUS].fn_ptr.gatherM_fn;    }
  } else {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHERM_RVOUS].fn_ptr.gatherM_fn;
  }
  
  return ret;
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_all_algorithm(gasnet_team_handle_t team, void *dst, void *src, 
                                               size_t nbytes, uint32_t flags  GASNETE_THREAD_FARG) {
  
  size_t max_dissem_msg_size = team->total_images*nbytes;
  size_t eager_limit = gasnete_coll_p2p_eager_scale/team->my_images;
  gasnete_coll_implementation_t ret;
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = 0;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_GATHER_ALL_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  
  if(!team->fixed_image_count) {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_GATH].fn_ptr.gather_all_fn;
  } else if(team->my_images*nbytes <=  gasnete_coll_get_dissem_limit(team->autotune_info, GASNET_COLL_GATHER_ALL_OP, flags) &&
     max_dissem_msg_size <= MIN(team->smallest_scratch_seg, gasnet_AMMaxLongRequest())) {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM].fn_ptr.gather_all_fn;
  } else {
    if((flags & GASNET_COLL_SINGLE) && (flags & GASNET_COLL_DST_IN_SEGMENT)) {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_FLAT_PUT].fn_ptr.gather_all_fn;
    } else {
      ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_GATH].fn_ptr.gather_all_fn;
    }
  }
  
  return ret;
  
  
}


gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_allM_algorithm(gasnet_team_handle_t team, void * const dstlist[], void * const srclist[], 
                                                size_t nbytes, uint32_t flags  GASNETE_THREAD_FARG) {
  
  size_t max_dissem_msg_size = team->total_images*nbytes;
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale/team->my_images, gasnet_AMMaxMedium()/team->total_images);

  gasnete_coll_implementation_t ret;
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)dstlist;
    args.src = (uint8_t**)srclist;
    args.rootimg = 0;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_GATHER_ALLM_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;

  if ((flags & GASNET_COLL_SINGLE) && (flags & GASNET_COLL_DST_IN_SEGMENT) && team->fixed_image_count) {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_DISSEM_NOSCRATCH_SEG].fn_ptr.gather_allM_fn;
  } else {
    ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALLM_GATH].fn_ptr.gather_allM_fn;
  } 
  return ret;
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_exchange_algorithm(gasnet_team_handle_t team, void *dst, void *src, 
                                             size_t nbytes, uint32_t flags  GASNETE_THREAD_FARG) {
  gasnete_coll_implementation_t ret;
  size_t max_dissem_msg_size = (team->my_images*team->my_images*nbytes)*(team->total_ranks/2+(team->total_ranks%2));
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = 0;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_EXCHANGE_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  
  if (nbytes <=  gasnete_coll_get_dissem_limit(team->autotune_info, GASNET_COLL_EXCHANGE_OP, flags) &&
      nbytes*team->total_images+(max_dissem_msg_size*2)<= team->smallest_scratch_seg  &&
      max_dissem_msg_size <=  gasnet_AMMaxLongRequest() &&
      team->fixed_image_count) {
    ret->fn_ptr =  (void*)team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM2].fn_ptr.exchange_fn;
  } else if(flags & GASNET_COLL_DST_IN_SEGMENT){
    if(flags & GASNET_COLL_SINGLE) {
      ret->fn_ptr =  (void*)team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_PUT].fn_ptr.exchange_fn;
    } else{
      ret->fn_ptr =  (void*)team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_RVPUT].fn_ptr.exchange_fn;
    }
  } else {
    ret->fn_ptr =  (void*)team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_GATH].fn_ptr.exchange_fn;
  }
  return ret;
  
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_exchangeM_algorithm(gasnet_team_handle_t team, void * const dstlist[], void * const srclist[], 
                                             size_t nbytes, uint32_t flags  GASNETE_THREAD_FARG) {
  gasnete_coll_implementation_t ret;
  size_t max_dissem_msg_size = (team->my_images*team->my_images*nbytes)*(team->total_ranks/2+(team->total_ranks%2));
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)dstlist;
    args.src = (uint8_t**)srclist;
    args.rootimg = 0;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_EXCHANGEM_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  if (team->my_images*team->my_images*nbytes <=  gasnete_coll_get_dissem_limit(team->autotune_info, GASNET_COLL_EXCHANGEM_OP, flags) &&
      team->my_images*nbytes*team->total_images+(max_dissem_msg_size*2) <= team->smallest_scratch_seg &&
      max_dissem_msg_size <= gasnet_AMMaxLongRequest() && 
      team->fixed_image_count)  {
    ret->fn_ptr =  (void*)team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_DISSEM2].fn_ptr.exchangeM_fn;
  } else {
    ret->fn_ptr =  (void*)team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGEM_GATH].fn_ptr.exchangeM_fn;
  }
  return ret;
}
  

gasnete_coll_implementation_t gasnete_coll_autotune_get_reduce_algorithm(gasnet_team_handle_t team, gasnet_image_t dstimage, void *dst, void * src,
                                                                          size_t src_blksz, size_t src_offset, size_t elem_size, size_t elem_count,
                                                                          gasnet_coll_fn_handle_t func, int func_arg,
                                                                          uint32_t flags GASNETE_THREAD_FARG){
  gasnete_coll_implementation_t ret;
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = dstimage;
    args.src_blksz = src_blksz;
    args.src_offset = src_offset;
    args.elem_size = elem_size;
    args.nbytes = elem_count * elem_size;
    args.func = func;
    args.func_arg = func_arg;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_REDUCE_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  ret->need_to_free = 1;
  ret->num_params =0;
  
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_REDUCE_OP, 
                                                       -1,elem_count*elem_size, flags);
  
  ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_REDUCE_OP][GASNETE_COLL_REDUCE_TREE_GET].fn_ptr.reduce_fn;
  
  return ret;
}

gasnete_coll_implementation_t gasnete_coll_autotune_get_reduceM_algorithm(gasnet_team_handle_t team, gasnet_image_t dstimage, void *dst, void * const srclist[],
                                                                          size_t src_blksz, size_t src_offset, size_t elem_size, size_t elem_count,
                                                                          gasnet_coll_fn_handle_t func, int func_arg,
                                                                          uint32_t flags GASNETE_THREAD_FARG){
  gasnete_coll_implementation_t ret;
  {
    gasnet_coll_args_t args = {0};
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)srclist;
    args.rootimg = dstimage;
    args.src_blksz = src_blksz;
    args.src_offset = src_offset;
    args.elem_size = elem_size;
    args.nbytes = elem_count * elem_size;
    args.func = func;
    args.func_arg = func_arg;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_REDUCEM_OP, args, flags GASNETE_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  ret->need_to_free = 1;
  ret->num_params =0;
  
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_REDUCEM_OP, 
                                                       -1,elem_count*elem_size, flags);
  
  ret->fn_ptr = (void*)team->autotune_info->collective_algorithms[GASNET_COLL_REDUCEM_OP][GASNETE_COLL_REDUCEM_TREE_GET].fn_ptr.reduceM_fn;
  
  return ret;
}


static void dump_tuning_state_helper(myxml_node_t *parent, gasnete_coll_autotune_index_entry_t *tuning_root) {
  int i;
  gasnete_coll_autotune_index_entry_t *temp=tuning_root;
  while(temp!=NULL) {
    char buffer[50];
    myxml_node_t *temp_xml;
    if(STRINGS_MATCH(temp->node_type, "sync_mode")) {
      syncmode_to_str(buffer, temp->start); 
      temp_xml =myxml_createNode(parent, temp->node_type, (char*)"val", buffer, NULL);
    } else if(STRINGS_MATCH(temp->node_type, "address_mode")) {
      addrmode_to_str(buffer, temp->start);
      temp_xml =myxml_createNode(parent, temp->node_type, (char*)"val", buffer, NULL);
    } else if(STRINGS_MATCH(temp->node_type, "collective")) {
      optype_to_str(buffer, temp->start);
      temp_xml =myxml_createNode(parent, temp->node_type, (char*)"val", buffer, NULL);
    } else {
      temp_xml =  myxml_createNodeInt(parent, temp->node_type, (char*)"val", temp->start, NULL);
    }
    if(temp->subtree) {
      dump_tuning_state_helper(temp_xml, temp->subtree);
    } else {
      char buffer[GASNETE_COLL_MAX_TREE_TYPE_STRLEN];
      char tempbuffer[20];
      int c;
      gasneti_assert(temp->impl);
      gasnete_coll_tree_type_to_str(buffer, temp->impl->tree_type);
      sprintf(tempbuffer, "%d",  temp->impl->fn_idx);
      myxml_createNode(temp_xml, (char*) "Best_Alg", NULL, NULL,tempbuffer);
      myxml_createNode(temp_xml, (char*) "Best_Tree", NULL, NULL, buffer);
      sprintf(tempbuffer, "%d",  temp->impl->num_params);
      myxml_createNode(temp_xml, (char*) "Num_Params", NULL, NULL, tempbuffer);


      for(c=0; c<temp->impl->num_params; c++) {
        char buff_idx[20];
        sprintf(tempbuffer, "%d",  temp->impl->fn_idx);
        sprintf(buff_idx, "param_%d", c);
        sprintf(buffer, "%d", (int)temp->impl->param_list[c]);
        myxml_createNode(temp_xml, buff_idx, NULL, NULL, buffer);
      }
    }
    temp = temp->next_interval;
  }
}


void gasnete_coll_dumpTuningState(char *filename, gasnete_coll_team_t team GASNETE_THREAD_FARG) {
  myxml_node_t *node;
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnet_image_t myrank = (team == GASNET_TEAM_ALL ? td->my_image : team->myrank);


  if(myrank==0 && team->autotune_info->search_enabled) {
    FILE *outstream;
    node = myxml_createNode(NULL, (char*) "machine", (char*)"CONFIG", (char*) GASNET_CONFIG_STRING, NULL);
    
    if(!filename) {
      if(team!=GASNET_TEAM_ALL) {fprintf(stderr, "WARNING: printing tuning output to default filename is not recommended for non-TEAM-ALL teams\n");}
      outstream = fopen("gasnet_coll_tuning_defaults.bin", "w");
    } else {
      outstream = fopen(filename, "w");
    }
    dump_tuning_state_helper(node, team->autotune_info->autotuner_defaults);
    myxml_printTreeBIN(outstream, node);
    fclose(outstream);
  }
}

static void dump_profile_helper(myxml_node_t *parent, gasnete_coll_autotune_index_entry_t *tuning_root) {
  int i;
  gasnete_coll_autotune_index_entry_t *temp=tuning_root;
  while(temp!=NULL) {
    char buffer[50];
    myxml_node_t *temp_xml;
    if(STRINGS_MATCH(temp->node_type, "sync_mode")) {
      syncmode_to_str(buffer, temp->start); 
      temp_xml =myxml_createNode(parent, temp->node_type, (char*)"val", buffer, NULL);
    } else if(STRINGS_MATCH(temp->node_type, "address_mode")) {
      addrmode_to_str(buffer, temp->start);
      temp_xml =myxml_createNode(parent, temp->node_type, (char*)"val", buffer, NULL);
    } else if(STRINGS_MATCH(temp->node_type, "collective")) {
      optype_to_str(buffer, temp->start);
      temp_xml =myxml_createNode(parent, temp->node_type, (char*)"val", buffer, NULL);
    } else {
      temp_xml =  myxml_createNodeInt(parent, temp->node_type, (char*)"val", temp->start, NULL);
    }
    if(temp->subtree) {
      dump_profile_helper(temp_xml, temp->subtree);
    } else {
      char tempbuffer[10];
      sprintf(tempbuffer, "%d",  temp->num_calls);
      myxml_createNode(temp_xml, (char*) "Num_Calls", NULL, NULL, tempbuffer);
    }
    temp = temp->next_interval;
  }
}


void gasnete_coll_dumpProfile(char *filename, gasnete_coll_team_t team GASNETE_THREAD_FARG) {
  myxml_node_t *node;
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnet_image_t myrank = td->my_image; 
  
  
  if(myrank==0 && team->autotune_info->profile_enabled) {
    FILE *outstream;
    node = myxml_createNode(NULL, (char*) "machine", (char*)"CONFIG", (char*) GASNET_CONFIG_STRING, NULL);
    
    if(!filename) {
      if(team!=GASNET_TEAM_ALL) {fprintf(stderr, "WARNING: printing tuning output to default filename is not recommended for non-TEAM-ALL teams\n");}
      outstream = fopen("gasnet_coll_profile.bin", "w");
    } else {
      outstream = fopen(filename, "w");
    }
    dump_profile_helper(node, team->autotune_info->collective_profile);
    myxml_printTreeBIN(outstream, node);
    fclose(outstream);
  }
}

void gasnete_coll_loadTuningState(char *filename, gasnete_coll_team_t team GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  myxml_node_t *nodes;
  gasnet_image_t myrank = team->myrank;
  
  PTHREAD_BARRIER(team, team->my_images);
  if(td->my_local_image == 0) {
    if(myrank == 0) {
      FILE *instream;
      myxml_bytestream_t file_content;
      
      if(!filename) {
        if(team!=GASNET_TEAM_ALL) {fprintf(stderr, "WARNING: loading tuning output to default filename is not recommended for non-TEAM-ALL teams\n");}
        instream = fopen("gasnet_coll_tuning_defaults.bin", "r");
      } else {
        instream = fopen(filename, "r");
      }
      
      /*load the tuning file into a bytestream*/
      if(!instream) return;
      file_content = myxml_loadFile_into_bytestream(instream);
      
      /*initiate a broadcast to all the other nodes*/
      gasnete_coll_safe_broadcast(team, &file_content.size, &file_content.size, 0, sizeof(size_t), 1 GASNETE_THREAD_PASS);
      gasnete_coll_safe_broadcast(team, file_content.bytes, file_content.bytes, 0, file_content.size, 1 GASNETE_THREAD_PASS);
      nodes = myxml_loadTreeBYTESTREAM(file_content.bytes, file_content.size);
      team->autotune_info->autotuner_defaults = gasnete_coll_load_autotuner_defaults(team->autotune_info, nodes);
    } else {
      size_t nbytes;
      char *buffer;
      /*receive the file size*/
      gasnete_coll_safe_broadcast(team, &nbytes, NULL, 0, sizeof(size_t), 1 GASNETE_THREAD_PASS);
      buffer = gasneti_malloc(sizeof(char)*nbytes);
      /*receive the file contents*/
      gasnete_coll_safe_broadcast(team, buffer, NULL, 0, sizeof(char)*nbytes, 1 GASNETE_THREAD_PASS);
      nodes = myxml_loadTreeBYTESTREAM(buffer, nbytes);
      team->autotune_info->autotuner_defaults = gasnete_coll_load_autotuner_defaults(team->autotune_info, nodes);
    }
  }
  PTHREAD_BARRIER(team, team->my_images);
  
}

