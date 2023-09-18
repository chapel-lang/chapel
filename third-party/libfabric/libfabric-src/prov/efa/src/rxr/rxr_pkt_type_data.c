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
#include "rxr_pkt_cmd.h"
#include "rxr_pkt_type_base.h"

int rxr_pkt_init_data(struct rxr_ep *ep,
		      struct rxr_op_entry *op_entry,
		      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_data_hdr *data_hdr;
	struct rdm_peer *peer;
	size_t hdr_size;
	int ret;

	data_hdr = rxr_get_data_hdr(pkt_entry->pkt);
	data_hdr->type = RXR_DATA_PKT;
	data_hdr->version = RXR_PROTOCOL_VERSION;
	data_hdr->flags = 0;

	/* Data is sent using rx_entry in the emulated longcts read 
	 * protocol. The emulated longcts write and the longcts 
	 * message protocols sends data using tx_entry.
	 * This check ensures appropriate recv_id is 
	 * assigned for the respective protocols */
        if (op_entry->type == RXR_RX_ENTRY)
		data_hdr->recv_id = op_entry->tx_id;
   	else {
		assert(op_entry->type == RXR_TX_ENTRY);
		data_hdr->recv_id = op_entry->rx_id;
        }

	hdr_size = sizeof(struct rxr_data_hdr);
	peer = rxr_ep_get_peer(ep, op_entry->addr);
	assert(peer);
	if (rxr_peer_need_connid(peer)) {
		data_hdr->flags |= RXR_PKT_CONNID_HDR;
		data_hdr->connid_hdr->connid = rxr_ep_raw_addr(ep)->qkey;
		hdr_size += sizeof(struct rxr_data_opt_connid_hdr);
	}

	/*
	 * Data packets are sent in order so using bytes_sent is okay here.
	 */
	data_hdr->seg_offset = op_entry->bytes_sent;
	data_hdr->seg_length = MIN(op_entry->total_len - op_entry->bytes_sent,
				   ep->max_data_payload_size);
	data_hdr->seg_length = MIN(data_hdr->seg_length, op_entry->window);
	ret = rxr_pkt_init_data_from_op_entry(ep, pkt_entry, hdr_size,
					      op_entry, op_entry->bytes_sent,
					      data_hdr->seg_length);
	if (ret)
		return ret;

	pkt_entry->x_entry = (void *)op_entry;
	pkt_entry->addr = op_entry->addr;

	return 0;
}

void rxr_pkt_handle_data_sent(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *op_entry;
	struct rxr_data_hdr *data_hdr;

	data_hdr = rxr_get_data_hdr(pkt_entry->pkt);
	assert(data_hdr->seg_length > 0);

	op_entry = pkt_entry->x_entry;
	op_entry->bytes_sent += data_hdr->seg_length;
	op_entry->window -= data_hdr->seg_length;
	assert(op_entry->window >= 0);
}

void rxr_pkt_handle_data_send_completion(struct rxr_ep *ep,
					 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *op_entry;

	op_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	op_entry->bytes_acked +=
		rxr_get_data_hdr(pkt_entry->pkt)->seg_length;

	if (op_entry->total_len == op_entry->bytes_acked) {
		if (!(op_entry->rxr_flags & RXR_DELIVERY_COMPLETE_REQUESTED))
			rxr_cq_handle_send_completion(ep, op_entry);
		else
			if (op_entry->rxr_flags & RXR_RECEIPT_RECEIVED)
				/* For long message protocol,
				 * when FI_DELIVERY_COMPLETE
				 * is requested, we have to write op
				 * completions in either
				 * rxr_pkt_handle_data_send_completion()
				 * or rxr_pkt_handle_receipt_recv()
				 * depending on which of them is called 
				 * later in order to avoid accessing 
				 * released op_entry. */
				rxr_cq_handle_send_completion(ep, op_entry);
	}
}

/*
 *  rxr_pkt_handle_data_recv() and related functions
 */

/*
 * rxr_pkt_proc_data() processes data in a DATA/READRSP
 * packet entry.
 */
void rxr_pkt_proc_data(struct rxr_ep *ep,
		       struct rxr_op_entry *op_entry,
		       struct rxr_pkt_entry *pkt_entry,
		       char *data, size_t seg_offset,
		       size_t seg_size)
{
	bool all_received = 0;
	ssize_t err;

#if ENABLE_DEBUG
	int pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;

	assert(pkt_type == RXR_DATA_PKT || pkt_type == RXR_READRSP_PKT);
#endif
	op_entry->bytes_received += seg_size;
	assert(op_entry->bytes_received <= op_entry->total_len);
	all_received = (op_entry->bytes_received == op_entry->total_len);

	op_entry->window -= seg_size;
#if ENABLE_DEBUG
	/* op_entry can be released by rxr_pkt_copy_data_to_op_entry
	 * so the call to dlist_remove must happen before
	 * call to rxr_copy_data_to_op_entry
	 */
	if (all_received) {
		dlist_remove(&op_entry->pending_recv_entry);
		ep->pending_recv_counter--;
	}
#endif
	err = rxr_pkt_copy_data_to_op_entry(ep, op_entry, seg_offset,
					    pkt_entry, data, seg_size);
	if (err) {
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		rxr_cq_write_rx_error(ep, op_entry, -err, FI_EFA_ERR_RX_ENTRY_COPY);
	}

	if (all_received)
		return;

	if (!op_entry->window) {
		err = rxr_pkt_post_or_queue(ep, op_entry, RXR_CTS_PKT, 0);
		if (err) {
			FI_WARN(&rxr_prov, FI_LOG_CQ, "post CTS packet failed!\n");
			rxr_cq_write_rx_error(ep, op_entry, -err, FI_EFA_ERR_PKT_POST);
		}
	}
}

void rxr_pkt_handle_data_recv(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_data_hdr *data_hdr;
	struct rxr_op_entry *op_entry;
	size_t hdr_size;

	data_hdr = rxr_get_data_hdr(pkt_entry->pkt);

	op_entry = ofi_bufpool_get_ibuf(ep->op_entry_pool,
					data_hdr->recv_id);

	hdr_size = sizeof(struct rxr_data_hdr);
	if (data_hdr->flags & RXR_PKT_CONNID_HDR)
		hdr_size += sizeof(struct rxr_data_opt_connid_hdr);

	rxr_pkt_proc_data(ep, op_entry,
			  pkt_entry,
			  pkt_entry->pkt + hdr_size,
			  data_hdr->seg_offset,
			  data_hdr->seg_length);
}

