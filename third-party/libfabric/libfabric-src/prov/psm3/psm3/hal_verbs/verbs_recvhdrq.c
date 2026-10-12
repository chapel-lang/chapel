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
#ifdef PSM_HAVE_RDMA_ERR_CHK
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
					if (ep->reconnect_timeout)
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

// process a WC with an error completion which was returned by ibv_pollcq
// returns:
//	PSM2_OK - WC has been processed and polling can continue
//	other - this is an unrecoverable for the QP
// When user RC QP reconnection is allowed, failures can be recovered
// from by replacing the RC QP and re-establishing the RC connection.
// When not allowed or for errors on UD QPs, the error is unrecoverable.
// If using SRQ, shouldn't get recv error CQEs upon QP error
// so treat as unrecoverable too
static psm2_error_t psm3_verbs_recv_wc_error(psm2_ep_t ep, struct ibv_wc *wc)
{
#ifdef USE_RC
	rbuf_t buf = (rbuf_t)wc->wr_id;
#endif
#ifdef PSM_RC_RECONNECT
	struct psm3_verbs_rc_qp *rc_qp;
	ips_epaddr_t *ipsaddr;
#endif

	// this funciton only called for WCs with error
	psmi_assert(wc->status);

	// only the following fields in wc are valid:
	//	wr_id, status, vendor_err, qp_num 

#ifdef USE_RC
	psmi_assert(buf);
#endif

	// errors on SRQ, on UD QPs or when reconnect not allowed are
	// unrecoverable
#ifdef USE_RC
	if (! ep->allow_reconnect || buf->pool->for_srq
		|| psm3_verbs_is_ud_qp_num(ep, wc->qp_num)) {
#else
	{
#endif
		// only report the 1st error (non-flush), it will
		// be followed by a burst of FLUSH_ERR for other queued WQEs
		if (wc->status != IBV_WC_WR_FLUSH_ERR)
			_HFI_ERROR("failed recv '%s' (%d) on %s port %u epid %s QP %u\n",
				ibv_wc_status_str(wc->status),
				(int)(wc->status), ep->dev_name, ep->portnum,
				psm3_epid_fmt_internal(ep->epid, 0),
				wc->qp_num);
		// QP is now in QPS_ERR and can't send/recv packets
#ifdef PSM_RC_RECONNECT
		// no need to worry about decrementing rc_qp->recv_pool.posted
		// (via buf->pool->posted--)
#endif
		// upcoming async event will cause fatal error
		return PSM2_INTERNAL_ERR;
	}
#if defined(USE_RC) && ! defined(PSM_RC_RECONNECT)
	psmi_assert(0);	// should not get here, allow_reconnect always false
	return PSM2_INTERNAL_ERR;
#endif
#ifdef PSM_RC_RECONNECT
	// For user space RC QP, the QP is now in QPS_ERR and we
	// need to replace and reconnect it.
	psmi_assert(! psm3_verbs_is_ud_qp_num(ep, wc->qp_num));
	rc_qp = psm3_verbs_rc_qp_from_recv_pool(buf->pool);
	psmi_assert(rc_qp);
	psmi_assert(rc_qp->qp->qp_num == wc->qp_num);

	psmi_assert(rc_qp->recv_pool.posted > 0);
	rc_qp->recv_pool.posted--;
	ipsaddr = rc_qp->ipsaddr;
	psmi_assert(ipsaddr == (ips_epaddr_t *)rbuf_qp_context(ep, buf));

	// treat a FLUSH or other error similar (except for logging)
	// Depending on order of polling, may detect the QP issue via an
	// RQ FLUSH CQE prior to detecting the SQ error CQE, in which case
	// we want to still start the reconnection protocol ASAP (via
	// psm3_ips_proto_connection_error)

	// log 1st observed issue on QP and all non-flush error CQEs.
	// We skip logging of other flush CQEs since there can be many.
	if ((_HFI_CONNDBG_ON || _HFI_DBG_ON)
	    && (wc->status != IBV_WC_WR_FLUSH_ERR || ! rc_qp->draining)) {
		_HFI_DBG_ALWAYS("[ipsaddr %p] failed recv buf %p on %s port %u epid %s QP %u posted %u + %u status: '%s' (%d)\n",
			ipsaddr, buf, ep->dev_name, ep->portnum,
			psm3_epid_fmt_internal(ep->epid, 0), wc->qp_num,
			rc_qp->send_posted, rc_qp->recv_pool.posted,
			ibv_wc_status_str(wc->status), (int)wc->status);
	}
	if (wc->status != IBV_WC_WR_FLUSH_ERR)
		ipsaddr->epaddr.proto->epaddr_stats.recv_wc_error++;

	// only call connect_error for 1st error on QP, if already
	// draining due to a separate send error, recv error or
	// reconnect packet don't call connection_error again.
	// psm3_ips_proto_connection_error tests ipsaddr->allow_reconnect
	// and reports fatal error if reconnect not allowed
	if (! ipsaddr->allow_reconnect || ! rc_qp->draining) {
		// for return != no progress, rc_qp may have been freed
		// fatal error in connection_error prior to other error returns
		if (PSM2_OK_NO_PROGRESS == psm3_ips_proto_connection_error(
				ipsaddr, "recv",
				ibv_wc_status_str(wc->status), wc->status, 0))
			psm3_verbs_free_rc_qp_if_empty("RQ WC Err",rc_qp);
	} else
		psm3_verbs_free_rc_qp_if_empty("RQ WC Err",rc_qp);
	return PSM2_OK;
#endif /* PSM_RC_RECONNECT */
}

#ifdef PSM_RC_RECONNECT_SRQ
// since the CQ has been emptied, we know any RQ WQEs and CQEs related to
// the given draining RC QP have now been fully processed and we can
// mark its receive side as fully drained.
static void recv_cq_empty(psm2_ep_t ep)
{
	struct psm3_verbs_rc_qp *rc_qp;

	while (NULL != (rc_qp = SLIST_FIRST(&ep->verbs_ep.qps_draining))) {
		psmi_assert(rc_qp->draining);
		SLIST_REMOVE_HEAD(&ep->verbs_ep.qps_draining, drain_next);
		psmi_assert(rc_qp->recv_pool.posted == 1);
		rc_qp->recv_pool.posted--;
		psm3_verbs_free_rc_qp_if_empty("RQ CQ Empty", rc_qp);
	}
}
#endif

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
		if (! ep->verbs_ep.recv_wc_count) {
			// TBD - negative error return is possible but unlikely
			err = ibv_poll_cq(ep->verbs_ep.recv_cq, VERBS_RECV_CQE_BATCH, ep->verbs_ep.recv_wc_list);
			if (err == 0) {
#ifdef PSM_RC_RECONNECT_SRQ
				// the CQE has been drained
				if (SLIST_FIRST(&ep->verbs_ep.qps_draining))
					recv_cq_empty(ep);
#endif
				break;
			} else if_pf(err < 0) {
				// ibv returns negative errno in the specific case of poll_cq
				err = -err;
				if (err == EAGAIN || err == EWOULDBLOCK || err == EBUSY || err == EINTR)
					break;
				_HFI_ERROR("failed ibv_poll_cq '%s' (%d) on %s port %u epid %s\n",
					strerror(err), err, ep->dev_name, ep->portnum,
					psm3_epid_fmt_internal(ep->epid, 0));
				goto fail;
			}
			// else positive value indicates number of completions returned
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
		struct ibv_wc wc[1];
		// TBD really only need to check this on 1st loop
		if_pf (ep->verbs_ep.revisit_buf) {
			buf = ep->verbs_ep.revisit_buf;
			ep->verbs_ep.revisit_buf = NULL;
			rcv_ev.payload_size = ep->verbs_ep.revisit_payload_size;
		} else if (0 == (err = ibv_poll_cq(ep->verbs_ep.recv_cq, 1, wc))) {
#ifdef PSM_RC_RECONNECT_SRQ
			// the CQE has been drained
			if (SLIST_FIRST(&ep->verbs_ep.qps_draining))
				recv_cq_empty(ep);
#endif
			break;
		} else if_pf (err < 0) {
			// ibv returns negative errno in the specific case of poll_cq
			err = -err;
			if (err == EAGAIN || err == EWOULDBLOCK || err == EBUSY || err == EINTR)
				break;
			_HFI_ERROR("failed ibv_poll_cq '%s' (%d) on %s port %u epid %s\n",
				strerror(err), err, ep->dev_name, ep->portnum,
				psm3_epid_fmt_internal(ep->epid, 0));
			goto fail;
		} else {
#endif	// VERBS_RECV_CQE_BATCH > 1
			_HFI_VDBG("poll_cq recv WC: QP %u wr_id: %p status: '%s' (%d)\n",
				wc->qp_num, (void*)wc->wr_id,
				ibv_wc_status_str(wc->status), wc->status);
#ifdef PSM_DEBUG
			if (! wc->wr_id)
				_HFI_ERROR("poll_cq recv WC: QP %u wr_id: %p status: '%s' (%d)\n",
					wc->qp_num, (void*)wc->wr_id,
					ibv_wc_status_str(wc->status), wc->status);
#endif
			psmi_assert_always(wc->wr_id);
			if_pf (wc->status) {
				if (PSM2_OK != psm3_verbs_recv_wc_error(ep, wc))
					goto fail;	// unrecoverable
				// recoverable, don't repost, QP has issue
				num_done++;
				continue;
			}
			buf = (rbuf_t)wc->wr_id;
			switch (wc->opcode) {
#ifdef USE_RC
			case IBV_WC_RECV_RDMA_WITH_IMM:
				_HFI_MMDBG("got RDMA Write Immediate RQ CQE %u bytes\n",
					wc->byte_len);
				// wc->byte_len is len of inbound rdma write not including immed
				// wc->qp_num - local QP
				ips_protoexp_handle_immed_data(rcv_ev.proto,
						(uint64_t)(rbuf_qp_context(ep, buf)),
						RDMA_IMMED_USER_RC, wc->imm_data, wc->byte_len);
				goto repost;
				break;
#endif
			default:
				_HFI_ERROR("unexpected recv opcode %d on %s port %u epid %s QP %u\n",
					wc->opcode, ep->dev_name, ep->portnum,
					psm3_epid_fmt_internal(ep->epid, 0), wc->qp_num);
				goto repost;
				break;
			case IBV_WC_RECV:
				_HFI_VDBG("got CQE %u bytes\n", wc->byte_len);
				// wc->byte_len is length of data including rbuf_addition
				// actual data starts after rbuf_addition in posted recv buffer
				// if we need it wc has:
				//		qp_num - local QP
				// 		src_qp - remote QP
				// 		slid - remote SLID
				// 		probably have GRH at start of buffer with remote GID
				if_pf (_HFI_PDBG_ON)
					_HFI_PDBG_DUMP_ALWAYS(rbuf_to_buffer(buf), wc->byte_len);
				if_pf (wc->byte_len < rbuf_addition(buf)+sizeof(struct ips_message_header)) {
					_HFI_ERROR( "unexpected small recv: %u on %s port %u\n",
						wc->byte_len, ep->dev_name, ep->portnum);
					goto repost;
				}
				rcv_ev.payload_size = wc->byte_len - rbuf_addition(buf) - sizeof(struct ips_message_header);
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
#ifdef PSM_RC_RECONNECT
		buf->pool->posted--; // consumed WQE and done with buffer
#endif
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
	} while(ep->verbs_ep.recv_wc_count || !done);
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
