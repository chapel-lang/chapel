/*
 * Copyright (c) 2018-2022 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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
#include "rdma/fi_eq.h"
#include "ofi_iov.h"
#include <ofi_prov.h>
#include "xnet.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <ofi_util.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netdb.h>


static inline struct xnet_xfer_entry *
xnet_alloc_send(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *send_entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	send_entry = xnet_alloc_tx(ep);
	if (send_entry) {
		send_entry->hdr.base_hdr.op = xnet_op_msg;
		send_entry->cntr = ep->util_ep.cntrs[CNTR_TX];
	}

	return send_entry;
}

static inline struct xnet_xfer_entry *
xnet_alloc_tsend(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *send_entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	send_entry = xnet_alloc_tx(ep);
	if (send_entry) {
		assert(ep->srx);
		send_entry->hdr.base_hdr.op = xnet_op_tag;
		send_entry->cntr = ep->util_ep.cntrs[CNTR_TX];
	}

	return send_entry;
}

static inline void
xnet_init_tx_sizes(struct xnet_xfer_entry *tx_entry, size_t hdr_len,
		   size_t data_len)
{
	tx_entry->hdr.base_hdr.size = hdr_len + data_len;
	tx_entry->hdr.base_hdr.hdr_size = (uint8_t) hdr_len;
}

static inline void
xnet_init_tx_inject(struct xnet_xfer_entry *tx_entry, size_t hdr_len,
		    const void *buf, size_t data_len)
{
	assert(data_len <= xnet_max_inject);
	xnet_init_tx_sizes(tx_entry, hdr_len, data_len);

	tx_entry->iov[0].iov_base = (void *) &tx_entry->hdr;
	memcpy((uint8_t *) &tx_entry->hdr + hdr_len, (uint8_t *) buf,
		data_len);
	tx_entry->iov[0].iov_len = hdr_len + data_len;
	tx_entry->iov_cnt = 1;
}

static inline void
xnet_init_tx_buf(struct xnet_xfer_entry *tx_entry, size_t hdr_len,
		 const void *buf, size_t data_len)
{
	if (data_len <= xnet_max_inject) {
		xnet_init_tx_inject(tx_entry, hdr_len, buf, data_len);
		return;
	}

	xnet_init_tx_sizes(tx_entry, hdr_len, data_len);
	tx_entry->iov[0].iov_base = (void *) &tx_entry->hdr;
	tx_entry->iov[0].iov_len = hdr_len;
	tx_entry->user_buf = (void *) buf;
	tx_entry->iov[1].iov_base = (void *) buf;
	tx_entry->iov[1].iov_len = data_len;
	tx_entry->iov_cnt = 2;
}

static inline void
xnet_init_tx_iov(struct xnet_xfer_entry *tx_entry, size_t hdr_len,
		 const struct iovec *iov, size_t count)
{
	size_t data_len;

	assert(count <= XNET_IOV_LIMIT);
	data_len = ofi_total_iov_len(iov, count);
	xnet_init_tx_sizes(tx_entry, hdr_len, data_len);

	tx_entry->iov[0].iov_base = (void *) &tx_entry->hdr;
	if (data_len <= xnet_max_inject) {
		ofi_copy_iov_buf(iov, count, 0, (uint8_t *) &tx_entry->hdr +
				 hdr_len, xnet_max_inject, OFI_COPY_IOV_TO_BUF);
		tx_entry->iov[0].iov_len = hdr_len + data_len;
		tx_entry->iov_cnt = 1;
	} else {
		tx_entry->iov[0].iov_len = hdr_len;
		tx_entry->iov_cnt = count + 1;
		if (count) {
			tx_entry->user_buf = iov[0].iov_base;
			memcpy(&tx_entry->iov[1], &iov[0],
			       count * sizeof(struct iovec));
		}
	}
}

/* If the transfer should use rendezvous protocol
 * (ready-to-send-> + <-clear-to-send + data->),
 * reformat for RTS-CTS flow.
 */
static int
xnet_rts_check(struct xnet_ep *ep, struct xnet_xfer_entry *tx_entry)
{
	uint64_t msg_len, hdr_size;
	uint8_t rts_ctx;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	assert(tx_entry->hdr.base_hdr.op == xnet_op_tag);

	if ((tx_entry->hdr.base_hdr.size <= xnet_max_saved_size) ||
	    !(ep->util_ep.flags & XNET_EP_RENDEZVOUS))
		return 0;

	/* User data is iov[1+] */
	assert(tx_entry->iov_cnt > 1);
	assert(tx_entry->iov[0].iov_len == tx_entry->hdr.base_hdr.hdr_size);

	rts_ctx = ofi_byte_idx_insert(&ep->rts_queue, tx_entry);
	if (!rts_ctx)
		return -FI_EAGAIN;

	msg_len = xnet_msg_len(&tx_entry->hdr);
	hdr_size = tx_entry->hdr.base_hdr.hdr_size;
	*(uint64_t *) (((uint8_t *) &tx_entry->hdr) + hdr_size) = msg_len;

	tx_entry->hdr.base_hdr.op = xnet_op_tag_rts;
	tx_entry->hdr.base_hdr.op_data = rts_ctx;
	tx_entry->hdr.base_hdr.hdr_size += sizeof(msg_len);
	tx_entry->hdr.base_hdr.size = tx_entry->hdr.base_hdr.hdr_size;

	tx_entry->iov[0].iov_len = tx_entry->hdr.base_hdr.size;
	tx_entry->rts_iov_cnt = tx_entry->iov_cnt;
	tx_entry->iov_cnt = 1;
	tx_entry->ctrl_flags |= XNET_NEED_CTS;
	return 0;
}

static inline bool
xnet_queue_recv(struct xnet_ep *ep, struct xnet_xfer_entry *recv_entry)
{
	bool ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	ret = ep->rx_avail;
	if (ret) {
		slist_insert_tail(&recv_entry->entry, &ep->rx_queue);
		ep->rx_avail--;

		if (xnet_has_unexp(ep)) {
			assert(!dlist_empty(&ep->unexp_entry));
			xnet_progress_rx(ep);
		}
	}
	return ret;
}

static ssize_t
xnet_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct xnet_xfer_entry *recv_entry;
	struct xnet_ep *ep;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	assert(msg->iov_count <= XNET_IOV_LIMIT);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	recv_entry = xnet_alloc_rx(ep);
	if (!recv_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	recv_entry->iov_cnt = msg->iov_count;
	if (msg->iov_count) {
		recv_entry->user_buf = msg->msg_iov[0].iov_base;
		memcpy(&recv_entry->iov[0], &msg->msg_iov[0],
		       msg->iov_count * sizeof(struct iovec));
	}

	recv_entry->cq_flags = (flags & FI_COMPLETION) | FI_MSG | FI_RECV;
	recv_entry->context = msg->context;

	if (!xnet_queue_recv(ep, recv_entry)) {
		xnet_free_xfer(xnet_ep2_progress(ep), recv_entry);
		ret = -FI_EAGAIN;
	}
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_recv(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
	  fi_addr_t src_addr, void *context)
{
	struct xnet_xfer_entry *recv_entry;
	struct xnet_ep *ep;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	recv_entry = xnet_alloc_rx(ep);
	if (!recv_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	recv_entry->user_buf = buf;
	recv_entry->iov_cnt = 1;
	recv_entry->iov[0].iov_base = buf;
	recv_entry->iov[0].iov_len = len;

	recv_entry->cq_flags = FI_MSG | FI_RECV;
	recv_entry->context = context;

	if (!xnet_queue_recv(ep, recv_entry)) {
		xnet_free_xfer(xnet_ep2_progress(ep), recv_entry);
		ret = -FI_EAGAIN;
	}
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_recvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
	   size_t count, fi_addr_t src_addr, void *context)
{
	struct xnet_xfer_entry *recv_entry;
	struct xnet_ep *ep;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	assert(count <= XNET_IOV_LIMIT);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	recv_entry = xnet_alloc_rx(ep);
	if (!recv_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	recv_entry->iov_cnt = count;
	if (count) {
		recv_entry->user_buf = iov[0].iov_base;
		memcpy(recv_entry->iov, iov, count * sizeof(*iov));
	}
	recv_entry->cq_flags = FI_MSG | FI_RECV;
	recv_entry->context = context;

	if (!xnet_queue_recv(ep, recv_entry)) {
		xnet_free_xfer(xnet_ep2_progress(ep), recv_entry);
		ret = -FI_EAGAIN;
	}
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	size_t hdr_len;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_send(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (flags & FI_REMOTE_CQ_DATA) {
		tx_entry->hdr.base_hdr.flags = XNET_REMOTE_CQ_DATA;
		tx_entry->hdr.cq_data_hdr.cq_data = msg->data;
		hdr_len = sizeof(tx_entry->hdr.cq_data_hdr);
	} else {
		hdr_len = sizeof(tx_entry->hdr.base_hdr);
	}

	xnet_init_tx_iov(tx_entry, hdr_len, msg->msg_iov, msg->iov_count);
	tx_entry->cq_flags = xnet_tx_completion_get_msgflags(ep, flags) |
			     FI_MSG | FI_SEND;
	xnet_set_ack_flags(tx_entry, flags);
	tx_entry->context = msg->context;

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_send(struct fid_ep *ep_fid, const void *buf, size_t len,
	  void *desc, fi_addr_t dest_addr, void *context)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_send(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	xnet_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.base_hdr), buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = xnet_tx_completion_get_opflags(ep) |
			     FI_MSG | FI_SEND;
	xnet_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
	   void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_send(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	xnet_init_tx_iov(tx_entry, sizeof(tx_entry->hdr.base_hdr), iov, count);
	tx_entry->context = context;
	tx_entry->cq_flags = xnet_tx_completion_get_opflags(ep) |
			     FI_MSG | FI_SEND;
	xnet_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}


static ssize_t
xnet_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
	    fi_addr_t dest_addr)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_send(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	xnet_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.base_hdr), buf, len);
	tx_entry->ctrl_flags = XNET_INJECT_OP;
	tx_entry->cq_flags = FI_INJECT | FI_MSG | FI_SEND;

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
	      void *desc, uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_send(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.cq_data_hdr.base_hdr.size =
		len + sizeof(tx_entry->hdr.cq_data_hdr);
	tx_entry->hdr.cq_data_hdr.base_hdr.flags = XNET_REMOTE_CQ_DATA;
	tx_entry->hdr.cq_data_hdr.cq_data = data;

	xnet_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.cq_data_hdr),
			 buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = xnet_tx_completion_get_opflags(ep) |
			     FI_MSG | FI_SEND;
	xnet_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		uint64_t data, fi_addr_t dest_addr)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_send(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.cq_data_hdr.base_hdr.flags = XNET_REMOTE_CQ_DATA;
	tx_entry->hdr.cq_data_hdr.cq_data = data;

	xnet_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.cq_data_hdr),
			    buf, len);
	tx_entry->ctrl_flags = XNET_INJECT_OP;
	tx_entry->cq_flags = FI_INJECT | FI_MSG | FI_SEND;

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

struct fi_ops_msg xnet_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = xnet_recv,
	.recvv = xnet_recvv,
	.recvmsg = xnet_recvmsg,
	.send = xnet_send,
	.sendv = xnet_sendv,
	.sendmsg = xnet_sendmsg,
	.inject = xnet_inject,
	.senddata = xnet_senddata,
	.injectdata = xnet_injectdata,
};

static ssize_t
xnet_tsendmsg(struct fid_ep *fid_ep, const struct fi_msg_tagged *msg,
	      uint64_t flags)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	size_t hdr_len;
	ssize_t ret;

	ep = container_of(fid_ep, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_tsend(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (flags & FI_REMOTE_CQ_DATA) {
		tx_entry->hdr.base_hdr.flags |= XNET_REMOTE_CQ_DATA;
		tx_entry->hdr.tag_data_hdr.cq_data = msg->data;
		tx_entry->hdr.tag_data_hdr.tag = msg->tag;
		hdr_len = sizeof(tx_entry->hdr.tag_data_hdr);
	} else {
		tx_entry->hdr.tag_hdr.tag = msg->tag;
		hdr_len = sizeof(tx_entry->hdr.tag_hdr);
	}

	xnet_init_tx_iov(tx_entry, hdr_len, msg->msg_iov, msg->iov_count);
	tx_entry->cq_flags = xnet_tx_completion_get_msgflags(ep, flags) |
			     FI_TAGGED | FI_SEND;
	xnet_set_ack_flags(tx_entry, flags);
	tx_entry->context = msg->context;

	ret = xnet_rts_check(ep, tx_entry);
	if (!ret)
		xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_tsend(struct fid_ep *fid_ep, const void *buf, size_t len,
	   void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret;

	ep = container_of(fid_ep, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_tsend(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.tag_hdr.tag = tag;

	xnet_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.tag_hdr), buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = xnet_tx_completion_get_opflags(ep) |
			     FI_TAGGED | FI_SEND;
	xnet_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	ret = xnet_rts_check(ep, tx_entry);
	if (!ret)
		xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_tsendv(struct fid_ep *fid_ep, const struct iovec *iov, void **desc,
	    size_t count, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret;

	ep = container_of(fid_ep, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_tsend(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.tag_hdr.tag = tag;

	xnet_init_tx_iov(tx_entry, sizeof(tx_entry->hdr.tag_hdr), iov, count);
	tx_entry->context = context;
	tx_entry->cq_flags = xnet_tx_completion_get_opflags(ep) |
			     FI_TAGGED | FI_SEND;
	xnet_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	ret = xnet_rts_check(ep, tx_entry);
	if (!ret)
		xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}


static ssize_t
xnet_tinject(struct fid_ep *fid_ep, const void *buf, size_t len,
	     fi_addr_t dest_addr, uint64_t tag)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(fid_ep, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_tsend(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.tag_hdr.tag = tag;

	xnet_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.tag_hdr), buf, len);
	tx_entry->ctrl_flags = XNET_INJECT_OP;
	tx_entry->cq_flags = FI_INJECT | FI_TAGGED | FI_SEND;

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_tsenddata(struct fid_ep *fid_ep, const void *buf, size_t len, void *desc,
	       uint64_t data, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret;

	ep = container_of(fid_ep, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_tsend(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.base_hdr.flags |= XNET_REMOTE_CQ_DATA;
	tx_entry->hdr.tag_data_hdr.tag = tag;
	tx_entry->hdr.tag_data_hdr.cq_data = data;

	xnet_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.tag_data_hdr),
			 buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = xnet_tx_completion_get_opflags(ep) |
			     FI_TAGGED | FI_SEND;
	xnet_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	ret = xnet_rts_check(ep, tx_entry);
	if (!ret)
		xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_tinjectdata(struct fid_ep *fid_ep, const void *buf, size_t len,
		 uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *tx_entry;
	ssize_t ret = 0;

	ep = container_of(fid_ep, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	tx_entry = xnet_alloc_tsend(ep);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	tx_entry->hdr.base_hdr.flags |= XNET_REMOTE_CQ_DATA;
	tx_entry->hdr.tag_data_hdr.tag = tag;
	tx_entry->hdr.tag_data_hdr.cq_data = data;

	xnet_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.tag_data_hdr),
			    buf, len);
	tx_entry->ctrl_flags = XNET_INJECT_OP;
	tx_entry->cq_flags = FI_INJECT | FI_TAGGED | FI_SEND;

	xnet_tx_queue_insert(ep, tx_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

struct fi_ops_tagged xnet_tagged_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
	.send = xnet_tsend,
	.sendv = xnet_tsendv,
	.sendmsg = xnet_tsendmsg,
	.inject = xnet_tinject,
	.senddata = xnet_tsenddata,
	.injectdata = xnet_tinjectdata,
};
