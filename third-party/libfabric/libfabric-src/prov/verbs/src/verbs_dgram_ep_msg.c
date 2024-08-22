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

#include "verbs_ofi.h"

static inline int
vrb_dgram_ep_set_addr(struct vrb_ep *ep, fi_addr_t addr,
			 struct ibv_send_wr *wr)
{
	struct vrb_dgram_av_entry *av_entry =
			vrb_dgram_av_lookup_av_entry(addr);
	if (OFI_UNLIKELY(!av_entry))
		return -FI_ENOENT;
	wr->wr.ud.ah = av_entry->ah;
	wr->wr.ud.remote_qpn = av_entry->addr.qpn;
	wr->wr.ud.remote_qkey = 0x11111111;

	return 0;
}

static inline ssize_t
vrb_dgram_ep_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_recv_wr wr = {
		.wr_id = (uintptr_t)msg->context,
		.num_sge = msg->iov_count,
		.next = NULL,
	};

	vrb_iov_dupa(wr.sg_list, msg->msg_iov, msg->desc, msg->iov_count);
	return vrb_post_recv(ep, &wr);
}

static inline ssize_t
vrb_dgram_ep_recvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		      size_t count, fi_addr_t src_addr, void *context)
{
	struct fi_msg msg = {
		.msg_iov	= iov,
		.desc		= desc,
		.iov_count	= count,
		.addr		= src_addr,
		.context	= context,
	};

	return vrb_dgram_ep_recvmsg(ep_fid, &msg, 0);
}

static inline ssize_t
vrb_dgram_ep_recv(struct fid_ep *ep_fid, void *buf, size_t len,
		     void *desc, fi_addr_t src_addr, void *context)
{
	struct iovec iov = {
		.iov_base	= buf,
		.iov_len	= len,
	};

	return vrb_dgram_ep_recvv(ep_fid, &iov, &desc,
				     1, src_addr, context);
}

static ssize_t
vrb_dgram_ep_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = (uintptr_t)msg->context,
	};

	if (flags & FI_REMOTE_CQ_DATA) {
		wr.opcode = IBV_WR_SEND_WITH_IMM;
		wr.imm_data = htonl((uint32_t)msg->data);
	} else {
		wr.opcode = IBV_WR_SEND;
	}

	if (vrb_dgram_ep_set_addr(ep, msg->addr, &wr))
		return -FI_ENOENT;

	return vrb_send_iov(ep, &wr, msg->msg_iov, msg->desc,
			    msg->iov_count, flags);
}

static inline ssize_t
vrb_dgram_ep_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
		      void **desc, size_t count, fi_addr_t dest_addr,
		      void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = (uintptr_t)context,
		.opcode = IBV_WR_SEND,
	};

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &wr))
		return -FI_ENOENT;

	return vrb_send_iov(ep, &wr, iov, desc, count,
			    ep->util_ep.tx_op_flags);
}

static ssize_t
vrb_dgram_ep_send(struct fid_ep *ep_fid, const void *buf, size_t len,
		     void *desc, fi_addr_t dest_addr, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.opcode = IBV_WR_SEND,
		.send_flags = VERBS_INJECT(ep, len, desc),
	};

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &wr))
		return -FI_ENOENT;

	return vrb_send_buf(ep, &wr, buf, len, desc);
}

static inline ssize_t
vrb_dgram_ep_senddata(struct fid_ep *ep_fid, const void *buf,
			 size_t len, void *desc, uint64_t data,
			 fi_addr_t dest_addr, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.opcode = IBV_WR_SEND_WITH_IMM,
		.send_flags = VERBS_INJECT(ep, len, desc),
	};

	wr.imm_data = htonl((uint32_t)data);

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &wr))
		return -FI_ENOENT;

	return vrb_send_buf(ep, &wr, buf, len, desc);
}

static ssize_t
vrb_dgram_ep_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
			   uint64_t data, fi_addr_t dest_addr)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_NO_COMP_FLAG,
		.opcode = IBV_WR_SEND_WITH_IMM,
		.send_flags = IBV_SEND_INLINE,
	};

	wr.imm_data = htonl((uint32_t)data);

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &wr))
		return -FI_ENOENT;

	return vrb_send_buf(ep, &wr, buf, len, NULL);
}

static ssize_t
vrb_dgram_ep_injectdata_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
				uint64_t data, fi_addr_t dest_addr)
{
	ssize_t ret;
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);

	ep->wrs->msg_wr.imm_data = htonl((uint32_t)data);
	ep->wrs->msg_wr.opcode = IBV_WR_SEND_WITH_IMM;

	ep->wrs->sge.addr = (uintptr_t) buf;
	ep->wrs->sge.length = (uint32_t) len;

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &ep->wrs->msg_wr))
		return -FI_ENOENT;

	ret = vrb_post_send(ep, &ep->wrs->msg_wr, 0);
	ep->wrs->msg_wr.opcode = IBV_WR_SEND;
	return ret;
}

static ssize_t
vrb_dgram_ep_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
		       fi_addr_t dest_addr)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_NO_COMP_FLAG,
		.opcode = IBV_WR_SEND,
		.send_flags = IBV_SEND_INLINE,
	};

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &wr))
		return -FI_ENOENT;

	return vrb_send_buf(ep, &wr, buf, len, NULL);
}

static ssize_t
vrb_dgram_ep_inject_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
			    fi_addr_t dest_addr)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);

	ep->wrs->sge.addr = (uintptr_t) buf;
	ep->wrs->sge.length = (uint32_t) len;

	if (vrb_dgram_ep_set_addr(ep, dest_addr, &ep->wrs->msg_wr))
		return -FI_ENOENT;

	return vrb_post_send(ep, &ep->wrs->msg_wr, 0);
}

const struct fi_ops_msg vrb_dgram_msg_ops = {
	.size		= sizeof(vrb_dgram_msg_ops),
	.recv		= vrb_dgram_ep_recv,
	.recvv		= vrb_dgram_ep_recvv,
	.recvmsg	= vrb_dgram_ep_recvmsg,
	.send		= vrb_dgram_ep_send,
	.sendv		= vrb_dgram_ep_sendv,
	.sendmsg	= vrb_dgram_ep_sendmsg,
	.inject		= vrb_dgram_ep_inject_fast,
	.senddata	= vrb_dgram_ep_senddata,
	.injectdata	= vrb_dgram_ep_injectdata_fast,
};

const struct fi_ops_msg vrb_dgram_msg_ops_ts = {
	.size		= sizeof(vrb_dgram_msg_ops),
	.recv		= vrb_dgram_ep_recv,
	.recvv		= vrb_dgram_ep_recvv,
	.recvmsg	= vrb_dgram_ep_recvmsg,
	.send		= vrb_dgram_ep_send,
	.sendv		= vrb_dgram_ep_sendv,
	.sendmsg	= vrb_dgram_ep_sendmsg,
	.inject		= vrb_dgram_ep_inject,
	.senddata	= vrb_dgram_ep_senddata,
	.injectdata	= vrb_dgram_ep_injectdata,
};
