/*
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _GNIX_VC_H_
#define _GNIX_VC_H_

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "gnix.h"
#include "gnix_bitmap.h"
#include "gnix_av.h"
#include "gnix_xpmem.h"
#include "gnix_cm_nic.h"

/*
 * mode bits
 */
#define GNIX_VC_MODE_IN_WQ		(1U)
#define GNIX_VC_MODE_IN_HT		(1U << 1)
#define GNIX_VC_MODE_DG_POSTED		(1U << 2)
#define GNIX_VC_MODE_PENDING_MSGS	(1U << 3)
#define GNIX_VC_MODE_PEER_CONNECTED	(1U << 4)
#define GNIX_VC_MODE_IN_TABLE		(1U << 5)
#define GNIX_VC_MODE_XPMEM		(1U << 6)

/* VC flags */
#define GNIX_VC_FLAG_RX_SCHEDULED	0
#define GNIX_VC_FLAG_WORK_SCHEDULED	1
#define GNIX_VC_FLAG_TX_SCHEDULED	2
#define GNIX_VC_FLAG_SCHEDULED		4

/*
 * defines for connection state for gnix VC
 */
enum gnix_vc_conn_state {
	GNIX_VC_CONN_NONE = 1,
	GNIX_VC_CONNECTING,
	GNIX_VC_CONNECTED,
	GNIX_VC_CONN_TERMINATING,
	GNIX_VC_CONN_TERMINATED,
	GNIX_VC_CONN_ERROR
};

enum gnix_vc_conn_req_type {
	GNIX_VC_CONN_REQ = 1,
	GNIX_VC_CONN_RESP
};

#define LOCAL_MBOX_SENT (1UL)
#define REMOTE_MBOX_RCVD (1UL << 1)

/**
 * Virtual Connection (VC) struct
 *
 * @var prog_list            NIC VC progress list
 * @var work_queue           Deferred work request queue
 * @var tx_queue             TX request queue
 * @var list                 used for unmapped vc list
 * @var fr_list              used for vc free list
 * @var entry                used internally for managing linked lists
 *                           of vc structs that require O(1) insertion/removal
 * @var peer_fi_addr         FI address of peer with which this VC is connected
 * @var peer_addr            address of peer with which this VC is connected
 * @var peer_cm_nic_addr     address of the cm_nic being used by peer, this
 *                           is the address to which GNI datagrams must be
 *                           posted
 * @var ep                   libfabric endpoint with which this VC is
 *                           associated
 * @var smsg_mbox            pointer to GNI SMSG mailbox used by this VC
 *                           to exchange SMSG messages with its peer
 * @var gnix_ep_name         cache for storing remote endpoint name
 * @var gni_ep               GNI endpoint for this VC
 * @var outstanding_fab_reqs Count of outstanding libfabric level requests
 *                           associated with this endpoint.
 * @var conn_state           Connection state of this VC
 * @var vc_id                ID of this vc. Allows for rapid O(1) lookup
 *                           of the VC when using GNI_CQ_GET_INST_ID to get
 *                           the inst_id of a GNI CQE.
 * @var peer_id              vc_id of peer.
 * @var modes                Used internally to track current state of
 *                           the VC not pertaining to the connection state.
 * @var flags                Bitmap used to hold vc schedule state
 * @var peer_irq_mem_hndl    peer GNI memhndl used for delivering
 *                           GNI_PostCqWrite requests to remote peer
 * @var peer_caps            peer capability flags
 */
struct gnix_vc {
	struct dlist_entry prog_list;	/* NIC VC progress list entry */
	struct dlist_entry work_queue;	/* Work reqs */
	struct dlist_entry tx_queue;	/* TX reqs */

	struct dlist_entry list;	/* General purpose list */
	struct dlist_entry fr_list;	/* fr list */
	fi_addr_t peer_fi_addr;
	struct gnix_address peer_addr;
	struct gnix_address peer_cm_nic_addr;
	struct gnix_fid_ep *ep;
	void *smsg_mbox;
	void *gnix_ep_name;
	gni_ep_handle_t gni_ep;
	ofi_atomic32_t outstanding_tx_reqs;
	enum gnix_vc_conn_state conn_state;
	uint32_t post_state;
	int vc_id;
	int peer_id;
	int modes;
	gnix_bitmap_t flags; /* We're missing regular bit ops */
	gni_mem_handle_t peer_irq_mem_hndl;
	xpmem_apid_t peer_apid;
	uint64_t peer_caps;
	uint32_t peer_key_offset;
};

/*
 * prototypes
 */

/**
 * @brief Allocates a virtual channel(vc) struct
 *
 * @param[in]  ep_priv    pointer to previously allocated gnix_fid_ep object
 * @param[in]  entry      av entry for remote peer for this VC.  Can be NULL
 *                        for accepting VCs.
 * @param[out] vc         location in which the address of the allocated vc
 *                        struct is to be returned.
 * @return FI_SUCCESS on success, -FI_ENOMEM if allocation of vc struct fails,
 */
int _gnix_vc_alloc(struct gnix_fid_ep *ep_priv,
		   struct gnix_av_addr_entry *entry, struct gnix_vc **vc);

/**
 * @brief Initiates non-blocking connect of a vc with its peer
 *
 * @param[in]  vc   pointer to previously allocated vc struct
 *
 * @return FI_SUCCESS on success, -FI_EINVAL if an invalid field in the vc
 *         struct is encountered, -ENOMEM if insufficient memory to initiate
 *         connection request.
 */
int _gnix_vc_connect(struct gnix_vc *vc);

/**
 * @brief Destroys a previously allocated vc and cleans up resources
 *        associated with the vc
 *
 * @param[in]  vc   pointer to previously allocated vc struct
 *
 * @return FI_SUCCESS on success, -FI_EINVAL if an invalid field in the vc
 *         struct is encountered.
 */
int _gnix_vc_destroy(struct gnix_vc *vc);

/**
 * @brief Add a vc to the work queue of its associated nic
 *
 * @param[in] vc  pointer to previously allocated vc struct
 *
 * @return FI_SUCCESS on success, -ENOMEM if insufficient memory
 * 	   allocate memory to enqueue work request
 */
int _gnix_vc_add_to_wq(struct gnix_vc *vc);

/**
 * @brief Progress a VC's SMSG mailbox.
 *
 * Messages are dequeued from the VCs SMSG mailbox until cleared or a failure
 * is encountered.
 *
 * @param[in] req The GNIX VC to progress.
 */
int _gnix_vc_dequeue_smsg(struct gnix_vc *vc);

/**
 * @brief Schedule a VC for RX progress.
 *
 * The VC will have it's SMSG mailbox progressed while the NIC is being
 * progressed in the near future.
 *
 * @param[in] vc The GNIX VC to schedule.
 */
int _gnix_vc_rx_schedule(struct gnix_vc *vc);

/**
 * @brief Queue a request with deferred work.
 *
 * @param[in] req The GNIX fabric request to queue.
 */
int _gnix_vc_queue_work_req(struct gnix_fab_req *req);

/**
 * @brief Requeue a request with deferred work.  Used only in TX completers
 * where the VC lock is not yet held.
 *
 * @param[in] req The GNIX fabric request to requeue.
 */
int _gnix_vc_requeue_work_req(struct gnix_fab_req *req);

/**
 * @brief Schedule a VC for TX progress.
 *
 * The VC will have it's tx_queue progressed while the NIC is being progressed
 * in the near future.
 *
 * @param[in] vc The GNIX VC to schedule.
 */
int _gnix_vc_tx_schedule(struct gnix_vc *vc);

/**
 * @brief Queue a new TX request.
 *
 * @param[in] req The GNIX fabric request to queue.
 */
int _gnix_vc_queue_tx_req(struct gnix_fab_req *req);

/**
 * @brief Progress NIC VCs.
 *
 * There are three facets of VC progress: RX, deferred work and TX.  The NIC
 * maintains one queue of VCs for each type of progress.  When a VC requires
 * progress, the associated _gnix_vc_<prog_type>_schedule() function is used to
 * schedule processing within _gnix_vc_nic_progress().  The queues are
 * independent to prevent a stall in TX processing from delaying RX processing,
 * and so forth.
 *
 * RX progress involves dequeueing SMSG messages and progressing the state of
 * associated requests.  If receipt of a message during RX progress will
 * trigger a new network operation (or similarly heavy or lock dependent
 * operation), that work should be queued in the deferred work queue, which
 * will be progressed once VC RX work is complete.  Examples of this deferred
 * work include the start of rendezvous data transfer or freeing an automatic
 * memory registration after an RX completion.
 *
 * The deferred work queue is processed after RX progress, where most deferred
 * work will be originated, and before TX processing, giving network resource
 * priority (specifically TXDs) to TX requests which have already been
 * initiated.
 *
 * New TX requests belong in a VCs TX queue.  Ordering of the VC TX queue is
 * enforced.  A request using the FI_FENCE flag will cause a VCs TX queue to be
 * stalled until that request is completed.
 *
 * @param[in] nic The GNIX NIC to progress.
 */
int _gnix_vc_nic_progress(struct gnix_nic *nic);

/**
 * @brief  return vc associated with a given ep/dest address, or the ep in the
 *         case of FI_EP_MSG endpoint type.  For FI_EP_RDM type, a vc may be
 *         allocated and a connection initiated if no vc is associated with
 *         ep/dest_addr.
 *
 * @param[in] ep        pointer to a previously allocated endpoint
 * @param[in] dest_addr for FI_EP_RDM endpoints, used to look up vc associated
 *                      with this target address
 * @param[out] vc_ptr   address in which to store pointer to returned vc
 * @return              FI_SUCCESS on success, -FI_ENOMEM insufficient
 *                      memory to allocate vc, -FI_EINVAL if an invalid
 *                      argument was supplied
 */
int _gnix_vc_ep_get_vc(struct gnix_fid_ep *ep, fi_addr_t dest_addr,
		    struct gnix_vc **vc_ptr);

/**
 * @brief       Return the FI address of a VC.
 *
 * @param vc    The VC for to use for lookup.
 * @return      The FI address of the input VC.  FI_ADDR_NOTAVAIL on error or
 *              if the VC is of incompatible type.
 */
fi_addr_t _gnix_vc_peer_fi_addr(struct gnix_vc *vc);

int _gnix_vc_cm_init(struct gnix_cm_nic *cm_nic);
int _gnix_vc_schedule(struct gnix_vc *vc);
int _gnix_vc_smsg_init(struct gnix_vc *vc,
		       int peer_id,
		       gni_smsg_attr_t *peer_smsg_attr,
		       gni_mem_handle_t *peer_irq_mem_hndl);

/*
 * inline functions
 */

/**
 * @brief Return connection state of a vc
 *
 * @param[in]  vc     pointer to previously allocated vc struct
 * @return connection state of vc
 */
static inline enum gnix_vc_conn_state _gnix_vc_state(struct gnix_vc *vc)
{
	assert(vc);
	return vc->conn_state;
}

#endif /* _GNIX_VC_H_ */
