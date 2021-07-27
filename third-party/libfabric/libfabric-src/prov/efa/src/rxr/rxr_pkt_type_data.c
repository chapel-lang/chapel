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

/*
 * This function contains data packet related functions
 * Data packet is used by long message protocol.
 */

/*
 * Functions to send data packet, including
 */

ssize_t rxr_pkt_send_data(struct rxr_ep *ep,
			  struct rxr_tx_entry *tx_entry,
			  struct rxr_pkt_entry *pkt_entry)
{
	uint64_t payload_size, copied_size;
	struct rxr_data_pkt *data_pkt;
	struct efa_mr *desc;

	pkt_entry->x_entry = (void *)tx_entry;
	pkt_entry->addr = tx_entry->addr;
	desc = tx_entry->desc[0];

	payload_size = MIN(tx_entry->total_len - tx_entry->bytes_sent,
			   ep->max_data_payload_size);
	payload_size = MIN(payload_size, tx_entry->window);

	data_pkt = (struct rxr_data_pkt *)pkt_entry->pkt;
	data_pkt->hdr.seg_size = payload_size;

	copied_size = ofi_copy_from_hmem_iov(data_pkt->data,
					     payload_size,
					     desc ? desc->peer.iface : FI_HMEM_SYSTEM,
					     desc ? desc->peer.device.reserved : 0,
					     tx_entry->iov,
					     tx_entry->iov_count,
					     tx_entry->bytes_sent);
	assert(copied_size == payload_size);

	pkt_entry->pkt_size = copied_size + sizeof(struct rxr_data_hdr);
	pkt_entry->addr = tx_entry->addr;

	return rxr_pkt_entry_send_with_flags(ep, pkt_entry, pkt_entry->addr,
					     tx_entry->send_flags);
}

/*
 * Copies all consecutive small iov's into one buffer. If the function reaches
 * an iov greater than the max memcpy size, it will end, only copying up to
 * that iov.
 */
static size_t rxr_copy_from_iov(void *buf, uint64_t remaining_len,
				struct rxr_tx_entry *tx_entry)
{
	struct iovec *tx_iov = tx_entry->iov;
	uint64_t done = 0, len;

	while (tx_entry->iov_index < tx_entry->iov_count &&
	       done < remaining_len) {
		len = tx_iov[tx_entry->iov_index].iov_len;
		if (tx_entry->mr[tx_entry->iov_index])
			break;

		len -= tx_entry->iov_offset;

		/*
		 * If the amount to be written surpasses the remaining length,
		 * copy up to the remaining length and return, else copy the
		 * entire iov and continue.
		 */
		if (done + len > remaining_len) {
			len = remaining_len - done;
			memcpy((char *)buf + done,
			       (char *)tx_iov[tx_entry->iov_index].iov_base +
			       tx_entry->iov_offset, len);
			tx_entry->iov_offset += len;
			done += len;
			break;
		}
		memcpy((char *)buf + done,
		       (char *)tx_iov[tx_entry->iov_index].iov_base +
		       tx_entry->iov_offset, len);
		tx_entry->iov_index++;
		tx_entry->iov_offset = 0;
		done += len;
	}
	return done;
}

ssize_t rxr_pkt_send_data_desc(struct rxr_ep *ep,
			       struct rxr_tx_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_data_pkt *data_pkt;
	/* The user's iov */
	struct iovec *tx_iov = tx_entry->iov;
	/* The constructed iov to be passed to sendv
	 * and corresponding fid_mrs
	 */
	struct iovec iov[ep->core_iov_limit];
	void *desc[ep->core_iov_limit];
	/* Constructed iov's total size */
	uint64_t payload_size = 0;
	/* pkt_entry offset to write data into */
	uint64_t pkt_used = 0;
	uint64_t orig_iov_index;
	uint64_t orig_iov_offset;
	/* Remaining size that can fit in the constructed iov */
	uint64_t remaining_len = MIN(tx_entry->window,
				     ep->max_data_payload_size);
	/* The constructed iov's index */
	size_t i = 0;
	size_t len = 0;

	ssize_t ret;

	orig_iov_index = tx_entry->iov_index;
	orig_iov_offset = tx_entry->iov_offset;

	data_pkt = (struct rxr_data_pkt *)pkt_entry->pkt;
	/* Assign packet header in constructed iov */
	iov[i].iov_base = rxr_pkt_start(pkt_entry);
	iov[i].iov_len = sizeof(struct rxr_data_hdr);
	desc[i] = fi_mr_desc(pkt_entry->mr);
	i++;

	/*
	 * Loops until payload size is at max, all user iovs are sent, the
	 * constructed iov count is greater than the core iov limit, or the tx
	 * entry window is exhausted.  Each iteration fills one entry of the
	 * iov to be sent.
	 */
	while (tx_entry->iov_index < tx_entry->iov_count &&
	       remaining_len > 0 && i < ep->core_iov_limit) {
		if (tx_entry->desc[tx_entry->iov_index]) {
			iov[i].iov_base =
				(char *)tx_iov[tx_entry->iov_index].iov_base +
				tx_entry->iov_offset;
			desc[i] = tx_entry->desc[tx_entry->iov_index];

			len = tx_iov[tx_entry->iov_index].iov_len
			      - tx_entry->iov_offset;
			if (len > remaining_len) {
				len = remaining_len;
				tx_entry->iov_offset += len;
			} else {
				tx_entry->iov_index++;
				tx_entry->iov_offset = 0;
			}
			iov[i].iov_len = len;
		} else {
			/* It should be noted for cuda buffer, caller will always
			 * provide desc, and will not enter this branch.
			 *
			 * Copies any consecutive small iov's, returning size
			 * written while updating iov index and offset
			 */

			len = rxr_copy_from_iov((char *)data_pkt->data +
						 pkt_used,
						 remaining_len,
						 tx_entry);

			iov[i].iov_base = (char *)data_pkt->data + pkt_used;
			iov[i].iov_len = len;
			desc[i] = fi_mr_desc(pkt_entry->mr);
			pkt_used += len;
		}
		payload_size += len;
		remaining_len -= len;
		i++;
	}
	data_pkt->hdr.seg_size = (uint16_t)payload_size;
	pkt_entry->pkt_size = payload_size + RXR_DATA_HDR_SIZE;
	pkt_entry->x_entry = tx_entry;
	pkt_entry->addr = tx_entry->addr;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "Sending an iov count, %zu with payload size: %lu.\n",
	       i, payload_size);
	ret = rxr_pkt_entry_sendv(ep, pkt_entry, tx_entry->addr,
				  (const struct iovec *)iov,
				  desc, i, tx_entry->send_flags);
	if (OFI_UNLIKELY(ret)) {
		/* Reset tx_entry iov pointer on send failure. */
		tx_entry->iov_index = orig_iov_index;
		tx_entry->iov_offset = orig_iov_offset;
	}
	return ret;
}

void rxr_pkt_handle_data_send_completion(struct rxr_ep *ep,
					 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked +=
		rxr_get_data_pkt(pkt_entry->pkt)->hdr.seg_size;

	if (tx_entry->total_len == tx_entry->bytes_acked) {
		if (!(tx_entry->rxr_flags & RXR_DELIVERY_COMPLETE_REQUESTED))
			rxr_cq_handle_tx_completion(ep, tx_entry);
		else
			if (tx_entry->rxr_flags & RXR_RECEIPT_RECEIVED)
				/*
				 * For long message protocol,
				 * when FI_DELIVERY_COMPLETE
				 * is requested,
				 * we have to write tx completions
				 * in either
				 * rxr_pkt_handle_data_send_completion()
				 * or rxr_pkt_handle_receipt_recv()
				 * depending on which of them
				 * is called later due
				 * to avoid accessing released
				 * tx_entry.
				 */
				rxr_cq_handle_tx_completion(ep, tx_entry);
	}
}

/*
 *  rxr_pkt_handle_data_recv() and related functions
 */

/*
 * rxr_pkt_proc_data() processes data in a DATA/READRSP
 * pakcet entry.
 */
void rxr_pkt_proc_data(struct rxr_ep *ep,
		       struct rxr_rx_entry *rx_entry,
		       struct rxr_pkt_entry *pkt_entry,
		       char *data, size_t seg_offset,
		       size_t seg_size)
{
	struct rxr_peer *peer;
	bool all_received = 0;
	ssize_t err;

#if ENABLE_DEBUG
	int pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;

	assert(pkt_type == RXR_DATA_PKT || pkt_type == RXR_READRSP_PKT);
#endif
	rx_entry->bytes_received += seg_size;
	assert(rx_entry->bytes_received <= rx_entry->total_len);
	all_received = (rx_entry->bytes_received == rx_entry->total_len);

	peer = rxr_ep_get_peer(ep, rx_entry->addr);
	peer->rx_credits += ofi_div_ceil(seg_size, ep->max_data_payload_size);

	rx_entry->window -= seg_size;
	if (ep->available_data_bufs < rxr_get_rx_pool_chunk_cnt(ep))
		ep->available_data_bufs++;

#if ENABLE_DEBUG
	/* rx_entry can be released by rxr_pkt_copy_to_rx
	 * so the call to dlist_remove must happen before
	 * call to rxr_copy_to_rx
	 */
	if (all_received) {
		dlist_remove(&rx_entry->rx_pending_entry);
		ep->rx_pending--;
	}
#endif
	err = rxr_pkt_copy_to_rx(ep, rx_entry, seg_offset,
				 pkt_entry, data, seg_size);
	if (err) {
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		rxr_cq_handle_rx_error(ep, rx_entry, err);
	}

	if (all_received)
		return;

	if (!rx_entry->window) {
		assert(rx_entry->state == RXR_RX_RECV);
		err = rxr_pkt_post_ctrl_or_queue(ep, RXR_RX_ENTRY, rx_entry, RXR_CTS_PKT, 0);
		if (err) {
			FI_WARN(&rxr_prov, FI_LOG_CQ, "post CTS packet failed!\n");
			rxr_cq_handle_rx_error(ep, rx_entry, err);
		}
	}
}

void rxr_pkt_handle_data_recv(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_data_pkt *data_pkt;
	struct rxr_rx_entry *rx_entry;

	data_pkt = (struct rxr_data_pkt *)pkt_entry->pkt;

	rx_entry = ofi_bufpool_get_ibuf(ep->rx_entry_pool,
					data_pkt->hdr.rx_id);

	rxr_pkt_proc_data(ep, rx_entry,
			  pkt_entry,
			  data_pkt->data,
			  data_pkt->hdr.seg_offset,
			  data_pkt->hdr.seg_size);
}

