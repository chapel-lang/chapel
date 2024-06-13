/*
 * Copyright (c) 2013-2020 Intel Corporation. All rights reserved.
 * Copyright (c) 2018 Cray Inc. All rights reserved.
 * Copyright (c) 2018 System Fabric Works, Inc. All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "ofi.h"
#include "ofi_iov.h"
#include "ofi_atomic.h"

#include "rxm.h"

size_t rxm_cm_progress_interval;
size_t rxm_cq_eq_fairness;

static const char *
rxm_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
		const void *err_data, char *buf, size_t len)
{
	struct util_cq *cq;
	struct rxm_ep *rxm_ep;
	struct fid_list_entry *fid_entry;

	cq = container_of(cq_fid, struct util_cq, cq_fid);
	fid_entry = container_of(cq->ep_list.next, struct fid_list_entry, entry);
	rxm_ep = container_of(fid_entry->fid, struct rxm_ep, util_ep.ep_fid);

	return fi_cq_strerror(rxm_ep->msg_cq, prov_errno, err_data, buf, len);
}

static struct rxm_rx_buf *
rxm_rx_buf_alloc(struct rxm_ep *rxm_ep, struct fid_ep *rx_ep)
{
	struct rxm_rx_buf *rx_buf;

	rx_buf = ofi_buf_alloc(rxm_ep->rx_pool);
	if (!rx_buf)
		return NULL;

	assert(rx_buf->ep == rxm_ep);
	rx_buf->hdr.state = RXM_RX;
	rx_buf->rx_ep = rx_ep;
	rx_buf->repost = true;

	if (!rxm_ep->msg_srx)
		rx_buf->conn = rx_ep->fid.context;

	return rx_buf;
}

/* Processing on the current rx buffer is expected to be slow.
 * Post a new buffer to take its place, and mark the current
 * buffer to return to the free pool when finished.
 */
static void rxm_replace_rx_buf(struct rxm_rx_buf *rx_buf)
{
	struct rxm_rx_buf *new_rx_buf;
	int ret;

	new_rx_buf = rxm_rx_buf_alloc(rx_buf->ep, rx_buf->rx_ep);
	if (!new_rx_buf)
		return;

	rx_buf->repost = false;
	ret = rxm_post_recv(new_rx_buf);
	if (ret)
		ofi_buf_free(new_rx_buf);
}

static void rxm_finish_buf_recv(struct rxm_rx_buf *rx_buf)
{
	uint64_t flags;
	char *data;

	if ((rx_buf->pkt.ctrl_hdr.type == rxm_ctrl_seg) &&
	    rxm_sar_get_seg_type(&rx_buf->pkt.ctrl_hdr) != RXM_SAR_SEG_FIRST) {
		dlist_insert_tail(&rx_buf->unexp_msg.entry,
				  &rx_buf->conn->deferred_sar_segments);
		rxm_replace_rx_buf(rx_buf);
	}

	flags = (rx_buf->pkt.hdr.flags | ofi_rx_flags[rx_buf->pkt.hdr.op]);

	if (rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_eager)
		flags |= FI_MORE;

	if (rx_buf->pkt.ctrl_hdr.type == rxm_ctrl_rndv_req)
		data = rxm_pkt_rndv_data(&rx_buf->pkt);
	else
		data = rx_buf->pkt.data;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "writing buffered recv completion: "
	       "length: %" PRIu64 "\n", rx_buf->pkt.hdr.size);
	rx_buf->recv_context.ep = &rx_buf->ep->util_ep.ep_fid;

	rxm_cq_write_recv_comp(rx_buf, &rx_buf->recv_context, flags,
			       rx_buf->pkt.hdr.size, data);
}

static void rxm_cq_write_error_trunc(struct rxm_rx_buf *rx_buf, size_t done_len)
{
	int ret;

	if (rx_buf->ep->util_ep.flags & OFI_CNTR_ENABLED)
		rxm_cntr_incerr(rx_buf->ep->util_ep.cntrs[CNTR_RX]);

	FI_WARN(&rxm_prov, FI_LOG_CQ, "Message truncated: "
		"recv buf length: %zu message length: %" PRIu64 "\n",
		done_len, rx_buf->pkt.hdr.size);
	ret = ofi_cq_write_error_trunc(rx_buf->ep->util_ep.rx_cq,
				       rx_buf->recv_entry->context,
				       rx_buf->recv_entry->comp_flags |
				       rx_buf->pkt.hdr.flags,
				       rx_buf->pkt.hdr.size,
				       rx_buf->recv_entry->rxm_iov.iov[0].iov_base,
				       rx_buf->pkt.hdr.data, rx_buf->pkt.hdr.tag,
				       rx_buf->pkt.hdr.size - done_len);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unable to write recv error CQ\n");
		assert(0);
	}
}

static void rxm_finish_recv(struct rxm_rx_buf *rx_buf, size_t done_len)
{
	struct rxm_recv_entry *recv_entry = rx_buf->recv_entry;

	if (done_len < rx_buf->pkt.hdr.size) {
		rxm_cq_write_error_trunc(rx_buf, done_len);
		goto release;
	}

	if (rx_buf->recv_entry->flags & FI_COMPLETION ||
	    rx_buf->ep->rxm_info->mode & FI_BUFFERED_RECV) {
		rxm_cq_write_recv_comp(rx_buf, rx_buf->recv_entry->context,
				       rx_buf->recv_entry->comp_flags |
				       rx_buf->pkt.hdr.flags |
				       (rx_buf->recv_entry->flags & FI_MULTI_RECV),
				       rx_buf->pkt.hdr.size,
				       rx_buf->recv_entry->rxm_iov.
				       iov[0].iov_base);
	}
	ofi_ep_cntr_inc(&rx_buf->ep->util_ep, CNTR_RX);

release:
	rxm_recv_entry_release(recv_entry);
	rxm_free_rx_buf(rx_buf);
}

static void
rxm_cq_write_tx_comp(struct rxm_ep *rxm_ep, uint64_t comp_flags,
		     void *app_context,  uint64_t flags)
{
	if (flags & FI_COMPLETION) {
		rxm_cq_write(rxm_ep->util_ep.tx_cq, app_context,
			     comp_flags, 0, NULL, 0, 0);
	}
}

static void rxm_finish_rma(struct rxm_ep *rxm_ep, struct rxm_tx_buf *rma_buf,
			  uint64_t comp_flags)
{
	assert(((comp_flags & FI_WRITE) && !(comp_flags & FI_READ)) ||
	       ((comp_flags & FI_READ) && !(comp_flags & FI_WRITE)));

	rxm_cq_write_tx_comp(rxm_ep, comp_flags, rma_buf->app_context,
			     rma_buf->flags);

	if (comp_flags & FI_WRITE)
		ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_WR);
	else
		ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_RD);

	if (!(rma_buf->flags & FI_INJECT) && !rxm_ep->rdm_mr_local &&
	    rxm_ep->msg_mr_local) {
		rxm_msg_mr_closev(rma_buf->rma.mr, rma_buf->rma.count);
	}

	rxm_free_tx_buf(rxm_ep, rma_buf);
}

void rxm_finish_eager_send(struct rxm_ep *rxm_ep, struct rxm_tx_buf *tx_buf)
{
	assert(ofi_tx_cq_flags(tx_buf->pkt.hdr.op) & FI_SEND);

	rxm_cq_write_tx_comp(rxm_ep, ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
			     tx_buf->app_context, tx_buf->flags);
	ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_TX);
}

static bool rxm_complete_sar(struct rxm_ep *rxm_ep,
			     struct rxm_tx_buf *tx_buf)
{
	struct rxm_tx_buf *first_tx_buf;

	assert(ofi_tx_cq_flags(tx_buf->pkt.hdr.op) & FI_SEND);
	switch (rxm_sar_get_seg_type(&tx_buf->pkt.ctrl_hdr)) {
	case RXM_SAR_SEG_FIRST:
		break;
	case RXM_SAR_SEG_MIDDLE:
		rxm_free_tx_buf(rxm_ep, tx_buf);
		break;
	case RXM_SAR_SEG_LAST:
		first_tx_buf = ofi_bufpool_get_ibuf(rxm_ep->tx_pool,
						tx_buf->pkt.ctrl_hdr.msg_id);
		rxm_free_tx_buf(rxm_ep, first_tx_buf);
		rxm_free_tx_buf(rxm_ep, tx_buf);
		return true;
	}

	return false;
}

static void rxm_handle_sar_comp(struct rxm_ep *rxm_ep,
				struct rxm_tx_buf *tx_buf)
{
	void *app_context;
	uint64_t comp_flags, tx_flags;

	app_context = tx_buf->app_context;
	comp_flags = ofi_tx_cq_flags(tx_buf->pkt.hdr.op);
	tx_flags = tx_buf->flags;

	if (!rxm_complete_sar(rxm_ep, tx_buf))
		return;

	rxm_cq_write_tx_comp(rxm_ep, comp_flags, app_context, tx_flags);
	ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_TX);
}

static void rxm_rndv_rx_finish(struct rxm_rx_buf *rx_buf)
{
	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_FINISH);

	if (rx_buf->recv_entry->rndv.tx_buf) {
		ofi_buf_free(rx_buf->recv_entry->rndv.tx_buf);
		rx_buf->recv_entry->rndv.tx_buf = NULL;
	}

	if (!rx_buf->ep->rdm_mr_local)
		rxm_msg_mr_closev(rx_buf->mr,
				  rx_buf->recv_entry->rxm_iov.count);

	rxm_finish_recv(rx_buf, rx_buf->recv_entry->total_len);
}

static void rxm_rndv_tx_finish(struct rxm_ep *rxm_ep,
			       struct rxm_tx_buf *tx_buf)
{
	assert(ofi_tx_cq_flags(tx_buf->pkt.hdr.op) & FI_SEND);

	RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf, RXM_RNDV_FINISH);
	if (!rxm_ep->rdm_mr_local)
		rxm_msg_mr_closev(tx_buf->rma.mr, tx_buf->rma.count);

	rxm_cq_write_tx_comp(rxm_ep, ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
			     tx_buf->app_context, tx_buf->flags);

	if (rxm_ep->rndv_ops == &rxm_rndv_ops_write &&
	    tx_buf->write_rndv.done_buf) {
		ofi_buf_free(tx_buf->write_rndv.done_buf);
		tx_buf->write_rndv.done_buf = NULL;
	}
	ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_TX);
	rxm_free_tx_buf(rxm_ep, tx_buf);
}

static void rxm_rndv_handle_rd_done(struct rxm_ep *rxm_ep,
				    struct rxm_rx_buf *rx_buf)
{
	struct rxm_tx_buf *tx_buf;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "Got ACK for msg_id: 0x%" PRIx64 "\n",
	       rx_buf->pkt.ctrl_hdr.msg_id);

	tx_buf = ofi_bufpool_get_ibuf(rxm_ep->tx_pool,
				      rx_buf->pkt.ctrl_hdr.msg_id);
	assert(tx_buf->pkt.ctrl_hdr.msg_id == rx_buf->pkt.ctrl_hdr.msg_id);

	rxm_free_rx_buf(rx_buf);

	if (tx_buf->hdr.state == RXM_RNDV_READ_DONE_WAIT) {
		rxm_rndv_tx_finish(rxm_ep, tx_buf);
	} else {
		assert(tx_buf->hdr.state == RXM_RNDV_TX);
		RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf, RXM_RNDV_READ_DONE_RECVD);
	}
}

static int rxm_rndv_rx_match(struct dlist_entry *item, const void *arg)
{
	uint64_t msg_id = *((uint64_t *) arg);
	struct rxm_rx_buf *rx_buf;

	rx_buf = container_of(item, struct rxm_rx_buf, rndv_wait_entry);
	return (msg_id == rx_buf->pkt.ctrl_hdr.msg_id);
}

static int rxm_rndv_handle_wr_done(struct rxm_ep *rxm_ep, struct rxm_rx_buf *rx_buf)
{
	struct dlist_entry *rx_buf_entry;
	struct rxm_rx_buf *rndv_rx_buf;
	int ret = 0;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "Got DONE for msg_id: 0x%" PRIx64 "\n",
	       rx_buf->pkt.ctrl_hdr.msg_id);

	rx_buf_entry = dlist_remove_first_match(&rx_buf->ep->rndv_wait_list,
						rxm_rndv_rx_match,
						&rx_buf->pkt.ctrl_hdr.msg_id);
	if (!rx_buf_entry) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"Failed to find rndv wait entry for msg_id: 0x%" PRIx64 "\n",
			rx_buf->pkt.ctrl_hdr.msg_id);
		ret = -FI_EINVAL;
		goto out;
	}
	rndv_rx_buf = container_of(rx_buf_entry, struct rxm_rx_buf,
				   rndv_wait_entry);

	if (rndv_rx_buf->hdr.state == RXM_RNDV_WRITE_DONE_WAIT) {
		rxm_rndv_rx_finish(rndv_rx_buf);
	} else {
		assert(rndv_rx_buf->hdr.state == RXM_RNDV_WRITE_DATA_SENT);
		RXM_UPDATE_STATE(FI_LOG_CQ, rndv_rx_buf, RXM_RNDV_WRITE_DONE_RECVD);
	}
out:
	rxm_free_rx_buf(rx_buf);
	return ret;
}

static int rxm_rx_buf_match_msg_id(struct dlist_entry *item, const void *arg)
{
	uint64_t msg_id = *((uint64_t *) arg);
	struct rxm_rx_buf *rx_buf;

	rx_buf = container_of(item, struct rxm_rx_buf, unexp_msg.entry);
	return (msg_id == rx_buf->pkt.ctrl_hdr.msg_id);
}

static void rxm_process_seg_data(struct rxm_rx_buf *rx_buf, int *done)
{
	enum fi_hmem_iface iface;
	uint64_t device;
	ssize_t done_len;

	iface = rxm_iov_desc_to_hmem_iface_dev(rx_buf->recv_entry->rxm_iov.iov,
					       rx_buf->recv_entry->rxm_iov.desc,
					       rx_buf->recv_entry->rxm_iov.count,
					       &device);

	done_len = ofi_copy_to_hmem_iov(iface, device,
					rx_buf->recv_entry->rxm_iov.iov,
					rx_buf->recv_entry->rxm_iov.count,
					rx_buf->recv_entry->sar.total_recv_len,
					rx_buf->pkt.data,
					rx_buf->pkt.ctrl_hdr.seg_size);
	assert(done_len == rx_buf->pkt.ctrl_hdr.seg_size);

	rx_buf->recv_entry->sar.total_recv_len += done_len;

	if ((rxm_sar_get_seg_type(&rx_buf->pkt.ctrl_hdr) == RXM_SAR_SEG_LAST) ||
	    (done_len != rx_buf->pkt.ctrl_hdr.seg_size)) {

		dlist_remove(&rx_buf->recv_entry->sar.entry);

		/* Mark rxm_recv_entry::msg_id as unknown for futher re-use */
		rx_buf->recv_entry->sar.msg_id = RXM_SAR_RX_INIT;

		done_len = rx_buf->recv_entry->sar.total_recv_len;
		rx_buf->recv_entry->sar.total_recv_len = 0;

		*done = 1;
		rxm_finish_recv(rx_buf, done_len);
	} else {
		if (rx_buf->recv_entry->sar.msg_id == RXM_SAR_RX_INIT) {
			if (!rx_buf->conn) {
				rx_buf->conn = ofi_idm_at(&rx_buf->ep->conn_idx_map,
						(int) rx_buf->pkt.ctrl_hdr.conn_id);
			}

			rx_buf->recv_entry->sar.conn = rx_buf->conn;
			rx_buf->recv_entry->sar.msg_id = rx_buf->pkt.ctrl_hdr.msg_id;

			dlist_insert_tail(&rx_buf->recv_entry->sar.entry,
					  &rx_buf->conn->deferred_sar_msgs);
		}

		/* The RX buffer can be reposted for further re-use */
		rx_buf->recv_entry = NULL;
		rxm_free_rx_buf(rx_buf);

		*done = 0;
	}
}

static void rxm_handle_seg_data(struct rxm_rx_buf *rx_buf)
{
	struct rxm_recv_entry *recv_entry;
	struct rxm_conn *conn;
	uint64_t msg_id;
	struct dlist_entry *entry;
	int done;

	rxm_process_seg_data(rx_buf, &done);
	if (done || !(rx_buf->ep->rxm_info->mode & FI_BUFFERED_RECV))
		return;

	recv_entry = rx_buf->recv_entry;
	conn = rx_buf->conn;
	msg_id = rx_buf->pkt.ctrl_hdr.msg_id;

	dlist_foreach_container_safe(&conn->deferred_sar_segments,
				     struct rxm_rx_buf, rx_buf,
				     unexp_msg.entry, entry) {
		if (!rxm_rx_buf_match_msg_id(&rx_buf->unexp_msg.entry, &msg_id))
			continue;

		dlist_remove(&rx_buf->unexp_msg.entry);
		rx_buf->recv_entry = recv_entry;
		rxm_process_seg_data(rx_buf, &done);
		if (done)
			break;
	}
}

static ssize_t rxm_rndv_xfer(struct rxm_ep *rxm_ep, struct fid_ep *msg_ep,
			     struct rxm_rndv_hdr *remote_hdr, struct iovec *local_iov,
			     void **local_desc, size_t local_count, size_t total_len,
			     void *context)
{
	size_t i, index = 0, offset = 0, count, copy_len;
	struct iovec iov[RXM_IOV_LIMIT];
	void *desc[RXM_IOV_LIMIT];
	ssize_t ret = FI_SUCCESS;

	for (i = 0; i < remote_hdr->count && total_len > 0; i++) {
		copy_len = MIN(remote_hdr->iov[i].len, total_len);

		ret = ofi_copy_iov_desc(&iov[0], &desc[0], &count,
					&local_iov[0],
					&local_desc[0],
					local_count,
					&index, &offset, copy_len);
		if (ret)
			return ret;
		total_len -= copy_len;
		ret = rxm_ep->rndv_ops->xfer(msg_ep, iov, desc, count, 0,
			       remote_hdr->iov[i].addr, remote_hdr->iov[i].key,
			       context);

		if (ret) {
			if (ret == -FI_EAGAIN) {
				struct rxm_deferred_tx_entry *def_tx_entry;

				ret = rxm_ep->rndv_ops->defer_xfer(
					&def_tx_entry, i, iov, desc, count,
					context);

				if (ret)
					break;
				rxm_queue_deferred_tx(def_tx_entry, OFI_LIST_TAIL);
				continue;
			}
			break;
		}
	}
	assert(!total_len);
	return ret;
}

ssize_t rxm_rndv_read(struct rxm_rx_buf *rx_buf)
{
	ssize_t ret;
	size_t total_len;

	total_len = MIN(rx_buf->recv_entry->total_len, rx_buf->pkt.hdr.size);
	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_READ);

	ret = rxm_rndv_xfer(rx_buf->ep, rx_buf->conn->msg_ep,
			    rx_buf->remote_rndv_hdr,
			    rx_buf->recv_entry->rxm_iov.iov,
			    rx_buf->recv_entry->rxm_iov.desc,
			    rx_buf->recv_entry->rxm_iov.count, total_len,
			    rx_buf);
	if (ret) {
		rxm_cq_write_error(rx_buf->ep->util_ep.rx_cq,
				   rx_buf->ep->util_ep.cntrs[CNTR_RX], rx_buf, (int) ret);
	}
	return ret;
}

static ssize_t rxm_rndv_handle_wr_data(struct rxm_rx_buf *rx_buf)
{
	int i;
	ssize_t ret;
	struct rxm_tx_buf *tx_buf;
	size_t total_len, rma_len = 0;
	struct rxm_rndv_hdr *rx_hdr = (struct rxm_rndv_hdr *) rx_buf->pkt.data;

	tx_buf = ofi_bufpool_get_ibuf(rx_buf->ep->tx_pool,
				      rx_buf->pkt.ctrl_hdr.msg_id);
	total_len = tx_buf->pkt.hdr.size;

	tx_buf->write_rndv.remote_hdr.count = rx_hdr->count;
	memcpy(tx_buf->write_rndv.remote_hdr.iov, rx_hdr->iov,
	       rx_hdr->count * sizeof(rx_hdr->iov[0]));
	// calculate number of RMA writes required to complete the transfer.
	// there me be less than iov count RMA writes required,
	// depending on differences between remote and local IOV sizes.
	for (i = 0; i < tx_buf->write_rndv.remote_hdr.count; i++) {
		if (total_len > rma_len) {
			tx_buf->write_rndv.rndv_rma_count++;
			rma_len += tx_buf->write_rndv.remote_hdr.iov[i].len;
		}
	}

	/* BUG: This is forcing a state change without knowing what state
	 * we're currently in.  This loses whether we processed the completion
	 * for the original send request.  Valid states here are
	 * RXM_RNDV_TX or RXM_RNDV_WRITE_DATA_WAIT.
	 */
	RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf, RXM_RNDV_WRITE);

	ret = rxm_rndv_xfer(rx_buf->ep, tx_buf->write_rndv.conn->msg_ep, rx_hdr,
			    tx_buf->write_rndv.iov, tx_buf->write_rndv.desc,
			    tx_buf->rma.count, total_len, tx_buf);

	if (ret)
		rxm_cq_write_error(rx_buf->ep->util_ep.rx_cq,
				   rx_buf->ep->util_ep.cntrs[CNTR_RX],
				   tx_buf, (int) ret);
	rxm_free_rx_buf(rx_buf);
	return ret;
}

static ssize_t rxm_handle_rndv(struct rxm_rx_buf *rx_buf)
{
	int ret = 0, i;
	size_t total_recv_len;

	rxm_replace_rx_buf(rx_buf);

	if (!rx_buf->conn) {
		assert(rx_buf->ep->msg_srx);
		rx_buf->conn = ofi_idm_at(&rx_buf->ep->conn_idx_map,
					  (int) rx_buf->pkt.ctrl_hdr.conn_id);
		if (!rx_buf->conn)
			return -FI_EOTHER;
	}
	assert(rx_buf->conn);

	FI_DBG(&rxm_prov, FI_LOG_CQ,
	       "Got incoming rndv req with msg_id: 0x%" PRIx64 "\n",
	       rx_buf->pkt.ctrl_hdr.msg_id);

	rx_buf->remote_rndv_hdr = (struct rxm_rndv_hdr *) rx_buf->pkt.data;
	rx_buf->rndv_rma_index = 0;

	if (!rx_buf->ep->rdm_mr_local) {
		total_recv_len = MIN(rx_buf->recv_entry->total_len,
				     rx_buf->pkt.hdr.size);
		ret = rxm_msg_mr_regv(rx_buf->ep, rx_buf->recv_entry->rxm_iov.iov,
				      rx_buf->recv_entry->rxm_iov.count,
				      total_recv_len,
				      rx_buf->ep->rndv_ops->rx_mr_access,
				      rx_buf->mr);
		if (ret)
			return ret;

		for (i = 0; (i < rx_buf->recv_entry->rxm_iov.count &&
			     rx_buf->mr[i]); i++) {
			rx_buf->recv_entry->rxm_iov.desc[i] =
						fi_mr_desc(rx_buf->mr[i]);
		}
	} else {
		struct rxm_mr *mr;

		for (i = 0; i < rx_buf->recv_entry->rxm_iov.count; i++) {
			mr = rx_buf->recv_entry->rxm_iov.desc[i];
			rx_buf->recv_entry->rxm_iov.desc[i] =
				fi_mr_desc(mr->msg_mr);
			rx_buf->mr[i] = mr->msg_mr;
		}
	}

	assert(rx_buf->remote_rndv_hdr->count &&
	       (rx_buf->remote_rndv_hdr->count <= RXM_IOV_LIMIT));

	return rx_buf->ep->rndv_ops->handle_rx(rx_buf);
}

void rxm_handle_eager(struct rxm_rx_buf *rx_buf)
{
	ssize_t done_len = rxm_copy_to_hmem_iov(
		rx_buf->recv_entry->rxm_iov.desc, rx_buf->data,
		rx_buf->pkt.hdr.size, rx_buf->recv_entry->rxm_iov.iov,
		rx_buf->recv_entry->rxm_iov.count, 0);

	assert((size_t) done_len == rx_buf->pkt.hdr.size);

	rxm_finish_recv(rx_buf, done_len);
}

void rxm_handle_coll_eager(struct rxm_rx_buf *rx_buf)
{
	enum fi_hmem_iface iface;
	uint64_t device;
	ssize_t done_len;

	iface = rxm_iov_desc_to_hmem_iface_dev(rx_buf->recv_entry->rxm_iov.iov,
					       rx_buf->recv_entry->rxm_iov.desc,
					       rx_buf->recv_entry->rxm_iov.count,
					       &device);

	done_len = ofi_copy_to_hmem_iov(iface, device,
					rx_buf->recv_entry->rxm_iov.iov,
					rx_buf->recv_entry->rxm_iov.count, 0,
					rx_buf->data, rx_buf->pkt.hdr.size);
	assert((size_t) done_len == rx_buf->pkt.hdr.size);

	if (rx_buf->ep->util_coll_peer_xfer_ops &&
	    rx_buf->pkt.hdr.tag & RXM_PEER_XFER_TAG_FLAG) {
		struct fi_cq_tagged_entry cqe = {
			.tag = rx_buf->pkt.hdr.tag,
			.op_context = rx_buf->recv_entry->context,
		};
		rx_buf->ep->util_coll_peer_xfer_ops->
			complete(rx_buf->ep->util_coll_ep, &cqe, 0);
		rxm_recv_entry_release(rx_buf->recv_entry);
		rxm_free_rx_buf(rx_buf);
	} else {
		rxm_finish_recv(rx_buf, done_len);
	}
}

ssize_t rxm_handle_rx_buf(struct rxm_rx_buf *rx_buf)
{
	switch (rx_buf->pkt.ctrl_hdr.type) {
	case rxm_ctrl_eager:
		rx_buf->ep->eager_ops->handle_rx(rx_buf);
		return 0;
	case rxm_ctrl_rndv_req:
		return rxm_handle_rndv(rx_buf);
	case rxm_ctrl_seg:
		rxm_handle_seg_data(rx_buf);
		return 0;
	default:
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unknown message type\n");
		assert(0);
		return -FI_EINVAL;
	}
}

static void rxm_adjust_multi_recv(struct rxm_rx_buf *rx_buf)
{
	struct rxm_recv_entry *recv_entry;
	struct iovec new_iov;
	size_t recv_size;

	recv_size = rx_buf->pkt.hdr.size;

	if (rx_buf->recv_entry->rxm_iov.iov[0].iov_len < recv_size ||
	    rx_buf->recv_entry->rxm_iov.iov[0].iov_len - recv_size <
	    rx_buf->ep->min_multi_recv_size)
		return;

	new_iov.iov_base = (uint8_t *)
		rx_buf->recv_entry->rxm_iov.iov[0].iov_base + recv_size;
	new_iov.iov_len = rx_buf->recv_entry->rxm_iov.iov[0].iov_len - recv_size;;

	rx_buf->recv_entry->rxm_iov.iov[0].iov_len = recv_size;

	recv_entry = rxm_multi_recv_entry_get(rx_buf->ep, &new_iov,
					rx_buf->recv_entry->rxm_iov.desc, 1,
					rx_buf->recv_entry->addr,
					rx_buf->recv_entry->tag,
					rx_buf->recv_entry->ignore,
					rx_buf->recv_entry->context,
					rx_buf->recv_entry->flags);

	rx_buf->recv_entry->flags &= ~FI_MULTI_RECV;

	dlist_insert_head(&recv_entry->entry, &rx_buf->ep->recv_queue.recv_list);
}

static ssize_t
rxm_match_rx_buf(struct rxm_rx_buf *rx_buf,
		 struct rxm_recv_queue *recv_queue,
		 struct rxm_recv_match_attr *match_attr)
{
	struct dlist_entry *entry;

	entry = dlist_remove_first_match(&recv_queue->recv_list,
					 recv_queue->match_recv, match_attr);
	if (entry) {
		rx_buf->recv_entry = container_of(entry, struct rxm_recv_entry, entry);

		if (rx_buf->recv_entry->flags & FI_MULTI_RECV)
			rxm_adjust_multi_recv(rx_buf);

		return rxm_handle_rx_buf(rx_buf);
	}

	RXM_DBG_ADDR_TAG(FI_LOG_CQ, "No matching recv found for incoming msg",
			 match_attr->addr, match_attr->tag);
	FI_DBG(&rxm_prov, FI_LOG_CQ, "Enqueueing msg to unexpected msg queue\n");
	rx_buf->unexp_msg.addr = match_attr->addr;
	rx_buf->unexp_msg.tag = match_attr->tag;

	dlist_insert_tail(&rx_buf->unexp_msg.entry,
			  &recv_queue->unexp_msg_list);
	rxm_replace_rx_buf(rx_buf);
	return 0;
}

static ssize_t rxm_handle_recv_comp(struct rxm_rx_buf *rx_buf)
{
	struct rxm_recv_match_attr match_attr = {
		.addr = FI_ADDR_UNSPEC,
	};

	if (rx_buf->ep->rxm_info->caps & (FI_SOURCE | FI_DIRECTED_RECV)) {
		if (rx_buf->ep->msg_srx)
			rx_buf->conn = ofi_idm_at(&rx_buf->ep->conn_idx_map,
					(int) rx_buf->pkt.ctrl_hdr.conn_id);
		if (!rx_buf->conn)
			return -FI_EOTHER;
		match_attr.addr = rx_buf->conn->peer->fi_addr;
	}

	if (rx_buf->ep->rxm_info->mode & FI_BUFFERED_RECV) {
		rxm_finish_buf_recv(rx_buf);
		return 0;
	}

	switch(rx_buf->pkt.hdr.op) {
	case ofi_op_msg:
		FI_DBG(&rxm_prov, FI_LOG_CQ, "Got MSG op\n");
		return rxm_match_rx_buf(rx_buf, &rx_buf->ep->recv_queue,
					&match_attr);
	case ofi_op_tagged:
		FI_DBG(&rxm_prov, FI_LOG_CQ, "Got TAGGED op\n");
		match_attr.tag = rx_buf->pkt.hdr.tag;
		return rxm_match_rx_buf(rx_buf, &rx_buf->ep->trecv_queue,
					&match_attr);
	default:
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unknown op!\n");
		assert(0);
		return -FI_EINVAL;
	}
}

static int rxm_sar_match_msg_id(struct dlist_entry *item, const void *arg)
{
	uint64_t msg_id = *((uint64_t *) arg);
	struct rxm_recv_entry *recv_entry;

	recv_entry = container_of(item, struct rxm_recv_entry, sar.entry);
	return (msg_id == recv_entry->sar.msg_id);
}

static ssize_t rxm_sar_handle_segment(struct rxm_rx_buf *rx_buf)
{
	struct dlist_entry *sar_entry;

	rx_buf->conn = ofi_idm_at(&rx_buf->ep->conn_idx_map,
				  (int) rx_buf->pkt.ctrl_hdr.conn_id);
	if (!rx_buf->conn)
		return -FI_EOTHER;

	FI_DBG(&rxm_prov, FI_LOG_CQ,
	       "Got incoming recv with msg_id: 0x%" PRIx64 " for conn - %p\n",
	       rx_buf->pkt.ctrl_hdr.msg_id, rx_buf->conn);
	sar_entry = dlist_find_first_match(&rx_buf->conn->deferred_sar_msgs,
					   rxm_sar_match_msg_id,
					   &rx_buf->pkt.ctrl_hdr.msg_id);
	if (!sar_entry)
		return rxm_handle_recv_comp(rx_buf);

	rx_buf->recv_entry = container_of(sar_entry, struct rxm_recv_entry,
					  sar.entry);
	rxm_handle_seg_data(rx_buf);
	return 0;
}

static void rxm_rndv_send_rd_done(struct rxm_rx_buf *rx_buf)
{
	struct rxm_deferred_tx_entry *def_entry;
	struct rxm_tx_buf *buf;
	ssize_t ret;

	assert(rx_buf->conn);
	assert(rx_buf->hdr.state == RXM_RNDV_READ);
	buf = ofi_buf_alloc(rx_buf->ep->tx_pool);
	if (!buf) {
		ret = -FI_ENOMEM;
		goto err;
	}

	rx_buf->recv_entry->rndv.tx_buf = buf;

	buf->pkt.ctrl_hdr.type = rxm_ctrl_rndv_rd_done;
	buf->pkt.ctrl_hdr.conn_id = rx_buf->conn->remote_index;
	buf->pkt.ctrl_hdr.msg_id = rx_buf->pkt.ctrl_hdr.msg_id;

	ret = fi_send(rx_buf->conn->msg_ep, &buf->pkt, sizeof(buf->pkt),
		      buf->hdr.desc, 0, rx_buf);
	if (ret) {
		if (ret == -FI_EAGAIN) {
			def_entry = rxm_ep_alloc_deferred_tx_entry(rx_buf->ep,
						rx_buf->conn,
						RXM_DEFERRED_TX_RNDV_ACK);
			if (def_entry) {
				def_entry->rndv_ack.rx_buf = rx_buf;
				def_entry->rndv_ack.pkt_size = sizeof(rx_buf->pkt);
				rxm_queue_deferred_tx(def_entry, OFI_LIST_TAIL);
				return;
			}
		}
		goto free;
	}

	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_READ_DONE_SENT);
	return;

free:
	ofi_buf_free(buf);
	rx_buf->recv_entry->rndv.tx_buf = NULL;
err:
	FI_WARN(&rxm_prov, FI_LOG_CQ,
		"unable to allocate/send rd rndv ack: %s\n",
		fi_strerror((int) ret));
	assert(0);
	/* TODO: Allocate all resources needed on receiving
	 * original message receive request, to avoid allocation failures.
	 * On other failures, we need to fail the receive.
	 */
}

static void
rxm_rndv_send_wr_done(struct rxm_ep *rxm_ep, struct rxm_tx_buf *tx_buf)
{
	struct rxm_deferred_tx_entry *def_entry;
	struct rxm_tx_buf *buf;
	ssize_t ret;

	assert(tx_buf->hdr.state == RXM_RNDV_WRITE);
	buf = ofi_buf_alloc(rxm_ep->tx_pool);
	if (!buf) {
		ret = -FI_ENOMEM;
		goto err;
	}

	tx_buf->write_rndv.done_buf = buf;

	buf->pkt.ctrl_hdr.type = rxm_ctrl_rndv_wr_done;
	buf->pkt.ctrl_hdr.conn_id = tx_buf->pkt.ctrl_hdr.conn_id;
	buf->pkt.ctrl_hdr.msg_id = tx_buf->pkt.ctrl_hdr.msg_id;

	ret = fi_send(tx_buf->write_rndv.conn->msg_ep, &buf->pkt,
		      sizeof(buf->pkt), buf->hdr.desc, 0, tx_buf);
	if (ret) {
		if (ret == -FI_EAGAIN) {
			def_entry = rxm_ep_alloc_deferred_tx_entry(rxm_ep,
						tx_buf->write_rndv.conn,
						RXM_DEFERRED_TX_RNDV_DONE);
			if (def_entry) {
				def_entry->rndv_done.tx_buf = tx_buf;
				rxm_queue_deferred_tx(def_entry, OFI_LIST_TAIL);
				return;
			}
		}
		goto free;
	}

	RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf, RXM_RNDV_WRITE_DONE_SENT);
	return;

free:
	ofi_buf_free(buf);
	tx_buf->write_rndv.done_buf = NULL;
err:
	FI_WARN(&rxm_prov, FI_LOG_CQ,
		"unable to allocate/send wr rndv ack: %s\n",
		fi_strerror((int) ret));
	assert(0);
	/* TODO: Allocate all resources needed prior to initiating the
	 * original message send request, to avoid allocation failures.
	 * On other failures, we need to fail the original message.
	 */
}

ssize_t rxm_rndv_send_wr_data(struct rxm_rx_buf *rx_buf)
{
	struct rxm_deferred_tx_entry *def_entry;
	struct rxm_tx_buf *buf;
	ssize_t ret;

	assert(rx_buf->conn);

	buf = ofi_buf_alloc(rx_buf->ep->tx_pool);
	if (!buf) {
		ret = -FI_ENOMEM;
		goto err;
	}

	rx_buf->recv_entry->rndv.tx_buf = buf;

	buf->pkt.ctrl_hdr.type = rxm_ctrl_rndv_wr_data;
	buf->pkt.ctrl_hdr.conn_id = rx_buf->conn->remote_index;
	buf->pkt.ctrl_hdr.msg_id = rx_buf->pkt.ctrl_hdr.msg_id;
	rxm_rndv_hdr_init(rx_buf->ep, buf->pkt.data,
			  rx_buf->recv_entry->rxm_iov.iov,
			  rx_buf->recv_entry->rxm_iov.count, rx_buf->mr);

	ret = fi_send(rx_buf->conn->msg_ep, &buf->pkt, sizeof(buf->pkt) +
		      sizeof(struct rxm_rndv_hdr), buf->hdr.desc, 0, rx_buf);
	if (ret) {
		if (ret == -FI_EAGAIN) {
			def_entry = rxm_ep_alloc_deferred_tx_entry(rx_buf->ep,
						rx_buf->conn,
						RXM_DEFERRED_TX_RNDV_ACK);
			if (def_entry) {
				def_entry->rndv_ack.rx_buf = rx_buf;
				def_entry->rndv_ack.pkt_size =
						sizeof(buf->pkt) +
						sizeof(struct rxm_rndv_hdr);
				rxm_queue_deferred_tx(def_entry, OFI_LIST_TAIL);
				return 0;
			}
		}
		goto free;
	}
	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_WRITE_DATA_SENT);
	return 0;

free:
	ofi_buf_free(buf);
	rx_buf->recv_entry->rndv.tx_buf = NULL;
err:
	FI_WARN(&rxm_prov, FI_LOG_CQ,
		"unable to allocate/send wr rndv ready: %s\n",
		fi_strerror((int) ret));
	assert(0);
	/* TODO: Sender will be blocked forever waiting for a response
	 * that will not come.  Need to tear down communication.
	 */
	return 0;
}

static void rxm_handle_remote_write(struct rxm_ep *rxm_ep,
				   struct fi_cq_data_entry *comp)
{
	rxm_cq_write(rxm_ep->util_ep.rx_cq, NULL, comp->flags, comp->len, NULL,
		     comp->data, 0);
	ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_REM_WR);
	if (comp->op_context)
		rxm_free_rx_buf(comp->op_context);
}

static void rxm_format_atomic_resp_pkt_hdr(struct rxm_conn *rxm_conn,
					   struct rxm_tx_buf *tx_buf,
					   size_t data_len, uint8_t pkt_op,
					   enum fi_datatype datatype,
					   uint8_t atomic_op)
{
	rxm_ep_format_tx_buf_pkt(rxm_conn, data_len, pkt_op, 0, 0, 0,
				 &tx_buf->pkt);
	tx_buf->pkt.ctrl_hdr.type = rxm_ctrl_atomic_resp;
	tx_buf->pkt.hdr.op = pkt_op;
	tx_buf->pkt.hdr.atomic.datatype = datatype;
	tx_buf->pkt.hdr.atomic.op = atomic_op;
	tx_buf->pkt.hdr.atomic.ioc_count = 0;
}

static ssize_t rxm_atomic_send_resp(struct rxm_ep *rxm_ep,
				    struct rxm_rx_buf *rx_buf,
				    struct rxm_tx_buf *resp_buf,
				    ssize_t result_len, uint32_t status)
{
	struct rxm_deferred_tx_entry *def_tx_entry;
	struct rxm_atomic_resp_hdr *atomic_hdr;
	ssize_t ret;
	size_t data_len, tot_len;

	data_len = result_len + sizeof(struct rxm_atomic_resp_hdr);
	tot_len = data_len + sizeof(struct rxm_pkt);

	resp_buf->hdr.state = RXM_ATOMIC_RESP_SENT;
	rxm_format_atomic_resp_pkt_hdr(rx_buf->conn, resp_buf, data_len,
				       rx_buf->pkt.hdr.op,
				       rx_buf->pkt.hdr.atomic.datatype,
				       rx_buf->pkt.hdr.atomic.op);
	resp_buf->pkt.ctrl_hdr.conn_id = rx_buf->conn->remote_index;
	resp_buf->pkt.ctrl_hdr.msg_id = rx_buf->pkt.ctrl_hdr.msg_id;
	atomic_hdr = (struct rxm_atomic_resp_hdr *) resp_buf->pkt.data;
	atomic_hdr->status = htonl(status);
	atomic_hdr->result_len = htonl((uint32_t) result_len);

	if (tot_len < rxm_ep->inject_limit) {
		ret = fi_inject(rx_buf->conn->msg_ep, &resp_buf->pkt,
				tot_len, 0);
		if (!ret)
			ofi_buf_free(resp_buf);
	} else {
		ret = rxm_atomic_send_respmsg(rxm_ep, rx_buf->conn, resp_buf,
					      tot_len);
	}
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"Unable to send Atomic Response\n");
		if (ret == -FI_EAGAIN) {
			def_tx_entry = rxm_ep_alloc_deferred_tx_entry(rxm_ep,
						rx_buf->conn,
						RXM_DEFERRED_TX_ATOMIC_RESP);
			if (!def_tx_entry) {
				FI_WARN(&rxm_prov, FI_LOG_CQ,
					"Unable to allocate deferred Atomic "
					"Response\n");
				return -FI_ENOMEM;
			}

			def_tx_entry->atomic_resp.tx_buf = resp_buf;
			def_tx_entry->atomic_resp.len = tot_len;
			rxm_queue_deferred_tx(def_tx_entry, OFI_LIST_TAIL);
			ret = 0;
		}
	}
	rxm_free_rx_buf(rx_buf);

	return ret;
}

static void rxm_do_atomic(uint8_t op, void *dst, void *src, void *cmp,
			  void *res, size_t count, enum fi_datatype datatype,
			  enum fi_op amo_op)
{
	switch (op) {
	case ofi_op_atomic:
		assert(ofi_atomic_iswrite_op(amo_op));
		ofi_atomic_write_handler(amo_op, datatype, dst, src, count);
		break;
	case ofi_op_atomic_fetch:
		assert(ofi_atomic_isreadwrite_op(amo_op));
		ofi_atomic_readwrite_handler(amo_op, datatype, dst, src, res,
					     count);
		break;
	case ofi_op_atomic_compare:
		assert(ofi_atomic_isswap_op(amo_op));
		ofi_atomic_swap_handler(amo_op, datatype, dst, src, cmp, res,
					count);
		break;
	default:
		/* Validated prior to calling function */
		break;
	}
}

static int rxm_do_device_mem_atomic(struct rxm_mr *dev_mr, uint8_t op,
				    void *dev_dst, void *src, void *cmp,
				    void *res, size_t amo_count,
				    enum fi_datatype datatype,
				    enum fi_op amo_op, size_t amo_op_size)
{
	struct rxm_domain *dom = dev_mr->domain;
	void *tx_buf;
	ssize_t ret __attribute__((unused));
	struct iovec iov = {
		.iov_base = dev_dst,
		.iov_len = amo_op_size,
	};

	ofi_mutex_lock(&dom->amo_bufpool_lock);
	tx_buf = ofi_buf_alloc(dom->amo_bufpool);
	ofi_mutex_unlock(&dom->amo_bufpool_lock);

	if (!tx_buf)
		return -FI_ENOMEM;

	ofi_mutex_lock(&dev_mr->amo_lock);
	ret = ofi_copy_from_hmem_iov(tx_buf, amo_op_size, dev_mr->iface, 0,
				    &iov, 1, 0);
	assert((size_t) ret == amo_op_size);

	rxm_do_atomic(op, tx_buf, src, cmp, res, amo_count, datatype,
		      amo_op);

	ret = ofi_copy_to_hmem_iov(dev_mr->iface, 0, &iov, 1, 0, tx_buf,
				   amo_op_size);
	assert((size_t) ret == amo_op_size);

	ofi_mutex_unlock(&dev_mr->amo_lock);

	ofi_mutex_lock(&dom->amo_bufpool_lock);
	ofi_buf_free(tx_buf);
	ofi_mutex_unlock(&dom->amo_bufpool_lock);

	return FI_SUCCESS;
}

static ssize_t rxm_handle_atomic_req(struct rxm_ep *rxm_ep,
				     struct rxm_rx_buf *rx_buf)
{
	struct rxm_atomic_hdr *req_hdr =
			(struct rxm_atomic_hdr *) rx_buf->pkt.data;
	enum fi_datatype datatype = rx_buf->pkt.hdr.atomic.datatype;
	enum fi_op atomic_op = rx_buf->pkt.hdr.atomic.op;
	size_t datatype_sz = ofi_datatype_size(datatype);
	size_t len;
	ssize_t result_len;
	uint64_t offset;
	int i;
	ssize_t ret = 0;
	struct rxm_tx_buf *resp_buf;
	struct rxm_atomic_resp_hdr *resp_hdr;
	struct rxm_domain *domain = container_of(rxm_ep->util_ep.domain,
					 struct rxm_domain, util_domain);
	uint8_t op = rx_buf->pkt.hdr.op;

	assert(!(rx_buf->comp_flags &
		 ~(FI_RECV | FI_RECV | FI_REMOTE_CQ_DATA)));
	assert(op == ofi_op_atomic || op == ofi_op_atomic_fetch ||
	       op == ofi_op_atomic_compare);

	if (rx_buf->ep->msg_srx)
		rx_buf->conn = ofi_idm_at(&rx_buf->ep->conn_idx_map,
					  (int) rx_buf->pkt.ctrl_hdr.conn_id);
	if (!rx_buf->conn)
		return -FI_EOTHER;

	resp_buf = ofi_buf_alloc(rxm_ep->tx_pool);
	if (!resp_buf) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Unable to allocate for atomic response\n");
		return -FI_ENOMEM;
	}

	resp_buf->pkt.ctrl_hdr.type = rxm_ctrl_atomic;
	for (i = 0; i < rx_buf->pkt.hdr.atomic.ioc_count; i++) {
		ret = ofi_mr_verify(&domain->util_domain.mr_map,
				    req_hdr->rma_ioc[i].count * datatype_sz,
				    (uintptr_t *)&req_hdr->rma_ioc[i].addr,
				    req_hdr->rma_ioc[i].key,
				    ofi_rx_mr_reg_flags(rx_buf->pkt.hdr.op,
							atomic_op));
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"Atomic RMA MR verify error %ld\n", ret);
			return rxm_atomic_send_resp(rxm_ep, rx_buf, resp_buf, 0,
						    (uint32_t) -FI_EACCES);
		}
	}

	len = ofi_total_rma_ioc_cnt(req_hdr->rma_ioc,
			rx_buf->pkt.hdr.atomic.ioc_count) * datatype_sz;
	resp_hdr = (struct rxm_atomic_resp_hdr *) resp_buf->pkt.data;

	for (i = 0, offset = 0; i < rx_buf->pkt.hdr.atomic.ioc_count; i++) {
		struct rxm_mr *mr =
			rxm_mr_get_map_entry(domain, req_hdr->rma_ioc[i].key);
		size_t amo_count = req_hdr->rma_ioc[i].count;
		size_t amo_op_size = amo_count * datatype_sz;
		void *src_buf = req_hdr->data + offset;
		void *cmp_buf = req_hdr->data + len + offset;
		void *res_buf = resp_hdr->data + offset;
		void *dst_buf = (void *) req_hdr->rma_ioc[i].addr;

		if (mr->iface != FI_HMEM_SYSTEM) {
			ret = rxm_do_device_mem_atomic(mr, op, dst_buf, src_buf,
						       cmp_buf, res_buf,
						       amo_count, datatype,
						       atomic_op, amo_op_size);
			if (ret) {
				FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
					"Atomic operation failed %ld\n", ret);

				return rxm_atomic_send_resp(rxm_ep, rx_buf,
							    resp_buf, 0,
							    (uint32_t) ret);
			}
		} else {
			rxm_do_atomic(op, dst_buf, src_buf, cmp_buf, res_buf,
				      amo_count, datatype, atomic_op);
		}

		offset += amo_op_size;
	}
	result_len = op == ofi_op_atomic ? 0 : offset;

	if (op == ofi_op_atomic)
		ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_REM_WR);
	else
		ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_REM_RD);

	return rxm_atomic_send_resp(rxm_ep, rx_buf, resp_buf,
				    result_len, FI_SUCCESS);
}

static ssize_t rxm_handle_atomic_resp(struct rxm_ep *rxm_ep,
				      struct rxm_rx_buf *rx_buf)
{
	struct rxm_tx_buf *tx_buf;
	struct rxm_atomic_resp_hdr *resp_hdr;
	struct util_cntr *cntr = NULL;
	uint64_t len;
	ssize_t copy_len;
	ssize_t ret = 0;
	enum fi_hmem_iface iface;
	uint64_t device;

	resp_hdr = (struct rxm_atomic_resp_hdr *) rx_buf->pkt.data;
	tx_buf = ofi_bufpool_get_ibuf(rxm_ep->tx_pool,
				      rx_buf->pkt.ctrl_hdr.msg_id);
	FI_DBG(&rxm_prov, FI_LOG_CQ, "received atomic response: op: %" PRIu8
	       " msg_id: 0x%" PRIx64 "\n", rx_buf->pkt.hdr.op,
	       rx_buf->pkt.ctrl_hdr.msg_id);

	iface = rxm_iov_desc_to_hmem_iface_dev(tx_buf->atomic_result.iov,
					       tx_buf->atomic_result.desc,
					       tx_buf->atomic_result.count,
					       &device);

	assert(!(rx_buf->comp_flags & ~(FI_RECV | FI_REMOTE_CQ_DATA)));

	if (resp_hdr->status) {
		ret = ntohl(resp_hdr->status);
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"bad atomic response status %d\n",
			ntohl(resp_hdr->status));
		goto write_err;
	}

	len = ofi_total_iov_len(tx_buf->atomic_result.iov,
				tx_buf->atomic_result.count);
	if (ntohl(resp_hdr->result_len) != len) {
		ret = -FI_EIO;
		FI_WARN(&rxm_prov, FI_LOG_CQ, "result size mismatch\n");
		goto write_err;
	}

	copy_len = ofi_copy_to_hmem_iov(iface, device, tx_buf->atomic_result.iov,
				   tx_buf->atomic_result.count, 0, resp_hdr->data,
				   len);
	if ((size_t) copy_len != len) {
		ret = -FI_EIO;
		FI_WARN(&rxm_prov, FI_LOG_CQ, "copy length error\n");
		goto write_err;
	}

	if (!(tx_buf->flags & FI_INJECT))
		rxm_cq_write_tx_comp(rxm_ep, ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
				     tx_buf->app_context, tx_buf->flags);

	if (tx_buf->pkt.hdr.op == ofi_op_atomic) {
		ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_WR);
	} else if (tx_buf->pkt.hdr.op == ofi_op_atomic_compare ||
		   tx_buf->pkt.hdr.op == ofi_op_atomic_fetch) {
		ofi_ep_cntr_inc(&rxm_ep->util_ep, CNTR_RD);
	} else {
		ret = -FI_EOPNOTSUPP;
		goto write_err;
	}
free:
	rxm_free_rx_buf(rx_buf);
	rxm_free_tx_buf(rxm_ep, tx_buf);
	return ret;

write_err:
	if (tx_buf->pkt.hdr.op == ofi_op_atomic) {
		cntr = rxm_ep->util_ep.cntrs[CNTR_WR];
	} else if (tx_buf->pkt.hdr.op == ofi_op_atomic_compare ||
		   tx_buf->pkt.hdr.op == ofi_op_atomic_fetch) {
		cntr = rxm_ep->util_ep.cntrs[CNTR_RD];
	} else {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"unknown atomic request op!\n");
		assert(0);
	}
	rxm_cq_write_error(rxm_ep->util_ep.tx_cq, cntr,
			   tx_buf->app_context, (int) ret);
	goto free;
}

static ssize_t rxm_handle_credit(struct rxm_ep *rxm_ep, struct rxm_rx_buf *rx_buf)
{
	struct rxm_domain *domain;

	assert(rx_buf->rx_ep->fid.fclass == FI_CLASS_EP);
	domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
			      util_domain);
	domain->flow_ctrl_ops->add_credits(rx_buf->rx_ep,
					   rx_buf->pkt.ctrl_hdr.ctrl_data);
	rxm_free_rx_buf(rx_buf);
	return FI_SUCCESS;
}

void rxm_finish_coll_eager_send(struct rxm_ep *rxm_ep,
			        struct rxm_tx_buf *tx_eager_buf)
{
	if (rxm_ep->util_coll_ep &&
	    (tx_eager_buf->pkt.hdr.tag & RXM_PEER_XFER_TAG_FLAG)) {
		struct fi_cq_tagged_entry cqe = {
			.tag = tx_eager_buf->pkt.hdr.tag,
			.op_context = tx_eager_buf->app_context,
		};
		rxm_ep->util_coll_peer_xfer_ops->
			complete(rxm_ep->util_coll_ep, &cqe, 0);
	} else {
		rxm_finish_eager_send(rxm_ep, tx_eager_buf);
	}
}

ssize_t rxm_handle_comp(struct rxm_ep *rxm_ep, struct fi_cq_data_entry *comp)
{
	struct rxm_rx_buf *rx_buf;
	struct rxm_tx_buf *tx_buf;

	/* Remote write events may not consume a posted recv so op context
	 * and hence state would be NULL */
	if (comp->flags & FI_REMOTE_WRITE) {
		rxm_handle_remote_write(rxm_ep, (struct fi_cq_data_entry *) comp);
		return 0;
	}

	switch (RXM_GET_PROTO_STATE(comp->op_context)) {
	case RXM_TX:
	case RXM_INJECT_TX:
		tx_buf = comp->op_context;
		rxm_ep->eager_ops->comp_tx(rxm_ep, tx_buf);
		rxm_free_tx_buf(rxm_ep, tx_buf);
		return 0;
	case RXM_CREDIT_TX:
		tx_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		ofi_buf_free(tx_buf);
		return 0;
	case RXM_RMA:
		tx_buf = comp->op_context;
		assert((comp->flags & (FI_WRITE | FI_RMA)) ||
		       (comp->flags & (FI_READ | FI_RMA)));
		rxm_finish_rma(rxm_ep, tx_buf, comp->flags);
		return 0;
	case RXM_RX:
		rx_buf = comp->op_context;
		assert(!(comp->flags & FI_REMOTE_READ));
		assert((rx_buf->pkt.hdr.version == OFI_OP_VERSION) &&
		       (rx_buf->pkt.ctrl_hdr.version == RXM_CTRL_VERSION));

		switch (rx_buf->pkt.ctrl_hdr.type) {
		case rxm_ctrl_eager:
		case rxm_ctrl_rndv_req:
			return rxm_handle_recv_comp(rx_buf);
		case rxm_ctrl_rndv_rd_done:
			rxm_rndv_handle_rd_done(rxm_ep, rx_buf);
			return 0;
		case rxm_ctrl_rndv_wr_done:
			return rxm_rndv_handle_wr_done(rxm_ep, rx_buf);
		case rxm_ctrl_rndv_wr_data:
			return rxm_rndv_handle_wr_data(rx_buf);
		case rxm_ctrl_seg:
			return rxm_sar_handle_segment(rx_buf);
		case rxm_ctrl_atomic:
			return rxm_handle_atomic_req(rxm_ep, rx_buf);
		case rxm_ctrl_atomic_resp:
			return rxm_handle_atomic_resp(rxm_ep, rx_buf);
		case rxm_ctrl_credit:
			return rxm_handle_credit(rxm_ep, rx_buf);
		default:
			FI_WARN(&rxm_prov, FI_LOG_CQ, "Unknown message type\n");
			assert(0);
			return -FI_EINVAL;
		}
	case RXM_SAR_TX:
		tx_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		rxm_handle_sar_comp(rxm_ep, tx_buf);
		return 0;
	case RXM_RNDV_TX:
		tx_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		if (rxm_ep->rndv_ops == &rxm_rndv_ops_write)
			RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf,
					 RXM_RNDV_WRITE_DATA_WAIT);
		else
			RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf,
					 RXM_RNDV_READ_DONE_WAIT);
		return 0;
	case RXM_RNDV_READ_DONE_WAIT:
	case RXM_RNDV_WRITE_DATA_WAIT:
		assert(0);
		return 0;
	case RXM_RNDV_READ:
		rx_buf = comp->op_context;
		assert(comp->flags & FI_READ);
		if (++rx_buf->rndv_rma_index < rx_buf->remote_rndv_hdr->count)
			return 0;

		rxm_rndv_send_rd_done(rx_buf);
		return 0;
	case RXM_RNDV_WRITE:
		tx_buf = comp->op_context;
		assert(comp->flags & FI_WRITE);
		if (++tx_buf->write_rndv.rndv_rma_index <
		    tx_buf->write_rndv.rndv_rma_count)
			return 0;

		rxm_rndv_send_wr_done(rxm_ep, tx_buf);
		return 0;
	case RXM_RNDV_READ_DONE_SENT:
		assert(comp->flags & FI_SEND);
		rxm_rndv_rx_finish(comp->op_context);
		return 0;
	case RXM_RNDV_WRITE_DATA_SENT:
		rx_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		dlist_insert_tail(&rx_buf->rndv_wait_entry, &rx_buf->ep->rndv_wait_list);
		RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_WRITE_DONE_WAIT);
		return 0;
	case RXM_RNDV_WRITE_DONE_SENT:
	case RXM_RNDV_READ_DONE_RECVD:
		assert(comp->flags & FI_SEND || comp->flags & FI_WRITE);
		rxm_rndv_tx_finish(rxm_ep, comp->op_context);
		return 0;
	case RXM_RNDV_WRITE_DONE_RECVD:
		assert(comp->flags & FI_SEND);
		rxm_rndv_rx_finish(comp->op_context);
		return 0;
	case RXM_RNDV_FINISH:
		assert(0);
		return 0;
	case RXM_ATOMIC_RESP_WAIT:
		/* BUG: need to wait for completion, even if a response has
		 * been received.
		 */
		assert(comp->flags & FI_SEND);
		return 0;
	case RXM_ATOMIC_RESP_SENT:
		tx_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		ofi_buf_free(tx_buf);	/* BUG: should have consumed tx credit */
		return 0;
	default:
		assert(0);
		return 0;
	}
}

void rxm_cq_write_error(struct util_cq *cq, struct util_cntr *cntr,
			void *op_context, int err)
{
	struct fi_cq_err_entry err_entry = {0};
	err_entry.op_context = op_context;
	err_entry.prov_errno = err;
	err_entry.err = -err;

	if (cntr)
		rxm_cntr_incerr(cntr);

	if (ofi_cq_write_error(cq, &err_entry)) {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unable to ofi_cq_write_error\n");
		assert(0);
	}
}

void rxm_cq_write_error_all(struct rxm_ep *rxm_ep, int err)
{
	struct fi_cq_err_entry err_entry = {0};
	ssize_t ret = 0;

	err_entry.prov_errno = err;
	err_entry.err = -err;
	if (rxm_ep->util_ep.tx_cq) {
		ret = ofi_cq_write_error(rxm_ep->util_ep.tx_cq, &err_entry);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_CQ,
				"Unable to ofi_cq_write_error\n");
			assert(0);
		}
	}
	if (rxm_ep->util_ep.rx_cq) {
		ret = ofi_cq_write_error(rxm_ep->util_ep.rx_cq, &err_entry);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_CQ,
				"Unable to ofi_cq_write_error\n");
			assert(0);
		}
	}
	if (rxm_ep->util_ep.cntrs[CNTR_TX])
		rxm_cntr_incerr(rxm_ep->util_ep.cntrs[CNTR_TX]);

	if (rxm_ep->util_ep.cntrs[CNTR_RX])
		rxm_cntr_incerr(rxm_ep->util_ep.cntrs[CNTR_RX]);

	if (rxm_ep->util_ep.cntrs[CNTR_WR])
		rxm_cntr_incerr(rxm_ep->util_ep.cntrs[CNTR_WR]);

	if (rxm_ep->util_ep.cntrs[CNTR_RD])
		rxm_cntr_incerr(rxm_ep->util_ep.cntrs[CNTR_RD]);
}

void rxm_handle_comp_error(struct rxm_ep *rxm_ep)
{
	struct rxm_tx_buf *tx_buf;
	struct rxm_rx_buf *rx_buf;
	struct util_cq *cq;
	struct util_cntr *cntr;
	struct fi_cq_err_entry err_entry = {0};
	ssize_t ret;

	ret = fi_cq_readerr(rxm_ep->msg_cq, &err_entry, 0);
	if ((ret) < 0) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"unable to fi_cq_readerr on msg cq\n");
		rxm_cq_write_error_all(rxm_ep, (int) ret);
		return;
	}

	if (err_entry.err != FI_ECANCELED) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"fi_cq_readerr: err: %s (%d), prov_err: %s (%d)\n",
			fi_strerror(err_entry.err), err_entry.err,
			fi_cq_strerror(rxm_ep->msg_cq, err_entry.prov_errno,
					err_entry.err_data, NULL, 0),
			err_entry.prov_errno);
	}

	cq = rxm_ep->util_ep.tx_cq;
	cntr = rxm_ep->util_ep.cntrs[CNTR_TX];

	switch (RXM_GET_PROTO_STATE(err_entry.op_context)) {
	case RXM_TX:
	case RXM_RNDV_TX:
	case RXM_RNDV_WRITE_DONE_SENT:
	case RXM_ATOMIC_RESP_WAIT:
		tx_buf = err_entry.op_context;
		err_entry.op_context = tx_buf->app_context;
		err_entry.flags = ofi_tx_cq_flags(tx_buf->pkt.hdr.op);
		rxm_free_tx_buf(rxm_ep, tx_buf);
		break;
	case RXM_RNDV_READ_DONE_RECVD:
		/* We received the response, so ignore the send error */
		rxm_rndv_tx_finish(rxm_ep, err_entry.op_context);
		return;
	case RXM_RNDV_WRITE_DONE_RECVD:
		/* We received the response, so ignore the send error */
		rxm_rndv_rx_finish(err_entry.op_context);
		return;
	case RXM_INJECT_TX:
		rxm_free_tx_buf(rxm_ep, err_entry.op_context);
		if (cntr)
			rxm_cntr_incerr(cntr);
		return;
	case RXM_CREDIT_TX:
	case RXM_ATOMIC_RESP_SENT: /* BUG: should have consumed tx credit */
		tx_buf = err_entry.op_context;
		ofi_buf_free(tx_buf);
		return;
	case RXM_RMA:
		tx_buf = err_entry.op_context;
		err_entry.op_context = tx_buf->app_context;
		/* err_entry.flags pass through from msg ep */
		if (!(tx_buf->flags & FI_INJECT) && !rxm_ep->rdm_mr_local &&
		    rxm_ep->msg_mr_local) {
			rxm_msg_mr_closev(tx_buf->rma.mr, tx_buf->rma.count);
		}
		rxm_free_tx_buf(rxm_ep, tx_buf);
		break;
	case RXM_SAR_TX:
		tx_buf = err_entry.op_context;
		err_entry.op_context = tx_buf->app_context;
		err_entry.flags = ofi_tx_cq_flags(tx_buf->pkt.hdr.op);
		if (!rxm_complete_sar(rxm_ep, tx_buf))
			return;
		break;
	case RXM_RNDV_WRITE:
		tx_buf = err_entry.op_context;
		err_entry.op_context = tx_buf->app_context;
		err_entry.flags = ofi_tx_cq_flags(tx_buf->pkt.hdr.op);
		break;

	/* Incoming application data error */
	case RXM_RX:
		/* Silently drop MSG CQ error entries for internal receive
		 * operations not associated with an application posted
		 * receive. This situation can happen when the MSG EP
		 * receives a reject / shutdown and CM thread hasn't handled
		 * the event yet.
		 */
		rx_buf = (struct rxm_rx_buf *) err_entry.op_context;
		if (!rx_buf->recv_entry) {
			ofi_buf_free((struct rxm_rx_buf *)err_entry.op_context);
			return;
		}
		/* fall through */
	case RXM_RNDV_READ_DONE_SENT:
	case RXM_RNDV_WRITE_DATA_SENT: /* BUG: should fail initial send */
	case RXM_RNDV_READ:
		rx_buf = (struct rxm_rx_buf *) err_entry.op_context;
		assert(rx_buf->recv_entry);
		err_entry.op_context = rx_buf->recv_entry->context;
		err_entry.flags = rx_buf->recv_entry->comp_flags;

		cq = rx_buf->ep->util_ep.rx_cq;
		cntr = rx_buf->ep->util_ep.cntrs[CNTR_RX];
		break;
	default:
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Invalid state!\nmsg cq error info: %s\n",
			fi_cq_strerror(rxm_ep->msg_cq, err_entry.prov_errno,
				       err_entry.err_data, NULL, 0));
		rxm_cq_write_error_all(rxm_ep, -FI_EOPBADSTATE);
		return;
	}

	if (cntr)
		rxm_cntr_incerr(cntr);

	assert(cq);
	ret = ofi_cq_write_error(cq, &err_entry);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unable to ofi_cq_write_error\n");
		assert(0);
	}
}

ssize_t rxm_thru_comp(struct rxm_ep *ep, struct fi_cq_data_entry *comp)
{
	struct util_cq *cq;
	int ret;

	cq = (comp->flags & (FI_RECV | FI_REMOTE_WRITE | FI_REMOTE_READ)) ?
	     ep->util_ep.rx_cq : ep->util_ep.tx_cq;

	ret = ofi_cq_write(cq, comp->op_context, comp->flags, comp->len,
			   comp->buf, comp->data, 0);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unable to report completion\n");
		assert(0);
	}
	if (cq->wait)
		cq->wait->signal(cq->wait);

	return ret;
}

void rxm_thru_comp_error(struct rxm_ep *ep)
{
	struct util_cq *cq;
	struct fi_cq_err_entry err_entry = {0};
	ssize_t ret;

	ret = fi_cq_readerr(ep->msg_cq, &err_entry, 0);
	if (ret < 0) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"unable to fi_cq_readerr on msg cq\n");
		rxm_cq_write_error_all(ep, (int) ret);
		return;
	}

	cq = (err_entry.flags & FI_RECV) ? ep->util_ep.rx_cq : ep->util_ep.tx_cq;
	ret = ofi_cq_write_error(cq, &err_entry);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unable to ofi_cq_write_error\n");
		assert(0);
	}
}

/*
 * Right now the peer CQ is only used for collectives. There are two cases:
 * (1) collective operations initiated by application and completed by
 *     util_coll or offload_coll, should write the completion to the real
 *     CQ;
 * (2) collective operations initiated by offload_coll and completed by
 *     util_coll (with FI_PEER_TRANSFER flag), should call the callback
 *     set by offload_coll.
 */
ssize_t rxm_cq_owner_write(struct fid_peer_cq *peer_cq, void *context,
			   uint64_t flags, size_t len, void *buf, uint64_t data,
			   uint64_t tag, fi_addr_t src)
{
	struct rxm_coll_buf *req = context;
	struct rxm_ep *rxm_ep = req->ep;
	struct rxm_cq *rxm_cq;

	if (req->flags & FI_PEER_TRANSFER) {
		struct fi_cq_tagged_entry entry = {
			.op_context = req->app_context,
			.flags = req->flags,
			.len = len,
			.buf = buf,
			.data = data,
			.tag = tag,
		};
		return rxm_ep->offload_coll_peer_xfer_ops->
				complete(rxm_ep->offload_coll_ep, &entry, src);
	}

	rxm_cq = container_of(peer_cq, struct rxm_cq, peer_cq);
	return ofi_cq_write(&rxm_cq->util_cq, req->app_context, req->flags, len,
			    buf, data, tag);
}

ssize_t rxm_cq_owner_writeerr(struct fid_peer_cq *peer_cq,
			      const struct fi_cq_err_entry *err_entry)
{
	struct rxm_coll_buf *req = err_entry->op_context;
	struct rxm_ep *rxm_ep = req->ep;
	struct rxm_cq *rxm_cq;
	struct fi_cq_err_entry cqe_err = *err_entry;

	cqe_err.op_context = req->app_context;
	cqe_err.flags = req->flags;

	if (req->flags & FI_PEER_TRANSFER) {
		return rxm_ep->offload_coll_peer_xfer_ops->
				comperr(rxm_ep->offload_coll_ep, &cqe_err);
	}

	rxm_cq = container_of(peer_cq, struct rxm_cq, peer_cq);
	return ofi_cq_write_error(&rxm_cq->util_cq, &cqe_err);
}

int rxm_post_recv(struct rxm_rx_buf *rx_buf)
{
	struct rxm_domain *domain;
	int ret;

	if (rx_buf->ep->msg_srx)
		rx_buf->conn = NULL;
	rx_buf->hdr.state = RXM_RX;
	rx_buf->recv_entry = NULL;

	domain = container_of(rx_buf->ep->util_ep.domain,
			      struct rxm_domain, util_domain);
	ret = (int) fi_recv(rx_buf->rx_ep, &rx_buf->pkt,
			    domain->rx_post_size, rx_buf->hdr.desc,
			    FI_ADDR_UNSPEC, rx_buf);
	if (!ret)
		return 0;

	if (ret != -FI_EAGAIN) {
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
		       "unable to post recv buf: %d\n", ret);
	}
	return ret;
}

int rxm_prepost_recv(struct rxm_ep *ep, struct fid_ep *rx_ep)
{
	struct rxm_rx_buf *rx_buf;
	int ret;
	size_t i;

	for (i = 0; i < ep->msg_info->rx_attr->size; i++) {
		rx_buf = rxm_rx_buf_alloc(ep, rx_ep);
		if (!rx_buf)
			return -FI_ENOMEM;

		ret = rxm_post_recv(rx_buf);
		if (ret) {
			ofi_buf_free(&rx_buf->hdr);
			return ret;
		}
	}
	return 0;
}

void rxm_ep_do_progress(struct util_ep *util_ep)
{
	struct rxm_ep *rxm_ep = container_of(util_ep, struct rxm_ep, util_ep);
	struct fi_cq_data_entry comp[32];
	struct dlist_entry *conn_entry_tmp;
	struct rxm_conn *rxm_conn;
	size_t comp_read = 0;
	uint64_t timestamp;
	ssize_t ret, i, err;

	do {
		ret = fi_cq_read(rxm_ep->msg_cq, &comp, 32);
		if (ret > 0) {
			comp_read += ret;
			for (i = 0; i < ret; i++) {
				err = rxm_ep->handle_comp(rxm_ep, &comp[i]);
				if (err) {
					// We don't have enough info to write a good
					// error entry to the CQ at this point
					rxm_cq_write_error_all(rxm_ep, (int) err);
				}
			}
		} else if (ret < 0 && (ret != -FI_EAGAIN)) {
			if (ret == -FI_EAVAIL)
				rxm_ep->handle_comp_error(rxm_ep);
			else
				rxm_cq_write_error_all(rxm_ep, (int) ret);
		}

		if (ret == -FI_EAGAIN || rxm_ep->connecting_cnt ||
		    --rxm_ep->cq_eq_fairness <= 0) {
			rxm_ep->cq_eq_fairness = rxm_cq_eq_fairness;
			if (rxm_ep->connecting_cnt == 0 &&
			    rxm_cm_progress_interval) {
				timestamp = ofi_gettime_us();
				if (timestamp - rxm_ep->msg_cq_last_poll >
				    rxm_cm_progress_interval) {
					rxm_ep->msg_cq_last_poll = timestamp;
					rxm_conn_progress(rxm_ep);
				}
			} else {
					rxm_conn_progress(rxm_ep);
			}
		}
	} while ((ret > 0) && (comp_read < rxm_ep->comp_per_progress));

	if (!dlist_empty(&rxm_ep->deferred_queue)) {
		dlist_foreach_container_safe(&rxm_ep->deferred_queue,
					     struct rxm_conn, rxm_conn,
					     deferred_entry, conn_entry_tmp) {
			rxm_ep_progress_deferred_queue(rxm_ep, rxm_conn);
		}
	}
}

void rxm_ep_progress(struct util_ep *util_ep)
{
	ofi_genlock_lock(&util_ep->lock);
	rxm_ep_do_progress(util_ep);
	ofi_genlock_unlock(&util_ep->lock);
}

void rxm_ep_progress_coll(struct util_ep *util_ep)
{
	struct rxm_ep *rxm_ep = container_of(util_ep, struct rxm_ep, util_ep);
	struct util_ep *coll_ep;

	rxm_ep_progress(util_ep);

	if (rxm_ep->util_coll_ep) {
		coll_ep = container_of(rxm_ep->util_coll_ep, struct util_ep,
				       ep_fid);
		coll_ep->progress(coll_ep);
	}

	if (rxm_ep->offload_coll_ep) {
		coll_ep = container_of(rxm_ep->offload_coll_ep, struct util_ep,
				       ep_fid);
		coll_ep->progress(coll_ep);
	}
}

static int rxm_cq_close(struct fid *fid)
{
	struct rxm_cq *rxm_cq;
	int ret, retv = 0;

	rxm_cq = container_of(fid, struct rxm_cq, util_cq.cq_fid.fid);

	if (rxm_cq->offload_coll_cq)
		fi_close(&rxm_cq->offload_coll_cq->fid);

	if (rxm_cq->util_coll_cq)
		fi_close(&rxm_cq->util_coll_cq->fid);

	ret = ofi_cq_cleanup(&rxm_cq->util_cq);
	if (ret)
		retv = ret;

	free(rxm_cq);
	return retv;
}

static struct fi_ops rxm_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_cq_close,
	.bind = fi_no_bind,
	.control = ofi_cq_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops rxm_peer_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = fi_no_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cq rxm_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = ofi_cq_sread,
	.sreadfrom = ofi_cq_sreadfrom,
	.signal = ofi_cq_signal,
	.strerror = rxm_cq_strerror,
};

static struct fi_ops_cq_owner rxm_cq_owner_ops = {
	.size = sizeof(struct fi_ops_cq_owner),
	.write = rxm_cq_owner_write,
	.writeerr = rxm_cq_owner_writeerr,
};

int rxm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	struct rxm_cq *rxm_cq;
	struct rxm_domain *rxm_domain;
	struct fi_cq_attr peer_cq_attr = {
		.flags = FI_PEER,
	};
	struct fi_peer_cq_context peer_cq_context = {
		.size = sizeof(struct fi_peer_cq_context),
	};
	int ret;

	rxm_cq = calloc(1, sizeof(*rxm_cq));
	if (!rxm_cq)
		return -FI_ENOMEM;

	ret = ofi_cq_init(&rxm_prov, domain, attr, &rxm_cq->util_cq,
			  &ofi_cq_progress, context);
	if (ret)
		goto err1;

	rxm_domain = container_of(domain, struct rxm_domain,
				  util_domain.domain_fid);

	rxm_cq->peer_cq.fid.fclass = FI_CLASS_PEER_CQ;
	rxm_cq->peer_cq.fid.ops = &rxm_peer_cq_fi_ops;
	rxm_cq->peer_cq.owner_ops = &rxm_cq_owner_ops;
	peer_cq_context.cq = &rxm_cq->peer_cq;

	if (rxm_domain->util_coll_domain) {
		ret = fi_cq_open(rxm_domain->util_coll_domain, &peer_cq_attr,
				 &rxm_cq->util_coll_cq, &peer_cq_context);
		if (ret)
			goto err2;
	}

	if (rxm_domain->offload_coll_domain) {
		ret = fi_cq_open(rxm_domain->offload_coll_domain, &peer_cq_attr,
				 &rxm_cq->offload_coll_cq, &peer_cq_context);
		if (ret)
			goto err2;
	}

	*cq_fid = &rxm_cq->util_cq.cq_fid;
	/* Override util_cq_fi_ops */
	(*cq_fid)->fid.ops = &rxm_cq_fi_ops;
	(*cq_fid)->ops = &rxm_cq_ops;
	return 0;

err2:
	if (rxm_cq->util_coll_cq)
		fi_close(&rxm_cq->util_coll_cq->fid);
	ofi_cq_cleanup(&rxm_cq->util_cq);
err1:
	free(rxm_cq);
	return ret;
}
