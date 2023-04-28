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
#ifndef _FI_PROV_OPX_HFI1_TRANSPORT_H_
#define _FI_PROV_OPX_HFI1_TRANSPORT_H_

#ifndef FI_OPX_FABRIC_HFI1
#error "fabric selection #define error"
#endif

#include "rdma/opx/fi_opx_hfi1.h"
#include <ofi_list.h>
#include <rdma/hfi/hfi1_user.h>

/*
 * ==== NOTE_COMPLETION_TYPES ====
 *
 * FI_INJECT_COMPLETE generates the completion entry when the
 * source buffer can be reused. This can be immedately done if
 * the entire source buffer is copied into the reliability
 * replay buffer(s). Otherwise the completion should not be
 * generated until the reliability protocol completes.
 *
 * FI_TRANSMIT_COMPLETE completion entry should only be generated
 * when "the operation is no longer dependent on local resources".
 * Does this means that it should be delivered only when the
 * reliability protocol has completed? If so, then the completion
 * may be delayed significantly, or the reliability protocol
 * needs to be enhanced for the target to do an 'immediate ack'
 * when this packet is received. Regardless, MPICH only uses
 * this completion type when performing a 'dynamic process
 * disconnect' so it is not critical for this completion type
 * to have good performance at this time.
 *
 * FI_DELIVERY_COMPLETE is not supposed to generate a completion
 * event until the send has been "processed by the destination
 * endpoint(s)". The reliability protocol has nothing to do with
 * that acknowledgement.
 *
 * OPX supports FI_DELIVERY_COMPLETE when required but will use
 * FI_INJECT_COMPLETE when possible.
 */

// Function for performing FI_INJECT_COMPLETIONs.
__OPX_FORCE_INLINE__
void fi_opx_ep_tx_cq_inject_completion(struct fid_ep *ep,
				void *context,
				const size_t len,
				const int lock_required,
				const uint64_t tag,
				const uint64_t caps)
{
	/*
	 * This is the default completion type, used for all
	 * sends that do not require delivery completion.
	 *
	 * See NOTE_COMPLETION_TYPES for more information.
	 */

	/* initialize the completion entry */
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	assert(context);
	assert(((uintptr_t)context & 0x07ull) == 0);	/* must be 8 byte aligned */
	union fi_opx_context * opx_context = (union fi_opx_context *)context;
	opx_context->flags =  FI_SEND | (caps & (FI_TAGGED | FI_MSG));
	opx_context->len = len;
	opx_context->buf = NULL;		/* receive data buffer */
	opx_context->byte_counter = 0;
	opx_context->tag = tag;
	opx_context->next = NULL;

	if (lock_required) { fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__); abort(); }
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== TX CQ COMPLETION QUEUED\n");
	fi_opx_context_slist_insert_tail(opx_context, opx_ep->tx->cq_completed_ptr);
}

// faster than memcpy() for this amount of data.
static inline void fi_opx_copy_scb(volatile uint64_t dest[8], uint64_t source[8])
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = source[2];
	dest[3] = source[3];
	dest[4] = source[4];
	dest[5] = source[5];
	dest[6] = source[6];
	dest[7] = source[7];
}

// Use this to fill out an SCB before the data is copied to local storage.
// (The local copy is usually used for setting up replay buffers or for log
// messages.)
static inline void fi_opx_set_scb(volatile uint64_t scb[8], uint64_t local[8],
	uint64_t d0, uint64_t d1, uint64_t d2, uint64_t d3,
	uint64_t d4, uint64_t d5, uint64_t d6, uint64_t d7)
{
	scb[0]   = d0;
	scb[1]   = d1;
	scb[2]   = d2;
	scb[3]   = d3;
	scb[4]   = d4;
	scb[5]   = d5;
	scb[6]   = d6;
	scb[7]   = d7;
	local[0] = d0;
	local[1] = d1;
	local[2] = d2;
	local[3] = d3;
	local[4] = d4;
	local[5] = d5;
	local[6] = d6;
	local[7] = d7;
}

//fi_opx_duff_copy --A function to handle a fast memcpy of non-trival byte length (like 8 or 16) in the scb copy crtical path
// Pre: Only called by fi_opx_set_scb_special()
// Pre: NEVER call with length 0, 8, or 16 (Use long = long for 8 and 16) 
// Post: len number of bytes are touched in both buffers. 
// Post: arg0 buffer will be equal to arg1 buffer for arg2 bytes 
__OPX_FORCE_INLINE__
void fi_opx_duff_copy(char *to, const char *from, int64_t len) {
        //memcpy(to, from, len);
        //return;
        assert(len > 0);
        assert(len < 16);
        assert(len != 8);
		switch (len & 15) { // (len % 16) power of 2
        // Does not handle 0, save some code gen
		case 15: *to++ = *from++; __attribute__ ((fallthrough));
		case 14: *to++ = *from++; __attribute__ ((fallthrough));
		case 13: *to++ = *from++; __attribute__ ((fallthrough));
		case 12: *to++ = *from++; __attribute__ ((fallthrough));
		case 11: *to++ = *from++; __attribute__ ((fallthrough));
		case 10: *to++ = *from++; __attribute__ ((fallthrough));
		case 9:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 8:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 7:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 6:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 5:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 4:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 3:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 2:	 *to++ = *from++; __attribute__ ((fallthrough));
		case 1:	 *to++ = *from++;
        }
}


// Use this to fill out an SCB before the data is copied to local storage.
// (The local copy is usually used for setting up replay buffers or for log
// messages.) 
//
// This version embeds up to 16 bytes of immediate data into the SCB. 
__OPX_FORCE_INLINE__
void fi_opx_set_scb_special(volatile uint64_t scb[8], uint64_t local[8],
	uint64_t d0, uint64_t d1, uint64_t d2, uint64_t d3,
	uint64_t d4, const void *buf, size_t len, uint64_t d7)
{
	// the purpose of this is to quickly copy the contents of buf into 
	// the 5th and 6th DWORDs of the SCB and the local copy.
	switch (len) {
		case 0:
			local[5] = 0;
			local[6] = 0;
			break;
		case 1:
			local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 1);
			local[6] = 0;
			break;
		case 2:
            local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 2);
            local[6] = 0;
            break;
		case 3:
            local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 3);
            local[6] = 0;
            break;
		case 4:
            local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 4);
            local[6] = 0;
            break;
		case 5:
            local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 5);
            local[6] = 0;
            break;
		case 6:
            local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 6);
            local[6] = 0;
            break;
		case 7:
			local[5] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 7);
			local[6] = 0;
			break;
		case 8:
			local[5] = *((uint64_t*)buf);
			local[6] = 0;
			break;
		case 9:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 9);
            break;
		case 10:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 10);
            break;
		case 11:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 11);
            break;
		case 12:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 12);
            break;
		case 13:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 13);
            break;
		case 14:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 14);
            break;
		case 15:
            local[6] = 0;
            fi_opx_duff_copy((char*)&local[5], buf, 15);
            break;
		case 16:
			local[5] = *((uint64_t*)buf);
			local[6] = *((uint64_t*)buf+1);
		break;
		default:
			fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__); abort();
		break;
	}

	scb[0]   = d0;
	scb[1]   = d1;
	scb[2]   = d2;
	scb[3]   = d3;
	scb[4]   = d4;
	scb[5]   = local[5];
	scb[6]   = local[6];
	scb[7]   = d7;

	local[0] = d0;
	local[1] = d1;
	local[2] = d2;
	local[3] = d3;
	local[4] = d4;
	// local[5] = d5;
	// local[6] = d6;
	local[7] = d7;
}

// Use this to fill out an SCB before the data is copied to local storage.
// (The local copy is usually used for setting up replay buffers or for log
// messages.)
static inline void fi_opx_set_scb_special2(volatile uint64_t scb[8], uint64_t local[8],
	uint64_t d0, uint64_t d1, uint64_t d2, uint64_t d3,
	uint64_t d4, uint64_t d5, const void *buf, size_t len, uint64_t d7)
{
	local[6] = 0;
	memcpy((void*)&local[6], buf, len);

	scb[0]   = d0;
	scb[1]   = d1;
	scb[2]   = d2;
	scb[3]   = d3;
	scb[4]   = d4;
	scb[5]   = d5;
	scb[6]   = local[6];
	scb[7]   = d7;
	local[0] = d0;
	local[1] = d1;
	local[2] = d2;
	local[3] = d3;
	local[4] = d4;
	local[5] = d5;
	// local[6] = d6;
	local[7] = d7;
}

void fi_opx_hfi1_rx_rzv_rts (struct fi_opx_ep *opx_ep,
			     const void * const hdr, const void * const payload,
			     const uint8_t u8_rx, const uint64_t niov,
			     uintptr_t origin_byte_counter_vaddr,
			     uintptr_t target_byte_counter_vaddr,
			     const uintptr_t dst_vaddr,
			     const uint64_t immediate_data,
			     const uint64_t immediate_end_block_count,
			     const struct iovec* src_iov,
			     uint8_t opcode,
			     const unsigned is_intranode,
			     const enum ofi_reliability_kind reliability,
				 const uint32_t u32_extended_rx);

union fi_opx_hfi1_deferred_work* fi_opx_hfi1_rx_rzv_cts  (struct fi_opx_ep * opx_ep,
		struct fi_opx_mr * opx_mr,
		const void * const hdr, const void * const payload,
		size_t payload_bytes_to_copy,
		const uint8_t u8_rx, const uint8_t origin_rs,
		const uint32_t niov,
		const struct fi_opx_hfi1_dput_iov * const dput_iov,
		const uint8_t op,
		const uint8_t dt,
		const uintptr_t target_byte_counter_vaddr,
		uint64_t * origin_byte_counter,
		uint32_t op_kind,
		void (*completion_action)(union fi_opx_hfi1_deferred_work * work_state),
		const unsigned is_intranode,
		const enum ofi_reliability_kind reliability,
		const uint32_t u32_extended_rx
);

union fi_opx_hfi1_deferred_work;
struct fi_opx_work_elem {
	struct slist_entry slist_entry;
	int (*work_fn)(union fi_opx_hfi1_deferred_work * work_state);
	void (*completion_action)(union fi_opx_hfi1_deferred_work * work_state);
	uint8_t unused[6];
	bool complete;
	bool low_priority;
	union fi_opx_hfi1_packet_payload *payload_copy;
};


struct fi_opx_hfi1_dput_params {
	struct fi_opx_work_elem work_elem;
	struct fi_opx_ep * opx_ep;
	struct fi_opx_mr * opx_mr;
	uint64_t lrh_dlid;
	uint64_t slid;
	struct fi_opx_hfi1_dput_iov *dput_iov;
	void *fetch_vaddr;
	void *compare_vaddr;
	struct fi_opx_completion_counter *cc;
	struct fi_opx_completion_counter *user_cc;
	struct fi_opx_hfi1_sdma_work_entry *sdma_we;
	struct slist sdma_reqs;
	struct iovec tid_iov;
	uintptr_t target_byte_counter_vaddr;
	uint64_t *origin_byte_counter;
	uint64_t key;
	uint64_t bytes_sent;
	uint32_t niov;
	uint32_t cur_iov;
	uint32_t opcode;
	uint32_t payload_bytes_for_iovec;
	uint32_t tididx;
	uint32_t tidlen_consumed;
	uint32_t tidlen_remaining;
	enum ofi_reliability_kind reliability;
	uint16_t origin_rs;
	uint16_t sdma_reqs_used;
	bool is_intranode;
	bool delivery_completion;
	bool use_tid;
	bool use_expected_opcode;
	uint8_t u8_rx;
	uint32_t u32_extended_rx;
	uint8_t dt;
	uint8_t op;
	uint8_t	target_hfi_unit;
	uint8_t inject_data[FI_OPX_HFI1_PACKET_IMM];
	/* Either FI_OPX_MAX_DPUT_IOV iov's or
	   1 iov and FI_OPX_MAX_DPUT_TIDPAIRS tidpairs */
	union {
		struct fi_opx_hfi1_dput_iov iov[FI_OPX_MAX_DPUT_IOV];
		struct {
			struct fi_opx_hfi1_dput_iov reserved;/* skip 1 iov */
			uint32_t tidpairs[FI_OPX_MAX_DPUT_TIDPAIRS];
		};
	};
};

struct fi_opx_hfi1_rx_rzv_rts_params {
	struct fi_opx_work_elem work_elem;
	struct fi_opx_ep *opx_ep;
	uint64_t lrh_dlid;
	uint64_t slid;
	uint16_t origin_rs;
	uint16_t origin_rx;
	uint8_t u8_rx;
	uint32_t u32_extended_rx;
	uint64_t niov;
	uintptr_t origin_byte_counter_vaddr;
	uintptr_t target_byte_counter_vaddr;
	uintptr_t dst_vaddr; /* bumped past immediate data */
	uint64_t immediate_data;
	uint64_t immediate_end_block_count;
	uint32_t ntidpairs;
	uint8_t opcode;
	uint8_t fallback_opcode;
	unsigned is_intranode;
	enum ofi_reliability_kind reliability;
	uint8_t	target_hfi_unit;
	/* Either FI_OPX_MAX_DPUT_IOV iov's or
	   1 iov and FI_OPX_MAX_DPUT_TIDPAIRS tidpairs */
	union {
		struct iovec src_iov[FI_OPX_MAX_DPUT_IOV];
		struct {
			struct iovec reserved;/* skip 1 iov */
			uint32_t tidpairs[FI_OPX_MAX_DPUT_TIDPAIRS];
		};
	};
};

struct fi_opx_hfi1_rx_dput_fence_params {
	struct fi_opx_work_elem work_elem;
	struct fi_opx_ep *opx_ep;
	struct fi_opx_completion_counter *cc;
	uint64_t lrh_dlid;
	uint64_t bth_rx;
	uint64_t bytes_to_fence;
	uint8_t u8_rx;
	uint32_t u32_extended_rx;
	uint8_t	target_hfi_unit;
};

struct fi_opx_hfi1_rx_readv_params {
	struct fi_opx_work_elem work_elem;
	struct fi_opx_ep *opx_ep;
	struct iovec iov;
	size_t niov;
	union fi_opx_addr opx_target_addr;
	struct fi_opx_completion_counter *cc;
	uint64_t addr_offset;
	uint64_t key;
	uint64_t dest_rx;
	uint32_t u32_extended_rx;
	uint64_t lrh_dlid;
	uint64_t bth_rx;
	uint64_t pbc_dws;
	uint64_t lrh_dws;
	uint64_t op;
	uint64_t dt;
	uint32_t opcode;
	enum ofi_reliability_kind reliability;
	bool is_intranode;
};

union fi_opx_hfi1_deferred_work {
	struct fi_opx_work_elem work_elem;
	struct fi_opx_hfi1_dput_params dput;
	struct fi_opx_hfi1_rx_rzv_rts_params rx_rzv_rts;
	struct fi_opx_hfi1_rx_dput_fence_params fence;
	struct fi_opx_hfi1_rx_readv_params readv;
};

int opx_hfi1_do_dput_fence(union fi_opx_hfi1_deferred_work *work);
void opx_hfi1_dput_fence(struct fi_opx_ep *opx_ep,
			const union fi_opx_hfi1_packet_hdr *const hdr,
			const uint8_t u8_rx,
			const uint32_t u32_extended_rx);

int fi_opx_hfi1_do_dput (union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma (union fi_opx_hfi1_deferred_work *work);

__OPX_FORCE_INLINE__
void fi_opx_hfi1_memcpy8(void *restrict dest, const void *restrict src, size_t n) {
	const size_t qw_to_copy = n >> 3;
	const size_t remain = n & 0x07ul;
	ssize_t idx;
	volatile uint64_t *d = dest;
	const uint64_t *s = src;

	for(idx = 0; idx < qw_to_copy; idx++) {
		*d++ = *s++;
	}
	if(remain == 0) {
		return;
	}

	union tmp_t {
		uint8_t byte[8];
		volatile uint64_t qw;
	} temp, *s8;
	assert(sizeof(temp.byte) == sizeof(temp.qw));
	assert(sizeof(temp.qw) == sizeof(temp));
	assert(sizeof(temp.byte) == sizeof(temp));
	temp.qw = 0ULL;
	s8 = (union tmp_t *) s;
	for(idx=0; idx < remain; idx++) {
		temp.byte[idx] = s8->byte[idx];
	}
	*d = temp.qw;
}

/*
 *  Force a credit return by sending a "no-op" packet where the
 *  PBC has the PbcCreditReturn bit set. The HAS describes the
 *  effect of setting this bit is that *only* the credit used
 *  to send this particular packet is returned immediately. However,
 *  in practice, in order to return this credit, all pending credits
 *  must also be returned.
 */
__OPX_FORCE_INLINE__
void fi_opx_force_credit_return(struct fid_ep *ep,
				fi_addr_t dest_addr,
				const uint64_t dest_rx,
				const uint64_t caps)
{

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const uint64_t bth_rx = ((uint64_t)dest_rx) << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(dest_addr);
	const uint64_t pbc_dws = 16;
	const uint16_t lrh_dws = htons(pbc_dws-1);

	const uint64_t force_credit_return = FI_OPX_HFI1_PBC_CR_MASK << FI_OPX_HFI1_PBC_CR_SHIFT;

	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	uint64_t loop = 0;

	/*
	 * If we can't even get a single credit to write a no-op packet, try a few times,
	 * but not too many. If there are zero credits available for sending, chances are
	 * credits will be returned soon naturally anyway, and sending a no-op packet
	 * forcing a credit return would just add unnecessary traffic.
	 */
	uint64_t available_credits = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 1);
	while (OFI_UNLIKELY(available_credits < 1)) {
		if (loop++ & 0x10) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			return;
		}
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		available_credits = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 1);
	}

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	uint64_t tmp[8];

	fi_opx_set_scb(scb, tmp,
		opx_ep->tx->send.qw0 | pbc_dws | force_credit_return,
		opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),
		opx_ep->tx->send.hdr.qw[1] | bth_rx | ((uint64_t)FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP << 48) | (uint64_t)FI_OPX_HFI_BTH_OPCODE_UD ,
		opx_ep->tx->send.hdr.qw[2],
		opx_ep->tx->send.hdr.qw[3],
		opx_ep->tx->send.hdr.qw[4],
		0, 0);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_inject (struct fid_ep *ep,
		const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
		const uint32_t data, int lock_required,
		const uint64_t dest_rx,
		const uint64_t caps,
		const enum ofi_reliability_kind reliability) {

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr = { .fi = dest_addr };

	const uint64_t bth_rx = dest_rx << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(addr.fi);

	if (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == FI_LOCAL_COMM) ||	/* compile-time constant expression */
		(((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == (FI_LOCAL_COMM | FI_REMOTE_COMM)) &&
			(opx_ep->tx->inject.hdr.stl.lrh.slid == addr.uid.lid))) {

		uint64_t pos;
		ssize_t rc;
		union fi_opx_hfi1_packet_hdr * const hdr =
			opx_shm_tx_next(&opx_ep->tx->shm, dest_rx, &pos,
				opx_ep->daos_info.hfi_rank_enabled, opx_ep->daos_info.rank,
				opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) return rc;

		hdr->qw[0] = opx_ep->tx->inject.hdr.qw[0] | lrh_dlid;

		hdr->qw[1] = opx_ep->tx->inject.hdr.qw[1] | bth_rx | (len << 48) |
			((caps & FI_MSG) ? /* compile-time constant expression */
				(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_INJECT :
				(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_INJECT);

		hdr->qw[2] = opx_ep->tx->inject.hdr.qw[2];

		hdr->qw[3] = opx_ep->tx->inject.hdr.qw[3] | (((uint64_t)data) << 32);

		hdr->qw[4] = 0;
		hdr->qw[5] = 0;
		fi_opx_hfi1_memcpy8((void*)&hdr->qw[4], buf, len);

		hdr->qw[6] = tag;

		opx_shm_tx_advance(&opx_ep->tx->shm, (void*)hdr, pos);

		return FI_SUCCESS;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== INJECT, HFI (begin)\n");

	/* first check for sufficient credits to inject the entire packet */

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 1) < 1)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 1) < 1) {
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay * replay = (reliability != OFI_RELIABILITY_KIND_NONE)?
	fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false) : NULL;
	if(replay == NULL) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	union fi_opx_reliability_tx_psn *psn_ptr;
	const int64_t psn = (reliability != OFI_RELIABILITY_KIND_NONE) ?	/* compile-time constant expression */
		fi_opx_reliability_tx_next_psn(ep, &opx_ep->reliability->state, addr.uid.lid,
						dest_rx, addr.reliability_rx, &psn_ptr, 1) :
		0;
	if(OFI_UNLIKELY(psn == -1)) {
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
		return -FI_EAGAIN;
	}


	if (lock_required) { fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__); abort(); }

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	uint64_t tmp[8] = {0};

	fi_opx_set_scb_special(scb, tmp,
		opx_ep->tx->inject.qw0 | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
		opx_ep->tx->inject.hdr.qw[0] | lrh_dlid,

		opx_ep->tx->inject.hdr.qw[1] | bth_rx | (len << 48) |
				((caps & FI_MSG) ? /* compile-time constant expression */
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_INJECT :
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_INJECT),

		opx_ep->tx->inject.hdr.qw[2] | psn,
		opx_ep->tx->inject.hdr.qw[3] | (((uint64_t)data) << 32),
		buf, len, tag);

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* consume one credit */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		replay->scb.qw0 = tmp[0];
		replay->scb.hdr.qw[0] = tmp[1];
		replay->scb.hdr.qw[1] = tmp[2];
		replay->scb.hdr.qw[2] = tmp[3];
		replay->scb.hdr.qw[3] = tmp[4];
		replay->scb.hdr.qw[4] = tmp[5];
		replay->scb.hdr.qw[5] = tmp[6];
		replay->scb.hdr.qw[6] = tmp[7];

		fi_opx_reliability_client_replay_register_no_update(&opx_ep->reliability->state, addr.uid.lid, addr.reliability_rx, dest_rx, psn_ptr, replay, reliability);
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== INJECT, HFI (end)\n");

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
bool fi_opx_hfi1_fill_from_iov8(const struct iovec *iov,   /* In:  iovec array */
								size_t niov,               /* In:  total iovecs */
								volatile const void *buf,  /* In:  target buffer to fill */
								ssize_t  *len,             /* In/Out:  buffer length to fill */
								ssize_t  *iov_idx,         /* In/Out:  start index, returns end */
								ssize_t  *iov_base_offset) /* In/Out:  start offset, returns offset */
{
	ssize_t idx = *iov_idx;
	ssize_t iov_offset = *iov_base_offset;
	ssize_t dst_len = *len;
	ssize_t dst_buff_offset = 0;

	for(; idx < niov; idx++) {
		const uint8_t *src_buf = (uint8_t*)iov[idx].iov_base + iov_offset;
		ssize_t src_len = iov[idx].iov_len - iov_offset;
		assert(src_len > 0);
		ssize_t to_copy = MIN(src_len, dst_len);
		assert(to_copy > 0);
		uint8_t *dst_buf = (uint8_t*)buf + dst_buff_offset;

		fi_opx_hfi1_memcpy8(dst_buf, src_buf, to_copy);

		dst_buff_offset += to_copy;
		dst_len -= to_copy;
		iov_offset += to_copy;

		// Terminates when dest buffer is filled
		if(dst_len == 0) {
			*len = dst_len;
			*iov_idx = idx;
			if(src_len == 0) {
				*iov_base_offset = 0;
				(*iov_idx)++;
			} else {
				*iov_base_offset = iov_offset;
			}
			return true;
		}

		// reset the iovec offset when starting a new iovec
		assert((src_len-=to_copy) == 0);
		iov_offset = 0;
	}
	// update state variables
	*len = dst_len;
	*iov_idx = idx;
	*iov_base_offset = iov_offset;
	if(idx == niov) {
		// returns true when iovec array is done
		// TODO:  do we want to assert this should never happen
		// We should never have any dst_len left, instead of taking
		// this branch, make the user contractually pass us the
		// appropriate length?
		// assert(dst_len == 0); ????
		return true;
	}
	return false;
}

static inline void fi_opx_shm_poll_many(struct fid_ep *ep, const int lock_required);

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_sendv_egr(struct fid_ep *ep, const struct iovec *iov, size_t niov,
				 size_t total_len, void *desc, fi_addr_t dest_addr, uint64_t tag,
				 void *context, const uint32_t data, int lock_required,
				 const unsigned override_flags, uint64_t tx_op_flags,
				 const uint64_t dest_rx, const uint64_t caps,
				 const enum ofi_reliability_kind reliability,
				 const uint64_t do_cq_completion)
{
	assert(lock_required == 0);
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr = { .fi = dest_addr };
	const size_t xfer_bytes_tail = total_len & 0x07ul;
	const size_t payload_qws_total = total_len >> 3;
	const size_t payload_qws_tail = payload_qws_total & 0x07ul;

	const uint64_t bth_rx = ((uint64_t)dest_rx) << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(dest_addr);
	uint16_t full_block_credits_needed = (total_len >> 6);
	uint16_t total_credits_needed = 1 +  /* packet header */
		full_block_credits_needed;      /* full blocks */

	if(payload_qws_tail || xfer_bytes_tail) {
		total_credits_needed += 1;
	}

	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lhr */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		         ((total_credits_needed-1) << 4);

	/* does not include pbc (8 bytes), but does include icrc (4 bytes) */
	const uint16_t lrh_dws = htons(pbc_dws - 1);
	if (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == FI_LOCAL_COMM) ||
	    (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == (FI_LOCAL_COMM | FI_REMOTE_COMM)) &&
	     (opx_ep->tx->send.hdr.stl.lrh.slid == addr.uid.lid))) {
		uint64_t pos;
		ssize_t rc;
		union fi_opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
			opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) return rc;

		hdr->qw[0] = opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
		hdr->qw[1] = opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) |
			     ((caps & FI_MSG) ? (uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_EAGER :
						(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_EAGER);
		hdr->qw[2] = opx_ep->tx->send.hdr.qw[2];
		hdr->qw[3] = opx_ep->tx->send.hdr.qw[3] | (((uint64_t)data) << 32);
		hdr->qw[4] = opx_ep->tx->send.hdr.qw[4] | (payload_qws_total << 48);

		/* Fill QW 5 from the iovec */
		uint8_t *buf = (uint8_t *)&hdr->qw[5];
		ssize_t remain = total_len, iov_idx = 0, iov_base_offset = 0;

		if (xfer_bytes_tail) {
			ssize_t tail_len = xfer_bytes_tail;
			remain = total_len - tail_len;
			while (false ==
			       fi_opx_hfi1_fill_from_iov8(
				       iov, /* In:  iovec array */
				       niov, /* In:  total iovecs */
				       buf, /* In:  target buffer to fill */
				       &tail_len, /* In/Out:  buffer length to fill */
				       &iov_idx, /* In/Out:  start index, returns end */
				       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
				// copy until done;
			}
			assert(tail_len == 0);
		}
		hdr->qw[6] = tag;

		union fi_opx_hfi1_packet_payload *const payload =
			(union fi_opx_hfi1_packet_payload *)(hdr + 1);

		buf = payload->byte;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(
			       iov, /* In:  iovec array */
			       niov, /* In:  total iovecs */
			       buf, /* In:  target buffer to fill */
			       &remain, /* In/Out:  buffer length to fill */
			       &iov_idx, /* In/Out:  start index, returns end */
			       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
			// copy until done;
		}
		assert(remain == 0);
		opx_shm_tx_advance(&opx_ep->tx->shm, (void *)hdr, pos);
		fi_opx_shm_poll_many(&opx_ep->ep_fid, 0);

		if (OFI_LIKELY(do_cq_completion)) {
			fi_opx_ep_tx_cq_inject_completion(ep, context, total_len,
			lock_required, tag, caps);
		}

		return FI_SUCCESS;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- EAGER (begin)\n");

	// Even though we're using the reliability service to pack this buffer
	// we still want to make sure it will have enough credits available to send
	// and allow the user to poll and quiesce the fabric some
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	uint64_t total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (total_credits_available < total_credits_needed) {
			return -FI_EAGAIN;
		}
	}

	/* compile-time constant expression */
	struct fi_opx_reliability_tx_replay *replay;
	replay = (reliability != OFI_RELIABILITY_KIND_NONE) ?
	fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false) :	NULL;
	if (replay == NULL) {
		return -FI_EAGAIN;
	}

	/* compile-time constant expression */
	union fi_opx_reliability_tx_psn *psn_ptr;
	const int64_t psn = (reliability != OFI_RELIABILITY_KIND_NONE) ?
				fi_opx_reliability_tx_next_psn(ep, &opx_ep->reliability->state,
								addr.uid.lid, dest_rx,
								addr.reliability_rx, &psn_ptr, 1) :
				0;
	if(OFI_UNLIKELY(psn == -1)) {
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
		return -FI_EAGAIN;
	}

	ssize_t remain = total_len, iov_idx = 0, iov_base_offset = 0;

	replay->scb.qw0       = opx_ep->tx->send.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT);
	replay->scb.hdr.qw[0] = opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	replay->scb.hdr.qw[1] = opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) |
		((caps & FI_MSG) ? (uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_EAGER :
		 (uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_EAGER);
	replay->scb.hdr.qw[2] = opx_ep->tx->send.hdr.qw[2] | psn;
	replay->scb.hdr.qw[3] = opx_ep->tx->send.hdr.qw[3] | (((uint64_t)data) << 32);
	replay->scb.hdr.qw[4] = opx_ep->tx->send.hdr.qw[4] | (payload_qws_total << 48);
	if (xfer_bytes_tail) {
		ssize_t tail_len = xfer_bytes_tail;
		remain = total_len - tail_len;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(
			       iov, /* In:  iovec array */
			       niov, /* In:  total iovecs */
			       &replay->scb.hdr.qw[5],   /* In:  target buffer to fill */
			       &tail_len, /* In/Out:  buffer length to fill */
			       &iov_idx, /* In/Out:  start index, returns end */
			       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
			// copy until done;
		}
		assert(tail_len == 0);
	}
	replay->scb.hdr.qw[6] = tag;

	remain = total_len - xfer_bytes_tail;
	uint64_t *payload = replay->payload;
	while (false ==
		   fi_opx_hfi1_fill_from_iov8(
			   iov, /* In:  iovec array */
			   niov, /* In:  total iovecs */
			   payload, /* In:  target buffer to fill */
			   &remain, /* In/Out:  buffer length to fill */
			   &iov_idx, /* In/Out:  start index, returns end */
			   &iov_base_offset)) { /* In/Out:  start offset, returns offset */
		// copy until done;
	}

	fi_opx_reliability_client_replay_register_no_update(
		&opx_ep->reliability->state, addr.uid.lid, addr.reliability_rx,
		dest_rx, psn_ptr, replay, reliability);

	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_inject_completion(ep, context, total_len,
			lock_required, tag, caps);
	}

	fi_opx_reliability_service_do_replay(&opx_ep->reliability->service, replay);

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- EAGER (end)\n");


	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_check_credits(struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_pio_state *pio_state,
				uint16_t credits_needed)
{
	union fi_opx_hfi1_pio_state pio_state_val = *pio_state;
	uint16_t total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state_val, &opx_ep->tx->force_credit_return, credits_needed);

	if (OFI_UNLIKELY(total_credits_available < credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state_val, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state_val, &opx_ep->tx->force_credit_return, credits_needed);

		if (total_credits_available < credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state_val.qw0;
			return -FI_ENOBUFS;
		}
	}
	pio_state->qw0 = pio_state_val.qw0;

	return (ssize_t) total_credits_available;
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_hfi1_tx_is_intranode(struct fid_ep *ep, const fi_addr_t dest_addr, const uint64_t caps)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const union fi_opx_addr addr = { .fi = dest_addr };

	/* Intranode if (exclusively FI_LOCAL_COMM) OR (FI_LOCAL_COMM is on AND
	   the source lid is the same as the destination lid) */
	return  ((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == FI_LOCAL_COMM) ||
		(((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == (FI_LOCAL_COMM | FI_REMOTE_COMM)) &&
			(opx_ep->tx->send.hdr.stl.lrh.slid == addr.uid.lid));
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_egr_intranode(struct fid_ep *ep,
					const void *buf,
					size_t len,
					fi_addr_t dest_addr,
					uint64_t tag,
					void * context,
					const uint32_t data,
					int lock_required,
					const uint64_t dest_rx,
					const uint64_t caps,
					const uint64_t do_cq_completion)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const uint64_t bth_rx = ((uint64_t)dest_rx) << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(dest_addr);

	const size_t xfer_bytes_tail = len & 0x07ul;
	const size_t payload_qws_total = len >> 3;

	const uint64_t pbc_dws =
		2 +			/* pbc */
		2 +			/* lhr */
		3 +			/* bth */
		9 +			/* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		(payload_qws_total << 1);

	const uint16_t lrh_dws = htons(pbc_dws-1);	/* does not include pbc (8 bytes), but does include icrc (4 bytes) */

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, SHM -- EAGER (begin)\n");
	uint64_t pos;
	ssize_t rc;
	union fi_opx_hfi1_packet_hdr * const hdr =
		opx_shm_tx_next(&opx_ep->tx->shm, dest_rx, &pos,
			opx_ep->daos_info.hfi_rank_enabled, opx_ep->daos_info.rank,
			opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND, SHM -- EAGER (end) - No packet available.\n");
		return rc;
	}

	hdr->qw[0] = opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);

	hdr->qw[1] = opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) |
		((caps & FI_MSG) ?
			(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_EAGER :
			(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_EAGER);

	hdr->qw[2] = opx_ep->tx->send.hdr.qw[2];

	hdr->qw[3] = opx_ep->tx->send.hdr.qw[3] | (((uint64_t)data) << 32);

	hdr->qw[4] = opx_ep->tx->send.hdr.qw[4] | (payload_qws_total << 48);

	/* only if is_contiguous */
	if (OFI_LIKELY(len > 7)) {
		/* safe to blindly qw-copy the first portion of the source buffer */
		hdr->qw[5] = *((uint64_t *)buf);
	} else {
		hdr->qw[5] = 0;
		memcpy((void*)&hdr->qw[5], buf, xfer_bytes_tail);
	}

	hdr->qw[6] = tag;

	union fi_opx_hfi1_packet_payload * const payload =
		(union fi_opx_hfi1_packet_payload *)(hdr+1);

	memcpy((void*)payload->byte,
		(const void *)((uintptr_t)buf + xfer_bytes_tail),
		payload_qws_total * sizeof(uint64_t));


	opx_shm_tx_advance(&opx_ep->tx->shm, (void*)hdr, pos);

	if (do_cq_completion) {
		fi_opx_ep_tx_cq_inject_completion(ep, context, len, lock_required,
			tag, caps);
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, SHM -- EAGER (end)\n");

	return FI_SUCCESS;

}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_egr_write_packet_header(struct fi_opx_ep *opx_ep,
					union fi_opx_hfi1_pio_state *pio_state,
					uint64_t local_target[8],
					const void *buf,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const uint16_t lrh_dws,
					const uint64_t pbc_dws,
					const ssize_t len,
					const ssize_t xfer_bytes_tail,
					const size_t payload_qws_total,
					const uint32_t psn,
					const uint32_t data,
					const uint64_t tag,
					const uint64_t caps)
{
	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	volatile uint64_t * const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	/* only if is_contiguous */
	if (OFI_LIKELY(len > 7)) {
		/* safe to blindly qw-copy the first portion of the source buffer */
		fi_opx_set_scb(scb, local_target,
			opx_ep->tx->send.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
			opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),

			opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) |
				((caps & FI_MSG) ?
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_EAGER :
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_EAGER),

			opx_ep->tx->send.hdr.qw[2] | psn,
			opx_ep->tx->send.hdr.qw[3] | (((uint64_t)data) << 32),
			opx_ep->tx->send.hdr.qw[4] | (payload_qws_total << 48),
			*((uint64_t *)buf), tag);
	} else {
		fi_opx_set_scb_special2(scb, local_target,
			opx_ep->tx->send.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
			opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),

			opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) |
				((caps & FI_MSG) ?
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_EAGER :
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_EAGER),

			opx_ep->tx->send.hdr.qw[2] | psn,
			opx_ep->tx->send.hdr.qw[3] | (((uint64_t)data) << 32),
			opx_ep->tx->send.hdr.qw[4] | (payload_qws_total << 48),
			buf, xfer_bytes_tail, tag);
	}

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; // Consumed 1 credit
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_egr_write_full_payload_blocks(struct fi_opx_ep *opx_ep,
					union fi_opx_hfi1_pio_state *pio_state,
					uint64_t *buf_qws,
					uint16_t full_block_credits_needed,
					const ssize_t total_credits_available)
{
	/*
	 * write the payload "send control block(s)"
	 */
	const uint16_t contiguous_credits_until_wrap =
		pio_state->credits_total - pio_state->scb_head_index;

	const uint16_t contiguous_credits_available =
		MIN(total_credits_available, contiguous_credits_until_wrap);

	union fi_opx_hfi1_pio_state pio_local = *pio_state;
	volatile uint64_t * scb_payload =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_local);

	const uint16_t contiguous_full_blocks_to_write =
		MIN(full_block_credits_needed, contiguous_credits_available);

	uint16_t i;
	for (i=0; i<contiguous_full_blocks_to_write; ++i) {
		scb_payload[0] = buf_qws[0];
		scb_payload[1] = buf_qws[1];
		scb_payload[2] = buf_qws[2];
		scb_payload[3] = buf_qws[3];
		scb_payload[4] = buf_qws[4];
		scb_payload[5] = buf_qws[5];
		scb_payload[6] = buf_qws[6];
		scb_payload[7] = buf_qws[7];
		scb_payload += 8;
		buf_qws += 8;
	}

	FI_OPX_HFI1_CONSUME_CREDITS(pio_local, contiguous_full_blocks_to_write);
	ssize_t credits_consumed = contiguous_full_blocks_to_write;

	full_block_credits_needed -= contiguous_full_blocks_to_write;

	if (OFI_UNLIKELY(full_block_credits_needed > 0)) {

		/*
		 * handle wrap condition
		 */

		scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_local);

		uint16_t i;
		for (i=0; i<full_block_credits_needed; ++i) {
			scb_payload[0] = buf_qws[0];
			scb_payload[1] = buf_qws[1];
			scb_payload[2] = buf_qws[2];
			scb_payload[3] = buf_qws[3];
			scb_payload[4] = buf_qws[4];
			scb_payload[5] = buf_qws[5];
			scb_payload[6] = buf_qws[6];
			scb_payload[7] = buf_qws[7];
			scb_payload += 8;
			buf_qws += 8;
		}

		FI_OPX_HFI1_CONSUME_CREDITS(pio_local, full_block_credits_needed);
		credits_consumed += full_block_credits_needed;
	}

	pio_state->qw0 = pio_local.qw0;

	return credits_consumed;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_egr_write_payload_tail(struct fi_opx_ep *opx_ep,
					union fi_opx_hfi1_pio_state *pio_state,
					uint64_t *buf_qws,
					const size_t payload_qws_tail)
{
	volatile uint64_t * scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, *pio_state);

	unsigned i = 0;
	for (; i<payload_qws_tail; ++i) {
		scb_payload[i] = buf_qws[i];
	}

	for (; i<8; ++i) {
		scb_payload[i] = 0;
	}
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);

	return 1;	/* Consumed 1 credit */
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_tx_send_egr_write_replay_data(struct fi_opx_ep *opx_ep,
			const union fi_opx_addr addr,
			struct fi_opx_reliability_tx_replay *replay,
			union fi_opx_reliability_tx_psn *psn_ptr,
			const ssize_t xfer_bytes_tail,
			const uint64_t tmp[8],
			const void *buf,
			const size_t payload_qws_total,
			const enum ofi_reliability_kind reliability)
{
	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		replay->scb.qw0 = tmp[0];
		replay->scb.hdr.qw[0] = tmp[1];
		replay->scb.hdr.qw[1] = tmp[2];
		replay->scb.hdr.qw[2] = tmp[3];
		replay->scb.hdr.qw[3] = tmp[4];
		replay->scb.hdr.qw[4] = tmp[5];
		replay->scb.hdr.qw[5] = tmp[6];
		replay->scb.hdr.qw[6] = tmp[7];

		uint64_t *buf_qws = (uint64_t*)((uintptr_t)buf + xfer_bytes_tail);
		uint64_t * payload = replay->payload;
		size_t i;
		for (i=0; i<payload_qws_total; i++) {
			payload[i] = buf_qws[i];
		}

		fi_opx_reliability_client_replay_register_no_update(&opx_ep->reliability->state, addr.uid.lid,
								addr.reliability_rx, addr.hfi1_rx, psn_ptr, replay,
								reliability);
	}
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_ep_tx_get_replay(struct fi_opx_ep *opx_ep,
				const union fi_opx_addr addr,
				struct fi_opx_reliability_tx_replay **replay,
				union fi_opx_reliability_tx_psn **psn_ptr,
				int32_t *psn,
				const enum ofi_reliability_kind reliability)
{
	if (reliability == OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		*replay = NULL;
		*psn_ptr = NULL;
		*psn = 0;
		return FI_SUCCESS;
	}

	*replay = fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false);
	if(*replay == NULL) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND, FI_EAGAIN HFI -- EAGER (null_reply_buffer)\n");
		return -FI_EAGAIN;
	}

	*psn = fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
					&opx_ep->reliability->state,
					addr.uid.lid, addr.hfi1_rx,
					addr.reliability_rx, psn_ptr, 1);

	if(OFI_UNLIKELY(*psn == -1)) {
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, *replay);
		return -FI_EAGAIN;
	}

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_egr(struct fid_ep *ep,
		const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, uint64_t tag, void* context,
		const uint32_t data, int lock_required,
		const unsigned override_flags, uint64_t tx_op_flags,
		const uint64_t dest_rx,
		const uint64_t caps,
		const enum ofi_reliability_kind reliability,
		const uint64_t do_cq_completion)
{
	if (fi_opx_hfi1_tx_is_intranode(ep, dest_addr, caps)) {
		return fi_opx_hfi1_tx_send_egr_intranode(ep, buf, len, dest_addr,
			tag, context, data, lock_required, dest_rx, caps, do_cq_completion);
	}

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr = { .fi = dest_addr };

	const size_t xfer_bytes_tail = len & 0x07ul;
	const size_t payload_qws_total = len >> 3;
	const size_t payload_qws_tail = payload_qws_total & 0x07ul;

	uint16_t full_block_credits_needed = (uint16_t)(payload_qws_total >> 3);

	const uint64_t bth_rx = ((uint64_t)dest_rx) << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(dest_addr);

	const uint64_t pbc_dws =
		2 +			/* pbc */
		2 +			/* lhr */
		3 +			/* bth */
		9 +			/* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		(payload_qws_total << 1);

	const uint16_t lrh_dws = htons(pbc_dws-1);	/* does not include pbc (8 bytes), but does include icrc (4 bytes) */

	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- EAGER (begin)\n");

	/* first check for sufficient credits to inject the entire packet */

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;


	const uint16_t total_credits_needed =
		1 +				/* packet header */
		full_block_credits_needed +	/* full payload blocks */
		(payload_qws_tail > 0);		/* partial payload block */

	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn *psn_ptr;
	int32_t psn;

	if (fi_opx_ep_tx_get_replay(opx_ep, addr, &replay, &psn_ptr, &psn, reliability)) {
		return -FI_EAGAIN;
	}

	uint64_t tmp[8];
#ifndef NDEBUG
	unsigned credits_consumed =
#endif
	fi_opx_hfi1_tx_egr_write_packet_header(opx_ep, &pio_state, tmp, buf, bth_rx, lrh_dlid,
						lrh_dws, pbc_dws, len, xfer_bytes_tail,
						payload_qws_total, psn, data, tag, caps);

	uint64_t *buf_qws = (uint64_t*)((uintptr_t)buf + xfer_bytes_tail);

	if (OFI_LIKELY(full_block_credits_needed)) {
#ifndef NDEBUG
		credits_consumed +=
#endif
		fi_opx_hfi1_tx_egr_write_full_payload_blocks(opx_ep, &pio_state,
							buf_qws, full_block_credits_needed,
							total_credits_available - 1);
	}

	if (OFI_LIKELY(payload_qws_tail)) {
#ifndef NDEBUG
		credits_consumed +=
#endif
		fi_opx_hfi1_tx_egr_write_payload_tail(opx_ep, &pio_state,
					buf_qws + (full_block_credits_needed << 3),
					payload_qws_tail);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr,
					xfer_bytes_tail, tmp, buf, payload_qws_total, reliability);

	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_inject_completion(ep, context, len,
			lock_required, tag, caps);
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- EAGER (end)\n");

	return FI_SUCCESS;
}

/*
 * Write the initial packet header of a multi-packet eager send. This will include the size of
 * the entire multi-packet eager payload.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_mp_egr_write_initial_packet_header(struct fi_opx_ep *opx_ep,
					union fi_opx_hfi1_pio_state *pio_state,
					uint64_t local_target[8],
					const void *buf,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const uint16_t lrh_dws,
					const uint64_t pbc_dws,
					const uint64_t payload_bytes_total,
					const uint32_t psn,
					const uint32_t data,
					const uint64_t tag,
					const uint64_t caps)
{
	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	volatile uint64_t * const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	/* For a multi-packet eager, the *first* packet's payload length should always be > 15 bytes,
	   so we should be safe to blindly copy 2 qws out of buf */
	fi_opx_set_scb(scb, local_target,
		opx_ep->tx->send.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
		opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),
		opx_ep->tx->send.hdr.qw[1] | bth_rx | FI_OPX_MP_EGR_XFER_BYTES_TAIL |
				((caps & FI_MSG) ?
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST :
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST),
		opx_ep->tx->send.hdr.qw[2] | (payload_bytes_total << 32) | psn,
		opx_ep->tx->send.hdr.qw[3] | (((uint64_t)data) << 32),
		*((uint64_t *)buf),
		*((uint64_t *)buf + 1), tag);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; /* Consumed 1 credit */
}

/*
 * Write the nth packet header of a multi-packet eager send where the remaining payload data is
 * more than 16 bytes. This means we'll use all 16 bytes of tail space in the packet header, and
 * there will be at least some payload data.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_mp_egr_write_nth_packet_header(struct fi_opx_ep *opx_ep,
					union fi_opx_hfi1_pio_state *pio_state,
					uint64_t local_target[8],
					const void *buf,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const uint16_t lrh_dws,
					const uint64_t pbc_dws,
					const ssize_t xfer_bytes_tail,
					const uint32_t payload_offset,
					const uint32_t psn,
					const uint32_t mp_egr_uid)
{
	volatile uint64_t * const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	fi_opx_set_scb(scb, local_target,
		opx_ep->tx->send.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
		opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),
		opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) | (uint64_t)FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH,
		opx_ep->tx->send.hdr.qw[2] | psn,
		opx_ep->tx->send.hdr.qw[3],
		*((uint64_t *)buf),
		*((uint64_t *)buf + 1),
		(((uint64_t) mp_egr_uid) << 32) | payload_offset);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; /* Consumed 1 credit */
}

/*
 * Write the nth packet header of a multi-packet eager send where the remaining payload data is <= 16 bytes.
 * This means we won't need to write a payload packet as the entire payload fits in the packet header.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_mp_egr_write_nth_packet_header_no_payload(struct fi_opx_ep *opx_ep,
					union fi_opx_hfi1_pio_state *pio_state,
					uint64_t local_target[8],
					const void *buf,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const uint16_t lrh_dws,
					const uint64_t pbc_dws,
					const ssize_t xfer_bytes_tail,
					const uint32_t payload_offset,
					const uint32_t psn,
					const uint32_t mp_egr_uid)
{
	volatile uint64_t * const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	fi_opx_set_scb_special(scb, local_target,
		opx_ep->tx->send.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
		opx_ep->tx->send.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),
		opx_ep->tx->send.hdr.qw[1] | bth_rx | (xfer_bytes_tail << 48) | (uint64_t)FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH,
		opx_ep->tx->send.hdr.qw[2] | psn,
		opx_ep->tx->send.hdr.qw[3],
		buf, xfer_bytes_tail,
		(((uint64_t) mp_egr_uid) << 32) | payload_offset);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; /* Consumed 1 credit */
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_mp_egr_first (struct fi_opx_ep *opx_ep,
					const void *buf,
					const uint64_t payload_bytes_total,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const union fi_opx_addr addr,
					uint64_t tag,
					const uint32_t data,
					int lock_required,
					const uint64_t caps,
					const enum ofi_reliability_kind reliability,
					uint32_t *psn_out)
{
	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- MULTI-PACKET EAGER FIRST (begin)\n");

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, FI_OPX_MP_EGR_CHUNK_CREDITS);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn *psn_ptr;
	int32_t psn;

	if (OFI_UNLIKELY(fi_opx_ep_tx_get_replay(opx_ep, addr, &replay, &psn_ptr, &psn, reliability))) {
		return -FI_EAGAIN;
	}

	*psn_out = psn;		/* This will be the UID used in the remaining packets */ 

	uint64_t tmp[8];
#ifndef NDEBUG
	unsigned credits_consumed =
#endif
	fi_opx_hfi1_tx_mp_egr_write_initial_packet_header(opx_ep, &pio_state, tmp, buf, bth_rx, lrh_dlid,
					htons(FI_OPX_MP_EGR_CHUNK_DWS - 1),
					FI_OPX_MP_EGR_CHUNK_DWS,
					payload_bytes_total,
					psn,
					data,
					tag,
					caps);

	uint64_t *buf_qws = (uint64_t*)((uintptr_t)buf + FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL);

#ifndef NDEBUG
	credits_consumed +=
#endif
	fi_opx_hfi1_tx_egr_write_full_payload_blocks(opx_ep, &pio_state,
						buf_qws,
						FI_OPX_MP_EGR_CHUNK_CREDITS - 1,
						total_credits_available - 1);

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == FI_OPX_MP_EGR_CHUNK_CREDITS);
#endif

	/* Since this is the first packet of a multi-packet eager send,
	   there will be no partial-payload blocks at the end to send */

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr,
					FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL, tmp, buf,
					FI_OPX_MP_EGR_CHUNK_PAYLOAD_QWS, reliability);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- MULTI-PACKET EAGER FIRST (end)\n");

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_mp_egr_nth (struct fi_opx_ep *opx_ep,
					const void *buf,
					const uint32_t payload_offset,
					const uint32_t mp_egr_uid,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const union fi_opx_addr addr,
					int lock_required,
					const enum ofi_reliability_kind reliability)
{
	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- MULTI-PACKET EAGER NTH (begin)\n");

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, FI_OPX_MP_EGR_CHUNK_CREDITS);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn *psn_ptr;
	int32_t psn;

	if (OFI_UNLIKELY(fi_opx_ep_tx_get_replay(opx_ep, addr, &replay, &psn_ptr, &psn, reliability))) {
		return -FI_EAGAIN;
	}

	uint64_t tmp[8];
#ifndef NDEBUG
	unsigned credits_consumed =
#endif
	fi_opx_hfi1_tx_mp_egr_write_nth_packet_header(opx_ep, &pio_state, tmp, buf, bth_rx, lrh_dlid,
					htons(FI_OPX_MP_EGR_CHUNK_DWS - 1),
					FI_OPX_MP_EGR_CHUNK_DWS,
					FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL,
					payload_offset,
					psn,
					mp_egr_uid);

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
	uint64_t *buf_qws = (uint64_t*)((uintptr_t)buf + FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL);

#ifndef NDEBUG
	credits_consumed +=
#endif
	fi_opx_hfi1_tx_egr_write_full_payload_blocks(opx_ep, &pio_state,
						buf_qws,
						FI_OPX_MP_EGR_CHUNK_CREDITS - 1,
						total_credits_available - 1);

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == FI_OPX_MP_EGR_CHUNK_CREDITS);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr,
					FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL, tmp, buf,
					FI_OPX_MP_EGR_CHUNK_PAYLOAD_QWS, reliability);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- MULTI-PACKET EAGER NTH (end)\n");

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_mp_egr_last (struct fi_opx_ep *opx_ep,
					const void *buf,
					const uint32_t payload_offset,
					const ssize_t len,
					const uint32_t mp_egr_uid,
					const uint64_t bth_rx,
					const uint64_t lrh_dlid,
					const union fi_opx_addr addr,
					int lock_required,
					const enum ofi_reliability_kind reliability)
{
	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- MULTI-PACKET EAGER LAST (begin)\n");

	size_t xfer_bytes_tail;
	if (len <= FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL) {
		xfer_bytes_tail = len;
	} else if (!(len & 0x07ul)) {
		/* Length is a multiple of 8 bytes and must be at least 24.
		   We can store 16 bytes of that in tail */
		xfer_bytes_tail = FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL;
	} else {
		/* Length is not a multiple of 8 bytes, and it's greater than 16.
		   We can store 8 + n bytes in tail (where n == len % 8) */
		xfer_bytes_tail = 8 + (len & 0x07ul);
	}

	const size_t payload_qws_total = (len - xfer_bytes_tail) >> 3;
	const size_t payload_qws_tail = payload_qws_total & 0x07ul;

	uint16_t full_block_credits_needed = (uint16_t)(payload_qws_total >> 3);
	uint16_t total_credits_needed = full_block_credits_needed +
					1 +				/* pbc/packet header */
					(payload_qws_tail ? 1 : 0);

	const uint64_t pbc_dws = 16 +	/* pbc + packet header */
		(payload_qws_total << 1);

	const uint16_t lrh_dws = htons(pbc_dws-1);	/* does not include pbc (8 bytes), but does include icrc (4 bytes) */

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn *psn_ptr;
	int32_t psn;

	if (OFI_UNLIKELY(fi_opx_ep_tx_get_replay(opx_ep, addr, &replay, &psn_ptr, &psn, reliability))) {
		return -FI_EAGAIN;
	}

	uint64_t tmp[8];

#ifndef NDEBUG
	unsigned credits_consumed;
#endif

	if (OFI_UNLIKELY(len <= FI_OPX_MP_EGR_CHUNK_PAYLOAD_TAIL)) {
#ifndef NDEBUG
		credits_consumed = 
#endif
		fi_opx_hfi1_tx_mp_egr_write_nth_packet_header_no_payload(opx_ep, &pio_state, tmp, buf, bth_rx,
								lrh_dlid, lrh_dws, pbc_dws, len, payload_offset,
								psn, mp_egr_uid);

	} else {
#ifndef NDEBUG
		credits_consumed = 
#endif
		fi_opx_hfi1_tx_mp_egr_write_nth_packet_header(opx_ep, &pio_state, tmp, buf, bth_rx, lrh_dlid,
							lrh_dws, pbc_dws, xfer_bytes_tail, payload_offset, psn, mp_egr_uid);

		uint64_t *buf_qws = (uint64_t*)((uintptr_t)buf + xfer_bytes_tail);

		if (OFI_LIKELY(full_block_credits_needed)) {
#ifndef NDEBUG
			credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_write_full_payload_blocks(opx_ep, &pio_state,
								buf_qws,
								full_block_credits_needed,
								total_credits_available - 1);
		}

		if (OFI_LIKELY(payload_qws_tail)) {
#ifndef NDEBUG
			credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_write_payload_tail(opx_ep, &pio_state,
							buf_qws + (full_block_credits_needed << 3),
							payload_qws_tail);
		}
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr,
					xfer_bytes_tail, tmp, buf,
					payload_qws_total, reliability);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- MULTI-PACKET EAGER LAST (end)\n");

	return FI_SUCCESS;
}

static inline void fi_opx_shm_write_fence(struct fi_opx_ep *opx_ep, const uint64_t dest_rx,
					  const uint64_t lrh_dlid,
					  struct fi_opx_completion_counter *cc,
					  const uint64_t bytes_to_sync,
					  const uint32_t dest_extended_rx)
{
	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lrh */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 (0 << 4);
	const uint16_t lrh_dws = htons(pbc_dws - 1);
	const uint64_t bth_rx = dest_rx << 56;
	uint64_t pos;
	ssize_t rc;
	union fi_opx_hfi1_packet_hdr * tx_hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);
	/* Potential infinite loop, unable to return result to application */
	while(OFI_UNLIKELY(tx_hdr == NULL)) {  //TODO: Verify that all callers of this function can tolderate a NULL rc
		fi_opx_shm_poll_many(&opx_ep->ep_fid, FI_OPX_LOCK_NOT_REQUIRED);
		tx_hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
			dest_extended_rx, opx_ep->daos_info.rank_inst, &rc);
	}

	tx_hdr->qw[0] = opx_ep->rx->tx.cts.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	tx_hdr->qw[1] = opx_ep->rx->tx.cts.hdr.qw[1] | bth_rx;
	tx_hdr->qw[2] = opx_ep->rx->tx.cts.hdr.qw[2];
	tx_hdr->qw[3] = opx_ep->rx->tx.cts.hdr.qw[3];
	tx_hdr->qw[4] = opx_ep->rx->tx.cts.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_FENCE | (0ULL << 32);
	tx_hdr->qw[5] = (uintptr_t)cc;
	tx_hdr->qw[6] = bytes_to_sync;

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *)tx_hdr, pos);
}

ssize_t fi_opx_hfi1_tx_sendv_rzv(struct fid_ep *ep, const struct iovec *iov, size_t niov,
				size_t total_len, void *desc, fi_addr_t dest_addr, uint64_t tag,
				void *context, const uint32_t data, int lock_required,
				const unsigned override_flags, uint64_t tx_op_flags,
				const uint64_t dest_rx, const uintptr_t origin_byte_counter_vaddr,
				uint64_t *origin_byte_counter_value, const uint64_t caps,
				const enum ofi_reliability_kind reliability);

ssize_t fi_opx_hfi1_tx_send_rzv(struct fid_ep *ep, const void *buf, size_t len, void *desc,
				fi_addr_t dest_addr, uint64_t tag, void *context,
				const uint32_t data, int lock_required,
				const unsigned override_flags, uint64_t tx_op_flags,
				const uint64_t dest_rx, const uintptr_t origin_byte_counter_vaddr,
				uint64_t *origin_byte_counter_value, const uint64_t caps,
				const enum ofi_reliability_kind reliability);


#endif /* _FI_PROV_OPX_HFI1_TRANSPORT_H_ */
