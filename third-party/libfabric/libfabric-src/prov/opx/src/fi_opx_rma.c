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
#include <ofi.h>

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_rma.h"
#include "rdma/opx/fi_opx_eq.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_hfi1_version.h"

#include <ofi_enosys.h>
#include <errno.h>

/* Delivery completion callback for atomics, RMA operations.
 *
 * This differs from the other completion callback in that it ignores
 * any message tag.
 */
void fi_opx_hit_zero(struct fi_opx_completion_counter *cc)
{
	if (cc->cntr) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== COUNTER INCREMENT\n");
		ofi_atomic_inc64(&cc->cntr->std);
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== NO COUNTER INCREMENT\n");
	}
	if (cc->cq && cc->context) {
		cc->context->next	  = NULL;
		cc->context->len	  = 0;
		cc->context->buf	  = NULL;
		cc->context->byte_counter = 0;
		cc->context->tag	  = 0;

		assert(cc->context->err_entry.op_context != NULL);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== CQ ENQUEUE COMPLETION\n");
		fi_opx_cq_enqueue_completed(cc->cq, cc->context, FI_OPX_LOCK_NOT_REQUIRED);
	} else {
		if (cc->context) {
			OPX_BUF_FREE(cc->context);
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== NO CQ COMPLETION\n");
	}
	OPX_BUF_FREE(cc);
}

int fi_opx_do_readv_internal_shm(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_readv_params *params = &work->readv;
	struct fi_opx_ep		   *opx_ep = params->opx_ep;

	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy u8_rx field.  Use the dest_rx field
	 * which can support the larger values.
	 */
	ssize_t rc = fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->dest_subctxt_rx,
						   params->opx_target_addr.hfi1_unit);
	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	uint64_t pos;
	/* DAOS support - rank_inst field has been depricated and will be phased out.
	 * The value is always zero.*/
	union opx_hfi1_packet_hdr *hdr =
		opx_shm_tx_next(&opx_ep->tx->shm, params->opx_target_addr.hfi1_unit, params->dest_subctxt_rx, &pos,
				opx_ep->daos_info.hfi_rank_enabled, params->u32_extended_rx, 0, &rc);
	if (OFI_UNLIKELY(hdr == NULL)) {
		return rc;
	}
	uint64_t niov = params->niov << 48;
	uint64_t op64 = params->op << 40;
	uint64_t dt64 = params->dt << 32;
	assert(FI_OPX_HFI_DPUT_OPCODE_GET == params->opcode); // double check packet type
	assert(params->dt == (FI_VOID - 1) || params->dt < FI_DATATYPE_LAST);
	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		hdr->qw_9B[0] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | params->lrh_dlid | (params->lrh_dws << 32);
		hdr->qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | params->bth_subctxt_rx;
		hdr->qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2];
		hdr->qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
		hdr->qw_9B[4] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | (params->opcode) | dt64 | op64 | niov;
		hdr->qw_9B[5] = (uintptr_t) params->rma_request;
		hdr->qw_9B[6] = params->key;
	} else {
		hdr->qw_16B[0] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
				 ((uint64_t) (params->lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
				  << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
				 ((uint64_t) params->lrh_dws << 20);
		hdr->qw_16B[1] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
				 ((uint64_t) ((params->lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					      OPX_LRH_JKR_16B_DLID20_SHIFT_16B) |
				  (uint64_t) (params->bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B));
		hdr->qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | params->bth_subctxt_rx;
		hdr->qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3];
		hdr->qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
		hdr->qw_16B[5] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | params->opcode | dt64 | op64 | niov;
		hdr->qw_16B[6] = (uintptr_t) params->rma_request;
		hdr->qw_16B[7] = params->key;
	}

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	tx_payload->cts.iov[0] = params->dput_iov;

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	return FI_SUCCESS;
}

int fi_opx_do_readv_internal(union fi_opx_hfi1_deferred_work *work)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "DO_READV");
	struct fi_opx_hfi1_rx_readv_params *params    = &work->readv;
	struct fi_opx_ep		   *opx_ep    = params->opx_ep;
	const enum opx_hfi1_type	    hfi1_type = OPX_HFI1_TYPE;

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const int credits_needed    = (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) ? 2 : 3;
	ssize_t	  credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, credits_needed);
	if (OFI_UNLIKELY(credits_available < 0)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "DO_READV");
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int32_t				     psn;

	const union fi_opx_addr addr = params->opx_target_addr;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, params->reliability, hfi1_type);

	if (OFI_UNLIKELY(psn == -1)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "DO_READV");
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	uint64_t niov	       = params->niov << 48;
	uint64_t op64	       = params->op << 40;
	uint64_t dt64	       = params->dt << 32;
	uint64_t credit_return = OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type);
	assert(FI_OPX_HFI_DPUT_OPCODE_GET == params->opcode); // double check packet type

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		opx_cacheline_copy_qw_vol(
			scb, replay->scb.qws,
			opx_ep->rx->tx.cts_9B.qw0 | OPX_PBC_LEN(params->pbc_dws, hfi1_type) | credit_return |
				params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type),
			opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | params->lrh_dlid | (params->lrh_dws << 32),
			opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | params->bth_subctxt_rx,
			opx_ep->rx->tx.cts_9B.hdr.qw_9B[2] | psn, opx_ep->rx->tx.cts_9B.hdr.qw_9B[3],
			opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | params->opcode | dt64 | op64 | niov,
			(uintptr_t) params->rma_request,
			params->key); // key
		/* consume one credit for the packet header */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

		FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

		/* write the CTS payload "send control block"  */
		volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_copy_qw_vol(scb_payload, replay->payload, params->dput_iov.qw[0], params->dput_iov.qw[1],
					  params->dput_iov.qw[2], params->dput_iov.qw[3], params->dput_iov.qw[4],
					  params->dput_iov.qw[5], 0, 0);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	} else {
		opx_cacheline_copy_qw_vol(
			scb, replay->scb.qws,
			opx_ep->rx->tx.cts_16B.qw0 | OPX_PBC_LEN(params->pbc_dws, hfi1_type) | credit_return |
				params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type),
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
				((uint64_t) (params->lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
				 << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
				((uint64_t) params->lrh_dws << 20),
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
				((uint64_t) ((params->lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					     OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				(uint64_t) (params->bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | params->bth_subctxt_rx,
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[3] | psn, opx_ep->rx->tx.cts_16B.hdr.qw_16B[4],
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | params->opcode | dt64 | op64 | niov,
			(uintptr_t) params->rma_request);
		/* consume one credit for the packet header */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

		FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

		volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_copy_qw_vol(scb_payload, &replay->scb.qws[8], params->key, params->dput_iov.qw[0],
					  params->dput_iov.qw[1], params->dput_iov.qw[2], params->dput_iov.qw[3],
					  params->dput_iov.qw[4], params->dput_iov.qw[5], 0UL);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

		scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_store_qw_vol(scb_payload, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL);
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, params->reliability,
							     OPX_HFI1_TYPE);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "DO_READV");
	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_inject_write_internal(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dst_addr,
				     uint64_t addr_offset, uint64_t key, int lock_required,
				     const enum fi_av_type av_type, const uint64_t caps,
				     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "INJECT_WRITE");
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "INJECT_WRITE");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "INJECT_WRITE");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(dst_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, dst_addr);

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	cc->next			     = NULL;
	cc->initial_byte_count		     = len;
	cc->byte_counter		     = len;
	cc->cntr			     = opx_ep->write_cntr;
	cc->cq				     = NULL;
	cc->context			     = NULL;
	cc->hit_zero			     = fi_opx_hit_zero;

	struct fi_opx_hmem_iov iov;
	uint64_t	       handle;
	const uint64_t	       is_hmem = opx_hmem_iov_init(buf, len, NULL, &iov, &handle);

	opx_write_internal(opx_ep, &iov, 1, OPX_NO_REMOTE_CQ_DATA, opx_dst_addr, addr_offset, key, cc, FI_VOID, FI_NOOP,
			   opx_ep->tx->op_flags | FI_INJECT, is_hmem, handle, lock_required, caps, reliability,
			   hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "INJECT_WRITE");
	return 0;
}

inline ssize_t fi_opx_inject_write_generic(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dst_addr,
					   uint64_t addr_offset, uint64_t key, int lock_required,
					   const enum fi_av_type av_type, const uint64_t caps,
					   const enum ofi_reliability_kind reliability,
					   const enum opx_hfi1_type	   hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA INJECT WRITE (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_inject_write_internal(ep, buf, len, dst_addr, addr_offset, key, FI_OPX_LOCK_NOT_REQUIRED,
						  av_type, caps, reliability, hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA INJECT WRITE (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_write(struct fid_ep *ep, const void *buf, size_t len, void *desc, const uint64_t data,
		     fi_addr_t dst_addr, uint64_t addr_offset, uint64_t key, void *user_context, int lock_required,
		     const enum fi_av_type av_type, const uint64_t tx_op_flags, const uint64_t caps,
		     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "WRITE");
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITE");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITE");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(dst_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, dst_addr);
	const uint64_t		op_flags     = opx_ep->tx->op_flags | (tx_op_flags & FI_REMOTE_CQ_DATA);

	struct fi_opx_cq *cq =
		(op_flags & (FI_COMPLETION | FI_DELIVERY_COMPLETE | FI_REMOTE_CQ_DATA)) ? opx_ep->tx->cq : NULL;
	struct opx_context *context;
	if (OFI_UNLIKELY(opx_rma_get_context(opx_ep, user_context, cq,
					     FI_RMA | FI_WRITE | (op_flags & FI_REMOTE_CQ_DATA),
					     &context) != FI_SUCCESS)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITE");
		return -FI_ENOMEM;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	if (OFI_UNLIKELY(cc == NULL)) {
		if (context) {
			OPX_BUF_FREE(context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITE");
		return -FI_ENOMEM;
	}

	cc->next	       = NULL;
	cc->initial_byte_count = len;
	cc->byte_counter       = len;
	cc->cntr	       = opx_ep->write_cntr;
	cc->cq		       = cq;
	cc->context	       = context;
	cc->hit_zero	       = fi_opx_hit_zero;

	struct fi_opx_hmem_iov iov;
	uint64_t	       handle;
	const uint64_t	       is_hmem = opx_hmem_iov_init(buf, len, desc, &iov, &handle);
	opx_write_internal(opx_ep, &iov, 1, data, opx_dst_addr, addr_offset, key, cc, FI_VOID, FI_NOOP, op_flags,
			   is_hmem, handle, lock_required, caps, reliability, hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITE");
	return 0;
}

inline ssize_t fi_opx_write_generic(struct fid_ep *ep, const void *buf, size_t len, void *desc, const uint64_t data,
				    fi_addr_t dst_addr, uint64_t addr_offset, uint64_t key, void *context,
				    int lock_required, const enum fi_av_type av_type, const uint64_t tx_op_flags,
				    const uint64_t caps, const enum ofi_reliability_kind reliability,
				    const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITE (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_write(ep, buf, len, desc, data, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, av_type, tx_op_flags, caps, reliability, hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITE (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_writev_internal(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,
			       fi_addr_t dst_addr, uint64_t addr_offset, uint64_t key, void *user_context,
			       int lock_required, const enum fi_av_type av_type, const uint64_t caps,
			       const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "WRITEV_INTERNAL");
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEV_INTERNAL");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEV_INTERNAL");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(dst_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, dst_addr);

	struct fi_opx_cq *cq = (opx_ep->tx->op_flags & (FI_COMPLETION | FI_DELIVERY_COMPLETE)) ? opx_ep->tx->cq : NULL;
	struct opx_context *context;
	if (OFI_UNLIKELY(opx_rma_get_context(opx_ep, user_context, cq, FI_RMA | FI_WRITE, &context) != FI_SUCCESS)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEV_INTERNAL");
		return -FI_ENOMEM;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	if (OFI_UNLIKELY(cc == NULL)) {
		if (context) {
			OPX_BUF_FREE(context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEV_INTERNAL");
		return -FI_ENOMEM;
	}

	size_t index;
	cc->next	 = NULL;
	cc->byte_counter = 0;
	for (index = 0; index < count; ++index) {
		cc->byte_counter += iov[index].iov_len;
	}
	cc->initial_byte_count = cc->byte_counter;
	cc->cntr	       = opx_ep->write_cntr;
	cc->cq		       = cq;
	cc->context	       = context;
	cc->hit_zero	       = fi_opx_hit_zero;

	struct fi_opx_mr **mr_ptr_array = (struct fi_opx_mr **) desc;
	for (index = 0; index < count; ++index) {
		struct fi_opx_mr *mr_ptr;
		if (mr_ptr_array != NULL) {
			mr_ptr = *mr_ptr_array;
			++mr_ptr_array;
		} else {
			mr_ptr = NULL;
		}
		struct fi_opx_hmem_iov hmem_iov;
		uint64_t	       handle;
		const uint64_t	       is_hmem =
			opx_hmem_iov_init(iov[index].iov_base, iov[index].iov_len, mr_ptr, &hmem_iov, &handle);
		opx_write_internal(opx_ep, &hmem_iov, 1, OPX_NO_REMOTE_CQ_DATA, opx_dst_addr, addr_offset, key, cc,
				   FI_VOID, FI_NOOP, 0, is_hmem, handle, lock_required, caps, reliability, hfi1_type);

		addr_offset += iov[index].iov_len;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITEV_INTERNAL");
	return 0;
}

inline ssize_t fi_opx_writev_generic(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,
				     fi_addr_t dst_addr, uint64_t addr_offset, uint64_t key, void *context,
				     int lock_required, const enum fi_av_type av_type, const uint64_t caps,
				     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEV (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_writev_internal(ep, iov, desc, count, dst_addr, addr_offset, key, context,
					    FI_OPX_LOCK_NOT_REQUIRED, av_type, caps, reliability, hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEV (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
void fi_opx_get_daos_av_addr_rank(struct fi_opx_ep *opx_ep, const union fi_opx_addr dst_addr)
{
#ifdef OPX_DAOS
	if (opx_ep->daos_info.av_rank_hashmap) {
		struct fi_opx_daos_av_rank     *cur_av_rank = NULL;
		struct fi_opx_daos_av_rank     *tmp_av_rank = NULL;
		__attribute__((__unused__)) int i	    = 0;
		int				found	    = 0;

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Get av_rank_hashmap - (DLID:0x%x fi:%08lx)\n",
			     dst_addr.lid, dst_addr.fi);

		HASH_ITER(hh, opx_ep->daos_info.av_rank_hashmap, cur_av_rank, tmp_av_rank)
		{
			if (cur_av_rank) {
				union fi_opx_addr addr;
				addr.fi = cur_av_rank->fi_addr;

				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "Get av_rank_hashmap[%d] = rank:%d, LID:0x%x, fi:%08lx.\n", i++,
					     cur_av_rank->key.rank, addr.lid, addr.fi);

				if (addr.fi == dst_addr.fi) {
					found			    = 1;
					opx_ep->daos_info.rank	    = cur_av_rank->key.rank;
					opx_ep->daos_info.rank_inst = cur_av_rank->key.rank_inst;

					FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
						     "Get av_rank_hashmap[%d] = rank:%d, LID:0x%x fi:%08lx - Found.\n",
						     (i - 1), opx_ep->daos_info.rank, addr.lid, addr.fi);
					break;
				}
			}
		}

		if (!found) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Dest addr %08lx not found in av_rank_hashmap.\n",
				dst_addr.fi);
		}
	}
#endif
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_writemsg_internal(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags, int lock_required,
				 const enum fi_av_type av_type, const uint64_t caps,
				 const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "WRITEMSG_INTERAL");
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEMSG_INTERNAL");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEMSG_INTERNAL");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(msg->addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, msg->addr);
	fi_opx_get_daos_av_addr_rank(opx_ep, opx_dst_addr);

	struct fi_opx_cq   *cq = (flags & (FI_COMPLETION | FI_REMOTE_CQ_DATA)) ? opx_ep->tx->cq : NULL;
	struct opx_context *context;
	if (OFI_UNLIKELY(opx_rma_get_context(opx_ep, msg->context, cq,
					     (FI_RMA | FI_WRITE | (flags & FI_REMOTE_CQ_DATA)),
					     &context) != FI_SUCCESS)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEMSG_INTERNAL");
		return -FI_ENOMEM;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	if (OFI_UNLIKELY(cc == NULL)) {
		if (context) {
			OPX_BUF_FREE(context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "WRITEMSG_INTERNAL");
		return -FI_ENOMEM;
	}

	size_t index;
	cc->next	 = NULL;
	cc->byte_counter = 0;
	for (index = 0; index < msg->iov_count; index++) {
		cc->byte_counter += msg->msg_iov[index].iov_len;
	}
	cc->initial_byte_count = cc->byte_counter;

	cc->cntr     = opx_ep->write_cntr;
	cc->cq	     = cq;
	cc->context  = context;
	cc->hit_zero = fi_opx_hit_zero;

	size_t	     rma_iov_index = 0;
	const size_t rma_iov_count = msg->rma_iov_count;
	uint64_t     rma_iov_bytes = msg->rma_iov[rma_iov_index].len;
	uint64_t     rma_iov_addr  = msg->rma_iov[rma_iov_index].addr;
	uint64_t     rma_iov_key   = msg->rma_iov[rma_iov_index].key;

	size_t	     msg_iov_index = 0;
	const size_t msg_iov_count = msg->iov_count;
	uint64_t     msg_iov_bytes = msg->msg_iov[msg_iov_index].iov_len;
	uintptr_t    msg_iov_vaddr = (uintptr_t) msg->msg_iov[msg_iov_index].iov_base;

	struct fi_opx_mr     **mr_ptr_array = (struct fi_opx_mr **) msg->desc;
	struct fi_opx_hmem_iov iov;

	while (msg_iov_bytes != 0 && rma_iov_bytes != 0) {
		struct fi_opx_mr *mr_ptr;
		if (mr_ptr_array != NULL) {
			mr_ptr = *mr_ptr_array;
			++mr_ptr_array;
		} else {
			mr_ptr = NULL;
		}
		uint64_t handle;
		uint64_t is_hmem = opx_hmem_iov_init((void *) msg_iov_vaddr, msg_iov_bytes, mr_ptr, &iov, &handle);
		size_t	 len	 = (msg_iov_bytes <= rma_iov_bytes) ? msg_iov_bytes : rma_iov_bytes;
		iov.buf		 = msg_iov_vaddr;
		iov.len		 = len;
		opx_write_internal(opx_ep, &iov, 1, msg->data, opx_dst_addr, rma_iov_addr, rma_iov_key, cc, FI_VOID,
				   FI_NOOP, flags, is_hmem, handle, lock_required, caps, reliability, hfi1_type);

		msg_iov_bytes -= len;
		msg_iov_vaddr += len;

		if ((msg_iov_bytes == 0) && ((msg_iov_index + 1) < msg_iov_count)) {
			++msg_iov_index;
			msg_iov_bytes = msg->msg_iov[msg_iov_index].iov_len;
			msg_iov_vaddr = (uintptr_t) msg->msg_iov[msg_iov_index].iov_base;
			is_hmem	      = opx_hmem_iov_init((void *) msg_iov_vaddr, msg_iov_bytes, mr_ptr, &iov, &handle);
		}

		rma_iov_bytes -= len;
		rma_iov_addr += len;

		if ((rma_iov_bytes == 0) && ((rma_iov_index + 1) < rma_iov_count)) {
			++rma_iov_index;
			rma_iov_bytes = msg->rma_iov[rma_iov_index].len;
			rma_iov_addr  = msg->rma_iov[rma_iov_index].addr;
			rma_iov_key   = msg->rma_iov[rma_iov_index].key;
		}
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITEMSG_INTERNAL");
	return 0;
}

inline ssize_t fi_opx_writemsg_generic(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags,
				       int lock_required, const enum fi_av_type av_type, const uint64_t caps,
				       const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA WRITEMSG (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_writemsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, av_type, caps, reliability,
					      hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEMSG (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_read_internal(struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,
			     uint64_t addr_offset, uint64_t key, void *user_context, int lock_required,
			     const enum fi_av_type av_type, const uint64_t caps,
			     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "READ");
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READ");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READ");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

#ifdef OPX_HMEM
	uint64_t	   hmem_device;
	uint64_t	   hmem_handle;
	enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(desc, &hmem_device, &hmem_handle);
#else
	const enum fi_hmem_iface hmem_iface  = FI_HMEM_SYSTEM;
	const uint64_t		 hmem_device = 0;
	const uint64_t		 hmem_handle = OPX_HMEM_NO_HANDLE;
#endif

	struct fi_opx_hmem_iov iov = {.buf = (uintptr_t) buf, .len = len, .iface = hmem_iface, .device = hmem_device};

	assert(src_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, src_addr);
	fi_opx_get_daos_av_addr_rank(opx_ep, opx_addr);

	struct fi_opx_cq *cq = (opx_ep->tx->op_flags & (FI_COMPLETION | FI_DELIVERY_COMPLETE)) ? opx_ep->tx->cq : NULL;
	struct opx_context *context;
	if (OFI_UNLIKELY(opx_rma_get_context(opx_ep, user_context, cq, FI_RMA | FI_READ, &context) != FI_SUCCESS)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READ");
		return -FI_ENOMEM;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	if (OFI_UNLIKELY(cc == NULL)) {
		if (context) {
			OPX_BUF_FREE(context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READ");
		return -FI_ENOMEM;
	}

	cc->next	       = NULL;
	cc->byte_counter       = len;
	cc->initial_byte_count = len;
	cc->cntr	       = opx_ep->read_cntr;
	cc->cq		       = cq;
	cc->context	       = context;
	cc->hit_zero	       = fi_opx_hit_zero;

	opx_readv_internal(opx_ep, &iov, 1, &hmem_handle, opx_addr, &addr_offset, &key, opx_ep->tx->op_flags,
			   opx_ep->tx->cq, opx_ep->read_cntr, cc, FI_VOID, FI_NOOP, FI_OPX_HFI_DPUT_OPCODE_GET,
			   lock_required, caps, reliability, hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "READ");
	return FI_SUCCESS;
}

inline ssize_t fi_opx_read_generic(struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,
				   uint64_t addr_offset, uint64_t key, void *context, int lock_required,
				   const enum fi_av_type av_type, const uint64_t caps,
				   const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READ (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_read_internal(ep, buf, len, desc, src_addr, addr_offset, key, context,
					  FI_OPX_LOCK_NOT_REQUIRED, av_type, caps, reliability, hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READ (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_readv(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count, fi_addr_t src_addr,
		     uint64_t addr_offset, uint64_t key, void *user_context, int lock_required,
		     const enum fi_av_type av_type, const uint64_t caps, const enum ofi_reliability_kind reliability,
		     const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "READV");
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READV");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READV");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(src_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, src_addr);

	struct fi_opx_cq *cq = (opx_ep->tx->op_flags & (FI_COMPLETION | FI_DELIVERY_COMPLETE)) ? opx_ep->tx->cq : NULL;
	struct opx_context *context;
	if (OFI_UNLIKELY(opx_rma_get_context(opx_ep, user_context, cq, FI_RMA | FI_READ, &context) != FI_SUCCESS)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READV");
		return -FI_ENOMEM;
	}

	const uint64_t tx_op_flags = opx_ep->tx->op_flags;

	uint64_t addr_v[8] = {addr_offset, addr_offset, addr_offset, addr_offset,
			      addr_offset, addr_offset, addr_offset, addr_offset};
	uint64_t key_v[8]  = {key, key, key, key, key, key, key, key};

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	if (OFI_UNLIKELY(cc == NULL)) {
		if (context) {
			OPX_BUF_FREE(context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READV");
		return -FI_ENOMEM;
	}

	size_t index;
	cc->next	 = NULL;
	cc->byte_counter = 0;
	for (index = 0; index < count; index++) {
		cc->byte_counter += iov[index].iov_len;
	}
	cc->initial_byte_count = cc->byte_counter;
	cc->cntr	       = opx_ep->read_cntr;
	cc->cq		       = cq;
	cc->context	       = context;
	cc->hit_zero	       = fi_opx_hit_zero;

	uint64_t	       hmem_device;
	enum fi_hmem_iface     hmem_iface;
	uint64_t	       hmem_handle[8];
	struct fi_opx_hmem_iov hmem_iovs[8];

	/* max 8 descriptors (iovecs) per readv_internal */
	struct fi_opx_mr **mr_ptr_array = (struct fi_opx_mr **) desc;
	const size_t	   full_count	= count >> 3;
	for (index = 0; index < full_count; index += 8) {
		for (int i = 0; i < 8; ++i) {
			struct fi_opx_mr *mr_ptr;
			if (mr_ptr_array != NULL) {
				mr_ptr = *mr_ptr_array;
				++mr_ptr_array;
			} else {
				mr_ptr = NULL;
			}
			hmem_iface	    = opx_hmem_get_mr_iface(mr_ptr, &hmem_device, &hmem_handle[i]);
			hmem_iovs[i].buf    = (uintptr_t) iov[index + i].iov_base;
			hmem_iovs[i].len    = iov[index + i].iov_len;
			hmem_iovs[i].iface  = hmem_iface;
			hmem_iovs[i].device = hmem_device;
		}
		opx_readv_internal(opx_ep, hmem_iovs, 8, hmem_handle, opx_addr, addr_v, key_v, 0, NULL, NULL, cc,
				   FI_VOID, FI_NOOP, FI_OPX_HFI_DPUT_OPCODE_GET, lock_required, caps, reliability,
				   hfi1_type);
	}

	/* if 'partial_ndesc' is zero, the opx_readv_internal() will fence */
	const size_t partial_ndesc = count & 0x07ull;
	for (int i = 0; i < partial_ndesc; ++i) {
		struct fi_opx_mr *mr_ptr;
		if (mr_ptr_array != NULL) {
			mr_ptr = *mr_ptr_array;
			++mr_ptr_array;
		} else {
			mr_ptr = NULL;
		}
		hmem_iface	    = opx_hmem_get_mr_iface(mr_ptr, &hmem_device, &hmem_handle[i]);
		hmem_iovs[i].buf    = (uintptr_t) iov[index + i].iov_base;
		hmem_iovs[i].len    = iov[index + i].iov_len;
		hmem_iovs[i].iface  = hmem_iface;
		hmem_iovs[i].device = hmem_device;
	}
	opx_readv_internal(opx_ep, hmem_iovs, partial_ndesc, hmem_handle, opx_addr, addr_v, key_v, tx_op_flags,
			   opx_ep->tx->cq, opx_ep->read_cntr, cc, FI_VOID, FI_NOOP, FI_OPX_HFI_DPUT_OPCODE_GET,
			   lock_required, caps, reliability, hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "READV");
	return 0;
}

inline ssize_t fi_opx_readv_generic(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,
				    fi_addr_t src_addr, uint64_t addr_offset, uint64_t key, void *context,
				    int lock_required, const enum fi_av_type av_type, const uint64_t caps,
				    const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READV (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_readv(ep, iov, desc, count, src_addr, addr_offset, key, context, FI_OPX_LOCK_NOT_REQUIRED,
				  av_type, caps, reliability, hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READV (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_readmsg_internal(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags, int lock_required,
				const enum fi_av_type av_type, const uint64_t caps,
				const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "READMSG_INTERNAL");
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	int ret = 0;
	ret	= fi_opx_check_rma(opx_ep);
	if (ret) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READMSG_INTERNAL");
		return ret;
	}
#endif

	if (lock_required) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READMSG_INTERNAL");
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(msg->addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_src_addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, msg->addr);
	fi_opx_get_daos_av_addr_rank(opx_ep, opx_src_addr);

	struct fi_opx_cq   *cq = ((flags & FI_COMPLETION) == FI_COMPLETION) ? opx_ep->tx->cq : NULL;
	struct opx_context *context;
	if (OFI_UNLIKELY(opx_rma_get_context(opx_ep, msg->context, cq, FI_RMA | FI_READ, &context) != FI_SUCCESS)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READMSG_INTERNAL");
		return -FI_ENOMEM;
	}

	/* for fi_read*(), the 'src' is the remote data */
	size_t	     src_iov_index = 0;
	const size_t src_iov_count = msg->rma_iov_count;
	uint64_t     src_iov_bytes = msg->rma_iov[0].len;
	uint64_t     src_iov_addr  = msg->rma_iov[0].addr;
	uint64_t     src_iov_key   = msg->rma_iov[0].key;

	/* for fi_read*(), the 'dst' is the local data */
	size_t	     dst_iov_index = 0;
	const size_t dst_iov_count = msg->iov_count;
	uint64_t     dst_iov_bytes = msg->msg_iov[0].iov_len;
	void	    *dst_iov_vaddr = msg->msg_iov[0].iov_base;

	size_t		       niov;
	struct fi_opx_hmem_iov iov[8];
	uint64_t	       addr[8];
	uint64_t	       key[8];
	uint64_t	       hmem_handle[8];

	ssize_t				  index;
	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	if (OFI_UNLIKELY(cc == NULL)) {
		if (context) {
			OPX_BUF_FREE(context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READMSG_INTERNAL");
		return -FI_ENOMEM;
	}

	memset(cc, 0, sizeof(*cc));
	cc->byte_counter = 0;
	for (index = 0; index < msg->iov_count; index++) {
		cc->byte_counter += msg->msg_iov[index].iov_len;
	}
	cc->initial_byte_count = cc->byte_counter;
#ifndef NDEBUG
	size_t totsize	      = 0;
	size_t totsize_issued = 0;
	for (index = 0; index < msg->rma_iov_count; index++) {
		totsize += msg->rma_iov[index].len;
	}
	assert(totsize == cc->byte_counter);
#endif
	cc->cntr     = opx_ep->read_cntr;
	cc->cq	     = cq;
	cc->context  = context;
	cc->hit_zero = fi_opx_hit_zero;

	struct fi_opx_mr **mr_ptr_array = (struct fi_opx_mr **) msg->desc;
	while (src_iov_index < src_iov_count) {
		for (niov = 0; niov < 8; ++niov) {
			struct fi_opx_mr *mr_ptr;
			if (mr_ptr_array != NULL) {
				mr_ptr = *mr_ptr_array;
				++mr_ptr_array;
			} else {
				mr_ptr = NULL;
			}
			const size_t len = (dst_iov_bytes <= src_iov_bytes) ? dst_iov_bytes : src_iov_bytes;
			opx_hmem_iov_init(dst_iov_vaddr, len, mr_ptr, &iov[niov], &hmem_handle[niov]);
			addr[niov] = src_iov_addr;
			key[niov]  = src_iov_key;

			dst_iov_bytes -= len;
			src_iov_bytes -= len;

			if (src_iov_bytes == 0) {
				/* all done with this src rma iovec */

				if (src_iov_index == (src_iov_count - 1)) {
					/* this is the last src rma iovec .. perform
					 * read with completion processing and return
					 *
					 * the 'dst_iov_bytes' must be zero and it must
					 * be the last dst iovec as well */
					assert(dst_iov_bytes == 0);
					assert(dst_iov_index == (dst_iov_count - 1));
#ifndef NDEBUG
					for (index = 0; index < niov + 1; index++) {
						totsize_issued += iov[index].len;
					}
					assert(totsize_issued <= totsize);
#endif
					opx_readv_internal(opx_ep, iov, niov + 1, hmem_handle, opx_src_addr, addr, key,
							   flags, cq, opx_ep->read_cntr, /* enable_cq, enable_cntr */
							   cc, FI_VOID, FI_NOOP, FI_OPX_HFI_DPUT_OPCODE_GET,
							   lock_required, caps, reliability, hfi1_type);

					OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "READMSG_INTERNAL");
					return 0;

				} else {
					/* advance to next src rma iovec */
					++src_iov_index;
					src_iov_bytes = msg->rma_iov[src_iov_index].len;
					src_iov_addr  = msg->rma_iov[src_iov_index].addr;
					src_iov_key   = msg->rma_iov[src_iov_index].key;
				}
			} else {
				src_iov_addr += len;
			}

			if (dst_iov_bytes == 0) {
				/* all done with this dst iovec */

				if (dst_iov_index == (dst_iov_count - 1)) {
					/* this is the last dst iovec .. do nothing since
					 * the 'src_iov_bytes' must be zero and it must
					 * be the last src rma iovec as well */
					assert(src_iov_bytes == 0);
					assert(src_iov_index == (src_iov_count - 1));

					/* in fact, it should be impossible to get here */
					assert(0);
				} else {
					/* advance to next dst iovec */
					++dst_iov_index;
					dst_iov_bytes = msg->msg_iov[dst_iov_index].iov_len;
					dst_iov_vaddr = msg->msg_iov[dst_iov_index].iov_base;
				}
			} else {
				dst_iov_vaddr = (void *) ((uintptr_t) dst_iov_vaddr + len);
			}

		} /* end for */
#ifndef NDEBUG
		for (index = 0; index < 8; index++) {
			totsize_issued += iov[index].len;
		}
		assert(totsize_issued <= totsize);
#endif
		opx_readv_internal(opx_ep, iov, 8, hmem_handle, opx_src_addr, addr, key, 0, NULL,
				   NULL, /* disable_cq, disable_cntr */
				   cc, FI_VOID, FI_NOOP, FI_OPX_HFI_DPUT_OPCODE_GET, lock_required, caps, reliability,
				   hfi1_type);

	} /* end while */

	/* should never get here */
	OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "READMSG_INTERNAL");
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Fatal -FI_EPERM\n");
	abort();

	return 0;
}

inline ssize_t fi_opx_readmsg_generic(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags,
				      int lock_required, const enum fi_av_type av_type, const uint64_t caps,
				      const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READMSG (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_readmsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, av_type, caps, reliability,
					     hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READMSG (end)\n");

	return rc;
}

static inline ssize_t fi_opx_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,
				      uint64_t addr_offset, uint64_t key, void *context)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READ (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_read_internal(ep, buf, len, desc, src_addr, addr_offset, key, context,
					  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_read_internal(ep, buf, len, desc, src_addr, addr_offset, key, context,
					  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_read_internal(ep, buf, len, desc, src_addr, addr_offset, key, context,
					  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_read_internal(ep, buf, len, desc, src_addr, addr_offset, key, context,
					  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_CYR);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READ (end)\n");
	return rc;
}

static inline ssize_t fi_opx_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READMSG (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_readmsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					     OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_readmsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					     OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_readmsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					     OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_readmsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					     OPX_HFI1_CYR);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA READMSG (end)\n");
	return rc;
}

static inline ssize_t fi_opx_rma_inject_write(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dst_addr,
					      uint64_t addr_offset, uint64_t key)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA INJECT WRITE (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_inject_write_internal(ep, buf, len, dst_addr, addr_offset, key, FI_OPX_LOCK_NOT_REQUIRED,
						  OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_inject_write_internal(ep, buf, len, dst_addr, addr_offset, key, FI_OPX_LOCK_NOT_REQUIRED,
						  OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_inject_write_internal(ep, buf, len, dst_addr, addr_offset, key, FI_OPX_LOCK_NOT_REQUIRED,
						  OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_inject_write_internal(ep, buf, len, dst_addr, addr_offset, key, FI_OPX_LOCK_NOT_REQUIRED,
						  OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_CYR);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA INJECT WRITE (end)\n");
	return rc;
}

static inline ssize_t fi_opx_rma_write(struct fid_ep *ep, const void *buf, size_t len, void *desc, fi_addr_t dst_addr,
				       uint64_t addr_offset, uint64_t key, void *context)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITE (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_write(ep, buf, len, desc, OPX_NO_REMOTE_CQ_DATA, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0, caps, OPX_RELIABILITY, OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_write(ep, buf, len, desc, OPX_NO_REMOTE_CQ_DATA, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0, caps, OPX_RELIABILITY, OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_write(ep, buf, len, desc, OPX_NO_REMOTE_CQ_DATA, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0, caps, OPX_RELIABILITY, OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_write(ep, buf, len, desc, OPX_NO_REMOTE_CQ_DATA, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0, caps, OPX_RELIABILITY, OPX_HFI1_CYR);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITE (end)\n");
	return rc;
}

static inline ssize_t fi_opx_rma_writev(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,
					fi_addr_t dest_addr, uint64_t addr_offset, uint64_t key, void *context)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEV (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_writev_internal(ep, iov, desc, count, dest_addr, addr_offset, key, context,
					    FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_writev_internal(ep, iov, desc, count, dest_addr, addr_offset, key, context,
					    FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_writev_internal(ep, iov, desc, count, dest_addr, addr_offset, key, context,
					    FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_writev_internal(ep, iov, desc, count, dest_addr, addr_offset, key, context,
					    FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY, OPX_HFI1_CYR);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEV (end)\n");
	return rc;
}

static inline ssize_t fi_opx_rma_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA WRITEMSG (begin)\n");
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_writemsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					      OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_writemsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					      OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_writemsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					      OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_writemsg_internal(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, caps, OPX_RELIABILITY,
					      OPX_HFI1_CYR);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEMSG (end)\n");
	return rc;
}

static inline ssize_t fi_opx_rma_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
					   const uint64_t data, fi_addr_t dst_addr, uint64_t addr_offset, uint64_t key,
					   void *context)
{
	struct fi_opx_ep *opx_ep	= container_of(ep, struct fi_opx_ep, ep_fid);
	const int	  lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	const uint64_t	  caps		= opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	ssize_t		  rc;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RMA WRITEDATA (begin)\n");

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		rc = fi_opx_write(ep, buf, len, desc, data, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, FI_REMOTE_CQ_DATA, caps, OPX_RELIABILITY,
				  OPX_HFI1_WFR);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		rc = fi_opx_write(ep, buf, len, desc, data, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, FI_REMOTE_CQ_DATA, caps, OPX_RELIABILITY,
				  OPX_HFI1_JKR_9B);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		rc = fi_opx_write(ep, buf, len, desc, data, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, FI_REMOTE_CQ_DATA, caps, OPX_RELIABILITY,
				  OPX_HFI1_JKR);
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		rc = fi_opx_write(ep, buf, len, desc, data, dst_addr, addr_offset, key, context,
				  FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, FI_REMOTE_CQ_DATA, caps, OPX_RELIABILITY,
				  OPX_HFI1_CYR);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== RMA WRITEDATA (end)\n");
	return rc;
}

static struct fi_ops_rma fi_opx_ops_rma_default = {
	.size	   = sizeof(struct fi_ops_rma),
	.read	   = fi_opx_rma_read,
	.readv	   = fi_no_rma_readv,
	.readmsg   = fi_opx_rma_readmsg,
	.write	   = fi_opx_rma_write,
	.inject	   = fi_opx_rma_inject_write,
	.writev	   = fi_opx_rma_writev,
	.writemsg  = fi_opx_rma_writemsg,
	.writedata = fi_opx_rma_writedata,
};

int fi_opx_init_rma_ops(struct fid_ep *ep, struct fi_info *info)
{
	if (!ep || !info) {
		errno = FI_EINVAL;
		goto err;
	}

	return 0;
err:
	return -errno;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_WFR)
FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_WFR)

FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR_9B)
FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR_9B)

FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR)
FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR)

FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_CYR)
FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_CYR)

#define FI_OPX_RMA_OPS_STRUCT_NAME(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE) \
	FI_OPX_RMA_OPS_STRUCT_NAME_(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE)

#define FI_OPX_RMA_OPS_STRUCT_NAME_(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE) \
	fi_opx_ops_rma_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE

#define FI_OPX_RMA_OPS_STRUCT(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE)                                                \
	static struct fi_ops_rma FI_OPX_RMA_OPS_STRUCT_NAME(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE) = {              \
		.size	   = sizeof(struct fi_ops_rma),                                                              \
		.read	   = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(read, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE),         \
		.readv	   = fi_no_rma_readv,                                                                        \
		.readmsg   = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(readmsg, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE),      \
		.write	   = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(write, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE),        \
		.inject	   = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(inject_write, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE), \
		.writev	   = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(writev, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE),       \
		.writemsg  = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(writemsg, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE),     \
		.writedata = FI_OPX_RMA_SPECIALIZED_FUNC_NAME(writedata, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE),    \
	}

FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_WFR);
FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_WFR);

FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR_9B);
FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR_9B);

FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR);
FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR);

FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_CYR);
FI_OPX_RMA_OPS_STRUCT(FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_CYR);

#pragma GCC diagnostic pop

int fi_opx_enable_rma_ops(struct fid_ep *ep)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (!opx_ep || !opx_ep->domain) {
		errno = FI_EINVAL;
		goto err;
	}

	uint64_t caps = opx_ep->tx->caps;
	if (caps == 0) {
		caps = FI_OPX_BASE_CAPS;
	}
	if (!(caps & FI_RMA)) {
		/* rma ops not enabled on this endpoint */
		return 0;
	}

	const enum fi_threading threading = opx_ep->domain->threading;

	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		opx_ep->ep_fid.rma = &fi_opx_ops_rma_default;
		errno		   = FI_EINVAL;
		goto err;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		if (!lock_required) {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_WFR);
		} else {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_WFR);
		}
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		if (!lock_required) {
			opx_ep->ep_fid.rma =
				&FI_OPX_RMA_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull,
							    OPX_RELIABILITY, OPX_HFI1_JKR_9B);
		} else {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR_9B);
		}
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		if (!lock_required) {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR);
		} else {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_JKR);
		}
	} else {
		assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
		if (!lock_required) {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_NOT_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_CYR);
		} else {
			opx_ep->ep_fid.rma = &FI_OPX_RMA_OPS_STRUCT_NAME(
				FI_OPX_LOCK_REQUIRED, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY, OPX_HFI1_CYR);
		}
	}

	return 0;
err:
	return -errno;
}

int fi_opx_finalize_rma_ops(struct fid_ep *ep)
{
	return 0;
}

ssize_t fi_opx_write_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len, void *desc, fi_addr_t dest_addr,
				   uint64_t addr_offset, uint64_t key, void *context)
{
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(write, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
							OPX_HFI1_WFR)(ep, buf, len, desc, dest_addr, addr_offset, key,
								      context);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(write, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
							OPX_HFI1_JKR_9B)(ep, buf, len, desc, dest_addr, addr_offset,
									 key, context);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(write, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
							OPX_HFI1_JKR)(ep, buf, len, desc, dest_addr, addr_offset, key,
								      context);
	}

	assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
	return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(write, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
						OPX_HFI1_CYR)(ep, buf, len, desc, dest_addr, addr_offset, key, context);
}

ssize_t fi_opx_inject_write_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr,
					  uint64_t addr_offset, uint64_t key)
{
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(inject_write, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
							OPX_RELIABILITY,
							OPX_HFI1_WFR)(ep, buf, len, dest_addr, addr_offset, key);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(inject_write, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
							OPX_RELIABILITY,
							OPX_HFI1_JKR_9B)(ep, buf, len, dest_addr, addr_offset, key);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(inject_write, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
							OPX_RELIABILITY,
							OPX_HFI1_JKR)(ep, buf, len, dest_addr, addr_offset, key);
	}
	assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
	return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(inject_write, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
						OPX_HFI1_CYR)(ep, buf, len, dest_addr, addr_offset, key);
}

ssize_t fi_opx_read_FABRIC_DIRECT(struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,
				  uint64_t addr_offset, uint64_t key, void *context)
{
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(read, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
							OPX_HFI1_WFR)(ep, buf, len, desc, src_addr, addr_offset, key,
								      context);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(read, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
							OPX_HFI1_JKR_9B)(ep, buf, len, desc, src_addr, addr_offset, key,
									 context);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(read, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
							OPX_HFI1_JKR)(ep, buf, len, desc, src_addr, addr_offset, key,
								      context);
	}
	assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
	return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(read, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
						OPX_HFI1_CYR)(ep, buf, len, desc, src_addr, addr_offset, key, context);
}

ssize_t fi_opx_readmsg_FABRIC_DIRECT(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(readmsg, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
							OPX_RELIABILITY, OPX_HFI1_WFR)(ep, msg, flags);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR_9B) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(readmsg, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
							OPX_RELIABILITY, OPX_HFI1_JKR_9B)(ep, msg, flags);
	} else if (OPX_HFI1_TYPE & OPX_HFI1_JKR) {
		return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(readmsg, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
							OPX_RELIABILITY, OPX_HFI1_JKR)(ep, msg, flags);
	}
	assert(OPX_HFI1_TYPE & OPX_HFI1_CYR);
	return FI_OPX_RMA_SPECIALIZED_FUNC_NAME(readmsg, OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY,
						OPX_HFI1_CYR)(ep, msg, flags);
}
