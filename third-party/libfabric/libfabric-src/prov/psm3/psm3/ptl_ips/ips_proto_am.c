/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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

  Copyright(c) 2015 Intel Corporation.

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

#include "psm_user.h"
#include "psm2_hal.h"
#include "psm2_am.h"
#include "psm_am_internal.h"
#include "psm_mq_internal.h"
#include "ips_proto.h"
#include "ips_expected_proto.h"
#include "ips_proto_help.h"

struct ips_am_token {
	struct psmi_am_token tok;

	/* ptl-specific token stuff */
	struct ips_epaddr *epaddr_rail;
	struct ips_proto_am *proto_am;
};

struct ips_am_message {
	struct ips_message_header p_hdr;
	struct ips_am_message *next;
	struct ips_epaddr *ipsaddr;
	struct ips_proto_am *proto_am;
	uint64_t *payload;
	uint32_t paylen;
	uint16_t seqnum;
};

/* These variables are shared for all packet flows in a PSM process; they are
 * shared across multiple rails.  There is no single AM object to hang these
 * off of, so they are declared here as globals. */
static struct {
	struct ips_am_message head;
	struct ips_am_message *tail;
} ips_am_outoforder_q;

static mpool_t ips_am_msg_pool;

/* This calculation ensures that the number of reply slots will always be at
 * least twice as large + 1 as the number of request slots. This is optimal: the
 * minimum amount required is actually only twice as many, but it is much
 * slower. */
#define calc_optimal_num_reply_slots(nslots) (((nslots)*2 / 3) + 1)

psm2_error_t
MOCKABLE(psm3_ips_proto_am_init)(struct ips_proto *proto,
		  int num_send_slots,
		  uint32_t imm_size,
		  struct ips_proto_am *proto_am)
{
	psm2_error_t err = PSM2_OK;
	int send_buf_size = proto->epinfo.ep_mtu;
	int num_rep_slots = calc_optimal_num_reply_slots(num_send_slots);
	int num_req_slots = num_send_slots - num_rep_slots;

	proto_am->proto = proto;

	/* In a node pair, the number of reply send buffers on at least one of
	 * the nodes must be at least double the number (optimal: double + 1) of
	 * send descriptors on the other node. While this constraint applies
	 * only to the reply send buffers, allowing the caller to tune only the
	 * number of request send buffers would be awkward, as they have no
	 * knowledge of the subdivision of the memory into separate mempools for
	 * requests and replies. It's an internal concern at this point. */
	if ((err = psm3_ips_scbctrl_init(proto->ep,
				    num_req_slots,
				    num_req_slots,
				    imm_size,
				    send_buf_size,
				    NULL,
				    NULL,
				    &proto_am->scbc_request)))
		goto fail;

	if ((err = psm3_ips_scbctrl_init(proto->ep,
				    num_rep_slots,
				    num_rep_slots,
				    imm_size,
				    send_buf_size,
				    NULL,
				    NULL,
				    &proto_am->scbc_reply)))
		goto fail;

	if (ips_am_msg_pool == NULL) {
		union psmi_envvar_val max_msgs;

		ips_am_outoforder_q.head.next = NULL;
		ips_am_outoforder_q.tail = &ips_am_outoforder_q.head;

		psm3_getenv("PSM3_AM_MAX_OOO_MSGS",
			"Maximum number of OOO Active Messages to queue before dropping.",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1024, &max_msgs);

		ips_am_msg_pool = psm3_mpool_create(
				sizeof(struct ips_am_message),
				32, max_msgs.e_uint, 0, UNDEFINED, NULL, NULL);
	}
fail:
	return err;
}
MOCK_DEF_EPILOGUE(psm3_ips_proto_am_init);

psm2_error_t psm3_ips_proto_am_fini(struct ips_proto_am *proto_am)
{
	psm3_ips_scbctrl_fini(&proto_am->scbc_request);
	psm3_ips_scbctrl_fini(&proto_am->scbc_reply);
	if (ips_am_msg_pool != NULL) {
		psm3_mpool_destroy(ips_am_msg_pool);
		ips_am_msg_pool = NULL;
	}

	return PSM2_OK;
}

/* Fill in AM capabilities parameters */
psm2_error_t
psm3_ips_am_get_parameters(psm2_ep_t ep, struct psm2_am_parameters *parameters)
{
	int max_nargs = min(1 << IPS_AM_HDR_NARGS_BITS, PSMI_AM_MAX_ARGS);
	int max_payload =
		ep->mtu -
		((max_nargs - IPS_AM_HDR_NARGS) * sizeof(psm2_amarg_t));

	if (parameters == NULL) {
		return PSM2_PARAM_ERR;
	}

	parameters->max_handlers = 1 << IPS_AM_HDR_HIDX_BITS;
	parameters->max_nargs = max_nargs;
	parameters->max_request_short = max_payload;
	parameters->max_reply_short = max_payload;

	return PSM2_OK;
}

static
psm2_error_t
am_short_reqrep(ips_scb_t *scb, struct ips_epaddr *ipsaddr,
		psm2_amarg_t *args, int nargs, uint8_t opcode,
		void *src, size_t len, int flags, int pad_bytes)
{
	int i, hdr_qwords = IPS_AM_HDR_NARGS;
	struct ips_proto *proto = ((psm2_epaddr_t)ipsaddr)->proto;

	psmi_assert(proto->msgflowid < EP_NUM_FLOW_ENTRIES);

	struct ips_flow *flow = &ipsaddr->flows[proto->msgflowid];

	/* There are a limited number of bits for nargs in the header, making
	   overflow very easy.  Make sure the values match. */
	psmi_assert(nargs == scb->ips_lrh.amhdr_nargs);

	_HFI_VDBG("%s src=%p len=%d, nargs=%d\n",
		  ((opcode == OPCODE_AM_REQUEST) ||
		   (opcode == OPCODE_AM_REQUEST_NOREPLY)) ? "req" : "rep",
		  src, (int)len, nargs);

	if (nargs == 1) {	/* fastpath */
		scb->ips_lrh.data[0].u64w0 = args[0].u64w0;
		hdr_qwords--;
	} else if (nargs > 1) {
		/* Easily unrollable but leave as is in case we can increase
		 * qwords on the chip in the near future */
		for (i = 0; i < IPS_AM_HDR_NARGS; i++, hdr_qwords--)
			scb->ips_lrh.data[i].u64w0 = args[i].u64w0;

		if (nargs > IPS_AM_HDR_NARGS) {
			/* Slow case -- we don't have iovec and not enough
			 * space in the message header, so we have to copy the
			 * user's arguments even if the payload is marked ASYNC
			 */
			uintptr_t bufp = (uintptr_t) ips_scb_buffer(scb);
			size_t arg_payload_len =
			    sizeof(psm2_amarg_t) * (nargs - IPS_AM_HDR_NARGS);

			psm3_mq_mtucpy((void *)bufp,
				       &args[IPS_AM_HDR_NARGS],
				       arg_payload_len);
			bufp += arg_payload_len;
			scb->payload_size = arg_payload_len;

			if (src != NULL && len > 0) {
				psm3_mq_mtucpy((void *)bufp, src, len);
				scb->payload_size += len;
			}

			psmi_assert(pad_bytes < (1 << IPS_AM_HDR_LEN_BITS));
			scb->payload_size += pad_bytes;
			scb->ips_lrh.amhdr_len = pad_bytes;
			goto send_scb;
		}
	}

	if (len == 0) {
		scb->payload_size = 0;
		scb->ips_lrh.amhdr_len = 0;
	} else if (len <= (hdr_qwords << 3)) {
		/* Inline the payload into the header. */
		/* This path CANNOT handle length = 0 due to limited space
		   in the header.  If IPS_SEND_FLAG_AMISTINY is set, an
		   amhdr_len value of 0 means a full payload, i.e.
		   1 << IPS_AM_HDR_LEN_BITS bytes of packed payload. */
		psmi_assert(len > 0);

		psm3_mq_mtucpy(&scb->ips_lrh.
			       data[IPS_AM_HDR_NARGS - hdr_qwords], src, len);
		scb->payload_size = 0;
		psmi_assert(len <= (1 << IPS_AM_HDR_LEN_BITS));
		scb->ips_lrh.amhdr_len = len & ((1 << IPS_AM_HDR_LEN_BITS) - 1);
		scb->scb_flags |= IPS_SEND_FLAG_AMISTINY;
	} else { /* Whatever's left requires a separate payload */
		if (ips_scb_buffer(scb) == NULL) /* Just attach the buffer */
			ips_scb_buffer(scb) = src;
		else /* May need to re-xmit user data, keep it around */
			psm3_mq_mtucpy(ips_scb_buffer(scb), src, len);

		psmi_assert(pad_bytes < (1 << IPS_AM_HDR_LEN_BITS));
		scb->payload_size = len + pad_bytes;
		scb->ips_lrh.amhdr_len = pad_bytes;
	}

send_scb:
	ips_scb_opcode(scb) = opcode;
	scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(ipsaddr->msgctl->am_send_seqnum);
	ipsaddr->msgctl->am_send_seqnum++;
	psm3_ips_proto_flow_enqueue(flow, scb);
	flow->flush(flow, NULL);

	return PSM2_OK;
}

static inline int
calculate_pad_bytes(size_t len)
{
	/* Align to dword (4 bytes) */
	size_t dword_aligned_len = (len + 3) & ~3;
	return dword_aligned_len - len;
}

static inline
void
ips_am_scb_init(ips_scb_t *scb, uint8_t handler, int nargs,
		int pad_bytes,
		psm2_am_completion_fn_t completion_fn, void *completion_ctxt)
{
	psmi_assert(pad_bytes < (1 << IPS_AM_HDR_LEN_BITS));

	scb->completion_am = completion_fn;
	scb->cb_param = completion_ctxt;
	scb->ips_lrh.amhdr_hidx = handler;
	scb->ips_lrh.amhdr_len = pad_bytes;
	scb->ips_lrh.amhdr_nargs = nargs;
	scb->ips_lrh.flags = 0;
	if (completion_fn)
		scb->scb_flags |= IPS_SEND_FLAG_ACKREQ;
	return;
}

psm2_error_t
psm3_ips_am_short_request(psm2_epaddr_t epaddr,
		     psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		     void *src, size_t len, int flags,
		     psm2_am_completion_fn_t completion_fn,
		     void *completion_ctxt)
{
	struct ips_proto_am *proto_am = &epaddr->proto->proto_am;
	psm2_error_t err;
	ips_scb_t *scb;
	ips_epaddr_t *ipsaddr;
	int pad_bytes = calculate_pad_bytes(len);
	int payload_sz = (nargs << 3);

	if_pt(!(flags & PSM2_AM_FLAG_ASYNC))
	    payload_sz += len;

	if (payload_sz > (IPS_AM_HDR_NARGS << 3)) {
		/* Payload can't fit in header, allocate buffer to carry data */
		int arg_sz = (nargs > IPS_AM_HDR_NARGS) ?
		    ((nargs - IPS_AM_HDR_NARGS) << 3) : 0;

		/* len + pad_bytes + overflow_args */
		PSMI_BLOCKUNTIL(epaddr->ptlctl->ep,
				err,
				((scb = psm3_ips_scbctrl_alloc(
				      &proto_am->scbc_request,
				      1,
				      len + pad_bytes + arg_sz,
				      IPS_SCB_FLAG_ADD_BUFFER)) != NULL));
	} else {
		PSMI_BLOCKUNTIL(epaddr->ptlctl->ep,
				err,
				((scb = psm3_ips_scbctrl_alloc_tiny(
				      &proto_am->scbc_request)) != NULL));
	}

	psmi_assert_always(scb != NULL);
	ips_am_scb_init(scb, handler, nargs, pad_bytes,
			completion_fn, completion_ctxt);

	if (payload_sz >= epaddr->proto->multirail_thresh_load_balance) {
		/* Select the next ipsaddr for multi-rail */
		ipsaddr = ((ips_epaddr_t *)epaddr)->msgctl->ipsaddr_next;
		ipsaddr->msgctl->ipsaddr_next = ipsaddr->next;
	} else {
		ipsaddr = (ips_epaddr_t *)epaddr;
	}

	return am_short_reqrep(scb, ipsaddr, args,
			       nargs,
			       (flags & PSM2_AM_FLAG_NOREPLY) ?
			       OPCODE_AM_REQUEST_NOREPLY : OPCODE_AM_REQUEST,
			       src, len, flags, pad_bytes);
}

psm2_error_t
psm3_ips_am_short_reply(psm2_am_token_t tok,
		   psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		   void *src, size_t len, int flags,
		   psm2_am_completion_fn_t completion_fn, void *completion_ctxt)
{
	struct ips_am_token *token = (struct ips_am_token *)tok;
	struct ips_proto_am *proto_am = token->proto_am;
	struct ips_epaddr *ipsaddr = token->epaddr_rail;
	int pad_bytes = calculate_pad_bytes(len);
	int scb_flags = 0;
	ips_scb_t *scb;

	if (!token->tok.can_reply) {
		_HFI_ERROR("Invalid AM reply for request!");
		return PSM2_AM_INVALID_REPLY;
	}

	psmi_assert(psm3_ips_scbctrl_avail(&proto_am->scbc_reply));

	if ((nargs << 3) + len <= (IPS_AM_HDR_NARGS << 3)) {
		scb = psm3_ips_scbctrl_alloc_tiny(&proto_am->scbc_reply);
	} else {
		int payload_sz = (nargs << 3);

		payload_sz += (flags & PSM2_AM_FLAG_ASYNC) ?
			      0 : (len + pad_bytes);
		scb_flags |= (payload_sz > (IPS_AM_HDR_NARGS << 3)) ?
		    IPS_SCB_FLAG_ADD_BUFFER : 0;

		scb =
		    psm3_ips_scbctrl_alloc(&proto_am->scbc_reply, 1, payload_sz,
				      scb_flags);
	}

	psmi_assert_always(scb != NULL);
	ips_am_scb_init(scb, handler, nargs, pad_bytes,
			completion_fn, completion_ctxt);
	am_short_reqrep(scb, ipsaddr, args, nargs, OPCODE_AM_REPLY,
			src, len, flags, pad_bytes);
	return PSM2_OK;
}

/* Prepares and runs a handler from a receive event. */
static int
ips_am_run_handler(const struct ips_message_header *p_hdr,
		struct ips_epaddr *ipsaddr, struct ips_proto_am *proto_am,
		uint64_t *payload,
		uint32_t paylen)
{
	struct ips_am_token token;
	int nargs = p_hdr->amhdr_nargs;
	int ret;
	struct psm2_ep_am_handle_entry *hentry;
	psm2_amarg_t *args = (psm2_amarg_t *)p_hdr->data;

	token.tok.flags = p_hdr->flags;
	token.tok.epaddr_incoming = (psm2_epaddr_t)&ipsaddr->msgctl->master_epaddr;
	token.tok.can_reply =
		(_get_proto_hfi_opcode(p_hdr) == OPCODE_AM_REQUEST);
	token.epaddr_rail = ipsaddr;
	token.proto_am = proto_am;

	if (token.tok.flags & IPS_SEND_FLAG_AMISTINY) {
		/* Payload is packed into header after args */
		payload = (uint64_t *)&p_hdr->data[nargs].u64;
		paylen = p_hdr->amhdr_len;
		/* Interpret amhdr_len == 0 as 16 bytes of payload */
		if (paylen == 0)
			paylen = 1 << IPS_AM_HDR_LEN_BITS;
	} else {
		if (nargs > IPS_AM_HDR_NARGS) {
			/* Args are split across header and payload */
			int payload_args_len =
				(nargs - IPS_AM_HDR_NARGS) *
				sizeof(psm2_amarg_t);

			args = alloca(PSMI_AM_MAX_ARGS * sizeof(psm2_amarg_t));

			args[0].u64 = p_hdr->data[0].u64;
			args[1].u64 = p_hdr->data[1].u64;

			memcpy(&args[2], payload, payload_args_len);

			payload += nargs - IPS_AM_HDR_NARGS;
			paylen -= payload_args_len;
		}

		/* Subtract off padding bytes (dword padding) for non-TINY. */
		paylen -= p_hdr->amhdr_len;
	}

	hentry = psm_am_get_handler_function(proto_am->proto->ep,
			p_hdr->amhdr_hidx);

	/* Note a guard here for hentry != NULL is not needed because at
	 * initialization, a psmi_assert_always() assure the entry will be
	 * non-NULL. */

	if (likely(hentry->version == PSM2_AM_HANDLER_V2)) {
		psm2_am_handler_2_fn_t hfn2 =
				(psm2_am_handler_2_fn_t)hentry->hfn;
		ret = hfn2(&token, args, nargs, payload, paylen, hentry->hctx);
	} else {
		psm2_am_handler_fn_t hfn1 =
				(psm2_am_handler_fn_t)hentry->hfn;
		ret = hfn1(&token, args, nargs, payload, paylen);
	}

	return ret;
}

static int
ips_proto_am_handle_outoforder_queue()
{
	struct ips_am_message *msg, *prev;
	int ret = IPS_RECVHDRQ_CONTINUE;

	prev = &ips_am_outoforder_q.head;
	msg = ips_am_outoforder_q.head.next;

	while (msg != NULL) {
		struct ips_epaddr *ipsaddr = msg->ipsaddr;
		if (ipsaddr->msgctl->am_recv_seqnum != msg->seqnum) {
			prev = msg;
			msg = msg->next;
			continue;
		}

		ipsaddr->msgctl->am_recv_seqnum++;

		if (ips_am_run_handler(&msg->p_hdr,
					ipsaddr, msg->proto_am,
					msg->payload, msg->paylen))
			ret = IPS_RECVHDRQ_BREAK;

		prev->next = msg->next;
		if (prev->next == NULL)
			ips_am_outoforder_q.tail = prev;

		psm3_mq_sysbuf_free(msg->proto_am->proto->mq, msg->payload);
		psm3_mpool_put(msg);

		msg = prev->next;
	}

	return ret;
}

static void
ips_proto_am_queue_msg(struct ips_am_message *msg)
{
	msg->next = NULL;
	ips_am_outoforder_q.tail->next = msg;
	ips_am_outoforder_q.tail = msg;
}

int psm3_ips_proto_am(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	struct ips_epaddr *ipsaddr = rcv_ev->ipsaddr;
	struct ips_proto_am *proto_am = &rcv_ev->proto->proto_am;
	ips_epaddr_flow_t flowid = ips_proto_flowid(p_hdr);
	struct ips_flow *flow;
	struct ips_am_message *msg = NULL;
	int ret = IPS_RECVHDRQ_CONTINUE;
	enum ips_msg_order msgorder;

	psmi_assert(flowid < EP_NUM_FLOW_ENTRIES);
	flow = &ipsaddr->flows[flowid];
	/*
	 * Based on AM request/reply traffic pattern, if we don't have a reply
	 * scb slot then we can't process the request packet, we just silently
	 * drop it.  Otherwise, it will be a deadlock.  note:
	 * ips_proto_is_expected_or_nak() can not be called in this case.
	 */
	if (_get_proto_hfi_opcode(p_hdr) == OPCODE_AM_REQUEST &&
	    !psm3_ips_scbctrl_avail(&proto_am->scbc_reply))
		return IPS_RECVHDRQ_CONTINUE;

	if (!ips_proto_is_expected_or_nak(rcv_ev))
		return IPS_RECVHDRQ_CONTINUE;

	uint16_t send_msgseq =
	    __le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK;
	msgorder = ips_proto_check_msg_order(ipsaddr, flow, send_msgseq,
			&ipsaddr->msgctl->am_recv_seqnum);

	if (msgorder == IPS_MSG_ORDER_FUTURE)
		return IPS_RECVHDRQ_REVISIT;
	else if (msgorder == IPS_MSG_ORDER_FUTURE_RECV) {
		uint64_t *msg_payload;
		uint64_t *payload = ips_recvhdrq_event_payload(rcv_ev);
		uint32_t paylen = ips_recvhdrq_event_paylen(rcv_ev);

		psmi_assert(paylen == 0 || payload);
		msg = psm3_mpool_get(ips_am_msg_pool);
		if (unlikely(msg == NULL)) {
			/* Out of memory, drop the packet. */
			flow->recv_seq_num.psn_num =
				(flow->recv_seq_num.psn_num - 1) &
				rcv_ev->proto->psn_mask;
			return IPS_RECVHDRQ_BREAK;
		}
		msg_payload = psm3_mq_sysbuf_alloc(
				proto_am->proto->mq,
				ips_recvhdrq_event_paylen(rcv_ev));
		if (unlikely(msg_payload == NULL)) {
			/* Out of memory, drop the packet. */
			flow->recv_seq_num.psn_num =
				(flow->recv_seq_num.psn_num - 1) &
				rcv_ev->proto->psn_mask;
			psm3_mpool_put(msg);
			return IPS_RECVHDRQ_BREAK;
		}

		memcpy(&msg->p_hdr, p_hdr, sizeof(struct ips_message_header));
		memcpy(msg_payload, payload, paylen);

		msg->payload = msg_payload;
		msg->ipsaddr = ipsaddr;
		msg->proto_am = proto_am;
		msg->paylen = paylen;
		msg->seqnum =
			__le32_to_cpu(p_hdr->khdr.kdeth0) &
			HFI_KHDR_MSGSEQ_MASK;

		ips_proto_am_queue_msg(msg);
	} else if ((msgorder == IPS_MSG_ORDER_EXPECTED) ||
		   (msgorder == IPS_MSG_ORDER_EXPECTED_MATCH)) {
		uint64_t *payload = ips_recvhdrq_event_payload(rcv_ev);
		uint32_t paylen = ips_recvhdrq_event_paylen(rcv_ev);

		psmi_assert(paylen == 0 || payload);
		if (ips_am_run_handler(p_hdr, ipsaddr, proto_am,
					payload, paylen))
			ret = IPS_RECVHDRQ_BREAK;

		ips_proto_am_handle_outoforder_queue();
	}

	/* Look if the handler replied, if it didn't, ack the request */
	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);
	return ret;
}
