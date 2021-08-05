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

#include "psm_verbs_ep.h"

/*
 * EPIDs encode the basic information needed to establish
 * datagram traffic so that PSM connection establishment can
 * negotiate and exchange the rest.
 *
 * EPID includes: EPID format version, network address, queue ID within NIC
 */


#define PSMI_SL_DEFAULT 0
#define PSMI_SL_MIN	0
#define PSMI_SL_MAX	31
// IB/OPA:
// 0-2: ver = 3
// 3-7: spare
// 8-31: QPN
// 32-47: lid [note, IB & OPA100 only support 16 bit LIDs]
// 48-63: subnet prefix low 16 bits
#define PSMI_EPID_PACK_V3(lid, qpn, subnet_id) \
	(((((uint64_t)PSMI_EPID_V3)&0x7)<<0)	|								\
	 ((((uint64_t)qpn)&0xffffff)<<8)	|			  					\
	 ((((uint64_t)lid)&0xffff)<<32)			|								\
	 ((((uint64_t)subnet_id)&0xffff)<<48))
// Eth:
// 0-2: ver = 4
// 3-7: subnet (number of high bits in IP addr representing IP subnet)
// 8-31: UD QPN or UDP socket
// 32-63: IPv4 address
#define PSMI_EPID_PACK_V4(ip, qpn, subnet_bits) \
	(((((uint64_t)PSMI_EPID_V4)&0x7)<<0)	|							\
	 ((((uint64_t)subnet_bits)&0x1f)<<3)	|			  			\
	 ((((uint64_t)qpn)&0xffffff)<<8)	|			  					\
	 ((((uint64_t)ip)&0xffffffff)<<32))

// shm and self:
// 0-2: ver = 0
// 3: shm-only flag (1)
// 4-31: spare
// 32-63: pid
#define PSMI_EPID_PACK_SHM(process_id, shmbool) \
	(((((uint64_t)process_id)&0xffffffff)<<32)			|				\
	 ((((uint64_t)shmbool)&0x1)<<3)		|			  					\
	 ((((uint64_t)PSMI_EPID_VERSION_SHM)&0x7)<<0))

#define PSMI_EPID_GET_EPID_VERSION(epid)	(((epid)>>0)&0x7)
#define PSMI_EPID_GET_LID_V3(epid)          (((epid)>>32)&0xffff) // lid
#define PSMI_EPID_GET_LID_V4(epid)          (((epid)>>32)&0xffffffff) // ip
#define PSMI_EPID_GET_CONTEXT(epid)         (((epid)>>8)&0xffffff) // qpn/sock
#define PSMI_EPID_GET_SUBNET_ID_V3(epid)	(((epid)>>48)&0xffff)
#define PSMI_EPID_GET_SUBNET_ID_V4(epid)	(psmi_bit_count_to_mask(((epid)>>3)&0x1f) &  PSMI_EPID_GET_LID_V4(epid)) // subnetwork
#define PSMI_EPID_GET_SUBNET_ID(epid) ((PSMI_EPID_GET_EPID_VERSION(epid) == PSMI_EPID_V3) ? \
										(uint32_t)PSMI_EPID_GET_SUBNET_ID_V3(epid) \
										: (uint32_t)PSMI_EPID_GET_SUBNET_ID_V4(epid))
#define PSMI_EPID_CONTEXT_FMT				"%d"
#define PSMI_EPID_GET_CONTEXT_VAL(epid)		(int)PSMI_EPID_GET_CONTEXT(epid)

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
	struct psm2_verbs_ep verbs_ep;

	int unit_id;
	uint16_t portnum;
	uint16_t out_sl;
	// mtu is PSM payload allowed by local HW,
	// mtu may be further reduced via PSM3_MTU by ips_proto_init
	// for UD/UDP, mtu is reduced by PSM hdr size
	uint16_t mtu;		/* out_sl-->vl-->mtu in sysfs */
	uint16_t network_pkey;	      /**> Pkey */
	uint16_t network_pkey_index;  /**> Pkey index */
	int did_syslog;
	psm2_uuid_t uuid;
	uint16_t jkey;
	uint64_t service_id;	/* OPA service ID */
	psm2_path_res_t path_res_type;	/* Path resolution for endpoint */
	psm2_ep_errhandler_t errh;
	int devid_enabled[PTL_MAX_INIT];
	int memmode;		    /**> min, normal, large memory mode */

	uint32_t hfi_num_sendbufs;/**> Number of allocated send buffers */
	uint32_t hfi_num_descriptors;/** Number of allocated scb descriptors*/
	uint32_t hfi_num_send_wqes;/** Number of allocated SQ WQEs for send*/
	uint32_t hfi_num_send_rdma;/** Number of concurrent RDMA*/
	uint32_t hfi_send_reap_thresh;/** when to reap SQ compleitions*/
	uint32_t hfi_num_recv_wqes;/** Number of allocated RQ WQEs*/
	uint32_t hfi_num_recv_cqes;/** Number of allocated RQ CQEs*/
	uint8_t hfi_qp_timeout;/** RC QP timeout, IB enum */
	uint8_t hfi_qp_retry;/** RC QP retry limit */
	uint8_t rdmamode; /** PSM3_RDMA */
	uint8_t mr_cache_mode; /** PSM3_MR_CACHE_MODE */
	uint8_t rv_num_conn; /** PSM3_RV_QP_PER_CONN */
	uint32_t rv_mr_cache_size; /** PSM3_RV_MR_CACHE_SIZE */
	uint32_t rv_q_depth; /** PSM3_RV_Q_DEPTH */
	uint32_t rv_reconnect_timeout; /* PSM3_RV_RECONNECT_TIMEOUT */
	uint32_t rv_hb_interval; /* PSM3_RV_HEARTBEAT_INTERVAL */
	uint32_t hfi_imm_size;	  /** Immediate data size */
	uint32_t connections;	    /**> Number of connections */

	psmi_context_t context;
	char *context_mylabel;
	uint32_t yield_spin_cnt;

	/* EP link-lists */
	struct psm2_ep *user_ep_next;

	/* EP link-lists for multi-context. */
	struct psm2_ep *mctxt_prev;
	struct psm2_ep *mctxt_next;
	struct psm2_ep *mctxt_master;

	/* Active Message handler table */
	struct psm2_ep_am_handle_entry *am_htable;

	uint64_t gid_hi;
	uint64_t gid_lo;

	ptl_ctl_t ptl_amsh;
	ptl_ctl_t ptl_ips;
	ptl_ctl_t ptl_self;

	/* All ptl data is allocated inline below */
	uint8_t ptl_base_data[0] __attribute__ ((aligned(64)));
	bool skip_affinity;
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
		err = psmi_poll_internal(ep, 1);			\
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

#endif /* _PSMI_EP_H */
