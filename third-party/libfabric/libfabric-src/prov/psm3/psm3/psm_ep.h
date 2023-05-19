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

#ifndef _PSMI_IN_USER_H
#error psm2_ep.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_EP_H
#define _PSMI_EP_H

#if !defined(PSM_VERBS) && !defined(PSM_SOCKETS) && !defined(PSM_NONE)
#error "At least one of PSM_VERBS or PSM_SOCKETS must be defined"
#endif
#if defined(PSM_VERBS) && defined(PSM_SOCKETS) && defined(UMR_CACHE)
#error "UMR_CACHE not yet allowed with both PSM_VERBS and PSM_SOCKETS enabled"
#endif

#ifdef PSM_VERBS
#include "hal_verbs/verbs_ep.h"
#endif
#if defined(PSM_SOCKETS)
#include "hal_sockets/sockets_ep.h"
#endif

/* PSM3 wide limits for port numbers.  Individual HALs may be more restricted */
#define PSM3_NIC_MIN_PORT 1
#define PSM3_NIC_MAX_PORT 1 // TBD - should be >= 2
/* any unit id to match. */
#define PSM3_NIC_ANY ((long)-1)
/* any port num to match. */
#define PSM3_NIC_PORT_ANY ((long)0)


#define PSMI_SL_DEFAULT 0
#define PSMI_SL_MIN	0
#define PSMI_SL_MAX	31

#define PSM_MCTXT_APPEND(head, node)	\
	node->mctxt_prev = head->mctxt_prev; \
	node->mctxt_next = head; \
	head->mctxt_prev->mctxt_next = node; \
	head->mctxt_prev = node; \
	node->mctxt_master = head
#define PSM_MCTXT_REMOVE(node)	\
	node->mctxt_prev->mctxt_next = node->mctxt_next; \
	node->mctxt_next->mctxt_prev = node->mctxt_prev; \
	node->mctxt_next = node->mctxt_prev = node; \
	node->mctxt_master = NULL

struct psm2_ep {
	psm2_epid_t epid;	    /**> This endpoint's Endpoint ID */
	psm2_epaddr_t epaddr;	    /**> This ep's ep address */
	psm2_mq_t mq;		    /**> only 1 MQ */
	union { /* HAL specific device info, when ptl_ips enabled */
#ifdef PSM_VERBS
		struct psm3_verbs_ep verbs_ep;
#endif
#ifdef PSM_SOCKETS
		struct psm3_sockets_ep sockets_ep;
#endif
	};

	/* unit_id and portnum are set to 0 when ptl_ips not enabled */
	int unit_id;
	uint8_t portnum;
	uint8_t addr_index;
	uint16_t out_sl;
	// mtu is PSM payload allowed by local HW,
	// mtu may be further reduced via PSM3_MTU by ips_proto_init
	// for UD/UDP, mtu is reduced by PSM hdr size
	// for TCP, mtu may be larger than wire MTU (up to 65535*4)
	uint32_t mtu;		/* out_sl-->vl-->mtu in sysfs */
	uint16_t network_pkey;	      /**> Pkey */
	uint16_t network_pkey_index;  /**> Pkey index */
	int did_syslog;
	const char *dev_name;	/* just for logging */
	psm2_uuid_t uuid;
	uint16_t jkey;
	uint64_t service_id;	/* OPA service ID */
	psm2_path_res_t path_res_type;	/* Path resolution for endpoint */
	psm2_ep_errhandler_t errh;
	int devid_enabled[PTL_MAX_INIT];
	int memmode;		    /**> min, normal, large memory mode */

	/* Shared among PSM_VERBS and PSM_SOCKETS */
#ifdef PSM_HAVE_RNDV_MOD
	psm3_rv_t rv;   // rendezvous module open handle
	uint32_t rv_mr_cache_size; /** PSM3_RV_MR_CACHE_SIZE */
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint32_t rv_gpu_cache_size; /** PSM3_RV_GPU_CACHE_SIZE */
#endif
#endif /* PSM_HAVE_RNDV_MOD */
	uint32_t hfi_num_sendbufs;/**> Number of allocated send buffers */
	uint32_t hfi_num_descriptors;/** Number of allocated scb descriptors*/
#ifdef PSM_HAVE_REG_MR
	uint32_t hfi_num_send_rdma;/** Number of concurrent RDMA*/
#endif
#ifdef PSM_ONEAPI
	int ze_ipc_socket;
	char *listen_sockname;
#endif
	uint8_t wiremode; /* EPID protocol specific basic modes
			   * For RoCE/IB reflects
			   * rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK
			   * Others currently only have 1 mode.
			   * Note UDP vs TCP are separate EPID protocols
			   */
	uint8_t rdmamode; /* PSM3_RDMA */
#ifdef PSM_HAVE_REG_MR
	/* per EP information needed to create verbs MR cache */
	uint8_t mr_cache_mode; /** PSM3_MR_CACHE_MODE */
	uint8_t mr_access; /** PSM3_MR_ACCESS */
#ifdef PSM_HAVE_RNDV_MOD
	int cmd_fd;
#endif
	struct ibv_pd *pd;
#endif
	uint32_t hfi_imm_size;	  /** Immediate data size */
	uint32_t connections;	    /**> Number of connections */

	/* HAL indicates send segmentation support (OPA Send DMA or UDP GSO)
	 * by setting max_segs>1 and max_size > 1 MTU.
	 * chunk_size used will be min(chunk_max_segs*frag_size, chunk_max_size)
	 * Can set 1 huge and other reasonable if want only 1 to control
	 * segmentation size.
	 * Note MTU (frag_size) may be negotiated down per conn (but not up).
	 */
	uint16_t chunk_max_segs;	/* max fragments in 1 HAL send call */
	uint32_t chunk_max_size;	/* max payload in 1 HAL send call */
	char *context_mylabel;
	uint32_t yield_spin_cnt;

	/* EP link-lists */
	struct psm2_ep *user_ep_next;

	/* EP link-lists for multi-context (multi-rail and multi-QP). */
	struct psm2_ep *mctxt_prev;
	struct psm2_ep *mctxt_next;
	struct psm2_ep *mctxt_master;

	/* Active Message handler table */
	struct psm2_ep_am_handle_entry *am_htable;

	uint8_t addr_fmt;	// one of PSMI_ADDR_FMT_*, always set

	// these values are as reported by get_port_subnet
		// subnet, addr and gid only defined when ptl_ips enabled
		// subnet and addr must have same addr_fmt and prefix_len
	psmi_subnet128_t subnet;
	psmi_naddr128_t addr;
	psmi_gid128_t gid;

	ptl_ctl_t ptl_amsh;
	ptl_ctl_t ptl_ips;
	ptl_ctl_t ptl_self;

	bool skip_affinity;

	/* All ptl data is allocated inline below */
	uint8_t ptl_base_data[0] __attribute__ ((aligned(64)));
		// NO FIELDS CAN BE PLACED HERE
};

struct mqq {
	psm2_mq_req_t first;
	psm2_mq_req_t last;
};

typedef
union psmi_seqnum {
	struct {
		uint32_t psn_seq:11;
		uint32_t psn_gen:20;
	};
	struct {
		uint32_t psn_num:31;
	};
	uint32_t psn_val;
} psmi_seqnum_t;

/*
 * PSM end point address. One per connection and per rail.
 */
struct psm2_epaddr {
	psm2_epid_t epid;	/* peer's epid */
	ptl_ctl_t *ptlctl;	/* The control structure for the ptl */
	struct ips_proto *proto;	/* only for ips protocol */
	void *usr_ep_ctxt;	/* User context associated with endpoint */
};

#ifndef PSMI_BLOCKUNTIL_POLLS_BEFORE_YIELD
#  define PSMI_BLOCKUNTIL_POLLS_BEFORE_YIELD  250
#endif

/*
 * Users of BLOCKUNTIL should check the value of err upon return
 */
#define PSMI_BLOCKUNTIL(ep, err, cond)	do {				\
	int spin_cnt = 0;						\
	PSMI_PROFILE_BLOCK();						\
	while (!(cond)) {						\
		err = psm3_poll_internal(ep, 1, 0);			\
		if (err == PSM2_OK_NO_PROGRESS) {			\
			PSMI_PROFILE_REBLOCK(1);			\
			if (++spin_cnt == (ep)->yield_spin_cnt) {	\
				spin_cnt = 0;				\
				PSMI_YIELD((ep)->mq->progress_lock);	\
			}						\
		}							\
		else if (err == PSM2_OK) {				\
			PSMI_PROFILE_REBLOCK(0);			\
			spin_cnt = 0;					\
		}							\
		else							\
		break;							\
	}								\
	PSMI_PROFILE_UNBLOCK();						\
} while (0)

psm2_error_t psm3_parse_devices(int devices[PTL_MAX_INIT]);
int psm3_device_is_enabled(const int devices[PTL_MAX_INIT], int devid);

#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
extern int64_t psm3_gpu_evict_some(psm2_ep_t ep, uint64_t length, int access);
#endif
#endif

#endif /* _PSMI_EP_H */
