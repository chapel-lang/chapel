#ifdef PSM_VERBS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2021 Intel Corporation.

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

  Copyright(c) 2021 Intel Corporation.

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

/* Copyright (c) 2003-2021 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_hal.h"

#include "ips_epstate.h"
#include "ips_proto.h"
#include "ips_expected_proto.h"
#include "ips_proto_help.h"
#include "ips_proto_internal.h"
#include "verbs_hal.h"

/*
 * Receive header queue initialization.
 */
psm2_error_t
psm3_verbs_recvhdrq_init(const struct ips_epstate *epstate,
		  const struct ips_proto *proto,
		  const struct ips_recvhdrq_callbacks *callbacks,
		  struct ips_recvhdrq *recvq
		)
{
	psm2_error_t err = PSM2_OK;

	memset(recvq, 0, sizeof(*recvq));
	recvq->proto = (struct ips_proto *)proto;
	pthread_spin_init(&recvq->hdrq_lock, PTHREAD_PROCESS_SHARED);

	recvq->epstate = epstate;
	recvq->recvq_callbacks = *callbacks;	/* deep copy */
	SLIST_INIT(&recvq->pending_acks);

	return err;
}

/* receive service routine for each packet opcode starting at
 * OPCODE_RESERVED (C0)
 */
ips_packet_service_fn_t
psm3_verbs_packet_service_routines[] = {
psm3_ips_proto_process_unknown_opcode,	/* 0xC0 */
psm3_ips_proto_mq_handle_tiny,		/* OPCODE_TINY */
psm3_ips_proto_mq_handle_short,		/* OPCODE_SHORT */
psm3_ips_proto_mq_handle_eager,		/* OPCODE_EAGER */
psm3_ips_proto_mq_handle_rts,		/* OPCODE_LONG_RTS */
psm3_ips_proto_mq_handle_cts,		/* OPCODE_LONG_CTS */
psm3_ips_proto_mq_handle_data,		/* OPCODE_LONG_DATA */
#ifdef RNDV_MOD
ips_protoexp_process_err_chk_rdma,	/* OPCODE_ERR_CHK_RDMA */
ips_protoexp_process_err_chk_rdma_resp,	/* OPCODE_ERR_CHK_RDMA_RESP */
#else
psm3_ips_proto_process_unknown_opcode,	/* C7 */
psm3_ips_proto_process_unknown_opcode,	/* C8 */
#endif

/* these are control packets */
psm3_ips_proto_process_ack,		/* OPCODE_ACK */
psm3_ips_proto_process_nak,		/* OPCODE_NAK */
psm3_ips_proto_process_unknown_opcode,	/* CB */
psm3_ips_proto_process_err_chk,		/* OPCODE_ERR_CHK */
psm3_ips_proto_process_unknown_opcode,	/* CD */
psm3_ips_proto_connect_disconnect,	/* OPCODE_CONNECT_REQUEST */
psm3_ips_proto_connect_disconnect,	/* OPCODE_CONNECT_REPLY */
psm3_ips_proto_connect_disconnect,	/* OPCODE_DISCONNECT__REQUEST */
psm3_ips_proto_connect_disconnect,	/* OPCODE_DISCONNECT_REPLY */

/* rest are not control packets */
psm3_ips_proto_am,			/* OPCODE_AM_REQUEST_NOREPLY */
psm3_ips_proto_am,			/* OPCODE_AM_REQUEST */
psm3_ips_proto_am			/* OPCODE_AM_REPLY */

/* D5-DF (OPCODE_FUTURE_FROM to OPCODE_FUTURE_TO) reserved for expansion */
};

#ifdef RNDV_MOD
// check for and process RV RDMA sends and RDMA recv
static
psm2_error_t psm3_verbs_check_rv_completion(psm2_ep_t ep, struct ips_proto *proto)
{
	struct rv_event ev;
	psm2_error_t ret = PSM2_OK_NO_PROGRESS;

	if (! IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode))
		return ret;

	while (psm3_rv_poll_cq(ep->rv, &ev) > 0) {
		ret = PSM2_OK;
		switch (ev.event_type) {
			case RV_WC_RDMA_WRITE:
				ep->verbs_ep.send_rdma_outstanding--;
				_HFI_MMDBG("got RV RDMA Write SQ CQE status %u outstanding %u\n", ev.wc.status, ep->verbs_ep.send_rdma_outstanding);
				if_pf (ev.wc.status || ev.wc.wr_id == 0) {
					if (PSM2_OK != ips_protoexp_rdma_write_completion_error(
								ep, ev.wc.wr_id, ev.wc.status))
						return PSM2_INTERNAL_ERR;

				} else {
					ips_protoexp_rdma_write_completion( ev.wc.wr_id);
				}
				break;
			case RV_WC_RECV_RDMA_WITH_IMM:
				if_pf (ev.wc.status) {
					if (ep->verbs_ep.rv_reconnect_timeout)
						break;	/* let sender handle errors */
					psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
							"failed rv recv RDMA '%s' (%d) on %s port %u epid %s\n",
							ibv_wc_status_str(ev.wc.status), (int)ev.wc.status, ep->dev_name, ep->portnum, psm3_epid_fmt_internal(ep->epid, 0));
					return PSM2_INTERNAL_ERR;
				}
				_HFI_MMDBG("got RV RDMA Write Immediate RQ CQE %u bytes\n",
							ev.wc.byte_len);
				ips_protoexp_handle_immed_data(proto,
							ev.wc.conn_handle, RDMA_IMMED_RV,
							ev.wc.imm_data, ev.wc.byte_len);
				break;
			default:
				psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"unexpected rv event %d status '%s' (%d) on %s port %u epid %s\n",
					ev.event_type, ibv_wc_status_str(ev.wc.status),
					(int)ev.wc.status, ep->dev_name, ep->portnum, psm3_epid_fmt_internal(ep->epid, 0));
				break;
			}
	}
	return ret;;
}
#endif // RNDV_MOD

psm2_error_t psm3_verbs_recvhdrq_progress(struct ips_recvhdrq *recvq)
{
	GENERIC_PERF_BEGIN(PSM_RX_SPEEDPATH_CTR); /* perf stats */

	int ret = IPS_RECVHDRQ_CONTINUE;
	struct ips_epstate_entry *epstaddr;
	psm2_ep_t ep = recvq->proto->ep;
	PSMI_CACHEALIGN struct ips_recvhdrq_event rcv_ev = {
		.proto = recvq->proto,
		.recvq = recvq,
		//.ptype = RCVHQ_RCV_TYPE_ERROR
	};
	rbuf_t buf;
	uint32_t num_done = 0;
	int err;

#ifdef RNDV_MOD
	// rv completes are for larger RDMAs and should be infrequent, give
	// them 1st chance
	switch (psm3_verbs_check_rv_completion(ep, recvq->proto)) {
	case PSM2_OK:
		num_done=1;	// triggers PSM_OK return below
		break;
	case PSM2_OK_NO_PROGRESS:
		break;
	default:
		goto fail;
		break;
	}
#endif
#if VERBS_RECV_CQE_BATCH > 1
	int done = 0;
	do {
		struct ibv_wc *wc;
// a little atypical, but allows ifdef to be smaller scope
#undef WC
#define WC(field) ((wc)->field)
		if (! ep->verbs_ep.recv_wc_count) {
			// TBD - negative error return is possible but unlikely
			if (0 == (err = ibv_poll_cq(ep->verbs_ep.recv_cq, VERBS_RECV_CQE_BATCH, ep->verbs_ep.recv_wc_list)))
				break;
			else if_pf (err < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK
				    || errno == EBUSY || errno = EINTR)
					break;
				_HFI_ERROR("failed ibv_poll_cq '%s' (%d) on %s port %u epid %s\n",
					strerror(errno), errno, ep->dev_name, ep->portnum, psm3_epid_fmt_internal(ep->epid, 0));
				goto fail;
			}
			ep->verbs_ep.recv_wc_count = err;
			ep->verbs_ep.recv_wc_next = 0;
			// once drained break out of loop w/o polling CQ again
			// don't worry about small race of new pkt arriving while we
			// process the CQEs.  poll_cq is expensive so avoid doing it
			// an extra time because it will usually be empty
			done = (ep->verbs_ep.recv_wc_count < VERBS_RECV_CQE_BATCH);
		}
		// consume next wc
		wc = &(ep->verbs_ep.recv_wc_list[ep->verbs_ep.recv_wc_next++]);
		ep->verbs_ep.recv_wc_count--;
		{
#else	// VERBS_RECV_CQE_BATCH > 1
	while (1) {
		struct ibv_wc wc;
// a little atypical, but allows ifdef to be smaller scope
#undef WC
#define WC(field) ((wc).field)
		// TBD really only need to check this on 1st loop
		if_pf (ep->verbs_ep.revisit_buf) {
			buf = ep->verbs_ep.revisit_buf;
			ep->verbs_ep.revisit_buf = NULL;
			rcv_ev.payload_size = ep->verbs_ep.revisit_payload_size;
		} else if (0 == (err = ibv_poll_cq(ep->verbs_ep.recv_cq, 1, &wc))) {
			break;
		} else if_pf (err < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK
			    || errno == EBUSY || errno == EINTR)
				break;
			_HFI_ERROR("failed ibv_poll_cq '%s' (%d) on %s port %u epid %s\n",
				strerror(errno), errno, ep->dev_name, ep->portnum, psm3_epid_fmt_internal(ep->epid, 0));
			goto fail;
		} else {
#endif	// VERBS_RECV_CQE_BATCH > 1
			psmi_assert_always(WC(wr_id));
			buf = (rbuf_t)WC(wr_id);
			if_pf (WC(status)) {
				if (WC(status) != IBV_WC_WR_FLUSH_ERR)
					_HFI_ERROR("failed recv '%s' (%d) on %s port %u epid %s QP %u\n",
						ibv_wc_status_str(WC(status)), (int)WC(status), ep->dev_name, ep->portnum, psm3_epid_fmt_internal(ep->epid, 0), WC(qp_num));
				goto fail;
			}
			switch (WC(opcode)) {
#ifdef USE_RC
			case IBV_WC_RECV_RDMA_WITH_IMM:
				_HFI_MMDBG("got RDMA Write Immediate RQ CQE %u bytes\n",
							WC(byte_len));
				// wc.byte_len is len of inbound rdma write not including immed
				// wc.qp_num - local QP
				ips_protoexp_handle_immed_data(rcv_ev.proto,
						(uint64_t)(rbuf_qp_context(ep, buf)),
						RDMA_IMMED_USER_RC, WC(imm_data), WC(byte_len));
				goto repost;
				break;
#endif
			default:
				_HFI_ERROR("unexpected recv opcode %d on %s port %u epid %s QP %u\n",
					WC(opcode), ep->dev_name, ep->portnum, psm3_epid_fmt_internal(ep->epid, 0), WC(qp_num));
				goto repost;
				break;
			case IBV_WC_RECV:
				_HFI_VDBG("got CQE %u bytes\n", WC(byte_len));
				// wc.byte_len is length of data including rbuf_addition
				// actual data starts after rbuf_addition in posted recv buffer
				// if we need it wc has:
				//		qp_num - local QP
				// 		src_qp - remote QP
				// 		slid - remote SLID
				// 		probably have GRH at start of buffer with remote GID
				if_pf (_HFI_PDBG_ON)
					_HFI_PDBG_DUMP_ALWAYS(rbuf_to_buffer(buf), WC(byte_len));
				if_pf (WC(byte_len) < rbuf_addition(buf)+sizeof(struct ips_message_header)) {
					_HFI_ERROR( "unexpected small recv: %u on %s port %u\n", WC(byte_len), ep->dev_name, ep->portnum);
					goto repost;
				}
				rcv_ev.payload_size = WC(byte_len) - rbuf_addition(buf) - sizeof(struct ips_message_header);
				break;
			}
			// fall through to process recv pkt in buf of rcv_ev.payload_size
		}
		rcv_ev.p_hdr = (struct ips_message_header *)(rbuf_to_buffer(buf)+rbuf_addition(buf));
		rcv_ev.payload = (rbuf_to_buffer(buf) + rbuf_addition(buf) + sizeof(struct ips_message_header));
		_HFI_VDBG("%s receive - opcode %x\n", rbuf_qp_type_str(ep, buf),
			_get_proto_hfi_opcode(rcv_ev.p_hdr));

		PSM2_LOG_PKT_STRM(PSM2_LOG_RX,rcv_ev.p_hdr,"PKT_STRM:");
		epstaddr = ips_epstate_lookup(recvq->epstate, rcv_ev.p_hdr->connidx);

		if_pf((epstaddr == NULL) || (epstaddr->ipsaddr == NULL)) {
			rcv_ev.ipsaddr = NULL;
			recvq->recvq_callbacks.callback_packet_unknown(&rcv_ev);
		} else {
			rcv_ev.ipsaddr = epstaddr->ipsaddr;
			psmi_assert(PSMI_HOWMANY(psm3_verbs_packet_service_routines)
				== OPCODE_FUTURE_FROM - OPCODE_RESERVED);
			ret = ips_proto_process_packet(&rcv_ev,
				psm3_verbs_packet_service_routines);
			if_pf (ret == IPS_RECVHDRQ_REVISIT)
			{
				// try processing on next progress call
				_HFI_VDBG("REVISIT returned on process_packet\n");
				// process this CQE again next time called
#if VERBS_RECV_CQE_BATCH > 1
				ep->verbs_ep.recv_wc_next--;
				ep->verbs_ep.recv_wc_count++;
#else
				ep->verbs_ep.revisit_buf = buf;
				ep->verbs_ep.revisit_payload_size = rcv_ev.payload_size;
#endif
				recvq->proto->stats.rcv_revisit++;
				GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
				return PSM2_OK_NO_PROGRESS;
			}
		}
repost:
		num_done++;
		// buffer processing is done, we can requeue it on QP
		if_pf (PSM2_OK != psm3_ep_verbs_post_recv(
#ifndef USE_RC
									&ep->verbs_ep.recv_pool,
#endif
									buf))
			_HFI_ERROR( "unable to post recv on %s port %u\n", ep->dev_name, ep->portnum); // leak the buffer

		// if we can't process this now (such as an RTS we revisited and
		// ended up queueing on unexpected queue) we're told
		// to stop processing, we'll look at the rest later
		if_pf (ret == IPS_RECVHDRQ_BREAK) {
			_HFI_VDBG("stop rcvq\n");
			break;
		}
#if VERBS_RECV_CQE_BATCH > 1
	} while(! done);
#else
	}
#endif

	/* Process any pending acks before exiting */
	process_pending_acks(recvq);
	GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */

	return num_done?PSM2_OK:PSM2_OK_NO_PROGRESS;

fail:
	GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
	return PSM2_INTERNAL_ERR;
}

#endif /* PSM_VERBS */
