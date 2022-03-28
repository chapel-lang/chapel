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
#error psm_verbs_ep.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_VERBS_EP_H
#define _PSMI_VERBS_EP_H

#include <infiniband/verbs.h>
#ifdef RNDV_MOD
#include <psm_rndv_mod.h>
#endif
#include "ptl_ips/ips_path_rec.h"

#define MAX_PSM_HEADER 64			// sizeof(ips_lrh) == 56, round up to 64

// defaults, these are reconfigurable with:
// PSM3_SEND_IMMEDIATE_SIZE
// PSM3_NUM_SEND_WQES
// PSM3_NUM_RECV_WQES
// PSM3_QP_TIMEOUT
// PSM3_QP_RETRY
#define VERBS_SEND_MAX_INLINE 64	// 56 is PSM header size
#define VERBS_SEND_QP_ENTRIES 4080	// will round down to multiple of COALLESCE
#define VERBS_NUM_SEND_RDMA    128	// max conurrent RDMA send WQEs per NIC
#define VERBS_RECV_QP_ENTRIES 4095	// avoid CQ overflow, CVL may be limited to 4095?
#define VERBS_QP_TIMEOUT 536870	// in microseconds (17)
#define VERBS_QP_RETRY 7	// limit on RC QP retries for rnr or timeout
#define VERBS_QP_MAX_RETRY 7	// max allowed by verbs for QP_RETRY

// hardcoded for now
#define VERBS_RECV_QP_FRACTION 4	// size RC QPs as 1/FRACTION of the
									// final UD RECV QP size
		// only ask for a completion this often.
		// If 1, ask for completion on every send.
#define VERBS_SEND_CQ_COALLESCE 8
									// For USE_RC, keep this modest as we
									// could have up to this many -1 unsignaled
									// WQEs per QP, which may consume send bufs
									// for quite some time if the QP is only
									// occasionally used
									// if we have ~100 QPs and 1000s of send
									// buffers, this should be ok
#define VERBS_RECV_QP_COALLESCE 16	// gather and build this many recv WQEs
									// before post on recv Q.
									// Reduces verbs calls
									// if 1, post as we recv them
#define VERBS_SEND_CQ_REAP 256	// check for completions when this many unreaped
#define VERBS_PORT 1			// default port if not specified
#define VERBS_RECV_CQE_BATCH 1	// how many CQEs to ask for at a time
#define UD_ADDITION (40)		// extra bytes at start of UD recv buffer
								// defined in verbs API to accomidate IB GRH
#define BUFFER_HEADROOM 0		// how much extra to allocate in buffers
								// as a paranoid headroom for use of more than
								// intended.  Was 64, but seems we can do
								// without it and hence make buffers better
								// page aligned
								// value here should be a multiple of CPU
								// cache size
#define CPU_PAGE_ALIGN	PSMI_PAGESIZE	// boundary to align buffer pools for
#include "psm_verbs_mr.h"

// some older distros lack some of the rates, so define our own list here
enum psm_ibv_rate {
	//PSM_IBV_RATE_MAX		= 0,
	PSM_IBV_RATE_2_5_GBPS	= 2,
	PSM_IBV_RATE_5_GBPS		= 5,
	PSM_IBV_RATE_10_GBPS	= 3,
	PSM_IBV_RATE_20_GBPS	= 6,
	PSM_IBV_RATE_30_GBPS	= 4,
	PSM_IBV_RATE_40_GBPS	= 7,
	PSM_IBV_RATE_60_GBPS	= 8,
	PSM_IBV_RATE_80_GBPS	= 9,
	PSM_IBV_RATE_120_GBPS	= 10,
	PSM_IBV_RATE_14_GBPS	= 11,
	PSM_IBV_RATE_56_GBPS	= 12,
	PSM_IBV_RATE_112_GBPS	= 13,
	PSM_IBV_RATE_168_GBPS	= 14,
	PSM_IBV_RATE_25_GBPS	= 15,
	PSM_IBV_RATE_100_GBPS	= 16,
	PSM_IBV_RATE_200_GBPS	= 17,
	PSM_IBV_RATE_300_GBPS	= 18,
	PSM_IBV_RATE_28_GBPS	= 19,
	PSM_IBV_RATE_50_GBPS	= 20,
	PSM_IBV_RATE_400_GBPS	= 21,
	PSM_IBV_RATE_600_GBPS	= 22,
};

// Per IBTA the wc.opcode is undefined in error CQEs
// so we need to save that information in the wr_id.
// Fortunately our wr_id's are well aligned pointers so
// we can stash the flag in the low bits of wr_id
#define VERBS_SQ_WR_ID_SEND		0x0
#define VERBS_SQ_WR_ID_RDMA_WRITE	0x1
#define VERBS_SQ_WR_ID_MASK		0x1
#define VERBS_SQ_WR_OP(wr_id)		((wr_id)&VERBS_SQ_WR_ID_MASK)
#define VERBS_SQ_WR_OP_STR(wr_id) (VERBS_SQ_WR_OP(wr_id)?"RDMA Write":"Send")

struct verbs_sbuf {
	struct verbs_sbuf *next;
	uint8_t *buffer;
	struct psm2_verbs_send_allocator *allocator;
};
typedef struct verbs_sbuf *sbuf_t;
#define sbuf_to_buffer(buf)	((buf)->buffer)
#define sbuf_pool(ep, buf)	((buf)->allocator->pool)
#define sbuf_lkey(ep, buf)	(sbuf_pool(ep, buf)->send_buffer_mr->lkey)


// when we get a CQE we need to find the pool and the QP it came from
// (pool has a reference to the qp).
// unfortunately, the CQE has a qp_num but not a ibv_qp pointer.  So we need
// to keep this information here and use this structure as the wr_id for our
// RQ WQE.
struct verbs_rbuf {
	uint8_t *buffer;
	struct psm2_verbs_recv_pool *pool;;
};
typedef struct verbs_rbuf *rbuf_t;
#define rbuf_to_buffer(buf)	((buf)->buffer)
#define rbuf_addition(buf) ((buf)->pool->addition)
#define rbuf_qp(ep, buf) ((buf)->pool->qp)

static inline const char*qp_type_str(struct ibv_qp *qp) {
	return (qp->qp_type == IBV_QPT_UD)?"UD":"RC";
}

// subset of RC QP attr which we need to exchange in PSM req/rep
// when ! defined(USE_RC), it is zeroed to keep req/rep size consistent
// this structure is also used in REQ/REP packet format and size can't change
// list of fields comes from IB CM for RC QP connection
// These fields are purely information about sender:
//		qpn, srq, target_ack_delay
// These fields are negotiated.
// 		mtu, responder_resources, initiator_depth
//		Each side sends their best possible value and the receiver picks
//		the min of it's own best and the REQ/REP received
struct psm_rc_qp_attr {
	uint32_t qpn:24;
	uint32_t mtu:4;  // HW MTU for RC QP
	uint32_t srq:1;   // using SRQ
	uint32_t resv:3;
	uint8_t target_ack_delay:5; // 5 bits for computing timeout - TBD if need
	uint8_t resv2:3;
	// these control how many concurrent RDMA reads/atomics are allowed per QP
	// the initiator of the RDMA reads must issue no more than target can handle
	// can be 0 if we don't plan to use RDMA read
	// behavior here is based on PSM CM approach, which differs from IB CM
	// IB CM REP would have the result of the negotiated value
	// for PSM CM, sender puts same values in REQ and REP
	// receiver will use the min of its preferred value and the received value
	// sent REQ/REP indicates what we desire to use from sender perspective
	//   responder_resources <= local CA max_qp_rd_atom
	//   initiator_depth <= local CA max_qp_init_rd_atom
	// REQ/REP recipient sets values as follows:
	//   QP max_rd_atomic = MIN(our requested initiator_depth,
	//   							received responder_resouces)
	//   QP max_dest_rd_atomic = MIN(our requested responder_resources,
	//   							received initiator_depth)
	// ibv_device_attr:
	//   CA max_qp_rd_atom - max incoming RDMA Reads (responder)
	//   CA max_qp_init_rd_atom -max outstanding outgoing RDMA Reads (initiator)
	// ibv_qp_attr:
	//   QP max_dest_rd_atomic - max incoming RDMA Reads (responder)
	//   QP max_rd_atomic - max outstanding outgoing RDMA Reads (initiator)
	uint8_t responder_resources;
	uint8_t initiator_depth;
	// QKey well known
	// starting PSN - use initpsn in req/rep
	// retry_cnt,rnr_retry_cnt - well known
	// pkey - already known
	// LID, GID, SL, etc - already known, same as UD QP
	uint8_t resv3[17];	// future expansion, keeping struct mult of 64b
} PACK_SUFFIX;

// pool of send buffers
// When USE_RC we allow multiple QPs to be to share the same send buffer pool.
struct psm2_verbs_send_pool {
	// our preregistered send buffers
	uint32_t send_buffer_size;
	uint32_t send_total;
	uint32_t send_num_free;
	uint8_t *send_buffers;				// aligned buffers for use
	struct verbs_sbuf *send_bufs;
	struct verbs_sbuf *send_free;	// head of free list
	struct ibv_mr *send_buffer_mr;
};
typedef struct psm2_verbs_send_pool *psm2_verbs_send_pool_t;

// track the list of allocated (aka inflight) send buffers so we
// can coalesce completions and still find all the completed buffers
// For USE_RC, we need an inflight list per QP to account for the unpredictable
// order of send CQEs from different QPs.
struct psm2_verbs_send_allocator {
	psm2_verbs_send_pool_t pool;		// pool we allocate from
	struct verbs_sbuf *send_alloc_head;	// head of allocated list
	struct verbs_sbuf *send_alloc_end;	// end of allocated list
	uint32_t send_num_til_coallesce;
};
typedef struct psm2_verbs_send_allocator *psm2_verbs_send_allocator_t;

// receive buffer pool
// we use the same basic mechanisms for UD and RC QP buffer pools
// but sizes may differ
// when USE_RC, we need a separate recv pool per QP so we can prepost bufs.
struct psm2_verbs_recv_pool {
	struct ibv_qp *qp;	// secondary reference to QP these buffers are for
	psm2_ep_t ep;
	// our preregistered recv buffers
	uint32_t recv_buffer_size;
	uint32_t recv_total;
	uint8_t *recv_buffers;
	struct ibv_mr *recv_buffer_mr;
	uint32_t addition;	// UD_ADDITION for UD QP, 0 for RC QP
#if VERBS_RECV_QP_COALLESCE > 1
			// list of ready to post WQEs and SGEs
	struct ibv_recv_wr recv_wr_list[VERBS_RECV_QP_COALLESCE];
	struct ibv_sge recv_sge_list[VERBS_RECV_QP_COALLESCE];
	uint32_t next_recv_wqe;	// next index in rsc_wr_list/sge_list to use
#endif
	struct verbs_rbuf *recv_bufs;
};
typedef struct psm2_verbs_recv_pool *psm2_verbs_recv_pool_t;

// this structure can be part of psm2_ep
// one instance of this per local end point (NIC)
// we will create a single PD and UD QP with related resources to
// permit an eager data movement mechanism
// conceptually similar to a psmi_context_t which refers to an HFI context
// TODO - later could optimize cache hit rates by putting some of the less
// frequently used fields in a different part of psm2_ep struct
struct psm2_verbs_ep {
	//struct ibv_device *ib_dev;
	struct ibv_context *context;
	struct ibv_port_attr port_attr;
	struct ibv_pd	*pd;
	struct ibv_comp_channel *recv_comp_channel;
	union  ibv_gid lgid;  // The GID to use when sending.
	unsigned lgid_index; 
	struct ibv_cq	*send_cq;
	struct ibv_cq	*recv_cq;
	struct ibv_qp	*qp;
	struct ibv_qp_cap qp_cap;   // capabilities of QP we got
	uint32_t qkey;
	uint8_t link_layer;         // IBV_LINK_LAYER_ETHERNET or other
	uint8_t active_rate;
	uint32_t ip_addr;           // ip_addr (valid for link_layer == Eth)
	uint32_t ip_netmask;        // netmask (valid for link_layer == Eth)
	struct psm2_verbs_send_pool send_pool;
	struct psm2_verbs_send_allocator send_allocator;
	uint32_t send_rdma_outstanding;	// number of outstanding RDMAs
	uint32_t send_reap_thresh;	// TBD if should be here or in pool
	struct psm2_verbs_recv_pool recv_pool;
#if VERBS_RECV_CQE_BATCH > 1
	struct ibv_wc recv_wc_list[VERBS_RECV_CQE_BATCH];
	int recv_wc_count;	// number left in recv_wc_list
	int recv_wc_next;	// next index
#else
	// if asked to revisit a packet we save it here
	rbuf_t revisit_buf;
	uint32_t revisit_payload_size;
#endif
#ifdef RNDV_MOD
	psm2_rv_t rv;	// rendezvous module open handle
	uint32_t rv_index;
	struct psm2_rv_conn_stats rv_conn_stats;
	struct psm2_rv_event_stats rv_event_stats;
#endif
};

// given index, return buffer start
#define send_buffer_start(pool, i) ((pool)->send_buffer_size *(i))
// given buffer start, return index
#define send_buffer_index(pool, buf) (((buf)-(pool)->send_buffers)/(pool)->send_buffer_size)

// given index, return buffer start
#define recv_buffer_start(pool, i) ((pool)->recv_buffer_size *(i))
// given buffer start, return index
#define recv_buffer_index(pool, buf) (((buf)-(pool)->recv_buffers)/(pool)->recv_buffer_size)

extern psm2_error_t __psm2_ep_open_verbs(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key);
extern void __psm2_ep_initstats_verbs(psm2_ep_t ep);
extern void __psm2_ep_free_verbs(psm2_ep_t ep);
extern psm2_error_t __psm2_ep_initialize_queues(psm2_ep_t ep);
extern struct ibv_qp* rc_qp_create(psm2_ep_t ep, void *context,
							struct ibv_qp_cap *cap);
extern void rc_qp_destroy(struct ibv_qp *qp);
extern psm2_error_t modify_rc_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp);
extern psm2_error_t modify_rc_qp_to_rtr(psm2_ep_t ep, struct ibv_qp *qp,
				const struct psm_rc_qp_attr *req_attr,
				const ips_path_rec_t *path_rec, uint32_t initpsn);
extern psm2_error_t modify_rc_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp,
				const struct psm_rc_qp_attr *req_attr, uint32_t initpsn);
extern int __psm2_ep_poll_type(int poll_type, psm2_ep_t ep);
extern psm2_error_t psm_verbs_alloc_send_pool(psm2_ep_t ep, struct ibv_pd *pd,
            psm2_verbs_send_pool_t pool,
            uint32_t send_total, uint32_t send_buffer_size);
extern psm2_error_t psm_verbs_init_send_allocator(
            psm2_verbs_send_allocator_t allocator,
            psm2_verbs_send_pool_t pool);
extern psm2_error_t psm_verbs_alloc_recv_pool(psm2_ep_t ep, struct ibv_qp *qp,
            psm2_verbs_recv_pool_t pool,
            uint32_t recv_total, uint32_t recv_buffer_size);
extern void psm_verbs_free_send_pool(psm2_verbs_send_pool_t pool);
extern void psm_verbs_free_recv_pool(psm2_verbs_recv_pool_t pool);
extern sbuf_t __psm2_ep_verbs_alloc_sbuf(psm2_verbs_send_allocator_t allocator);
extern void __psm2_ep_verbs_free_sbuf(
				sbuf_t buf, uint32_t count);
extern psm2_error_t __psm2_ep_verbs_post_recv(
				rbuf_t buf);
extern psm2_error_t __psm2_ep_verbs_prepost_recv(psm2_verbs_recv_pool_t pool);

extern psm2_error_t psm2_verbs_post_rdma_write_immed(psm2_ep_t ep,
				struct ibv_qp *qp,
				void *loc_buf, struct psm2_verbs_mr *loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id);

#ifdef RNDV_MOD
extern psm2_error_t psm2_verbs_post_rv_rdma_write_immed(psm2_ep_t ep,
				psm2_rv_conn_t conn,
				void *loc_buf, struct psm2_verbs_mr *loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id,
				uint8_t *sconn_index, uint32_t *conn_count);
#endif

extern psm2_error_t psm2_verbs_completion_update(psm2_ep_t ep);

extern int __psm2_nonzero_gid(const union ibv_gid *gid);
extern char *__psm2_dump_gid(union ibv_gid *gid, char *buf, size_t bufsize);
extern void __psm2_dump_verbs_qp(struct ibv_qp *qp);
extern enum psm_ibv_rate min_rate(enum psm_ibv_rate a, enum psm_ibv_rate b);
#ifndef UD_SAMPLE
extern int verbs_get_port_index2pkey(psm2_ep_t ep, int port, int index);
#endif
#endif // _PSMI_VERBS_EP_H
