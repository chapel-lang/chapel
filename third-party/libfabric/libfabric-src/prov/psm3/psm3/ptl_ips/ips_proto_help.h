/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2017 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef _IPS_PROTO_HELP_H
#define _IPS_PROTO_HELP_H

#include "ptl_ips.h"

/* hfi_opcode is not the ips-level opcode. */
PSMI_ALWAYS_INLINE(
uint8_t
_get_proto_hfi_opcode(const struct ips_message_header *p_hdr))
{
	return ((__be32_to_cpu(p_hdr->bth[0]) >>
		 HFI_BTH_OPCODE_SHIFT) & HFI_BTH_OPCODE_MASK);
}

/* convert a total packet length in bytes to the dword count used in lrh[2] */
PSMI_ALWAYS_INLINE(
__be16
ips_proto_bytes_to_lrh2_be(struct ips_proto *proto, uint32_t bytes))
{
	uint16_t words = (bytes >> BYTE2DWORD_SHIFT) & proto->pktlen_mask;
	return __cpu_to_be16(words);
}

/* convert a lrh[2] dword count to a total packet length in bytes */
PSMI_ALWAYS_INLINE(
uint32_t
ips_proto_lrh2_be_to_bytes(struct ips_proto *proto, __be16 lrh2))
{
	return (__be16_to_cpu(lrh2) & proto->pktlen_mask) << BYTE2DWORD_SHIFT;
}

PSMI_ALWAYS_INLINE(
uint8_t
ips_flow_gen_ackflags(ips_scb_t *scb, struct ips_flow *flow))
{
	/*
	 * Setup ACK request if more than ack_interval packets
	 * have not been requested an ACK
	 */
	if (scb->scb_flags & IPS_SEND_FLAG_ACKREQ || scb->nfrag > 1) {
		flow->ack_counter = 0;
#ifdef PSM_BYTE_FLOW_CREDITS
		flow->ack_counter_bytes = 0;
#endif
	} else {
		flow->ack_counter++;
#ifdef PSM_BYTE_FLOW_CREDITS
		flow->ack_counter_bytes += scb->chunk_size;
#endif
		if (flow->ack_counter > flow->ack_interval
#ifdef PSM_BYTE_FLOW_CREDITS
			|| flow->ack_counter_bytes > flow->ack_interval_bytes
#endif
			) {
			flow->ack_counter = 0;
#ifdef PSM_BYTE_FLOW_CREDITS
			flow->ack_counter_bytes = 0;
#endif
			scb->scb_flags |= IPS_SEND_FLAG_ACKREQ;
		}
	}

	/* Bottom 6 bits wind up in protocol header fields, other bits
	 * control other aspects of packet composition */
	return (uint8_t) (scb->scb_flags & IPS_SEND_FLAG_PROTO_OPTS);
}

PSMI_ALWAYS_INLINE(
ips_epaddr_flow_t
ips_proto_flowid(struct ips_message_header *p_hdr))
{
	return (ips_epaddr_flow_t) ((__be32_to_cpu(p_hdr->bth[1]) >>
				     HFI_BTH_FLOWID_SHIFT) &
				    HFI_BTH_FLOWID_MASK);
}

PSMI_ALWAYS_INLINE(
int
ips_do_cksum(struct ips_proto *proto, struct ips_message_header *p_hdr,
	     void *payload, uint32_t paylen, uint32_t *cksum))
{
	/* Update the payload words in header */
	p_hdr->lrh[2] = ips_proto_bytes_to_lrh2_be(proto,
				sizeof(struct ips_message_header) + paylen +
				PSM_CRC_SIZE_IN_BYTES + HFI_CRC_SIZE_IN_BYTES);

	/* Need to regenerate KDETH checksum after updating payload length */
	/* ips_kdeth_cksum(p_hdr); */

	*cksum = psm3_ips_cksum_calculate(p_hdr, (uint8_t *)payload, paylen);

	return 0;
}


PSMI_ALWAYS_INLINE(
void
ips_proto_hdr(struct ips_proto *proto, struct ips_epaddr *ipsaddr,
	      struct ips_flow *flow, ips_scb_t *scb, uint8_t flags))
{
	__be16 slid, dlid;
	__be16 lrh2_be = ips_proto_bytes_to_lrh2_be(proto,
				sizeof(struct ips_message_header) +
				scb->payload_size + HFI_CRC_SIZE_IN_BYTES);
	struct ips_message_header *p_hdr = &scb->ips_lrh;
#if 0
	/*
	 * This scb has been used by this connection last time,
	 * so some of the header fields are already set.
	 */
	if (scb->flow == flow) {
		p_hdr->lrh[2] = lrh2_be;

		p_hdr->bth[0] = __cpu_to_be32(flow->path->pr_pkey |
					      (scb->
					       opcode << BTH_OPCODE_SHIFT) |
					      (extra_bytes <<
					       BTH_EXTRA_BYTE_SHIFT));
		p_hdr->bth[2] =
		    __cpu_to_be32(flow->xmit_seq_num.
				  psn | (scb->scb_flags & IPS_SEND_FLAG_ACKREQ));

		p_hdr->khdr.kdeth0 = __cpu_to_le32(scb->offset |
						   (scb->
						    offset_mode <<
						    HFI_KHDR_OM_SHIFT)
						   (scb->
						    flags & IPS_SEND_FLAG_INTR)
						   | (scb->
						      flags &
						      IPS_SEND_FLAG_HDR_SUPPRESS)
						   | (IPS_PROTO_VERSION <<
						      HFI_KHDR_KVER_SHIFT));

		/* ips_kdeth_cksum(p_hdr); // Generate KDETH checksum */

		p_hdr->ack_seq_num = flow->recv_seq_num.psn;
		p_hdr->flags = flags;

		return;
	}
#endif
	slid = flow->path->pr_slid;
	dlid = flow->path->pr_dlid;
	if (scb->scb_flags & IPS_SEND_FLAG_NO_LMC) {
        	slid = ipsaddr->pathgrp->pg_base_slid;
        	dlid = ipsaddr->pathgrp->pg_base_dlid;
	}

	/* Setup LRH fields */
	p_hdr->lrh[0] = __cpu_to_be16(HFI_LRH_BTH |
				      ((flow->path->pr_sl & HFI_LRH_SL_MASK) <<
				       HFI_LRH_SL_SHIFT)
					);
	p_hdr->lrh[1] = dlid;
	p_hdr->lrh[2] = lrh2_be;
	p_hdr->lrh[3] = slid;

	/* Setup BTH fields */
	p_hdr->bth[0] = __cpu_to_be32(flow->path->pr_pkey |
			      (scb->opcode << HFI_BTH_OPCODE_SHIFT));
	p_hdr->bth[2] = __cpu_to_be32(flow->xmit_seq_num.psn_num |
				      (scb->scb_flags & IPS_SEND_FLAG_ACKREQ));

	{
		p_hdr->bth[1] = __cpu_to_be32((flow->flowid
						 << HFI_BTH_FLOWID_SHIFT));
		/* Setup KHDR fields */
		p_hdr->khdr.kdeth0 = __cpu_to_le32(p_hdr->khdr.kdeth0 |
						   (scb->scb_flags &
							IPS_SEND_FLAG_INTR)
						   | (IPS_PROTO_VERSION <<
						      HFI_KHDR_KVER_SHIFT));

		p_hdr->ack_seq_num = flow->recv_seq_num.psn_num;
	}

	p_hdr->khdr.job_key = 0;
	p_hdr->connidx = ipsaddr->connidx_outgoing;
	p_hdr->flags = flags;

	scb->flow = flow;

	return;
}

/*
 * Assumes that the following fields are already set in scb:
 * payload
 * payload_size
 * flags
 */
PSMI_INLINE(
void
ips_scb_prepare_flow_inner(struct ips_proto *proto, struct ips_epaddr *ipsaddr,
			   struct ips_flow *flow, ips_scb_t *scb))
{
	// On UD and UDP, ips_ptl_mq_rndv can allow small odd sized payload
	// in RTS and eager can do odd length send
	psmi_assert(psmi_hal_has_cap(PSM_HAL_CAP_NON_DW_PKT_SIZE)
			|| ((scb->payload_size & 3) == 0));
	ips_proto_hdr(proto, ipsaddr, flow, scb,
		      ips_flow_gen_ackflags(scb, flow));

	scb->ack_timeout = proto->epinfo.ep_timeout_ack;
	scb->abs_timeout = TIMEOUT_INFINITE;
	scb->scb_flags |= IPS_SEND_FLAG_PENDING;

	if (flow->protocol == PSM_PROTOCOL_TIDFLOW) {
		flow->xmit_seq_num.psn_seq += scb->nfrag;
		scb->seq_num = flow->xmit_seq_num;
		scb->seq_num.psn_seq--;
	} else {
		flow->xmit_seq_num.psn_num =
		    (flow->xmit_seq_num.psn_num + scb->nfrag) & proto->psn_mask;
		scb->seq_num.psn_num =
		    (flow->xmit_seq_num.psn_num - 1) & proto->psn_mask;
	}

	return;
}

PSMI_ALWAYS_INLINE(
void
ips_proto_epaddr_stats_set(struct ips_proto *proto, uint8_t msgtype))
{
	switch (msgtype) {
	case OPCODE_ACK:
		break;
	case OPCODE_ERR_CHK:
		proto->epaddr_stats.err_chk_send++;
		break;
	case OPCODE_NAK:
		proto->epaddr_stats.nak_send++;
		break;
	case OPCODE_CONNECT_REQUEST:
		proto->epaddr_stats.connect_req_send++;
		break;
	case OPCODE_CONNECT_REPLY:
		proto->epaddr_stats.connect_rep_send++;
		break;
	case OPCODE_DISCONNECT_REQUEST:
		proto->epaddr_stats.disconnect_req_send++;
		break;
	case OPCODE_DISCONNECT_REPLY:
		proto->epaddr_stats.disconnect_rep_send++;
		break;
	default:
		break;
	}
	return;
}

/*
 * Exported there solely for inlining is_expected_or_nak and mq_tiny handling
 */
extern
psm2_error_t psm3_ips_proto_send_ctrl_message(struct ips_flow *flow,
		uint8_t message_type, uint16_t *msg_queue_mask,
		ips_scb_t *ctrlscb, void *payload, uint32_t paylen);

PSMI_ALWAYS_INLINE(
void
ips_proto_send_ack(struct ips_recvhdrq *recvq, struct ips_flow *flow))
{
	if_pt(recvq->proto->flags & IPS_PROTO_FLAG_COALESCE_ACKS) {
		if (flow->flags & IPS_FLOW_FLAG_PENDING_NAK) {
			flow->flags &= ~IPS_FLOW_FLAG_PENDING_NAK;	/* ACK clears NAK */
		} else if (!(flow->flags & IPS_FLOW_FLAG_PENDING_ACK)) {
			SLIST_INSERT_HEAD(&recvq->pending_acks, flow, next);
		}

		flow->flags |= IPS_FLOW_FLAG_PENDING_ACK;
	}
	else {
		ips_scb_t ctrlscb;

		ctrlscb.scb_flags = 0;
		ctrlscb.ips_lrh.ack_seq_num = flow->recv_seq_num.psn_num;
		/* Coalesced ACKs disabled. Send ACK immediately */
		// no payload, pass cksum so non-NULL
		psm3_ips_proto_send_ctrl_message(flow, OPCODE_ACK,
					    &flow->ipsaddr->ctrl_msg_queued,
					    &ctrlscb, ctrlscb.cksum, 0);
	}
}

PSMI_ALWAYS_INLINE(
void
ips_proto_send_nak(struct ips_recvhdrq *recvq, struct ips_flow *flow))
{
	if_pt(recvq->proto->flags & IPS_PROTO_FLAG_COALESCE_ACKS) {
		if (flow->flags & IPS_FLOW_FLAG_PENDING_ACK) {
			flow->flags &= ~IPS_FLOW_FLAG_PENDING_ACK;	/* NAK clears ACK */
		} else if (!(flow->flags & IPS_FLOW_FLAG_PENDING_NAK)) {
			SLIST_INSERT_HEAD(&recvq->pending_acks, flow, next);
		}

		flow->flags |= IPS_FLOW_FLAG_PENDING_NAK;
	}
	else {
		ips_scb_t ctrlscb;

		ctrlscb.scb_flags = 0;
		ctrlscb.ips_lrh.ack_seq_num = flow->recv_seq_num.psn_num;
		/* Coalesced ACKs disabled. Send NAK immediately */
		// no payload, pass cksum so non-NULL
		psm3_ips_proto_send_ctrl_message(flow, OPCODE_NAK,
					    &flow->ipsaddr->ctrl_msg_queued,
					    &ctrlscb, ctrlscb.cksum, 0);
	}
}

/* return 1 if packet is next expected in flow
 * return 0 if packet is not next expected in flow (and nak packet).
 */
PSMI_ALWAYS_INLINE(
int
ips_proto_is_expected_or_nak(struct ips_recvhdrq_event *rcv_ev))
{
	struct ips_proto *proto = rcv_ev->proto;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_flow_t flowid = ips_proto_flowid(p_hdr);
	struct ips_flow *flow;
	psmi_seqnum_t sequence_num;

	psmi_assert(flowid == EP_FLOW_GO_BACK_N_PIO);
	flow = &ipsaddr->flows[flowid];

	sequence_num.psn_val = __be32_to_cpu(p_hdr->bth[2]);
	if_pf(flow->recv_seq_num.psn_num == sequence_num.psn_num) {
		flow->flags &= ~IPS_FLOW_FLAG_NAK_SEND;

		flow->recv_seq_num.psn_num =
		    (flow->recv_seq_num.psn_num + 1) & proto->psn_mask;

		/* don't process ack, caller will do it. */
		return 1;

	}

	int16_t diff = (int16_t) (sequence_num.psn_num -
			       flow->recv_seq_num.psn_num);
	if (diff > 0) {
		if (!(flow->flags & IPS_FLOW_FLAG_NAK_SEND)) {
			/* Queue/Send NAK to peer  */
			ips_proto_send_nak((struct ips_recvhdrq *)
					   rcv_ev->recvq, flow);
			flow->flags |= IPS_FLOW_FLAG_NAK_SEND;
		}
	}

	/* process ack if packet is not in sequence. */
	psm3_ips_proto_process_ack(rcv_ev);

	return 0;
}

/*
 * Note, some code depends on the literal values specified in this enum.
 */
enum ips_msg_order {
	IPS_MSG_ORDER_PAST  = 3,	/* Old message, recv & drop */
	IPS_MSG_ORDER_EXPECTED_MATCH = 2, /* Expected message, recv on match */
	IPS_MSG_ORDER_EXPECTED = 1,	/* Expected message, always recv */
	IPS_MSG_ORDER_FUTURE_RECV = 0,	/* Future message, buffer in OOO Q */
	IPS_MSG_ORDER_FUTURE = -1,	/* Future message, leave on RHQ */
};

PSMI_ALWAYS_INLINE(
enum ips_msg_order
ips_proto_check_msg_order(ips_epaddr_t *ipsaddr,
			 struct ips_flow *flow,
			 uint16_t send_seqnum,
			 uint16_t *recv_seqnum))

{
	int16_t diff = (int16_t) (*recv_seqnum - send_seqnum);

	if (likely(diff == 0)) {
		*recv_seqnum += 1;

		ipsaddr->msg_toggle ^= IPS_FLOW_MSG_TOGGLE_UNEXP_MASK;
		if (ipsaddr->msg_toggle & IPS_FLOW_MSG_TOGGLE_UNEXP_MASK)
			return IPS_MSG_ORDER_EXPECTED_MATCH;

		return IPS_MSG_ORDER_EXPECTED;
	} else if (diff > 0) {
		return IPS_MSG_ORDER_PAST;
	}

	ipsaddr->msg_toggle ^= IPS_FLOW_MSG_TOGGLE_OOO_MASK;
	if (!(ipsaddr->msg_toggle & IPS_FLOW_MSG_TOGGLE_OOO_MASK)) {
		/*
		 * Second time to see the same ooo message, receive and put
		 * into OOO queue.
		 */
		return IPS_MSG_ORDER_FUTURE_RECV;
	}

	/* The first time to see an OOO message, leave it there and try
	 * next time. But we need to revert back the receiving flow PSN. */
	uint32_t psn_mask = ((psm2_epaddr_t)ipsaddr)->proto->psn_mask;
	flow->recv_seq_num.psn_num =
		(flow->recv_seq_num.psn_num - 1) & psn_mask;
	return IPS_MSG_ORDER_FUTURE;
}

PSMI_INLINE(
int
ips_proto_process_packet(const struct ips_recvhdrq_event *rcv_ev,
		ips_packet_service_fn_t *packet_service_routines))
{
	uint32_t index;

#ifdef PSM_FI

	if_pf(PSM3_FAULTINJ_ENABLED_EP(rcv_ev->proto->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_recv, "recvlost",
					  "drop "
#ifdef PSM_VERBS
#ifdef USE_RC
					  "RC eager or any UD "
#endif
#endif
					  "packet at recv",
					   1, IPS_FAULTINJ_RECVLOST);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_recv, rcv_ev->proto->ep, ""))
			return IPS_RECVHDRQ_CONTINUE;
	}
#endif // PSM_FI
	/* see file ips_proto_header.h for details */
	index = _get_proto_hfi_opcode(rcv_ev->p_hdr) - OPCODE_RESERVED;
	if (index >= (OPCODE_FUTURE_FROM - OPCODE_RESERVED))
		index = 0;

	return packet_service_routines[index]
			((struct ips_recvhdrq_event *)rcv_ev);
}

/*
 * Breaks header encapsulation but needed in mq sends so we can pay
 * "near-equal" attention to putting sends on the wire and servicing the
 * receive queue.
 */

PSMI_ALWAYS_INLINE(
psm2_error_t
ips_recv_progress_if_busy(ptl_t *ptl_gen, psm2_error_t err))
{
	struct ptl_ips *ptl = (struct ptl_ips *) ptl_gen;

	if (err == PSM2_EP_NO_RESOURCES) {
		ptl->ctl->ep_poll(ptl_gen, 0, 1);
		return PSM2_OK;
	} else
		return err;
}

/* Find next lowest power of a two for a 32 bit number*/
PSMI_ALWAYS_INLINE(
unsigned int
ips_next_low_pow2(unsigned int v))
{

	const unsigned int b[] = { 0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
	const unsigned int S[] = { 1, 2, 4, 8, 16 };
	register unsigned int r = 1;
	int i;

	for (i = 4; i >= 0; i--) {
		if (v & b[i]) {
			v >>= S[i];
			r <<= S[i];
		}
	}

	return r;
}

PSMI_ALWAYS_INLINE(
ips_path_rec_t *
ips_select_path(struct ips_proto *proto, ips_path_type_t path_type,
		ips_epaddr_t *ipsaddr, ips_path_grp_t *pathgrp))
{
	uint32_t path_idx;

	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE) {
		/* If dispersive routes are configured then select the routes
		 * in round robin order. We may want to use congestion
		 * information to select the least lightly loaded path.
		 */
		path_idx = pathgrp->pg_next_path[path_type];
		if (++pathgrp->pg_next_path[path_type] >=
		    pathgrp->pg_num_paths[path_type])
			pathgrp->pg_next_path[path_type] = 0;
	} else if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_DST)
		path_idx =	/* Key on destination context */
		    ipsaddr->hash % pathgrp->pg_num_paths[path_type];
	else if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_SRC)
		path_idx =	/* Key off src context */
		    proto->epinfo.ep_hash % pathgrp->pg_num_paths[path_type];
	else			/* Base LID routed - Default in Infinhfi 2.5 (Oct 09). */
		path_idx = 0;

	return pathgrp->pg_path[path_idx][path_type];
}

PSMI_ALWAYS_INLINE(
void
process_pending_acks(struct ips_recvhdrq *recvq))
{
	ips_scb_t ctrlscb;
	struct ips_message_header *msg_hdr = NULL;

	/* If any pending acks, dispatch them now */
	while (!SLIST_EMPTY(&recvq->pending_acks)) {
		struct ips_flow *flow = SLIST_FIRST(&recvq->pending_acks);

		SLIST_REMOVE_HEAD(&recvq->pending_acks, next);
		SLIST_NEXT(flow, next) = NULL;

		ctrlscb.scb_flags = 0;
		msg_hdr = &ctrlscb.ips_lrh;
		msg_hdr->ack_seq_num = flow->recv_seq_num.psn_num;

		if (flow->flags & IPS_FLOW_FLAG_PENDING_ACK) {
			psmi_assert_always((flow->
					    flags & IPS_FLOW_FLAG_PENDING_NAK)
					   == 0);

			flow->flags &= ~IPS_FLOW_FLAG_PENDING_ACK;
			// no payload, pass cksum so non-NULL
			psm3_ips_proto_send_ctrl_message(flow, OPCODE_ACK,
						    &flow->ipsaddr->
						    ctrl_msg_queued,
						    &ctrlscb, ctrlscb.cksum, 0);
		} else {
			psmi_assert_always(flow->
					   flags & IPS_FLOW_FLAG_PENDING_NAK);

			flow->flags &= ~IPS_FLOW_FLAG_PENDING_NAK;
			// no payload, pass cksum so non-NULL
			psm3_ips_proto_send_ctrl_message(flow, OPCODE_NAK,
						    &flow->ipsaddr->
						    ctrl_msg_queued,
						    &ctrlscb, ctrlscb.cksum, 0);
		}
	}
}

#endif /* _IPS_PROTO_HELP_H */
