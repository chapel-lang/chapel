/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2025 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_RMA_H_
#define _FI_PROV_OPX_RMA_H_

#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_eq.h"
#include "rdma/opx/fi_opx_rma_ops.h"
#include "rdma/opx/fi_opx_hfi1_transport.h"
#include "rdma/opx/fi_opx_hfi1_sdma.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OPX_NO_REMOTE_CQ_DATA 0

__OPX_FORCE_INLINE__
int fi_opx_check_rma(struct fi_opx_ep *opx_ep)
{
	return OFI_UNLIKELY(!opx_ep || (opx_ep->state != FI_OPX_EP_INITITALIZED_ENABLED) ||
			    (opx_ep->av->type == FI_AV_UNSPEC)) ?
		       -FI_EINVAL :
		       0;
}

void fi_opx_hit_zero(struct fi_opx_completion_counter *cc);

int fi_opx_do_readv_internal(union fi_opx_hfi1_deferred_work *work);
int fi_opx_do_readv_internal_shm(union fi_opx_hfi1_deferred_work *work);
int opx_hfi1_tx_rma_rts(union fi_opx_hfi1_deferred_work *work);
int opx_hfi1_tx_rma_rts_shm(union fi_opx_hfi1_deferred_work *work);

__OPX_FORCE_INLINE__
void opx_readv_internal(struct fi_opx_ep *opx_ep, const struct fi_opx_hmem_iov *iov, const size_t niov,
			const uint64_t *hmem_handle, const union fi_opx_addr opx_target_addr,
			const uint64_t *addr_offset, const uint64_t *key, const uint64_t tx_op_flags,
			const struct fi_opx_cq *opx_cq, const struct fi_opx_cntr *opx_cntr,
			struct fi_opx_completion_counter *cc, enum fi_datatype dt, enum fi_op op, const uint32_t opcode,
			const int lock_required, const uint64_t caps, const enum ofi_reliability_kind reliability,
			const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "READV_INTERNAL");

	union fi_opx_hfi1_deferred_work *work =
		(union fi_opx_hfi1_deferred_work *) ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct fi_opx_hfi1_rx_readv_params *params = &work->readv;
	params->opx_ep				   = opx_ep;
	params->work_elem.slist_entry.next	   = NULL;
	params->work_elem.completion_action	   = NULL;
	params->work_elem.payload_copy		   = NULL;
	params->work_elem.complete		   = false;

	params->opx_target_addr = opx_target_addr;
	params->key		= (key == NULL) ? -1 : *key;
	params->cc		= cc;
	params->dest_subctxt_rx = opx_target_addr.hfi1_subctxt_rx;
	params->bth_subctxt_rx	= params->dest_subctxt_rx << OPX_BTH_SUBCTXT_RX_SHIFT;
	params->pbc_dlid	= OPX_PBC_DLID(opx_target_addr.lid, hfi1_type);
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		params->pbc_dws = 2 + /* pbc */
				  2 + /* lrh */
				  3 + /* bth */
				  9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				  16; /* one "struct opx_hfi1_dput_iov" */
		params->lrh_dws =
			htons(params->pbc_dws - 2 +
			      1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
		params->lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(opx_target_addr.lid);
	} else {
		params->pbc_dws = 2 +			       /* pbc */
				  4 +			       /* lrh uncompressed */
				  3 +			       /* bth */
				  9 +			       /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				  16 +			       /* one "struct opx_hfi1_dput_iov" */
				  2;			       /* ICRC/tail */
		params->lrh_dws	 = (params->pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
		params->lrh_dlid = opx_target_addr.lid;
	}
	params->is_shm	    = fi_opx_hfi1_tx_is_shm(opx_ep, opx_target_addr, caps);
	params->reliability = reliability;
	params->opcode	    = opcode;

	assert(op == FI_NOOP || op < OFI_ATOMIC_OP_LAST);
	assert(dt == FI_VOID || dt < OFI_DATATYPE_LAST);
	params->op = (op == FI_NOOP) ? FI_NOOP - 1 : op;
	params->dt = (dt == FI_VOID) ? FI_VOID - 1 : dt;

	assert(niov == 1); // TODO, support something ... bigger
	params->niov		     = niov;
	params->dput_iov.rbuf	     = iov->buf;
	params->dput_iov.sbuf	     = *addr_offset;
	params->dput_iov.bytes	     = iov->len;
	params->dput_iov.rbuf_iface  = iov->iface;
	params->dput_iov.rbuf_device = iov->device;
	params->dput_iov.sbuf_iface  = FI_HMEM_SYSTEM;	   // TBD by remote node
	params->dput_iov.sbuf_device = 0;		   // TBD by remote node
	params->dput_iov.sbuf_handle = OPX_HMEM_NO_HANDLE; // TBD by remote node

	params->rma_request = ofi_buf_alloc(opx_ep->tx->rma_request_pool);
	assert(params->rma_request != NULL);
	params->rma_request->cc		 = cc;
	params->rma_request->hmem_iface	 = iov->iface;
	params->rma_request->hmem_device = iov->device;
	params->rma_request->hmem_handle = hmem_handle[0];

	if (params->is_shm) {
		params->work_elem.work_fn   = fi_opx_do_readv_internal_shm;
		params->work_elem.work_type = OPX_WORK_TYPE_SHM;
	} else {
		params->work_elem.work_fn   = fi_opx_do_readv_internal;
		params->work_elem.work_type = OPX_WORK_TYPE_PIO;
	}

	FI_OPX_DEBUG_COUNTERS_INC_COND((iov->iface != FI_HMEM_SYSTEM) && params->is_shm,
				       opx_ep->debug_counters.hmem.rma_read_intranode);
	FI_OPX_DEBUG_COUNTERS_INC_COND((iov->iface != FI_HMEM_SYSTEM) && !params->is_shm,
				       opx_ep->debug_counters.hmem.rma_read_hfi);

	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy u8_rx field.  Although the dest_rx field
	 * can support the larger values, in order to maintain consistency with other
	 * deferred work operations, continue to use the u32_extended_rx field.
	 */
	params->u32_extended_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, params->is_shm, params->dest_subctxt_rx);

	int rc = params->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "READV_INTERNAL");
		return;
	}
	assert(rc == -FI_EAGAIN);

	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[params->work_elem.work_type]);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "READV_INTERNAL");
}

__OPX_FORCE_INLINE__
void opx_write_internal(struct fi_opx_ep *opx_ep, const struct fi_opx_hmem_iov *iov, const size_t niov,
			const uint64_t data, const union fi_opx_addr opx_dst_addr, uint64_t addr_offset,
			const uint64_t key, struct fi_opx_completion_counter *cc, enum fi_datatype dt, enum fi_op op,
			const uint64_t tx_op_flags, const uint64_t is_hmem, const uint64_t handle, int lock_required,
			const uint64_t caps, const enum ofi_reliability_kind reliability,
			const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "WRITE_INTERNAL");
	assert(niov == 1); // TODO, support something ... bigger
	assert(op == FI_NOOP || op < OFI_ATOMIC_OP_LAST);
	assert(dt == FI_VOID || dt < OFI_DATATYPE_LAST);

	union fi_opx_hfi1_deferred_work *work =
		(union fi_opx_hfi1_deferred_work *) ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	const uint64_t is_shm	       = fi_opx_hfi1_tx_is_shm(opx_ep, opx_dst_addr, caps);
	uint32_t       u32_extended_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, opx_dst_addr.hfi1_subctxt_rx);

	uint64_t lrh_dlid;
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(opx_dst_addr.lid);
	} else {
		lrh_dlid = opx_dst_addr.lid;
	}
	uint64_t slid	  = opx_dst_addr.lid;
	uint64_t pbc_dlid = OPX_PBC_DLID(opx_dst_addr.lid, hfi1_type);

	if (tx_op_flags & FI_REMOTE_CQ_DATA) {
		struct fi_opx_rma_request *rma_req = ofi_buf_alloc(opx_ep->tx->rma_request_pool);
		assert(rma_req != NULL);

		struct opx_hfi1_rma_rts_params *params = &work->rma_rts;
		params->work_elem.slist_entry.next     = NULL;
		params->work_elem.completion_action    = NULL;
		params->work_elem.payload_copy	       = NULL;
		params->work_elem.complete	       = false;
		params->opx_ep			       = opx_ep;
		params->lrh_dlid		       = lrh_dlid;
		params->slid			       = slid;
		params->pbc_dlid		       = pbc_dlid;

		params->niov	       = niov;
		rma_req->cc	       = cc;
		params->origin_rma_req = rma_req;
		params->key	       = key;
		params->data	       = data;

		// params->iov[0].rbuf_iface = FI_HMEM_SYSTEM;		// TBD on remote node
		// params->iov[0].rbuf_device = 0;			// TBD on remote node
		params->dput_iov[0].sbuf_iface	= iov[0].iface;
		params->dput_iov[0].sbuf_device = iov[0].device;
		params->dput_iov[0].sbuf_handle = handle;
		params->dput_iov[0].rbuf	= addr_offset;
		params->dput_iov[0].sbuf	= iov[0].buf;
		params->dput_iov[0].bytes	= iov[0].len;

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "is_shm %lu, opcode=%u\n", is_shm,
		       FI_OPX_HFI_BTH_OPCODE_RMA_RTS);
		if (is_shm) {
			params->work_elem.work_fn   = opx_hfi1_tx_rma_rts_shm;
			params->work_elem.work_type = OPX_WORK_TYPE_SHM;
		} else {
			params->work_elem.work_fn   = opx_hfi1_tx_rma_rts;
			params->work_elem.work_type = OPX_WORK_TYPE_PIO;
		}

		params->u32_extended_rx = u32_extended_rx;
		params->reliability	= reliability;
		params->dest_rx		= opx_dst_addr.hfi1_subctxt_rx;
		params->is_shm		= is_shm;
		params->opcode		= FI_OPX_HFI_BTH_OPCODE_RMA_RTS;
		params->dt		= dt == FI_VOID ? FI_VOID - 1 : dt;
		params->op		= op == FI_NOOP ? FI_NOOP - 1 : op;
		params->target_hfi_unit = opx_dst_addr.hfi1_unit;

	} else { // else we don't have CQ_DATA and can just send the DPUT PUT

		struct fi_opx_hfi1_dput_params *params = &work->dput;

		params->work_elem.slist_entry.next  = NULL;
		params->work_elem.completion_action = NULL;
		params->work_elem.payload_copy	    = NULL;
		params->work_elem.complete	    = false;
		params->opx_ep			    = opx_ep;
		params->lrh_dlid		    = lrh_dlid;
		params->pbc_dlid		    = pbc_dlid;
		params->slid			    = slid;
		params->origin_rx		    = opx_dst_addr.hfi1_subctxt_rx;
		params->dt			    = dt == FI_VOID ? FI_VOID - 1 : dt;
		params->op			    = op == FI_NOOP ? FI_NOOP - 1 : op;
		params->key			    = key;
		params->cc			    = cc;
		params->user_cc			    = NULL;
		params->niov			    = niov;
		params->iov[0].bytes		    = iov->len;
		params->iov[0].rbuf		    = addr_offset;
		params->iov[0].sbuf		    = iov->buf;
		params->iov[0].sbuf_iface	    = iov->iface;
		params->iov[0].sbuf_device	    = iov->device;
		params->iov[0].sbuf_handle	    = handle;
		params->iov[0].rbuf_iface	    = FI_HMEM_SYSTEM; // TBD on remote node
		params->iov[0].rbuf_device	    = 0;	      // TBD on remote node
		params->dput_iov		    = &params->iov[0];
		params->opcode			    = FI_OPX_HFI_DPUT_OPCODE_PUT;
		params->is_shm			    = is_shm;
		params->origin_rx		    = opx_dst_addr.hfi1_subctxt_rx; // dest_rx, also used for bth_rx
		params->u32_extended_rx		    = u32_extended_rx;		    // dest_rx, also used for bth_rx
		params->reliability		    = reliability;
		params->cur_iov			    = 0;
		params->bytes_sent		    = 0;
		params->src_base_addr		    = NULL;
		params->origin_byte_counter	    = NULL;
		params->payload_bytes_for_iovec	    = 0;
		params->target_hfi_unit		    = opx_dst_addr.hfi1_unit;

		fi_opx_hfi1_dput_sdma_init(opx_ep, params, iov->len, 0, 0, NULL, is_hmem, hfi1_type);
		FI_OPX_DEBUG_COUNTERS_INC_COND(is_hmem && is_shm, opx_ep->debug_counters.hmem.rma_write_intranode);
		FI_OPX_DEBUG_COUNTERS_INC_COND(is_hmem && !is_shm, opx_ep->debug_counters.hmem.rma_write_hfi);
	}

	ssize_t rc = work->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		assert(work->work_elem.complete);
		OPX_BUF_FREE(work);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITE_INTERNAL");
		return;
	}
	assert(rc == -FI_EAGAIN);
	if (work->work_elem.work_type == OPX_WORK_TYPE_LAST) {
		slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending_completion);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITE_INTERNAL");
		return;
	}
	/* We weren't able to complete the write on the first try. If this was an inject,
	   the outbound buffer may be re-used as soon as we return to the caller, even when
	   this operation will be completed asyncronously. So copy the payload bytes into
	   our own copy of the buffer, and set iov.sbuf to point to it. */
	if (tx_op_flags & FI_INJECT) {
		struct fi_opx_hfi1_dput_params *params = &work->dput;
		assert(iov->len <= FI_OPX_HFI1_PACKET_IMM);
		OPX_HMEM_COPY_FROM((void *) params->inject_data, (void *) iov->buf, iov->len, OPX_HMEM_NO_HANDLE,
				   OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET, iov->iface, iov->device);
		params->iov[0].sbuf	   = (uintptr_t) params->inject_data;
		params->iov[0].sbuf_iface  = FI_HMEM_SYSTEM;
		params->iov[0].sbuf_device = 0;
	}

	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[work->work_elem.work_type]);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITE_INTERNAL");
}

__OPX_FORCE_INLINE__
ssize_t opx_rma_get_context(struct fi_opx_ep *opx_ep, const void *user_context, const void *cq, const uint64_t flags,
			    struct opx_context **context)
{
	if (!cq || !user_context) {
		*context = NULL;
		return FI_SUCCESS;
	}

	struct opx_context *ctx = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
	if (OFI_UNLIKELY(ctx == NULL)) {
		*context = NULL;
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		return -FI_ENOMEM;
	}

	ctx->next		  = NULL;
	ctx->flags		  = (uint64_t) flags;
	ctx->err_entry.err	  = 0;
	ctx->err_entry.op_context = (void *) user_context;

	*context = ctx;
	return FI_SUCCESS;
}

ssize_t fi_opx_inject_write_generic(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dst_addr,
				    uint64_t addr_offset, uint64_t key, int lock_required,
				    const enum fi_av_type av_type, const uint64_t caps,
				    const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

ssize_t fi_opx_write_generic(struct fid_ep *ep, const void *buf, size_t len, void *desc, const uint64_t data,
			     fi_addr_t dst_addr, uint64_t addr_offset, uint64_t key, void *context, int lock_required,
			     const enum fi_av_type av_type, const uint64_t tx_op_flags, const uint64_t caps,
			     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

ssize_t fi_opx_writev_generic(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count, fi_addr_t dst_addr,
			      uint64_t addr_offset, uint64_t key, void *context, int lock_required,
			      const enum fi_av_type av_type, const uint64_t caps,
			      const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

ssize_t fi_opx_writemsg_generic(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags, int lock_required,
				const enum fi_av_type av_type, const uint64_t caps,
				const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

ssize_t fi_opx_read_generic(struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,
			    uint64_t addr_offset, uint64_t key, void *context, int lock_required,
			    const enum fi_av_type av_type, const uint64_t caps,
			    const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

ssize_t fi_opx_readv_generic(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count, fi_addr_t src_addr,
			     uint64_t addr_offset, uint64_t key, void *context, int lock_required,
			     const enum fi_av_type av_type, const uint64_t caps,
			     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

ssize_t fi_opx_readmsg_generic(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags, int lock_required,
			       const enum fi_av_type av_type, const uint64_t caps,
			       const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_RMA_H_ */
