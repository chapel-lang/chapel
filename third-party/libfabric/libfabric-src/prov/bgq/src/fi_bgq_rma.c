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
#include <errno.h>

/* "FI_BGQ_RMA_SPECIALIZED_FUNC(0)" is already declared via FABRIC_DIRECT */
FI_BGQ_RMA_SPECIALIZED_FUNC(1)

#define FI_BGQ_RMA_OPS_STRUCT_NAME(LOCK)			\
	fi_bgq_ops_rma_ ## LOCK

#define FI_BGQ_RMA_OPS_STRUCT(LOCK)				\
static struct fi_ops_rma					\
	FI_BGQ_RMA_OPS_STRUCT_NAME(LOCK) = {			\
	.size	= sizeof(struct fi_ops_rma),			\
	.read	= FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(read, LOCK),	\
	.readv	= fi_no_rma_readv,				\
	.readmsg = FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(readmsg,	\
			LOCK),					\
	.write	= FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(write,	\
			LOCK),					\
	.inject = FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(inject_write,\
			LOCK),					\
	.writev = FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(writev,	\
			LOCK),					\
	.writemsg = FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(writemsg,	\
			LOCK),					\
	.writedata = fi_no_rma_writedata,			\
}

FI_BGQ_RMA_OPS_STRUCT(0);
FI_BGQ_RMA_OPS_STRUCT(1);

static inline ssize_t fi_bgq_rma_read(struct fid_ep *ep,
		void *buf, size_t len, void *desc,
		fi_addr_t src_addr, uint64_t addr,
		uint64_t key, void *context)
{
	int lock_required;
	struct fi_bgq_ep *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	switch (bgq_ep->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	return fi_bgq_read_generic(ep, buf, len, desc, src_addr,
			addr, key, context, lock_required);
}

static inline ssize_t fi_bgq_rma_readmsg(struct fid_ep *ep,
		const struct fi_msg_rma *msg, uint64_t flags)
{
	int lock_required;
	struct fi_bgq_ep *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	switch (bgq_ep->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	return fi_bgq_readmsg_generic(ep, msg, flags,
			lock_required);
}

static inline ssize_t fi_bgq_rma_inject_write(struct fid_ep *ep,
		const void *buf, size_t len,
		fi_addr_t dst_addr, uint64_t addr, uint64_t key)
{
	int lock_required;
	struct fi_bgq_ep *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	switch (bgq_ep->threading) {
		case FI_THREAD_ENDPOINT:
		case FI_THREAD_DOMAIN:
			lock_required = 0;
		default:
			lock_required = 1;
	}

	return fi_bgq_inject_write_generic(ep, buf, len, dst_addr,
			addr, key, lock_required);
}

static inline ssize_t fi_bgq_rma_write(struct fid_ep *ep,
		const void *buf, size_t len, void *desc,
		fi_addr_t dst_addr, uint64_t addr,
		uint64_t key, void *context)
{
	int lock_required;
	struct fi_bgq_ep *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	switch (bgq_ep->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	return fi_bgq_write_generic(ep, buf, len, desc, dst_addr,
			addr, key, context, lock_required);
}

static inline ssize_t fi_bgq_rma_writev(struct fid_ep *ep,
		const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, void *context)
{
	int lock_required;
	struct fi_bgq_ep *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	switch (bgq_ep->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	return fi_bgq_writev_generic(ep, iov, desc, count, dest_addr, addr,
			key, context, lock_required);
}

static inline ssize_t fi_bgq_rma_writemsg(struct fid_ep *ep,
		const struct fi_msg_rma *msg, uint64_t flags)
{
	int lock_required;
	struct fi_bgq_ep *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	switch (bgq_ep->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	return fi_bgq_writemsg_generic(ep, msg, flags,
			lock_required);
}

static struct fi_ops_rma fi_bgq_ops_rma_default = {
	.size		= sizeof(struct fi_ops_rma),
	.read		= fi_bgq_rma_read,
	.readv		= fi_no_rma_readv,
	.readmsg	= fi_bgq_rma_readmsg,
	.write		= fi_bgq_rma_write,
	.inject		= fi_bgq_rma_inject_write,
	.writev		= fi_bgq_rma_writev,
	.writemsg	= fi_bgq_rma_writemsg,
	.writedata	= fi_no_rma_writedata,
};

int fi_bgq_init_rma_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info)
{
	if (!bgq_ep || !info) {
		errno = FI_EINVAL;
		goto err;
	}

	return 0;
err:
	return -errno;
}

int fi_bgq_enable_rma_ops(struct fi_bgq_ep *bgq_ep)
{
	if (!bgq_ep || !bgq_ep->domain) {
		errno = FI_EINVAL;
		goto err;
	}

	if (!(bgq_ep->tx.caps & FI_RMA)) {
		/* rma ops not enabled on this endpoint */
		return 0;
	}

	switch (bgq_ep->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
	case FI_THREAD_COMPLETION:
		bgq_ep->ep_fid.rma = &FI_BGQ_RMA_OPS_STRUCT_NAME(0);
		break;
	case FI_THREAD_FID:
	case FI_THREAD_UNSPEC:
	case FI_THREAD_SAFE:
		bgq_ep->ep_fid.rma = &FI_BGQ_RMA_OPS_STRUCT_NAME(1);
		break;
	default:
		/*bgq_ep->ep_fid.rma = &fi_bgq_ops_rma_default;*/
		errno = FI_EINVAL;
		goto err;
	}


	return 0;
err:
	return -errno;
}

int fi_bgq_finalize_rma_ops(struct fi_bgq_ep *bgq_ep)
{
	if (!bgq_ep) {
		return 0;
	}

	return 0;
}
