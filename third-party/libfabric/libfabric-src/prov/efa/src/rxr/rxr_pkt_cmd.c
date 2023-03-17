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

#include "efa.h"
#include "rxr.h"
#include "rxr_msg.h"
#include "rxr_cntr.h"
#include "rxr_read.h"
#include "rxr_pkt_cmd.h"
#include "rxr_pkt_type_base.h"

/* Handshake wait timeout in microseconds */
#define RXR_HANDSHAKE_WAIT_TIMEOUT 1000000

/* This file implements 4 actions that can be applied to a packet:
 *          posting,
 *          handling send completion and,
 *          handing recv completion.
 *          dump (for debug only)
 */

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
		ret = rxr_pkt_init_readrsp(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_CTS_PKT:
		ret = rxr_pkt_init_cts(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_EOR_PKT:
		ret = rxr_pkt_init_eor(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_ATOMRSP_PKT:
		ret = rxr_pkt_init_atomrsp(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_RECEIPT_PKT:
		ret = rxr_pkt_init_receipt(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_EAGER_MSGRTM_PKT:
		ret = rxr_pkt_init_eager_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_EAGER_TAGRTM_PKT:
		ret = rxr_pkt_init_eager_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_MEDIUM_MSGRTM_PKT:
		ret = rxr_pkt_init_medium_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_MEDIUM_TAGRTM_PKT:
		ret = rxr_pkt_init_medium_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGCTS_MSGRTM_PKT:
		ret = rxr_pkt_init_longcts_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGCTS_TAGRTM_PKT:
		ret = rxr_pkt_init_longcts_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGREAD_MSGRTM_PKT:
		ret = rxr_pkt_init_longread_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGREAD_TAGRTM_PKT:
		ret = rxr_pkt_init_longread_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_RUNTREAD_MSGRTM_PKT:
		ret = rxr_pkt_init_runtread_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_RUNTREAD_TAGRTM_PKT:
		ret = rxr_pkt_init_runtread_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_EAGER_RTW_PKT:
		ret = rxr_pkt_init_eager_rtw(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGCTS_RTW_PKT:
		ret = rxr_pkt_init_longcts_rtw(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGREAD_RTW_PKT:
		ret = rxr_pkt_init_longread_rtw(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_SHORT_RTR_PKT:
		ret = rxr_pkt_init_short_rtr(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_LONGCTS_RTR_PKT:
		ret = rxr_pkt_init_longcts_rtr(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_WRITE_RTA_PKT:
		ret = rxr_pkt_init_write_rta(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_FETCH_RTA_PKT:
		ret = rxr_pkt_init_fetch_rta(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_COMPARE_RTA_PKT:
		ret = rxr_pkt_init_compare_rta(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_EAGER_MSGRTM_PKT:
		ret = rxr_pkt_init_dc_eager_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_EAGER_TAGRTM_PKT:
		ret = rxr_pkt_init_dc_eager_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_MEDIUM_MSGRTM_PKT:
		ret = rxr_pkt_init_dc_medium_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_MEDIUM_TAGRTM_PKT:
		ret = rxr_pkt_init_dc_medium_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_LONGCTS_MSGRTM_PKT:
		ret = rxr_pkt_init_dc_longcts_msgrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_LONGCTS_TAGRTM_PKT:
		ret = rxr_pkt_init_dc_longcts_tagrtm(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_EAGER_RTW_PKT:
		ret = rxr_pkt_init_dc_eager_rtw(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_LONGCTS_RTW_PKT:
		ret = rxr_pkt_init_dc_longcts_rtw(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DC_WRITE_RTA_PKT:
		ret = rxr_pkt_init_dc_write_rta(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	case RXR_DATA_PKT:
		ret = rxr_pkt_init_data(rxr_ep, (struct rxr_op_entry *)x_entry, pkt_entry);
		break;
	default:
		assert(0 && "unknown pkt type to init");
		ret = -FI_EINVAL;
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
	case RXR_LONGCTS_MSGRTM_PKT:
	case RXR_DC_LONGCTS_MSGRTM_PKT:
	case RXR_LONGCTS_TAGRTM_PKT:
	case RXR_DC_LONGCTS_TAGRTM_PKT:
		rxr_pkt_handle_longcts_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_LONGREAD_MSGRTM_PKT:
	case RXR_LONGREAD_TAGRTM_PKT:
		rxr_pkt_handle_longread_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_RUNTREAD_MSGRTM_PKT:
	case RXR_RUNTREAD_TAGRTM_PKT:
		rxr_pkt_handle_runtread_rtm_sent(rxr_ep, pkt_entry);
		break;
	case RXR_EAGER_RTW_PKT:
		rxr_pkt_handle_eager_rtw_sent(rxr_ep, pkt_entry);
		break;
	case RXR_LONGCTS_RTW_PKT:
	case RXR_DC_LONGCTS_RTW_PKT:
		rxr_pkt_handle_longcts_rtw_sent(rxr_ep, pkt_entry);
		break;
	case RXR_LONGREAD_RTW_PKT:
		rxr_pkt_handle_longread_rtw_sent(rxr_ep, pkt_entry);
		break;
	case RXR_SHORT_RTR_PKT:
	case RXR_LONGCTS_RTR_PKT:
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
	case RXR_DATA_PKT:
		rxr_pkt_handle_data_sent(rxr_ep, pkt_entry);
		break;
	default:
		assert(0 && "Unknown packet type to handle sent");
		break;
	}
}

/**
 * @brief post one packet.
 *
 * The function does the following:
 *  1. construct a packet.
 *  2. send it
 *  3. Upon success, call the packet's sent handler.
 *  4. If inject, call the packet's send completion handler.
 *
 * @param[in]   rxr_ep          endpoint
 * @param[in]   x_entry         pointer to rxr_op_entry. (either a tx_entry or an rx_entry)
 * @param[in]   pkt_type        packet type.
 * @param[in]   inject          send control packet via inject or not.
 * @param[in]   flags           additional flags to apply for fi_sendmsg.
 *                              currently only accepted flags is FI_MORE.
 * @return      On success return 0, otherwise return a negative libfabric error code.
 *              Possible error code include (but not limited to):
 * 		-FI_EAGAIN	temporary out of resource
 */
static inline
ssize_t rxr_pkt_post_one(struct rxr_ep *rxr_ep, struct rxr_op_entry *op_entry,
			 int pkt_type, bool inject, uint64_t flags)
{
	struct rxr_pkt_entry *pkt_entry;
	struct rdm_peer *peer;
	ssize_t err;
	fi_addr_t addr;

	addr = op_entry->addr;
	peer = rxr_ep_get_peer(rxr_ep, addr);
	assert(peer);
	if (peer->is_local && rxr_ep->use_shm_for_tx) {
		pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->shm_tx_pkt_pool, RXR_PKT_FROM_SHM_TX_POOL);
	} else {
		pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->efa_tx_pkt_pool, RXR_PKT_FROM_EFA_TX_POOL);
	}

	if (!pkt_entry)
		return -FI_EAGAIN;

	/*
	 * rxr_pkt_init_ctrl will set pkt_entry->send if it want to use multi iov
	 */
	err = rxr_pkt_init_ctrl(rxr_ep, op_entry->type, op_entry, pkt_type, pkt_entry);
	if (OFI_UNLIKELY(err)) {
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);
		return err;
	}

	/* If the send (or inject) succeeded, the function rxr_pkt_entry_send
	 * (or rxr_pkt_entry_inject) will increase the counter in rxr_ep that
	 * tracks number of outstanding TX ops.
	 */
	if (inject) {
		/*
		 * Currently, the only accepted flags is FI_MORE, which is not
		 * compatible with inject. Add an additional check here to make
		 * sure flags is set by the caller correctly.
		 */
		assert(!flags);
		err = rxr_pkt_entry_inject(rxr_ep, pkt_entry, addr);
	} else
		err = rxr_pkt_entry_send(rxr_ep, pkt_entry, flags);

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

/**
 * @brief post packet(s) according to packet type.
 *
 * Depend on packet type, this function may post one packet or multiple packets.
 * This is because some REQ packet types such as MEDIUM RTM must be sent as a series of packets.
 *
 * @param[in]   rxr_ep          endpoint
 * @param[in]   op_entry        pointer to rxr_op_entry. (either a tx_entry or an rx_entry)
 * @param[in]   pkt_type        packet type.
 * @param[in]   inject          send control packet via inject or not.
 * @return      On success return 0, otherwise return a negative libfabric error code. Possible error codes include:
 * 		-FI_EAGAIN	temporarily  out of resource
 */
ssize_t rxr_pkt_post(struct rxr_ep *ep, struct rxr_op_entry *op_entry, int pkt_type, bool inject, uint64_t flags)
{
	ssize_t err;
	size_t num_req, i;
	uint64_t extra_flags;

	if (rxr_pkt_type_is_mulreq(pkt_type)) {
		assert(!inject);

		if(rxr_pkt_type_is_runt(pkt_type))
			rxr_tx_entry_set_runt_size(ep, op_entry);

		rxr_tx_entry_set_max_req_data_size(ep, op_entry, pkt_type);

		num_req = rxr_tx_entry_num_req(op_entry, pkt_type);

		if (num_req > (ep->efa_max_outstanding_tx_ops - ep->efa_outstanding_tx_ops))
			return -FI_EAGAIN;

		for (i = 0; i < num_req; ++i) {
			extra_flags = (i == num_req - 1) ? 0 : FI_MORE;

			err = rxr_pkt_post_one(ep, op_entry, pkt_type, 0, flags | extra_flags);
			if (OFI_UNLIKELY(err))
				return err;
		}

		assert(op_entry->bytes_sent == rxr_op_entry_mulreq_total_data_size(op_entry, pkt_type));
		return 0;
	}

	return rxr_pkt_post_one(ep, op_entry, pkt_type, inject, flags);
}

/**
 * @brief post packet(s) according to packet type. Queue the post if -FI_EAGAIN is encountered.
 *
 * This function will cal rxr_pkt_post() to post packet(s) according to packet type.
 * If rxr_pkt_post() returned -FI_EAGAIN, this function will put the tx_entry in rxr_ep's
 * queued_ctrl_list. The progress engine will try to post the packet later.
 *
 * This function is called by rxr_pkt_post_req() to post MEDIUM RTM packets, and is
 * called by packet handler to post responsive ctrl packet (such as EOR and CTS).
 *
 * @param[in]   rxr_ep          endpoint
 * @param[in]   x_entry         pointer to rxr_op_entry. (either a tx_entry or an rx_entry)
 * @param[in]   pkt_type        packet type.
 * @param[in]   inject          send control packet via inject or not.
 * @return      On success return 0, otherwise return a negative libfabric error code.
 */
ssize_t rxr_pkt_post_or_queue(struct rxr_ep *ep, struct rxr_op_entry *op_entry, int pkt_type, bool inject)
{
	ssize_t err;

	err = rxr_pkt_post(ep, op_entry, pkt_type, inject, 0);
	if (err == -FI_EAGAIN) {
		assert(!(op_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR));
		op_entry->rxr_flags |= RXR_OP_ENTRY_QUEUED_CTRL;
		op_entry->queued_ctrl.type = pkt_type;
		op_entry->queued_ctrl.inject = inject;
		dlist_insert_tail(&op_entry->queued_ctrl_entry,
				  &ep->op_entry_queued_ctrl_list);
		err = 0;
	}

	return err;
}

/**
 * @brief post req packet(s). Queue the post for multi-req packet types
 *
 * We must use rxr_pkt_post_or_queue() for multi-req packet types.
 *
 * This is because for multi-req packets, rxr_pkt_post() will
 * send multiple packets.
 *
 * It can happen that 1st packet was sent successfully, and the next
 * one encountered -FI_EAGAIN, which will cause rxr_pkt_post()
 * to return -FI_EAGAIN.
 *
 * If rxr_pkt_post() was used by this function, the -FI_EAGAIN will
 * be returned to user application. User application will then send
 * the entire message again. This would cause the receiver to receive duplicated
 * packets (because 1st packet was sent successfully).
 *
 * @param[in]   rxr_ep          endpoint
 * @param[in]   op_entry        pointer to rxr_op_entry. (either a tx_entry or an rx_entry)
 * @param[in]   pkt_type        packet type.
 * @param[in]   inject          send control packet via inject or not.
 * @return      On success return 0, otherwise return a negative libfabric error code.
 */
ssize_t rxr_pkt_post_req(struct rxr_ep *ep, struct rxr_op_entry *op_entry, int req_type, bool inject, uint64_t flags)
{
	assert(op_entry->type == RXR_TX_ENTRY);
	assert(req_type >= RXR_REQ_PKT_BEGIN);

	if (rxr_pkt_type_is_mulreq(req_type)) {
		assert(!inject);
		return rxr_pkt_post_or_queue(ep, op_entry, req_type, inject);
	}

	return rxr_pkt_post(ep, op_entry, req_type, inject, flags);
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
	struct rxr_op_entry *tx_entry;
	ssize_t err;

	if ((peer->flags & RXR_PEER_HANDSHAKE_RECEIVED) ||
	    (peer->flags & RXR_PEER_REQ_SENT))
		return 0;

	tx_entry = ofi_buf_alloc(ep->op_entry_pool);
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

	tx_entry->bytes_acked = 0;
	tx_entry->bytes_sent = 0;
	tx_entry->window = 0;
	tx_entry->rma_iov_count = 0;
	tx_entry->iov_count = 0;
	tx_entry->fi_flags = RXR_NO_COMPLETION | RXR_NO_COUNTER;
	tx_entry->rxr_flags = 0;

	dlist_insert_tail(&tx_entry->ep_entry, &ep->tx_entry_list);

	err = rxr_pkt_post(ep, tx_entry, RXR_EAGER_RTW_PKT, 0, 0);

	if (OFI_UNLIKELY(err))
		return err;

	return 0;
}

void rxr_pkt_handle_data_copied(struct rxr_ep *ep,
				struct rxr_pkt_entry *pkt_entry,
				size_t data_size)
{
	struct rxr_op_entry *op_entry;
	bool post_ctrl;
	int ctrl_type;

	op_entry = pkt_entry->x_entry;
	assert(op_entry);
	op_entry->bytes_copied += data_size;

	rxr_pkt_entry_release_rx(ep, pkt_entry);

	if (op_entry->total_len == op_entry->bytes_copied) {
		if (op_entry->cuda_copy_method == RXR_CUDA_COPY_GDRCOPY) {
			assert(ep->gdrcopy_rx_entry_num > 0);
			op_entry->cuda_copy_method = RXR_CUDA_COPY_UNSPEC;
			ep->gdrcopy_rx_entry_num -= 1;
		}

		post_ctrl = false;
		ctrl_type = 0;
		if (op_entry->rxr_flags & RXR_DELIVERY_COMPLETE_REQUESTED) {
			post_ctrl = true;
			ctrl_type = RXR_RECEIPT_PKT;
		}
		rxr_cq_complete_recv(ep, op_entry, post_ctrl, ctrl_type);
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
 *         Otherwise, the packet will be queued and resent by progress engine.
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
 * If the packet is not associated with a user operation (such packet include
 * HANDSHAKE):
 *
 *      If the error is RNR, the packet is queued and resent by progress engine.
 *
 *      For other types of error, an error EQ entry is written.
 *
 * @param[in]	ep		endpoint
 * @param[in]	pkt_entry	pkt entry
 * @param[in]	err		libfabric error code
 * @param[in]	prov_errno	provider specific error code
 */
void rxr_pkt_handle_send_error(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry, int err, int prov_errno)
{
	struct rdm_peer *peer;
	struct rxr_op_entry *tx_entry;
	struct rxr_op_entry *rx_entry;

	assert(pkt_entry->alloc_type == RXR_PKT_FROM_EFA_TX_POOL ||
	       pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL);

	FI_DBG(&rxr_prov, FI_LOG_CQ,
			"Packet send error: %s (%d)\n",
			efa_strerror(prov_errno), prov_errno);

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
		if (prov_errno == FI_EFA_REMOTE_ERROR_RNR) {
			/*
			 * handshake should always be queued for RNR
			 */
			assert(!(peer->flags & RXR_PEER_HANDSHAKE_QUEUED));
			peer->flags |= RXR_PEER_HANDSHAKE_QUEUED;
			dlist_insert_tail(&peer->handshake_queued_entry,
					  &ep->handshake_queued_peer_list);
		} else if (prov_errno != FI_EFA_REMOTE_ERROR_BAD_DEST_QPN) {
			/* If prov_errno is FI_EFA_REMOTE_ERROR_BAD_DEST_QPN  the peer has
			 * been destroyed. Which is normal, as peer does not always need a
			 * handshake packet to perform its duty. (For example, if a peer
			 * just want to sent 1 message to the ep, it does not need
			 * handshake.) In this case, it is safe to ignore this error
			 * completion. In all other cases, we write an eq entry because
			 * there is no application operation associated with handshake.
			 */
			char ep_addr_str[OFI_ADDRSTRLEN], peer_addr_str[OFI_ADDRSTRLEN];
			size_t buflen=0;

			memset(&ep_addr_str, 0, sizeof(ep_addr_str));
			memset(&peer_addr_str, 0, sizeof(peer_addr_str));
			buflen = sizeof(ep_addr_str);
			rxr_ep_raw_addr_str(ep, ep_addr_str, &buflen);
			buflen = sizeof(peer_addr_str);
			rxr_peer_raw_addr_str(ep, pkt_entry->addr, peer_addr_str, &buflen);
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"While sending a handshake packet, an error occurred."
				"  Our address: %s, peer address: %s\n",
				ep_addr_str, peer_addr_str);
			efa_eq_write_error(&ep->util_ep, err, prov_errno);
		}
		return;
	}

	switch (RXR_GET_X_ENTRY_TYPE(pkt_entry)) {
	case RXR_TX_ENTRY:
		tx_entry = pkt_entry->x_entry;
		if (prov_errno == FI_EFA_REMOTE_ERROR_RNR) {
			if (ep->handle_resource_management == FI_RM_DISABLED) {
				/*
				 * Write an error to the application for RNR when resource
				 * management is disabled.
				 *
				 * Note that a tx_entry might send multiple packets, therefore
				 * might encounter RNR from device multiple times, but it
				 * should only write cq err entry once
				 */
				if (!(tx_entry->rxr_flags & RXR_TX_ENTRY_WRITTEN_RNR_CQ_ERR_ENTRY)) {
					tx_entry->rxr_flags |= RXR_TX_ENTRY_WRITTEN_RNR_CQ_ERR_ENTRY;
					rxr_cq_write_tx_error(ep, pkt_entry->x_entry, FI_ENORX, FI_EFA_REMOTE_ERROR_RNR);
				}

				rxr_pkt_entry_release_tx(ep, pkt_entry);
				if (!tx_entry->efa_outstanding_tx_ops)
					rxr_release_tx_entry(ep, tx_entry);
			} else {
				/*
				 * This packet is associated with a send operation, (such
				 * packets include all REQ, DATA) thus shoud be queued for RNR
				 * only if application wants EFA to manager resource.
				 */
				rxr_cq_queue_rnr_pkt(ep, &tx_entry->queued_pkts, pkt_entry);
				if (!(tx_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR)) {
					tx_entry->rxr_flags |= RXR_TX_ENTRY_QUEUED_RNR;
					dlist_insert_tail(&tx_entry->queued_rnr_entry,
							  &ep->tx_entry_queued_rnr_list);
				}
			}
		} else {
			rxr_cq_write_tx_error(ep, pkt_entry->x_entry, err, prov_errno);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		}
		break;
	case RXR_RX_ENTRY:
		rx_entry = pkt_entry->x_entry;
		if (prov_errno == FI_EFA_REMOTE_ERROR_RNR) {
			/*
			 * This packet is associated with a recv operation, (such packets
			 * include CTS and EOR) thus should always be queued for RNR. This
			 * is regardless value of ep->handle_resource_management, because
			 * resource management is only applied to send operation.
			 */
			rxr_cq_queue_rnr_pkt(ep, &rx_entry->queued_pkts, pkt_entry);
			if (!(rx_entry->rxr_flags & RXR_RX_ENTRY_QUEUED_RNR)) {
				rx_entry->rxr_flags |= RXR_RX_ENTRY_QUEUED_RNR;
				dlist_insert_tail(&rx_entry->queued_rnr_entry,
						  &ep->rx_entry_queued_rnr_list);
			}
		} else {
			rxr_cq_write_rx_error(ep, pkt_entry->x_entry, err, prov_errno);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		}
		break;
	case RXR_READ_ENTRY:
		/* read will not encounter RNR */
		assert(prov_errno != FI_EFA_REMOTE_ERROR_RNR);
		rxr_read_write_error(ep, pkt_entry->x_entry, err, prov_errno);
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
				"%s unknown x_entry type %d\n",
				__func__, RXR_GET_X_ENTRY_TYPE(pkt_entry));
		assert(0 && "unknown x_entry state");
		efa_eq_write_error(&ep->util_ep, err, prov_errno);
		rxr_pkt_entry_release_tx(ep, pkt_entry);
		break;
	}
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
	case RXR_LONGCTS_MSGRTM_PKT:
	case RXR_LONGCTS_TAGRTM_PKT:
		rxr_pkt_handle_longcts_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_LONGREAD_MSGRTM_PKT:
	case RXR_LONGREAD_TAGRTM_PKT:
		rxr_pkt_handle_longread_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_RUNTREAD_MSGRTM_PKT:
	case RXR_RUNTREAD_TAGRTM_PKT:
		rxr_pkt_handle_runtread_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_EAGER_RTW_PKT:
		rxr_pkt_handle_eager_rtw_send_completion(ep, pkt_entry);
		break;
	case RXR_LONGCTS_RTW_PKT:
		rxr_pkt_handle_longcts_rtw_send_completion(ep, pkt_entry);
		break;
	case RXR_LONGREAD_RTW_PKT:
		rxr_pkt_handle_longread_rtw_send_completion(ep, pkt_entry);
		break;
	case RXR_SHORT_RTR_PKT:
	case RXR_LONGCTS_RTR_PKT:
		/* Unlike other protocol, for emulated read, tx_entry
	 	 * is released in rxr_cq_complete_recv().
	         * Therefore there is nothing to be done here. */
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
	case RXR_DC_LONGCTS_MSGRTM_PKT:
	case RXR_DC_LONGCTS_TAGRTM_PKT:
		rxr_pkt_handle_dc_longcts_rtm_send_completion(ep, pkt_entry);
		break;
	case RXR_DC_LONGCTS_RTW_PKT:
		rxr_pkt_handle_dc_longcts_rtw_send_completion(ep, pkt_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"invalid control pkt type %d\n",
			rxr_get_base_hdr(pkt_entry->pkt)->type);
		assert(0 && "invalid control pkt type");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
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
	FI_DBG(&rxr_prov, FI_LOG_CQ,
			"Packet receive error: %s (%d)\n",
			efa_strerror(prov_errno), prov_errno);

	if (!pkt_entry->x_entry) {
		char ep_addr_str[OFI_ADDRSTRLEN];
		size_t buflen=0;

		memset(&ep_addr_str, 0, sizeof(ep_addr_str));
		buflen = sizeof(ep_addr_str);
		rxr_ep_raw_addr_str(ep, ep_addr_str, &buflen);
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Packet receive error from non TX/RX packet.  Our address: %s\n",
			ep_addr_str);

		efa_eq_write_error(&ep->util_ep, err, prov_errno);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
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
	if (base_hdr->version < RXR_PROTOCOL_VERSION) {
		/* Allocate RXR_MAX_NAME_LENGTH * 3 to allow compilation with MSVC 
		 * We are calling abort after this, so allocating on stack is fine
		 */
		char host_gid[RXR_MAX_NAME_LENGTH * 3];
		int length = 0;

		for (i = 0; i < ep->core_addrlen; i++)
			length += sprintf(&host_gid[length], "%02x ",
					  ep->core_addr[i]);
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Host %s received a packet with invalid protocol version %d.\n"
			"This host can only support protocol version %d and above.\n",
			host_gid, base_hdr->version, RXR_PROTOCOL_VERSION);
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		fprintf(stderr, "Host %s received a packet with invalid protocol version %d.\n"
			"This host can only support protocol version %d and above. %s:%d\n",
			host_gid, base_hdr->version, RXR_PROTOCOL_VERSION, __FILE__, __LINE__);
		abort();
	}

	assert(base_hdr->type >= RXR_REQ_PKT_BEGIN);

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	ret = efa_av_insert_one(efa_ep->av, (struct efa_ep_addr *)raw_addr,
	                        &rdm_addr, 0, NULL);
	if (OFI_UNLIKELY(ret != 0)) {
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_AV_INSERT);
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
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_DEPRECATED_PKT_TYPE);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	case RXR_RETIRED_CONNACK_PKT:
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Received a CONNACK packet, which has been retired since protocol version 4\n");
		assert(0 && "deprecated CONNACK pakcet received");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_DEPRECATED_PKT_TYPE);
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
	case RXR_LONGCTS_MSGRTM_PKT:
	case RXR_LONGCTS_TAGRTM_PKT:
	case RXR_DC_LONGCTS_MSGRTM_PKT:
	case RXR_DC_LONGCTS_TAGRTM_PKT:
	case RXR_LONGREAD_MSGRTM_PKT:
	case RXR_LONGREAD_TAGRTM_PKT:
	case RXR_RUNTREAD_MSGRTM_PKT:
	case RXR_RUNTREAD_TAGRTM_PKT:
	case RXR_WRITE_RTA_PKT:
	case RXR_DC_WRITE_RTA_PKT:
	case RXR_FETCH_RTA_PKT:
	case RXR_COMPARE_RTA_PKT:
		rxr_pkt_handle_rtm_rta_recv(ep, pkt_entry);
		return;
	case RXR_EAGER_RTW_PKT:
		rxr_pkt_handle_eager_rtw_recv(ep, pkt_entry);
		return;
	case RXR_LONGCTS_RTW_PKT:
	case RXR_DC_LONGCTS_RTW_PKT:
		rxr_pkt_handle_longcts_rtw_recv(ep, pkt_entry);
		return;
	case RXR_LONGREAD_RTW_PKT:
		rxr_pkt_handle_longread_rtw_recv(ep, pkt_entry);
		return;
	case RXR_SHORT_RTR_PKT:
	case RXR_LONGCTS_RTR_PKT:
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
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}
}

/**
 * @brief Read peer raw address from packet header and insert the peer in AV.
 * @param ep Pointer to RDM endpoint
 * @param pkt_entry Pointer to packet entry
 * @returns Peer address, or FI_ADDR_NOTAVIL if the packet header does not include raw address
 */
fi_addr_t rxr_pkt_determine_addr(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	if (base_hdr->type >= RXR_REQ_PKT_BEGIN && rxr_pkt_req_raw_addr(pkt_entry)) {
		void *raw_addr;
		raw_addr = rxr_pkt_req_raw_addr(pkt_entry);
		assert(raw_addr);
		return rxr_pkt_insert_addr(ep, pkt_entry, raw_addr);
	}

	return FI_ADDR_NOTAVAIL;
}

/**
 * @brief handle a received packet
 *
 * @param	ep[in,out]		endpoint
 * @param	pkt_entry[in,out]	received packet, will be released by this function
 * @param	lower_ep_type[in]	indicates which type of lower device this packet was received from.
 * 					Possible values are SHM_EP and EFA_EP.
 */
void rxr_pkt_handle_recv_completion(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry,
				    enum rxr_lower_ep_type lower_ep_type)
{
	int pkt_type;
	struct rdm_peer *peer;
	struct rxr_base_hdr *base_hdr;
	struct rxr_op_entry *zcpy_rx_entry = NULL;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	pkt_type = base_hdr->type;
	if (pkt_type >= RXR_EXTRA_REQ_PKT_END) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Peer %d is requesting feature %d, which this EP does not support.\n",
			(int)pkt_entry->addr, base_hdr->type);

		assert(0 && "invalid REQ packe type");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	/*
	 * Ignore packet if peer address cannot be determined. This ususally happens if
	 * we had prior communication with the peer, but
	 * application called fi_av_remove() to remove the address
	 * from address vector.
	 */
	if (pkt_entry->addr == FI_ADDR_NOTAVAIL) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Warning: ignoring a received packet from a removed address. packet type: %" PRIu8
			", packet flags: %x\n", rxr_get_base_hdr(pkt_entry->pkt)->type, rxr_get_base_hdr(pkt_entry->pkt)->flags);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

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
	if (peer->is_local && lower_ep_type == EFA_EP) {
		/*
		 * This happens when the peer is on same instance, but chose to
		 * use EFA device to communicate with me. In this case, we respect
		 * that and will not use shm with the peer.
		 * TODO: decide whether to use shm through handshake packet.
		 */
		peer->is_local = 0;
	}

	rxr_pkt_post_handshake_or_queue(ep, peer);

	if (lower_ep_type == SHM_EP) {
		ep->shm_rx_pkts_posted--;
	} else {
		assert(lower_ep_type == EFA_EP);
		ep->efa_rx_pkts_posted--;
	}

	if (pkt_entry->alloc_type == RXR_PKT_FROM_USER_BUFFER) {
		assert(pkt_entry->x_entry);
		zcpy_rx_entry = pkt_entry->x_entry;
	}

	rxr_pkt_proc_received(ep, pkt_entry);

	if (zcpy_rx_entry && pkt_type != RXR_EAGER_MSGRTM_PKT) {
		/* user buffer was not matched with a message,
		 * therefore reposting the buffer */
		rxr_ep_post_user_recv_buf(ep, zcpy_rx_entry, 0);
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
	       "%s RxR HANDSHAKE packet, nextra_p3: %d\n",
	       prefix, handshake_hdr->nextra_p3);
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
	       cts_hdr->send_id, cts_hdr->recv_id, cts_hdr->recv_length);
}

static
void rxr_pkt_print_data(char *prefix, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_data_hdr *data_hdr;
	char str[RXR_PKT_DUMP_DATA_LEN * 4];
	size_t str_len = RXR_PKT_DUMP_DATA_LEN * 4, l, hdr_size;
	uint8_t *data;
	int i;

	str[str_len - 1] = '\0';

	data_hdr = rxr_get_data_hdr(pkt_entry->pkt);

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s RxR DATA packet -  version: %" PRIu8
	       " flags: %x rx_id: %" PRIu32
	       " seg_size: %"	     PRIu64
	       " seg_offset: %"	     PRIu64
	       "\n", prefix, data_hdr->version, data_hdr->flags,
	       data_hdr->recv_id, data_hdr->seg_length,
	       data_hdr->seg_offset);

	hdr_size = sizeof(struct rxr_data_hdr);
	if (data_hdr->flags & RXR_PKT_CONNID_HDR) {
		hdr_size += sizeof(struct rxr_data_opt_connid_hdr);
		FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
		       "sender_connid: %d\n",
		       data_hdr->connid_hdr->connid);
	}

	data = (uint8_t *)pkt_entry->pkt + hdr_size;

	l = snprintf(str, str_len, ("\tdata:    "));
	for (i = 0; i < MIN(data_hdr->seg_length, RXR_PKT_DUMP_DATA_LEN);
	     i++)
		l += snprintf(str + l, str_len - l, "%02x ",
			      data[i]);
	FI_DBG(&rxr_prov, FI_LOG_EP_DATA, "%s\n", str);
}

void rxr_pkt_print(char *prefix, struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *hdr;

	hdr = rxr_get_base_hdr(pkt_entry->pkt);

	switch (hdr->type) {
	case RXR_HANDSHAKE_PKT:
		rxr_pkt_print_handshake(prefix, rxr_get_handshake_hdr(pkt_entry->pkt));
		break;
	case RXR_CTS_PKT:
		rxr_pkt_print_cts(prefix, rxr_get_cts_hdr(pkt_entry->pkt));
		break;
	case RXR_DATA_PKT:
		rxr_pkt_print_data(prefix, pkt_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid ctl pkt type %d\n",
			rxr_get_base_hdr(hdr)->type);
		assert(0);
		return;
	}
}
#endif

