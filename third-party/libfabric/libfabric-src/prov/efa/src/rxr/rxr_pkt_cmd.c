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
#include "rxr_read.h"
#include "rxr_pkt_cmd.h"

/* Handshake wait timeout in microseconds */
#define RXR_HANDSHAKE_WAIT_TIMEOUT 1000000

/* This file implements 4 actions that can be applied to a packet:
 *          posting,
 *          handling send completion and,
 *          handing recv completion.
 *          dump (for debug only)
 */

/*
 *  Functions used to post a packet
 */
ssize_t rxr_pkt_post_data(struct rxr_ep *rxr_ep,
			  struct rxr_tx_entry *tx_entry)
{
	struct rxr_pkt_entry *pkt_entry;
	struct rxr_data_pkt *data_pkt;
	ssize_t ret;

	pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->efa_tx_pkt_pool, RXR_PKT_FROM_EFA_TX_POOL);
	if (OFI_UNLIKELY(!pkt_entry)) {
		FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
		       "TX packets exhausted, current tx ops in flight %lu",
		       rxr_ep->efa_outstanding_tx_ops);
		return -FI_EAGAIN;
	}

	pkt_entry->x_entry = (void *)tx_entry;
	pkt_entry->addr = tx_entry->addr;

	data_pkt = (struct rxr_data_pkt *)pkt_entry->pkt;

	data_pkt->hdr.type = RXR_DATA_PKT;
	data_pkt->hdr.version = RXR_BASE_PROTOCOL_VERSION;
	data_pkt->hdr.flags = 0;

	data_pkt->hdr.rx_id = tx_entry->rx_id;

	/*
	 * Data packets are sent in order so using bytes_sent is okay here.
	 */
	data_pkt->hdr.seg_offset = tx_entry->bytes_sent;

	if (tx_entry->desc[0])
		ret = rxr_pkt_send_data_desc(rxr_ep, tx_entry, pkt_entry);
	else
		ret = rxr_pkt_send_data(rxr_ep, tx_entry, pkt_entry);

	if (OFI_UNLIKELY(ret)) {
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);
		return ret;
	}

	data_pkt = rxr_get_data_pkt(pkt_entry->pkt);
	tx_entry->bytes_sent += data_pkt->hdr.seg_size;
	tx_entry->window -= data_pkt->hdr.seg_size;
	assert(data_pkt->hdr.seg_size > 0);
	assert(tx_entry->window >= 0);
	return ret;
}

/*
 *   rxr_pkt_init_ctrl() uses init functions declared in rxr_pkt_type.h
 */
static
int rxr_pkt_init_ctrl(struct rxr_ep *rxr_ep, int entry_type, void *x_entry,
		      int ctrl_type, struct rxr_pkt_entry *pkt_entry)
{
	int ret = 0;

	switch (ctrl_type) {
	case RXR_READRSP_PKT:
		ret = rxr_pkt_init_readrsp(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_CTS_PKT:
		ret = rxr_pkt_init_cts(rxr_ep, (struct rxr_rx_entry *)x_entry, pkt_entry);
		break;
	case RXR_EOR_PKT:
		ret = rxr_pkt_init_eor(rxr_ep, (struct rxr_rx_entry *)x_entry, pkt_entry);
		break;
	case RXR_ATOMRSP_PKT:
		ret = rxr_pkt_init_atomrsp(rxr_ep, (struct rxr_rx_entry *)x_entry, pkt_entry);
		break;
	case RXR_RECEIPT_PKT:
		ret = rxr_pkt_init_receipt(rxr_ep, (struct rxr_rx_entry *)x_entry, pkt_entry);
		break;
	case RXR_EAGER_MSGRTM_PKT:
		ret = rxr_pkt_init_eager_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_EAGER_TAGRTM_PKT:
		ret = rxr_pkt_init_eager_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_MEDIUM_MSGRTM_PKT:
		ret = rxr_pkt_init_medium_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_MEDIUM_TAGRTM_PKT:
		ret = rxr_pkt_init_medium_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONG_MSGRTM_PKT:
		ret = rxr_pkt_init_long_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONG_TAGRTM_PKT:
		ret = rxr_pkt_init_long_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_READ_MSGRTM_PKT:
		ret = rxr_pkt_init_read_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_READ_TAGRTM_PKT:
		ret = rxr_pkt_init_read_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_EAGER_RTW_PKT:
		ret = rxr_pkt_init_eager_rtw(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONG_RTW_PKT:
		ret = rxr_pkt_init_long_rtw(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_READ_RTW_PKT:
		ret = rxr_pkt_init_read_rtw(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_SHORT_RTR_PKT:
		ret = rxr_pkt_init_short_rtr(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONG_RTR_PKT:
		ret = rxr_pkt_init_long_rtr(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_WRITE_RTA_PKT:
		ret = rxr_pkt_init_write_rta(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_FETCH_RTA_PKT:
		ret = rxr_pkt_init_fetch_rta(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_COMPARE_RTA_PKT:
		ret = rxr_pkt_init_compare_rta(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_EAGER_MSGRTM_PKT:
		ret = rxr_pkt_init_dc_eager_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_EAGER_TAGRTM_PKT:
		ret = rxr_pkt_init_dc_eager_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_MEDIUM_MSGRTM_PKT:
		ret = rxr_pkt_init_dc_medium_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_MEDIUM_TAGRTM_PKT:
		ret = rxr_pkt_init_dc_medium_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_LONG_MSGRTM_PKT:
		ret = rxr_pkt_init_dc_long_msgrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_LONG_TAGRTM_PKT:
		ret = rxr_pkt_init_dc_long_tagrtm(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_EAGER_RTW_PKT:
		ret = rxr_pkt_init_dc_eager_rtw(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_LONG_RTW_PKT:
		ret = rxr_pkt_init_dc_long_rtw(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_WRITE_RTA_PKT:
		ret = rxr_pkt_init_dc_write_rta(rxr_ep, (struct rxr_tx_entry *)x_entry, pkt_entry);
		break;
	default:
		ret = -FI_EINVAL;
		assert(0 && "unknown pkt type to init");
		break;
	}

	return ret;
}

/*
 *   rxr_pkt_handle_ctrl_sent() uses handle_sent() functions declared in rxr_pkt_type.h
 */
static
void rxr_pkt_handle_ctrl_sent(struct rxr_ep *rxr_ep, struct rxr_pkt_entry *pkt_entry)
{
	int ctrl_type = rxr_get_base_hdr(pkt_entry->pkt)->type;

	switch (ctrl_type) {
	case RXR_READRSP_PKT:
		rxr_pkt_handle_readrsp_sent(rxr_ep, pkt_entry);
		break;
	case RXR_CTS_PKT:
		rxr_pkt_handle_cts_sent(rxr_ep, pkt_entry);
		break;
	case RXR_EOR_PKT:
		rxr_pkt_handle_eor_sent(rxr_ep, pkt_entry);
		break;
	case RXR_ATOMRSP_PKT:
		rxr_pkt_handle_atomrsp_sent(rxr_ep, pkt_entry);
		break;
	case RXR_RECEIPT_PKT:
		rxr_pkt_handle_receipt_sent(rxr_ep, pkt_entry);
		break;
	case RXR_EAGER_MSGRTM_PKT:
	case RXR_EAGER_TAGRTM_PKT:
		rxr_pkt_handle_eager_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_MEDIUM_MSGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
	case RXR_DC_MEDIUM_MSGRTM_PKT:
	case RXR_DC_MEDIUM_TAGRTM_PKT:
		rxr_pkt_handle_medium_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_LONG_MSGRTM_PKT:
	case RXR_DC_LONG_MSGRTM_PKT:
	case RXR_LONG_TAGRTM_PKT:
	case RXR_DC_LONG_TAGRTM_PKT:
		rxr_pkt_handle_long_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_READ_MSGRTM_PKT:
	case RXR_READ_TAGRTM_PKT:
		rxr_pkt_handle_read_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_EAGER_RTW_PKT:
		rxr_pkt_handle_eager_rtw_sent(rxr_ep, pkt_entry);
		break;
	case RXR_LONG_RTW_PKT:
	case RXR_DC_LONG_RTW_PKT:
		rxr_pkt_handle_long_rtw_sent(rxr_ep, pkt_entry);
		break;
	case RXR_READ_RTW_PKT:
		rxr_pkt_handle_read_rtw_sent(rxr_ep, pkt_entry);
		break;
	case RXR_SHORT_RTR_PKT:
	case RXR_LONG_RTR_PKT:
		/* nothing can be done when RTR packets are sent */
		break;
	case RXR_WRITE_RTA_PKT:
	case RXR_DC_WRITE_RTA_PKT:
	case RXR_FETCH_RTA_PKT:
	case RXR_COMPARE_RTA_PKT:
		rxr_pkt_handle_rta_sent(rxr_ep, pkt_entry);
		break;
	case RXR_DC_EAGER_MSGRTM_PKT:
	case RXR_DC_EAGER_TAGRTM_PKT:
	case RXR_DC_EAGER_RTW_PKT:
		break;
	default:
		assert(0 && "Unknown packet type to handle sent");
		break;
	}
}

ssize_t rxr_pkt_post_ctrl_once(struct rxr_ep *rxr_ep, int entry_type, void *x_entry,
			       int ctrl_type, bool inject)
{
	struct rxr_pkt_entry *pkt_entry;
	struct rxr_tx_entry *tx_entry;
	struct rxr_rx_entry *rx_entry;
	struct rdm_peer *peer;
	ssize_t err;
	fi_addr_t addr;

	if (entry_type == RXR_TX_ENTRY) {
		tx_entry = (struct rxr_tx_entry *)x_entry;
		addr = tx_entry->addr;
	} else {
		rx_entry = (struct rxr_rx_entry *)x_entry;
		addr = rx_entry->addr;
	}

	peer = rxr_ep_get_peer(rxr_ep, addr);
	assert(peer);
	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->shm_tx_pkt_pool, RXR_PKT_FROM_SHM_TX_POOL);
	} else {
		pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->efa_tx_pkt_pool, RXR_PKT_FROM_EFA_TX_POOL);
	}

	if (!pkt_entry)
		return -FI_EAGAIN;

	/*
	 * The allocated pkt_entry->send will be released when releasing the
	 * pkt_entry, because pkt_entry could be queued and re-send later
	 */
	pkt_entry->send = ofi_buf_alloc(rxr_ep->pkt_sendv_pool);
	if (!pkt_entry->send) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unable to allocate rxr_pkt_sendv from pkt_sendv_pool\n");
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);
		return -FI_EAGAIN;
	}
	pkt_entry->send->iov_count = 0;

	/*
	 * rxr_pkt_init_ctrl will set pkt_entry->send if it want to use multi iov
	 */
	err = rxr_pkt_init_ctrl(rxr_ep, entry_type, x_entry, ctrl_type, pkt_entry);
	if (OFI_UNLIKELY(err)) {
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);
		return err;
	}

	/* If the send (or inject) succeeded, the function rxr_pkt_entry_send
	 * (or rxr_pkt_entry_inject) will increase the counter in rxr_ep that
	 * tracks number of outstanding TX ops.
	 */
	if (inject)
		err = rxr_pkt_entry_inject(rxr_ep, pkt_entry, addr);
	else
		err = rxr_pkt_entry_send(rxr_ep, pkt_entry, 0);

	if (OFI_UNLIKELY(err)) {
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);
		return err;
	}

	peer->flags |= RXR_PEER_REQ_SENT;
	rxr_pkt_handle_ctrl_sent(rxr_ep, pkt_entry);

	/* If injection succeeded, packet should be considered as sent completed.
	 * therefore call rxr_pkt_handle_send_completion().
	 * rxr_pkt_handle_send_completion() will release pkt_entry and decrease
	 * the counter in rxr_ep that tracks number of outstanding TX ops.
	 */
	if (inject)
		rxr_pkt_handle_send_completion(rxr_ep, pkt_entry);

	return 0;
}

ssize_t rxr_pkt_post_ctrl(struct rxr_ep *ep, int entry_type, void *x_entry,
			  int ctrl_type, bool inject)
{
	ssize_t err;
	struct rxr_tx_entry *tx_entry;

	if (ctrl_type == RXR_MEDIUM_TAGRTM_PKT ||
	    ctrl_type == RXR_MEDIUM_MSGRTM_PKT ||
	    ctrl_type == RXR_DC_MEDIUM_MSGRTM_PKT ||
	    ctrl_type == RXR_DC_MEDIUM_TAGRTM_PKT) {
		assert(entry_type == RXR_TX_ENTRY);
		assert(!inject);

		tx_entry = (struct rxr_tx_entry *)x_entry;
		while (tx_entry->bytes_sent < tx_entry->total_len) {
			err = rxr_pkt_post_ctrl_once(ep, RXR_TX_ENTRY, x_entry, ctrl_type, 0);
			if (OFI_UNLIKELY(err))
				return err;
		}

		return 0;
	}

	return rxr_pkt_post_ctrl_once(ep, entry_type, x_entry, ctrl_type, inject);
}

ssize_t rxr_pkt_post_ctrl_or_queue(struct rxr_ep *ep, int entry_type, void *x_entry, int ctrl_type, bool inject)
{
	ssize_t err;
	struct rxr_tx_entry *tx_entry;
	struct rxr_rx_entry *rx_entry;

	err = rxr_pkt_post_ctrl(ep, entry_type, x_entry, ctrl_type, inject);
	if (err == -FI_EAGAIN) {
		if (entry_type == RXR_TX_ENTRY) {
			tx_entry = (struct rxr_tx_entry *)x_entry;
			assert(!(tx_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR));
			tx_entry->state = RXR_TX_QUEUED_CTRL;
			tx_entry->queued_ctrl.type = ctrl_type;
			tx_entry->queued_ctrl.inject = inject;
			dlist_insert_tail(&tx_entry->queued_ctrl_entry,
					  &ep->tx_entry_queued_ctrl_list);
		} else {
			assert(entry_type == RXR_RX_ENTRY);
			rx_entry = (struct rxr_rx_entry *)x_entry;
			assert(rx_entry->state != RXR_RX_QUEUED_CTRL);
			rx_entry->state = RXR_RX_QUEUED_CTRL;
			rx_entry->queued_ctrl.type = ctrl_type;
			rx_entry->queued_ctrl.inject = inject;
			dlist_insert_tail(&rx_entry->queued_ctrl_entry,
					  &ep->rx_entry_queued_ctrl_list);
		}

		err = 0;
	}

	return err;
}

/*
 * This function is used for any extra feature that does not have an alternative.
 *
 * This function will send a eager rtw packet to trigger handshake.
 *
 * We do not send eager rtm packets here because the receiver might require
 * ordering and an extra eager rtm will interrupt the reorder
 * process.
 *
 * ep: The endpoint on which the packet for triggering handshake will be sent.
 * peer: The peer from which the sender receives handshake.
 * addr: The address of the peer.
 *
 * This function will return 0 if sender successfully receives / have already
 * received the handshake from the peer
 *
 * This function will return FI_EAGAIN if it fails to allocate or send the trigger packet.
 * It will return FI_ETIMEDOUT if it fails to receive
 * handshake packet within a certain period of time.
 */

ssize_t rxr_pkt_wait_handshake(struct rxr_ep *ep, fi_addr_t addr, struct rdm_peer *peer)
{
	ssize_t ret;

	uint64_t current, endwait;

	ret = rxr_pkt_trigger_handshake(ep, addr, peer);
	if (OFI_UNLIKELY(ret))
		return ret;

	current = ofi_gettime_us();
	endwait = current + RXR_HANDSHAKE_WAIT_TIMEOUT;

	while (current < endwait &&
	       !(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED)) {
		rxr_ep_progress_internal(ep);
		current = ofi_gettime_us();
	}

	if (!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"did not get handshake back in %f second(s). returning -FI_EAGAIN!\n",
			RXR_HANDSHAKE_WAIT_TIMEOUT * 1e-6);
		return -FI_EAGAIN;
	}

	return 0;
}

/*
 * This function is used for any extra feature that does not have an
 * alternative.
 *
 * This function will send a eager rtw packet to trigger handshake.
 *
 * We do not send eager rtm packets here because the receiver might require
 * ordering and an extra eager rtm will interrupt the reorder
 * process.
 *
 * ep: The endpoint on which the packet for triggering handshake will be sent.
 * peer: The peer from which the sender receives handshake.
 * addr: The address of the peer.
 *
 * This function will return 0 if the eager rtw packet is successfully sent.
 */
ssize_t rxr_pkt_trigger_handshake(struct rxr_ep *ep,
				  fi_addr_t addr, struct rdm_peer *peer)
{
	struct rxr_tx_entry *tx_entry;
	ssize_t err;

	if ((peer->flags & RXR_PEER_HANDSHAKE_RECEIVED) ||
	    (peer->flags & RXR_PEER_REQ_SENT))
		return 0;

	tx_entry = ofi_buf_alloc(ep->tx_entry_pool);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return -FI_EAGAIN;
	}

	tx_entry->total_len = 0;
	tx_entry->addr = addr;
	tx_entry->peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(tx_entry->peer);
	dlist_insert_tail(&tx_entry->peer_entry, &tx_entry->peer->tx_entry_list);
	tx_entry->msg_id = -1;
	tx_entry->cq_entry.flags = FI_RMA | FI_WRITE;
	tx_entry->cq_entry.buf = NULL;
	dlist_init(&tx_entry->queued_pkts);

	tx_entry->type = RXR_TX_ENTRY;
	tx_entry->op = ofi_op_write;
	tx_entry->state = RXR_TX_REQ;

	tx_entry->send_flags = 0;
	tx_entry->bytes_acked = 0;
	tx_entry->bytes_sent = 0;
	tx_entry->window = 0;
	tx_entry->rma_iov_count = 0;
	tx_entry->iov_count = 0;
	tx_entry->iov_index = 0;
	tx_entry->iov_mr_start = 0;
	tx_entry->iov_offset = 0;
	tx_entry->fi_flags = RXR_NO_COMPLETION | RXR_NO_COUNTER;
	tx_entry->rxr_flags = 0;

	dlist_insert_tail(&tx_entry->ep_entry, &ep->tx_entry_list);

	err = rxr_pkt_post_ctrl(ep, RXR_TX_ENTRY, tx_entry, RXR_EAGER_RTW_PKT, 0);

	if (OFI_UNLIKELY(err))
		return err;

	return 0;
}

/* return the data size in a packet entry */
size_t rxr_pkt_data_size(struct rxr_pkt_entry *pkt_entry)
{
	int pkt_type;

	assert(pkt_entry);
	pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;

	if (pkt_type == RXR_DATA_PKT)
		return pkt_entry->pkt_size - sizeof(struct rxr_data_hdr);

	if (pkt_type == RXR_READRSP_PKT)
		return pkt_entry->pkt_size - sizeof(struct rxr_readrsp_hdr);

	if (pkt_type >= RXR_REQ_PKT_BEGIN) {
		assert(pkt_type == RXR_EAGER_MSGRTM_PKT || pkt_type == RXR_EAGER_TAGRTM_PKT ||
		       pkt_type == RXR_MEDIUM_MSGRTM_PKT || pkt_type == RXR_MEDIUM_TAGRTM_PKT ||
		       pkt_type == RXR_LONG_MSGRTM_PKT || pkt_type == RXR_LONG_TAGRTM_PKT ||
		       pkt_type == RXR_EAGER_RTW_PKT ||
		       pkt_type == RXR_LONG_RTW_PKT ||
		       pkt_type == RXR_DC_EAGER_MSGRTM_PKT ||
		       pkt_type == RXR_DC_EAGER_TAGRTM_PKT ||
		       pkt_type == RXR_DC_MEDIUM_MSGRTM_PKT ||
		       pkt_type == RXR_DC_MEDIUM_TAGRTM_PKT ||
		       pkt_type == RXR_DC_LONG_MSGRTM_PKT ||
		       pkt_type == RXR_DC_LONG_TAGRTM_PKT ||
		       pkt_type == RXR_DC_EAGER_RTW_PKT ||
		       pkt_type == RXR_DC_LONG_RTW_PKT);

		return pkt_entry->pkt_size - rxr_pkt_req_hdr_size(pkt_entry);
	}

	/* other packet type does not contain data, thus return 0
	 */
	return 0;
}

/*
 * rxr_pkt_copy_to_rx() copy data to receiving buffer then
 * update counter in rx_entry.
 *
 * If receiving buffer is on GPU memory, it will post a
 * read request, otherwise it will copy data.
 *
 * If all data has been copied to receiving buffer,
 * it will write rx completion and release rx_entry.
 *
 * Return value and states:
 *
 *    On success, return 0 and release pkt_entry
 *    On failure, return error code
 */
ssize_t rxr_pkt_copy_to_rx(struct rxr_ep *ep,
			   struct rxr_rx_entry *rx_entry,
			   size_t data_offset,
			   struct rxr_pkt_entry *pkt_entry,
			   char *data, size_t data_size)
{
	ssize_t err, bytes_copied;

	pkt_entry->x_entry = rx_entry;

	if (data_size > 0 && efa_ep_is_cuda_mr(rx_entry->desc[0])) {
		err = rxr_read_post_local_read_or_queue(ep, rx_entry, data_offset,
							pkt_entry, data, data_size);
		if (err)
			FI_WARN(&rxr_prov, FI_LOG_CQ, "cannot post read to copy data\n");

		return err;
	}

	if (OFI_LIKELY(!(rx_entry->rxr_flags & RXR_RECV_CANCEL)) &&
	    rx_entry->cq_entry.len > data_offset && data_size > 0) {
		bytes_copied = ofi_copy_to_iov(rx_entry->iov,
					       rx_entry->iov_count,
					       data_offset + ep->msg_prefix_size,
					       data,
					       data_size);
		if (bytes_copied != MIN(data_size, rx_entry->cq_entry.len - data_offset)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ, "wrong size! bytes_copied: %ld\n",
				bytes_copied);
			return -FI_EINVAL;
		}
	}

	rxr_pkt_handle_data_copied(ep, pkt_entry, data_size);
	return 0;
}

void rxr_pkt_handle_data_copied(struct rxr_ep *ep,
				struct rxr_pkt_entry *pkt_entry,
				size_t data_size)
{
	struct rxr_rx_entry *rx_entry;
	ssize_t ret;

	rx_entry = pkt_entry->x_entry;
	assert(rx_entry);
	rx_entry->bytes_copied += data_size;

	if (rx_entry->total_len == rx_entry->bytes_copied) {
		if (rx_entry->rxr_flags & RXR_DELIVERY_COMPLETE_REQUESTED) {
			ret = rxr_pkt_post_ctrl_or_queue(ep,
							 RXR_RX_ENTRY,
							 rx_entry,
							 RXR_RECEIPT_PKT, 0);
			if (OFI_UNLIKELY(ret)) {
				FI_WARN(&rxr_prov,
					FI_LOG_CQ,
					"Posting of receipt packet failed! err=%s\n",
					fi_strerror(ret));
				efa_eq_write_error(&ep->util_ep,
						   FI_EIO,
						   ret);
				rxr_release_rx_entry(ep,
						     rx_entry);
				return;
			}
			rxr_cq_handle_rx_completion(ep, pkt_entry, rx_entry);
			rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);
			/* rx_entry will be released
			 * when sender receives the
			 * receipt packet.
			 */
			return;
		}
		rxr_cq_handle_rx_completion(ep, pkt_entry, rx_entry);
		rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);
		rxr_release_rx_entry(ep, rx_entry);
	} else {
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	}
}

/**
 * @brief handle the a packet that encountered error completion while sending
 *
 * Depend on the packet type and error type, the error are handled differently.
 *
 * If the packet is associated with an user initialized TX operation:
 * (TX means send,read or write; such packets include all REQ packets and DATA):
 *
 *    If the error is Receiver Not Ready (RNR). there are two cases:
 *
 *         If user wants to manager RNR by itself (FI_RM_DISABLED),
 *         an error CQ entry will be written.
 *
 *         Otherwise, the packet will be queued and resnt by progress engine.
 *
 *    For other type of error, an error CQ entry is written.
 *
 * If the packet is associated with an user initialized recv operiaton,
 * (such packets include EOR, CTS):
 *
 *      If the error is RNR, the packet is queued and resent by progress
 *      engine. No CQ entry is written.
 *
 *      For other types of error, an error CQ entry is written.
 *
 * If the packet is not associated with an user operation (such packet include
 * HANDSHAKE):
 *
 *      If the error is RNR, the packet is queued and resent by progress engine.
 *
 *      For othre types of error, an error EQ entry is written.
 *
 * @param[in]	ep		endpoint
 * @param[in]	pkt_entry	pkt entry
 * @param[in]	err		libfabric error code
 * @param[in]	prov_errno	provider specific error code
 */
void rxr_pkt_handle_send_error(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry, int err, int prov_errno)
{
	struct rdm_peer *peer;
	struct rxr_tx_entry *tx_entry;
	struct rxr_rx_entry *rx_entry;

	assert(pkt_entry->alloc_type == RXR_PKT_FROM_EFA_TX_POOL ||
	       pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL);

	rxr_ep_record_tx_op_completed(ep, pkt_entry);

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	if (!peer) {
		/*
		 * If peer is NULL, it means the peer has been removed from AV.
		 * In this case, ignore this error completion.
		 */
		FI_WARN(&rxr_prov, FI_LOG_CQ, "ignoring send error completion of a packet to a removed peer.\n");
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		return;
	}

	if (!pkt_entry->x_entry) {
		/* only handshake packet is not associated with any TX/RX operation */
		assert(rxr_get_base_hdr(pkt_entry->pkt)->type == RXR_HANDSHAKE_PKT);
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		if (prov_errno == IBV_WC_RNR_RETRY_EXC_ERR) {
			/*
			 * handshake should always be queued for RNR
			 */
			assert(!(peer->flags & RXR_PEER_HANDSHAKE_QUEUED));
			peer->flags |= RXR_PEER_HANDSHAKE_QUEUED;
			dlist_insert_tail(&peer->handshake_queued_entry,
					  &ep->handshake_queued_peer_list);
		} else if (prov_errno != IBV_WC_REM_INV_RD_REQ_ERR) {
			/* If prov_errno is IBV_WC_REM_INV_RD_REQ_ERR, the peer has been destroyed.
			 * Which is normal, as peer does not always need a handshake packet perform
			 * its duty. (For example, if a peer just want to sent 1 message to the ep, it
			 * does not need handshake.)
			 * In this case, it is safe to ignore this error completion.
			 * In all other cases, we write an eq entry because there is no application
			 * operation associated with handshake.
			 */
			efa_eq_write_error(&ep->util_ep, err, prov_errno);
		}
		return;
	}

	if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_TX_ENTRY) {
		tx_entry = pkt_entry->x_entry;
		if (prov_errno == IBV_WC_RNR_RETRY_EXC_ERR &&
		    ep->handle_resource_management == FI_RM_ENABLED) {
			/*
			 * This packet is assoiciated with a send operation,
			 * (such packets include all REQ, DATA)
			 * thus shoud be queued for RNR only if
			 * application want EFA to manager resource.
			 */
			rxr_cq_queue_rnr_pkt(ep, &tx_entry->queued_pkts, pkt_entry);
			if (!(tx_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR)) {
				tx_entry->rxr_flags |= RXR_TX_ENTRY_QUEUED_RNR;
				dlist_insert_tail(&tx_entry->queued_rnr_entry,
						  &ep->tx_entry_queued_rnr_list);
			}
		} else {
			rxr_cq_write_tx_error(ep, pkt_entry->x_entry, err, prov_errno);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		}

		return;
	}

	if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_RX_ENTRY) {
		rx_entry = pkt_entry->x_entry;
		if (prov_errno == IBV_WC_RNR_RETRY_EXC_ERR) {
			/*
			 * This packet is associated with a recv operation,
			 * (such packets include CTS and EOR)
			 * thus should always be queued for RNR.
			 * This is regardless value of ep->handle_resource_management,
			 * because resource management is only applied to send operation.
			 */
			rxr_cq_queue_rnr_pkt(ep, &rx_entry->queued_pkts, pkt_entry);
			/*
			 * rx_entry send one ctrl packet at a time, so if we
			 * received RNR for the packet, the rx_entry must not
			 * be in ep's rx_queued_entry_rnr_list, thus cannot
			 * have the QUEUED_RNR flag
			 */
			assert(!(rx_entry->rxr_flags & RXR_RX_ENTRY_QUEUED_RNR));
			rx_entry->rxr_flags |= RXR_RX_ENTRY_QUEUED_RNR;
			dlist_insert_tail(&rx_entry->queued_rnr_entry,
					  &ep->rx_entry_queued_rnr_list);

		} else {
			rxr_cq_write_rx_error(ep, pkt_entry->x_entry, err, prov_errno);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		}

		return;
	}

	if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_READ_ENTRY) {
		/* read will not encounter RNR */
		assert(prov_errno != IBV_WC_RNR_RETRY_EXC_ERR);
		rxr_read_write_error(ep, pkt_entry->x_entry, err, prov_errno);
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		return;
	}

	FI_WARN(&rxr_prov, FI_LOG_CQ,
		"%s unknown x_entry type %d\n",
		__func__, RXR_GET_X_ENTRY_TYPE(pkt_entry));
	assert(0 && "unknown x_entry state");
	efa_eq_write_error(&ep->util_ep, err, prov_errno);
	rxr_pkt_entry_release_tx(ep, pkt_entry);
}

void rxr_pkt_handle_send_completion(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	/*
	 * For a send completion, pkt_entry->addr can be FI_ADDR_NOTAVAIL in 3 situations:
	 * 1. the pkt_entry is used for a local read operation
	 * 2. a new peer with same gid+qpn was inserted to av, thus the peer was removed from AV.
	 * 3. application removed the peer's address from av.
	 * In 1, we should proceed. For 2 and 3, the send completion should be ignored.
	 */
	if (pkt_entry->addr == FI_ADDR_NOTAVAIL &&
	    !(pkt_entry->flags & RXR_PKT_ENTRY_LOCAL_READ)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "ignoring send completion of a packet to a removed peer.\n");
		rxr_ep_record_tx_op_completed(ep, pkt_entry);
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		return;
	}

	switch (rxr_get_base_hdr(pkt_entry->pkt)->type) {
	case RXR_HANDSHAKE_PKT:
		break;
	case RXR_CTS_PKT:
		break;
	case RXR_DATA_PKT:
		rxr_pkt_handle_data_send_completion(ep, pkt_entry);
		break;
	case RXR_READRSP_PKT:
		rxr_pkt_handle_readrsp_send_completion(ep, pkt_entry);
		break;
	case RXR_EOR_PKT:
		rxr_pkt_handle_eor_send_completion(ep, pkt_entry);
		break;
	case RXR_RMA_CONTEXT_PKT:
		rxr_pkt_handle_rma_completion(ep, pkt_entry);
		return;
	case RXR_ATOMRSP_PKT:
		rxr_pkt_handle_atomrsp_send_completion(ep, pkt_entry);
		break;
	case RXR_RECEIPT_PKT:
		rxr_pkt_handle_receipt_send_completion(ep, pkt_entry);
		break;
	case RXR_EAGER_MSGRTM_PKT:
	case RXR_EAGER_TAGRTM_PKT:
		rxr_pkt_handle_eager_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_MEDIUM_MSGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
		rxr_pkt_handle_medium_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_LONG_MSGRTM_PKT:
	case RXR_LONG_TAGRTM_PKT:
		rxr_pkt_handle_long_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_READ_MSGRTM_PKT:
	case RXR_READ_TAGRTM_PKT:
		rxr_pkt_handle_read_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_EAGER_RTW_PKT:
		rxr_pkt_handle_eager_rtw_send_completion(ep, pkt_entry);
		break;
	case RXR_LONG_RTW_PKT:
		rxr_pkt_handle_long_rtw_send_completion(ep, pkt_entry);
		break;
	case RXR_READ_RTW_PKT:
		rxr_pkt_handle_read_rtw_send_completion(ep, pkt_entry);
		break;
	case RXR_SHORT_RTR_PKT:
	case RXR_LONG_RTR_PKT:
		rxr_pkt_handle_rtr_send_completion(ep, pkt_entry);
		break;
	case RXR_WRITE_RTA_PKT:
		rxr_pkt_handle_write_rta_send_completion(ep, pkt_entry);
		break;
	case RXR_FETCH_RTA_PKT:
		/* no action to be taken here */
		break;
	case RXR_COMPARE_RTA_PKT:
		/* no action to be taken here */
		break;
	case RXR_DC_EAGER_MSGRTM_PKT:
	case RXR_DC_EAGER_TAGRTM_PKT:
	case RXR_DC_MEDIUM_MSGRTM_PKT:
	case RXR_DC_MEDIUM_TAGRTM_PKT:
	case RXR_DC_EAGER_RTW_PKT:
	case RXR_DC_WRITE_RTA_PKT:
		/* no action to be taken here */
		/* For non-dc version of the packet types,
		 * this is the place to write tx completion.
		 * However, for dc tx completion will always be
		 * written upon receving the receipt packet
		 * if not using long message protocols.
		 * Moreoever, because receipt can arrive
		 * before send completion, we cannot take
		 * any action on tx_entry here.
		 */
		break;
	case RXR_DC_LONG_MSGRTM_PKT:
	case RXR_DC_LONG_TAGRTM_PKT:
		rxr_pkt_handle_dc_long_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_DC_LONG_RTW_PKT:
		rxr_pkt_handle_dc_long_rtw_send_completion(ep, pkt_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"invalid control pkt type %d\n",
			rxr_get_base_hdr(pkt_entry->pkt)->type);
		assert(0 && "invalid control pkt type");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EIO);
		return;
	}

	rxr_ep_record_tx_op_completed(ep, pkt_entry);
	rxr_pkt_entry_release_tx(ep, pkt_entry);
}

/**
 * @brief handle the a packet that encountered error completion while receiving
 *
 * This function will write error cq or eq entry, then release the packet entry.
 *
 * @param[in]	ep		endpoint
 * @param[in]	pkt_entry	pkt entry
 * @param[in]	err		libfabric error code
 * @param[in]	prov_errno	provider specific error code
 */
void rxr_pkt_handle_recv_error(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry, int err, int prov_errno)
{
	if (!pkt_entry->x_entry) {
		efa_eq_write_error(&ep->util_ep, err, prov_errno);
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		return;
	}

	if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_TX_ENTRY) {
		rxr_cq_write_tx_error(ep, pkt_entry->x_entry, err, prov_errno);
	} else if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_RX_ENTRY) {
		rxr_cq_write_rx_error(ep, pkt_entry->x_entry, err, prov_errno);
	} else {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
		"%s unknown x_entry type %d\n",
			__func__, RXR_GET_X_ENTRY_TYPE(pkt_entry));
		assert(0 && "unknown x_entry state");
		efa_eq_write_error(&ep->util_ep, err, prov_errno);
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

static
fi_addr_t rxr_pkt_insert_addr(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry, void *raw_addr)
{
	int i, ret;
	fi_addr_t rdm_addr;
	struct efa_ep *efa_ep;
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	if (base_hdr->version < RXR_BASE_PROTOCOL_VERSION) {
		char host_gid[ep->core_addrlen * 3];
		int length = 0;

		for (i = 0; i < ep->core_addrlen; i++)
			length += sprintf(&host_gid[length], "%02x ",
					  ep->core_addr[i]);
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Host %s received a packet with invalid protocol version %d.\n"
			"This host can only support protocol version %d and above.\n",
			host_gid, base_hdr->version, RXR_BASE_PROTOCOL_VERSION);
		efa_eq_write_error(&ep->util_ep, FI_EIO, -FI_EINVAL);
		fprintf(stderr, "Host %s received a packet with invalid protocol version %d.\n"
			"This host can only support protocol version %d and above. %s:%d\n",
			host_gid, base_hdr->version, RXR_BASE_PROTOCOL_VERSION, __FILE__, __LINE__);
		abort();
	}

	assert(base_hdr->type >= RXR_REQ_PKT_BEGIN);

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	ret = efa_av_insert_one(efa_ep->av, (struct efa_ep_addr *)raw_addr,
	                        &rdm_addr, 0, NULL);
	if (OFI_UNLIKELY(ret != 0)) {
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, ret);
		return -1;
	}

	return rdm_addr;
}

/**
 * @brief process a received packet
 *
 * @param[in]	ep		endpoint
 * @param[in]	pkt_entry	received packet entry
 */
void rxr_pkt_proc_received(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	switch (base_hdr->type) {
	case RXR_RETIRED_RTS_PKT:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Received a RTS packet, which has been retired since protocol version 4\n");
		assert(0 && "deprecated RTS pakcet received");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EIO);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	case RXR_RETIRED_CONNACK_PKT:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Received a CONNACK packet, which has been retired since protocol version 4\n");
		assert(0 && "deprecated CONNACK pakcet received");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EIO);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	case RXR_EOR_PKT:
		rxr_pkt_handle_eor_recv(ep, pkt_entry);
		return;
	case RXR_HANDSHAKE_PKT:
		rxr_pkt_handle_handshake_recv(ep, pkt_entry);
		return;
	case RXR_CTS_PKT:
		rxr_pkt_handle_cts_recv(ep, pkt_entry);
		return;
	case RXR_DATA_PKT:
		rxr_pkt_handle_data_recv(ep, pkt_entry);
		return;
	case RXR_READRSP_PKT:
		rxr_pkt_handle_readrsp_recv(ep, pkt_entry);
		return;
	case RXR_ATOMRSP_PKT:
		rxr_pkt_handle_atomrsp_recv(ep, pkt_entry);
		return;
	case RXR_RECEIPT_PKT:
		rxr_pkt_handle_receipt_recv(ep, pkt_entry);
		return;
	case RXR_EAGER_MSGRTM_PKT:
	case RXR_EAGER_TAGRTM_PKT:
	case RXR_DC_EAGER_MSGRTM_PKT:
	case RXR_DC_EAGER_TAGRTM_PKT:
	case RXR_MEDIUM_MSGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
	case RXR_DC_MEDIUM_MSGRTM_PKT:
	case RXR_DC_MEDIUM_TAGRTM_PKT:
	case RXR_LONG_MSGRTM_PKT:
	case RXR_LONG_TAGRTM_PKT:
	case RXR_DC_LONG_MSGRTM_PKT:
	case RXR_DC_LONG_TAGRTM_PKT:
	case RXR_READ_MSGRTM_PKT:
	case RXR_READ_TAGRTM_PKT:
	case RXR_WRITE_RTA_PKT:
	case RXR_DC_WRITE_RTA_PKT:
	case RXR_FETCH_RTA_PKT:
	case RXR_COMPARE_RTA_PKT:
		rxr_pkt_handle_rtm_rta_recv(ep, pkt_entry);
		return;
	case RXR_EAGER_RTW_PKT:
		rxr_pkt_handle_eager_rtw_recv(ep, pkt_entry);
		return;
	case RXR_LONG_RTW_PKT:
	case RXR_DC_LONG_RTW_PKT:
		rxr_pkt_handle_long_rtw_recv(ep, pkt_entry);
		return;
	case RXR_READ_RTW_PKT:
		rxr_pkt_handle_read_rtw_recv(ep, pkt_entry);
		return;
	case RXR_SHORT_RTR_PKT:
	case RXR_LONG_RTR_PKT:
		rxr_pkt_handle_rtr_recv(ep, pkt_entry);
		return;
	case RXR_DC_EAGER_RTW_PKT:
		rxr_pkt_handle_dc_eager_rtw_recv(ep, pkt_entry);
		return;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"invalid control pkt type %d\n",
			rxr_get_base_hdr(pkt_entry->pkt)->type);
		assert(0 && "invalid control pkt type");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EIO);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}
}

void rxr_pkt_handle_recv_completion(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry)
{
	int pkt_type;
	struct rdm_peer *peer;
	struct rxr_base_hdr *base_hdr;
	struct rxr_rx_entry *zcpy_rx_entry = NULL;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	pkt_type = base_hdr->type;
	if (pkt_type >= RXR_EXTRA_REQ_PKT_END) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Peer %d is requesting feature %d, which this EP does not support.\n",
			(int)pkt_entry->addr, base_hdr->type);

		assert(0 && "invalid REQ packe type");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EIO);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	if (pkt_type >= RXR_REQ_PKT_BEGIN && rxr_pkt_req_raw_addr(pkt_entry)) {
		/*
		 * A REQ packet with raw address in its header could always
		 * be the 1st packet we receive from a peer, even if we already
		 * have the address in AV.
		 *
		 * This is because the peer might be a newly created one,
		 * with the same GID+QPN as an old peer (though a different Q-Key),
		 * therefore lower provider thinks it is the older peer.
		 *
		 * Therefore, we alwyas need to call rxr_pkt_insert_addr() for
		 * such a packet. rxr_pkt_insert_addr() will insert the address
		 * to AV if it is indeed new.
		 */
		void *raw_addr;

		raw_addr = rxr_pkt_req_raw_addr(pkt_entry);
		assert(raw_addr);
		pkt_entry->addr = rxr_pkt_insert_addr(ep, pkt_entry, raw_addr);
	} else if (pkt_entry->addr == FI_ADDR_NOTAVAIL) {
		/*
		 * Receiving a non-REQ packet or a REQ packet without raw address means
		 * we had prior communication with the peer. For such a packet,
		 * the only possiblity for its pkt_entry->addr to be FI_ADDR_NOTAVAIL
		 * is application called fi_av_remove() to remove the address
		 * from AV. In this case, this packet should be ignored.
		 */
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Warning: ignoring a received packet from a removed address\n");
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	assert(pkt_entry->addr != FI_ADDR_NOTAVAIL);

#if ENABLE_DEBUG
	if (!ep->use_zcpy_rx) {
		dlist_remove(&pkt_entry->dbg_entry);
		dlist_insert_tail(&pkt_entry->dbg_entry, &ep->rx_pkt_list);
	}
#ifdef ENABLE_RXR_PKT_DUMP
	rxr_pkt_print("Received", ep, (struct rxr_base_hdr *)pkt_entry->pkt);
#endif
#endif
	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	rxr_pkt_post_handshake_or_queue(ep, peer);

	if (peer->is_local) {
		assert(ep->use_shm);
		ep->posted_bufs_shm--;
	} else {
		ep->posted_bufs_efa--;
	}

	if (pkt_entry->alloc_type == RXR_PKT_FROM_USER_BUFFER) {
		assert(pkt_entry->x_entry);
		zcpy_rx_entry = pkt_entry->x_entry;
	}

	rxr_pkt_proc_received(ep, pkt_entry);

	if (zcpy_rx_entry && pkt_type != RXR_EAGER_MSGRTM_PKT) {
		/* user buffer was not matched with a message,
		 * therefore reposting the buffer */
		rxr_ep_post_user_buf(ep, zcpy_rx_entry, 0);
	}
}

#if ENABLE_DEBUG

/*
 *  Functions used to dump packets
 */

#define RXR_PKT_DUMP_DATA_LEN 64

static
void rxr_pkt_print_handshake(char *prefix,
			     struct rxr_handshake_hdr *handshake_hdr)
{
	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s RxR HANDSHAKE packet - version: %" PRIu8
	       " flags: %x\n", prefix, handshake_hdr->version,
	       handshake_hdr->flags);

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s RxR HANDSHAKE packet, maxproto: %d\n",
	       prefix, handshake_hdr->maxproto);
}

static
void rxr_pkt_print_cts(char *prefix, struct rxr_cts_hdr *cts_hdr)
{
	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s RxR CTS packet - version: %"	PRIu8
	       " flags: %x tx_id: %" PRIu32
	       " rx_id: %"	   PRIu32
	       " window: %"	   PRIu64
	       "\n", prefix, cts_hdr->version, cts_hdr->flags,
	       cts_hdr->tx_id, cts_hdr->rx_id, cts_hdr->window);
}

static
void rxr_pkt_print_data(char *prefix, struct rxr_data_pkt *data_pkt)
{
	char str[RXR_PKT_DUMP_DATA_LEN * 4];
	size_t str_len = RXR_PKT_DUMP_DATA_LEN * 4, l;
	int i;

	str[str_len - 1] = '\0';

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s RxR DATA packet -  version: %" PRIu8
	       " flags: %x rx_id: %" PRIu32
	       " seg_size: %"	     PRIu64
	       " seg_offset: %"	     PRIu64
	       "\n", prefix, data_pkt->hdr.version, data_pkt->hdr.flags,
	       data_pkt->hdr.rx_id, data_pkt->hdr.seg_size,
	       data_pkt->hdr.seg_offset);

	l = snprintf(str, str_len, ("\tdata:    "));
	for (i = 0; i < MIN(data_pkt->hdr.seg_size, RXR_PKT_DUMP_DATA_LEN);
	     i++)
		l += snprintf(str + l, str_len - l, "%02x ",
			      ((uint8_t *)data_pkt->data)[i]);
	FI_DBG(&rxr_prov, FI_LOG_EP_DATA, "%s\n", str);
}

void rxr_pkt_print(char *prefix, struct rxr_ep *ep, struct rxr_base_hdr *hdr)
{
	switch (hdr->type) {
	case RXR_HANDSHAKE_PKT:
		rxr_pkt_print_handshake(prefix, (struct rxr_handshake_hdr *)hdr);
		break;
	case RXR_CTS_PKT:
		rxr_pkt_print_cts(prefix, (struct rxr_cts_hdr *)hdr);
		break;
	case RXR_DATA_PKT:
		rxr_pkt_print_data(prefix, (struct rxr_data_pkt *)hdr);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid ctl pkt type %d\n",
			rxr_get_base_hdr(hdr)->type);
		assert(0);
		return;
	}
}
#endif

