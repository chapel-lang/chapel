/*
 * Copyright (c) 2013-2016 Intel Corporation. All rights reserved.
 * Copyright (c) 2018 Cray Inc. All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
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
#include <ofi_coll.h>

#include "rxm.h"

size_t rxm_cm_progress_interval;
size_t rxm_cq_eq_fairness;

static const char *rxm_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
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

static inline uint64_t
rxm_cq_get_rx_comp_and_op_flags(struct rxm_rx_buf *rx_buf)
{
	return (rx_buf->pkt.hdr.flags | ofi_rx_flags[rx_buf->pkt.hdr.op]);
}

static int rxm_finish_buf_recv(struct rxm_rx_buf *rx_buf)
{
	uint64_t flags;
	char *data;

	if (rx_buf->pkt.ctrl_hdr.type == rxm_ctrl_seg &&
	    rxm_sar_get_seg_type(&rx_buf->pkt.ctrl_hdr) != RXM_SAR_SEG_FIRST) {
		dlist_insert_tail(&rx_buf->unexp_msg.entry,
				  &rx_buf->conn->sar_deferred_rx_msg_list);
		rx_buf = rxm_rx_buf_alloc(rx_buf->ep, rx_buf->msg_ep, 1);
		if (OFI_UNLIKELY(!rx_buf)) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"ran out of buffers from RX buffer pool\n");
			return -FI_ENOMEM;
		}
		dlist_insert_tail(&rx_buf->repost_entry,
				  &rx_buf->ep->repost_ready_list);

		return 0;
	}

	flags = rxm_cq_get_rx_comp_and_op_flags(rx_buf);

	if (rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_eager)
		flags |= FI_MORE;

	if (rx_buf->pkt.ctrl_hdr.type == rxm_ctrl_rndv)
		data = rxm_pkt_rndv_data(&rx_buf->pkt);
	else
		data = rx_buf->pkt.data;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "writing buffered recv completion: "
	       "length: %" PRIu64 "\n", rx_buf->pkt.hdr.size);
	rx_buf->recv_context.ep = &rx_buf->ep->util_ep.ep_fid;

	return rxm_cq_write_recv_comp(rx_buf, &rx_buf->recv_context, flags,
				      rx_buf->pkt.hdr.size, data);
}

static int rxm_cq_write_error_trunc(struct rxm_rx_buf *rx_buf, size_t done_len)
{
	int ret;

	if (rx_buf->ep->util_ep.flags & OFI_CNTR_ENABLED)
		rxm_cntr_incerr(rx_buf->ep->util_ep.rx_cntr);

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
	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"Unable to write recv error CQ\n");
		return ret;
	}
	return 0;
}

static int rxm_finish_recv(struct rxm_rx_buf *rx_buf, size_t done_len)
{
	struct rxm_recv_entry *recv_entry = rx_buf->recv_entry;
	size_t recv_size;
	int ret = FI_SUCCESS;

	if (done_len < rx_buf->pkt.hdr.size) {
		ret = rxm_cq_write_error_trunc(rx_buf, done_len);
		goto release;
	}

	if (rx_buf->recv_entry->flags & FI_COMPLETION ||
	    rx_buf->ep->rxm_info->mode & FI_BUFFERED_RECV) {
		ret = rxm_cq_write_recv_comp(rx_buf, rx_buf->recv_entry->context,
				rx_buf->recv_entry->comp_flags |
				rx_buf->pkt.hdr.flags,
				rx_buf->pkt.hdr.size,
				rx_buf->recv_entry->rxm_iov.iov[0].iov_base);
		if (ret)
			goto release;
	}
	ofi_ep_rx_cntr_inc(&rx_buf->ep->util_ep);

	if (rx_buf->recv_entry->flags & FI_MULTI_RECV) {
		recv_size = rx_buf->pkt.hdr.size;

		recv_entry->total_len -= recv_size;

		if (recv_entry->total_len < rx_buf->ep->min_multi_recv_size) {
			ret = ofi_cq_write(rx_buf->ep->util_ep.rx_cq, recv_entry->context,
					   FI_MULTI_RECV, 0, NULL, 0, 0);
			goto release;
		}

		recv_entry->rxm_iov.iov[0].iov_base = (uint8_t *)
				recv_entry->rxm_iov.iov[0].iov_base + recv_size;
		recv_entry->rxm_iov.iov[0].iov_len -= recv_size;

		dlist_insert_head(&recv_entry->entry,
				  &recv_entry->recv_queue->recv_list);
		goto free_buf;
	}

release:
	rxm_recv_entry_release(recv_entry->recv_queue, recv_entry);
free_buf:
	rxm_rx_buf_free(rx_buf);
	if (ret)
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Error writing CQ entry\n");
	return ret;
}

static inline int
rxm_cq_tx_comp_write(struct rxm_ep *rxm_ep, uint64_t comp_flags,
		     void *app_context,  uint64_t flags)
{
	if (flags & FI_COMPLETION) {
		int ret = ofi_cq_write(rxm_ep->util_ep.tx_cq, app_context,
				       comp_flags, 0, NULL, 0, 0);
		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxm_prov, FI_LOG_CQ,
				"Unable to report completion\n");
			return ret;
		}
		rxm_cq_log_comp(comp_flags);
	}
	return 0;
}

static inline int rxm_finish_rma(struct rxm_ep *rxm_ep, struct rxm_rma_buf *rma_buf,
				 uint64_t comp_flags)
{
	int ret = rxm_cq_tx_comp_write(rxm_ep, comp_flags,
				       rma_buf->app_context, rma_buf->flags);

	assert(((comp_flags & FI_WRITE) && !(comp_flags & FI_READ)) ||
	       ((comp_flags & FI_READ) && !(comp_flags & FI_WRITE)));

	if (comp_flags & FI_WRITE)
		ofi_ep_wr_cntr_inc(&rxm_ep->util_ep);
	else
		ofi_ep_rd_cntr_inc(&rxm_ep->util_ep);

	if (!(rma_buf->flags & FI_INJECT) && !rxm_ep->rdm_mr_local &&
	    rxm_ep->msg_mr_local) {
		rxm_msg_mr_closev(rma_buf->mr.mr, rma_buf->mr.count);
	}

	ofi_buf_free(rma_buf);
	return ret;
}

int rxm_finish_eager_send(struct rxm_ep *rxm_ep, struct rxm_tx_eager_buf *tx_buf)
{
	int ret = rxm_cq_tx_comp_write(rxm_ep, ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
				       tx_buf->app_context, tx_buf->flags);

	assert(ofi_tx_cq_flags(tx_buf->pkt.hdr.op) & FI_SEND);
	ofi_ep_tx_cntr_inc(&rxm_ep->util_ep);

	return ret;
}

static int rxm_finish_sar_segment_send(struct rxm_ep *rxm_ep,
				       struct rxm_tx_sar_buf *tx_buf, bool err)
{
	struct rxm_tx_sar_buf *first_tx_buf;
	int ret = FI_SUCCESS;

	switch (rxm_sar_get_seg_type(&tx_buf->pkt.ctrl_hdr)) {
	case RXM_SAR_SEG_FIRST:
		break;
	case RXM_SAR_SEG_MIDDLE:
		ofi_buf_free(tx_buf);
		break;
	case RXM_SAR_SEG_LAST:
		if (!err) {
			ret = rxm_cq_tx_comp_write(rxm_ep,
					ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
					tx_buf->app_context, tx_buf->flags);

			assert(ofi_tx_cq_flags(tx_buf->pkt.hdr.op) & FI_SEND);
			ofi_ep_tx_cntr_inc(&rxm_ep->util_ep);
		}
		first_tx_buf = ofi_bufpool_get_ibuf(rxm_ep->
					buf_pools[RXM_BUF_POOL_TX_SAR].pool,
					tx_buf->pkt.ctrl_hdr.msg_id);
		ofi_buf_free(first_tx_buf);
		ofi_buf_free(tx_buf);
		break;
	}

	return ret;
}

static inline int rxm_finish_send_rndv_ack(struct rxm_rx_buf *rx_buf)
{
	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_FINISH);

	if (rx_buf->recv_entry->rndv.tx_buf) {
		ofi_buf_free(rx_buf->recv_entry->rndv.tx_buf);
		rx_buf->recv_entry->rndv.tx_buf = NULL;
	}

	if (!rx_buf->ep->rdm_mr_local)
		rxm_msg_mr_closev(rx_buf->mr, rx_buf->recv_entry->rxm_iov.count);

	return rxm_finish_recv(rx_buf, rx_buf->recv_entry->total_len);
}

static int rxm_rndv_tx_finish(struct rxm_ep *rxm_ep, struct rxm_tx_rndv_buf *tx_buf)
{
	int ret;

	RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf, RXM_RNDV_FINISH);

	if (!rxm_ep->rdm_mr_local)
		rxm_msg_mr_closev(tx_buf->mr, tx_buf->count);

	ret = rxm_cq_tx_comp_write(rxm_ep, ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
				   tx_buf->app_context, tx_buf->flags);

	assert(ofi_tx_cq_flags(tx_buf->pkt.hdr.op) & FI_SEND);
	ofi_ep_tx_cntr_inc(&rxm_ep->util_ep);

	ofi_buf_free(tx_buf);

	return ret;
}

static int rxm_rndv_handle_ack(struct rxm_ep *rxm_ep, struct rxm_rx_buf *rx_buf)
{
	struct rxm_tx_rndv_buf *tx_buf;

	tx_buf = ofi_bufpool_get_ibuf(rxm_ep->buf_pools[RXM_BUF_POOL_TX_RNDV].pool,
				      rx_buf->pkt.ctrl_hdr.msg_id);

	FI_DBG(&rxm_prov, FI_LOG_CQ, "Got ACK for msg_id: 0x%" PRIx64 "\n",
	       rx_buf->pkt.ctrl_hdr.msg_id);

	assert(tx_buf->pkt.ctrl_hdr.msg_id == rx_buf->pkt.ctrl_hdr.msg_id);

	rxm_rx_buf_free(rx_buf);

	if (tx_buf->hdr.state == RXM_RNDV_ACK_WAIT) {
		return rxm_rndv_tx_finish(rxm_ep, tx_buf);
	} else {
		assert(tx_buf->hdr.state == RXM_RNDV_TX);
		RXM_UPDATE_STATE(FI_LOG_CQ, tx_buf, RXM_RNDV_ACK_RECVD);
		return 0;
	}
}

static int rxm_rx_buf_match_msg_id(struct dlist_entry *item, const void *arg)
{
	uint64_t msg_id = *((uint64_t *)arg);
	struct rxm_rx_buf *rx_buf =
		container_of(item, struct rxm_rx_buf, unexp_msg.entry);
	return (msg_id == rx_buf->pkt.ctrl_hdr.msg_id);
}

static inline
ssize_t rxm_cq_copy_seg_data(struct rxm_rx_buf *rx_buf, int *done)
{
	uint64_t done_len = ofi_copy_to_iov(rx_buf->recv_entry->rxm_iov.iov,
					    rx_buf->recv_entry->rxm_iov.count,
					    rx_buf->recv_entry->sar.total_recv_len,
					    rx_buf->pkt.data,
					    rx_buf->pkt.ctrl_hdr.seg_size);
	rx_buf->recv_entry->sar.total_recv_len += done_len;

	if ((rxm_sar_get_seg_type(&rx_buf->pkt.ctrl_hdr) == RXM_SAR_SEG_LAST) ||
	    (done_len != rx_buf->pkt.ctrl_hdr.seg_size)) {
		dlist_remove(&rx_buf->recv_entry->sar.entry);

		/* Mark rxm_recv_entry::msg_id as unknown for futher re-use */
		rx_buf->recv_entry->sar.msg_id = RXM_SAR_RX_INIT;

		done_len = rx_buf->recv_entry->sar.total_recv_len;
		rx_buf->recv_entry->sar.total_recv_len = 0;

		*done = 1;
		return rxm_finish_recv(rx_buf, done_len);
	} else {
		if (rx_buf->recv_entry->sar.msg_id == RXM_SAR_RX_INIT) {
			if (!rx_buf->conn) {
				rx_buf->conn = rxm_key2conn(rx_buf->ep,
							    rx_buf->pkt.ctrl_hdr.conn_id);
			}

			rx_buf->recv_entry->sar.conn = rx_buf->conn;
			rx_buf->recv_entry->sar.msg_id = rx_buf->pkt.ctrl_hdr.msg_id;

			dlist_insert_tail(&rx_buf->recv_entry->sar.entry,
					  &rx_buf->conn->sar_rx_msg_list);
		}

		/* The RX buffer can be reposted for further re-use */
		rx_buf->recv_entry = NULL;
		rxm_rx_buf_free(rx_buf);

		*done = 0;
		return FI_SUCCESS;
	}
}

ssize_t rxm_cq_handle_seg_data(struct rxm_rx_buf *rx_buf)
{
	int done;

	if (rx_buf->ep->rxm_info->mode & FI_BUFFERED_RECV) {
		struct rxm_recv_entry *recv_entry = rx_buf->recv_entry;
		struct rxm_conn *conn = rx_buf->conn;
		uint64_t msg_id = rx_buf->pkt.ctrl_hdr.msg_id;
		struct dlist_entry *entry;
		ssize_t ret;

		ret = rxm_cq_copy_seg_data(rx_buf, &done);
		if (done)
			return ret;

		dlist_foreach_container_safe(&conn->sar_deferred_rx_msg_list,
					     struct rxm_rx_buf, rx_buf,
					     unexp_msg.entry, entry) {
			if (!rxm_rx_buf_match_msg_id(&rx_buf->unexp_msg.entry, &msg_id))
				continue;
			dlist_remove(&rx_buf->unexp_msg.entry);
			rx_buf->recv_entry = recv_entry;
			ret = rxm_cq_copy_seg_data(rx_buf, &done);
			if (done)
				break;
		}
		return ret;
	} else {
		return rxm_cq_copy_seg_data(rx_buf, &done);
	}
}

static inline ssize_t
rxm_cq_rndv_read_prepare_deferred(struct rxm_deferred_tx_entry **def_tx_entry, size_t index,
				 struct iovec *iov, void *desc[RXM_IOV_LIMIT],
				 size_t count, struct rxm_rx_buf *rx_buf)
{
	uint8_t i;

	*def_tx_entry = rxm_ep_alloc_deferred_tx_entry(rx_buf->ep, rx_buf->conn,
						       RXM_DEFERRED_TX_RNDV_READ);
	if (OFI_UNLIKELY(!*def_tx_entry))
		return -FI_ENOMEM;

	(*def_tx_entry)->rndv_read.rx_buf = rx_buf;
	(*def_tx_entry)->rndv_read.rma_iov.addr =
			rx_buf->rndv_hdr->iov[index].addr;
	(*def_tx_entry)->rndv_read.rma_iov.key =
			rx_buf->rndv_hdr->iov[index].key;
	for (i = 0; i < count; i++) {
		(*def_tx_entry)->rndv_read.rxm_iov.iov[i] = iov[i];
		(*def_tx_entry)->rndv_read.rxm_iov.desc[i] = desc[i];
	}
	(*def_tx_entry)->rndv_read.rxm_iov.count = count;

	return 0;
}

ssize_t rxm_cq_handle_rndv(struct rxm_rx_buf *rx_buf)
{
	size_t i, index = 0, offset = 0, count, total_recv_len;
	struct iovec iov[RXM_IOV_LIMIT];
	void *desc[RXM_IOV_LIMIT];
	struct rxm_rx_buf *new_rx_buf;
	int ret = 0;

	rx_buf->repost = 0;

	/* En-queue new rx buf to be posted ASAP so that we don't block any
	 * incoming messages. RNDV processing can take a while. */
	new_rx_buf = rxm_rx_buf_alloc(rx_buf->ep, rx_buf->msg_ep, 1);
	if (OFI_UNLIKELY(!new_rx_buf))
		return -FI_ENOMEM;
	dlist_insert_tail(&new_rx_buf->repost_entry,
			  &new_rx_buf->ep->repost_ready_list);

	if (!rx_buf->conn) {
		assert(rx_buf->ep->srx_ctx);
		rx_buf->conn = rxm_key2conn(rx_buf->ep,
					    rx_buf->pkt.ctrl_hdr.conn_id);
		if (OFI_UNLIKELY(!rx_buf->conn))
			return -FI_EOTHER;
	}
	assert(rx_buf->conn);

	FI_DBG(&rxm_prov, FI_LOG_CQ,
	       "Got incoming recv with msg_id: 0x%" PRIx64 "\n",
	       rx_buf->pkt.ctrl_hdr.msg_id);

	rx_buf->rndv_hdr = (struct rxm_rndv_hdr *)rx_buf->pkt.data;
	rx_buf->rndv_rma_index = 0;

	if (!rx_buf->ep->rdm_mr_local) {
		total_recv_len = MIN(rx_buf->recv_entry->total_len,
				     rx_buf->pkt.hdr.size);
		ret = rxm_msg_mr_regv(rx_buf->ep,
				      rx_buf->recv_entry->rxm_iov.iov,
				      rx_buf->recv_entry->rxm_iov.count,
				      total_recv_len, FI_READ, rx_buf->mr);
		if (OFI_UNLIKELY(ret))
			return ret;

		for (i = 0; (i < rx_buf->recv_entry->rxm_iov.count &&
			     rx_buf->mr[i]); i++)
			rx_buf->recv_entry->rxm_iov.desc[i] =
						fi_mr_desc(rx_buf->mr[i]);
	} else {
		for (i = 0; i < rx_buf->recv_entry->rxm_iov.count; i++)
			rx_buf->recv_entry->rxm_iov.desc[i] =
				fi_mr_desc(rx_buf->recv_entry->rxm_iov.desc[i]);
		total_recv_len = MIN(rx_buf->recv_entry->total_len,
				     rx_buf->pkt.hdr.size);
	}

	assert(rx_buf->rndv_hdr->count &&
	       (rx_buf->rndv_hdr->count <= RXM_IOV_LIMIT));

	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_READ);

	for (i = 0; i < rx_buf->rndv_hdr->count; i++) {
		size_t copy_len = MIN(rx_buf->rndv_hdr->iov[i].len,
				      total_recv_len);

		ret = ofi_copy_iov_desc(&iov[0], &desc[0], &count,
					&rx_buf->recv_entry->rxm_iov.iov[0],
					&rx_buf->recv_entry->rxm_iov.desc[0],
					rx_buf->recv_entry->rxm_iov.count,
					&index, &offset, copy_len);
		if (ret) {
			assert(ret == -FI_ETOOSMALL);
			return rxm_cq_write_error_trunc(
				rx_buf, rx_buf->recv_entry->total_len);
		}
		total_recv_len -= copy_len;
		ret = fi_readv(rx_buf->conn->msg_ep, iov, desc, count, 0,
			       rx_buf->rndv_hdr->iov[i].addr,
			       rx_buf->rndv_hdr->iov[i].key, rx_buf);
		if (OFI_UNLIKELY(ret)) {
			if (OFI_LIKELY(ret == -FI_EAGAIN)) {
				struct rxm_deferred_tx_entry *def_tx_entry;

				ret = rxm_cq_rndv_read_prepare_deferred(
						&def_tx_entry, i, iov, desc,
						count, rx_buf);
				if (ret)
					goto readv_err;
				rxm_ep_enqueue_deferred_tx_queue(def_tx_entry);
				continue;
			}
readv_err:
			rxm_cq_write_error(rx_buf->ep->util_ep.rx_cq,
					   rx_buf->ep->util_ep.rx_cntr,
					   rx_buf->recv_entry->context, ret);
			break;
		}
	}
	assert(!total_recv_len);
	return ret;
}

ssize_t rxm_cq_handle_eager(struct rxm_rx_buf *rx_buf)
{
	uint64_t done_len = ofi_copy_to_iov(rx_buf->recv_entry->rxm_iov.iov,
					    rx_buf->recv_entry->rxm_iov.count,
					    0, rx_buf->pkt.data,
					    rx_buf->pkt.hdr.size);
	return rxm_finish_recv(rx_buf, done_len);
}

ssize_t rxm_cq_handle_coll_eager(struct rxm_rx_buf *rx_buf)
{
	uint64_t done_len = ofi_copy_to_iov(rx_buf->recv_entry->rxm_iov.iov,
					    rx_buf->recv_entry->rxm_iov.count,
					    0, rx_buf->pkt.data,
					    rx_buf->pkt.hdr.size);
	if (rx_buf->pkt.hdr.tag & OFI_COLL_TAG_FLAG) {
		ofi_coll_handle_xfer_comp(rx_buf->pkt.hdr.tag,
				rx_buf->recv_entry->context);
		rxm_rx_buf_free(rx_buf);
		rxm_recv_entry_release(rx_buf->recv_entry->recv_queue,
				rx_buf->recv_entry);
		return FI_SUCCESS;
	}
	return rxm_finish_recv(rx_buf, done_len);
}

ssize_t rxm_cq_handle_rx_buf(struct rxm_rx_buf *rx_buf)
{
	switch (rx_buf->pkt.ctrl_hdr.type) {
	case rxm_ctrl_eager:
		return rx_buf->ep->txrx_ops->handle_eager_rx(rx_buf);
	case rxm_ctrl_rndv:
		return rx_buf->ep->txrx_ops->handle_rndv_rx(rx_buf);
	case rxm_ctrl_seg:
		return rx_buf->ep->txrx_ops->handle_seg_data_rx(rx_buf);
	default:
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unknown message type\n");
		assert(0);
		return -FI_EINVAL;
	}
}

static inline ssize_t
rxm_cq_match_rx_buf(struct rxm_rx_buf *rx_buf,
		    struct rxm_recv_queue *recv_queue,
		    struct rxm_recv_match_attr *match_attr)
{
	struct dlist_entry *entry;
	struct rxm_ep *rxm_ep;
	struct fid_ep *msg_ep;

	entry = dlist_remove_first_match(&recv_queue->recv_list,
					 recv_queue->match_recv, match_attr);
	if (!entry) {
		RXM_DBG_ADDR_TAG(FI_LOG_CQ, "No matching recv found for "
				 "incoming msg", match_attr->addr,
				 match_attr->tag);
		FI_DBG(&rxm_prov, FI_LOG_CQ, "Enqueueing msg to unexpected msg"
		       "queue\n");
		rx_buf->unexp_msg.addr = match_attr->addr;
		rx_buf->unexp_msg.tag = match_attr->tag;
		rx_buf->repost = 0;

		dlist_insert_tail(&rx_buf->unexp_msg.entry,
				  &recv_queue->unexp_msg_list);

		msg_ep = rx_buf->msg_ep;
		rxm_ep = rx_buf->ep;

		rx_buf = rxm_rx_buf_alloc(rxm_ep, msg_ep, 1);
		if (OFI_UNLIKELY(!rx_buf)) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"ran out of buffers from RX buffer pool\n");
			return -FI_ENOMEM;
		}

		dlist_insert_tail(&rx_buf->repost_entry,
				  &rxm_ep->repost_ready_list);
		return 0;
	}

	rx_buf->recv_entry = container_of(entry, struct rxm_recv_entry, entry);
	return rxm_cq_handle_rx_buf(rx_buf);
}

static inline ssize_t rxm_handle_recv_comp(struct rxm_rx_buf *rx_buf)
{
	struct rxm_recv_match_attr match_attr = {
		.addr = FI_ADDR_UNSPEC,
	};

	if (rx_buf->ep->rxm_info->caps & (FI_SOURCE | FI_DIRECTED_RECV)) {
		if (rx_buf->ep->srx_ctx)
			rx_buf->conn =
				rxm_key2conn(rx_buf->ep, rx_buf->pkt.ctrl_hdr.conn_id);
		if (OFI_UNLIKELY(!rx_buf->conn))
			return -FI_EOTHER;
		match_attr.addr = rx_buf->conn->handle.fi_addr;
	}

	if (rx_buf->ep->rxm_info->mode & FI_BUFFERED_RECV)
		return rxm_finish_buf_recv(rx_buf);

	switch(rx_buf->pkt.hdr.op) {
	case ofi_op_msg:
		FI_DBG(&rxm_prov, FI_LOG_CQ, "Got MSG op\n");
		return rxm_cq_match_rx_buf(rx_buf, &rx_buf->ep->recv_queue,
					   &match_attr);
	case ofi_op_tagged:
		FI_DBG(&rxm_prov, FI_LOG_CQ, "Got TAGGED op\n");
		match_attr.tag = rx_buf->pkt.hdr.tag;
		return rxm_cq_match_rx_buf(rx_buf, &rx_buf->ep->trecv_queue,
					   &match_attr);
	default:
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unknown op!\n");
		assert(0);
		return -FI_EINVAL;
	}
}

static int rxm_sar_match_msg_id(struct dlist_entry *item, const void *arg)
{
	uint64_t msg_id = *((uint64_t *)arg);
	struct rxm_recv_entry *recv_entry =
		container_of(item, struct rxm_recv_entry, sar.entry);
	return (msg_id == recv_entry->sar.msg_id);
}

static inline
ssize_t rxm_sar_handle_segment(struct rxm_rx_buf *rx_buf)
{
	struct dlist_entry *sar_entry;

	rx_buf->conn = rxm_key2conn(rx_buf->ep,
				    rx_buf->pkt.ctrl_hdr.conn_id);
	if (OFI_UNLIKELY(!rx_buf->conn))
		return -FI_EOTHER;
	FI_DBG(&rxm_prov, FI_LOG_CQ,
	       "Got incoming recv with msg_id: 0x%" PRIx64 "for conn - %p\n",
	       rx_buf->pkt.ctrl_hdr.msg_id, rx_buf->conn);
	sar_entry = dlist_find_first_match(&rx_buf->conn->sar_rx_msg_list,
					   rxm_sar_match_msg_id,
					   &rx_buf->pkt.ctrl_hdr.msg_id);
	if (!sar_entry)
		return rxm_handle_recv_comp(rx_buf);
	rx_buf->recv_entry =
		container_of(sar_entry, struct rxm_recv_entry, sar.entry);
	return rx_buf->ep->txrx_ops->handle_seg_data_rx(rx_buf);
}

static ssize_t rxm_rndv_send_ack_inject(struct rxm_rx_buf *rx_buf)
{
	struct rxm_pkt pkt = {
		.hdr.op = ofi_op_msg,
		.hdr.version = OFI_OP_VERSION,
		.ctrl_hdr.version = RXM_CTRL_VERSION,
		.ctrl_hdr.type = rxm_ctrl_rndv_ack,
		.ctrl_hdr.conn_id = rx_buf->conn->handle.remote_key,
		.ctrl_hdr.msg_id = rx_buf->pkt.ctrl_hdr.msg_id
	};
	struct iovec iov = {
		.iov_base = &pkt,
		.iov_len = sizeof(pkt),
	};
	struct fi_msg msg = {
		.msg_iov = &iov,
		.iov_count = 1,
		.context = rx_buf,
	};

	return fi_sendmsg(rx_buf->conn->msg_ep, &msg, FI_INJECT);
}

static ssize_t rxm_rndv_send_ack(struct rxm_rx_buf *rx_buf)
{
	ssize_t ret;

	assert(rx_buf->conn);

	if (sizeof(rx_buf->pkt) <= rx_buf->ep->inject_limit) {
		ret = rxm_rndv_send_ack_inject(rx_buf);
		if (!ret)
			goto out;

		if (OFI_UNLIKELY(ret != -FI_EAGAIN)) {
			FI_WARN(&rxm_prov, FI_LOG_CQ,
				"send ack via inject failed for MSG provider\n");
			return ret;
		}
	}

	rx_buf->recv_entry->rndv.tx_buf = (struct rxm_tx_base_buf *)
		rxm_tx_buf_alloc(rx_buf->ep, RXM_BUF_POOL_TX_ACK);
	if (OFI_UNLIKELY(!rx_buf->recv_entry->rndv.tx_buf)) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"ran out of buffers from ACK buffer pool\n");
		return -FI_EAGAIN;
	}
	assert(rx_buf->recv_entry->rndv.tx_buf->pkt.ctrl_hdr.type == rxm_ctrl_rndv_ack);

	assert(rx_buf->hdr.state == RXM_RNDV_READ);

	rx_buf->recv_entry->rndv.tx_buf->pkt.ctrl_hdr.conn_id =
		rx_buf->conn->handle.remote_key;
	rx_buf->recv_entry->rndv.tx_buf->pkt.ctrl_hdr.msg_id =
		rx_buf->pkt.ctrl_hdr.msg_id;

	ret = fi_send(rx_buf->conn->msg_ep, &rx_buf->recv_entry->rndv.tx_buf->pkt,
		      sizeof(rx_buf->recv_entry->rndv.tx_buf->pkt),
		      rx_buf->recv_entry->rndv.tx_buf->hdr.desc, 0, rx_buf);
	if (OFI_UNLIKELY(ret)) {
		if (OFI_LIKELY(ret == -FI_EAGAIN)) {
			struct rxm_deferred_tx_entry *def_tx_entry =
				rxm_ep_alloc_deferred_tx_entry(
					rx_buf->ep, rx_buf->conn,
					RXM_DEFERRED_TX_RNDV_ACK);
			if (OFI_UNLIKELY(!def_tx_entry)) {
				FI_WARN(&rxm_prov, FI_LOG_CQ, "unable to "
					"allocate TX entry for deferred ACK\n");
				ret = -FI_EAGAIN;
				goto err;
			}

			def_tx_entry->rndv_ack.rx_buf = rx_buf;
			rxm_ep_enqueue_deferred_tx_queue(def_tx_entry);
			return 0;
		} else {
			FI_WARN(&rxm_prov, FI_LOG_CQ,
				"unable to send ACK: %zd\n", ret);
		}
		goto err;
	}
out:
	RXM_UPDATE_STATE(FI_LOG_CQ, rx_buf, RXM_RNDV_ACK_SENT);
	return 0;
err:
	ofi_buf_free(rx_buf->recv_entry->rndv.tx_buf);
	return ret;
}



static int rxm_handle_remote_write(struct rxm_ep *rxm_ep,
				   struct fi_cq_data_entry *comp)
{
	int ret;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "writing remote write completion\n");
	ret = ofi_cq_write(rxm_ep->util_ep.rx_cq, NULL, comp->flags, 0, NULL,
			   comp->data, 0);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
				"Unable to write remote write completion\n");
		return ret;
	}
	ofi_ep_rem_wr_cntr_inc(&rxm_ep->util_ep);
	if (comp->op_context)
		rxm_rx_buf_free(comp->op_context);
	return 0;
}

static inline void rxm_ep_format_atomic_resp_pkt_hdr(struct rxm_conn *rxm_conn,
				struct rxm_tx_atomic_buf *tx_buf,
				size_t data_len, uint32_t pkt_op,
				enum fi_datatype datatype, uint8_t atomic_op)
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
				    struct rxm_tx_atomic_buf *resp_buf,
				    ssize_t result_len, uint32_t status)
{
	struct rxm_deferred_tx_entry *def_tx_entry;
	struct rxm_atomic_resp_hdr *atomic_hdr;
	ssize_t ret;
	ssize_t resp_len = result_len + sizeof(struct rxm_atomic_resp_hdr) +
				sizeof(struct rxm_pkt);

	resp_buf->hdr.state = RXM_ATOMIC_RESP_SENT;
	rxm_ep_format_atomic_resp_pkt_hdr(rx_buf->conn,
					  resp_buf,
					  resp_len,
					  rx_buf->pkt.hdr.op,
					  rx_buf->pkt.hdr.atomic.datatype,
					  rx_buf->pkt.hdr.atomic.op);
	resp_buf->pkt.ctrl_hdr.conn_id = rx_buf->conn->handle.remote_key;
	resp_buf->pkt.ctrl_hdr.msg_id = rx_buf->pkt.ctrl_hdr.msg_id;
	atomic_hdr = (struct rxm_atomic_resp_hdr *) resp_buf->pkt.data;
	atomic_hdr->status = htonl(status);
	atomic_hdr->result_len = htonl(result_len);

	if (resp_len < rxm_ep->inject_limit) {
		ret = fi_inject(rx_buf->conn->msg_ep, &resp_buf->pkt,
				resp_len, 0);
		if (OFI_LIKELY(!ret))
			ofi_buf_free(resp_buf);
	} else {
		ret = rxm_atomic_send_respmsg(rxm_ep, rx_buf->conn, resp_buf,
					      resp_len);
	}
	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"Unable to send Atomic Response\n");
		if (OFI_LIKELY(ret == -FI_EAGAIN)) {
			def_tx_entry =
				rxm_ep_alloc_deferred_tx_entry(rxm_ep,
						rx_buf->conn,
						RXM_DEFERRED_TX_ATOMIC_RESP);
			if (OFI_UNLIKELY(!def_tx_entry)) {
				FI_WARN(&rxm_prov, FI_LOG_CQ,
					"Unable to allocate deferred Atomic "
					"Response\n");
				return -FI_ENOMEM;
			}

			def_tx_entry->atomic_resp.tx_buf = resp_buf;
			def_tx_entry->atomic_resp.len = resp_len;
			rxm_ep_enqueue_deferred_tx_queue(def_tx_entry);
			ret = 0;
		}
	}
	rxm_rx_buf_free(rx_buf);

	return ret;
}

static inline void rxm_do_atomic(struct rxm_pkt *pkt, void *dst, void *src,
				 void *cmp, void *res, size_t count,
				 enum fi_datatype datatype, enum fi_op op)
{
	switch (pkt->hdr.op) {
	case ofi_op_atomic:
		ofi_atomic_write_handlers[op][datatype](dst, src, count);
		break;
	case ofi_op_atomic_fetch:
		ofi_atomic_readwrite_handlers[op][datatype](dst, src, res,
							    count);
		break;
	case ofi_op_atomic_compare:
		assert(op >= OFI_SWAP_OP_START &&
		       op < OFI_SWAP_OP_START + OFI_SWAP_OP_LAST);
		ofi_atomic_swap_handlers[op - OFI_SWAP_OP_START][datatype](dst,
						src, cmp, res, count);
		break;
	default:
		/* Validated prior to calling function */
		break;
	}
}

static inline ssize_t rxm_handle_atomic_req(struct rxm_ep *rxm_ep,
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
	int ret = 0;
	struct rxm_tx_atomic_buf *resp_buf;
	struct rxm_atomic_resp_hdr *resp_hdr;
	struct rxm_domain *domain = container_of(rxm_ep->util_ep.domain,
					 struct rxm_domain, util_domain);

	assert(!(rx_buf->comp_flags &
		 ~(FI_RECV | FI_RECV | FI_REMOTE_CQ_DATA)));
	assert(rx_buf->pkt.hdr.op == ofi_op_atomic ||
	       rx_buf->pkt.hdr.op == ofi_op_atomic_fetch ||
	       rx_buf->pkt.hdr.op == ofi_op_atomic_compare);

	if (rx_buf->ep->srx_ctx)
		rx_buf->conn = rxm_key2conn(rx_buf->ep,
					    rx_buf->pkt.ctrl_hdr.conn_id);
	if (OFI_UNLIKELY(!rx_buf->conn))
		return -FI_EOTHER;

	resp_buf = (struct rxm_tx_atomic_buf *)
		   rxm_tx_buf_alloc(rxm_ep, RXM_BUF_POOL_TX_ATOMIC);
	if (OFI_UNLIKELY(!resp_buf)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Unable to allocate from Atomic buffer pool\n");
		/* TODO: Should this be -FI_ENOMEM - how does it get
		 * processed again */
		return -FI_EAGAIN;
	}

	for (i = 0; i < rx_buf->pkt.hdr.atomic.ioc_count; i++) {
		ret = ofi_mr_verify(&domain->util_domain.mr_map,
				    req_hdr->rma_ioc[i].count * datatype_sz,
				    (uintptr_t *)&req_hdr->rma_ioc[i].addr,
				    req_hdr->rma_ioc[i].key,
				    ofi_rx_mr_reg_flags(rx_buf->pkt.hdr.op,
							atomic_op));
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"Atomic RMA MR verify error %d\n", ret);
			ret = -FI_EACCES;
			goto send_nak;
		}
	}

	len = ofi_total_rma_ioc_cnt(req_hdr->rma_ioc,
			rx_buf->pkt.hdr.atomic.ioc_count) * datatype_sz;
	resp_hdr = (struct rxm_atomic_resp_hdr *) resp_buf->pkt.data;

	for (i = 0, offset = 0; i < rx_buf->pkt.hdr.atomic.ioc_count; i++) {
		rxm_do_atomic(&rx_buf->pkt,
			      (uintptr_t *) req_hdr->rma_ioc[i].addr,
			      req_hdr->data + offset,
			      req_hdr->data + len + offset,
			      resp_hdr->data + offset,
			      req_hdr->rma_ioc[i].count, datatype, atomic_op);
		offset += req_hdr->rma_ioc[i].count * datatype_sz;
	}
	result_len = rx_buf->pkt.hdr.op == ofi_op_atomic ? 0 : offset;

	if (rx_buf->pkt.hdr.op == ofi_op_atomic)
		ofi_ep_rem_wr_cntr_inc(&rxm_ep->util_ep);
	else
		ofi_ep_rem_rd_cntr_inc(&rxm_ep->util_ep);

	return rxm_atomic_send_resp(rxm_ep, rx_buf, resp_buf,
				    result_len, FI_SUCCESS);
send_nak:
	return rxm_atomic_send_resp(rxm_ep, rx_buf, resp_buf, 0, ret);
}


static inline ssize_t rxm_handle_atomic_resp(struct rxm_ep *rxm_ep,
					     struct rxm_rx_buf *rx_buf)
{
	struct rxm_tx_atomic_buf *tx_buf;
	struct rxm_atomic_resp_hdr *resp_hdr =
			(struct rxm_atomic_resp_hdr *) rx_buf->pkt.data;
	uint64_t len;
	int ret = 0;

	tx_buf = ofi_bufpool_get_ibuf(rxm_ep->buf_pools[RXM_BUF_POOL_TX_ATOMIC].pool,
				      rx_buf->pkt.ctrl_hdr.msg_id);
	FI_DBG(&rxm_prov, FI_LOG_CQ, "received atomic response: op: %" PRIu8
	       " msg_id: 0x%" PRIx64 "\n", rx_buf->pkt.hdr.op,
	       rx_buf->pkt.ctrl_hdr.msg_id);

	assert(!(rx_buf->comp_flags & ~(FI_RECV | FI_REMOTE_CQ_DATA)));

	if (OFI_UNLIKELY(resp_hdr->status)) {
		struct util_cntr *cntr = NULL;
		FI_WARN(&rxm_prov, FI_LOG_CQ,
		       "bad atomic response status %d\n", ntohl(resp_hdr->status));

		if (tx_buf->pkt.hdr.op == ofi_op_atomic) {
			cntr = rxm_ep->util_ep.wr_cntr;
		} else if (tx_buf->pkt.hdr.op == ofi_op_atomic_compare ||
			   tx_buf->pkt.hdr.op == ofi_op_atomic_fetch) {
			cntr = rxm_ep->util_ep.rd_cntr;
		} else {
			FI_WARN(&rxm_prov, FI_LOG_CQ,
				"unknown atomic request op!\n");
			assert(0);
		}
		rxm_cq_write_error(rxm_ep->util_ep.tx_cq, cntr,
				   tx_buf->app_context, ntohl(resp_hdr->status));
		goto err;
	}

	len = ofi_total_iov_len(tx_buf->result_iov, tx_buf->result_iov_count);
	assert(ntohl(resp_hdr->result_len) == len);
	ofi_copy_to_iov(tx_buf->result_iov, tx_buf->result_iov_count, 0,
			resp_hdr->data, len);

	if (!(tx_buf->flags & FI_INJECT))
		ret = rxm_cq_tx_comp_write(rxm_ep,
					   ofi_tx_cq_flags(tx_buf->pkt.hdr.op),
					   tx_buf->app_context, tx_buf->flags);

	if (tx_buf->pkt.hdr.op == ofi_op_atomic) {
		ofi_ep_wr_cntr_inc(&rxm_ep->util_ep);
	} else if (tx_buf->pkt.hdr.op == ofi_op_atomic_compare ||
		   tx_buf->pkt.hdr.op == ofi_op_atomic_fetch) {
		ofi_ep_rd_cntr_inc(&rxm_ep->util_ep);
	} else {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "unknown atomic request op!\n");
		rxm_cq_write_error(rxm_ep->util_ep.tx_cq, NULL,
				   tx_buf->app_context, ntohl(resp_hdr->status));
		assert(0);
	}
err:
	rxm_rx_buf_free(rx_buf);
	ofi_buf_free(tx_buf);
	ofi_atomic_inc32(&rxm_ep->atomic_tx_credits);
	assert(ofi_atomic_get32(&rxm_ep->atomic_tx_credits) <=
				rxm_ep->rxm_info->tx_attr->size);

	return ret;
}

int rxm_finish_coll_eager_send(struct rxm_ep *rxm_ep, struct rxm_tx_eager_buf *tx_eager_buf)
{
	int ret;

	if (tx_eager_buf->pkt.hdr.tag & OFI_COLL_TAG_FLAG) {
		ofi_coll_handle_xfer_comp(tx_eager_buf->pkt.hdr.tag,
				tx_eager_buf->app_context);
		ret = FI_SUCCESS;
	} else {
		ret = rxm_finish_eager_send(rxm_ep, tx_eager_buf);
	}

	return ret;
};

ssize_t rxm_cq_handle_comp(struct rxm_ep *rxm_ep, struct fi_cq_data_entry *comp)
{
	ssize_t ret;
	struct rxm_rx_buf *rx_buf;
	struct rxm_tx_sar_buf *tx_sar_buf;
	struct rxm_tx_eager_buf *tx_eager_buf;
	struct rxm_tx_rndv_buf *tx_rndv_buf;
	struct rxm_tx_atomic_buf *tx_atomic_buf;
	struct rxm_rma_buf *rma_buf;

	/* Remote write events may not consume a posted recv so op context
	 * and hence state would be NULL */
	if (comp->flags & FI_REMOTE_WRITE)
		return rxm_handle_remote_write(rxm_ep, comp);

	switch (RXM_GET_PROTO_STATE(comp->op_context)) {
	case RXM_TX:
		tx_eager_buf = comp->op_context;
		ret = rxm_ep->txrx_ops->comp_eager_tx(rxm_ep, tx_eager_buf);
		ofi_buf_free(tx_eager_buf);
		return ret;
	case RXM_INJECT_TX:
		assert(0);
		return -FI_EOPBADSTATE;
	case RXM_RMA:
		rma_buf = comp->op_context;
		assert((comp->flags & (FI_WRITE | FI_RMA)) ||
		       (comp->flags & (FI_READ | FI_RMA)));
		return rxm_finish_rma(rxm_ep, rma_buf, comp->flags);
	case RXM_RX:
		rx_buf = comp->op_context;
		assert(!(comp->flags & FI_REMOTE_READ));
		assert((rx_buf->pkt.hdr.version == OFI_OP_VERSION) &&
		       (rx_buf->pkt.ctrl_hdr.version == RXM_CTRL_VERSION));

		switch (rx_buf->pkt.ctrl_hdr.type) {
		case rxm_ctrl_eager:
		case rxm_ctrl_rndv:
			return rxm_handle_recv_comp(rx_buf);
		case rxm_ctrl_rndv_ack:
			return rxm_rndv_handle_ack(rxm_ep, rx_buf);
		case rxm_ctrl_seg:
			return rxm_sar_handle_segment(rx_buf);
		case rxm_ctrl_atomic:
			return rxm_handle_atomic_req(rxm_ep, rx_buf);
		case rxm_ctrl_atomic_resp:
			return rxm_handle_atomic_resp(rxm_ep, rx_buf);
		default:
			FI_WARN(&rxm_prov, FI_LOG_CQ, "Unknown message type\n");
			assert(0);
			return -FI_EINVAL;
		}
	case RXM_SAR_TX:
		tx_sar_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		return rxm_finish_sar_segment_send(rxm_ep, tx_sar_buf, false);
	case RXM_RNDV_TX:
		tx_rndv_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		RXM_UPDATE_STATE(FI_LOG_CQ, tx_rndv_buf, RXM_RNDV_ACK_WAIT);
		return 0;
	case RXM_RNDV_ACK_WAIT:
		assert(0);
		return -FI_EOPBADSTATE;
	case RXM_RNDV_READ:
		rx_buf = comp->op_context;
		assert(comp->flags & FI_READ);
		if (++rx_buf->rndv_rma_index < rx_buf->rndv_hdr->count)
			return 0;
		else
			return rxm_rndv_send_ack(rx_buf);
	case RXM_RNDV_ACK_SENT:
		assert(comp->flags & FI_SEND);
		return rxm_finish_send_rndv_ack(comp->op_context);
	case RXM_RNDV_ACK_RECVD:
		tx_rndv_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		return rxm_rndv_tx_finish(rxm_ep, tx_rndv_buf);
	case RXM_RNDV_FINISH:
		assert(0);
		return -FI_EOPBADSTATE;
	case RXM_ATOMIC_RESP_WAIT:
		/* Optional atomic request completion; TX completion
		 * processing is performed when atomic response is received */
		assert(comp->flags & FI_SEND);
		return 0;
	case RXM_ATOMIC_RESP_SENT:
		tx_atomic_buf = comp->op_context;
		assert(comp->flags & FI_SEND);
		ofi_buf_free(tx_atomic_buf);
		return 0;
	default:
		assert(0);
		return -FI_EOPBADSTATE;
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
	if (rxm_ep->util_ep.tx_cntr)
		rxm_cntr_incerr(rxm_ep->util_ep.tx_cntr);

	if (rxm_ep->util_ep.rx_cntr)
		rxm_cntr_incerr(rxm_ep->util_ep.rx_cntr);

	if (rxm_ep->util_ep.wr_cntr)
		rxm_cntr_incerr(rxm_ep->util_ep.wr_cntr);

	if (rxm_ep->util_ep.rd_cntr)
		rxm_cntr_incerr(rxm_ep->util_ep.rd_cntr);
}

void rxm_cq_read_write_error(struct rxm_ep *rxm_ep)
{
	struct rxm_tx_eager_buf *eager_buf;
	struct rxm_tx_sar_buf *sar_buf;
	struct rxm_tx_rndv_buf *rndv_buf;
	struct rxm_rx_buf *rx_buf;
	struct rxm_rma_buf *rma_buf;
	struct util_cq *cq;
	struct util_cntr *cntr;
	struct fi_cq_err_entry err_entry = {0};
	ssize_t ret;

	ret = fi_cq_readerr(rxm_ep->msg_cq, &err_entry, 0);
	if ((ret) < 0) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"unable to fi_cq_readerr on msg cq\n");
		rxm_cq_write_error_all(rxm_ep, (int)ret);
		return;
	}

	if (err_entry.err != FI_ECANCELED)
		OFI_CQ_STRERROR(&rxm_prov, FI_LOG_WARN, FI_LOG_CQ,
				rxm_ep->msg_cq, &err_entry);

	cq = rxm_ep->util_ep.tx_cq;
	cntr = rxm_ep->util_ep.tx_cntr;

	switch (RXM_GET_PROTO_STATE(err_entry.op_context)) {
	case RXM_TX:
		eager_buf = err_entry.op_context;
		err_entry.op_context = eager_buf->app_context;
		err_entry.flags = ofi_tx_cq_flags(eager_buf->pkt.hdr.op);
		ofi_buf_free(eager_buf);
		break;
	case RXM_INJECT_TX:
		assert(0);
		return;
	case RXM_RMA:
		rma_buf = err_entry.op_context;
		err_entry.op_context = rma_buf->app_context;
		err_entry.flags = err_entry.flags;
		if (!(rma_buf->flags & FI_INJECT) && !rxm_ep->rdm_mr_local &&
		    rxm_ep->msg_mr_local) {
			rxm_msg_mr_closev(rma_buf->mr.mr, rma_buf->mr.count);
		}
		ofi_buf_free(rma_buf);
		break;
	case RXM_SAR_TX:
		sar_buf = err_entry.op_context;
		err_entry.op_context = sar_buf->app_context;
		err_entry.flags = ofi_tx_cq_flags(sar_buf->pkt.hdr.op);
		rxm_finish_sar_segment_send(rxm_ep, sar_buf, true);
		break;
	case RXM_RNDV_TX:
		rndv_buf = err_entry.op_context;
		err_entry.op_context = rndv_buf->app_context;
		err_entry.flags = ofi_tx_cq_flags(rndv_buf->pkt.hdr.op);
		break;

	/* Application receive related error */
	case RXM_RX:
		/* Silently drop any MSG CQ error entries for canceled receive
		 * operations as these are internal to RxM. This situation can
		 * happen when the MSG EP receives a reject / shutdown and CM
		 * thread hasn't handled the event yet. */
		if (err_entry.err == FI_ECANCELED) {
			/* No need to re-post these buffers. Free directly */
			ofi_buf_free((struct rxm_rx_buf *)err_entry.op_context);
			return;
		}
		/* fall through */
	case RXM_RNDV_ACK_SENT:
		/* fall through */
	case RXM_RNDV_READ:
		rx_buf = (struct rxm_rx_buf *) err_entry.op_context;
		assert(rx_buf->recv_entry);
		err_entry.op_context = rx_buf->recv_entry->context;
		err_entry.flags = rx_buf->recv_entry->comp_flags;

		cq = rx_buf->ep->util_ep.rx_cq;
		cntr = rx_buf->ep->util_ep.rx_cntr;
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
	if (ret)
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Unable to ofi_cq_write_error\n");
}

static inline int rxm_msg_ep_recv(struct rxm_rx_buf *rx_buf)
{
	int ret;

	if (rx_buf->ep->srx_ctx)
		rx_buf->conn = NULL;
	rx_buf->hdr.state = RXM_RX;

	ret = (int)fi_recv(rx_buf->msg_ep, &rx_buf->pkt,
			   rxm_eager_limit + sizeof(struct rxm_pkt),
			   rx_buf->hdr.desc, FI_ADDR_UNSPEC, rx_buf);
	if (OFI_LIKELY(!ret))
		return 0;

	if (ret != -FI_EAGAIN) {
		int level = FI_LOG_WARN;
		if (rx_buf->conn->handle.state == RXM_CMAP_SHUTDOWN)
			level = FI_LOG_DEBUG;
		FI_LOG(&rxm_prov, level, FI_LOG_EP_CTRL,
		       "unable to post recv buf: %d\n", ret);
	}
	return ret;
}

int rxm_msg_ep_prepost_recv(struct rxm_ep *rxm_ep, struct fid_ep *msg_ep)
{
	struct rxm_rx_buf *rx_buf;
	int ret;
	size_t i;

	for (i = 0; i < rxm_ep->msg_info->rx_attr->size; i++) {
		rx_buf = rxm_rx_buf_alloc(rxm_ep, msg_ep, 1);
		if (OFI_UNLIKELY(!rx_buf))
			return -FI_ENOMEM;

		ret = rxm_msg_ep_recv(rx_buf);
		if (OFI_UNLIKELY(ret)) {
			ofi_buf_free(&rx_buf->hdr);
			return ret;
		}
	}
	return 0;
}

void rxm_ep_do_progress(struct util_ep *util_ep)
{
	struct rxm_ep *rxm_ep = container_of(util_ep, struct rxm_ep, util_ep);
	struct fi_cq_data_entry comp;
	struct dlist_entry *conn_entry_tmp;
	struct rxm_conn *rxm_conn;
	struct rxm_rx_buf *buf;
	ssize_t ret;
	size_t comp_read = 0;
	uint64_t timestamp;

	while (!dlist_empty(&rxm_ep->repost_ready_list)) {
		dlist_pop_front(&rxm_ep->repost_ready_list, struct rxm_rx_buf,
				buf, repost_entry);

		/* Discard rx buffer if its msg_ep was closed */
		if (!rxm_ep->srx_ctx && !buf->conn->msg_ep) {
			ofi_buf_free(&buf->hdr);
			continue;
		}

		ret = rxm_msg_ep_recv(buf);
		if (ret) {
			if (OFI_LIKELY(ret == -FI_EAGAIN))
				ofi_buf_free(&buf->hdr);
		}
	}

	do {

		ret = fi_cq_read(rxm_ep->msg_cq, &comp, 1);
		if (ret > 0) {
			// We don't have enough info to write a good
			// error entry to the CQ at this point
			ret = rxm_cq_handle_comp(rxm_ep, &comp);
			if (OFI_UNLIKELY(ret)) {
				rxm_cq_write_error_all(rxm_ep, ret);
			} else {
				ret = 1;
			}
		} else if (ret < 0 && (ret != -FI_EAGAIN)) {
			if (ret == -FI_EAVAIL)
				rxm_cq_read_write_error(rxm_ep);
			else
				rxm_cq_write_error_all(rxm_ep, ret);
		}

		if (ret == -FI_EAGAIN || --rxm_ep->cq_eq_fairness <= 0) {
			rxm_ep->cq_eq_fairness = rxm_cq_eq_fairness;
			timestamp = ofi_gettime_us();
			if (timestamp - rxm_ep->msg_cq_last_poll >
				rxm_cm_progress_interval) {
				rxm_ep->msg_cq_last_poll = timestamp;
				rxm_msg_eq_progress(rxm_ep);
			}
		}
	} while ((ret > 0) && (++comp_read < rxm_ep->comp_per_progress));

	if (OFI_UNLIKELY(!dlist_empty(&rxm_ep->deferred_tx_conn_queue))) {
		dlist_foreach_container_safe(&rxm_ep->deferred_tx_conn_queue,
					     struct rxm_conn, rxm_conn,
					     deferred_conn_entry, conn_entry_tmp)
			rxm_ep_progress_deferred_queue(rxm_ep, rxm_conn);
	}
}

void rxm_ep_progress(struct util_ep *util_ep)
{
	ofi_ep_lock_acquire(util_ep);
	rxm_ep_do_progress(util_ep);
	ofi_ep_lock_release(util_ep);
}

void rxm_ep_progress_coll(struct util_ep *util_ep)
{
	ofi_ep_lock_acquire(util_ep);
	rxm_ep_do_progress(util_ep);
	ofi_ep_lock_release(util_ep);

	ofi_coll_ep_progress(&util_ep->ep_fid);
}

static int rxm_cq_close(struct fid *fid)
{
	struct util_cq *util_cq;
	int ret, retv = 0;

	util_cq = container_of(fid, struct util_cq, cq_fid.fid);

	ret = ofi_cq_cleanup(util_cq);
	if (ret)
		retv = ret;

	free(util_cq);
	return retv;
}

static struct fi_ops rxm_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_cq_close,
	.bind = fi_no_bind,
	.control = ofi_cq_control,
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

int rxm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_fid, void *context)
{
	struct util_cq *util_cq;
	int ret;

	util_cq = calloc(1, sizeof(*util_cq));
	if (!util_cq)
		return -FI_ENOMEM;

	ret = ofi_cq_init(&rxm_prov, domain, attr, util_cq, &ofi_cq_progress,
			context);
	if (ret)
		goto err1;

	*cq_fid = &util_cq->cq_fid;
	/* Override util_cq_fi_ops */
	(*cq_fid)->fid.ops = &rxm_cq_fi_ops;
	(*cq_fid)->ops = &rxm_cq_ops;
	return 0;
err1:
	free(util_cq);
	return ret;
}
