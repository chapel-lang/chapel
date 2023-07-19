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
#ifndef _FI_PROV_OPX_HFI1_PROGRESS_H_
#define _FI_PROV_OPX_HFI1_PROGRESS_H_

#ifndef FI_OPX_FABRIC_HFI1
#error "fabric selection #define error"
#endif

#define FI_OPX_SIZEOF_ONE_CONTEXT_BYTES (0x20ul)

#include "rdma/opx/fi_opx_hfi1.h"
#include "uthash.h"
#include "fi_opx_reliability.h"
#include "rdma/opx/fi_opx_flight_recorder.h"

unsigned fi_opx_hfi1_handle_poll_error(struct fi_opx_ep *opx_ep, volatile uint32_t *rhf_ptr,
				       const uint32_t rhf_msb, const uint32_t rhf_lsb,
				       const uint32_t rhf_seq, const uint64_t hdrq_offset,
				       const uint32_t hdrq_offset_notifyhw);

__OPX_FORCE_INLINE__
void fi_opx_hfi1_update_hdrq_head_register(struct fi_opx_ep *opx_ep, const uint64_t hdrq_offset,
				      const uint32_t hdrq_offset_notifyhw)
{
	/*
	 * Notify the hfi that this packet has been processed....
	 * BUT only do this when get to the notification stops:  hdrq_offset_notifyhw
	 * The write to hardware is expensive.
	 */
	if (OFI_UNLIKELY((hdrq_offset & hdrq_offset_notifyhw) == FI_OPX_SIZEOF_ONE_CONTEXT_BYTES)) {
		*opx_ep->rx->hdrq.head_register = hdrq_offset - 32;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "================== > Set HFI head register\n");
	}
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_handle_ud_eager_packet(struct fi_opx_ep *opx_ep,
					const union fi_opx_hfi1_packet_hdr *const hdr,
					const uint32_t rhf_msb, const uint32_t rhf_lsb)
{
	/* "eager" packet - has payload */
	const uint32_t egrbfr_index = (rhf_lsb >> FI_OPX_HFI1_RHF_EGRBFR_INDEX_SHIFT) &
				      FI_OPX_HFI1_RHF_EGRBFR_INDEX_MASK;
	const uint32_t egrbfr_offset = rhf_msb & 0x0FFFu;
	const uint8_t *const __attribute__((unused)) payload =
		(uint8_t *)((uintptr_t)opx_ep->rx->egrq.base_addr +
			    (uintptr_t)egrbfr_index * (uintptr_t)opx_ep->rx->egrq.elemsz +
			    egrbfr_offset * 64);

	assert(payload != NULL);

	/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
	const uint16_t lrh_pktlen_le = ntohs(hdr->stl.lrh.pktlen);
	const size_t __attribute__((unused)) total_bytes_to_copy =
		(lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
	const size_t __attribute__((unused)) payload_bytes_to_copy =
		total_bytes_to_copy - sizeof(union fi_opx_hfi1_packet_hdr);

	/* currently no eager UD packets are defined */
	fprintf(stderr, "%s:%s():%d bad ud eager packet; abort.\n", __FILE__, __func__,
		__LINE__);
	abort();

	const uint32_t last_egrbfr_index = opx_ep->rx->egrq.last_egrbfr_index;
	if (OFI_UNLIKELY(last_egrbfr_index != egrbfr_index)) {
		*opx_ep->rx->egrq.head_register = last_egrbfr_index;
		opx_ep->rx->egrq.last_egrbfr_index = egrbfr_index;
	}
}

static
void fi_opx_hfi1_handle_ud_ping(struct fi_opx_ep *opx_ep,
				const union fi_opx_hfi1_packet_hdr *const hdr)
{
	struct fi_opx_reliability_service *service =
		opx_ep->reliability->state.service;
	struct fi_opx_pending_rx_reliability_op_key lookup_key;
	lookup_key.key = hdr->service.key;
	lookup_key.psn_start = hdr->service.psn_start;
	struct fi_opx_pending_rx_reliability_op *ping_op = NULL;

	HASH_FIND(hh, service->pending_rx_reliability_ops_hashmap, &lookup_key,
		  sizeof(lookup_key), ping_op);

	/*
	 * Coalesce a ping op if we find it, otherwise allocate a ping op and add it as a new item to the hash of pending ops
	 */
	if (ping_op) {
		ping_op->psn_count_coalesce =
			MAX(ping_op->psn_count_coalesce, hdr->service.psn_count);
	} else {
		ping_op = ofi_buf_alloc(opx_ep->reliability->state.service
						->pending_rx_reliability_pool);
		ping_op->ud_opcode = hdr->ud.opcode;
		ping_op->slid = (uint64_t)hdr->stl.lrh.slid;
		ping_op->rx = (uint64_t)hdr->service.origin_reliability_rx;
		ping_op->key.key = hdr->service.key;
		ping_op->psn_count = hdr->service.psn_count;
		ping_op->psn_count_coalesce = 0;
		ping_op->key.psn_start = hdr->service.psn_start;
		//Send the first ping right away, it might be an RMA fence event
		fi_opx_hfi1_rx_reliability_ping(&opx_ep->ep_fid, service,
						ping_op->key.key,
						ping_op->psn_count,
						ping_op->key.psn_start,
						ping_op->slid, ping_op->rx);

		HASH_ADD(hh, service->pending_rx_reliability_ops_hashmap, key,
			 sizeof(ping_op->key), ping_op);
	}
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_handle_ud_ack(struct fi_opx_ep *opx_ep,
				const union fi_opx_hfi1_packet_hdr *const hdr)
{
	const uint64_t key = hdr->service.key;
	const uint64_t psn_count = hdr->service.psn_count;
	const uint64_t psn_start = hdr->service.psn_start;

	fi_opx_hfi1_rx_reliability_ack(&opx_ep->ep_fid,
				       opx_ep->reliability->state.service, key,
				       psn_count, psn_start);
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_handle_ud_nack(struct fi_opx_ep *opx_ep,
				const union fi_opx_hfi1_packet_hdr *const hdr)
{
	const uint64_t key = hdr->service.key;
	const uint64_t psn_count = hdr->service.psn_count;
	const uint64_t psn_start = hdr->service.psn_start;

	fi_opx_hfi1_rx_reliability_nack(&opx_ep->ep_fid,
					opx_ep->reliability->state.service, key,
					psn_count, psn_start);
}

__OPX_FORCE_INLINE__
unsigned fi_opx_hfi1_handle_ud_packet(struct fi_opx_ep *opx_ep,
				      const union fi_opx_hfi1_packet_hdr *const hdr,
				      const uint32_t rhf_msb, const uint32_t rhf_lsb,
				      const uint32_t rhf_seq, const uint64_t hdrq_offset,
				      const uint32_t hdrq_offset_notifyhw)
{
	if (OFI_LIKELY(!(rhf_lsb & 0x00008000u))) {
		/* "header only" packet - no payload */
		switch(hdr->ud.opcode) {
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING:
				fi_opx_hfi1_handle_ud_ping(opx_ep, hdr);
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK:
				fi_opx_hfi1_handle_ud_ack(opx_ep, hdr);
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK:
				fi_opx_hfi1_handle_ud_nack(opx_ep, hdr);
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP:
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT:
				fi_opx_reliability_handle_ud_init(&opx_ep->ep_fid, &opx_ep->reliability->state, hdr);
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT_ACK:
				fi_opx_reliability_handle_ud_init_ack(&opx_ep->reliability->state, hdr);
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH:
				fi_opx_hfi1_rx_reliability_resynch(&opx_ep->ep_fid,
					opx_ep->reliability->state.service,
					hdr->service.origin_reliability_rx,
					hdr);
				break;
			case FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK:
				fi_opx_hfi1_rx_reliability_ack_resynch(&opx_ep->ep_fid, opx_ep->reliability->state.service, hdr);
				break;
			default:
				fprintf(stderr, "%s:%s():%d bad ud header packet; abort.\n", __FILE__,
					__func__, __LINE__);
				abort();
		};
	} else {
		fi_opx_hfi1_handle_ud_eager_packet(opx_ep, hdr, rhf_msb, rhf_lsb);
	}

	opx_ep->rx->state.hdrq.rhf_seq = (rhf_seq < 0xD0000000u) * rhf_seq + 0x10000000u;
	opx_ep->rx->state.hdrq.head =
		hdrq_offset + 32; /* 32 dws == 128 bytes, the maximum header queue entry size */

	fi_opx_hfi1_update_hdrq_head_register(opx_ep, hdrq_offset, hdrq_offset_notifyhw);

	return 1; /* one packet was processed */
}

__OPX_FORCE_INLINE__
unsigned fi_opx_hfi1_error_inject(struct fi_opx_ep *opx_ep,
				  const union fi_opx_hfi1_packet_hdr *const hdr,
				  const uint32_t rhf_lsb, const uint32_t rhf_seq,
				  const uint64_t hdrq_offset, const uint32_t hdrq_offset_notifyhw)
{
#ifdef OPX_RELIABILITY_TEST
	/*
	 * Error injection .. purposefully drop packet
	 */
	if (OFI_UNLIKELY(FI_OPX_RELIABILITY_RX_DROP_PACKET(&opx_ep->reliability->state, hdr))) {
		opx_ep->rx->state.hdrq.rhf_seq = (rhf_seq < 0xD0000000u) * rhf_seq + 0x10000000u;
		opx_ep->rx->state.hdrq.head =
			hdrq_offset +
			32; /* 32 dws == 128 bytes, the maximum header queue entry size */

		if ((rhf_lsb & 0x00008000u) == 0x00008000u) { /* eager */
			const uint32_t egrbfr_index =
				(rhf_lsb >> FI_OPX_HFI1_RHF_EGRBFR_INDEX_SHIFT) &
				FI_OPX_HFI1_RHF_EGRBFR_INDEX_MASK;
			const uint32_t last_egrbfr_index = opx_ep->rx->egrq.last_egrbfr_index;
			if (OFI_UNLIKELY(last_egrbfr_index != egrbfr_index)) {
				*opx_ep->rx->egrq.head_register = last_egrbfr_index;
				opx_ep->rx->egrq.last_egrbfr_index = egrbfr_index;
			}
		}

		fi_opx_hfi1_update_hdrq_head_register(opx_ep, hdrq_offset, hdrq_offset_notifyhw);

		return 0;
	}
#endif
	return -1;
}

__OPX_FORCE_INLINE__
unsigned fi_opx_hfi1_handle_reliability(struct fi_opx_ep *opx_ep,
					const union fi_opx_hfi1_packet_hdr *const hdr,
					const uint32_t rhf_msb, const uint32_t rhf_lsb,
					const uint32_t rhf_seq, const uint64_t hdrq_offset,
					const uint32_t hdrq_offset_notifyhw,
					uint8_t *origin_rx)
{
	/*
	 * Check for 'reliability' exceptions
	 */
	const uint64_t slid = hdr->stl.lrh.slid;
	const uint64_t origin_tx = FI_OPX_HFI1_PACKET_ORIGIN_TX(hdr);
	const uint64_t psn = FI_OPX_HFI1_PACKET_PSN(hdr);
	if (OFI_UNLIKELY(fi_opx_reliability_rx_check(&opx_ep->reliability->state, slid, origin_tx,
						     psn, origin_rx) == FI_OPX_RELIABILITY_EXCEPTION)) {
		if (!(rhf_lsb & 0x00008000u)) {
			/* no payload */
			fi_opx_reliability_rx_exception(&opx_ep->reliability->state, slid,
							origin_tx, psn, &opx_ep->ep_fid, hdr, NULL);

		} else {
			/* has payload */
			const uint32_t egrbfr_index =
				(rhf_lsb >> FI_OPX_HFI1_RHF_EGRBFR_INDEX_SHIFT) &
				FI_OPX_HFI1_RHF_EGRBFR_INDEX_MASK;
			const uint32_t egrbfr_offset = rhf_msb & 0x0FFFu;
			const uint8_t *const payload =
				(uint8_t *)((uintptr_t)opx_ep->rx->egrq.base_addr +
					    (uintptr_t)egrbfr_index *
						    (uintptr_t)opx_ep->rx->egrq.elemsz +
					    egrbfr_offset * 64);

			assert(payload != NULL);
			fi_opx_reliability_rx_exception(&opx_ep->reliability->state, slid,
							origin_tx, psn, &opx_ep->ep_fid, hdr,
							payload);

			const uint32_t last_egrbfr_index = opx_ep->rx->egrq.last_egrbfr_index;
			if (OFI_UNLIKELY(last_egrbfr_index != egrbfr_index)) {
				*opx_ep->rx->egrq.head_register = last_egrbfr_index;
				opx_ep->rx->egrq.last_egrbfr_index = egrbfr_index;
			}
		}

		opx_ep->rx->state.hdrq.rhf_seq = (rhf_seq < 0xD0000000u) * rhf_seq + 0x10000000u;
		opx_ep->rx->state.hdrq.head =
			hdrq_offset +
			32; /* 32 dws == 128 bytes, the maximum header queue entry size */

		fi_opx_hfi1_update_hdrq_head_register(opx_ep, hdrq_offset, hdrq_offset_notifyhw);

		return 1; /* one packet was processed - even though it was a "reliability event" packet */
	}
	return -1;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_handle_packet(struct fi_opx_ep *opx_ep, const uint8_t opcode,
			       const union fi_opx_hfi1_packet_hdr *const hdr,
			       const uint32_t rhf_msb, const uint32_t rhf_lsb,
			       const uint32_t rhf_seq, const uint64_t hdrq_offset,
			       const uint32_t hdrq_offset_notifyhw, const int lock_required,
			       const enum ofi_reliability_kind reliability,
			       const uint8_t origin_rx)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "================ received a packet from the fabric\n");

	if (!(rhf_lsb & 0x00008000u)) {
		if (OFI_LIKELY(opcode == FI_OPX_HFI_BTH_OPCODE_TAG_INJECT)) {
			/* "header only" packet - no payload */
			fi_opx_ep_rx_process_header(&opx_ep->ep_fid, hdr, NULL, 0, FI_TAGGED,
						    FI_OPX_HFI_BTH_OPCODE_TAG_INJECT,
						    origin_rx,
						    0, /* is_intranode */
						    lock_required, reliability);

		} else if (opcode > FI_OPX_HFI_BTH_OPCODE_TAG_INJECT) {
			/* all other "tag" packets */
			fi_opx_ep_rx_process_header_tag(&opx_ep->ep_fid, hdr, NULL, 0, opcode,
							origin_rx, 0,
							lock_required, reliability);

		} else {
			fi_opx_ep_rx_process_header_msg(&opx_ep->ep_fid, hdr, NULL, 0, opcode,
							origin_rx, 0,
							lock_required, reliability);
		}
	} else {
		/* "eager" packet - has payload */
		const uint32_t egrbfr_index = (rhf_lsb >> FI_OPX_HFI1_RHF_EGRBFR_INDEX_SHIFT) &
					      FI_OPX_HFI1_RHF_EGRBFR_INDEX_MASK;
		const uint32_t egrbfr_offset = rhf_msb & 0x0FFFu;
		const uint8_t *const payload =
			(uint8_t *)((uintptr_t)opx_ep->rx->egrq.base_addr +
				    (uintptr_t)egrbfr_index * (uintptr_t)opx_ep->rx->egrq.elemsz +
				    egrbfr_offset * 64);

		assert(payload != NULL);

		/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
		const uint16_t lrh_pktlen_le = ntohs(hdr->stl.lrh.pktlen);
		const size_t total_bytes_to_copy =
			(lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
		const size_t payload_bytes_to_copy =
			total_bytes_to_copy - sizeof(union fi_opx_hfi1_packet_hdr);

		if (OFI_LIKELY(opcode == FI_OPX_HFI_BTH_OPCODE_TAG_EAGER)) {
			fi_opx_ep_rx_process_header(
				&opx_ep->ep_fid, hdr,
				(const union fi_opx_hfi1_packet_payload *const)payload,
				payload_bytes_to_copy, FI_TAGGED, FI_OPX_HFI_BTH_OPCODE_TAG_EAGER,
				origin_rx,
				0, /* is_intranode */
				lock_required, reliability);
		} else if (opcode > FI_OPX_HFI_BTH_OPCODE_TAG_EAGER) { /* all other "tag" packets */
			fi_opx_ep_rx_process_header_tag(&opx_ep->ep_fid, hdr, payload,
							payload_bytes_to_copy, opcode,
							origin_rx, 0,
							lock_required, reliability);

		} else {
			fi_opx_ep_rx_process_header_msg(&opx_ep->ep_fid, hdr, payload,
							payload_bytes_to_copy, opcode,
							origin_rx, 0,
							lock_required, reliability);
		}
		const uint32_t last_egrbfr_index = opx_ep->rx->egrq.last_egrbfr_index;
		if (OFI_UNLIKELY(last_egrbfr_index != egrbfr_index)) {
			*opx_ep->rx->egrq.head_register = last_egrbfr_index;
			opx_ep->rx->egrq.last_egrbfr_index = egrbfr_index;
		}

		FLIGHT_RECORDER_PACKET_HDR(opx_ep->fr, FR_EVENT_HFI1_POLL_ONCE, hdr);
	}

	opx_ep->rx->state.hdrq.rhf_seq = (rhf_seq < 0xD0000000u) * rhf_seq + 0x10000000u;
	/* 32 dws == 128 bytes, the maximum header queue entry size */
	opx_ep->rx->state.hdrq.head = hdrq_offset + 32;

	fi_opx_hfi1_update_hdrq_head_register(opx_ep, hdrq_offset, hdrq_offset_notifyhw);

	/* Send preemptive ACKs at regular intervals to improve performance (when enabled).
	 *
	 * NOTE: While a PSN of 0 would technically be on a rate boundary, we do not do
	 *       preemptive ACKs for PSN 0, or the previous N packets leading up to MAX_PSN
	 *       before the PSN wrap. The main reason for this is performance. A PSN of 0
	 *       would require a separate if-block with two separate calls to send_pre_acks
	 *       in order to avoid a rollover range. The added cost of executing that check
	 *       2^24 times for the 1 time we'd see that edge case isn't worth the payoff.
	 */

	uint32_t psn = FI_OPX_HFI1_PACKET_PSN(hdr);
	if (!(psn & opx_ep->reliability->service.preemptive_ack_rate_mask) && psn) {

		fi_opx_hfi1_rx_reliability_send_pre_acks(&opx_ep->ep_fid,
				opx_ep->reliability->state.lid_be,
				opx_ep->reliability->state.rx,
				psn - opx_ep->reliability->service.preemptive_ack_rate + 1, /* psn_start */
				opx_ep->reliability->service.preemptive_ack_rate, /* psn_count */
				hdr, origin_rx);

	} else if (hdr->stl.bth.opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA &&
			((ntohl(hdr->stl.bth.psn) & 0x80000000) ||
			(hdr->dput.target.opcode == FI_OPX_HFI_DPUT_OPCODE_PUT))) {
		/* Send preemptive ACKs on Rendezvous FI_OPX_HFI_DPUT_OPCODE_PUT or
		 * on the final packet of a Rendezvous SDMA writev (the high bit
		 * of the PSN - the Acknowledge Request bit - is set)
		 */
		uint32_t psn_count = MAX(MIN(opx_ep->reliability->service.preemptive_ack_rate, psn), 1);
		assert(psn >= psn_count - 1);

		fi_opx_hfi1_rx_reliability_send_pre_acks(&opx_ep->ep_fid,
				opx_ep->reliability->state.lid_be,
				opx_ep->reliability->state.rx,
				psn - psn_count + 1, /* psn_start */
				psn_count, /* psn_count */
				hdr, origin_rx);
	}
}

/*
 * ============================================================================
 *                      THIS IS THE HFI POLL FUNCTION
 * ============================================================================
*/
__OPX_FORCE_INLINE__
unsigned fi_opx_hfi1_poll_once(struct fid_ep *ep, const int lock_required,
			       const enum ofi_reliability_kind reliability,
			       const uint64_t hdrq_mask)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const uint64_t local_hdrq_mask = (hdrq_mask == FI_OPX_HDRQ_MASK_RUNTIME) ?
						 opx_ep->hfi->info.rxe.hdrq.rx_poll_mask :
						 hdrq_mask;
	const uint64_t hdrq_offset = opx_ep->rx->state.hdrq.head & local_hdrq_mask;
	const uint32_t hdrq_offset_notifyhw = local_hdrq_mask / 2;

	assert(local_hdrq_mask % FI_OPX_SIZEOF_ONE_CONTEXT_BYTES == 0);
	volatile uint32_t *rhf_ptr = (uint32_t *)opx_ep->rx->hdrq.rhf_base + hdrq_offset;
	const uint32_t rhf_lsb = rhf_ptr[0];
	const uint32_t rhf_msb = rhf_ptr[1];

	/*
	 * The RHF.RcvSeq field is located in bits [31:28] and values are in
	 * the range of (1..13) inclusive. A new packet is available when the
	 * expected sequence number in the next header queue element matches
	 * the RHF.RcvSeq field.
	 *
	 * Instead of shifting and masking the RHF bits to read the sequence
	 * number in the range of 1..13 (or, 0x1..0xD) use only a bit mask to
	 * obtain the RHF sequence in the range of 0x10000000..0xD0000000.
	 * In this scheme the expected sequence number is incremented by
	 * 0x10000000 instead of 0x1.
	 */
	const uint32_t rhf_seq = opx_ep->rx->state.hdrq.rhf_seq;

	if ((reliability != OFI_RELIABILITY_KIND_NONE) && /* compile-time constant expression */
	    /*
		 * Check for receive errors
		 */
	    OFI_UNLIKELY((rhf_msb & 0xFFE00000u) != 0)) {
		return fi_opx_hfi1_handle_poll_error(opx_ep, rhf_ptr, rhf_msb, rhf_lsb, rhf_seq,
						     hdrq_offset, hdrq_offset_notifyhw);
	}

	if (rhf_seq == (rhf_lsb & 0xF0000000u)) {
		const uint64_t hdrq_offset_dws = (rhf_msb >> 12) & 0x01FFu;

		uint32_t *pkt = (uint32_t *)rhf_ptr - 32 + /* header queue entry size in dw */
				2 + /* rhf field size in dw */
				hdrq_offset_dws;

		const union fi_opx_hfi1_packet_hdr *const hdr = (union fi_opx_hfi1_packet_hdr *)pkt;

		const uint8_t opcode = hdr->stl.bth.opcode;

		if (OFI_UNLIKELY(opcode == FI_OPX_HFI_BTH_OPCODE_UD)) {
			assert(reliability == OFI_RELIABILITY_KIND_ONLOAD);
			/*
			 * process "unreliable datagram" packets first - before all the
			 * software reliability protocol checks.
			 */
			return fi_opx_hfi1_handle_ud_packet(opx_ep, hdr, rhf_msb, rhf_lsb, rhf_seq,
							    hdrq_offset, hdrq_offset_notifyhw);
		}

		uint8_t origin_rx;
		if (reliability != OFI_RELIABILITY_KIND_NONE) {
			/*
			 * check for software reliability events
			 */
			/* This error inject call will compile out in optimized builds */
			unsigned rc = fi_opx_hfi1_error_inject(opx_ep, hdr, rhf_lsb, rhf_seq,
							       hdrq_offset, hdrq_offset_notifyhw);
			if (OFI_UNLIKELY(rc != -1)) {
				return rc;
			}
			rc = fi_opx_hfi1_handle_reliability(opx_ep, hdr, rhf_msb, rhf_lsb, rhf_seq,
							    hdrq_offset, hdrq_offset_notifyhw, &origin_rx);
			if (OFI_UNLIKELY(rc != -1)) {
				return rc;
			}
		} else {
			origin_rx = 0;
		}
		fi_opx_hfi1_handle_packet(opx_ep, opcode, hdr, rhf_msb, rhf_lsb, rhf_seq,
					  hdrq_offset, hdrq_offset_notifyhw, lock_required,
					  reliability, origin_rx);
		return 1; /* one packet was processed */
	}

	return 0;
}

/*
 * ============================================================================
 *                      THIS IS THE SHM POLL FUNCTION
 * ============================================================================
 */
static inline
void fi_opx_shm_poll_many(struct fid_ep *ep, const int lock_required)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	uint64_t pos;
	struct opx_shm_packet* packet = opx_shm_rx_next(&opx_ep->rx->shm, &pos);
	union fi_opx_hfi1_packet_hdr * hdr = (packet) ? 
		(union fi_opx_hfi1_packet_hdr *) packet->data : NULL; 

	while (hdr != NULL) {
		const uint8_t opcode = hdr->stl.bth.opcode;
		uint32_t origin_reliability_rx = hdr->service.origin_reliability_rx;

		/* HFI Rank Support: */
		if (opx_ep->daos_info.hfi_rank_enabled) {
			/* origin_reliability_rx is HFI rank instead of HFI rx */
			origin_reliability_rx = packet->origin_rank;
			/* Settings used for possible response patcket(s) */
			opx_ep->daos_info.rank = packet->origin_rank;
			opx_ep->daos_info.rank_inst = packet->origin_rank_inst;
		}

		if (opcode == FI_OPX_HFI_BTH_OPCODE_TAG_INJECT) {
			fi_opx_ep_rx_process_header(ep, hdr, NULL, 0,
				FI_TAGGED,
				FI_OPX_HFI_BTH_OPCODE_TAG_INJECT,
				(const uint8_t) origin_reliability_rx,
				1, /* is_intranode */
				lock_required,
				OFI_RELIABILITY_KIND_NONE);

		} else if (opcode == FI_OPX_HFI_BTH_OPCODE_UD) {
			const uint8_t ud_opcode = hdr->ud.opcode;

			if (ud_opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
				fi_opx_hfi1_rx_reliability_resynch(&opx_ep->ep_fid,
					opx_ep->reliability->state.service, origin_reliability_rx,
					hdr);

			} else if (ud_opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
				fi_opx_hfi1_rx_reliability_ack_resynch(&opx_ep->ep_fid,
					opx_ep->reliability->state.service, hdr);

			} else {
				fprintf(stderr, "%s:%s():%d bad ud opcode (%u); abort.\n",
					__FILE__, __func__, __LINE__, ud_opcode);
				abort();
			}

		} else {

			const uint8_t * const payload = (uint8_t *)(hdr+1);

			/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
			const uint16_t lrh_pktlen_le = ntohs(hdr->stl.lrh.pktlen);
			const size_t total_bytes_to_copy = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
			const size_t payload_bytes_to_copy = total_bytes_to_copy - sizeof(union fi_opx_hfi1_packet_hdr);

			if (opcode >= FI_OPX_HFI_BTH_OPCODE_TAG_INJECT) {

				fi_opx_ep_rx_process_header_tag(ep, hdr, payload,
					payload_bytes_to_copy, opcode,
					(const uint8_t) origin_reliability_rx,
					1, /* is_intranode */
					lock_required, OFI_RELIABILITY_KIND_NONE);

			} else {

				fi_opx_ep_rx_process_header_msg(ep, hdr, payload,
					payload_bytes_to_copy, opcode,
					(const uint8_t) origin_reliability_rx,
					1,
					lock_required, OFI_RELIABILITY_KIND_NONE);
			}
		}

		opx_shm_rx_advance(&opx_ep->rx->shm, (void *)hdr, pos);
		packet = opx_shm_rx_next(&opx_ep->rx->shm, &pos);
		hdr = (packet) ? (union fi_opx_hfi1_packet_hdr *) packet->data : NULL;
	}
}



__OPX_FORCE_INLINE__
void fi_opx_hfi1_poll_many (struct fid_ep *ep,
		const int lock_required,
		const uint64_t caps,
		const enum ofi_reliability_kind reliability,
		const uint64_t hdrq_mask)
{
	/* All callers to this function should have already obtained the necessary lock */
	assert(!lock_required);

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	static const unsigned hfi1_poll_max = 256;
	unsigned hfi1_poll_count = 0;
	unsigned packets = 0;


	if ((caps & FI_LOCAL_COMM) || (caps == 0)) {
		fi_opx_shm_poll_many(ep, 0);
	}

	if ((caps & FI_REMOTE_COMM) || (caps == 0)) {
		do {
			packets = fi_opx_hfi1_poll_once(ep, FI_OPX_LOCK_NOT_REQUIRED, reliability, hdrq_mask);
		} while ((packets > 0) && (hfi1_poll_count++ < hfi1_poll_max));

		if (reliability == OFI_RELIABILITY_KIND_ONLOAD) {	/* compile-time constant expression */

			struct fi_opx_reliability_service *service = opx_ep->reliability->state.service;

			union fi_opx_timer_state *timer = &service->tx.timer;
			union fi_opx_timer_stamp *timestamp = &service->tx.timestamp;
			uint64_t compare = fi_opx_timer_now(timestamp, timer);

			//TODO: There needs to be feedback from the replay buffer pool into this following if as well
			//		If the pool is getting full, then send pings out more frequently

			if (OFI_UNLIKELY(compare > service->usec_next)) {
				// Drain all coalesced pings
				fi_opx_hfi_rx_reliablity_process_requests(ep, PENDING_RX_RELIABLITY_COUNT_MAX);
				fi_reliability_service_ping_remote(ep, service);
				// Fetch the timer again as it could have taken us a while to get through reliability
				fi_opx_timer_now(timestamp, timer);
				service->usec_next = fi_opx_timer_next_event_usec(timer, timestamp, service->usec_max);
			}// End timer fired


		}
	}

	fi_opx_compiler_msync_writes();  //Workaround for STL-62043


	return;
}





#endif /* _FI_PROV_OPX_HFI1_PROGRESS_H_ */
