/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
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

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <limits.h>

#include <ofi_iov.h>

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_EP_DATA, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_EP_DATA, __VA_ARGS__)

ssize_t sock_ep_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
			    uint64_t flags)
{
	int ret;
	size_t i;
	struct sock_op tx_op;
	union sock_iov tx_iov;
	struct sock_conn *conn;
	struct sock_tx_ctx *tx_ctx;
	uint64_t total_len, src_len, dst_len, op_flags;
	struct sock_ep *sock_ep;
	struct sock_ep_attr *ep_attr;

	switch (ep->fid.fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(ep, struct sock_ep, ep);
		tx_ctx = sock_ep->attr->tx_ctx->use_shared ?
			sock_ep->attr->tx_ctx->stx_ctx : sock_ep->attr->tx_ctx;
		ep_attr = sock_ep->attr;
		op_flags = sock_ep->tx_attr.op_flags;
		break;

	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(ep, struct sock_tx_ctx, fid.ctx);
		ep_attr = tx_ctx->ep_attr;
		op_flags = tx_ctx->attr.op_flags;
		break;

	default:
		SOCK_LOG_ERROR("Invalid EP type\n");
		return -FI_EINVAL;
	}

#if ENABLE_DEBUG
	if (msg->iov_count > SOCK_EP_MAX_IOV_LIMIT ||
		msg->rma_iov_count > SOCK_EP_MAX_IOV_LIMIT)
		return -FI_EINVAL;
#endif

	if (!tx_ctx->enabled)
		return -FI_EOPBADSTATE;

	ret = sock_ep_get_conn(ep_attr, tx_ctx, msg->addr, &conn);
	if (ret)
		return ret;

	SOCK_EP_SET_TX_OP_FLAGS(flags);
	if (flags & SOCK_USE_OP_FLAGS)
		flags |= op_flags;

	if (flags & FI_TRIGGER) {
		ret = sock_queue_rma_op(ep, msg, flags, FI_OP_READ);
		if (ret != 1)
			return ret;
	}

	total_len = sizeof(struct sock_op_send) +
		(msg->iov_count * sizeof(union sock_iov)) +
		(msg->rma_iov_count * sizeof(union sock_iov));

	sock_tx_ctx_start(tx_ctx);
	if (ofi_rbavail(&tx_ctx->rb) < total_len) {
		ret = -FI_EAGAIN;
		goto err;
	}

	memset(&tx_op, 0, sizeof(struct sock_op));
	tx_op.op = SOCK_OP_READ;
	tx_op.src_iov_len = msg->rma_iov_count;
	tx_op.dest_iov_len = msg->iov_count;

	sock_tx_ctx_write_op_send(tx_ctx, &tx_op, flags,
			(uintptr_t) msg->context, msg->addr,
			(uintptr_t) msg->msg_iov[0].iov_base,
			ep_attr, conn);

	src_len = 0;
	for (i = 0; i < msg->rma_iov_count; i++) {
		tx_iov.iov.addr = msg->rma_iov[i].addr;
		tx_iov.iov.key = msg->rma_iov[i].key;
		tx_iov.iov.len = msg->rma_iov[i].len;
		sock_tx_ctx_write(tx_ctx, &tx_iov, sizeof(tx_iov));
		src_len += tx_iov.iov.len;
	}

	dst_len = 0;
	for (i = 0; i < msg->iov_count; i++) {
		tx_iov.iov.addr = (uintptr_t) msg->msg_iov[i].iov_base;
		tx_iov.iov.len = msg->msg_iov[i].iov_len;
		sock_tx_ctx_write(tx_ctx, &tx_iov, sizeof(tx_iov));
		dst_len += tx_iov.iov.len;
	}

#if ENABLE_DEBUG
	if (dst_len != src_len) {
		SOCK_LOG_ERROR("Buffer length mismatch\n");
		ret = -FI_EINVAL;
		goto err;
	}
#endif

	sock_tx_ctx_commit(tx_ctx);
	return 0;

err:
	sock_tx_ctx_abort(tx_ctx);
	return ret;
}

static ssize_t sock_ep_rma_read(struct fid_ep *ep, void *buf, size_t len,
				 void *desc, fi_addr_t src_addr, uint64_t addr,
				 uint64_t key, void *context)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &msg_iov,
		.desc = &desc,
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = src_addr,
		.context = context,
		.data = 0,
	};

	return sock_ep_rma_readmsg(ep, &msg, SOCK_USE_OP_FLAGS);
}

static ssize_t sock_ep_rma_readv(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count,
				fi_addr_t src_addr, uint64_t addr, uint64_t key,
				void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = ofi_total_iov_len(iov, count),
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = src_addr,
		.context = context,
		.data = 0,
	};

	return sock_ep_rma_readmsg(ep, &msg, SOCK_USE_OP_FLAGS);
}

ssize_t sock_ep_rma_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
			     uint64_t flags)
{
	int ret;
	size_t i;
	struct sock_op tx_op;
	union sock_iov tx_iov;
	struct sock_conn *conn;
	struct sock_tx_ctx *tx_ctx;
	uint64_t total_len, src_len, dst_len, op_flags;
	struct sock_ep *sock_ep;
	struct sock_ep_attr *ep_attr;

	switch (ep->fid.fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(ep, struct sock_ep, ep);
		tx_ctx = sock_ep->attr->tx_ctx->use_shared ?
			sock_ep->attr->tx_ctx->stx_ctx : sock_ep->attr->tx_ctx;
		ep_attr = sock_ep->attr;
		op_flags = sock_ep->tx_attr.op_flags;
		break;

	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(ep, struct sock_tx_ctx, fid.ctx);
		ep_attr = tx_ctx->ep_attr;
		op_flags = tx_ctx->attr.op_flags;
		break;

	default:
		SOCK_LOG_ERROR("Invalid EP type\n");
		return -FI_EINVAL;
	}

#if ENABLE_DEBUG
	if (msg->iov_count > SOCK_EP_MAX_IOV_LIMIT ||
		msg->rma_iov_count > SOCK_EP_MAX_IOV_LIMIT)
		return -FI_EINVAL;
#endif

	if (!tx_ctx->enabled)
		return -FI_EOPBADSTATE;

	ret = sock_ep_get_conn(ep_attr, tx_ctx, msg->addr, &conn);
	if (ret)
		return ret;

	SOCK_EP_SET_TX_OP_FLAGS(flags);
	if (flags & SOCK_USE_OP_FLAGS)
		flags |= op_flags;

	if (flags & FI_TRIGGER) {
		ret = sock_queue_rma_op(ep, msg, flags, FI_OP_WRITE);
		if (ret != 1)
			return ret;
	}

	memset(&tx_op, 0, sizeof(struct sock_op));
	tx_op.op = SOCK_OP_WRITE;
	tx_op.dest_iov_len = msg->rma_iov_count;

	total_len = 0;
	if (flags & FI_INJECT) {
		for (i = 0; i < msg->iov_count; i++)
			total_len += msg->msg_iov[i].iov_len;

		if (total_len > SOCK_EP_MAX_INJECT_SZ)
			return -FI_EINVAL;

		tx_op.src_iov_len = total_len;
	} else {
		total_len += msg->iov_count * sizeof(union sock_iov);
		tx_op.src_iov_len = msg->iov_count;
	}

	total_len += (sizeof(struct sock_op_send) +
		      (msg->rma_iov_count * sizeof(union sock_iov)));

	sock_tx_ctx_start(tx_ctx);
	if (ofi_rbavail(&tx_ctx->rb) < total_len) {
		ret = -FI_EAGAIN;
		goto err;
	}

	sock_tx_ctx_write_op_send(tx_ctx, &tx_op, flags,
			(uintptr_t) msg->context, msg->addr,
			(uintptr_t) msg->msg_iov[0].iov_base, ep_attr, conn);

	if (flags & FI_REMOTE_CQ_DATA)
		sock_tx_ctx_write(tx_ctx, &msg->data, sizeof(msg->data));

	src_len = 0;
	if (flags & FI_INJECT) {
		for (i = 0; i < msg->iov_count; i++) {
			sock_tx_ctx_write(tx_ctx, msg->msg_iov[i].iov_base,
					  msg->msg_iov[i].iov_len);
			src_len += msg->msg_iov[i].iov_len;
		}
	} else {
		for (i = 0; i < msg->iov_count; i++) {
			tx_iov.iov.addr = (uintptr_t) msg->msg_iov[i].iov_base;
			tx_iov.iov.len = msg->msg_iov[i].iov_len;
			sock_tx_ctx_write(tx_ctx, &tx_iov, sizeof(tx_iov));
			src_len += tx_iov.iov.len;
		}
	}

	dst_len = 0;
	for (i = 0; i < msg->rma_iov_count; i++) {
		tx_iov.iov.addr = msg->rma_iov[i].addr;
		tx_iov.iov.key = msg->rma_iov[i].key;
		tx_iov.iov.len = msg->rma_iov[i].len;
		sock_tx_ctx_write(tx_ctx, &tx_iov, sizeof(tx_iov));
		dst_len += tx_iov.iov.len;
	}

#if ENABLE_DEBUG
	if (dst_len != src_len) {
		SOCK_LOG_ERROR("Buffer length mismatch\n");
		ret = -FI_EINVAL;
		goto err;
	}
#endif

	sock_tx_ctx_commit(tx_ctx);
	return 0;

err:
	sock_tx_ctx_abort(tx_ctx);
	return ret;
}

static ssize_t sock_ep_rma_write(struct fid_ep *ep, const void *buf,
				  size_t len, void *desc, fi_addr_t dest_addr,
				  uint64_t addr, uint64_t key, void *context)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &msg_iov,
		.desc = &desc,
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = dest_addr,
		.context = context,
		.data = 0,
	};

	return sock_ep_rma_writemsg(ep, &msg, SOCK_USE_OP_FLAGS);
}

static ssize_t sock_ep_rma_writev(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count, fi_addr_t dest_addr,
				uint64_t addr, uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = ofi_total_iov_len(iov, count),
	};
	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = dest_addr,
		.context = context,
		.data = 0,
	};

	return sock_ep_rma_writemsg(ep, &msg, SOCK_USE_OP_FLAGS);
}

static ssize_t sock_ep_rma_writedata(struct fid_ep *ep, const void *buf,
				      size_t len, void *desc, uint64_t data,
				      fi_addr_t dest_addr, uint64_t addr,
				      uint64_t key, void *context)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.desc = &desc,
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.msg_iov = &msg_iov,
		.addr = dest_addr,
		.context = context,
		.data = data,
	};

	return sock_ep_rma_writemsg(ep, &msg, FI_REMOTE_CQ_DATA |
					SOCK_USE_OP_FLAGS);
}

static ssize_t sock_ep_rma_inject(struct fid_ep *ep, const void *buf,
				size_t len, fi_addr_t dest_addr, uint64_t addr,
				uint64_t key)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.msg_iov = &msg_iov,
		.desc = NULL,
		.addr = dest_addr,
		.context = NULL,
		.data = 0,
	};

	return sock_ep_rma_writemsg(ep, &msg, FI_INJECT |
				    SOCK_NO_COMPLETION | SOCK_USE_OP_FLAGS);
}

static ssize_t sock_ep_rma_injectdata(struct fid_ep *ep, const void *buf,
					size_t len, uint64_t data,
					fi_addr_t dest_addr, uint64_t addr,
					uint64_t key)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.msg_iov = &msg_iov,
		.desc = NULL,
		.addr = dest_addr,
		.context = NULL,
		.data = data,
	};

	return sock_ep_rma_writemsg(ep, &msg, FI_INJECT | FI_REMOTE_CQ_DATA |
		SOCK_NO_COMPLETION | SOCK_USE_OP_FLAGS);
}


struct fi_ops_rma sock_ep_rma = {
	.size  = sizeof(struct fi_ops_rma),
	.read = sock_ep_rma_read,
	.readv = sock_ep_rma_readv,
	.readmsg = sock_ep_rma_readmsg,
	.write = sock_ep_rma_write,
	.writev = sock_ep_rma_writev,
	.writemsg = sock_ep_rma_writemsg,
	.inject = sock_ep_rma_inject,
	.injectdata = sock_ep_rma_injectdata,
	.writedata = sock_ep_rma_writedata,
};

