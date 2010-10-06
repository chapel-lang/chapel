/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_putget.c,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.78 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* for now this file will be directly included in refcoll.c so no need to worry*/
/* about including the header files*/
#include <gasnet_internal.h>
#include <gasnet_coll.h>

#include <gasnet_coll_internal.h>
#include <gasnet_coll_trees.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_autotune_internal.h>
#include <gasnet_vis.h>

#define USE_CONSENSUS_BARRIER 1
typedef struct {int num_handles; gasnet_coll_handle_t *handles;} gasnete_coll_handle_vec_t;
/*for the segmented algorithms limit the maximum number of subcollectives to 2048*/


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */

/* bcast Get: all nodes perform uncoordinated gets */
static int gasnete_coll_pf_bcast_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        data->handle = gasnete_get_nb_bulk(args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode), args->src,
                                           args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);

  }
  
  return result;
}
extern gasnet_coll_handle_t

gasnete_coll_bcast_Get(gasnet_team_handle_t team,
                       void * dst,
                       gasnet_image_t srcimage, void *src,
                       size_t nbytes, int flags,
                       gasnete_coll_implementation_t coll_params,
                       uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  gasneti_assert(flags & GASNET_COLL_SINGLE);
  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);
  gasneti_assert(coll_params->num_params == 0);
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_Get, options, NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* bcast Put: root node performs carefully ordered puts */
static int gasnete_coll_pf_bcast_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank != args->srcnode) {
        /* Nothing to do */
      } else {
        void   *src   = args->src;
        void   *dst   = args->dst;
        size_t nbytes = args->nbytes;
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        
        /* Queue PUTS in an NBI access region */
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        {
          int i;
          
          /* Put to nodes to the "right" of ourself */
          for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, i), dst, src, nbytes GASNETE_THREAD_PASS);
          }
          /* Put to nodes to the "left" of ourself */
          for (i = 0; i < op->team->myrank; ++i) {
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, i), dst, src, nbytes GASNETE_THREAD_PASS);
          }
        }
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* Do local copy LAST, perhaps overlapping with communication */
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dst, src, nbytes);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Put(gasnet_team_handle_t team,
                       void * dst,
                       gasnet_image_t srcimage, void *src,
                       size_t nbytes, int flags,
                       gasnete_coll_implementation_t coll_params,
                       uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  gasneti_assert(flags & GASNET_COLL_SINGLE);
  gasneti_assert(flags & GASNET_COLL_DST_IN_SEGMENT);
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_Put, options,
                                           NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


/* TreePut: COLL_SINGLE IN_NO/ALL_SYNC/OUT_* */
/* TreePutScratch: COLL_LOCAL (all sync variations) and COLL_SINGLE IN_MYSYNC/OUT_* */
/* bcast TreePut */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
/* Naturally IN_NOSYNC, OUT_MYSYNC */
/* max size is MaxLongRequest */
static int gasnete_coll_pf_bcast_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  
  gasnet_node_t barrier_count;
  int result = 0;
  int child;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      data->state = 1; 
      
  case 1:
        if(!(op->flags & GASNET_COLL_IN_NOSYNC)) {
          if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
            break;
          }
          if (op->team->myrank != args->srcnode) {
            gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
          }
        }
        data->state = 2;
      
      case 2:
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      if (op->team->myrank == args->srcnode) {
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        for (child = 0; child < child_count; child++) {
          if(tree->geom->subtree_sizes[child] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, args->src, args->nbytes GASNETE_THREAD_PASS);
          } else { 
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, 
                                                 args->src, args->nbytes, 0, 1);
            
          }
        }
        data->handle  = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (child_count == 0) {
        /* leaves fall right through*/
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);	
        for (child = 0; child < child_count; child++) {
          if(tree->geom->subtree_sizes[child] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, args->dst, args->nbytes GASNETE_THREAD_PASS);
          } else { 
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, 
                                                 args->dst, args->nbytes, 0, 1);
            
          }
        }
        data->handle  = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);        
      } else {
        break;	/* Internal node waiting for parent to push data and signal */
      }
      data->state = 3;
      
      case 3:     /* sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 4;
      
      case 4:	/* Optional OUT barrier */
#if 1
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
#else
      /* Custom Out Barrrier Here */
      /* XXX: Idea Sync all the puts (acts as notify for the leaves) */
      /* XXX: Perform barrier over internal nodes */
      /* XXX: Wake leaves with an AMShort */
#endif
      data->state = 5;
      
      case 5: /*done*/
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePut(gasnet_team_handle_t team,
                           void *dst,
                           gasnet_image_t srcimage, void *src,
                           size_t nbytes, int flags,
                           gasnete_coll_implementation_t coll_params,
                           uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  
  gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreePut, options,
                                           gasnete_coll_tree_init(coll_params->tree_type, 
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
}

/* bcast TreePutScratch */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */

/* Naturally IN_MYSYNC, OUT_MYSYNC and should only be used when IN_MYSYNC is needed*/
/* Goes through the scratch space so COLL_LOCAL w/ IN_MYSYNC also works*/
/* Use gasnete_coll_bcast_TreePut() for all other cases*/
/* max size is MaxLongRequest */
static int gasnete_coll_pf_bcast_TreePutScratch(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int result = 0;
  int child;
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
      case 1: /*thread barrier*/
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      
      data->state = 2;
      case 2:
      if((op->flags & GASNET_COLL_IN_ALLSYNC)) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 3;
      
      
      
      case 3:
      if (op->team->myrank == args->srcnode) {
        for (child = 0; child < child_count; child++) {
          
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 
                                          (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                          args->src, args->nbytes, 0, 1);
          
        }
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        
        for (child = 0; child < child_count; child++) {
          
          gasnete_coll_p2p_signalling_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 
                                                   (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                                   (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                                   args->nbytes, 0, 1);
          
        }
        
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, args->nbytes);
        
      } else {
        break;	/* Waiting for parent to push data and signal */
      }
      data->state = 4;
      
      
      case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      /*free up the scratch space used by this op*/
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePutScratch(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags,
                                  gasnete_coll_implementation_t coll_params,
                                  uint32_t sequence
                                  GASNETE_THREAD_FARG)
{
 /*never allocated an insync barrier since it is folded in with the collective*/
  int options =
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  
  gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreePutScratch, options,
                                           gasnete_coll_tree_init(coll_params->tree_type, 
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
	
}

static int gasnete_coll_pf_bcast_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size;
      int num_segs;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();

      
#if !GASNET_SEQ
      gasnet_image_t srcproc = args->srcimage;
#else
      gasnet_node_t srcproc = args->srcnode;
#endif
      size_t nbytes = args->nbytes;
      size_t sent_bytes=0;
      
      int i;
      
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
    
      seg_size = (size_t) op->param_list[0];
      num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
      
      /*
       the TreePut routines only work w/ the single address mode and therefore we need ensure choose 
       between TreePut and TreePutScratch which collective routine gets invoked when the we are segmenting broadcast
       */
      if(flags & GASNET_COLL_SINGLE) {
        for(i=0; i<num_segs-1; i++) {
          handle_vec->handles[i] = gasnete_coll_bcast_TreePut(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                              srcproc, gasnete_coll_scale_ptr(args->src, sent_bytes, 1), seg_size, flags, 
                                                              impl, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes+=seg_size;
        }
        handle_vec->handles[i] = gasnete_coll_bcast_TreePut(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                            srcproc, 
                                                            gasnete_coll_scale_ptr(args->src, sent_bytes, 1), args->nbytes-sent_bytes, flags, 
                                                            impl, op->sequence+i+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      } else {
        for(i=0; i<num_segs-1; i++) {
          handle_vec->handles[i] = gasnete_coll_bcast_TreePutScratch(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                                     srcproc, gasnete_coll_scale_ptr(args->src, sent_bytes, 1), seg_size, flags, 
                                                                     impl, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes+=seg_size;
        }
        handle_vec->handles[i] = gasnete_coll_bcast_TreePutScratch(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                                   srcproc, 
                                                                   gasnete_coll_scale_ptr(args->src, sent_bytes, 1), args->nbytes-sent_bytes, flags, 
                                                                   impl, op->sequence+i+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);          
      }
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2;
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePutSeg(gasnet_team_handle_t team,
                              void * dst,
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags,
                             gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));

  
  size_t seg_size;
  uint32_t num_segs;
  
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreePutSeg, options,
                                           gasnete_coll_tree_init(coll_params->tree_type, 
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS), 
                                           (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_bcast_ScatterAllgather(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result =0;
  
  switch (data->state) {
  case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
  case 1:	/* Initiate Scatter */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
        gasnet_coll_handle_t *handle;
        uint8_t *tempspace;
        size_t seg_size = (args->nbytes)/op->team->total_ranks;
        size_t remainder = (args->nbytes) % op->team->total_ranks;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        int i;
#if !GASNET_SEQ
        gasnet_image_t srcproc = args->srcimage;
#else
        gasnet_node_t srcproc = args->srcnode;
#endif 
       
        data->private_data = gasneti_malloc(sizeof(gasnet_coll_handle_t)*2+seg_size);
        handle = (gasnet_coll_handle_t*) data->private_data;
        
        tempspace = ((uint8_t*) data->private_data) + sizeof(gasnet_coll_handle_t)*2; 

        handle[0] = gasnete_coll_scatter_nb_default(op->team, tempspace, srcproc, args->src, seg_size, flags | GASNET_COLL_LOCAL, op->sequence+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle[0] GASNETE_THREAD_PASS);
        
        if(remainder > 0) {

          handle[1] = gasnete_coll_broadcast_nb_default(op->team, 
                                                        gasnete_coll_scale_ptr(args->dst,op->team->total_ranks,seg_size), 
                                                        srcproc, 
                                                        gasnete_coll_scale_ptr(args->src,op->team->total_ranks,seg_size), 
                                                        remainder, flags, op->sequence+2 GASNETE_THREAD_PASS);
        } else {
          handle[1] = GASNET_COLL_INVALID_HANDLE;
        }
        gasnete_coll_save_coll_handle(&handle[1] GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
  case 2:	/* Initiate all gather*/
      
      /*we only need the scatter to be done before we initiate the allgather*/
      if (!gasnete_coll_generic_coll_sync((gasnet_coll_handle_t*) data->private_data, 1 GASNETE_THREAD_PASS)) {
        break;
      } else {
        gasnet_coll_handle_t *handle;
        uint8_t *tempspace;
        size_t seg_size = (args->nbytes)/op->team->total_ranks;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        
        handle = (gasnet_coll_handle_t*) data->private_data;
      
        tempspace = ((uint8_t*) data->private_data) + sizeof(gasnet_coll_handle_t)*2; 
        if(seg_size) {
          handle[0] = gasnete_coll_gather_all_nb_default(op->team, args->dst, tempspace, seg_size, flags | GASNET_COLL_LOCAL, op->sequence+3 GASNETE_THREAD_PASS);
        } else {
          handle[0] = GASNET_COLL_INVALID_HANDLE;
        }
        gasnete_coll_save_coll_handle(&handle[0] GASNETE_THREAD_PASS);
        
        data->state = 3;
      }
      
  case 3:
      /*sync both the allgather and broadcast*/
      if (!gasnete_coll_generic_coll_sync((gasnet_coll_handle_t*) data->private_data, 2 GASNETE_THREAD_PASS)) {
        break;
      }
      data->state =4;
      
  case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_bcast_ScatterAllgather(gasnet_team_handle_t team,
                              void * dst,
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags,
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC | GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  
  size_t seg_size;
  uint32_t num_segs;
  

  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_ScatterAllgather, options,
                                           NULL, 
                                           3 + 2*GASNETE_COLL_MAX_NUM_SEGS +team->total_ranks, /*scatter and broadcast can initiate upto GASNETE_COLL_MAX_NUM_SEGS sub scatters, broadcasts and 
   all gather can initiate upto total ranks sub collectives*/
                                           
                                           coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcastM_nb() */

/* bcastM Get: all nodes perform uncoordinated gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_bcastM_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0),
                                     args->src, args->nbytes);
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        /* Get only the 1st local image */
        data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0),
                                           GASNETE_COLL_REL2ACT(op->team, args->srcnode), args->src, args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement and perform local copies */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      } else if (op->team->myrank != args->srcnode) {
        void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0);
        gasneti_sync_reads();
        gasnete_coll_local_broadcast(op->team->my_images - 1, p + 1, *p, args->nbytes);
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Get(gasnet_team_handle_t team,
                        void * const dstlist[],
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags, 
                        gasnete_coll_implementation_t coll_params, 
                        uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_Get, options,
                                            NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* bcastM Put: root node performs carefully ordered puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_bcastM_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1: 	/* Initiate data movement */
      if (op->team->myrank != args->srcnode) {
        /* Nothing to do */
      } else {
        void   *src   = args->src;
        size_t nbytes = args->nbytes;
        int i, j, limit;
        void * const *p;
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        
        /* Queue PUTS in an NBI access region */
        /* We don't use VIS here, since that would send the same data multiple times */
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        {
          /* Put to nodes to the "right" of ourself */
          if (op->team->myrank < op->team->total_ranks - 1) {
            p = &GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, op->team->myrank + 1);
            for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
              limit = op->team->all_images[i];
              for (j = 0; j < limit; ++j) {
                gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,i), *p, src, nbytes GASNETE_THREAD_PASS);
                ++p;
              }
            }
          }
          /* Put to nodes to the "left" of ourself */
          if (op->team->myrank != 0) {
            p = &GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, 0);
            for (i = 0; i < op->team->myrank; ++i) {
              limit = op->team->all_images[i];
              for (j = 0; j < limit; ++j) {
                gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,i), *p, src, nbytes GASNETE_THREAD_PASS);
                ++p;
              }
            }
          }
        }
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* Do local copy LAST, perhaps overlapping with communication */
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0),
                                     src, nbytes);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Put(gasnet_team_handle_t team,
                        void * const dstlist[],
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_Put, options,
                                            NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_bcastM_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t barrier_count;
  int result = 0;
  int child;
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  
  switch (data->state) {
  case 0:	/* thread barrier if it's thread local data or in my/all sync*/
    
    if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
      break;
    }
    
    data->state = 1; 
      
      case 1:
      if(!(op->flags & GASNET_COLL_IN_NOSYNC)) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 2;
      
      case 2:
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      if (op->team->myrank == args->srcnode) {
        gasneti_sync_reads();
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        for (child = 0; child < child_count; child++) {
          if(tree->geom->subtree_sizes[child] == 1 && op->team->all_images[children[child]] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[child]), GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, children[child]), args->src, args->nbytes GASNETE_THREAD_PASS);
          } else {
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[child]), GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, children[child]), 
                                                 args->src, args->nbytes, 0, 1);
            
          }
        }
        data->handle  = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0),
                                     args->src, args->nbytes);
      } else if (child_count == 0 && op->team->my_images == 1) {
        /* leaves fall right through*/
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);	
        for (child = 0; child < child_count; child++) {
          if(tree->geom->subtree_sizes[child] == 1 && op->team->all_images[children[child]] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[child]), GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, children[child]), 
                                 GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), args->nbytes GASNETE_THREAD_PASS);
          } else { 
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[child]), GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, children[child]), 
                                                 GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), args->nbytes, 0, 1);
            
          }
        }
        data->handle  = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);  
        /** XXX: Here we might do extra work by copying from teh same source and dest **/
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), args->nbytes);
        
      } else {
        break;	/* Internal node waiting for parent to push data and signal */
      }
      data->state = 3;
      
      case 3:     /* sync data movement */
      if (data->handle!=GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 4;
      
      case 4:	/* Optional OUT barrier */
#if USE_CONSENSUS_BARRIER
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      data->state = 5;
      case 5:
#else
        if((op->team->total_ranks > 1) &&
           ((op->flags & GASNET_COLL_OUT_ALLSYNC) || (op->flags & GASNET_COLL_OUT_MYSYNC))) {
          if(tree->geom->mysubtree_size > 1) {
            /*intermediate node*/
            /*wait for all non leaf children to report and report to the parent*/
            if(gasneti_weakatomic_read(&data->p2p->counter[1],0) < tree->geom->num_non_leaf_children) {
            break;
          }
          if(args->srcnode!=op->team->myrank) 
            gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), 1);
        }
      } 
      /* Custom Out Barrrier Here */
      /* XXX: Idea Sync all the puts (acts as notify for the leaves) */
      /* XXX: Perform barrier over internal nodes */
      /* XXX: Wake leaves with an AMShort */
      

      data->state = 5;
      
      case 5: /*wait on the out barrier on the way down*/

        if((op->team->total_ranks > 1) &&
           ((op->flags & GASNET_COLL_OUT_ALLSYNC) || (op->flags & GASNET_COLL_OUT_MYSYNC))) {
          if(args->srcnode!=op->team->myrank) {
            if(gasneti_weakatomic_read(&data->p2p->counter[1],0) < (tree->geom->num_non_leaf_children + 1)) {
              break;
            }
          }
          /*signal all the children that the barrier is done*/
          for(child = 0; child < child_count; child++) {
            gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 1);
          }
        }

        data->state = 6;
      case 6: /*done*/
#endif
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePut(gasnet_team_handle_t team,
                            void * const dstlist[],
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, int flags, 
                            gasnete_coll_implementation_t coll_params,    
                            uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |*/
#if USE_CONSENSUS_BARRIER    
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) | 
#endif
  GASNETE_COLL_GENERIC_OPT_P2P;

  gasnete_coll_tree_data_t *tree_info = 
    gasnete_coll_tree_init(coll_params->tree_type, 
      gasnete_coll_image_node(team,srcimage), team
      GASNETE_THREAD_PASS);

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_TreePut, options,
                                            tree_info,
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


static int gasnete_coll_pf_bcastM_TreePutScratch(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int result = 0;
  int child;
  
  switch (data->state) {
    case 0: /*thread barrier*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }      
      data->state = 1;
      case 1:
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
        break;
      }
      data->state = 2;
      case 2:
      if((op->flags & GASNET_COLL_IN_ALLSYNC)) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 3;
      
      
      
      case 3:
      if (op->team->myrank == args->srcnode) {
        gasneti_sync_reads();
        
        for (child = 0; child < child_count; child++) {
          
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 
                                          (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                          args->src, args->nbytes, 0, 1);
          
        }
        /* local bcast*/
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     args->src, args->nbytes);
        
        
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        
        for (child = 0; child < child_count; child++) {
          
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 
                                          (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                          (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                          args->nbytes, 0, 1);
          
        }
        /* local bcast*/
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, args->nbytes);
        
      } else {
        break;	/* Waiting for parent to push data and signal */
      }
      data->state = 4;
      
 
#if USE_CONSENSUS_BARRIER
      case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      data->state = 5;
      
      case 5:
#else
     case 4:	/* Optional OUT barrier */

      if((op->team->total_ranks > 1) &&
         (op->flags & GASNET_COLL_OUT_ALLSYNC)) {
        /*intermediate node*/
        /*wait for all non leaf children to report and report to the parent*/
        if(gasneti_weakatomic_read(&data->p2p->counter[1],0) < child_count) {
          break;
        }
        if(args->srcnode!=op->team->myrank) 
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), 1);
      } 
      /* Custom Out Barrrier Here */
      /* XXX: Idea Sync all the puts (acts as notify for the leaves) */
      /* XXX: Perform barrier over internal nodes */
      /* XXX: Wake leaves with an AMShort */

      data->state = 5;
      
      case 5: /*wait on the out barrier on the way down*/
        if((op->team->total_ranks > 1) &&
           (op->flags & GASNET_COLL_OUT_ALLSYNC)) {
          if(args->srcnode!=op->team->myrank) {
            if(gasneti_weakatomic_read(&data->p2p->counter[1],0) < child_count+1) {
              break;
            }
          }
          /*signal all the children that the barrier is done*/
          for(child = 0; child < child_count; child++) {
            gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 1);
          }
        }

        data->state = 6;
      case 6: /*done*/
#endif
        gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      /*free up the scratch space used by this op*/
      
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePutScratch(gasnet_team_handle_t team,
                                   void * const dstlist[],
                                   gasnet_image_t srcimage, void *src,
                                   size_t nbytes, int flags, 
                                   gasnete_coll_implementation_t coll_params,    
                                   uint32_t sequence
                                   GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |*/
#if USE_CONSENSUS_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
#endif
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_TreePutScratch, options,
                                            gasnete_coll_tree_init(coll_params->tree_type, 
                                                                   gasnete_coll_image_node(team,srcimage), team
                                                                   GASNETE_THREAD_PASS),
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}




static int gasnete_coll_pf_bcastM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result =0;
  
  switch (data->state) {
  case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
    {
      gasnete_coll_handle_vec_t *handle_vec;
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
      size_t seg_size;
      int num_segs;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      void ** dstlist;
      size_t nbytes = args->nbytes;
      size_t sent_bytes=0;
#if !GASNET_SEQ
      gasnet_image_t srcproc = args->srcimage;
#else
      gasnet_node_t srcproc = args->srcnode;
#endif
      
      int i;
      gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? op->team->my_images : op->team->total_images);
  
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      seg_size = (size_t) op->param_list[0];
      num_segs = (args->nbytes + seg_size - 1)/seg_size;
      
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
      dstlist = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));
      
      if(flags & GASNET_COLL_SINGLE) {
        for(i=0; i<num_segs-1; i++) {
          gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
          handle_vec->handles[i] = gasnete_coll_bcastM_TreePut(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), seg_size, flags, 
                                                               impl, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes+=seg_size;
        }
        gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_bcastM_TreePut(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes),  args->nbytes-sent_bytes, flags, 
                                                             impl, op->sequence+i+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      } else {
        for(i=0; i<num_segs-1; i++) {
          gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
          handle_vec->handles[i] = gasnete_coll_bcastM_TreePutScratch(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes),  seg_size, flags, 
                                                                      impl, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes+=seg_size;
        }
        gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_bcastM_TreePutScratch(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), args->nbytes-sent_bytes, flags, 
                                                                    impl, op->sequence+i+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      }
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2;
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePutSeg(gasnet_team_handle_t team,
                               void * const dstlist[],
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags,
                               gasnete_coll_implementation_t coll_params,
                               uint32_t sequence
                               GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));
  
  size_t seg_size;
  uint32_t num_segs;
  
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  num_segs = (nbytes + seg_size - 1)/seg_size;
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_TreePutSeg, options,
                                            gasnete_coll_tree_init(coll_params->tree_type, 
                                                                   gasnete_coll_image_node(team,srcimage), team
                                                                   GASNETE_THREAD_PASS), 
                                            (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list
                                            GASNETE_THREAD_PASS);
}


static int gasnete_coll_pf_bcastM_ScatterAllgather(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
    case 1:	/* Initiate Scatter */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
    {
      gasnet_coll_handle_t *handle;
      uint8_t *tempspace;
      size_t seg_size = (args->nbytes)/op->team->total_ranks;
      size_t remainder = (args->nbytes) % op->team->total_ranks;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      int i;
      int num_addrs;
#if !GASNET_SEQ
      gasnet_image_t srcproc = args->srcimage;
#else
      gasnet_node_t srcproc = args->srcnode;
#endif 
      
      
      data->private_data = gasneti_malloc(sizeof(gasnet_coll_handle_t)*2+seg_size);
      handle = (gasnet_coll_handle_t*) data->private_data;
      
      tempspace = ((uint8_t*) data->private_data) + sizeof(gasnet_coll_handle_t)*2;
      
      if(seg_size > 0) {
        handle[0] = gasnete_coll_scatter_nb_default(op->team, tempspace, srcproc, args->src, seg_size, flags | GASNET_COLL_LOCAL, op->sequence+1 GASNETE_THREAD_PASS);
      } else {
        handle[0] = GASNET_COLL_INVALID_HANDLE;
      }
      gasnete_coll_save_coll_handle(&handle[0] GASNETE_THREAD_PASS);
      
      if(remainder > 0) {
        
        handle[1] = gasnete_coll_broadcast_nb_default(op->team, 
                                                      gasnete_coll_scale_ptr(GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),op->team->total_ranks,seg_size), 
                                                      srcproc, 
                                                      gasnete_coll_scale_ptr(args->src,op->team->total_ranks,seg_size), 
                                                      remainder, flags, op->sequence+2 GASNETE_THREAD_PASS);
      } else {
        handle[1] = GASNET_COLL_INVALID_HANDLE;
      }
      gasnete_coll_save_coll_handle(&handle[1] GASNETE_THREAD_PASS);
    }
      data->state = 2;
      
    case 2:	/* Initiate all gather*/
      
      /*we only need the scatter to be done before we initiate the allgather*/
      if (!gasnete_coll_generic_coll_sync((gasnet_coll_handle_t*) data->private_data, 1 GASNETE_THREAD_PASS)) {
        break;
      } else {
        gasnet_coll_handle_t *handle;
        uint8_t *tempspace;
        int num_addrs;
        size_t seg_size = (args->nbytes)/op->team->total_ranks;
        size_t remainder = (args->nbytes)% op->team->total_ranks;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        
        handle = (gasnet_coll_handle_t*) data->private_data;
        
        
        
        tempspace = ((uint8_t*) data->private_data) + sizeof(gasnet_coll_handle_t)*2;
        if(seg_size) {
          handle[0] = gasnete_coll_gather_all_nb_default(op->team, GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), tempspace, seg_size, flags | GASNET_COLL_LOCAL, op->sequence+3 GASNETE_THREAD_PASS);
        } else {
          handle[0] = GASNET_COLL_INVALID_HANDLE;
        }
        gasnete_coll_save_coll_handle(&handle[0] GASNETE_THREAD_PASS);
      }
      data->state = 3;
      
      
    case 3:
      /*sync both the allgather and broadcast*/
      if (!gasnete_coll_generic_coll_sync((gasnet_coll_handle_t*) data->private_data, 2 GASNETE_THREAD_PASS)) {
        break;
      }
      gasnete_coll_local_broadcast(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), args->nbytes);
      data->state =4;
      
    case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
  
extern gasnet_coll_handle_t
gasnete_coll_bcastM_ScatterAllgather(gasnet_team_handle_t team,
                               void * const dstlist[],
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags,
                               gasnete_coll_implementation_t coll_params,
                               uint32_t sequence
                               GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_ScatterAllgather, options,
                                            NULL, 
                                            3+team->total_ranks, coll_params->num_params, coll_params->param_list 
                                            GASNETE_THREAD_PASS);
}



/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat Get: all nodes perform uncoordinated gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scat_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst,
                                            gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
                                            args->nbytes);
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        data->handle = gasnete_get_nb_bulk(args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode),
                                           gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
                                           args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Get(gasnet_team_handle_t team,
                      void *dst,
                      gasnet_image_t srcimage, void *src,
                      size_t nbytes, size_t dist, int flags, 
                      gasnete_coll_implementation_t coll_params,
                      uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scat_Get, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* scat Put: root node performs carefully ordered puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scat_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:
      if (op->team->myrank != args->srcnode) {
        /* Nothing to do */
      } else {
        void   *dst   = args->dst;
        size_t nbytes = args->nbytes;
        uintptr_t p;
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        
        /* Queue PUTS in an NBI access region */
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        {
          int i;
          
          /* Put to nodes to the "right" of ourself */
          p = (uintptr_t)gasnete_coll_scale_ptr(args->src, (op->team->myrank + 1), nbytes);
          for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i, p += nbytes) {
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, i), dst, (void *)p, nbytes GASNETE_THREAD_PASS);
          }
          /* Put to nodes to the "left" of ourself */
          p = (uintptr_t)gasnete_coll_scale_ptr(args->src, 0, nbytes);
          for (i = 0; i < op->team->myrank; ++i, p += nbytes) {
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, i), dst, (void *)p, nbytes GASNETE_THREAD_PASS);
          }
        }
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* Do local copy LAST, perhaps overlapping with communication */
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dst,
                                            gasnete_coll_scale_ptr(args->src, op->team->myrank, nbytes),
                                            nbytes);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Put(gasnet_team_handle_t team,
                      void *dst,
                      gasnet_image_t srcimage, void *src,
                      size_t nbytes, size_t dist, int flags, 
                      gasnete_coll_implementation_t coll_params,
                      uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scat_Put, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}



/* scat Put: root node performs carefully ordered puts */
/* Valid for SINGLE and LOCAL, any size < scratch size ... since we write into the scratch space we need not worry about whether the sender knows the dest address */
static int gasnete_coll_pf_scat_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t barrier_count;
  uint8_t direct_put_ok = (!((op->flags & GASNET_COLL_IN_MYSYNC) || (op->flags & GASNET_COLL_OUT_MYSYNC) ||(op->flags & GASNET_COLL_LOCAL) ||  (args->nbytes !=args->dist))); 

  int result = 0,p=1,i,j;
  uint64_t sent_bytes=0;
  
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
      case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)){
        break;
      }
      
      data->state = 2;
      
      case 2:
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          /*no need for memory barriers here since the change of the value is the data itself*/
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 3;
      
      case 3:
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      if (op->team->myrank == args->srcnode) {
        if(args->dist!=args->nbytes) {
          int8_t *myscratchpos = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          int8_t *send_arr;
          /*compress the data and send it*/
          for(i=0,p=0; i<child_count; i++) {
             gasnet_node_t child = children[i];
            send_arr = myscratchpos+p*args->nbytes;
            for(j=0; j<tree->geom->subtree_sizes[i]; j++,p++) {
              size_t src_pos = tree->geom->child_offset[i]+j+1;
              src_pos=(src_pos+args->srcnode)%op->team->total_ranks;
              GASNETE_FAST_UNALIGNED_MEMCPY(myscratchpos+p*args->nbytes, gasnete_coll_scale_ptr(args->src, args->dist, src_pos), args->nbytes);
            }
            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              /* use AMLong*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              send_arr,
                                              args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
            } else {
              if(tree->geom->subtree_sizes[i] == 1  && direct_put_ok) {
                /* if i am sending to a leaf into dest*/
                /* Perform NB Put */
                gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[i]), args->dst,
                                     send_arr,
                                     args->nbytes*tree->geom->subtree_sizes[i] GASNETE_THREAD_PASS);                              
              } else {
                /* else if i am sending to internal node async long into scratch space*/
                gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                     (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                     send_arr, 
                                                     args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
                
              }
            }
          }
            
        
              
          GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, gasnete_coll_scale_ptr(args->src,args->dist,(op->team->myrank)), args->nbytes);
        } else {
          int8_t* src_arr;
          int8_t* scratch_space= (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
          
          sent_bytes+=args->nbytes;
          if(args->srcnode == 0) {
            /*if 0 is the source node then there's no need to shift the array around we can do it direclty from the source*/
            src_arr = args->src;
          } else {
            /*perform the rotations*/
            src_arr = scratch_space;
            gasneti_assert(tree->geom->num_rotations==1); /*for now only works w/ one level trees*/
            gasnete_coll_local_rotate_left(scratch_space, args->src, 
                                      args->nbytes, op->team->total_ranks, tree->geom->rotation_points[0]);
          }
          for(i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            int8_t *send_arr = gasnete_coll_scale_ptr(src_arr,(tree->geom->child_offset[i]+1),args->nbytes);

            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              /* use AMLong*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                               send_arr,
                                              args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
            } else {
              if(tree->geom->subtree_sizes[i] == 1  && direct_put_ok) {
                /* if i am sending to a leaf into dest*/
                /* Perform NB Put */
                gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[i]), args->dst,
                                     send_arr,
                                     args->nbytes*tree->geom->subtree_sizes[i] GASNETE_THREAD_PASS);                              
              } else {
                /* else if i am sending to internal node async long into scratch space*/
                if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
                  /*no need to wait for local completion since the barrier will indicate that the data movement is complete*/
                  gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                       (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                       send_arr, 
                                                       args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
                } else {
                  /*we'll get no singal that the data movement is done so we need to wait until we get that signal*/
                  gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                       (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                       send_arr, 
                                                       args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
                  
                }
              }
            }
          }
          GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, src_arr, args->nbytes);
          data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
          gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        }
      } else if(child_count == 0  && direct_put_ok) {
        /* for leaves with out no/all sync ... fall through right through*/
        /* for out mysync the leave nodes will need to wait for the data to arrive*/
      } else if(data->p2p->state[0]){
        int8_t *scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        /*read memory barrier to ensure that the reading of the data doesn't preceed the flag*/
        gasneti_sync_reads();
        /*skip the first slot of the input array since it is destined for me*/
        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
          if(tree->geom->subtree_sizes[i]==1 && direct_put_ok) {
            /* if i am sending to a leaf ... put it right where it needs to go */
            gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                            args->dst, 
                                            gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes),
                                            args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
          } else {
            /*need to stick the data into scratch space*/
            if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
              /*out barrier will tell us when put is complete so therefore no need for local completion here*/
              gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                   (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                    gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes),
                                                    args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
            } else {
              /*need to wait for local completion*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                   (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                   gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes),
                                                   args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
              
            }
          }
          sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes;
        }
        
        
        /* In the case of Mysync the data is always sent to the scratch space so copy it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, scratchspace, args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      
      data->state = 4;
      
      case 4: /* wait for all puts to finish*/
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      
      data->state = 5;
      
      
      case 5:	/* Final Out Barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
        
      }
      data->state = 6;
      
      case 6: /*done*/    
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_TreePut(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, size_t dist, int flags, 
                          gasnete_coll_implementation_t coll_params,   
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scat_TreePut, options,
                                         gasnete_coll_tree_init(coll_params->tree_type,
                                                                gasnete_coll_image_node(team,srcimage), team
                                                                GASNETE_THREAD_PASS),
                                         sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}  

/*like the scatter treePut but data is not resuffled, instead it is sent from
  directly where it is*/
static int gasnete_coll_pf_scat_TreePutNoCopy(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t barrier_count;
  uint8_t direct_put_ok = (!((op->flags & GASNET_COLL_IN_MYSYNC) || (op->flags & GASNET_COLL_OUT_MYSYNC) ||(op->flags & GASNET_COLL_LOCAL) ||  (args->nbytes !=args->dist))); 

  int result = 0,p=1,i,j;
  uint64_t sent_bytes=0;
  
  
  switch (data->state) {
  case 0: /*scratch alloc*/
  
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
      case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)){
        break;
      }
      
      data->state = 2;
      
      case 2:
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          /*no need for memory barriers here since the change of the value is the data itself*/
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 3;
      
      case 3:
     if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
     
      if (op->team->myrank == args->srcnode) {
        if(args->dist!=args->nbytes) {
          gasneti_fatalerror("not yet supported!");
        } else {
          gasneti_assert(tree->geom->num_rotations == 1);
          
          for(i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
            if(children[i]+tree->geom->subtree_sizes[i] <= op->team->total_ranks) {
              /*can do one put since data is already contiguous*/
              int8_t *send_arr = gasnete_coll_scale_ptr(args->src,(tree->geom->child_offset[i]+1+op->team->myrank)%op->team->total_ranks,args->nbytes);

              if(tree->geom->subtree_sizes[i] == 1 && direct_put_ok) {
                gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[i]), args->dst, send_arr, args->nbytes GASNETE_THREAD_PASS);
              } else {
                gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                send_arr,
                                                args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
                
              }
            } else {
              int8_t *send_arr = gasnete_coll_scale_ptr(args->src,(tree->geom->child_offset[i]+1+op->team->myrank),args->nbytes);
              int8_t *send_arr2 = args->src;
              size_t first_part = op->team->total_ranks - children[i];
              size_t second_part = tree->geom->subtree_sizes[i]-first_part;
              /*we dont' need the case for a direct put here because if there is exactly oen node in the subtree then the top condition will always be true*/
              /*need to do two puts w/ the wrap*/

              gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                                 (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                 send_arr,
                                                 args->nbytes*first_part,0);
              gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                                 (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i]+first_part*args->nbytes, 
                                                 send_arr2,
                                                 args->nbytes*second_part,0);
              
            }
            data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
            gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
          }
        }
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, gasnete_coll_scale_ptr(args->src,args->dist,(op->team->myrank)), args->nbytes);
      } else if(child_count == 0  && direct_put_ok) {
        /* for leaves with out no/all sync ... fall through right through*/

        /* for out mysync the leave nodes will need to wait for the data to arrive*/
      } else if (data->p2p->state[0] 
                 || (op->team->myrank+tree->geom->mysubtree_size > op->team->total_ranks && 
                     gasneti_weakatomic_read(&(data->p2p->counter[0]),0)==(op->flags& GASNET_COLL_IN_ALLSYNC ? child_count:0)+2)){
        int8_t *scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        /*read memory barrier to ensure that the reading of the data doesn't preceed the flag*/
        gasneti_sync_reads();
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        /*skip the first slot of the input array since it is destined for me*/
        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
          if(tree->geom->subtree_sizes[i]==1 && direct_put_ok) {
            gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team, children[i]), args->dst, 
                                 gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes), 
                                 args->nbytes GASNETE_THREAD_PASS);
          
          } else {
            /*need to stick the data into scratch space*/
            if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
              /*out barrier will tell us when put is complete so therefore no need for local completion here*/
              gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                   (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                    gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes),
                                                    args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
            } else {
              /*need to wait for local completion*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                   (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                   gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes),
                                                   args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
              
            }
          }
          sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes;
        }
        
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* In the case of Mysync or being an intermediate node  the data is always sent to the scratch space so copy it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, scratchspace, args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      
      data->state = 4;
      
      case 4: /* wait for all puts to finish*/
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      
      data->state = 5;
      
      
      case 5:	/* Final Out Barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
        
      }
      data->state = 6;
      
      case 6: /*done*/    
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_TreePutNoCopy(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, size_t dist, int flags, 
                          gasnete_coll_implementation_t coll_params,   
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  
  if(srcimage!=0) {
    return gasnete_coll_scat_TreePut(team, dst, srcimage, src, nbytes, dist, flags, coll_params, sequence GASNETE_THREAD_PASS);
  } else {
    int options = 
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
    GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
    
    return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                           &gasnete_coll_pf_scat_TreePutNoCopy, options,
                                           gasnete_coll_tree_init(coll_params->tree_type,
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           sequence,coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
  }
}  

static int gasnete_coll_pf_scat_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op) ) break;
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNET_COLL_SCATTER_OP, op->flags);
      int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      gasnete_coll_implementation_t impl;
#if !GASNET_SEQ
      gasnet_image_t srcproc = args->srcimage;
#else
      gasnet_node_t srcproc = args->srcnode;
#endif
      size_t nbytes = args->nbytes;
      size_t sent_bytes=0;
      
      int i;
      
      impl = gasnete_coll_get_implementation();
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      for(i=0; i<num_segs - 1; i++) {
        /*ignore the handle returned*/
        handle_vec->handles[i] = gasnete_coll_scat_TreePut(op->team, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                           seg_size, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        sent_bytes += seg_size;
      }
      
      handle_vec->handles[i] = gasnete_coll_scat_TreePut(op->team, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                         args->nbytes-sent_bytes, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
      gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      gasnete_coll_free_implementation(impl);

    }
      data->state = 2;
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_scat_TreePutSeg(gasnet_team_handle_t team,
                             void *dst,
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, size_t dist, int flags, 
                             gasnete_coll_implementation_t coll_params,    
                             uint32_t sequence
                             GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));
  
  int num_segs;
  size_t seg_size;
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = coll_params->param_list[0];
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


    return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                           &gasnete_coll_pf_scat_TreePutSeg, options,
                                           gasnete_coll_tree_init(coll_params->tree_type,
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs),
                                           coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
}  

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatterM_nb() */

/* scatM Get: all nodes perform uncoordinated gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scatM_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0),
                                   gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->nbytes),
                                   args->nbytes);
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        data->private_data = gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->nbytes),
        data->handle = gasnete_geti(gasnete_synctype_nb, op->team->my_images,
                                    &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0), args->nbytes,
                                    GASNETE_COLL_REL2ACT(op->team, args->srcnode), 1, &(data->private_data),
                                    op->team->my_images * args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Get(gasnet_team_handle_t team,
                       void * const dstlist[],
                       gasnet_image_t srcimage, void *src,
                       size_t nbytes, size_t dist, int flags, 
                       gasnete_coll_implementation_t coll_params, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_scatM_Get, options,
                                          NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* scatM Put: root node performs carefully ordered puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scatM_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:
      if (op->team->myrank != args->srcnode) {
        /* Nothing to do */
      } else {
        size_t nbytes = args->nbytes;
        uintptr_t src_addr;
        int i;
        void ** srclist;
        void * const *p;
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        
        /* Allocate a source vector for puti */
        /* XXX: Use freelist? */
        srclist = gasneti_malloc(op->team->total_ranks * sizeof(void *));
        data->private_data = srclist;
        
        /* Queue PUTIs in an NBI access region */
        /* XXX: is gasnete_puti(gasnete_synctype_nbi,...) correct non-tracing variant of puti ? */
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        {
          void **q;
          
          /* Put to nodes to the "right" of ourself */
          src_addr = (uintptr_t)gasnete_coll_scale_ptr(args->src,
                                                       op->team->all_offset[op->team->myrank + 1],
                                                       nbytes);
          p = &GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, op->team->myrank + 1);
          q = &srclist[op->team->myrank + 1];
          for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
            size_t count = op->team->all_images[i];
            size_t len = count * nbytes;
            *q = (void *)src_addr;
            gasnete_puti(gasnete_synctype_nbi, GASNETE_COLL_REL2ACT(op->team, i), count, p, nbytes, 1, q, len GASNETE_THREAD_PASS);
            src_addr += len;
            p += count;
            ++q;
          }
          /* Put to nodes to the "left" of ourself */
          src_addr = (uintptr_t)gasnete_coll_scale_ptr(args->src, 0, nbytes);
          p = &GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist, 0);
          q = &srclist[0];
          for (i = 0; i < op->team->myrank; ++i) {
            size_t count = op->team->all_images[i];
            size_t len = count * nbytes;
            *q = (void *)src_addr;
            gasnete_puti(gasnete_synctype_nbi, GASNETE_COLL_REL2ACT(op->team, i), count, p, nbytes, 1, q, len GASNETE_THREAD_PASS);
            src_addr += len;
            p += count;
            ++q;
          }
        }
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* Do local copy LAST, perhaps overlapping with communication */
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, 0),
                                   gasnete_coll_scale_ptr(args->src, op->team->my_offset, nbytes),
                                   nbytes);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (op->team->myrank == args->srcnode) {
        if (data->handle != GASNET_INVALID_HANDLE) {
          break;
        }
        gasneti_free(data->private_data);	/* the temporary srclist */
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Put(gasnet_team_handle_t team,
                       void * const dstlist[],
                       gasnet_image_t srcimage, void *src,
                       size_t nbytes, size_t dist, int flags, 
                       gasnete_coll_implementation_t coll_params, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_scatM_Put, options,
                                          NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* scat Put: root node performs carefully ordered puts */
/* Valid for SINGLE and LOCAL, any size < scratch size ... since we write into the scratch space we need not worry about whether the sender knows the dest address */
static int gasnete_coll_pf_scatM_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t barrier_count;
  
  int result = 0,t=1,p=1,i,j;
  uint64_t sent_bytes=0;
  
  
  switch (data->state) {
    case 0:	/* alloc scratch */
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
      case 1: /*thread barrier*/
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)){
        break;
      }      
      data->state = 2;
      
      case 2:
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 3;
      
      case 3:
      
      if (op->team->myrank == args->srcnode) {
        gasneti_sync_reads();
        if(args->dist!=args->nbytes) {
          int8_t *myscratchpos = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          int8_t *send_arr;
          /*compress the data and send it*/
          for(i=0,p=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            send_arr = myscratchpos+p*args->nbytes*op->team->my_images;
            for(j=0; j<tree->geom->subtree_sizes[i]; j++,p++) {
              size_t src_pos = tree->geom->child_offset[i]+j+1;
              src_pos=(src_pos+args->srcnode)%op->team->total_ranks;
              for(t=0; t<op->team->my_images; t++) {
                GASNETE_FAST_UNALIGNED_MEMCPY(myscratchpos+(p*op->team->my_images+t)*args->nbytes, gasnete_coll_scale_ptr(args->src, args->dist, src_pos*op->team->my_images+t), args->nbytes);
              }
            }
            /* else if i am sending to internal node async long into scratch space*/
            if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
              gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              send_arr, 
                                              args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);              
            } else {
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                   (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                   send_arr, 
                                                   args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);              
            }
            
          }
          
        /*local scatter*/
          {
            void * const* dstlist = &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags);
            void *temp_src = gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->dist);
            for(t=0; t<op->team->my_images; t++) {
              GASNETE_FAST_UNALIGNED_MEMCPY(dstlist[t],  
                                            gasnete_coll_scale_ptr(temp_src, t, args->dist),
                                            args->nbytes);
            }
          }
        } else {
          int8_t* src_arr;
          int8_t* scratch_space= (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;

          
          sent_bytes+=args->nbytes;
          if(args->srcnode == 0) {
            /*if 0 is the source node then there's no need to shift the array around we can do it direclty from the source*/
            src_arr = args->src;
          } else {
            /*perform the rotations*/
            src_arr = scratch_space;
            gasneti_assert(tree->geom->num_rotations==1); /*for now only works w/ one level trees*/
            gasnete_coll_local_rotate_left(scratch_space, args->src, args->nbytes*op->team->my_images, op->team->total_ranks, tree->geom->rotation_points[0]);
            
          }
          /* no need to reorder the data ... send directy from the source buffer into the remote scratch/dest*/
   
          for(i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
              /* use AMLong Async since the out barrier will signal whenthe src array can be reused*/
              gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              gasnete_coll_scale_ptr(src_arr,(tree->geom->child_offset[i]+1),args->nbytes*op->team->my_images), 
                                              args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);              
            } else {
              /*there is no clear way of knowing when the scratch space or src array are safe to read so 
               we need to wait for the local completion*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                               gasnete_coll_scale_ptr(src_arr,(tree->geom->child_offset[i]+1),args->nbytes*op->team->my_images), 
                                              args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);              

            }
          }
          /* local scatter*/
          gasnete_coll_local_scatter(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->nbytes),
                                     args->nbytes);
        }
      } else if(data->p2p->state[0]){
        int8_t* scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        gasneti_sync_reads();
        /*skip the first slot of the input array since it is destined for me*/
        sent_bytes = args->nbytes*op->team->my_images;
        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
          if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, child), 
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes*op->team->my_images), 
                                            args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);
          } else {
            gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, child), 
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes*op->team->my_images), 
                                            args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);
            
          }
          
          sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes*op->team->my_images;
        }
                /* In the case of Mysync the data is always sent to the scratch space so copy it out*/
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   scratchspace,
                                   args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      
      data->state = 4;
      
      
      
      case 4:	/* Final Out Barrier */
#if USE_CONSENSUS_BARRIER
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      data->state = 5;
      
      case 5: /*done*/    
#else
      if((op->team->total_ranks > 1) &&
         (op->flags & GASNET_COLL_OUT_ALLSYNC)) {
        
          /*intermediate node*/
          /*wait for all non leaf children to report and report to the parent*/
          if(gasneti_weakatomic_read(&data->p2p->counter[1],0) < child_count) {
            break;
          }
          if(args->srcnode!=op->team->myrank) 
            gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), 1);
        
      } 
      /* Custom Out Barrrier Here */
      /* XXX: Idea Sync all the puts (acts as notify for the leaves) */
      /* XXX: Perform barrier over internal nodes */
      /* XXX: Wake leaves with an AMShort */
      

      data->state = 5;
      
      case 5: /*wait on the out barrier on the way down*/

        if((op->team->total_ranks > 1) &&
           (op->flags & GASNET_COLL_OUT_ALLSYNC)) {
          if(args->srcnode!=op->team->myrank) {
            if(gasneti_weakatomic_read(&data->p2p->counter[1],0) < (child_count + 1)) {
              break;
            }
          }
          /*signal all the children that the barrier is done*/
          for(i = 0; i < child_count; i++) {
            gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 1);
          }
        }

        data->state = 6;
      case 6: /*done*/

#endif
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePut(gasnet_team_handle_t team,
                           void * const dstlist[],
                           gasnet_image_t srcimage, void *src,
                           size_t nbytes, size_t dist, int flags, 
                           gasnete_coll_implementation_t coll_params,    
                           uint32_t sequence
                           GASNETE_THREAD_FARG)


{
  int options = 
#if USE_CONSENSUS_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
#endif
  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P;
  
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_scatM_TreePut, options,
                                          gasnete_coll_tree_init(coll_params->tree_type,
                                                                 gasnete_coll_image_node(team,srcimage), team
                                                                 GASNETE_THREAD_PASS),
                                          sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}  

static int gasnete_coll_pf_scatM_TreePutNoCopy(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t barrier_count;
  
  int result = 0,t=1,p=1,i,j;
  uint64_t sent_bytes=0;
  
  
  switch (data->state) {
    case 0:	/* alloc scratch */
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
      case 1: /*thread barrier*/
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)){
        break;
      }      
      data->state = 2;
      
      case 2:
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 3;
      
      case 3:
      
     
      if (op->team->myrank == args->srcnode) {
        int8_t* src_arr;
        
        
        gasneti_sync_reads();
        /*the other case is already handled by the code that does the copy so no need
          to redo it here*/
        gasneti_assert(args->nbytes==args->dist);
        gasneti_assert(tree->geom->num_rotations==1); /*for now only works w/ one level trees*/
        gasneti_assert(op->team->fixed_image_count==1); /*for now only works with fixed image count*/
      
        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
          if(children[i]+tree->geom->subtree_sizes[i] <= op->team->total_ranks) {
            /*can do one put since data is already contiguous*/
            int8_t *send_arr = gasnete_coll_scale_ptr(args->src,(tree->geom->child_offset[i]+1+op->team->myrank)%op->team->total_ranks,
                                                      args->nbytes*op->team->my_images);
            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              send_arr, 
                                              args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);
            } else {
               gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              send_arr, 
                                              args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);
            }
          } else {
            int8_t *send_arr = gasnete_coll_scale_ptr(args->src,(tree->geom->child_offset[i]+1+op->team->myrank),
                                                      args->nbytes*op->team->my_images);
            int8_t *send_arr2 = args->src;
            size_t first_part = op->team->total_ranks - children[i];
            size_t second_part = tree->geom->subtree_sizes[i]-first_part;
            /*we dont' need the case for a direct put here because if there is exactly oen node in the subtree then the top condition will always be true*/
            /*need to do two puts w/ the wrap*/

            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            send_arr,
                                            args->nbytes*first_part*op->team->my_images,0);
              
              gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,children[i]),
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i]+first_part*args->nbytes*op->team->my_images, 
                                            send_arr2,
                                            args->nbytes*second_part*op->team->my_images,0);
              
            } else {
              gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                                 (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                 send_arr,
                                                 args->nbytes*first_part*op->team->my_images,0);
              gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                                 (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i]+first_part*args->nbytes*op->team->my_images, 
                                                 send_arr2,
                                                 args->nbytes*second_part*op->team->my_images,0);
            }
          }
        }        
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->nbytes),
                                   args->nbytes);
      } else if(data->p2p->state[0] 
                || (op->team->myrank+tree->geom->mysubtree_size > op->team->total_ranks && 
                    gasneti_weakatomic_read(&(data->p2p->counter[0]),0)==(op->flags& GASNET_COLL_IN_ALLSYNC ? child_count:0)+2)){
        int8_t* scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        gasneti_sync_reads();
        /*skip the first slot of the input array since it is destined for me*/
      

        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
          if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, child), 
                                                 (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                 gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes*op->team->my_images), 
                                                 args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);
          } else {
            gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, child), 
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            gasnete_coll_scale_ptr(scratchspace,(tree->geom->child_offset[i]+1),args->nbytes*op->team->my_images), 
                                            args->nbytes*tree->geom->subtree_sizes[i]*op->team->my_images, 0, 1);
            
          }
          
 
        }
        /* In the case of Mysync the data is always sent to the scratch space so copy it out*/
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   scratchspace,
                                   args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      
      data->state = 4;
      
      
      
  case 4:	/* Final Out Barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    data->state = 5;
    
  case 5: /*done*/    
    gasnete_coll_free_scratch(op);
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePutNoCopy(gasnet_team_handle_t team,
                                 void * const dstlist[],
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, size_t dist, int flags, 
                                 gasnete_coll_implementation_t coll_params,    
                                 uint32_t sequence
                                 GASNETE_THREAD_FARG)
     
     
{
  int options = 
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
    GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P;
 
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_scatM_TreePutNoCopy, options,
                                          gasnete_coll_tree_init(coll_params->tree_type,
                                                                 gasnete_coll_image_node(team,srcimage), team
                                                                 GASNETE_THREAD_PASS),
                                          sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}  

static int gasnete_coll_pf_scatM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op) ) break;
    {
      
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNET_COLL_SCATTER_OP, op->flags);
      int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      gasnete_coll_implementation_t impl;
#if !GASNET_SEQ
      gasnet_image_t srcproc = args->srcimage;
#else
      gasnet_node_t srcproc = args->srcnode;
#endif
      
      void ** dstlist;
      size_t nbytes = args->nbytes;
      size_t sent_bytes=0;
      gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? op->team->my_images : op->team->total_images);
      int i;
      
      impl = gasnete_coll_get_implementation();
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
      dstlist = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));
      
      for(i=0; i<num_segs - 1; i++) {
        /*ignore the handle returned*/
        gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_scatM_TreePut(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                            seg_size, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        sent_bytes += seg_size;
      }
      gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
      handle_vec->handles[i] = gasnete_coll_scatM_TreePut(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                          args->nbytes-sent_bytes, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
      gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      gasnete_coll_free_implementation(impl);

    }
            data->state = 2;
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePutSeg(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, size_t dist, int flags, 
                              gasnete_coll_implementation_t coll_params, 
                              uint32_t sequence
                              GASNETE_THREAD_FARG) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));

  size_t seg_size; 
  int num_segs;
  gasneti_assert(coll_params->num_params > 0);
  seg_size =  coll_params->param_list[0];
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_scatM_TreePutSeg, options,
                                          gasnete_coll_tree_init(coll_params->tree_type,
                                                                 gasnete_coll_image_node(team,srcimage), team
                                                                 GASNETE_THREAD_PASS),
                                          (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list
                                          GASNETE_THREAD_PASS);
  
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath Get: root node performs carefully ordered gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gath_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank != args->dstnode) {
        /* Nothing to do */
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        /* Queue GETs in an NBI access region */
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        {
          int i;
          uintptr_t p;
          
          /* Get from nodes to the "right" of ourself */
          p = (uintptr_t)gasnete_coll_scale_ptr(args->dst, (op->team->myrank + 1), args->nbytes);
          for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i, p += args->nbytes) {
            gasnete_get_nbi_bulk((void *)p, GASNETE_COLL_REL2ACT(op->team,i), args->src, args->nbytes GASNETE_THREAD_PASS);
          }
          /* Get from nodes to the "left" of ourself */
          p = (uintptr_t)gasnete_coll_scale_ptr(args->dst, 0, args->nbytes);
          for (i = 0; i < op->team->myrank; ++i, p += args->nbytes) {
            gasnete_get_nbi_bulk((void *)p, GASNETE_COLL_REL2ACT(op->team,i), args->src, args->nbytes GASNETE_THREAD_PASS);
          }
        }
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* Do local copy LAST, perhaps overlapping with communication */
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
                                            args->src, args->nbytes);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(Get)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
                                        &gasnete_coll_pf_gath_Get, options,
                                        NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* gath Put: all nodes perform uncoordinated puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gath_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank == args->dstnode) {
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
                                            args->src, args->nbytes);
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        data->handle = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team, args->dstnode), 
                                           gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
                                           args->src, args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(Put)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
                                        &gasnete_coll_pf_gath_Put, options,
                                        NULL, sequence,coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
/* gath Put: all nodes perform uncoordinated puts */
/* Valid for SINGLE and LOCAL, any size < scratch size ... remote threads will not touch user buffers*/
/* XXX Note that an optimization can be made here where the data can be put into the user buffer at the root*/
/* XXX However this will make this implementation only valid for SINGLE and not LOCAL*/

static int gasnete_coll_pf_gath_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t expected_count;
  gasnet_node_t child;
  int result = 0;
  int i=0;
  uint8_t direct_put_ok = !(/*can't perform direct put if any of hte following are true*/
                            (args->dstnode!=0) || 
                            (op->flags & GASNET_COLL_IN_MYSYNC) || 
                            (op->flags & GASNET_COLL_OUT_MYSYNC) || 
                            (op->flags & GASNET_COLL_LOCAL) || 
                            (args->nbytes!=args->dist));
  
  switch (data->state) {
    case 0:
      if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))    break;
      data->state = 1;
      
    case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }

      data->state = 2;
      
    case 2:	/* Local Data Movement */
      /* go up the tree with the data */
      /* copy my data into the start of the scratch space */
      
      if(child_count > 0) {
        if(direct_put_ok && (op->team->myrank == args->dstnode)) {
           GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst,op->team->myrank,args->nbytes), 
                                        (int8_t*)args->src, args->nbytes);
        } else{
          GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        (int8_t*)args->src, args->nbytes);
          
        }
        data->state = 3;
      } else if_pf(op->team->total_ranks == 1) {
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, 
                                      args->src, args->nbytes);
        data->state = 4;
        break;
      }
      
      
    case 3:
      if(op->team->myrank == args->dstnode) {        
        if(!direct_put_ok) {
          int8_t* scratchspace =  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < child_count) {
            /* all children have not yet reported*/ 
            break;
          }
          gasneti_sync_reads();
          /*all children have reported so now need to reshuffle the data into the correct place*/
          if(args->nbytes == args->dist) {
            gasneti_assert(tree->geom->num_rotations==1);
            gasnete_coll_local_rotate_right(args->dst,scratchspace, 
                                            args->nbytes, op->team->total_ranks, tree->geom->rotation_points[0]);
          } else { 
            for(i=0; i<op->team->total_ranks; i++) {
              GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, (i+tree->geom->rotation_points[0])%op->team->total_ranks, args->dist),
                                            gasnete_coll_scale_ptr(scratchspace, i,args->nbytes),
                                            args->nbytes);
            } 
          } 
        } else {
          /*however if it is an out all sync we still need to wait for all the children to arrive before we broadcast the down barrier*/
          if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
            if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < child_count) {
              /* all children have not yet reported*/ 
              break;
            }
          /*data is in the right place already no need to do anything*/          
          }
        }
      } else {
     
        int8_t* src_addr;
        if(child_count > 0) {
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < child_count) {
            /* all children have not yet reported*/ 
            break;
          }
          src_addr  = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        } else {
          src_addr = (int8_t*) args->src;
        }
        /*all children have reported*/
        gasneti_sync_reads();
        if(parent == args->dstnode && direct_put_ok) {
          /*data can be put right where it needs to go 
            in this case the parent is not waiting for the data to arrive in the mainline code
           thus the children have to wait for the puts to complete
            */
          
          if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
            /*parent is waiting for the put to complete so signal*/
            /*since the parent is going to signal us on the down barrier we don't need to wait for local completion*/
            gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team,parent),
                                               gasnete_coll_scale_ptr(args->dst,(tree->geom->sibling_offset+1),args->nbytes),
                                               src_addr,
                                               args->nbytes*tree->geom->mysubtree_size,0);
          } else {
            /*parent is not waiting for the collective to finish so go ahead and just do a nonblocking put*/
            data->handle = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team, parent), gasnete_coll_scale_ptr(args->dst,(tree->geom->sibling_offset+1),args->nbytes),
                                               src_addr, args->nbytes*tree->geom->mysubtree_size GASNETE_THREAD_PASS);
            gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
          }
        } else {
          /*data is going into the scratch space*/
          if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
            /*since the parent will signal us anyway with a barrier when the data is complete we can safely send w/o local completion*/
            gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                   (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset+1)*args->nbytes,
                                                   src_addr,
                                                   args->nbytes*tree->geom->mysubtree_size,0);
          } else  {
            /*in the case of an out NOSYNC the parent will not be waiting to signal so the first level of children will be the last in the tree to leave*/
            /*in the case of an out mysync there will be no signal when the scratch space nor the src data is safe to reuse so we must wait*/
            /*in either case we need local completion semantics*/
            gasnete_coll_p2p_counting_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                   (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset+1)*args->nbytes,
                                                   src_addr,
                                                   args->nbytes*tree->geom->mysubtree_size,0);
          }
        }
      }
      data->state = 4;
  case 4: /*sync data movement*/
    if(data->handle !=GASNET_INVALID_HANDLE) {
      break;
    }
    data->state = 5;
  case 5: /*node level barrier*/
    
    /* go down the tree with the barrier again*/
    if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
      if(op->team->myrank!=args->dstnode) {
        /*wait for clear signal from parent*/
        expected_count = child_count + 1;
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) < expected_count) {
          break;
        }
      }
      /*send clear signal to all the other nodes*/
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]),0);
      }
    }
    data->state = 6;
    
  case 6: /* done*/
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      gasnete_coll_free_scratch(op);
  }
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreePut) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
                                        &gasnete_coll_pf_gath_TreePut, options,
                                        gasnete_coll_tree_init(coll_params->tree_type, 
                                                               gasnete_coll_image_node(team,dstimage), team
                                                               GASNETE_THREAD_PASS), sequence, 
                                        coll_params->num_params, coll_params->param_list  GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gath_TreePutNoCopy(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t expected_count;
  gasnet_node_t child;
  int result = 0;
  int i=0;
  uint8_t direct_put_ok = !(/*can't do direct put if any of the following are true*/
                            (op->flags & GASNET_COLL_IN_MYSYNC) || 
                            (op->flags & GASNET_COLL_OUT_MYSYNC) || 
                            (op->flags & GASNET_COLL_LOCAL) || 
                            (args->nbytes!=args->dist)
                            ); 
  

  
  switch (data->state) {
  case 0:
    if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))    break;
    data->state = 1;
    
  case 1:	/* Optional IN barrier */
    if (!gasnete_coll_generic_all_threads(data)||
        !gasnete_coll_generic_insync(op->team, data)) {
      break;
    }
    
    data->state = 2;
    
  case 2: /* no need for local datamovement since all data movement is done through the network*/
    /*since i am here, my data is ready to be shipped up the tree*/
    if(op->team->myrank!=args->dstnode) {
      
      if(parent == args->dstnode && direct_put_ok) {
        /*data can be put right where it needs to go 
          in this case the parent is not waiting for the data to arrive in the mainline code
          thus the children have to wait for the puts to complete
        */

        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          /*parent is waiting for the put to complete so signal*/
          /*since the parent is going to signal us on the down barrier we don't need to wait for local completion*/
          
          gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                             gasnete_coll_scale_ptr(args->dst,(tree->geom->sibling_offset+1)-args->dstnode,args->nbytes),
                                             args->src,
                                             args->nbytes,0);
        } else {
          /*parent is not waiting for the collective to finish so go ahead and just do a nonblocking put*/
          data->handle = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team, parent), gasnete_coll_scale_ptr(args->dst,(tree->geom->sibling_offset+1),args->nbytes),
                                             args->src,
                                             args->nbytes GASNETE_THREAD_PASS);
          gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        }
      } else {

        /*data is going into the scratch space*/
        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          /*since the parent will signal us anyway with a barrier when the data is complete we can safely send w/o local completion*/
          gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                             (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset)*args->nbytes,
                                             args->src,
                                             args->nbytes,0);
        } else  {
          /*in the case of an out NOSYNC the parent will not be waiting to signal so the first level of children will be the last in the tree to leave*/
          /*in the case of an out mysync there will be no signal when the scratch space nor the src data is safe to reuse so we must wait*/
          /*in either case we need local completion semantics*/
          gasnete_coll_p2p_counting_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                 (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset)*args->nbytes,
                                                 args->src,
                                                 args->nbytes,0);
        }
      }
     
    } else {
      GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst,op->team->myrank,args->nbytes), 
                                    (int8_t*)args->src, args->nbytes);
    }
    data->state = 3;
    
  case 3:
    if(op->team->myrank == args->dstnode) {        
      gasneti_assert(tree->geom->num_rotations==1);
      if(!direct_put_ok) {
        int8_t* scratchspace =  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        /*leaf children will singal once while non-leaf children will signal twice times*/
        if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < (tree->geom->num_leaf_children + tree->geom->num_non_leaf_children*2)) {
          /* all children have not yet reported*/ 
          break;
        }
        gasneti_sync_reads();
        /*all children have reported so now need to reshuffle the data into the correct place*/

#if 0
        if(args->nbytes == args->dist) {
          gasneti_assert(tree->geom->num_rotations==1);
          gasnete_coll_local_rotate_right(args->dst,scratchspace, 
                                          args->nbytes, op->team->total_ranks, tree->geom->rotation_points[0]);
        } else { 
          for(i=0; i<op->team->total_ranks; i++) {
            GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, (i+tree->geom->rotation_points[0])%op->team->total_ranks, args->dist),
                                          gasnete_coll_scale_ptr(scratchspace, i,args->nbytes),
                                          args->nbytes);
          } 
        } 
#else
        GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst,1,args->nbytes),
                                      scratchspace,
                                      args->nbytes*(op->team->total_ranks-1));

#endif
      } else {
        gasneti_assert(tree->geom->num_rotations ==1);
        /*however if it is an out all sync we still need to wait for all the children to arrive before we broadcast the down barrier*/
        /*the subtree that contians the node wrap around will signal twice*/
        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < (tree->geom->num_leaf_children + tree->geom->num_non_leaf_children*2)) {
            /* all children have not yet reported*/ 
            break;
          }
          /*data is in the right place already no need to do anything*/          
        }
      }
    } else if(child_count>0){
      
      int8_t* src_addr;
      
      if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < (tree->geom->num_leaf_children+tree->geom->num_non_leaf_children*2)) {
        /* all children have not yet reported*/ 
        break;
      }
      gasneti_sync_reads();
      src_addr  = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
      /*all children have reported*/
      
      if(parent == args->dstnode && direct_put_ok) {

        /*data can be put right where it needs to go 
          in this case the parent is not waiting for the data to arrive in the mainline code
          thus the children have to wait for the puts to complete
        */
        
        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          /*parent is waiting for the put to complete so signal*/
          /*since the parent is going to signal us on the down barrier we don't need to wait for local completion*/

          gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                             gasnete_coll_scale_ptr(args->dst,(2+tree->geom->sibling_offset)-args->dstnode,args->nbytes),
                                             src_addr,
                                             args->nbytes*(tree->geom->mysubtree_size-1),0);
        } else {
          /*parent is not waiting for the collective to finish so go ahead and just do a nonblocking put*/
          data->handle2 = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team, parent), gasnete_coll_scale_ptr(args->dst,(2+tree->geom->sibling_offset),args->nbytes),
                                             src_addr,
                                              args->nbytes*(tree->geom->mysubtree_size-1) GASNETE_THREAD_PASS);
          gasnete_coll_save_handle(&data->handle2 GASNETE_THREAD_PASS);
        }
      } else {

        /*data is going into the scratch space*/
        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          /*since the parent will signal us anyway with a barrier when the data is complete we can safely send w/o local completion*/
          gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team,parent),
                                             (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(1+tree->geom->sibling_offset)*args->nbytes,
                                             src_addr,
                                             args->nbytes*(tree->geom->mysubtree_size-1),0);
        } else  {
          /*in the case of an out NOSYNC the parent will not be waiting to signal so the first level of children will be the last in the tree to leave*/
          /*in the case of an out mysync there will be no signal when the scratch space nor the src data is safe to reuse so we must wait*/
          /*in either case we need local completion semantics*/
          gasnete_coll_p2p_counting_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                 (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(1+tree->geom->sibling_offset)*args->nbytes,
                                                 src_addr,
                                                 args->nbytes*(tree->geom->mysubtree_size-1),0);
        }
      }
    } else {
      /*no children data movement already hadled in state 2*/
    }
    
    data->state = 4;
  case 4: /*sync data movement*/
    /*since we have initiated possibly more than one put we need to sync all of them*/
    if(data->handle !=GASNET_INVALID_HANDLE || data->handle2 !=GASNET_INVALID_HANDLE) {
      break;
    }
    
    data->state = 5;
  case 5: /*node level barrier*/
    
    /* go down the tree with the barrier again*/
    if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
      if(op->team->myrank!=args->dstnode) {
        /*wait for clear signal from parent*/
         if (gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) == 0) {
          break;
        }
      }
      /*send clear signal to all the other nodes*/
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]),1);
      }
    }
    data->state = 6;
    
  case 6: /* done*/
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    gasnete_coll_free_scratch(op);
  }
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreePutNoCopy)
{

  if(dstimage != 0) {
    /*tree put copy as written doesn't work when the root is not 0*/
    return gasnete_coll_gath_TreePut(team,
                                     dstimage, dst,
                                     src,
                                     nbytes, dist, flags, 
                                     coll_params,
                                     sequence
                                     GASNETE_THREAD_PASS);
  } else {
    int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
    GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
    return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_gath_TreePutNoCopy, options,
                                          gasnete_coll_tree_init(coll_params->tree_type, 
                                                                 gasnete_coll_image_node(team,dstimage), team
                                                                 GASNETE_THREAD_PASS), sequence, coll_params->num_params, coll_params->param_list
                                          GASNETE_THREAD_PASS);
  }
}

static int gasnete_coll_pf_gath_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNET_COLL_GATHER_OP, op->flags);
      int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
#if !GASNET_SEQ
      gasnet_image_t dstproc = args->dstimage;
#else
      gasnet_node_t dstproc = args->dstnode;
#endif
      size_t nbytes = args->nbytes;
      size_t sent_bytes=0;
      
      int i;
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
      
      for(i=0; i<num_segs - 1; i++) {
        /*ignore the handle returned*/
        handle_vec->handles[i] = gasnete_coll_gath_TreePut(op->team, GASNETE_COLL_REL2ACT(op->team, dstproc), gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , 
                                                           gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                           seg_size, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        sent_bytes += seg_size;
      }
      
      handle_vec->handles[i] = gasnete_coll_gath_TreePut(op->team, GASNETE_COLL_REL2ACT(op->team, dstproc), gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , 
                                                         gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                         args->nbytes-sent_bytes, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
      gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      gasnete_coll_free_implementation(impl);
    }

      data->state = 2;
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreePutSeg) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));

  size_t seg_size; 
  int num_segs;
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  num_segs = (nbytes + seg_size - 1)/seg_size;
  
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);
  


  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
                                        &gasnete_coll_pf_gath_TreePutSeg, options,
                                        gasnete_coll_tree_init(coll_params->tree_type, 
                                                               gasnete_coll_image_node(team,dstimage), team
                                                               GASNETE_THREAD_PASS), 
                                        (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), 
                                        coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}  


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gatherM_nb() */

/* gathM Get: root node performs carefully ordered gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gathM_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank != args->dstnode) {
        /* Nothing to do */
      } else {
        size_t nbytes = args->nbytes;
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        
        /* Queue GETIs in an NBI access region */
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
        {
          void **q;
          uintptr_t dst_addr;
          int i;
          void * const *p;
          void ** dstlist = gasneti_malloc(op->team->total_ranks * sizeof(void *));
          data->private_data = dstlist;
          
          /* Get from the "right" of ourself */
          dst_addr = (uintptr_t)gasnete_coll_scale_ptr(args->dst,
                                                       op->team->all_offset[op->team->myrank + 1],
                                                       nbytes);
          p = &GASNETE_COLL_1ST_IMAGE(op->team,args->srclist, op->team->myrank + 1);
          q = &dstlist[op->team->myrank + 1];
          for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
            size_t count = op->team->all_images[i];
            size_t len = count * nbytes;
            *q = (void *)dst_addr;
            gasnete_geti(gasnete_synctype_nbi, 1, q, len, GASNETE_COLL_REL2ACT(op->team, i), count, p, nbytes GASNETE_THREAD_PASS);
            dst_addr += len;
            p += count;
            ++q;
          }
          /* Get from nodes to the "left" of ourself */
          dst_addr = (uintptr_t)args->dst;
          dst_addr = (uintptr_t)gasnete_coll_scale_ptr(args->dst, 0, nbytes);
          p = &GASNETE_COLL_1ST_IMAGE(op->team,args->srclist, 0);
          q = &dstlist[0];
          for (i = 0; i < op->team->myrank; ++i) {
            size_t count = op->team->all_images[i];
            size_t len = count * nbytes;
            *q = (void *)dst_addr;
            gasnete_geti(gasnete_synctype_nbi, 1, q, len, GASNETE_COLL_REL2ACT(op->team, i), count, p, nbytes GASNETE_THREAD_PASS);
            dst_addr += len;
            p += count;
            ++q;
          }
        }
        data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        
        /* Do local copy LAST, perhaps overlapping with communication */
        gasnete_coll_local_gather(op->team->my_images,
                                  gasnete_coll_scale_ptr(args->dst, op->team->my_offset, nbytes),
                                  &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, 0), nbytes);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (op->team->myrank == args->dstnode) {
        if (data->handle != GASNET_INVALID_HANDLE) {
          break;
        }
        gasneti_free(data->private_data);	/* the temporary dstlist */
      }
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHERM_ALG(Get) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
                                         &gasnete_coll_pf_gathM_Get, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* gathM Put: all nodes perform uncoordinated puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gathM_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;
  
  gasneti_assert(op->flags & GASNET_COLL_SINGLE);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (op->team->myrank == args->dstnode) {
        gasnete_coll_local_gather(op->team->my_images,
                                  gasnete_coll_scale_ptr(args->dst, op->team->my_offset, args->nbytes),
                                  &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, 0), args->nbytes);
      } else {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        data->private_data = gasnete_coll_scale_ptr(args->dst, op->team->my_offset, args->nbytes);
        /* use a vis operation to stride through all the local data*/
        data->handle = gasnete_puti(gasnete_synctype_nb, GASNETE_COLL_REL2ACT(op->team, args->dstnode),
                                    1, &(data->private_data), op->team->my_images * args->nbytes,
                                    op->team->my_images, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, 0),
                                    args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;
      
      case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
        break;
      }
      
      data->state = 3;
      
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHERM_ALG(Put) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
                                         &gasnete_coll_pf_gathM_Put, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gathM_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t expected_count;
  gasnet_node_t child;
  int result = 0;
  int i=0,j=0;
  /*  uint8_t direct_put_ok =(tree->geom->seq_dfs_order==1 && args->nbytes==args->dist && (op->flags & GASNET_COLL_SINGLE) && (op->flags & GASNET_COLL_OUT_NOSYNC) );*/
  uint8_t direct_put_ok =0; /*XXX: temporarily disable direct puts to destination until we fix the bug in the scratch space regarding multiple clears*/
  
  switch (data->state) {
    case 0:
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
      
      case 1:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS)||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 2;
      
      case 2:	/* Local Data Movement */
      /* perform a local gather into the scratch space*/
      gasnete_coll_local_gather(op->team->my_images,
                                (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
      data->state = 3;
      
      case 3:
      if (gasneti_weakatomic_read(&(data->p2p->counter[0]), GASNETI_ATOMIC_RMB_POST) < child_count) {
        break;
      }
      /* forward the data up to my parent if i am not the root node*/
      if(op->team->myrank != args->dstnode) {
        gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,parent),
                                      (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset+1)*args->nbytes*op->team->my_images,
                                      (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                      args->nbytes*tree->geom->mysubtree_size*op->team->my_images,0);
      } else if(args->nbytes!=args->dist){    /* i am root... need to reorder*/    
        gasnet_image_t t;
        int8_t* scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        for(i=0; i<op->team->total_ranks; i++) { 
          gasnet_node_t src_node = (i+tree->geom->rotation_points[0])%op->team->total_ranks;
          for(t=0; t<op->team->all_images[i]; t++) {
            GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, (src_node*op->team->my_images+t), args->dist),
                                          gasnete_coll_scale_ptr(scratchspace,(i*op->team->my_images+t),args->nbytes),
                                          args->nbytes);
          }
        }
      } else { 
        int8_t* scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        gasnete_coll_local_rotate_right(args->dst,scratchspace, 
                                        args->nbytes*op->team->my_images, op->team->total_ranks, tree->geom->rotation_points[0]);
        
      }
      data->state = 4;
      
      case 4: /*node level barrier*/
      /* go down the tree with the barrier again*/
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        /* Sync data movement */
        gasneti_sync_reads();
        expected_count = child_count + ((op->team->myrank!=args->dstnode) ? 1 : 0);
        if(op->team->myrank != args->dstnode) {
          if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) < expected_count) {
            break;
          }
        }
        for(child=0; child<child_count; child++) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team,children[child]),0);
        }
      }
      data->state = 5;
      
      case 5: /* done*/
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
      
  }
  return result;
  
}

GASNETE_COLL_DECLARE_GATHERM_ALG(TreePut) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH; 
  
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
                                         &gasnete_coll_pf_gathM_TreePut, options,
                                         gasnete_coll_tree_init(coll_params->tree_type, 
                                                                gasnete_coll_image_node(team,dstimage), team
                                                                GASNETE_THREAD_PASS), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
  
  
}

static int gasnete_coll_pf_gathM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op) ) break;
    {
      
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNET_COLL_SCATTER_OP, op->flags);
      int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
#if !GASNET_SEQ
      gasnet_image_t dstproc = args->dstimage;
#else
      gasnet_node_t dstproc = args->dstnode;
#endif
      
      void ** srclist;
      size_t nbytes = args->nbytes;
      size_t sent_bytes=0;
      gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? op->team->my_images : op->team->total_images);
      int i;
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
      srclist = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));
      
      for(i=0; i<num_segs - 1; i++) {
        /*ignore the handle returned*/
        gasnete_coll_scale_ptrM(srclist, args->srclist, sent_bytes, 1, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_gathM_TreePut(op->team, dstproc, gasnete_coll_scale_ptr(args->dst,1,sent_bytes), srclist,
                                                            seg_size, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        sent_bytes += seg_size;
      }
      gasnete_coll_scale_ptrM(srclist, args->srclist, sent_bytes, 1, numaddrs); 
      handle_vec->handles[i] = gasnete_coll_gathM_TreePut(op->team, dstproc, gasnete_coll_scale_ptr(args->dst,1,sent_bytes), srclist, 
                                                          args->nbytes-sent_bytes, args->nbytes, flags, impl, op->sequence+i+1 GASNETE_THREAD_PASS);   
      gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2;
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


GASNETE_COLL_DECLARE_GATHERM_ALG(TreePutSeg) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));

  
  size_t seg_size;
  int num_segs;
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = coll_params->param_list[0];
  num_segs = (nbytes +seg_size - 1)/seg_size;
  
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
                                         &gasnete_coll_pf_gathM_TreePutSeg, options,
                                         gasnete_coll_tree_init(coll_params->tree_type,
                                                                gasnete_coll_image_node(team,dstimage), team
                                                                GASNETE_THREAD_PASS),
                                         (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
  
  
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/* gall Gath: Implement as a tree-gather followed by a broadcast */
/* THIS WILL ONLY WORK WITH ALIGNED SEGMENTS AND SINGLE*/
/* (i.e. the virtual address of the dest is the same on all the nodes)*/
#if 0
#define GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT 0
static int gasnete_coll_pf_gall_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  const gasnet_node_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t expected_count;
  gasnet_node_t child;
  int result = 0;
  int i=0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) || 
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      /* perform a Gather up a tree rooted at 0*/
      case 1:
#if 0
      /* go down the tree with the barrier*/
      if(!(op->flags & GASNET_COLL_IN_NOSYNC)) {
        if(op->team->myrank !=  GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT ) {
          if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < 1) {
            break;
          }
        }
        for(child=0; child<child_count; child++) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]));
        }
      }
#endif
      data->state = 2;
      
      
      
      case 2:	/* Initiate data movement */
      /* copy my data into the start of the scratch space */
      if(child_count > 0) {
        if(op->team->myrank != GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT || tree->geom->seq_dfs_order) {
          GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)args->dst, 
                                        (int8_t*)args->src, args->nbytes);
        } else {
          GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        (int8_t*)args->src, args->nbytes);
        }
      }
      data->state=3;
      
      case 3:
      expected_count = child_count;
      
      /* wait for all my children to send data to me*/
      if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != expected_count) {
        break;
      }
      gasneti_sync_reads();
      /* forward the data up to my parent if i am not the root node*/
      if(op->team->myrank != GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT) {
        int8_t *src = (child_count > 0 ? 
                       args->dst : args->src);
        int8_t* dest = (parent == GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT && !(tree->geom->seq_dfs_order) ?
                        (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0] : (int8_t*)args->dst);
        gasnete_coll_p2p_counting_put(op,  GASNETE_COLL_REL2ACT(op->team, parent), dest+(tree->geom->sibling_offset+1)*args->nbytes, src,
                                      args->nbytes*tree->geom->mysubtree_size);
      } else {        
        /* Sync data movement */
        /* XXX: THIS IS A BIG BOTTLENECK*/
        /* XXX: If the tree's DFS order is the sequential order of the nodes, this rearrrangment can be avoided*/
        /* XXX: However we leave it in here so that trees don't neccissarily have to have this condition for this algorithim to be correct*/
        
        if(!(tree->geom->seq_dfs_order)) {
          for(i=0; i<op->team->total_ranks; i++) {
            /*used for temporary variables to aid GDB*/
            int8_t *curr_dest = args->dst;
            int8_t *curr_src = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
            GASNETE_FAST_UNALIGNED_MEMCPY(curr_dest+args->nbytes*tree->geom->dfs_order[i],
                                          curr_src+args->nbytes*i,
                                          args->nbytes);
            
          }
        }
      }
      data->state = 4;
      
      /* broadcast the data back down the tree*/
      case 4:
      if (op->team->myrank == GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT) {
        for (child = 0; child < child_count; child++) {
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, 
                                          args->dst, args->nbytes*op->team->total_ranks, 0, 1);
        }
      } else if (data->p2p->state[0]) {
        
        gasneti_sync_reads();
        for (child = 0; child < child_count; child++) {
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, 
                                          args->dst, args->nbytes*op->team->total_ranks, 0, 1);
        }
      } else {
        break;	/* Waiting for parent to push data and signal */
      }
      data->state = 5;
      
      
      case 5:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      /*free up the scratch space used by this op*/
      gasnete_coll_free_scratch(op);
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gall_TreePut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, gasnete_coll_tree_type_t tree_type, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1) | GASNETE_COLL_USE_SCRATCH_TREE;

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_TreePut, options,
                                            gasnete_coll_tree_init(tree_type, 
                                                                   gasnete_coll_image_node(team,GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT), team
                                                                   GASNETE_THREAD_PASS), 
                                            sequence GASNETE_THREAD_PASS);
}
#undef GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT


/* This is a ring based all-gather*/
/* every processor sends their piece to the right and receives from the left. In (P-1) steps all processors will have all the data 
 Here the state will be used to describe which processor to recieve from.
 This algorithm is naturally IN_NOSYNC/OUT_NOSYNC and will only work for SINGLE/single-addr. However, 
 the algorithm can easily be modified to work for OUT_MYSYNC without using a full barrier. 
 */
#define GASNETE_COLL_MYDECMOD(A,DIFF) (((A)-(DIFF)) + ((A)<(DIFF) ? op->team->total_ranks : 0))
static int gasnete_coll_pf_gall_RingPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int slot;
  gasnet_node_t neighbor = (op->team->myrank == (op->team->total_ranks-1) ? 0 : op->team->myrank+1);
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    data->state++;
  }   
  else if(data->state == op->team->total_ranks) {
    /* If using MYSYNC Wait for the last put to finish*/
    if(op->flags & GASNET_COLL_OUT_MYSYNC) {
      if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < (op->team->total_ranks-1)) {
        return 0;
      }
    }
    data->state++;
  } 
  /* State Nodes+1: Out Barrier (if needed)*/
  else if(data->state == op->team->total_ranks+1) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    gasneti_free(data->private_data);
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);    
  }
  /* State [1,NODES) perform ring*/
  else if(data->state < op->team->total_ranks) {
    /* Copy the data from source to dest*/
    if(data->state == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes), 
                                    (int8_t*)args->src, args->nbytes);
    }     /*Wait for the P2P counter to be at least state*/
    else if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < (data->state-1)) {
      return 0;
    }
    /*Send to my neighbor from slot (MYNODE+(state-1))%NODES*/
    /* All puts except for the last one can be asynccounting put*/
    gasneti_sync_reads();
    
    if(data->state < (op->team->total_ranks-1)) {      
      gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, neighbor),
                                         gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(op->team->myrank,(data->state-1)), args->nbytes),
                                         gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(op->team->myrank,(data->state-1)), args->nbytes),
                                         args->nbytes); 
    } else {
      gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team, neighbor),
                                    gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(op->team->myrank,(data->state-1)), args->nbytes),
                                    gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(op->team->myrank,(data->state-1)), args->nbytes),
                                    args->nbytes); 
      
    }
    data->state++;
    
  }
  return result;
}
#undef GASNETE_COLL_MYDECMOD


extern gasnet_coll_handle_t
gasnete_coll_gall_RingPut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, 
                          gasnete_coll_implementation_t coll_params,
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1);

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_RingPut, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
#endif



/*only works for COLL_SINGLE and DST IN SEGMENT*/
static int gasnete_coll_pf_gall_FlatPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    data->state++;
  }
  
  if(data->state == 1) {
    gasnet_node_t dst;
    if (!GASNETE_COLL_MAY_INIT_FOR(op)) return result;
    
    if_pt(op->team->total_ranks > 1) {
      gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
      {
        for(dst=op->team->myrank+1; dst<op->team->total_ranks; dst++) {
          /* send to threads above me*/
          gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,dst), (int8_t*) args->dst + op->team->myrank*args->nbytes, args->src, args->nbytes GASNETE_THREAD_PASS);
        }
        for(dst=0; dst<op->team->myrank; dst++) {
          /*send to threads below me*/
          gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,dst), (int8_t*) args->dst + op->team->myrank*args->nbytes, args->src, args->nbytes GASNETE_THREAD_PASS);
        }
      }
      data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
      gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
    }
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*) args->dst + op->team->myrank*args->nbytes, 
                                        args->src, args->nbytes);
    
    data->state++;
  }
  
  if(data->state == 2) {
    /* sync all the handles for the puts*/
    if (op->team->total_ranks > 1 && data->handle != GASNET_INVALID_HANDLE) {
      return 0;
    }
    data->state++;
  }
  
  if(data->state == 3) {
    /* out barrier and cleanup*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gall_FlatPut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, 
                          gasnete_coll_implementation_t coll_params,
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_FlatPut, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*GASNET_COLL_SINGLE and GASNET_SRC_IN_SEG*/
static int gasnete_coll_pf_gall_FlatGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    data->state++;
  }
  
  if(data->state == 1) {
    gasnet_node_t srcnode;
    if (!GASNETE_COLL_MAY_INIT_FOR(op)) return result;
    
    if_pt(op->team->total_ranks > 1) {
      gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
      {
        for(srcnode=op->team->myrank+1; srcnode<op->team->total_ranks; srcnode++) {
          /* get from threads above me*/
          gasnete_get_nbi_bulk(gasnete_coll_scale_ptr(args->dst, srcnode, args->nbytes), 
                               GASNETE_COLL_REL2ACT(op->team,srcnode), args->src, 
                               args->nbytes GASNETE_THREAD_PASS);
        }
        for(srcnode=0; srcnode<op->team->myrank; srcnode++) {
          /*get threads below me*/
          gasnete_get_nbi_bulk(gasnete_coll_scale_ptr(args->dst, srcnode, args->nbytes), 
                               GASNETE_COLL_REL2ACT(op->team,srcnode), args->src, 
                               args->nbytes GASNETE_THREAD_PASS);
        }
      }
      data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
      gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
    }
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*) args->dst + op->team->myrank*args->nbytes, 
                                        args->src, args->nbytes);
    
    data->state++;
  }
  
  if(data->state == 2) {
    /* sync all the handles for the gets*/
    if (op->team->total_ranks > 1 && data->handle != GASNET_INVALID_HANDLE) {
      return 0;
    }
    data->state++;
  }
  
  if(data->state == 3) {
    /* out barrier and cleanup*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gall_FlatGet(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, 
                          gasnete_coll_implementation_t coll_params,
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_FlatGet, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


static int gasnete_coll_pf_gall_Dissem(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if(op->team->total_ranks>1) 
      if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS)) 
        return 0;
    
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    data->state++;
  } 
  if(data->state == 1) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    if(op->team->total_ranks == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      data->state = dissem->dissemination_phases*2+2;
    } else {
      /*memcpy the data into the start of the scratch space*/
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                    args->src, args->nbytes);
      data->state++;
    }
  }
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                      (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+curr_len,
                                      (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                      curr_len, phase, 1);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(data->p2p->state[phase] == 1) {
        data->state++;
      } else {
        return 0;
      }
    }
  }
  
  if(data->state == (dissem->dissemination_phases)*2) {
    uint32_t phase = (data->state-2)/2;
    size_t nblk = op->team->total_ranks - (1<<phase); 
    size_t curr_len = args->nbytes*(nblk);
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                    (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+(1<<phase)*args->nbytes,
                                    (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                    curr_len, phase, 1);
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1) {
    uint32_t phase = (data->state-2)/2;
    if(data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
    
    /*rotate the data around*/
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*)args->dst+args->nbytes*op->team->myrank, 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        args->nbytes*(op->team->total_ranks-op->team->myrank));
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+args->nbytes*(op->team->total_ranks-op->team->myrank), 
                                        args->nbytes*op->team->myrank);
    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*2+2) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    /*free up the scratch space used by this op*/
    if(op->team->total_ranks > 1) gasnete_coll_free_scratch(op);    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gall_Dissem(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, 
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  
  

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_Dissem, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gall_DissemNoScratch(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    data->state++;
  } 
  if(data->state == 1) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    if(op->team->total_ranks == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      data->state = dissem->dissemination_phases*2+2;
    } else {
      /*memcpy the data into the start of the scratch space*/
      GASNETE_FAST_UNALIGNED_MEMCPY(args->dst,
                                    args->src, args->nbytes);
      data->state++;
    }
  }
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                      (int8_t*)args->dst+curr_len,
                                      args->dst,
                                      curr_len, phase, 1);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(data->p2p->state[phase] == 1) {
        data->state++;
      } else {
        return 0;
      }
    }
  }
  
  if(data->state == (dissem->dissemination_phases)*2) {
    uint32_t phase = (data->state-2)/2;
    size_t nblk = op->team->total_ranks - (1<<phase); 
    size_t curr_len = args->nbytes*(nblk);
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                    (int8_t*)args->dst+(1<<phase)*args->nbytes,
                                    (int8_t*)args->dst,
                                    curr_len, phase, 1);
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1) {
    uint32_t phase = (data->state-2)/2;
    if(data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
    data->private_data = gasneti_malloc(args->nbytes*op->team->total_ranks);
    /*rotate the data around*/
    gasnete_coll_local_rotate_right(data->private_data, args->dst, args->nbytes, op->team->total_ranks, op->team->myrank);
    GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->private_data, args->nbytes*op->team->total_ranks);
    gasneti_free(data->private_data);
    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*2+2) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    /*free up the scratch space used by this op*/
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gall_DissemNoScratch(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, 
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_DissemNoScratch, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_allM_nb() */

/*only works for COLL_SINGLE*/
static int gasnete_coll_pf_gallM_FlatPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;
  
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    /* local gather */
    gasnete_coll_local_gather(op->team->my_images, 
                              (int8_t*) GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags)+op->team->my_images*args->nbytes*op->team->myrank,
                              &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
    data->state++;
  }
  
  if(data->state == 1) {
    int dst;
    int8_t *mysrc = (int8_t*) GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags)+op->team->my_images*args->nbytes*op->team->myrank;
    for(dst=op->team->myrank+1; dst<op->team->total_ranks; dst++) {
      /* send to threads above me*/
      gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,dst), 
                                    (int8_t*) GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist,dst) + 
                                    op->team->myrank*args->nbytes*op->team->my_images, 
                                    mysrc, args->nbytes*op->team->my_images,0);
    }
    for(dst=0; dst<op->team->myrank; dst++) {
      /*send to threads below me*/
      gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,dst), 
                                    (int8_t*) GASNETE_COLL_1ST_IMAGE(op->team,args->dstlist,dst) + 
                                    op->team->myrank*args->nbytes*op->team->my_images, 
                                    mysrc, args->nbytes*op->team->my_images,0);
    }
    
    data->state++;
  }
  
  if(data->state == 2) {
    /* wait for all puts to finish*/
    if(gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) < op->team->total_ranks-1) return 0;
    gasneti_sync_reads();
    if_pt(op->team->my_images > 1) {
      gasnete_coll_local_broadcast(op->team->my_images-1, &args->dstlist[(op->flags & GASNET_COLL_LOCAL ? 0 : op->team->my_offset)+1], 
                                   GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   op->team->total_images*args->nbytes);
    }
    data->state++;
  }
  
  if(data->state == 3) {
    /* out barrier and cleanup*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gallM_FlatPut(gasnet_team_handle_t team,
                           void * const dstlist[], void * const srclist[],
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, 
                           uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC))|
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  

  
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
                                             &gasnete_coll_pf_gallM_FlatPut, options,
                                             NULL, 
                                             sequence, coll_params->num_params, 
                                             coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gallM_Dissem(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS)) return 0;
    data->state++;
  } 
  
  if(data->state == 1) {
    int i;
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    
    /*Gather the data into the start of the scratch space*/
    gasnete_coll_local_gather(op->team->my_images, 
                              (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                              &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
    
    data->state++;
  }
  
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1 && op->team->total_ranks!=1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = op->team->my_images*args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasneti_sync_reads();
      gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                      (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+curr_len,
                                      (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                      curr_len, phase, 1);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(data->p2p->state[phase] == 1) {
        data->state++;
      } else {
        return 0;
      }
    }
  }
  
  if(data->state == (dissem->dissemination_phases)*2) {
    uint32_t phase = (data->state-2)/2;
    size_t nblk = op->team->total_ranks - (1<<phase); 
    size_t curr_len = op->team->my_images*args->nbytes*(nblk);
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasneti_sync_reads();
    gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                    (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+
                                    op->team->my_images*(1<<phase)*args->nbytes,
                                    (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                    curr_len, phase, 1);
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1 || (op->team->total_ranks==1 && data->state==2)) {
    uint32_t phase = (data->state-2)/2; int i; 
    if(op->team->total_ranks > 1 && data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
    
    
    
    gasneti_sync_reads();
    /*rotate the data around*/
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags)+op->team->my_images*args->nbytes*op->team->myrank, 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        op->team->my_images*args->nbytes*(op->team->total_ranks-op->team->myrank));
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+op->team->my_images*args->nbytes*(op->team->total_ranks-op->team->myrank), 
                                        op->team->my_images*args->nbytes*op->team->myrank);
    
    if(op->team->my_images > 1) {
      gasnete_coll_local_broadcast(op->team->my_images-1, &args->dstlist[(op->flags & GASNET_COLL_LOCAL ? 0 : op->team->my_offset)+1], 
                                   GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   op->team->total_images*args->nbytes);
    }
    
    
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+2 || (op->team->total_ranks==1 && data->state==3)) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    gasnete_coll_free_scratch(op);
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/
    
    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gallM_Dissem(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  


    return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
                                               &gasnete_coll_pf_gallM_Dissem, options,
                                               NULL, 
                                               sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gallM_DissemNoScratch(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    data->state++;
  } 
  
  if(data->state == 1) {
    int i;
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    
    /*Gather the data into the start of the scratch space*/
    gasnete_coll_local_gather(op->team->my_images, 
                              (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                              &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
    
    data->state++;
  }
  
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1 && op->team->total_ranks!=1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = op->team->my_images*args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasneti_sync_reads();
      gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                      (int8_t*)GASNETE_COLL_1ST_IMAGE(op->team, args->dstlist, dstnode)+curr_len,
                                      (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                      curr_len, phase, 1);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(data->p2p->state[phase] == 1) {
        data->state++;
      } else {
        return 0;
      }
    }
  }
  
  if(data->state == (dissem->dissemination_phases)*2) {
    uint32_t phase = (data->state-2)/2;
    size_t nblk = op->team->total_ranks - (1<<phase); 
    size_t curr_len = op->team->my_images*args->nbytes*(nblk);
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasneti_sync_reads();
    gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                    (int8_t*)GASNETE_COLL_1ST_IMAGE(op->team, args->dstlist, dstnode)+
                                    op->team->my_images*(1<<phase)*args->nbytes,
                                    (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                    curr_len, phase, 1);
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1 || (op->team->total_ranks==1 && data->state==2)) {
    uint32_t phase = (data->state-2)/2; int i; 
    if(op->team->total_ranks > 1 && data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
    
    
    if(op->team->my_images == 1) {
      data->private_data = gasneti_malloc(args->nbytes*op->team->total_images);
      gasnete_coll_local_rotate_right(data->private_data, GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), op->team->my_images*args->nbytes, op->team->total_ranks, op->team->myrank);
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), data->private_data, op->team->total_images*args->nbytes);
      gasneti_free(data->private_data);
    } else {
      gasnete_coll_local_rotate_right(((int8_t**) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))[1], 
                                      ((int8_t**) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))[0],
                                      op->team->my_images*args->nbytes, op->team->total_ranks, op->team->myrank);
      gasnete_coll_local_broadcast(op->team->my_images, ((void * const *) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags)), 
                                   ((int8_t**) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))[1], op->team->total_images*args->nbytes);
      
    }
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+2 || (op->team->total_ranks==1 && data->state==3)) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gallM_DissemNoScratch(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  
  
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
                                             &gasnete_coll_pf_gallM_DissemNoScratch, options,
                                             NULL, 
                                             sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gallM_DissemNoScratchSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    data->state++;
  } 
  
  if(data->state == 1) {
    int i;
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    
    /*Gather the data into the start of the scratch space*/
    gasnete_coll_local_gather(op->team->my_images, 
                              (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                              &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
    
    data->state++;
  }
  
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*3+1 && op->team->total_ranks!=1) {
    uint32_t phase = (data->state-2)/3;
    size_t curr_len = op->team->my_images*args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(((data->state-2) % 3) == 0) {
      /* send in this phase */
      gasneti_sync_reads();
      data->handle = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                         (int8_t*)GASNETE_COLL_1ST_IMAGE(op->team, args->dstlist, dstnode)+curr_len,
                                         (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                         curr_len GASNETE_THREAD_PASS);
      gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
#if 0
      gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                      (int8_t*)GASNETE_COLL_1ST_IMAGE(op->team, args->dstlist, dstnode)+curr_len,
                                      (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                      curr_len, phase, 1);
#endif
      data->state++;
    } 
    if(((data->state-2) % 3) == 1) {
      if(data->handle!=GASNET_INVALID_HANDLE) return 0;
      gasnete_coll_p2p_change_states(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                     1, phase, 1);
      data->state++;
    }
    if(((data->state-2) % 3) == 2) {
      if(data->p2p->state[phase] == 1) {
        data->state++;
      } else {
        return 0;
      }
    }
  }
  
  if(data->state == (dissem->dissemination_phases)*3 - 1) {
    uint32_t phase = (data->state-2)/3;
    size_t nblk = op->team->total_ranks - (1<<phase); 
    size_t curr_len = op->team->my_images*args->nbytes*(nblk);
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasneti_sync_reads();
    data->handle = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team,dstnode),
                                       (int8_t*)GASNETE_COLL_1ST_IMAGE(op->team, args->dstlist, dstnode)+
                                       op->team->my_images*(1<<phase)*args->nbytes,
                                       (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                       curr_len GASNETE_THREAD_PASS);
    gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

#if 0
    gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                    (int8_t*)GASNETE_COLL_1ST_IMAGE(op->team, args->dstlist, dstnode)+
                                    op->team->my_images*(1<<phase)*args->nbytes,
                                    (int8_t*)GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                    curr_len, phase, 1);
#endif
    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*3) {
    /*wait for the previous put to finish*/
    uint32_t phase = (data->state-2)/3; 
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    if(data->handle != GASNET_INVALID_HANDLE) return 0;
    gasnete_coll_p2p_change_states(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                   1, phase, 1);
    
    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*3+1 || (op->team->total_ranks==1 && data->state==2)) {
    uint32_t phase = (data->state-2)/3; 
    int i; 
    if(op->team->total_ranks > 1 && data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
    
    
    if(op->team->my_images == 1) {
      data->private_data = gasneti_malloc(args->nbytes*op->team->total_images);
      gasnete_coll_local_rotate_right(data->private_data, GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), op->team->my_images*args->nbytes, op->team->total_ranks, op->team->myrank);
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), data->private_data, op->team->total_images*args->nbytes);
      gasneti_free(data->private_data);
    } else {
      gasnete_coll_local_rotate_right(((int8_t**) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))[1], 
                                      ((int8_t**) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))[0],
                                      op->team->my_images*args->nbytes, op->team->total_ranks, op->team->myrank);
      gasnete_coll_local_broadcast(op->team->my_images, ((void * const *) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags)), 
                                   ((int8_t**) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))[1], op->team->total_images*args->nbytes);
      
    }
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*3+2 || (op->team->total_ranks==1 && data->state==3)) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gallM_DissemNoScratchSeg(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  
  
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
                                             &gasnete_coll_pf_gallM_DissemNoScratchSeg, options,
                                             NULL, 
                                             sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/* exchg Brucks: Implemented using Dissemination based exchange algorithm */
/*
 Based on Bruck et al. algorithm 
 (from IEEE Transactions on Parallel and Distributed Computiing Vol. 8 No. 11 Nov. 1997)
 */
GASNETI_INLINE(gasnete_coll_mypow)
int gasnete_coll_mypow(int base, int pow) {
  int ret = 1;
  while(pow!=0) {
    ret *=base;
    pow--;
  }
  return ret;
}

GASNETI_INLINE(gasnete_coll_pack_all_to_all_msg)
int gasnete_coll_pack_all_to_all_msg(void *src, void *dest, size_t nbytes,
                                     int digit, int radix, int j, int total_ranks) {
  int i_idx;
  
  int ret=0;
  
  
  /*pack if the digit_th digit of the radix-r representation of block_id is j*/
  for(i_idx=0; i_idx<total_ranks; i_idx++) {
    
    if( ((i_idx / gasnete_coll_mypow(radix, digit)) % radix) == j ) {
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)dest+ret*nbytes, (int8_t*)src+i_idx*nbytes, nbytes);
      ret++;
    }
  }
  return ret;
}

GASNETI_INLINE(gasnete_coll_unpack_all_to_all_msg)
void gasnete_coll_unpack_all_to_all_msg(void *src, void *dest, size_t nbytes,
                                        int digit, int radix, int j, int total_ranks) {
  int i_idx;
  
  int blk_count=0;
  
  for(i_idx=0; i_idx<total_ranks; i_idx++) {
    if( ((i_idx / gasnete_coll_mypow(radix, digit)) % radix) == j ) {
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)dest+i_idx*nbytes, (int8_t*)src+blk_count*nbytes, nbytes);
      blk_count++;
    }
  }
}

/* This algorithm does not directly send into remote data... it uses the remote scratch space instead
 therefore it is valid for COLL_SINGLE and COLL_LOCAL
 */

static int gasnete_coll_pf_exchg_Dissem(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_exchange_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchange);
  int result = 0;
  
  size_t offset;
  
  void *scratch2;
  void *scratch1;
  
  /*this will be a slightly different poll function than the other ones*/
  /*the state will be used to describe the dissemination phase*/
  /*reserving state 0 and dissem_phase*2+2+1 for the in/out barrier stages*/
  /*state 1 will be used for local memory copies*/
  /*state dissem_phases*2+2 will represent memory copies on the output side*/
  /*states 2 through dissem_phases*2+1 represent intermediary steps*/
  /*each dissem phase will get two steps, one for sending and one for recieiving*/
  if(data->state == 0) {
    if_pt(op->team->total_ranks != 1) 
    if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS)) 
      return 0;
    data->state = 1;
  } 
  
  scratch1 = (int8_t*)op->team->scratch_segs[op->team->myrank].addr + op->myscratchpos;
  scratch2 = (int8_t*)scratch1 + ((args->nbytes)*dissem->max_dissem_blocks)*((2)*(dissem->dissemination_radix-1));
  
  
  if(data->state == 1) {
    
    if (!gasnete_coll_generic_all_threads(data) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      return result;
    }
    
    if(op->team->total_ranks == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      data->state = dissem->dissemination_phases*3+3;
      return 0; 
    }    
    /* perform local rotation right by total_ranks - myrank elements*/
    gasnete_coll_local_rotate_right((int8_t*) scratch2, args->src, args->nbytes, op->team->total_ranks, op->team->total_ranks - op->team->myrank);
    data->state = 2;
  }
  
  if(data->state>=2 && data->state<=dissem->dissemination_phases*3+1) {
    /*data transfer stages*/
    /*global phase id */
    int destnode,nblocks;
    gasnet_node_t* out_nodes, *in_nodes;
    int phase = (data->state - 2)/3;
    int h,j;
    int distance = gasnete_coll_mypow(dissem->dissemination_radix, phase);
    offset = GASNETE_COLL_DISSEM_MAX_BLOCKS(dissem)*args->nbytes;
    h = GASNETE_COLL_DISSEM_GET_PEER_COUNT_PHASE(dissem, phase);
    out_nodes = GASNETE_COLL_DISSEM_GET_FRONT_PEERS_PHASE(dissem, phase);
    in_nodes =GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase);
#define IDX_EXPR (((phase%2)*(dissem->dissemination_radix-1) + (j))*offset)
#define IDXP1_EXPR ((((phase+1)%2)*(dissem->dissemination_radix-1) + (j))*offset)
    
    if((data->state-2) % 3 == 0) {
      /*send the ok to send signal*/
      gasneti_sync_writes();
      for(j=0; j<h; j++) {
        
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team,in_nodes[j]), phase*2);
        /*XXX: switch to counting put for higher radices*/ 
        /*gasnete_coll_p2p_change_states(op,  GASNETE_COLL_REL2ACT(op->team, in_nodes[j]), 1, phase*2, 1);*/
      }
      data->state++;
    }
    if((data->state-2) % 3 == 1) {
      /*XXX: Need atomic counter read for higher radices*/
      /* if(data->p2p->state[phase*2] != h) return 0;*/
      if_pf(gasneti_weakatomic_read(&(data->p2p->counter[phase*2]),0)!=h) return 0;
      gasneti_sync_reads();
      for(j=0; j<h; j++) {
        destnode = out_nodes[j];
        nblocks = 
        gasnete_coll_pack_all_to_all_msg(scratch2, (int8_t*)scratch1+IDX_EXPR,args->nbytes,
                                         phase, dissem->dissemination_radix, j+1, op->team->total_ranks);
        gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,destnode), 
                                      (int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR, (int8_t*)scratch1+IDX_EXPR,
                                      nblocks*args->nbytes, phase*2+1);
      }
      /*once all the change the state and return 0*/
      /*let the poll function bring us back here*/
      data->state++;
      return 0;
    } 
    if((data->state-2) % 3 == 2) { /*receive in odd sub phases*/
      /*wait for all the states to trip*/
      /*need to change this to an atomic state increment to do this properly for radix>2*/
      if(gasneti_weakatomic_read(&(data->p2p->counter[phase*2+1]),0) == h) {
        gasneti_sync_reads();
        for(j=0; j<h; j++) {
          gasnete_coll_unpack_all_to_all_msg((int8_t*)scratch1+IDXP1_EXPR, (int8_t*)scratch2, args->nbytes, phase,
                                             dissem->dissemination_radix, j+1, op->team->total_ranks);
        }			
        data->state++;
        return 0;
      } else {
        return 0;
      }
    }
    
#undef IDX_EXPR
#undef IDXP1_EXPR
    
  }
  
  if(data->state == dissem->dissemination_phases*3+2) {
    int i;
    int srcnode;
    for(i=0; i<op->team->total_ranks; i++) {
      srcnode  = (op->team->myrank - i);
      if(srcnode < 0) {
        srcnode = op->team->total_ranks+srcnode;
      }
      
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)args->dst+i*args->nbytes,
                                    (int8_t*)scratch2+srcnode*args->nbytes,
                                    args->nbytes);
    }
    data->state ++;
    
  }
  if(data->state == dissem->dissemination_phases*3+3) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    /*free up the scratch space used by this op*/
    if(op->team->total_ranks != 1) gasnete_coll_free_scratch(op);    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_exchg_Dissem2(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 2;

  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
extern gasnet_coll_handle_t
gasnete_coll_exchg_Dissem3(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 3;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
extern gasnet_coll_handle_t
gasnete_coll_exchg_Dissem4(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 4;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_exchg_Dissem8(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 8;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_exchg_FlatScratch(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = team->total_ranks;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}



static int gasnete_coll_pf_exchg_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_exchange_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchange);
  int result = 0;
  int i;


  switch(data->state) {

  case 0:	/* Optional IN barrier */
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      break;
    }
    
    data->state = 1;
    
  case 1: /* fire off all the nonblocking puts*/
    gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
    /*put to the left of me*/
    for(i=op->team->myrank+1; i<op->team->total_ranks; i++) {
      gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,i), ((int8_t*) args->dst) + op->team->myrank*args->nbytes, ((int8_t*) args->src)+i*args->nbytes, args->nbytes GASNETE_THREAD_PASS);
    } 
    /*put to the right of me*/
    for(i=0; i<op->team->myrank; i++) {
      gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,i), ((int8_t*) args->dst) + op->team->myrank*args->nbytes, ((int8_t*) args->src)+i*args->nbytes, args->nbytes GASNETE_THREAD_PASS);
    }
    data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
    gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*) args->dst + op->team->myrank*args->nbytes, 
                                        (int8_t*) args->src+op->team->myrank*args->nbytes, args->nbytes);
    data->state = 2;
  case 2: /* sync all the handles for the puts*/
    if (op->team->total_ranks > 1 && data->handle != GASNET_INVALID_HANDLE) {
      return 0;
    }
    data->state=3;
  case 3: /*final out barrier*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    data->state = 4;
  case 4: /*done*/
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_exchg_Put(gasnet_team_handle_t team,
                       void *dst, void *src,
                       size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_GENERIC_OPT_P2P | 
    GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  gasneti_assert(flags & GASNET_COLL_SINGLE);
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Put, options,
                                          NULL, NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchangeM_nb() */


/* This algorithm does not directly send into remote data... it uses the remote scratch space instead
 therefore it is valid for COLL_SINGLE and COLL_LOCAL
 */
static int gasnete_coll_pf_exchgM_Dissem(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_exchangeM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchangeM);
  int result = 0;
  
  size_t offset;
  
  void *scratch2;
  void *scratch1;
  
  /*this will be a slightly different poll function than the other ones*/
  /*the state will be used to describe the dissemination phase*/
  /*reserving state 0 and dissem_phase*2+3 for the in/out barrier stages*/
  /*state 1 will be used for local memory copies*/
  /*state dissem_phases*2+2 will represent memory copies on the output side*/
  /*states 2 through dissem_phases*2+1 represent intermediary steps*/
  /*each dissem phase will get two steps, one for sending and one for recieiving*/
  
  if(data->state == 0) {
    if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS)) return 0;
    data->state = 1;
  } 
  
  scratch1 = (int8_t*)op->team->scratch_segs[op->team->myrank].addr + op->myscratchpos;
  scratch2 = (int8_t*)scratch1 + ((args->nbytes*op->team->my_images*op->team->my_images)*dissem->max_dissem_blocks)*((2)*(dissem->dissemination_radix-1));
  
  if(data->state == 1) {
    int i,j,k=0;
    int8_t **out_ptr;
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      return result;
    }
    
    /* perform local rotation and gather*/
    data->private_data = (void**) gasneti_malloc(sizeof(void*)*op->team->my_images);
    
    for(i=0; i<op->team->total_ranks; i++) {
      int i_node = i;
      int position = ((i_node < op->team->myrank) ? 
                      op->team->total_ranks + (i_node - op->team->myrank) : 
                      i_node-op->team->myrank);
      for(j=0; j<op->team->my_images; j++) {
        gasnete_coll_scale_ptrM((void**) data->private_data, 
                                &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                (i*op->team->my_images+j)*args->nbytes, 1, op->team->my_images);
        gasnete_coll_local_gather(op->team->my_images, 
                                  (int8_t*) scratch2+position*(op->team->my_images*op->team->my_images*args->nbytes)+j*op->team->my_images*args->nbytes,
                                  (void* const*) data->private_data, args->nbytes);
      }
    }
    data->state = 2; 
  }
  
  if( data->state>=2 && data->state<=dissem->dissemination_phases*3+1) {
    /*data transfer stages*/
    /*global phase id */
    int destnode,nblocks;
    gasnet_node_t* out_nodes, *in_nodes;
    int phase = (data->state - 2)/3;
    int h,j,i;
    int distance = gasnete_coll_mypow(dissem->dissemination_radix, phase);
    offset = GASNETE_COLL_DISSEM_MAX_BLOCKS(dissem)*args->nbytes*op->team->my_images*op->team->my_images;
    h = GASNETE_COLL_DISSEM_GET_PEER_COUNT_PHASE(dissem, phase);
    out_nodes = GASNETE_COLL_DISSEM_GET_FRONT_PEERS_PHASE(dissem, phase);
    in_nodes = GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase);
    
#define IDX_EXPR (((phase%2)*(dissem->dissemination_radix-1) + (j))*offset)
#define IDXP1_EXPR ((((phase+1)%2)*(dissem->dissemination_radix-1) + (j))*offset)
    
    /*send in even sub phases*/ 
    if((data->state-2) % 3 == 0) {
      /*send the ok to send signal*/
      for(j=0; j<h; j++) {
        /*XXX: switch to counting put for higher radices*/
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, in_nodes[j]), phase*2);
        /*gasnete_coll_p2p_change_states(op,  GASNETE_COLL_REL2ACT(op->team, in_nodes[j]), 1, phase*2, 1);*/
      }
      data->state++;
    }
    if((data->state-2) % 3 == 1) {
      if_pf(gasneti_weakatomic_read(&(data->p2p->counter[phase*2]),0)!=h) return 0;
      /*if(data->p2p->state[phase*2] != h) return 0;*/
      
      
      for(j=0; j<h; j++) {
        destnode = out_nodes[j];
        nblocks = 
        gasnete_coll_pack_all_to_all_msg(scratch2, (int8_t*)scratch1+IDX_EXPR,args->nbytes*op->team->my_images*op->team->my_images,
                                         phase, dissem->dissemination_radix, j+1, op->team->total_ranks);
        gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,destnode), 
                                      (int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR, (int8_t*)scratch1+IDX_EXPR,
                                      nblocks*args->nbytes*op->team->my_images*op->team->my_images, phase*2+1);
      }
      /*once all the change the state and return 0*/
      /*let the poll function bring us back here*/
      data->state++;
    } 
    if((data->state-2) % 3 == 2) { /*receive in odd sub phases*/
      /*wait for all the states to trip*/
      /*need to change this to an atomic state increment to do this properly for radix>2*/
      if(gasneti_weakatomic_read(&(data->p2p->counter[phase*2+1]),0) == h) {
        /*if(data->p2p->state[phase*2+1] == h) {*/
        gasneti_sync_reads();
        for(j=0; j<h; j++) {
          gasnete_coll_unpack_all_to_all_msg((int8_t*)scratch1+IDXP1_EXPR, (int8_t*)scratch2, args->nbytes*op->team->my_images*op->team->my_images, phase,
                                             dissem->dissemination_radix, j+1, op->team->total_ranks);
        }			
        data->state++;
      } else {
        return 0;
      }
    }
    
#undef IDX_EXPR
#undef IDXP1_EXPR
    
  }
  
  if(data->state == dissem->dissemination_phases*3+2) {
    int i;
    int j;
    int srcnode;
    gasneti_sync_reads();
    for(i=0; i<op->team->total_ranks; i++) {
      srcnode  = (op->team->myrank - i);
      if(srcnode < 0) {
        srcnode = op->team->total_ranks+srcnode;
      }
      
      
      gasnete_coll_scale_ptrM((void**) data->private_data, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), i*op->team->my_images,
                              args->nbytes, op->team->my_images);
      gasnete_coll_local_scatter(op->team->my_images,
                                 (void* const*) data->private_data, 
                                 (int8_t*) scratch2+srcnode*op->team->my_images*op->team->my_images*args->nbytes, args->nbytes*op->team->my_images);
    }
    data->state ++;
    
  }
  if(data->state == dissem->dissemination_phases*3+3) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_free_scratch(op);
    gasneti_free(data->private_data);
    data->private_data = NULL;    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_Dissem2(gasnet_team_handle_t team,
                           void * const dstlist[], void * const srclist[],
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 2;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_exchgM_Dissem3(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 3;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_exchgM_Dissem4(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 4;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_Dissem8(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 8;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_FlatScratch(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = team->total_ranks;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_exchgM_Dissem_Segmented(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_exchangeM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchangeM);
  int result = 0;
  
  size_t offset;
  
  void *scratch2;
  void *scratch1;
  
  /*this will be a slightly different poll function than the other ones*/
  /*the state will be used to describe the dissemination phase*/
  /*reserving state 0 and dissem_phase*2+3 for the in/out barrier stages*/
  /*state 1 will be used for local memory copies*/
  /*state dissem_phases*2+2 will represent memory copies on the output side*/
  /*states 2 through dissem_phases*2+1 represent intermediary steps*/
  /*each dissem phase will get two steps, one for sending and one for recieiving*/
  
  if(data->state == 0) {
    if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS)) return 0;
    data->state = 1;
  } 
  
  scratch1 = (int8_t*)op->team->scratch_segs[op->team->myrank].addr + op->myscratchpos;
  scratch2 = (int8_t*)scratch1 + ((args->nbytes*op->team->my_images*op->team->my_images)*dissem->max_dissem_blocks)*((2)*(dissem->dissemination_radix-1));
  
  if(data->state == 1) {
    int i,j,k=0;
    int8_t **out_ptr;
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      return result;
    }
    
    /* perform local rotation and gather*/
    data->private_data = (void**) gasneti_malloc(sizeof(void*)*op->team->my_images);
    
    for(i=0; i<op->team->total_ranks; i++) {
      int i_node = i;
      int position = ((i_node < op->team->myrank) ? 
                      op->team->total_ranks + (i_node - op->team->myrank) : 
                      i_node-op->team->myrank);
      for(j=0; j<op->team->my_images; j++) {
        gasnete_coll_scale_ptrM((void**) data->private_data, 
                                &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                (i*op->team->my_images+j)*args->nbytes, 1, op->team->my_images);
        gasnete_coll_local_gather(op->team->my_images, 
                                  (int8_t*) scratch2+position*(op->team->my_images*op->team->my_images*args->nbytes)+j*op->team->my_images*args->nbytes,
                                  (void* const*) data->private_data, args->nbytes);
      }
    }
    data->state = 2; 
  }
  
  if( data->state>=2 && data->state<=dissem->dissemination_phases*4+1) {
    /*data transfer stages*/
    /*global phase id */
    int destnode,nblocks;
    gasnet_node_t* out_nodes, *in_nodes;
    int phase = (data->state - 2)/4;
    int h,j,i;
    int distance = gasnete_coll_mypow(dissem->dissemination_radix, phase);
    offset = GASNETE_COLL_DISSEM_MAX_BLOCKS(dissem)*args->nbytes*op->team->my_images*op->team->my_images;
    h = GASNETE_COLL_DISSEM_GET_PEER_COUNT_PHASE(dissem, phase);
    out_nodes = GASNETE_COLL_DISSEM_GET_FRONT_PEERS_PHASE(dissem, phase);
    in_nodes = GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase);
    
#define IDX_EXPR (((phase%2)*(dissem->dissemination_radix-1) + (j))*offset)
#define IDXP1_EXPR ((((phase+1)%2)*(dissem->dissemination_radix-1) + (j))*offset)
    
    /*send in even sub phases*/ 
    if((data->state-2) % 4 == 0) {
      /*send the ok to send signal*/
      for(j=0; j<h; j++) {
        /*XXX: switch to counting put for higher radices*/
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, in_nodes[j]), phase*2);
        /*gasnete_coll_p2p_change_states(op,  GASNETE_COLL_REL2ACT(op->team, in_nodes[j]), 1, phase*2, 1);*/
      }
      
      data->state++;
    }
    if(((data->state-2) % 4) == 1) {
      if_pf(gasneti_weakatomic_read(&(data->p2p->counter[phase*2]),0)!=h) return 0;
      /*if(data->p2p->state[phase*2] != h) return 0;*/
      
      gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
      for(j=0; j<h; j++) {
        destnode = out_nodes[j];
        nblocks = 
        gasnete_coll_pack_all_to_all_msg(scratch2, (int8_t*)scratch1+IDX_EXPR,args->nbytes*op->team->my_images*op->team->my_images,
                                         phase, dissem->dissemination_radix, j+1, op->team->total_ranks);
        gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,destnode),
                             (int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR,
                             (int8_t*)scratch1+IDX_EXPR,
                             nblocks*args->nbytes*op->team->my_images*op->team->my_images GASNETE_THREAD_PASS);
        /*gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,destnode), */
        /*(int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR, (int8_t*)scratch1+IDX_EXPR,*/
        /*                                      nblocks*args->nbytes*op->team->my_images*op->team->my_images, phase*2+1);*/
      }
      /*once all the change the state and return 0*/
      /*let the poll function bring us back here*/
      data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
      gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      data->state++;
    }
    
    if(((data->state-2) % 4) == 2) {
      if(data->handle != GASNET_INVALID_HANDLE) return 0;
      for(j=0; j<h; j++) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, out_nodes[j]),phase*2+1);
      }
      data->state++;
      /*wait for the put to finish and increment the remote counter*/
    }
    if(((data->state-2) % 4) == 3) { /*receive in odd sub phases*/
      /*wait for all the states to trip*/
      /*need to change this to an atomic state increment to do this properly for radix>2*/
      if(gasneti_weakatomic_read(&(data->p2p->counter[phase*2+1]),0) == h) {
        /*if(data->p2p->state[phase*2+1] == h) {*/
        gasneti_sync_reads();
        for(j=0; j<h; j++) {
          gasnete_coll_unpack_all_to_all_msg((int8_t*)scratch1+IDXP1_EXPR, (int8_t*)scratch2, args->nbytes*op->team->my_images*op->team->my_images, phase,
                                             dissem->dissemination_radix, j+1, op->team->total_ranks);
        }			
        data->state++;
      } else {
        return 0;
      }
    }
    
#undef IDX_EXPR
#undef IDXP1_EXPR
    
  }
  
  if(data->state == dissem->dissemination_phases*4+2) {
    int i;
    int j;
    int srcnode;
    gasneti_sync_reads();
    for(i=0; i<op->team->total_ranks; i++) {
      srcnode  = (op->team->myrank - i);
      if(srcnode < 0) {
        srcnode = op->team->total_ranks+srcnode;
      }
      
      
      gasnete_coll_scale_ptrM((void**) data->private_data, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), i*op->team->my_images,
                              args->nbytes, op->team->my_images);
      gasnete_coll_local_scatter(op->team->my_images,
                                 (void* const*) data->private_data, 
                                 (int8_t*) scratch2+srcnode*op->team->my_images*op->team->my_images*args->nbytes, args->nbytes*op->team->my_images);
    }
    data->state ++;
    
  }
  if(data->state == dissem->dissemination_phases*4+3) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_free_scratch(op);
    gasneti_free(data->private_data);
    data->private_data = NULL;    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_DissemSeg2(gasnet_team_handle_t team,
                           void * const dstlist[], void * const srclist[],
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 2;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem_Segmented, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_exchgM_DissemSeg3(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 3;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem_Segmented, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_exchgM_DissemSeg4(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 4;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem_Segmented, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_DissemSeg8(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 8;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem_Segmented, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_FlatScratchSeg(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = team->total_ranks;
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
                                           &gasnete_coll_pf_exchgM_Dissem_Segmented, options,
                                           NULL, gasnete_coll_fetch_dissemination(radix,team), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* Reductions*/
#define FOLD_REDUCE_BARRIER 1
static int gasnete_coll_pf_reduce_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  const gasnete_coll_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduce);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  gasnete_coll_p2p_t *p2p = data->p2p;
  int result = 0;
  uintptr_t dst_addr, src_addr;
  int i;
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
    case 1:	/* Optional IN barrier */
      
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      if(op->team->myrank == args->dstnode) {
        GASNETE_FAST_UNALIGNED_MEMCPY((void*) args->dst, args->src, args->nbytes);
      } else if(child_count > 0) {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        GASNETE_FAST_UNALIGNED_MEMCPY((void*) myscratch, args->src, args->nbytes);
      } 
      data->state = 2;
    case 2:
     if (child_count>0) {
       int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
       volatile uint32_t *state;
       size_t nbytes = args->nbytes;
       int i, done;
       gasnet_coll_reduce_fn_t reduce_fn = gasnete_coll_fn_tbl[args->func].fnptr;
       uint32_t red_fn_flags = gasnete_coll_fn_tbl[args->func].flags;
       uint32_t reduce_args = args->func_arg;
       
       gasneti_assert(p2p != NULL);
       gasneti_assert(p2p->state != NULL);
       state = data->p2p->state;
       gasneti_assert(p2p->data != NULL);
       
   
       done = 1;
       if(op->team->myrank == args->dstnode) {
         dst_addr = (uintptr_t)(args->dst);
       } else {
         dst_addr = (uintptr_t) myscratch;
       }
       src_addr = ((uintptr_t)myscratch)+nbytes;
       
       for(i=0; i<child_count; i++, src_addr +=nbytes) {
         uint32_t s= state[i+1];
         if(s == 0) {
           done = 0;
         } else if(s== 1) {
           gasneti_sync_reads();
           (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                        (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
           state[i+1]=2;
         }
       }
       if(!done) {break;}
       if(op->team->myrank != args->dstnode) {
         int8_t* parent_scratch = (int8_t*)(((int8_t*)op->team->scratch_segs[parent].addr)+op->scratchpos[0]);
         if(op->flags & GASNET_COLL_OUT_MYSYNC) {
           gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                            ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                           (void*) dst_addr, args->nbytes, tree->geom->sibling_id+1, 1);
         } else {
           gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                            ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                           (void*) dst_addr, args->nbytes, tree->geom->sibling_id+1, 1);
           
         }
       } 
     } else if(op->team->myrank != args->dstnode){
       int8_t* parent_scratch = (int8_t*)(((int8_t*)op->team->scratch_segs[parent].addr)+op->scratchpos[0]);
       if(op->flags & GASNET_COLL_OUT_MYSYNC) {
         gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                         ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                         args->src, args->nbytes, tree->geom->sibling_id+1, 1);
       } else {
         gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                         ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                         args->src, args->nbytes, tree->geom->sibling_id+1, 1);
       }
     } 
      data->state = 3;
    case 3:
#if FOLD_REDUCE_BARRIER
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        int i;
        if(args->dstnode != op->team->myrank) {
          /*wait for parent to signal*/
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) == 0) {
            break;
          }
        }
        /*parent has signaled*/
        /*signal all children*/
        for(i=0; i<child_count; i++) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 0);
        }
      }
#else
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }      
#endif

      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      gasnete_coll_free_scratch(op);
  }
  
  return result;
}  

extern gasnet_coll_handle_t
gasnete_coll_reduce_TreePut(gasnet_team_handle_t team,
                              gasnet_image_t dstimage, void *dst,
                              void *src, size_t src_blksz, size_t src_offset,
                              size_t elem_size, size_t elem_count,
                              gasnet_coll_fn_handle_t func, int func_arg,
                              int flags, 
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                            GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_REDUCE_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
#endif
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1) | GASNETE_COLL_USE_SCRATCH;
  gasnete_coll_scratch_req_t *scratch_req;
  gasnete_coll_tree_data_t *tree_info;
  size_t nbytes = elem_size*elem_count;
  tree_info = gasnete_coll_tree_init(coll_params->tree_type,
                                     gasnete_coll_image_node(team,dstimage), team
                                     GASNETE_THREAD_PASS);
  
  scratch_req = (gasnete_coll_scratch_req_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_req_t));
  /*fill out the tree information*/
  scratch_req->tree_type = tree_info->geom->tree_type;
  scratch_req->tree_dir = GASNETE_COLL_UP_TREE;
  scratch_req->root = tree_info->geom->root;
  
  scratch_req->team = team;
  scratch_req->op_type = GASNETE_COLL_TREE_OP;
  /*fill out the peer information*/
  scratch_req->incoming_size = nbytes*(tree_info->geom->child_count+1);
  /*  fprintf(stderr, "%d> requesting %d bytes as incoming\n", gasneti_mynode, scratch_req->incoming_size); */
  scratch_req->num_in_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree_info->geom);
  if(scratch_req->num_in_peers > 0) {
    scratch_req->in_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(tree_info->geom);      
  } else {
    scratch_req->in_peers = NULL;
  }
  if(team->myrank == gasnete_coll_image_node(team, dstimage)) {
    scratch_req->num_out_peers = 0;
    scratch_req->out_peers = NULL;      
    scratch_req->out_sizes = NULL;
  } else {
    scratch_req->num_out_peers = 1;
    scratch_req->out_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(tree_info->geom));
    scratch_req->out_sizes = (uint64_t*) gasneti_malloc(sizeof(uint64_t)*1);
    scratch_req->out_sizes[0] = nbytes*(tree_info->geom->num_siblings+1);
  }
  
  return gasnete_coll_generic_reduce_nb(team, dstimage, dst, src, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduce_TreePut, options,
                                        tree_info, sequence, coll_params->num_params, coll_params->param_list, scratch_req
                                        GASNETE_THREAD_PASS);
  
}
static int gasnete_coll_pf_reduce_TreeGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  const gasnete_coll_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduce);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  gasnete_coll_p2p_t *p2p = data->p2p;
  int result = 0;
  uintptr_t dst_addr, src_addr;
  int i;
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
    case 1:	/* Optional IN barrier */
      
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      if(op->team->myrank == args->dstnode) {
        GASNETE_FAST_UNALIGNED_MEMCPY((void*) args->dst, args->src, args->nbytes);
      } else if(child_count > 0) {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        GASNETE_FAST_UNALIGNED_MEMCPY((void*) myscratch, args->src, args->nbytes);
      } 
      data->private_data = gasneti_malloc(sizeof(gasnet_handle_t)*child_count);
      data->state =2;
    case 2:
      if(child_count > 0) {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        int8_t* child_scratch;
        volatile uint32_t *state;
        size_t nbytes = args->nbytes;
        int i, done;
        gasnet_coll_reduce_fn_t reduce_fn = gasnete_coll_fn_tbl[args->func].fnptr;
        uint32_t red_fn_flags = gasnete_coll_fn_tbl[args->func].flags;
        uint32_t reduce_args = args->func_arg;
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        done = 1;
        
        if(op->team->myrank == args->dstnode) {
          dst_addr = (uintptr_t)(args->dst);
        } else {
          dst_addr = (uintptr_t) myscratch;
        }
        src_addr = ((uintptr_t)myscratch)+nbytes;
        
        for(i=0; i<child_count; i++, src_addr+=nbytes) {
          uint32_t s = state[i];
          
          switch(s) {
            case 0: done = 0; break;
            case 1: {
              /* ok to get so initiate get*/
              if((op->flags & GASNET_COLL_LOCAL || op->flags & GASNET_COLL_IN_MYSYNC || op->flags & GASNET_COLL_OUT_MYSYNC) || (tree->geom->grand_children[i]>0)) {
                child_scratch = ((int8_t*)op->team->scratch_segs[children[i]].addr)+op->scratchpos[i];
              } else {
                child_scratch = args->src;
              }
              ((gasnet_handle_t*) data->private_data)[i] = gasnete_get_nb_bulk((void*) src_addr, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                                               child_scratch, args->nbytes GASNETE_THREAD_PASS);
              gasnete_coll_save_handle(&((gasnet_handle_t*) data->private_data)[i] GASNETE_THREAD_PASS);
              
              state[i]++;
              
            }
            case 2: {
              if(((gasnet_handle_t*) data->private_data)[i] != GASNET_INVALID_HANDLE) {done = 0; break;}
              gasneti_sync_reads();
              if(!(op->flags & GASNET_COLL_OUT_ALLSYNC)) {
                gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 0);
              }
              (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                           (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
              state[i]++;
            }
            case 3: {
              /*this child is done so nothing more to do so continue on to the next child*/
              continue;
            }
          }
        }
        
        if(!done) {break;}
        if(args->dstnode != op->team->myrank){
          gasnete_coll_p2p_change_states(op, GASNETE_COLL_REL2ACT(op->team, parent), 1, tree->geom->sibling_id, 1);
        }
      } else if(op->team->myrank!=args->dstnode){
        if(op->flags & GASNET_COLL_LOCAL || op->flags & GASNET_COLL_IN_MYSYNC || op->flags & GASNET_COLL_OUT_MYSYNC) {
          int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
          GASNETE_FAST_UNALIGNED_MEMCPY((void*) myscratch, args->src, args->nbytes);
        }
        gasnete_coll_p2p_change_states(op, GASNETE_COLL_REL2ACT(op->team, parent), 1, tree->geom->sibling_id, 1);
      }
      gasneti_free(data->private_data);
      data->state = 3;
    case 3:
      /*wait for the signal from parent that the get is done*/
      if(!(op->flags & GASNET_COLL_OUT_ALLSYNC) && op->team->myrank!=args->dstnode) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) == 0) {
          break;
        }
      } 
      data->state = 4;
    case 4:
#if FOLD_REDUCE_BARRIER
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        int i;
        if(args->dstnode != op->team->myrank) {
          /*wait for parent to signal*/
          if(gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) == 0) {
            break;
          }
        }
        /*parent has signaled*/
        /*signal all children*/
        for(i=0; i<child_count; i++) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 1);
        }
      }
#else
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }      
#endif
    
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      gasnete_coll_free_scratch(op);
}
      
  
  
  return result;
}
           

extern gasnet_coll_handle_t
gasnete_coll_reduce_TreeGet(gasnet_team_handle_t team,
                            gasnet_image_t dstimage, void *dst,
                            void *src, size_t src_blksz, size_t src_offset,
                            size_t elem_size, size_t elem_count,
                            gasnet_coll_fn_handle_t func, int func_arg,
                            int flags, 
                            gasnete_coll_implementation_t coll_params,
                            uint32_t sequence
                            GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_REDUCE_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
#endif
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1) | GASNETE_COLL_USE_SCRATCH;
  gasnete_coll_scratch_req_t *scratch_req;
  gasnete_coll_tree_data_t *tree_info;
  size_t nbytes = elem_size*elem_count;
  int i;
  
  tree_info = gasnete_coll_tree_init(coll_params->tree_type,
                                     gasnete_coll_image_node(team,dstimage), team
                                     GASNETE_THREAD_PASS);
  
  scratch_req = (gasnete_coll_scratch_req_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_req_t));
  /*fill out the tree information*/
  scratch_req->tree_type = tree_info->geom->tree_type;
  scratch_req->tree_dir = GASNETE_COLL_DOWN_TREE;
  scratch_req->root = tree_info->geom->root;
  
  scratch_req->team = team;
  scratch_req->op_type = GASNETE_COLL_TREE_OP;
  /*fill out the peer information*/
  scratch_req->incoming_size = nbytes*(tree_info->geom->child_count+1);
  /*  fprintf(stderr, "%d> requesting %d bytes as incoming\n", gasneti_mynode, scratch_req->incoming_size); */
  
  if(team->myrank == gasnete_coll_image_node(team, dstimage)) {
    scratch_req->num_in_peers = 0;
    scratch_req->in_peers = NULL;
  } else {
    scratch_req->num_in_peers = 1;
    scratch_req->in_peers = &GASNETE_COLL_TREE_GEOM_PARENT(tree_info->geom); 
  }
  
  scratch_req->num_out_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree_info->geom);
  scratch_req->out_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(tree_info->geom);
  scratch_req->out_sizes = (uint64_t*) gasneti_malloc(sizeof(uint64_t)*GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree_info->geom));
  for(i=0; i<scratch_req->num_out_peers; i++) {
    scratch_req->out_sizes[i] = nbytes*(tree_info->geom->grand_children[i]+1);
  }
    
  return gasnete_coll_generic_reduce_nb(team, dstimage, dst, src, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduce_TreeGet, options,
                                        tree_info, sequence, coll_params->num_params, coll_params->param_list, scratch_req
                                        GASNETE_THREAD_PASS);
  
}
static int gasnete_coll_pf_reduce_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduce);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size_elem;
      int num_elem_segs;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
      
      
#if !GASNET_SEQ
      gasnet_image_t dstproc = args->dstimage;
#else
      gasnet_node_t dstproc = args->dstnode;
#endif
      size_t nbytes = args->nbytes;
      size_t sent_elem=0;
      
      int i;
      
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      gasneti_assert((size_t) op->param_list[0] % args->elem_size == 0);

      seg_size_elem = (size_t) op->param_list[0]/args->elem_size;
      num_elem_segs = (args->elem_count+seg_size_elem-1)/seg_size_elem;
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_elem_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_elem_segs);
      
      /*
       the TreePut routines only work w/ the single address mode and therefore we need ensure choose 
       between TreePut and TreePutScratch which collective routine gets invoked when the we are segmenting broadcast
       */
      
      for(i=0; i<num_elem_segs-1; i++) {
        handle_vec->handles[i] = gasnete_coll_reduce_TreePut(op->team, dstproc, 
                                                             gasnete_coll_scale_ptr(args->dst, sent_elem, args->elem_size), 
                                                             gasnete_coll_scale_ptr(args->src, sent_elem, args->elem_size), 
                                                             args->src_blksz, args->src_offset, args->elem_size, seg_size_elem,
                                                             args->func, args->func_arg, flags, 
                                                            impl, op->sequence+i+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        sent_elem+=seg_size_elem;
      }
      handle_vec->handles[i] = gasnete_coll_reduce_TreePut(op->team, dstproc, 
                                                           gasnete_coll_scale_ptr(args->dst, sent_elem, args->elem_size), 
                                                           gasnete_coll_scale_ptr(args->src, sent_elem, args->elem_size), 
                                                           args->src_blksz, args->src_offset, args->elem_size, args->elem_count - sent_elem,
                                                           args->func, args->func_arg, flags, 
                                                           impl, op->sequence+i+1 GASNETE_THREAD_PASS);
      gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2;
      
    case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_reduce_TreePutSeg(gasnet_team_handle_t team,
                            gasnet_image_t dstimage, void *dst,
                            void *src, size_t src_blksz, size_t src_offset,
                            size_t elem_size, size_t elem_count,
                            gasnet_coll_fn_handle_t func, int func_arg,
                            int flags, 
                            gasnete_coll_implementation_t coll_params,
                            uint32_t sequence
                            GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  gasnete_coll_tree_data_t *tree_info;
  size_t nbytes = elem_size*elem_count;
  int i;
  size_t seg_size;
  uint32_t num_segs;
  
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  
  num_segs = (nbytes+seg_size-1)/seg_size;
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);
  
  
  tree_info = gasnete_coll_tree_init(coll_params->tree_type,
                                     gasnete_coll_image_node(team,dstimage), team
                                     GASNETE_THREAD_PASS);
  
  
  
  return gasnete_coll_generic_reduce_nb(team, dstimage, dst, src, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduce_TreePutSeg, options,
                                        tree_info, (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list, NULL
                                        GASNETE_THREAD_PASS);
  
}



static int gasnete_coll_pf_reduceM_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  const gasnete_coll_reduceM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduceM);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  gasnete_coll_p2p_t *p2p = data->p2p;
  int result = 0;
  uintptr_t dst_addr, src_addr;
  int i;
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
    case 1:	/* Optional IN barrier */
      
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      if(op->team->myrank == args->dstnode) {
        
        /*perform local reduction into the args->dst*/
        gasnete_coll_local_reduce(op->team->my_images, args->dst, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                  args->elem_size, args->elem_count, args->func, args->func_arg);
      } else  {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        /*perform local reduction into the p2p->data*/
        gasnete_coll_local_reduce(op->team->my_images, (void*) myscratch, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                  args->elem_size, args->elem_count, args->func, args->func_arg);
      } 
      
      data->state = 2;
    case 2:
      if (child_count>0) {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        volatile uint32_t *state;
        size_t nbytes = args->nbytes;
        int i, done;
        gasnet_coll_reduce_fn_t reduce_fn = gasnete_coll_fn_tbl[args->func].fnptr;
        uint32_t red_fn_flags = gasnete_coll_fn_tbl[args->func].flags;
        uint32_t reduce_args = args->func_arg;
        
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        
        
        done = 1;
        if(op->team->myrank == args->dstnode) {
          dst_addr = (uintptr_t)(args->dst);
        } else {
          dst_addr = (uintptr_t) myscratch;
        }
        src_addr = ((uintptr_t)myscratch)+nbytes;
        
        for(i=0; i<child_count; i++, src_addr +=nbytes) {
          uint32_t s= state[i+1];
          if(s == 0) {
            done = 0;
          } else if(s== 1) {
            gasneti_sync_reads();
            (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                         (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
            state[i+1]=2;
          }
        }
        if(!done) {break;}
        if(op->team->myrank != args->dstnode) {
          int8_t* parent_scratch = (int8_t*)(((int8_t*)op->team->scratch_segs[parent].addr)+op->scratchpos[0]);
          if(op->flags & GASNET_COLL_OUT_MYSYNC) {
            gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                            ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                            (void*) dst_addr, args->nbytes, tree->geom->sibling_id+1, 1);
          } else {
            gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                                 ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                                 (void*) dst_addr, args->nbytes, tree->geom->sibling_id+1, 1);
            
          }
        } 
      } else if(op->team->myrank != args->dstnode){
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        int8_t* parent_scratch = (int8_t*)(((int8_t*)op->team->scratch_segs[parent].addr)+op->scratchpos[0]);
        if(op->flags & GASNET_COLL_OUT_MYSYNC) {
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                          ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                          myscratch, args->nbytes, tree->geom->sibling_id+1, 1);
        } else {
          gasnete_coll_p2p_signalling_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent), 
                                               ((int8_t*)parent_scratch)+(tree->geom->sibling_id+1)*args->nbytes, 
                                               myscratch, args->nbytes, tree->geom->sibling_id+1, 1);
        }
      } 
      data->state = 3;
    case 3:
#if FOLD_REDUCE_BARRIER
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        int i;
        if(args->dstnode != op->team->myrank) {
          /*wait for parent to signal*/
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) == 0) {
            break;
          }
        }
        /*parent has signaled*/
        /*signal all children*/
        for(i=0; i<child_count; i++) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 0);
        }
      }
#else
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }      
#endif
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      gasnete_coll_free_scratch(op);
  }
  
  return result;
}  

extern gasnet_coll_handle_t
gasnete_coll_reduceM_TreePut(gasnet_team_handle_t team,
                            gasnet_image_t dstimage, void *dst,
                            void * const srclist[], size_t src_blksz, size_t src_offset,
                            size_t elem_size, size_t elem_count,
                            gasnet_coll_fn_handle_t func, int func_arg,
                            int flags, 
                            gasnete_coll_implementation_t coll_params,
                            uint32_t sequence
                            GASNETE_THREAD_FARG){
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_REDUCE_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
#endif
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1) | GASNETE_COLL_USE_SCRATCH;
  gasnete_coll_scratch_req_t *scratch_req;
  gasnete_coll_tree_data_t *tree_info;
  size_t nbytes = elem_size*elem_count;
  tree_info = gasnete_coll_tree_init(coll_params->tree_type,
                                     gasnete_coll_image_node(team,dstimage), team
                                     GASNETE_THREAD_PASS);
  
  if(td->my_local_image == 0) {
    scratch_req = (gasnete_coll_scratch_req_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_req_t));
    /*fill out the tree information*/
    scratch_req->tree_type = tree_info->geom->tree_type;
    scratch_req->tree_dir = GASNETE_COLL_UP_TREE;
    scratch_req->root = tree_info->geom->root;
    
    scratch_req->team = team;
    scratch_req->op_type = GASNETE_COLL_TREE_OP;
    /*fill out the peer information*/
    scratch_req->incoming_size = nbytes*(tree_info->geom->child_count+1);
    /*  fprintf(stderr, "%d> requesting %d bytes as incoming\n", gasneti_mynode, scratch_req->incoming_size); */
    scratch_req->num_in_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree_info->geom);
    if(scratch_req->num_in_peers > 0) {
      scratch_req->in_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(tree_info->geom);      
    } else {
      scratch_req->in_peers = NULL;
    }
    if(team->myrank == gasnete_coll_image_node(team, dstimage)) {
      scratch_req->num_out_peers = 0;
      scratch_req->out_peers = NULL;      
      scratch_req->out_sizes = NULL;
    } else {
      scratch_req->num_out_peers = 1;
      scratch_req->out_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(tree_info->geom));
      scratch_req->out_sizes = (uint64_t*) gasneti_malloc(sizeof(uint64_t)*1);
      scratch_req->out_sizes[0] = nbytes*(tree_info->geom->num_siblings+1);
    }
  } else {
    scratch_req = NULL;
  }
  
  return gasnete_coll_generic_reduceM_nb(team, dstimage, dst, srclist, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduceM_TreePut, options,
                                        tree_info, sequence, coll_params->num_params, coll_params->param_list, scratch_req
                                        GASNETE_THREAD_PASS);
  
}

static int gasnete_coll_pf_reduceM_TreeGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  const gasnete_coll_reduceM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduceM);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  gasnete_coll_p2p_t *p2p = data->p2p;
  int result = 0;
  uintptr_t dst_addr, src_addr;
  int i;
  
  switch (data->state) { 
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))
          break;
      }
      data->state = 1;
    case 1:	/* Optional IN barrier */
      
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      if(op->team->myrank == args->dstnode) {
        
        /*perform local reduction into the args->dst*/
        gasnete_coll_local_reduce(op->team->my_images, args->dst, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                  args->elem_size, args->elem_count, args->func, args->func_arg);
      } else  {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        /*perform local reduction into the scratchspace*/
        gasnete_coll_local_reduce(op->team->my_images, (void*) myscratch, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                  args->elem_size, args->elem_count, args->func, args->func_arg);
      } 
      data->private_data = gasneti_malloc(sizeof(gasnet_handle_t)*child_count);
      data->state = 2;
    case 2:
      if(child_count > 0) {
        int8_t *myscratch = ((int8_t*)op->team->scratch_segs[op->team->myrank].addr)+op->myscratchpos;
        int8_t* child_scratch;
        volatile uint32_t *state;
        size_t nbytes = args->nbytes;
        int i, done;
        gasnet_coll_reduce_fn_t reduce_fn = gasnete_coll_fn_tbl[args->func].fnptr;
        uint32_t red_fn_flags = gasnete_coll_fn_tbl[args->func].flags;
        uint32_t reduce_args = args->func_arg;
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        done = 1;
        
        if(op->team->myrank == args->dstnode) {
          dst_addr = (uintptr_t)(args->dst);
        } else {
          dst_addr = (uintptr_t) myscratch;
        }
        src_addr = ((uintptr_t)myscratch)+nbytes;
        
        for(i=0; i<child_count; i++, src_addr+=nbytes) {
          uint32_t s = state[i];
          
          switch(s) {
            case 0: done = 0; break;
            case 1: {
              /* ok to get so initiate get*/
              child_scratch = ((int8_t*)op->team->scratch_segs[children[i]].addr)+op->scratchpos[i];
              ((gasnet_handle_t*) data->private_data)[i] = gasnete_get_nb_bulk((void*) src_addr, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                                               child_scratch, args->nbytes GASNETE_THREAD_PASS);
              gasnete_coll_save_handle(&((gasnet_handle_t*) data->private_data)[i] GASNETE_THREAD_PASS);
              
              state[i]++;
              
            }
            case 2: {
              if(((gasnet_handle_t*) data->private_data)[i] != GASNET_INVALID_HANDLE) {done = 0; break;}
              gasneti_sync_reads();
              if(!(op->flags & GASNET_COLL_OUT_ALLSYNC)) {
                gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 0);
              }
              (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                           (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
              state[i]++;
            }
            case 3: {
              /*this child is done so nothing more to do so continue on to the next child*/
              continue;
            }
          }
        }
        
        if(!done) {break;}
        if(args->dstnode != op->team->myrank){
          gasnete_coll_p2p_change_states(op, GASNETE_COLL_REL2ACT(op->team, parent), 1, tree->geom->sibling_id, 1);
        }
      } else if(op->team->myrank!=args->dstnode){
        gasnete_coll_p2p_change_states(op, GASNETE_COLL_REL2ACT(op->team, parent), 1, tree->geom->sibling_id, 1);
      }
      gasneti_free(data->private_data);
      data->state = 3;
    case 3:
      /*wait for the signal from parent that the get is done*/
      if(!(op->flags & GASNET_COLL_OUT_ALLSYNC) && op->team->myrank!=args->dstnode) {
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) == 0) {
          break;
        }
      } 
      data->state = 4;
    case 4:
#if FOLD_REDUCE_BARRIER
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        int i;
        if(args->dstnode != op->team->myrank) {
          /*wait for parent to signal*/
          if(gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) == 0) {
            break;
          }
        }
        /*parent has signaled*/
        /*signal all children*/
        for(i=0; i<child_count; i++) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 1);
        }
      }
#else
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }      
#endif
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      gasnete_coll_free_scratch(op);
  }
  
  
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_reduceM_TreeGet(gasnet_team_handle_t team,
                            gasnet_image_t dstimage, void *dst,
                            void * const srclist[], size_t src_blksz, size_t src_offset,
                            size_t elem_size, size_t elem_count,
                            gasnet_coll_fn_handle_t func, int func_arg,
                            int flags, 
                            gasnete_coll_implementation_t coll_params,
                            uint32_t sequence
                            GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_REDUCE_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
#endif
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1) | GASNETE_COLL_USE_SCRATCH;
  gasnete_coll_scratch_req_t *scratch_req=NULL;
  gasnete_coll_tree_data_t *tree_info;
  size_t nbytes = elem_size*elem_count;
  int i;
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  
  tree_info = gasnete_coll_tree_init(coll_params->tree_type,
                                     gasnete_coll_image_node(team,dstimage), team
                                     GASNETE_THREAD_PASS);
  
  if(td->my_local_image == 0) {
    scratch_req = (gasnete_coll_scratch_req_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_req_t));
    /*fill out the tree information*/
    scratch_req->tree_type = tree_info->geom->tree_type;
    scratch_req->tree_dir = GASNETE_COLL_DOWN_TREE;
    scratch_req->root = tree_info->geom->root;
    
    scratch_req->team = team;
    scratch_req->op_type = GASNETE_COLL_TREE_OP;
    /*fill out the peer information*/
    scratch_req->incoming_size = nbytes*(tree_info->geom->child_count+1);
    /*  fprintf(stderr, "%d> requesting %d bytes as incoming\n", gasneti_mynode, scratch_req->incoming_size); */
    
    if(team->myrank == gasnete_coll_image_node(team, dstimage)) {
      scratch_req->num_in_peers = 0;
      scratch_req->in_peers = NULL;
    } else {
      scratch_req->num_in_peers = 1;
      scratch_req->in_peers = &GASNETE_COLL_TREE_GEOM_PARENT(tree_info->geom); 
    }
    
    scratch_req->num_out_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree_info->geom);
    scratch_req->out_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(tree_info->geom);
    scratch_req->out_sizes = (uint64_t*) gasneti_malloc(sizeof(uint64_t)*GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree_info->geom));
    for(i=0; i<scratch_req->num_out_peers; i++) {
      scratch_req->out_sizes[i] = nbytes*(tree_info->geom->grand_children[i]+1);
    }
  }
  return gasnete_coll_generic_reduceM_nb(team, dstimage, dst, srclist, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduceM_TreeGet, options,
                                        tree_info, sequence, coll_params->num_params, coll_params->param_list, scratch_req
                                        GASNETE_THREAD_PASS);
  
}

static int gasnete_coll_pf_reduceM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_reduceM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduceM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size_elem;
      int num_elem_segs;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      void **addrs;
      gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? op->team->my_images : op->team->total_images);

      
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
      
      
#if !GASNET_SEQ
      gasnet_image_t dstproc = args->dstimage;
#else
      gasnet_node_t dstproc = args->dstnode;
#endif
      size_t nbytes = args->nbytes;
      size_t sent_elem=0;
      
      int i;
      
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = op->tree_info->geom->tree_type;
      
      gasneti_assert((size_t) op->param_list[0] % args->elem_size == 0);
      
      seg_size_elem = (size_t) op->param_list[0]/args->elem_size;
      num_elem_segs = (args->elem_count+seg_size_elem-1)/seg_size_elem;
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
      handle_vec = data->private_data;
      handle_vec->num_handles = num_elem_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_elem_segs);
      addrs = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));
            
      /*
       the TreePut routines only work w/ the single address mode and therefore we need ensure choose 
       between TreePut and TreePutScratch which collective routine gets invoked when the we are segmenting broadcast
       */
      
      for(i=0; i<num_elem_segs-1; i++) {
        gasnete_coll_scale_ptrM(addrs, args->srclist, sent_elem, args->elem_size, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_reduceM_TreePut(op->team, dstproc, 
                                                             gasnete_coll_scale_ptr(args->dst, sent_elem, args->elem_size), 
                                                             addrs,
                                                             args->src_blksz, args->src_offset, args->elem_size, seg_size_elem,
                                                             args->func, args->func_arg, flags, 
                                                             impl, op->sequence+i+1 GASNETE_THREAD_PASS);
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        sent_elem+=seg_size_elem;
      }
      gasnete_coll_scale_ptrM(addrs, args->srclist, sent_elem, args->elem_size, numaddrs); 
      handle_vec->handles[i] = gasnete_coll_reduceM_TreePut(op->team, dstproc, 
                                                           gasnete_coll_scale_ptr(args->dst, sent_elem, args->elem_size), 
                                                           addrs, 
                                                           args->src_blksz, args->src_offset, args->elem_size, args->elem_count - sent_elem,
                                                           args->func, args->func_arg, flags, 
                                                           impl, op->sequence+i+1 GASNETE_THREAD_PASS);
      gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2;
      
    case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETE_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
      data->state = 3;
    }   
    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_reduceM_TreePutSeg(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void * const srclist[], size_t src_blksz, size_t src_offset,
                               size_t elem_size, size_t elem_count,
                               gasnet_coll_fn_handle_t func, int func_arg,
                               int flags, 
                               gasnete_coll_implementation_t coll_params,
                               uint32_t sequence
                               GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  gasnete_coll_tree_data_t *tree_info;
  size_t nbytes = elem_size*elem_count;
  int i;
  size_t seg_size;
  uint32_t num_segs;
  
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  
  num_segs = (nbytes+seg_size-1)/seg_size;
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);
  
  
  tree_info = gasnete_coll_tree_init(coll_params->tree_type,
                                     gasnete_coll_image_node(team,dstimage), team
                                     GASNETE_THREAD_PASS);
  
  
  
  return gasnete_coll_generic_reduceM_nb(team, dstimage, dst, srclist, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduceM_TreePutSeg, options,
                                        tree_info, (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list, NULL
                                        GASNETE_THREAD_PASS);
  
}

