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
#include "psm_mq_internal.h"
#include "ptl_ips/ips_proto_header.h"

#ifdef PSM_CUDA
#include "psm_gdrcpy.h"
#endif

#if 0
/* Not exposed in public psm, but may extend parts of PSM 2.1 to support
 * this feature before 2.3 */
psm_mq_unexpected_callback_fn_t
psmi_mq_register_unexpected_callback(psm2_mq_t mq,
				     psm_mq_unexpected_callback_fn_t fn)
{
	psm_mq_unexpected_callback_fn_t old_fn = mq->unexpected_callback;
	mq->unexpected_callback = fn;
	return old_fn;
}
#endif

// the RTS/CTS sequence using TID is now complete
// used on both sender and receiver side
// LONG_DATA on sender ends up in ips_proto_mq_eager_complete
// LONG_DATA on receiver ends up in psmi_mq_handle_data
void psmi_mq_handle_rts_complete(psm2_mq_req_t req)
{
	psm2_mq_t mq = req->mq;

	if (req->mr) {
		_HFI_MMDBG("RTS complete, releasing MR: rkey: 0x%x\n", req->mr->rkey);
		psm2_verbs_release_mr(req->mr);
		req->mr = NULL;
		ips_tid_mravail_callback(req->rts_peer->proto);
	}

	/* Stats on rendez-vous messages */
	psmi_mq_stats_rts_account(req);
	req->state = MQ_STATE_COMPLETE;
	ips_barrier();
	if(!psmi_is_req_internal(req))
		mq_qq_append(&mq->completed_q, req);

	_HFI_VDBG("RTS complete, req=%p, recv_msglen = %d\n",
		  req, req->req_data.recv_msglen);
	return;
}

static void
psmi_mq_req_copy(psm2_mq_req_t req,
		 uint32_t offset, const void *buf, uint32_t nbytes)
{
	/* recv_msglen may be changed by unexpected receive req_data.buf. */
	uint32_t msglen_this, end;
	uint8_t *msgptr = (uint8_t *) req->req_data.buf + offset;

	/* out of receiving range. */
	if (offset >= req->req_data.recv_msglen) {
		req->send_msgoff += nbytes;
		return;
	}

	end = offset + nbytes;
	if (end > req->req_data.recv_msglen) {
		msglen_this = req->req_data.recv_msglen - offset;
		end = req->req_data.recv_msglen;
	} else {
		msglen_this = nbytes;
	}

	psmi_mq_mtucpy(msgptr, buf, msglen_this);

	if (req->recv_msgoff < end) {
		req->recv_msgoff = end;
	}

	req->send_msgoff += nbytes;
	return;
}

// This handles eager and LONG_DATA payload and completion for receiver
int
psmi_mq_handle_data(psm2_mq_t mq, psm2_mq_req_t req,
		    uint32_t offset, const void *buf, uint32_t nbytes)
{
	psmi_assert(req != NULL);
	int rc;

	if (req->state == MQ_STATE_MATCHED)
		rc = MQ_RET_MATCH_OK;
	else {
		psmi_assert(req->state == MQ_STATE_UNEXP);
		rc = MQ_RET_UNEXP_OK;
	}

	psmi_mq_req_copy(req, offset, buf, nbytes);

	/*
	 * the reason to use >= is because send_msgoff
	 * may be DW pad included.
	 */
	if (req->send_msgoff >= req->req_data.send_msglen) {
		if (req->type & MQE_TYPE_EAGER_QUEUE) {
			STAILQ_REMOVE(&mq->eager_q, req, psm2_mq_req, nextq);
		}

		if (req->state == MQ_STATE_MATCHED) {
			psmi_assert(! req->mr);
			req->state = MQ_STATE_COMPLETE;
			ips_barrier();
			mq_qq_append(&mq->completed_q, req);
		} else {	/* MQ_STATE_UNEXP */
			req->state = MQ_STATE_COMPLETE;
		}
	}

	return rc;
}

static
void mq_add_to_unexpected_hashes(psm2_mq_t mq, psm2_mq_req_t req)
{
	int table;
	mq_qq_append(&mq->unexpected_q, req);
	req->q[PSM2_ANYTAG_ANYSRC] = &mq->unexpected_q;
	mq->unexpected_list_len++;
	if_pt (mq->nohash_fastpath) {
		if_pf (mq->unexpected_list_len >= HASH_THRESHOLD)
			psmi_mq_fastpath_disable(mq);
		return;
	}

	for (table = PSM2_TAG_SRC; table < PSM2_ANYTAG_ANYSRC; table++)
		mq_qq_append_which(mq->unexpected_htab,
				   table, mq->hashvals[table], req);
	mq->unexpected_hash_len++;
}


psm2_mq_req_t
mq_list_scan(struct mqq *q, psm2_epaddr_t src, psm2_mq_tag_t *tag, int which, uint64_t *time_threshold)
{
	psm2_mq_req_t *curp, cur;

	for (curp = &q->first;
	     ((cur = *curp) != NULL) && (cur->timestamp < *time_threshold);
	     curp = &cur->next[which]) {
		if ((cur->req_data.peer == PSM2_MQ_ANY_ADDR || src == cur->req_data.peer) &&
		    !((tag->tag[0] ^ cur->req_data.tag.tag[0]) & cur->req_data.tagsel.tag[0]) &&
		    !((tag->tag[1] ^ cur->req_data.tag.tag[1]) & cur->req_data.tagsel.tag[1]) &&
		    !((tag->tag[2] ^ cur->req_data.tag.tag[2]) & cur->req_data.tagsel.tag[2])) {
			*time_threshold = cur->timestamp;
			return cur;
		}
	}
	return NULL;
}

psm2_mq_req_t
mq_req_match(psm2_mq_t mq, psm2_epaddr_t src, psm2_mq_tag_t *tag, int remove)
{
	psm2_mq_req_t match[4];
	int table;
	uint64_t best_ts = -1;

	if (mq->nohash_fastpath) {
		table = PSM2_ANYTAG_ANYSRC;
		match[table] =
			mq_list_scan(&mq->expected_q,
				     src, tag, PSM2_ANYTAG_ANYSRC, &best_ts);
		if (match[table] && remove) {
			mq->expected_list_len--;
			mq_qq_remove_which(match[table], table);
		}
		return match[table];
	}

	mq->hashvals[PSM2_TAG_SRC] = hash_64(tag->tag64) % NUM_HASH_BUCKETS;
	mq->hashvals[PSM2_TAG_ANYSRC] = hash_32(tag->tag[0]) % NUM_HASH_BUCKETS;
	mq->hashvals[PSM2_ANYTAG_SRC] = hash_32(tag->tag[1]) % NUM_HASH_BUCKETS;

	for (table = PSM2_TAG_SRC; table < PSM2_ANYTAG_ANYSRC; table++)
		match[table] =
			mq_list_scan(&mq->expected_htab[table][mq->hashvals[table]],
				     src, tag, table, &best_ts);
	table = PSM2_ANYTAG_ANYSRC;
	match[table] = mq_list_scan(&mq->expected_q, src, tag, table, &best_ts);

	table = min_timestamp_4(match);
	if (table == -1)
		return NULL;

	if (remove) {
		if_pt (table == PSM2_ANYTAG_ANYSRC)
			mq->expected_list_len--;
		else
			mq->expected_hash_len--;
		mq_qq_remove_which(match[table], table);
		psmi_mq_fastpath_try_reenable(mq);
	}
	return match[table];
}
/*
 * This handles the rendezvous MPI envelopes, the packet might have the whole
 * message payload, or zero payload.
 * our return indicates if we had a match.  If no match we prepare the
 * req for future processing and callback when a future MPI_recv call matches
 * as a performance optmization, the first time we lack a match we ask for
 * a REVISIT of the message to help the case where the MPI_recv is just slightly
 * after the RTS arrived
 */
int
psmi_mq_handle_rts(psm2_mq_t mq, psm2_epaddr_t src, psm2_mq_tag_t *tag,
		   uint32_t send_msglen, const void *payload, uint32_t paylen,
		   int msgorder, mq_rts_callback_fn_t cb, psm2_mq_req_t *req_o)
{
	psm2_mq_req_t req;
	uint32_t msglen;
	int rc;

	PSMI_LOCK_ASSERT(mq->progress_lock);

	_HFI_MMDBG("rts from 0x%"PRIx64" 0x%x,0x%x,0x%x",
					src->epid, tag->tag0, tag->tag1, tag->tag2);
	if (msgorder && (req = mq_req_match(mq, src, tag, 1))) {
		/* we have a match, no need to callback */
		msglen = mq_set_msglen(req, req->req_data.buf_len, send_msglen);
		/* reset send_msglen because sender only sends this many */
		req->req_data.send_msglen = msglen;
		req->state = MQ_STATE_MATCHED;
		req->req_data.peer = src;
		req->req_data.tag = *tag;

		if (paylen > msglen) paylen = msglen;
		if (paylen) {
			// payload of RTS can contain a single packet synchronous MPI msg
			psmi_mq_mtucpy(req->req_data.buf, payload, paylen);
		}
		req->recv_msgoff = req->send_msgoff = paylen;
		*req_o = req;	/* yes match */
		PSM2_LOG_EPM(OPCODE_LONG_RTS,PSM2_LOG_RX,src->epid,mq->ep->epid,
			    "req->rts_reqidx_peer: %d",req->rts_reqidx_peer);
		rc = MQ_RET_MATCH_OK;
	} else if (msgorder > 1) {
		/* There is NO request match, and this is the first time
		 * to try to process this packet, we leave the packet in
		 * hardware queue for retry in hope there is a request
		 * match next time, this is for performance
		 * consideration.
		 */
		_HFI_MMDBG("no match 1st time - revisit msgorder=%d\n", msgorder);
		rc = MQ_RET_UNEXP_NO_RESOURCES;
	} else {		/* No match, keep track of callback */
		/* this is the 2nd attempt so we need to put it on the unexpected
		 * queue and move on.  A future MPI_Recv call will match it
		 */
		// TBD - on OPA for OSU latency we tend to hit revisit queue and then
		// match on 2nd call.  On PSM UD we tend to hit revisit queue and
		// then still not match on 2nd attempt and end up here.  Unclear
		// why MPI_Recv gets posted a little slower.  Maybe RDMA Write acks
		// occur a little slower then Native OPA's explicit TID_COMPLETION
		// such that sender does not get it's MPI_Send done prior to remote
		// node completing it's MPI_Recv and starting it's next MPI_Send
		// may want to see if REVISIT is providing value or whether anything
		// to tune to speed up RDMA Send completion (eg. and the ack which
		// triggers it).
		// Experiment with skipping revisit return above and always doing
		// this need more analysis but limited if any impact on native OPA.
		_HFI_MMDBG("no match req queue msgorder=%d\n", msgorder);
		req = psmi_mq_req_alloc(mq, MQE_TYPE_RECV);
		psmi_assert(req != NULL);
		/* We don't know recv_msglen yet but we set it here for
		 * mq_iprobe */
		req->req_data.send_msglen = req->req_data.recv_msglen = send_msglen;
		PSM2_LOG_EPM_COND(req->req_data.send_msglen > mq->hfi_thresh_rv,
				 OPCODE_LONG_RTS,PSM2_LOG_RX,src->epid,mq->ep->epid,
				    "req->rts_reqidx_peer: %d",req->rts_reqidx_peer);
		req->state = MQ_STATE_UNEXP_RV;
		req->req_data.peer = src;
		req->req_data.tag = *tag;
		req->rts_callback = cb;
		if (paylen > send_msglen) paylen = send_msglen;
		if (paylen) {
			req->req_data.buf = psmi_mq_sysbuf_alloc(mq, paylen);
			psmi_assert(paylen == 0 || req->req_data.buf != NULL);
			mq->stats.rx_sysbuf_num++;
			mq->stats.rx_sysbuf_bytes += paylen;
			psmi_mq_mtucpy(req->req_data.buf, payload, paylen);
		}
		req->recv_msgoff = req->send_msgoff = paylen;

		if (msgorder) {
			mq_add_to_unexpected_hashes(mq, req);
		}
		/* caller will handle out of order case */
		*req_o = req;	/* no match, will callback */
		rc = MQ_RET_UNEXP_OK;
	}

#ifdef PSM_DEBUG
	if (req)
		_HFI_VDBG("match=%s (req=%p) src=%s mqtag=%08x.%08x.%08x recvlen=%d "
			  "sendlen=%d errcode=%d\n",
			  rc == MQ_RET_MATCH_OK ? "YES" : "NO", req,
			  psmi_epaddr_get_name(src->epid),
			  req->req_data.tag.tag[0], req->req_data.tag.tag[1], req->req_data.tag.tag[2],
			  req->req_data.recv_msglen, req->req_data.send_msglen, req->req_data.error_code);
	else
		_HFI_VDBG("match=%s (req=%p) src=%s\n",
			  rc == MQ_RET_MATCH_OK ? "YES" : "NO", req,
			  psmi_epaddr_get_name(src->epid));
#endif /* #ifdef PSM_DEBUG */
	return rc;
}

/*
 * This handles the regular (i.e. non-rendezvous MPI envelopes)
 */
int
psmi_mq_handle_envelope(psm2_mq_t mq, psm2_epaddr_t src, psm2_mq_tag_t *tag,
			uint32_t send_msglen, uint32_t offset,
			const void *payload, uint32_t paylen, int msgorder,
			uint32_t opcode, psm2_mq_req_t *req_o)
{
	psm2_mq_req_t req;
	uint32_t msglen;
	psmi_mtucpy_fn_t psmi_mtucpy_fn;
#if defined(PSM_CUDA)
	int converted = 0;
#endif // PSM_CUDA

	if (msgorder && (req = mq_req_match(mq, src, tag, 1))) {
		/* we have a match */
		void *user_buffer = req->req_data.buf;
		psmi_assert(MQE_TYPE_IS_RECV(req->type));
		req->req_data.peer = src;
		req->req_data.tag = *tag;
		msglen = mq_set_msglen(req, req->req_data.buf_len, send_msglen);

		_HFI_VDBG("match=YES (req=%p) opcode=%x src=%s mqtag=%x.%x.%x"
			  " msglen=%d paylen=%d\n", req, opcode,
			  psmi_epaddr_get_name(src->epid),
			  tag->tag[0], tag->tag[1], tag->tag[2], msglen,
			  paylen);

		switch (opcode) {
		case MQ_MSG_TINY:
			/* mq_copy_tiny() can handle zero byte */
#ifdef PSM_CUDA
			if (PSMI_USE_GDR_COPY(req, msglen)) {
				user_buffer = gdr_convert_gpu_to_host_addr(GDR_FD,
								(unsigned long)req->req_data.buf,
								msglen, 1, src->proto);
				converted = 1;
			}
#endif
			mq_copy_tiny((uint32_t *) user_buffer, (uint32_t *) payload, msglen);
#if defined(PSM_CUDA)
			if (converted) {
				gdr_unmap_gpu_host_addr(GDR_FD, user_buffer, msglen,
                                    src->proto);
			}
#endif // PSM_CUDA

			req->state = MQ_STATE_COMPLETE;
			ips_barrier();
			mq_qq_append(&mq->completed_q, req);
			break;

		case MQ_MSG_SHORT:	/* message fits in 1 payload */
			psmi_mtucpy_fn = psmi_mq_mtucpy;
#ifdef PSM_CUDA
			if (PSMI_USE_GDR_COPY(req, msglen)) {
				user_buffer = gdr_convert_gpu_to_host_addr(GDR_FD,
							(unsigned long)req->req_data.buf,
							msglen, 1, src->proto);
				psmi_mtucpy_fn = psmi_mq_mtucpy_host_mem;
				converted = 1;
			}
#endif
			if (msglen <= paylen) {
				psmi_mtucpy_fn(user_buffer, payload, msglen);
			} else {
				psmi_assert((msglen & ~0x3) == paylen);
				psmi_mtucpy_fn(user_buffer, payload, paylen);
				/*
				 * there are nonDW bytes attached in header,
				 * copy after the DW payload.
				 */
				mq_copy_tiny((uint32_t *)((uint8_t *)user_buffer + paylen),
					(uint32_t *)&offset, msglen & 0x3);
			}
#if defined(PSM_CUDA)
			if (converted) {
				gdr_unmap_gpu_host_addr(GDR_FD, user_buffer, msglen,
                                    src->proto);
			}
#endif // PSM_CUDA
			req->state = MQ_STATE_COMPLETE;
			ips_barrier();
			mq_qq_append(&mq->completed_q, req);
			break;

		case MQ_MSG_EAGER:
			req->state = MQ_STATE_MATCHED;
			req->type |= MQE_TYPE_EAGER_QUEUE;
			req->send_msgoff = req->recv_msgoff = 0;
			STAILQ_INSERT_TAIL(&mq->eager_q, req, nextq);
			_HFI_VDBG("exp MSG_EAGER of length %d bytes pay=%d\n",
				  msglen, paylen);
#ifdef PSM_CUDA
			if (PSMI_USE_GDR_COPY(req, req->req_data.send_msglen)) {
				req->req_data.buf = gdr_convert_gpu_to_host_addr(GDR_FD,
						(unsigned long)req->user_gpu_buffer,
						req->req_data.send_msglen, 1, src->proto);
				converted = 1;
			}
#endif
			if (paylen > 0)
				psmi_mq_handle_data(mq, req, offset, payload,
						    paylen);
#if defined(PSM_CUDA)
			if (converted) {
				gdr_unmap_gpu_host_addr(GDR_FD, req->req_data.buf,
									req->req_data.send_msglen, src->proto);
			}
#endif // PSM_CUDA
			break;

		default:
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "Internal error, unknown packet 0x%x",
					  opcode);
		}

		mq->stats.rx_user_bytes += msglen;
		mq->stats.rx_user_num++;

		*req_o = req;	/* yes match */
		return MQ_RET_MATCH_OK;
	}

	/* unexpected message or out of order message. */

#if 0
	/*
	 * Keep a callback here in case we want to fit some other high-level
	 * protocols over MQ (i.e. shmem).  These protocols would bypass the
	 * normal message handling and go to higher-level message handlers.
	 */
	if (msgorder && mq->unexpected_callback) {
		mq->unexpected_callback(mq, opcode, epaddr, req_data.tag, send_msglen,
					payload, paylen);
		*req_o = NULL;
		return MQ_RET_UNEXP_OK;
	}
#endif

	if (msgorder > 1) {
		/* There is NO request match, and this is the first time
		 * to try to process this packet, we leave the packet in
		 * hardware queue for retry in hope there is a request
		 * match nex time, this is for performance
		 * consideration.
		 */
		return MQ_RET_UNEXP_NO_RESOURCES;
	}

	req = psmi_mq_req_alloc(mq, MQE_TYPE_RECV);
	psmi_assert(req != NULL);

	req->req_data.peer = src;
	req->req_data.tag = *tag;
	req->recv_msgoff = 0;
	req->req_data.recv_msglen = req->req_data.send_msglen = req->req_data.buf_len = msglen =
	    send_msglen;

	_HFI_VDBG("match=NO (req=%p) opcode=%x src=%s mqtag=%08x.%08x.%08x"
		  " send_msglen=%d\n", req, opcode,
		  psmi_epaddr_get_name(src->epid),
		  tag->tag[0], tag->tag[1], tag->tag[2], send_msglen);

	switch (opcode) {
	case MQ_MSG_TINY:
		if (msglen > 0) {
			req->req_data.buf = psmi_mq_sysbuf_alloc(mq, msglen);
			psmi_assert(msglen == 0 || req->req_data.buf != NULL);
			mq->stats.rx_sysbuf_num++;
			mq->stats.rx_sysbuf_bytes += paylen;
			mq_copy_tiny((uint32_t *) req->req_data.buf,
				     (uint32_t *) payload, msglen);
		} else
			req->req_data.buf = NULL;
		req->state = MQ_STATE_COMPLETE;
		break;

	case MQ_MSG_SHORT:
		req->req_data.buf = psmi_mq_sysbuf_alloc(mq, msglen);
		psmi_assert(msglen == 0 || req->req_data.buf != NULL);
		mq->stats.rx_sysbuf_num++;
		mq->stats.rx_sysbuf_bytes += paylen;
		if (msglen <= paylen) {
			psmi_mq_mtucpy(req->req_data.buf, payload, msglen);
		} else {
			psmi_assert((msglen & ~0x3) == paylen);
			psmi_mq_mtucpy(req->req_data.buf, payload, paylen);
			/*
			 * there are nonDW bytes attached in header,
			 * copy after the DW payload.
			 */
			mq_copy_tiny((uint32_t *)(req->req_data.buf+paylen),
				(uint32_t *)&offset, msglen & 0x3);
		}
		req->state = MQ_STATE_COMPLETE;
		break;

	case MQ_MSG_EAGER:
		req->send_msgoff = 0;
		req->req_data.buf = psmi_mq_sysbuf_alloc(mq, msglen);
		psmi_assert(msglen == 0 || req->req_data.buf != NULL);
		mq->stats.rx_sysbuf_num++;
		mq->stats.rx_sysbuf_bytes += paylen;
		req->state = MQ_STATE_UNEXP;
		req->type |= MQE_TYPE_EAGER_QUEUE;
		STAILQ_INSERT_TAIL(&mq->eager_q, req, nextq);
		_HFI_VDBG("unexp MSG_EAGER of length %d bytes pay=%d\n",
			  msglen, paylen);
		if (paylen > 0)
			psmi_mq_handle_data(mq, req, offset, payload, paylen);
		break;

	default:
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Internal error, unknown packet 0x%x",
				  opcode);
	}

	mq->stats.rx_sys_bytes += msglen;
	mq->stats.rx_sys_num++;

	if (msgorder) {
		mq_add_to_unexpected_hashes(mq, req);
	}
	/* caller will handle out of order case */
	*req_o = req;		/* no match, will callback */
	return MQ_RET_UNEXP_OK;
}

int psmi_mq_handle_outoforder(psm2_mq_t mq, psm2_mq_req_t ureq)
{
	psm2_mq_req_t ereq;
	uint32_t msglen;

	ereq = mq_req_match(mq, ureq->req_data.peer, &ureq->req_data.tag, 1);
	if (ereq == NULL) {
		mq_add_to_unexpected_hashes(mq, ureq);
		return 0;
	}

	psmi_assert(MQE_TYPE_IS_RECV(ereq->type));
	ereq->req_data.peer = ureq->req_data.peer;
	ereq->req_data.tag = ureq->req_data.tag;
	msglen = mq_set_msglen(ereq, ereq->req_data.buf_len, ureq->req_data.send_msglen);

	switch (ureq->state) {
	case MQ_STATE_COMPLETE:
		if (ureq->req_data.buf != NULL) {	/* 0-byte don't alloc a sysreq_data.buf */
			psmi_mq_mtucpy(ereq->req_data.buf, (const void *)ureq->req_data.buf,
				       msglen);
			psmi_mq_sysbuf_free(mq, ureq->req_data.buf);
		}
		ereq->state = MQ_STATE_COMPLETE;
		ips_barrier();
		mq_qq_append(&mq->completed_q, ereq);
		break;
	case MQ_STATE_UNEXP:	/* not done yet */
		ereq->state = MQ_STATE_MATCHED;
		ereq->msg_seqnum = ureq->msg_seqnum;
		ereq->ptl_req_ptr = ureq->ptl_req_ptr;
		ereq->send_msgoff = ureq->send_msgoff;
		ereq->recv_msgoff = min(ureq->recv_msgoff, msglen);
		if (ereq->recv_msgoff) {
			psmi_mq_mtucpy(ereq->req_data.buf,
				       (const void *)ureq->req_data.buf,
				       ereq->recv_msgoff);
		}
		psmi_mq_sysbuf_free(mq, ureq->req_data.buf);
		ereq->type = ureq->type;
		STAILQ_INSERT_AFTER(&mq->eager_q, ureq, ereq, nextq);
		STAILQ_REMOVE(&mq->eager_q, ureq, psm2_mq_req, nextq);
		break;
	case MQ_STATE_UNEXP_RV:	/* rendez-vous ... */
		ereq->state = MQ_STATE_MATCHED;
		ereq->rts_peer = ureq->rts_peer;
		ereq->rts_sbuf = ureq->rts_sbuf;
		ereq->send_msgoff = ureq->send_msgoff;
		ereq->recv_msgoff = min(ureq->recv_msgoff, msglen);
		if (ereq->recv_msgoff) {
			psmi_mq_mtucpy(ereq->req_data.buf,
				       (const void *)ureq->req_data.buf,
				       ereq->recv_msgoff);
		}
		if (ereq->send_msgoff) {
			psmi_mq_sysbuf_free(mq, ureq->req_data.buf);
		}
		ereq->rts_callback = ureq->rts_callback;
		ereq->rts_reqidx_peer = ureq->rts_reqidx_peer;
		ereq->type = ureq->type;
		ereq->rts_callback(ereq, 0);
		break;
	default:
		fprintf(stderr, "Unexpected state %d in req %p\n", ureq->state,
			ureq);
		fprintf(stderr, "type=%d, mq=%p, tag=%08x.%08x.%08x\n",
			ureq->type, ureq->mq, ureq->req_data.tag.tag[0],
			ureq->req_data.tag.tag[1], ureq->req_data.tag.tag[2]);
		abort();
	}

	psmi_mq_req_free(ureq);
	return 0;
}
