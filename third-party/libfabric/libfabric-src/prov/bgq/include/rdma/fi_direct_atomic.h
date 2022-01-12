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
#ifndef _FI_BGQ_DIRECT_ATOMIC_H_
#define _FI_BGQ_DIRECT_ATOMIC_H_

#define FABRIC_DIRECT_ATOMIC 1

#include "rdma/bgq/fi_bgq_compiler.h"
#include "rdma/bgq/fi_bgq_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <complex.h>
#if 0

#define FI_BGQ_DATATYPES		\
	sizeof(int8_t),			\
	sizeof(uint8_t),		\
	sizeof(int16_t),		\
	sizeof(uint16_t),		\
	sizeof(int32_t),		\
	sizeof(uint32_t),		\
	sizeof(int64_t),		\
	sizeof(uint64_t),		\
	sizeof(float),			\
	sizeof(double),			\
	sizeof(float complex),		\
	sizeof(double complex),		\
	sizeof(long double),		\
	sizeof(long double complex),

#ifdef __cplusplus
struct __fi_bgq_datatype{
	static const size_t size(int index){
		static size_t __fi_bgq_datatype_size[] =
		{
			FI_BGQ_DATATYPES
		};
		return __fi_bgq_datatype_size[index];
	}
};
#else
static size_t __fi_bgq_datatype_size[] =
{
	FI_BGQ_DATATYPES
};
#endif

/*
 * Warning: bogus datatype will result in out of bounds array access.
 * Use with caution.
 */
static inline size_t fi_bgq_datatype_size_unsafe(enum fi_datatype dt)
{
#ifdef __cplusplus
        return __fi_bgq_datatype::size(dt);
#else
	return __fi_bgq_datatype_size[dt];
#endif
}

static inline size_t fi_bgq_datatype_size(enum fi_datatype dt)
{
	return
		(((int)dt) < 0 || dt >= FI_DATATYPE_LAST)
			? 0
			: fi_bgq_datatype_size_unsafe(dt);
}
#endif



static inline int fi_bgq_check_atomic(struct fi_bgq_ep *bgq_ep,
		enum fi_av_type av_type, enum fi_datatype dt, enum fi_op op,
		size_t count)
{
#ifdef DEBUG
	switch((int)op) {
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
	if (((int) dt >= FI_DATATYPE_LAST) || ((int) dt < 0))
		return -FI_EINVAL;

	if (!bgq_ep)
		return -FI_EINVAL;
	if (bgq_ep->state != FI_BGQ_EP_ENABLED)
		return -FI_EINVAL;

	if (count == 0)
		return -FI_EINVAL;

	if (av_type == FI_AV_UNSPEC)
		return -FI_EINVAL;
	if (av_type == FI_AV_MAP && bgq_ep->av_type != FI_AV_MAP)
		return -FI_EINVAL;
	if (av_type == FI_AV_TABLE && bgq_ep->av_type != FI_AV_TABLE)
		return -FI_EINVAL;
#endif
	return 0;
}

static inline size_t sizeofdt(const enum fi_datatype datatype) {

	static const size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t),			/* FI_INT8 */
		sizeof(uint8_t),		/* FI_UINT8 */
		sizeof(int16_t),		/* FI_INT16 */
		sizeof(uint16_t),		/* FI_UINT16 */
		sizeof(int32_t),		/* FI_INT32 */
		sizeof(uint32_t),		/* FI_UINT32 */
		sizeof(int64_t),		/* FI_INT64 */
		sizeof(uint64_t),		/* FI_UINT64 */
		sizeof(float),			/* FI_FLOAT */
		sizeof(double),			/* FI_DOUBLE */
		sizeof(complex float),		/* FI_FLOAT_COMPLEX */
		sizeof(complex double),		/* FI_DOUBLE_COMPLEX */
		sizeof(long double),		/* FI_LONG_DOUBLE */
		sizeof(complex long double)	/* FI_LONG_DOUBLE_COMPLEX */
	};

	return sizeofdt[datatype];
}

static inline size_t maxcount (const enum fi_datatype datatype,
		const unsigned is_compare,
		const unsigned is_fetch) {

#define INIT_MAXCOUNT_ARRAY(maxbytes)			\
	maxbytes / sizeof(int8_t),		/* FI_INT8 */		\
	maxbytes / sizeof(uint8_t),		/* FI_UINT8 */		\
	maxbytes / sizeof(int16_t),		/* FI_INT16 */		\
	maxbytes / sizeof(uint16_t),		/* FI_UINT16 */		\
	maxbytes / sizeof(int32_t),		/* FI_INT32 */		\
	maxbytes / sizeof(uint32_t),		/* FI_UINT32 */		\
	maxbytes / sizeof(int64_t),		/* FI_INT64 */		\
	maxbytes / sizeof(uint64_t),		/* FI_UINT64 */		\
	maxbytes / sizeof(float),		/* FI_FLOAT */		\
	maxbytes / sizeof(double),		/* FI_DOUBLE */		\
	maxbytes / sizeof(complex float),	/* FI_FLOAT_COMPLEX */	\
	maxbytes / sizeof(complex double),	/* FI_DOUBLE_COMPLEX */	\
	maxbytes / sizeof(long double),		/* FI_LONG_DOUBLE */	\
	maxbytes / sizeof(complex long double)	/* FI_LONG_DOUBLE_COMPLEX */

	static const size_t maxcount[2][2][FI_DATATYPE_LAST] = {
		{
			{	/* !compare, !fetch */
				INIT_MAXCOUNT_ARRAY(512)
			},
			{	/* !compare, fetch */
				INIT_MAXCOUNT_ARRAY((512-sizeof(struct fi_bgq_mu_fetch_metadata)))
			}
		},
		{
			{	/* compare, !fetch */
				INIT_MAXCOUNT_ARRAY(256)
			},
			{	/* compare, fetch */
				INIT_MAXCOUNT_ARRAY((256-sizeof(struct fi_bgq_mu_fetch_metadata)))
			}
		}
	};

#undef INIT_MAXCOUNT_ARRAY

	return maxcount[is_compare][is_fetch][datatype];
}

static inline void fi_bgq_atomic_fence (struct fi_bgq_ep * bgq_ep,
		const uint64_t tx_op_flags,
		const union fi_bgq_addr * bgq_dst_addr,
		union fi_bgq_context * bgq_context,
		const int lock_required)
{
	const uint64_t do_cq = ((tx_op_flags & FI_COMPLETION) == FI_COMPLETION);

	struct fi_bgq_cntr * write_cntr = bgq_ep->write_cntr;
	const uint64_t do_cntr = (write_cntr != 0);

	assert(do_cq || do_cntr);

		MUHWI_Descriptor_t * model = &bgq_ep->tx.atomic.emulation.fence.mfifo_model;

		MUHWI_Descriptor_t * desc =
			fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

		qpx_memcpy64((void*)desc, (const void*)model);

		/* set the destination torus address and fifo map */
		desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr->uid.fi);

		const uint64_t fifo_map = (uint64_t) fi_bgq_addr_get_fifo_map(bgq_dst_addr->fi);
		desc->Torus_FIFO_Map = fifo_map;

		desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
			fi_bgq_addr_rec_fifo_id(bgq_dst_addr->fi);

		/* locate the payload lookaside slot */
		void * payload =
			fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
				desc, &desc->Pa_Payload);

		if (do_cntr && !do_cq) {	/* likely */

			/* increment the origin fi_cntr value */

			/* copy the 'fi_atomic' counter completion descriptor
			 * model into the payload lookaside slot */
			model = &bgq_ep->tx.atomic.emulation.fence.cntr_model;
			MUHWI_Descriptor_t * cntr_desc = (MUHWI_Descriptor_t *) payload;
			qpx_memcpy64((void*)cntr_desc, (const void*)model);

			cntr_desc->Torus_FIFO_Map = fifo_map;

			MUSPI_SetRecPayloadBaseAddressInfo(cntr_desc, write_cntr->std.batid,
				MUSPI_GetAtomicAddress(0, MUHWI_ATOMIC_OPCODE_STORE_ADD));	/* TODO - init */

		} else if (do_cq) {

			/* add the cq byte counter decrement direct-put
			 * descriptor to the tail of the rget/mfifo payload */

			/* initialize the completion entry */
			assert(bgq_context);
			assert(((uintptr_t)bgq_context & 0x07ull) == 0);	/* must be 8 byte aligned */
			bgq_context->flags = FI_RMA | FI_READ;
			bgq_context->len = 0;
			bgq_context->buf = NULL;
			bgq_context->byte_counter = 1;
			bgq_context->tag = 0;

			uint64_t byte_counter_paddr = 0;
			uint32_t cnk_rc __attribute__ ((unused));
			cnk_rc = fi_bgq_cnk_vaddr2paddr((void*)&bgq_context->byte_counter,
					sizeof(uint64_t), &byte_counter_paddr);
			assert(cnk_rc == 0);

			/* copy the 'fi_atomic' cq completion descriptor
			 * model into the payload lookaside slot */
			model = &bgq_ep->tx.atomic.emulation.fence.cq_model;
			MUHWI_Descriptor_t * cq_desc = (MUHWI_Descriptor_t *) payload;
			qpx_memcpy64((void*)cq_desc, (const void*)model);

			cq_desc->Torus_FIFO_Map = fifo_map;

			MUSPI_SetRecPayloadBaseAddressInfo(cq_desc,
				FI_BGQ_MU_BAT_ID_GLOBAL, byte_counter_paddr);

			fi_bgq_cq_enqueue_pending(bgq_ep->send_cq, bgq_context, lock_required);

			if (do_cntr) {

				/* increment the origin fi_cntr value */

				/* copy the 'fi_atomic' counter completion descriptor
				 * model into the payload lookaside slot */
				model = &bgq_ep->tx.atomic.emulation.fence.cntr_model;
				MUHWI_Descriptor_t * cntr_desc = &(((MUHWI_Descriptor_t *) payload)[1]);
				qpx_memcpy64((void*)cntr_desc, (const void*)model);

				cntr_desc->Torus_FIFO_Map = fifo_map;

				MUSPI_SetRecPayloadBaseAddressInfo(cntr_desc, write_cntr->std.batid,
					MUSPI_GetAtomicAddress(0, MUHWI_ATOMIC_OPCODE_STORE_ADD));	/* TODO - init */

				desc->Message_Length += sizeof(MUHWI_Descriptor_t);
				union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
				hdr->rma.ndesc += 1;
			}

		} else {	/* !do_cntr && !do_cq */

			assert(0);

		}

		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);
}

static inline size_t fi_bgq_atomic_internal(struct fi_bgq_ep *bgq_ep,
		const void *buf, size_t count, union fi_bgq_addr *bgq_dst_addr,
		uint64_t addr, uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context,
		const unsigned is_fetch, const void * fetch_vaddr,
		const unsigned is_compare, const void * compare_vaddr,
		const uint64_t tx_op_flags, const int lock_required,
		const uint64_t enable_cntr, const uint64_t enable_cq,
		const unsigned is_inject)
{
	assert((is_fetch==0)||(is_fetch==1));
	assert((is_compare==0)||(is_compare==1));

	const uint64_t do_cq = enable_cq && ((tx_op_flags & FI_COMPLETION) == FI_COMPLETION);
	struct fi_bgq_cntr * write_cntr = bgq_ep->tx.write_cntr;
	const uint64_t do_cntr = enable_cntr && (write_cntr != 0);

	MUHWI_Descriptor_t * desc =
		fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

	qpx_memcpy64((void*)desc, (const void*)&bgq_ep->tx.atomic.emulation.mfifo_model);

	/* set the destination torus address and fifo map */
	desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr->uid.fi);
	const uint64_t fifo_map = (uint64_t) fi_bgq_addr_get_fifo_map(bgq_dst_addr->fi);
	desc->Torus_FIFO_Map = fifo_map;

	desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
		fi_bgq_addr_rec_fifo_id(bgq_dst_addr->fi);

	const size_t max_count = maxcount(datatype, is_compare, is_fetch);
	const size_t xfer_count = MIN(max_count,count);
	const uint32_t nbytes = (uint32_t)(sizeofdt(datatype) * xfer_count);

	union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
	hdr->atomic.dt = datatype;
	hdr->atomic.op = op;
	hdr->atomic.do_cntr = do_cntr;
	hdr->atomic.cntr_bat_id = do_cntr ? write_cntr->std.batid : -1;
	hdr->atomic.nbytes_minus_1 = nbytes - 1;
	hdr->atomic.key = (uint16_t)key;
	hdr->atomic.offset = addr;
	hdr->atomic.is_local = fi_bgq_addr_is_local(bgq_dst_addr->fi);

	hdr->atomic.is_fetch = is_fetch;


	if (is_inject) {	/* const expression with cause branch to compile out */

		/* locate the payload lookaside slot */
		void * payload =
			fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
				desc, &desc->Pa_Payload);

		desc->Message_Length = nbytes;

		if (buf) memcpy((void *)payload, (const void *)buf, nbytes);

	} else if (!is_fetch && !is_compare) {	/* const expression with cause branch to compile out */

		desc->Message_Length = nbytes;
		fi_bgq_cnk_vaddr2paddr(buf, nbytes, &desc->Pa_Payload);

		assert(!do_cq);

	} else {

		/* locate the payload lookaside slot */
		union fi_bgq_mu_packet_payload * payload =
			(union fi_bgq_mu_packet_payload *)fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
				desc, &desc->Pa_Payload);

		/* initialize the atomic operation metadata in the packet payload */
		payload->atomic_fetch.metadata.fifo_map = fifo_map;
		payload->atomic_fetch.metadata.cq_paddr = 0;

		if (is_fetch) {
			fi_bgq_cnk_vaddr2paddr(fetch_vaddr, nbytes,
				&payload->atomic_fetch.metadata.dst_paddr);

			/* copy the origin (source) data into the injection lookaside buffer */
			if (buf) memcpy((void*)&payload->atomic_fetch.data[0], (const void*) buf, nbytes);
			desc->Message_Length = sizeof(struct fi_bgq_mu_fetch_metadata) +
				nbytes + nbytes * is_compare;

			if (is_compare) {
				/* copy the origin (compare) data into the injection lookaside buffer */
				memcpy((void*)&payload->atomic_fetch.data[nbytes], compare_vaddr, nbytes);
			}

			if (do_cq) {

				/* initialize the completion entry */
				assert(context);
				assert(((uintptr_t)context & 0x07ull) == 0);	/* must be 8 byte aligned */
				union fi_bgq_context * bgq_context = (union fi_bgq_context *)context;
				bgq_context->flags = 0;		/* TODO */
				bgq_context->len = nbytes;
				bgq_context->buf = NULL;
				bgq_context->byte_counter = nbytes;
				bgq_context->tag = 0;

				fi_bgq_cnk_vaddr2paddr((const void*)&bgq_context->byte_counter,
					sizeof(uint64_t), &payload->atomic_fetch.metadata.cq_paddr);

				fi_bgq_cq_enqueue_pending(bgq_ep->tx.send_cq, bgq_context, lock_required);
			}

		} else {
			assert(0);	/* !fetch, compare */
		}
	}

	MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

	return xfer_count;
}


static inline ssize_t fi_bgq_atomic_generic(struct fid_ep *ep,
		const void *buf, size_t count,
		fi_addr_t dst_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void* context,
		const int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	/* TODO - if this is a FI_CLASS_STX_CTX, then the lock is required */
	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	size_t xfer __attribute__ ((unused));
	xfer = fi_bgq_atomic_internal(bgq_ep, buf, count,
		(union fi_bgq_addr *)&dst_addr,	addr, key, datatype, op,
		context, 0, NULL, 0, NULL,
		bgq_ep->tx.op_flags, lock_required, 0, 0, 0);
	assert(xfer == count);

	/* TODO - if this is a FI_CLASS_STX_CTX, then the lock is required */
	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}

static inline ssize_t fi_bgq_atomic_writemsg_generic(struct fid_ep *ep,
		const struct fi_msg_atomic *msg, const uint64_t flags,
		const int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	const enum fi_datatype datatype = msg->datatype;
	const enum fi_op op = msg->op;

	ret = fi_bgq_check_atomic(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV, datatype, op, 1);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	union fi_bgq_addr * bgq_dst_addr = (union fi_bgq_addr *)&msg->addr;

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

		const size_t count_requested = MIN(msg_iov_dtcount,rma_iov_dtcount);

		const size_t count_transfered =
			fi_bgq_atomic_internal(bgq_ep, (void*)msg_iov_vaddr,
				count_requested, bgq_dst_addr, rma_iov_addr,
				rma_iov_key, datatype, op, NULL,
				0, NULL, 0, NULL, flags, lock_required, 0, 0, 0);

		const size_t bytes_transfered = dtsize * count_transfered;

		msg_iov_dtcount -= count_transfered;
		msg_iov_vaddr += bytes_transfered;

		if ((msg_iov_dtcount == 0) && ((msg_iov_index+1) < msg_iov_count)) {
			++msg_iov_index;
			msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
			msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;
		}

		rma_iov_dtcount -= count_transfered;
		rma_iov_addr  += bytes_transfered;

		if ((rma_iov_dtcount == 0) && ((rma_iov_index+1) < rma_iov_count)) {
			++rma_iov_index;
			rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
			rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
			rma_iov_key = msg->rma_iov[rma_iov_index].key;
		}
	}

	fi_bgq_atomic_fence(bgq_ep, flags, bgq_dst_addr,
		(union fi_bgq_context *)msg->context,
		lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}



static inline ssize_t fi_bgq_atomic_readwritemsg_generic (struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		struct fi_ioc *resultv,
		const size_t result_count,
		const uint64_t flags,
		const int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	const enum fi_datatype datatype = msg->datatype;
	const enum fi_op op = msg->op;

	ret = fi_bgq_check_atomic(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV, datatype, op, 1);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	union fi_bgq_addr * bgq_dst_addr = (union fi_bgq_addr *)&msg->addr;

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

	if (op != FI_ATOMIC_READ) {	/* likely */

		size_t msg_iov_index = 0;
		const size_t msg_iov_count = msg->iov_count;
		uint64_t msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
		uintptr_t msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;

		size_t count_requested = MIN3(msg_iov_dtcount, rma_iov_dtcount, rst_iov_dtcount);

		while (count_requested > 0) {

			const size_t count_transfered =
				fi_bgq_atomic_internal(bgq_ep, (void*)msg_iov_vaddr,
					count_requested, bgq_dst_addr, rma_iov_addr,
					rma_iov_key, datatype, op, NULL,
					1, (const void *)rst_iov_vaddr, 0, NULL,
					flags, lock_required, 0, 0, 0);

			const size_t bytes_transfered = dtsize * count_transfered;

			msg_iov_dtcount -= count_transfered;
			msg_iov_vaddr += bytes_transfered;

			if ((msg_iov_dtcount == 0) && ((msg_iov_index+1) < msg_iov_count)) {
				++msg_iov_index;
				msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
				msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;
			}

			rma_iov_dtcount -= count_transfered;
			rma_iov_addr  += bytes_transfered;

			if ((rma_iov_dtcount == 0) && ((rma_iov_index+1) < rma_iov_count)) {
				++rma_iov_index;
				rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
				rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
				rma_iov_key = msg->rma_iov[rma_iov_index].key;
			}

			rst_iov_dtcount -= count_transfered;
			rst_iov_vaddr += bytes_transfered;

			if ((rst_iov_dtcount == 0) && ((rst_iov_index+1) < rst_iov_count)) {
				++rst_iov_index;
				rst_iov_dtcount = resultv[rst_iov_index].count;
				rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;
			}

			count_requested = MIN3(msg_iov_dtcount, rma_iov_dtcount, rst_iov_dtcount);
		}

	} else {

		size_t count_requested = MIN(rma_iov_dtcount, rst_iov_dtcount);

		while (rma_iov_dtcount != 0 && rst_iov_dtcount != 0) {

			const size_t count_transfered =
				fi_bgq_atomic_internal(bgq_ep, NULL,
					count_requested, bgq_dst_addr, rma_iov_addr,
					rma_iov_key, datatype, op, NULL,
					1, (const void *)rst_iov_vaddr, 0, NULL,
					flags, lock_required, 0, 0, 0);

			const size_t bytes_transfered = dtsize * count_transfered;

			rma_iov_dtcount -= count_transfered;
			rma_iov_addr  += bytes_transfered;

			if ((rma_iov_dtcount == 0) && ((rma_iov_index+1) < rma_iov_count)) {
				++rma_iov_index;
				rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
				rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
				rma_iov_key = msg->rma_iov[rma_iov_index].key;
			}

			rst_iov_dtcount -= count_transfered;
			rst_iov_vaddr += bytes_transfered;

			if ((rst_iov_dtcount == 0) && ((rst_iov_index+1) < rst_iov_count)) {
				++rst_iov_index;
				rst_iov_dtcount = resultv[rst_iov_index].count;
				rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;
			}

			count_requested = MIN(rma_iov_dtcount, rst_iov_dtcount);
		}
	}

	fi_bgq_atomic_fence(bgq_ep, flags, bgq_dst_addr,
		(union fi_bgq_context *)msg->context,
		lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}

static inline ssize_t fi_bgq_atomic_compwritemsg_generic (struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		const struct fi_ioc *comparev,
		size_t compare_count,
		struct fi_ioc *resultv,
		size_t result_count,
		uint64_t flags,
		const int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	const enum fi_datatype datatype = msg->datatype;
	const enum fi_op op = msg->op;

	ret = fi_bgq_check_atomic(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV, datatype, op, 1);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	union fi_bgq_addr * bgq_dst_addr = (union fi_bgq_addr *)&msg->addr;

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

	while (msg_iov_dtcount != 0 && rma_iov_dtcount != 0 && rst_iov_dtcount != 0 && cmp_iov_dtcount != 0) {

		const size_t count_requested =
			MIN4(msg_iov_dtcount,rma_iov_dtcount,rst_iov_dtcount,cmp_iov_dtcount);

		const size_t count_transfered =
			fi_bgq_atomic_internal(bgq_ep, (void*)msg_iov_vaddr,
				count_requested, bgq_dst_addr, rma_iov_addr,
				rma_iov_key, datatype, op, NULL,
				1, (const void *)rst_iov_vaddr, 1, (const void *)cmp_iov_vaddr,
				flags, lock_required, 0, 0, 0);

		const size_t bytes_transfered = dtsize * count_transfered;

		msg_iov_dtcount -= count_transfered;
		msg_iov_vaddr += bytes_transfered;

		if ((msg_iov_dtcount == 0) && ((msg_iov_index+1) < msg_iov_count)) {
			++msg_iov_index;
			msg_iov_dtcount = msg->msg_iov[msg_iov_index].count;
			msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].addr;
		}

		rma_iov_dtcount -= count_transfered;
		rma_iov_addr  += bytes_transfered;

		if ((rma_iov_dtcount == 0) && ((rma_iov_index+1) < rma_iov_count)) {
			++rma_iov_index;
			rma_iov_dtcount = msg->rma_iov[rma_iov_index].count;
			rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
			rma_iov_key = msg->rma_iov[rma_iov_index].key;
		}

		rst_iov_dtcount -= count_transfered;
		rst_iov_vaddr += bytes_transfered;

		if ((rst_iov_dtcount == 0) && ((rst_iov_index+1) < rst_iov_count)) {
			++rst_iov_index;
			rst_iov_dtcount = resultv[rst_iov_index].count;
			rst_iov_vaddr = (uintptr_t)resultv[rst_iov_index].addr;
		}

		cmp_iov_dtcount -= count_transfered;
		cmp_iov_vaddr += bytes_transfered;

		if ((cmp_iov_dtcount == 0) && ((cmp_iov_index+1) < cmp_iov_count)) {
			++cmp_iov_index;
			cmp_iov_dtcount = comparev[cmp_iov_index].count;
			cmp_iov_vaddr = (uintptr_t)comparev[cmp_iov_index].addr;
		}
	}

	fi_bgq_atomic_fence(bgq_ep, flags, bgq_dst_addr,
		(union fi_bgq_context *)msg->context,
		lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}

/*
 * Generic function to handle both fetching (1 operand) and compare
 * (2 operand) atomics.
 */
static inline ssize_t fi_bgq_fetch_compare_atomic_generic(struct fid_ep *ep,
		const void *buf, size_t count,
		void *desc,
		const void *compare, void *compare_desc,
		void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context,
		int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;
/* MPICH does NOT call fi_fetch_atomic or fi_compare_atomic so these functions
 * have not been properly tested - for now just assert 0 and come back later
 * and implement if an application on BGQ needs this.
 */
	assert(0);
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_atomic(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV, datatype, op, count);
	if (ret)
		return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret)
		return ret;

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret)
		return ret;

	return 0;

}

static inline ssize_t fi_bgq_fetch_atomic_generic(struct fid_ep *ep,
		const void *buf, size_t count,
		void *desc,
		void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context,
		int lock_required)
{



	return fi_bgq_fetch_compare_atomic_generic(ep,
			buf, count, desc, NULL, NULL,
			result, result_desc, dest_addr, addr,
			key, datatype, op, context,
			lock_required);
}

static inline ssize_t fi_bgq_compare_atomic_generic(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		const void *compare, void *compare_desc,
		void  *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context,
		int lock_required)
{
	return fi_bgq_fetch_compare_atomic_generic(ep,
			buf, count, desc, compare, compare_desc,
			result, result_desc, dest_addr, addr,
			key, datatype, op, context,
			lock_required);
}

static inline ssize_t fi_bgq_inject_atomic_generic(struct fid_ep *ep,
                const void *buf, size_t count,
                fi_addr_t dest_addr, uint64_t addr, uint64_t key,
                enum fi_datatype datatype, enum fi_op op,
		int lock_required)
{
	int			ret = 0;
	struct fi_bgq_ep        *bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);
	ret = fi_bgq_check_atomic(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV, datatype, op, count);
	if (ret)
		return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret)
		return ret;

	fi_bgq_atomic_internal(bgq_ep, buf, count,
		(union fi_bgq_addr *)&dest_addr, addr, key, datatype, op,
		NULL, 0, NULL, 0, NULL,
		bgq_ep->tx.op_flags, lock_required, 1, 0, 1);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret)
		return ret;

	return 0;
}

/* Declare specialized functions that qualify for FABRIC_DIRECT.
 * - No locks
 */

#define FI_BGQ_ATOMIC_FABRIC_DIRECT_LOCK	0

FI_BGQ_ATOMIC_SPECIALIZED_FUNC(FI_BGQ_ATOMIC_FABRIC_DIRECT_LOCK)

#ifdef FABRIC_DIRECT
#define fi_atomic(ep, buf, count, desc, dest_addr, 			\
		addr, key, datatype, op, context) 			\
	(FI_BGQ_ATOMIC_SPECIALIZED_FUNC_NAME(atomic,			\
			FI_BGQ_ATOMIC_FABRIC_DIRECT_LOCK)		\
	(ep, buf, count, desc, dest_addr, addr, key,			\
		datatype, op, context))

#define fi_inject_atomic(ep, buf, count, dest_addr, addr, key,		\
		datatype, op)						\
	(FI_BGQ_ATOMIC_SPECIALIZED_FUNC_NAME(inject_atomic,		\
			FI_BGQ_ATOMIC_FABRIC_DIRECT_LOCK)		\
	(ep, buf, count, dest_addr, addr, key, datatype, op))

#define fi_fetch_atomic(ep, buf, count, desc, result, result_desc,	\
		dest_addr, addr, key, datatype, op, context)		\
	(FI_BGQ_ATOMIC_SPECIALIZED_FUNC_NAME(fetch_atomic,		\
			FI_BGQ_ATOMIC_FABRIC_DIRECT_LOCK)		\
	 (ep, buf, count, desc, result, result_desc,			\
		dest_addr, addr, key, datatype, op, context))

#define fi_compare_atomic(ep, buf, count, desc, compare, compare_desc,	\
		result, result_desc, dest_addr, addr, key, datatype,	\
		op, context)						\
	(FI_BGQ_ATOMIC_SPECIALIZED_FUNC_NAME(compare_atomic,		\
			FI_BGQ_ATOMIC_FABRIC_DIRECT_LOCK)		\
	 (ep, buf, count, desc, compare, compare_desc,			\
		result, result_desc, dest_addr, addr, key,		\
		datatype, op, context))

static inline int
fi_atomicvalid(struct fid_ep *ep,
	       enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	return ep->atomic->writevalid(ep, datatype, op, count);
}

static inline int
fi_fetch_atomicvalid(struct fid_ep *ep,
		     enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	return ep->atomic->readwritevalid(ep, datatype, op, count);
}

static inline int
fi_compare_atomicvalid(struct fid_ep *ep,
		       enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	return ep->atomic->compwritevalid(ep, datatype, op, count);
}

static inline ssize_t
fi_atomicmsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg, uint64_t flags)
{
	return ep->atomic->writemsg(ep, msg, flags);
}

static inline ssize_t
fi_fetch_atomicmsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		struct fi_ioc *resultv, void **result_desc, size_t result_count,
		uint64_t flags)
{
	return ep->atomic->readwritemsg(ep, msg, resultv, result_desc,
			result_count, flags);
}

static inline ssize_t
fi_compare_atomicmsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
		const struct fi_ioc *comparev, void **compare_desc,
		size_t compare_count, struct fi_ioc *resultv,
		void **result_desc, size_t result_count, uint64_t flags)
{
	return ep->atomic->compwritemsg(ep, msg, comparev, compare_desc,
		compare_count, resultv, result_desc, result_count, flags);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_BGQ_DIRECT_ATOMIC_H_ */
