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

/*
 * This file implements the PSM PTL for self (loopback)
 */

#include "psm_user.h"
#include "psm_mq_internal.h"
#include "psm_am_internal.h"

struct ptl_self {
	psm2_ep_t ep;
	psm2_epid_t epid;
	psm2_epaddr_t epaddr;
	ptl_ctl_t *ctl;
} __attribute__((aligned(16)));

/* not reported yet, so just track in a global so can pass a pointer to
 * psm3_mq_handle_envelope and psm3_mq_handle_rts
 */
static struct ptl_strategy_stats strat_stats;

static
psm2_error_t
ptl_handle_rtsmatch(psm2_mq_req_t recv_req, int was_posted)
{
	psm2_mq_req_t send_req = (psm2_mq_req_t) recv_req->ptl_req_ptr;

	if (recv_req->req_data.recv_msglen > 0) {
#ifdef PSM_DSA
		if (psm3_use_dsa(recv_req->req_data.recv_msglen))
			psm3_dsa_memcpy(recv_req->req_data.buf,
					send_req->req_data.buf,
					recv_req->req_data.recv_msglen, 0,
					&send_req->mq->stats.dsa_stats[0]);
		else
#endif
		psm3_mq_mtucpy(recv_req->req_data.buf, send_req->req_data.buf,
			       recv_req->req_data.recv_msglen);
	}

	recv_req->mq->stats.rx_user_num++;
	recv_req->mq->stats.rx_user_bytes += recv_req->req_data.recv_msglen;
	psm3_mq_handle_rts_complete(recv_req);

	send_req->mq->stats.tx_rndv_bytes += send_req->req_data.send_msglen;
	send_req->mq->stats.tx_self_bytes += send_req->req_data.send_msglen;
	/* If the send is already marked complete, that's because it was internally
	 * buffered. */
	if (send_req->state == MQ_STATE_COMPLETE) {
		if (send_req->req_data.buf != NULL && send_req->req_data.send_msglen > 0)
			psm3_mq_sysbuf_free(send_req->mq, send_req->req_data.buf);
		/* req was left "live" even though the sender was told that the
		 * send was done */
		psm3_mq_req_free_internal(send_req);
	} else
		psm3_mq_handle_rts_complete(send_req);

	_HFI_VDBG("[self][complete][b=%p][sreq=%p][rreq=%p]\n",
		  recv_req->req_data.buf, send_req, recv_req);
	return PSM2_OK;
}

static
psm2_error_t self_mq_send_testwait(psm2_mq_req_t *ireq)
{
	uint8_t *ubuf;
	psm2_mq_req_t req = *ireq;

	PSMI_LOCK_ASSERT(req->mq->progress_lock);

	/* We're waiting on a send request, and the matching receive has not been
	 * posted yet.  This is a deadlock condition in MPI but we accommodate it
	 * here in the "self ptl" by using system-allocated memory.
	 */
	req->testwait_callback = NULL;	/* no more calls here */

	ubuf = req->req_data.buf;
	if (ubuf != NULL && req->req_data.send_msglen > 0) {
		req->req_data.buf = psm3_mq_sysbuf_alloc(req->mq, req->req_data.send_msglen);
		if (req->req_data.buf == NULL)
			return PSM2_NO_MEMORY;
		psm3_mq_mtucpy(req->req_data.buf, ubuf, req->req_data.send_msglen);
	}

	/* Mark it complete but don't free the req, it's freed when the receiver
	 * does the match */
	req->state = MQ_STATE_COMPLETE;
	*ireq = PSM2_MQ_REQINVALID;
	return PSM2_OK;
}

/* Self is different.  We do everything as rendezvous. */
static
psm2_error_t
self_mq_isend(psm2_mq_t mq, psm2_epaddr_t epaddr, uint32_t flags_user,
	      uint32_t flags_internal, psm2_mq_tag_t *tag, const void *ubuf,
	      uint32_t len, void *context, psm2_mq_req_t *req_o)
{
	psm2_mq_req_t send_req;
	psm2_mq_req_t recv_req;
	int rc;

	send_req = psm3_mq_req_alloc(mq, MQE_TYPE_SEND);
	if_pf(send_req == NULL)
	    return PSM2_NO_MEMORY;

#ifdef PSM_CUDA
	// we technically don't need to set is_buf_gpu_mem because psm3_mq_mtucpy
	// will be used to copy the data to the destination or a sysbuf and it will
	// check if the buffer is GPU memory. But we do need the sync_memops()
	if (len && PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(ubuf)) {
		psmi_cuda_set_attr_sync_memops(ubuf);
		send_req->is_buf_gpu_mem = 1;
	} else
		send_req->is_buf_gpu_mem = 0;
#endif
#ifdef PSM_ONEAPI
	// we don't need to set is_buf_gpu_mem because psm3_mq_mtucpy will be
	// used to copy the data to the destination or a sysbuf and it will
	// check if the buffer is a GPU memory
	//if (len && PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(ubuf)) {
	//	send_req->is_buf_gpu_mem = 1;
	//} else
	//	send_req->is_buf_gpu_mem = 0;
#endif

	mq->stats.tx_num++;
	mq->stats.tx_rndv_num++;
	// we count tx_rndv_bytes as we transfer data
	mq->stats.tx_self_num++;
	// we count tx_self_bytes as we transfer data

	rc = psm3_mq_handle_rts(mq, epaddr, tag->tag, &strat_stats,
				len, NULL, 0, 1,
				ptl_handle_rtsmatch, &recv_req);
	send_req->req_data.tag = *tag;
	send_req->req_data.buf = (void *)ubuf;
	send_req->req_data.send_msglen = len;
	send_req->req_data.context = context;
	recv_req->ptl_req_ptr = (void *)send_req;
	recv_req->rts_sbuf = (uintptr_t) ubuf;
	recv_req->rts_peer = epaddr;
	if (rc == MQ_RET_MATCH_OK)
		ptl_handle_rtsmatch(recv_req, 1);
	else
		send_req->testwait_callback = self_mq_send_testwait;

	_HFI_VDBG("[self][b=%p][m=%d][t=%08x.%08x.%08x][match=%s][req=%p]\n",
		  ubuf, len, tag->tag[0], tag->tag[1], tag->tag[2],
		  rc == MQ_RET_MATCH_OK ? "YES" : "NO", send_req);
	*req_o = send_req;
	return PSM2_OK;
}

static
psm2_error_t
self_mq_send(psm2_mq_t mq, psm2_epaddr_t epaddr, uint32_t flags,
	     psm2_mq_tag_t *tag, const void *ubuf, uint32_t len)
{
	psm2_error_t err;
	psm2_mq_req_t req;
	err = self_mq_isend(mq, epaddr, flags, PSMI_REQ_FLAG_NORMAL, tag, ubuf, len, NULL, &req);
	psm3_mq_wait_internal(&req);
	return err;
}

/* Fill in AM capabilities parameters */
static psm2_error_t
self_am_get_parameters(psm2_ep_t ep, struct psm2_am_parameters *parameters)
{
	if (parameters == NULL) {
		return PSM2_PARAM_ERR;
	}

	/* Self is just a loop-back and has no restrictions. */
	parameters->max_handlers = INT_MAX;
	parameters->max_nargs = INT_MAX;
	parameters->max_request_short = INT_MAX;
	parameters->max_reply_short = INT_MAX;

	return PSM2_OK;
}

static
psm2_error_t
self_am_short_request(psm2_epaddr_t epaddr,
		      psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		      void *src, size_t len, int flags,
		      psm2_am_completion_fn_t completion_fn,
		      void *completion_ctxt)
{
	struct psm2_ep_am_handle_entry *hentry;
	psm2_ep_t ep = ((struct ptl_self *)(epaddr->ptlctl->ptl))->ep;
	struct psmi_am_token tok;

	tok.epaddr_incoming = epaddr;

	hentry = psm_am_get_handler_function(ep, handler);

	/* Note a guard here for hentry != NULL is not needed because at
	 * initialization, a psmi_assert_always() assure the entry will be
	 * non-NULL. */

	if (likely(hentry->version == PSM2_AM_HANDLER_V2)) {
		psm2_am_handler_2_fn_t hfn2 =
				(psm2_am_handler_2_fn_t)hentry->hfn;
		hfn2(&tok, args, nargs, src, len, hentry->hctx);
	} else {
		psm2_am_handler_fn_t hfn1 =
				(psm2_am_handler_fn_t)hentry->hfn;
		hfn1(&tok, args, nargs, src, len);
	}

	if (completion_fn) {
		completion_fn(completion_ctxt);
	}

	return PSM2_OK;
}

static
psm2_error_t
self_am_short_reply(psm2_am_token_t token,
		    psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		    void *src, size_t len, int flags,
		    psm2_am_completion_fn_t completion_fn, void *completion_ctxt)
{
	struct psm2_ep_am_handle_entry *hentry;
	struct psmi_am_token *tok = token;
	struct ptl_self *ptl = (struct ptl_self *)tok->epaddr_incoming->ptlctl->ptl;
	psm2_ep_t ep = ptl->ep;

	hentry = psm_am_get_handler_function(ep, handler);

	/* Note a guard here for hentry != NULL is not needed because at
	 * initialization, a psmi_assert_always() assure the entry will be
	 * non-NULL. */

	if (likely(hentry->version == PSM2_AM_HANDLER_V2)) {
		psm2_am_handler_2_fn_t hfn2 =
				(psm2_am_handler_2_fn_t)hentry->hfn;
		hfn2(token, args, nargs, src, len, hentry->hctx);
	} else {
		psm2_am_handler_fn_t hfn1 =
				(psm2_am_handler_fn_t)hentry->hfn;
		hfn1(token, args, nargs, src, len);
	}

	if (completion_fn) {
		completion_fn(completion_ctxt);
	}

	return PSM2_OK;
}

static
psm2_error_t
self_connect(ptl_t *ptl_gen,
	     int numep,
	     const psm2_epid_t array_of_epid[],
	     const int array_of_epid_mask[],
	     psm2_error_t array_of_errors[],
	     psm2_epaddr_t array_of_epaddr[], uint64_t timeout_ns)
{
	struct ptl_self *ptl = (struct ptl_self *)ptl_gen;
	psmi_assert_always(ptl->epaddr != NULL);
	psm2_error_t err = PSM2_OK;
	int i;

	PSMI_LOCK_ASSERT(ptl->ep->mq->progress_lock);

	for (i = 0; i < numep; i++) {
		if (!array_of_epid_mask[i])
			continue;

		if (!psm3_epid_cmp_internal(array_of_epid[i], ptl->epid)) {
			_HFI_CONNDBG("connect self\n");
			array_of_epaddr[i] = ptl->epaddr;
			array_of_epaddr[i]->ptlctl = ptl->ctl;
			array_of_epaddr[i]->epid = ptl->epid;
			if (psm3_epid_set_hostname(psm3_epid_nid(ptl->epid),
						   psm3_gethostname(), 0)) {
				err = PSM2_NO_MEMORY;
				goto fail;
			}
			psm3_epid_add(ptl->ep, ptl->epid, ptl->epaddr);
			array_of_errors[i] = PSM2_OK;
		} else {
			array_of_epaddr[i] = NULL;
			array_of_errors[i] = PSM2_EPID_UNREACHABLE;
		}
	}

fail:
	return err;
}

static
psm2_error_t
self_disconnect(ptl_t *ptl_gen, int force, int numep,
		   psm2_epaddr_t array_of_epaddr[],
		   const int array_of_epaddr_mask[],
		   psm2_error_t array_of_errors[], uint64_t timeout_in)
{
	struct ptl_self *ptl = (struct ptl_self *)ptl_gen;
	int i;
	for (i = 0; i < numep; i++) {
		if (array_of_epaddr_mask[i] == 0)
			continue;

		if (array_of_epaddr[i] == ptl->epaddr) {
			_HFI_CONNDBG("disconnect self\n");
			psm3_epid_remove(ptl->ep, ptl->epid);
			array_of_errors[i] = PSM2_OK;
		}
	}
	return PSM2_OK;
}

static
size_t self_ptl_sizeof(void)
{
	return sizeof(struct ptl_self);
}

ustatic
psm2_error_t self_ptl_init(const psm2_ep_t ep, ptl_t *ptl_gen, ptl_ctl_t *ctl)
{
	struct ptl_self *ptl = (struct ptl_self *)ptl_gen;
	psmi_assert_always(ep != NULL);
	psmi_assert_always(ep->epaddr != NULL);
	psmi_assert_always(!psm3_epid_zero_internal(ep->epid));

	ptl->ep = ep;
	ptl->epid = ep->epid;
	ptl->epaddr = ep->epaddr;
	ptl->ctl = ctl;

	memset(ctl, 0, sizeof(*ctl));
	/* Fill in the control structure */
	ctl->ptl = ptl_gen;
	ctl->ep = ep;
	ctl->ep_poll = NULL;
	ctl->ep_connect = self_connect;
	ctl->ep_disconnect = self_disconnect;

	ctl->mq_send = self_mq_send;
	ctl->mq_isend = self_mq_isend;

	ctl->am_get_parameters = self_am_get_parameters;
	ctl->am_short_request = self_am_short_request;
	ctl->am_short_reply = self_am_short_reply;

#if 0	// unused code, specific to QLogic MPI
	/* No stats in self */
	ctl->epaddr_stats_num = NULL;
	ctl->epaddr_stats_init = NULL;
	ctl->epaddr_stats_get = NULL;
#endif

	return PSM2_OK;
}

static psm2_error_t self_ptl_fini(ptl_t *ptl, int force, uint64_t timeout_ns)
{
	return PSM2_OK;		/* nothing to do */
}

static
psm2_error_t
self_ptl_setopt(const void *component_obj, int optname,
		const void *optval, uint64_t optlen)
{
	/* No options for SELF PTL at the moment */
	return psm3_handle_error(NULL, PSM2_PARAM_ERR,
				 "Unknown SELF ptl option %u.", optname);
}

static
psm2_error_t
self_ptl_getopt(const void *component_obj, int optname,
		void *optval, uint64_t *optlen)
{
	/* No options for SELF PTL at the moment */
	return psm3_handle_error(NULL, PSM2_PARAM_ERR,
				 "Unknown SELF ptl option %u.", optname);
}

/* Only symbol we expose out of here */
struct ptl_ctl_init
psm3_ptl_self = {
	self_ptl_sizeof, self_ptl_init, self_ptl_fini, self_ptl_setopt,
	self_ptl_getopt
};
