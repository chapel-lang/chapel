/*
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
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

#include "efa.h"
#include "rxr.h"
#include "rxr_msg.h"
#include "rxr_cntr.h"
#include "rxr_pkt_cmd.h"
#include "rxr_read.h"

/* This file define functons for the following packet type:
 *       HANDSHAKE
 *       CTS
 *       READRSP
 *       RMA_CONTEXT
 *       EOR
 */

/* HANDSHAKE packet related functions */
ssize_t rxr_pkt_init_handshake(struct rxr_ep *ep,
			       struct rxr_pkt_entry *pkt_entry,
			       fi_addr_t addr)
{
	struct rxr_handshake_hdr *handshake_hdr;

	handshake_hdr = (struct rxr_handshake_hdr *)pkt_entry->pkt;
	handshake_hdr->type = RXR_HANDSHAKE_PKT;
	handshake_hdr->version = RXR_BASE_PROTOCOL_VERSION;
	handshake_hdr->flags = 0;
	handshake_hdr->maxproto = RXR_CUR_PROTOCOL_VERSION;
	memcpy(handshake_hdr->features, ep->features,
	       RXR_NUM_PROTOCOL_VERSION * sizeof(uint64_t));

	pkt_entry->pkt_size = sizeof(struct rxr_handshake_hdr)
			      + RXR_NUM_PROTOCOL_VERSION * sizeof(uint64_t);
	pkt_entry->addr = addr;
	return 0;
}

/** @brief Post a handshake packet to a peer.
 *
 * @param ep The endpoint on which the handshake packet is sent out.
 * @param peer The peer to which the handshake packet is posted.
 * @return 0 on success, fi_errno on error.
 */
ssize_t rxr_pkt_post_handshake(struct rxr_ep *ep, struct rxr_peer *peer)
{
	struct rxr_pkt_entry *pkt_entry;
	fi_addr_t addr;
	ssize_t ret;

	addr = peer->efa_fiaddr;
	pkt_entry = rxr_pkt_entry_alloc(ep, ep->tx_pkt_efa_pool);
	if (OFI_UNLIKELY(!pkt_entry))
		return -FI_EAGAIN;

	rxr_pkt_init_handshake(ep, pkt_entry, addr);

	ret = rxr_pkt_entry_send(ep, pkt_entry, addr);
	if (OFI_UNLIKELY(ret)) {
		rxr_pkt_entry_release_tx(ep, pkt_entry);
	}
	return ret;
}

/** @brief Post a handshake packet to a peer.
 *
 * Note that if FI_EAGAIN is returned from the post of the handshake
 * packet, the peer will be added into queue_peer_list for retry
 * later. For other errors, we will hard fail.
 *
 * @param ep The endpoint on which the handshake packet is sent out.
 * @param peer The peer to which the handshake packet is posted.
 * @return Void.
 */
void rxr_pkt_post_handshake_or_queue(struct rxr_ep *ep, struct rxr_peer *peer)
{
	ssize_t ret;

	assert(!(peer->flags & RXR_PEER_HANDSHAKE_SENT_OR_QUEUED));

	ret = rxr_pkt_post_handshake(ep, peer);
	if (OFI_UNLIKELY(ret)) {
		if (ret == -FI_EAGAIN) {
			/* add peer to peer_queued_list for retry later */
			dlist_insert_tail(&peer->queued_entry,
					  &ep->peer_queued_list);
		} else {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Failed to post HANDSHAKE to peer %ld: %s\n",
				peer->efa_fiaddr, fi_strerror(-ret));
			assert(0 && "Failed to post HANDSHAKE to peer");
			efa_eq_write_error(&ep->util_ep, FI_EIO, -ret);
		}
	}
	/*
	 * If rxr_pkt_post_handshake returns success or FI_EAGAIN,
	 * set the flag to RXR_PEER_HANDSHAKE_SENT_OR_QUEUED to
	 * avoid posting handshake packet multiple times to the
	 * same peer.
	 */
	peer->flags |= RXR_PEER_HANDSHAKE_SENT_OR_QUEUED;
}

void rxr_pkt_handle_handshake_recv(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_peer *peer;
	struct rxr_handshake_hdr *handshake_pkt;

	assert(pkt_entry->addr != FI_ADDR_NOTAVAIL);

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED));

	handshake_pkt = (struct rxr_handshake_hdr *)pkt_entry->pkt;

	peer->maxproto = handshake_pkt->maxproto;
	memcpy(peer->features, handshake_pkt->features,
	       (handshake_pkt->maxproto - RXR_BASE_PROTOCOL_VERSION + 1) * sizeof(uint64_t));
	peer->flags |= RXR_PEER_HANDSHAKE_RECEIVED;
	FI_DBG(&rxr_prov, FI_LOG_CQ,
	       "HANDSHAKE received from %" PRIu64 "\n", pkt_entry->addr);
	rxr_pkt_entry_release_rx(ep, pkt_entry);

}

/*  CTS packet related functions */
void rxr_pkt_calc_cts_window_credits(struct rxr_ep *ep, struct rxr_peer *peer,
				     uint64_t size, int request,
				     int *window, int *credits)
{
	struct efa_av *av;
	int num_peers;

	/*
	 * Adjust the peer credit pool based on the current AV size, which could
	 * have grown since the time this peer was initialized.
	 */
	av = rxr_ep_av(ep);
	num_peers = av->used - 1;
	if (num_peers && ofi_div_ceil(rxr_env.rx_window_size, num_peers) < peer->rx_credits)
		peer->rx_credits = ofi_div_ceil(peer->rx_credits, num_peers);

	/*
	 * Allocate credits for this transfer based on the request, the number
	 * of available data buffers, and the number of outstanding peers this
	 * endpoint is actively tracking in the AV. Also ensure that a minimum
	 * number of credits are allocated to the transfer so the sender can
	 * make progress.
	 */
	*credits = MIN(MIN(ep->available_data_bufs, ep->posted_bufs_efa),
		       peer->rx_credits);
	*credits = MIN(request, *credits);
	*credits = MAX(*credits, rxr_env.tx_min_credits);
	*window = MIN(size, *credits * ep->max_data_payload_size);
	if (peer->rx_credits > ofi_div_ceil(*window, ep->max_data_payload_size))
		peer->rx_credits -= ofi_div_ceil(*window, ep->max_data_payload_size);
}

ssize_t rxr_pkt_init_cts(struct rxr_ep *ep,
			 struct rxr_rx_entry *rx_entry,
			 struct rxr_pkt_entry *pkt_entry)
{
	int window = 0;
	struct rxr_cts_hdr *cts_hdr;
	struct rxr_peer *peer;
	size_t bytes_left;

	cts_hdr = (struct rxr_cts_hdr *)pkt_entry->pkt;
	cts_hdr->type = RXR_CTS_PKT;
	cts_hdr->version = RXR_BASE_PROTOCOL_VERSION;
	cts_hdr->flags = 0;

	if (rx_entry->cq_entry.flags & FI_READ)
		cts_hdr->flags |= RXR_CTS_READ_REQ;

	cts_hdr->tx_id = rx_entry->tx_id;
	cts_hdr->rx_id = rx_entry->rx_id;

	bytes_left = rx_entry->total_len - rx_entry->bytes_received;
	peer = rxr_ep_get_peer(ep, rx_entry->addr);
	rxr_pkt_calc_cts_window_credits(ep, peer, bytes_left,
					rx_entry->credit_request,
					&window, &rx_entry->credit_cts);
	cts_hdr->window = window;
	pkt_entry->pkt_size = sizeof(struct rxr_cts_hdr);
	pkt_entry->addr = rx_entry->addr;
	pkt_entry->x_entry = (void *)rx_entry;
	return 0;
}

void rxr_pkt_handle_cts_sent(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;

	rx_entry = (struct rxr_rx_entry *)pkt_entry->x_entry;
	rx_entry->window = rxr_get_cts_hdr(pkt_entry->pkt)->window;
	ep->available_data_bufs -= rx_entry->credit_cts;

	/*
	 * Set a timer if available_bufs is exhausted. We may encounter a
	 * scenario where a peer has stopped responding so we need a fallback
	 * to replenish the credits.
	 */
	if (OFI_UNLIKELY(ep->available_data_bufs == 0))
		ep->available_data_bufs_ts = ofi_gettime_us();
}

void rxr_pkt_handle_cts_recv(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_peer *peer;
	struct rxr_cts_hdr *cts_pkt;
	struct rxr_tx_entry *tx_entry;

	cts_pkt = (struct rxr_cts_hdr *)pkt_entry->pkt;
	if (cts_pkt->flags & RXR_CTS_READ_REQ)
		tx_entry = ofi_bufpool_get_ibuf(ep->readrsp_tx_entry_pool, cts_pkt->tx_id);
	else
		tx_entry = ofi_bufpool_get_ibuf(ep->tx_entry_pool, cts_pkt->tx_id);

	tx_entry->rx_id = cts_pkt->rx_id;
	tx_entry->window = cts_pkt->window;

	/* Return any excess tx_credits that were borrowed for the request */
	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	tx_entry->credit_allocated = ofi_div_ceil(cts_pkt->window, ep->max_data_payload_size);
	if (tx_entry->credit_allocated < tx_entry->credit_request)
		peer->tx_credits += tx_entry->credit_request - tx_entry->credit_allocated;

	rxr_pkt_entry_release_rx(ep, pkt_entry);

	if (tx_entry->state != RXR_TX_SEND) {
		tx_entry->state = RXR_TX_SEND;
		dlist_insert_tail(&tx_entry->entry, &ep->tx_pending_list);
	}
}

/*  READRSP packet functions */
int rxr_pkt_init_readrsp(struct rxr_ep *ep,
			 struct rxr_tx_entry *tx_entry,
			 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_readrsp_pkt *readrsp_pkt;
	struct rxr_readrsp_hdr *readrsp_hdr;
	size_t mtu = ep->mtu_size;

	readrsp_pkt = (struct rxr_readrsp_pkt *)pkt_entry->pkt;
	readrsp_hdr = &readrsp_pkt->hdr;
	readrsp_hdr->type = RXR_READRSP_PKT;
	readrsp_hdr->version = RXR_BASE_PROTOCOL_VERSION;
	readrsp_hdr->flags = 0;
	readrsp_hdr->tx_id = tx_entry->tx_id;
	readrsp_hdr->rx_id = tx_entry->rx_id;
	readrsp_hdr->seg_size = ofi_copy_from_iov(readrsp_pkt->data,
						  mtu - RXR_READRSP_HDR_SIZE,
						  tx_entry->iov,
						  tx_entry->iov_count, 0);
	pkt_entry->pkt_size = RXR_READRSP_HDR_SIZE + readrsp_hdr->seg_size;
	pkt_entry->addr = tx_entry->addr;
	pkt_entry->x_entry = tx_entry;
	return 0;
}

void rxr_pkt_handle_readrsp_sent(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;
	size_t data_len;
	struct efa_domain *efa_domain;
	struct rxr_domain *rxr_domain = rxr_ep_domain(ep);

	efa_domain = container_of(rxr_domain->rdm_domain, struct efa_domain,
				  util_domain.domain_fid);

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	data_len = rxr_get_readrsp_hdr(pkt_entry->pkt)->seg_size;
	tx_entry->bytes_sent += data_len;
	tx_entry->window -= data_len;
	assert(tx_entry->window >= 0);
	if (tx_entry->bytes_sent < tx_entry->total_len) {
		assert(!efa_ep_is_cuda_mr(tx_entry->desc[0]));
		if (tx_entry->desc[0] || efa_is_cache_available(efa_domain))
			rxr_prepare_desc_send(rxr_ep_domain(ep), tx_entry);

		tx_entry->state = RXR_TX_SEND;
		dlist_insert_tail(&tx_entry->entry,
				  &ep->tx_pending_list);
	}
}

void rxr_pkt_handle_readrsp_send_completion(struct rxr_ep *ep,
					    struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;
	struct rxr_readrsp_hdr *readrsp_hdr;

	readrsp_hdr = (struct rxr_readrsp_hdr *)pkt_entry->pkt;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	assert(tx_entry->cq_entry.flags & FI_READ);

	tx_entry->bytes_acked += readrsp_hdr->seg_size;
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_tx_completion(ep, tx_entry);
}

void rxr_pkt_handle_readrsp_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_readrsp_pkt *readrsp_pkt = NULL;
	struct rxr_readrsp_hdr *readrsp_hdr = NULL;
	struct rxr_rx_entry *rx_entry = NULL;

	readrsp_pkt = (struct rxr_readrsp_pkt *)pkt_entry->pkt;
	readrsp_hdr = &readrsp_pkt->hdr;
	rx_entry = ofi_bufpool_get_ibuf(ep->rx_entry_pool, readrsp_hdr->rx_id);
	assert(rx_entry->cq_entry.flags & FI_READ);
	rx_entry->tx_id = readrsp_hdr->tx_id;
	rxr_pkt_proc_data(ep, rx_entry, pkt_entry,
			  readrsp_pkt->data,
			  0, readrsp_hdr->seg_size);
}

/*  RMA_CONTEXT packet functions
 *
 *  RMA context packet is used a context of RMA operations and is not
 *  sent over wire. It is named packet because currently all EFA operation
 *  use a packet as context.
 */
void rxr_pkt_init_write_context(struct rxr_tx_entry *tx_entry,
				struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rma_context_pkt *rma_context_pkt;

	pkt_entry->x_entry = (void *)tx_entry;
	rma_context_pkt = (struct rxr_rma_context_pkt *)pkt_entry->pkt;
	rma_context_pkt->type = RXR_RMA_CONTEXT_PKT;
	rma_context_pkt->version = RXR_BASE_PROTOCOL_VERSION;
	rma_context_pkt->context_type = RXR_WRITE_CONTEXT;
	rma_context_pkt->tx_id = tx_entry->tx_id;
}

void rxr_pkt_init_read_context(struct rxr_ep *rxr_ep,
			       struct rxr_read_entry *read_entry,
			       size_t seg_size,
			       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rma_context_pkt *ctx_pkt;

	pkt_entry->x_entry = read_entry;
	pkt_entry->addr = read_entry->addr;
	pkt_entry->pkt_size = sizeof(struct rxr_rma_context_pkt);

	ctx_pkt = (struct rxr_rma_context_pkt *)pkt_entry->pkt;
	ctx_pkt->type = RXR_RMA_CONTEXT_PKT;
	ctx_pkt->flags = 0;
	ctx_pkt->version = RXR_BASE_PROTOCOL_VERSION;
	ctx_pkt->context_type = RXR_READ_CONTEXT;
	ctx_pkt->read_id = read_entry->read_id;
	ctx_pkt->seg_size = seg_size;
}

static
void rxr_pkt_handle_rma_read_completion(struct rxr_ep *ep,
					struct rxr_pkt_entry *context_pkt_entry)
{
	struct rxr_tx_entry *tx_entry;
	struct rxr_rx_entry *rx_entry;
	struct rxr_pkt_entry *pkt_entry;
	struct rxr_read_entry *read_entry;
	struct rxr_rma_context_pkt *rma_context_pkt;
	struct rxr_peer *peer;
	int inject;
	size_t data_size;
	ssize_t ret;

	rma_context_pkt = (struct rxr_rma_context_pkt *)context_pkt_entry->pkt;
	assert(rma_context_pkt->type == RXR_RMA_CONTEXT_PKT);
	assert(rma_context_pkt->context_type == RXR_READ_CONTEXT);

	read_entry = (struct rxr_read_entry *)context_pkt_entry->x_entry;
	read_entry->bytes_finished += rma_context_pkt->seg_size;
	assert(read_entry->bytes_finished <= read_entry->total_len);

	if (read_entry->bytes_finished == read_entry->total_len) {
		if (read_entry->context_type == RXR_READ_CONTEXT_TX_ENTRY) {
			tx_entry = read_entry->context;
			assert(tx_entry && tx_entry->cq_entry.flags & FI_READ);
			rxr_cq_write_tx_completion(ep, tx_entry);
		} else if (read_entry->context_type == RXR_READ_CONTEXT_RX_ENTRY) {
			rx_entry = read_entry->context;
			if (rx_entry->op == ofi_op_msg || rx_entry->op == ofi_op_tagged) {
				rxr_cq_write_rx_completion(ep, rx_entry);
			} else {
				assert(rx_entry->op == ofi_op_write);
				if (rx_entry->cq_entry.flags & FI_REMOTE_CQ_DATA)
					rxr_cq_write_rx_completion(ep, rx_entry);
			}

			inject = (read_entry->lower_ep_type == SHM_EP);
			ret = rxr_pkt_post_ctrl_or_queue(ep, RXR_RX_ENTRY, rx_entry, RXR_EOR_PKT, inject);
			if (OFI_UNLIKELY(ret)) {
				if (rxr_cq_handle_rx_error(ep, rx_entry, ret))
					assert(0 && "failed to write err cq entry");
				rxr_release_rx_entry(ep, rx_entry);
			}
		} else {
			assert(read_entry->context_type == RXR_READ_CONTEXT_PKT_ENTRY);
			pkt_entry = read_entry->context;
			data_size = rxr_pkt_data_size(pkt_entry);
			assert(data_size > 0);
			rxr_pkt_handle_data_copied(ep, pkt_entry, data_size);
		}

		rxr_read_release_entry(ep, read_entry);
	}

	if (read_entry->context_type == RXR_READ_CONTEXT_PKT_ENTRY) {
		assert(context_pkt_entry->addr == FI_ADDR_NOTAVAIL);
		ep->tx_pending--;
	} else {
		peer = rxr_ep_get_peer(ep, context_pkt_entry->addr);
		if (!peer->is_local)
			rxr_ep_dec_tx_pending(ep, peer, 0);
	}
}

void rxr_pkt_handle_rma_completion(struct rxr_ep *ep,
				   struct rxr_pkt_entry *context_pkt_entry)
{
	struct rxr_tx_entry *tx_entry = NULL;
	struct rxr_rma_context_pkt *rma_context_pkt;

	assert(rxr_get_base_hdr(context_pkt_entry->pkt)->version == RXR_BASE_PROTOCOL_VERSION);

	rma_context_pkt = (struct rxr_rma_context_pkt *)context_pkt_entry->pkt;

	switch (rma_context_pkt->context_type) {
	case RXR_WRITE_CONTEXT:
		tx_entry = (struct rxr_tx_entry *)context_pkt_entry->x_entry;
		if (tx_entry->fi_flags & FI_COMPLETION) {
			rxr_cq_write_tx_completion(ep, tx_entry);
		} else {
			efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
			rxr_release_tx_entry(ep, tx_entry);
		}
		break;
	case RXR_READ_CONTEXT:
		rxr_pkt_handle_rma_read_completion(ep, context_pkt_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid rma_context_type in RXR_RMA_CONTEXT_PKT %d\n",
			rma_context_pkt->context_type);
		assert(0 && "invalid RXR_RMA_CONTEXT_PKT rma_context_type\n");
	}

	rxr_pkt_entry_release_tx(ep, context_pkt_entry);
}

/*  EOR packet related functions */
int rxr_pkt_init_eor(struct rxr_ep *ep, struct rxr_rx_entry *rx_entry, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_eor_hdr *eor_hdr;

	eor_hdr = (struct rxr_eor_hdr *)pkt_entry->pkt;
	eor_hdr->type = RXR_EOR_PKT;
	eor_hdr->version = RXR_BASE_PROTOCOL_VERSION;
	eor_hdr->flags = 0;
	eor_hdr->tx_id = rx_entry->tx_id;
	eor_hdr->rx_id = rx_entry->rx_id;
	pkt_entry->pkt_size = sizeof(struct rxr_eor_hdr);
	pkt_entry->addr = rx_entry->addr;
	pkt_entry->x_entry = rx_entry;
	return 0;
}

void rxr_pkt_handle_eor_sent(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_eor_send_completion(struct rxr_ep *ep,
					struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;

	rx_entry = pkt_entry->x_entry;
	assert(rx_entry && rx_entry->rx_id == rxr_get_eor_hdr(pkt_entry->pkt)->rx_id);
	rxr_release_rx_entry(ep, rx_entry);
}

/*
 *   Sender handles the acknowledgment (RXR_EOR_PKT) from receiver on the completion
 *   of the large message copy via fi_readmsg operation
 */
void rxr_pkt_handle_eor_recv(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_eor_hdr *eor_hdr;
	struct rxr_tx_entry *tx_entry;

	eor_hdr = (struct rxr_eor_hdr *)pkt_entry->pkt;

	/* pre-post buf used here, so can NOT track back to tx_entry with x_entry */
	tx_entry = ofi_bufpool_get_ibuf(ep->tx_entry_pool, eor_hdr->tx_id);
	rxr_cq_write_tx_completion(ep, tx_entry);
	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

/* receipt packet related functions */
int rxr_pkt_init_receipt(struct rxr_ep *ep, struct rxr_rx_entry *rx_entry,
			 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_receipt_hdr *receipt_hdr;

	receipt_hdr = rxr_get_receipt_hdr(pkt_entry->pkt);
	receipt_hdr->type = RXR_RECEIPT_PKT;
	receipt_hdr->version = RXR_BASE_PROTOCOL_VERSION;
	receipt_hdr->flags = 0;
	receipt_hdr->tx_id = rx_entry->tx_id;
	receipt_hdr->msg_id = rx_entry->msg_id;

	pkt_entry->pkt_size = sizeof(struct rxr_receipt_hdr);
	pkt_entry->addr = rx_entry->addr;
	pkt_entry->x_entry = rx_entry;

	return 0;
}

void rxr_pkt_handle_receipt_sent(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_receipt_send_completion(struct rxr_ep *ep,
					    struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;

	rx_entry = (struct rxr_rx_entry *)pkt_entry->x_entry;
	rxr_release_rx_entry(ep, rx_entry);
}

/* atomrsp packet related functions: init, handle_sent, handle_send_completion and recv
 *
 * initialize atomic response packet by creating a packet that hold original data
 * in rx_entry->iov. rx_entry->iov will then be changed by atomic operation.
 * release that packet entry until it is sent.
 */
int rxr_pkt_init_atomrsp(struct rxr_ep *ep, struct rxr_rx_entry *rx_entry,
			 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_atomrsp_hdr *atomrsp_hdr;

	assert(rx_entry->atomrsp_data);
	pkt_entry->addr = rx_entry->addr;
	pkt_entry->x_entry = rx_entry;

	atomrsp_hdr = (struct rxr_atomrsp_hdr *)pkt_entry->pkt;
	atomrsp_hdr->type = RXR_ATOMRSP_PKT;
	atomrsp_hdr->version = RXR_BASE_PROTOCOL_VERSION;
	atomrsp_hdr->flags = 0;
	atomrsp_hdr->tx_id = rx_entry->tx_id;
	atomrsp_hdr->rx_id = rx_entry->rx_id;
	atomrsp_hdr->seg_size = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);

	assert(RXR_ATOMRSP_HDR_SIZE + atomrsp_hdr->seg_size < ep->mtu_size);

	/* rx_entry->atomrsp_data was filled in rxr_pkt_handle_req_recv() */
	memcpy((char*)pkt_entry->pkt + RXR_ATOMRSP_HDR_SIZE, rx_entry->atomrsp_data, atomrsp_hdr->seg_size);
	pkt_entry->pkt_size = RXR_ATOMRSP_HDR_SIZE + atomrsp_hdr->seg_size;
	return 0;
}

void rxr_pkt_handle_atomrsp_sent(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_atomrsp_send_completion(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	
	rx_entry = (struct rxr_rx_entry *)pkt_entry->x_entry;
	ofi_buf_free(rx_entry->atomrsp_data);
	rx_entry->atomrsp_data = NULL;
	rxr_release_rx_entry(ep, rx_entry);
}

void rxr_pkt_handle_atomrsp_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_atomrsp_pkt *atomrsp_pkt = NULL;
	struct rxr_atomrsp_hdr *atomrsp_hdr = NULL;
	struct rxr_tx_entry *tx_entry = NULL;

	atomrsp_pkt = (struct rxr_atomrsp_pkt *)pkt_entry->pkt;
	atomrsp_hdr = &atomrsp_pkt->hdr;
	tx_entry = ofi_bufpool_get_ibuf(ep->tx_entry_pool, atomrsp_hdr->tx_id);

	ofi_copy_to_iov(tx_entry->atomic_ex.resp_iov,
			tx_entry->atomic_ex.resp_iov_count,
			0, atomrsp_pkt->data,
			atomrsp_hdr->seg_size);

	if (tx_entry->fi_flags & FI_COMPLETION) {
		/* Note write_tx_completion() will release tx_entry */
		rxr_cq_write_tx_completion(ep, tx_entry);
	} else {
		efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
		rxr_release_tx_entry(ep, tx_entry);
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

void rxr_pkt_handle_receipt_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry = NULL;
	struct rxr_receipt_hdr *receipt_hdr;

	receipt_hdr = rxr_get_receipt_hdr(pkt_entry->pkt);
	/* Retrieve the tx_entry that will be written into TX CQ*/
	tx_entry = ofi_bufpool_get_ibuf(ep->tx_entry_pool,
					receipt_hdr->tx_id);
	if (!tx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Failed to retrive the tx_entry when hadling receipt packet.\n");
		return;
	}

	tx_entry->rxr_flags |= RXR_RECEIPT_RECEIVED;
	if (tx_entry->rxr_flags & RXR_LONGCTS_PROTOCOL) {
		/*
		 * For long message protocol, when FI_DELIVERY_COMPLETE
		 * is requested, we have to write tx completions
		 * in either rxr_pkt_handle_data_send_completion()
		 * or rxr_pkt_handle_receipt_recv() depending on which of them
		 * is called later due to avoid accessing released
		 * tx_entry.
		 */
		if (tx_entry->total_len == tx_entry->bytes_acked)
			rxr_cq_handle_tx_completion(ep, tx_entry);
	} else {
		rxr_cq_handle_tx_completion(ep, tx_entry);
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

