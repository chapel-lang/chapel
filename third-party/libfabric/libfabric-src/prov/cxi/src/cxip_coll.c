/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2020-2025 Hewlett Packard Enterprise Development LP
 * Support for accelerated collective reductions.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

#include <ofi_list.h>
#include <ofi.h>
#include <fenv.h>

#include "cxip.h"

#ifndef _MM_GET_FLUSH_ZERO_MODE
#define _MM_GET_FLUSH_ZERO_MODE() ({0;})
#endif

#define	TRACE_PKT(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_COLL_PKT, fmt, \
					   ##__VA_ARGS__)
#define	TRACE_JOIN(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_COLL_JOIN, fmt, \
					   ##__VA_ARGS__)
#define	TRACE_DEBUG(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_COLL_DEBUG, fmt, \
					   ##__VA_ARGS__)

// TODO regularize usage of these
#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

#if ENABLE_DEBUG
#define __chk_pkts	1
#define __trc_pkts	1
#define __trc_data	1
#else
#define __chk_pkts	0
#define __trc_pkts	0
#define __trc_data	0
#endif

#define	MAGIC		0x677d
#define	TIMER_UNSET	-1

/****************************************************************************
 * Metrics for evaluating collectives
 */

struct cxip_coll_metrics_loc {
	ofi_atomic64_t red_count_bad;
	ofi_atomic64_t red_count_full;
	ofi_atomic64_t red_count_partial;
	ofi_atomic64_t red_count_unreduced;
	struct cxip_coll_metrics_ep ep_data;
};
static struct cxip_coll_metrics_loc _coll_metrics;

void cxip_coll_init_metrics(void)
{
	ofi_atomic_initialize64(&_coll_metrics.red_count_bad, 0);
	ofi_atomic_initialize64(&_coll_metrics.red_count_full, 0);
	ofi_atomic_initialize64(&_coll_metrics.red_count_partial, 0);
	ofi_atomic_initialize64(&_coll_metrics.red_count_unreduced, 0);
	memset(&_coll_metrics.ep_data, 0, sizeof(_coll_metrics.ep_data));
}

void cxip_coll_get_metrics(struct cxip_coll_metrics *metrics)
{
	metrics->red_count_bad =
		ofi_atomic_get64(&_coll_metrics.red_count_bad);
	metrics->red_count_full =
		ofi_atomic_get64(&_coll_metrics.red_count_full);
	metrics->red_count_partial =
		ofi_atomic_get64(&_coll_metrics.red_count_partial);
	metrics->red_count_unreduced =
		ofi_atomic_get64(&_coll_metrics.red_count_unreduced);
	memcpy(&metrics->ep_data, &_coll_metrics.ep_data,
	       sizeof(struct cxip_coll_metrics_ep));
}

static inline void _measure_completions(int red_cnt, size_t total)
{
	if (red_cnt >= total)
		ofi_atomic_inc64(&_coll_metrics.red_count_bad);
	else if (red_cnt == total-1)
		ofi_atomic_inc64(&_coll_metrics.red_count_full);
	else if (red_cnt > 1)
		ofi_atomic_inc64(&_coll_metrics.red_count_partial);
	else if (red_cnt > 0)
		ofi_atomic_inc64(&_coll_metrics.red_count_unreduced);
	else
		ofi_atomic_inc64(&_coll_metrics.red_count_bad);
}


void dump_reduction_table(int red_id, struct cxip_coll_reduction *r)
{
#if ENABLE_DEBUG
	int i;
	char idx[4];

	TRACE_DEBUG("+----------------------------------------------------------------+\n");
	TRACE_DEBUG("| red_id | seqno | resno | in_use | pktsent | completed | red_rc |\n");

	for (i = 0; i < CXIP_COLL_MAX_CONCUR; i++) {
		if (i == red_id)
			snprintf(idx, sizeof(idx), "*%d*", i);
		else
			snprintf(idx, sizeof(idx), " %d ", i);
		TRACE_DEBUG(
		"|----------------------------------------------------------------|\n");
		TRACE_DEBUG("|   %s  | %5d | %5d | %6d | %7d | %9d | %6d |\n",
		idx, r[i].seqno, r[i].resno, r[i].in_use, r[i].pktsent, r[i].completed,
		r[i].red_rc);
	}
	TRACE_DEBUG("+----------------------------------------------------------------+\n");
#endif
}

/****************************************************************************
 * Reduction packet for hardware accelerated collectives:
 *
 *  +----------------------------------------------------------+
 *  | BYTES | Mnemonic    | Definition                         |
 *  +----------------------------------------------------------+
 *  | 48:17 | RED_PAYLOAD | Reduction payload, always 32 bytes |
 *  | 16:5  | RED_HDR     | Reduction Header (below)           |
 *  | 4:0   | RED_PADDING | Padding                            |
 *  +----------------------------------------------------------+
 *
 *  Reduction header format:
 *  --------------------------------------------------------
 *  | Field          | Description              | Bit | Size (bits)
 *  --------------------------------------------------------
 *  | rt_seqno       | Sequence number          |  0  | 10 |
 *  | rt_arm         | Multicast arm command    | 10  |  1 |
 *  | rt_op          | Reduction operation      | 11  |  6 |
 *  | rt_count       | Number of contributions  | 17  | 20 |
 *  | rt_resno       | Result number            | 37  | 10 |
 *  | rt_rc          | result code              | 47  |  4 |
 *  | rt_repsum_m    | Reproducible sum M value | 51  |  8 |
 *  | rt_repsum_ovfl | Reproducible sum M ovfl  | 59  |  2 |
 *  | rt_pad         | Pad to 64 bits           | 61  |  3 |
 *  | rt_cookie      | Cookie value             | 64  | 32 |
 *  --------------------------------------------------------
 *
 * Note that this header is a 12-byte object, and "network-defined order" means
 * big-endian for the entire 12-byte object. Thus, bytes must be swapped so
 * that the MSByte of rt_cookie appears at byte 0, and the LS 8 bits of
 * rt_seqno appear in byte 11.
 *
 * The cookie is ignored by reduction hardware, and is used as follows:
 *
 * mcast_id is the 13-bit multicast address used to disambiguate multiple
 * multicast trees, since all incoming collective traffic is received by a
 * single PTE bound to the endpoint.
 *
 * red_id is used to disambiguate packets delivered for different concurrent
 * reductions.
 *
 * magic is a magic number used to positively identify this packet as a
 * reduction packet. The basic send/receive code could be used for other kinds
 * of restricted IDC packets. At present, all such packets are discarded.
 *
 * retry is a control bit that can be invoked by the hw root node to initiate a
 * retransmission of the data from the leaves, if packets are lost.
 *
 * A re-arm of an armed switch port may not clear the data in the port,
 * resulting in incorrect results. Arming twice will guarantee that the
 * old data is cleared.
 *
 * To disambiguate these two arming packets, it is recommended that the first
 * arm use a reserved sequence number, allowing the software to receive the
 * first arm packet (and data), identify it as a pre-emptive arm, and discard
 * it.
 *
 * The sequence numbers occupy 10 bits of the packet header. The sequence
 * numbers are monotonically incremented modulo ((1 << 10)-1), meaning that
 * the largest sequence number will be ((1 << 10)-2). The unreachable value
 * of ((1 << 10)-1) is designated the reserved value for pre-emptive arming.
 */

struct cxip_coll_cookie {
	uint32_t mcast_id:13;
	uint32_t red_id:3;
	uint32_t magic: 16;
} __attribute__((__packed__));           /* size  4b */

/* Packed header bits and cookie from above */
struct cxip_coll_hdr {
	uint64_t seqno:10;
	uint64_t arm:1;
	uint64_t op:6;
	uint64_t redcnt:20;
	uint64_t resno:10;
	uint64_t red_rc:4;
	uint64_t repsum_m:8;
	uint64_t repsum_ovflid:2;
	uint64_t retry:1;
	uint64_t pad:2;
        struct cxip_coll_cookie cookie;
} __attribute__((__packed__));		/* size 12b */

/* The following structure is 49 bytes in size, and all of the fields align
 * properly for network transmission.
 */
struct red_pkt {
	uint8_t pad[1];			/* size  1b offset  0b */
	uint32_t sender_rank;		/* size  4b offset  1b */
	struct cxip_coll_hdr hdr;	/* size 12b offset  5b */
	uint8_t data[32];		/* size 32b offset 17b */
} __attribute__((__packed__));		/* size 49b */

struct red_pkt_64 {
	struct red_pkt pkt;
} __attribute__((aligned(64)));

/* Swap byte order in an object of any size. Works for even or odd counts */
static inline
void _swapbyteorder(void *ptr, int count)
{
	uint8_t *p1 = (uint8_t *)ptr;
	uint8_t *p2 = p1 + count - 1;
	uint8_t swp;
	while (p1 < p2) {
		swp = *p1;
		*p1 = *p2;
		*p2 = swp;
		p1++;
		p2--;
	}
}
static int _leaf_rdma_get(struct cxip_coll_reduction *reduction);
/**
 * Reformat the packet to accommodate network-ordering (big-endian) Rosetta
 * expectations, versus little-endian Intel processing.
 *
 * Note in particular that the header bytes are treated as a single 12-byte
 * object, rather than an 8-byte followed by a 4-byte, i.e. the last byte of the
 * cookie is the first byte of the data processed by Rosetta. Note also that
 * there is a 5-byte pad at the beginning of the packet, not included in the
 * byte-swapping.
 *
 * This is done in-place for convenience. For reductions, it is copied to a
 * properly-aligned data structure for mathematical operations.
 */
static inline
void _swappkt(struct red_pkt *pkt)
{
#if (BYTE_ORDER == LITTLE_ENDIAN)
	uint64_t *data = (uint64_t *)pkt->data;
	int i;

	_swapbyteorder(&pkt->hdr, sizeof(pkt->hdr));
	for (i = 0; i < 4; i++)
		_swapbyteorder(&data[i], 8);
#else
#error "Unsupported processor byte ordering"
#endif
}

/**
 * Verificaton of the packet structure, normally disabled. Sizes and offsets
 * cannot be checked at compile time. If the structure is wrong, this will
 * call abort().
 */
#define FLDOFFSET(base, fld)	((uint8_t *)&base.fld - (uint8_t *)&base)
__attribute__((unused)) static inline
void check_red_pkt(void)
{
#if __chk_pkts
	static int checked = 0;
	struct red_pkt pkt;
	uint64_t len, exp;
	uint8_t *ptr, offset;
	int i, err = 0;

	if (checked)
		return;
	checked = 1;

	len = sizeof(pkt);
	exp = 49;
	if (len != exp) {
		TRACE_PKT("sizeof(pkt) = %ld, exp %ld\n", len, exp);
		err++;
	}
	len = sizeof(pkt.pad);
	exp = 1;
	if (len != exp) {
		TRACE_PKT("sizeof(pkt.pad) = %ld, exp %ld\n", len, exp);
		err++;
	}
	len = sizeof(pkt.sender_rank);
	exp = 4;
	if (len != exp) {
		TRACE_PKT("sizeof(pkt.sender_rank) = %ld, exp %ld\n", len, exp);
		err++;
	}
	len = sizeof(pkt.hdr);
	exp = 12;
	if (len != exp) {
		TRACE_PKT("sizeof(pkt.hdr) = %ld, exp %ld\n", len, exp);
		err++;
	}
	len = sizeof(pkt.data);
	exp = 32;
	if (len != exp) {
		TRACE_PKT("sizeof(pkt.data) = %ld, exp %ld\n", len, exp);
		err++;
	}
	len = FLDOFFSET(pkt, hdr);
	exp = 5;
	if (len != exp) {
		TRACE_PKT("offset(pkt.hdr) = %ld, exp %ld\n", len, exp);
		err++;
	}
	len = FLDOFFSET(pkt, data);
	exp = 17;
	if (len != exp) {
		TRACE_PKT("offset(pkt.data) = %ld, exp %ld\n", len, exp);
		err++;
	}

	/* Arbitrary value between 1,15 inclusive, ensure non-zero fill */
	offset = 13;

	/* Fill, swap, and confirm integrity of all 49 bytes */
	ptr = (uint8_t *)&pkt;
	for (i = 0; i < sizeof(pkt); i++)
		ptr[i] = i + offset;
	_swappkt(&pkt);
	_swappkt(&pkt);
	for (i = 0; i < sizeof(pkt); i++)
		if (ptr[i] != i + offset) {
			TRACE_PKT("pkt[%d] = %d, exp %d\n", i, ptr[i], i + offset);
			err++;
		}

	if (err) {
		TRACE_PKT("*** INVALID STRUCTURE see above ***\n");
		abort();
	}
#endif
}

__attribute__((unused)) static inline
void _dump_red_pkt(struct red_pkt *pkt, char *dir, char *tag)
{
#if __trc_pkts
	__attribute__((__unused__)) const uint64_t *data
		= (const uint64_t *)pkt->data;
	__attribute__((__unused__)) int i;

	TRACE_PKT("---------------\n");
	TRACE_PKT("Reduction packet (%s):\n", dir);
	TRACE_PKT("%s - seqno        = %d\n", tag, pkt->hdr.seqno);
	TRACE_PKT("%s - retry        = %d\n", tag, pkt->hdr.retry);
	TRACE_PKT("%s - arm          = %d\n", tag, pkt->hdr.arm);
	TRACE_PKT("%s - op           = %d\n", tag, pkt->hdr.op);
	TRACE_PKT("%s - redcnt       = %d\n", tag, pkt->hdr.redcnt);
	TRACE_PKT("%s - resno        = %d\n", tag, pkt->hdr.resno);
	TRACE_PKT("%s - red_rc       = %d\n", tag, pkt->hdr.red_rc);
	TRACE_PKT("%s - repsum_m     = %d\n", tag, pkt->hdr.repsum_m);
	TRACE_PKT("%s - repsum_ovflid= %d\n", tag, pkt->hdr.repsum_ovflid);
	TRACE_PKT("%s - cookie --\n", tag);
	TRACE_PKT("%s - .mcast_id   = %08x\n", tag, pkt->hdr.cookie.mcast_id);
	TRACE_PKT("%s - .red_id     = %08x\n", tag, pkt->hdr.cookie.red_id);
	TRACE_PKT("%s - .magic      = %08x\n", tag, pkt->hdr.cookie.magic);
	for (i = 0; i < 4; i++)
		TRACE_PKT("%s - ival[%d]     = %016lx\n", tag, i, data[i]);
	TRACE_PKT("---------------\n");
#endif
}

/****************************************************************************
 * Reduction operators for accelerated collectives.
 *
 * The array lookup is faster than a switch. Non-static initialization makes
 * this adaptive to changes in header files (e.g. new opcodes in FI).
 */
#define COLL_OPCODE_BARRIER		0x00
#define COLL_OPCODE_BIT_AND		0x01
#define COLL_OPCODE_BIT_OR		0x02
#define COLL_OPCODE_BIT_XOR		0x03
#if 0 /* keep for future use */
#define COLL_OPCODE_LOG_AND		0x04
#define COLL_OPCODE_LOG_OR		0x05
#define COLL_OPCODE_LOG_XOR		0x06
#endif
#define COLL_OPCODE_INT_MIN		0x10
#define COLL_OPCODE_INT_MAX		0x11
#define COLL_OPCODE_INT_MINMAXLOC	0x12
#define COLL_OPCODE_INT_SUM		0x14
#define COLL_OPCODE_FLT_MINNUM		0x24
#define COLL_OPCODE_FLT_MAXNUM		0x25
#define COLL_OPCODE_FLT_MINMAXNUMLOC	0x26
#define COLL_OPCODE_FLT_SUM_NOFTZ_RND0	0x28
#define COLL_OPCODE_FLT_SUM_NOFTZ_RND1	0x29
#define COLL_OPCODE_FLT_SUM_NOFTZ_RND2	0x2a
#define COLL_OPCODE_FLT_SUM_NOFTZ_RND3	0x2b
#define COLL_OPCODE_FLT_SUM_FTZ_RND0	0x2c
#define COLL_OPCODE_FLT_SUM_FTZ_RND1	0x2d
#define COLL_OPCODE_FLT_SUM_FTZ_RND2	0x2e
#define COLL_OPCODE_FLT_SUM_FTZ_RND3	0x2f
#define COLL_OPCODE_FLT_REPSUM		0x30
#define COLL_OPCODE_MAX			0x31

/* Convert exported op values to Rosetta opcodes */
static cxip_coll_op_t _int8_16_32_op_to_opcode[FI_CXI_OP_LAST];
static cxip_coll_op_t _uint8_16_32_op_to_opcode[FI_CXI_OP_LAST];
static cxip_coll_op_t _int64_op_to_opcode[FI_CXI_OP_LAST];
static cxip_coll_op_t _uint64_op_to_opcode[FI_CXI_OP_LAST];
static cxip_coll_op_t _flt_op_to_opcode[FI_CXI_OP_LAST];
static enum cxip_coll_redtype _cxi_op_to_redtype[COLL_OPCODE_MAX];

/* One-time dynamic initialization of FI to CXI opcode.
 */
void cxip_coll_populate_opcodes(void)
{
	int i;

	if ((int)FI_CXI_MINMAXLOC < (int)FI_ATOMIC_OP_LAST) {
		CXIP_FATAL("Invalid CXI_FMINMAXLOC value\n");
	}
	for (i = 0; i < FI_CXI_OP_LAST; i++) {
		_int8_16_32_op_to_opcode[i] = -FI_EOPNOTSUPP;
		_uint8_16_32_op_to_opcode[i] = -FI_EOPNOTSUPP;
		_int64_op_to_opcode[i] = -FI_EOPNOTSUPP;
		_uint64_op_to_opcode[i] = -FI_EOPNOTSUPP;
		_flt_op_to_opcode[i] = -FI_EOPNOTSUPP;
		_cxi_op_to_redtype[i] = REDTYPE_BYT;
	}
	/* operations supported by 32, 16, and 8 bit signed int operands */
	/* NOTE: executed as packed 64-bit quantities */
	_int8_16_32_op_to_opcode[FI_BOR] = COLL_OPCODE_BIT_OR;
	_int8_16_32_op_to_opcode[FI_BAND] = COLL_OPCODE_BIT_AND;
	_int8_16_32_op_to_opcode[FI_BXOR] = COLL_OPCODE_BIT_XOR;
#if 0 /* keep for future use */	
	_int8_16_32_op_to_opcode[FI_LOR] = COLL_OPCODE_LOG_OR;
	_int8_16_32_op_to_opcode[FI_LAND] = COLL_OPCODE_LOG_AND;
	_int8_16_32_op_to_opcode[FI_LXOR] = COLL_OPCODE_LOG_XOR;
#endif	

	/* operations supported by 32, 16, and 8 bit unsigned int operands */
	_uint8_16_32_op_to_opcode[FI_BOR] = COLL_OPCODE_BIT_OR;
	_uint8_16_32_op_to_opcode[FI_BAND] = COLL_OPCODE_BIT_AND;
	_uint8_16_32_op_to_opcode[FI_BXOR] = COLL_OPCODE_BIT_XOR;
#if 0 /* keep for future use */	
	_uint8_16_32_op_to_opcode[FI_LOR] = COLL_OPCODE_LOG_OR;
	_uint8_16_32_op_to_opcode[FI_LAND] = COLL_OPCODE_LOG_AND;
	_uint8_16_32_op_to_opcode[FI_LXOR] = COLL_OPCODE_LOG_XOR;
#endif

	/* operations supported by 64 bit signed int operands */
	_int64_op_to_opcode[FI_MIN] = COLL_OPCODE_INT_MIN;
	_int64_op_to_opcode[FI_MAX] = COLL_OPCODE_INT_MAX;
	_int64_op_to_opcode[FI_SUM] = COLL_OPCODE_INT_SUM;
	_int64_op_to_opcode[FI_CXI_MINMAXLOC] = COLL_OPCODE_INT_MINMAXLOC;

	/* operations supported by 64 bit unsigned int operands */
	_uint64_op_to_opcode[FI_BOR] = COLL_OPCODE_BIT_OR;
	_uint64_op_to_opcode[FI_BAND] = COLL_OPCODE_BIT_AND;
	_uint64_op_to_opcode[FI_BXOR] = COLL_OPCODE_BIT_XOR;
#if 0 /* keep for future use */	
	_uint64_op_to_opcode[FI_LOR] = COLL_OPCODE_LOG_OR;
	_uint64_op_to_opcode[FI_LAND] = COLL_OPCODE_LOG_AND;
	_uint64_op_to_opcode[FI_LXOR] = COLL_OPCODE_LOG_XOR;
#endif

	/* operations supported by 64 bit double operands */
	_flt_op_to_opcode[FI_MIN] = COLL_OPCODE_FLT_MINNUM;
	_flt_op_to_opcode[FI_MAX] = COLL_OPCODE_FLT_MAXNUM;
	_flt_op_to_opcode[FI_CXI_MINMAXLOC] = COLL_OPCODE_FLT_MINMAXNUMLOC;
	_flt_op_to_opcode[FI_CXI_REPSUM] = COLL_OPCODE_FLT_REPSUM;
	/* NOTE: FI_SUM handled in flt_op_to_opcode() function */

	/* cxi_opcode to redtype translation */
	_cxi_op_to_redtype[COLL_OPCODE_BIT_OR] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_BIT_AND] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_BIT_XOR] = REDTYPE_INT;
#if 0 /* keep for future use */	
	_cxi_op_to_redtype[COLL_OPCODE_LOG_OR] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_LOG_AND] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_LOG_XOR] = REDTYPE_INT;
#endif

	_cxi_op_to_redtype[COLL_OPCODE_INT_MIN] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_INT_MAX] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_INT_SUM] = REDTYPE_INT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_MINNUM] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_MAXNUM] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_NOFTZ_RND0] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_NOFTZ_RND1] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_NOFTZ_RND2] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_NOFTZ_RND3] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_FTZ_RND0] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_FTZ_RND1] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_FTZ_RND2] = REDTYPE_FLT;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_SUM_FTZ_RND3] = REDTYPE_FLT;

	_cxi_op_to_redtype[COLL_OPCODE_INT_MINMAXLOC] = REDTYPE_IMINMAX;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_MINMAXNUMLOC] = REDTYPE_FMINMAX;
	_cxi_op_to_redtype[COLL_OPCODE_FLT_REPSUM] = REDTYPE_REPSUM;
}

static inline int int8_16_32_op_to_opcode(int op)
{
	return _int8_16_32_op_to_opcode[op];
}

static inline int uint8_16_32_op_to_opcode(int op)
{
	return _uint8_16_32_op_to_opcode[op];
}

static inline int int64_op_to_opcode(int op)
{
	return _int64_op_to_opcode[op];
}

static inline int uint64_op_to_opcode(int op)
{
	return _uint64_op_to_opcode[op];
}

static inline int flt_op_to_opcode(int op)
{
	if (op != FI_SUM)
		return _flt_op_to_opcode[op];

	switch (fegetround()) {
	case FE_TONEAREST:
		return (_MM_GET_FLUSH_ZERO_MODE()) ?
			COLL_OPCODE_FLT_SUM_FTZ_RND0 :
			COLL_OPCODE_FLT_SUM_NOFTZ_RND0;
	case FE_UPWARD:
		return (_MM_GET_FLUSH_ZERO_MODE()) ?
			COLL_OPCODE_FLT_SUM_FTZ_RND1 :
			COLL_OPCODE_FLT_SUM_NOFTZ_RND1;
	case FE_DOWNWARD:
		return (_MM_GET_FLUSH_ZERO_MODE()) ?
			COLL_OPCODE_FLT_SUM_FTZ_RND2 :
			COLL_OPCODE_FLT_SUM_NOFTZ_RND2;
	case FE_TOWARDZERO:
		return (_MM_GET_FLUSH_ZERO_MODE()) ?
			COLL_OPCODE_FLT_SUM_FTZ_RND3 :
			COLL_OPCODE_FLT_SUM_NOFTZ_RND3;
	}
	return -FI_EOPNOTSUPP;
}

/* Convert CXI opcode to reduction data type */
static inline
enum cxip_coll_redtype _opcode_to_redtype(cxip_coll_op_t cxi_opcode)
{
	return _cxi_op_to_redtype[cxi_opcode];
}

/* Convert FI opcode to CXI opcode, depending on FI data type */
static inline
int cxip_fi2cxi_opcode(enum fi_op op, enum fi_datatype datatype)
{
	switch ((int)datatype) {
	case FI_INT8:
	case FI_INT16:
	case FI_INT32:
		return int8_16_32_op_to_opcode(op);
	case FI_UINT8:
	case FI_UINT16:
	case FI_UINT32:
		return uint8_16_32_op_to_opcode(op);
	case FI_INT64:
		return int64_op_to_opcode(op);
	case FI_UINT64:
		return uint64_op_to_opcode(op);
	case FI_DOUBLE:
		return flt_op_to_opcode(op);
	}
	return -FI_EOPNOTSUPP;
}

/* Determine FI datatype size */
static inline
int _get_cxi_data_bytcnt(cxip_coll_op_t cxi_opcode,
			 enum fi_datatype datatype, size_t count)
{
	int size;

	switch (datatype) {
	case FI_INT8:
	case FI_UINT8:
		size = sizeof(uint8_t);
		break;
	case FI_INT16:
	case FI_UINT16:
		size = sizeof(uint16_t);
		break;
	case FI_INT32:
	case FI_UINT32:
		size = sizeof(uint32_t);
		break;
	case FI_INT64:
	case FI_UINT64:
		size = sizeof(uint64_t);
		break;
	case FI_FLOAT:
		size = sizeof(float);
		break;
	case FI_DOUBLE:
		size = sizeof(double);
		break;
	default:
		return -FI_EOPNOTSUPP;
	}
	switch (cxi_opcode) {
	case COLL_OPCODE_INT_MINMAXLOC:
	case COLL_OPCODE_FLT_MINMAXNUMLOC:
	case COLL_OPCODE_FLT_REPSUM:
		size *= 4;
		break;
	default:
		// do nothing, size is correct
		break;
	}
	size *= count;
	if (size > CXIP_COLL_MAX_DATA_SIZE)
		return -FI_EINVAL;
	return size;
}

/****************************************************************************
 * SEND operation (restricted Put to a remote PTE)
 */

/* Forward references */
static void _progress_coll(struct cxip_coll_reduction *reduction,
			   struct red_pkt *pkt);
static ssize_t _coll_append_buffer(struct cxip_coll_pte *coll_pte,
				   struct cxip_coll_buf *buf);

/* Generate a dfa and index extension for a reduction */
static int _gen_tx_dfa(struct cxip_coll_reduction *reduction,
		       int av_set_idx, union c_fab_addr *dfa,
		       uint8_t *index_ext, bool *is_mcast)
{
	struct cxip_coll_mc *mc;
	struct cxip_ep_obj *ep_obj;
	struct cxip_av_set *av_set_obj;
	struct cxip_addr dest_caddr;
	fi_addr_t dest_addr;
	int pid_bits;
	int idx_ext;
	int ret;

	/* cxi_build_mcast_dfa() found in:
	    cassini-headers/src/csrdef/cassini_user_defs.h
	    cassini-headers/include/cxi_prov_hw.h
	*/
	mc = reduction->mc_obj;
	ep_obj = mc->ep_obj;
	av_set_obj = mc->av_set_obj;

	/* Send address */
	switch (av_set_obj->comm_key.keytype) {
	case COMM_KEY_NONE:
	case COMM_KEY_MULTICAST:
		/* - destination == multicast ID
		 * - idx_ext == 0
		 * - dfa == multicast address
		 * - index_ext == 0
		 */
		if (is_netsim(ep_obj)) {
			CXIP_WARN("NETSIM does not support mcast\n");
			return -FI_EINVAL;
		}
		idx_ext = 0;
		cxi_build_mcast_dfa(mc->mcast_addr,	// mcast_id
				    reduction->red_id,	// red_id
				    idx_ext,		// idx_ext
				    dfa,		// return dfa
				    index_ext);		// return idx_ext
		*is_mcast = true;
		break;
	case COMM_KEY_UNICAST:
		/* - destination == remote node in av_set_obj
		 * - idx_ext == CXIP_PTL_IDX_COLL
		 * - dfa = remote nic
		 * - index_ext == idx_ext
		 */
		if (av_set_idx >= av_set_obj->fi_addr_cnt) {
			CXIP_WARN("av_set_idx out-of-range\n");
			return -FI_EINVAL;
		}
		dest_addr = av_set_obj->fi_addr_ary[av_set_idx];
		ret = cxip_av_lookup_addr(ep_obj->av, dest_addr, &dest_caddr);
		if (ret != FI_SUCCESS)
			return ret;
		idx_ext = CXIP_PTL_IDX_COLL;
		pid_bits = ep_obj->domain->iface->dev->info.pid_bits;
		cxi_build_dfa(dest_caddr.nic,		// dest NIC
			      dest_caddr.pid,		// dest PID
			      pid_bits,			// pid width
			      idx_ext,			// idx_ext
			      dfa,			// return dfa
			      index_ext);		// return idx_ext
		*is_mcast = false;
		break;
	case COMM_KEY_RANK:
		/* - destination == source NIC
		 * - idx_ext == extended PID
		 * - dfa == source NIC
		 * - index_ext == idx_ext offset beyond RXCs (5-bit range)
		 */
		if (av_set_idx >= av_set_obj->fi_addr_cnt) {
			CXIP_WARN("av_set_idx out-of-range\n");
			return -FI_EINVAL;
		}
		dest_caddr = ep_obj->src_addr;
		pid_bits = ep_obj->domain->iface->dev->info.pid_bits;
		idx_ext = CXIP_PTL_IDX_COLL + av_set_idx;
		cxi_build_dfa(dest_caddr.nic,		// dest NIC
			      dest_caddr.pid,		// dest PID
			      pid_bits,			// pid width
			      idx_ext,			// idx_ext
			      dfa,			// return dfa
			      index_ext);		// return idx_ext
		*is_mcast = false;
		break;
	default:
		CXIP_WARN("Unexpected comm_key type: %d\n",
			  av_set_obj->comm_key.keytype);
		return -FI_EINVAL;
	}
	return FI_SUCCESS;
}

/**
 * Issue a restricted Put to the destination address.
 * If md is NULL, this performs an IDC Put, otherwise it issues a DMA Put.
 *
 * Exported for unit testing.
 *
 * This will return -FI_EAGAIN on transient errors.
 */
int cxip_coll_send(struct cxip_coll_reduction *reduction,
		   int av_set_idx, const void *buffer, size_t buflen,
		   struct cxi_md *md)
{
	union c_cmdu cmd = {};
	struct cxip_coll_mc *mc_obj;
	struct cxip_ep_obj *ep_obj;
	struct cxip_cmdq *cmdq;
	union c_fab_addr dfa;
	uint8_t index_ext;
	bool is_mcast;
	int ret;

	if (!buffer) {
		CXIP_WARN("No buffer\n");
		return -FI_EINVAL;
	}

	mc_obj = reduction->mc_obj;
	ep_obj = mc_obj->ep_obj;
	cmdq = ep_obj->coll.tx_cmdq;

	ret = _gen_tx_dfa(reduction, av_set_idx, &dfa, &index_ext, &is_mcast);
	if (ret) {
		CXIP_INFO("Failed to generate TX destination fabric address "
			  "(ret=%d, av_set_idx=%d, red_id=%d)\n",
			  ret, av_set_idx, reduction->red_id);
		return ret;
	}

	if (cxip_evtq_saturated(ep_obj->coll.tx_evtq)) {
		CXIP_INFO("TX HW EQ saturated\n");
		return -FI_EAGAIN;
	}

#if ENABLE_DEBUG
	if (reduction->drop_send) {
		reduction->drop_send = false;
		goto drop_pkt;
	}
#endif

	if (md) {
		cmd.full_dma.command.opcode = C_CMD_PUT;
		cmd.full_dma.event_send_disable = 1;
		cmd.full_dma.event_success_disable = 1;
		cmd.full_dma.restricted = 1;
		cmd.full_dma.reduction = is_mcast;
		cmd.full_dma.index_ext = index_ext;
		cmd.full_dma.eq = cxip_evtq_eqn(ep_obj->coll.tx_evtq);
		cmd.full_dma.dfa = dfa;
		cmd.full_dma.lac = md->lac;
		cmd.full_dma.local_addr = CXI_VA_TO_IOVA(md, buffer);
		cmd.full_dma.request_len = buflen;

		/* this uses cached values, returns -FI_EAGAIN if queue full */
		ret = cxip_cmdq_cp_set(cmdq, ep_obj->auth_key.vni,
				       mc_obj->tc, mc_obj->tc_type);
		if (ret)
			goto err;

		ret = cxi_cq_emit_dma(cmdq->dev_cmdq, &cmd.full_dma);
	} else {
		cmd.c_state.event_send_disable = 1;
		cmd.c_state.event_success_disable = 1;
		cmd.c_state.restricted = 1;
		cmd.c_state.reduction = is_mcast;
		cmd.c_state.index_ext = index_ext;
		cmd.c_state.eq = cxip_evtq_eqn(ep_obj->coll.tx_evtq);
		cmd.c_state.initiator = CXI_MATCH_ID(
			ep_obj->domain->iface->dev->info.pid_bits,
			ep_obj->src_addr.pid, ep_obj->src_addr.nic);

		/* this uses cached values, returns -FI_EAGAIN if queue full */
		ret = cxip_cmdq_cp_set(cmdq, ep_obj->auth_key.vni,
				       mc_obj->tc, mc_obj->tc_type);
		if (ret)
			goto err;

		/* returns -FI_EAGAIN on failure */
		ret = cxip_cmdq_emit_c_state(cmdq, &cmd.c_state);
		if (ret) {
			ret = -FI_EAGAIN;
			goto err;
		}

		memset(&cmd.idc_put, 0, sizeof(cmd.idc_put));
		cmd.idc_put.idc_header.dfa = dfa;
		ret = cxi_cq_emit_idc_put(cmdq->dev_cmdq, &cmd.idc_put,
					  buffer, buflen);
		if (ret) {
			ret = -FI_EAGAIN;
			goto err;
		}
	}

	if (ret) {
		/* Return error according to Domain Resource Management */
		ret = -FI_EAGAIN;
		goto err;
	}

	cxi_cq_ring(cmdq->dev_cmdq);

#if ENABLE_DEBUG
drop_pkt:
#endif
	ret = FI_SUCCESS;
	ofi_atomic_inc32(&reduction->mc_obj->send_cnt);

err:
	return ret;
}

/****************************************************************************
 * RECV operation (of restricted Put to a local PTE)
 *
 * Collectives use a dedicated EP and PTE for each MC object.
 *
 * Packet space is allocated and linked to the PTE with a request. When a
 * packet is received, CXI hardware puts the request pointer and incoming
 * packet offset into a hardware-managed CXI event queue. When the CXI evtq
 * is progressed, completed hardware events are harvested, and the request
 * pointer (along with completion data) is inserted into an OFI CQ for the
 * endpont. Reading any OFI CQ bound to that endpoint will harvest all CXI
 * (hardware) evtqs bound to that endpoint, but will return only events
 * associated with the specified CQ, IF there are multiple CQs.
 *
 * Collectives services two CXI (hardware) evtqs for each MC object.
 *
 * The tx_evtq is only used to detect hardware buffer overflow, which
 * reflects -FI_EAGAIN back to the client.
 *
 * The rx_evtq manages PTE events for the collective endpoint. Buffer link
 * and unlink events are consumed silently: buffer exhaustion is checked on
 * every packet receipt, and will automatically recycle exhausted buffers.
 * PUT events are filtered for correct format and passed into the collective
 * state machine for processing. All other received packets are discarded.
 *
 * cxip_cq_req_complete() is used internally for PTE events, and externally
 * to report collective operation completions. The internal events are useful
 * for certain bench test models, where we need to count the packets received
 * as well as the collective completion. In production, we want to disable
 * the internal events. This is done independently for each MC object with
 * the mc->rx_discard flag.
 */

/* Report success/error results of an RX event through CQ/counters, and roll
 * over the buffers if appropriate.
 *
 * NOTE: req may be invalid after this call.
 *
 * Caller must hold ep_obj->lock.
 */
static void _coll_rx_req_report(struct cxip_req *req)
{
	size_t overflow;
	int err, ret;

	req->flags &= (FI_RECV | FI_COMPLETION);

	/* Interpret results */
	overflow = req->coll.hw_req_len - req->data_len;
	if (req->coll.cxi_rc == C_RC_OK && req->coll.isred && !overflow) {
		/* receive success */
		if (req->flags & FI_COMPLETION) {
			/* failure means progression is hung */
			ret = cxip_cq_req_complete(req);
			if (ret)
				CXIP_FATAL(
				    "cxip_cq_req_complete failed: %d\n", ret);
		}

		if (req->coll.coll_pte->ep_obj->coll.rx_cntr) {
			/* failure means counts cannot be trusted */
			ret = cxip_cntr_mod(
				req->coll.coll_pte->ep_obj->coll.rx_cntr, 1,
				false, false);
			if (ret)
				CXIP_WARN(
					"Failed success cxip_cntr_mod: %d\n",
					ret);
		}
	} else {
		/* failure */
		if (req->coll.cxi_rc != C_RC_OK) {
			/* real network error of some sort */
			err = proverr2errno(req->coll.cxi_rc);
			CXIP_WARN("Request error: %p (err: %d, %s)\n",
				  req, err, cxi_rc_to_str(err));
		} else if (overflow) {
			/* can only happen on very large packet (> 64 bytes) */
			err = FI_EMSGSIZE;
			CXIP_WARN("Request truncated: %p (err: %d, %s)\n",
				  req, err, cxi_rc_to_str(err));
		} else {
			/* non-reduction packet */
			err = FI_ENOMSG;
			CXIP_WARN("Not reduction pkt: %p (err: %d, %s)\n",
				  req, err, cxi_rc_to_str(err));
		}

		/* failure means progression is hung */
		ret = cxip_cq_req_error(req, overflow, err,
					req->coll.cxi_rc,
					NULL, 0, FI_ADDR_UNSPEC);
		if (ret)
			CXIP_FATAL("cxip_cq_req_error: %d\n", ret);

		if (req->coll.coll_pte->ep_obj->coll.rx_cntr) {
			/* failure means counts cannot be trusted */
			ret = cxip_cntr_mod(
				req->coll.coll_pte->ep_obj->coll.rx_cntr, 1,
				false, true);
			if (ret)
				CXIP_WARN("cxip_cntr_mod: %d\n", ret);
		}
	}

	/* manage buffer rollover */
	if (req->coll.mrecv_space <
	    req->coll.coll_pte->ep_obj->coll.min_multi_recv) {
		struct cxip_coll_pte *coll_pte = req->coll.coll_pte;
		struct cxip_coll_buf *buf = req->coll.coll_buf;
		int cnt;

		/* Will be re-incremented when LINK is received */
		cnt = ofi_atomic_dec32(&coll_pte->buf_cnt);
		if (req->coll.coll_pte->buf_low_water > cnt)
			req->coll.coll_pte->buf_low_water = cnt;
		if (cnt <= 0) {
			CXIP_WARN("COLL buffers exhausted\n");
			TRACE_DEBUG("COLL buffers exhausted\n");
			// TODO set flag to shut this down
		}
		ofi_atomic_inc32(&coll_pte->buf_swap_cnt);

		/* Re-use this buffer in the hardware */
		ret = _coll_append_buffer(coll_pte, buf);
		if (ret != FI_SUCCESS)
			CXIP_WARN("Re-link buffer failed: %d\n", ret);

		/* Hardware has silently unlinked this */
		cxip_evtq_req_free(req);
	}
}

/* Evaluate PUT receive request to see if this is a reduction packet */
static void _coll_rx_progress(struct cxip_req *req,
			      const union c_event *event)
{
	struct cxip_coll_mc *mc_obj;
	struct cxip_coll_reduction *reduction;
	struct red_pkt *pkt;

	TRACE_DEBUG("_coll_rx_progress entry\n");

	/* Raw packet of some sort received */
	ofi_atomic_inc32(&req->coll.coll_pte->recv_cnt);

	/* If not the right size, don't swap bytes */
	if (req->data_len != sizeof(struct red_pkt)) {
		CXIP_INFO("Bad coll packet size: %ld\n", req->data_len);
		return;
	}

	/* If swap doesn't look like reduction packet, swap back and discard */
	pkt = (struct red_pkt *)req->buf;
	_swappkt(pkt);
	if (pkt->hdr.cookie.magic != MAGIC)
	{
		TRACE_DEBUG("Bad coll MAGIC: %x\n", pkt->hdr.cookie.magic);
		CXIP_INFO("Bad coll MAGIC: %x\n", pkt->hdr.cookie.magic);
		_swappkt(pkt);
		return;
	}
	/* This is a reduction packet */

	/* The coll.coll_pte->mc_obj is defined only for COMM_KEY_RANK */
	mc_obj = req->coll.coll_pte->mc_obj;
	if (!mc_obj)
		mc_obj = ofi_idm_lookup(
				&req->coll.coll_pte->ep_obj->coll.mcast_map,
				pkt->hdr.cookie.mcast_id);
	if (!mc_obj) {
		TRACE_PKT("Bad coll lookup: %x\n", pkt->hdr.cookie.mcast_id);
		CXIP_INFO("Bad coll lookup: %x\n", pkt->hdr.cookie.mcast_id);
		return;
	}
	/* This is a valid reduction packet */
	ofi_atomic_inc32(&mc_obj->recv_cnt);
	req->coll.isred = true;
	req->discard = mc_obj->rx_discard;
	reduction = &mc_obj->reduction[pkt->hdr.cookie.red_id];

#if ENABLE_DEBUG
	/* Test case, simulate packet dropped in-flight */
	if (reduction->drop_recv) {
		reduction->drop_recv = false;
		return;
	}
#endif
	// A re-arm of an armed switch port drop this packet
	if (pkt->hdr.seqno == CXIP_COLL_MOD_SEQNO) {
		TRACE_PKT("pre-rearm pkt dropped\n");
		CXIP_INFO("pre-rearm pkt dropped\n");
		return;
	}

	TRACE_PKT("valid reduction packet from rank %u\n", pkt->sender_rank);

	/* Progress the reduction */
	_dump_red_pkt(pkt, "recv", "");
	ofi_atomic_inc32(&mc_obj->pkt_cnt);
	_progress_coll(reduction, pkt);
	TRACE_DEBUG("_coll_rx_progress return\n");
}

/* Event-handling callback for posted receive buffers */
static int _coll_recv_cb(struct cxip_req *req, const union c_event *event)
{
	req->coll.cxi_rc = cxi_tgt_event_rc(event);
	switch (event->hdr.event_type) {
	case C_EVENT_LINK:
		/* Enabled */
		if (req->coll.cxi_rc != C_RC_OK) {
			CXIP_WARN("LINK error rc: %d\n", req->coll.cxi_rc);
			break;
		}
		CXIP_DBG("LINK event seen\n");
		ofi_atomic_inc32(&req->coll.coll_pte->buf_cnt);
		break;
	case C_EVENT_UNLINK:
		/* Normally disabled, errors only */
		req->coll.cxi_rc = cxi_tgt_event_rc(event);
		if (req->coll.cxi_rc != C_RC_OK) {
			CXIP_WARN("UNLINK error rc: %d\n", req->coll.cxi_rc);
			break;
		}
		CXIP_DBG("UNLINK event seen\n");
		break;
	case C_EVENT_PUT:
		req->coll.isred = false;
		req->coll.cxi_rc = cxi_tgt_event_rc(event);
		if (req->coll.cxi_rc != C_RC_OK) {
			CXIP_WARN("PUT error rc: %d\n", req->coll.cxi_rc);
			break;
		}
		CXIP_DBG("PUT event seen\n");
		req->buf = (uint64_t)(CXI_IOVA_TO_VA(
					req->coll.coll_buf->cxi_md->md,
					event->tgt_long.start));
		req->coll.mrecv_space -= event->tgt_long.mlength;
		req->coll.hw_req_len = event->tgt_long.rlength;
		req->data_len = event->tgt_long.mlength;
		_coll_rx_progress(req, event);
		_coll_rx_req_report(req);
		break;
	default:
		req->coll.cxi_rc = cxi_tgt_event_rc(event);
		CXIP_WARN(CXIP_UNEXPECTED_EVENT,
			  cxi_event_to_str(event),
			  cxi_rc_to_str(req->coll.cxi_rc));
		break;
	}

	return FI_SUCCESS;
}

/* Inject a hardware LE append. Does not generate HW LINK event unless error. */
static int _hw_coll_recv(struct cxip_coll_pte *coll_pte, struct cxip_req *req)
{
	uint32_t le_flags;
	uint64_t recv_iova;
	int ret;

	/* C_LE_MANAGE_LOCAL makes Cassini ignore initiator remote_offset in all
	 * Puts, and causes automatic UNLINK when buffer capacity drops below
	 * CXIP_COLL_MIN_MULTI_RECV.
	 *
	 * C_LE_EVENT_UNLINK_DISABLE prevents generation of UNLINK events. We
	 * detect UNLINK by counting packets, and presume automatic UNLINK drops
	 * below CXIP_COLL_MIN_MULTI_RECV.
	 *
	 * C_LE_EVENT_UNLINK_DISABLE prevents UNLINK events from being
	 * generated. Hardware performs UNLINK automatically when buffer
	 * capacity is below CXIP_COLL_MIN_MULTI_RECV.
	 *
	 * C_LE_OP_PUT indicates this is an input buffer that responses to PUT.
	 *
	 * C_LE_NO_TRUNCATE is not used, because all packets are a fixed size,
	 * and CXIP_COLL_MIN_MULTI_RECV is sufficient to guarantee space for one new
	 * reduction packet.
	 */
	le_flags = C_LE_EVENT_UNLINK_DISABLE | C_LE_OP_PUT | C_LE_MANAGE_LOCAL;

	recv_iova = CXI_VA_TO_IOVA(req->coll.coll_buf->cxi_md->md,
				   (uint64_t)req->coll.coll_buf->buffer);

	ret = cxip_pte_append(coll_pte->pte,
			      recv_iova,
			      req->coll.coll_buf->bufsiz,
			      req->coll.coll_buf->cxi_md->md->lac,
			      C_PTL_LIST_PRIORITY,
			      req->req_id,
			      0, 0, 0,
			      req->coll.coll_pte->ep_obj->coll.min_multi_recv,
			      le_flags, coll_pte->ep_obj->coll.rx_cntr,
			      coll_pte->ep_obj->coll.rx_cmdq,
			      true);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("PTE append inject failed: %d\n", ret);
		return ret;
	}

	return FI_SUCCESS;
}

/* Append a receive buffer to the PTE, with callback to handle receives.
 *
 * Caller must hold ep_obj->lock.
 */
static ssize_t _coll_append_buffer(struct cxip_coll_pte *coll_pte,
				   struct cxip_coll_buf *buf)
{
	struct cxip_req *req;
	int ret;

	if (buf->bufsiz && !buf->buffer) {
		CXIP_WARN("No buffer\n");
		return -FI_EINVAL;
	}

	/* Allocate and populate a new request
	 * Sets:
	 * - req->cq
	 * - req->req_id to request index
	 * - req->req_ctx to passed context (buf)
	 * - req->discard to false
	 * - Inserts into the cq->req_list
	 */
	req = cxip_evtq_req_alloc(coll_pte->ep_obj->coll.rx_evtq, 1, buf);
	if (!req) {
		CXIP_WARN("Failed to allocate event queue request for "
			  "collective buffer (bufsiz=%zu)\n",
			  buf->bufsiz);
		ret = -FI_ENOMEM;
		goto recv_unmap;
	}

	/* CQ event fields, set according to fi_cq.3
	 *   - set by provider
	 *   - returned to user in completion event
	 * uint64_t context;	// operation context
	 * uint64_t flags;	// operation flags
	 * uint64_t data_len;	// received data length
	 * uint64_t buf;	// receive buf offset
	 * uint64_t data;	// receive REMOTE_CQ_DATA
	 * uint64_t tag;	// receive tag value on matching interface
	 * fi_addr_t addr;	// sender address (if known) ???
	 */

	/* Request parameters */
	req->type = CXIP_REQ_COLL;
	req->flags = (FI_RECV | FI_COMPLETION);
	req->cb = _coll_recv_cb;
	req->triggered = false;
	req->trig_thresh = 0;
	req->trig_cntr = NULL;
	req->context = (uint64_t)buf;
	req->data_len = 0;
	req->buf = (uint64_t)buf->buffer;
	req->data = 0;
	req->tag = 0;
	req->coll.coll_pte = coll_pte;
	req->coll.coll_buf = buf;
	req->coll.mrecv_space = req->coll.coll_buf->bufsiz;

	/* Returns FI_SUCCESS or FI_EAGAIN */
	ret = _hw_coll_recv(coll_pte, req);
	if (ret != FI_SUCCESS)
		goto recv_dequeue;

	return FI_SUCCESS;

recv_dequeue:
	cxip_evtq_req_free(req);

recv_unmap:
	cxip_unmap(buf->cxi_md);
	return ret;
}

/****************************************************************************
 * PTE management functions.
 */

/* PTE state-change callback */
 __attribute__((__unused__))
static void _coll_pte_cb(struct cxip_pte *pte, const union c_event *event)
{
	switch (pte->state) {
	case C_PTLTE_ENABLED:
	case C_PTLTE_DISABLED:
		break;
	default:
		CXIP_FATAL("Unexpected state received: %u\n", pte->state);
	}
}

/* Enable a collective PTE. Wait for completion. */
static inline
int _coll_pte_enable(struct cxip_coll_pte *coll_pte, uint32_t drop_count)
{
	return cxip_pte_set_state_wait(coll_pte->pte,
				       coll_pte->ep_obj->coll.rx_cmdq,
				       coll_pte->ep_obj->coll.rx_evtq,
				       C_PTLTE_ENABLED, drop_count);
}

/* Disable a collective PTE. Wait for completion */
static inline
int _coll_pte_disable(struct cxip_coll_pte *coll_pte)
{
	return cxip_pte_set_state_wait(coll_pte->pte,
				       coll_pte->ep_obj->coll.rx_cmdq,
				       coll_pte->ep_obj->coll.rx_evtq,
				       C_PTLTE_DISABLED, 0);
}

/* Destroy and unmap all buffers used by the collectives PTE.
 *
 * Caller must hold ep_obj->lock.
 */
static void _coll_destroy_buffers(struct cxip_coll_pte *coll_pte)
{
	struct dlist_entry *list = &coll_pte->buf_list;
	struct cxip_coll_buf *buf;

	while (!dlist_empty(list)) {
		dlist_pop_front(list, struct cxip_coll_buf, buf, buf_entry);
		cxip_unmap(buf->cxi_md);
		free(buf);
	}
}

/* Adds 'count' buffers of 'size' bytes to the collecives PTE. This succeeds
 * fully, or it fails and removes all added buffers.
 */
static int _coll_add_buffers(struct cxip_coll_pte *coll_pte, size_t size,
			     size_t count)
{
	struct cxip_coll_buf *buf;
	int ret, i;

	if (count < CXIP_COLL_MIN_RX_BUFS) {
		CXIP_WARN("Buffer count %ld < minimum (%d)\n",
			  count, CXIP_COLL_MIN_RX_BUFS);
		return -FI_EINVAL;
	}

	if (size < CXIP_COLL_MIN_RX_SIZE) {
		CXIP_WARN("Buffer size %ld < minimum (%d)\n",
			  size, CXIP_COLL_MIN_RX_SIZE);
		return -FI_EINVAL;
	}

	CXIP_DBG("Adding %ld buffers of size %ld\n", count, size);
	for (i = 0; i < count; i++) {
		buf = calloc(1, sizeof(*buf) + size);
		if (!buf) {
			CXIP_WARN("Failed to allocate collective buffer %d of "
				  "%zu (size=%zu)\n",
				  i, count, sizeof(*buf) + size);
			ret = -FI_ENOMEM;
			goto out;
		}
		ret = cxip_ep_obj_map(coll_pte->ep_obj, (void *)buf->buffer,
				      size, CXI_MAP_READ | CXI_MAP_WRITE, 0,
				      &buf->cxi_md);
		if (ret) {
			CXIP_WARN("Failed to map collective buffer %d of %zu "
				  "(ret=%d, size=%zu)\n",
				  i, count, ret, size);
			goto del_msg;
		}
		buf->bufsiz = size;
		dlist_insert_tail(&buf->buf_entry, &coll_pte->buf_list);

		ret = _coll_append_buffer(coll_pte, buf);
		if (ret) {
			CXIP_WARN("Add buffer %d of %ld: %d\n",
				  i, count, ret);
			goto out;
		}
	}
	/* Block until PTE completes buffer appends */
	do {
		sched_yield();
		cxip_evtq_progress(coll_pte->ep_obj->coll.rx_evtq, true);
	} while (ofi_atomic_get32(&coll_pte->buf_cnt) < count);
	coll_pte->buf_low_water = (int)count;

	return FI_SUCCESS;
del_msg:
	free(buf);
out:
	_coll_destroy_buffers(coll_pte);
	return ret;
}

/****************************************************************************
 * Mathematical routines used for collective reductions.
 */

/* Set RC only if new is higher priority than old */
// TODO avoid branch:
// http://geeksforgeeks.org/
//     compute-the-minimum-or-maximum-of-two-integers-without-branching
#define SET_RED_RC(redrc, rc) do {if ((redrc)<(rc)) (redrc)=(rc);} while(0)

static inline
bool cxip_is_snan64(double d)
{
	/* This detection is universal IEEE */
	return isnan(d) && !(_dbl2bits(d) & 0x0008000000000000);
}

/* convert signalling NaN to quiet NaN */
static inline
bool _quiesce_nan(double *d)
{
	if (!cxip_is_snan64(*d))
		return false;
	*d = NAN;
	return true;
}

/**
 * Implement NaN comparisons FLT_MINNUM and FLT_MAXNUM
 *
 * Only associative mode is supported. The old IEEE mode is incorrect, and has
 * been deprecated.
 *
 * Compares two doubles, replaces *d1 as appropriate, and indicates swap.
 *
 * If the values are normal doubles, less=true indicates we are looking for the
 * lesser of the two values, while less=false indicates we are looking for the
 * greater of the two values. The appropriate value will be swapped into *d1 if
 * necessary.
 *
 * In general, this will give preference to real values over NaN, which is the
 * opposite of swpnan1() above. This will only return NaN if BOTH values in
 * the comparison are NaN.
 *
 * If either NaN is sNaN, this will set the CXIP_COLL_RC_FLT_INVALID error.
 *
 * The return value can be used when associating an index with the value.
 *
 * Note that since this quiets any signalling NaNs, we need set the
 * CXIP_COLL_RC_FLT_INVALID error.
 *
 * - return  0 indicates the values are equivalent, so use the smallest index.
 * - return +1 indicates the values were swapped, so use the second index.
 * - return -1 indicates no swap, so use the first index.
 */
static int swpnan2(double *d1, double d2, bool less, cxip_coll_rc_t *rc)
{
	bool nan1, nan2, snan1, snan2;

	// isnan() does not distinguish sNaN from qNaN
	nan1 = isnan(*d1);
	nan2 = isnan(d2);
	// Neither is NaN, so simple comparison
	if (!nan1 && !nan2) {
		if (*d1 == d2)
			return 0;
		if (less && (*d1 > d2)) {
			*d1 = d2;
			return 1;
		}
		if (!less && (*d1 < d2)) {
			*d1 = d2;
			return 1;
		}
		return -1;
	}

	// ----- FLT_MINNUM and FLT_MAXNUM rules
	// At least one is NaN, check for sNaN
	snan1 = _quiesce_nan(d1);
	snan2 = _quiesce_nan(&d2);
	if (snan1 || snan2)
		SET_RED_RC(*rc, CXIP_COLL_RC_FLT_INVALID);

	// return qNaN only if both are NaN
	if (nan1 && nan2)
		return 0;

	// Prefers number
	if (nan1) {
		*d1 = d2;
		return 1;
	}
	// Prefers number
	return -1;
}

/* Companion to swpnan1() and swpnan2() to swap associated indices */
static inline
void swpidx(uint64_t *i1, uint64_t i2, int swp)
{
	if (swp >= 0 && (swp > 0 || *i1 > i2))
		*i1 = i2;
}

static inline
void _dump_coll_data(const char *tag, const struct cxip_coll_data *coll_data)
{
#if __trc_data
	int i;

	TRACE_PKT("=== Coll data: %s\n", tag);
	TRACE_PKT("  init    = %d\n", coll_data->initialized);
	TRACE_PKT("  red_op  = %d\n", coll_data->red_op);
	TRACE_PKT("  red_rc  = %d\n", coll_data->red_rc);
	TRACE_PKT("  red_cnt = %d\n", coll_data->red_cnt);
	TRACE_PKT("  data:\n");
	for (i = 0; i < 4; i++)
		TRACE_PKT(" %016lx\n", coll_data->intval.ival[i]);
	TRACE_PKT("\n");
	TRACE_PKT("===================\n");
#endif
}

/* initialize coll_data structure from raw user data */
static void _init_coll_data(struct cxip_coll_data *coll_data, int opcode,
			    const void *user_data, int bytcnt)
{
	double d;
	int i;

	/* NOTE: snan can be directly injected here */
	memset(coll_data, 0, sizeof(*coll_data));
	if (user_data)
		memcpy(coll_data->databuf, user_data, bytcnt);
	coll_data->red_rc = 0;
	coll_data->red_cnt = 1;
	coll_data->red_op = opcode;
	switch (coll_data->red_op) {
	case COLL_OPCODE_FLT_MINNUM:
	case COLL_OPCODE_FLT_MAXNUM:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND0:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND1:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND2:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND3:
	case COLL_OPCODE_FLT_SUM_FTZ_RND0:
	case COLL_OPCODE_FLT_SUM_FTZ_RND1:
	case COLL_OPCODE_FLT_SUM_FTZ_RND2:
	case COLL_OPCODE_FLT_SUM_FTZ_RND3:
		/* evaluate all four doubles */
		for (i = 0; i < 4; i++) {
			if (cxip_is_snan64(coll_data->fltval.fval[i]))
				SET_RED_RC(coll_data->red_rc,
					   CXIP_COLL_RC_FLT_INVALID);
			if (isnan(coll_data->fltval.fval[i]) ||
			    isinf(coll_data->fltval.fval[i]))
				SET_RED_RC(coll_data->red_rc,
					   CXIP_COLL_RC_FLT_OVERFLOW);
		}
		break;
	case COLL_OPCODE_FLT_MINMAXNUMLOC:
		/* evaluate the two doubles */
		for (i = 0; i < 4; i += 2) {
			if (cxip_is_snan64(coll_data->fltval.fval[i]))
				SET_RED_RC(coll_data->red_rc,
				CXIP_COLL_RC_FLT_INVALID);
			if (isinf(coll_data->fltval.fval[i]))
				SET_RED_RC(coll_data->red_rc,
					   CXIP_COLL_RC_FLT_OVERFLOW);
		}
		break;
	case COLL_OPCODE_FLT_REPSUM:
		/* perform the conversion */
		d = coll_data->fltval.fval[0];
		cxip_dbl_to_rep(&coll_data->repsum, d);
		break;
	}
	coll_data->initialized = true;
}

/* reduce data into accumulator - can be used on uninitialized accumulator */
static void _reduce(struct cxip_coll_data *accum,
		    const struct cxip_coll_data *coll_data,
		    bool pre_reduce)
{
	int i, swp;

	TRACE_DEBUG("%s entry\n", __func__);
	/* Initialize with new data */
	if (!accum->initialized) {
		TRACE_DEBUG("%s !accum->initialize\n", __func__);
		memcpy(accum, coll_data, sizeof(*accum));
		return;
	}

	/* copy new error (if any) to accumulator */
	SET_RED_RC(accum->red_rc, coll_data->red_rc);

	/* Real reduction (send or receive) must count contributions.
	 */
	if (!pre_reduce) {
		TRACE_DEBUG("%s counting contribtions %d += %d\n",
			__func__, accum->red_cnt, coll_data->red_cnt);
		accum->red_cnt += coll_data->red_cnt;
	}

	/* ops must always match, else don't apply data */
	if (accum->red_op != coll_data->red_op) {
		TRACE_DEBUG("%s OP MISMATCH, %d vs %d\n",
			__func__, accum->red_op, coll_data->red_op);
		SET_RED_RC(accum->red_rc, CXIP_COLL_RC_OP_MISMATCH);
		return;
	}

	TRACE_DEBUG("%s Performing reduction in sw\n", __func__);
	/* Perform the reduction in software */
	switch (accum->red_op) {
	case COLL_OPCODE_BARRIER:
		break;
	case COLL_OPCODE_BIT_AND:
		for (i = 0; i < 4; i++)
			accum->intval.ival[i] &= coll_data->intval.ival[i];
		/* overflow not possible */
		break;
	case COLL_OPCODE_BIT_OR:
		for (i = 0; i < 4; i++)
			accum->intval.ival[i] |= coll_data->intval.ival[i];
		/* overflow not possible */
		break;
	case COLL_OPCODE_BIT_XOR:
		for (i = 0; i < 4; i++)
			accum->intval.ival[i] ^= coll_data->intval.ival[i];
		/* overflow not possible */
		break;
#if 0 /* keep for future use */	
	case COLL_OPCODE_LOG_AND:
		for (i = 0; i < 4; i++)
			accum->intval.ival[i] =	(accum->intval.ival[i] &&
			                         coll_data->intval.ival[i]);
		/* overflow not possible */
		break;
	case COLL_OPCODE_LOG_OR:
		for (i = 0; i < 4; i++)
			accum->intval.ival[i] =	(accum->intval.ival[i] ||
			                         coll_data->intval.ival[i]);
		/* overflow not possible */
		break;
	case COLL_OPCODE_LOG_XOR:
		for (i = 0; i < 4; i++)
		        accum->intval.ival[i] = ((accum->intval.ival[i] &&
                                                  !coll_data->intval.ival[i])
                                                  || (!accum->intval.ival[i] &&
                                                  coll_data->intval.ival[i]));
		/* overflow not possible */
		break;
#endif		
	case COLL_OPCODE_INT_MIN:
		for (i = 0; i < 4; i++)
			if (accum->intval.ival[i] > coll_data->intval.ival[i])
				accum->intval.ival[i] = coll_data->intval.ival[i];
		/* overflow not possible */
		break;
	case COLL_OPCODE_INT_MAX:
		for (i = 0; i < 4; i++)
			if (accum->intval.ival[i] < coll_data->intval.ival[i])
				accum->intval.ival[i] = coll_data->intval.ival[i];
		/* overflow not possible */
		break;
	case COLL_OPCODE_INT_MINMAXLOC:
		/* return smallest value and its index */
		if (accum->intminmax.iminval > coll_data->intminmax.iminval) {
			accum->intminmax.iminval = coll_data->intminmax.iminval;
			accum->intminmax.iminidx = coll_data->intminmax.iminidx;
		} else
		/* return smallest index if values equal */
		if (accum->intminmax.iminval == coll_data->intminmax.iminval &&
		    accum->intminmax.iminidx > coll_data->intminmax.iminidx) {
			accum->intminmax.iminidx = coll_data->intminmax.iminidx;
		}

		/* return largest value and its index */
		if (accum->intminmax.imaxval < coll_data->intminmax.imaxval) {
			accum->intminmax.imaxval = coll_data->intminmax.imaxval;
			accum->intminmax.imaxidx = coll_data->intminmax.imaxidx;
		} else
		/* return smallest (yes) index if values equal */
		if (accum->intminmax.imaxval == coll_data->intminmax.imaxval &&
		    accum->intminmax.imaxidx > coll_data->intminmax.imaxidx) {
			accum->intminmax.imaxidx = coll_data->intminmax.imaxidx;
		}
		/* overflow not possible */
		break;
	case COLL_OPCODE_INT_SUM:
		for (i = 0; i < 4; i++) {
			bool newneg = (coll_data->intval.ival[i] < 0);
			bool oldneg = (accum->intval.ival[i] < 0);
			bool sumneg;
			accum->intval.ival[i] += coll_data->intval.ival[i];
			sumneg = (accum->intval.ival[i] < 0);
			/* if sum changed sign, and doesn't match new sign */
			if (sumneg != oldneg && sumneg != newneg)
				SET_RED_RC(accum->red_rc,
					   CXIP_COLL_RC_INT_OVERFLOW);
		}
		break;
	case COLL_OPCODE_FLT_MINNUM:
		for (i = 0; i < 4; i++) {
			swpnan2(&accum->fltval.fval[i], coll_data->fltval.fval[i], 1,
				&accum->red_rc);
		}
		break;
	case COLL_OPCODE_FLT_MAXNUM:
		for (i = 0; i < 4; i++) {
			swpnan2(&accum->fltval.fval[i], coll_data->fltval.fval[i], 0,
				&accum->red_rc);
		}
		break;
	case COLL_OPCODE_FLT_MINMAXNUMLOC:
		swp = swpnan2(&accum->fltminmax.fminval,
			      coll_data->fltminmax.fminval, 1, &accum->red_rc);
		swpidx(&accum->fltminmax.fminidx, coll_data->fltminmax.fminidx, swp);
		swp = swpnan2(&accum->fltminmax.fmaxval,
			      coll_data->fltminmax.fmaxval, 0, &accum->red_rc);
		swpidx(&accum->fltminmax.fmaxidx, coll_data->fltminmax.fmaxidx, swp);
		break;
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND0:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND1:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND2:
	case COLL_OPCODE_FLT_SUM_NOFTZ_RND3:
		/* Rosetta opcode has been chosen according to the current
		 * rounding mode for this application, so all we need to do is
		 * add the numbers.
		 */
		for (i = 0; i < 4; i++) {
			/* NOTE: arithmetic operations will quiesce snan */
			accum->fltval.fval[i] +=  coll_data->fltval.fval[i];

			if (isinf(accum->fltval.fval[i]))
				SET_RED_RC(accum->red_rc,
					   CXIP_COLL_RC_FLT_OVERFLOW);
		}
		break;
	case COLL_OPCODE_FLT_SUM_FTZ_RND0:
	case COLL_OPCODE_FLT_SUM_FTZ_RND1:
	case COLL_OPCODE_FLT_SUM_FTZ_RND2:
	case COLL_OPCODE_FLT_SUM_FTZ_RND3:
		/* Rosetta opcode has been chosen according to the current
		 * rounding mode for this application, so all we need to do is
		 * add the numbers.
		 */
		for (i = 0; i < 4; i++) {
			/* NOTE: arithmetic operations will quiesce snan */
			accum->fltval.fval[i] +=  coll_data->fltval.fval[i];

			if (isinf(accum->fltval.fval[i]))
				SET_RED_RC(accum->red_rc,
					   CXIP_COLL_RC_FLT_OVERFLOW);
		}
		break;
	case COLL_OPCODE_FLT_REPSUM:
		cxip_rep_add(&accum->repsum, &coll_data->repsum);
		break;
	}
}

/****************************************************************************
 * Reduction packet management.
 */

/**
 * Prevent setting the ARM bit on a root packet.
 *
 * This is used in testing to suppress Rosetta collective operations, forcing
 * all leaf packets to arrive at the root, creating an incast.
 */
int cxip_coll_arm_disable(struct fid_mc *mc, bool disable)
{
	struct cxip_coll_mc *mc_obj = (struct cxip_coll_mc *)mc;
	int old = mc_obj->arm_disable;

	mc_obj->arm_disable = disable;

	return old;
}

/**
 * Limit the reduction ID values.
 *
 * Reduction ID values do round-robin over an adjustable range of values. This
 * is useful in testing to force all reductions to use reduction id zero (set
 * max_red_id to 1), but could be used in production to use only a subset of
 * reduction IDs to limit fabric resource exhaustion when concurrent reductions
 * are used.
 */
void cxip_coll_limit_red_id(struct fid_mc *mc, int max_red_id)
{
	struct cxip_coll_mc *mc_obj = (struct cxip_coll_mc *)mc;

	if (max_red_id < 1)
		max_red_id = 1;
	if (max_red_id > CXIP_COLL_MAX_CONCUR)
		max_red_id = CXIP_COLL_MAX_CONCUR;
	mc_obj->max_red_id = max_red_id;
}

/* drop the next packet sent */
void cxip_coll_drop_send(struct cxip_coll_reduction *reduction)
{
	reduction->drop_send = true;
}

/* drop the next packet received */
void cxip_coll_drop_recv(struct cxip_coll_reduction *reduction)
{
	reduction->drop_recv = true;
}

/* Return true if this node is the hwroot node */
static inline
bool is_hw_root(struct cxip_coll_mc *mc_obj)
{
	return (mc_obj->hwroot_idx == mc_obj->mynode_idx);
}

/* Simulated unicast send of multiple packets as root node to leaf nodes */
static inline
ssize_t _send_pkt_as_root(struct cxip_coll_reduction *reduction)
{
	int i, ret, err;

	err = 0;
	for (i = 0; i < reduction->mc_obj->av_set_obj->fi_addr_cnt; i++) {
		if (i == reduction->mc_obj->mynode_idx &&
		    reduction->mc_obj->av_set_obj->fi_addr_cnt > 1) {
			TRACE_DEBUG("root: skip=%d\n", i);
			continue;
		}
		ret = cxip_coll_send(reduction, i,
				     reduction->tx_msg,
				     sizeof(struct red_pkt),
				     reduction->mc_obj->reduction_md);
		TRACE_DEBUG("root: send=%d ret=%d\n", i, ret);
		if (!err)
			err = ret;
	}
	return err;
}

/* Simulated unicast send of single packet as leaf node to root node */
static inline
ssize_t _send_pkt_as_leaf(struct cxip_coll_reduction *reduction)
{
	int ret;

	ret = cxip_coll_send(reduction, reduction->mc_obj->hwroot_idx,
			      reduction->tx_msg, sizeof(struct red_pkt),
			      reduction->mc_obj->reduction_md);
	TRACE_DEBUG("leaf: send=%d ret=%d\n", 1, ret);
	return ret;
}

/* Multicast send of single packet from root or leaf node */
static inline
ssize_t _send_pkt_mc(struct cxip_coll_reduction *reduction)
{
	int ret;

	ret = cxip_coll_send(reduction, 0, reduction->tx_msg,
			     sizeof(struct red_pkt),
			     reduction->mc_obj->reduction_md);
	TRACE_DEBUG("mcast: send=%d ret=%d\n", 1, ret);
	return ret;
}

/* Send packet from root or leaf node as appropriate */
static inline
ssize_t _send_pkt(struct cxip_coll_reduction *reduction)
{
	int ret;

	if (reduction->mc_obj->is_multicast) {
		ret = _send_pkt_mc(reduction);
	} else if (is_hw_root(reduction->mc_obj)) {
		ret = _send_pkt_as_root(reduction);
	} else {
		ret = _send_pkt_as_leaf(reduction);
	}
	return ret;
}

/* prepare and issue the reduction packet */
int cxip_coll_send_red_pkt(struct cxip_coll_reduction *reduction,
			   const struct cxip_coll_data *coll_data,
			   bool arm, bool retry, bool root_result_pkt)
{
	struct red_pkt *pkt;
	int ret = FI_SUCCESS;
	struct cxip_coll_mc *mc_obj = reduction->mc_obj;
	struct red_pkt_64 *rdma_get_pkt_p;
	uint8_t *copy_pntr;
	int red_id = reduction->red_id;

	pkt = (struct red_pkt *)reduction->tx_msg;

	memset(&pkt->hdr, 0, sizeof(pkt->hdr));
	pkt->hdr.arm = arm;
	pkt->hdr.retry = retry;
	pkt->hdr.seqno = reduction->seqno;
	pkt->hdr.resno = reduction->resno;
	pkt->hdr.cookie.mcast_id = reduction->mc_obj->mcast_addr;
	pkt->hdr.cookie.red_id = reduction->red_id;
	pkt->hdr.cookie.magic = MAGIC;
	pkt->sender_rank = reduction->mc_obj->mynode_idx;

	if (coll_data) {
		pkt->hdr.redcnt = coll_data->red_cnt;
		pkt->hdr.op = coll_data->red_op;
		pkt->hdr.red_rc = coll_data->red_rc;
		/* repsum has some additional information that must be set */
		if (_opcode_to_redtype(coll_data->red_op) == REDTYPE_REPSUM) {
			pkt->hdr.repsum_m = coll_data->repsum.M;
			pkt->hdr.repsum_ovflid = coll_data->repsum.overflow_id;
		}
		memcpy(pkt->data, &coll_data->databuf, CXIP_COLL_MAX_DATA_SIZE);
	} else {
		pkt->hdr.redcnt = 0;
		pkt->hdr.op = 0;
		pkt->hdr.red_rc = 0;
		pkt->hdr.repsum_m = 0;
		pkt->hdr.repsum_ovflid = 0;
		memset(pkt->data, 0, CXIP_COLL_MAX_DATA_SIZE);
	}
	if(root_result_pkt) {
		if(mc_obj->root_rdma_get_data_p) {
			rdma_get_pkt_p = ((struct red_pkt_64 *)mc_obj->root_rdma_get_data_p);
			/* index into the base buffer */
			copy_pntr = (uint8_t *)&rdma_get_pkt_p[red_id].pkt;
			TRACE_JOIN("%s: rdma copy pkt red_id %d op %d seqno %d\n",
				__func__, red_id, pkt->hdr.op, pkt->hdr.seqno);
			/* save a copy for the leaf in the rdma_get buffer */
			memcpy(copy_pntr,pkt,sizeof(struct red_pkt));
			_dump_red_pkt(pkt, "root rdma copy packet", "rdma");
		} else
			TRACE_JOIN("%s: root NULL rdma get data pointer!\n",
				__func__);
	}
	// A re-arm of an armed switch port send clearing packet
	if (arm && retry) {
		int save_seqno = pkt->hdr.seqno;

		// A re-arm of an armed switch port skip illegal value
		pkt->hdr.seqno = CXIP_COLL_MOD_SEQNO;
		_dump_red_pkt(pkt, "retry", "");
		TRACE_DEBUG("arm/retry send op = %d seqno = %d red_id = %d\n",
			pkt->hdr.op, reduction->seqno, reduction->red_id);
		_swappkt(pkt);
		do {
			/* -FI_EAGAIN means HW queue is full, self-clears */
			ret = _send_pkt(reduction);
		} while (ret == -FI_EAGAIN);
		_swappkt(pkt);
		pkt->hdr.seqno = save_seqno;
	}

	if (ret == FI_SUCCESS) {
		_dump_red_pkt(pkt, "send", "");
		TRACE_DEBUG("normal send op = %d seqno = %d red_id = %d\n",
			pkt->hdr.op, reduction->seqno, reduction->red_id);
		_swappkt(pkt);
		do {
			/* -FI_EAGAIN means HW queue is full, self-clears */
			ret = _send_pkt(reduction);
		} while (ret == -FI_EAGAIN);
		_swappkt(pkt);
	}

	/* any other error is a serious config/hardware issue */
	if (ret) {
		TRACE_DEBUG("Fatal send error = %d\n", ret);
		CXIP_WARN("Fatal send error = %d\n", ret);
	}

	return ret;
}

/* Post a reduction completion request to the collective completion queue */
static void _post_coll_complete(struct cxip_coll_reduction *reduction)
{
	struct cxip_req *req;
	int ret, prov;

	TRACE_DEBUG("%s Entering\n", __func__);

	/* Indicates collective completion by writing to the endpoint TX CQ */
	req = reduction->op_inject_req;
	if (!req) {
		TRACE_DEBUG("!req\n");
		return;
	}

	/* convert Rosetta return codes to CXIP return codes */
	if (reduction->accum.red_rc == CXIP_COLL_RC_SUCCESS ||
	    reduction->accum.red_rc == CXIP_COLL_RC_FLT_INEXACT ||
	    reduction->accum.red_rc == CXIP_COLL_RC_FLT_INVALID ||
	    reduction->accum.red_rc == CXIP_COLL_RC_FLT_OVERFLOW) {
		switch (reduction->accum.red_rc) {
		case CXIP_COLL_RC_FLT_INEXACT:
			CXIP_WARN("Coll reduce FLT result was rounded\n");
			break;
		case CXIP_COLL_RC_FLT_INVALID:
			CXIP_WARN("Coll reduce FLT invalid\n");
			break;
		case CXIP_COLL_RC_FLT_OVERFLOW:
			CXIP_WARN("Coll reduce FLT overflow\n");
			break;
		default:
			break;
		}
		ret = cxip_cq_req_complete(req);
	} else {
		switch (reduction->accum.red_rc) {
		case CXIP_COLL_RC_INT_OVERFLOW:
			prov = FI_CXI_ERRNO_RED_INT_OVERFLOW;
			break;
		case CXIP_COLL_RC_CONTR_OVERFLOW:
			prov = FI_CXI_ERRNO_RED_CONTR_OVERFLOW;
			break;
		case CXIP_COLL_RC_OP_MISMATCH:
			prov = FI_CXI_ERRNO_RED_OP_MISMATCH;
			break;
		case CXIP_COLL_RC_RDMA_FAILURE:
			prov = FI_CXI_COLL_RC_RDMA_FAILURE;
			break;
		case CXIP_COLL_RC_RDMA_DATA_FAILURE:
			prov = FI_CXI_COLL_RC_RDMA_DATA_FAILURE;
			break;
		default:
			prov = FI_CXI_ERRNO_RED_OTHER;
			break;
		}
		ret = cxip_cq_req_error(req, 0, -FI_EOTHER, prov,
					NULL, 0, FI_ADDR_UNSPEC);
	}
	if (ret) {
		TRACE_DEBUG("Attempt to post completion failed %s\n",
			   fi_strerror(-ret));
		CXIP_FATAL("Attempt to post completion failed %s\n",
			   fi_strerror(-ret));
	}

	/* req structure no longer needed */
	cxip_evtq_req_free(req);

	/* restore reduction object to usable state */
	dlist_init(&reduction->tmout_link);
	memset(&reduction->backup, 0, sizeof(reduction->backup));
	reduction->accum.initialized = false;
	reduction->in_use = false;
	reduction->completed = false;
	reduction->pktsent = false;
	reduction->accum.red_rc = CXIP_COLL_RC_SUCCESS;
	reduction->op_inject_req = NULL;
	reduction->rdma_get_sent = false;
	reduction->rdma_get_completed = false;
	reduction->leaf_contrib_start_us = 0;
	reduction->rdma_get_cb_rc = 0;
}

/* unpack reduction data from a reduction packet */
static void _unpack_red_data(struct cxip_coll_data *coll_data,
			     const struct red_pkt *pkt)
{
	memcpy(coll_data->databuf, pkt->data, 32);
	coll_data->repsum.M = pkt->hdr.repsum_m;
	coll_data->repsum.overflow_id = pkt->hdr.repsum_ovflid;
	coll_data->red_op = pkt->hdr.op;
	coll_data->red_cnt = pkt->hdr.redcnt;
	coll_data->red_rc = pkt->hdr.red_rc;
	coll_data->initialized = true;
}

/****************************************************************************
 * Collective State Machine
 *
 * The basic flow is:
 *   - all nodes reach a common reduction call (at different times)
 *   - leaf nodes send their data, to be reduced, and block, polling CQ
 *   - root node prepares for the reduction, and blocks, polling CQ
 *   - root node receives leaf packets and reduces them, until all received
 *   - root node sends Arm Packet with final result, and unblocks
 *   - leaf nodes receive Arm Packet with final result, and unblock
 *
 * The Rosetta acceleration comes from the Arm Packet, which speculatively arms
 * the Rosetta tree for the NEXT operation. This persists until a timeout
 * expires. The timeout is specified when the multicast tree is created by the
 * Rosetta configuration service, and cannot be modified after join is complete.
 *
 * If the next collective operation occurs within the timeout, the leaf results
 * will be reduced in reduction engines by Rosetta as they move up the tree,
 * reducing the number of packets received by the root.
 *
 * If the reduction engine times out with partial results, it forwards the
 * partial results, and all subsequent results are passed directly to the next
 * Rosetta.
 *
 * The first leaf contribution to reach a reduction engine establishes the
 * reduction operation. All subsequent contributions must use the same
 * operation, or Rosetta returns an error.
 *
 * There are eight reduction_id values, which can be used to acquire and use up
 * to eight independent reduction engines (REs) at each upstream port of each
 * Rosetta switch in the collective tree.
 *
 * We use a round-robin selection of reduction id values. There is a small race
 * condition among the leaf nodes as the result is distributed from the root. If
 * another reduction were to be initiated during this race, the leaf nodes would
 * be in disagreement as to which reduction IDs were free for the new reduction.
 * To avoid this, we use a deterministic algorithm (round-robin) so that the
 * "next" reduction id is always predetermined for each reduction.
 *
 * Ordering of requests and responses will the same on all nodes.
 *
 * Ordering of requests is required of the application. If requests are ordered
 * differently on different nodes, results are undefined, and it is considered
 * an application error.
 *
 * Ordering of responses is guaranteed by the mc_obj->tail_red_id value, which
 * is advanced after the reduction completes. This ordering is required to
 * ensure that the round-robin is observed.
 */

/* modular increment/decrement */
#define INCMOD(val, mod)	do {(val)=((val)+1)%(mod);} while (0)
#define DECMOD(val, mod)	do {(val)=((val)+(mod)-1)%(mod);} while (0)

/* MONOTONIC timestamp operations for timeouts/retries */

/* get current time */
static inline
void _tsget(struct timespec *ts)
{
	uint64_t ns;

	ns = ofi_gettime_ns();
	ts->tv_sec = ns / 1000000000;
	ts->tv_nsec = ns % 1000000000;
}

/* advance time by delta */
static inline
void _tsadd(struct timespec *ts, const struct timespec *dt)
{
	ts->tv_sec += dt->tv_sec;
	ts->tv_nsec += dt->tv_nsec;
	if (ts->tv_nsec >= 1000000000L) {
		ts->tv_sec += 1;
		ts->tv_nsec -= 1000000000L;
	}
}

/* set current time plus increment */
static inline
void _tsset(struct timespec *ts, const struct timespec *dt)
{
	_tsget(ts);
	_tsadd(ts, dt);
}

/* test for expiration of time */
static inline
bool _tsexp(struct timespec *ts)
{
	struct timespec tsnow;

	_tsget(&tsnow);
	if (ts->tv_sec == TIMER_UNSET)
		return false;
	if (tsnow.tv_sec < ts->tv_sec)
		return false;
	if (tsnow.tv_sec > ts->tv_sec)
		return true;
	return (tsnow.tv_nsec >= ts->tv_nsec);
}

/* Clear reduction expiration time */
static inline
void _ts_red_clr(struct cxip_coll_reduction *reduction)
{
	TRACE_DEBUG("clearing timer on red_id %d, seqno %d\n",
		    reduction->red_id, reduction->seqno);
	dlist_remove(&reduction->tmout_link);
	reduction->tv_expires.tv_sec = TIMER_UNSET;
	reduction->tv_expires.tv_nsec = TIMER_UNSET;
}

/* Set reduction expiration time (future) */
static inline
void _ts_red_set(struct cxip_coll_reduction *reduction,
		 struct timespec *expires, struct dlist_entry *retry_list)
{
	TRACE_DEBUG("setting timer on red_id %d, seqno %d\n",
		    reduction->red_id, reduction->seqno);
	_tsset(&reduction->tv_expires, expires);
	dlist_insert_tail(&reduction->tmout_link, retry_list);
}

static inline
void _set_arm_expires(struct cxip_coll_reduction *reduction) {
	struct timespec arm_expires;

	arm_expires.tv_sec = cxip_env.coll_timeout_usec / 1000000;
	arm_expires.tv_nsec = cxip_env.coll_timeout_usec % 1000000;

	/* Re-arm half-way through the timeout period to ensure we
	 * re-arm before it expires. */
	arm_expires.tv_sec /= 2;
	arm_expires.tv_nsec /= 2;

	/* get current time */
	_tsget(&reduction->arm_expires);

	TRACE_DEBUG("arm_expires: %ld.%ld\n", arm_expires.tv_sec,
		    arm_expires.tv_nsec);
	_tsset(&reduction->arm_expires, &arm_expires);
}

/* Used to prevent incast storm */
static inline
bool _need_to_arm(struct cxip_coll_reduction *reduction)
{
	if (_tsexp(&reduction->arm_expires)) {
		_set_arm_expires(reduction);
		return true;
	}
	return false;
}

/* Used to reduce incast congestion (_need_to_arm) and
 * detect the need to retry */
static inline
bool _is_red_timed_out(struct cxip_coll_reduction *reduction, struct red_pkt *pkt)
{
	if (reduction->mc_obj->retry_disable)
		return false;
	if (!pkt && _need_to_arm(reduction)) {
		TRACE_DEBUG("=== root red_id=%d needs (re)arm\n",
			    reduction->red_id);
		return true;
	}

	return _tsexp(&reduction->tv_expires);
}
/* Root node state machine progress
 * - force_root_retry indicates retry from cxip_coll_progress_cq_poll(),
 *   pkt == NULL
 * - !pkt means this is progressing from injection call (e.g. fi_reduce())
 *   or from cxip_coll_progress_cq_poll()
 * - pkt means this is progressing from event callback _coll_recv_cb()
 *   (incoming leaf packet)
 */
static void _progress_root(struct cxip_coll_reduction *reduction,
			   struct red_pkt *pkt, bool force_root_retry)
{
	struct cxip_coll_mc *mc_obj = reduction->mc_obj;
	struct cxip_coll_data coll_data = {0} ;
	ssize_t ret;
	bool timed_out;

	/* State machine disabled for testing */
	if (reduction->coll_state != CXIP_COLL_STATE_READY)
		return;

	/* Retry preempts partial collective (pkt == NULL) */
	timed_out = _is_red_timed_out(reduction, pkt);
	if (force_root_retry || timed_out) {
		/* reset reduction for retry send */
		reduction->seqno = mc_obj->seqno;
		INCMOD(mc_obj->seqno, CXIP_COLL_MOD_SEQNO);
		TRACE_PKT("progress_root (retry: %d, timed out: %d) "
			  "seqno %d red_id %d\n",
			  force_root_retry, timed_out, reduction->seqno,
			  reduction->red_id);
		ofi_atomic_inc32(&mc_obj->tmout_cnt);

		/* restore data for retry */
		memcpy(&reduction->accum, &reduction->backup,
		       sizeof(reduction->backup));

		_ts_red_clr(reduction);
		_ts_red_set(reduction, &reduction->mc_obj->rootexpires,
			    &mc_obj->ep_obj->coll.root_retry_list);

		ret = cxip_coll_send_red_pkt(reduction, NULL,
					     !mc_obj->arm_disable, true, false);
		if (ret) {
			SET_RED_RC(reduction->accum.red_rc,
				   CXIP_COLL_RC_TX_FAILURE);
			reduction->completed = true;
			goto post_complete;
		}
		return;
	}

	/* Process received packet */
	if (pkt) {
		/* Drop out-of-date packets */
		if (pkt->hdr.resno != reduction->seqno) {
			TRACE_DEBUG("bad seqno, exp=%d saw=%d red_id %d\n",
				reduction->seqno, pkt->hdr.resno,
				reduction->red_id);
			ofi_atomic_inc32(&mc_obj->seq_err_cnt);
			return;
		}

		/* capture packet information */
		_unpack_red_data(&coll_data, pkt);
#if ENABLE_DEBUG
		/* capture completion metrics */
		_measure_completions(coll_data.red_cnt,
				     mc_obj->av_set_obj->fi_addr_cnt);
#endif
		/* perform the reduction */
		_reduce(&reduction->accum, &coll_data, false);
		_dump_coll_data("after leaf contrib to root", &reduction->accum);
	} else {
		/* After the first 8 sends, we let the leaves lead, but we still need
		 * to set the timer in case we don't hear from all of them.
		 */
		_ts_red_set(reduction, &reduction->mc_obj->rootexpires,
			    &mc_obj->ep_obj->coll.root_retry_list);
	}

	/* check for reduction complete */
	if (reduction->accum.red_cnt == mc_obj->av_set_obj->fi_addr_cnt) {
		/* copy reduction result to user result buffer */
		if (reduction->op_rslt_data && reduction->op_data_bytcnt) {
			memcpy(reduction->op_rslt_data,
			       reduction->accum.databuf,
			       reduction->op_data_bytcnt);
		}

		/* send reduction result to leaves, arm new seqno */
		reduction->seqno = mc_obj->seqno;
		INCMOD(mc_obj->seqno, CXIP_COLL_MOD_SEQNO);
		reduction->completed = true;
		_ts_red_clr(reduction);

		TRACE_DEBUG("root final result, mc %x seqn %d red_id %d op %d\n",
			mc_obj->mcast_addr, reduction->seqno, reduction->red_id,
			reduction->accum.red_op);

		ret = cxip_coll_send_red_pkt(reduction, &reduction->accum,
			!mc_obj->arm_disable, false, true);
		_set_arm_expires(reduction);

		if (ret)
			SET_RED_RC(reduction->accum.red_rc,
				   CXIP_COLL_RC_TX_FAILURE);

	} else {
		if (pkt) {
			TRACE_DEBUG("incomplete reduction (recvd: %d, expected: %lu)\n",
				reduction->accum.red_cnt, mc_obj->av_set_obj->fi_addr_cnt);
			assert(reduction->accum.red_cnt <= mc_obj->av_set_obj->fi_addr_cnt);
		}
	}

post_complete:
	/* Post completions in injection order */
	reduction = &mc_obj->reduction[mc_obj->tail_red_id];
	while (reduction->in_use && reduction->completed) {
		/* Reduction completed on root */
		TRACE_DEBUG("posting root completion\n");
		_post_coll_complete(reduction);
		dump_reduction_table(mc_obj->tail_red_id,  &mc_obj->reduction[0]);

		/* Advance to the next reduction */
		INCMOD(mc_obj->tail_red_id, mc_obj->max_red_id);
		reduction = &mc_obj->reduction[mc_obj->tail_red_id];
	}
}

static inline
void _clear_leaf_rget_state(struct cxip_coll_reduction *reduction) {

	reduction->rdma_get_sent = false;
	reduction->rdma_get_completed = false;
	reduction->leaf_contrib_start_us = 0;
	reduction->rdma_get_cb_rc = 0;

} 
/* Leaf node state machine progress.
 * !pkt means this is progressing from injection call (e.g. fi_reduce())
 * pkt means this is progressing from event callback (receipt of packet)
 * force_rdma_read triggered from cxip_coll_progress_cq_poll
 */
static void _progress_leaf(struct cxip_coll_reduction *reduction,
			   struct red_pkt *pkt, bool force_rdma_read)
{
	struct cxip_coll_mc *mc_obj = reduction->mc_obj;
	struct cxip_coll_data coll_data = {0};
	int ret, red_id, rdma_read_stat, next_rdma_exp_seqn;
	struct red_pkt_64 *get_pkt_64_p = NULL;
	uint64_t leaf_contrib_ts_delta __attribute__((unused));
	struct red_pkt *root_pkt = NULL;

	/* state machine disabled for testing */
	if (reduction->coll_state != CXIP_COLL_STATE_READY)
		return;

	if (force_rdma_read) {

		leaf_contrib_ts_delta = (ofi_gettime_us() - reduction->leaf_contrib_start_us);
		TRACE_DEBUG("%s force_rdma_read set red_id %d red_op %d ts_delta %016lx\n",
			__func__, reduction->red_id, reduction->accum.red_op, leaf_contrib_ts_delta);
		if (!reduction->rdma_get_sent) {
			TRACE_DEBUG("%s starting rdma get red_id %d red_op %d ts %016lx\n",
				__func__, reduction->red_id, reduction->accum.red_op, ofi_gettime_us());
			TRACE_DEBUG("%s rdma get mc_addr: %x\n", __func__, mc_obj->mcast_addr);
			TRACE_DEBUG("%s rdma get ts delta: %016lx\n", __func__, leaf_contrib_ts_delta);
			rdma_read_stat = _leaf_rdma_get(reduction);
			if (rdma_read_stat) {
				TRACE_DEBUG("%s leaf rdma get start failed! %d red_id %d red_op %d ts %016lx\n",
					__func__, rdma_read_stat, reduction->red_id, reduction->accum.red_op,
					ofi_gettime_us());
				SET_RED_RC(reduction->accum.red_rc, CXIP_COLL_RC_RDMA_FAILURE);
				reduction->completed = true;
				/* stop progress call from cxip_coll_progress_cq_poll,
				 * and prevent leaf hangs
				 */
				_ts_red_clr(reduction);
				goto post_complete;
			}
		}
		if (reduction->rdma_get_completed) {
			if (reduction->rdma_get_cb_rc == C_RC_OK) {
				TRACE_DEBUG("%s leaf_rdma_get completed! red_id %d red_op %d seqn %d\n",
					__func__, reduction->red_id, reduction->accum.red_op, reduction->seqno);
				/* unpack the the rdma read buffer and go to post_complete */
				red_id = reduction->red_id;
				get_pkt_64_p = (struct red_pkt_64 *)mc_obj->leaf_rdma_get_data_p;
				root_pkt = (struct red_pkt *)&get_pkt_64_p[red_id].pkt;
				TRACE_DEBUG("%s leaf_rdma_get pkt red_id %d red_op %d seqn %d\n",
					__func__, root_pkt->hdr.cookie.red_id, root_pkt->hdr.op, root_pkt->hdr.seqno);
				/* verify packet and make sure it is what we expected */
				next_rdma_exp_seqn = reduction->seqno;
				INCMOD(next_rdma_exp_seqn, CXIP_COLL_MOD_SEQNO);
				if (next_rdma_exp_seqn != root_pkt->hdr.seqno) {
					/* dont touch user data, make sure we have the correct seqn
					 * log event only
					 */
					TRACE_DEBUG("%s leaf_rdma_get pkt seqn err! red_id %d red_op %d eseqn %d seqn %d\n",
						__func__, root_pkt->hdr.cookie.red_id, root_pkt->hdr.op,
						reduction->seqno, root_pkt->hdr.seqno);
					_clear_leaf_rget_state(reduction);	
					_ts_red_clr(reduction);
					return;
				}
				if (reduction->accum.red_op != root_pkt->hdr.op) {
					/* dont touch user data, make sure we have the correct opcode
					 * log event only
					 */
					TRACE_DEBUG("%s leaf_rdma_get pkt opcode err! red_op %d pkt_op %d pkt_red_id %d\n",
						__func__, reduction->accum.red_op, root_pkt->hdr.op,
						root_pkt->hdr.cookie.red_id);
					_clear_leaf_rget_state(reduction);	
					_ts_red_clr(reduction);
					return;
				}
				if (mc_obj->mcast_addr != root_pkt->hdr.cookie.mcast_id) {
					/* dont touch user data, make sure we have the correct mcast addr
					 * log event only
					 */
					TRACE_DEBUG("%s leaf_rdma_get pkt mcast err! red_id %d red_op %d emcast %x mcast %x\n",
						__func__, root_pkt->hdr.cookie.red_id, root_pkt->hdr.op,
						mc_obj->mcast_addr, root_pkt->hdr.cookie.mcast_id);
					_clear_leaf_rget_state(reduction);	
					_ts_red_clr(reduction);
					return;
				}
				/* should not happen, we only save final packet at the root */
				if (root_pkt->hdr.retry) {
					/* dont touch user data, log event only */
					TRACE_DEBUG("%s leaf_rdma_get pkt retry! red_op %d pkt_op %d pkt_red_id %d\n",
						__func__, reduction->accum.red_op, root_pkt->hdr.op,
						root_pkt->hdr.cookie.red_id);
					_clear_leaf_rget_state(reduction);	
					_ts_red_clr(reduction);
					return;
				}
				if (root_pkt->hdr.cookie.red_id != red_id) {
					/* dont touch user data, fatal event that indicates corruption */
					TRACE_DEBUG("%s leaf_rdma_get pkt bad red_id! red_op %d p_op %d p_red_id %d red_id %d\n",
						__func__, reduction->accum.red_op, root_pkt->hdr.op,
						root_pkt->hdr.cookie.red_id, red_id);
					SET_RED_RC(reduction->accum.red_rc, CXIP_COLL_RC_RDMA_DATA_FAILURE);
					reduction->completed = true;
					/* stop progress call from cxip_coll_progress_cq_poll, and prevent leaf hangs */
					_ts_red_clr(reduction);
					goto post_complete;
				}
				/* Capture final reduction data in user-pointer */
				SET_RED_RC(reduction->accum.red_rc, root_pkt->hdr.red_rc);
				if (reduction->op_rslt_data) {
					_unpack_red_data(&coll_data, root_pkt);
					memcpy(reduction->op_rslt_data,&coll_data.databuf,reduction->op_data_bytcnt);
				}
				reduction->completed = true;
				/* stop progress call from cxip_coll_progress_cq_poll,
				 * and prevent leaf hangs
				 */
				_ts_red_clr(reduction);
				TRACE_DEBUG("%s leaf ok after rdma! red_id %d red_op %d seqn %d ts %016lx\n",
					__func__, red_id, reduction->accum.red_op, reduction->seqno, ofi_gettime_us());
				goto post_complete;
			} else {
				TRACE_DEBUG("%s leaf rdma bad status %d red_id %d red_op %d seqn %d ts %016lx\n",
					__func__, reduction->rdma_get_cb_rc, reduction->red_id,
					reduction->accum.red_op, reduction->seqno, ofi_gettime_us());
				/* read attempt failed, we are done. Should we attempt retries? */
				SET_RED_RC(reduction->accum.red_rc, CXIP_COLL_RC_RDMA_FAILURE);
				reduction->completed = true;
				/* stop progress call from cxip_coll_progress_cq_poll, and prevent leaf hangs */
				_ts_red_clr(reduction);
				goto post_complete;
			}
		}
		/* continue to progress the timer, once the read completes good or bad the timer is cleared */
		return;
	}

	/* if reduction packet, reset timer, seqno, honor retry */
	if (pkt) {
		TRACE_PKT("_progress_leaf: packet seen, seqno %d\n",
			  pkt->hdr.seqno);
		reduction->seqno = pkt->hdr.seqno;
		reduction->resno = pkt->hdr.seqno;
		if (pkt->hdr.retry) {
			TRACE_PKT("%s leaf honoring retry op %d red_id %d seqno %d\n",
				__func__, reduction->accum.red_op, reduction->red_id,
				reduction->seqno);
			reduction->pktsent = false;
		}
	}

	/* leaves lead with sending a packet */
	if (!reduction->pktsent) {
		TRACE_PKT("%s leaf preparing to send op %d red_id %d seqno %d\n",
			__func__, reduction->accum.red_op, reduction->red_id, reduction->seqno);

		_ts_red_clr(reduction);
		if (!pkt && _need_to_arm(reduction)) {
			TRACE_DEBUG("%s leaf waiting for arm op %d red_id %d seqno %d\n",
				__func__, reduction->accum.red_op, reduction->red_id, reduction->seqno);
			return;
		}

		/* Don't send if nothing to send yet */
		if (!reduction->accum.initialized) {
			TRACE_DEBUG("%s accm not initialized op %d red_id %d seqno %d\n",
				__func__, reduction->accum.red_op, reduction->red_id, reduction->seqno);
			return;
		}

		reduction->leaf_contrib_start_us = ofi_gettime_us();
		_ts_red_set(reduction, &reduction->mc_obj->leafexpires,
		    &mc_obj->ep_obj->coll.leaf_rdma_get_list);

		/* Send leaf data */
		ret = cxip_coll_send_red_pkt(reduction, &reduction->accum,
					     false, false, false);
		if (ret) {
			SET_RED_RC(reduction->accum.red_rc,
				   CXIP_COLL_RC_TX_FAILURE);
			reduction->completed = true;
			TRACE_DEBUG("%s cxip_coll_send_red_pkt failure %d\n",__func__, ret);
			goto post_complete;
		}
		reduction->pktsent = true;
		TRACE_DEBUG("%s LEAF sent its contribution op %d red_id %d seqno %d, ts %016lx\n",
			__func__, reduction->accum.red_op, reduction->red_id,
			reduction->seqno, ofi_gettime_us());
		
	}

	/* If no incoming reduction packet, we are done */
	if (!pkt)
		return;

	/* If packet has no reduction count (retry), done */
	if (!pkt->hdr.redcnt)
		return;

	/* Capture final reduction data in user-pointer */
	SET_RED_RC(reduction->accum.red_rc, pkt->hdr.red_rc);
	if (reduction->op_rslt_data) {
		_unpack_red_data(&coll_data, pkt);
		memcpy(reduction->op_rslt_data,
			&coll_data.databuf,
			reduction->op_data_bytcnt);
	}
	TRACE_DEBUG("%s leaf reduction completed normally! red_id %d red_op %d seqno %d ts %016lx\n",
		__func__, reduction->red_id, reduction->accum.red_op, reduction->seqno, ofi_gettime_us());
	_ts_red_clr(reduction);
	reduction->completed = true;
post_complete:
	/* Post completions in injection order */
	reduction = &mc_obj->reduction[mc_obj->tail_red_id];
	while (reduction->in_use && reduction->completed) {
		TRACE_DEBUG("%s posting leaf completion\n", __func__);
		dump_reduction_table(mc_obj->tail_red_id,  &mc_obj->reduction[0]);
		_post_coll_complete(reduction);
		INCMOD(mc_obj->tail_red_id, mc_obj->max_red_id);
		reduction = &mc_obj->reduction[mc_obj->tail_red_id];
	}
}

/* Root or leaf progress state machine.
 */
static void _progress_coll(struct cxip_coll_reduction *reduction,
			   struct red_pkt *pkt)
{
	if (is_hw_root(reduction->mc_obj))
		_progress_root(reduction, pkt, false);
	else
		_progress_leaf(reduction, pkt, false);
}

/* Root or leaf CQ poll timeout progress state machine.
 * Called on every CQ poll, so must be fast.
 * Must be exported, call is in cxip_txc_hpc_progress().
 */
void cxip_coll_progress_cq_poll(struct cxip_ep_obj *ep_obj)
{
	struct cxip_coll_reduction *reduction;

	if (!ep_obj->coll.enabled)
		return;

	reduction = dlist_first_entry_or_null(&ep_obj->coll.root_retry_list,
					      struct cxip_coll_reduction,
					      tmout_link);
	if (reduction && _tsexp(&reduction->tv_expires)) {
		TRACE_DEBUG("progressing root red_id %d, seqno %d from hpc_progress\n",
			    reduction->red_id, reduction->seqno);
		_progress_root(reduction, NULL, true);
	}
	reduction = dlist_first_entry_or_null(&ep_obj->coll.leaf_rdma_get_list,
					      struct cxip_coll_reduction,
					      tmout_link);
	if (reduction && _tsexp(&reduction->tv_expires)) {

		TRACE_DEBUG("progressing leaf reduction addr %p\n",reduction);
		TRACE_DEBUG("progressing leaf red_id %d, seqno %d from hpc_progress\n",
			    reduction->red_id, reduction->seqno);
		_progress_leaf(reduction, NULL, true);
	}

}

/* Debugging only */
static int *_injected_red_id_buf;
void cxip_capture_red_id(int *red_id_buf)
{
	_injected_red_id_buf = red_id_buf;
}

/* Generic collective pre-reduction into cxip_coll_data structure */
static void
_cxip_coll_prereduce(int cxi_opcode, const void *op_send_data,
		     void *accum, size_t sendcnt, uint64_t flags)
{
	const struct cxip_coll_data *coll_data_ptr;
	struct cxip_coll_data coll_data = {0};

	/* Convert user data to local coll_data structure */
	if (flags & FI_CXI_PRE_REDUCED) {
		coll_data_ptr = op_send_data;
	} else {
		_init_coll_data(&coll_data, cxi_opcode, op_send_data,
				sendcnt);
		coll_data_ptr = &coll_data;
	}
	_dump_coll_data("coll_data initialized pre", coll_data_ptr);

	/* pre-reduce data into accumulator */
	_reduce(accum, coll_data_ptr, true);
}

/* Generic collective injection into fabric.
 *
 * Reduction ID is normally hidden. Can be exposed by calling _capture_red_id()
 * just before calling a reduction operation.
 *
 * - Acquires next available reduction structure in MC, or returns -FI_EAGAIN.
 * - Acquires evtq request, or return -FI_EAGAIN.
 * - Marks reduction structure in-use.
 * - Advances next available reduction pointer.
 * - Initializes:
 *   - result data pointer
 *   - source data (pre-reduced or raw)
 *   - data byte count
 * - Reduces user data into reduction accumulator (may already contain data)
 * - Progresses reduction (no packet supplied)
 */
static ssize_t
_cxip_coll_inject(struct cxip_coll_mc *mc_obj, int cxi_opcode,
		  const void *op_send_data, void *op_rslt_data,
		  size_t bytcnt, uint64_t flags, void *context)
{
	struct cxip_coll_reduction *reduction;
	struct cxip_coll_data coll_data = {0};
	struct cxip_req *req;
	int ret;

	TRACE_DEBUG("%s entry\n", __func__);
	TRACE_DEBUG("%s bytecnt=%ld\n", __func__, bytcnt);
	ofi_genlock_lock(&mc_obj->ep_obj->lock);

	TRACE_DEBUG("attempting to use red_id %d\n", mc_obj->next_red_id);

	/* must observe strict round-robin across all nodes */
	reduction = &mc_obj->reduction[mc_obj->next_red_id];
	if (reduction->in_use) {
		dump_reduction_table(mc_obj->next_red_id,  &mc_obj->reduction[0]);
		ret = -FI_EAGAIN;
		goto quit;
	}

	TRACE_DEBUG("using red_id %d\n", mc_obj->next_red_id);

	/* acquire a request structure */
	req = cxip_evtq_req_alloc(mc_obj->ep_obj->coll.tx_evtq, 1, NULL);
	if (!req) {
		ret = -FI_EAGAIN;
		goto quit;
	}

	/* Used for debugging */
	if (_injected_red_id_buf) {
		*_injected_red_id_buf = reduction->red_id;
		_injected_red_id_buf = NULL;
	}

	/* advance next_red_id, reserving this one for us */
	INCMOD(mc_obj->next_red_id, mc_obj->max_red_id);
	reduction->in_use = true;

	/* Set up the reduction structure */
	reduction->op_rslt_data = op_rslt_data;
	reduction->op_data_bytcnt = bytcnt;
	reduction->op_context = context;
	reduction->op_inject_req = req;
	reduction->op_inject_req->context = (uint64_t)context;

	/* Convert user data to local coll_data structure */
	if (flags & FI_CXI_PRE_REDUCED)
		memcpy(&coll_data, op_send_data, sizeof(coll_data));
	else
		_init_coll_data(&coll_data, cxi_opcode, op_send_data, bytcnt);

	/* reduce data into accumulator */
	_reduce(&reduction->accum, &coll_data, false);
	/* make a copy for retries */
	memcpy(&reduction->backup, &reduction->accum, sizeof(reduction->backup));
	_dump_coll_data("coll_data initialized inj", &coll_data);

	/* Progress the collective */
	_progress_coll(reduction, NULL);
	ret = FI_SUCCESS;

quit:
	ofi_genlock_unlock(&mc_obj->ep_obj->lock);
	TRACE_DEBUG("%s return %d\n", __func__, ret);
	return ret;
}

/* Get the mc_obj from ep/coll_addr and check for consistency */
static inline
ssize_t _get_mc_obj(struct fid_ep *ep, fi_addr_t coll_addr,
		    struct cxip_coll_mc **mc_obj)
{
	struct cxip_ep *cxi_ep;

	if (!ep) {
		CXIP_WARN("Collective requires ep\n");
		return -FI_EINVAL;
	}

	if (!coll_addr) {
		CXIP_WARN("Collective requires coll_addr\n");
		return -FI_EINVAL;
	}

	cxi_ep = container_of(ep, struct cxip_ep, ep.fid);
	*mc_obj = (struct cxip_coll_mc *)((uintptr_t)coll_addr);

	if ((*mc_obj)->ep_obj != cxi_ep->ep_obj) {
		CXIP_WARN("Multicast does not belong to ep\n");
		return -FI_EINVAL;
	}

	if (!(*mc_obj)->is_joined) {
		CXIP_WARN("Multicast collective not joined\n");
		return -FI_EOPBADSTATE;
	}

	return FI_SUCCESS;
}

/* get payload byte count and check for consistency */
static inline
ssize_t _get_bytcnt(int cxi_opcode, enum fi_datatype datatype,
		    const void *buf, size_t count)
{
	ssize_t bytcnt;

	if (cxi_opcode < 0) {
		CXIP_WARN("Opcode not supported\n");
		return -FI_EINVAL;
	}

	if (!buf || count <= 0L) {
		CXIP_WARN("Buffer required\n");
		return -FI_EINVAL;
	}

	bytcnt = _get_cxi_data_bytcnt(cxi_opcode, datatype, count);
	if (bytcnt < 0)
		CXIP_WARN("Opcode does not support datatype\n");

	return bytcnt;
}

ssize_t cxip_barrier(struct fid_ep *ep, fi_addr_t coll_addr, void *context)
{
	struct cxip_coll_mc *mc_obj;
	int cxi_opcode;
	ssize_t ret;

	/* barrier requires mc_obj */
	ret = _get_mc_obj(ep, coll_addr, &mc_obj);
	if (ret) {
		CXIP_WARN("Failed to get multicast object for barrier "
			  "(ret=%zd, coll_addr=%lu, ep=%p)\n",
			  ret, coll_addr, ep);
		return ret;
	}

	cxi_opcode = COLL_OPCODE_BARRIER;

	return _cxip_coll_inject(mc_obj, cxi_opcode, NULL, NULL, 0, 0, context);
}

ssize_t cxip_broadcast(struct fid_ep *ep, void *buf, size_t count,
		       void *desc, fi_addr_t coll_addr, fi_addr_t root_addr,
		       enum fi_datatype datatype, uint64_t flags,
		       void *context)
{
	struct cxip_coll_mc *mc_obj;
	int cxi_opcode, bytcnt;
	ssize_t ret;

	if (flags & (FI_MORE|FI_CXI_PRE_REDUCED)) {
		CXIP_WARN("Unsupported flags for broadcast operation "
			  "(flags=0x%lx, FI_MORE=%s, FI_CXI_PRE_REDUCED=%s)\n",
			  flags, (flags & FI_MORE) ? "true" : "false",
			  (flags & FI_CXI_PRE_REDUCED) ? "true" : "false");
		return -FI_EINVAL;
	}

	cxi_opcode = COLL_OPCODE_BIT_OR;
	bytcnt = _get_bytcnt(cxi_opcode, datatype, buf, count);
	if (bytcnt < 0) {
		CXIP_WARN("Invalid byte count for broadcast operation (ret=%d, "
			  "count=%zu, datatype=%d, cxi_opcode=%d)\n",
			  bytcnt, count, datatype, cxi_opcode);
		return -FI_EINVAL;
	}

	/* broadcast requires mc_obj */
	ret = _get_mc_obj(ep, coll_addr, &mc_obj);
	if (ret) {
		CXIP_WARN("Failed to get multicast object for broadcast "
			  "(ret=%zd, coll_addr=%lu, ep=%p)\n",
			  ret, coll_addr, ep);
		return ret;
	}

	/* only root node contributes data, others contribute 0 */
	if (root_addr != mc_obj->mynode_fiaddr)
		memset(buf, 0, bytcnt);

	/* buf serves as source and result */
	return _cxip_coll_inject(mc_obj, cxi_opcode, buf, buf, bytcnt,
				 flags, context);
}

ssize_t cxip_reduce(struct fid_ep *ep, const void *buf, size_t count,
		    void *desc, void *result, void *result_desc,
		    fi_addr_t coll_addr, fi_addr_t root_addr,
		    enum fi_datatype datatype, enum fi_op op, uint64_t flags,
		    void *context)
{
	struct cxip_coll_mc *mc_obj;
	int cxi_opcode;
	ssize_t bytcnt, ret;

	TRACE_DEBUG("%s entry\n", __func__);
	cxi_opcode = cxip_fi2cxi_opcode(op, datatype);
	bytcnt = _get_bytcnt(cxi_opcode, datatype, buf, count);
	if (bytcnt < 0)
		return (ssize_t)bytcnt;

	/* FI_MORE requires result buffer, succeeds immediately */
	if (flags & FI_MORE) {
		if (!result) {
			CXIP_WARN("Result buffer cannot be NULL with FI_MORE "
				  "flag for reduce (buf=%p, count=%zu, "
				  "root_addr=%lu)\n",
				  buf, count, root_addr);
			return -FI_EINVAL;
		}
		_cxip_coll_prereduce(cxi_opcode, buf, result, bytcnt, flags);
		return FI_SUCCESS;
	}

	/* otherwise reduce requires mc_obj */
	ret = _get_mc_obj(ep, coll_addr, &mc_obj);
	if (ret) {
		CXIP_WARN("Failed to get multicast object for reduce (ret=%zd, "
			  "coll_addr=%lu, ep=%p)\n",
			  ret, coll_addr, ep);
		return ret;
	}

	/* root requires a result buffer */
	if (!result && (mc_obj->mynode_fiaddr == root_addr)) {
		CXIP_WARN(
			"Root node requires result buffer for reduce operation "
			"(mynode_fiaddr=%lu, root_addr=%lu, coll_addr=%lu)\n",
			mc_obj->mynode_fiaddr, root_addr, coll_addr);
		return -FI_EINVAL;
	}

	return _cxip_coll_inject(mc_obj, cxi_opcode, buf, result, bytcnt,
				 flags, context);
}

ssize_t cxip_allreduce(struct fid_ep *ep, const void *buf, size_t count,
		       void *desc, void *result, void *result_desc,
		       fi_addr_t coll_addr, enum fi_datatype datatype,
		       enum fi_op op, uint64_t flags, void *context)
{
	struct cxip_coll_mc *mc_obj;
	int cxi_opcode, bytcnt;
	ssize_t ret;

	TRACE_DEBUG("%s entry\n", __func__);
	cxi_opcode = cxip_fi2cxi_opcode(op, datatype);
	TRACE_DEBUG("%s cxi_opcode = %d\n", __func__, cxi_opcode);
	bytcnt = _get_bytcnt(cxi_opcode, datatype, buf, count);
	TRACE_DEBUG("%s bytcnt = %d\n", __func__, bytcnt);
	if (bytcnt < 0)
		return bytcnt;

	/* result required in all cases */
	if (!result) {
		CXIP_WARN("Result buffer cannot be NULL for allreduce "
			  "operation (buf=%p, count=%zu)\n",
			  buf, count);
		return -FI_EINVAL;
	}

	/* FI_MORE succeeds immediately */
	if (flags & FI_MORE) {
		_cxip_coll_prereduce(cxi_opcode, buf, result, bytcnt, flags);
		return FI_SUCCESS;
	}

	/* otherwise reduce requires mc_obj */
	ret = _get_mc_obj(ep, coll_addr, &mc_obj);
	if (ret) {
		CXIP_WARN("Failed to get multicast object for allreduce "
			  "(ret=%zd, coll_addr=%lu, ep=%p)\n",
			  ret, coll_addr, ep);
		return ret;
	}

	return _cxip_coll_inject(mc_obj, cxi_opcode, buf, result, bytcnt,
				 flags, context);
}

/****************************************************************************
 * JOIN COLLECTIVE STATE MACHINE
 */

/* Packed structure to fit information into zbcoll broadcast payload */
union pack_mcast {
	uint64_t uint64;
	struct {
		uint64_t mcast_addr: 16;// maximum anticipated multicast
		uint64_t hwroot_idx: 27;// 128M endpoints in tree
		uint64_t valid: 1;	// success flag
		uint64_t pad: 20;	// used by zbcoll
	} __attribute__((__packed__));
} __attribute__((__packed__));

union pack_errbits {
	uint64_t uint64;
	struct {
		uint64_t error_bits: 43;// up to 43 independent errors
		uint64_t valid: 1;	// success flag
		uint64_t pad1: 20;	// needed by zbcoll

	} __attribute__((__packed__));
} __attribute__((__packed__));

/* State structure for carrying data through the join sequence */
struct cxip_join_state {
	struct cxip_ep_obj *ep_obj;	// ep object
	struct cxip_av_set *av_set_obj;	// av set for this collective
	struct cxip_coll_mc *mc_obj;	// mc object for this collective
	struct cxip_zbcoll_obj *zb;	// zb object associated with state
	struct fid_mc **mc;		// user pointer to return mc_obj
	void *context;			// user context for concurrent joins
	uint64_t join_flags;		// user-supplied libfabric join flags
	union pack_mcast bcast_data;	// packed multicast data
	union pack_errbits reduce_err;	// packed join error bits
	bool rx_discard;		// set if RX events should be discarded
	bool is_rank;			// set if using COLL_RANK simulation model
	bool is_mcast;			// set if using Rosetta multicast tree
	bool create_mcast;		// set to create Rosetta multicast tree
	bool creating_mcast;		// set once CURL has been initiated
	bool finished_mcast;		// set once CURL has been completed
	bool created_ptlte;		// set once PtlTE is initialized
	int mynode_idx;			// index within the fi_addr[] list
	int mynode_fiaddr;		// fi_addr of this node
	int simrank;			// simulated rank of NIC
	int prov_errno;			// collective provider error
	int sched_state;		// scheduled operation
	int join_idx;			// unique join index for diagnostics
	struct dlist_entry sched_link;	// link to scheduled actions
};

static int _root_rdma_get_setup(struct cxip_join_state *jstate);
static int _leaf_rdma_get_setup(struct cxip_join_state *jstate);

/* State structure for recovering data from CURL response */
struct cxip_curl_mcast_create_usrptr {
	struct cxip_join_state *jstate;	// join state
	int mcast_id;			// multicast address
	int hwroot_rank;		// hardware root index
};

struct cxip_curl_mcast_delete_usrptr {
	struct cxip_coll_mc *mc_obj;	// multicast object
};

/* pack provider errors into AND bitmask - address data */
void _proverr_to_bits(struct cxip_join_state *jstate)
{
	int bitno;

	/* record error as a bit for this endpoint */
	TRACE_JOIN("%s: prov_errno=%d\n", __func__, jstate->prov_errno);
	jstate->reduce_err.error_bits = 0L;
	if (jstate->prov_errno) {
		if (jstate->prov_errno >= FI_CXI_ERRNO_JOIN_LAST)
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_OTHER;
		bitno = jstate->prov_errno - FI_CXI_ERRNO_JOIN_FIRST;
		jstate->reduce_err.error_bits |= (1L << bitno);
	}
	/* invert bits, zbcoll reduce does AND */
	TRACE_JOIN("%s: error bitmask=%016lx\n", __func__,
		   (uint64_t)jstate->reduce_err.error_bits);
	jstate->reduce_err.error_bits ^= -1L;
}

/* unpack bitmask and return largest error */
void _bits_to_proverr(struct cxip_join_state *jstate)
{
	int prov_errno;
	uint64_t bitmask;

	/* zbcoll reduce does AND, invert bits */
	jstate->reduce_err.error_bits ^= -1L;
	TRACE_JOIN("%s: error bitmask=%016lx\n", __func__,
		   (uint64_t)jstate->reduce_err.error_bits);

	/* display all errors, capture the highest value error */
	jstate->prov_errno = 0L;
	if (!jstate->reduce_err.error_bits) {
		TRACE_JOIN("%s: no error seen\n", __func__);
		return;
	}

	bitmask = 1L;
	for (prov_errno = FI_CXI_ERRNO_JOIN_FIRST;
	     prov_errno < FI_CXI_ERRNO_JOIN_LAST;
	     prov_errno++) {
		if (jstate->reduce_err.error_bits & bitmask) {
			jstate->prov_errno = prov_errno;
			CXIP_WARN("%s\n", cxip_strerror(jstate->prov_errno));
			TRACE_JOIN("%s\n", cxip_strerror(jstate->prov_errno));
		}
		bitmask <<= 1;
	}
}

/* Close collective pte object - ep_obj->lock must be held */
static void _close_pte(struct cxip_coll_pte *coll_pte)
{
	int ret;

	if (!coll_pte)
		return;
	do {
		ret = _coll_pte_disable(coll_pte);
	} while (ret == -FI_EAGAIN);
	_coll_destroy_buffers(coll_pte);
	cxip_pte_free(coll_pte->pte);
	free(coll_pte);
}

/* pid_idx == CXIP_PTL_IDX_COLL+rank for NETSIM
 * pid_idx == CXIP_PTL_IDX_COLL for UNICAST
 * pid_idx == multicast for MULTICAST
 */
static int _acquire_pte(struct cxip_ep_obj *ep_obj, int pid_idx,
			 bool is_mcast, struct cxip_coll_pte **coll_pte_ret)
{
	struct cxi_pt_alloc_opts pt_opts = {
		.use_long_event = 1,
		.do_space_check = 1,
		.en_restricted_unicast_lm = 1,
	};
	struct cxip_coll_pte *coll_pte;
	int ret;

	*coll_pte_ret = NULL;
	coll_pte = calloc(1, sizeof(*coll_pte));
	if (!coll_pte) {
		TRACE_JOIN("out of memory\n");
		CXIP_WARN("Failed to allocate collective PTE structure "
			  "(size=%zu, pid_idx=%d)\n",
			  sizeof(*coll_pte), pid_idx);
		return -FI_ENOMEM;
	}

	/* initialize coll_pte */
	coll_pte->ep_obj = ep_obj;
	dlist_init(&coll_pte->buf_list);
	ofi_atomic_initialize32(&coll_pte->buf_cnt, 0);
	ofi_atomic_initialize32(&coll_pte->buf_swap_cnt, 0);
	ofi_atomic_initialize32(&coll_pte->recv_cnt, 0);

	/* bind PTE to domain */
	ret = cxip_pte_alloc(ep_obj->ptable, ep_obj->coll.rx_evtq->eq,
			     pid_idx, is_mcast, &pt_opts, _coll_pte_cb,
			     coll_pte, &coll_pte->pte);
	if (ret) {
		TRACE_JOIN("cxip_pte_alloc failed=%d\n", ret);
		CXIP_WARN("Failed to allocate PTE resource (ret=%d, "
			  "pid_idx=%d, is_mcast=%s)\n",
			  ret, pid_idx, is_mcast ? "true" : "false");
		free(coll_pte);
		return ret;
	}

	/* enable the PTE */
	ret = _coll_pte_enable(coll_pte, CXIP_PTE_IGNORE_DROPS);
	if (ret) {
		TRACE_JOIN("_coll_pte_enable failed=%d\n", ret);
		CXIP_WARN("Failed to enable collective PTE (ret=%d, "
			  "pid_idx=%d, pte=%p)\n",
			  ret, pid_idx, coll_pte->pte);
		goto fail;
	}

	/* add buffers to the PTE */
	ret = _coll_add_buffers(coll_pte,
				ep_obj->coll.buffer_size,
				ep_obj->coll.buffer_count);
	if (ret) {
		TRACE_JOIN("_coll_add_buffers failed=%d\n", ret);
		CXIP_WARN("Failed to add buffers to collective PTE (ret=%d, "
			  "buffer_size=%zu, buffer_count=%ld, pid_idx=%d)\n",
			  ret, ep_obj->coll.buffer_size,
			  ep_obj->coll.buffer_count, pid_idx);
		goto fail;
	}

	*coll_pte_ret = coll_pte;
	return FI_SUCCESS;

fail:
	_close_pte(coll_pte);
	return ret;
}

/* forward references for CURL operations */
static void _create_mcast_addr(struct cxip_join_state *jstate);
static void _cxip_create_mcast_cb(struct cxip_curl_handle *handle);
static void _curl_delete_mc_obj(struct cxip_coll_mc *mc_obj);
static void _cxip_delete_mcast_cb(struct cxip_curl_handle *handle);

/* Close multicast collective object */
static void _close_mc(struct cxip_coll_mc *mc_obj, bool delete, bool has_error)
{
	struct cxip_coll_reduction *reduction;
	int count;
	int red_id;
	bool is_hwroot = false;

	if (!mc_obj)
		return;
	TRACE_JOIN("%s starting MC cleanup\n", __func__);

	is_hwroot = is_hw_root(mc_obj);
	if (!is_netsim(mc_obj->ep_obj)) {

		if(is_hwroot) {
			/* revisit this, maybe do a barrier instead
			 * sleep before we cleanup incase we have a rdma read
			 * in progress at the leaf
			 */
			usleep(CXIP_COLL_MAX_RETRY_USEC * CXIP_COLL_MAX_LEAF_TIMEOUT_MULT);
			TRACE_JOIN("%s root mcast_addr %x %p\n", __func__, mc_obj->mcast_addr, mc_obj);
			if(mc_obj->root_rdma_get_md) {
				TRACE_JOIN("%s unmap/free root rdma_get_md %p\n", __func__, mc_obj->root_rdma_get_md);
				cxip_unmap(mc_obj->root_rdma_get_md);
			}
			if(mc_obj->root_rdma_get_data_p) {
				TRACE_JOIN("%s unmap/free root rdma_get_data_p %p\n", __func__, mc_obj->root_rdma_get_data_p);
				free(mc_obj->root_rdma_get_data_p);
			}

		} else {

			TRACE_JOIN("%s leaf mcast_addr %x %p\n", __func__, mc_obj->mcast_addr, mc_obj);
			if(mc_obj->leaf_rdma_get_md) {
				TRACE_JOIN("%s unmap/free leaf rdma_get_md %p\n", __func__, mc_obj->leaf_rdma_get_md);
				cxip_unmap(mc_obj->leaf_rdma_get_md);
			}
			if(mc_obj->leaf_rdma_get_data_p) {
				TRACE_JOIN("%s unmap/free leaf rdma_get_data_p %p\n", __func__, mc_obj->leaf_rdma_get_data_p);
				free(mc_obj->leaf_rdma_get_data_p);
			}

		}

	}
	for (red_id = 0; red_id < CXIP_COLL_MAX_CONCUR; red_id++) {
		reduction = &mc_obj->reduction[red_id];
		_ts_red_clr(reduction);
	}
	mc_obj->has_closed = true;
	mc_obj->has_error = has_error;

	/* clear the mcast_addr -> mc_obj reference*/
	ofi_idm_clear(&mc_obj->ep_obj->coll.mcast_map, mc_obj->mcast_addr);
	mc_obj->ep_obj->coll.is_hwroot = false;

	/* clear the avset alteration lockout */
	mc_obj->av_set_obj->mc_obj = NULL;

	/* unmap the reduction mem descriptor for DMA */
	if (mc_obj->reduction_md)
		cxil_unmap(mc_obj->reduction_md);

	/* close any PTE associated with mc_obj (NETSIM) */
	if (mc_obj->coll_pte != mc_obj->ep_obj->coll.coll_pte)
		_close_pte(mc_obj->coll_pte);

	/* decrement multicast count (real), close PTE if unused */
	count = ofi_atomic_dec32(&mc_obj->ep_obj->coll.num_mc);
	count = ofi_atomic_get32(&mc_obj->ep_obj->coll.num_mc);
	if (!count && mc_obj->ep_obj->coll.coll_pte) {
		_close_pte(mc_obj->ep_obj->coll.coll_pte);
		mc_obj->ep_obj->coll.coll_pte = NULL;
	}

	/* index zero deletes the multicast address */
	if (delete && mc_obj->is_multicast && !mc_obj->mynode_idx) {
		if (!mc_obj->has_error)
			mc_obj->close_state = -FI_EAGAIN;

		_curl_delete_mc_obj(mc_obj);
	} else {
		if (mc_obj->has_error) {
			free(mc_obj);
		} else {
			mc_obj->close_state = FI_SUCCESS;
		}
	}
}

/* The user can close an individual collective MC address. It must do so on
 * all endpoints in the collective group, just as fi_join_collective() must
 * be called on all endpoints in the group.
 */
static int _fi_close_mc(struct fid *fid)
{
	struct cxip_coll_mc *mc_obj;
	int ret = FI_SUCCESS;

	TRACE_JOIN("%s: closing MC\n", __func__);
	mc_obj = container_of(fid, struct cxip_coll_mc, mc_fid.fid);
	if (!mc_obj) {
		TRACE_JOIN("%s: MC object is null\n", __func__);
		return ret;
	} else if (mc_obj->has_closed) {
		TRACE_JOIN("%s: close already called before\n", __func__);
		return ret;
	} else if (mc_obj->has_error) {
		TRACE_JOIN("%s: encounted an error earlier\n", __func__);
		return ret;
	}

	_close_mc(mc_obj, true, false);
	while (mc_obj && (ret = mc_obj->close_state) == -FI_EAGAIN) {
		ret = cxip_curl_progress(NULL);
		if (ret == -FI_EAGAIN) {
			usleep(10);
			continue;
		}
		if (ret < 0 && ret != -FI_ENODATA) {
			CXIP_INFO("%s: Curl progress failed, error=%d\n",
				  __func__, ret);
			TRACE_JOIN("%s: Curl progress failed, error=%d\n", __func__, ret);
			break;
		}
		usleep(10);
	}
	free(mc_obj);

	return ret;
}

/* multicast object libfabric functions */
static struct fi_ops mc_ops = {
	.size = sizeof(struct fi_ops),
	.close = _fi_close_mc,
};

/**
 * Utility routine to set up the collective framework in response to calls to
 * fi_join_collective().
 *
 * This currently supports three different collectives transport models.
 *
 * If jstate->is_rank is true, this is a NETSIM model. This is an early
 * testing model, and is retained for regression testing of the code for code
 * merge. The model requires a PTE for each simulated endpoint in the tree,
 * since the endpoint can only send to itself: there is a single domain (and
 * simulated NIC) under NETSIM. The pid_index is used to simulate multiple
 * "multicast" target endpoints. Setup creates multiple PTEs, one for each
 * simulated endpoint, each using a different pid_index. The NETSIM tests run
 * in isolated test processes, so pid_index values should not conflict with
 * other traffic.
 *
 * If jstate->is_rank is false, and jstate->is_mcast is also false, this is the
 * UNICAST model. This is a test model developed to parallelize development
 * during a period of time when fabric multicast was unavailable, to allow a
 * full multi-node simulation of collectives, and may be deprecated as multicast
 * capability matures. This model requires only a single PTE per domain (NIC).
 * Sends are serialized through each endpoint, but receives can race and become
 * disordered as they pass through the fabric, as will occur in production. The
 * pid_index is set to the reserved value of CXIP_PTL_IDX_COLL, which should not
 * be used by any other traffic on a given NIC, allowing this model to be used
 * concurrently with other traffic.
 *
 * If jstate->is_rank is false and jstate->is_mcast is true, this is the
 * production MULTICAST model. This supports multiple multicast trees, and
 * requires a PTE for each tree, since the pid_index is used to encode the
 * multicast address.
 *
 * Normal PTE setup populates the address portion of the PTE from the domains
 * that have been defined, each domain representing a NIC, and the pid_index
 * sets only the lower pid_width bits of the PTE address to differentiate
 * different traffic streams. However, when a PTE is created with is_mcast=true,
 * the driver code sets the entire PTE address. This calling code must encode
 * the multicast address by bit-shifting it out of the pid_width range. The
 * lower bits are arbitrary, since this PTE cannot receive any other traffic,
 * and are set to zero.
 *
 * Caller must hold ep_obj->lock.
 */
static int _initialize_mc(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_ep_obj *ep_obj = jstate->ep_obj;
	struct cxip_av_set *av_set_obj = jstate->av_set_obj;
	struct cxip_coll_mc *mc_obj;
	struct cxip_coll_pte *coll_pte;
	struct cxip_cmdq *cmdq;
	union cxi_pte_map_offset pid_mcast;
	int pid_idx;
	int red_id;
	int ret;

	TRACE_JOIN("%s entry\n", __func__);

	mc_obj = calloc(1, sizeof(*mc_obj));
	if (!mc_obj) {
		CXIP_WARN("Failed to allocate multicast object (size=%zu)\n",
			  sizeof(*mc_obj));
		return -FI_ENOMEM;
	}

	TRACE_JOIN("acquiring PTE\n");
	if (jstate->is_rank) {
		// NETSIM
		TRACE_JOIN("acquiring PTE NETSIM\n");
		// pid_idx = simulated collective rank
		pid_idx = CXIP_PTL_IDX_COLL + jstate->simrank;
		ret = _acquire_pte(ep_obj, pid_idx, false, &coll_pte);
		// suppress attempt to set multiple times in idm
		coll_pte->mc_obj = mc_obj;
	} else if (!jstate->is_mcast) {
		// UNICAST
		// pid_idx = simulated collective tree
		TRACE_JOIN("acquiring PTE UNICAST\n");
		pid_idx = CXIP_PTL_IDX_COLL;
		ret = _acquire_pte(ep_obj, pid_idx, false, &coll_pte);
	} else {
		// MULTICAST
		// pid_idx = bit-shifted multicast address
		TRACE_JOIN("acquiring PTE MULTICAST\n");
		memset(&pid_mcast, 0, sizeof(pid_mcast));
		pid_mcast.mcast_id = jstate->bcast_data.mcast_addr;
		pid_mcast.mcast_pte_index = 0;
		pid_idx = *((int *)&pid_mcast);
		ret = _acquire_pte(ep_obj, pid_idx, true, &coll_pte);
	}
	if (ret) {
		TRACE_DEBUG("acquiring PTE failed %d\n", ret);
		CXIP_WARN("Failed to acquire PTE (ret=%d, pid_idx=%d, "
			  "is_mcast=%s)\n",
			  ret, pid_idx, jstate->is_mcast ? "true" : "false");
		free(mc_obj);
		return ret;
	}
	/* copy coll_pte to mc_obj */
	mc_obj->coll_pte = coll_pte;

	/* if COMM_KEY_RANK model, PTE must know the mc_obj */
	coll_pte->mc_obj = (jstate->is_rank) ? mc_obj : NULL;

	/* link ep_obj to mc_obj (1 to many) */
	mc_obj->ep_obj = ep_obj;
	ofi_atomic_inc32(&ep_obj->coll.num_mc);

	/* link av_set_obj to mc_obj (one to one) */
	av_set_obj->mc_obj = mc_obj;
	mc_obj->av_set_obj = av_set_obj;

	/* define whether this is multicast */
	switch (av_set_obj->comm_key.keytype) {
	case COMM_KEY_NONE:
	case COMM_KEY_MULTICAST:
		mc_obj->is_multicast = true;
		break;
	default:
		mc_obj->is_multicast = false;
		break;
	}

	/* initialize remainder of mc_obj */
	mc_obj->mc_fid.fid.fclass = FI_CLASS_MC;
	mc_obj->mc_fid.fid.context = mc_obj;
	mc_obj->mc_fid.fid.ops = &mc_ops;
	mc_obj->mc_fid.fi_addr = (fi_addr_t)(uintptr_t)mc_obj;
	mc_obj->hwroot_idx = jstate->bcast_data.hwroot_idx;
	mc_obj->mcast_addr = jstate->bcast_data.mcast_addr;
	mc_obj->mynode_idx = jstate->mynode_idx;
	mc_obj->mynode_fiaddr = jstate->mynode_fiaddr;
	mc_obj->max_red_id = CXIP_COLL_MAX_CONCUR;
	mc_obj->arm_disable = false;
	mc_obj->rx_discard = jstate->rx_discard;
	mc_obj->rootexpires.tv_sec = cxip_env.coll_retry_usec / 1000000L;
	mc_obj->rootexpires.tv_nsec =
		(cxip_env.coll_retry_usec % 1000000L) * 1000L;
	/* leaf timeout before rdma read starts from root node */
	mc_obj->leafexpires.tv_sec = ((cxip_env.coll_retry_usec*CXIP_COLL_MAX_LEAF_TIMEOUT_MULT) / 1000000L);
	mc_obj->leafexpires.tv_nsec =
		(((cxip_env.coll_retry_usec*CXIP_COLL_MAX_LEAF_TIMEOUT_MULT) % 1000000L) * 1000L);
	for (red_id = 0; red_id < CXIP_COLL_MAX_CONCUR; red_id++) {
		struct cxip_coll_reduction *reduction;

		reduction = &mc_obj->reduction[red_id];
		reduction->coll_state = CXIP_COLL_STATE_READY;
		reduction->mc_obj = mc_obj;
		reduction->red_id = red_id;
		reduction->in_use = false;
		reduction->completed = false;
		/* _tsexp() is used for both the retry timer (tv_expires)
		 * and the RE timer (arm_expires).  _tsexp() checks for
		 * TIMER_UNSET to ensure we don't unnecessarily retry when
		 * the reduction doesn't have a timer set on it yet.
		 */
		reduction->tv_expires.tv_sec = TIMER_UNSET;
		reduction->tv_expires.tv_nsec = TIMER_UNSET;
		/* set to 0 to insure we arm for the first operation */
		reduction->arm_expires.tv_sec = 0;
		reduction->arm_expires.tv_nsec = 0;
		reduction->rdma_get_sent = false;
		reduction->rdma_get_completed = false;
		reduction->leaf_contrib_start_us = 0;
		reduction->rdma_get_cb_rc = 0;
		dlist_init(&reduction->tmout_link);
	}
	TRACE_DEBUG("reduction table initialized\n");
	dump_reduction_table(0, &mc_obj->reduction[0]);
	TRACE_DEBUG("Initializing mc_obj=%p counters\n", mc_obj);
	ofi_spin_init(&mc_obj->lock);
	ofi_atomic_initialize32(&mc_obj->send_cnt, 0);
	ofi_atomic_initialize32(&mc_obj->recv_cnt, 0);
	ofi_atomic_initialize32(&mc_obj->pkt_cnt, 0);
	ofi_atomic_initialize32(&mc_obj->seq_err_cnt, 0);
	ofi_atomic_initialize32(&mc_obj->tmout_cnt, 0);

	/* map entire reduction block if using DMA */
	if (cxip_env.coll_use_dma_put) {
		/* EXPERIMENTAL */
		ret = cxil_map(ep_obj->domain->lni->lni,
			       mc_obj->reduction,
			       sizeof(mc_obj->reduction),
			       CXI_MAP_PIN  | CXI_MAP_READ | CXI_MAP_WRITE,
			       NULL, &mc_obj->reduction_md);
		if (ret) {
			CXIP_WARN("Failed to map reduction buffer for DMA "
				  "(ret=%d, size=%zu)\n",
				  ret, sizeof(mc_obj->reduction));
			goto fail;
		}
	}

	/* define the traffic class */
	if (is_netsim(ep_obj)) {
		/* NETSIM RANK model */
		mc_obj->tc = CXI_TC_BEST_EFFORT;
		mc_obj->tc_type = CXI_TC_TYPE_DEFAULT;
	} else if (!jstate->is_mcast) {
		/* UNICAST model */
		mc_obj->tc = CXI_TC_BEST_EFFORT;
		mc_obj->tc_type = CXI_TC_TYPE_DEFAULT;
	} else if (is_hw_root(mc_obj)) {
		/* MULTICAST model, hw_root */
		mc_obj->tc = CXI_TC_LOW_LATENCY;
		mc_obj->tc_type = CXI_TC_TYPE_DEFAULT;
	} else {
		/* MULTICAST model, leaves */
		mc_obj->tc = CXI_TC_LOW_LATENCY;
		mc_obj->tc_type = CXI_TC_TYPE_COLL_LEAF;
	}
	/* Set this now to instantiate cmdq CP */
	cmdq = ep_obj->coll.tx_cmdq;
	ret = cxip_cmdq_cp_set(cmdq, ep_obj->auth_key.vni,
			       mc_obj->tc, mc_obj->tc_type);
	if (ret) {
		TRACE_JOIN("%s: cxip_txq_cp_set() = %d\n", __func__, ret);
		CXIP_WARN("Failed to set command queue control path (ret=%d, "
			  "vni=%d, tc=%d, tc_type=%d)\n",
			  ret, ep_obj->auth_key.vni, mc_obj->tc,
			  mc_obj->tc_type);
		goto fail;
	}

	/* index mc_obj by mcast_addr for fast lookup */
	TRACE_JOIN("%s: mc addr=%d obj=%p\n", __func__, mc_obj->mcast_addr, mc_obj);
	ret =  ofi_idm_set(&ep_obj->coll.mcast_map,
			   mc_obj->mcast_addr, mc_obj);
	if (ret < 0) {
		TRACE_JOIN("%s: idm set failed %d\n", __func__, ret);
		CXIP_WARN("Failed to index multicast object in mcast_map "
			  "(ret=%d, mcast_addr=%d, mc_obj=%p)\n",
			  ret, mc_obj->mcast_addr, mc_obj);
		goto fail;
	}
	/* lock out reuse of this endpoint as hw_root for any multicast addr */
	if (mc_obj->hwroot_idx == mc_obj->mynode_idx) {
		TRACE_JOIN("%s: set is_hwroot\n", __func__);
		/* this is an endpoint level flag, and may not
		 * indicate current root node within the context of
		 * mcast addresses. It is used to prevent a node from being
		 * root more than once, see _finish_bcast().
		 */
		ep_obj->coll.is_hwroot = true;
	} else {
		TRACE_JOIN("%s: node idx is_hwleaf\n", __func__);
		TRACE_JOIN("%s: node idx %d root idx %d\n", __func__, mc_obj->mynode_idx, mc_obj->hwroot_idx);
	}
#if ENABLE_DEBUG
	struct cxip_coll_mc *mc_obj_chk;

	mc_obj_chk = ofi_idm_lookup(&ep_obj->coll.mcast_map,
				    mc_obj->mcast_addr);
	if (mc_obj_chk != mc_obj) {
		TRACE_JOIN("%s: mcast set=%p get=%p\n",
			   __func__, mc_obj, mc_obj_chk);
	}
#endif
	/* Last field to set */
	mc_obj->is_joined = true;

	/* Prepare static metrics for this endpoint*/
	_coll_metrics.ep_data.myrank = mc_obj->mynode_idx;
	_coll_metrics.ep_data.isroot =
		mc_obj->hwroot_idx == mc_obj->mynode_idx;

	/* Initially set close states to success */
	mc_obj->close_state = FI_SUCCESS;
	mc_obj->has_closed = false;
	mc_obj->has_error = false;

	/* Return information to the caller */
	jstate->mc_obj = mc_obj;
	*jstate->mc = &mc_obj->mc_fid;
	TRACE_JOIN("%s: initialized mc[%d] to %p\n",
		   __func__, jstate->mynode_idx, *jstate->mc);

	return FI_SUCCESS;

fail:
	jstate->prov_errno = FI_CXI_ERRNO_JOIN_FAIL_PTE;
	_close_mc(mc_obj, true, true);
	return ret;
}

/**
 * CURL MODEL
 *
 * void _cxip_action(void *object);
 * void _cxip_action_cb(struct cxip_curl_handle *handle);
 *
 * The action object must persist until the action has reached a conclusion,
 * which may involve multiple CURL requests, particularly retries on busy
 * responses. It must retain state for multiple retries of the action if the
 * CURL response indicates a retry is needed. This is the cxip_join_state
 * object for multicast creation, and the mc_obj object for multicast deletion.
 *
 * The curl_usrptr object is allocated for each CURL request, and deleted after
 * the response has been evaluated. The response may be a retry of the same
 * CURL request, or it may be some other recovery or completion operation.
 *
 * This simplifies retries and adaptive responses to the CURL result. The
 * callback function runs as an agent of the CURL processing, using the
 * curl_usrptr object, and can assume that the CURL implementation (cxip_curl.c)
 * will do all CURL memory cleanup, regardless of success or failure. This means
 * that the callback can simply re-issue the same command as if for the first
 * time to perform a retry on any kind of busy error.
 *
 * To prevent endless retries, the elapsing time must be recorded in the
 * action object (so that it will persist across multiple CURL operations).
 */

/**
 * Perform a CURL request to delete a multicast address.
 *
 * This is the last thing done after closing down the mc_object in libfabric, so
 * all that remains is to remove the actual multicast in the FM and delete
 * allocated memory for mc_obj. If the CURL operation cannot complete
 * successfully, the multicast delete will occur at the end of the job.
 */
static void _curl_delete_mc_obj(struct cxip_coll_mc *mc_obj)
{
	struct cxip_curl_mcast_delete_usrptr *curl_usrptr;
	char *url;
	int ret;

	/* early exit will attempt to free these */
	curl_usrptr = NULL;
	url = NULL;

	TRACE_JOIN("deleting multicast address via REST\n");
	ret = asprintf(&url, "%s/%d", cxip_env.coll_fabric_mgr_url,
		       mc_obj->mcast_addr);
	if (ret < 0) {
		TRACE_JOIN("Failed to construct CURL address\n");
		CXIP_WARN("Failed to construct CURL delete URL (ret=%d, "
			  "fm_url='%s', mcast_addr=%d)\n",
			  ret,
			  cxip_env.coll_fabric_mgr_url ?
				  cxip_env.coll_fabric_mgr_url :
				  "NULL",
			  mc_obj->mcast_addr);
		goto quit;
	}
	/* create the return pointer */
	curl_usrptr = calloc(1, sizeof(*curl_usrptr));
	if (!curl_usrptr) {
		TRACE_JOIN("curl_usrptr calloc() error\n");
		CXIP_WARN("Failed to allocate CURL user pointer for multicast "
			  "delete (size=%zu, mcast_addr=%d)\n",
			  sizeof(*curl_usrptr), mc_obj->mcast_addr);
		ret = -FI_ENOMEM;
		goto quit;
	}
	curl_usrptr->mc_obj = mc_obj;
	ret = cxip_curl_perform(url, NULL, cxip_env.coll_mcast_token, 0,
				CURL_DELETE, false, _cxip_delete_mcast_cb,
				curl_usrptr);
	if (ret < 0) {
		TRACE_JOIN("CURL delete mcast %d dispatch failed %d\n",
			   mc_obj->mcast_addr, ret);
		CXIP_WARN("CURL multicast delete dispatch failed (ret=%d, "
			  "mcast_addr=%d, url='%s')\n",
			  ret, mc_obj->mcast_addr, url ? url : "NULL");
		goto quit;
	}
	TRACE_JOIN("CURL delete mcast %d dispatch successful\n",
		   mc_obj->mcast_addr);
quit:
	free(url);
	if (ret < 0) {
		TRACE_JOIN("CURL delete mcast %d failed\n",
			   mc_obj->mcast_addr);
		free(curl_usrptr);
		if (mc_obj->has_error) {
			free(mc_obj);
		} else {
			mc_obj->close_state = ret;
		}
	}
}

static void _cxip_delete_mcast_cb(struct cxip_curl_handle *handle)
{
	struct cxip_curl_mcast_delete_usrptr *curl_usrptr = handle->usrptr;
	struct cxip_coll_mc *mc_obj = curl_usrptr->mc_obj;
	struct json_object *json_obj;
	const char *errmsg = "";

	/* note: allocates space for strings, free at end */
	json_obj = cxip_json_tokener_parse(handle->response);
	if (json_obj) {
		if (cxip_json_string("message", json_obj, &errmsg))
			errmsg = "";
	} else {
		TRACE_JOIN("callback: malformed server response: '%s'\n",
			   handle->response);
	}

	switch (handle->status) {
	case 200:
	case 201:
		TRACE_JOIN("callback: %ld SUCCESS MCAST DELETED\n",
			   handle->status);
		if (mc_obj->has_error) {
			free(mc_obj);
		} else {
			mc_obj->close_state = FI_SUCCESS;
		}
		break;
	case 409:
		TRACE_JOIN("callback: delete mcast failed: %ld '%s'\n",
			   handle->status, errmsg);

		/* retry, but don't DOS the fabric manager */
		usleep(100000);
		_curl_delete_mc_obj(mc_obj);
		break;
	default:
		TRACE_JOIN("callback: %ld unknown status\n", handle->status);
		if (mc_obj->has_error) {
			free(mc_obj);
		} else {
			mc_obj->close_state = FI_CXI_ERRNO_JOIN_CURL_FAILED;
		}
		break;
	}
	/* free json memory */
	cxip_json_object_put(json_obj);
	free(curl_usrptr);
}

/**
 * Perform a CURL request to create a new multicast address.
 */
static void _create_mcast_addr(struct cxip_join_state *jstate)
{
	struct cxip_curl_mcast_create_usrptr *curl_usrptr;
	struct cxip_addr caddr;
	char *jsonreq, *mac, *url, *tok, *p;
	int i, ret;

	/* all exit paths attempt to free these */
	curl_usrptr = NULL;
	jsonreq = NULL;
	mac = NULL;
	url = NULL;
	tok = NULL;

	/* check the environment variables needed */
	TRACE_JOIN("ENV jobid   = %s\n", cxip_env.coll_job_id);
	TRACE_JOIN("ENV stepid  = %s\n", cxip_env.coll_job_step_id);
	TRACE_JOIN("ENV fmurl   = %s\n", cxip_env.coll_fabric_mgr_url);
	TRACE_JOIN("ENV token   = %s\n", cxip_env.coll_mcast_token);
	TRACE_JOIN("ENV maxadrs = %ld\n", cxip_env.hwcoll_addrs_per_job);
	TRACE_JOIN("ENV minnodes= %ld\n", cxip_env.hwcoll_min_nodes);
	TRACE_JOIN("ENV retry   = %ld\n", cxip_env.coll_retry_usec);
	TRACE_JOIN("ENV tmout   = %ld\n", cxip_env.coll_timeout_usec);

	/* Generic error for any preliminary failures */
	ret = 0;
	if (!cxip_env.coll_job_id) {
		TRACE_JOIN("missing job id\n");
		CXIP_WARN("Missing collective job ID\n");
		ret = -FI_EINVAL;
	}
	if (!cxip_env.coll_fabric_mgr_url) {
		TRACE_JOIN("missing FM url\n");
		CXIP_WARN("Missing fabric manager URL\n");
		ret = -FI_EINVAL;
	}
	if (!cxip_env.coll_mcast_token) {
		TRACE_JOIN("missing FM token\n");
		CXIP_WARN("Missing fabric manager authentication token\n");
		ret = -FI_EINVAL;
	}
	if (ret < 0)
		goto quit;

	if (cxip_trap_search(0, CXIP_TRAP_CURL_FM_URL, NULL, NULL))
		ret = asprintf(&url, "%s-bad", cxip_env.coll_fabric_mgr_url);
	else
		ret = asprintf(&url, "%s", cxip_env.coll_fabric_mgr_url);
	if (ret < 0) {
		TRACE_JOIN("failed to construct CURL address\n");
		CXIP_WARN("Failed to construct CURL address (ret=%d)\n", ret);
		ret = -FI_ENOMEM;
		goto quit;
	}
	TRACE_JOIN("final fmurl = %s\n", url);
	if (cxip_trap_search(0, CXIP_TRAP_CURL_TOKEN, NULL, NULL))
		ret = asprintf(&tok, "%s-bad", cxip_env.coll_mcast_token);
	else
		ret = asprintf(&tok, "%s", cxip_env.coll_mcast_token);
	if (ret < 0) {
		TRACE_JOIN("failed to construct CURL token\n");
		CXIP_WARN("Failed to construct CURL token (ret=%d)\n", ret);
		ret = -FI_ENOMEM;
		goto quit;
	}
	TRACE_JOIN("final token = %s\n", tok);

	/* five hex digits per mac, two colons, two quotes, comma */
	p = mac = malloc(10*jstate->av_set_obj->fi_addr_cnt + 1);
	if (!mac) {
		TRACE_JOIN("failed to allocate mac list\n");
		CXIP_WARN("Failed to allocate MAC list for %ld addresses\n",
			  jstate->av_set_obj->fi_addr_cnt);
		ret = -FI_ENOMEM;
		goto quit;
	}
	for (i = 0; i < jstate->av_set_obj->fi_addr_cnt; i++) {
		ret = cxip_av_lookup_addr(
				jstate->av_set_obj->cxi_av,
				jstate->av_set_obj->fi_addr_ary[i], &caddr);
		if (ret < 0) {
			TRACE_JOIN("failed to find address[%d]=%ld\n",
				   i, jstate->av_set_obj->fi_addr_ary[i]);
			CXIP_WARN("Failed to lookup address[%d]=%ld in AV set "
				  "(ret=%d)\n",
				  i, jstate->av_set_obj->fi_addr_ary[i], ret);
			goto quit;
		}
		p += sprintf(p, "'%01X:%02X:%02X',",
		 		(caddr.nic >> 16) & 0xf,
				(caddr.nic >> 8) & 0xff,
				(caddr.nic) & 0xff);

	}
	*(--p) = 0;

	/* generate the CURL JSON request */
	ret = asprintf(&jsonreq,
			"{'macs':[%s],'vni': %d,'timeout':%ld,'jobID':'%s',"
			"'jobStepID':'%s'}",
			mac,
			jstate->ep_obj->auth_key.vni,
			cxip_env.coll_timeout_usec,
			cxip_env.coll_job_id,
			cxip_env.coll_job_step_id);
	if (ret < 0) {
		TRACE_JOIN("failed to create jsonreq= %d\n", ret);
		CXIP_WARN("Failed to create JSON request string (ret=%d, "
			  "vni=%d, job_id='%s', step_id='%s')\n",
			  ret, jstate->ep_obj->auth_key.vni,
			  cxip_env.coll_job_id ? cxip_env.coll_job_id : "NULL",
			  cxip_env.coll_job_step_id ?
				  cxip_env.coll_job_step_id :
				  "NULL");
		ret = -FI_ENOMEM;
		goto quit;
	}
	single_to_double_quote(jsonreq);

	/* create the user return pointer */
	curl_usrptr = calloc(1, sizeof(*curl_usrptr));
	if (!curl_usrptr) {
		TRACE_JOIN("failed to calloc() curl_usrptr\n");
		CXIP_WARN("Failed to allocate CURL user pointer structure "
			  "(size=%zu)\n",
			  sizeof(*curl_usrptr));
		ret = -FI_ENOMEM;
		goto quit;
	}
	/* dispatch CURL request */
	curl_usrptr->jstate = jstate;
	ret = cxip_curl_perform(url, jsonreq, tok, 0, CURL_POST, false,
				_cxip_create_mcast_cb, curl_usrptr);
	if (ret < 0) {
		TRACE_JOIN("CURL create mcast dispatch failed %d\n", ret);
		CXIP_WARN("CURL multicast creation dispatch failed (ret=%d, "
			  "url='%s')\n",
			  ret, url ? url : "NULL");
		goto quit;
	}
	TRACE_JOIN("CURL create mcast dispatch successful\n");
quit:
	free(tok);
	free(url);
	free(mac);
	free(jsonreq);
	if (ret < 0) {
		TRACE_JOIN("CURL create mcast failed\n");
		free(curl_usrptr);
		jstate->prov_errno = FI_CXI_ERRNO_JOIN_CURL_FAILED;
		jstate->finished_mcast = true;
	}
}

static void _cxip_create_mcast_cb(struct cxip_curl_handle *handle)
{
	struct cxip_curl_mcast_create_usrptr *curl_usrptr = handle->usrptr;
	struct cxip_join_state *jstate = curl_usrptr->jstate;
	struct json_object *json_obj;
	struct cxip_addr caddr;
	const char *hwrootstr = "";
	const char *message = "";
	const char *cptr;
	int mcaddr = -1;
	int hwroot = -1;
	int curl_errcode = 0;
	uint32_t octet[6], n;
	int i, ret;

	/* note: allocates space for strings, free at end */
	json_obj = cxip_json_tokener_parse(handle->response);
	if (json_obj) {
		if (cxip_json_string("message", json_obj, &message))
			message = "";
		if (cxip_json_string("hwRoot", json_obj, &hwrootstr))
			hwrootstr = "";
		if (cxip_json_int("mcastID", json_obj, &mcaddr))
			mcaddr = -1;
	} else {
		TRACE_JOIN("callback: malformed server response: '%s'\n",
			   handle->response);
	}
	TRACE_JOIN("%s status   =%ld\n", __func__, handle->status);
	TRACE_JOIN("%s response ='%s'\n", __func__, handle->response);
	TRACE_JOIN("%s message  ='%s'\n", __func__, message);
	TRACE_JOIN("%s hwrootstr='%s'\n", __func__, hwrootstr);
	TRACE_JOIN("%s mcaddr   ='%d'\n", __func__, mcaddr);

	/* Process result */
	switch (handle->status) {
	case 200:
	case 201:
		if (mcaddr < 0 || mcaddr >= 8192) {
			TRACE_JOIN("callback: mcaddr=%d is invalid\n", mcaddr);
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_MCAST_INVALID;
			jstate->finished_mcast = true;
			break;
		}
		memset(octet, 0, sizeof(octet));
		hwroot = 0;
		n = 0;
		if (hwrootstr)
			n = sscanf(hwrootstr, "%x:%x:%x:%x:%x:%x",
				&octet[5], &octet[4], &octet[3],
				&octet[2], &octet[1], &octet[0]);
		if (n < 3) {
			TRACE_JOIN("callback: hwroot '%s' too few octets\n",
				   hwrootstr);
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_HWROOT_INVALID;
			jstate->finished_mcast = true;
			break;
		}
		for (i = 0; i < n; i++)
			hwroot |= octet[i] << (8*i);

		for (i = 0; i < jstate->av_set_obj->fi_addr_cnt; i++) {
			ret = cxip_av_lookup_addr(
					jstate->av_set_obj->cxi_av,
					jstate->av_set_obj->fi_addr_ary[i],
					&caddr);
			if (ret < 0)
				continue;
			if (hwroot == caddr.nic)
				break;
		}
		if (i >= jstate->av_set_obj->fi_addr_cnt) {
			TRACE_JOIN("callback: hwroot rank invalid\n");
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_HWROOT_INVALID;
			jstate->finished_mcast = true;
			break;
		}
		/* Production MCAST address */
		jstate->bcast_data.valid = true;
		jstate->bcast_data.hwroot_idx = i;
		jstate->bcast_data.mcast_addr = (uint32_t)mcaddr;
		jstate->is_mcast = true;
		/* This succeeded */
		TRACE_JOIN("callback: SUCCESS mcaddr=%d hwroot=%d\n",
			   jstate->bcast_data.mcast_addr,
			   jstate->bcast_data.hwroot_idx);
		jstate->prov_errno = 0;
		jstate->finished_mcast = true;
		break;
	case 400:
		TRACE_JOIN("callback: create mcast failed: %ld '%s'\n",
			   handle->status, message ? message : "<empty>");
		jstate->prov_errno = FI_CXI_ERRNO_JOIN_HWROOT_INUSE;
		jstate->finished_mcast = true;
		break;
	case 409:
		TRACE_JOIN("callback: create mcast failed: %ld '%s'\n",
		           handle->status, message);

		/* retry, but don't DOS the fabric manager */
		usleep(100000);
		_create_mcast_addr(jstate);
		break;
	case 507:
		/* find and parse error instance number */
		cptr = message;
		curl_errcode = 0;
		while (cptr && *cptr != ':')
			cptr++;
		if (*cptr == ':') {
			cptr -= 2;
			sscanf(cptr, "%02d:", &curl_errcode);
			TRACE_JOIN("error code = %d\n", curl_errcode);
		}
		switch (curl_errcode) {
		case 1:
			TRACE_JOIN("failed: no mcast, exceeded job limit\n");
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_MCAST_INUSE;
			break;
		case 2:
			TRACE_JOIN("failed: no mcast, no addresses left\n");
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_MCAST_INUSE;
			break;
		case 3:
			TRACE_JOIN("failed: no hwroot available in group\n");
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_HWROOT_INUSE;
			break;
		default:
			TRACE_JOIN("failed: errcode=%d\n", curl_errcode);
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_SERVER_ERR;
			break;
		}
		jstate->finished_mcast = true;
		break;
	default:
		TRACE_JOIN("callback: unhandled CURL error %ld '%s'\n",
			   handle->status, message ? message : "<empty>");
		jstate->prov_errno = FI_CXI_ERRNO_JOIN_SERVER_ERR;
		jstate->finished_mcast = true;
		TRACE_JOIN("jstate->prov_errno = %d\n", jstate->prov_errno);
		break;
	}
	TRACE_JOIN("jstate->prov_errno = %d\n", jstate->prov_errno);
	/* free json memory */
	cxip_json_object_put(json_obj);
	free(curl_usrptr);
}


/****************************************************************************
 * State machine for performing fi_join_collective()
 *
 * The zbcoll operations use unrestricted packets, will re-route dynamically,
 * and manage NAK retries automatically, so they are resistant to dropped
 * packets and other transient errors. They will not (and should not) time out:
 * a persistently unresponsive endpoint in the collective tree will cause the
 * collective join to block indefinitely.
 *
 * Each state operation returns without doing any retries. The state machine
 * progress table will decide whether to retry the operation.
 *
 * Each state operation must set zb->error as follows:
 * - FI_SUCCESS - continues the state machine
 * - FI_EAGAIN  - retries the same state
 * - other      - fails the join operation
 *
 * The bcast_data value is used to carry 64 bits of data.
 * The prov_errno value records a local (speculative) error
 * prov_errno is ignored if bcast_data.valid == true
 *
 * getgroup:
 *	acquires a group ID for zbcoll collectives
 * broadcast (zbcoll rank 0):
 *   if appropriate, starts CURL request, evaluates return
 *   otherwise, assumes static initialization, sets return
 *   on broadcast completion
 *   - all endpoints share bcast_data from zbcoll rank 0
 *   - prov_errno indicates an error if bcast_data.valid is false
 *   - if bcast_data.valid, initializes a new MC object, new PTE if needed
 *   - creation errors set bcast_data.valid false, set prov_errno
 * reduce:
 *   converts this endpoint prov_errno to bitmask
 *   overwrites mcast_addr and hwcoll_idx in bcast_data with bitmask
 *   bcast_data.valid remains unchanged
 *   on reduce completion
 *   - bitmask is bitwise OR of all error bits and address valid bit
 *   - prov_errno is set to prioritized error code (0 if bcast_data.valid)
 *   - all endpoints report the same completion status and error
 */

/**
 * Join state machine.
 *
 * The state machine walks through the following functions top-to-bottom.
 * If the return code is success, it advances to the next state.
 * If the return code is -FI_EAGAIN, it repeats the current state.
 * If the return code is anything else, the join operation fails.
 */

/* suppress repeated BUSY log messages */
static long suppress_busy_log;

/* append a jstate to the zbcoll scheduler */
static void _append_sched(struct cxip_zbcoll_obj *zb, void *usrptr)
{
	struct cxip_ep_coll_obj *coll_obj = &zb->ep_obj->coll;
	struct cxip_join_state *jstate = usrptr;

	dlist_ts_insert_tail(&coll_obj->sched_list, &jstate->sched_link);
}

static void _noop(void *ptr)
{
	TRACE_JOIN("%s: entry\n", __func__);
}

/* get a zbcoll group identifier */
static void _start_getgroup(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;

	TRACE_JOIN("%s on %d: entry\n", __func__, jstate->mynode_idx);

	if (cxip_trap_search(jstate->mynode_idx, CXIP_TRAP_GETGRP, &zb->error,
			     &jstate->prov_errno))
		goto quit;
	/* zb->error == FI_SUCCESS, -FI_EAGAIN, -FI_EINVAL */
	zb->error = cxip_zbcoll_getgroup(zb);
quit:
	TRACE_JOIN("getgroup error = %d\n", zb->error);
	if (zb->error)
		_append_sched(zb, jstate);
}

static void _finish_getgroup(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;

	TRACE_JOIN("%s on %d: entry\n", __func__, jstate->mynode_idx);
	_append_sched(zb, jstate);	// _start_bcast
}

/* Create a multicast address and broadcast it to all endpoints.
 * If jstate->create_mcast is set, this will use CURL to get an address.
 * Otherwise, this presumes static initialization, and sets bcast_data.valid.
 *
 * Caution: re-entrant routine.
 * This routine is called repeatedly by rank 0, returning -FI_EAGAIN to drive
 * the CURL state. See the branch to 'quit' below.
 */
static void _start_bcast(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;
	struct cxip_ep_obj *ep_obj = jstate->ep_obj;

	if (!suppress_busy_log)
		TRACE_JOIN("%s: entry\n", __func__);

	/* rank 0 always does the work here */
	if (jstate->mynode_idx == 0) {
		if (!suppress_busy_log)
			TRACE_JOIN("%s: rank 0\n", __func__);
		if (jstate->create_mcast) {
			/* first call (only) initiates CURL request */
			if (!jstate->creating_mcast) {
				TRACE_JOIN("%s create mcast\n", __func__);
				jstate->creating_mcast = true;

				_create_mcast_addr(jstate);
				TRACE_JOIN("%s create mcast initiated\n", __func__);
			}
			/* every retry call checks to see if CURL is complete */
			if (!jstate->finished_mcast) {
				zb->error = -FI_EAGAIN;
				suppress_busy_log++;
				goto quit;
			}
			TRACE_JOIN("%s create mcast completed\n", __func__);
			suppress_busy_log = 0;
			/* bcast_data.valid is set by curl callback */
		} else {
			/* static bcast data is presumed correct */
			TRACE_JOIN("%s static multicast accepted\n", __func__);
			jstate->bcast_data.valid = true;
		}
	}
	if (cxip_trap_search(jstate->mynode_idx, CXIP_TRAP_BCAST, &zb->error,
			     &jstate->prov_errno))
		goto quit;
	/* make sure we clear this before we go down the zbsend path
	 * _start_rdma_init uses it to send a special cntrl msg
	 * CXIP_CTRL_MSG_ZB_DATA_RDMA_LAC during the join flow with the
	 * same zb context.
	 */	
	ep_obj->coll.leaf_save_root_lac = false;
	/* rank > 0 endpoints overwritten by rank = 0 data */
	/* zb->error == FI_SUCCESS, -FI_EAGAIN, -FI_EINVAL */
	zb->error = cxip_zbcoll_broadcast(zb, &jstate->bcast_data.uint64);
quit:
	if (zb->error)
		_append_sched(zb, jstate);
}

/* Check broadcast validity, and if valid, set up the MC object */
static void _finish_bcast(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;
	bool is_hwroot;
	int ret;

	TRACE_JOIN("%s: mc addr=%d hw_root=%d valid=%d\n",
		   __func__,
		   jstate->bcast_data.mcast_addr,
		   jstate->bcast_data.hwroot_idx,
		   jstate->bcast_data.valid);
	TRACE_JOIN("%s: jstate->prov_errno %d\n", __func__,
		   jstate->prov_errno);
	/* all NICs now have same mc_addr data, if invalid, fail */
	/* jstate->prov_errno is presumed set if not valid */
	if (!jstate->bcast_data.valid)
		goto quit;
	/* error indicates that attempt to configure fails */

	/* check for invalid hwroot index */
	TRACE_JOIN("check hwroot\n");
	if (jstate->bcast_data.hwroot_idx >=
	    jstate->av_set_obj->fi_addr_cnt) {
		CXIP_INFO("Invalid hwroot index: hwroot_idx=%d, "
			  "max_addr_cnt=%zu\n",
			  jstate->bcast_data.hwroot_idx,
			  jstate->av_set_obj->fi_addr_cnt);
		TRACE_JOIN("%s: reject invalid hwroot_idx\n", __func__);
		jstate->prov_errno = FI_CXI_ERRNO_JOIN_HWROOT_INVALID;
		ret = -FI_EINVAL;
		goto quit;
	}

	/* check for hwroot overlap on this node */
	is_hwroot = (jstate->bcast_data.hwroot_idx == jstate->mynode_idx);
	if (is_hwroot && jstate->ep_obj->coll.is_hwroot) {
		CXIP_INFO("Hardware root already in use on this node: "
			  "hwroot_idx=%d, mynode_idx=%d\n",
			  jstate->bcast_data.hwroot_idx, jstate->mynode_idx);
		TRACE_JOIN("%s: reject join, hwroot in use\n", __func__);
		jstate->prov_errno = FI_CXI_ERRNO_JOIN_HWROOT_INUSE;
		ret = -FI_EINVAL;
		goto quit;

	}
	/* check for mcast_addr overlap */
	TRACE_JOIN("check mcast addr\n");
	if (!jstate->is_rank &&
	    ofi_idm_lookup(&jstate->ep_obj->coll.mcast_map,
			   jstate->bcast_data.mcast_addr)) {
		CXIP_INFO("Multicast address already in use: mcast_addr=%d\n",
			  jstate->bcast_data.mcast_addr);
		TRACE_JOIN("%s: reject join, mcast %d in use\n", __func__,
			   jstate->bcast_data.mcast_addr);
		jstate->prov_errno = FI_CXI_ERRNO_JOIN_MCAST_INUSE;
		ret = -FI_EINVAL;
		goto quit;
	}
	jstate->prov_errno = 0;

	if (cxip_trap_search(jstate->mynode_idx, CXIP_TRAP_INITPTE, &ret,
			     &jstate->prov_errno))
		goto quit;

	/* all endpoints initialize with same mcast addr and hwroot */
	ret = _initialize_mc(jstate);
quit:
	/* if initialization fails, invalidate bcast_data */
	if (ret != FI_SUCCESS)
		jstate->bcast_data.valid = false;
	/* represent prov_errno values as inverted bitmask */
	_proverr_to_bits(jstate);
	_append_sched(zb, jstate);	// _start_rdma_init
}
static void _start_rdma_init(void *ptr) {

	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;
	struct cxip_ep_obj *ep_obj = jstate->ep_obj;
	struct cxip_coll_mc *mc_obj = jstate->mc_obj;
	bool is_hwroot = false;
	int addr_index = 0;
	uint64_t data = 0;
	struct cxip_addr dstaddr __attribute__((unused));

	/* if mc_obj is NULL, _finish_bcast did not complete normally */
	if(mc_obj) {
		TRACE_JOIN("%s num_mc %d\n", __func__, ofi_atomic_get32(&ep_obj->coll.num_mc));
		TRACE_JOIN("%s mcast_id %x %p\n", __func__, mc_obj->mcast_addr, mc_obj);
		is_hwroot = is_hw_root(mc_obj);
		data = mc_obj->mcast_addr;
	} else {
		TRACE_JOIN("%s mc_obj is NULL\n",__func__);
		goto append;
	}
	/* netsim loops on join flow, we do not support concurrent joins */
	if (is_netsim(ep_obj)) {
		TRACE_JOIN("%s Skipping netsim active\n",__func__);
		goto append;
	}
	TRACE_JOIN("%s: my_idx %d mc addr=%x hw_root=%d valid=%d ts:%016lx\n",
		__func__, jstate->mynode_idx, jstate->bcast_data.mcast_addr,
		jstate->bcast_data.hwroot_idx, jstate->bcast_data.valid,
		ofi_gettime_us());

	jstate->prov_errno = 0;
	if(is_hwroot) {
		TRACE_JOIN("%s Calling _root_rdma_get_setup()\n", __func__);
		if( _root_rdma_get_setup(jstate) ) {
			TRACE_JOIN("%s: Root rdma get setup failed\n", __func__);
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_FAIL_RDMA;
		} else {
			ep_obj->coll.leaf_save_root_lac = true;
			for(addr_index=0; addr_index < zb->num_caddrs; addr_index++) {
				if(addr_index == jstate->bcast_data.hwroot_idx)
					continue;
				dstaddr = zb->caddrs[addr_index];
				zb->error = FI_SUCCESS;
				zb->reduce = false;
				TRACE_JOIN("%s: rdma index %d pid %x nic %x vni %x\n", __func__,
					addr_index, dstaddr.pid,dstaddr.nic, dstaddr.vni);
				cxip_zbcoll_send(zb, 0, addr_index, data);
			}
			ep_obj->coll.leaf_save_root_lac = false;
		}
	} else {
		TRACE_JOIN("%s Calling _leaf_rdma_get_setup()\n", __func__);
		if( _leaf_rdma_get_setup(jstate) ) {
			TRACE_JOIN("%s: Leaf rdma get setup failed\n", __func__);
			jstate->prov_errno = FI_CXI_ERRNO_JOIN_FAIL_RDMA;
		}
	}
append:
	_append_sched(zb, jstate);	// _finish_rdma_init
}
static void _finish_rdma_init(void *ptr) {

	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;
	struct cxip_ep_obj *ep_obj = jstate->ep_obj;
	struct cxip_coll_mc *mc_obj = jstate->mc_obj;
	bool is_hwroot = false;

	/* if mc_obj is NULL, _finish_bcast did not complete normally */
	if(mc_obj) {
		is_hwroot = is_hw_root(mc_obj);
		TRACE_JOIN("%s mcast_id %x\n",__func__,mc_obj->mcast_addr);
	} else {
		TRACE_JOIN("%s mc_obj is NULL\n",__func__);
		goto append;
	}
	/* netsim loops on join flow, we do not support concurrent joins */
	if (is_netsim(ep_obj)) {
		TRACE_JOIN("%s Skipping netsim active\n",__func__);
		goto append;
	}
	TRACE_JOIN("%s: my_idx %d mc addr=%x hw_root=%d valid=%d ts:%016lx\n",
		__func__, jstate->mynode_idx, jstate->bcast_data.mcast_addr,
		jstate->bcast_data.hwroot_idx, jstate->bcast_data.valid,
		ofi_gettime_us());

	if(is_hwroot) {
			TRACE_JOIN("%s: root rdma tx lac %016lx\n",
				__func__, mc_obj->rdma_get_lac_va_tx);
			TRACE_JOIN("%s: root rdma get md pntr %p\n",
				__func__, mc_obj->root_rdma_get_md);
			TRACE_JOIN("%s: root rdma get data pntr %p\n",
				__func__, mc_obj->root_rdma_get_data_p);
	} else {
			/* this may be 0, depending on if we handled the cb */
			TRACE_JOIN("%s: leaf rdma rx lac %016lx\n",
				__func__, mc_obj->rdma_get_lac_va_rx);
			TRACE_JOIN("%s: leaf rdma get md pntr %p\n",
				__func__, mc_obj->leaf_rdma_get_md);
			TRACE_JOIN("%s: leaf rdma get data pntr %p\n",
				__func__, mc_obj->leaf_rdma_get_data_p);
	}
append:
	_append_sched(zb, jstate);	// _start_reduce
}
/* Accumulate composite errors from different endpoints */
static void _start_reduce(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;

	/* Create an error bitmask from the prov_errno */
	_proverr_to_bits(jstate);
	if (cxip_trap_search(jstate->mynode_idx, CXIP_TRAP_REDUCE, &zb->error,
			     &jstate->prov_errno))
		goto quit;
	/* zb->error == FI_SUCCESS, -FI_EAGAIN, -FI_EINVAL */
	zb->error = cxip_zbcoll_reduce(zb, &jstate->reduce_err.uint64);
quit:
	if (zb->error)
		_append_sched(zb, jstate);
}

/* process error bits (if any) to produce an error condition */
static void _finish_reduce(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct cxip_zbcoll_obj *zb = jstate->zb;

	TRACE_JOIN("%s: entry\n", __func__);

	/* re-invert bitmap, select common reported error */
	_bits_to_proverr(jstate);

	TRACE_JOIN("%s: prov_errno=0x%x\n", __func__, jstate->prov_errno);
	_append_sched(zb, jstate);	// _start_cleanup
}

/* state machine cleanup */
static void _start_cleanup(void *ptr)
{
	struct cxip_join_state *jstate = ptr;
	struct fi_eq_err_entry entry = {};
	size_t size = sizeof(entry);
	uint64_t flags = 0L;
	int ret;

	TRACE_JOIN("%s: entry\n", __func__);
	if (jstate) {
		entry.fid = (jstate->mc_obj) ?
				&jstate->mc_obj->mc_fid.fid : NULL;
		entry.context = jstate->context;

		if (jstate->prov_errno >= FI_CXI_ERRNO_JOIN_FIRST) {
			size = sizeof(struct fi_eq_err_entry);
			entry.data = FI_JOIN_COMPLETE;
			entry.err = -FI_ECONNREFUSED;
			entry.prov_errno = jstate->prov_errno;
			flags |= UTIL_FLAG_ERROR;
		}
		ret = ofi_eq_write(&jstate->ep_obj->eq->util_eq.eq_fid,
				FI_JOIN_COMPLETE, &entry,
				size, flags);
		if (ret < 0)
			CXIP_INFO("FATAL ERROR: cannot post to EQ\n");
		cxip_zbcoll_free(jstate->zb);
		jstate->ep_obj->coll.join_busy = false;
	}
	free(jstate);
}

typedef	void (*sched_func)(void *ptr);

enum state_code {
	state_init,
	start_getgroup,
	finish_getgroup,
	start_bcast,
	finish_bcast,
	start_rdma_init,
	finish_rdma_init,
	start_reduce,
	finish_reduce,
	start_cleanup,
	state_done
};

const char *state_name[] = {
	"state_init",
	"start_getgroup",
	"finish_getgroup",
	"start_bcast",
	"finish_bcast",
	"start_rdma_init",
	"finish_rdma_init",
	"start_reduce",
	"finish_reduce",
	"start_cleanup",
	"state_done"
};
sched_func state_func[] = {
	_noop,
	_start_getgroup,
	_finish_getgroup,
	_start_bcast,
	_finish_bcast,
	_start_rdma_init,
	_finish_rdma_init,
	_start_reduce,
	_finish_reduce,
	_start_cleanup,
	_noop,
};

/**
 * State progression table
 *
 * Row is the current state.
 * Col contains states reachable from this state on success/again/fail.
 */
static enum state_code progress_state[][3] = {
	/* STATE              SUCCESS         EAGAIN         FAIL */
	/* state_init       */ {start_getgroup, start_cleanup, start_cleanup},
	/* start_getgroup   */ {finish_getgroup,start_getgroup,start_cleanup},
	/* finish_getgroup  */ {start_bcast,    start_cleanup, start_cleanup},
	/* start_bcast      */ {finish_bcast,   start_bcast,   start_cleanup},
	/* finish_bcast     */ {start_rdma_init,   start_cleanup, start_cleanup},
	/* start_rdma_init  */ {finish_rdma_init,   start_cleanup,   start_cleanup},
	/* finish_rdma_init */ {start_reduce,   start_cleanup, start_cleanup},
	/* start_reduce     */ {finish_reduce,  start_reduce,  start_cleanup},
	/* finish_reduce    */ {start_cleanup,  start_cleanup, start_cleanup},
	/* start_cleanup    */ {state_done,     state_done,    state_done},
	/* state_done       */ {state_done,     state_done,    state_done},
};

/* Advance the state and run scheduled operations */
static void _progress_sched(struct cxip_join_state *jstate)
{
	struct cxip_zbcoll_obj *zb = jstate->zb;
	enum state_code *codes;

	/* acquire the success/again/fail state codes for current state */
	codes = progress_state[jstate->sched_state];
	switch (zb->error) {
	case FI_SUCCESS:
		/* last operation succeeded */
		TRACE_JOIN("%s: success\n", __func__);
		jstate->sched_state = codes[0];
		suppress_busy_log = 0;
		break;
	case -FI_EBUSY:
	case -FI_EAGAIN:
		/* last operation needs a retry */
		if (!suppress_busy_log)
			TRACE_JOIN("%s: busy retry\n", __func__);
		jstate->sched_state = codes[1];
		suppress_busy_log++;
		break;
	default:
		/* last operation failed */
		TRACE_JOIN("%s: fail zberr=%d\n", __func__, zb->error);
		jstate->sched_state = codes[2];
		break;
	}
	if (!suppress_busy_log)
		TRACE_JOIN("----> jstate[%d,%d]=%s\n",
			jstate->join_idx, jstate->mynode_idx,
			state_name[jstate->sched_state]);

	/* execute the new state function */
	state_func[jstate->sched_state](jstate);
}

/* Process the schedule list and dispatch next scheduled operation */
static void _progress_join(struct cxip_ep_obj *ep_obj)
{
	struct cxip_ep_coll_obj *coll_obj = &ep_obj->coll;
	struct cxip_join_state *jstate = NULL;

	dlist_ts_pop_front(&coll_obj->sched_list,
			   struct cxip_join_state,
			   jstate, sched_link);

	if (jstate)
		_progress_sched(jstate);
}

/* During join, determine my index position in the av_set_obj */
static unsigned int _caddr_to_idx(struct cxip_av_set *av_set_obj,
				  struct cxip_addr caddr)
{
	struct cxip_addr addr;
	size_t size = sizeof(addr);
	int i, ret;

	for (i = 0; i < av_set_obj->fi_addr_cnt; i++) {
		ret = fi_av_lookup(&av_set_obj->cxi_av->av_fid,
				   av_set_obj->fi_addr_ary[i],
				   &addr, &size);
		if (ret) {
			CXIP_INFO("AV lookup failed during address search: "
				  "index=%d, fi_addr=%lu, ret=%d\n",
				  i, av_set_obj->fi_addr_ary[i], ret);
			return ret;
		}
		if (CXIP_ADDR_EQUAL(addr, caddr))
			return i;
	}
	CXIP_INFO("Address not found in AV set: target_nic=0x%x, "
		  "target_pid=0x%x, searched_count=%ld\n",
		  caddr.nic, caddr.pid, av_set_obj->fi_addr_cnt);
	return -FI_EADDRNOTAVAIL;
}

/**
 * fi_join_collective() implementation.
 *
 * Calling syntax is defined by libfabric.
 *
 * This is a multi-stage collective operation, progressed by calling TX/RX CQs
 * and the EQ for the endpoint. Upon completion of the state machine, the EQ
 * will return an EQ event structure.
 *
 * We go through the following steps:
 *
 * 1) allocate a join state for this operation
 * 2) allocate zbcoll object
 * 3) get a collective group identifier
 * 4) generate a multicast tree from NIC 0
 * 5) broadcast multicast address from NIC 0
 * 6) reduce error mask across all NICs
 * 7) cleanup
 *
 * Joins are non-concurrent, and return FI_EAGAIN until any active join
 * completes. The final return code of a join is not known to all nodes until
 * the final state completes.
 *
 * Joins are progressed by polling TX/RX CQs, and completion status is
 * returned by polling the endpoint EQ.
 *
 * CPU errors like -FI_ENOMEM will likely occur on individual endpoints,
 * and the correct response is to exit the application. There is no
 * reasonable way to re-enter the state machine once any participant has
 * unexpectedly failed.
 *
 * Internal errors, such as inability to acquire a multicast address, are
 * are represented by a CXIP_PROV_ERRNO value, which is returned through the
 * EQ polling with an error of -FI_EAVAIL, and the CXIP_PROV_ERRNO value.
 * These values are ranked, and if multiple nodes show different errors, the
 * returned error will be the most-significant (most-negative) value.
 *
 * There are four operational models, one for production, and three for testing.
 *
 * In all cases, there must be one join for every NIC address in the av_set_obj
 * fi_addr_ary, and the collective proceeds among these joined endpoints.
 *
 * COMM_KEY_RANK tests using a single process on a single Cassini, which
 * supplies the src/tgt, but different pid_idx values, representing different
 * PTLTE objects, each with its own buffers. The zbcoll operations are performed
 * using linked zb objects, which represent a single zbcoll collective, so each
 * zb callback function is called only once for the entire set, yet must provide
 * a unique mc return value and FI_COLL_COMPLETE event for each joined object.
 * We manage this with the simstates array, which associates the simulated rank
 * with the state pointer, so that upon completion, we can provide all of the
 * return pointers and events.
 *
 * COMM_KEY_UNICAST tests on multiple nodes on a real network, but without any
 * multicast support. It initializes one mc object on each node, and designates
 * the first node in the multicast list, fiaddr[0], as the hardware root node.
 * fiaddr[1..N] send directly to fiaddr[0], and fiaddr[0] sends to each of the
 * other addresses in a simulated broadcast. This is not expected to be
 * performant, but it does exercise a necessary incast edge case, and it fully
 * exercises the collectives software across multiple nodes.
 *
 * COMM_KEY_MULTICAST is a fully-functioning model, but requires that an
 * external application prepare the multicast address on the fabric before
 * calling fi_join_collective() on any node. This information must be supplied
 * through the av_set_obj->comm_key structure.
 *
 * COMM_KEY_NONE is the production model, in which fi_join_collective() creates
 * the multicast address by making a CURL call to the fabric manager REST API.
 * fiaddr[0] manages the CURL call, and broadcasts the results to all of the
 * other objects across the collective group.
 */
int cxip_join_collective(struct fid_ep *ep, fi_addr_t coll_addr,
			 const struct fid_av_set *coll_av_set,
			 uint64_t flags, struct fid_mc **mc, void *context)
{
	struct cxip_ep *cxip_ep;
	struct cxip_ep_obj *ep_obj;
	struct cxip_av_set *av_set_obj;
	struct cxip_join_state *jstate;
	struct cxip_zbcoll_obj *zb;
	bool link_zb;
	int ret;
	
	if(cxip_collectives_supported) {
		TRACE_JOIN("%s: CXI Collectives are supported\n", __func__);
	}
	else {
		CXIP_WARN("CXI Collectives are not supported on this system\n");
		TRACE_JOIN("%s: CXI Collectives are not supported\n", __func__);
		return -FI_EOPNOTSUPP;
	}

	check_red_pkt();

	TRACE_JOIN("%s: entry\n", __func__);
	/* Validate arguments */
	if (!ep || !coll_av_set || !mc || coll_addr != FI_ADDR_NOTAVAIL) {
		CXIP_WARN("Invalid parameters: ep=%p, coll_av_set=%p, mc=%p, "
			  "coll_addr=%lu\n",
			  ep, coll_av_set, mc, coll_addr);
		return -FI_EINVAL;
	}
	/* flags are ignored, per util_coll.c example code
	 * Only FI_SCATTER is documented, and applies to fi_query_collective().
	 */

	cxip_ep = container_of(ep, struct cxip_ep, ep.fid);
	av_set_obj = container_of(coll_av_set, struct cxip_av_set, av_set_fid);
	jstate = NULL;
	zb = NULL;
	*mc = NULL;

	ep_obj = cxip_ep->ep_obj;

	/* join must be serialized through to completion */
	ofi_genlock_lock(&ep_obj->lock);
	if (ep_obj->coll.join_busy) {
		CXIP_INFO("Collective join already in progress for this "
			  "endpoint\n");
		ofi_genlock_unlock(&ep_obj->lock);
		return -FI_EAGAIN;
	}
	/* SHORT-TERM HACK see NETCASSINI-5771 */
	if (av_set_obj->comm_key.keytype != COMM_KEY_RANK)
		ep_obj->coll.join_busy = true;
	ofi_genlock_unlock(&ep_obj->lock);

	/* allocate state to pass arguments through callbacks */
	jstate = calloc(1, sizeof(*jstate));
	if (!jstate) {
		CXIP_WARN("Failed to allocate join state: size=%zu\n",
			  sizeof(*jstate));
		ret = -FI_ENOMEM;
		goto fail;
	}

	jstate->ep_obj = ep_obj;
	jstate->av_set_obj = av_set_obj;
	jstate->mc = mc;
	jstate->context = context;
	jstate->join_flags = flags;
	jstate->sched_state = state_init;
	jstate->join_idx = ofi_atomic_inc32(&ep_obj->coll.join_cnt);

	/* rank 0 (av_set_obj->fi_addr_cnt[0]) does zb broadcast, so all nodes
	 * will share whatever bcast_data rank 0 ends up with.
	 */

	ret = -FI_EINVAL;
	switch (av_set_obj->comm_key.keytype) {
	case COMM_KEY_NONE:
		/* Production case, acquire multicast from FM */
		if (is_netsim(ep_obj)) {
			CXIP_WARN("NETSIM COMM_KEY_NONE not supported\n");
			goto fail;
		}
		TRACE_JOIN("%s: MULTICAST CURL model setup\n", __func__);
		jstate->mynode_idx =
			_caddr_to_idx(av_set_obj, ep_obj->src_addr);
		jstate->mynode_fiaddr =
			av_set_obj->fi_addr_ary[jstate->mynode_idx];
		jstate->simrank = ZB_NOSIM;
		jstate->bcast_data.hwroot_idx = 0;
		jstate->bcast_data.mcast_addr = 0;
		jstate->bcast_data.valid = false;
		jstate->is_rank = false;
		jstate->is_mcast = true;
		jstate->create_mcast = (jstate->mynode_idx == 0);
		jstate->rx_discard = true;
		link_zb = false;
		break;
	case COMM_KEY_MULTICAST:
		/* Real network test with predefined multicast address */
		if (is_netsim(ep_obj)) {
			CXIP_WARN("NETSIM COMM_KEY_MULTICAST not supported\n");
			goto fail;
		}
		TRACE_JOIN("%s: MULTICAST prefab model setup\n", __func__);
		jstate->mynode_idx =
			_caddr_to_idx(av_set_obj, ep_obj->src_addr);
		jstate->mynode_fiaddr =
			av_set_obj->fi_addr_ary[jstate->mynode_idx];
		jstate->simrank = ZB_NOSIM;
		jstate->bcast_data.hwroot_idx =
			av_set_obj->comm_key.mcast.hwroot_idx;
		jstate->bcast_data.mcast_addr =
			av_set_obj->comm_key.mcast.mcast_addr;
		jstate->bcast_data.valid = true;
		jstate->is_rank = false;
		jstate->is_mcast = true;
		jstate->create_mcast = false;
		jstate->rx_discard = true;
		link_zb = false;
		break;
	case COMM_KEY_UNICAST:
		/* Real network test without multicast address */
		if (is_netsim(ep_obj)) {
			CXIP_WARN("NETSIM COMM_KEY_UNICAST not supported\n");
			goto fail;
		}
		TRACE_JOIN("%s: UNICAST model setup\n", __func__);
		jstate->mynode_idx =
			_caddr_to_idx(av_set_obj, ep_obj->src_addr);
		jstate->mynode_fiaddr =
			av_set_obj->fi_addr_ary[jstate->mynode_idx];
		jstate->simrank = ZB_NOSIM;
		jstate->bcast_data.hwroot_idx =
			av_set_obj->comm_key.ucast.hwroot_idx;
		jstate->bcast_data.mcast_addr =
			av_set_obj->comm_key.ucast.mcast_addr;
		jstate->bcast_data.valid = false;
		jstate->is_rank = false;
		jstate->is_mcast = false;
		jstate->create_mcast = false;
		jstate->rx_discard = true;
		link_zb = false;
		break;
	case COMM_KEY_RANK:
		/* Single process simulation, can run under NETSIM */
		TRACE_JOIN("%s: COMM_KEY_RANK model setup\n", __func__);
		jstate->mynode_idx = av_set_obj->comm_key.rank.rank;
		jstate->mynode_fiaddr = (fi_addr_t)jstate->mynode_idx;
		jstate->simrank = jstate->mynode_idx;
		jstate->bcast_data.hwroot_idx = 0;
		jstate->bcast_data.mcast_addr = ep_obj->src_addr.nic;
		jstate->bcast_data.valid = true;
		jstate->is_rank = true;
		jstate->is_mcast = false;
		jstate->create_mcast = false;
		jstate->rx_discard = av_set_obj->comm_key.rank.rx_discard;
		link_zb = true;
		break;
	default:
		CXIP_WARN("Unexpected comm_key keytype: %d\n",
			  av_set_obj->comm_key.keytype);
		goto fail;
	}

	/* Reject if a rank tries to join a group it doesn't belong to */
	ret = jstate->mynode_idx;
	if (ret < 0) {
		CXIP_WARN("Node not found in collective group: mynode_idx=%d\n",
			  ret);
		TRACE_JOIN("May not participate\n");
		goto fail;
	}

	/* Acquire a zbcoll identifier */
	TRACE_JOIN("%s: allocate zb\n", __func__);
	ret = cxip_zbcoll_alloc(jstate->ep_obj,
				jstate->av_set_obj->fi_addr_cnt,
				jstate->av_set_obj->fi_addr_ary,
				jstate->simrank, &zb);
	TRACE_JOIN("%s: returned=%d\n", __func__, ret);
	if (ret) {
		CXIP_WARN("ZB collective allocation failed: addr_cnt=%zu, "
			  "simrank=%d, ret=%d\n",
			  jstate->av_set_obj->fi_addr_cnt, jstate->simrank,
			  ret);
		goto fail;
	}

	/* Install the callback function for zb collectives */
	TRACE_JOIN("%s: cxip_zbcoll_set_user_cb\n", __func__);
	cxip_zbcoll_set_user_cb(zb, _append_sched, jstate);

	/* If COMM_KEY_RANK, join is called for each rank */
	if (link_zb) {
		static struct cxip_zbcoll_obj *zb0 = NULL;
		static int zb0_count = 0;
		int rank = av_set_obj->comm_key.rank.rank;

		/* first call sets the zb0 simulated endpoint */
		TRACE_JOIN("%s: rank = %d, zb0_count=%d\n", __func__, rank, zb0_count);
		if (!zb0_count++) {
			/* first must be rank 0 */
			if (rank != 0) {
				CXIP_WARN("First COMM_KEY_RANK join must be "
					  "rank 0: actual_rank=%d\n",
					  rank);
				TRACE_JOIN("%s: rank %d not 0\n", __func__, rank);
				ret = -FI_EINVAL;
				goto fail;
			}
			zb0 = zb;
			TRACE_JOIN("%s: zb0=%p zb=%p\n", __func__, zb0, zb);
		}
		/* link this zb to zb0 */
		ret = cxip_zbcoll_simlink(zb0, zb);
		if (ret) {
			CXIP_WARN("ZB collective simulation link failed: "
				  "zb0=%p, zb=%p, ret=%d\n",
				  zb0, zb, ret);
			TRACE_JOIN("%s: return=%d\n", __func__, ret);
			return ret;
		}
		/* after the last, we need to reset this */
		if (zb0_count == av_set_obj->fi_addr_cnt) {
			zb0_count = 0;
			zb0 = NULL;
		}
	}

	jstate->zb = zb;
	_append_sched(zb, jstate);

	return FI_SUCCESS;

fail:
	/* this path returns error, does not post to EQ */
	TRACE_JOIN("cxip_join_collective, ret=%d\n", ret);
	cxip_zbcoll_free(zb);
	free(jstate);
	ep_obj->coll.join_busy = false;

	return ret;
}

/* Exported to be called by EQ read function */
void cxip_coll_progress_join(struct cxip_ep_obj *ep_obj)
{
	ofi_genlock_lock(&ep_obj->lock);

	/* progress the work schedule */
	_progress_join(ep_obj);

	/* don't want handle returned, callback function manages it */
	cxip_curl_progress(NULL);

	/* progress the underlying zbcoll */
	cxip_ep_zbcoll_progress(ep_obj);

	ofi_genlock_unlock(&ep_obj->lock);
}

/* Reset all of the diagnostic counters */
void cxip_coll_reset_mc_ctrs(struct fid_mc *mc)
{
	struct cxip_coll_mc *mc_obj = (struct cxip_coll_mc *)mc;

	ofi_atomic_set32(&mc_obj->coll_pte->recv_cnt, 0);
	ofi_atomic_set32(&mc_obj->send_cnt, 0);
	ofi_atomic_set32(&mc_obj->recv_cnt, 0);
	ofi_atomic_set32(&mc_obj->pkt_cnt, 0);
	ofi_atomic_set32(&mc_obj->seq_err_cnt, 0);
	ofi_atomic_set32(&mc_obj->tmout_cnt, 0);
}

void cxip_coll_get_mc_ctrs(struct fid_mc *mc, struct coll_counters *counters)
{
	struct cxip_coll_mc *mc_obj = (struct cxip_coll_mc *)mc;

	counters->coll_recv_cnt = ofi_atomic_get32(&mc_obj->coll_pte->recv_cnt);
	counters->send_cnt = ofi_atomic_get32(&mc_obj->send_cnt);
	counters->recv_cnt = ofi_atomic_get32(&mc_obj->recv_cnt);
	counters->pkt_cnt = ofi_atomic_get32(&mc_obj->pkt_cnt);
	counters->seq_err_cnt = ofi_atomic_get32(&mc_obj->seq_err_cnt);
	counters->tmout_cnt = ofi_atomic_get32(&mc_obj->tmout_cnt);
}

/****************************************************************************
 * Manage the static coll structure in the EP. Because of its specialized
 * nature, it made sense to manage it here, rather than in the EP module.
 */
struct fi_ops_collective cxip_collective_ops = {
	.size = sizeof(struct fi_ops_collective),
	.barrier = cxip_barrier,
	.broadcast = cxip_broadcast,
	.alltoall = fi_coll_no_alltoall,
	.allreduce = cxip_allreduce,
	.allgather = fi_coll_no_allgather,
	.reduce_scatter = fi_coll_no_reduce_scatter,
	.reduce = cxip_reduce,
	.scatter = fi_coll_no_scatter,
	.gather = fi_coll_no_gather,
	.msg = fi_coll_no_msg,
};

struct fi_ops_collective cxip_collective_no_ops = {
	.size = sizeof(struct fi_ops_collective),
	.barrier = fi_coll_no_barrier,
	.broadcast = fi_coll_no_broadcast,
	.alltoall = fi_coll_no_alltoall,
	.allreduce = fi_coll_no_allreduce,
	.allgather = fi_coll_no_allgather,
	.reduce_scatter = fi_coll_no_reduce_scatter,
	.reduce = fi_coll_no_reduce,
	.scatter = fi_coll_no_scatter,
	.gather = fi_coll_no_gather,
	.msg = fi_coll_no_msg,
};

/* Close collectives - called during EP close, ep_obj->lock is held.
 * This does not issue CURL requests to delete multicast addresses.
 *
 * This is called as part of an endpoint shutdown, which is part of an
 * application shutdown, and the SLURM cleanup handler will destroy all
 * multicast addresses with an efficient method that deletes all per-job
 * addresses. The concern is that if there is a large count of multicast
 * addresses, deleting them individually in this code will create a delay,
 * and could clog the REST API.
 */
void cxip_coll_close(struct cxip_ep_obj *ep_obj)
{
	struct cxip_coll_mc *mc_obj;

	while (!dlist_empty(&ep_obj->coll.mc_list)) {
		dlist_pop_front(&ep_obj->coll.mc_list,
				struct cxip_coll_mc, mc_obj, entry);
		_close_mc(mc_obj, false, true);
	}
}

/**
 * Initialize collectives - call during EP init */
void cxip_coll_init(struct cxip_ep_obj *ep_obj)
{
	cxip_coll_populate_opcodes();

	memset(&ep_obj->coll.mcast_map, 0, sizeof(ep_obj->coll.mcast_map));
	dlist_ts_init(&ep_obj->coll.sched_list);
	dlist_init(&ep_obj->coll.mc_list);
	ep_obj->coll.rx_cmdq = NULL;
	ep_obj->coll.tx_cmdq = NULL;
	ep_obj->coll.rx_cntr = NULL;
	ep_obj->coll.tx_cntr = NULL;
	ep_obj->coll.rx_evtq = NULL;
	ep_obj->coll.tx_evtq = NULL;
	ep_obj->coll.min_multi_recv = CXIP_COLL_MIN_MULTI_RECV;
	ep_obj->coll.buffer_count = CXIP_COLL_MIN_RX_BUFS;
	ep_obj->coll.buffer_size = CXIP_COLL_MIN_RX_SIZE;

	ofi_atomic_initialize32(&ep_obj->coll.num_mc, 0);
	ofi_atomic_initialize32(&ep_obj->coll.join_cnt, 0);
}

/**
 * Enable collectives - call from EP enable.
 */
int cxip_coll_enable(struct cxip_ep *ep)
{
	struct cxip_ep_obj *ep_obj = ep->ep_obj;

	if (ep_obj->coll.enabled)
		return FI_SUCCESS;

	if (!(ep_obj->caps & FI_COLLECTIVE)) {
		CXIP_INFO("FI_COLLECTIVE not requested\n");
		return FI_SUCCESS;
	}

	/* A read-only or write-only endpoint is legal */
	if (!(ofi_recv_allowed(ep_obj->rxc->attr.caps) &&
	      ofi_send_allowed(ep_obj->txc->attr.caps))) {
		CXIP_INFO("EP not recv/send, collectives not enabled\n");
		return FI_SUCCESS;
	}

	/* Sanity checks */
	if (ep_obj->coll.buffer_size == 0) {
		CXIP_WARN("Invalid configuration: buffer_size is 0\n");
		return -FI_EINVAL;
	}
	if (ep_obj->coll.buffer_count == 0) {
		CXIP_WARN("Invalid configuration: buffer_count is 0\n");
		return -FI_EINVAL;
	}
	if (ep_obj->coll.min_multi_recv == 0) {
		CXIP_WARN("Invalid configuration: min_multi_recv is 0\n");
		return -FI_EINVAL;
	}
	if (ep_obj->coll.min_multi_recv >= ep_obj->coll.buffer_size) {
		CXIP_WARN("Invalid configuration: min_multi_recv(%zu) >= "
			  "buffer_size(%zu)\n",
			  ep_obj->coll.min_multi_recv,
			  ep_obj->coll.buffer_size);
		return -FI_EINVAL;
	}

	/* Bind all STD EP objects to the coll object */
	ep_obj->coll.rx_cmdq = ep_obj->rxc->rx_cmdq;
	ep_obj->coll.tx_cmdq = ep_obj->txc->tx_cmdq;
	ep_obj->coll.rx_cntr = ep_obj->rxc->recv_cntr;
	ep_obj->coll.tx_cntr = ep_obj->txc->send_cntr;
	ep_obj->coll.rx_evtq = &ep_obj->rxc->rx_evtq;
	ep_obj->coll.tx_evtq = &ep_obj->txc->tx_evtq;
	ep_obj->coll.eq = ep_obj->eq;

	ep->ep.collective = &cxip_collective_ops;
	ep_obj->coll.enabled = true;

	dlist_init(&ep_obj->coll.root_retry_list);
	dlist_init(&ep_obj->coll.leaf_rdma_get_list);

	cxip_coll_init_metrics();
	cxip_coll_trace_init();
	return FI_SUCCESS;
}

/* Disable collectives - call from EP disable */
int cxip_coll_disable(struct cxip_ep_obj *ep_obj)
{
	if (!ep_obj->coll.enabled)
		return FI_SUCCESS;

	cxip_coll_trace_close();
	ep_obj->coll.enabled = false;
	ep_obj->coll.rx_cmdq = NULL;
	ep_obj->coll.tx_cmdq = NULL;
	ep_obj->coll.rx_cntr = NULL;
	ep_obj->coll.tx_cntr = NULL;
	ep_obj->coll.rx_evtq = NULL;
	ep_obj->coll.tx_evtq = NULL;
	ep_obj->coll.eq = NULL;

	return FI_SUCCESS;
}

static int _root_rdma_get_setup(struct cxip_join_state *jstate)
{
	struct cxip_ep_obj *ep_obj;
	struct cxip_coll_mc *mc_obj;
	struct cxip_domain *dom;
	struct cxip_md *cxip_md;
	uint8_t *buf;
	uint32_t len = (sizeof(struct red_pkt_64) * CXIP_COLL_MAX_CONCUR);
	int lock_held=0, ret=0;
	uint64_t iova=0, lac=0;
	struct cxip_txc_hpc *txc;
	struct red_pkt_64 *pkt_p;
	uint32_t *data, index=0, tag=0xdead;

	if(!jstate) {
		TRACE_DEBUG("%s: Bad input params!\n", __func__);
		return FI_EINVAL;
	}
	ep_obj = jstate->ep_obj;
	mc_obj = jstate->mc_obj;
	dom = ep_obj->domain;
	mc_obj->rdma_get_lac_va_tx = 0;
	mc_obj->root_rdma_get_data_p = calloc(1, len);
	buf = mc_obj->root_rdma_get_data_p;
	if(!buf) {
		TRACE_DEBUG("%s: Unable to calloc RDMA get base pointer\n", __func__);
		return FI_ENOMEM;
	}
	pkt_p = (struct red_pkt_64 *)buf;
	/* Init the root buffers with a known pattern */
	for (index=0; index<CXIP_COLL_MAX_CONCUR; index++) {
		data = (uint32_t *)&pkt_p[index];
		data[0] = (tag<<16);
		data[0] |= index;
		TRACE_DEBUG("%s: pkt addr %p index %d data[0] - %08x\n", __func__, data, index, data[0]);
	}
	TRACE_DEBUG("%s: RDMA get base pointer %p\n", __func__, ((void *)buf));
	/* setup code needs to be envoked holding the endpoint lock */
	lock_held = ofi_genlock_held(&ep_obj->lock);
	TRACE_DEBUG("%s: ep lock held status %d\n", __func__, lock_held);
	if(!lock_held)
		ofi_genlock_lock(&ep_obj->lock);

	ret = cxip_map(dom, buf, len, CXI_MAP_READ, 0, &mc_obj->root_rdma_get_md);
	if (ret != FI_SUCCESS) {
		TRACE_DEBUG("%s: cxip_map() failed! %d\n", __func__, ret);
		if(!lock_held)
			ofi_genlock_unlock(&ep_obj->lock);
		return ret;
	} else
		TRACE_DEBUG("%s: cxip_map() success! %d\n", __func__, ret);

	/* we unmap it in _close_mc */
	cxip_md = mc_obj->root_rdma_get_md;
	iova = CXI_VA_TO_IOVA(cxip_md->md, buf);
	/* Build the LAC used by the leafs, first save the virtual address */
	mc_obj->rdma_get_lac_va_tx = iova;
	TRACE_DEBUG("%s: virtual address %16lx\n", __func__, iova);
	/* we use default protocol, so all we need is the LAC, stuff that in the upper 3 bits,0-7 ordinal */
	lac = cxip_md->md->lac;
	TRACE_DEBUG("%s: LAC %16lx\n", __func__, lac);
	mc_obj->rdma_get_lac_va_tx |= (lac<<61);
	TRACE_DEBUG("%s: final lac|va %16lx\n", __func__, mc_obj->rdma_get_lac_va_tx);
	txc = container_of(ep_obj->txc, struct cxip_txc_hpc, base);
	ret = cxip_rdzv_pte_src_req_alloc(txc->rdzv_pte, cxip_md->md->lac);
	if (ret != FI_SUCCESS) {
		TRACE_DEBUG("%s: cxip_rdzv_pte_src_req_alloc() failed! %d\n", __func__, ret);
		if(!lock_held)
			ofi_genlock_unlock(&ep_obj->lock);
		return ret;
	} else
		TRACE_DEBUG("%s: cxip_rdzv_pte_src_req_alloc() success! %d\n", __func__, ret);

	if(!lock_held)
		ofi_genlock_unlock(&ep_obj->lock);


	return FI_SUCCESS;
}
static int _leaf_rdma_get_setup(struct cxip_join_state *jstate)
{
	struct cxip_ep_obj *ep_obj = jstate->ep_obj;
	struct cxip_coll_mc *mc_obj = jstate->mc_obj;
	uint32_t len = (sizeof(struct red_pkt_64) * CXIP_COLL_MAX_CONCUR);
	struct cxip_domain *dom = ep_obj->domain;
	uint8_t *buf;
	int ret;

	if(!jstate) {
		TRACE_DEBUG("%s: Bad input params!\n", __func__);
		return FI_EINVAL;
	}
	/* alloc leaf rdma get dest buffers */
	mc_obj->leaf_rdma_get_data_p = calloc(1, len);
	buf = mc_obj->leaf_rdma_get_data_p;
	if(!buf) {
		TRACE_DEBUG("%s: Unable to calloc RDMA get base pointer\n", __func__);
		return FI_ENOMEM;
	}
	TRACE_DEBUG("%s: leaf rdma get dest buffers allocated\n", __func__);
	/* map the local buffer for the read */
	ret = cxip_map(dom, buf, len, CXI_MAP_WRITE, 0, &mc_obj->leaf_rdma_get_md);
	if (ret != FI_SUCCESS)
		TRACE_DEBUG("%s: cxip_map() failed! %d\n", __func__, ret);
	else
		TRACE_DEBUG("%s: cxip_map() success! %d\n", __func__, ret);

	return FI_SUCCESS;
}

int leaf_rdma_get_callback(struct cxip_req *req, const union c_event *evt);
static int _leaf_rdma_get(struct cxip_coll_reduction *reduction)
{
	int lock_held = 0, ret = FI_SUCCESS, red_id = 0, av_set_idx = 0, len = 0;
	int red_op __attribute__((unused));
	struct cxip_ep_obj *ep_obj;
	struct cxip_coll_mc *mc_obj;
	struct cxip_av_set *av_set_obj;
	struct cxip_addr dest_caddr;
	fi_addr_t dest_addr;
	struct c_full_dma_cmd dma_cmd = {};
	struct cxip_req *req;
	struct cxip_txc *txc;
	struct cxip_md *cxip_md;
	uint8_t *buf, lac = 0;
	uint32_t rdzv_idx = 0;
	union c_fab_addr dfa;
	uint8_t idx_ext = 0;
	uint64_t local_addr;
	uint64_t rem_offset;
	union cxip_match_bits mb = {};
	uint16_t vni = 0;
	struct red_pkt_64 *get_pkt_64_p;

	red_id = reduction->red_id;
	red_op = reduction->accum.red_op;
	ep_obj = reduction->mc_obj->ep_obj;
	mc_obj = reduction->mc_obj;
	ep_obj = mc_obj->ep_obj;
	av_set_obj = mc_obj->av_set_obj;
	txc = ep_obj->txc;
	get_pkt_64_p = (struct red_pkt_64 *)mc_obj->leaf_rdma_get_data_p;
	if(!get_pkt_64_p) {
		TRACE_DEBUG("%s: leaf rdma get base pntr is NULL\n", __func__);
		TRACE_DEBUG("%s: alloc is in leaf_rdma_get_setup\n", __func__);
		return FI_ENOMEM;
	}
	buf = (uint8_t *)&get_pkt_64_p[red_id].pkt;
	len = sizeof(struct red_pkt_64);
	/* check the lac sent from root before we do anything */
	if(!mc_obj->rdma_get_lac_va_rx) {
		TRACE_DEBUG("%s: Invalid RDMA_GET_LAC_VA_RX %016lx\n",
			__func__, mc_obj->rdma_get_lac_va_rx);
		return FI_EINVAL;
	}
	TRACE_DEBUG("%s: starting leaf rdma_get mc_addr %x red_id %d red_op %d\n",
		__func__, mc_obj->mcast_addr, red_id, red_op);
	/* rdma_get code needs to be envoked holding the endpoint lock */
	lock_held = ofi_genlock_held(&ep_obj->lock);
	TRACE_DEBUG("%s: ep lock held status %d\n", __func__, lock_held);
	if(!lock_held)
		ofi_genlock_lock(&ep_obj->lock);
	/* init the recv buffer */
	memset(buf,0,len);
	/* figure out the root network address */
	av_set_idx = reduction->mc_obj->hwroot_idx;
	dest_addr = av_set_obj->fi_addr_ary[av_set_idx];
	ret = cxip_av_lookup_addr(ep_obj->av, dest_addr, &dest_caddr);
	if (ret != FI_SUCCESS) {
		TRACE_DEBUG("%s: Failed to find root address\n", __func__);
		goto unlock;
	}
	TRACE_DEBUG("%s: dest_addr nic %d pid %d\n", __func__, dest_caddr.nic, dest_caddr.pid);
	/* get a tx evtq handle needed for the rdma read */
	req = cxip_evtq_req_alloc(&txc->tx_evtq, false, txc);
	if (!req) {
		TRACE_DEBUG("%s: Failed to get a tx eq handle \n", __func__);
		ret = FI_EBUSY;
		goto unlock;
	}
	/* set the callback for the get */
	req->cb = leaf_rdma_get_callback;
	req->rma.txc = txc;
	req->rma.reduction = reduction;
	req->type = CXIP_REQ_RMA;
	/* fill in the dma req */
	cxip_md = mc_obj->leaf_rdma_get_md;
	dma_cmd.command.cmd_type = C_CMD_TYPE_DMA;
	dma_cmd.command.opcode = C_CMD_GET;
	dma_cmd.event_send_disable = 1;
	TRACE_DEBUG("%s: RDMA_GET_LAC_VA_RX %016lx\n", __func__, mc_obj->rdma_get_lac_va_rx);
	/* lac is in upper 3 bits, this came in from the recv message path at
	 * cxip_zbcoll_recv_cb, it is the user_data variable passed in and copied to rdma_get_lac_va_rx
	 */
	lac = (uint8_t)((mc_obj->rdma_get_lac_va_rx & 0xE000000000000000) >> 61);
	/* build the dfa */
	rdzv_idx = txc->domain->iface->dev->info.rdzv_get_idx;
	mb.rdzv_lac = lac;
	mb.coll_get = 1;
	dma_cmd.match_bits = mb.raw;
	dma_cmd.user_ptr = (uint64_t)req;
	cxi_build_dfa(dest_caddr.nic, dest_caddr.pid,
		txc->pid_bits, rdzv_idx, &dfa, &idx_ext);
	dma_cmd.dfa = dfa;
	dma_cmd.index_ext = idx_ext;
	local_addr = CXI_VA_TO_IOVA(cxip_md->md,buf);
	/* VA of the root buffer, 56 bit remote offset */
	rem_offset = ((mc_obj->rdma_get_lac_va_rx & 0x00FFFFFFFFFFFFFF) +
		(red_id * sizeof(struct red_pkt_64)));
	dma_cmd.lac = lac;
	dma_cmd.local_addr = local_addr;
	dma_cmd.remote_offset = rem_offset;
	dma_cmd.request_len = len;
	if (ep_obj->av_auth_key)
		vni = dest_caddr.vni;
	else
		vni = ep_obj->auth_key.vni;
	TRACE_DEBUG("%s: vni %d\n", __func__, vni);
	dma_cmd.eq = cxip_evtq_eqn(&txc->tx_evtq);
	/* Enqueue the DMA get command */
	ret = cxip_txc_emit_dma(txc, vni,
			cxip_ofi_to_cxi_tc(cxip_env.rget_tc),
			CXI_TC_TYPE_DEFAULT, NULL, 0, &dma_cmd, 0);
	if (ret != FI_SUCCESS) {
		TRACE_DEBUG("%s: cxip_txc_emit_dma() failed! %d\n", __func__, ret);
		goto free_mem;
	} else
		TRACE_DEBUG("%s: cxip_txc_emit_dma() success! %d\n", __func__, ret);

	reduction->rdma_get_sent = true;
	TRACE_DEBUG("%s: rdma_get was posted for red_id %d red_op %d completed!\n",
		__func__, red_id, red_op);
	/* if we successfully launched a dma, the callback will free the request and the tx credits */
	goto unlock;

free_mem:
	cxip_unmap(mc_obj->leaf_rdma_get_md);
	cxip_evtq_req_free(req);
unlock:
	if(!lock_held)
		ofi_genlock_unlock(&ep_obj->lock);
	TRACE_DEBUG("%s: ret = %d\n", __func__, ret);
	return ret;
}
int leaf_rdma_get_callback(struct cxip_req *req, const union c_event *evt)
{
	int event_rc = cxi_event_rc(evt);
	struct cxip_txc *txc = NULL;
	struct red_pkt_64 *get_pkt_64_p = NULL;
	uint8_t red_id;
	struct red_pkt *root_pkt;
	struct cxip_coll_reduction *reduction;
	struct cxip_coll_mc *mc_obj;

	if(!req || !evt) {
		TRACE_DEBUG("%s: Bad input params!\n", __func__);
		return FI_EINVAL;
	}
	event_rc = cxi_event_rc(evt);
	switch(evt->hdr.event_type) {
	case C_EVENT_SEND:
		if (event_rc == C_RC_OK) {
			TRACE_DEBUG("%s: Good status after rdma get C_EVENT_SEND\n",
				__func__);
		} else
			TRACE_DEBUG("%s: Bad status after rdma get C_EVENT_SEND %d\n",
				__func__, event_rc);
		break;
	case C_EVENT_ACK:
		if (event_rc == C_RC_OK) {
			TRACE_DEBUG("%s: Good status after rdma get C_EVENT_ACK\n", __func__);
		} else
			TRACE_DEBUG("%s: Bad status after rdma get C_EVENT_ACK %d\n",
				__func__, event_rc);
		break;
	case C_EVENT_REPLY:
		if (event_rc == C_RC_OK) {
			TRACE_DEBUG("%s: Good stat after rget C_EVENT_REPLY - ts:%016lx\n",
				__func__, ofi_gettime_us());
			txc = req->rma.txc;
			reduction = req->rma.reduction;
			mc_obj = reduction->mc_obj;
			red_id = reduction->red_id;
			/* print the dest rd buffer */
			get_pkt_64_p = (struct red_pkt_64 *)mc_obj->leaf_rdma_get_data_p;
			root_pkt = (struct red_pkt *)&get_pkt_64_p[red_id].pkt;
			_dump_red_pkt(root_pkt, "leaf rdma get callback", "rdma");
			reduction->rdma_get_cb_rc = C_RC_OK;
			reduction->rdma_get_completed = true;
			TRACE_DEBUG("%s: rdma_get_completed for mc_addr %x red_id %d\n",
				__func__, mc_obj->mcast_addr, red_id);
		} else {
			txc = req->rma.txc;
			reduction = req->rma.reduction;
			red_id = reduction->red_id;
			reduction->rdma_get_cb_rc = event_rc;
			reduction->rdma_get_completed = true;
			TRACE_DEBUG("%s: Bad stat after rget red_id %d C_EVENT_REPLY %d\n",
				__func__, red_id,event_rc);
		}
		/* free the tx credits and the request */
		cxip_txc_otx_reqs_dec(txc);
		cxip_evtq_req_free(req);
		break;
	}
	return 0;
}
