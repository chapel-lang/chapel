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

/* Copyright (c) 2016 Intel Corporation. All rights reserved. */

// This file implements the RDMA
// protocol for verbs mode.  The majority of functons in this file (perhaps all)
// are not used when RDMA is disabled via PSM3_RDMA
// RDMA is N/A for sockets, so it will behave as if PSM3_RDMA is disabled
// and not use functions in this file.

#include "psm_user.h"
#include "psm2_hal.h"

#include "ips_scb.h"
#include "ips_tidflow.h"
#include "ips_proto.h"
#include "ips_expected_proto.h"
#include "ips_proto_help.h"
#include "psm_mq_internal.h"

#define _HFI_CONNDBG_OR_MMDBG(fmt, ...) \
	do { \
		if (_HFI_CONNDBG_ON || _HFI_MMDBG_ON) \
			_HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__); \
	} while (0)

/*
 * Timer callbacks.  When we need work to be done out of the receive process
 * loop, we schedule work on timers to be done at a later time.
 */
#ifdef PSM_HAVE_RDMA
static psm2_error_t
ips_tid_pendsend_timer_callback(struct psmi_timer *timer, uint64_t current);
#endif // PSM_HAVE_RDMA

static psm2_error_t
ips_tid_pendtids_timer_callback(struct psmi_timer *timer, uint64_t current);

// TBD explore when to use PSM_HAVE_REG_MR vs PSM_VERBS vs put in HAL
// also code which tests RNDV_MOD or USE_RC is a candidate to move to HAL
// any code which remains here and tests RNDV_MOD should test PSM_HAVE_RNDV_MOD
#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
static void ips_protoexp_send_err_chk_rdma_resp(struct ips_flow *flow);
static void ips_tid_reissue_rdma_write(struct ips_tid_send_desc *tidsendc);
#endif
#endif

#ifdef PSM_HAVE_RDMA
static void ips_tid_scbavail_callback(struct ips_scbctrl *scbc, void *context);
static void ips_tidflow_avail_callback(struct ips_tf *tfc, void *context);
#endif // PSM_HAVE_RDMA


#ifdef PSM_HAVE_RDMA
static psm2_error_t ips_tid_recv_free(struct ips_tid_recv_desc *tidrecvc);
#endif // PSM_HAVE_RDMA
static psm2_error_t ips_tid_send_exp(struct ips_tid_send_desc *tidsendc);

#ifdef PSM_HAVE_GPU
static
void psmi_gpu_run_prefetcher(struct ips_protoexp *protoexp,
			     struct ips_tid_send_desc *tidsendc);
static void psmi_attach_chb_to_tidsendc(struct ips_protoexp *protoexp,
					psm2_mq_req_t req,
					struct ips_tid_send_desc *tidsendc,
					struct ips_gpu_hostbuf *chb_prev,
					uint32_t tsess_srcoff,
					uint32_t tsess_length,
					psm2_chb_match_type_t type);
#endif

psm2_error_t
MOCKABLE(psm3_ips_protoexp_init)(const struct ips_proto *proto,
		  uint32_t protoexp_flags,
		  int num_of_send_bufs,
		  int num_of_send_desc, struct ips_protoexp **protoexp_o)
{
#ifndef PSM_HAVE_RDMA
	psmi_assert_always(0);
	return PSM2_INTERNAL_ERR;
#else
	struct ips_protoexp *protoexp = NULL;
	psm2_ep_t ep = proto->ep;
	psm2_error_t err = PSM2_OK;

	if (!psmi_hal_has_cap(PSM_HAL_CAP_RDMA)) {
		psmi_assert_always(0);
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	protoexp = (struct ips_protoexp *)
	    psmi_calloc(ep, UNDEFINED, 1, sizeof(struct ips_protoexp));
	if (protoexp == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}
	*protoexp_o = protoexp;

	protoexp->ptl = (const struct ptl *)proto->ptl;
	protoexp->proto = (struct ips_proto *)proto;
	protoexp->timerq = proto->timerq;
	protoexp->tid_flags = protoexp_flags;

	if (ep->memmode == PSMI_MEMMODE_MINIMAL) {
		protoexp->tid_flags |= IPS_PROTOEXP_FLAG_CTS_SERIALIZED;
	}


	/* Must be initialized already */
	/* Comment out because of Klockwork scanning critical error. CQ 11/16/2012
	   psmi_assert_always(proto->ep != NULL && proto->ep->mq != NULL &&
	   proto->ep->mq->rreq_pool != NULL &&
	   proto->ep->mq->sreq_pool != NULL);
	 */
	psmi_assert_always(proto->timerq != NULL);

	/* These request pools are managed by the MQ component */
	protoexp->tid_sreq_pool = proto->ep->mq->sreq_pool;
	protoexp->tid_rreq_pool = proto->ep->mq->rreq_pool;

	protoexp->ctrl_xfer_type = PSM_TRANSFER_PIO;

	/* Initialize tid flow control. */
	err = psm3_ips_tf_init(protoexp, &protoexp->tfc,
			       ips_tidflow_avail_callback);
	if (err != PSM2_OK)
		goto fail;

	if ((err = psm3_ips_scbctrl_init(proto, num_of_send_desc, 0,
				    0, 0, ips_tid_scbavail_callback,
				    protoexp, &protoexp->tid_scbc_rv)))
		goto fail;

	{
		union psmi_envvar_val env_rts_cts_interleave;

		psm3_getenv("PSM3_RTS_CTS_INTERLEAVE",
			    "Interleave the handling of RTS to provide a fair distribution between multiple senders",
			    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			    (union psmi_envvar_val)0, &env_rts_cts_interleave);
		if (env_rts_cts_interleave.e_uint)
			protoexp->tid_flags |= IPS_PROTOEXP_FLAG_RTS_CTS_INTERLEAVE;
	}

	/* Send descriptors.
	 *
	 * There can be up to 2^32 of these send descriptors.  We conservatively
	 * allocate 256 but large node configurations can allocate up to sdesc_num
	 * of these (they are about 2k each).
	 * We impose a theoretical limit of 2^30.
	 */
	{
		struct psmi_rlimit_mpool rlim = TID_SENDSESSIONS_LIMITS;
		uint32_t maxsz, chunksz;

		if ((err = psm3_parse_mpool_env(protoexp->proto->mq, 1,
						&rlim, &maxsz, &chunksz)))
			goto fail;

		protoexp->tid_desc_send_pool =
		    psm3_mpool_create(sizeof(struct ips_tid_send_desc), chunksz,
				      maxsz, 0, DESCRIPTORS, NULL, NULL);

		if (protoexp->tid_desc_send_pool == NULL) {
			err = psm3_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate tid descriptor memory pool");
			goto fail;
		}
	}

	/* Receive descriptors are an array in tidflow structure. */

	/* This pool can never be smaller than the max number of rreqs that can be
	 * allocated. */
	{
		uint32_t rreq_per_chunk, rreq_max;

		psmi_assert_always(protoexp->proto->mq->rreq_pool != NULL);

		psm3_mpool_get_obj_info(protoexp->proto->mq->rreq_pool,
					&rreq_per_chunk, &rreq_max);

		protoexp->tid_getreq_pool =
		    psm3_mpool_create(sizeof(struct ips_tid_get_request),
				      rreq_per_chunk, rreq_max, 0, DESCRIPTORS,
				      NULL, NULL);

		if (protoexp->tid_getreq_pool == NULL) {
			err = psm3_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate getreq descriptor memory pool");
			goto fail;
		}
	}

	/* Timers to handle requeueing of work out of the receive path */
	psmi_timer_entry_init(&protoexp->timer_send,
			      ips_tid_pendsend_timer_callback, protoexp);
	STAILQ_INIT(&protoexp->pend_sendq);
	psmi_timer_entry_init(&protoexp->timer_getreqs,
			      ips_tid_pendtids_timer_callback, protoexp);
	STAILQ_INIT(&protoexp->pend_getreqsq);
#ifdef PSM_HAVE_RDMA_ERR_CHK
	STAILQ_INIT(&protoexp->pend_err_resp);
#endif

#ifdef PSM_HAVE_GPU
	if (PSM3_GPU_IS_ENABLED) {
		struct psmi_rlimit_mpool rlim = GPU_HOSTBUFFER_LIMITS;
		uint32_t maxsz, chunksz, max_elements;
		uint32_t pool_num_obj_max_total;
		uint32_t small_pool_num_obj_max_total;

		if ((err = psm3_parse_mpool_env(protoexp->proto->mq, 1,
						&rlim, &maxsz, &chunksz)))
			goto fail;

		/* the maxsz is the amount in MB, not the number of entries,
		 * since the element size depends on the window size */
		max_elements = (maxsz*1024*1024) /
			psm3_mq_max_window_rv(proto->mq, 1);
		/* mpool requires max_elements to be power of 2. round down. */
		max_elements = 1 << (31 - __builtin_clz(max_elements));
		/* need at least 2 buffers */
		max_elements = max(2, max_elements);
		protoexp->gpu_hostbuf_recv_cfg.bufsz =
			psm3_mq_max_window_rv(proto->mq, 1);

		protoexp->gpu_hostbuf_pool_recv =
			psm3_mpool_create_for_gpu(sizeof(struct ips_gpu_hostbuf),
						  chunksz, max_elements, 0,
						  UNDEFINED, NULL, NULL,
						  psmi_gpu_hostbuf_alloc_func,
						  (void *)
						  &protoexp->gpu_hostbuf_recv_cfg);

		if (protoexp->gpu_hostbuf_pool_recv == NULL) {
			err = psm3_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate GPU host receive buffer pool");
			goto fail;
		}
		psm3_mpool_get_obj_info(protoexp->gpu_hostbuf_pool_recv,
					NULL, &pool_num_obj_max_total);

		protoexp->gpu_hostbuf_small_recv_cfg.bufsz =
			GPU_SMALLHOSTBUF_SZ;
		protoexp->gpu_hostbuf_pool_small_recv =
			psm3_mpool_create_for_gpu(sizeof(struct ips_gpu_hostbuf),
						  chunksz, max_elements, 0,
						  UNDEFINED, NULL, NULL,
						  psmi_gpu_hostbuf_alloc_func,
						  (void *)
						  &protoexp->gpu_hostbuf_small_recv_cfg);

		if (protoexp->gpu_hostbuf_pool_small_recv == NULL) {
			err = psm3_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate GPU host small receive buffer pool");
			goto fail;
		}
		psm3_mpool_get_obj_info(protoexp->gpu_hostbuf_pool_small_recv,
					NULL, &small_pool_num_obj_max_total);
		_HFI_DBG("GPU Recv Copy Pipeline: %u of %u bytes (small), %u of %u bytes\n",
			small_pool_num_obj_max_total,
			protoexp->gpu_hostbuf_small_recv_cfg.bufsz,
			pool_num_obj_max_total,
			protoexp->gpu_hostbuf_recv_cfg.bufsz);
		PSM3_GPU_PREPARE_HTOD_MEMCPYS(protoexp);
		STAILQ_INIT(&protoexp->gpupend_getreqsq);
	} else {
		protoexp->gpu_hostbuf_pool_recv = NULL;
		protoexp->gpu_hostbuf_pool_small_recv = NULL;
	}
#endif
	psmi_assert(err == PSM2_OK);
	return err;

fail:
#ifdef PSM_HAVE_GPU
	if (protoexp != NULL && protoexp->gpu_hostbuf_pool_recv != NULL)
		psm3_mpool_destroy(protoexp->gpu_hostbuf_pool_recv);
	if (protoexp != NULL && protoexp->gpu_hostbuf_pool_small_recv != NULL)
		psm3_mpool_destroy(protoexp->gpu_hostbuf_pool_small_recv);
#endif
	if (protoexp != NULL && protoexp->tid_getreq_pool != NULL)
		psm3_mpool_destroy(protoexp->tid_getreq_pool);
	if (protoexp != NULL && protoexp->tid_desc_send_pool != NULL)
		psm3_mpool_destroy(protoexp->tid_desc_send_pool);
	if (protoexp != NULL)
		psm3_ips_scbctrl_fini(&protoexp->tid_scbc_rv);
	if (protoexp != NULL)
		psmi_free(protoexp);
	return err;
#endif // PSM_HAVE_RDMA
}
MOCK_DEF_EPILOGUE(psm3_ips_protoexp_init);

psm2_error_t psm3_ips_protoexp_fini(struct ips_protoexp *protoexp)
{
	psm2_error_t err = PSM2_OK;

#ifdef PSM_HAVE_GPU
	if (PSM3_GPU_IS_ENABLED) {
		psm3_mpool_destroy(protoexp->gpu_hostbuf_pool_small_recv);
		psm3_mpool_destroy(protoexp->gpu_hostbuf_pool_recv);
		PSM3_GPU_SHUTDOWN_HTOD_MEMCPYS(protoexp);
	}
#endif
	psm3_mpool_destroy(protoexp->tid_getreq_pool);
	psm3_mpool_destroy(protoexp->tid_desc_send_pool);

	if ((err = psm3_ips_scbctrl_fini(&protoexp->tid_scbc_rv)))
		goto fail;

	/* finalize tid flow control. */
	if ((err = psm3_ips_tf_fini(&protoexp->tfc)))
		goto fail;

	psmi_free(protoexp);
fail:
	return err;
}

/* New scbs now available.  If we have pending sends or pending get requests,
 * turn on the timer so it can be processed. */
/* for RDMA we can also use this routine when an MR is freed.  scbc is not used
 */
#ifdef PSM_HAVE_RDMA
static
void ips_tid_scbavail_callback(struct ips_scbctrl *scbc, void *context)
{
	struct ips_protoexp *protoexp = (struct ips_protoexp *)context;

	if (!STAILQ_EMPTY(&protoexp->pend_sendq))
		psmi_timer_request(protoexp->timerq,
				   &protoexp->timer_send, PSMI_TIMER_PRIO_1);
	if (!STAILQ_EMPTY(&protoexp->pend_getreqsq)
#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
		|| !STAILQ_EMPTY(&protoexp->pend_err_resp)
#endif
#endif
		)
		psmi_timer_request(protoexp->timerq,
				   &protoexp->timer_getreqs, PSMI_TIMER_PRIO_1);
	return;
}
#endif // PSM_HAVE_RDMA

#ifdef PSM_HAVE_REG_MR
void ips_tid_mravail_callback(struct ips_proto *proto)
{
	// if we have Send DMA but not RDMA, no proto->protoexp
#ifdef PSM_HAVE_RDMA
	if (proto->protoexp)
		ips_tid_scbavail_callback(NULL, proto->protoexp);
#else
	psmi_assert_always(!proto->protoexp);
#endif
}
#endif


// On STL100 ips_tf was a user space control for the HW tidflow which
// would fully process most valid inbound EXPTID packets within an RV Window.
// For verbs we maintain the user space control to help manage each active
// RV window.
// There is one CTS per RV window (typically 128K).
// For verbs with RV, RDMA is used instread of EXPTID, with 1 RDMA per RV window
// Typically there are 32 (HFI_TF_NFLOWS) configured.
// The 32 is hard coded, could make it tunable.
// The tidflow provides a natural pacing mechanism and limits the total amount
// of inflight RDMA incoming to given receiver.

/* New Tid Flows are available. If there are pending get requests put the
 * get timer on the timerq so it can be processed. */
#ifdef PSM_HAVE_RDMA
static
void ips_tidflow_avail_callback(struct ips_tf *tfc, void *context)
{
	struct ips_protoexp *protoexp = (struct ips_protoexp *)context;

	if (!STAILQ_EMPTY(&protoexp->pend_getreqsq))
	{
		psmi_timer_request(protoexp->timerq,
				   &protoexp->timer_getreqs, PSMI_TIMER_PRIO_1);
	}
	return;
}
#endif // PSM_HAVE_RDMA

// this is called from ips_proto_mq_rts_match_callback when a RTS is matched
// and we chose to use the TID receive mechanism
// this kicks off the receiver side protocol for preparing TIDs and issuing a
// CTS which requests use of TID
/*
 * The tid get request is always issued from within the receive progress loop,
 * which is why we always enqueue the request instead of issuing it directly.
 * Eventually, if we expose tid_get to users, we will want to differentiate
 * when the request comes from the receive progress loop from cases where the
 * tid_get is issued directly from user code.
 *
 */
psm2_error_t
psm3_ips_protoexp_tid_get_from_token(struct ips_protoexp *protoexp,
				void *buf,
				uint32_t length,
				psm2_epaddr_t epaddr,
				uint32_t remote_tok,
				uint32_t flags,
				ips_tid_completion_callback_t callback,
				psm2_mq_req_t req)
{
	struct ips_tid_get_request *getreq;
	int count;
	int tidflows;
	uint64_t nbytes;

	PSM2_LOG_MSG("entering");
	psmi_assert((psm3_mq_get_window_rv(req) % PSMI_PAGESIZE) == 0);
	getreq = (struct ips_tid_get_request *)
	    psm3_mpool_get(protoexp->tid_getreq_pool);

	/* We can't *really* run out of these here because we always allocate as
	 * much as available receive reqs */
	if_pf(getreq == NULL)
	{
		PSM2_LOG_MSG("leaving");
		return psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			      "Ran out of 'getreq' descriptors");
	}

	getreq->tidgr_protoexp = protoexp;
	getreq->tidgr_epaddr = epaddr;
	getreq->tidgr_lbuf = buf;
	getreq->tidgr_length = length;
	getreq->tidgr_sendtoken = remote_tok;
	getreq->tidgr_req = req;
	getreq->tidgr_callback = callback;
	getreq->tidgr_offset = 0;
	getreq->tidgr_bytesdone = 0;
	getreq->tidgr_flags = flags;

#ifdef PSM_HAVE_GPU
	if ((req->is_buf_gpu_mem &&
	    !(protoexp->proto->flags & IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV)) ||
	    ((req->is_buf_gpu_mem &&
	     (protoexp->proto->flags & IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV) &&
	     (length > psm3_gpu_gpudirect_rdma_recv_limit
		|| length & 0x03 || (uintptr_t)buf & 0x03
 		)))) {
		getreq->gpu_hostbuf_used = 1;
		getreq->tidgr_gpu_bytesdone = 0;
		STAILQ_INIT(&getreq->pend_gpubuf);
		protoexp->proto->strat_stats.rndv_rdma_hbuf_recv++;
		protoexp->proto->strat_stats.rndv_rdma_hbuf_recv_bytes += length;
	} else {
		getreq->gpu_hostbuf_used = 0;
		if (req->is_buf_gpu_mem) {
			protoexp->proto->strat_stats.rndv_rdma_gdr_recv++;
			protoexp->proto->strat_stats.rndv_rdma_gdr_recv_bytes += length;
		} else {
#endif
			protoexp->proto->strat_stats.rndv_rdma_cpu_recv++;
			protoexp->proto->strat_stats.rndv_rdma_cpu_recv_bytes += length;
#ifdef PSM_HAVE_GPU
		}
	}
#endif

	/* nbytes is the bytes each channel should transfer. */
	count = ((ips_epaddr_t *) epaddr)->msgctl->ipsaddr_count;
#ifdef PSM_HAVE_GPU
	if (req->is_buf_gpu_mem)
		nbytes = PSMI_ALIGNUP((length + count - 1) / count, PSMI_GPU_PAGESIZE);
	else
#endif
		nbytes = PSMI_ALIGNUP((length + count - 1) / count, PSMI_PAGESIZE);
	getreq->tidgr_rndv_winsz = psm3_mq_get_window_rv(req);
	if (nbytes < getreq->tidgr_rndv_winsz)
		getreq->tidgr_rndv_winsz = nbytes;
	_HFI_MMDBG("posting TID get request: nbytes=%"PRIu64" winsz=%u len=%u\n",
				 nbytes, getreq->tidgr_rndv_winsz, getreq->tidgr_length);
	// we have now computed the size of each TID sequence (tidgr_rndv_winsz)

	STAILQ_INSERT_TAIL(&protoexp->pend_getreqsq, getreq, tidgr_next);
	// by using tidflow we also constrain amount of concurrent RDMA to our NIC
	tidflows = ips_tf_available(&protoexp->tfc);
	_HFI_MMDBG("available tidflow %u\n", tidflows);

	if (tidflows > 0)
		// get the actual TIDs and tidflows and send the CTS
		ips_tid_pendtids_timer_callback(&protoexp->timer_getreqs, 0);
	else if (tidflows != -1)
		// out of TIDs, set a timer to try again later
		psmi_timer_request(protoexp->timerq, &protoexp->timer_getreqs,
				   PSMI_TIMER_PRIO_1);
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}

/* List of perf events */
#define _ips_logeventid_tid_send_reqs	0	/* out of tid send descriptors */

#define ips_logevent_id(event)	 _ips_logeventid_ ## event
#define ips_logevent(proto, event, ptr) ips_logevent_inner(proto, ips_logevent_id(event), ptr)

static
void ips_logevent_inner(struct ips_proto *proto, int eventid, void *context)
{
	uint64_t t_now = get_cycles();

	switch (eventid) {
	case ips_logevent_id(tid_send_reqs):{
			psm2_epaddr_t epaddr = (psm2_epaddr_t) context;
			proto->psmi_logevent_tid_send_reqs.count++;

			if (t_now >=
			    proto->psmi_logevent_tid_send_reqs.next_warning) {
				psm3_handle_error(PSMI_EP_LOGEVENT, PSM2_OK,
						  "Non-fatal temporary exhaustion of send rdma descriptors "
						  "(elapsed=%.3fs, source %s, count=%lld)",
						  (double)
						  cycles_to_nanosecs(t_now -
								     proto->
								     t_init) /
						  1.0e9,
						  psm3_epid_fmt_addr(epaddr->epid, 0),
						  (long long)proto->
						  psmi_logevent_tid_send_reqs.
						  count);
				proto->psmi_logevent_tid_send_reqs.
				    next_warning =
				    t_now +
				    sec_2_cycles(proto->
						 psmi_logevent_tid_send_reqs.
						 interval_secs);
			}
		}
		break;

	default:
		break;
	}

	return;
}

/*
 * Expected Protocol.
 *
 * We're granted tids (as part of a tid get request) and expected to fulfill
 * the request by associating the request's sendtoken to a tid send descriptor.
 *
 * It's possible to be out of tid send descriptors when somehow all allocated
 * descriptors can't complete all of their sends.  For example, the targets of
 * the sends may be busy in computation loops and not processing incoming
 * packets.
 */

// build and issue CTS
void
psm3_ips_protoexp_send_tid_grant(struct ips_tid_recv_desc *tidrecvc)
{
	ips_epaddr_t *ipsaddr = tidrecvc->ipsaddr;
	struct ips_proto *proto = tidrecvc->protoexp->proto;
	psmi_assert(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
	struct ips_flow *flow = &ipsaddr->flows[proto->msgflowid];
	ips_scb_t *scb;

	scb = tidrecvc->grantscb;
	ips_scb_opcode(scb) = OPCODE_LONG_CTS;
	scb->ips_lrh.khdr.kdeth0 = 0;
	scb->ips_lrh.mdata = tidrecvc->tidflow_genseq.psn_val;
	scb->ips_lrh.data[0] = tidrecvc->rdescid;
	scb->ips_lrh.data[1].u32w1 = tidrecvc->getreq->tidgr_length;
	scb->ips_lrh.data[1].u32w0 = tidrecvc->getreq->tidgr_sendtoken;

	ips_scb_buffer(scb) = (void *)&tidrecvc->tid_list;
	scb->chunk_size = ips_scb_length(scb) = sizeof(tidrecvc->tid_list);
	_HFI_MMDBG("sending CTS rkey 0x%x\n", tidrecvc->tid_list.tsess_rkey);

	PSM2_LOG_EPM(OPCODE_LONG_CTS,PSM2_LOG_TX, proto->ep->epid,
		    flow->ipsaddr->epaddr.epid ,"tidrecvc->getreq->tidgr_sendtoken; %d",
		    tidrecvc->getreq->tidgr_sendtoken);
	proto->epaddr_stats.cts_rdma_send++;

	psm3_ips_proto_flow_enqueue(flow, scb);
	flow->flush(flow, NULL);
}


#ifdef PSM_HAVE_GPU
void psm3_ips_deallocate_send_chb(struct ips_gpu_hostbuf* chb, int reset)
{
	if (chb->is_tempbuf) {
		PSM3_GPU_HOSTBUF_DESTROY(chb);
		psmi_free(chb);
	} else {
		chb->req = NULL;
		chb->offset = 0;
		chb->bytes_read = 0;
		if (reset)
			PSM3_GPU_HOSTBUF_RESET(chb);
		psm3_mpool_put(chb);
	}
}
#endif

#ifdef PSM_HAVE_RDMA
// indicate the given tidsendc has been completed and cleanup after it
// This is called by the send CQE polling which might be within a send
// so it cannot issue any sends directly, otherwise we will have a recursive
// situation and potentially deeper recursion if more send CQEs found
// so instead it queues callbacks and schedules timers to run the callback
static void
ips_protoexp_tidsendc_complete(struct ips_tid_send_desc *tidsendc)
{
#ifdef PSM_VERBS
	struct ips_protoexp *protoexp = tidsendc->protoexp;
#elif defined(PSM_HAVE_GPU)
	struct ips_protoexp *protoexp = tidsendc->protoexp;
#endif
	psm2_mq_req_t req = tidsendc->mqreq;

	_HFI_MMDBG("ips_protoexp_tidsendc_complete tidsendc %p\n", tidsendc);
	PSM2_LOG_MSG("entering");

	req->send_msgoff += tidsendc->length;

#ifdef PSM_VERBS
	if (tidsendc->mr) {
		_HFI_MMDBG("send chunk complete, releasing MR: rkey: 0x%x\n", tidsendc->mr->rkey);
		psm3_verbs_release_mr(tidsendc->mr);
		tidsendc->mr = NULL;
	}
#endif

#ifdef PSM_HAVE_GPU
	if (req->gpu_hostbuf_used) {
		if (tidsendc->gpu_num_buf == 1) {
			tidsendc->gpu_hostbuf[0]->bytes_read +=
				tidsendc->tid_list.tsess_length;
			if(tidsendc->gpu_hostbuf[0]->bytes_read ==
				tidsendc->gpu_hostbuf[0]->size){
				STAILQ_REMOVE(&req->sendreq_prefetch,
					      tidsendc->gpu_hostbuf[0],
					      ips_gpu_hostbuf, req_next);
				psm3_ips_deallocate_send_chb(tidsendc->gpu_hostbuf[0], 1);
				psmi_gpu_run_prefetcher(protoexp, tidsendc);
			}
		} else {
			psm3_ips_deallocate_send_chb(tidsendc->gpu_split_buf, 0);
			tidsendc->gpu_split_buf = NULL;
		}
	}
#endif
	/* Check if we can complete the send request. */
	_HFI_MMDBG("ips_protoexp_tidsendc_complete tidsendc %p off %u req len %u\n",
		tidsendc, req->send_msgoff, req->req_data.send_msglen);
	if (req->send_msgoff >= req->req_data.send_msglen) {
		psm3_mq_handle_rts_complete(req);
	}

	psm3_mpool_put(tidsendc);
#ifdef PSM_VERBS
	/* we freed an MR  If we have pending sends or pending get requests,
	 * turn on the timer so it can be processed. */
	ips_tid_mravail_callback(protoexp->proto);
#endif

	PSM2_LOG_MSG("leaving");
}
#endif // PSM_HAVE_RDMA

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA
// our RDMA Write has completed on our send Q (RV or user space RC QP)
// This is called by the send CQE polling which might be within a send
// so it cannot issue any sends directly, otherwise we will have a recursive
// situation and potentially deeper recursion if more send CQEs found
// key notes in this regard:
//	psm3_mq_handle_rts_complete - sets flags in req and queues it, no callbacks
//	psm3_mpool_put(tidsendc) - tid_desc_send_pool has no callback configured
//	ips_tid_mravail_callback - psmi_timer_request call queues timer for future
//							callback  (no immediate callback)
//	psm3_mpool_put(tidsendc->gpu_hostbuf[0]) - gpu_hostbuf_pool_send has a
//							callback of psmi_gpu_hostbuf_alloc_func which
//							manages cuda buffers but does not issue any sends

int
ips_protoexp_rdma_write_completion(uint64_t wr_id)
{
	struct ips_tid_send_desc *tidsendc = (struct ips_tid_send_desc *)(uintptr_t)wr_id;

	_HFI_MMDBG("ips_protoexp_rdma_write_completion tidsendc %p\n", tidsendc);
	PSM2_LOG_MSG("entering");
#ifdef PSM_RC_RECONNECT
	// for USER RC, expect rc_qp to be specified
	psmi_assert(! (IPS_PROTOEXP_FLAG_USER_RC_QP(tidsendc->protoexp->proto->ep->rdmamode)) || tidsendc->rc_qp);
	if (tidsendc->rc_qp) {
		psm3_verbs_dec_posted(tidsendc->rc_qp);
		psm3_verbs_free_rc_qp_if_drained("RDMA Complete", tidsendc->rc_qp);
		tidsendc->rc_qp = NULL;
	}
#endif

	ips_protoexp_tidsendc_complete(tidsendc);

	PSM2_LOG_MSG("leaving");
	return IPS_RECVHDRQ_CONTINUE;
}
#endif // PSM_HAVE_RDMA
#endif // defined(PSM_VERBS)

#ifdef PSM_RC_RECONNECT
struct psm3_verbs_rc_qp *
ips_protoexp_rdma_write_completion_rc_qp(psm2_ep_t ep, uint64_t wr_id)
{
	struct ips_tid_send_desc *tidsendc = (struct ips_tid_send_desc *)(uintptr_t)wr_id;
	if (! tidsendc)
		return NULL;
	return tidsendc->rc_qp;
}
#endif /* PSM_RC_RECONNECT */

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
// our RV or RC RDMA Write has completed with error on our send Q
// This is called by the send CQE polling which might be within a send
// so it cannot issue any sends directly, otherwise we will have a recursive
// situation and potentially deeper recursion if more send CQEs found
// key notes in this regard:
// if we don't return PSM2_OK, caller will consider it an unrecoverable error
// for RC QPs, the caller will handle any necessary free of rc_qp if now drained
int
ips_protoexp_rdma_write_completion_error(psm2_ep_t ep, uint64_t wr_id,
												enum ibv_wc_status wc_status)
{
	struct ips_tid_send_desc *tidsendc = (struct ips_tid_send_desc *)(uintptr_t)wr_id;
	struct ips_protoexp *protoexp;

	PSM2_LOG_MSG("entering");
	if (! tidsendc) {
		psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"failed RDMA Write with invalid tidsendc (wr_id) on %s port %u epid %s status: '%s' (%d)\n",
			ep->dev_name, ep->portnum,
			psm3_epaddr_get_name(ep->epid, 0),
			ibv_wc_status_str(wc_status),(int)wc_status);
		goto fail_ret;
	}
	protoexp = tidsendc->protoexp;
#ifdef PSM_RC_RECONNECT
	// for USER RC, expect rc_qp to be specified
	psmi_assert(! (IPS_PROTOEXP_FLAG_USER_RC_QP(tidsendc->protoexp->proto->ep->rdmamode)) || tidsendc->rc_qp);
	if (tidsendc->rc_qp) {
		ips_protoexp_report_inflight_rc_qp(tidsendc->protoexp,
			tidsendc->rc_qp);
		tidsendc->err_chk_rdma_rcnt = tidsendc->rc_qp->reconnect_count;
		psm3_verbs_dec_posted(tidsendc->rc_qp);
		// caller will free_rc_qp if now drained
		tidsendc->rc_qp = NULL;
	}
#endif
	_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] failed RDMA Write tidsendc %p on %s port %u epid %s to %s status: '%s' (%d)\n",
			tidsendc->ipsaddr, tidsendc, ep->dev_name, ep->portnum,
			psm3_epaddr_get_name(ep->epid, 0),
			psm3_epaddr_get_name(tidsendc->ipsaddr->epaddr.epid, 1),
			ibv_wc_status_str(wc_status),(int)wc_status);

	if (! ((IPS_PROTOEXP_FLAG_USER_RC_QP(protoexp->proto->ep->rdmamode) && tidsendc->ipsaddr->allow_reconnect)
		|| (IPS_PROTOEXP_FLAG_KERNEL_QP(protoexp->proto->ep->rdmamode) && protoexp->proto->ep->reconnect_timeout)))
		goto fail; /* reconnect disabled, can't recover */

	// perhaps depending on wc_status
	// IBV_WC_REM_ACCESS_ERR and others unrecoverable
	// IBV_WC_RETRY_EXC_ERR may be recoverable
	// IBV_WC_RNR_RETRY_EXC_ERR may be recoverable
	// IBV_WC_RESP_TIMEOUT_ERR may be recoverable (is this applicable?)
	// any others?  IB_WC_GENERAL_ERR?

	tidsendc->need_err_chk_rdma = 1;
	tidsendc->is_complete = 0;	// status of send of err_chk_rdma

	/* Add as a pending op and ring up the timer */
	/* ips_tid_pendsend_timer_callback timer will issue ERR_CHK_RDMA */
	STAILQ_INSERT_TAIL(&protoexp->pend_sendq, tidsendc, next);
	psmi_timer_request(protoexp->timerq, &protoexp->timer_send,
		   PSMI_TIMER_PRIO_1);

	return PSM2_OK;

fail:
	psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"failed RDMA Write on %s port %u epid %s to %s status: '%s' (%d)\n",
			ep->dev_name, ep->portnum,
			psm3_epaddr_get_name(ep->epid, 0),
			psm3_epaddr_get_name(tidsendc->ipsaddr->epaddr.epid, 1),
			ibv_wc_status_str(wc_status),(int)wc_status);
fail_ret:
	PSM2_LOG_MSG("leaving");
	return PSM2_INTERNAL_ERR;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
static psm2_error_t ips_protoexp_send_err_chk_rdma(struct ips_tid_send_desc *tidsendc)
{
	ips_scb_t *scb = NULL;
	struct ips_protoexp *protoexp = tidsendc->protoexp;
	struct ips_proto *proto = protoexp->proto;
	ips_epaddr_t *ipsaddr = tidsendc->ipsaddr;
	struct ips_flow *flow = &ipsaddr->flows[proto->msgflowid];
	psm2_error_t err = PSM2_OK;
#ifdef RNDV_MOD
	uint32_t conn_count;
#endif

	PSM2_LOG_MSG("entering");
	_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] tidsendc %p\n", ipsaddr, tidsendc);

#ifdef RNDV_MOD
	if (ipsaddr->verbs.rv_conn) {
		psmi_assert(! IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
		// we delay our sending of err chk rdma until after the connection is
		// restored as reflected by an increment of conn_count relative to when
		// tidsendc issued the rdma_write.  This also forms a barrier to
		// ensure our err chk rdma does not arrive at receiver prior to the
		// rdma completion (eg. in case we timeded out for RC QP ack but
		// receiver got the full rdma write).
		if (psm3_rv_get_conn_count(proto->ep->rv, ipsaddr->verbs.rv_conn,
				tidsendc->rv_sconn_index, &conn_count)) {
			psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"send_err_chk_rdma: Connect unrecoverable on %s to %s\n",
				proto->ep->dev_name,
				psm3_epaddr_get_name(ipsaddr->epaddr.epid, 0));
			err = PSM2_TIMEOUT; /* force a resend reschedule */
			goto done;
		}

		// conn_count only advances.  Only need to test for equality.
		// 32b reconnect_count sufficient for 13 years of constant reconnect
		// at 100ms intervals (eg. RV_DELAY) before wrapping
		if (conn_count == tidsendc->rv_conn_count) {
			err = PSM2_TIMEOUT; /* force a resend reschedule */
			goto done;
		}
	}
#endif /* RNDV_MOD */
#ifdef USE_RC
	if (IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode)) {
#ifdef RNDV_MOD
		psmi_assert(! ipsaddr->verbs.rv_conn);
#endif
		if (! (ipsaddr->verbs.rc_connected)) {
			psmi_assert(ipsaddr->reconnecting);
			err = PSM2_TIMEOUT; /* force a resend reschedule */
			goto done;
		}
	}
#endif /* USE_RC */

	// limit to 1 outstanding per remote connection.
	// receiver can only queue 1 response if it's low on scb's
	if (ipsaddr->verbs.err_chk_rdma_outstanding) {
		err = PSM2_TIMEOUT; /* force a resend reschedule */
		goto done;
	}

	scb = psm3_ips_scbctrl_alloc(&protoexp->tid_scbc_rv, 1, 0,
				     IPS_SCB_FLAG_PRIORITY);
	if (scb == NULL) {
		// ips_tid_scbavail_callback will trigger pend_sendq again
		// and call ips_tid_pendsend_timer_callback
		err = PSM2_EP_NO_RESOURCES;
		goto done;
	}

	_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] sending ERR_CHK_RDMA tidsendc %p\n", ipsaddr, tidsendc);
	PSM2_LOG_EPM(OPCODE_ERR_CHK_RDMA,PSM2_LOG_TX, proto->ep->epid,
			ipsaddr->epaddr.epid,
			"psm3_mpool_get_obj_index(tidsendc->mqreq): %d, tidsendc->rdescid. _desc_genc %d _desc_idx: %d, tidsendc->sdescid._desc_idx: %d",
			psm3_mpool_get_obj_index(tidsendc->mqreq),
			tidsendc->rdescid._desc_genc,tidsendc->rdescid._desc_idx,
			tidsendc->sdescid._desc_idx);

	ips_scb_opcode(scb) = OPCODE_ERR_CHK_RDMA;
	scb->ips_lrh.khdr.kdeth0 = 0;
		// providing our REQ index gives receiver an extra sanity check
	scb->ips_lrh.mdata = psm3_mpool_get_obj_index(tidsendc->mqreq);
	scb->ips_lrh.data[0] = tidsendc->rdescid;
	scb->ips_lrh.data[1] = tidsendc->sdescid;
	/* path is having issue, ask for ack */
	scb->scb_flags |= IPS_SEND_FLAG_ACKREQ;
	/* INTR makes sure remote end works on it immediately */
	if (proto->flags & IPS_PROTO_FLAG_RCVTHREAD)
		scb->scb_flags |= IPS_SEND_FLAG_INTR;
#ifdef PSM_RC_RECONNECT
	if (ipsaddr->allow_reconnect) {
		// include reconnect_count of affected RC QP in ERR_CHK_RDMA payload
		// so remote can determine if recv QP's CQEs have been drained
		// as part of determining if the send RDMA was in fact received
		ips_scb_buffer(scb) = (void*)&tidsendc->err_chk_rdma_rcnt;
		scb->chunk_size = ips_scb_length(scb) = 1;
	}
#endif

	ipsaddr->verbs.err_chk_rdma_outstanding = 1;
	tidsendc->is_complete = 1;	// status of send of err_chk_rdma

	proto->epaddr_stats.err_chk_rdma_send++;

	psm3_ips_proto_flow_enqueue(flow, scb);
	flow->flush(flow, NULL);

	/* inbound ack will free scb */
done:
	PSM2_LOG_MSG("leaving");
	return err;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
// scan all alternate addresses for "expected" (multi-QP and multi-EP)
// to see if a match for "got" can be found
static
int ips_protoexp_ipsaddr_match(ips_epaddr_t *expected, ips_epaddr_t *got)
{
	ips_epaddr_t *p = expected;

	do {
		if (p == got)
			return 1;
		p = p->next;
	} while (p != expected);

	return 0;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
// process and compose info for response to an ERR_CHK_RDMA whose info has been
// stashed in ipsaddr->verbs.*err_chk*
// if unable to compose response now (still waiting for CQ to drain), sets
//	err_chk_rdma_recheck and returns PSM2_NO_PROGRESS
// otherwise returns PSM2_OK and all info needed to compose and send packet is
//	in ipsaddr
psm2_error_t ips_protoexp_do_err_chk_rdma(ips_epaddr_t *ipsaddr)
{
	struct ips_tid_recv_desc *tidrecvc;
	ptl_arg_t rdesc_id = ipsaddr->verbs.err_chk_rdma_resp_rdesc_id;
	struct ips_proto *proto = ipsaddr->epaddr.proto;
	struct ips_protoexp *protoexp = proto->protoexp;
	tidrecvc = &protoexp->tfc.tidrecvc[rdesc_id._desc_idx];

	ipsaddr->verbs.err_chk_rdma_recheck = 0;

	// for the rare case that ERR_CHK_RDMA has a rdescid which we completed
	// a while ago, we need to sanity check not only rdescid, but also
	// the identity of the sender and the sendtoken for the senders RTS
	// this protects us in case rdescid generation has wrapped
	if (tidrecvc->rdescid._desc_genc != rdesc_id._desc_genc
		|| tidrecvc->state != TIDRECVC_STATE_BUSY
		|| ! ips_protoexp_ipsaddr_match(tidrecvc->ipsaddr, ipsaddr)
		|| tidrecvc->getreq->tidgr_sendtoken
			!= ipsaddr->verbs.err_chk_rdma_sendtoken
		) {
		/* Receive descriptor mismatch in time and space.
		 * Must have completed recv for this RDMA
		 * (eg. sender timeout waiting for RC QP ack)
		 */
		ipsaddr->verbs.err_chk_rdma_resp_need_resend = 0;
		_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] not found: rdma was done\n",
			ipsaddr);
#ifdef RNDV_MOD
	} else if (ipsaddr->verbs.rv_conn
			   && psm3_rv_scan_cq(proto->ep->rv, RV_WC_RECV_RDMA_WITH_IMM,
				RDMA_IMMED_DESC_MASK,
				RDMA_PACK_IMMED(tidrecvc->rdescid._desc_genc,
								tidrecvc->rdescid._desc_idx, 0))) {
		// the CQ scan above solves a very rare race where the receiving QP is
		// very slow to issue CQEs and PSM happens to poll the UD QP and
		// find the ERR_CHK_RDMA before finding the given inbound RDMA Write
		// was successfully received.
		// Due to reconnection essentially being a barrier, we know the
		// CQE must be processed in RV drain prior to the new connection and
		// hence prior to the err chk rdma on UD QP.  So we scan the RV CQ
		// to close the race, if we find a matching completion we can
		// respond with resend_needed=0 and know we will process the CQE
		// soon to fully complete the RDMA receipt.
		// We ignore RV_IDX in this scan, it should always match us and better
		// to not ask for a resend and fail when we process the completion
		// than to ask for an a resend into a freed buffer
		_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] RV CQ shows rdma was done\n",
			ipsaddr);
		ipsaddr->verbs.err_chk_rdma_resp_need_resend = 0;
#endif /* RNDV_MOD */
#ifdef PSM_RC_RECONNECT
	} else if (IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode)) {
		// this is to solve a race where the receiving QP is very slow to
		// isssue CQEs and PSM happens to poll the UD QP and
		// find the ERR_CHK_RDMA before finding the given inbound RDMA Write
		// was successfully received.
		// So we use the reconnect_count in the ERR_CHK_RDMA packet to
		// search for the matching ipsaddr rc_qp.  If the rc_qp still exists
		// it has not yet been drained and we need to delay processing
		// the ERR_CHK_RDMA until the QP has been drained (and freed) so
		// we can be certain the RDMA Write will not successfully complete.
		// The reconnect_count forms a barrier, since the lost QP connection
		// will have a finite amount of processing and any RC QPs created
		// since then will have a new reconnect_count.

		// if ! allow_reconnect, failed RDMA is fatal at sender
		// and we should never get ERR_CHK_RDMA
		psmi_assert(ipsaddr->allow_reconnect);

		if (ipsaddr->verbs.err_chk_rdma_rcnt == ipsaddr->reconnect_count) {
			// This is our 1st discovery of the QP issue.
			// flags==1 will cause HAL to force QP to ERR in drain_rc_qp
			// We are not processing a RC QP CQE here, so no need to
			// explicitly psm3_verbs_free_rc_qp_if_empty() if
			// connection_error() reports NO_PROGRESS
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] call connection error\n",
				ipsaddr);
			(void)psm3_ips_proto_connection_error(ipsaddr,
				"ERR_CHK_RDMA", "before new REQ", 0, 1);
			psmi_assert(ipsaddr->verbs.err_chk_rdma_rcnt
					!= ipsaddr->reconnect_count);
		}

		if (psm3_verbs_have_rc_qp(ipsaddr, ipsaddr->verbs.err_chk_rdma_rcnt)) {
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] need recheck: still have RC QP rcnt %u\n",
				ipsaddr, ipsaddr->verbs.err_chk_rdma_rcnt);
			// rc_qp still exists, must delay checking for RDMA done.
			ipsaddr->verbs.err_chk_rdma_recheck = 1;
			return PSM2_OK_NO_PROGRESS;
		}
		// QP is gone and we did not yet get RDMA completion so need resend
		tidrecvc->stats.nReXmit++;
		ipsaddr->verbs.err_chk_rdma_resp_need_resend = 1;
		_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] need resend\n", ipsaddr);
#endif /* PSM_RC_RECONNECT */
	} else {
		tidrecvc->stats.nReXmit++;
		ipsaddr->verbs.err_chk_rdma_resp_need_resend = 1;
		_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] need resend\n", ipsaddr);
	}

	ipsaddr->verbs.err_chk_rdma_recheck_timeout_secs = 0;
	return PSM2_OK;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
// parse ERR_CHK_RDMA, stash info in ipsaddr->verbs.*err_chk* and attempt to process
// and respond now if can
int ips_protoexp_process_err_chk_rdma(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_proto *proto = rcv_ev->proto;
	struct ips_protoexp *protoexp = proto->protoexp;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	ptl_arg_t rdesc_id = p_hdr->data[0];
	ptl_arg_t sdesc_id = p_hdr->data[1];
	struct ips_tid_recv_desc *tidrecvc;
	psmi_assert(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
	struct ips_flow *flow = &ipsaddr->flows[proto->msgflowid];

	PSM2_LOG_MSG("entering");
	_HFI_MMDBG("ips_protoexp_process_err_chk_rdma\n");

	/* normal packet reliabilty protocol handling */
	if (!ips_proto_is_expected_or_nak(rcv_ev))
		goto done;

	/* processing specific to err chk rdma packet */
	proto->epaddr_stats.err_chk_rdma_recv++;

	_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] received ERR_CHK_RDMA rdescid._desc_idx: %d tidrecvc %p\n",
		ipsaddr, rdesc_id._desc_idx,
		&protoexp->tfc.tidrecvc[rdesc_id._desc_idx]);
	PSM2_LOG_EPM(OPCODE_ERR_CHK_RDMA,PSM2_LOG_RX,ipsaddr->epaddr.epid,
			proto->ep->epid,
			"rdescid._desc_genc %d _desc_idx: %d, sdescid._desc_idx: %d",
			rdesc_id._desc_genc,rdesc_id._desc_idx, sdesc_id._desc_idx);

	if (ipsaddr->verbs.need_send_err_chk_rdma_resp) {
		/* sender has >1 err chk rdma outstanding: protocol violation */
		psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"process_err_chk_rdma: Protocol Violation: > 1 outstanding from remote node %s on %s\n",
			psm3_epaddr_get_name(ipsaddr->epaddr.epid, 0),
			proto->ep->dev_name);
		goto do_acks;
	}

	/* Get receive descriptor */
	psmi_assert(rdesc_id._desc_idx < HFI_TF_NFLOWS);
	tidrecvc = &protoexp->tfc.tidrecvc[rdesc_id._desc_idx];

	tidrecvc->stats.nErrChkReceived++;

	// stash information to build resp in ipsaddr
	psmi_assert(! ipsaddr->verbs.need_send_err_chk_rdma_resp);
	ipsaddr->verbs.need_send_err_chk_rdma_resp = 1;
	ipsaddr->verbs.err_chk_rdma_sendtoken = p_hdr->mdata;
	ipsaddr->verbs.err_chk_rdma_resp_rdesc_id = rdesc_id;
	ipsaddr->verbs.err_chk_rdma_resp_sdesc_id = sdesc_id;
#ifdef PSM_RC_RECONNECT
	if (ipsaddr->allow_reconnect) {
		if (! ips_recvhdrq_event_paylen(rcv_ev)) {
			psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"process_err_chk_rdma: Protocol Violation: reconnect_count omitted from remote node %s on %s\n",
				psm3_epaddr_get_name(ipsaddr->epaddr.epid, 0),
				proto->ep->dev_name);
			goto do_acks;
		} else {
			// for forward compatibility, ignore extra bytes
			ipsaddr->verbs.err_chk_rdma_rcnt = *(uint8_t*)ips_recvhdrq_event_payload(rcv_ev);
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] rcnt %u\n",
				ipsaddr, ipsaddr->verbs.err_chk_rdma_rcnt);
		}
	} else {
		// for forward compatibility, ignore payload
		ipsaddr->verbs.err_chk_rdma_rcnt = 0;
	}
#endif

	if (PSM2_OK != ips_protoexp_do_err_chk_rdma(ipsaddr)) {
		// Need to wait for RC QP CQE to drain before know proper
		// ERR_CHK_RDMA_RESP to compose, so we will schedule a timer
		// to recheck in near future
		psmi_assert(ipsaddr->verbs.err_chk_rdma_recheck);
		// allow up to 10 seconds for QP to drain
		// we only keep a precision of seconds to save space
		ipsaddr->verbs.err_chk_rdma_recheck_timeout_secs =
			(cycles_to_nanosecs(get_cycles()) / NSEC_PER_SEC) + 10;
		// timer_request is a noop if already scheduled.
		// Only this code schedules timer_getreqs with a delay, so
		// no harm except we may delay timer callback if a future
		// scb_avail tries to schedule timer for immediate execution
		_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] schedule timer\n", ipsaddr);
		psmi_timer_request(protoexp->timerq, &protoexp->timer_getreqs,
			get_cycles() + nanosecs_to_cycles(VERBS_QP_DRAIN_DELAY_USEC * NSEC_PER_USEC));
		STAILQ_INSERT_TAIL(&protoexp->pend_err_resp, ipsaddr, verbs.pend_err_resp_next);
	} else {
		psmi_assert(! ipsaddr->verbs.err_chk_rdma_recheck);
		psmi_assert(! ipsaddr->verbs.err_chk_rdma_recheck_timeout_secs);
		// try to send it now, will remain "queued" until we can send
		ips_protoexp_send_err_chk_rdma_resp(flow);
		if (ipsaddr->verbs.need_send_err_chk_rdma_resp) {
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] queue til scb avail\n",
				ipsaddr);
			// ips_tid_scbavail_callback will trigger pend_err_resp
			// again and call ips_tid_pendtids_timer_callback
			STAILQ_INSERT_TAIL(&protoexp->pend_err_resp, ipsaddr, verbs.pend_err_resp_next);
		} else {
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] sent ERR_CHK_RESP\n",
				ipsaddr);
		}
	}

do_acks:
	if (__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ)
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq,
					&ipsaddr->flows[ips_proto_flowid(p_hdr)]);

	psm3_ips_proto_process_ack(rcv_ev);
done:
	PSM2_LOG_MSG("leaving");
	return IPS_RECVHDRQ_CONTINUE;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
static
void ips_protoexp_send_err_chk_rdma_resp(struct ips_flow *flow)
{
	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	struct ips_proto *proto = ipsaddr->epaddr.proto;
	struct ips_protoexp *protoexp = proto->protoexp;
	ips_scb_t *scb;

	PSM2_LOG_MSG("entering");
	_HFI_MMDBG("ips_protoexp_send_err_chk_rdma_resp\n");
	psmi_assert(ipsaddr->verbs.need_send_err_chk_rdma_resp);
	scb = psm3_ips_scbctrl_alloc(&protoexp->tid_scbc_rv, 1, 0,
				     IPS_SCB_FLAG_PRIORITY);
	if (scb == NULL) {
		/* ips_tid_scbavail_callback() will reschedule */
		return;
	}

	_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] sending ERR_CHK_RDMA_RESP need_resend %d rdescid. _desc_genc %d _desc_idx: %d, sdescid._desc_idx: %d\n",
		ipsaddr, ipsaddr->verbs.err_chk_rdma_resp_need_resend,
		ipsaddr->verbs.err_chk_rdma_resp_rdesc_id._desc_genc,
		ipsaddr->verbs.err_chk_rdma_resp_rdesc_id._desc_idx,
		ipsaddr->verbs.err_chk_rdma_resp_sdesc_id._desc_idx);
	PSM2_LOG_EPM(OPCODE_ERR_CHK_RDMA,PSM2_LOG_TX, proto->ep->epid,
			ipsaddr->epaddr.epid,
			"need_resend %d rdescid. _desc_genc %d _desc_idx: %d, sdescid._desc_idx: %d",
			ipsaddr->verbs.err_chk_rdma_resp_need_resend,
			ipsaddr->verbs.err_chk_rdma_resp_rdesc_id._desc_genc,
			ipsaddr->verbs.err_chk_rdma_resp_rdesc_id._desc_idx,
			ipsaddr->verbs.err_chk_rdma_resp_sdesc_id._desc_idx);

	ips_scb_opcode(scb) = OPCODE_ERR_CHK_RDMA_RESP;
	scb->ips_lrh.khdr.kdeth0 = 0;
	scb->ips_lrh.mdata = ipsaddr->verbs.err_chk_rdma_resp_need_resend;
	scb->ips_lrh.data[0] = ipsaddr->verbs.err_chk_rdma_resp_rdesc_id;
	scb->ips_lrh.data[1] = ipsaddr->verbs.err_chk_rdma_resp_sdesc_id;
	/* path is having issue, ask for ack */
	scb->scb_flags |= IPS_SEND_FLAG_ACKREQ;
	/* INTR makes sure remote end works on it immediately */
	if (proto->flags & IPS_PROTO_FLAG_RCVTHREAD)
		scb->scb_flags |= IPS_SEND_FLAG_INTR;

	// The scb will own reliable transmission of resp, we can clear flag
	ipsaddr->verbs.need_send_err_chk_rdma_resp = 0;

	proto->epaddr_stats.err_chk_rdma_resp_send++;

	psm3_ips_proto_flow_enqueue(flow, scb);
	flow->flush(flow, NULL);

	PSM2_LOG_MSG("leaving");
	return;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
int ips_protoexp_process_err_chk_rdma_resp(struct ips_recvhdrq_event *rcv_ev)
{
	struct ips_protoexp *protoexp = rcv_ev->proto->protoexp;
	struct ips_message_header *p_hdr = rcv_ev->p_hdr;
	ips_epaddr_t *ipsaddr = rcv_ev->ipsaddr;
	struct ips_tid_send_desc *tidsendc;
	uint32_t need_resend = p_hdr->mdata;
	//ptl_arg_t rdesc_id = p_hdr->data[0];
	ptl_arg_t sdesc_id = p_hdr->data[1];

	PSM2_LOG_MSG("entering");
	_HFI_MMDBG("ips_protoexp_process_err_chk_rdma_resp\n");

	/* normal packet reliabilty protocol handling */
	if (!ips_proto_is_expected_or_nak(rcv_ev))
		goto done;

	/* processing specific to err chk rdma resp packet */

	protoexp->proto->epaddr_stats.err_chk_rdma_resp_recv++;

	PSM2_LOG_EPM(OPCODE_ERR_CHK_RDMA,PSM2_LOG_RX,ipsaddr->epaddr.epid,
			protoexp->proto->ep->epid,
			"rdescid. _desc_genc %d _desc_idx: %d, sdescid._desc_idx: %d",
			p_hdr->data[0]._desc_genc,p_hdr->data[0]._desc_idx,
			sdesc_id._desc_idx);
	/* Get the session send descriptor
	 * a subset of get_tidflow in ips_proto_recv.c since we don't
	 * have tidflow sequence numbers to check
	 */
	tidsendc = (struct ips_tid_send_desc *)
		psm3_mpool_find_obj_by_index(protoexp->tid_desc_send_pool,
					sdesc_id._desc_idx);
	_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] received ERR_CHK_RDMA_RESP tidsendc %p\n",
		tidsendc?tidsendc->ipsaddr:NULL, tidsendc);
	_HFI_VDBG("desc_id=%d (%p)\n", sdesc_id._desc_idx, tidsendc);
	if (tidsendc == NULL) {
		_HFI_ERROR("err_chk_rdma_resp: Index %d is out of range\n",
					sdesc_id._desc_idx);
		goto do_acks;
	} else {
		ptl_arg_t desc_tidsendc;

		psm3_mpool_get_obj_index_gen_count(tidsendc,
						 &desc_tidsendc._desc_idx, &desc_tidsendc._desc_genc);

		_HFI_VDBG("sdesc_req:id=%d,gen=%d desc_sendc:id=%d,gen=%d\n",
				sdesc_id._desc_idx, sdesc_id._desc_genc,
				desc_tidsendc._desc_idx, desc_tidsendc._desc_genc);

		/* See if the reference is still live and valid */
		if (desc_tidsendc.u64 != sdesc_id.u64) {
			_HFI_ERROR("err_chk_rdma_resp: Genc %d does not match\n",
				sdesc_id._desc_genc);
			goto do_acks;
		}
	}

	ipsaddr->verbs.err_chk_rdma_outstanding = 0;
	tidsendc->need_err_chk_rdma = 0;
#ifdef PSM_RC_RECONNECT
	tidsendc->err_chk_rdma_rcnt = 0;	// for sanity, clearing is optional
	psmi_assert(! tidsendc->rc_qp);
#endif
	if (need_resend)
		ips_tid_reissue_rdma_write(tidsendc);
	else
		ips_protoexp_tidsendc_complete(tidsendc);

do_acks:
	if (__be32_to_cpu(p_hdr->bth[2]) & IPS_SEND_FLAG_ACKREQ)
		ips_proto_send_ack((struct ips_recvhdrq *)rcv_ev->recvq,
					&ipsaddr->flows[ips_proto_flowid(p_hdr)]);

	psm3_ips_proto_process_ack(rcv_ev);
done:
	PSM2_LOG_MSG("leaving");
	return IPS_RECVHDRQ_CONTINUE;
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA
// Upon completion of an RDMA Write, a completion is delivered with
// immediate data.  The immediate data is used
// to indicate the completed receive of the RDMA Write.
// If we use RDMA Read, the local SQ Completion will indicate this.
// could build and pass a ips_recvhdrq_event or pass struct ips_recvhdrq
// but all we really need is proto and len
// conn indicates where we received RDMA Write, just for quick sanity check
// 	for RV module conn will be the psm3_rv_conn_t
// 	for user RC QPs conn will be will be the RC struct ibv_qp*
int ips_protoexp_handle_immed_data(struct ips_proto *proto, uint64_t conn_ref,
									int conn_type, uint32_t immed, uint32_t len)
{
	struct ips_tid_recv_desc *tidrecvc;
	struct ips_protoexp *protoexp = proto->protoexp;
	ptl_arg_t desc_id;
	PSM2_LOG_MSG("entering");
	desc_id._desc_genc = RDMA_UNPACK_IMMED_GENC(immed);
	desc_id._desc_idx = RDMA_UNPACK_IMMED_IDX(immed);

	tidrecvc = &protoexp->tfc.tidrecvc[desc_id._desc_idx];
	_HFI_MMDBG("ips_protoexp_immed_data tidrecvc %p\n", tidrecvc);

	if ((tidrecvc->rdescid._desc_genc & IPS_HDR_RDESCID_GENC_MASK)
		!= desc_id._desc_genc) {
		_HFI_ERROR("stale inbound rv RDMA generation: expected %u got %u\n",
				tidrecvc->rdescid._desc_genc, desc_id._desc_genc);
		tidrecvc->stats.nGenErr++;
		PSM2_LOG_MSG("leaving");
		return IPS_RECVHDRQ_CONTINUE;		/* skip */
	}

	// maybe should use assert below so don't add test in production code
	if (tidrecvc->state != TIDRECVC_STATE_BUSY) {
		_HFI_ERROR("stale inbound rv RDMA (tidrecvc not busy)\n");
		PSM2_LOG_MSG("leaving");
		return IPS_RECVHDRQ_CONTINUE;		/* skip */
	}
	// some sanity checks
	// maybe this should be an assert so don't add test in production code
	if (len != tidrecvc->recv_msglen) {
		// RDMA Write does not match what we asked for in CTS
		_HFI_ERROR("incorrect RDMA Write Len: expected %u got %u\n",
				tidrecvc->recv_msglen, len);
		// TBD - what to do?
	}
	psmi_assert(IPS_PROTOEXP_FLAG_ENABLED & tidrecvc->protoexp->proto->ep->rdmamode);
#ifdef RNDV_MOD
	if (conn_type == RDMA_IMMED_RV
		&& RDMA_UNPACK_IMMED_RV_IDX(immed) != proto->ep->verbs_ep.rv_index) {
		// RV module should not have delivered this CQE to us
		_HFI_ERROR("incorrect RDMA RV Index: expected %u got %u\n",
				proto->ep->verbs_ep.rv_index, RDMA_UNPACK_IMMED_RV_IDX(immed));
		return IPS_RECVHDRQ_CONTINUE;		/* skip */
	}
#endif
#if defined(USE_RC)
	// For User RC conn_ref is context we set in rc_qp_create (*ipsaddr)
	// For Kernel RC, conn_ref is the conn handle (psm3_rv_conn_get_conn_handle)
	// maybe this should be an assert so don't add test in production code
	// caller can't get qp_context (conn_ref) from rbuf_qp for SRQ
	if ((conn_type == RDMA_IMMED_USER_RC)
			&& ! proto->ep->verbs_ep.srq
			&& (uint64_t)tidrecvc->ipsaddr != conn_ref) {
		// RDWA Write is not on expected RC QP from remote node
		_HFI_ERROR("RDMA Write on Wrong User QP 0x%"PRIx64", expect 0x%"PRIx64"\n",
				 	conn_ref, (uint64_t)tidrecvc->ipsaddr);
		// TBD - what to do?
	}
#endif
#ifdef RNDV_MOD
	if (conn_type == RDMA_IMMED_RV
		&& psm3_rv_conn_get_conn_handle(tidrecvc->ipsaddr->verbs.rv_conn)
					 != conn_ref) {
		// RDWA Write is not on expected RV QP from remote node
		_HFI_ERROR("RDMA Write on Wrong RV QP 0x%"PRIx64", expect 0x%"PRIx64"\n",
				 	conn_ref,
		 			psm3_rv_conn_get_conn_handle(tidrecvc->ipsaddr->verbs.rv_conn));
		// TBD - what to do?
	}
#endif
	if (_HFI_PDBG_ON) {
#ifdef PSM_HAVE_GPU
		if (tidrecvc->is_ptr_gpu_backed)
			_HFI_PDBG_DUMP_GPU_ALWAYS(tidrecvc->buffer, len);
		else
#endif
			_HFI_PDBG_DUMP_ALWAYS(tidrecvc->buffer, len);
	}

	/* Reset the swapped generation count as we received a valid packet */
	tidrecvc->tidflow_nswap_gen = 0;

	/* Do some sanity checking */
	psmi_assert_always(tidrecvc->state == TIDRECVC_STATE_BUSY);
	// STL100 did this at the end of ips_protoexp_send_tid_completion
	// TBD - seems like this should be done after ips_tid_recv_free
	// so we have more likelihood of getting freshly freed resources?
	if (tidrecvc->protoexp->tid_flags & IPS_PROTOEXP_FLAG_CTS_SERIALIZED) {
		tidrecvc->ipsaddr->flows[protoexp->proto->msgflowid].flags &= ~IPS_FLOW_FLAG_SKIP_CTS;                                  /* Let the next CTS be processed */
		ips_tid_pendtids_timer_callback(&tidrecvc->protoexp->timer_getreqs, 0);  /* and make explicit progress for it. */
	}

		/* Mark receive as done */
		ips_tid_recv_free(tidrecvc);
		_HFI_MMDBG("tidrecv done tidrecvc %p\n", tidrecvc);
	PSM2_LOG_MSG("leaving");

	return IPS_RECVHDRQ_CONTINUE;
}
#endif // PSM_HAVE_RDMA
#endif /* PSM_VERBS */



#ifdef PSM_HAVE_GPU
static
psm2_error_t
psmi_gpu_reclaim_hostbufs(struct ips_tid_get_request *getreq)
{
	struct ips_protoexp *protoexp = getreq->tidgr_protoexp;
	struct ips_tid_getreq_gpu_hostbuf_pend *cmemcpyhead =
		&getreq->pend_gpubuf;
	struct ips_gpu_hostbuf *chb;

	/* Get the getreq's first memcpy op */
	while (!STAILQ_EMPTY(cmemcpyhead)) {
		chb = STAILQ_FIRST(cmemcpyhead);
		if (!PSM3_GPU_MEMCPY_DONE(chb)) {
			/* At least one of the copies is still
			 * in progress. Schedule the timer,
			 * then leave the GPU progress phase
			 * and check for other pending TID work.
			 */
			psmi_timer_request(protoexp->timerq,
					   &protoexp->timer_getreqs,
					   PSMI_TIMER_PRIO_1);
			return PSM2_OK_NO_PROGRESS;
		}
		/* The getreq's oldest cudabuf is done. Reclaim it. */
		getreq->tidgr_gpu_bytesdone += chb->size;
		STAILQ_REMOVE_HEAD(cmemcpyhead, next);
		PSM3_GPU_HOSTBUF_RESET(chb);
		psm3_mpool_put(chb);
	}
	return PSM2_OK;
}

// allocate a chb control structure.  The actual buffer and event needed for the
// DTOH async copy are allocated in chb's 1st use in PSM3_GPU_MEMCPY_DTOH_START
struct ips_gpu_hostbuf* psm3_ips_allocate_send_chb(struct ips_proto *proto,
				uint32_t nbytes, int allow_temp)
{
	struct ips_gpu_hostbuf* chb = NULL;
	unsigned bufsz;

	if (nbytes <= GPU_SMALLHOSTBUF_SZ) {
		chb = (struct ips_gpu_hostbuf *) psm3_mpool_get(
			proto->gpu_hostbuf_pool_small_send);
		bufsz = proto->gpu_hostbuf_small_send_cfg.bufsz;
	}
	if (chb == NULL) {
		chb = (struct ips_gpu_hostbuf *) psm3_mpool_get(
			proto->gpu_hostbuf_pool_send);
		bufsz = proto->gpu_hostbuf_send_cfg.bufsz;
	}

	/* were any buffers available? If not force allocate */
	if (chb == NULL && allow_temp) {
		chb = (struct ips_gpu_hostbuf*) psmi_calloc(PSMI_EP_NONE,
							    UNDEFINED, 1,
							    sizeof(struct ips_gpu_hostbuf));
		if_pf (chb == NULL) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
							"Couldn't allocate GPU host bounce buffers ");
			return NULL;
		}
		chb->is_tempbuf = 1;
		bufsz = nbytes;
	}
	if (chb && ! chb->host_buf)
		PSM3_GPU_HOST_ALLOC(&chb->host_buf, bufsz);
	return chb;
}

static
void psmi_gpu_run_prefetcher(struct ips_protoexp *protoexp,
			     struct ips_tid_send_desc *tidsendc)
{
	struct ips_proto *proto = protoexp->proto;
	struct ips_gpu_hostbuf *chb = NULL;
	psm2_mq_req_t req = tidsendc->mqreq;
	uint32_t offset, window_len;

	/* try to push the prefetcher forward */
	if (req->prefetch_send_msgoff < req->req_data.send_msglen) {
		/* some data remains to be sent */
		offset = req->prefetch_send_msgoff;
		psmi_assert(req->is_buf_gpu_mem);
		window_len =
			ips_gpu_next_window(
					     psm3_mq_get_window_rv(req),
					     offset, req->req_data.buf_len);
		chb = psm3_ips_allocate_send_chb(proto, window_len, 0);
		/* were any buffers available for the prefetcher? */
		if (chb == NULL)
			return;
		req->prefetch_send_msgoff += window_len;
		chb->offset = offset;
		chb->size = window_len;
		chb->req = req;
		chb->gpu_buf = (uint8_t*)req->req_data.buf + offset;
		chb->bytes_read = 0;

		PSM3_GPU_MEMCPY_DTOH_START(proto, chb, window_len);

		STAILQ_INSERT_TAIL(&req->sendreq_prefetch, chb, req_next);
		return;
	}
	return;
}

static
void psmi_attach_chb_to_tidsendc(struct ips_protoexp *protoexp,
				 psm2_mq_req_t req,
				 struct ips_tid_send_desc *tidsendc,
				 struct ips_gpu_hostbuf *chb_prev,
				 uint32_t tsess_srcoff,
				 uint32_t tsess_length,
				 psm2_chb_match_type_t type)
{
	struct ips_proto *proto = protoexp->proto;
	struct ips_gpu_hostbuf *chb = NULL;
	uint32_t offset, window_len, attached=0;

	/* try to push the prefetcher forward */
	while (req->prefetch_send_msgoff < tsess_srcoff + tsess_length) {
		/* some data remains to be sent */
		offset = req->prefetch_send_msgoff;
		psmi_assert(req->is_buf_gpu_mem);
		window_len =
			ips_gpu_next_window(
					     psm3_mq_get_window_rv(req),
					     offset, req->req_data.buf_len);
		/* if no buffers available, force allocate of a temp buf */
		chb = psm3_ips_allocate_send_chb(proto, window_len, 1);
		req->prefetch_send_msgoff += window_len;
		chb->offset = offset;
		chb->size = window_len;
		chb->req = req;
		chb->gpu_buf = (uint8_t*)req->req_data.buf + offset;
		chb->bytes_read = 0;

		PSM3_GPU_MEMCPY_DTOH_START(proto, chb, window_len);

		STAILQ_INSERT_TAIL(&req->sendreq_prefetch, chb, req_next);
		if (type == PSMI_GPU_PARTIAL_MATCH_FOUND) {
			// caller matched 1st chb, but needed more prefetched
			// see if we have what we need now
			if ((tsess_srcoff < chb->offset)
			     && ((tsess_srcoff + tsess_length) > chb->offset)) {
				// will collect the 2 prefetched chb's for this
				// RDMA Write send into a single CPU temp buffer
				// do alloc now, hoping to hide it behind GPU async copy to chb
				tidsendc->gpu_hostbuf[0] = chb_prev;
				tidsendc->gpu_hostbuf[1] = chb;
				tidsendc->gpu_num_buf = 2;
				tidsendc->gpu_split_buf = psm3_ips_allocate_send_chb(proto,
											tsess_length, 1);
				tidsendc->userbuf =
					(void *)((uintptr_t) tidsendc->gpu_split_buf->host_buf);
				tidsendc->buffer =
					(void *)((uintptr_t)tidsendc->userbuf);
				return;
			}
		} else {
			if (attached) {
				// we attached one in prior loop, now have
				// a second, should have what we need now
				psmi_assert((tsess_srcoff + tsess_length) > chb->offset);
				// will collect the 2 prefetched chb's for this
				// RDMA Write send into a single CPU temp buffer
				// do alloc now, hoping to hide it behind GPU async copy to chb
				tidsendc->gpu_hostbuf[0] = chb_prev;
				tidsendc->gpu_hostbuf[1] = chb;
				tidsendc->gpu_num_buf = 2;
				tidsendc->gpu_split_buf = psm3_ips_allocate_send_chb(proto,
											tsess_length, 1);
				tidsendc->userbuf =
					(void *)((uintptr_t) tidsendc->gpu_split_buf->host_buf);
				tidsendc->buffer =
					(void *)((uintptr_t)tidsendc->userbuf);
				return;
			}
			if ((tsess_srcoff > chb->offset)
			    && (tsess_srcoff < (chb->offset + chb->size))
			     && ((tsess_srcoff + tsess_length) > (chb->offset + chb->size))) {
				// we prefetched one, but need another
				chb_prev = chb;
				attached = 1;
				continue;
			} else if ((chb->offset <= tsess_srcoff) &&
				  ((tsess_srcoff + tsess_length) <=
				   (chb->offset+chb->size))) {
				// we prefetched one and have what we need
				tidsendc->gpu_hostbuf[0] = chb;
				tidsendc->gpu_hostbuf[1] = NULL;
				tidsendc->gpu_num_buf = 1;
				tidsendc->userbuf =
					(void *)((uintptr_t) chb->host_buf +
						tsess_srcoff - chb->offset);
				tidsendc->buffer =
					(void *)((uintptr_t)tidsendc->userbuf);
				return;
			}
		}
	}
}

static
psm2_chb_match_type_t psmi_find_match_in_prefeteched_chb(struct ips_gpu_hostbuf* chb,
				       ips_tid_session_list *tid_list,
				       uint32_t prefetch_send_msgoff)
{
	/* To get a match:
	 * 1. Tid list offset + length is contained within a chb
	 * 2. Tid list offset + length is contained within
	 * the prefetched offset of this req.
	 * 3. Tid list offset + length is partially prefetched
	 * within one chb. (A partial match)
	 */
	if (chb->offset <= tid_list->tsess_srcoff) {
		if ((chb->offset + chb->size) >=
		    (tid_list->tsess_srcoff + tid_list->tsess_length)) {
			return PSMI_GPU_FULL_MATCH_FOUND;
		} else {
			if((chb->offset + chb->size) > tid_list->tsess_srcoff){
				if(((chb->offset + (2 * chb->size)) >
				   (tid_list->tsess_srcoff + tid_list->tsess_length)) &&
						  ((prefetch_send_msgoff) >=
						   (tid_list->tsess_srcoff + tid_list->tsess_length))){
					return PSMI_GPU_SPLIT_MATCH_FOUND;
				} else if((tid_list->tsess_srcoff + tid_list->tsess_length)
					> prefetch_send_msgoff) {
					return PSMI_GPU_PARTIAL_MATCH_FOUND;
				}
			}
		}
	}
	return PSMI_GPU_CONTINUE;
}
#endif

// sender handling of a CTS which indicates use of TID protocol
psm2_error_t
psm3_ips_tid_send_handle_tidreq(struct ips_protoexp *protoexp,
			   ips_epaddr_t *ipsaddr,
			   psm2_mq_req_t req,
			   ptl_arg_t rdescid,
			   uint32_t tidflow_genseq,
			   ips_tid_session_list *tid_list,
			   uint32_t tid_list_size)
{
	struct ips_tid_send_desc *tidsendc;
	_HFI_MMDBG("psm3_ips_tid_send_handle_tidreq\n");

	PSM2_LOG_MSG("entering");
	psmi_assert(tid_list_size == sizeof(ips_tid_session_list));

	tidsendc = (struct ips_tid_send_desc *)
	    psm3_mpool_get(protoexp->tid_desc_send_pool);
	if (tidsendc == NULL) {
		_HFI_MMDBG("failed to alloc tidsendc\n");
		PSM2_LOG_MSG("leaving");
		ips_logevent(protoexp->proto, tid_send_reqs, ipsaddr);
		return PSM2_EP_NO_RESOURCES;
	}

	// mpool_get does not zero memory, so we must clear or initialize all fields
	// we care about
	req->ptl_req_ptr = (void *)tidsendc;
	tidsendc->protoexp = protoexp;

	/* Uniquely identify this send descriptor in space and time */
	tidsendc->sdescid._desc_idx = psm3_mpool_get_obj_index(tidsendc);
	tidsendc->sdescid._desc_genc = psm3_mpool_get_obj_gen_count(tidsendc);
	tidsendc->rdescid = rdescid;
	tidsendc->ipsaddr = ipsaddr;
	tidsendc->mqreq = req;

	/* Copy received tidinfo to local tidsendc buffer. */
	psm3_mq_mtucpy_host_mem(&tidsendc->tid_list, tid_list,
				sizeof(ips_tid_session_list));
#if defined(PSM_SOCKETS) && PSMI_HAL_INST_CNT == 1
	psmi_assert_always(0);	// should not get here
#elif defined(PSM_VERBS)
	// for verbs we do not need a ips_flow since we will use the RC QP and
	// then will use our main flow for the final RV completion control msg
	// The path record for use by RDMA will be selected when the connection
	// is established
	tidsendc->mr = NULL;	// be safe,but should be NULL since clear on release
	_HFI_VDBG("recv'd CTS: rkey 0x%x srcoff %u raddr 0x%"PRIx64" len %u\n",
		tid_list->tsess_rkey, tid_list->tsess_srcoff, tid_list->tsess_raddr,
		tid_list->tsess_length);
#endif // defined(PSM_VERBS)

	tidsendc->userbuf =
	    (void *)((uintptr_t) req->req_data.buf + tid_list->tsess_srcoff);
	tidsendc->buffer = (void *)((uintptr_t)tidsendc->userbuf
				);
	tidsendc->length = tid_list->tsess_length;
	_HFI_MMDBG("tidsendc created %p rkey 0x%x userbuf %p buffer %p length %u\n",
			tidsendc, tid_list->tsess_rkey,
			tidsendc->userbuf,  tidsendc->buffer,
			tidsendc->length);

#ifdef PSM_HAVE_GPU
	/* Matching on previous prefetches and initiating next prefetch */
	struct ips_gpu_hostbuf *chb = NULL, *chb_next = NULL;
	psm2_chb_match_type_t rc = PSMI_GPU_CONTINUE;

	/* check if the prefetcher has a buffer ready to use */
	tidsendc->gpu_hostbuf[0] = NULL;
	tidsendc->gpu_hostbuf[1] = NULL;
	tidsendc->gpu_num_buf = 0;
	if (req->gpu_hostbuf_used) {
		/* To get a match:
		 * 1. FULL - Tid list offset + length is contained within a chb
		 * 2. SPLIT - Tid list offset + length is contained within
		 * the prefetched offset of this req. (2 chb)
		 * 3. PARTIAL - Tid list offset + length is partially prefetched
		 * within one chb.
		 */
		STAILQ_FOREACH(chb, &req->sendreq_prefetch, req_next) {
			rc = psmi_find_match_in_prefeteched_chb(chb,
								tid_list,
								req->prefetch_send_msgoff);
			if (rc < PSMI_GPU_CONTINUE)
				break;
		}
		if (rc == PSMI_GPU_FULL_MATCH_FOUND) {
			tidsendc->userbuf =
				(void *)((uintptr_t) chb->host_buf+
					 tid_list->tsess_srcoff - chb->offset);
			tidsendc->buffer =
				(void *)((uintptr_t)tidsendc->userbuf
					);
			/* now associate the buffer with the tidsendc */
			tidsendc->gpu_hostbuf[0] = chb;
			tidsendc->gpu_hostbuf[1] = NULL;
			tidsendc->gpu_num_buf = 1;
		} else if (rc == PSMI_GPU_SPLIT_MATCH_FOUND){
			// will collect the 2 prefetched chb's for this
			// RDMA Write send into a single CPU temp buffer
			// do alloc now, hoping to hide it behind GPU async copy to chb
			tidsendc->gpu_split_buf =psm3_ips_allocate_send_chb(protoexp->proto,
										tid_list->tsess_length, 1);
			tidsendc->userbuf =
				(void *)((uintptr_t) tidsendc->gpu_split_buf->host_buf);
			tidsendc->buffer =
				(void *)((uintptr_t)tidsendc->userbuf
				);
			chb_next = STAILQ_NEXT(chb, req_next);
			tidsendc->gpu_hostbuf[0] = chb;
			tidsendc->gpu_hostbuf[1] = chb_next;
			tidsendc->gpu_num_buf = 2;
		} else if (rc == PSMI_GPU_PARTIAL_MATCH_FOUND) {
			// need to prefetch more
			psmi_attach_chb_to_tidsendc(protoexp, req,
						    tidsendc,
						    chb,
						    tid_list->tsess_srcoff,
						    tid_list->tsess_length,
						    rc);
		} else {
			// no match, need to prefetch
			psmi_attach_chb_to_tidsendc(protoexp, req,
						    tidsendc,
						    NULL,
						    tid_list->tsess_srcoff,
						    tid_list->tsess_length,
						    PSMI_GPU_CONTINUE);
		}
		protoexp->proto->strat_stats.rndv_rdma_hbuf_send++;
		protoexp->proto->strat_stats.rndv_rdma_hbuf_send_bytes += tid_list->tsess_length;
	} else if (req->is_buf_gpu_mem) {
		protoexp->proto->strat_stats.rndv_rdma_gdr_send++;
		protoexp->proto->strat_stats.rndv_rdma_gdr_send_bytes += tid_list->tsess_length;
	} else
#endif /* PSM_HAVE_GPU */
	{
		protoexp->proto->strat_stats.rndv_rdma_cpu_send++;
		protoexp->proto->strat_stats.rndv_rdma_cpu_send_bytes += tid_list->tsess_length;
	}

	tidsendc->is_complete = 0;
	tidsendc->reserved = 0;

#ifdef PSM_HAVE_RDMA_ERR_CHK
	tidsendc->need_err_chk_rdma = 0;
#ifdef PSM_RC_RECONNECT
	tidsendc->rc_qp = NULL;
	tidsendc->err_chk_rdma_rcnt = 0;	// for sanity, clearing is optional
#endif
#endif
#if defined(PSM_HAVE_RDMA) && defined(RNDV_MOD)
	tidsendc->rv_sconn_index = 0;
	tidsendc->rv_conn_count = 0;
#endif

	_HFI_EXP
	    ("alloc tidsend=%4d tidrecv=%4d srcoff=%6d length=%6d"
		"\n",
	     tidsendc->sdescid._desc_idx, rdescid._desc_idx,
	     tid_list->tsess_srcoff, tid_list->tsess_length
		);

	// start sending RDMA packets
	ips_tid_send_exp(tidsendc);

	/* Add as a pending op and ring up the timer */
	if (tidsendc->is_complete == 0) {
		STAILQ_INSERT_TAIL(&protoexp->pend_sendq, tidsendc, next);
		psmi_timer_request(protoexp->timerq, &protoexp->timer_send,
			   PSMI_TIMER_PRIO_1);
	}

	PSM2_LOG_MSG("leaving");
	/* Consider breaking out of progress engine here */
	return PSM2_OK;
}


#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA
/*
 * Returns:
 *
 * PSM2_OK: scb was allocated for at least one frame, the packet may be queued
 *         or actually sent.
 *
 * PSM2_OK_NO_PROGRESS: Reached a limit on the maximum number of sends we allow
 *		       to be enqueued before polling receive queue.
 *
 * PSM2_EP_NO_RESOURCES: No scbs, available, a callback will be issued when more
 *                      scbs become available.
 *
 * PSM2_TIMEOUT: PIO-busy or DMA-busy, stop trying to send for now.
 *
 */

// issue RDMA Write in response to a CTS
static
psm2_error_t ips_tid_issue_rdma_write(struct ips_tid_send_desc *tidsendc)
{
	struct ips_protoexp *protoexp = tidsendc->protoexp;
	struct ips_proto *proto = protoexp->proto;
	psm2_error_t err = PSM2_OK;

	// for STL100 the EXPTID scbs were sent by software and had to loop
	// to get and queue scbs for the STL100 TIDFLOW protocol.
	// Once they were all posted the is_complete flag would be set.
	// For verbs we are delegating the RC Write "flow" to the NIC's RC QP
	// it will manage segmentation, sequence numbers and acks for the flow
	// so our job is done here after one call.
	// we use immediate data with the rdescid to trigger a CQE on receiver
	// so it knows when RDMA is done
	// if too many RDMA in flight retry later when next RDMA completes
	if (protoexp->proto->ep->verbs_ep.send_rdma_outstanding
				 >= protoexp->proto->ep->hfi_num_send_rdma) {
		err = PSM2_EP_NO_RESOURCES; // try again on next RDMA completion
	} else if (tidsendc->mr) {
		// registered or referenced in previous failed ips_tid_send_exp attempt
		// no need to register again
		err = PSM2_OK;
	} else if (
#ifdef PSM_HAVE_GPU
		! tidsendc->mqreq->gpu_hostbuf_used &&
#endif
			// separate MR cache's per EP, so this confirms we have the same EP
		tidsendc->mqreq->mr && tidsendc->mqreq->mr->cache == proto->mr_cache) {
		// we can use the same MR as the whole mqreq
		_HFI_MMDBG("CTS send chunk reference send: %p buf %p %u bytes via %p %"PRIu64"\n",
			tidsendc, tidsendc->buffer, tidsendc->length,
			tidsendc->mqreq->mr->addr, tidsendc->mqreq->mr->length);
		tidsendc->mr = psm3_verbs_ref_mr(tidsendc->mqreq->mr);
	} else {
		// we need an MR for this chunk
		_HFI_MMDBG("CTS send chunk register send: %p buf %p %u bytes\n",
			tidsendc, tidsendc->buffer , tidsendc->length);
		tidsendc->mr = psm3_verbs_reg_mr(proto->mr_cache, 1,
                         tidsendc->buffer, tidsendc->length, IBV_ACCESS_RDMA
#ifdef PSM_HAVE_GPU
						| (PSM3_GPU_ADDR_SEND_MR(tidsendc->mqreq)
							?IBV_ACCESS_IS_GPU_ADDR:0)
#endif
						);
		if (! tidsendc->mr)
			err = PSM2_TIMEOUT;	/* force a resend reschedule */
	}

	// if post_send fails below, we'll try again later
	// completion handler decides how to handle any WQE/CQE errors
	_HFI_MMDBG("tidsendc %p prior to post userbuf %p buffer %p rkey 0x%x length %u err %d"
#ifdef PSM_VERBS
	" outstanding %u"
#endif
	"\n",
			tidsendc, tidsendc->userbuf,  tidsendc->buffer,
			tidsendc->tid_list.tsess_rkey,
			tidsendc->length, err
#ifdef PSM_VERBS
			, protoexp->proto->ep->verbs_ep.send_rdma_outstanding
#endif
			);
#ifdef RNDV_MOD
	if (err == PSM2_OK) {
		psmi_assert(IPS_PROTOEXP_FLAG_ENABLED & protoexp->proto->ep->rdmamode);

		if (IPS_PROTOEXP_FLAG_KERNEL_QP(protoexp->proto->ep->rdmamode)) {
			err = psm3_verbs_post_rv_rdma_write_immed(
				protoexp->proto->ep,
				tidsendc->ipsaddr->verbs.rv_conn,
				tidsendc->buffer, tidsendc->mr,
				tidsendc->tid_list.tsess_raddr, tidsendc->tid_list.tsess_rkey,
				tidsendc->tid_list.tsess_length,
				RDMA_PACK_IMMED(tidsendc->rdescid._desc_genc,
							 tidsendc->rdescid._desc_idx,
							 tidsendc->ipsaddr->verbs.remote_rv_index),
				(uintptr_t)tidsendc,
				&tidsendc->rv_sconn_index, &tidsendc->rv_conn_count);
#if defined(USE_RC) /* AND */
		} else if (IPS_PROTOEXP_FLAG_USER_RC_QP(protoexp->proto->ep->rdmamode)) {
#ifdef PSM_RC_RECONNECT
			if (! tidsendc->ipsaddr->verbs.rc_connected) {
				err = PSM2_TIMEOUT;	/* force a resend reschedule */
			} else {
				struct psm3_verbs_rc_qp *rc_qp =
				    psm3_verbs_active_rc_qp(tidsendc->ipsaddr);
				psmi_assert(rc_qp);
				psmi_assert(! tidsendc->rc_qp);
				tidsendc->rc_qp = rc_qp;
				err = psm3_verbs_post_rdma_write_immed(
					protoexp->proto->ep, rc_qp,
					tidsendc->buffer, tidsendc->mr,
					tidsendc->tid_list.tsess_raddr,
					tidsendc->tid_list.tsess_rkey,
					tidsendc->tid_list.tsess_length,
					RDMA_PACK_IMMED(tidsendc->rdescid._desc_genc,
							 tidsendc->rdescid._desc_idx, 0),
					(uintptr_t)tidsendc);
				if (err)
					tidsendc->rc_qp = NULL;
			}
#else /* PSM_RC_RECONNECT */
			err = psm3_verbs_post_rdma_write_immed(
				protoexp->proto->ep,
				tidsendc->ipsaddr->verbs.rc_qp,
				tidsendc->buffer, tidsendc->mr,
				tidsendc->tid_list.tsess_raddr, tidsendc->tid_list.tsess_rkey,
				tidsendc->tid_list.tsess_length,
				RDMA_PACK_IMMED(tidsendc->rdescid._desc_genc,
							 tidsendc->rdescid._desc_idx, 0),
				(uintptr_t)tidsendc);
#endif /* PSM_RC_RECONNECT */
#endif // defined(USE_RC)
		}
	}
	if (err == PSM2_OK) {
		if (_HFI_PDBG_ON) {
#ifdef PSM_HAVE_GPU
			if (tidsendc->mqreq->is_buf_gpu_mem && !tidsendc->mqreq->gpu_hostbuf_used)
				_HFI_PDBG_DUMP_GPU_ALWAYS(tidsendc->buffer, tidsendc->tid_list.tsess_length);
			else
#endif
				_HFI_PDBG_DUMP_ALWAYS(tidsendc->buffer, tidsendc->tid_list.tsess_length);
		}
		tidsendc->is_complete = 1;	// send queued
	} else
		_HFI_MMDBG("after posted IBV Write: err %d\n", err);

#else // RNDV_MOD
#if defined(USE_RC)
	if (err == PSM2_OK) {
		psmi_assert(IPS_PROTOEXP_FLAG_ENABLED & protoexp->proto->ep->rdmamode);
		if (IPS_PROTOEXP_FLAG_USER_RC_QP(protoexp->proto->ep->rdmamode)) {
#ifdef PSM_RC_RECONNECT
			if (! tidsendc->ipsaddr->verbs.rc_connected) {
				err = PSM2_TIMEOUT;	/* force a resend reschedule */
			} else {
				struct psm3_verbs_rc_qp *rc_qp =
				    psm3_verbs_active_rc_qp(tidsendc->ipsaddr);
				psmi_assert(rc_qp);
				psmi_assert(! tidsendc->rc_qp);
				tidsendc->rc_qp = rc_qp;
				err = psm3_verbs_post_rdma_write_immed(
					protoexp->proto->ep, rc_qp,
					tidsendc->buffer, tidsendc->mr,
					tidsendc->tid_list.tsess_raddr, tidsendc->tid_list.tsess_rkey,
					tidsendc->tid_list.tsess_length,
					RDMA_PACK_IMMED(tidsendc->rdescid._desc_genc,
							 tidsendc->rdescid._desc_idx, 0),
					(uintptr_t)tidsendc);
				if (err)
					tidsendc->rc_qp = NULL;
			}
#else /* PSM_RC_RECONNECT */
			err = psm3_verbs_post_rdma_write_immed(
				protoexp->proto->ep,
				tidsendc->ipsaddr->verbs.rc_qp,
				tidsendc->buffer, tidsendc->mr,
				tidsendc->tid_list.tsess_raddr, tidsendc->tid_list.tsess_rkey,
				tidsendc->tid_list.tsess_length,
				RDMA_PACK_IMMED(tidsendc->rdescid._desc_genc,
							 tidsendc->rdescid._desc_idx, 0),
				(uintptr_t)tidsendc);
#endif /* PSM_RC_RECONNECT */
		}
	}
	if (err == PSM2_OK) {
		if (_HFI_PDBG_ON) {
#ifdef PSM_HAVE_GPU
			if (tidsendc->mqreq->is_buf_gpu_mem && !tidsendc->mqreq->gpu_hostbuf_used)
				_HFI_PDBG_DUMP_GPU_ALWAYS(tidsendc->buffer, tidsendc->tid_list.tsess_length);
			else
#endif
				_HFI_PDBG_DUMP_ALWAYS(tidsendc->buffer, tidsendc->tid_list.tsess_length);
		}
		tidsendc->is_complete = 1;	// send queued
	} else
		_HFI_MMDBG("after posted IBV Write 2: err %d\n", err);
#endif /* defined(USE_RC) */
#endif // RNDV_MOD
	return err;
}
#endif /* PSM_HAVE_RDMA */
#endif // defined(PSM_VERBS)

/*
 * Returns:
 *
 * PSM2_OK: scb was allocated for at least one frame, the packet may be queued
 *         or actually sent.
 *
 * PSM2_OK_NO_PROGRESS: Reached a limit on the maximum number of sends we allow
 *		       to be enqueued before polling receive queue.
 *
 * PSM2_EP_NO_RESOURCES: No scbs, available, a callback will be issued when more
 *                      scbs become available.
 *
 * PSM2_TIMEOUT: PIO-busy or DMA-busy, stop trying to send for now.
 *
 */

// we got a CTS and processed it.  Now we can start sending RDMA packets.
static
psm2_error_t ips_tid_send_exp(struct ips_tid_send_desc *tidsendc)
{
	psm2_error_t err = PSM2_OK;
#ifdef PSM_HAVE_GPU
	struct ips_protoexp *protoexp = tidsendc->protoexp;
#endif

	_HFI_MMDBG("ips_tid_send_exp tidsendc %p\n", tidsendc);
#ifdef PSM_HAVE_GPU
	struct ips_gpu_hostbuf *chb, *chb_next;
	uint32_t offset_in_chb, i;
	// wait for async copies into needed prefetcher chb's to finish
	for (i = 0; i < tidsendc->gpu_num_buf; i++) {
		chb = tidsendc->gpu_hostbuf[i];
		if (chb) {
			if (!PSM3_GPU_MEMCPY_DONE(chb)) {
				err = PSM2_OK_NO_PROGRESS;
				PSM2_LOG_MSG("leaving");
				return err;
			}
		}
	}

	if (tidsendc->gpu_num_buf == 2) {
		chb = tidsendc->gpu_hostbuf[0];
		chb_next = tidsendc->gpu_hostbuf[1];
		offset_in_chb = tidsendc->tid_list.tsess_srcoff - chb->offset;
		/* Copying data from multiple prefetched
		 * host buffers into a single temp CPU bounce buffer.
		 * so can issue a single RDMA Write from the temp bounce buffer
		 */
		memcpy(tidsendc->buffer, (void *)((uintptr_t)chb->host_buf +
			offset_in_chb), chb->size-offset_in_chb);
		memcpy((void *)((uintptr_t)tidsendc->buffer + chb->size -
			offset_in_chb), chb_next->host_buf,
			tidsendc->tid_list.tsess_srcoff +
			tidsendc->tid_list.tsess_length - chb_next->offset);

		chb->bytes_read += chb->size - offset_in_chb;
		chb_next->bytes_read += tidsendc->tid_list.tsess_srcoff +
				  tidsendc->tid_list.tsess_length -
				  chb_next->offset;
		if(chb->bytes_read == chb->size) {
			STAILQ_REMOVE(&tidsendc->mqreq->sendreq_prefetch, chb,
				       ips_gpu_hostbuf, req_next);
			psm3_ips_deallocate_send_chb(chb, 1);
			psmi_gpu_run_prefetcher(protoexp, tidsendc);
		 }
		if(chb_next->bytes_read == chb_next->size) {
			STAILQ_REMOVE(&tidsendc->mqreq->sendreq_prefetch, chb_next,
				       ips_gpu_hostbuf, req_next);
			psm3_ips_deallocate_send_chb(chb_next, 1);
			psmi_gpu_run_prefetcher(protoexp, tidsendc);
		}
		/* Clean Up tidsendc ref's to split cuda hostbufs when no longer needed */
		tidsendc->gpu_num_buf = 0;
		tidsendc->gpu_hostbuf[0] = NULL;
		tidsendc->gpu_hostbuf[1] = NULL;
	}
#endif
#if   defined(PSM_VERBS) && defined(PSM_HAVE_RDMA)
	err = ips_tid_issue_rdma_write(tidsendc);
#else
	err = PSM2_INTERNAL_ERR;	// should not get here
#endif

	PSM2_LOG_MSG("leaving");
	return err;
}

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
// Used when err chk rdma resp indicates we must resend the rdma
static
void ips_tid_reissue_rdma_write(struct ips_tid_send_desc *tidsendc)
{
	struct ips_protoexp *protoexp = tidsendc->protoexp;

	_HFI_MMDBG("ips_tid_reissue_rdma_write tidsendc %p\n", tidsendc);

	PSM2_LOG_MSG("entering");
	protoexp->proto->epaddr_stats.rdma_rexmit++;
	tidsendc->is_complete = 0;	// sends not yet queued

	ips_tid_issue_rdma_write(tidsendc);

	/* Add as a pending op and ring up the timer */
	if (tidsendc->is_complete == 0) {
		STAILQ_INSERT_TAIL(&protoexp->pend_sendq, tidsendc, next);
		psmi_timer_request(protoexp->timerq, &protoexp->timer_send,
			   PSMI_TIMER_PRIO_1);
	}

	PSM2_LOG_MSG("leaving");
}
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#endif // defined(PSM_VERBS)

#ifdef PSM_HAVE_RDMA
static
psm2_error_t
ips_tid_pendsend_timer_callback(struct psmi_timer *timer, uint64_t current)
{
	struct ips_protoexp *protoexp = (struct ips_protoexp *)timer->context;
	struct ips_tid_send_pend *phead = &protoexp->pend_sendq;
	struct ips_tid_send_desc *tidsendc;
	psm2_error_t err = PSM2_OK;
	_HFI_MMDBG("ips_tid_pendsend_timer_callback\n");

	while (!STAILQ_EMPTY(phead)) {
		tidsendc = STAILQ_FIRST(phead);

		// we have some scb's and can use them to queue some more packets
#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
		if (tidsendc->need_err_chk_rdma)
			err = ips_protoexp_send_err_chk_rdma(tidsendc);
		else
#endif
#endif
			err = ips_tid_send_exp(tidsendc);

		if (tidsendc->is_complete)
			STAILQ_REMOVE_HEAD(phead, next);

		if (err == PSM2_OK) {
			/* Was able to complete the send, keep going */
		} else if (err == PSM2_EP_NO_RESOURCES) {
			/* No more sendbufs available, sendbuf callback will requeue this
			 * timer */
			break;
		} else if (err == PSM2_TIMEOUT
				  || err == PSM2_EPID_RV_CONNECT_RECOVERING
				  || err == PSM2_EPID_RC_CONNECT_ERROR) {
			/* Always a case of try later:
			 * On PIO flow, means no send pio bufs available
			 * On DMA flow, means kernel can't queue request or would have to block
			 * On RV or user RDMA QP is full or connection recovery/issues
			 */
			psmi_timer_request(protoexp->proto->timerq,
					   &protoexp->timer_send,
					   get_cycles() +
					   protoexp->proto->timeout_send);
			break;
		} else {
			/* Forced to reschedule later so we can check receive queue */
			psmi_assert(err == PSM2_OK_NO_PROGRESS);
			psmi_timer_request(protoexp->proto->timerq,
					   &protoexp->timer_send,
					   PSMI_TIMER_PRIO_1);
			break;
		}
	}

	return PSM2_OK;
}
#endif // PSM_HAVE_RDMA


static
psm2_error_t
ips_tid_recv_alloc(struct ips_protoexp *protoexp,
		   ips_epaddr_t *ipsaddr,
		   const struct ips_tid_get_request *getreq,
		   uint32_t nbytes_this, struct ips_tid_recv_desc **ptidrecvc)
{
	psm2_error_t err;
	ips_scb_t *grantscb;
#ifdef PSM_VERBS
	psm2_mq_req_t req = getreq->tidgr_req;
#elif defined(PSM_HAVE_GPU)
	psm2_mq_req_t req = getreq->tidgr_req;
#endif
#if defined(PSM_VERBS)
	struct ips_proto *proto = protoexp->proto;
#endif

	struct ips_tid_recv_desc *tidrecvc;

	PSM2_LOG_MSG("entering");
	/* Allocate all necessary resources. */
	_HFI_MMDBG("tid_recv_alloc\n");

	// allocate what we need to handle TID or RDMA on receive side
	// we do this before we issue CTS

	/* 1. allocate a tid grant (CTS) scb. */
	grantscb = psm3_ips_scbctrl_alloc(&protoexp->tid_scbc_rv, 1, 0,
					  IPS_SCB_FLAG_PRIORITY);
	if (grantscb == NULL) {
		_HFI_MMDBG("Wait: NO GRANT SCB\n");
		/* ips_tid_scbavail_callback() will reschedule */
		PSM2_LOG_MSG("leaving");
		return PSM2_EP_NO_RESOURCES;
	}


	/* 3. allocate a tid flow entry. */
	err = psm3_ips_tf_allocate(&protoexp->tfc, &tidrecvc);
	if (err != PSM2_OK) {
		_HFI_MMDBG("Wait: NO tid flow\n");
		psm3_ips_scbctrl_free(grantscb);
		/* Unable to get a tidflow for expected protocol. */
		psmi_timer_request(protoexp->timerq,
			&protoexp->timer_getreqs, PSMI_TIMER_PRIO_1);
		PSM2_LOG_MSG("leaving");
		return err;
	}
#if defined(PSM_VERBS)
	tidrecvc->mr = NULL;	// be safe,but should be NULL since clear on release
#endif

#ifdef PSM_HAVE_GPU
       if (req->is_buf_gpu_mem)
               tidrecvc->is_ptr_gpu_backed = !getreq->gpu_hostbuf_used;
       else
               tidrecvc->is_ptr_gpu_backed = req->is_buf_gpu_mem;

	/* 4. allocate a cuda bounce buffer, if required */
	struct ips_gpu_hostbuf *chb = NULL;
	if (getreq->gpu_hostbuf_used) {
		unsigned bufsz;
		if (nbytes_this <= GPU_SMALLHOSTBUF_SZ) {
			chb = (struct ips_gpu_hostbuf *)
				psm3_mpool_get(
					protoexp->gpu_hostbuf_pool_small_recv);
			bufsz = protoexp->gpu_hostbuf_small_recv_cfg.bufsz;
		}
		if (chb == NULL) {
			chb = (struct ips_gpu_hostbuf *)
				psm3_mpool_get(
					protoexp->gpu_hostbuf_pool_recv);
			bufsz = protoexp->gpu_hostbuf_recv_cfg.bufsz;
		}
		if (chb == NULL) {
			/* Unable to get a cudahostbuf for TID.
			 * Release the resources we're holding and reschedule.*/
			psm3_ips_tf_deallocate(&protoexp->tfc,
					  tidrecvc->rdescid._desc_idx, 0);
			psm3_ips_scbctrl_free(grantscb);
			psmi_timer_request(protoexp->timerq,
					   &protoexp->timer_getreqs,
					   PSMI_TIMER_PRIO_1);
			PSM2_LOG_MSG("leaving");
			return PSM2_EP_NO_RESOURCES;
		}

		if (chb->host_buf == NULL) {
			PSM3_GPU_HOST_ALLOC(&chb->host_buf, bufsz);
		}
		tidrecvc->gpu_hostbuf = chb;
		tidrecvc->buffer = chb->host_buf;
		chb->size = 0;
		chb->gpu_buf = (uint8_t*)getreq->tidgr_lbuf +
					getreq->tidgr_offset;
	} else {
		chb = NULL;
		tidrecvc->buffer = (void *)((uintptr_t) getreq->tidgr_lbuf +
					    getreq->tidgr_offset);
		tidrecvc->gpu_hostbuf = NULL;
	}
#else /* PSM_HAVE_GPU */
	tidrecvc->buffer =
	    (void *)((uintptr_t) getreq->tidgr_lbuf + getreq->tidgr_offset);
#endif /* PSM_HAVE_GPU */

#if defined(PSM_SOCKETS) && PSMI_HAL_INST_CNT == 1
	psmi_assert_always(0);	// should not get here
#elif defined(PSM_VERBS)
	// separate MR cache's per EP, so this confirms we have the same EP
	if (
#ifdef PSM_HAVE_GPU
		! getreq->gpu_hostbuf_used &&
#endif
		req->mr && req->mr->cache == proto->mr_cache) {
		_HFI_MMDBG("CTS chunk reference recv: %p buf %p %u bytes via %p %"PRIu64" rkey 0x%x\n",
				tidrecvc, tidrecvc->buffer, nbytes_this,
				req->mr->addr, req->mr->length, req->mr->rkey);
		tidrecvc->mr = psm3_verbs_ref_mr(req->mr);
	} else {
		_HFI_MMDBG("CTS chunk register recv: %p buf %p %u bytes\n",
				tidrecvc, tidrecvc->buffer, nbytes_this);
		tidrecvc->mr = psm3_verbs_reg_mr(proto->mr_cache, 1,
                        tidrecvc->buffer, nbytes_this, IBV_ACCESS_RDMA|IBV_ACCESS_REMOTE_WRITE
#ifdef PSM_HAVE_GPU
                                       | (PSM3_GPU_ADDR_RECV_MR(tidrecvc, getreq->gpu_hostbuf_used)?IBV_ACCESS_IS_GPU_ADDR:0)
#endif
						);
		if (! tidrecvc->mr) {
#ifdef PSM_HAVE_GPU
			if (chb)
				psm3_mpool_put(chb);
#endif
			psm3_ips_tf_deallocate(&protoexp->tfc, tidrecvc->rdescid._desc_idx, 0);
			//psm3_ips_scbctrl_free(completescb);
			psm3_ips_scbctrl_free(grantscb);
			/* Unable to register MR */
			psmi_timer_request(protoexp->timerq,
				&protoexp->timer_getreqs, PSMI_TIMER_PRIO_1);
			PSM2_LOG_MSG("leaving");
			return PSM2_TIMEOUT;	// make sure we try again
		}
		_HFI_MMDBG("CTS chunk registered: addr %p len %d rkey 0x%x\n",  tidrecvc->buffer , nbytes_this, tidrecvc->mr->rkey);
	}

	tidrecvc->recv_msglen = nbytes_this;
#endif /* PSM_VERBS */

	/* Initialize recv descriptor */
	tidrecvc->ipsaddr = ipsaddr;
	tidrecvc->getreq = (struct ips_tid_get_request *)getreq;


	tidrecvc->tidflow_nswap_gen = 0;
	tidrecvc->tidflow_genseq.psn_gen = tidrecvc->tidflow_active_gen;
	tidrecvc->tidflow_genseq.psn_seq = 0;	/* Always start sequence number at 0 (zero),
	 	 	 	 	 	   in order to prevent wraparound sequence numbers */

	tidrecvc->tid_list.tsess_srcoff = getreq->tidgr_offset;
	tidrecvc->tid_list.tsess_length = tidrecvc->recv_msglen;
#if defined(PSM_VERBS)
	// when using kernel PD/MR for kernel rendezvous QP, we need to xlat
	// our buffer to the kernel PD/MR iova space.
	// for user space PD/MR iova == addr and xlat is a noop
	tidrecvc->tid_list.tsess_rkey = tidrecvc->mr->rkey;
	tidrecvc->tid_list.tsess_raddr = tidrecvc->mr->iova + ((uintptr_t)tidrecvc->buffer -  (uintptr_t)tidrecvc->mr->addr);
#endif

	tidrecvc->state = TIDRECVC_STATE_BUSY;

	tidrecvc->stats.nSeqErr = 0;
	tidrecvc->stats.nGenErr = 0;
	tidrecvc->stats.nReXmit = 0;
	tidrecvc->stats.nErrChkReceived = 0;

	_HFI_EXP("alloc tidrecv=%d srcoff=%u length=%u\n",
		 tidrecvc->rdescid._desc_idx,
		 tidrecvc->tid_list.tsess_srcoff,
		 tidrecvc->tid_list.tsess_length);

	tidrecvc->grantscb = grantscb;

	*ptidrecvc = tidrecvc; /* return to caller */
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}

// process receiver side TID queue
// If we have TID resources, we will acquire them, setup TID HW,
// prepare a CTS and send it
// If we run out of resources with more to do, we reschedule ourselves on timer
// called directly or on timer
static
psm2_error_t
ips_tid_pendtids_timer_callback(struct psmi_timer *timer, uint64_t current)
{
	struct ips_tid_get_pend *phead =
	    &((struct ips_protoexp *)timer->context)->pend_getreqsq;
	struct ips_protoexp *protoexp;
	struct ips_tid_get_request *getreq;
	struct ips_tid_recv_desc *tidrecvc;
	ips_epaddr_t *ipsaddr;
	uint32_t nbytes_this, count;
#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
	struct ips_tid_err_resp_pend *phead_resp =
	    &((struct ips_protoexp *)timer->context)->pend_err_resp;
#endif
#endif
	int ret;

	PSM2_LOG_MSG("entering");
	_HFI_MMDBG("ips_tid_pendtids_timer_callback\n");

#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RDMA_ERR_CHK
	while (!STAILQ_EMPTY(phead_resp)) {
		ipsaddr = STAILQ_FIRST(phead_resp);
		protoexp = ipsaddr->epaddr.proto->protoexp;
		psmi_assert(ipsaddr->verbs.need_send_err_chk_rdma_resp);
		if (ipsaddr->verbs.err_chk_rdma_recheck) {
			psmi_assert(ipsaddr->verbs.err_chk_rdma_recheck_timeout_secs);
			if (PSM2_OK != ips_protoexp_do_err_chk_rdma(ipsaddr)) {
				// not yet ready to compose ERR_CHK_RDMA_RESP
				if ((cycles_to_nanosecs(get_cycles()) / NSEC_PER_SEC) >
					ipsaddr->verbs.err_chk_rdma_recheck_timeout_secs)
				{
					// fatal error, QP not draining
					psm3_handle_error(PSMI_EP_NORETURN,
						PSM2_INTERNAL_ERR,
			      			"Timeout waiting for QP to drain after QP error");
				}
				// schedule timer again in near future to recheck
				psmi_assert(ipsaddr->verbs.err_chk_rdma_recheck);
				_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] schedule recheck timer\n",
					ipsaddr);
				psmi_timer_request(protoexp->timerq, &protoexp->timer_getreqs,
					get_cycles() + nanosecs_to_cycles(VERBS_QP_DRAIN_DELAY_USEC * NSEC_PER_USEC));
				break;
			}
			psmi_assert(! ipsaddr->verbs.err_chk_rdma_recheck);
			psmi_assert(! ipsaddr->verbs.err_chk_rdma_recheck_timeout_secs);
		}
		ips_protoexp_send_err_chk_rdma_resp(&ipsaddr->flows[protoexp->proto->msgflowid]);
		if (! ipsaddr->verbs.need_send_err_chk_rdma_resp) {
			STAILQ_REMOVE_HEAD(phead_resp, verbs.pend_err_resp_next);
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] sent ERR_CHK_RESP from timer callback\n",
				ipsaddr);
		} else {
			_HFI_CONNDBG_OR_MMDBG("[ipsaddr %p] retry when scb avail\n",
				ipsaddr);
			break; // ips_tid_scbavail_callback will trigger us again
		}
	}
#endif /* PSM_HAVE_RDMA */
#endif /* PSM_VERBS */

#ifdef PSM_HAVE_GPU
	/* due to unaligned recv using hostbuf, must always do this */
	{
		/* Before processing pending TID requests, first try to free up
		 * any GPU host buffers that are now idle. */
		struct ips_tid_get_gpupend *cphead =
			&((struct ips_protoexp *)timer->context)->gpupend_getreqsq;
		psm2_error_t err;

		/* See if any GPU memcpys are in progress. Grab the first getreq... */
		while (!STAILQ_EMPTY(cphead)) {
			getreq = STAILQ_FIRST(cphead);

			err = psmi_gpu_reclaim_hostbufs(getreq);
			if (err == PSM2_OK_NO_PROGRESS)
				goto gpupend_exit;

			/* This pending gpu getreq has no more GPU ops queued up.
			 * Either it's completely done, or the GPU copies have caught
			 * up with the TID data xfer, but the TID xfer itself is not
			 * finished.
			 */
			if (getreq->tidgr_gpu_bytesdone == getreq->tidgr_length) {
				/* TID xfer is done.
				 * We should only get here if:
				 * this was involved a cuda copy, and
				 * the TIX xfer is done.
				 */
				psmi_assert(getreq->gpu_hostbuf_used);
				psmi_assert(getreq->tidgr_length ==
					    getreq->tidgr_offset);

				/* Remove from the gpupend list, and reclaim */
				getreq->tidgr_protoexp = NULL;
				getreq->tidgr_epaddr = NULL;
				STAILQ_REMOVE_HEAD(cphead, tidgr_next);

				/* mark the req as done */
				if (getreq->tidgr_callback)
					getreq->tidgr_callback(getreq->tidgr_req);
				psm3_mpool_put(getreq);
			} else
				break; /* GPU xfers in progress. Leave. */
		}
	}
gpupend_exit:
#endif

	while (!STAILQ_EMPTY(phead)) {
		getreq = STAILQ_FIRST(phead);
		ipsaddr = (ips_epaddr_t *) (getreq->tidgr_epaddr);
		count = ipsaddr->msgctl->ipsaddr_count;

ipsaddr_next:
		// always stripe for rendezvous, ignore multirail_thresh_load_balance
		// TBD - for multi-rail does this eliminate any advantages of
		// registering the MR in ips_proto_mq_rts_match_callback
		ipsaddr = ipsaddr->msgctl->ipsaddr_next;
		ipsaddr->msgctl->ipsaddr_next = ipsaddr->next;
		protoexp = ((psm2_epaddr_t) ipsaddr)->proto->protoexp;

		if (protoexp->tid_flags & IPS_PROTOEXP_FLAG_CTS_SERIALIZED) {
			psmi_assert(protoexp->proto->msgflowid < EP_NUM_FLOW_ENTRIES);
			struct ips_flow *flow = &ipsaddr->flows[protoexp->proto->msgflowid];
			if (flow->flags & IPS_FLOW_FLAG_SKIP_CTS) {
				break;                                    /* skip sending next CTS */
			}
		}

#ifdef PSM_HAVE_GPU
		if (getreq->gpu_hostbuf_used) {
			/* If this is a large transfer, we may be able to
			 * start reclaiming before all of the data is sent. */
			psmi_gpu_reclaim_hostbufs(getreq);
		}
#endif
		/*
		 * Calculate the next window size, avoid the last
		 * window too small.
		 */
		nbytes_this = getreq->tidgr_length - getreq->tidgr_offset;
		if (nbytes_this >= 2 * getreq->tidgr_rndv_winsz)
			nbytes_this = getreq->tidgr_rndv_winsz;
		else if (nbytes_this > getreq->tidgr_rndv_winsz)
			nbytes_this /= 2;
		_HFI_MMDBG("ips_tid_pendtids_timer_callback: getreq len %u offset %u nbytes_this %u\n", getreq->tidgr_length, getreq->tidgr_offset, nbytes_this);

		/*
		 * If there is a next window and the next window
		 * length is greater than PAGESIZE, make sure the window
		 * starts on a receive buffer page boundary.
		 * This assumes that page aligned receives will perform
		 * better so it strives to have only the 1st CTS be non-page
		 * aligned.  It does however incur a cost in that some large
		 * IOs may require an extra CTS.  For example with a GPU
		 * and 2MB window_sz, a 4M IO may end up usng 3 CTS instead of 2
		 * However by default such CTS are pipelined so most of the
		 * costs are hidden.  In the case of large GPU sends, the
		 * sender will be using a host bounce buffer and
		 * async cuda copies to fill it, so the extra CTS is minimal
		 * impact to the sender.
		 */
#ifdef PSM_HAVE_GPU
		psm2_mq_req_t req = getreq->tidgr_req;
		if (req->is_buf_gpu_mem){
			if (((getreq->tidgr_offset + nbytes_this) <
					getreq->tidgr_length) &&
					nbytes_this > PSMI_GPU_PAGESIZE) {
				uint32_t pageoff =
					(((uintptr_t)getreq->tidgr_lbuf) &
						(PSMI_GPU_PAGESIZE - 1)) +
					getreq->tidgr_offset + nbytes_this;
				nbytes_this -= pageoff & (PSMI_GPU_PAGESIZE - 1);
			}
		} else
#endif
		{
			if ((getreq->tidgr_offset + nbytes_this) <
					getreq->tidgr_length &&
					nbytes_this > PSMI_PAGESIZE) {
				uint32_t pageoff =
					(((uintptr_t)getreq->tidgr_lbuf) &
						(PSMI_PAGESIZE - 1)) +
					getreq->tidgr_offset + nbytes_this;
				nbytes_this -= pageoff & (PSMI_PAGESIZE - 1);
			}
		}
		_HFI_MMDBG("ips_tid_pendtids_timer_callback: page align nbytes_this %u\n", nbytes_this);

		psmi_assert(nbytes_this >= 4);

		// for verbs we use tidflows available to pace incoming RDMA
			if ((ret = ips_tf_available(&protoexp->tfc)) <= 0) {
			/* We're out of tidflow. If this process used all the resource,
			 * the free callback will reschedule the operation, otherwise,
			 * we reschedule it here */
			if (ret == 0)
			{
				psmi_timer_request(protoexp->timerq,
						   &protoexp->timer_getreqs,
						   PSMI_TIMER_PRIO_1);
			}
		} else if (ips_tid_recv_alloc(protoexp, ipsaddr,
			      getreq, nbytes_this, &tidrecvc) == PSM2_OK) {
			// send the actual CTS
			psm3_ips_protoexp_send_tid_grant(tidrecvc);
			_HFI_VDBG("GRANT sent tididx=%d srcoff=%d nbytes=%d/%d\n",
				  tidrecvc->rdescid._desc_idx,
				  getreq->tidgr_offset, tidrecvc->recv_msglen,
				  getreq->tidgr_length);

			if (protoexp->tid_flags & IPS_PROTOEXP_FLAG_CTS_SERIALIZED) {
				/*
				 * Once the CTS was sent, we mark it per 'flow' object
				 * not to proceed with next CTSes until that one is done.
				 */
				struct ips_proto *proto = tidrecvc->protoexp->proto;
				psmi_assert(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
				struct ips_flow *flow = &ipsaddr->flows[proto->msgflowid];
				flow->flags |= IPS_FLOW_FLAG_SKIP_CTS;
			}

			/*
			 * nbytes_this is the asked length for this session,
			 * ips_tid_recv_alloc() might register less pages, the
			 * real length is in tidrecvc->recv_msglen.
			 */
			getreq->tidgr_offset += tidrecvc->recv_msglen;
			psmi_assert(getreq->tidgr_offset <=
				    getreq->tidgr_length);

			if (getreq->tidgr_offset == getreq->tidgr_length) {
#ifdef PSM_HAVE_GPU
				if (getreq->gpu_hostbuf_used) {
					/* this completes the tid xfer setup.
					   move to the pending cuda ops queue,
					   set the timer to catch completion */
					STAILQ_REMOVE_HEAD(phead, tidgr_next);
					STAILQ_INSERT_TAIL(
						&getreq->tidgr_protoexp->gpupend_getreqsq,
						getreq, tidgr_next);
					psmi_timer_request(getreq->tidgr_protoexp->timerq,
							   &getreq->tidgr_protoexp->timer_getreqs,
							   PSMI_TIMER_PRIO_1);
					continue;
				}
#endif
				getreq->tidgr_protoexp = NULL;
				getreq->tidgr_epaddr = NULL;
				STAILQ_REMOVE_HEAD(phead, tidgr_next);
				continue;	/* try next grant request */
			}
			else if (protoexp->tid_flags & IPS_PROTOEXP_FLAG_RTS_CTS_INTERLEAVE) {
				/* In case of multi rail, PSM sends one CTS per request
				 * per card after which the request is moved to the end
				 * of the queue.
				 */
				count--;
				if (count)
					goto ipsaddr_next;
				STAILQ_REMOVE_HEAD(phead, tidgr_next);
				STAILQ_INSERT_TAIL(phead, getreq ,tidgr_next);
				continue;
			}

			/* created a tidrecvc, reset count */
			count = ipsaddr->msgctl->ipsaddr_count;
			goto ipsaddr_next;	/* try next fragment on next ipsaddr */
		}

		/*
		 * We need to loop until we can't get a tidrecvc on all
		 * ipsaddrs, then the callbacks on the home protoexp where
		 * getreq is linked can resume this routine. Otherwise, we
		 * might make this getreq to be orphaned and cause deadlock.
		 */
		count--;
		if (count)
			goto ipsaddr_next;
		break;
	}
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;		/* XXX err-broken */
}

#ifdef PSM_HAVE_RDMA
#ifdef PSM_HAVE_GPU
static
void psmi_cudamemcpy_tid_to_device(struct ips_tid_recv_desc *tidrecvc)
{
	struct ips_protoexp *protoexp = tidrecvc->protoexp;
	struct ips_gpu_hostbuf *chb;
	const uint32_t transfer_size =
		tidrecvc->recv_msglen;
	chb = tidrecvc->gpu_hostbuf;
	chb->size += transfer_size;

	PSM3_GPU_MEMCPY_HTOD_START(protoexp, chb, transfer_size);

	STAILQ_INSERT_TAIL(&tidrecvc->getreq->pend_gpubuf, chb, next);
	tidrecvc->gpu_hostbuf = NULL;
	ips_tid_pendtids_timer_callback(&tidrecvc->getreq->tidgr_protoexp->timer_getreqs,0);
}
#endif /* PSM_HAVE_GPU */
#endif // PSM_HAVE_RDMA

#ifdef PSM_HAVE_RDMA
// we have completed receipt of the TIDs for a given CTS
// For RC QP, this is indicated by RDMA completion w/immediate
static
psm2_error_t ips_tid_recv_free(struct ips_tid_recv_desc *tidrecvc)
{
	struct ips_protoexp *protoexp = tidrecvc->protoexp;
	struct ips_tid_get_request *getreq = tidrecvc->getreq;
	psm2_error_t err = PSM2_OK;

	psmi_assert(getreq != NULL);
	psmi_assert(tidrecvc->state == TIDRECVC_STATE_BUSY);

#ifdef PSM_HAVE_GPU
	if (tidrecvc->gpu_hostbuf)
		psmi_cudamemcpy_tid_to_device(tidrecvc);
#endif

#ifdef PSM_VERBS
	if (tidrecvc->mr) {
		_HFI_MMDBG("CTS recv chunk complete %p, releasing MR: rkey: 0x%x\n",
			tidrecvc, tidrecvc->mr->rkey);
        psm3_verbs_release_mr(tidrecvc->mr);
        tidrecvc->mr = NULL;
    }
#endif /* PSM_VERBS */

	getreq->tidgr_bytesdone += tidrecvc->recv_msglen;

	_HFI_EXP("req=%p bytes=%d/%d\n",
		 getreq->tidgr_req,
		 getreq->tidgr_bytesdone, getreq->tidgr_length);

	tidrecvc->state = TIDRECVC_STATE_FREE;

	/* finally free the tidflow */
	psm3_ips_tf_deallocate(&protoexp->tfc, tidrecvc->rdescid._desc_idx, 1);

	if (getreq->tidgr_bytesdone == getreq->tidgr_length) {
#ifdef PSM_HAVE_GPU
		/* if cuda, we handle callbacks when the cuda xfer is done */
		if (!getreq->gpu_hostbuf_used) {
			if (getreq->tidgr_callback)
				getreq->tidgr_callback(getreq->tidgr_req);
			psm3_mpool_put(getreq);
		}
#else
		if (getreq->tidgr_callback)
			getreq->tidgr_callback(getreq->tidgr_req);
		psm3_mpool_put(getreq);
#endif
	} else {
		/* We just released some tids.
		 * If requests are waiting on tids to be
		 * freed, queue up the timer */
		if (getreq->tidgr_offset < getreq->tidgr_length) {
			ips_tid_pendtids_timer_callback(&getreq->
							tidgr_protoexp->
							timer_getreqs, 0);
		}
	}

#ifdef PSM_VERBS
	/* we freed some an MR  If we have pending sends or pending get requests,
	 * turn on the timer so it can be processed. */
	ips_tid_mravail_callback(protoexp->proto);
#endif

	return err;
}
#endif // PSM_HAVE_RDMA

#ifdef PSM_RC_RECONNECT
static void match_rc_qp_callback(void *obj, void *context)
{
	struct ips_tid_send_desc *tidsendc = (struct ips_tid_send_desc *)obj;

	if (tidsendc->rc_qp == (struct psm3_verbs_rc_qp *)context) {
		_HFI_VDBG("tidsendc %p req %p userbuf %p buffer %p length %u rkey 0x%x\n",
			tidsendc, tidsendc->mqreq, tidsendc->userbuf,  tidsendc->buffer,
			tidsendc->length, tidsendc->tid_list.tsess_rkey);
	}
}

void ips_protoexp_report_inflight_rc_qp(struct ips_protoexp *protoexp,
				struct psm3_verbs_rc_qp *rc_qp)
{
	if (! _HFI_VDBG_ON)
		return;
	_HFI_VDBG("send RDMA in flight: for rc_qp %p RC QP %u\n",
			rc_qp, rc_qp->qp->qp_num);
	psmi_mpool_foreach(protoexp->tid_desc_send_pool, rc_qp,
			match_rc_qp_callback);
}
#endif /* PSM_RC_RECONNECT */

static void send_rdma_inflight_callback(void *obj, void *context)
{
	struct ips_tid_send_desc *tidsendc = (struct ips_tid_send_desc *)obj;

#ifdef PSM_RC_RECONNECT
	_HFI_VDBG("tidsendc %p req %p rkey 0x%x for rc_qp %p, QP %u draining %u posted %u + %u\n",
		tidsendc, tidsendc->mqreq, tidsendc->tid_list.tsess_rkey,
		tidsendc->rc_qp, tidsendc->rc_qp?tidsendc->rc_qp->qp->qp_num:0,
		tidsendc->rc_qp?tidsendc->rc_qp->draining:0,
		tidsendc->rc_qp?tidsendc->rc_qp->send_posted:0,
		tidsendc->rc_qp?tidsendc->rc_qp->recv_pool.posted:0);
#else
	_HFI_VDBG("tidsendc %p req %p\n",
		tidsendc, tidsendc->mqreq);
#endif
}

void ips_protoexp_report_send_inflight(psm2_ep_t ep,
				struct ips_protoexp *protoexp)
{
	if (! _HFI_VDBG_ON)
		return;
#ifdef PSM_RC_RECONNECT
	_HFI_VDBG("send RDMA in flight: (%u QPs draining)\n",
		ep->verbs_ep.send_drain_outstanding);
#else
	_HFI_VDBG("send RDMA in flight:\n");
#endif
	psmi_mpool_foreach(protoexp->tid_desc_send_pool, NULL,
			send_rdma_inflight_callback);
}

static void recv_inflight_callback(void *obj, void *context)
{
	struct ips_tid_get_request *getreq = (struct ips_tid_get_request *)obj;
	ips_epaddr_t *ipsaddr = (ips_epaddr_t *) (getreq->tidgr_epaddr);

	_HFI_VDBG("getreq %p for ipsaddr %p req %p\n",
		getreq, ipsaddr, getreq->tidgr_req);
}

void ips_protoexp_report_recv_inflight(struct ips_protoexp *protoexp)
{
	int i;

	_HFI_VDBG("recv RDMA in flight:\n");
	psmi_mpool_foreach(protoexp->tid_getreq_pool, NULL,
			recv_inflight_callback);

	_HFI_VDBG("recv tidflows in flight:\n");
	struct ips_tf *tfc = &protoexp->tfc;
	for (i = 0; i < HFI_TF_NFLOWS; i++) {
		if (tfc->tidrecvc[i].state == TF_STATE_ALLOCATED) {
			struct ips_tid_recv_desc *tidrecvc = &tfc->tidrecvc[i];
#ifdef PSM_VERBS
			_HFI_VDBG("tidrecvc %p mr %p rkey 0x%x getreq %p\n",
				tidrecvc, tidrecvc->mr,
				tidrecvc->mr?tidrecvc->mr->rkey:0,
				tidrecvc->getreq);
#else
			_HFI_VDBG("tidrecvc %p getreq %p\n",
				tidrecvc, tidrecvc->getreq);
#endif
		}
	}
}
