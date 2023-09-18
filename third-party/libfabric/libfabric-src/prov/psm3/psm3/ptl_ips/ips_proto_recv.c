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

static void ips_report_strays(struct ips_proto *proto);

psm2_error_t psm3_ips_proto_recv_init(struct ips_proto *proto)
{
	uint32_t interval_secs;
	union psmi_envvar_val env_stray;

	psm3_getenv("PSM3_STRAY_WARNINTERVAL",
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

psm2_error_t psm3_ips_proto_recv_fini(struct ips_proto *proto)
{
	ips_report_strays(proto);
	return PSM2_OK;
}

#define cycles_to_sec_f(cycles)		    \
	(((double)cycles_to_nanosecs(cycles)) / 1000000000.0)

struct ips_stray_epid {
	psm2_epid_t epid;
	uint32_t err_check_bad_sent;
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
	psm3_epid_itor_init(&itor, PSMI_EP_CROSSTALK);

#if _HFI_DEBUGGING
	double t_first = 0;
	double t_last = 0;
	double t_runtime = 0;
	if (_HFI_INFO_ON) {
		t_runtime = cycles_to_sec_f(proto->t_fini - proto->t_init);
	}
#endif

	while ((sepid = psm3_epid_itor_next(&itor))) {
		char bufpid[32];
#if _HFI_DEBUGGING
		if (_HFI_INFO_ON) {
			t_first =
				cycles_to_sec_f(sepid->t_first - proto->t_init);
			t_last =
				cycles_to_sec_f(sepid->t_last - proto->t_init);
		}
#endif
		if (sepid->pid)
			snprintf(bufpid, sizeof(bufpid), "PID=%d", sepid->pid);
		else
			snprintf(bufpid, sizeof(bufpid), "PID unknown");

		if (_HFI_INFO_ON) {
			_HFI_INFO_ALWAYS
				("Process %s on host %s sent %d stray message(s) and "
				"was told so %d time(s) (first stray message at %.1fs "
				"(%d%%), last at %.1fs (%d%%) into application run)\n",
				bufpid, psm3_epaddr_get_name(sepid->epid, 0),
				sepid->num_messages,
				sepid->err_check_bad_sent, t_first,
				(int)(t_first * 100.0 / t_runtime), t_last,
				(int)(t_last * 100.0 / t_runtime));
		}

		psm3_epid_remove(PSMI_EP_CROSSTALK, sepid->epid);
		psmi_free(sepid);
	}
	psm3_epid_itor_fini(&itor);
	return;
}

/* New scbs now available.  If we have pending sends because we were out of
 * scbs, put the pendq on the timerq so it can be processed. */
void psm3_ips_proto_rv_scbavail_callback(struct ips_scbctrl *scbc, void *context)
{
	struct ips_proto *proto = (struct ips_proto *)context;
	struct ips_pend_sreq *sreq = STAILQ_FIRST(&proto->pend_sends.pendq);
	if (sreq != NULL)
		psmi_timer_request(proto->timerq,
				   &proto->pend_sends.timer, PSMI_TIMER_PRIO_1);
	return;
}

psm2_error_t
psm3_ips_proto_timer_pendq_callback(struct psmi_timer *timer, uint64_t current)
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
			err = psm3_ips_proto_mq_push_cts_req(proto, sreq->req);
			break;
		case IPS_PENDSEND_EAGER_DATA:
			err = psm3_ips_proto_mq_push_rts_data(proto, sreq->req);
			break;

		default:
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "Unknown pendq state %d\n",
					  sreq->type);
		}

		if (err == PSM2_OK) {
			STAILQ_REMOVE_HEAD(phead, next);
			psm3_mpool_put(sreq);
		} else {	/* out of scbs. wait for the next scb_avail callback */
			/* printf("!!!!! breaking out of pendq progress\n"); */
			break;
		}
	}

	return err;
}

// return 1 if this ack_seq_num is potentially for a packet on unackedq
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



/* NAK post process for any flow where an scb may describe more than 1 packet
 * (OPA dma flow or GSO PIO flow). In which case we may need to resume in
 * middle of scb.
 */
void psm3_ips_segmentation_nak_post_process(struct ips_proto *proto,
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
			scb->ips_lrh.lrh[2] = ips_proto_bytes_to_lrh2_be(proto,
					scb->payload_size +
					sizeof(struct ips_message_header) +
					HFI_CRC_SIZE_IN_BYTES);
		}
	}
}

/* process an incoming ack message.  Separate function to allow */
/* for better optimization by compiler */
int
psm3_ips_proto_process_ack(struct ips_recvhdrq_event *rcv_ev)
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

	ack_seq_num.psn_num = p_hdr->ack_seq_num;
	// check actual psn acked (ack_seq_num-1), we only want to process acks
	// for packets we never got an ack for
	if ((flowid = ips_proto_flowid(p_hdr)) < EP_FLOW_TIDFLOW) {
		ack_seq_num.psn_num =
		    (ack_seq_num.psn_num - 1) & proto->psn_mask;
		psmi_assert(flowid < EP_FLOW_LAST);
		flow = &ipsaddr->flows[flowid];
		if (!pio_dma_ack_valid(proto, flow, ack_seq_num))
			goto ret;
	} else {
		// we don't put TID (aka RDMA) pkts on UD, shouldn't get ACKs about it
		_HFI_ERROR("Got ack for invalid flowid\n");
		goto ret;
	}
#ifndef PSM_TCP_ACK
	// for ack-less TCP we should have acked self-packet before recv reports
	// the given ack_seq_num
	psmi_assert(psm3_epid_protocol(proto->ep->epid) != PSMI_ETH_PROTO_TCP);
#endif
	flow->xmit_ack_num.psn_num = p_hdr->ack_seq_num;

	unackedq = &flow->scb_unacked;
	scb_pend = &flow->scb_pend;

	if (STAILQ_EMPTY(unackedq))
		goto ret;

	last_seq_num = STAILQ_LAST(unackedq, ips_scb, nextq)->seq_num;
	_HFI_VDBG("ack_seq_num=%d last_seq_num=%d \n", ack_seq_num.psn_num, last_seq_num.psn_num);

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
		// this assumes the ACK will always be for a complete scb
		flow->credits += scb->nfrag;
#ifdef PSM_BYTE_FLOW_CREDITS
		flow->credit_bytes += scb->chunk_size;
		_HFI_VDBG("after ACK psn=%d: flow_credits %d bytes %d\n",
				scb->seq_num.psn_num, flow->credits, flow->credit_bytes);
#else
		_HFI_VDBG("after ACK: flow_credits %d\n", flow->credits);
#endif

#ifdef PSM_HAVE_SDMA
		if (scb->sdma_outstanding) {
			// we got an ack for a DMA we did not yet complete
			// maybe a late arrival of original we were asked to
			// retry.  We choose to wait here so we can properly
			// mark this scb and perhaps scbs after it as acked.
			// If we don't wait and mark it now we would end up
			// timing out on lack of ack for this scb later.
			proto->stats.sdma_compl_wait_ack++;
			ips_proto_dma_wait_until(proto, scb);
		}
#endif /* PSM_HAVE_SDMA */
#ifdef PSM_ONEAPI
		if (scb->scb_flags & IPS_SEND_FLAG_USE_GDRCOPY) {
			psmi_hal_gdr_munmap_gpu_to_host_addr(
					scb->gdr_addr, scb->gdr_size,
					0, proto->ep);
			scb->scb_flags &= ~IPS_SEND_FLAG_USE_GDRCOPY;
		}
#endif
		if (scb->callback)
			(*scb->callback) (scb->cb_param, scb->nfrag > 1 ?
					  scb->chunk_size : scb->payload_size);

		if (!(scb->scb_flags & IPS_SEND_FLAG_PERSISTENT))
			psm3_ips_scbctrl_free(scb);

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
#ifdef PSM_BYTE_FLOW_CREDITS
			flow->credit_bytes = proto->flow_credit_bytes;
			flow->ack_interval_bytes = max((flow->credit_bytes >> 2) - 1, 1);
			_HFI_VDBG("after all ACKed: flow_credits %d bytes %d\n",
				flow->credits, flow->credit_bytes);
#else
			_HFI_VDBG("after all ACKed: flow_credits %d\n",
				flow->credits);
#endif
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
			// this only happens for OPA, so we don't have to
			// increase ack_interval_bytes and flow_credit_bytes
			// since we never decrease them for congestion
			flow->credits +=
			    min(flow->cwin << 1,
				proto->flow_credits) - flow->cwin;
			flow->cwin = min(flow->cwin << 1, proto->flow_credits);
			flow->ack_interval = max((flow->credits >> 2) - 1, 1);
#ifdef PSM_BYTE_FLOW_CREDITS
			//flow->credit_bytes += TBD
			//flow->ack_interval_bytes = max((flow->credit_bytes >> 2) - 1, 1);
			_HFI_VDBG("after grow cwin: flow_credits %d bytes %d\n",
				flow->credits, flow->credit_bytes);
#else
			_HFI_VDBG("after grow cwin: flow_credits %d\n",
				flow->credits);
#endif
		}
	}

	/* Reclaimed some credits - attempt to flush flow */
	if (!SLIST_EMPTY(scb_pend))
		flow->flush(flow, NULL);
	/*
	 * If the next packet has not even been put on the wire, cancel the
	 * retransmission timer since we're still presumably waiting on free
	 * pio bufs.  If flow->flush has drained the scb_pend queue and TCP HAL
	 * has self-acked all those packets, we may get here with unackedq empty.
	 */
	if ((scb = STAILQ_FIRST(unackedq)) && scb->abs_timeout == TIMEOUT_INFINITE)
		psmi_timer_cancel(proto->timerq, flow->timer_ack);

ret:
	return IPS_RECVHDRQ_CONTINUE;
}

/* process an incoming nack message.  Separate function to allow */
/* for better optimization by compiler */
int psm3_ips_proto_process_nak(struct ips_recvhdrq_event *rcv_ev)
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

	INC_TIME_SPEND(TIME_SPEND_USER3);

	ack_seq_num.psn_num = p_hdr->ack_seq_num;
	// we are likely to get a previous ack_seq_num in NAK, in which case
	// we need to resend unacked packets starting with ack_seq_num.  So check
	// psn of 1st NAK would like us to retransmit (e.g. don't -1 before check)
	if ((flowid = ips_proto_flowid(p_hdr)) < EP_FLOW_TIDFLOW) {
		protocol = PSM_PROTOCOL_GO_BACK_N;
		psmi_assert(flowid < EP_FLOW_LAST);
		flow = &ipsaddr->flows[flowid];
		if (!pio_dma_ack_valid(proto, flow, ack_seq_num))
			goto ret;
		ack_seq_num.psn_num =
		    (ack_seq_num.psn_num - 1) & proto->psn_mask;
		flow->xmit_ack_num.psn_num = p_hdr->ack_seq_num;
	} else {
		// we don't put TID (aka RDMA) pkts on UD, shouldn't get NAKs about it
		_HFI_ERROR("Got nak for invalid flowid\n");
		goto ret;
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

#ifdef PSM_HAVE_SDMA
		if (scb->sdma_outstanding) {
			// we got an ack (via nak) for a DMA we did not yet
			// complete
			// maybe a late arrival of original we were asked to
			// retry.  We choose to wait here so we can properly
			// mark this scb and perhaps scbs after it as acked.
			// If we don't wait and mark it now we would end up
			// timing out on lack of ack for this scb later.
			proto->stats.sdma_compl_wait_ack++;
			ips_proto_dma_wait_until(proto, scb);
		}
#endif /* PSM_HAVE_SDMA */

#ifdef PSM_ONEAPI
		if (scb->scb_flags & IPS_SEND_FLAG_USE_GDRCOPY) {
			psmi_hal_gdr_munmap_gpu_to_host_addr(
					scb->gdr_addr, scb->gdr_size,
					0, proto->ep);
			scb->scb_flags &= ~IPS_SEND_FLAG_USE_GDRCOPY;
		}
#endif
		if (scb->callback)
			(*scb->callback) (scb->cb_param, scb->nfrag > 1 ?
					  scb->chunk_size : scb->payload_size);

		if (!(scb->scb_flags & IPS_SEND_FLAG_PERSISTENT))
			psm3_ips_scbctrl_free(scb);

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
#ifdef PSM_BYTE_FLOW_CREDITS
			flow->credit_bytes = proto->flow_credit_bytes;
			flow->ack_interval_bytes = max((flow->credit_bytes >> 2) - 1, 1);
			_HFI_VDBG("after all NAKed: flow_credits %d bytes %d\n",
				flow->credits, flow->credit_bytes);
#else
			_HFI_VDBG("after all NAKed: flow_credits %d\n",
				flow->credits);
#endif
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
		// we don't put TID (aka RDMA) pkts on UD, shouldn't get NAKs about it
		_HFI_ERROR("post processing, Got nak for TID flow, not allowed for UD\n");
	else if (scb->nfrag > 1)
		psm3_ips_segmentation_nak_post_process(proto, flow);

	/* Always cancel ACK timer as we are going to restart the flow */
	psmi_timer_cancel(proto->timerq, flow->timer_ack);

	/* What's now pending is all that was unacked */
	SLIST_FIRST(scb_pend) = scb;
#ifdef PSM_DEBUG
	flow->scb_num_pending = flow->scb_num_unacked;
#endif
	while (scb && !(scb->scb_flags & IPS_SEND_FLAG_PENDING)) {

#ifdef PSM_HAVE_SDMA
		if (scb->sdma_outstanding) {
			// The NAK implies the receiver got something
			// out of order and we need to retransmit everything
			// which follows.  If we have incomplete DMAs queued
			// to HW queue, they will unfortunately be discarded
			// as out of order at the receiver, but we must wait
			// for them to complete so we can queue their
			// retransmission.  It's also possible they have been
			// sent but are waiting for lazy compl reaping
			proto->stats.sdma_compl_wait_resend++;
			ips_proto_dma_wait_until(proto, scb);
		}
#endif /* PSM_HAVE_SDMA */
		scb->scb_flags |= IPS_SEND_FLAG_PENDING;
		scb = SLIST_NEXT(scb, next);
	}

	{
		int num_resent = 0;

		/* Reclaim all credits upto congestion window only */
		flow->credits = flow->cwin;
		flow->ack_interval = max((flow->credits >> 2) - 1, 1);
#ifdef PSM_BYTE_FLOW_CREDITS
		// TBD cwin not implemented for UD and UDP so can predict
		// credit_bytes here
		psmi_assert(flow->cwin == proto->flow_credits);
		flow->credit_bytes = proto->flow_credit_bytes;
		flow->ack_interval_bytes = max((flow->credit_bytes >> 2) - 1, 1);
		_HFI_VDBG("after reclaim cwin: flow_credits %d\n",
				flow->credits);
#else /* PSM_BYTE_FLOW_CREDITS */
		_HFI_VDBG("after reclaim cwin: flow_credits %d\n",
				flow->credits);
#endif /* PSM_BYTE_FLOW_CREDITS */

		/* Flush pending scb's */
		flow->flush(flow, &num_resent);

		proto->epaddr_stats.send_rexmit += num_resent;
	}

ret:
	return IPS_RECVHDRQ_CONTINUE;
}

int
psm3_ips_proto_process_err_chk(struct ips_recvhdrq_event *rcv_ev)
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

		// BECN only for OPA (could ifdef)
		// so no need to test ack_interval_bytes
		if (seq_off < -flow->ack_interval)
			flow->flags |= IPS_FLOW_FLAG_GEN_BECN;

		ips_proto_send_nak(recvq, flow);
		flow->flags |= IPS_FLOW_FLAG_NAK_SEND;
	}
	else {
		ips_scb_t ctrlscb;

		ctrlscb.scb_flags = 0;
		ctrlscb.ips_lrh.ack_seq_num = flow->recv_seq_num.psn_num;
		// no payload, pass cksum so non-NULL
		psm3_ips_proto_send_ctrl_message(flow, OPCODE_ACK,
					    &ipsaddr->ctrl_msg_queued,
					    &ctrlscb, ctrlscb.cksum, 0);
	}

	PSM2_LOG_MSG("leaving");
	return IPS_RECVHDRQ_CONTINUE;
}

static void ips_bad_opcode(uint8_t op_code, struct ips_message_header *proto)
{
	_HFI_DBG("Discarding message with bad opcode 0x%x\n", op_code);

	if (psm3_dbgmask & __HFI_DBG) {
		psm3_ips_proto_show_header(proto, "received bad opcode");
		psm3_ips_proto_dump_frame(proto, sizeof(struct ips_message_header),
				     "Opcode error protocol header dump");
	}
}

int
psm3_ips_proto_process_unknown_opcode(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_message_header *protocol_header = rcv_ev->p_hdr;
	struct ips_proto *proto = rcv_ev->proto;

	proto->stats.unknown_packets++;
	ips_bad_opcode(_get_proto_hfi_opcode(protocol_header), protocol_header);

	return IPS_RECVHDRQ_CONTINUE;
}

int
psm3_ips_proto_connect_disconnect(struct ips_recvhdrq_event *rcv_ev)
{
	psm2_error_t err = PSM2_OK;
	char *payload = ips_recvhdrq_event_payload(rcv_ev);
	uint32_t paylen = ips_recvhdrq_event_paylen(rcv_ev);

	psmi_assert(payload);
	err = psm3_ips_proto_process_connect(rcv_ev->proto,
					_get_proto_hfi_opcode(rcv_ev->p_hdr),
					rcv_ev->p_hdr,
					payload,
					paylen);
	if (err != PSM2_OK)
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Process connect/disconnect error: %d, opcode %x\n",
			err, _get_proto_hfi_opcode(rcv_ev->p_hdr));

	return IPS_RECVHDRQ_CONTINUE;
}

/* helper function which should be called by a HAL specific wrapper function */
/* Return 0 if packet is now handled. */
/* Return 1 if packet should be skipped with error */
int psm3_ips_proto_process_unknown(const struct ips_recvhdrq_event *rcv_ev, int *opcode)
{
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	struct ips_proto *proto = rcv_ev->proto;

	*opcode = (int)_get_proto_hfi_opcode(p_hdr);

	/*
	 * If the protocol is disabled or not yet enabled, no processing happens
	 * We set it t_init to 0 when disabling the protocol
	 */
	if (proto->t_init == 0)
		return 0;

	/* Connect messages don't have to be from a known epaddr */
	switch (*opcode) {
	case OPCODE_CONNECT_REQUEST:
	case OPCODE_CONNECT_REPLY:
	case OPCODE_DISCONNECT_REQUEST:
	case OPCODE_DISCONNECT_REPLY:
		psm3_ips_proto_connect_disconnect(
				(struct ips_recvhdrq_event *)rcv_ev);
		return 0;
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

		if (psm3_dbgmask & __HFI_PKTDBG) {
			psm3_ips_proto_dump_frame(rcv_ev->p_hdr,
					     sizeof(struct ips_message_header), "header");
			if (paylen)
				psm3_ips_proto_dump_frame(payload, paylen, "data");
		}
	}

	proto->stats.stray_packets++;

	/* If we have debug mode, print the complete packet every time */
	if (psm3_dbgmask & __HFI_PKTDBG)
		psm3_ips_proto_show_header(p_hdr, "invalid connidx");

	return 1;	/* caller will skip with error output */
}
