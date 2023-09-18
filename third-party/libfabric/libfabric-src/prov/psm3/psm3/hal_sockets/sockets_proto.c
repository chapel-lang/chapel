#ifdef PSM_SOCKETS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2022 Intel Corporation.

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

  Copyright(c) 2022 Intel Corporation.

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

/* Copyright (c) 2003-2022 Intel Corporation. All rights reserved. */

/* This file implements the HAL specific code for PSM proto */
#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "sockets_hal.h"

#include "sockets_spio.c"

static __inline__ void
psm3_tcp_proto_local_ack(struct ips_proto *proto, struct ips_flow *flow)
{
	ips_scb_t *scb;
	struct ips_scb_unackedq *unackedq = &flow->scb_unacked;
	struct ips_scb_pendlist *scb_pend = &flow->scb_pend;

	if (STAILQ_EMPTY(unackedq))
		return;

	psmi_seqnum_t last_seq_num = STAILQ_LAST(unackedq, ips_scb, nextq)->seq_num;
	while (between((scb = STAILQ_FIRST(unackedq))->seq_num.psn_num,
		       last_seq_num.psn_num, flow->xmit_ack_num.psn_num-1)
	    ) {
		STAILQ_REMOVE_HEAD(unackedq, nextq);
#ifdef PSM_DEBUG
		flow->scb_num_unacked--;
		psmi_assert(flow->scb_num_unacked >= flow->scb_num_pending);
#endif
#ifdef PSM_ONEAPI
		if (scb->scb_flags & IPS_SEND_FLAG_USE_GDRCOPY) {
			psm3_sockets_gdr_munmap_gpu_to_host_addr(
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
		if (STAILQ_EMPTY(unackedq)) {
			// timer_ack shall not start
			psmi_assert(! (flow->timer_ack->flags & PSMI_TIMER_FLAG_PENDING));
			flow->timer_ack = NULL;
			psmi_timer_cancel(proto->timerq, flow->timer_send);
			flow->timer_send = NULL;

			SLIST_FIRST(scb_pend) = NULL;
			psmi_assert(flow->scb_num_pending == 0);
			break;
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
}

psm2_error_t
psm3_tcp_proto_flow_flush_pio(struct ips_flow *flow, int *nflushed)
{
	struct ips_proto *proto = ((psm2_epaddr_t) (flow->ipsaddr))->proto;
	struct ips_scb_pendlist *scb_pend = &flow->scb_pend;
	int num_sent = 0, i = 0;
	ips_scb_t *scb = NULL;
	psm2_error_t err = PSM2_OK;

	psmi_assert(!SLIST_EMPTY(scb_pend));

	GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
	err = psm3_sockets_tcp_spio_transfer_frames(proto, flow, scb_pend, &num_sent);
	GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
	if (num_sent) {
		while (!SLIST_EMPTY(scb_pend) && i < num_sent) {
			scb = SLIST_FIRST(scb_pend);
			scb->scb_flags &= ~IPS_SEND_FLAG_PENDING;
			SLIST_REMOVE_HEAD(scb_pend, next);
			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&scb->ips_lrh,"PKT_STRM: payload_size=%d err: %d",
				scb->payload_size, err);
			i += 1;
		}
#ifdef PSM_DEBUG
		flow->scb_num_pending -= num_sent;
#endif
		// local ack
		if (scb) { // this check is unnecessary, but can make KW happy
			flow->xmit_ack_num.psn_num = 1 + (__be32_to_cpu(scb->ips_lrh.bth[2]) & proto->psn_mask);
		}
		psm3_tcp_proto_local_ack(proto, flow);
	}

	if (!SLIST_EMPTY(scb_pend)) {
		psmi_timer_request(proto->timerq, flow->timer_send,
				   get_cycles() + proto->timeout_send);
	}

	if (nflushed != NULL) {
		*nflushed = num_sent;
	}
	return err;
}

#endif
