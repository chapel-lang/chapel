/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_eager.c $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_trees.h>
#include <coll/gasnet_scratch.h>
#include <coll/gasnet_autotune_internal.h>
#include <gasnet_vis.h>


#define FOLD_OUT_BARRIER 1

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */


/* bcast Eager: root node performs carefully ordered eager puts */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_Eager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
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
        
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_BCAST_ALG(Eager)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));
  
  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_min);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_Eager, options,
                                           NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}


/* bcast TreeEager */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
/* Naturally IN_MYSYNC, OUT_MYSYNC */
/* Max size is the eager limit */
static int gasnete_coll_pf_bcast_TreeEager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  int result = 0;
  int child;
  
  switch (data->state) {
    case 0:	/* Thread barrier */
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      
      data->state = 1; GASNETI_FALLTHROUGH
      
      case 1:	/* Optional IN barrier over the SAME tree */
      if ((op->flags & GASNET_COLL_IN_ALLSYNC) &&
          !gasnete_coll_generic_upsync_acq(op, args->srcnode, 0, child_count)) {
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        for (child=0;child<child_count; child++){
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), args->src, args->nbytes);
        }
        
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
        
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
        for (child=0;child<child_count;child++) {
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), args->dst, args->nbytes);
        }
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
      case 3: /*optional out barrier over the same tree*/
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      
      data->state = 4; GASNETI_FALLTHROUGH
      
      case 4: /*done*/
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_BCAST_ALG(TreeEager)
{
  int options =
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_min);
 
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreeEager, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type,
                                                                              srcimage, team),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_Eager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
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
        
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst,
                                            gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
                                            args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_SCATTER_ALG(Eager)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scat_Eager, options,
                                         NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

static int gasnete_coll_pf_scat_TreeEager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  int result = 0;
  int child;
  
  switch (data->state) {
    case 0:	/* Thread barrier */
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      
      data->state = 1; GASNETI_FALLTHROUGH
      
    case 1:	/* Optional IN barrier over the SAME tree */
      if ((op->flags & GASNET_COLL_IN_ALLSYNC) &&
          !gasnete_coll_generic_upsync_acq(op, args->srcnode, 0, child_count)) {
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
    case 2:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        uint8_t *src;
      
        if(op->team->myrank !=0) {
          gasnete_coll_local_rotate_left(data->p2p->data, args->src, args->nbytes, op->team->total_ranks, geom->rotation_points[0]);
          src = (uint8_t*) data->p2p->data;
        } else {
          src = (uint8_t*) args->src;
        }
        for (child=0;child<child_count; child++){
          int8_t *send_arr = gasnete_coll_scale_ptr(src,(geom->child_offset[child]+1),args->nbytes);
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]),
                                          send_arr, args->nbytes*geom->subtree_sizes[child]);
        }
        
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, src, args->nbytes);
      } else if (data->p2p->state[0]) {
        uint8_t *src;
        gasneti_sync_reads();
        for (child=0;child<child_count;child++) {
          src = gasnete_coll_scale_ptr(data->p2p->data,(geom->child_offset[child]+1), args->nbytes);
          gasnete_coll_p2p_eager_put_tree(op, GASNETE_COLL_REL2ACT(op->team,children[child]), 
                                          src, args->nbytes*geom->subtree_sizes[child]);
        }
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);

      } else {
        break;	/* Stalled until data arrives */
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
    case 3: /*optional out barrier over the same tree*/
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      
      data->state = 4; GASNETI_FALLTHROUGH
      
    case 4: /*done*/
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_SCATTER_ALG(TreeEager)
{
  int options =
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(gasnete_coll_p2p_eager_scale>= nbytes);
  gasneti_assert(dist == nbytes);
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                           &gasnete_coll_pf_scat_TreeEager, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type,
                                                                              srcimage, team),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETI_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gath_Eager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      
      /* Initiate data movement */
      if (op->team->myrank != args->dstnode) {
        gasnete_coll_p2p_eager_put(op, GASNETE_COLL_REL2ACT(op->team, args->dstnode), args->src, args->nbytes, op->team->myrank, 1);
      } else {
        GASNETI_MEMCPY_SAFE_IDENTICAL(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
                                            args->src, args->nbytes);
        data->p2p->state[op->team->myrank] = 2;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
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
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
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
                                        NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

static int gasnete_coll_pf_gath_TreeEager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  gex_Rank_t parent = GASNETE_COLL_TREE_GEOM_PARENT(geom);
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      
      if (child_count > 0) {
        GASNETI_MEMCPY_SAFE_IDENTICAL(data->p2p->data, args->src, args->nbytes);
      }  
      
      data->state = 1; GASNETI_FALLTHROUGH
      
      
    case 1:	/* Complete data movement */
      if(child_count > 0) {
        if (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != child_count) {
          break;
        }
        if(op->team->myrank != args->dstnode) {
          gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, parent), data->p2p->data,
                                     args->nbytes*geom->mysubtree_size, args->nbytes, (geom->sibling_offset+1), 0);
        } else {
          gasneti_assert(geom->num_rotations==1);
          gasnete_coll_local_rotate_right(args->dst,data->p2p->data, 
                                          args->nbytes, op->team->total_ranks, geom->rotation_points[0]);
        }
      } else if(op->team->myrank !=args->dstnode){

        gasnete_coll_p2p_counting_eager_put(op, GASNETE_COLL_REL2ACT(op->team, parent), args->src,
                                   args->nbytes, args->nbytes, (geom->sibling_offset+1),0);
      } else {
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
      }
        
      data->state = 2; GASNETI_FALLTHROUGH
      
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
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
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
                                        gasnete_coll_local_tree_geom_fetch(coll_params->tree_type,
                                                                           dstimage, team),
                                        sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */
static int gasnete_coll_pf_gall_FlatEagerPut(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    data->state++;
  }
  
  if(data->state == 1) {
    gex_Rank_t dst;
    
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
    GASNETI_MEMCPY_SAFE_IDENTICAL((int8_t*) data->p2p->data + op->team->myrank*args->nbytes, 
                                        args->src, args->nbytes);
    
    data->state++;
  }
  
  if(data->state == 2) {
    /* sync all the handles for the puts*/
    if (op->team->total_ranks > 1 && (gasneti_weakatomic_read(&data->p2p->counter[0], 0) != (op->team->total_ranks-1))) {
      return 0;
    }
    GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, data->p2p->data, args->nbytes*op->team->total_ranks);
    data->state++;
  }
  
  if(data->state == 3) {
    /* out barrier and cleanup*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALL_ALG(FlatEagerPut)
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
                                            sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

static int gasnete_coll_pf_gall_EagerDissem(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  
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
    gex_Rank_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
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
    gex_Rank_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
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
    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALL_ALG(EagerDissem)
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
                                            sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/*---------------------------------------------------------------------------------*/
