/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>

ssize_t fi_bgq_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			uint64_t flags)
{
	struct fi_bgq_ep * bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);
	const enum fi_threading threading = bgq_ep->threading;

	return fi_bgq_send_generic_flags(ep, msg->msg_iov, msg->iov_count,
		msg->desc, msg->addr, 0, msg->context, msg->data,
		(threading != FI_THREAD_ENDPOINT && threading != FI_THREAD_DOMAIN),	/* "lock required"? */
		1 /* is_msg */,
		0 /* is_contiguous */,
		1 /* override the default tx flags */,
		flags);
}

ssize_t fi_bgq_sendv(struct fid_ep *ep, const struct iovec *iov,
			void **desc, size_t count, fi_addr_t dest_addr,
			void *context)
{
	struct fi_bgq_ep * bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);
	const enum fi_threading threading = bgq_ep->threading;

	return fi_bgq_send_generic_flags(ep, iov, count,
		desc, dest_addr, 0, context, 0,
		(threading != FI_THREAD_ENDPOINT && threading != FI_THREAD_DOMAIN),	/* "lock required"? */
		1 /* is_msg */,
		0 /* is_contiguous */,
		0 /* do not override flags */,
		0);
}


ssize_t fi_bgq_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
			uint64_t data, void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

/* "FI_BGQ_MSG_SPECIALIZED_FUNC(0)" is already declared via FABRIC_DIRECT */
FI_BGQ_MSG_SPECIALIZED_FUNC(1)

#define FI_BGQ_MSG_OPS_STRUCT_NAME(LOCK)				\
	fi_bgq_ops_msg_ ## LOCK

#define FI_BGQ_MSG_OPS_STRUCT(LOCK)					\
static struct fi_ops_msg						\
	FI_BGQ_MSG_OPS_STRUCT_NAME(LOCK) = {				\
	.size		= sizeof(struct fi_ops_msg),			\
	.recv		=						\
		FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(recv, LOCK),		\
	.recvv		= fi_no_msg_recvv,				\
	.recvmsg	=						\
		FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(recvmsg, LOCK),	\
	.send		=						\
		FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(send, LOCK),		\
	.sendv		= fi_bgq_sendv,					\
	.sendmsg	= fi_bgq_sendmsg,				\
	.inject		=						\
		FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(inject, LOCK),		\
	.senddata	=						\
		FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(senddata, LOCK),	\
	.injectdata	=						\
		FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(injectdata, LOCK),	\
}

FI_BGQ_MSG_OPS_STRUCT(0);
FI_BGQ_MSG_OPS_STRUCT(1);

static struct fi_ops_msg fi_bgq_no_msg_ops = {
	.size		= sizeof(struct fi_ops_msg),
	.recv		= fi_no_msg_recv,
	.recvv		= fi_no_msg_recvv,
	.recvmsg	= fi_no_msg_recvmsg,
	.send		= fi_no_msg_send,
	.sendv		= fi_no_msg_sendv,
	.sendmsg	= fi_no_msg_sendmsg,
	.inject		= fi_no_msg_inject,
	.senddata	= fi_no_msg_senddata,
	.injectdata	= fi_no_msg_injectdata
};

int fi_bgq_init_msg_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info)
{
	if (!info || !bgq_ep) {
		errno = FI_EINVAL;
		goto err;
	}
	if (info->caps & FI_MSG ||
			(info->tx_attr &&
			 (info->tx_attr->caps & FI_MSG))) {

		bgq_ep->rx.min_multi_recv = sizeof(union fi_bgq_mu_packet_payload);
		bgq_ep->rx.poll.min_multi_recv = bgq_ep->rx.min_multi_recv;

	}

	return 0;

err:
	return -errno;
}

int fi_bgq_enable_msg_ops(struct fi_bgq_ep *bgq_ep)
{
	if (!bgq_ep || !bgq_ep->domain)
		return -FI_EINVAL;

	if (!(bgq_ep->tx.caps & FI_MSG)) {
		/* Messaging ops not enabled on this endpoint */
		bgq_ep->ep_fid.msg =
			&fi_bgq_no_msg_ops;
		return 0;
	}


	switch (bgq_ep->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
	case FI_THREAD_COMPLETION:
		bgq_ep->ep_fid.msg = &FI_BGQ_MSG_OPS_STRUCT_NAME(0);
		break;
	case FI_THREAD_FID:
	case FI_THREAD_UNSPEC:
	case FI_THREAD_SAFE:
		bgq_ep->ep_fid.msg = &FI_BGQ_MSG_OPS_STRUCT_NAME(1);
		break;
	default:
		return -FI_EINVAL;
	}

	return 0;
}

int fi_bgq_finalize_msg_ops(struct fi_bgq_ep *bgq_ep)
{
	if (!bgq_ep) {
		return 0;
	}

	return 0;
}
