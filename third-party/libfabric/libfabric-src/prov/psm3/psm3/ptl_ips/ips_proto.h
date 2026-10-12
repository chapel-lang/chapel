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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#ifndef _IPS_PROTO_H
#define _IPS_PROTO_H

#include "ips_config.h"
#include "psm_user.h"

#include "ips_recvhdrq.h"
#include "ips_epstate.h"
#include "ips_proto_am.h"
#include "ips_tidflow.h"
#include "ips_path_rec.h"

#if defined(PSM_SOCKETS) && defined(USE_UDP)
// when defined, this enables use of byte based flow credits in addition
// to packet based.
// It can help UDP to avoid overflowing the sockets kernel buffers.
// However it adds 12 bytes of flow state and will ultmately affect PSM
// memory at scale.
// UD/RC, TCP and OPA HALs self configure so this has no effect
#define PSM_BYTE_FLOW_CREDITS
#endif

typedef enum ips_path_type {
	IPS_PATH_LOW_PRIORITY,
	IPS_PATH_NORMAL_PRIORITY,
	IPS_PATH_HIGH_PRIORITY,
	IPS_PATH_MAX_PRIORITY
} ips_path_type_t;

/*
 * Local Endpoint info.
 *
 * Contains information necessary for composing packets for the local endpoint
 * Most are direct copies from psm2_ep_t
 */
struct ips_epinfo {
	__be16 ep_base_lid;
	uint8_t ep_hash;	// for hashing adaptive dispersive routing
	uint8_t ep_lmc;
	enum psm3_ibv_rate ep_link_rate;
	uint16_t ep_sl;		/* PSM3_NIC_SL only when path record not used */
	uint32_t ep_mtu;	// PSM payload after potential hdr & PSM3_MTU adjustment
	                	// for TCP and RC it can be beyond wire size
	uint16_t ep_pkey;	/* PSM3_PKEY only when path record not used */
	uint64_t ep_timeout_ack;	/* PSM3_ERRCHK_TIMEOUT if no path record */
	uint64_t ep_timeout_ack_max;
	uint32_t ep_timeout_ack_factor;
};

/*
 * This contains a path record table table that Enumerate the paths available
 * between the local node and a remote node associated with an end point.
 * Also maintain a state value for each message priority that keeps indicates
 * which path should be assigned to the next message of that priority.
 *
 * For LMC/Torus, keep list of base and max dlid. Used for pkt verification
 *
 * pg_base_dlid and pg_base_slid are in network byte order.
 */
#define IPS_MAX_PATH_LMC 3
typedef struct ips_path_grp {
	__be16 pg_base_dlid;
	__be16 pg_base_slid;
	uint8_t pg_num_paths[IPS_PATH_MAX_PRIORITY];
	uint8_t pg_next_path[IPS_PATH_MAX_PRIORITY];
	ips_path_rec_t *pg_path[0][IPS_PATH_MAX_PRIORITY];
} ips_path_grp_t;

/*
 * Start and finish routines for constructing an ips_proto.
 */
struct ips_proto;
psm2_error_t psm3_ips_proto_init(psm2_ep_t ep,
			    const struct ptl *ptl,
			    int num_of_send_bufs,
			    int num_of_send_desc,
			    uint32_t imm_size,
			    const struct psmi_timer_ctrl *timerq, /* PTL's timerq */
			    const struct ips_epstate *epstate,	  /* PTL's epstate */
			    void *spioc,	                  /* PTL's opaque spio control */
			    struct ips_proto *proto);	          /* output protocol */

psm2_error_t psm3_ips_proto_disconnect_all(struct ips_proto *proto, int force,
			   uint64_t timeout);

psm2_error_t psm3_ips_proto_fini(struct ips_proto *proto);

/*
 * Control message structures
 *
 * ips low-level control messages to ensure reliability of eager packets.
 */
#define CTRL_MSG_QEUEUE_SIZE 64	/* power of two */

struct ips_ctrlq_elem {
	uint8_t message_type;
	uint16_t *msg_queue_mask;
	ips_scb_t msg_scb;
};

struct ips_ctrlq {
	/* Queued control messages, queued when pio is busy */
	struct ips_proto *ctrlq_proto;

	uint32_t ctrlq_head;
	uint32_t ctrlq_tail;
	//uint32_t ctrlq_overflow;

	struct ips_ctrlq_elem ctrlq_cqe[CTRL_MSG_QEUEUE_SIZE] PSMI_CACHEALIGN;
	struct psmi_timer ctrlq_timer;	/* when in timerq */
};

/* Connect/disconnect, as implemented by ips */

/*
 * Connections are not pairwise but we keep a single 'epaddr' for messages-from
 * and messages-to a remote 'epaddr'.  State transitions for connecting TO and
 * FROM 'epaddrs' are the following:
 * Connect TO (Connect OUTGOING):
 *   NONE -> WAITING -> ESTABLISHED -> WAITING_DISC -> DISCONNECTED -> NONE
 * for HALs which may need a reconnect (for user space RC QPs), this
 * sequence may also occur within the above sequence as many times as needed:
 * ESTABLISHED -> WAITING -> ESTABLISHED
 *
 * Connect FROM (we receive a connect request - Connect INCOMING)
 *   NONE -> ESTABLISHED -> DISCONNECTED -> NONE
 * for HALs which may need a reconnect (for user space RC QPs), this
 * sequence may also occur within the above sequence as many times as needed:
 * ESTABLISHED -> NEED_RECONNECT -> ESTABLISHED
 */
#define CSTATE_ESTABLISHED		1
#define CSTATE_NONE			2
#define CSTATE_DISCONNECTED		3
#define CSTATE_OUTGOING_WAITING		4
#define CSTATE_OUTGOING_WAITING_DISC	5
#define CSTATE_INCOMING_NEED_RECONNECT	6

psm2_error_t psm3_ips_proto_connect(struct ips_proto *proto, int numep,
			      const psm2_epid_t *array_of_epid,
			      const int *array_of_epid_mask,
			      psm2_error_t *array_of_errors,
			      psm2_epaddr_t *array_of_epaddr,
			      uint64_t timeout_in);

psm2_error_t psm3_ips_proto_disconnect(struct ips_proto *proto, int force, int numep,
				 psm2_epaddr_t array_of_epaddr[],
				 const int array_of_epaddr_mask[],
				 psm2_error_t array_of_errors[],
				 uint64_t timeout_in);

psm2_error_t psm3_ips_proto_connection_error(ips_epaddr_t *ipsaddr,
		const char *op, const char *err_str, int err_num, unsigned flags);

/*
 * Pending operation structures
 */
struct ips_pend_sreq {
	STAILQ_ENTRY(ips_pend_sreq) next;
	psm2_mq_req_t req;
	uint32_t type;
};

#define IPS_PENDSEND_EAGER_DATA	1
#define IPS_PENDSEND_EAGER_REQ	2
#define IPS_PENDSEND_EXP_TIDS	3
#define IPS_PENDSEND_EXP_SENDS	4

STAILQ_HEAD(ips_pendsendq, ips_pend_sreq);

struct ips_pend_sends {
	struct ips_proto *proto;	/* back ptr */
	struct psmi_timer timer;
	struct ips_pendsendq pendq;
};

/*
 * One instance of the protocol
 */

struct ips_protoexp;

struct ips_proto_stats {
	uint64_t pio_busy_cnt;
	uint64_t pio_no_flow_credits;
	uint64_t pio_cc_max_duration;
#ifdef USE_RC
	uint64_t pio_rc_cc_max_duration;
#endif
#ifdef PSM_BYTE_FLOW_CREDITS
	uint64_t pio_no_flow_credit_bytes;
#endif
#ifdef PSM_VERBS
	uint64_t post_send_fail;
#endif
#ifdef PSM_HAVE_SDMA
	uint64_t sdma_compl_wait_ack;
	uint64_t sdma_compl_wait_resend;
	uint64_t sdma_compl_slow;
	uint64_t sdma_compl_yield;
#endif

	uint64_t scb_egr_unavail_cnt;
	uint64_t unknown_packets;
	uint64_t stray_packets;
	uint64_t rcv_revisit;
#ifdef PSM_SOCKETS
	uint64_t partial_data_write_cnt;
	uint64_t partial_ctr_write_cnt;
	uint64_t partial_read_cnt;
	uint64_t rcv_hol_blocking;
#endif
};

struct ips_scb_stats {
	uint64_t alloc;
	uint64_t free;
	uint64_t max_inuse;
	uint64_t dyn_alloc;
	uint64_t dyn_free;
	uint64_t max_dyn_inuse;
};

/*
 * Updates to these stats must be reflected in ips_ptl_epaddr_stats_init
 */
struct ips_proto_epaddr_stats {
	uint64_t err_chk_send;
	uint64_t err_chk_recv;
#ifdef PSM_HAVE_RDMA_ERR_CHK
	uint64_t err_chk_rdma_send;
	uint64_t err_chk_rdma_recv;
	uint64_t err_chk_rdma_resp_send;
	uint64_t err_chk_rdma_resp_recv;
#endif
	uint64_t nak_send;
	uint64_t nak_recv;
	uint64_t max_connected_outgoing;
	uint64_t max_connected_incoming;
	uint64_t connect_req_send;
	uint64_t connect_req_retry;
	uint64_t reconnect_req_send;
	uint64_t reconnect_req_retry;
	uint64_t connect_req_recv;
	uint64_t connect_req_rej;
	uint64_t connect_req_inconsistent;
	uint64_t connect_req_dup;
	uint64_t connect_req_stale;
	uint64_t connect_req_disc;
	uint64_t reconnect_req_recv;
	uint64_t reconnect_req_inconsistent;
	uint64_t reconnect_req_dup;
	uint64_t connect_rep_send;
	uint64_t connect_rep_recv;
	uint64_t connect_rep_unknown;
	uint64_t connect_rep_inconsistent;
	uint64_t connect_rep_dup;
	uint64_t connect_rep_stale;
	uint64_t connect_rep_disc;
	uint64_t reconnect_rep_dup;
	uint64_t reconnect_rep_stale;
	uint64_t reconnect_rep_disc;
	uint64_t disconnect_req_send;
	uint64_t disconnect_req_retry;
	uint64_t disconnect_req_recv;
	uint64_t disconnect_req_dup;
	uint64_t disconnect_rep_send;
	uint64_t disconnect_rep_recv;
	uint64_t disconnect_rep_unknown;
	uint64_t disconnect_rep_dup;
	uint64_t connections_lost;
	uint64_t reconnect_started;
	uint64_t max_reconnect_count;
	uint64_t max_reconnect_usec;
	uint64_t rts_send;
	uint64_t rts_recv;
	uint64_t cts_long_data_send;
	uint64_t cts_long_data_recv;
	uint64_t cts_rdma_send;
	uint64_t cts_rdma_recv;
	uint64_t send_rexmit;
#ifdef PSM_HAVE_RDMA_ERR_CHK
	uint64_t rdma_rexmit;
#endif
	uint64_t rep_proc_err;
#ifdef PSM_RC_RECONNECT
	uint64_t send_wc_error;
	uint64_t rdma_wc_error;
	uint64_t drain_wc_error;
	uint64_t recv_wc_error;
#endif
};

/* OPP support structure. */
struct opp_api {
	void *(*op_path_find_hca) (const char *name, void **device);
	void *(*op_path_open) (void *device, int port_num);
	void (*op_path_close) (void *context);
	int (*op_path_get_path_by_rec) (void *context, ibta_path_rec_t *query,
					ibta_path_rec_t *response);
};

struct ips_ibta_compliance_fn {
	psm2_error_t(*get_path_rec) (struct ips_proto *proto, __be16 slid,
				    __be16 dlid,
				    __be64 gid_hi, __be64 gid_lo,
				    unsigned long timeout,
				    ips_path_grp_t **ppathgrp);
	psm2_error_t(*fini) (struct ips_proto *proto);
};

/* please don't change the flow id order */
typedef enum ips_epaddr_flow {
	EP_FLOW_GO_BACK_N_PIO = 0,
	EP_FLOW_LAST		/* Keep this the last endpoint flow */
} ips_epaddr_flow_t;
// number of entries in ips_epaddr.ips_flow[], use this only when
// sizing array and testing index.  When testing flowid in other
// places use EP_FLOW_LAST as there could later be special flow types which
// don't need an entry in ips_flow[]
#define EP_NUM_FLOW_ENTRIES (EP_FLOW_LAST)

typedef enum psm_transfer_type {
	PSM_TRANSFER_PIO = 0,
	PSM_TRANSFER_LAST	/* Keep this the last transfer type */
} psm_transfer_type_t;

typedef enum psm_protocol_type {
	PSM_PROTOCOL_GO_BACK_N = 0,
	PSM_PROTOCOL_LAST	/* Keep this the last protocol type */
} psm_protocol_type_t;

struct ips_proto {
	struct ptl *ptl;	/* cached */
	psm2_ep_t ep;		/* cached, for errors */
	psm2_mq_t mq;		/* cached, for mq handling */
	/* Pending sends */
	struct ips_pend_sends pend_sends;
	struct ips_epstate *epstate;
	struct psmi_timer_ctrl *timerq;

	struct ips_protoexp *protoexp;
	struct ips_scbctrl *scbc_rv;
	void *spioc;	// HAL specific structure
	struct ips_scbctrl scbc_egr;
	struct ips_epinfo epinfo;


	uint64_t timeout_send;
	uint32_t flags;
#if   defined(PSM_HAVE_REG_MR)
	// TBD adjust rest of Send DMA code to use PSM_HAVE_SDMA
	uint32_t iovec_thresh_eager;
	uint32_t iovec_thresh_eager_blocking;
#endif
#ifdef PSM_HAVE_REG_MR
#ifdef PSM_HAVE_GPU
	uint32_t iovec_gpu_thresh_eager;
	uint32_t iovec_gpu_thresh_eager_blocking;
#endif
#endif
	uint32_t psn_mask;		// mask BTH PSN (24b or 31b)
	uint32_t scb_bufsize;
	uint32_t multirail_thresh_load_balance;
	uint16_t pktlen_mask;		// mask LRH pktlen (words) (12b or 16b)
	uint16_t flow_credits;		// credit limit in packets
#ifdef PSM_BYTE_FLOW_CREDITS
	uint32_t flow_credit_bytes;	// credit limit in bytes
#endif
	uint16_t min_credits;		// min credits
	uint16_t max_credits;		// max credits
	uint16_t credits_adjust;	// credit adjusting amount
	uint16_t credits_allow_adv_ctrl:1;	// whether enable advanced credit control
	uint16_t credits_inc_thresh:15;	// credit increase threshold. The max is IPS_MAX_CREDIT (16384)
	mpool_t pend_sends_pool;
	struct ips_ibta_compliance_fn ibta;
	struct ips_proto_stats stats;
	struct ips_proto_epaddr_stats epaddr_stats;
	struct ptl_strategy_stats strat_stats;
	struct ips_scb_stats scb_stats;

	struct ips_proto_am proto_am;

	struct ips_ctrlq ctrlq;
	/* pure sdma mode, use dma flow, otherwise, use pio flow */
	ips_epaddr_flow_t msgflowid;

#ifdef PSM_HAVE_REG_MR
	// mr_cache is only allocated and used when PSM3_RDMA enabled
	psm2_mr_cache_t mr_cache;
#endif


	uint64_t t_init;
	uint64_t t_fini;
	uint32_t runid_key;	/* we use our pid, not ideal */

	int num_connected_outgoing;
	int num_connected_incoming;
	int num_disconnect_requests;

	/* misc state variables. */

	/* Smallest interval in cycles between which we warn about stray
	 * messages This is a per-endpoint quantity, overridable with
	 * PSM_STRAY_WARN_INTERVAL We use the same interval to send the "die"
	 * message.
	 */
	uint64_t stray_warn_interval;
	int done_warning;
	int done_once;
	int num_bogus_warnings;
	struct {
		uint32_t interval_secs;
		uint64_t next_warning;
		uint64_t count;
	} psmi_logevent_tid_send_reqs;

	/*
	 * Disable the LMC based dispersive routing for all message
	 * sizes in bytes between ips_lmc_disable_low and ips_lmc_disable_high,
	 * inclusive.
	 */
	uint32_t ips_lmc_disable_low;
	uint32_t ips_lmc_disable_high;
	struct hsearch_data ips_path_rec_hash;
	struct hsearch_data ips_path_grp_hash;
	void *opp_lib;
	void *hndl;
	void *device;
	void *opp_ctxt;
	struct opp_api opp_fn;

#ifdef PSM_HAVE_GPU
	struct ips_gpu_hostbuf_mpool_cb_context gpu_hostbuf_send_cfg;
	struct ips_gpu_hostbuf_mpool_cb_context gpu_hostbuf_small_send_cfg;
	mpool_t gpu_hostbuf_pool_send;
	mpool_t gpu_hostbuf_pool_small_send;
	union ips_proto_gpu_specific gpu_specific;
	unsigned gpu_prefetch_limit;
#endif
/*
 * Control message queue for pending messages.
 *
 * Control messages are queued as pending when no PIO is available for sending
 * the message.  They are composed on the fly and do not need buffering.
 *
 * Variables here are write once (at init) and read afterwards (except the msg
 * queue overflow counters).
 *
 * Each of the queable messages is given a single bit in the queue_mask and
 * message_type_to_mask indicates the bit in the mask for a given message type.
 */
	uint64_t ctrl_msg_queue_overflow;
	uint32_t ctrl_msg_queue_enqueue;
	uint32_t message_type_to_mask[256];

	time_t writevFailTime;
};


/*
 * Test the payload length against the lmc_disable_low and lmc_disable_hi
 * values, to determine if a transfer of this size should use LMC LIDs.
 * Set the IPS_SEND_FLAG_NO_LMC flag in the scb.
 */
static inline void
ips_set_LMC_LID_choice(struct ips_proto *proto, ips_scb_t *scb, uint32_t len)
{
	if ((len >= proto->ips_lmc_disable_low) &&
	    (len <= proto->ips_lmc_disable_high)) {
		PSM2_LOG_MSG("DISABLE LMC paylen %u\n", len);
		scb->scb_flags |= IPS_SEND_FLAG_NO_LMC;
	}

	return;
}

/*
 * Endpoint address, encapsulates per-endpoint protocol metadata
 *
 * Directly implements the ptl epaddr.
 */
typedef psm2_error_t(*ips_flow_flush_fn_t) (struct ips_flow *, int *nflushed);

/**
 * ips_flow is a structure that combines all information regarding a send
 * from one endpoint to another one. Specifically, it is the place where
 * the Maximum Transmission Unit for a send is calculated, given how many
 * factors could possibly influence the MTU calculation. See ips_flow_init
 * documentation for more details.
 */
struct ips_flow {
	SLIST_ENTRY(ips_flow) next;	/* List of flows with pending acks */
	ips_flow_flush_fn_t flush;	/* flush function for this flow */

	struct ips_epaddr *ipsaddr;	/* back pointer, remote endpoint */
	ips_path_rec_t *path;	/* Path to use for flow */

	uint32_t frag_size;	/* < This flow's fragment size, calculated as the
				   < minimum of all relevant MTUs involved */

	uint16_t flowid:2;	/* flow id: pio(0) or dma(1) or tidflow(2) */
	uint16_t transfer:3;	/* spio or sdma */
	uint16_t protocol:3;	/* go-back-n or tidflow */
	uint16_t flags:8;	/* flow state flags */

	// to allow for good pipelining of send/ACK need to trigger an ack at
	// least every ack_interval packets (roughy flow_credits/4) or every
	// ack_inteval_bytes bytes (roughly flow_credit_bytes/4) whichever
	// comes 1st.  When flow_credit_bytes >= flow_credits*mtu
	// ack_interval will be the trigger.
	// Only when flow_credit_bytes < flow_credits*mtu will
	// ack_interval_bytes come into play.
	uint16_t ack_interval;	/* interval to ack packets */
	uint16_t ack_counter;	/* counter to ack packets */
#ifdef PSM_BYTE_FLOW_CREDITS
	uint32_t ack_interval_bytes;	/* interval to ack packets in bytes */
	uint32_t ack_counter_bytes;	/* counter to ack packets in bytes */
#endif
#ifdef PSM_SOCKETS
	uint32_t used_snd_buff; // number of bytes in socket send buffer
	uint32_t send_remaining; // the length of remaining data to send
	uint8_t* partial_conn_msg; // partial sent connection msg
	uint16_t partial_conn_msg_size; // size of the partial_conn_msg buffer
	uint16_t conn_msg_remainder; // the length of remaining connection msg to send
#endif
	// We track credits in terms of packets and bytes.
	// For UD and OPA, packets is sufficient since per pkt buffering.
	// For UDP, sockets has byte oriented buffering so we need to
	// impose a credit_bytes limit to allow sufficient pkt credits
	// but avoid sockets buffer overflow and recv side discards/flow control
	int16_t  credits;	 /* Current credits available to send on flow */
	uint16_t max_credits;	 /* credits limit */
	uint32_t credits_inc_psn; /* the reference pkt psn used for increasing credit. We increase */
	                         /* credit if current psn - credits_inc_psn > credit_inc_thresh */
#ifdef PSM_BYTE_FLOW_CREDITS
	int32_t  credit_bytes;	/* Current credit bytes avail to send on flow */
#endif
	uint32_t ack_index;     /* Index of the last ACK message type in pending message queue */
	psmi_seqnum_t xmit_seq_num;	/* next psn for xmit */
	psmi_seqnum_t xmit_ack_num;	/* last xmited psn acked + 1 */
	psmi_seqnum_t recv_seq_num;	/* next psn expect to recv */

	psmi_timer timer_send;	/* timer for frames that got a busy PIO */
	psmi_timer timer_ack;	/* timer for unacked frames */

	 STAILQ_HEAD(ips_scb_unackedq, ips_scb) scb_unacked;	/* unacked queue */
	 SLIST_HEAD(ips_scb_pendlist, ips_scb) scb_pend;	/* pending queue */

#ifdef PSM_DEBUG
	uint32_t scb_num_pending;	/* pending scb counter */
	uint32_t scb_num_unacked;	/* unacked scb counter */
#endif
};

#define IPS_FLOW_MSG_TOGGLE_OOO_MASK	(1 << 0)	/* ooo msg check */
#define IPS_FLOW_MSG_TOGGLE_UNEXP_MASK	(1 << 1)	/* unexp msg check */
/*
 * Make sure ips_epaddr_t and psm2_epaddr_t can be converted to each other.
 */
struct ips_epaddr {
	struct psm2_epaddr epaddr;	/* inlined psm level epaddr */
	struct ips_msgctl *msgctl;	/* ips level msg control */

	struct ips_epaddr *next;	/* linklist */

	struct ips_flow flows[EP_NUM_FLOW_ENTRIES];	/* pio and dma */
	ips_path_grp_t *pathgrp;	/* pointer to slid/dlid group in hash */

	uint32_t connidx_outgoing;	/* peer's connection idx */
	uint32_t connidx_incoming;	/* my connection idx */

	uint16_t ctrl_msg_queued;	/* bitmap of queued control messages to be send */

	uint8_t  hpp_index;	/* high priority index */
	uint8_t  msg_toggle;	/* only 2 bits used, 6 bits for future */
	uint8_t  hash;		/* hash for adaptve and static_dst dispersive */
	// congestion control count for unreliable send
	uint16_t cc_count;
	uint8_t  reconnect_count; /* for HAL specific reconnect */
	uint8_t  allow_reconnect:1; /* negotiated: allow HAL spec reconnect */
	uint8_t  reconnecting:1; /* HAL specific reconnect in progress */
	uint8_t  waiting_in_connect:1;	/* psm2_connect looping til EST */
	uint8_t  waiting_in_disconnect:1;/* psm2_disconnect looping til EST */
	uint8_t  reserved:4;
	union {
		// HAL specific fields
#ifdef PSM_VERBS
		struct {
			uint32_t remote_qpn;
#ifdef RNDV_MOD
			// for PSM3_RDMA=1
				/* GID of dest to use for IB CM  */
			union  ibv_gid remote_gid;
			psm3_rv_conn_t rv_conn;
 				/* RV index of dest to use for immed */
			uint32_t remote_rv_index;
			// state of connection - here so don't call kernel to
			// 	poll
			// ! conn - no connection
			// conn && ! connected - connection processes started,
			// 	but not done
			// connected - connection established and usable
			// 	(implies conn)
			uint8_t rv_connected:1;
#else
			uint8_t reserved_rv:1;
#endif /* RNDV_MOD */
			uint8_t reserved:3; /* [1] + 3 + [1 + 1 + 1 + 1] */
#ifndef PSM_HAVE_RDMA_ERR_CHK
			uint8_t reserved_err_chk:4;
#else
			// during error recovery a receiver may be unable to
			// allocate an scb to send the response or may need to
			// delay processing of the ERR_CHK_RDMA.  In which case
			// the information is stashed here and checked in
			// psm3_ips_proto_timer_send_callback for the
			// proto->msgflowid flow when an scb is available, this
			// info allows the response to be built.
			// Since we can only stash one such info per ipsaddr,
			// we limit senders to one outstanding err_chk_rdma at
			// a time.  Recovery is infrequent and already slow due
			// to QP reconnect so this is a reasonable compromise
			// the idea of using the ctrlq (64 entries deep per
			// proto) was explored but is not really for "level 2"
			// reliability messages so this approach
			// was deemed simpler to implement and lower risk to
			// mature code
 				/* only one per requestor */
			uint8_t err_chk_rdma_outstanding:1;
				/* is resp info stashed */
			uint8_t need_send_err_chk_rdma_resp:1;
				/* need recheck after delay */
			uint8_t err_chk_rdma_recheck:1;
				/* info for resp */
			uint8_t err_chk_rdma_resp_need_resend:1;
				/* info from request */
			uint8_t err_chk_rdma_rcnt; // reconnect_count at sender
				/* info from request */
			uint32_t err_chk_rdma_sendtoken;
				/* when recheck timeout (getcycles() in secs) */
			uint32_t err_chk_rdma_recheck_timeout_secs;
				/* info for resp */
			ptl_arg_t err_chk_rdma_resp_rdesc_id;
				/* info for resp */
			ptl_arg_t err_chk_rdma_resp_sdesc_id;
				/* queue to send resp */
			STAILQ_ENTRY(ips_epaddr) pend_err_resp_next;
#endif /* PSM_HAVE_RDMA_ERR_CHK */
#ifdef USE_RC
			// for PSM3_RDMA=2 or 3
#ifdef PSM_RC_RECONNECT
			// first in list is the active QP
			// rest (if any) are draining
	 		SLIST_HEAD(rc_qp_list, psm3_verbs_rc_qp) rc_qps;
#else
			// TBD - to reduce memory footprint, perhaps allocate a
			// separate structure only when RC QP enabled and point
			// to it here and/or merge these with fields above
			struct ibv_qp *rc_qp;
			struct psm3_verbs_recv_pool recv_pool;
			struct psm3_verbs_send_allocator send_allocator;
#endif
				// TBD if we allocated recv buffers sooner we
				// wouldn't need this field
			uint32_t rc_qp_max_recv_wr;
			uint32_t rc_qp_max_inline_data;

			// curr_* indicate the "current" version of various
			// objects/values to help avoid if tests in post_send
			// datapath point to verbs_ep fields (UD QP) until
			// rc_connected
			psm3_verbs_send_allocator_t curr_allocator;
			struct ibv_qp *curr_qp;
			uint32_t curr_max_inline_data;
#ifdef PSM_RC_RECONNECT
			struct psm3_verbs_rc_qp *curr_rc_qp; // NULL if ! rc_connected
#endif

			uint8_t rc_connected:1;
			uint8_t rc_reserved:7;

			// psn num of remote flow->recv_seq_num
			uint32_t remote_recv_psn;
			// congestion control count for reliable send
			uint16_t rc_cc_count;
#ifdef USE_RDMA_READ
			// MR for flow->recv_seq_num
			struct ibv_mr *recv_seq_mr;
			// remote flow->recv_seq_num addr and rkey
			uint64_t remote_recv_seq_addr;
			uint32_t remote_recv_seq_rkey;
			// MR for remote flow->recv_seq_num storage
			struct ibv_mr *remote_recv_psn_mr;
			// indicare whether we have outstanding RDMA Read for
			// remote flow->recv_seq_num
			uint8_t remote_seq_outstanding;
#endif /* USE_RDMA_READ */
#endif /* USE_RC */
		} verbs;
#endif /* PSM_VERBS */
#if defined(PSM_SOCKETS)
		struct {
			// for UDP the pri_addr is a UDP socket
			// for TCP the pri_addr is a TCP socket and the
			// aux_addr is a UDP socket used only for disconnect
			psm3_sockaddr_in_t remote_pri_addr;
			psm3_sockaddr_in_t remote_aux_addr;
			int tcp_fd;
			uint8_t connected;
		} sockets;
#endif /* PSM_SOCKETS */
	};

	/* this portion is only for connect/disconnect */
	uint64_t s_timeout;	/* used as a time in connect/reconnect/disc */
	uint64_t s_start;	/* when reconnect started */
	uint32_t runid_key;	/* peer process pid */
	uint32_t credit:1;	/* credit to connect/disconnect */
#ifdef PSM_DEBUG
	uint32_t set_req_params:1; /* has ipsaddr_set_req_params been called */
#else
	uint32_t reserved2:1;
#endif
	uint32_t cstate_outgoing:3;	/* connection state to, max 7 */
	uint32_t cstate_incoming:3;	/* connection state from, max 7 */
	uint32_t delay_in_ms:8;	/* disconnect delay in ms */
	uint32_t cerror_outgoing:8;	/* error code during connection */
	uint32_t cerror_incoming:8;	/* error code during connection */
	psmi_timer timer_reconnect;	/* connect/disconnect/timewait */
};

// has outgoing been connected, test to verify valid to issue MQ sends
static inline int
psm3_ips_proto_isconnected(ips_epaddr_t *ipsaddr)
{
	// While we might be able to issue MQ sends when
	// only incoming is ESTABLISHED or NEED_RECONNECT
	// callers should not be issuing MQ send against endpoints
	// they never called psm3_ips_proto_connect() for
	if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED
			|| ipsaddr->reconnecting)
		return 1;
	else
		return 0;
}

#ifdef PSM_HAVE_REG_MR
// This is only called when proto->protoexp != NULL (eg. HAL
// supports RDMA and RDMA is enabled).  So for now we can get away
// without adding this to HAL, even though it tests some ipsaddr fields
// which are undefined for sockets HAL.
// This indicates RDMA is available now or will soon be after recovery
// so MQ may choose the RDMA strategy in RTS and CTS
static inline int
ips_epaddr_rdma_connected(struct ips_epaddr *ipsaddr)
{
	psmi_assert(psmi_hal_has_cap(PSM_HAL_CAP_RDMA));
#ifdef PSM_VERBS
#ifdef PSM_RC_RECONNECT
	if (ipsaddr->verbs.rc_connected || ipsaddr->reconnecting)
		return 1;
#elif defined(USE_RC)
	if (ipsaddr->verbs.rc_connected)
		return 1;
#endif
#ifdef RNDV_MOD
	if (ipsaddr->verbs.rv_connected)
		return 1;
#endif
#endif /* PSM_VERBS */
	return 0;
}
#endif

// Reconnect count wraps at 8 bits.  However, the value of 0 is reserved
// to indicate the initial connect attempt and a connection which has
// never needed recovery
static inline uint8_t
ips_epaddr_next_reconnect_count(uint8_t reconnect_count)
{
	reconnect_count++;
	if (! reconnect_count)
		reconnect_count = 1;
	return reconnect_count;
}

/*
 * ips_msgctl_t is per connection struct.
 */
struct ips_msgctl {
	struct ips_epaddr master_epaddr; /* Master rail's epaddr */

	struct ips_epaddr *ipsaddr_next; /* next ipsaddr to send packet */
	uint16_t mq_send_seqnum;	 /* next sending message sequence */
	uint16_t mq_recv_seqnum;	 /* next receiving message sequence */
	uint16_t am_send_seqnum;	 /* next sending message sequence */
	uint16_t am_recv_seqnum;	 /* next receiving message sequence */
	uint16_t ipsaddr_count;		 /* number of ipsaddr to use */
	uint16_t outoforder_count;	 /* number of outoforder messages */
};

static inline __attribute__ ((unused))
void IPS_MCTXT_APPEND(ips_epaddr_t *head, ips_epaddr_t *node)
{
	ips_epaddr_t *cur;

	/* The new node is inserted before head. */
	node->next = head;

	/* Circle around the linked list to head's predecessor and update. */
	for (cur = head; cur->next != head; cur = cur->next);
	cur->next = node;
}

static inline __attribute__ ((unused))
void IPS_MCTXT_REMOVE(ips_epaddr_t *node)
{
	ips_epaddr_t *cur;

	/* Circle around to node's predecessor and update. */
	for (cur = node; cur->next != node; cur = cur->next);
	cur->next = node->next;
	node->next = node;
}

/*
 * Initialize a flow, setting its attributes. Selects the path the flow will
 * use as well as calculates the flow's fragment size defined as:
 * - min(remote EP MTU, selected path's MTU, local EP MTU) for DMA sends
 * - min(remote EP MTU, selected path's MTU, local EP MTU, local PIO bufsize) for PIO sends
 */
void MOCKABLE(psm3_ips_flow_init)(struct ips_flow *flow, struct ips_proto *proto,
		   ips_epaddr_t *ipsaddr, psm_transfer_type_t transfer_type,
		   psm_protocol_type_t protocol, ips_path_type_t path_type,
		   uint32_t flow_index);
MOCK_DCL_EPILOGUE(psm3_ips_flow_init);

void ips_scb_prepare_flow(ips_scb_t *scb, ips_epaddr_t *ipsaddr,
			  struct ips_flow *flow);

void MOCKABLE(psm3_ips_proto_flow_enqueue)(struct ips_flow *flow, ips_scb_t *scb);
MOCK_DCL_EPILOGUE(psm3_ips_proto_flow_enqueue);

psm2_error_t psm3_ips_proto_flow_flush_pio(struct ips_flow *flow, int *nflushed);

/* Wrapper for enqueue + flush */
psm2_error_t ips_proto_scb_pio_send(struct ips_flow *flow, ips_scb_t *scb);

#ifdef PSM_HAVE_SDMA
psm2_error_t ips_proto_dma_wait_until(struct ips_proto *proto, ips_scb_t *scb);
#endif

/*
 * Protocol receive processing
 *
 */
/* Error handling for unknown packet, packet is unknown when epid doesn't match
 * in epstate table */
int psm3_ips_proto_process_unknown(const struct ips_recvhdrq_event *rcv_ev, int *opcode);
/* Exposed for fastpath only */
int psm3_ips_proto_process_ack(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_process_nak(struct ips_recvhdrq_event *rcv_ev);

/*
 * Protocol exception handling and frame dumps
 */
void psm3_ips_proto_show_header(struct ips_message_header *p_hdr, char *msg);
void psm3_ips_proto_dump_frame(void *frame, int lenght, char *message);
void psm3_ips_proto_dump_data(void *data, int data_length);
void ips_proto_dump_eager(uint32_t *curr_rcv_hdr);

/*
 * Checksum of ips packets
 */
uint32_t psm3_ips_cksum_calculate(struct ips_message_header *p_hdr,
                                uint8_t *payload, uint32_t paylen);

/*
 * Matched-Queue processing and sends
 */
psm2_error_t psm3_ips_proto_mq_push_cts_req(struct ips_proto *proto,
				      psm2_mq_req_t req);
psm2_error_t psm3_ips_proto_mq_push_rts_data(struct ips_proto *proto,
				       psm2_mq_req_t req);
int psm3_ips_proto_mq_handle_cts(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_mq_handle_rts(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_mq_handle_tiny(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_mq_handle_short(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_mq_handle_eager(struct ips_recvhdrq_event *rcv_ev);
void psm3_ips_proto_mq_handle_outoforder_queue(psm2_mq_t mq, ips_msgctl_t *msgctl);
int psm3_ips_proto_mq_handle_data(struct ips_recvhdrq_event *rcv_ev);

psm2_error_t psm3_ips_proto_mq_send(psm2_mq_t mq, psm2_epaddr_t epaddr,
			      uint32_t flags, psm2_mq_tag_t *tag,
			      const void *ubuf, uint32_t len);

psm2_error_t psm3_ips_proto_mq_isend(psm2_mq_t mq, psm2_epaddr_t epaddr,
				uint32_t flags_user, uint32_t flags_internal,
				psm2_mq_tag_t *tag, const void *ubuf, uint32_t len,
				void *context, psm2_mq_req_t *req_o);

int psm3_ips_proto_am(struct ips_recvhdrq_event *rcv_ev);

/*
 * IPS packet service routine table.
 */
typedef int (*ips_packet_service_fn_t)(struct ips_recvhdrq_event *rcv_ev);
extern ips_packet_service_fn_t
	psm3_ips_packet_service_routine[OPCODE_FUTURE_FROM-OPCODE_RESERVED];

/* IBTA feature related functions (path record, etc.) */
psm2_error_t
MOCKABLE(psm3_ips_ibta_init)(struct ips_proto *proto);
MOCK_DCL_EPILOGUE(psm3_ips_ibta_init);

psm2_error_t psm3_ips_ibta_fini(struct ips_proto *proto);


#ifdef PSM_HAVE_GPU
PSMI_ALWAYS_INLINE(
uint32_t ips_gpu_next_window(uint32_t max_window, uint32_t offset,
			      uint32_t len))
{
	uint32_t window_len;
	window_len = len - offset;
	if (window_len >= max_window)
		window_len = max_window;
	return window_len;
}
#endif

extern void ips_proto_report_inflight(psm2_ep_t ep);

#endif /* _IPS_PROTO_H */
