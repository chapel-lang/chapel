#ifdef PSM_VERBS
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

/* Copyright (c) 2003-2017 Intel Corporation. All rights reserved. */
#ifndef _VERBS_SPIO_C_
#define _VERBS_SPIO_C_

/* included header files  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sched.h>

#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "ips_proto_params.h"

#ifdef USE_RC
// update remote PSN via err_chk based on UD
static inline psm2_error_t
psm3_verbs_update_remote_psn_ud(struct ips_proto *proto, struct ips_flow *flow) {
	psm2_error_t ret = PSM2_OK;
	psmi_seqnum_t err_chk_seq;
	ips_scb_t ctrlscb;

	ctrlscb.scb_flags = 0;
	if (proto->flags & IPS_PROTO_FLAG_RCVTHREAD)
		ctrlscb.scb_flags |= IPS_SEND_FLAG_INTR;

	// When a sender doesn't get ack for a psn within certain time period,
	// it will send err_chk msg to its receiver to ask for an update for
	// this psn. If the receiver doesn't get the specified psn, it returns
	// back nak msg that will trigger the sender to re-send all messages
	// back to the psn (the GO_BACK_N approach). If the receiver already got
	// the psn, it returns back ack msg with the latest received psn.
	// Here, we purposely send an err_chk msg with a psn we know the receiver
	// already got, so we will get an ack msg with latest received psn
	err_chk_seq.psn_num = (flow->ipsaddr->verbs.remote_recv_psn - 1)
		& proto->psn_mask;
	ctrlscb.ips_lrh.bth[2] = __cpu_to_be32(err_chk_seq.psn_num);

	psm3_ips_proto_send_ctrl_message(flow, OPCODE_ERR_CHK,
			&flow->ipsaddr->ctrl_msg_queued,
			&ctrlscb, ctrlscb.cksum, 0);

	_HFI_VDBG("posted UD err_chk to get remote recv psn (cur=%d)\n",
		flow->ipsaddr->verbs.remote_recv_psn);
	return ret;
}

#ifdef USE_RDMA_READ
// get remote PSN via RDMA_Read
static inline psm2_error_t
psm3_verbs_read_remote_psn(psm2_ep_t ep, struct ips_epaddr *ipsaddr) {
	psm2_error_t ret = PSM2_OK;
	int err;
	struct ibv_send_wr wr;
	struct ibv_send_wr *bad_wr;
	struct ibv_sge list;
#ifdef PSM_RC_RECONNECT
	struct psm3_verbs_rc_qp *rc_qp = SLIST_FIRST(&ipsaddr->verbs.rc_qps);
	struct ibv_qp *qp = rc_qp->qp;
#else
	struct ibv_qp *qp = ipsaddr->verbs.rc_qp;
#endif

	// set local location to store received data
	list.addr = (uintptr_t)ipsaddr->verbs.remote_recv_psn_mr->addr;
	list.length = sizeof(ipsaddr->verbs.remote_recv_psn);
	list.lkey = ipsaddr->verbs.remote_recv_psn_mr->lkey;

	wr.next = NULL; // just post 1
	wr.wr_id = (uintptr_t)ipsaddr;
	wr.sg_list = &list;
	wr.num_sge = 1; // size of sg_list
	wr.opcode = IBV_WR_RDMA_READ;

	// set remote location where to read data from
	wr.wr.rdma.remote_addr = ipsaddr->verbs.remote_recv_seq_addr;
	wr.wr.rdma.rkey = ipsaddr->verbs.remote_recv_seq_rkey;
	wr.send_flags = IBV_SEND_SIGNALED;

	err = ibv_post_send(qp, &wr, &bad_wr);
	if_pf(err) {
		if (err != EBUSY && err != EAGAIN && err != ENOMEM)
			_HFI_ERROR("failed to get remote psn num on %s port %u: %s\n",
				ep->dev_name, ep->portnum, strerror(err));
		return PSM2_EP_NO_RESOURCES;
	}
	ipsaddr->verbs.remote_seq_outstanding = 1;
	_HFI_VDBG("posted remote_recv_psn RDMA READ: from 0x%"PRIx64" to 0x%"PRIx64" len %u rkey 0x%x\n",
		wr.wr.rdma.remote_addr, list.addr, list.length, wr.wr.rdma.rkey);
	return ret;
}
#endif

// psn update strategy. Below defines that for every 32 send attempts
// we update psn only for the first 2 attempts
#define NEED_PSN_UPDATE(count) ((count & 0x1f) < 2)
#endif

// TBD we could get also get scb->cksum out of scb
// when called:
//		scb->ips_lrh has fixed size PSM header including OPA LRH
//		payload, length is data after header
//		we don't do checksum, let verbs handle that for us
// for isCtrlMsg, scb is only partially initialized (see ips_scb.h)
// and payload and length may refer to buffers on stack
//
// we need to manage our own registered send buffers because
// in the control paths (connect, disconnect), the scb may be on the stack
// and we must be done with it when this returns.
// in the normal path the scb could be longer lived if we wanted it to be.
// OPA SDMA had a synchronous routine on control path (ips_dma_transfer_frame)
// which started the DMA and waits for it to complete
// in the normal path, scb_send_dma was used.  This sends all the scb's on a
// pending queue.  It only reaps DMA in that path if it is out of DMA resources
// a few receive paths for ack and nak also reap send DMAs.
// In general scb's just describe an IO, they do not have persistent buffers.
// So send bounce buffers avoid MR handling overheads.
// So for simplicity here we will take a lazy Send CQ reaping strategy.
// We'll reap if we need more and will do a quick reap after we post a new send
// this should keep CQ reaping out of the latency path for microbenchmarks.
// It does not seem that DMA does any reaping in other progress calls
// however the reaping in ack's may help it.
// important to note that UD Send completion just means the packet exited the
// local HFI, does not imply end to end delivery.  PIO has
// similar semantics and we know the UDP sendto simply puts a packet on
// a UDP queue for future transmission, much like a UD QP post_send works
static inline psm2_error_t
psm3_verbs_spio_transfer_frame(struct ips_proto *proto, struct ips_flow *flow,
			struct ips_scb *scb, uint32_t *payload,
			uint32_t length, uint32_t isCtrlMsg,
			uint32_t cksum_valid, uint32_t cksum
#ifdef PSM_HAVE_GPU
			, uint32_t is_gpu_payload
#endif
			)
{
	psm2_error_t ret = PSM2_OK;
	psm2_error_t err_psm; // psm errors
	int err_ibv;          // ibv errors
	psm2_ep_t ep = proto->ep;
	struct ibv_send_wr wr;
	struct ibv_send_wr *bad_wr;
	struct ibv_sge list[2];
	sbuf_t sbuf;
	sbuf_t prev_sbuf; // in case need to unalloc_sbuf
	struct ips_message_header *ips_lrh = &scb->ips_lrh;
	int send_dma = ips_scb_flags(scb) & IPS_SEND_FLAG_SEND_MR;

	struct psm3_verbs_send_allocator *allocator;
	struct ibv_qp *qp;
	uint32_t max_inline;
#ifdef PSM_RC_RECONNECT
	struct psm3_verbs_rc_qp *rc_qp;
#endif

#ifdef USE_RC
	// for RC we continue to use UD QP for control messages
	// (connect/disconnect/ack/nak/becn), this avoids issues especially during
	// QP teardown in disconnect.  We also use UD for ACK/NAK, this allows
	// flow credits to be managed over UD
	if (isCtrlMsg) {
		// always use UD QP for control messages
		allocator = &ep->verbs_ep.send_allocator;
		qp = ep->verbs_ep.qp;
		max_inline = ep->verbs_ep.qp_cap.max_inline_data;
#ifdef PSM_RC_RECONNECT
		rc_qp = NULL;
#endif
	} else {
		// curr_* will refer to RC QP if connected and RDMA=3
		// otherwise points to UD QP
		allocator = flow->ipsaddr->verbs.curr_allocator;
		qp = flow->ipsaddr->verbs.curr_qp;
		max_inline = flow->ipsaddr->verbs.curr_max_inline_data;
#ifdef PSM_RC_RECONNECT
		rc_qp = flow->ipsaddr->verbs.curr_rc_qp;
		psmi_assert(! rc_qp || rc_qp->qp == qp);
		psmi_assert(! rc_qp || allocator == &rc_qp->send_allocator);
		psmi_assert(! rc_qp || flow->ipsaddr->verbs.rc_connected);
#endif
	}
#else /* USE_RC */
	allocator = &ep->verbs_ep.send_allocator;
	qp = ep->verbs_ep.qp;
	max_inline = ep->verbs_ep.qp_cap.max_inline_data;
#endif /* USE_RC */

	psmi_assert(flow->transfer == PSM_TRANSFER_PIO);

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);
	psmi_assert_always(! cksum_valid);	// no software checksum yet

	/* When using RC, we do not need an explicit ACK as we can depend on the
	 * underlying reliability of RC itself.  This means we can:
	 *
	 *   a) avoid sending ACKREQ, and...
	 *   b) indicate to the caller that this send can be immediately
	 *      treated as though it were ACKed.
	 *
	 * However, this is only true if the upper layer protocol is itself
	 * reliable and scb can be freed immediately.  For example, PSM RMA
	 * is NOT reliable, and may discard at the receiver even though the
	 * send completed with respect to the QP (e.g. when the reply pool is
	 * exhausted to prevent deadlock).  Thus, PSM RMA depends on PSM ACKs
	 * being propagated normally.
	 *
	 * scb's are expected to indicate if the upper layer protocol is reliable
	 * via the IPS_SEND_FLAG_UNRELIABLE flag.
	 *
	 * However, when using SDMA (send_dma), we can't free the scb nor indicate
	 * a completion to the app until the DMA is done because the data is coming
	 * directly from the app, using the MR associated with the scb.
	 *
	 * When not using SDMA, the bounce buffer has a copy of the data, so the
	 * WQE is not dependent on the scb nor app buffer.
	 *
	 * When the RC QP reconnect protocol is enabled (allow_reconnect), RC QP
	 * errors may cause loss of packets, so we must depend on end to end ACKs
	 * and the PSM recovery protocols to ensure reliable data delivery.
	 */
	bool is_reliable;

	if (isCtrlMsg || send_dma || ep->allow_reconnect) {
		is_reliable = false;
	} else {
		bool is_qp_rc            = qp->qp_type == IBV_QPT_RC;
		bool is_scb_unacked_head = scb == STAILQ_FIRST(&flow->scb_unacked);
		bool is_scb_reliable     = !(scb->scb_flags & IPS_SEND_FLAG_UNRELIABLE);

		/* head of queue is a reliable scb and flow is reliable */
		is_reliable = is_qp_rc
		            && is_scb_unacked_head
		            && is_scb_reliable;

		if (is_reliable)
			ips_lrh->bth[2] &= __cpu_to_be32(~IPS_SEND_FLAG_ACKREQ);
	}

#ifdef PSM_FI
	if_pf(! is_reliable && PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sendlost, "sendlost",
#ifdef USE_RC
				"drop RC eager or any UD packet before sending",
#else
				"drop UD packet before sending",
#endif
				1, IPS_FAULTINJ_SENDLOST);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendlost, ep, "")) {
			return PSM2_OK;
		}
	}
#endif // PSM_FI

#ifdef USE_RC
	if (is_reliable && proto->credits_allow_adv_ctrl) {
#ifdef USE_RDMA_READ
		if (flow->ipsaddr->verbs.remote_seq_outstanding) {
			psm3_verbs_completion_update(ep, 1);
			if (flow->ipsaddr->verbs.remote_seq_outstanding)
				return PSM2_EP_NO_RESOURCES;
		}
#endif
		// NOTE: the remote_recv_psn is the actual received pkt psn + 1 (see ips_proto_is_expected_or_nak())
		//       and the scb psn_num is the pkt we are going to send out. So we have below diff calculation
		int diff = scb->seq_num.psn_num - flow->ipsaddr->verbs.remote_recv_psn;

		if (diff < 0)
			diff += proto->psn_mask + 1;
		if (diff >= proto->max_credits || (flow->ipsaddr->verbs.rc_cc_count && diff >= proto->min_credits)) {
#ifdef USE_RDMA_READ
			if (RDMA_READ_AVAILABLE(ep))
				psm3_verbs_read_remote_psn(ep, flow->ipsaddr);
			else
#endif
			if (NEED_PSN_UPDATE(flow->ipsaddr->verbs.rc_cc_count))
				psm3_verbs_update_remote_psn_ud(proto, flow);
			// rc_cc_count is congestion control count. right now we use it to indicate whether is
			// under congestion control. The count can potentially used in dynamic CC adjustment
			// in the future
			flow->ipsaddr->verbs.rc_cc_count += 1;
			if (flow->ipsaddr->verbs.rc_cc_count > proto->stats.pio_rc_cc_max_duration)
				proto->stats.pio_rc_cc_max_duration = flow->ipsaddr->verbs.rc_cc_count;

			return PSM2_EP_NO_RESOURCES;
		}

		flow->ipsaddr->verbs.rc_cc_count = 0;
	}
#endif
	// allocate a send buffer
	// if we have no buffers, we can return PSM2_EP_NO_RESOURCES and caller
	// will try again later
#ifdef PSM_RC_RECONNECT
	sbuf = psm3_ep_verbs_alloc_sbuf(allocator, rc_qp, &prev_sbuf);
#else
	sbuf = psm3_ep_verbs_alloc_sbuf(allocator, &prev_sbuf);
#endif
	if_pf (! sbuf) {
		// reap some SQ completions
		err_psm = psm3_verbs_completion_update(ep, 0);
		if_pf (err_psm != PSM2_OK)
			return err_psm;
#ifdef PSM_RC_RECONNECT
		// processing of completions may have detected an RC QP error
		// and reverted to UD QP while reconnect, so re-evaluate curr_*
		if (rc_qp) {
			_HFI_VDBG("RC QP re-evaluated mid-transfer old %p new %p\n",
				rc_qp, flow->ipsaddr->verbs.curr_rc_qp);
			// curr_* will refer to RC QP if still connected
			// otherwise points to UD QP
			allocator = flow->ipsaddr->verbs.curr_allocator;
			qp = flow->ipsaddr->verbs.curr_qp;
			max_inline = flow->ipsaddr->verbs.curr_max_inline_data;
			rc_qp = flow->ipsaddr->verbs.curr_rc_qp;
			psmi_assert(! rc_qp || rc_qp->qp == qp);
			psmi_assert(! rc_qp || allocator == &rc_qp->send_allocator);
			psmi_assert(! rc_qp || flow->ipsaddr->verbs.rc_connected);
		}
		sbuf = psm3_ep_verbs_alloc_sbuf(allocator, rc_qp, &prev_sbuf);
#else
		sbuf = psm3_ep_verbs_alloc_sbuf(allocator, &prev_sbuf);
#endif
	}
	if_pf (! sbuf) {
		_HFI_VDBG("out of send buffers\n");
		// try to poll send completion and see if we can free some sbuf
		psm3_verbs_completion_update(ep, 1);
		return PSM2_EP_NO_RESOURCES;
	}
	_HFI_VDBG("got sbuf %p index %lu\n", sbuf_to_buffer(sbuf), send_buffer_index(sbuf_pool(ep, sbuf), sbuf_to_buffer(sbuf)));

	// TBD - we should be able to skip sending some headers such as OPA lrh and
	// perhaps bth (does PSM use bth to hold PSNs?)
	// copy scb->ips_lrh to send buffer
	_HFI_VDBG("copy lrh %p\n", ips_lrh);
	memcpy(sbuf_to_buffer(sbuf), ips_lrh, sizeof(*ips_lrh));
	if (!send_dma) {
		// copy payload to send buffer, length could be zero, be safe
#ifdef PSM_HAVE_GPU
		if (is_gpu_payload) {
			_HFI_VDBG("copy gpu payload %p %u\n",  payload, length);
			PSM3_GPU_MEMCPY_DTOH(sbuf_to_buffer(sbuf) + sizeof(*ips_lrh),
				payload, length);
		} else
#endif
		{
			_HFI_VDBG("copy host payload %p %u\n",  payload, length);
			memcpy(sbuf_to_buffer(sbuf)+sizeof(*ips_lrh), payload, length);
		}
	}
	_HFI_VDBG("%s send - opcode %x dma %d MR %p\n", qp_type_str(qp),
            _get_proto_hfi_opcode(ips_lrh), !!send_dma, scb->mr);
	// we don't support software checksum
	psmi_assert_always(! (proto->flags & IPS_PROTO_FLAG_CKSUM));
	psmi_assert_always(qp);	// make sure we aren't called too soon
	list[0].addr = (uintptr_t)sbuf_to_buffer(sbuf);
	list[0].lkey = sbuf_lkey(ep, sbuf);
	if (send_dma) {
		list[0].length = sizeof(*ips_lrh);	// note no UD_ADDITION
		list[1].addr = scb->mr->iova
			+ ((uintptr_t)ips_scb_buffer(scb) - (uintptr_t)scb->mr->addr);
		psmi_assert(ips_scb_buffer(scb) == payload);
#ifdef RNDV_MOD
		psmi_assert(psm3_verbs_user_space_mr(scb->mr));
#endif
		list[1].length = length;
		list[1].lkey = scb->mr->lkey;
		sbuf->scb = scb;
		scb->sdma_outstanding++;
	} else {
		list[0].length = sizeof(*ips_lrh)+ length ;	// note no UD_ADDITION
		list[1].length = 0;
	}
	psmi_assert(!((uintptr_t)sbuf & VERBS_SQ_WR_ID_MASK));
	wr.wr_id = (uintptr_t)sbuf | VERBS_SQ_WR_ID_SEND;	// we'll get this back in completion
	wr.next = NULL;	// just post 1
		// we don't use the scb as wr_id since for PIO they may be freed
		// immediately after a succesful call to transfer
	wr.sg_list = list;
	if (send_dma) {
		wr.num_sge = 2;	// size of sg_list
		// we need Send DMA completions so we can reap them quickly and
		// maintain scb->sdma_outstanding to allow scb->mr release on ACK
		wr.send_flags = IBV_SEND_SIGNALED;	// get a completion
	} else {
		wr.num_sge = 1;	// size of sg_list
		wr.send_flags = 0;
	}
	wr.opcode = IBV_WR_SEND;
	// we want to only get occasional send completions
	// and use them to release a whole set of buffers for reuse
	// For USE_RC this is imperfect, we track when to ask for a CQE
	// per RC QP.  However when traffic is using varied RC QPs, we may be
	// left with some RC QPs with up to VERBS_SEND_CQ_COALLESCE-1 unsignalled
	// WQEs and no traffic for a while, hence consuming a few send buffers per
	// QP.  By tracking it per RC QP we at least avoid the case of a rotating
	// traffic pattern never asking for a CQE for a given QP
	if_pf ( ! --(allocator->send_num_til_coallesce)) {
		wr.send_flags = IBV_SEND_SIGNALED;	// get a completion
		allocator->send_num_til_coallesce = VERBS_SEND_CQ_COALLESCE;
	}
	if_pf (ips_lrh->khdr.kdeth0 & __cpu_to_le32(IPS_SEND_FLAG_INTR)) {
		_HFI_VDBG("send solicted event\n");
		wr.send_flags |= IBV_SEND_SOLICITED;
	}

		// for small messages, we may use IBV_SEND_INLINE for performance
	if (! send_dma && list[0].length <= max_inline)
		wr.send_flags |= IBV_SEND_INLINE;
#ifdef PSM_FI
		// don't inject bad lkey for inline since lkey ignored
	else if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sq_lkey, "sq_lkey",
				"send "
#ifdef USE_RC
				"RC eager or any "
#endif
				"UD packet with bad lkey",
				0, IPS_FAULTINJ_SQ_LKEY);
#ifdef PSM_RC_RECONNECT
		PSM3_FAULTINJ_STATIC_DECL(fi_rc_sq_lkey, "rc_sq_lkey",
				"send RC eager packet with bad lkey",
				ep->allow_reconnect?1:0,
				IPS_FAULTINJ_RC_SQ_LKEY);
#endif
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sq_lkey, ep,
			    " QP %u wr_id %p op 0x%x len %u", qp->qp_num,
			    (void *)wr.wr_id, _get_proto_hfi_opcode(ips_lrh),
			    (uint32_t)sizeof(*ips_lrh) + length ))
			list[0].lkey = IPS_BAD_LKEY;
#ifdef PSM_RC_RECONNECT
		else if_pf(rc_qp && PSM3_FAULTINJ_IS_FAULT(fi_rc_sq_lkey, ep,
			    " RC QP %u wr_id %p op 0x%x len %u", qp->qp_num,
			    (void *)wr.wr_id, _get_proto_hfi_opcode(ips_lrh),
			    (uint32_t)sizeof(*ips_lrh) + length ))
			list[0].lkey = IPS_BAD_LKEY;
#endif /* PSM_RC_RECONNECT */
	}
#endif // PSM_FI
	//wr.imm_data = 0;	// only if we use IBV_WR_SEND_WITH_IMM;
	// ud fields are ignored for RC send (overlay fields for RDMA)
	// so reduce branches by just always filling in these few fields
	psmi_assert_always(flow->path->verbs.pr_ah);
	wr.wr.ud.ah = flow->path->verbs.pr_ah;
	wr.wr.ud.remote_qpn = flow->ipsaddr->verbs.remote_qpn;
	wr.wr.ud.remote_qkey = ep->verbs_ep.qkey;

	if (_HFI_PDBG_ON) {
		_HFI_PDBG_ALWAYS("len %u, QP %p (%u) remote qpn %u payload %u\n",
			list[0].length+list[1].length,
			qp, qp->qp_num,
#ifdef USE_RC
				(qp->qp_type != IBV_QPT_UD)? flow->ipsaddr->verbs.remote_qpn :
#endif
				 wr.wr.ud.remote_qpn,
			length);
		_HFI_PDBG_DUMP_ALWAYS((uint8_t*)list[0].addr, list[0].length);
		// cannot dump list[1] since SDMA may be a GPU address or iova
		// could be different from CPU virtual
	}
	err_ibv = ibv_post_send(qp, &wr, &bad_wr);
	if_pf (err_ibv) {
		if (err_ibv != EBUSY && err_ibv != EAGAIN && err_ibv != ENOMEM)
			_HFI_ERROR("failed to post SQ on %s: %s", ep->dev_name, strerror(err_ibv));
		proto->stats.post_send_fail++;
		// unwind our allocation and our update to send_num_til_coalllesce
		if_pf ( (allocator->send_num_til_coallesce) == VERBS_SEND_CQ_COALLESCE)
			(allocator->send_num_til_coallesce) = 1;
		psm3_ep_verbs_unalloc_sbuf(allocator, sbuf, prev_sbuf);
		ret = PSM2_EP_NO_RESOURCES;
	}
	_HFI_VDBG("done spio_transfer_frame: len %u, remote qpn %u\n",
		list[0].length +list[1].length,
#ifdef USE_RC
		qp->qp_type != IBV_QPT_UD ? flow->ipsaddr->verbs.remote_qpn :
#endif
		wr.wr.ud.remote_qpn);
	// reap any completions
	err_psm = psm3_verbs_completion_update(ep, 0);
	if_pf (err_psm != PSM2_OK)
		return err_psm;
	return (is_reliable && ret == PSM2_OK) ? PSM2_RELIABLE_DATA_SENT : ret;
}

#endif /* PSM_VERBS */
#endif /* _VERBS_SPIO_C_ */
