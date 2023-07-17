/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_scb.h"
#include "ips_proto.h"
#include "psm_mq_internal.h"
#include "ips_expected_proto.h"
#include "ips_proto_help.h"

PSMI_NEVER_INLINE(ips_scb_t *
		  ips_poll_scb(struct ips_proto *proto,
			       int npkts, int len, uint32_t flags, int istiny))
{
	ips_scb_t *scb = NULL;
	psmi_assert(npkts > 0);
	psm2_error_t err;

	proto->stats.scb_egr_unavail_cnt++;

	PSMI_BLOCKUNTIL(proto->ep, err,
			((scb =
			  (istiny ?
			   psm3_ips_scbctrl_alloc_tiny(&proto->scbc_egr) :
			   psm3_ips_scbctrl_alloc(&proto->scbc_egr, npkts, len,
					     flags))) != NULL));
	psmi_assert(scb != NULL);
	return scb;
}

PSMI_ALWAYS_INLINE(ips_scb_t *mq_alloc_tiny(struct ips_proto *proto))
{
	ips_scb_t *scb = psm3_ips_scbctrl_alloc_tiny(&proto->scbc_egr);
	/* common case should branch right through */
	if_pt(scb != NULL)
	    return scb;
	else
	return ips_poll_scb(proto, 1, 0, 0, 1);
}

PSMI_ALWAYS_INLINE(
ips_scb_t *
mq_alloc_pkts(struct ips_proto *proto, int npkts, int len, uint32_t flags))
{
	psmi_assert(npkts > 0);
	ips_scb_t *scb = psm3_ips_scbctrl_alloc(&proto->scbc_egr, npkts, len, flags);
	if_pt(scb != NULL) {
		return scb;
	}
	else {
		return ips_poll_scb(proto, npkts, len, flags,
				    0 /* not tiny scb */);
	}
}

#ifdef PSM_HAVE_REG_MR
// Used for short sync send with Send DMA.
// Caller (psm3_ips_proto_process_ack or ipsm3_ps_proto_process_nak)
// will ensure !sdma_outstanding for the scb which triggered the callback.
static
int ips_proto_scb_mr_complete(void *context, uint32_t nbytes)
{
	ips_scb_t *scb = (ips_scb_t *)context;
	if (scb->mr) {
		_HFI_MMDBG("SDMA complete, releasing MR: lkey: 0x%x\n", scb->mr->lkey);
		psmi_assert(!scb->sdma_outstanding);
		psm3_verbs_release_mr(scb->mr);
		scb->mr = NULL;
		ips_tid_mravail_callback(scb->flow->ipsaddr->epaddr.proto);
	}
	return IPS_RECVHDRQ_CONTINUE;
}
#endif

// Handle end to end completion of eager and LONG_DATA sends
// as well as short with send DMA.
// Caller (psm3_ips_proto_process_ack or ipsm3_ps_proto_process_nak)
// will ensure !sdma_outstanding for the scb which triggered the callback.
// For multi-packet messages, like mq_eager and LONG_DATA, all packets
// will use the same MR, same HW queue and same SendDMA or simple send strategy,
// even if multiple scbs used.  As such, the check in caller for
// !sdma_outstanding on last scb in message will imply all prior scb's in
// message have also completed all their send DMAs.
// Note: since all packets in a message use the same flow and HW queue,
// and we issue ERR_CHK on same HW queue and flow as original packets,
// we can't get a NAK causing us to resend earlier packets followed by
// a late ACK of later packets (which would leave some earlier scb SDMA
// incomplete).
static
int ips_proto_mq_eager_complete(void *reqp, uint32_t nbytes)
{
	psm2_mq_req_t req = (psm2_mq_req_t) reqp;

	/* This code path is executed when the send is on a device buffer
	 * and the receive is completed using eager buffers. As there is no
	 * completion notification sent to the sender, this is the only place
	 * where send side chb's can be freed and put back into the mpool.
	 */
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	struct ips_gpu_hostbuf *chb;
	if (req->gpu_hostbuf_used) {
		while (!STAILQ_EMPTY(&req->sendreq_prefetch)) {
			/* If any prefetched buffers weren't used, they
			   must be reclaimed here. */
			chb = STAILQ_FIRST(&req->sendreq_prefetch);
			STAILQ_REMOVE_HEAD(&req->sendreq_prefetch,
						   req_next);
			PSM3_GPU_HOSTBUF_RESET(chb);
			psm3_mpool_put(chb);
		}
	}
#endif

	req->send_msgoff += nbytes;
	/*
	 * the reason to use >= is because
	 * we may have DW pad in nbytes.
	 */
	if (req->send_msgoff >= req->req_data.send_msglen) {
#ifdef PSM_HAVE_REG_MR
		// If we predicted use of RDMA and pre-registered our buffer when we
		// sent RTS, and receiver chose LONG_DATA in CTS, we can end up here
		// and need to release our MR.
		// Also applicable to eager and short with Send DMA
		if (req->mr) {
			_HFI_MMDBG("RTS complete, releasing MR: rkey: 0x%x\n", req->mr->rkey);
			psm3_verbs_release_mr(req->mr);
			req->mr = NULL;
			ips_tid_mravail_callback(req->rts_peer->proto);
		}
#endif
		req->state = MQ_STATE_COMPLETE;
		ips_barrier();
		if(!psmi_is_req_internal(req))
			mq_qq_append(&req->mq->completed_q, req);
	}
	return IPS_RECVHDRQ_CONTINUE;
}

static
void ips_proto_mq_rv_complete(psm2_mq_req_t req)
{
	psm3_mq_handle_rts_complete(req);
}

PSMI_ALWAYS_INLINE(
void
ips_shortcpy(void *vdest, const void *vsrc, uint32_t nchars))
{
	unsigned char *dest = vdest;
	const unsigned char *src = vsrc;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (PSMI_IS_GPU_ENABLED && (PSMI_IS_GPU_MEM(vdest) || PSMI_IS_GPU_MEM(vsrc))) {
		PSM3_GPU_MEMCPY(vdest, vsrc, nchars);
		return;
	}
#endif

	if (nchars >> 2)
		psm3_dwordcpy((uint32_t *) dest, (uint32_t *) src, nchars >> 2);
	dest += (nchars >> 2) << 2;
	src += (nchars >> 2) << 2;
	switch (nchars & 0x03) {
	case 3: *dest++ = *src++;
	/* fall through */
	case 2: *dest++ = *src++;
	/* fall through */
	case 1: *dest++ = *src++;
	}
	return;
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
PSMI_ALWAYS_INLINE(
void
ips_shortcpy_host_mem(void *vdest, const void *vsrc, uint32_t nchars))
{
	unsigned char *dest = vdest;
	const unsigned char *src = vsrc;

	if (nchars >> 2)
		psm3_dwordcpy((uint32_t *) dest, (uint32_t *) src, nchars >> 2);
	dest += (nchars >> 2) << 2;
	src += (nchars >> 2) << 2;
	switch (nchars & 0x03) {
	case 3: *dest++ = *src++;
	/* fall through */
	case 2: *dest++ = *src++;
	/* fall through */
	case 1: *dest++ = *src++;
	}
	return;
}
#endif

/*
 * Mechanism to capture PIO-ing or DMA-ing the MQ message envelope
 *
 * Recoverable errors:
 * PSM2_OK: If PIO, envelope is sent.
 *	   If DMA, all queued up packets on flow were flushed.
 *
 * Recoverable errors converted to PSM2_OK just before return:
 * PSM2_OK_NO_PROGRESS: DMA-only, flushed 1 but not all queued packets.
 * PSM2_EP_NO_RESOURCES:
 *	   If PIO, no pio available or cable currently pulled.
 *	   If DMA, can be that no scb's available to handle unaligned packets
 *	           or writev returned a recoverable error (no mem for
 *	           descriptors, dma interrupted or no space left in dma queue).
 *
 * Unrecoverable errors (PIO or DMA).
 * PSM2_EP_DEVICE_FAILURE: Unexpected error calling writev(), chip failure,
 *			  rxe/txe parity error.
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 */
PSMI_ALWAYS_INLINE(
psm2_error_t
ips_mq_send_envelope(struct ips_proto *proto, struct ips_flow *flow,
		     struct ips_scb *scb, int do_flush))
{
	psm2_error_t err = PSM2_OK;

	psm3_ips_proto_flow_enqueue(flow, scb);

	if ((flow->transfer == PSM_TRANSFER_PIO) || do_flush)
		err = flow->flush(flow, NULL);

	if (do_flush)
		err = ips_recv_progress_if_busy(proto->ptl, err);

	/* As per the PSM error model (or lack thereof), PSM clients expect to see
	 * only PSM2_OK as a recoverable error */
	if (err == PSM2_EP_NO_RESOURCES || err == PSM2_OK_NO_PROGRESS)
		err = PSM2_OK;
	return err;
}

/*
 * We don't use message striping for middle message protocol,
 * Tests on sandy-bridge two HFIs show lower bandwidth if
 * message striping is used.
 */
ustatic
psm2_error_t
ips_ptl_mq_eager(struct ips_proto *proto, psm2_mq_req_t req,
		 struct ips_flow *flow, psm2_mq_tag_t *tag, const void *ubuf,
		 uint32_t len)
{
	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	psm2_error_t err = PSM2_OK;
	uintptr_t buf = (uintptr_t) ubuf;
	uint32_t nbytes_left, pktlen, offset, chunk_size;
	uint16_t msgseq;
	ips_scb_t *scb;
	uint16_t padding = 0;	// padding for 1st in sequence
	uint32_t frag_size = flow->frag_size;

	psmi_assert(len > 0);
	psmi_assert(req != NULL);

	if (! psmi_hal_has_cap(PSM_HAL_CAP_NON_DW_PKT_SIZE))
		padding = len & 0x3;	// will pad 1st in sequence
	chunk_size = min(proto->ep->chunk_max_segs*frag_size,
				proto->ep->chunk_max_size);
	msgseq = ipsaddr->msgctl->mq_send_seqnum++;

	nbytes_left = len;
	offset = 0;
	do {
		if (padding) {
			psmi_assert(nbytes_left > frag_size); // runt was 1st packet
			/* over reading should be OK on sender because
			 * the padding area is within the whole buffer,
			 * receiver will discard the extra bytes via
			 * next packet overwriting them (but all subsequent
			 * packets in message land at odd boundaries)
			 */
			padding = 4 - padding;
			// when need padding, don't attempt GSO or send DMA
			// and send the runt as the 1st packet
			pktlen = frag_size - padding;
		} else {
			pktlen = min(chunk_size, nbytes_left);
		}

		scb = mq_alloc_pkts(proto, 1, 0, 0);
		psmi_assert(scb != NULL);
		ips_scb_opcode(scb) = OPCODE_EAGER;
		ips_set_LMC_LID_choice(proto, scb, len);
		scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(msgseq);
		ips_scb_copy_tag(scb->ips_lrh.tag, tag->tag);
		scb->ips_lrh.hdr_data.u32w1 = len;
		scb->ips_lrh.hdr_data.u32w0 = offset;	/* initial offset */

		_HFI_VDBG
		    ("payload=%p, thislen=%d, frag_size=%d, nbytes_left=%d\n",
		     (void *)buf, pktlen, frag_size, nbytes_left);
		ips_scb_buffer(scb) = (void *)buf;
#ifdef PSM_HAVE_REG_MR
		if (req->mr) {
			scb->mr = req->mr;
			ips_scb_flags(scb) |= IPS_SEND_FLAG_SEND_MR;
		}
#endif

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (req->is_buf_gpu_mem) {
			// flags will get handled in pio transfer_frame
			// but use cuMemcpy instead of GDRCopy
#ifdef PSM_HAVE_REG_MR
			if (!req->mr)
				ips_scb_flags(scb) |= IPS_SEND_FLAG_PAYLOAD_BUF_GPU;
#else
			ips_scb_flags(scb) |= IPS_SEND_FLAG_PAYLOAD_BUF_GPU;
#endif
			// TBD USER_BUF_GPU only useful for RTS
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
		}
#endif // PSM_CUDA || PSM_ONEAPI

		buf += pktlen;
		offset += pktlen;
		nbytes_left -= pktlen;

		pktlen += padding;
		padding = 0; // rest of packets don't need padding

		scb->frag_size = frag_size;
		scb->nfrag = (pktlen + frag_size - 1) / frag_size;
		scb->chunk_size = pktlen;
		if (scb->nfrag > 1) {
			ips_scb_length(scb) = frag_size;
			scb->nfrag_remaining = scb->nfrag;
			scb->chunk_size_remaining = pktlen;
		} else
			ips_scb_length(scb) = pktlen;

		if (nbytes_left == 0) {	/* last segment/packet */
			ips_scb_cb(scb) = ips_proto_mq_eager_complete;
			ips_scb_cb_param(scb) = req;

			/* Set ACKREQ if single packet per scb. For multi
			 * packets per scb, it is SDMA, driver will set
			 * ACKREQ in last packet, we only need ACK for
			 * last packet.
			 */
			if (scb->nfrag == 1)
				ips_scb_flags(scb) |= IPS_SEND_FLAG_ACKREQ;
		} else {
			req->send_msgoff += pktlen;
		}
		psm3_ips_proto_flow_enqueue(flow, scb);

		if (flow->transfer == PSM_TRANSFER_PIO) {
			/* we need to flush the pio pending queue as quick as possible */
			err = flow->flush(flow, NULL);
		}

	} while (nbytes_left);


	/* Before return, try to make some progress as long as the operation is
	 * not a fast path isend. If this is a fast path isend we cannot call
	 * progress functions since that will cause recursion into recvhdrq_progress
	 * and cause messages to be lost. Instead, for fast path if the operation
	 * was successfully enqueued, but flush returned PSM2_OK_NO_PROGRESS we return
	 * PSM2_OK since the user will progress the queue once the fast path call is
	 * complete.
	*/
	if (err == PSM2_EP_NO_RESOURCES || err == PSM2_OK_NO_PROGRESS) {
		if (likely(!(req->flags_internal & PSMI_REQ_FLAG_FASTPATH))) {
			err = ips_recv_progress_if_busy(proto->ptl, PSM2_EP_NO_RESOURCES);
		} else if (err == PSM2_EP_NO_RESOURCES) {
			err = PSM2_OK;
		}
	}

	return err;
}

static
psm2_error_t
ips_ptl_mq_rndv(struct ips_proto *proto, psm2_mq_req_t req,
		ips_epaddr_t *ipsaddr, const void *buf, uint32_t len)
{
	psmi_assert(proto->msgflowid < EP_FLOW_LAST);
	struct ips_flow *flow = &ipsaddr->flows[proto->msgflowid];
	psm2_error_t err = PSM2_OK;
	ips_scb_t *scb;

	PSM2_LOG_MSG("entering");
	req->req_data.buf = (void *)buf;
	req->req_data.buf_len = len;
	req->req_data.send_msglen = len;
	req->recv_msgoff = 0;
	req->rts_peer = (psm2_epaddr_t) ipsaddr;

	scb = mq_alloc_pkts(proto, 1, 0, 0);
	psmi_assert(scb);
	ips_scb_opcode(scb) = OPCODE_LONG_RTS;
	ips_scb_flags(scb) |= IPS_SEND_FLAG_ACKREQ;
	if (req->type & MQE_TYPE_WAITING)
		ips_scb_flags(scb) |= IPS_SEND_FLAG_BLOCKING;
	scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(ipsaddr->msgctl->mq_send_seqnum);
	ipsaddr->msgctl->mq_send_seqnum++;
	ips_scb_copy_tag(scb->ips_lrh.tag, req->req_data.tag.tag);
	scb->ips_lrh.hdr_data.u32w1 = len;
	scb->ips_lrh.hdr_data.u32w0 = psm3_mpool_get_obj_index(req);

	// small synchronous payload is sent in RTS itself
	// CTS becomes the synchronous ACK
	if (len <= flow->frag_size &&
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	    !req->is_buf_gpu_mem &&
#endif
	    (psmi_hal_has_cap(PSM_HAL_CAP_NON_DW_PKT_SIZE) || !(len & 0x3))) {
		ips_scb_buffer(scb) = (void *)buf;
		scb->chunk_size = ips_scb_length(scb) = len;
		req->send_msgoff = len;
		req->mq->stats.tx_rndv_bytes += len;
	} else {
		ips_scb_length(scb) = 0;
		req->send_msgoff = 0;
	}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	/* Used to indicate to the receiver that the send
	 * is issued on a device buffer. This helps the
	 * receiver select TID instead of using eager buffers.
	 */
	if (req->is_buf_gpu_mem) {
		_HFI_VDBG("Req bufer is on GPU\n");
		ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
		scb->mq_req = req;	/* request comes from GPU domain (device) ... */
	}
	req->gpu_hostbuf_used = 0;
	if ((!(proto->flags & IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND) &&
	   req->is_buf_gpu_mem &&
	    (len > GPUDIRECT_THRESH_RV)) ||
	    ((proto->flags & IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND)  &&
	    req->is_buf_gpu_mem &&
	    (len > gpudirect_rdma_send_limit))) {
		/* send from intermediate host buffer */
		_HFI_VDBG("send from intermediate host buffer\n");
		struct ips_gpu_hostbuf *chb;
		uint32_t offset, window_len;
		int prefetch_lookahead = 0;

		STAILQ_INIT(&req->sendreq_prefetch);
		offset = 0;
		req->gpu_hostbuf_used = 1;
		/* start prefetching */
		req->prefetch_send_msgoff = 0;
		while ((offset < len) &&
		       (prefetch_lookahead < proto->cuda_prefetch_limit)) {
			chb = NULL;
			window_len =
				ips_cuda_next_window(
						     proto->mq->hfi_base_window_rv,
						     offset, len);

			unsigned bufsz;
			if (window_len <= CUDA_SMALLHOSTBUF_SZ) {
				chb = (struct ips_gpu_hostbuf *)
					psm3_mpool_get(
					proto->cuda_hostbuf_pool_small_send);
				bufsz = proto->cuda_hostbuf_small_send_cfg.bufsz;
			}
			if (chb == NULL) {
				chb = (struct ips_gpu_hostbuf *)
					psm3_mpool_get(
					proto->cuda_hostbuf_pool_send);
				bufsz = proto->cuda_hostbuf_send_cfg.bufsz;
			}

			/* any buffers available? */
			if (chb == NULL) {
				break;
			}

			req->prefetch_send_msgoff += window_len;

			chb->offset = offset;
			chb->size = window_len;
			chb->req = req;
			chb->gpu_buf = (uint8_t*)buf + offset;
			chb->bytes_read = 0;

			PSM3_GPU_MEMCPY_DTOH_START(proto, chb, window_len, bufsz);

			STAILQ_INSERT_TAIL(&req->sendreq_prefetch, chb,
					   req_next);
			offset += window_len;
			prefetch_lookahead++;
		}
	}
#endif

	PSM2_LOG_EPM_COND((len > proto->mq->hfi_thresh_rv) &&
			  proto->protoexp,
			  OPCODE_LONG_RTS,PSM2_LOG_TX,proto->ep->epid, req->rts_peer->epid,
			    "scb->ips_lrh.hdr_data.u32w0: %d",scb->ips_lrh.hdr_data.u32w0);
	proto->epaddr_stats.rts_send++;

	_HFI_VDBG("sending with rndv %u\n", len);
	/* If this is a fast path isend, then we cannot poll or
	 * allow progressing of the mq from within the fast path
	 * call otherwise messages will be lost. Therefore given fast path
	 * we will avoid calling poll_internal and not set PSMI_TRUE which would
	 * call ips_recv_progress_if_busy.
	*/
	if ((err = ips_mq_send_envelope(proto, flow, scb, 
					! unlikely(req->flags_internal & PSMI_REQ_FLAG_FASTPATH))))
		goto fail;
#ifdef PSM_HAVE_REG_MR
// TBD - we may want to include odd bytes at start
// and end of message in the RTS itself as opposed to being in last
// EXPTID payload packet's header
// then the RDMA Write can be better aligned and may perform better
	// Start registering memory for anticipated CTS requesting RDMA
	// TBD - we could reduce duation of memory pin by doing this only
	// once we receive CTS, but that will put this call in the critical
	// path.  If done after getting CTS we don't have to predict
	// if remote end will chose RDMA vs LONG DATA approach (eg. if tests of
	// length, etc below)
	//
	// register buffer we will use as source for RDMA Write
	// for PSM_CUDA/PSM_ONEAPI, a group of host bounce buffers may be used above
	// ips_scb_buffer catches when RTS contains the data, in which case no
	// need for memory registration.  While unlkely we also skip
	// registration for zero length sync messages
	// PSM3_RDMA if disabled causes proto->protoexp == NULL
	if (! ips_scb_buffer(scb) && len
			&& len > proto->mq->hfi_thresh_rv
			&& proto->protoexp 	/* expected tid recieve enabled */
			&& ips_epaddr_rdma_connected(ipsaddr)
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			&& len > GPUDIRECT_THRESH_RV
			&& ! req->gpu_hostbuf_used
#endif
		) {
		req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
						 req->req_data.buf, req->req_data.send_msglen, IBV_ACCESS_RDMA
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
						| (req->is_buf_gpu_mem?IBV_ACCESS_IS_GPU_ADDR:0)
#endif
						);
		// if we failed to register memory we will try again when
		// we get the CTS.
	}
#endif

	if_pt (! (req->flags_internal & PSMI_REQ_FLAG_FASTPATH)) {
		/* Assume that we already put a few rndv requests in flight.  This helps
		 * for bibw microbenchmarks and doesn't hurt the 'blocking' case since
		 * we're going to poll anyway */
		psm3_poll_internal(proto->ep, 1, 0);
	}

fail:
	_HFI_VDBG
	    ("[rndv][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x][req=%p/%d]: %s\n",
	     psm3_epaddr_get_name(proto->ep->epid, 0),
	     psm3_epaddr_get_name(req->rts_peer->epid, 1), buf, len,
	     req->req_data.tag.tag[0], req->req_data.tag.tag[1], req->req_data.tag.tag[2], req,
	     psm3_mpool_get_obj_index(req), psm3_error_get_string(err));
	PSM2_LOG_MSG("leaving");
	return err;
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
static inline
int psm3_is_needed_rendezvous(struct ips_proto *proto, uint32_t len)
{
	if (
		len > cuda_thresh_rndv){
		return 1;
	}

	return 0;
}
#endif //PSM_CUDA || PSM_ONEAPI


psm2_error_t
psm3_ips_proto_mq_isend(psm2_mq_t mq, psm2_epaddr_t mepaddr, uint32_t flags_user,
		   uint32_t flags_internal, psm2_mq_tag_t *tag, const void *ubuf,
		   uint32_t len, void *context, psm2_mq_req_t *req_o)
{
	psm2_error_t err = PSM2_OK;
	struct ips_proto *proto;
	struct ips_flow *flow;
	ips_epaddr_t *ipsaddr;
	ips_scb_t *scb;
	psm2_mq_req_t req;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	int gpu_mem = 0;
#endif // PSM_CUDA || PSM_ONEAPI

	req = psm3_mq_req_alloc(mq, MQE_TYPE_SEND);
	if_pf(req == NULL)
		return PSM2_NO_MEMORY;

	_HFI_VDBG("(req=%p) ubuf=%p len=%u, flags_user=0x%x\n", req, ubuf, len, flags_user);

	req->flags_user = flags_user;
	req->flags_internal = flags_internal;
	if (len >= mepaddr->proto->multirail_thresh_load_balance) {
		ipsaddr = ((ips_epaddr_t *) mepaddr)->msgctl->ipsaddr_next;
		ipsaddr->msgctl->ipsaddr_next = ipsaddr->next;
	} else {
		ipsaddr = (ips_epaddr_t *)mepaddr;
	}
	psmi_assert(ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED);

	proto = ((psm2_epaddr_t) ipsaddr)->proto;

	psmi_assert(proto->msgflowid < EP_FLOW_LAST);
	req->req_data.send_msglen = len;
	req->req_data.tag = *tag;
	req->req_data.context = context;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	req->is_buf_gpu_mem = PSM3_IS_BUFFER_GPU_MEM(ubuf, len);
	req->gpu_hostbuf_used = 0;
	if (req->is_buf_gpu_mem) {
		gpu_mem = 1;
		PSM3_MARK_BUF_SYNCHRONOUS(ubuf);
		if (psm3_is_needed_rendezvous(proto, len))
			goto do_rendezvous;
	}
#endif
	flow = &ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO];

	if (flags_user & PSM2_MQ_FLAG_SENDSYNC) {
		goto do_rendezvous;
	} else if (len <= mq->hfi_thresh_tiny) {
		scb = mq_alloc_tiny(proto);
		psmi_assert(scb);
		ips_scb_opcode(scb) = OPCODE_TINY;
		ips_set_LMC_LID_choice(proto, scb, len);
		scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(
		    ((len & HFI_KHDR_TINYLEN_MASK) << HFI_KHDR_TINYLEN_SHIFT) |
		    ipsaddr->msgctl->mq_send_seqnum);
		ipsaddr->msgctl->mq_send_seqnum++;
		ips_scb_copy_tag(scb->ips_lrh.tag, tag->tag);

		const void *user_buffer = ubuf;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (!req->is_buf_gpu_mem) {
			mq_copy_tiny_host_mem((uint32_t *) &scb->ips_lrh.hdr_data,
							  (uint32_t *) user_buffer, len);
			proto->strat_stats.tiny_cpu_isend++;
			proto->strat_stats.tiny_cpu_isend_bytes += len;
		} else {
			// TBD USER_BUF_GPU only useful for RTS
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
			/* The following functions PINS the GPU pages
			 * and mmaps the pages into the process virtual
			 * space. This allows PSM to issue a standard
			 * memcpy to move data between HFI resources
			 * and the GPU
			 */
			if (len <= gdr_copy_limit_send &&
				NULL != (user_buffer = psmi_hal_gdr_convert_gpu_to_host_addr(
						(unsigned long)ubuf, len, 0, proto->ep))) {
				mq_copy_tiny_host_mem((uint32_t *) &scb->ips_lrh.hdr_data,
							  (uint32_t *) user_buffer, len);
#ifdef PSM_ONEAPI
				psmi_hal_gdr_munmap_gpu_to_host_addr(
						(unsigned long)ubuf, len, 0,
						proto->ep);
#endif
				proto->strat_stats.tiny_gdrcopy_isend++;
				proto->strat_stats.tiny_gdrcopy_isend_bytes += len;
			} else {
				user_buffer = ubuf;
				mq_copy_tiny((uint32_t *) &scb->ips_lrh.hdr_data,
						 (uint32_t *) user_buffer, len);
				proto->strat_stats.tiny_cuCopy_isend++;
				proto->strat_stats.tiny_cuCopy_isend_bytes += len;
			}
		}
#else
		mq_copy_tiny((uint32_t *) &scb->ips_lrh.hdr_data,
			 (uint32_t *) user_buffer, len);
		proto->strat_stats.tiny_cpu_isend++;
		proto->strat_stats.tiny_cpu_isend_bytes += len;
#endif

		/* If this is a fast path isend, then we cannot allow
		 * progressing of the mq from within the fast path
		 * call otherwise messages will be lost. Therefore given fast path
		 * we will set PSMI_FALSE which will prevent the call to
		 * ips_recv_progress_if_busy.
		*/
		err = ips_mq_send_envelope(proto, flow, scb, !(flags_internal & PSMI_REQ_FLAG_FASTPATH));
		if (err != PSM2_OK)
			return err;

		/* We can mark this op complete since all the data is now copied
		 * into an SCB that remains live until it is remotely acked */
		req->state = MQ_STATE_COMPLETE;
		mq_qq_append(&mq->completed_q, req);
		_HFI_VDBG
		    ("[itiny][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x][req=%p]\n",
		     psm3_epaddr_get_name(mq->ep->epid, 0),
		     psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 1), ubuf,
		     len, tag->tag[0], tag->tag[1], tag->tag[2], req);
	} else if (len <= flow->frag_size) {
		uint32_t paylen = len & ~0x3;

		scb = mq_alloc_pkts(proto, 1, 0, 0);
		psmi_assert(scb);
		ips_scb_opcode(scb) = OPCODE_SHORT;
		ips_set_LMC_LID_choice(proto, scb, len);
		scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(ipsaddr->msgctl->mq_send_seqnum);
		ipsaddr->msgctl->mq_send_seqnum++;
		scb->ips_lrh.hdr_data.u32w1 = len;
		ips_scb_copy_tag(scb->ips_lrh.tag, tag->tag);
		const void * user_buffer = ubuf;
#ifdef PSM_HAVE_REG_MR
		int used_send_dma = 0;
#endif
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (req->is_buf_gpu_mem) {
			// TBD USER_BUF_GPU only useful for RTS
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
			if (len <= gdr_copy_limit_send &&
				NULL != (user_buffer = psmi_hal_gdr_convert_gpu_to_host_addr(
					(unsigned long)ubuf, len , 0, proto->ep))) {
				/* init req so ips_proto_mq_eager_complete can unmap */
				req->req_data.buf = (uint8_t*)ubuf;
				req->req_data.buf_len = len;
				req->req_data.send_msglen = len;
#ifdef PSM_ONEAPI
				scb->gdr_addr = (unsigned long)ubuf;
				scb->gdr_size = len;
				ips_scb_flags(scb) |= IPS_SEND_FLAG_USE_GDRCOPY;
#endif
				proto->strat_stats.short_gdrcopy_isend++;
				proto->strat_stats.short_gdrcopy_isend_bytes += len;
			} else {
				user_buffer = ubuf;
#ifdef PSM_HAVE_REG_MR
				if (len > proto->iovec_gpu_thresh_eager) {
					scb->mr = req->mr = psm3_verbs_reg_mr(
							proto->mr_cache, 0,
							(void*)user_buffer, len,
							IBV_ACCESS_IS_GPU_ADDR);
				}
				if (req->mr) {
					ips_scb_flags(scb) |= IPS_SEND_FLAG_SEND_MR;
					req->rts_peer = (psm2_epaddr_t) ipsaddr;
					ips_scb_cb(scb) = ips_proto_mq_eager_complete;
					ips_scb_cb_param(scb) = req;
					used_send_dma = 1;
					proto->strat_stats.short_gdr_isend++;
					proto->strat_stats.short_gdr_isend_bytes += len;
				} else
#endif // PSM_HAVE_REG_MR
				{
					ips_scb_flags(scb) |= IPS_SEND_FLAG_PAYLOAD_BUF_GPU;
					// TBD for OPA flow_type could be DMA
					proto->strat_stats.short_cuCopy_isend++;
					proto->strat_stats.short_cuCopy_isend_bytes += len;
				}
			}
		} else
#endif // PSM_CUDA || PSM_ONEAPI
		{
#ifdef PSM_HAVE_REG_MR
			if (len > proto->iovec_thresh_eager) {
				scb->mr = req->mr = psm3_verbs_reg_mr(
						proto->mr_cache, 0,
						(void*)user_buffer, len, 0);
			}
			if (req->mr) {
				ips_scb_flags(scb) |= IPS_SEND_FLAG_SEND_MR;
				req->rts_peer = (psm2_epaddr_t) ipsaddr;
				ips_scb_cb(scb) = ips_proto_mq_eager_complete;
				ips_scb_cb_param(scb) = req;
				used_send_dma = 1;
				proto->strat_stats.short_dma_cpu_isend++;
				proto->strat_stats.short_dma_cpu_isend_bytes += len;
			} else
#endif
			{
				// TBD for OPA flow_type could be DMA
				proto->strat_stats.short_copy_cpu_isend++;
				proto->strat_stats.short_copy_cpu_isend_bytes += len;
			}
		}

		ips_scb_buffer(scb) = (void *)user_buffer;

		scb->chunk_size = ips_scb_length(scb) = paylen;
		if (len > paylen) {
			/* there are nonDW bytes, copy to header */
			mq_copy_tiny
				((uint32_t *)&scb->ips_lrh.hdr_data.u32w0,
				(uint32_t *)((uintptr_t)ubuf + paylen),
				len - paylen);

			/* for complete callback */
			req->send_msgoff = len - paylen;
		} else {
			req->send_msgoff = 0;
		}

		/*
		 * Need ack for send side completion because we
		 * send from user buffer.  ACK will trigger scb callback
		 */
		ips_scb_flags(scb) |= IPS_SEND_FLAG_ACKREQ;

		/* If this is a fast path isend, then we cannot allow
		 * progressing of the mq from within the fast path
		 * call otherwise messages will be lost. Therefore given fast path
		 * we will set PSMI_FALSE which will prevent the call to
		 * ips_recv_progress_if_busy.
		*/
		err = ips_mq_send_envelope(proto, flow, scb, !(flags_internal & PSMI_REQ_FLAG_FASTPATH));
		if (err != PSM2_OK)
			return err;

		/*
		 * It should be OK to check the buffer address in
		 * 'scb' to be changed, when this scb is done, the
		 * address is set to NULL when scb is put back to
		 * scb pool. Even if the same scb is re-used, it
		 * is not possible to set to this 'buf' address
		 * because the app has not yet had a chance to start
		 * another IO.  TBD - possible odd scenario if app
		 * had this IO started in middle of a buffer which it also
		 * had a multi-packet eager IO working on, then could see
		 * same user_buffer from two IOs here.
		 */
#ifdef PSM_HAVE_REG_MR
		if (used_send_dma) {
			// noop, callback already setup
		} else
#endif
		// TBD - could avoid this if/else code by always marking
		// callback above, but may be less efficient for msgrate
		if (ips_scb_buffer(scb) == (void *)user_buffer) {
			/* continue to send from user buffer */
			ips_scb_cb(scb) = ips_proto_mq_eager_complete;
			ips_scb_cb_param(scb) = req;
		} else {
			/* mark the message done */
			req->state = MQ_STATE_COMPLETE;
			mq_qq_append(&mq->completed_q, req);
		}
		_HFI_VDBG
		    ("[ishrt][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x][req=%p]\n",
		     psm3_epaddr_get_name(mq->ep->epid, 0),
		     psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 1), ubuf,
		     len, tag->tag[0], tag->tag[1], tag->tag[2], req);
	} else if (len <= mq->hfi_thresh_rv) {
		req->send_msgoff = 0;
		req->rts_peer = (psm2_epaddr_t) ipsaddr;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (req->is_buf_gpu_mem) {
#ifdef PSM_HAVE_REG_MR
			// TBD - no upper bound for send DMA here
			// non-priority MR and will fallback if can't register
			if (len > proto->iovec_gpu_thresh_eager) {
				req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
                                        (void*)ubuf, len, IBV_ACCESS_IS_GPU_ADDR);
			}
			if (req->mr) {
				proto->strat_stats.eager_gdr_isend++;
				proto->strat_stats.eager_gdr_isend_bytes += len;
			} else
#endif
			{
				proto->strat_stats.eager_cuCopy_isend++;
				proto->strat_stats.eager_cuCopy_isend_bytes += len;
			}
		} else
#endif // PSM_CUDA || PSM_ONEAPI
		{
#ifdef PSM_HAVE_REG_MR
			// TBD - no upper bound for send DMA here
			// non-priority MR and will fallback if can't register
			if (len > proto->iovec_thresh_eager) {
				req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
                                        	(void*)ubuf, len, 0);
			}
			if (req->mr) {
				proto->strat_stats.eager_dma_cpu_isend++;
				proto->strat_stats.eager_dma_cpu_isend_bytes += len;
			} else
#endif
			{
				// TBD for OPA flow_type could be DMA
				proto->strat_stats.eager_copy_cpu_isend++;
				proto->strat_stats.eager_copy_cpu_isend_bytes += len;
			}
		}
		err = ips_ptl_mq_eager(proto, req, flow, tag, ubuf, len);
		if (err != PSM2_OK)
			return err;

		_HFI_VDBG
		    ("[ilong][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x][req=%p]\n",
		     psm3_epaddr_get_name(mq->ep->epid, 0),
		     psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 1), ubuf,
		     len, tag->tag[0], tag->tag[1], tag->tag[2], req);
	} else {		/* skip eager accounting below */
do_rendezvous:
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (gpu_mem) {
			proto->strat_stats.rndv_gpu_isend++;
			proto->strat_stats.rndv_gpu_isend_bytes += len;
		} else {
			proto->strat_stats.rndv_cpu_isend++;
			proto->strat_stats.rndv_cpu_isend_bytes += len;
		}
#else
		proto->strat_stats.rndv_cpu_isend++;
		proto->strat_stats.rndv_cpu_isend_bytes += len;
#endif

		mq->stats.tx_num++;
		mq->stats.tx_rndv_num++;
		// we count tx_rndv_bytes as we get CTS

		err = ips_ptl_mq_rndv(proto, req, ipsaddr, ubuf, len);
		*req_o = req;
		return err;
	}

	*req_o = req;
	mq->stats.tx_num++;
	mq->stats.tx_eager_num++;
	mq->stats.tx_eager_bytes += len;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (gpu_mem) {
		mq->stats.tx_eager_gpu_num++;
		mq->stats.tx_eager_gpu_bytes += len;
	} else {
		mq->stats.tx_eager_cpu_num++;
		mq->stats.tx_eager_cpu_bytes += len;
	}
#endif

	return err;
}

psm2_error_t
psm3_ips_proto_mq_send(psm2_mq_t mq, psm2_epaddr_t mepaddr, uint32_t flags,
		  psm2_mq_tag_t *tag, const void *ubuf, uint32_t len)
{
	psm2_error_t err = PSM2_OK;
	struct ips_proto *proto;
	struct ips_flow *flow;
	ips_epaddr_t *ipsaddr;
	ips_scb_t *scb;

#if   defined(PSM_CUDA) || defined (PSM_ONEAPI)
	int gpu_mem = 0;
#endif

	_HFI_VDBG("ubuf=%p len=%u flags=0x%x\n", ubuf, len, flags);

	if (len >= mepaddr->proto->multirail_thresh_load_balance) {
		ipsaddr = ((ips_epaddr_t *) mepaddr)->msgctl->ipsaddr_next;
		ipsaddr->msgctl->ipsaddr_next = ipsaddr->next;
	} else {
		ipsaddr = (ips_epaddr_t *)mepaddr;
	}
	psmi_assert(ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED);

	proto = ((psm2_epaddr_t) ipsaddr)->proto;

	psmi_assert(proto->msgflowid < EP_FLOW_LAST);

#if defined(PSM_CUDA) || defined (PSM_ONEAPI)
	gpu_mem = PSM3_IS_BUFFER_GPU_MEM(ubuf, len);
	if (gpu_mem) {
		PSM3_MARK_BUF_SYNCHRONOUS(ubuf);
		if (psm3_is_needed_rendezvous(proto, len))
			goto do_rendezvous;
	}
#endif
	flow = &ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO];

	if (flags & PSM2_MQ_FLAG_SENDSYNC) {
		goto do_rendezvous;
	} else if (len <= mq->hfi_thresh_tiny) {
		scb = mq_alloc_tiny(proto);
		psmi_assert(scb);
		ips_scb_opcode(scb) = OPCODE_TINY;
		ips_set_LMC_LID_choice(proto, scb, len);
		scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(
		    ((len & HFI_KHDR_TINYLEN_MASK) << HFI_KHDR_TINYLEN_SHIFT) |
		    ipsaddr->msgctl->mq_send_seqnum);
		ipsaddr->msgctl->mq_send_seqnum++;
		ips_scb_copy_tag(scb->ips_lrh.tag, tag->tag);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		const void *user_buffer = ubuf;
		if (!gpu_mem) {
			mq_copy_tiny_host_mem((uint32_t *) &scb->ips_lrh.hdr_data,
							  (uint32_t *) user_buffer, len);
			proto->strat_stats.tiny_cpu_send++;
			proto->strat_stats.tiny_cpu_send_bytes += len;
		} else {
			// TBD USER_BUF_GPU only useful for RTS
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
			/* The following functions PINS the GPU pages
			 * and mmaps the pages into the process virtual
			 * space. This allows PSM to issue a standard
			 * memcpy to move data between HFI resources
			 * and the GPU
			 */
			if (len <= gdr_copy_limit_send &&
				NULL != (user_buffer = psmi_hal_gdr_convert_gpu_to_host_addr(
						(unsigned long)ubuf, len, 0, proto->ep))) {
				mq_copy_tiny_host_mem((uint32_t *) &scb->ips_lrh.hdr_data,
							  (uint32_t *) user_buffer, len);
#ifdef PSM_ONEAPI
				psmi_hal_gdr_munmap_gpu_to_host_addr(
					(unsigned long)ubuf, len, 0, proto->ep);
#endif
				proto->strat_stats.tiny_gdrcopy_send++;
				proto->strat_stats.tiny_gdrcopy_send_bytes += len;
			} else {
				user_buffer = ubuf;
				mq_copy_tiny(
					(uint32_t *) &scb->ips_lrh.hdr_data,
					     (uint32_t *) ubuf, len);
				proto->strat_stats.tiny_cuCopy_send++;
				proto->strat_stats.tiny_cuCopy_send_bytes += len;
			}
		}
#else
		mq_copy_tiny((uint32_t *) &scb->ips_lrh.hdr_data, (uint32_t *) ubuf, len);
		proto->strat_stats.tiny_cpu_send++;
		proto->strat_stats.tiny_cpu_send_bytes += len;
#endif
		err = ips_mq_send_envelope(proto, flow, scb, PSMI_TRUE);
		if (err != PSM2_OK)
			return err;

		_HFI_VDBG("[tiny][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x]\n",
			  psm3_epaddr_get_name(mq->ep->epid, 0),
			  psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 1),
			  ubuf, len, tag->tag[0], tag->tag[1], tag->tag[2]);
	} else if (len <= flow->frag_size) {
		uint32_t paylen = len & ~0x3;

		scb = mq_alloc_pkts(proto, 1, 0, 0);
		psmi_assert(scb);
		ips_scb_opcode(scb) = OPCODE_SHORT;
		ips_set_LMC_LID_choice(proto, scb, len);
		scb->ips_lrh.khdr.kdeth0 = __cpu_to_le32(ipsaddr->msgctl->mq_send_seqnum);
		ipsaddr->msgctl->mq_send_seqnum++;
		scb->ips_lrh.hdr_data.u32w1 = len;
		ips_scb_copy_tag(scb->ips_lrh.tag, tag->tag);

		const void * user_buffer = ubuf;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		int converted = 0;
		if (gpu_mem) {
			// TBD USER_BUF_GPU only useful for RTS
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
			/* will use PIO */
			if (len <= gdr_copy_limit_send &&
				NULL != (user_buffer = psmi_hal_gdr_convert_gpu_to_host_addr(
						(unsigned long)ubuf, len, 0, proto->ep))) {
				converted = 1;
#ifdef PSM_ONEAPI
				scb->gdr_addr = (unsigned long)ubuf;
				scb->gdr_size = len;
				ips_scb_flags(scb) |= IPS_SEND_FLAG_USE_GDRCOPY;
#endif
				proto->strat_stats.short_gdrcopy_send++;
				proto->strat_stats.short_gdrcopy_send_bytes += len;
			} else {
				user_buffer = ubuf;
#ifdef PSM_HAVE_REG_MR
				if (len > proto->iovec_gpu_thresh_eager_blocking) {
					scb->mr = psm3_verbs_reg_mr(
						proto->mr_cache, 0,
						(void*)user_buffer, len, IBV_ACCESS_IS_GPU_ADDR);
				} else
					scb->mr = NULL;
				if (scb->mr) {
					ips_scb_flags(scb) |= IPS_SEND_FLAG_SEND_MR;
					ips_scb_cb(scb) = ips_proto_scb_mr_complete;
					ips_scb_cb_param(scb) = scb;
					proto->strat_stats.short_gdr_send++;
					proto->strat_stats.short_gdr_send_bytes += len;
				} else
#endif // PSM_HAVE_REG_MR
				{
					ips_scb_flags(scb) |= IPS_SEND_FLAG_PAYLOAD_BUF_GPU;
					// TBD for OPA flow_type could be DMA
					proto->strat_stats.short_cuCopy_send++;
					proto->strat_stats.short_cuCopy_send_bytes += len;
				}
			}
		} else
#endif // PSM_CUDA || PSM_ONEAPI
		{
#ifdef PSM_HAVE_REG_MR
			if (len > proto->iovec_thresh_eager_blocking) {
				scb->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
						(void*)user_buffer, len, 0);
			} else
				scb->mr = NULL;
			if (scb->mr) {
				ips_scb_flags(scb) |= IPS_SEND_FLAG_SEND_MR;
				ips_scb_cb(scb) = ips_proto_scb_mr_complete;
				ips_scb_cb_param(scb) = scb;
				proto->strat_stats.short_dma_cpu_send++;
				proto->strat_stats.short_dma_cpu_send_bytes += len;
			} else
#endif
			{
				// TBD for OPA flow_type could be DMA
				proto->strat_stats.short_copy_cpu_send++;
				proto->strat_stats.short_copy_cpu_send_bytes += len;
			}
		}

		ips_scb_buffer(scb) = (void *)user_buffer;
		scb->chunk_size = ips_scb_length(scb) = paylen;
		if (len > paylen) {
			/* there are nonDW bytes, copy to header */
			mq_copy_tiny
				((uint32_t *)&scb->ips_lrh.hdr_data.u32w0,
				(uint32_t *)((uintptr_t)ubuf + paylen),
				len - paylen);
		}

		/*
		 * Need ack for send side completion because we
		 * send from user buffer. ACK will trigger scb callback
		 */
		ips_scb_flags(scb) |= IPS_SEND_FLAG_ACKREQ;
		err = ips_mq_send_envelope(proto, flow, scb, PSMI_TRUE);
		if (err != PSM2_OK)
			return err;

		/*
		 * It should be OK to check the buffer address in
		 * 'scb' to be changed, when this scb is done, the
		 * address is set to NULL when scb is put back to
		 * scb pool. Even if the same scb is re-used, it
		 * is not possible to set to this 'ubuf' address
		 * because the app has not yet had a chance to start
		 * another IO.  TBD - possible odd scenario if app
		 * had this IO started in middle of a buffer which it also
		 * had a multi-packet eager IO working on, then could see
		 * same user_buffer from two IOs here.
		 */
		if (ips_scb_buffer(scb) == (void *)user_buffer) {
#if   defined(PSM_HAVE_REG_MR)
			if ((ips_scb_flags(scb) & IPS_SEND_FLAG_SEND_MR) ||
#else
			if (
#endif
			    paylen > proto->scb_bufsize ||
			    !psm3_ips_scbctrl_bufalloc(scb)) {
				/* sdma transfer (can't change user buffer),
				 * or, payload is larger than bounce buffer,
				 * or, can't allocate bounce buffer,
				 * send from user buffer till complete */
				PSMI_BLOCKUNTIL(mq->ep, err,
					ips_scb_buffer(scb) != (void*)user_buffer);
				if (err > PSM2_OK_NO_PROGRESS)
					return err;
				err = PSM2_OK;
			} else {
				psmi_assert(flow->transfer == PSM_TRANSFER_PIO);
				/* PIO and now have a bounce buffer */
				/* copy to bounce buffer */
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				if (!gpu_mem || converted) {
					// host address
					ips_shortcpy_host_mem
						(ips_scb_buffer(scb),
					 	(void*)user_buffer, paylen);
				} else {
					// cuda address - undo flags so PIO
					// doesn't cuMemcpy too
					ips_scb_flags(scb) &= ~IPS_SEND_FLAG_PAYLOAD_BUF_GPU;
					// TBD - could call cuMemcpy directly
					ips_shortcpy(ips_scb_buffer(scb),
					 	(void*)user_buffer, paylen);
				}
#else
				ips_shortcpy(ips_scb_buffer(scb),
					 (void*)user_buffer, paylen);
#endif
			}
		}
		_HFI_VDBG("[shrt][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x]\n",
			  psm3_epaddr_get_name(mq->ep->epid, 0),
			  psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 1),
			  ubuf, len, tag->tag[0], tag->tag[1], tag->tag[2]);

	} else if (len <= mq->hfi_thresh_rv) {
		psm2_mq_req_t req;

		/* Block until we can get a req */
		PSMI_BLOCKUNTIL(mq->ep, err,
				(req =
				 psm3_mq_req_alloc(mq, MQE_TYPE_SEND)));
		if (err > PSM2_OK_NO_PROGRESS)
			return err;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		req->gpu_hostbuf_used = 0;
		if (gpu_mem) {
			req->is_buf_gpu_mem = 1;
#ifdef PSM_HAVE_REG_MR
			// TBD - no upper bound for send DMA here
			// non-priority MR and will fallback if can't register
			if (len > proto->iovec_gpu_thresh_eager_blocking) {
				req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
                                        (void*)ubuf, len, IBV_ACCESS_IS_GPU_ADDR);
			}
			if (req->mr) {
				proto->strat_stats.eager_gdr_send++;
				proto->strat_stats.eager_gdr_send_bytes += len;
			} else
#endif
			{
				proto->strat_stats.eager_cuCopy_send++;
				proto->strat_stats.eager_cuCopy_send_bytes += len;
			}
		} else {
			req->is_buf_gpu_mem = 0;
#else
		{
#endif // PSM_CUDA || PSM_ONEAPI
#ifdef PSM_HAVE_REG_MR
			// TBD - no upper bound for send DMA here
			// non-priority MR and will fallback if can't register
			if (len > proto->iovec_thresh_eager_blocking) {
				req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
                                        	(void*)ubuf, len, 0);
			}
			if (req->mr) {
				proto->strat_stats.eager_dma_cpu_send++;
				proto->strat_stats.eager_dma_cpu_send_bytes += len;
			} else
#endif
			{
				// TBD for OPA flow_type could be DMA
				proto->strat_stats.eager_copy_cpu_send++;
				proto->strat_stats.eager_copy_cpu_send_bytes += len;
			}
		}
		req->type |= MQE_TYPE_WAITING;
		req->req_data.send_msglen = len;
		req->req_data.tag = *tag;
		req->send_msgoff = 0;
		req->flags_user = flags;
		req->flags_internal |= PSMI_REQ_FLAG_IS_INTERNAL;
		req->rts_peer = (psm2_epaddr_t) ipsaddr;

		err = ips_ptl_mq_eager(proto, req, flow, tag, ubuf, len);
		if (err != PSM2_OK)
			return err;

		psm3_mq_wait_internal(&req);

		_HFI_VDBG("[long][%s->%s][b=%p][m=%d][t=%08x.%08x.%08x]\n",
			  psm3_epaddr_get_name(mq->ep->epid, 0),
			  psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 1),
			  ubuf, len, tag->tag[0], tag->tag[1], tag->tag[2]);
	} else {
		psm2_mq_req_t req;
do_rendezvous:
		/* Block until we can get a req */
		PSMI_BLOCKUNTIL(mq->ep, err,
				(req = psm3_mq_req_alloc(mq, MQE_TYPE_SEND)));
		if (err > PSM2_OK_NO_PROGRESS)
			return err;

		req->type |= MQE_TYPE_WAITING;
		req->req_data.tag = *tag;
		req->flags_user = flags;
		req->flags_internal |= PSMI_REQ_FLAG_IS_INTERNAL;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (gpu_mem) {
			req->is_buf_gpu_mem = 1;
			proto->strat_stats.rndv_gpu_send++;
			proto->strat_stats.rndv_gpu_send_bytes += len;
		} else {
			req->is_buf_gpu_mem = 0;
			proto->strat_stats.rndv_cpu_send++;
			proto->strat_stats.rndv_cpu_send_bytes += len;
		}
#else
		proto->strat_stats.rndv_cpu_send++;
		proto->strat_stats.rndv_cpu_send_bytes += len;
#endif

		mq->stats.tx_num++;
		mq->stats.tx_rndv_num++;
		// we count tx_rndv_bytes as we get CTS

		err = ips_ptl_mq_rndv(proto, req, ipsaddr, ubuf, len);
		if (err != PSM2_OK)
			return err;
		psm3_mq_wait_internal(&req);
		return err;	/* skip accounting, done separately at completion time */
	}

	mq->stats.tx_num++;
	mq->stats.tx_eager_num++;
	mq->stats.tx_eager_bytes += len;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (gpu_mem) {
		mq->stats.tx_eager_gpu_num++;
		mq->stats.tx_eager_gpu_bytes += len;
	} else {
		mq->stats.tx_eager_cpu_num++;
		mq->stats.tx_eager_cpu_bytes += len;
	}
#endif

	return err;
}

static
psm2_error_t
ips_proto_mq_rts_match_callback(psm2_mq_req_t req, int was_posted)
{
	psm2_epaddr_t epaddr = req->rts_peer;
	struct ips_proto *proto = epaddr->proto;

	/* We have a match.
	 * We may already set with first packet,
	 * If we're doing eager-based r-v, just send back the sreq and length and
	 * have the sender complete the send.
	 */
	PSM2_LOG_MSG("entering");
	_HFI_MMDBG("rts_match_callback\n");
	// while matching RTS we set both recv and send msglen to min of the two
	psmi_assert(req->req_data.recv_msglen == req->req_data.send_msglen);
	req->mq->stats.rx_user_num++;
	req->mq->stats.rx_user_bytes += req->req_data.recv_msglen;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	/* Cases where we do not use TIDs:
	 * 0) Received full message as payload to RTS, CTS is just an ack
	 * 1) Recv on a host buffer, Send on a gpu buffer and len is <= 3 bytes
	 * 2) Recv on a host buffer, Send on a host buffer and len <= hfi_thresh_rv
	 * 3) Recv on gpu buf and len is <= 3 bytes
	 * 4) Expected protocol not initialized.
	 */
	if (req->recv_msgoff >= req->req_data.recv_msglen
	    || (!req->is_buf_gpu_mem && ((req->is_sendbuf_gpu_mem &&
	     req->req_data.recv_msglen <= GPUDIRECT_THRESH_RV)||
	    (!req->is_sendbuf_gpu_mem &&
	     req->req_data.recv_msglen <= proto->mq->hfi_thresh_rv))) ||
	    (req->is_buf_gpu_mem && req->req_data.recv_msglen <= GPUDIRECT_THRESH_RV) ||
	    proto->protoexp == NULL	/* no expected tid recieve */
#ifdef PSM_HAVE_REG_MR
		|| ! ips_epaddr_rdma_connected((ips_epaddr_t *) epaddr)
#endif
		) {
#else // PSM_CUDA || PSM_ONEAPI
	if (req->recv_msgoff >= req->req_data.recv_msglen ||
	    proto->protoexp == NULL	/* no expected tid recieve */
#ifdef PSM_HAVE_REG_MR
		|| ! ips_epaddr_rdma_connected((ips_epaddr_t *) epaddr)
#endif
	    || req->req_data.recv_msglen <= proto->mq->hfi_thresh_rv /* less rv theshold */
		) {  /* no expected tid recieve */
#endif // PSM_CUDA || PSM_ONEAPI
#ifdef PSM_HAVE_REG_MR
//do_long_data:
#endif
		// send CTS asking for use of LONG_DATA send of large message

		/* there is no order requirement, try to push CTS request
		 * directly, if fails, then queue it for later try. */
		_HFI_VDBG("pushing CTS recv off %u len %u"
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			" rGPU %u sGPU %u"
#endif
			" rv thresh %u"
#ifdef PSM_HAVE_REG_MR
			" conn %u"
#endif
			" epaddr %p RDMA %u\n",
			req->recv_msgoff, req->req_data.recv_msglen,
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			req->is_buf_gpu_mem, req->is_sendbuf_gpu_mem,
#endif
			proto->mq->hfi_thresh_rv,
#ifdef PSM_HAVE_REG_MR
			proto->protoexp?ips_epaddr_rdma_connected((ips_epaddr_t *) epaddr):0,
#endif
			epaddr, proto->protoexp != NULL);

		if (req->recv_msgoff < req->req_data.recv_msglen) {
			// RTS did not have the message as payload
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			if (req->is_buf_gpu_mem) {
				proto->strat_stats.rndv_long_gpu_recv++;
				proto->strat_stats.rndv_long_gpu_recv_bytes += req->req_data.recv_msglen;
			} else {
#endif
				proto->strat_stats.rndv_long_cpu_recv++;
				proto->strat_stats.rndv_long_cpu_recv_bytes += req->req_data.recv_msglen;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			}
#endif
		}
		if (psm3_ips_proto_mq_push_cts_req(proto, req) != PSM2_OK) {
			struct ips_pend_sends *pends = &proto->pend_sends;
			struct ips_pend_sreq *sreq =
			    psm3_mpool_get(proto->pend_sends_pool);
			psmi_assert(sreq != NULL);
			if (sreq == NULL)
			{
				PSM2_LOG_MSG("leaving");
				return PSM2_NO_MEMORY;
			}
			sreq->type = IPS_PENDSEND_EAGER_REQ;
			sreq->req = req;

			STAILQ_INSERT_TAIL(&pends->pendq, sreq, next);
			psmi_timer_request(proto->timerq, &pends->timer,
					   PSMI_TIMER_PRIO_1);
		}
	} else {
		// send CTS asking for use of TID send of large message
		// register buffer we will use as destination for remote RDMA Write
		// We choose not to register memory when recv is posted since
		// that could pin memory for a long time waiting for a tag match
		// and recv buffers could be much larger than the messages they tag
		// match with, resulting in unnecessary MR registration.
		// req->req_data.buf is app buffer
		// req->req_data.buf_len is app buffer length
		// req->req_data.send_msglen is agreed amount to transfer (<= buf_len)
		// TBD - if we were tight on MR resources, this could tie up more
		// resources than needed, in which case skipping this and registering
		// per CTS below could be better
		// TBD - it might help MR cache hit rate if we registered the whole
		// receive buffer (req->req_data.buf_len), this way large receive
		// buffers which match smaller messages can get MR cache hit for
		// various sized messages which may arrive in the buffer
#ifdef PSM_HAVE_REG_MR
		// TBD is this assert valid for OPA also?  Should be since
		// with pick LONG DATA above if recv_msgoff >= recv_msglen
		// and send_msglen should == recv_msglen
		psmi_assert(req->req_data.send_msglen);	// 0 len uses LONG_DATA above
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		// for GPU receive buffer we need to sort things out at a lower level
		// since may use a host bounce buffer for RDMA and need to register it
		if (! req->is_buf_gpu_mem) {
#else
		{
#endif
			req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
					req->req_data.buf, req->req_data.send_msglen,
					IBV_ACCESS_RDMA|IBV_ACCESS_REMOTE_WRITE);
			if (! req->mr) {
				// psm3_ips_protoexp_tid_get_from_token will try to get MR again
				// and will retry via ips_tid_pendtids_timer_callback.  So we
				// can just fall through with req->mr == NULL.
				// The alternative would be to goto and force use of LONG_DATA
				//goto do_long_data;
			} else {
				_HFI_MMDBG("rbuf registered: addr %p len %d rkey 0x%x\n",  req->req_data.buf, req->req_data.send_msglen, req->mr->rkey);
			}
		}
#endif // PSM_HAVE_REG_MR
		_HFI_VDBG("matched rts, trying TID\n");
		psm3_ips_protoexp_tid_get_from_token(proto->protoexp, req->req_data.buf,
						req->req_data.recv_msglen, epaddr,
						req->rts_reqidx_peer,
						req->type & MQE_TYPE_WAITING_PEER ?
						IPS_PROTOEXP_TIDGET_PEERWAIT :
						0, ips_proto_mq_rv_complete,
						req);
	}

	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}

psm2_error_t
psm3_ips_proto_mq_push_cts_req(struct ips_proto *proto, psm2_mq_req_t req)
{
	ips_epaddr_t *ipsaddr = (ips_epaddr_t *) (req->rts_peer);
	struct ips_flow *flow;
	ips_scb_t *scb;
	ptl_arg_t *args;

	PSM2_LOG_MSG("entering");
	psmi_assert(proto->msgflowid < EP_FLOW_LAST);
	flow = &ipsaddr->flows[proto->msgflowid];
	scb = psm3_ips_scbctrl_alloc(&proto->scbc_egr, 1, 0, 0);
	if (scb == NULL)
	{
		PSM2_LOG_MSG("leaving");
		return PSM2_OK_NO_PROGRESS;
	}
	args = (ptl_arg_t *) scb->ips_lrh.data;

	ips_scb_opcode(scb) = OPCODE_LONG_CTS;
	scb->ips_lrh.khdr.kdeth0 = 0;
	args[0].u32w0 = psm3_mpool_get_obj_index(req);
	args[1].u32w1 = req->req_data.recv_msglen;
	args[1].u32w0 = req->rts_reqidx_peer;

	PSM2_LOG_EPM(OPCODE_LONG_CTS,PSM2_LOG_TX, proto->ep->epid,
		    flow->ipsaddr->epaddr.epid ,"req->rts_reqidx_peer: %d",
		    req->rts_reqidx_peer);
	proto->epaddr_stats.cts_long_data_send++;

	psm3_ips_proto_flow_enqueue(flow, scb);
	flow->flush(flow, NULL);

	/* have already received enough bytes */
	if (req->recv_msgoff == req->req_data.recv_msglen) {
		ips_proto_mq_rv_complete(req);
	}

	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}

// rendezvous using LONG DATA "eager push" instead of TID
// If we run out of resources (scbs), this is called again to continue
psm2_error_t
psm3_ips_proto_mq_push_rts_data(struct ips_proto *proto, psm2_mq_req_t req)
{
	psm2_error_t err = PSM2_OK;
	uintptr_t buf = (uintptr_t) req->req_data.buf + req->recv_msgoff;
	ips_epaddr_t *ipsaddr = (ips_epaddr_t *) (req->rts_peer);
	uint32_t nbytes_left = req->req_data.send_msglen - req->recv_msgoff;
	uint32_t nbytes_sent = 0;
	uint32_t nbytes_this, chunk_size;
	uint32_t frag_size, unaligned_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	int converted = 0;
#endif
	struct ips_flow *flow;
	ips_scb_t *scb;
	int dostats = !req->recv_msgoff; // if continuing, don't update stats

	psmi_assert(nbytes_left > 0);

	PSM2_LOG_MSG("entering.");
	{
		/* use PIO transfer */
		flow = &ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO];
		frag_size = flow->frag_size;
		chunk_size = min(proto->ep->chunk_max_segs*frag_size,
					 proto->ep->chunk_max_size);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (req->is_buf_gpu_mem) {
#ifdef PSM_HAVE_REG_MR
			// rare, but when RV connection not available, we
			// can select LONG DATA for a GPU send buffer.  Normally
			// won't happen for GPU send >3 unless RDMA disabled
			// or RV not connected
			// TBD - no upper bound for send DMA here
			// non-priority MR and will fallback if can't register
			if (!req->mr && req->req_data.send_msglen > proto->iovec_gpu_thresh_eager) {
				req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
					req->req_data.buf, req->req_data.send_msglen, 
					IBV_ACCESS_IS_GPU_ADDR);
			}
			if (req->mr) {
				proto->strat_stats.rndv_long_gdr_send += dostats;
				proto->strat_stats.rndv_long_gdr_send_bytes += dostats*req->req_data.send_msglen;
			} else
#endif
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				// for GPU send buffer <= 3, receiver can select
				// LONG DATA and we can use GDRCopy
				// must repin per attempt
			if (req->req_data.send_msglen <= gdr_copy_limit_send &&
				0 != (buf =  (uintptr_t)psmi_hal_gdr_convert_gpu_to_host_addr(
					(unsigned long)req->req_data.buf,
					req->req_data.send_msglen, 0, proto->ep))) {
				converted = 1;
				proto->strat_stats.rndv_long_gdrcopy_send += dostats;
				proto->strat_stats.rndv_long_gdrcopy_send_bytes += dostats*req->req_data.send_msglen;
			} else {
				buf = (uintptr_t) req->req_data.buf + req->recv_msgoff;
#else
			{
#endif
				proto->strat_stats.rndv_long_cuCopy_send += dostats;
				proto->strat_stats.rndv_long_cuCopy_send_bytes += dostats*req->req_data.send_msglen;
			}
		} else {
#endif
#ifdef PSM_HAVE_REG_MR
			// TBD - no upper bound for send DMA here
			// non-priority MR and will fallback if can't register
			if (!req->mr && req->req_data.send_msglen > proto->iovec_thresh_eager) {
				req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
					req->req_data.buf,
					req->req_data.send_msglen, 0);
			}
			if (req->mr) {
				proto->strat_stats.rndv_long_dma_cpu_send += dostats;
				proto->strat_stats.rndv_long_dma_cpu_send_bytes += dostats*req->req_data.send_msglen;
			} else
#endif
			{
				proto->strat_stats.rndv_long_copy_cpu_send += dostats;
				proto->strat_stats.rndv_long_copy_cpu_send_bytes += dostats*req->req_data.send_msglen;
			}
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		}
#endif
	}

	do {
		/*
		 * don't try to call progression routine such as:
		 * ips_recv_progress_if_busy() in this loop,
		 * it will cause recursive call of this function.
		 */

		/*
		 * When tid code path is enabled, we don’t allocate scbc_rv
		 * objects. If the message is less than the hfi_thresh_rv,
		 * we normally use eager protocol to do the transfer.
		 * However, if it is sync send, we use the rendezvous
		 * rts/cts/rts-data protocol.
		 * In this case, because scbc_rv is null,
		 * we use scbc_egr instead.
		 */

		scb = psm3_ips_scbctrl_alloc(proto->scbc_rv ? proto->scbc_rv
					: &proto->scbc_egr, 1, 0, 0);
		if (scb == NULL) {
			err = PSM2_OK_NO_PROGRESS;
			break;
		}
		ips_scb_opcode(scb) = OPCODE_LONG_DATA;
		scb->ips_lrh.khdr.kdeth0 = 0;
		scb->ips_lrh.data[0].u32w0 = req->rts_reqidx_peer;
		scb->ips_lrh.data[1].u32w1 = req->req_data.send_msglen;

		/* attached unaligned bytes into packet header */
		unaligned_bytes = nbytes_left & 0x3;
		if (unaligned_bytes) {
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			if (!req->is_buf_gpu_mem
			    || converted
			    )
				mq_copy_tiny_host_mem((uint32_t *)&scb->ips_lrh.mdata,
					(uint32_t *)buf, unaligned_bytes);
			else
#endif
			mq_copy_tiny((uint32_t *)&scb->ips_lrh.mdata,
				(uint32_t *)buf, unaligned_bytes);

			/* position to send */
			buf += unaligned_bytes;
			req->recv_msgoff += unaligned_bytes;
			psmi_assert(req->recv_msgoff < 4);

			/* for complete callback */
			req->send_msgoff += unaligned_bytes;

			nbytes_left -= unaligned_bytes;
			nbytes_sent += unaligned_bytes;
		}
		scb->ips_lrh.data[1].u32w0 = req->recv_msgoff;
		ips_scb_buffer(scb) = (void *)buf;
#ifdef PSM_HAVE_REG_MR
		if (req->mr) {
			scb->mr = req->mr;
			ips_scb_flags(scb) |= IPS_SEND_FLAG_SEND_MR;
		}
#endif
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		// SDMA identifies GPU buffers itself. But PIO path needs flags
		if (req->is_buf_gpu_mem
		) {
#ifdef PSM_HAVE_REG_MR
			if (!req->mr && !converted)
#else
			if (!converted)
#endif
				ips_scb_flags(scb) |= IPS_SEND_FLAG_PAYLOAD_BUF_GPU;
			// TBD USER_BUF_GPU only useful for RTS
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USER_BUF_GPU;
		}
#ifdef PSM_ONEAPI
		if (converted) {
			scb->gdr_addr = (unsigned long)req->req_data.buf;
			scb->gdr_size = req->req_data.send_msglen;
			ips_scb_flags(scb) |= IPS_SEND_FLAG_USE_GDRCOPY;
		}
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */

		scb->frag_size = frag_size;
		nbytes_this = min(chunk_size, nbytes_left);
		if (nbytes_this > 0)
			scb->nfrag = (nbytes_this + frag_size - 1) / frag_size;
		else
			scb->nfrag = 1;
		scb->chunk_size = nbytes_this;

		if (scb->nfrag > 1) {
			ips_scb_length(scb) = frag_size;
			scb->nfrag_remaining = scb->nfrag;
			scb->chunk_size_remaining = nbytes_this;
		} else
			ips_scb_length(scb) = nbytes_this;

		buf += nbytes_this;
		req->recv_msgoff += nbytes_this;
		nbytes_sent += nbytes_this;
		nbytes_left -= nbytes_this;
		if (nbytes_left == 0) {
			/* because of scb callback, use eager complete */
			ips_scb_cb(scb) = ips_proto_mq_eager_complete;
			ips_scb_cb_param(scb) = req;

			/* Set ACKREQ if single packet per scb. For multi
			 * packets per scb, it is SDMA, driver will set
			 * ACKREQ in last packet, we only need ACK for
			 * last packet.
			 */
			if (scb->nfrag == 1)
				ips_scb_flags(scb) |= IPS_SEND_FLAG_ACKREQ;
		} else {
			req->send_msgoff += nbytes_this;
		}
		psm3_ips_proto_flow_enqueue(flow, scb);
		if (flow->transfer == PSM_TRANSFER_PIO) {
			/* we need to flush the pio pending queue as quick as possible */
			flow->flush(flow, NULL);
		}

	} while (nbytes_left);


	PSM2_LOG_MSG("leaving.");

	return err;
}

// received a CTS
int
psm3_ips_proto_mq_handle_cts(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	struct ips_proto *proto = rcv_ev->proto;
	psm2_mq_t mq = proto->ep->mq;
	struct ips_flow *flow;
	psm2_mq_req_t req;
	uint32_t paylen;

	/*
	 * if PSN does not match, drop the packet.
	 */
	PSM2_LOG_MSG("entering");
	if (!ips_proto_is_expected_or_nak((struct ips_recvhdrq_event *)rcv_ev))
	{
		PSM2_LOG_MSG("leaving");
		return IPS_RECVHDRQ_CONTINUE;
	}
	req = psm3_mpool_find_obj_by_index(mq->sreq_pool, p_hdr->data[1].u32w0);
	psmi_assert(req != NULL);

	/*
	 * if there is payload, it is expected tid protocol
	 * with tid session info as the payload.
	 */
	paylen = ips_recvhdrq_event_paylen(rcv_ev);
	if (paylen > 0) {
		// we will use TID RDMA
		ips_tid_session_list *payload =
			ips_recvhdrq_event_payload(rcv_ev);
		psmi_assert(paylen == 0 || payload);
		PSM2_LOG_EPM(OPCODE_LONG_CTS,PSM2_LOG_RX,rcv_ev->ipsaddr->epaddr.epid,
			    mq->ep->epid,"p_hdr->data[1].u32w0 %d",
			    p_hdr->data[1].u32w0);
		proto->epaddr_stats.cts_rdma_recv++;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		psmi_assert(p_hdr->data[1].u32w1 > min(cuda_thresh_rndv, mq->hfi_thresh_rv));	// msglen
#else
		psmi_assert(p_hdr->data[1].u32w1 > mq->hfi_thresh_rv);	// msglen
#endif
		psmi_assert(proto->protoexp != NULL);

		/* ptl_req_ptr will be set to each tidsendc */
		if (req->ptl_req_ptr == NULL) {
			req->req_data.send_msglen = p_hdr->data[1].u32w1;
			req->mq->stats.tx_rndv_bytes += req->req_data.send_msglen;
		}
		psmi_assert(req->req_data.send_msglen == p_hdr->data[1].u32w1);

#ifdef PSM_HAVE_REG_MR
		if (! req->mr
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			&& ! req->gpu_hostbuf_used
#endif
			) {
			// we predicted use of LONG DATA and remote side chose RDMA
			// or we failed to register memory previously.
			req->mr = psm3_verbs_reg_mr(proto->mr_cache, 0,
							req->req_data.buf, req->req_data.send_msglen, IBV_ACCESS_RDMA
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
								| (req->is_buf_gpu_mem?IBV_ACCESS_IS_GPU_ADDR:0)
#endif
							);
			// if we still don't have an MR, we will try again later
		}
#endif // PSM_HAVE_REG_MR
		_HFI_MMDBG("ips_proto_mq_handle_cts for TID CTS\n");
		if (psm3_ips_tid_send_handle_tidreq(proto->protoexp,
					       rcv_ev->ipsaddr, req, p_hdr->data[0],
					       p_hdr->mdata, payload, paylen) == 0) {
			proto->psmi_logevent_tid_send_reqs.next_warning = 0;
		} else {
			flow = &rcv_ev->ipsaddr->flows[ips_proto_flowid(p_hdr)];
			flow->recv_seq_num.psn_num -= 1;                            /* Decrement seq number to NAK proper CTS */
			ips_proto_send_nak((struct ips_recvhdrq *)rcv_ev->recvq, flow);
			static unsigned int msg_cnt = 0;
			if (msg_cnt++ == 0) {                                       /* Report the message only once */
				_HFI_INFO("PSM3 memory shortage detected. Please consider modifying PSM3_MEMORY setting\n");
			}
			return PSM2_EP_NO_RESOURCES;
		}
	} else {
		// we will use LONG DATA push
		PSM2_LOG_EPM(OPCODE_LONG_CTS,PSM2_LOG_RX,rcv_ev->ipsaddr->epaddr.epid,
			    mq->ep->epid, "long data");
		proto->epaddr_stats.cts_long_data_recv++;
		req->rts_reqidx_peer = p_hdr->data[0].u32w0; /* eager receive only */
		req->req_data.send_msglen = p_hdr->data[1].u32w1;

		if (req->send_msgoff >= req->req_data.send_msglen) {
// TBD - should cleanup from pin as needed
			/* already sent enough bytes, may truncate so using >= */
			/* RTS payload is only used for CPU memory */
			proto->strat_stats.rndv_rts_copy_cpu_send++;
			proto->strat_stats.rndv_rts_copy_cpu_send_bytes += req->req_data.send_msglen;
			ips_proto_mq_rv_complete(req);
		} else {
			req->mq->stats.tx_rndv_bytes += (req->req_data.send_msglen - req->send_msgoff);
#ifdef PSM_HAVE_REG_MR
#ifdef PSM_HAVE_RNDV_MOD
			// If we have an MR due to incorrect prediction of RDMA
			// release it if can't be used for send DMA or don't
			// want send DMA.  push_rts_data will attempt to use
			// for send DMA if req->mr != NULL.
			if (req->mr &&
				(!psm3_verbs_user_space_mr(req->mr)
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				|| (req->is_buf_gpu_mem && req->req_data.send_msglen <= proto->iovec_gpu_thresh_eager)
				|| (!req->is_buf_gpu_mem && req->req_data.send_msglen <= proto->iovec_thresh_eager)
#else
				|| (req->req_data.send_msglen <= proto->iovec_thresh_eager)
#endif
				)) {

				_HFI_MMDBG("Using LONG_DATA, releasing RV RDMA MR: %p rkey: 0x%x\n", req->mr, req->mr->rkey);
				psm3_verbs_release_mr(req->mr);
				req->mr = NULL;
				ips_tid_mravail_callback(req->rts_peer->proto);
			}
#endif
#endif

			if (psm3_ips_proto_mq_push_rts_data(proto, req) != PSM2_OK) {
				/* there is no order requirement, tried to push RTS data
				 * directly and not done, so queue it for later try. */
				struct ips_pend_sreq *sreq =
					psm3_mpool_get(proto->pend_sends_pool);
				psmi_assert_always(sreq != NULL);

				sreq->type = IPS_PENDSEND_EAGER_DATA;
				sreq->req = req;
				STAILQ_INSERT_TAIL(&proto->pend_sends.pendq, sreq, next);
				/* Make sure it's processed by timer */
				psmi_timer_request(proto->timerq, &proto->pend_sends.timer,
						   PSMI_TIMER_PRIO_1);
			}
		}
	}

	flow = &rcv_ev->ipsaddr->flows[ips_proto_flowid(p_hdr)];
	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);

	PSM2_LOG_MSG("leaving");
	return IPS_RECVHDRQ_CONTINUE;
}

// received an RTS
int
psm3_ips_proto_mq_handle_rts(struct ips_recvhdrq_event *rcv_ev)
{
	int ret = IPS_RECVHDRQ_CONTINUE;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_flow *flow = &ipsaddr->flows[ips_proto_flowid(p_hdr)];
	psm2_mq_t mq = rcv_ev->proto->mq;
	ips_msgctl_t *msgctl = ipsaddr->msgctl;
	enum ips_msg_order msgorder;
	char *payload;
	uint32_t paylen;
	psm2_mq_req_t req;

	/*
	 * if PSN does not match, drop the packet.
	 */
	PSM2_LOG_MSG("entering");
	_HFI_MMDBG("got rts\n");
	if (!ips_proto_is_expected_or_nak((struct ips_recvhdrq_event *)rcv_ev))
	{
		PSM2_LOG_MSG("leaving");
		return IPS_RECVHDRQ_CONTINUE;
	}

	msgorder = ips_proto_check_msg_order(ipsaddr, flow,
		__le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK,
		&ipsaddr->msgctl->mq_recv_seqnum);
	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE))
	{
		PSM2_LOG_MSG("leaving");
		return IPS_RECVHDRQ_REVISIT;
	}

	payload = ips_recvhdrq_event_payload(rcv_ev);
	paylen = ips_recvhdrq_event_paylen(rcv_ev);
	/* either no payload or whole message */
	psmi_assert(paylen == 0 || paylen >= p_hdr->data[1].u32w1);

	/*
	 * We can't have past message sequence here. For eager message,
	 * it must always have an eager queue matching because even in
	 * truncation case the code logic will wait till all packets
	 * have been received.
	 */
	psmi_assert(msgorder != IPS_MSG_ORDER_PAST);

	_HFI_VDBG("tag=%llx reqidx_peer=%d, msglen=%d\n",
		  (long long)p_hdr->data[0].u64,
		  p_hdr->data[1].u32w0, p_hdr->data[1].u32w1);

	int rc = psm3_mq_handle_rts(mq,
				    (psm2_epaddr_t) &ipsaddr->msgctl->
				    master_epaddr,
				    p_hdr->tag,
				    &rcv_ev->proto->strat_stats,
				    p_hdr->data[1].u32w1, payload, paylen,
				    msgorder, ips_proto_mq_rts_match_callback,
				    &req);
	if (unlikely(rc == MQ_RET_UNEXP_NO_RESOURCES)) {
		// as a performance optimization, the 1st time we process an
		// unmatched RTS, we ask to REVISIT it next poll loop hoping for
		// a match due to a slightly late MPI_recv call
		uint32_t psn_mask = ((psm2_epaddr_t)ipsaddr)->proto->psn_mask;

		flow->recv_seq_num.psn_num =
			(flow->recv_seq_num.psn_num - 1) & psn_mask;
		ipsaddr->msgctl->mq_recv_seqnum--;

		PSM2_LOG_MSG("leaving");
		return IPS_RECVHDRQ_REVISIT;
	}

	rcv_ev->proto->epaddr_stats.rts_recv++;

	req->rts_peer = (psm2_epaddr_t) ipsaddr;
	req->rts_reqidx_peer = p_hdr->data[1].u32w0;
	if (req->req_data.send_msglen > mq->hfi_thresh_rv)
	{
		PSM2_LOG_EPM(OPCODE_LONG_RTS,PSM2_LOG_RX,req->rts_peer->epid,mq->ep->epid,
			    "req->rts_reqidx_peer: %d",req->rts_reqidx_peer);
	}
	if (p_hdr->flags & IPS_SEND_FLAG_BLOCKING)
		req->type |= MQE_TYPE_WAITING_PEER;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (p_hdr->flags & IPS_SEND_FLAG_USER_BUF_GPU)
		req->is_sendbuf_gpu_mem = 1;
	else
		req->is_sendbuf_gpu_mem = 0;
#endif

	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE_RECV)) {
		/* for out of order matching only */
		req->msg_seqnum =
		    __le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK;
		req->ptl_req_ptr = (void *)msgctl;

		msgctl->outoforder_count++;
		mq_qq_append(&mq->outoforder_q, req);

		ret = IPS_RECVHDRQ_BREAK;
	} else {
		ipsaddr->msg_toggle = 0;
		if (rc == MQ_RET_MATCH_OK)
			ips_proto_mq_rts_match_callback(req, 1);
		/* XXX if blocking, break out of progress loop */

		if (msgctl->outoforder_count)
			psm3_ips_proto_mq_handle_outoforder_queue(mq, msgctl);

		if (rc == MQ_RET_UNEXP_OK)
			ret = IPS_RECVHDRQ_BREAK;
	}

	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);

	PSM2_LOG_MSG("leaving");
	return ret;
}

int
psm3_ips_proto_mq_handle_tiny(struct ips_recvhdrq_event *rcv_ev)
{
	int ret = IPS_RECVHDRQ_CONTINUE;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_flow *flow = &ipsaddr->flows[ips_proto_flowid(p_hdr)];
	psm2_mq_t mq = rcv_ev->proto->mq;
	ips_msgctl_t *msgctl = ipsaddr->msgctl;
	enum ips_msg_order msgorder;
	char *payload;
	uint32_t paylen;
	psm2_mq_req_t req;

	/*
	 * if PSN does not match, drop the packet.
	 */
	if (!ips_proto_is_expected_or_nak((struct ips_recvhdrq_event *)rcv_ev))
		return IPS_RECVHDRQ_CONTINUE;

	msgorder = ips_proto_check_msg_order(ipsaddr, flow,
		__le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK,
		&ipsaddr->msgctl->mq_recv_seqnum);
	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE))
		return IPS_RECVHDRQ_REVISIT;

	payload = (void *)&p_hdr->hdr_data;
	paylen = (__le32_to_cpu(p_hdr->khdr.kdeth0) >>
		  HFI_KHDR_TINYLEN_SHIFT) & HFI_KHDR_TINYLEN_MASK;

	/*
	 * We can't have past message sequence here. For eager message,
	 * it must always have an eager queue matching because even in
	 * truncation case the code logic will wait till all packets
	 * have been received.
	 */
	psmi_assert(msgorder != IPS_MSG_ORDER_PAST);

	_HFI_VDBG("tag=%08x.%08x.%08x opcode=%x, msglen=%d\n",
		  p_hdr->tag[0], p_hdr->tag[1], p_hdr->tag[2],
		  OPCODE_TINY, paylen);

	/* store in req below too! */
	int rc = psm3_mq_handle_envelope(mq,
				(psm2_epaddr_t) &ipsaddr->msgctl->master_epaddr,
				p_hdr->tag,
				&rcv_ev->proto->strat_stats,  paylen, 0,
				payload, paylen, msgorder, OPCODE_TINY, &req);
	if (unlikely(rc == MQ_RET_UNEXP_NO_RESOURCES)) {
		uint32_t psn_mask = ((psm2_epaddr_t)ipsaddr)->proto->psn_mask;

		flow->recv_seq_num.psn_num =
			(flow->recv_seq_num.psn_num - 1) & psn_mask;
		ipsaddr->msgctl->mq_recv_seqnum--;

		return IPS_RECVHDRQ_REVISIT;
	}

	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE_RECV)) {
		/* for out of order matching only */
		req->msg_seqnum =
		    __le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK;
		req->ptl_req_ptr = (void *)msgctl;

		msgctl->outoforder_count++;
		mq_qq_append(&mq->outoforder_q, req);

		ret = IPS_RECVHDRQ_BREAK;
	} else {
		ipsaddr->msg_toggle = 0;

		if (msgctl->outoforder_count)
			psm3_ips_proto_mq_handle_outoforder_queue(mq, msgctl);

		if (rc == MQ_RET_UNEXP_OK)
			ret = IPS_RECVHDRQ_BREAK;
	}
	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);
	return ret;
}

int
psm3_ips_proto_mq_handle_short(struct ips_recvhdrq_event *rcv_ev)
{
	int ret = IPS_RECVHDRQ_CONTINUE;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_flow *flow = &ipsaddr->flows[ips_proto_flowid(p_hdr)];
	psm2_mq_t mq = rcv_ev->proto->mq;
	ips_msgctl_t *msgctl = ipsaddr->msgctl;
	enum ips_msg_order msgorder;
	char *payload;
	uint32_t paylen;
	psm2_mq_req_t req;

	/*
	 * if PSN does not match, drop the packet.
	 */
	if (!ips_proto_is_expected_or_nak((struct ips_recvhdrq_event *)rcv_ev))
		return IPS_RECVHDRQ_CONTINUE;

	msgorder = ips_proto_check_msg_order(ipsaddr, flow,
		__le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK,
		&ipsaddr->msgctl->mq_recv_seqnum);
	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE))
		return IPS_RECVHDRQ_REVISIT;

	payload = ips_recvhdrq_event_payload(rcv_ev);
	paylen = ips_recvhdrq_event_paylen(rcv_ev);
	psmi_assert(paylen == 0 || payload);

	/*
	 * We can't have past message sequence here. For eager message,
	 * it must always have an eager queue matching because even in
	 * truncation case the code logic will wait till all packets
	 * have been received.
	 */
	psmi_assert(msgorder != IPS_MSG_ORDER_PAST);

	_HFI_VDBG("tag=%08x.%08x.%08x opcode=%x, msglen=%d\n",
		  p_hdr->tag[0], p_hdr->tag[1], p_hdr->tag[2],
		  OPCODE_SHORT, p_hdr->hdr_data.u32w1);

	/* store in req below too! */
	int rc = psm3_mq_handle_envelope(mq,
				(psm2_epaddr_t) &ipsaddr->msgctl->master_epaddr,
				p_hdr->tag,
				&rcv_ev->proto->strat_stats,
				p_hdr->hdr_data.u32w1, p_hdr->hdr_data.u32w0,
				payload, paylen, msgorder, OPCODE_SHORT, &req);
	if (unlikely(rc == MQ_RET_UNEXP_NO_RESOURCES)) {
		uint32_t psn_mask = ((psm2_epaddr_t)ipsaddr)->proto->psn_mask;

		flow->recv_seq_num.psn_num =
			(flow->recv_seq_num.psn_num - 1) & psn_mask;
		ipsaddr->msgctl->mq_recv_seqnum--;

		return IPS_RECVHDRQ_REVISIT;
	}

	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE_RECV)) {
		/* for out of order matching only */
		req->msg_seqnum =
		    __le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK;
		req->ptl_req_ptr = (void *)msgctl;

		msgctl->outoforder_count++;
		mq_qq_append(&mq->outoforder_q, req);

		ret = IPS_RECVHDRQ_BREAK;
	} else {
		ipsaddr->msg_toggle = 0;

		if (msgctl->outoforder_count)
			psm3_ips_proto_mq_handle_outoforder_queue(mq, msgctl);

		if (rc == MQ_RET_UNEXP_OK)
			ret = IPS_RECVHDRQ_BREAK;
	}

	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);
	return ret;
}

int
psm3_ips_proto_mq_handle_eager(struct ips_recvhdrq_event *rcv_ev)
{
	int ret = IPS_RECVHDRQ_CONTINUE;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_flow *flow = &ipsaddr->flows[ips_proto_flowid(p_hdr)];
	psm2_mq_t mq = rcv_ev->proto->mq;
	ips_msgctl_t *msgctl = ipsaddr->msgctl;
	enum ips_msg_order msgorder;
	char *payload;
	uint32_t paylen;
	psm2_mq_req_t req;

	/*
	 * if PSN does not match, drop the packet.
	 */
	if (!ips_proto_is_expected_or_nak((struct ips_recvhdrq_event *)rcv_ev))
		return IPS_RECVHDRQ_CONTINUE;

	msgorder = ips_proto_check_msg_order(ipsaddr, flow,
		__le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK,
		&ipsaddr->msgctl->mq_recv_seqnum);
	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE))
		return IPS_RECVHDRQ_REVISIT;

	payload = ips_recvhdrq_event_payload(rcv_ev);
	paylen = ips_recvhdrq_event_paylen(rcv_ev);
	psmi_assert(paylen == 0 || payload);

	if (msgorder == IPS_MSG_ORDER_PAST ||
			msgorder == IPS_MSG_ORDER_FUTURE_RECV) {
		req = mq_eager_match(mq, msgctl,
		    __le32_to_cpu(p_hdr->khdr.kdeth0)&HFI_KHDR_MSGSEQ_MASK);
		/*
		 * It is future message sequence or past message sequence,
		 * and there is request matching in eager queue, we handle
		 * the packet data and return. We can't go continue to
		 * match envelope.
		 * Past message sequence must always have a matching!!!
		 * error is caught below.
		 */
		if (req) {
			//u32w0 is offset - only cnt recv msgs on 1st pkt in msg
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			int use_gdrcopy = 0;
			if (!req->is_buf_gpu_mem) {
				if (req->state == MQ_STATE_UNEXP) {
					if (p_hdr->data[1].u32w0<4) rcv_ev->proto->strat_stats.eager_sysbuf_recv++;
					rcv_ev->proto->strat_stats.eager_sysbuf_recv_bytes += paylen;
				} else {
					if (p_hdr->data[1].u32w0<4) rcv_ev->proto->strat_stats.eager_cpu_recv++;
					rcv_ev->proto->strat_stats.eager_cpu_recv_bytes += paylen;
				}
			} else if (PSMI_USE_GDR_COPY_RECV(paylen)) {
				use_gdrcopy = 1;
				if (p_hdr->data[1].u32w0<4) rcv_ev->proto->strat_stats.eager_gdrcopy_recv++;
				rcv_ev->proto->strat_stats.eager_gdrcopy_recv_bytes += paylen;
			} else {
				if (p_hdr->data[1].u32w0<4) rcv_ev->proto->strat_stats.eager_cuCopy_recv++;
				rcv_ev->proto->strat_stats.eager_cuCopy_recv_bytes += paylen;
			}
			psm3_mq_handle_data(mq, req,
				p_hdr->data[1].u32w0, payload, paylen,
				use_gdrcopy, rcv_ev->proto->ep);
#else
			if (req->state == MQ_STATE_UNEXP) {
				if (p_hdr->data[1].u32w0<4) rcv_ev->proto->strat_stats.eager_sysbuf_recv++;
				rcv_ev->proto->strat_stats.eager_sysbuf_recv_bytes += paylen;
			} else {
				if (p_hdr->data[1].u32w0<4) rcv_ev->proto->strat_stats.eager_cpu_recv++;
				rcv_ev->proto->strat_stats.eager_cpu_recv_bytes += paylen;
			}
			psm3_mq_handle_data(mq, req,
				p_hdr->data[1].u32w0, payload, paylen);
#endif // PSM_CUDA || PSM_ONEAPI

			if (msgorder == IPS_MSG_ORDER_FUTURE_RECV)
				ret = IPS_RECVHDRQ_BREAK;
			if ((__be32_to_cpu(p_hdr->bth[2]) &
			    IPS_SEND_FLAG_ACKREQ) ||
			    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
				ips_proto_send_ack((struct ips_recvhdrq *)
					rcv_ev->recvq, flow);

			psm3_ips_proto_process_ack(rcv_ev);
			return ret;
		}

		psmi_assert(msgorder == IPS_MSG_ORDER_FUTURE_RECV);
		/*
		 * For future message sequence, since there is no eager
		 * queue matching yet, this must be the first packet for
		 * the message sequence. And of course, expected message
		 * sequence is always the first packet for the sequence.
		 */
	}

	/*
	 * We can't have past message sequence here. For eager message,
	 * it must always have an eager queue matching because even in
	 * truncation case the code logic will wait till all packets
	 * have been received.
	 */
	psmi_assert(msgorder != IPS_MSG_ORDER_PAST);

	_HFI_VDBG("tag=%08x.%08x.%08x opcode=%x, msglen=%d\n",
		p_hdr->tag[0], p_hdr->tag[1], p_hdr->tag[2],
		OPCODE_EAGER, p_hdr->hdr_data.u32w1);

	/* store in req below too! */
	int rc = psm3_mq_handle_envelope(mq,
				(psm2_epaddr_t) &ipsaddr->msgctl->master_epaddr,
				p_hdr->tag,
				&rcv_ev->proto->strat_stats,
				p_hdr->hdr_data.u32w1, p_hdr->hdr_data.u32w0,
				payload, paylen, msgorder, OPCODE_EAGER, &req);
	if (unlikely(rc == MQ_RET_UNEXP_NO_RESOURCES)) {
		uint32_t psn_mask = ((psm2_epaddr_t)ipsaddr)->proto->psn_mask;

		flow->recv_seq_num.psn_num =
			(flow->recv_seq_num.psn_num - 1) & psn_mask;
		ipsaddr->msgctl->mq_recv_seqnum--;

		return IPS_RECVHDRQ_REVISIT;
	}

	/* for both outoforder matching and eager matching */
	req->msg_seqnum =
		    __le32_to_cpu(p_hdr->khdr.kdeth0) & HFI_KHDR_MSGSEQ_MASK;
	req->ptl_req_ptr = (void *)msgctl;

	if (unlikely(msgorder == IPS_MSG_ORDER_FUTURE_RECV)) {
		msgctl->outoforder_count++;
		mq_qq_append(&mq->outoforder_q, req);

		ret = IPS_RECVHDRQ_BREAK;
	} else {
		ipsaddr->msg_toggle = 0;

		if (msgctl->outoforder_count)
			psm3_ips_proto_mq_handle_outoforder_queue(mq, msgctl);

		if (rc == MQ_RET_UNEXP_OK)
			ret = IPS_RECVHDRQ_BREAK;
	}
	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);
	return ret;
}

/*
 * Progress the out of order queue to see if any message matches
 * current receiving sequence number.
 */
void
psm3_ips_proto_mq_handle_outoforder_queue(psm2_mq_t mq, ips_msgctl_t *msgctl)
{
	psm2_mq_req_t req;

	do {
		req =
		    mq_ooo_match(&mq->outoforder_q, msgctl,
				 msgctl->mq_recv_seqnum);
		if (req == NULL)
			return;

		msgctl->outoforder_count--;
		msgctl->mq_recv_seqnum++;

		psm3_mq_handle_outoforder(mq, req);

	} while (msgctl->outoforder_count > 0);

	return;
}

// LONG_DATA packet handler
int
psm3_ips_proto_mq_handle_data(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	psm2_mq_t mq = rcv_ev->proto->mq;
	char *payload;
	uint32_t paylen;
	psm2_mq_req_t req;
	struct ips_flow *flow;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	int use_gdrcopy = 0;
	struct ips_proto *proto = rcv_ev->proto;
#ifdef PSM_ONEAPI
	int converted = 0;
#endif
#endif // PSM_CUDA || PSM_ONEAPI
	psmi_copy_tiny_fn_t psmi_copy_tiny_fn = mq_copy_tiny;


	/*
	 * if PSN does not match, drop the packet.
	 */
	if (!ips_proto_is_expected_or_nak((struct ips_recvhdrq_event *)rcv_ev))
		return IPS_RECVHDRQ_CONTINUE;

	req = psm3_mpool_find_obj_by_index(mq->rreq_pool, p_hdr->data[0].u32w0);
	psmi_assert(req != NULL);
	// while matching RTS we set both recv and send msglen to min of the two
	psmi_assert(req->req_data.recv_msglen == req->req_data.send_msglen);
	psmi_assert(p_hdr->data[1].u32w1 == req->req_data.send_msglen);

	payload = ips_recvhdrq_event_payload(rcv_ev);
	paylen = ips_recvhdrq_event_paylen(rcv_ev);
	psmi_assert(paylen == 0 || payload);

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	// cpu stats already tracked when sent CTS
	if (req->is_buf_gpu_mem) {
		req->req_data.buf = req->user_gpu_buffer;
		// 1st packet with any unaligned data we handle here
		if (p_hdr->data[1].u32w0 < 4) {
			void *buf;
			if (PSMI_USE_GDR_COPY_RECV(paylen + p_hdr->data[1].u32w0) &&
				NULL != (buf = psmi_hal_gdr_convert_gpu_to_host_addr(
						(unsigned long)req->user_gpu_buffer,
						paylen + p_hdr->data[1].u32w0, 1, proto->ep))) {
				req->req_data.buf = buf;
				psmi_copy_tiny_fn = mq_copy_tiny_host_mem;
#ifdef PSM_ONEAPI
				converted = 1;
#endif
				proto->strat_stats.rndv_long_gdr_recv++;
				proto->strat_stats.rndv_long_gdr_recv_bytes += paylen;
			} else {
				proto->strat_stats.rndv_long_cuCopy_recv++;
				proto->strat_stats.rndv_long_cuCopy_recv_bytes += paylen;
			}
		} else if (PSMI_USE_GDR_COPY_RECV(paylen)) {
			// let mq_handle_data do the conversion
			use_gdrcopy = 1;
			//proto->strat_stats.rndv_long_gdr_recv++;
			proto->strat_stats.rndv_long_gdr_recv_bytes += paylen;
		} else {
			if (p_hdr->data[1].u32w0 < 4) proto->strat_stats.rndv_long_cuCopy_recv++;
			proto->strat_stats.rndv_long_cuCopy_recv_bytes += paylen;
		}
	}
#endif
	/*
	 * if a packet has very small offset, it must have unaligned data
	 * attached in the packet header, and this must be the first packet
	 * for that message.
	 */
	if (p_hdr->data[1].u32w0 < 4 && p_hdr->data[1].u32w0 > 0) {
		psmi_assert(p_hdr->data[1].u32w0 == (req->req_data.send_msglen&0x3));
		psmi_copy_tiny_fn((uint32_t *)req->req_data.buf,
				(uint32_t *)&p_hdr->mdata,
				p_hdr->data[1].u32w0);
		req->send_msgoff += p_hdr->data[1].u32w0;
	}

	psm3_mq_handle_data(mq, req, p_hdr->data[1].u32w0, payload, paylen
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, use_gdrcopy, rcv_ev->proto->ep);
#else
				);
#endif
#ifdef PSM_ONEAPI
	if (converted)
		psmi_hal_gdr_munmap_gpu_to_host_addr(
				(unsigned long)req->user_gpu_buffer,
				paylen + p_hdr->data[1].u32w0, 1, proto->ep);
#endif
	flow = &rcv_ev->ipsaddr->flows[ips_proto_flowid(p_hdr)];
	if ((__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ) ||
	    (flow->flags & IPS_FLOW_FLAG_GEN_BECN))
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq, flow);

	psm3_ips_proto_process_ack(rcv_ev);
	return IPS_RECVHDRQ_CONTINUE;
}
