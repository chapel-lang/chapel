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
#ifndef _FI_BGQ_DIRECT_EP_H_
#define _FI_BGQ_DIRECT_EP_H_

#define FABRIC_DIRECT_ENDPOINT 1

#include <stdint.h>
#include <pthread.h>
#include <sys/uio.h>

#include "rdma/bgq/fi_bgq_compiler.h"
#include "rdma/bgq/fi_bgq_hwi.h"
#include "rdma/bgq/fi_bgq_spi.h"

#include "rdma/fi_direct_eq.h"

#include "rdma/bgq/fi_bgq_mu.h"
#include "rdma/bgq/fi_bgq_progress.h"
#include "rdma/bgq/fi_bgq_flight_recorder.h"

#define FI_BGQ_L2FIFO_CTL_SIZE (1024)

#define IS_TAG (0)
#define IS_MSG (1)

// #define FI_BGQ_TRACE 1
// #define FI_BGQ_REMOTE_COMPLETION

enum fi_bgq_ep_state {
	FI_BGQ_EP_UNINITIALIZED = 0,
	FI_BGQ_EP_INITITALIZED_DISABLED,
	FI_BGQ_EP_INITITALIZED_ENABLED
};

struct fi_bgq_stx {
	struct fid_stx			stx_fid;	/* 80 bytes */
	struct fi_bgq_domain		*domain;
	struct fi_tx_attr		attr;


	struct fi_bgq_spi_injfifo	injfifo;
	struct fi_bgq_spi_injfifo	rgetfifo;

	MUSPI_InjFifoSubGroup_t		injfifo_subgroup;
	MUSPI_InjFifoSubGroup_t		rgetfifo_subgroup;

	struct l2atomic_counter		ref_cnt;
	struct l2atomic_lock    	lock;
};

struct rx_operation {
	MUSPI_RecFifo_t			*rfifo;
	struct l2atomic_fifo		fifo;
	struct {
		union fi_bgq_context	*head;
		union fi_bgq_context	*tail;
	} mq;
	struct {
		struct fi_bgq_mu_packet	*head;
		struct fi_bgq_mu_packet	*tail;
		struct fi_bgq_mu_packet	*free;
	} ue;
};

struct fi_bgq_ep_tx {

	struct fi_bgq_spi_injfifo	injfifo;	/* cloned from stx; 88 bytes */
	uint64_t			unused0[5];

	/* == L2 CACHE LINE == */

	uint64_t			op_flags;
	struct fi_bgq_cq		*send_cq;
	struct fi_bgq_cntr		*send_cntr;
	struct fi_bgq_cntr		*write_cntr;
	uint64_t			unused[12];

	/* == L2 CACHE LINE == */

	struct {	/* three l2 cache lines */
		MUHWI_Descriptor_t	send_model;
		MUHWI_Descriptor_t	local_completion_model;	/* only "local completion eager" */
		MUHWI_Descriptor_t	rzv_model[2];	/* [0]=="internode"; [1]=="intranode" */
		MUHWI_Descriptor_t	remote_completion_model;
		uint8_t			unused[64];
	} send __attribute((aligned(L2_CACHE_LINE_SIZE)));
	/* == L2 CACHE LINE == */

	struct {
		struct {
			MUHWI_Descriptor_t	rget_model;
			MUHWI_Descriptor_t	dput_model;
		} direct;
		/* == L2 CACHE LINE == */

		struct {
			MUHWI_Descriptor_t	mfifo_model;
			MUHWI_Descriptor_t	dput_model;
		} emulation;
		/* == L2 CACHE LINE == */

		MUHWI_Descriptor_t		cntr_model;
		MUHWI_Descriptor_t		cq_model;
		/* == L2 CACHE LINE == */

		uint64_t			global_one_paddr;
		uint64_t			global_zero_paddr;

		uint8_t				unused[48];
	} read __attribute((aligned(L2_CACHE_LINE_SIZE)));

	/* == L2 CACHE LINE == */

	struct {
		struct {
			MUHWI_Descriptor_t	dput_model;
			MUHWI_Descriptor_t	unused;
		} direct;
		/* == L2 CACHE LINE == */

		struct {
			MUHWI_Descriptor_t	mfifo_model;
			MUHWI_Descriptor_t	rget_model;
		/* == L2 CACHE LINE == */

			MUHWI_Descriptor_t	dput_model;
			MUHWI_Descriptor_t	cntr_model;
		} emulation;
	} write __attribute((aligned(L2_CACHE_LINE_SIZE)));

	/* == L2 CACHE LINE == */

	struct {
		struct {
			MUHWI_Descriptor_t		mfifo_model;
			MUHWI_Descriptor_t		unused;

			/* == L2 CACHE LINE == */

			struct {
				MUHWI_Descriptor_t		mfifo_model;
				MUHWI_Descriptor_t		cntr_model;

				/* == L2 CACHE LINE == */

				MUHWI_Descriptor_t		cq_model;
				MUHWI_Descriptor_t		unused;
			} fence;
		} emulation;
	} atomic __attribute((aligned(L2_CACHE_LINE_SIZE)));

	/* == L2 CACHE LINE == */

	uint64_t		caps;
	uint64_t		mode;
	enum fi_bgq_ep_state	state;

	struct fi_bgq_stx	*stx;
	struct fi_bgq_stx	exclusive_stx;
	ssize_t			index;

} __attribute((aligned(L2_CACHE_LINE_SIZE)));

struct fi_bgq_ep_rx {

	/* == L2 CACHE LINE == */

	/* 'post' data is used when a thread is INITIATING recv operations */
	struct {
		uint64_t			op_flags;
		struct l2atomic_fifo_producer	match[2];	/* 0 == 'tag', 1 == 'msg' */	/* TODO - use an enum */
		struct l2atomic_fifo_producer	control;

	} post __attribute((aligned(L2_CACHE_LINE_SIZE)));

	/* == L2 CACHE LINE == */

	/* 'poll' data is used when a thread is making PROGRESS on recv operations */
	struct {

		MUSPI_RecFifo_t				*muspi_recfifo;
		uintptr_t				pad;

		/* 'rfifo' data is used to poll a mu reception fifo */
		struct {

			struct l2atomic_fifo_consumer	match;

			struct {
				union fi_bgq_context	*head;
				union fi_bgq_context	*tail;
			} mq;

			struct {
				struct fi_bgq_mu_packet	*head;
				struct fi_bgq_mu_packet	*tail;
				struct fi_bgq_mu_packet	*free;
			} ue;


		} rfifo[2];	/* 0 == 'tag', 1 == 'msg' */

		struct fi_bgq_spi_injfifo		injfifo;

		/* == L2 CACHE LINE == */

		struct {
			MUHWI_Descriptor_t	rget_model[2];	/* [0]=="internode"; [1]=="intranode" */
			/* == L2 CACHE LINE == */
			MUHWI_Descriptor_t	dput_model[2];	/* [0]=="internode"; [1]=="intranode" */
			/* == L2 CACHE LINE == */
			MUHWI_Descriptor_t	dput_completion_model;
			MUHWI_Descriptor_t	multi_recv_ack_model;

		} rzv __attribute((aligned(L2_CACHE_LINE_SIZE)));	/* TODO reorganize rzv for better cache layout */

		/* == L2 CACHE LINE == */

		/* TODO reorganize ack (with rzv) for better cache layout */
		MUHWI_Descriptor_t		ack_model[2];	/* [0]=="internode"; [1]=="intranode" */

		/* == L2 CACHE LINE == */

		MUHWI_Descriptor_t		atomic_dput_model;
		struct fi_bgq_bat_entry		*bat;
		uint64_t			pad_1[7];

		/* == L2 CACHE LINE == */

		MUHWI_Descriptor_t		atomic_cntr_update_model[2];	/* [0]=="internode"; [1]=="intranode" */

		/* == L2 CACHE LINE == */

		struct fi_bgq_cq	*recv_cq;
		struct fi_bgq_cntr	*write_cntr;
		uint64_t		min_multi_recv;

		/* -- non-critical -- */
		struct fi_bgq_domain    *domain;
		struct l2atomic_fifo_consumer	control;

		MUSPI_InjFifoSubGroup_t			injfifo_subgroup;

	} poll __attribute((aligned(L2_CACHE_LINE_SIZE)));

	uint64_t			caps;
	uint64_t			mode;
	uint64_t			op_flags;
	size_t				total_buffered_recv;	/* TODO - is this only used by receive operations? */
	struct fi_bgq_sep		*sep;
	struct fi_bgq_ep		*srx;
	union fi_bgq_addr		self;

	/* -- non-critical -- */
	ssize_t				index;
	uint64_t			min_multi_recv;
	void				*l2atomic_memptr;
	enum fi_bgq_ep_state	state;

} __attribute((aligned(L2_CACHE_LINE_SIZE)));

/*
 * The 'fi_bgq_ep' struct defines an endpoint with a single tx context and a
 * single rx context. The tx context is only valid if the FI_READ, FI_WRITE,
 * or FI_SEND capability is specified. The rx context is only valid if the
 * FI_RECV, FI_REMOTE_READ, or FI_REMOTE_WRITE flags are specified.
 *
 * A 'scalable tx context' is simply an endpoint structure with only the
 * tx flags specified, and a 'scalable rx context' is simply an endpoint
 * structure with only the rx flags specified.
 *
 * As such, multiple OFI 'classes' share this endpoint structure:
 *   FI_CLASS_EP
 *   FI_CLASS_TX_CTX
 *   --- no FI_CLASS_STX_CTX
 *   FI_CLASS_RX_CTX
 *   -- no FI_CLASS_SRX_CTX
 */
struct fi_bgq_ep {

	struct fid_ep		ep_fid;		/* 80 bytes */
	struct l2atomic_lock	lock;		/* 16 bytes */
	uint32_t		threading;
	uint32_t		av_type;
	uint32_t		mr_mode;
	uint8_t			unused[20];

	/* == L2 CACHE LINE == */

	struct fi_bgq_ep_tx	tx;
	struct fi_bgq_ep_rx	rx;

	struct fi_bgq_cntr	*read_cntr;
	struct fi_bgq_cntr	*write_cntr;
	struct fi_bgq_cntr	*send_cntr;
	struct fi_bgq_cntr	*recv_cntr;

	struct fi_bgq_cq	*send_cq;
	struct fi_bgq_cq	*recv_cq;

	struct fi_bgq_domain	*domain;
	void			*mem;

	struct fi_bgq_av	*av;
	struct fi_bgq_sep	*sep;

	struct {
		volatile uint64_t	enabled;
		volatile uint64_t	active;
		pthread_t		thread;
	} async;
	enum fi_bgq_ep_state	state;

} __attribute((aligned(L2_CACHE_LINE_SIZE)));

/*
 * A 'scalable endpoint' may not be directly specified in a data movement
 * functions, such as fi_tsend(), as it is only a container for multiple
 * tx and rx contexts.
 *
 * The scalable contexts share certain resources, such as the address vector.
 */
struct fi_bgq_sep {
	struct fid_ep		ep_fid;

	struct fi_bgq_domain	*domain;
	struct fi_bgq_av	*av;
	struct fi_info		*info;
	void			*memptr;

	struct l2atomic_counter	ref_cnt;

} __attribute((aligned(L2_CACHE_LINE_SIZE)));


void fi_bgq_ep_progress_manual_cancel (struct fi_bgq_ep * bgq_ep,
		const uint64_t cancel_context);

int fi_bgq_ep_progress_manual_recv (struct fi_bgq_ep *bgq_ep,
		const uint64_t is_msg,
		union fi_bgq_context * context,
		const uint64_t rx_op_flags,
		const uint64_t is_context_ext);

/* See: process_mfifo_context() */
int fi_bgq_ep_progress_manual_recv_fast (struct fi_bgq_ep *bgq_ep,
		const uint64_t is_msg,
		union fi_bgq_context * context);

int fi_bgq_ep_progress_manual (struct fi_bgq_ep *bgq_ep);

static inline
int fi_bgq_check_endpoint(struct fi_bgq_ep *bgq_ep, enum fi_av_type av_type)
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

	/* currently, only FI_AV_MAP is supported */
	if (av_type == FI_AV_TABLE) {
		return -FI_ENOSYS;
	} else if (av_type != FI_AV_MAP) {
		return -FI_EINVAL;
	}
#endif
        return 0;
}

static inline
int fi_bgq_ep_tx_check(struct fi_bgq_ep_tx * tx, enum fi_av_type av_type)
{
#ifdef DEBUG
	if (!tx)
		return -FI_EINVAL;
	if (tx->state != FI_BGQ_TX_ENABLED)
		return -FI_EINVAL;

	if (av_type == FI_AV_UNSPEC)
		return -FI_EINVAL;
	if (av_type == FI_AV_MAP && tx->av_type != FI_MAP)
		return -FI_EINVAL;
	if (av_type == FI_AV_TABLE && tx->av_type != FI_TABLE)
		return -FI_EINVAL;

	/* currently, only FI_AV_MAP is supported */
	if (av_type == FI_AV_TABLE)
		return -FI_ENOSYS;
	if (av_type != FI_AV_MAP)
		return -FI_EINVAL;
#endif
	return 0;
}


/*
 * NOTE-CACHE: This code touches 4 cachelines in the code path - not including
 * cachelines used for the actual injection fifo descriptor and the hardware
 * SRAM - when the message length <= 8 bytes.
 *
 * NOTE-CACHE: This code touches 5 cachelines in the code path - not including
 * cachelines used for the actual injection fifo descriptor, the hardware SRAM,
 * and the immediate payload - when the message length > 8 bytes.
 *
 * TODO - reorganize fi_bgq structures and implement a slimmed down version of
 * MUSPI_InjFifoAdvanceDesc to reduce cache pressure. The "<=8" code path
 * should only need to touch 2 cachelines and the ">8" code path should only
 * need to touch 3 cachelines.
 */
static inline
ssize_t fi_bgq_inject_generic(struct fid_ep *ep,
		const void *buf,
		size_t len,
		fi_addr_t dest_addr,
		uint64_t tag,
		const uint32_t data,
		int lock_required,
		const unsigned is_msg)
{
	assert(is_msg == 0 || is_msg == 1);

	struct fi_bgq_ep *bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ssize_t ret;
	ret = fi_bgq_ep_tx_check(&bgq_ep->tx, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	/* TODO - if this is a FI_CLASS_STX_CTX, then the lock is required */
	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	/* get the destination bgq torus address */
	const union fi_bgq_addr bgq_dst_addr = {.fi=dest_addr};

	/* eager with lookaside payload buffer and no completion */

	/* busy-wait until a fifo slot is available ... */
	MUHWI_Descriptor_t * send_desc = fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

	/* copy the descriptor model into the injection fifo */
	qpx_memcpy64((void*)send_desc, (const void*)&bgq_ep->tx.send.send_model);

	/* set the destination torus address and fifo map */
	send_desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr.uid.fi);
	send_desc->Torus_FIFO_Map = fi_bgq_addr_get_fifo_map(dest_addr);

	send_desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
		fi_bgq_addr_rec_fifo_id(dest_addr);

	union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &send_desc->PacketHeader;

	if (is_msg) {
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_EAGER);
	}

	/* locate the payload lookaside slot */
	uint64_t payload_paddr = 0;
	void *payload_vaddr =
		fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
			send_desc, &payload_paddr);
	send_desc->Pa_Payload = payload_paddr;

	send_desc->Message_Length = len;
	if (len) memcpy(payload_vaddr, buf, len);	/* TODO use a qpx-optimized memcpy instead */

	hdr->pt2pt.send.message_length = len;
	hdr->pt2pt.ofi_tag = tag;
	hdr->pt2pt.immediate_data = data;

#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_inject_generic dest addr is:\n");
	FI_BGQ_ADDR_DUMP((fi_addr_t *)&dest_addr);
#endif
	MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

	/* TODO - if this is a FI_CLASS_STX_CTX, then the lock is required */
	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}

static inline
ssize_t fi_bgq_send_generic_flags(struct fid_ep *ep,
		const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, uint64_t tag, void *context,
		const uint32_t data, int lock_required,
		const unsigned is_msg, const unsigned is_contiguous,
		const unsigned override_flags, uint64_t tx_op_flags)
{
#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_send_generic_flags starting\n");
#endif
	assert(is_msg == 0 || is_msg == 1);
	assert(is_contiguous == 0 || is_contiguous == 1);

	struct fi_bgq_ep *bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	ssize_t ret;
	ret = fi_bgq_ep_tx_check(&bgq_ep->tx, FI_BGQ_FABRIC_DIRECT_AV);
	if (ret) return ret;

	/* TODO - if this is a FI_CLASS_STX_CTX, then the lock is required */
	ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	/* get the destination bgq torus address */
	const union fi_bgq_addr bgq_dst_addr = {.fi=dest_addr};

	size_t xfer_len = 0;
	if (is_contiguous) xfer_len = len;
	else {
		size_t i;
		const struct iovec * iov = (const struct iovec *)buf;
		for (i=0; i<len; ++i) xfer_len += iov[i].iov_len;
	}

	if (!override_flags) tx_op_flags = bgq_ep->tx.op_flags;

	/* busy-wait until a fifo slot is available .. */
	MUHWI_Descriptor_t * send_desc = fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

	if (xfer_len <= FI_BGQ_TOTAL_BUFFERED_RECV) {
		/* eager */

		/* copy the descriptor model into the injection fifo */
		qpx_memcpy64((void*)send_desc, (const void *)&bgq_ep->tx.send.send_model);

		/* set the destination torus address and fifo map */
		send_desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr.uid.fi);
		send_desc->Torus_FIFO_Map = fi_bgq_addr_get_fifo_map(dest_addr);

		send_desc->Message_Length = xfer_len;

		send_desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
			fi_bgq_addr_rec_fifo_id(dest_addr);

		if (is_contiguous && ((tx_op_flags & FI_INJECT) == 0)) {
			fi_bgq_cnk_vaddr2paddr(buf, len, &send_desc->Pa_Payload);
		} else {
			/* locate the payload lookaside slot */
			uint64_t payload_paddr = 0;
			uintptr_t payload_vaddr =
				(uintptr_t) fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
					send_desc, &payload_paddr);
			send_desc->Pa_Payload = payload_paddr;

			if (is_contiguous) {
				if (len) memcpy((void*)payload_vaddr, buf, len);
			} else {
				unsigned i;
				const struct iovec * iov = (const struct iovec *)buf;
				for (i=0; i<len; ++i) {
					memcpy((void*)payload_vaddr, iov[i].iov_base, iov[i].iov_len);
						payload_vaddr += iov[i].iov_len;
				}
			}
		}

		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &send_desc->PacketHeader;
		hdr->pt2pt.send.message_length = xfer_len;
		hdr->pt2pt.ofi_tag = tag;
		hdr->pt2pt.immediate_data = data;

#ifdef FI_BGQ_TRACE
		fprintf(stderr,"eager sending to dest:\n");
		FI_BGQ_ADDR_DUMP(&dest_addr);
#endif
		if (is_msg) {
			fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_EAGER);	/* clear the 'TAG' bit in the packet type */
		}

		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

#ifdef FI_BGQ_REMOTE_COMPLETION
		if (tx_op_flags & (FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)) {

			/*
			 * TODO - this code is buggy and results in a hang at job completion for 'cpi'
			 *
			 * Suspect that remote processes are exiting before the 'request for ack'
			 * remote completion packet is received, then the process that issued the
			 * 'request for ack' messagee will hang because the ack is never received.
			 *
			 * Alternative implementations:
			 *   1. Do not support remote completions on bgq (current)
			 *   2. Support remote completions via rendezvous protocol
			 */

			/* inject the 'remote completion' descriptor */
			send_desc = fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

			/* copy the descriptor model into the injection fifo */
			qpx_memcpy64((void*)send_desc, (const void *)&bgq_ep->tx.send.remote_completion_model);

			/* initialize the completion entry */
			assert(context);
			assert(((uintptr_t)context & 0x07ull) == 0);	/* must be 8 byte aligned */
			union fi_bgq_context * bgq_context = (union fi_bgq_context *)context;
			bgq_context->flags = 0;		/* TODO */
			bgq_context->len = xfer_len;
			bgq_context->buf = NULL;	/* TODO */
			bgq_context->byte_counter = xfer_len;
			bgq_context->tag = tag;

			uint64_t byte_counter_paddr = 0;
			fi_bgq_cnk_vaddr2paddr((const void*)&bgq_context->byte_counter, sizeof(uint64_t), &byte_counter_paddr);

			/* set the destination torus address and fifo map */
			send_desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr.uid.fi);
			send_desc->Torus_FIFO_Map = (uint64_t) bgq_dst_addr.fifo_map;
			send_desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
				fi_bgq_addr_rec_fifo_id(dest_addr);

			hdr = (union fi_bgq_mu_packet_hdr *) &send_desc->PacketHeader;
			hdr->completion.is_local = fi_bgq_addr_is_local(dest_addr);
			hdr->completion.cntr_paddr_rsh3b = byte_counter_paddr >> 3;

			fi_bgq_cq_enqueue_pending(bgq_ep->send_cq, bgq_context, lock_required);

			MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		} else
#endif
		{

			if (tx_op_flags & (FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)) {

#ifdef FI_BGQ_TRACE
                fprintf(stderr,"eager injecting local completion dput\n");
#endif

				/* inject the 'local completion' direct put descriptor */
				send_desc = fi_bgq_spi_injfifo_tail_wait(&bgq_ep->tx.injfifo);

				/* copy the descriptor model into the injection fifo */
				qpx_memcpy64((void*)send_desc, (const void *)&bgq_ep->tx.send.local_completion_model);

				/* initialize the completion entry */
				assert(context);
				assert(((uintptr_t)context & 0x07ull) == 0);	/* must be 8 byte aligned */
				union fi_bgq_context * bgq_context = (union fi_bgq_context *)context;
				bgq_context->flags = 0;		/* TODO */
				bgq_context->len = xfer_len;
				bgq_context->buf = NULL;	/* TODO */
				bgq_context->byte_counter = xfer_len;
				bgq_context->tag = tag;

				uint64_t byte_counter_paddr = 0;
				fi_bgq_cnk_vaddr2paddr((const void*)&bgq_context->byte_counter, sizeof(uint64_t), &byte_counter_paddr);

				send_desc->Pa_Payload =
					MUSPI_GetAtomicAddress(byte_counter_paddr,
						MUHWI_ATOMIC_OPCODE_LOAD_CLEAR);

				fi_bgq_cq_enqueue_pending(bgq_ep->send_cq, bgq_context, lock_required);

				MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);
			}
		}

	} else {
		/* rendezvous */

#ifdef FI_BGQ_TRACE
                fprintf(stderr,"rendezvous sending to dest:\n");
                FI_BGQ_ADDR_DUMP(&dest_addr);
#endif

		assert((tx_op_flags & FI_INJECT) == 0);

		const uint64_t is_local = fi_bgq_addr_is_local(dest_addr);

		/* copy the descriptor model into the injection fifo */
		qpx_memcpy64((void*)send_desc, (const void *)&bgq_ep->tx.send.rzv_model[is_local]);

		/* set the destination torus address and fifo map */
		send_desc->PacketHeader.NetworkHeader.pt2pt.Destination = fi_bgq_uid_get_destination(bgq_dst_addr.uid.fi);
		send_desc->Torus_FIFO_Map = fi_bgq_addr_get_fifo_map(dest_addr);

		send_desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
			fi_bgq_addr_rec_fifo_id(dest_addr);

		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &send_desc->PacketHeader;

		if (is_msg) {
			fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_RENDEZVOUS);
		}

		/* locate the payload lookaside slot */
		uint64_t payload_paddr = 0;
		union fi_bgq_mu_packet_payload *payload = 
			(union fi_bgq_mu_packet_payload *) fi_bgq_spi_injfifo_immediate_payload(&bgq_ep->tx.injfifo,
				send_desc, &payload_paddr);
		send_desc->Pa_Payload = payload_paddr;

		payload->rendezvous.fifo_map = fi_bgq_addr_get_fifo_map(bgq_dst_addr.fi);

		if (is_contiguous) {
			/* only send one mu iov */
			fi_bgq_cnk_vaddr2paddr(buf, len, &payload->rendezvous.mu_iov[0].src_paddr);
			payload->rendezvous.mu_iov[0].message_length = len;
			hdr->pt2pt.rendezvous.niov_minus_1 = 0;
		} else {
			assert(len <= 31);
			size_t i;
			const struct iovec * iov = (const struct iovec *)buf;
			send_desc->Message_Length += (len-1) * sizeof(struct fi_bgq_mu_iov);
			for (i=0; i<len; ++i) {
				fi_bgq_cnk_vaddr2paddr(iov[i].iov_base, iov[i].iov_len, &payload->rendezvous.mu_iov[i].src_paddr);
				payload->rendezvous.mu_iov[i].message_length = iov[i].iov_len;
			}
			hdr->pt2pt.rendezvous.niov_minus_1 = len - 1;
		}

		/* initialize the completion entry */
		assert(context);
		assert(((uintptr_t)context & 0x07ull) == 0);	/* must be 8 byte aligned */
		union fi_bgq_context * bgq_context = (union fi_bgq_context *)context;
		bgq_context->flags = 0;		/* TODO */
		bgq_context->len = xfer_len;
		bgq_context->buf = NULL;	/* TODO */
		bgq_context->byte_counter = xfer_len;
		bgq_context->tag = tag;

		uint64_t byte_counter_paddr = 0;
		fi_bgq_cnk_vaddr2paddr((const void*)&bgq_context->byte_counter, sizeof(uint64_t), &byte_counter_paddr);
		payload->rendezvous.cntr_paddr_rsh3b = byte_counter_paddr >> 3;

		hdr->pt2pt.ofi_tag = tag;
		hdr->pt2pt.immediate_data = data;

		MUSPI_InjFifoAdvanceDesc(bgq_ep->tx.injfifo.muspi_injfifo);

		fi_bgq_cq_enqueue_pending(bgq_ep->send_cq, bgq_context, lock_required);
	}

	/* TODO - if this is a FI_CLASS_STX_CTX, then the lock is required */
	ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
	if (ret) return ret;

	return 0;
}

static inline
ssize_t fi_bgq_send_generic(struct fid_ep *ep,
		const void *buf, size_t len, void *desc,
		fi_addr_t dst_addr, uint64_t tag, void *context,
		int lock_required,
		const unsigned is_msg)
{
	assert(is_msg == 0 || is_msg == 1);
	return fi_bgq_send_generic_flags(ep, buf, len, desc, dst_addr,
		tag, context, 0, lock_required, is_msg,
		1 /* is_contiguous */,
		0 /* do not override flags */,
		0 /* no flags */);
}

/*
 * In FI_PROGRESS_MANUAL mode:
 * The bgq 'recv' implementation is THREAD SAFE and LOCKLESS due to its use of
 * the L2 atomic operations to post the match information to the progress thread.
 * The 'fi_bgq_lock_if_required()' utility function is not used.
 */
static inline
ssize_t fi_bgq_recv_generic(struct fid_ep *ep,
	       	void *buf, size_t len, void *desc,
		fi_addr_t src_addr, uint64_t tag, uint64_t ignore, void *context,
		const int lock_required,
		const uint64_t is_msg)
{
	assert(is_msg == 0 || is_msg == 1);
	struct fi_bgq_ep *bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	const uint64_t rx_op_flags = bgq_ep->rx.post.op_flags;

	assert(context);
	assert(((uintptr_t)context & 0x07ull) == 0);	/* must be 8 byte aligned */
	union fi_bgq_context * bgq_context = (union fi_bgq_context *)context;
	bgq_context->flags = rx_op_flags;
	bgq_context->len = len;
	bgq_context->buf = buf;
	bgq_context->src_addr = src_addr;

#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_recv_generic from source addr:\n");
	FI_BGQ_ADDR_DUMP(&bgq_context->src_addr);
#endif

	bgq_context->tag = tag;
	bgq_context->ignore = ignore;
	bgq_context->byte_counter = (uint64_t)-1;

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* constant expression will compile out */

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
		if (ret) return ret;
#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_recv_generic calling fi_bgq_ep_progress_manual_recv_fast:\n");
#endif

		fi_bgq_ep_progress_manual_recv_fast(bgq_ep, is_msg, context);

		ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
		if (ret) return ret;

	} else {

		/* the *only* difference between a 'tagged' and 'non-tagged' recv is
		 * the L2 atomic fifo used to post the receive information */
		struct l2atomic_fifo_producer * fifo = &bgq_ep->rx.post.match[is_msg];

		uint64_t context_rsh3b = (uint64_t)context >> 3;
		while (l2atomic_fifo_produce(fifo, context_rsh3b) != 0);	/* spin loop! */
	}

	return 0;
}

/*
 * In FI_PROGRESS_AUTO mode:
 * The bgq 'recv' implementation is THREAD SAFE and LOCKLESS due to its use of
 * the L2 atomic operations to post the match information to the progress thread.
 * The 'fi_bgq_lock_if_required()' utility function is not used.
 *
 * \note The bgq provider asserts the following mode bits which affect
 * 	the behavior of this routine:
 *
 * 	- 'FI_ASYNC_IOV' mode bit which requires the application to maintain
 * 	  the 'msg->msg_iov' iovec array until the operation completes
 *
 * 	- 'FI_LOCAL_MR' mode bit which allows the provider to ignore the 'desc'
 * 	  parameter .. no memory regions are required to access the local
 * 	  memory
 */
static inline
ssize_t fi_bgq_recvmsg_generic(struct fid_ep *ep,
		const struct fi_msg *msg, uint64_t flags,
		int lock_required)
{
	struct fi_bgq_ep *bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

	uint64_t context_rsh3b = 0;
	uint64_t rx_op_flags = 0;

	if (flags | FI_MULTI_RECV) {

		assert(msg->context);
		assert(((uintptr_t)msg->context & 0x07ull) == 0);	/* must be 8 byte aligned */
		union fi_bgq_context * bgq_context =
			(union fi_bgq_context *) msg->context;

		uint64_t len = msg->msg_iov[0].iov_len;
		void * base = msg->msg_iov[0].iov_base;

		assert(msg->iov_count == 1);
		assert(base != NULL);
		if ((uintptr_t)base & 0x07ull) {
			uintptr_t new_base = (((uintptr_t)base + 8) & (~0x07ull));
			len -= (new_base - (uintptr_t)base);
			base = (void *)new_base;
		}
		assert(((uintptr_t)base & 0x07ull) == 0);
		assert(len >= (sizeof(union fi_bgq_context) + bgq_ep->rx.min_multi_recv));
		bgq_context->flags = FI_MULTI_RECV;
		bgq_context->len = len - sizeof(union fi_bgq_context);
		bgq_context->buf = (void *)((uintptr_t)base + sizeof(union fi_bgq_context));
		bgq_context->src_addr = msg->addr;
		bgq_context->byte_counter = 0;
		bgq_context->multi_recv_next = (union fi_bgq_context *)base;
		bgq_context->ignore = (uint64_t)-1;

		context_rsh3b = (uint64_t)bgq_context >> 3;
		rx_op_flags = flags;

	} else if (msg->iov_count == 0) {

		assert(msg->context);
		assert(((uintptr_t)msg->context & 0x07ull) == 0);	/* must be 8 byte aligned */

		union fi_bgq_context * bgq_context =
			(union fi_bgq_context *) msg->context;
		bgq_context->flags = flags;
		bgq_context->len = 0;
		bgq_context->buf = NULL;
		bgq_context->src_addr = msg->addr;
		bgq_context->tag = 0;
		bgq_context->ignore = (uint64_t)-1;
		bgq_context->byte_counter = (uint64_t)-1;

		context_rsh3b = (uint64_t)bgq_context >> 3;
		rx_op_flags = flags;

	} else if (msg->iov_count == 1) {
		assert(msg->context);
		assert(((uintptr_t)msg->context & 0x07ull) == 0);	/* must be 8 byte aligned */

		union fi_bgq_context * bgq_context =
			(union fi_bgq_context *) msg->context;
		bgq_context->flags = flags;
		bgq_context->len = msg->msg_iov[0].iov_len;
		bgq_context->buf = msg->msg_iov[0].iov_base;
		bgq_context->src_addr = msg->addr;
		bgq_context->tag = 0;
		bgq_context->ignore = (uint64_t)-1;
		bgq_context->byte_counter = (uint64_t)-1;

		context_rsh3b = (uint64_t)bgq_context >> 3;
		rx_op_flags = flags;

	} else {
		struct fi_bgq_context_ext * ext;
		posix_memalign((void**)&ext, 32, sizeof(struct fi_bgq_context_ext));

		ext->bgq_context.flags = flags | FI_BGQ_CQ_CONTEXT_EXT;
		ext->bgq_context.byte_counter = (uint64_t)-1;
		ext->bgq_context.src_addr = msg->addr;
		ext->bgq_context.tag = 0;
		ext->bgq_context.ignore = (uint64_t)-1;
		ext->msg.op_context = (struct fi_context *)msg->context;
		ext->msg.iov_count = msg->iov_count;
		ext->msg.iov = (struct iovec *)msg->msg_iov;

		context_rsh3b = (uint64_t)ext >> 3;
		rx_op_flags = flags | FI_BGQ_CQ_CONTEXT_EXT;
		if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* constant expression will compile out */

			int ret;
			ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
			if (ret) return ret;

			fi_bgq_ep_progress_manual_recv(bgq_ep,
				1, /* is_msg */
				(union fi_bgq_context *)(context_rsh3b << 3),
				rx_op_flags,
				1  /* is_context_ext */);

			ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
			if (ret) return ret;

			return 0;
		}
	}

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* constant expression will compile out */

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
		if (ret) return ret;

		fi_bgq_ep_progress_manual_recv(bgq_ep,
			1, /* is_msg */
			(union fi_bgq_context *)(context_rsh3b << 3),
			rx_op_flags,
			0  /* is_context_ext */);

		ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
		if (ret) return ret;

	} else {

		/* the *only* difference between a 'tagged' and 'non-tagged' recv is
		 * the L2 atomic fifo used to post the receive information */
		struct l2atomic_fifo_producer * fifo = &bgq_ep->rx.post.match[1];	/* TODO - use enum */

		while (l2atomic_fifo_produce(fifo, context_rsh3b) != 0);		/* spin loop! */

	}

	return 0;
}


static inline
ssize_t fi_bgq_injectdata_generic(struct fid_ep *ep,
		const void *buf, size_t len, uint64_t data,
		fi_addr_t dst_addr, uint64_t tag,
		int lock_required,
		const unsigned is_msg)
{
	return fi_bgq_inject_generic(ep, buf, len, dst_addr, tag, data,
			lock_required, is_msg);
}


static inline
ssize_t fi_bgq_senddata_generic(struct fid_ep *ep,
		const void *buf, size_t len, void *desc, uint64_t data,
		fi_addr_t dst_addr, uint64_t tag, void *context,
		int lock_required,
		const unsigned is_msg)
{

	assert(is_msg == 0 || is_msg == 1);
	return fi_bgq_send_generic_flags(ep, buf, len, desc, dst_addr,
		tag, context, data, lock_required, is_msg,
		1 /* is_contiguous */,
		0 /* do not override flags */,
		0 /* no flags */);
}


static inline
ssize_t fi_bgq_inject(struct fid_ep *ep,
		const void *buf,
		size_t len,
		fi_addr_t dest_addr,
		int lock_required)
{
	return fi_bgq_inject_generic(ep, buf, len, dest_addr, 0, 0,
			lock_required, 1);
}


/*
 * Declare specialized functions that qualify for FABRIC_DIRECT.
 * - No locks
 */

#define FI_BGQ_MSG_FABRIC_DIRECT_LOCK		0

FI_BGQ_MSG_SPECIALIZED_FUNC(FI_BGQ_MSG_FABRIC_DIRECT_LOCK)


#ifdef FABRIC_DIRECT
#define fi_send(ep, buf, len, desc, dest_addr, context)			\
	(FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(send,				\
			FI_BGQ_MSG_FABRIC_DIRECT_LOCK)			\
	(ep, buf, len, desc, dest_addr, context))

#define fi_recv(ep, buf, len, desc, src_addr, context)			\
	(FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(recv,				\
			FI_BGQ_MSG_FABRIC_DIRECT_LOCK)			\
	(ep, buf, len, desc, src_addr, context))

#define fi_inject(ep, buf, len, dest_addr)				\
	(FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(inject,			\
			FI_BGQ_MSG_FABRIC_DIRECT_LOCK)			\
	(ep, buf, len, dest_addr))

#define fi_recvmsg(ep, msg, flags)					\
	(FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(recvmsg,			\
			FI_BGQ_MSG_FABRIC_DIRECT_LOCK)			\
	(ep, msg, flags))

#define fi_senddata(ep, buf, len, desc, data, dest_addr, context)	\
	(FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(senddata,			\
			FI_BGQ_MSG_FABRIC_DIRECT_LOCK)			\
	(ep, buf, len, desc, data, dest_addr, context))

#define fi_injectdata(ep, buf, len, data, dest_addr)			\
	(FI_BGQ_MSG_SPECIALIZED_FUNC_NAME(injectdata,			\
			FI_BGQ_MSG_FABRIC_DIRECT_LOCK)			\
	(ep, buf, len, data, dest_addr))

static inline ssize_t
fi_sendmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	return ep->msg->sendmsg(ep, msg, flags);
}

static inline ssize_t
fi_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, void *context)
{
	return ep->msg->sendv(ep, iov, desc, count, dest_addr, context);
}

static inline int
fi_enable(struct fid_ep *ep)
{
	return ep->fid.ops->control(&ep->fid, FI_ENABLE, NULL);
}

static inline int
fi_cancel(fid_t fid, void *context)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->ops->cancel(fid, context);
}

static inline int
fi_endpoint(struct fid_domain *domain, struct fi_info *info,
	    struct fid_ep **ep, void *context)
{
	return domain->ops->endpoint(domain, info, ep, context);
}

static inline int
fi_scalable_ep(struct fid_domain *domain, struct fi_info *info,
	    struct fid_ep **sep, void *context)
{
	return domain->ops->scalable_ep(domain, info, sep, context);
}

static inline int
fi_setopt(fid_t fid, int level, int optname,
		const void *optval, size_t optlen)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->ops->setopt(fid, level, optname, optval, optlen);
}

static inline int
fi_getopt(fid_t fid, int level, int optname,
		void *optval, size_t *optlen)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->ops->getopt(fid, level, optname, optval, optlen);
}

static inline int
fi_tx_context(struct fid_ep *ep, int index, struct fi_tx_attr *attr,
	      struct fid_ep **tx_ep, void *context)
{
	return ep->ops->tx_ctx(ep, index, attr, tx_ep, context);
}

static inline int
fi_rx_context(struct fid_ep *ep, int index, struct fi_rx_attr *attr,
	      struct fid_ep **rx_ep, void *context)
{
	return ep->ops->rx_ctx(ep, index, attr, rx_ep, context);
}

static inline int
fi_ep_bind(struct fid_ep *ep, struct fid *bfid, uint64_t flags)
{
	return ep->fid.ops->bind(&ep->fid, bfid, flags);
}

static inline int
fi_scalable_ep_bind(struct fid_ep *sep, struct fid *bfid, uint64_t flags)
{
	return sep->fid.ops->bind(&sep->fid, bfid, flags);
}

static inline int
fi_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr,
	       struct fid_stx **stx, void *context)
{
	return domain->ops->stx_ctx(domain, attr, stx, context);
}

static inline int
fi_ep_alias(struct fid_ep *ep, struct fid_ep **alias_ep, uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int
fi_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
	     struct fid_pep **pep, void *context)
{
	return fabric->ops->passive_ep(fabric, info, pep, context);
}

static inline int fi_pep_bind(struct fid_pep *pep, struct fid *bfid, uint64_t flags)
{
	return pep->fid.ops->bind(&pep->fid, bfid, flags);
}

#endif

#endif /* _FI_BGQ_DIRECT_EP_H_ */
