/*   $Source$
 *     $Date$
 * $Revision$
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2004, Rajesh Nishtala <rajeshn@eecs.berkeley.edu> Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* for now this file will be directly included in refcoll.c so no need to worry*/
/* about including the header files*/
#include <gasnet_internal.h>
#include <gasnet_coll_internal.h>
#include <gasnet_coll_trees.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_autotune.h>
#include <gasnet_vis.h>

typedef struct {int num_handles; gasnet_coll_handle_t *handles;} gasnete_coll_handle_vec_t;
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode, args->src,
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Get(gasnet_team_handle_t team,
		       void *dst,
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  gasneti_assert(flags & GASNET_COLL_SINGLE);
  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_Get, options, NULL, sequence GASNETE_THREAD_PASS);
}

/* bcast Put: root node performs carefully ordered puts */
static int gasnete_coll_pf_bcast_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode != args->srcnode) {
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
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    gasnete_put_nbi_bulk(i, dst, src, nbytes GASNETE_THREAD_PASS);
	  }
	  /* Put to nodes to the "left" of ourself */
	  for (i = 0; i < gasneti_mynode; ++i) {
	    gasnete_put_nbi_bulk(i, dst, src, nbytes GASNETE_THREAD_PASS);
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Put(gasnet_team_handle_t team,
		       void *dst,
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  gasneti_assert(flags & GASNET_COLL_SINGLE);
  gasneti_assert(flags & GASNET_COLL_DST_IN_SEGMENT);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_Put, options,
					   NULL, sequence GASNETE_THREAD_PASS);
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
  	if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
	  break;
	}
        if (gasneti_mynode != args->srcnode) {
	  gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
	}
      }
      data->state = 2;

    case 2:
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      if (gasneti_mynode == args->srcnode) {
        gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	for (child = 0; child < child_count; child++) {
          if(tree->geom->subtree_sizes[child] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(children[child], args->dst, args->src, args->nbytes GASNETE_THREAD_PASS);
          } else { 
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, children[child], args->dst, 
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
            gasnete_put_nbi_bulk(children[child], args->dst, args->dst, args->nbytes GASNETE_THREAD_PASS);
          } else { 
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, children[child], args->dst, 
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
      if (!gasnete_coll_generic_outsync(data)) {
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
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePut(gasnet_team_handle_t team,
		           void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNET_COLL_IN_NOSYNC))  |*/
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) |
    GASNETE_COLL_GENERIC_OPT_P2P;
  

  gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreePut, options,
					   gasnete_coll_tree_init(tree_type, 
								  gasnete_coll_image_node(srcimage), team
								  GASNETE_THREAD_PASS),
					   sequence
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
      if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
        break;
      }
      if (gasneti_mynode != args->srcnode) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
      }
    }
    data->state = 3;
    


  case 3:
      if (gasneti_mynode == args->srcnode) {
          for (child = 0; child < child_count; child++) {
      
	  gasnete_coll_p2p_signalling_put(op, children[child], 
					  (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                          args->src, args->nbytes, 0, 1);
	  
	}
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	
	for (child = 0; child < child_count; child++) {

	  gasnete_coll_p2p_signalling_put/*Async*/(op, children[child], 
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }
      /*free up the scratch space used by this op*/
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePutScratch(gasnet_team_handle_t team,
		           void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG)
{
  /* never allocate an insync barrier since this function should not be used for IN_ALLSYNC*/
  /* use TreePut instead since an inall sync need not pay the extra copy costs to and from the scratch*/
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & (GASNET_COLL_IN_NOSYNC|GASNET_COLL_IN_MYSYNC)))  |*/
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;

  gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreePutScratch, options,
					   gasnete_coll_tree_init(tree_type, 
								  gasnete_coll_image_node(srcimage), team
								  GASNETE_THREAD_PASS),
					   sequence
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
        gasnete_coll_handle_vec_t *handle_vec;
        size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNETE_COLL_BROADCAST_OP, op->flags);
        int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
	int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        #if !GASNET_SEQ
        gasnet_image_t srcproc = args->srcimage;
        #else
	gasnet_node_t srcproc = args->srcnode;
        #endif
	size_t nbytes = args->nbytes;
        size_t sent_bytes=0;
        
	int i;

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
                                                                tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
            gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
            sent_bytes+=seg_size;
          }
          handle_vec->handles[i] = gasnete_coll_bcast_TreePut(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                              srcproc, 
                                                              gasnete_coll_scale_ptr(args->src, sent_bytes, 1), args->nbytes-sent_bytes, flags, 
                                                              tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        } else {
          for(i=0; i<num_segs-1; i++) {
            handle_vec->handles[i] = gasnete_coll_bcast_TreePutScratch(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                                srcproc, gasnete_coll_scale_ptr(args->src, sent_bytes, 1), seg_size, flags, 
                                                                tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
            gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
            sent_bytes+=seg_size;
          }
          handle_vec->handles[i] = gasnete_coll_bcast_TreePutScratch(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                              srcproc, 
                                                              gasnete_coll_scale_ptr(args->src, sent_bytes, 1), args->nbytes-sent_bytes, flags, 
                                                              tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);          
        }
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
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        
        gasneti_free(data->private_data);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePutSeg(gasnet_team_handle_t team,
                               void * dst,
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags,
                               gasnete_coll_tree_type_t tree_type,
                               uint32_t sequence
                               GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC |
      GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_BROADCAST_OP, flags);
  uint32_t num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcast_TreePutSeg, options,
					    gasnete_coll_tree_init(tree_type, 
                                                                   gasnete_coll_image_node(srcimage), team
                                                                   GASNETE_THREAD_PASS), 
                                            num_segs
                                            GASNETE_THREAD_PASS);
}

#if 0
/* bcast TreeGet */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
/* Naturally IN_MYSYNC, OUT_MYSYNC */
/* size is unbounded */

static int gasnete_coll_pf_bcast_TreeGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  int i;
  int child;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;


    case 1:
      if (gasneti_mynode == args->srcnode) {
		      /* Sent my address to my children so they can issue their gets */
        for (child=0; child < GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom); child++) {
	  gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom)[child], args->src, 0, 1);
        }

	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
        data->state = 3;

	break;	/* skip state 2 */
      } else if (data->p2p->state[0]){
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* I have address from my parent, so perform a get */
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom),
					   *(void **)data->p2p->data,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
  data->state = 2;


    case 2:
      gasneti_assert(gasneti_mynode != args->srcnode);
      if (data->handle != GASNET_INVALID_HANDLE) {
	 break;
      }

      /* Send ack to my parent */
      gasnete_coll_p2p_change_state(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom), GASNETE_COLL_TREE_GEOM_SIBLING_ID(tree->geom)+1, 1);
      /* Sent my address to my children so they can issue their gets */
      for (child=0; child < GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom); child++) {
	gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom)[child], args->dst, 0, 1);
      }
      data->state = 3;


    case 3:	/* Wait for all children to ack */
    {
      int done = 1;
      for (i=1; i <= GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom); i++) {
	if (data->p2p->state[i] == 0) {
	  done = 0;
	  break;
	}
      }

      if (done) {
	
	data->state = 4;
      } else {
	break;
      }
    }

    case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

/*      gasnete_coll_tree_free(tree GASNETE_THREAD_PASS); */
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeGet(gasnet_team_handle_t team,
		           void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreeGet, options,
					    gasnete_coll_tree_init(tree_type, 
								  gasnete_coll_image_node(srcimage), team 
								  GASNETE_THREAD_PASS),
					   sequence
					   GASNETE_THREAD_PASS);
}
#endif

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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				     args->src, args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        /* Get only the 1st local image */
	data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
					   args->srcnode, args->src, args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement and perform local copies */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      } else if (gasneti_mynode != args->srcnode) {
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0);
	gasneti_sync_reads();
	gasnete_coll_local_broadcast(gasnete_coll_my_images - 1, p + 1, *p, args->nbytes);
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Get(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_Get, options,
					    NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1: 	/* Initiate data movement */
      if (gasneti_mynode != args->srcnode) {
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
	  if (gasneti_mynode < gasneti_nodes - 1) {
	    p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, gasneti_mynode + 1);
	    for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	      limit = gasnete_coll_all_images[i];
	      for (j = 0; j < limit; ++j) {
		gasnete_put_nbi_bulk(i, *p, src, nbytes GASNETE_THREAD_PASS);
		++p;
	      }
	    }
	  }
	  /* Put to nodes to the "left" of ourself */
	  if (gasneti_mynode != 0) {
	    p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, 0);
	    for (i = 0; i < gasneti_mynode; ++i) {
	      limit = gasnete_coll_all_images[i];
	      for (j = 0; j < limit; ++j) {
		gasnete_put_nbi_bulk(i, *p, src, nbytes GASNETE_THREAD_PASS);
		++p;
	      }
	    }
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				     src, nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Put(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_Put, options,
					    NULL, sequence GASNETE_THREAD_PASS);
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
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
	break;
      }
      data->state = 1; 
      
    case 1:
      if(!(op->flags & GASNET_COLL_IN_NOSYNC)) {
  	if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
	  break;
	}
        if (gasneti_mynode != args->srcnode) {
	  gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
	}
      }
      data->state = 2;
      
    case 2:
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      if (gasneti_mynode == args->srcnode) {
        gasneti_sync_reads();
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	for (child = 0; child < child_count; child++) {
	  if(tree->geom->subtree_sizes[child] == 1 && gasnete_coll_all_images[children[child]] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(children[child], GASNETE_COLL_1ST_IMAGE(args->dstlist, children[child]), args->src, args->nbytes GASNETE_THREAD_PASS);
          } else {
	    /* Destiantion is an Internal Node*/
	    /* Use Async Long */
	    gasnete_coll_p2p_signalling_putAsync(op, children[child], GASNETE_COLL_1ST_IMAGE(args->dstlist, children[child]), 
						 args->src, args->nbytes, 0, 1);
	  
	  }
	}
        data->handle  = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				     args->src, args->nbytes);
      } else if (child_count == 0 && gasnete_coll_my_images == 1) {
        /* leaves fall right through*/
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);	
        for (child = 0; child < child_count; child++) {
	  if(tree->geom->subtree_sizes[child] == 1 && gasnete_coll_all_images[children[child]] == 1) {
            /* Destination is a Leaf */ 
            /* Use NB Put + Sync */
            gasnete_put_nbi_bulk(children[child], GASNETE_COLL_1ST_IMAGE(args->dstlist, children[child]), 
                                 GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), args->nbytes GASNETE_THREAD_PASS);
          } else { 
            /* Destiantion is an Internal Node*/
            /* Use Async Long */
            gasnete_coll_p2p_signalling_putAsync(op, children[child], GASNETE_COLL_1ST_IMAGE(args->dstlist, children[child]), 
                                                 GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), args->nbytes, 0, 1);
            
          }
	}
	data->handle  = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);  
        /** XXX: Here we might do extra work by copying from teh same source and dest **/
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), args->nbytes);
        
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
#if 1
        if (!gasnete_coll_generic_outsync(data)) {
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
          gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
	  result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePut(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, 
                        gasnete_coll_tree_type_t tree_type,    
                        uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |*/
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) | 
                GASNETE_COLL_GENERIC_OPT_P2P;
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_TreePut, options,
					    gasnete_coll_tree_init(tree_type, 
                                                                   gasnete_coll_image_node(srcimage), team
                                                                   GASNETE_THREAD_PASS),
                                            sequence GASNETE_THREAD_PASS);
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
        if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
          break;
        }
        if (gasneti_mynode != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
        }
      }
      data->state = 3;
      
      
      
    case 3:
      if (gasneti_mynode == args->srcnode) {
        gasneti_sync_reads();

        for (child = 0; child < child_count; child++) {
          
	  gasnete_coll_p2p_signalling_put(op, children[child], 
					  (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                          args->src, args->nbytes, 0, 1);
	  
	}
        /* local bcast*/
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     args->src, args->nbytes);
        
        
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	
	for (child = 0; child < child_count; child++) {
          
	  gasnete_coll_p2p_signalling_put(op, children[child], 
                                               (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                               (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                               args->nbytes, 0, 1);
          
	}
        /* local bcast*/
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, args->nbytes);
        
      } else {
	break;	/* Waiting for parent to push data and signal */
      }
      data->state = 4;
      
      
    case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }
      data->state = 5;
      
    case 5:
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
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
                            gasnete_coll_tree_type_t tree_type,    
                            uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |*/
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) | 
                GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_TreePutScratch, options,
					    gasnete_coll_tree_init(tree_type, 
                                                                   gasnete_coll_image_node(srcimage), team
                                                                   GASNETE_THREAD_PASS),
                                            sequence GASNETE_THREAD_PASS);
}



static int gasnete_coll_pf_bcastM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
        gasnete_coll_handle_vec_t *handle_vec;
        size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNETE_COLL_BROADCAST_OP, op->flags);
        int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
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
        gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? gasnete_coll_my_images : gasnete_coll_total_images);
        


        data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
        handle_vec = data->private_data;
        handle_vec->num_handles = num_segs;
        handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
        dstlist = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));

        if(flags & GASNET_COLL_SINGLE) {
          for(i=0; i<num_segs-1; i++) {
            gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
            handle_vec->handles[i] = gasnete_coll_bcastM_TreePut(op->team, dstlist, srcproc, (int8_t*) args->src+sent_bytes, seg_size, flags, 
                                                                tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
            gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
            sent_bytes+=seg_size;
          }
          gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
          handle_vec->handles[i] = gasnete_coll_bcastM_TreePut(op->team, dstlist, srcproc, (int8_t*) args->src+i*seg_size, args->nbytes-sent_bytes, flags, 
                                                               tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        } else {
          for(i=0; i<num_segs-1; i++) {
            gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
            handle_vec->handles[i] = gasnete_coll_bcastM_TreePutScratch(op->team, dstlist, srcproc, (int8_t*) args->src+sent_bytes, seg_size, flags, 
                                                                 tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
            gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
            sent_bytes+=seg_size;
          }
          gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
          handle_vec->handles[i] = gasnete_coll_bcastM_TreePutScratch(op->team, dstlist, srcproc, (int8_t*) args->src+i*seg_size, args->nbytes-sent_bytes, flags, 
                                                               tree->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
        }
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
      if (!gasnete_coll_generic_outsync(data)) {
        break;
      }
        
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

        
extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePutSeg(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags,
                              gasnete_coll_tree_type_t tree_type,
                              uint32_t sequence
                              GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC | 
      GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_BROADCAST_OP, flags);
  uint32_t num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_TreePutSeg, options,
					    gasnete_coll_tree_init(tree_type, 
                                                                   gasnete_coll_image_node(srcimage), team
                                                                   GASNETE_THREAD_PASS), 
                                            num_segs
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst,
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode,
					   gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Get(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scat_Get, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:
      if (gasneti_mynode != args->srcnode) {
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
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->src, (gasneti_mynode + 1), nbytes);
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i, p += nbytes) {
	    gasnete_put_nbi_bulk(i, dst, (void *)p, nbytes GASNETE_THREAD_PASS);
	  }
	  /* Put to nodes to the "left" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->src, 0, nbytes);
	  for (i = 0; i < gasneti_mynode; ++i, p += nbytes) {
	    gasnete_put_nbi_bulk(i, dst, (void *)p, nbytes GASNETE_THREAD_PASS);
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dst,
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, nbytes),
				      nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);

  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Put(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scat_Put, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
/* uint8_t direct_put_ok = (!((op->flags & GASNET_COLL_IN_MYSYNC)||(op->flags & GASNET_COLL_LOCAL)) && 0); */
  uint8_t direct_put_ok = 0; /* XXX: temporarily disable direct puts to scratch to avoid race condition in which empty scratch sends too simultaneous clears*/
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
  	if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
	  break;
	}
        if (gasneti_mynode != args->srcnode) {
	  gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
	}
      }
      data->state = 3;
      
    case 3:
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      if (op->team->myrank == args->srcnode) {
        if(tree->geom->seq_dfs_order == 0 || args->dist!=args->nbytes) {
          for(p=1,i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            /* 
              reorder/compress the data into the scratch space only if dfs traversal of tree is not in a seq order
              or the dist is not equal to the transfer size
            */
            for(j=0; j<tree->geom->subtree_sizes[i]; j++, p++) {
                GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+args->nbytes*(p-1), 
                                              (int8_t*)args->src+args->dist*tree->geom->dfs_order[p], args->nbytes);
            }
            if(tree->geom->subtree_sizes[i] == 1  && direct_put_ok) {
              /* if i am sending to a leaf ... put it right where it needs to go */
              /* since i am using scratch space AMLongAsync is always safe*/
              gasnete_coll_p2p_signalling_put/*Async*/(op, children[i], 
                                                  args->dst, 
                                                  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+sent_bytes, 
                                                  args->nbytes*tree->geom->subtree_sizes[i], 0, 1);            
            } else {
              /* if i am sending to an internal node put it to scratch space */
              gasnete_coll_p2p_signalling_put/*Async*/(op, children[i], 
                                                  (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+sent_bytes, 
                                                  args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
            }
            sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes;
          }
          GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, gasnete_coll_scale_ptr(args->src,args->dist,(op->team->myrank)), args->nbytes);
	} else {
          gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
          /* no need to reorder the data ... send directy from the source buffer into the remote scratch/dest*/
          sent_bytes+=args->nbytes;
          for(i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              /* use AMLong*/
              gasnete_coll_p2p_signalling_put(op, children[i], 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              (int8_t*)args->src+sent_bytes, 
                                              args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
            } else {
              if(tree->geom->subtree_sizes[i] == 1  && direct_put_ok) {
                /* if i am sending to a leaf into dest*/
                /* Perform NB Put */
                gasnete_put_nbi_bulk(children[i], args->dst,(int8_t*)args->src+sent_bytes,
                                     args->nbytes*tree->geom->subtree_sizes[i] GASNETE_THREAD_PASS);                              
              } else {
                /* else if i am sending to internal node async long into scratch space*/
                gasnete_coll_p2p_signalling_putAsync(op, children[i], 
                                                     (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                     (int8_t*)args->src+sent_bytes, 
                                                     args->nbytes*tree->geom->subtree_sizes[i], 0, 1);              
                
              }
            }
            sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes;
          }
          GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, gasnete_coll_scale_ptr(args->src,args->dist,op->team->myrank), args->nbytes);
	  data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
          gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
        }
      } else if(child_count == 0  && direct_put_ok) {
        /* for leaves with out no/all sync ... fall through right through*/
        /* for out mysync the leave nodes will need to wait for the data to arrive*/
      } else if(data->p2p->state[0]){
        gasneti_sync_reads();
        /*skip the first slot of the input array since it is destined for me*/
        sent_bytes = args->nbytes;
        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
      /*    fprintf(stderr, "%d> sending to %d from %d to %d\n", gasneti_mynode, children[i], (int)(op->myscratchpos+sent_bytes), (int)(op->scratchpos[i])); */
          if(tree->geom->subtree_sizes[i] == 1  && direct_put_ok) {
            /* if i am sending to a leaf ... put it right where it needs to go */
              gasnete_coll_p2p_signalling_put/*Async*/(op, children[i], 
                                                  args->dst, 
                                                  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+sent_bytes, 
                                                  args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
          } else {
            /* if i am sending to a leaf ... put it right where it needs to go */
            gasnete_coll_p2p_signalling_put/*Async*/(op, children[i], 
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+sent_bytes, 
                                            args->nbytes*tree->geom->subtree_sizes[i], 0, 1);
          }
          sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes;
        }
        
        
        /* In the case of Mysync the data is always sent to the scratch space so copy it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, args->nbytes);
        
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
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        
        }
      data->state = 6;
    
    case 6: /*done*/    
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
 
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_TreePut(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, size_t dist, int flags, 
                      gasnete_coll_tree_type_t tree_type,    
                      uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = 
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
    GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
					 &gasnete_coll_pf_scat_TreePut, options,
                                         gasnete_coll_tree_init(tree_type,
                                                                gasnete_coll_image_node(srcimage), team
                                                                GASNETE_THREAD_PASS),
                                         sequence GASNETE_THREAD_PASS);
}  

static int gasnete_coll_pf_scat_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op) ) break;
      {
        gasnete_coll_handle_vec_t *handle_vec;
        size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNETE_COLL_SCATTER_OP, op->flags);
        int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
	int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        #if !GASNET_SEQ
        gasnet_image_t srcproc = args->srcimage;
        #else
	gasnet_node_t srcproc = args->srcnode;
        #endif
	size_t nbytes = args->nbytes;
        size_t sent_bytes=0;
        
	int i;
        

        data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
        handle_vec = data->private_data;
        handle_vec->num_handles = num_segs;
        handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
        
        for(i=0; i<num_segs - 1; i++) {
          /*ignore the handle returned*/
          handle_vec->handles[i] = gasnete_coll_scat_TreePut(op->team, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                    seg_size, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes += seg_size;
        }
       
        handle_vec->handles[i] = gasnete_coll_scat_TreePut(op->team, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                         args->nbytes-sent_bytes, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
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
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        
        gasneti_free(data->private_data);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_scat_TreePutSeg(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags, 
                          gasnete_coll_tree_type_t tree_type,    
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC | 
      GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_SCATTER_OP, flags);
  int num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scat_TreePutSeg, options,
                                         gasnete_coll_tree_init(tree_type,
                                                                gasnete_coll_image_node(srcimage), team
                                                                GASNETE_THREAD_PASS),
                                         num_segs GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
				   args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->private_data = gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
	data->handle = gasnete_geti(gasnete_synctype_nb, gasnete_coll_my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0), args->nbytes,
			  	    args->srcnode, 1, &(data->private_data),
				    gasnete_coll_my_images * args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Get(gasnet_team_handle_t team,
		       void * const dstlist[],
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, nbytes, flags,
					  &gasnete_coll_pf_scatM_Get, options,
					  NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:
      if (gasneti_mynode != args->srcnode) {
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
	srclist = gasneti_malloc(gasneti_nodes * sizeof(void *));
	data->private_data = srclist;

	/* Queue PUTIs in an NBI access region */
	/* XXX: is gasnete_puti(gasnete_synctype_nbi,...) correct non-tracing variant of puti ? */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  void **q;

	  /* Put to nodes to the "right" of ourself */
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(args->src,
			  			       gasnete_coll_all_offset[gasneti_mynode + 1],
						       nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, gasneti_mynode + 1);
	  q = &srclist[gasneti_mynode + 1];
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)src_addr;
	    gasnete_puti(gasnete_synctype_nbi, i, count, p, nbytes, 1, q, len GASNETE_THREAD_PASS);
	    src_addr += len;
	    p += count;
	    ++q;
	  }
	  /* Put to nodes to the "left" of ourself */
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(args->src, 0, nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, 0);
	  q = &srclist[0];
	  for (i = 0; i < gasneti_mynode; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)src_addr;
	    gasnete_puti(gasnete_synctype_nbi, i, count, p, nbytes, 1, q, len GASNETE_THREAD_PASS);
	    src_addr += len;
	    p += count;
	    ++q;
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, nbytes),
				   nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (gasneti_mynode == args->srcnode) {
        if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
        }
        gasneti_free(data->private_data);	/* the temporary srclist */
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Put(gasnet_team_handle_t team,
		       void * const dstlist[],
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, nbytes, flags,
					  &gasnete_coll_pf_scatM_Put, options,
					  NULL, sequence GASNETE_THREAD_PASS);
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
  	if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
	  break;
	}
        if (gasneti_mynode != args->srcnode) {
	  gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
	}
      }
      data->state = 3;
      
    case 3:
      
      if (op->team->myrank == args->srcnode) {
        gasneti_sync_reads();
        if(tree->geom->seq_dfs_order == 0 || args->dist!=args->nbytes) {
          for(p=1,i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            /* 
            reorder/compress the data into the scratch space only if dfs traversal of tree is not in a seq order
             or the dist is not equal to the transfer size
             */
            for(j=0; j<tree->geom->subtree_sizes[i]; j++, p++) {
              for(t=0; t<gasnete_coll_my_images; t++) {
                GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+
                                              op->myscratchpos+args->nbytes*((p-1)*gasnete_coll_my_images+t), 
                                              (int8_t*)args->src+args->dist*(tree->geom->dfs_order[p]*gasnete_coll_my_images+t), 
                                              args->nbytes);
              }
            }
            /* if i am sending to an internal node put it to scratch space */
            gasnete_coll_p2p_signalling_put(op, children[i], 
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+sent_bytes, 
                                            args->nbytes*tree->geom->subtree_sizes[i]*gasnete_coll_my_images, 0, 1);
            
            sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes*gasnete_coll_my_images;
          }
          /* local scatter */
          if(args->nbytes == args->dist) {
            gasnete_coll_local_scatter(gasnete_coll_my_images,
                                       &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                       gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
                                       args->nbytes);
          } else {
            void * const* dstlist = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
            void *temp_src = gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->dist);
            for(t=0; t<gasnete_coll_my_images; t++) {
              GASNETE_FAST_UNALIGNED_MEMCPY(dstlist[t],  
					    gasnete_coll_scale_ptr(temp_src, t, args->dist),
                                            args->nbytes);
            }
          }
        } else {
          /* no need to reorder the data ... send directy from the source buffer into the remote scratch/dest*/
          sent_bytes+=args->nbytes*gasnete_coll_my_images;
          for(i=0; i<child_count; i++) {
            gasnet_node_t child = children[i];
            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              /* use AMLong*/
              gasnete_coll_p2p_signalling_put(op, children[i], 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              (int8_t*)args->src+sent_bytes, 
                                              args->nbytes*tree->geom->subtree_sizes[i]*gasnete_coll_my_images, 0, 1);              
            } else {
              /* else if i am sending to internal node async long into scratch space*/
              gasnete_coll_p2p_signalling_put(op, children[i], 
                                                   (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                   (int8_t*)args->src+sent_bytes, 
                                                   args->nbytes*tree->geom->subtree_sizes[i]*gasnete_coll_my_images, 0, 1);              
                
              
            }
            sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes*gasnete_coll_my_images;
          }
          /* local scatter*/
          gasnete_coll_local_scatter(gasnete_coll_my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                     gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
                                     args->nbytes);
        }
      } else if(data->p2p->state[0]){
        gasneti_sync_reads();
        /*skip the first slot of the input array since it is destined for me*/
        sent_bytes = args->nbytes*gasnete_coll_my_images;
        for(i=0; i<child_count; i++) {
          gasnet_node_t child = children[i];
          gasnete_coll_p2p_signalling_put(op, child, 
                                               (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                               (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+sent_bytes, 
                                               args->nbytes*tree->geom->subtree_sizes[i]*gasnete_coll_my_images, 0, 1);
          
          sent_bytes+=tree->geom->subtree_sizes[i]*args->nbytes*gasnete_coll_my_images;
        }
        
        
        /* In the case of Mysync the data is always sent to the scratch space so copy it out*/
        gasnete_coll_local_scatter(gasnete_coll_my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                   (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                   args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      
      data->state = 4;
      
        
        
      case 4:	/* Final Out Barrier */
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        data->state = 5;
          
      case 5: /*done*/    
	gasnete_coll_free_scratch(op);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);

  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePut(gasnet_team_handle_t team,
                          void * const dstlist[],
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, size_t dist, int flags, 
                          gasnete_coll_tree_type_t tree_type,    
                          uint32_t sequence
                          GASNETE_THREAD_FARG)


{
  int options = 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P;
  
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
					 &gasnete_coll_pf_scatM_TreePut, options,
                                         gasnete_coll_tree_init(tree_type,
                                                                gasnete_coll_image_node(srcimage), team
                                                                GASNETE_THREAD_PASS),
                                         sequence GASNETE_THREAD_PASS);
}  

static int gasnete_coll_pf_scatM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op) ) break;
      {

        gasnete_coll_handle_vec_t *handle_vec;
        size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNETE_COLL_SCATTER_OP, op->flags);
        int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
	int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        #if !GASNET_SEQ
        gasnet_image_t srcproc = args->srcimage;
        #else
	gasnet_node_t srcproc = args->srcnode;
        #endif
        
        void ** dstlist;
	size_t nbytes = args->nbytes;
        size_t sent_bytes=0;
        gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? gasnete_coll_my_images : gasnete_coll_total_images);
	int i;


        data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
        handle_vec = data->private_data;
        handle_vec->num_handles = num_segs;
        handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
        dstlist = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));
        
        for(i=0; i<num_segs - 1; i++) {
          /*ignore the handle returned*/
          gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
          handle_vec->handles[i] = gasnete_coll_scatM_TreePut(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                             seg_size, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes += seg_size;
        }
        gasnete_coll_scale_ptrM(dstlist, args->dstlist, sent_bytes, 1, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_scatM_TreePut(op->team, dstlist, srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                           args->nbytes-sent_bytes, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
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
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        
        gasneti_free(data->private_data);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePutSeg(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags, 
                              gasnete_coll_tree_type_t tree_type,    
                              uint32_t sequence
                              GASNETE_THREAD_FARG) {
    int options = GASNETE_COLL_GENERIC_OPT_INSYNC | 
	GASNETE_COLL_GENERIC_OPT_OUTSYNC; 
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_SCATTER_OP, flags);
  int num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scatM_TreePutSeg, options,
                                         gasnete_coll_tree_init(tree_type,
                                                                gasnete_coll_image_node(srcimage), team
                                                                GASNETE_THREAD_PASS),
                                         num_segs GASNETE_THREAD_PASS);
  
}

#if 0
/* Segmented version of scatter to handle large transfers. 
XXX: currently synchronizes on EACH sub-collective rather than just once
*/
extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePutSeg(gasnet_team_handle_t team,
                              void * const dstlist[],
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, int flags, 
                             gasnete_coll_tree_type_t tree_type,    
                             uint32_t sequence
                             GASNETE_THREAD_FARG) {
  
  size_t sent_bytes=0,num_segs,i;
  int sub_flags;
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_SCATTER_OP, flags);
  void **  temp_dstlist;
  void ** dstlist_pass;
  gasnet_coll_handle_t ret;
  
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  if(num_segs == 1) {
    return gasnete_coll_scatM_TreePut(team, dstlist, srcimage, src, 
                                      nbytes, nbytes, flags, tree_type, sequence GASNETE_THREAD_PASS);
  } else {
    /*add in the COLL_AGGREGATE flag*/
    sub_flags = flags  | GASNET_COLL_AGGREGATE;
    temp_dstlist = (void**) gasneti_malloc(sizeof(void* const)*gasnete_coll_total_images);
    /*add in the COLL_AGGREGATE flag*/
    sub_flags = flags | GASNET_COLL_AGGREGATE;
      
    for(i=0; i<num_segs - 1; i++) {
      if(sent_bytes !=0) {
        gasnete_coll_scale_ptrM(temp_dstlist,dstlist,1,sent_bytes,gasnete_coll_total_images);
        dstlist_pass = temp_dstlist;
      } else {
        dstlist_pass = (void**) dstlist;
      }
      
      /*ignore the handle returned*/
      gasnete_coll_scatM_TreePut(team, dstlist_pass, srcimage, gasnete_coll_scale_ptr(src,1,sent_bytes), 
                                 seg_size, nbytes, sub_flags, tree_type, sequence GASNETE_THREAD_PASS);   
      sent_bytes += seg_size;
    }
    if(sent_bytes !=0) {
      gasnete_coll_scale_ptrM(temp_dstlist,dstlist,1,sent_bytes,gasnete_coll_total_images);
      dstlist_pass = temp_dstlist;
    } else {
      dstlist_pass = (void**) dstlist;
    }
    /* the last segment of the collective has the same flags as the user requested ones*/
    /* If the user requested OUT_ALLSYNC or COLL_AGGREGATE then it'll be passed right in*/
    sub_flags = flags;
    ret = gasnete_coll_scatM_TreePut(team, dstlist_pass, srcimage, gasnete_coll_scale_ptr(src,1,sent_bytes), 
                                       nbytes-sent_bytes, nbytes, sub_flags, tree_type, sequence GASNETE_THREAD_PASS);   
    gasneti_free(temp_dstlist);
    return ret;
  }
}
#endif

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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	/* Nothing to do */
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* Queue GETs in an NBI access region */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  int i;
	  uintptr_t p;

	  /* Get from nodes to the "right" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->dst, (gasneti_mynode + 1), args->nbytes);
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i, p += args->nbytes) {
	    gasnete_get_nbi_bulk((void *)p, i, args->src, args->nbytes GASNETE_THREAD_PASS);
	  }
	  /* Get from nodes to the "left" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->dst, 0, args->nbytes);
	  for (i = 0; i < gasneti_mynode; ++i, p += args->nbytes) {
	    gasnete_get_nbi_bulk((void *)p, i, args->src, args->nbytes GASNETE_THREAD_PASS);
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_Get(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_Get, options,
					NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->dstnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->handle = gasnete_put_nb_bulk(args->dstnode, 
					   gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_Put(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_Put, options,
					NULL, sequence GASNETE_THREAD_PASS);
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
/*  uint8_t direct_put_ok =(tree->geom->seq_dfs_order==1 && args->nbytes==args->dist && (op->flags & GASNET_COLL_SINGLE) && (op->flags & GASNET_COLL_OUT_NOSYNC) );*/
  uint8_t direct_put_ok =0; /*XXX: temporarily disable direct puts to destination until we fix the bug in the scratch space regarding multiple clears*/
  
  switch (data->state) {
    case 0:
       if(!gasnete_coll_scratch_alloc_nb(op GASNETE_THREAD_PASS))    break;
       data->state = 1;
     
    case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 2;
      
    case 2:	/* Local Data Movement */
      /* go up the tree with the data */
      /* copy my data into the start of the scratch space */
     
      if(child_count > 0) {
         if(direct_put_ok && gasneti_mynode==args->dstnode) {
          GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst,op->team->myrank,args->nbytes), 
                                         (int8_t*)args->src, args->nbytes);
        } else{
          GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        (int8_t*)args->src, args->nbytes);

        }
      } else if_pf(op->team->total_ranks == 1) {
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, 
                                      args->src, args->nbytes);
      }
      data->state = 3;
        
    case 3:
      /* wait for all my children to send data to me only if i am not the root in a seq_dfs_tree or mysync*/
      if(gasneti_mynode == args->dstnode && direct_put_ok ) {
        /* no need to wait*/
        
      } else if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < child_count) {
        break;
      }
      /* forward the data up to my parent if i am not the root node*/
      if(gasneti_mynode != args->dstnode) {
        if(child_count > 0) {
          gasneti_sync_reads();
          if(parent == args->dstnode && direct_put_ok) {
            /*put it right where it needs to go */
            gasnete_coll_p2p_counting_put/*Async*/(op, parent,
                                               (int8_t*)args->dst+(tree->geom->sibling_offset+1)*args->nbytes,
                                               (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                               args->nbytes*tree->geom->mysubtree_size);
            
          } else {
            gasnete_coll_p2p_counting_put/*Async*/(op, parent,
                                               (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset+1)*args->nbytes,
                                               (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                               args->nbytes*tree->geom->mysubtree_size);
          }
        } else {
          if(parent == args->dstnode && direct_put_ok) {
            /*put it right where it needs to go*/
            gasnete_coll_p2p_counting_put(op, parent,
                                          (int8_t*)args->dst+(tree->geom->sibling_offset+1)*args->nbytes,
                                          args->src,
                                          args->nbytes);          
            
          } else {
            gasnete_coll_p2p_counting_put(op, parent,
                                          (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset+1)*args->nbytes,
                                          args->src,
                                          args->nbytes);          
          }
        }
      } else if((tree->geom->seq_dfs_order==0 || args->nbytes!=args->dist) && op->team->total_ranks > 1){        
          /* Sync data movement */
          gasneti_sync_reads();
          /* reorder the information if i am not the root*/
            for(i=0; i<gasneti_nodes; i++) {
                /*used for temporary variables to aid GDB*/
              int8_t *curr_dest = args->dst;
              int8_t *curr_src = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
              GASNETE_FAST_UNALIGNED_MEMCPY(curr_dest+args->dist*tree->geom->dfs_order[i],
                                            curr_src+args->nbytes*i,
                                            args->nbytes);

            }
      } else if_pt(!direct_put_ok && op->team->total_ranks > 1) {
        /* no need to reorder but the data is in the scartch space so pull it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst,
                                     (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                      args->nbytes*gasneti_nodes);
        
      }  else { 
        /* Do Nothing. I am the root and the flags are OUT_NOSYNC or OUT_ALLSYNC*/
        /* and the tree is a seq_dfs order thus the data will be put directly where it needs to go */
        /* perform an out_allsync barrier if needed*/
      }
      data->state = 4;
          
    case 4: /*node level barrier*/
      /* go down the tree with the barrier again*/
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        expected_count = child_count + ((gasneti_mynode!=args->dstnode) ? 1 : 0);
        if(gasneti_mynode != args->dstnode) {
          if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < expected_count) {
              break;
          }
        }
        for(child=0; child<child_count; child++) {
            gasnete_coll_p2p_advance(op, children[child]);
        }
      }
      data->state = 5;
            
            
    case 5: /* done*/
          gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
          result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
          gasnete_coll_free_scratch(op);

  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_TreePut(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, size_t dist, int flags,  gasnete_coll_tree_type_t tree_type,  
                      uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		/*GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |*/ 
               GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
					&gasnete_coll_pf_gath_TreePut, options,
                                        gasnete_coll_tree_init(tree_type, 
                                                               gasnete_coll_image_node(dstimage), team
                                                               GASNETE_THREAD_PASS), sequence GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gath_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
        gasnete_coll_handle_vec_t *handle_vec;
        size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNETE_COLL_GATHER_OP, op->flags);
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

        data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
        handle_vec = data->private_data;
        handle_vec->num_handles = num_segs;
        handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
        
        for(i=0; i<num_segs - 1; i++) {
          /*ignore the handle returned*/
          handle_vec->handles[i] = gasnete_coll_gath_TreePut(op->team, dstproc, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , 
                                                             gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                             seg_size, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes += seg_size;
        }
        
        handle_vec->handles[i] = gasnete_coll_gath_TreePut(op->team, dstproc, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , 
                                                           gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                           args->nbytes-sent_bytes, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
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
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        
        gasneti_free(data->private_data);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gath_TreePutSeg(gasnet_team_handle_t team,
                             gasnet_image_t dstimage,
                             void *dst,
                             void *src,
                             size_t nbytes, int flags, 
                             gasnete_coll_tree_type_t tree_type,    
                             uint32_t sequence
                             GASNETE_THREAD_FARG) {
 int options = GASNETE_COLL_GENERIC_OPT_INSYNC | 
     GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_GATHER_OP, flags);
  int num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_TreePutSeg, options,
                                        gasnete_coll_tree_init(tree_type, 
                                                               gasnete_coll_image_node(dstimage), team
                                                               GASNETE_THREAD_PASS), num_segs GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
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
	  void ** dstlist = gasneti_malloc(gasneti_nodes * sizeof(void *));
	  data->private_data = dstlist;

	  /* Get from the "right" of ourself */
	  dst_addr = (uintptr_t)gasnete_coll_scale_ptr(args->dst,
			  			       gasnete_coll_all_offset[gasneti_mynode + 1],
						       nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->srclist, gasneti_mynode + 1);
	  q = &dstlist[gasneti_mynode + 1];
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)dst_addr;
	    gasnete_geti(gasnete_synctype_nbi, 1, q, len, i, count, p, nbytes GASNETE_THREAD_PASS);
	    dst_addr += len;
	    p += count;
	    ++q;
	  }
	  /* Get from nodes to the "left" of ourself */
	  dst_addr = (uintptr_t)args->dst;
	  dst_addr = (uintptr_t)gasnete_coll_scale_ptr(args->dst, 0, nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->srclist, 0);
	  q = &dstlist[0];
	  for (i = 0; i < gasneti_mynode; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)dst_addr;
	    gasnete_geti(gasnete_synctype_nbi, 1, q, len, i, count, p, nbytes GASNETE_THREAD_PASS);
	    dst_addr += len;
	    p += count;
	    ++q;
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, 0), nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (gasneti_mynode == args->dstnode) {
        if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
        }
        gasneti_free(data->private_data);	/* the temporary dstlist */
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_Get(gasnet_team_handle_t team,
		       gasnet_image_t dstimage, void *dst,
		       void * const srclist[],
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, nbytes, flags,
					 &gasnete_coll_pf_gathM_Get, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, 0), args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->private_data = gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes);
        /* use a vis operation to stride through all the local data*/
	data->handle = gasnete_puti(gasnete_synctype_nb, args->dstnode,
				    1, &(data->private_data), gasnete_coll_my_images * args->nbytes,
				    gasnete_coll_my_images, &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, 0),
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_Put(gasnet_team_handle_t team,
		       gasnet_image_t dstimage, void *dst,
		       void * const srclist[],
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, nbytes, flags,
					 &gasnete_coll_pf_gathM_Put, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 2;
      
    case 2:	/* Local Data Movement */
      /* perform a local gather into the scratch space*/
      gasnete_coll_local_gather(gasnete_coll_my_images,
                                (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
      data->state = 3;
        
    case 3:
      if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < child_count) {
        break;
      }
      /* forward the data up to my parent if i am not the root node*/
      if(gasneti_mynode != args->dstnode) {
          gasneti_sync_reads();
          gasnete_coll_p2p_counting_put(op, parent,
                                             (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(tree->geom->sibling_offset+1)*args->nbytes*gasnete_coll_my_images,
                                             (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                             args->nbytes*tree->geom->mysubtree_size*gasnete_coll_my_images);
      } else if(tree->geom->seq_dfs_order==0 || args->nbytes!=args->dist){    /* i am root... need to reorder*/    
        /* Sync data movement */
        gasneti_sync_reads();
        
        /* reorder the information if i am not the root*/
        for(i=0; i<gasneti_nodes; i++) {
          int8_t *curr_dest = (int8_t*)args->dst+args->dist*(tree->geom->dfs_order[i]*gasnete_coll_my_images);
          int8_t *curr_src = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+args->nbytes*i*gasnete_coll_my_images;
          for(j=0; j<gasnete_coll_my_images; j++) {
            /*used for temporary variables to aid GDB*/
            GASNETE_FAST_UNALIGNED_MEMCPY(curr_dest+args->dist*j,
                                          curr_src+args->nbytes*j,
                                          args->nbytes);
                
          }
        }
      } else { /* i am root but no need to reorder.. just one massive data read out of scratch*/
        /* Sync data movement */
        gasneti_sync_reads();
        /* no need to reorder but the data is in the scartch space so pull it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst,
                                      (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                      args->nbytes*gasneti_nodes*gasnete_coll_my_images);
            
      }
      data->state = 4;
          
    case 4: /*node level barrier*/
      /* go down the tree with the barrier again*/
      if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
        /* Sync data movement */
        gasneti_sync_reads();
        expected_count = child_count + ((gasneti_mynode!=args->dstnode) ? 1 : 0);
        if(gasneti_mynode != args->dstnode) {
          if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < expected_count) {
            break;
          }
        }
        for(child=0; child<child_count; child++) {
          gasnete_coll_p2p_advance(op, children[child]);
        }
      }
     data->state = 5;
          
    case 5: /* done*/
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);

      
  }
  return result;
  
}

extern gasnet_coll_handle_t
gasnete_coll_gathM_TreePut(gasnet_team_handle_t team,
                              gasnet_image_t dstimage, void *dst,
                              void * const srclist[],
                              size_t nbytes, size_t dist, int flags, 
                              gasnete_coll_tree_type_t tree_type,
                              uint32_t sequence
                              GASNETE_THREAD_FARG) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) | 
                GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH; 
  
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
					 &gasnete_coll_pf_gathM_TreePut, options,
					 gasnete_coll_tree_init(tree_type, 
                                                                gasnete_coll_image_node(dstimage), team
                                                                GASNETE_THREAD_PASS), sequence GASNETE_THREAD_PASS);
 
  
}

static int gasnete_coll_pf_gathM_TreePutSeg(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      
    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op) ) break;
      {
	  
        gasnete_coll_handle_vec_t *handle_vec;
        size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNETE_COLL_SCATTER_OP, op->flags);
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
        gasnet_node_t numaddrs = (op->flags & GASNET_COLL_LOCAL ? gasnete_coll_my_images : gasnete_coll_total_images);
	int i;


        data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t)+sizeof(void* const)*numaddrs);
        handle_vec = data->private_data;
        handle_vec->num_handles = num_segs;
        handle_vec->handles = gasneti_malloc(sizeof(gasnet_coll_handle_t)*num_segs);
        srclist = (void**) ((int8_t*) data->private_data + sizeof(gasnete_coll_handle_vec_t));
        
        for(i=0; i<num_segs - 1; i++) {
          /*ignore the handle returned*/
          gasnete_coll_scale_ptrM(srclist, args->srclist, sent_bytes, 1, numaddrs); 
          handle_vec->handles[i] = gasnete_coll_gathM_TreePut(op->team, dstproc, gasnete_coll_scale_ptr(args->dst,1,sent_bytes), srclist,
                                                              seg_size, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
          gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
          sent_bytes += seg_size;
        }
        gasnete_coll_scale_ptrM(srclist, args->srclist, sent_bytes, 1, numaddrs); 
        handle_vec->handles[i] = gasnete_coll_gathM_TreePut(op->team, dstproc, gasnete_coll_scale_ptr(args->dst,1,sent_bytes), srclist, 
                                                            args->nbytes-sent_bytes, args->nbytes, flags, data->tree_info->geom->tree_type, op->sequence+i+1 GASNETE_THREAD_PASS);   
        gasnete_coll_save_coll_handle(&handle_vec->handles[i] GASNETE_THREAD_PASS);
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
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        
        gasneti_free(data->private_data);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gathM_TreePutSeg(gasnet_team_handle_t team,
                              gasnet_image_t dstimage,
                              void *dst,
                              void * const srclist[],
                              size_t nbytes, int flags, 
                              gasnete_coll_tree_type_t tree_type,    
                              uint32_t sequence
                              GASNETE_THREAD_FARG) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC | 
                GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_GATHER_OP, flags);
  int num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, nbytes, flags,
                                          &gasnete_coll_pf_gathM_TreePutSeg, options,
                                          gasnete_coll_tree_init(tree_type,
                                                                 gasnete_coll_image_node(dstimage), team
                                                                 GASNETE_THREAD_PASS),
                                          num_segs GASNETE_THREAD_PASS);
  

}

#if 0
extern gasnet_coll_handle_t
gasnete_coll_gathM_TreePutSeg(gasnet_team_handle_t team,
                             gasnet_image_t dstimage,
                             void *dst,
                            void * const srclist[],
                             size_t nbytes, int flags, 
                             gasnete_coll_tree_type_t tree_type,    
                             uint32_t sequence
                             GASNETE_THREAD_FARG) {
  
  size_t sent_bytes=0,num_segs,i;
  int sub_flags;
  size_t seg_size = gasnete_coll_get_pipe_seg_size(team->autotune_info, GASNETE_COLL_GATHER_OP, flags);
  void **  temp_srclist;
  void **  srclist_pass;
  gasnet_coll_handle_t ret;
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  
  
    if(num_segs == 1) {
      return gasnete_coll_gathM_TreePut(team, dstimage, dst, srclist, 
                                       nbytes, nbytes, flags, tree_type, sequence GASNETE_THREAD_PASS);
    } else {
      /*add in the COLL_AGGREGATE flag*/
      sub_flags = flags| GASNET_COLL_AGGREGATE;
      temp_srclist = (void**) gasneti_malloc(sizeof(void* const)*gasnete_coll_total_images);
      for(i=0; i<num_segs - 1; i++) {
        if(sent_bytes !=0) {
          gasnete_coll_scale_ptrM(temp_srclist,srclist,1,sent_bytes,gasnete_coll_total_images);
          srclist_pass = temp_srclist;
        } else {
          srclist_pass = (void**) srclist;
        }
        
        /*ignore the handle returned*/
        gasnete_coll_gathM_TreePut(team, dstimage, gasnete_coll_scale_ptr(dst,1,sent_bytes), 
                                  srclist_pass, 
                                  seg_size, nbytes, sub_flags , tree_type, sequence GASNETE_THREAD_PASS);   
        sent_bytes += seg_size;
      }
      if(sent_bytes !=0) {
        gasnete_coll_scale_ptrM(temp_srclist,srclist,1,sent_bytes,gasnete_coll_total_images);
        srclist_pass = temp_srclist;
      } else {
        srclist_pass = (void**) srclist;
      }
      
      /* the last segment of the collective has the same flags as the user requested ones*/
      /* If the user requested OUT_ALLSYNC or COLL_AGGREGATE then it'll be passed right in*/
      sub_flags = flags;
      ret = gasnete_coll_gathM_TreePut(team, dstimage, gasnete_coll_scale_ptr(dst,1,sent_bytes), 
                                       srclist_pass, 
                                       nbytes-sent_bytes, nbytes, sub_flags, tree_type, sequence GASNETE_THREAD_PASS);   
      gasneti_free(temp_srclist);
      return ret;
    
    }
  
}
#endif

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
          !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;
      /* perform a Gather up a tree rooted at 0*/
    case 1:
#if 0
      /* go down the tree with the barrier*/
      if(!(op->flags & GASNET_COLL_IN_NOSYNC)) {
        if(gasneti_mynode !=  GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT ) {
          if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < 1) {
            break;
          }
        }
        for(child=0; child<child_count; child++) {
	  gasnete_coll_p2p_advance(op, children[child]);
	}
      }
#endif
      data->state = 2;
      
      
      
    case 2:	/* Initiate data movement */
      /* copy my data into the start of the scratch space */
      if(child_count > 0) {
        if(gasneti_mynode != GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT || tree->geom->seq_dfs_order) {
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
        if(gasneti_mynode != GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT) {
          int8_t *src = (child_count > 0 ? 
                          args->dst : args->src);
          int8_t* dest = (parent == GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT && !(tree->geom->seq_dfs_order) ?
                          (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0] : (int8_t*)args->dst);
          gasnete_coll_p2p_counting_put(op, parent, dest+(tree->geom->sibling_offset+1)*args->nbytes, src,
                                        args->nbytes*tree->geom->mysubtree_size);
        } else {        
          /* Sync data movement */
          /* XXX: THIS IS A BIG BOTTLENECK*/
          /* XXX: If the tree's DFS order is the sequential order of the nodes, this rearrrangment can be avoided*/
          /* XXX: However we leave it in here so that trees don't neccissarily have to have this condition for this algorithim to be correct*/
       
          if(!(tree->geom->seq_dfs_order)) {
            for(i=0; i<gasneti_nodes; i++) {
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
        if (gasneti_mynode == GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT) {
          for (child = 0; child < child_count; child++) {
            gasnete_coll_p2p_signalling_put(op, children[child], args->dst, 
                                            args->dst, args->nbytes*op->team->total_ranks, 0, 1);
          }
        } else if (data->p2p->state[0]) {
          
          gasneti_sync_reads();
          for (child = 0; child < child_count; child++) {
            gasnete_coll_p2p_signalling_put(op, children[child], args->dst, 
                                            args->dst, args->nbytes*op->team->total_ranks, 0, 1);
          }
        } else {
          break;	/* Waiting for parent to push data and signal */
        }
        data->state = 5;
        
        
      case 5:	/* Optional OUT barrier */
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
	/*free up the scratch space used by this op*/
        gasnete_coll_free_scratch(op);
        gasneti_free(data->private_data);
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
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
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
					    &gasnete_coll_pf_gall_TreePut, options,
					    gasnete_coll_tree_init(tree_type, 
                                                                   gasnete_coll_image_node(GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT), team
                                                                   GASNETE_THREAD_PASS), 
                                            gasnete_coll_total_images GASNETE_THREAD_PASS);
}
#undef GASNETE_COLL_TREE_ALL_GATHER_ALL_ROOT


/* This is a ring based all-gather*/
/* every processor sends their piece to the right and receives from the left. In (P-1) steps all processors will have all the data 
  Here the state will be used to describe which processor to recieve from.
  This algorithm is naturally IN_NOSYNC/OUT_NOSYNC and will only work for SINGLE/single-addr. However, 
  the algorithm can easily be modified to work for OUT_MYSYNC without using a full barrier. 
*/
#define GASNETE_COLL_MYDECMOD(A,DIFF) (((A)-(DIFF)) + ((A)<(DIFF) ? gasneti_nodes : 0))
static int gasnete_coll_pf_gall_RingPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int slot;
  gasnet_node_t neighbor = (gasneti_mynode == (gasneti_nodes-1) ? 0 : gasneti_mynode+1);
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(data)) {
      return 0;
    }
    data->state++;
  }   
  else if(data->state == gasneti_nodes) {
    /* If using MYSYNC Wait for the last put to finish*/
    if(op->flags & GASNET_COLL_OUT_MYSYNC) {
      if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < (gasneti_nodes-1)) {
        return 0;
      }
    }
    data->state++;
  } 
  /* State Nodes+1: Out Barrier (if needed)*/
  else if(data->state == gasneti_nodes+1) {
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }
    gasneti_free(data->private_data);
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);    
  }
  /* State [1,NODES) perform ring*/
  else if(data->state < gasneti_nodes) {
    /* Copy the data from source to dest*/
    if(data->state == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes), 
                                    (int8_t*)args->src, args->nbytes);
    }     /*Wait for the P2P counter to be at least state*/
    else if (gasneti_weakatomic_read(&(data->p2p->counter), 0) < (data->state-1)) {
      return 0;
    }
    /*Send to my neighbor from slot (MYNODE+(state-1))%NODES*/
    /* All puts except for the last one can be asynccounting put*/
    gasneti_sync_reads();

    if(data->state < (gasneti_nodes-1)) {      
      gasnete_coll_p2p_counting_putAsync(op, neighbor,
                                         gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(gasneti_mynode,(data->state-1)), args->nbytes),
                                         gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(gasneti_mynode,(data->state-1)), args->nbytes),
                                         args->nbytes); 
    } else {
      gasnete_coll_p2p_counting_put(op, neighbor,
                                    gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(gasneti_mynode,(data->state-1)), args->nbytes),
                                    gasnete_coll_scale_ptr(args->dst, GASNETE_COLL_MYDECMOD(gasneti_mynode,(data->state-1)), args->nbytes),
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
                          size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
    Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
                GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
					    &gasnete_coll_pf_gall_RingPut, options,
					    NULL, 
                                            gasnete_coll_total_images GASNETE_THREAD_PASS);
}
#endif



/*only works for COLL_SINGLE*/
static int gasnete_coll_pf_gall_FlatPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(data)) {
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
	  gasnete_put_nbi_bulk(dst, (int8_t*) args->dst + op->team->myrank*args->nbytes, args->src, args->nbytes GASNETE_THREAD_PASS);
	}
	for(dst=0; dst<op->team->myrank; dst++) {
	  /*send to threads below me*/
	  gasnete_put_nbi_bulk(dst, (int8_t*) args->dst + op->team->myrank*args->nbytes, args->src, args->nbytes GASNETE_THREAD_PASS);
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
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
       
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gall_FlatPut(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
  Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
					    &gasnete_coll_pf_gall_FlatPut, options,
					    NULL, 
                                            0 GASNETE_THREAD_PASS);
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
        !gasnete_coll_generic_insync(data)) {
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
      gasnete_coll_p2p_signalling_put(op, dstnode, 
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
    gasnete_coll_p2p_signalling_put(op, dstnode, 
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
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }

    /*free up the scratch space used by this op*/
    if(op->team->total_ranks > 1) gasnete_coll_free_scratch(op);    
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }

  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gall_Dissem(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
  Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
                GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  

  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
					    &gasnete_coll_pf_gall_Dissem, options,
					    NULL, 
                                            0 GASNETE_THREAD_PASS);
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
        !gasnete_coll_generic_insync(data)) {
      return 0;
    }
    /* local gather */
    gasnete_coll_local_gather(gasnete_coll_my_images, 
                              (int8_t*) GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags)+gasnete_coll_my_images*args->nbytes*op->team->myrank,
                              &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
    data->state++;
  }
  
  if(data->state == 1) {
    int dst;
    int8_t *mysrc = (int8_t*) GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags)+gasnete_coll_my_images*args->nbytes*op->team->myrank;
    for(dst=op->team->myrank+1; dst<op->team->total_ranks; dst++) {
      /* send to threads above me*/
      gasnete_coll_p2p_counting_put(op, dst, 
                                    (int8_t*) GASNETE_COLL_1ST_IMAGE(args->dstlist,dst) + 
                                    op->team->myrank*args->nbytes*gasnete_coll_my_images, 
                                    mysrc, args->nbytes*gasnete_coll_my_images);
    }
    for(dst=0; dst<op->team->myrank; dst++) {
      /*send to threads below me*/
      gasnete_coll_p2p_counting_put(op, dst, 
                                    (int8_t*) GASNETE_COLL_1ST_IMAGE(args->dstlist,dst) + 
                                    op->team->myrank*args->nbytes*gasnete_coll_my_images, 
                                    mysrc, args->nbytes*gasnete_coll_my_images);
    }
    
    data->state++;
  }
  
  if(data->state == 2) {
    /* wait for all puts to finish*/
    if(gasneti_weakatomic_read(&(data->p2p->counter), 0) < op->team->total_ranks-1) return 0;
    gasneti_sync_reads();
    if_pt(gasnete_coll_my_images > 1) {
      gasnete_coll_local_broadcast(gasnete_coll_my_images-1, &args->dstlist[(op->flags & GASNET_COLL_LOCAL ? 0 : gasnete_coll_my_offset)+1], 
                                   GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                   gasnete_coll_total_images*args->nbytes);
    }
    data->state++;
  }
  
  if(data->state == 3) {
    /* out barrier and cleanup*/
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gallM_FlatPut(gasnet_team_handle_t team,
                           void * const dstlist[], void * const srclist[],
                           size_t nbytes, int flags, uint32_t sequence
                           GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
  Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC))|
                GASNETE_COLL_GENERIC_OPT_P2P;
  
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
					    &gasnete_coll_pf_gallM_FlatPut, options,
					    NULL, 
                                            0 GASNETE_THREAD_PASS);
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
        !gasnete_coll_generic_insync(data)) {
      return 0;
    }
    
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    
    /*Gather the data into the start of the scratch space*/
    gasnete_coll_local_gather(gasnete_coll_my_images, 
                              (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                              &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);

    data->state++;
  }
  
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1 && op->team->total_ranks!=1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = gasnete_coll_my_images*args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
  
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasneti_sync_reads();
      gasnete_coll_p2p_signalling_put(op, dstnode, 
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
    size_t curr_len = gasnete_coll_my_images*args->nbytes*(nblk);
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasneti_sync_reads();
    gasnete_coll_p2p_signalling_put(op, dstnode, 
                                    (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+
                                    gasnete_coll_my_images*(1<<phase)*args->nbytes,
                                    (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                    curr_len, phase, 1);
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1 || (op->team->total_ranks==1 && data->state==2)) {
    uint32_t phase = (data->state-2)/2; int i; 
    if(op->team->total_ranks > 1 && data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
 


    gasneti_sync_reads();
    /*rotate the data around*/
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*)GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags)+gasnete_coll_my_images*args->nbytes*op->team->myrank, 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        gasnete_coll_my_images*args->nbytes*(op->team->total_ranks-op->team->myrank));
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+gasnete_coll_my_images*args->nbytes*(op->team->total_ranks-op->team->myrank), 
                                        gasnete_coll_my_images*args->nbytes*op->team->myrank);
    
    if(gasnete_coll_my_images > 1) {
      gasnete_coll_local_broadcast(gasnete_coll_my_images-1, &args->dstlist[(op->flags & GASNET_COLL_LOCAL ? 0 : gasnete_coll_my_offset)+1], 
                                   GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                   gasnete_coll_total_images*args->nbytes);
    }
    
    
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+2 || (op->team->total_ranks==1 && data->state==3)) {
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }
    gasnete_coll_free_scratch(op);
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/

    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gallM_Dissem(gasnet_team_handle_t team,
                         void * const dstlist[], void * const srclist[],
                         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
  Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
					    &gasnete_coll_pf_gallM_Dissem, options,
					    NULL, 
                                            0 GASNETE_THREAD_PASS);
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
         !gasnete_coll_generic_insync(data)) {
       return result;
     }
     
    if(op->team->total_ranks == 1) {
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
        data->state = dissem->dissemination_phases*3+3;
        return 0; 
    }    
    /* perform local rotation*/
    GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)scratch2 + (op->team->total_ranks-op->team->myrank)*args->nbytes, 
                                  (int8_t*)args->src, op->team->myrank*args->nbytes);
	
    GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)scratch2, (int8_t*)args->src+op->team->myrank*args->nbytes,
                                  (op->team->total_ranks-op->team->myrank)*args->nbytes);
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
      for(j=0; j<h; j++) {
        /*XXX: switch to counting put for higher radices*/
        gasnete_coll_p2p_change_states(op, in_nodes[j], 1, phase*2, 1);
      }
      data->state++;
    }
    if((data->state-2) % 3 == 1) {
      /*XXX: Need atomic counter read for higher radices*/
      if(data->p2p->state[phase*2] != h) return 0;
      for(j=0; j<h; j++) {
	destnode = out_nodes[j];
	nblocks = 
	  gasnete_coll_pack_all_to_all_msg(scratch2, (int8_t*)scratch1+IDX_EXPR,args->nbytes,
					   phase, dissem->dissemination_radix, j+1, op->team->total_ranks);
	gasnete_coll_p2p_signalling_put(op, destnode, 
					(int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR, (int8_t*)scratch1+IDX_EXPR,
					nblocks*args->nbytes, phase*2+1, 1);
      }
      /*once all the change the state and return 0*/
      /*let the poll function bring us back here*/
      data->state++;
      return 0;
    } 
    if((data->state-2) % 3 == 2) { /*receive in odd sub phases*/
      /*wait for all the states to trip*/
      /*need to change this to an atomic state increment to do this properly for radix>2*/
      if(data->p2p->state[phase*2+1] == h) {
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
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }

    /*free up the scratch space used by this op*/
    if(op->team->total_ranks != 1) gasnete_coll_free_scratch(op);    
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_exchg_Dissem(gasnet_team_handle_t team,
			void *dst, void *src,
			size_t nbytes, int flags, uint32_t sequence
			GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
                GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
					  &gasnete_coll_pf_exchg_Dissem, options,
					  NULL, gasnete_coll_fetch_dissemination(2,team), 0 GASNETE_THREAD_PASS);
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
  scratch2 = (int8_t*)scratch1 + ((args->nbytes*gasnete_coll_my_images*gasnete_coll_my_images)*dissem->max_dissem_blocks)*((2)*(dissem->dissemination_radix-1));
   
  if(data->state == 1) {
    int i,j,k=0;
    int8_t **out_ptr;
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) ||
        !gasnete_coll_generic_insync(data)) {
      return result;
    }
      
    /* perform local rotation and gather*/
    data->private_data = (void**) gasneti_malloc(sizeof(void*)*gasnete_coll_my_images);
    
    for(i=0; i<op->team->total_ranks; i++) {
      int i_node = i;
      int position = ((i_node < op->team->myrank) ? 
                      op->team->total_ranks + (i_node - op->team->myrank) : 
                      i_node-op->team->myrank);
      for(j=0; j<gasnete_coll_my_images; j++) {
        gasnete_coll_scale_ptrM((void**) data->private_data, 
                                &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags),
                                (i*gasnete_coll_my_images+j)*args->nbytes, 1, gasnete_coll_my_images);
        gasnete_coll_local_gather(gasnete_coll_my_images, 
                                  (int8_t*) scratch2+position*(gasnete_coll_my_images*gasnete_coll_my_images*args->nbytes)+j*gasnete_coll_my_images*args->nbytes,
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
    offset = GASNETE_COLL_DISSEM_MAX_BLOCKS(dissem)*args->nbytes*gasnete_coll_my_images*gasnete_coll_my_images;
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
        gasnete_coll_p2p_change_states(op, in_nodes[j], 1, phase*2, 1);
      }
      data->state++;
    }
    if((data->state-2) % 3 == 1) {
      if(data->p2p->state[phase*2] != h) return 0;

      for(j=0; j<h; j++) {
	destnode = out_nodes[j];
	nblocks = 
	  gasnete_coll_pack_all_to_all_msg(scratch2, (int8_t*)scratch1+IDX_EXPR,args->nbytes*gasnete_coll_my_images*gasnete_coll_my_images,
					   phase, dissem->dissemination_radix, j+1, op->team->total_ranks);
        gasnete_coll_p2p_signalling_put(op, destnode, 
					(int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR, (int8_t*)scratch1+IDX_EXPR,
					nblocks*args->nbytes*gasnete_coll_my_images*gasnete_coll_my_images, phase*2+1, 1);
      }
      /*once all the change the state and return 0*/
      /*let the poll function bring us back here*/
      data->state++;
    } 
    if((data->state-2) % 3 == 2) { /*receive in odd sub phases*/
      /*wait for all the states to trip*/
      /*need to change this to an atomic state increment to do this properly for radix>2*/
      if(data->p2p->state[phase*2+1] == h) {
        gasneti_sync_reads();
	for(j=0; j<h; j++) {
	  gasnete_coll_unpack_all_to_all_msg((int8_t*)scratch1+IDXP1_EXPR, (int8_t*)scratch2, args->nbytes*gasnete_coll_my_images*gasnete_coll_my_images, phase,
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
      

      gasnete_coll_scale_ptrM((void**) data->private_data, &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), i*gasnete_coll_my_images,
                              args->nbytes, gasnete_coll_my_images);
      gasnete_coll_local_scatter(gasnete_coll_my_images,
                                 (void* const*) data->private_data, 
                                 (int8_t*) scratch2+srcnode*gasnete_coll_my_images*gasnete_coll_my_images*args->nbytes, args->nbytes*gasnete_coll_my_images);
    }
    data->state ++;
    
  }
  if(data->state == dissem->dissemination_phases*3+3) {
    if (!gasnete_coll_generic_outsync(data)) {
      return 0;
    }

    gasnete_coll_free_scratch(op);
    gasneti_free(data->private_data);
    data->private_data = NULL;    
    gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    /*free up the scratch space used by this op*/
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_exchgM_Dissem(gasnet_team_handle_t team,
			 void * const dstlist[], void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
			 GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
                 GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
                 GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
					  &gasnete_coll_pf_exchgM_Dissem, options,
					  NULL, gasnete_coll_fetch_dissemination(2,team), 0 GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/


