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

inline int
efa_rdm_ep_peer_map_insert(struct efa_rdm_ep_peer_map_entry **peer_map,
			   fi_addr_t addr,
			   struct efa_rdm_ep_peer_map_entry *map_entry)
{
	HASH_ADD(hndl, *peer_map, addr, sizeof(addr), map_entry);
	return FI_SUCCESS;
}

inline struct efa_rdm_peer *
efa_rdm_ep_peer_map_lookup(struct efa_rdm_ep_peer_map_entry **peer_map,
			   fi_addr_t addr)
{
	struct efa_rdm_ep_peer_map_entry *map_entry;

	HASH_FIND(hndl, *peer_map, &addr, sizeof(addr), map_entry);

	return map_entry ? &map_entry->peer : NULL;
}

void efa_rdm_ep_peer_map_remove(struct efa_rdm_ep_peer_map_entry **peer_map,
				fi_addr_t addr)
{
	struct efa_rdm_ep_peer_map_entry *map_entry = NULL;

	HASH_FIND(hndl, *peer_map, &addr, sizeof(addr), map_entry);
	assert(map_entry);
	HASH_DELETE(hndl, *peer_map, map_entry);
	ofi_buf_free(map_entry);
}

/**
 * @brief get pointer to efa_rdm_peer structure for a given libfabric address in
 * the explicit AV
 *
 * @param[in]		ep		endpoint
 * @param[in]		addr 		libfabric address
 * @returns pointer to #efa_rdm_peer
 */
struct efa_rdm_peer *efa_rdm_ep_get_peer(struct efa_rdm_ep *ep, fi_addr_t addr)
{
	struct efa_conn *conn;
	struct efa_rdm_ep_peer_map_entry *map_entry;
	struct efa_rdm_peer *peer;
	int err;

	conn = efa_av_addr_to_conn(ep->base_ep.av, addr);

	if (OFI_UNLIKELY(addr == FI_ADDR_NOTAVAIL))
		return NULL;

	peer = efa_rdm_ep_peer_map_lookup(&ep->fi_addr_to_peer_map, addr);
	if (peer)
		return peer;

	EFA_INFO(FI_LOG_EP_DATA, "Creating peer for addr %lu\n", addr);
	map_entry = ofi_buf_alloc(ep->peer_map_entry_pool);
	if (OFI_UNLIKELY(!map_entry)) {
		EFA_WARN(FI_LOG_EP_DATA,
			"Map entries for fi_addr to peer mapping exhausted.\n");
		return NULL;
	}

	memset(map_entry, 0, sizeof(*map_entry));

	map_entry->addr = addr;

	efa_rdm_peer_construct(&map_entry->peer, ep, conn);

	err = efa_rdm_ep_peer_map_insert(&ep->fi_addr_to_peer_map, addr, map_entry);
	if (err)
		return NULL;

	return &map_entry->peer;
}

/**
 * @brief get pointer to efa_rdm_peer structure for a given libfabric address in
 * the implicit AV
 *
 * @param[in]		ep		endpoint
 * @param[in]		addr 		libfabric address
 * @returns pointer to #efa_rdm_peer
 */
struct efa_rdm_peer *efa_rdm_ep_get_peer_implicit(struct efa_rdm_ep *ep, fi_addr_t addr)
{
	struct efa_conn *conn;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep_peer_map_entry *map_entry;
	int err;

	conn = efa_av_addr_to_conn_implicit(ep->base_ep.av, addr);

	if (OFI_UNLIKELY(addr == FI_ADDR_NOTAVAIL))
		return NULL;

	peer = efa_rdm_ep_peer_map_lookup(&ep->fi_addr_to_peer_map_implicit, addr);
	if (peer)
		goto out;

	EFA_INFO(FI_LOG_EP_DATA, "Creating peer for addr %lu\n", addr);
	map_entry = ofi_buf_alloc(ep->peer_map_entry_pool);
	if (OFI_UNLIKELY(!map_entry)) {
		EFA_WARN(FI_LOG_EP_DATA,
			"Map entries for fi_addr to peer mapping exhausted.\n");
		return NULL;
	}

	memset(map_entry, 0, sizeof(*map_entry));
	map_entry->addr = addr;

	efa_rdm_peer_construct(&map_entry->peer, ep, conn);
	peer = &map_entry->peer;

	err = efa_rdm_ep_peer_map_insert(&ep->fi_addr_to_peer_map_implicit, addr, map_entry);
	if (err)
		return NULL;

out:
	assert(peer);
	/* Move to the front of the LRU list */
	efa_av_implicit_av_lru_move(ep->base_ep.av, peer->conn);
	return peer;
}

void efa_rdm_ep_peer_map_implicit_to_explicit(struct efa_rdm_ep *ep,
					      struct efa_rdm_peer *peer,
					      fi_addr_t implicit_fi_addr,
					      fi_addr_t explicit_fi_addr)
{
	struct efa_rdm_ep_peer_map_entry *map_entry;

	HASH_FIND(hndl, ep->fi_addr_to_peer_map_implicit, &implicit_fi_addr, sizeof(implicit_fi_addr), map_entry);
	assert(map_entry);
	assert(peer == &map_entry->peer);
	assert(implicit_fi_addr == map_entry->addr);

	HASH_DELETE(hndl, ep->fi_addr_to_peer_map_implicit, map_entry);
	assert(map_entry);
	map_entry->addr = explicit_fi_addr;

	HASH_ADD(hndl, ep->fi_addr_to_peer_map, addr, sizeof(explicit_fi_addr), map_entry);
}

/**
 * @brief allocate an rxe for an operation
 *
 * @param ep[in]	end point
 * @param peer[in]	peer struct of the sender/requester.
 * @param op[in]	operation type (ofi_op_msg/ofi_op_tagged/ofi_op_read/ofi_op_write/ofi_op_atomic_xxx)
 * @return		if allocation succeeded, return pointer to rxe
 * 			if allocation failed, return NULL
 */
struct efa_rdm_ope *efa_rdm_ep_alloc_rxe(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer, uint32_t op)
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
	if (peer) {
		rxe->peer = peer;
		dlist_insert_tail(&rxe->peer_entry, &rxe->peer->rxe_list);
	} else {
		/*
		 * If peer is not provided, rxe->peer will be set
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
	rxe->rxe_map = NULL;
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
			"Unknown operation for RX entry allocation\n");
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
int efa_rdm_ep_post_user_recv_buf(struct efa_rdm_ep *ep, struct efa_rdm_ope *rxe, uint64_t flags)
{
	struct efa_rdm_pke *pkt_entry = NULL;
	size_t rx_iov_offset = 0;
	int err, rx_iov_index = 0;

	assert(rxe->iov_count > 0  && rxe->iov_count <= ep->base_ep.info->rx_attr->iov_limit);
	assert(rxe->iov[0].iov_len >= ep->msg_prefix_size);
	pkt_entry = efa_rdm_pke_alloc(ep, ep->user_rx_pkt_pool, EFA_RDM_PKE_FROM_USER_RX_POOL);
	if (OFI_UNLIKELY(!pkt_entry))
		return -FI_EAGAIN;

	pkt_entry->ope = rxe;
	pkt_entry->peer = rxe->peer;
	rxe->state = EFA_RDM_RXE_MATCHED;

	err = ofi_iov_locate(rxe->iov, rxe->iov_count, ep->msg_prefix_size, &rx_iov_index, &rx_iov_offset);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "ofi_iov_locate failure: %s (%d)\n", fi_strerror(-err), -err);
		goto err_free;
	}

	assert(rx_iov_index < rxe->iov_count);
	assert(rx_iov_offset < rxe->iov[rx_iov_index].iov_len);
	assert(ep->base_ep.domain->mr_local);

	if (!rxe->desc[rx_iov_index]) {
		err = -FI_EINVAL;
		EFA_WARN(FI_LOG_EP_CTRL,
			 "No valid desc is provided, not complied with FI_MR_LOCAL: %s (%d)\n",
			 fi_strerror(-err), -err);
		goto err_free;
	}

	pkt_entry->payload = (char *) rxe->iov[rx_iov_index].iov_base + rx_iov_offset;
	pkt_entry->payload_mr = rxe->desc[rx_iov_index];
	pkt_entry->payload_size = ofi_total_iov_len(&rxe->iov[rx_iov_index], rxe->iov_count - rx_iov_index) - rx_iov_offset;

	err = efa_rdm_pke_user_recvv(&pkt_entry, 1, flags);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"failed to post user supplied buffer %d (%s)\n", -err,
			fi_strerror(-err));
		goto err_free;
	}

#if ENABLE_DEBUG
	dlist_insert_tail(&pkt_entry->dbg_entry, &ep->rx_posted_buf_list);
#endif
	ep->user_rx_pkts_posted++;
	return 0;

err_free:
	if (pkt_entry)
		efa_rdm_pke_release_rx(pkt_entry);
	return err;
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
	if (peer) {
		dlist_insert_tail(&pkt_entry->entry,
				  &peer->outstanding_tx_pkts);
		pkt_entry->flags |= EFA_RDM_PKE_IN_PEER_OUTSTANDING_TX_PKTS;
	}

	assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_TX_POOL);
	ep->efa_outstanding_tx_ops++;
	if (peer)
		peer->efa_outstanding_tx_ops++;

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
	if (pkt_entry->peer) {
		dlist_remove(&pkt_entry->entry);
		pkt_entry->flags &= ~EFA_RDM_PKE_IN_PEER_OUTSTANDING_TX_PKTS;
	}

	assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_TX_POOL);
	ep->efa_outstanding_tx_ops--;
	if (pkt_entry->peer)
		pkt_entry->peer->efa_outstanding_tx_ops--;

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
 * @param[in]	peer	efa_rdm_peer struct of the receiver
 */
void efa_rdm_ep_queue_rnr_pkt(struct efa_rdm_ep *ep, struct efa_rdm_pke *pkt_entry)
{
	static const int random_min_timeout = 40;
	static const int random_max_timeout = 120;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ope *ope = pkt_entry->ope;

#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
	peer = pkt_entry->peer;

	assert(ope);
	dlist_insert_tail(&pkt_entry->entry, &ope->queued_pkts);
	pkt_entry->flags |= EFA_RDM_PKE_IN_OPE_QUEUED_PKTS;
	ep->efa_rnr_queued_pkt_cnt += 1;
	assert(peer);
	if (!(ope->internal_flags & EFA_RDM_OPE_QUEUED_RNR)) {
		ope->internal_flags |= EFA_RDM_OPE_QUEUED_RNR;
		dlist_insert_tail(&ope->queued_entry, &efa_rdm_ep_domain(ep)->ope_queued_list);
	}
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
			  &efa_rdm_ep_domain(ep)->peer_backoff_list);

	peer->rnr_backoff_begin_ts = ofi_gettime_us();
	if (peer->rnr_backoff_wait_time == 0) {
		if (efa_env.rnr_backoff_initial_wait_time > 0)
			peer->rnr_backoff_wait_time = efa_env.rnr_backoff_initial_wait_time;
		else
			peer->rnr_backoff_wait_time = MAX(random_min_timeout,
							  rand() %
							  random_max_timeout);

		EFA_DBG(FI_LOG_EP_DATA,
			"initializing backoff timeout for peer fi_addr: "
			"%" PRIu64 " implicit fi_addr: %" PRIu64
			" timeout: %ld rnr_queued_pkts: %d\n",
			peer->conn->fi_addr, peer->conn->implicit_fi_addr,
			peer->rnr_backoff_wait_time, peer->rnr_queued_pkt_cnt);
	} else {
		peer->rnr_backoff_wait_time = MIN(peer->rnr_backoff_wait_time * 2,
						  efa_env.rnr_backoff_wait_time_cap);
		EFA_DBG(FI_LOG_EP_DATA,
			"increasing backoff timeout for peer fi_addr: %" PRIu64
			" implicit fi_addr %" PRIu64
			" to %ld rnr_queued_pkts: %d\n",
			peer->conn->fi_addr, peer->conn->implicit_fi_addr,
			peer->rnr_backoff_wait_time, peer->rnr_queued_pkt_cnt);
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

	msg.addr = peer->conn->fi_addr;

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
	txe->internal_flags |= EFA_RDM_OPE_INTERNAL;

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

	addr = peer->conn->fi_addr;
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
	txe->internal_flags |= EFA_RDM_OPE_INTERNAL;

	pkt_entry = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);
	if (OFI_UNLIKELY(!pkt_entry)) {
		EFA_DBG(FI_LOG_EP_CTRL, "PKE entries exhausted.\n");
		efa_rdm_txe_release(txe);
		return -FI_EAGAIN;
	}

	pkt_entry->ope = txe;
	pkt_entry->peer = peer;

	efa_rdm_pke_init_handshake(pkt_entry, peer);

	ret = efa_rdm_pke_sendv(&pkt_entry, 1, 0);
	if (OFI_UNLIKELY(ret)) {
		efa_rdm_pke_release_tx(pkt_entry);
		efa_rdm_txe_release(txe);
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
				  &efa_rdm_ep_domain(ep)->handshake_queued_peer_list);
		return;
	}

	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Failed to post HANDSHAKE to peer fi_addr: %ld implicit fi_addr %ld. %s\n",
			peer->conn->fi_addr, peer->conn->implicit_fi_addr, fi_strerror(-err));
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_PEER_HANDSHAKE);
		return;
	}

	peer->flags |= EFA_RDM_PEER_HANDSHAKE_SENT;
}

/**
 * @brief post a linked list of packets
 *
 * @param[in]	ep	RDM endpoint
 * @param[in]	pkts	Linked list of packets to post
 * @return		0 on success, negative error code on failure
 */
ssize_t efa_rdm_ep_post_queued_pkts(struct efa_rdm_ep *ep,
				    struct dlist_entry *pkts)
{
	struct dlist_entry *tmp;
	struct efa_rdm_pke *pkt_entry;
	ssize_t ret;

	dlist_foreach_container_safe(pkts, struct efa_rdm_pke,
				     pkt_entry, entry, tmp) {

		/* If send succeeded, pkt_entry->entry will be added
		 * to peer->outstanding_tx_pkts. Therefore, it must
		 * be removed from the list before send.
		 */
		dlist_remove(&pkt_entry->entry);
		pkt_entry->flags &= ~EFA_RDM_PKE_IN_OPE_QUEUED_PKTS;

		switch (efa_rdm_pkt_type_of(pkt_entry)) {
		case EFA_RDM_RMA_CONTEXT_PKT:
			assert(((struct efa_rdm_rma_context_pkt *)pkt_entry->wiredata)->context_type == EFA_RDM_RDMA_WRITE_CONTEXT);
			ret = efa_rdm_pke_write(pkt_entry);
			break;
		default:
			ret = efa_rdm_pke_sendv(&pkt_entry, 1, 0);
			break;
		}

		if (ret) {
			if (ret == -FI_EAGAIN) {
				/* add the pkt back to pkts, so it can be resent again */
				dlist_insert_tail(&pkt_entry->entry, pkts);
				pkt_entry->flags |= EFA_RDM_PKE_IN_OPE_QUEUED_PKTS;
			}

			return ret;
		}

		pkt_entry->flags &= ~EFA_RDM_PKE_RNR_RETRANSMIT;
		ep->efa_rnr_queued_pkt_cnt--;
		assert(pkt_entry->peer);
		pkt_entry->peer->rnr_queued_pkt_cnt--;
	}

	return 0;
}

/**
 * @brief bulk post internal receive buffers to EFA device
 *
 * Received packets were not reposted to device immediately
 * after they are processed. Instead, endpoint keep a counter
 * of number packets to be posted, and post them in bulk
 *
 * @param[in]	ep		endpoint
 * @return	On success, return 0
 * 		On failure, return a negative error code.
 */
int efa_rdm_ep_bulk_post_internal_rx_pkts(struct efa_rdm_ep *ep)
{
	int i, err;

	/**
	 * When efa_env.internal_rx_refill_threshold > efa_rdm_ep_get_rx_pool_size(ep),
	 * we should always refill when the pool is empty.
	 */
	if (ep->efa_rx_pkts_to_post < MIN(efa_env.internal_rx_refill_threshold, efa_rdm_ep_get_rx_pool_size(ep)))
		return 0;

	assert(ep->efa_rx_pkts_to_post + ep->efa_rx_pkts_posted <= ep->efa_max_outstanding_rx_ops);
	for (i = 0; i < ep->efa_rx_pkts_to_post; ++i) {
		ep->pke_vec[i] = efa_rdm_pke_alloc(ep, ep->efa_rx_pkt_pool,
					       EFA_RDM_PKE_FROM_EFA_RX_POOL);
		assert(ep->pke_vec[i]);
	}

	err = efa_rdm_pke_recvv(ep->pke_vec, ep->efa_rx_pkts_to_post);
	if (OFI_UNLIKELY(err)) {
		for (i = 0; i < ep->efa_rx_pkts_to_post; ++i)
			efa_rdm_pke_release_rx(ep->pke_vec[i]);

		EFA_WARN(FI_LOG_EP_CTRL,
			"failed to post buf %d (%s)\n", -err,
			fi_strerror(-err));
		return err;
	}

#if ENABLE_DEBUG
	for (i = 0; i < ep->efa_rx_pkts_to_post; ++i) {
		dlist_insert_tail(&ep->pke_vec[i]->dbg_entry,
				  &ep->rx_posted_buf_list);
	}
#endif

	ep->efa_rx_pkts_posted += ep->efa_rx_pkts_to_post;
	ep->efa_rx_pkts_to_post = 0;
	return 0;
}

/*
 * @brief explicitly allocate a chunk of memory for 6 pools on RX side:
 *     efa's receive packet pool (efa_rx_pkt_pool)
 *     unexpected packet pool (rx_unexp_pkt_pool),
 *     out-of-order packet pool (rx_ooo_pkt_pool), and
 *     local read-copy packet pool (rx_readcopy_pkt_pool).
 *
 * This function is called when the progress engine is called for
 * the 1st time on this endpoint.
 *
 * @param ep[in,out]	endpoint
 * @return		On success, return 0
 * 			On failure, return a negative error code.
 */
int efa_rdm_ep_grow_rx_pools(struct efa_rdm_ep *ep)
{
	int err;

	assert(ep->efa_rx_pkt_pool);
	err = ofi_bufpool_grow(ep->efa_rx_pkt_pool);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ,
			"cannot allocate memory for EFA's RX packet pool. error: %s\n",
			strerror(-err));
		return err;
	}

	if (ep->rx_unexp_pkt_pool) {
		err = ofi_bufpool_grow(ep->rx_unexp_pkt_pool);
		if (OFI_UNLIKELY(err)) {
			EFA_WARN(FI_LOG_CQ,
				"cannot allocate memory for unexpected packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	if (ep->rx_ooo_pkt_pool) {
		err = ofi_bufpool_grow(ep->rx_ooo_pkt_pool);
		if (OFI_UNLIKELY(err)) {
			EFA_WARN(FI_LOG_CQ,
				"cannot allocate memory for out-of-order packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	if (ep->rx_readcopy_pkt_pool) {
		err = ofi_bufpool_grow(ep->rx_readcopy_pkt_pool);
		if (OFI_UNLIKELY(err)) {
			EFA_WARN(FI_LOG_CQ,
				"cannot allocate and register memory for readcopy packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	if (ep->map_entry_pool) {
		err = ofi_bufpool_grow(ep->map_entry_pool);
		if (OFI_UNLIKELY(err)) {
			EFA_WARN(FI_LOG_CQ,
				 "cannot allocate memory for map entry pool. error: %s\n",
				 strerror(-err));
			return err;
		}
	}

	return 0;
}

/**
 * @brief post internal receive buffers for progress engine.
 *
 * It is more efficient to post multiple receive buffers
 * to the device at once than to post each receive buffer
 * individually.
 *
 * Therefore, after an internal receive buffer (a packet
 * entry) was processed, it is not posted to the device
 * right away.
 *
 * Instead, we increase counter
 *      ep->efa_rx_pkts_to_post
 * by one.
 *
 * Later, progress engine calls this function to
 * bulk post internal receive buffers (according to
 * the counter).
 *
 * This function also control number of internal
 * buffers posted to the device in zero copy receive
 * mode.
 *
 * param[in]	ep	endpoint
 */
void efa_rdm_ep_post_internal_rx_pkts(struct efa_rdm_ep *ep)
{
	int err;

	if (ep->efa_rx_pkts_posted == 0 && ep->efa_rx_pkts_to_post == 0 && ep->efa_rx_pkts_held == 0) {
		/* All of efa_rx_pkts_posted, efa_rx_pkts_to_post and
		 * efa_rx_pkts_held equal to 0 means
		 * this is the first call of the progress engine on this endpoint.
		 *
		 * In this case, we explictly allocate the 1st chunk of memory
		 * for unexp/ooo/readcopy RX packet pool.
		 *
		 * The reason to explicitly allocate the memory for RX packet
		 * pool is to improve efficiency.
		 *
		 * Without explicit memory allocation, a pkt pools's memory
		 * is allocated when 1st packet is allocated from it.
		 * During the computation, different processes got their 1st
		 * unexp/ooo/read-copy packet at different time. Therefore,
		 * if we do not explicitly allocate memory at the beginning,
		 * memory will be allocated at different time.
		 *
		 * When one process is allocating memory, other processes
		 * have to wait. When each process allocate memory at different
		 * time, the accumulated waiting time became significant.
		 *
		 * By explicitly allocating memory at ep initialization
		 * engine, the memory allocation is parallelized.
		 * (This assumes the ep initialization on
		 * all processes happen at roughly the same time, which
		 * is a valid assumption according to our knowledge of
		 * the workflow of most application)
		 */
		err = efa_rdm_ep_grow_rx_pools(ep);
		if (err)
			goto err_exit;

		ep->efa_rx_pkts_to_post = efa_rdm_ep_get_rx_pool_size(ep);
	}

	assert(ep->efa_rx_pkts_to_post + ep->efa_rx_pkts_posted + ep->efa_rx_pkts_held == efa_rdm_ep_get_rx_pool_size(ep));

	err = efa_rdm_ep_bulk_post_internal_rx_pkts(ep);
	if (err)
		goto err_exit;

	return;

err_exit:

	efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_INTERNAL_RX_BUF_POST);
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

/**
 * @brief Enforce a handshake to made for given txe.
 * It will trigger a handshake with peer and choose to
 * return EAGAIN or queue the txe.
 * @param ep efa_rdm_ep
 * @param txe tx entry
 * @return int 0 on success, negative integer on failure.
 */
int efa_rdm_ep_enforce_handshake_for_txe(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe)
{
	int ret;

	assert(txe->type == EFA_RDM_TXE);
	assert(!(txe->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED));

	ret = efa_rdm_ep_trigger_handshake(ep, txe->peer);
	if (ret)
		return ret;
	/**
	 * We cannot queue requests (and return 0) for inject,
	 * which expects the buffer can be reused when the call
	 * returns success. We also have a limit for the number
	 * of opes queued due to handshake not made
	 */
	if ((txe->fi_flags & FI_INJECT) ||
	    (ep->ope_queued_before_handshake_cnt >= EFA_RDM_MAX_QUEUED_OPE_BEFORE_HANDSHAKE))
		return -FI_EAGAIN;

	if (!(txe->internal_flags & EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE)) {
		txe->internal_flags |= EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE;
		dlist_insert_tail(&txe->queued_entry, &efa_rdm_ep_domain(ep)->ope_queued_list);
		ep->ope_queued_before_handshake_cnt++;
	}
	return FI_SUCCESS;
}
