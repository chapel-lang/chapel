/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2022 Cornelis Networks.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _FI_PROV_OPX_RELIABILITY_H_
#define _FI_PROV_OPX_RELIABILITY_H_

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rbtree.h"
#include "ofi_lock.h"
#include <ofi_mem.h>
#include "uthash.h"

#include "rdma/opx/fi_opx_atomic_fifo.h"
#include "rdma/opx/fi_opx_timer.h"

enum ofi_reliability_kind {
	OFI_RELIABILITY_KIND_NONE = 0,
	OFI_RELIABILITY_KIND_OFFLOAD,
	OFI_RELIABILITY_KIND_ONLOAD,
	OFI_RELIABILITY_KIND_RUNTIME,
	OFI_RELIABILITY_KIND_COUNT,
	OFI_RELIABILITY_KIND_UNSET,
};

enum ofi_reliability_app_kind {
	OFI_RELIABILITY_APP_KIND_MPI = 0,
	OFI_RELIABILITY_APP_KIND_DAOS
};

/* #define SKIP_RELIABILITY_PROTOCOL_RX */
/* #define SKIP_RELIABILITY_PROTOCOL_TX */
/* #define OPX_RELIABILITY_DEBUG */
/* #define OPX_RELIABILITY_TEST */

#ifdef OPX_RELIABILITY_TEST
#define FI_OPX_RELIABILITY_RX_DROP_PACKET(x,y)	fi_opx_reliability_rx_drop_packet(x,y)
#else
#define FI_OPX_RELIABILITY_RX_DROP_PACKET(x,y)	(0)
#endif

#define PENDING_RX_RELIABLITY_COUNT_MAX (1024)  // Max depth of the pending Rx reliablity pool

struct fi_opx_completion_counter {
		struct fi_opx_completion_counter *next;
		union {
			uint64_t tag;
			ssize_t initial_byte_count;
		};
		ssize_t byte_counter;
		struct fi_opx_cntr *cntr;
		struct fi_opx_cq *cq;
		union {
			union fi_opx_context *context;
			void *container;
		};
		void (*hit_zero)(struct fi_opx_completion_counter*);
};

union fi_opx_reliability_deferred_work;
struct fi_opx_reliability_work_elem {
	struct slist_entry slist_entry;
	ssize_t (*work_fn)(union fi_opx_reliability_deferred_work * work_state);
};

struct fi_opx_reliability_tx_sdma_replay_params {
	struct fi_opx_reliability_work_elem work_elem;
	void *opx_ep;
	struct slist sdma_reqs;
	uint64_t flow_key;
};

#define OPX_RELIABILITY_TX_MAX_REPLAYS		(32)
struct fi_opx_reliability_tx_pio_replay_params {
	struct fi_opx_reliability_work_elem work_elem;
	void *opx_ep;
	struct fi_opx_reliability_tx_replay *replays[OPX_RELIABILITY_TX_MAX_REPLAYS];
	uint64_t flow_key;
	uint16_t num_replays;
	uint16_t start_index;
};

union fi_opx_reliability_deferred_work {
	struct fi_opx_reliability_work_elem work_elem;
	struct fi_opx_reliability_tx_sdma_replay_params sdma_replay;
	struct fi_opx_reliability_tx_pio_replay_params pio_replay;
};

struct fi_opx_reliability_service {

	struct fi_opx_atomic_fifo			fifo;		/* 27 qws = 216 bytes */
	uint64_t					usec_next;
	uint32_t					usec_max;
	uint32_t					preemptive_ack_rate_mask;

	// ==== 232 bytes here, 24 bytes left in cacheline ======
	struct {
		union fi_opx_timer_stamp		timestamp;	/*  2 qws =  16 bytes */
		union fi_opx_timer_state		timer;		/*  5 bytes           */
		uint8_t					unused_padding[3];

	/* == CACHE LINE == */
		RbtHandle				flow;		/*  1 qw  =   8 bytes */
		uint64_t				ping_start_key;
		uint64_t				unused;

		struct {
			uint64_t			unused_cacheline_1;
			union fi_opx_hfi1_pio_state *	pio_state;
			volatile uint64_t *		pio_scb_sop_first;
			volatile uint64_t *		pio_credits_addr;
			volatile uint64_t *		pio_scb_first;

	/* == CACHE LINE == */
			struct fi_opx_hfi1_txe_scb	ping_model;
			struct fi_opx_hfi1_txe_scb	ack_model;
			struct fi_opx_hfi1_txe_scb	nack_model;
		} hfi1;
	} tx __attribute__((__packed__));;

	/* == CACHE LINE == */

	struct {

		RbtHandle				flow;		/*  1 qw  =   8 bytes */
		struct {

			struct fi_opx_hfi1_rxe_state	state;		/*  2 qws =  16 bytes */

			struct {
				uint32_t *		rhf_base;
				volatile uint64_t *	head_register;
			} hdrq;

			/* -- not critical; can be moved to another cacheline */

			struct {
				uint32_t *		base_addr;
				uint32_t		elemsz;
				uint32_t		last_egrbfr_index;
				volatile uint64_t *	head_register;
			} egrq;
		} hfi1;
	} rx;

	/* == CACHE LINE == */

	struct fi_opx_hfi1_context *			context;
	volatile uint64_t				enabled;
	volatile uint64_t				active;
	pthread_t					thread;				/* 1 qw =  8 bytes */
	struct ofi_bufpool 				*pending_rx_reliability_pool;
	struct fi_opx_pending_rx_reliability_op		*pending_rx_reliability_ops_hashmap;
	uint64_t					backoff_period;
	uint32_t					lid_be;
	uint32_t					preemptive_ack_rate;

	/* == CACHE LINE == */
	int				is_backoff_enabled;
	enum ofi_reliability_kind	reliability_kind;	/* 4 bytes */
	uint16_t			nack_threshold;
	uint16_t			unused[2];
	uint8_t				fifo_max;
	uint8_t				hfi1_max;
	RbtHandle			handshake_init;		/*  1 qw  =   8 bytes */
	struct ofi_bufpool 		*uepkt_pool;
	struct slist			work_pending;		/* 16 bytes */
	struct ofi_bufpool 		*work_pending_pool;
	/* 8 bytes left in cacheline */

} __attribute__((__aligned__(64))) __attribute__((__packed__));


union fi_opx_reliability_service_flow_key {
	uint64_t		value;
	uint32_t		value32b[2];
	struct {
		uint32_t	slid	: 24;
		uint32_t	tx	:  8;
		uint32_t	dlid	: 24;
		uint32_t	rx	:  8;
	} __attribute__((__packed__));
};

struct fi_opx_reliability_flow {
	//opx_spin_t				lock;
	uint64_t					next_psn;
	union fi_opx_reliability_service_flow_key	key;
	struct fi_opx_reliability_rx_uepkt *		uepkt;
	uint8_t						origin_rx;
};

struct fi_opx_pending_rx_reliability_op_key {
	uint64_t		key;
	uint64_t 		psn_start;
};

struct fi_opx_pending_rx_reliability_op {
	struct fi_opx_pending_rx_reliability_op_key key;
	uint64_t 		psn_count;
	uint64_t 		psn_count_coalesce;
	uint64_t		slid;
	uint64_t 		rx;
	uint64_t		ud_opcode;  // Only needs to be uint8_t
	UT_hash_handle 	hh;         /* makes this structure hashable */
};


struct fi_opx_reliability_tx_replay {
	/* == CACHE LINE 0 == */
	struct fi_opx_reliability_tx_replay		*next;
	struct fi_opx_reliability_tx_replay		*prev;
	uint64_t					target_reliability_rx;
	union fi_opx_reliability_tx_psn			*psn_ptr;
	struct fi_opx_completion_counter		*cc_ptr;
	uint64_t					cc_dec;
	union {
		uint64_t				*payload;
		struct iovec				*iov;
	};
	uint16_t					unused1;
	uint16_t					nack_count;
	bool						acked;
	bool						pinned;
	bool						use_sdma;
	bool						use_iov;

	/* == CACHE LINE 1 == */
	void						*sdma_we;
	uint32_t					sdma_we_use_count;
	uint32_t					unused2;
	uint64_t					unused3[6];

#ifndef NDEBUG
	uint64_t					orig_payload[8];
#endif

	/* == CACHE LINE == */

	/* --- MUST BE 64 BYTE ALIGNED --- */
	struct fi_opx_hfi1_txe_scb			scb;
	uint8_t						data[];
} __attribute__((__aligned__(64)));


struct fi_opx_reliability_resynch_flow {
	bool client_ep;
	uint64_t resynch_counter;
	union fi_opx_reliability_service_flow_key key;
	/* client related fields */
	bool remote_ep_resynch_completed;
	bool resynch_client_ep;
};

// Begin rbtree implementation
// Import and inline data structures from the red-black tree implementation
// The RBTree functions cause a fairly large loss of message rate
// as the message rate code is instruction bound and the out of line implementations
// are small  These data structures mirror the out of line implementations
// but if those ever change, we need to update our copies of this
// or lobby for an interface change to rb tree to allow for inline implementations
// On skylake 18 core pairs, the rbtree implementation leads to a loss of about
// 10 mmps by not being in lined
// These substitute inline functions are copies of the implementation in rbtree.c
#include "ofi_tree.h"
typedef enum ofi_node_color NodeColor;

typedef struct NodeTag {
        struct NodeTag *left;       // left child
        struct NodeTag *right;      // right child
        struct NodeTag *parent;     // parent
        NodeColor color;            // node color (BLACK, RED)
        void *key;                  // key used for searching
        void *val;                // user data
} NodeType;

typedef struct RbtTag {
        NodeType *root;   // root of red-black tree
        NodeType sentinel;
        int (*compare)(void *a, void *b);    // compare keys
} RbtType;

__OPX_FORCE_INLINE__
int fi_opx_reliability_compare (void *a, void *b) {

        const uintptr_t a_key = (uintptr_t)a;
        const uintptr_t b_key = (uintptr_t)b;

        if (a_key > b_key) return 1;
        if (a_key < b_key) return -1;

        return 0;
}

__OPX_FORCE_INLINE__
void *fi_opx_rbt_find(RbtHandle h, void *key) {
        RbtType *rbt = h;
        NodeType *current;
        current = rbt->root;
        while(current != &rbt->sentinel) {
                int rc = fi_opx_reliability_compare(key, current->key);
                if (rc == 0) return current;
                current = (rc < 0) ? current->left : current->right;
        }
        return NULL;
}

__OPX_FORCE_INLINE__
void ** fi_opx_rbt_value_ptr(RbtHandle h, RbtIterator it) {
        NodeType *i = it;

        return &i->val;
}

__OPX_FORCE_INLINE__
RbtIterator fi_opx_rbt_begin(RbtHandle h) {
        RbtType *rbt = h;

        // return pointer to first value
        NodeType *i;
        for (i = rbt->root; i->left != &rbt->sentinel; i = i->left);
        return i != &rbt->sentinel ? i : NULL;
}

__OPX_FORCE_INLINE__
void fi_opx_rbt_key_value(RbtHandle h, RbtIterator it, void **key, void **val) {
        NodeType *i = it;

        *key = i->key;
        *val = i->val;
}

/*
 * Initialize the reliability service - and pthread
 *
 * \return reliability service hfi1 rx identifier
 */
uint8_t fi_opx_reliability_service_init (struct fi_opx_reliability_service * service, uuid_t unique_job_key,
		struct fi_opx_hfi1_context * hfi1,
		const enum ofi_reliability_kind reliability_kind);
void fi_opx_reliability_service_fini (struct fi_opx_reliability_service * service);

void fi_reliability_service_ping_remote (struct fid_ep *ep, struct fi_opx_reliability_service * service);
unsigned fi_opx_reliability_service_poll_hfi1 (struct fid_ep *ep, struct fi_opx_reliability_service * service);

void fi_opx_reliability_service_process_pending (struct fi_opx_reliability_service * service);

#define RX_CMD	(0x0000000000000008ul)
#define TX_CMD	(0x0000000000000010ul)


#define FI_OPX_RELIABILITY_EXCEPTION	(0)
#define FI_OPX_RELIABILITY_EXPECTED	(1)


struct fi_opx_reliability_rx_uepkt {
	struct fi_opx_reliability_rx_uepkt *	prev;
	struct fi_opx_reliability_rx_uepkt *	next;
	uint64_t				psn;
	uint64_t				unused_0[5];

	/* == CACHE LINE == */

	uint64_t				unused_1;
	union fi_opx_hfi1_packet_hdr		hdr;	/* 56 bytes */

	/* == CACHE LINE == */

	uint8_t					payload[FI_OPX_HFI1_PACKET_MTU];

} __attribute__((__packed__)) __attribute__((aligned(64)));

union fi_opx_reliability_tx_psn {
	uint64_t value;
	struct {
		uint64_t	psn:24;
		uint64_t	throttle:8;
		uint64_t	nack_count:8;
		uint64_t	bytes_outstanding:24;
	} psn;
} __attribute__((__packed__));
 
// TODO - make these tunable.
#define FI_OPX_RELIABILITY_TX_REPLAY_BLOCKS	(2048)
#define FI_OPX_RELIABILITY_TX_REPLAY_IOV_BLOCKS	(8192)
#define OPX_MAX_OUTSTANDING_REPLAYS	(FI_OPX_RELIABILITY_TX_REPLAY_BLOCKS + FI_OPX_RELIABILITY_TX_REPLAY_IOV_BLOCKS)

#define OPX_REPLAY_BASE_SIZE			(sizeof(struct fi_opx_reliability_tx_replay))
#define OPX_REPLAY_IOV_SIZE			(sizeof(struct iovec) << 1)
#define OPX_REPLAY_PAYLOAD_SIZE			(FI_OPX_HFI1_PACKET_MTU + 64)
#define OPX_RELIABILITY_TX_REPLAY_SIZE		(OPX_REPLAY_BASE_SIZE + OPX_REPLAY_PAYLOAD_SIZE)
#define OPX_RELIABILITY_TX_REPLAY_IOV_SIZE	(OPX_REPLAY_BASE_SIZE + OPX_REPLAY_IOV_SIZE)

// Maximum PSNs to NACK when receiving an out of order packet or responding to a ping
#ifndef OPX_RELIABILITY_RX_MAX_NACK
#define OPX_RELIABILITY_RX_MAX_NACK		(1)
#endif

/*
 * For incoming packets:
 * Normally, we can simply add the high 40 bits of the receiver's 64-bit PSN
 * to the 24-bit PSN of an incoming packet to get the 64-bit PSN. However,
 * we have to deal with the case where the Sender's PSN has rolled over but
 * the receiver's hasn't. The difficulty is determining when this is the case.
 *
 * In the ideal case, the Sender's PSN will always be greater than or equal to
 * the PSN the receiver expects to see (the expected PSN). However, this is
 * not the case when the packet is a duplicate of one that was already
 * received. This can happen when a packet gets NACK'ed twice for some reason,
 * resulting in the sender sending two new copies. This is futher complicated
 * by the fact that the sender's replay list has a fixed size but does not
 * have to contain a continuous range of PSNs - there can be gaps. Thus it
 * is hard to recognize packets that are "too old" to be legitimate.
 *
 * Definitions:
 * R-PSN-64 = The next expected PSN.
 * R-PSN-24 = (R-PSN-64 & 0xFFFFFF)
 * S-PSN-24 = 24-bit OPA PSN from the incoming packet.
 * S-PSN-64 = 64-bit "scaled up" PSN of incoming packet.
 *
 * (Note: I'm deliberately not defining "Low", "High", or "Middle" yet.)
 *
 * S-PSN-24		R-PSN-24
 * Middle		Middle		S-PSN-64 = S-PSN-24 + (high 40 bits of R-PSN-64)
 *
 * Low			High		Sender has rolled over. Add an extra 2^24.
 * 							S-PSN-64 = S-PSN-24 + (high 40 bits of R-PSN-64) +
 * 							2^24
 *
 * High			Low			Packet is old and should be discarded. (The only
 * 							way for this to occur is if the R-PSN-24 has rolled
 * 							over but the incoming S-PSN-24 has not. Since
 * 							R-PSN-64 represents the highest PSN successfully
 * 							received, we must have already received this packet.
 *
 * The risks in defining "High" and "Low" is that if they are too generous we
 * might scale up S-PSN-24 incorrectly, causing us to discard a valid packet,
 * or assign a packet an invalid PSN (causing it to be processed in the wrong
 * order).
 *
 * First we will assume we will never have to deal with PSNs that are correctly
 * close to 2^24 apart because that would indicate much more fundamental
 * problems with the protocol - how did we manage to drop ~2^24 packets?
 *
 * This assumption allows us to be generous in how we define "Low" and "High"
 * for the Low/High case. If we define "Low" as < 10k and "High" as > "2^24-10k"
 * that should be adequate. Why 10K? We currently allocate 2048 replay buffers
 * with room for payloads, and 8192 replay buffers for IOV only, meaning a
 * sender could have up to 10K PSNs "in flight" at once.
 *
 * In the "High/Low" case, if we say S-PSN-64 = S-PSN-24 + (high 40 bits of
 * R-PSN-64) then S-PSN-64 will be much, much, higher than R-PSN-64. Presumably
 * close to 2^24 apart but it is hard to say "how close". Let's assume anything
 * greater than 2^23 is enough to reject a packet as "too old".
 *
 * Ping/Ack/Nack Handling:
 * Ping/Ack/Nack packets can hold 64-bit PSNs but the replay queue stores
 * packets it uses 24 bit PSNs. Therefore, a ping will contain a 24-bit PSN
 * and a count. To prevent rollover issues, code sending pings
 * will truncate the count in the case where the replay queue contains a
 * rollover. Because of this limitation the Ack and Nack replies are similarly
 * constrained.
 *
 */
#define MAX_PSN		0x0000000000FFFFFFull
#define MAX_PSN_MASK	0xFFFFFFFFFF000000ull

#define PSN_WINDOW_SIZE 0x0000000001000000ull
#define PSN_LOW_WINDOW	OPX_MAX_OUTSTANDING_REPLAYS
#define PSN_HIGH_WINDOW (PSN_WINDOW_SIZE - PSN_LOW_WINDOW)
#define PSN_AGE_LIMIT	0x0000000000F00000ull

struct fi_opx_reliability_client_state {

	union {
		enum ofi_reliability_kind		kind;			/* runtime check for fi_cq_read(), etc */
		uint64_t				pad;
	};
	// 8 bytes
	struct fi_opx_atomic_fifo_producer		fifo;			/* 6 qws = 48 bytes; only for OFI_RELIABILITY_KIND_OFFLOAD */
	// 56 bytes
	RbtHandle					tx_flow_rbtree;
	// 64 bytes
	RbtHandle					rx_flow_rbtree;
	// 72 bytes
	struct ofi_bufpool *		replay_pool; // for main data path
	struct ofi_bufpool *		replay_iov_pool; // for main data path
	// 88 bytes
	struct fi_opx_reliability_service *		service;
	void (*process_fn)(struct fid_ep *ep,
			   const union fi_opx_hfi1_packet_hdr * const hdr,
			   const uint8_t * const payload,
			   const uint8_t origin_rs);
	// 104 bytes
	uint32_t					lid_be;
	uint8_t						tx;
	uint8_t						rx;
	// 110 bytes
	/* -- not critical; only for debug, init/fini, etc. -- */
	uint16_t					drop_count;
	uint16_t					drop_mask;
	enum ofi_reliability_kind	reliability_kind;
	// 118 bytes
	RbtHandle	flow_rbtree_resynch;
	// 126 bytes
} __attribute__((__packed__)) __attribute__((aligned(64)));

void fi_opx_reliability_client_init (struct fi_opx_reliability_client_state * state,
		struct fi_opx_reliability_service * service,
		const uint8_t rx,
		const uint8_t tx,
		void (*process_fn)(struct fid_ep *ep,
				   const union fi_opx_hfi1_packet_hdr * const hdr,
				   const uint8_t * const payload,
				   const uint8_t origin_reliability_rx));

void fi_opx_reliability_client_fini (struct fi_opx_reliability_client_state * state);

__OPX_FORCE_INLINE__
unsigned fi_opx_reliability_client_active (struct fi_opx_reliability_client_state * state)
{
	return (state->service->reliability_kind != OFI_RELIABILITY_KIND_NONE) &&
		((state->replay_pool && !ofi_bufpool_empty(state->replay_pool)) ||
		 (state->replay_iov_pool && !ofi_bufpool_empty(state->replay_iov_pool)));
}

static inline
void fi_reliability_service_process_command (struct fi_opx_reliability_client_state *state,
		struct fi_opx_reliability_tx_replay * replay)
{
	union fi_opx_reliability_service_flow_key key = {
		.slid = replay->scb.hdr.stl.lrh.slid,
		.tx = FI_OPX_HFI1_PACKET_ORIGIN_TX(&replay->scb.hdr),
		.dlid = replay->scb.hdr.stl.lrh.dlid,
		.rx = replay->scb.hdr.stl.bth.rx
	};

	void * itr = NULL;

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) packet %016lx %08u posted.\n", key.value, FI_OPX_HFI1_PACKET_PSN(&replay->scb.hdr));
#endif

#ifndef NDEBUG
	itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void*)key.value);
	if (itr == NULL) {
		fprintf(stderr, "(%d) %s:%s():%d [%016lX] [slid=%04hX tx=%08X dlid=%04hX rx=%0hhX] Error trying to register replay for flow with no handshake!\n",
			getpid(), __FILE__, __func__, __LINE__,
			key.value,
			replay->scb.hdr.stl.lrh.slid,
			FI_OPX_HFI1_PACKET_ORIGIN_TX(&replay->scb.hdr),
			replay->scb.hdr.stl.lrh.dlid,
			replay->scb.hdr.stl.bth.rx);
		assert(itr);
	}
#endif
	/* search for existing unack'd flows */
	itr = fi_opx_rbt_find(state->service->tx.flow, (void*)key.value);
	if (OFI_UNLIKELY((itr == NULL))) {

		/* did not find an existing flow */
		replay->prev = replay;
		replay->next = replay;

		rbtInsert(state->service->tx.flow, (void*)key.value, (void*)replay);

#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr,"\nNew flow: %016lx\n", key.value);
#endif
		return;

	}

	struct fi_opx_reliability_tx_replay ** value_ptr =
		(struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(state->service->tx.flow, itr);

	struct fi_opx_reliability_tx_replay * head = *value_ptr;

	if (head == NULL) {

		/* the existing flow does not have any un-ack'd replay buffers */
		replay->prev = replay;
		replay->next = replay;
		*value_ptr = replay;

	} else {

		/* insert this replay at the end of the list */
		replay->prev = head->prev;
		replay->next = head;
		head->prev->next = replay;
		head->prev = replay;
	}
}

#ifdef OPX_RELIABILITY_TEST

#define FI_PSN_TO_DROP 0xfffff0

// Debugging tool that deliberately drops packets.
static inline
uint16_t fi_opx_reliability_rx_drop_packet (struct fi_opx_reliability_client_state * state,
	const union fi_opx_hfi1_packet_hdr *const hdr)
{
/*
 * Two variations of when to drop packets. The first drops a percentage of the
 * incoming packets. The other drops 1 specific packet every 2^24 packets (i.e.
 * according to the incoming PSN.) When debugging a reliability issue you can
 * use either of these or code up something different depending on what you're
 * trying to debug.
 */
#if 0
	// drops a percentage of the packets based on drop_mask.
	const uint16_t tmp = state->drop_count & state->drop_mask;

	if (tmp == 0)
		FI_WARN(fi_opx_global.prov,FI_LOG_EP_DATA, 
			"DEBUG: discarding packet %hu\n", state->drop_count);

	state->drop_count = tmp + 1;
	return !tmp;
#else
	// drops every other version of this packet(so reliability can shove it through)
	const uint64_t psn = hdr->reliability.psn;
	if (psn == FI_PSN_TO_DROP && (state->drop_count == 0)) {
		fprintf(stderr, "Dropping packet %lx\n", psn);
		state->drop_count++;
		return 1;
	} else if (psn == FI_PSN_TO_DROP) {
		fprintf(stderr, "Allowing replay packet %lx\n", psn);
		state->drop_count=0;
	}
	return 0;
#endif
}
#endif

#ifdef OPX_PING_DEBUG
void dump_ping_counts();
#endif

ssize_t fi_opx_hfi1_tx_reliability_inject_ud_init(struct fid_ep *ep,
						const uint64_t key,
						const uint64_t dlid,
						const uint64_t reliability_rx,
						const uint64_t opcode);

ssize_t fi_opx_hfi1_tx_reliability_inject_ud_resynch(struct fid_ep *ep,
						const uint64_t key,
						const uint64_t dlid,
						const uint64_t reliability_rx,
						const uint64_t opcode);

__OPX_FORCE_INLINE__
size_t fi_opx_reliability_replay_get_payload_size(struct fi_opx_reliability_tx_replay *replay)
{
	if (replay->use_iov) {
		return replay->iov->iov_len;
	}

	/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
	const uint16_t lrh_pktlen_le = ntohs(replay->scb.hdr.stl.lrh.pktlen);
	const size_t total_bytes = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
	return total_bytes - sizeof(union fi_opx_hfi1_packet_hdr);
}

__OPX_FORCE_INLINE__
void fi_opx_reliability_create_rx_flow(struct fi_opx_reliability_client_state * state,
					const uint64_t key,
					const uint8_t origin_rx)
{
	/* TODO - allocate from a pool of flow objects instead for better memory utilization */
	int rc __attribute__ ((unused));
	struct fi_opx_reliability_flow * flow = NULL;
	rc = posix_memalign((void **)&flow, 32, sizeof(*flow));
	assert(rc==0);

	flow->next_psn = 0;
	flow->key.value = key;
	flow->uepkt = NULL;
	flow->origin_rx = origin_rx;
	//ofi_spin_init(&flow->lock);

	rbtInsert(state->rx_flow_rbtree, (void*)key, (void*)flow);

	if (state->service->reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {
		fi_opx_atomic_fifo_produce(&state->fifo, (uint64_t)flow | RX_CMD);
	} else if (state->service->reliability_kind == OFI_RELIABILITY_KIND_ONLOAD) {
		rbtInsert(state->service->rx.flow, (void*)flow->key.value, (void*)flow);
	}
}

__OPX_FORCE_INLINE__
void fi_opx_reliability_handle_ud_init(struct fid_ep *ep,
					struct fi_opx_reliability_client_state *state,
					const union fi_opx_hfi1_packet_hdr *const hdr)
{
	union fi_opx_reliability_service_flow_key key = {
		.value = hdr->service.key
	};

	uint8_t origin_rx = hdr->service.origin_reliability_rx;

	void *itr = fi_opx_rbt_find(state->rx_flow_rbtree, (void*)key.value);

	if (itr == NULL) {
		fi_opx_reliability_create_rx_flow(state, key.value, origin_rx);
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(%d) %s:%s():%d [%016lX] Reliability Handshake RX Flow created!\n",
			getpid(), __FILE__, __func__, __LINE__,
			key.value);
#endif
	}

	fi_opx_hfi1_tx_reliability_inject_ud_init(ep, key.value, key.slid, origin_rx, FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT_ACK);
}

__OPX_FORCE_INLINE__
void fi_opx_reliability_handle_ud_init_ack(struct fi_opx_reliability_client_state *state,
					const union fi_opx_hfi1_packet_hdr *const hdr)
{
	/* Find the flow for this communication in flow_rbtree */
	union fi_opx_reliability_service_flow_key key = {
		.value = hdr->service.key
	};

	void * itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void*)key.value);

	if (!itr) {
		union fi_opx_reliability_tx_psn value;
		value.value = 0; // Initializes the whole union.
		rbtInsert(state->tx_flow_rbtree, (void*)key.value, (void*)value.value);
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(%d) %s:%s():%d [%016lX] Reliability Handshake Complete!\n",
			getpid(), __FILE__, __func__, __LINE__,
			key.value);
#endif
	}
}

/*
 * returns !0 if this packet is expected (success)
 * returns 0 on exception
 */
static inline
unsigned fi_opx_reliability_rx_check (struct fi_opx_reliability_client_state * state,
		uint64_t slid, uint64_t origin_tx, uint32_t psn,
		uint8_t *origin_rx)
{
	struct fi_opx_reliability_flow *flow;

	void *itr, *key_ptr;

	const union fi_opx_reliability_service_flow_key key = {
		.slid = slid,
		.tx = origin_tx,
		.dlid = state->lid_be,
		.rx = state->rx
	};

	itr = fi_opx_rbt_find(state->rx_flow_rbtree, (void*)key.value);
#ifdef OPX_RELIABILITY_DEBUG
	if (!itr) {
		fprintf(stderr, "(rx) packet %016lx %08u received but no flow for this found!\n", key.value, psn);
	}
#endif

	assert(itr);

	fi_opx_rbt_key_value(state->rx_flow_rbtree, itr, &key_ptr, (void **)&flow);

	if (((flow->next_psn & MAX_PSN) == psn) && (flow->uepkt == NULL)) {
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(rx) packet %016lx %08u received.\n", key.value, psn);
#endif
		*origin_rx = flow->origin_rx;
		flow->next_psn += 1;
		return FI_OPX_RELIABILITY_EXPECTED;
	}
	return FI_OPX_RELIABILITY_EXCEPTION;
}

void fi_opx_hfi1_rx_reliability_ping (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const uint64_t key, uint64_t psn_count, uint64_t psn_start,
		const uint64_t slid, const uint64_t rx);


void fi_opx_hfi1_rx_reliability_ack (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const uint64_t key, const uint64_t psn_count, const uint64_t psn_start);

void fi_opx_hfi1_rx_reliability_nack (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const uint64_t key, const uint64_t psn_count, const uint64_t psn_start);

void fi_opx_reliability_rx_exception (struct fi_opx_reliability_client_state * state,
		uint64_t slid, uint64_t origin_tx, uint32_t psn,
		struct fid_ep *ep, const union fi_opx_hfi1_packet_hdr * const hdr, const uint8_t * const payload);

ssize_t fi_opx_hfi1_tx_reliability_inject (struct fid_ep *ep,
		const uint64_t key, const uint64_t dlid, const uint64_t reliability_rx,
		const uint64_t psn_start, const uint64_t psn_count,
		const uint64_t opcode);

void fi_opx_hfi1_rx_reliability_send_pre_acks(struct fid_ep *ep, const uint64_t dlid,
					      const uint64_t reliability_rx, const uint64_t psn_start,
					      const uint64_t psn_count,
					      const union fi_opx_hfi1_packet_hdr *const hdr,
					      const uint8_t origin_rx);

void fi_opx_hfi1_rx_reliability_resynch (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		uint32_t origin_reliability_rx,
		const union fi_opx_hfi1_packet_hdr *const hdr);

void fi_opx_hfi1_rx_reliability_ack_resynch (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const union fi_opx_hfi1_packet_hdr *const hdr);

void opx_reliability_handshake_init(struct fid_ep *ep,
				    union fi_opx_reliability_service_flow_key key,
				    const uint64_t target_reliability_rx);

__OPX_FORCE_INLINE__
int32_t fi_opx_reliability_tx_max_outstanding () {
	// Eager buffer size is 32*262144
	// We'll do 28 * 262144 as a starting point
	// Empirically this survives a message rate test
	// with no packet loss with a single pair
	// in biband
	// TODO:  fetch this from the context info
	// TODO:  This buffer should be dynamically adjusted
	// back when we get a nack, and dyamically adjusted
	// forward when we have nothing outstanding as
	// a starting point for per destination windowing
	// max outstanding could be adjusted to zero until
	// all the packets are replayed, then this can
	// be adjusted back to it's base value.
	// Either way, there should be knobs and controls
	// to make this dynamic when packets are lost
	return 28*262144;
}

__OPX_FORCE_INLINE__
int32_t fi_opx_reliability_tx_max_nacks () {
	// TODO, make this tunable.
	return 0;
}

void fi_opx_reliability_inc_throttle_count();
void fi_opx_reliability_inc_throttle_nacks();
void fi_opx_reliability_inc_throttle_maxo();

__OPX_FORCE_INLINE__
bool opx_reliability_ready(struct fid_ep *ep,
			struct fi_opx_reliability_client_state * state,
			const uint64_t dlid,
			const uint64_t rx,
			const uint64_t target_reliability_rx,
			const enum ofi_reliability_kind reliability)
{

	/* Not using reliability, or it's Intranode */
	if (reliability == OFI_RELIABILITY_KIND_NONE || state->lid_be == dlid)
		return true;

	union fi_opx_reliability_service_flow_key key = {
		.slid = (uint32_t) state->lid_be,
		.tx = (uint32_t) state->tx,
		.dlid = (uint32_t) dlid,
		.rx = (uint32_t) rx,
	};

	void * itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void*)key.value);
	if (OFI_UNLIKELY(!itr)) {
		/* Reliability handshake is incomplete, initiate it */
		opx_reliability_handshake_init(ep, key, target_reliability_rx);
		return false;
	}

	return true;
}

__OPX_FORCE_INLINE__
int32_t fi_opx_reliability_tx_available_psns (struct fid_ep *ep,
					struct fi_opx_reliability_client_state * state,
					const uint64_t lid,
					const uint64_t rx,
					const uint64_t target_reliability_rx,
					union fi_opx_reliability_tx_psn **psn_ptr,
					uint32_t psns_to_get,
					uint32_t bytes_per_packet)
{
	assert(psns_to_get && psns_to_get <= 128);

	union fi_opx_reliability_service_flow_key key = {
		.slid = (uint32_t) state->lid_be,
		.tx = (uint32_t) state->tx,
		.dlid = (uint32_t) lid,
		.rx = (uint32_t) rx,
	};

	void * itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void*)key.value);
	if (OFI_UNLIKELY(!itr)) {
		/* We've never sent to this receiver, so initiate a reliability handshake
		with them. Once they create the receive flow on their end, and we receive
		their ack, we'll create the flow on our end and be able to send. */
		opx_reliability_handshake_init(ep, key, target_reliability_rx);
		return -1;
	}

	*psn_ptr = (union fi_opx_reliability_tx_psn *)fi_opx_rbt_value_ptr(state->tx_flow_rbtree, itr);

	/*
	 * We can leverage the fact athat every packet needs a packet sequence
	 * number before it can be sent to implement some simply throttling.
	 *
	 * If the throttle is on, or if the # of bytes outstanding exceeds
	 * a threshold, return an error.
	 */
	if(OFI_UNLIKELY((*psn_ptr)->psn.throttle != 0)) {
		return -1;
	}
	if(OFI_UNLIKELY((*psn_ptr)->psn.nack_count > fi_opx_reliability_tx_max_nacks())) {
		(*psn_ptr)->psn.throttle = 1;
		fi_opx_reliability_inc_throttle_count();
		fi_opx_reliability_inc_throttle_nacks();
		return -1;
	}
	uint32_t max_outstanding = fi_opx_reliability_tx_max_outstanding();
	if(OFI_UNLIKELY((*psn_ptr)->psn.bytes_outstanding > max_outstanding)) {
		(*psn_ptr)->psn.throttle = 1;
		fi_opx_reliability_inc_throttle_count();
		fi_opx_reliability_inc_throttle_maxo();
		return -1;
	}

	uint32_t bytes_avail = max_outstanding - (*psn_ptr)->psn.bytes_outstanding;
	return MIN(bytes_avail / bytes_per_packet, psns_to_get);
}

__OPX_FORCE_INLINE__
int32_t fi_opx_reliability_tx_next_psn (struct fid_ep *ep,
					struct fi_opx_reliability_client_state * state,
					const uint64_t lid,
					const uint64_t rx,
					const uint64_t target_reliability_rx,
					union fi_opx_reliability_tx_psn **psn_ptr,
					uint32_t psns_to_get)
{
	assert(psns_to_get && psns_to_get <= 128);
	uint32_t psn = 0;

	union fi_opx_reliability_service_flow_key key = {
		.slid = (uint32_t) state->lid_be,
		.tx = (uint32_t) state->tx,
		.dlid = (uint32_t) lid,
		.rx = (uint32_t) rx,
	};

	void * itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void*)key.value);
	if (OFI_UNLIKELY(!itr)) {
		/* We've never sent to this receiver, so initiate a reliability handshake
		   with them. Once they create the receive flow on their end, and we receive
		   their ack, we'll create the flow on our end and be able to send. */
		opx_reliability_handshake_init(ep, key, target_reliability_rx);
		return -1;
	} else {
		*psn_ptr = (union fi_opx_reliability_tx_psn *)fi_opx_rbt_value_ptr(state->tx_flow_rbtree, itr);
		union fi_opx_reliability_tx_psn  psn_value = **psn_ptr;

		/*
		 * We can leverage the fact athat every packet needs a packet sequence
		 * number before it can be sent to implement some simply throttling.
		 *
		 * If the throttle is on, or if the # of bytes outstanding exceeds
		 * a threshold, return an error.
		 */
		if(OFI_UNLIKELY((*psn_ptr)->psn.throttle != 0)) {
			return -1;
		}
		if(OFI_UNLIKELY((*psn_ptr)->psn.nack_count > fi_opx_reliability_tx_max_nacks())) {
			(*psn_ptr)->psn.throttle = 1;
			fi_opx_reliability_inc_throttle_count();
			return -1;
		}
		if(OFI_UNLIKELY((*psn_ptr)->psn.bytes_outstanding >
			fi_opx_reliability_tx_max_outstanding())) {
			(*psn_ptr)->psn.throttle = 1;
			fi_opx_reliability_inc_throttle_count();
			return -1;
		}

		psn = psn_value.psn.psn;
		(*psn_ptr)->psn.psn = (psn_value.psn.psn + psns_to_get) & MAX_PSN;
	}

	return psn;
}
__OPX_FORCE_INLINE__
struct fi_opx_reliability_tx_replay *
fi_opx_reliability_client_replay_allocate(struct fi_opx_reliability_client_state * state,
	const bool use_iov)
{
	struct fi_opx_reliability_tx_replay * return_value;

	if (!use_iov) {
		return_value = (struct fi_opx_reliability_tx_replay *)ofi_buf_alloc(state->replay_pool);
	} else {
		return_value = (struct fi_opx_reliability_tx_replay *)ofi_buf_alloc(state->replay_iov_pool);
	}
	if (OFI_LIKELY(return_value != NULL)) {
			return_value->nack_count = 0;
			return_value->pinned = false;
			return_value->acked = false;
			return_value->use_sdma = false;
			return_value->use_iov = use_iov;
			return_value->sdma_we = NULL;

			// This will implicitly set return_value->iov correctly
			return_value->payload = (uint64_t *) &return_value->data;
	}

	return return_value;
}

static inline
void fi_opx_reliability_client_replay_deallocate(struct fi_opx_reliability_client_state *state __attribute__((unused)),
	struct fi_opx_reliability_tx_replay * replay)
{
#ifdef OPX_RELIABILITY_DEBUG
	replay->next = replay->prev = 0;
#endif
	OPX_BUF_FREE(replay);
}

static inline
void fi_opx_reliability_client_replay_register_no_update (struct fi_opx_reliability_client_state * state,
		const uint16_t dlid, const uint8_t rs, const uint8_t rx, union fi_opx_reliability_tx_psn *psn_ptr,
		struct fi_opx_reliability_tx_replay * replay,
		const enum ofi_reliability_kind reliability_kind)
{
	const uint16_t lrh_pktlen_le = ntohs(replay->scb.hdr.stl.lrh.pktlen);
	const size_t total_bytes = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
	psn_ptr->psn.bytes_outstanding += total_bytes;
	replay->target_reliability_rx = rs;
	replay->psn_ptr = psn_ptr;

	replay->cc_ptr = NULL;
	replay->cc_dec = 0;

	// If this replay is pointing to an outside buffer instead of us copying
	// the payload into the replay's bounce buffer, there needs to be a completion
	// counter associated with the replay, and replay_register_with_update should
	// be used instead.
	assert(!replay->use_iov);

	if (reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {			/* constant compile-time expression */

#ifndef NDEBUG
		if ((uint64_t)replay & TX_CMD) { fprintf(stderr, "%s():%d abort\n", __func__, __LINE__); abort(); }
#endif
		/*
		 * ok to block .. the reliability service is completely non-blocking
		 * and will always consume from this atomic fifo
		 */
		fi_opx_atomic_fifo_produce(&state->fifo, (uint64_t)replay | TX_CMD);

	} else if (reliability_kind == OFI_RELIABILITY_KIND_ONLOAD  || reliability_kind == OFI_RELIABILITY_KIND_RUNTIME) {		/* constant compile-time expression */
		fi_reliability_service_process_command(state, replay);
	} else {
		fprintf(stderr, "%s():%d abort\n", __func__, __LINE__); abort();
	}

	return;
}


static inline
void fi_opx_reliability_client_replay_register_with_update (struct fi_opx_reliability_client_state * state,
		const uint16_t dlid, const uint8_t rs, const uint8_t rx, union fi_opx_reliability_tx_psn *psn_ptr,
		struct fi_opx_reliability_tx_replay * replay,
		struct fi_opx_completion_counter * counter, uint64_t value,
		const enum ofi_reliability_kind reliability_kind)
{
	const uint16_t lrh_pktlen_le = ntohs(replay->scb.hdr.stl.lrh.pktlen);
	const size_t total_bytes = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
	psn_ptr->psn.bytes_outstanding += total_bytes;
	replay->target_reliability_rx = rs;
	replay->psn_ptr = psn_ptr;
	replay->cc_ptr = counter;
	replay->cc_dec = value;

#ifndef NDEBUG
	if (replay->use_iov) {
		/* Copy up to 64 bytes of the current payload value for
		 * later comparison as a sanity check to make sure the
		 * user didn't alter the buffer */
		uint32_t copy_payload_qws = MIN(8, value >> 3);
		for (int i = 0; i < copy_payload_qws; ++i) {
			replay->orig_payload[i] = ((uint64_t *)replay->iov->iov_base)[i];
		}
	}
#endif
	/* constant compile-time expression */
	if (reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {

#ifndef NDEBUG
		if ((uint64_t)replay & TX_CMD) { fprintf(stderr, "%s():%d abort\n", __func__, __LINE__); abort(); }
#endif
		/*
		 * ok to block .. the reliability service is completely non-blocking
		 * and will always consume from this atomic fifo
		 */
		fi_opx_atomic_fifo_produce(&state->fifo, (uint64_t)replay | TX_CMD);

	} else if (reliability_kind == OFI_RELIABILITY_KIND_ONLOAD  || reliability_kind == OFI_RELIABILITY_KIND_RUNTIME) {		/* constant compile-time expression */

		fi_reliability_service_process_command(state, replay);

	} else {
		fprintf(stderr, "%s():%d abort\n", __func__, __LINE__); abort();
	}

	return;
}

ssize_t fi_opx_reliability_service_do_replay_sdma (struct fid_ep *ep,
						struct fi_opx_reliability_service *service,
						struct fi_opx_reliability_tx_replay *start,
						struct fi_opx_reliability_tx_replay *stop,
						uint32_t num_replays);

ssize_t fi_opx_reliability_service_do_replay (struct fi_opx_reliability_service * service,
					struct fi_opx_reliability_tx_replay * replay);


void fi_opx_hfi_rx_reliablity_process_requests(struct fid_ep *ep, int max_to_send);

ssize_t fi_opx_reliability_do_remote_ep_resynch(struct fid_ep *ep,
	union fi_opx_addr dest_addr,
	void *context,
	const uint64_t caps);

#endif /* _FI_PROV_OPX_RELIABILITY_H_ */
