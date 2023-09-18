/*
 * Copyright (c) 2019-2022 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ofi_iov.h>
#include <ofi_recvwin.h>
#include "efa.h"
#include "rxr_rma.h"
#include "rxr_msg.h"
#include "rxr_cntr.h"
#include "rxr_read.h"
#include "rxr_atomic.h"
#include "rxr_pkt_cmd.h"

#include "rxr_tp.h"

static const char *rxr_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
				   const void *err_data, char *buf, size_t len)
{
	return efa_strerror(prov_errno);
}

/**
 * @brief handle error happened to an RX (receive) operation
 *
 * This function will write an error cq entry to notify application the rx
 * operation failed. If write failed, it will write an eq entry.
 *
 * It will also release resources owned by the RX entry, such as unexpected
 * packet entry, because the RX operation is aborted.
 *
 * It will remove the rx_entry from queued rx_entry list for the same reason.
 *
 * It will NOT release the rx_entry because it is still possible to receive
 * packet for this rx_entry.
 *
 * TODO: add a NACK message to tear down state on sender side
 *
 * @param[in]	ep		endpoint
 * @param[in]	rx_entry	rx_entry that contains information of the tx operation
 * @param[in]	err		positive libfabric error code
 * @param[in]	prov_errno	positive provider specific error code
 */
void rxr_cq_write_rx_error(struct rxr_ep *ep, struct rxr_op_entry *rx_entry,
			   int err, int prov_errno)
{
	struct fi_cq_err_entry err_entry;
	struct util_cq *util_cq;
	struct dlist_entry *tmp;
	struct rxr_pkt_entry *pkt_entry;
	int write_cq_err;
	char ep_addr_str[OFI_ADDRSTRLEN], peer_addr_str[OFI_ADDRSTRLEN];
	size_t buflen = 0;

	memset(&err_entry, 0, sizeof(err_entry));
	memset(&ep_addr_str, 0, sizeof(ep_addr_str));
	memset(&peer_addr_str, 0, sizeof(peer_addr_str));

	util_cq = ep->util_ep.rx_cq;

	err_entry.err = err;
	err_entry.prov_errno = prov_errno;

	switch (rx_entry->state) {
	case RXR_RX_INIT:
	case RXR_RX_UNEXP:
		dlist_remove(&rx_entry->entry);
		break;
	case RXR_RX_MATCHED:
		break;
	case RXR_RX_RECV:
#if ENABLE_DEBUG
		dlist_remove(&rx_entry->pending_recv_entry);
#endif
		break;
	case RXR_RX_QUEUED_CTRL:
		dlist_remove(&rx_entry->queued_ctrl_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "rx_entry unknown state %d\n",
			rx_entry->state);
		assert(0 && "rx_entry unknown state");
	}

	if (rx_entry->rxr_flags & RXR_RX_ENTRY_QUEUED_RNR) {
		dlist_foreach_container_safe(&rx_entry->queued_pkts,
					     struct rxr_pkt_entry,
					     pkt_entry, entry, tmp)
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		dlist_remove(&rx_entry->queued_rnr_entry);
	}

	if (rx_entry->unexp_pkt) {
		rxr_pkt_entry_release_rx(ep, rx_entry->unexp_pkt);
		rx_entry->unexp_pkt = NULL;
	}

	if (rx_entry->fi_flags & FI_MULTI_RECV)
		rxr_msg_multi_recv_handle_completion(ep, rx_entry);

	err_entry.flags = rx_entry->cq_entry.flags;
	if (rx_entry->state != RXR_RX_UNEXP)
		err_entry.op_context = rx_entry->cq_entry.op_context;
	err_entry.buf = rx_entry->cq_entry.buf;
	err_entry.data = rx_entry->cq_entry.data;
	err_entry.tag = rx_entry->cq_entry.tag;

	rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);

	buflen = sizeof(ep_addr_str);
	rxr_ep_raw_addr_str(ep, ep_addr_str, &buflen);
	buflen = sizeof(peer_addr_str);
	rxr_peer_raw_addr_str(ep, rx_entry->addr, peer_addr_str, &buflen);

	FI_WARN(&rxr_prov, FI_LOG_CQ,
		"rxr_cq_write_rx_error: err: %d, prov_err: %s (%d) our address: %s, peer address %s\n",
		err_entry.err, efa_strerror(err_entry.prov_errno),
		err_entry.prov_errno, ep_addr_str, peer_addr_str);
	/*
	 * TODO: We can't free the rx_entry as we may receive additional
	 * packets for this entry. Add ref counting so the rx_entry can safely
	 * be freed once all packets are accounted for.
	 */
	//rxr_release_rx_entry(ep, rx_entry);

	efa_cntr_report_error(&ep->util_ep, err_entry.flags);
	write_cq_err = ofi_cq_write_error(util_cq, &err_entry);
	if (write_cq_err) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Error writing error cq entry when handling RX error");
		efa_eq_write_error(&ep->util_ep, err, prov_errno);
	}
}

/**
 * @brief write error CQ entry for a TX operation.
 *
 * This function write an error cq entry for a TX operation, if writing
 * CQ error entry failed, it will write eq entry.
 *
 * If also remote the TX entry from ep->tx_queued_list and ep->tx_pending_list
 * if the tx_entry is on it.
 *
 * It does NOT release tx entry because it is still possible to receive
 * send completion for this TX entry
 *
 * TODO: add NACK message to tear down receive side state
 *
 * @param[in]	ep		endpoint
 * @param[in]	tx_entry	tx_entry that contains information of the tx operation
 * @param[in]	err		positive libfabric error code
 * @param[in]	prov_errno	positive EFA provider specific error code
 */
void rxr_cq_write_tx_error(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
			   int err, int prov_errno)
{
	struct fi_cq_err_entry err_entry;
	struct util_cq *util_cq;
	uint32_t api_version;
	struct dlist_entry *tmp;
	struct rxr_pkt_entry *pkt_entry;
	int write_cq_err;
	char ep_addr_str[OFI_ADDRSTRLEN], peer_addr_str[OFI_ADDRSTRLEN];
	size_t buflen = 0;

	memset(&err_entry, 0, sizeof(err_entry));
	memset(&ep_addr_str, 0, sizeof(ep_addr_str));
	memset(&peer_addr_str, 0, sizeof(peer_addr_str));

	util_cq = ep->util_ep.tx_cq;
	api_version = util_cq->domain->fabric->fabric_fid.api_version;

	err_entry.err = err;
	err_entry.prov_errno = prov_errno;

	switch (tx_entry->state) {
	case RXR_TX_REQ:
		break;
	case RXR_TX_SEND:
		dlist_remove(&tx_entry->entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "tx_entry unknown state %d\n",
			tx_entry->state);
		assert(0 && "tx_entry unknown state");
	}

	if (tx_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR)
		dlist_remove(&tx_entry->queued_rnr_entry);

	if (tx_entry->rxr_flags & RXR_OP_ENTRY_QUEUED_CTRL)
		dlist_remove(&tx_entry->queued_ctrl_entry);

	dlist_foreach_container_safe(&tx_entry->queued_pkts,
				     struct rxr_pkt_entry,
				     pkt_entry, entry, tmp)
		rxr_pkt_entry_release_tx(ep, pkt_entry);

	err_entry.flags = tx_entry->cq_entry.flags;
	err_entry.op_context = tx_entry->cq_entry.op_context;
	err_entry.buf = tx_entry->cq_entry.buf;
	err_entry.data = tx_entry->cq_entry.data;
	err_entry.tag = tx_entry->cq_entry.tag;
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
		err_entry.err_data_size = 0;

	buflen = sizeof(ep_addr_str);
	rxr_ep_raw_addr_str(ep, ep_addr_str, &buflen);
	buflen = sizeof(peer_addr_str);
	rxr_peer_raw_addr_str(ep, tx_entry->addr, peer_addr_str, &buflen);

	FI_WARN(&rxr_prov, FI_LOG_CQ,
		"rxr_cq_write_tx_error: err: %d, prov_err: %s (%d) our address: %s, peer address %s\n",
		err_entry.err, efa_strerror(err_entry.prov_errno),
		err_entry.prov_errno, ep_addr_str, peer_addr_str);

	/*
	 * TODO: We can't free the tx_entry as we may receive a control packet
	 * for this entry. Add ref counting so the tx_entry can safely
	 * be freed once all packets are accounted for.
	 */
	//rxr_release_tx_entry(ep, tx_entry);

	efa_cntr_report_error(&ep->util_ep, tx_entry->cq_entry.flags);
	write_cq_err = ofi_cq_write_error(util_cq, &err_entry);
	if (write_cq_err) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Error writing error cq entry when handling TX error");
		efa_eq_write_error(&ep->util_ep, err, prov_errno);
	}
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
void rxr_cq_queue_rnr_pkt(struct rxr_ep *ep,
			  struct dlist_entry *list,
			  struct rxr_pkt_entry *pkt_entry)
{
	struct rdm_peer *peer;

#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
	dlist_insert_tail(&pkt_entry->entry, list);

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	if (!(pkt_entry->flags & RXR_PKT_ENTRY_RNR_RETRANSMIT)) {
		/* This is the first time this packet encountered RNR,
		 * we are NOT going to put the peer in backoff mode just yet.
		 */
		pkt_entry->flags |= RXR_PKT_ENTRY_RNR_RETRANSMIT;
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
	if (peer->flags & RXR_PEER_IN_BACKOFF) {
		peer->rnr_backoff_begin_ts = ofi_gettime_us();
		return;
	}

	peer->flags |= RXR_PEER_IN_BACKOFF;
	dlist_insert_tail(&peer->rnr_backoff_entry,
			  &ep->peer_backoff_list);

	peer->rnr_backoff_begin_ts = ofi_gettime_us();
	if (peer->rnr_backoff_wait_time == 0) {
		if (rxr_env.rnr_backoff_initial_wait_time > 0)
			peer->rnr_backoff_wait_time = rxr_env.rnr_backoff_initial_wait_time;
		else
			peer->rnr_backoff_wait_time = MAX(RXR_RAND_MIN_TIMEOUT,
							  rand() %
							  RXR_RAND_MAX_TIMEOUT);

		FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
		       "initializing backoff timeout for peer: %" PRIu64
		       " timeout: %ld rnr_queued_pkts: %d\n",
		       pkt_entry->addr, peer->rnr_backoff_wait_time,
		       peer->rnr_queued_pkt_cnt);
	} else {
		peer->rnr_backoff_wait_time = MIN(peer->rnr_backoff_wait_time * 2,
						  rxr_env.rnr_backoff_wait_time_cap);
		FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
		       "increasing backoff timeout for peer: %" PRIu64
		       "to %ld rnr_queued_pkts: %d\n",
		       pkt_entry->addr, peer->rnr_backoff_wait_time,
		       peer->rnr_queued_pkt_cnt);
	}
}

void rxr_cq_write_rx_completion(struct rxr_ep *ep,
				struct rxr_op_entry *rx_entry)
{
	struct util_cq *rx_cq = ep->util_ep.rx_cq;
	int ret = 0;
	if (OFI_UNLIKELY(rx_entry->cq_entry.len < rx_entry->total_len)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Message truncated: tag: %"PRIu64" len: %"PRIu64" total_len: %zu\n",
			rx_entry->cq_entry.tag,	rx_entry->total_len,
			rx_entry->cq_entry.len);

		ret = ofi_cq_write_error_trunc(ep->util_ep.rx_cq,
					       rx_entry->cq_entry.op_context,
					       rx_entry->cq_entry.flags,
					       rx_entry->total_len,
					       rx_entry->cq_entry.buf,
					       rx_entry->cq_entry.data,
					       rx_entry->cq_entry.tag,
					       rx_entry->total_len -
					       rx_entry->cq_entry.len);

		rxr_rm_rx_cq_check(ep, rx_cq);

		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Unable to write recv error cq: %s\n",
				fi_strerror(-ret));
			return;
		}

		rx_entry->fi_flags |= RXR_NO_COMPLETION;
		efa_cntr_report_error(&ep->util_ep, rx_entry->cq_entry.flags);
		return;
	}

	if (!(rx_entry->rxr_flags & RXR_RECV_CANCEL) &&
	    (ofi_need_completion(rxr_rx_flags(ep), rx_entry->fi_flags) ||
	     (rx_entry->cq_entry.flags & FI_MULTI_RECV))) {
		FI_DBG(&rxr_prov, FI_LOG_CQ,
		       "Writing recv completion for rx_entry from peer: %"
		       PRIu64 " rx_id: %" PRIu32 " msg_id: %" PRIu32
		       " tag: %lx total_len: %" PRIu64 "\n",
		       rx_entry->addr, rx_entry->rx_id, rx_entry->msg_id,
		       rx_entry->cq_entry.tag, rx_entry->total_len);

		rxr_tracing(recv_end, 
			    rx_entry->msg_id, (size_t) rx_entry->cq_entry.op_context, 
			    rx_entry->total_len, rx_entry->cq_entry.tag, rx_entry->addr);


		if (ep->util_ep.caps & FI_SOURCE)
			ret = ofi_cq_write_src(rx_cq,
					       rx_entry->cq_entry.op_context,
					       rx_entry->cq_entry.flags,
					       rx_entry->cq_entry.len,
					       rx_entry->cq_entry.buf,
					       rx_entry->cq_entry.data,
					       rx_entry->cq_entry.tag,
					       rx_entry->addr);
		else
			ret = ofi_cq_write(rx_cq,
					   rx_entry->cq_entry.op_context,
					   rx_entry->cq_entry.flags,
					   rx_entry->cq_entry.len,
					   rx_entry->cq_entry.buf,
					   rx_entry->cq_entry.data,
					   rx_entry->cq_entry.tag);

		rxr_rm_rx_cq_check(ep, rx_cq);

		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Unable to write recv completion: %s\n",
				fi_strerror(-ret));
			rxr_cq_write_rx_error(ep, rx_entry, -ret, FI_EFA_ERR_WRITE_RECV_COMP);
			return;
		}

		rx_entry->fi_flags |= RXR_NO_COMPLETION;
	}

	efa_cntr_report_rx_completion(&ep->util_ep, rx_entry->cq_entry.flags);
}

/**
 * @brief complete an operation of receiving data
 *
 * This function completes an RX operation. RX operation can be
 * a receive, a write response, or a read response. This function
 * is called when all data has been transmitted.
 *
 * To complete a RX operation, this function does 3 things:
 *
 * 1. If necessary, write completion to application. (Not all
 *    completed RX action will cause a completion to be written).
 *
 * 2. When asked, send a ctrl packet back to the peer.
 *    Caller of this function needs to decide whether such a ctrl packet
 *    is needed. Usually it is because of the two scenarios:
 *
 *    a. longread message/write protocol was used, which requires the receiver/
 *       responder send an EOR back to the sender/requester.
 *
 *    b. dc eager/medium/long message/write protocol was used, which requires
 *       the receiver/responder to send an RECEIPT back to sender/requester.
 *
 * 3. Ensure release of rx_entry.
 *
 * @param[in,out]	ep		endpoint
 * @param[in,out]	op_entry	op_entry that contains information of a data receive operation
 * @param[in]		post_ctrl	whether to post a ctrl packet back to sender/requester
 * @param[in]		ctrl_type	ctrl packet type.
 */
void rxr_cq_complete_recv(struct rxr_ep *ep,
		          struct rxr_op_entry *op_entry,
	 		  bool post_ctrl, int ctrl_type)
{
	struct rxr_op_entry *tx_entry = NULL;
	struct rxr_op_entry *rx_entry = NULL;
	struct rdm_peer *peer;
	bool inject;
	int err;

	/* It is important to write completion before sending ctrl packet, because the
	 * action of sending ctrl packet may cause the release of RX entry (when inject
	 * was used on lower device).
	 */
	if (op_entry->cq_entry.flags & FI_WRITE) {
		/*
		 * For write, only write RX completion when REMOTE_CQ_DATA is on
		 */
		if (op_entry->cq_entry.flags & FI_REMOTE_CQ_DATA)
			rxr_cq_write_rx_completion(ep, op_entry);
	} else if (op_entry->cq_entry.flags & FI_READ) {
		/* This op_entry is part of the for emulated read protocol,
		 * created on the read requester side.
		 * The following shows the sequence of events in an emulated
		 * read protocol.
		 *
		 * Requester                      Responder
		 * create tx_entry
		 * send rtr
		 *                                receive rtr
		 *                                create rx_entry
		 *                                rx_entry sending data
		 * tx_entry receiving data
		 * receive completed              send completed
		 * call rxr_cq_complete_recv()    call rxr_cq_handle_send_completion()
		 *
		 * As can be seen, in the emulated read protocol, this function is called only
		 * on the requester side, so we need to find the corresponding tx_entry and
		 * complete it.
		 */
		assert(!post_ctrl); /* in emulated read, no ctrl should be posted */
		assert(op_entry->type == RXR_TX_ENTRY);
		tx_entry = op_entry; /* Intentionally assigned for easier understanding */

		assert(tx_entry->state == RXR_TX_REQ);
		if (tx_entry->fi_flags & FI_COMPLETION) {
			rxr_cq_write_tx_completion(ep, tx_entry);
		} else {
			efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
		}
	} else {
		assert(op_entry->type == RXR_RX_ENTRY);
		rx_entry = op_entry; /* Intentionally assigned for easier understanding */

		assert(rx_entry->op == ofi_op_msg || rx_entry->op == ofi_op_tagged);
		if (rx_entry->fi_flags & FI_MULTI_RECV)
			rxr_msg_multi_recv_handle_completion(ep, rx_entry);

		rxr_cq_write_rx_completion(ep, rx_entry);
		rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);
	}

	/* As can be seen, this function does not release rx_entry when
	 * rxr_pkt_post_or_queue() was successful.
	 *
	 * This is because that rxr_pkt_post_or_queue() might have
	 * queued the ctrl packet (due to out of resource), and progress
	 * engine will resend the packet. In that case, progress engine
	 * needs the rx_entry to construct the ctrl packet.
	 * 
	 * Hence, the rx_entry can be safely released only when we got
	 * the send completion of the ctrl packet.
	 *
	 * Another interesting point is that when inject was used, the
	 * rx_entry was released by rxr_pkt_post_or_queue(), because
	 * when inject was used, lower device will not provider send
	 * completion for the ctrl packet.
	 */
	if (post_ctrl) {
		assert(op_entry->type == RXR_RX_ENTRY);
		rx_entry = op_entry; /* Intentionally assigned for easier understanding */
		assert(ctrl_type == RXR_RECEIPT_PKT || ctrl_type == RXR_EOR_PKT);
		peer = rxr_ep_get_peer(ep, rx_entry->addr);
		assert(peer);
		inject = peer->is_local && ep->use_shm_for_tx;
		err = rxr_pkt_post_or_queue(ep, rx_entry, ctrl_type, inject);
		if (OFI_UNLIKELY(err)) {
			FI_WARN(&rxr_prov,
				FI_LOG_CQ,
				"Posting of ctrl packet failed when complete rx! err=%s(%d)\n",
				fi_strerror(-err), -err);
			rxr_cq_write_rx_error(ep, rx_entry, -err, FI_EFA_ERR_PKT_POST);
			rxr_release_rx_entry(ep, rx_entry);
		}

		return;
	}

	if (op_entry->rxr_flags & RXR_EOR_IN_FLIGHT)
		return;
	
	if (op_entry->type == RXR_TX_ENTRY) {
		rxr_release_tx_entry(ep, op_entry);
	} else {
		assert(op_entry->type == RXR_RX_ENTRY);
		rxr_release_rx_entry(ep, op_entry);
	}
}

int rxr_cq_reorder_msg(struct rxr_ep *ep,
		       struct rdm_peer *peer,
		       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_pkt_entry *ooo_entry;
	struct rxr_pkt_entry *cur_ooo_entry;
	struct rxr_robuf *robuf;
	uint32_t msg_id;

	assert(rxr_get_base_hdr(pkt_entry->pkt)->type >= RXR_REQ_PKT_BEGIN);

	msg_id = rxr_pkt_msg_id(pkt_entry);

	robuf = &peer->robuf;
#if ENABLE_DEBUG
	if (msg_id != ofi_recvwin_next_exp_id(robuf))
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "msg OOO msg_id: %" PRIu32 " expected: %"
		       PRIu32 "\n", msg_id,
		       ofi_recvwin_next_exp_id(robuf));
#endif
	if (ofi_recvwin_is_exp(robuf, msg_id))
		return 0;
	else if (!ofi_recvwin_id_valid(robuf, msg_id)) {
		if (ofi_recvwin_id_processed(robuf, msg_id)) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			       "Error: message id has already been processed. received: %" PRIu32 " expected: %"
			       PRIu32 "\n", msg_id, ofi_recvwin_next_exp_id(robuf));
			return -FI_EALREADY;
		} else {
			fprintf(stderr,
				"Current receive window size (%d) is too small to hold incoming messages.\n"
				"As a result, you application cannot proceed.\n"
				"Receive window size can be increased by setting the environment variable:\n"
				"              FI_EFA_RECVWIN_SIZE\n"
				"\n"
				"Your job will now abort.\n\n", rxr_env.recvwin_size);
			abort();
		}
	}

	if (OFI_LIKELY(rxr_env.rx_copy_ooo)) {
		assert(pkt_entry->alloc_type == RXR_PKT_FROM_EFA_RX_POOL);
		ooo_entry = rxr_pkt_entry_clone(ep, ep->rx_ooo_pkt_pool, RXR_PKT_FROM_OOO_POOL, pkt_entry);
		if (OFI_UNLIKELY(!ooo_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Unable to allocate rx_pkt_entry for OOO msg\n");
			return -FI_ENOMEM;
		}
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	} else {
		ooo_entry = pkt_entry;
	}

	cur_ooo_entry = *ofi_recvwin_get_msg(robuf, msg_id);
	if (cur_ooo_entry) {
		assert(rxr_pkt_type_is_mulreq(rxr_get_base_hdr(cur_ooo_entry->pkt)->type));
		assert(rxr_pkt_msg_id(cur_ooo_entry) == msg_id);
		assert(rxr_pkt_rtm_total_len(cur_ooo_entry) == rxr_pkt_rtm_total_len(ooo_entry));
		rxr_pkt_entry_append(cur_ooo_entry, ooo_entry);
	} else {
		ofi_recvwin_queue_msg(robuf, &ooo_entry, msg_id);
	}

	return 1;
}

void rxr_cq_proc_pending_items_in_recvwin(struct rxr_ep *ep,
					  struct rdm_peer *peer)
{
	struct rxr_pkt_entry *pending_pkt;
	int ret = 0;
	uint32_t msg_id;

	while (1) {
		pending_pkt = *ofi_recvwin_peek((&peer->robuf));
		if (!pending_pkt)
			return;

		msg_id = rxr_pkt_msg_id(pending_pkt);
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "Processing msg_id %d from robuf\n", msg_id);
		/* rxr_pkt_proc_rtm_rta will write error cq entry if needed */
		ret = rxr_pkt_proc_rtm_rta(ep, pending_pkt);
		*ofi_recvwin_get_next_msg((&peer->robuf)) = NULL;
		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Error processing msg_id %d from robuf: %s\n",
				msg_id, fi_strerror(-ret));
			return;
		}
	}
	return;
}

/* Handle two scenarios:
 *  1. RMA writes with immediate data at remote endpoint,
 *  2. atomic completion on the requester
 * write completion for both
 */
void rxr_cq_handle_shm_completion(struct rxr_ep *ep, struct fi_cq_data_entry *cq_entry, fi_addr_t src_addr)
{
	struct util_cq *target_cq;
	int ret;

	if (cq_entry->flags & FI_ATOMIC) {
		target_cq = ep->util_ep.tx_cq;
	} else {
		assert(cq_entry->flags & FI_REMOTE_CQ_DATA);
		target_cq = ep->util_ep.rx_cq;
	}

	if (ep->util_ep.caps & FI_SOURCE)
		ret = ofi_cq_write_src(target_cq,
				       cq_entry->op_context,
				       cq_entry->flags,
				       cq_entry->len,
				       cq_entry->buf,
				       cq_entry->data,
				       0,
				       src_addr);
	else
		ret = ofi_cq_write(target_cq,
				   cq_entry->op_context,
				   cq_entry->flags,
				   cq_entry->len,
				   cq_entry->buf,
				   cq_entry->data,
				   0);

	rxr_rm_rx_cq_check(ep, target_cq);

	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Unable to write a cq entry for shm operation: %s\n",
			fi_strerror(-ret));
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_WRITE_SHM_CQ_ENTRY);
	}

	if (cq_entry->flags & FI_ATOMIC) {
		efa_cntr_report_tx_completion(&ep->util_ep, cq_entry->flags);
	} else {
		assert(cq_entry->flags & FI_REMOTE_CQ_DATA);
		efa_cntr_report_rx_completion(&ep->util_ep, cq_entry->flags);
	}
}

static inline
bool rxr_cq_need_tx_completion(struct rxr_ep *ep,
			       struct rxr_op_entry *tx_entry)

{
	if (tx_entry->fi_flags & RXR_NO_COMPLETION)
		return false;

	/*
	 * ep->util_ep.tx_msg_flags is either 0 or FI_COMPLETION, depend on
	 * whether app specfied FI_SELECTIVE_COMPLETION when binding CQ.
	 * (ep->util_ep.tx_msg_flags was set in ofi_ep_bind_cq())
	 *
	 * If tx_msg_flags is 0, we only write completion when app specify
	 * FI_COMPLETION in flags.
	 */
	return ep->util_ep.tx_msg_flags == FI_COMPLETION ||
	       tx_entry->fi_flags & FI_COMPLETION;
}

/**
 * @brief write a cq entry for an operation (send/read/write) if application wants it.
 *        Sometimes application does not want to receive a cq entry for a tx operation.
 *
 * @param[in]	ep			end point
 * @param[in]	op_entry	tx entry that contains information of the TX operation
 */
void rxr_cq_write_tx_completion(struct rxr_ep *ep,
				struct rxr_op_entry *tx_entry)
{
	struct util_cq *tx_cq = ep->util_ep.tx_cq;
	int ret;

	assert(tx_entry->type == RXR_TX_ENTRY);
	if (rxr_cq_need_tx_completion(ep, tx_entry)) {
		FI_DBG(&rxr_prov, FI_LOG_CQ,
		       "Writing send completion for tx_entry to peer: %" PRIu64
		       " tx_id: %" PRIu32 " msg_id: %" PRIu32 " tag: %lx len: %"
		       PRIu64 "\n",
		       tx_entry->addr, tx_entry->tx_id, tx_entry->msg_id,
		       tx_entry->cq_entry.tag, tx_entry->total_len);


	rxr_tracing(send_end, 
		    tx_entry->msg_id, (size_t) tx_entry->cq_entry.op_context, 
		    tx_entry->total_len, tx_entry->cq_entry.tag, tx_entry->addr);

		/* TX completions should not send peer address to util_cq */
		if (ep->util_ep.caps & FI_SOURCE)
			ret = ofi_cq_write_src(tx_cq,
					       tx_entry->cq_entry.op_context,
					       tx_entry->cq_entry.flags,
					       tx_entry->cq_entry.len,
					       tx_entry->cq_entry.buf,
					       tx_entry->cq_entry.data,
					       tx_entry->cq_entry.tag,
					       FI_ADDR_NOTAVAIL);
		else
			ret = ofi_cq_write(tx_cq,
					   tx_entry->cq_entry.op_context,
					   tx_entry->cq_entry.flags,
					   tx_entry->cq_entry.len,
					   tx_entry->cq_entry.buf,
					   tx_entry->cq_entry.data,
					   tx_entry->cq_entry.tag);

		rxr_rm_tx_cq_check(ep, tx_cq);

		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Unable to write send completion: %s\n",
				fi_strerror(-ret));
			rxr_cq_write_tx_error(ep, tx_entry, -ret, FI_EFA_ERR_WRITE_SEND_COMP);
			return;
		}
	}

	efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
	tx_entry->fi_flags |= RXR_NO_COMPLETION;
	return;
}

/**
 * @brief This functions handles completion process after all data bytes of a message are sent 
 *  	  and the receiver/requestor has acknowledged that the sent bytes were received. 
 *
 * @param[in]	ep		end point
 * @param[in]	op_entry	tx/rx entry that contains information of the data send operation  
 */
void rxr_cq_handle_send_completion(struct rxr_ep *ep, struct rxr_op_entry *op_entry)
{
	struct rxr_op_entry *rx_entry;

	if (op_entry->state == RXR_TX_SEND)
		dlist_remove(&op_entry->entry);

	if (op_entry->cq_entry.flags & FI_READ) {
		/*
		 * This is on responder side of an emulated read operation.
		 * In this case, we do not write any completion.
		 * The entry is allocated for emulated read, so no need to write tx completion.
		 * EFA does not support FI_RMA_EVENT, so no need to write rx completion.
		 */
		assert(op_entry->type == RXR_RX_ENTRY);
		rx_entry = op_entry;
		rxr_release_rx_entry(ep, rx_entry);
		return;
	} else if (op_entry->cq_entry.flags & FI_WRITE) {
		if (op_entry->fi_flags & FI_COMPLETION) {
			rxr_cq_write_tx_completion(ep, op_entry);
		} else {
			if (!(op_entry->fi_flags & RXR_NO_COUNTER))
				efa_cntr_report_tx_completion(&ep->util_ep, op_entry->cq_entry.flags);
		}

	} else {
		assert(op_entry->cq_entry.flags & FI_SEND);
		rxr_cq_write_tx_completion(ep, op_entry);
	}

	assert(op_entry->type == RXR_TX_ENTRY);
	rxr_release_tx_entry(ep, op_entry);
}

static int rxr_cq_close(struct fid *fid)
{
	int ret;
	struct util_cq *cq;

	cq = container_of(fid, struct util_cq, cq_fid.fid);
	ret = ofi_cq_cleanup(cq);
	if (ret)
		return ret;
	free(cq);
	return 0;
}

static struct fi_ops rxr_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxr_cq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cq rxr_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = fi_no_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = rxr_cq_strerror,
};

int rxr_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	int ret;
	struct util_cq *cq;
	struct efa_domain *efa_domain;

	if (attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	efa_domain = container_of(domain, struct efa_domain,
				  util_domain.domain_fid);
	/* Override user cq size if it's less than recommended cq size */
	attr->size = MAX(efa_domain->rdm_cq_size, attr->size);

	ret = ofi_cq_init(&rxr_prov, domain, attr, cq,
			  &ofi_cq_progress, context);

	if (ret)
		goto free;

	*cq_fid = &cq->cq_fid;
	(*cq_fid)->fid.ops = &rxr_cq_fi_ops;
	(*cq_fid)->ops = &rxr_cq_ops;
	return 0;
free:
	free(cq);
	return ret;
}
