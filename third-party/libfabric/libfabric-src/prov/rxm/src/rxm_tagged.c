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


static ssize_t
rxm_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
	     uint64_t flags)
{
	uint64_t tag = msg->tag;
	struct rxm_ep *rxm_ep = container_of(ep_fid, struct rxm_ep,
					     util_ep.ep_fid.fid);

	if (flags & FI_PEER_TRANSFER)
		tag |= RXM_PEER_XFER_TAG_FLAG;

	return util_srx_generic_trecv(&rxm_ep->srx->ep_fid, msg->msg_iov,
				      msg->desc, msg->iov_count, msg->addr,
				      msg->context, tag, msg->ignore,
				      flags | rxm_ep->util_ep.rx_msg_flags);
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
	return util_srx_generic_trecv(&rxm_ep->srx->ep_fid, &iov, &desc, 1,
				      src_addr, context, tag, ignore,
				      rxm_ep->util_ep.rx_op_flags);
}

static ssize_t
rxm_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
	   void **desc, size_t count, fi_addr_t src_addr,
	   uint64_t tag, uint64_t ignore, void *context)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return util_srx_generic_trecv(&rxm_ep->srx->ep_fid, iov, desc, count,
				      src_addr, context, tag, ignore,
				      rxm_ep->util_ep.rx_op_flags);
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

struct fi_ops_tagged rxm_no_recv_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
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
