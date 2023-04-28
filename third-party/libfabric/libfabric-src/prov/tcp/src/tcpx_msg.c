/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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
#include "tcpx.h"

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


static inline struct tcpx_xfer_entry *
tcpx_alloc_send(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *send_entry;

	send_entry = tcpx_alloc_tx(ep);
	if (send_entry)
		send_entry->hdr.base_hdr.op = ofi_op_msg;

	return send_entry;
}

/* When dynamic receive buffers are enabled, receive buffer matching
 * is handled by the upper layer (rxm).  The tcp provider is only
 * carrying the tag to reduce header overhead.  The transport operation
 * is still op_msg at the tcp provider.  This is needed for backwards
 * compatibility.
 *
 * If dynamic receive buffers are disabled, then tagged messages are
 * being handled entirely by the tcp provider.  We use the op_tagged
 * protocol for this, which allows distinguishing between the two
 * cases at the receiver.
 *
 * We assume the peer is configured similar to the local side, which
 * is all we can check.
 */
static inline struct tcpx_xfer_entry *
tcpx_alloc_tsend(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *send_entry;

	send_entry = tcpx_alloc_tx(ep);
	if (send_entry) {
		if (tcpx_dynamic_rbuf(ep)) {
			send_entry->hdr.base_hdr.op = ofi_op_msg;
			send_entry->hdr.base_hdr.flags = TCPX_TAGGED;
		} else {
			assert(ep->srx_ctx);
			send_entry->hdr.base_hdr.op = ofi_op_tagged;
		}
	}

	return send_entry;
}

static inline void
tcpx_init_tx_sizes(struct tcpx_xfer_entry *tx_entry, size_t hdr_len,
		   size_t data_len)
{
	tx_entry->hdr.base_hdr.size = hdr_len + data_len;
	tx_entry->hdr.base_hdr.hdr_size = (uint8_t) hdr_len;
}

static inline void
tcpx_init_tx_inject(struct tcpx_xfer_entry *tx_entry, size_t hdr_len,
		    const void *buf, size_t data_len)
{
	assert(data_len <= TCPX_MAX_INJECT);
	tcpx_init_tx_sizes(tx_entry, hdr_len, data_len);

	tx_entry->iov[0].iov_base = (void *) &tx_entry->hdr;
	memcpy((uint8_t *) &tx_entry->hdr + hdr_len, (uint8_t *) buf,
		data_len);
	tx_entry->iov[0].iov_len = hdr_len + data_len;
	tx_entry->iov_cnt = 1;
}

static inline void
tcpx_init_tx_buf(struct tcpx_xfer_entry *tx_entry, size_t hdr_len,
		 const void *buf, size_t data_len)
{
	if (data_len <= TCPX_MAX_INJECT) {
		tcpx_init_tx_inject(tx_entry, hdr_len, buf, data_len);
		return;
	}

	tcpx_init_tx_sizes(tx_entry, hdr_len, data_len);
	tx_entry->iov[0].iov_base = (void *) &tx_entry->hdr;
	tx_entry->iov[0].iov_len = hdr_len;
	tx_entry->iov[1].iov_base = (void *) buf;
	tx_entry->iov[1].iov_len = data_len;
	tx_entry->iov_cnt = 2;
}

static inline void
tcpx_init_tx_iov(struct tcpx_xfer_entry *tx_entry, size_t hdr_len,
		 const struct iovec *iov, size_t count)
{
	size_t data_len;

	assert(count <= TCPX_IOV_LIMIT);
	data_len = ofi_total_iov_len(iov, count);
	tcpx_init_tx_sizes(tx_entry, hdr_len, data_len);

	tx_entry->iov[0].iov_base = (void *) &tx_entry->hdr;
	if (data_len <= TCPX_MAX_INJECT) {
		ofi_copy_iov_buf(iov, count, 0, (uint8_t *) &tx_entry->hdr +
				 hdr_len, TCPX_MAX_INJECT, OFI_COPY_IOV_TO_BUF);
		tx_entry->iov[0].iov_len = hdr_len + data_len;
		tx_entry->iov_cnt = 1;
	} else {
		tx_entry->iov[0].iov_len = hdr_len;
		tx_entry->iov_cnt = count + 1;
		memcpy(&tx_entry->iov[1], &iov[0], count * sizeof(struct iovec));
	}
}

static inline bool
tcpx_queue_recv(struct tcpx_ep *ep, struct tcpx_xfer_entry *recv_entry)
{
	bool ret;

	ofi_mutex_lock(&ep->lock);
	ret = ep->rx_avail;
	if (ret) {
		slist_insert_tail(&recv_entry->entry, &ep->rx_queue);
		ep->rx_avail--;
	}
	ofi_mutex_unlock(&ep->lock);
	return ret;
}

static ssize_t
tcpx_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct tcpx_xfer_entry *recv_entry;
	struct tcpx_ep *ep;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	assert(msg->iov_count <= TCPX_IOV_LIMIT);

	recv_entry = tcpx_alloc_rx(ep);
	if (!recv_entry)
		return -FI_EAGAIN;

	recv_entry->iov_cnt = msg->iov_count;
	memcpy(&recv_entry->iov[0], &msg->msg_iov[0],
	       msg->iov_count * sizeof(struct iovec));

	recv_entry->cq_flags = tcpx_rx_completion_flag(ep, flags) |
			       FI_MSG | FI_RECV;
	recv_entry->context = msg->context;

	if (!tcpx_queue_recv(ep, recv_entry)) {
		tcpx_free_rx(recv_entry);
		return -FI_EAGAIN;
	}
	return FI_SUCCESS;
}

static ssize_t
tcpx_recv(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
	  fi_addr_t src_addr, void *context)
{
	struct tcpx_xfer_entry *recv_entry;
	struct tcpx_ep *ep;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	recv_entry = tcpx_alloc_rx(ep);
	if (!recv_entry)
		return -FI_EAGAIN;

	recv_entry->iov_cnt = 1;
	recv_entry->iov[0].iov_base = buf;
	recv_entry->iov[0].iov_len = len;

	recv_entry->cq_flags = tcpx_rx_completion_flag(ep, 0) |
			       FI_MSG | FI_RECV;
	recv_entry->context = context;

	if (!tcpx_queue_recv(ep, recv_entry)) {
		tcpx_free_rx(recv_entry);
		return -FI_EAGAIN;
	}
	return FI_SUCCESS;
}

static ssize_t
tcpx_recvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
	   size_t count, fi_addr_t src_addr, void *context)
{
	struct tcpx_xfer_entry *recv_entry;
	struct tcpx_ep *ep;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	assert(count <= TCPX_IOV_LIMIT);

	recv_entry = tcpx_alloc_rx(ep);
	if (!recv_entry)
		return -FI_EAGAIN;

	recv_entry->iov_cnt = count;
	memcpy(recv_entry->iov, iov, count * sizeof(*iov));

	recv_entry->cq_flags = tcpx_rx_completion_flag(ep, 0) |
			       FI_MSG | FI_RECV;
	recv_entry->context = context;

	if (!tcpx_queue_recv(ep, recv_entry)) {
		tcpx_free_rx(recv_entry);
		return -FI_EAGAIN;
	}
	return FI_SUCCESS;
}

static inline void
tcpx_queue_send(struct tcpx_ep *ep, struct tcpx_xfer_entry *tx_entry)
{
	ofi_mutex_lock(&ep->lock);
	tcpx_tx_queue_insert(ep, tx_entry);
	ofi_mutex_unlock(&ep->lock);
}

static ssize_t
tcpx_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;
	size_t hdr_len;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	tx_entry = tcpx_alloc_send(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	if (flags & FI_REMOTE_CQ_DATA) {
		tx_entry->hdr.base_hdr.flags = TCPX_REMOTE_CQ_DATA;
		tx_entry->hdr.cq_data_hdr.cq_data = msg->data;
		hdr_len = sizeof(tx_entry->hdr.cq_data_hdr);
	} else {
		hdr_len = sizeof(tx_entry->hdr.base_hdr);
	}

	tcpx_init_tx_iov(tx_entry, hdr_len, msg->msg_iov, msg->iov_count);
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, flags) |
			     FI_MSG | FI_SEND;
	tcpx_set_ack_flags(tx_entry, flags);
	tx_entry->context = msg->context;

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_send(struct fid_ep *ep_fid, const void *buf, size_t len,
	  void *desc, fi_addr_t dest_addr, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	tx_entry = tcpx_alloc_send(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tcpx_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.base_hdr), buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, 0) |
			     FI_MSG | FI_SEND;
	tcpx_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
	   void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	tx_entry = tcpx_alloc_send(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tcpx_init_tx_iov(tx_entry, sizeof(tx_entry->hdr.base_hdr), iov, count);
	tx_entry->context = context;
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, 0) |
			     FI_MSG | FI_SEND;
	tcpx_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}


static ssize_t
tcpx_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
	    fi_addr_t dest_addr)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	tx_entry = tcpx_alloc_send(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tcpx_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.base_hdr), buf, len);
	tx_entry->ctrl_flags = TCPX_INJECT_OP;

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
	      void *desc, uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	tx_entry = tcpx_alloc_send(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tx_entry->hdr.cq_data_hdr.base_hdr.size =
		len + sizeof(tx_entry->hdr.cq_data_hdr);
	tx_entry->hdr.cq_data_hdr.base_hdr.flags = TCPX_REMOTE_CQ_DATA;
	tx_entry->hdr.cq_data_hdr.cq_data = data;

	tcpx_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.cq_data_hdr),
			 buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, 0) |
			     FI_MSG | FI_SEND;
	tcpx_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		uint64_t data, fi_addr_t dest_addr)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	tx_entry = tcpx_alloc_send(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tx_entry->hdr.cq_data_hdr.base_hdr.flags = TCPX_REMOTE_CQ_DATA;
	tx_entry->hdr.cq_data_hdr.cq_data = data;

	tcpx_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.cq_data_hdr),
			    buf, len);
	tx_entry->ctrl_flags = TCPX_INJECT_OP;

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

struct fi_ops_msg tcpx_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = tcpx_recv,
	.recvv = tcpx_recvv,
	.recvmsg = tcpx_recvmsg,
	.send = tcpx_send,
	.sendv = tcpx_sendv,
	.sendmsg = tcpx_sendmsg,
	.inject = tcpx_inject,
	.senddata = tcpx_senddata,
	.injectdata = tcpx_injectdata,
};


/* There's no application driven need for tagged message operations over
 * connected endpoints.  The tcp provider exposes the ability to send
 * tagged messages using the tcp header, with the expectation that the
 * peer side is using dynamic receive buffers to match the tagged messages
 * with application buffers.  This provides an optimized path for rxm
 * over tcp, that allows rxm to drop its header in certain cases an only
 * use a minimal tcp header.
 */
static ssize_t
tcpx_tsendmsg(struct fid_ep *fid_ep, const struct fi_msg_tagged *msg,
	      uint64_t flags)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;
	size_t hdr_len;

	ep = container_of(fid_ep, struct tcpx_ep, util_ep.ep_fid);
	tx_entry = tcpx_alloc_tsend(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	if (flags & FI_REMOTE_CQ_DATA) {
		tx_entry->hdr.base_hdr.flags |= TCPX_REMOTE_CQ_DATA;
		tx_entry->hdr.tag_data_hdr.cq_data_hdr.cq_data = msg->data;
		tx_entry->hdr.tag_data_hdr.tag = msg->tag;
		hdr_len = sizeof(tx_entry->hdr.tag_data_hdr);
	} else {
		tx_entry->hdr.tag_hdr.tag = msg->tag;
		hdr_len = sizeof(tx_entry->hdr.tag_hdr);
	}

	tcpx_init_tx_iov(tx_entry, hdr_len, msg->msg_iov, msg->iov_count);
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, flags) |
			     FI_TAGGED | FI_SEND;
	tcpx_set_ack_flags(tx_entry, flags);
	tx_entry->context = msg->context;

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_tsend(struct fid_ep *fid_ep, const void *buf, size_t len,
	   void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(fid_ep, struct tcpx_ep, util_ep.ep_fid);
	tx_entry = tcpx_alloc_tsend(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tx_entry->hdr.tag_hdr.tag = tag;

	tcpx_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.tag_hdr), buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, 0) |
			     FI_TAGGED | FI_SEND;
	tcpx_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_tsendv(struct fid_ep *fid_ep, const struct iovec *iov, void **desc,
	    size_t count, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(fid_ep, struct tcpx_ep, util_ep.ep_fid);
	tx_entry = tcpx_alloc_tsend(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tx_entry->hdr.tag_hdr.tag = tag;

	tcpx_init_tx_iov(tx_entry, sizeof(tx_entry->hdr.tag_hdr), iov, count);
	tx_entry->context = context;
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, 0) |
			     FI_TAGGED | FI_SEND;
	tcpx_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}


static ssize_t
tcpx_tinject(struct fid_ep *fid_ep, const void *buf, size_t len,
	     fi_addr_t dest_addr, uint64_t tag)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(fid_ep, struct tcpx_ep, util_ep.ep_fid);
	tx_entry = tcpx_alloc_tsend(ep);
	if (!tx_entry)
		return -FI_EAGAIN;
	tx_entry->hdr.tag_hdr.tag = tag;

	tcpx_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.tag_hdr), buf, len);
	tx_entry->ctrl_flags = TCPX_INJECT_OP;

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_tsenddata(struct fid_ep *fid_ep, const void *buf, size_t len, void *desc,
	       uint64_t data, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(fid_ep, struct tcpx_ep, util_ep.ep_fid);
	tx_entry = tcpx_alloc_tsend(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tx_entry->hdr.base_hdr.flags |= TCPX_REMOTE_CQ_DATA;
	tx_entry->hdr.tag_data_hdr.tag = tag;
	tx_entry->hdr.tag_data_hdr.cq_data_hdr.cq_data = data;

	tcpx_init_tx_buf(tx_entry, sizeof(tx_entry->hdr.tag_data_hdr),
			 buf, len);
	tx_entry->context = context;
	tx_entry->cq_flags = tcpx_tx_completion_flag(ep, 0) |
			     FI_TAGGED | FI_SEND;
	tcpx_set_ack_flags(tx_entry, ep->util_ep.tx_op_flags);

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

static ssize_t
tcpx_tinjectdata(struct fid_ep *fid_ep, const void *buf, size_t len,
		 uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *tx_entry;

	ep = container_of(fid_ep, struct tcpx_ep, util_ep.ep_fid);

	tx_entry = tcpx_alloc_tsend(ep);
	if (!tx_entry)
		return -FI_EAGAIN;

	tx_entry->hdr.base_hdr.flags |= TCPX_REMOTE_CQ_DATA;
	tx_entry->hdr.tag_data_hdr.tag = tag;
	tx_entry->hdr.tag_data_hdr.cq_data_hdr.cq_data = data;

	tcpx_init_tx_inject(tx_entry, sizeof(tx_entry->hdr.tag_data_hdr),
			    buf, len);
	tx_entry->ctrl_flags = TCPX_INJECT_OP;

	tcpx_queue_send(ep, tx_entry);
	return FI_SUCCESS;
}

struct fi_ops_tagged tcpx_tagged_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
	.send = tcpx_tsend,
	.sendv = tcpx_tsendv,
	.sendmsg = tcpx_tsendmsg,
	.inject = tcpx_tinject,
	.senddata = tcpx_tsenddata,
	.injectdata = tcpx_tinjectdata,
};
