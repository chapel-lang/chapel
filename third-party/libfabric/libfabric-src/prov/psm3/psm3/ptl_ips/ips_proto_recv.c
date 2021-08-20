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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_proto_internal.h"

/* receive service routine for each packet opcode */
ips_packet_service_fn_t
ips_packet_service_routine[OPCODE_FUTURE_FROM-OPCODE_RESERVED] = {
ips_proto_process_unknown_opcode,	/* 0xC0 */
ips_proto_mq_handle_tiny,		/* OPCODE_TINY */
ips_proto_mq_handle_short,
ips_proto_mq_handle_eager,
ips_proto_mq_handle_rts,                /* RTS */
ips_proto_mq_handle_cts,                /* CTS */
ips_proto_mq_handle_data,               /* DATA */
#ifdef RNDV_MOD
ips_protoexp_process_err_chk_rdma,		/* ERR_CHK_RDMA */
ips_protoexp_process_err_chk_rdma_resp,		/* ERR_CHK_RDMA_RESP */
#else
ips_proto_process_unknown_opcode,
ips_proto_process_unknown_opcode,
#endif
/* these are control packets */
ips_proto_process_ack,
ips_proto_process_nak,
ips_proto_process_unknown_opcode,		/* BECN */
ips_proto_process_err_chk,
// ERR_CHK_GEN only valid for STL100 HW TIDFLOW
ips_proto_process_unknown_opcode,		/* ERR_CHK_GEN */
ips_proto_connect_disconnect,
ips_proto_connect_disconnect,
ips_proto_connect_disconnect,
ips_proto_connect_disconnect,
/* rest are not control packets */
ips_proto_am,
ips_proto_am,
ips_proto_am				/* OPCODE_AM_REPLY */
};

static void ips_report_strays(struct ips_proto *proto);

#define INC_TIME_SPEND(timer)

psm2_error_t ips_proto_recv_init(struct ips_proto *proto)
{
	uint32_t interval_secs;
	union psmi_envvar_val env_stray;

	psmi_getenv("PSM3_STRAY_WARNINTERVAL",
		    "min secs between stray process warnings",
		    PSMI_ENVVAR_LEVEL_HIDDEN,
		    PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)PSM_STRAY_WARN_INTERVAL_DEFAULT_SECS,
		    &env_stray);
	interval_secs = env_stray.e_uint;
	if (interval_secs > 0)
		proto->stray_warn_interval = sec_2_cycles(interval_secs);
	else
		proto->stray_warn_interval = 0;

	return PSM2_OK;
}

psm2_error_t ips_proto_recv_fini(struct ips_proto *proto)
{
	ips_report_strays(proto);
	return PSM2_OK;
}

#define cycles_to_sec_f(cycles)		    \
	(((double)cycles_to_nanosecs(cycles)) / 1000000000.0)

struct ips_stray_epid {
	psm2_epid_t epid;
	uint32_t err_check_bad_sent;
	uint32_t ipv4_addr;
	uint32_t pid;
	uint32_t num_messages;
	uint64_t t_warn_next;
	uint64_t t_first;
	uint64_t t_last;
};

static
void ips_report_strays(struct ips_proto *proto)
{
	struct ips_stray_epid *sepid;
	struct psmi_eptab_iterator itor;
	psmi_epid_itor_init(&itor, PSMI_EP_CROSSTALK);

#if _HFI_DEBUGGING
	double t_first = 0;
	double t_last = 0;
	double t_runtime = 0;
	if (_HFI_INFO_ON) {
		t_runtime = cycles_to_sec_f(proto->t_fini - proto->t_init);
	}
#endif

	while ((sepid = psmi_epid_itor_next(&itor))) {
		char ipbuf[INET_ADDRSTRLEN], *ip = NULL;
		char bufpid[32];
		uint32_t lid = psm2_epid_nid(sepid->epid);
#if _HFI_DEBUGGING
		if (_HFI_INFO_ON) {
			t_first =
				cycles_to_sec_f(sepid->t_first - proto->t_init);
			t_last =
				cycles_to_sec_f(sepid->t_last - proto->t_init);
		}
#endif
		if (sepid->ipv4_addr)
			ip = (char *)
			    inet_ntop(AF_INET, &sepid->ipv4_addr, ipbuf,
				      sizeof(ipbuf));
		if (!ip)
			snprintf(ipbuf, sizeof(ipbuf), "%d (%x)", lid, lid);

		if (sepid->pid)
			snprintf(bufpid, sizeof(bufpid), "PID=%d", sepid->pid);
		else
			snprintf(bufpid, sizeof(bufpid), "PID unknown");

		if (_HFI_INFO_ON) {
			_HFI_INFO_ALWAYS
				("Process %s on host %s=%s sent %d stray message(s) and "
				"was told so %d time(s) (first stray message at %.1fs "
				"(%d%%), last at %.1fs (%d%%) into application run)\n",
				bufpid, ip ? "IP" : "LID", ipbuf, sepid->num_messages,
				sepid->err_check_bad_sent, t_first,
				(int)(t_first * 100.0 / t_runtime), t_last,
				(int)(t_last * 100.0 / t_runtime));
		}

		psmi_epid_remove(PSMI_EP_CROSSTALK, sepid->epid);
		psmi_free(sepid);
	}
	psmi_epid_itor_fini(&itor);
	return;
}

/* New scbs now available.  If we have pending sends because we were out of
 * scbs, put the pendq on the timerq so it can be processed. */
void ips_proto_rv_scbavail_callback(struct ips_scbctrl *scbc, void *context)
{
	struct ips_proto *proto = (struct ips_proto *)context;
	struct ips_pend_sreq *sreq = STAILQ_FIRST(&proto->pend_sends.pendq);
	if (sreq != NULL)
		psmi_timer_request(proto->timerq,
				   &proto->pend_sends.timer, PSMI_TIMER_PRIO_1);
	return;
}

psm2_error_t
ips_proto_timer_pendq_callback(struct psmi_timer *timer, uint64_t current)
{
	psm2_error_t err = PSM2_OK;
	struct ips_pend_sends *pend_sends =
	    (struct ips_pend_sends *)timer->context;
	struct ips_pendsendq *phead = &pend_sends->pendq;
	struct ips_proto *proto = (struct ips_proto *)pend_sends->proto;
	struct ips_pend_sreq *sreq;

	while (!STAILQ_EMPTY(phead)) {
		sreq = STAILQ_FIRST(phead);
		switch (sreq->type) {
		case IPS_PENDSEND_EAGER_REQ:
			err = ips_proto_mq_push_cts_req(proto, sreq->req);
			break;
		case IPS_PENDSEND_EAGER_DATA:
			err = ips_proto_mq_push_rts_data(proto, sreq->req);
			break;

		default:
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "Unknown pendq state %d\n",
					  sreq->type);
		}

		if (err == PSM2_OK) {
			STAILQ_REMOVE_HEAD(phead, next);
			psmi_mpool_put(sreq);
		} else {	/* out of scbs. wait for the next scb_avail callback */
			/* printf("!!!!! breaking out of pendq progress\n"); */
			break;
		}
	}

	return err;
}

PSMI_INLINE(
int
between(int first_seq, int last_seq, int seq))
{
	if (last_seq >= first_seq) {
		if (seq < first_seq || seq > last_seq) {
			return 0;
		}
	} else {
		if (seq > last_seq && seq < first_seq) {
			return 0;
		}
	}
	return 1;
}

PSMI_INLINE(
int
pio_dma_ack_valid(struct ips_proto *proto, struct ips_flow *flow,
		  psmi_seqnum_t ack_seq_num))
{
	uint32_t last_num;
	struct ips_scb_unackedq *unackedq = &flow->scb_unacked;

	if (STAILQ_EMPTY(unackedq))
		return 0;

	/* scb_pend will be moved back when an nak is received, but
	 * the packet may actually be received and acked after the nak,
	 * so we use the tail of unacked queue, which may include packets
	 * not being sent out yet, this is over do, but it should be OK. */
	last_num = STAILQ_LAST(unackedq, ips_scb, nextq)->seq_num.psn_num;

	return between(flow->xmit_ack_num.psn_num,
				last_num, ack_seq_num.psn_num);
}



/* NAK post process for dma flow */
void ips_dmaflow_nak_post_process(struct ips_proto *proto,
				  struct ips_flow *flow)
{
	ips_scb_t *scb;
	uint32_t first_num, ack_num;
	uint16_t padding = 0;

	scb = STAILQ_FIRST(&flow->scb_unacked);
	first_num = __be32_to_cpu(scb->ips_lrh.bth[2]) & proto->psn_mask;
	ack_num = (flow->xmit_ack_num.psn_num - 1) & proto->psn_mask;


	/* If the ack PSN falls into a multi-packets scb,
	 * don't re-send the packets already acked. */
	psmi_assert(scb->nfrag > 1);
	if (between(first_num, scb->seq_num.psn_num, ack_num)) {
		uint32_t npkt, pktlen, nbytes;

		/* how many packets acked in this scb */
		npkt = ((ack_num - first_num) & proto->psn_mask) + 1;

		/* how many bytes already acked in this scb, for eager receive
		 * packets, all payload size is frag_size except the last packet
		 * which is not acked yet */
		pktlen = scb->frag_size;
		nbytes = (((ack_num - first_num) &
			proto->psn_mask) + 1) * pktlen;

		/* 0. update scb info */
		psmi_assert(scb->nfrag_remaining > npkt);
		scb->nfrag_remaining -= npkt;
		psmi_assert(scb->chunk_size_remaining > nbytes);
		scb->chunk_size_remaining -= nbytes;
		ips_scb_buffer(scb) = (void *)((char *)ips_scb_buffer(scb) + nbytes);

		/* 1. if last packet in sequence, set IPS_SEND_FLAG_ACKREQ */
		if (scb->chunk_size_remaining <= scb->frag_size) {
			psmi_assert(scb->nfrag_remaining == 1);
			scb->scb_flags |= IPS_SEND_FLAG_ACKREQ;

			/* last packet is what remaining */
			/* check if padding is required*/
			padding = scb->chunk_size_remaining & 0x3;
			if_pf(padding) {
				/* how much to pad with also equals how many bytes we need
				 * to rewind the source buffer offset by to keep it dw aligned */
				padding = 4 - padding;
				ips_scb_buffer(scb) = (void *)((char*)ips_scb_buffer(scb) - padding);
				scb->chunk_size_remaining += padding;
			}
			pktlen = scb->chunk_size_remaining;
		}

		/* 2. set new packet sequence number */
		scb->ips_lrh.bth[2] = __cpu_to_be32(
			((ack_num + 1) & proto->psn_mask) |
			(scb->scb_flags & IPS_SEND_FLAG_ACKREQ));

		/* 3. set new packet offset adjusted with padding */
		scb->ips_lrh.hdr_data.u32w0 += nbytes - padding;

		/* 4. if packet length is changed, set new length */
		if (scb->payload_size != pktlen) {
			scb->payload_size = pktlen;
			scb->ips_lrh.lrh[2] = __cpu_to_be16((
				(scb->payload_size +
				sizeof(struct ips_message_header) +
				HFI_CRC_SIZE_IN_BYTES) >>
				BYTE2DWORD_SHIFT) & HFI_LRH_PKTLEN_MASK);
		}
	}
}

/* process an incoming ack message.  Separate function to allow */
/* for better optimization by compiler */
int
ips_proto_process_ack(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_proto *proto = rcv_ev->proto;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	struct ips_flow *flow = NULL;
	struct ips_scb_unackedq *unackedq;
	struct ips_scb_pendlist *scb_pend;
	psmi_seqnum_t ack_seq_num, last_seq_num;
	ips_epaddr_flow_t flowid;
	ips_scb_t *scb;
	uint32_t tidctrl;

	ack_seq_num.psn_num = p_hdr->ack_seq_num;
	tidctrl = GET_HFI_KHDR_TIDCTRL(__le32_to_cpu(p_hdr->khdr.kdeth0));
	if (!tidctrl && ((flowid = ips_proto_flowid(p_hdr)) < EP_FLOW_TIDFLOW)) {
		ack_seq_num.psn_num =
		    (ack_seq_num.psn_num - 1) & proto->psn_mask;
		psmi_assert(flowid < EP_FLOW_LAST);
		flow = &ipsaddr->flows[flowid];
		if (!pio_dma_ack_valid(proto, flow, ack_seq_num))
			goto ret;
	} else {
		// we don't use tidflow on UD nor UDP, shouldn't get ACKs about it
		_HFI_ERROR("Got ack for TID flow, not allowed for UD\n");
			goto ret;
	}
	flow->xmit_ack_num.psn_num = p_hdr->ack_seq_num;

	unackedq = &flow->scb_unacked;
	scb_pend = &flow->scb_pend;

	if (STAILQ_EMPTY(unackedq))
		goto ret;

	last_seq_num = STAILQ_LAST(unackedq, ips_scb, nextq)->seq_num;

	INC_TIME_SPEND(TIME_SPEND_USER2);

	/* For tidflow, psn_gen matches. So for all flows, tid/pio/dma,
	 * we can used general psn_num to compare the PSN. */
	while (between((scb = STAILQ_FIRST(unackedq))->seq_num.psn_num,
		       last_seq_num.psn_num, ack_seq_num.psn_num)
	    ) {

		/* take it out of the xmit queue and ..  */
		if (scb == SLIST_FIRST(scb_pend)) {
#ifdef PSM_DEBUG
			flow->scb_num_pending--;
#endif
			SLIST_REMOVE_HEAD(scb_pend, next);
		}

		STAILQ_REMOVE_HEAD(unackedq, nextq);
#ifdef PSM_DEBUG
		flow->scb_num_unacked--;
		psmi_assert(flow->scb_num_unacked >= flow->scb_num_pending);
#endif
		flow->credits += scb->nfrag;


		if (scb->callback)
			(*scb->callback) (scb->cb_param, scb->nfrag > 1 ?
					  scb->chunk_size : scb->payload_size);

		if (!(scb->scb_flags & IPS_SEND_FLAG_PERSISTENT))
			ips_scbctrl_free(scb);

		/* set all index pointer to NULL if all frames have been
		 * acked */
		if (STAILQ_EMPTY(unackedq)) {
			psmi_timer_cancel(proto->timerq, flow->timer_ack);
			flow->timer_ack = NULL;
			psmi_timer_cancel(proto->timerq, flow->timer_send);
			flow->timer_send = NULL;

			SLIST_FIRST(scb_pend) = NULL;
			psmi_assert(flow->scb_num_pending == 0);
			/* Reset congestion window - all packets ACK'd */
			flow->credits = flow->cwin = proto->flow_credits;
			flow->ack_interval = max((flow->credits >> 2) - 1, 1);
			goto ret;
		} else if (flow->timer_ack == scb->timer_ack) {
			/*
			 * Exchange timers with last scb on unackedq.
			 * timer in scb is used by flow, cancelling current
			 * timer and then requesting a new timer takes more
			 * time, instead, we exchange the timer between current
			 * freeing scb and the last scb on unacked queue.
			 */
			psmi_timer *timer;
			ips_scb_t *last = STAILQ_LAST(unackedq, ips_scb, nextq);

			timer = scb->timer_ack;
			scb->timer_ack = last->timer_ack;
			last->timer_ack = timer;
			timer = scb->timer_send;
			scb->timer_send = last->timer_send;
			last->timer_send = timer;

			scb->timer_ack->context = scb;
			scb->timer_send->context = scb;
			last->timer_ack->context = last;
			last->timer_send->context = last;
		}
	}

	psmi_assert(!STAILQ_EMPTY(unackedq));	/* sanity for above loop */

	{
		/* Increase congestion window if flow is not congested */
		if_pf(flow->cwin < proto->flow_credits) {
			flow->credits +=
			    min(flow->cwin << 1,
				proto->flow_credits) - flow->cwin;
			flow->cwin = min(flow->cwin << 1, proto->flow_credits);
			flow->ack_interval = max((flow->credits >> 2) - 1, 1);
		}
	}

	/* Reclaimed some credits - attempt to flush flow */
	if (!SLIST_EMPTY(scb_pend))
		flow->flush(flow, NULL);

	/*
	 * If the next packet has not even been put on the wire, cancel the
	 * retransmission timer since we're still presumably waiting on free
	 * pio bufs
	 */
	if (STAILQ_FIRST(unackedq)->abs_timeout == TIMEOUT_INFINITE)
		psmi_timer_cancel(proto->timerq, flow->timer_ack);

ret:
	return IPS_RECVHDRQ_CONTINUE;
}

/* process an incoming nack message.  Separate function to allow */
/* for better optimization by compiler */
int ips_proto_process_nak(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_proto *proto = rcv_ev->proto;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	struct ips_flow *flow = NULL;
	struct ips_scb_unackedq *unackedq;
	struct ips_scb_pendlist *scb_pend;
	psmi_seqnum_t ack_seq_num, last_seq_num;
	psm_protocol_type_t protocol;
	ips_epaddr_flow_t flowid;
	ips_scb_t *scb;
	uint32_t tidctrl;

	INC_TIME_SPEND(TIME_SPEND_USER3);

	ack_seq_num.psn_num = p_hdr->ack_seq_num;
	tidctrl = GET_HFI_KHDR_TIDCTRL(__le32_to_cpu(p_hdr->khdr.kdeth0));
	if (!tidctrl && ((flowid = ips_proto_flowid(p_hdr)) < EP_FLOW_TIDFLOW)) {
		protocol = PSM_PROTOCOL_GO_BACK_N;
		psmi_assert(flowid < EP_FLOW_LAST);
		flow = &ipsaddr->flows[flowid];
		if (!pio_dma_ack_valid(proto, flow, ack_seq_num))
			goto ret;
		ack_seq_num.psn_num =
		    (ack_seq_num.psn_num - 1) & proto->psn_mask;
		flow->xmit_ack_num.psn_num = p_hdr->ack_seq_num;
	} else {
		// we don't use tidflow on UD nor UDP, shouldn't get NAKs about it
		_HFI_ERROR("Got nak for TID flow, not allowed for UD\n");
			goto ret;	/* Invalid ack for flow */
		ack_seq_num.psn_seq--;

		psmi_assert(flow->xmit_seq_num.psn_gen == ack_seq_num.psn_gen);
		psmi_assert(flow->xmit_ack_num.psn_gen == ack_seq_num.psn_gen);
		/* Update xmit_ack_num with both new generation and new
		 * acked sequence; update xmit_seq_num with the new flow
		 * generation, don't change the sequence number. */
		flow->xmit_ack_num = (psmi_seqnum_t) p_hdr->data[1].u32w0;
		flow->xmit_seq_num.psn_gen = flow->xmit_ack_num.psn_gen;
		psmi_assert(flow->xmit_seq_num.psn_gen != ack_seq_num.psn_gen);
	}

	unackedq = &flow->scb_unacked;
	scb_pend = &flow->scb_pend;

	if (STAILQ_EMPTY(unackedq))
		goto ret;

	last_seq_num = STAILQ_LAST(unackedq, ips_scb, nextq)->seq_num;

	proto->epaddr_stats.nak_recv++;

	_HFI_VDBG("got a nack %d on flow %d, "
		  "first is %d, last is %d\n", ack_seq_num.psn_num,
		  flow->flowid,
		  STAILQ_EMPTY(unackedq) ? -1 : STAILQ_FIRST(unackedq)->seq_num.
		  psn_num, STAILQ_EMPTY(unackedq) ? -1 : STAILQ_LAST(unackedq,
								     ips_scb,
								     nextq)->
		  seq_num.psn_num);

	/* For tidflow, psn_gen matches. So for all flows, tid/pio/dma,
	 * we can use general psn_num to compare the PSN. */
	while (between((scb = STAILQ_FIRST(unackedq))->seq_num.psn_num,
		       last_seq_num.psn_num, ack_seq_num.psn_num)
	    ) {
		/* take it out of the xmit queue and ..  */
		if (scb == SLIST_FIRST(scb_pend)) {
#ifdef PSM_DEBUG
			flow->scb_num_pending--;
#endif
			SLIST_REMOVE_HEAD(scb_pend, next);
		}

		STAILQ_REMOVE_HEAD(unackedq, nextq);
#ifdef PSM_DEBUG
		flow->scb_num_unacked--;
		psmi_assert(flow->scb_num_unacked >= flow->scb_num_pending);
#endif


		if (scb->callback)
			(*scb->callback) (scb->cb_param, scb->nfrag > 1 ?
					  scb->chunk_size : scb->payload_size);

		if (!(scb->scb_flags & IPS_SEND_FLAG_PERSISTENT))
			ips_scbctrl_free(scb);

		/* set all index pointer to NULL if all frames has been acked */
		if (STAILQ_EMPTY(unackedq)) {
			psmi_timer_cancel(proto->timerq, flow->timer_ack);
			flow->timer_ack = NULL;
			psmi_timer_cancel(proto->timerq, flow->timer_send);
			flow->timer_send = NULL;

			SLIST_FIRST(scb_pend) = NULL;
			psmi_assert(flow->scb_num_pending == 0);
			/* Reset congestion window if all packets acknowledged */
			flow->credits = flow->cwin = proto->flow_credits;
			flow->ack_interval = max((flow->credits >> 2) - 1, 1);
			goto ret;
		} else if (flow->timer_ack == scb->timer_ack) {
			/*
			 * Exchange timers with last scb on unackedq.
			 * timer in scb is used by flow, cancelling current
			 * timer and then requesting a new timer takes more
			 * time, instead, we exchange the timer between current
			 * freeing scb and the last scb on unacked queue.
			 */
			psmi_timer *timer;
			ips_scb_t *last = STAILQ_LAST(unackedq, ips_scb, nextq);

			timer = scb->timer_ack;
			scb->timer_ack = last->timer_ack;
			last->timer_ack = timer;
			timer = scb->timer_send;
			scb->timer_send = last->timer_send;
			last->timer_send = timer;

			scb->timer_ack->context = scb;
			scb->timer_send->context = scb;
			last->timer_ack->context = last;
			last->timer_send->context = last;
		}
	}

	psmi_assert(!STAILQ_EMPTY(unackedq));	/* sanity for above loop */

	if (protocol == PSM_PROTOCOL_TIDFLOW)
		_HFI_ERROR("post processing, Got nak for TID flow, not allowed for UD\n");
	else if (scb->nfrag > 1)
		ips_dmaflow_nak_post_process(proto, flow);

	/* Always cancel ACK timer as we are going to restart the flow */
	psmi_timer_cancel(proto->timerq, flow->timer_ack);

	/* What's now pending is all that was unacked */
	SLIST_FIRST(scb_pend) = scb;
#ifdef PSM_DEBUG
	flow->scb_num_pending = flow->scb_num_unacked;
#endif
	while (scb && !(scb->scb_flags & IPS_SEND_FLAG_PENDING)) {

		scb->scb_flags |= IPS_SEND_FLAG_PENDING;
		scb = SLIST_NEXT(scb, next);
	}

	{
		int num_resent = 0;

		/* Reclaim all credits upto congestion window only */
		flow->credits = flow->cwin;
		flow->ack_interval = max((flow->credits >> 2) - 1, 1);

		/* Flush pending scb's */
		flow->flush(flow, &num_resent);

		proto->epaddr_stats.send_rexmit += num_resent;
	}

ret:
	return IPS_RECVHDRQ_CONTINUE;
}

int
ips_proto_process_err_chk(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_recvhdrq *recvq = (struct ips_recvhdrq *)rcv_ev->recvq;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	ips_epaddr_flow_t flowid = ips_proto_flowid(p_hdr);
	struct ips_flow *flow;
	psmi_seqnum_t seq_num;
	int16_t seq_off;

	INC_TIME_SPEND(TIME_SPEND_USER4);
	PSM2_LOG_MSG("entering");
	psmi_assert(flowid < EP_FLOW_LAST);
	flow = &ipsaddr->flows[flowid];
	recvq->proto->epaddr_stats.err_chk_recv++;

	seq_num.psn_val = __be32_to_cpu(p_hdr->bth[2]);
	seq_off = (int16_t) (flow->recv_seq_num.psn_num - seq_num.psn_num);

	if_pf(seq_off <= 0) {
		_HFI_VDBG("naking for seq=%d, off=%d on flowid  %d\n",
			  seq_num.psn_num, seq_off, flowid);

		if (seq_off < -flow->ack_interval)
			flow->flags |= IPS_FLOW_FLAG_GEN_BECN;

		ips_proto_send_nak(recvq, flow);
		flow->flags |= IPS_FLOW_FLAG_NAK_SEND;
	}
	else {
		ips_scb_t ctrlscb;

		ctrlscb.scb_flags = 0;
		ctrlscb.ips_lrh.ack_seq_num = flow->recv_seq_num.psn_num;
		ips_proto_send_ctrl_message(flow, OPCODE_ACK,
					    &ipsaddr->ctrl_msg_queued,
					    &ctrlscb, ctrlscb.cksum, 0);
	}

	PSM2_LOG_MSG("leaving");
	return IPS_RECVHDRQ_CONTINUE;
}



static void ips_bad_opcode(uint8_t op_code, struct ips_message_header *proto)
{
	_HFI_DBG("Discarding message with bad opcode 0x%x\n", op_code);

	if (hfi_debug & __HFI_DBG) {
		ips_proto_show_header(proto, "received bad opcode");
		ips_proto_dump_frame(proto, sizeof(struct ips_message_header),
				     "Opcode error protocol header dump");
	}
}

int
ips_proto_process_unknown_opcode(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_message_header *protocol_header = rcv_ev->p_hdr;
	struct ips_proto *proto = rcv_ev->proto;

	proto->stats.unknown_packets++;
	ips_bad_opcode(_get_proto_hfi_opcode(protocol_header), protocol_header);

	return IPS_RECVHDRQ_CONTINUE;
}

int
ips_proto_connect_disconnect(struct ips_recvhdrq_event *rcv_ev)
{
	psm2_error_t err = PSM2_OK;
	char *payload = ips_recvhdrq_event_payload(rcv_ev);
	uint32_t paylen = ips_recvhdrq_event_paylen(rcv_ev);

	psmi_assert(payload);
	err = ips_proto_process_connect(rcv_ev->proto,
					_get_proto_hfi_opcode(rcv_ev->p_hdr),
					rcv_ev->p_hdr,
					payload,
					paylen);
	if (err != PSM2_OK)
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Process connect/disconnect error: %d, opcode %d\n",
			err, _get_proto_hfi_opcode(rcv_ev->p_hdr));

	return IPS_RECVHDRQ_CONTINUE;
}

/* Return 1 if packet is ok. */
/* Return 0 if packet should be skipped */
int ips_proto_process_unknown(const struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	struct ips_proto *proto = rcv_ev->proto;
	int opcode = (int)_get_proto_hfi_opcode(p_hdr);

	/*
	 * If the protocol is disabled or not yet enabled, no processing happens
	 * We set it t_init to 0 when disabling the protocol
	 */
	if (proto->t_init == 0)
		return IPS_RECVHDRQ_CONTINUE;

	/* Connect messages don't have to be from a known epaddr */
	switch (opcode) {
	case OPCODE_CONNECT_REQUEST:
	case OPCODE_CONNECT_REPLY:
	case OPCODE_DISCONNECT_REQUEST:
	case OPCODE_DISCONNECT_REPLY:
		ips_proto_connect_disconnect(
				(struct ips_recvhdrq_event *)rcv_ev);
		return IPS_RECVHDRQ_CONTINUE;
	default:
		break;
	}

	/* Packet from "unknown" peer. Log the packet and payload if at appropriate
	 * verbose level.
	 */
	{
		char *payload = ips_recvhdrq_event_payload(rcv_ev);
		uint32_t paylen = ips_recvhdrq_event_paylen(rcv_ev) +
		    ((__be32_to_cpu(rcv_ev->p_hdr->bth[0]) >> 20) & 3);

		if (hfi_debug & __HFI_PKTDBG) {
			ips_proto_dump_frame(rcv_ev->p_hdr,
					     HFI_MESSAGE_HDR_SIZE, "header");
			if (paylen)
				ips_proto_dump_frame(payload, paylen, "data");
		}
	}


	proto->stats.stray_packets++;

	/* If we have debug mode, print the complete packet every time */
	if (hfi_debug & __HFI_PKTDBG)
		ips_proto_show_header(p_hdr, "invalid connidx");

	psmi_handle_error(PSMI_EP_LOGEVENT, PSM2_EPID_NETWORK_ERROR,
			 "Received message(s) opcode=0x%x from an unknown process", opcode);

	return 0;		/* Always skip this packet unless the above call was a noreturn
				 * call */
}


