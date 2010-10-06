/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_eager.c,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.70 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_coll.h>

#include <gasnet_coll_internal.h>
#include <gasnet_coll_trees.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_autotune_internal.h>
#include <gasnet_vis.h>


#define FOLD_OUT_BARRIER 1

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */


/* bcast Eager: root node performs carefully ordered eager puts */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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
      
      case 1:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        int i;
        for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, i), args->src, args->nbytes, 0, 1);
        }
        /* Send to nodes to the "left" of ourself */
        for (i = 0; i < op->team->myrank; ++i) {
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, i), args->src, args->nbytes, 0, 1);
        }
        
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 2;
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_bcast_Eager(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));
  
  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_min);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_Eager, options,
                                           NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


/* bcast TreeEager */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
/* Naturally IN_MYSYNC, OUT_MYSYNC */
/* Max size is the eager limit */
static int gasnete_coll_pf_bcast_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int barrier_count;
  int result = 0;
  int child;
  
  
  switch (data->state) {
    case 0:	/* Thread barrier */
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      
      data->state = 1;
      
      case 1:	/* Optional IN barrier over the SAME tree */
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
    	if (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != child_count) {
	  break;
	}
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
	}
      }
      data->state = 2;
      
      case 2:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        for (child=0;child<child_count; child++){
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), args->src, args->nbytes);
        }
        
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
        
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
        for (child=0;child<child_count;child++) {
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), args->dst, args->nbytes);
        }
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 3;
      
      case 3: /*optional out barrier over the same tree*/
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
gasnete_coll_bcast_TreeEager(gasnet_team_handle_t team,
                             void *dst,
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, int flags,
                             gasnete_coll_implementation_t coll_params,
                             uint32_t sequence
                             GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |*/
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_min);
 
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreeEager, options,
                                           gasnete_coll_tree_init(coll_params->tree_type,
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcastM_nb() */

/* bcastM Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcastM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;
  int i;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team,i), args->src, args->nbytes, 0, 1);
        }
        /* Send to nodes to the "left" of ourself */
        for (i = 0; i < op->team->myrank; ++i) {
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team,i), args->src, args->nbytes, 0,1);
        }
        
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     data->p2p->data, args->nbytes);
      } else {
        break;  /* Stalled until data arrives */
      }
      data->state = 2;
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Eager(gasnet_team_handle_t team,
                          void * const dstlist[],
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_Eager, options,
                                            NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_bcastM_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int child;
  
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
        break;
      }
      data->state = 1;
      
      case 1:
      if((op->flags & GASNET_COLL_IN_ALLSYNC)) {
        
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 2;
      
      
      
      case 2:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        gasneti_sync_reads();
        for(child=0; child<child_count; child++) {
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), args->src, args->nbytes);
        }
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        for (child=0;child<child_count;child++) {
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), 
                                          data->p2p->data, 
                                          args->nbytes);
        }
        
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                     data->p2p->data, args->nbytes);
      } else {
        break;  /* Stalled until data arrives */
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
gasnete_coll_bcastM_TreeEager(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags, 
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_TreeEager, options,
                                            gasnete_coll_tree_init(coll_params->tree_type,
                                                                   gasnete_coll_image_node(team,srcimage), team
                                                                   GASNETE_THREAD_PASS),
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      case 1:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        int i;
        uintptr_t src_addr;
        
        /* Send to nodes to the "right" of ourself */
        src_addr = (uintptr_t)args->src + args->nbytes * ((op->team->myrank) + 1);
        for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i, src_addr += args->nbytes) {
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, i), (void *)src_addr, args->nbytes, 0,1);
        }
        /* Send to nodes to the "left" of ourself */
        src_addr = (uintptr_t)args->src;
        for (i = 0; i < op->team->myrank; ++i, src_addr += args->nbytes) {
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, i), (void *)src_addr, args->nbytes, 0, 1);
        }
        
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst,
                                            gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
                                            args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 2;
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Eager(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, size_t dist, int flags, 
                        gasnete_coll_implementation_t coll_params,
                        uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scat_Eager, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_scat_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int barrier_count;
  int result = 0;
  int child;
  
  
  switch (data->state) {
    case 0:	/* Thread barrier */
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      
      data->state = 1;
      
    case 1:	/* Optional IN barrier over the SAME tree */
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
        if (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 2;
      
    case 2:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        uint8_t *src, *temp;
      
        if(op->team->myrank !=0) {
          gasnete_coll_local_rotate_left(data->p2p->data, args->src, args->nbytes, op->team->total_ranks, tree->geom->rotation_points[0]);
          src = (uint8_t*) data->p2p->data;
        } else {
          src = (uint8_t*) args->src;
        }
        for (child=0;child<child_count; child++){
          int8_t *send_arr = gasnete_coll_scale_ptr(src,(tree->geom->child_offset[child]+1),args->nbytes);
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]),
                                          send_arr, args->nbytes*tree->geom->subtree_sizes[child]);
          
        }
        
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, src, args->nbytes);
        
      } else if (data->p2p->state[0]) {
        uint8_t *src;
        gasneti_sync_reads();
        for (child=0;child<child_count;child++) {
          src = gasnete_coll_scale_ptr(data->p2p->data,(tree->geom->child_offset[child]+1), args->nbytes);
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), 
                                          src, args->nbytes*tree->geom->subtree_sizes[child]);
        }
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);

      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 3;
      
    case 3: /*optional out barrier over the same tree*/
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
gasnete_coll_scat_TreeEager(gasnet_team_handle_t team,
                             void *dst,
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, size_t dist, int flags,
                             gasnete_coll_implementation_t coll_params,
                             uint32_t sequence
                             GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |*/
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(gasnete_coll_p2p_eager_scale>= nbytes);
  gasneti_assert(dist == nbytes);
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                           &gasnete_coll_pf_scat_TreeEager, options,
                                           gasnete_coll_tree_init(coll_params->tree_type,
                                                                  gasnete_coll_image_node(team,srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatterM_nb() */

/* scatM Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scatM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      
      data->state = 1;
      
      case 1:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        const void * const src   = args->src;
        size_t nbytes = args->nbytes;
        uintptr_t src_addr;
        int i;
        
        /* Send to nodes to the "right" of ourself */
        if (op->team->myrank < op->team->total_ranks - 1) {
          src_addr = (uintptr_t)gasnete_coll_scale_ptr(src, op->team->all_offset[op->team->myrank + 1], nbytes);
          for (i = op->team->myrank + 1; i < op->team->total_ranks; ++i) {
            const size_t count = op->team->all_images[i];
            
            gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team,i), (void *)src_addr, count*nbytes, 0, 1);
            src_addr += count * nbytes;
          }
        }
        /* Send to nodes to the "left" of ourself */
        if (op->team->myrank > 0) {
          src_addr = (uintptr_t)gasnete_coll_scale_ptr(src, 0, nbytes);
          for (i = 0; i < op->team->myrank; ++i) {
            const size_t count = op->team->all_images[i];
            
            gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team,i), (void *)src_addr, count*nbytes, 0, 1);
            src_addr += count * nbytes;
          }
        }
        
        /* Local data movement */
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   gasnete_coll_scale_ptr(src, op->team->my_offset, nbytes), nbytes);
      } else if(data->p2p->state[0]) {
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   data->p2p->data, args->nbytes);
#if 0
        gasnete_coll_p2p_t *p2p = data->p2p;
        volatile uint32_t *state;
        size_t nbytes = args->nbytes;
        void * const *p;
        uintptr_t src_addr;
        int i, done;
        
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        
        done = 1;
        p = &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags);
        src_addr = (uintptr_t)(p2p->data);
        for (i = 0; i < op->team->my_images; ++i, ++p, src_addr += nbytes) {
          uint32_t s = state[i];
          
          if (s == 0) {
            /* Nothing received yet */
            done = 0;
          } else {
            /* Received but not yet copied into place */
            gasneti_sync_reads();
            GASNETE_FAST_UNALIGNED_MEMCPY(*p, (void *)src_addr, nbytes);
            state[i] = 2;
          }
        }
        
        if (!done) { break; }
#endif
      } else {
        break;
      }
      data->state = 2;
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Eager(gasnet_team_handle_t team,
                         void * const dstlist[],
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, size_t dist, int flags, 
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
  GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));
  
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_scatM_Eager, options,
                                          NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_scatM_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int barrier_count;
  int result = 0;
  int child;
  
  
  switch (data->state) {
    case 0:	/* Thread barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
        break;
      }
      
      data->state = 1;
      
    case 1:	/* Optional IN barrier over the SAME tree */
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
        if (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != child_count) {
          break;
        }
        if (op->team->myrank != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),0);
        }
      }
      data->state = 2;
      
    case 2:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        uint8_t *src, *temp;
        
        if(op->team->myrank !=0) {
          gasnete_coll_local_rotate_left(data->p2p->data, args->src, args->nbytes*op->team->my_images, op->team->total_ranks,
                                         tree->geom->rotation_points[0]);
          src = (uint8_t*) data->p2p->data;
        } else {
          src = (uint8_t*) args->src;
        }
        for (child=0;child<child_count; child++){
          int8_t *send_arr = gasnete_coll_scale_ptr(src,(tree->geom->child_offset[child]+1),args->nbytes*op->team->my_images);
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]),
                                          send_arr, args->nbytes*tree->geom->subtree_sizes[child]*op->team->my_images);
          
        }
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   src,
                                   args->nbytes);
        
      } else if (data->p2p->state[0]) {
        uint8_t *src;
        gasneti_sync_reads();
        for (child=0;child<child_count;child++) {
          src = gasnete_coll_scale_ptr(data->p2p->data,(tree->geom->child_offset[child]+1), args->nbytes*op->team->my_images);
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), 
                                          src, args->nbytes*tree->geom->subtree_sizes[child]*op->team->my_images);
        }
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   data->p2p->data,
                                   args->nbytes);
        
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 3;
      
    case 3: /*optional out barrier over the same tree*/
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
gasnete_coll_scatM_TreeEager(gasnet_team_handle_t team,
                            void * const dstlist[],
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, size_t dist, int flags,
                            gasnete_coll_implementation_t coll_params,
                            uint32_t sequence
                            GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |*/
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(gasnete_coll_p2p_eager_scale>= nbytes);
  gasneti_assert(dist == nbytes);
  
  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scatM_TreeEager, options,
                                         gasnete_coll_tree_init(coll_params->tree_type,
                                                                gasnete_coll_image_node(team,srcimage), team
                                                                GASNETE_THREAD_PASS),
                                         sequence, coll_params->num_params, coll_params->param_list
                                         GASNETE_THREAD_PASS);
}



/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gath_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      /* Initiate data movement */
      if (op->team->myrank != args->dstnode) {
        gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, args->dstnode), args->src, args->nbytes, op->team->myrank, 1);
      } else {
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
                                            args->src, args->nbytes);
        data->p2p->state[op->team->myrank] = 2;
      }
      
      case 1:	/* Complete data movement */
      if (op->team->myrank == args->dstnode) {
        gasnete_coll_p2p_t *p2p = data->p2p;
        volatile uint32_t *state;
        uintptr_t dst_addr, src_addr;
        size_t nbytes = args->nbytes;
        int i, done;
        
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        
        done = 1;
        dst_addr = (uintptr_t)(args->dst);
        src_addr = (uintptr_t)(p2p->data);
        for (i = 0; i < op->team->total_ranks; ++i, dst_addr += nbytes, src_addr += nbytes) {
          uint32_t s = state[i];
          
          if (s == 0) {
            /* Nothing received yet */
            done = 0;
          } else if (s == 1) {
            /* Received but not yet copied into place */
            gasneti_sync_reads();
            GASNETE_FAST_UNALIGNED_MEMCPY((void *)dst_addr, (void *)src_addr, nbytes);
            state[i] = 2;
          }
        }
        
        if (!done) { break; }
      }
      data->state = 2;
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(Eager)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
  GASNETE_COLL_GENERIC_OPT_P2P_IF(gasnete_coll_image_is_local(team, dstimage));
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
                                        &gasnete_coll_pf_gath_Eager, options,
                                        NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gath_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  int child;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      
      if (child_count > 0) {
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(data->p2p->data, args->src, args->nbytes);
      }  
      
      data->state = 1;
      
      
    case 1:	/* Complete data movement */
      if(child_count > 0) {
        int i;
        if (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != child_count) {
          break;
        }
        if(op->team->myrank != args->dstnode) {
          gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, parent), data->p2p->data,
                                     args->nbytes*tree->geom->mysubtree_size, args->nbytes, (tree->geom->sibling_offset+1), 0);
        } else {
          gasneti_assert(tree->geom->num_rotations==1);
          gasnete_coll_local_rotate_right(args->dst,data->p2p->data, 
                                          args->nbytes, op->team->total_ranks, tree->geom->rotation_points[0]);
        }
      } else if(op->team->myrank !=args->dstnode){

        gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, parent), args->src,
                                   args->nbytes, args->nbytes, (tree->geom->sibling_offset+1),0);
      } else {
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      }
        
      data->state = 2;
      
    case 2:	/* Optional OUT barrier */
#if FOLD_OUT_BARRIER
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
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreeEager)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_OUT_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
#endif
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
                                        &gasnete_coll_pf_gath_TreeEager, options,
                                        gasnete_coll_tree_init(coll_params->tree_type,
                                                               gasnete_coll_image_node(team,dstimage), team
                                                               GASNETE_THREAD_PASS), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gatherM_nb() */

/* gathM Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gathM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1;
      
      /* Initiate data movement */
      if (op->team->myrank != args->dstnode) {
        size_t nbytes = args->nbytes;
        void * tmp = gasneti_malloc(op->team->my_images * nbytes);
        gasnete_coll_local_gather(op->team->my_images, tmp,
                                  &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), nbytes);
        gasnete_coll_p2p_eager_putM(op, GASNETE_COLL_REL2ACT(op->team,args->dstnode), tmp, op->team->my_images,
                                    nbytes, op->team->my_offset, 1);
        gasneti_free(tmp);
      } else {
        volatile uint32_t *s;
        int i;
        
        gasnete_coll_local_gather(op->team->my_images,
                                  gasnete_coll_scale_ptr(args->dst, op->team->my_offset, args->nbytes),
                                  &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
        s = &(data->p2p->state[op->team->my_offset]);
        for (i = 0; i < op->team->my_images; ++i) {
          *(s++) = 2;
        }
      }
      
      case 1:	/* Complete data movement */
      if (op->team->myrank == args->dstnode) {
        gasnete_coll_p2p_t *p2p = data->p2p;
        volatile uint32_t *state;
        uintptr_t dst_addr, src_addr;
        size_t nbytes = args->nbytes;
        int i, done;
        
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        
        done = 1;
        dst_addr = (uintptr_t)(args->dst);
        src_addr = (uintptr_t)(p2p->data);
        for (i = 0; i < op->team->total_images; ++i, dst_addr += nbytes, src_addr += nbytes) {
          uint32_t s = state[i];
          
          if (s == 0) {
            /* Nothing received yet */
            done = 0;
          } else if (s == 1) {
            /* Received but not yet copied into place */
            gasneti_sync_reads();
            GASNETE_FAST_UNALIGNED_MEMCPY((void *)dst_addr, (void *)src_addr, nbytes);
            state[i] = 2;
          }
        }
        
        if (!done) { break; }
      }
      data->state = 2;
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHERM_ALG(Eager){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P_IF(gasnete_coll_image_is_local(team, dstimage));
  
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
                                         &gasnete_coll_pf_gathM_Eager, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gathM_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  gasnet_node_t parent = GASNETE_COLL_TREE_GEOM_PARENT(tree->geom);
  int child;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      
      gasnete_coll_local_gather(op->team->my_images, data->p2p->data, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), 
                                args->nbytes);
      
      data->state = 1;
      
      
    case 1:	/* Complete data movement */
    {
      int i;
      if (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != child_count) {
        break;
      }
      if(op->team->myrank != args->dstnode) {
        gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, parent), data->p2p->data,
                                            args->nbytes*tree->geom->mysubtree_size*op->team->my_images, args->nbytes*op->team->my_images, (tree->geom->sibling_offset+1), 0);
      } else {
        gasneti_assert(tree->geom->num_rotations==1);
        gasnete_coll_local_rotate_right(args->dst,data->p2p->data, 
                                        args->nbytes*op->team->my_images, op->team->total_ranks, tree->geom->rotation_points[0]);
      }
    } 
      data->state = 2;
      
    case 2:	/* Optional OUT barrier */
#if FOLD_OUT_BARRIER
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
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHERM_ALG(TreeEager){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_OUT_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
#endif
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  
  
  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
                                         &gasnete_coll_pf_gathM_TreeEager, options,
                                         gasnete_coll_tree_init(coll_params->tree_type,
                                                                gasnete_coll_image_node(team,dstimage), team
                                                                GASNETE_THREAD_PASS), sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */
static int gasnete_coll_pf_gall_FlatEagerPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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
      {
        for(dst=op->team->myrank+1; dst<op->team->total_ranks; dst++) {
          /* send to threads above me*/
          gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dst), args->src,
                                              args->nbytes, args->nbytes, op->team->myrank, 0);
        }
        for(dst=0; dst<op->team->myrank; dst++) {
          /*send to threads below me*/
          gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dst), args->src,
                                              args->nbytes, args->nbytes, op->team->myrank, 0);          
        }
      }
    }
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*) data->p2p->data + op->team->myrank*args->nbytes, 
                                        args->src, args->nbytes);
    
    data->state++;
  }
  
  if(data->state == 2) {
    /* sync all the handles for the puts*/
    if (op->team->total_ranks > 1 && (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != (op->team->total_ranks-1))) {
      return 0;
    }
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, data->p2p->data, args->nbytes*op->team->total_ranks);
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
gasnete_coll_gall_FlatEagerPut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, 
                          gasnete_coll_implementation_t coll_params,
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  

  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_scale);
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_FlatEagerPut, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gall_EagerDissem(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)data->p2p->data,
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
      gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dstnode), data->p2p->data,
                                          curr_len, 1, curr_len, phase);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(gasneti_weakatomic_read(&data->p2p->counter[phase], 0) == 1) {
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
    gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dstnode), data->p2p->data,
                                        curr_len, args->nbytes, (1<<phase), phase);

    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1) {
    uint32_t phase = (data->state-2)/2;
    if(gasneti_weakatomic_read(&data->p2p->counter[phase], 0) !=1) return 0; /*wait for the last transfer to finish*/
    /*rotate the data right to fix up the indices*/
    gasnete_coll_local_rotate_right(args->dst, data->p2p->data, args->nbytes, op->team->total_ranks, op->team->myrank);

    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*2+2) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gall_EagerDissem(gasnet_team_handle_t team,
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
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  

  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_scale);
  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_EagerDissem, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_allM_nb() */
static int gasnete_coll_pf_gallM_FlatEagerPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    data->state++;
  }
  
  if(data->state == 1) {
    gasnet_node_t dst;
    int8_t* mydata;
    if (!GASNETE_COLL_MAY_INIT_FOR(op)) return result;
    /*Gather the data into the start of the scratch space*/
    
    gasnete_coll_local_gather(op->team->my_images, 
                              gasnete_coll_scale_ptr(data->p2p->data, args->nbytes, op->team->myrank*op->team->my_images),
                              &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
    mydata = gasnete_coll_scale_ptr(data->p2p->data, args->nbytes, op->team->myrank*op->team->my_images);
    if_pt(op->team->total_ranks > 1) {
      {
        for(dst=op->team->myrank+1; dst<op->team->total_ranks; dst++) {
          /* send to threads above me*/
          gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dst), mydata,
                                              args->nbytes*op->team->my_images, args->nbytes, op->team->myrank*op->team->my_images, 0);
        }
        for(dst=0; dst<op->team->myrank; dst++) {
          /*send to threads below me*/
          gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dst), mydata,
                                              args->nbytes*op->team->my_images, args->nbytes, op->team->myrank*op->team->my_images, 0);          
        }
      }
    }

    
    data->state++;
  }
  
  if(data->state == 2) {
    /*wait for all the data to arrive*/
    if (op->team->total_ranks > 1 && (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != (op->team->total_ranks-1))) {
      return 0;
    }
    /*local broadcast*/
    gasnete_coll_local_broadcast(op->team->my_images, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), 
                                 data->p2p->data, op->team->total_images*args->nbytes);
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
gasnete_coll_gallM_FlatEagerPut(gasnet_team_handle_t team,
                               void * const dstlist[], void * const srclist[],
                               size_t nbytes, int flags, 
                               gasnete_coll_implementation_t coll_params,
                               uint32_t sequence
                               GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  

  gasneti_assert(nbytes*team->my_images <= gasnete_coll_p2p_eager_scale);
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
                                            &gasnete_coll_pf_gallM_FlatEagerPut, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_gallM_EagerDissem(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;
  int8_t *myscratch;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    data->state++;
  } 
  
  if(data->state == 1) {
    if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    if(op->team->total_ranks == 1) {
      gasnete_coll_local_gather(op->team->my_images, 
                                data->p2p->data,
                                &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);      
      gasnete_coll_local_broadcast(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags),
                                   data->p2p->data, op->team->total_images*args->nbytes);
      data->state = dissem->dissemination_phases*2+2;
    } else {
      /*memcpy the data into the start of the scratch space*/
      gasnete_coll_local_gather(op->team->my_images, 
                                data->p2p->data,
                                &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags), args->nbytes);
      
      data->state++;
    }
  }
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = op->team->my_images*args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gasnet_node_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dstnode), data->p2p->data,
                                          curr_len, 1, curr_len, phase);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(gasneti_weakatomic_read(&data->p2p->counter[phase], 0) == 1) {
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
    gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, dstnode), data->p2p->data,
                                        curr_len, op->team->my_images*args->nbytes, (1<<phase), phase);
    
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1) {
    uint32_t phase = (data->state-2)/2;
    if(gasneti_weakatomic_read(&data->p2p->counter[phase], 0) !=1) return 0; /*wait for the last transfer to finish*/
    /*rotate the data right to fix up the indices*/
    gasnete_coll_local_rotate_right(GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), data->p2p->data, op->team->my_images*args->nbytes, op->team->total_ranks, op->team->myrank);
    gasnete_coll_local_broadcast(op->team->my_images-1, ((void * const *) &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags))+1, 
                                 GASNETE_COLL_MY_1ST_IMAGE(op->team,args->dstlist, op->flags), op->team->total_images*args->nbytes);
    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*2+2) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}


extern gasnet_coll_handle_t
gasnete_coll_gallM_EagerDissem(gasnet_team_handle_t team,
                              void * const dstlist[], void * const srclist[],
                              size_t nbytes, int flags, 
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETE_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  

  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_scale);
  
  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
                                            &gasnete_coll_pf_gallM_EagerDissem, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchangeM_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_reduce_nb() */

static int gasnete_coll_pf_reduce_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduce);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      
      
      /* Initiate data movement */

      gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, args->dstnode), args->src, args->nbytes, op->team->myrank, 1);
      
      data->state = 1;
      
    case 1:	/* Complete data movement */
      if (op->team->myrank == args->dstnode) {
        gasnete_coll_p2p_t *p2p = data->p2p;
        volatile uint32_t *state;
        uintptr_t dst_addr, src_addr;
        size_t nbytes = args->nbytes;
        int i, done;
        gasnet_coll_reduce_fn_t reduce_fn = gasnete_coll_fn_tbl[args->func].fnptr;
        uint32_t red_fn_flags = gasnete_coll_fn_tbl[args->func].flags;
        uint32_t reduce_args = args->func_arg;
        static int first=1;
        
        gasneti_assert(p2p != NULL);
        gasneti_assert(p2p->state != NULL);
        state = data->p2p->state;
        gasneti_assert(p2p->data != NULL);
        
        done = 1;
        dst_addr = (uintptr_t)(args->dst);
        src_addr = (uintptr_t)(p2p->data);
        for (i = 0; i < op->team->total_ranks; ++i, src_addr += nbytes) {
          uint32_t s = state[i];
          
          if (s == 0) {
            /* Nothing received yet */
            done = 0;
          } else if (s == 1) {
            /* Received but not yet copied into place */
            gasneti_sync_reads();
            /*apply the reduction function*/
            if(first) {
              GASNETE_FAST_UNALIGNED_MEMCPY((void*) dst_addr, (void*) src_addr, args->nbytes); 
              first = 0;
            } else {
              (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                           (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
            }
            state[i] = 2;
          }
        }
        
        if (!done) { break; }
        first = 1;
      }
      data->state = 2;
      
    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_reduce_Eager(gasnet_team_handle_t team,
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
  gasneti_assert(gasnete_coll_p2p_eager_scale>= elem_size*elem_count);

  return gasnete_coll_generic_reduce_nb(team, dstimage, dst, src, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduce_Eager, options,
                                        NULL, sequence, 0, NULL, NULL GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_reduce_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  const gasnete_coll_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduce);
  gasnete_coll_p2p_t *p2p = data->p2p;
  int result = 0;
  uintptr_t dst_addr, src_addr;
  int i;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      if(op->team->myrank == args->dstnode) {
        GASNETE_FAST_UNALIGNED_MEMCPY((void*) args->dst, args->src, args->nbytes);
      } else if(child_count > 0) {
        GASNETE_FAST_UNALIGNED_MEMCPY((void*) p2p->data, args->src, args->nbytes);
      } 
      data->state = 1;
      
    case 1:	/* Complete data movement */
    
      if (child_count > 0) {
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
          dst_addr = (uintptr_t) p2p->data;
        }
        src_addr = ((uintptr_t)(p2p->data))+nbytes;
        for (i = 0; i < child_count; ++i, src_addr += nbytes) {
          uint32_t s = state[i+1];
          
          if (s == 0) {
            /* Nothing received yet */
            done = 0;
          } else if (s == 1) {
            /* Received but not yet copied into place */
            gasneti_sync_reads();
            /*apply the reduction function*/
            
            (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                         (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
            state[i+1] = 2;
          }
        }
        
        if (!done) { break; }
        if(op->team->myrank != args->dstnode) 
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), (void*) dst_addr, args->nbytes, tree->geom->sibling_id+1, 1);
      } else if(op->team->myrank != args->dstnode) {
        gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), args->src, args->nbytes, tree->geom->sibling_id+1, 1);
      }
      data->state = 2;
      
    case 2:	/* Optional OUT barrier */
#if FOLD_OUT_BARRIER
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
  }
  
  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_reduce_TreeEager(gasnet_team_handle_t team,
                          gasnet_image_t dstimage, void *dst,
                          void *src, size_t src_blksz, size_t src_offset,
                          size_t elem_size, size_t elem_count,
                          gasnet_coll_fn_handle_t func, int func_arg,
                          int flags, 
                          gasnete_coll_implementation_t coll_params,
                          uint32_t sequence
                          GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_OUT_BARRIER
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
#endif
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  gasneti_assert(gasnete_coll_p2p_eager_scale>= elem_size*elem_count);

  return gasnete_coll_generic_reduce_nb(team, dstimage, dst, src, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduce_TreeEager, options,
                                        gasnete_coll_tree_init(coll_params->tree_type,
                                                               gasnete_coll_image_node(team,dstimage), team
                                                               GASNETE_THREAD_PASS), sequence, coll_params->num_params, coll_params->param_list, NULL
                                        GASNETE_THREAD_PASS);
}



static int gasnete_coll_pf_reduceM_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  const gasnete_coll_reduceM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, reduceM);
  gasnete_coll_p2p_t *p2p = data->p2p;
  int result = 0;
  uintptr_t dst_addr, src_addr;
  int i;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS)||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      if(op->team->myrank == args->dstnode) {
        /*perform local reduction into the args->dst*/
        gasnete_coll_local_reduce(op->team->my_images, args->dst, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                  args->elem_size, args->elem_count, args->func, args->func_arg);
      } else  {
        /*perform local reduction into the p2p->data*/
        gasnete_coll_local_reduce(op->team->my_images, (void*) p2p->data, &GASNETE_COLL_MY_1ST_IMAGE(op->team,args->srclist, op->flags),
                                  args->elem_size, args->elem_count, args->func, args->func_arg);
      } 
      data->state = 1;
      
    case 1:	/* Complete data movement */
      
      if (child_count > 0) {
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
          dst_addr = (uintptr_t) p2p->data;
        }
        src_addr = ((uintptr_t)(p2p->data))+nbytes;
        for (i = 0; i < child_count; ++i, src_addr += nbytes) {
          uint32_t s = state[i+1];
          
          if (s == 0) {
            /* Nothing received yet */
            done = 0;
          } else if (s == 1) {
            /* Received but not yet copied into place */
            gasneti_sync_reads();
            /*apply the reduction function*/
            
            (*reduce_fn)((void*) dst_addr, args->elem_count, (void*) dst_addr, args->elem_count, 
                         (void*) src_addr, args->elem_size, red_fn_flags, reduce_args); 
            state[i+1] = 2;
          }
        }
        
        if (!done) { break; }
        if(op->team->myrank != args->dstnode) 
          gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), (void*) dst_addr, args->nbytes, tree->geom->sibling_id+1, 1);
      } else if(op->team->myrank != args->dstnode) {
        gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)), (void*) p2p->data, args->nbytes, tree->geom->sibling_id+1, 1);
      }
      data->state = 2;
     
    case 2:	/* Optional OUT barrier */
#if FOLD_OUT_BARRIER
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


  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_reduceM_TreeEager(gasnet_team_handle_t team,
                              gasnet_image_t dstimage, void *dst,
                              void * const srclist[], size_t src_blksz, size_t src_offset,
                              size_t elem_size, size_t elem_count,
                              gasnet_coll_fn_handle_t func, int func_arg,
                              int flags, 
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETE_THREAD_FARG){
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
#if !FOLD_OUT_BARRIER
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) | 
#endif
  GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  gasneti_assert(gasnete_coll_p2p_eager_scale>= elem_size*elem_count);
  
  return gasnete_coll_generic_reduceM_nb(team, dstimage, dst, srclist, src_blksz, src_offset,
                                        elem_size, elem_count, func, func_arg, flags, 
                                        &gasnete_coll_pf_reduceM_TreeEager, options,
                                        gasnete_coll_tree_init(coll_params->tree_type,
                                                               gasnete_coll_image_node(team,dstimage), team
                                                               GASNETE_THREAD_PASS), sequence, coll_params->num_params, coll_params->param_list, NULL
                                        GASNETE_THREAD_PASS);
}
