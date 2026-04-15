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
#ifndef _FI_PROV_OPX_HFI1_H_
#define _FI_PROV_OPX_HFI1_H_

#include "opa_user.h"

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_hfi1_packet.h"
#include "rdma/opx/fi_opx_compiler.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <arpa/inet.h>

#include "rdma/fi_errno.h" // only for FI_* errno return codes
#include "rdma/fabric.h"   // only for 'fi_addr_t' ... which is a typedef to uint64_t
#include <rdma/hfi/hfi1_user.h>
#include <uuid/uuid.h>

#include "rdma/opx/opx_hfi1_sim.h"
#include "rdma/opx/fi_opx_hfi1_version.h"
#include "rdma/opx/fi_opx_timer.h"

#include "rdma/opx/opx_hfi1_rdma_core.h"

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64)))
#define OPX_HAS_SSE2
#include <x86intrin.h> // For SIMD instructions
#endif

// #define FI_OPX_TRACE 1

#define FI_OPX_HFI1_PBC_VL_MASK	     (0xf)    /* a.k.a. "HFI_PBC_VL_MASK" */
#define FI_OPX_HFI1_PBC_VL_SHIFT     (12)     /* a.k.a. "HFI_PBC_VL_SHIFT" */
#define FI_OPX_HFI1_PBC_CR_MASK	     (0x1)    /* Force Credit return */
#define FI_OPX_HFI1_PBC_CR_SHIFT     (25)     /* Force Credit return */
#define FI_OPX_HFI1_PBC_SC4_SHIFT    (4)      /* a.k.a. "HFI_PBC_SC4_SHIFT" */
#define FI_OPX_HFI1_PBC_SC4_MASK     (0x1)    /* a.k.a. "HFI_PBC_SC4_MASK" */
#define FI_OPX_HFI1_PBC_DCINFO_SHIFT (30)     /* a.k.a. "HFI_PBC_DCINFO_SHIFT" */
#define FI_OPX_HFI1_LRH_BTH	     (0x0002) /* a.k.a. "HFI_LRH_BTH" */
#define FI_OPX_HFI1_LRH_SL_MASK	     (0xf)    /* a.k.a. "HFI_LRH_SL_MASK" */
#define FI_OPX_HFI1_LRH_SL_SHIFT     (4)      /* a.k.a. "HFI_LRH_SL_SHIFT" */
#define FI_OPX_HFI1_LRH_SC_MASK	     (0xf)    /* a.k.a. "HFI_LRH_SC_MASK" */
#define FI_OPX_HFI1_LRH_SC_SHIFT     (12)     /* a.k.a. "HFI_LRH_SC_SHIFT" */

#define FI_OPX_HFI1_SL_DEFAULT (0)  /* PSMI_SL_DEFAULT */
#define FI_OPX_HFI1_SC_DEFAULT (0)  /* PSMI_SC_DEFAULT */
#define FI_OPX_HFI1_VL_DEFAULT (0)  /* PSMI_VL_DEFAULT */
#define FI_OPX_HFI1_SC_ADMIN   (15) /* PSMI_SC_ADMIN */
#define FI_OPX_HFI1_VL_ADMIN   (15) /* PSMI_VL_ADMIN */

#define FI_OPX_HFI1_TX_SEND_RZV_CREDIT_MAX_WAIT	 0x7fffffff
#define FI_OPX_HFI1_TX_DO_REPLAY_CREDIT_MAX_WAIT 0x0000ffff
#define FI_OPX_HFI1_TX_MIN_RZV_PAYLOAD_BYTES                                       \
	(64) /* The Minimum size of a data payload Rendezvous can send an RTS for. \
		Normally, the payload should be larger than 8K */

#define FI_OPX_HFI1_TX_RELIABILITY_RESERVED_CREDITS (1) // Todo not actually reserving a credit
#define FI_OPX_HFI1_TX_CREDIT_DELTA_THRESHOLD \
	(63) // If the incomming request asks for more credits than this, force a return.  Lower number here is more
	     // agressive
#define FI_OPX_HFI1_TX_CREDIT_MIN_FORCE_CR \
	(130) // We won't force a credit return for FI_OPX_HFI1_TX_CREDIT_DELTA_THRESHOLD if the number
	      // of avalible credits is above this number

#define OPX_MP_EGR_MAX_PAYLOAD_BYTES_DEFAULT (16384) /* Default for max payload size for using Multi-packet Eager */
#define OPX_MP_EGR_MAX_PAYLOAD_BYTES_MAX     (65535) /* Max value (set to fit within uint16_t) */
#define OPX_MP_EGR_DISABLE_SET		     (1)
#define OPX_MP_EGR_DISABLE_NOT_SET	     (0)
#define OPX_MP_EGR_DISABLE_DEFAULT	     (OPX_MP_EGR_DISABLE_NOT_SET)

/* Default for payload threshold size for RZV */
#if HAVE_CUDA
#define OPX_RZV_MIN_PAYLOAD_BYTES_DEFAULT (4096)
#elif HAVE_ROCR
#define OPX_RZV_MIN_PAYLOAD_BYTES_DEFAULT (256)
#else
#define OPX_RZV_MIN_PAYLOAD_BYTES_DEFAULT (OPX_MP_EGR_MAX_PAYLOAD_BYTES_DEFAULT + 1)
#endif
#define OPX_RZV_MIN_PAYLOAD_BYTES_MIN (FI_OPX_HFI1_TX_MIN_RZV_PAYLOAD_BYTES) /* Min value */
#define OPX_RZV_MIN_PAYLOAD_BYTES_MAX (OPX_MP_EGR_MAX_PAYLOAD_BYTES_MAX + 1) /* Max value */

/* Maximum Multi-packet eager chunk size. The chunk size should be at most 8K,
   even if we're using a larger (i.e. 10K) packet MTU */
#define OPX_MP_EGR_MAX_CHUNK_SIZE_WFR  (4096)
#define OPX_MP_EGR_MAX_CHUNK_SIZE_CN5K (8192)

/* SDMA tuning constants */

/*
 * The maximum number of packets to send for a single SDMA call to writev.
 */
#ifndef OPX_HFI1_SDMA_MAX_PKTS
#define OPX_HFI1_SDMA_MAX_PKTS (128)
#endif

#ifndef OPX_HFI1_SDMA_DEFAULT_PKTS
#define OPX_HFI1_SDMA_DEFAULT_PKTS (32)
#endif

#ifndef OPX_HFI1_SDMA_MAX_PKTS_TID
#define OPX_HFI1_SDMA_MAX_PKTS_TID (512)
#endif

#ifndef OPX_HFI1_SDMA_DEFAULT_PKTS_TID
#define OPX_HFI1_SDMA_DEFAULT_PKTS_TID (64)
#endif

static_assert(OPX_HFI1_SDMA_DEFAULT_PKTS <= OPX_HFI1_SDMA_MAX_PKTS,
	      "OPX_HFI1_SDMA_DEFAULT_PKTS must be less than or equal to OPX_HFI1_SDMA_MAX_PKTS!");
static_assert(OPX_HFI1_SDMA_DEFAULT_PKTS_TID <= OPX_HFI1_SDMA_MAX_PKTS_TID,
	      "OPX_HFI1_SDMA_DEFAULT_PKTS_TID must be less than or equal to OPX_HFI1_SDMA_MAX_PKTS_TID!");
/*
 * The number of SDMA requests (SDMA work entries) available.
 * Each of these will use a single comp index entry in the SDMA ring buffer
 * queue when writev is called. There should be at least as many of these as
 * there are queue entries (FI_OPX_HFI1_SDMA_MAX_COMP_INDEX), but ideally more
 * so that new requests can be built while others are in flight.
 *
 * Note: We never want this limit to be the source of throttling progress in
 *       an SDMA request. If we are hitting this limit (as represented by
 *       debug_counters.sdma.eagain_sdma_we_none_free), we should increase it.
 *       The hfi->info.sdma.available_counter will take care of throttling us
 *       on too much SDMA work at once.
 */
#ifndef FI_OPX_HFI1_SDMA_MAX_WE
#ifndef OPX_JKR_SUPPORT
#define FI_OPX_HFI1_SDMA_MAX_WE (256)
#else
#define FI_OPX_HFI1_SDMA_MAX_WE (1024)
#endif
#endif

/*
 * The maximum number of SDMA work entries that a single DPUT operation can use.
 *
 * Note: We never want this limit to be the source of throttling progress in
 *       an SDMA request. If we are hitting this limit (as represented by
 *       debug_counters.sdma.eagain_sdma_we_max_used), we should increase it.
 *       The hfi->info.sdma.available_counter will take care of throttling us
 *       on too much SDMA work at once.
 */
#ifndef FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ
#define FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ (8)
#endif

/*
 * The number of iovecs in a single SDMA Work Entry.
 * 1 payload data vec, 1 TID mapping.
 */
#define FI_OPX_HFI1_SDMA_WE_IOVS (2)

#define FI_OPX_HFI1_SDMA_MAX_COMP_INDEX (128) // This should what opx_ep->hfi->info.sdma.queue_size is set to.

/* Default for payload threshold size for SDMA */
#ifndef FI_OPX_SDMA_MIN_PAYLOAD_BYTES_DEFAULT
#if HAVE_CUDA
#define FI_OPX_SDMA_MIN_PAYLOAD_BYTES_DEFAULT (4096)
#elif HAVE_ROCR
#define FI_OPX_SDMA_MIN_PAYLOAD_BYTES_DEFAULT (256)
#else
#define FI_OPX_SDMA_MIN_PAYLOAD_BYTES_DEFAULT (16385)
#endif
#endif
#define FI_OPX_SDMA_MIN_PAYLOAD_BYTES_MIN (FI_OPX_HFI1_TX_MIN_RZV_PAYLOAD_BYTES) /* Min Value */
#define FI_OPX_SDMA_MIN_PAYLOAD_BYTES_MAX (INT_MAX - 1)				 /* Max Value */

/* Default for payload threshold size for TID */
#ifndef OPX_TID_MIN_PAYLOAD_BYTES_DEFAULT
#if HAVE_CUDA
#define OPX_TID_MIN_PAYLOAD_BYTES_DEFAULT (4096)
#elif HAVE_ROCR
#define OPX_TID_MIN_PAYLOAD_BYTES_DEFAULT (4096)
#else
#define OPX_TID_MIN_PAYLOAD_BYTES_DEFAULT (4096)
#endif
#endif
#define OPX_TID_MIN_PAYLOAD_BYTES_MIN (OPX_HFI1_TID_PAGESIZE)
static_assert(OPX_TID_MIN_PAYLOAD_BYTES_DEFAULT >= OPX_TID_MIN_PAYLOAD_BYTES_MIN,
	      "OPX_TID_MIN_PAYLOAD_BYTES_DEFAULT must be >= OPX_TID_MIN_PAYLOAD_BYTES_MIN!\n");

static_assert(!(FI_OPX_HFI1_SDMA_MAX_COMP_INDEX & (FI_OPX_HFI1_SDMA_MAX_COMP_INDEX - 1)),
	      "FI_OPX_HFI1_SDMA_MAX_COMP_INDEX must be power of 2!\n");
static_assert(FI_OPX_HFI1_SDMA_MAX_WE >= FI_OPX_HFI1_SDMA_MAX_COMP_INDEX,
	      "FI_OPX_HFI1_SDMA_MAX_WE must be >= FI_OPX_HFI1_SDMA_MAX_COMP_INDEX!\n");

/*
 * SDMA includes 8B sdma hdr, 8B PBC, and message header.
 */
#define FI_OPX_HFI1_SDMA_HDR_SIZE (8 + 8 + 56)

// Version 1, EAGER opcode (1)(byte 0), 0 iovectors (byte 1, set at runtime)
#define FI_OPX_HFI1_SDMA_REQ_HEADER_EAGER_FIXEDBITS (0x0011)

// Version 1, EXPECTED TID opcode (0)(byte 0), 0 iovectors (byte 1, set at runtime)
#define FI_OPX_HFI1_SDMA_REQ_HEADER_EXPECTED_FIXEDBITS (0x0001)

static inline uint32_t fi_opx_addr_calculate_base_rx(const uint32_t process_id, const uint32_t processes_per_node)
{
	abort();
	return 0;
}

/* Also refer to union opx_hfi1_packet_hdr comment

 SCB (Send Control Block) is 8 QW's written to PIO SOP.

 Optimally, store 8 contiguous QW's.

 Cannot define a common 9B/16B structure that is contiguous,
 so send code is 9B/16B aware.

		    TX SCB
      =====================================================
      GENERIC      9B                   16B
      =========    ==================   ===================
QW[0]  PBC
QW[1]  HDR         qw_9B[0] LRH         qw_16B[0] LRH
QW[2]  HDR         qw_9B[1] BTH         qw_16B[1] LRH
QW[3]  HDR         qw_9B[2] BTH/KDETH   qw_16B[2] BTH
QW[4]  HDR         qw_9B[3] KDETH       qw_16B[3] BTH/KDETH
QW[5]  HDR         qw_9B[4] USER/SW     qw_16B[4] KDETH
QW[6]  HDR         qw_9B[5] USER/SW     qw_16B[5] USER/SW
QW[7]  HDR         qw_9B[6] USER/SW     qw_16B[6] USER/SW

					qw_16B[7] USER/SW

Generic example

// faster than memcpy() for this amount of data.
// SCB (PIO or UREG) COPY ONLY (STORE)
static inline void fi_opx_store_scb_qw(volatile uint64_t dest[8], const uint64_t source[8])
{
	OPX_HFI1_BAR_STORE(&dest[0], source[0]);
	OPX_HFI1_BAR_STORE(&dest[1], source[1]);
	OPX_HFI1_BAR_STORE(&dest[2], source[2]);
	OPX_HFI1_BAR_STORE(&dest[3], source[3]);
	OPX_HFI1_BAR_STORE(&dest[4], source[4]);
	OPX_HFI1_BAR_STORE(&dest[5], source[5]);
	OPX_HFI1_BAR_STORE(&dest[6], source[6]);
	OPX_HFI1_BAR_STORE(&dest[7], source[7]);
}


9B/16B example, must be hfi1-aware

	struct fi_opx_hfi1_txe_scb_9B  model_9B  = opx_ep->reliability->service.tx.hfi1.ping_model_9B;
	struct fi_opx_hfi1_txe_scb_16B model_16B = opx_ep->reliability->service.tx.hfi1.ping_model_16B;

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	if ((hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B))) {
		OPX_HFI1_BAR_STORE(&scb[0], (model_9B.qw0 | OPX_PBC_CR(0x1, hfi1_type) |
OPX_PBC_LRH_DLID_TO_PBC_DLID(lrh_dlid, hfi1_type))); OPX_HFI1_BAR_STORE(&scb[1], (model_9B.hdr.qw_9B[0] | lrh_dlid));
		OPX_HFI1_BAR_STORE(&scb[2], (model_9B.hdr.qw_9B[1] | bth_rx));
<...>
	} else {
		OPX_HFI1_BAR_STORE(&scb[0], (model_16B.qw0 | OPX_PBC_CR(1, hfi1_type) |
OPX_PBC_LRH_DLID_TO_PBC_DLID(lrh_dlid, hfi1_type))); OPX_HFI1_BAR_STORE(&scb[1], (model_16B.hdr.qw_16B[0] |
((uint64_t)(ntohs(dlid) & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B)));
		OPX_HFI1_BAR_STORE(&scb[2], (model_16B.hdr.qw_16B[1] | ((uint64_t)(ntohs(dlid) &
OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B))); OPX_HFI1_BAR_STORE(&scb[3],
model_16B.hdr.qw_16B[2] | bth_rx);
<...>
	}

*/

/* 8 QWs valid in 16 QW storage. */
struct fi_opx_hfi1_txe_scb_9B {
	union { /* 15 QWs union*/

		/* pbc is qw0. it overlays hdr's unused_pad_9B */
		struct {
			uint64_t qw0;
			uint64_t qw[14];
		} __attribute__((__packed__)) __attribute__((__aligned__(8)));

		union opx_hfi1_packet_hdr hdr; /* 1 QW unused + 7 QWs 9B header + 7 QWs unused*/

	} __attribute__((__packed__)) __attribute__((__aligned__(8)));

	uint64_t pad; /* 1 QW pad (to 16 QWs) */
} __attribute__((__aligned__(8))) __attribute__((packed));
static_assert(offsetof(struct fi_opx_hfi1_txe_scb_9B, qw0) == 0,
	      "Offset of fi_opx_hfi1_txe_scb_9B.hdr.qw_9B[0] should immediately follow fi_opx_hfi1_txe_scb_9B.qw0!\n");
static_assert(offsetof(struct fi_opx_hfi1_txe_scb_9B, hdr.qw_9B[0]) ==
		      (offsetof(struct fi_opx_hfi1_txe_scb_9B, qw0) + sizeof(uint64_t)),
	      "Offset of fi_opx_hfi1_txe_scb_9B.hdr.qw_9B[0] should immediately follow fi_opx_hfi1_txe_scb_9B.qw0!\n");

/* 9 QWs valid in 16 QW storage.  */
struct fi_opx_hfi1_txe_scb_16B {
	uint64_t		  qw0; /* PBC */
	union opx_hfi1_packet_hdr hdr; /* 8 QWs 16B header + 7 QWs currently unused */
} __attribute__((__aligned__(8))) __attribute__((packed));

static_assert((sizeof(struct fi_opx_hfi1_txe_scb_9B) == sizeof(struct fi_opx_hfi1_txe_scb_16B)),
	      "storage for scbs should match");
static_assert((sizeof(struct fi_opx_hfi1_txe_scb_9B) == (sizeof(uint64_t) * 16)), "16 qw scb storage");

/* Storage for a scb. Use HFI1 type to access the correct structure */
union opx_hfi1_txe_scb_union {
	uint64_t		       qws[16];
	struct fi_opx_hfi1_txe_scb_9B  scb_9B;
	struct fi_opx_hfi1_txe_scb_16B scb_16B;
} __attribute__((__aligned__(8))) __attribute__((packed));
static_assert((sizeof(struct fi_opx_hfi1_txe_scb_9B) == sizeof(union opx_hfi1_txe_scb_union)),
	      "storage for scbs should match");
static_assert((sizeof(struct fi_opx_hfi1_txe_scb_16B) == sizeof(union opx_hfi1_txe_scb_union)),
	      "storage for scbs should match");
static_assert(offsetof(struct fi_opx_hfi1_txe_scb_9B, qw0) == offsetof(union opx_hfi1_txe_scb_union, qws[0]),
	      "Offset of fi_opx_hfi1_txe_scb_9B.hdr.qw0 should be same as opx_hfi1_txe_scb_union.qws[0]!\n");
static_assert(offsetof(struct fi_opx_hfi1_txe_scb_16B, qw0) == offsetof(union opx_hfi1_txe_scb_union, qws[0]),
	      "Offset of fi_opx_hfi1_txe_scb_16B.hdr.qw0 should be same as opx_hfi1_txe_scb_union.qws[0]!\n");
static_assert(offsetof(struct fi_opx_hfi1_txe_scb_9B, hdr.qw_9B[0]) == offsetof(union opx_hfi1_txe_scb_union, qws[1]),
	      "Offset of fi_opx_hfi1_txe_scb_9B.hdr.qw_9B[0] should be same as opx_hfi1_txe_scb_union.qws[1]!\n");
static_assert(offsetof(struct fi_opx_hfi1_txe_scb_16B, hdr.qw_16B[0]) == offsetof(union opx_hfi1_txe_scb_union, qws[1]),
	      "Offset of fi_opx_hfi1_txe_scb_16B.hdr.qw_16B[0] should be same as opx_hfi1_txe_scb_union.qws[1]!\n");

#define HFI_TXE_CREDITS_COUNTER(credits)	((credits.raw16b[0] >> 0) & 0x07FFu)
#define HFI_TXE_CREDITS_STATUS(credits)		((credits.raw16b[0] >> 11) & 0x01u)
#define HFI_TXE_CREDITS_DUETOPBC(credits)	((credits.raw16b[0] >> 12) & 0x01u)
#define HFI_TXE_CREDITS_DUETOTHRESHOLD(credits) ((credits.raw16b[0] >> 13) & 0x01u)
#define HFI_TXE_CREDITS_DUETOERR(credits)	((credits.raw16b[0] >> 14) & 0x01u)
#define HFI_TXE_CREDITS_DUETOFORCE(credits)	((credits.raw16b[0] >> 15) & 0x01u)
union fi_opx_hfi1_txe_credits {
	uint16_t raw16b[4];
	uint64_t raw64b;

	struct {
		uint16_t Counter : 11; /* use macros to access */
		uint16_t Status : 1;
		uint16_t CreditReturnDueToPbc : 1;
		uint16_t CreditReturnDueToThreshold : 1;
		uint16_t CreditReturnDueToErr : 1;
		uint16_t CreditReturnDueToForce : 1;

		uint16_t pad[3];
	} __attribute__((packed));
};

#define FI_OPX_HFI1_DUMP_TXE_CREDITS(credits) fi_opx_hfi1_dump_txe_credits(credits, __FILE__, __func__, __LINE__);

static inline void fi_opx_hfi1_dump_txe_credits(union fi_opx_hfi1_txe_credits *credits, const char *file,
						const char *func, unsigned line)
{
	fprintf(stderr, "%s:%s():%d === dump hfi1 txe credits ===\n", file, func, line);
	fprintf(stderr, "%s:%s():%d .raw64b ...................... 0x%016lx\n", file, func, line, credits->raw64b);
	fprintf(stderr, "%s:%s():%d .Counter ..................... %hu\n", file, func, line, credits->Counter);
	fprintf(stderr, "%s:%s():%d .Status ...................... %hu\n", file, func, line, credits->Status);
	fprintf(stderr, "%s:%s():%d .CreditReturnDueToPbc ........ %hu\n", file, func, line,
		credits->CreditReturnDueToPbc);
	fprintf(stderr, "%s:%s():%d .CreditReturnDueToThreshold .. %hu\n", file, func, line,
		credits->CreditReturnDueToThreshold);
	fprintf(stderr, "%s:%s():%d .CreditReturnDueToErr ........ %hu\n", file, func, line,
		credits->CreditReturnDueToErr);
	fprintf(stderr, "%s:%s():%d .CreditReturnDueToForce ...... %hu\n", file, func, line,
		credits->CreditReturnDueToForce);
}

/* This 'state' information will update on each txe pio operation */
union fi_opx_hfi1_pio_state {
	uint64_t qw0;

	struct {
		uint16_t fill_counter;
		uint16_t free_counter_shadow;
		uint16_t scb_head_index;
		uint16_t credits_total; /* yeah, yeah .. THIS field is static, but there was an unused halfword at this
					   spot, so .... */
	};
};

/* This 'static' information will not change after it is set by the driver
 * and can be safely copied into other structures to improve cache layout */
struct fi_opx_hfi1_pio_static {
	volatile uint64_t *scb_sop_first;
	volatile uint64_t *scb_first;

	/* pio credit return address. The HFI TXE periodically updates this
	 * host memory location with the current credit state. To avoid cache
	 * thrashing software should read from this location sparingly. */
	union {
		volatile uint64_t		       *credits_addr;
		volatile union fi_opx_hfi1_txe_credits *credits;
	};
};

/* This 'state' information will update on each txe sdma operation */
union fi_opx_hfi1_sdma_state {
	uint64_t qw0;

	//	struct {
	//		uint16_t		pio_fill_counter;
	//		uint16_t		pio_free_counter_shadow;
	//		uint16_t		pio_scb_head_index;
	//		uint16_t		unused;
	//	};
};

struct opx_sdma_comp_entry {
	uint32_t status;
	uint32_t errcode;
	uint64_t start_time_ns;
	uint64_t end_time_ns;
};

/* This 'static' information will not change after it is set by the driver
 * and can be safely copied into other structures to improve cache layout */
struct fi_opx_hfi1_sdma_static {
	uint16_t			      available_counter;
	uint16_t			      fill_index;
	uint16_t			      done_index;
	uint16_t			      queue_size;
	volatile struct hfi1_sdma_comp_entry *completion_queue;
	struct opx_sdma_comp_entry	     *queued_entries[FI_OPX_HFI1_SDMA_MAX_COMP_INDEX];
};

struct fi_opx_hfi1_rxe_state {
	struct {
		uint64_t head;
		uint64_t rhf_seq;
	} __attribute__((__packed__)) hdrq;

} __attribute__((__packed__));

struct fi_opx_hfi1_rxe_static {
	struct {
		uint32_t *base_addr;

		uint32_t rhf_off;
		uint32_t elemsz;

		uint32_t elemlast;
		uint32_t elemcnt;

		uint64_t rx_poll_mask;

		uint32_t *rhf_base;

		uint64_t *rhe_base;

		volatile uint64_t *head_register;

	} hdrq;

	struct {
		uint32_t *base_addr;

		uint32_t elemsz;
		uint32_t size;

		volatile uint64_t *head_register;

	} egrq;

	uint8_t id; /* hfi receive context id [0..159] */
	uint8_t unused[7];
};

/*
 * This is the shared PIO state structure that sits in shared memory. It is only used for context sharing
 * and allows all of the endpoints in a context sharing group to synchronize over the HFI context's PIO
 * resources. Any endpoint wishing to send over PIO must hold the spio_ctrl_lock before reading/updating the available
 * credits. This structure is placed in the page of shared memory setup by the driver and pointed to
 * by hfi1_base_info.subctxt_uregbase.
 */
struct opx_spio_ctrl {
	union fi_opx_hfi1_pio_state pio;	    // 1 qw
	pthread_spinlock_t	    spio_ctrl_lock; // 4 bytes
	uint32_t		    unused;
	volatile uint64_t	   *credits_addr_copy;
	uint64_t		    dummy_free_credits;
	uint64_t		    unused1[4];
} __attribute__((aligned(64)));

/* Up to HFI1_MAX_SHARED_CTXTS of these strucutres are placed in the page of shared memory
pointed to by hfi1_base_info.subctxt_uregbase. There is one for each subctxt in a context sharing group and
describes the subctxt's software rx queue. An endpoint wishing to forward a packet to a subctxt's software rx
queue must hold the opx_hwcontext_ctrl.context_lock and then use the hdrq_rhf_seq for determining the next
sequence number to write into the software RHQ entry. uregbase contains head and tail pointers to positions in
the software rx queue's RHQ and eager buffer queue respectively.*/
struct opx_subcontext_ureg {
	/* head/eager head/tail register storage, one per cacheline */
	uint64_t uregbase[40 /* i.e. ur_maxreg * 8 */];
	uint64_t hdrq_rhf_seq; /* Next sequence number to be written from the writer's perspective */
	uint64_t last_offset;  /* Last offset into the software rx q's eager buffer index.*/
} __attribute__((aligned(64)));

/*
 * Describes the shared hardware RHQ state for when context sharing is in use.
 * This structure is placed in shared memory that can be accessed by all processes
 * sharing a single HFI context. The context_lock must be held before reading or writing
 * to this structure. Only one process is allowed to make progress on the hardware RHQ for
 * a single HFI context being shared.
 */
struct opx_hwcontext_ctrl {
	pthread_spinlock_t context_lock; /* lock shared by all subctxts */
	uint32_t	   last_egrbfr_index;
	uint64_t	   rx_hdrq_rhf_seq; /* rhf seq for the hw hdrq shared by all subctxts */
	uint64_t	   hdrq_head;	    /* software copy of head */
	uint64_t	   hfi_frozen_count;
} __attribute__((aligned(64)));

/* Locking macros for shared PIO state when context sharing is in use. */
#define OPX_SHD_CTX_PIO_LOCK(ctx_sharing_enabled, ep_tx)              \
	if (ctx_sharing_enabled) {                                    \
		pthread_spin_lock(&ep_tx->spio_ctrl->spio_ctrl_lock); \
	}
#define OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing_enabled, ep_tx)              \
	if (ctx_sharing_enabled) {                                      \
		pthread_spin_unlock(&ep_tx->spio_ctrl->spio_ctrl_lock); \
	}

struct fi_opx_hfi1_context {
	struct {
		union fi_opx_hfi1_pio_state  pio;
		union fi_opx_hfi1_sdma_state sdma;
		struct fi_opx_hfi1_rxe_state rxe;
	} state;

	struct {
		struct fi_opx_hfi1_pio_static  pio;
		struct fi_opx_hfi1_sdma_static sdma;
		struct fi_opx_hfi1_rxe_static  rxe;

	} info;

	int		   fd_cdev;
	int		   fd_verbs;
	opx_lid_t	   lid;
	enum opx_hfi1_type hfi1_type;
	struct _hfi_ctrl  *ctrl;
	uint32_t	   hfi_unit;
	uint32_t	   hfi_port;
	uint16_t	   subctxt_cnt;
	uint16_t	   unused[3];

	uint64_t gid_hi;
	uint64_t gid_lo;
	uint16_t mtu;
	uint8_t	 bthqp;
	uint8_t	 subctxt;
	uint16_t jkey;
	uint16_t send_ctxt;

	uint16_t sl2sc[32];
	uint16_t sc2vl[32];
	uint64_t sl;
	uint64_t sc;
	uint64_t vl;
	uint64_t pkey;

	uint64_t runtime_flags;

	struct {
		int rank;
		int rank_inst;
	} daos_info;

	int64_t		   ref_cnt;
	size_t		   status_lasterr;
	time_t		   network_lost_time;
	uint64_t	   status_check_next_usec;
	uint64_t	   link_down_wait_time_max_sec;
	volatile uint64_t *credits_addr_copy;

	uint64_t		    dummy_free_credits;
	uint64_t		    hfi1_frozen_count; /* local copy */
	union fi_opx_hfi1_pio_state dummy_pio_state;

	/* struct ibv_context * for hfi1 direct/rdma-core only */
	void *ibv_context;

	/* Pointers to context sharing structures in shared memory */
	struct opx_hwcontext_ctrl  *hwcontext_ctrl;
	struct opx_subcontext_ureg *subcontext_ureg[HFI1_MAX_SHARED_CTXTS];
	struct opx_spio_ctrl	   *spio_ctrl;
};

struct fi_opx_hfi1_context_internal {
	struct fi_opx_hfi1_context context;

	struct hfi1_user_info_dep user_info;
	struct _hfi_ctrl	 *ctrl;
};

#ifdef NDEBUG
#define FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(credits_addr)
#else
#define FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(credits_addr) \
	fi_opx_hfi1_check_credits_for_error(credits_addr, __FILE__, __func__, __LINE__);
#endif

static inline void fi_opx_hfi1_check_credits_for_error(volatile uint64_t *credits_addr, const char *file,
						       const char *func, unsigned line)
{
	const uint64_t credit_return = *credits_addr;
	if ((credit_return & 0x0000000000004800ul) != 0) {
		fprintf(stderr, "%s:%s():%d ########### PIO SEND ERROR!\n", file, func, line);
		fi_opx_hfi1_dump_txe_credits((union fi_opx_hfi1_txe_credits *) credits_addr, file, func, line);
		abort();
	}

	return;
}

__OPX_FORCE_INLINE__
uint16_t fi_opx_credits_in_use(union fi_opx_hfi1_pio_state *pio_state)
{
	assert(((pio_state->fill_counter - pio_state->free_counter_shadow) & 0x07FFu) <= pio_state->credits_total);
	return ((pio_state->fill_counter - pio_state->free_counter_shadow) & 0x07FFu);
}

__OPX_FORCE_INLINE__
uint16_t fi_opx_credits_avail(union fi_opx_hfi1_pio_state *pio_state, uint8_t *force_credit_return,
			      uint16_t credits_needed)
{
	const uint16_t return_value = pio_state->credits_total - fi_opx_credits_in_use(pio_state);
	if ((return_value < FI_OPX_HFI1_TX_CREDIT_MIN_FORCE_CR) &&
	    (credits_needed > FI_OPX_HFI1_TX_CREDIT_DELTA_THRESHOLD)) {
		*force_credit_return = 1;
	}
	return return_value;
}

__OPX_FORCE_INLINE__
uint16_t fi_opx_reliability_credits_avail(union fi_opx_hfi1_pio_state *pio_state)
{
	return pio_state->credits_total - fi_opx_credits_in_use(pio_state);
}

__OPX_FORCE_INLINE__
volatile uint64_t *fi_opx_pio_scb_base(volatile uint64_t *pio_scb_base, union fi_opx_hfi1_pio_state *pio_state)
{
	return ((pio_scb_base) + (pio_state->scb_head_index << 3));
}

__OPX_FORCE_INLINE__
void fi_opx_update_credits(union fi_opx_hfi1_pio_state *pio_state, volatile uint64_t *pio_credits_addr)
{
	volatile uint64_t *credits_addr	 = (uint64_t *) (pio_credits_addr);
	const uint64_t	   credit_return = *credits_addr;
	pio_state->free_counter_shadow	 = (uint16_t) (credit_return & 0x00000000000007FFul);
}

__OPX_FORCE_INLINE__
void fi_opx_consume_credits(union fi_opx_hfi1_pio_state *pio_state, size_t count)
{
	assert((pio_state->scb_head_index + count) <= pio_state->credits_total);
	pio_state->scb_head_index =
		(pio_state->scb_head_index + count) * (pio_state->credits_total != (pio_state->scb_head_index + count));
	pio_state->fill_counter = (pio_state->fill_counter + count) & 0x00000000000007FFul;
}

#define FI_OPX_HFI1_CREDITS_IN_USE(pio_state)			fi_opx_credits_in_use(&pio_state)
#define FI_OPX_HFI1_UPDATE_CREDITS(pio_state, pio_credits_addr) fi_opx_update_credits(&pio_state, pio_credits_addr)
#define FI_OPX_HFI1_PIO_SCB_HEAD(pio_scb_base, pio_state)	fi_opx_pio_scb_base(pio_scb_base, &pio_state)
#define FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, force_credit_return, credits_needed) \
	fi_opx_credits_avail(&pio_state, force_credit_return, credits_needed)
#define FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) fi_opx_reliability_credits_avail(&pio_state)
#define FI_OPX_HFI1_CONSUME_CREDITS(pio_state, count)	     fi_opx_consume_credits(&pio_state, count)
#define FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state)	     FI_OPX_HFI1_CONSUME_CREDITS(pio_state, 1)

#ifdef OPX_HAS_SSE2

/* SIMD version (preferred, fastest) */
__OPX_FORCE_INLINE__
int opx_local_lid_index(opx_lid_t lid)
{
	// Create a 4-element vector populated with the lid we're looking for
	__m128i target = _mm_set1_epi32(lid);

	// Iterate through the array of lids, examining 4 entries at once. Since
	// we know unused entries are zero, and lids will be non-zero, it's safe
	// to include unused entries beyond local_lids_size in our comparison
	for (int i = 0; i < fi_opx_global.hfi_local_info.local_lids_size; i += 4) {
		__m128i current = _mm_load_si128((__m128i *) &fi_opx_global.hfi_local_info.local_lid_ids[i]);
		__m128i compare = _mm_cmpeq_epi32(current, target);
		int	mask	= _mm_movemask_ps((__m128) compare);
		if (mask) {
			// Return the array index into local_lid_ids where the match was found
			return i + __builtin_ctz(mask);
		}
	}

	return -1;
}

#else

/* Loop unrolled version */
__OPX_FORCE_INLINE__
int opx_local_lid_index(opx_lid_t lid)
{
	// Iterate through the array of lids, examining 4 entries at once. Since
	// we know unused entries are zero, and lids will be non-zero, it's safe
	// to include unused entries beyond local_lids_size in our comparison
	for (int i = 0; i < fi_opx_global.hfi_local_info.local_lids_size; i += 4) {
		if (fi_opx_global.hfi_local_info.local_lid_ids[i] == lid) {
			return i;
		}
		if (fi_opx_global.hfi_local_info.local_lid_ids[i + 1] == lid) {
			return i + 1;
		}
		if (fi_opx_global.hfi_local_info.local_lid_ids[i + 2] == lid) {
			return i + 2;
		}
		if (fi_opx_global.hfi_local_info.local_lid_ids[i + 3] == lid) {
			return i + 3;
		}
	}

	return -1;
}
#endif

__OPX_FORCE_INLINE__
struct opx_hfi_local_entry *fi_opx_hfi1_get_lid_local(opx_lid_t lid)
{
	int lid_index = opx_local_lid_index(lid);

	// We should only ever be calling this function for lids that are shm
	assert(lid_index != -1);

	return &fi_opx_global.hfi_local_info.local_lid_entries[lid_index];
}

__OPX_FORCE_INLINE__
bool opx_lid_is_shm(opx_lid_t lid)
{
	return (opx_local_lid_index(lid) != -1);
}

__OPX_FORCE_INLINE__
int fi_opx_hfi1_get_lid_local_unit(opx_lid_t lid)
{
	return ((fi_opx_global.hfi_local_info.lid == lid) ? fi_opx_global.hfi_local_info.hfi_unit :
							    fi_opx_hfi1_get_lid_local(lid)->hfi_unit);
}

struct fi_opx_hfi1_context *fi_opx_hfi1_context_open(struct fid_ep *ep, uuid_t unique_job_key);

int init_hfi1_rxe_state(struct fi_opx_hfi1_context *context, struct fi_opx_hfi1_rxe_state *rxe_state);

void opx_remove_self_lid(const int hfi_unit, const opx_lid_t lid);

void fi_opx_init_hfi_lookup(bool sriov);

/*
 * Shared memory transport
 */
#define FI_OPX_SHM_FIFO_SIZE   (1024)
#define FI_OPX_SHM_BUFFER_MASK (FI_OPX_SHM_FIFO_SIZE - 1)

#define FI_OPX_SHM_PACKET_SIZE (OPX_HFI1_MAX_PKT_SIZE + sizeof(union opx_hfi1_packet_hdr))

#ifndef NDEBUG
#define OPX_BUF_FREE(x)                                \
	do {                                           \
		memset(x, 0x3C, MIN(512, sizeof(*x))); \
		ofi_buf_free(x);                       \
	} while (0)
#else
#define OPX_BUF_FREE(x) ofi_buf_free(x)
#endif

__OPX_FORCE_INLINE__
void opx_print_context(struct fi_opx_hfi1_context *context)
{
	/* WARNING: Do not read PROT_WRITE memory (pio) */
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.pio.qw0                 %#lX\n",
	       context->state.pio.qw0);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.pio.fill_counter        %#X \n",
	       context->state.pio.fill_counter);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.pio.free_counter_shadow %#X \n",
	       context->state.pio.free_counter_shadow);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.pio.scb_head_index      %#X \n",
	       context->state.pio.scb_head_index);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.pio.credits_total       %#X \n",
	       context->state.pio.credits_total);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.sdma.qw0                %#lX\n",
	       context->state.sdma.qw0);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.rxe.hdrq.head           %#lX\n",
	       context->state.rxe.hdrq.head);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context state.rxe.hdrq.rhf_seq        %#lX \n",
	       context->state.rxe.hdrq.rhf_seq);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.pio.scb_sop_first pio_bufbase_sop %p \n",
	       context->info.pio.scb_sop_first);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.pio.scb_first pio_bufbase         %p \n",
	       context->info.pio.scb_first);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.pio.free_counter_shadow           %p credits: %#lX\n",
	       context->info.pio.credits_addr, context->info.pio.credits_addr ? *context->info.pio.credits_addr : 0UL);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.sdma.available_counter   %#X\n",
	       context->info.sdma.available_counter);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.sdma.fill_index          %#X\n",
	       context->info.sdma.fill_index);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.sdma.done_index          %#X\n",
	       context->info.sdma.done_index);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.sdma.queue_size          %#X\n",
	       context->info.sdma.queue_size);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.sdma.completion_queue    %p errcode %#X status %#X\n",
	       context->info.sdma.completion_queue, context->info.sdma.completion_queue->errcode,
	       context->info.sdma.completion_queue->status);
	/*	Not printing                                Context info.sdma.queued_entries);          */

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.base_addr       %p \n",
	       context->info.rxe.hdrq.base_addr);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.rhf_off         %#X\n",
	       context->info.rxe.hdrq.rhf_off);
	/*	Not printing                                Context info.rxe.hdrq.rhf_notail            */
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.elemsz          %#X\n",
	       context->info.rxe.hdrq.elemsz);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.elemlast        %#X\n",
	       context->info.rxe.hdrq.elemlast);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.elemcnt         %#X\n",
	       context->info.rxe.hdrq.elemcnt);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.rx_poll_mask    %#lX\n",
	       context->info.rxe.hdrq.rx_poll_mask);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.rhf_base        %p \n",
	       context->info.rxe.hdrq.rhf_base);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.rhe_base        %p \n",
	       context->info.rxe.hdrq.rhe_base);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.hdrq.head_register   %p \n",
	       context->info.rxe.hdrq.head_register);
	/*	Not printing                                Context info.rxe.hdrq.tail_register         */
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.egrq.base_addr       %p \n",
	       context->info.rxe.egrq.base_addr);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.egrq.elemsz          %#X\n",
	       context->info.rxe.egrq.elemsz);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.egrq.size            %#X\n",
	       context->info.rxe.egrq.size);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.egrq.head_register   %p \n",
	       context->info.rxe.egrq.head_register);
	/*	Not printing                                Context info.rxe.egrq.tail_register         */
	/*	Not printing                                Context info.rxe.uregbase                   */
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context info.rxe.id                   %#X\n", context->info.rxe.id);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context fd_cdev                       %#X \n", context->fd_cdev);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context fd_verbs                      %#X \n", context->fd_verbs);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context lid                           %#X \n", context->lid);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context ctrl                          %p  \n", context->ctrl);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context hfi1_type                     %#X \n", context->hfi1_type);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context hfi_unit                      %#X \n", context->hfi_unit);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context hfi_port                      %#X \n", context->hfi_port);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context gid_hi                        %#lX\n", context->gid_hi);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context gid_lo                        %#lX\n", context->gid_lo);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context mtu                           %#X \n", context->mtu);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context bthqp                         %#X \n", context->bthqp);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context jkey                          %#X \n", context->jkey);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context send_ctxt                     %#X \n", context->send_ctxt);
	for (int s = 0; s < 32; ++s) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context sl2sc[%d]                     %#X \n", s,
		       context->sl2sc[s]);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context sc2vl[%d]                     %#X \n", s,
		       context->sc2vl[s]);
	}
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context sl                            %#lX \n", context->sl);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context sc                            %#lX \n", context->sc);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context vl                            %#lX \n", context->vl);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context pkey                          %#lX \n", context->pkey);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context runtime_flags                 %#lX \n",
	       context->runtime_flags);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context daos_info.rank                %#X  \n",
	       context->daos_info.rank);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context daos_info.rank_inst           %#X  \n",
	       context->daos_info.rank_inst);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context ref_cnt                       %#lX \n", context->ref_cnt);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context subctxt_cnt                   %#X  \n",
	       context->subctxt_cnt);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Context subctxt                  	  %#X  \n", context->subctxt);
}

void opx_reset_context(struct fi_opx_ep *opx_ep, uint64_t events, const enum opx_hfi1_type hfi1_type,
		       const bool ctx_sharing);

void opx_link_down_update_pio_credit_addr(struct fi_opx_hfi1_context *context, struct fi_opx_ep *opx_ep,
					  const bool ctx_sharing);

void opx_link_up_update_pio_credit_addr(struct fi_opx_hfi1_context *context, struct fi_opx_ep *opx_ep,
					const bool ctx_sharing);

#define OPX_CONTEXT_STATUS_CHECK_INTERVAL_USEC 250000 /* 250 ms*/

__OPX_FORCE_INLINE__
uint64_t opx_get_hw_status_wfr(struct fi_opx_hfi1_context *context)
{
	struct hfi1_status *status = (struct hfi1_status *) context->ctrl->base_info.status_bufbase;

	return ((status->dev & (HFI1_STATUS_INITTED | HFI1_STATUS_CHIP_PRESENT | HFI1_STATUS_HWERROR)) |
		(status->port & (HFI1_STATUS_IB_READY | HFI1_STATUS_IB_CONF)));
}

#ifdef OPX_JKR_SUPPORT
__OPX_FORCE_INLINE__
uint64_t opx_get_hw_status_jkr(struct fi_opx_hfi1_context *context)
{
	struct hfi1_status_v2 *status	  = (struct hfi1_status_v2 *) context->ctrl->base_info.status_bufbase;
	uint32_t	       port_index = (context->hfi_port - 1);

	return ((status->dev & (HFI1_STATUS_INITTED | HFI1_STATUS_CHIP_PRESENT | HFI1_STATUS_HWERROR)) |
		(status->ports[port_index] & (HFI1_STATUS_IB_READY | HFI1_STATUS_IB_CONF)));
}
#else
__OPX_FORCE_INLINE__
uint64_t opx_get_hw_status_jkr(struct fi_opx_hfi1_context *context)
{
	return 0;
}
#endif

#define OPX_HFI1_HW_CHIP_STATUS (HFI1_STATUS_CHIP_PRESENT | HFI1_STATUS_INITTED)
#define OPX_HFI1_IB_STATUS	(HFI1_STATUS_IB_CONF | HFI1_STATUS_IB_READY)

/* The linkup time duration for a system should allow the time needed
   to complete 3 LNI passes which is:
   50 seconds for a passive copper channel
   65 seconds for optical channel.
   (we add 5 seconds of margin.) */
#define OPX_LINK_DOWN_WAIT_TIME_MAX_SEC_DEFAULT 70.0

__OPX_FORCE_INLINE__
size_t fi_opx_context_check_status(struct fi_opx_hfi1_context *context, const enum opx_hfi1_type hfi1_type,
				   struct fi_opx_ep *opx_ep, const bool ctx_sharing)
{
	size_t	 err	= FI_SUCCESS;
	uint64_t status = (hfi1_type & OPX_HFI1_WFR) ? opx_get_hw_status_wfr(context) : opx_get_hw_status_jkr(context);

	/* Fatal chip-related errors */
	if (!((status & OPX_HFI1_HW_CHIP_STATUS) == OPX_HFI1_HW_CHIP_STATUS) || (status & HFI1_STATUS_HWERROR)) {
		err = FI_ENETUNREACH;
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_CTRL, "HFI1 chip error detected\n");
		abort();
		return (err);
	} else if (!((status & OPX_HFI1_IB_STATUS) == OPX_HFI1_IB_STATUS)) {
		err = FI_ENETDOWN;
		if (err != context->status_lasterr) {
			context->network_lost_time = time(NULL);
			/* Point the pio credit addr to a dummy pointer */
			opx_link_down_update_pio_credit_addr(context, opx_ep, ctx_sharing);
		} else {
			time_t now = time(NULL);

			if (difftime(now, context->network_lost_time) > context->link_down_wait_time_max_sec) {
				fprintf(stderr, "Link has been down more than %lds. Aborting\n",
					context->link_down_wait_time_max_sec);
				abort();
				return (err);
			}
		}
	}

	if (err != FI_SUCCESS) {
		context->status_lasterr = err; /* record error */
	}

	return err;
}

#endif /* _FI_PROV_OPX_HFI1_H_ */
