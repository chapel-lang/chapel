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

#include "ips_tid.h"
#include "ips_recvhdrq.h"
#include "ips_epstate.h"
#include "ips_proto_am.h"
#include "ips_tidflow.h"
#include "ips_path_rec.h"

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
 */
struct ips_epinfo {
	uint16_t ep_base_lid;
	uint8_t ep_hash;	// for hashing adaptive dispersive routing
#define EP_HASH ep_hash
	uint8_t ep_lmc;
	opa_rate ep_link_rate;
	uint16_t ep_sl;		/* PSM3_NIC_SL only when path record not used */
	uint16_t ep_mtu;	// PSM payload after potential hdr & PSM3_MTU decrease
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
	uint16_t pg_base_dlid;
	uint16_t pg_base_slid;
	uint8_t pg_num_paths[IPS_PATH_MAX_PRIORITY];
	uint8_t pg_next_path[IPS_PATH_MAX_PRIORITY];
	ips_path_rec_t *pg_path[0][IPS_PATH_MAX_PRIORITY];
} ips_path_grp_t;

/*
 * Start and finish routines for constructing an ips_proto.
 */
struct ips_proto;
psm2_error_t ips_proto_init(const psmi_context_t *context,
			    const struct ptl *ptl,
			    int num_of_send_bufs,
			    int num_of_send_desc,
			    uint32_t imm_size,
			    const struct psmi_timer_ctrl *timerq, /* PTL's timerq */
			    const struct ips_epstate *epstate,	  /* PTL's epstate */
			    void *spioc,	                  /* PTL's opaque spio control */
			    struct ips_proto *proto);	          /* output protocol */

psm2_error_t ips_proto_fini(struct ips_proto *proto, int force,
			   uint64_t timeout);

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
	uint32_t ctrlq_overflow;

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
 *
 * Connect FROM (we receive a connect request - Connect INCOMING)
 *   NONE -> ESTABLISHED -> NONE
 */
#define CSTATE_ESTABLISHED		1
#define CSTATE_NONE			2
#define CSTATE_OUTGOING_DISCONNECTED	3
#define CSTATE_OUTGOING_WAITING		4
#define CSTATE_OUTGOING_WAITING_DISC	5

psm2_error_t ips_proto_connect(struct ips_proto *proto, int numep,
			      const psm2_epid_t *array_of_epid,
			      const int *array_of_epid_mask,
			      psm2_error_t *array_of_errors,
			      psm2_epaddr_t *array_of_epaddr,
			      uint64_t timeout_in);

psm2_error_t ips_proto_disconnect(struct ips_proto *proto, int force, int numep,
				 psm2_epaddr_t array_of_epaddr[],
				 const int array_of_epaddr_mask[],
				 psm2_error_t array_of_errors[],
				 uint64_t timeout_in);

int ips_proto_isconnected(struct ips_epaddr *ipsaddr);

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
	uint64_t writev_busy_cnt;
	uint64_t scb_egr_unavail_cnt;
	uint64_t unknown_packets;
	uint64_t stray_packets;
};


/*
 * Updates to these stats must be reflected in ips_ptl_epaddr_stats_init
 */
struct ips_proto_epaddr_stats {
	uint64_t err_chk_send;
	uint64_t err_chk_recv;
#ifdef RNDV_MOD
	uint64_t err_chk_rdma_send;
	uint64_t err_chk_rdma_recv;
#endif
	uint64_t nak_send;
	uint64_t nak_recv;
	uint64_t connect_req_send;
	uint64_t connect_req_recv;
	uint64_t connect_rep_send;
	uint64_t connect_rep_recv;
	uint64_t disconnect_req_send;
	uint64_t disconnect_req_recv;
	uint64_t disconnect_rep_send;
	uint64_t disconnect_rep_recv;
	uint64_t tids_grant_send;
	uint64_t tids_grant_recv;
	uint64_t send_rexmit;
#ifdef RNDV_MOD
	uint64_t rdma_rexmit;
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
	psm2_error_t(*get_path_rec) (struct ips_proto *proto, uint16_t slid,
				    uint16_t dlid,
				    uint16_t ip_hi,
				    unsigned long timeout,
				    ips_path_grp_t **ppathgrp);
	psm2_error_t(*fini) (struct ips_proto *proto);
};

/* please don't change the flow id order */
typedef enum ips_epaddr_flow {
	EP_FLOW_GO_BACK_N_PIO,
	EP_FLOW_TIDFLOW,	/* Can either pio or dma for tidflow */
	EP_FLOW_LAST		/* Keep this the last endpoint flow */
} ips_epaddr_flow_t;

typedef enum psm_transfer_type {
	PSM_TRANSFER_PIO,
	PSM_TRANSFER_LAST	/* Keep this the last transfer type */
} psm_transfer_type_t;

typedef enum psm_protocol_type {
	PSM_PROTOCOL_GO_BACK_N,
	PSM_PROTOCOL_TIDFLOW,
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
	struct ips_spio *spioc;
	struct ips_scbctrl scbc_egr;
	struct ips_epinfo epinfo;

	ips_scb_t **sdma_scb_queue;
	uint16_t sdma_queue_size;
	uint16_t sdma_fill_index;
	uint16_t sdma_done_index;
	uint16_t sdma_avail_counter;

	uint64_t timeout_send;
	uint32_t flags;
	uint32_t iovec_thresh_eager;
	uint32_t iovec_thresh_eager_blocking;
	uint32_t psn_mask;
	uint32_t scb_bufsize;
	uint32_t multirail_thresh_load_balance;
	uint16_t flow_credits;
	mpool_t pend_sends_pool;
	mpool_t timer_pool;
	struct ips_ibta_compliance_fn ibta;
	struct ips_proto_stats stats;
	struct ips_proto_epaddr_stats epaddr_stats;

	struct ips_proto_am proto_am;

	struct ips_ctrlq ctrlq;
	/* pure sdma mode, use dma flow, otherwise, use pio flow */
	ips_epaddr_flow_t msgflowid;

	// mr_cache is only allocated and used when PSM3_RDMA enabled
	psm2_mr_cache_t mr_cache;


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

#ifdef PSM_CUDA
	struct ips_cuda_hostbuf_mpool_cb_context cuda_hostbuf_send_cfg;
	struct ips_cuda_hostbuf_mpool_cb_context cuda_hostbuf_small_send_cfg;
	mpool_t cuda_hostbuf_pool_send;
	mpool_t cuda_hostbuf_pool_small_send;
	CUstream cudastream_send;
	unsigned cuda_prefetch_limit;
#endif

/*
 * Control message queue for pending messages.
 *
 * Control messages are queued as pending when no PIO is available for sending
 * the message.  They are composed on the fly and do not need buffering.
 *
 * Variables here are write once (at init) and read afterwards (except the msg
 * queue overflow counters).
 */
	uint32_t ctrl_msg_queue_overflow;
	uint32_t ctrl_msg_queue_enqueue;
	uint32_t message_type_to_index[256];
#define message_type2index(proto, msg_type) (proto->message_type_to_index[(msg_type)])

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

	uint16_t frag_size;	/* < This flow's fragment size, calculated as the
				   < minimum of all relevant MTUs involved */

	uint16_t flowid:2;	/* flow id: pio(0) or dma(1) or tidflow(2) */
	uint16_t transfer:3;	/* spio or sdma */
	uint16_t protocol:3;	/* go-back-n or tidflow */
	uint16_t flags:8;	/* flow state flags */

	uint16_t cwin;		/* Size of congestion window */
	uint16_t ack_interval;	/* interval to ack packets */
	uint16_t ack_counter;	/* counter to ack packets */
	int16_t  credits;	/* Current credits available to send on flow */
	uint32_t ack_index;     /* Index of the last ACK message type in pending message queue */

	psmi_seqnum_t xmit_seq_num;	/* transmit packet sequence number */
	psmi_seqnum_t xmit_ack_num;	/* acked packet sequence number */
	psmi_seqnum_t recv_seq_num;	/* recieved packet sequence number */

	psmi_timer *timer_send;	/* timer for frames that got a busy PIO */
	psmi_timer *timer_ack;	/* timer for unacked frames */

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
 * Make sure ips_epaddr_t and psm2_epaddr_t can be converted each other.
 */
struct ips_epaddr {
	struct psm2_epaddr epaddr;	/* inlined psm level epaddr */
	struct ips_msgctl *msgctl;	/* ips level msg control */

	struct ips_epaddr *next;	/* linklist */

	struct ips_flow flows[EP_FLOW_LAST - 1];	/* pio and dma */
	ips_path_grp_t *pathgrp;	/* pointer to slid/dlid group in hash */

	uint32_t connidx_outgoing;	/* peer's connection idx */
	uint32_t connidx_incoming;	/* my connection idx */

	uint16_t ctrl_msg_queued;	/* bitmap of queued control messages to be send */
	uint32_t window_rv;		/* RNDV window size per connection */

	uint8_t  hpp_index;	/* high priority index */
	uint8_t  msg_toggle;	/* only 2 bits used, 6 bits for future */
	// on UD/UDP context only used for hashing adaptive dispersive routing
	uint32_t remote_qpn;
#define IPSADDR_HASH remote_qpn
#ifdef RNDV_MOD
	union  ibv_gid remote_gid;	/* GID of dest to use for IB CM  */
	psm2_rv_conn_t rv_conn;
	uint32_t remote_rv_index; // RV index of dest to use for immed */
	// state of connection - need it here so we don't call kernel to poll
	// ! conn - no connection
	// conn && ! connected - connection processes started, but not done
	// connected - connection established and usable (implies conn)
	uint8_t rv_connected:1;
	uint8_t reserved:4;
	// during error recovery a receiver may be unable to allocate an scb to
	// send the respond.  In which case the information is stashed here and
	// checked in ips_proto_timer_send_callback for the proto->msgflowid flow
	// when an scb is available, this info allows the response to be built
	// Since we can only stash one such info per ipsaddr, we limit senders
	// to one outstanding err_chk_rdma at a time.  Recovery is infrequent
	// and already slow due to QP reconnect so this is a reasonable compromise
	// the idea of using the ctrlq (64 entries deep per proto) was explored
	// but is not really for "level 2" reliability messages so this approach
	// was deemed simpler to implement and lower risk to mature code
	uint8_t rv_err_chk_rdma_outstanding:1; /* only one per requestor */
	uint8_t rv_need_send_err_chk_rdma_resp:1; /* is resp info stashed */
	uint8_t rv_err_chk_rdma_resp_need_resend:1; /* info for resp */
	ptl_arg_t rv_err_chk_rdma_resp_rdesc_id; /* info for resp */
	ptl_arg_t rv_err_chk_rdma_resp_sdesc_id; /* info for resp */
	STAILQ_ENTRY(ips_epaddr) pend_err_resp_next; /* queue to send resp */
#endif
	// TBD - to reduce memory footprint, perhaps allocate a separate
	// structure only when RC QP enabled and point to it here
	struct ibv_qp *rc_qp;
	struct psm2_verbs_recv_pool recv_pool;
	uint32_t rc_qp_max_recv_wr;	// TBD if we allocated recv buffers sooner we
							// wouldn't need this field
	uint32_t rc_qp_max_inline_data;
	struct psm2_verbs_send_allocator send_allocator;
	// use_* help avoid if tests in post_send datapath
	psm2_verbs_send_allocator_t use_allocator;	// points to verbs_ep until
												// rc_connected
	struct ibv_qp *use_qp;	// points to verbs_ep UD QP until
							// rc_connected
	uint32_t use_max_inline_data;	// verbs_ep UD QP value until connected
	uint8_t rc_connected;

	/* this portion is only for connect/disconnect */
	uint64_t s_timeout;	/* used as a time in close */
	uint32_t runid_key;	/* peer process pid */
	uint32_t credit:2;	/* credit to connect/disconnect: 0 or 1 */
	uint32_t cstate_outgoing:3;	/* connection state to, max 7 */
	uint32_t cstate_incoming:3;	/* connection state from, max 7 */
	uint32_t delay_in_ms:8;	/* disconnect delay in ms */
	uint32_t cerror_outgoing:8;	/* error code during connection */
	uint32_t cerror_incoming:8;	/* error code during connection */
};

static inline int
ips_epaddr_connected(struct ips_epaddr *ipsaddr)
{
	if (ipsaddr->rc_connected)
		return 1;
#ifdef RNDV_MOD
	if (ipsaddr->rv_connected)
		return 1;
#endif
	return 0;
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
void MOCKABLE(ips_flow_init)(struct ips_flow *flow, struct ips_proto *proto,
		   ips_epaddr_t *ipsaddr, psm_transfer_type_t transfer_type,
		   psm_protocol_type_t protocol, ips_path_type_t path_type,
		   uint32_t flow_index);
MOCK_DCL_EPILOGUE(ips_flow_init);

void ips_scb_prepare_flow(ips_scb_t *scb, ips_epaddr_t *ipsaddr,
			  struct ips_flow *flow);

void MOCKABLE(ips_proto_flow_enqueue)(struct ips_flow *flow, ips_scb_t *scb);
MOCK_DCL_EPILOGUE(ips_proto_flow_enqueue);

psm2_error_t ips_proto_flow_flush_pio(struct ips_flow *flow, int *nflushed);
psm2_error_t ips_proto_flow_flush_dma(struct ips_flow *flow, int *nflushed);

/* Wrapper for enqueue + flush */
psm2_error_t ips_proto_scb_pio_send(struct ips_flow *flow, ips_scb_t *scb);

void ips_proto_scb_dma_enqueue(struct ips_proto *proto, ips_scb_t *scb);
psm2_error_t ips_proto_scb_dma_flush(struct ips_proto *proto,
				    ips_epaddr_t *ipsaddr, int *nflushed);
psm2_error_t ips_proto_dma_wait_until(struct ips_proto *proto, ips_scb_t *scb);

/*
 * Protocol receive processing
 *
 */
/* Error handling for unknown packet, packet is unknown when epid doesn't match
 * in epstate table */
int ips_proto_process_unknown(const struct ips_recvhdrq_event *rcv_ev);
/* Exposed for fastpath only */
int ips_proto_process_ack(struct ips_recvhdrq_event *rcv_ev);
int ips_proto_process_nak(struct ips_recvhdrq_event *rcv_ev);

/*
 * Protocol exception handling and frame dumps
 */
void ips_proto_show_header(struct ips_message_header *p_hdr, char *msg);
void ips_proto_dump_frame(void *frame, int lenght, char *message);
void ips_proto_dump_data(void *data, int data_length);
void ips_proto_dump_eager(uint32_t *curr_rcv_hdr);

/*
 * Checksum of ips packets
 */
uint32_t ips_crc_calculate(uint32_t len, uint8_t *data, uint32_t crc);

/*
 * Matched-Queue processing and sends
 */
psm2_error_t ips_proto_mq_push_cts_req(struct ips_proto *proto,
				      psm2_mq_req_t req);
psm2_error_t ips_proto_mq_push_rts_data(struct ips_proto *proto,
				       psm2_mq_req_t req);
int ips_proto_mq_handle_cts(struct ips_recvhdrq_event *rcv_ev);
int ips_proto_mq_handle_rts(struct ips_recvhdrq_event *rcv_ev);
int ips_proto_mq_handle_tiny(struct ips_recvhdrq_event *rcv_ev);
int ips_proto_mq_handle_short(struct ips_recvhdrq_event *rcv_ev);
int ips_proto_mq_handle_eager(struct ips_recvhdrq_event *rcv_ev);
void ips_proto_mq_handle_outoforder_queue(psm2_mq_t mq, ips_msgctl_t *msgctl);
int ips_proto_mq_handle_data(struct ips_recvhdrq_event *rcv_ev);

psm2_error_t ips_proto_mq_send(psm2_mq_t mq, psm2_epaddr_t epaddr,
			      uint32_t flags, psm2_mq_tag_t *tag,
			      const void *ubuf, uint32_t len);

psm2_error_t ips_proto_mq_isend(psm2_mq_t mq, psm2_epaddr_t epaddr,
				uint32_t flags_user, uint32_t flags_internal,
				psm2_mq_tag_t *tag, const void *ubuf, uint32_t len,
				void *context, psm2_mq_req_t *req_o);

psm2_error_t ips_proto_msg_size_thresh_query (enum psm2_info_query_thresh_et,
					      uint32_t *out, psm2_mq_t mq, psm2_epaddr_t);

int ips_proto_am(struct ips_recvhdrq_event *rcv_ev);

/*
 * IPS packet service routine table.
 */
typedef int (*ips_packet_service_fn_t)(struct ips_recvhdrq_event *rcv_ev);
extern ips_packet_service_fn_t
	ips_packet_service_routine[OPCODE_FUTURE_FROM-OPCODE_RESERVED];

psm2_error_t ips_ibta_link_updown_event(struct ips_proto *proto);

psm2_error_t
MOCKABLE(ips_ibta_init)(struct ips_proto *proto);
MOCK_DCL_EPILOGUE(ips_ibta_init);

psm2_error_t ips_ibta_fini(struct ips_proto *proto);


#ifdef PSM_CUDA
PSMI_ALWAYS_INLINE(
uint32_t ips_cuda_next_window(uint32_t max_window, uint32_t offset,
			      uint32_t len))
{
	uint32_t window_len;
	window_len = len - offset;
	if (window_len >= max_window)
		window_len = max_window;
	return window_len;
}
#endif


#endif /* _IPS_PROTO_H */
