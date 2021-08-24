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

/*
 * IPS - Interconnect Protocol Stack.
 */

#include <assert.h>
#include <sys/uio.h>		/* writev */
#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "ips_proto_help.h"
#include "psmi_wrappers.h"
#include "psm_mq_internal.h"

#ifdef PSM_CUDA
#include "psm_gdrcpy.h"
#endif

/*
 * Control message types have their own flag to determine whether a message of
 * that type is queued or not.  These flags are kept in a state bitfield.
 */
#define CTRL_MSG_ACK_QUEUED                     0x0001
#define CTRL_MSG_NAK_QUEUED                     0x0002
#define CTRL_MSG_BECN_QUEUED                    0x0004
#define CTRL_MSG_ERR_CHK_QUEUED                 0x0008
// reserved                                     0x0010
#define CTRL_MSG_CONNECT_REQUEST_QUEUED		0x0020
#define CTRL_MSG_CONNECT_REPLY_QUEUED		0x0040
#define CTRL_MSG_DISCONNECT_REQUEST_QUEUED	0x0080
#define CTRL_MSG_DISCONNECT_REPLY_QUEUED	0x0100

#ifdef PSM_CUDA
uint32_t gpudirect_send_threshold;
uint32_t gpudirect_recv_threshold;
#endif

static void ctrlq_init(struct ips_ctrlq *ctrlq, struct ips_proto *proto);

#ifdef PSM_CUDA
void psmi_cuda_hostbuf_alloc_func(int is_alloc, void *context, void *obj)
{
	struct ips_cuda_hostbuf *icb;
	struct ips_cuda_hostbuf_mpool_cb_context *ctxt =
		(struct ips_cuda_hostbuf_mpool_cb_context *) context;

	icb = (struct ips_cuda_hostbuf *)obj;
	if (is_alloc) {
		PSMI_CUDA_CALL(cuMemHostAlloc,
			       (void **) &icb->host_buf,
			       ctxt->bufsz,
			       CU_MEMHOSTALLOC_PORTABLE);
		PSMI_CUDA_CALL(cuEventCreate, &icb->copy_status, CU_EVENT_DEFAULT);
	} else {
		if (icb->host_buf) {
			PSMI_CUDA_CALL(cuMemFreeHost, icb->host_buf);
			PSMI_CUDA_CALL(cuEventDestroy, icb->copy_status);
		}
	}
	return;
}
#endif

psm2_error_t
ips_proto_init(const psmi_context_t *context, const ptl_t *ptl,
	       int num_of_send_bufs, int num_of_send_desc, uint32_t imm_size,
	       const struct psmi_timer_ctrl *timerq,
	       const struct ips_epstate *epstate,
	       void *spioc, struct ips_proto *proto)
{
	uint32_t protoexp_flags, cksum_sz;
	union psmi_envvar_val env_tid, env_cksum, env_mtu;
	psm2_error_t err = PSM2_OK;

	/*
	 * Checksum packets within PSM. Default is off.
	 * This is heavy weight and done in software so not recommended for
	 * production runs.
	 */

	psmi_getenv("PSM3_CHECKSUM",
		    "Enable checksum of messages (0 disables checksum)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		    (union psmi_envvar_val)0, &env_cksum);

	memset(proto, 0, sizeof(struct ips_proto));
	proto->ptl = (ptl_t *) ptl;
	proto->ep = context->ep;	/* cached */
	proto->mq = context->ep->mq;	/* cached */
	proto->pend_sends.proto = proto;
	psmi_timer_entry_init(&proto->pend_sends.timer,
			      ips_proto_timer_pendq_callback,
			      &proto->pend_sends);
	STAILQ_INIT(&proto->pend_sends.pendq);
	proto->epstate = (struct ips_epstate *)epstate;
	proto->timerq = (struct psmi_timer_ctrl *)timerq;
	proto->spioc = spioc;

	// hash for dispersive routing
	proto->epinfo.ep_hash = context->ep->verbs_ep.qp->qp_num;// low 8b only

	/* If checksums enabled we insert checksum at end of packet */
	cksum_sz = env_cksum.e_uint ? PSM_CRC_SIZE_IN_BYTES : 0;
	proto->epinfo.ep_mtu = context->ep->mtu;
	/* Decrement checksum */
	proto->epinfo.ep_mtu -= cksum_sz;

	/* See if user specifies a lower MTU to use */
	if (!psmi_getenv("PSM3_MTU",
		"Upper bound on packet MTU (<=0 uses port MTU): 1-5,256,512,1024,2048,4096]",
	     PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
	     (union psmi_envvar_val)-1, &env_mtu)) {
		if (env_mtu.e_int >= OPA_MTU_MIN && env_mtu.e_int <= OPA_MTU_MAX) //enum
			env_mtu.e_int = opa_mtu_enum_to_int((enum opa_mtu)env_mtu.e_int);
		else if (env_mtu.e_int < OPA_MTU_MIN) // pick default
			env_mtu.e_int = 8192;
		else // wash through enum to force round up to next valid MTU
			env_mtu.e_int = opa_mtu_enum_to_int(opa_mtu_int_to_enum(env_mtu.e_int));
		if (proto->epinfo.ep_mtu > env_mtu.e_int) {
			proto->epinfo.ep_mtu = env_mtu.e_int;
			proto->epinfo.ep_mtu -= MAX_PSM_HEADER;
		}
	}
	// ep_mtu is our final choice of local PSM payload we can support, save it
	// back to ep->mtu
	proto->ep->mtu = proto->epinfo.ep_mtu;

	// create and size the buffer pools based on the selected ep->mtu
	err = __psm2_ep_initialize_queues(proto->ep);
	if (err)
		goto fail;

	/* sdma queue size */
	proto->sdma_queue_size = 16;	// hack until we ifdef rest of sdma
	/* don't use the last slot */

	if (proto->sdma_queue_size > 8) {
		/* configure sdma_avail_counter */
		proto->sdma_avail_counter = 8; // hack until we ifdef rest of sdma
	} else {
		err = PSM2_PARAM_ERR;
		goto fail;
	}


	proto->sdma_fill_index = 0;
	proto->sdma_done_index = 0;
	proto->sdma_scb_queue = (struct ips_scb **)
		psmi_calloc(proto->ep, UNDEFINED,
		proto->sdma_queue_size, sizeof(struct ips_scb *));
	if (proto->sdma_scb_queue == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}

	proto->timeout_send = us_2_cycles(IPS_PROTO_SPIO_RETRY_US_DEFAULT);
	proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking = ~0U;
	proto->t_init = get_cycles();
	proto->t_fini = 0;
	proto->flags = env_cksum.e_uint ? IPS_PROTO_FLAG_CKSUM : 0;
	proto->runid_key = getpid();

	proto->num_connected_outgoing = 0;
	proto->num_connected_incoming = 0;
	proto->num_disconnect_requests = 0;
	proto->stray_warn_interval = (uint64_t) -1;
	proto->done_warning = 0;
	proto->done_once = 0;
	proto->num_bogus_warnings = 0;
	proto->psmi_logevent_tid_send_reqs.interval_secs = 15;
	proto->psmi_logevent_tid_send_reqs.next_warning = 0;
	proto->psmi_logevent_tid_send_reqs.count = 0;

	{
		/* threshold for multirail load balancing */
		union psmi_envvar_val env_thresh_load_balance;

		psmi_getenv("PSM3_MULTIRAIL_THRESH_LOAD_BALANCE",
			    "Min packet size at which load balance for multi-rail (default is 0)",
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)0,
			    &env_thresh_load_balance);
		proto->multirail_thresh_load_balance = env_thresh_load_balance.e_uint;
	}

	/* Initialize IBTA related stuff (path record, SL2VL, CCA etc.) */
	if ((err = ips_ibta_init(proto)))
		goto fail;

	{
		/* User asks for HFI loopback? */
		union psmi_envvar_val env_loopback;

		psmi_getenv("PSM3_NIC_LOOPBACK",
			"PSM uses NIC loopback (default is disabled i.e. 0)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			(union psmi_envvar_val)0, /* Disabled by default */
			&env_loopback);

		if (env_loopback.e_uint)
			proto->flags |= IPS_PROTO_FLAG_LOOPBACK;
	}


	{
		/* Disable coalesced ACKs? */
		union psmi_envvar_val env_coalesce_acks;

		psmi_getenv("PSM3_COALESCE_ACKS", "Coalesce ACKs on the wire (default is enabled i.e. 1)", PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS, (union psmi_envvar_val)1,	/* Enabled by default */
			    &env_coalesce_acks);

		if (env_coalesce_acks.e_uint)
			proto->flags |= IPS_PROTO_FLAG_COALESCE_ACKS;
	}

	{
		/* Number of credits per flow */
		union psmi_envvar_val env_flow_credits;
		int df_flow_credits = min(PSM2_FLOW_CREDITS, num_of_send_desc);

		psmi_getenv("PSM3_FLOW_CREDITS",
			    "Number of unacked packets (credits) per flow (default is 64)",
			    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)df_flow_credits,
			    &env_flow_credits);
		proto->flow_credits = env_flow_credits.e_uint;
	}

	/*
	 * Pre-calculate the PSN mask to support 31 bit PSN.
	 */
	proto->psn_mask = 0x7FFFFFFF;

	/*
	 * Initialize SDMA, otherwise, turn on all PIO.
	 */
	{
		proto->flags |= IPS_PROTO_FLAG_SPIO;
		proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking =
		    ~0U;
	}

	/*
	 * Setup the protocol wide short message ep flow.
	 */
	proto->msgflowid = EP_FLOW_GO_BACK_N_PIO;

	/*
	 * Clone sendreq mpool configuration for pend sends config
	 */
	{
		uint32_t chunks, maxsz;

		psmi_assert_always(proto->ep->mq->sreq_pool != NULL);
		psmi_mpool_get_obj_info(proto->ep->mq->sreq_pool, &chunks,
					&maxsz);

		proto->pend_sends_pool =
		    psmi_mpool_create(sizeof(struct ips_pend_sreq), chunks,
				      maxsz, 0, DESCRIPTORS, NULL, NULL);
		if (proto->pend_sends_pool == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}

	/*
	 * Create a pool of CCA timers for path_rec. The timers should not
	 * exceed the scb number num_of_send_desc(default 4K).
	 */
	{
		uint32_t chunks, maxsz;

		chunks = 256;
		maxsz = num_of_send_desc;

		proto->timer_pool =
		    psmi_mpool_create(sizeof(struct psmi_timer), chunks, maxsz,
				      0, DESCRIPTORS, NULL, NULL);
		if (proto->timer_pool == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}

	/*
	 * Register ips protocol statistics
	 *
	 * We put a (*) in the output to denote stats that may cause a drop in
	 * performance.
	 *
	 * We put a (**) in the output of those stats that "should never happen"
	 */
	{

		struct psmi_stats_entry entries[] = {
			PSMI_STATS_DECLU64("pio_busy_count",
					   &proto->stats.pio_busy_cnt),
			/* Throttling by kernel */
			PSMI_STATS_DECLU64("writev_busy_cnt",
					   &proto->stats.writev_busy_cnt),
			PSMI_STATS_DECLU64("scb_unavail_eager_count",
					   &proto->stats.scb_egr_unavail_cnt),
			PSMI_STATS_DECLU64("unknown_packets_(**)",	/* shouldn't happen */
					   &proto->stats.unknown_packets),
			PSMI_STATS_DECLU64("stray_packets_(*)",
					   &proto->stats.stray_packets),
			PSMI_STATS_DECLU64("err_chk_send",
					   &proto->epaddr_stats.err_chk_send),
			PSMI_STATS_DECLU64("err_chk_recv",
					   &proto->epaddr_stats.err_chk_recv),
#ifdef RNDV_MOD
			PSMI_STATS_DECLU64("err_chk_rdma_send",
					   &proto->epaddr_stats.err_chk_rdma_send),
			PSMI_STATS_DECLU64("err_chk_rdma_recv",
					   &proto->epaddr_stats.err_chk_rdma_recv),
#endif
			PSMI_STATS_DECLU64("nak_send",
					   &proto->epaddr_stats.nak_send),
			PSMI_STATS_DECLU64("nak_recv",
					   &proto->epaddr_stats.nak_recv),
			PSMI_STATS_DECLU64("connect_req_send",
					   &proto->epaddr_stats.connect_req_send),
			PSMI_STATS_DECLU64("connect_req_recv",
					   &proto->epaddr_stats.connect_req_recv),
			PSMI_STATS_DECLU64("connect_rep_send",
					   &proto->epaddr_stats.connect_rep_send),
			PSMI_STATS_DECLU64("connect_rep_recv",
					   &proto->epaddr_stats.connect_rep_recv),
			PSMI_STATS_DECLU64("disconnect_req_send",
					   &proto->epaddr_stats.disconnect_req_send),
			PSMI_STATS_DECLU64("disconnect_req_recv",
					   &proto->epaddr_stats.disconnect_req_recv),
			PSMI_STATS_DECLU64("disconnect_rep_send",
					   &proto->epaddr_stats.disconnect_rep_send),
			PSMI_STATS_DECLU64("disconnect_rep_recv",
					   &proto->epaddr_stats.disconnect_rep_recv),
			PSMI_STATS_DECLU64("tids_grant_send",
					   &proto->epaddr_stats.tids_grant_send),
			PSMI_STATS_DECLU64("tids_grant_recv",
					   &proto->epaddr_stats.tids_grant_recv),
			PSMI_STATS_DECLU64("send_rexmit",
					   &proto->epaddr_stats.send_rexmit),
#ifdef RNDV_MOD
			PSMI_STATS_DECLU64("rdma_rexmit",
					   &proto->epaddr_stats.rdma_rexmit),
#endif
		};

		err =
		    psmi_stats_register_type
		    ("PSM_low-level_protocol_stats",
		     PSMI_STATSTYPE_IPSPROTO, entries,
		     PSMI_STATS_HOWMANY(entries), proto->ep->epid, proto);
		if (err != PSM2_OK)
			goto fail;
	}

	/*
	 * Control Queue and messaging
	 */
	ctrlq_init(&proto->ctrlq, proto);

	/*
	 * Receive-side handling
	 */
	if ((err = ips_proto_recv_init(proto)))
		goto fail;

	/* If progress thread is enabled, set the proto flag */
	{
		if (psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD))
			proto->flags |= IPS_PROTO_FLAG_RCVTHREAD;
	}

	/*
	 * Eager buffers.  We don't care to receive a callback when eager buffers
	 * are newly released since we actively poll for new bufs.
	 */
	{
		/* configure PSM bounce buffer size */
		union psmi_envvar_val env_bbs;

		psmi_getenv("PSM3_BOUNCE_SZ",
			"PSM send bounce buffer size (default is 8192B)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)8192,
			&env_bbs);

		proto->scb_bufsize = env_bbs.e_uint;
	}

	if ((err = ips_scbctrl_init(context, num_of_send_desc,
				    num_of_send_bufs, imm_size,
				    proto->scb_bufsize, NULL, NULL,
				    &proto->scbc_egr)))
		goto fail;

	/*
	 * Expected protocol handling.
	 * If we enable tid-based expected rendezvous, the expected protocol code
	 * handles its own rv scb buffers.  If not, we have to enable eager-based
	 * rendezvous and we allocate scb buffers for it.
	 * For UD PSM3_RDMA (ep->rdmamode) controls our use of RDMA for Rendezvous
	 * For STL100 PSM3_TID controls use of EXPTID for Rendezvous
	 */
	env_tid.e_uint = proto->ep->rdmamode;	// PSM3_RDMA
	protoexp_flags = env_tid.e_uint;

	// protoexp implements RDMA for UD and TID for STL100 native.  N/A to UDP
	// when proto->protoexp is NULL, we will not attempt to use TID nor RDMA
	{
		// for UD, even when RDMA is enabled, we may fall back to LONG_DATA
		// in which case we want the scbc_rv scb's so we don't exhaust the
		// scbc_egr pool
		proto->scbc_rv = (struct ips_scbctrl *)
		    psmi_calloc(proto->ep, DESCRIPTORS,
				1, sizeof(struct ips_scbctrl));
		if (proto->scbc_rv == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
		/*
		 * Rendezvous buffers. We want to get a callback for rendezvous bufs
		 * since we asynchronously try to make progress on these sends and only
		 * schedule them on the timerq if there are pending sends and available
		 * bufs.
		 */
		if ((err =
		     ips_scbctrl_init(context, num_of_send_desc,
				      0 /* no bufs */ ,
				      0, 0 /* bufsize==0 */ ,
				      ips_proto_rv_scbavail_callback,
				      proto, proto->scbc_rv)))
			goto fail;
	}
	if (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED) {
#ifdef PSM_CUDA
		if (PSMI_IS_CUDA_ENABLED) {
			PSMI_CUDA_CALL(cuStreamCreate,
				   &proto->cudastream_send, CU_STREAM_NON_BLOCKING);
		}
#endif
		if ((err = ips_protoexp_init(context, proto, protoexp_flags,
					     num_of_send_bufs, num_of_send_desc,
					     &proto->protoexp)))
			goto fail;
	} else {
		proto->protoexp = NULL;
	}

	// we allocate MR cache here (as opposed to in protoexp) in case we later
	// decide to implement RC send for medium messages and use it to register
	// medium sized user eager buffers (SDMA-like)
	if (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED) {
		union psmi_envvar_val env_mr_cache_size;
		uint32_t default_cache_size;	// in entries
		uint32_t cache_pri_entries;
		uint64_t cache_pri_size;	// in bytes

		// we can have at most HFI_TF_NFLOWS inbound RDMA and hfi_num_send_rdma
		// outbound RDMA.  Each of which potentially needs an MR.
		// so mr_cache_size should be >= HFI_TF_NFLOWS + ep->hfi_num_send_rdma
		// but can survive if it's smaller as we will delay transfer til avail
		cache_pri_entries =  HFI_TF_NFLOWS + proto->ep->hfi_num_send_rdma;
		cache_pri_size  = (uint64_t)cache_pri_entries * proto->mq->hfi_base_window_rv;
		if (proto->ep->mr_cache_mode == MR_CACHE_MODE_USER) {
			// we attempt to cache, so can benefit from more than inflight
			default_cache_size = cache_pri_entries * 16;
		} else {
			// we only reference count
			// could benefit from some extra so we can preregister MRs for
			// transfers we don't yet have resources for
			default_cache_size = cache_pri_entries * 8;
		}
		/* Size of user space MR Cache
		 */
		psmi_getenv("PSM3_MR_CACHE_SIZE",
				"user space MR table/cache size (num MRs)",
				PSMI_ENVVAR_LEVEL_USER,
				PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)default_cache_size, &env_mr_cache_size);

		proto->mr_cache = psm2_verbs_alloc_mr_cache(proto->ep,
						env_mr_cache_size.e_uint, proto->ep->mr_cache_mode,
						cache_pri_entries, cache_pri_size);
		if (! proto->mr_cache) {
			_HFI_ERROR( "Unable to allocate MR cache (%u entries)\n",
					env_mr_cache_size.e_uint);
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}


	/* Active Message interface. AM requests compete with MQ for eager
	 * buffers, since request establish the amount of buffering in the
	 * network (maximum number of requests in flight). The AM init function
	 * does not allow the number of send buffers to be set separately from
	 * the number of send descriptors, because otherwise it would have to
	 * impose extremely arcane constraints on the relative amounts to avoid
	 * a deadlock scenario. Thus, it handles it internally. The constraint
	 * is: In a node pair, the number of reply send buffers on at least one
	 * of the nodes must be at least double the number (optimal: double + 1)
	 * of send descriptors on the other node. */
	if ((err = ips_proto_am_init(proto,
				     min(num_of_send_bufs, num_of_send_desc),
				     imm_size,
				     &proto->proto_am)))
		goto fail;

#if 0
	if (!host_pid) {
		char ipbuf[INET_ADDRSTRLEN], *p;
		host_pid = (uint32_t) getpid();
		host_ipv4addr = psmi_get_ipv4addr();	/* already be */
		if (host_ipv4addr == 0) {
			_HFI_DBG("Unable to obtain local IP address, "
				 "not fatal but some features may be disabled\n");
		} else if (host_ipv4addr == __cpu_to_be32(0x7f000001)) {
			_HFI_INFO("Localhost IP address is set to the "
				  "loopback address 127.0.0.1, "
				  "not fatal but some features may be disabled\n");
		} else {
			p = (char *)inet_ntop(AF_INET,
					      (const void *)&host_ipv4addr,
					      ipbuf, sizeof(ipbuf));
			_HFI_PRDBG("Ethernet Host IP=%s and PID=%d\n", p,
				   host_pid);
		}

		/* Store in big endian for use in ERR_CHK */
		host_pid = __cpu_to_be32(host_pid);
	}
#endif
#ifdef PSM_CUDA
	union psmi_envvar_val env_gpudirect_rdma;
	psmi_getenv("PSM3_GPUDIRECT",
				"Use GPUDirect RDMA support to allow the NIC to directly read"
				" from the GPU for SDMA and write to the GPU for TID RDMA."
				" Requires driver support.(default is disabled i.e. 0)",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
				(union psmi_envvar_val)0, /* Disabled by default */
				&env_gpudirect_rdma);
	/* The following cases need to be handled:
	 * 1) GPU DIRECT is turned off but GDR COPY is turned on by the user or
	 *    by default - Turn off GDR COPY
	 * 2) GPU DIRECT is on but GDR COPY is turned off by the user - Leave
	 *.   this config as it is.
	 */
	if (!env_gpudirect_rdma.e_uint)
		is_gdr_copy_enabled = 0;

	/* Default Send threshold for Gpu-direct set to 30000 */
	union psmi_envvar_val env_gpudirect_send_thresh;
	psmi_getenv("PSM3_GPUDIRECT_SEND_THRESH",
		    "GPUDirect feature on send side will be switched off if threshold value is exceeded.",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)30000, &env_gpudirect_send_thresh);
	gpudirect_send_threshold = env_gpudirect_send_thresh.e_uint;

	union psmi_envvar_val env_gpudirect_recv_thresh;
	psmi_getenv("PSM3_GPUDIRECT_RECV_THRESH",
		    "GPUDirect feature on receive side will be switched off if threshold value is exceeded.",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)UINT_MAX, &env_gpudirect_recv_thresh);
	gpudirect_recv_threshold = env_gpudirect_recv_thresh.e_uint;

	if (env_gpudirect_rdma.e_uint && device_support_gpudirect) {
		if (PSMI_IS_CUDA_DISABLED ||
			!(protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED) ||
			PSMI_IS_DRIVER_GPUDIRECT_DISABLED)
			err = psmi_handle_error(PSMI_EP_NORETURN,
					PSM2_INTERNAL_ERR,
					"Requires hfi1 driver with GPU-Direct feature enabled.\n");
		proto->flags |= IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND;
		proto->flags |= IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV;
	} else {
		/* The following environment variables are here for internal
		 * experimentation and will not be documented for any customers.
		 */
		/* Use GPUDirect RDMA for SDMA send? */
		union psmi_envvar_val env_gpudirect_rdma_send;
		psmi_getenv("PSM3_GPUDIRECT_RDMA_SEND",
					"Use GPUDirect RDMA support to allow the NIC to directly"
					" read from the GPU for SDMA.  Requires driver"
					" support.(default is disabled i.e. 0)",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
					(union psmi_envvar_val)0, /* Disabled by default */
					&env_gpudirect_rdma_send);

		if (env_gpudirect_rdma_send.e_uint && device_support_gpudirect) {
			if (PSMI_IS_CUDA_DISABLED
				|| !(protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED)
				)
				err = psmi_handle_error(PSMI_EP_NORETURN,
						PSM2_INTERNAL_ERR,
						"Unable to start run as PSM would require cuda, sdma"
						"and TID support\n");
			proto->flags |= IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND;
		}
		/* Use GPUDirect RDMA for recv? */
		union psmi_envvar_val env_gpudirect_rdma_recv;
		psmi_getenv("PSM3_GPUDIRECT_RDMA_RECV",
					"Use GPUDirect RDMA support to allow the NIC to directly"
					" write into GPU.  Requires driver support.(default is"
					" disabled i.e. 0)",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
					(union psmi_envvar_val)0, /* Disabled by default */
					&env_gpudirect_rdma_recv);

		if (env_gpudirect_rdma_recv.e_uint && device_support_gpudirect) {
			if (PSMI_IS_CUDA_DISABLED ||
				!(protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED))
					err = psmi_handle_error(PSMI_EP_NORETURN,
							PSM2_INTERNAL_ERR,
							"Unable to start run as PSM would require cuda,"
							" sdma and TID support\n");
			proto->flags |= IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV;
		}
	}

	if (PSMI_IS_CUDA_ENABLED &&
		 (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED)) {
		struct psmi_rlimit_mpool rlim = CUDA_HOSTBUFFER_LIMITS;
		uint32_t maxsz, chunksz, max_elements;

		if ((err = psmi_parse_mpool_env(proto->mq, 1,
						&rlim, &maxsz, &chunksz)))
			goto fail;

		/* the maxsz is the amount in MB, not the number of entries,
		 * since the element size depends on the window size */
		max_elements = (maxsz*1024*1024) / proto->mq->hfi_base_window_rv;
		/* mpool requires max_elements to be power of 2. round down. */
		max_elements = 1 << (31 - __builtin_clz(max_elements));
		proto->cuda_hostbuf_send_cfg.bufsz = proto->mq->hfi_base_window_rv;
		proto->cuda_hostbuf_pool_send =
			psmi_mpool_create_for_cuda(sizeof(struct ips_cuda_hostbuf),
						   chunksz, max_elements, 0,
						   UNDEFINED, NULL, NULL,
						   psmi_cuda_hostbuf_alloc_func,
						   (void *)
						   &proto->cuda_hostbuf_send_cfg);

		if (proto->cuda_hostbuf_pool_send == NULL) {
			err = psmi_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate CUDA host send buffer pool");
			goto fail;
		}

		/* use the same number of elements for the small pool */
		proto->cuda_hostbuf_small_send_cfg.bufsz = CUDA_SMALLHOSTBUF_SZ;
		proto->cuda_hostbuf_pool_small_send =
			psmi_mpool_create_for_cuda(sizeof(struct ips_cuda_hostbuf),
						   chunksz, max_elements, 0,
						   UNDEFINED, NULL, NULL,
						   psmi_cuda_hostbuf_alloc_func,
						   (void *)
						   &proto->cuda_hostbuf_small_send_cfg);

		if (proto->cuda_hostbuf_pool_small_send == NULL) {
			err = psmi_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate CUDA host small send buffer pool");
			goto fail;
		}

		/* Configure the amount of prefetching */
		union psmi_envvar_val env_prefetch_limit;

		psmi_getenv("PSM3_CUDA_PREFETCH_LIMIT",
			    "How many TID windows to prefetch at RTS time(default is 2)",
			    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			    (union psmi_envvar_val)CUDA_WINDOW_PREFETCH_DEFAULT,
			    &env_prefetch_limit);
		proto->cuda_prefetch_limit = env_prefetch_limit.e_uint;
	}
#endif
fail:
	return err;
}

psm2_error_t
ips_proto_fini(struct ips_proto *proto, int force, uint64_t timeout_in)
{
	struct psmi_eptab_iterator itor;
	uint64_t t_start;
	uint64_t t_grace_start, t_grace_time, t_grace_interval;
	psm2_epaddr_t epaddr;
	psm2_error_t err = PSM2_OK;
	int i;
	union psmi_envvar_val grace_intval;

	/* Poll one more time to attempt to synchronize with the peer ep's. */
	ips_ptl_poll(proto->ptl, 0);

	psmi_getenv("PSM3_CLOSE_GRACE_PERIOD",
		    "Additional grace period in seconds for closing end-point.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)0, &grace_intval);

	if (getenv("PSM3_CLOSE_GRACE_PERIOD")) {
		t_grace_time = grace_intval.e_uint * SEC_ULL;
	} else if (timeout_in > 0) {
		/* default to half of the close time-out */
		t_grace_time = timeout_in / 2;
	} else {
		/* propagate the infinite time-out case */
		t_grace_time = 0;
	}

	if (t_grace_time > 0 && t_grace_time < PSMI_MIN_EP_CLOSE_TIMEOUT)
		t_grace_time = PSMI_MIN_EP_CLOSE_TIMEOUT;

	/* At close we will busy wait for the grace interval to see if any
	 * receive progress is made. If progress is made we will wait for
	 * another grace interval, until either no progress is made or the
	 * entire grace period has passed. If the grace interval is too low
	 * we may miss traffic and exit too early. If the grace interval is
	 * too large the additional time spent while closing the program
	 * will become visible to the user. */
	psmi_getenv("PSM3_CLOSE_GRACE_INTERVAL",
		    "Grace interval in seconds for closing end-point.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)0, &grace_intval);

	if (getenv("PSM3_CLOSE_GRACE_INTERVAL")) {
		t_grace_interval = grace_intval.e_uint * SEC_ULL;
	} else {
		/* A heuristic is used to scale up the timeout linearly with
		 * the number of endpoints, and we allow one second per 1000
		 * endpoints. */
		t_grace_interval = (proto->ep->connections * SEC_ULL) / 1000;
	}

	if (t_grace_interval < PSMI_MIN_EP_CLOSE_GRACE_INTERVAL)
		t_grace_interval = PSMI_MIN_EP_CLOSE_GRACE_INTERVAL;
	if (t_grace_interval > PSMI_MAX_EP_CLOSE_GRACE_INTERVAL)
		t_grace_interval = PSMI_MAX_EP_CLOSE_GRACE_INTERVAL;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	t_start = proto->t_fini = get_cycles();

	/* Close whatever has been left open */
	if (proto->num_connected_outgoing > 0) {
		int num_disc = 0;
		int *mask;
		psm2_error_t *errs;
		psm2_epaddr_t *epaddr_array;

		psmi_epid_itor_init(&itor, proto->ep);
		while ((epaddr = psmi_epid_itor_next(&itor))) {
			if (epaddr->ptlctl->ptl == proto->ptl)
				num_disc++;
		}
		psmi_epid_itor_fini(&itor);
		mask =
		    (int *)psmi_calloc(proto->ep, UNDEFINED, num_disc,
				       sizeof(int));
		errs = (psm2_error_t *)
		    psmi_calloc(proto->ep, UNDEFINED, num_disc,
				sizeof(psm2_error_t));
		epaddr_array = (psm2_epaddr_t *)
		    psmi_calloc(proto->ep, UNDEFINED, num_disc,
				sizeof(psm2_epaddr_t));

		if (errs == NULL || epaddr_array == NULL || mask == NULL) {
			if (epaddr_array)
				psmi_free(epaddr_array);
			if (errs)
				psmi_free(errs);
			if (mask)
				psmi_free(mask);
			err = PSM2_NO_MEMORY;
			goto fail;
		}
		psmi_epid_itor_init(&itor, proto->ep);
		i = 0;
		while ((epaddr = psmi_epid_itor_next(&itor))) {
			/*
			 * if cstate_outgoing is CSTATE_NONE, then we know it
			 * is an uni-directional connect, in that the peer
			 * sent a connect request to us, but we never sent one
			 * out to the peer epid. Ignore handling those in
			 * ips_proto_disconnect() as we will do the right thing
			 * when a disconnect request for the epaddr comes in from the peer.
			 */
			if (epaddr->ptlctl->ptl == proto->ptl &&
				((ips_epaddr_t *) epaddr)->cstate_outgoing != CSTATE_NONE) {
				mask[i] = 1;
				epaddr_array[i] = epaddr;
				i++;
				IPS_MCTXT_REMOVE((ips_epaddr_t *) epaddr);
			}
		}
		psmi_epid_itor_fini(&itor);
		err = ips_proto_disconnect(proto, force, num_disc, epaddr_array,
					   mask, errs, timeout_in);
		psmi_free(mask);
		psmi_free(errs);
		psmi_free(epaddr_array);
	}

	t_grace_start = get_cycles();

	while (psmi_cycles_left(t_grace_start, t_grace_time)) {
		uint64_t t_grace_interval_start = get_cycles();
		int num_disconnect_requests = proto->num_disconnect_requests;
		PSMI_BLOCKUNTIL(
			proto->ep, err,
			proto->num_connected_incoming == 0 ||
			(!psmi_cycles_left(t_start, timeout_in) &&
			 (!psmi_cycles_left(t_grace_interval_start,
					    t_grace_interval) ||
			  !psmi_cycles_left(t_grace_start, t_grace_time))));
		if (num_disconnect_requests == proto->num_disconnect_requests) {
			/* nothing happened in this grace interval so break out early */
			break;
		}
	}

#if _HFI_DEBUGGING
	if (_HFI_PRDBG_ON) {
		uint64_t t_grace_finish = get_cycles();

		_HFI_PRDBG_ALWAYS(
			"Closing endpoint disconnect left to=%d,from=%d after %d millisec of grace (out of %d)\n",
			proto->num_connected_outgoing, proto->num_connected_incoming,
			(int)(cycles_to_nanosecs(t_grace_finish - t_grace_start) /
			MSEC_ULL), (int)(t_grace_time / MSEC_ULL));
	}
#endif

#ifdef PSM_CUDA
	if (PSMI_IS_CUDA_ENABLED && proto->cudastream_send) {
		PSMI_CUDA_CALL(cuStreamDestroy, proto->cudastream_send);
	}
#endif

	if ((err = ips_ibta_fini(proto)))
		goto fail;

	if ((err = ips_proto_am_fini(&proto->proto_am)))
		goto fail;

	if ((err = ips_scbctrl_fini(&proto->scbc_egr)))
		goto fail;

	ips_proto_recv_fini(proto);

	if (proto->protoexp) {
		if ((err = ips_protoexp_fini(proto->protoexp)))
			goto fail;
	}
	if (proto->scbc_rv) {
		ips_scbctrl_fini(proto->scbc_rv);
		psmi_free(proto->scbc_rv);
	}

	if (proto->mr_cache) {
		psm2_verbs_free_mr_cache(proto->mr_cache);
		proto->mr_cache = NULL;
    }
	psmi_stats_deregister_type(PSMI_STATSTYPE_IPSPROTO, proto);

	psmi_mpool_destroy(proto->pend_sends_pool);
	psmi_mpool_destroy(proto->timer_pool);

	psmi_free(proto->sdma_scb_queue);

fail:
	proto->t_fini = proto->t_init = 0;
	return err;
}


static
void ctrlq_init(struct ips_ctrlq *ctrlq, struct ips_proto *proto)
{
	/* clear the ctrl send queue */
	memset(ctrlq, 0, sizeof(*ctrlq));

	proto->message_type_to_index[OPCODE_ACK] = CTRL_MSG_ACK_QUEUED;
	proto->message_type_to_index[OPCODE_NAK] = CTRL_MSG_NAK_QUEUED;
	proto->message_type_to_index[OPCODE_BECN] = CTRL_MSG_BECN_QUEUED;
	proto->message_type_to_index[OPCODE_ERR_CHK] = CTRL_MSG_ERR_CHK_QUEUED;
	proto->message_type_to_index[OPCODE_CONNECT_REQUEST] =
	    CTRL_MSG_CONNECT_REQUEST_QUEUED;
	proto->message_type_to_index[OPCODE_CONNECT_REPLY] =
	    CTRL_MSG_CONNECT_REPLY_QUEUED;
	proto->message_type_to_index[OPCODE_DISCONNECT_REQUEST] =
	    CTRL_MSG_DISCONNECT_REQUEST_QUEUED;
	proto->message_type_to_index[OPCODE_DISCONNECT_REPLY] =
	    CTRL_MSG_DISCONNECT_REPLY_QUEUED;

	ctrlq->ctrlq_head = ctrlq->ctrlq_tail = 0;
	ctrlq->ctrlq_overflow = 0;
	ctrlq->ctrlq_proto = proto;

	/*
	 * We never enqueue ctrl messages with real payload. If we do,
	 * the queue 'elem_payload' size needs to be big enough.
	 * Note: enqueue nak/ack is very important for performance.
	 */
	proto->ctrl_msg_queue_enqueue =
	    CTRL_MSG_ACK_QUEUED |
	    CTRL_MSG_NAK_QUEUED |
	    CTRL_MSG_BECN_QUEUED;

	psmi_timer_entry_init(&ctrlq->ctrlq_timer,
			      ips_proto_timer_ctrlq_callback, ctrlq);

	return;
}

static __inline__ void _build_ctrl_message(struct ips_proto *proto,
			struct ips_flow *flow, uint8_t message_type,
			ips_scb_t *ctrlscb, uint32_t paylen)
{
	uint32_t tot_paywords = (sizeof(struct ips_message_header) +
		HFI_CRC_SIZE_IN_BYTES + paylen) >> BYTE2DWORD_SHIFT;
	uint32_t slid, dlid;
	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	struct ips_message_header *p_hdr = &ctrlscb->ips_lrh;
	ips_path_rec_t *ctrl_path =
	    ipsaddr->pathgrp->pg_path[ipsaddr->
				      hpp_index][IPS_PATH_HIGH_PRIORITY];

	if ((proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE) &&
	    (++ipsaddr->hpp_index >=
	     ipsaddr->pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY]))
		ipsaddr->hpp_index = 0;

	/*
	 * If the size of the transfer is NOT within the "exclusion range",
	 * then use the "dispersive routling" slid/dlid.  Otherwise
	 * use the base LIDS.
	 *
	 * This is a control message, so it should never be a TID transfer.
	 */
	slid = ctrl_path->pr_slid;
	dlid = ctrl_path->pr_dlid;
	if (ctrlscb->scb_flags & IPS_SEND_FLAG_NO_LMC) {
		slid = ipsaddr->pathgrp->pg_base_slid;
		dlid = ipsaddr->pathgrp->pg_base_dlid;
	}

	/* Control messages go over the control path. */
	p_hdr->lrh[0] = __cpu_to_be16(HFI_LRH_BTH |
				      ((ctrl_path->pr_sl & HFI_LRH_SL_MASK) <<
				       HFI_LRH_SL_SHIFT)
					);
	p_hdr->lrh[1] = dlid;
	p_hdr->lrh[2] = __cpu_to_be16(tot_paywords & HFI_LRH_PKTLEN_MASK);
	p_hdr->lrh[3] = slid;

	p_hdr->bth[0] = __cpu_to_be32(ctrl_path->pr_pkey |
				      (message_type << HFI_BTH_OPCODE_SHIFT));

	p_hdr->bth[1] = __cpu_to_be32(flow->flowid << HFI_BTH_FLOWID_SHIFT);
	flow->flags &= ~IPS_FLOW_FLAG_GEN_BECN;

	/* p_hdr->bth[2] already set by caller, or don't care */
	/* p_hdr->ack_seq_num already set by caller, or don't care */

	p_hdr->connidx = ipsaddr->connidx_outgoing;
	p_hdr->flags = 0;

	p_hdr->khdr.kdeth0 = __cpu_to_le32(
			(ctrlscb->scb_flags & IPS_SEND_FLAG_INTR) |
			(IPS_PROTO_VERSION << HFI_KHDR_KVER_SHIFT));
	p_hdr->khdr.kdeth1 = 0;

	return;
}

psm2_error_t
ips_proto_timer_ctrlq_callback(struct psmi_timer *timer, uint64_t t_cyc_expire)
{
	struct ips_ctrlq *ctrlq = (struct ips_ctrlq *)timer->context;
	struct ips_proto *proto = ctrlq->ctrlq_proto;
	struct ips_ctrlq_elem *cqe;
	uint32_t have_cksum = proto->flags & IPS_PROTO_FLAG_CKSUM;
	psm2_error_t err;

	/* service ctrl send queue first */
	while (ctrlq->ctrlq_cqe[ctrlq->ctrlq_tail].msg_queue_mask) {
		cqe = &ctrlq->ctrlq_cqe[ctrlq->ctrlq_tail];
		GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
		if (cqe->msg_scb.flow->transfer == PSM_TRANSFER_PIO) {
			err = psmi_hal_spio_transfer_frame(proto,
							   cqe->msg_scb.flow, &cqe->msg_scb,
							   cqe->msg_scb.cksum, 0, PSMI_TRUE,
							   have_cksum, cqe->msg_scb.cksum[0],
							   proto->ep->context.psm_hw_ctxt
#ifdef PSM_CUDA
			       , 0
#endif
				);
		} else {
			psmi_assert_always(0);
			err = PSM2_INTERNAL_ERR;
		}
		GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */

		if (err == PSM2_OK) {
			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&cqe->msg_scb.ips_lrh,"PKT_STRM: err: %d", err);
			ips_proto_epaddr_stats_set(proto, cqe->message_type);
			*cqe->msg_queue_mask &=
			    ~message_type2index(proto, cqe->message_type);
			cqe->msg_queue_mask = NULL;
			ctrlq->ctrlq_tail =
			    (ctrlq->ctrlq_tail + 1) % CTRL_MSG_QEUEUE_SIZE;
		} else {
			psmi_assert(err == PSM2_EP_NO_RESOURCES);

			proto->stats.pio_busy_cnt++;
			/* re-request a timer expiration */
			psmi_timer_request(proto->timerq, &ctrlq->ctrlq_timer,
					   PSMI_TIMER_PRIO_0);
			return PSM2_OK;
		}
	}

	return PSM2_OK;
}

/* Update cqe struct which is a single element from pending control message queue */
PSMI_ALWAYS_INLINE(
void ips_proto_update_cqe(struct ips_ctrlq_elem *cqe, uint16_t *msg_queue_mask,
			  struct ips_flow *flow, ips_scb_t *ctrlscb, uint8_t message_type)){

	cqe->message_type = message_type;
	cqe->msg_queue_mask = msg_queue_mask;
	psmi_mq_mtucpy(&cqe->msg_scb.ips_lrh,
		       &ctrlscb->ips_lrh, sizeof(ctrlscb->ips_lrh));
	cqe->msg_scb.flow = flow;
	cqe->msg_scb.cksum[0] = ctrlscb->cksum[0];
}

psm2_error_t
ips_proto_send_ctrl_message(struct ips_flow *flow, uint8_t message_type,
			uint16_t *msg_queue_mask, ips_scb_t *ctrlscb,
			void *payload, uint32_t paylen)
{
	psm2_error_t err = PSM2_EP_NO_RESOURCES;
	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	struct ips_proto *proto = ((psm2_epaddr_t) ipsaddr)->proto;
	struct ips_ctrlq *ctrlq = &proto->ctrlq;
	struct ips_ctrlq_elem *cqe = ctrlq->ctrlq_cqe;
	uint32_t have_cksum;

	psmi_assert(message_type >= OPCODE_ACK &&
			message_type <= OPCODE_DISCONNECT_REPLY);
	psmi_assert((paylen & 0x3) == 0);	/* require 4-byte multiple */
	psmi_assert(flow->frag_size >=
			(paylen + PSM_CRC_SIZE_IN_BYTES));

	/* Drain queue if non-empty */
	if (cqe[ctrlq->ctrlq_tail].msg_queue_mask)
		ips_proto_timer_ctrlq_callback(&ctrlq->ctrlq_timer, 0ULL);

	/* finish setup control message header */
	ips_set_LMC_LID_choice(proto, ctrlscb, paylen);
	_build_ctrl_message(proto, flow, message_type, ctrlscb, paylen);

	/* If enabled checksum control message */
	have_cksum = proto->flags & IPS_PROTO_FLAG_CKSUM;
	if (have_cksum) {
		ctrlscb->ips_lrh.flags |= IPS_SEND_FLAG_PKTCKSUM;
		ips_do_cksum(proto, &ctrlscb->ips_lrh,
				payload, paylen, ctrlscb->cksum);
	}

	/*
	 * for ACK/NAK/BECN, we use the fast flow to send over, otherwise,
	 * we use the original flow
	 */
	if (message_type == OPCODE_ACK ||
	    message_type == OPCODE_NAK ||
	    message_type == OPCODE_BECN)
	{
		psmi_assert(proto->msgflowid < EP_FLOW_LAST);
		flow = &ipsaddr->flows[proto->msgflowid];
	}

	switch (flow->transfer) {
	case PSM_TRANSFER_PIO:
		GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
		err = psmi_hal_spio_transfer_frame(proto, flow,
						   ctrlscb, payload, paylen,
						   PSMI_TRUE, have_cksum, ctrlscb->cksum[0],
						   proto->ep->context.psm_hw_ctxt
#ifdef PSM_CUDA
						   , 0
#endif
			     );
		GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
		break;
	default:
		err = PSM2_INTERNAL_ERR;
		break;
	}

	if (err == PSM2_OK)
	{
		PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&ctrlscb->ips_lrh,"PKT_STRM: err: %d", err);
		ips_proto_epaddr_stats_set(proto, message_type);
	}

	_HFI_VDBG("transfer_frame of opcode=0x%x,remote_lid=%d,"
		  "src=%p,len=%d returns %d\n",
		  (int)_get_proto_hfi_opcode(&ctrlscb->ips_lrh),
		  __be16_to_cpu(ctrlscb->ips_lrh.lrh[1]), payload, paylen, err);

	if (err != PSM2_EP_NO_RESOURCES)
		return err;
	proto->stats.pio_busy_cnt++;

	if (proto->ctrl_msg_queue_enqueue & proto->
	    message_type_to_index[message_type]) {
		/* We only queue control msg without payload */
		psmi_assert(paylen == 0);

		if ((*msg_queue_mask) & proto->
		    message_type_to_index[message_type]) {

			if (message_type == OPCODE_ACK) {
				/* Pending queue should contain latest ACK type message,
				 * overwrite the previous one. */
				ips_proto_update_cqe(&cqe[flow->ack_index], msg_queue_mask,
						     flow, ctrlscb, message_type);
			}

			err = PSM2_OK;
		} else if (cqe[ctrlq->ctrlq_head].msg_queue_mask == NULL) {
			/* entry is free */
			if (message_type == OPCODE_ACK) {
				/* Track the index of last ACK type message in queue*/
				flow->ack_index = ctrlq->ctrlq_head;
			}

			*msg_queue_mask |=
			    message_type2index(proto, message_type);

			ips_proto_update_cqe(&cqe[ctrlq->ctrlq_head], msg_queue_mask,
					     flow, ctrlscb, message_type);

			ctrlq->ctrlq_head =
			    (ctrlq->ctrlq_head + 1) % CTRL_MSG_QEUEUE_SIZE;
			/* _HFI_INFO("requesting ctrlq timer for msgtype=%d!\n", message_type); */
			psmi_timer_request(proto->timerq, &ctrlq->ctrlq_timer,
					   PSMI_TIMER_PRIO_0);

			err = PSM2_OK;
		} else {
			proto->ctrl_msg_queue_overflow++;
		}
	}

	return err;
}

void MOCKABLE(ips_proto_flow_enqueue)(struct ips_flow *flow, ips_scb_t *scb)
{
	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	struct ips_proto *proto = ((psm2_epaddr_t) ipsaddr)->proto;

	ips_scb_prepare_flow_inner(proto, ipsaddr, flow, scb);
	if ((proto->flags & IPS_PROTO_FLAG_CKSUM) &&
	    (scb->tidctrl == 0) && (scb->nfrag == 1)) {
		scb->ips_lrh.flags |= IPS_SEND_FLAG_PKTCKSUM;
		ips_do_cksum(proto, &scb->ips_lrh,
			     ips_scb_buffer(scb), scb->payload_size, &scb->cksum[0]);
	}

	/* If this is the first scb on flow, pull in both timers. */
	if (flow->timer_ack == NULL) {
		psmi_assert(flow->timer_send == NULL);
		flow->timer_ack = scb->timer_ack;
		flow->timer_send = scb->timer_send;
	}
	psmi_assert(flow->timer_ack != NULL);
	psmi_assert(flow->timer_send != NULL);

	/* Every flow has a pending head that points into the unacked queue.
	 * If sends are already pending, process those first */
	if (SLIST_EMPTY(&flow->scb_pend))
        {
                PSM2_LOG_PKT_STRM(PSM2_LOG_PEND,&scb->ips_lrh,"PKT_STRM: pkt in pend list");
		SLIST_FIRST(&flow->scb_pend) = scb;
	}

	/* Insert scb into flow's unacked queue */
	STAILQ_INSERT_TAIL(&flow->scb_unacked, scb, nextq);

#ifdef PSM_DEBUG
	/* update scb counters in flow. */
	flow->scb_num_pending++;
	flow->scb_num_unacked++;
#endif
}
MOCK_DEF_EPILOGUE(ips_proto_flow_enqueue);

/*
 * This function attempts to flush the current list of pending
 * packets through PIO.
 *
 * Recoverable errors:
 * PSM2_OK: Packet triggered through PIO.
 * PSM2_EP_NO_RESOURCES: No PIO bufs available or cable pulled.
 *
 * Unrecoverable errors:
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 * PSM2_EP_DEVICE_FAILURE: Chip failures, rxe/txe parity, etc.
 */
psm2_error_t
ips_proto_flow_flush_pio(struct ips_flow *flow, int *nflushed)
{
	struct ips_proto *proto = ((psm2_epaddr_t) (flow->ipsaddr))->proto;
	struct ips_scb_pendlist *scb_pend = &flow->scb_pend;
	int num_sent = 0;
	uint64_t t_cyc;
	ips_scb_t *scb;
	psm2_error_t err = PSM2_OK;

	psmi_assert(!SLIST_EMPTY(scb_pend));

	/* Out of credits - ACKs/NAKs reclaim recredit or congested flow */
	if_pf((flow->credits <= 0)
		) {
		if (nflushed)
			*nflushed = 0;
		return PSM2_EP_NO_RESOURCES;
	}

	while (!SLIST_EMPTY(scb_pend) && flow->credits > 0) {
		scb = SLIST_FIRST(scb_pend);
		psmi_assert(scb->nfrag == 1);
		GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
		if ((err = psmi_hal_spio_transfer_frame(proto, flow, scb,
							ips_scb_buffer(scb),
							scb->payload_size,
							PSMI_FALSE,
							scb->ips_lrh.flags &
							IPS_SEND_FLAG_PKTCKSUM,
							scb->cksum[0],
							proto->ep->context.psm_hw_ctxt
#ifdef PSM_CUDA
						   , IS_TRANSFER_BUF_GPU_MEM(scb)
#endif
			     ))
		    == PSM2_OK) {
			GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
			t_cyc = get_cycles();
			scb->scb_flags &= ~IPS_SEND_FLAG_PENDING;
			scb->ack_timeout = proto->epinfo.ep_timeout_ack;
			scb->abs_timeout = proto->epinfo.ep_timeout_ack + t_cyc;
			psmi_timer_request(proto->timerq, flow->timer_ack,
					   scb->abs_timeout);
			num_sent++;
			flow->credits--;
			SLIST_REMOVE_HEAD(scb_pend, next);
#ifdef PSM_DEBUG
			flow->scb_num_pending--;
#endif
			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&scb->ips_lrh,"PKT_STRM: err: %d", err);

		} else
		{
			GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
			break;
		}
	}

	/* If out of flow credits re-schedule send timer */
	if (!SLIST_EMPTY(scb_pend)) {
		proto->stats.pio_busy_cnt++;
		psmi_timer_request(proto->timerq, flow->timer_send,
				   get_cycles() + proto->timeout_send);
	}

	if (nflushed != NULL)
		*nflushed = num_sent;

	return err;
}

/*
 * Flush all packets currently marked as pending
 */
static psm2_error_t scb_dma_send(struct ips_proto *proto, struct ips_flow *flow,
				struct ips_scb_pendlist *slist, int *num_sent);

/*
 * Flush all packets queued up on a flow via send DMA.
 *
 * Recoverable errors:
 * PSM2_OK: Able to flush entire pending queue for DMA.
 * PSM2_OK_NO_PROGRESS: Flushed at least 1 but not all pending packets for DMA.
 * PSM2_EP_NO_RESOURCES: No scb's available to handle unaligned packets
 *                      or writev returned a recoverable error (no mem for
 *                      descriptors, dma interrupted or no space left in dma
 *                      queue).
 *
 * Unrecoverable errors:
 * PSM2_EP_DEVICE_FAILURE: Unexpected error calling writev(), chip failure,
 *			  rxe/txe parity error.
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 */
psm2_error_t
ips_proto_flow_flush_dma(struct ips_flow *flow, int *nflushed)
{
	struct ips_proto *proto = ((psm2_epaddr_t) (flow->ipsaddr))->proto;
	struct ips_scb_pendlist *scb_pend = &flow->scb_pend;
	ips_scb_t *scb = NULL;
	psm2_error_t err = PSM2_OK;
	int nsent = 0;

	psmi_assert(!SLIST_EMPTY(scb_pend));

	/* Out of credits - ACKs/NAKs reclaim recredit or congested flow */
	if_pf((flow->credits <= 0)
		) {
		if (nflushed)
			*nflushed = 0;
		return PSM2_EP_NO_RESOURCES;
	}

	// scb will descrbe header needed, which may be TID
	err = scb_dma_send(proto, flow, scb_pend, &nsent);
	if (err != PSM2_OK && err != PSM2_EP_NO_RESOURCES &&
	    err != PSM2_OK_NO_PROGRESS)
		goto fail;

	if (nsent > 0) {
		uint64_t t_cyc = get_cycles();
		int i = 0;
		/*
		 * inflight counter proto->iovec_cntr_next_inflight should not drift
		 * from completion counter proto->iovec_cntr_last_completed away too
		 * far because we only have very small scb counter compared with
		 * uint32_t counter value.
		 */
#ifdef PSM_DEBUG
		flow->scb_num_pending -= nsent;
#endif
		SLIST_FOREACH(scb, scb_pend, next) {
			if (++i > nsent)
				break;

			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&scb->ips_lrh,"PKT_STRM: (dma)");

			scb->scb_flags &= ~IPS_SEND_FLAG_PENDING;
			scb->ack_timeout =
			    scb->nfrag * proto->epinfo.ep_timeout_ack;
			scb->abs_timeout =
			    scb->nfrag * proto->epinfo.ep_timeout_ack + t_cyc;

			psmi_assert(proto->sdma_scb_queue
					[proto->sdma_fill_index] == NULL);
			proto->sdma_scb_queue[proto->sdma_fill_index] = scb;
			scb->dma_complete = 0;

			proto->sdma_avail_counter--;
			proto->sdma_fill_index++;
			if (proto->sdma_fill_index == proto->sdma_queue_size)
				proto->sdma_fill_index = 0;

			/* Flow credits can temporarily go to negative for
			 * packets tracking purpose, because we have sdma
			 * chunk processing which can't send exact number
			 * of packets as the number of credits.
			 */
			flow->credits -= scb->nfrag;
		}
		SLIST_FIRST(scb_pend) = scb;
	}

	if (SLIST_FIRST(scb_pend) != NULL) {
		psmi_assert(flow->scb_num_pending > 0);

		switch (flow->protocol) {
		case PSM_PROTOCOL_TIDFLOW:
			// for UD we use RC QP instead of STL100's TIDFLOW HW
			// UDP has no RDMA
			psmi_assert_always(0);	// we don't allocate ips_flow for TID

			break;
		case PSM_PROTOCOL_GO_BACK_N:
		default:
			if (flow->credits > 0) {
				/* Schedule send timer and increment writev_busy_cnt */
				psmi_timer_request(proto->timerq,
						   flow->timer_send,
						   get_cycles() +
						   (proto->timeout_send << 1));
				proto->stats.writev_busy_cnt++;
			} else {
				/* Schedule ACK timer to reap flow credits */
				psmi_timer_request(proto->timerq,
						   flow->timer_ack,
						   get_cycles() +
						   (proto->epinfo.
						    ep_timeout_ack >> 2));
			}
			break;
		}
	} else {
		/* Schedule ack timer */
		psmi_timer_cancel(proto->timerq, flow->timer_send);
		psmi_timer_request(proto->timerq, flow->timer_ack,
				   get_cycles() + proto->epinfo.ep_timeout_ack);
	}

	/* We overwrite error with its new meaning for flushing packets */
	if (nsent > 0)
		if (scb)
			err = PSM2_OK_NO_PROGRESS;	/* partial flush */
		else
			err = PSM2_OK;	/* complete flush */
	else
		err = PSM2_EP_NO_RESOURCES;	/* no flush at all */

fail:
	if (nflushed)
		*nflushed = nsent;

	return err;
}





/*
 * Caller still expects num_sent to always be correctly set in case of an
 * error.
 *
 * Recoverable errors:
 * PSM2_OK: At least one packet was successfully queued up for DMA.
 * PSM2_EP_NO_RESOURCES: No scb's available to handle unaligned packets
 *                      or writev returned a recoverable error (no mem for
 *                      descriptors, dma interrupted or no space left in dma
 *                      queue).
 * PSM2_OK_NO_PROGRESS: Cable pulled.
 *
 * Unrecoverable errors:
 * PSM2_EP_DEVICE_FAILURE: Error calling hfi_sdma_inflight() or unexpected
 *                        error in calling writev(), or chip failure, rxe/txe
 *                        parity error.
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 */
static
psm2_error_t
scb_dma_send(struct ips_proto *proto, struct ips_flow *flow,
	     struct ips_scb_pendlist *slist, int *num_sent)
{
	psmi_assert_always(0);	// should not get here
	return PSM2_INTERNAL_ERR;
}


psm2_error_t
ips_proto_timer_ack_callback(struct psmi_timer *current_timer,
			     uint64_t current)
{
	struct ips_flow *flow = ((ips_scb_t *)current_timer->context)->flow;
	struct ips_proto *proto = ((psm2_epaddr_t) (flow->ipsaddr))->proto;
	uint64_t t_cyc_next = get_cycles();
	psmi_seqnum_t err_chk_seq;
	ips_scb_t *scb, ctrlscb;
	uint8_t message_type;

	if (STAILQ_EMPTY(&flow->scb_unacked))
		return PSM2_OK;

	scb = STAILQ_FIRST(&flow->scb_unacked);

	if (current >= scb->abs_timeout) {
		int done_local = 0;

		done_local = 1;	/* Always done for PIO flows */

		scb->ack_timeout =
		    min(scb->ack_timeout * proto->epinfo.ep_timeout_ack_factor,
			proto->epinfo.ep_timeout_ack_max);
		scb->abs_timeout = t_cyc_next + scb->ack_timeout;
		if (done_local) {
			_HFI_VDBG
			    ("sending err_chk flow=%d with first=%d,last=%d\n",
			     flow->flowid,
			     STAILQ_FIRST(&flow->scb_unacked)->seq_num.psn_num,
			     STAILQ_LAST(&flow->scb_unacked, ips_scb,
					 nextq)->seq_num.psn_num);

			ctrlscb.scb_flags = 0;
			if (proto->flags & IPS_PROTO_FLAG_RCVTHREAD)
				ctrlscb.scb_flags |= IPS_SEND_FLAG_INTR;

			err_chk_seq = (SLIST_EMPTY(&flow->scb_pend)) ?
					flow->xmit_seq_num :
					SLIST_FIRST(&flow->scb_pend)->seq_num;

			if (flow->protocol == PSM_PROTOCOL_TIDFLOW) {
				// for UD we use RC QP instead of STL100's TIDFLOW HW
				// UDP has no RDMA
				psmi_assert_always(0);	// we don't allocate ips_flow for TID
				message_type = OPCODE_ERR_CHK;	// keep KlockWorks happy
			} else {
				PSM2_LOG_MSG("sending ERR_CHK message");
				message_type = OPCODE_ERR_CHK;
				err_chk_seq.psn_num = (err_chk_seq.psn_num - 1)
					& proto->psn_mask;
			}
			ctrlscb.ips_lrh.bth[2] =
					__cpu_to_be32(err_chk_seq.psn_num);

			ips_proto_send_ctrl_message(flow, message_type,
					&flow->ipsaddr->ctrl_msg_queued,
					&ctrlscb, ctrlscb.cksum, 0);
		}

		t_cyc_next = get_cycles() + scb->ack_timeout;
	} else
		t_cyc_next += (scb->abs_timeout - current);

	psmi_timer_request(proto->timerq, current_timer, t_cyc_next);

	return PSM2_OK;
}

psm2_error_t
ips_proto_timer_send_callback(struct psmi_timer *current_timer,
			      uint64_t current)
{
	struct ips_flow *flow = ((ips_scb_t *)current_timer->context)->flow;

	if (!SLIST_EMPTY(&flow->scb_pend))
		flow->flush(flow, NULL);

	return PSM2_OK;
}

