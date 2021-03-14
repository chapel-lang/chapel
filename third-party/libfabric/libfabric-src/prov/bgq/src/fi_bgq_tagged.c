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

ssize_t fi_bgq_trecvmsg(struct fid_ep *ep,
		const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct fi_bgq_ep * bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);
	const enum fi_threading threading = bgq_ep->domain->threading;
	const int lock_required =
		(threading == FI_THREAD_FID) ||
		(threading == FI_THREAD_UNSPEC) ||
		(threading == FI_THREAD_SAFE);

	return fi_bgq_trecvmsg_generic(ep, msg, flags, lock_required);
}

ssize_t fi_bgq_tsendmsg(struct fid_ep *ep,
		const struct fi_msg_tagged *msg, uint64_t flags)
{
	const size_t niov = msg->iov_count;

	if (niov > 32) {

		/* ---------------------------------------------------------
		 * a single torus packet payload can only transfer 32
		 * 'struct fi_bgq_mu_iov' elements - this is the current
		 * limit for non-contiguous rendezvous operations
		 *
		 * TODO - support >32 iov elements?
		 * --------------------------------------------------------- */
		return -FI_EINVAL;

	} else {

		struct fi_bgq_ep * bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);
		const enum fi_threading threading = bgq_ep->threading;

		return fi_bgq_send_generic_flags(ep, msg->msg_iov, niov,
			msg->desc, msg->addr, msg->tag, msg->context, msg->data,
			(threading != FI_THREAD_ENDPOINT && threading != FI_THREAD_DOMAIN),
			0 /* is_msg */,
			0 /* is_contiguous */,
			1 /* override flags */,
			flags);
	}
}

ssize_t fi_bgq_tsenddata(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, uint64_t data, fi_addr_t dest_addr, uint64_t tag,
		void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

/* "FI_BGQ_TAGGED_SPECIALIZED_FUNC(0)" is already declared via FABRIC_DIRECT */
FI_BGQ_TAGGED_SPECIALIZED_FUNC(1)

#define FI_BGQ_TAGGED_OPS_STRUCT_NAME(LOCK)				\
	fi_bgq_ops_tagged_ ## LOCK

#define FI_BGQ_TAGGED_OPS_STRUCT(LOCK)					\
static struct fi_ops_tagged						\
	FI_BGQ_TAGGED_OPS_STRUCT_NAME(LOCK) = {				\
	.size		= sizeof(struct fi_ops_tagged),			\
	.recv		=						\
		FI_BGQ_TAGGED_SPECIALIZED_FUNC_NAME(trecv, LOCK),	\
	.recvv		= fi_no_tagged_recvv,				\
	.recvmsg	=						\
		FI_BGQ_TAGGED_SPECIALIZED_FUNC_NAME(trecvmsg, LOCK),	\
	.send		=						\
		FI_BGQ_TAGGED_SPECIALIZED_FUNC_NAME(tsend, LOCK),	\
	.sendv		= fi_no_tagged_sendv,				\
	.sendmsg	= fi_bgq_tsendmsg,				\
	.inject 	=						\
		FI_BGQ_TAGGED_SPECIALIZED_FUNC_NAME(tinject, LOCK),	\
	.senddata	=						\
		FI_BGQ_TAGGED_SPECIALIZED_FUNC_NAME(tsenddata, LOCK),	\
	.injectdata	=						\
		FI_BGQ_TAGGED_SPECIALIZED_FUNC_NAME(tinjectdata, LOCK),	\
}

FI_BGQ_TAGGED_OPS_STRUCT(0);
FI_BGQ_TAGGED_OPS_STRUCT(1);

ssize_t fi_bgq_tsearch(struct fid_ep *ep, uint64_t *tag,
		uint64_t ignore, uint64_t flags,
		fi_addr_t *src_addr, size_t *len, void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

static struct fi_ops_tagged fi_bgq_no_tagged_ops = {
        .size           = sizeof(struct fi_ops_tagged),
        .recv           = fi_no_tagged_recv,
        .recvv          = fi_no_tagged_recvv,
        .recvmsg        = fi_no_tagged_recvmsg,
        .send           = fi_no_tagged_send,
        .sendv          = fi_no_tagged_sendv,
        .sendmsg        = fi_no_tagged_sendmsg,
        .inject         = fi_no_tagged_inject,
        .senddata       = fi_no_tagged_senddata,
        .injectdata     = fi_no_tagged_injectdata
};

int fi_bgq_init_tagged_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info)
{
        if (!info || !bgq_ep)
                goto err;

        if (info->caps & FI_TAGGED ||
                        (info->tx_attr &&
                         (info->tx_attr->caps & FI_TAGGED))) {
        }

        return 0;

err:
        errno = FI_EINVAL;
        return -errno;
}

int fi_bgq_enable_tagged_ops(struct fi_bgq_ep *bgq_ep)
{
        if (!bgq_ep || !bgq_ep->domain)
                goto err;

        if (!(bgq_ep->tx.caps & FI_TAGGED)) {
                /* Tagged ops not enabled on this endpoint */
                bgq_ep->ep_fid.tagged =
                        &fi_bgq_no_tagged_ops;
                return 0;
        }

        switch (bgq_ep->domain->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
		bgq_ep->ep_fid.tagged = &FI_BGQ_TAGGED_OPS_STRUCT_NAME(0);
                break;
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
		bgq_ep->ep_fid.tagged = &FI_BGQ_TAGGED_OPS_STRUCT_NAME(1);
                break;
        default:
                bgq_ep->ep_fid.tagged = &fi_bgq_no_tagged_ops;
                FI_WARN(fi_bgq_global.prov, FI_LOG_EP_DATA,
                                "Tagged ops not enabled on EP\n");
		break;
        }

	return 0;
err:
        errno = FI_EINVAL;
        return -errno;
}

int fi_bgq_finalize_tagged_ops(struct fi_bgq_ep *bgq_ep)
{
	if (!bgq_ep) {
		return 0;
	}

	return 0;
}
