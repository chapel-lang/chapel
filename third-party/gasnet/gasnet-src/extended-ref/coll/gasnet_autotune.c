/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_autotune.c $
 * Description: GASNet Autotuner Implementation
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


/* This is intended as a stub for the autotuner routines*/
#include <coll/gasnet_autotune_internal.h>

typedef struct gasnet_coll_args_t_ {
  uint8_t **dst; 
  uint8_t **src; 
  gasnet_image_t rootimg; 
  size_t src_blksz;
  size_t src_offset;
  size_t elem_size; 
  /*elem count will be nbytes / elem_size*/
  size_t nbytes;
  size_t dist;
} gasnet_coll_args_t;
#define GASNET_COLL_ARGS_INITIALIZER { NULL, NULL, 0, 0, 0, 0, 0, 0 }

/*a small library to write and read XML style sheets for the collective tuner*/
#include <myxml/myxml.h>
#include <myxml/myxml.c>

/*this array is the maximum size of hte log2 array for fanouts*/


#define GASNETE_COLL_PRINT_TIMERS 0
static int gasnete_coll_print_autotuner_timers;
static int gasnete_coll_print_coll_alg;

struct gasnet_coll_tuning_iterator_t_{
  uint32_t num_params;
  struct gasnet_coll_tuning_parameter_t params[GASNET_COLL_NUM_PARAM_TYPES];
  uint32_t param_space_size[GASNET_COLL_NUM_PARAM_TYPES];
  uint32_t curr_pos[GASNET_COLL_NUM_PARAM_TYPES]; /*a position into the parameter space for each parameter*/
  uint32_t max_idx;
  uint32_t curr_idx;
};

GASNETI_INLINE(gasnete_coll_nextpower2)
size_t gasnete_coll_nextpower2(size_t n)
{
  size_t x;
  if(n==0) return 0;
  x=1;
  while(x < n) x<<=1;
  return x;
}

/*register the collective algorithm
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
  int log2_threads = gasnete_coll_log2_rank(MIN((uint32_t) team->total_ranks,128));
  
  return (team->autotune_info->allow_flat_tree ? 1 : 0) + /*flat_tree*/
    log2_threads * (GASNETE_COLL_NUM_PLATFORM_INDEP_TREE_CLASSES-1); /*num powers of two for each of the three tree types*/
}


gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type_idx(gasnet_team_handle_t team, int idx) {
  gasnete_coll_tree_type_t ret = gasnete_coll_get_tree_type();
  int log2_threads = gasnete_coll_log2_rank(MIN((uint32_t) team->total_ranks,128));
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
                                                                  gex_Event_t (*coll_fnptr)(),
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
    case GASNET_COLL_SCATTER_OP: ret.fn_ptr.scatter_fn = (gasnete_coll_scatter_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_GATHER_OP: ret.fn_ptr.gather_fn = (gasnete_coll_gather_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_GATHER_ALL_OP: ret.fn_ptr.gather_all_fn = (gasnete_coll_gather_all_fn_ptr_t) coll_fnptr; break;
    case GASNET_COLL_EXCHANGE_OP: ret.fn_ptr.exchange_fn = (gasnete_coll_exchange_fn_ptr_t) coll_fnptr; break;
    default: gasneti_fatalerror("not implemented yet");
  }
  return ret;
}

static void gasnete_coll_autotune_deregister_algorithm(gasnete_coll_algorithm_t *in) {
  if (in->num_parameters) gasneti_free(in->parameter_list);
}

#define GASNETE_COLL_EVERY_IN_SYNC_FLAG GASNET_COLL_IN_NOSYNC | GASNET_COLL_IN_MYSYNC | GASNET_COLL_IN_ALLSYNC 
#define GASNETE_COLL_EVERY_OUT_SYNC_FLAG GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC 
#define GASNETE_COLL_EVERY_SYNC_FLAG GASNETE_COLL_EVERY_IN_SYNC_FLAG | GASNETE_COLL_EVERY_OUT_SYNC_FLAG

#define GASNETE_COLL_MAX_BYTES ((size_t) -1)
void gasnete_coll_register_broadcast_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch)  {
  
  /*first register all the broadcast algorithms*/
  /*all tuning parameters are inclusinve (i.e. iterations go from for(i=start; i<=end; i+=stride (or) i*=stride)*/
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_BROADCAST_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SCRATCH] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0,
                                           MIN(gex_AM_LUBRequestLong(),smallest_scratch), 0, 1,
                                           0,NULL,gasnete_coll_bcast_TreePutScratch, "BROADCAST_TREE_PUT_SCRATCH");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_SCATTERALLGATHER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags (scatter/allgather will pick their right implementations based on the actual flags)*/ 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,gasnete_coll_bcast_ScatterAllgather, "BROADCAST_SCATTERALLGATHER");
  {
    GASNETE_COLL_TUNING_PARAMETER(tuning_params, GASNET_COLL_PIPE_SEG_SIZE, GASNET_COLL_MIN_PIPE_SEG_SIZE, MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch), 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM); 
    
    size_t largest_seg_size =  MIN(gex_AM_LUBRequestLong(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch));
    info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0,
                                             largest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, GASNET_COLL_MIN_PIPE_SEG_SIZE, 1,
                                             1,tuning_params,gasnete_coll_bcast_TreePutSeg, "BROADCAST_TREE_PUT_SEG");
    
    
  }
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           gasnete_coll_p2p_eager_min, 0, 0,
                                           0,NULL,gasnete_coll_bcast_Eager, "BROADCAST_EAGER");
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           gasnete_coll_p2p_eager_min,0, 1,
                                           0,NULL,gasnete_coll_bcast_TreeEager, "BROADCAST_TREE_EAGER");
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVOUS] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, /*works for all flags as long as size is small enough*/ 0,
                                           GASNETE_COLL_MAX_BYTES, /*works for all sizes*/ 0, 0,
                                           0,NULL,gasnete_coll_bcast_RVous, "BROADCAST_RVOUS");
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, /*works for all sizes*/ 0, 0, 
                                           0,NULL,gasnete_coll_bcast_RVGet, "BROADCAST_RVGET");
  
  
  
  
  info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCAST_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_DST_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES /*works for all sizes*/, 0, 1,
                                           0,NULL,gasnete_coll_bcast_TreeRVGet, "BROADCAST_TREE_RVGET");
}

void gasnete_coll_register_scatter_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  info->collective_algorithms[GASNET_COLL_SCATTER_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_SCATTER_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           MIN(gex_AM_LUBRequestLong(),smallest_scratch)/(info->team->total_ranks), 0, 1,
                                           0, NULL, gasnete_coll_scat_TreePut, "SCATTER_TREE_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_NO_COPY] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0, 
                                           MIN(gex_AM_LUBRequestLong(),smallest_scratch)/(info->team->total_ranks), 0, 1,
                                           0, NULL, gasnete_coll_scat_TreePutNoCopy, "SCATTER_TREE_PUT_NOCOPY");
  {
    size_t smallest_seg_size =  MIN(gex_AM_LUBRequestLong(),GASNET_COLL_MIN_PIPE_SEG_SIZE)/(info->team->total_ranks);
    size_t largest_seg_size =  MIN(gex_AM_LUBRequestLong(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch))/(info->team->total_ranks);
    GASNETE_COLL_TUNING_PARAMETER(tuning_params, GASNET_COLL_PIPE_SEG_SIZE,smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM); 

    info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             largest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, smallest_seg_size, 1,
                                             1, tuning_params, gasnete_coll_scat_TreePutSeg, "SCATTER_TREE_PUT_SEG");
  }

  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gex_AM_LUBRequestMedium()), 0, 0,
                                           0,NULL,gasnete_coll_scat_Eager, "SCATTER_EAGER");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_EAGER] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gex_AM_LUBRequestMedium()/info->team->total_ranks), 0, 1,
                                           0,NULL,gasnete_coll_scat_TreeEager, "SCATTER_TREE_EAGER");

  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVGET] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_SRC_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,gasnete_coll_scat_RVGet, "SCATTER_RVGET");
  
  info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,gasnete_coll_scat_RVous, "SCATTER_RVOUS");
}

void gasnete_coll_register_gather_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  info->collective_algorithms[GASNET_COLL_GATHER_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_GATHER_NUM_ALGS);
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT,  0, 
                                           MIN(gex_AM_LUBRequestLong(),smallest_scratch)/info->team->total_ranks, 0, 1,
                                           0, NULL, gasnete_coll_gath_TreePut, "GATHER_TREE_PUT");

  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_NO_COPY] = 
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT,  0, 
                                           MIN(gex_AM_LUBRequestLong(),smallest_scratch)/info->team->total_ranks, 0, 1,
                                           0, NULL, gasnete_coll_gath_TreePutNoCopy, "GATHER_TREE_PUT_NO_COPY");
  
  {
    size_t smallest_seg_size =  MIN(gex_AM_LUBRequestLong(),GASNET_COLL_MIN_PIPE_SEG_SIZE)/info->team->total_ranks;
    size_t largest_seg_size =  MIN(gex_AM_LUBRequestLong(),MIN(GASNET_COLL_MAX_PIPE_SEG_SIZE,smallest_scratch))/info->team->total_ranks;
    GASNETE_COLL_TUNING_PARAMETER(tuning_params, GASNET_COLL_PIPE_SEG_SIZE,smallest_seg_size, largest_seg_size, 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY | GASNET_COLL_TUNING_SIZE_PARAM); 

    info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_SEG] = 
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             largest_seg_size*GASNETE_COLL_MAX_NUM_SEGS, smallest_seg_size, 1,
                                             1, tuning_params, gasnete_coll_gath_TreePutSeg, "GATHER_TREE_PUT_SEG");
  }
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gex_AM_LUBRequestMedium()/info->team->total_ranks), 0, 1,
                                           0,NULL,gasnete_coll_gath_TreeEager, "GATHER_TREE_EAGER");
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_EAGER]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           MIN(gasnete_coll_p2p_eager_scale, gex_AM_LUBRequestMedium()), 0, 0,
                                           0,NULL,gasnete_coll_gath_Eager, "GATHER_EAGER");
  
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVPUT]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           GASNET_COLL_DST_IN_SEGMENT, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,gasnete_coll_gath_RVPut, "GATHER_RVPUT");
  info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS]=
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_OP, 
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0,
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0,NULL,gasnete_coll_gath_RVous, "GATHER_RVOUS");
  
}

void gasnete_coll_register_gather_all_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_GATHER_ALL_NUM_ALGS);

  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_GATH] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, /*works for every flag (gather will take care of figuiring out right algorithm)*/
                                           GASNETE_COLL_MAX_BYTES, 0, 0,
                                           0, NULL,  gasnete_coll_gall_Gath, "GATHER_ALL_GATH");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM_EAGER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           MIN(gasnete_coll_p2p_eager_scale,gex_AM_LUBRequestMedium()/info->team->total_ranks), 0, 0,
                                           0, NULL,  gasnete_coll_gall_EagerDissem, "GATHER_ALL_EAGER_DISSEM");

  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           MIN(gex_AM_LUBRequestLong(),smallest_scratch)/info->team->total_ranks, 0, 0,
                                           0, NULL,  gasnete_coll_gall_Dissem, "GATHER_ALL_DISSEM");

  info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_FLAT_PUT_EAGER] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALL_OP,
                                           GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0, 0, 
                                           gasnete_coll_p2p_eager_scale, 0, 0,
                                           0, NULL,  gasnete_coll_gall_FlatEagerPut, "GATHER_ALL_FLAT_PUT_EAGER");
}

void gasnete_coll_register_exchange_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  gasnete_coll_dissem_info_t *dissem;
  info->collective_algorithms[GASNET_COLL_EXCHANGE_OP] = gasneti_malloc(sizeof(gasnete_coll_algorithm_t)*GASNETE_COLL_EXCHANGE_NUM_ALGS);
  
  {
    int radix = 2;
    size_t max_size;
    const gasnet_image_t tpn = 1;
    gex_Rank_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gex_AM_LUBRequestLong(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM2] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_Dissem2, "EXCHANGE_DISSEM2");
  }
  {
    int radix = 3;
    size_t max_size;
    const gasnet_image_t tpn = 1;
    gex_Rank_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gex_AM_LUBRequestLong(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM3] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_Dissem3, "EXCHANGE_DISSEM3");
  }
  {
    int radix = 4;
    size_t max_size;
    const gasnet_image_t tpn = 1;
    gex_Rank_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gex_AM_LUBRequestLong(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM4] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_Dissem4, "EXCHANGE_DISSEM4");
  }
  {
    int radix = 8;
    size_t max_size;
    const gasnet_image_t tpn = 1;
    gex_Rank_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gex_AM_LUBRequestLong(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM8] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_Dissem8, "EXCHANGE_DISSEM8");
  }
  {
    int radix = info->team->total_ranks;
    size_t max_size;
    const gasnet_image_t tpn = 1;
    gex_Rank_t nodes = info->team->total_ranks;
    
    
    dissem = gasnete_coll_fetch_dissemination(radix ,info->team);
    /*max size formula is limit/(tpn*tpn*nodes + 2*tpn*tpn*max_dissem_blocks*(radix-1))*/
    max_size = MIN(gex_AM_LUBRequestLong(),smallest_scratch)/ (tpn*tpn*nodes + 2*tpn*tpn*(dissem->max_dissem_blocks)*(radix-1));
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_FLAT_SCRATCH] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             max_size, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_FlatScratch, "EXCHANGE_FLAT_SCRATCH");
  }
  {
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_RVPUT] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             GASNET_COLL_DST_IN_SEGMENT, 0, 
                                             GASNETE_COLL_MAX_BYTES, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_RVPut, "EXCHANGE_RVPUT");
  }
  {
    info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_GATH] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGE_OP,
                                             GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0, 0, 
                                             GASNETE_COLL_MAX_BYTES, 0, 0,
                                             0, NULL,  gasnete_coll_exchg_Gath, "EXCHANGE_GATH");
  }
}


void gasnete_coll_register_collectives(gasnete_coll_autotune_info_t* info, size_t smallest_scratch) {
  gasnete_coll_register_broadcast_collectives(info, smallest_scratch);
  gasnete_coll_register_scatter_collectives(info, smallest_scratch);
  gasnete_coll_register_gather_collectives(info, smallest_scratch);
  gasnete_coll_register_gather_all_collectives(info, smallest_scratch);
  gasnete_coll_register_exchange_collectives(info, smallest_scratch);
}

static void gasnete_coll_deregister_collectives(gasnete_coll_autotune_info_t* info) {
  for (int i = 0; i < GASNETE_COLL_BROADCAST_NUM_ALGS; ++i) {
    gasnete_coll_autotune_deregister_algorithm(info->collective_algorithms[GASNET_COLL_BROADCAST_OP] + i);
  }
  gasneti_free(info->collective_algorithms[GASNET_COLL_BROADCAST_OP]);

  for (int i = 0; i < GASNETE_COLL_SCATTER_NUM_ALGS; ++i) {
    gasnete_coll_autotune_deregister_algorithm(info->collective_algorithms[GASNET_COLL_SCATTER_OP] + i);
  }
  gasneti_free(info->collective_algorithms[GASNET_COLL_SCATTER_OP]);

  for (int i = 0; i < GASNETE_COLL_GATHER_NUM_ALGS; ++i) {
    gasnete_coll_autotune_deregister_algorithm(info->collective_algorithms[GASNET_COLL_GATHER_OP] + i);
  }
  gasneti_free(info->collective_algorithms[GASNET_COLL_GATHER_OP]);

  for (int i = 0; i < GASNETE_COLL_GATHER_ALL_NUM_ALGS; ++i) {
    gasnete_coll_autotune_deregister_algorithm(info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP] + i);
  }
  gasneti_free(info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP]);

  for (int i = 0; i < GASNETE_COLL_EXCHANGE_NUM_ALGS; ++i) {
    gasnete_coll_autotune_deregister_algorithm(info->collective_algorithms[GASNET_COLL_EXCHANGE_OP] + i);
  }
  gasneti_free(info->collective_algorithms[GASNET_COLL_EXCHANGE_OP]);
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

#if GASNETE_COLL_CONDUIT_COLLECTIVES
static int allow_conduit_collectives=1;
#endif
static char* gasnete_coll_team_all_tuning_file;
gasnete_coll_autotune_info_t* gasnete_coll_autotune_init(gasnet_team_handle_t team
                                                         GASNETI_THREAD_FARG) {
  const size_t min_scratch_size = team->scratch_size;
  const gex_Rank_t mynode = team->myrank;
  const gasnet_image_t total_images = team->total_ranks;
  const gasnet_image_t my_images = 1;

  /* read all the environment variables and setup the defaults*/
  gasnete_coll_autotune_info_t* ret;
  char *default_tree_type;
  size_t dissem_limit;
  size_t temp_size;
  size_t dissem_limit_per_thread;
  int i;
  
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
      fprintf(stderr, "WARNING: Conflicting environment values for GASNET_COLL_GATHER_ALL_DISSEM_LIMIT (%"PRIuPTR") and GASNET_COLL_GATHER_ALL_DISSEM_LIMIT_PER_THREAD (%"PRIuPTR")\n", (uintptr_t) dissem_limit, (uintptr_t) dissem_limit_per_thread);
      fprintf(stderr, "WARNING: Using: %"PRIuPTR"\n", (uintptr_t) MIN(dissem_limit, temp_size));
    }
  }
  ret->gather_all_dissem_limit = MIN(dissem_limit, temp_size);
  
  dissem_limit_per_thread = gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_LIMIT_PER_THREAD", GASNETE_COLL_DEFAULT_DISSEM_LIMIT_PER_THREAD, 1);
  temp_size = gasnete_coll_nextpower2(dissem_limit_per_thread*my_images*my_images);
  dissem_limit = gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_LIMIT", temp_size, 1);
  if(temp_size != dissem_limit) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting environment values for GASNET_COLL_EXCHANGE_DISSEM_LIMIT (%"PRIuPTR") and GASNET_COLL_EXCHANGE_DISSEM_LIMIT_PER_THREAD (%"PRIuPTR")\n", (uintptr_t) dissem_limit, (uintptr_t) temp_size);
      fprintf(stderr, "WARNING: Using: %"PRIuPTR"\n", (uintptr_t) MIN(dissem_limit, temp_size));
    }
  }
  ret->exchange_dissem_limit = MIN(dissem_limit, temp_size);
  ret->exchange_dissem_radix = MIN(gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_RADIX", 2, 0),total_images);

  if(min_scratch_size < total_images) {
    gasneti_fatalerror("SCRATCH SPACE TOO SMALL Please set it to at least (%"PRIuPTR" bytes) through the GASNET_COLL_SCRATCH_SIZE environment variable", (uintptr_t) total_images);
  }
  ret->pipe_seg_size = gasneti_getenv_int_withdefault("GASNET_COLL_PIPE_SEG_SIZE", MIN(min_scratch_size, gex_AM_LUBRequestLong())/total_images, 1);
  /*  if(ret->pipe_seg_size == 0) {
   ret->pipe_seg_size = MIN(min_scratch_size, gex_AM_LUBRequestLong())/total_images;
   } 
   */
  if(ret->pipe_seg_size*total_images > min_scratch_size) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting evnironment values for scratch space allocated (%d bytes) and GASNET_COLL_PIPE_SEG_SIZE (%d bytes)\n", (int) min_scratch_size, (int)ret->pipe_seg_size);
      fprintf(stderr, "WARNING: Using %d bytes for GASNET_COLL_PIPE_SEG_SIZE\n", (int)(min_scratch_size/total_images));
    } 
    ret->pipe_seg_size = min_scratch_size/(total_images);
  } 
  
  if(ret->pipe_seg_size*total_images > gex_AM_LUBRequestLong()) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: GASNET_COLL_PIPE_SEG_SIZE (%d bytes) * total images (%d) has to be less than max size for an AMLong for this conduit (%"PRIuPTR")\n", 
              (int)ret->pipe_seg_size, (int)total_images, (uintptr_t) gex_AM_LUBRequestLong());
      fprintf(stderr, "WARNING: Using %"PRIuPTR" bytes for GASNET_COLL_PIPE_SEG_SIZE instead\n", (uintptr_t) gex_AM_LUBRequestLong()/total_images);
      ret->pipe_seg_size = gex_AM_LUBRequestLong()/total_images;
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
    gasnete_coll_print_coll_alg = gasneti_getenv_yesno_withdefault("GASNET_COLL_PRINT_COLL_ALG", 0);
  }
  
  ret->autotuner_defaults = NULL;
  ret->search_enabled = gasneti_getenv_yesno_withdefault("GASNET_COLL_ENABLE_SEARCH", 0);
  ret->profile_enabled = gasneti_getenv_yesno_withdefault("GASNET_COLL_ENABLE_PROFILE", 0);
  
  return ret;
}


void gasnete_coll_autotune_free(gasnete_coll_team_t team) {
  gasnete_coll_autotune_info_t *info = team->autotune_info;
  gasnete_coll_free_tree_type(info->bcast_tree_type);
  gasnete_coll_free_tree_type(info->scatter_tree_type);
  gasnete_coll_free_tree_type(info->gather_tree_type);
  gasnete_coll_deregister_collectives(info);
  gasneti_free(info);

  gasnete_coll_purge_dissemination(team);
}

gasnete_coll_tree_type_t gasnete_coll_autotune_get_bcast_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                                   gasnet_coll_optype_t op_type, 
                                                                   gex_Rank_t root, size_t nbytes, int flags) {
	gasnete_coll_tree_type_t ret = NULL;
	/*first check if we've seen this size*/
	/*find the log of the transfer size we are interested in*/
	uint32_t log2_nbytes;
	log2_nbytes = gasnete_coll_log2_sz(nbytes);
	
	if(autotune_info->bcast_tree_radix_limits[log2_nbytes] == -1) {
		/*perform search across fanouts*/
		/* do a barrier to ensure all threads have arrived*/
		gasnet_coll_barrier(autotune_info->team, 0, GASNET_BARRIERFLAG_UNNAMED);
	} else {
    /*for larger arrays just use the maximum setting that we've already found*/
		ret = gasnete_coll_make_tree_type_str((char*) "KNOMIAL_TREE,2");
	}
	
	return ret;
  
}

gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                             gasnet_coll_optype_t op_type, 
                                                             gex_Rank_t root, size_t nbytes, int flags) {
  switch(op_type) {
    case GASNET_COLL_BROADCAST_OP:
      return autotune_info->bcast_tree_type;  
      
    case GASNET_COLL_SCATTER_OP: 
      return autotune_info->scatter_tree_type;
      
    case GASNET_COLL_GATHER_OP:
      return autotune_info->gather_tree_type;
      
    default: gasneti_fatalerror("unknown tree based collective op type");
      return 0;
  }
}


size_t gasnete_coll_get_dissem_limit(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags) {
  switch(op_type) {
    case GASNET_COLL_GATHER_ALL_OP:
      return autotune_info->gather_all_dissem_limit;
    case GASNET_COLL_EXCHANGE_OP: 
      return autotune_info->exchange_dissem_limit;
    default:  gasneti_fatalerror("unknown dissem based collective op type");
      return 0;
  }
}

int gasnete_coll_get_dissem_radix(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags) {
  switch(op_type) {
    case GASNET_COLL_EXCHANGE_OP: 
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
      gasnete_coll_free_tree_type(team->autotune_info->bcast_tree_type);
      team->autotune_info->bcast_tree_type = gasnete_coll_make_tree_type(tree_class, &fanout,1); break;
    case GASNET_COLL_SCATTER_OP:
      gasnete_coll_free_tree_type(team->autotune_info->scatter_tree_type);
      team->autotune_info->scatter_tree_type = gasnete_coll_make_tree_type(tree_class, &fanout,1); break;
    case GASNET_COLL_GATHER_OP:
      gasnete_coll_free_tree_type(team->autotune_info->gather_tree_type);
      team->autotune_info->gather_tree_type = gasnete_coll_make_tree_type(tree_class, &fanout,1); break;
    default: gasneti_fatalerror("unknown tree based collective op");
  }
  return;
}

void gasnet_coll_set_dissem_limit(gasnete_coll_team_t team, size_t dissemlimit, gasnet_coll_optype_t optype) {
  switch(optype) {
    case GASNET_COLL_GATHER_ALL_OP:
      team->autotune_info->gather_all_dissem_limit = dissemlimit; break;
    case GASNET_COLL_EXCHANGE_OP:
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
    strcpy(buf, "broadcast SINGLE/");
    break;
  case GASNET_COLL_SCATTER_OP:
    strcpy(buf, "scatter SINGLE/");
    break;
  case GASNET_COLL_GATHER_OP:
    strcpy(buf, "gather SINGLE/");
    break;
  case GASNET_COLL_GATHER_ALL_OP:
    strcpy(buf, "gather_all SINGLE/");
    break;
  case GASNET_COLL_EXCHANGE_OP:
    strcpy(buf, "exchange SINGLE/");
    break;
    
  default:
    strcpy(buf, "FILLIN");
    
  }

  strncat(buf, "LOCAL", 100);
  return buf;
}

static char* print_flag_str(char *outstr, int flags) {
  
  if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    strcpy(outstr, "no/no");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    strcpy(outstr, "no/my");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    strcpy(outstr, "no/all");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    strcpy(outstr, "my/no");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    strcpy(outstr, "my/my");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    strcpy(outstr, "my/all");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    strcpy(outstr, "all/no");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    strcpy(outstr, "all/my");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    strcpy(outstr, "all/all");
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
  return (gasnete_coll_syncmode_t)(-1); /* NOT REACHED */
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

static char* syncmode_to_str(char *buffer, int mode) {
  switch ((gasnete_coll_syncmode_t)mode) {
    case GASNETE_COLL_NONO:
      strcpy(buffer, "no/no");
      break;
    case GASNETE_COLL_NOMY:
      strcpy(buffer, "no/my");
      break;
    case GASNETE_COLL_NOALL:
      strcpy(buffer, "no/all");
      break;
    case GASNETE_COLL_MYNO:
      strcpy(buffer, "my/no");
      break;
    case GASNETE_COLL_MYMY:
      strcpy(buffer, "my/my");
      break;
    case GASNETE_COLL_MYALL:
      strcpy(buffer, "my/all");
      break;
    case GASNETE_COLL_ALLNO:
      strcpy(buffer, "all/no");
      break;
    case GASNETE_COLL_ALLMY:
      strcpy(buffer, "all/my");
      break;
    case GASNETE_COLL_ALLALL:
      strcpy(buffer, "all/all");
      break;
    default:
      gasneti_fatalerror("unknown syncmode");
      break;
  }
  return buffer;
}

static gasnete_coll_addr_mode_t get_addrmode_from_str(char *str) { 
  if(STRINGS_MATCH(str, "local"))
    return GASNETE_COLL_LOCAL_MODE;
  return (gasnete_coll_addr_mode_t)(-1); /* NOT REACHED */
}

static char * addrmode_to_str(char *buffer, int mode) {
  switch((gasnete_coll_addr_mode_t)mode){
    case GASNETE_COLL_LOCAL_MODE:
      strcpy(buffer, "local");
      break;
    default:
      gasneti_fatalerror("unknown address mode");
  }
  return buffer;
} 

GASNETI_INLINE(get_addrmode_from_flags)
gasnete_coll_addr_mode_t get_addrmode_from_flags(int flags) { 
  return GASNETE_COLL_LOCAL_MODE;
}

static gasnet_coll_optype_t get_optype_from_str(char *str) { 
 
  if(STRINGS_MATCH(str, "broadcast")) 
    return GASNET_COLL_BROADCAST_OP;
  
  else if(STRINGS_MATCH(str, "scatter")) 
    return GASNET_COLL_SCATTER_OP;
  
  else if(STRINGS_MATCH(str, "gather")) 
    return GASNET_COLL_GATHER_OP;
  
  else if(STRINGS_MATCH(str, "gather_all")) 
    return GASNET_COLL_GATHER_ALL_OP;
  
  else if(STRINGS_MATCH(str, "exchange")) 
    return GASNET_COLL_EXCHANGE_OP;
  
  else gasneti_fatalerror("op %s not yet supported\n", str);
  return (gasnet_coll_optype_t)(-1); /* NOT REACHED */
}

static char * optype_to_str(char *buffer, int op) {
  switch ((gasnet_coll_optype_t)op) {
    case GASNET_COLL_BROADCAST_OP:
      strcpy(buffer, "broadcast");
      break;
    case GASNET_COLL_SCATTER_OP:
      strcpy(buffer, "scatter");
      break;
    case GASNET_COLL_GATHER_OP:
      strcpy(buffer, "gather");
      break;
    case GASNET_COLL_GATHER_ALL_OP:
      strcpy(buffer, "gather_all");
      break;
    case GASNET_COLL_EXCHANGE_OP:
      strcpy(buffer, "gather_all");
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
      temp->start = optype = get_optype_from_str(MYXML_ATTRIBUTES(child_node)[0].attribute_value);
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
  gasnete_coll_autotune_index_entry_t *root;
  const char *tree_levels[8] = {"machine", "num_nodes", "threads_per_node", "sync_mode", "address_mode", "collective", "root", "size"};
  
  /*the root of the tree contains the GASNET config string*/
  /*throw a warning if the tree does not match the current tree*/
  if(STRINGS_MATCH(MYXML_TAG(tuning_data), "machine")) {
    if(!STRINGS_MATCH(MYXML_ATTRIBUTES(tuning_data)[0].attribute_value, GASNET_CONFIG_STRING)) {
      printf("warning! tuning data's config string: %s does not match current gasnet config string: %s\n", MYXML_ATTRIBUTES(tuning_data)[0].attribute_value, GASNET_CONFIG_STRING);
    } 
    root= load_autotuner_defaults_helper(autotune_info, tuning_data, tree_levels, 1, 8, (gasnet_coll_optype_t)(-1));
  } else {
    root = NULL; /* warning suppression */
    gasneti_fatalerror("exepected machine as the root of the tree");
  }
  return root;
}

/****************************/
/****** RUN THE AUTOTUNER ***/
/****************************/

/*run the given op on the given arguments*/
/*and return the best one*/
int gasnete_coll_autotune_barrier(gasnete_coll_team_t team) {
  int ret = gasnet_coll_barrier(team, 0, GASNET_BARRIERFLAG_UNNAMED);
  gasneti_assert_always(ret == GASNET_OK);
  return ret;
}

#define PTHREAD_BARRIER(team, local_pthread_count)  \
  gasnete_coll_autotune_barrier(team)


static gasnett_tick_t run_collective_bench(gasnet_team_handle_t team, gasnet_coll_optype_t op,
                                           gasnet_coll_args_t coll_args, int flags,
                                           gasnete_coll_implementation_t impl, gasnet_coll_overlap_sample_work_t fnptr, void *sample_work_arg GASNETI_THREAD_FARG) {
  int iter, niters;
  gasnett_tick_t start, total;
  gex_Event_t handle;

  niters = team->autotune_info->perf_iters;
  if (coll_args.nbytes <= 1024)
    niters *= 4;
  else if (coll_args.nbytes <= 16384)
    niters *= 2;

  gasneti_assert(coll_args.rootimg < team->total_ranks);
  PTHREAD_BARRIER(team, 1);

  for(iter=0; iter<team->autotune_info->warm_iters; iter++) {
    switch(op){
      case GASNET_COLL_BROADCAST_OP:
        handle = (*((gasnete_coll_bcast_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, 
                                                                   coll_args.src[0], coll_args.nbytes, flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_SCATTER_OP:
        handle = (*((gasnete_coll_scatter_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, 
                                                                   coll_args.src[0], coll_args.nbytes, coll_args.dist, flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_OP:
        handle = (*((gasnete_coll_gather_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.rootimg, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                    coll_args.dist, flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_ALL_OP:
        handle = (*((gasnete_coll_gather_all_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                        flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_EXCHANGE_OP:
        handle = (*((gasnete_coll_exchange_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                      flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
        
      default:
        gasneti_fatalerror("collective not yet implemented");  
    }    
  }
  
  PTHREAD_BARRIER(team, 1);
  
  start = gasnett_ticks_now();
  for(iter=0; iter<niters; iter++) {
    switch(op){
      case GASNET_COLL_BROADCAST_OP:
        handle = (*((gasnete_coll_bcast_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, coll_args.src[0], 
                                                                   coll_args.nbytes, flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_SCATTER_OP:
        handle = (*((gasnete_coll_scatter_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.rootimg, 
                                                                     coll_args.src[0], coll_args.nbytes, coll_args.dist, flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_OP:
        handle = (*((gasnete_coll_gather_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.rootimg, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                    coll_args.dist, flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_GATHER_ALL_OP:
        handle = (*((gasnete_coll_gather_all_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                        flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      case GASNET_COLL_EXCHANGE_OP:
        handle = (*((gasnete_coll_exchange_fn_ptr_t) (impl->fn_ptr)))(team, coll_args.dst[0], coll_args.src[0], coll_args.nbytes,
                                                                      flags, impl, 0 GASNETI_THREAD_PASS);
        if(fnptr) (*fnptr)(sample_work_arg);
        gasnete_wait(handle GASNETI_THREAD_PASS);
        break;
      default:
        gasneti_fatalerror("collective not yet implemented");  
    }    
  }

  PTHREAD_BARRIER(team, 1);
  
  total = gasnett_ticks_now()-start;

  /* gasnete_coll_barrier(team, 0, GASNET_BARRIERFLAG_UNNAMED GASNETI_THREAD_PASS); */

  return total;

}


static void do_tuning_loop(gasnet_team_handle_t team, gasnet_coll_optype_t op,
                           gasnet_coll_args_t coll_args, int flags,
                           gasnet_coll_overlap_sample_work_t fnptr, void *sample_work_arg,
                           int alg_idx, gasnett_tick_t *best_time,  uint32_t *best_param_list, char *best_tree, int current_param_number, uint32_t *curr_idx_in  GASNETI_THREAD_FARG) {
  int idx;

  /*no tuning parameters*/
  if(gasnet_coll_get_num_params(team, op, alg_idx)==0) {
    gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();

    impl->team = team;
    impl->optype = op;
    impl->flags = flags;
    impl->fn_ptr = team->autotune_info->collective_algorithms[op][alg_idx].fn_ptr.generic_coll_fn_ptr;
    impl->fn_idx = alg_idx;
    *best_time = run_collective_bench(team, op, coll_args, flags, impl, fnptr, sample_work_arg GASNETI_THREAD_PASS);
    if(team->myrank==0 && gasnete_coll_print_autotuner_timers) {
      int i;
      char buf1[100];
      char buf2[100];
      
      printf("%d> %s alg: %s (%d) syncflags: %s nbytes: %d params:<", (int)team->myrank, print_op_str(buf1, op, flags), team->autotune_info->collective_algorithms[op][alg_idx].name_str, alg_idx,
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
          impl->team = team;
          impl->optype = op;
          impl->flags = flags;
          impl->fn_ptr = team->autotune_info->collective_algorithms[op][alg_idx].fn_ptr.generic_coll_fn_ptr;
          impl->fn_idx = alg_idx;
          impl->num_params = team->autotune_info->collective_algorithms[op][alg_idx].num_parameters;
          GASNETE_FAST_UNALIGNED_MEMCPY(impl->param_list, curr_idx, impl->num_params*sizeof(uint32_t));
          if(team->autotune_info->collective_algorithms[op][alg_idx].parameter_list[current_param_number].flags & GASNET_COLL_TUNING_TREE_SHAPE)
            impl->tree_type = gasnete_coll_autotune_get_tree_type_idx(team, idx);
          
          
          /*run the measurement iterations*/
          curr_run = run_collective_bench(team, op,coll_args, flags, impl, fnptr, sample_work_arg GASNETI_THREAD_PASS);

          if(team->myrank==0 && gasnete_coll_print_autotuner_timers) {
            char buf1[100];
            char buf2[100];
            int i;
            printf("%d> %s alg: %s (%d) syncflags: %s nbytes: %d params:<", (int)team->myrank, print_op_str(buf1, op, flags), 
                   team->autotune_info->collective_algorithms[op][alg_idx].name_str, alg_idx,
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
          do_tuning_loop(team, op, coll_args, flags, fnptr, sample_work_arg, alg_idx, best_time, best_param_list, best_tree, current_param_number+1, curr_idx GASNETI_THREAD_PASS);
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
                                  uint32_t *best_algidx, uint32_t *num_params, uint32_t **best_param, char **best_tree GASNETI_THREAD_FARG)  {
  int algidx = 0;
  int num_algs;
  gasnett_tick_t curr_best_time=GASNETT_TICK_MAX, alg_best_time=GASNETT_TICK_MAX;
  uint32_t loc_best_param_list[GASNET_COLL_NUM_PARAM_TYPES];
  uint32_t sync_flags = (flags &  GASNET_COLL_SYNC_FLAG_MASK); /*strip the sync flags off the flags*/
  uint32_t req_flags = (flags & (~GASNET_COLL_SYNC_FLAG_MASK));
  char *loc_best_tree;

  loc_best_tree = gasneti_calloc(1,sizeof(char)*100);
  

  switch (op) {
    case GASNET_COLL_BROADCAST_OP:
      num_algs = GASNETE_COLL_BROADCAST_NUM_ALGS;
      break;
    case GASNET_COLL_SCATTER_OP:
      num_algs = GASNETE_COLL_SCATTER_NUM_ALGS;
      break;
    case GASNET_COLL_GATHER_OP:
      num_algs = GASNETE_COLL_GATHER_NUM_ALGS;
      break;
    case GASNET_COLL_GATHER_ALL_OP:
      num_algs = GASNETE_COLL_GATHER_ALL_NUM_ALGS;
      break;
    case GASNET_COLL_EXCHANGE_OP:
      num_algs = GASNETE_COLL_EXCHANGE_NUM_ALGS;
      break;
    default:
      num_algs = 0; /* warning suppression */
      gasneti_fatalerror("not yet supported");
      break;
  }
  
  *best_algidx = -1;
  PTHREAD_BARRIER(team, 1);
  for (algidx=0; algidx<num_algs; algidx++) {
    
    int size_ok = (coll_args.nbytes <= team->autotune_info->collective_algorithms[op][algidx].max_num_bytes && 
		   coll_args.nbytes >= team->autotune_info->collective_algorithms[op][algidx].min_num_bytes);
    /*ensure that all the flags required by the algorithm are passed in through the flags*/
    int req_flags_ok = ((req_flags & team->autotune_info->collective_algorithms[op][algidx].requirements) == team->autotune_info->collective_algorithms[op][algidx].requirements);
    /*ensure that the synchronization flags exist in the list of possible synch flags for this algorithm*/
    int sync_flags_ok = ((sync_flags & team->autotune_info->collective_algorithms[op][algidx].syncflags) == sync_flags);
    int nreq_flags_ok = (!(req_flags & team->autotune_info->collective_algorithms[op][algidx].n_requirements));
#if GASNET_DEBUG
    if(!size_ok){if(team->myrank==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: size too large)\n", (int)gasneti_mynode, algidx);continue;}
    if(!req_flags_ok){if(team->myrank==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: all req flags are not present)\n", (int)gasneti_mynode, algidx);continue;}
    if(!nreq_flags_ok){if(team->myrank==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: one of the nreq flags is present)\n", (int)gasneti_mynode, algidx);continue;}
    if(!sync_flags_ok){if(team->myrank==0 && gasnete_coll_print_autotuner_timers) fprintf(stderr, "%d> skipping alg: %d (reason: not valid for this syncflag)\n", (int)gasneti_mynode, algidx);continue;}
    
#else
    if(!(size_ok && req_flags_ok && sync_flags_ok &&  nreq_flags_ok/*match!*/)) {
      continue;
    }
#endif
    
    PTHREAD_BARRIER(team, 1);
     if(op == GASNET_COLL_BROADCAST_OP   && algidx == GASNETE_COLL_BROADCAST_SCATTERALLGATHER) {
       if (*best_algidx == -1) {
         // no alg has been timed yet
         // assume this one is "best" in case there are no other valid ones (bug 3731)
         *best_algidx = algidx;
       }
       continue; // TODO: unclear to PHH why these algorithms are never timed
     }
	
    alg_best_time = curr_best_time;

    do_tuning_loop(team, op, coll_args, flags, fnptr, sample_work_arg, 
                   algidx, &alg_best_time, loc_best_param_list, loc_best_tree, 0, NULL GASNETI_THREAD_PASS);
    
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
  gasneti_assert(*best_algidx != -1);
  /*take the best time that we've seen so far and then copy out the number of parameters to it*/
  /*the tuning loop will set the loc_best_param_list with the appropriate parameters so we just have to copy it out and return it*/
  *num_params = gasnet_coll_get_num_params(team, op, *best_algidx);
  *best_param = gasneti_malloc(sizeof(uint32_t)*gasnet_coll_get_num_params(team, op, *best_algidx));
  GASNETI_MEMCPY_SAFE_EMPTY(*best_param, loc_best_param_list, sizeof(uint32_t)*(*num_params));
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
  
  temp = search_intervals(temp->subtree, 1,exact_match);
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
  temp->subtree = add_interval(temp->subtree, 1, "threads_per_node");
  temp = search_intervals(temp->subtree, 1, 1);
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
void gasnete_coll_safe_broadcast(gasnete_coll_team_t team, void *dst, void *src, gasnet_image_t root, size_t nbytes, int nodes_only GASNETI_THREAD_FARG) {
  int flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC | GASNET_COLL_LOCAL |  GASNET_COLL_DISABLE_AUTOTUNE;

  if(nodes_only) 
    flags = flags | GASNET_COLL_NO_IMAGES;

  /* turn off algorithm tracing for the following broadcast because it's internal */
  int tmp = gasnete_coll_print_coll_alg;
  gasnete_coll_print_coll_alg = 0;
  
  gasnet_coll_broadcast(team, dst, root, src, nbytes, flags);
  
  gasnete_coll_print_coll_alg = tmp;
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

static gasnete_coll_implementation_t autotune_op(gasnet_team_handle_t team, gasnet_coll_optype_t op, gasnet_coll_args_t args, int flags GASNETI_THREAD_FARG) {
  gasnete_coll_implementation_t ret;
  
  /* explicit request to not tune */
  if(flags & GASNET_COLL_DISABLE_AUTOTUNE) return NULL;

  if (gasnete_coll_print_coll_alg && team->myrank == 0) {
    char buf1[256], buf2[256];
    print_op_str(buf1, op, flags);
    print_flag_str(buf2, flags);
    fprintf(stderr, "Autotuning %s: flags %s, nbytes %"PRIuPTR", root %u\n", buf1, buf2, (uintptr_t)args.nbytes, (unsigned int)args.rootimg);
  }

  /*if a tuning file has been specified for TEAM ALL and hasn't been yet loaded load it now*/
  if(team == GASNET_TEAM_ALL && gasnete_coll_team_all_tuning_file && !team->autotune_info->autotuner_defaults) {
    gasnete_coll_loadTuningState(gasnete_coll_team_all_tuning_file, team GASNETI_THREAD_PASS);
  }
  
  if(team->autotune_info->profile_enabled) {
    gasnete_coll_autotune_index_entry_t *idx;
    idx = add_to_index(op, team, flags, args.nbytes, args.rootimg, 1);
    idx->num_calls++;
  }
  
  if(team->autotune_info->autotuner_defaults  || team->autotune_info->search_enabled) {
    ret = search_index(op, team, flags, args.nbytes, args.rootimg, team->autotune_info->search_enabled);  
    /*make sure the returned algortithm can handle the cases*/
    if(verify_algorithm(team, op, flags, args.nbytes, ret)) {
      if (ret->team == NULL) {
        ret->team = team;
        ret->optype = op;
        ret->flags = flags;
      }

      if (gasnete_coll_print_coll_alg && team->myrank == 0) {
        /* print a search hit here */
        fprintf(stderr, "The algorithm for the collective is in the existing autotuning database.\n");
        gasnete_coll_implementation_print(ret, stderr);
      }
      return ret;
    }    
  }
  /*we searched the index w/ no luck so need to run a tuning run (if enabled)*/
  if(team->autotune_info->search_enabled && team==GASNET_TEAM_ALL) {
    gasnete_coll_implementation_t temp = gasnete_coll_get_implementation();
    gasnete_coll_autotune_index_entry_t *idx;
    gasnet_image_t myrank;
    uint32_t best_algidx;
    uint32_t num_params;
    uint32_t *param_list;
#if 0
    char flagstr[15];
#endif

    char best_tree[GASNETE_COLL_MAX_TREE_TYPE_STRLEN];
    char all_best_tree[GASNETE_COLL_MAX_TREE_TYPE_STRLEN];
    char *temp_tree_str;
    
    
    ret = gasnete_coll_get_implementation();
    myrank = team->myrank;
#if 0
    if(myrank == srcimage) {
      fprintf(stderr, "%d> starting autotune for %s %d byte broadcast\n", myrank, print_flag_str(flagstr, flags), (int)nbytes);
    }
#endif
    gasnete_coll_tune_generic_op(team, op, 
                                 args, flags, 
                                 NULL, NULL,
                                 /*returned by the algorithm*/
                                 &best_algidx, &num_params, &param_list, &temp_tree_str GASNETI_THREAD_PASS);
    /*until we have a solution for teams with srcimages*/
    if(myrank == args.rootimg) {
#if 0
      fprintf(stderr, "%d> finishing autotune for %s %d byte broadcast\n", myrank, print_flag_str(flagstr, flags), (int)nbytes);      
#endif
      temp->optype = op;
      temp->flags = flags;
      temp->fn_idx = best_algidx;
      temp->num_params = num_params;
      GASNETI_MEMCPY_SAFE_EMPTY(temp->param_list, param_list, sizeof(uint32_t)*num_params);
      if(strlen(temp_tree_str) > 0) {
        gasneti_assert(strlen(temp_tree_str)<(GASNETE_COLL_MAX_TREE_TYPE_STRLEN-1));
        strcpy(best_tree, temp_tree_str);
        gasneti_free(temp_tree_str);
      } else {
        memset(best_tree, 0, sizeof(char)*GASNETE_COLL_MAX_TREE_TYPE_STRLEN);
      }
    }
    /*have the root tell all other nodes in this team what the correct implementation is*/
    gasnete_coll_safe_broadcast(team, ret, temp, args.rootimg, sizeof(struct gasnete_coll_implementation_t_), 0 GASNETI_THREAD_PASS);
    memset(all_best_tree, 0, sizeof(char)*GASNETE_COLL_MAX_TREE_TYPE_STRLEN);
    gasnete_coll_safe_broadcast(team, all_best_tree, best_tree, args.rootimg, GASNETE_COLL_MAX_TREE_TYPE_STRLEN*sizeof(char), 0 GASNETI_THREAD_PASS);
    ret->fn_ptr = team->autotune_info->collective_algorithms[op][ret->fn_idx].fn_ptr.generic_coll_fn_ptr;
    ret->team = team;

    if(strlen(all_best_tree) > 0) {
      ret->tree_type = gasnete_coll_make_tree_type_str(all_best_tree);
    }
    gasnete_coll_free_implementation(temp);
    gasneti_assert(ret->team == team);
      
    /*insert ret into the search index*/
    PTHREAD_BARRIER(team, 1); 
    {
      idx = add_to_index(op, team, flags, args.nbytes, args.rootimg, 0);
      idx->impl = ret;
    }
    
    PTHREAD_BARRIER(team, 1); 
    
    if (gasnete_coll_print_coll_alg && team->myrank == 0) {
      fprintf(stderr, "Finish autotuning. The best algorithm searched is:\n");
      gasnete_coll_implementation_print(ret, stderr);
    }

    return ret;
  } else {

    if (gasnete_coll_print_coll_alg && team->myrank == 0) {
      fprintf(stderr, "Search is disabled or the team is not GASNET_TEAM_ALL.\n");
    }

    return NULL;
  }
}

gasnete_coll_implementation_t gasnete_coll_autotune_get_bcast_algorithm(gasnet_team_handle_t team, void *dst, gasnet_image_t srcimage, void *src, size_t nbytes, uint32_t flags  GASNETI_THREAD_FARG) {
  
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_min, gex_AM_LUBRequestMedium());
  gasnete_coll_implementation_t ret;

  /*first try to search our gasnet autotuner index to see if we have anything for it*/
  /*if not then fall back to our orignal implementation*/

  {
    gasnet_coll_args_t args = GASNET_COLL_ARGS_INITIALIZER;
    
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = srcimage;
    args.nbytes = nbytes;
    ret = autotune_op(team, GASNET_COLL_BROADCAST_OP, args, flags GASNETI_THREAD_PASS);
    if(ret) return ret;
  }
  
  /*autotuning is turned off or we need to fall back to the default*/
  ret = gasnete_coll_get_implementation();
  ret->need_to_free = 1;
  ret->team = team;
  ret->flags = flags;
  ret->optype = GASNET_COLL_BROADCAST_OP;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_BROADCAST_OP, 
                                                       -1,nbytes, flags);
  
  /*for now encode the original decision tree*/
  if (nbytes <= eager_limit) {
    /* Small enough for Eager, which will eliminate any barriers for *_MYSYNC and
     * the need for passing addresses for _LOCAL
     * Eager is totally AM-based and thus safe regardless of *_IN_SEGMENT
     */
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_EAGER].fn_ptr.bcast_fn; 
    ret->fn_idx = GASNETE_COLL_BROADCAST_TREE_EAGER;
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT ) {
    /* run the segmented broadcast code 
     function internally checks synch flags and SINGLE/LOCAL flags
     */
    /*this should also be part of the space*/
    if((nbytes > team->total_ranks) && !(flags & GASNETE_COLL_SUBORDINATE) && 0) {       
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_SCATTERALLGATHER].fn_ptr.bcast_fn;
      ret->fn_idx = GASNETE_COLL_BROADCAST_SCATTERALLGATHER;
    } else if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_BROADCAST_OP, flags)) {
        ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SCRATCH].fn_ptr.bcast_fn;
        ret->fn_idx = GASNETE_COLL_BROADCAST_TREE_PUT_SCRATCH;
    } else if(nbytes<=team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG].max_num_bytes) {
      ret->num_params = 1;
      ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_BROADCAST_OP, flags);  
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_PUT_SEG].fn_ptr.bcast_fn;
      ret->fn_idx = GASNETE_COLL_BROADCAST_TREE_PUT_SEG;
    } else if(flags & GASNET_COLL_SRC_IN_SEGMENT) {
      ret->num_params = 0;
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_TREE_RVGET].fn_ptr.bcast_fn;
      ret->fn_idx = GASNETE_COLL_BROADCAST_TREE_RVGET;
    } else {
      ret->num_params = 0;
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVOUS].fn_ptr.bcast_fn;
      ret->fn_idx = GASNETE_COLL_BROADCAST_RVOUS;
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
      /* We can use Rendezvous+Get to eliminate any barriers for *_MYSYNC.
       * The Rendezvous is needed for _LOCAL.
       */
      ret->num_params = 0;
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVGET].fn_ptr.bcast_fn;
      ret->fn_idx = GASNETE_COLL_BROADCAST_RVGET;
  }  else {
    /* If we reach here then neither src nor dst is in-segment */
    ret->num_params = 0;
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_BROADCAST_OP][GASNETE_COLL_BROADCAST_RVOUS].fn_ptr.bcast_fn;
    ret->fn_idx = GASNETE_COLL_BROADCAST_RVOUS;
  }
  
  if (gasnete_coll_print_coll_alg && team->myrank == 0) {
    fprintf(stderr, "The algorithm for broadcast is selected by the default logic.\n");
    gasnete_coll_implementation_print(ret, stderr);
  }

  return ret;
}


gasnete_coll_implementation_t 
gasnete_coll_autotune_get_scatter_algorithm(gasnet_team_handle_t team, void *dst, gasnet_image_t srcimage, 
                                            void *src, size_t nbytes, size_t dist, uint32_t flags  GASNETI_THREAD_FARG) {
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale, gex_AM_LUBRequestMedium()/team->total_ranks);
  gasnete_coll_implementation_t ret;

  {
    gasnet_coll_args_t args = GASNET_COLL_ARGS_INITIALIZER;
    
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = srcimage;
    args.nbytes = nbytes;
    args.dist = dist;
    ret = autotune_op(team, GASNET_COLL_SCATTER_OP, args, flags GASNETI_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(team, flags, 0, 0, dst, nbytes,
                                     1, srcimage, src, nbytes*gasneti_nodes);
  ret->team = team;
  ret->optype = GASNET_COLL_SCATTER_OP;
  ret->flags = flags;
  ret->need_to_free = 1;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                        GASNET_COLL_SCATTER_OP, 
                                                        srcimage, nbytes, flags);
  /* Choose algorithm based on arguments */
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if (nbytes <= eager_limit) {
      /* Small enough for Eager, which works for out-of-segment src and/or dst */
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_EAGER].fn_ptr.scatter_fn;
      ret->fn_idx = GASNETE_COLL_SCATTER_TREE_EAGER;
    } else if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_SCATTER_OP, flags)) {
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_NO_COPY].fn_ptr.scatter_fn;
      ret->fn_idx = GASNETE_COLL_SCATTER_TREE_PUT_NO_COPY;
    } else if(nbytes <= team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_SEG].max_num_bytes) {
      ret->num_params = 1;
      ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_SCATTER_OP, flags);
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_TREE_PUT_SEG].fn_ptr.scatter_fn;
      ret->fn_idx = GASNETE_COLL_SCATTER_TREE_PUT_SEG;
    } else {
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS].fn_ptr.scatter_fn;
      ret->fn_idx = GASNETE_COLL_SCATTER_RVOUS;
    }
  } else if (nbytes <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_EAGER].fn_ptr.scatter_fn;
    ret->fn_idx = GASNETE_COLL_SCATTER_EAGER;
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVGET].fn_ptr.scatter_fn;  
      ret->fn_idx = GASNETE_COLL_SCATTER_RVGET;
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
      /* XXX: could do better since DST is in-segment */
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS].fn_ptr.scatter_fn;
      ret->fn_idx = GASNETE_COLL_SCATTER_RVOUS;
  } else {
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_SCATTER_OP][GASNETE_COLL_SCATTER_RVOUS].fn_ptr.scatter_fn;
    ret->fn_idx = GASNETE_COLL_SCATTER_RVOUS;
  }

  if (gasnete_coll_print_coll_alg && team->myrank == 0) {
    fprintf(stderr, "The algorithm for scatter is selected by the default logic.\n");
    gasnete_coll_implementation_print(ret, stderr);
  }

  return ret;
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_algorithm(gasnet_team_handle_t team,gasnet_image_t dstimage, void *dst, void *src, 
                                           size_t nbytes, size_t dist, uint32_t flags  GASNETI_THREAD_FARG) {
  const size_t eager_limit = MIN(gasnete_coll_p2p_eager_scale, gex_AM_LUBRequestMedium()/team->total_ranks);
  gasnete_coll_implementation_t ret;

  {
    gasnet_coll_args_t args = GASNET_COLL_ARGS_INITIALIZER;
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = dstimage;
    args.nbytes = nbytes;
    args.dist = dist;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_GATHER_OP, args, flags GASNETI_THREAD_PASS);
    if(ret) return ret;
  }
  
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  ret->team = team;
  ret->flags = flags;
  ret->optype = GASNET_COLL_GATHER_OP;
  ret->tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info, 
                                                       GASNET_COLL_GATHER_OP, 
                                                       dstimage, nbytes, flags);
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    if (nbytes <= eager_limit) {
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
      ret->fn_idx = GASNETE_COLL_GATHER_TREE_EAGER;
    } else if(!(flags & GASNETE_COLL_NONROOT_SUBORDINATE)) {
      if(nbytes <= gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_GATHER_OP, flags)) {
        ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_NO_COPY].fn_ptr.gather_fn;
        ret->fn_idx = GASNETE_COLL_GATHER_TREE_PUT_NO_COPY;
      } else if(nbytes <= team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_SEG].max_num_bytes) {
        ret->num_params = 1;
        ret->param_list[0] = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNET_COLL_GATHER_OP, flags);
        ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_PUT_SEG].fn_ptr.gather_fn;
        ret->fn_idx = GASNETE_COLL_GATHER_TREE_PUT_SEG;
      } else {
        ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS].fn_ptr.gather_fn;
        ret->fn_idx = GASNETE_COLL_GATHER_RVOUS;
      }
    } else {
      if (nbytes <= eager_limit) {
        ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
        ret->fn_idx = GASNETE_COLL_GATHER_TREE_EAGER;
      } else {
        ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVPUT].fn_ptr.gather_fn;
        ret->fn_idx = GASNETE_COLL_GATHER_RVPUT;
      }
    }
  } else if (nbytes <=eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_TREE_EAGER].fn_ptr.gather_fn;
    ret->fn_idx = GASNETE_COLL_GATHER_TREE_EAGER;
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVPUT].fn_ptr.gather_fn;
      ret->fn_idx = GASNETE_COLL_GATHER_RVPUT;
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
      /* XXX: could do better since src is in-segment */
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS].fn_ptr.gather_fn;
      ret->fn_idx = GASNETE_COLL_GATHER_RVOUS;
  } else {
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_OP][GASNETE_COLL_GATHER_RVOUS].fn_ptr.gather_fn;
    ret->fn_idx = GASNETE_COLL_GATHER_RVOUS;
  }

  if (gasnete_coll_print_coll_alg && team->myrank == 0) {
    fprintf(stderr, "The algorithm for gather is selected by the default logic.\n");
    gasnete_coll_implementation_print(ret, stderr);
  }

  return ret;
  
  
}

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_all_algorithm(gasnet_team_handle_t team, void *dst, void *src, 
                                               size_t nbytes, uint32_t flags  GASNETI_THREAD_FARG) {
  
  size_t max_dissem_msg_size = team->total_ranks*nbytes;
  gasnete_coll_implementation_t ret;

  {
    gasnet_coll_args_t args = GASNET_COLL_ARGS_INITIALIZER;
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = 0;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_GATHER_ALL_OP, args, flags GASNETI_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  ret->team = team;
  ret->flags = flags;
  ret->optype = GASNET_COLL_GATHER_ALL_OP;

  if(nbytes <=  gasnete_coll_get_dissem_limit(team->autotune_info, GASNET_COLL_GATHER_ALL_OP, flags) &&
     max_dissem_msg_size <= MIN(team->scratch_size, gex_AM_LUBRequestLong())) {
    ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_DISSEM].fn_ptr.gather_all_fn;
    ret->fn_idx = GASNETE_COLL_GATHER_ALL_DISSEM;
  } else {
      ret->fn_ptr = team->autotune_info->collective_algorithms[GASNET_COLL_GATHER_ALL_OP][GASNETE_COLL_GATHER_ALL_GATH].fn_ptr.gather_all_fn;
      ret->fn_idx = GASNETE_COLL_GATHER_ALL_GATH;
  }
 
  if (gasnete_coll_print_coll_alg && team->myrank == 0) {
    fprintf(stderr, "The algorithm for gather_all is selected by the default logic.\n");
    gasnete_coll_implementation_print(ret, stderr);
  }

  return ret;
}


gasnete_coll_implementation_t 
gasnete_coll_autotune_get_exchange_algorithm(gasnet_team_handle_t team, void *dst, void *src, 
                                             size_t nbytes, uint32_t flags  GASNETI_THREAD_FARG) {
  gasnete_coll_implementation_t ret;
  size_t max_dissem_msg_size = nbytes*(team->total_ranks/2+(team->total_ranks%2));

  {
    gasnet_coll_args_t args = GASNET_COLL_ARGS_INITIALIZER;
    args.dst = (uint8_t**)&dst;
    args.src = (uint8_t**)&src;
    args.rootimg = 0;
    args.nbytes = nbytes;
    
    /*first try to search our gasnet autotuner index to see if we have anything for it*/
    ret = autotune_op(team, GASNET_COLL_EXCHANGE_OP, args, flags GASNETI_THREAD_PASS);
    if(ret) return ret;
  } 
  ret = gasnete_coll_get_implementation();
  ret->need_to_free =1;
  ret->team = team;
  ret->flags = flags;
  ret->optype = GASNET_COLL_EXCHANGE_OP;

  if (nbytes <=  gasnete_coll_get_dissem_limit(team->autotune_info, GASNET_COLL_EXCHANGE_OP, flags) &&
      nbytes*team->total_ranks+(max_dissem_msg_size*2)<= team->scratch_size  &&
      max_dissem_msg_size <=  gex_AM_LUBRequestLong()) {
    ret->fn_ptr =  team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_DISSEM2].fn_ptr.exchange_fn;
    ret->fn_idx = GASNETE_COLL_EXCHANGE_DISSEM2;
  } else if(flags & GASNET_COLL_DST_IN_SEGMENT){
      ret->fn_ptr =  team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_RVPUT].fn_ptr.exchange_fn;
      ret->fn_idx = GASNETE_COLL_EXCHANGE_RVPUT;
  } else {
    ret->fn_ptr =  team->autotune_info->collective_algorithms[GASNET_COLL_EXCHANGE_OP][GASNETE_COLL_EXCHANGE_GATH].fn_ptr.exchange_fn;
    ret->fn_idx = GASNETE_COLL_EXCHANGE_GATH;
  }

  if (gasnete_coll_print_coll_alg && team->myrank == 0) {
    fprintf(stderr, "The algorithm for exchange is selected by the default logic.\n");
    gasnete_coll_implementation_print(ret, stderr);
  }

  return ret;
  
}

static void dump_tuning_state_helper(myxml_node_t *parent, gasnete_coll_autotune_index_entry_t *tuning_root) {
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
      char tempbuffer[512];
      int c;
      gasnete_coll_implementation_t impl = temp->impl;
      
      gasneti_assert(impl);
      gasnete_coll_tree_type_to_str(buffer, impl->tree_type);
      /* fn_idx number goes first so that a later atoi() when reading the xml file would work. */
      sprintf(tempbuffer, "%d (%s)", impl->fn_idx,
              impl->team->autotune_info->collective_algorithms[impl->optype][impl->fn_idx].name_str);
      /* sprintf(tempbuffer, "%d", impl->fn_idx); */
      myxml_createNode(temp_xml, (char*) "Best_Alg", NULL, NULL,tempbuffer);
      myxml_createNode(temp_xml, (char*) "Best_Tree", NULL, NULL, buffer);
      sprintf(tempbuffer, "%d",  impl->num_params);
      myxml_createNode(temp_xml, (char*) "Num_Params", NULL, NULL, tempbuffer);

      for(c=0; c<impl->num_params; c++) {
        char buff_idx[128];
        sprintf(tempbuffer, "%d",  impl->fn_idx);
        sprintf(buff_idx, "param_%d", c);
        sprintf(buffer, "%d", (int)impl->param_list[c]);
        myxml_createNode(temp_xml, buff_idx, NULL, NULL, buffer);
      }
    }
    temp = temp->next_interval;
  }
}


void gasnete_coll_dumpTuningState(char *filename, gasnete_coll_team_t team GASNETI_THREAD_FARG) {
  myxml_node_t *node;
  gasnet_image_t myrank = team->myrank;

  if(myrank==0 && team->autotune_info->search_enabled) {
    FILE *outstream;
    node = myxml_createNode(NULL, (char*) "machine", (char*)"CONFIG", (char*) GASNET_CONFIG_STRING, NULL);
    
    if(!filename) {
      if(team!=GASNET_TEAM_ALL) {fprintf(stderr, "WARNING: printing tuning output to default filename is not recommended for non-TEAM-ALL teams\n");}
      outstream = fopen("gasnet_coll_tuning_defaults.bin", "w");
    } else {
      outstream = fopen(filename, "w");
    }
    if (outstream == NULL) {
      gasneti_fatalerror("gasnete_coll_dumpTuningState failed to open output file %s!\n",
                         filename ? filename : "gasnet_coll_tuning_defaults.bin");
    }

    dump_tuning_state_helper(node, team->autotune_info->autotuner_defaults);
    myxml_printTreeBIN(outstream, node);
    fclose(outstream);
  }
}

static void dump_profile_helper(myxml_node_t *parent, gasnete_coll_autotune_index_entry_t *tuning_root) {
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


void gasnete_coll_dumpProfile(char *filename, gasnete_coll_team_t team GASNETI_THREAD_FARG) {
  myxml_node_t *node;
  gasnet_image_t myrank = team->myrank;
  
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

void gasnete_coll_loadTuningState(char *filename, gasnete_coll_team_t team GASNETI_THREAD_FARG) {
  myxml_node_t *nodes;
  gasnet_image_t myrank = team->myrank;
  
  PTHREAD_BARRIER(team, 1);
  {
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
      if(instream == NULL) {
        gasneti_fatalerror("gasnete_coll_loadTuningState() failed to open the tuning file %s!\n",
                           filename ? filename : "gasnet_coll_tuning_defaults.bin");
      }
      file_content = myxml_loadFile_into_bytestream(instream);
      
      /*initiate a broadcast to all the other nodes*/
      gasnete_coll_safe_broadcast(team, &file_content.size, &file_content.size, 0, sizeof(size_t), 1 GASNETI_THREAD_PASS);
      gasnete_coll_safe_broadcast(team, file_content.bytes, file_content.bytes, 0, file_content.size, 1 GASNETI_THREAD_PASS);
      nodes = myxml_loadTreeBYTESTREAM(file_content.bytes, file_content.size);
      team->autotune_info->autotuner_defaults = gasnete_coll_load_autotuner_defaults(team->autotune_info, nodes);
    } else {
      size_t nbytes;
      char *buffer;
      /*receive the file size*/
      gasnete_coll_safe_broadcast(team, &nbytes, NULL, 0, sizeof(size_t), 1 GASNETI_THREAD_PASS);
      buffer = gasneti_malloc(sizeof(char)*nbytes);
      /*receive the file contents*/
      gasnete_coll_safe_broadcast(team, buffer, NULL, 0, sizeof(char)*nbytes, 1 GASNETI_THREAD_PASS);
      nodes = myxml_loadTreeBYTESTREAM(buffer, nbytes);
      team->autotune_info->autotuner_defaults = gasnete_coll_load_autotuner_defaults(team->autotune_info, nodes);
    }
  }
  PTHREAD_BARRIER(team, 1);
  
}

void gasnete_coll_implementation_print(gasnete_coll_implementation_t impl, FILE *fp)
{
  int i;
  char str_buf[256];

  fprintf(fp, "gasnete_coll_implementation_t %p:\n", (void *)impl);

  gasneti_assert(impl != NULL);
  gasneti_assert(impl->optype < GASNET_COLL_NUM_COLL_OPTYPES);
  gasneti_assert(impl->team != NULL);
  gasneti_assert(impl->team->autotune_info != NULL);
  gasneti_assert(impl->team->autotune_info->collective_algorithms[impl->optype][impl->fn_idx].name_str != NULL);
  
  print_op_str(str_buf, impl->optype, impl->flags);
  fprintf(fp, "Collective Op type: %s (%d), Algorithm: %d (%s).\n",
          str_buf, impl->optype, impl->fn_idx,
          impl->team->autotune_info->collective_algorithms[impl->optype][impl->fn_idx].name_str);

  print_flag_str(str_buf, impl->flags);
  fprintf(fp, "flag: %s\n", str_buf);
  
  gasnete_coll_tree_type_to_str(str_buf, impl->tree_type);
  fprintf(fp, "tree_type: %s\n", str_buf);

  fprintf(fp, "Num of params: %d; ", impl->num_params);
    for (i=0; i<impl->num_params; i++) {
      fprintf(fp, "params[%d]=%u ", i, (unsigned int)impl->param_list[i]);
    }
  fprintf(fp, "\n\n");
  fflush(fp);
}
