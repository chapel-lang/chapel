/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_rvous.c $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_trees.h>
#include <coll/gasnet_scratch.h>
#include <coll/gasnet_autotune_internal.h>
#include <gasnet_vis.h>

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */

/* bcast RVGet: root node broadcasts address, others get from that address */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_RVGet(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier and rendezvous */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      data->state = 1; GASNETI_FALLTHROUGH

    case 1:	/* Data movement */
      if (op->team->myrank == args->srcnode) {
        gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1, op->team);	/* broadcast src address */
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        data->handle = gasnete_get_nb(gasneti_THUNK_TM, args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode),
                                           *(void **)data->p2p->data,
                                           args->nbytes, 0 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&data->handle);
      } else {
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
  case 2:	/* Sync data movement */
    if (data->handle != GEX_EVENT_INVALID) {
      break;
    }
    data->state = 3; GASNETI_FALLTHROUGH
    
  case 3:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gex_Event_t
gasnete_coll_bcast_RVGet(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));

  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);


  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVGet, options,
                                           0, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
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
  
  
static int gasnete_coll_pf_bcast_TreeRVGet(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  
  int child;
  int result = 0;

  switch (data->state) {
  case 0:	/* thread barrier */
    if (!gasnete_coll_generic_all_threads(data)) {
      break;
    }
    data->state = 1; GASNETI_FALLTHROUGH
    
  case 1:       /* Optional IN barrier over the SAME tree */
    if ((op->flags & GASNET_COLL_IN_ALLSYNC) &&
        !gasnete_coll_generic_upsync(op, args->srcnode, 0, child_count)) {
      break;
    }
    data->state = 2; GASNETI_FALLTHROUGH
    
  case 2:	/* Data movement */
    if (op->team->myrank == args->srcnode) {
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->src, 0, 1);	/* broadcast src address to all the children*/
      }
      
      GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
    } else if (data->p2p->state[0]) {
      gasneti_sync_reads();
      data->handle = gasnete_get_nb(gasneti_THUNK_TM, args->dst, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(geom)),
                                         *(void **)data->p2p->data,
                                         args->nbytes, 0 GASNETI_THREAD_PASS);
      gasnete_coll_save_event(&data->handle);
    } else {
      break;
    }
    data->state = 3; GASNETI_FALLTHROUGH
    
  case 3:	/* Sync data movement */
    if (data->handle != GEX_EVENT_INVALID) {
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
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(geom)),1);
      }
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(op->team, children[child]), args->dst, 0, 1);	/* broadcast src address to all the children*/
      }
      
    }
    data->state = 4; GASNETI_FALLTHROUGH
    
  case 4:
    /*wait for all the children to respond*/
    if(op->flags & GASNET_COLL_OUT_MYSYNC) {
      if (gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) != child_count) {
        break;
      }
    }
    data->state = 5; GASNETI_FALLTHROUGH
    
  case 5:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gex_Event_t
gasnete_coll_bcast_TreeRVGet(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETI_THREAD_FARG)
{
  int options = 
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P;
  
  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);
  gasneti_assert(flags & GASNET_COLL_DST_IN_SEGMENT);

  
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreeRVGet, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type, 
                                                                              srcimage, team),
                                           sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/* bcast RVous: use AM Mediums to send to addrs provided by each node of a binimial tree */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_bcast_RVous(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gex_TM_t const tm = op->e_tm;
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  gex_Flags_t imm_flag = 0;

  // TODO-EX: pre-compute quantities such as these at injection,
  //          rather than repeatedly upon every poll.
  gex_Rank_t rel_rank = gasnete_tm_binom_rel_root(tm, args->srcnode);

  switch (data->state) {
  case 0:	/* Optional IN barrier */
    if (!gasnete_coll_generic_all_threads(data) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      break;
    }
    imm_flag = GEX_FLAG_IMMEDIATE; // execute next step w/ IMM flag only on first attempt
    data->state = 1; GASNETI_FALLTHROUGH
    
  case 1:	/* Rendevous w/ parent to pass addr */
    if (! rel_rank) {
      GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
    } else {
      /* Send our addr to parent */
      gex_Rank_t parent = gasnete_tm_binom_parent(tm, rel_rank);
      gex_Rank_t offset = gasnete_tm_binom_age(tm, rel_rank);
      if (gasnete_tm_p2p_send_rtr(op, data->p2p, parent, offset,
                                  args->dst, args->nbytes, imm_flag GASNETI_THREAD_PASS)) {
        break; // back pressure
      }
    }
    data->state = 2; GASNETI_FALLTHROUGH
    
  case 2:	/* Wait for data to arrive, unless root */
    if (! rel_rank) {
       // Nothing to do
    } else if (gasnete_tm_p2p_send_done(data->p2p)) {
      gasneti_sync_reads();
    } else {
      /* Not all data has arrived yet */
      break;
    }
    data->state = 3; GASNETI_FALLTHROUGH

  case 3: {	/* Send data */
    // We execute this step w/ IMM flag when the private_data field is NULL.
    // TODO: we are using an otherwise unused pointer field as a Boolean,
    // to avoid dynamic allocation, but could/should do something less obscure.
    imm_flag = data->private_data ? 0 : GEX_FLAG_IMMEDIATE;
    gex_Rank_t child_cnt = gasnete_tm_binom_children(tm, rel_rank);
    if (child_cnt) {
      /* Internal nodes send at most one AM per peer for each poll */
      const gex_Rank_t self = gex_TM_QueryRank(tm);
      const gex_Rank_t size = gex_TM_QuerySize(tm);
      int idx = child_cnt;
      int work_remains = 0;
      int sent = 0;
      while (idx--) { // Note reverse order for deepest subtree first
        gex_Rank_t distance = 1 << idx;
        gex_Rank_t peer = (distance >= size - self) ? self - (size - distance) : self + distance;
        int status = gasnete_tm_p2p_send_data(op, data->p2p, peer, idx, args->dst,
                                         args->nbytes, imm_flag GASNETI_THREAD_PASS);
        work_remains |= status;
        sent |= (status == 1);
      }
      if (work_remains) {
        // At least one send_data call indicated some data is still unsent.
        // If nothing was sent this poll, turn off IMM flag for next poll.
        // Otherwise, enable IMM flag (even if already enabled).
        data->private_data = (void*)(uintptr_t)(! sent);
        break;
      }
    }
    data->state = 4; GASNETI_FALLTHROUGH
  }
    
  case 4:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gex_Event_t
gasnete_coll_bcast_RVous(gasnet_team_handle_t team,
                         void * dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags,
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVous, options,
                                           NULL, sequence, coll_params->num_params, coll_params->param_list
 GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat RVGet: root node broadcasts address, others get from offsets from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_RVGet(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
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

    case 1:	/* Initiate data movement */
      if (op->team->myrank == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1, op->team);	/* broadcast src address */
	GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, 
				      gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
				      args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	data->handle = gasnete_get_nb(gasneti_THUNK_TM, args->dst, GASNETE_COLL_REL2ACT(op->team, args->srcnode),
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  op->team->myrank, args->nbytes),
					   args->nbytes, 0 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&data->handle);
      } else {
	break;
      }
      data->state = 2; GASNETI_FALLTHROUGH

    case 2:	/* Sync data movement */
      if (data->handle != GEX_EVENT_INVALID) {
	break;
      }
      data->state = 3; GASNETI_FALLTHROUGH

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
	break;
      }

      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gex_Event_t
gasnete_coll_scat_RVGet(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, size_t dist, int flags, 
                        gasnete_coll_implementation_t coll_params,
                        uint32_t sequence
                        GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(team, srcimage));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
					 &gasnete_coll_pf_scat_RVGet, options,
					 NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/* scat RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_scat_RVous(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;
  gex_Flags_t imm_flag = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      imm_flag = GEX_FLAG_IMMEDIATE; // execute next step w/ IMM flag only on first attempt
      data->state = 1; GASNETI_FALLTHROUGH

    case 1:	/* Rendevous w/ root to pass addr */
      if (op->team->myrank == args->srcnode) {
	GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, 
				      gasnete_coll_scale_ptr(args->src, op->team->myrank, args->nbytes),
				      args->nbytes);
      } else {
	/* Send our addr to root */
	if (gasnete_tm_p2p_send_rtr(op, data->p2p, args->srcnode, op->team->myrank,
                                    args->dst, args->nbytes, imm_flag GASNETI_THREAD_PASS)) {
          break; // back pressure
        }
      }
      imm_flag = GEX_FLAG_IMMEDIATE; // execute next step w/ IMM flag only on first attempt
      data->state = 2; GASNETI_FALLTHROUGH

    case 2:
      if (op->team->myrank == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	int work_remains = 0;
	for (gex_Rank_t i=0; i<op->team->total_ranks; ++i) {
	  if (i == op->team->myrank) continue;
	  int status = gasnete_tm_p2p_send_data(op, data->p2p, i, i,
					   gasnete_coll_scale_ptr(args->src, i, args->nbytes),
					   args->nbytes, imm_flag GASNETI_THREAD_PASS);
          work_remains |= status;
	}
	if (work_remains) {break;}
      } else if (gasnete_tm_p2p_send_done(data->p2p)) {
        gasneti_sync_reads();
      } else {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3; GASNETI_FALLTHROUGH

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
	break;
      }

      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gex_Event_t
gasnete_coll_scat_RVous(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, size_t dist, int flags, 
                        gasnete_coll_implementation_t coll_params,
                        uint32_t sequence
                        GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
					 &gasnete_coll_pf_scat_RVous, options,
					 NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath RVPut: root node broadcasts addresses, others put to that address (plus offset) */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_gath_RVPut(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      data->state = 1; GASNETI_FALLTHROUGH

    case 1:	/* Initiate data movement */
      if (op->team->myrank == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1, op->team);	/* broadcast dst address */
	GASNETI_MEMCPY_SAFE_IDENTICAL(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
				      args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	data->handle = gasnete_put_nb(gasneti_THUNK_TM, GASNETE_COLL_REL2ACT(op->team, args->dstnode),
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  op->team->myrank, args->nbytes),
					   args->src, args->nbytes, GEX_EVENT_DEFER, 0
                                           GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&data->handle);
      } else {
	  break;
      }
      data->state = 2; GASNETI_FALLTHROUGH

    case 2:	/* Sync data movement */
      if (data->handle != GEX_EVENT_INVALID) {
	break;
      }
      data->state = 3; GASNETI_FALLTHROUGH

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
	break;
      }

      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
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
					NULL, sequence,coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/* gath RVous: non-root nodes use AM Mediums to send to addrs provided by root */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_gath_RVous(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;
  gex_Flags_t imm_flag = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(op->team, data)) {
	break;
      }
      imm_flag = GEX_FLAG_IMMEDIATE; // execute next step w/ IMM flag only on first attempt
      data->state = 1; GASNETI_FALLTHROUGH

    case 1:	/* Root send addrs */
      if (op->team->myrank == args->dstnode) {
	int done = 1;
	for (gex_Rank_t i = 0; i < op->team->total_ranks; ++i) {
	  if (i == op->team->myrank) continue;
          if (data->p2p->state[i]) continue;
          if (gasnete_tm_p2p_send_rtr(op, data->p2p, i, 0,
				      gasnete_coll_scale_ptr(args->dst, i, args->nbytes),
				      args->nbytes, imm_flag GASNETI_THREAD_PASS)) {
            done = 0;
          } else {
            data->p2p->state[i] = 1;
          }
	}
        if (!done) break; // back pressure from at least one child
	GASNETI_MEMCPY_SAFE_IDENTICAL(gasnete_coll_scale_ptr(args->dst, op->team->myrank, args->nbytes),
				      args->src, args->nbytes);
      }
      imm_flag = GEX_FLAG_IMMEDIATE; // execute next step w/ IMM flag only on first attempt
      data->state = 2; GASNETI_FALLTHROUGH

    case 2:
      if (op->team->myrank != args->dstnode) {
	/* non-root nodes send at most one AM per poll */
	int work_remains = gasnete_tm_p2p_send_data(op, data->p2p, args->dstnode, 0,
                                            args->src, args->nbytes, imm_flag GASNETI_THREAD_PASS);
	if (work_remains) break;
      } else if (gasnete_tm_p2p_send_done(data->p2p)) {
        gasneti_sync_reads();
      } else {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3; GASNETI_FALLTHROUGH

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
	break;
      }

      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
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
					NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */
static int gasnete_coll_pf_exchg_RVPut(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
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
    
    data->state = 1; GASNETI_FALLTHROUGH
    
  case 1: /* send out all the destination addresses addresses*/
    gasnete_coll_p2p_eager_addr_all(op, args->dst, op->team->myrank, 1, op->team);	
    data->state = 2; GASNETI_FALLTHROUGH
    
  case 2: /*wait for all addresses to arrive*/
    for(i=0; i<op->team->total_ranks; i++) {
      if(i==op->team->myrank) continue;
      if(!data->p2p->state[i]) return 0;
    }
    data->state = 3; GASNETI_FALLTHROUGH
    /*if we go to here that means all addresses are ready*/
  case 3: /* fire off all the nonblocking puts*/
    gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
    /*put to the left of me*/
    for(i=op->team->myrank+1; i<op->team->total_ranks; i++) {
      gasnete_put_nbi(gasneti_THUNK_TM, GASNETE_COLL_REL2ACT(op->team,i), (int8_t*) ((void**)data->p2p->data)[i] + op->team->myrank*args->nbytes, (int8_t*) args->src+i*args->nbytes, args->nbytes, GEX_EVENT_DEFER, 0 GASNETI_THREAD_PASS);
    } 
    /*put to the right of me*/
    for(i=0; i<op->team->myrank; i++) {
      gasnete_put_nbi(gasneti_THUNK_TM, GASNETE_COLL_REL2ACT(op->team,i), (int8_t*) ((void**)data->p2p->data)[i] + op->team->myrank*args->nbytes, (int8_t*) args->src+i*args->nbytes, args->nbytes, GEX_EVENT_DEFER, 0 GASNETI_THREAD_PASS);
    }
    data->handle = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
    gasnete_coll_save_event(&data->handle);
    GASNETI_MEMCPY_SAFE_IDENTICAL((int8_t*) args->dst + op->team->myrank*args->nbytes, 
                                        (int8_t*) args->src+op->team->myrank*args->nbytes, args->nbytes);
    data->state = 4; GASNETI_FALLTHROUGH
  case 4: /* sync all the handles for the puts*/
    if (op->team->total_ranks > 1 && data->handle != GEX_EVENT_INVALID) {
      return 0;
    }
    data->state = 5; GASNETI_FALLTHROUGH
  case 5: /*final out barrier*/
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }
    data->state = 6; GASNETI_FALLTHROUGH
  case 6: /*done*/
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gex_Event_t
gasnete_coll_exchg_RVPut(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETI_THREAD_FARG)
{
  int options =  GASNETE_COLL_GENERIC_OPT_P2P | 
    GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
    
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_RVPut, options,
                                          NULL, NULL, sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
