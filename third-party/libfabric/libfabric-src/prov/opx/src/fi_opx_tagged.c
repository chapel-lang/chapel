/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_tagged.h"

#include <ofi_enosys.h>

/*
 * \note The opx provider asserts the following mode bits which affect
 * 	the behavior of this routine:
 *
 * 	- 'FI_ASYNC_IOV' mode bit which requires the application to maintain
 * 	  the 'msg->msg_iov' iovec array until the operation completes
 *
 * 	- 'FI_LOCAL_MR' mode bit which allows the provider to ignore the 'desc'
 * 	  parameter .. no memory regions are required to access the local
 * 	  memory
 */
ssize_t fi_opx_trecvmsg_generic (struct fid_ep *ep,
		const struct fi_msg_tagged *msg,
		uint64_t flags,
		const int lock_required,
		const enum fi_av_type av_type,
		const enum ofi_reliability_kind reliability,
		const enum fi_progress progress)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	union fi_opx_context * opx_context = NULL;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"===================================== POST TRECVMSG\n");

	assert(!lock_required);
	assert(!(flags & FI_MULTI_RECV));	/* Multi-receive incompatible with tagged receives */
	assert(msg->context);
	assert(((uintptr_t)msg->context & 0x07ull) == 0);	/* must be 8 byte aligned */

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.recv.posted_recv_tag);

	if (msg->iov_count == 0) {
		opx_context = (union fi_opx_context *) msg->context;
		opx_context->next = NULL;
		opx_context->src_addr = msg->addr;
		opx_context->flags = flags;
		opx_context->len = 0;
		opx_context->buf = NULL;
		opx_context->byte_counter = (uint64_t)-1;
		if ((flags & (FI_PEEK | FI_CLAIM)) != FI_CLAIM) {
			/* do not overwrite state from a previous "peek|claim" operation */
			opx_context->tag = msg->tag;
			opx_context->ignore = msg->ignore;
		}

		return fi_opx_ep_rx_process_context(opx_ep, FI_TAGGED,
						    OPX_CANCEL_CONTEXT_FALSE,
						    opx_context, flags,
						    OPX_CONTEXT_EXTENDED_FALSE,
						    OPX_HMEM_FALSE,
						    lock_required, av_type,
						    reliability);
	}

#ifdef OPX_HMEM
	/* NOTE: Assume that all IOVs reside in the same HMEM space */
	uint64_t hmem_device;
	enum fi_hmem_iface hmem_iface = fi_opx_hmem_get_iface(msg->msg_iov[0].iov_base,
							      msg->desc ? msg->desc[0] : NULL,
							      &hmem_device);
#ifndef NDEBUG
	if (msg->iov_count > 1) {
		for (int i = 1; i < msg->iov_count; ++i) {
			uint64_t tmp_hmem_device;
			enum fi_hmem_iface tmp_hmem_iface =
				fi_opx_hmem_get_iface(msg->msg_iov[i].iov_base,
						      msg->desc ? msg->desc[i] : NULL,
						      &tmp_hmem_device);
			assert(tmp_hmem_iface == hmem_iface);
			assert(tmp_hmem_device == hmem_device);
		}
	}
#endif
	if (hmem_iface != FI_HMEM_SYSTEM) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hmem.posted_recv_tag);
		struct fi_opx_context_ext * ext = (struct fi_opx_context_ext *) ofi_buf_alloc(opx_ep->rx->ctx_ext_pool);
		if (OFI_UNLIKELY(ext == NULL)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Out of memory.\n");
			return -FI_ENOMEM;
		}
		flags |= FI_OPX_CQ_CONTEXT_EXT | FI_OPX_CQ_CONTEXT_HMEM;

		ext->err_entry.err = 0;
		ext->opx_context.next = NULL;
		ext->opx_context.src_addr = msg->addr;
		ext->opx_context.flags = flags;
		ext->opx_context.byte_counter = (uint64_t)-1;
		ext->msg.op_context = msg->context;
		ext->msg.iov_count = msg->iov_count;
		ext->msg.iov = (struct iovec *)msg->msg_iov;

		if (msg->iov_count == 1) {
			ext->opx_context.len = msg->msg_iov[0].iov_len;
			ext->opx_context.buf = msg->msg_iov[0].iov_base;
			if ((flags & (FI_PEEK | FI_CLAIM)) != FI_CLAIM) {
				/* do not overwrite state from a previous "peek|claim" operation */
				ext->opx_context.tag = msg->tag;
				ext->opx_context.ignore = msg->ignore;
			}
		} else {
			assert((flags & (FI_PEEK | FI_CLAIM)) != FI_CLAIM);	/* TODO - why not? */
			ext->opx_context.tag = msg->tag;
			ext->opx_context.ignore = msg->ignore;
		}

		struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) ext->hmem_info_qws;
		hmem_info->iface = hmem_iface;
		hmem_info->device = hmem_device;

		return fi_opx_ep_rx_process_context(opx_ep, FI_TAGGED,
						    OPX_CANCEL_CONTEXT_FALSE,
						    (union fi_opx_context *) ext, flags,
						    OPX_CONTEXT_EXTENDED_TRUE,
						    OPX_HMEM_TRUE,
						    lock_required, av_type,
						    reliability);
	}
#endif
	if (msg->iov_count == 1) {
		opx_context = (union fi_opx_context *) msg->context;
		opx_context->next = NULL;
		opx_context->src_addr = msg->addr;
		opx_context->flags = flags;
		opx_context->len = msg->msg_iov[0].iov_len;
		opx_context->buf = msg->msg_iov[0].iov_base;
		opx_context->byte_counter = (uint64_t)-1;
		if ((flags & (FI_PEEK | FI_CLAIM)) != FI_CLAIM) {
			/* do not overwrite state from a previous "peek|claim" operation */
			opx_context->tag = msg->tag;
			opx_context->ignore = msg->ignore;
		}

		return fi_opx_ep_rx_process_context(opx_ep, FI_TAGGED,
						    OPX_CANCEL_CONTEXT_FALSE,
						    opx_context, flags,
						    OPX_CONTEXT_EXTENDED_FALSE,
						    OPX_HMEM_FALSE,
						    lock_required, av_type,
						    reliability);
	}

	assert((flags & (FI_PEEK | FI_CLAIM)) != FI_CLAIM);	/* TODO - why not? */

	struct fi_opx_context_ext * ext = (struct fi_opx_context_ext *) ofi_buf_alloc(opx_ep->rx->ctx_ext_pool);
	if (OFI_UNLIKELY(ext == NULL)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"Out of memory.\n");
		return -FI_ENOMEM;
	}
	flags |= FI_OPX_CQ_CONTEXT_EXT;

	ext->err_entry.err = 0;
	ext->opx_context.next = NULL;
	ext->opx_context.src_addr = msg->addr;
	ext->opx_context.flags = flags;
	ext->opx_context.byte_counter = (uint64_t)-1;
	ext->opx_context.tag = msg->tag;
	ext->opx_context.ignore = msg->ignore;
	ext->msg.op_context = msg->context;
	ext->msg.iov_count = msg->iov_count;
	ext->msg.iov = (struct iovec *)msg->msg_iov;

	return fi_opx_ep_rx_process_context(opx_ep, FI_TAGGED,
					    OPX_CANCEL_CONTEXT_FALSE,
					    (union fi_opx_context *) ext, flags,
					    OPX_CONTEXT_EXTENDED_TRUE,
					    OPX_HMEM_FALSE,
					    lock_required, av_type,
					    reliability);

}

ssize_t fi_opx_trecvmsg(struct fid_ep *ep,
		const struct fi_msg_tagged *msg, uint64_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const enum fi_threading threading = opx_ep->domain->threading;
	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);
	const enum fi_av_type av_type = opx_ep->av_type;

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_trecvmsg_generic(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, av_type,
						opx_ep->reliability->state.kind,
						opx_ep->domain->data_progress);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	return rc;
}

ssize_t fi_opx_tsendmsg(struct fid_ep *ep,
		const struct fi_msg_tagged *msg, uint64_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const enum fi_threading threading = opx_ep->threading;
	const enum fi_av_type av_type = opx_ep->av_type;

	const size_t niov = msg->iov_count;

	const uint64_t caps = opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc;

	if (niov == 0) {
		if (!msg->context) {
			rc = fi_opx_ep_tx_inject_internal(ep, 0, 0,
				msg->addr, msg->tag, msg->data,
				FI_OPX_LOCK_NOT_REQUIRED,
				av_type,
				caps | FI_TAGGED,
				opx_ep->reliability->state.kind);
		} else {
			rc = fi_opx_ep_tx_send_internal(ep, 0, 0,
				msg->desc, msg->addr, msg->tag, msg->context, msg->data,
				FI_OPX_LOCK_NOT_REQUIRED,
				av_type,
				OPX_CONTIG_TRUE,
				OPX_FLAGS_OVERRIDE_TRUE,
				flags,
				caps | FI_TAGGED,
				opx_ep->reliability->state.kind);
		}
	} else if (niov == 1) {
		rc = fi_opx_ep_tx_send_internal(ep, msg->msg_iov->iov_base,
			msg->msg_iov->iov_len, msg->desc, msg->addr,
			msg->tag, msg->context, msg->data,
			FI_OPX_LOCK_NOT_REQUIRED,
			av_type,
			OPX_CONTIG_TRUE,
			OPX_FLAGS_OVERRIDE_TRUE,
			flags,
			caps | FI_TAGGED,
			opx_ep->reliability->state.kind);
	} else {
		rc = fi_opx_ep_tx_send_internal(ep, msg->msg_iov, msg->iov_count,
			msg->desc, msg->addr, msg->tag, msg->context, msg->data,
			FI_OPX_LOCK_NOT_REQUIRED,
			av_type,
			OPX_CONTIG_FALSE,
			OPX_FLAGS_OVERRIDE_TRUE,
			flags,
			caps | FI_TAGGED,
			opx_ep->reliability->state.kind);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	return rc;
}



FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)

FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)

FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)


FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)

FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)

FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)


FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)

FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)

FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_TAGGED_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)






#define FI_OPX_TAGGED_OPS_STRUCT_NAME(LOCK,AV,CAPS,RELIABILITY)						\
        FI_OPX_TAGGED_OPS_STRUCT_NAME_(LOCK,AV,CAPS,RELIABILITY)

#define FI_OPX_TAGGED_OPS_STRUCT_NAME_(LOCK,AV,CAPS,RELIABILITY)					\
	fi_opx_ops_tagged_ ## LOCK ## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY

#define FI_OPX_TAGGED_OPS_STRUCT(LOCK,AV,CAPS,RELIABILITY)							\
static struct fi_ops_tagged										\
	FI_OPX_TAGGED_OPS_STRUCT_NAME(LOCK,AV,CAPS,RELIABILITY) __attribute__ ((unused)) = {		\
	.size		= sizeof(struct fi_ops_tagged),							\
	.recv		= FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(trecv, LOCK, AV, CAPS, RELIABILITY),		\
	.recvv		= fi_no_tagged_recvv,								\
	.recvmsg	= fi_opx_trecvmsg,								\
	.send		= FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tsend, LOCK, AV, CAPS, RELIABILITY),		\
	.sendv		= fi_no_tagged_sendv,								\
	.sendmsg	= fi_opx_tsendmsg,								\
	.inject 	= FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tinject, LOCK, AV, CAPS, RELIABILITY),	\
	.senddata	= FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tsenddata, LOCK, AV, CAPS, RELIABILITY),	\
	.injectdata	= FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tinjectdata, LOCK, AV, CAPS, RELIABILITY),	\
}

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);


FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);


FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_TAGGED_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);






ssize_t fi_opx_tsearch(struct fid_ep *ep, uint64_t *tag,
		uint64_t ignore, uint64_t flags,
		fi_addr_t *src_addr, size_t *len, void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

static struct fi_ops_tagged fi_opx_no_tagged_ops = {
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

int fi_opx_init_tagged_ops(struct fid_ep *ep, struct fi_info *info)
{
        if (!info || !ep)
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

int fi_opx_enable_tagged_ops(struct fid_ep *ep)
{
	struct fi_opx_ep * opx_ep =
		container_of(ep, struct fi_opx_ep, ep_fid);

        if (!opx_ep || !opx_ep->domain)
                goto err;

	if (!(opx_ep->tx->caps & FI_TAGGED) || !(opx_ep->rx->caps & FI_TAGGED)) {
		/* Tagged ops not enabled on this endpoint */
		opx_ep->ep_fid.tagged = &fi_opx_no_tagged_ops;
		return 0;
	}

	if ((opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) !=
		(opx_ep->rx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM))) {
		/* rx/tx cpability mismatch */
		opx_ep->ep_fid.tagged = &fi_opx_no_tagged_ops;
		return 0;
	}

	const enum fi_threading threading = opx_ep->domain->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
                opx_ep->ep_fid.tagged = &fi_opx_no_tagged_ops;
                FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
                                "Unknown thread mode, tagged ops not enabled on EP\n");
		return 0;
        }

	uint64_t comm_caps = opx_ep->rx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	if (comm_caps == 0)
		comm_caps = FI_LOCAL_COMM | FI_REMOTE_COMM;

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	if (!lock_required) {
		if (opx_ep->av->type == FI_AV_TABLE) {

			if (comm_caps == 0x0008000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE,0x0008000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE,0x0008000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == 0x0010000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE,0x0010000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE,0x0010000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* 0x0018000000000000ull */

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE,0x0018000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE,0x0018000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);
			}

		} else if (opx_ep->av->type == FI_AV_MAP) {

			if (comm_caps == 0x0008000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP,0x0008000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP,0x0008000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == 0x0010000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP,0x0010000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP,0x0010000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* 0x0018000000000000ull */

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP,0x0018000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP,0x0018000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			}
		}  else {
			/* FI_AV_UNSPEC is not a runtime value in the address vector so FI_OPX_TAGGED_OPS_STRUCT_NAME is not used here.  It is used in FABRIC_DIRECT */
			assert((opx_ep->av->type==FI_AV_TABLE)||(opx_ep->av->type==FI_AV_MAP));
		}
	} else {
		if (opx_ep->av->type == FI_AV_TABLE) {

			if (comm_caps == 0x0008000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE,0x0008000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE,0x0008000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == 0x0010000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE,0x0010000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE,0x0010000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* 0x0018000000000000ull */

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE,0x0018000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE,0x0018000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			}
		} else  if (opx_ep->av->type == FI_AV_MAP) {

			if (comm_caps == 0x0008000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP,0x0008000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP,0x0008000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == 0x0010000000000000ull) {

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP,0x0010000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP,0x0010000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* 0x0018000000000000ull */

				if (opx_ep->reliability->state.kind == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP,0x0018000000000000ull,OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.tagged = &FI_OPX_TAGGED_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP,0x0018000000000000ull,OFI_RELIABILITY_KIND_OFFLOAD);

			}
		} else {
			/* FI_AV_UNSPEC is not a runtime value in the address vector so FI_OPX_TAGGED_OPS_STRUCT_NAME is not used here.  It is used in FABRIC_DIRECT */
			assert((opx_ep->av->type==FI_AV_TABLE)||(opx_ep->av->type==FI_AV_MAP));
		}
	}

	return 0;
err:
        errno = FI_EINVAL;
        return -errno;
}

int fi_opx_finalize_tagged_ops(struct fid_ep *ep)
{
	if (!ep) {
		return 0;
	}

	return 0;
}


ssize_t
fi_opx_tinject_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t tag)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	return FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tinject,
			OPX_LOCK,
			OPX_AV,
			OPX_TAGGED_CAPS,
			OPX_RELIABILITY)
				(ep, buf, len, dest_addr, tag);
}

ssize_t
fi_opx_tsend_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	return FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tsend,
			OPX_LOCK,
			OPX_AV,
			OPX_TAGGED_CAPS,
			OPX_RELIABILITY)
				(ep, buf, len, desc, dest_addr, tag, context);
}

ssize_t
fi_opx_tinjectdata_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	return FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tinjectdata,
			OPX_LOCK,
			OPX_AV,
			OPX_TAGGED_CAPS,
			OPX_RELIABILITY)
				(ep, buf, len, data, dest_addr, tag);
}

ssize_t
fi_opx_tsenddata_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, uint64_t data, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	return FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(tsenddata,
			OPX_LOCK,
			OPX_AV,
			OPX_TAGGED_CAPS,
			OPX_RELIABILITY)
				(ep, buf, len, desc, data, dest_addr, tag, context);
}

ssize_t
fi_opx_trecv_FABRIC_DIRECT(struct fid_ep *ep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, uint64_t tag, uint64_t ignore, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	return FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(trecv,
			OPX_LOCK,
			OPX_AV,
			OPX_TAGGED_CAPS,
			OPX_RELIABILITY)
				(ep, buf, len, desc, src_addr, tag, ignore, context);
}
