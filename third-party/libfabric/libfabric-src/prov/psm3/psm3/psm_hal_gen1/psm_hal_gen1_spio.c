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

/* included header files  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sched.h>

#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "psm_hal_gen1_spio.h"
#include "ips_proto_params.h"

/* Report PIO stalls every 20 seconds at the least */
#define SPIO_STALL_WARNING_INTERVAL	  (nanosecs_to_cycles(20e9))
#define SPIO_MAX_CONSECUTIVE_SEND_FAIL	  (1<<20)	/* 1M */
/* RESYNC_CONSECUTIVE_SEND_FAIL has to be a multiple of MAX_CONSECUTIVE */
#define SPIO_RESYNC_CONSECUTIVE_SEND_FAIL (1<<4)	/* 16 */














/*
 * Check and process events
 * return value:
 *  PSM2_OK: normal events processing;
 *  PSM2_OK_NO_PROGRESS: no event is processed;
 */
static PSMI_HAL_INLINE psm2_error_t
ips_spio_process_events(const struct ptl *ptl_gen)
{
	// TODD - TBD - check link status events for UD/UDP
	return PSM2_OK;
}


// TBD we could get also get scb->cksum out of scb
// when called:
//		scb->ips_lrh has fixed size PSM header including OPA LRH
//		payload, length is data after header
//		we don't do checksum, let verbs handle that for us
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
psm2_error_t
ips_spio_transfer_frame(struct ips_proto *proto, struct ips_flow *flow,
			struct ips_scb *scb, uint32_t *payload,
			uint32_t length, uint32_t isCtrlMsg,
			uint32_t cksum_valid, uint32_t cksum
#ifdef PSM_CUDA
			, uint32_t is_cuda_payload
#endif
			)
{
	psm2_error_t ret = PSM2_OK;
	psm2_error_t err;
	psm2_ep_t ep = proto->ep;
	struct ibv_send_wr wr;
	struct ibv_send_wr *bad_wr;
	struct ibv_sge list[2];
	sbuf_t sbuf;
	struct ips_message_header *ips_lrh = &scb->ips_lrh;
	int send_dma = ips_scb_flags(scb) & IPS_SEND_FLAG_SEND_MR;

	// these defines are bit ugly, but make code below simpler with less ifdefs
	// once we decide if USE_RC is valuable we can cleanup
	// for RC we continue to use UD QP for control messages
	// (connect/disconnect/ack/nak/becn), this avoids issues especially during
	// QP teardown in disconnect.  We also use UD for ACK/NAK, this allows
	// flow credits to be managed over UD
#define USE_ALLOCATOR (isCtrlMsg?&ep->verbs_ep.send_allocator:flow->ipsaddr->use_allocator)
#define USE_QP (isCtrlMsg?ep->verbs_ep.qp:flow->ipsaddr->use_qp)
#define USE_MAX_INLINE (isCtrlMsg?ep->verbs_ep.qp_cap.max_inline_data:flow->ipsaddr->use_max_inline_data)

#ifdef PSM_FI
	if_pf(PSMI_FAULTINJ_ENABLED_EP(ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_sendlost, "sendlost",
				"drop "
				"RC eager or any "
				"UD packet before sending",
				1, IPS_FAULTINJ_SENDLOST);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_sendlost, ""))
			return PSM2_OK;
	}
#endif // PSM_FI
	PSMI_LOCK_ASSERT(proto->mq->progress_lock);
	psmi_assert_always(! cksum_valid);	// no software checksum yet
	// allocate a send buffer
	// if we have no buffers, we can return PSM2_EP_NO_RESOURCES and caller
	// will try again later
	sbuf = __psm2_ep_verbs_alloc_sbuf(USE_ALLOCATOR);
	if_pf (! sbuf) {
		// reap some SQ completions
		ret = psm2_verbs_completion_update(proto->ep);
		if_pf (ret != PSM2_OK)
			return ret;
		sbuf = __psm2_ep_verbs_alloc_sbuf(USE_ALLOCATOR);
	}
	if_pf (! sbuf) {
		_HFI_VDBG("out of send buffers\n");
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
		_HFI_VDBG("copy payload %p %u\n",  payload, length);
#ifdef PSM_CUDA
		if (is_cuda_payload) {
			//_HFI_ERROR("cuMemcpyDtoH %p %u\n", payload, length);
			PSMI_CUDA_CALL(cuMemcpyDtoH, sbuf_to_buffer(sbuf)+sizeof(*ips_lrh),
				(CUdeviceptr)payload, length);
		} else
#endif
		{
			memcpy(sbuf_to_buffer(sbuf)+sizeof(*ips_lrh), payload, length);
		}
	}
	_HFI_VDBG("%s send - opcode %x dma %d MR %p\n", qp_type_str(USE_QP),
            _get_proto_hfi_opcode((struct  ips_message_header*)sbuf_to_buffer(sbuf)), !!send_dma, scb->mr);
	// we don't support software checksum
	psmi_assert_always(! (proto->flags & IPS_PROTO_FLAG_CKSUM));
	psmi_assert_always(USE_QP);	// make sure we aren't called too soon
	list[0].addr = (uintptr_t)sbuf_to_buffer(sbuf);
	list[0].lkey = sbuf_lkey(ep, sbuf);
	if (send_dma) {
		list[0].length = sizeof(*ips_lrh);	// note no UD_ADDITION
		list[1].addr = scb->mr->iova
			+ ((uintptr_t)ips_scb_buffer(scb) - (uintptr_t)scb->mr->addr);
		psmi_assert(ips_scb_buffer(scb) == payload);
#ifdef RNDV_MOD
		psmi_assert(psm2_verbs_user_space_mr(scb->mr));
#endif
		list[1].length = length;
		list[1].lkey = scb->mr->lkey;
	} else {
		list[0].length = sizeof(*ips_lrh)+ length ;	// note no UD_ADDITION
		list[1].length = 0;
	}
#ifdef PSM_FI
	if_pf(PSMI_FAULTINJ_ENABLED_EP(ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_sq_lkey, "sq_lkey",
				"send "
				"RC eager or any "
				"UD packet with bad lkey",
				0, IPS_FAULTINJ_SQ_LKEY);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_sq_lkey, " QP %u", USE_QP->qp_num ))
			list[0].lkey = 0x55;
	}
#endif // PSM_FI
	wr.next = NULL;	// just post 1
	psmi_assert(!((uintptr_t)sbuf & VERBS_SQ_WR_ID_MASK));
	wr.wr_id = (uintptr_t)sbuf | VERBS_SQ_WR_ID_SEND;	// we'll get this back in completion
		// we don't use the scb as wr_id since for PIO they may be freed
		// immediately after a succesful call to transfer
	wr.sg_list = list;
	if (send_dma)
		wr.num_sge = 2;	// size of sg_list
	else
		wr.num_sge = 1;	// size of sg_list
	wr.opcode = IBV_WR_SEND;
	// we want to only get occasional send completions
	// and use them to release a whole set of buffers for reuse
	// For USE_RC this is imperfect, we track when to ask for a CQE
	// per RC QP.  However when traffic is using varied RC QPs, we may be
	// left with some RC QPs with up to VERBS_SEND_CQ_COALLESCE-1 unsignalled
	// WQEs and no traffic for a while, hence consuming a few send buffers per
	// QP.  By tracking it per RC QP we at least avoid the case of a rotating
	// traffic pattern never asking for a CQE for a given QP
	if_pf ( ! --(USE_ALLOCATOR->send_num_til_coallesce)) {
		wr.send_flags = IBV_SEND_SIGNALED;	// get a completion
		USE_ALLOCATOR->send_num_til_coallesce = VERBS_SEND_CQ_COALLESCE;
	} else {
		wr.send_flags = 0;
	}
	if_pf (ips_lrh->khdr.kdeth0 & __cpu_to_le32(IPS_SEND_FLAG_INTR)) {
		_HFI_VDBG("send solicted event\n");
		wr.send_flags |= IBV_SEND_SOLICITED;
	}

		// for small messages, we may use IBV_SEND_INLINE for performance
	if (! send_dma && list[0].length <= USE_MAX_INLINE)
		wr.send_flags |= IBV_SEND_INLINE;
	//wr.imm_data = 0;	// only if we use IBV_WR_SEND_WITH_IMM;
	// ud fields are ignored for RC send (overlay fields for RDMA)
	// so reduce branches by just always filling in these few fields
	//if (USE_QP->qp_type == IBV_QPT_UD)
	psmi_assert_always(flow->path->ah);
	wr.wr.ud.ah = flow->path->ah;
	wr.wr.ud.remote_qpn = flow->ipsaddr->remote_qpn;
	wr.wr.ud.remote_qkey = ep->verbs_ep.qkey;

	if (_HFI_PDBG_ON) {
		_HFI_PDBG("ud_transfer_frame: len %u, remote qpn %u payload %u\n",
			list[0].length+list[1].length,
				(USE_QP->qp_type != IBV_QPT_UD)? flow->ipsaddr->remote_qpn :
				 wr.wr.ud.remote_qpn,
			length);
		_HFI_PDBG_DUMP((uint8_t*)list[0].addr, list[1].length);
		_HFI_PDBG("post send: QP %p (%u)\n", USE_QP, USE_QP->qp_num);
	}
	if_pf (ibv_post_send(USE_QP, &wr, &bad_wr)) {
		if (errno != EBUSY && errno != EAGAIN && errno != ENOMEM)
			_HFI_ERROR("failed to post SQ on %s: %s", ep->dev_name, strerror(errno));
		proto->stats.post_send_fail++;
		ret = PSM2_EP_NO_RESOURCES;
	}
	_HFI_VDBG("done ud_transfer_frame: len %u, remote qpn %u\n",
		list[0].length +list[1].length,
		(USE_QP->qp_type != IBV_QPT_UD)? flow->ipsaddr->remote_qpn :
 		wr.wr.ud.remote_qpn);
	// reap any completions
	err = psm2_verbs_completion_update(proto->ep);
	if_pf (err != PSM2_OK)
		return err;
	return ret;
#undef USE_ALLOCATOR
#undef USE_QP
#undef USE_MAX_INLINE
}

