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
#ifndef _FI_BGQ_DIRECT_RMA_H_
#define _FI_BGQ_DIRECT_RMA_H_

#define FABRIC_DIRECT_RMA 1

#include <pthread.h>

#include "rdma/bgq/fi_bgq_compiler.h"
#include "rdma/bgq/fi_bgq_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline int fi_bgq_check_rma(struct fi_bgq_ep *bgq_ep,
		enum fi_av_type av_type)
{
#ifdef DEBUG
	if (!bgq_ep)
		return -FI_EINVAL;
	if (bgq_ep->state != FI_BGQ_EP_ENABLED)
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


static inline void fi_bgq_readv_internal (struct fi_bgq_ep * bgq_ep,
		const struct iovec * iov,
		const size_t niov,
		const union fi_bgq_addr * bgq_target_addr,
		const uint64_t * addr,
		const uint64_t * key,
		union fi_bgq_context * bgq_context,
		const uint64_t tx_op_flags,
		const uint64_t enable_cq,
		const uint64_t enable_cntr,
		const int lock_required)
{
#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_readv_internal starting - niov is %ld do_cntr is %d\n",niov,(enable_cntr && ( bgq_ep->write_cntr != 0)));
fflush(stderr);
#endif
	assert(niov <= 8);

	const uint64_t do_cq = enable_cq && (tx_op_flags & FI_COMPLETION);

	struct fi_bgq_cntr * write_cntr = bgq_ep->write_cntr;
	const uint64_t do_cntr = enable_cntr && (write_cntr != 0);

	MUHWI_Descriptor_t * model = &bgq_ep->tx.read.emulation.mfifo_model;

	const uint64_t fifo_map = fi_bgq_addr_get_fifo_map(bgq_target_addr->fi);

	/* busy-wait until a fifo slot is available .. */
	MUHWI_Descriptor_t * desc =
		fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

	/* copy the descriptor model into the injection fifo */
	qpx_memcpy64((void*)desc, (const void *)model);

	/* set the target torus address and fifo map */
	desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_target_addr->uid.fi);
	desc->Torus_FIFO_Map = fifo_map;

	/* locate the payload lookaside slot */
	MUHWI_Descriptor_t * dput_desc =
		(MUHWI_Descriptor_t *)fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
			desc, &desc->Pa_Payload);
	desc->Message_Length = (niov << BGQ_MU_DESCRIPTOR_SIZE_IN_POWER_OF_2);


	desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
	fi_bgq_addr_rec_fifo_id(bgq_target_addr->fi);

	union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
	hdr->rma.ndesc = niov;

	/* TODO - how to specify multiple remote injection fifos? */

	union fi_bgq_mu_descriptor * fi_dput_desc = (union fi_bgq_mu_descriptor *) dput_desc;

	unsigned i;
	for (i = 0; i < niov; ++i) {	/* on fence this loop will compile out (niov is 0) */

		qpx_memcpy64((void*)&dput_desc[i],
			(const void*)&bgq_ep->tx.read.emulation.dput_model);

		dput_desc[i].Torus_FIFO_Map = fifo_map;
		dput_desc[i].Message_Length = iov[i].iov_len;
		dput_desc[i].Pa_Payload = addr[i];

		/* determine the physical address of the destination data location */
		uint64_t iov_base_paddr = 0;
		uint32_t cnk_rc __attribute__ ((unused));
		cnk_rc = fi_bgq_cnk_vaddr2paddr(iov[i].iov_base, iov[i].iov_len, &iov_base_paddr);
		assert(cnk_rc==0);
		MUSPI_SetRecPayloadBaseAddressInfo(&dput_desc[i], FI_BGQ_MU_BAT_ID_GLOBAL, iov_base_paddr);

		assert((key[i] & 0xFFFF000000000000ul) == 0);	/* TODO - change this when key size > 48b */
		fi_dput_desc[i].rma.key_lsb = key[i];
	}

	if (do_cntr && niov < 8) {	/* likely */
#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_readv_internal do_cntr && niov %ld < 8\n",niov);
fflush(stderr);
#endif
		/* add the counter update direct-put descriptor to the
		 * tail of the rget/mfifo payload */

		qpx_memcpy64((void*)&dput_desc[niov],
			(const void*)&bgq_ep->tx.read.cntr_model);

		dput_desc[niov].Torus_FIFO_Map = fifo_map;
		MUSPI_SetRecPayloadBaseAddressInfo(&dput_desc[niov],
			FI_BGQ_MU_BAT_ID_GLOBAL,
			MUSPI_GetAtomicAddress(write_cntr->std.paddr, MUHWI_ATOMIC_OPCODE_STORE_ADD));

		desc->Message_Length += sizeof(MUHWI_Descriptor_t);
		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		hdr->rma.ndesc += 1;

		if (!do_cq) {	/* likely */

#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_readv_internal do_cntr && niov < 8 AND (!do_cq)\n");
fflush(stderr);
#endif
			MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		} else 	if (niov < 7) {

			/* add the cq update direct-put descriptor to the
			 * tail of the rget/mfifo payload (after the cntr update) */

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

			MUHWI_Descriptor_t * cq_desc = &dput_desc[niov+1];

			qpx_memcpy64((void*)cq_desc,
				(const void*)&bgq_ep->tx.read.cq_model);

			cq_desc->Torus_FIFO_Map = fifo_map;
			MUSPI_SetRecPayloadBaseAddressInfo(cq_desc,
				FI_BGQ_MU_BAT_ID_GLOBAL, byte_counter_paddr);

			desc->Message_Length += sizeof(MUHWI_Descriptor_t);
			union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
			hdr->rma.ndesc += 1;

			MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

			fi_bgq_cq_enqueue_pending(bgq_ep->send_cq, bgq_context, lock_required);

		} else {

			/* the rget/mfifo payload is full - inject the data
			 * movement descriptors, then inject the counter
			 * completion descriptor */
			MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

			/* be lazy and do a single recursive call */
			fi_bgq_readv_internal(bgq_ep,
				NULL, 0,		/* no iovec array */
				bgq_target_addr,
				NULL, NULL,		/* no addr array, no key array */
				bgq_context, tx_op_flags,
				1,			/* enable cq */
				0,			/* disable cntr */
				lock_required);
		}

	} else if (do_cntr) {	/* unlikely */

		/* the rget/mfifo payload is full - inject the data
		 * movement descriptors, then inject any counter or cq
		 * completion descriptor(s) via a recursive call */
		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		fi_bgq_readv_internal(bgq_ep,
			NULL, 0,		/* no iovec array */
			bgq_target_addr,
			NULL, NULL,		/* no addr array, no key array */
			bgq_context, tx_op_flags,
			do_cq,
			1,			/* enable cntr */
			lock_required);

	} else if (do_cq && niov < 8) {

		/* no cntr completion
		 *
		 * add the cq byte counter decrement direct-put
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

		MUHWI_Descriptor_t * cq_desc = &dput_desc[niov];

		qpx_memcpy64((void*)cq_desc,
			(const void*)&bgq_ep->tx.read.cq_model);

		cq_desc->Torus_FIFO_Map = fifo_map;
		MUSPI_SetRecPayloadBaseAddressInfo(cq_desc,
			FI_BGQ_MU_BAT_ID_GLOBAL, byte_counter_paddr);

		desc->Message_Length += sizeof(MUHWI_Descriptor_t);
		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		hdr->rma.ndesc += 1;

		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		fi_bgq_cq_enqueue_pending(bgq_ep->send_cq, bgq_context, lock_required);

	} else if (do_cq) {

		/* the rget/mfifo payload is full - inject the data
		 * movement descriptors, then inject the cq completion
		 * descriptor via a recursive call */
		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		fi_bgq_readv_internal(bgq_ep,
			NULL, 0,		/* no iovec array */
			bgq_target_addr,
			NULL, NULL,		/* no addr array, no key array */
			bgq_context, tx_op_flags,
			1,	/* enable cq */
			0,	/* disable cntr */
			lock_required);

	} else {
		/* no cntr and no cq? very unlikely, if not invalid */

		/* if there are no completion operations then there *must* be
		 * at least one data movement operations */
		assert(niov > 0);

		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);
	}
}

static inline ssize_t fi_bgq_inject_write_generic(struct fid_ep *ep,
		const void *buf, size_t len, fi_addr_t dst_addr,
		uint64_t addr, uint64_t key,
		int lock_required)
{
#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_inject_write_generic starting\n");
#endif
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

//	if (av_type == FI_AV_TABLE)
//		dst_addr = bgq_ep->av->table[(size_t)dst_addr];

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	MUHWI_Descriptor_t * model =
		(FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) ?
			&bgq_ep->tx.write.direct.dput_model :
			&bgq_ep->tx.write.emulation.mfifo_model;

	/*
	 * busy-wait until a fifo slot is available ..
	 */
	MUHWI_Descriptor_t * desc =
		fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

	/* copy the descriptor model into the injection fifo */
	qpx_memcpy64((void*)desc, (const void *)model);

	/* set the destination torus address and fifo map */
	union fi_bgq_addr * bgq_dst_addr = (union fi_bgq_addr *)&dst_addr;
	desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr->uid.fi);
	desc->Torus_FIFO_Map = fi_bgq_addr_get_fifo_map(bgq_dst_addr->fi);
	desc->Message_Length = len;

	/* locate the payload lookaside slot */
	void * payload =
		fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
			desc, &desc->Pa_Payload);
	assert(len <= sizeof(union fi_bgq_mu_packet_payload));
	memcpy(payload, buf, len);

	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) {		/* branch will compile out */
#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_inject_write_generic - virtual addr is 0x%016lx physical addr is 0x%016lx key is %lu  \n",addr,(addr-key),key);
#endif

		/* the 'key' is the paddr of the remote memory region */
		MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, addr-key);

	} else if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {	/* branch will compile out */

		desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
			fi_bgq_addr_rec_fifo_id(bgq_dst_addr->fi);

		/* the 'key' is used to index into the remote base address table */
		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		hdr->rma.key = key;
		hdr->rma.offset = addr;
		hdr->rma.nbytes = len;
		hdr->rma.ndesc = 0;

	} else {
		assert(0);
	}

	/* the src buffer is available for reuse - increment the endpoint counter */
	struct fi_bgq_cntr * write_cntr = bgq_ep->write_cntr;
	if (write_cntr) L2_AtomicStoreAdd(write_cntr->std.l2_vaddr, 1);

	MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}

static inline void fi_bgq_write_fence (struct fi_bgq_ep * bgq_ep,
		const uint64_t tx_op_flags,
		const union fi_bgq_addr * bgq_dst_addr,
		union fi_bgq_context * bgq_context,
		const int lock_required)
{
	fi_bgq_readv_internal(bgq_ep,
		NULL, 0,		/* no iovec array */
		bgq_dst_addr,
		NULL, NULL,		/* no addr array, key array */
		bgq_context, tx_op_flags,
		1,
		1,
		lock_required);
}

static inline void fi_bgq_write_internal (struct fi_bgq_ep * bgq_ep,
		const void * buf,
		size_t len,
		const union fi_bgq_addr * bgq_dst_addr,
		uint64_t addr,
		const uint64_t key,
		union fi_bgq_context * bgq_context,
		const uint64_t tx_op_flags,
		const uint64_t enable_cq,
		const uint64_t enable_cntr,
		const int lock_required)
{

#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_write_internal starting\n");
#endif
	const uint64_t do_cq = enable_cq && ((tx_op_flags & FI_COMPLETION) == FI_COMPLETION);

	struct fi_bgq_cntr * write_cntr = bgq_ep->write_cntr;
	const uint64_t do_cntr = enable_cntr && (write_cntr != 0);

	MUHWI_Descriptor_t * model =
		(FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) ?
			&bgq_ep->tx.write.direct.dput_model :
			&bgq_ep->tx.write.emulation.mfifo_model;

	/* busy-wait until a fifo slot is available .. */
	MUHWI_Descriptor_t * desc =
		fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

	/* copy the descriptor model into the injection fifo */
	qpx_memcpy64((void*)desc, (const void *)model);

	/* set the destination torus address and fifo map */
	desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr->uid.fi);
	desc->Torus_FIFO_Map = fi_bgq_addr_get_fifo_map(bgq_dst_addr->fi);

	if (tx_op_flags & FI_INJECT) {	/* unlikely */

		assert(len <= sizeof(union fi_bgq_mu_packet_payload));

		/* locate the payload lookaside slot */
		void * payload =
			fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
				desc, &desc->Pa_Payload);

		memcpy(payload, buf, len);
		desc->Message_Length = len;

		if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) {		/* branch will compile out */

#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_write_internal tx_op_flags & FI_INJECT - virtual addr is 0x%016lx physical addr is 0x%016lx key is %lu  \n",addr,(addr-key),key);
#endif
			/* the 'key' is the paddr of the remote memory region */
			MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, addr-key);

		} else if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {	/* branch will compile out */

			desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
				fi_bgq_addr_rec_fifo_id(bgq_dst_addr->fi);

			/* the 'key' is used to index into the remote base address table */
			union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
			hdr->rma.key = key;
			hdr->rma.offset = addr;
			hdr->rma.nbytes = len;
			hdr->rma.ndesc = 0;

		} else {
			assert(0);
		}

		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		/* FI_TRANSMIT_COMPLETE and FI_DELIVERY_COMPLETE are not supported */
		assert((tx_op_flags & (FI_COMPLETION | FI_TRANSMIT_COMPLETE)) != (FI_COMPLETION | FI_TRANSMIT_COMPLETE));
		assert((tx_op_flags & (FI_COMPLETION | FI_DELIVERY_COMPLETE)) != (FI_COMPLETION | FI_DELIVERY_COMPLETE));

		if (do_cq) {

			assert(bgq_context);
			assert(((uintptr_t)bgq_context & 0x07ull) == 0);	/* must be 8 byte aligned */
			bgq_context->flags = FI_RMA | FI_WRITE;
			bgq_context->len = 0;
			bgq_context->buf = NULL;
			bgq_context->byte_counter = 0;
			bgq_context->tag = 0;

			fi_bgq_cq_enqueue_completed(bgq_ep->send_cq, bgq_context, lock_required);
		}

		/* the src buffer is available for reuse - increment the endpoint counter */
		if (do_cntr) L2_AtomicStoreAdd(write_cntr->std.l2_vaddr, 1);

	} else {
		size_t xfer_bytes = MIN(len, sizeof(union fi_bgq_mu_packet_payload));

		if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) {		/* branch will compile out */

#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_write_internal - NOT tx_op_flags & FI_INJECT - virtual addr is 0x%016lx physical addr is 0x%016lx key is %lu  \n",addr,(addr-key),key);
#endif
			/* the 'key' is the paddr of the remote memory region */
			MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, addr-key);

		} else if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {	/* branch will compile out */

			desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
				fi_bgq_addr_rec_fifo_id(bgq_dst_addr->fi);

			/* the 'key' is used to index into the remote base address table */
			union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
			hdr->rma.key = key;
			hdr->rma.offset = addr;
			hdr->rma.nbytes = xfer_bytes;
			hdr->rma.ndesc = 0;

		} else {
			assert(0);
		}

		/* determine the physical address of the source data */
		uint64_t src_paddr = 0;
		uint32_t cnk_rc __attribute__ ((unused));
		cnk_rc = fi_bgq_cnk_vaddr2paddr(buf, len, &src_paddr);
		assert(cnk_rc==0);

		desc->Message_Length = xfer_bytes;
		desc->Pa_Payload = src_paddr;

		if (len <= sizeof(union fi_bgq_mu_packet_payload)) {	/* likely */

			MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		} else {

			MUHWI_Descriptor_t model = *desc;
			MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

			src_paddr += xfer_bytes;
			len -= xfer_bytes;
			addr += xfer_bytes;

			while (len > 0) {
				desc = fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

				qpx_memcpy64((void*)desc, (const void*)&model);

				xfer_bytes = MIN(len, sizeof(union fi_bgq_mu_packet_payload));
				desc->Message_Length = xfer_bytes;
				desc->Pa_Payload = src_paddr;

				union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
				if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) {
#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_write_internal for multiple packets - NOT tx_op_flags & FI_INJECT - virtual addr is 0x%016lx physical addr is 0x%016lx key is %lu  \n",addr,(addr-key),key);
#endif
					/* the 'key' is the paddr of the remote memory region */
					MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, addr-key);

				}
				else if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
					hdr->rma.offset = addr;
					hdr->rma.nbytes = xfer_bytes;
				}
				else {
                		        assert(0);
		                }


				MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

				src_paddr += xfer_bytes;
				len -= xfer_bytes;
				addr += xfer_bytes;
			}
		}

		if (do_cq || do_cntr)
			fi_bgq_readv_internal(bgq_ep, NULL, 0, bgq_dst_addr,
				NULL, NULL, bgq_context,
				tx_op_flags, do_cq, do_cntr, lock_required);
	}
}





static inline ssize_t fi_bgq_write_generic(struct fid_ep *ep,
		const void *buf, size_t len, void *desc, fi_addr_t dst_addr,
		uint64_t addr, uint64_t key, void *context,
		int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	fi_bgq_write_internal(bgq_ep, buf, len, (union fi_bgq_addr *)&dst_addr,
		addr, key, (union fi_bgq_context *)context,
		bgq_ep->tx.op_flags, 1, 1, lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) {
		return ret;
	}

	return 0;
}

static inline ssize_t fi_bgq_writev_generic(struct fid_ep *ep,
		const struct iovec *iov, void **desc, size_t count,
		fi_addr_t dst_addr, uint64_t addr, uint64_t key, void *context,
		int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	const union fi_bgq_addr bgq_dst_addr = *((union fi_bgq_addr *)&dst_addr);

	size_t index = 0;
	for (index = 0; index < count; ++index) {

		size_t len = iov[index].iov_len;
		void * buf = iov[index].iov_base;

		fi_bgq_write_internal(bgq_ep, buf, len, &bgq_dst_addr,
			addr, key, (union fi_bgq_context *)context,
			0, 0, 0, lock_required);

		addr += len;
	}

	fi_bgq_write_fence(bgq_ep, bgq_ep->tx.op_flags, &bgq_dst_addr, (union fi_bgq_context *)context,
		lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}


static inline ssize_t fi_bgq_writemsg_generic(struct fid_ep *ep,
		const struct fi_msg_rma *msg, uint64_t flags,
		int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	union fi_bgq_addr * bgq_dst_addr = (union fi_bgq_addr *)&msg->addr;


	size_t rma_iov_index = 0;
	const size_t rma_iov_count = msg->rma_iov_count;
	uint64_t rma_iov_bytes = msg->rma_iov[rma_iov_index].len;
	uint64_t rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
	uint64_t rma_iov_key = msg->rma_iov[rma_iov_index].key;

	size_t msg_iov_index = 0;
	const size_t msg_iov_count = msg->iov_count;
	uint64_t msg_iov_bytes = msg->msg_iov[msg_iov_index].iov_len;
	uintptr_t msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].iov_base;

#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_writemsg_generic msg_iov_bytes is %lu rma_iov_bytes is %lu base vadder is 0x%016lx lock_required is %d\n",msg_iov_bytes,rma_iov_bytes,msg_iov_vaddr,lock_required);
fflush(stderr);
#endif
	while (msg_iov_bytes != 0 && rma_iov_bytes != 0) {

		size_t len = (msg_iov_bytes <= rma_iov_bytes) ? msg_iov_bytes : rma_iov_bytes;

#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_writemsg_generic calling fi_bgq_write_internal with msg_iov_vaddr 0x%016lx and len %lu\n",msg_iov_vaddr,len);
fflush(stderr);
#endif
		fi_bgq_write_internal(bgq_ep, (void*)msg_iov_vaddr, len, bgq_dst_addr,
			rma_iov_addr, rma_iov_key, NULL, 0, 0, 0, lock_required);

		msg_iov_bytes -= len;
		msg_iov_vaddr += len;

		if ((msg_iov_bytes == 0) && ((msg_iov_index+1) < msg_iov_count)) {
			++msg_iov_index;
			msg_iov_bytes = msg->msg_iov[msg_iov_index].iov_len;
			msg_iov_vaddr = (uintptr_t)msg->msg_iov[msg_iov_index].iov_base;
		}

		rma_iov_bytes -= len;
		rma_iov_addr  += len;

		if ((rma_iov_bytes == 0) && ((rma_iov_index+1) < rma_iov_count)) {
			++rma_iov_index;
			rma_iov_bytes = msg->rma_iov[rma_iov_index].len;
			rma_iov_addr = msg->rma_iov[rma_iov_index].addr;
			rma_iov_key = msg->rma_iov[rma_iov_index].key;
		}
	}

#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_writemsg_generic calling fi_bgq_write_fence\n");
fflush(stderr);
#endif
	fi_bgq_write_fence(bgq_ep, flags, bgq_dst_addr,
		(union fi_bgq_context *)msg->context,
		lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}


static inline ssize_t fi_bgq_read_generic(struct fid_ep *ep,
		void *buf, size_t len, void *desc, fi_addr_t src_addr,
		uint64_t addr, uint64_t key, void *context,
		int lock_required)
{
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	struct iovec iov;
	iov.iov_base = buf;
	iov.iov_len = len;

	fi_bgq_readv_internal(bgq_ep, &iov, 1, (union fi_bgq_addr *)&src_addr,
		&addr, &key, (union fi_bgq_context *)context,
		bgq_ep->tx.op_flags, 1, 1, lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret)
		return ret;

	return 0;
}

static inline ssize_t fi_bgq_readv_generic (struct fid_ep *ep,
		const struct iovec *iov, void **desc, size_t count,
		fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context,
		int lock_required)
{

#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_readv_generic count is %lu addr is 0x%016lx key is 0x%016lx\n",count,addr,key);
fflush(stderr);
#endif

	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	union fi_bgq_addr * bgq_addr = (union fi_bgq_addr *)&src_addr;
	union fi_bgq_context * bgq_context = (union fi_bgq_context *)context;
	const uint64_t tx_op_flags = bgq_ep->tx.op_flags;

	uint64_t addr_v[8] = { addr, addr, addr, addr, addr, addr, addr, addr };
	uint64_t key_v[8] = { key, key, key, key, key, key, key, key };

	/* max 8 descriptors (iovecs) per readv_internal */
	size_t index = 0;
	const size_t full_count = count >> 3;
	for (index = 0; index < full_count; index += 8) {

		fi_bgq_readv_internal(bgq_ep, &iov[index], 8, bgq_addr,
			addr_v, key_v, NULL, 0, 0, 0,
			lock_required);
	}

	/* if 'partial_ndesc' is zero, the fi_bgq_readv_internal() will fence */
	const size_t partial_ndesc = count & 0x07ull;
	fi_bgq_readv_internal(bgq_ep, &iov[index], partial_ndesc, bgq_addr,
		addr_v, key_v, bgq_context, tx_op_flags, 1, 1,
		lock_required);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret)
		return ret;

	return 0;
}


static inline ssize_t fi_bgq_readmsg_generic(struct fid_ep *ep,
		const struct fi_msg_rma *msg, uint64_t flags,
		int lock_required)
{
#ifdef FI_BGQ_TRACE
fprintf(stderr,"fi_bgq_readmsg_generic starting\n");
fflush(stderr);
#endif
	int			ret;
	struct fi_bgq_ep	*bgq_ep;

	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ret = fi_bgq_check_rma(bgq_ep, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	struct fi_bgq_cq * cq = bgq_ep->send_cq;
	const uint64_t enable_cq =
		(cq == NULL) || ((cq != NULL) && ((cq->bflags & FI_SELECTIVE_COMPLETION) && (flags & FI_COMPLETION) == 0)) ? 0 : 1;

	union fi_bgq_context * bgq_context = (union fi_bgq_context *) msg->context;
	union fi_bgq_addr * bgq_src_addr = (union fi_bgq_addr *)&msg->addr;

	/* for fi_read*(), the 'src' is the remote data */
	size_t src_iov_index = 0;
	const size_t src_iov_count = msg->rma_iov_count;
	uint64_t src_iov_bytes = msg->rma_iov[0].len;
	uint64_t src_iov_addr = msg->rma_iov[0].addr;
	uint64_t src_iov_key = msg->rma_iov[0].key;

	/* for fi_read*(), the 'dst' is the local data */
	size_t dst_iov_index = 0;
	const size_t dst_iov_count = msg->iov_count;
	uint64_t dst_iov_bytes = msg->msg_iov[0].iov_len;
	void * dst_iov_vaddr = msg->msg_iov[0].iov_base;

	size_t niov;
	struct iovec iov[8];
	uint64_t addr[8];
	uint64_t key[8];

	while (src_iov_index < src_iov_count) {

		for (niov = 0; niov < 8; ++niov) {
			const size_t len = (dst_iov_bytes <= src_iov_bytes) ? dst_iov_bytes : src_iov_bytes;
			iov[niov].iov_len = len;
			iov[niov].iov_base = dst_iov_vaddr;
			addr[niov] = src_iov_addr;
			key[niov] = src_iov_key;

			dst_iov_bytes -= len;
			src_iov_bytes -= len;

			if (src_iov_bytes == 0) {

				/* all done with this src rma iovec */

				if (src_iov_index == (src_iov_count-1)) {

					/* this is the last src rma iovec .. perform
					 * read with completion processing and return
					 *
					 * the 'dst_iov_bytes' must be zero and it must
					 * be the last dst iovec as well */
					assert(dst_iov_bytes==0);
					assert(dst_iov_index == (dst_iov_count-1));

					fi_bgq_readv_internal(bgq_ep, iov, niov+1,
						bgq_src_addr, addr, key,
						bgq_context,
						flags,
						enable_cq, 1,				/* enable_cq, enable_cntr */
						lock_required);

					ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
					if (ret) return ret;

					return 0;

				} else {

					/* advance to next src rma iovec */
					++src_iov_index;
					src_iov_bytes = msg->rma_iov[src_iov_index].len;
					src_iov_addr = msg->rma_iov[src_iov_index].addr;
					src_iov_key = msg->rma_iov[src_iov_index].key;
				}
			} else {
				src_iov_addr += len;
			}


			if (dst_iov_bytes == 0) {

				/* all done with this dst iovec */

				if (dst_iov_index == (dst_iov_count-1)) {
					/* this is the last dst iovec .. do nothing since
					 * the 'src_iov_bytes' must be zero and it must
					 * be the last src rma iovec as well */
					assert(src_iov_bytes==0);
					assert(src_iov_index == (src_iov_count-1));

					/* in fact, it should be impossible to get here */
					assert(0);
				} else {

					/* advance to next dst iovec */
					++dst_iov_index;
					dst_iov_bytes = msg->msg_iov[dst_iov_index].iov_len;
					dst_iov_vaddr = msg->msg_iov[dst_iov_index].iov_base;
				}
			} else {
				dst_iov_vaddr = (void*)((uintptr_t)dst_iov_vaddr + len);
			}


		}	/* end for */

		fi_bgq_readv_internal(bgq_ep, iov, 8, bgq_src_addr, addr, key,
			NULL, 0,
			0, 0,	/* disable_cq, disable_cntr */
			lock_required);

	}	/* end while */

	/* should never get here */
	assert(0);

	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}


/* Declare specialized functions that qualify for FABRIC_DIRECT.
 * - No locks
 */

#define FI_BGQ_RMA_FABRIC_DIRECT_LOCK	0

FI_BGQ_RMA_SPECIALIZED_FUNC(FI_BGQ_RMA_FABRIC_DIRECT_LOCK)

#ifdef FABRIC_DIRECT

#define fi_write(ep, buf, len, desc, dst_addr, addr, key, context)	\
	(FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(write,			\
			FI_BGQ_RMA_FABRIC_DIRECT_LOCK)			\
	(ep, buf, len, desc, dst_addr, addr, key, context))

#define fi_inject_write(ep, buf, len, dst_addr, addr, key)		\
	(FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(inject_write,			\
			FI_BGQ_RMA_FABRIC_DIRECT_LOCK)			\
			(ep, buf, len, dst_addr, addr, key))

#define fi_read(ep, buf, len, desc, src_addr, addr, key, context)	\
	(FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(read,				\
			FI_BGQ_RMA_FABRIC_DIRECT_LOCK)			\
			(ep, buf, len, desc, src_addr, addr, key, context))

#define fi_readmsg(ep, msg, flags)					\
	(FI_BGQ_RMA_SPECIALIZED_FUNC_NAME(readmsg,			\
			FI_BGQ_RMA_FABRIC_DIRECT_LOCK)			\
			(ep, msg, flags))

static inline ssize_t
fi_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	return ep->rma->writemsg(ep, msg, flags);
}
static inline ssize_t
fi_writev(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	return ep->rma->writev(ep, iov, desc, count, dest_addr, addr, key, context);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_BGQ_DIRECT_RMA_H_ */
