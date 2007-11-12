/* 
* Description: Files for Autotuner
 * Copyright 2007, Rajesh Nishtala <rajeshn@eecs.berkeley.edu> Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* This is intended as a stub for the autotuner routines*/

#include "gasnet_coll_autotune.h"

struct gasnete_coll_autotune_info_t_ {
  gasnete_coll_tree_type_t bcast_tree_type;
  gasnete_coll_tree_type_t scatter_tree_type;
  gasnete_coll_tree_type_t gather_tree_type;
  
  size_t gather_all_dissem_limit;
  size_t exchange_dissem_limit;
  
  size_t pipe_seg_size;
};


/* These "set" routines are only intended for testing purposes. Eventually 
   The "get tree" routines will be the primary method of picking trees*/

gasnete_coll_autotune_info_t* gasnete_coll_autotune_init(gasnet_node_t mynode, gasnet_node_t total_nodes, gasnet_image_t my_images, gasnet_image_t total_images, size_t min_scratch_size) {
  /* read all the environment variables and setup the defaults*/
  gasnete_coll_autotune_info_t* ret;
  char *default_tree_type;
  gasnet_node_t default_tree_fanout;
  size_t dissem_limit;
  size_t dissem_limit_per_thread;
  
  ret = gasneti_malloc(sizeof(gasnete_coll_autotune_info_t));
  /* first read the environment variables for tree types*/
  default_tree_type = gasneti_getenv_withdefault("GASNET_COLL_ROOTED_GEOM", GASNETE_COLL_DEFAULT_TREE_TYPE_STR);
  default_tree_fanout = gasneti_getenv_int_withdefault("GASNET_COLL_ROOTED_ARITY", GASNETE_COLL_DEFAULT_TREE_FANOUT, 0);
  
  /* now over-ride the defaults w/ the collective specific tree types in the environment*/
  ret->bcast_tree_type = gasnete_coll_make_tree_type(gasneti_getenv_withdefault("GASNET_COLL_BROADCAST_GEOM", default_tree_type),
                                                     MIN(total_nodes, gasneti_getenv_int_withdefault("GASNET_COLL_BROADCAST_ARITY", default_tree_fanout, 0)));
  ret->scatter_tree_type = gasnete_coll_make_tree_type(gasneti_getenv_withdefault("GASNET_COLL_SCATTER_GEOM", default_tree_type),
                                                     MIN(total_nodes, gasneti_getenv_int_withdefault("GASNET_COLL_SCATTER_ARITY", default_tree_fanout, 0)));
  ret->gather_tree_type = gasnete_coll_make_tree_type(gasneti_getenv_withdefault("GASNET_COLL_GATHER_GEOM", default_tree_type),
                                                     MIN(total_nodes, gasneti_getenv_int_withdefault("GASNET_COLL_GATHER_ARITY", default_tree_fanout, 0)));
  
  dissem_limit = gasneti_getenv_int_withdefault("GASNET_COLL_GATHER_ALL_DISSEM_LIMIT", GASNETE_COLL_DEFAULT_DISSEM_LIMIT, 1);
  dissem_limit_per_thread = gasneti_getenv_int_withdefault("GASNET_COLL_GATHER_ALL_DISSEM_LIMIT_PER_THREAD", dissem_limit/my_images, 1);
  if(dissem_limit_per_thread*my_images != dissem_limit) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting environment values for GASNET_COLL_GATHER_ALL_DISSEM_LIMIT (%ld) and GASNET_COLL_GATHER_ALL_DISSEM_LIMIT_PER_THREAD (%ld)\n", (long int) dissem_limit, (long int) dissem_limit_per_thread);
      fprintf(stderr, "WARNING: Using: %ld\n", (long int) MIN(dissem_limit, dissem_limit_per_thread));
    }
  }
  ret->gather_all_dissem_limit = MIN(dissem_limit, dissem_limit_per_thread*my_images);
  
  dissem_limit = gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_LIMIT", GASNETE_COLL_DEFAULT_DISSEM_LIMIT, 1);
  dissem_limit_per_thread = gasneti_getenv_int_withdefault("GASNET_COLL_EXCHANGE_DISSEM_LIMIT_PER_THREAD", dissem_limit/(my_images*my_images), 1);
  if(dissem_limit_per_thread*my_images*my_images != dissem_limit) {
    if(mynode == 0) {
      fprintf(stderr, "WARNING: Conflicting environment values for GASNET_COLL_EXCHANGE_DISSEM_LIMIT (%ld) and GASNET_COLL_EXCHANGE_DISSEM_LIMIT_PER_THREAD (%ld)\n", (long int) dissem_limit, (long int) dissem_limit_per_thread);
      fprintf(stderr, "WARNING: Using: %ld\n", (long int) MIN(dissem_limit, dissem_limit_per_thread));
    }
  }
  ret->exchange_dissem_limit = MIN(dissem_limit, dissem_limit_per_thread*my_images*my_images);
  
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
              (int)ret->pipe_seg_size, total_images, (long int) gasnet_AMMaxLongRequest());
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
  
  return ret;
}

gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                             gasnete_coll_autotune_optype_t op_type, 
                                                             gasnet_node_t root, size_t nbytes, int flags) {
  switch(op_type) {
    case GASNETE_COLL_BROADCAST_OP: return autotune_info->bcast_tree_type;
    case GASNETE_COLL_SCATTER_OP: return autotune_info->scatter_tree_type;
    case GASNETE_COLL_GATHER_OP: return autotune_info->gather_tree_type;
    default: gasneti_fatalerror("unknown tree based collective op type"); return autotune_info->bcast_tree_type;
  }
}

size_t gasnete_coll_get_dissem_limit(gasnete_coll_autotune_info_t* autotune_info, gasnete_coll_autotune_optype_t op_type, int flags) {
  switch(op_type) {
    case GASNETE_COLL_GATHER_ALL_OP: return autotune_info->gather_all_dissem_limit;
    case GASNETE_COLL_EXCHANGE_OP: return autotune_info->exchange_dissem_limit;
    default:  gasneti_fatalerror("unknown dissem based collective op type"); return 0;
  }
}


size_t gasnete_coll_get_pipe_seg_size(gasnete_coll_autotune_info_t* autotune_info, gasnete_coll_autotune_optype_t op_type, int flags){
  return autotune_info->pipe_seg_size;
}
