/*
 * Copyright (c) 2017-2022 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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
#include "xnet.h"
#include <poll.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <ofi_util.h>
#include <ofi_iov.h>


static ssize_t (*xnet_start_op[ofi_op_write + 1])(struct xnet_ep *ep);

static struct ofi_sockapi xnet_sockapi_uring =
{
	.send = ofi_sockapi_send_uring,
	.sendv = ofi_sockapi_sendv_uring,
	.recv = ofi_sockapi_recv_socket,
	.recvv = ofi_sockapi_recvv_socket,
};

static struct ofi_sockapi xnet_sockapi_socket =
{
	.send = ofi_sockapi_send_socket,
	.sendv = ofi_sockapi_sendv_socket,
	.recv = ofi_sockapi_recv_socket,
	.recvv = ofi_sockapi_recvv_socket,
};


static bool xnet_save_and_cont(struct xnet_ep *ep)
{
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(ep->cur_rx.hdr.base_hdr.op == ofi_op_tagged);

	return (ep->saved_cnt < xnet_max_saved) &&
	       (ep->cur_rx.data_left <= XNET_MAX_INJECT);
}

static struct xnet_xfer_entry *
xnet_get_save_rx(struct xnet_ep *ep, uint64_t tag)
{
	struct xnet_progress *progress;
	struct xnet_xfer_entry *rx_entry;

	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));
	assert(xnet_save_and_cont(ep));
	assert(ep->cur_rx.hdr_done == ep->cur_rx.hdr_len &&
	       !ep->cur_rx.claim_ctx);

	FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "Saving msg tag 0x%zx src %zu\n",
	       tag, ep->peer->fi_addr);
	rx_entry = xnet_alloc_xfer(xnet_srx2_progress(ep->srx));
	if (!rx_entry)
		return NULL;

	rx_entry->ctrl_flags = XNET_SAVED_XFER;
	rx_entry->tag = tag;
	rx_entry->ignore = 0;
	rx_entry->src_addr = ep->peer->fi_addr;
	rx_entry->context = NULL;
	rx_entry->user_buf = NULL;
	rx_entry->iov_cnt = 1;
	rx_entry->iov[0].iov_base = &rx_entry->hdr.max_hdr[XNET_MAX_HDR];
	rx_entry->iov[0].iov_len = XNET_MAX_INJECT;

	slist_insert_tail(&rx_entry->entry, &ep->saved_queue);
	if (!ep->saved_cnt++) {
		assert(dlist_empty(&ep->saved_entry));
		dlist_insert_tail(&ep->saved_entry,
				  &progress->saved_tag_list);
	}

	return rx_entry;
}

void xnet_complete_saved(struct xnet_xfer_entry *saved_entry)
{
	struct xnet_ep *ep;
	struct xnet_progress *progress;
	size_t msg_len, copied;

	ep = saved_entry->ep;
	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));

	msg_len = (saved_entry->hdr.base_hdr.size -
		   saved_entry->hdr.base_hdr.hdr_size);
	FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "Completing saved msg "
	       "tag 0x%zx src %zu size %zu\n", saved_entry->tag,
	       saved_entry->src_addr, msg_len);

	if (msg_len) {
		copied = ofi_copy_iov_buf(saved_entry->iov,
				saved_entry->iov_cnt, 0,
				&saved_entry->hdr.max_hdr[XNET_MAX_HDR],
				msg_len, OFI_COPY_BUF_TO_IOV);
	} else {
		copied = 0;
	}

	if (copied == msg_len) {
		ep->report_success(ep, ep->util_ep.rx_cq, saved_entry);
	} else {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "saved recv truncated\n");
		xnet_cntr_incerr(ep, saved_entry);
		xnet_cq_report_error(ep->util_ep.rx_cq, saved_entry, FI_ETRUNC);
	}
	xnet_free_xfer(progress, saved_entry);
}

void xnet_recv_saved(struct xnet_xfer_entry *saved_entry,
		     struct xnet_xfer_entry *rx_entry)
{
	struct xnet_ep *ep;
	struct xnet_progress *progress;
	size_t msg_len, done_len;
	int ret;

	ep = saved_entry->ep;
	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));
	FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "recv matched saved msg "
	       "tag 0x%zx src %zu\n", saved_entry->tag, saved_entry->src_addr);

	saved_entry->ctrl_flags &= ~XNET_SAVED_XFER;
	saved_entry->context = rx_entry->context;
	saved_entry->user_buf = rx_entry->user_buf;
	saved_entry->cq_flags |= rx_entry->cq_flags;

	if (rx_entry->iov_cnt) {
		memcpy(&saved_entry->iov[0], &rx_entry->iov[0],
			rx_entry->iov_cnt * sizeof(rx_entry->iov[0]));
		saved_entry->iov_cnt = rx_entry->iov_cnt;
	}

	if (saved_entry != ep->cur_rx.entry) {
		xnet_complete_saved(saved_entry);
	/* TODO: need io_uring async recv posted check
	} else if (async recv posted using io_uring) {
		saved_entry->ctrl_flags |= XNET_COPY_RECV;
	*/
	} else {
		FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "saved msg still active "
		       "needs %zu bytes\n", ep->cur_rx.data_left);

		msg_len = (saved_entry->hdr.base_hdr.size -
			  saved_entry->hdr.base_hdr.hdr_size);
		done_len = msg_len - ep->cur_rx.data_left;
		assert(msg_len && ep->cur_rx.data_left);

		ret = ofi_truncate_iov(&saved_entry->iov[0],
				       &saved_entry->iov_cnt, msg_len);
		if (ret) {
			/* truncation failure */
			saved_entry->iov_cnt = 0;
			xnet_complete_saved(saved_entry);
		} else {
			(void) ofi_copy_iov_buf(saved_entry->iov,
					saved_entry->iov_cnt, 0,
					&saved_entry->hdr.max_hdr[XNET_MAX_HDR],
					done_len, OFI_COPY_BUF_TO_IOV);
			ofi_consume_iov(&saved_entry->iov[0],
					&saved_entry->iov_cnt, done_len);
		}
	}

	xnet_free_xfer(progress, rx_entry);
}

void xnet_update_pollflag(struct xnet_ep *ep, short pollflag, bool set)
{
	struct xnet_progress *progress;

	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));
	if (set) {
		if (ep->pollflags & pollflag)
			return;

		ep->pollflags |= pollflag;
	} else {
		if (!(ep->pollflags & pollflag))
			return;

		ep->pollflags &= ~pollflag;
	}

	ofi_dynpoll_mod(&progress->epoll_fd, ep->bsock.sock,
			ep->pollflags, &ep->util_ep.ep_fid.fid);
	xnet_signal_progress(progress);
}

static ssize_t xnet_send_msg(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret;
	size_t len;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
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
		tx_entry->ctrl_flags |= XNET_ASYNC;
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

static ssize_t xnet_recv_msg_data(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
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

static void xnet_progress_tx(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *tx_entry;
	struct xnet_cq *cq;
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	while (ep->cur_tx.entry) {
		ret = xnet_send_msg(ep);
		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret)) {
			xnet_update_pollflag(ep, POLLOUT, true);
			return;
		}

		tx_entry = ep->cur_tx.entry;
		cq = container_of(ep->util_ep.tx_cq, struct xnet_cq, util_cq);

		if (ret) {
			FI_WARN(&xnet_prov, FI_LOG_DOMAIN, "msg send failed\n");
			xnet_cntr_incerr(ep, tx_entry);
			xnet_cq_report_error(&cq->util_cq, tx_entry, (int) -ret);
			xnet_free_xfer(xnet_ep2_progress(ep), tx_entry);
		} else if (tx_entry->ctrl_flags & XNET_NEED_ACK) {
			/* A SW ack guarantees the peer received the data, so
			 * we can skip the async completion.
			 */
			slist_insert_tail(&tx_entry->entry,
					  &ep->need_ack_queue);
		} else if (tx_entry->ctrl_flags & XNET_NEED_RESP) {
			/* discard send but enable receive for completion */
			assert(tx_entry->resp_entry);
			tx_entry->resp_entry->ctrl_flags &= ~XNET_INTERNAL_XFER;
			xnet_free_xfer(xnet_ep2_progress(ep), tx_entry);
		} else if ((tx_entry->ctrl_flags & XNET_ASYNC) &&
			   (ofi_val32_gt(tx_entry->async_index,
					 ep->bsock.done_index))) {
			slist_insert_tail(&tx_entry->entry,
					  &ep->async_queue);
		} else {
			ep->report_success(ep, &cq->util_cq, tx_entry);
			xnet_free_xfer(xnet_ep2_progress(ep), tx_entry);
		}

		if (!slist_empty(&ep->priority_queue)) {
			ep->cur_tx.entry = container_of(slist_remove_head(
							&ep->priority_queue),
					     struct xnet_xfer_entry, entry);
			assert(ep->cur_tx.entry->ctrl_flags & XNET_INTERNAL_XFER);
		} else if (!slist_empty(&ep->tx_queue)) {
			ep->cur_tx.entry = container_of(slist_remove_head(
							&ep->tx_queue),
					     struct xnet_xfer_entry, entry);
			assert(!(ep->cur_tx.entry->ctrl_flags & XNET_INTERNAL_XFER));
		} else {
			ep->cur_tx.entry = NULL;
			break;
		}

		ep->cur_tx.data_left = ep->cur_tx.entry->hdr.base_hdr.size;
		OFI_DBG_SET(ep->cur_tx.entry->hdr.base_hdr.id, ep->tx_id++);
		ep->hdr_bswap(ep, &ep->cur_tx.entry->hdr.base_hdr);
	}

	/* Buffered data is sent first by xnet_send_msg, but if we don't
	 * have other data to send, we need to try flushing any buffered data.
	 */
	(void) ofi_bsock_flush(&ep->bsock);
	xnet_update_pollflag(ep, POLLOUT, ofi_bsock_tosend(&ep->bsock));
}

static int xnet_queue_ack(struct xnet_xfer_entry *rx_entry)
{
	struct xnet_xfer_entry *resp;

	assert(xnet_progress_locked(xnet_ep2_progress(rx_entry->ep)));
	resp = xnet_alloc_xfer(xnet_ep2_progress(rx_entry->ep));
	if (!resp)
		return -FI_ENOMEM;

	resp->iov[0].iov_base = (void *) &resp->hdr;
	resp->iov[0].iov_len = sizeof(resp->hdr.base_hdr);
	resp->iov_cnt = 1;

	resp->hdr.base_hdr.version = XNET_HDR_VERSION;
	resp->hdr.base_hdr.op_data = XNET_OP_ACK;
	resp->hdr.base_hdr.op = ofi_op_msg;
	resp->hdr.base_hdr.size = sizeof(resp->hdr.base_hdr);
	resp->hdr.base_hdr.hdr_size = (uint8_t) sizeof(resp->hdr.base_hdr);

	resp->ctrl_flags = XNET_INTERNAL_XFER;
	resp->context = NULL;
	resp->ep = rx_entry->ep;

	xnet_tx_queue_insert(rx_entry->ep, resp);
	return FI_SUCCESS;
}

static ssize_t xnet_process_recv(struct xnet_ep *ep)
{
	struct xnet_progress *progress;
	struct xnet_xfer_entry *rx_entry;
	ssize_t ret;

	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));
	rx_entry = ep->cur_rx.entry;
	ret = xnet_recv_msg_data(ep);
	if (ret) {
		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
			return ret;

		goto err;
	}

	if (rx_entry->hdr.base_hdr.flags & XNET_DELIVERY_COMPLETE) {
		ret = xnet_queue_ack(rx_entry);
		if (ret)
			goto err;
	}

	if (!(rx_entry->ctrl_flags & XNET_SAVED_XFER)) {
		ep->report_success(ep, ep->util_ep.rx_cq, rx_entry);
		xnet_free_xfer(progress, rx_entry);
	}
	xnet_reset_rx(ep);
	return 0;

err:
	FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
		"msg recv failed ret = %zd (%s)\n", ret, fi_strerror((int)-ret));
	xnet_cntr_incerr(ep, rx_entry);
	xnet_cq_report_error(rx_entry->ep->util_ep.rx_cq, rx_entry, (int) -ret);
	xnet_free_xfer(progress, rx_entry);
	xnet_reset_rx(ep);
	return ret;
}

static void xnet_pmem_commit(struct xnet_xfer_entry *rx_entry)
{
	struct ofi_rma_iov *rma_iov;
	size_t offset;
	int i;

	assert(xnet_progress_locked(xnet_ep2_progress(rx_entry->ep)));
	if (!ofi_pmem_commit)
		return ;

	if (rx_entry->hdr.base_hdr.flags & XNET_REMOTE_CQ_DATA)
		offset = sizeof(rx_entry->hdr.base_hdr) + sizeof(uint64_t);
	else
		offset = sizeof(rx_entry->hdr.base_hdr);


	rma_iov = (struct ofi_rma_iov *)((uint8_t *)&rx_entry->hdr + offset);

	for (i = 0; i < rx_entry->hdr.base_hdr.rma_iov_cnt; i++) {
		(*ofi_pmem_commit)((const void *) (uintptr_t) rma_iov[i].addr,
				   rma_iov[i].len);
	}
}

static ssize_t xnet_process_remote_write(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	rx_entry = ep->cur_rx.entry;
	ret = xnet_recv_msg_data(ep);
	if (ret) {
		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
			return ret;

		goto err;
	}

	if (rx_entry->hdr.base_hdr.flags &
	    (XNET_DELIVERY_COMPLETE | XNET_COMMIT_COMPLETE)) {

		if (rx_entry->hdr.base_hdr.flags & XNET_COMMIT_COMPLETE)
			xnet_pmem_commit(rx_entry);

		ret = xnet_queue_ack(rx_entry);
		if (ret)
			goto err;
	}

	ep->report_success(ep, ep->util_ep.rx_cq, rx_entry);
	xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	xnet_reset_rx(ep);
	return FI_SUCCESS;

err:
	FI_WARN(&xnet_prov, FI_LOG_DOMAIN, "remote write failed %zd\n", ret);
	xnet_cntr_incerr(ep, rx_entry);
	xnet_cq_report_error(rx_entry->ep->util_ep.rx_cq, rx_entry, (int) -ret);
	xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	xnet_reset_rx(ep);
	return ret;
}

static ssize_t xnet_process_remote_read(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct xnet_cq *cq;
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	rx_entry = ep->cur_rx.entry;
	cq = container_of(ep->util_ep.tx_cq, struct xnet_cq, util_cq);

	ret = xnet_recv_msg_data(ep);
	if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
		return ret;

	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_DOMAIN,
			"msg recv Failed ret = %zd\n", ret);
		xnet_cntr_incerr(ep, rx_entry);
		xnet_cq_report_error(&cq->util_cq, rx_entry, (int) -ret);
	} else {
		ep->report_success(ep, &cq->util_cq, rx_entry);
	}

	slist_remove_head(&rx_entry->ep->rma_read_queue);
	xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	xnet_reset_rx(ep);
	return ret;
}

static int xnet_alter_mrecv(struct xnet_ep *ep, struct xnet_xfer_entry *xfer,
			    size_t msg_len)
{
	struct xnet_xfer_entry *recv_entry;
	size_t left;
	int ret = FI_SUCCESS;

	assert(ep->srx);
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));

	if ((msg_len && !xfer->iov_cnt) || (msg_len > xfer->iov[0].iov_len)) {
		ret = -FI_ETRUNC;
		goto complete;
	}

	left = xfer->iov[0].iov_len - msg_len;
	if (!xfer->iov_cnt || (left < ep->srx->min_multi_recv_size))
		goto complete;

	/* If we can't repost the remaining buffer, return it to the user. */
	recv_entry = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (!recv_entry)
		goto complete;

	recv_entry->ctrl_flags = XNET_MULTI_RECV;
	recv_entry->cq_flags = FI_MSG | FI_RECV;
	recv_entry->cntr_inc = ofi_ep_rx_cntr_inc;
	recv_entry->context = xfer->context;

	recv_entry->iov_cnt = 1;
	recv_entry->user_buf =  (char *) xfer->iov[0].iov_base + msg_len;
	recv_entry->iov[0].iov_base = recv_entry->user_buf;
	recv_entry->iov[0].iov_len = left;

	slist_insert_head(&recv_entry->entry, &ep->srx->rx_queue);
	return 0;

complete:
	xfer->cq_flags |= FI_MULTI_RECV;
	return ret;
}

static struct xnet_xfer_entry *xnet_get_rx_entry(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *xfer;
	struct xnet_srx *srx;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (ep->srx) {
		srx = ep->srx;
		if (!slist_empty(&srx->rx_queue)) {
			xfer = container_of(slist_remove_head(&srx->rx_queue),
					    struct xnet_xfer_entry, entry);
		} else {
			xfer = NULL;
		}
	} else {
		if (!slist_empty(&ep->rx_queue)) {
			xfer = container_of(slist_remove_head(&ep->rx_queue),
					    struct xnet_xfer_entry, entry);
			ep->rx_avail++;
		} else {
			xfer = NULL;
		}
	}

	return xfer;
}

static int xnet_handle_ack(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *tx_entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (ep->cur_rx.hdr.base_hdr.size !=
	    sizeof(ep->cur_rx.hdr.base_hdr))
		return -FI_EIO;

	assert(!slist_empty(&ep->need_ack_queue));
	tx_entry = container_of(slist_remove_head(&ep->need_ack_queue),
				struct xnet_xfer_entry, entry);

	ep->report_success(ep, ep->util_ep.tx_cq, tx_entry);
	xnet_free_xfer(xnet_ep2_progress(ep), tx_entry);
	xnet_reset_rx(ep);
	return FI_SUCCESS;
}

ssize_t xnet_start_recv(struct xnet_ep *ep, struct xnet_xfer_entry *rx_entry)
{
	struct xnet_active_rx *msg = &ep->cur_rx;
	size_t msg_len;
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (!dlist_empty(&ep->unexp_entry)) {
		dlist_remove_init(&ep->unexp_entry);
		xnet_update_pollflag(ep, POLLIN, true);
	}

	msg_len = (msg->hdr.base_hdr.size - msg->hdr.base_hdr.hdr_size);

	rx_entry->cq_flags |= xnet_rx_completion_flag(ep);
	memcpy(&rx_entry->hdr, &msg->hdr,
	       (size_t) msg->hdr.base_hdr.hdr_size);
	rx_entry->ep = ep;

	if (rx_entry->ctrl_flags & XNET_MULTI_RECV) {
		assert(msg->hdr.base_hdr.op == ofi_op_msg);
		ret = xnet_alter_mrecv(ep, rx_entry, msg_len);
		if (ret)
			goto truncate_err;
	}

	ret = ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_cnt,
				msg_len);
	if (ret)
		goto truncate_err;

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = xnet_process_recv;
	return xnet_process_recv(ep);

truncate_err:
	FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
		"posted rx buffer size is not big enough\n");
	xnet_cntr_incerr(ep, rx_entry);
	xnet_cq_report_error(rx_entry->ep->util_ep.rx_cq, rx_entry, (int) -ret);
	xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	return ret;
}

static ssize_t xnet_op_msg(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct xnet_active_rx *msg = &ep->cur_rx;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (msg->hdr.base_hdr.op_data == XNET_OP_ACK)
		return xnet_handle_ack(ep);

	rx_entry = xnet_get_rx_entry(ep);
	if (!rx_entry) {
		if (dlist_empty(&ep->unexp_entry)) {
			dlist_insert_tail(&ep->unexp_entry,
					  &xnet_ep2_progress(ep)->unexp_msg_list);
			xnet_update_pollflag(ep, POLLIN, false);
		}
		return -FI_EAGAIN;
	}

	return xnet_start_recv(ep, rx_entry);
}

static ssize_t xnet_op_tagged(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct xnet_active_rx *msg = &ep->cur_rx;
	uint64_t tag;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(ep->srx);

	tag = (msg->hdr.base_hdr.flags & XNET_REMOTE_CQ_DATA) ?
	      msg->hdr.tag_data_hdr.tag : msg->hdr.tag_hdr.tag;

	rx_entry = ep->srx->match_tag_rx(ep->srx, ep, tag);
	if (!rx_entry) {
		if (xnet_save_and_cont(ep)) {
			rx_entry = xnet_get_save_rx(ep, tag);
			if (rx_entry)
				goto start;
		}
		if (dlist_empty(&ep->unexp_entry)) {
			dlist_insert_tail(&ep->unexp_entry,
					  &xnet_ep2_progress(ep)->unexp_tag_list);
			xnet_update_pollflag(ep, POLLIN, false);
		}
		return -FI_EAGAIN;
	}

start:
	return xnet_start_recv(ep, rx_entry);
}

static ssize_t xnet_op_read_req(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *resp;
	struct ofi_rma_iov *rma_iov;
	ssize_t i, ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	resp = xnet_alloc_xfer(xnet_ep2_progress(ep));
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
			FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
			       "invalid rma iov received\n");
			xnet_free_xfer(xnet_ep2_progress(ep), resp);
			return ret;
		}

		resp->iov[i + 1].iov_base = (void *) (uintptr_t)
					    rma_iov[i].addr;
		resp->iov[i + 1].iov_len = rma_iov[i].len;
		resp->hdr.base_hdr.size += resp->iov[i + 1].iov_len;
	}

	resp->hdr.base_hdr.op = ofi_op_read_rsp;
	resp->hdr.base_hdr.hdr_size = (uint8_t) sizeof(resp->hdr.base_hdr);

	resp->ctrl_flags = XNET_INTERNAL_XFER;
	resp->context = NULL;

	xnet_tx_queue_insert(ep, resp);
	xnet_reset_rx(ep);
	return FI_SUCCESS;
}

static ssize_t xnet_op_write(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct ofi_rma_iov *rma_iov;
	ssize_t ret, i;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	rx_entry = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (!rx_entry)
		return -FI_ENOMEM;

	if (ep->cur_rx.hdr.base_hdr.flags & XNET_REMOTE_CQ_DATA) {
		rx_entry->cq_flags = (FI_COMPLETION | FI_REMOTE_WRITE |
				      FI_REMOTE_CQ_DATA);
		rma_iov = (struct ofi_rma_iov *) ((uint8_t *) &rx_entry->hdr +
			   sizeof(rx_entry->hdr.cq_data_hdr));
	} else {
		rx_entry->ctrl_flags = XNET_INTERNAL_XFER;
		rma_iov = (struct ofi_rma_iov *) ((uint8_t *) &rx_entry->hdr +
			  sizeof(rx_entry->hdr.base_hdr));
	}
	rx_entry->cntr_inc = ofi_ep_rem_wr_cntr_inc;

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
			FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
			       "invalid rma iov received\n");
			xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
			return ret;
		}
		rx_entry->iov[i].iov_base = (void *) (uintptr_t)
					      rma_iov[i].addr;
		rx_entry->iov[i].iov_len = rma_iov[i].len;
	}

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = xnet_process_remote_write;
	return xnet_process_remote_write(ep);
}

static ssize_t xnet_op_read_rsp(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct slist_entry *entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (slist_empty(&ep->rma_read_queue))
		return -FI_EINVAL;

	entry = ep->rma_read_queue.head;
	rx_entry = container_of(entry, struct xnet_xfer_entry, entry);

	memcpy(&rx_entry->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	rx_entry->hdr.base_hdr.op_data = 0;

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = xnet_process_remote_read;
	return xnet_process_remote_read(ep);
}

static ssize_t xnet_recv_hdr(struct xnet_ep *ep)
{
	size_t len;
	void *buf;
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
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

		if (ep->cur_rx.hdr.base_hdr.hdr_size > XNET_MAX_HDR) {
			FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
				"Payload offset is too large\n");
			return -FI_EIO;
		}
		ep->cur_rx.hdr_len = (size_t) ep->cur_rx.hdr.base_hdr.hdr_size;
		if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len)
			goto next_hdr;

	} else if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len) {
		return -FI_EAGAIN;
	}

	ep->hdr_bswap(ep, &ep->cur_rx.hdr.base_hdr);
	assert(ep->cur_rx.hdr.base_hdr.id == ep->rx_id++);
	if (ep->cur_rx.hdr.base_hdr.op >= ARRAY_SIZE(xnet_start_op)) {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
			"Received invalid opcode\n");
		return -FI_EIO;
	}

	ep->cur_rx.data_left = ep->cur_rx.hdr.base_hdr.size -
			       ep->cur_rx.hdr.base_hdr.hdr_size;
	ep->cur_rx.handler = xnet_start_op[ep->cur_rx.hdr.base_hdr.op];

	return ep->cur_rx.handler(ep);
}

void xnet_progress_rx(struct xnet_ep *ep)
{
	ssize_t ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	do {
		if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len) {
			ret = xnet_recv_hdr(ep);
		} else {
			ret = ep->cur_rx.handler(ep);
		}

	} while (!ret && ofi_bsock_readable(&ep->bsock));

	if (ret && !OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
		xnet_ep_disable(ep, 0, NULL, 0);
}

void xnet_progress_async(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *xfer;
	uint32_t done;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	done = ofi_bsock_async_done(&xnet_prov, &ep->bsock);
	while (!slist_empty(&ep->async_queue)) {
		xfer = container_of(ep->async_queue.head,
				    struct xnet_xfer_entry, entry);
		if (ofi_val32_gt(xfer->async_index, done))
			break;

		slist_remove_head(&ep->async_queue);
		ep->report_success(ep, ep->util_ep.tx_cq, xfer);
		xnet_free_xfer(xnet_ep2_progress(ep), xfer);
	}
}

void xnet_tx_queue_insert(struct xnet_ep *ep,
			  struct xnet_xfer_entry *tx_entry)
{
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));

	if (!ep->cur_tx.entry) {
		ep->cur_tx.entry = tx_entry;
		ep->cur_tx.data_left = tx_entry->hdr.base_hdr.size;
		OFI_DBG_SET(tx_entry->hdr.base_hdr.id, ep->tx_id++);
		ep->hdr_bswap(ep, &tx_entry->hdr.base_hdr);
		xnet_progress_tx(ep);
	} else if (tx_entry->ctrl_flags & XNET_INTERNAL_XFER) {
		slist_insert_tail(&tx_entry->entry, &ep->priority_queue);
	} else {
		slist_insert_tail(&tx_entry->entry, &ep->tx_queue);
	}
}

static ssize_t (*xnet_start_op[ofi_op_write + 1])(struct xnet_ep *ep) = {
	[ofi_op_msg] = xnet_op_msg,
	[ofi_op_tagged] = xnet_op_tagged,
	[ofi_op_read_req] = xnet_op_read_req,
	[ofi_op_read_rsp] = xnet_op_read_rsp,
	[ofi_op_write] = xnet_op_write,
};

static void xnet_run_ep(struct xnet_ep *ep, bool pin, bool pout, bool perr)
{
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	switch (ep->state) {
	case XNET_CONNECTED:
		if (perr)
			xnet_progress_async(ep);
		if (pin)
			xnet_progress_rx(ep);
		if (pout)
			xnet_progress_tx(ep);
		break;
	case XNET_CONNECTING:
		xnet_connect_done(ep);
		break;
	case XNET_REQ_SENT:
		xnet_req_done(ep);
		break;
	default:
		break;
	};
}

static void xnet_progress_cqe(struct xnet_uring *uring,
			      ofi_io_uring_cqe_t *cqe)
{
	struct ofi_bsock *bsock;

	assert(xnet_io_uring);
	bsock = (struct ofi_bsock *) cqe->user_data;
	if (!bsock)
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "io_uring error\n");
}

static void xnet_progress_uring(struct xnet_uring *uring)
{
	ofi_io_uring_cqe_t *cqes[XNET_MAX_EVENTS];
	int nready;
	int i;

	assert(xnet_io_uring);

	nready = ofi_uring_peek_batch_cqe(&uring->ring, cqes, XNET_MAX_EVENTS);
	if (!nready)
		return;

	assert(nready <= XNET_MAX_EVENTS);
	for (i = 0; i < nready; i++) {
		uring->credits++;
		xnet_progress_cqe(uring, cqes[i]);
	}

	ofi_uring_cq_advance(&uring->ring, nready);
}

static void
xnet_handle_events(struct xnet_progress *progress,
		   struct ofi_epollfds_event *events, int nfds,
		   bool clear_signal)
{
	struct fid *fid;
	bool pin, pout, perr;
	int i;

	assert(ofi_genlock_held(progress->active_lock));
	for (i = 0; i < nfds; i++) {
		fid = events[i].data.ptr;
		assert(fid);

		pin = events[i].events & POLLIN;
		pout = events[i].events & POLLOUT;
		perr = events[i].events & POLLERR;

		switch (fid->fclass) {
		case FI_CLASS_EP:
			xnet_run_ep(events[i].data.ptr, pin, pout, perr);
			break;
		case FI_CLASS_PEP:
			xnet_accept_sock(events[i].data.ptr);
			break;
		case FI_CLASS_CONNREQ:
			xnet_run_conn(events[i].data.ptr, pin, pout, perr);
			break;
		case XNET_CLASS_URING:
			xnet_progress_uring(events[i].data.ptr);
			break;
		default:
			assert(fid->fclass == XNET_CLASS_PROGRESS);
			if (clear_signal)
				fd_signal_reset(&progress->signal);
			break;
		}
	}

	xnet_handle_event_list(progress);
}

void xnet_progress_unexp(struct xnet_progress *progress)
{
	struct dlist_entry *item, *tmp;
	struct xnet_ep *ep;

	assert(ofi_genlock_held(progress->active_lock));
	dlist_foreach_safe(&progress->unexp_tag_list, item, tmp) {
		ep = container_of(item, struct xnet_ep, unexp_entry);
		assert(xnet_has_unexp(ep));
		assert(ep->state == XNET_CONNECTED);
		xnet_progress_rx(ep);
	}
}

void xnet_run_progress(struct xnet_progress *progress, bool clear_signal)
{
	struct ofi_epollfds_event events[XNET_MAX_EVENTS];
	int nfds;

	assert(ofi_genlock_held(progress->active_lock));
	nfds = ofi_dynpoll_wait(&progress->epoll_fd, events,
				XNET_MAX_EVENTS, 0);
	xnet_handle_events(progress, events, nfds, clear_signal);
}

void xnet_progress(struct xnet_progress *progress, bool clear_signal)
{
	ofi_genlock_lock(progress->active_lock);
	xnet_run_progress(progress, clear_signal);
	ofi_genlock_unlock(progress->active_lock);
}

void xnet_progress_all(struct xnet_fabric *fabric)
{
	struct xnet_domain *domain;
	struct dlist_entry *item;

	ofi_mutex_lock(&fabric->util_fabric.lock);
	dlist_foreach(&fabric->util_fabric.domain_list, item) {
		domain = container_of(item, struct xnet_domain,
				      util_domain.list_entry);
		xnet_progress(&domain->progress, false);
	}

	ofi_mutex_unlock(&fabric->util_fabric.lock);

	xnet_progress(&fabric->progress, false);
}

/* The epoll fd is updated dynamically for polling/pollout events on the
 * attached sockets as needed.  There's one possible issue around data
 * that's been buffered on the bsock byteq.  In that case, we have data
 * ready to be received, but the pollin event will not be set.  However,
 * when this occurs, it's an indication that we have an unexpected message
 * that the application needs to post a receive buffer for.  Whether we
 * allow the app to block on the epoll fd is mostly irrelevant.  Doing so
 * doesn't contribute to progress being stalled.  We're stalled until the
 * app posts the receive buffer.  This could come from another thread, or
 * the app might not post the buffer until it receives data from some
 * other peer.  (The latter occurs with MPI, though MPI doesn't usually
 * use blocking calls.)
 */
int xnet_trywait(struct fid_fabric *fabric_fid, struct fid **fid, int count)
{
	return 0;
}

/* We can't hold the progress lock around waiting, or we
 * can hang another thread trying to obtain the lock.  But
 * the poll fds may change while we're waiting for an event.
 * To avoid possibly processing an event for an object that
 * we just removed from the poll fds, which could access freed
 * memory, we must re-acquire the progress lock and re-read
 * any queued events before processing it.
 */
int xnet_progress_wait(struct xnet_progress *progress, int timeout)
{
	struct ofi_epollfds_event event;

	return ofi_dynpoll_wait(&progress->epoll_fd, &event, 1, timeout);
}

static void *xnet_auto_progress(void *arg)
{
	struct xnet_progress *progress = arg;
	int nfds;

	FI_INFO(&xnet_prov, FI_LOG_DOMAIN, "progress thread starting\n");
	ofi_genlock_lock(progress->active_lock);
	while (progress->auto_progress) {
		ofi_genlock_unlock(progress->active_lock);

		nfds = xnet_progress_wait(progress, -1);
		ofi_genlock_lock(progress->active_lock);
		if (nfds >= 0)
			xnet_run_progress(progress, true);
	}
	ofi_genlock_unlock(progress->active_lock);
	FI_INFO(&xnet_prov, FI_LOG_DOMAIN, "progress thread exiting\n");
	return NULL;
}

int xnet_monitor_sock(struct xnet_progress *progress, SOCKET sock,
		      uint32_t events, struct fid *fid)
{
	int ret;

	assert(xnet_progress_locked(progress));
	ret = ofi_dynpoll_add(&progress->epoll_fd, sock, events, fid);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"Failed to add fd to progress\n");
	}
	return ret;
}

/* May be called from progress thread to disable endpoint. */
void xnet_halt_sock(struct xnet_progress *progress, SOCKET sock)
{
	int ret;

	assert(xnet_progress_locked(progress));
	ret = ofi_dynpoll_del(&progress->epoll_fd, sock);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"Failed to del fd from progress\n");
	}
}

int xnet_start_progress(struct xnet_progress *progress)
{
	int ret;

	if (xnet_disable_autoprog)
		return 0;

	ofi_genlock_lock(progress->active_lock);
	if (progress->auto_progress) {
		ret = 0;
		goto unlock;
	}

	progress->auto_progress = true;
	ret = pthread_create(&progress->thread, NULL, xnet_auto_progress,
			     progress);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_DOMAIN,
			"unable to start progress thread\n");
		progress->auto_progress = false;
		ret = -ret;
	}

unlock:
	ofi_genlock_unlock(progress->active_lock);
	return ret;
}

int xnet_start_all(struct xnet_fabric *fabric)
{
	struct xnet_domain *domain;
	struct dlist_entry *item;
	int ret;

	ret = xnet_start_progress(&fabric->progress);
	if (ret)
		return ret;

	ofi_mutex_lock(&fabric->util_fabric.lock);
	dlist_foreach(&fabric->util_fabric.domain_list, item) {
		domain = container_of(item, struct xnet_domain,
				      util_domain.list_entry);
		ret = xnet_start_progress(&domain->progress);
		if (ret)
			break;
	}

	ofi_mutex_unlock(&fabric->util_fabric.lock);
	return ret;
}

void xnet_stop_progress(struct xnet_progress *progress)
{
	ofi_genlock_lock(progress->active_lock);
	if (!progress->auto_progress) {
		ofi_genlock_unlock(progress->active_lock);
		return;
	}

	progress->auto_progress = false;
	fd_signal_set(&progress->signal);
	ofi_genlock_unlock(progress->active_lock);
	(void) pthread_join(progress->thread, NULL);
}

/* Because we may need to start the progress thread to support blocking CQ
 * or EQ calls, we always need to enable an active lock, independent from
 * the threading model requested by the app.
 */
static int xnet_init_locks(struct xnet_progress *progress, struct fi_info *info)
{
	enum ofi_lock_type base_type, rdm_type;
	int ret;

	if (info && info->ep_attr && info->ep_attr->type == FI_EP_RDM) {
		base_type = OFI_LOCK_NONE;
		rdm_type = OFI_LOCK_MUTEX;
		progress->active_lock = &progress->rdm_lock;
	} else {
		base_type = OFI_LOCK_MUTEX;
		rdm_type = OFI_LOCK_NONE;
		progress->active_lock = &progress->lock;
	}

	ret = ofi_genlock_init(&progress->lock, base_type);
	if (ret)
		return ret;

	ret = ofi_genlock_init(&progress->rdm_lock, rdm_type);
	if (ret)
		ofi_genlock_destroy(&progress->lock);

	return ret;
}

static int xnet_init_uring(struct xnet_uring *uring, size_t entries,
			   struct ofi_dynpoll *dynpoll)
{
	int ret;

	ret = ofi_uring_init(&uring->ring, entries);
	if (ret)
		return ret;

	uring->fid.fclass = XNET_CLASS_URING;
	/* The number of entries may be higher than requested */
	uring->credits = ofi_uring_sq_space_left(&uring->ring);

	ret = ofi_dynpoll_add(dynpoll,
			      ofi_uring_get_fd(&uring->ring),
			      POLLIN, &uring->fid);
	if (ret)
		(void) ofi_uring_destroy(&uring->ring);

	return ret;
}

static void xnet_destroy_uring(struct xnet_uring *uring,
			       struct ofi_dynpoll *dynpoll)
{
	int ret;

	assert(xnet_io_uring);
	ofi_dynpoll_del(dynpoll, ofi_uring_get_fd(&uring->ring));
	assert(ofi_uring_sq_space_left(&uring->ring) == uring->credits);
	ret = ofi_uring_destroy(&uring->ring);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"Failed to destroy io_uring\n");
	}
}

int xnet_init_progress(struct xnet_progress *progress, struct fi_info *info)
{
	int ret;

	progress->fid.fclass = XNET_CLASS_PROGRESS;
	progress->auto_progress = false;
	dlist_init(&progress->unexp_msg_list);
	dlist_init(&progress->unexp_tag_list);
	dlist_init(&progress->saved_tag_list);
	slist_init(&progress->event_list);

	ret = fd_signal_init(&progress->signal);
	if (ret)
		return ret;

	ret = xnet_init_locks(progress, info);
	if (ret)
		goto err1;

	/* We may expose epoll fd to app, need a lock. */
	ret = ofi_dynpoll_create(&progress->epoll_fd, OFI_DYNPOLL_EPOLL,
				 OFI_LOCK_MUTEX);
	if (ret)
		goto err2;

	ret = ofi_bufpool_create(&progress->xfer_pool,
				 sizeof(struct xnet_xfer_entry), 16, 0,
				 1024, 0);
	if (ret)
		goto err3;

	ret = ofi_dynpoll_add(&progress->epoll_fd, progress->signal.fd[FI_READ_FD],
			      POLLIN, &progress->fid);
	if (ret)
		goto err4;

	if (xnet_io_uring) {
		ret = xnet_init_uring(&progress->tx_uring,
				      info ? info->tx_attr->size :
					     xnet_default_tx_size,
				      &progress->epoll_fd);
		if (ret)
			goto err5;

		ret = xnet_init_uring(&progress->rx_uring,
				      info ? info->rx_attr->size :
					     xnet_default_rx_size,
				      &progress->epoll_fd);
		if (ret)
			goto err6;

		progress->sockapi = xnet_sockapi_uring;
		progress->sockapi.tx_io_uring = &progress->tx_uring.ring;
		progress->sockapi.rx_io_uring = &progress->rx_uring.ring;
	} else {
		progress->sockapi = xnet_sockapi_socket;
	}

	return 0;
err6:
	xnet_destroy_uring(&progress->tx_uring, &progress->epoll_fd);
err5:
	ofi_dynpoll_del(&progress->epoll_fd, progress->signal.fd[FI_READ_FD]);
err4:
	ofi_bufpool_destroy(progress->xfer_pool);
err3:
	ofi_dynpoll_close(&progress->epoll_fd);
err2:
	ofi_genlock_destroy(&progress->rdm_lock);
	ofi_genlock_destroy(&progress->lock);
err1:
	fd_signal_free(&progress->signal);
	return ret;
}

void xnet_close_progress(struct xnet_progress *progress)
{
	assert(dlist_empty(&progress->unexp_msg_list));
	assert(dlist_empty(&progress->unexp_tag_list));
	assert(dlist_empty(&progress->saved_tag_list));
	assert(slist_empty(&progress->event_list));
	xnet_stop_progress(progress);
	if (xnet_io_uring) {
		xnet_destroy_uring(&progress->rx_uring, &progress->epoll_fd);
		xnet_destroy_uring(&progress->tx_uring, &progress->epoll_fd);
	}
	ofi_dynpoll_close(&progress->epoll_fd);
	ofi_bufpool_destroy(progress->xfer_pool);
	ofi_genlock_destroy(&progress->lock);
	ofi_genlock_destroy(&progress->rdm_lock);
	fd_signal_free(&progress->signal);
}
