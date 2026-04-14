#ifdef PSM_VERBS
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
#include <netdb.h>
//#include <infiniband/verbs.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "psm_user.h"
#include "psm_mq_internal.h"
#include "psm_am_internal.h"
#ifdef PSM_VERBS
#ifdef RNDV_MOD
#include "psm_rndv_mod.h"
#endif
#endif
#include "utils_user.h"
#include "ips_proto_params.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#ifdef PSM_FI
#include "ips_config.h"
#endif

// convert MTU enums to bytes
// TBD - is there a way to specify MTU > 4K, such as 9000 byte jumbo
#define MTU_FIX          (7) // mtu_ind of 1 (256) => 2^(7+1)
#define MTU_SIZE(mtu_ind) (((uint64_t)1 << (MTU_FIX + mtu_ind)))

static void psm3_verbs_parse_params(psm2_ep_t ep);
static psm2_error_t verbs_open_dev(psm2_ep_t ep, int unit, int port, int addr_index, psm2_uuid_t const job_key);
static psm2_error_t
check_port_state(psm2_ep_t ep);
static struct ibv_qp* ud_qp_create(psm2_ep_t ep);
static psm2_error_t modify_ud_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp);
static psm2_error_t modify_ud_qp_to_rtr(psm2_ep_t ep, struct ibv_qp *qp);
static psm2_error_t modify_ud_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp);
static const char *link_layer_str(int8_t link_layer);
static enum psm3_ibv_rate verbs_get_rate(uint8_t width, uint8_t speed);

void psm3_ep_free_verbs(psm2_ep_t ep);
#ifdef RNDV_MOD
static void deregister_rv_conn_stats(psm2_ep_t ep);
static void deregister_rv_event_stats(psm2_ep_t ep);
#endif

// initialize the ep->verbs_ep portion of the ep
psm2_error_t
psm3_ep_open_verbs(psm2_ep_t ep, int unit, int port, int addr_index, psm2_uuid_t const job_key)
{
	int flags;

	// make sure all fields are empty.
	memset(&ep->verbs_ep,0,sizeof(ep->verbs_ep));

	ep->verbs_ep.qkey = (*(uint32_t*)job_key) & 0x7FFFFFFF; // use 1st 31 bits of job_key (MSB is reserved)

	if (_HFI_PRDBG_ON) {
		char uuid_str[64];
		memset(&uuid_str, 0, sizeof(uuid_str));
		uuid_unparse(job_key, uuid_str);
		_HFI_PRDBG("job key %s qkey=0x%x\n", uuid_str, ep->verbs_ep.qkey);
	}

	psm3_verbs_parse_params(ep);

	if (PSM2_OK != verbs_open_dev(ep, unit, port, addr_index, job_key)) {
		// verbs_open_dev already posted error.
		goto fail;
	}

	// compute an appropriate PSM payload size based on the UD MTU
	// and save result into ep->mtu
	if (PSM2_OK != check_port_state(ep)) {
		goto fail;
	}

	// we'll poll, so no need to allocate an event channel
	// 		eg. ibv_create_comp_channel

	// TBD - should we simply use ep->pd and remove verbs_ep.pd field
	errno = 0;
	ep->verbs_ep.pd = ibv_alloc_pd(ep->verbs_ep.context);
	if (! ep->verbs_ep.pd) {
		_HFI_ERROR("Unable to alloc PD on %s: %s\n", ep->dev_name, strerror(errno));
		goto fail;
	}
	// for use by MR cache
#ifdef RNDV_MOD
	ep->cmd_fd = ep->verbs_ep.context->cmd_fd;
#endif
	ep->pd = ep->verbs_ep.pd;

	// planned QP sizes, also influences CQ sizes
	// PSM3_NUM_SEND_WQES, PSM3_NUM_RECV_WQES

	// we use ep as the cq_context (would be in callbacks if any)
	// we don't setup a completion channel nor completion vector since we will
	// poll
	// we will never have more than hfi_num_send_wqes +  hfi_num_send_rdma
	// so CQ only needs a little headroom to be safe (1000)
	errno = 0;
	ep->verbs_ep.send_cq = ibv_create_cq(ep->verbs_ep.context, ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma + 1000, (void*)ep, NULL, 0);
	if (! ep->verbs_ep.send_cq) {
		_HFI_ERROR("Unable to create send CQ of size %u on %s: %s\n",
			ep->verbs_ep.hfi_num_send_wqes + 1000, ep->dev_name, strerror(errno));
		goto fail;
	}

	errno = 0;
	ep->verbs_ep.recv_comp_channel = ibv_create_comp_channel(ep->verbs_ep.context);
	if (! ep->verbs_ep.recv_comp_channel) {
		_HFI_ERROR("Unable to create recv CQ completion channel on %s: %s\n",
			ep->dev_name, strerror(errno));
		goto fail;
	}
	// change completion channel to non-blocking
	flags = fcntl( ep->verbs_ep.recv_comp_channel->fd, F_GETFL);
	if (0 > fcntl( ep->verbs_ep.recv_comp_channel->fd, F_SETFL, flags | O_NONBLOCK)) {
		_HFI_ERROR( "Unable to change file descriptor of completion event channel for %s: %s\n",
					ep->dev_name, strerror(errno));
		goto fail;
	}

#ifdef USE_RC
	if (IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode)) {
		// SRQ improves scalability
		struct ibv_device_attr dev_attr;
		union psmi_envvar_val envvar_val;
		int err;

		// get RDMA capabilities of device
		err = ibv_query_device(ep->verbs_ep.context, &dev_attr);
		if (err) {
			_HFI_ERROR("Unable to query device %s: %s\n", ep->dev_name, strerror(err));
			goto fail;
		}
		_HFI_DBG("max_srq=%d\n", dev_attr.max_srq);
#ifdef PSM_RC_RECONNECT_SRQ
		if (dev_attr.max_srq) {
#else
		if (dev_attr.max_srq && ! ep->allow_reconnect) {
#endif
			// SRQ will introduce some overhead and increase latency for small fabrics
			// only try SRQ on fabric with more than one node and fabric size > 4
			unsigned int default_enable =
				psm3_get_myrank_count() == -1 || psm3_get_mylocalrank_count() == -1 // no rank info
				|| (psm3_get_myrank_count() > psm3_get_mylocalrank_count() // has more than one node
					&& psm3_get_myrank_count() >= RC_SRQ_MIN_RANK_CNT); // and fabric size >= RC_SRQ_MIN_RANK_CNT (5)
			psm3_getenv("PSM3_USE_SRQ",
				"If device supports SRQ, use it [1=yes, 0=no). Default is 1 if\n"
				"fabric has more than one node and fabric size >= " STRINGIFY(RC_SRQ_MIN_RANK_CNT),
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)default_enable, &envvar_val);
			if (envvar_val.e_uint) {
				struct ibv_srq_init_attr attr = { 0 };
				attr.srq_context = ep;	// our own pointer
				attr.attr.max_wr = ep->verbs_ep.hfi_num_recv_wqes;
				attr.attr.max_sge = 1;

				errno = 0;
				ep->verbs_ep.srq = ibv_create_srq(ep->verbs_ep.pd, &attr);
				if (ep->verbs_ep.srq == NULL) {
					_HFI_ERROR("Unable to create SRQ on %s: %s\n",
						ep->dev_name, strerror(errno));
					if (errno == ENOMEM) {
						_HFI_ERROR("Requested SRQ size might be too big. Try reducing TX depth and/or inline size.\n");
						_HFI_ERROR("Requested RX depth was %u.\n", ep->verbs_ep.hfi_num_recv_wqes);
					}
					goto fail;
				}
				_HFI_DBG("created SRQ\n");
				ep->addl_nic_info = " SRQ";
			}
		}
	}
#endif /* USE_RC */

	// TBD - should we pick an EQ number
	// we use ep as the cq_context (would be in callbacks if any)
	// we will never have more than hfi_num_recv_wqes+HFI_TF_NFLOWS
	// inflight WQEs
	// so CQ only needs a little headroom to be safe (1000)
	// HFI_TF_NFLOWS (32) limits receiver side concurrent tidflows (aka inbound
	// RDMA w/immed).
	// For USER RC Eager without SRQ we can have num_recv_wqes/FRACTION per
	// QP, and we calculate the total size based on the total QPs required.
	// The CQ size for the UD QP is covered by hfi_num_recv_wqes.
	// For USER RC Eager with SRQ worse case is num_recv_wqes so we
	// add that to allow up to num_recv_wqes on UD QP and SRQ each and keep
	// the HFI_TF_NFLOWS+1000 as headroom.
	if (! ep->verbs_ep.hfi_num_recv_cqes) {
		ep->verbs_ep.hfi_num_recv_cqes = ep->verbs_ep.hfi_num_recv_wqes+HFI_TF_NFLOWS+1000;
#ifdef USE_RC
		if ((ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
			if (ep->verbs_ep.srq) {
				ep->verbs_ep.hfi_num_recv_cqes += ep->verbs_ep.hfi_num_recv_wqes;
			} else {
				int tot_cnt = psm3_get_myrank_count();
				int loc_cnt = psm3_get_mylocalrank_count();
				uint32_t rem_cnt;
				uint32_t cqes_per_qp;

				/*
				 * Check to see if MPI is used. If yes, we will calculate the total
				 * number of RC QPs. Otherwise, we use a arbitrary large number to
				 * accommodate up to 128 remote connections
				 */
				if (tot_cnt > 0 && loc_cnt > 0)
					rem_cnt = (uint32_t)(tot_cnt - loc_cnt);
				else
					rem_cnt = 128;

				cqes_per_qp = ep->verbs_ep.hfi_num_recv_wqes / VERBS_RECV_QP_FRACTION;
				ep->verbs_ep.hfi_num_recv_cqes += rem_cnt * cqes_per_qp;
			}
		}
#endif
	}

	errno = 0;
	ep->verbs_ep.recv_cq = ibv_create_cq(ep->verbs_ep.context,
						 ep->verbs_ep.hfi_num_recv_cqes,
						 (void*)ep,  ep->verbs_ep.recv_comp_channel, 0);
	if (! ep->verbs_ep.recv_cq) {
		_HFI_ERROR("Unable to create recv CQ of size %u on %s: %s\n",
			ep->verbs_ep.hfi_num_recv_cqes, ep->dev_name, strerror(errno));
		goto fail;
	}

	ep->verbs_ep.qp = ud_qp_create(ep);
	if (! ep->verbs_ep.qp)
		goto fail;

	psmi_assert_always (ep->verbs_ep.context);

	/* Construct epid for this Endpoint */
	if (ep->addr.fmt == PSMI_ADDR_FMT_IPV4) {
		ep->epid = psm3_epid_pack_ipv4(ep->addr,
						PSMI_ETH_PROTO_ROCE,
						ep->verbs_ep.qp->qp_num, 0);
		_HFI_VDBG("construct epid ipv4: %s: ip %s QP %u mtu %u\n",
						psm3_epid_fmt_internal(ep->epid, 0),
						psm3_naddr128_fmt(ep->addr, 1),
						ep->verbs_ep.qp->qp_num, ep->mtu);
	} else if (ep->addr.fmt == PSMI_ADDR_FMT_IPV6) {
		ep->epid = psm3_epid_pack_ipv6(ep->addr,
						PSMI_ETH_PROTO_ROCE,
						ep->verbs_ep.qp->qp_num, 0);
		_HFI_VDBG("construct epid ipv6: %s: ip %s QP %u mtu %u\n",
						psm3_epid_fmt_internal(ep->epid, 0),
						psm3_naddr128_fmt(ep->addr, 1),
						ep->verbs_ep.qp->qp_num, ep->mtu);
	} else {
		psmi_assert(ep->addr.fmt == PSMI_ADDR_FMT_IB);
		ep->epid = psm3_epid_pack_ib(ep->verbs_ep.port_attr.lid,
							ep->verbs_ep.qp->qp_num,
							ep->addr);
		_HFI_VDBG("construct epid ib: %s: lid 0x%x QP %u addr %s mtu %u\n",
						psm3_epid_fmt_internal(ep->epid, 0), ep->verbs_ep.port_attr.lid,
						ep->verbs_ep.qp->qp_num,
						psm3_naddr128_fmt(ep->addr, 1),
						ep->mtu);
	}
	ep->wiremode = ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK;

	// rest of resources initialized by psm3_verbs_ips_proto_init after we
	// have processed ips_proto configuration such as PSM3_MTU

	return PSM2_OK;

fail:
	psm3_ep_free_verbs(ep);
	return PSM2_INTERNAL_ERR;
}

static void
psm3_verbs_parse_params(psm2_ep_t ep)
{
	union psmi_envvar_val envvar_val;

	ep->rdmamode = psm3_verbs_parse_rdmamode(0);
	ep->mr_cache_mode = psm3_verbs_parse_mr_cache_mode(ep->rdmamode, 0);

	// when enabled this can improve MR cache hit rate
	psm3_getenv("PSM3_MR_ACCESS",
			"When register MR for send, should inbound recv access be allowed (1=yes, 0=no) [1]",
			PSMI_ENVVAR_LEVEL_HIDDEN,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1, &envvar_val);
	ep->mr_access = envvar_val.e_uint;

	/* Get number of send WQEs
	 */
	psm3_getenv("PSM3_NUM_SEND_WQES",
			"Number of send WQEs to allocate [4080]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_SEND_QP_ENTRIES, &envvar_val);
	ep->verbs_ep.hfi_num_send_wqes = envvar_val.e_uint;

	psm3_getenv("PSM3_SEND_REAP_THRESH",
			"Number of outstanding send WQEs before reap CQEs [256]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_SEND_CQ_REAP, &envvar_val);
	ep->verbs_ep.hfi_send_reap_thresh = envvar_val.e_uint;

	/* Get number of recv WQEs
	 */
	psm3_getenv("PSM3_NUM_RECV_WQES",
			"Number of recv WQEs to allocate [4095]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_RECV_QP_ENTRIES, &envvar_val);
	ep->verbs_ep.hfi_num_recv_wqes = envvar_val.e_uint;

	/* Get number of recv CQEs
	 */
	psm3_getenv("PSM3_NUM_RECV_CQES",
			"Number of recv CQEs to allocate\n"
			"(0 will calculate as PSM3_NUM_RECV_WQES+1032 for PSM3_RDMA=0-2\n"
			"for PSM3_RDMA=3 with SRQ, allow an additional PSM3_NUM_RECV_WQES\n"
			"for PSM3_RDMA=3 without SRQ, calculate based on total QPs) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->verbs_ep.hfi_num_recv_cqes = envvar_val.e_uint;

// TBD conditional parsing based on rdmamode
	psm3_getenv("PSM3_NUM_SEND_RDMA",
		"Number of user space send RDMA to allow [128]",
		PSMI_ENVVAR_LEVEL_USER,
		PSMI_ENVVAR_TYPE_UINT,
		(union psmi_envvar_val)VERBS_NUM_SEND_RDMA, &envvar_val);
	ep->hfi_num_send_rdma = envvar_val.e_uint;

	/* Get RC QP timeout and retry
	 */
	psm3_getenv("PSM3_QP_TIMEOUT",
			"Number of microseconds for RC QP timeouts [536870]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_ULONG,
			(union psmi_envvar_val)VERBS_QP_TIMEOUT, &envvar_val);
	ep->verbs_ep.hfi_qp_timeout = psm3_timeout_usec_to_mult(envvar_val.e_ulong);

	psm3_getenv("PSM3_QP_RETRY",
			"Limit on retries after RC QP timeout or RNR [7]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_QP_RETRY, &envvar_val);
	ep->verbs_ep.hfi_qp_retry = (envvar_val.e_uint <= VERBS_QP_MAX_RETRY)?
								envvar_val.e_uint:VERBS_QP_MAX_RETRY;

#ifdef RNDV_MOD
// TBD conditional parsing based on mr_cache_mode
	/* Size of RV Cache - only used for MR_CACHE_MODE_RV or KERNEL,
	 * otherwise ignored
	 */
	// RV defaults are sufficient for default PSM parameters
	// but if user adjusts ep->hfi_num_send_rdma or mq->ips_cpu_window_rv
	// they also need to increase the cache size.  psm3_verbs_alloc_mr_cache
	// will verify cache size is sufficient.
	// min size is (HFI_TF_NFLOWS + ep->hfi_num_send_rdma) *
	// chunk size (psm3_mq_max_window_rv(mq, 0) after
	// psm3_mq_initialize_params)
	psm3_getenv("PSM3_RV_MR_CACHE_SIZE",
			"kernel space MR cache size"
			" (MBs, 0 lets rv module decide) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->rv_mr_cache_size = envvar_val.e_uint;
	// TBD - we could check cache_size >= minimum based on:
	// 		(HFI_TF_NFLOWS + ep->hfi_num_send_rdma)
	//		* psm3_mq_max_window_rv(mq, 0)
	// and automatically increase with warning if not?
#ifdef PSM_HAVE_GPU
	ep->rv_gpu_cache_size = psmi_parse_gpudirect_rv_gpu_cache_size(0);
	// TBD - we could check gpu_cache_size >= minimum based on:
	// 		(HFI_TF_NFLOWS + ep->hfi_num_send_rdma)
	//		* psm3_mq_max_window_rv(mq, 1)
	// and automatically increase with warning if not?
#endif

	psm3_getenv("PSM3_RV_QP_PER_CONN",
			"Number of sets of RC QPs per RV connection (0 lets rv module decide) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->verbs_ep.rv_num_conn = envvar_val.e_uint;

	psm3_getenv("PSM3_RV_Q_DEPTH",
			"Size of QPs and CQs per RV QP (0 lets rv module decide) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->verbs_ep.rv_q_depth = envvar_val.e_uint;

	psm3_getenv("PSM3_RV_HEARTBEAT_INTERVAL",
			"RV End-point heartbeat interval in milliseconds. 0 for no heartbeat [1000]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1000, &envvar_val);
	ep->verbs_ep.rv_hb_interval = envvar_val.e_uint;

	psm3_getenv_range("PSM3_RV_FR_PAGE_LIST_LEN",
			  "RV Fast-registration page list length (0 lets rv module decide) [1024]", NULL,
			  PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			  (union psmi_envvar_val)RV_FR_PAGE_LIST_LEN_DEFAULT,
			  (union psmi_envvar_val)RV_FR_PAGE_LIST_LEN_MIN,
			  (union psmi_envvar_val)UINT32_MAX,
			  NULL, NULL, &envvar_val);
	ep->verbs_ep.rv_fr_page_list_len = envvar_val.e_uint;

#endif // RNDV_MOD

#if defined(RNDV_MOD) || defined(USE_RC)
#ifdef PSM_RC_RECONNECT
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)
		|| IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode)) {
			// depricated parameter
#else
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
#endif
#ifdef PSM_RC_RECONNECT
		// RC QP recovery is tech preview, default to 0 (off) for now
		// only parse PSM3_RV_RECONNECT_TIMEOUT when using KERNEL_QP (RV)
		if (IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode)) {
			envvar_val.e_uint = 0;
		} else
#endif
		{
			psm3_getenv("PSM3_RV_RECONNECT_TIMEOUT",
#ifdef PSM_RC_RECONNECT
				"RV and RC End-point minimum re-connection timeout in seconds. Deprecated. 0 for no connection recovery [30]",
				PSMI_ENVVAR_LEVEL_HIDDEN,
#else
				"RV and RC End-point minimum re-connection timeout in seconds. 0 for no connection recovery [30]",
				PSMI_ENVVAR_LEVEL_USER,
#endif
				PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)30, &envvar_val);
		}
		ep->reconnect_timeout = envvar_val.e_uint;
#ifdef PSM_RC_RECONNECT
		psm3_getenv("PSM3_RECONNECT_TIMEOUT",
			"RV and RC End-point minimum re-connection timeout in seconds. 0 for no connection recovery (default of 30 for RV, 0 for user RC QPs)",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)ep->reconnect_timeout,
			&envvar_val);
		ep->reconnect_timeout = envvar_val.e_uint;
		ep->allow_reconnect = (IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode)
			&& ep->reconnect_timeout);
#endif
	}
#endif /* RNDV_MOD || USE_RC */
}

// complete initialization which requires ips_proto.
// initialize the buffer pools and move the UD QP to RTS
psm2_error_t
psm3_verbs_ips_proto_init(struct ips_proto *proto, uint32_t cksum_sz)
{
	psm2_ep_t ep = proto->ep;
	union psmi_envvar_val env_mtu;

	// defaults for SDMA thresholds.  These may be updated when
	// PSM3_* env for SDMA are parsed later in psm3_ips_proto_init.
	proto->iovec_thresh_eager = 8192;
	proto->iovec_thresh_eager_blocking = 8192;
#ifdef PSM_HAVE_GPU
	proto->iovec_gpu_thresh_eager = 128;
	proto->iovec_gpu_thresh_eager_blocking = 128;
#endif

	// hash for dispersive routing
	proto->epinfo.ep_hash = ep->verbs_ep.qp->qp_num;// low 8b only

	// at this point ep->mtu is our HW capability found during open
	// and adjusted to allow for PSM headers so ep->mtu reflects maximum
	// PSM payload (not yet adjusted for optional cksum_sz)
	char help[128];

	if ((ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
		snprintf(help, sizeof(help), "Upper bound on PSM3 payload (<=0 uses port MTU): 1-7, 1024-PSM3_MQ_RNDV_NIC_THRESH(%u)", ep->mq->rndv_nic_thresh);
	} else {
		snprintf(help, sizeof(help), "Upper bound on packet MTU (<=0 uses port MTU): 1-5,256,512,1024,2048,4096,8192");
	}
	/* See if user specifies a MTU to use */
	if (!psm3_getenv("PSM3_MTU", help,
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
		(union psmi_envvar_val)-1, &env_mtu)) {
		uint32_t mtu; // in bytes
		// use OPA_MTU_MAX so we don't round down to min MTU when
		// OPA enum values mistakenly used here.
		if (env_mtu.e_int >= IBTA_MTU_MIN && env_mtu.e_int <= OPA_MTU_MAX) { //enum
			mtu = opa_mtu_enum_to_int((enum opa_mtu)env_mtu.e_int);
		} else if (env_mtu.e_int < IBTA_MTU_MIN) { // pick default
			mtu = ep->mtu + MAX_PSM_HEADER; // use wire MTU
		} else if ((ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC
			   && ! ep->allow_reconnect) { // use as local PSM3 MTU
			// Under RDMA3 mode without reconnect, UD is only used
			// for ctr msg only that shall be smaller than
			// wire MTU. It's safe to increase PSM3 MTU beyond wire
			// MTU because RC will be used, and the NIC driver will
			// segment a msg into multiple packets to ensure 
			// each pkt size is within wire MTU. 

			mtu = env_mtu.e_int;
			// only apply PSM3_MTU on eager messages
			if (env_mtu.e_int > ep->mq->rndv_nic_thresh)
				mtu = ep->mq->rndv_nic_thresh;
			if (env_mtu.e_int < opa_mtu_enum_to_int(IBTA_MTU_MIN))
				mtu = opa_mtu_enum_to_int(IBTA_MTU_MIN);
			// round down to nearest multiple of 64
			mtu = ROUNDDOWNP2(mtu, 64);
			proto->epinfo.ep_mtu = mtu - MAX_PSM_HEADER;
		} else { // wash through enum to force round up to next valid MTU
			mtu = opa_mtu_enum_to_int(opa_mtu_int_to_enum(env_mtu.e_int));
		}

		// only allow MTU decrease
		// PSM3_MTU specified ends up being used as max verbs payload
		// so decrease by PSM HEADER size (and cksum below)
		if (ep->mtu > mtu - MAX_PSM_HEADER)
			ep->mtu = mtu - MAX_PSM_HEADER;
	}

	/* allow space for optional software managed checksum (for debug) */
	ep->mtu -= cksum_sz;
	// if proto->epinfo.ep_mtu is not set, use ep->mtu as our final choice 
	// of local PSM payload we can support
	if (!proto->epinfo.ep_mtu)
		proto->epinfo.ep_mtu = ep->mtu;

	if (PSM2_OK != psm_verbs_alloc_send_pool(ep, ep->verbs_ep.pd, &ep->verbs_ep.send_pool, 
				// save 1 send WQE just to be paranoid (should be unnecessary)
				min(ep->verbs_ep.hfi_num_send_wqes, ep->verbs_ep.qp_cap.max_send_wr-1),
				// want to end up with multiple of cache line (64)
				// proto->epinfo.ep_mtu+MAX_PSM_HEADERS will be power of 2 verbs MTU
				// be conservative (+BUFFER_HEADROOM)
				proto->epinfo.ep_mtu + MAX_PSM_HEADER + BUFFER_HEADROOM
		)) {
		_HFI_ERROR( "Unable to allocate UD send buffer pool\n");
		goto fail;
	}
	if (PSM2_OK != psm_verbs_init_send_allocator(&ep->verbs_ep.send_allocator, 
					&ep->verbs_ep.send_pool)) {
		_HFI_ERROR( "Unable to init UD send buffer allocator\n");
		goto fail;
	}

	ep->verbs_ep.send_reap_thresh = min(ep->verbs_ep.hfi_send_reap_thresh, ep->verbs_ep.send_pool.send_total/2);
	_HFI_PRDBG("reaping when %u posted.\n", ep->verbs_ep.send_reap_thresh);

	if (PSM2_OK != psm_verbs_alloc_recv_pool(ep, 0, ep->verbs_ep.qp, &ep->verbs_ep.recv_pool,
				min(ep->verbs_ep.hfi_num_recv_wqes, ep->verbs_ep.qp_cap.max_recv_wr),
				// want to end up with multiple of cache line (64)
				// proto->epinfo.ep_mtu+MAX_PSM_HEADERS will be power of 2 verbs MTU
				// be conservative (+BUFFER_HEADROOM)
				proto->epinfo.ep_mtu + MAX_PSM_HEADER + BUFFER_HEADROOM
		)) {
		_HFI_ERROR( "Unable to allocate UD recv buffer pool\n");
		goto fail;
	}
#ifdef USE_RC
	if (ep->verbs_ep.srq) {
		if (PSM2_OK != psm_verbs_alloc_recv_pool(ep, 1, ep->verbs_ep.srq, &ep->verbs_ep.srq_recv_pool,
					ep->verbs_ep.hfi_num_recv_wqes,
					 (proto->ep->rdmamode == IPS_PROTOEXP_FLAG_RDMA_USER)? 0
					// want to end up with multiple of cache line (64)
					// proto->epinfo.ep_mtu+MAX_PSM_HEADERS will be power of 2 verbs MTU
					// be conservative (+BUFFER_HEADROOM)
					: (proto->epinfo.ep_mtu + MAX_PSM_HEADER + BUFFER_HEADROOM)
			)) {
			_HFI_ERROR( "Unable to allocate SRQ recv buffer pool\n");
			goto fail;
		}
		if (PSM2_OK != psm3_ep_verbs_prepost_recv(&ep->verbs_ep.srq_recv_pool)) {
			_HFI_ERROR( "Unable to prepost recv buffers on SRQ for %s port %u\n", ep->dev_name, ep->portnum);
			goto fail;
		}
	}
#endif /* USE_RC */

	// no send segmentation, max_segs will constrain
	ep->chunk_max_segs = 1;
	ep->chunk_max_size = proto->epinfo.ep_mtu;
#ifdef PSM_BYTE_FLOW_CREDITS
	// let flow_credits be the control
	proto->flow_credit_bytes = proto->epinfo.ep_mtu * proto->max_credits;
	_HFI_DBG("initial flow_credits %d bytes %d\n",
				proto->flow_credits, proto->flow_credit_bytes);
#else
	_HFI_DBG("initial flow_credits %d\n", proto->flow_credits);
#endif

	if (PSM2_OK != modify_ud_qp_to_init(ep, ep->verbs_ep.qp)) {
		goto fail;
	}

	if (PSM2_OK != psm3_ep_verbs_prepost_recv(&ep->verbs_ep.recv_pool)) {
		_HFI_ERROR( "Unable to prepost recv buffers on QP for %s port %u\n", ep->dev_name, ep->portnum);
		goto fail;
	}

	// move QP to RTR and RTS
	if(PSM2_OK != modify_ud_qp_to_rtr(ep, ep->verbs_ep.qp)) {
		goto fail;
	}
	if(PSM2_OK != modify_ud_qp_to_rts(ep, ep->verbs_ep.qp)) {
		goto fail;
	}
	_HFI_PRDBG("created UD QP %p (%u)\n", ep->verbs_ep.qp, ep->verbs_ep.qp->qp_num);

	/*
	 * Pre-calculate the PSN mask to support 31 bit PSN.
	 */
	proto->psn_mask = 0x7FFFFFFF;
	/* 12 bit pktlen (limit to <= 4095 32 bit words per packet */
	proto->pktlen_mask = 0xFFF;

	return PSM2_OK;

fail:
#ifdef USE_RC
	psm_verbs_free_recv_pool(&ep->verbs_ep.srq_recv_pool);
#endif
	psm_verbs_free_send_pool(&ep->verbs_ep.send_pool);
	psm_verbs_free_recv_pool(&ep->verbs_ep.recv_pool);
	return PSM2_INTERNAL_ERR;
}

// Fetch current link state to update linkinfo fields in ips_proto:
// 	ep_base_lid, ep_lmc, ep_link_rate
// These are all fields which can change during a link bounce.
// Note "active" state is not adjusted as on link down PSM will wait for
// the link to become usable again so it's always a viable/active device
// afer initial PSM startup has selected devices.
// Called during initialization of ips_proto during ibta_init as well
// as during a link bounce.
// TBD - may be able to call this from HAL ips_proto_init as well as
// directly within HAL event processing, in which case this could
// be completely internal to HAL and not exposed in HAL API
psm2_error_t psm3_verbs_ips_proto_update_linkinfo(struct ips_proto *proto)
{
	uint16_t lid;

	// for Ethernet LID of 1 is returned
	lid = psm3_epid_lid(proto->ep->epid);
	proto->epinfo.ep_base_lid = __cpu_to_be16(lid);

	// TBD don't yet have multi-path dispersive routing
	// For IB could directly fetch LMC
	// for RoCE maybe we use env to derrive multiple sequential IP
	// addresses, sort of like an LMC concept
	// or use ECMP or other mechanism
	proto->epinfo.ep_lmc = 0; // No LMC for UD
	proto->epinfo.ep_link_rate = proto->ep->verbs_ep.active_rate;
	return PSM2_OK;
}

// This works for UD address vectors as well as the ah_attr in an RC QP attrs
psm2_error_t psm3_verbs_ips_path_rec_to_ah_attr(psm2_ep_t ep,
				const ips_path_rec_t *path_rec,
				struct ibv_ah_attr *ah_attr)
{
	memset(ah_attr, 0, sizeof *ah_attr);

	// we keep PR in network byte order
	// ah_attr is in CPU byte order except for GIDs which are always
	// in network byte order
	ah_attr->sl = path_rec->pr_sl;
	ah_attr->port_num = ep->portnum;
	ah_attr->static_rate = path_rec->pr_static_rate;
	// NIC/HCA/HFI will only look at low "LMC" worth of bits
	ah_attr->src_path_bits = __be16_to_cpu(path_rec->pr_slid);
	ah_attr->dlid = __be16_to_cpu(path_rec->pr_dlid);
	// for OPA/IB we use dlid and is_global=0, for eth use dgid and is_global=1
	// IPv6 ::0 is the default route and only appears in routing tables
	//if (ep->verbs_ep.link_layer != IBV_LINK_LAYER_ETHERNET) {
	//if (PSMI_ADDR_FMT_IS_ETH(ep->verbs_ep.addr_fmt)) {
	if (path_rec->pr_gid_lo == 0 && path_rec->pr_gid_hi == 0) {
		// OPA or IB
		ah_attr->is_global  = 0;
		_HFI_CONNDBG("creating AH with DLID %u\n", ah_attr->dlid);
	} else {
		ah_attr->is_global  = 1;
		ah_attr->grh.dgid.global.subnet_prefix = path_rec->pr_gid_hi;
		ah_attr->grh.dgid.global.interface_id = path_rec->pr_gid_lo;
		ah_attr->grh.sgid_index = ep->verbs_ep.lgid_index;
		ah_attr->grh.hop_limit = 0xFF;
		ah_attr->grh.traffic_class = 0;
		_HFI_CONNDBG("creating AH with DLID %u DGID: %s\n",
			ah_attr->dlid, psm3_ibv_gid_fmt(ah_attr->grh.dgid, 0));
	}
	return PSM2_OK;
}

psm2_error_t psm3_verbs_ips_make_ah(psm2_ep_t ep, ips_path_rec_t *path_rec)
{
	struct ibv_ah_attr ah_attr;

	if (path_rec->verbs.pr_ah) {
		_HFI_CONNDBG("make_ah called second time on given path_rec, skipping\n");
		return PSM2_OK;
	}
	if (PSM2_OK != psm3_verbs_ips_path_rec_to_ah_attr(ep, path_rec, &ah_attr)) {
		_HFI_ERROR( "Unable to convert path_rec to AH for %s port %u\n", ep->dev_name, ep->portnum);
		return PSM2_INTERNAL_ERR;
	}
	errno = 0;
	path_rec->verbs.pr_ah = ibv_create_ah(ep->verbs_ep.pd, &ah_attr);
	if (! path_rec->verbs.pr_ah) {
		_HFI_ERROR("Unable to create AH for %s: %s\n", ep->dev_name, strerror(errno));
		if (errno == ETIMEDOUT)
			return PSM2_EPID_PATH_RESOLUTION;
		else
			return PSM2_INTERNAL_ERR;
	}
	_HFI_CONNDBG("created AH %p\n", path_rec->verbs.pr_ah);
	// PSM doesn't free path_rec structures on shutdown, so this will
	// simply leak and be cleaned up by the kernel close when we shutdown
	return PSM2_OK;
}

#ifdef RNDV_MOD
void psm3_verbs_ips_path_rec_to_ib_user_path_rec(psm2_ep_t ep,
		const ips_path_rec_t *path_rec, union ibv_gid *dgid,
		struct ib_user_path_rec *path)
{
	memset(path, 0, sizeof(*path));
	memcpy(&path->sgid, &ep->verbs_ep.lgid, sizeof(path->sgid));
	memcpy(&path->dgid, dgid, sizeof(path->dgid));
	path->slid = path_rec->pr_slid; /* __be16 */
	path->dlid = path_rec->pr_dlid; /* __be16 */
	//path->raw_traffic
	//path->flow_label
	path->reversible = 1;
	path->mtu = opa_mtu_int_to_enum(path_rec->pr_mtu);
	path->pkey = __cpu_to_be16(path_rec->pr_pkey); /* __be16 */
	// hoplimit indicates if need GRH (aka implies is_global)
	// for OPA/IB we use dlid and hop_limit=0, for eth use dgid and 0xFF
	// IPv6 ::0 is the default route and only appears in routing tables
	//path->hop_limit = (ep->verbs_ep.link_layer != IBV_LINK_LAYER_ETHERNET)
	//path->hop_limit = if (!PSMI_ADDR_FMT_IS_ETH(ep->verbs_ep.addr_fmt))
	path->hop_limit = (path_rec->pr_gid_lo == 0 && path_rec->pr_gid_hi == 0)
						? 0 : 0xFF;
	//path->traffic_class
	path->numb_path = 1;
	path->sl = path_rec->pr_sl;
	path->mtu_selector = 2;  /* Exactly the given MTU */
	path->rate_selector = 2; /* Exactly the given rate */
	// ips_path_rec.pr_static_rate is negotiated in PSM REQ/REP
	// then also use negotiated rate in user RC QP, ah_attr above and here
	path->rate = path_rec->pr_static_rate;
	path->packet_life_time_selector = 2; /* Exactly the given LT */
	// the value supplied here will be increased by the CM based on ack_delay
	// typically ack_delay will be small compared to packet_life_time
	// in which case the CM wil end up using packet_life_time+1 as the timeout
	// so we pass timeout-1 here so final timeout is usually what was requested
	path->packet_life_time = ep->verbs_ep.hfi_qp_timeout - 1;
	//path->preferences
}
#endif // RNDV_MOD

/* Handle HAL specific initialization of an ips_path_rec
 * as part of fetching or hand building a path record.
 * Responsible for all fields in the HAL specific union and any tweaks to
 * other fields which may be HAL specific (such as pr_mtu).
 * response is only provided when we are building a ips_path_rec from a
 * fetched ibta_path_rec.  Otherwise we are building it solely based on
 * our own end point and what our caller knows from the EPID.
 */
psm2_error_t psm3_verbs_ips_path_rec_init(struct ips_proto *proto,
				struct ips_path_rec *path_rec,
				struct _ibta_path_rec *response)
{
	if (response) {
		// we are building path_rec from an IBTA path_rec query response

		// this should not happen since we are using a LID to LID query
		// but at some point we need to figure out how to deal with
		// virtualized IB environments where a GRH may be needed
		// HOP Limit >1 indicates a global route with a GRH
		if ((__be32_to_cpu(response->hop_flow_raw) & 0xFF) > 1) {
			_HFI_ERROR
				("Global Routed Path Record not supported SLID 0x%d DLID 0x%x\n",
				__be16_to_cpu(response->slid),
				__be16_to_cpu(response->dlid));
			return PSM2_EPID_PATH_RESOLUTION;
		}

		/* fixup up pr_mtu */
		path_rec->pr_mtu = min(opa_mtu_enum_to_int(response->mtu & 0x3f)
							- MAX_PSM_HEADER,
					 proto->epinfo.ep_mtu);
	}
	return psm3_verbs_ips_make_ah(proto->ep, path_rec);
}

int psm3_verbs_poll_type(int poll_type, psm2_ep_t ep)
{
	int err;

	switch (poll_type) {
	case PSMI_HAL_POLL_TYPE_NONE:
		// no events for solicted and unsolictited recv
		_HFI_PRDBG("disable solicited event - noop\n");
		// this is only done once during PSM shutdown of rcvthread.
		// Verbs events are one-shots.  No way to disable.  However once PSM
		// stops rcvthread shortly after this call, no one will be polling
		// for these events so worst case only a few additional events occur
		break;
	case PSMI_HAL_POLL_TYPE_URGENT:
		// set for event on solicted recv (urgent PSM protocol pkts)
		_HFI_PRDBG("enable solicited event\n");
		err = ibv_req_notify_cq(ep->verbs_ep.recv_cq, 1);
		if (err) {
			_HFI_ERROR("Can't request solicitied RQ events on %s: %s\n",
				ep->dev_name, strerror(err));
			return -1;
		}
		break;
	case PSMI_HAL_POLL_TYPE_ANYRCV:
		_HFI_VDBG("enable all events\n");
		err = ibv_req_notify_cq(ep->verbs_ep.recv_cq, 0);
		if (err) {
			_HFI_ERROR("Can't request all RQ events on %s: %s\n",
				ep->dev_name, strerror(err));
			return -1;
		}
		break;
	default:
		psmi_assert(0);
		return -1;
	}
	return 0;
}

// free reources in ep->verbs_ep portion of the ep
void psm3_ep_free_verbs(psm2_ep_t ep)
{
	if (ep->verbs_ep.qp) {
		ibv_destroy_qp(ep->verbs_ep.qp);
		ep->verbs_ep.qp = NULL;
	}
	psm_verbs_free_send_pool(&ep->verbs_ep.send_pool);
	psm_verbs_free_recv_pool(&ep->verbs_ep.recv_pool);
	if (ep->verbs_ep.recv_cq) {
		ibv_destroy_cq(ep->verbs_ep.recv_cq);
		ep->verbs_ep.recv_cq = NULL;
	}
	if (ep->verbs_ep.recv_comp_channel) {
		ibv_destroy_comp_channel(ep->verbs_ep.recv_comp_channel);
		ep->verbs_ep.recv_comp_channel = NULL;
	}

	if (ep->verbs_ep.send_cq) {
		ibv_destroy_cq(ep->verbs_ep.send_cq);
		ep->verbs_ep.send_cq = NULL;
	}
#ifdef RNDV_MOD
	// must close rv prior to dealloc pd or closing device in case
	// MR_CACHE_MODE_KERNEL with user MRs in RV cache
	if (ep->rv) {
		if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
			deregister_rv_conn_stats(ep);
			deregister_rv_event_stats(ep);
		}
		psm3_rv_close(ep->rv);
		ep->rv = NULL;
	}
#endif
#ifdef USE_RC
	if (ep->verbs_ep.srq) {
		ibv_destroy_srq(ep->verbs_ep.srq);
		ep->verbs_ep.srq = NULL;
	}
	psm_verbs_free_recv_pool(&ep->verbs_ep.srq_recv_pool);
#endif
	if (ep->verbs_ep.pd) {
		ibv_dealloc_pd(ep->verbs_ep.pd);
		ep->verbs_ep.pd = NULL;
	}
	if (ep->verbs_ep.context) {
		ibv_close_device(ep->verbs_ep.context);
		ep->verbs_ep.context = NULL;
	}
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}
}

// ep argument is only for calloc to associate memory statistics with ep
// do NOT use ep->verbs_ep.*_pool in this function, instead of pool
// to access buffering fields.  This function will be called for other pools
// which are tracked in other structures but still part of the ep's memory stats
psm2_error_t psm_verbs_alloc_send_pool(psm2_ep_t ep, struct ibv_pd *pd,
			psm3_verbs_send_pool_t pool,
			uint32_t send_total, uint32_t send_buffer_size)
{
	memset(pool,0,sizeof(*pool));

	// use what we got, make sure it's a multiple of coallesce
	// don't grow beyond requested, otherwise we could exceed CQ sizes
	pool->send_total = ROUNDDOWN(send_total, VERBS_SEND_CQ_COALLESCE);

	if (send_total && send_buffer_size) {
		// allocate send buffers
		int i;
		pool->send_buffer_size = send_buffer_size;
		pool->send_num_free = pool->send_total;
		pool->send_buffers = (uint8_t *)psmi_memalign(ep, NETWORK_BUFFERS, CPU_PAGE_ALIGN,
													 pool->send_total*pool->send_buffer_size);
		if (! pool->send_buffers) {
			_HFI_ERROR( "can't alloc send buffers");
			goto fail;
		}
		// This can allows faster copies from GPU to the send buffer
		PSM3_GPU_REGISTER_HOSTMEM( pool->send_buffers,
				pool->send_total*pool->send_buffer_size);

		_HFI_PRDBG("send pool: buffers: %p size %u\n",  pool->send_buffers, pool->send_buffer_size);
		pool->send_bufs = (struct verbs_sbuf *)psmi_calloc(ep, NETWORK_BUFFERS,
							 pool->send_total*sizeof(struct verbs_sbuf), 1);
		if (! pool->send_bufs) {
			_HFI_ERROR("can't alloc send buffers ctrl");
			goto fail;
		}
		// prepare free list, put lower numbered buffers at head of free list
		for (i=pool->send_total-1; i >= 0; i--) {
			pool->send_bufs[i].buffer = &(pool->send_buffers[send_buffer_start(pool, i)]);
			pool->send_bufs[i].next = pool->send_free;
#ifdef PSM_DEBUG
			// help detect and debug double free
			pool->send_bufs[i].allocator = NULL;
			pool->send_bufs[i].freed_by = "init";
#endif
			pool->send_free = &(pool->send_bufs[i]);
		}
		_HFI_PRDBG("%u Send Buffers of %u bytes each allocated at %p.\n", pool->send_total, pool->send_buffer_size,
			pool->send_buffers);

		// UD doesn't support RDMA, so we just need local NIC to be able to
		// access our buffers with kernel bypass (IBV_ACCESS_LOCAL_WRITE)
		// technically we probably don't need LOCAL_WRITE for send buffers
		errno = 0;
		pool->send_buffer_mr = ibv_reg_mr(
						pd, pool->send_buffers,
						pool->send_total*pool->send_buffer_size,
						IBV_ACCESS_LOCAL_WRITE);
		if (! pool->send_buffer_mr) {
			_HFI_ERROR("Unable to alloc send buffer MR on %s: %s\n",
				ep->dev_name, strerror(errno));
			goto fail;
		}
	}
	return PSM2_OK;

fail:
	psm_verbs_free_send_pool(pool);
	return PSM2_INTERNAL_ERR;
}

extern psm2_error_t psm_verbs_init_send_allocator(
			psm3_verbs_send_allocator_t allocator,
			psm3_verbs_send_pool_t pool)
{

	memset(allocator,0,sizeof(*allocator));
	allocator->pool = pool;
	allocator->send_num_til_coallesce = VERBS_SEND_CQ_COALLESCE;
	return PSM2_OK;
}


// ep argument is only for calloc to associate memory statistics with ep
// do NOT use ep->verbs_ep.*_pool in this function, instead of pool
// to access buffering fields.  This function will be called for other pools
// which are tracked in other structures but still part of the ep's memory stats
// For RC QPs receiving only RDMA Write with immediate, no buffer space is
// needed.  Caller will specify recv_buffer_size==0 with a recv_total.
psm2_error_t psm_verbs_alloc_recv_pool(psm2_ep_t ep, uint32_t for_srq,
			void *qp_srq, psm3_verbs_recv_pool_t pool,
			uint32_t recv_total, uint32_t recv_buffer_size)
{
	memset(pool,0,sizeof(*pool));

#ifdef USE_RC
	pool->for_srq = for_srq;
	if (for_srq)
		pool->srq = (struct ibv_srq *)qp_srq;	// save a reference
	else
#endif
		pool->qp = (struct ibv_qp *)qp_srq;	// save a reference
#ifndef USE_RC
	psmi_assert(! for_srq);
#endif
	pool->ep = ep;
	pool->recv_total = recv_total;

	if (recv_total ) {
#if (VERBS_RECV_QP_COALLESCE > 1) || defined(USE_RC)
		int i;
#endif
		if (recv_buffer_size) {
			// allocate recv buffers
			pool->recv_buffer_size = recv_buffer_size;
			// beginning of UD QP Recv Buf always consumed with space for IB GRH
			if (
#ifdef USE_RC
				! pool->for_srq &&
#endif
				pool->qp->qp_type == IBV_QPT_UD) {
				// round up UD_ADDITION (40) to multiple of 64 for better
				// cache alignment of buffers
				pool->recv_buffer_size += ROUNDUP(UD_ADDITION, 64);
#ifdef USE_RC
				pool->addition = UD_ADDITION;
#endif
			}
			pool->recv_buffers = (uint8_t *)psmi_calloc(ep, NETWORK_BUFFERS,
							 pool->recv_total*pool->recv_buffer_size, 1);
			if (! pool->recv_buffers) {
				_HFI_ERROR( "can't alloc recv buffers");
				goto fail;
			}
			// This can allow faster copies from recv buffer to GPU
			PSM3_GPU_REGISTER_HOSTMEM(pool->recv_buffers,
				pool->recv_total*pool->recv_buffer_size);
			//printf("recv pool: buffers: %p size %u\n",  pool->recv_buffers, pool->recv_buffer_size);
#ifdef USE_RC
			pool->recv_bufs = (struct verbs_rbuf *)psmi_calloc(ep, NETWORK_BUFFERS,
								 pool->recv_total*sizeof(struct verbs_rbuf), 1);
			if (! pool->recv_bufs) {
				_HFI_ERROR("can't alloc recv buffers ctrl");
				goto fail;
			}
			// prepare rbuf handles for use as wr_id
			for (i=0; i<pool->recv_total; i++) {
				pool->recv_bufs[i].buffer = &(pool->recv_buffers[recv_buffer_start(pool, i)]);
				pool->recv_bufs[i].pool = pool;
			}
#endif
			_HFI_PRDBG("%u Recv Buffers of %u bytes each allocated at %p.\n", pool->recv_total, pool->recv_buffer_size,
				pool->recv_buffers);

			// UD doesn't support RDMA, so we just need local NIC to be able to
			// access our buffers with kernel bypass (IBV_ACCESS_LOCAL_WRITE)
			errno = 0;
			pool->recv_buffer_mr = ibv_reg_mr(
#ifdef USE_RC
							for_srq?pool->srq->pd:
#endif
								pool->qp->pd,
							pool->recv_buffers,
							pool->recv_total*pool->recv_buffer_size,
							IBV_ACCESS_LOCAL_WRITE);
			if (! pool->recv_buffer_mr) {
				_HFI_ERROR("Unable to alloc recv buffer MR on %s: %s\n",
					ep->dev_name, strerror(errno));
				goto fail;
			}
		} else {
#ifdef USE_RC
			// we want a pool for RDMA Write w/immediate recv.  No buffers
			psmi_assert(for_srq || pool->qp->qp_type != IBV_QPT_UD);
			// we use exactly 1 rbuf so wr_id can lead us to pool and qp
			pool->recv_bufs = (struct verbs_rbuf *)psmi_calloc(ep, NETWORK_BUFFERS,
							 sizeof(struct verbs_rbuf), 1);
			if (! pool->recv_bufs) {
				_HFI_ERROR("can't alloc recv buffers ctrl");
				goto fail;
			}
			// prepare rbuf handle for use as wr_id
			pool->recv_bufs->pool = pool;
			_HFI_PRDBG("%u Recv Buffers of %u bytes each allocated.\n", pool->recv_total, pool->recv_buffer_size);
#else
			_HFI_ERROR("invalid to allocate zero size non-RC buffers\n");
			goto fail;
#endif
		}
#if VERBS_RECV_QP_COALLESCE > 1
		// prebuild as much as we can
		for (i=0; i < VERBS_RECV_QP_COALLESCE; i++ ) {
			struct ibv_recv_wr *wr = &(pool->recv_wr_list[i]);
			wr->next = &(pool->recv_wr_list[i+1]);
			// condition is always true only for non-RC builds
			// coveriy[dead_error_condition]
			if (recv_buffer_size) {
				struct ibv_sge *list = &(pool->recv_sge_list[i]);
				wr->sg_list = list;
				list->length = pool->recv_buffer_size;
				list->lkey = pool->recv_buffer_mr->lkey;
				wr->num_sge = 1;	// size of sg_list
			} else {
				wr->sg_list = NULL;
				wr->num_sge = 0;	// size of sg_list
			}
		}
		// fixup end of list
		pool->recv_wr_list[VERBS_RECV_QP_COALLESCE-1].next = NULL;
		pool->next_recv_wqe = 0;
#endif
	}
	return PSM2_OK;

fail:
	psm_verbs_free_recv_pool(pool);
	return PSM2_INTERNAL_ERR;
}

void psm_verbs_free_send_pool(psm3_verbs_send_pool_t pool)
{
	if (pool->send_buffer_mr) {
		ibv_dereg_mr(pool->send_buffer_mr);
		pool->send_buffer_mr = NULL;
	}
	if (pool->send_bufs) {
		psmi_free(pool->send_bufs);
		pool->send_bufs = NULL;
	}
	if (pool->send_buffers) {
		PSM3_GPU_UNREGISTER_HOSTMEM(pool->send_buffers);
		psmi_free(pool->send_buffers);
		pool->send_buffers = NULL;
	}
	memset(pool,0,sizeof(*pool));	// in case anyone looks at other integers
}

// this is not allowed to access pool->qp, it may already be destroyed
void psm_verbs_free_recv_pool(psm3_verbs_recv_pool_t pool)
{
	if (pool->recv_buffer_mr) {
		ibv_dereg_mr(pool->recv_buffer_mr);
		pool->recv_buffer_mr = NULL;
	}
#ifdef USE_RC
	if (pool->recv_bufs) {
		psmi_free(pool->recv_bufs);
		pool->recv_bufs = NULL;
	}
#endif
	if (pool->recv_buffers) {
		PSM3_GPU_UNREGISTER_HOSTMEM(pool->recv_buffers);
		psmi_free(pool->recv_buffers);
		pool->recv_buffers = NULL;
	}
	memset(pool,0,sizeof(*pool));	// in case anyone looks at other integers
}

// the allocator tries to reallocate recently freed send buffers
// so we can tend to allocate a small set of buffers
// to improve CPU, MMU and NIC MMU hit rates
sbuf_t psm3_ep_verbs_alloc_sbuf(psm3_verbs_send_allocator_t allocator,
#ifdef PSM_RC_RECONNECT
				struct psm3_verbs_rc_qp *rc_qp,
#endif
				sbuf_t *prev_sbuf)
{
	psm3_verbs_send_pool_t pool = allocator->pool;
	sbuf_t sbuf = pool->send_free;
#ifdef PSM_RC_RECONNECT
	psmi_assert(! rc_qp || ! rc_qp->draining);
#endif
	if_pt (sbuf) {
#ifdef PSM_DEBUG
		// helps detect double free or double alloc
		psmi_assert(! sbuf->allocator);
#endif
		// take off head of free list
		pool->send_free = sbuf->next;
		pool->send_num_free--;
		sbuf->next = NULL;
		// keep a list of allocated buffers in order at alloc_head
		// and put this one at the alloc_end of the list
			// save alloc_end for immediate unalloc if needed
		//allocator->send_alloc_end_prev = allocator->send_alloc_end;
		*prev_sbuf = allocator->send_alloc_end;
		if_pf (! allocator->send_alloc_head)	// unlikely when more than 1 posted
			allocator->send_alloc_head = sbuf;
		if_pt (allocator->send_alloc_end)	// likely when more than 1 posted
			allocator->send_alloc_end->next = sbuf;
		allocator->send_alloc_end = sbuf;
#ifdef USE_RC
		sbuf->allocator = allocator;
#ifdef PSM_DEBUG
		// help debug double free
		sbuf->freed_by = "allocated";
#endif
#ifdef PSM_RC_RECONNECT
		if (rc_qp) {
			psmi_assert(allocator == &rc_qp->send_allocator);
			sbuf->rc_qp = rc_qp;
			rc_qp->send_posted++;
		}
#endif /* PSM_RC_RECONNECT */
#endif /* USE_RC */
	}
	return sbuf;
}

/* used for rare can't post to QP error path */
/* only valid immediately after the cooresponding alloc_sbuf */
void psm3_ep_verbs_unalloc_sbuf(psm3_verbs_send_allocator_t allocator,
								 sbuf_t sbuf, sbuf_t prev_sbuf)
{
	psm3_verbs_send_pool_t pool = allocator->pool;
#ifdef USE_RC
	psmi_assert(sbuf->allocator == allocator);
#endif
	_HFI_VDBG("unalloc sbuf %p prev %p sbuf->scb %p\n", sbuf, prev_sbuf, sbuf->scb);
#ifdef USE_RC
#ifdef PSM_RC_RECONNECT
	if (sbuf->rc_qp) {
		psmi_assert(allocator == &sbuf->rc_qp->send_allocator);
		psm3_verbs_dec_posted(sbuf->rc_qp);
		// caller allocated an sbuf, then unalloced, can't be draining
		psmi_assert(!sbuf->rc_qp->draining);
		sbuf->rc_qp = NULL;
	}
#endif
#endif
	if (sbuf->scb) {
		psmi_assert(sbuf->scb->sdma_outstanding);
		sbuf->scb->sdma_outstanding--;
		sbuf->scb = NULL;
	}
	// remove from list of allocated buffers
	psmi_assert(allocator->send_alloc_end == sbuf);
	psmi_assert(sbuf->next == NULL);
	if_pf(allocator->send_alloc_head == sbuf) {
		// only one on allocated list
		//psmi_assert(allocator->send_alloc_end_prev == NULL);
		psmi_assert(prev_sbuf == NULL);
		allocator->send_alloc_head = NULL;
		allocator->send_alloc_end = NULL;
	} else {
		//psmi_assert(allocator->send_alloc_end_prev->next == sbuf);
		//allocator->send_alloc_end = allocator->send_alloc_end_prev;
		//allocator->send_alloc_end_prev->next = NULL;
		psmi_assert(prev_sbuf->next == sbuf);
		allocator->send_alloc_end = prev_sbuf;
		prev_sbuf->next = NULL;
	}
	// put back on head of free list
	sbuf->next = pool->send_free;
	pool->send_free = sbuf;
	pool->send_num_free++;
#ifdef PSM_DEBUG
	// help detect and debug double free
	sbuf->allocator = NULL;
	sbuf->freed_by = "unalloc";
#endif
}

// buffers must be freed in order, the fact the SQ reports completions in
// same order as send WQEs ensures this
// this will free up to count buffers with buf being the last freed
void psm3_ep_verbs_free_sbuf(
#ifndef USE_RC
			psm3_verbs_send_allocator_t allocator,
#endif
			sbuf_t buf, uint32_t count
#ifdef PSM_RC_RECONNECT
			, int can_free_rc_qp
#endif
#ifdef PSM_DEBUG
			// to help debug double free
			, const char *caller
			, const char *caller_mid
#endif
			)
{
#ifdef USE_RC
	psm3_verbs_send_allocator_t allocator = buf->allocator;
#endif
#ifdef PSM_DEBUG
	if (! allocator) {
		_HFI_ERROR("double sbuf free.  caller=%s buf=%p allocator=%p count %u freed_by %s\n",
			caller, buf, allocator, count, buf->freed_by);
#ifdef PSM_RC_RECONNECT
		if (buf->rc_qp)
			_HFI_ERROR("details ipsaddr %p QP %u draining %u posted %u + %u drain outstanding %u \n",
				buf->rc_qp->ipsaddr, buf->rc_qp->qp->qp_num,
				buf->rc_qp->draining, buf->rc_qp->send_posted,
				buf->rc_qp->recv_pool.posted,
				buf->rc_qp->ipsaddr->epaddr.proto->ep->verbs_ep.send_drain_outstanding);
		psmi_assert(allocator);
#endif /* PSM_RC_RECONNECT */
	}
#ifdef PSM_RC_RECONNECT
	if (buf->rc_qp)
		psmi_assert(allocator == &buf->rc_qp->send_allocator);
#endif /* PSM_RC_RECONNECT */
#endif /* PSM_DEBUG */
	psm3_verbs_send_pool_t pool = allocator->pool;
	sbuf_t b;
	do {
		// take 1st off allocated list
		b = allocator->send_alloc_head;
#ifdef PSM_DEBUG
		if (! b) {
			_HFI_ERROR("no buffers on sbuf alloc list.  caller=%s buf=%p allocator=%p count %u\n",
				caller, buf, allocator, count);
#ifdef PSM_RC_RECONNECT
			if (buf->rc_qp)
				_HFI_ERROR("details ipsaddr %p QP %u draining %u posted %u + %u drain outstanding %u \n",
					 buf->rc_qp->ipsaddr,
					 buf->rc_qp->qp->qp_num,
					 buf->rc_qp->draining,
					 buf->rc_qp->send_posted,
					 buf->rc_qp->recv_pool.posted,
					 buf->rc_qp->ipsaddr->epaddr.proto->ep->verbs_ep.send_drain_outstanding);
#endif /* PSM_RC_RECONNECT */
		}
#endif /* PSM_DEBUG */
		psmi_assert(b);
		allocator->send_alloc_head = b->next;
		if_pf (allocator->send_alloc_end == b)	// unlikely last outstanding
			allocator->send_alloc_end = NULL;
#ifdef PSM_RC_RECONNECT
		if (b->rc_qp) {
			psmi_assert(allocator == &b->rc_qp->send_allocator);
			psm3_verbs_dec_posted(b->rc_qp);
			if (can_free_rc_qp)
				psm3_verbs_free_rc_qp_if_drained("SQ WC", b->rc_qp);
			b->rc_qp = NULL;
		}
#endif
		if (b->scb) {
			psmi_assert(b->scb->sdma_outstanding);
			b->scb->sdma_outstanding--;
			_HFI_VDBG("reap SDMA sbuf, scb %p new cnt %d\n", b->scb,
						b->scb->sdma_outstanding);
			b->scb = NULL;
		}
		// put at head of free list
		b->next =  pool->send_free;
		pool->send_free = b;
		pool->send_num_free++;
#ifdef PSM_DEBUG
		// helps to detect and debug double free
		b->allocator = NULL;
		if (b != buf)
			b->freed_by = caller_mid;
		else
			b->freed_by = caller;
#endif /* PSM_DEBUG */
#ifdef UD_DEBUG
		printf("freed: %u num free: %u\n", 
			(uint32_t)send_buffer_index(pool, b->buffer),
			pool->send_num_free);
#endif
	} while (--count && b != buf);
	// normally we will find buf just as we exhaust count (coallesce amount).
	// however when send error CQEs occur (such as flush) we may find less
	// than count inflight ahead of buf
	//psmi_assert_always(b == buf && count == 0);
#ifdef PSM_DEBUG
	if (b != buf) {
		_HFI_ERROR("free loop ended without finding buf, caller=%s buf %p b %p allocator=%p next %p\n",
			caller, buf, b, allocator, allocator->send_alloc_head);
#ifdef PSM_RC_RECONNECT
		if (buf->rc_qp)
			_HFI_ERROR("details for buf ipsaddr %p QP %u draining %u posted %u + %u drain outstanding %u\n",
				buf->rc_qp->ipsaddr, buf->rc_qp->qp->qp_num,
				buf->rc_qp->draining, buf->rc_qp->send_posted,
				buf->rc_qp->recv_pool.posted,
				buf->rc_qp->ipsaddr->epaddr.proto->ep->verbs_ep.send_drain_outstanding);
#endif /* PSM_RC_RECONNECT */
	}
#endif /* PSM_DEBUG */
	psmi_assert_always(b == buf);
}

#ifdef PSM_RC_RECONNECT
// all the sends on the given allocator are now done, free their sbufs
// will not free rc_qp, even if QP now empty
void psm3_ep_verbs_send_drained(psm3_verbs_send_allocator_t allocator)
{
	sbuf_t b;
	// we could just free_sbuf(allocator->send_alloc_end, pool->send_total);
	// but this is more obvious and we don't care about minor perf differences
	// in recovery paths
	while (NULL != (b = allocator->send_alloc_head)) 
		psm3_ep_verbs_free_sbuf(b, 1, 0
#ifdef PSM_DEBUG
				, "send_drained", "send drained"
#endif
				);
}
#endif

psm2_error_t psm3_ep_verbs_post_recv(
#ifndef USE_RC
				psm3_verbs_recv_pool_t pool,
#endif
				rbuf_t buf)
{
#ifdef USE_RC
	psm3_verbs_recv_pool_t pool = buf->pool;
#endif
#if VERBS_RECV_QP_COALLESCE > 1
	struct ibv_recv_wr *wr;
#else
	struct ibv_recv_wr wr;
	struct ibv_sge list;
#endif
	struct ibv_recv_wr *bad_wr;
	int err;

#ifdef USE_RC
#ifdef PSM_RC_RECONNECT
	if (pool->draining) {
		psmi_assert(pool->qp != pool->ep->verbs_ep.qp);// not UD QP pool
		psmi_assert(! pool->for_srq);
		psm3_verbs_free_rc_qp_if_empty("RQ WC",
				psm3_verbs_rc_qp_from_recv_pool(pool));
		return PSM2_OK;
	}
#endif
	// only RC QPs doing just RDMA Write can have a zero buffer size
	if (pool->recv_buffer_size) {
#else
	{
#endif
		uint32_t index = recv_buffer_index(pool, rbuf_to_buffer(buf));
		// make sure its a buffer in our pool
		psmi_assert_always(index < pool->recv_total);
		// assert on index covers these 2 asserts
		//psmi_assert_always(rbuf_to_buffer(buf) >= pool->recv_buffers);
		//psmi_assert_always(rbuf_to_buffer(buf) <= pool->recv_buffers +
		//						 pool->recv_total)*pool->recv_buffer_size);
		// make sure buf is exactly at the start of a buffer in our pool
		psmi_assert_always(rbuf_to_buffer(buf) == &(pool->recv_buffers[recv_buffer_start(pool, index)]));

#if VERBS_RECV_QP_COALLESCE > 1
		// put buf in wr at end of list
		wr = &(pool->recv_wr_list[pool->next_recv_wqe]);
		psmi_assert(wr->sg_list == &(pool->recv_sge_list[pool->next_recv_wqe]));
		wr->sg_list->addr = (uintptr_t)rbuf_to_buffer(buf);
		wr->wr_id = (uintptr_t)buf;	// we'll get this back in completion
#ifdef PSM_FI
		if_pf(PSM3_FAULTINJ_ENABLED_EP(pool->ep)) {
			PSM3_FAULTINJ_STATIC_DECL(fi_rq_lkey, "rq_lkey",
					"post UD "
#ifdef USE_RC
					"or RC or SRQ "
#endif
					"RQ WQE with bad lkey",
					0, IPS_FAULTINJ_RQ_LKEY);
#ifdef PSM_RC_RECONNECT
			PSM3_FAULTINJ_STATIC_DECL(fi_rc_rq_lkey, "rc_rq_lkey",
					"post RC RQ WQE with bad lkey",
					pool->ep->allow_reconnect?1:0,
					IPS_FAULTINJ_RC_RQ_LKEY);
#endif
			// SRQ has no number but need consistency in fmt and number of args
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_rq_lkey, pool->ep,
#ifdef USE_RC
				 "%s %u buf %p", pool->for_srq?"SRQ":"QP",
				 pool->for_srq?0:pool->qp->qp_num, buf))
#else
				 " QP %u buf %p", pool->qp->qp_num, buf))
#endif
				wr->sg_list->lkey = IPS_BAD_LKEY;
#ifdef PSM_RC_RECONNECT
			else if_pf(! pool->for_srq && pool->ep->verbs_ep.qp != pool->qp
				&& PSM3_FAULTINJ_IS_FAULT(fi_rc_rq_lkey, pool->ep,
				 " RC QP %u buf %p", pool->qp->qp_num, buf))
				wr->sg_list->lkey = IPS_BAD_LKEY;
#endif
			else
				wr->sg_list->lkey = pool->recv_buffer_mr->lkey;
		}
#endif // PSM_FI
		if_pf (++pool->next_recv_wqe >= VERBS_RECV_QP_COALLESCE) {
			// we have a batch ready to post
#ifdef USE_RC
			if (pool->for_srq) {
				err = ibv_post_srq_recv(pool->srq, pool->recv_wr_list, &bad_wr);
				if_pf (err) {
					_HFI_ERROR("failed to post SRQ on %s port %u: %s",
						pool->ep->dev_name, pool->ep->portnum, strerror(err));
					return PSM2_INTERNAL_ERR;
				}
				//_HFI_VDBG("posted SRQ, including buffer %u\n", index);
			} else
#endif
			{
				err = ibv_post_recv(pool->qp, pool->recv_wr_list, &bad_wr);
				if_pf (err) {
					_HFI_ERROR("failed to post RQ on %s port %u: %s",
						pool->ep->dev_name, pool->ep->portnum, strerror(err));
					return PSM2_INTERNAL_ERR;
				}
#ifdef PSM_RC_RECONNECT
				pool->posted += pool->next_recv_wqe;
#endif
				//_HFI_VDBG("posted RQ, including buffer %u\n", index);
			}
			pool->next_recv_wqe = 0;
		} else {
			//_HFI_VDBG("preped RQE, buffer %u\n", index);
		}
#else /* VERBS_RECV_QP_COALLESCE > 1 */
		list.addr = (uintptr_t)rbuf_to_buffer(buf);
		list.length = pool->recv_buffer_size;
		list.lkey = pool->recv_buffer_mr->lkey;
#ifdef PSM_FI
		if_pf(PSM3_FAULTINJ_ENABLED_EP(pool->ep)) {
			PSM3_FAULTINJ_STATIC_DECL(fi_rq_lkey, "rq_lkey",
					"post UD "
#ifdef USE_RC
					"or RC or SRQ"
#endif
					"RQ WQE with bad lkey",
					0, IPS_FAULTINJ_RQ_LKEY);
#ifdef PSM_RC_RECONNECT
			PSM3_FAULTINJ_STATIC_DECL(fi_rc_rq_lkey, "rc_rq_lkey",
					"post RC RQ WQE with bad lkey",
					pool->ep->allow_reconnect?1:0,
					IPS_FAULTINJ_RC_RQ_LKEY);
#endif
			// SRQ has no number but need consistency in fmt and number of args
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_rq_lkey, pool->ep,
#ifdef USE_RC
				 "%s %u buf %p", pool->for_srq?"SRQ":"QP",
				 pool->for_srq?0:pool->qp->qp_num, buf))
#else
				 " QP %u buf %p", pool->qp->qp_num, buf))
#endif
				list.lkey = IPS_BAD_LKEY;
#ifdef PSM_RC_RECONNECT
			else if_pf(! pool->for_srq && pool->ep->verbs_ep.qp != pool->qp
				&& PSM3_FAULTINJ_IS_FAULT(fi_rc_rq_lkey, pool->ep,
				 " RC QP %u buf %p", pool->qp->qp_num, buf))
				list.lkey = IPS_BAD_LKEY;
#endif
		}
#endif // PSM_FI
		wr.next = NULL;	// just post 1
		wr.wr_id = (uintptr_t)buf;	// we'll get this back in completion
		wr.sg_list = &list;
		wr.num_sge = 1;	// size of sg_list

#ifdef USE_RC
		if (pool->for_srq) {
			err = ibv_post_srq_recv(pool->srq, &wr, &bad_wr);
			if_pf (err) {
				_HFI_ERROR("failed to post SRQ on %s port %u: %s",
					pool->ep->dev_name, pool->ep->portnum, strerror(err));
				return PSM2_INTERNAL_ERR;
			}
			//_HFI_VDBG("posted SRQ, buffer %u\n", index);
		} else
#endif
		{
			err = ibv_post_recv(pool->qp, &wr, &bad_wr);
			if_pf (err) {
				_HFI_ERROR("failed to post RQ on %s port %u: %s",
					pool->ep->dev_name, pool->ep->portnum, strerror(err));
				return PSM2_INTERNAL_ERR;
			}
#ifdef PSM_RC_RECONNECT
			pool->posted++;
#endif
			//_HFI_VDBG("posted RQ, buffer %u\n", index);
		}
#endif /* VERBS_RECV_QP_COALLESCE > 1 */
#ifdef USE_RC
	} else {
#if VERBS_RECV_QP_COALLESCE > 1
		// put buf in wr at end of list
		wr = &(pool->recv_wr_list[pool->next_recv_wqe]);
		psmi_assert(wr->sg_list == NULL);
		wr->wr_id = (uintptr_t)buf;	// we'll get this back in completion
		if_pf (++pool->next_recv_wqe >= VERBS_RECV_QP_COALLESCE) {
			// we have a batch ready to post
			if (pool->for_srq) {
				err = ibv_post_srq_recv(pool->srq, pool->recv_wr_list, &bad_wr);
				if_pf (err) {
					_HFI_ERROR("failed to post SRQ on %s port %u: %s",
						pool->ep->dev_name, pool->ep->portnum, strerror(err));
					return PSM2_INTERNAL_ERR;
				}
				//_HFI_VDBG("posted SRQ\n");
			} else {
				err = ibv_post_recv(pool->qp, pool->recv_wr_list, &bad_wr);
				if_pf (err) {
					_HFI_ERROR("failed to post RQ on %s on port %u: %s",
						pool->ep->dev_name, pool->ep->portnum, strerror(err));
					return PSM2_INTERNAL_ERR;
				}
#ifdef PSM_RC_RECONNECT
				pool->posted += pool->next_recv_wqe;
#endif
				//_HFI_VDBG("posted RQ\n");
			}
			pool->next_recv_wqe = 0;
		} else {
			//_HFI_VDBG("preped RQE\n");
		}
#else /* VERBS_RECV_QP_COALLESCE > 1 */
		wr.next = NULL;	// just post 1
		wr.wr_id = (uintptr_t)buf;	// we'll get this back in completion
		wr.sg_list = NULL;
		wr.num_sge = 0;	// size of sg_list

		if (pool->for_srq) {
			err = ibv_post_srq_recv(pool->srq, &wr, &bad_wr);
			if_pf (err) {
				_HFI_ERROR("failed to post SRQ on %s port %u: %s",
					pool->ep->dev_name, pool->ep->portnum, strerror(err));
				return PSM2_INTERNAL_ERR;
			}
			//_HFI_VDBG("posted SRQ\n");
		} else {
			err = ibv_post_recv(pool->qp, &wr, &bad_wr);
			if_pf (err) {
				_HFI_ERROR("failed to post RQ on %s on port %u: %s",
					pool->ep->dev_name, pool->ep->portnum, strerror(err));
				return PSM2_INTERNAL_ERR;
			}
#ifdef PSM_RC_RECONNECT
			pool->posted++;
#endif
			//_HFI_VDBG("posted RQ\n");
		}
#endif /* VERBS_RECV_QP_COALLESCE > 1 */
#endif // USE_RC
	}
	return PSM2_OK;
}

psm2_error_t psm3_ep_verbs_prepost_recv(
						psm3_verbs_recv_pool_t pool)
{
	int i;

	if (! pool->recv_total)
		return PSM2_INTERNAL_ERR;
#ifdef PSM_RC_RECONNECT
	psmi_assert(! pool->draining);
#endif
	// prepare RQ
	for (i=0; i< pool->recv_total; i++) {
#ifdef USE_RC
		rbuf_t buf = &(pool->recv_bufs[i]);
		if (pool->recv_buffer_size)
			buf = &(pool->recv_bufs[i]);
		else
			buf = pool->recv_bufs;	// only 1, just to find pool and qp
#else
		uint8_t *buf = &(pool->recv_buffers[recv_buffer_start(pool, i)]);
#endif
		if (PSM2_OK != psm3_ep_verbs_post_recv(
#ifndef USE_RC
							pool,
#endif
							buf)) {
			_HFI_ERROR( "Unable to post RQ on %s port %u\n", pool->ep->dev_name, pool->ep->portnum);
			return PSM2_INTERNAL_ERR;
		}
	}
	return PSM2_OK;
}

// only used when PSM3_RDMA enabled
psm2_error_t psm3_verbs_post_rdma_write_immed(psm2_ep_t ep,
#ifdef PSM_RC_RECONNECT
				struct psm3_verbs_rc_qp *rc_qp,
#else
				struct ibv_qp *qp,
#endif
				void *loc_buf, struct psm3_verbs_mr *loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id)
{
	struct ibv_send_wr wr;
	struct ibv_send_wr *bad_wr;
	struct ibv_sge list;
	psm2_error_t ret = PSM2_OK;
#ifdef PSM_RC_RECONNECT
	struct ibv_qp *qp = rc_qp->qp;
#endif
	int err;

	//printf("XXXX %s 0x%p %ld 0x%x\n", __FUNCTION__, loc_buf, len, loc_mr->lkey);
	psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode));

	list.addr = loc_mr->iova +
		   ((uintptr_t)loc_buf - (uintptr_t)loc_mr->addr);
	list.length = len;
	list.lkey = loc_mr->lkey;
	wr.next = NULL; // just post 1
	psmi_assert(! VERBS_SQ_WR_OP(wr_id));
	wr.wr_id = wr_id | VERBS_SQ_WR_ID_RDMA_WRITE;
#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_rc_rdma_lkey, "rc_rdma_lkey",
				"post RC RDMA Write WQE with bad lkey",
				ep->allow_reconnect?1:0,
				IPS_FAULTINJ_RC_RDMA_LKEY);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_rc_rdma_lkey, ep,
				" RC QP %u wr_id %p len %u", qp->qp_num,
				(void*)wr.wr_id, list.length))
			list.lkey = IPS_BAD_LKEY;
	}
#endif // PSM_FI
	wr.sg_list = &list;
	wr.num_sge = 1; // size of sg_list
	wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
	wr.imm_data = immed;
	wr.wr.rdma.remote_addr = rem_buf;
	wr.wr.rdma.rkey = rkey;
#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_rc_rdma_rkey, "rc_rdma_rkey",
				"post RC RDMA Write WQE with bad rkey",
				ep->allow_reconnect?1:0,
				IPS_FAULTINJ_RC_RDMA_RKEY);
		// no use injecting rkey faults if already injected bad lkey
		if_pf(list.lkey != IPS_BAD_LKEY
		    && PSM3_FAULTINJ_IS_FAULT(fi_rc_rdma_rkey, ep,
			" RC QP %u wr_id %p len %u", qp->qp_num,
			(void*)wr.wr_id, list.length))
			wr.wr.rdma.rkey = IPS_BAD_RKEY;
	}
#endif // PSM_FI
	// RDMA Writes will tend to be larger and we want the completion
	// to reflect the RDMA for a given CTS is completed
	wr.send_flags = IBV_SEND_SIGNALED;  // get a completion
	// no need for wr.send_flags |= IBV_SEND_SOLICITED
	// these will be bigger sends, no need for inline
	ep->verbs_ep.send_rdma_outstanding++;
	err = ibv_post_send(qp, &wr, &bad_wr);
	if_pf (err) {
		if (err != EBUSY && err != EAGAIN && err != ENOMEM)
			_HFI_ERROR("failed to post RC SQ on %s port %u: %s",
				ep->dev_name, ep->portnum, strerror(err));
		// caller will try again later when next send buffer freed
		// or timer expires
		ret = PSM2_TIMEOUT;
		ep->verbs_ep.send_rdma_outstanding--;
		goto done;
	}
#ifdef PSM_RC_RECONNECT
	rc_qp->send_posted++;
#endif
	_HFI_VDBG("posted RDMA Write: from 0x%"PRIx64" to 0x%"PRIx64" len %u rkey 0x%x\n",
		list.addr,  wr.wr.rdma.remote_addr, list.length,  wr.wr.rdma.rkey /* TBD rem QPN */ );
#if 0
	// we will not have many in flight at a time so
	// normal progress calls should be sufficient
	// no need to reap completions here
	err = psm3_verbs_completion_update(ep, 0);
	if_pf (err != PSM2_OK)
		return err;
#endif
done:
	//printf("XXXX %s ret:%d\n", __FUNCTION__, ret);
	return ret;
}

#ifdef RNDV_MOD
psm2_error_t psm3_verbs_post_rv_rdma_write_immed(psm2_ep_t ep,
				psm3_rv_conn_t conn,
				void *loc_buf, struct psm3_verbs_mr *loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id,
				uint8_t *sconn_index, uint32_t *conn_count)
{
	psm2_error_t ret = PSM2_OK;

	//printf("XXXX %s 0x%p %ld 0x%x\n", __FUNCTION__, loc_buf, len, loc_mr->lkey);
	psmi_assert(IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode));

	ep->verbs_ep.send_rdma_outstanding++;
#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_rv_rdma_len, "rv_rdma_len",
				"post RV RDMA Write with bad len (may want RV build with RNDV_LOCAL_ERR_TEST)",
				0, IPS_FAULTINJ_RV_RDMA_LEN);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_rv_rdma_len, ep, ""))
			len += 1000000000;
	}
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_rv_rdma_rkey, "rv_rdma_rkey",
				"post RV RDMA Write with bad rkey",
				1, IPS_FAULTINJ_RV_RDMA_RKEY);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_rv_rdma_rkey, ep, ""))
			rkey = IPS_BAD_RKEY;
	}
#endif // PSM_FI
	if (psm3_rv_post_rdma_write_immed(ep->rv, conn,
                loc_buf, loc_mr->mr.rv_mr,
                rem_buf, rkey,
                len, immed, wr_id, sconn_index, conn_count)) {
		switch (errno) {
		case EIO:
			// lost or failed connection
			ret = PSM2_EPID_RC_CONNECT_ERROR;
			break;
		case EAGAIN:
			// lost connection and are recoverying it
			ret = PSM2_EPID_RV_CONNECT_RECOVERING;
			break;
		case ENOMEM:
		case EBUSY:
			// caller will try again later when next send buffer freed
			// or timer expires
			ret = PSM2_TIMEOUT;
			break;
		default:
			ret = PSM2_INTERNAL_ERR;
			break;
		}
		if (errno != EBUSY && errno != EAGAIN && errno != ENOMEM) {
			_HFI_ERROR("failed to post RV RC SQ on %s port %u: %s",
					ep->dev_name, ep->portnum, strerror(errno));
			psmi_assert_always(errno != EINVAL);
		}
		ep->verbs_ep.send_rdma_outstanding--;
		goto done;
	}
	_HFI_VDBG("posted RV RDMA Write: from 0x%"PRIx64" to 0x%"PRIx64" len %u rkey 0x%x\n",
		(uint64_t)loc_buf,  rem_buf, (unsigned)len,  rkey /* TBD rem QPN */ );
done:
	//printf("XXXX %s ret:%d\n", __FUNCTION__, ret);
	return ret;
}
#endif // RNDV_MOD

#ifdef USE_RC
extern int ips_protoexp_rdma_write_completion( uint64_t wr_id);
#ifdef PSM_RC_RECONNECT
struct psm3_verbs_rc_qp *ips_protoexp_rdma_write_completion_rc_qp(psm2_ep_t ep,
						uint64_t wr_id);
int ips_protoexp_rdma_write_completion_error(psm2_ep_t ep, uint64_t wr_id,
						enum ibv_wc_status wc_status);
#endif /* PSM_RC_RECONNECT */
#endif /* USE_RC */

// process a send WC with an error completion which was returned by ibv_pollcq
// When user RC QP reconnection is allowed, failures can be recovered
// from by replacing the RC QP and reestablishing the RC connection.
// When not allowed or for errors on UD QPs, the error is unrecoverable.
// For RDMA write completions, the wr_id is tidsendc and tidsendc tracks the
// rc_qp.  This assumes a given tidsendc has exactly 1 RDMA Write posted.
static void psm3_verbs_send_wc_error(psm2_ep_t ep, struct ibv_wc *wc)
{
#ifdef PSM_RC_RECONNECT
	struct psm3_verbs_rc_qp *rc_qp;
	ips_epaddr_t *ipsaddr;
#endif
	// this function only called for WCs with error
	psmi_assert(wc->status);

	// only the following fields in wc are valid:
	//	wr_id, status, vendor_err, qp_num

	// on UD QPs or when reconnect not allowed WC errors are unrecoverable
#ifdef USE_RC
	if (! ep->allow_reconnect || psm3_verbs_is_ud_qp_num(ep, wc->qp_num)) {
#else
	{
#endif
		// only report the 1st error (non-flush), it will
		// be followed by a burst of FLUSH_ERR for other queued WQEs
		if (wc->status != IBV_WC_WR_FLUSH_ERR)
			_HFI_ERROR("failed %s on %s port %u epid %s status: '%s' (%d) QP %u\n",
				VERBS_SQ_WR_OP_STR(wc->wr_id),
				ep->dev_name, ep->portnum,
				psm3_epid_fmt_internal(ep->epid, 0),
				ibv_wc_status_str(wc->status),
				(int)wc->status, wc->qp_num);
		// QP is now in QPS_ERR and can't send/recv packets
		// Upcoming async event will cause fatal error
#ifdef PSM_RC_RECONNECT
		psmi_assert(VERBS_SQ_WR_OP(wc->wr_id) != VERBS_SQ_WR_ID_DRAIN_MARKER);
#endif
		if (VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_SEND) {
			psm3_ep_verbs_free_sbuf(
#ifndef USE_RC
				&ep->verbs_ep.send_allocator,
#endif
				(sbuf_t)VERBS_SQ_WR_PTR(wc->wr_id),
				VERBS_SEND_CQ_COALLESCE
#ifdef PSM_RC_RECONNECT
				, 0
#endif
#ifdef PSM_DEBUG
				, "UD WC err", "UD WC err mid"
#endif
				);
		} else {
#ifdef USE_RC
			psmi_assert(VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_RDMA_WRITE);
			// no need to dec rdma counts and call
			// ips_protoexp_rdma_write_completion_error since
			// async event will cause fatal error
			// Note: we could also have some successful sbuf sends
			// which didn't ask for a completion.
			psmi_assert(! psm3_verbs_is_ud_qp_num(ep, wc->qp_num));
#else
			psmi_assert(0);	// should not happen
#endif
		}
		return;
	}
#if defined(USE_RC) && ! defined(PSM_RC_RECONNECT)
	psmi_assert(0); // should not get here, allow_reconnect always false
	return;
#endif
#ifdef PSM_RC_RECONNECT
	// For user space RC QP, the QP is now in QPS_ERR and we
	// need to replace and reconnect it.
	// we will ultimately get an ack timeout for affected packets & retry
	psmi_assert(! psm3_verbs_is_ud_qp_num(ep, wc->qp_num));
	if (VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_SEND) {
		sbuf_t sbuf = (sbuf_t)VERBS_SQ_WR_PTR(wc->wr_id);
		rc_qp = sbuf->rc_qp;
		psm3_ep_verbs_free_sbuf(sbuf, VERBS_SEND_CQ_COALLESCE, 0
#ifdef PSM_DEBUG
			, "RC WC Err", "RC WC Err mid"
#endif
			);
	} else if (VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_RDMA_WRITE) {
		ep->verbs_ep.send_rdma_outstanding--;
		rc_qp = ips_protoexp_rdma_write_completion_rc_qp(ep, 
				VERBS_SQ_WR_PTR(wc->wr_id));
		if (ips_protoexp_rdma_write_completion_error(ep,
				VERBS_SQ_WR_PTR(wc->wr_id), wc->status))
			goto rdma_fatal;
	} else {
		psmi_assert(VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_DRAIN_MARKER);
		rc_qp = (struct psm3_verbs_rc_qp *)VERBS_SQ_WR_PTR(wc->wr_id);
		psmi_assert(rc_qp->draining);
		psmi_assert(rc_qp->send_posted);
		psm3_ep_verbs_send_drained(&rc_qp->send_allocator);
		psm3_verbs_dec_posted(rc_qp);	// for marker itself
		psmi_assert(! rc_qp->send_posted);	// SQ now empty
	}
	psmi_assert(rc_qp);
	psmi_assert(rc_qp->qp->qp_num == wc->qp_num);
	ipsaddr = rc_qp->ipsaddr;
	psmi_assert(ipsaddr);
	psmi_assert(ipsaddr == (ips_epaddr_t *)rc_qp->qp->qp_context);

	// treat a FLUSH or other error similar (except for logging)
	// Depending on order of polling, may detect the QP issue via an
	// SQ FLUSH CQE prior to detecting the RQ error CQE, in which case
	// we want to still start the reconnection protocol ASAP (via
	// psm3_ips_proto_connection_error)

	// log 1st observed issue on QP and all non-flush error CQEs.
	// We skip logging of other flush CQEs since there can be many.
	if ((_HFI_CONNDBG_ON || _HFI_DBG_ON)
	    && (wc->status != IBV_WC_WR_FLUSH_ERR || ! rc_qp->draining)) {
		_HFI_DBG_ALWAYS("[ipsaddr %p] failed %s wr_id %p on %s port %u epid %s RC QP %u rcnt %u posted %u + %u status: '%s' (%d)\n",
			ipsaddr, VERBS_SQ_WR_OP_STR(wc->wr_id),
			(void *)(wc->wr_id),
			ep->dev_name, ep->portnum,
			psm3_epid_fmt_internal(ep->epid, 0), wc->qp_num,
			rc_qp->reconnect_count,
			rc_qp->send_posted, rc_qp->recv_pool.posted,
			ibv_wc_status_str(wc->status), (int)wc->status);
	}
	if (wc->status != IBV_WC_WR_FLUSH_ERR) {
		if (VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_SEND)
			ipsaddr->epaddr.proto->epaddr_stats.send_wc_error++;
		else if (VERBS_SQ_WR_OP(wc->wr_id) == VERBS_SQ_WR_ID_RDMA_WRITE)
			ipsaddr->epaddr.proto->epaddr_stats.rdma_wc_error++;
		else
			ipsaddr->epaddr.proto->epaddr_stats.drain_wc_error++;
	}

	// only call connect_error for 1st error on QP, if already
	// draining due to a separate send error, recv error or
	// reconnect packet don't call connection_error again.
	// psm3_ips_proto_connection_error tests ipsaddr->allow_reconnect
	// and reports fatal error if reconnect not allowed
	if (! ipsaddr->allow_reconnect || ! rc_qp->draining) {
		// for return != no progress, rc_qp may have been freed
		// fatal error in connection_error prior to other error returns
		if (PSM2_OK_NO_PROGRESS == psm3_ips_proto_connection_error(
				ipsaddr, VERBS_SQ_WR_OP_STR(wc->wr_id),
				ibv_wc_status_str(wc->status), wc->status, 0))
			psm3_verbs_free_rc_qp_if_empty("SQ WC Err", rc_qp);
	} else
		psm3_verbs_free_rc_qp_if_empty("SQ WC Err", rc_qp);
	return;

rdma_fatal:
	// RDMA WC unrecoverable
	// we either have a bug which yielded a NULL wr_id
	// or we are not enabling reconnection and RDMA retry
	psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
		"failed RDMA Write '%s' (%d) on %s port %u epid %s\n",
		ibv_wc_status_str(wc->status), (int)wc->status,
		ep->dev_name, ep->portnum,
		psm3_epid_fmt_internal(ep->epid, 0));
	return;
#endif /* PSM_RC_RECONNECT */
}

// we structure this similar to psm3_gen1_dma_completion_update
// this is non-blocking.  We reap what's available and then return
// when drain=1 we do an unconditional reap, this is used in error handling
// to wait for a DMA on the send QP to complete before retrying it due to
// ack timeout.
psm2_error_t
psm3_verbs_completion_update(psm2_ep_t ep, int drain)
{
	#define CQE_BATCH 32	// reap a few at a time, hopefully faster this way
	//#define CQE_BATCH 8 or 18	// reap a few at a time, hopefully faster this way
							// 18*COALLESE > default reap threshold so we
							// should get away with one poll_q
							// not sure if doing the exact math here would
							// add clocks and hurt a bit more than approx math
							// int batch = (send_reap_thresh/COALLESCE) + 2
							// alloca(sizeof(ibv_wc) & batch)
	struct ibv_wc wc[CQE_BATCH];
	int ne;

	PSMI_LOCK_ASSERT(ep->mq->progress_lock);
	// TBD - when coallescing completions we'll tend to fall through to poll_cq
	// this only called when out of buffers or immediately after posting a send
	// reduce the frequency of poll_cq by only checking once we have at least
	// send_reap_thresh sends in flight
	// for USE_RC this is imperfect, we can have a handful of unsignaled
	// send WQEs on multiple RC QPs, in which case we may exceed the
	// reap_thresh but not find any CQEs until we post more sends and
	// hit the coalsce threshold.
	if_pt (!drain && (! ep->verbs_ep.send_rdma_outstanding
				 || IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode))
		   && ep->verbs_ep.send_pool.send_num_free > ep->verbs_ep.send_pool.send_total - ep->verbs_ep.send_reap_thresh
#ifdef PSM_RC_RECONNECT
		   && ! ep->verbs_ep.send_drain_outstanding
#endif
		   )
		return PSM2_OK;	// not ready to reap, return quickly
#ifdef PSM_DEBUG
	// detect recursive calls to completion_update()
	psmi_assert(! ep->verbs_ep.in_completion_update);
	ep->verbs_ep.in_completion_update = 1;
#endif

	//if ( 0 != (ne = ibv_poll_cq(ep->verbs_ep.send_cq, CQE_BATCH, wc)))
	while ( 0 != (ne = ibv_poll_cq(ep->verbs_ep.send_cq, CQE_BATCH, wc)))
	{
		unsigned i;
		for (i=0; i<ne; i++) {
			_HFI_VDBG("poll_cq send WC: QP %u wr_id: %p (%s) status: '%s' (%d)\n",
				wc[i].qp_num, (void*)wc[i].wr_id,
				VERBS_SQ_WR_OP_STR(wc[i].wr_id),
				ibv_wc_status_str(wc[i].status), wc[i].status);
			psmi_assert_always(wc[i].wr_id);
			if_pf (wc[i].status) {
				psm3_verbs_send_wc_error(ep, &wc[i]);
				continue;
			}
#ifdef PSM_RC_RECONNECT
			psmi_assert(VERBS_SQ_WR_OP(wc[i].wr_id) != VERBS_SQ_WR_ID_DRAIN_MARKER);
#endif
			switch (wc[i].opcode) {
			case IBV_WC_SEND:
				// UD sends just mean it got onto the wire and can reuse our buf
				// no guarantees it made it to the remote side
				// buffer address is in wc.wr_id
				psmi_assert(VERBS_SQ_WR_OP(wc[i].wr_id) == VERBS_SQ_WR_ID_SEND);
				_HFI_VDBG("send done (%u bytes) sbuf index %lu\n", wc[i].byte_len,
					send_buffer_index(&ep->verbs_ep.send_pool, sbuf_to_buffer((sbuf_t)VERBS_SQ_WR_PTR(wc[i].wr_id))));
				psm3_ep_verbs_free_sbuf(
#ifndef USE_RC
							&ep->verbs_ep.send_allocator,
#endif
							(sbuf_t)VERBS_SQ_WR_PTR(wc[i].wr_id),
							VERBS_SEND_CQ_COALLESCE
#ifdef PSM_RC_RECONNECT
							, 1
#endif
#ifdef PSM_DEBUG
							, psm3_verbs_is_ud_qp_num(ep, wc->qp_num)?
								"UD WC":"RC WC"
							, psm3_verbs_is_ud_qp_num(ep, wc->qp_num)?
								"UD WC mid":"RC WC mid"
#endif
							);
				break;
#ifdef USE_RC
			case IBV_WC_RDMA_WRITE:
				psmi_assert(VERBS_SQ_WR_OP(wc[i].wr_id) == VERBS_SQ_WR_ID_RDMA_WRITE);
				ep->verbs_ep.send_rdma_outstanding--;
				ips_protoexp_rdma_write_completion(
						 VERBS_SQ_WR_PTR(wc[i].wr_id));
				break;
#ifdef USE_RDMA_READ
			case IBV_WC_RDMA_READ:
			{
				struct ips_epaddr *ipsaddr = (struct ips_epaddr *)wc[i].wr_id;

				ipsaddr->verbs.remote_seq_outstanding = 0;
				_HFI_VDBG("Got remote_recv_psn=%d\n", ipsaddr->verbs.remote_recv_psn);
				break;
			}
#endif // USE_RDMA_READ
#endif // USE_RC
			default:
				_HFI_ERROR("unexpected send completion on %s port %u opcode %u QP %u\n",
							ep->dev_name, ep->portnum,
							wc[i].opcode, wc[i].qp_num);
				break;
			}
		}
#if 0
		// this is optional, especially if use "if" above instead of while
		if (ne <CQE_BATCH)
			break;	// we got less than we asked, we are fast enought that
					// there probably aren't any more on CQE, so just let our
					// next pass reap any that appear while we were processing
#endif
	}
#ifdef PSM_DEBUG
	// detect recursive calls to completion_update()
	ep->verbs_ep.in_completion_update = 0;
#endif
	return PSM2_OK;
}

int verbs_get_port_index2pkey(psm2_ep_t ep, int index)
{
	__be16 pkey;
	int err;

	psmi_assert_always(ep->verbs_ep.context);
	err = ibv_query_pkey(ep->verbs_ep.context, ep->portnum, index, &pkey);
	if (err) {
		_HFI_ERROR("Can't query pkey index %d on %s port %u: %s\n",
			index, ep->dev_name, ep->portnum, strerror(err));
		return -1;
	}
	_HFI_PRDBG("got pkey 0x%x on %s port %u\n", __be16_to_cpu(pkey), ep->dev_name, ep->portnum);
	return __be16_to_cpu(pkey);
}

#ifdef RNDV_MOD
// accessor functions for cm statistics
#define EP_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		psm2_ep_t ep = (psm2_ep_t)context; \
		return ep->stat; \
	}

EP_STAT_FUNC(rv_q_depth, verbs_ep.rv_q_depth)
EP_STAT_FUNC(rv_reconnect_timeout, reconnect_timeout)
EP_STAT_FUNC(rv_hb_interval, verbs_ep.rv_hb_interval)
#undef EP_STAT_FUNC

static uint64_t rv_index(void *context)
{
	struct psm3_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep;
	return vep->rv_index;
}

static uint64_t rv_conn_flags(void *context)
{
	psm2_ep_t ep = (psm2_ep_t)context;
	struct psm3_verbs_ep *vep = &ep->verbs_ep;

	if (ep->rv) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		// we get aggregated values instead of per conn
		(void)psm3_rv_get_conn_stats(ep->rv, NULL, 0, &vep->rv_conn_stats);
	}
	return vep->rv_conn_stats.flags;
}

#define RV_CM_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		struct psm3_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep; \
		return vep->rv_conn_stats.stat; \
	}

RV_CM_STAT_FUNC(rv_conn_num_conn, num_conn)
RV_CM_STAT_FUNC(rv_conn_req_error, req_error)
RV_CM_STAT_FUNC(rv_conn_req_recv, req_recv)
RV_CM_STAT_FUNC(rv_conn_rep_error, rep_error)
RV_CM_STAT_FUNC(rv_conn_rep_recv, rep_recv)
RV_CM_STAT_FUNC(rv_conn_rtu_recv, rtu_recv)
RV_CM_STAT_FUNC(rv_conn_established, established)
RV_CM_STAT_FUNC(rv_conn_dreq_error, dreq_error)
RV_CM_STAT_FUNC(rv_conn_dreq_recv, dreq_recv)
RV_CM_STAT_FUNC(rv_conn_drep_recv, drep_recv)
RV_CM_STAT_FUNC(rv_conn_timewait, timewait)
RV_CM_STAT_FUNC(rv_conn_mra_recv, mra_recv)
RV_CM_STAT_FUNC(rv_conn_rej_recv, rej_recv)
RV_CM_STAT_FUNC(rv_conn_lap_error, lap_error)
RV_CM_STAT_FUNC(rv_conn_lap_recv, lap_recv)
RV_CM_STAT_FUNC(rv_conn_apr_recv, apr_recv)
RV_CM_STAT_FUNC(rv_conn_unexp_event, unexp_event)
RV_CM_STAT_FUNC(rv_conn_req_sent, req_sent)
RV_CM_STAT_FUNC(rv_conn_rep_sent, rep_sent)
RV_CM_STAT_FUNC(rv_conn_rtu_sent, rtu_sent)
RV_CM_STAT_FUNC(rv_conn_rej_sent, rej_sent)
RV_CM_STAT_FUNC(rv_conn_dreq_sent, dreq_sent)
RV_CM_STAT_FUNC(rv_conn_drep_sent, drep_sent)
//RV_CM_STAT_FUNC(rv_conn_wait_time, wait_time)
//RV_CM_STAT_FUNC(rv_conn_resolve_time, resolve_time)
//RV_CM_STAT_FUNC(rv_conn_connect_time, connect_time)
//RV_CM_STAT_FUNC(rv_conn_connected_time, connected_time)
RV_CM_STAT_FUNC(rv_conn_resolve, resolve)
RV_CM_STAT_FUNC(rv_conn_resolve_fail, resolve_fail)
RV_CM_STAT_FUNC(rv_conn_conn_recovery, conn_recovery)
//RV_CM_STAT_FUNC(rv_conn_rewait_time, rewait_time)
//RV_CM_STAT_FUNC(rv_conn_reresolve_time, reresolve_time)
//RV_CM_STAT_FUNC(rv_conn_reconnect_time, reconnect_time)
//RV_CM_STAT_FUNC(rv_conn_max_rewait_time, max_rewait_time)
//RV_CM_STAT_FUNC(rv_conn_max_reresolve_time, max_reresolve_time)
//RV_CM_STAT_FUNC(rv_conn_max_reconnect_time, max_reconnect_time)
RV_CM_STAT_FUNC(rv_conn_reresolve, reresolve)
RV_CM_STAT_FUNC(rv_conn_reresolve_fail, reresolve_fail)
//RV_CM_STAT_FUNC(rv_conn_post_write, post_write)
//RV_CM_STAT_FUNC(rv_conn_post_write_fail, post_write_fail)
//RV_CM_STAT_FUNC(rv_conn_post_write_bytes, post_write_bytes)
RV_CM_STAT_FUNC(rv_conn_outstand_send_write, outstand_send_write)
//RV_CM_STAT_FUNC(rv_conn_send_write_cqe, send_write_cqe)
//RV_CM_STAT_FUNC(rv_conn_send_write_cqe_fail, send_write_cqe_fail)
//RV_CM_STAT_FUNC(rv_conn_recv_write_cqe, recv_write_cqe)
//RV_CM_STAT_FUNC(rv_conn_recv_write_bytes, recv_write_bytes)
//RV_CM_STAT_FUNC(rv_conn_recv_cqe_fail, recv_cqe_fail)
//RV_CM_STAT_FUNC(rv_conn_post_hb, post_hb)
//RV_CM_STAT_FUNC(rv_conn_post_hb_fail, post_hb_fail)
//RV_CM_STAT_FUNC(rv_conn_send_hb_cqe, send_hb_cqe)
//RV_CM_STAT_FUNC(rv_conn_send_hb_cqe_fail, send_hb_cqe_fail)
//RV_CM_STAT_FUNC(rv_conn_recv_hb_cqe, recv_hb_cqe)
#undef RV_CM_STAT_FUNC

static void register_rv_conn_stats(psm2_ep_t ep)
{
	struct psm3_rv_conn_stats *ep_rv_conn_stats = &ep->verbs_ep.rv_conn_stats;

	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL_HELP("RV Configuration Settings:"),
		PSMI_STATS_DECL("rv_q_depth",
				"Size of QPs and CQs per RV QP",
				MPSPAWN_STATS_REDUCTION_ALL,
				rv_q_depth, NULL),
		PSMI_STATS_DECL("rv_reconnect_timeout",
				"RV End-point minimum re-connection timeout in seconds",
				 MPSPAWN_STATS_REDUCTION_ALL,
				rv_reconnect_timeout, NULL),
		PSMI_STATS_DECL("rv_hb_interval",
				"RV End-point heartbeat interval in milliseconds",
				MPSPAWN_STATS_REDUCTION_ALL,
				rv_hb_interval, NULL),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV Connection Statistics:"),
		PSMI_STATS_DECL("rv_index",
				"RV unique end point index within job on given NIC",
				MPSPAWN_STATS_REDUCTION_ALL,
				rv_index, NULL),
		PSMI_STATS_DECL("rv_conn_flags",
				"RV_CONN_STAT flags (1=server, 2=client, 4=was connected)",
				MPSPAWN_STATS_REDUCTION_ALL,
				rv_conn_flags, NULL),
		PSMI_STATS_DECL_FUNC("num_conn",
				"RV Total QPs shared with this process",
				rv_conn_num_conn),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV CM Total Event Counts:\n"
			"The CM connection protocol uses a Request (REQ), "
			"Reply (REP) and Ready-to-Use (RTU).  If data arrives "
			"prior to the RTU, ib_cm_notify can report connection "
			"established.\n"
			"Connection attempts may be Rejected (REJ).\n"
			"The CM disconnect protocol uses a Request (DREQ) "
			"and Reply (DREP).\n"
			"After a disconnect, a timewait occurs before the QP "
			"can be reused, however RV avoids this delay by using a fresh QP.\n"
			"A Message Request Ack (MRA) may be used to ack any "
			"message if the recipient anticipates a slow response.\n"
			"A connection can change it's path via Load "
			"Alternate Path (LAP) and Alternate Path Response (APR)."),
		PSMI_STATS_DECL_FUNC("req_error",
				"Connection REQ errors",
				rv_conn_req_error),
		PSMI_STATS_DECL_FUNC("req_recv",
				"Connection REQ received",
				rv_conn_req_recv),
		PSMI_STATS_DECL_FUNC("rep_error",
				"Connection REP errors",
				rv_conn_rep_error),
		PSMI_STATS_DECL_FUNC("rep_recv",
				"Connection REP received",
				rv_conn_rep_recv),
		PSMI_STATS_DECL_FUNC("rtu_recv",
				"Connection RTU received",
				rv_conn_rtu_recv),
		PSMI_STATS_DECL_FUNC("established",
				"Connection established reported via ib_cm_notify",
				rv_conn_established),
		PSMI_STATS_DECL_FUNC("dreq_error",
				"Disconnect DREQ errors",
				rv_conn_dreq_error),
		PSMI_STATS_DECL_FUNC("dreq_recv",
				"Disconnect DREQ received",
				rv_conn_dreq_recv),
		PSMI_STATS_DECL_FUNC("drep_recv",
				"Disconnect DREP received",
				rv_conn_drep_recv),
		PSMI_STATS_DECL_FUNC("timewait",
				"Timewait exit events",
				rv_conn_timewait),
		PSMI_STATS_DECL_FUNC("mra_recv",
				"MRA received (Message Receipt Ack)",
				rv_conn_mra_recv),
		PSMI_STATS_DECL_FUNC("rej_recv",
				"Connection REJ received",
				rv_conn_rej_recv),
		PSMI_STATS_DECL_FUNC("lap_error",
				"LAP errors (Load Alternate Path)",
				rv_conn_lap_error),
		PSMI_STATS_DECL_FUNC("lap_recv",
				"LAP received (Load Alternate Path)",
				rv_conn_lap_recv),
		PSMI_STATS_DECL_FUNC("apr_recv",
				"APR received (Alternate Path Response)",
				rv_conn_apr_recv),
		PSMI_STATS_DECL_FUNC("unexp_event",
				"Unexpected events from CM",
				rv_conn_unexp_event),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV Total outbound CM Message Counts:"),
		PSMI_STATS_DECL_FUNC("req_sent",
				"Connection REQ sent",
				rv_conn_req_sent),
		PSMI_STATS_DECL_FUNC("rep_sent",
				"Connection REP sent",
				rv_conn_rep_sent),
		PSMI_STATS_DECL_FUNC("rtu_sent",
				"Connection RTU sent",
				rv_conn_rtu_sent),
		PSMI_STATS_DECL_FUNC("rej_sent",
				"Connection REJ sent",
				rv_conn_rej_sent),
		PSMI_STATS_DECL_FUNC("dreq_sent",
				"Disconnect DREQ sent",
				rv_conn_dreq_sent),
		PSMI_STATS_DECL_FUNC("drep_sent",
				"Disconnect DREP sent",
				rv_conn_drep_sent),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV CM Initial Connection Performance:\n"
			"The listener (server side) of a connection must wait "
			"for inbound connections from clients.\n"
			"A client begins by resolving the address of the "
			"server and then performing the connection protocol "
			"starting with the REQ.\n"
			"Once established, the connection remains connected "
			"until job end or a connection loss due to comms "
			"or QP errors.\n"),
		PSMI_STATS_DECLU64("wait_time",
				"Maximum microseconds listening for initial connection",
				(uint64_t*)&ep_rv_conn_stats->wait_time),
		PSMI_STATS_DECLU64("resolve_time",
				"Maximum microseconds for initial connection address resolution",
				(uint64_t*)&ep_rv_conn_stats->resolve_time),
		PSMI_STATS_DECLU64("connect_time",
				"Maximum microseconds for initial connection establishment",
				(uint64_t*)&ep_rv_conn_stats->connect_time),
		PSMI_STATS_DECLU64("connected_time",
				"Maximum total microseconds connection was established",
				(uint64_t*)&ep_rv_conn_stats->connected_time),
		PSMI_STATS_DECL_FUNC("resolve",
				"Total address resolution attempts",
				rv_conn_resolve),
		PSMI_STATS_DECL_FUNC("resolve_fail",
				"Total address resolution failures",
				rv_conn_resolve_fail),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV CM Connection Recovery Performance:\n"
			"When a connection is lost before job end, RV attempts "
			"to reestablish it by doing a time wait followed by "
			"repeatng the address resolution and connection "
			"protocol."),
		PSMI_STATS_DECL_FUNC("conn_recovery",
				"Total successful connection recovery",
				rv_conn_conn_recovery),
		PSMI_STATS_DECLU64("rewait_time",
				"Maximum total microseconds listening for reconnection",
				(uint64_t*)&ep_rv_conn_stats->rewait_time),
		PSMI_STATS_DECLU64("reresolve_time",
				"Maximum total microseconds for reconnection addr resolution",
				(uint64_t*)&ep_rv_conn_stats->reresolve_time),
		PSMI_STATS_DECLU64("reconnect_time",
				"Maximum total microseconds for reconnection establishment",
				(uint64_t*)&ep_rv_conn_stats->reconnect_time),
		PSMI_STATS_DECLU64("max_rewait_time",
				"Maximum microseconds listening for reconnection",
				(uint64_t*)&ep_rv_conn_stats->max_rewait_time),
		PSMI_STATS_DECLU64("max_reresolve_time",
				"Maximum microseconds for reconnection addr resolution",
				(uint64_t*)&ep_rv_conn_stats->max_reresolve_time),
		PSMI_STATS_DECLU64("max_reconnect_time",
				"Maximum microseconds for reconnection establishment",
				(uint64_t*)&ep_rv_conn_stats->max_reconnect_time),
		PSMI_STATS_DECL_FUNC("reresolve",
				"Total address re-resolution attempts",
				rv_conn_reresolve),
		PSMI_STATS_DECL_FUNC("reresolve_fail",
				"Total address re-resolution failures",
				rv_conn_reresolve_fail),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV outbound RDMA Statistics:\n"
			"PSM3 transfers rendezvous messages by issuing "
			"RDMA writes via RV for window size chunks of the message."),
		PSMI_STATS_DECLU64("post_write",
				"Total RDMA writes successfully posted",
				(uint64_t*)&ep_rv_conn_stats->post_write),
		PSMI_STATS_DECLU64("post_write_fail",
				"Total RDMA writes failed at time of posting",
				(uint64_t*)&ep_rv_conn_stats->post_write_fail),
		PSMI_STATS_DECLU64("post_write_bytes",
				"Total RDMA write bytes successfully posted",
				(uint64_t*)&ep_rv_conn_stats->post_write_bytes),
		PSMI_STATS_DECL_FUNC("send_write_out",
				"Current RDMA Writes Outstanding",
				rv_conn_outstand_send_write),
		PSMI_STATS_DECLU64("send_write_cqe",
				"Total RDMA write successful completions",
				(uint64_t*)&ep_rv_conn_stats->send_write_cqe),
		PSMI_STATS_DECLU64("send_write_cqe_fail",
				"Total RDMA write failed completions",
				(uint64_t*)&ep_rv_conn_stats->send_write_cqe_fail),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV inbound RDMA Statistics:"),
		PSMI_STATS_DECLU64("recv_write_cqe",
				"Total inbound RDMA write successful completions",
				(uint64_t*)&ep_rv_conn_stats->recv_write_cqe),
		PSMI_STATS_DECLU64("recv_write_bytes",
				"Total inbound RDMA write bytes successfully received",
				(uint64_t*)&ep_rv_conn_stats->recv_write_bytes),
		PSMI_STATS_DECLU64("recv_cqe_fail",
				"Total inbound RDMA write failed completions",
				(uint64_t*)&ep_rv_conn_stats->recv_cqe_fail),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV outbound Heartbeat Statistics:\n"
			"In order to detect network disruptions or abnormal "
			"termination of remote processes in a job, a "
			"periodic heartbeat is used."),
		PSMI_STATS_DECLU64("post_hb",
				"Total heartbeat sends successfully posted",
				(uint64_t*)&ep_rv_conn_stats->post_hb),
		PSMI_STATS_DECLU64("post_hb_fail",
				"Total heartbeat sends failed at time of posting",
				(uint64_t*)&ep_rv_conn_stats->post_hb_fail),
		PSMI_STATS_DECLU64("send_hb_cqe",
				"Total heartbeat send successful completions",
				(uint64_t*)&ep_rv_conn_stats->send_hb_cqe),
		PSMI_STATS_DECLU64("send_hb_cqe_fail",
				"Total heartbeat send failed completions",
				(uint64_t*)&ep_rv_conn_stats->send_hb_cqe_fail),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV inbound Heartbeat Statistics:"),
		PSMI_STATS_DECLU64("recv_hb_cqe",
				"Total heartbeat recv successful completions",
				(uint64_t*)&ep_rv_conn_stats->recv_hb_cqe),
	};

	psm3_stats_register_type("RV_Shared_Conn_RDMA_Statistics",
	    "Kernel RV Stats for all connections/QPs shared with an endpoint in the process",
					PSMI_STATSTYPE_RV_RDMA,
					entries,
					PSMI_HOWMANY(entries),
					psm3_epid_fmt_internal(ep->epid, 0), ep, ep->dev_name);
}

static void deregister_rv_conn_stats(psm2_ep_t ep)
{
	psm3_stats_deregister_type(PSMI_STATSTYPE_RV_RDMA, ep);
}

// accessor functions for event statistics
static uint64_t rv_send_write_cqe(void *context)
{
	psm2_ep_t ep = (psm2_ep_t)context;
	struct psm3_verbs_ep *vep = &ep->verbs_ep;

	if (ep->rv) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		// we get aggregated values instead of per conn
		(void)psm3_rv_get_event_stats(ep->rv, &vep->rv_event_stats);
	}
	return vep->rv_event_stats.send_write_cqe;
}

#define RV_EVENT_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		struct psm3_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep; \
		return vep->rv_event_stats.stat; \
	}

//RV_EVENT_STAT_FUNC(rv_send_write_cqe_fail, send_write_cqe_fail)
//RV_EVENT_STAT_FUNC(rv_send_write_bytes, send_write_bytes)

//RV_EVENT_STAT_FUNC(rv_recv_write_cqe, recv_write_cqe)
//RV_EVENT_STAT_FUNC(rv_recv_write_cqe_fail, recv_write_cqe_fail)
//RV_EVENT_STAT_FUNC(rv_recv_write_bytes, recv_write_bytes)
#undef RV_EVENT_STAT_FUNC

static void register_rv_event_stats(psm2_ep_t ep)
{
	struct psm3_rv_event_stats *ep_rv_event_stats = &ep->verbs_ep.rv_event_stats;

	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL_HELP("RV outbound RDMA Events:"),
		PSMI_STATS_DECL_FUNC("send_write_cqe",
				"Total successful RDMA Write completions",
				rv_send_write_cqe),
		PSMI_STATS_DECLU64("send_write_cqe_fail",
				"Total failed RDMA Write completions",
				(uint64_t*)&ep_rv_event_stats->send_write_cqe_fail),
		PSMI_STATS_DECLU64("send_write_bytes",
				"Total RDMA Write bytes successfully sent",
				(uint64_t*)&ep_rv_event_stats->send_write_bytes),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("RV inbound RDMA Events:"),
		PSMI_STATS_DECLU64("recv_write_cqe",
				"Total successful inbound RDMA Write completions",
				(uint64_t*)&ep_rv_event_stats->recv_write_cqe),
		PSMI_STATS_DECLU64("recv_write_cqe_fail",
				"Total failed inbound RDMA Write completions",
				(uint64_t*)&ep_rv_event_stats->recv_write_cqe_fail),
		PSMI_STATS_DECLU64("recv_write_bytes",
				"Total inbound RDMA Write bytes successfully received",
				(uint64_t*)&ep_rv_event_stats->recv_write_bytes),
	};

	psm3_stats_register_type("RV_User_Event_Statistics",
		"Kernel RV Events delivered to user space for an end point in the process.\n"
		"The RV kernel module reports events to PSM3 for outbound and "
		"inbound RDMA completions.",
					PSMI_STATSTYPE_RV_EVENT,
					entries,
					PSMI_HOWMANY(entries),
					psm3_epid_fmt_internal(ep->epid, 0), ep, ep->dev_name);
}

static void deregister_rv_event_stats(psm2_ep_t ep)
{
	psm3_stats_deregister_type(PSMI_STATSTYPE_RV_EVENT, ep);
}

static psm2_error_t open_rv(psm2_ep_t ep, psm2_uuid_t const job_key)
{
	struct local_info loc_info = { 0 };

	// we always fill in everything we might need in loc_info
	// in some modes, some of the fields are not used by RV
	loc_info.mr_cache_size = ep->rv_mr_cache_size;
#ifdef PSM_HAVE_GPU
	/* gpu_cache_size ignored unless RV_RDMA_MODE_GPU */
	loc_info.gpu_cache_size = ep->rv_gpu_cache_size;
#endif
	loc_info.rdma_mode = IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)?
					RV_RDMA_MODE_KERNEL: RV_RDMA_MODE_USER;
#ifdef PSM_HAVE_GPU
	if (PSM3_GPU_IS_ENABLED) {
		// when GPU is enabled we will have larger window_sz and
		// need to upsize the caches we will use for priority MRs
		if (ep->rdmamode & IPS_PROTOEXP_FLAG_ENABLED) {
			// priority window_sz reg_mr for CPU
			loc_info.rdma_mode |= RV_RDMA_MODE_UPSIZE_CPU;
		}
 		if (psmi_parse_gpudirect()) {
			// When GPU Direct is enabled we need a GPU Cache
			loc_info.rdma_mode |= RV_RDMA_MODE_GPU;

			PSM3_GPU_USING_RV_FOR_MRS();

			if ((ep->rdmamode & IPS_PROTOEXP_FLAG_ENABLED)
				&& (psmi_parse_gpudirect_rdma_send_limit(1)
				|| psmi_parse_gpudirect_rdma_recv_limit(1))) {
				// priority window_sz reg_mr for GPU memory
				loc_info.rdma_mode |= RV_RDMA_MODE_UPSIZE_GPU;
			}
		}
	}
#endif

	// need portnum for rdma_mode KERNEL or USER|GPU
	loc_info.port_num = ep->portnum;
	// the rest of loc_info is really only needed for RV_RDMA_MODE_KERNEL
	loc_info.num_conn = ep->verbs_ep.rv_num_conn;
	// caller computes our local EPID, but loc_addr must == our network addr (NID)
	// for what will be established as our local epid by psm3_context_open
	// later rem_addr will be compared to this and is based on low 32b of
	// network address for a remote epid
	// must be consistent with what psm3_epid_get_rem_addr will later report
	if (ep->verbs_ep.port_attr.link_layer == IBV_LINK_LAYER_ETHERNET) {
		// Supply IP addr as local address
		// However since create_conn's ah.is_global we'll actually
		// use loc_gid to decide client/server in RV instead of loc_addr
		loc_info.loc_addr = ep->addr.bare.lo;
	} else {
		loc_info.loc_addr = ep->verbs_ep.port_attr.lid;
	}
	loc_info.index_bits = RV_INDEX_BITS;
	loc_info.loc_gid_index = ep->verbs_ep.lgid_index;
	loc_info.loc_gid = ep->verbs_ep.lgid;
	// TBD qos_class_sl
	loc_info.job_key_len = min(RV_MAX_JOB_KEY_LEN, sizeof(psm2_uuid_t));
	loc_info.job_key = (uint8_t*)job_key;
	loc_info.service_id = ep->service_id;
	loc_info.context = ep;
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
		// HFI_TF_NFLOWS (32) limits recv side concurrent tidflows (aka inbound
		// for send we never have more than hfi_num_send_rdma RDMA outstanding
		loc_info.cq_entries = ep->hfi_num_send_rdma + HFI_TF_NFLOWS + 32;
	}
	loc_info.q_depth = ep->verbs_ep.rv_q_depth;
	loc_info.reconnect_timeout = ep->reconnect_timeout;
	loc_info.hb_interval = ep->verbs_ep.rv_hb_interval;
	loc_info.fr_page_list_len = ep->verbs_ep.rv_fr_page_list_len;

	ep->rv =psm3_rv_open(ep->dev_name, &loc_info);
	if (! ep->rv) {
		return PSM2_INTERNAL_ERR;
	}
	// parallel hal_gen1/gen1_hal_inline_i.h handling HFI1_CAP_GPUDIRECT_OT
#ifndef RV_CAP_GPU_DIRECT
#ifdef PSM_HAVE_GPU
#error "Inconsistent build.  RV_CAP_GPU_DIRECT must be defined for GPU builds. Must use GPU enabled rv headers"
#else
// lifted from rv_user_ioctls.h
#define RV_CAP_GPU_DIRECT (1UL << 63)
#endif
#endif
	if (loc_info.capability & RV_CAP_USER_MR)
		psmi_hal_add_cap(PSM_HAL_CAP_USER_MR);
	if (loc_info.capability & RV_CAP_GPU_DIRECT) {
		psmi_hal_add_cap(PSM_HAL_CAP_GPUDIRECT);
		psmi_hal_add_cap(PSM_HAL_CAP_GPUDIRECT_SDMA);
		psmi_hal_add_cap(PSM_HAL_CAP_GPUDIRECT_RDMA);
	}
#ifdef PSM_HAVE_GPU
	PSM3_GPU_RV_SET_HAL_CAP(loc_info.capability);
#endif
	ep->verbs_ep.rv_index = loc_info.rv_index;
	ep->rv_mr_cache_size = loc_info.mr_cache_size;
#ifdef PSM_HAVE_GPU
	ep->rv_gpu_cache_size = loc_info.gpu_cache_size;
#endif
	ep->verbs_ep.rv_q_depth = loc_info.q_depth;
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode))
		ep->reconnect_timeout = loc_info.reconnect_timeout;
	/* Default for mlx5: 256 MB */
#define MAX_FMR_SIZE_DEFAULT (64 * 1024 * 4096)
	ep->verbs_ep.max_fmr_size = loc_info.max_fmr_size;
	if (!ep->verbs_ep.max_fmr_size)
		ep->verbs_ep.max_fmr_size = MAX_FMR_SIZE_DEFAULT;

	return PSM2_OK;
}
#endif // RNDV_MOD

// initialize verbs specific statistics
void
psm3_hfp_verbs_context_initstats(psm2_ep_t ep)
{
#ifdef RNDV_MOD
	if (ep->rv && IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
		// only one set of conn stats per job_dev, so
		// no use gathering for any extra QPs we open
		if (ep->mctxt_master == ep)
			register_rv_conn_stats(ep);
		register_rv_event_stats(ep);
	}
#endif
}

static psm2_error_t verbs_open_dev(psm2_ep_t ep, int unit, int port, int addr_index, psm2_uuid_t const job_key)
{
	// similar to code in ifs-all/Topology, enumerates devices and picks one
	int i, num_of_devices;
	struct ibv_device **dev_list = NULL;
	struct ibv_device *ib_dev = NULL;
	int err = PSM2_OK;
	const char *unitpath = psm3_sysfs_unit_path(unit);
	int flags;
	int ret;

	// callers tend not to set port, 0 means any
	if (PSM3_NIC_PORT_ANY == port)
		port = VERBS_PORT;
	ep->portnum = port;
	if (! unitpath) {
		_HFI_ERROR( "NULL sysfs unitpath for unit %d\n", unit);
		return PSM2_INTERNAL_ERR;
	}

	char *dev_name = strrchr(unitpath, '/');
	if (dev_name == NULL) {
		_HFI_ERROR( "invalid sysfs unitpath for unit %d\n", unit);
		return PSM2_INTERNAL_ERR;
	}
	dev_name++; // Inc past last '/'

	ep->dev_name = psmi_strdup(ep, dev_name);
	if (! ep->dev_name) {
		_HFI_ERROR( "can't alloc devname");
		return PSM2_INTERNAL_ERR;
	}

	dev_list = ibv_get_device_list(&num_of_devices);
	if (num_of_devices <= 0) {
		_HFI_ERROR(" Did not detect any RDMA devices \n");
		_HFI_ERROR(" If device exists, check if driver is up\n");
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	if (!dev_list) {
		_HFI_ERROR(" Internal error, exiting.\n");
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	for (i = 0; i < num_of_devices; i++) {
		if (!strcmp(ibv_get_device_name(dev_list[i]), ep->dev_name))
			break;
	}
	if (i >= num_of_devices) {
		_HFI_ERROR("Unit Id [%d] name %s not found, number of devices is %d\n",
				   unit, ep->dev_name, num_of_devices);
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ep->unit_id = unit;
	_HFI_PRDBG("Using unit_id[%d] %s.\n", ep->unit_id, ep->dev_name);

	ib_dev = dev_list[i];	// device list order may differ from unit order
	errno = 0;
	ep->verbs_ep.context = ibv_open_device(ib_dev);
	if (! ep->verbs_ep.context) {
		_HFI_ERROR( "Unable to open %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else {
		_HFI_PRDBG("Opened %s.\n",ep->dev_name);
	}
	// change async events to non-blocking
	flags = fcntl( ep->verbs_ep.context->async_fd, F_GETFL);
	if (0 > fcntl( ep->verbs_ep.context->async_fd, F_SETFL, flags | O_NONBLOCK)) {
		_HFI_ERROR( "Unable to change file descriptor of async events for %s: %s\n",
					ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	ret = ibv_query_port(ep->verbs_ep.context, ep->portnum, &ep->verbs_ep.port_attr);
	if (ret) {
		_HFI_ERROR("Unable to query port %u of %s: %s\n",
			ep->portnum, ep->dev_name, strerror(ret));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else {
		_HFI_PRDBG("Queried %s.\n",ep->dev_name);
	}
	// When IPv4 or IPv6 is requested, must be an Ethernet port (RoCE)
	// when OPA/IB is requested, must be IB/OPA port
	if (! psm3_addr_fmt) {
		// all port types ok
	} else if (PSMI_ADDR_FMT_IS_ETH(psm3_addr_fmt)
		&& ep->verbs_ep.port_attr.link_layer != IBV_LINK_LAYER_ETHERNET) {
		_HFI_ERROR("PSM3_ADDR_FMT=%u specified, but selected port is not Ethernet: port %u of %s\n",
				psm3_addr_fmt, ep->portnum,
				ep->dev_name);
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else if (! PSMI_ADDR_FMT_IS_ETH(psm3_addr_fmt)
		&& ep->verbs_ep.port_attr.link_layer == IBV_LINK_LAYER_ETHERNET) {
		_HFI_ERROR("PSM3_ADDR_FMT=%u specified, but selected port is Ethernet: port %u of %s\n",
				psm3_addr_fmt, ep->portnum,
				ep->dev_name);
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	ep->addr_index = addr_index;
	if (0 != psmi_hal_get_port_subnet(ep->unit_id, ep->portnum, ep->addr_index,
			&ep->subnet, &ep->addr,
			&ep->verbs_ep.lgid_index, &ep->gid)) {
		_HFI_ERROR( "Unable to get subnet for port %u of %s: %s\n",
				ep->portnum, ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else {
		ep->verbs_ep.lgid.global.subnet_prefix = __cpu_to_be64(ep->gid.hi);
		ep->verbs_ep.lgid.global.interface_id = __cpu_to_be64(ep->gid.lo);
		_HFI_PRDBG("Subnet for port %u of %s: %s addr %s gid %s\n",
				ep->portnum, ep->dev_name,
				psm3_subnet128_fmt(ep->subnet, 0),
				psm3_naddr128_fmt(ep->addr, 1),
				psm3_gid128_fmt(ep->gid, 2));
	}

#if defined(USE_RDMA_READ)
#if defined(USE_RC)
	{
		union psmi_envvar_val envvar_val;
		struct ibv_device_attr dev_attr;
		// get RDMA capabilities of device
		ret = ibv_query_device(ep->verbs_ep.context, &dev_attr);
		if (ret) {
			_HFI_ERROR("Unable to query device %s: %s\n",
				ep->dev_name, strerror(ret));
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
		ep->verbs_ep.max_qp_rd_atom = dev_attr.max_qp_rd_atom;
		ep->verbs_ep.max_qp_init_rd_atom = dev_attr.max_qp_init_rd_atom;
		_HFI_PRDBG("got device attr: rd_atom %u init_rd_atom %u\n",
						dev_attr.max_qp_rd_atom, dev_attr.max_qp_init_rd_atom);
		// TBD could have an env variable to reduce requested values

		if (ep->verbs_ep.max_qp_rd_atom && ep->verbs_ep.max_qp_init_rd_atom &&
			IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode)) {
			psm3_getenv("PSM3_USE_RDMA_READ",
				"Allow PSM3 to use RDMA_READ if available (used by flow control) [1=yes, 0=no]",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)1, &envvar_val);
			if (envvar_val.e_uint == 0) {
				ep->verbs_ep.max_qp_rd_atom = 0;
				ep->verbs_ep.max_qp_init_rd_atom = 0;
			}
		}
	}
#endif // USE_RC
#endif // USE_RDMA_READ
#ifdef RNDV_MOD
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)
		|| ep->mr_cache_mode == MR_CACHE_MODE_KERNEL ) {
		// open rendezvous module for the same port as our verbs device
		err = open_rv(ep, job_key);
		if (err != PSM2_OK) {
			_HFI_ERROR( "Unable to open rendezvous module for port %u of %s.\n",
				ep->portnum, ep->dev_name);
			// TBD - could ignore error and proceed with UD mode
			//err = PSM2_OK;
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
		if (ep->mr_cache_mode == MR_CACHE_MODE_KERNEL
			&& ! psmi_hal_has_cap(PSM_HAL_CAP_USER_MR)) {
			_HFI_ERROR( "Rendezvous module lacks enable_user_mr capability.\n");
			// TBD - could ignore error and proceed with UD mode
			//err = PSM2_OK;
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
	}
#endif
	if (! IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode))
		psmi_hal_add_cap(PSM_HAL_CAP_NIC_LOOPBACK);

done:
	if (dev_list)
		ibv_free_device_list(dev_list);
	return err;

fail:
	if (ep->verbs_ep.context) {
		ibv_close_device(ep->verbs_ep.context);
		ep->verbs_ep.context = NULL;
	}
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}
	goto done;
}

static psm2_error_t
check_port_state(psm2_ep_t ep)
{
	uint32_t active_mtu;

	active_mtu = MTU_SIZE(ep->verbs_ep.port_attr.active_mtu);
	if (ep->verbs_ep.port_attr.link_layer == IBV_LINK_LAYER_ETHERNET) {
		_HFI_PRDBG("running on ethernet at %d MTU\n", active_mtu);
	} else {
		_HFI_PRDBG( "running on %s at %d MTU\n", link_layer_str(ep->verbs_ep.port_attr.link_layer), active_mtu);
	}
	if (strcmp("Unknown", link_layer_str(ep->verbs_ep.port_attr.link_layer)) == 0) {
		_HFI_ERROR( "Link layer on port %d of %s is Unknown\n", ep->portnum,
						ep->dev_name);
		return PSM2_INTERNAL_ERR;
	}
	//ep->verbs_ep.link_layer = ep->verbs_ep.port_attr.link_layer;

	if (ep->verbs_ep.port_attr.state != IBV_PORT_ACTIVE) {
		_HFI_ERROR( " Port state is not active for %s port %d: %d\n",
						ep->dev_name, ep->portnum,
						ep->verbs_ep.port_attr.state);
		//_HFI_ERROR( " Port number %d on %s state is %s\n",
				//params->ib_port, ep->dev_name,
				//portStates[ep->verbs_ep.port_attr.state]);
		return PSM2_INTERNAL_ERR;
	}

	// compute MTU.
	// ep->mtu is the PSM payload size.  For OPA native mode, this did not
	// include headers as OPA allowed up to an additional 128 bytes of headers.
	// However all UD QP payloads (including PSM headers) are
	// counted toward MTU in UD verbs.  So need to discount by PSM header size
	ep->mtu = active_mtu - MAX_PSM_HEADER;
	_HFI_PRDBG("Max PSM payload (aka MTU): %u\n", ep->mtu);
#ifdef USE_RC
	// TBD - *act_mtu = defined constant, we can use an eager RC message size
	// for PSM which is larger than packet MTU
#endif
	ep->verbs_ep.active_rate = verbs_get_rate(
									ep->verbs_ep.port_attr.active_width,
									ep->verbs_ep.port_attr.active_speed);
	return PSM2_OK;
}

static struct ibv_qp* ud_qp_create(psm2_ep_t ep)
{
	struct ibv_qp* qp = NULL;

	struct ibv_qp_init_attr attr = { 0 };

	attr.qp_context = ep;	// our own pointer
	attr.send_cq = ep->verbs_ep.send_cq;
	attr.recv_cq = ep->verbs_ep.recv_cq;
	// one extra WQE to be safe in case verbs needs a spare WQE
	attr.cap.max_send_wr  = ep->verbs_ep.hfi_num_send_wqes+1;
	attr.cap.max_send_sge = 2;
	attr.cap.max_inline_data = ep->hfi_imm_size;

	attr.srq = NULL;
	attr.cap.max_recv_wr  = ep->verbs_ep.hfi_num_recv_wqes;
	attr.cap.max_recv_sge = 1;

	attr.qp_type = IBV_QPT_UD;

	errno = 0;
	qp = ibv_create_qp(ep->verbs_ep.pd, &attr);
	if (qp == NULL) {
		_HFI_ERROR("Unable to create UD QP on %s: %s\n", ep->dev_name, strerror(errno));
		if (errno == ENOMEM) {
			_HFI_ERROR("Requested QP size might be too big. Try reducing TX depth and/or inline size.\n");
			_HFI_ERROR("Requested TX depth was %u and RX depth was %u.\n",
				ep->verbs_ep.hfi_num_send_wqes+1, ep->verbs_ep.hfi_num_recv_wqes);
		}
		return NULL;
	}

	// attr reports what we got, double check and react in case
	ep->verbs_ep.qp_cap = attr.cap;

	// QP adjusted values due to HW limits
	if (ep->hfi_imm_size > attr.cap.max_inline_data) {
		_HFI_PRDBG( "Limited to inline size of %d, requested %u\n",
			attr.cap.max_inline_data, ep->hfi_imm_size);
	} else {
		_HFI_PRDBG("Inline Size: %u\n", attr.cap.max_inline_data);
	}
	if (ep->verbs_ep.hfi_num_send_wqes+1 > attr.cap.max_send_wr) {
		_HFI_PRDBG( "Limited to %d SQ WQEs, requested %u\n",
			attr.cap.max_send_wr, ep->verbs_ep.hfi_num_send_wqes+1);
	} else {
		_HFI_PRDBG("SQ WQEs: %u\n", attr.cap.max_send_wr);
	}
	if (2 > attr.cap.max_send_sge) {
		_HFI_PRDBG( "Limited to %d SQ SGEs\n",
			attr.cap.max_send_sge);
	}
	if (ep->verbs_ep.hfi_num_recv_wqes > attr.cap.max_recv_wr) {
		_HFI_PRDBG( "Limited to %d RQ WQEs, requested %u\n",
			attr.cap.max_recv_wr, ep->verbs_ep.hfi_num_recv_wqes);
	} else {
		_HFI_PRDBG("RQ WQEs: %u\n", attr.cap.max_recv_wr);
	}
	if (1 > attr.cap.max_recv_sge) {
		_HFI_PRDBG( "Limited to %d RQ SGEs\n",
			attr.cap.max_recv_sge);
	}

	return qp;
}

static psm2_error_t modify_ud_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_QKEY;
	int err;

	attr.qp_state = IBV_QPS_INIT;
	attr.pkey_index = ep->network_pkey_index;
	attr.port_num =  ep->portnum;
	attr.qkey = ep->verbs_ep.qkey;
	//attr.qp_access_flags N/A for UD
	//flags |= IBV_QP_ACCESS_FLAGS;

	err = ibv_modify_qp(qp, &attr,flags);
	if (err) {
		_HFI_ERROR("Failed to modify UD QP to INIT on %s: %s\n",
			ep->dev_name, strerror(err));
		return PSM2_INTERNAL_ERR;
	}
	return PSM2_OK;
}

static psm2_error_t modify_ud_qp_to_rtr(psm2_ep_t ep,struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE;
	int err;

	attr.qp_state = IBV_QPS_RTR;

	err = ibv_modify_qp(qp, &attr, flags);
	if (err) {
		_HFI_ERROR("Failed to modify UD QP to RTR on %s: %s\n",
			ep->dev_name, strerror(err));
		return PSM2_INTERNAL_ERR;
	}
	return PSM2_OK;
}

static psm2_error_t modify_ud_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE | IBV_QP_SQ_PSN;
	int err;

	attr.qp_state = IBV_QPS_RTS;
	attr.sq_psn = 0x1234;	// doesn't really matter for UD

	err = ibv_modify_qp(qp, &attr, flags);
	if (err) {
		_HFI_ERROR("Failed to modify UD QP to RTS on %s: %s\n",
			ep->dev_name, strerror(err));
		return PSM2_INTERNAL_ERR;
	}
	return PSM2_OK;
}

#ifdef USE_RC
struct ibv_qp* rc_qp_create(psm2_ep_t ep, void *context, struct ibv_qp_cap *cap)
{
	struct ibv_qp* qp = NULL;

	struct ibv_qp_init_attr attr;
	memset(&attr, 0, sizeof(struct ibv_qp_init_attr));

	attr.qp_context = context;
	attr.send_cq = ep->verbs_ep.send_cq;
	attr.recv_cq = ep->verbs_ep.recv_cq;
	attr.srq = ep->verbs_ep.srq;
	// one extra WQE to be safe in case verbs needs a spare WQE
	if ((ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
		// need to be prepared in case all sends posted to same RC QP, so
		// match the number of send buffers we plan to allocate
		// +1 in case verbs needs an extra WQE to detect QP full
#ifdef PSM_RC_RECONNECT
		// +1 so have a WQE for drain MARKER
		attr.cap.max_send_wr  = ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+2;
#else
		attr.cap.max_send_wr  = ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+1;
#endif
		attr.cap.max_send_sge = 2;
		// inline data helps latency and message rate for small sends
		attr.cap.max_inline_data = ep->hfi_imm_size;
		attr.cap.max_recv_wr  = ep->verbs_ep.srq?0
				:(ep->verbs_ep.hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION);// TBD
		attr.cap.max_recv_sge = 1;
	} else {
		// only RDMA Write w/immediate
		attr.cap.max_send_wr  = ep->hfi_num_send_rdma+1;
		attr.cap.max_send_sge = 1;
		attr.cap.max_inline_data = 0;
		// incoming Write w/immediate consumes a RQ WQE but no buffer needed
		attr.cap.max_recv_wr  = ep->verbs_ep.srq?0:(HFI_TF_NFLOWS+1);
		attr.cap.max_recv_sge = 0;
	}

	attr.qp_type = IBV_QPT_RC;

	qp = ibv_create_qp(ep->verbs_ep.pd, &attr);
	if (qp == NULL) {
		_HFI_ERROR("Unable to create RC QP on %s\n", ep->dev_name);
		_HFI_ERROR("Requested QP size might be too big. Try reducing TX depth and/or inline size.\n");
		if ((ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
			_HFI_ERROR("Requested TX depth was %u and RX depth was %u.\n",
#ifdef PSM_RC_RECONNECT
				ep->verbs_ep.hfi_num_send_wqes + ep->hfi_num_send_rdma + 2,
#else
				ep->verbs_ep.hfi_num_send_wqes + ep->hfi_num_send_rdma + 1,
#endif
				ep->verbs_ep.srq
					? 0 : ep->verbs_ep.hfi_num_recv_wqes / VERBS_RECV_QP_FRACTION);
		} else {
			_HFI_ERROR("Requested TX depth was %u and RX depth was %u\n",
				ep->hfi_num_send_rdma + 1,
				ep->verbs_ep.srq ? 0 : HFI_TF_NFLOWS + 1);
		}
		return NULL;
	}

// TBD - getting too small resources should be fatal or adjust limits to be smaller - such as num_sge
	if ((ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
		// QP adjusted values due to HW limits
		if (ep->hfi_imm_size > attr.cap.max_inline_data) {
			_HFI_PRDBG( "Limited to inline size of %d, requested %u\n",
				attr.cap.max_inline_data, ep->hfi_imm_size);
		} else {
			_HFI_PRDBG("Inline Size: %u\n", attr.cap.max_inline_data);
		}
#ifdef PSM_RC_RECONNECT
		if (ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+2 > attr.cap.max_send_wr) {
#else
		if (ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+1 > attr.cap.max_send_wr) {
#endif
			_HFI_PRDBG( "Limited to %d SQ WQEs, requested %u\n",
#ifdef PSM_RC_RECONNECT
				attr.cap.max_send_wr, ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+2);
#else
				attr.cap.max_send_wr, ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+1);
#endif
		} else {
			_HFI_PRDBG("SQ WQEs: %u\n", attr.cap.max_send_wr);
		}
		if (2 > attr.cap.max_send_sge) {
			_HFI_PRDBG( "Limited to %d SQ SGEs\n",
				attr.cap.max_send_sge);
		}
		if (! ep->verbs_ep.srq
		    && ep->verbs_ep.hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION > attr.cap.max_recv_wr) {
			_HFI_PRDBG( "Limited to %d RQ WQEs, requested %u\n",
				attr.cap.max_recv_wr, ep->verbs_ep.hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION);
		} else {
			_HFI_PRDBG("RQ WQEs: %u\n", attr.cap.max_recv_wr);
		}
		if (1 > attr.cap.max_recv_sge) {
			_HFI_PRDBG( "Limited to %d RQ SGEs\n",
				attr.cap.max_recv_sge);
		}

		// we need to make sure we can't overflow send Q
#ifdef PSM_RC_RECONNECT
		// -1 to hold back 1 WQE for use as drain MARKER
		if (attr.cap.max_send_wr-1 < ep->verbs_ep.send_pool.send_total) {
#else
		if (attr.cap.max_send_wr < ep->verbs_ep.send_pool.send_total) {
#endif
			_HFI_ERROR("RC QP Send Q too small: Limited to %d SQ WQEs, requested %u, min %u\n",
				attr.cap.max_send_wr,
#ifdef PSM_RC_RECONNECT
				ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+2,
#else
				ep->verbs_ep.hfi_num_send_wqes+ep->hfi_num_send_rdma+1,
#endif
				ep->verbs_ep.send_pool.send_total);
			goto fail;
		}
	} else {
		// QP adjusted values due to HW limits
		if (ep->hfi_num_send_rdma+1 > attr.cap.max_send_wr) {
			_HFI_PRDBG( "Limited to %d SQ WQEs, requested %u\n",
				attr.cap.max_send_wr, ep->hfi_num_send_rdma+1);
		} else {
			_HFI_PRDBG("SQ WQEs: %u\n", attr.cap.max_send_wr);
		}
		if (1 > attr.cap.max_send_sge) {
			_HFI_PRDBG( "Limited to %d SQ SGEs\n",
				attr.cap.max_send_sge);
		}
		if (! ep->verbs_ep.srq
		    && HFI_TF_NFLOWS+1 > attr.cap.max_recv_wr) {
			_HFI_PRDBG( "Limited to %d RQ WQEs, requested %u\n",
				attr.cap.max_recv_wr, HFI_TF_NFLOWS+1);
		} else {
			_HFI_PRDBG("RQ WQEs: %u\n", attr.cap.max_recv_wr);
		}
	}

	if (cap)
		*cap = attr.cap;
	_HFI_PRDBG("created RC QP %u\n", qp->qp_num);
	return qp;

fail:
	ibv_destroy_qp(qp);
	return NULL;
}

void rc_qp_destroy(struct ibv_qp* qp)
{
	_HFI_PRDBG("destroying RC QP %u\n", qp->qp_num);
	ibv_destroy_qp(qp);
}

psm2_error_t modify_rc_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT;
	int err;

	attr.qp_state        = IBV_QPS_INIT;
	attr.pkey_index = ep->network_pkey_index;
	attr.port_num =  ep->portnum;

	//attr.qkey = ep->verbs_ep.qkey;
	//flags |= IBV_QP_QKEY;	// only allowed for UD
	attr.qp_access_flags = 0;
#ifdef USE_RDMA_READ
	attr.qp_access_flags |= IBV_ACCESS_REMOTE_READ;
#endif
	attr.qp_access_flags |= IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_LOCAL_WRITE;
	//attr.qp_access_flags |= IBV_ACCESS_REMOTE_ATOMIC;
	flags |= IBV_QP_ACCESS_FLAGS;

	err = ibv_modify_qp(qp, &attr, flags);
	if (err) {
		_HFI_ERROR("Failed to modify RC QP to INIT on %s: %s\n",
			ep->dev_name, strerror(err));
		return PSM2_INTERNAL_ERR;
	}
	_HFI_PRDBG("moved RC QP %u to INIT\n", qp->qp_num);
	return PSM2_OK;
}

// initpsn is from packet we received
// req_attr is from REQ or REP from other side
psm2_error_t modify_rc_qp_to_rtr(psm2_ep_t ep, struct ibv_qp *qp,
				const struct psm_rc_qp_attr *req_attr,
				const ips_path_rec_t *path_rec, uint32_t initpsn)
{
	int flags = IBV_QP_STATE;
	struct ibv_qp_attr attr = { 0 };
	int err;

	attr.qp_state = IBV_QPS_RTR;

	psm3_verbs_ips_path_rec_to_ah_attr(ep, path_rec, &attr.ah_attr);
	flags |= IBV_QP_AV;

	// TBD - we already factored in req vs pr to update pr no need
	// for modify_cq_qp_to_rtr to repeat it
	// pr_mtu is max PSM payload in bytes and req_attr_mtu is IB enum
	attr.path_mtu = MIN(ibv_mtu_int_to_enum(ep->mtu), req_attr->mtu);
	attr.dest_qp_num = req_attr->qpn;
	attr.rq_psn = initpsn;
	flags |= (IBV_QP_PATH_MTU | IBV_QP_DEST_QPN | IBV_QP_RQ_PSN);

#ifdef USE_RDMA_READ
	ep->verbs_ep.max_qp_rd_atom =
		min(ep->verbs_ep.max_qp_rd_atom, req_attr->initiator_depth);
	attr.max_dest_rd_atomic = ep->verbs_ep.max_qp_rd_atom;
#endif
	_HFI_PRDBG("set max_dest_rd_atomic to %u\n", attr.max_dest_rd_atomic);
	attr.min_rnr_timer = 12;	// TBD well known
	flags |= (IBV_QP_MIN_RNR_TIMER | IBV_QP_MAX_DEST_RD_ATOMIC);

	err = ibv_modify_qp(qp, &attr, flags);
	if (err) {
		_HFI_ERROR("Failed to modify RC QP to RTR on %s: %s\n",
			ep->dev_name, strerror(err));
		return PSM2_INTERNAL_ERR;
	}
	_HFI_PRDBG("moved RC QP %u to RTR with MTU=%d\n", qp->qp_num, attr.path_mtu);

	return PSM2_OK;
}

// initpsn is value we sent in our req and rep
// req_attr is from REP we received from other side
// if returns PSM2_EPID_RC_CONNECT_ERROR then RC QP already got an error in RTR
// and is in ERR, so can't move to RTS
psm2_error_t modify_rc_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp,
				const struct psm_rc_qp_attr *req_attr, uint32_t initpsn)
{
	int flags = IBV_QP_STATE;
	struct ibv_qp_attr attr = { 0 };
	struct ibv_qp_init_attr init_attr;
	int rc;

	attr.qp_state = IBV_QPS_RTS;

	attr.sq_psn = initpsn;	// value we told other side
	flags |= IBV_QP_SQ_PSN;

#ifdef USE_RDMA_READ
	ep->verbs_ep.max_qp_init_rd_atom =
		min(ep->verbs_ep.max_qp_init_rd_atom, req_attr->responder_resources);
	attr.max_rd_atomic = ep->verbs_ep.max_qp_init_rd_atom;
#endif
	_HFI_PRDBG("set max_rd_atomic to %u\n", attr.max_rd_atomic);
	flags |=  IBV_QP_MAX_QP_RD_ATOMIC;

	attr.retry_cnt = ep->verbs_ep.hfi_qp_retry;
	attr.rnr_retry = ep->verbs_ep.hfi_qp_retry;	// only for eager RC QP rdmamode
	attr.timeout = ep->verbs_ep.hfi_qp_timeout;
	flags |= IBV_QP_RETRY_CNT | IBV_QP_RNR_RETRY | IBV_QP_TIMEOUT;

	_HFI_PRDBG("moving RC QP %u to RTS\n", qp->qp_num);
	rc = ibv_modify_qp(qp, &attr, flags);
	if (rc) {
		int query_rc;

		// could already be in ERR (possibly due to race with async event)
		attr.qp_state = IBV_QPS_UNKNOWN;
		query_rc = ibv_query_qp(qp, &attr, IBV_QP_STATE, &init_attr);
		if (!query_rc && attr.qp_state == IBV_QPS_ERR) {
			_HFI_VDBG("Failed to modify RC QP %u to RTS, QP in ERR on %s: %s\n",
				qp->qp_num, ep->dev_name, strerror(query_rc));
			return PSM2_EPID_RC_CONNECT_ERROR;
		}
		_HFI_ERROR("Failed to modify RC QP %u to RTS on %s: %s\n",
			qp->qp_num, ep->dev_name, strerror(rc));
		return PSM2_INTERNAL_ERR;
	}
	//psm3_dump_verbs_qp(qp);
	return PSM2_OK;
}
#ifdef PSM_RC_RECONNECT
static psm2_error_t modify_rc_qp_to_err(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	struct ibv_qp_init_attr init_attr;
	int err;
	int save_err;

	attr.qp_state = IBV_QPS_ERR;

	err = ibv_modify_qp(qp, &attr, IBV_QP_STATE);
	if (!err) {
		_HFI_VDBG("moved RC QP %u to ERR\n", qp->qp_num);
		return PSM2_OK;
	}
	save_err = err;
	// some drivers can fail the ERR->ERR transition, check if
	// already in ERR (possibly due to race with async event)
	attr.qp_state = IBV_QPS_UNKNOWN;
	err = ibv_query_qp(qp, &attr, IBV_QP_STATE, &init_attr);
	if (!err && attr.qp_state == IBV_QPS_ERR) {
		_HFI_VDBG("RC QP %u already in ERR\n", qp->qp_num);
		return PSM2_OK;
	}
	_HFI_ERROR("Failed to modify RC QP %u to ERR on %s: state %u: %s\n",
		qp->qp_num, ep->dev_name, attr.qp_state, strerror(save_err));
	return PSM2_INTERNAL_ERR;
}

psm2_error_t psm3_verbs_post_send_drain(psm2_ep_t ep, struct psm3_verbs_rc_qp *rc_qp)
{
	// post special WQE on SQ
	struct ibv_send_wr wr;
	struct ibv_send_wr *bad_wr;
	struct ibv_sge list;
	int err;

	_HFI_CONNDBG("posting send drain QP %u\n", rc_qp->qp->qp_num);
	psmi_assert(! rc_qp->draining);
	wr.next = NULL; // just post 1
	psmi_assert(! VERBS_SQ_WR_OP((uintptr_t)rc_qp));
	wr.wr_id = (uintptr_t)rc_qp | VERBS_SQ_WR_ID_DRAIN_MARKER;
	// races inside the NIC have been observed where a QP is in ERR, but the
	// drain WQE is still successfully processed and generates a packet on wire
	// to avoid that we post an invalid sg_list and lkey
	// using invalid lkey and a vaddr not registered with NIC
	list.addr = (uintptr_t)rc_qp;	// valid vaddr in heap
	list.length = 1;
	list.lkey = VERBS_SQ_DRAIN_BAD_LKEY;
	wr.sg_list = &list;
	wr.num_sge = 1;
	wr.opcode = IBV_WR_SEND;
	//wr.sg_list = NULL;
	//wr.num_sge = 0;
	//wr.opcode = IBV_WR_RDMA_WRITE;
	wr.send_flags = IBV_SEND_SIGNALED; // get a completion

	// EINTR should be transient (call interrupted by event)
	// any other issue is not retried
	while (1) {
		rc_qp->send_posted++;
		err = ibv_post_send(rc_qp->qp, &wr, &bad_wr);
		if_pf (err) {
			rc_qp->send_posted--;
			if (err != EINTR) {
				_HFI_ERROR("failed to post RC SQ drain on ipsaddr %p %s port %u: %s (%d)",
					rc_qp->ipsaddr, ep->dev_name, ep->portnum,
					strerror(errno), err);
				return PSM2_INTERNAL_ERR;
			}
		} else {
			break;	// success
		}
	}
	_HFI_CONNDBG("[ipsaddr %p] RC QP %u rcnt %u posted RC SQ drain WQE\n",
		rc_qp->ipsaddr, rc_qp->qp->qp_num, rc_qp->reconnect_count);
	return PSM2_OK;
}

// even if fail to move QP to IBV_QPS_ERR, we must still set draining flag
// and attempt to free QP, otherwise, we may end up invalidly calling
// psm3_ips_proto_connection_error a second time for the same rc_qp if we
// see a bad CQE for the QP.
// when this returns error, caller will fatal error, otherwise we will never
// finish draining the QP
psm2_error_t psm3_verbs_drain_rc_qp(psm2_ep_t ep, struct psm3_verbs_rc_qp *rc_qp,
		int move_to_err)
{
	psm2_error_t err = PSM2_OK;

	psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode));
	if (! rc_qp->draining) {
		// if never got to Init, can't have anything posted
		psmi_assert(rc_qp->initialized
			|| rc_qp->send_posted + rc_qp->recv_pool.posted == 0);
		// if never got to Init, invalid to move to ERR
		if (move_to_err && rc_qp->initialized) {
			err = modify_rc_qp_to_err(ep, rc_qp->qp);
			if (! err)
				_HFI_CONNDBG("[ipsaddr %p] RC QP %u to ERR\n",
					rc_qp->ipsaddr, rc_qp->qp->qp_num);
		}
		if (! err && rc_qp->send_allocator.send_alloc_head) {
			psmi_assert(rc_qp->send_posted);
			psmi_assert(rc_qp->initialized);
			// must issue a send with completion to force flush of
			// prior sbufs which may have successfully completed
			// already but did not request a completion
			err = psm3_verbs_post_send_drain(ep, rc_qp);
		}
		// send_drain_outstanding will force poll_cq until we free the
		// rc_qp.  Not worth optimizing to limit to QPs which have
		// outstanding sends since the QP will be freed soon enough.
		ep->verbs_ep.send_drain_outstanding++;
		rc_qp->draining = 1;
		rc_qp->recv_pool.draining = 1;
		_HFI_CONNDBG("[ipsaddr %p] RC QP %u rcnt %u to draining posted %u + %u\n",
			rc_qp->ipsaddr, rc_qp->qp->qp_num, rc_qp->reconnect_count,
			rc_qp->send_posted, rc_qp->recv_pool.posted);
	}
	psm3_verbs_free_rc_qp_if_empty("start drain", rc_qp);
	return err;
}

// free the rc_qp and corresponding recv_pool
// in rare cases of errors draining QP (such as unable to move to IBV_QPS_ERR,
// the QP might not yet be drained and ! rc_qp->draining
void psm3_verbs_free_rc_qp(const char *why, struct psm3_verbs_rc_qp *rc_qp)
{
	_HFI_CONNDBG("[ipsaddr %p] free RC QP %u rcnt %u posted %u + %u: %s\n", rc_qp->ipsaddr,
			rc_qp->qp?rc_qp->qp->qp_num:0, rc_qp->reconnect_count,
			rc_qp->send_posted, rc_qp->recv_pool.posted, why);
	if (rc_qp->draining)
		rc_qp->ipsaddr->epaddr.proto->ep->verbs_ep.send_drain_outstanding--;
	if (rc_qp->ipsaddr) {
		ips_epaddr_t *ipsaddr = rc_qp->ipsaddr;
		// SLIST_REMOVE will walk list, but list is short
		SLIST_REMOVE(&ipsaddr->verbs.rc_qps, rc_qp, psm3_verbs_rc_qp, next);
	}
	// ep is one of 1st things set in alloc_recv_pool so use it to
	// decide if appropriate to call free_recv_pool.
	if (rc_qp->recv_pool.ep)
		psm_verbs_free_recv_pool(&rc_qp->recv_pool);
	if (rc_qp->qp)
		rc_qp_destroy(rc_qp->qp);
	psmi_free(rc_qp);
}

void psm3_verbs_free_rc_qp_if_empty(const char *why, struct psm3_verbs_rc_qp *rc_qp)
{
	if (rc_qp->send_posted + rc_qp->recv_pool.posted == 0)
		psm3_verbs_free_rc_qp(why, rc_qp);
}

psm2_error_t psm3_verbs_create_rc_qp(psm2_ep_t ep, ips_epaddr_t *ipsaddr)
{
	struct psm3_verbs_rc_qp *rc_qp;
	struct ibv_qp_cap qp_cap;
	psm2_error_t err = PSM2_OK;

	rc_qp = (struct psm3_verbs_rc_qp *)psmi_calloc(ep, PER_PEER_ENDPOINT,
					sizeof(struct psm3_verbs_rc_qp), 1);
	if (! rc_qp) {
		_HFI_ERROR("unable to allocate RC QP info: No Memory\n");
		return PSM2_NO_MEMORY;
	}

	rc_qp->qp = rc_qp_create(ep, ipsaddr, &qp_cap);
	if (! rc_qp->qp) {
		// _HFI_ERROR already output
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	// these should be the same as prior QP, but to be safe
	// update them.  max_recv_wr controls sizing of recv_pool which
	// will be allocated for this new rc_qp and max_inline_data
	// controls future send WQEs placed on this new rc_qp
	ipsaddr->verbs.rc_qp_max_recv_wr = qp_cap.max_recv_wr;
	ipsaddr->verbs.rc_qp_max_inline_data = qp_cap.max_inline_data;

	rc_qp->ipsaddr = ipsaddr;
	rc_qp->reconnect_count = ipsaddr->reconnect_count;
	SLIST_INSERT_HEAD(&ipsaddr->verbs.rc_qps, rc_qp, next);
	return PSM2_OK;

fail:
	psm3_verbs_free_rc_qp("failed RC QP create", rc_qp);
	return err;
}

struct psm3_verbs_rc_qp *psm3_verbs_active_rc_qp(ips_epaddr_t *ipsaddr)
{
	struct psm3_verbs_rc_qp *rc_qp =
		SLIST_FIRST(&ipsaddr->verbs.rc_qps);
	psmi_assert(rc_qp);
	psmi_assert(! rc_qp->draining);
	return rc_qp;
}

psm2_error_t psm3_verbs_have_rc_qp(ips_epaddr_t *ipsaddr, uint8_t reconnect_count)
{
	struct psm3_verbs_rc_qp *rc_qp;
	SLIST_FOREACH(rc_qp, &ipsaddr->verbs.rc_qps, next) {
		if (rc_qp->reconnect_count == reconnect_count) {
			_HFI_CONNDBG("[ipsaddr %p] have RC QP %u draining = %u posted %u + %u\n",
				ipsaddr, rc_qp->qp->qp_num, rc_qp->draining,
				rc_qp->send_posted, rc_qp->recv_pool.posted);
			return 1;
		}
	}
	return 0;
}

#ifdef PSM_RC_RECONNECT_SRQ
struct psm3_verbs_rc_qp *psm3_verbs_lookup_rc_qp(ips_epaddr_t *ipsaddr,
			uint32_t qp_num)
{
	struct psm3_verbs_rc_qp *rc_qp;
	SLIST_FOREACH(rc_qp, &ipsaddr->verbs.rc_qps, next) {
		if (rc_qp->qp->qp_num == qp_num) {
			_HFI_CONNDBG("[ipsaddr %p] have RC QP %u draining = %u posted %u + %u\n",
				ipsaddr, rc_qp->qp->qp_num, rc_qp->draining,
				rc_qp->send_posted, rc_qp->recv_pool.posted);
			return rc_qp;
		}
	}
	_HFI_CONNDBG("[ipsaddr %p] don't have RC QP %u\n", ipsaddr, qp_num);
	return NULL;
}
#endif // PSM_RC_RECONNECT_SRQ
#endif // PSM_RC_RECONNECT
#endif // USE_RC

/* RDMA mode */
// we need this early when setting defaults for RV thresholds in psm3_mq_malloc
// and also want this available when creating the verbs_ep since it may affect
// sizing of CQs and buffers.  But during psm3_mq_malloc we don't have an ep
// or proto to save this into.
// The value returned is a bitmask of IPS_PROTOEXP_FLAG_* selections
// When reload==1, we refetch the env variable and reload the cached value
// While this can also be used to set additional flags (TID_DEBUG,
// RTS_CTS_INTERLEAVE and CTS_SERIALIZED), it should not.
// TID_DEBUG and CTS_SERIALIZED are automatically set when appropriate,
// and there is an env variable for RTS_CTS_INTERLEAVE.
unsigned psm3_verbs_parse_rdmamode(int reload)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;
	unsigned default_value;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (!reload && have_value)
		return saved;

	psmi_assert(psmi_hal_has_cap(PSM_HAL_CAP_RDMA));
	// Note extra flags such as IPS_PROTOEXP_FLAGS_SERIALIZE and
	// IPS_PROTOEXP_FLAGS_INTERLEAVE are N/A when RDMA not enabled

	default_value = 0;
#ifdef RNDV_MOD
	if (psm3_rv_available()) {
		default_value = IPS_PROTOEXP_FLAG_RDMA_KERNEL;
	}
#ifdef PSM_HAVE_GPU
	// GPUDIRECT causes default_value of RDMA=1
	if (PSM3_GPU_IS_ENABLED && psmi_parse_gpudirect())
		default_value = IPS_PROTOEXP_FLAG_RDMA_KERNEL;
#endif
#endif

	psm3_getenv("PSM3_RDMA",
		    "RDMA proto control (0-no RDMA,"
#ifdef RNDV_MOD
			" 1-kernel RDMA,"
#endif
#ifdef USE_RC
			" 2-user RDMA, 3-user RC send/RDMA) "
#endif
			//" additional flags: 8-interleave, 0x10-serialize"
			// IPS_PROTOEXP_FLAG_TID_DEBUG (0x4)      N/A
			,
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		    (union psmi_envvar_val)default_value,
		    &envval);
#ifndef USE_RC
	if (IPS_PROTOEXP_FLAG_USER_RC_QP(envval.e_uint)) {
		static int logged = 0;
		if (! logged) {
			_HFI_INFO("WARNING: PSM built without user RC QP support enabled, RDMA modes %d and %d unavailable\n", IPS_PROTOEXP_FLAG_RDMA_USER, IPS_PROTOEXP_FLAG_RDMA_USER_RC);
			logged = 1;
		}
		envval.e_uint = 0;
	}
#endif
#ifndef RNDV_MOD
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(envval.e_uint)) {
		static int logged = 0;
		if (! logged) {
			_HFI_INFO("WARNING: PSM built without rv module enabled, RDMA mode %d unavailable\n", IPS_PROTOEXP_FLAG_RDMA_KERNEL);
			logged = 1;
		}
		envval.e_uint = 0;
	}
#endif
	saved = envval.e_uint;
	have_value = 1;
	return saved;
}

/* MR cache mode */
// We need this early since it will affect if the HAL opens the rv module.
// The value is a MR_CACHE_MODE_* selection
unsigned psm3_verbs_parse_mr_cache_mode(unsigned rdmamode, int reload)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;

	// Note for MR_CACHE_MODE_NONE, if RDMA is enabled we
	// still create a mr_cache to reference count MRs.
	// However if MR_CACHE_MODE_NONE is chosen, we will disable SDMA.
	// In the rare case where GPUDIRECT is enabled, all RDMA is disabled
	// and all SendDMA is disabled (eg. GDRCopy only) we still create a
	// mr_cache (KERNEL mode) but won't use it.
	// PSM_HAL_CAP_GPUDIRECT_* flags not known until after HAL device open,
	// so we test SDMA and RDMA here as prereqs for GPUDIRECT_SDMA and RDMA.
	if (! (rdmamode & IPS_PROTOEXP_FLAG_ENABLED)
#ifdef PSM_HAVE_GPU
		&& (! PSM3_GPU_IS_ENABLED || ! psmi_parse_gpudirect()
			//verbs always has these HAL capabilities set
			//|| (!psmi_hal_has_cap(PSM_HAL_CAP_SDMA)
			//	&& !psmi_hal_has_cap(PSM_HAL_CAP_RDMA)))
			)
#endif
		&& ! psm3_parse_senddma()) {
		envval.e_uint = MR_CACHE_MODE_NONE;
	} else if (IPS_PROTOEXP_FLAG_KERNEL_QP(rdmamode)) {
		// RDMA enabled in kernel mode.  Must use rv MR cache
		envval.e_uint = MR_CACHE_MODE_RV;
#ifdef PSM_HAVE_GPU
#ifdef PSM_HAVE_RNDV_MOD
	} else if (PSM3_GPU_IS_ENABLED && psmi_parse_gpudirect()) {
		// GPU Direct (RDMA, send DMA and/or gdrcopy) must
		// use kernel MR cache in RV
		envval.e_uint = MR_CACHE_MODE_KERNEL;
#endif
#endif
	} else if (!reload && have_value) {
		envval.e_uint = saved;
	} else {
		/* Behavior of user space MR Cache
		 * when 0, we merely share MRs for concurrently used buffers
		 */
		// user space MR cache w/o invalidate purposely not documented
		psm3_getenv("PSM3_MR_CACHE_MODE",
				"Enable MR caching 0=user space MR no cache"
#ifdef UMR_CACHE
#ifdef RNDV_MOD
				", 1=kernel MR w/cache, 2=user MR w/cache [1]",
#else
				", 2=user MR w/cache [0]",
#endif
#else /* UMR_CACHE */
#ifdef RNDV_MOD
				", 1=kernel MR w/cache [1]",
#else
				" [0]",
#endif
#endif /* UMR_CACHE */
				PSMI_ENVVAR_LEVEL_USER,
				PSMI_ENVVAR_TYPE_UINT,
#ifdef RNDV_MOD
				(union psmi_envvar_val)MR_CACHE_MODE_KERNEL,
#else
				(union psmi_envvar_val)MR_CACHE_MODE_NONE,
#endif
				 &envval);
		if (! MR_CACHE_MODE_VALID(envval.e_uint)
			|| envval.e_uint == MR_CACHE_MODE_RV)
			envval.e_uint = MR_CACHE_MODE_NONE;
	}
#ifndef RNDV_MOD
	if (envval.e_uint == MR_CACHE_MODE_KERNEL) {
		static int logged = 0;
		if (! logged) {
			_HFI_INFO("WARNING: PSM built without rv module enabled, kernel MR caching unavailable\n");
			logged = 1;
		}
		envval.e_uint = MR_CACHE_MODE_NONE;
	}
#endif
	saved = envval.e_uint;
	have_value = 1;
	return saved;
}

/******************************************************************************
 *  * Try to map verbs' link layer types to a descriptive string or "Unknown"
 *   ******************************************************************************/
static const char *link_layer_str(int8_t link_layer)
{
    switch (link_layer) {

        case IBV_LINK_LAYER_UNSPECIFIED:
        case IBV_LINK_LAYER_INFINIBAND:
            return "IB";
        case IBV_LINK_LAYER_ETHERNET:
            return "Ethernet";
        default:
            return "Unknown";
    }
}

void
psm3_dump_verbs_ep(psm2_ep_t ep, unsigned igid)
{
	struct psm3_verbs_ep *vep = &(ep->verbs_ep);
	union ibv_gid gid;
	int err;

	printf("ib_devname = %s\n", ep->dev_name);
	printf("qp_num     = %u\n", vep->qp->qp_num);
	printf("GID        = ");

	err = ibv_query_gid(vep->context, ep->portnum, igid, &gid);
	if_pf (err) {
		printf("unable to query GID: %s\n", strerror(err));
		return;
	}

	printf("%s\n", psm3_ibv_gid_fmt(gid, 0));
}

void
psm3_dump_verbs_qp(struct ibv_qp *qp)
{
	struct ibv_qp_attr attr;
	struct ibv_qp_init_attr init_attr;
	int mask = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_CAP
			/*| IBV_QP_RATE_LIMIT*/ ;
	int err;

	if (qp->qp_type == IBV_QPT_RC) {
		mask |= IBV_QP_ACCESS_FLAGS | IBV_QP_AV | IBV_QP_PATH_MTU
				| IBV_QP_TIMEOUT | IBV_QP_RETRY_CNT | IBV_QP_RNR_RETRY
				| IBV_QP_RQ_PSN | IBV_QP_MAX_QP_RD_ATOMIC
				// | IBV_QP_ALT_PATH
				| IBV_QP_MIN_RNR_TIMER | IBV_QP_SQ_PSN
				| IBV_QP_MAX_DEST_RD_ATOMIC | IBV_QP_PATH_MIG_STATE
				| IBV_QP_DEST_QPN;
	} else {
		mask |= IBV_QP_QKEY;
	}

	err = ibv_query_qp(qp, &attr, mask, &init_attr);
	if (err) {
		printf("unable to query QP: %s\n", strerror(err));
		return;
	}

	// rate_limit field not available in some versions of verbs.h
	//printf("QP %p (%u), type %u state %u PkeyIndx %u Port %u rate %u draining %u\n",
	//		qp, qp->qp_num, qp->qp_type, attr.qp_state, attr.pkey_index,
	//		attr.port_num, attr.rate_limit, attr.sq_draining);
	printf("QP %p (%u), type %u state %u PkeyIndx %u Port %u draining %u\n",
			qp, qp->qp_num, qp->qp_type, attr.qp_state, attr.pkey_index,
			attr.port_num, attr.sq_draining);
	printf("  send: wr %u sge %u inline %u recv: wr %u sge %u\n",
			attr.cap.max_send_wr, attr.cap.max_send_sge, attr.cap.max_inline_data,
			attr.cap.max_recv_wr, attr.cap.max_recv_sge);
	printf("  context %p send_cq %p recv_cq %p srq %p sg_sig_all %u\n",
			init_attr.qp_context, init_attr.send_cq, init_attr.recv_cq,
			init_attr.srq, init_attr.sq_sig_all);
	if (qp->qp_type == IBV_QPT_RC) {
		printf("  mtu %u mig %u rq_psn %u sq_psn %u dest_qp %u access %u\n",
			attr.path_mtu, attr.path_mig_state, attr.rq_psn, attr.sq_psn,
			attr.dest_qp_num, attr.qp_access_flags);
		printf("  max_rd_atomic %u max_dest_rd_atomic %u\n",
			attr.max_rd_atomic, attr.max_dest_rd_atomic);
		printf("  min_rnr_timer %u timeout %u retry_cnt %u rnr_retry %u\n",
			attr.min_rnr_timer, attr.timeout, attr.retry_cnt, attr.rnr_retry);
		printf("  ah_attr:  port %u dlid %u sl %u src_path_bits %u rate %u global %u\n",
			attr.ah_attr.port_num, attr.ah_attr.dlid,
			attr.ah_attr.sl,
			attr.ah_attr.src_path_bits, attr.ah_attr.static_rate,
			attr.ah_attr.is_global);
		if (attr.ah_attr.is_global) {
			printf("           dgid: %s\n",
				psm3_ibv_gid_fmt(attr.ah_attr.grh.dgid, 0));
			printf("           flow %u sgid_idx %u hop %u tc %u\n",
				attr.ah_attr.grh.flow_label, attr.ah_attr.grh.sgid_index,
				attr.ah_attr.grh.hop_limit, attr.ah_attr.grh.traffic_class);
		}
		printf("  alt_ah_attr:  port %u dlid %u sl %u src_path_bits %u rate %u global %u\n",
			attr.alt_ah_attr.port_num, attr.alt_ah_attr.dlid,
			attr.alt_ah_attr.sl,
			attr.alt_ah_attr.src_path_bits, attr.alt_ah_attr.static_rate,
			attr.alt_ah_attr.is_global);
		if (attr.alt_ah_attr.is_global) {
			printf("              dgid: %s\n",
				psm3_ibv_gid_fmt(attr.alt_ah_attr.grh.dgid, 0));
			printf("              flow %u sgid_idx %u hop %u tc %u\n",
				attr.alt_ah_attr.grh.flow_label, attr.alt_ah_attr.grh.sgid_index,
				attr.alt_ah_attr.grh.hop_limit, attr.alt_ah_attr.grh.traffic_class);
		}
		printf("  alt pkey idx %u alt port %u alt timeout %u\n",
			attr.alt_pkey_index, attr.alt_port_num, attr.alt_timeout);
	} else {
			printf("qkey: 0x%x\n", attr.qkey);
	}
	return;
}

static enum psm3_ibv_rate verbs_get_rate(uint8_t width, uint8_t speed)
{
	switch (width) {
	case 1: /* 1x */
		switch (speed) {
		case 1: return PSM3_IBV_RATE_2_5_GBPS;
		case 2: return PSM3_IBV_RATE_5_GBPS;
		case 4: /* fall through */
		case 8: return PSM3_IBV_RATE_10_GBPS;
		case 16: return PSM3_IBV_RATE_14_GBPS;
		case 32: return PSM3_IBV_RATE_25_GBPS;
		case 64: return PSM3_IBV_RATE_50_GBPS;
		case 128: return PSM3_IBV_RATE_100_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM3_IBV_RATE_100_GBPS;
		}
	case 2: /* 4x */
		switch (speed) {
		case 1: return PSM3_IBV_RATE_10_GBPS;
		case 2: return PSM3_IBV_RATE_20_GBPS;
		case 4: /* fall through */
		case 8: return PSM3_IBV_RATE_40_GBPS;
		case 16: return PSM3_IBV_RATE_56_GBPS;
		case 32: return PSM3_IBV_RATE_100_GBPS;
		case 64: return PSM3_IBV_RATE_200_GBPS;
		case 128: return PSM3_IBV_RATE_400_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM3_IBV_RATE_100_GBPS;
		}
	case 4: /* 8x */
		switch (speed) {
		case 1: return PSM3_IBV_RATE_20_GBPS;
		case 2: return PSM3_IBV_RATE_40_GBPS;
		case 4: /* fall through */
		case 8: return PSM3_IBV_RATE_80_GBPS;
		case 16: return PSM3_IBV_RATE_112_GBPS;
		case 32: return PSM3_IBV_RATE_200_GBPS;
		case 64: return PSM3_IBV_RATE_400_GBPS;
		case 128: return PSM3_IBV_RATE_800_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM3_IBV_RATE_100_GBPS;
		}
	case 8: /* 12x */
		switch (speed) {
		case 1: return PSM3_IBV_RATE_30_GBPS;
		case 2: return PSM3_IBV_RATE_60_GBPS;
		case 4: /* fall through */
		case 8: return PSM3_IBV_RATE_120_GBPS;
		case 16: return PSM3_IBV_RATE_168_GBPS;
		case 32: return PSM3_IBV_RATE_300_GBPS;
		case 64: return PSM3_IBV_RATE_600_GBPS;
		case 128: return PSM3_IBV_RATE_1200_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM3_IBV_RATE_100_GBPS;
		}
	case 16: /* 2x */
		switch (speed) {
		case 1: return PSM3_IBV_RATE_5_GBPS;
		case 2: return PSM3_IBV_RATE_10_GBPS;
		case 4: /* fall through */
		case 8: return PSM3_IBV_RATE_20_GBPS;
		case 16: return PSM3_IBV_RATE_28_GBPS;
		case 32: return PSM3_IBV_RATE_50_GBPS;
		case 64: return PSM3_IBV_RATE_100_GBPS;
		case 128: return PSM3_IBV_RATE_200_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM3_IBV_RATE_100_GBPS;
		}
	default:
		_HFI_ERROR( "unknown link width 0x%x\n", width);
		return PSM3_IBV_RATE_100_GBPS;
	}
}
#endif // PSM_VERBS
