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
#include "rdma/opx/fi_opx_atomic.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_rma.h"
#include "rdma/opx/fi_opx.h"
#include <ofi_enosys.h>
#include <complex.h>


static inline int fi_opx_check_atomic(struct fi_opx_ep *opx_ep, enum fi_datatype dt, enum fi_op op,
				      size_t count)
{
#ifdef DEBUG
	switch ((int)op) {
	case FI_MIN:
	case FI_MAX:
	case FI_SUM:
	case FI_PROD:
	case FI_LOR:
	case FI_LAND:
	case FI_BOR:
	case FI_BAND:
	case FI_LXOR:
	case FI_ATOMIC_READ:
	case FI_ATOMIC_WRITE:
	case FI_CSWAP:
	case FI_CSWAP_NE:
	case FI_CSWAP_LE:
	case FI_CSWAP_LT:
	case FI_CSWAP_GE:
	case FI_CSWAP_GT:
	case FI_MSWAP:
		break;
	default:
		return -FI_EINVAL;
	}
	if (((int)dt >= FI_DATATYPE_LAST) || ((int)dt < 0))
		return -FI_EINVAL;

	if (!opx_ep)
		return -FI_EINVAL;
	if (opx_ep->state != FI_OPX_EP_ENABLED)
		return -FI_EINVAL;

	if (count == 0)
		return -FI_EINVAL;

	const enum fi_av_type av_type = opx_ep->av->av_type;

	if (av_type == FI_AV_UNSPEC)
		return -FI_EINVAL;
	if (av_type == FI_AV_MAP && opx_ep->av_type != FI_AV_MAP)
		return -FI_EINVAL;
	if (av_type == FI_AV_TABLE && opx_ep->av_type != FI_AV_TABLE)
		return -FI_EINVAL;
#endif
	return 0;
}

void fi_opx_atomic_completion_action(union fi_opx_hfi1_deferred_work * work_state)
{
	struct fi_opx_hfi1_dput_params *params = &work_state->dput;
	uint64_t* rbuf_qws = (uint64_t *)((char*)params->opx_mr->buf + params->dput_iov->sbuf);
	const uint64_t *sbuf_qws = (uint64_t*)&work_state->work_elem.payload_copy->byte[sizeof(struct fi_opx_hfi1_dput_fetch)];
	assert(params->op != (FI_NOOP-1));
	assert(params->dt != (FI_VOID-1));
	fi_opx_rx_atomic_dispatch(sbuf_qws, rbuf_qws,
				params->dput_iov->bytes,
				params->dt,
				params->op);
}

__OPX_FORCE_INLINE__
void fi_opx_atomic_op_internal(struct fi_opx_ep *opx_ep,
				const uint32_t opcode,
				const void *buf,
				const size_t len,
				const union fi_opx_addr opx_dst_addr,
				const uint64_t addr_offset,
				const uint64_t key,
				const void *fetch_vaddr,
				const void *compare_vaddr,
				union fi_opx_context *opx_context, const uint64_t tx_op_flags,
				const struct fi_opx_cq *opx_cq,
				const struct fi_opx_cntr *opx_cntr,
				struct fi_opx_completion_counter *cc,
				enum fi_datatype dt, enum fi_op op,
				const int lock_required, const uint64_t caps,
				const enum ofi_reliability_kind reliability)
{
	if (tx_op_flags & FI_INJECT) {
		assert((tx_op_flags & (FI_COMPLETION | FI_TRANSMIT_COMPLETE)) !=
		       (FI_COMPLETION | FI_TRANSMIT_COMPLETE));
		assert((tx_op_flags & (FI_COMPLETION | FI_DELIVERY_COMPLETE)) !=
		       (FI_COMPLETION | FI_DELIVERY_COMPLETE));
	}

	assert(dt == FI_VOID || dt < FI_DATATYPE_LAST);
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work);
	struct fi_opx_hfi1_dput_params *params = &work->dput;

	params->work_elem.slist_entry.next = NULL;
	params->work_elem.work_fn = fi_opx_hfi1_do_dput;
	params->work_elem.completion_action = NULL;
	params->work_elem.payload_copy = NULL;
	params->work_elem.complete = false;
	params->work_elem.low_priority = false;
	params->opx_ep = opx_ep;
	params->lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(opx_dst_addr.fi);
	params->slid = opx_dst_addr.uid.lid;
	params->origin_rs = opx_dst_addr.reliability_rx;
	params->dt = dt == FI_VOID ? FI_VOID-1 : dt;
	params->op = op == FI_NOOP ? FI_NOOP-1 : op;
	params->u8_rx = opx_dst_addr.hfi1_rx; //dest_rx, also used for bth_rx
	params->key = key;
	params->niov = 1;
	params->iov[0].bytes = len;
	params->iov[0].rbuf = addr_offset;
	params->iov[0].sbuf = (uintptr_t) buf;
	params->dput_iov = &params->iov[0];
	params->opcode = opcode;
	params->is_intranode = fi_opx_rma_dput_is_intranode(caps, opx_dst_addr, opx_ep);
	params->reliability = reliability;
	params->cur_iov = 0;
	params->bytes_sent = 0;
	params->cc = NULL;
	params->user_cc = NULL;
	params->opx_mr = NULL;
	params->origin_byte_counter = NULL;
	params->payload_bytes_for_iovec = sizeof(struct fi_opx_hfi1_dput_fetch);
	params->fetch_vaddr = (void *) fetch_vaddr;
	params->compare_vaddr = (void *) compare_vaddr;
	params->target_byte_counter_vaddr = (const uintptr_t) cc;
	params->target_hfi_unit = opx_dst_addr.hfi1_unit;
	params->u32_extended_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, params->is_intranode, opx_dst_addr.hfi1_rx);

	fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);

	fi_opx_hfi1_dput_sdma_init(opx_ep, params, len, 0, NULL);

	int rc = params->work_elem.work_fn(work);
	if(rc == FI_SUCCESS) {
		assert(params->work_elem.complete);
		OPX_BUF_FREE(work);
		return;
	}
	assert(rc == -FI_EAGAIN);
	if (params->work_elem.low_priority) {
		slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending_completion);
		return;
	}

	/* We weren't able to complete the write on the first try. If this was an inject,
	   the outbound buffer(s) may be re-used as soon as we return to the caller, even when
	   this operation will be completed asynchronously. So copy the payload/compare bytes into
	   our own copy of the buffer, and set iov.sbuf/compare addr to point to it. */
	if (tx_op_flags & FI_INJECT) {
		assert(len <= FI_OPX_HFI1_PACKET_IMM);
		if (compare_vaddr) {
			size_t buf_len = len >> 1;
			memcpy(params->inject_data, buf, buf_len);
			memcpy(params->inject_data + buf_len, compare_vaddr, buf_len);
			params->compare_vaddr = (void *) (&params->inject_data[buf_len]);
		} else {
			memcpy(params->inject_data, buf, len);
		}
		params->iov[0].sbuf = (uintptr_t) params->inject_data;
	}

	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending);
}

__OPX_FORCE_INLINE__
size_t fi_opx_atomic_internal(struct fi_opx_ep *opx_ep,
				const void *buf, size_t count,
				const union fi_opx_addr opx_dst_addr,
				uint64_t addr, uint64_t key,
				enum fi_datatype datatype, enum fi_op op,
				void *context, struct fi_opx_completion_counter *cc,
				const unsigned is_fetch, const void *fetch_vaddr,
				const unsigned is_compare, const void *compare_vaddr,
				const uint64_t tx_op_flags, const int lock_required,
				const enum fi_av_type av_type, const uint64_t caps,
				const enum ofi_reliability_kind reliability)
{
	assert((is_fetch == 0) || (is_fetch == 1));
	assert((is_compare == 0) || (is_compare == 1));

	size_t buf_len = count * sizeofdt(datatype);
	if(op == FI_ATOMIC_READ) {
		assert(!is_compare);
		assert(datatype < FI_DATATYPE_LAST);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC READ (begin)\n");
		struct iovec iov = { (void*)fetch_vaddr, buf_len };
		cc->cntr = opx_ep->read_cntr;
		fi_opx_readv_internal(opx_ep, &iov, 1, opx_dst_addr, &addr, &key,
							  (union fi_opx_context *)context, opx_ep->tx->op_flags,
							  opx_ep->rx->cq, opx_ep->read_cntr, cc,
							  datatype, op,
							  FI_OPX_HFI_DPUT_OPCODE_GET,
							  lock_required, caps, reliability);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC READ (end)\n");
		return count;
	}

	if (is_fetch && !is_compare) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC FETCH (begin)\n");
		cc->cntr = opx_ep->read_cntr;
		fi_opx_atomic_op_internal(opx_ep, FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH, buf,
					buf_len, opx_dst_addr, addr, key,
					fetch_vaddr, NULL, (union fi_opx_context *)context,
					opx_ep->tx->op_flags, opx_ep->rx->cq, opx_ep->read_cntr,
					cc, datatype, op, lock_required, caps, reliability);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC FETCH (end)\n");

	} else if (is_fetch && is_compare) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC CAS (begin)\n");
		cc->cntr = opx_ep->read_cntr;
		fi_opx_atomic_op_internal(opx_ep, FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH, buf,
					buf_len << 1, opx_dst_addr, addr, key,
					fetch_vaddr, compare_vaddr, (union fi_opx_context *)context,
					opx_ep->tx->op_flags, opx_ep->rx->cq, opx_ep->read_cntr,
					cc, datatype, op, lock_required, caps, reliability);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC CAS (end)\n");

	} else if(!is_fetch && is_compare) {
		fprintf(stderr, "fi_opx_atomic_internal:  compare without fetch not implemented\n");
		abort();
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC WRITE (begin)\n");
		cc->cntr = opx_ep->write_cntr;
		fi_opx_write_internal(opx_ep, buf, buf_len, opx_dst_addr, addr, key,
					(union fi_opx_context *)NULL, cc, datatype, op, opx_ep->tx->op_flags,
					lock_required, caps, reliability);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					 "===================================== ATOMIC WRITE (end)\n");
	}

	return count;
}


ssize_t fi_opx_atomic_generic(struct fid_ep *ep, const void *buf, size_t count, fi_addr_t dst_addr,
			      uint64_t addr, uint64_t key, enum fi_datatype datatype, enum fi_op op,
			      void *context, const int lock_required, const enum fi_av_type av_type,
			      const uint64_t caps, const enum ofi_reliability_kind reliability)
{
	struct fi_opx_ep *opx_ep;

	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_addr = FI_OPX_EP_AV_ADDR(av_type,opx_ep,dst_addr);

	if (OFI_UNLIKELY(!opx_reliability_ready(ep,
			&opx_ep->reliability->state,
			opx_addr.uid.lid,
			opx_addr.hfi1_rx,
			opx_addr.reliability_rx,
			reliability))) {
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
		return -FI_EAGAIN;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	cc->next = NULL;
	cc->byte_counter = sizeofdt(datatype) * count;
	cc->initial_byte_count = cc->byte_counter;
	cc->cq = (((opx_ep->tx->op_flags & FI_COMPLETION) == FI_COMPLETION) ||
		  ((opx_ep->tx->op_flags & FI_DELIVERY_COMPLETE) == FI_DELIVERY_COMPLETE)) ?
			 opx_ep->rx->cq :
			 NULL;
	cc->context = context;
	cc->hit_zero = fi_opx_hit_zero;

	union fi_opx_context *opx_context = (union fi_opx_context *)cc->context;
	if(opx_context && cc->cq) opx_context->flags = FI_ATOMIC | FI_WRITE;

	size_t xfer __attribute__((unused));
	xfer = fi_opx_atomic_internal(opx_ep, buf, count, opx_addr, addr, key, datatype, op,
				      context, cc, 0, NULL, 0, NULL, opx_ep->tx->op_flags,
				      lock_required, av_type, caps, reliability);
	assert(xfer == count);

	return 0;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_atomic_writemsg_generic(struct fid_ep *ep,
					const struct fi_msg_atomic *msg,
					const uint64_t flags,
					const int lock_required,
					const enum fi_av_type av_type,
					const uint64_t caps,
					const enum ofi_reliability_kind reliability)
{
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_datatype datatype = msg->datatype;
	const enum fi_op op = msg->op;

	int ret = fi_opx_check_atomic(opx_ep, datatype, op, 1);
	if (ret)
		return ret;

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(msg->addr != FI_ADDR_UNSPEC);
	/* This function is NOT called with build constants.  It's called
	   with constant FI_AV_MAP or FI_AV_TABLE based on the endpoint */
	assert((FI_AV_TABLE == av_type) || (FI_AV_MAP == av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type,opx_ep,msg->addr);

	if (OFI_UNLIKELY(!opx_reliability_ready(ep,
			&opx_ep->reliability->state,
			opx_dst_addr.uid.lid,
			opx_dst_addr.hfi1_rx,
			opx_dst_addr.reliability_rx,
			reliability))) {
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
		return -FI_EAGAIN;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	size_t index;
	cc->next = NULL;
	cc->byte_counter = 0;
	for (index = 0; index < msg->iov_count; index++) {
		cc->byte_counter += sizeofdt(datatype) * msg->msg_iov[index].count;
	}
	cc->initial_byte_count = cc->byte_counter;

	cc->cq = ((flags & FI_COMPLETION) == FI_COMPLETION) ? opx_ep->rx->cq : NULL;
	cc->context = msg->context;
	union fi_opx_context *opx_context = (union fi_opx_context *)cc->context;
	if(opx_context && cc->cq) opx_context->flags = FI_ATOMIC | FI_WRITE;

	cc->hit_zero = fi_opx_hit_zero;

	const size_t dtsize = sizeofdt(datatype);

	size_t rma_iov_index = 0;
	const size_t rma_iov_count = msg->rma_iov_count;
	uint64_t rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
	uint64_t rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
	uint64_t rma_iov_key = msg->rma_iov[rma_iov_index].key;

	size_t msg_iov_index = 0;
	const size_t msg_iov_count = msg->iov_count;
	uint64_t msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
	uintptr_t msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;

	while (msg_iov_dtcount != 0 && rma_iov_dtcount != 0) {
		const size_t count_requested = MIN(msg_iov_dtcount, rma_iov_dtcount);

		const size_t count_transfered =
			fi_opx_atomic_internal(opx_ep, (void *)msg_iov_vaddr, count_requested,
					       opx_dst_addr, rma_iov_addr, rma_iov_key, datatype,
					       op, NULL, cc, 0, NULL, 0, NULL, flags, lock_required,
					       av_type, caps, reliability);

		const size_t bytes_transfered = dtsize * count_transfered;

		msg_iov_dtcount -= count_transfered;
		msg_iov_vaddr += bytes_transfered;

		if ((msg_iov_dtcount == 0) && ((msg_iov_index + 1) < msg_iov_count)) {
			++msg_iov_index;
			msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
			msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;
		}

		rma_iov_dtcount -= count_transfered;
		rma_iov_addr += bytes_transfered;

		if ((rma_iov_dtcount == 0) && ((rma_iov_index + 1) < rma_iov_count)) {
			++rma_iov_index;
			rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
			rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
			rma_iov_key = msg->rma_iov[rma_iov_index].key;
		}
	}

	return 0;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_atomic_readwritemsg_generic(struct fid_ep *ep,
					   const struct fi_msg_atomic *msg,
					   struct fi_ioc *resultv,
					   const size_t result_count,
					   const uint64_t flags,
					   const int lock_required,
					   const enum fi_av_type av_type,
					   const uint64_t caps,
					   const enum ofi_reliability_kind reliability)
{
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_datatype datatype = msg->datatype;
	const enum fi_op op = msg->op;

	int ret = fi_opx_check_atomic(opx_ep, datatype, op, 1);
	if (ret)
		return ret;

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(msg->addr != FI_ADDR_UNSPEC);
	/* This function is NOT called with build constants.  It's called
	   with constant FI_AV_MAP or FI_AV_TABLE based on the endpoint */
	assert((FI_AV_TABLE == av_type) || (FI_AV_MAP == av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type,opx_ep,msg->addr);

	if (OFI_UNLIKELY(!opx_reliability_ready(ep,
			&opx_ep->reliability->state,
			opx_dst_addr.uid.lid,
			opx_dst_addr.hfi1_rx,
			opx_dst_addr.reliability_rx,
			reliability))) {
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
		return -FI_EAGAIN;
	}

	const size_t dtsize = sizeofdt(datatype);

	size_t rma_iov_index = 0;
	const size_t rma_iov_count = msg->rma_iov_count;
	uint64_t rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
	uint64_t rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
	uint64_t rma_iov_key = msg->rma_iov[rma_iov_index].key;

	size_t rst_iov_index = 0;
	const size_t rst_iov_count = result_count;
	uint64_t rst_iov_dtcount = resultv[rst_iov_index].count;
	uintptr_t rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	cc->next = NULL;
	cc->byte_counter = 0;
	ssize_t index = 0;
	for (index = 0; index < msg->iov_count; index++) {
		cc->byte_counter += sizeofdt(datatype) * msg->msg_iov[index].count;
	}
	cc->initial_byte_count = cc->byte_counter;
	cc->cq = ((flags & FI_COMPLETION) == FI_COMPLETION) ? opx_ep->rx->cq : NULL;
	cc->context = msg->context;
	union fi_opx_context *opx_context = (union fi_opx_context *)cc->context;
	if(opx_context && cc->cq) opx_context->flags = FI_ATOMIC | FI_READ;


	cc->hit_zero = fi_opx_hit_zero;

	if (op != FI_ATOMIC_READ) { /* likely */

		size_t msg_iov_index = 0;
		const size_t msg_iov_count = msg->iov_count;
		uint64_t msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
		uintptr_t msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;

		size_t count_requested = MIN3(msg_iov_dtcount, rma_iov_dtcount, rst_iov_dtcount);

		while (count_requested > 0) {
			const size_t count_transfered =
				fi_opx_atomic_internal(opx_ep, (void *)msg_iov_vaddr,
						       count_requested, opx_dst_addr, rma_iov_addr,
						       rma_iov_key, datatype, op, NULL, cc, 1,
						       (const void *)rst_iov_vaddr, 0, NULL, flags,
						       lock_required, av_type, caps, reliability);

			const size_t bytes_transfered = dtsize * count_transfered;

			msg_iov_dtcount -= count_transfered;
			msg_iov_vaddr += bytes_transfered;

			if ((msg_iov_dtcount == 0) && ((msg_iov_index + 1) < msg_iov_count)) {
				++msg_iov_index;
				msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
				msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;
			}

			rma_iov_dtcount -= count_transfered;
			rma_iov_addr += bytes_transfered;

			if ((rma_iov_dtcount == 0) && ((rma_iov_index + 1) < rma_iov_count)) {
				++rma_iov_index;
				rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
				rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
				rma_iov_key = msg->rma_iov[rma_iov_index].key;
			}

			rst_iov_dtcount -= count_transfered;
			rst_iov_vaddr += bytes_transfered;

			if ((rst_iov_dtcount == 0) && ((rst_iov_index + 1) < rst_iov_count)) {
				++rst_iov_index;
				rst_iov_dtcount = resultv[rst_iov_index].count;
				rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;
			}

			count_requested = MIN3(msg_iov_dtcount, rma_iov_dtcount, rst_iov_dtcount);
		}

	} else {
		size_t count_requested = MIN(rma_iov_dtcount, rst_iov_dtcount);

		while (rma_iov_dtcount != 0 && rst_iov_dtcount != 0) {
			const size_t count_transfered = fi_opx_atomic_internal(
				opx_ep, NULL, count_requested, opx_dst_addr, rma_iov_addr,
				rma_iov_key, datatype, op, NULL, cc, 1, (const void *)rst_iov_vaddr,
				0, NULL, flags, lock_required, av_type, caps, reliability);

			const size_t bytes_transfered = dtsize * count_transfered;

			rma_iov_dtcount -= count_transfered;
			rma_iov_addr += bytes_transfered;

			if ((rma_iov_dtcount == 0) && ((rma_iov_index + 1) < rma_iov_count)) {
				++rma_iov_index;
				rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
				rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
				rma_iov_key = msg->rma_iov[rma_iov_index].key;
			}

			rst_iov_dtcount -= count_transfered;
			rst_iov_vaddr += bytes_transfered;

			if ((rst_iov_dtcount == 0) && ((rst_iov_index + 1) < rst_iov_count)) {
				++rst_iov_index;
				rst_iov_dtcount = resultv[rst_iov_index].count;
				rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;
			}

			count_requested = MIN(rma_iov_dtcount, rst_iov_dtcount);
		}
	}

	return 0;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_atomic_compwritemsg_generic(struct fid_ep *ep,
					   const struct fi_msg_atomic *msg,
					   const struct fi_ioc *comparev,
					   size_t compare_count,
					   struct fi_ioc *resultv,
					   size_t result_count,
					   uint64_t flags,
					   const int lock_required,
					   const enum fi_av_type av_type,
					   const uint64_t caps,
					   const enum ofi_reliability_kind reliability)
{
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_datatype datatype = msg->datatype;
	const enum fi_op op = msg->op;

	int ret = fi_opx_check_atomic(opx_ep, datatype, op, 1);
	if (ret)
		return ret;

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert(msg->addr != FI_ADDR_UNSPEC);
	/* This function is NOT called with build constants.  It's called
	   with constant FI_AV_MAP or FI_AV_TABLE based on the endpoint */
	assert((FI_AV_TABLE == av_type) || (FI_AV_MAP == av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type,opx_ep,msg->addr);

	if (OFI_UNLIKELY(!opx_reliability_ready(ep,
			&opx_ep->reliability->state,
			opx_dst_addr.uid.lid,
			opx_dst_addr.hfi1_rx,
			opx_dst_addr.reliability_rx,
			reliability))) {
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
		return -FI_EAGAIN;
	}

	const size_t dtsize = sizeofdt(datatype);

	size_t rma_iov_index = 0;
	const size_t rma_iov_count = msg->rma_iov_count;
	uint64_t rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
	uint64_t rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
	uint64_t rma_iov_key = msg->rma_iov[rma_iov_index].key;

	size_t msg_iov_index = 0;
	const size_t msg_iov_count = msg->iov_count;
	uint64_t msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
	uintptr_t msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;

	size_t rst_iov_index = 0;
	const size_t rst_iov_count = result_count;
	uint64_t rst_iov_dtcount = resultv[rst_iov_index].count;
	uintptr_t rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;

	size_t cmp_iov_index = 0;
	const size_t cmp_iov_count = compare_count;
	uint64_t cmp_iov_dtcount = comparev[cmp_iov_index].count;
	uintptr_t cmp_iov_vaddr = (uintptr_t)comparev[cmp_iov_index].addr;

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	cc->next = NULL;
	cc->byte_counter = 0;
	ssize_t index;
	for (index = 0; index < msg->iov_count; index++) {
		cc->byte_counter += sizeofdt(datatype)* msg->msg_iov[index].count;
	}
	cc->initial_byte_count = cc->byte_counter;
	cc->cq = ((flags & FI_COMPLETION) == FI_COMPLETION) ? opx_ep->rx->cq : NULL;
	cc->context = msg->context;
	union fi_opx_context *opx_context = (union fi_opx_context *)cc->context;
	if(opx_context && cc->cq) opx_context->flags = FI_ATOMIC | FI_READ;

	cc->hit_zero = fi_opx_hit_zero;

	while (msg_iov_dtcount != 0 && rma_iov_dtcount != 0 && rst_iov_dtcount != 0 &&
	       cmp_iov_dtcount != 0) {
		const size_t count_requested =
			MIN4(msg_iov_dtcount, rma_iov_dtcount, rst_iov_dtcount, cmp_iov_dtcount);

		const size_t count_transfered =
			fi_opx_atomic_internal(opx_ep, (void *)msg_iov_vaddr, count_requested,
					       opx_dst_addr, rma_iov_addr, rma_iov_key, datatype,
					       op, NULL, cc, 1, (const void *)rst_iov_vaddr, 1,
					       (const void *)cmp_iov_vaddr, flags, lock_required,
					       av_type, caps, reliability);

		const size_t bytes_transfered = dtsize * count_transfered;

		msg_iov_dtcount -= count_transfered;
		msg_iov_vaddr += bytes_transfered;

		if ((msg_iov_dtcount == 0) && ((msg_iov_index + 1) < msg_iov_count)) {
			++msg_iov_index;
			msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
			msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;
		}

		rma_iov_dtcount -= count_transfered;
		rma_iov_addr += bytes_transfered;

		if ((rma_iov_dtcount == 0) && ((rma_iov_index + 1) < rma_iov_count)) {
			++rma_iov_index;
			rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
			rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
			rma_iov_key = msg->rma_iov[rma_iov_index].key;
		}

		rst_iov_dtcount -= count_transfered;
		rst_iov_vaddr += bytes_transfered;

		if ((rst_iov_dtcount == 0) && ((rst_iov_index + 1) < rst_iov_count)) {
			++rst_iov_index;
			rst_iov_dtcount = resultv[rst_iov_index].count;
			rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;
		}

		cmp_iov_dtcount -= count_transfered;
		cmp_iov_vaddr += bytes_transfered;

		if ((cmp_iov_dtcount == 0) && ((cmp_iov_index + 1) < cmp_iov_count)) {
			++cmp_iov_index;
			cmp_iov_dtcount = comparev[cmp_iov_index].count;
			cmp_iov_vaddr = (uintptr_t)comparev[cmp_iov_index].addr;
		}
	}

	return 0;
}

/*
 * Generic function to handle both fetching (1 operand) and compare
 * (2 operand) atomics.
 */

__OPX_FORCE_INLINE__
ssize_t fi_opx_fetch_compare_atomic_generic(
	struct fid_ep *ep, const void *buf, size_t count, void *desc, const void *compare,
	void *compare_desc, void *result, void *result_desc, fi_addr_t dest_addr, uint64_t addr,
	uint64_t key, enum fi_datatype datatype, enum fi_op op, void *context, int lock_required,
	const enum fi_av_type av_type, const uint64_t caps,
	const enum ofi_reliability_kind reliability)
{
	struct fi_opx_ep *opx_ep;

	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_addr = FI_OPX_EP_AV_ADDR(av_type,opx_ep,dest_addr);

	if (OFI_UNLIKELY(!opx_reliability_ready(ep,
			&opx_ep->reliability->state,
			opx_addr.uid.lid,
			opx_addr.hfi1_rx,
			opx_addr.reliability_rx,
			reliability))) {
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
		return -FI_EAGAIN;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	cc->next = NULL;
	cc->byte_counter = sizeofdt(datatype) * count;
	cc->initial_byte_count = cc->byte_counter;
	cc->cq = (((opx_ep->tx->op_flags & FI_COMPLETION) == FI_COMPLETION) ||
		  ((opx_ep->tx->op_flags & FI_DELIVERY_COMPLETE) == FI_DELIVERY_COMPLETE)) ?
			 opx_ep->rx->cq :
			 NULL;
	cc->context = context;
	cc->hit_zero = fi_opx_hit_zero;

	union fi_opx_context *opx_context = (union fi_opx_context *)cc->context;
	if(opx_context && cc->cq) opx_context->flags = FI_ATOMIC | FI_WRITE;

	size_t xfer __attribute__((unused));
	xfer = fi_opx_atomic_internal(opx_ep, buf, count, opx_addr, addr, key, datatype, op,
				      context, cc, 1, result, compare!=NULL, compare, opx_ep->tx->op_flags,
				      lock_required, av_type, caps, reliability);
	assert(xfer == count);

	return 0;
}
ssize_t fi_opx_fetch_atomic_generic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
				    void *result, void *result_desc, fi_addr_t dest_addr,
				    uint64_t addr, uint64_t key, enum fi_datatype datatype,
				    enum fi_op op, void *context, const int lock_required,
				    const enum fi_av_type av_type, const uint64_t caps,
				    const enum ofi_reliability_kind reliability)
{
	return fi_opx_fetch_compare_atomic_generic(ep, buf, count, desc, NULL, NULL, result,
						   result_desc, dest_addr, addr, key, datatype, op,
						   context, lock_required, av_type, caps,
						   reliability);
}

ssize_t fi_opx_compare_atomic_generic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
				      const void *compare, void *compare_desc, void *result,
				      void *result_desc, fi_addr_t dest_addr, uint64_t addr,
				      uint64_t key, enum fi_datatype datatype, enum fi_op op,
				      void *context, const int lock_required,
				      const enum fi_av_type av_type, const uint64_t caps,
				      const enum ofi_reliability_kind reliability)
{
	return fi_opx_fetch_compare_atomic_generic(ep, buf, count, desc, compare, compare_desc,
						   result, result_desc, dest_addr, addr, key,
						   datatype, op, context, lock_required, av_type,
						   caps, reliability);
}

ssize_t fi_opx_inject_atomic_generic(struct fid_ep *ep, const void *buf, size_t count,
				     fi_addr_t dest_addr, uint64_t addr, uint64_t key,
				     enum fi_datatype datatype, enum fi_op op,
				     const int lock_required, const enum fi_av_type av_type,
				     const uint64_t caps,
				     const enum ofi_reliability_kind reliability)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	int ret = fi_opx_check_atomic(opx_ep, datatype, op, count);
	if (ret)
		return ret;

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}
	assert(dest_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr opx_dst_addr = FI_OPX_EP_AV_ADDR(av_type,opx_ep,dest_addr);

	if (OFI_UNLIKELY(!opx_reliability_ready(ep,
			&opx_ep->reliability->state,
			opx_dst_addr.uid.lid,
			opx_dst_addr.hfi1_rx,
			opx_dst_addr.reliability_rx,
			reliability))) {
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
		return -FI_EAGAIN;
	}

	struct fi_opx_completion_counter *cc = ofi_buf_alloc(opx_ep->rma_counter_pool);
	cc->next = NULL;
	cc->byte_counter = sizeofdt(datatype) * count;
	cc->initial_byte_count = cc->byte_counter;
	cc->cq = NULL;
	cc->context = NULL;
	cc->hit_zero = fi_opx_hit_zero;
	cc->cntr = opx_ep->write_cntr;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "===================================== ATOMIC INJECT WRITE (begin)\n");

	fi_opx_write_internal(opx_ep, buf, count*sizeofdt(datatype),
				opx_dst_addr, addr, key, NULL, cc, datatype,
				op, opx_ep->tx->op_flags | FI_INJECT,
				lock_required, caps, reliability);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== ATOMIC INJECT WRITE (end)\n");
	return 0;
}

ssize_t fi_opx_atomic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
		      fi_addr_t dst_addr, uint64_t addr, uint64_t key, enum fi_datatype datatype,
		      enum fi_op op, void *context)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	rc = fi_opx_atomic_generic(ep, buf, count, dst_addr, addr, key, datatype, op,
				     context, FI_OPX_LOCK_NOT_REQUIRED,
				     opx_ep->av_type, 0x0018000000000000ull,
				     OPX_RELIABILITY);

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

ssize_t fi_opx_fetch_atomic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
			    void *result, void *result_desc, fi_addr_t dest_addr, uint64_t addr,
			    uint64_t key, enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	if (opx_ep->av_type == FI_AV_MAP) {
		rc = fi_opx_fetch_atomic_generic(
			ep, buf, count, desc, result, result_desc, dest_addr, addr, key,
			datatype, op, context, FI_OPX_LOCK_NOT_REQUIRED,
			FI_AV_MAP, 0x0018000000000000ull, OPX_RELIABILITY);
	} else {
		rc = fi_opx_fetch_atomic_generic(
			ep, buf, count, desc, result, result_desc, dest_addr, addr, key,
			datatype, op, context, FI_OPX_LOCK_NOT_REQUIRED,
			FI_AV_TABLE, 0x0018000000000000ull, OPX_RELIABILITY);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

ssize_t fi_opx_compare_atomic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
			      const void *compare, void *compare_desc, void *result,
			      void *result_desc, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	if (opx_ep->av_type == FI_AV_MAP) {
		rc = fi_opx_compare_atomic_generic(
			ep, buf, count, desc, compare, compare_desc, result, result_desc,
			dest_addr, addr, key, datatype, op, context, FI_OPX_LOCK_NOT_REQUIRED,
			FI_AV_MAP, 0x0018000000000000ull, OPX_RELIABILITY);
	} else {
		rc = fi_opx_compare_atomic_generic(
			ep, buf, count, desc, compare, compare_desc, result, result_desc,
			dest_addr, addr, key, datatype, op, context, FI_OPX_LOCK_NOT_REQUIRED,
			FI_AV_TABLE, 0x0018000000000000ull, OPX_RELIABILITY);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

ssize_t fi_opx_inject_atomic(struct fid_ep *ep, const void *buf, size_t count, fi_addr_t dest_addr,
			     uint64_t addr, uint64_t key, enum fi_datatype datatype, enum fi_op op)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	if (opx_ep->av_type == FI_AV_MAP) {
		rc = fi_opx_inject_atomic_generic(ep, buf, count, dest_addr, addr, key,
						  datatype, op, FI_OPX_LOCK_NOT_REQUIRED,
						  FI_AV_MAP, 0x0018000000000000ull,
						  OPX_RELIABILITY);
	} else {
		rc = fi_opx_inject_atomic_generic(ep, buf, count, dest_addr, addr, key,
						  datatype, op, FI_OPX_LOCK_NOT_REQUIRED,
						  FI_AV_TABLE, 0x0018000000000000ull,
						  OPX_RELIABILITY);
	}
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	return rc;
}

ssize_t fi_opx_atomicv(struct fid_ep *ep, const struct fi_ioc *iov, void **desc, size_t count,
		       uint64_t addr, uint64_t key, enum fi_datatype datatype, enum fi_op op,
		       void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

ssize_t fi_opx_atomic_writemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	if (opx_ep->av_type == FI_AV_MAP) {
		rc = fi_opx_atomic_writemsg_generic(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED,
						    FI_AV_MAP,
						    0x0018000000000000ull,
						    OPX_RELIABILITY);
	} else {
		rc = fi_opx_atomic_writemsg_generic(ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED,
						    FI_AV_TABLE,
						    0x0018000000000000ull,
						    OPX_RELIABILITY);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	return rc;
}

ssize_t fi_opx_atomic_readwritemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
				   struct fi_ioc *resultv, void **result_desc, size_t result_count,
				   uint64_t flags)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	if (opx_ep->av_type == FI_AV_MAP) {
		rc = fi_opx_atomic_readwritemsg_generic(ep, msg, resultv, result_count,
							flags, FI_OPX_LOCK_NOT_REQUIRED,
							FI_AV_MAP,
							0x0018000000000000ull,
							OPX_RELIABILITY);
	} else {
		rc = fi_opx_atomic_readwritemsg_generic(ep, msg, resultv, result_count,
							flags, FI_OPX_LOCK_NOT_REQUIRED,
							FI_AV_TABLE,
							0x0018000000000000ull,
							OPX_RELIABILITY);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	return rc;
}

ssize_t fi_opx_atomic_compwritemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
				   const struct fi_ioc *comparev, void **compare_desc,
				   size_t compare_count, struct fi_ioc *resultv, void **result_desc,
				   size_t result_count, uint64_t flags)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const enum fi_threading threading = opx_ep->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	ssize_t rc;
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	if (opx_ep->av_type == FI_AV_MAP) {
		rc = fi_opx_atomic_compwritemsg_generic(ep, msg, comparev, compare_count,
							resultv, result_count, flags,
							FI_OPX_LOCK_NOT_REQUIRED,
							FI_AV_MAP, 0x0018000000000000ull,
							OPX_RELIABILITY);
	} else {
		rc = fi_opx_atomic_compwritemsg_generic(ep, msg, comparev, compare_count,
							resultv, result_count, flags,
							FI_OPX_LOCK_NOT_REQUIRED,
							FI_AV_TABLE, 0x0018000000000000ull,
							OPX_RELIABILITY);
	}

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
	return rc;
}

int fi_opx_atomic_writevalid(struct fid_ep *ep, enum fi_datatype datatype, enum fi_op op,
			     size_t *count)
{
	static size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t), /* FI_INT8 */
		sizeof(uint8_t), /* FI_UINT8 */
		sizeof(int16_t), /* FI_INT16 */
		sizeof(uint16_t), /* FI_UINT16 */
		sizeof(int32_t), /* FI_INT32 */
		sizeof(uint32_t), /* FI_UINT32 */
		sizeof(int64_t), /* FI_INT64 */
		sizeof(uint64_t), /* FI_UINT64 */
		sizeof(float), /* FI_FLOAT */
		sizeof(double), /* FI_DOUBLE */
		sizeof(complex float), /* FI_FLOAT_COMPLEX */
		sizeof(complex double), /* FI_DOUBLE_COMPLEX */
		sizeof(long double), /* FI_LONG_DOUBLE */
		sizeof(complex long double) /* FI_LONG_DOUBLE_COMPLEX */
	};

	if ((op > FI_ATOMIC_WRITE) || (datatype >= FI_DATATYPE_LAST)) {
		*count = 0;
		errno = FI_EOPNOTSUPP;
		return -errno;
	}

	// *count = sizeof(union fi_opx_hfi1_packet_payload) / sizeofdt[datatype];
	*count = UINT64_MAX / sizeofdt[datatype];
	return 0;
}

int fi_opx_atomic_readwritevalid(struct fid_ep *ep, enum fi_datatype datatype, enum fi_op op,
				 size_t *count)
{
	static size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t), /* FI_INT8 */
		sizeof(uint8_t), /* FI_UINT8 */
		sizeof(int16_t), /* FI_INT16 */
		sizeof(uint16_t), /* FI_UINT16 */
		sizeof(int32_t), /* FI_INT32 */
		sizeof(uint32_t), /* FI_UINT32 */
		sizeof(int64_t), /* FI_INT64 */
		sizeof(uint64_t), /* FI_UINT64 */
		sizeof(float), /* FI_FLOAT */
		sizeof(double), /* FI_DOUBLE */
		sizeof(complex float), /* FI_FLOAT_COMPLEX */
		sizeof(complex double), /* FI_DOUBLE_COMPLEX */
		sizeof(long double), /* FI_LONG_DOUBLE */
		sizeof(complex long double) /* FI_LONG_DOUBLE_COMPLEX */
	};

	if ((op > FI_ATOMIC_WRITE) || (datatype >= FI_DATATYPE_LAST)) {
		*count = 0;
		errno = FI_EOPNOTSUPP;
		return -errno;
	}

	//*count = (sizeof(union fi_opx_hfi1_packet_payload) -
	//	  sizeof(struct fi_opx_hfi1_fetch_metadata)) /
	//	 sizeofdt[datatype];
	*count = UINT64_MAX /sizeofdt[datatype];
	return 0;
}

int fi_opx_atomic_compwritevalid(struct fid_ep *ep, enum fi_datatype datatype, enum fi_op op,
				 size_t *count)
{
	static size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t), /* FI_INT8 */
		sizeof(uint8_t), /* FI_UINT8 */
		sizeof(int16_t), /* FI_INT16 */
		sizeof(uint16_t), /* FI_UINT16 */
		sizeof(int32_t), /* FI_INT32 */
		sizeof(uint32_t), /* FI_UINT32 */
		sizeof(int64_t), /* FI_INT64 */
		sizeof(uint64_t), /* FI_UINT64 */
		sizeof(float), /* FI_FLOAT */
		sizeof(double), /* FI_DOUBLE */
		sizeof(complex float), /* FI_FLOAT_COMPLEX */
		sizeof(complex double), /* FI_DOUBLE_COMPLEX */
		sizeof(long double), /* FI_LONG_DOUBLE */
		sizeof(complex long double) /* FI_LONG_DOUBLE_COMPLEX */
	};

	if ((op < FI_CSWAP) || (op >= FI_ATOMIC_OP_LAST) || (datatype >= FI_DATATYPE_LAST)) {
		*count = 0;
		errno = FI_EOPNOTSUPP;
		return -errno;
	}

	// *count = (sizeof(union fi_opx_hfi1_packet_payload) / 2) / sizeofdt[datatype];
	*count = (UINT64_MAX / 2) / sizeofdt[datatype];
	return 0;
}

static struct fi_ops_atomic fi_opx_ops_atomic = { .size = sizeof(struct fi_ops_atomic),
						  .write = fi_opx_atomic,
						  .writev = fi_no_atomic_writev,
						  .writemsg = fi_opx_atomic_writemsg,
						  .inject = fi_opx_inject_atomic,
						  .readwrite = fi_opx_fetch_atomic,
						  .readwritev = fi_no_atomic_readwritev,
						  .readwritemsg = fi_opx_atomic_readwritemsg,
						  .compwrite = fi_opx_compare_atomic,
						  .compwritev = fi_no_atomic_compwritev,
						  .compwritemsg = fi_opx_atomic_compwritemsg,
						  .writevalid = fi_opx_atomic_writevalid,
						  .readwritevalid = fi_opx_atomic_readwritevalid,
						  .compwritevalid = fi_opx_atomic_compwritevalid };

int fi_opx_init_atomic_ops(struct fid_ep *ep, struct fi_info *info)
{
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (!info || !opx_ep)
		goto err;

	if (info->caps & FI_ATOMICS || (info->tx_attr && (info->tx_attr->caps & FI_ATOMICS))) {
		opx_ep->ep_fid.atomic = &fi_opx_ops_atomic;
	}
	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_opx_enable_atomic_ops(struct fid_ep *ep)
{
	struct fi_opx_ep *opx_ep;
	opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (!opx_ep || !opx_ep->domain)
		goto err;

	if (!opx_ep->ep_fid.atomic) {
		/* atomic ops not enabled on this endpoint */
		return 0;
	}
	/* fill in atomic formats */

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
	return 0;
}

int fi_opx_finalize_atomic_ops(struct fid_ep *ep)
{
	return 0;
}

FI_OPX_ATOMIC_SPECIALIZED_FUNC(OPX_LOCK, OPX_AV, 0x0018000000000000ull, OPX_RELIABILITY)

ssize_t fi_opx_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t count, void *desc,
				    fi_addr_t dest_addr, uint64_t addr, uint64_t key,
				    enum fi_datatype datatype, enum fi_op op, void *context)
{
	return FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME(atomic, OPX_LOCK, OPX_AV, 0x0018000000000000ull,
						   OPX_RELIABILITY)(
		ep, buf, count, desc, dest_addr, addr, key, datatype, op, context);
}

ssize_t fi_opx_inject_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t count,
					   fi_addr_t dest_addr, uint64_t addr, uint64_t key,
					   enum fi_datatype datatype, enum fi_op op)
{
	return FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME(inject_atomic, OPX_LOCK, OPX_AV,
						   0x0018000000000000ull, OPX_RELIABILITY)(
		ep, buf, count, dest_addr, addr, key, datatype, op);
}

ssize_t fi_opx_fetch_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t count,
					  void *desc, void *result, void *result_desc,
					  fi_addr_t dest_addr, uint64_t addr, uint64_t key,
					  enum fi_datatype datatype, enum fi_op op, void *context)
{
	return FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME(fetch_atomic, OPX_LOCK, OPX_AV,
						   0x0018000000000000ull,
						   OPX_RELIABILITY)(ep, buf, count, desc, result,
								    result_desc, dest_addr, addr,
								    key, datatype, op, context);
}

ssize_t fi_opx_compare_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t count,
					    void *desc, const void *compare, void *compare_desc,
					    void *result, void *result_desc, fi_addr_t dest_addr,
					    uint64_t addr, uint64_t key, enum fi_datatype datatype,
					    enum fi_op op, void *context)
{
	return FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME(compare_atomic, OPX_LOCK, OPX_AV,
						   0x0018000000000000ull, OPX_RELIABILITY)(
		ep, buf, count, desc, compare, compare_desc, result, result_desc, dest_addr, addr,
		key, datatype, op, context);
}
