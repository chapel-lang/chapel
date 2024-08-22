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


static int (*xnet_start_op[xnet_op_max])(struct xnet_ep *ep);

static struct ofi_sockapi xnet_sockapi_uring =
{
	.connect = ofi_sockapi_connect_uring,
	.accept = ofi_sockapi_accept_uring,
	.send = ofi_sockapi_send_uring,
	.sendv = ofi_sockapi_sendv_uring,
	.recv = ofi_sockapi_recv_uring,
	.recvv = ofi_sockapi_recvv_uring,
};

static struct ofi_sockapi xnet_sockapi_socket =
{
	.connect = ofi_sockapi_connect_socket,
	.accept = ofi_sockapi_accept_socket,
	.send = ofi_sockapi_send_socket,
	.sendv = ofi_sockapi_sendv_socket,
	.recv = ofi_sockapi_recv_socket,
	.recvv = ofi_sockapi_recvv_socket,
};

static void xnet_submit_uring(struct xnet_uring *uring)
{
	int submitted;
	int ready;

	assert(xnet_io_uring);

	ready = ofi_uring_sq_ready(&uring->ring);
	if (!ready)
		return;

	submitted = ofi_uring_submit(&uring->ring);
	(void) submitted; /* avoid unused variable warning */
	assert(ready == submitted);
}

static bool xnet_save_and_cont(struct xnet_ep *ep)
{
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(ep->cur_rx.hdr.base_hdr.op == xnet_op_tag ||
	       ep->cur_rx.hdr.base_hdr.op == xnet_op_tag_rts);
	assert(ep->srx);

	if ((ep->cur_rx.data_left > xnet_max_saved_size) ||
	    (ep->peer->fi_addr == FI_ADDR_NOTAVAIL))
		return false;

	if (!ep->saved_msg) {
		ep->saved_msg = ofi_array_at(&ep->srx->saved_msgs,
					     ep->peer->fi_addr);
		if (!ep->saved_msg)
			return false;
	}

	return (ep->saved_msg->cnt < xnet_max_saved);
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

	rx_entry->saving_ep = ep;
	rx_entry->tag = tag;
	rx_entry->ignore = 0;
	rx_entry->ctrl_flags = XNET_SAVED_XFER;

	if (ep->cur_rx.data_left <= xnet_buf_size) {
		rx_entry->user_buf = NULL;
		rx_entry->iov[0].iov_base = &rx_entry->msg_data;
		rx_entry->iov[0].iov_len = xnet_buf_size;
		rx_entry->iov_cnt = 1;
	} else if (xnet_alloc_xfer_buf(rx_entry, ep->cur_rx.data_left)) {
		goto free_xfer;
	}

	slist_insert_tail(&rx_entry->entry, &ep->saved_msg->queue);
	if (!ep->saved_msg->cnt++) {
		assert(dlist_empty(&ep->saved_msg->entry));
		dlist_insert_tail(&ep->saved_msg->entry,
				  &progress->saved_tag_list);
	}

	xnet_prof_unexp_msg(ep->profile, 1);

	return rx_entry;

free_xfer:
	xnet_free_xfer(progress, rx_entry);
	return NULL;
}

static int xnet_handle_truncate(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	int ret;

	FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "msg recv truncated\n");
	assert(ep->cur_rx.entry);
	assert(ep->cur_rx.data_left);

	rx_entry = ep->cur_rx.entry;
	xnet_cntr_incerr(rx_entry);
	/* TODO: need to report received message size =
	 * base_hdr.size - base_hdr.hdr_size - data_left
	 */
	xnet_report_error(rx_entry, FI_ETRUNC);

	/* Prepare to remove excess msg data from stream to continue */
	ep->cur_rx.claim_ctx = (void *) (uintptr_t) -1;
	rx_entry->cq_flags = 0;
	rx_entry->ctrl_flags = XNET_CLAIM_RECV | XNET_INTERNAL_XFER;
	ret = xnet_alloc_xfer_buf(rx_entry, ep->cur_rx.data_left);
	if (ret) {
		xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
		xnet_reset_rx(ep);
		return ret;
	}

	return 0;
}

static int xnet_queue_ack(struct xnet_ep *ep, uint8_t op, uint8_t op_data)
{
	struct xnet_xfer_entry *resp;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(op == xnet_op_msg || op == xnet_op_cts);
	resp = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (!resp)
		return -FI_ENOMEM;

	resp->iov[0].iov_base = (void *) &resp->hdr;
	resp->iov[0].iov_len = sizeof(resp->hdr.base_hdr);
	resp->iov_cnt = 1;

	resp->hdr.base_hdr.version = XNET_HDR_VERSION;
	resp->hdr.base_hdr.op_data = op_data;
	resp->hdr.base_hdr.op = op;
	resp->hdr.base_hdr.size = sizeof(resp->hdr.base_hdr);
	resp->hdr.base_hdr.hdr_size = (uint8_t) sizeof(resp->hdr.base_hdr);

	resp->ctrl_flags = XNET_INTERNAL_XFER;
	resp->context = NULL;

	xnet_tx_queue_insert(ep, resp);
	return FI_SUCCESS;
}

static int
xnet_rts_matched(struct xnet_rdm *rdm, struct xnet_ep *ep,
		 struct xnet_xfer_entry *rx_entry)
{
	uint8_t cts_ctx;
	int ret;

	assert(xnet_progress_locked(xnet_rdm2_progress(rdm)));
	if (!ep) {
		ep = xnet_get_rx_ep(rdm, rx_entry->src_addr);
		if (!ep) {
			FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
				"CTS connection no longer active\n");
			ret = -FI_EIO;
			goto err_comp;
		}
	}

	cts_ctx = ofi_byte_idx_set(&ep->cts_queue,
				   rx_entry->hdr.base_hdr.op_data, rx_entry);
	assert(cts_ctx == rx_entry->hdr.base_hdr.op_data);

	ret = xnet_queue_ack(ep, xnet_op_cts, cts_ctx);
	if (ret) {
		ofi_byte_idx_clear(&ep->cts_queue, cts_ctx);
		goto err_comp;
	}
	return 0;

err_comp:
	xnet_cntr_incerr(rx_entry);
	xnet_report_error(rx_entry, -ret);
	xnet_free_xfer(xnet_rdm2_progress(rdm), rx_entry);
	return ret;
}

void xnet_complete_saved(struct xnet_xfer_entry *saved_entry, void *msg_data)
{
	struct xnet_progress *progress;
	size_t msg_len, copied;

	progress = xnet_cq2_progress(saved_entry->cq);
	assert(xnet_progress_locked(progress));

	msg_len = xnet_msg_len(&saved_entry->hdr);
	FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "Completing saved msg "
	       "tag 0x%zx src %zu size %zu\n", saved_entry->tag,
	       saved_entry->src_addr, msg_len);

	if (msg_len) {
		copied = ofi_copy_iov_buf(saved_entry->iov,
				saved_entry->iov_cnt, 0, msg_data,
				msg_len, OFI_COPY_BUF_TO_IOV);
	} else {
		copied = 0;
	}

	if (copied == msg_len) {
		xnet_report_success(saved_entry);
	} else {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "saved recv truncated\n");
		xnet_cntr_incerr(saved_entry);
		/* TODO: need to report received message size = copied */
		xnet_report_error(saved_entry, FI_ETRUNC);
	}
	xnet_free_xfer(progress, saved_entry);
}

void xnet_recv_saved(struct xnet_rdm *rdm, struct xnet_xfer_entry *saved_entry,
		     struct xnet_xfer_entry *rx_entry)
{
	struct xnet_progress *progress;
	size_t msg_len, done_len, copy_len;
	struct xnet_ep *ep;
	void *buf2free, *msg_data;

	progress = xnet_rdm2_progress(rdm);
	assert(xnet_progress_locked(progress));
	FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "recv matched saved msg "
	       "tag 0x%zx src %zu\n", saved_entry->tag, saved_entry->src_addr);

	if (saved_entry->ctrl_flags & XNET_FREE_BUF) {
		buf2free = saved_entry->user_buf;
		msg_data = saved_entry->user_buf;
		saved_entry->ctrl_flags &= ~(XNET_SAVED_XFER | XNET_FREE_BUF);
	} else {
		buf2free = NULL;
		msg_data = &saved_entry->msg_data;
		saved_entry->ctrl_flags &= ~XNET_SAVED_XFER;
	}
	saved_entry->context = rx_entry->context;
	saved_entry->user_buf = rx_entry->user_buf;
	saved_entry->cq_flags |= rx_entry->cq_flags;
	saved_entry->cntr = rx_entry->cntr;
	saved_entry->cq = rx_entry->cq;

	if (rx_entry->iov_cnt) {
		memcpy(&saved_entry->iov[0], &rx_entry->iov[0],
			rx_entry->iov_cnt * sizeof(rx_entry->iov[0]));
		saved_entry->iov_cnt = rx_entry->iov_cnt;
	}

	if (saved_entry->hdr.base_hdr.op == xnet_op_tag_rts) {
		ep = saved_entry->saving_ep;
		(void) xnet_rts_matched(rdm, ep, saved_entry);
		if (ep) {
			saved_entry->saving_ep = NULL;
			assert(saved_entry == ep->cur_rx.entry);
			FI_DBG(&xnet_prov, FI_LOG_EP_DATA,
			       "saved rts msg still active\n");
			assert(!buf2free);
			xnet_reset_rx(ep);
		}
	} else if (!saved_entry->saving_ep) {
		xnet_complete_saved(saved_entry, msg_data);
		free(buf2free);
	/* TODO: io_uring support
	} else if (async recv posted using io_uring) {
	 * If we have an async recv posted to the io_uring, we need to
	 * wait for that to complete before we can copy the data into
	 * the user's buffer.  We cannot free buf2free in this case.
		saved_entry->ctrl_flags |= XNET_COPY_RECV;
	*/
	} else {
		ep = saved_entry->saving_ep;
		saved_entry->saving_ep = NULL;
		assert(saved_entry == ep->cur_rx.entry);
		FI_DBG(&xnet_prov, FI_LOG_EP_DATA, "saved msg still active "
		       "needs %zu bytes\n", ep->cur_rx.data_left);

		msg_len = (saved_entry->hdr.base_hdr.size -
			  saved_entry->hdr.base_hdr.hdr_size);
		done_len = msg_len - ep->cur_rx.data_left;
		assert(msg_len && ep->cur_rx.data_left);

		(void) ofi_truncate_iov(&saved_entry->iov[0],
				        &saved_entry->iov_cnt, msg_len);

		copy_len = ofi_copy_iov_buf(saved_entry->iov,
					    saved_entry->iov_cnt, 0, msg_data,
					    done_len, OFI_COPY_BUF_TO_IOV);
		if (copy_len < done_len) {
			xnet_handle_truncate(ep);
		} else {
			ofi_consume_iov(&saved_entry->iov[0],
					&saved_entry->iov_cnt, done_len);
		}
		free(buf2free);
	}

	xnet_free_xfer(progress, rx_entry);
}

int xnet_uring_pollin_add(struct xnet_progress *progress, int fd,
			  bool multishot, struct ofi_sockctx *pollin_ctx)
{
	int ret;

	assert(xnet_progress_locked(progress));
	if (!pollin_ctx->uring_sqe_inuse) {
		ret = ofi_sockctx_uring_poll_add(progress->rx_uring.sockapi,
						 fd, POLLIN, multishot,
						 pollin_ctx);
		if (ret != -OFI_EINPROGRESS_URING)
			return ret;
	}
	return 0;
}

static int xnet_update_pollflag(struct xnet_ep *ep, short pollflag, bool set)
{
	struct xnet_progress *progress;
	int ret;

	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));
	if (set) {
		if (ep->pollflags & pollflag)
			return 0;

		ep->pollflags |= pollflag;
	} else {
		if (!(ep->pollflags & pollflag))
			return 0;

		ep->pollflags &= ~pollflag;
	}

	if (xnet_io_uring) {
		if (!set)
			return 0;

		if (ep->pollflags & POLLOUT) {
			/* We are not supposed to request POLLOUT event. */
			if (!ep->bsock.tx_sockctx.uring_sqe_inuse)
				return -FI_ENOSYS;

			/* A TX SQE is in use and will wake us up */
			ep->pollflags &= ~POLLOUT;
		}

		if ((ep->pollflags & POLLIN) &&
			ep->bsock.rx_sockctx.uring_sqe_inuse) {
			/* A RX SQE is in use and will wake us up */
			ep->pollflags &= ~POLLIN;
			assert((ep->pollflags & (POLLIN | POLLOUT)) == 0);
			return 0;
		}

		ret = xnet_uring_pollin_add(progress, ep->bsock.sock,
					    false, &ep->bsock.pollin_sockctx);
	} else {
		ret = ofi_dynpoll_mod(&progress->epoll_fd, ep->bsock.sock,
				      ep->pollflags, &ep->util_ep.ep_fid.fid);
		xnet_signal_progress(progress);
	}
	return ret;
}

static int xnet_send_msg(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *tx_entry;
	int ret;
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
	}

	ep->cur_tx.data_left -= len;
	if (ep->cur_tx.data_left) {
		ofi_consume_iov(tx_entry->iov, &tx_entry->iov_cnt, len);
		return -FI_EAGAIN;
	}
	return FI_SUCCESS;
}

static int xnet_recv_msg_data(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	int ret;
	size_t len;

start:
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (!ep->cur_rx.data_left)
		return FI_SUCCESS;

	rx_entry = ep->cur_rx.entry;
	ret = ofi_bsock_recvv(&ep->bsock, rx_entry->iov, rx_entry->iov_cnt, &len);
	if (ret < 0) {
		if (ret == -OFI_EINPROGRESS_URING) {
			ep->cur_rx.data_left -= len;
			assert(ep->cur_rx.data_left);
			ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt, len);
		}
		return ret;
	}

	ep->cur_rx.data_left -= len;
	if (!ep->cur_rx.data_left)
		return FI_SUCCESS;

	ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt, len);
	if (!rx_entry->iov_cnt || !rx_entry->iov[0].iov_len) {
		ret = xnet_handle_truncate(ep);
		if (!ret)
			goto start; /* remove msg data from the tcp stream */
		return ret;
	}

	return -FI_EAGAIN;
}

static void xnet_need_cts(struct xnet_ep *ep, struct xnet_xfer_entry *tx_entry)
{
	uint64_t msg_len;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(tx_entry->hdr.base_hdr.op == xnet_op_tag_rts);

	msg_len = xnet_msg_len(&tx_entry->hdr);
	tx_entry->hdr.base_hdr.op = xnet_op_data;
	tx_entry->hdr.base_hdr.hdr_size = sizeof(tx_entry->hdr.base_hdr);

	tx_entry->iov[0].iov_len = tx_entry->hdr.base_hdr.hdr_size;
	tx_entry->iov_cnt = tx_entry->rts_iov_cnt;
	tx_entry->hdr.base_hdr.size = tx_entry->hdr.base_hdr.hdr_size + msg_len;
	assert(tx_entry->hdr.base_hdr.size ==
	       ofi_total_iov_len(tx_entry->iov, tx_entry->iov_cnt));
}

static void xnet_complete_tx(struct xnet_ep *ep, int ret)
{
	struct xnet_xfer_entry *tx_entry;

	tx_entry = ep->cur_tx.entry;

	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_DOMAIN, "msg send failed\n");
		xnet_cntr_incerr(tx_entry);
		xnet_report_error(tx_entry, -ret);
		xnet_free_xfer(xnet_ep2_progress(ep), tx_entry);
	} else if (tx_entry->ctrl_flags & XNET_NEED_CTS) {
		/* Will get SW CTS ack, async completion not needed */
		xnet_need_cts(ep, tx_entry);
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
		slist_insert_tail(&tx_entry->entry, &ep->async_queue);
	} else {
		xnet_report_success(tx_entry);
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
		return;
	}

	ep->cur_tx.data_left = ep->cur_tx.entry->hdr.base_hdr.size;
	OFI_DBG_SET(ep->cur_tx.entry->hdr.base_hdr.id, ep->tx_id++);
	ep->hdr_bswap(ep, &ep->cur_tx.entry->hdr.base_hdr);
}

static void xnet_progress_tx(struct xnet_ep *ep)
{
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	while (ep->cur_tx.entry) {
		ret = xnet_send_msg(ep);
		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret)) {
			ret = xnet_update_pollflag(ep, POLLOUT, true);
			if (!ret)
				return;
			else
				goto disable_ep;
		} else if (ret == -OFI_EINPROGRESS_URING) {
			return;
		}

		xnet_complete_tx(ep, ret);
	}

	/* Buffered data is sent first by xnet_send_msg, but if we don't
	 * have other data to send, we need to try flushing any buffered data.
	 */
	ret = ofi_bsock_flush(&ep->bsock);
	if (ret == -OFI_EINPROGRESS_URING)
		return;

	ret = xnet_update_pollflag(ep, POLLOUT, ofi_bsock_tosend(&ep->bsock));
	if (!ret)
		return;

disable_ep:
	xnet_ep_disable(ep, 0, NULL, 0);
}

static void xnet_pmem_commit(struct xnet_ep *ep, struct xnet_xfer_entry *rx_entry)
{
	struct ofi_rma_iov *rma_iov;
	size_t offset;
	int i;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
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

	if (!xfer->mrecv) {
		xfer->mrecv = calloc(1, sizeof(struct xnet_mrecv));
		if (!xfer->mrecv) {
			xfer->cq_flags |= FI_MULTI_RECV;
			return FI_SUCCESS;
		}
		xfer->mrecv->ref_cnt = 1;
	}

	recv_entry->ctrl_flags = XNET_MULTI_RECV;
	recv_entry->cq_flags = FI_MSG | FI_RECV;
	recv_entry->cntr = xfer->cntr;
	recv_entry->cq = xfer->cq;
	recv_entry->context = xfer->context;
	recv_entry->mrecv = xfer->mrecv;
	recv_entry->mrecv->ref_cnt++;

	recv_entry->iov_cnt = 1;
	recv_entry->user_buf =  (char *) xfer->iov[0].iov_base + msg_len;
	recv_entry->iov[0].iov_base = recv_entry->user_buf;
	recv_entry->iov[0].iov_len = left;

	slist_insert_head(&recv_entry->entry, &ep->srx->rx_queue);
	return 0;

complete:
	if (!xfer->mrecv)
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
	if (ep->cur_rx.hdr.base_hdr.size != sizeof(ep->cur_rx.hdr.base_hdr))
		return -FI_EIO;

	assert(!slist_empty(&ep->need_ack_queue));
	tx_entry = container_of(slist_remove_head(&ep->need_ack_queue),
				struct xnet_xfer_entry, entry);

	xnet_report_success(tx_entry);
	xnet_free_xfer(xnet_ep2_progress(ep), tx_entry);
	xnet_reset_rx(ep);
	return FI_SUCCESS;
}

int xnet_start_recv(struct xnet_ep *ep, struct xnet_xfer_entry *rx_entry)
{
	struct xnet_active_rx *msg = &ep->cur_rx;
	size_t recv_len;
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (!dlist_empty(&ep->unexp_entry)) {
		dlist_remove_init(&ep->unexp_entry);
		ret = xnet_update_pollflag(ep, POLLIN, true);
		if (ret)
			goto poll_err;
	}

	recv_len = msg->hdr.base_hdr.size - msg->hdr.base_hdr.hdr_size;

	memcpy(&rx_entry->hdr, &msg->hdr,
	       (size_t) msg->hdr.base_hdr.hdr_size);
	if (ep->peer)
		rx_entry->src_addr = ep->peer->fi_addr;
	rx_entry->cq_flags |= xnet_rx_completion_flag(ep);
	rx_entry->cq = xnet_ep_rx_cq(ep);
	rx_entry->cntr = ep->util_ep.cntrs[CNTR_RX];

	if (rx_entry->ctrl_flags & XNET_MULTI_RECV) {
		assert(msg->hdr.base_hdr.op == xnet_op_msg);
		(void) xnet_alter_mrecv(ep, rx_entry, recv_len);
	}

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = xnet_recv_msg_data;

	if ((msg->hdr.base_hdr.op == xnet_op_tag_rts) &&
	    !(rx_entry->ctrl_flags & XNET_SAVED_XFER)) {
		ret = xnet_rts_matched(ep->srx->rdm, ep, rx_entry);
		xnet_reset_rx(ep);
	} else {
		(void) ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_cnt, recv_len);
		ret = xnet_recv_msg_data(ep);
	}
	return ret;

 poll_err:
	xnet_cntr_incerr(rx_entry);
	xnet_report_error(rx_entry, -ret);
	xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	return ret;
}

static int xnet_handle_msg(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct xnet_active_rx *msg = &ep->cur_rx;
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (msg->hdr.base_hdr.op_data == XNET_OP_ACK)
		return xnet_handle_ack(ep);

	rx_entry = xnet_get_rx_entry(ep);
	if (!rx_entry) {
		if (dlist_empty(&ep->unexp_entry)) {
			dlist_insert_tail(&ep->unexp_entry,
					  &xnet_ep2_progress(ep)->unexp_msg_list);
			ret = xnet_update_pollflag(ep, POLLIN, false);
			if (ret)
				return ret;
		}
		return -FI_EAGAIN;
	}

	return xnet_start_recv(ep, rx_entry);
}

static int xnet_handle_tag(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct xnet_active_rx *msg = &ep->cur_rx;
	uint64_t tag;
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(ep->srx);

	tag = (msg->hdr.base_hdr.flags & XNET_REMOTE_CQ_DATA) ?
	      msg->hdr.tag_data_hdr.tag : msg->hdr.tag_hdr.tag;

	rx_entry = ep->srx->match_tag_rx(ep->srx, ep, tag);
	if (rx_entry)
		return xnet_start_recv(ep, rx_entry);

	if (xnet_save_and_cont(ep)) {
		rx_entry = xnet_get_save_rx(ep, tag);
		if (rx_entry)
			return xnet_start_recv(ep, rx_entry);
	}
	if (dlist_empty(&ep->unexp_entry)) {
		dlist_insert_tail(&ep->unexp_entry,
				  &xnet_ep2_progress(ep)->unexp_tag_list);
		ret = xnet_update_pollflag(ep, POLLIN, false);
		if (ret)
			return ret;
	}
	return -FI_EAGAIN;
}

static int xnet_handle_cts(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *tx_entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	tx_entry = ofi_byte_idx_remove(&ep->rts_queue,
				       ep->cur_rx.hdr.base_hdr.op_data);
	if (!tx_entry) {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "Invalid cst index\n");
		return -FI_EINVAL;
	}

	assert(tx_entry->ctrl_flags & XNET_NEED_CTS);
	tx_entry->ctrl_flags &= ~XNET_NEED_CTS;
	xnet_tx_queue_insert(ep, tx_entry);
	xnet_reset_rx(ep);
	return 0;
}

static int xnet_handle_data(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct xnet_active_rx *msg = &ep->cur_rx;
	size_t msg_len;
	uint8_t cts_ctx;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	cts_ctx = ep->cur_rx.hdr.base_hdr.op_data;
	rx_entry = ofi_byte_idx_clear(&ep->cts_queue, cts_ctx);
	if (!rx_entry) {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "Invalid cts index\n");
		return -FI_EINVAL;
	}

	/* RTS header carries all the meta-data about the request. */
	assert(rx_entry->cq);
	msg_len = xnet_msg_len(&msg->hdr);
	if (msg_len != xnet_msg_len(&rx_entry->hdr)) {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "rts - data size mismatch\n");
		return -FI_EIO;
	}

	(void) ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_cnt, msg_len);

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = xnet_recv_msg_data;
	return xnet_recv_msg_data(ep);
}

static int xnet_handle_read_req(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *resp;
	struct ofi_rma_iov *rma_iov;
	ssize_t i;
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	resp = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (!resp)
		return -FI_ENOMEM;

	memcpy(&resp->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	resp->hdr.base_hdr.op_data = 0;
	/* record src_addr for debugging */
	if (ep->peer)
		resp->src_addr = ep->peer->fi_addr;

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

	resp->hdr.base_hdr.op = xnet_op_read_rsp;
	resp->hdr.base_hdr.hdr_size = (uint8_t) sizeof(resp->hdr.base_hdr);

	resp->ctrl_flags = XNET_INTERNAL_XFER;
	resp->context = NULL;

	xnet_tx_queue_insert(ep, resp);
	xnet_reset_rx(ep);
	return FI_SUCCESS;
}

static int xnet_handle_write(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;
	struct ofi_rma_iov *rma_iov;
	ssize_t i;
	int ret;

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
	rx_entry->cntr = ep->util_ep.cntrs[CNTR_REM_WR];
	rx_entry->cq = xnet_ep_rx_cq(ep);

	memcpy(&rx_entry->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	rx_entry->hdr.base_hdr.op_data = 0;
	if (ep->peer)
		rx_entry->src_addr = ep->peer->fi_addr;

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
	ep->cur_rx.handler = xnet_recv_msg_data;
	return xnet_recv_msg_data(ep);
}

static int xnet_handle_read_rsp(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *rx_entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	if (slist_empty(&ep->rma_read_queue))
		return -FI_EINVAL;

	rx_entry = container_of(slist_remove_head(&ep->rma_read_queue),
				struct xnet_xfer_entry, entry);

	memcpy(&rx_entry->hdr, &ep->cur_rx.hdr,
	       (size_t) ep->cur_rx.hdr.base_hdr.hdr_size);
	rx_entry->hdr.base_hdr.op_data = 0;

	ep->cur_rx.entry = rx_entry;
	ep->cur_rx.handler = xnet_recv_msg_data;
	return xnet_recv_msg_data(ep);
}

static int xnet_progress_hdr(struct xnet_ep *ep)
{
	if (ep->cur_rx.hdr_done == sizeof(ep->cur_rx.hdr.base_hdr)) {
		assert(ep->cur_rx.hdr_len == sizeof(ep->cur_rx.hdr.base_hdr));

		if (ep->cur_rx.hdr.base_hdr.hdr_size > XNET_MAX_HDR) {
			FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
				"Payload offset is too large\n");
			return -FI_EIO;
		}
		ep->cur_rx.hdr_len = (size_t) ep->cur_rx.hdr.base_hdr.hdr_size;
	}

	if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len)
		return -FI_EAGAIN;

	ep->hdr_bswap(ep, &ep->cur_rx.hdr.base_hdr);

#ifndef NDEBUG
	if (ep->cur_rx.hdr.base_hdr.id != ep->rx_id++) {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
			"Received invalid hdr sequence number\n");
		return -FI_EIO;
	}
#endif

	if (ep->cur_rx.hdr.base_hdr.op >= ARRAY_SIZE(xnet_start_op)) {
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
			"Received invalid opcode\n");
		return -FI_EIO;
	}

	ep->cur_rx.data_left = ep->cur_rx.hdr.base_hdr.size -
			       ep->cur_rx.hdr.base_hdr.hdr_size;
	ep->cur_rx.handler = xnet_start_op[ep->cur_rx.hdr.base_hdr.op];
	return FI_SUCCESS;
}

static int xnet_recv_hdr(struct xnet_ep *ep)
{
	size_t len;
	void *buf;
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(ep->cur_rx.hdr_done < ep->cur_rx.hdr_len);

next_hdr:
	buf = (uint8_t *) &ep->cur_rx.hdr + ep->cur_rx.hdr_done;
	len = ep->cur_rx.hdr_len - ep->cur_rx.hdr_done;
	ret = ofi_bsock_recv(&ep->bsock, buf, &len);
	if (ret < 0) {
		if (ret == -OFI_EINPROGRESS_URING)
			ep->cur_rx.hdr_done += len;
		return ret;
	}

	ep->cur_rx.hdr_done += len;

	ret = xnet_progress_hdr(ep);
	if (ret) {
		if (ret == -FI_EAGAIN &&
		    ep->cur_rx.hdr_done == sizeof(ep->cur_rx.hdr.base_hdr)) {
			goto next_hdr;
		}

		return ret;
	}

	return ep->cur_rx.handler(ep);
}

static void xnet_complete_rx(struct xnet_ep *ep, ssize_t ret)
{
	struct xnet_xfer_entry *rx_entry;

	rx_entry = ep->cur_rx.entry;
	assert(rx_entry);

	if (ret)
		goto cq_error;

	if (rx_entry->hdr.base_hdr.flags & XNET_COMMIT_COMPLETE)
		xnet_pmem_commit(ep, rx_entry);

	if ((rx_entry->hdr.base_hdr.flags &
	    (XNET_DELIVERY_COMPLETE | XNET_COMMIT_COMPLETE)) &&
	    ((rx_entry->hdr.base_hdr.op != xnet_op_tag_rts) ||
	     !(rx_entry->ctrl_flags & XNET_SAVED_XFER))) {
		ret = xnet_queue_ack(ep, xnet_op_msg, XNET_OP_ACK);
		if (ret)
			goto cq_error;
	}

	if (!(rx_entry->ctrl_flags & XNET_SAVED_XFER)) {
		xnet_report_success(rx_entry);
		xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	} else {
		rx_entry->saving_ep = NULL;
	}
	xnet_reset_rx(ep);
	return;

cq_error:
	FI_WARN(&xnet_prov, FI_LOG_EP_DATA,
		"msg recv failed ret = %zd (%s)\n", ret, fi_strerror((int)-ret));
	xnet_cntr_incerr(rx_entry);
	xnet_report_error(rx_entry, (int) -ret);
	xnet_free_xfer(xnet_ep2_progress(ep), rx_entry);
	xnet_reset_rx(ep);
	xnet_ep_disable(ep, 0, NULL, 0);
}

void xnet_progress_rx(struct xnet_ep *ep)
{
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	do {
		assert(ep->state == XNET_CONNECTED);
		if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len) {
			ret = xnet_recv_hdr(ep);
		} else {
			ret = ep->cur_rx.handler(ep);
		}

		if (OFI_SOCK_TRY_SND_RCV_AGAIN(-ret) ||
		    ret == -OFI_EINPROGRESS_URING)
			break;

		if (ep->cur_rx.entry)
			xnet_complete_rx(ep, ret);
		else if (ret)
			xnet_ep_disable(ep, 0, NULL, 0);

	} while (!ret && ofi_bsock_readable(&ep->bsock));

	if (xnet_io_uring) {
		if (ret == -OFI_EINPROGRESS_URING)
			ret = xnet_update_pollflag(ep, POLLIN, false);
		else if (!ret || OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
			ret = xnet_update_pollflag(ep, POLLIN, true);

		if (ret)
			xnet_ep_disable(ep, 0, NULL, 0);
	}
}

void xnet_progress_async(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *xfer;
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	ret = ofi_bsock_async_done(&xnet_prov, &ep->bsock);
	if (ret) {
		xnet_ep_disable(ep, 0, NULL, 0);
		return;
	}

	while (!slist_empty(&ep->async_queue)) {
		xfer = container_of(ep->async_queue.head,
				    struct xnet_xfer_entry, entry);
		if (ofi_val32_gt(xfer->async_index, ep->bsock.done_index))
			break;

		slist_remove_head(&ep->async_queue);
		xnet_report_success(xfer);
		xnet_free_xfer(xnet_ep2_progress(ep), xfer);
	}
}

static void xnet_uring_tx_done(struct xnet_ep *ep, int res)
{
	struct xnet_xfer_entry *tx_entry;

	tx_entry = ep->cur_tx.entry;
	assert(tx_entry);

	if (res < 0) {
		if (!OFI_SOCK_TRY_SND_RCV_AGAIN(-res))
			xnet_complete_tx(ep, res);
	} else {
		assert(res <= ep->cur_tx.data_left);
		ep->cur_tx.data_left -= res;
		if (ep->cur_tx.data_left)
			ofi_consume_iov(tx_entry->iov, &tx_entry->iov_cnt,
					res);
		else
			xnet_complete_tx(ep, FI_SUCCESS);
	}
	xnet_progress_tx(ep);
}

static void xnet_uring_rx_done(struct xnet_ep *ep, int res)
{
	struct xnet_xfer_entry *rx_entry;
	int ret;

	if (ep->bsock.async_prefetch) {
		if (res > 0)
			ofi_bsock_prefetch_done(&ep->bsock, res);
		else {
			ep->bsock.async_prefetch = false;
			goto disable_ep;
		}
	} else if (res <= 0 && !OFI_SOCK_TRY_SND_RCV_AGAIN(-res)) {
		if (ep->cur_rx.entry)
			xnet_complete_rx(ep, res);
		else
			goto disable_ep;
	} else if (ep->cur_rx.hdr_done < ep->cur_rx.hdr_len) {
		ep->cur_rx.hdr_done += res;
		ret = xnet_progress_hdr(ep);
		if (ret != 0 && !OFI_SOCK_TRY_SND_RCV_AGAIN(-ret))
			goto disable_ep;
	} else {
		rx_entry = ep->cur_rx.entry;
		assert(rx_entry);

		assert(res <= ep->cur_rx.data_left);
		ep->cur_rx.data_left -= res;
		if (ep->cur_rx.data_left)
			ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt,
					res);
		else
			xnet_complete_rx(ep, FI_SUCCESS);
	}
	xnet_progress_rx(ep);
	return;

disable_ep:
	xnet_ep_disable(ep, 0, NULL, 0);
}

static void xnet_uring_connect_done(struct xnet_ep *ep, int res)
{
	struct xnet_progress *progress;
	int ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "socket connected, sending req\n");
	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));

	if (res < 0) {
		FI_WARN_SPARSE(&xnet_prov, FI_LOG_EP_CTRL,
				"connection failure (sockerr %d)\n", res);
		ret = res;
		goto disable;
	}

	ret = xnet_send_cm_msg(ep);
	if (ret)
		goto disable;

	ep->state = XNET_REQ_SENT;
	ret = ofi_bsock_recv_unbuffered(&ep->bsock, ep->cm_msg,
					sizeof(ep->cm_msg->hdr));
	if (ret != -OFI_EINPROGRESS_URING)
		goto disable;

	xnet_signal_progress(progress);
	return;

disable:
	xnet_ep_disable(ep, -ret, NULL, 0);
}

static void xnet_uring_run_ep(struct xnet_ep *ep, struct ofi_sockctx *sockctx,
			      int res)
{
	switch (ep->state) {
	case XNET_CONNECTED:
		if (sockctx == &ep->bsock.tx_sockctx) {
			xnet_uring_tx_done(ep, res);
		} else if (sockctx == &ep->bsock.rx_sockctx) {
			xnet_uring_rx_done(ep, res);
		} else if (sockctx == &ep->bsock.pollin_sockctx) {
			if (res < 0) {
				xnet_ep_disable(ep, -res, NULL, 0);
			} else {
				assert(res & POLLIN);
				xnet_progress_rx(ep);
			}
		}
		/* Must be a cancelation otherwise */
		break;
	case XNET_CONNECTING:
		xnet_uring_connect_done(ep, res);
		break;
	case XNET_REQ_SENT:
		if (sockctx == &ep->bsock.rx_sockctx)
			xnet_uring_req_done(ep, res);
		/* Must be a cancelation otherwise */
		break;
	default:
		break;
	}
}

static void xnet_uring_run_conn(struct xnet_conn_handle *conn, int res)
{
	if (res >= 0) {
		conn->sock = res;
		xnet_handle_conn(conn, false);
	} else {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "accept socket error\n");
		free(conn);
	}
}

static void xnet_progress_cqe(struct xnet_progress *progress,
			      struct xnet_uring *uring,
			      ofi_io_uring_cqe_t *cqe)
{
	struct ofi_sockctx *sockctx;
	struct fid *fid;
	struct xnet_ep *ep;
	struct xnet_conn_handle *conn;
	struct xnet_pep *pep;

	assert(xnet_io_uring);
	sockctx = (struct ofi_sockctx *) cqe->user_data;
	assert(sockctx);
	assert(sockctx->uring_sqe_inuse);
	if (!(cqe ->flags & IORING_CQE_F_MORE))
		sockctx->uring_sqe_inuse = false;
	uring->sockapi->credits++;

	fid = sockctx->context;
	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(fid, struct xnet_ep, util_ep.ep_fid.fid);
		xnet_uring_run_ep(ep, sockctx, cqe->res);
		break;
	case FI_CLASS_CONNREQ:
		conn = container_of(fid, struct xnet_conn_handle, fid);
		xnet_uring_run_conn(conn, cqe->res);
		break;
	case FI_CLASS_PEP:
		pep = container_of(fid, struct xnet_pep, util_pep.pep_fid.fid);
		if (sockctx == &pep->pollin_sockctx) {
			if (cqe->res >= 0)
				xnet_accept_sock(pep);
		}
		/* Must be a cancelation otherwise */
		break;
	default:
		assert(0);
	}
}

static void xnet_progress_uring(struct xnet_progress *progress,
				struct xnet_uring *uring)
{
	int nready;
	int i;

	assert(xnet_io_uring);

	nready = ofi_uring_peek_batch_cqe(&uring->ring, progress->cqes,
					  XNET_MAX_EVENTS);
	if (!nready)
		return;

	assert(nready <= XNET_MAX_EVENTS);
	for (i = 0; i < nready; i++) {
		xnet_progress_cqe(progress, uring, progress->cqes[i]);
	}

	ofi_uring_cq_advance(&uring->ring, nready);
}

int xnet_uring_cancel(struct xnet_progress *progress,
		      struct xnet_uring *uring,
		      struct ofi_sockctx *canceled_ctx,
		      void *context)
{
	struct ofi_sockctx ctx;
	bool submitted = false;
	int ret;

	assert(xnet_progress_locked(progress));
	if (!xnet_io_uring)
		return 0;

	ofi_sockctx_init(&ctx, context);
	while (canceled_ctx->uring_sqe_inuse || ctx.uring_sqe_inuse) {
		assert(xnet_io_uring);
		if (!submitted) {
			ret = ofi_sockctx_uring_cancel(uring->sockapi,
						       canceled_ctx,
						       &ctx);
			if (ret == -OFI_EINPROGRESS_URING) {
				(void) ofi_uring_submit(&uring->ring);
				submitted = true;
			} else if (ret != -FI_EAGAIN)
				return ret;
		}

		xnet_progress_uring(progress, uring);
	}

	xnet_submit_uring(uring);
	return 0;
}

void xnet_tx_queue_insert(struct xnet_ep *ep,
			  struct xnet_xfer_entry *tx_entry)
{
	struct xnet_progress *progress;

	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));

	if (!ep->cur_tx.entry) {
		ep->cur_tx.entry = tx_entry;
		ep->cur_tx.data_left = tx_entry->hdr.base_hdr.size;
		OFI_DBG_SET(tx_entry->hdr.base_hdr.id, ep->tx_id++);
		ep->hdr_bswap(ep, &tx_entry->hdr.base_hdr);
		xnet_progress_tx(ep);
		if (xnet_io_uring)
			xnet_submit_uring(&progress->tx_uring);
	} else if (tx_entry->ctrl_flags & XNET_INTERNAL_XFER) {
		slist_insert_tail(&tx_entry->entry, &ep->priority_queue);
	} else {
		slist_insert_tail(&tx_entry->entry, &ep->tx_queue);
	}
}

static int (*xnet_start_op[xnet_op_max])(struct xnet_ep *ep) = {
	[xnet_op_msg] = xnet_handle_msg,
	[xnet_op_tag] = xnet_handle_tag,
	[xnet_op_read_req] = xnet_handle_read_req,
	[xnet_op_read_rsp] = xnet_handle_read_rsp,
	[xnet_op_write] = xnet_handle_write,
	[xnet_op_tag_rts] = xnet_handle_tag,
	[xnet_op_cts] = xnet_handle_cts,
	[xnet_op_data] = xnet_handle_data,
};

static void xnet_run_ep(struct xnet_ep *ep, bool pin, bool pout, bool perr)
{
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	switch (ep->state) {
	case XNET_CONNECTED:
		if (perr)
			xnet_progress_async(ep);
		if (pin && ep->state == XNET_CONNECTED)
			xnet_progress_rx(ep);
		if (pout && ep->state == XNET_CONNECTED)
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

static void
xnet_run_conn(struct xnet_conn_handle *conn, bool pin, bool pout, bool perr)
{
	assert(xnet_progress_locked(conn->pep->progress));

	/* Don't monitor the socket until the user calls fi_accept */
	xnet_halt_sock(conn->pep->progress, conn->sock);
	xnet_handle_conn(conn, perr);
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
			xnet_progress_uring(progress, events[i].data.ptr);
			break;
		default:
			assert(fid->fclass == XNET_CLASS_PROGRESS);
			if (clear_signal)
				fd_signal_reset(&progress->signal);
			break;
		}
	}

	xnet_handle_event_list(progress);
	if (xnet_io_uring) {
		xnet_submit_uring(&progress->tx_uring);
		xnet_submit_uring(&progress->rx_uring);
	}
}

void xnet_progress_unexp(struct xnet_progress *progress,
			 struct dlist_entry *unexp_list)
{
	struct dlist_entry *item, *tmp;
	struct xnet_ep *ep;

	assert(ofi_genlock_held(progress->active_lock));
	dlist_foreach_safe(unexp_list, item, tmp) {
		ep = container_of(item, struct xnet_ep, unexp_entry);
		assert(xnet_has_unexp(ep));
		assert(ep->state == XNET_CONNECTED);
		xnet_progress_rx(ep);
		if (xnet_io_uring)
			xnet_submit_uring(&progress->rx_uring);
	}
}

void xnet_run_progress(struct xnet_progress *progress, bool clear_signal)
{
	int nfds;

	assert(ofi_genlock_held(progress->active_lock));
	if (xnet_io_uring) {
		xnet_progress_uring(progress, &progress->tx_uring);
		xnet_progress_uring(progress, &progress->rx_uring);
		xnet_handle_event_list(progress);
		xnet_submit_uring(&progress->tx_uring);
		xnet_submit_uring(&progress->rx_uring);
	} else {
		nfds = ofi_dynpoll_wait(&progress->epoll_fd, &progress->events[0],
					ARRAY_SIZE(progress->events), 0);
		xnet_handle_events(progress, &progress->events[0], nfds, clear_signal);
	}
}

void xnet_progress(struct xnet_progress *progress, bool clear_signal)
{
	ofi_genlock_lock(progress->active_lock);
	xnet_run_progress(progress, clear_signal);
	ofi_genlock_unlock(progress->active_lock);
}

void xnet_progress_all(struct xnet_eq *eq)
{
	struct xnet_domain *domain;
	struct dlist_entry *item;
	struct fid_list_entry *entry;

	ofi_mutex_lock(&eq->domain_lock);
	dlist_foreach(&eq->domain_list, item) {
		entry = container_of(item, struct fid_list_entry, entry);
		domain = container_of(entry->fid, struct xnet_domain,
				      util_domain.domain_fid.fid);
		xnet_progress(&domain->progress, false);
	}
	ofi_mutex_unlock(&eq->domain_lock);

	xnet_progress(&eq->progress, false);
}

static void xnet_reset_wait(struct util_wait *wait)
{
	struct util_wait_fd *wait_fd;

	if (!wait)
		return;

	wait_fd = container_of(wait, struct util_wait_fd, util_wait);
	fd_signal_reset(&wait_fd->signal);
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
	struct xnet_cq *cq;
	struct xnet_eq *eq;
	struct util_cntr *cntr;
	struct util_wait *wait;
	int ret = 0, i;

	for (i = 0; i < count && !ret; i++) {
		switch (fid[i]->fclass) {
		case FI_CLASS_CQ:
			cq = container_of(fid[i], struct xnet_cq,
					  util_cq.cq_fid.fid);
			ofi_genlock_lock(xnet_cq2_progress(cq)->active_lock);
			if (ofi_cirque_isempty(cq->util_cq.cirq))
				xnet_reset_wait(cq->util_cq.wait);
			else
				ret = -FI_EAGAIN;
			ofi_genlock_unlock(xnet_cq2_progress(cq)->active_lock);
			break;
		case FI_CLASS_EQ:
			eq = container_of(fid[i], struct xnet_eq,
					  util_eq.eq_fid.fid);
			ofi_mutex_lock(&eq->util_eq.lock);
			if (!slist_empty(&eq->util_eq.list))
				ret = -FI_EAGAIN;
			ofi_mutex_unlock(&eq->util_eq.lock);
			if (!ret)
				xnet_reset_wait(eq->util_eq.wait);
			break;
		case FI_CLASS_CNTR:
			/* The user must read the counter before and after
			 * fi_trywait and compare the results to ensure that no
			 * events were lost.
			 */
			cntr = container_of(fid[i], struct util_cntr,
					    cntr_fid.fid);
			ofi_genlock_lock(xnet_cntr2_progress(cntr)->active_lock);
			xnet_reset_wait(cntr->wait);
			ofi_genlock_unlock(xnet_cntr2_progress(cntr)->active_lock);
			break;
		case FI_CLASS_WAIT:
			wait = container_of(fid[i], struct util_wait,
					    wait_fid.fid);
			ret = wait->wait_try(wait);
			break;
		default:
			ret = -FI_ENOSYS;
			break;
		}

	}

	return ret;
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

	/* We cannot enter blocking if io_uring has entries
	 * that need submission. */
	if (xnet_io_uring) {
		assert(ofi_uring_sq_ready(&progress->tx_uring.ring) == 0);
		assert(ofi_uring_sq_ready(&progress->rx_uring.ring) == 0);
	}
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

	assert(!xnet_io_uring);
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

	assert(!xnet_io_uring);
	assert(xnet_progress_locked(progress));
	ret = ofi_dynpoll_del(&progress->epoll_fd, sock);
	if (ret && ret != -FI_ENOENT) {
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
		progress->active_lock = &progress->ep_lock;
	}

	ret = ofi_genlock_init(&progress->ep_lock, base_type);
	if (ret)
		return ret;

	ret = ofi_genlock_init(&progress->rdm_lock, rdm_type);
	if (ret)
		ofi_genlock_destroy(&progress->ep_lock);

	return ret;
}

static int xnet_init_uring(struct xnet_uring *uring, size_t entries,
			   struct ofi_sockapi_uring *sockapi,
			   struct ofi_dynpoll *dynpoll)
{
	int ret;

	ret = ofi_uring_init(&uring->ring, entries);
	if (ret)
		return ret;

	uring->fid.fclass = XNET_CLASS_URING;
	uring->sockapi = sockapi;
	uring->sockapi->io_uring = &uring->ring;
	uring->sockapi->credits = ofi_uring_sq_space_left(&uring->ring);

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
	assert(ofi_uring_sq_ready(&uring->ring) == 0);
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
			sizeof(struct xnet_xfer_entry) + xnet_buf_size,
			16, 0, 1024, 0);
	if (ret)
		goto err3;

	ret = ofi_dynpoll_add(&progress->epoll_fd, progress->signal.fd[FI_READ_FD],
			      POLLIN, &progress->fid);
	if (ret)
		goto err4;

	if (xnet_io_uring) {
		progress->cqes = calloc(XNET_MAX_EVENTS, sizeof(*progress->cqes));
		if (!progress->cqes)
			goto err5;

		progress->sockapi = xnet_sockapi_uring;

		ret = xnet_init_uring(&progress->tx_uring,
				      info ? info->tx_attr->size :
					     xnet_default_tx_size,
				      &progress->sockapi.tx_uring,
				      &progress->epoll_fd);
		if (ret)
			goto err6;

		ret = xnet_init_uring(&progress->rx_uring,
				      info ? info->rx_attr->size :
					     xnet_default_rx_size,
				      &progress->sockapi.rx_uring,
				      &progress->epoll_fd);
		if (ret)
			goto err7;
	} else {
		progress->sockapi = xnet_sockapi_socket;
	}

	return 0;
err7:
	xnet_destroy_uring(&progress->tx_uring, &progress->epoll_fd);
err6:
	ofi_dynpoll_del(&progress->epoll_fd, progress->signal.fd[FI_READ_FD]);
err5:
	free(progress->cqes);
err4:
	ofi_bufpool_destroy(progress->xfer_pool);
err3:
	ofi_dynpoll_close(&progress->epoll_fd);
err2:
	ofi_genlock_destroy(&progress->rdm_lock);
	ofi_genlock_destroy(&progress->ep_lock);
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
		free(progress->cqes);
		xnet_destroy_uring(&progress->rx_uring, &progress->epoll_fd);
		xnet_destroy_uring(&progress->tx_uring, &progress->epoll_fd);
	}
	ofi_dynpoll_close(&progress->epoll_fd);
	ofi_bufpool_destroy(progress->xfer_pool);
	ofi_genlock_destroy(&progress->ep_lock);
	ofi_genlock_destroy(&progress->rdm_lock);
	fd_signal_free(&progress->signal);
}
