/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 by Cornelis Networks.
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

#include "rdma/opx/fi_opx_eq.h"

#define FI_OPX_DEFAULT_CQ_DEPTH (8192)
#define FI_OPX_MAXIMUM_CQ_DEPTH (8192)

/*
 * Dump out the contents of a completion queue. Can be called from gdb,
 * just pass NULL for the function name.
 */
void fi_opx_cq_debug(struct fid_cq *cq, char *func, const int line) {

	char str[2048];
	char *s = str;
	size_t len = 2047;
	int n = 0;
	union fi_opx_context * context = NULL;;

	struct fi_opx_cq *opx_cq = (struct fi_opx_cq *)cq;

	if (!func) func = "undef";

	n = snprintf(s, len, "%s():%d [%p] completed(%p,%p)", func, line, opx_cq, opx_cq->completed.head, opx_cq->completed.tail);
	s += n;
	len -= n;

	if (opx_cq->completed.head != NULL) {
		context = opx_cq->completed.head;
		n = snprintf(s, len, " = { %p", context); s += n; len -= n;

		context = context->next;
		while (context != NULL) {
			n = snprintf(s, len, ", %p", context); s += n; len += n;
			context = context->next;
		}
		n = snprintf(s, len, " }"); s += n; len -= n;
	}
	fprintf(stderr, "%s\n", str);

	n = 0; len = 2047; s = str; *s = 0;
	n = snprintf(s, len, "%s():%d [%p] pending(%p,%p)", func, line, opx_cq, opx_cq->pending.head, opx_cq->pending.tail); s += n; len -= n;
	if (opx_cq->pending.head != NULL) {
		context = opx_cq->pending.head;
		n = snprintf(s, len, " = { %p(%lu,0x%016lx)", context, context->byte_counter, context->byte_counter); s += n; len -= n;

		context = context->next;
		while (context != NULL) {
			n = snprintf(s, len, ", %p(%lu,0x%016lx)", context, context->byte_counter, context->byte_counter); s += n; len += n;
			context = context->next;
		}
		n = snprintf(s, len, " }"); s += n; len -= n;
	}

	fprintf(stderr, "%s\n", str);

	n = 0; len = 2047; s = str; *s = 0;
	n = snprintf(s, len, "%s():%d [%p] err(%p,%p)", func, line, opx_cq, opx_cq->err.head, opx_cq->err.tail); s += n; len -= n;
	if (opx_cq->err.head != NULL) {
		context = opx_cq->err.head;
		n = snprintf(s, len, " = { %p(%lu)", context, context->byte_counter); s += n; len -= n;

		context = context->next;
		while (context != NULL) {
			n = snprintf(s, len, ", %p(%lu)", context, context->byte_counter); s += n; len += n;
			context = context->next;
		}
		n = snprintf(s, len, " }"); s += n; len -= n;
	}

	fprintf(stderr, "%s\n", str);
}

static int fi_opx_close_cq(fid_t fid)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "close cq\n");

	int ret;
	struct fi_opx_cq *opx_cq =
		container_of(fid, struct fi_opx_cq, cq_fid);
	const int lock_required = fi_opx_threading_lock_required(opx_cq->domain->threading, fi_opx_global.progress);

	if (lock_required) {
		fi_opx_lock(&opx_cq->lock);
	}

	ret = fi_opx_fid_check(fid, FI_CLASS_CQ, "completion queue");
	if (ret)
		goto fail;

	ret = fi_opx_ref_dec(&opx_cq->domain->ref_cnt, "domain");
	if (ret)
		goto fail;

	ret = fi_opx_ref_finalize(&opx_cq->ref_cnt, "completion queue");
	if (ret)
		goto fail;

	if (fi_opx_global.progress == FI_PROGRESS_AUTO) {
		fi_opx_unlock(&opx_cq->lock);
	}

	if (opx_cq->progress_track) {
		fi_opx_stop_progress(opx_cq->progress_track);
		free(opx_cq->progress_track);
		opx_cq->progress_track = NULL;
	}

	ofi_spin_destroy(&opx_cq->lock);

	free(opx_cq);
	opx_cq = NULL;
	//opx_cq (the object passed in as fid) is now unusable 

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "cq closed\n");
	return 0;

fail:
	if (lock_required) {
		fi_opx_unlock(&opx_cq->lock);
	}
	return ret;
}

static int fi_opx_bind_cq(struct fid *fid, struct fid *bfid,
		uint64_t flags)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
	errno = FI_ENOSYS;
	return -errno;
}

static int fi_opx_control_cq(fid_t fid, int command, void *arg)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
	errno = FI_ENOSYS;
	return -errno;
}

static int fi_opx_ops_open_cq(struct fid *fid, const char *name,
		uint64_t flags, void **ops, void *context)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
	errno = FI_ENOSYS;
	return -errno;
}

static struct fi_ops fi_opx_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_opx_close_cq,
	.bind		= fi_opx_bind_cq,
	.control	= fi_opx_control_cq,
	.ops_open	= fi_opx_ops_open_cq
};

int fi_opx_cq_enqueue_err (struct fi_opx_cq * opx_cq,
		struct fi_opx_context_ext * ext,
		const int lock_required)
{
	assert(ext->opx_context.flags & FI_OPX_CQ_CONTEXT_EXT);	/* DEBUG */
	assert(!lock_required);
	ext->opx_context.next = NULL;

	fi_opx_context_slist_insert_tail((union fi_opx_context *)ext, &opx_cq->err);

	return 0;
}

struct fi_ops_cq * fi_opx_cq_select_ops(const enum fi_cq_format format,
		const enum fi_threading threading,
		const enum ofi_reliability_kind reliability,
		const uint64_t rcvhdrcnt,
		const uint64_t caps,
		const enum fi_progress progress)
{

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(called)\n");

	const uint64_t comm_caps = (caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) >> 51;
	assert(comm_caps < 4);

	// Figure out what functions to use for hdrq_mask
	// This is a per-context setting tuneable in the HFI1 driver as the rcvhdrcnt parm
	// Optimized values are 2048 (default) and 8192.  All other values will be non-optimal
	// 2048 computed mask is FI_OPX_HDRQ_MASK_2048, and is array element 1
	// 8192 computed mask is FI_OPX_HDRQ_MASK_8192 and is array element 2
	// Runtime handle is array element 0, use the computed value of fi_opx_hfi1_rxe_static.rx_poll_mask

	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		abort();
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);	

	switch(rcvhdrcnt) {	
		case 2048:
			return lock_required ? fi_opx_cq_select_locking_2048_ops(format, reliability, comm_caps) :
			                       fi_opx_cq_select_non_locking_2048_ops(format, reliability, comm_caps);
		case 8192:
			return lock_required ? fi_opx_cq_select_locking_8192_ops(format, reliability, comm_caps) :
			                       fi_opx_cq_select_non_locking_8192_ops(format, reliability, comm_caps);
		default:
			FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "WARNING: non-optimal setting specified for hfi1 rcvhdrcnt.  Optimal values are 2048 and 8192\n");			
			return lock_required ? fi_opx_cq_select_locking_runtime_ops(format, reliability, comm_caps) :
			                       fi_opx_cq_select_non_locking_runtime_ops(format, reliability, comm_caps);
	}

}

int fi_opx_cq_open(struct fid_domain *dom,
		struct fi_cq_attr *attr,
		struct fid_cq **cq, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "open cq\n");

	int ret;
	struct fi_opx_cq *opx_cq;

	if (!attr) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_CQ,
				"no attr supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}
	ret = fi_opx_fid_check(&dom->fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	opx_cq = calloc(1, sizeof(*opx_cq));
	if (!opx_cq) {
		errno = FI_ENOMEM;
		goto err;
	}

	opx_cq->cq_fid.fid.fclass = FI_CLASS_CQ;
	opx_cq->cq_fid.fid.context= context;
	opx_cq->cq_fid.fid.ops    = &fi_opx_fi_ops;

	opx_cq->size = attr->size ? attr->size : FI_OPX_DEFAULT_CQ_DEPTH;

	opx_cq->domain = (struct fi_opx_domain *) dom;

	opx_cq->format = attr->format ? attr->format : FI_CQ_FORMAT_CONTEXT;

	fi_opx_context_slist_init(&opx_cq->pending);
	fi_opx_context_slist_init(&opx_cq->completed);
	fi_opx_context_slist_init(&opx_cq->err);

	opx_cq->ep_bind_count = 0;
	opx_cq->progress.ep_count = 0;
	unsigned i;
	for (i=0; i<64; ++i) {		/* TODO - check this array size */
		opx_cq->ep[i] = NULL;
		opx_cq->progress.ep[i] = NULL;
	}


	//fi_opx_ref_init(&opx_cq->domain->fabric->node, &opx_cq->ref_cnt, "completion queue");
	fi_opx_ref_inc(&opx_cq->domain->ref_cnt, "domain");

	ofi_spin_init(&opx_cq->lock);

	*cq = &opx_cq->cq_fid;

	if (fi_opx_global.progress == FI_PROGRESS_AUTO){
		FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "Trying to start a PROGRESS_AUTO thread\n");
		opx_cq->progress_track = malloc(sizeof(struct fi_opx_progress_track));
		if (opx_cq->progress_track) {
			fi_opx_progress_init(opx_cq->progress_track);
			fi_opx_start_progress(opx_cq->progress_track, &opx_cq->cq_fid, opx_cq->domain->progress_affinity_str, opx_cq->domain->auto_progress_interval);
			if (!opx_cq->progress_track->progress_thread) {
				FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "Failed to start PROGRESS_AUTO thread\n");
				goto err;
			}
		} else {
			FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "Failed to setup PROGRESS_AUTO\n");
			goto err;
		}
	} else {
		opx_cq->progress_track = NULL;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "cq opened\n");
	return 0;
err:
	return -errno;
}

int fi_opx_bind_ep_cq(struct fid_ep *ep,
		struct fid_cq *cq, uint64_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(begin)\n");

	struct fi_opx_ep *opx_ep =
		container_of(ep, struct fi_opx_ep, ep_fid);

	struct fi_opx_cq *opx_cq =
		container_of(cq, struct fi_opx_cq, cq_fid);
	if (!(flags & (FI_TRANSMIT | FI_RECV)))
		goto err;

	if (flags & FI_TRANSMIT) {
		opx_ep->init_tx_cq = opx_cq;
		opx_ep->tx_cq_bflags = flags;
		opx_cq->bflags = flags;
	}

	if (flags & FI_RECV) {
		opx_ep->init_rx_cq = opx_cq;
		opx_ep->rx_cq_bflags = flags;
		opx_cq->bflags = flags;
	}

	if (FI_CLASS_RX_CTX == opx_ep->ep_fid.fid.fclass ||
			FI_CLASS_EP == opx_ep->ep_fid.fid.fclass) {
		opx_cq->ep[(opx_cq->ep_bind_count)++] = opx_ep;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(end)\n");
	return 0;
err:
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(end, error)\n");
	errno = FI_EINVAL;
	return -errno;
}


extern enum ofi_reliability_kind fi_opx_select_reliability(struct fi_opx_ep *opx_ep);
void fi_opx_cq_finalize_ops(struct fid_ep *ep)
{
	struct fi_opx_ep *opx_ep = (struct fi_opx_ep*) ep;
	struct fi_opx_cq *opx_cq = opx_ep->rx->cq;

	if (opx_cq) {
		opx_cq->cq_fid.ops =
		fi_opx_cq_select_ops(opx_cq->format,
			opx_cq->domain->threading,
			fi_opx_select_reliability(opx_ep),
			opx_ep->hfi->info.rxe.hdrq.elemcnt,
			opx_cq->ep_comm_caps,
			opx_cq->domain->data_progress);
	}

	if (opx_ep->tx->cq && (opx_ep->tx->cq != opx_ep->rx->cq)) {
		opx_cq = opx_ep->tx->cq;
		opx_cq->cq_fid.ops =
		fi_opx_cq_select_ops(opx_cq->format,
			opx_cq->domain->threading,
			fi_opx_select_reliability(opx_ep),
			opx_ep->hfi->info.rxe.hdrq.elemcnt,
			opx_cq->ep_comm_caps,
			opx_cq->domain->data_progress);
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(end)\n");
}

