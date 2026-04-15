/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2025 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_ENDPOINT_H_
#define _FI_PROV_OPX_ENDPOINT_H_

#include <stdint.h>
#include <pthread.h>
#include <sys/uio.h>

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_compiler.h"
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_reliability.h"
#include "rdma/opx/fi_opx_rma_ops.h"
#include "rdma/opx/fi_opx_match.h"
#include "rdma/opx/fi_opx_addr.h"
#include "rdma/opx/opx_hfisvc.h"
#include "rdma/opx/opx_tracer.h"
#include "rdma/opx/fi_opx_debug_counters.h"
#include "rdma/opx/fi_opx_flight_recorder.h"
#include "opx_shm.h"
#include "fi_opx_tid_cache.h"
#include "rdma/opx/opx_ipc.h"

void fi_opx_cq_debug(struct fid_cq *cq, char *func, const int line);

#define FI_OPX_KIND_TAG (0)
#define FI_OPX_KIND_MSG (1)

#define OPX_SHM_TRUE  (1)
#define OPX_SHM_FALSE (0)

#define OPX_CONTIG_TRUE	 (1)
#define OPX_CONTIG_FALSE (0)

#define OPX_FLAGS_OVERRIDE_TRUE	 (1)
#define OPX_FLAGS_OVERRIDE_FALSE (0)

#define OPX_MULTI_RECV_TRUE  (1)
#define OPX_MULTI_RECV_FALSE (0)

#define OPX_HMEM_TRUE  (1)
#define OPX_HMEM_FALSE (0)

// #define FI_OPX_TRACE 1
// #define FI_OPX_REMOTE_COMPLETION

/* #define IS_MATCH_DEBUG */

/* Macro for declaring a compile/constant fi_opx_addr based on AV type.
 * const FI_AV_MAP/FI_AV_TABLE compile optimized.
 * const FI_AV_UNSPEC requires a conditional pulling it out of the endpoint   */

#define FI_OPX_EP_AV_ADDR(const_av_type, ep, addr)      \
	{(const_av_type == FI_AV_TABLE) ?               \
		 ep->tx->av_addr[addr].fi :             \
		 ((const_av_type == FI_AV_MAP) ? addr : \
						 ((ep->av_type == FI_AV_TABLE) ? ep->tx->av_addr[addr].fi : addr))}

/* Macro indirection in order to support other macros as arguments
 * C requires another indirection for expanding macros since
 * operands of the token pasting operator are not expanded */

#define FI_OPX_MSG_SPECIALIZED_FUNC(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE, CTX_SHARING) \
	FI_OPX_MSG_SPECIALIZED_FUNC_(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE, CTX_SHARING)

#define FI_OPX_MSG_SPECIALIZED_FUNC_(LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE, CTX_SHARING)                              \
	static inline ssize_t fi_opx_send_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING(        \
		struct fid_ep *ep, const void *buf, size_t len, void *desc, fi_addr_t dest_addr, void *context)        \
	{                                                                                                              \
		return fi_opx_ep_tx_send(ep, buf, len, desc, dest_addr, 0, context, 0, LOCK, /* lock_required */       \
					 AV,						     /* av_type */             \
					 1,						     /* is_contiguous */       \
					 0,						     /* override_flags */      \
					 0,						     /* flags */               \
					 CAPS | FI_MSG, RELIABILITY, HFI1_TYPE, CTX_SHARING);                          \
	}                                                                                                              \
	static inline ssize_t fi_opx_recv_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING(        \
		struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr, void *context)               \
	{                                                                                                              \
		return fi_opx_recv_generic(ep, buf, len, desc, src_addr, 0, (uint64_t) - 1, context, LOCK, AV, FI_MSG, \
					   RELIABILITY, HFI1_TYPE, CTX_SHARING);                                       \
	}                                                                                                              \
	static inline ssize_t fi_opx_inject_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING(      \
		struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr)                                   \
	{                                                                                                              \
		return fi_opx_ep_tx_inject(ep, buf, len, dest_addr, 0, 0, LOCK, /* lock_required */                    \
					   AV,					/* av_type */                          \
					   0,					/* flags */                            \
					   CAPS | FI_MSG, RELIABILITY, HFI1_TYPE, CTX_SHARING);                        \
	}                                                                                                              \
	static inline ssize_t fi_opx_recvmsg_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING(     \
		struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)                                           \
	{                                                                                                              \
		return fi_opx_recvmsg_generic(ep, msg, flags, LOCK, AV, RELIABILITY, HFI1_TYPE, CTX_SHARING);          \
	}                                                                                                              \
	static inline ssize_t fi_opx_senddata_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING(    \
		struct fid_ep *ep, const void *buf, size_t len, void *desc, uint64_t data, fi_addr_t dest_addr,        \
		void *context)                                                                                         \
	{                                                                                                              \
		return fi_opx_ep_tx_send(ep, buf, len, desc, dest_addr, 0, context, data, LOCK, /* lock_required */    \
					 AV,							/* av_type */          \
					 1,							/* is_contiguous */    \
					 0,							/* override_flags */   \
					 FI_REMOTE_CQ_DATA,					/* flags */            \
					 CAPS | FI_MSG, RELIABILITY, HFI1_TYPE, CTX_SHARING);                          \
	}                                                                                                              \
	static inline ssize_t fi_opx_injectdata_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING(  \
		struct fid_ep *ep, const void *buf, size_t len, uint64_t data, fi_addr_t dest_addr)                    \
	{                                                                                                              \
		return fi_opx_ep_tx_inject(ep, buf, len, dest_addr, 0, data, LOCK, /* lock_required */                 \
					   AV,					   /* av_type */                       \
					   FI_REMOTE_CQ_DATA,			   /* flags */                         \
					   CAPS | FI_MSG, RELIABILITY, HFI1_TYPE, CTX_SHARING);                        \
	}

#define FI_OPX_MSG_SPECIALIZED_FUNC_NAME(TYPE, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE, CTX_SHARING) \
	FI_OPX_MSG_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE, CTX_SHARING)

#define FI_OPX_MSG_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY, HFI1_TYPE, CTX_SHARING) \
	fi_opx_##TYPE##_##LOCK##_##AV##_##CAPS##_##RELIABILITY##_##HFI1_TYPE##_##CTX_SHARING

enum fi_opx_ep_state { FI_OPX_EP_UNINITIALIZED = 0, FI_OPX_EP_INITITALIZED_DISABLED, FI_OPX_EP_INITITALIZED_ENABLED };

enum opx_work_type {
	OPX_WORK_TYPE_SDMA = 0, // SDMA should always be first value in enum
	OPX_WORK_TYPE_PIO,
	OPX_WORK_TYPE_SHM,
	OPX_WORK_TYPE_TID_SETUP,
	OPX_WORK_TYPE_HFISVC,
	OPX_WORK_TYPE_LAST
};

OPX_COMPILE_TIME_ASSERT(OPX_WORK_TYPE_SDMA == 0, "OPX_WORK_TYPE_SDMA needs to be 0/first value in the enum!");

static const char *const OPX_WORK_TYPE_STR[] = {
	[OPX_WORK_TYPE_SDMA] = "SDMA",		 [OPX_WORK_TYPE_PIO] = "PIO",	    [OPX_WORK_TYPE_SHM] = "SHM",
	[OPX_WORK_TYPE_TID_SETUP] = "TID_SETUP", [OPX_WORK_TYPE_HFISVC] = "HFISVC", [OPX_WORK_TYPE_LAST] = "LAST"};

/*
 * Describes state for an endpoint's software RHQ and software eager buffer queue.
 * Software will both read and write to this memory.
 */
struct opx_software_rx_q {
	// Software RHQ info
	volatile uint64_t *rhq_head_reg;   // Pointer to RHQ head register
	volatile uint64_t *rhq_tail_reg;   // Pointer to RHQ tail register
	uint32_t	  *hdrq_base_addr; // base address of the subcontext's SW RHQ
	uint32_t	  *rhf_base;	   // Base address for SW RHQ RHF

	// Software Eager queue info
	volatile uint64_t *egrq_head;	 // Pointer to eager buffer head register
	volatile uint64_t *egrq_tail;	 // Pointer to eager buffer tail register
	uint32_t	  *egr_buf_base; // Base of this subcontext's eager buffer memory
	uint64_t	   unused;
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));

/*
 * This structure layout ensures that the 'fi_tinject()' function will only
 * touch 2 cachelines - one from this structure and one to obtain the pio
 * state information.
 *
 * This structure layout ensures that the 'fi_tsend()' function will only
 * touch 3 cachelines - two from this structure and one to obtain the pio
 * state information. Additional cachelines will be touched if a completion
 * queue entry is requested.
 *
 * 'fi_inject()' -> 3 cachelines
 * 'fi_send()'   -> 4 cachelines
 */
struct fi_opx_ep_tx {
	/* == CACHE LINE 0 == */

	volatile union fi_opx_hfi1_pio_state *pio_state; /* 1 qw = 8 bytes */
	volatile uint64_t		     *pio_scb_sop_first;

	volatile uint64_t *pio_credits_addr; /* const; only used to infrequently "refresh" credit information */
	volatile uint64_t *pio_scb_first;    /* const; only eager and rendezvous */
	struct slist	  *cq_completed_ptr;
	uint32_t	   rzv_min_payload_bytes;
	uint32_t	   mp_eager_max_payload_bytes;
	uint16_t	   mp_eager_min_payload_bytes;
	uint16_t	   mp_eager_chunk_size;
	uint16_t	   pio_max_eager_tx_bytes;
	uint16_t	   pio_flow_eager_tx_bytes;
	uint32_t	   unused_cacheline1;
	uint8_t		   do_cq_completion;
	uint8_t		   force_credit_return;
	uint8_t		   use_sdma;
	uint8_t		   unused_cacheline1_1;

	struct {
		/* == CACHE LINE 1,2 == */
		union {
			struct fi_opx_hfi1_txe_scb_9B  inject_9B; /* qws 5,6, and 7 specified at runtime */
			struct fi_opx_hfi1_txe_scb_16B inject_16B;
		};

		/* == CACHE LINE 3,4 == */
		union {
			struct fi_opx_hfi1_txe_scb_9B  send_9B;
			struct fi_opx_hfi1_txe_scb_16B send_16B;
		};

		/* == CACHE LINE 5,6 == */
		union {
			struct fi_opx_hfi1_txe_scb_9B  send_mp_9B;
			struct fi_opx_hfi1_txe_scb_16B send_mp_16B;
		};

		/* == CACHE LINE 7,8 == */
		union {
			struct fi_opx_hfi1_txe_scb_9B  rzv_9B;
			struct fi_opx_hfi1_txe_scb_16B rzv_16B;
		};
	};

	/* == CACHE LINE 9 == */

	union fi_opx_addr *av_addr; /* only FI_ADDR_TABLE */
	uint64_t	   op_flags;
	uint64_t	   caps;
	uint64_t	   mode;
	uint64_t	   cq_bind_flags;
	struct slist	  *cq_err_ptr;
	struct fi_opx_cq  *cq;
	struct slist	  *cq_pending_ptr; /* only rendezvous (typically) */

	/* == CACHE LINE 10 == */

	struct slist	    work_pending_completion;
	struct ofi_bufpool *work_pending_pool;
	struct ofi_bufpool *rma_payload_pool;
	struct ofi_bufpool *rma_request_pool;
	struct ofi_bufpool *sdma_work_pool;
	uint32_t	    sdma_min_payload_bytes;
	uint32_t	    tid_min_payload_bytes;
	uint64_t	    unused_cacheline6_1;

	/* == CACHE LINE 11 == */
	struct opx_sdma_queue sdma_request_queue;
	struct slist	      sdma_pending_queue;
	struct ofi_bufpool   *sdma_request_pool;
	uint16_t	      sdma_max_writevs_per_cycle;
	uint16_t	      sdma_max_iovs_per_writev;
	uint16_t	      sdma_max_pkts_tid;
	uint16_t	      sdma_max_pkts;
	uint32_t	      sdma_bounce_buf_threshold;
	uint32_t	      unused_cacheline7;

	/* == CACHE LINE 12, 13+ == */

	struct slist	      work_pending[OPX_WORK_TYPE_LAST];
	int64_t		      ref_cnt;
	struct opx_spio_ctrl *spio_ctrl;
	// struct opx_shm_tx is very large and should go last!
	struct opx_shm_tx shm;
	void		 *mem;
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));

OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, inject_9B) == (FI_OPX_CACHE_LINE_SIZE * 1),
			"Offset of fi_opx_ep_tx->inject_9B should start at cacheline 1!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, send_9B) == (FI_OPX_CACHE_LINE_SIZE * 3),
			"Offset of fi_opx_ep_tx->send_9B should start at cacheline 3!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, send_mp_9B) == (FI_OPX_CACHE_LINE_SIZE * 5),
			"Offset of fi_opx_ep_tx->send_mp_9B should start at cacheline 5!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, rzv_9B) == (FI_OPX_CACHE_LINE_SIZE * 7),
			"Offset of fi_opx_ep_tx->rzv_9B should start at cacheline 7!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, inject_16B) == (FI_OPX_CACHE_LINE_SIZE * 1),
			"Offset of fi_opx_ep_tx->inject_16B should start at cacheline 1!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, send_16B) == (FI_OPX_CACHE_LINE_SIZE * 3),
			"Offset of fi_opx_ep_tx->send_16B should start at cacheline 3!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, send_mp_16B) == (FI_OPX_CACHE_LINE_SIZE * 5),
			"Offset of fi_opx_ep_tx->send_mp_16B should start at cacheline 5!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, rzv_16B) == (FI_OPX_CACHE_LINE_SIZE * 7),
			"Offset of fi_opx_ep_tx->rzv_16B should start at cacheline 7!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, av_addr) == (FI_OPX_CACHE_LINE_SIZE * 9),
			"Offset of fi_opx_ep_tx->av_addr should start at cacheline 9!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, work_pending_completion) == (FI_OPX_CACHE_LINE_SIZE * 10),
			"Offset of fi_opx_ep_tx->work_pending_completion should start at cacheline 10!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, sdma_request_queue) == (FI_OPX_CACHE_LINE_SIZE * 11),
			"Offset of fi_opx_ep_tx->sdma_request_queue should start at cacheline 11!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_tx, work_pending) == (FI_OPX_CACHE_LINE_SIZE * 12),
			"Offset of fi_opx_ep_tx->work_pending should start at cacheline 12!");

struct fi_opx_ep_rx {
	/* == CACHE LINE 0 == */

	/*
	 * NOTE: This cacheline is used when a thread is INITIATING
	 * receive operations
	 */
	uint64_t	   op_flags;
	union fi_opx_addr *av_addr;
	uint64_t	   unused_cacheline_0[6];

	/*
	 * NOTE: The following 2 cachelines are shared between the application-facing
	 * functions, such as 'fi_trecv()', and the progress functions, such as
	 * those invoked during 'fi_cq_read()'.
	 */

	/* == CACHE LINE 1 & 2 == */

	struct {
		struct fi_opx_hfi1_ue_packet_slist ue; /* 3 qws */
		struct slist			   mq; /* 2 qws */
	} queue[2];				       /* 0 = FI_TAGGED, 1 = FI_MSG */

	struct {
		struct fi_opx_hfi1_ue_packet_slist ue; /* 3 qws */
		struct slist			   mq; /* 2 qws */
	} mp_egr_queue;

	struct fi_opx_match_ue_hash *match_ue_tag_hash;

	/* == CACHE LINE 3 == */
	struct slist	   *cq_pending_ptr;
	struct slist	   *cq_completed_ptr;
	struct ofi_bufpool *ue_packet_pool;
	struct ofi_bufpool *ctx_pool;

	uint64_t unused_cacheline_3[4];
	/* == CACHE LINE 4 == */

	/*
	 * NOTE: This cacheline is used when a thread is making PROGRESS to
	 * process fabric events.
	 */
	struct fi_opx_hfi1_rxe_state state; /* 2 qws */

	struct {
		uint32_t	  *rhf_base;
		uint64_t	  *rhe_base;
		volatile uint64_t *head_register;
	} hdrq;

	struct {
		uint32_t	  *base_addr;
		uint32_t	   elemsz;
		uint32_t	   last_egrbfr_index;
		volatile uint64_t *head_register;
	} egrq __attribute__((__packed__));

	/* == CACHE LINE 5 - 12 == */

	/*
	 * NOTE: These cachelines are shared between the application-facing
	 * functions, such as 'fi_trecv()', and the progress functions, such as
	 * those invoked during 'fi_cq_read()'.
	 *
	 * This 'tx' information is used when sending acks, etc.
	 */
	struct {
		union {
			struct fi_opx_hfi1_txe_scb_9B  dput_9B;
			struct fi_opx_hfi1_txe_scb_16B dput_16B;
		};
		union {
			struct fi_opx_hfi1_txe_scb_9B  rzv_dput_9B;
			struct fi_opx_hfi1_txe_scb_16B rzv_dput_16B;
		};
		union {
			struct fi_opx_hfi1_txe_scb_9B  cts_9B;
			struct fi_opx_hfi1_txe_scb_16B cts_16B;
		};
		union {
			struct fi_opx_hfi1_txe_scb_9B  rma_rts_9B;
			struct fi_opx_hfi1_txe_scb_16B rma_rts_16B;
		};
	} tx;

	/* == CACHE LINE 13 == */
	struct {
		struct opx_hwcontext_ctrl *hwcontext_ctrl;
		// Head index into endpoint's software rx RHQ
		uint64_t head;
		// Next expected sequence number in endpoint's software rx RHQ
		uint64_t	   rhf_seq;
		uint32_t	  *rhf_base;
		uint32_t	  *eager_buf_base;
		volatile uint64_t *rhq_head_reg;
		volatile uint64_t *eager_head_reg;
		uint32_t	   last_egrbfr_index;
		uint8_t		   subctxt;
		uint8_t		   unused[3];
		/* == CACHE LINE 22 == */
		struct opx_subcontext_ureg *subcontext_ureg[HFI1_MAX_SHARED_CTXTS];
		/* == CACHE LINE 23 == */
		struct opx_software_rx_q soft_rx_qs[HFI1_MAX_SHARED_CTXTS];
	} shd_ctx;

	/* -- non-critical -- */
	/* == CACHE LINE 23 == */
	uint64_t	      min_multi_recv;
	struct fi_opx_domain *domain;

	uint64_t caps;
	uint64_t mode;
	// This is the endpoint's fabric address
	union fi_opx_addr self;

	struct slist	 *cq_err_ptr;
	struct fi_opx_cq *cq;

	struct opx_shm_rx shm;
	void		 *mem;
	int64_t		  ref_cnt;
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));

OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_rx, queue) == FI_OPX_CACHE_LINE_SIZE,
			"Offset of fi_opx_ep_rx->queue should start at cacheline 1!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_rx, cq_pending_ptr) == (FI_OPX_CACHE_LINE_SIZE * 3),
			"Offset of fi_opx_ep_rx->queue should start at cacheline 3!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_rx, state) == (FI_OPX_CACHE_LINE_SIZE * 4),
			"Offset of fi_opx_ep_rx->queue should start at cacheline 4!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_rx, tx) == (FI_OPX_CACHE_LINE_SIZE * 5),
			"Offset of fi_opx_ep_rx->tx should start at cacheline 5!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_rx, shd_ctx) == (FI_OPX_CACHE_LINE_SIZE * 13),
			"Offset of fi_opx_ep_rx->shd_ctx should start at cacheline 13!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep_rx, min_multi_recv) == (FI_OPX_CACHE_LINE_SIZE * 23),
			"Offset of fi_opx_ep_rx->min_multi_recv should start at cacheline 23!");

struct fi_opx_daos_av_rank_key {
	uint32_t rank;
	uint32_t rank_inst;
};

struct fi_opx_daos_av_rank {
	struct fi_opx_daos_av_rank_key key;
	uint32_t		       updated;
	fi_addr_t		       fi_addr;
	UT_hash_handle		       hh; /* makes this structure hashable */
};

struct fi_opx_ep_daos_info {
	struct fi_opx_daos_av_rank *av_rank_hashmap;
	uint32_t		    rank_inst;
	uint32_t		    rank;
	bool			    do_resynch_remote_ep;
	bool			    hfi_rank_enabled;
} __attribute__((__packed__));

/*
 * The 'fi_opx_ep' struct defines an endpoint with a single tx context and a
 * single rx context. The tx context is only valid if the FI_READ, FI_WRITE,
 * or FI_SEND capability is specified. The rx context is only valid if the
 * FI_RECV, FI_REMOTE_READ, or FI_REMOTE_WRITE flags are specified.
 *
 * A 'scalable tx context' is simply an endpoint structure with only the
 * tx flags specified, and a 'scalable rx context' is simply an endpoint
 * structure with only the rx flags specified.
 *
 * As such, multiple OFI 'classes' share this endpoint structure:
 *   FI_CLASS_EP
 *   FI_CLASS_TX_CTX
 *   --- no FI_CLASS_STX_CTX
 *   FI_CLASS_RX_CTX
 *   -- no FI_CLASS_SRX_CTX
 */
struct fi_opx_ep {
	/* == CACHE LINE 0,1 == */
	struct fid_ep			   ep_fid; /* 10 qws */
	uint64_t			   rpc_reserved;
	struct fi_opx_ep_tx		  *tx;
	struct fi_opx_ep_rx		  *rx;
	struct fi_opx_reliability_service *reli_service; /* ONLOAD only */
	struct fi_opx_cntr		  *read_cntr;
	struct fi_opx_cntr		  *write_cntr;

	/* == CACHE LINE 2 == */
	struct fi_opx_cntr    *recv_cntr;
	struct fi_opx_cntr    *send_cntr;
	struct fi_opx_domain  *domain;
	struct opx_tid_domain *tid_domain;
	struct ofi_bufpool    *rma_counter_pool;
	struct ofi_bufpool    *rzv_completion_pool;
	struct fi_opx_av      *av;
	struct fi_opx_sep     *sep;

	/* == CACHE LINE 3 == */
	struct fi_opx_hfi1_context *hfi;
	uint8_t			   *hmem_copy_buf;

	int		     sep_index;
	enum fi_opx_ep_state state;

	uint32_t	threading;
	uint32_t	av_type;
	uint32_t	mr_mode;
	enum fi_ep_type type;
	uint64_t	unused_cacheline3[3];

	/* == CACHE LINE 4 == */
	// Only used for initialization
	// free these flags
	struct fi_info	   *common_info;
	struct fi_info	   *tx_info;
	struct fi_info	   *rx_info;
	struct fi_opx_cq   *init_tx_cq;
	struct fi_opx_cq   *init_rx_cq;
	struct fi_opx_cntr *init_read_cntr;
	struct fi_opx_cntr *init_write_cntr;
	uint64_t	    rx_cq_bflags;

	/* == CACHE LINE 5 == */
	struct fi_opx_cntr	  *init_send_cntr;
	struct fi_opx_cntr	  *init_recv_cntr;
	uint64_t		   tx_cq_bflags;
	struct fi_opx_ep_daos_info daos_info; /* 18 bytes */
	bool			   is_tx_cq_bound;
	bool			   is_rx_cq_bound;
	bool			   use_expected_tid_rzv;
	bool			   use_hfisvc;
	uint8_t			   unused_cacheline5[2];
	enum fi_hmem_iface	   use_gpu_ipc;
	ofi_spin_t		   lock; /* lock size varies based on ENABLE_DEBUG*/

	/* == CACHE LINE 6 == */
#if HAVE_HFISVC
	struct {
		/**
		 * @brief Command queue used by this endpoint to submit commands to the hfisvc
		 */
		hfisvc_client_command_queue_t command_queue;

		/**
		 * @brief Completion queue used by the endpoint for handling completions from the
		 * hfisvc where we need to do additional work. For example, we may need to free certain
		 * resources before sending a completion up to the user/middleware.
		 */
		hfisvc_client_completion_queue_t internal_completion_queue;

		/**
		 * @brief Pointer to the CQ's hfisvc completion queue. Use this completion queue
		 * when no further action is needed before bubbling up the completion to
		 * the user/middleware.
		 */
		hfisvc_client_completion_queue_t *cq_completion_queue;

		uint32_t rdma_read_count;
		uint32_t unused_dw;

		uint64_t unused_qws[4];
	} hfisvc;
#endif

	/* == CACHE LINE 7 (if ENABLE_DEBUG) == */
#ifdef FLIGHT_RECORDER_ENABLE
	struct flight_recorder *fr;
#endif

	FI_OPX_DEBUG_COUNTERS_DECLARE_COUNTERS;

	void *mem;
} __attribute((aligned(L2_CACHE_LINE_SIZE)));

OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep, recv_cntr) == (FI_OPX_CACHE_LINE_SIZE * 2),
			"Offset of fi_opx_ep->recv_cntr should start at cacheline 2!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep, hfi) == (FI_OPX_CACHE_LINE_SIZE * 3),
			"Offset of fi_opx_ep->hfi should start at cacheline 3!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep, common_info) == (FI_OPX_CACHE_LINE_SIZE * 4),
			"Offset of fi_opx_ep->hfi should start at cacheline 4!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep, init_send_cntr) == (FI_OPX_CACHE_LINE_SIZE * 5),
			"Offset of fi_opx_ep->init_send_cntr should start at cacheline 5!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_ep, lock) == ((FI_OPX_CACHE_LINE_SIZE * 5) + 52),
			"Offset of fi_opx_ep->lock should start before cacheline 6!");

/*
 * A 'scalable endpoint' may not be directly specified in a data movement
 * functions, such as fi_tsend(), as it is only a container for multiple
 * tx and rx contexts.
 *
 * The scalable contexts share certain resources, such as the address vector.
 */
struct fi_opx_sep {
	struct fid_ep ep_fid;

	struct fi_opx_domain	     *domain;
	struct fi_opx_av	     *av;
	struct fi_info		     *info;
	void			     *memptr;
	struct fi_opx_ep	     *ep[FI_OPX_ADDR_SEP_RX_MAX];
	struct fi_opx_hfi1_context   *hfi1[FI_OPX_ADDR_SEP_RX_MAX];
	struct fi_opx_ep_reliability *reliability[FI_OPX_ADDR_SEP_RX_MAX];
	struct fi_opx_ep_tx	     *tx[FI_OPX_ADDR_SEP_RX_MAX];
	struct fi_opx_ep_rx	     *rx[FI_OPX_ADDR_SEP_RX_MAX];

	int64_t ref_cnt;

} __attribute((aligned(L2_CACHE_LINE_SIZE)));

struct fi_opx_rzv_completion {
	struct opx_context *context;
	union {
		struct {
			uint64_t tid_length;
			uint64_t tid_vaddr;
		};
		struct {
			// uintptr_t app_context;
			uint64_t unused;
			uint32_t access_key;
			uint32_t unused_also;
		};
	};
	uint64_t byte_counter;
	uint64_t bytes_accumulated;
};

struct fi_opx_rma_request {
	union {
		struct fi_opx_completion_counter *cc;
		struct opx_context		 *context;
	};
	uint64_t	   hmem_device;
	enum fi_hmem_iface hmem_iface;
	uint32_t	   padding;
	uint64_t	   hmem_handle;
};

/*
 * =========================== begin: no-inline functions ===========================
 */

__attribute__((noinline)) void
fi_opx_ep_rx_process_context_noinline(struct fi_opx_ep *opx_ep, const uint64_t static_flags,
				      struct opx_context *context, const uint64_t rx_op_flags, const uint64_t is_hmem,
				      const int lock_required, const enum fi_av_type av_type,
				      const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hf1_type);

void fi_opx_ep_rx_process_header_tag(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
				     const uint8_t *const payload, const size_t payload_bytes, const uint8_t opcode,
				     const unsigned is_shm, const int lock_required,
				     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hf1_type,
				     opx_lid_t slid);

void fi_opx_ep_rx_process_header_msg(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
				     const uint8_t *const payload, const size_t payload_bytes, const uint8_t opcode,
				     const unsigned is_shm, const int lock_required,
				     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hf1_type,
				     opx_lid_t slid);

void fi_opx_ep_rx_reliability_process_packet(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
					     const uint8_t *const payload);

void fi_opx_ep_rx_append_ue_msg(struct fi_opx_ep_rx *const rx, const union opx_hfi1_packet_hdr *const hdr,
				const union fi_opx_hfi1_packet_payload *const payload, const size_t payload_bytes,
				const unsigned is_shm, const uint32_t rank, const uint32_t rank_inst,
				const bool daos_enabled, struct fi_opx_debug_counters *debug_counters,
				const opx_lid_t slid, const uint64_t rcv_time_ns);

void fi_opx_ep_rx_append_ue_tag(struct fi_opx_ep_rx *const rx, const union opx_hfi1_packet_hdr *const hdr,
				const union fi_opx_hfi1_packet_payload *const payload, const size_t payload_bytes,
				const unsigned is_shm, const uint32_t rank, const uint32_t rank_inst,
				const bool daos_enabled, struct fi_opx_debug_counters *debug_counters,
				const opx_lid_t slid, const uint64_t rcv_time_ns);

void fi_opx_ep_rx_append_ue_egr(struct fi_opx_ep_rx *const rx, const union opx_hfi1_packet_hdr *const hdr,
				const union fi_opx_hfi1_packet_payload *const payload, const size_t payload_bytes,
				const opx_lid_t slid);

int fi_opx_ep_tx_check(struct fi_opx_ep_tx *tx, enum fi_av_type av_type);

/*
 * =========================== end: no-inline functions ===========================
 */

__OPX_FORCE_INLINE__
void fi_opx_ep_clear_credit_return(struct fi_opx_ep *opx_ep)
{
	if (OFI_UNLIKELY(opx_ep->tx->force_credit_return)) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "======================================= Forced a credit return\n");
		opx_ep->tx->force_credit_return = 0;
	}
}

#define FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep) fi_opx_ep_clear_credit_return(opx_ep)

#include "rdma/opx/fi_opx_fabric_transport.h"

#ifdef OPX_DAOS_DEBUG
static void fi_opx_dump_daos_av_addr_rank(struct fi_opx_ep *opx_ep, const union fi_opx_addr find_addr,
					  const char *title)
{
	if (opx_ep->daos_info.av_rank_hashmap) {
		struct fi_opx_daos_av_rank *cur_av_rank = NULL;
		struct fi_opx_daos_av_rank *tmp_av_rank = NULL;
		int			    i = 0, found = 0;

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "%s Dump av_rank_hashmap (rank:%d LID:0x%x fi_addr:0x%08lx)\n", title,
			     opx_ep->daos_info.rank, find_addr.lid, find_addr.fi);

		HASH_ITER(hh, opx_ep->daos_info.av_rank_hashmap, cur_av_rank, tmp_av_rank)
		{
			if (cur_av_rank) {
				union fi_opx_addr addr;
				addr.fi = cur_av_rank->fi_addr;

				if ((addr.lid == find_addr.lid) && (cur_av_rank->key.rank == opx_ep->daos_info.rank)) {
					found = 1;
					FI_DBG_TRACE(
						fi_opx_global.prov, FI_LOG_EP_DATA,
						"Dump av_rank_hashmap[%d] = rank:%d LID:0x%x fi_addr:0x%08lx - Found.\n",
						i++, cur_av_rank->key.rank, addr.lid, addr.fi);
				} else {
					FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
						     "Dump av_rank_hashmap[%d] = rank:%d LID:0x%x fi:0x%08lx.\n", i++,
						     cur_av_rank->key.rank, addr.lid, addr.fi);
				}
			}
		}

		if (!found) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "Dump av_rank_hashmap - rank:%d LID:0x%x fi_addr:0x%08lx - Not found.\n",
				     opx_ep->daos_info.rank, find_addr.lid, find_addr.fi);
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s Dump av_rank_hashmap (completed)\n\n", title);
	}
}
#endif

#ifdef OPX_DAOS
static struct fi_opx_daos_av_rank *fi_opx_get_daos_av_rank(struct fi_opx_ep *opx_ep, uint32_t rank, uint32_t rank_inst)
{
	struct fi_opx_daos_av_rank_key key;
	struct fi_opx_daos_av_rank    *av_rank = NULL;
	/*
	 * DAOS Persistent Address Support:
	 * No Context Resource Management Framework is supported by OPX to enable
	 * acquiring a context with attributes that exactly match the specified
	 * source address.
	 *
	 * Therefore, treat the source address as an ‘opaque’ ID, so reference the
	 * rank data associated with the source address, which maps to the appropriate
	 * HFI and HFI port.
	 */
	key.rank      = rank;
	key.rank_inst = rank_inst;

	HASH_FIND(hh, opx_ep->daos_info.av_rank_hashmap, &key, sizeof(key), av_rank);

#ifdef IS_MATCH_DEBUG
	if (av_rank) {
		union fi_opx_addr addr;

		addr.fi = av_rank->fi_addr;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Found AV rank - rank:%d, LID:0x%x, fi_addr:%08lx.\n",
			     av_rank->key.rank, addr.lid, addr.fi);
	} else if (opx_ep->daos_info.av_rank_hashmap) {
		struct fi_opx_daos_av_rank *cur_av_rank = NULL;
		struct fi_opx_daos_av_rank *tmp_av_rank = NULL;
		int			    i		= 0;

		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "AV hash lookup of rank %d failed.\n", key.rank);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "GET Dump av_rank_hashmap (rank:%d)\n", key.rank);

		HASH_ITER(hh, opx_ep->daos_info.av_rank_hashmap, cur_av_rank, tmp_av_rank)
		{
			if (cur_av_rank) {
				union fi_opx_addr addr;
				addr.fi = cur_av_rank->fi_addr;

				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "GET Dump av_rank_hashmap[%d] = rank:%d LID:0x%x fi_addr:0x%08lx\n", i++,
					     cur_av_rank->key.rank, addr.lid, addr.fi);

				if (cur_av_rank->key.rank == key.rank) {
					FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
						"AV linear lookup of rank %d succeeded.\n", key.rank);
					return cur_av_rank;
				}
			}
		}
	}
#endif

	return av_rank;
}

#endif

__OPX_FORCE_INLINE__
uint64_t fi_opx_ep_is_matching_packet(const uint64_t origin_tag, const opx_lid_t origin_lid, const uint16_t origin_rx,
				      const uint64_t ignore, const uint64_t target_tag_and_not_ignore,
				      const uint64_t any_addr, const union fi_opx_addr src_addr,
				      struct fi_opx_ep *opx_ep, uint32_t rank, uint32_t rank_inst,
				      const unsigned is_shm)
{
	const uint64_t origin_tag_and_not_ignore = origin_tag & ~ignore;

	return (origin_tag_and_not_ignore == target_tag_and_not_ignore) &&
	       ((any_addr) || ((origin_lid == src_addr.lid) && (origin_rx == src_addr.hfi1_subctxt_rx))
#ifdef OPX_DAOS
		|| (opx_ep->daos_info.hfi_rank_enabled && is_shm && fi_opx_get_daos_av_rank(opx_ep, rank, rank_inst))
#endif
	       );
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_ue_packet *fi_opx_ep_find_matching_packet(struct fi_opx_ep *opx_ep, struct opx_context *context,
							     const uint64_t kind, const enum opx_hfi1_type hfi1_type)
{
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.match.default_searches);
	struct fi_opx_hfi1_ue_packet *uepkt = opx_ep->rx->queue[kind].ue.head;

	if (!uepkt) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.match.default_not_found);
		return NULL;
	}

	const union fi_opx_addr src_addr		  = {.fi = context->src_addr};
	const uint64_t		ignore			  = context->ignore;
	const uint64_t		target_tag_and_not_ignore = context->tag & ~ignore;
	const uint64_t		any_addr		  = (context->src_addr == FI_ADDR_UNSPEC);

	while (uepkt && !fi_opx_ep_is_matching_packet(
				uepkt->tag, uepkt->lid, uepkt->subctxt_rx, ignore, target_tag_and_not_ignore, any_addr,
				src_addr, opx_ep, uepkt->daos_info.rank, uepkt->daos_info.rank_inst, uepkt->is_shm)) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.match.default_misses);
		uepkt = uepkt->next;
	}

	FI_OPX_DEBUG_COUNTERS_INC_COND(uepkt, opx_ep->debug_counters.match.default_hits);
	FI_OPX_DEBUG_COUNTERS_INC_COND(!uepkt, opx_ep->debug_counters.match.default_not_found);

	return uepkt;
}

__OPX_FORCE_INLINE__
uint64_t is_match(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr, struct opx_context *context,
		  uint32_t rank, uint32_t rank_inst, unsigned is_shm, const opx_lid_t slid)
{
	const union fi_opx_addr src_addr		  = {.fi = context->src_addr};
	const uint64_t		ignore			  = context->ignore;
	const uint64_t		target_tag		  = context->tag;
	const uint64_t		origin_tag		  = hdr->match.ofi_tag;
	const uint64_t		target_tag_and_not_ignore = target_tag & ~ignore;
	const uint64_t		origin_tag_and_not_ignore = origin_tag & ~ignore;

	const uint64_t answer =
		((origin_tag_and_not_ignore == target_tag_and_not_ignore) &&
		 ((context->src_addr == FI_ADDR_UNSPEC) ||
		  ((slid == src_addr.lid) && (hdr->reliability.origin_rx == src_addr.hfi1_subctxt_rx))
#ifdef OPX_DAOS
		  || (opx_ep->daos_info.hfi_rank_enabled && is_shm && fi_opx_get_daos_av_rank(opx_ep, rank, rank_inst))
#endif
			  ));

#ifdef IS_MATCH_DEBUG
	fprintf(stderr,
		"%s:%s():%d context = %p, context->src_addr = 0x%016lx, context->ignore = 0x%016lx, context->tag = 0x%016lx, src_addr.fi = 0x%08lx\n",
		__FILE__, __func__, __LINE__, context, context->src_addr, context->ignore, context->tag, src_addr.fi);
	if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		fprintf(stderr,
			"%s:%s():%d hdr->match.slid = 0x%04x (%u), hdr->match.origin_subctxt_rx = 0x%x (%u), origin_lid = 0x%08x, reliability.origin_subctxt_rx = 0x%x\n",
			__FILE__, __func__, __LINE__, __be16_to_cpu24((__be16) hdr->lrh_9B.slid),
			__be16_to_cpu24((__be16) hdr->lrh_9B.slid), hdr->match.origin_subctxt_rx,
			hdr->match.origin_subctxt_rx, slid, hdr->reliability.origin_subctxt_rx);
	} else {
		fprintf(stderr,
			"%s:%s():%d hdr->match.slid = 0x%lx (%u), hdr->match.origin_subctxt_rx = 0x%x (%u), origin_lid = 0x%08x, reliability.origin_subctxt_rx = 0x%x\n",
			__FILE__, __func__, __LINE__,
			__le24_to_cpu((opx_lid_t) ((hdr->lrh_16B.slid20 << 20) | (hdr->lrh_16B.slid))),
			__le24_to_cpu((opx_lid_t) ((hdr->lrh_16B.slid20 << 20) | (hdr->lrh_16B.slid))),
			hdr->match.origin_subctxt_rx, hdr->match.origin_subctxt_rx, slid,
			hdr->reliability.origin_subctxt_rx);
	}
	fprintf(stderr,
		"%s:%s():%d hdr->match.ofi_tag = 0x%016lx, target_tag_and_not_ignore = 0x%016lx, origin_tag_and_not_ignore = 0x%016lx, FI_ADDR_UNSPEC = 0x%08lx\n",
		__FILE__, __func__, __LINE__, hdr->match.ofi_tag, target_tag_and_not_ignore, origin_tag_and_not_ignore,
		FI_ADDR_UNSPEC);
	if (opx_ep->daos_info.hfi_rank_enabled && is_shm) {
		struct fi_opx_daos_av_rank *av_rank = fi_opx_get_daos_av_rank(opx_ep, rank, rank_inst);

		if (av_rank) {
			fprintf(stderr, "%s:%s():%d AV - rank %d, rank_inst %d, fi_addr 0x%08lx\n", __FILE__, __func__,
				__LINE__, av_rank->key.rank, av_rank->key.rank_inst, av_rank->fi_addr);
		} else {
			fprintf(stderr, "%s:%s():%d AV - Not Found.\n", __FILE__, __func__, __LINE__);
			fprintf(stderr, "%s:%s():%d EP - rank %d, rank_inst %d, fi_addr 0x%08lx\n", __FILE__, __func__,
				__LINE__, opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, av_rank->fi_addr);
		}
	}
	fprintf(stderr, "%s:%s():%d answer = %lu\n", __FILE__, __func__, __LINE__, answer);
#endif
	return answer;
}

__OPX_FORCE_INLINE__
uint32_t fi_opx_ep_get_u32_extended_rx(struct fi_opx_ep *opx_ep, const unsigned is_shm, const uint16_t origin_rx)
{
#ifdef OPX_DAOS
	return (is_shm && opx_ep->daos_info.hfi_rank_enabled) ? opx_ep->daos_info.rank : origin_rx;
#else
	return origin_rx;
#endif
}

__OPX_FORCE_INLINE__
void fi_opx_enqueue_completed(struct slist *queue, struct opx_context *context, const int lock_required)
{
	assert(!lock_required);
	assert(context);
	context->flags &= ~FI_OPX_CQ_CONTEXT_HMEM;
	slist_insert_tail((struct slist_entry *) context, queue);
}

__OPX_FORCE_INLINE__
void opx_enqueue_err_from_pending(struct slist *pending, struct slist *err, struct opx_context *context)
{
	struct slist_entry *prev;
	struct slist_entry *cur;

	slist_foreach (pending, cur, prev) {
		if (cur == (struct slist_entry *) context) {
			slist_remove(pending, cur, prev);
			break;
		}
	}

	slist_insert_tail((struct slist_entry *) context, err);
}

__OPX_FORCE_INLINE__
void opx_ep_copy_immediate_data(struct fi_opx_ep *opx_ep, const union fi_opx_hfi1_rzv_rts_immediate_info immediate_info,
				struct opx_payload_rzv_contig *contiguous, const uint64_t immediate_byte_count,
				const uint64_t immediate_qw_count, const uint64_t immediate_block,
				const uint64_t immediate_tail, const uint64_t immediate_total, const size_t xfer_len,
				const uint64_t is_hmem, const enum fi_hmem_iface rbuf_iface, const uint64_t rbuf_device,
				const uint64_t hmem_handle, uint8_t *rbuf_in)
{
	uint8_t *rbuf = is_hmem ? opx_ep->hmem_copy_buf : rbuf_in;

	for (int i = 0; i < immediate_byte_count; ++i) {
		rbuf[i] = contiguous->immediate_byte[i];
	}
	rbuf += immediate_byte_count;

	uint64_t *rbuf_qw = (uint64_t *) rbuf;
	for (int i = 0; i < immediate_qw_count; ++i) {
		rbuf_qw[i] = contiguous->immediate_qw[i];
	}
	rbuf += immediate_qw_count * sizeof(uint64_t);

	if (immediate_block) {
#if (defined __GNUC__) && (__GNUC__ > 10)
#pragma GCC diagnostic ignored "-Wstringop-overread"
#endif
#pragma GCC diagnostic ignored "-Warray-bounds"
		if (immediate_byte_count || immediate_qw_count) { // immediate fragment
			memcpy(rbuf, (void *) (&contiguous->immediate_block), FI_OPX_CACHE_LINE_SIZE);
		} else {
			memcpy(rbuf, (void *) (&contiguous->cache_line_1), FI_OPX_CACHE_LINE_SIZE);
		}
	}

	if (is_hmem && immediate_total) {
		opx_copy_to_hmem(rbuf_iface, rbuf_device, hmem_handle, rbuf_in, opx_ep->hmem_copy_buf, immediate_total,
				 OPX_HMEM_DEV_REG_RECV_THRESHOLD);
	}

	if (immediate_tail) {
		uint8_t *rbuf_start = rbuf_in + xfer_len - OPX_IMMEDIATE_TAIL_BYTE_COUNT;

		if (!is_hmem) {
			for (int i = 0; i < OPX_IMMEDIATE_TAIL_BYTE_COUNT; ++i) {
				rbuf_start[i] = immediate_info.tail_bytes[i];
			}
		} else {
			opx_copy_to_hmem(rbuf_iface, rbuf_device, hmem_handle, rbuf_start, immediate_info.tail_bytes,
					 OPX_IMMEDIATE_TAIL_BYTE_COUNT, OPX_HMEM_DEV_REG_RECV_THRESHOLD);
		}
	}
}

__OPX_FORCE_NOINLINE__
void fi_opx_handle_recv_rts_truncation(struct fi_opx_ep_rx *rx, struct opx_context *context, const uint64_t xfer_len,
				       const uint64_t ofi_data, const uint64_t origin_tag, const uint8_t opcode,
				       const uint64_t recv_len, void *recv_buf)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "RENDEZVOUS truncation - xfer_len %lu > recv_len %lu posting error\n", xfer_len, recv_len);

	/* Post a CTS Truncation error (FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC) to unblock the Tx of RTS */

	context->len	      = xfer_len;
	context->data	      = ofi_data;
	context->tag	      = origin_tag;
	context->next	      = NULL;
	context->byte_counter = 0;
	context->flags =
		FI_RECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode) | FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode);

	/* Post a E_TRUNC to our local RX error queue because a client called receive
	with too small a buffer.  Tell them about it via the error cq */

	context->err_entry.flags	 = context->flags;
	context->err_entry.len		 = recv_len;
	context->err_entry.buf		 = recv_buf;
	context->err_entry.data		 = ofi_data;
	context->err_entry.tag		 = origin_tag;
	context->err_entry.olen		 = xfer_len - recv_len;
	context->err_entry.err		 = FI_ETRUNC;
	context->err_entry.prov_errno	 = 0;
	context->err_entry.err_data	 = NULL;
	context->err_entry.err_data_size = 0;

	context->byte_counter = 0;
	context->next	      = NULL;

	/* post an 'error' completion event */
	slist_insert_tail((struct slist_entry *) context, rx->cq_err_ptr);
}

#if HAVE_HFISVC
static uint64_t opx_trunc_scratch_buf;
#endif

__OPX_FORCE_INLINE__
void fi_opx_handle_recv_rts_hfisvc(const union opx_hfi1_packet_hdr *const	 hdr,
				   const union fi_opx_hfi1_packet_payload *const payload, struct fi_opx_ep *opx_ep,
				   const uint64_t origin_tag, const uint8_t opcode, struct opx_context *context,
				   const uint64_t is_multi_receive, const unsigned is_intranode, const uint64_t is_hmem,
				   const int lock_required, const enum ofi_reliability_kind reliability,
				   const enum opx_hfi1_type hfi1_type)
{
#if HAVE_HFISVC
	assert(hfi1_type & OPX_HFI1_JKR);
	assert(FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_HFISVC);
	assert(!lock_required);
	assert(!is_multi_receive); // Multi recv not supported with HFISVC yet
	assert(!is_hmem);	   // HMEM not supported with HFISVC yet

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV -- RENDEZVOUS RTS HFISVC (%X) (begin) context %p is_multi_recv (%lu)\n",
		opcode, context, is_multi_receive);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-RTS-HFISVC");

	const uint64_t		   ofi_data = hdr->match.ofi_data;
	uint64_t		   recv_len = context->len;
	void			  *recv_buf = context->buf;
	struct fi_opx_ep_rx *const rx	    = opx_ep->rx;

	const hfisvc_client_key_t sbuf_key = (hfisvc_client_key_t) hdr->rzv_rts.sbuf_client_key;
	const uint32_t		  niov	   = hdr->rzv_rts.niov;
	const uint32_t		  lid	   = (uint32_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);

	uint64_t xfer_len = hdr->rzv_rts.message_length;

	OPX_HFISVC_DEBUG_LOG("Matched HFISVC RTS packet, recv context=%p sbuf_client_key=%u xfer_len=%lu niov=%u\n",
			     context, sbuf_key, xfer_len, niov);

	struct hfisvc_client_completion completion = {
		.flags		= HFISVC_CLIENT_COMPLETION_FLAG_CQ,
		.cq.app_context = (uint64_t) context,
		.cq.handle	= *opx_ep->hfisvc.cq_completion_queue,
	};

	if (OFI_LIKELY(xfer_len <= recv_len)) {
		context->byte_counter = xfer_len;
		context->len	      = xfer_len;
		context->data	      = ofi_data;
		context->tag	      = origin_tag;
		context->next	      = NULL;
		context->flags |= FI_RECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode) |
				  FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode);

		for (int i = 0; i < niov; ++i) {
			const uint32_t sbuf_access_key = payload->rendezvous.hfisvc.iovs[i].access_key;
			const uint64_t sbuf_len	       = payload->rendezvous.hfisvc.iovs[i].len;
			const uint64_t sbuf_offset     = payload->rendezvous.hfisvc.iovs[i].offset;
			int	       rc	       = (*opx_ep->domain->hfisvc.cmd_rdma_read_va)(
				opx_ep->hfisvc.command_queue, completion, 0ul /* flags */, lid, sbuf_key, sbuf_len,
				0ul /* immediate data */, sbuf_access_key, sbuf_offset, recv_buf);

			if (rc) {
				int deferred_rc = opx_hfisvc_deferred_recv_rts_enqueue(
					opx_ep, context, niov - i, sbuf_key, lid, recv_buf,
					&payload->rendezvous.hfisvc.iovs[i]);
				if (OFI_UNLIKELY(deferred_rc)) {
					fprintf(stderr,
						"(%d) %s:%s():%d Error: Couldn't create deferred work to handle rendezvous receive (%d), aborting.\n",
						getpid(), __FILE__, __func__, __LINE__, deferred_rc);
					abort();
				}
				OPX_HFISVC_DEBUG_LOG(
					"[%d/%d] rdma_read failed with rc=%d context=%p recv_buf=%p sbuf_key=%u, sbuf_access_key=%u sbuf_len=%lu\n",
					i + 1, niov, rc, context, recv_buf, sbuf_key, sbuf_access_key, sbuf_len);

				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_recv_rts.deferred);

				// rdma_read_count only needs to be incremented by a non-zero value if/when we
				// successfully issued an rdma_read. The reason for incrementing it by 'i' here,
				// but only by 1 down below right before recv_rts_hfisvc_finish is that for the
				// case of i=0, we don't want to actually increment the count. Always incrementing
				// by 'i' here is just more performant/less branchy than doing '+= i ? 1 : 0'
				opx_ep->hfisvc.rdma_read_count += i;
				slist_insert_tail((struct slist_entry *) context, rx->cq_pending_ptr);

				goto recv_rts_hfisvc_finish;
			}
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_recv_rts.rdma_read);
			OPX_HFISVC_DEBUG_LOG(
				"[%d/%d] Successfully issued rdma_read context=%p recv_buf=%p sbuf_key=%u, sbuf_access_key=%u sbuf_len=%lu\n",
				i + 1, niov, context, recv_buf, sbuf_key, sbuf_access_key, sbuf_len);

			recv_buf = (void *) ((uintptr_t) recv_buf + sbuf_len);
		}

		slist_insert_tail((struct slist_entry *) context, rx->cq_pending_ptr);
	} else { /* truncation - unlikely */
		OPX_HFISVC_DEBUG_LOG("Truncation for client_key %u! xfer_len=%lu recv_len=%lu\n", sbuf_key, xfer_len,
				     recv_len);

		/* Iterate through the IOVs, issuing a 1-byte rdma_read of each
		   sbuf info a scratch buffer so that the sender will receive a
		   completion for each and free the access key. If the rdma_read
		   fails, create a deferred work item to keep retrying. */
		for (int i = 0; i < niov; ++i) {
			const uint32_t sbuf_access_key = payload->rendezvous.hfisvc.iovs[i].access_key;
			int	       rc	       = (*opx_ep->domain->hfisvc.cmd_rdma_read_va)(
				opx_ep->hfisvc.command_queue, completion, 0ul /* flags */, lid, sbuf_key,
				1ul /* length */, 0ul /* immediate data */, sbuf_access_key, 0ul /* remote offset */,
				&opx_trunc_scratch_buf);
			if (OFI_UNLIKELY(rc != FI_SUCCESS)) {
				union opx_hfisvc_iov trunc_iov = payload->rendezvous.hfisvc.iovs[i];
				trunc_iov.len		       = 1;

				int deferred_rc = opx_hfisvc_deferred_recv_rts_enqueue(
					opx_ep, context, 1 /* niov */, sbuf_key, lid, &opx_trunc_scratch_buf,
					(const union opx_hfisvc_iov *) &trunc_iov);

				if (OFI_UNLIKELY(deferred_rc)) {
					fprintf(stderr,
						"(%d) %s:%s():%d Error: Couldn't create deferred work to handle truncated rendezvous receive (%d), aborting.\n",
						getpid(), __FILE__, __func__, __LINE__, deferred_rc);
					abort();
				}
			}
		}

		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_recv_rts.truncation_rdma_read);
		fi_opx_handle_recv_rts_truncation(rx, context, xfer_len, ofi_data, origin_tag, opcode, recv_len,
						  recv_buf);
	}

	++opx_ep->hfisvc.rdma_read_count;
recv_rts_hfisvc_finish:
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS-HFISVC");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- RENDEZVOUS RTS HFISVC (end) context %p\n", context);
#endif
}

__OPX_FORCE_INLINE__
void fi_opx_handle_recv_rts(const union opx_hfi1_packet_hdr *const	  hdr,
			    const union fi_opx_hfi1_packet_payload *const payload, struct fi_opx_ep *opx_ep,
			    const uint64_t origin_tag, const uint8_t opcode, struct opx_context *context,
			    const uint64_t is_multi_receive, const unsigned is_shm, const uint64_t is_hmem,
			    const int lock_required, const enum ofi_reliability_kind reliability,
			    const enum opx_hfi1_type hfi1_type)
{
	assert(FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS);
	assert(!lock_required);

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV -- RENDEZVOUS RTS (%X) (begin) context %p is_multi_recv (%lu)\n",
		opcode, context, is_multi_receive);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-RTS");

	const uint64_t		   ofi_data	= hdr->match.ofi_data;
	const uint64_t		   recv_len	= context->len;
	void			  *recv_buf	= context->buf;
	struct fi_opx_ep_rx *const rx		= opx_ep->rx;
	const uint64_t		   xfer_len	= hdr->rendezvous.message_length;
	const uint64_t		   niov		= hdr->rendezvous.niov;
	const uint64_t		   is_noncontig = hdr->rendezvous.flags & FI_OPX_PKT_RZV_FLAGS_NONCONTIG;

#ifdef OPX_HMEM
	const uint64_t is_ipc = hdr->rendezvous.flags & FI_OPX_PKT_RZV_FLAGS_IPC;
#endif
	const uint16_t origin_rx = FI_OPX_HFI1_PACKET_ORIGIN_RX(hdr);

	if (is_multi_receive) { /* compile-time constant expression */
		assert(FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode) == FI_MSG);
		const uint32_t	    u32_ext_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, origin_rx);
		struct opx_context *original_multi_recv_context = context;
		context = (struct opx_context *) ((uintptr_t) recv_buf - sizeof(struct opx_context));

		assert((((uintptr_t) context) & 0x07) == 0);
		context->flags =
			FI_RECV | FI_MSG | FI_OPX_CQ_CONTEXT_MULTIRECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode);
		context->buf		    = recv_buf;
		context->len		    = xfer_len;
		context->data		    = ofi_data;
		context->tag		    = 0; /* tag is not valid for multi-receives */
		context->multi_recv_context = original_multi_recv_context;
		context->byte_counter	    = xfer_len;
		context->next		    = NULL;
		uint8_t *rbuf		    = (uint8_t *) recv_buf;

		if (OFI_LIKELY(is_noncontig)) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.recv.multi_recv_rzv_noncontig);
			FI_OPX_FABRIC_RX_RZV_RTS(opx_ep, hdr, payload, origin_rx, niov,
						 payload->rendezvous.noncontiguous.origin_byte_counter_vaddr, context,
						 (uintptr_t) (rbuf), /* receive buffer virtual address */
						 FI_HMEM_SYSTEM,     /* receive buffer iface */
						 0UL,		     /* receive buffer device */
						 0UL,		     /* immediate_data */
						 0UL,		     /* immediate_end_block_count */
						 &payload->rendezvous.noncontiguous.iov[0],
						 FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG, is_shm,
						 reliability, /* compile-time constant expression */
						 u32_ext_rx, hfi1_type);
		} else {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.recv.multi_recv_rzv_contig);
			assert(niov == 1);
			struct opx_payload_rzv_contig *contiguous =
				(hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) ?
					(struct opx_payload_rzv_contig *) &payload->rendezvous.contiguous :
					(struct opx_payload_rzv_contig *) &payload->rendezvous.contiguous_16B;
			const union fi_opx_hfi1_rzv_rts_immediate_info immediate_info = {
				.qw0 = contiguous->immediate_info};
			const uint64_t immediate_byte_count = (immediate_info.count & OPX_IMMEDIATE_BYTE_COUNT_MASK) >>
							      OPX_IMMEDIATE_BYTE_COUNT_SHIFT;
			const uint64_t immediate_qw_count =
				(immediate_info.count & OPX_IMMEDIATE_QW_COUNT_MASK) >> OPX_IMMEDIATE_QW_COUNT_SHIFT;
			const uint64_t immediate_block =
				(immediate_info.count & OPX_IMMEDIATE_BLOCK_MASK) >> OPX_IMMEDIATE_BLOCK_SHIFT;
			const uint64_t immediate_tail =
				(immediate_info.count & OPX_IMMEDIATE_TAIL_MASK) >> OPX_IMMEDIATE_TAIL_SHIFT;
			const uint64_t immediate_total = immediate_byte_count + immediate_qw_count * sizeof(uint64_t) +
							 immediate_block * sizeof(union cacheline);

			const struct fi_opx_hmem_iov src_dst_iov[1] = {
				{.buf	 = contiguous->src_vaddr,
				 .len	 = contiguous->src_len,
				 .device = contiguous->src_device_id,
				 .iface	 = (enum fi_hmem_iface) contiguous->src_iface}};

			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"IMMEDIATE  RZV_RTS immediate_total %#lX, immediate_byte_count %#lX, immediate_qw_count %#lX, immediate_block_count %#lX\n",
				immediate_total, immediate_byte_count, immediate_qw_count, immediate_block);

			context->byte_counter -= immediate_total;

			FI_OPX_FABRIC_RX_RZV_RTS(
				opx_ep, hdr, payload, origin_rx, niov, contiguous->origin_byte_counter_vaddr, context,
				(uintptr_t) (rbuf + immediate_total), /* receive buffer virtual address */
				FI_HMEM_SYSTEM,			      /* receive buffer iface */
				0UL,				      /* receive buffer device */
				immediate_total, immediate_tail, src_dst_iov, FI_OPX_HFI_DPUT_OPCODE_RZV, is_shm,
				reliability, /* compile-time constant expression */
				u32_ext_rx, hfi1_type);

			opx_ep_copy_immediate_data(opx_ep, immediate_info, contiguous, immediate_byte_count,
						   immediate_qw_count, immediate_block, immediate_tail, immediate_total,
						   xfer_len, OPX_HMEM_FALSE, FI_HMEM_SYSTEM, 0ul, OPX_HMEM_NO_HANDLE,
						   rbuf);
		}

		uint64_t bytes_consumed = ((xfer_len + 8) & (~0x07ull)) + sizeof(struct opx_context);
		original_multi_recv_context->len -= bytes_consumed;
		original_multi_recv_context->byte_counter++;	       // re-using the byte counter as a "pending flag"
		original_multi_recv_context->tag = (uintptr_t) opx_ep; // re-using tag to store the ep
		original_multi_recv_context->buf =
			(void *) ((uintptr_t) (original_multi_recv_context->buf) + bytes_consumed);
		assert(context->next == NULL);
		if (lock_required) {
			fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
			abort();
		}
		slist_insert_tail((struct slist_entry *) context, rx->cq_pending_ptr);

	} else if (OFI_LIKELY(xfer_len <= recv_len)) {
		context->len  = xfer_len;
		context->data = ofi_data;
		context->tag  = origin_tag;
		context->next = NULL;
		context->flags |= FI_RECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode) |
				  FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode);

		const uint32_t u32_ext_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, origin_rx);

		if (OFI_LIKELY(niov == 1)) {
			assert(!is_noncontig);

#ifdef OPX_HMEM
			if (is_ipc) {
				opx_hfi1_rx_ipc_rts(opx_ep, hdr, payload, origin_rx, niov, is_hmem, context, xfer_len,
						    u32_ext_rx, hfi1_type);
				OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS");
				FI_DBG_TRACE(
					fi_opx_global.prov, FI_LOG_EP_DATA,
					"===================================== RECV -- RENDEZVOUS RTS (end) context %p\n",
					context);
				return;
			}
#endif

			uint64_t	   rbuf_device;
			enum fi_hmem_iface rbuf_iface;
			uint64_t	   hmem_handle;
			if (is_hmem) { /* Branch should compile out */
				struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) context->hmem_info_qws;
				rbuf_device			   = hmem_info->device;
				rbuf_iface			   = hmem_info->iface;
				hmem_handle			   = hmem_info->hmem_dev_reg_handle;
				FI_OPX_DEBUG_COUNTERS_INC_COND(
					is_shm,
					opx_ep->debug_counters.hmem.intranode
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.rzv);
				FI_OPX_DEBUG_COUNTERS_INC_COND(
					!is_shm,
					opx_ep->debug_counters.hmem.hfi
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.rzv);
			} else {
				rbuf_device = 0;
				hmem_handle = 0;
				rbuf_iface  = FI_HMEM_SYSTEM;
			}
			uint8_t *rbuf = (uint8_t *) recv_buf;

			struct opx_payload_rzv_contig *contiguous =
				(hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) ?
					(struct opx_payload_rzv_contig *) &payload->rendezvous.contiguous :
					(struct opx_payload_rzv_contig *) &payload->rendezvous.contiguous_16B;
			const union fi_opx_hfi1_rzv_rts_immediate_info immediate_info = {
				.qw0 = contiguous->immediate_info};
			const uint64_t immediate_byte_count = (immediate_info.count & OPX_IMMEDIATE_BYTE_COUNT_MASK) >>
							      OPX_IMMEDIATE_BYTE_COUNT_SHIFT;
			const uint64_t immediate_qw_count =
				(immediate_info.count & OPX_IMMEDIATE_QW_COUNT_MASK) >> OPX_IMMEDIATE_QW_COUNT_SHIFT;
			const uint64_t immediate_block =
				(immediate_info.count & OPX_IMMEDIATE_BLOCK_MASK) >> OPX_IMMEDIATE_BLOCK_SHIFT;
			const uint64_t immediate_tail =
				(immediate_info.count & OPX_IMMEDIATE_TAIL_MASK) >> OPX_IMMEDIATE_TAIL_SHIFT;
			const uint64_t immediate_total = immediate_byte_count + immediate_qw_count * sizeof(uint64_t) +
							 immediate_block * sizeof(union cacheline);

			const struct fi_opx_hmem_iov src_dst_iov[1] = {
				{.buf	 = contiguous->src_vaddr,
				 .len	 = contiguous->src_len,
				 .device = contiguous->src_device_id,
				 .iface	 = (enum fi_hmem_iface) contiguous->src_iface}};

			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"IMMEDIATE  RZV_RTS immediate_total %#lX, immediate_byte_count %#lX, immediate_qw_count %#lX, immediate_block_count %#lX\n",
				immediate_total, immediate_byte_count, immediate_qw_count, immediate_block);
			context->byte_counter = xfer_len - immediate_total;

			FI_OPX_FABRIC_RX_RZV_RTS(
				opx_ep, hdr, payload, origin_rx, 1, contiguous->origin_byte_counter_vaddr, context,
				(uintptr_t) (rbuf + immediate_total), rbuf_iface, rbuf_device, immediate_total,
				immediate_tail, src_dst_iov, FI_OPX_HFI_DPUT_OPCODE_RZV, is_shm,
				reliability, /* compile-time constant expression */
				u32_ext_rx, hfi1_type);

			opx_ep_copy_immediate_data(opx_ep, immediate_info, contiguous, immediate_byte_count,
						   immediate_qw_count, immediate_block, immediate_tail, immediate_total,
						   xfer_len, is_hmem, rbuf_iface, rbuf_device, hmem_handle, rbuf);
		} else {
			/*fi_opx_hfi1_dump_packet_hdr(hdr, __func__, __LINE__); */
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"rendezvous non-contiguous source data not implemented; abort\n");
			abort();
		}

		/* post a pending completion event for the individual receive */
		if (lock_required) {
			fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
			abort();
		}
		slist_insert_tail((struct slist_entry *) context, rx->cq_pending_ptr);

	} else { /* truncation - unlikely */

		fi_opx_handle_recv_rts_truncation(rx, context, xfer_len, ofi_data, origin_tag, opcode, recv_len,
						  recv_buf);

		// clear the send-side rendezvous state
		const uint32_t u32_ext_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, origin_rx);

		assert(payload != NULL);

		uintptr_t origin_byte_counter_vaddr =
			is_noncontig ? payload->rendezvous.noncontiguous.origin_byte_counter_vaddr :
			(hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) ?
				       payload->rendezvous.contiguous.origin_byte_counter_vaddr :
				       payload->rendezvous.contiguous_16B.origin_byte_counter_vaddr;
		FI_OPX_FABRIC_RX_RZV_RTS_ETRUNC(opx_ep, (const void *const) hdr, origin_rx, origin_byte_counter_vaddr,
						is_shm, reliability, /* compile-time constant expression */
						u32_ext_rx, hfi1_type);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- RENDEZVOUS RTS (end) context %p\n", context);
}

/**
 * \brief Complete a receive operation that has matched the packet header with
 * 		the match information
 *
 * \param[in]		rx	Receive endoint
 * \param[in]		hdr	MU packet header that matched
 * \param[in,out]	entry	Completion entry
 */
__OPX_FORCE_INLINE__
void opx_ep_complete_receive_operation(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
				       const union fi_opx_hfi1_packet_payload *const payload, const uint64_t origin_tag,
				       struct opx_context *context, const uint8_t opcode,
				       const uint64_t is_multi_receive, const unsigned is_shm, const uint64_t is_hmem,
				       const int lock_required, const enum ofi_reliability_kind reliability,
				       const enum opx_hfi1_type hfi1_type)
{
	assert((is_multi_receive && !is_hmem) || !is_multi_receive);

	struct fi_opx_ep	  *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	struct fi_opx_ep_rx *const rx	  = opx_ep->rx;

	const uint64_t recv_len = context->len;
	/*
	 * The context buffer pointer has already been set to the appropriate
	 * value (NULL or receive data buffer) to be returned to the user
	 * application.  The value is based upon the type of receive operation
	 * done by the user application.
	 */
	void *recv_buf = context->buf;

	if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_MSG_INJECT) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- INJECT (begin)\n");
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-INJECT");

		const uint64_t ofi_data = hdr->match.ofi_data;
		const uint64_t send_len = OPX_BTH_INJECT_MSG_LENGTH(hdr->inject.subctxt_message_length);

		if (is_multi_receive) { /* branch should compile out */

			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.recv.multi_recv_inject);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "INJECT is_multi_recv\n");

			if (send_len) {
				memcpy(recv_buf, (void *) &hdr->inject.app_data_u8[0], send_len);
			}

			struct opx_context *original_multi_recv_context = context;
			context = (struct opx_context *) ((uintptr_t) recv_buf - sizeof(struct opx_context));
			assert((((uintptr_t) context) & 0x07) == 0);

			context->flags = FI_RECV | FI_MSG | FI_OPX_CQ_CONTEXT_MULTIRECV |
					 FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode);
			context->buf		    = recv_buf;
			context->len		    = send_len;
			context->data		    = ofi_data;
			context->tag		    = 0; /* tag is not valid for multi-receives */
			context->multi_recv_context = original_multi_recv_context;
			context->byte_counter	    = 0;
			context->next		    = NULL;

			/* the next 'fi_opx_context' must be 8-byte aligned */
			uint64_t bytes_consumed = ((send_len + 8) & (~0x07ull)) + sizeof(struct opx_context);
			original_multi_recv_context->len -= bytes_consumed;
			original_multi_recv_context->buf =
				(void *) ((uintptr_t) (original_multi_recv_context->buf) + bytes_consumed);

			/* post a completion event for the individual receive */
			if (lock_required) {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
				abort();
			}
			slist_insert_tail((struct slist_entry *) context, rx->cq_completed_ptr);

		} else if (OFI_LIKELY(send_len <= recv_len)) {
			if (is_hmem && send_len) {
				struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) context->hmem_info_qws;
				opx_copy_to_hmem(hmem_info->iface, hmem_info->device, hmem_info->hmem_dev_reg_handle,
						 recv_buf, hdr->inject.app_data_u8, send_len,
						 OPX_HMEM_DEV_REG_RECV_THRESHOLD);
				FI_OPX_DEBUG_COUNTERS_INC_COND(
					is_shm,
					opx_ep->debug_counters.hmem.intranode
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.inject);
				FI_OPX_DEBUG_COUNTERS_INC_COND(
					!is_shm,
					opx_ep->debug_counters.hmem.hfi
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.inject);
			} else {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"
				switch (send_len) {
				case 0:
					break;
				case 1:
					*((uint8_t *) recv_buf) = hdr->inject.app_data_u8[0];
					break;
				case 2:
					*((uint16_t *) recv_buf) = hdr->inject.app_data_u16[0];
					break;
				case 3:
					memcpy(recv_buf, (void *) &hdr->inject.app_data_u8[0], send_len);
					break;
				case 4:
					*((uint32_t *) recv_buf) = hdr->inject.app_data_u32[0];
					break;
				case 5:
				case 6:
				case 7:
					memcpy(recv_buf, (void *) &hdr->inject.app_data_u8[0], send_len);
					break;
				case 8:
					*((uint64_t *) recv_buf) = hdr->inject.app_data_u64[0];
					break;
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
					memcpy(recv_buf, (void *) &hdr->inject.app_data_u8[0], send_len);
					break;
				case 16:
					((uint64_t *) recv_buf)[0] = hdr->inject.app_data_u64[0];
					((uint64_t *) recv_buf)[1] = hdr->inject.app_data_u64[1];
					break;
				default:
					FI_WARN(fi_opx_global.prov, FI_LOG_EP_CTRL,
						"Invalid send length for inject: %lu\n", send_len);
					abort();
					break;
				}
#pragma GCC diagnostic pop
			}

			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"INJECT send_len %lu <= recv_len %lu; enqueue cq (completed) ofi_data = %ld tag = %ld\n",
				send_len, recv_len, ofi_data, origin_tag);

			context->flags |= FI_RECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode) |
					  FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode);
			context->len	      = send_len;
			context->data	      = ofi_data;
			context->tag	      = origin_tag;
			context->byte_counter = 0;
			context->next	      = NULL;

			/* post a completion event for the individual receive */
			fi_opx_enqueue_completed(rx->cq_completed_ptr, context, lock_required);

		} else { /* truncation - unlikely */

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "INJECT truncation - send_len %lu > recv_len %lu posting error\n", send_len,
				     recv_len);

			context->err_entry.flags	 = context->flags;
			context->err_entry.len		 = recv_len;
			context->err_entry.buf		 = recv_buf;
			context->err_entry.data		 = ofi_data;
			context->err_entry.tag		 = origin_tag;
			context->err_entry.olen		 = send_len - recv_len;
			context->err_entry.err		 = FI_ETRUNC;
			context->err_entry.prov_errno	 = 0;
			context->err_entry.err_data	 = NULL;
			context->err_entry.err_data_size = 0;

			context->byte_counter = 0;
			context->next	      = NULL;

			/* post an 'error' completion event for the receive */
			if (lock_required) {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
				abort();
			}
			slist_insert_tail((struct slist_entry *) context, rx->cq_err_ptr);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-INJECT");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- INJECT (end)\n");

	} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- EAGER (begin)\n");
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-EAGER");

		const uint64_t ofi_data	       = hdr->match.ofi_data;
		uint8_t	       xfer_bytes_tail = OPX_BTH_SEND_XFER_BYTES_TAIL(hdr->send.subctxt_xfer_bytes_tail);
		const uint64_t send_len	       = xfer_bytes_tail + hdr->send.payload_qws_total * sizeof(uint64_t);

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "hdr->send.subctxt_xfer_bytes_tail = %u, hdr->send.payload_qws_total = %u, send_len = %lu\n",
		       xfer_bytes_tail, hdr->send.payload_qws_total, send_len);

		if (is_multi_receive) { /* branch should compile out */

			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.recv.multi_recv_eager);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "EAGER is_multi_recv\n");

			struct opx_context *original_multi_recv_context = context;

			context = (struct opx_context *) ((uintptr_t) recv_buf - sizeof(struct opx_context));
			assert((((uintptr_t) context) & 0x07) == 0);
			context->flags = FI_RECV | FI_MSG | FI_OPX_CQ_CONTEXT_MULTIRECV |
					 FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode);
			context->buf		    = recv_buf;
			context->len		    = send_len;
			context->data		    = ofi_data;
			context->tag		    = 0; /* tag is not valid for multi-receives */
			context->multi_recv_context = original_multi_recv_context;
			context->byte_counter	    = 0;
			context->next		    = NULL;

			if (xfer_bytes_tail) {
				memcpy(recv_buf, (void *) &hdr->send.xfer_tail, xfer_bytes_tail);
				recv_buf = (void *) ((uintptr_t) recv_buf + xfer_bytes_tail);
			}

			if (payload) {
				uint64_t *recv_buf_qw = (uint64_t *) recv_buf;
				uint64_t *payload_qw  = (uint64_t *) payload;
				unsigned  i;
				for (i = 0; i < hdr->send.payload_qws_total; ++i) {
					recv_buf_qw[i] = payload_qw[i];
				}
			}

			/* the next 'fi_opx_context' must be 8-byte aligned */
			uint64_t bytes_consumed = ((send_len + 8) & (~0x07ull)) + sizeof(struct opx_context);
			original_multi_recv_context->len -= bytes_consumed;
			original_multi_recv_context->buf =
				(void *) ((uintptr_t) (original_multi_recv_context->buf) + bytes_consumed);

			assert(context->next == NULL);
			/* post a completion event for the individual receive */
			if (lock_required) {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
				abort();
			}
			slist_insert_tail((struct slist_entry *) context, rx->cq_completed_ptr);

		} else if (OFI_LIKELY(send_len <= recv_len)) {
			if (is_hmem) {
				recv_buf = (void *) opx_ep->hmem_copy_buf;
			}

			if (xfer_bytes_tail) {
#pragma GCC diagnostic ignored "-Wnonnull"
				memcpy(recv_buf, (void *) &hdr->send.xfer_tail, xfer_bytes_tail);
				recv_buf = (void *) ((uint8_t *) recv_buf + xfer_bytes_tail);
			}

			if (send_len != xfer_bytes_tail) {
				uint64_t      *recv_buf_qw	 = (uint64_t *) recv_buf;
				uint64_t      *payload_qw	 = (uint64_t *) payload;
				const unsigned payload_qws_total = hdr->send.payload_qws_total;
				unsigned       i;
				for (i = 0; i < payload_qws_total; ++i) {
					recv_buf_qw[i] = payload_qw[i];
				}
			}

			if (is_hmem) {
				struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) context->hmem_info_qws;
				opx_copy_to_hmem(hmem_info->iface, hmem_info->device, hmem_info->hmem_dev_reg_handle,
						 context->buf, opx_ep->hmem_copy_buf, send_len,
						 OPX_HMEM_DEV_REG_RECV_THRESHOLD);
				FI_OPX_DEBUG_COUNTERS_INC_COND(
					is_shm,
					opx_ep->debug_counters.hmem.intranode
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.eager);
				FI_OPX_DEBUG_COUNTERS_INC_COND(
					!is_shm,
					opx_ep->debug_counters.hmem.hfi
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.eager);
			}

			/* fi_opx_hfi1_dump_packet_hdr(hdr, __func__, __LINE__); */

			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"EAGER send_len %lu <= recv_len %lu; enqueue cq (completed), tag %#lX/%#lX, ofi_data %#lX \n",
				send_len, recv_len, context->tag, origin_tag, ofi_data);

			context->flags |= FI_RECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode) |
					  FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode);
			context->len	      = send_len;
			context->data	      = ofi_data;
			context->tag	      = origin_tag;
			context->byte_counter = 0;
			context->next	      = NULL;

			/* post a completion event for the individual receive */
			fi_opx_enqueue_completed(rx->cq_completed_ptr, context, lock_required);

		} else { /* truncation - unlikely */

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "EAGER truncation - send_len %lu > recv_len %lu posting error\n", send_len,
				     recv_len);

			context->err_entry.flags	 = context->flags;
			context->err_entry.len		 = recv_len;
			context->err_entry.buf		 = recv_buf;
			context->err_entry.data		 = ofi_data;
			context->err_entry.tag		 = origin_tag;
			context->err_entry.olen		 = send_len - recv_len;
			context->err_entry.err		 = FI_ETRUNC;
			context->err_entry.prov_errno	 = 0;
			context->err_entry.err_data	 = NULL;
			context->err_entry.err_data_size = 0;

			context->byte_counter = 0;
			context->next	      = NULL;

			/* post an 'error' completion event for the receive */
			if (lock_required) {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
				abort();
			}
			slist_insert_tail((struct slist_entry *) context, rx->cq_err_ptr);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-EAGER");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- EAGER (end)\n");

	} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- MULTI PACKET EAGER FIRST (begin)\n");
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-MP-EAGER-FIRST");

		const uint64_t ofi_data = hdr->match.ofi_data;

		uint64_t payload_qws_total;
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			payload_qws_total = (((uint64_t) __be16_to_cpu(hdr->lrh_9B.pktlen)) - 15) >> 1;
		} else {
			payload_qws_total = (uint64_t) (hdr->lrh_16B.pktlen - 9);
		}
		const uint64_t packet_payload_len = (payload_qws_total << 3);
		const uint64_t payload_total_len  = hdr->mp_eager_first.payload_bytes_total;

		assert(packet_payload_len < payload_total_len);

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "hdr->mp_eager_first.payload_bytes_total = %u send_len = %lu, xfer_len = %lu\n",
		       hdr->mp_eager_first.payload_bytes_total, packet_payload_len, payload_total_len);

		if (OFI_UNLIKELY(is_multi_receive)) { /* branch should compile out */
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Multi-receive used with Multi-packet Eager not implemented. abort.");
			abort();
		} else if (OFI_LIKELY(payload_total_len <= recv_len)) {
#ifndef NDEBUG
			/* For non-optimized builds, fill in the entire buffer area we expect to use.
			   Then as we process multi-packet eager Nth packets, we can check that the
			   buffer area we're writing to contains the filled value. */
			if (!is_hmem) {
				memset(recv_buf, 0xAA, payload_total_len);
			}
#endif
			uint64_t *recv_buf_qw = is_hmem ? (uint64_t *) opx_ep->hmem_copy_buf : (uint64_t *) recv_buf;

			uint64_t *payload_qw = (uint64_t *) payload;

			for (unsigned i = 0; i < payload_qws_total; ++i) {
				recv_buf_qw[i] = payload_qw[i];
			}

			context->flags |= FI_RECV | FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode) |
					  FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode);
			context->len	      = payload_total_len;
			context->data	      = ofi_data;
			context->tag	      = origin_tag;
			context->byte_counter = payload_total_len - packet_payload_len;
			context->next	      = NULL;

			if (is_hmem) {
				struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) context->hmem_info_qws;
				opx_copy_to_hmem(hmem_info->iface, hmem_info->device, hmem_info->hmem_dev_reg_handle,
						 recv_buf, opx_ep->hmem_copy_buf, packet_payload_len,
						 OPX_HMEM_DEV_REG_RECV_THRESHOLD);

				/* MP Eager sends are never intranode */
				FI_OPX_DEBUG_COUNTERS_INC(
					opx_ep->debug_counters.hmem.hfi
						.kind[FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode) ? FI_OPX_KIND_TAG :
												FI_OPX_KIND_MSG]
						.recv.mp_eager);
			}
		} else { /* truncation - unlikely */

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "EAGER truncation - xfer_len %lu > recv_len %lu posting error\n",
				     payload_total_len, recv_len);

			context->err_entry.flags	 = context->flags;
			context->err_entry.len		 = recv_len;
			context->err_entry.buf		 = recv_buf;
			context->err_entry.data		 = ofi_data;
			context->err_entry.tag		 = origin_tag;
			context->err_entry.olen		 = payload_total_len - recv_len;
			context->err_entry.err		 = FI_ETRUNC;
			context->err_entry.prov_errno	 = 0;
			context->err_entry.err_data	 = NULL;
			context->err_entry.err_data_size = 0;

			context->byte_counter = payload_total_len - packet_payload_len;
			context->next	      = NULL;
		}
#ifndef NDEBUG
		if (context->byte_counter == 0) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== RECV -- MULTI PACKET EAGER FIRST UNEXPECTED COMPLETE\n");
		}
#endif

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-MP-EAGER-FIRST");
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== RECV -- MULTI PACKET EAGER FIRST byte counter %lu (end)\n",
			context->byte_counter);

	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- MULTI PACKET EAGER NTH (begin)\n");
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-MP-EAGER-NTH");

		uint64_t payload_qws_total;
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			payload_qws_total = (((uint64_t) __be16_to_cpu(hdr->lrh_9B.pktlen)) - 15) >> 1;
		} else {
			payload_qws_total = (uint64_t) hdr->lrh_16B.pktlen - 9;
		}
		const uint64_t send_len = OPX_BTH_SEND_XFER_BYTES_TAIL(hdr->mp_eager_nth.subctxt_xfer_bytes_tail) +
					  (payload_qws_total << 3);
		const uint64_t xfer_len = send_len + hdr->mp_eager_nth.payload_offset;

		assert(xfer_len <= context->len);

		/* If we flagged this context w/ an error, just decrement the byte counter that this
		 * nth packet would have filled in */
		if (OFI_UNLIKELY(context->err_entry.err == FI_ETRUNC)) {
			context->byte_counter -= send_len;
			return;
		}
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "hdr->mp_eager_nth.subctxt_xfer_bytes_tail = %u, hdr->mp_eager_nth.payload_offset = %u, send_len = %lu, xfer_len = %lu\n",
		       OPX_BTH_SEND_XFER_BYTES_TAIL(hdr->mp_eager_nth.subctxt_xfer_bytes_tail),
		       hdr->mp_eager_nth.payload_offset, send_len, xfer_len);

		if (OFI_UNLIKELY(is_multi_receive)) { /* branch should compile out */
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Multi-receive used with Multi-packet Eager not implemented. abort.");
			abort();
		} else if (OFI_LIKELY(xfer_len <= recv_len)) {
#ifndef NDEBUG
			uint8_t *orig_recv_buf_end = ((uint8_t *) recv_buf) + context->len;
			if (!is_hmem) {
				uint8_t *payload_end = ((uint8_t *) recv_buf) + send_len;
				for (uint8_t *buf_check = (uint8_t *) recv_buf + hdr->mp_eager_nth.payload_offset;
				     buf_check < payload_end; ++buf_check) {
					if (*buf_check != 0xAA) {
						fprintf(stderr,
							"(%d) %s:%s:%d Multi-Packet Eager Nth packet encountered "
							"corrupted destination buffer! Initial PSN = %u, recv_buf=%p, "
							"offset=%0X, payload_length=%ld, buffer altered at %p, "
							"current value is %0hhX\n",
							getpid(), __FILE__, __func__, __LINE__,
							hdr->mp_eager_nth.mp_egr_uid, recv_buf,
							hdr->mp_eager_nth.payload_offset, send_len, buf_check,
							*buf_check);
						abort();
					}
				}
			}
#endif

			const size_t xfer_bytes_tail =
				OPX_BTH_SEND_XFER_BYTES_TAIL(hdr->mp_eager_nth.subctxt_xfer_bytes_tail);
			recv_buf = is_hmem ? (void *) opx_ep->hmem_copy_buf :
					     (void *) ((uint8_t *) recv_buf + hdr->mp_eager_nth.payload_offset);

			assert(is_hmem || ((uint8_t *) recv_buf) <= orig_recv_buf_end);

			uint64_t *recv_buf_qw = (uint64_t *) recv_buf;
			if (OFI_LIKELY(send_len > xfer_bytes_tail)) {
				uint64_t *payload_qw = (uint64_t *) payload;

				unsigned i;
				for (i = 0; i < payload_qws_total; ++i) {
					recv_buf_qw[i] = payload_qw[i];
					assert(is_hmem || ((uint8_t *) &recv_buf_qw[i]) <= orig_recv_buf_end);
				}
			}

			if (xfer_bytes_tail) {
				uint8_t *recv_tail    = (uint8_t *) &recv_buf_qw[payload_qws_total];
				uint8_t *payload_tail = (uint8_t *) &hdr->mp_eager_nth.xfer_tail;
				for (int i = 0; i < xfer_bytes_tail; ++i) {
					recv_tail[i] = payload_tail[i];
				}
			}

			if (is_hmem) {
				recv_buf = (void *) ((uint8_t *) context->buf + hdr->mp_eager_nth.payload_offset);
				struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) context->hmem_info_qws;
				opx_copy_to_hmem(hmem_info->iface, hmem_info->device, hmem_info->hmem_dev_reg_handle,
						 recv_buf, opx_ep->hmem_copy_buf, send_len,
						 OPX_HMEM_DEV_REG_RECV_THRESHOLD);
			}
			/* fi_opx_hfi1_dump_packet_hdr(hdr, __func__, __LINE__);*/

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "Multi-packet EAGER (nth) send_len %lu <= recv_len %lu; enqueue cq (pending)\n",
				     send_len, recv_len);

			assert(context->byte_counter >= send_len);
			context->byte_counter -= send_len;
#ifndef NDEBUG
			if (context->byte_counter == 0) {
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "===================================== RECV -- MULTI PACKET EAGER NTH COMPLETE\n");
			}
#endif
		} else { /* truncation - unlikely */
			/* We verified the context had enough buffer space for the entire multi-packet payload
			 * when we processed the first multi-egr packet. So if xver_len > recv_len, then something
			 * went wrong somewhere. Either the offset/payload size for this packet is incorrect, or
			 * something messed up the context. */
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Multi-packet Eager nth truncation error. Abort.");
			abort();
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-MP-EAGER-NTH");
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== RECV -- MULTI PACKET EAGER NTH byte counter %lu (end)\n",
			context->byte_counter);

	} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_HFISVC_RZV_RTS) {
		fi_opx_handle_recv_rts_hfisvc(hdr, payload, opx_ep, origin_tag, opcode, context, is_multi_receive,
					      is_shm, is_hmem, lock_required, reliability, hfi1_type);

	} else {
		fi_opx_handle_recv_rts(hdr, payload, opx_ep, origin_tag, opcode, context, is_multi_receive, is_shm,
				       is_hmem, lock_required, reliability, hfi1_type);
	}
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_shm_dynamic_tx_connect(const unsigned is_shm, struct fi_opx_ep *opx_ep, const uint16_t rx_id,
				      const uint8_t hfi1_unit)
{
	if (!is_shm) {
		return FI_SUCCESS;
	}

	assert(hfi1_unit < OPX_MAX_HFIS);

#ifdef OPX_DAOS
	uint32_t segment_index;

	if (!opx_ep->daos_info.hfi_rank_enabled) {
		assert(rx_id < 256);
		segment_index = OPX_SHM_SEGMENT_INDEX(hfi1_unit, rx_id);
	} else {
		segment_index = rx_id & OPX_SHM_MAX_CONN_MASK;
	}
#else
	uint32_t segment_index = OPX_SHM_SEGMENT_INDEX(hfi1_unit, rx_id);
#endif

	struct opx_shm_info *shm_info = opx_shm_rbt_get_shm_info(&(opx_ep->tx->shm), segment_index);

	if (OFI_LIKELY((shm_info != NULL) && (shm_info->fifo_segment != NULL))) {
		/* Connection already established */
		return FI_SUCCESS;
	}

	/* Setup new connection */
	char buffer[OPX_JOB_KEY_STR_SIZE + 32];
	int  inst = 0;

#ifdef OPX_DAOS
	if (opx_ep->daos_info.hfi_rank_enabled) {
		inst = opx_ep->daos_info.rank_inst;
	}
#endif

	snprintf(buffer, sizeof(buffer), OPX_SHM_FILE_NAME_PREFIX_FORMAT, opx_ep->domain->unique_job_key_str, hfi1_unit,
		 inst);

	return opx_shm_tx_connect(&opx_ep->tx->shm, (const char *const) buffer, segment_index, rx_id,
				  FI_OPX_SHM_FIFO_SIZE, FI_OPX_SHM_PACKET_SIZE);
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_header_rzv_cts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
					 const union fi_opx_hfi1_packet_payload *const payload, const unsigned is_shm,
					 const int lock_required, const enum ofi_reliability_kind reliability,
					 const enum opx_hfi1_type hfi1_type)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- %s RENDEZVOUS CTS (begin), opcode=%d\n",
		     is_shm ? "SHM" : "HFI", hdr->cts.target.opcode);

	uint16_t origin_rx = FI_OPX_HFI1_PACKET_ORIGIN_RX(hdr);

	assert(payload != NULL || hdr->cts.target.opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC);
	const uint32_t u32_ext_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, origin_rx);

	switch (hdr->cts.target.opcode) {
	case FI_OPX_HFI_DPUT_OPCODE_RZV:
	case FI_OPX_HFI_DPUT_OPCODE_RZV_TID: {
		const union opx_hfi1_dput_iov *const dput_iov		  = payload->cts.iov;
		const uintptr_t			     target_context_vaddr = hdr->cts.target.vaddr.target_context_vaddr;
		const uint32_t			     niov		  = hdr->cts.target.vaddr.niov;
		uint64_t *origin_byte_counter = (uint64_t *) hdr->cts.target.vaddr.origin_byte_counter_vaddr;
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-CTS-HFI:%p", (void *) target_context_vaddr);
		FI_OPX_FABRIC_RX_RZV_CTS(opx_ep, hdr, (const void *const) payload, 0, origin_rx, niov, dput_iov, NULL,
					 (const uint8_t)(FI_NOOP - 1), (const uint8_t)(FI_VOID - 1),
					 (uintptr_t) NULL, /* No RMA Request */
					 target_context_vaddr, origin_byte_counter, hdr->cts.target.opcode, NULL,
					 is_shm,      /* compile-time constant expression */
					 reliability, /* compile-time constant expression */
					 u32_ext_rx, hfi1_type);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-CTS-HFI:%p", (void *) target_context_vaddr);
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG: {
		const union opx_hfi1_dput_iov *const dput_iov		  = payload->cts.iov;
		const uintptr_t			     target_context_vaddr = hdr->cts.target.vaddr.target_context_vaddr;
		const uint32_t			     niov		  = hdr->cts.target.vaddr.niov;
		uint64_t *origin_byte_counter = (uint64_t *) hdr->cts.target.vaddr.origin_byte_counter_vaddr;
		FI_OPX_FABRIC_RX_RZV_CTS(opx_ep, hdr, (const void *const) payload, 0, origin_rx, niov, dput_iov, NULL,
					 (const uint8_t)(FI_NOOP - 1), (const uint8_t)(FI_VOID - 1),
					 (uintptr_t) NULL, /* No RMA Request */
					 target_context_vaddr, origin_byte_counter,
					 FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG, NULL,
					 is_shm,      /* compile-time constant expression */
					 reliability, /* compile-time constant expression */
					 u32_ext_rx, hfi1_type);
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC: {
		uint64_t *origin_byte_counter = (uint64_t *) hdr->cts.target.vaddr.origin_byte_counter_vaddr;
		*origin_byte_counter	      = 0;
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_GET: {
		const uintptr_t	  rma_request_vaddr = hdr->cts.target.mr.rma_request_vaddr;
		struct fi_opx_mr *opx_mr	    = NULL;
		const uint32_t	  niov		    = hdr->cts.target.mr.niov;
		uint64_t	  temp_key[2]	    = {[0] = hdr->cts.target.mr.key, [1] = 0ul};

		HASH_FIND(hh, opx_ep->domain->mr_hashmap, &temp_key[0], sizeof(hdr->cts.target.mr.key), opx_mr);
		// Permissions (TODO)
		// check MR permissions
		// nack on failed lookup
		assert(opx_mr != NULL);

#ifdef OPX_HMEM
		// Our MR code only supports 1 IOV per registration.
		uint64_t	   hmem_device;
		uint64_t	   hmem_handle;
		enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(opx_mr, &hmem_device, &hmem_handle);
		assert(niov == 1);
		const union opx_hfi1_dput_iov	     dput_iov	  = {.rbuf	  = payload->cts.iov[0].rbuf,
								     .sbuf	  = payload->cts.iov[0].sbuf,
								     .bytes	  = payload->cts.iov[0].bytes,
								     .rbuf_iface  = payload->cts.iov[0].rbuf_iface,
								     .rbuf_device = payload->cts.iov[0].rbuf_device,
								     .sbuf_iface  = hmem_iface,
								     .sbuf_device = hmem_device,
								     .sbuf_handle = hmem_handle};
		const union opx_hfi1_dput_iov *const dput_iov_ptr = &dput_iov;
#else
		const union opx_hfi1_dput_iov *const dput_iov_ptr = payload->cts.iov;
#endif
		FI_OPX_FABRIC_RX_RZV_CTS(
			opx_ep, hdr, (const void *const) payload, 0, origin_rx, niov, dput_iov_ptr, opx_mr->base_addr,
			hdr->cts.target.mr.op, hdr->cts.target.mr.dt, rma_request_vaddr,
			(uintptr_t) NULL,			  /* Target completion counter is in rma_request */
			NULL,					  /* No origin byte counter here */
			FI_OPX_HFI_DPUT_OPCODE_GET, NULL, is_shm, /* compile-time constant expression */
			reliability,				  /* compile-time constant expression */
			u32_ext_rx, hfi1_type);
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_PUT_CQ: {
		const union opx_hfi1_dput_iov *const dput_iov_ptr = payload->cts.iov;
		FI_OPX_FABRIC_RX_RZV_CTS(opx_ep, hdr, (const void *const) payload, 0, origin_rx,
					 hdr->cts.target.rma.niov, dput_iov_ptr, NULL, hdr->cts.target.rma.op,
					 hdr->cts.target.rma.dt, hdr->cts.target.rma.origin_rma_request_vaddr,
					 hdr->cts.target.rma.rma_request_vaddr, NULL, /* No origin byte counter here */
					 FI_OPX_HFI_DPUT_OPCODE_PUT_CQ, NULL,
					 is_shm,      /* compile-time constant expression */
					 reliability, /* compile-time constant expression */
					 u32_ext_rx, hfi1_type);
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_FENCE: {
		opx_hfi1_dput_fence(opx_ep, hdr, origin_rx, u32_ext_rx, hfi1_type);
	} break;
#ifdef OPX_HMEM
	case FI_OPX_HFI_DPUT_OPCODE_IPC: {
		uint64_t *origin_byte_counter = (uint64_t *) hdr->cts.target.vaddr.origin_byte_counter_vaddr;
		*origin_byte_counter	      = 0;
		OPX_TRACER_TRACE(OPX_TRACER_INSTANT, "IPC-SEND-PROCESS-CTS");
	} break;
#endif
	default:
		abort();
		break;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- %s RENDEZVOUS CTS (end)\n", is_shm ? "SHM" : "HFI");
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_header_rma_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
					 const union fi_opx_hfi1_packet_payload *const payload, const unsigned is_shm,
					 const int lock_required, const enum ofi_reliability_kind reliability,
					 const enum opx_hfi1_type hfi1_type)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- RMA RTS (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RMA-RTS");

	struct fi_opx_ep_rx *const rx = opx_ep->rx;
	assert(payload != NULL);

	uint64_t temp_key[2] = {[0] = hdr->rma_rts.key, [1] = 0ul};

	struct fi_opx_mr *opx_mr = NULL;
	HASH_FIND(hh, opx_ep->domain->mr_hashmap, &temp_key[0], sizeof(hdr->rma_rts.key), opx_mr);

	if (opx_mr == NULL) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "lookup of key (%ld) failed; packet dropped\n",
			hdr->rma_rts.key);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RECV -- RMA RTS - failed (end)\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "RECV-RMA-RTS");
		assert(0);
		return;
	}

	uint64_t	  *rbuf_qws = (uint64_t *) (((uint8_t *) opx_mr->base_addr) + payload->rma_rts.iov[0].rbuf);
	uint64_t	   rbuf_device;
	uint64_t	   rbuf_handle;
	enum fi_hmem_iface rbuf_iface = opx_hmem_get_mr_iface(opx_mr, &rbuf_device, &rbuf_handle);

	struct opx_context *context   = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
	context->flags		      = FI_REMOTE_CQ_DATA | FI_RMA | FI_REMOTE_WRITE;
	context->data		      = hdr->match.ofi_data;
	context->buf		      = rbuf_qws;
	context->next		      = NULL;
	context->tag		      = 0;
	context->err_entry.err	      = 0;
	context->err_entry.op_context = NULL;

	opx_hfi1_rx_rma_rts(opx_ep, hdr, payload, hdr->rma_rts.niov, hdr->rma_rts.rma_request_vaddr, context,
			    (uintptr_t) rbuf_qws, rbuf_iface, rbuf_device, rbuf_handle, payload->rma_rts.iov, is_shm,
			    reliability, /* compile-time constant expression */
			    hfi1_type);

	/* post a pending completion event for when the PUT completes */
	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}
	if (context->len) {
		slist_insert_tail((struct slist_entry *) context, rx->cq_pending_ptr);
	} else { /* length is 0, there will be no RZV data sent, so post now */
		slist_insert_tail((struct slist_entry *) context, rx->cq_completed_ptr);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RMA-RTS");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- RMA RTS (end) context %p\n", context);
}

void fi_opx_atomic_completion_action(union fi_opx_hfi1_deferred_work *work_state);

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_header_rzv_data(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
					  const union fi_opx_hfi1_packet_payload *const payload,
					  const size_t payload_bytes, const unsigned is_shm, const int lock_required,
					  const enum ofi_reliability_kind reliability,
					  const enum opx_hfi1_type	  hfi1_type)
{
	const uint8_t  opcode	 = FI_OPX_HFI_DPUT_GET_OPCODE(hdr->dput.target.opcode_origin_rx);
	const uint16_t origin_rx = FI_OPX_HFI1_PACKET_ORIGIN_RX(hdr);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- %s RENDEZVOUS DATA Opcode=%0hhX (begin)\n",
		     is_shm ? "SHM" : "HFI", opcode);
	switch (opcode) {
	case FI_OPX_HFI_DPUT_OPCODE_RZV:
	case FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG: {
		struct fi_opx_rzv_completion *rzv_comp =
			(struct fi_opx_rzv_completion *) (hdr->dput.target.rzv.completion_vaddr);
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-DATA-HFI-DPUT:%p", rzv_comp);
		struct opx_context *target_context = rzv_comp->context;
		assert(target_context);
		uint64_t *rbuf_qws = (uint64_t *) fi_opx_dput_rbuf_in(hdr->dput.target.rzv.rbuf);

		/* In a multi-packet SDMA send, the driver sets the high bit on
		 * in the PSN to indicate this is the last packet. The payload
		 * size of the last packet may be smaller than the other packets
		 * in the multi-packet send, so set the payload bytes accordingly */
		const uint16_t bytes =
			(ntohl(hdr->bth.psn) & 0x80000000) ? hdr->dput.target.last_bytes : hdr->dput.target.bytes;

		assert(bytes <= OPX_HFI1_PKT_SIZE);
#ifndef NDEBUG
		if (bytes == 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Received RZV (non-TID) data packet with 0-byte payload size. hdr->dput.target.last_bytes=%hd, hdr->dput.target.bytes=%hd. Based on PSN high bit (%s), bytes was set to %s\n",
				hdr->dput.target.last_bytes, hdr->dput.target.bytes,
				(ntohl(hdr->bth.psn) & 0x80000000) ? "ON" : "OFF",
				(ntohl(hdr->bth.psn) & 0x80000000) ? "last_bytes" : "bytes");
			abort();
		}
#endif
		const uint64_t *sbuf_qws = (uint64_t *) &payload->byte[0];
#ifdef OPX_HMEM
		if (target_context->flags & FI_OPX_CQ_CONTEXT_HMEM) {
			struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) target_context->hmem_info_qws;
			assert(hmem_info->iface > FI_HMEM_SYSTEM);
			opx_copy_to_hmem(hmem_info->iface, hmem_info->device, hmem_info->hmem_dev_reg_handle, rbuf_qws,
					 sbuf_qws, bytes, OPX_HMEM_DEV_REG_RECV_THRESHOLD);
		} else
#endif
		{
			memcpy(rbuf_qws, sbuf_qws, bytes);
		}

		assert(rzv_comp->byte_counter >= bytes);
		rzv_comp->bytes_accumulated += bytes;
		rzv_comp->byte_counter -= bytes;

		if (rzv_comp->byte_counter == 0) {
			assert(target_context->byte_counter >= rzv_comp->bytes_accumulated);
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "hdr->dput.target.last_bytes = %hu, hdr->dput.target.bytes = %u, bytes = %u, rzv_comp->bytes_accumulated=%lu, target_byte_counter = %p, %lu -> %lu\n",
			       hdr->dput.target.last_bytes, hdr->dput.target.bytes, bytes, rzv_comp->bytes_accumulated,
			       &target_context->byte_counter, target_context->byte_counter,
			       target_context->byte_counter - rzv_comp->bytes_accumulated);

			target_context->byte_counter -= rzv_comp->bytes_accumulated;

			/* free the rendezvous completion structure */
			OPX_BUF_FREE(rzv_comp);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-DATA-HFI-DPUT:%p", rzv_comp);
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_RZV_TID: {
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RX_PROCESS_HEADER_RZV_TID");
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_rcv_pkts);
		struct fi_opx_rzv_completion *rzv_comp =
			(struct fi_opx_rzv_completion *) (hdr->dput.target.rzv.completion_vaddr);
		struct opx_context *target_context = rzv_comp->context;
		assert(target_context);

		/* TID packets are mixed 4k/8k packets and length adjusted,
		 * so use actual packet size here reported in LRH as the
		 * number of 4-byte words in the packet; header + payload - icrc
		 */
		uint16_t lrh_pktlen_le;
		size_t	 total_bytes_to_copy;
		uint16_t bytes;

		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			lrh_pktlen_le	    = __be16_to_cpu(hdr->lrh_9B.pktlen);
			total_bytes_to_copy = (lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
			bytes = (uint16_t) (total_bytes_to_copy - sizeof(struct fi_opx_hfi1_stl_packet_hdr_9B));
		} else {
			lrh_pktlen_le	    = hdr->lrh_16B.pktlen;
			total_bytes_to_copy = (lrh_pktlen_le - 1) * 8; /* do not copy the trailing icrc */
			bytes = (uint16_t) ((total_bytes_to_copy - sizeof(struct fi_opx_hfi1_stl_packet_hdr_16B)));
		}

		assert(bytes <= OPX_HFI1_PKT_SIZE);

		/* SDMA expected receive w/TID will use CTRL 1, 2 or 3.
		   Replays should indicate we are not using TID (CTRL 0) */
		int tidctrl = KDETH_GET(hdr->kdeth.offset_ver_tid, TIDCTRL);
		assert((tidctrl == 0) || (tidctrl == 1) || (tidctrl == 2) || (tidctrl == 3));

		/* Copy only if there's a replay payload and TID direct rdma was NOT done.
		 * Note: out of order queued TID packets appear to have a
		 * payload when they don't, so checking tidctrl (not a replay) is necessary.
		 */
		if ((payload != NULL) && (tidctrl == 0)) {
			uint64_t       *rbuf_qws = (uint64_t *) fi_opx_dput_rbuf_in(hdr->dput.target.rzv.rbuf);
			const uint64_t *sbuf_qws = (uint64_t *) &payload->byte[0];
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "TID REPLAY rbuf_qws %p, sbuf_qws %p, bytes %u/%#x, target_context->byte_counter %p\n",
			       (void *) rbuf_qws, (void *) sbuf_qws, bytes, bytes, &target_context->byte_counter);
			if (target_context->flags & FI_OPX_CQ_CONTEXT_HMEM) {
				struct fi_opx_hmem_info *hmem_info =
					(struct fi_opx_hmem_info *) target_context->hmem_info_qws;
				assert(hmem_info->iface > FI_HMEM_SYSTEM);
				opx_copy_to_hmem(hmem_info->iface, hmem_info->device, hmem_info->hmem_dev_reg_handle,
						 rbuf_qws, sbuf_qws, bytes, OPX_HMEM_DEV_REG_RECV_THRESHOLD);
			} else {
				memcpy(rbuf_qws, sbuf_qws, bytes);
			}
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_rcv_pkts_replays);
		}
#ifndef NDEBUG
		else { /* Debug, tracking where the TID wrote even though we don't memcpy here */
			uint64_t       *rbuf_qws = (uint64_t *) fi_opx_dput_rbuf_in(hdr->dput.target.rzv.rbuf);
			const uint64_t *sbuf_qws = (uint64_t *) &payload->byte[0];
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "NOT REPLAY tidctrl %#x, tid %#X, tid0M %#X, tidoffset %#X rbuf_qws %p, "
			       "sbuf_qws %p, bytes %u/%#x, target_context->byte_counter %p\n",
			       tidctrl, KDETH_GET(hdr->kdeth.offset_ver_tid, TID),
			       KDETH_GET(hdr->kdeth.offset_ver_tid, OM), KDETH_GET(hdr->kdeth.offset_ver_tid, OFFSET),
			       (void *) rbuf_qws, (void *) sbuf_qws, bytes, bytes, &target_context->byte_counter);
		}
#endif
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "hdr->dput.target.last_bytes = %hu, hdr->dput.target.bytes = %u, bytes = %u, target_context->byte_counter = %p, %lu -> %lu\n",
		       hdr->dput.target.last_bytes, hdr->dput.target.bytes, bytes, &target_context->byte_counter,
		       rzv_comp->byte_counter, rzv_comp->byte_counter - bytes);
		assert(rzv_comp->byte_counter >= bytes);
		rzv_comp->bytes_accumulated += bytes;
		rzv_comp->byte_counter -= bytes;

		/* On completion, decrement TID refcount and maybe free the TID cache */
		if (rzv_comp->byte_counter == 0) {
			const uint64_t tid_vaddr  = rzv_comp->tid_vaddr;
			const uint64_t tid_length = rzv_comp->tid_length;
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "tid vaddr>buf [%p - %p] tid len %lu/%#lX\n",
			       (void *) tid_vaddr, (void *) (tid_vaddr + tid_length), tid_length, tid_length);
			target_context->byte_counter -= rzv_comp->bytes_accumulated;

			opx_deregister_for_rzv(opx_ep, tid_vaddr, tid_length);

			/* free the rendezvous completion structure */
			OPX_BUF_FREE(rzv_comp);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RX_PROCESS_HEADER_RZV_TID");
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_PUT: {
		assert(payload != NULL);
		const uint64_t	 *sbuf_qws    = (uint64_t *) &payload->byte[0];
		struct fi_opx_mr *opx_mr      = NULL;
		uint64_t	  temp_key[2] = {[0] = hdr->dput.target.mr.key, [1] = 0ul};
		HASH_FIND(hh, opx_ep->domain->mr_hashmap, &temp_key[0], sizeof(hdr->dput.target.mr.key), opx_mr);

		if (opx_mr == NULL) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "lookup of key (%ld) failed; packet dropped\n",
				hdr->dput.target.mr.key);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== RECV -- RENDEZVOUS DATA - failed (end)\n");
			assert(0);
			return;
		}

		uint64_t	  *rbuf_qws = (uint64_t *) (((uint8_t *) opx_mr->base_addr) +
						    fi_opx_dput_rbuf_in(hdr->dput.target.mr.offset));
		uint64_t	   hmem_device;
		uint64_t	   hmem_handle;
		enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(opx_mr, &hmem_device, &hmem_handle);

		/* In a multi-packet SDMA send, the driver sets the high bit on
		 * in the PSN to indicate this is the last packet. The payload
		 * size of the last packet may be smaller than the other packets
		 * in the multi-packet send, so set the payload bytes accordingly */
		const uint16_t bytes =
			(ntohl(hdr->bth.psn) & 0x80000000) ? hdr->dput.target.last_bytes : hdr->dput.target.bytes;
		assert(bytes <= OPX_HFI1_PKT_SIZE);

#ifndef NDEBUG
		if (bytes == 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Received RMA PUT data packet with 0-byte payload size. hdr->dput.target.last_bytes=%hd, hdr->dput.target.bytes=%hd. Based on PSN high bit (%s), bytes was set to %s\n",
				hdr->dput.target.last_bytes, hdr->dput.target.bytes,
				(ntohl(hdr->bth.psn) & 0x80000000) ? "ON" : "OFF",
				(ntohl(hdr->bth.psn) & 0x80000000) ? "last_bytes" : "bytes");
			abort();
		}
#endif
		// Optimize Memcpy
		if (hdr->dput.target.op == FI_NOOP - 1 && hdr->dput.target.dt == FI_VOID - 1) {
			OPX_HMEM_COPY_TO(rbuf_qws, sbuf_qws, bytes, hmem_handle, OPX_HMEM_DEV_REG_RECV_THRESHOLD,
					 hmem_iface, hmem_device);
		} else {
			OPX_HMEM_ATOMIC_DISPATCH(sbuf_qws, rbuf_qws, bytes, hdr->dput.target.dt, hdr->dput.target.op,
						 hmem_iface, hmem_device);
		}
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_PUT_CQ: {
		assert(payload != NULL);
		struct fi_opx_rma_request *rma_req =
			(struct fi_opx_rma_request *) hdr->dput.target.rma.rma_request_vaddr;
		struct opx_context *context  = rma_req->context;
		uint64_t	   *rbuf_qws = (uint64_t *) fi_opx_dput_rbuf_in(hdr->dput.target.rma.offset);
		const uint64_t	   *sbuf_qws = (uint64_t *) &payload->byte[0];

		/* In a multi-packet SDMA send, the driver sets the high bit on
		 * in the PSN to indicate this is the last packet. The payload
		 * size of the last packet may be smaller than the other packets
		 * in the multi-packet send, so set the payload bytes accordingly */
		const uint16_t bytes =
			(ntohl(hdr->bth.psn) & 0x80000000) ? hdr->dput.target.last_bytes : hdr->dput.target.bytes;
		assert(context);
		assert(bytes <= OPX_HFI1_PKT_SIZE);

#ifndef NDEBUG
		if (bytes == 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Received RMA PUT data packet with 0-byte payload size. hdr->dput.target.last_bytes=%hd, hdr->dput.target.bytes=%hd. Based on PSN high bit (%s), bytes was set to %s\n",
				hdr->dput.target.last_bytes, hdr->dput.target.bytes,
				(ntohl(hdr->bth.psn) & 0x80000000) ? "ON" : "OFF",
				(ntohl(hdr->bth.psn) & 0x80000000) ? "last_bytes" : "bytes");
			abort();
		}
#endif
		// Optimize Memcpy
		if (hdr->dput.target.op == (FI_NOOP - 1) && hdr->dput.target.dt == (FI_VOID - 1)) {
			OPX_HMEM_COPY_TO(rbuf_qws, sbuf_qws, bytes, rma_req->hmem_handle,
					 OPX_HMEM_DEV_REG_RECV_THRESHOLD, rma_req->hmem_iface, rma_req->hmem_device);
		} else {
			OPX_HMEM_ATOMIC_DISPATCH(sbuf_qws, rbuf_qws, bytes, hdr->dput.target.dt, hdr->dput.target.op,
						 rma_req->hmem_iface, rma_req->hmem_device);
		}

		const uint64_t value = context->byte_counter;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "hdr->dput.target.last_bytes = %hu, hdr->dput.target.bytes = %u, bytes = %u, byte_counter = %lu, %lu -> %lu\n",
		       hdr->dput.target.last_bytes, hdr->dput.target.bytes, bytes, context->byte_counter, value,
		       value - bytes);
		assert(value >= bytes);
		context->byte_counter = value - bytes;

		if (value == bytes) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "RENDEZVOUS DATA; enqueue cq (completed), ofi_data %#lX \n", context->data);
			OPX_BUF_FREE(rma_req);
		}
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_GET: {
		assert(payload != NULL);
		struct fi_opx_rma_request *rma_req =
			(struct fi_opx_rma_request *) hdr->dput.target.get.rma_request_vaddr;
		struct fi_opx_completion_counter *cc = rma_req->cc;
		uint64_t       *rbuf_qws	     = (uint64_t *) fi_opx_dput_rbuf_in(hdr->dput.target.get.rbuf);
		const uint64_t *sbuf_qws	     = (uint64_t *) &payload->byte[0];

		/* In a multi-packet SDMA send, the driver sets the high bit on
		 * in the PSN to indicate this is the last packet. The payload
		 * size of the last packet may be smaller than the other packets
		 * in the multi-packet send, so set the payload bytes accordingly */
		const uint16_t bytes =
			(ntohl(hdr->bth.psn) & 0x80000000) ? hdr->dput.target.last_bytes : hdr->dput.target.bytes;

		assert(cc);
		assert(bytes <= OPX_HFI1_PKT_SIZE);

#ifndef NDEBUG
		if (bytes == 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Received RMA GET data packet with 0-byte payload size. hdr->dput.target.last_bytes=%hd, hdr->dput.target.bytes=%hd. Based on PSN high bit (%s), bytes was set to %s\n",
				hdr->dput.target.last_bytes, hdr->dput.target.bytes,
				(ntohl(hdr->bth.psn) & 0x80000000) ? "ON" : "OFF",
				(ntohl(hdr->bth.psn) & 0x80000000) ? "last_bytes" : "bytes");
			abort();
		}
#endif
		if (hdr->dput.target.dt == (FI_VOID - 1)) {
			OPX_HMEM_COPY_TO(rbuf_qws, sbuf_qws, bytes, rma_req->hmem_handle,
					 OPX_HMEM_DEV_REG_RECV_THRESHOLD, rma_req->hmem_iface, rma_req->hmem_device);
		} else {
			OPX_HMEM_ATOMIC_DISPATCH(sbuf_qws, rbuf_qws, bytes, hdr->dput.target.dt, FI_ATOMIC_WRITE,
						 rma_req->hmem_iface, rma_req->hmem_device);
		}
		assert(cc->byte_counter >= bytes);
		cc->byte_counter -= bytes;
		assert(cc->byte_counter >= 0);

		if (cc->byte_counter == 0) {
			OPX_BUF_FREE(rma_req);
			cc->hit_zero(cc);
		}
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH: {
		const uint32_t	  u32_ext_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, origin_rx);
		struct fi_opx_mr *opx_mr     = NULL;

		uint64_t key = hdr->dput.target.mr.key;
		HASH_FIND(hh, opx_ep->domain->mr_hashmap, &key, sizeof(key), opx_mr);
		assert(opx_mr != NULL);
		uintptr_t mr_offset = fi_opx_dput_rbuf_in(hdr->dput.target.mr.offset);
		uint64_t *rbuf_qws  = (uint64_t *) (((uint8_t *) opx_mr->base_addr) + mr_offset);
		const struct fi_opx_hfi1_dput_fetch *dput_fetch = (struct fi_opx_hfi1_dput_fetch *) &payload->byte[0];

		/* In a multi-packet SDMA send, the driver sets the high bit on
		 * in the PSN to indicate this is the last packet. The payload
		 * size of the last packet may be smaller than the other packets
		 * in the multi-packet send, so set the payload bytes accordingly */
		const uint16_t bytes =
			(ntohl(hdr->bth.psn) & 0x80000000) ? hdr->dput.target.last_bytes : hdr->dput.target.bytes;

#ifndef NDEBUG
		if (bytes == 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Received ATOMIC FETCH data packet with 0-byte payload size. hdr->dput.target.last_bytes=%hd, hdr->dput.target.bytes=%hd. Based on PSN high bit (%s), bytes was set to %s\n",
				hdr->dput.target.last_bytes, hdr->dput.target.bytes,
				(ntohl(hdr->bth.psn) & 0x80000000) ? "ON" : "OFF",
				(ntohl(hdr->bth.psn) & 0x80000000) ? "last_bytes" : "bytes");
			abort();
		}
#endif
		assert(bytes > sizeof(*dput_fetch));
		uint64_t	   hmem_device;
		uint64_t	   hmem_handle;
		enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(opx_mr, &hmem_device, &hmem_handle);

		// rbuf_iface and rbuf_hmem are contained in the rma_request that
		// resides in the originating endpoint, so can just be set to
		// system/0/OPX_HMEM_NO_HANDLE here.
		union opx_hfi1_dput_iov dput_iov = {.sbuf	 = mr_offset,
						    .rbuf	 = dput_fetch->fetch_rbuf,
						    .bytes	 = bytes - sizeof(struct fi_opx_hfi1_dput_fetch),
						    .rbuf_iface	 = FI_HMEM_SYSTEM,
						    .sbuf_iface	 = hmem_iface,
						    .rbuf_device = 0,
						    .sbuf_device = hmem_device,
						    .sbuf_handle = hmem_handle};
		assert(dput_iov.bytes <= OPX_HFI1_PKT_SIZE - sizeof(*dput_fetch));
		assert(hdr->dput.target.op != (FI_NOOP - 1));
		assert(hdr->dput.target.dt != (FI_VOID - 1));

		// Do the FETCH part of this atomic fetch operation
		union fi_opx_hfi1_deferred_work *work = FI_OPX_FABRIC_RX_RZV_CTS(
			opx_ep, hdr, (const void *const) payload, bytes, origin_rx, 1, &dput_iov, opx_mr->base_addr,
			hdr->dput.target.op, hdr->dput.target.dt, dput_fetch->rma_request_vaddr,
			(uintptr_t) NULL, /* target byte counter is in rma_request */
			NULL,		  /* No origin byte counter here */
			FI_OPX_HFI_DPUT_OPCODE_GET, fi_opx_atomic_completion_action, is_shm, reliability, u32_ext_rx,
			hfi1_type);
		if (work == NULL) {
			// The FETCH completed without being deferred, now do
			// the actual atomic operation.
			const uint64_t *sbuf_qws = (uint64_t *) (dput_fetch + 1);
			OPX_HMEM_ATOMIC_DISPATCH(sbuf_qws, rbuf_qws, dput_iov.bytes, hdr->dput.target.dt,
						 hdr->dput.target.op, hmem_iface, hmem_device);
		}
		// else the FETCH was deferred, so the atomic operation will
		// be done upon FETCH completion via fi_opx_atomic_completion_action
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH: {
		const uint32_t	  u32_ext_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, origin_rx);
		struct fi_opx_mr *opx_mr     = NULL;

		uint64_t key = hdr->dput.target.mr.key;
		HASH_FIND(hh, opx_ep->domain->mr_hashmap, &key, sizeof(key), opx_mr);
		assert(opx_mr != NULL);
		uintptr_t mr_offset = fi_opx_dput_rbuf_in(hdr->dput.target.mr.offset);
		uint64_t *rbuf_qws  = (uint64_t *) (((uint8_t *) opx_mr->base_addr) + mr_offset);
		const struct fi_opx_hfi1_dput_fetch *dput_fetch = (struct fi_opx_hfi1_dput_fetch *) &payload->byte[0];

		/* In a multi-packet SDMA send, the driver sets the high bit on
		 * in the PSN to indicate this is the last packet. The payload
		 * size of the last packet may be smaller than the other packets
		 * in the multi-packet send, so set the payload bytes accordingly */
		const uint16_t bytes =
			(ntohl(hdr->bth.psn) & 0x80000000) ? hdr->dput.target.last_bytes : hdr->dput.target.bytes;

#ifndef NDEBUG
		if (bytes == 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Received ATOMIC COMPARE FETCH data packet with 0-byte payload size. hdr->dput.target.last_bytes=%hd, hdr->dput.target.bytes=%hd. Based on PSN high bit (%s), bytes was set to %s\n",
				hdr->dput.target.last_bytes, hdr->dput.target.bytes,
				(ntohl(hdr->bth.psn) & 0x80000000) ? "ON" : "OFF",
				(ntohl(hdr->bth.psn) & 0x80000000) ? "last_bytes" : "bytes");
			abort();
		}
#endif
		assert(bytes > sizeof(*dput_fetch));
		uint64_t	   hmem_device;
		uint64_t	   hmem_handle;
		enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(opx_mr, &hmem_device, &hmem_handle);

		// rbuf_iface and rbuf_hmem are contained in the rma_request that
		// resides in the originating endpoint, so can just be set to
		// system/0/OPX_HMEM_NO_HANDLE here.
		union opx_hfi1_dput_iov dput_iov = {.sbuf	 = mr_offset,
						    .rbuf	 = dput_fetch->fetch_rbuf,
						    .bytes	 = (bytes - sizeof(struct fi_opx_hfi1_dput_fetch)) >> 1,
						    .rbuf_iface	 = FI_HMEM_SYSTEM,
						    .sbuf_iface	 = hmem_iface,
						    .rbuf_device = 0,
						    .sbuf_device = hmem_device,
						    .sbuf_handle = hmem_handle};
		assert(dput_iov.bytes <= ((OPX_HFI1_PKT_SIZE - sizeof(*dput_fetch)) >> 1));
		assert(hdr->dput.target.op != (FI_NOOP - 1));
		assert(hdr->dput.target.dt != (FI_VOID - 1));

		// Do the FETCH part of this atomic fetch operation
		union fi_opx_hfi1_deferred_work *work = FI_OPX_FABRIC_RX_RZV_CTS(
			opx_ep, hdr, (const void *const) payload, bytes, origin_rx, 1, &dput_iov, opx_mr->base_addr,
			hdr->dput.target.op, hdr->dput.target.dt, dput_fetch->rma_request_vaddr,
			(uintptr_t) NULL, /* Target completion counter is in rma request */
			NULL,		  /* No origin byte counter here */
			FI_OPX_HFI_DPUT_OPCODE_GET, fi_opx_atomic_completion_action, is_shm, reliability, u32_ext_rx,
			hfi1_type);
		if (work == NULL) {
			// The FETCH completed without being deferred, now do
			// the actual atomic operation.
			const uint64_t *sbuf_qws = (uint64_t *) (dput_fetch + 1);
			OPX_HMEM_ATOMIC_DISPATCH(sbuf_qws, rbuf_qws, dput_iov.bytes, hdr->dput.target.dt,
						 hdr->dput.target.op, hmem_iface, hmem_device);
		}
		// else the FETCH was deferred, so the atomic operation will
		// be done upon FETCH completion via fi_opx_atomic_completion_action
	} break;
	case FI_OPX_HFI_DPUT_OPCODE_FENCE: {
		assert(payload != NULL);
		struct fi_opx_completion_counter *cc =
			(struct fi_opx_completion_counter *) hdr->dput.target.fence.completion_counter;
		const uint32_t bytes = hdr->dput.target.fence.bytes_to_fence;

		assert(cc);
		assert(cc->byte_counter >= bytes);
		cc->byte_counter -= bytes;
		if (cc->byte_counter == 0) {
			cc->hit_zero(cc);
		}
	} break;
	default:
		abort();
		break;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== RECV -- %s RENDEZVOUS DATA (end)\n",
		     is_shm ? "SHM" : "HFI");
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_header_non_eager(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
					   const union fi_opx_hfi1_packet_payload *const payload,
					   const size_t payload_bytes, const uint64_t static_flags,
					   const uint8_t opcode, const unsigned is_shm, const int lock_required,
					   const enum ofi_reliability_kind reliability,
					   const enum opx_hfi1_type	   hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (opcode == FI_OPX_HFI_BTH_OPCODE_RZV_CTS) {
		fi_opx_ep_rx_process_header_rzv_cts(opx_ep, hdr, payload, is_shm, lock_required, reliability,
						    hfi1_type);
	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA) {
		fi_opx_ep_rx_process_header_rzv_data(opx_ep, hdr, payload, payload_bytes, is_shm, lock_required,
						     reliability, hfi1_type);
	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_ACK) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "unimplemented opcode (%u); abort\n", opcode);
		abort();
	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_RMA_RTS) {
		fi_opx_ep_rx_process_header_rma_rts(opx_ep, hdr, payload, is_shm, lock_required, reliability,
						    hfi1_type);
	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_ATOMIC) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "unimplemented opcode (%u); abort\n", opcode);
		abort();
	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_UD) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "reliability exception with opcode %d, dropped\n", opcode);
	} else {
		fprintf(stderr, "unimplemented opcode (%#x); abort\n", opcode);
		fprintf(stderr,
			"%s:%u payload %p, payload bytes %zu, is_instranode %u,  %#16.16llX %#16.16llX %#16.16llX %#16.16llX %#16.16llX %#16.16llX %#16.16llX \n",
			__func__, __LINE__, payload, payload_bytes, is_shm, (long long) hdr->qw_9B[0],
			(long long) hdr->qw_9B[1], (long long) hdr->qw_9B[2], (long long) hdr->qw_9B[3],
			(long long) hdr->qw_9B[4], (long long) hdr->qw_9B[5], (long long) hdr->qw_9B[6]);
		abort();
	}
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_mp_egr_id_from_nth_packet(const union opx_hfi1_packet_hdr *hdr, const opx_lid_t slid)
{
	return ((((uint64_t) hdr->mp_eager_nth.mp_egr_uid) << 40) | ((uint64_t) slid << 16) |
		(uint64_t) hdr->reliability.origin_rx);
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_pending_mp_eager_ue(struct fid_ep *ep, struct opx_context *context, uint64_t mp_egr_id,
					      const unsigned is_shm, const int lock_required,
					      const enum ofi_reliability_kind reliability,
					      const enum opx_hfi1_type	      hfi1_type)
{
	struct fi_opx_ep	     *opx_ep  = container_of(ep, struct fi_opx_ep, ep_fid);
	const uint64_t		      is_hmem = context->flags & FI_OPX_CQ_CONTEXT_HMEM;
	struct fi_opx_hfi1_ue_packet *uepkt   = opx_ep->rx->mp_egr_queue.ue.head;

	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(length);

	while (uepkt && context->byte_counter) {
		opx_lid_t slid;
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			slid = (opx_lid_t) __be16_to_cpu24(((__be16) (uepkt->hdr.lrh_9B.slid)));
		} else {
			slid = (opx_lid_t) __le24_to_cpu((uepkt->hdr.lrh_16B.slid20 << 20) | (uepkt->hdr.lrh_16B.slid));
		}

		if (fi_opx_mp_egr_id_from_nth_packet(&uepkt->hdr, slid) == mp_egr_id) {
			opx_ep_complete_receive_operation(
				ep, &uepkt->hdr, &uepkt->payload, 0, /* OFI Tag, N/A for multi-packet eager nth */
				context, FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH, OPX_MULTI_RECV_FALSE, OPX_SHM_FALSE,
				is_hmem, lock_required, reliability, hfi1_type);

			/* Remove this packet and get the next one */
			uepkt = fi_opx_hfi1_ue_packet_slist_remove_item(uepkt, &opx_ep->rx->mp_egr_queue.ue);
		} else {
			uepkt = uepkt->next;
		}
		FI_OPX_DEBUG_COUNTERS_INC(length);
	}

	FI_OPX_DEBUG_COUNTERS_MAX_OF(opx_ep->debug_counters.mp_eager.recv_max_ue_queue_length, length);
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_header_mp_eager_first(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
						const union fi_opx_hfi1_packet_payload *const payload,
						const size_t payload_bytes, const uint64_t static_flags,
						const uint8_t opcode, const unsigned is_shm, const int lock_required,
						const enum ofi_reliability_kind reliability,
						const enum opx_hfi1_type hfi1_type, const opx_lid_t slid)
{
	struct fi_opx_ep *opx_ep    = container_of(ep, struct fi_opx_ep, ep_fid);
	const uint16_t	  origin_rx = FI_OPX_HFI1_PACKET_ORIGIN_RX(hdr);

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.recv_first_packets);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "search the match queue\n");

	const uint64_t kind = (static_flags & FI_TAGGED) ? FI_OPX_KIND_TAG : FI_OPX_KIND_MSG;
	assert((kind == FI_OPX_KIND_TAG && FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode)) ||
	       (kind == FI_OPX_KIND_MSG && !FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode)));
	struct opx_context *context = (struct opx_context *) opx_ep->rx->queue[kind].mq.head;
	struct opx_context *prev    = NULL;

	while (context &&
	       !is_match(opx_ep, hdr, context, opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, is_shm, slid)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "context = %p\n", context);
		prev	= context;
		context = context->next;
	}

	if (!context) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "did not find a match .. add this packet to the unexpected queue\n");

		if (OFI_LIKELY(FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode))) {
			fi_opx_ep_rx_append_ue_tag(opx_ep->rx, hdr, payload, payload_bytes, is_shm,
						   opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst,
						   opx_ep->daos_info.hfi_rank_enabled,
						   FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep), slid, 0UL);
		} else {
			fi_opx_ep_rx_append_ue_msg(opx_ep->rx, hdr, payload, payload_bytes, is_shm,
						   opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst,
						   opx_ep->daos_info.hfi_rank_enabled,
						   FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep), slid, 0UL);
		}

		return;
	}

	/* Found a match. Remove from the match queue */
	slist_remove(&opx_ep->rx->queue[kind].mq, (struct slist_entry *) context, (struct slist_entry *) prev);

	uint64_t is_hmem = context->flags & FI_OPX_CQ_CONTEXT_HMEM;

	/* Copy this packet's payload to the context's buffer. */
	opx_ep_complete_receive_operation(ep, hdr, payload, hdr->match.ofi_tag, context, opcode, OPX_MULTI_RECV_FALSE,
					  OPX_SHM_FALSE, /* Should always be false for mp_eager */
					  is_hmem, lock_required, reliability, hfi1_type);

	const uint64_t mp_egr_id = OPX_GET_MP_EGR_ID(hdr->reliability.psn, slid, origin_rx);

	/* Process any other early arrival packets that are part of this multi-packet egr */
	fi_opx_ep_rx_process_pending_mp_eager_ue(ep, context, mp_egr_id, is_shm, lock_required, reliability, hfi1_type);

	/* Only add this to the multi-packet egr queue if we still expect additional packets to come in */
	if (context->byte_counter) {
		context->mp_egr_id = mp_egr_id;
		slist_insert_tail((struct slist_entry *) context, &opx_ep->rx->mp_egr_queue.mq);
	} else {
		context->next = NULL;

		if (OFI_UNLIKELY(context->err_entry.err == FI_ETRUNC)) {
			slist_insert_tail((struct slist_entry *) context, opx_ep->rx->cq_err_ptr);
		} else {
			fi_opx_enqueue_completed(opx_ep->rx->cq_completed_ptr, context, lock_required);
		}
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.recv_completed_eager_first);
	}
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_process_header_mp_eager_nth(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
					      const union fi_opx_hfi1_packet_payload *const payload,
					      const size_t payload_bytes, const uint64_t static_flags,
					      const uint8_t opcode, const unsigned is_shm, const int lock_required,
					      const enum ofi_reliability_kind reliability,
					      const enum opx_hfi1_type hfi1_type, const opx_lid_t slid)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.recv_nth_packets);

	/* Search mp-eager queue for the context w/ matching mp-eager ID */

	const uint64_t	    mp_egr_id = fi_opx_mp_egr_id_from_nth_packet(hdr, slid);
	struct opx_context *context   = (struct opx_context *) opx_ep->rx->mp_egr_queue.mq.head;
	struct opx_context *prev      = NULL;

	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(length);

	while (context && context->mp_egr_id != mp_egr_id) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "process_header_mp_eager_nth: Searching mp_egr match queue, context = %p\n", context);
		prev	= context;
		context = context->next;
		FI_OPX_DEBUG_COUNTERS_INC(length);
	}

	FI_OPX_DEBUG_COUNTERS_MAX_OF(opx_ep->debug_counters.mp_eager.recv_max_mq_queue_length, length);

	if (!context) {
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"process_header_mp_eager_nth: did not find a match .. add this packet to the unexpected queue\n");
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.recv_nth_no_match);

		fi_opx_ep_rx_append_ue_egr(opx_ep->rx, hdr, payload, payload_bytes, slid);

		return;
	}

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.recv_nth_match);

	/* We found a match!  */
	opx_ep_complete_receive_operation(ep, hdr, payload, 0, /* OFI Tag, N/A for multi-packet eager nth */
					  context,
					  opcode, // FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH
					  OPX_MULTI_RECV_FALSE, is_shm, context->flags & FI_OPX_CQ_CONTEXT_HMEM,
					  lock_required, reliability, hfi1_type);

	if (!context->byte_counter) {
		/* Remove from the mp-eager queue */
		slist_remove(&opx_ep->rx->mp_egr_queue.mq, (struct slist_entry *) context, (struct slist_entry *) prev);

		if (OFI_UNLIKELY(context->err_entry.err == FI_ETRUNC)) {
			slist_insert_tail((struct slist_entry *) context, opx_ep->rx->cq_err_ptr);
		} else {
			fi_opx_enqueue_completed(opx_ep->rx->cq_completed_ptr, context, lock_required);
		}

		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.recv_completed_eager_nth);
	}
}

static inline void fi_opx_ep_rx_process_header(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
					       const union fi_opx_hfi1_packet_payload *const payload,
					       const size_t payload_bytes, const uint64_t static_flags,
					       const uint8_t opcode, const unsigned is_shm, const int lock_required,
					       const enum ofi_reliability_kind reliability,
					       const enum opx_hfi1_type hfi1_type, const opx_lid_t slid)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (OFI_UNLIKELY(opcode < FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH)) {
		fi_opx_ep_rx_process_header_non_eager(ep, hdr, payload, payload_bytes, static_flags, opcode, is_shm,
						      lock_required, reliability, hfi1_type);
		return;
	} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) == FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST) {
		fi_opx_ep_rx_process_header_mp_eager_first(ep, hdr, payload, payload_bytes, static_flags, opcode,
							   is_shm, lock_required, reliability, hfi1_type, slid);

		return;
	} else if (opcode == FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH) {
		fi_opx_ep_rx_process_header_mp_eager_nth(ep, hdr, payload, payload_bytes, static_flags, opcode, is_shm,
							 lock_required, reliability, hfi1_type, slid);
		return;
	}

	assert(opcode >= FI_OPX_HFI_BTH_OPCODE_MSG_INJECT);

	/* search the match queue */
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "search the match queue\n");

	assert(static_flags & (FI_TAGGED | FI_MSG));
	const uint64_t	    kind    = (static_flags & FI_TAGGED) ? FI_OPX_KIND_TAG : FI_OPX_KIND_MSG;
	struct opx_context *context = (struct opx_context *) opx_ep->rx->queue[kind].mq.head;
	struct opx_context *prev    = NULL;

	while (OFI_LIKELY(context != NULL) &&
	       !is_match(opx_ep, hdr, context, opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, is_shm, slid)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "context = %p\n", context);
		prev	= context;
		context = context->next;
	}
	if (!context) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "did not find a match .. add this packet to the unexpected queue\n");

		uint64_t ue_pkt_rcv_ns;
#ifdef OPX_DEBUG_COUNTERS_RECV
		uint8_t ue_opcode = FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(hdr->bth.opcode);
		if (ue_opcode == FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) {
			OPX_COUNTERS_TIME_NS(ue_pkt_rcv_ns, &opx_ep->debug_counters);
		} else
#endif
		{
			ue_pkt_rcv_ns = 0UL;
		}

		if (OFI_LIKELY(kind == FI_OPX_KIND_TAG)) {
			fi_opx_ep_rx_append_ue_tag(opx_ep->rx, hdr, payload, payload_bytes, is_shm,
						   opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst,
						   opx_ep->daos_info.hfi_rank_enabled,
						   FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep), slid, ue_pkt_rcv_ns);
		} else {
			fi_opx_ep_rx_append_ue_msg(opx_ep->rx, hdr, payload, payload_bytes, is_shm,
						   opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst,
						   opx_ep->daos_info.hfi_rank_enabled,
						   FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep), slid, ue_pkt_rcv_ns);
		}

		return;
	}

	const uint64_t rx_op_flags = context->flags;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "found a match\n");

#ifdef OPX_DEBUG_COUNTERS_RECV
	uint8_t ue_opcode = FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(hdr->bth.opcode);
	if (ue_opcode == FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) {
		uint64_t match_time_ns;
		OPX_COUNTERS_TIME_NS(match_time_ns, &opx_ep->debug_counters);
		OPX_COUNTERS_RECORD_MEASURE(match_time_ns - context->byte_counter,
					    opx_ep->debug_counters.recv.rzv_rts_rcv_early);
	}
#endif
	if (OFI_LIKELY((static_flags & FI_TAGGED) || /* branch will compile out for tag */
		       ((rx_op_flags & FI_MULTI_RECV) == 0))) {
		assert((prev == NULL) || (prev->next == context));
		if (prev) {
			prev->next = context->next;
		} else {
			assert(opx_ep->rx->queue[kind].mq.head == (struct slist_entry *) context);
			opx_ep->rx->queue[kind].mq.head = (struct slist_entry *) context->next;
		}

		if (context->next == NULL) {
			assert(opx_ep->rx->queue[kind].mq.tail == (struct slist_entry *) context);
			opx_ep->rx->queue[kind].mq.tail = (struct slist_entry *) prev;
		}

		context->next = NULL;

		opx_ep_complete_receive_operation(ep, hdr, payload, hdr->match.ofi_tag, context, opcode,
						  OPX_MULTI_RECV_FALSE, is_shm, rx_op_flags & FI_OPX_CQ_CONTEXT_HMEM,
						  lock_required, reliability, hfi1_type);

		return;
	}

	/*
	 * verify that there is enough space available in
	 * the multi-receive buffer for the incoming data
	 */
	const uint64_t recv_len = context->len;
	const uint64_t send_len = fi_opx_hfi1_packet_hdr_message_length(hdr);

	assert(!(context->flags & FI_OPX_CQ_CONTEXT_HMEM));
	if (OFI_LIKELY(send_len <= recv_len)) {
		opx_ep_complete_receive_operation(ep, hdr, payload, 0, context, opcode, OPX_MULTI_RECV_TRUE, is_shm,
						  OPX_HMEM_FALSE, lock_required, reliability, hfi1_type);

		if (context->len < opx_ep->rx->min_multi_recv) {
			/* after processing this message there is not
			 * enough space available in the multi-receive
			 * buffer to receive the next message; post a
			 * 'FI_MULTI_RECV' event to the completion
			 * queue and return. */

			/* remove context from match queue */
			if (prev) {
				prev->next = context->next;
			} else {
				assert(opx_ep->rx->queue[kind].mq.head == (struct slist_entry *) context);
				opx_ep->rx->queue[kind].mq.head = (struct slist_entry *) context->next;
			}

			if (context->next == NULL) {
				assert(opx_ep->rx->queue[kind].mq.tail == (struct slist_entry *) context);
				opx_ep->rx->queue[kind].mq.tail = (struct slist_entry *) prev;
			}

			context->next = NULL;

			// Signaling the userneeds to be deferred until the op is completed for rendezvous
			// reusing byte counter as a pending flag
			// to ensure that any pending ops are completed (eg rendezvous multi-receive)
			if (lock_required) {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
				abort();
			}
			if (context->byte_counter == 0) {
				slist_insert_tail((struct slist_entry *) context, opx_ep->rx->cq_completed_ptr);
			}
		}
	} else {
		/*
		 * there is not enough space available in
		 * the multi-receive buffer;
		 */
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Not enough space in multi-receive buffer; abort\n");
		abort();
	}
}

#include "rdma/opx/fi_opx_fabric_progress.h"

void opx_hfi1_sdma_process_requests(struct fi_opx_ep *opx_ep);
void opx_hfi1_sdma_process_pending(struct fi_opx_ep *opx_ep);

__OPX_FORCE_INLINE__
void fi_opx_ep_do_pending_sdma_work(struct fi_opx_ep *opx_ep)
{
	/* Process pending (sent) SDMA requests to maximize free slots for new requests */
	if (!slist_empty(&opx_ep->tx->sdma_pending_queue)) {
		fi_opx_hfi1_poll_sdma_completion(opx_ep);
		opx_hfi1_sdma_process_pending(opx_ep);
	} else {
		/* If no SDMA requests were pending, then the SDMA completion
		   queue should also not have any outstanding requests */
		assert(opx_ep->hfi->info.sdma.done_index == opx_ep->hfi->info.sdma.fill_index);
	}

	struct opx_sdma_queue		*sdma_queue	= &opx_ep->tx->sdma_request_queue;
	struct slist_entry		*sdma_work_prev = NULL;
	union fi_opx_hfi1_deferred_work *sdma_work =
		(union fi_opx_hfi1_deferred_work *) opx_ep->tx->work_pending[OPX_WORK_TYPE_SDMA].head;

	uint16_t iovs_left = sdma_queue->max_iovs - sdma_queue->num_iovs;
	/* Make a single pass through the SDMA work queue, trying each work item
	   and queuing up SDMA requests. Non-TID work items require a minimum of
	   2 IOV slots, and TID work items require 3. */
	while (sdma_work && (sdma_queue->num_reqs < sdma_queue->slots_avail) && (iovs_left > 1)) {
		struct slist_entry *sdma_work_next = sdma_work->work_elem.slist_entry.next;

		if (OFI_UNLIKELY(iovs_left < 3) && sdma_work->dput.opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID) {
			sdma_work_prev = &sdma_work->work_elem.slist_entry;
			sdma_work      = (union fi_opx_hfi1_deferred_work *) sdma_work_next;
			continue;
		}

		int rc = sdma_work->work_elem.work_fn(sdma_work);
		if (rc == FI_SUCCESS) {
			if (sdma_work->work_elem.completion_action) {
				sdma_work->work_elem.completion_action(sdma_work);
			}
			if (sdma_work->work_elem.payload_copy) {
				OPX_BUF_FREE(sdma_work->work_elem.payload_copy);
			}
			slist_remove(&opx_ep->tx->work_pending[OPX_WORK_TYPE_SDMA], &sdma_work->work_elem.slist_entry,
				     sdma_work_prev);
			OPX_BUF_FREE(sdma_work);
		} else if (sdma_work->work_elem.work_type == OPX_WORK_TYPE_LAST) {
			slist_remove(&opx_ep->tx->work_pending[OPX_WORK_TYPE_SDMA], &sdma_work->work_elem.slist_entry,
				     sdma_work_prev);
			/* Move this to the pending completion queue,
				since there's nothing left to do but wait */
			slist_insert_tail(&sdma_work->work_elem.slist_entry, &opx_ep->tx->work_pending_completion);
		} else {
			sdma_work_prev = &sdma_work->work_elem.slist_entry;
		}
		sdma_work = (union fi_opx_hfi1_deferred_work *) sdma_work_next;
		iovs_left = sdma_queue->max_iovs - sdma_queue->num_iovs;
	}

	/* Process new SDMA requests to egress */
	if (!slist_empty(&opx_ep->tx->sdma_request_queue.list)) {
		opx_hfi1_sdma_process_requests(opx_ep);
	}
}

__OPX_FORCE_INLINE__
void fi_opx_ep_do_pending_work(struct fi_opx_ep *opx_ep)
{
	/* Clean up all the pending completion work, but stop as soon as we
	   encounter one that isn't done (and requeue that one) */
	uintptr_t work_pending_completion = (uintptr_t) opx_ep->tx->work_pending_completion.head;
	while (work_pending_completion) {
		union fi_opx_hfi1_deferred_work *work =
			(union fi_opx_hfi1_deferred_work *) slist_remove_head(&opx_ep->tx->work_pending_completion);
		work->work_elem.slist_entry.next = NULL;
		int rc				 = work->work_elem.work_fn(work);
		if (rc == FI_SUCCESS) {
			if (work->work_elem.completion_action) {
				work->work_elem.completion_action(work);
			}
			if (work->work_elem.payload_copy) {
				OPX_BUF_FREE(work->work_elem.payload_copy);
			}
			OPX_BUF_FREE(work);
			work_pending_completion = (uintptr_t) opx_ep->tx->work_pending_completion.head;
		} else {
			assert(work->work_elem.slist_entry.next == NULL);
			slist_insert_head(&work->work_elem.slist_entry, &opx_ep->tx->work_pending_completion);
			work_pending_completion = 0;
		}
	}

	/* Note that SDMA work is not included in this loop, as it is done separately */
	for (enum opx_work_type work_type = OPX_WORK_TYPE_PIO; work_type < OPX_WORK_TYPE_LAST; ++work_type) {
		const uintptr_t work_pending = (const uintptr_t) opx_ep->tx->work_pending[work_type].head;
		if (work_pending) {
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "DO-DEFERRED-WORK-%s", OPX_WORK_TYPE_STR[work_type]);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== POLL WORK PENDING type %d <start \n",
				     work_type);
			union fi_opx_hfi1_deferred_work *work = (union fi_opx_hfi1_deferred_work *) slist_remove_head(
				&opx_ep->tx->work_pending[work_type]);
			work->work_elem.slist_entry.next = NULL;
			int rc				 = work->work_elem.work_fn(work);
			if (rc == FI_SUCCESS) {
				if (work->work_elem.completion_action) {
					work->work_elem.completion_action(work);
				}
				if (work->work_elem.payload_copy) {
					OPX_BUF_FREE(work->work_elem.payload_copy);
				}
				OPX_BUF_FREE(work);
			} else {
				assert(work->work_elem.slist_entry.next == NULL);
				if (work->work_elem.work_type == OPX_WORK_TYPE_LAST) {
					/* Move this to the pending completion queue,
					   since there's nothing left to do but wait */
					slist_insert_tail(&work->work_elem.slist_entry,
							  &opx_ep->tx->work_pending_completion);
				} else {
					slist_insert_head(&work->work_elem.slist_entry,
							  &opx_ep->tx->work_pending[work->work_elem.work_type]);
				}
			}
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "DO-DEFERRED-WORK-%s", OPX_WORK_TYPE_STR[work_type]);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== POLL WORK PENDING type %d %u done>\n",
				     work_type, rc);
		}
	}

	fi_opx_ep_do_pending_sdma_work(opx_ep);
}

#if HAVE_HFISVC
__OPX_FORCE_INLINE__
void opx_ep_hfisvc_poll_proc_internal_completion(struct fi_opx_ep *opx_ep, struct hfisvc_client_cq_entry *hfisvc_entry)
{
	struct fi_opx_rzv_completion *rzv_comp = (struct fi_opx_rzv_completion *) hfisvc_entry->app_context;
	assert(rzv_comp);
	struct opx_context *context = rzv_comp->context;
	if (OFI_UNLIKELY(hfisvc_entry->status != HFISVC_CLIENT_CQ_ENTRY_STATUS_SUCCESS)) {
		OPX_HFISVC_DEBUG_LOG(
			"Error: HFISVC CQ completion status is %d for access_key=%u context=%p (was expecting 0/success)\n",
			hfisvc_entry->status, rzv_comp->access_key, rzv_comp->context);
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.internal_completion.error);
		if (context) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Enqueuing completion error from HFI Service with status %d\n", hfisvc_entry->status);
			context->err_entry.flags	 = context->flags;
			context->err_entry.len		 = context->byte_counter;
			context->err_entry.buf		 = context->buf;
			context->err_entry.data		 = context->data;
			context->err_entry.tag		 = context->tag;
			context->err_entry.olen		 = 0;
			context->err_entry.err		 = hfisvc_entry->status;
			context->err_entry.prov_errno	 = hfisvc_entry->status;
			context->err_entry.err_data	 = NULL;
			context->err_entry.err_data_size = 0;
			context->byte_counter		 = 0;
			opx_enqueue_err_from_pending(opx_ep->tx->cq_pending_ptr, opx_ep->tx->cq_err_ptr, context);
		}
	} else {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.internal_completion.success);
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.completed);
		// TODO: Use the completion's xfer_len when that becomes available
		// const uint64_t completion_len = hfisvc_out[i].xfer_len;
		uint64_t completion_len = 0;

		if (context) {
			completion_len = rzv_comp->context->byte_counter;
			OPX_HFISVC_DEBUG_LOG(
				"HFISVC Internal completion with context: rzv_comp=%p access_key=%u xfer_len=%lu context=%p byte_counter=%lu -> %lu\n",
				rzv_comp, rzv_comp->access_key, completion_len, rzv_comp->context,
				rzv_comp->context->byte_counter, rzv_comp->context->byte_counter - completion_len);

			assert(completion_len <= rzv_comp->context->byte_counter);
			rzv_comp->context->byte_counter -= completion_len;
		} else {
			OPX_HFISVC_DEBUG_LOG(
				"HFISVC Internal completion without context: rzv_comp=%p access_key=%u xfer_len=%lu\n",
				rzv_comp, rzv_comp->access_key, completion_len);
		}
	}

	opx_hfisvc_keyset_free_key(opx_ep->domain->hfisvc.access_key_set, rzv_comp->access_key,
				   FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep));
	OPX_BUF_FREE(rzv_comp);
}
#endif

__OPX_FORCE_INLINE__
void opx_ep_hfisvc_poll_internal_queue(struct fi_opx_ep *opx_ep)
{
#if HAVE_HFISVC
	if (!opx_ep->use_hfisvc) {
		return;
	}

	struct hfisvc_client_cq_entry hfisvc_out[64];

	size_t n = (*opx_ep->domain->hfisvc.cq_read)(opx_ep->hfisvc.internal_completion_queue, 0ul /* flags */,
						     hfisvc_out, sizeof(*hfisvc_out), 64);
	while (n > 0) {
		for (size_t i = 0; i < n; ++i) {
			opx_ep_hfisvc_poll_proc_internal_completion(opx_ep, &hfisvc_out[i]);
		}
		n = (*opx_ep->domain->hfisvc.cq_read)(opx_ep->hfisvc.internal_completion_queue, 0ul /* flags */,
						      hfisvc_out, sizeof(*hfisvc_out), 64);
	}
#endif
}

__OPX_FORCE_INLINE__
void fi_opx_ep_rx_poll_internal(struct fid_ep *ep, const uint64_t caps, const enum ofi_reliability_kind reliability,
				const uint64_t hdrq_mask, const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	const uint64_t rx_caps =
		(caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) ? caps : opx_ep->rx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);

	if (OFI_LIKELY(hdrq_mask == FI_OPX_HDRQ_MASK_RUNTIME)) { /* constant compile-time expression */
		FI_OPX_FABRIC_POLL_MANY(ep, FI_OPX_LOCK_NOT_REQUIRED, rx_caps, OFI_RELIABILITY_KIND_ONLOAD,
					FI_OPX_HDRQ_MASK_RUNTIME, hfi1_type, ctx_sharing);
	} else if (hdrq_mask == FI_OPX_HDRQ_MASK_2048) { /* constant compile-time expression */
		FI_OPX_FABRIC_POLL_MANY(ep, FI_OPX_LOCK_NOT_REQUIRED, rx_caps, OFI_RELIABILITY_KIND_ONLOAD,
					FI_OPX_HDRQ_MASK_2048, hfi1_type, ctx_sharing);
	} else if (hdrq_mask == FI_OPX_HDRQ_MASK_8192) { /* constant compile-time expression */
		FI_OPX_FABRIC_POLL_MANY(ep, FI_OPX_LOCK_NOT_REQUIRED, rx_caps, OFI_RELIABILITY_KIND_ONLOAD,
					FI_OPX_HDRQ_MASK_8192, hfi1_type, ctx_sharing);
	} else {
		FI_OPX_FABRIC_POLL_MANY(ep, FI_OPX_LOCK_NOT_REQUIRED, rx_caps, OFI_RELIABILITY_KIND_ONLOAD, hdrq_mask,
					hfi1_type, ctx_sharing);
	}

	opx_ep_hfisvc_poll_internal_queue(opx_ep);

	fi_opx_ep_do_pending_work(opx_ep);

	if (!slist_empty(&opx_ep->reli_service->rx.work_pending)) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== RELIABILITY SERVICE WORK PENDING\n");
		fi_opx_reliability_service_process_pending(opx_ep->reli_service);
	}
}

static inline void fi_opx_ep_rx_poll(struct fid_ep *ep, const uint64_t caps,
				     const enum ofi_reliability_kind reliability, const uint64_t hdrq_mask,
				     const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	if (ctx_sharing) {
		if (hfi1_type & OPX_HFI1_WFR) {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_WFR, OPX_CTX_SHARING_ON);
		} else if (hfi1_type & OPX_HFI1_JKR) {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_JKR, OPX_CTX_SHARING_ON);
		} else if (hfi1_type & OPX_HFI1_CYR) {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_CYR, OPX_CTX_SHARING_ON);
		} else {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_MIXED_9B,
						   OPX_CTX_SHARING_ON);
		}
	} else {
		if (hfi1_type & OPX_HFI1_WFR) {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_WFR, OPX_CTX_SHARING_OFF);
		} else if (hfi1_type & OPX_HFI1_JKR) {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_JKR, OPX_CTX_SHARING_OFF);
		} else if (hfi1_type & OPX_HFI1_CYR) {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_CYR, OPX_CTX_SHARING_OFF);
		} else {
			fi_opx_ep_rx_poll_internal(ep, caps, reliability, hdrq_mask, OPX_HFI1_MIXED_9B,
						   OPX_CTX_SHARING_OFF);
		}
	}
}

__OPX_FORCE_INLINE__
int fi_opx_ep_cancel_context(struct fi_opx_ep *opx_ep, const uint64_t cancel_context, struct opx_context *context,
			     const uint64_t rx_op_flags, const int lock_required)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "unimplemented; abort\n");
	abort();

	const uint64_t compare_context = (uint64_t) context->err_entry.op_context;

	if (compare_context == cancel_context) {
		context->byte_counter		 = 0;
		context->err_entry.flags	 = rx_op_flags;
		context->err_entry.len		 = 0;
		context->err_entry.buf		 = 0;
		context->err_entry.data		 = 0;
		context->err_entry.tag		 = context->tag;
		context->err_entry.olen		 = 0;
		context->err_entry.err		 = FI_ECANCELED;
		context->err_entry.prov_errno	 = 0;
		context->err_entry.err_data	 = NULL;
		context->err_entry.err_data_size = 0;

		/* post an 'error' completion event for the canceled receive */
		if (lock_required) {
			fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
			abort();
		}
		slist_insert_tail((struct slist_entry *) context, opx_ep->rx->cq_err_ptr);

		return FI_ECANCELED;
	}

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
int fi_opx_ep_process_context_match_ue_packets(struct fi_opx_ep *opx_ep, const uint64_t static_flags,
					       struct opx_context *context, const uint64_t is_hmem,
					       const int lock_required, const enum ofi_reliability_kind reliability,
					       const enum opx_hfi1_type hfi1_type)
{
	assert(static_flags & (FI_TAGGED | FI_MSG));
	const uint64_t kind = (static_flags & FI_TAGGED) ? FI_OPX_KIND_TAG : FI_OPX_KIND_MSG;

	struct fid_ep *ep = &opx_ep->ep_fid;
	/*
	 * search the unexpected packet queue
	 */
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "searching unexpected queue\n");

	__attribute__((__unused__)) bool from_hash_queue = false;
	struct fi_opx_hfi1_ue_packet	*uepkt = fi_opx_ep_find_matching_packet(opx_ep, context, kind, hfi1_type);

#ifndef FI_OPX_MATCH_HASH_DISABLE
	if (!uepkt && kind == FI_OPX_KIND_TAG) {
		from_hash_queue = true;
		uepkt		= fi_opx_match_find_uepkt(opx_ep->rx->match_ue_tag_hash, context,
							  FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep));
	}
#endif

	if (uepkt) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "UEPKT found a match, uepkt = %p\n", uepkt);

		uint8_t is_mp_eager = (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(uepkt->hdr.bth.opcode) ==
				       FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST);

#ifdef OPX_DEBUG_COUNTERS_RECV
		uint8_t ue_opcode = FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(uepkt->hdr.bth.opcode);
		if (ue_opcode == FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) {
			uint64_t match_time_ns;
			OPX_COUNTERS_TIME_NS(match_time_ns, &opx_ep->debug_counters);
			OPX_COUNTERS_RECORD_MEASURE(match_time_ns - uepkt->recv_time_ns,
						    opx_ep->debug_counters.recv.rzv_rts_pkt_early);
		}
#endif
		const unsigned is_shm = uepkt->is_shm;
		if (is_mp_eager) {
			opx_ep_complete_receive_operation(ep, &uepkt->hdr, &uepkt->payload, uepkt->hdr.match.ofi_tag,
							  context, uepkt->hdr.bth.opcode, OPX_MULTI_RECV_FALSE, is_shm,
							  is_hmem, lock_required, reliability, hfi1_type);

			/* Since this is the first multi-packet eager packet,
			   the uid portion of the mp_egr_id will be this packet's PSN */
			opx_lid_t slid;
			if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
				slid = (opx_lid_t) __be16_to_cpu24((__be16) uepkt->hdr.lrh_9B.slid);
			} else {
				slid = (opx_lid_t) __le24_to_cpu((__le24) (uepkt->hdr.lrh_16B.slid20 << 20) |
								 (uepkt->hdr.lrh_16B.slid));
			}
			const uint64_t mp_egr_id =
				OPX_GET_MP_EGR_ID(uepkt->hdr.reliability.psn, slid, uepkt->subctxt_rx);

			fi_opx_ep_rx_process_pending_mp_eager_ue(ep, context, mp_egr_id, is_shm, lock_required,
								 reliability, hfi1_type);

			if (context->byte_counter) {
				context->mp_egr_id = mp_egr_id;
				slist_insert_tail((struct slist_entry *) context, &opx_ep->rx->mp_egr_queue.mq);
			} else {
				FI_OPX_DEBUG_COUNTERS_INC(
					opx_ep->debug_counters.mp_eager.recv_completed_process_context);

				context->next = NULL;
				if (OFI_UNLIKELY(context->err_entry.err == FI_ETRUNC)) {
					slist_insert_tail((struct slist_entry *) context, opx_ep->rx->cq_err_ptr);
				} else {
					fi_opx_enqueue_completed(opx_ep->rx->cq_completed_ptr, context, lock_required);
				}
			}
		} else {
			opx_ep_complete_receive_operation(ep, &uepkt->hdr, &uepkt->payload, uepkt->hdr.match.ofi_tag,
							  context, uepkt->hdr.bth.opcode, OPX_MULTI_RECV_FALSE, is_shm,
							  is_hmem, lock_required, reliability, hfi1_type);
		}

#ifndef FI_OPX_MATCH_HASH_DISABLE
		if (from_hash_queue) {
			fi_opx_match_ue_hash_remove(uepkt, opx_ep->rx->match_ue_tag_hash);
		} else {
			fi_opx_hfi1_ue_packet_slist_remove_item(uepkt, &opx_ep->rx->queue[kind].ue);
		}
#else
		fi_opx_hfi1_ue_packet_slist_remove_item(uepkt, &opx_ep->rx->queue[kind].ue);
#endif

		return 0;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "nothing found on unexpected queue; adding to match queue\n");
	/*
	 * no unexpected headers were matched; add this match information
	 * (context) to the appropriate match queue
	 */

	// Use the context's byte_counter to store the timestamp of when the recv
	// was posted, so we can later record how long it took for the receive to
	// be matched with a packet. This should be safe since byte_counter is not
	// used elsewhere until after the match occurs.
	OPX_COUNTERS_TIME_NS(context->byte_counter, &opx_ep->debug_counters);

	context->next = NULL;
	slist_insert_tail((struct slist_entry *) context, &opx_ep->rx->queue[kind].mq);

	return 0;
}

/* rx_op_flags is only checked for FI_PEEK | FI_CLAIM | FI_MULTI_RECV
 * rx_op_flags is only used if FI_PEEK | FI_CLAIM
 *
 * The "normal" data movement functions, such as fi_[t]recv(), can safely
 * specify '0' for rx_op_flags in order to reduce code path.
 *
 * TODO - use payload pointer? keep data in hfi eager buffer as long
 * as possible to avoid memcpy?
 */
__OPX_FORCE_INLINE__
int fi_opx_ep_rx_process_context(struct fi_opx_ep *opx_ep, const uint64_t static_flags, struct opx_context *context,
				 const uint64_t rx_op_flags, const uint64_t is_hmem, const int lock_required,
				 const enum fi_av_type av_type, const enum ofi_reliability_kind reliability,
				 const enum opx_hfi1_type hfi1_type)
{
#if HAVE_HFISVC
	uint32_t start_rdma_read_count = opx_ep->hfisvc.rdma_read_count;
#endif
	int rc;

	if (OFI_LIKELY((rx_op_flags & (FI_PEEK | FI_CLAIM | FI_MULTI_RECV)) == 0)) {
		if (is_hmem) { /* branch should compile out */
			rc = fi_opx_ep_process_context_match_ue_packets(opx_ep, static_flags, context, OPX_HMEM_TRUE,
									lock_required, reliability, hfi1_type);
		} else {
			rc = fi_opx_ep_process_context_match_ue_packets(opx_ep, static_flags, context, OPX_HMEM_FALSE,
									lock_required, reliability, hfi1_type);
		}
	} else {
		/*
		 * Not for critical path: peek, or claim, or multi-receive
		 * context information
		 */
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "process peek, claim, or multi-receive context\n");

		fi_opx_ep_rx_process_context_noinline(opx_ep, static_flags, context, rx_op_flags, is_hmem,
						      lock_required, av_type, reliability, hfi1_type);
		rc = 0;
	}

#if HAVE_HFISVC
	if (opx_ep->hfisvc.rdma_read_count != start_rdma_read_count) {
		OPX_HFISVC_DEBUG_LOG(
			"Rang doorbell because start_rdma_read_count=%u and opx_ep->hfisvc.rdma_read_count=%u\n",
			start_rdma_read_count, opx_ep->hfisvc.rdma_read_count);
		int doorbell_rc __attribute__((unused));
		doorbell_rc = (*opx_ep->domain->hfisvc.doorbell)(opx_ep->domain->hfisvc.ctx);

		assert(doorbell_rc == 0);
	}
#endif

	return rc;
}

__OPX_FORCE_INLINE__
fi_addr_t fi_opx_ep_get_src_addr(struct fi_opx_ep *opx_ep, const enum fi_av_type av_type, const fi_addr_t msg_addr)
{
	if (av_type == FI_AV_MAP) { /* constant compile-time expression */
		return msg_addr;
	}

	if (av_type == FI_AV_TABLE) {
		return OFI_LIKELY(msg_addr != FI_ADDR_UNSPEC) ? opx_ep->rx->av_addr[msg_addr].fi : FI_ADDR_UNSPEC;
	}

	assert(av_type == FI_AV_UNSPEC);

	if (opx_ep->av_type != FI_AV_TABLE) {
		return msg_addr;
	}

	/* use runtime endpoint value*/
	return OFI_LIKELY(msg_addr != FI_ADDR_UNSPEC) ? opx_ep->rx->av_addr[msg_addr].fi : FI_ADDR_UNSPEC;
}

/*
 * =========================== Application-facing ===========================
 */

__OPX_FORCE_INLINE__
ssize_t fi_opx_ep_rx_recv_internal(struct fi_opx_ep *opx_ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,
				   uint64_t tag, uint64_t ignore, void *user_context, const int lock_required,
				   const enum fi_av_type av_type, const uint64_t static_flags,
				   const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	assert(((static_flags & (FI_TAGGED | FI_MSG)) == FI_TAGGED) ||
	       ((static_flags & (FI_TAGGED | FI_MSG)) == FI_MSG));

	assert(IS_PROGRESS_MANUAL(opx_ep->domain));

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	FI_OPX_DEBUG_COUNTERS_INC_COND(static_flags & FI_MSG, opx_ep->debug_counters.recv.posted_recv_msg);
	FI_OPX_DEBUG_COUNTERS_INC_COND(static_flags & FI_TAGGED, opx_ep->debug_counters.recv.posted_recv_tag);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== POST RECV: context = %p\n", user_context);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "POST-RECV");

	struct opx_context *context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
	if (OFI_UNLIKELY(context == NULL)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		return -FI_ENOMEM;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "process context (check unexpected queue, append match queue)\n");

	const uint64_t rx_op_flags = opx_ep->rx->op_flags;
	uint64_t       rx_caps	   = opx_ep->rx->caps;

	context->next		      = NULL;
	context->err_entry.err	      = 0;
	context->err_entry.op_context = user_context;
	context->flags		      = rx_op_flags;
	context->len		      = len;
	context->buf		      = buf;
	context->src_addr =
		(rx_caps & FI_DIRECTED_RECV) ? fi_opx_ep_get_src_addr(opx_ep, av_type, src_addr) : FI_ADDR_UNSPEC;
	context->tag	      = tag;
	context->ignore	      = ignore;
	context->byte_counter = (uint64_t) -1;

	struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) &context->hmem_info_qws[0];
	hmem_info->is_unified		   = desc ? ((struct fi_opx_mr *) desc)->hmem_unified : 0;

#ifdef OPX_HMEM
	uint64_t	   hmem_device;
	uint64_t	   hmem_handle;
	enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(desc, &hmem_device, &hmem_handle);
	if (hmem_iface != FI_HMEM_SYSTEM) {
		FI_OPX_DEBUG_COUNTERS_INC_COND(static_flags & FI_MSG, opx_ep->debug_counters.hmem.posted_recv_msg);
		FI_OPX_DEBUG_COUNTERS_INC_COND(static_flags & FI_TAGGED, opx_ep->debug_counters.hmem.posted_recv_tag);
		hmem_info->iface	       = hmem_iface;
		hmem_info->device	       = hmem_device;
		hmem_info->hmem_dev_reg_handle = hmem_handle;

		context->flags |= FI_OPX_CQ_CONTEXT_HMEM;

		fi_opx_ep_rx_process_context(opx_ep, static_flags, context,
					     0, // rx_op_flags
					     OPX_HMEM_TRUE, lock_required, av_type, reliability, hfi1_type);
	} else
#endif
	{
		hmem_info->iface	       = FI_HMEM_SYSTEM;
		hmem_info->device	       = 0UL;
		hmem_info->hmem_dev_reg_handle = OPX_HMEM_NO_HANDLE;
		fi_opx_ep_rx_process_context(opx_ep, static_flags, context,
					     0, // rx_op_flags
					     OPX_HMEM_FALSE, lock_required, av_type, reliability, hfi1_type);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "POST-RECV");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== POST RECV RETURN\n");

	return 0;
}

/*
 * \note The opx provider asserts the following mode bits which affect
 * 	the behavior of this routine:
 *
 * 	- 'FI_ASYNC_IOV' mode bit which requires the application to maintain
 * 	  the 'msg->msg_iov' iovec array until the operation completes
 *
 * 	- 'FI_LOCAL_MR' mode bit which allows the provider to ignore the 'desc'
 * 	  parameter .. no memory regions are required to access the local
 * 	  memory
 */
static inline ssize_t fi_opx_ep_rx_recvmsg_internal(struct fi_opx_ep *opx_ep, const struct fi_msg *msg, uint64_t flags,
						    const int lock_required, const enum fi_av_type av_type,
						    const enum ofi_reliability_kind reliability,
						    const enum opx_hfi1_type	    hfi1_type)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== POST RECVMSG\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "POST-RECVMSG");
	FI_OPX_DEBUG_COUNTERS_INC_COND(!(flags & FI_MULTI_RECV), opx_ep->debug_counters.recv.posted_recv_msg);
	FI_OPX_DEBUG_COUNTERS_INC_COND((flags & FI_MULTI_RECV), opx_ep->debug_counters.recv.posted_multi_recv);
	assert(!lock_required);

	struct opx_context *context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
	if (OFI_UNLIKELY(context == NULL)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "POST-RECVMSG");
		return -FI_ENOMEM;
	}
	context->next		      = NULL;
	context->err_entry.err	      = 0;
	context->err_entry.op_context = msg->context;

	if (OFI_LIKELY(flags & FI_MULTI_RECV)) {
		uint64_t len  = msg->msg_iov[0].iov_len;
		void	*base = msg->msg_iov[0].iov_base;

		assert(msg->iov_count == 1);
		assert(base != NULL);
		if ((uintptr_t) base & 0x07ull) {
			uintptr_t new_base = (((uintptr_t) base + 8) & (~0x07ull));
			len -= (new_base - (uintptr_t) base);
			base = (void *) new_base;
		}
		assert(((uintptr_t) base & 0x07ull) == 0);
		assert(len >= (sizeof(struct opx_context) + opx_ep->rx->min_multi_recv));
		context->flags		  = FI_MULTI_RECV;
		context->len		  = len - sizeof(struct opx_context);
		context->buf		  = (void *) ((uintptr_t) base + sizeof(struct opx_context));
		context->src_addr	  = fi_opx_ep_get_src_addr(opx_ep, av_type, msg->addr);
		context->byte_counter	  = 0;
		context->ignore		  = (uint64_t) -1;
		context->hmem_info_qws[0] = 0ul;
		context->hmem_info_qws[1] = 0ul;
		context->hmem_info_qws[2] = 0ul;

		ssize_t rc = fi_opx_ep_rx_process_context(opx_ep, FI_MSG, context, flags, OPX_HMEM_FALSE, lock_required,
							  av_type, reliability, hfi1_type);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== POST RECVMSG RETURN\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "POST-RECVMSG");
		return rc;

	} else if (msg->iov_count == 0) {
		context->flags		  = flags;
		context->len		  = 0;
		context->buf		  = NULL;
		context->src_addr	  = fi_opx_ep_get_src_addr(opx_ep, av_type, msg->addr);
		context->tag		  = 0;
		context->ignore		  = (uint64_t) -1;
		context->byte_counter	  = (uint64_t) -1;
		context->hmem_info_qws[0] = 0ul;
		context->hmem_info_qws[1] = 0ul;
		context->hmem_info_qws[2] = 0ul;

		ssize_t rc = fi_opx_ep_rx_process_context(opx_ep, FI_MSG, context, flags, OPX_HMEM_FALSE, lock_required,
							  av_type, reliability, hfi1_type);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== POST RECVMSG RETURN\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "POST-RECVMSG");
		return rc;
	}

#ifdef OPX_HMEM
	/* NOTE: Assume that all IOVs reside in the same HMEM space */
	struct fi_opx_hmem_info *hmem_info = (struct fi_opx_hmem_info *) &context->hmem_info_qws[0];
	uint64_t		 hmem_device;
	enum fi_hmem_iface	 hmem_iface;
	uint64_t		 hmem_handle;
	if (msg->desc && msg->desc[0]) {
		hmem_iface		       = opx_hmem_get_mr_iface(msg->desc[0], &hmem_device, &hmem_handle);
		hmem_info->iface	       = hmem_iface;
		hmem_info->device	       = hmem_device;
		hmem_info->hmem_dev_reg_handle = hmem_handle;
		hmem_info->is_unified	       = ((struct fi_opx_mr *) msg->desc[0])->hmem_unified;
	} else {
		hmem_iface		  = FI_HMEM_SYSTEM;
		hmem_device		  = 0UL;
		context->hmem_info_qws[0] = 0;
		context->hmem_info_qws[1] = 0;
		context->hmem_info_qws[2] = 0;
	}

#ifndef NDEBUG
	if (msg->iov_count > 1) {
		for (int i = 1; i < msg->iov_count; ++i) {
			uint64_t	   tmp_hmem_device;
			uint64_t	   hmem_handle;
			enum fi_hmem_iface tmp_hmem_iface =
				opx_hmem_get_mr_iface(msg->desc ? msg->desc[i] : NULL, &tmp_hmem_device, &hmem_handle);
			assert(tmp_hmem_iface == hmem_iface);
			assert(tmp_hmem_device == hmem_device);
		}
	}
#endif
	if (hmem_iface != FI_HMEM_SYSTEM) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hmem.posted_recv_msg);
		context->flags	       = flags | FI_OPX_CQ_CONTEXT_HMEM;
		context->len	       = msg->msg_iov[0].iov_len;
		context->buf	       = msg->msg_iov[0].iov_base;
		context->byte_counter  = (uint64_t) -1;
		context->src_addr      = fi_opx_ep_get_src_addr(opx_ep, av_type, msg->addr);
		context->tag	       = 0;
		context->ignore	       = (uint64_t) -1;
		context->msg.iov_count = msg->iov_count;
		context->msg.iov       = (struct iovec *) msg->msg_iov;

		ssize_t rc = fi_opx_ep_rx_process_context(opx_ep, FI_MSG, context, context->flags, OPX_HMEM_TRUE,
							  lock_required, av_type, reliability, hfi1_type);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== POST RECVMSG (HMEM) RETURN\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "POST-RECVMSG");
		return rc;
	}
#endif

	if (msg->iov_count == 1) {
		context->flags	      = flags;
		context->len	      = msg->msg_iov[0].iov_len;
		context->buf	      = msg->msg_iov[0].iov_base;
		context->src_addr     = fi_opx_ep_get_src_addr(opx_ep, av_type, msg->addr);
		context->tag	      = 0;
		context->ignore	      = (uint64_t) -1;
		context->byte_counter = (uint64_t) -1;

		ssize_t rc = fi_opx_ep_rx_process_context(opx_ep, FI_MSG, context, flags, OPX_HMEM_FALSE, lock_required,
							  av_type, reliability, hfi1_type);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== POST RECVMSG RETURN\n");
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "POST-RECVMSG");
		return rc;
	}

	/* msg->iov_count > 1 */

	context->flags	       = flags;
	context->byte_counter  = (uint64_t) -1;
	context->src_addr      = fi_opx_ep_get_src_addr(opx_ep, av_type, msg->addr);
	context->tag	       = 0;
	context->ignore	       = (uint64_t) -1;
	context->msg.iov_count = msg->iov_count;
	context->msg.iov       = (struct iovec *) msg->msg_iov;

	ssize_t rc = fi_opx_ep_rx_process_context(opx_ep, FI_MSG, context, flags, OPX_HMEM_FALSE, lock_required,
						  av_type, reliability, hfi1_type);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== POST RECVMSG RETURN\n");
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "POST-RECVMSG");
	return rc;
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_ep_tx_do_cq_completion(struct fi_opx_ep *opx_ep, const unsigned override_flags,
				       const uint64_t tx_op_flags)
{
	/*
	 * ==== NOTE_SELECTIVE_COMPLETION ====
	 *
	 * FI_SELECTIVE_COMPLETION essentially changes the default from:
	 *
	 *   "generate a completion of some kind if FI_TRANSMIT is
	 *   also specified"
	 *
	 * to
	 *
	 *   "only generate a completion of some kind if FI_TRANSMIT
	 *   and FI_COMPLETION are also specified".
	 *
	 * and as specified in commit 8bf9bf74b719f265186a7dea1c1e1f26a24bfb5a:
	 *
	 *   "FI_COMPLETION is only needed in cases where an endpoint was
	 *   bound to a CQ or counter with the FI_SELECTIVE_COMPLETION flag."
	 */

	if (!override_flags) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== DO TX CQ COMPLETION %u (tx)\n",
			     opx_ep->tx->do_cq_completion);
		return opx_ep->tx->do_cq_completion;
	}

	const uint64_t selective_completion = FI_SELECTIVE_COMPLETION | FI_TRANSMIT | FI_COMPLETION;

	const uint64_t flags = tx_op_flags | opx_ep->tx->cq_bind_flags;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== DO TX CQ COMPLETION %u (flags)\n",
		     ((flags & selective_completion) == selective_completion) ||
			     ((flags & (FI_SELECTIVE_COMPLETION | FI_TRANSMIT)) == FI_TRANSMIT));
	return ((flags & selective_completion) == selective_completion) ||
	       ((flags & (FI_SELECTIVE_COMPLETION | FI_TRANSMIT)) == FI_TRANSMIT);
}

__OPX_FORCE_INLINE__
void fi_opx_ep_tx_cq_completion_rzv(struct fid_ep *ep, void *context, const size_t len, const int lock_required,
				    const uint64_t tag, const uint64_t caps)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	assert(context);
	assert(((uintptr_t) context & 0x07ull) == 0); /* must be 8 byte aligned */
	struct opx_context *opx_context = (struct opx_context *) context;
	opx_context->flags		= FI_SEND | (caps & (FI_TAGGED | FI_MSG));
	opx_context->len		= len;
	opx_context->buf		= NULL; /* receive data buffer */
	opx_context->tag		= tag;
	opx_context->next		= NULL;

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

	OPX_HFISVC_DEBUG_LOG(
		"cq_pending context=%p context->err_entry.op_context=%p context->tag=%016lX context->data=%016lX context->len=%lu\n",
		opx_context, opx_context->err_entry.op_context, opx_context->tag, opx_context->data, opx_context->len);

	slist_insert_tail((struct slist_entry *) opx_context, opx_ep->tx->cq_pending_ptr);
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_mp_egr_remaining(struct fi_opx_ep *opx_ep, uint8_t **buf_ptr, uint32_t *payload_offset,
					  ssize_t *payload_remaining, const uint32_t mp_egr_id, const size_t chunk_size,
					  const uint64_t pbc_dlid, const uint64_t bth_rx, const uint64_t lrh_dlid,
					  const union fi_opx_addr addr, const enum ofi_reliability_kind reliability,
					  const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	ssize_t	 remaining = *payload_remaining;
	uint32_t offset	   = *payload_offset;
	uint8_t *buf	   = *buf_ptr;
	ssize_t	 rc;

	while (remaining >= chunk_size) {
		if (hfi1_type == OPX_HFI1_WFR) {
			rc = fi_opx_hfi1_tx_send_mp_egr_nth(opx_ep, (void *) buf, offset, chunk_size, mp_egr_id,
							    pbc_dlid, bth_rx, lrh_dlid, addr, FI_OPX_LOCK_NOT_REQUIRED,
							    reliability, OPX_HFI1_WFR, ctx_sharing);
		} else if (hfi1_type == OPX_HFI1_MIXED_9B) {
			rc = fi_opx_hfi1_tx_send_mp_egr_nth(opx_ep, (void *) buf, offset, chunk_size, mp_egr_id,
							    pbc_dlid, bth_rx, lrh_dlid, addr, FI_OPX_LOCK_NOT_REQUIRED,
							    reliability, OPX_HFI1_MIXED_9B, ctx_sharing);
		} else {
			rc = fi_opx_hfi1_tx_send_mp_egr_nth_16B(
				opx_ep, (void *) buf, offset, chunk_size, mp_egr_id, pbc_dlid, bth_rx, lrh_dlid, addr,
				FI_OPX_LOCK_NOT_REQUIRED, reliability, OPX_HFI1_JKR, ctx_sharing);
		}

		if (rc != FI_SUCCESS) {
			goto mp_egr_eagain;
		}
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.send_nth_packets);

		remaining -= chunk_size;
		offset += chunk_size;
		buf += chunk_size;
	}

	if (remaining > 0) {
		if (hfi1_type == OPX_HFI1_WFR) {
			rc = fi_opx_hfi1_tx_send_mp_egr_nth(opx_ep, (void *) buf, offset, remaining, mp_egr_id,
							    pbc_dlid, bth_rx, lrh_dlid, addr, FI_OPX_LOCK_NOT_REQUIRED,
							    reliability, OPX_HFI1_WFR, ctx_sharing);
		} else if (hfi1_type == OPX_HFI1_MIXED_9B) {
			rc = fi_opx_hfi1_tx_send_mp_egr_nth(opx_ep, (void *) buf, offset, remaining, mp_egr_id,
							    pbc_dlid, bth_rx, lrh_dlid, addr, FI_OPX_LOCK_NOT_REQUIRED,
							    reliability, OPX_HFI1_MIXED_9B, ctx_sharing);
		} else {
			rc = fi_opx_hfi1_tx_send_mp_egr_nth_16B(
				opx_ep, (void *) buf, offset, remaining, mp_egr_id, pbc_dlid, bth_rx, lrh_dlid, addr,
				FI_OPX_LOCK_NOT_REQUIRED, reliability, OPX_HFI1_JKR, ctx_sharing);
		}
		if (rc != FI_SUCCESS) {
			goto mp_egr_eagain;
		}
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.send_nth_packets);
	}

	return FI_SUCCESS;

mp_egr_eagain:

	(*payload_remaining) = remaining;
	(*buf_ptr)	     = buf;
	(*payload_offset)    = offset;

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_try_mp_egr(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				    void *context, const uint32_t data, int lock_required,
				    const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t caps,
				    const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
				    const enum fi_hmem_iface hmem_iface, const uint64_t hmem_device,
				    const uint64_t hmem_handle, const enum opx_hfi1_type hfi1_type,
				    const bool ctx_sharing)
{
	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	assert(!fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps));
	assert(len > opx_ep->tx->mp_eager_chunk_size);

	const uint64_t bth_subctxt_rx = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t lrh_dlid =
		hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B) ? FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid) : addr.lid;
	const uint64_t pbc_dlid = OPX_PBC_DLID(addr.lid, hfi1_type);

	/* Write the first packet */
	uint32_t first_packet_psn;
	size_t	 payload_bytes_sent;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER (begin)\n");

	uint8_t *buf_bytes_ptr = (uint8_t *) buf;
	ssize_t	 rc;
	rc = opx_hfi1_tx_send_mp_egr_first_common(
		opx_ep, (void **) &buf_bytes_ptr, len, opx_ep->hmem_copy_buf, pbc_dlid, bth_subctxt_rx, lrh_dlid, addr,
		tag, data, lock_required, tx_op_flags, caps, reliability, &first_packet_psn, &payload_bytes_sent,
		hmem_iface, hmem_device, hmem_handle, hfi1_type, ctx_sharing);

	if (rc != FI_SUCCESS) {
		FI_OPX_DEBUG_COUNTERS_INC_COND(rc == -FI_ENOBUFS,
					       opx_ep->debug_counters.mp_eager.send_first_eagain_credits);
		FI_OPX_DEBUG_COUNTERS_INC_COND(rc != -FI_ENOBUFS,
					       opx_ep->debug_counters.mp_eager.send_first_eagain_reliability);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== SEND, HFI -- MULTI-PACKET EAGER EAGAIN (%s)\n",
			     (rc == -FI_ENOBUFS) ? "credits" : "reliability");

		return rc;
	}

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.mp_eager.send_first_packets);

	/* The first packet was successful. We're now committed to finishing this */
	const size_t chunk_size	       = opx_ep->tx->mp_eager_chunk_size;
	ssize_t	     payload_remaining = len - payload_bytes_sent;
	uint32_t     payload_offset    = payload_bytes_sent;
	buf_bytes_ptr += payload_bytes_sent;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER SEND REMAINING (%zu)\n",
		     payload_remaining);

	rc = opx_hfi1_tx_send_mp_egr_remaining(opx_ep, &buf_bytes_ptr, &payload_offset, &payload_remaining,
					       first_packet_psn, chunk_size, pbc_dlid, bth_subctxt_rx, lrh_dlid, addr,
					       reliability, hfi1_type, ctx_sharing);

	while (rc != FI_SUCCESS) {
		FI_OPX_DEBUG_COUNTERS_INC_COND(rc == -FI_ENOBUFS,
					       opx_ep->debug_counters.mp_eager.send_nth_eagain_credits);
		FI_OPX_DEBUG_COUNTERS_INC_COND(rc != -FI_ENOBUFS,
					       opx_ep->debug_counters.mp_eager.send_nth_eagain_reliability);

		fi_opx_ep_rx_poll(ep, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME, hfi1_type, ctx_sharing);

		OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
		union fi_opx_hfi1_pio_state pio_state = {.qw0 = opx_ep->tx->pio_state->qw0};
		fi_opx_update_credits(&pio_state, opx_ep->tx->pio_credits_addr);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

		rc = opx_hfi1_tx_send_mp_egr_remaining(opx_ep, &buf_bytes_ptr, &payload_offset, &payload_remaining,
						       first_packet_psn, chunk_size, pbc_dlid, bth_subctxt_rx, lrh_dlid,
						       addr, reliability, hfi1_type, ctx_sharing);
	}

	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_inject_completion(ep, context, len, lock_required, tag, caps);
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER (end)\n");

	return FI_SUCCESS;
}

#ifndef FI_OPX_EP_TX_SEND_EAGER_MAX_RETRIES
#define FI_OPX_EP_TX_SEND_EAGER_MAX_RETRIES (1)
#endif

__OPX_FORCE_INLINE__
ssize_t opx_ep_tx_send_try_eager(struct fid_ep *ep, const void *buf, size_t len, const union fi_opx_addr addr,
				 uint64_t tag, void *context, const struct iovec *local_iov, size_t niov,
				 size_t total_len, const uint32_t data, const int lock_required,
				 const unsigned is_contiguous, const unsigned override_flags,
				 const uint64_t tx_op_flags, const uint64_t caps,
				 const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
				 const enum fi_hmem_iface hmem_iface, const uint64_t hmem_device,
				 const uint64_t hmem_handle, const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	ssize_t rc;
	if (is_contiguous) {
		rc = OPX_FABRIC_TX_SEND_EGR(ep, buf, len, addr.fi, tag, context, data, lock_required, override_flags,
					    tx_op_flags, caps, reliability, do_cq_completion, hmem_iface, hmem_device,
					    hmem_handle, hfi1_type, ctx_sharing);
	} else {
		rc = OPX_FABRIC_TX_SENDV_EGR(ep, local_iov, niov, total_len, addr.fi, tag, context, data, lock_required,
					     override_flags, tx_op_flags, caps, reliability, do_cq_completion,
					     hmem_iface, hmem_device, hmem_handle, hfi1_type, ctx_sharing);
	}

	if (OFI_LIKELY(rc == FI_SUCCESS)) {
		return rc;
	}

	/* Note that we'll only potentially iterate this loop more than once if
	   we got here due to insufficient credits. */
	uint64_t loop = 0;
	do {
		fi_opx_ep_rx_poll(ep, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME, hfi1_type, ctx_sharing);

		if (is_contiguous) {
			rc = OPX_FABRIC_TX_SEND_EGR(ep, buf, len, addr.fi, tag, context, data, lock_required,
						    override_flags, tx_op_flags, caps, reliability, do_cq_completion,
						    hmem_iface, hmem_device, hmem_handle, hfi1_type, ctx_sharing);
		} else {
			rc = OPX_FABRIC_TX_SENDV_EGR(ep, local_iov, niov, total_len, addr.fi, tag, context, data,
						     lock_required, override_flags, tx_op_flags, caps, reliability,
						     do_cq_completion, hmem_iface, hmem_device, hmem_handle, hfi1_type,
						     ctx_sharing);
		}
	} while (rc == -FI_ENOBUFS && loop++ < FI_OPX_EP_TX_SEND_EAGER_MAX_RETRIES);

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_ep_tx_send_rzv(struct fid_ep *ep, const void *buf, size_t len, const union fi_opx_addr addr, uint64_t tag,
			   void *context, const struct iovec *local_iov, size_t niov, size_t total_len,
			   const uint32_t data, const int lock_required, const unsigned is_contiguous,
			   const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t caps,
			   const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
			   const enum fi_hmem_iface hmem_iface, const uint64_t hmem_device, const uint64_t hmem_handle,
			   const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	if (is_contiguous) {
		return OPX_FABRIC_TX_SEND_RZV(ep, buf, len, addr.fi, tag, context, data, lock_required, override_flags,
					      tx_op_flags, addr.hfi1_subctxt_rx, caps, reliability, do_cq_completion,
					      hmem_iface, hmem_device, hmem_handle, hfi1_type, ctx_sharing);
	}

	return OPX_FABRIC_TX_SENDV_RZV(ep, local_iov, niov, total_len, addr.fi, tag, context, data, lock_required,
				       override_flags, tx_op_flags, addr.hfi1_subctxt_rx, caps, reliability,
				       do_cq_completion, hmem_iface, hmem_device, hmem_handle, hfi1_type, ctx_sharing);
}

static inline ssize_t fi_opx_ep_tx_send_internal(struct fid_ep *ep, const void *buf, size_t len, void *desc,
						 fi_addr_t dest_addr, uint64_t tag, void *context, const uint32_t data,
						 const int lock_required, const enum fi_av_type av_type,
						 const unsigned is_contiguous, const unsigned override_flags,
						 const uint64_t tx_op_flags, const uint64_t caps,
						 const enum ofi_reliability_kind reliability,
						 const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== SEND (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND");

	uint64_t	   hmem_device;
	uint64_t	   hmem_handle;
	enum fi_hmem_iface hmem_iface = opx_hmem_get_mr_iface(desc, &hmem_device, &hmem_handle);

	assert(is_contiguous == OPX_CONTIG_FALSE || is_contiguous == OPX_CONTIG_TRUE);

	// Exactly one of FI_MSG or FI_TAGGED should be on
	assert((caps & (FI_MSG | FI_TAGGED)) && ((caps & (FI_MSG | FI_TAGGED)) != (FI_MSG | FI_TAGGED)));

	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	ssize_t ret;
	ret = fi_opx_ep_tx_check(opx_ep->tx, av_type);
	if (ret) {
		return ret;
	}
#endif

	assert(dest_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, dest_addr);

	ssize_t rc = 0;

#ifdef OPX_DAOS
	/* Resynch of all the Reliability Protocol(RP) related data maintained by the
	 * remote EP, must be done first before any other RP related operations are
	 * done with the remote EP.
	 */
	if (opx_ep->daos_info.do_resynch_remote_ep) {
		rc = fi_opx_reliability_do_remote_ep_resynch(ep, addr, context, caps);
		if (OFI_UNLIKELY(rc == -FI_EAGAIN)) {
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND");
			return rc;
		}
	}
#endif

	size_t		    total_len = len;
	const struct iovec *local_iov = NULL;
	size_t		    niov      = 0;
	if (!is_contiguous) { /* constant compile time expression */
		ssize_t i;
		local_iov = buf;
		niov	  = len;
		total_len = 0;
		for (i = 0; i < niov; i++) {
			total_len += local_iov[i].iov_len;
		}
	}

	const uint64_t do_cq_completion = fi_opx_ep_tx_do_cq_completion(opx_ep, override_flags, tx_op_flags);

	if (total_len < opx_ep->tx->rzv_min_payload_bytes) {
		if (total_len <= opx_ep->tx->pio_flow_eager_tx_bytes) {
			rc = opx_ep_tx_send_try_eager(ep, buf, len, addr, tag, context, local_iov, niov, total_len,
						      data, lock_required, is_contiguous, override_flags, tx_op_flags,
						      caps, reliability, do_cq_completion, hmem_iface, hmem_device,
						      hmem_handle, hfi1_type, ctx_sharing);
			if (OFI_LIKELY(rc == FI_SUCCESS)) {
				OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND");
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "===================================== SEND (end)\n");
				return FI_SUCCESS;
			}
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND");
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== SEND -- FI_EAGAIN (Eager) len=%lu\n",
				     total_len);

			return -FI_EAGAIN;
		}

		/* If hmem_iface != FI_HMEM_SYSTEM, we skip MP EGR because RZV yields better performance for devices */
		if (total_len <= opx_ep->tx->mp_eager_max_payload_bytes && is_contiguous &&
		    !fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps) && (caps & FI_TAGGED) && hmem_iface == FI_HMEM_SYSTEM) {
			assert(total_len >= opx_ep->tx->mp_eager_min_payload_bytes);
			rc = opx_hfi1_tx_send_try_mp_egr(ep, buf, len, addr.fi, tag, context, data, lock_required,
							 override_flags, tx_op_flags, caps, reliability,
							 do_cq_completion, FI_HMEM_SYSTEM, 0ul, OPX_HMEM_NO_HANDLE,
							 hfi1_type, ctx_sharing);
			if (OFI_LIKELY(rc == FI_SUCCESS)) {
				OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND");
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "===================================== SEND (end)\n");
				return FI_SUCCESS;
			}
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND");
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== SEND -- FI_EAGAIN (MP Eager) len=%lu\n",
				     total_len);

			return -FI_EAGAIN;
		}
	}

	rc = opx_ep_tx_send_rzv(ep, buf, len, addr, tag, context, local_iov, niov, total_len, data, lock_required,
				is_contiguous, override_flags, tx_op_flags, caps, reliability, do_cq_completion,
				hmem_iface, hmem_device, hmem_handle, hfi1_type, ctx_sharing);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== SEND (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_ep_tx_send(struct fid_ep *ep, const void *buf, size_t len, void *desc, fi_addr_t dest_addr, uint64_t tag,
			  void *context, const uint32_t data, const int lock_required, const enum fi_av_type av_type,
			  const unsigned is_contiguous, const unsigned override_flags, const uint64_t tx_op_flags,
			  const uint64_t caps, const enum ofi_reliability_kind reliability,
			  const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	ssize_t rc = fi_opx_ep_tx_send_internal(ep, buf, len, desc, dest_addr, tag, context, data,
						FI_OPX_LOCK_NOT_REQUIRED, av_type, is_contiguous, override_flags,
						tx_op_flags, caps, reliability, hfi1_type, ctx_sharing);

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_ep_tx_inject_internal(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				     const uint32_t data, const int lock_required, const enum fi_av_type av_type,
				     const uint64_t tx_op_flags, const uint64_t caps,
				     const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type,
				     const bool ctx_sharing)
{
	// Exactly one of FI_MSG or FI_TAGGED should be on
	assert((caps & (FI_MSG | FI_TAGGED)) && ((caps & (FI_MSG | FI_TAGGED)) != (FI_MSG | FI_TAGGED)));

	// This message check is a workaround for some versions of MPI
	// that do not check or enforce inject limits for FI_MSG
	// Remove this workaround when MPI's are upgraded to obey these limits
	if (caps & FI_MSG && len > FI_OPX_HFI1_PACKET_IMM) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "Downgrading inject to eager due to > inject limit (begin)\n");

		return fi_opx_ep_tx_send_internal(ep, buf, len, NULL, dest_addr, tag,
						  NULL, // context
						  0,	// data
						  lock_required, av_type, OPX_CONTIG_TRUE, OPX_FLAGS_OVERRIDE_TRUE,
						  FI_SELECTIVE_COMPLETION, // op flags to turn off context
						  caps, reliability, hfi1_type, ctx_sharing);
	} else {
		assert(len <= FI_OPX_HFI1_PACKET_IMM);
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== INJECT (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "INJECT");

	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

#ifndef NDEBUG
	ssize_t ret;
	ret = fi_opx_ep_tx_check(opx_ep->tx, av_type);
	if (ret) {
		return ret;
	}
#endif
	assert(dest_addr != FI_ADDR_UNSPEC);
	assert((FI_AV_TABLE == opx_ep->av_type) || (FI_AV_MAP == opx_ep->av_type));
	const union fi_opx_addr addr = FI_OPX_EP_AV_ADDR(av_type, opx_ep, dest_addr);

	const ssize_t rc = FI_OPX_FABRIC_TX_INJECT(ep, buf, len, addr.fi, tag, data, lock_required, tx_op_flags, caps,
						   reliability, hfi1_type, ctx_sharing);

	if (OFI_UNLIKELY(rc == -FI_EAGAIN)) {
		// In this case we are probably out of replay buffers. To deal
		// with this, we do a poll which may send a ping and will
		// process any incoming ACKs, hopefully releasing a buffer for
		// reuse.
		fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME, hfi1_type,
				  ctx_sharing);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "INJECT");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== INJECT (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_ep_tx_inject(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
			    const uint32_t data, const int lock_required, const enum fi_av_type av_type,
			    const uint64_t tx_op_flags, const uint64_t caps,
			    const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type,
			    const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	ssize_t rc = fi_opx_ep_tx_inject_internal(ep, buf, len, dest_addr, tag, data, FI_OPX_LOCK_NOT_REQUIRED, av_type,
						  tx_op_flags, caps, reliability, hfi1_type, ctx_sharing);

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_recv_generic(struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr, uint64_t tag,
			    uint64_t ignore, void *context, const int lock_required, const enum fi_av_type av_type,
			    const uint64_t static_flags, const enum ofi_reliability_kind reliability,
			    const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc =
		fi_opx_ep_rx_recv_internal(opx_ep, buf, len, desc, src_addr, tag, ignore, context,
					   FI_OPX_LOCK_NOT_REQUIRED, av_type, static_flags, reliability, hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_recvmsg_generic(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags, const int lock_required,
			       const enum fi_av_type av_type, const enum ofi_reliability_kind reliability,
			       const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	fi_opx_lock_if_required(&opx_ep->lock, lock_required);
	ssize_t rc = fi_opx_ep_rx_recvmsg_internal(opx_ep, msg, flags, FI_OPX_LOCK_NOT_REQUIRED, av_type, reliability,
						   hfi1_type);
	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);

	return rc;
}

#endif /* _FI_PROV_OPX_ENDPOINT_H_ */
