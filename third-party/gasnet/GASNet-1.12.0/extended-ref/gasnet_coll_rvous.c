/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_rvous.c,v $
 *     $Date: 2007/10/15 20:00:18 $
 * $Revision: 1.65 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_coll_internal.h>
#include <gasnet_coll_trees.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_autotune.h>
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode,
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_RVGet(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVGet, options,
					   NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, gasneti_mynode, args->dst, args->srcnode, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, i, i, args->src, args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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
gasnete_coll_bcast_RVous(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVous, options,
					   NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* Get 1st image only */
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
					   args->srcnode, *(void **)data->p2p->data,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
        break;
      }
      data->state = 2;

    case 2:	/* Complete data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
      } else if (gasneti_mynode != args->srcnode) {
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
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
gasnete_coll_bcastM_RVGet(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)   |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC))|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_RVGet, options,
					    NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                     args->src, args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, gasneti_mynode,
				  GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				  args->srcnode, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, i, i, args->src, args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      } else {
	/* All data has arrived - perform local copies */
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
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
gasnete_coll_bcastM_RVous(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_RVous, options,
					    NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode,
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  gasneti_mynode, args->nbytes),
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_RVGet(gasnet_team_handle_t team,
			void *dst,
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scat_RVGet, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, gasneti_mynode, args->dst, args->srcnode, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, i, i,
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_RVous(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scat_RVous, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
				   args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->private_data = gasnete_coll_scale_ptr(*(void **)data->p2p->data,
					       gasnete_coll_my_offset,
					       args->nbytes);
	data->handle = gasnete_geti(gasnete_synctype_nb,
				    gasnete_coll_my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), args->nbytes,
				    args->srcnode, 1, &(data->private_data),
				    args->nbytes * gasnete_coll_my_images GASNETE_THREAD_PASS);
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_RVGet(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, nbytes, flags,
					  &gasnete_coll_pf_scatM_RVGet, options,
					  NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
        gasnete_coll_local_scatter(gasnete_coll_my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
                                   args->nbytes);
      } else {
	/* Send our addrs to root */
	gasneti_assert(gasnete_coll_p2p_eager_scale >= sizeof(gasnete_coll_p2p_send_struct_t));
	gasnete_coll_p2p_send_rtrM(op, data->p2p, gasnete_coll_my_offset,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				   args->srcnode, args->nbytes, gasnete_coll_my_images);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  gasnet_image_t j,k;
	  if (i == gasneti_mynode) continue;
	  k = gasnete_coll_all_offset[i] + gasnete_coll_all_images[i];
	  for (j = gasnete_coll_all_offset[i]; j < k; ++j) {
	    /* TODO: some pack/unpack of data (makes completion counter harder) */
	    done &= gasnete_coll_p2p_send_data(op, data->p2p, i, j,
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_RVous(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, nbytes, flags,
					  &gasnete_coll_pf_scatM_RVous, options,
					  NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1);	/* broadcast dst address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_put_nb_bulk(args->dstnode,
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  gasneti_mynode, args->nbytes),
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_RVPut(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_RVPut, options,
					NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Root send addrs */
      if (gasneti_mynode == args->dstnode) {
	gasnet_node_t i;
	for (i = 0; i < gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  gasnete_coll_p2p_send_rtr(op, data->p2p, 0,
				    gasnete_coll_scale_ptr(args->dst, i, args->nbytes),
				    i, args->nbytes);
	}
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode != args->dstnode) {
	/* non-root nodes send at most one AM per poll */
	int done = gasnete_coll_p2p_send_data(op, data->p2p, args->dstnode, 0, args->src, args->nbytes);
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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
gasnete_coll_gath_RVous(gasnet_team_handle_t team,
			gasnet_image_t dstimage, void *dst,
			void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_RVous, options,
					NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1);	/* broadcast dst address */
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->private_data = gasnete_coll_scale_ptr(*(void **)data->p2p->data, gasnete_coll_my_offset, args->nbytes);
	data->handle = gasnete_puti(gasnete_synctype_nb, args->dstnode,
				    1, &(data->private_data), args->nbytes * gasnete_coll_my_images,
				    gasnete_coll_my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags),
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
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_RVPut(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, nbytes, flags,
					 &gasnete_coll_pf_gathM_RVPut, options,
					 NULL, sequence GASNETE_THREAD_PASS);
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
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Root send addrs */
      if (gasneti_mynode == args->dstnode) {
	void **tmp = gasneti_malloc(sizeof(void *) * gasnete_coll_total_images);
	gasnet_image_t j;
	gasnet_node_t i;
	for (j = 0; j < gasnete_coll_total_images; ++j) {
	  tmp[j] = gasnete_coll_scale_ptr(args->dst, j, args->nbytes);
	}
	for (i = 0; i < gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  gasnete_coll_p2p_send_rtrM(op, data->p2p, 0, &GASNETE_COLL_1ST_IMAGE(tmp, i),
				     i, args->nbytes, gasnete_coll_all_images[i]);
	}
	gasneti_free(tmp);
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode != args->dstnode) {
	/* non-root nodes send at most one AM per image each poll */
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags);
	int done = 1;
	gasnet_image_t i;
	for (i = 0; i < gasnete_coll_my_images; ++i) {
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, args->dstnode, i, p[i], args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
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
gasnete_coll_gathM_RVous(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, nbytes, flags,
					 &gasnete_coll_pf_gathM_RVous, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_allM_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchangeM_nb() */

/*---------------------------------------------------------------------------------*/
