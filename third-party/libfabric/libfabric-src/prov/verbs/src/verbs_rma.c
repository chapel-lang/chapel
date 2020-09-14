/*
 * Copyright (c) 2013-2018 Intel Corporation, Inc.  All rights reserved.
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

#include "config.h"

#include "fi_verbs.h"


#define VERBS_COMP_READ_FLAGS(ep, flags, context)		\
	(((ep)->util_ep.tx_op_flags | (flags)) &			\
	 (FI_COMPLETION | FI_TRANSMIT_COMPLETE |		\
	  FI_DELIVERY_COMPLETE) ? context : VERBS_NO_COMP_FLAG)

#define VERBS_COMP_READ(ep, context)		\
	VERBS_COMP_READ_FLAGS(ep, 0, context)

static ssize_t
vrb_msg_ep_rma_write(struct fid_ep *ep_fid, const void *buf, size_t len,
		     void *desc, fi_addr_t dest_addr,
		     uint64_t addr, uint64_t key, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = VERBS_INJECT(ep, len),
	};

	return vrb_send_buf(ep, &wr, buf, len, desc);
}

static ssize_t
vrb_msg_ep_rma_writev(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		      size_t count, fi_addr_t dest_addr,
		      uint64_t addr, uint64_t key, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = (uintptr_t)context,
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
	};

	return vrb_send_iov(ep, &wr, iov, desc, count);
}

static ssize_t
vrb_msg_ep_rma_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
			uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = (uintptr_t)msg->context,
		.wr.rdma.remote_addr = msg->rma_iov->addr,
		.wr.rdma.rkey = (uint32_t)msg->rma_iov->key,
	};

	if (flags & FI_REMOTE_CQ_DATA) {
		wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
		wr.imm_data = htonl((uint32_t)msg->data);
	} else {
		wr.opcode = IBV_WR_RDMA_WRITE;
	}

	return vrb_send_msg(ep, &wr, msg, flags);
}

static ssize_t
vrb_msg_ep_rma_read(struct fid_ep *ep_fid, void *buf, size_t len,
		    void *desc, fi_addr_t src_addr,
		    uint64_t addr, uint64_t key, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_READ(ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_READ,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
	};

	return vrb_send_buf(ep, &wr, buf, len, desc);
}

static ssize_t
vrb_msg_ep_rma_readv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		     size_t count, fi_addr_t src_addr,
		     uint64_t addr, uint64_t key, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_READ(ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_READ,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.num_sge = count,
	};

	vrb_set_sge_iov(wr.sg_list, iov, count, desc);

	return vrb_post_send(ep, &wr);
}

static ssize_t
vrb_msg_ep_rma_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
			uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_READ_FLAGS(ep, flags, (uintptr_t)msg->context),
		.opcode = IBV_WR_RDMA_READ,
		.wr.rdma.remote_addr = msg->rma_iov->addr,
		.wr.rdma.rkey = (uint32_t)msg->rma_iov->key,
		.num_sge = msg->iov_count,
	};

	vrb_set_sge_iov(wr.sg_list, msg->msg_iov, msg->iov_count, msg->desc);

	return vrb_post_send(ep, &wr);
}

static ssize_t
vrb_msg_ep_rma_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
			void *desc, uint64_t data, fi_addr_t dest_addr,
			uint64_t addr, uint64_t key, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_WRITE_WITH_IMM,
		.imm_data = htonl((uint32_t)data),
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = VERBS_INJECT(ep, len),
	};

	return vrb_send_buf(ep, &wr, buf, len, desc);
}

static ssize_t
vrb_msg_ep_rma_inject_write(struct fid_ep *ep_fid, const void *buf, size_t len,
		     fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_NO_COMP_FLAG,
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = IBV_SEND_INLINE,
	};

	return vrb_send_buf_inline(ep, &wr, buf, len);
}

static ssize_t
vrb_rma_write_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct vrb_ep *ep;

	ep = container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);

	ep->wrs->rma_wr.wr.rdma.remote_addr = addr;
	ep->wrs->rma_wr.wr.rdma.rkey = (uint32_t) key;

	ep->wrs->sge.addr = (uintptr_t) buf;
	ep->wrs->sge.length = (uint32_t) len;

	return vrb_post_send(ep, &ep->wrs->rma_wr);
}

static ssize_t
vrb_msg_ep_rma_inject_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
			uint64_t data, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_NO_COMP_FLAG,
		.opcode = IBV_WR_RDMA_WRITE_WITH_IMM,
		.imm_data = htonl((uint32_t)data),
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = IBV_SEND_INLINE,
	};

	return vrb_send_buf_inline(ep, &wr, buf, len);
}

static ssize_t
vrb_msg_ep_rma_inject_writedata_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
					uint64_t data, fi_addr_t dest_addr, uint64_t addr,
					uint64_t key)
{
	ssize_t ret;
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	ep->wrs->rma_wr.wr.rdma.remote_addr = addr;
	ep->wrs->rma_wr.wr.rdma.rkey = (uint32_t) key;

	ep->wrs->rma_wr.imm_data = htonl((uint32_t) data);
	ep->wrs->rma_wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;

	ep->wrs->sge.addr = (uintptr_t) buf;
	ep->wrs->sge.length = (uint32_t) len;

	ret = vrb_post_send(ep, &ep->wrs->rma_wr);
	ep->wrs->rma_wr.opcode = IBV_WR_RDMA_WRITE;
	return ret;
}

struct fi_ops_rma vrb_msg_ep_rma_ops_ts = {
	.size = sizeof(struct fi_ops_rma),
	.read = vrb_msg_ep_rma_read,
	.readv = vrb_msg_ep_rma_readv,
	.readmsg = vrb_msg_ep_rma_readmsg,
	.write = vrb_msg_ep_rma_write,
	.writev = vrb_msg_ep_rma_writev,
	.writemsg = vrb_msg_ep_rma_writemsg,
	.inject = vrb_msg_ep_rma_inject_write,
	.writedata = vrb_msg_ep_rma_writedata,
	.injectdata = vrb_msg_ep_rma_inject_writedata,
};

struct fi_ops_rma vrb_msg_ep_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = vrb_msg_ep_rma_read,
	.readv = vrb_msg_ep_rma_readv,
	.readmsg = vrb_msg_ep_rma_readmsg,
	.write = vrb_msg_ep_rma_write,
	.writev = vrb_msg_ep_rma_writev,
	.writemsg = vrb_msg_ep_rma_writemsg,
	.inject = vrb_rma_write_fast,
	.writedata = vrb_msg_ep_rma_writedata,
	.injectdata = vrb_msg_ep_rma_inject_writedata_fast,
};

static ssize_t
vrb_msg_xrc_ep_rma_write(struct fid_ep *ep_fid, const void *buf,
		size_t len, void *desc, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(&ep->base_ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = VERBS_INJECT(&ep->base_ep, len),
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	return vrb_send_buf(&ep->base_ep, &wr, buf, len, desc);
}

static ssize_t
vrb_msg_xrc_ep_rma_writev(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = (uintptr_t)context,
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	return vrb_send_iov(&ep->base_ep, &wr, iov, desc, count);
}

static ssize_t
vrb_msg_xrc_ep_rma_writemsg(struct fid_ep *ep_fid,
			const struct fi_msg_rma *msg, uint64_t flags)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = (uintptr_t)msg->context,
		.wr.rdma.remote_addr = msg->rma_iov->addr,
		.wr.rdma.rkey = (uint32_t)msg->rma_iov->key,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	if (flags & FI_REMOTE_CQ_DATA) {
		wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
		wr.imm_data = htonl((uint32_t)msg->data);
	} else {
		wr.opcode = IBV_WR_RDMA_WRITE;
	}

	return vrb_send_msg(&ep->base_ep, &wr, msg, flags);
}

static ssize_t
vrb_msg_xrc_ep_rma_read(struct fid_ep *ep_fid, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, uint64_t addr,
		uint64_t key, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_READ(&ep->base_ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_READ,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	return vrb_send_buf(&ep->base_ep, &wr, buf, len, desc);
}

static ssize_t
vrb_msg_xrc_ep_rma_readv(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t src_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_READ(&ep->base_ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_READ,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.num_sge = count,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	vrb_set_sge_iov(wr.sg_list, iov, count, desc);

	return vrb_post_send(&ep->base_ep, &wr);
}

static ssize_t
vrb_msg_xrc_ep_rma_readmsg(struct fid_ep *ep_fid,
		const struct fi_msg_rma *msg, uint64_t flags)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_READ_FLAGS(&ep->base_ep, flags,
					       (uintptr_t)msg->context),
		.opcode = IBV_WR_RDMA_READ,
		.wr.rdma.remote_addr = msg->rma_iov->addr,
		.wr.rdma.rkey = (uint32_t)msg->rma_iov->key,
		.num_sge = msg->iov_count,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	vrb_set_sge_iov(wr.sg_list, msg->msg_iov, msg->iov_count, msg->desc);

	return vrb_post_send(&ep->base_ep, &wr);
}

static ssize_t
vrb_msg_xrc_ep_rma_writedata(struct fid_ep *ep_fid, const void *buf,
		size_t len, void *desc, uint64_t data, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(&ep->base_ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_WRITE_WITH_IMM,
		.imm_data = htonl((uint32_t)data),
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = VERBS_INJECT(&ep->base_ep, len),
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	return vrb_send_buf(&ep->base_ep, &wr, buf, len, desc);
}

static ssize_t
vrb_msg_xrc_ep_rma_inject_write(struct fid_ep *ep_fid, const void *buf,
		size_t len, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_NO_COMP_FLAG,
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = IBV_SEND_INLINE,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	return vrb_send_buf_inline(&ep->base_ep, &wr, buf, len);
}

static ssize_t
vrb_xrc_rma_write_fast(struct fid_ep *ep_fid, const void *buf,
	  size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);

	ep->base_ep.wrs->rma_wr.wr.rdma.remote_addr = addr;
	ep->base_ep.wrs->rma_wr.wr.rdma.rkey = (uint32_t) key;
	VRB_SET_REMOTE_SRQN(ep->base_ep.wrs->rma_wr, ep->peer_srqn);
	ep->base_ep.wrs->sge.addr = (uintptr_t) buf;
	ep->base_ep.wrs->sge.length = (uint32_t) len;

	return vrb_post_send(&ep->base_ep, &ep->base_ep.wrs->rma_wr);
}

static ssize_t
vrb_msg_xrc_ep_rma_inject_writedata(struct fid_ep *ep_fid,
		const void *buf, size_t len, uint64_t data,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);

	struct ibv_send_wr wr = {
		.wr_id = VERBS_NO_COMP_FLAG,
		.opcode = IBV_WR_RDMA_WRITE_WITH_IMM,
		.imm_data = htonl((uint32_t)data),
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)key,
		.send_flags = IBV_SEND_INLINE,
	};

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	return vrb_send_buf_inline(&ep->base_ep, &wr, buf, len);
}

static ssize_t
vrb_msg_xrc_ep_rma_inject_writedata_fast(struct fid_ep *ep_fid,
		const void *buf, size_t len, uint64_t data,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	ssize_t ret;
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	ep->base_ep.wrs->rma_wr.wr.rdma.remote_addr = addr;
	ep->base_ep.wrs->rma_wr.wr.rdma.rkey = (uint32_t) key;
	VRB_SET_REMOTE_SRQN(ep->base_ep.wrs->rma_wr, ep->peer_srqn);

	ep->base_ep.wrs->rma_wr.imm_data = htonl((uint32_t) data);
	ep->base_ep.wrs->rma_wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;

	ep->base_ep.wrs->sge.addr = (uintptr_t) buf;
	ep->base_ep.wrs->sge.length = (uint32_t) len;

	ret = vrb_post_send(&ep->base_ep, &ep->base_ep.wrs->rma_wr);
	ep->base_ep.wrs->rma_wr.opcode = IBV_WR_RDMA_WRITE;
	return ret;
}

struct fi_ops_rma vrb_msg_xrc_ep_rma_ops_ts = {
	.size = sizeof(struct fi_ops_rma),
	.read = vrb_msg_xrc_ep_rma_read,
	.readv = vrb_msg_xrc_ep_rma_readv,
	.readmsg = vrb_msg_xrc_ep_rma_readmsg,
	.write = vrb_msg_xrc_ep_rma_write,
	.writev = vrb_msg_xrc_ep_rma_writev,
	.writemsg = vrb_msg_xrc_ep_rma_writemsg,
	.inject = vrb_msg_xrc_ep_rma_inject_write,
	.writedata = vrb_msg_xrc_ep_rma_writedata,
	.injectdata = vrb_msg_xrc_ep_rma_inject_writedata,
};

struct fi_ops_rma vrb_msg_xrc_ep_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = vrb_msg_xrc_ep_rma_read,
	.readv = vrb_msg_xrc_ep_rma_readv,
	.readmsg = vrb_msg_xrc_ep_rma_readmsg,
	.write = vrb_msg_xrc_ep_rma_write,
	.writev = vrb_msg_xrc_ep_rma_writev,
	.writemsg = vrb_msg_xrc_ep_rma_writemsg,
	.inject = vrb_xrc_rma_write_fast,
	.writedata = vrb_msg_xrc_ep_rma_writedata,
	.injectdata = vrb_msg_xrc_ep_rma_inject_writedata_fast,
};
