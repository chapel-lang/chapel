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
#include "psm_mq_internal.h"
#include "psm_am_internal.h"
#include "cmarw.h"

#ifdef PSM_CUDA
#include "am_cuda_memhandle_cache.h"
#endif
#ifdef PSM_ONEAPI
#include "am_oneapi_memhandle.h"
#endif

/* not reported yet, so just track in a global so can pass a pointer to
 * psm3_mq_handle_envelope and psm3_mq_handle_rts
 */
static struct ptl_strategy_stats strat_stats;

/**
 * Callback function when a receive request is matched with the
 * tag obtained from the RTS packet.
 */
static
psm2_error_t
ptl_handle_rtsmatch_request(psm2_mq_req_t req, int was_posted,
			    amsh_am_token_t *tok)
{
	psm2_amarg_t args[5];
	psm2_epaddr_t epaddr = req->rts_peer;
	struct ptl_am *ptl = (struct ptl_am *)(epaddr->ptlctl->ptl);
	int cma_succeed = 0;
	int pid = 0, gpu_ipc_send_completion = 0;

	PSM2_LOG_MSG("entering.");
	psmi_assert((tok != NULL && was_posted)
		    || (tok == NULL && !was_posted));

	_HFI_VDBG("[shm][rndv][recv] req=%p dest=%p len=%d tok=%p\n",
		  req, req->req_data.buf, req->req_data.recv_msglen, tok);
#ifdef PSM_CUDA
	if (req->cuda_ipc_handle_attached) {

		CUdeviceptr cuda_ipc_dev_ptr = am_cuda_memhandle_acquire(req->rts_sbuf - req->cuda_ipc_offset,
						  (CUipcMemHandle*)&req->cuda_ipc_handle,
								 req->req_data.recv_msglen,
								 req->rts_peer->epid);
		cuda_ipc_dev_ptr = cuda_ipc_dev_ptr + req->cuda_ipc_offset;
		/* cuMemcpy into the receive side buffer
		 * based on its location */
		if (req->is_buf_gpu_mem) {
			PSM3_GPU_MEMCPY_DTOD(req->req_data.buf, cuda_ipc_dev_ptr,
				       req->req_data.recv_msglen);
			PSM3_GPU_SYNCHRONIZE_MEMCPY();
		} else {
			PSM3_GPU_MEMCPY_DTOH(req->req_data.buf, cuda_ipc_dev_ptr,
				req->req_data.recv_msglen);
		}
		gpu_ipc_send_completion = 1;
		am_cuda_memhandle_release(cuda_ipc_dev_ptr - req->cuda_ipc_offset);
		req->cuda_ipc_handle_attached = 0;
		goto send_cts;
	}
#endif
#ifdef PSM_ONEAPI
	if (req->ze_ipc_handle_attached) {

		int ipc_fd;

		ze_device_handle_t *ze_ipc_dev_ptr = am_ze_memhandle_acquire(ptl,
						  req->rts_sbuf - req->ze_ipc_offset, &req->ze_ipc_handle,
						  req->req_data.recv_msglen, &ipc_fd, req->rts_peer);
		ze_ipc_dev_ptr = ze_ipc_dev_ptr + req->ze_ipc_offset;
		/* zeMemcpy into the receive side buffer
		 * based on its location */
		if (req->is_buf_gpu_mem) {
			PSM3_GPU_MEMCPY_DTOD(req->req_data.buf, ze_ipc_dev_ptr,
				       req->req_data.recv_msglen);
			PSM3_GPU_SYNCHRONIZE_MEMCPY();
		} else {
			PSM3_GPU_MEMCPY_DTOH(req->req_data.buf, ze_ipc_dev_ptr,
				req->req_data.recv_msglen);
		}
		gpu_ipc_send_completion = 1;
		am_ze_memhandle_release(ze_ipc_dev_ptr - req->ze_ipc_offset);
		close(ipc_fd);
		req->ze_ipc_handle_attached = 0;
		goto send_cts;
	}
#endif

	if ((ptl->psmi_kassist_mode & PSMI_KASSIST_GET)
	    && req->req_data.recv_msglen > 0
	    && (pid = psm3_epaddr_pid(epaddr))) {
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		/* If the buffer on the send side is on the host,
		 * we alloc a bounce buffer, use kassist and then
		 * do a cuMemcpy if the buffer on the recv side
		 * resides on the GPU
		 */
		if (req->is_buf_gpu_mem) {
			void* cuda_ipc_bounce_buf = psmi_malloc(PSMI_EP_NONE, UNDEFINED, req->req_data.recv_msglen);
			size_t nbytes = psm3_cma_get(pid, (void *)req->rts_sbuf,
					cuda_ipc_bounce_buf, req->req_data.recv_msglen);
			psmi_assert_always(nbytes == req->req_data.recv_msglen);
			PSM3_GPU_MEMCPY_HTOD(req->req_data.buf, cuda_ipc_bounce_buf,
				req->req_data.recv_msglen);
			/* Cuda library has recent optimizations where they do
			 * not guarantee synchronus nature for Host to Device
			 * copies for msg sizes less than 64k. The event record
			 * and synchronize calls are to guarentee completion.
			 */
			PSM3_GPU_SYNCHRONIZE_MEMCPY();
			psmi_free(cuda_ipc_bounce_buf);
		} else {
			/* cma can be done in handler context or not. */
			size_t nbytes = psm3_cma_get(pid, (void *)req->rts_sbuf,
						req->req_data.buf, req->req_data.recv_msglen);
			psmi_assert_always(nbytes == req->req_data.recv_msglen);
		}
#else
		/* cma can be done in handler context or not. */
		size_t nbytes = psm3_cma_get(pid, (void *)req->rts_sbuf,
					req->req_data.buf, req->req_data.recv_msglen);
		if (nbytes == -1) {
			ptl->psmi_kassist_mode = PSMI_KASSIST_OFF;
			_HFI_ERROR("Reading from remote process' memory failed. Disabling CMA support\n");
		}
		else {
			psmi_assert_always(nbytes == req->req_data.recv_msglen);
			cma_succeed = 1;
		}
		psmi_assert_always(nbytes == req->req_data.recv_msglen);
#endif
	}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
send_cts:
#endif
	args[0].u64w0 = (uint64_t) (uintptr_t) req->ptl_req_ptr;
	args[1].u64w0 = (uint64_t) (uintptr_t) req;
	args[2].u64w0 = (uint64_t) (uintptr_t) req->req_data.buf;
	args[3].u32w0 = req->req_data.recv_msglen;
	args[3].u32w1 = tok != NULL ? 1 : 0;
	args[4].u32w0 = ptl->psmi_kassist_mode;		// pass current kassist mode to the peer process

	if (tok != NULL) {
		psm3_am_reqq_add(AMREQUEST_SHORT, tok->ptl,
				 tok->tok.epaddr_incoming, mq_handler_rtsmatch_hidx,
				 args, 5, NULL, 0, NULL, 0);
	} else
		psm3_amsh_short_request((struct ptl *)ptl, epaddr, mq_handler_rtsmatch_hidx,
					args, 5, NULL, 0, 0);

	req->mq->stats.rx_user_num++;
	req->mq->stats.rx_user_bytes += req->req_data.recv_msglen;
	req->mq->stats.rx_shm_num++;
	req->mq->stats.rx_shm_bytes += req->req_data.recv_msglen;

	/* 0-byte completion or we used kassist */
	if (pid || cma_succeed ||
		req->req_data.recv_msglen == 0 || gpu_ipc_send_completion == 1) {
		psm3_mq_handle_rts_complete(req);
	}
	PSM2_LOG_MSG("leaving.");
	return PSM2_OK;
}

static
psm2_error_t
ptl_handle_rtsmatch(psm2_mq_req_t req, int was_posted)
{
	/* was_posted == 0 allows us to assume that we're not running this callback
	 * within am handler context (i.e. we can poll) */
	psmi_assert(was_posted == 0);
	return ptl_handle_rtsmatch_request(req, 0, NULL);
}

void
psm3_am_mq_handler(void *toki, psm2_amarg_t *args, int narg, void *buf,
		   size_t len)
{
	amsh_am_token_t *tok = (amsh_am_token_t *) toki;
	psm2_mq_req_t req;
	uint32_t tag[3];
	int rc;
	uint32_t opcode = args[0].u32w0;
	uint32_t msglen = opcode <= MQ_MSG_SHORT ? len : args[0].u32w1;

	tag[0] = args[1].u32w1;
	tag[1] = args[1].u32w0;
	tag[2] = args[2].u32w1;
	psmi_assert(toki != NULL);
	_HFI_VDBG("mq=%p opcode=%x, len=%d, msglen=%d\n",
		  tok->mq, opcode, (int)len, msglen);

	switch (opcode) {
	case MQ_MSG_TINY:
	case MQ_MSG_SHORT:
	case MQ_MSG_EAGER:
		rc = psm3_mq_handle_envelope(tok->mq, tok->tok.epaddr_incoming,
					     tag, &strat_stats, msglen, 0, buf,
					     (uint32_t) len, 1, opcode, &req);

		/* for eager matching */
		req->ptl_req_ptr = (void *)tok->tok.epaddr_incoming;
		req->msg_seqnum = 0;	/* using seqnum 0 */
		req->mq->stats.rx_shm_num++;
		// close enough, may not yet be matched,
		//  don't know recv buf_len, so assume no truncation
		req->mq->stats.rx_shm_bytes += msglen;
		break;
	default:{
			void *sreq = (void *)(uintptr_t) args[3].u64w0;
			uintptr_t sbuf = (uintptr_t) args[4].u64w0;
			psmi_assert(narg == 5);
			psmi_assert_always(opcode == MQ_MSG_LONGRTS);
			rc = psm3_mq_handle_rts(tok->mq, tok->tok.epaddr_incoming,
						tag, &strat_stats, msglen, NULL, 0, 1,
						ptl_handle_rtsmatch, &req);

			req->rts_peer = tok->tok.epaddr_incoming;
			req->ptl_req_ptr = sreq;
			req->rts_sbuf = sbuf;
#ifdef PSM_CUDA
			/* Payload in RTS would mean an IPC handle has been
			 * sent. This would also mean the sender has to
			 * send from a GPU buffer
			 */
			if (buf && len > 0) {
				req->cuda_ipc_handle = *((CUipcMemHandle*)buf);
				req->cuda_ipc_handle_attached = 1;
				req->cuda_ipc_offset = args[2].u32w0;
			}
#endif
#ifdef PSM_ONEAPI
			/* Payload in RTS would mean an IPC handle has been
			 * sent. This would also mean the sender has to
			 * send from a GPU buffer
			 */
			if (buf && len > 0) {
				req->ze_ipc_handle = *((ze_ipc_mem_handle_t*)buf);
				req->ze_ipc_handle_attached = 1;
				req->ze_ipc_offset = args[2].u32w0;

			}
#endif

			if (rc == MQ_RET_MATCH_OK)	/* we are in handler context, issue a reply */
				ptl_handle_rtsmatch_request(req, 1, tok);
			/* else will be called later */
			break;
		}
	}
	return;
}

void
psm3_am_mq_handler_data(void *toki, psm2_amarg_t *args, int narg, void *buf,
			size_t len)
{
	amsh_am_token_t *tok = (amsh_am_token_t *) toki;

	psmi_assert(toki != NULL);

	psm2_epaddr_t epaddr = (psm2_epaddr_t) tok->tok.epaddr_incoming;
	psm2_mq_req_t req = mq_eager_match(tok->mq, epaddr, 0);	/* using seqnum 0 */
	psmi_assert_always(req != NULL);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	psm3_mq_handle_data(tok->mq, req, args[2].u32w0, buf, len, 0, NULL);
#else
	psm3_mq_handle_data(tok->mq, req, args[2].u32w0, buf, len);
#endif

	return;
}

/**
 * Function to handle CTS on the sender.
 */
void
psm3_am_mq_handler_rtsmatch(void *toki, psm2_amarg_t *args, int narg, void *buf,
			    size_t len)
{
	amsh_am_token_t *tok = (amsh_am_token_t *) toki;

	psmi_assert(toki != NULL);

	ptl_t *ptl = tok->ptl;
	psm2_mq_req_t sreq = (psm2_mq_req_t) (uintptr_t) args[0].u64w0;
#ifdef PSM_CUDA
	/* If send side req has a cuda ipc handle attached then we can
	 * assume the data has been copied as soon as we get a CTS
	 */
	if (sreq->cuda_ipc_handle_attached) {
		sreq->cuda_ipc_handle_attached = 0;
		sreq->mq->stats.tx_shm_bytes += sreq->req_data.send_msglen;
		sreq->mq->stats.tx_rndv_bytes += sreq->req_data.send_msglen;
		psm3_mq_handle_rts_complete(sreq);
		return;
	}
#endif
#ifdef PSM_ONEAPI
	/* If send side req has a ze ipc handle attached then we can
	 * assume the data has been copied as soon as we get a CTS
	 */
	if (sreq->ze_ipc_handle_attached) {
		sreq->ze_ipc_handle_attached = 0;
		sreq->mq->stats.tx_shm_bytes += sreq->req_data.send_msglen;
		sreq->mq->stats.tx_rndv_bytes += sreq->req_data.send_msglen;
		psm3_mq_handle_rts_complete(sreq);
		return;
	}
#endif
	void *dest = (void *)(uintptr_t) args[2].u64w0;
	uint32_t msglen = args[3].u32w0;
	psm2_amarg_t rarg[1];

	_HFI_VDBG("[rndv][send] req=%p dest_req=%p src=%p dest=%p len=%d\n",
		  sreq, (void *)(uintptr_t) args[1].u64w0, sreq->req_data.buf, dest,
		  msglen);

	if (msglen > 0) {
		rarg[0].u64w0 = args[1].u64w0;	/* rreq */
		int kassist_mode = ((struct ptl_am *)ptl)->psmi_kassist_mode;
		int kassist_mode_peer = args[4].u32w0;
		// In general, peer process(es) shall have the same kassist mode set,
		// but due to dynamic CMA failure detection, we must align local and remote state,
		// and make protocol to adopt to that potential change.
		if (kassist_mode_peer == PSMI_KASSIST_OFF && (kassist_mode & PSMI_KASSIST_MASK)) {
			((struct ptl_am *)ptl)->psmi_kassist_mode = PSMI_KASSIST_OFF;
			goto no_kassist;
		}

		if (kassist_mode & PSMI_KASSIST_PUT) {
			int pid = psm3_epaddr_pid(tok->tok.epaddr_incoming);
			size_t nbytes = psm3_cma_put(sreq->req_data.buf, pid, dest, msglen);
			if (nbytes == -1) {
				_HFI_ERROR("Writing to remote process' memory failed. Disabling CMA support\n");
				((struct ptl_am *)ptl)->psmi_kassist_mode = PSMI_KASSIST_OFF;
				goto no_kassist;
			}

			psmi_assert_always(nbytes == msglen);

			/* Send response that PUT is complete */
			psm3_amsh_short_reply(tok, mq_handler_rtsdone_hidx,
					      rarg, 1, NULL, 0, 0);
		} else if (!(kassist_mode & PSMI_KASSIST_MASK)) {
			/* Only transfer if kassist is off, i.e. neither GET nor PUT. */
no_kassist:
			psm3_amsh_long_reply(tok, mq_handler_rtsdone_hidx, rarg,
					     1, sreq->req_data.buf, msglen, dest, 0);
		}
	}
	sreq->mq->stats.tx_shm_bytes += sreq->req_data.send_msglen;
	sreq->mq->stats.tx_rndv_bytes += sreq->req_data.send_msglen;
	psm3_mq_handle_rts_complete(sreq);
}

void
psm3_am_mq_handler_rtsdone(void *toki, psm2_amarg_t *args, int narg, void *buf,
			   size_t len)
{
	psm2_mq_req_t rreq = (psm2_mq_req_t) (uintptr_t) args[0].u64w0;
	psmi_assert(narg == 1);
	_HFI_VDBG("[rndv][recv] req=%p dest=%p len=%d\n", rreq, rreq->req_data.buf,
		  rreq->req_data.recv_msglen);
	psm3_mq_handle_rts_complete(rreq);
}

void
psm3_am_handler(void *toki, psm2_amarg_t *args, int narg, void *buf, size_t len)
{
	amsh_am_token_t *tok = (amsh_am_token_t *) toki;
	struct psm2_ep_am_handle_entry *hentry;

	psmi_assert(toki != NULL);

	hentry = psm_am_get_handler_function(tok->mq->ep,
					  (psm2_handler_t) args[0].u32w0);

	/* Note a guard here for hentry != NULL is not needed because at
	 * initialization, a psmi_assert_always() assure the entry will be
	 * non-NULL. */

	/* Invoke handler function. For AM we do not support break functionality */
	if (likely(hentry->version == PSM2_AM_HANDLER_V2)) {
		psm2_am_handler_2_fn_t hfn2 =
				(psm2_am_handler_2_fn_t)hentry->hfn;
		hfn2(toki, args + 1, narg - 1, buf, len, hentry->hctx);
	} else {
		psm2_am_handler_fn_t hfn1 =
				(psm2_am_handler_fn_t)hentry->hfn;
		hfn1(toki, args + 1, narg - 1, buf, len);
	}

	return;
}
