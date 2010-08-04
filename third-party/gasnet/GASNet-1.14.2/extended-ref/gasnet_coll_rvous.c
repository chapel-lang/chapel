/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_rvous.c,v $
 *     $Date: 2009/10/15 19:02:11 $
 * $Revision: 1.67 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_coll.h>


#include <gasnet_coll_internal.h>
#include <gasnet_coll_trees.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_autotune_internal.h>
#include <gasnet_vis.h>

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */

/* bcast RVGet: root node broadcasts address, others get from that address */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier and rendezvous */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1, op->team);	/* broadcast src address */
        GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        gasneti_sync_reads();
        data->handle = gasnete_get_nb_bulk(args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode),
                                           *(void **)data->p2p->data,
                                           args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
        break;
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
gasnete_coll_bcast_RVGet(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));

  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);


  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVGet, options,
                                           0, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*
  parent sends addresses to children who then perform a get to grab the data
  the collective is naturally IN_MYSYNC OUT_NOSYNC since
  a rendezvous is performed with the parent before the data is read
  however the parent has no idea when the data has been read
  to allow for an OUT_MYSYNC collective w/o a barrier we introduce a counter at the parent which
  is advanced for each child that has finished the gets

  THIS collective requires the source and destination to be in the segment since they will be both used as sources for a get
  will work for either SINGLE or LOCAL
  works as written for all synch flags
*/
  
  
static int gasnete_coll_pf_bcast_TreeRVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  
  int child;
  int result = 0;

  switch (data->state) {
  case 0:	/* Optional IN barrier and rendezvous */
    if (!gasnete_coll_generic_all_threads(data)) {
      break;
    }
    data->state = 1;
    
  case 1:
    if(op->flags & GASNET_COLL_IN_ALLSYNC) {
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
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->src, 0, 1);	/* broadcast src address to all the children*/
      }
      
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
    } else if (data->p2p->state[0]) {
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      gasneti_sync_reads();
      data->handle = gasnete_get_nb_bulk(args->dst, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),
                                         *(void **)data->p2p->data,
                                         args->nbytes GASNETE_THREAD_PASS);
      gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
    } else {
      break;
    }
    data->state = 3;
    
  case 3:	/* Sync data movement */
    if (data->handle != GASNET_INVALID_HANDLE) {
      break;
    }
    /*the get has finished now send a signal down the tree signalling the ok to get*/
    if(op->team->myrank != args->srcnode) {
      /*if the collective is an out mysync collective 
        then the parent needs to know taht the data transfers have finished 
        send a signal to our parent indicating that the get has finished by advancing counter 1
        in the case of out all sync the out barrier takes care of the synchronization 
      */
      if(op->flags & GASNET_COLL_OUT_MYSYNC) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),1);
      }
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, 0, 1);	/* broadcast src address to all the children*/
      }
      
    }
    data->state = 4;
    
  case 4:
    /*wait for all the children to respond*/
    if(op->flags & GASNET_COLL_OUT_MYSYNC) {
      if (gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) != child_count) {
        break;
      }
    }
    data->state = 5;
    
  case 5:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeRVGet(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = 
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);
  gasneti_assert(flags & GASNET_COLL_DST_IN_SEGMENT);

  
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreeRVGet, options,
                                           gasnete_coll_tree_init(coll_params->tree_type, 
                                                                  gasnete_coll_image_node(team, srcimage), team
                                                                  GASNETE_THREAD_PASS),
                                           sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* bcast RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_bcast_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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
    
  case 1:	/* Rendevous w/ root to pass addr */
    if (op->team->myrank == args->srcnode) {
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
    } else {
      /* Send our addr to root */
      gasnete_coll_p2p_send_rtr(op, data->p2p, op->team->myrank, args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode), args->nbytes);
    }
    data->state = 2;
    
  case 2:
    if (op->team->myrank == args->srcnode) {
      /* Root sends at most one AM per peer for each poll */
      gasnet_node_t i;
      int done = 1;
      for (i=0; i<op->team->total_ranks; ++i) {
        if (i == op->team->myrank) continue;
        done &= gasnete_coll_p2p_send_data(op, data->p2p, GASNETE_COLL_REL2ACT(op->team, i), i, args->src, args->nbytes);
      }
      if (!done) {break;}
    } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
      /* Not all data has arrived yet */
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
gasnete_coll_bcast_RVous(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVous, options,
                                           NULL, sequence, coll_params->num_params, coll_params->param_list
 GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcastM_nb() */

/* bcastM RVGet: root node broadcasts address, others get from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcastM_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1,op->team);	/* broadcast src address */
	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_broadcast(op->team->my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
				     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* Get 1st image only */
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
					   GASNETE_COLL_REL2ACT(op->team, args->srcnode), *(void **)data->p2p->data,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
        break;
      }
      data->state = 2;

    case 2:	/* Complete data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
      } else if (op->team->myrank != args->srcnode) {
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags);
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
gasnete_coll_bcastM_RVGet(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags,  gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)   |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC))|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_RVGet, options,
					    NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_bcastM_TreeRVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  
  int child;
  int result = 0;

  switch (data->state) {
  case 0:	/* Optional IN barrier and rendezvous */
    if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
      break;
    }
    data->state = 1;
    
  case 1:
    if(op->flags & GASNET_COLL_IN_ALLSYNC) {
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
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->src, 0, 1);	/* broadcast src address to all the children*/
      }
      gasnete_coll_local_broadcast(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
                                   args->src, args->nbytes);
      
    } else if (data->p2p->state[0]) {
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      gasneti_sync_reads();
      data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
                                         GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),
                                         *(void **)data->p2p->data,
                                         args->nbytes GASNETE_THREAD_PASS);
      gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
    } else {
      break;
    }
    data->state = 3;
    
  case 3:	/* Sync data movement */
    if (data->handle != GASNET_INVALID_HANDLE) {
      break;
    }
    /*the get has finished now send a signal down the tree signalling the ok to get*/
    if(op->team->myrank != args->srcnode) {
      /*if the collective is an out mysync collective 
        then the parent needs to know taht bbthe data transfers have finished 
        send a signal to our parent indicating that the get has finished by advancing counter 1
        in the case of out all sync the out barrier takes care of the synchronization 
      */
      if(op->flags & GASNET_COLL_OUT_MYSYNC) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom)),1);
      }
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(op->team, children[child]), GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags), 0, 1);	/* broadcast src address to all the children*/
      }
      gasnete_coll_local_broadcast(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
                                   GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags), args->nbytes);

    }
    data->state = 4;
    
  case 4:
    /*wait for all the children to respond*/
    if(op->flags & GASNET_COLL_OUT_MYSYNC) {
      if (gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) != child_count) {
        break;
      }
    }
    data->state = 5;
    
  case 5:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreeRVGet(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags, 
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETE_THREAD_FARG) {
  int options = 
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) | GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
                                            &gasnete_coll_pf_bcastM_TreeRVGet, options,
                                            gasnete_coll_tree_init(coll_params->tree_type, 
                                                                   gasnete_coll_image_node(team, srcimage), team
                                                                   GASNETE_THREAD_PASS),
                                            sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
  
}


/* bcastM RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_bcastM_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_p2p_t *p2p = data->p2p;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (op->team->myrank == args->srcnode) {
        gasnete_coll_local_broadcast(op->team->my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
                                     args->src, args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, op->team->myrank,
				  GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
				  GASNETE_COLL_REL2ACT(op->team, args->srcnode), args->nbytes);
      }
      data->state = 2;

    case 2:
      if (op->team->myrank == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<op->team->total_ranks; ++i) {
	  if (i == op->team->myrank) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, GASNETE_COLL_REL2ACT(op->team, i), i, args->src, args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      } else {
	/* All data has arrived - perform local copies */
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags);
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
gasnete_coll_bcastM_RVous(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags,  gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_RVous, options,
					    NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat RVGet: root node broadcasts address, others get from offsets from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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

    case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1, op->team);	/* broadcast src address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
				      gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
				      args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode),
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  op->team->myrank, args->nbytes),
					   args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
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
gasnete_coll_scat_RVGet(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, size_t dist, int flags, 
                        gasnete_coll_implementation_t coll_params,
                        uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
					 &gasnete_coll_pf_scat_RVGet, options,
					 NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* scat RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_scat_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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

    case 1:	/* Rendevous w/ root to pass addr */
      if (op->team->myrank == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
				      gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
				      args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, op->team->myrank, args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode), args->nbytes);
      }
      data->state = 2;

    case 2:
      if (op->team->myrank == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<op->team->total_ranks; ++i) {
	  if (i == op->team->myrank) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, GASNETE_COLL_REL2ACT(op->team, i), i,
					     gasnete_coll_scale_ptr(args->src, i, args->nbytes),
					     args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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
gasnete_coll_scat_RVous(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, size_t dist, int flags, 
                        gasnete_coll_implementation_t coll_params,
                        uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
					 &gasnete_coll_pf_scat_RVous, options,
					 NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatterM_nb() */

/* scatM RVGet: root node scatters address, others get from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scatM_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }

    case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1, op->team);	/* broadcast src address */
	gasnete_coll_local_scatter(op->team->my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
				   gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->nbytes),
				   args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->private_data = gasnete_coll_scale_ptr(*(void **)data->p2p->data,
					       op->team->my_offset,
					       args->nbytes);
	data->handle = gasnete_geti(gasnete_synctype_nb,
				    op->team->my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags), args->nbytes,
				    GASNETE_COLL_REL2ACT(op->team, args->srcnode), 1, &(data->private_data),
				    args->nbytes * op->team->my_images GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;

    case 2:
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
gasnete_coll_scatM_RVGet(gasnet_team_handle_t team,
                         void * const dstlist[],
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, size_t dist, int flags, 
                         gasnete_coll_implementation_t coll_params,    
                         uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
					  &gasnete_coll_pf_scatM_RVGet, options,
					  NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* scatM RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_scatM_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_p2p_t *p2p = data->p2p;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (op->team->myrank == args->srcnode) {
        gasnete_coll_local_scatter(op->team->my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
                                   gasnete_coll_scale_ptr(args->src, op->team->my_offset, args->nbytes),
                                   args->nbytes);
      } else {
	/* Send our addrs to root */
	gasneti_assert(gasnete_coll_p2p_eager_scale >= sizeof(gasnete_coll_p2p_send_struct_t));
	gasnete_coll_p2p_send_rtrM(op, data->p2p, op->team->my_offset,
				   &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->dstlist, op->flags),
				   GASNETE_COLL_REL2ACT(op->team, args->srcnode), args->nbytes, op->team->my_images);
      }
      data->state = 2;

    case 2:
      if (op->team->myrank == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<op->team->total_ranks; ++i) {
	  gasnet_image_t j,k;
	  if (i == op->team->myrank) continue;
	  k = op->team->all_offset[i] + op->team->all_images[i];
	  for (j = op->team->all_offset[i]; j < k; ++j) {
	    /* TODO: some pack/unpack of data (makes completion counter harder) */
	    done &= gasnete_coll_p2p_send_data(op, data->p2p, GASNETE_COLL_REL2ACT(op->team, i), j,
					       gasnete_coll_scale_ptr(args->src, j, args->nbytes),
					       args->nbytes);
	  }
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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
gasnete_coll_scatM_RVous(gasnet_team_handle_t team,
                         void * const dstlist[],
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, size_t dist, int flags, 
                         gasnete_coll_implementation_t coll_params,    uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, dist, flags,
					  &gasnete_coll_pf_scatM_RVous, options,
					  NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath RVPut: root node broadcasts addresses, others put to that address (plus offset) */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_gath_RVPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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

    case 1:	/* Initiate data movement */
      if (op->team->myrank == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1, op->team);	/* broadcast dst address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
				      args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_put_nb_bulk(GASNETE_COLL_REL2ACT(op->team, args->dstnode),
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  op->team->myrank, args->nbytes),
					   args->src, args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	  break;
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

GASNETE_COLL_DECLARE_GATHER_ALG(RVPut) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, dstimage));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_RVPut, options,
					NULL, sequence,coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* gath RVous: non-root nodes use AM Mediums to send to addrs provided by root */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_gath_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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

    case 1:	/* Root send addrs */
      if (op->team->myrank == args->dstnode) {
	gasnet_node_t i;
	for (i = 0; i < op->team->total_ranks; ++i) {
	  if (i == op->team->myrank) continue;
	  gasnete_coll_p2p_send_rtr(op, data->p2p, 0,
				    gasnete_coll_scale_ptr(args->dst, i, args->nbytes),
				    GASNETE_COLL_REL2ACT(op->team, i), args->nbytes);
	}
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
				      args->src, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (op->team->myrank != args->dstnode) {
	/* non-root nodes send at most one AM per poll */
	int done = gasnete_coll_p2p_send_data(op, data->p2p, GASNETE_COLL_REL2ACT(op->team, args->dstnode), 0, args->src, args->nbytes);
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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

GASNETE_COLL_DECLARE_GATHER_ALG(RVous) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
					&gasnete_coll_pf_gath_RVous, options,
					NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gatherM_nb() */

/* gathM RVPut: root node broadcasts addresses, others put to that address (plus offset) */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_gathM_RVPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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

    case 1:
      if (op->team->myrank == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1, op->team);	/* broadcast dst address */
	gasnete_coll_local_gather(op->team->my_images,
				  gasnete_coll_scale_ptr(args->dst, op->team->my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->srclist, op->flags), args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->private_data = gasnete_coll_scale_ptr(*(void **)data->p2p->data, op->team->my_offset, args->nbytes);
	data->handle = gasnete_puti(gasnete_synctype_nb, GASNETE_COLL_REL2ACT(op->team, args->dstnode),
				    1, &(data->private_data), args->nbytes * op->team->my_images,
				    op->team->my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->srclist, op->flags),
				    args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;

    case 2:
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

GASNETE_COLL_DECLARE_GATHERM_ALG(RVPut) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, dstimage));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
					 &gasnete_coll_pf_gathM_RVPut, options,
					 NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/* gathM RVous: non-root nodes use AM Mediums to send to addrs provided by root */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_gathM_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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

    case 1:	/* Root send addrs */
      if (op->team->myrank == args->dstnode) {
	void **tmp = gasneti_malloc(sizeof(void *) * op->team->total_images);
	gasnet_image_t j;
	gasnet_node_t i;
	for (j = 0; j < op->team->total_images; ++j) {
	  tmp[j] = gasnete_coll_scale_ptr(args->dst, j, args->nbytes);
	}
	for (i = 0; i < op->team->total_ranks; ++i) {
	  if (i == op->team->myrank) continue;
	  gasnete_coll_p2p_send_rtrM(op, data->p2p, 0, &GASNETE_COLL_1ST_IMAGE(op->team, tmp, i),
				     GASNETE_COLL_REL2ACT(op->team, i), args->nbytes, op->team->all_images[i]);
	}
	gasneti_free(tmp);
	gasnete_coll_local_gather(op->team->my_images,
				  gasnete_coll_scale_ptr(args->dst, op->team->my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->srclist, op->flags), args->nbytes);
      }
      data->state = 2;

    case 2:
      if (op->team->myrank != args->dstnode) {
	/* non-root nodes send at most one AM per image each poll */
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(op->team, args->srclist, op->flags);
	int done = 1;
	gasnet_image_t i;
	for (i = 0; i < op->team->my_images; ++i) {
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, GASNETE_COLL_REL2ACT(op->team, args->dstnode), i, p[i], args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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

GASNETE_COLL_DECLARE_GATHERM_ALG(RVous) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, dist, flags,
					 &gasnete_coll_pf_gathM_RVous, options,
					 NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_allM_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */
static int gasnete_coll_pf_exchg_RVPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
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
    
  case 1: /* send out all the destination addresses addresses*/
    gasnete_coll_p2p_eager_addr_all(op, args->dst, op->team->myrank, 1, op->team);	
    data->state = 2;
    
  case 2: /*wait for all addresses to arrive*/
    for(i=0; i<op->team->total_ranks; i++) {
      if(i==op->team->myrank) continue;
      if(!data->p2p->state[i]) return 0;
    }
    data->state = 3;
    /*if we go to here that means all addresses are ready*/
  case 3: /* fire off all the nonblocking puts*/
    gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
    /*put to the left of me*/
    for(i=op->team->myrank+1; i<op->team->total_ranks; i++) {
      gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,i), (int8_t*) ((void**)data->p2p->data)[i] + op->team->myrank*args->nbytes, (int8_t*) args->src+i*args->nbytes, args->nbytes GASNETE_THREAD_PASS);
    } 
    /*put to the right of me*/
    for(i=0; i<op->team->myrank; i++) {
      gasnete_put_nbi_bulk(GASNETE_COLL_REL2ACT(op->team,i), (int8_t*) ((void**)data->p2p->data)[i] + op->team->myrank*args->nbytes, (int8_t*) args->src+i*args->nbytes, args->nbytes GASNETE_THREAD_PASS);
    }
    data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
    gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK((int8_t*) args->dst + op->team->myrank*args->nbytes, 
                                        (int8_t*) args->src+op->team->myrank*args->nbytes, args->nbytes);
    data->state = 4;
  case 4: /* sync all the handles for the puts*/
    if (op->team->total_ranks > 1 && data->handle != GASNET_INVALID_HANDLE) {
      return 0;
    }
    data->state=5;
  case 5: /*final out barrier*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    data->state = 6;
  case 6: /*done*/
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_exchg_RVPut(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options =  GASNETE_COLL_GENERIC_OPT_P2P | 
    GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
    
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_RVPut, options,
                                          NULL, NULL, sequence, coll_params->num_params, coll_params->param_list GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchangeM_nb() */

/*---------------------------------------------------------------------------------*/
