/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "ofi.h"
#include <ofi_util.h>
#include <ofi_iov.h>
#include "efa.h"
#include "efa_av.h"
#include "efa_rdm_msg.h"
#include "efa_rdm_rma.h"
#include "efa_rdm_atomic.h"
#include "efa_rdm_pke_cmd.h"
#include <infiniband/verbs.h>
#include "efa_rdm_tracepoint.h"
#include "efa_cntr.h"
#include "efa_rdm_srx.h"
#include "efa_rdm_cq.h"
#include "efa_rdm_pke_nonreq.h"

struct efa_ep_addr *efa_rdm_ep_raw_addr(struct efa_rdm_ep *ep)
{
	return &ep->base_ep.src_addr;
}

const char *efa_rdm_ep_raw_addr_str(struct efa_rdm_ep *ep, char *buf, size_t *buflen)
{
	return ofi_straddr(buf, buflen, FI_ADDR_EFA, efa_rdm_ep_raw_addr(ep));
}

/**
 * @brief return peer's raw address in #efa_ep_addr
 *
 * @param[in] ep		end point
 * @param[in] addr 		libfabric address
 * @returns
 * If peer exists, return peer's raw addrress as pointer to #efa_ep_addr;
 * Otherwise, return NULL
 * @relates efa_rdm_peer
 */
struct efa_ep_addr *efa_rdm_ep_get_peer_raw_addr(struct efa_rdm_ep *ep, fi_addr_t addr)
{
	struct efa_av *efa_av;
	struct efa_conn *efa_conn;

	efa_av = ep->base_ep.av;
	efa_conn = efa_av_addr_to_conn(efa_av, addr);
	return efa_conn ? efa_conn->ep_addr : NULL;
}

/**
 * @brief return peer's ahn
 *
 * @param[in] ep		end point
 * @param[in] addr 		libfabric address
 * @returns
 * If peer exists, return peer's ahn
 * Otherwise, return -1
 */
int32_t efa_rdm_ep_get_peer_ahn(struct efa_rdm_ep *ep, fi_addr_t addr)
{
	struct efa_av *efa_av;
	struct efa_conn *efa_conn;

	efa_av = ep->base_ep.av;
	efa_conn = efa_av_addr_to_conn(efa_av, addr);
	return efa_conn ? efa_conn->ah->ahn : -1;
}

/**
 * @brief return peer's raw address in a reable string
 *
 * @param[in] ep		end point
 * @param[in] addr 		libfabric address
 * @param[out] buf		a buffer tat to be used to store string
 * @param[in,out] buflen	length of `buf` as input. length of the string as output.
 * @relates efa_rdm_peer
 * @return a string with peer's raw address
 */
const char *efa_rdm_ep_get_peer_raw_addr_str(struct efa_rdm_ep *ep, fi_addr_t addr, char *buf, size_t *buflen)
{
	return ofi_straddr(buf, buflen, FI_ADDR_EFA, efa_rdm_ep_get_peer_raw_addr(ep, addr));
}

/**
 * @brief get pointer to efa_rdm_peer structure for a given libfabric address
 *
 * @param[in]		ep		endpoint
 * @param[in]		addr 		libfabric address
 * @returns if peer exists, return pointer to #efa_rdm_peer;
 *          otherwise, return NULL.
 */
struct efa_rdm_peer *efa_rdm_ep_get_peer(struct efa_rdm_ep *ep, fi_addr_t addr)
{
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *av_entry;

	if (OFI_UNLIKELY(addr == FI_ADDR_NOTAVAIL))
		return NULL;

	util_av_entry = ofi_bufpool_get_ibuf(ep->base_ep.util_ep.av->av_entry_pool,
	                                     addr);
	av_entry = (struct efa_av_entry *)util_av_entry->data;
	return av_entry->conn.ep_addr ? &av_entry->conn.rdm_peer : NULL;
}

/**
 * @brief allocate an rxe for an operation
 *
 * @param ep[in]	end point
 * @param addr[in]	fi address of the sender/requester.
 * @param op[in]	operation type (ofi_op_msg/ofi_op_tagged/ofi_op_read/ofi_op_write/ofi_op_atomic_xxx)
 * @return		if allocation succeeded, return pointer to rxe
 * 			if allocation failed, return NULL
 */
struct efa_rdm_ope *efa_rdm_ep_alloc_rxe(struct efa_rdm_ep *ep, fi_addr_t addr, uint32_t op)
{
	struct efa_rdm_ope *rxe;

	rxe = ofi_buf_alloc(ep->ope_pool);
	if (OFI_UNLIKELY(!rxe)) {
		EFA_WARN(FI_LOG_EP_CTRL, "RX entries exhausted\n");
		return NULL;
	}

	rxe->ep = ep;
	dlist_insert_tail(&rxe->ep_entry, &ep->rxe_list);
	rxe->type = EFA_RDM_RXE;
	rxe->internal_flags = 0;
	rxe->fi_flags = 0;
	rxe->rx_id = ofi_buf_index(rxe);
	rxe->iov_count = 0;
	memset(rxe->mr, 0, sizeof(*rxe->mr) * EFA_RDM_IOV_LIMIT);

	dlist_init(&rxe->queued_pkts);

	rxe->state = EFA_RDM_RXE_INIT;
	rxe->addr = addr;
	if (addr != FI_ADDR_UNSPEC) {
		rxe->peer = efa_rdm_ep_get_peer(ep, addr);
		assert(rxe->peer);
		dlist_insert_tail(&rxe->peer_entry, &rxe->peer->rxe_list);
	} else {
		/*
		 * If msg->addr is not provided, rxe->peer will be set
		 * after it is matched with a message.
		 */
		assert(op == ofi_op_msg || op == ofi_op_tagged);
		rxe->peer = NULL;
	}

	rxe->bytes_received = 0;
	rxe->bytes_received_via_mulreq = 0;
	rxe->bytes_copied = 0;
	rxe->bytes_queued_blocking_copy = 0;
	rxe->bytes_acked = 0;
	rxe->bytes_sent = 0;
	rxe->bytes_runt = 0;
	rxe->cuda_copy_method = EFA_RDM_CUDA_COPY_UNSPEC;
	rxe->efa_outstanding_tx_ops = 0;
	rxe->window = 0;
	rxe->op = op;
	rxe->peer_rxe = NULL;
	rxe->unexp_pkt = NULL;
	rxe->atomrsp_data = NULL;
	rxe->bytes_read_total_len = 0;

	switch (op) {
	case ofi_op_tagged:
		rxe->cq_entry.flags = (FI_RECV | FI_MSG | FI_TAGGED);
		break;
	case ofi_op_msg:
		rxe->cq_entry.flags = (FI_RECV | FI_MSG);
		break;
	case ofi_op_read_rsp:
		rxe->cq_entry.flags = (FI_REMOTE_READ | FI_RMA);
		break;
	case ofi_op_write:
		rxe->cq_entry.flags = (FI_REMOTE_WRITE | FI_RMA);
		break;
	case ofi_op_atomic:
		rxe->cq_entry.flags = (FI_REMOTE_WRITE | FI_ATOMIC);
		break;
	case ofi_op_atomic_fetch:
	case ofi_op_atomic_compare:
		rxe->cq_entry.flags = (FI_REMOTE_READ | FI_ATOMIC);
		break;
	default:
		EFA_WARN(FI_LOG_EP_CTRL,
			"Unknown operation while %s\n", __func__);
		assert(0 && "Unknown operation");
	}

	return rxe;
}

/**
 * @brief post user provided receiving buffer to the device.
 *
 * The user receive buffer was converted to an RX packet, then posted to the device.
 *
 * @param[in]	ep		endpint
 * @param[in]	rxe	rxe that contain user buffer information
 * @param[in]	flags		user supplied flags passed to fi_recv
 */
int efa_rdm_ep_post_user_recv_buf(struct efa_rdm_ep *ep, struct efa_rdm_ope *rxe, size_t flags)
{
	struct efa_rdm_pke *pkt_entry;
	struct efa_mr *mr;
	int err;

	assert(rxe->iov_count == 1);
	assert(rxe->iov[0].iov_len >= ep->msg_prefix_size);
	pkt_entry = (struct efa_rdm_pke *)rxe->iov[0].iov_base;
	assert(pkt_entry);

	/*
	 * The ownership of the prefix buffer lies with the application, do not
	 * put it on the dbg list for cleanup during shutdown or poison it. The
	 * provider loses jurisdiction over it soon after writing the rx
	 * completion.
	 */
	dlist_init(&pkt_entry->entry);
	mr = (struct efa_mr *)rxe->desc[0];
	pkt_entry->mr = &mr->mr_fid;
	pkt_entry->alloc_type = EFA_RDM_PKE_FROM_USER_BUFFER;
	pkt_entry->flags = EFA_RDM_PKE_IN_USE;
	pkt_entry->next = NULL;
	/*
	 * The actual receiving buffer size (pkt_size) is
	 *    rxe->total_len - sizeof(struct efa_rdm_pke)
	 * because the first part of user buffer was used to
	 * construct pkt_entry. The actual receiving buffer
	 * posted to device starts from pkt_entry->wiredata.
	 */
	pkt_entry->pkt_size = rxe->iov[0].iov_len - sizeof(struct efa_rdm_pke);

	pkt_entry->ope = rxe;
	rxe->state = EFA_RDM_RXE_MATCHED;

	err = efa_rdm_pke_recvv(&pkt_entry, 1);
	if (OFI_UNLIKELY(err)) {
		efa_rdm_pke_release_rx(pkt_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"failed to post user supplied buffer %d (%s)\n", -err,
			fi_strerror(-err));
		return err;
	}

	ep->efa_rx_pkts_posted++;
	return 0;
}



/* create a new txe */
struct efa_rdm_ope *efa_rdm_ep_alloc_txe(struct efa_rdm_ep *efa_rdm_ep,
					 struct efa_rdm_peer *peer,
					 const struct fi_msg *msg,
					 uint32_t op,
					 uint64_t tag,
					 uint64_t flags)
{
	struct efa_rdm_ope *txe;

	txe = ofi_buf_alloc(efa_rdm_ep->ope_pool);
	if (OFI_UNLIKELY(!txe)) {
		EFA_DBG(FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return NULL;
	}

	efa_rdm_txe_construct(txe, efa_rdm_ep, peer, msg, op, flags);
	if (op == ofi_op_tagged) {
		txe->cq_entry.tag = tag;
		txe->tag = tag;
	}

	dlist_insert_tail(&txe->ep_entry, &efa_rdm_ep->txe_list);
	return txe;
}

/**
 * @brief record the event that a TX op has been submitted
 *
 * This function is called after a TX operation has been posted
 * successfully. It will:
 *
 *  1. increase the outstanding tx_op counter in endpoint and
 *     in the peer structure.
 *
 *  2. add the TX packet to peer's outstanding TX packet list.
 *
 * Both send and read are considered TX operation.
 *
 * The tx_op counters used to prevent over posting the device
 * and used in flow control. They are also usefull for debugging.
 *
 * Peer's outstanding TX packet list is used when removing a peer
 * to invalidate address of these packets, so that the completion
 * of these packet is ignored.
 *
 * @param[in,out]	ep		endpoint
 * @param[in]		pkt_entry	TX pkt_entry, which contains
 * 					the info of the TX op.
 */
void efa_rdm_ep_record_tx_op_submitted(struct efa_rdm_ep *ep, struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_peer *peer;
	struct efa_rdm_ope *ope;

	ope = pkt_entry->ope;
	assert(ope);

	/*
	 * peer can be NULL when the pkt_entry is a RMA_CONTEXT_PKT,
	 * and the RMA is a local read toward the endpoint itself
	 */
	peer = ope->peer;
	if (peer)
		dlist_insert_tail(&pkt_entry->entry,
				  &peer->outstanding_tx_pkts);

	assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_TX_POOL);
	ep->efa_outstanding_tx_ops++;
	if (peer)
		peer->efa_outstanding_tx_ops++;

	if (ope)
		ope->efa_outstanding_tx_ops++;
#if ENABLE_DEBUG
	ep->efa_total_posted_tx_ops++;
#endif
}

/**
 * @brief record the event that an TX op is completed
 *
 * This function is called when the completion of
 * a TX operation is received. It will
 *
 * 1. decrease the outstanding tx_op counter in the endpoint
 *    and in the peer.
 *
 * 2. remove the TX packet from peer's outstanding
 *    TX packet list.
 *
 * Both send and read are considered TX operation.
 *
 * One may ask why this function is not integrated
 * into efa_rdm_pke_relase_tx()?
 *
 * The reason is the action of decrease tx_op counter
 * is not tied to releasing a TX pkt_entry.
 *
 * Sometimes we need to decreate the tx_op counter
 * without releasing a TX pkt_entry. For example,
 * we handle a TX pkt_entry encountered RNR. We need
 * to decrease the tx_op counter and queue the packet.
 *
 * Sometimes we need release TX pkt_entry without
 * decreasing the tx_op counter. For example, when
 * efa_rdm_ope_post_send() failed to post a pkt entry.
 *
 * @param[in,out]	ep		endpoint
 * @param[in]		pkt_entry	TX pkt_entry, which contains
 * 					the info of the TX op
 */
void efa_rdm_ep_record_tx_op_completed(struct efa_rdm_ep *ep, struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *ope = NULL;
	struct efa_rdm_peer *peer;

	ope = pkt_entry->ope;
	/*
	 * peer can be NULL when:
	 *
	 * 1. the pkt_entry is a RMA_CONTEXT_PKT, and the RMA op is a local read
	 *    toward the endpoint itself.
	 * 2. peer's address has been removed from address vector. Either because
	 *    a new peer has the same GID+QPN was inserted to address, or because
	 *    application removed the peer from address vector.
	 */
	peer = efa_rdm_ep_get_peer(ep, pkt_entry->addr);
	if (peer)
		dlist_remove(&pkt_entry->entry);

	assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_TX_POOL);
	ep->efa_outstanding_tx_ops--;
	if (peer)
		peer->efa_outstanding_tx_ops--;

	if (ope)
		ope->efa_outstanding_tx_ops--;
}

/* @brief Queue a packet that encountered RNR error and setup RNR backoff
 *
 * We uses an exponential backoff strategy to handle RNR errors.
 *
 * `Backoff` means if a peer encountered RNR, an endpoint will
 * wait a period of time before sending packets to the peer again
 *
 * `Exponential` means the more RNR encountered, the longer the
 * backoff wait time will be.
 *
 * To quantify how long a peer stay in backoff mode, two parameters
 * are defined:
 *
 *    rnr_backoff_begin_ts (ts is timestamp) and rnr_backoff_wait_time.
 *
 * A peer stays in backoff mode until:
 *
 * current_timestamp >= (rnr_backoff_begin_ts + rnr_backoff_wait_time),
 *
 * with one exception: a peer can got out of backoff mode early if a
 * packet's send completion to this peer was reported by the device.
 *
 * Specifically, the implementation of RNR backoff is:
 *
 * For a peer, the first time RNR is encountered, the packet will
 * be resent immediately.
 *
 * The second time RNR is encountered, the endpoint will put the
 * peer in backoff mode, and initialize rnr_backoff_begin_timestamp
 * and rnr_backoff_wait_time.
 *
 * The 3rd and following time RNR is encounter, the RNR will be handled
 * like this:
 *
 *     If peer is already in backoff mode, rnr_backoff_begin_ts
 *     will be updated
 *
 *     Otherwise, peer will be put in backoff mode again,
 *     rnr_backoff_begin_ts will be updated and rnr_backoff_wait_time
 *     will be doubled until it reached maximum wait time.
 *
 * @param[in]	ep		endpoint
 * @param[in]	list		queued RNR packet list
 * @param[in]	pkt_entry	packet entry that encounter RNR
 */
void efa_rdm_ep_queue_rnr_pkt(struct efa_rdm_ep *ep,
			  struct dlist_entry *list,
			  struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_peer *peer;
	static const int random_min_timeout = 40;
	static const int random_max_timeout = 120;

#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
	dlist_insert_tail(&pkt_entry->entry, list);
	ep->efa_rnr_queued_pkt_cnt += 1;
	peer = efa_rdm_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	if (!(pkt_entry->flags & EFA_RDM_PKE_RNR_RETRANSMIT)) {
		/* This is the first time this packet encountered RNR,
		 * we are NOT going to put the peer in backoff mode just yet.
		 */
		pkt_entry->flags |= EFA_RDM_PKE_RNR_RETRANSMIT;
		peer->rnr_queued_pkt_cnt++;
		return;
	}

	/* This packet has encountered RNR multiple times, therefore the peer
	 * need to be in backoff mode.
	 *
	 * If the peer is already in backoff mode, we just need to update the
	 * RNR backoff begin time.
	 *
	 * Otherwise, we need to put the peer in backoff mode and set up backoff
	 * begin time and wait time.
	 */
	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF) {
		peer->rnr_backoff_begin_ts = ofi_gettime_us();
		return;
	}

	peer->flags |= EFA_RDM_PEER_IN_BACKOFF;
	dlist_insert_tail(&peer->rnr_backoff_entry,
			  &ep->peer_backoff_list);

	peer->rnr_backoff_begin_ts = ofi_gettime_us();
	if (peer->rnr_backoff_wait_time == 0) {
		if (efa_env.rnr_backoff_initial_wait_time > 0)
			peer->rnr_backoff_wait_time = efa_env.rnr_backoff_initial_wait_time;
		else
			peer->rnr_backoff_wait_time = MAX(random_min_timeout,
							  rand() %
							  random_max_timeout);

		EFA_DBG(FI_LOG_EP_DATA,
		       "initializing backoff timeout for peer: %" PRIu64
		       " timeout: %ld rnr_queued_pkts: %d\n",
		       pkt_entry->addr, peer->rnr_backoff_wait_time,
		       peer->rnr_queued_pkt_cnt);
	} else {
		peer->rnr_backoff_wait_time = MIN(peer->rnr_backoff_wait_time * 2,
						  efa_env.rnr_backoff_wait_time_cap);
		EFA_DBG(FI_LOG_EP_DATA,
		       "increasing backoff timeout for peer: %" PRIu64
		       " to %ld rnr_queued_pkts: %d\n",
		       pkt_entry->addr, peer->rnr_backoff_wait_time,
		       peer->rnr_queued_pkt_cnt);
	}
}

/**
 * @brief trigger a peer to send a handshake packet
 *
 * This patch send a EAGER_RTW packet of 0 byte to a peer, which would
 * cause the peer to send a handshake packet back to the endpoint.
 *
 * This function is used for any extra feature that does not have an
 * alternative.
 *
 * We do not send eager rtm packets here because the receiver might require
 * ordering and an extra eager rtm will interrupt the reorder
 * process.
 *
 * @param[in]	ep	The endpoint on which the packet for triggering handshake will be sent.
 * @param[in]	addr	The address of the peer.
 *
 * @returns
 * return 0 for success.
 * return negative libfabric error code for error. Possible errors include:
 * -FI_EAGAIN	temporarily out of resource to send packet
 */
ssize_t efa_rdm_ep_trigger_handshake(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer)
{
	struct efa_rdm_ope *txe;
	struct fi_msg msg = {0};
	ssize_t err;

	assert(peer);
	if ((peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED) ||
	    (peer->flags & EFA_RDM_PEER_REQ_SENT))
		return 0;

	msg.addr = peer->efa_fiaddr;

	txe = efa_rdm_ep_alloc_txe(ep, peer, &msg, ofi_op_write, 0, 0);

	if (OFI_UNLIKELY(!txe)) {
		EFA_WARN(FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return -FI_EAGAIN;
	}

	/* efa_rdm_ep_alloc_txe() joins ep->base_ep.util_ep.tx_op_flags and passed in flags,
	 * reset to desired flags (remove things like FI_DELIVERY_COMPLETE, and FI_COMPLETION)
	 */
	txe->fi_flags = EFA_RDM_TXE_NO_COMPLETION | EFA_RDM_TXE_NO_COUNTER;
	txe->msg_id = -1;

	err = efa_rdm_ope_post_send(txe, EFA_RDM_EAGER_RTW_PKT);

	if (OFI_UNLIKELY(err))
		return err;

	return 0;
}

/** @brief Post a handshake packet to a peer.
 *
 * @param ep The endpoint on which the handshake packet is sent out.
 * @param peer The peer to which the handshake packet is posted.
 * @return 0 on success, fi_errno on error.
 */
ssize_t efa_rdm_ep_post_handshake(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer)
{
	struct efa_rdm_ope *txe;
	struct fi_msg msg = {0};
	struct efa_rdm_pke *pkt_entry;
	fi_addr_t addr;
	ssize_t ret;

	addr = peer->efa_fiaddr;
	msg.addr = addr;

	/* ofi_op_write is ignored in handshake path */
	txe = efa_rdm_ep_alloc_txe(ep, peer, &msg, ofi_op_write, 0, 0);

	if (OFI_UNLIKELY(!txe)) {
		EFA_WARN(FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return -FI_EAGAIN;
	}

	/* efa_rdm_ep_alloc_txe() joins ep->base_ep.util_ep.tx_op_flags and passed in flags,
	 * reset to desired flags (remove things like FI_DELIVERY_COMPLETE, and FI_COMPLETION)
	 */
	txe->fi_flags = EFA_RDM_TXE_NO_COMPLETION | EFA_RDM_TXE_NO_COUNTER;

	pkt_entry = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);
	if (OFI_UNLIKELY(!pkt_entry)) {
		EFA_WARN(FI_LOG_EP_CTRL, "PKE entries exhausted.\n");
		return -FI_EAGAIN;
	}

	pkt_entry->ope = txe;

	efa_rdm_pke_init_handshake(pkt_entry, addr);

	ret = efa_rdm_pke_sendv(&pkt_entry, 1);
	if (OFI_UNLIKELY(ret)) {
		efa_rdm_pke_release_tx(pkt_entry);
	}
	return ret;
}

/** @brief Post a handshake packet to a peer.
 *
 * This function ensures an endpoint post one and only one handshake
 * to a peer.
 *
 * For a peer that the endpoint has not attempted to send handshake,
 * it will send a handshake packet.
 *
 * If the send succeeded, EFA_RDM_PEER_HANDSHAKE_SENT flag will be set to peer->flags.
 *
 * If the send encountered FI_EAGAIN failure, the peer will be added to
 * efa_rdm_ep->handshake_queued_peer_list. The handshake will be resend later
 * by the progress engine.
 *
 * If the send encountered other failure, an EQ entry will be written.
 *
 * To ensure only one handshake is send to a peer, the function will not send
 * packet to a peer whose peer->flags has either EFA_RDM_PEER_HANDSHAKE_SENT or
 * EFA_RDM_PEER_HANDSHAKE_QUEUED.
 *
 * @param[in]	ep	The endpoint on which the handshake packet is sent out.
 * @param[in]	peer	The peer to which the handshake packet is posted.
 * @return 	void.
 */
void efa_rdm_ep_post_handshake_or_queue(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer)
{
	ssize_t err;

	if (peer->flags & (EFA_RDM_PEER_HANDSHAKE_SENT | EFA_RDM_PEER_HANDSHAKE_QUEUED))
		return;

	err = efa_rdm_ep_post_handshake(ep, peer);
	if (OFI_UNLIKELY(err == -FI_EAGAIN)) {
		/* add peer to handshake_queued_peer_list for retry later */
		peer->flags |= EFA_RDM_PEER_HANDSHAKE_QUEUED;
		dlist_insert_tail(&peer->handshake_queued_entry,
				  &ep->handshake_queued_peer_list);
		return;
	}

	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Failed to post HANDSHAKE to peer %ld: %s\n",
			peer->efa_fiaddr, fi_strerror(-err));
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_PEER_HANDSHAKE);
		return;
	}

	peer->flags |= EFA_RDM_PEER_HANDSHAKE_SENT;
}

/**
 * @brief Get memory alignment for given ep and hmem iface
 *
 * @param ep efa rdm ep
 * @param iface hmem iface
 * @return size_t the memory alignment
 */
size_t efa_rdm_ep_get_memory_alignment(struct efa_rdm_ep *ep, enum fi_hmem_iface iface)
{
	size_t memory_alignment = EFA_RDM_DEFAULT_MEMORY_ALIGNMENT;

	if (ep->sendrecv_in_order_aligned_128_bytes) {
		memory_alignment = EFA_RDM_IN_ORDER_ALIGNMENT;
	} else if (iface == FI_HMEM_CUDA) {
		memory_alignment = EFA_RDM_CUDA_MEMORY_ALIGNMENT;
	}

	return memory_alignment;
}

