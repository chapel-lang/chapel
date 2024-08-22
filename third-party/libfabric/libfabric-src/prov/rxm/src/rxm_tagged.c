/*
 * Copyright (c) 2013-2021 Intel Corporation. All rights reserved.
 * Copyright (c) 2020 Cisco Systems, Inc.  All rights reserved.
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

#include <inttypes.h>
#include <math.h>

#include <rdma/fabric.h>
#include <rdma/fi_collective.h>
#include "ofi.h"
#include <ofi_util.h>

#include "rxm.h"


static void
rxm_discard_recv(struct rxm_ep *rxm_ep, struct rxm_rx_buf *rx_buf,
		 void *context)
{
	RXM_DBG_ADDR_TAG(FI_LOG_EP_DATA, "Discarding message",
			 rx_buf->unexp_msg.addr, rx_buf->unexp_msg.tag);

	rxm_cq_write(rxm_ep->util_ep.rx_cq, context, FI_TAGGED | FI_RECV,
		     0, NULL, rx_buf->pkt.hdr.data, rx_buf->pkt.hdr.tag);
	rxm_free_rx_buf(rx_buf);
}

static void
rxm_peek_recv(struct rxm_ep *rxm_ep, fi_addr_t addr, uint64_t tag,
	      uint64_t ignore, void *context, uint64_t flags,
	      struct rxm_recv_queue *recv_queue)
{
	struct rxm_rx_buf *rx_buf;
	int ret;

	RXM_DBG_ADDR_TAG(FI_LOG_EP_DATA, "Peeking message", addr, tag);

	/* peek doesn't support peer transfer at this moment */
	assert(!(flags & FI_PEER_TRANSFER));

	rxm_ep_do_progress(&rxm_ep->util_ep);

	rx_buf = rxm_get_unexp_msg(recv_queue, addr, tag, ignore);
	if (!rx_buf) {
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Message not found\n");
		ret = ofi_cq_write_error_peek(rxm_ep->util_ep.rx_cq, tag,
					      context);
		if (ret)
			FI_WARN(&rxm_prov, FI_LOG_CQ, "Error writing to CQ\n");
		return;
	}

	FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Message found\n");

	if (flags & FI_DISCARD) {
		dlist_remove(&rx_buf->unexp_msg.entry);
		rxm_discard_recv(rxm_ep, rx_buf, context);
		return;
	}

	if (flags & FI_CLAIM) {
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Marking message for Claim\n");
		((struct fi_context *)context)->internal[0] = rx_buf;
		dlist_remove(&rx_buf->unexp_msg.entry);
	}

	rxm_cq_write(rxm_ep->util_ep.rx_cq, context, FI_TAGGED | FI_RECV,
		     rx_buf->pkt.hdr.size, NULL,
		     rx_buf->pkt.hdr.data, rx_buf->pkt.hdr.tag);
}

static ssize_t
rxm_post_trecv(struct rxm_ep *rxm_ep, const struct iovec *iov,
	       void **desc, size_t count, fi_addr_t src_addr,
	       uint64_t tag, uint64_t ignore, void *context, uint64_t op_flags)
{
	struct rxm_recv_entry *recv_entry;
	struct rxm_rx_buf *rx_buf;

	assert(count <= rxm_ep->rxm_info->rx_attr->iov_limit);

	recv_entry = rxm_recv_entry_get(rxm_ep, iov, desc, count, src_addr,
					tag, ignore, context, op_flags,
					&rxm_ep->trecv_queue);
	if (!recv_entry)
		return -FI_EAGAIN;

	rx_buf = rxm_get_unexp_msg(&rxm_ep->trecv_queue, recv_entry->addr,
				   recv_entry->tag, recv_entry->ignore);
	if (!rx_buf) {
		dlist_insert_tail(&recv_entry->entry,
				  &rxm_ep->trecv_queue.recv_list);
		return FI_SUCCESS;
	}

	dlist_remove(&rx_buf->unexp_msg.entry);
	rx_buf->recv_entry = recv_entry;

	if (rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_seg)
		return rxm_handle_rx_buf(rx_buf);
	else
		return rxm_handle_unexp_sar(&rxm_ep->trecv_queue, recv_entry,
					    rx_buf);
}

static ssize_t
rxm_trecv_common(struct rxm_ep *rxm_ep, const struct iovec *iov,
		 void **desc, size_t count, fi_addr_t src_addr,
		 uint64_t tag, uint64_t ignore, void *context,
		 uint64_t op_flags)
{
	ssize_t ret;

	if (op_flags & FI_PEER_TRANSFER)
		tag |= RXM_PEER_XFER_TAG_FLAG;

	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_post_trecv(rxm_ep, iov, desc, count, src_addr,
			     tag, ignore, context, op_flags);
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
	     uint64_t flags)
{
	struct rxm_ep *rxm_ep;
	struct rxm_recv_entry *recv_entry;
	struct fi_recv_context *recv_ctx;
	struct rxm_rx_buf *rx_buf;
	void *context = msg->context;
	ssize_t ret = 0;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	flags |= rxm_ep->util_ep.rx_msg_flags;

	if (!(flags & (FI_CLAIM | FI_PEEK)) &&
	    !(rxm_ep->rxm_info->mode & FI_BUFFERED_RECV)) {
		return rxm_trecv_common(rxm_ep, msg->msg_iov, msg->desc,
					msg->iov_count, msg->addr,
					msg->tag, msg->ignore, context, flags);
	}

	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	if (rxm_ep->rxm_info->mode & FI_BUFFERED_RECV) {
		recv_ctx = msg->context;
		context = recv_ctx->context;
		rx_buf = container_of(recv_ctx, struct rxm_rx_buf, recv_context);

		if (flags & FI_CLAIM) {
			FI_DBG(&rxm_prov, FI_LOG_EP_DATA,
			       "Claiming buffered receive\n");
			goto claim;
		}

		assert(flags & FI_DISCARD);
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Discarding buffered receive\n");
		rxm_free_rx_buf(rx_buf);
		goto unlock;
	}

	if (flags & FI_PEEK) {
		rxm_peek_recv(rxm_ep, msg->addr, msg->tag, msg->ignore,
			      context, flags, &rxm_ep->trecv_queue);
		goto unlock;
	}

	rx_buf = ((struct fi_context *) context)->internal[0];
	assert(rx_buf);
	FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Claim message\n");

	if (flags & FI_DISCARD) {
		rxm_discard_recv(rxm_ep, rx_buf, context);
		goto unlock;
	}

claim:
	assert (flags & FI_CLAIM);
	recv_entry = rxm_recv_entry_get(rxm_ep, msg->msg_iov, msg->desc,
					msg->iov_count, msg->addr,
					msg->tag, msg->ignore, context, flags,
					&rxm_ep->trecv_queue);
	if (!recv_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (rxm_ep->rxm_info->mode & FI_BUFFERED_RECV)
		recv_entry->comp_flags |= FI_CLAIM;

	rx_buf->recv_entry = recv_entry;
	ret = rxm_handle_rx_buf(rx_buf);

unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
	  void *desc, fi_addr_t src_addr, uint64_t tag,
	  uint64_t ignore, void *context)
{
	struct rxm_ep *rxm_ep;
	struct iovec iov = {
		.iov_base	= buf,
		.iov_len	= len,
	};

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_trecv_common(rxm_ep, &iov, &desc, 1, src_addr, tag, ignore,
				context, rxm_ep->util_ep.rx_op_flags);
}

static ssize_t
rxm_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
	   void **desc, size_t count, fi_addr_t src_addr,
	   uint64_t tag, uint64_t ignore, void *context)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_trecv_common(rxm_ep, iov, desc, count, src_addr, tag,
				ignore, context, rxm_ep->util_ep.rx_op_flags);
}

static ssize_t
rxm_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
	     uint64_t flags)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_get_conn(rxm_ep, msg->addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_send_common(rxm_ep, rxm_conn, msg->msg_iov, msg->desc,
			      msg->iov_count, msg->context, msg->data,
			      flags | rxm_ep->util_ep.tx_msg_flags, msg->tag,
			      ofi_op_tagged);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
	  void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	struct iovec iov = {
		.iov_base = (void *) buf,
		.iov_len = len,
	};
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_send_common(rxm_ep, rxm_conn, &iov, &desc, 1, context, 0,
			      rxm_ep->util_ep.tx_op_flags, tag, ofi_op_tagged);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
	   void **desc, size_t count, fi_addr_t dest_addr,
	   uint64_t tag, void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_send_common(rxm_ep, rxm_conn, iov, desc, count, context, 0,
			      rxm_ep->util_ep.tx_op_flags, tag, ofi_op_tagged);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
	    fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	rxm_ep->inject_pkt->hdr.op = ofi_op_tagged;
	rxm_ep->inject_pkt->hdr.flags = 0;
	rxm_ep->inject_pkt->hdr.tag = tag;
	rxm_ep->inject_pkt->hdr.data = 0;

	ret = rxm_inject_send(rxm_ep, rxm_conn, buf, len);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
	      void *desc, uint64_t data, fi_addr_t dest_addr,
	      uint64_t tag, void *context)
{
	struct rxm_conn *rxm_conn;
	struct iovec iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_send_common(rxm_ep, rxm_conn, &iov, &desc, 1, context, data,
			      rxm_ep->util_ep.tx_op_flags | FI_REMOTE_CQ_DATA,
			tag, ofi_op_tagged);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	rxm_ep->inject_pkt->hdr.op = ofi_op_tagged;
	rxm_ep->inject_pkt->hdr.flags = FI_REMOTE_CQ_DATA;
	rxm_ep->inject_pkt->hdr.tag = tag;
	rxm_ep->inject_pkt->hdr.data = data;

	ret = rxm_inject_send(rxm_ep, rxm_conn, buf, len);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

struct fi_ops_tagged rxm_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = rxm_trecv,
	.recvv = rxm_trecvv,
	.recvmsg = rxm_trecvmsg,
	.send = rxm_tsend,
	.sendv = rxm_tsendv,
	.sendmsg = rxm_tsendmsg,
	.inject = rxm_tinject,
	.senddata = rxm_tsenddata,
	.injectdata = rxm_tinjectdata,
};


static ssize_t
rxm_trecv_thru(struct fid_ep *ep_fid, void *buf, size_t len,
	       void *desc, fi_addr_t src_addr,
	       uint64_t tag, uint64_t ignore,  void *context)
{
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	assert(ep->msg_srx);

	ofi_genlock_lock(&ep->util_ep.lock);
	ret = fi_trecv(ep->msg_srx, buf, len, desc, src_addr, tag, ignore,
		       context);
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_trecvv_thru(struct fid_ep *ep_fid, const struct iovec *iov,
	        void **desc, size_t count, fi_addr_t src_addr,
		uint64_t tag, uint64_t ignore, void *context)
{
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	assert(ep->msg_srx);

	ofi_genlock_lock(&ep->util_ep.lock);
	ret = fi_trecvv(ep->msg_srx, iov, desc, count, src_addr, tag, ignore,
			context);
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_trecvmsg_thru(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
		  uint64_t flags)
{
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	assert(ep->msg_srx);

	ofi_genlock_lock(&ep->util_ep.lock);
	ret = fi_trecvmsg(ep->msg_srx, msg, flags);
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsend_thru(struct fid_ep *ep_fid, const void *buf, size_t len,
	       void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct rxm_conn *conn;
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsend(conn->msg_ep, buf, len, desc, 0, tag, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsendv_thru(struct fid_ep *ep_fid, const struct iovec *iov,
	        void **desc, size_t count, fi_addr_t dest_addr,
		uint64_t tag, void *context)
{
	struct rxm_conn *conn;
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsendv(conn->msg_ep, iov, desc, count, 0, tag, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsendmsg_thru(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
		  uint64_t flags)
{
	struct rxm_conn *conn;
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsendmsg(conn->msg_ep, msg, flags);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tinject_thru(struct fid_ep *ep_fid, const void *buf,
		 size_t len, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *conn;
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tinject(conn->msg_ep, buf, len, 0, tag);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tsenddata_thru(struct fid_ep *ep_fid, const void *buf, size_t len,
		   void *desc, uint64_t data, fi_addr_t dest_addr,
		   uint64_t tag, void *context)
{
	struct rxm_conn *conn;
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsenddata(conn->msg_ep, buf, len, desc, data, 0, tag, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_tinjectdata_thru(struct fid_ep *ep_fid, const void *buf, size_t len,
		     uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *conn;
	struct rxm_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tinjectdata(conn->msg_ep, buf, len, data, 0, tag);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

struct fi_ops_tagged rxm_tagged_thru_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = rxm_trecv_thru,
	.recvv = rxm_trecvv_thru,
	.recvmsg = rxm_trecvmsg_thru,
	.send = rxm_tsend_thru,
	.sendv = rxm_tsendv_thru,
	.sendmsg = rxm_tsendmsg_thru,
	.inject = rxm_tinject_thru,
	.senddata = rxm_tsenddata_thru,
	.injectdata = rxm_tinjectdata_thru,
};
