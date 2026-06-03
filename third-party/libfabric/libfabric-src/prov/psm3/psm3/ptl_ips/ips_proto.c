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

#define CREDITS_INC_THRESH 2048
// we are using 31 bits psn, and int16_t for psn diff on nak detection
// to play safe we set max credit to 16384
#define IPS_MAX_CREDIT 16384

static void ctrlq_init(struct ips_ctrlq *ctrlq, struct ips_proto *proto);

#ifdef PSM_HAVE_REG_MR
static psm2_error_t proto_sdma_init(struct ips_proto *proto);
#endif
static psm2_error_t ips_proto_register_stats(struct ips_proto *proto);

#ifdef PSM_HAVE_GPU
void psmi_gpu_hostbuf_alloc_func(int is_alloc, void *context, void *obj)
{
	struct ips_gpu_hostbuf *icb = (struct ips_gpu_hostbuf *)obj;
	if (is_alloc) {
		PSM3_GPU_HOSTBUF_LAZY_INIT(icb);
		icb->host_buf = NULL;
	} else {
		PSM3_GPU_HOSTBUF_DESTROY(icb);
	}
	return;
}
#endif /* PSM_HAVE_GPU */

static int parse_flow_credits(const char *str,
			size_t errstr_size, char errstr[],
			int tvals[3])
{
	psmi_assert(tvals);
	int ntup = psm3_count_tuples(str);
	int ret = psm3_parse_str_tuples(str, ntup, tvals);
	if (ret < 0)
		return ret;
        // back compatibility - when only one value specified, set max=min, step=0
        // this also can make value check to be accurate
	if (ntup == 1) {
		tvals[1] = tvals[0];
		tvals[2] = 0;
	}
	if (tvals[0] < 0 || tvals[1] < 0 || tvals[2] < 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Negative values not allowed");
		return -2;
	}
	if (tvals[0] > IPS_MAX_CREDIT || tvals[1] > IPS_MAX_CREDIT || tvals[2] > IPS_MAX_CREDIT) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Max allowed is %u", IPS_MAX_CREDIT);
		return -2;
	}
	if (tvals[0] == 0 || tvals[1] == 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Zero values not allowed on min, max");
		return -2;
	}
	if (tvals[1] > tvals[0] && tvals[2] == 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Zero values not allowed on adjust when max > min");
		return -2;
	}
	if (tvals[0] > tvals[1]) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " min (%d) must be <= max (%d)", tvals[0], tvals[1]);
		return -2;
	}
	return 0;
}

static int parse_check_flow_credits(int type,
			const union psmi_envvar_val val, void *ptr,
			size_t errstr_size, char errstr[])
{
	// parser will set tvals to result, use a copy to protect input of defaults
	int tvals[3] = { ((int*)ptr)[0], ((int*)ptr)[1], ((int*)ptr)[2]};
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR_TUPLES);
	return parse_flow_credits(val.e_str, errstr_size, errstr, tvals);
}

psm2_error_t
psm3_ips_proto_init(psm2_ep_t ep, const ptl_t *ptl,
	       int num_of_send_bufs, int num_of_send_desc, uint32_t imm_size,
	       const struct psmi_timer_ctrl *timerq,
	       const struct ips_epstate *epstate,
	       void *spioc, struct ips_proto *proto)
{
	uint32_t protoexp_flags, cksum_sz;
	union psmi_envvar_val env_cksum;
	psm2_error_t err = PSM2_OK;

	memset(proto, 0, sizeof(struct ips_proto));
	proto->ptl = (ptl_t *) ptl;
	proto->ep = ep;	/* cached */
	proto->mq = ep->mq;	/* cached */
	proto->pend_sends.proto = proto;
	psmi_timer_entry_init(&proto->pend_sends.timer,
			      psm3_ips_proto_timer_pendq_callback,
			      &proto->pend_sends);
	STAILQ_INIT(&proto->pend_sends.pendq);
	proto->epstate = (struct ips_epstate *)epstate;
	proto->timerq = (struct psmi_timer_ctrl *)timerq;
	proto->spioc = spioc;

	{
		union psmi_envvar_val env_flow_ctr;

		psm3_getenv("PSM3_ADV_FLOW_CONTROL",
			    "Enable advanced flow control. It enables psn sync based flow control for PSM3_RDMA=3\n"
			    "mode and dynamic credit for other RDMA modes",
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)1, &env_flow_ctr);
		proto->credits_allow_adv_ctrl = env_flow_ctr.e_uint;
	}

	{
		/* Number of credits per flow */
		union psmi_envvar_val env_flow_credits;
#ifdef PSM_VERBS
		int min_credits = IPS_PROTOEXP_FLAG_RDMA_QP(proto->ep->rdmamode) == IPS_PROTOEXP_FLAG_RDMA_USER_RC ?
				IPS_PROTO_FLOW_CREDITS_RC_MIN_DEFAULT : IPS_PROTO_FLOW_CREDITS_MIN_DEFAULT;
		int max_credits = IPS_PROTOEXP_FLAG_RDMA_QP(proto->ep->rdmamode) == IPS_PROTOEXP_FLAG_RDMA_USER_RC ?
				IPS_PROTO_FLOW_CREDITS_RC_MAX_DEFAULT : IPS_PROTO_FLOW_CREDITS_MAX_DEFAULT;
		int tvals[3] = {
			min(min_credits, num_of_send_desc),
			min(max_credits, num_of_send_desc),
			IPS_PROTO_FLOW_CREDITS_STEP_DEFAULT
		};
#else
		int tvals[3] = {
			min(IPS_PROTO_FLOW_CREDITS_MIN_DEFAULT, num_of_send_desc),
			min(IPS_PROTO_FLOW_CREDITS_MAX_DEFAULT, num_of_send_desc),
			IPS_PROTO_FLOW_CREDITS_STEP_DEFAULT
		};
#endif
		char fcredits_def[32];
		snprintf(fcredits_def, sizeof(fcredits_def), "%d:%d:%d", tvals[0], tvals[1], tvals[2]);

		(void)psm3_getenv_range("PSM3_FLOW_CREDITS",
			    "Number of unacked packets (credits) per flow in <min:max:adjust>",
			    "Specified as min:max:adjust where min and max is the range of credits,\n"
			    "and adjust is the adjustment amount for adjusting credits. For PSM3_RDMA=3,\n"
			    "adjust is ignored. Data send pauses when number of unacked packets is beyond\n"
			    "max credits, and send resumes when the number is below min credits",
			    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR_TUPLES,
			    (union psmi_envvar_val)fcredits_def,
			    (union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
			    parse_check_flow_credits, tvals,
			    &env_flow_credits);
		if (parse_flow_credits(env_flow_credits.e_str, 0, NULL, tvals) < 0) {
                	// already checked, shouldn't get parse errors nor empty strings
                	psmi_assert(0);
                }
                if (tvals[0] > num_of_send_desc) {
                	tvals[0] = num_of_send_desc;
                }
                if (tvals[1] > num_of_send_desc) {
                	tvals[1] = num_of_send_desc;
                }

                // set init flow credits. Use PSM2_FLOW_CREDITS when possible
		int df_flow_credits = min(PSM2_FLOW_CREDITS, num_of_send_desc);
		if (df_flow_credits > tvals[0] && df_flow_credits < tvals[1]) {
			proto->flow_credits = df_flow_credits;
		} else {
			proto->flow_credits = (tvals[0] + tvals[1]) / 2;
		}

		if (proto->credits_allow_adv_ctrl) {
			proto->min_credits = tvals[0];
			proto->max_credits = tvals[1];
			proto->credits_adjust = tvals[2];
		} else {
			// fixed credits
			proto->min_credits = proto->flow_credits;
			proto->max_credits = proto->flow_credits;
			proto->credits_adjust = 0;
		}
	}

	{
		union psmi_envvar_val env_thresh;
		psm3_getenv_range("PSM3_CREDITS_INC_THRESH",
			    "Threshold for increasing credits", NULL,
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)CREDITS_INC_THRESH,
			    (union psmi_envvar_val)0, (union psmi_envvar_val)IPS_MAX_CREDIT,
			    NULL, NULL, &env_thresh);
		proto->credits_inc_thresh = env_thresh.e_uint;
	}


	/*
	 * Checksum packets within PSM. Default is off.
	 * This is heavy weight and done in software so not recommended for
	 * production runs.
	 */
	psm3_getenv("PSM3_CHECKSUM",
		    "Enable checksum of messages (0 disables checksum)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		    (union psmi_envvar_val)0, &env_cksum);
	/* If checksums enabled we insert checksum at end of packet */
	cksum_sz = env_cksum.e_uint ? PSM_CRC_SIZE_IN_BYTES : 0;

	/* HAL specific initialization of ips_proto and HAL details
	 * (such as hash for dispersive routing, MTU, buffer pools, psn_mask,
	 * pktlen_mask, etc)) which require an ips_proto to hold the selections.
	 */
	err = psmi_hal_ips_proto_init(proto, cksum_sz);
	if (err)
		goto fail;
	// HAL will have adjusted ep->mtu to reflect cksum_sz
	// and final choice of maximum PSM payload.
	// HAL will have set: proto->flow_credit_bytes, proto->psn_mask,
	// proto->epinfo.ep_hash, proto->epinfo.ep_mtu, ep->chunk_max_size,
	// ep->chunk_max_segs, proto->iovec_*

	proto->timeout_send = us_2_cycles(IPS_PROTO_SPIO_RETRY_US_DEFAULT);
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

		psm3_getenv("PSM3_MULTIRAIL_THRESH_LOAD_BALANCE",
			    "Min packet size at which load balance for multi-rail (default is 0)",
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)0,
			    &env_thresh_load_balance);
		proto->multirail_thresh_load_balance = env_thresh_load_balance.e_uint;
	}

	/* Initialize IBTA related stuff (path record, etc.) */
	if ((err = psm3_ips_ibta_init(proto)))
		goto fail;

	{
		/* User asks for HFI loopback? */
		union psmi_envvar_val env_loopback;

		psm3_getenv("PSM3_NIC_LOOPBACK",
			"PSM uses NIC loopback [user should also omit shm from PSM3_DEVICES in this mode] (default is disabled i.e. 0)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			(union psmi_envvar_val)0, /* Disabled by default */
			&env_loopback);

		if (env_loopback.e_uint) {
			if (! psmi_hal_has_cap(PSM_HAL_CAP_NIC_LOOPBACK)) {
				_HFI_ERROR("Loopback mode not permitted\n");
				err = PSM2_PARAM_ERR;
				goto fail;
			}
			proto->flags |= IPS_PROTO_FLAG_LOOPBACK;
		}
	}


	{
		/* Disable coalesced ACKs? */
		union psmi_envvar_val env_coalesce_acks;

		psm3_getenv("PSM3_COALESCE_ACKS", "Coalesce ACKs on the wire (default is enabled i.e. 1)", PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS, (union psmi_envvar_val)1,	/* Enabled by default */
			    &env_coalesce_acks);

		if (env_coalesce_acks.e_uint)
			proto->flags |= IPS_PROTO_FLAG_COALESCE_ACKS;
	}

	// initialize sdma after PSM3_MR_CACHE_MODE
	proto->flags |= IPS_PROTO_FLAG_SPIO;

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
		psm3_mpool_get_obj_info(proto->ep->mq->sreq_pool, &chunks,
					&maxsz);

		proto->pend_sends_pool =
		    psm3_mpool_create(sizeof(struct ips_pend_sreq), chunks,
				      maxsz, 0, DESCRIPTORS, NULL, NULL);
		if (proto->pend_sends_pool == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}


	/*
	 * Register ips protocol statistics
	 */
	err = ips_proto_register_stats(proto);
	if (err != PSM2_OK)
		goto fail;

	/*
	 * Control Queue and messaging
	 */
	ctrlq_init(&proto->ctrlq, proto);

	/*
	 * Receive-side handling
	 */
	if ((err = psm3_ips_proto_recv_init(proto)))
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

		psm3_getenv("PSM3_BOUNCE_SZ",
			"PSM send bounce buffer size (default is 8192B)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)8192,
			&env_bbs);

		proto->scb_bufsize = env_bbs.e_uint;
	}

	if ((err = psm3_ips_scbctrl_init(proto, num_of_send_desc,
				    num_of_send_bufs, imm_size,
				    proto->scb_bufsize, NULL, NULL,
				    &proto->scbc_egr)))
		goto fail;

	/*
	 * Expected protocol handling.
	 * If we enable tid-based expected rendezvous, the expected protocol code
	 * handles its own rv scb buffers.  If not, we have to enable eager-based
	 * rendezvous and we allocate scb buffers for it.
	 * For verbs PSM3_RDMA (ep->rdmamode) controls our use of RDMA for Rendezvous
	 */
	protoexp_flags = proto->ep->rdmamode;	// PSM3_RDMA

	// protoexp implements RDMA for verbs.  N/A to sockets
	// when proto->protoexp is NULL, we will not attempt to use RDMA
	{
		(void)protoexp_flags;
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
		     psm3_ips_scbctrl_init(proto, num_of_send_desc,
				      0 /* no bufs */ ,
				      0, 0 /* bufsize==0 */ ,
				      psm3_ips_proto_rv_scbavail_callback,
				      proto, proto->scbc_rv)))
			goto fail;
	}
	if (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED) {
#ifdef PSM_HAVE_GPU
		PSM3_GPU_PREPARE_DTOH_MEMCPYS(proto);
#endif
		if ((err = psm3_ips_protoexp_init(proto, protoexp_flags,
					     num_of_send_bufs, num_of_send_desc,
					     &proto->protoexp)))
			goto fail;
	} else {
		proto->protoexp = NULL;
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
	if ((err = psm3_ips_proto_am_init(proto,
				     min(num_of_send_bufs, num_of_send_desc),
				     imm_size,
				     &proto->proto_am)))
		goto fail;

#ifdef PSM_HAVE_GPU
	psm3_gpu_is_gpudirect_enabled = psmi_parse_gpudirect();
	psm3_gpu_gpudirect_rdma_send_limit = psmi_parse_gpudirect_rdma_send_limit(0);
	psm3_gpu_gpudirect_rdma_recv_limit = psmi_parse_gpudirect_rdma_recv_limit(0);
#ifdef PSM_HAVE_RNDV_MOD
	if (psmi_hal_has_cap(PSM_HAL_CAP_GPUDIRECT))
		psm3_gpu_is_driver_gpudirect_enabled = 1;
	/* Check for mismatch between PSM3 and RV module */
	if (! psmi_hal_has_cap(PSM3_GPU_HAL_CAP_EXPECTED))
		psm3_gpu_is_driver_gpudirect_enabled = 0;
#else
	psm3_gpu_is_driver_gpudirect_enabled = 0;
#endif

	if (! psm3_gpu_is_gpudirect_enabled) {
		psm3_gpu_gpudirect_rdma_send_limit = psm3_gpu_gpudirect_rdma_recv_limit = 0;
	} else if (! PSM3_GPU_IS_ENABLED) {
		// should not happen since we test psmi_parse_gpudirect earlier
		// and it will trigger initialization of the proper GPU.  Then
		// we provide no disabling of the GPU per EP.
		_HFI_INFO("WARNING: Non-GPU application, PSM3_GPUDIRECT option ignored\n");
		psm3_gpu_is_gpudirect_enabled = 0;
		psm3_gpu_gpudirect_rdma_send_limit = psm3_gpu_gpudirect_rdma_recv_limit = 0;
	} else if (!PSM3_GPU_GPUDIRECT_SUPPORTED()) {
		_HFI_INFO("WARNING: GPU device does not support GPU Direct, PSM3_GPUDIRECT option ignored\n");
		psm3_gpu_is_gpudirect_enabled = 0;
		psm3_gpu_gpudirect_rdma_send_limit = psm3_gpu_gpudirect_rdma_recv_limit = 0;
	} else if (! PSM3_GPU_IS_DRIVER_GPUDIRECT_ENABLED) {
#ifdef PSM_HAVE_RNDV_MOD
		char buf[100];
		PSM3_GPU_RV_CAP_STRING(buf, sizeof(buf), PSM3_GPU_RV_CAPABILITY_EXPECTED);
		err = psm3_handle_error(PSMI_EP_NORETURN,
				PSM2_INTERNAL_ERR,
				"Unable to start run, PSM3_GPUDIRECT requires rv module with %s support.\n", buf);
#else
		err = psm3_handle_error(PSMI_EP_NORETURN,
				PSM2_INTERNAL_ERR,
				"Unable to start run, PSM3_GPUDIRECT requires rv module with GPU support.\n");
#endif
	} else if (!(protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED)) {
		// only GDR Copy and GPU Send DMA allowed
		psm3_gpu_gpudirect_rdma_send_limit = psm3_gpu_gpudirect_rdma_recv_limit = 0;
	} else {
		if (psm3_gpu_gpudirect_rdma_send_limit)
			proto->flags |= IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND;
		if (psm3_gpu_gpudirect_rdma_recv_limit)
			proto->flags |= IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV;
	}
	// from here forward can't use psmi_parse_gpudirect,
	// must use psm3_gpu_is_gpudirect_enabled

	/* The following cases need to be handled:
	 * 1) GPU DIRECT is turned off but GDR COPY is turned on by the user or
	 *    by default - Turn off GDR COPY
	 * 2) GPU DIRECT is turned on but App, GPU or RV doesn't support it
	 *    (tested above) - Turn off GDR COPY
	 * 2) GPU DIRECT is on but GDR COPY is turned off by the user - Leave
	 *.   this config as it is.
	 */
	if (!psm3_gpu_is_gpudirect_enabled)
		psm3_gpu_is_gdr_copy_enabled = psm3_gpu_gdr_copy_limit_send =
			psm3_gpu_gdr_copy_limit_recv = 0;
	/* technically this is not needed since we only consider GDRCopy Send
	 * for TINY, SHORT, and single MTU RTS payload.  But does no harm.
	 */
	psm3_gpu_gdr_copy_limit_send = min(psm3_gpu_gdr_copy_limit_send, proto->ep->mtu);

	if (PSM3_GPU_IS_ENABLED &&
		 (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED)) {
		struct psmi_rlimit_mpool rlim = GPU_HOSTBUFFER_LIMITS;
		uint32_t maxsz, chunksz, max_elements;
		uint32_t pool_num_obj_max_total;
		uint32_t small_pool_num_obj_max_total;

		if ((err = psm3_parse_mpool_env(proto->mq, 1,
						&rlim, &maxsz, &chunksz)))
			goto fail;

		/* the maxsz is the amount in MB, not the number of entries,
		 * since the element size depends on the window size */
		max_elements = (maxsz*1024*1024) / psm3_mq_max_window_rv(proto->mq, 1);
		/* mpool requires max_elements to be power of 2. round down. */
		max_elements = 1 << (31 - __builtin_clz(max_elements));
		/* need at least 3 buffers */
		max_elements = max(4, max_elements);
		proto->gpu_hostbuf_send_cfg.bufsz = psm3_mq_max_window_rv(proto->mq, 1);
		proto->gpu_hostbuf_pool_send =
			psm3_mpool_create_for_gpu(sizeof(struct ips_gpu_hostbuf),
						  chunksz, max_elements, 0,
						  UNDEFINED, NULL, NULL,
						  psmi_gpu_hostbuf_alloc_func,
						  (void *)
						  &proto->gpu_hostbuf_send_cfg);

		if (proto->gpu_hostbuf_pool_send == NULL) {
			err = psm3_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate GPU host send buffer pool");
			goto fail;
		}
		psm3_mpool_get_obj_info(proto->gpu_hostbuf_pool_send,
					NULL, &pool_num_obj_max_total);

		/* use the same number of elements for the small pool */
		proto->gpu_hostbuf_small_send_cfg.bufsz = GPU_SMALLHOSTBUF_SZ;
		proto->gpu_hostbuf_pool_small_send =
			psm3_mpool_create_for_gpu(sizeof(struct ips_gpu_hostbuf),
						  chunksz, max_elements, 0,
						  UNDEFINED, NULL, NULL,
						  psmi_gpu_hostbuf_alloc_func,
						  (void *)
						  &proto->gpu_hostbuf_small_send_cfg);

		if (proto->gpu_hostbuf_pool_small_send == NULL) {
			err = psm3_handle_error(proto->ep, PSM2_NO_MEMORY,
						"Couldn't allocate GPU host small send buffer pool");
			goto fail;
		}
		psm3_mpool_get_obj_info(proto->gpu_hostbuf_pool_small_send,
					NULL, &small_pool_num_obj_max_total);

		/* Configure the amount of prefetching */
		union psmi_envvar_val env_prefetch_limit;

		psm3_getenv("PSM3_GPU_PREFETCH_LIMIT",
			    "How many RDMA windows to prefetch at RTS time(default is 2)",
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)GPU_WINDOW_PREFETCH_DEFAULT,
			    &env_prefetch_limit);
		proto->gpu_prefetch_limit = env_prefetch_limit.e_uint;
		_HFI_DBG("GPU Send Copy Pipeline: %u of %u bytes (small), %u of %u bytes, prefetch %u\n",
			small_pool_num_obj_max_total,
			proto->gpu_hostbuf_small_send_cfg.bufsz,
			pool_num_obj_max_total,
			proto->gpu_hostbuf_send_cfg.bufsz,
			proto->gpu_prefetch_limit);
	}
#endif /* PSM_HAVE_GPU */

#ifdef PSM_HAVE_REG_MR
	// we allocate MR cache here (as opposed to in protoexp) because
	// we can use it for Send DMA as well as RDMA.
	// We also need to know GPU Direct Copy sizes for pri_size
	// For RDMA with cache mode NONE (no caching, just reference counting)
	// we still have an ep->mr_cache to track reference counting of MRs
	if ((protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED)
		|| proto->ep->mr_cache_mode) {
		union psmi_envvar_val env_mr_cache_entries;
		uint32_t default_cache_entries;
		union psmi_envvar_val env_mr_cache_size_mb;
		uint32_t default_cache_size_mb;	// in megabytes
		uint32_t cache_pri_entries;
		uint64_t cache_pri_size;	// in bytes
#ifdef PSM_HAVE_GPU
		uint64_t cache_gpu_pri_size;	// in bytes
		union psmi_envvar_val env_mr_cache_gpu_evict;
#endif

		// we can have at most HFI_TF_NFLOWS inbound RDMA and hfi_num_send_rdma
		// outbound RDMA.  Each of which potentially needs an MR. So
		// mr_cache_entries should be >= HFI_TF_NFLOWS + ep->hfi_num_send_rdma
		// but can survive if it's smaller as we will delay transfer til avail
		if (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED) {
			cache_pri_entries =  HFI_TF_NFLOWS + proto->ep->hfi_num_send_rdma;
			cache_pri_size  = (uint64_t)cache_pri_entries *
					psm3_mq_max_window_rv(proto->mq, 0);
			if (MR_CACHE_USER_CACHING(proto->ep->mr_cache_mode)) {
				// we attempt to cache, so can benefit from more than inflight
				// make enough room to have a good number of entries
				default_cache_entries = max(16384, cache_pri_entries * 16);
			} else {
				// we only reference count
				// could benefit from some extra so we can preregister MRs for
				// transfers we don't yet have resources for
				default_cache_entries = cache_pri_entries * 8;
			}
		} else {
			// just for non-priority send DMA
			cache_pri_entries =  0;
			cache_pri_size  = 0;
			if (MR_CACHE_USER_CACHING(proto->ep->mr_cache_mode)) {
				// we attempt to cache, so can benefit from more than inflight
				default_cache_entries = 128 * 16;
			} else {
				// we only reference count
				default_cache_entries = 128;
			}
		}
		/* Size of user space MR Cache
		 */
		if (MR_CACHE_USER_CACHING(proto->ep->mr_cache_mode)) {
			default_cache_size_mb = max(1024,
									(cache_pri_size+(1024*1024))/(1024*1024));
			psm3_getenv("PSM3_MR_CACHE_SIZE_MB",
				"user space MR cache size (megabytes)",
#ifdef UMR_CACHE
				PSMI_ENVVAR_LEVEL_USER,
#else
				PSMI_ENVVAR_LEVEL_HIDDEN,	// MR_CACHE_MODE_USER_NOINVAL hidden
#endif
				PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)default_cache_size_mb,
				&env_mr_cache_size_mb);
			// focus on possible size increase, footprint per entry is small
			// enough that even if downsize cache the entries don't take that
			// much space and cache byte limit will avoid extra entry allocation
			if (protoexp_flags & IPS_PROTOEXP_FLAG_ENABLED) {
				// size cache based on RDMA sizes, assume avg is 1/2 window
				// but >= thresh.  Note window is largest single RDMA but
				// MRs can be registered for the whole IO
				default_cache_entries = max(default_cache_entries,
								((uint64_t)env_mr_cache_size_mb.e_uint
									* (1024*1024))
										/ max(psm3_mq_max_window_rv(proto->mq, 0)/2,
												proto->mq->rndv_nic_thresh));
			} else {
				// only send DMA, size based on smaller MRs
				default_cache_entries = max(default_cache_entries,
								((uint64_t)env_mr_cache_size_mb.e_uint
									* (1024*1024))
										/ proto->epinfo.ep_mtu);
			}
		} else {
			env_mr_cache_size_mb.e_uint = 0;	// N/A
		}
		psm3_getenv("PSM3_MR_CACHE_SIZE",
				"user space MR table/cache size (num MRs)",
				PSMI_ENVVAR_LEVEL_USER,
				PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)default_cache_entries,
				&env_mr_cache_entries);
#ifdef PSM_HAVE_GPU
		// cache_gpu_pri_size only used to confirm RV GPU cache size
		// Without GPU Direct we will not register any GPU MRs
		// if we have GPU Direct w/o RDMA, no priority pin/MRs except
		// for GDRCopy
		// since GdrCopy doesn't use psm2_mr_cache, no need to
		// grow pri_entries to account for it
		// Note cache_pri_size == 0 if rdmamode not enabled
		cache_gpu_pri_size = 0;
		if (PSM3_GPU_IS_ENABLED && psm3_gpu_is_gpudirect_enabled) {
			if (psm3_gpu_gpudirect_rdma_send_limit || psm3_gpu_gpudirect_rdma_recv_limit)
				cache_gpu_pri_size = cache_pri_size;
			if (psm3_gpu_gdr_copy_limit_send || psm3_gpu_gdr_copy_limit_recv) {
				// min of one extra for GDRCopy
				// largest recv with GDR copy is psm3_gpu_gdr_copy_limit_recv
				// largest send with GDR copy is psm3_gpu_gdr_copy_limit_send
				cache_gpu_pri_size +=
					ROUNDUP64P2(max(proto->epinfo.ep_mtu,
							max(psm3_gpu_gdr_copy_limit_recv,
							psm3_gpu_gdr_copy_limit_send)),
						PSMI_GPU_PAGESIZE);
			}
			psm3_getenv("PSM3_RV_GPU_CACHE_EVICT",
				"Number of kilobytes to evict from GPU cache if can't pin memory for GPUDIRECT (0=just exact amount needed))",
				PSMI_ENVVAR_LEVEL_USER,
				PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)0, &env_mr_cache_gpu_evict);
			psm3_gpu_cache_evict = (uint64_t)env_mr_cache_gpu_evict.e_uint * 1024;
		}

#endif /* PSM_HAVE_GPU */
		proto->ep->mr_cache = proto->mr_cache
					= psm3_verbs_alloc_mr_cache(proto->ep,
						env_mr_cache_entries.e_uint, proto->ep->mr_cache_mode,
						env_mr_cache_size_mb.e_uint,
						cache_pri_entries, cache_pri_size
#ifdef PSM_HAVE_GPU
						, cache_gpu_pri_size
#endif
						);
		if (! proto->mr_cache) {
			_HFI_ERROR( "Unable to allocate MR cache (%u entries)\n",
					env_mr_cache_entries.e_uint);
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}
	// Send DMA only makes sense if we have a real MR cache
	if (proto->ep->mr_cache_mode) {
		if ((err = proto_sdma_init(proto)))
			goto fail;
	} else {
		if (psm3_parse_senddma())
			_HFI_INFO("WARNING: Send DMA requires an MR Cache, disabling PSM3_SDMA\n");
		proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking =
		    ~0U;
#ifdef PSM_HAVE_GPU
		proto->iovec_gpu_thresh_eager = proto->iovec_gpu_thresh_eager_blocking =
		    ~0U;
#endif
	}
	// without a real cache, Send DMA makes no sense
	psmi_assert(proto->ep->mr_cache_mode || proto->iovec_thresh_eager == ~0);
	psmi_assert(proto->ep->mr_cache_mode || proto->iovec_thresh_eager_blocking == ~0U);
#ifdef PSM_HAVE_GPU
	// without a real cache, GPU Direct Send DMA makes no sense
	psmi_assert(proto->ep->mr_cache_mode || proto->iovec_gpu_thresh_eager == ~0);
	psmi_assert(proto->ep->mr_cache_mode || proto->iovec_gpu_thresh_eager_blocking == ~0U);
#endif
#endif /* PSM_HAVE_REG_MR */

#ifdef PSM_HAVE_GPU
	_HFI_DBG("GPU ("PSM3_GPU_TYPES") Enabled %d (%s) GPU Direct support: driver %d GPU device %d\n",
		PSM3_GPU_IS_ENABLED, PSM3_GPU_TYPE, psm3_gpu_is_driver_gpudirect_enabled, PSM3_GPU_GPUDIRECT_SUPPORTED());
	_HFI_DBG("GDR Copy: %d limit send=%u recv=%u gpu_rndv=%u GPU RDMA flags=0x%x limit send=%u recv=%u\n",
		psm3_gpu_is_gdr_copy_enabled, psm3_gpu_gdr_copy_limit_send, psm3_gpu_gdr_copy_limit_recv,
		psm3_gpu_thresh_rndv,
		proto->flags & (IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV
				|IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND),
		psm3_gpu_gpudirect_rdma_send_limit, psm3_gpu_gpudirect_rdma_recv_limit);
#ifdef PSM_HAVE_REG_MR
	_HFI_DBG("send dma thresh: %u %u GPU send DMA thresh %u %u\n",
		proto->iovec_thresh_eager, proto->iovec_thresh_eager_blocking,
		proto->iovec_gpu_thresh_eager,
		proto->iovec_gpu_thresh_eager_blocking);
#endif
#else /* PSM_HAVE_GPU */
#ifdef PSM_HAVE_REG_MR
	_HFI_DBG("send dma thresh: %u %u\n", proto->iovec_thresh_eager,
		proto->iovec_thresh_eager_blocking);
#endif
#endif /* PSM_HAVE_GPU */
#ifdef PSM_HAVE_REG_MR
	_HFI_DBG("rdma: %u MR cache %u\n", proto->ep->rdmamode,
		proto->ep->mr_cache_mode);
#endif

fail:
	return err;
}

psm2_error_t
psm3_ips_proto_disconnect_all(struct ips_proto *proto, int force,
							uint64_t timeout_in)
{
	struct psmi_eptab_iterator itor;
	uint64_t t_start;
	uint64_t t_grace_start, t_grace_time, t_grace_interval;
	psm2_epaddr_t epaddr;
	psm2_error_t err = PSM2_OK;
	int i;
	union psmi_envvar_val grace_intval;

	/* Poll one more time to attempt to synchronize with the peer ep's. */
	proto->ep->ptl_ips.ep_poll(proto->ptl, 0, 1);

	if (! psm3_getenv("PSM3_CLOSE_GRACE_PERIOD",
		    "Additional grace period in seconds for closing end-point.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)0, &grace_intval)) {
		t_grace_time = grace_intval.e_uint * NSEC_PER_SEC;
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
	if (! psm3_getenv("PSM3_CLOSE_GRACE_INTERVAL",
		    "Grace interval in seconds for closing end-point.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)0, &grace_intval)) {
		t_grace_interval = grace_intval.e_uint * NSEC_PER_SEC;
	} else {
		/* A heuristic is used to scale up the timeout linearly with
		 * the number of endpoints, and we allow one second per 1000
		 * endpoints. */
		t_grace_interval = (proto->ep->connections * NSEC_PER_SEC) / 1000;
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

		psm3_epid_itor_init(&itor, proto->ep);
		while ((epaddr = psm3_epid_itor_next(&itor))) {
			if (epaddr->ptlctl->ptl == proto->ptl)
				num_disc++;
		}
		psm3_epid_itor_fini(&itor);
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
		psm3_epid_itor_init(&itor, proto->ep);
		i = 0;
		while ((epaddr = psm3_epid_itor_next(&itor))) {
			/*
			 * if cstate_outgoing is CSTATE_NONE, then we know it
			 * is an uni-directional connect, in that the peer
			 * sent a connect request to us, but we never sent one
			 * out to the peer epid. Ignore handling those in
			 * psm3_ips_proto_disconnect() as we will do the right thing
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
		psm3_epid_itor_fini(&itor);
		err = psm3_ips_proto_disconnect(proto, force, num_disc, epaddr_array,
					   mask, errs, timeout_in);
		psmi_free(mask);
		psmi_free(errs);
		psmi_free(epaddr_array);
	}

	t_grace_start = get_cycles();

	// The grace interval is >= PSMI_MIN_EP_CLOSE_TIMEOUT (1 second)
	// and the time wait is < 1 second, so by waiting 1 extra
	// grace interval during which no inbound disconnect requests arrive,
	// we can also be sure time wait for any prior inbound or outbound
	// disconnects have also completed
	while (psm3_cycles_left(t_grace_start, t_grace_time)) {
		uint64_t t_grace_interval_start = get_cycles();
		int num_disconnect_requests = proto->num_disconnect_requests;
		PSMI_BLOCKUNTIL(
			proto->ep, err,
			proto->num_connected_incoming == 0 ||
			(!psm3_cycles_left(t_start, timeout_in) &&
			 (!psm3_cycles_left(t_grace_interval_start,
					    t_grace_interval) ||
			  !psm3_cycles_left(t_grace_start, t_grace_time))));
		if (num_disconnect_requests == proto->num_disconnect_requests) {
			/* nothing happened in this grace interval so break out early */
			break;
		}
	}
	err = PSM2_OK;	// ignore disconnect errors

#if _HFI_DEBUGGING
	if (_HFI_PRDBG_ON) {
		uint64_t t_grace_finish = get_cycles();

		_HFI_PRDBG_ALWAYS(
			"Closing endpoint disconnect left to=%d,from=%d after %d millisec of grace (out of %d)\n",
			proto->num_connected_outgoing, proto->num_connected_incoming,
			(int)(cycles_to_nanosecs(t_grace_finish - t_grace_start) / NSEC_PER_MSEC),
			(int)(t_grace_time / MSEC_PER_SEC));
	}
#endif
fail:
	return err;
}

psm2_error_t
psm3_ips_proto_fini(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	PSM3_GPU_SHUTDOWN_DTOH_MEMCPYS(proto);

	if ((err = psm3_ips_ibta_fini(proto)))
		goto fail;

	if ((err = psm3_ips_proto_am_fini(&proto->proto_am)))
		goto fail;

	if ((err = psm3_ips_scbctrl_fini(&proto->scbc_egr)))
		goto fail;

	psm3_ips_proto_recv_fini(proto);

	if (proto->protoexp) {
		if ((err = psm3_ips_protoexp_fini(proto->protoexp)))
			goto fail;
	}
	if (proto->scbc_rv) {
		psm3_ips_scbctrl_fini(proto->scbc_rv);
		psmi_free(proto->scbc_rv);
	}

#ifdef PSM_HAVE_REG_MR
	if (proto->mr_cache) {
		psm3_verbs_free_mr_cache(proto->mr_cache);
		proto->ep->mr_cache = proto->mr_cache = NULL;
	}
#endif
	psm3_stats_deregister_type(PSMI_STATSTYPE_IPSPROTO, proto);

	psm3_mpool_destroy(proto->pend_sends_pool);


fail:
	proto->t_fini = proto->t_init = 0;
	return err;
}

#if   defined(PSM_HAVE_REG_MR)
static
psm2_error_t
proto_sdma_init(struct ips_proto *proto)
{
	union psmi_envvar_val env_sdma, env_hfiegr;
	psm2_error_t err = PSM2_OK;

	env_sdma.e_uint = psm3_parse_senddma();
	if (!env_sdma.e_uint) {
		proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking =
		    ~0U;
	} else if (! psm3_verbs_mr_cache_allows_user_mr(proto->mr_cache)) {
		_HFI_INFO("WARNING: Cache does not allow user MRs, disabling PSM3_SDMA (check rv enable_user_mr)\n");
		proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking =
		    ~0U;
	} else {
		if (!psm3_getenv("PSM3_MQ_EAGER_SDMA_THRESH",
				"UD copy-to-sdma eager switchover threshold",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val) proto->iovec_thresh_eager,
				&env_hfiegr)) {
			proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking =
				 env_hfiegr.e_uint;
		}
	}

#ifdef PSM_HAVE_GPU
	if (! psm3_gpu_is_gpudirect_enabled
	    || !psmi_hal_has_cap(PSM_HAL_CAP_GPUDIRECT_SDMA))
		env_sdma.e_uint = 0;
	else
		psm3_getenv("PSM3_GPUDIRECT_SDMA",
		    "UD GPU send dma flags (0 disables send dma, 1 enables), default 1",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		    (union psmi_envvar_val)1, &env_sdma);
	if (!env_sdma.e_uint) {
		proto->iovec_gpu_thresh_eager = proto->iovec_gpu_thresh_eager_blocking =
		    ~0U;
	} else if (! psm3_verbs_mr_cache_allows_user_mr(proto->mr_cache)) {
		_HFI_INFO("WARNING: Cache does not allow user MRs, disabling PSM3_GPUDIRECT_SDMA (check rv enable_user_mr)\n");
		proto->iovec_gpu_thresh_eager = proto->iovec_gpu_thresh_eager_blocking =
		    ~0U;
	} else {
		if (!psm3_getenv("PSM3_GPU_MQ_EAGER_SDMA_THRESH",
				"UD GPU copy-to-sdma eager switchover threshold",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val) proto->iovec_gpu_thresh_eager,
				&env_hfiegr)) {
			proto->iovec_gpu_thresh_eager = proto->iovec_gpu_thresh_eager_blocking =
				 env_hfiegr.e_uint;
		}
	}
#endif /* PSM_HAVE_GPU */

	return err;
}
#endif

static
void ctrlq_init(struct ips_ctrlq *ctrlq, struct ips_proto *proto)
{
	/* clear the ctrl send queue */
	memset(ctrlq, 0, sizeof(*ctrlq));

	proto->message_type_to_mask[OPCODE_ACK] = CTRL_MSG_ACK_QUEUED;
	proto->message_type_to_mask[OPCODE_NAK] = CTRL_MSG_NAK_QUEUED;
	proto->message_type_to_mask[OPCODE_BECN] = CTRL_MSG_BECN_QUEUED;
	proto->message_type_to_mask[OPCODE_ERR_CHK] = CTRL_MSG_ERR_CHK_QUEUED;
	proto->message_type_to_mask[OPCODE_CONNECT_REQUEST] =
	    CTRL_MSG_CONNECT_REQUEST_QUEUED;
	proto->message_type_to_mask[OPCODE_CONNECT_REPLY] =
	    CTRL_MSG_CONNECT_REPLY_QUEUED;
	proto->message_type_to_mask[OPCODE_DISCONNECT_REQUEST] =
	    CTRL_MSG_DISCONNECT_REQUEST_QUEUED;
	proto->message_type_to_mask[OPCODE_DISCONNECT_REPLY] =
	    CTRL_MSG_DISCONNECT_REPLY_QUEUED;

	ctrlq->ctrlq_head = ctrlq->ctrlq_tail = 0;
	//ctrlq->ctrlq_overflow = 0;
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
			      psm3_ips_proto_timer_ctrlq_callback, ctrlq);

	return;
}

static __inline__ void _build_ctrl_message(struct ips_proto *proto,
			struct ips_flow *flow, uint8_t message_type,
			ips_scb_t *ctrlscb, uint32_t paylen)
{
	__be32 slid, dlid;
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
	p_hdr->lrh[2] = ips_proto_bytes_to_lrh2_be(proto,
				sizeof(struct ips_message_header) +
				HFI_CRC_SIZE_IN_BYTES + paylen);
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
psm3_ips_proto_timer_ctrlq_callback(struct psmi_timer *timer, uint64_t t_cyc_expire)
{
	struct ips_ctrlq *ctrlq = (struct ips_ctrlq *)timer->context;
	struct ips_proto *proto = ctrlq->ctrlq_proto;
	struct ips_ctrlq_elem *cqe;
	uint32_t have_cksum = proto->flags & IPS_PROTO_FLAG_CKSUM;
	psm2_error_t err;

	/* service ctrl send queue first */
	while (ctrlq->ctrlq_cqe[ctrlq->ctrlq_tail].msg_queue_mask) {
		cqe = &ctrlq->ctrlq_cqe[ctrlq->ctrlq_tail];
#ifdef PSM_FI
		if_pf(PSM3_FAULTINJ_ENABLED_EP(proto->ep)) {
			PSM3_FAULTINJ_STATIC_DECL(fi_sendfullcb, "sendfullcb",
				"no more send resources during ctrl timer cb",
				1, IPS_FAULTINJ_SENDFULLCB);
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendfullcb, proto->ep, "")) {
				err = PSM2_EP_NO_RESOURCES;
				goto sendfullcb;
			}
		}
#endif // PSM_FI


		GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
		err = psmi_hal_transfer_frame(proto,
						   cqe->msg_scb.flow, &cqe->msg_scb,
						   cqe->msg_scb.cksum, 0, PSMI_TRUE,
						   have_cksum, cqe->msg_scb.cksum[0]
#ifdef PSM_HAVE_GPU
			       , 0
#endif
				);
		GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
#ifdef PSM_FI
sendfullcb:
#endif

		if (err == PSM2_OK) {
			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&cqe->msg_scb.ips_lrh,"PKT_STRM: err: %d", err);
			ips_proto_epaddr_stats_set(proto, cqe->message_type);
			*cqe->msg_queue_mask &=
			    ~(proto->message_type_to_mask[cqe->message_type]);
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
	psm3_mq_mtucpy(&cqe->msg_scb.ips_lrh,
		       &ctrlscb->ips_lrh, sizeof(ctrlscb->ips_lrh));
	cqe->msg_scb.flow = flow;
	cqe->msg_scb.cksum[0] = ctrlscb->cksum[0];
}

psm2_error_t
psm3_ips_proto_send_ctrl_message(struct ips_flow *flow, uint8_t message_type,
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

	// cover for callers who don't fully init ctrlscb
	ctrlscb->nfrag = 1;
	ctrlscb->chunk_size = paylen;

	/* Drain queue if non-empty */
	if (cqe[ctrlq->ctrlq_tail].msg_queue_mask)
		psm3_ips_proto_timer_ctrlq_callback(&ctrlq->ctrlq_timer, 0ULL);

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
		psmi_assert(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
		flow = &ipsaddr->flows[proto->msgflowid];
	}

#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(proto->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sendfullctrl, "sendfullctrl",
			"no more send resources for ctrl message",
			1, IPS_FAULTINJ_SENDFULLCTRL);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendfullctrl, proto->ep, "")) {
			err = PSM2_EP_NO_RESOURCES;
			goto sendfullctrl;
		}
	}
#endif // PSM_FI
	GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
	err = psmi_hal_transfer_frame(proto, flow,
						   ctrlscb, payload, paylen,
						   PSMI_TRUE, have_cksum, ctrlscb->cksum[0]
#ifdef PSM_HAVE_GPU
						   , 0
#endif
			     );
	GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
#ifdef PSM_FI
sendfullctrl:
#endif
	if (err == PSM2_OK)
	{
		PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&ctrlscb->ips_lrh,"PKT_STRM: err: %d", err);
		ips_proto_epaddr_stats_set(proto, message_type);
	}

	_HFI_VDBG("transfer_frame of opcode=%x,remote_lid=%d,"
		  "src=%p,len=%d returns %d\n",
		  (int)_get_proto_hfi_opcode(&ctrlscb->ips_lrh),
		  __be16_to_cpu(ctrlscb->ips_lrh.lrh[1]), payload, paylen, err);

	if (err != PSM2_EP_NO_RESOURCES)
		return err;
	proto->stats.pio_busy_cnt++;

	/* to limit the performance penalty when transfer_frame is out
	 * of resources, we can queue a modest number of zero payload
	 * control messages (up to CTRL_MSG_QEUEUE_SIZE).  We only queue
	 * those identified in ctrl_msg_queue_enqueue (ACK, NAK, BECN)
	 * Each remote ipsaddr can have exactly 1 of each message type queued
	 * as tracked in the ipsaddr->ctrl_msg_queued mask, passed in as
	 * *msg_queue_mask here.  For the queued ACK, flow->ack_index indicates
	 * where in the queue the ACK has been queued, this way we can update
	 * the ACK's sequence number, essentially replacing the queued ACK with
	 * the newer one we now want to queue.
	 * If the queue overflows, the outbound message is lost and the caller
	 * must handle retries.  For lost ACK and NAK, this will depend on
	 * a remote timeout and ERR_CHK.
	 */
	if (proto->ctrl_msg_queue_enqueue & proto->
	    message_type_to_mask[message_type]) {
		/* We only queue control msg without payload */
		psmi_assert(paylen == 0);

		if ((*msg_queue_mask) & proto->
		    message_type_to_mask[message_type]) {

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
			    proto->message_type_to_mask[message_type];

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

void MOCKABLE(psm3_ips_proto_flow_enqueue)(struct ips_flow *flow, ips_scb_t *scb)
{
	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	struct ips_proto *proto = ((psm2_epaddr_t) ipsaddr)->proto;

	ips_scb_prepare_flow_inner(proto, ipsaddr, flow, scb);
	if ((proto->flags & IPS_PROTO_FLAG_CKSUM) &&
	    (scb->nfrag == 1)) {
		scb->ips_lrh.flags |= IPS_SEND_FLAG_PKTCKSUM;
		ips_do_cksum(proto, &scb->ips_lrh,
			     ips_scb_buffer(scb), scb->payload_size, &scb->cksum[0]);
	}

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
MOCK_DEF_EPILOGUE(psm3_ips_proto_flow_enqueue);

/*
 * This function attempts to flush the current list of pending
 * packets through PIO.
 * For HAl with segmentation in PIO flow (UDP w/GSO) an scb can have nfrag>1
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
psm3_ips_proto_flow_flush_pio(struct ips_flow *flow, int *nflushed)
{
	struct ips_proto *proto = ((psm2_epaddr_t) (flow->ipsaddr))->proto;
	struct ips_scb_pendlist *scb_pend = &flow->scb_pend;
	int num_sent = 0;
	uint64_t t_cyc;
	ips_scb_t *scb;
	psm2_error_t err = PSM2_OK;

	psmi_assert(!SLIST_EMPTY(scb_pend));

	/* Out of credits - ACKs/NAKs reclaim recredit or congested flow */
	if_pf(flow->credits <= 0
#ifdef PSM_BYTE_FLOW_CREDITS
		 || flow->credit_bytes <= 0
#endif
		) {
		if (nflushed)
			*nflushed = 0;
		flow->ipsaddr->cc_count += 1;
		if (flow->ipsaddr->cc_count > proto->stats.pio_cc_max_duration)
			proto->stats.pio_cc_max_duration = flow->ipsaddr->cc_count;
		return PSM2_EP_NO_RESOURCES;
	}
	flow->ipsaddr->cc_count = 0;

	while (!SLIST_EMPTY(scb_pend) && flow->credits > 0
#ifdef PSM_BYTE_FLOW_CREDITS
		&& flow->credit_bytes > 0
#endif
		) {
		scb = SLIST_FIRST(scb_pend);
#ifdef PSM_FI
		if_pf(PSM3_FAULTINJ_ENABLED_EP(proto->ep)) {
			PSM3_FAULTINJ_STATIC_DECL(fi_sendfull, "sendfull",
				"no more send resources pio flush",
				1, IPS_FAULTINJ_SENDFULL);
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendfull, proto->ep, "")) {
				err = PSM2_EP_NO_RESOURCES;
				goto sendfull;
			}
		}
#endif // PSM_FI
		GENERIC_PERF_BEGIN(PSM_TX_SPEEDPATH_CTR); /* perf stats */
		if ((err = psmi_hal_spio_transfer_frame(proto, flow, scb,
							ips_scb_buffer(scb),
							scb->payload_size,
							PSMI_FALSE,
							scb->ips_lrh.flags &
							IPS_SEND_FLAG_PKTCKSUM,
							scb->cksum[0]
#ifdef PSM_HAVE_GPU
						   , IS_TRANSFER_BUF_GPU_MEM(scb)
#endif
			     ))
		    == PSM2_OK) {
			GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
			flow->ipsaddr->cc_count = 0;
			t_cyc = get_cycles();
			scb->scb_flags &= ~IPS_SEND_FLAG_PENDING;
			scb->ack_timeout = scb->nfrag * proto->epinfo.ep_timeout_ack;
			scb->abs_timeout = scb->ack_timeout + t_cyc;
			psmi_timer_request(proto->timerq, &flow->timer_ack, scb->abs_timeout);
			num_sent++;
			/* Flow credits can temporarily go to negative for
			 * packets tracking purpose, because we have GSO/sdma
			 * chunk processing which can't send exact number
			 * of packets as the number of credits.
			 */
			flow->credits -= scb->nfrag;
#ifdef PSM_BYTE_FLOW_CREDITS
			flow->credit_bytes -= scb->chunk_size;
			_HFI_VDBG("after send: credits %d bytes %d sent %u bytes %u\n",
					flow->credits, flow->credit_bytes,
					scb->nfrag, scb->chunk_size);
#else
			_HFI_VDBG("after send: credits %d sent %u bytes %u\n",
					flow->credits,
					scb->nfrag, scb->chunk_size);
#endif
			SLIST_REMOVE_HEAD(scb_pend, next);
#ifdef PSM_DEBUG
			flow->scb_num_pending--;
#endif
			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&scb->ips_lrh,"PKT_STRM: payload_size=%d err: %d",
				scb->payload_size, err);
		} else if (err == PSM2_RELIABLE_DATA_SENT) {
			// no credits and timers
			// TDB - implement credits for reliable send
			GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
			scb->scb_flags &= ~IPS_SEND_FLAG_PENDING;
			num_sent++;
			SLIST_REMOVE_HEAD(scb_pend, next);
#ifdef PSM_DEBUG
			flow->scb_num_pending--;
#endif
			PSM2_LOG_PKT_STRM(PSM2_LOG_TX,&scb->ips_lrh,"PKT_STRM: payload_size=%d err: %d",
				scb->payload_size, err);

			// immediately ack the msg
			struct ips_scb_unackedq *unackedq = &flow->scb_unacked;
			flow->xmit_ack_num.psn_num = 1 + (__be32_to_cpu(scb->ips_lrh.bth[2]) & proto->psn_mask);
			flow->xmit_ack_num.psn_num &= proto->psn_mask;

			psmi_assert(scb == STAILQ_FIRST(unackedq));
			STAILQ_REMOVE_HEAD(unackedq, nextq);
#ifdef PSM_DEBUG
			flow->scb_num_unacked--;
			psmi_assert(flow->scb_num_unacked >= flow->scb_num_pending);
#endif
			if (scb->callback)
				(*scb->callback) (scb->cb_param, scb->nfrag > 1 ?
						  scb->chunk_size : scb->payload_size);

			psm3_ips_scbctrl_free(scb);

			if (STAILQ_EMPTY(unackedq)) {
				/* even though we sent reliably, the ACK timer may still be
				 * running if reliable SCBs were placed on queue after
				 * unreliable SCBs.  when the unreliable SCBs get ACKed while
				 * queue is not empty, the timers will not be terminated,
				 * leaving behind a queue of reliable SCBs with a running timer.
				 */
				psmi_timer_cancel(proto->timerq, &flow->timer_ack);
				psmi_timer_cancel(proto->timerq, &flow->timer_send);

				SLIST_FIRST(scb_pend) = NULL;
				psmi_assert(flow->scb_num_pending == 0);
			}

			err = PSM2_OK;
		} else {
			GENERIC_PERF_END(PSM_TX_SPEEDPATH_CTR); /* perf stats */
#ifdef PSM_FI
sendfull:
#endif
			break;
		}
	}

	/* If out of flow credits re-schedule send timer */
	if (!SLIST_EMPTY(scb_pend)) {
#ifdef PSM_BYTE_FLOW_CREDITS
		// if out of both, only report no_pkt flow credits
		if (flow->credits <= 0)
			proto->stats.pio_no_flow_credits++;
		else
			proto->stats.pio_no_flow_credit_bytes++;
#else
		proto->stats.pio_no_flow_credits++;
#endif
		if (flow->credits <= 0) {
//			_HFI_VDBG("flow=%p next=%d first_os=%d delta=%d\n", flow,
//				flow->xmit_seq_num.psn_num, flow->credits_inc_psn,
//				flow->xmit_seq_num.psn_num - flow->credits_inc_psn);
			if (flow->max_credits < proto->max_credits && !between(flow->credits_inc_psn,
				(flow->credits_inc_psn + proto->credits_inc_thresh) & proto->psn_mask,
				flow->xmit_seq_num.psn_num)) {
				// adjust with a small "random" number to avoid potential oscillation
				uint16_t actual_adjust = min(proto->credits_adjust + (flow->xmit_seq_num.psn_num & 0xF),
					proto->max_credits - flow->max_credits);
				flow->max_credits += actual_adjust;
				flow->credits += actual_adjust;
				flow->credits_inc_psn = flow->xmit_seq_num.psn_num;
				_HFI_VDBG("Increased flow (%p) credits to %d\n", flow, flow->max_credits);
			}
		}
		psmi_timer_request(proto->timerq, &flow->timer_send,
				   get_cycles() + proto->timeout_send);
	}

	if (nflushed != NULL)
		*nflushed = num_sent;

	return err;
}


#ifdef PSM_HAVE_SDMA
/*
 * Because we only lazily reap send dma completions, it's possible that we
 * receive a packet's remote acknowledgement before seeing that packet's local
 * completion.  As part of processing ack packets and releasing scbs, we issue
 * a wait for the local completion if the scb is marked as having been sent via
 * send dma.
 */
psm2_error_t
ips_proto_dma_wait_until(struct ips_proto *proto, ips_scb_t *scb)
{
	psm2_error_t err = PSM2_OK;
	int spin_cnt = 0;
	int did_yield = 0;

	_HFI_VDBG("Wait for DMA complete: %d\n", scb->sdma_outstanding);
	PSMI_PROFILE_BLOCK();

	do {
		if (spin_cnt++ == proto->ep->yield_spin_cnt) {
			/* Have to yield holding the PSM lock, mostly because we don't
			 * support another thread changing internal state at this point in
			 * the code.
			 */
			did_yield = 1;
			spin_cnt = 0;
			sched_yield();
		}

		err = psmi_hal_drain_sdma_completions(proto);
		if (err)
			break;
	} while (scb->sdma_outstanding);

	if (did_yield)
		proto->stats.sdma_compl_yield++;

	PSMI_PROFILE_UNBLOCK();

	return err;
}
#endif /* PSM_HAVE_SDMA */

psm2_error_t
psm3_ips_proto_timer_ack_callback(struct psmi_timer *current_timer,
			     uint64_t current)
{
	struct ips_flow *flow = current_timer->context;
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

#ifdef PSM_HAVE_SDMA
		/* We have to ensure that the send is at least locally complete before
		 * sending an error check or else earlier data can get to the
		 * destination *after* we pio or dma this err_chk.
		 */
		if (scb->sdma_outstanding) {
			/* error is caught inside this routine */
			psmi_hal_drain_sdma_completions(proto);
			if (!scb->sdma_outstanding)
				done_local = 1;
			else
				proto->stats.sdma_compl_slow++;
		} else
#endif /* PSM_HAVE_SDMA */
		{
			// TBD, if out of credits we can have some on unacked queue
			// which have not yet been sent, so we may send an err_chk
			// here.  But it does save us if we somehow forgot to ACKREQ
			// Note, if we are using Send DMA on the UD QP, the ERR_CHK
			// below will wait behind it, so we can't end up with a NAK
			// and retry before the 1st attempt has locally completed.
			done_local = 1;	/* Always done for non-DMA scb */
		}

		scb->ack_timeout =
		    min(scb->ack_timeout * proto->epinfo.ep_timeout_ack_factor,
			proto->epinfo.ep_timeout_ack_max);
		scb->abs_timeout = t_cyc_next + scb->ack_timeout;
		if (done_local) {
			_HFI_VDBG
			    ("sending err_chk flow=%p with first=%d, last=%d\n",
			     flow, scb->seq_num.psn_num,
			     STAILQ_LAST(&flow->scb_unacked, ips_scb,
					 nextq)->seq_num.psn_num);
#ifdef PSM_BYTE_FLOW_CREDITS
			_HFI_VDBG("credits %d %d\n", flow->credits, flow->credit_bytes);
#else
			_HFI_VDBG("credits %d\n", flow->credits);
#endif

			ctrlscb.scb_flags = 0;
			if (proto->flags & IPS_PROTO_FLAG_RCVTHREAD)
				ctrlscb.scb_flags |= IPS_SEND_FLAG_INTR;

			err_chk_seq = (SLIST_EMPTY(&flow->scb_pend)) ?
					flow->xmit_seq_num :
					SLIST_FIRST(&flow->scb_pend)->seq_num;

			PSM2_LOG_MSG("sending ERR_CHK message");
			message_type = OPCODE_ERR_CHK;
			err_chk_seq.psn_num = (err_chk_seq.psn_num - 1)
					& proto->psn_mask;
			ctrlscb.ips_lrh.bth[2] =
					__cpu_to_be32(err_chk_seq.psn_num);

			psm3_ips_proto_send_ctrl_message(flow, message_type,
					&flow->ipsaddr->ctrl_msg_queued,
					&ctrlscb, ctrlscb.cksum, 0);
			flow->credits_inc_psn = scb->seq_num.psn_num;
			// decrease flow credits
			if (flow->max_credits > proto->min_credits) {
				uint16_t actual_adjust = min(proto->credits_adjust + (flow->xmit_seq_num.psn_num & 0xF),
					flow->max_credits - proto->min_credits);
				flow->max_credits -= actual_adjust;
				if (flow->credits > actual_adjust) {
					flow->credits -= actual_adjust;
				} else {
					flow->credits = 0;
				}
				_HFI_VDBG("Decreased flow (%p) credits to %d\n", flow, flow->max_credits);
			}
		}

		t_cyc_next = get_cycles() + scb->ack_timeout;
	} else
		t_cyc_next += (scb->abs_timeout - current);

	psmi_timer_request(proto->timerq, current_timer, t_cyc_next);

	return PSM2_OK;
}

psm2_error_t
psm3_ips_proto_timer_send_callback(struct psmi_timer *current_timer,
			      uint64_t current)
{
	struct ips_flow *flow = current_timer->context;

	if (!SLIST_EMPTY(&flow->scb_pend))
		flow->flush(flow, NULL);

	return PSM2_OK;
}


#ifdef PSM_VERBS
static uint64_t verbs_ep_send_num_free(void *context)
{
	if (psmi_hal_get_hal_instance_index() != PSM_HAL_INDEX_VERBS) {
		return 0;
	} else {
		struct psm3_verbs_ep *vep = &((struct ips_proto *)context)->ep->verbs_ep;
		return vep->send_allocator.pool->send_num_free;
	}
}

static uint64_t verbs_ep_send_rdma_outstanding(void *context)
{
	if (psmi_hal_get_hal_instance_index() != PSM_HAL_INDEX_VERBS) {
		return 0;
	} else {
		struct psm3_verbs_ep *vep = &((struct ips_proto *)context)->ep->verbs_ep;
		return vep->send_rdma_outstanding;
	}
}

#ifdef PSM_RC_RECONNECT
static uint64_t verbs_ep_send_drain_outstanding(void *context)
{
	if (psmi_hal_get_hal_instance_index() != PSM_HAL_INDEX_VERBS) {
		return 0;
	} else {
		struct psm3_verbs_ep *vep = &((struct ips_proto *)context)->ep->verbs_ep;
		return vep->send_drain_outstanding;
	}
}
#endif
#endif

static uint64_t ep_reconnect_timeout(void *context)
{
	psm2_ep_t ep = ((struct ips_proto *)context)->ep;
	if (ep->allow_reconnect)
		return ep->reconnect_timeout;
	else
		return 0;
}

static uint64_t proto_num_connected_outgoing(void *context)
{
	struct ips_proto *proto = (struct ips_proto *)context;
	return proto->num_connected_outgoing;
}

static uint64_t proto_num_connected_incoming(void *context)
{
	struct ips_proto *proto = (struct ips_proto *)context;
	return proto->num_connected_incoming;
}

static psm2_error_t
ips_proto_register_stats(struct ips_proto *proto)
{
	/*
	 * We put a (*) in the output to denote stats that may cause a drop in
	 * performance.
	 *
	 * We put a (**) in the output of those stats that "should never happen"
	 */

	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL_HELP("Low-Level Message Send Mechanism Statisics:"),
		PSMI_STATS_DECLU64("pio_busy_count",
				   "Total times delayed send due to out of send resources",
				   &proto->stats.pio_busy_cnt),
		PSMI_STATS_DECLU64("pio_no_flow_credits",
				   "Total times delayed send due to no credits from remote process",
				   &proto->stats.pio_no_flow_credits),
#ifdef PSM_BYTE_FLOW_CREDITS
		PSMI_STATS_DECLU64("pio_no_flow_credit_bytes",
				   "Total bytes delayed send due to no credits from remote process",
				   &proto->stats.pio_no_flow_credit_bytes),
#endif
		PSMI_STATS_DECLU64("pio_cc_max",
				   "Max times delayed send during a congestion control for unreliable send",
				   &proto->stats.pio_cc_max_duration),
#ifdef USE_RC
		PSMI_STATS_DECLU64("pio_rc_cc_max",
				   "Max times delayed send during a congestion control for reliable send",
				   &proto->stats.pio_rc_cc_max_duration),
#endif
		PSMI_STATS_DECLU64("ctrl_msg_queue_overflow",
				   "Total times unable to queue a zero payload control message",
				   &proto->ctrl_msg_queue_overflow),
#ifdef PSM_VERBS
		PSMI_STATS_DECLU64("post_send_fail_(*)",
				   "Total failed ibv_post_send",
				   &proto->stats.post_send_fail),
		PSMI_STATS_DECL("ud_sbuf_free",
				"current number of free verbs UD send buffers",
				MPSPAWN_STATS_REDUCTION_ALL,
				   verbs_ep_send_num_free, NULL),
		PSMI_STATS_DECL_FUNC("send_rdma_outstanding",
				"current number of verbs outbound RDMA outstanding",
				   verbs_ep_send_rdma_outstanding),
#ifdef PSM_RC_RECONNECT
		PSMI_STATS_DECL_FUNC("send_drain_outstanding",
				"current number of verbs QPs waiting for send Q to drain",
				   verbs_ep_send_drain_outstanding),
#endif
#endif

#ifdef PSM_HAVE_SDMA
		/* SDMA statistics only applicable to HALs with send DMA */
		// When must wait for local SDMA completions.
		// wait for completion of SDMA as part of ACK processing.
		// got an ACK for original SDMA which we did not yet complete.
		// can imply late arrival of original at remote end after we
		// queued a retry or simply lazy reaping of original.
		PSMI_STATS_DECLU64("sdma_compl_wait_ack",
				   "Total times had to wait for sdma completion in order to process it's ack",
				   &proto->stats.sdma_compl_wait_ack),
		// wait for completion of SDMA before resending.
		// got a NAK for packets including an SDMA still in HW queue
		// Had to wait for original (now useless) SDMA to complete
		// before posting retry to HW queue. Can be lazy reaping.
		PSMI_STATS_DECLU64("sdma_compl_wait_resend",
				   "Total times had to wait for sdma completion before resending",
				   &proto->stats.sdma_compl_wait_resend),
		// ack_timeout fired and had to wait for local SDMA completions
		// due to slow HW queue. Can't be lazy reaping (reaped before)
		PSMI_STATS_DECLU64("sdma_compl_slow (*)",
				   "Total times ack timeout occurred before sdma completion",
				   &proto->stats.sdma_compl_slow),
		// Had to yield CPU to wait for local SDMA completion
		PSMI_STATS_DECLU64("sdma_compl_yield (*)",
				   "Total times yielded CPU while waiting for sdma completion",
				   &proto->stats.sdma_compl_yield),
#endif
#if defined(PSM_SOCKETS)
		PSMI_STATS_DECLU64("partial_data_write_cnt",
				   "Total times socket send Q only accepted part of a data message 'packet'",
				   &proto->stats.partial_data_write_cnt),
		PSMI_STATS_DECLU64("partial_ctr_write_cnt",
				   "Total times socket send Q only accepted part of a control message 'packet'",
				   &proto->stats.partial_ctr_write_cnt),
#endif /* PSM_SOCKETS */
		PSMI_STATS_DECLU64("scb_unavail_eager_count",
				   "Total times an eager send had to wait for a send descriptor to become available",
				   &proto->stats.scb_egr_unavail_cnt),
		// ----------------------------------------------------------
		PSMI_STATS_DECL_HELP("Low-Level Message Receive Mechanism Statisics:"),
		PSMI_STATS_DECLU64("unknown_packets_(**)",	/* shouldn't happen */
				   "Total times received packet with unknown PSM3 opcode",
				   &proto->stats.unknown_packets),
		PSMI_STATS_DECLU64("stray_packets_(*)",
				   "Total times received packet from unknown peer",
				   &proto->stats.stray_packets),
		PSMI_STATS_DECLU64("rcv_revisit",
				   "Total times delayed receive processing to see if application will post a receive",
				   &proto->stats.rcv_revisit),
#if defined(PSM_SOCKETS)
		PSMI_STATS_DECLU64("partial_read_cnt",
				   "Total times socket recv Q only returned part of a message 'packet'",
				   &proto->stats.partial_read_cnt),
		PSMI_STATS_DECLU64("rcv_hol_blocking",
				   "Total times socket recv processing blocked until complete receipt of another message 'packet'",
				   &proto->stats.rcv_hol_blocking),
#endif
		// -----------------------------------------------------------
		PSMI_STATS_DECL_HELP("PSM3 Reliabilty Protocol Statistics:"),
		PSMI_STATS_DECLU64("send_rexmit",
				   "Number of PSM3 packets re-transmitted",
				   &proto->epaddr_stats.send_rexmit),
#ifdef PSM_HAVE_RDMA_ERR_CHK
		PSMI_STATS_DECLU64("rdma_rexmit_(*)",
				   "Number of PSM3 RDMA re-transmitted",
				   &proto->epaddr_stats.rdma_rexmit),
#endif
		PSMI_STATS_DECLU64("err_chk_send",
				   "Total PSM3 err_chk packet sent indicating out of order or lost packet receipt",
				   &proto->epaddr_stats.err_chk_send),
		PSMI_STATS_DECLU64("err_chk_recv",
				   "Total PSM3 err_chk packet received indicating need to resend packets",
				   &proto->epaddr_stats.err_chk_recv),
#ifdef PSM_HAVE_RDMA_ERR_CHK
		PSMI_STATS_DECLU64("err_chk_rdma_send_(*)",
				   "Total PSM3 err_chk_rdma packet sent indicating out of order or lost RDMA receipt",
				   &proto->epaddr_stats.err_chk_rdma_send),
		PSMI_STATS_DECLU64("err_chk_rdma_recv_(*)",
				   "Total PSM3 err_chk_rdma packet received indicating need to resend RDMA",
				   &proto->epaddr_stats.err_chk_rdma_recv),
		PSMI_STATS_DECLU64("err_chk_rdma_resp_send_(*)",
				   "Total PSM3 err_chk_rdma response packet sent",
				   &proto->epaddr_stats.err_chk_rdma_resp_send),
		PSMI_STATS_DECLU64("err_chk_rdma_resp_recv_(*)",
				   "Total PSM3 err_chk_rdma response packet received",
				   &proto->epaddr_stats.err_chk_rdma_resp_recv),
#endif
		PSMI_STATS_DECLU64("nak_send",
				   "Total PSM3 NAK sent",
				   &proto->epaddr_stats.nak_send),
		PSMI_STATS_DECLU64("nak_recv",
				   "Total PSM3 NAK received",
				   &proto->epaddr_stats.nak_recv),
		PSMI_STATS_DECLU64("rep_proc_err",
				   "Total HAL discovered errors during connect REP processing",
				   &proto->epaddr_stats.rep_proc_err),
#ifdef PSM_RC_RECONNECT
		PSMI_STATS_DECLU64("recv_wc_error",
				   "Total RC QP Recv WCs with Non-Flush Error",
				   &proto->epaddr_stats.recv_wc_error),
		PSMI_STATS_DECLU64("send_wc_error",
				   "Total RC QP Send WCs with Non-Flush Error",
				   &proto->epaddr_stats.send_wc_error),
		PSMI_STATS_DECLU64("rdma_wc_error",
				   "Total RC QP RDMA Write WCs with Non-Flush Error",
				   &proto->epaddr_stats.rdma_wc_error),
		PSMI_STATS_DECLU64("drain_wc_error",
				   "Total RC QP Drain send WCs with Non-Flush Error",
				   &proto->epaddr_stats.drain_wc_error),
#endif
		// ---------------------------------------------------------
		PSMI_STATS_DECL_HELP("PSM3 Connection Establishment Protocol:\n"
			"PSM3 uses a peer-to-peer connection model, where both "
			"sides of a connection send a connection request "
			"and each side responds to the other with a "
			"connection reply.\n"
			"Retry counters are the subset of total request "
			"packets which were retries.\n"
			"During disconnect some dups, unknown and other "
			"oddities are typical.\n"
			"When reconnect_timeout is non-zero recovery "
			"of HAL specific connection resources is permitted, "
			"such as user space RC QP connections\n"),
		PSMI_STATS_DECL_FUNC("reconnect_timeout",
				   "reconnection timeout in seconds",
				   ep_reconnect_timeout),
		PSMI_STATS_DECL_FUNC("num_connected_outgoing",
				   "Current PSM3 established outgoing connections",
				   proto_num_connected_outgoing),
		PSMI_STATS_DECLU64("max_connected_outgoing",
				   "Maximum PSM3 established outgoing connections",
				   &proto->epaddr_stats.max_connected_outgoing),
		PSMI_STATS_DECL_FUNC("num_connected_incoming",
				   "Current PSM3 established incoming connections",
				   proto_num_connected_incoming),
		PSMI_STATS_DECLU64("max_connected_incoming",
				   "Maximum PSM3 established incoming connections",
				   &proto->epaddr_stats.max_connected_incoming),
		PSMI_STATS_DECLU64("connect_req_send",
				   "Total PSM3 (re)connection request sent",
				   &proto->epaddr_stats.connect_req_send),
		PSMI_STATS_DECLU64("connect_req_retry",
				   "Total PSM3 connection request retries",
				   &proto->epaddr_stats.connect_req_retry),
		PSMI_STATS_DECLU64("reconnect_req_send",
				   "Total PSM3 reconnection request send",
				   &proto->epaddr_stats.reconnect_req_send),
		PSMI_STATS_DECLU64("reconnect_req_retry",
				   "Total PSM3 reconnection request retries",
				   &proto->epaddr_stats.reconnect_req_retry),
		PSMI_STATS_DECLU64("connect_req_recv",
				   "Total PSM3 (re)connection request received",
				   &proto->epaddr_stats.connect_req_recv),
		PSMI_STATS_DECLU64("connect_req_rej",
				   "Total rejected PSM3 (re)connection request",
				   &proto->epaddr_stats.connect_req_rej),
		PSMI_STATS_DECLU64("connect_req_inconsistent",
				   "Total inconsistent PSM3 duplicate connection request received (ignored)",
				   &proto->epaddr_stats.connect_req_inconsistent),
		PSMI_STATS_DECLU64("connect_req_dup",
				   "Total duplicate PSM3 connection request received",
				   &proto->epaddr_stats.connect_req_dup),
		PSMI_STATS_DECLU64("connect_req_stale",
				   "Total stale PSM3 (re)connection request (ignored)",
				   &proto->epaddr_stats.connect_req_stale),
		PSMI_STATS_DECLU64("connect_req_disc",
				   "Total PSM3 (re)connection request while disconnecting (ignored)",
				   &proto->epaddr_stats.connect_req_disc),
		PSMI_STATS_DECLU64("reconnect_req_recv",
				   "Total PSM3 reconnection request received",
				   &proto->epaddr_stats.reconnect_req_recv),
		PSMI_STATS_DECLU64("reconnect_req_inconsistent",
				   "Total inconsistent PSM3 reconnection request received (ignored)",
				   &proto->epaddr_stats.reconnect_req_inconsistent),
		PSMI_STATS_DECLU64("reconnect_req_dup",
				   "Total duplicate PSM3 reconnection request received",
				   &proto->epaddr_stats.reconnect_req_dup),
		PSMI_STATS_DECLU64("connect_rep_send",
				   "Total PSM3 (re)connection reply sent",
				   &proto->epaddr_stats.connect_rep_send),
		PSMI_STATS_DECLU64("connect_rep_recv",
				   "Total PSM3 (re)connection reply received",
				   &proto->epaddr_stats.connect_rep_recv),
		PSMI_STATS_DECLU64("connect_rep_unknown",
				   "Total PSM3 (re)connection reply receieved from unknown epid (ignored)",
				   &proto->epaddr_stats.connect_rep_unknown),
		PSMI_STATS_DECLU64("connect_rep_inconsistent",
				   "Total inconsistent PSM3 (re)connection reply received (ignored)",
				   &proto->epaddr_stats.connect_rep_inconsistent),
		PSMI_STATS_DECLU64("connect_rep_dup",
				   "Total duplicate or late PSM3 connection reply received (ignored)",
				   &proto->epaddr_stats.connect_rep_dup),
		PSMI_STATS_DECLU64("connect_rep_stale",
				   "Total stale PSM3 connection reply (ignored)",
				   &proto->epaddr_stats.connect_rep_stale),
		PSMI_STATS_DECLU64("connect_rep_disc",
				   "Total PSM3 connection reply while disconnecting (ignored)",
				   &proto->epaddr_stats.connect_rep_disc),
		PSMI_STATS_DECLU64("reconnect_rep_dup",
				   "Total duplicate or late PSM3 reconnection reply received (ignored)",
				   &proto->epaddr_stats.reconnect_rep_dup),
		PSMI_STATS_DECLU64("reconnect_rep_stale",
				   "Total stale PSM3 reconnection reply (ignored)",
				   &proto->epaddr_stats.reconnect_rep_stale),
		PSMI_STATS_DECLU64("reconnect_rep_disc",
				   "Total PSM3 reconnection reply while disconnecting (ignored)",
				   &proto->epaddr_stats.reconnect_rep_disc),
		PSMI_STATS_DECLU64("disconnect_req_send",
				   "Total PSM3 disconnect request sent",
				   &proto->epaddr_stats.disconnect_req_send),
		PSMI_STATS_DECLU64("disconnect_req_retry",
				   "Total PSM3 disconnect request retries",
				   &proto->epaddr_stats.disconnect_req_retry),
		PSMI_STATS_DECLU64("disconnect_req_recv",
				   "Total PSM3 disconnect request received",
				   &proto->epaddr_stats.disconnect_req_recv),
		PSMI_STATS_DECLU64("disconnect_req_dup",
				   "Total duplicate PSM3 disconnection request received",
				   &proto->epaddr_stats.disconnect_req_dup),
		PSMI_STATS_DECLU64("disconnect_rep_send",
				   "Total PSM3 disconnect reply sent",
				   &proto->epaddr_stats.disconnect_rep_send),
		PSMI_STATS_DECLU64("disconnect_rep_recv",
				   "Total PSM3 disconnect reply received",
				   &proto->epaddr_stats.disconnect_rep_recv),
		PSMI_STATS_DECLU64("disconnect_rep_unknown",
				   "Total PSM3 disconnection reply received from unknown epid (ignored)",
				   &proto->epaddr_stats.disconnect_rep_unknown),
		PSMI_STATS_DECLU64("disconnect_rep_dup",
				   "Total duplicate or late PSM3 disconnection reply received (ignored)",
				   &proto->epaddr_stats.disconnect_rep_dup),
		PSMI_STATS_DECLU64("connections_lost",
				   "Total PSM3 HAL connections lost",
				   &proto->epaddr_stats.connections_lost),
		PSMI_STATS_DECLU64("reconnect_started",
				   "Total PSM3 HAL reconnection started",
				   &proto->epaddr_stats.reconnect_started),
		PSMI_STATS_DECLU64("max_reconnect_count",
				   "Largest HAL connection reconnect_count (capped at 255)",
				   &proto->epaddr_stats.max_reconnect_count),
		PSMI_STATS_DECLU64("max_reconnect_usec",
				   "Largest HAL reconnection time in microseconds",
				   &proto->epaddr_stats.max_reconnect_usec),
		// -----------------------------------------------------------
		PSMI_STATS_DECL_HELP("PSM3 Rendezvous Protocol Statistics:\n"
			"The rendezvous protocol is used for large messages "
			"and for some synchronous application message sends. "
			"The protoccl starts with a Request To Send (RTS) "
			"from the sender.  In response, when the receiver is "
			"ready to process the message ,typically after "
			"the application has posted a matching receive, "
			"the receiver will respond with a Clear To Send "
			"(CTS). The CTS may select an RDMA or 'Long Data' "
			"protocol for use by the sender."),
		PSMI_STATS_DECLU64("rts_send",
				   "Total PSM3 rendezvous request to send (RTS) sent",
				   &proto->epaddr_stats.rts_send),
		PSMI_STATS_DECLU64("rts_recv",
				   "Total PSM3 rendezvous request to send (RTS) received",
				   &proto->epaddr_stats.rts_recv),
		PSMI_STATS_DECLU64("cts_long_data_send",
				   "Total PSM3 rendezvous clear to send (CTS) Long Data sent",
				   &proto->epaddr_stats.cts_long_data_send),
		PSMI_STATS_DECLU64("cts_long_data_recv",
				   "Total PSM3 rendezvous clear to send (CTS) Long Data received",
				   &proto->epaddr_stats.cts_long_data_recv),
		PSMI_STATS_DECLU64("cts_rdma_send",
				   "Total PSM3 rendezvous clear to send (CTS) RDMA sent",
				   &proto->epaddr_stats.cts_rdma_send),
		PSMI_STATS_DECLU64("cts_rdma_recv",
				   "Total PSM3 rendezvous clear to send (CTS) RDMA received",
				   &proto->epaddr_stats.cts_rdma_recv),
		// ----------------------------------------------------------
		PSMI_STATS_DECL_HELP("PSM3 Message Strategy Statistics:\n"
			"PSM3 uses a variety of message transmission "
			"strategies based on message size, async or sync as "
			"requested by the application (isend or send), etc. "
			"Fundamental strategies include:\n"
			"  - Tiny - <= 8 bytes\n"
			"  - Short - up to 1 'packet'\n"
			"  - Eager - medium multi-packet messages\n"
			"  - Rendezvous - large multi-packet messages\n"
			"Tiny, Short and Eager are variations of Eager where the\n"
			"sender does not wait for receiver tag matching.\n"
			"Rendezvous messages may use RDMA or 'Long Data'. "
			"'Long Data' is similar to Eager.\n"
			"Short, Eager and 'Long Data' messages may use "
			"send bounce buffers and the 'pio' mechanisms or "
			"they may use send Send Dma (dma).\n"
			"The RTS at the start of a rendezvous message may "
			"also carry all or a portion of the message payload.\n"
			"Large Rendezvous messages may be broken into multiple "
			"window size chunks each with a separate CTS.\n"
#ifdef PSM_HAVE_GPU
			"When sending from a GPU application buffer the "
			"mechanisms include:\n"
			"  - gdrcopy - Direct GPU copy via mmaping GPU memory\n"
			"  - cuCopy - GPU API calls to copy from GPU memory\n"
			"  - gdr - Direct GPU send DMA from GPU memory\n"
			"  - rdma_gdr - Direct GPU send RDMA from GPU memory\n"
			"  - rdma_hbuf - send RDMA via pipelined copies from GPU to host buffers\n"
#endif
			"When receiving messages, if the application has not "
			"yet posted a matching receive, a bounce buffer "
			"(sysbuf) is used and data is later copied to the "
			"application buffer when it posts the receive. "
			"With the exception of RDMA, all receive mechanisms "
			"involve some form of copy.\n"
#ifdef PSM_HAVE_GPU
			"When receiving into a GPU application buffer the "
			"mechanisms include:\n"
			"  - gdrcopy - Direct GPU copy via mmaping GPU memory\n"
			"  - cuCopy - GPU API calls to copy to GPU memory\n"
			"  - rdma_gdr - Direct GPU recv RDMA into GPU memory\n"
			"  - rdma_hbuf - recv RDMA via pipelined copies into GPU from host buffers\n"
#endif
			"The statistics below reflect how many times each "
			"strategy/mechanism was used and the total bytes "
			"transfered via each."),
		PSMI_STATS_DECLU64("tiny_cpu_isend",
				   "Tiny messages sent async from a CPU buffer",
				   &proto->strat_stats.tiny_cpu_isend),
		PSMI_STATS_DECLU64("tiny_cpu_isend_bytes",
				   "Tiny message bytes sent async from a CPU buffer",
				   &proto->strat_stats.tiny_cpu_isend_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("tiny_gdrcopy_isend",
				   "Tiny messages sent async from a GPU buffer via GDR copy",
				   &proto->strat_stats.tiny_gdrcopy_isend),
		PSMI_STATS_DECLU64("tiny_gdrcopy_isend_bytes",
				   "Tiny message bytes sent async from a GPU buffer via GDR copy",
				   &proto->strat_stats.tiny_gdrcopy_isend_bytes),
		PSMI_STATS_DECLU64("tiny_cuCopy_isend",
				   "Tiny messages sent async from a GPU buffer via GPU copy",
				   &proto->strat_stats.tiny_cuCopy_isend),
		PSMI_STATS_DECLU64("tiny_cuCopy_isend_bytes",
				   "Tiny message bytes sent async from a GPU buffer via GPU copy",
				   &proto->strat_stats.tiny_cuCopy_isend_bytes),
#endif
		PSMI_STATS_DECLU64("tiny_cpu_send",
				   "Tiny messages sent sync from a CPU buffer",
				   &proto->strat_stats.tiny_cpu_send),
		PSMI_STATS_DECLU64("tiny_cpu_send_bytes",
				   "Tiny message bytes sent sync from a CPU buffer",
				   &proto->strat_stats.tiny_cpu_send_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("tiny_gdrcopy_send",
				   "Tiny messages sent sync from a GPU buffer via GDR copy",
				   &proto->strat_stats.tiny_gdrcopy_send),
		PSMI_STATS_DECLU64("tiny_gdrcopy_send_bytes",
				   "Tiny message bytes sent sync from a GPU buffer via GDR copy",
				   &proto->strat_stats.tiny_gdrcopy_send_bytes),
		PSMI_STATS_DECLU64("tiny_cuCopy_send",
				   "Tiny messages sent sync from a GPU buffer via GPU copy",
				   &proto->strat_stats.tiny_cuCopy_send),
		PSMI_STATS_DECLU64("tiny_cuCopy_send_bytes",
				   "Tiny message bytes sent sync from a GPU buffer via GPU copy",
				   &proto->strat_stats.tiny_cuCopy_send_bytes),
#endif
		PSMI_STATS_DECLU64("tiny_cpu_recv",
				   "Tiny messages received into an application CPU buffer",
				   &proto->strat_stats.tiny_cpu_recv),
		PSMI_STATS_DECLU64("tiny_cpu_recv_bytes",
				   "Tiny message bytes received into an application CPU buffer",
				   &proto->strat_stats.tiny_cpu_recv_bytes),
		PSMI_STATS_DECLU64("tiny_sysbuf_recv",
				   "Tiny messages received into a bounce buffer",
				   &proto->strat_stats.tiny_sysbuf_recv),
		PSMI_STATS_DECLU64("tiny_sysbuf_recv_bytes",
				   "Tiny message bytes received into a bounce buffer",
				   &proto->strat_stats.tiny_sysbuf_recv_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("tiny_gdrcopy_recv",
				   "Tiny messages received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.tiny_gdrcopy_recv),
		PSMI_STATS_DECLU64("tiny_gdrcopy_recv_bytes",
				   "Tiny message bytes received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.tiny_gdrcopy_recv_bytes),
		PSMI_STATS_DECLU64("tiny_cuCopy_recv",
				   "Tiny messages received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.tiny_cuCopy_recv),
		PSMI_STATS_DECLU64("tiny_cuCopy_recv_bytes",
				   "Tiny message bytes received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.tiny_cuCopy_recv_bytes),
#endif

		PSMI_STATS_DECLU64("short_copy_cpu_isend",
				   "Short messages sent async from a CPU buffer via send buffer",
				   &proto->strat_stats.short_copy_cpu_isend),
		PSMI_STATS_DECLU64("short_copy_cpu_isend_bytes",
				   "Short message bytes sent async from a CPU buffer via send buffer",
				   &proto->strat_stats.short_copy_cpu_isend_bytes),
		PSMI_STATS_DECLU64("short_dma_cpu_isend",
				   "Short messages sent async from a CPU buffer via send DMA",
				   &proto->strat_stats.short_dma_cpu_isend),
		PSMI_STATS_DECLU64("short_dma_cpu_isend_bytes",
				   "Short message bytes sent async from a CPU buffer via send DMA",
				   &proto->strat_stats.short_dma_cpu_isend_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("short_gdrcopy_isend",
				   "Short messages sent async from a GPU buffer via GDR copy",
				   &proto->strat_stats.short_gdrcopy_isend),
		PSMI_STATS_DECLU64("short_gdrcopy_isend_bytes",
				   "Short message bytes sent async from a GPU buffer via GDR copy",
				   &proto->strat_stats.short_gdrcopy_isend_bytes),
		PSMI_STATS_DECLU64("short_cuCopy_isend",
				   "Short messages sent async from a GPU buffer via GPU copy",
				   &proto->strat_stats.short_cuCopy_isend),
		PSMI_STATS_DECLU64("short_cuCopy_isend_bytes",
				   "Short message bytes sent async from a GPU buffer via GPU copy",
				   &proto->strat_stats.short_cuCopy_isend_bytes),
		PSMI_STATS_DECLU64("short_gdr_isend",
				   "Short messages sent async from a GPU buffer via GPU Send DMA",
				   &proto->strat_stats.short_gdr_isend),
		PSMI_STATS_DECLU64("short_gdr_isend_bytes",
				   "Short message bytes sent async from a GPU buffer via GPU Send DMA",
				   &proto->strat_stats.short_gdr_isend_bytes),
#endif
		PSMI_STATS_DECLU64("short_copy_cpu_send",
				   "Short messages sent sync from a CPU buffer via send buffer",
				   &proto->strat_stats.short_copy_cpu_send),
		PSMI_STATS_DECLU64("short_copy_cpu_send_bytes",
				   "Short message bytes sent sync from a CPU buffer via send buffer",
				   &proto->strat_stats.short_copy_cpu_send_bytes),
		PSMI_STATS_DECLU64("short_dma_cpu_send",
				   "Short messages sent sync from a CPU buffer via send DMA",
				   &proto->strat_stats.short_dma_cpu_send),
		PSMI_STATS_DECLU64("short_dma_cpu_send_bytes",
				   "Short message bytes sent sync from a CPU buffer via send DMA",
				   &proto->strat_stats.short_dma_cpu_send_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("short_gdrcopy_send",
				   "Short messages sent sync from a GPU buffer via GDR copy",
				   &proto->strat_stats.short_gdrcopy_send),
		PSMI_STATS_DECLU64("short_gdrcopy_send_bytes",
				   "Short message bytes sent sync from a GPU buffer via GDR copy",
				   &proto->strat_stats.short_gdrcopy_send_bytes),
		PSMI_STATS_DECLU64("short_cuCopy_send",
				   "Short messages sent sync from a GPU buffer via GPU copy",
				   &proto->strat_stats.short_cuCopy_send),
		PSMI_STATS_DECLU64("short_cuCopy_send_bytes",
				   "Short message bytes sent sync from a GPU buffer via GPU copy",
				   &proto->strat_stats.short_cuCopy_send_bytes),
		PSMI_STATS_DECLU64("short_gdr_send",
				   "Short messages sent sync from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.short_gdr_send),
		PSMI_STATS_DECLU64("short_gdr_send_bytes",
				   "Short message bytes sent sync from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.short_gdr_send_bytes),
#endif

		PSMI_STATS_DECLU64("short_cpu_recv",
				   "Short messages received into an application CPU buffer",
				   &proto->strat_stats.short_cpu_recv),
		PSMI_STATS_DECLU64("short_cpu_recv_bytes",
				   "Short message bytes received into an application CPU buffer",
				   &proto->strat_stats.short_cpu_recv_bytes),
		PSMI_STATS_DECLU64("short_sysbuf_recv",
				   "Short messages received into a bounce buffer",
				   &proto->strat_stats.short_sysbuf_recv),
		PSMI_STATS_DECLU64("short_sysbuf_recv_bytes",
				   "Short message bytes received into a bounce buffer",
				   &proto->strat_stats.short_sysbuf_recv_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("short_gdrcopy_recv",
				   "Short messages received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.short_gdrcopy_recv),
		PSMI_STATS_DECLU64("short_gdrcopy_recv_bytes",
				   "Short message bytes received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.short_gdrcopy_recv_bytes),
		PSMI_STATS_DECLU64("short_cuCopy_recv",
				   "Short messages received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.short_cuCopy_recv),
		PSMI_STATS_DECLU64("short_cuCopy_recv_bytes",
				   "Short message bytes received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.short_cuCopy_recv_bytes),
#endif

		PSMI_STATS_DECLU64("eager_copy_cpu_isend",
				   "Eager messages sent async from a CPU buffer via send buffer",
				   &proto->strat_stats.eager_copy_cpu_isend),
		PSMI_STATS_DECLU64("eager_copy_cpu_isend_bytes",
				   "Eager message bytes sent async from a CPU buffer via send buffer",
				   &proto->strat_stats.eager_copy_cpu_isend_bytes),
		PSMI_STATS_DECLU64("eager_dma_cpu_isend",
				   "Eager messages sent async from a CPU buffer via send DMA",
				   &proto->strat_stats.eager_dma_cpu_isend),
		PSMI_STATS_DECLU64("eager_dma_cpu_isend_bytes",
				   "Eager message bytes sent async from a CPU buffer via send DMA",
				   &proto->strat_stats.eager_dma_cpu_isend_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("eager_cuCopy_isend",
				   "Eager messages sent async from a GPU buffer via GPU copy",
				   &proto->strat_stats.eager_cuCopy_isend),
		PSMI_STATS_DECLU64("eager_cuCopy_isend_bytes",
				   "Eager message bytes sent async from a GPU buffer via GPU copy",
				   &proto->strat_stats.eager_cuCopy_isend_bytes),
		PSMI_STATS_DECLU64("eager_gdr_isend",
				   "Eager messages sent async from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.eager_gdr_isend),
		PSMI_STATS_DECLU64("eager_gdr_isend_bytes",
				   "Eager message bytes sent async from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.eager_gdr_isend_bytes),
#endif
		PSMI_STATS_DECLU64("eager_copy_cpu_send",
				   "Eager messages sent sync from a CPU buffer via send buffer",
				   &proto->strat_stats.eager_copy_cpu_send),
		PSMI_STATS_DECLU64("eager_copy_cpu_send_bytes",
				   "Eager message bytes sent sync from a CPU buffer via send buffer",
				   &proto->strat_stats.eager_copy_cpu_send_bytes),
		PSMI_STATS_DECLU64("eager_dma_cpu_send",
				   "Eager messages sent sync from a CPU buffer via send DMA",
				   &proto->strat_stats.eager_dma_cpu_send),
		PSMI_STATS_DECLU64("eager_dma_cpu_send_bytes",
				   "Eager message bytes sent sync from a CPU buffer via send DMA",
				   &proto->strat_stats.eager_dma_cpu_send_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("eager_cuCopy_send",
				   "Eager messages sent sync from a GPU buffer via GPU copy",
				   &proto->strat_stats.eager_cuCopy_send),
		PSMI_STATS_DECLU64("eager_cuCopy_send_bytes",
				   "Eager message bytes sent sync from a GPU buffer via GPU copy",
				   &proto->strat_stats.eager_cuCopy_send_bytes),
		PSMI_STATS_DECLU64("eager_gdr_send",
				   "Eager messages sent sync from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.eager_gdr_send),
		PSMI_STATS_DECLU64("eager_gdr_send_bytes",
				   "Eager message bytes sent sync from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.eager_gdr_send_bytes),
#endif

		PSMI_STATS_DECLU64("eager_cpu_recv",
				   "Eager messages received into an application CPU buffer",
				   &proto->strat_stats.eager_cpu_recv),
		PSMI_STATS_DECLU64("eager_cpu_recv_bytes",
				   "Eager message bytes received into an application CPU buffer",
				   &proto->strat_stats.eager_cpu_recv_bytes),
		PSMI_STATS_DECLU64("eager_sysbuf_recv",
				   "Eager messages received into a bounce buffer",
				   &proto->strat_stats.eager_sysbuf_recv),
		PSMI_STATS_DECLU64("eager_sysbuf_recv_bytes",
				   "Eager message bytes received into a bounce buffer",
				   &proto->strat_stats.eager_sysbuf_recv_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("eager_gdrcopy_recv",
				   "Eager messages received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.eager_gdrcopy_recv),
		PSMI_STATS_DECLU64("eager_gdrcopy_recv_bytes",
				   "Eager message bytes received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.eager_gdrcopy_recv_bytes),
		PSMI_STATS_DECLU64("eager_cuCopy_recv",
				   "Eager messages received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.eager_cuCopy_recv),
		PSMI_STATS_DECLU64("eager_cuCopy_recv_bytes",
				   "Eager message bytes received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.eager_cuCopy_recv_bytes),
#endif

		PSMI_STATS_DECLU64("rndv_cpu_isend",
				   "Rendezvous messages sent async from a CPU buffer",
				   &proto->strat_stats.rndv_cpu_isend),
		PSMI_STATS_DECLU64("rndv_cpu_isend_bytes",
				   "Rendezvous message bytes sent async from a CPU buffer",
				   &proto->strat_stats.rndv_cpu_isend_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_gpu_isend",
				   "Rendezvous messages sent async from a GPU buffer",
				   &proto->strat_stats.rndv_gpu_isend),
		PSMI_STATS_DECLU64("rndv_gpu_isend_bytes",
				   "Rendezvous message bytes sent async from a GPU buffer",
				   &proto->strat_stats.rndv_gpu_isend_bytes),
#endif
		PSMI_STATS_DECLU64("rndv_cpu_send",
				   "Rendezvous messages sent sync from a CPU buffer",
				   &proto->strat_stats.rndv_cpu_send),
		PSMI_STATS_DECLU64("rndv_cpu_send_bytes",
				   "Rendezvous message bytes sent sync from a CPU buffer",
				   &proto->strat_stats.rndv_cpu_send_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_gpu_send",
				   "Rendezvous messages sent sync from a GPU buffer",
				   &proto->strat_stats.rndv_gpu_send),
		PSMI_STATS_DECLU64("rndv_gpu_send_bytes",
				   "Rendezvous message bytes sent sync from a GPU buffer",
				   &proto->strat_stats.rndv_gpu_send_bytes),
#endif

		PSMI_STATS_DECLU64("rndv_rts_cpu_recv",
				   "RTS packet messages received into an application CPU buffer",
				   &proto->strat_stats.rndv_rts_cpu_recv),
		PSMI_STATS_DECLU64("rndv_rts_cpu_recv_bytes",
				   "RTS packet message bytes received into an application CPU buffer",
				   &proto->strat_stats.rndv_rts_cpu_recv_bytes),
		PSMI_STATS_DECLU64("rndv_rts_sysbuf_recv",
				   "RTS packet messages received into an bounce buffer",
				   &proto->strat_stats.rndv_rts_sysbuf_recv),
		PSMI_STATS_DECLU64("rndv_rts_sysbuf_recv_bytes",
				   "RTS packet message bytes received into an bounce buffer",
				   &proto->strat_stats.rndv_rts_sysbuf_recv_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_rts_cuCopy_recv",
				   "RTS packet messages received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.rndv_rts_cuCopy_recv),
		PSMI_STATS_DECLU64("rndv_rts_cuCopy_recv_bytes",
				   "RTS packet message bytes received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.rndv_rts_cuCopy_recv_bytes),
#endif
		PSMI_STATS_DECLU64("rndv_rts_copy_cpu_send",
				   "Rendezvous messages sent in RTS sync from a CPU buffer via send buffer",
				   &proto->strat_stats.rndv_rts_copy_cpu_send),
		PSMI_STATS_DECLU64("rndv_rts_copy_cpu_send_bytes",
				   "Rendezvous message bytes sent in RTS sync from a CPU buffer via send buffer",
				   &proto->strat_stats.rndv_rts_copy_cpu_send_bytes),

		PSMI_STATS_DECLU64("rndv_long_cpu_recv",
				   "Long Data rendezvous messages received into an application CPU buffer",
				   &proto->strat_stats.rndv_long_cpu_recv),
		PSMI_STATS_DECLU64("rndv_long_cpu_recv_bytes",
				   "Long Data rendezvous message bytes received into an application CPU buffer",
				   &proto->strat_stats.rndv_long_cpu_recv_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_long_cuCopy_recv",
				   "Long Data rendezvous messages received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.rndv_long_cuCopy_recv),
		PSMI_STATS_DECLU64("rndv_long_cuCopy_recv_bytes",
				   "Long Data rendezvous message bytes received into an application GPU buffer via GPU copy",
				   &proto->strat_stats.rndv_long_cuCopy_recv_bytes),
		PSMI_STATS_DECLU64("rndv_long_gdrcopy_recv",
				   "Long Data rendezvous messages received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.rndv_long_gdrcopy_recv),
		PSMI_STATS_DECLU64("rndv_long_gdrcopy_recv_bytes",
				   "Long Data rendezvous message bytes received into an application GPU buffer via GDR copy",
				   &proto->strat_stats.rndv_long_gdrcopy_recv_bytes),
#endif

		PSMI_STATS_DECLU64("rndv_long_copy_cpu_send",
				   "Long Data rendezvous messages sent from a CPU buffer via send buffer",
				   &proto->strat_stats.rndv_long_copy_cpu_send),
		PSMI_STATS_DECLU64("rndv_long_copy_cpu_send_bytes",
				   "Long Data rendezvous message bytes sent from a CPU buffer via send buffer",
				   &proto->strat_stats.rndv_long_copy_cpu_send_bytes),
		PSMI_STATS_DECLU64("rndv_long_dma_cpu_send",
				   "Long Data rendezvous messages sent from a CPU buffer via send DMA",
				   &proto->strat_stats.rndv_long_dma_cpu_send),
		PSMI_STATS_DECLU64("rndv_long_dma_cpu_send_bytes",
				   "Long Data rendezvous message bytes sent from a CPU buffer via send DMA",
				   &proto->strat_stats.rndv_long_dma_cpu_send_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_long_cuCopy_send",
				   "Long Data rendezvous messages sent from a GPU buffer via GPU copy",
				   &proto->strat_stats.rndv_long_cuCopy_send),
		PSMI_STATS_DECLU64("rndv_long_cuCopy_send_bytes",
				   "Long Data rendezvous message bytes sent from a GPU buffer via GPU copy",
				   &proto->strat_stats.rndv_long_cuCopy_send_bytes),
		PSMI_STATS_DECLU64("rndv_long_gdrcopy_send",
				   "Long Data rendezvous messages sent from a GPU buffer via GDR copy",
				   &proto->strat_stats.rndv_long_gdrcopy_send),
		PSMI_STATS_DECLU64("rndv_long_gdrcopy_send_bytes",
				   "Long Data rendezvous message bytes sent from a GPU buffer via GDR copy",
				   &proto->strat_stats.rndv_long_gdrcopy_send_bytes),
		PSMI_STATS_DECLU64("rndv_long_gdr_send",
				   "Long Data rendezvous messages sent from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.rndv_long_gdr_send),
		PSMI_STATS_DECLU64("rndv_long_gdr_send_bytes",
				   "Long Data rendezvous message bytes sent from a GPU buffer via GPU send DMA",
				   &proto->strat_stats.rndv_long_gdr_send_bytes),
#endif

		PSMI_STATS_DECLU64("rndv_rdma_cpu_recv",
				   "RDMA rendezvous messages received direct into a CPU buffer",
				   &proto->strat_stats.rndv_rdma_cpu_recv),
		PSMI_STATS_DECLU64("rndv_rdma_cpu_recv_bytes",
				   "RDMA rendezvous message bytes received direct into a CPU buffer",
				   &proto->strat_stats.rndv_rdma_cpu_recv_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_rdma_gdr_recv",
				   "RDMA rendezvous messages received direct into a GPU buffer",
				   &proto->strat_stats.rndv_rdma_gdr_recv),
		PSMI_STATS_DECLU64("rndv_rdma_gdr_recv_bytes",
				   "RDMA rendezvous message bytes received direct into a GPU buffer",
				   &proto->strat_stats.rndv_rdma_gdr_recv_bytes),
		PSMI_STATS_DECLU64("rndv_rdma_hbuf_recv",
				   "RDMA rendezvous messages received into a GPU buffer via pipelined GPU copy",
				   &proto->strat_stats.rndv_rdma_hbuf_recv),
		PSMI_STATS_DECLU64("rndv_rdma_hbuf_recv_bytes",
				   "RDMA rendezvous message bytes received into a GPU buffer via pipelined GPU copy",
				   &proto->strat_stats.rndv_rdma_hbuf_recv_bytes),
#endif
		PSMI_STATS_DECLU64("rndv_rdma_cpu_send",
				   "RDMA rendezvous messages sent from a CPU buffer via send RDMA",
				   &proto->strat_stats.rndv_rdma_cpu_send),
		PSMI_STATS_DECLU64("rndv_rdma_cpu_send_bytes",
				   "RDMA rendezvous message bytes sent from a CPU buffer via send RDMA",
				   &proto->strat_stats.rndv_rdma_cpu_send_bytes),
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("rndv_rdma_gdr_send",
				   "RDMA rendezvous messages sent from a GPU buffer via send RDMA",
				   &proto->strat_stats.rndv_rdma_gdr_send),
		PSMI_STATS_DECLU64("rndv_rdma_gdr_send_bytes",
				   "RDMA rendezvous message bytes sent from a GPU buffer via send RDMA",
				   &proto->strat_stats.rndv_rdma_gdr_send_bytes),
		PSMI_STATS_DECLU64("rndv_rdma_hbuf_send",
				   "RDMA rendezvous messages sent from a GPU buffer via pipelined GPU copy",
				   &proto->strat_stats.rndv_rdma_hbuf_send),
		PSMI_STATS_DECLU64("rndv_rdma_hbuf_send_bytes",
				   "RDMA rendezvous message bytes sent from a GPU buffer via pipelined GPU copy",
				   &proto->strat_stats.rndv_rdma_hbuf_send_bytes),
#endif
		PSMI_STATS_DECL_HELP("Internal IPS SCB Pool Statisics:"),
		PSMI_STATS_DECLU64("scb_alloc",
				   "Number of SCB allocation events",
				   &proto->scb_stats.alloc),
		PSMI_STATS_DECLU64("scb_free",
				   "Number of SCB deallocation events",
				   &proto->scb_stats.free),
		PSMI_STATS_DECLU64("scb_max_inuse",
				   "Maximum number of outstanding SCBs",
				   &proto->scb_stats.max_inuse),
		PSMI_STATS_DECLU64("scb_dyn_alloc",
				   "Number of dynamic SCB allocation events",
				   &proto->scb_stats.dyn_alloc),
		PSMI_STATS_DECLU64("scb_dyn_free",
				   "Number of dynamic SCB deallocation events",
				   &proto->scb_stats.dyn_free),
		PSMI_STATS_DECLU64("scb_max_dyn_inuse",
				   "Maximum number of outstanding dynamic SCBs",
				   &proto->scb_stats.max_dyn_inuse),

	};

	return psm3_stats_register_type("PSM_low-level_protocol_stats",
		"PSM3 nic protocol statistics for an endpoint in the process.\n"
		"The PSM3 nic protocol uses the NIC hardware to send messages "
		"to other NICs in the job. To optimize performance a variety "
		"of transmission protocols and strategies are used based on "
		"message size, sync/async as requested by the application, "
		"etc.i\n"
		"The term 'pio' refers to a 'packet' sent via the simplest and most "
		"expedient mechanism for the NIC HW.  Typically via use of "
		"pre-established send bounce buffers.\n"
		"For the Sockets HAL use of TCP, the term 'packet' refers to a single "
		"portion of a message intended to be sent via a single TCP send() "
		"call.\n",
			PSMI_STATSTYPE_IPSPROTO, entries, PSMI_HOWMANY(entries),
			psm3_epid_fmt_internal(proto->ep->epid, 0), proto,
			proto->ep->dev_name);
}

static void sreq_inflight_callback(void *obj, void *context)
{
	psm2_mq_req_t sreq = (psm2_mq_req_t)obj;

	_HFI_VDBG("sreq %p state %u tidsendc %p tag %08x.%08x.%08x tagsel %08x.%08x.%08x buf %p off %u len %u\n",
		sreq, sreq->state, sreq->ptl_req_ptr,
		sreq->req_data.tag.tag0, sreq->req_data.tag.tag1,
			sreq->req_data.tag.tag2,
		sreq->req_data.tagsel.tag0, sreq->req_data.tagsel.tag1,
			sreq->req_data.tagsel.tag2,
		sreq->req_data.buf, sreq->send_msgoff, sreq->req_data.send_msglen);
		// peer could be PSM2_MQ_ANY_ADDR or 
		// could output sreq->req_data.peer->epid
}

static void rreq_inflight_callback(void *obj, void *context)
{
	psm2_mq_req_t rreq = (psm2_mq_req_t)obj;

	_HFI_VDBG("rreq %p state %u tag %08x.%08x.%08x tagsel %08x.%08x.%08x buf %p off %u len %u"
#ifdef PSM_VERBS
		" mr %p rkey 0x%x"
#endif
		"\n",
		rreq, rreq->state,
		rreq->req_data.tag.tag0, rreq->req_data.tag.tag1,
			rreq->req_data.tag.tag2,
		rreq->req_data.tagsel.tag0, rreq->req_data.tagsel.tag1,
			rreq->req_data.tagsel.tag2,
		rreq->req_data.buf, rreq->recv_msgoff, rreq->req_data.recv_msglen
#ifdef PSM_VERBS
		, rreq->mr, rreq->mr?rreq->mr->rkey:0
#endif
		);
		// peer could be PSM2_MQ_ANY_ADDR or 
		// could output sreq->req_data.peer->epid
}

void ips_proto_report_inflight(psm2_ep_t ep)
{
	struct ptl_ips *ptl = (struct ptl_ips *)(ep->ptl_ips.ptl);

	if (! _HFI_VDBG_ON)
		return;

	_HFI_VDBG("MQ Send Req in flight:\n");
	psmi_mpool_foreach(ep->mq->sreq_pool, NULL, sreq_inflight_callback);

	if (ptl->proto.protoexp)
		ips_protoexp_report_send_inflight(ep, ptl->proto.protoexp);

	_HFI_VDBG("MQ Recv Req in flight:\n");
	psmi_mpool_foreach(ep->mq->rreq_pool, NULL, rreq_inflight_callback);

	if (ptl->proto.protoexp)
		ips_protoexp_report_recv_inflight(ptl->proto.protoexp);
}
