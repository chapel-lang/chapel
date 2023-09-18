/*
 * Copyright (c) 2017 Intel Corporation, Inc.  All rights reserved.
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

#include <rdma/fi_errno.h>

#include <ofi_prov.h>
#include "tcpx.h"
#include <poll.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <ofi_util.h>
#include <ofi_iov.h>


static ssize_t (*tcpx_start_op[ofi_op_write + 1])(struct tcpx_ep *ep);


static ssize_t tcpx_send_msg(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *tx_entry;
	ssize_t ret;
	size_t len;

	assert(ep->cur_tx.entry);
	tx_entry = ep->cur_tx.entry;
	ret = ofi_bsock_sendv(&ep->bsock, tx_entry->iov, tx_entry->iov_cnt,
			      &len);
	if (ret < 0 && ret != -OFI_EINPROGRESS_ASYNC)
		return ret;

	if (ret == -OFI_EINPROGRESS_ASYNC) {
		/* If a transfer generated multiple async sends, we only
		 * need to track the last async index to know when the entire
		 * transfer has completed.
		 */
		tx_entry->async_index = ep->bsock.async_index;
		tx_entry->ctrl_flags |= TCPX_ASYNC;
	} else {
		len = ret;
	}

	ep->cur_tx.data_left -= len;
	if (ep->cur_tx.data_left) {
		ofi_consume_iov(tx_entry->iov, &tx_entry->iov_cnt, len);
		return -FI_EAGAIN;
	}
	return FI_SUCCESS;
}

static ssize_t tcpx_recv_msg_data(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	ssize_t ret;

	if (!ep->cur_rx.data_left)
		return FI_SUCCESS;

	rx_entry = ep->cur_rx.entry;
	ret = ofi_bsock_recvv(&ep->bsock, rx_entry->iov, rx_entry->iov_cnt);
	if (ret < 0)
		return ret;

	ep->cur_rx.data_left -= ret;
	if (!ep->cur_rx.data_left)
		return FI_SUCCESS;

	ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt, ret);
	if (!rx_entry->iov_cnt || !rx_entry->iov[0].iov_len)
		return -FI_ETRUNC;

	return -FI_EAGAIN;
}

void tcpx_progress_tx(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *tx_entry;
	struct tcpx_cq *cq;
	ssize_t ret;

	assert(ofi_mutex_held(&ep->lock));
	while (ep->cur_tx.entry) {
		ret = tcpx_send_msg(ep);
		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
			return;

		tx_entry = ep->cur_tx.entry;
		cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);

		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_DOMAIN, "msg send failed\n");
			tcpx_cntr_incerr(ep, tx_entry);
			tcpx_cq_report_error(&cq->util_cq, tx_entry, (int) -ret);
			tcpx_free_xfer(cq, tx_entry);
		} else if (tx_entry->ctrl_flags & TCPX_NEED_ACK) {
			/* A SW ack guarantees the peer received the data, so
			 * we can skip the async completion.
			 */
			slist_insert_tail(&tx_entry->entry,
					  &ep->need_ack_queue);
		} else if (tx_entry->ctrl_flags & TCPX_NEED_RESP) {
			// discard send but enable receive for completeion
			assert(tx_entry->resp_entry);
			tx_entry->resp_entry->ctrl_flags &= ~TCPX_INTERNAL_XFER;
			tcpx_free_xfer(cq, tx_entry);
		} else if ((tx_entry->ctrl_flags & TCPX_ASYNC) &&
			   (ofi_val32_gt(tx_entry->async_index,
					 ep->bsock.done_index))) {
			slist_insert_tail(&tx_entry->entry,
						&ep->async_queue);
		} else {
			ep->report_success(ep, &cq->util_cq, tx_entry);
			tcpx_free_xfer(cq, tx_entry);
		}

		if (!slist_empty(&ep->priority_queue)) {
			ep->cur_tx.entry = container_of(slist_remove_head(
							&ep->priority_queue),
					     struct tcpx_xfer_entry, entry);
			assert(ep->cur_tx.entry->ctrl_flags & TCPX_INTERNAL_XFER);
		} else if (!slist_empty(&ep->tx_queue)) {
			ep->cur_tx.entry = container_of(slist_remove_head(
							&ep->tx_queue),
					     struct tcpx_xfer_entry, entry);
			assert(!(ep->cur_tx.entry->ctrl_flags & TCPX_INTERNAL_XFER));
		} else {
			ep->cur_tx.entry = NULL;
			break;
		}

		ep->cur_tx.data_left = ep->cur_tx.entry->hdr.base_hdr.size;
		OFI_DBG_SET(ep->cur_tx.entry->hdr.base_hdr.id, ep->tx_id++);
		ep->hdr_bswap(&ep->cur_tx.entry->hdr.base_hdr);
	}

	/* Buffered data is sent first by tcpx_send_msg, but if we don't
	 * have other data to send, we need to try flushing any buffered data.
	 */
	(void) ofi_bsock_flush(&ep->bsock);
}

static int tcpx_queue_ack(struct tcpx_xfer_entry *rx_entry)
{
	struct tcpx_ep *ep;
	struct tcpx_cq *cq;
	struct tcpx_xfer_entry *resp;

	ep = rx_entry->ep;
	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);

	resp = tcpx_alloc_xfer(cq);
	if (!resp)
		return -FI_ENOMEM;

	resp->iov[0].iov_base = (void *) &resp->hdr;
	resp->iov[0].iov_len = sizeof(resp->hdr.base_hdr);
	resp->iov_cnt = 1;

	resp->hdr.base_hdr.version = TCPX_HDR_VERSION;
	resp->hdr.base_hdr.op_data = TCPX_OP_ACK;
	resp->hdr.base_hdr.op = ofi_op_msg;
	resp->hdr.base_hdr.size = sizeof(resp->hdr.base_hdr);
	resp->hdr.base_hdr.hdr_size = (uint8_t) sizeof(resp->hdr.base_hdr);

	resp->ctrl_flags = TCPX_INTERNAL_XFER;
	resp->context = NULL;
	resp->ep = ep;

	tcpx_tx_queue_insert(ep, resp);
	return FI_SUCCESS;
}

static int tcpx_update_rx_iov(struct tcpx_xfer_entry *rx_entry)
{
	struct ofi_cq_rbuf_entry cq_entry;
	int ret;

	assert(tcpx_dynamic_rbuf(rx_entry->ep));

	cq_entry.ep_context = rx_entry->ep->util_ep.ep_fid.fid.context;
	cq_entry.op_context = rx_entry->context;
	cq_entry.flags = 0;
	cq_entry.len = rx_entry->hdr.base_hdr.size -
		       rx_entry->hdr.base_hdr.hdr_size;
	cq_entry.buf = rx_entry->mrecv_msg_start;
	tcpx_get_cq_info(rx_entry, &cq_entry.flags, &cq_entry.data,
			 &cq_entry.tag);

	rx_entry->iov_cnt = TCPX_IOV_LIMIT;
	ret = (int) tcpx_dynamic_rbuf(rx_entry->ep)->
		    get_rbuf(&cq_entry, &rx_entry->iov[0], &rx_entry->iov_cnt);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
			"get_rbuf callback failed %s\n",
			fi_strerror(-ret));
		return ret;
	}

	assert(rx_entry->iov_cnt <= TCPX_IOV_LIMIT);
	ret = ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_cnt,
			       rx_entry->ep->cur_rx.data_left);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
			"dynamically provided rbuf is too small\n");
		return ret;
	}

	return 0;
}

static ssize_t tcpx_process_recv(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	ssize_t ret;

	rx_entry = ep->cur_rx.entry;
retry:
	ret = tcpx_recv_msg_data(ep);
	if (ret) {
		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
			return ret;

		if (ret != -FI_ETRUNC)
			goto err;
		assert(rx_entry->ctrl_flags & TCPX_NEED_DYN_RBUF);
	}

	if (rx_entry->ctrl_flags & TCPX_NEED_DYN_RBUF) {
		ret = tcpx_update_rx_iov(rx_entry);
		if (ret)
			goto err;

		rx_entry->ctrl_flags &= ~TCPX_NEED_DYN_RBUF;
		goto retry;
	}

	if (rx_entry->hdr.base_hdr.flags & TCPX_DELIVERY_COMPLETE) {
		ret = tcpx_queue_ack(rx_entry);
		if (ret)
			goto err;
	}

	ep->report_success(ep, ep->util_ep.rx_cq, rx_entry);
	tcpx_free_rx(rx_entry);
	tcpx_reset_rx(ep);
	return 0;

err:
	FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
		"msg recv failed ret = %zd (%s)\n", ret, fi_strerror((int)-ret));
	tcpx_cntr_incerr(ep, rx_entry);
	tcpx_cq_report_error(rx_entry->ep->util_ep.rx_cq, rx_entry, (int) -ret);
	tcpx_free_rx(rx_entry);
	tcpx_reset_rx(ep);
	return ret;
}

static void tcpx_pmem_commit(struct tcpx_xfer_entry *rx_entry)
{
	struct ofi_rma_iov *rma_iov;
	size_t offset;
	int i;

	if (!ofi_pmem_commit)
		return ;

	if (rx_entry->hdr.base_hdr.flags & TCPX_REMOTE_CQ_DATA)
		offset = sizeof(rx_entry->hdr.base_hdr) + sizeof(uint64_t);
	else
		offset = sizeof(rx_entry->hdr.base_hdr);


	rma_iov = (struct ofi_rma_iov *)((uint8_t *)&rx_entry->hdr + offset);

	for (i = 0; i < rx_entry->hdr.base_hdr.rma_iov_cnt; i++) {
		(*ofi_pmem_commit)((const void *) (uintptr_t) rma_iov[i].addr,
				   rma_iov[i].len);
	}
}

static ssize_t tcpx_process_remote_write(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	struct tcpx_cq *cq;
	ssize_t ret;

	rx_entry = ep->cur_rx.entry;
	ret = tcpx_recv_msg_data(ep);
	if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
		return ret;

	cq = container_of(ep->util_ep.rx_cq, struct tcpx_cq, util_cq);
	if (ret)
		goto err;

	if (rx_entry->hdr.base_hdr.flags &
	    (TCPX_DELIVERY_COMPLETE | TCPX_COMMIT_COMPLETE)) {

		if (rx_entry->hdr.base_hdr.flags & TCPX_COMMIT_COMPLETE)
			tcpx_pmem_commit(rx_entry);

		ret = tcpx_queue_ack(rx_entry);
		if (ret)
			goto err;
	}

	ep->report_success(ep, ep->util_ep.rx_cq, rx_entry);
	tcpx_free_xfer(cq, rx_entry);
	tcpx_reset_rx(ep);
	return FI_SUCCESS;

err:
	FI_WARN(&tcpx_prov, FI_LOG_DOMAIN, "remote write failed %zd\n", ret);
	tcpx_free_xfer(cq, rx_entry);
	tcpx_reset_rx(ep);
	return ret;
}

static ssize_t tcpx_process_remote_read(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	struct tcpx_cq *cq;
	ssize_t ret;

	rx_entry = ep->cur_rx.entry;
	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);

	ret = tcpx_recv_msg_data(ep);
	if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
		return ret;

	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_DOMAIN,
			"msg recv Failed ret = %zd\n", ret);
		tcpx_cntr_incerr(ep, rx_entry);
		tcpx_cq_report_error(&cq->util_cq, rx_entry, (int) -ret);
	} else {
		ep->report_success(ep, &cq->util_cq, rx_entry);
	}

	slist_remove_head(&rx_entry->ep->rma_read_queue);
	tcpx_free_xfer(cq, rx_entry);
	tcpx_reset_rx(ep);
	return ret;
}

static struct tcpx_xfer_entry *tcpx_get_rx_entry(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *xfer;
	struct tcpx_rx_ctx *srx;

	if (ep->srx_ctx) {
		srx = ep->srx_ctx;
		ofi_mutex_lock(&srx->lock);
		if (!slist_empty(&srx->rx_queue)) {
			xfer = container_of(slist_remove_head(&srx->rx_queue),
					    struct tcpx_xfer_entry, entry);
			xfer->cq_flags |= tcpx_rx_completion_flag(ep, 0);
		} else {
			xfer = NULL;
		}
		ofi_mutex_unlock(&ep->srx_ctx->lock);
	} else {
		assert(ofi_mutex_held(&ep->lock));
		if (!slist_empty(&ep->rx_queue)) {
			xfer = container_of(slist_remove_head(&ep->rx_queue),
					    struct tcpx_xfer_entry, entry);
			ep->rx_avail++;
		} else {
			xfer = NULL;
		}
	}

	return xfer;
}

static int tcpx_handle_ack(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *tx_entry;
	struct tcpx_cq *cq;

	if (ep->cur_rx.hdr.base_hdr.size !=
	    sizeof(ep->cur_rx.hdr.base_hdr))
		return -FI_EIO;

	assert(!slist_empty(&ep->need_ack_queue));
	tx_entry = container_of(slist_remove_head(&ep->need_ack_queue),
				struct tcpx_xfer_entry, entry);

	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);
	ep->report_success(ep, ep->util_ep.tx_cq, tx_entry);
	tcpx_free_xfer(cq, tx_entry);
	tcpx_reset_rx(ep);
	return FI_SUCCESS;
}

static ssize_t tcpx_op_msg(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	struct tcpx_cur_rx *msg = &ep->cur_rx;
	size_t msg_len;
	ssize_t ret;

	if (msg->hdr.base_hdr.op_data == TCPX_OP_ACK)
		return tcpx_handle_ack(ep);

	msg_len = (msg->hdr.base_hdr.size - msg->hdr.base_hdr.hdr_size);

	rx_entry = tcpx_get_rx_entry(ep);
	if (!rx_entry)
		return -FI_EAGAIN;

	memcpy(&rx_entry->hdr, &msg->hdr,
	       (size_t) msg->hdr.base_hdr.hdr_size);
	rx_entry->ep = ep;
	rx_entry->mrecv_msg_start = rx_entry->iov[0].iov_base;

	if (tcpx_dynamic_rbuf(ep)) {
		rx_entry->ctrl_flags = TCPX_NEED_DYN_RBUF;

		if (msg->hdr.base_hdr.flags & TCPX_TAGGED) {
			/* Raw message, no rxm header */
			rx_entry->iov_cnt = 0;
		} else {
			/* Receiving only rxm header */
			assert(msg_len >= ofi_total_iov_len(rx_entry->iov,
							    rx_entry->iov_cnt));
		}
	} else {
		ret = ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_cnt,
				       msg_len);
		if (ret)
			goto truncate_err;
	}

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = tcpx_process_recv;
	return tcpx_process_recv(ep);

truncate_err:
	FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
		"posted rx buffer size is not big enough\n");
	tcpx_cntr_incerr(ep, rx_entry);
	tcpx_cq_report_error(rx_entry->ep->util_ep.rx_cq, rx_entry, (int) -ret);
	tcpx_free_rx(rx_entry);
	return ret;
}

static ssize_t tcpx_op_tagged(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	struct tcpx_cur_rx *msg = &ep->cur_rx;
	size_t msg_len;
	uint64_t tag;
	ssize_t ret;

	assert(ep->srx_ctx && !tcpx_dynamic_rbuf(ep));
	msg_len = (msg->hdr.base_hdr.size - msg->hdr.base_hdr.hdr_size);

	tag = (msg->hdr.base_hdr.flags & TCPX_REMOTE_CQ_DATA) ?
	      msg->hdr.tag_data_hdr.tag : msg->hdr.tag_hdr.tag;

	rx_entry = ep->srx_ctx->match_tag_rx(ep->srx_ctx, ep, tag);
	if (!rx_entry)
		return -FI_EAGAIN;

	rx_entry->cq_flags |= tcpx_rx_completion_flag(ep, 0);
	memcpy(&rx_entry->hdr, &msg->hdr,
	       (size_t) msg->hdr.base_hdr.hdr_size);
	rx_entry->ep = ep;

	ret = ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_cnt, msg_len);
	if (ret)
		goto truncate_err;

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = tcpx_process_recv;
	return tcpx_process_recv(ep);

truncate_err:
	FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
		"posted rx buffer size is not big enough\n");
	tcpx_cntr_incerr(ep, rx_entry);
	tcpx_cq_report_error(rx_entry->ep->util_ep.rx_cq, rx_entry, (int) -ret);
	tcpx_free_rx(rx_entry);
	return ret;
}

static ssize_t tcpx_op_read_req(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *resp;
	struct tcpx_cq *cq;
	struct ofi_rma_iov *rma_iov;
	ssize_t i, ret;

	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);
	resp = tcpx_alloc_xfer(cq);
	if (!resp)
		return -FI_ENOMEM;

	memcpy(&resp->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	resp->hdr.base_hdr.op_data = 0;
	resp->ep = ep;

	resp->iov[0].iov_base = (void *) &resp->hdr;
	resp->iov[0].iov_len = sizeof(resp->hdr.base_hdr);

	rma_iov = (struct ofi_rma_iov *) ((uint8_t *)
		  &resp->hdr + sizeof(resp->hdr.base_hdr));

	resp->iov_cnt = 1 + resp->hdr.base_hdr.rma_iov_cnt;
	resp->hdr.base_hdr.size = resp->iov[0].iov_len;
	for (i = 0; i < resp->hdr.base_hdr.rma_iov_cnt; i++) {
		ret = ofi_mr_verify(&ep->util_ep.domain->mr_map, rma_iov[i].len,
				    (uintptr_t *) &rma_iov[i].addr,
				    rma_iov[i].key, FI_REMOTE_READ);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
			       "invalid rma iov received\n");
			tcpx_free_xfer(cq, resp);
			return ret;
		}

		resp->iov[i + 1].iov_base = (void *) (uintptr_t)
					    rma_iov[i].addr;
		resp->iov[i + 1].iov_len = rma_iov[i].len;
		resp->hdr.base_hdr.size += resp->iov[i + 1].iov_len;
	}

	resp->hdr.base_hdr.op = ofi_op_read_rsp;
	resp->hdr.base_hdr.hdr_size = (uint8_t) sizeof(resp->hdr.base_hdr);

	resp->ctrl_flags = TCPX_INTERNAL_XFER;
	resp->context = NULL;

	tcpx_tx_queue_insert(ep, resp);
	tcpx_reset_rx(ep);
	return FI_SUCCESS;
}

static ssize_t tcpx_op_write(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	struct tcpx_cq *cq;
	struct ofi_rma_iov *rma_iov;
	ssize_t ret, i;

	cq = container_of(ep->util_ep.rx_cq, struct tcpx_cq, util_cq);
	rx_entry = tcpx_alloc_xfer(cq);
	if (!rx_entry)
		return -FI_ENOMEM;

	if (ep->cur_rx.hdr.base_hdr.flags & TCPX_REMOTE_CQ_DATA) {
		rx_entry->cq_flags = (FI_COMPLETION | FI_REMOTE_WRITE |
				      FI_REMOTE_CQ_DATA);
		rma_iov = (struct ofi_rma_iov *) ((uint8_t *) &rx_entry->hdr +
			   sizeof(rx_entry->hdr.cq_data_hdr));
	} else {
		rx_entry->ctrl_flags = TCPX_INTERNAL_XFER;
		rma_iov = (struct ofi_rma_iov *) ((uint8_t *) &rx_entry->hdr +
			  sizeof(rx_entry->hdr.base_hdr));
	}

	memcpy(&rx_entry->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	rx_entry->hdr.base_hdr.op_data = 0;
	rx_entry->ep = ep;

	rx_entry->iov_cnt = rx_entry->hdr.base_hdr.rma_iov_cnt;
	for (i = 0; i < rx_entry->hdr.base_hdr.rma_iov_cnt; i++) {
		ret = ofi_mr_verify(&ep->util_ep.domain->mr_map, rma_iov[i].len,
				    (uintptr_t *) &rma_iov[i].addr,
				    rma_iov[i].key, FI_REMOTE_WRITE);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
			       "invalid rma iov received\n");
			tcpx_free_xfer(cq, rx_entry);
			return ret;
		}
		rx_entry->iov[i].iov_base = (void *) (uintptr_t)
					      rma_iov[i].addr;
		rx_entry->iov[i].iov_len = rma_iov[i].len;
	}

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = tcpx_process_remote_write;
	return tcpx_process_remote_write(ep);
}

static ssize_t tcpx_op_read_rsp(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *rx_entry;
	struct slist_entry *entry;

	if (slist_empty(&ep->rma_read_queue))
		return -FI_EINVAL;

	entry = ep->rma_read_queue.head;
	rx_entry = container_of(entry, struct tcpx_xfer_entry, entry);

	memcpy(&rx_entry->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	rx_entry->hdr.base_hdr.op_data = 0;

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = tcpx_process_remote_read;
	return tcpx_process_remote_read(ep);
}

static ssize_t tcpx_recv_hdr(struct tcpx_ep *ep)
{
	size_t len;
	void *buf;
	ssize_t ret;

	assert(ep->cur_rx.hdr_done < ep->cur_rx.hdr_len);

next_hdr:
	buf = (uint8_t *) &ep->cur_rx.hdr + ep->cur_rx.hdr_done;
	len = ep->cur_rx.hdr_len - ep->cur_rx.hdr_done;
	ret = ofi_bsock_recv(&ep->bsock, buf, len);
	if (ret < 0)
		return ret;

	ep->cur_rx.hdr_done += ret;
	if (ep->cur_rx.hdr_done == sizeof(ep->cur_rx.hdr.base_hdr)) {
		assert(ep->cur_rx.hdr_len == sizeof(ep->cur_rx.hdr.base_hdr));

		if (ep->cur_rx.hdr.base_hdr.hdr_size > TCPX_MAX_HDR) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
				"Payload offset is too large\n");
			return -FI_EIO;
		}
		ep->cur_rx.hdr_len = (size_t) ep->cur_rx.hdr.base_hdr.hdr_size;
		if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len)
			goto next_hdr;

	} else if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len) {
		return -FI_EAGAIN;
	}

	if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len)
		return -FI_EAGAIN;

	ep->hdr_bswap(&ep->cur_rx.hdr.base_hdr);
	assert(ep->cur_rx.hdr.base_hdr.id == ep->rx_id++);
	if (ep->cur_rx.hdr.base_hdr.op >= ARRAY_SIZE(tcpx_start_op)) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
			"Received invalid opcode\n");
		return -FI_EIO;
	}

	ep->cur_rx.data_left = ep->cur_rx.hdr.base_hdr.size -
			       ep->cur_rx.hdr.base_hdr.hdr_size;
	ep->cur_rx.handler = tcpx_start_op[ep->cur_rx.hdr.base_hdr.op];

	return ep->cur_rx.handler(ep);
}

void tcpx_progress_rx(struct tcpx_ep *ep)
{
	ssize_t ret;

	assert(ofi_mutex_held(&ep->lock));
	if (ep->state != TCPX_CONNECTED)
		return;

	do {
		if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len) {
			ret = tcpx_recv_hdr(ep);
		} else {
			ret = ep->cur_rx.handler(ep);
		}

	} while (!ret && ofi_bsock_readable(&ep->bsock));

	if (ret && !OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
		tcpx_ep_disable(ep, 0, NULL, 0);
}

void tcpx_progress_async(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *xfer;
	uint32_t done;

	assert(ofi_mutex_held(&ep->lock));
	if (ep->state != TCPX_CONNECTED)
		return;

	done = ofi_bsock_async_done(&tcpx_prov, &ep->bsock);
	while (!slist_empty(&ep->async_queue)) {
		xfer = container_of(ep->async_queue.head,
				    struct tcpx_xfer_entry, entry);
		if (ofi_val32_gt(xfer->async_index, done))
			break;

		slist_remove_head(&ep->async_queue);
		ep->report_success(ep, ep->util_ep.tx_cq, xfer);
		tcpx_free_tx(xfer);
	}
}

static bool tcpx_tx_pending(struct tcpx_ep *ep)
{
	return ep->cur_tx.entry || ofi_bsock_tosend(&ep->bsock);
}

static int tcpx_mod_epoll(struct tcpx_ep *ep, struct util_wait_fd *wait_fd)
{
	uint32_t events;
	int ret;

	assert(ofi_mutex_held(&ep->lock));
	if (ep->pollout_set) {
		events = (wait_fd->util_wait.wait_obj == FI_WAIT_FD) ?
			 (OFI_EPOLL_IN | OFI_EPOLL_OUT) : (POLLIN | POLLOUT);
	} else {
		events = (wait_fd->util_wait.wait_obj == FI_WAIT_FD) ?
			 OFI_EPOLL_IN : POLLIN;
	}

	ret = (wait_fd->util_wait.wait_obj == FI_WAIT_FD) ?
	      ofi_epoll_mod(wait_fd->epoll_fd, ep->bsock.sock, events,
			    &ep->util_ep.ep_fid.fid) :
	      ofi_pollfds_mod(wait_fd->pollfds, ep->bsock.sock, events,
			      &ep->util_ep.ep_fid.fid);
	if (ret)
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA,
			"epoll modify failed\n");

	return ret;
}

/* We may need to send data in response to received requests,
 * such as delivery complete acks or RMA read responses.  So,
 * even if this is the Rx CQ, we need to progress transmits.
 * We also need to keep the rx and tx epoll wait fd's in sync,
 * such that we ask for POLLOUT on both or neither.  This is
 * required in case they share the same wait set and underlying
 * epoll fd.  So we only maintain a single pollout_set state
 * variable rather than trying to track them independently.
 * The latter does not work if the epoll fd behind the tx
 * and rx CQs is the same fd.
 */
int tcpx_update_epoll(struct tcpx_ep *ep)
{
	struct util_wait_fd *rx_wait, *tx_wait;
	int ret;

	assert(ofi_mutex_held(&ep->lock));
	if ((tcpx_tx_pending(ep) && ep->pollout_set) ||
	    (!tcpx_tx_pending(ep) && !ep->pollout_set))
		return FI_SUCCESS;

	rx_wait = ep->util_ep.rx_cq ?
		  container_of(ep->util_ep.rx_cq->wait,
		  	       struct util_wait_fd, util_wait) : NULL;
	tx_wait = ep->util_ep.tx_cq ?
		  container_of(ep->util_ep.tx_cq->wait,
		  	       struct util_wait_fd, util_wait) : NULL;

	ep->pollout_set = tcpx_tx_pending(ep);
	ret = tcpx_mod_epoll(ep, rx_wait);
	if (!ret && rx_wait != tx_wait)
		ret = tcpx_mod_epoll(ep, tx_wait);

	if (ret)
		ep->pollout_set = false;
	return ret;
}

int tcpx_try_func(void *util_ep)
{
	struct tcpx_ep *ep;
	int ret;

	ep = container_of(util_ep, struct tcpx_ep, util_ep);
	ofi_mutex_lock(&ep->lock);
	if (ofi_bsock_readable(&ep->bsock)) {
		ret = -FI_EAGAIN;
	} else {
		ret = tcpx_update_epoll(ep);
	}
	ofi_mutex_unlock(&ep->lock);
	return ret;
}

void tcpx_tx_queue_insert(struct tcpx_ep *ep,
			  struct tcpx_xfer_entry *tx_entry)
{
	struct util_wait *rx_wait, *tx_wait;

	if (!ep->cur_tx.entry) {
		ep->cur_tx.entry = tx_entry;
		ep->cur_tx.data_left = tx_entry->hdr.base_hdr.size;
		OFI_DBG_SET(tx_entry->hdr.base_hdr.id, ep->tx_id++);
		ep->hdr_bswap(&tx_entry->hdr.base_hdr);
		tcpx_progress_tx(ep);

		/* Wake-up blocked threads if they need to add POLLOUT to
		 * their events to monitor for this socket.
		 */
		tx_wait = ep->util_ep.tx_cq->wait;
		rx_wait = ep->util_ep.rx_cq->wait;
		if (ep->cur_tx.entry) {
			if (tx_wait)
				tx_wait->signal(tx_wait);
			if (rx_wait && rx_wait != tx_wait)
				rx_wait->signal(rx_wait);
		}
	} else if (tx_entry->ctrl_flags & TCPX_INTERNAL_XFER) {
		slist_insert_tail(&tx_entry->entry, &ep->priority_queue);
	} else {
		slist_insert_tail(&tx_entry->entry, &ep->tx_queue);
	}
}

static ssize_t (*tcpx_start_op[ofi_op_write + 1])(struct tcpx_ep *ep) = {
	[ofi_op_msg] = tcpx_op_msg,
	[ofi_op_tagged] = tcpx_op_tagged,
	[ofi_op_read_req] = tcpx_op_read_req,
	[ofi_op_read_rsp] = tcpx_op_read_rsp,
	[ofi_op_write] = tcpx_op_write,
};
