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
#ifndef _FI_PROV_OPX_HFI1_PACKET_H_
#define _FI_PROV_OPX_HFI1_PACKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> /* only for fi_opx_addr_dump ... */
#include <sys/user.h>

#include "ofi_mem.h"
#include "rdma/fabric.h" /* only for 'fi_addr_t' ... which is a typedef to uint64_t */
#include "rdma/opx/fi_opx_addr.h"

#if defined(__riscv) && defined(__riscv_xlen) && (__riscv_xlen == 64)
#ifndef PAGE_SIZE
/* 4K pages default */
#define PAGE_SIZE 4096
#endif
#endif

#define FI_OPX_ADDR_SEP_RX_MAX (4)

/* OPX runtime default packet size. Runtime checked against driver MTU */
#ifndef OPX_HFI1_DEFAULT_PKT_SIZE
#ifndef OPX_JKR_SUPPORT
#define OPX_HFI1_DEFAULT_PKT_SIZE (8192)
#else
#define OPX_HFI1_DEFAULT_PKT_SIZE (10240)
#endif
#endif

/* OPX absolute max for storage allocation/structs. Driver MTU can vary. */
#define OPX_HFI1_MAX_PKT_SIZE (10240)

#define OPX_HFI1_N_PKT_SIZES 4 /* num of valid packet sizes */

static const uint16_t opx_valid_pkt_sizes[OPX_HFI1_N_PKT_SIZES] = {2048, 4096, 8192, 10240};

static_assert(((OPX_HFI1_DEFAULT_PKT_SIZE == 2048) || (OPX_HFI1_DEFAULT_PKT_SIZE == 4096) ||
	       (OPX_HFI1_DEFAULT_PKT_SIZE == 8192) || (OPX_HFI1_DEFAULT_PKT_SIZE == 10240)),
	      "OPX_HFI1_DEFAULT_PKT_SIZE must be one of 2048, 4096, 8192, or 10240");

#define OPX_HFI1_PKT_SIZE (fi_opx_global.pkt_size) /* Runtime checked against driver MTU. */

#define OPX_HFI1_TID_PAGESIZE (PAGE_SIZE) /* assume 4K, no hugepages*/

#define FI_OPX_HFI1_PACKET_IMM (16)

/* opcodes (0x00..0xBF) are reserved */
#define FI_OPX_HFI_BTH_OPCODE_INVALID  (0xC0)
#define FI_OPX_HFI_BTH_OPCODE_RZV_CTS  (0xC1)
#define FI_OPX_HFI_BTH_OPCODE_RZV_DATA (0xC2)
#define FI_OPX_HFI_BTH_OPCODE_RMA_RTS  (0xC3)
#define FI_OPX_HFI_BTH_OPCODE_ATOMIC   (0xC4)
#define FI_OPX_HFI_BTH_OPCODE_ACK      (0xC5)
#define FI_OPX_HFI_BTH_OPCODE_UD       (0xC6) /* unreliabile datagram */
/* opcodes (0xC7..0xEE) are unused */

#define FI_OPX_HFI_BTH_OPCODE_CQ_BIT  (0x01)
#define FI_OPX_HFI_BTH_OPCODE_TAG_BIT (0x02)
#define FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(opcode) \
	(opcode & ~(FI_OPX_HFI_BTH_OPCODE_CQ_BIT | FI_OPX_HFI_BTH_OPCODE_TAG_BIT))
#define FI_OPX_HFI_BTH_OPCODE_WITHOUT_CQ(opcode)   (opcode & ~(FI_OPX_HFI_BTH_OPCODE_CQ_BIT))
#define FI_OPX_HFI_BTH_OPCODE_GET_CQ_FLAG(opcode)  ((opcode & FI_OPX_HFI_BTH_OPCODE_CQ_BIT) ? FI_REMOTE_CQ_DATA : 0)
#define FI_OPX_HFI_BTH_OPCODE_GET_MSG_FLAG(opcode) ((opcode & FI_OPX_HFI_BTH_OPCODE_TAG_BIT) ? FI_TAGGED : FI_MSG)
#define FI_OPX_HFI_BTH_OPCODE_IS_TAGGED(opcode)	   ((opcode & FI_OPX_HFI_BTH_OPCODE_TAG_BIT) ? 1 : 0)

#define FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH  (0xEF)
#define FI_OPX_HFI_BTH_OPCODE_MSG_INJECT    (0xF0)
#define FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ (FI_OPX_HFI_BTH_OPCODE_MSG_INJECT | FI_OPX_HFI_BTH_OPCODE_CQ_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_INJECT    (FI_OPX_HFI_BTH_OPCODE_MSG_INJECT | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ \
	(FI_OPX_HFI_BTH_OPCODE_MSG_INJECT | FI_OPX_HFI_BTH_OPCODE_CQ_BIT | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_MSG_EAGER	   (0xF4)
#define FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ (FI_OPX_HFI_BTH_OPCODE_MSG_EAGER | FI_OPX_HFI_BTH_OPCODE_CQ_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_EAGER	   (FI_OPX_HFI_BTH_OPCODE_MSG_EAGER | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ \
	(FI_OPX_HFI_BTH_OPCODE_MSG_EAGER | FI_OPX_HFI_BTH_OPCODE_CQ_BIT | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST (0xF8)
#define FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ \
	(FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST | FI_OPX_HFI_BTH_OPCODE_CQ_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST \
	(FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ \
	(FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST | FI_OPX_HFI_BTH_OPCODE_CQ_BIT | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS    (0xFC)
#define FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ (FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS | FI_OPX_HFI_BTH_OPCODE_CQ_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS    (FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)
#define FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ \
	(FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS | FI_OPX_HFI_BTH_OPCODE_CQ_BIT | FI_OPX_HFI_BTH_OPCODE_TAG_BIT)

#define FI_OPX_HFI_DPUT_GET_OPCODE(_opcode) ((_opcode & 0x00F0) >> 4)

static const char *FI_OPX_HFI_BTH_LOW_OPCODE_STRINGS[] = {
	/* opcodes (0x00..0xBF) are reserved */
	"FI_OPX_HFI_BTH_OPCODE_INVALID             ", "FI_OPX_HFI_BTH_OPCODE_RZV_CTS             ",
	"FI_OPX_HFI_BTH_OPCODE_RZV_DATA            ", "FI_OPX_HFI_BTH_OPCODE_RMA_RTS             ",
	"FI_OPX_HFI_BTH_OPCODE_ATOMIC              ", "FI_OPX_HFI_BTH_OPCODE_ACK                 ",
	"FI_OPX_HFI_BTH_OPCODE_UD                  "};

static const char *FI_OPX_HFI_BTH_HIGH_OPCODE_STRINGS[] = {
	/* opcodes (0xC7..0xEE) are unused */
	"FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH          ", "FI_OPX_HFI_BTH_OPCODE_MSG_INJECT            ",
	"FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ         ", "FI_OPX_HFI_BTH_OPCODE_TAG_INJECT            ",
	"FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ         ", "FI_OPX_HFI_BTH_OPCODE_MSG_EAGER             ",
	"FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ          ", "FI_OPX_HFI_BTH_OPCODE_TAG_EAGER             ",
	"FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ          ", "FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST    ",
	"FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ ", "FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST    ",
	"FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ ", "FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS           ",
	"FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ        ", "FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS           ",
	"FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ        ", "INVALID BTH OPCODE                          "};

OPX_COMPILE_TIME_ASSERT(
	(FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH ==
	 (FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ - sizeof(FI_OPX_HFI_BTH_HIGH_OPCODE_STRINGS) / sizeof(char *) + 2)),
	"FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH must be first in the high opcode array, or dependent code conditionals need updated");
OPX_COMPILE_TIME_ASSERT(
	(FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ == 0xFF),
	"FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ must be last in the high opcode array, or dependent code conditionals need updated");

OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_INJECT ^ FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ) == FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_INJECT and FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_EAGER ^ FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ) == FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_EAGER and FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST ^ FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ) ==
	 FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST and FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS ^ FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ) == FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS and FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_TAG_INJECT ^ FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ) == FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_TAG_INJECT and FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_TAG_EAGER ^ FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ) == FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_TAG_EAGER and FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST ^ FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ) ==
	 FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST and FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS ^ FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ) == FI_OPX_HFI_BTH_OPCODE_CQ_BIT),
	"FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS and FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_CQ_BIT");

OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_INJECT ^ FI_OPX_HFI_BTH_OPCODE_TAG_INJECT) == FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_INJECT and FI_OPX_HFI_BTH_OPCODE_TAG_INJECT must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ ^ FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ) == FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ and FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_EAGER ^ FI_OPX_HFI_BTH_OPCODE_TAG_EAGER) == FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_EAGER and FI_OPX_HFI_BTH_OPCODE_TAG_EAGER must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ ^ FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ) == FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ and FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST ^ FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST) ==
	 FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST and FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ ^ FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ) ==
	 FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ and FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS ^ FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS) == FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS and FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");
OPX_COMPILE_TIME_ASSERT(
	((FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ ^ FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ) ==
	 FI_OPX_HFI_BTH_OPCODE_TAG_BIT),
	"FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ and FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ must only differ by FI_OPX_HFI_BTH_OPCODE_TAG_BIT");

static inline const char *opx_hfi1_bth_opcode_to_string(uint16_t opcode)
{
	/* uint8_t to uint16_t to avoid limited data type warnings */
	if ((opcode >= (uint16_t) FI_OPX_HFI_BTH_OPCODE_INVALID) && (opcode <= (uint16_t) FI_OPX_HFI_BTH_OPCODE_UD)) {
		return FI_OPX_HFI_BTH_LOW_OPCODE_STRINGS[opcode - FI_OPX_HFI_BTH_OPCODE_INVALID];
	} else if ((opcode >= (uint16_t) FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH) &&
		   (opcode <= (uint16_t) FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ)) {
		return FI_OPX_HFI_BTH_HIGH_OPCODE_STRINGS[opcode - FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH];
	}
	return FI_OPX_HFI_BTH_HIGH_OPCODE_STRINGS[sizeof(FI_OPX_HFI_BTH_HIGH_OPCODE_STRINGS) / sizeof(char *) -
						  1]; /* INVALID */
}

#define FI_OPX_HFI1_PACKET_ORIGIN_RX(packet_hdr)                                          \
	(((packet_hdr)->bth.opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA) ?                   \
		 ((packet_hdr)->dput.target.opcode_origin_rx & OPX_BTH_SUBCTXT_RX_MASK) : \
		 (packet_hdr)->reliability.origin_rx)

#define FI_OPX_HFI1_PACKET_PSN(packet_hdr)                                                                          \
	(((packet_hdr)->bth.opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA) ? ntohl((packet_hdr)->bth.psn) & 0x00FFFFFF : \
									(packet_hdr)->reliability.psn)

#define FI_OPX_HFI_UD_OPCODE_MASK (0xf0)

#define FI_OPX_HFI_UD_OPCODE_FIRST_INVALID	     (0x00)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING	     (0x10)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK	     (0x20)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK	     (0x30)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH     (0x40)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK (0x50)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP	     (0x60)
/* Add new UD Opcodes here, and increment LAST_INVALID accordingly */
#define FI_OPX_HFI_UD_OPCODE_LAST_INVALID (0x70)

#define FI_OPX_HFI_UD_OPCODE_FIRST_INVALID_INDEX	   (FI_OPX_HFI_UD_OPCODE_FIRST_INVALID >> 4)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING_INDEX	   (FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING >> 4)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK_INDEX	   (FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK >> 4)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK_INDEX	   (FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK >> 4)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_INDEX	   (FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH >> 4)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK_INDEX (FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK >> 4)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP_INDEX	   (FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP >> 4)

static const char *FI_OPX_HFI_UD_OPCODE_STRINGS[] = {
	[FI_OPX_HFI_UD_OPCODE_FIRST_INVALID_INDEX]	     = "INVALID UD OPCODE",
	[FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING_INDEX]	     = "FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING",
	[FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK_INDEX]	     = "FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK",
	[FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK_INDEX]	     = "FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK",
	[FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_INDEX]     = "FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH",
	[FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK_INDEX] = "FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK",
	[FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP_INDEX]	     = "FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP"};

static inline const char *opx_hfi1_ud_opcode_to_string(uint8_t opcode)
{
	const uint8_t ud_opcode = FI_OPX_HFI_UD_OPCODE_MASK & opcode;

	if (ud_opcode > FI_OPX_HFI_UD_OPCODE_FIRST_INVALID && ud_opcode < FI_OPX_HFI_UD_OPCODE_LAST_INVALID) {
		return FI_OPX_HFI_UD_OPCODE_STRINGS[ud_opcode >> 4];
	}
	return FI_OPX_HFI_UD_OPCODE_STRINGS[FI_OPX_HFI_UD_OPCODE_FIRST_INVALID];
}

#define FI_OPX_HFI_DPUT_OPCODE_RZV		    (0x00)
#define FI_OPX_HFI_DPUT_OPCODE_PUT		    (0x01)
#define FI_OPX_HFI_DPUT_OPCODE_GET		    (0x02)
#define FI_OPX_HFI_DPUT_OPCODE_FENCE		    (0x03)
#define FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH	    (0x04)
#define FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH (0x05)
#define FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG	    (0x06)
#define FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC	    (0x07)
#define FI_OPX_HFI_DPUT_OPCODE_RZV_TID		    (0x08)
#define FI_OPX_HFI_DPUT_OPCODE_PUT_CQ		    (0x09)
#define FI_OPX_HFI_DPUT_OPCODE_IPC		    (0x0A)
/* Add new DPUT Opcodes here, and increment LAST_INVALID accordingly */
#define FI_OPX_HFI_DPUT_OPCODE_LAST_INVALID (0x0B)

#define FI_OPX_HFI_DPUT_GET_OPCODE(_opcode) ((_opcode & 0x00F0) >> 4)

static const char *FI_OPX_HFI_DPUT_OPCODE_STRINGS[] = {
	[FI_OPX_HFI_DPUT_OPCODE_RZV]		      = "FI_OPX_HFI_DPUT_OPCODE_RZV",
	[FI_OPX_HFI_DPUT_OPCODE_PUT]		      = "FI_OPX_HFI_DPUT_OPCODE_PUT",
	[FI_OPX_HFI_DPUT_OPCODE_GET]		      = "FI_OPX_HFI_DPUT_OPCODE_GET",
	[FI_OPX_HFI_DPUT_OPCODE_FENCE]		      = "FI_OPX_HFI_DPUT_OPCODE_FENCE",
	[FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH]	      = "FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH",
	[FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH] = "FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH",
	[FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG]	      = "FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG",
	[FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC]	      = "FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC",
	[FI_OPX_HFI_DPUT_OPCODE_RZV_TID]	      = "FI_OPX_HFI_DPUT_OPCODE_RZV_TID",
	[FI_OPX_HFI_DPUT_OPCODE_PUT_CQ]		      = "FI_OPX_HFI_DPUT_OPCODE_PUT_CQ",
	[FI_OPX_HFI_DPUT_OPCODE_IPC]		      = "FI_OPX_HFI_DPUT_OPCODE_IPC",
	[FI_OPX_HFI_DPUT_OPCODE_LAST_INVALID]	      = "INVALID DPUT OPCODE"};

static inline const char *opx_hfi1_dput_opcode_to_string(uint8_t opcode)
{
	if (opcode < FI_OPX_HFI_DPUT_OPCODE_LAST_INVALID) {
		return FI_OPX_HFI_DPUT_OPCODE_STRINGS[opcode];
	}
	return FI_OPX_HFI_DPUT_OPCODE_STRINGS[FI_OPX_HFI_DPUT_OPCODE_LAST_INVALID];
}

/* KDETH header consts */

#define FI_OPX_HFI1_KDETH_VERSION	   (0x1)
#define FI_OPX_HFI1_KDETH_VERSION_SHIFT	   (30) /* a.k.a. "HFI_KHDR_KVER_SHIFT" */
#define FI_OPX_HFI1_KDETH_VERSION_OFF_MASK ((FI_OPX_HFI1_KDETH_VERSION << FI_OPX_HFI1_KDETH_VERSION_SHIFT) - 1)

#define FI_OPX_HFI1_KDETH_TIDCTRL	(0x3)
#define FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT (26)
#define FI_OPX_HFI1_KDETH_TIDCTL_MASK	(FI_OPX_HFI1_KDETH_TIDCTRL << FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT)

#define FI_OPX_HFI1_KDETH_TID	    (0x3ff)
#define FI_OPX_HFI1_KDETH_TID_SHIFT (16)
#define FI_OPX_HFI1_KDETH_TID_MASK  (FI_OPX_HFI1_KDETH_TID << FI_OPX_HFI1_KDETH_TID_SHIFT)

#define HFI_KHDR_OFFSET_MASK   0x7fff
#define HFI_KHDR_OM_SHIFT      15
#define HFI_KHDR_TID_SHIFT     16
#define HFI_KHDR_TID_MASK      0x3ff
#define HFI_KHDR_TIDCTRL_SHIFT 26
#define HFI_KHDR_TIDCTRL_MASK  0x3
#define HFI_KHDR_INTR_SHIFT    28
#define HFI_KHDR_SH_SHIFT      29
#define HFI_KHDR_KVER_SHIFT    30
#define HFI_KHDR_KVER_MASK     0x3

/* "Legacy" header with 9DWs of KDETH */
struct fi_opx_hfi1_stl_packet_hdr_9B {
	/* == quadword 0 == */
	union {
		uint64_t qw[1];
		uint32_t dw[2];
		uint16_t w[4];
		struct {
			uint16_t flags;	 /* lrh.w[0] - big-endian! */
			uint16_t dlid;	 /* lrh.w[1] - big-endian! */
			uint16_t pktlen; /* lrh.w[2] - big-endian! */
			uint16_t slid;	 /* lrh.w[3] - big-endian! */
		} __attribute__((__packed__));
	} lrh_9B;

	/* == quadword 1 == */
	union {
		uint32_t dw[3];
		uint16_t w[6];
		uint8_t	 hw[12];
		struct {
			uint8_t	 opcode;  /* bth.hw[0] */
			uint8_t	 bth_1;	  /* bth.hw[1] */
			uint16_t pkey;	  /* bth.w[1]  - big-endian! */
			uint8_t	 ecn;	  /* bth.hw[4] (FECN, BECN, (CSPEC and RC2 for JKR) and reserved) */
			uint8_t	 qp;	  /* bth.hw[5] */
			uint8_t	 subctxt; /* bth.hw[6] -----> subctxt: higher 3 bits, inject::message_length,
					     send::xfer_bytes_tail */
			uint8_t rx;	  /* bth.hw[7] */

			/* == quadword 2 == */
			uint32_t psn; /* bth.dw[2] ..... the 'psn' field is unused for 'eager' packets ----->
					 reliability::psn, etc */
		} __attribute__((__packed__));
	} bth;

	union {
		uint32_t dw[3];
		uint16_t w[6];
		uint8_t	 hw[12];
		struct {
			uint32_t
				offset_ver_tid; /* kdeth.dw[0]  .... the 'offset' field is unused for 'eager' packets */

			/* == quadword 3 == */
			uint16_t jkey;	 /* kdeth.w[2]  */
			uint16_t hcrc;	 /* kdeth.w[3]  */
			uint32_t unused; /* kdeth.dw[2] -----> immediate data (32b) */
		} __attribute__((__packed__));
	} kdeth;

	/* == quadword 4,5,6 == */
	uint64_t kdeth_sw[3];

} __attribute__((__packed__));

/* "Legacy" header with 9DWs of KDETH */
struct fi_opx_hfi1_stl_packet_hdr_16B {
	/* == quadword 0,1 == */
	union {
		struct {
			__le64 qw0;
			__le64 qw1;
		};
		__le64 qw[2];
		__le32 dw[4];
		__le16 w[8];
		struct {		  /* 16B header */
			__le32 slid : 20; /* dw[0] qw[0]*/
			/* This is the packet length and is in units of flits (QWs) for 8B, 10B and 16B
			   formats, but in units of DWs for 9B formats.*/
			__le32 pktlen : 11;
			__le32 b : 1;

			__le32 dlid : 20; /* dw[1] */
			__le32 sc : 5;
			__le32 rc : 3;
			__le32 f : 1;
			__le32 l2 : 2;
			__le32 lt : 1;

			__le32 l4 : 8; /* dw[2] qw[1] */
			__le32 slid20 : 4;
			__le32 dlid20 : 4;
			__le32 pkey : 16;

			__le32 entropy : 16; /* dw[3] */
			__le32 age : 3;
			__le32 cspec : 5;
			__le32 r : 8;
		};
	} lrh_16B;

	/* == quadword 2 == */
	union {
		uint32_t dw[3];
		uint16_t w[6];
		uint8_t	 hw[12];
		struct {
			uint8_t	 opcode;  /* bth.hw[0] */
			uint8_t	 bth_1;	  /* bth.hw[1] */
			uint16_t pkey;	  /* bth.w[1]  - big-endian! */
			uint8_t	 ecn;	  /* bth.hw[4] (FECN, BECN, (CSPEC and RC2 for JKR) and reserved) */
			uint8_t	 qp;	  /* bth.hw[5] */
			uint8_t	 subctxt; /* bth.hw[6] -----> subctxt: higher 3 bits, inject::message_length,
					     send::xfer_bytes_tail */
			uint8_t rx;	  /* bth.hw[7] */

			/* == quadword 3 == */
			uint32_t psn; /* bth.dw[2] ..... the 'psn' field is unused for 'eager' packets ----->
					 reliability::psn, etc */
		} __attribute__((__packed__));
	} bth;

	union {
		uint32_t dw[3];
		uint16_t w[6];
		uint8_t	 hw[12];
		struct {
			uint32_t
				offset_ver_tid; /* kdeth.dw[0]  .... the 'offset' field is unused for 'eager' packets */

			/* == quadword 4 == */
			uint16_t jkey;	 /* kdeth.w[2]  */
			uint16_t hcrc;	 /* kdeth.w[3]  */
			uint32_t unused; /* kdeth.dw[2] -----> immediate data (32b) */
		} __attribute__((__packed__));
	} kdeth;

	/* == quadword 5,6,7 == */
	uint64_t kdeth_sw[3];

} __attribute__((__packed__));

/*
 * Define fields in the KDETH header so we can update the header
 * template.
 */
#define KDETH_OFFSET_SHIFT     0
#define KDETH_OFFSET_MASK      0x7fff
#define KDETH_OM_SHIFT	       15
#define KDETH_OM_MASK	       0x1
#define KDETH_TID_SHIFT	       16
#define KDETH_TID_MASK	       0x3ff
#define KDETH_TIDCTRL_SHIFT    26
#define KDETH_TIDCTRL_MASK     0x3
#define KDETH_INTR_SHIFT       28
#define KDETH_INTR_MASK	       0x1
#define KDETH_SH_SHIFT	       29
#define KDETH_SH_MASK	       0x1
#define KDETH_KVER_SHIFT       30
#define KDETH_KVER_MASK	       0x3
#define KDETH_JKEY_SHIFT       0x0
#define KDETH_JKEY_MASK	       0xff
#define KDETH_HCRC_UPPER_SHIFT 16
#define KDETH_HCRC_UPPER_MASK  0xff
#define KDETH_HCRC_LOWER_SHIFT 24
#define KDETH_HCRC_LOWER_MASK  0xff

#include "opa_byteorder.h"

#define KDETH_GET(val, field) (((__le32_to_cpu((val))) >> KDETH_##field##_SHIFT) & KDETH_##field##_MASK)
#define KDETH_SET(dw, field, val)                                                   \
	do {                                                                        \
		u32 dwval = __le32_to_cpu(dw);                                      \
		dwval &= ~(KDETH_##field##_MASK << KDETH_##field##_SHIFT);          \
		dwval |= (((val) & KDETH_##field##_MASK) << KDETH_##field##_SHIFT); \
		dw = __cpu_to_le32(dwval);                                          \
	} while (0)

#define KDETH_RESET(dw, field, val)        \
	({                                 \
		dw = 0;                    \
		KDETH_SET(dw, field, val); \
	})

/* KDETH OM multipliers and switch over point */
#define KDETH_OM_SMALL	     4
#define KDETH_OM_SMALL_SHIFT 2
#define KDETH_OM_LARGE	     64
#define KDETH_OM_LARGE_SHIFT 6
#define KDETH_OM_MAX_SIZE    (1 << ((KDETH_OM_LARGE / KDETH_OM_SMALL) + 1))

#define FI_OPX_EXP_TID_TIDLEN_MASK     0x7FFULL
#define FI_OPX_EXP_TID_TIDLEN_SHIFT    0
#define FI_OPX_EXP_TID_TIDCTRL_MASK    0x3ULL
#define FI_OPX_EXP_TID_TIDCTRL_SHIFT   20
#define FI_OPX_EXP_TID_TIDIDX_MASK     0x3FFULL
#define FI_OPX_EXP_TID_TIDIDX_SHIFT    22
#define FI_OPX_EXP_TID_GET(tid, field) (((tid) >> FI_OPX_EXP_TID_TID##field##_SHIFT) & FI_OPX_EXP_TID_TID##field##_MASK)
#define FI_OPX_EXP_TID_SET(field, value) \
	(((value) & FI_OPX_EXP_TID_TID##field##_MASK) << FI_OPX_EXP_TID_TID##field##_SHIFT)
#define FI_OPX_EXP_TID_CLEAR(tid, field) \
	({ (tid) &= ~(FI_OPX_EXP_TID_TID##field##_MASK << FI_OPX_EXP_TID_TID##field##_SHIFT); })
#define FI_OPX_EXP_TID_RESET(tid, field, value)              \
	do {                                                 \
		FI_OPX_EXP_TID_CLEAR(tid, field);            \
		(tid) |= FI_OPX_EXP_TID_SET(field, (value)); \
	} while (0)

#define FI_OPX_PKT_RZV_FLAGS_SHIFT	    (16)
#define FI_OPX_PKT_RZV_FLAGS_NONCONTIG	    (1ul)
#define FI_OPX_PKT_RZV_FLAGS_NONCONTIG_MASK (FI_OPX_PKT_RZV_FLAGS_NONCONTIG << FI_OPX_PKT_RZV_FLAGS_SHIFT)
#define FI_OPX_PKT_RZV_FLAGS_IPC	    (2ul)
#define FI_OPX_PKT_RZV_FLAGS_IPC_MASK	    (FI_OPX_PKT_RZV_FLAGS_IPC << FI_OPX_PKT_RZV_FLAGS_SHIFT)
#if 0
#ifndef NDEBUG
static inline
void fi_opx_hfi1_dump_stl_packet_hdr (struct fi_opx_hfi1_stl_packet_hdr_9B * hdr,
		const char * fn, const unsigned ln) {

#if __GNUC__ > 9
#pragma GCC diagnostic ignored "=Waddress-of-packed-member"
#endif
	const uint64_t * const qw = (uint64_t *)hdr;
#if __GNUC__ > 9
#pragma GCC diagnostic pop
#endif

	fprintf(stderr, "%s():%u ==== dump stl packet header @ %p [%016lx %016lx %016lx %016lx]\n", fn, ln, hdr, qw[0], qw[1], qw[2], qw[3]);
	fprintf(stderr, "%s():%u .lrh.flags .............     0x%04hx\n", fn, ln, hdr->lrh_9B.flags);
	fprintf(stderr, "%s():%u .lrh.dlid ..............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh_9B.dlid, hdr->lrh_9B.dlid, ntohs(hdr->lrh.dlid));
	fprintf(stderr, "%s():%u .lrh.pktlen ............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh_9B.pktlen, hdr->lrh_9B.pktlen, ntohs(hdr->lrh.pktlen));
	fprintf(stderr, "%s():%u .lrh.slid ..............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh_9B.slid, hdr->lrh_9B.slid, ntohs(hdr->lrh.slid));
	fprintf(stderr, "%s():%u\n", fn, ln);
	fprintf(stderr, "%s():%u .bth.opcode ............       0x%02x \n", fn, ln, hdr->bth.opcode);
	fprintf(stderr, "%s():%u .bth.bth_1 .............       0x%02x \n", fn, ln, hdr->bth.bth_1);
	fprintf(stderr, "%s():%u .bth.pkey ..............     0x%04hx \n", fn, ln, hdr->bth.pkey);
	fprintf(stderr, "%s():%u .bth.ecn ...............       0x%02x \n", fn, ln, hdr->bth.ecn);
	fprintf(stderr, "%s():%u .bth.qp ................       0x%02x \n", fn, ln, hdr->bth.qp);
	fprintf(stderr, "%s():%u .bth.unused ............       0x%02x \n", fn, ln, hdr->bth.unused);
	fprintf(stderr, "%s():%u .bth.subctxt_rx ........       0x%02x \n", fn, ln, hdr->bth.subctxt_rx);
	fprintf(stderr, "%s():%u\n", fn, ln);
	fprintf(stderr, "%s():%u .bth.psn ............... 0x%08x \n", fn, ln, hdr->bth.psn);
	fprintf(stderr, "%s():%u .kdeth.offset_ver_tid .. 0x%08x\n", fn, ln, hdr->kdeth.offset_ver_tid);
	fprintf(stderr, "%s():%u\n", fn, ln);
	fprintf(stderr, "%s():%u .kdeth.jkey ............     0x%04hx\n", fn, ln, hdr->kdeth.jkey);
	fprintf(stderr, "%s():%u .kdeth.hcrc ............     0x%04hx\n", fn, ln, hdr->kdeth.hcrc);
	fprintf(stderr, "%s():%u .kdeth.unused .......... 0x%08x\n", fn, ln, hdr->kdeth.unused);

	return;
}
#endif
#endif

/*
	  HEADER  UNION             RX POLL
	=====================    =============
	 9B         16B           9B      16B
	=========  ==========    =====   =====
QW[0]   (pad)      LRH               <--    | <-- RX header union pointer
QW[1]   LRH        LRH              |       |
QW[2]   BTH        BTH              |       | <- RX common OPX headers
QW[3]   BTH/KDETH  BTH/KDETH        |       |
QW[4]   KDETH      KDETH            |       |
QW[5]   USER/SW    USER/SW          |       |
QW[6]   USER/SW    USER/SW          |       |
QW[7]   USER/SW    USER/SW          |       |
		RHF        RHF

  (*) HDRQ entries are 128 bytes (16 quadwords) and include HEADER + RHF

  In RX POLL, pull SLID, DLID and PKTLEN out of 9B/16B LRH.
  All other RX stack can use the common OPX headers to access OPX fields.
*/

/**
 * \brief Converged HFI1 packet header for 9B & 16B (JKR)
 *
 * The HFI1 packet header is consumed in many places and sometimes overloaded
 * for cache and memory allocation reasons.
 */

union opx_hfi1_packet_hdr {
	/* STL UNION */
	union opx_hfi1_stl_packet_hdr {
		struct {
			uint64_t			     qw0;
			struct fi_opx_hfi1_stl_packet_hdr_9B hdr_9B;	      /* 9B legacy w/ 9 DW KDETH */
			uint64_t			     qwn[7];	      /* 9B(+) QW's */
		} __attribute__((__packed__)) fi_opx_hfi1_stl_packet_hdr_9BP; /* 9B(+14 DWs of KDETH ) */
		struct {
			struct fi_opx_hfi1_stl_packet_hdr_16B hdr_16B;	       /* 16B legacy w/ 9 DW KDETH */
			uint64_t			      qwn[7];	       /* 16B(+) QW's */
		} __attribute__((__packed__)) fi_opx_hfi1_stl_packet_hdr_16BP; /* 16B(+14 DWs of KDETH  */
	} __attribute__((__packed__)) stl;				       /* for alignment/sizes*/

	/* QUADWORD UNION */
	struct {
		uint64_t unused_pad_9B;
		uint64_t qw_9B[7];  /* 9B QW's */
		uint64_t qw_9BP[7]; /* 9B(+) QW's */
	};
	uint64_t qw_16B[15]; /* 16B QW's */

	/* Standard (new) Headers - LRH, BTH, KDETH, SW defined (KDETH)
	   15 quadwords  */
	struct {
		/* LRH union for (padded) 9B and 16B LRH */
		union {
			struct {
				uint64_t unused_pad_qw0;
				union {
					uint64_t qw[1]; /* 9B LRH is 1 quadword */

					struct { /* 9B LRH  */
						uint16_t flags;
						uint16_t dlid;
						uint16_t pktlen;
						uint16_t slid;
					} __attribute__((__packed__));
				} lrh_9B;
			};
			union {
				__le64 qw[2]; /* 16B is 2 quadwords */

				struct { /* 16B LRH  */
					__le32 slid : 20;
					/* This is the packet length and is in units of flits (QWs) for 8B, 10B and 16B
					   formats, but in units of DWs for 9B formats.*/
					__le32 pktlen : 11;
					__le32 b : 1;

					__le32 dlid : 20;
					__le32 sc : 5;
					__le32 rc : 3;
					__le32 f : 1;
					__le32 l2 : 2;
					__le32 lt : 1;

					__le32 l4 : 8;
					__le32 slid20 : 4;
					__le32 dlid20 : 4;
					__le32 pkey : 16;

					__le32 entropy : 16;
					__le32 age : 3;
					__le32 cspec : 5;
					__le32 r : 8;
				};
			} lrh_16B;
		};

		/* QW[2-3] BTH 1 1/2 quadwords, 3 dwords */
		struct {
			uint8_t	 opcode;
			uint8_t	 bth_1;
			uint16_t pkey;
			uint8_t	 ecn; /* (FECN, BECN, (CSPEC and RC2 for JKR) and reserved) */
			uint8_t	 qp;
			uint16_t subctxt_rx;

			/* QW[3] starts */
			uint32_t psn;
		} __attribute__((__packed__)) bth;

		/* QW[3-4] KDETH 1 1/2 quadwords, 3 dwords */
		struct {
			uint32_t offset_ver_tid;

			/* QW[4] starts */
			uint16_t jkey;
			uint16_t hcrc;
			uint32_t unused;
		} __attribute__((__packed__)) kdeth;

		/*  QW[5-7]   9B    SW defined */
		/*  QW[8-14]  9B(+) SW defined */
		/*  QW[5-14] 16B    SW defined */
		uint64_t sw_defined[10];
	} __attribute__((__packed__));

	/* OPX headers
	 *
	 * overlay/redefine some standard header fields
	 * and the SW defined header */

	/*    OPX RELIABILITY HEADER               */
	struct {
		uint64_t reserved[3]; /* QW[0-2] */

		/* QW[3] BTH/KDETH (psn,offset_ver_tid)*/
		uint32_t psn : 24;
		uint32_t psn_reserved : 8;
		uint16_t origin_rx;
		uint16_t reserved_1;

		uint64_t reserved_n[10]; /* QW[4-14] KDETH/SW */

	} __attribute__((__packed__)) reliability;

	/*    OPX MATCH HEADER               */
	struct {
		uint64_t reserved[3]; /* QW[0-2] */

		/* QW[3] BTH/KDETH (psn) */
		uint32_t reserved_0;
		uint16_t origin_rx;
		uint16_t reserved_1;

		/* QW[4] KDETH (unused) */
		uint32_t reserved_2;
		uint32_t ofi_data; /* used for FI_RX_CQ_DATA */

		uint64_t reserved_3[2]; /* QW[5-6] SW */

		uint64_t ofi_tag;	/* QW[7] SW last 9B quadword */
		uint64_t reserved_n[7]; /* QW[8-14] SW */

	} __attribute__((__packed__)) match;

	/*    OPX INJECT HEADER               */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint16_t reserved_1[3];
		uint8_t subctxt_message_length; /* lower 3 bits is subctxt. higher 5 bit is inject message (max value 16
						   bytes) */
		uint8_t reserved_2;

		/* QW[3-4] BTH/KDETH*/
		uint64_t reserved_3[2];

		/* QW[5-6] SW */
		union {
			uint8_t	 app_data_u8[16];
			uint16_t app_data_u16[8];
			uint32_t app_data_u32[4];
			uint64_t app_data_u64[2];
		};

		uint64_t reserved_n[8]; /* QW[7-14] SW */

	} __attribute__((__packed__)) inject;

	/*    OPX SEND HEADER               */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint16_t reserved_1[3];
		uint8_t	 subctxt_xfer_bytes_tail; /* lower 3 bits is subctxt. higher 4 bits is xfer_bytes_tail */
		uint8_t	 reserved_2;

		/* QW[3-4] BTH/KDETH*/
		uint64_t reserved_3[2];

		/* QW[5] SW */
		uint16_t unused[3];
		uint16_t payload_qws_total; /* TODO - use stl.lrh.pktlen instead (num dws); only need 11 bits; maximum
					       number of payload qw is 10240 / 8 = 1280 */

		/* QW[6] SW */
		uint64_t xfer_tail;

		uint64_t reserved_n[8]; /* QW[7-14] SW */

	} __attribute__((__packed__)) send;

	/*    OPX MP EAGER 1ST HEADER            */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint16_t reserved_1[3];
		uint8_t	 subctxt_xfer_bytes_tail; /* lower 3 bits is subctxt. higher 4 bits is xfer_bytes_tail */
		uint8_t	 reserved_2;

		/* QW[3] BTH/KDETH (offset_ver_tid) */
		uint64_t reserved_3;

		/* QW[4] KDETH */
		uint64_t reserved_4;

		/* QW[5-6] SW */
		uint16_t payload_bytes_total;
		uint16_t unused[3];

		uint64_t reserved_n[8]; /* QW[7-14] SW */

	} __attribute__((__packed__)) mp_eager_first;

	/*    OPX MP EAGER NTH HEADER               */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint16_t reserved_1[3];
		uint8_t	 subctxt_xfer_bytes_tail; /* lower 3 bits is subctxt. higher 5 bits is xfer_bytes_tail */
		uint8_t	 reserved_2;

		/* QW[3-4] BTH/KDETH */
		uint64_t reserved_3[2];

		/* QW[5] SW */
		uint64_t xfer_tail;

		/* QW[6] SW */
		uint64_t reserved_4;

		/* QW[7] SW last 9B quadword */
		uint32_t payload_offset;
		uint32_t mp_egr_uid;

		uint64_t reserved_n[7]; /* QW[8-14] SW */

	} __attribute__((__packed__)) mp_eager_nth;

	/*    OPX RENDEZVOUS HEADER                   */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint64_t reserved_1;

		/* QW[3-4] BTH/KDETH */
		uint64_t reserved_2[2];

		/* QW[5] SW */
		uint16_t origin_rx;
		uint8_t	 flags;
		uint8_t	 unused_1[3];
		uint16_t niov; /* number of non-contiguous buffers */

		/* QW[6] SW */
		uint64_t message_length; /* total length in bytes of all non-contiguous buffers and immediate data */

		uint64_t reserved_n[8]; /* QW[7-14] SW */

	} __attribute__((__packed__)) rendezvous;

	/*    OPX CTS HEADER                   */
	struct {
		uint64_t reserved[5]; /* QW[0-4] */

		/* QW[5-7] SW */
		union {
			uint8_t opcode;
			struct {
				/* QW[5] SW */
				uint8_t	 opcode;
				uint8_t	 unused0;
				uint16_t unused1;
				uint16_t ntidpairs; /* number of tidpairs described in the packet payload */
				uint16_t niov; /* number of non-contiguous buffers described in the packet payload */

				/* QW[6-7] SW */
				uintptr_t origin_byte_counter_vaddr;
				uintptr_t target_context_vaddr;
			} vaddr;
			struct {
				/* QW[5] SW */
				uint8_t	 opcode;
				uint8_t	 unused0;
				uint16_t unused1;
				uint8_t	 dt;
				uint8_t	 op;
				uint16_t niov; /* number of non-contiguous buffers described in the packet payload */

				/* QW[6-7] SW */
				uintptr_t rma_request_vaddr;
				uint64_t  key;
			} mr;
			struct {
				/* QW[5] SW */
				uint8_t	 opcode;
				uint8_t	 unused0;
				uint16_t unused1;
				uint8_t	 unused2;
				uint8_t	 unused3;
				uint16_t unused4; /* number of non-contiguous buffers described in the packet payload */

				/* QW[6-7] SW */
				uintptr_t completion_counter;
				uint64_t  bytes_to_fence;
			} fence;
			struct {
				/* QW[5] SW */
				uint8_t	 opcode;
				uint8_t	 unused0;
				uint16_t unused1;
				uint8_t	 dt;
				uint8_t	 op;
				uint16_t niov; /* number of non-contiguous buffers described in the packet payload */

				/* QW[6-7] SW */
				uintptr_t origin_rma_request_vaddr;
				uint64_t  rma_request_vaddr;
			} rma;
		} target;

		uint64_t reserved_n[7]; /* QW[8-14] SW */

	} __attribute__((__packed__)) cts;

	/*    OPX RMA RTS HEADER                   */
	struct {
		uint64_t reserved[5]; /* QW[0-4] */

		/* QW[5] SW */
		uint8_t	 opcode;
		uint8_t	 unused0[3];
		uint8_t	 dt;
		uint8_t	 op;
		uint16_t niov; /* number of non-contiguous buffers described in the packet payload */

		/* QW[6-7] SW */
		uint64_t  key;
		uintptr_t rma_request_vaddr;

		uint64_t reserved_n[7]; /* QW[8-14] SW */

	} __attribute__((__packed__)) rma_rts;

	/*    OPX DPUT HEADER                   */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint64_t reserved_1;

		/* QW[3] BTH/KDETH */
		uint64_t reserved_3;

		/* QW[4] KDETH/SW */
		uint32_t reserved_4;
		uint8_t	 unused1[4];

		/* QW[5,6,7] KDETH/SW */
		union {
			/*  Common fields   */
			struct {
				/* QW[5] KDETH/SW */
				uint16_t opcode_origin_rx;
				uint8_t	 dt;
				uint8_t	 op;
				uint16_t last_bytes;
				uint16_t bytes;

				/* QW[6,7] SW */
				uint64_t reserved[2]; /* op-specific */
			};

			struct {
				/* QW[5] SW */
				uint64_t reserved; /* Common fields */

				/* QW[6] SW */
				uintptr_t rma_request_vaddr;
				/* QW[7] SW */
				uintptr_t rbuf;
			} get;

			struct {
				/* QW[5] SW */
				uint64_t reserved; /* Common fields */

				/* QW[6] SW */
				uintptr_t completion_vaddr; /* struct fi_opx_rzv_completion * */
				/* QW[7] SW */
				uintptr_t rbuf;
			} rzv;

			struct {
				/* QW[5] SW */
				uint64_t reserved; /* Common fields */

				/* QW[6,7] SW */
				uintptr_t key;
				uintptr_t offset;
			} mr;

			struct {
				/* QW[5] SW */
				uint64_t reserved; /* Common fields */

				/* QW[6,7] SW */
				uintptr_t completion_counter;
				uint64_t  bytes_to_fence;
			} fence;
			struct {
				/* QW[5] SW */
				uint64_t reserved; /* Common fields */

				/* QW[6] SW */
				uintptr_t rma_request_vaddr;
				/* QW[7] SW */
				uintptr_t offset;
			} rma;
		} target;

		uint64_t reserved_n[7]; /* QW[8-14] SW */

	} __attribute__((__packed__)) dput;

	/*    OPX UD HEADER                   */
	struct {
		uint64_t reserved[2]; /* QW[0-1] */

		/* QW[2] BTH (unused)*/
		uint16_t reserved_1[3];
		uint8_t	 opcode; // lower bits reserved for subctxt
		uint8_t	 reserved_2;

		uint64_t reserved_n[12]; /* QW[3-14] SW */

	} __attribute__((__packed__)) ud;

	/*    OPX SERVICE HEADER                   */
	struct {
		uint64_t reserved[3]; /* QW[0-2] */

		/* QW[3] BTH/KDETH (psn,offset_ver_tid) */
		uint32_t reserved_3;
		uint16_t origin_reliability_subctxt_rx;
		uint8_t	 reserved_1[2];

		/* QW[4] KDETH (unused) */
		uint32_t reserved_2;
		uint32_t key_dw_suffix;

		/* QW[5-7] SW */
		uint64_t psn_count;
		uint64_t psn_start;
		uint64_t key; /* fi_opx_reliability_service_flow_key */

		uint64_t reserved_n[7]; /* QW[8-14] SW */

	} __attribute__((__packed__)) service; /* "reliability service" */
} __attribute__((__packed__)) __attribute__((__aligned__(8)));

#define OPX_HDR_SLID_9B(_hdr) ((union opx_hfi1_packet_hdr *) _hdr)->lrh_9b.slid
#define OPX_HDR_SLID_16B(_hdr)                                        \
	(((union opx_hfi1_packet_hdr *) _hdr)->lrh_16b.slid20 << 20 | \
	 ((union opx_hfi1_packet_hdr *) _hdr)->lrh_16b.slid)

static_assert(sizeof(union opx_hfi1_packet_hdr) == sizeof(uint64_t[15]),
	      "sizeof(union opx_hfi1_packet_hdr) must be 15 qwords!");

static inline size_t fi_opx_hfi1_packet_hdr_message_length(const union opx_hfi1_packet_hdr *const hdr)
{
	size_t message_length = 0;
	switch (hdr->bth.opcode) {
	case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT:
	case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT:
	case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ:
		message_length = hdr->inject.subctxt_message_length >> 3;
		break;
	case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER:
	case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER:
	case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ:
		message_length =
			(hdr->send.subctxt_xfer_bytes_tail >> 3) + hdr->send.payload_qws_total * sizeof(uint64_t);
		break;
	case FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST:
	case FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST:
	case FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ:
		message_length = (size_t) hdr->mp_eager_first.payload_bytes_total;
		break;
	case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS:
	case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS:
	case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ:
		// assert(hdr->rendezvous.niov == 1);
		message_length = hdr->rendezvous.message_length;
		break;
	default:
		fprintf(stderr, "%s:%s():%d abort. hdr->stl.bth.opcode = %02x (%u)\n", __FILE__, __func__, __LINE__,
			hdr->bth.opcode, hdr->bth.opcode);
		abort();
		break;
	}

	return message_length;
}

static inline size_t opx_hfi1_packet_hdr_payload_bytes(const union opx_hfi1_packet_hdr *const hdr,
						       const enum opx_hfi1_type		      hfi1_type)
{
	/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
	uint16_t lrh_pktlen_le;
	size_t	 total_bytes_to_copy;

	if (hfi1_type & OPX_HFI1_CNX000) {
		lrh_pktlen_le	    = (uint16_t) hdr->lrh_16B.pktlen;
		total_bytes_to_copy = (lrh_pktlen_le - 1) * 8; /* do not copy the trailing tail/icrc QW*/
		return total_bytes_to_copy - sizeof(struct fi_opx_hfi1_stl_packet_hdr_16B);
	}

	lrh_pktlen_le	    = ntohs(hdr->lrh_9B.pktlen);
	total_bytes_to_copy = (lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
	return total_bytes_to_copy - sizeof(struct fi_opx_hfi1_stl_packet_hdr_9B);
}

#ifndef NDEBUG

#ifndef OPX_RC2_MASK
#define OPX_RC2_MASK 0b100
#endif

#define OPX_PRINT_16B_PBC(a)	    opx_print_16B_pbc((a), __func__)
#define OPX_PRINT_9B_PBC(a, b)	    opx_print_9B_pbc((a), b, __func__)
#define OPX_JKR_PRINT_16B_LRH(a, b) opx_jkr_print_16B_lrh((a), (b), __func__)
#define OPX_JKR_PRINT_16B_BTH(a, b) opx_jkr_print_16B_bth((a), (b), __func__)

void opx_print_16B_pbc(uint64_t pbc1, const char *func);
void opx_print_9B_pbc(uint64_t pbc1, const enum opx_hfi1_type hfi1_type, const char *func);

void opx_jkr_print_16B_lrh(uint64_t lrh1, uint64_t lrh2, const char *func);
void opx_jkr_print_16B_bth(uint64_t bth1, uint64_t bth2, const char *func);

static inline void fi_opx_hfi1_dump_stl_packet_hdr(const union opx_hfi1_packet_hdr *hdr,
						   const enum opx_hfi1_type hfi1_type, const char *fn,
						   const unsigned ln)
{
#if __GNUC__ > 9
#pragma GCC diagnostic ignored "=Waddress-of-packed-member"
#endif
	const uint64_t *const qw = (uint64_t *) hdr;
#if __GNUC__ > 9
#pragma GCC diagnostic pop
#endif

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "%s():%u ==== dump stl packet header @ %p [%016lx %016lx %016lx %016lx]\n", fn, ln, hdr, qw[0],
		     qw[1], qw[2], qw[3]);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .lrh.flags .............     0x%04hx\n", fn, ln,
		     hdr->lrh_9B.flags);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "%s():%u .lrh.dlid ..............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh_9B.dlid,
		     hdr->lrh_9B.dlid, __be16_to_cpu24((__be16) (hdr->lrh_9B.dlid)));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "%s():%u .lrh.pktlen ............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh_9B.pktlen,
		     hdr->lrh_9B.pktlen, ntohs(hdr->lrh_9B.pktlen));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "%s():%u .lrh.slid ..............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh_9B.slid,
		     hdr->lrh_9B.slid, __be16_to_cpu24((__be16) hdr->lrh_9B.slid));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u\n", fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.opcode ............       0x%02x \n", fn, ln,
		     hdr->bth.opcode);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.bth_1 .............       0x%02x \n", fn, ln,
		     hdr->bth.bth_1);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.pkey ..............     0x%04hx \n", fn, ln,
		     hdr->bth.pkey);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.ecn ...............       0x%02x, RC2 %u \n", fn,
		     ln, hdr->bth.ecn, (hdr->bth.ecn & OPX_RC2_MASK) >> 2);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.qp ................       0x%02x \n", fn, ln,
		     hdr->bth.qp);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.subctxt_rx ........       0x%04x \n", fn, ln,
		     hdr->bth.subctxt_rx);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u\n", fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .bth.psn ............... 0x%08x \n", fn, ln,
		     hdr->bth.psn);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .kdeth.offset_ver_tid .. 0x%08x\n", fn, ln,
		     hdr->kdeth.offset_ver_tid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u\n", fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .kdeth.jkey ............     0x%04hx\n", fn, ln,
		     hdr->kdeth.jkey);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .kdeth.hcrc ............     0x%04hx\n", fn, ln,
		     hdr->kdeth.hcrc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s():%u .kdeth.unused .......... 0x%08x\n", fn, ln,
		     hdr->kdeth.unused);

	return;
}

static inline void fi_opx_hfi1_dump_packet_hdr(const union opx_hfi1_packet_hdr *const hdr,
					       const enum opx_hfi1_type hfi1_type, const char *fn, const unsigned ln)
{
	const uint64_t *const qw  = (uint64_t *) hdr;
	const pid_t	      pid = getpid();
	// fi_opx_hfi1_dump_stl_packet_hdr (hdr, hfi1_type, fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "(%d) %s():%u ==== dump packet header @ %p [%016lx %016lx %016lx %016lx]\n", pid, fn, ln, hdr,
		     qw[0], qw[1], qw[2], qw[3]);
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .lrh.flags ...........     0x%04hx\n",
			     pid, fn, ln, hdr->lrh_9B.flags);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .lrh.dlid ............     0x%04hx (be: %5hu, le: %5hu)\n", pid, fn, ln,
			     hdr->lrh_9B.dlid, hdr->lrh_9B.dlid, __be16_to_cpu24((__be16) hdr->lrh_9B.dlid));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .lrh.pktlen ..........     0x%04hx (be: %5hu, le: %5hu)\n", pid, fn, ln,
			     hdr->lrh_9B.pktlen, hdr->lrh_9B.pktlen, ntohs(hdr->lrh_9B.pktlen));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .lrh.slid ............     0x%04hx (be: %5hu, le: %5hu)\n", pid, fn, ln,
			     hdr->lrh_9B.slid, hdr->lrh_9B.slid, __be16_to_cpu24((__be16) hdr->lrh_9B.slid));
	} else {
		OPX_JKR_PRINT_16B_LRH(hdr->qw_16B[0], hdr->qw_16B[1]);
		OPX_JKR_PRINT_16B_BTH(hdr->qw_16B[2], hdr->qw_16B[2]);
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u\n", pid, fn, ln);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .stl.bth.opcode ......     0x%02x  (%s)\n", pid,
		     fn, ln, hdr->bth.opcode, opx_hfi1_bth_opcode_to_string((uint16_t) hdr->bth.opcode));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .bth.bth_1  ..........     0x%02x \n", pid, fn,
		     ln, hdr->bth.bth_1);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .bth.pkey   ..........     0x%04hx\n", pid, fn,
		     ln, hdr->bth.pkey);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .bth.ecn    ..........     0x%02x, RC2 %u \n",
		     pid, fn, ln, hdr->bth.ecn, (hdr->bth.ecn & OPX_RC2_MASK) >> 2);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .bth.qp     ..........     0x%02x \n", pid, fn,
		     ln, hdr->bth.qp);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .bth.subctxt_rx ......     0x%04x \n", pid, fn,
		     ln, hdr->bth.subctxt_rx);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .bth.psn    ..........     0x%08x \n", pid, fn,
		     ln, hdr->bth.psn);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u\n", pid, fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .kdeth.offset_ver_tid.     0x%08x \n", pid, fn,
		     ln, hdr->kdeth.offset_ver_tid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .jkey       ..........     0x%04hx \n", pid, fn,
		     ln, hdr->kdeth.jkey);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .hcrc       ..........     0x%04hx \n", pid, fn,
		     ln, hdr->kdeth.hcrc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .unused     ..........     0x%08x \n", pid, fn,
		     ln, hdr->kdeth.unused);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u ofi_tag, last 9B QW...     0x%16.16lx\n", pid,
		     fn, ln, hdr->qw_9B[6]);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u\n", pid, fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .match.origin_rx .....     0x%04x \n", pid, fn,
		     ln, hdr->match.origin_rx);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .match.ofi_data ......     0x%08x \n", pid, fn,
		     ln, hdr->match.ofi_data);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .match.ofi_tag .......     0x%016lx \n", pid, fn,
		     ln, hdr->match.ofi_tag);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u\n", pid, fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .inject.subctxt_message_length     0x%04x \n",
		     pid, fn, ln, hdr->inject.subctxt_message_length);

	switch (hdr->bth.opcode) {
	case FI_OPX_HFI_BTH_OPCODE_UD:
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .ud.opcode ...     0x%02x (%s) \n", pid,
			     fn, ln, hdr->ud.opcode, opx_hfi1_ud_opcode_to_string(hdr->ud.opcode));
		break;
	case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT:
	case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT:
	case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ:
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .inject.subctxt_message_length ...     0x%02x \n", pid, fn, ln,
			     hdr->inject.subctxt_message_length);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .inject.app_data_u64[0] ..     0x%016lx \n", pid, fn, ln,
			     hdr->inject.app_data_u64[0]);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .inject.app_data_u64[1] ..     0x%016lx \n", pid, fn, ln,
			     hdr->inject.app_data_u64[1]);
		break;
	case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER:
	case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER:
	case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ:
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .send.subctxt_xfer_bytes_tail ....     0x%02x \n", pid, fn, ln,
			     hdr->send.subctxt_xfer_bytes_tail);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .send.payload_qws_total ..     0x%04x \n", pid, fn, ln,
			     hdr->send.payload_qws_total);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .send.xfer_tail ..........     0x%016lx \n", pid, fn, ln,
			     hdr->send.xfer_tail);
		break;
	case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS:
	case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS: /* calculate (?) total bytes to be transfered */
	case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ:
	case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ: /* calculate (?) total bytes to be transfered */
	case FI_OPX_HFI_BTH_OPCODE_RZV_CTS:
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .cts.target.vaddr.ntidpairs ..........     0x%x \n", pid, fn, ln,
			     hdr->cts.target.vaddr.ntidpairs);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .cts.target.opcode          ..........     0x%x (%s) \n", pid, fn, ln,
			     hdr->cts.target.opcode, opx_hfi1_dput_opcode_to_string(hdr->cts.target.opcode));
		break;
	case FI_OPX_HFI_BTH_OPCODE_RMA_RTS:
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .rma_rts.opcode .............. 0x%x\n",
			     pid, fn, ln, hdr->rma_rts.opcode);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .rma_rts.dt/op ............... 0x%x/0x%x\n", pid, fn, ln, hdr->rma_rts.dt,
			     hdr->rma_rts.op);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u .rma_rts.niov ................ 0x%x\n",
			     pid, fn, ln, hdr->rma_rts.niov);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .rma_rts.key ................. 0x%16.16lx\n", pid, fn, ln, hdr->rma_rts.key);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(%d) %s():%u .rma_rts.rma_request_vaddr ... 0x%16.16lx\n", pid, fn, ln,
			     hdr->rma_rts.rma_request_vaddr);
		break;
	default:
		break;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u\n", pid, fn, ln);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(%d) %s():%u ==== QWs 4-7 : [%016lx %016lx %016lx %016lx]\n",
		     pid, fn, ln, qw[4], qw[5], qw[6], qw[7]);

	return;
}

#else
// Disable the macros
#define OPX_PRINT_16B_PBC(a)
#define OPX_PRINT_9B_PBC(a, b)
#define OPX_JKR_PRINT_16B_LRH(a, b)
#define OPX_JKR_PRINT_16B_BTH(a, b)

void opx_print_16B_pbc(uint64_t pbc1, const char *func);
void opx_print_9B_pbc(uint64_t pbc1, const enum opx_hfi1_type hfi1_type, const char *func);
void opx_jkr_print_16B_lrh(uint64_t lrh1, uint64_t lrh2, const char *func);
void opx_jkr_print_16B_bth(uint64_t bth1, uint64_t bth2, const char *func);

static inline void fi_opx_hfi1_dump_packet_hdr(const union opx_hfi1_packet_hdr *const hdr,
					       const enum opx_hfi1_type hfi1_type, const char *fn, const unsigned ln)
{
	return;
}

#endif

/* Defined but unused.  Add as needed for debug */
#define OPX_DEBUG_PRINT_HDR(__hdr, __hfi1_type)                                      \
	if (__hfi1_type & OPX_HFI1_CNX000) {                                         \
		OPX_JKR_PRINT_16B_LRH(__hdr->qw_16B[0], __hdr->qw_16B[1]);           \
		OPX_JKR_PRINT_16B_BTH(__hdr->qw_16B[2], __hdr->qw_16B[3]);           \
	} else {                                                                     \
		fi_opx_hfi1_dump_packet_hdr(__hdr, __hfi1_type, __func__, __LINE__); \
	}

#define OPX_DEBUG_PRINT_PBC(__pbc, __hfi1_type)        \
	if (__hfi1_type & OPX_HFI1_CNX000) {           \
		OPX_PRINT_16B_PBC(__pbc, __hfi1_type); \
	} else {                                       \
		OPX_PRINT_9B_PBC(__pbc, __hfi1_type);  \
	}

#define OPX_DEBUG_PRINT_PBC_HDR(__pbc, __hdr, __hfi1_type)                           \
	if (__hfi1_type & OPX_HFI1_CNX000) {                                         \
		OPX_PRINT_16B_PBC(__pbc);                                            \
		OPX_JKR_PRINT_16B_LRH(__hdr->qw_16B[0], __hdr->qw_16B[1]);           \
		OPX_JKR_PRINT_16B_BTH(__hdr->qw_16B[2], __hdr->qw_16B[3]);           \
	} else {                                                                     \
		OPX_PRINT_9B_PBC(__pbc, __hfi1_type);                                \
		fi_opx_hfi1_dump_packet_hdr(__hdr, __hfi1_type, __func__, __LINE__); \
	}

#define OPX_DEBUG_PRINT_PBC_HDR_QW(q0, q1, q2, q3, q4, __hfi1_type) \
	if (__hfi1_type & OPX_HFI1_CNX000) {                        \
		OPX_PRINT_16B_PBC(q0);                              \
		OPX_JKR_PRINT_16B_LRH(q1, q2);                      \
		OPX_JKR_PRINT_16B_BTH(q3, q4);                      \
	}

union cacheline {
	uint64_t qw[8];
	uint32_t dw[16];
	uint8_t	 byte[64];
};

union opx_hfi1_dput_iov {
	uint64_t qw[8];
	struct {
		uintptr_t	   rbuf;
		uintptr_t	   sbuf;
		uint64_t	   bytes;
		uint64_t	   rbuf_device;
		uint64_t	   sbuf_device;
		enum fi_hmem_iface rbuf_iface;
		enum fi_hmem_iface sbuf_iface;
		uint64_t	   pad;
		uint64_t	   sbuf_handle;
	};
};

struct fi_opx_hfi1_dput_fetch {
	uintptr_t fetch_rbuf;
	uintptr_t rma_request_vaddr;
};

union fi_opx_hfi1_dput_rbuf {
	uintptr_t ptr;
	uint32_t  dw[2];
};

static inline uintptr_t fi_opx_dput_rbuf_out(const uintptr_t rbuf_in)
{
	union fi_opx_hfi1_dput_rbuf rbuf_out = {.ptr = htonll(rbuf_in)};
	rbuf_out.dw[1]			     = ntohl(rbuf_out.dw[1]);
	return rbuf_out.ptr;
}

static inline uintptr_t fi_opx_dput_rbuf_in(const uintptr_t rbuf_out)
{
	union fi_opx_hfi1_dput_rbuf rbuf_in = {.ptr = rbuf_out};
	rbuf_in.dw[1]			    = htonl(rbuf_in.dw[1]);
	return ntohll(rbuf_in.ptr);
}

struct fi_opx_hmem_iov {
	uintptr_t	   buf;
	uint64_t	   len;
	uint64_t	   device;
	enum fi_hmem_iface iface;
} __attribute__((__packed__));

#define FI_OPX_MAX_HMEM_IOV	    ((OPX_HFI1_MAX_PKT_SIZE - sizeof(uintptr_t)) / sizeof(struct fi_opx_hmem_iov))
#define FI_OPX_RZV_NONCONTIG_UNUSED (OPX_HFI1_MAX_PKT_SIZE - 8 - (((OPX_HFI1_MAX_PKT_SIZE - 8) / 28) * 28))
#define FI_OPX_MAX_DPUT_IOV	    ((OPX_HFI1_MAX_PKT_SIZE / sizeof(union opx_hfi1_dput_iov) - 4) + 3)

#define FI_OPX_MAX_DPUT_TIDPAIRS \
	((OPX_HFI1_MAX_PKT_SIZE - sizeof(union opx_hfi1_dput_iov) - (4 * sizeof(uint32_t))) / sizeof(uint32_t))

#define OPX_IMMEDIATE_BYTE_COUNT_SHIFT (5)
#define OPX_IMMEDIATE_BYTE_COUNT_MASK  (0xE0)
#define OPX_IMMEDIATE_QW_COUNT_SHIFT   (2)
#define OPX_IMMEDIATE_QW_COUNT_MASK    (0x1C)
#define OPX_IMMEDIATE_BLOCK_SHIFT      (1)
#define OPX_IMMEDIATE_BLOCK_MASK       (0x02)
#define OPX_IMMEDIATE_TAIL_SHIFT       (0)
#define OPX_IMMEDIATE_TAIL_MASK	       (0x01)
#define OPX_IMMEDIATE_TAIL_BYTE_COUNT  (7)

union fi_opx_hfi1_rzv_rts_immediate_info {
	uint64_t qw0;
	struct {
		uint8_t tail_bytes[7];
		uint8_t count;
	};
};

static_assert(sizeof(((union fi_opx_hfi1_rzv_rts_immediate_info *) 0)->tail_bytes) == OPX_IMMEDIATE_TAIL_BYTE_COUNT,
	      "sizeof(immediate_info->tail_bytes) must be equal to OPX_IMMEDIATE_TAIL_BYTE_COUNT!");

/* Cache "blocked" payloads in 16B are currently "tricky".
 * The sender will always send 1 QW of header after SOP so STORE'ing
 * a full cacheline block is not possible.  The payload will
 * arrive cacheline aligned in the eager buffer but not in the
 * same "blocks" as written.
 *
 * For example, contiguous rzv:
 *
 * STORE(tag + 7 qw's of CACHELINE 0 unused[1], not unused[2] as in 9B above)
 * optionally STORE(icrc/tail) if no more immediate data
 *
 * STORE(full block of immediate fragment unaligned data)
 * STORE(full block of immediate data)
 * STORE(full block of immediate end data)
 * STORE(icrc/tail)
 */

struct opx_payload_rzv_contig {
	/* ==== CACHE LINE 0 ==== */

	uintptr_t src_vaddr;
	uint64_t  src_len;
	uint64_t  src_device_id;
	uint64_t  src_iface;
	uint64_t  immediate_info;
	uintptr_t origin_byte_counter_vaddr;
	uint64_t  unused;

	/* ==== CACHE LINE 1 (WFR/9B only) ==== */
	union {
		struct {
			uint8_t	 immediate_byte[8];
			uint64_t immediate_qw[7];
		};

		union cacheline cache_line_1;
	};

	/* ==== CACHE LINE 2-127 ==== */

	union cacheline immediate_block[OPX_HFI1_MAX_PKT_SIZE / sizeof(union cacheline) - 2];
};

/* 9B and common payload structure */
union fi_opx_hfi1_packet_payload {
	uint8_t	 byte[OPX_HFI1_MAX_PKT_SIZE];
	uint64_t qw[OPX_HFI1_MAX_PKT_SIZE >> 3];
	union {
		struct {
			uint64_t		      contig_9B_padding;
			struct opx_payload_rzv_contig contiguous;
		};
		struct opx_payload_rzv_contig contiguous_16B;

		struct {
			/* ==== CACHE LINE 0 ==== */

			uintptr_t	       origin_byte_counter_vaddr;
			struct fi_opx_hmem_iov iov[2];

			/* ==== CACHE LINE 1-127 (for max pkt size) ==== */
			struct fi_opx_hmem_iov iov_ext[FI_OPX_MAX_HMEM_IOV - 2];
#if FI_OPX_RZV_NONCONTIG_UNUSED
			uint8_t unused[FI_OPX_RZV_NONCONTIG_UNUSED];
#endif

		} noncontiguous;

		struct {
			uintptr_t	origin_byte_counter_vaddr; // copy into cts packet that will be sent
			uint64_t	alignment_unused[2];	   // Align the IPC handle on a cacheline
			struct ipc_info ipc_info;		   // Needed on receiver for opening handle
		} ipc;
	} rendezvous;

	struct {
		union opx_hfi1_dput_iov iov[FI_OPX_MAX_DPUT_IOV];
	} rma_rts;

	struct {
		union opx_hfi1_dput_iov iov[FI_OPX_MAX_DPUT_IOV];
	} cts;

	/* tid_cts extends cts*/
	struct {
		/* ==== CACHE LINE 0 ==== */
		union opx_hfi1_dput_iov iov[1];
		/* ==== CACHE LINE 1 ==== */
		uint32_t tid_offset;
		uint32_t ntidpairs;
		int32_t	 origin_byte_counter_adjust;
		uint32_t unused;

		uint32_t tidpairs[FI_OPX_MAX_DPUT_TIDPAIRS];
	} tid_cts;
} __attribute__((__aligned__(32)));

static_assert(sizeof(union fi_opx_hfi1_packet_payload) <= OPX_HFI1_MAX_PKT_SIZE,
	      "sizeof(union fi_opx_hfi1_packet_payload) must be <= OPX_HFI1_MAX_PKT_SIZE!");
static_assert(
	offsetof(union fi_opx_hfi1_packet_payload, rendezvous.contiguous.immediate_byte) == FI_OPX_CACHE_LINE_SIZE,
	"union fi_opx_hfi1_packet_payload.rendezvous.contiguous.immediate_byte should be aligned on cacheline 1!");
static_assert(
	offsetof(union fi_opx_hfi1_packet_payload, rendezvous.contiguous.immediate_block) == 2 * FI_OPX_CACHE_LINE_SIZE,
	"union fi_opx_hfi1_packet_payload.rendezvous.contiguous.immediate_block should be aligned on cacheline 2!");
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.noncontiguous.iov) == 8,
	      "union fi_opx_hfi1_packet_payload.rendezvous.noncontiguous.iov should be 8 bytes into cacheline 0!");
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.noncontiguous.iov_ext) == FI_OPX_CACHE_LINE_SIZE,
	      "union fi_opx_hfi1_packet_payload.rendezvous.noncontiguous.iov_ext should be aligned on cacheline 1!");
static_assert(
	OPX_HFI1_MAX_PKT_SIZE - sizeof(uintptr_t) - ((FI_OPX_MAX_HMEM_IOV) * sizeof(struct fi_opx_hmem_iov)) ==
		FI_OPX_RZV_NONCONTIG_UNUSED,
	"FI_OPX_RZV_NONCONTIG_UNUSED should be based on union fi_opx_hfi1_packet_payload.rendezvous.noncontiguous!");
#if FI_OPX_RZV_NONCONTIG_UNUSED
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.noncontiguous.unused) +
			      FI_OPX_RZV_NONCONTIG_UNUSED ==
		      OPX_HFI1_MAX_PKT_SIZE,
	      "union fi_opx_hfi1_packet_payload.rendezvous.noncontiguous.unused should end at packet MTU!");
#endif
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.ipc.ipc_info.ipc_handle) == FI_OPX_CACHE_LINE_SIZE,
	      "union fi_opx_hfi1_packet_payload.rendezvous.ipc.ipc_info.ipc_handle should be aligned on cacheline 1!");
static_assert(
	(offsetof(union fi_opx_hfi1_packet_payload, tid_cts.tidpairs) +
	 sizeof(((union fi_opx_hfi1_packet_payload *) 0)->tid_cts.tidpairs)) == OPX_HFI1_MAX_PKT_SIZE,
	"offsetof(fi_opx_hfi1_packet_payload.tid_cts.tidpairs) + sizeof(...tid_cts.tidpairs) should equal packet MTU! "
	"If you added/removed fields in struct tid_cts, you need to adjust OPX_HFI1_MAX_PKT_SIZE!");

struct fi_opx_hfi1_ue_packet_slist;
struct fi_opx_hfi1_ue_packet {
	/* == CACHE LINE 0 == */
	struct fi_opx_hfi1_ue_packet *next;
	struct fi_opx_hfi1_ue_packet *prev;

	struct {
		struct fi_opx_hfi1_ue_packet	   *next;
		struct fi_opx_hfi1_ue_packet	   *prev;
		struct fi_opx_hfi1_ue_packet_slist *ht;
	} tag_ht;

	struct {
		uint32_t rank;
		uint32_t rank_inst;
	} daos_info;

	/* Copies of tag, lid and rx so that
	 * packet can be matched only accessing the
	 * first cacheline */
	uint64_t  tag;
	opx_lid_t lid;
	uint16_t  subctxt_rx;
	uint8_t	  is_shm;

	uint8_t unused;

	/* == CACHE LINE 1, 2 == */
	uint64_t		  recv_time_ns;
	union opx_hfi1_packet_hdr hdr;

	/* == CACHE LINE 3 == */
	union fi_opx_hfi1_packet_payload payload;
} __attribute__((__packed__)) __attribute__((aligned(64)));

static_assert(offsetof(struct fi_opx_hfi1_ue_packet, recv_time_ns) == FI_OPX_CACHE_LINE_SIZE,
	      "struct fi_opx_hfi1_ue_packet->recv_time_ns should be aligned on cache boundary!");

static_assert(offsetof(struct fi_opx_hfi1_ue_packet, payload) == 3 * FI_OPX_CACHE_LINE_SIZE,
	      "struct fi_opx_hfi1_ue_packet->payload should be aligned on cache boundary!");

struct fi_opx_hfi1_ue_packet_slist {
	struct fi_opx_hfi1_ue_packet *head;
	struct fi_opx_hfi1_ue_packet *tail;
	uint64_t		      length;
};

static inline void fi_opx_hfi1_ue_packet_slist_init(struct fi_opx_hfi1_ue_packet_slist *list)
{
	list->head = list->tail = NULL;
	list->length		= 0;
}

static inline int fi_opx_hfi1_ue_packet_slist_empty(struct fi_opx_hfi1_ue_packet_slist *list)
{
	return !list->head;
}

static inline void fi_opx_hfi1_ue_packet_slist_insert_tail(struct fi_opx_hfi1_ue_packet	      *item,
							   struct fi_opx_hfi1_ue_packet_slist *list)
{
	assert(item->next == NULL);
	if (fi_opx_hfi1_ue_packet_slist_empty(list)) {
		assert(list->length == 0);
		item->prev = NULL;
		list->head = item;
	} else {
		assert(list->length > 0);
		item->prev	 = list->tail;
		list->tail->next = item;
	}
	++list->length;

	list->tail = item;
}

/*
 * Remove item from the list, but don't free it.
 * Return the removed item.
 */
static inline struct fi_opx_hfi1_ue_packet *
fi_opx_hfi1_ue_packet_slist_pop_item(struct fi_opx_hfi1_ue_packet *item, struct fi_opx_hfi1_ue_packet_slist *list)
{
	struct fi_opx_hfi1_ue_packet *prev = item->prev;
	if (prev == NULL) {
		list->head = item->next;
	} else {
		item->prev = NULL;
		prev->next = item->next;
	}

	if (!item->next) {
		list->tail = prev;
	} else {
		item->next->prev = prev;
	}
	item->next = NULL;

	assert(list->length > 0);
	--list->length;

	return item;
}

/*
 * Remove item from the list, and add it to the free pool.
 * Return next item in the list.
 */
static inline struct fi_opx_hfi1_ue_packet *
fi_opx_hfi1_ue_packet_slist_remove_item(struct fi_opx_hfi1_ue_packet *item, struct fi_opx_hfi1_ue_packet_slist *list)
{
	struct fi_opx_hfi1_ue_packet *next_item = item->next;
	struct fi_opx_hfi1_ue_packet *prev	= item->prev;
	if (prev == NULL) {
		list->head = next_item;
	} else {
		item->prev = NULL;
		prev->next = next_item;
	}

	if (!next_item) {
		list->tail = prev;
	} else {
		next_item->prev = prev;
	}

	item->next = NULL;

	assert(list->length > 0);
	--list->length;

#ifndef NDEBUG
	/* Clobber the contents of the packet header and payload before
	   adding it to the free pool. This way, if there is still a reference
	   to this packet in use that we don't expect, the garbage value
	   should cause an error which will expose such cases. */
	memset(&item->hdr, 0x55, sizeof(item->hdr));
	memset(&item->payload, 0xAA, sizeof(item->payload));
#endif
	/* add uepkt to ue free pool */
	ofi_buf_free(item);

	return next_item;
}

#if 0
static inline
void fi_opx_hfi1_dump_packet_hdr (const union fi_opx_hfi1_packet_hdr * const hdr,
		const char * fn, const unsigned ln) {

	const uint64_t * const qw = (uint64_t *)hdr;
	const pid_t pid = getpid();

	fprintf(stderr, "(%d) %s():%u ==== dump packet header @ %p [%016lx %016lx %016lx %016lx]\n",
		pid, fn, ln, hdr, qw[0], qw[1], qw[2], qw[3]);
	fprintf(stderr, "(%d) %s():%u .stl.lrh.flags ...................... 0x%04hx\n",
		pid, fn, ln, hdr->stl.lrh.flags);
	fprintf(stderr, "(%d) %s():%u .stl.lrh.dlid                         0x%04hx (be: %5hu, le: %5hu)\n",
		pid, fn, ln, hdr->stl.lrh.dlid, hdr->stl.lrh.dlid,
		ntohs(hdr->stl.lrh.dlid));
	fprintf(stderr, "(%d) %s():%u .stl.lrh.pktlen ..................... 0x%04hx (be: %5hu, le: %5hu)\n",
		pid, fn, ln, hdr->stl.lrh.pktlen, hdr->stl.lrh.pktlen,
		ntohs(hdr->stl.lrh.pktlen));
	fprintf(stderr, "(%d) %s():%u .stl.lrh.slid                         0x%04hx (be: %5hu, le: %5hu)\n",
		pid, fn, ln, hdr->stl.lrh.slid, hdr->stl.lrh.slid,
		ntohs(hdr->stl.lrh.slid));
	fprintf(stderr, "(%d) %s():%u .stl.bth.opcode ..................... 0x%02x  (%s)\n",
		pid, fn, ln, hdr->stl.bth.opcode,
		opx_hfi1_bth_opcode_to_string((uint16_t)hdr->stl.bth.opcode));
	fprintf(stderr, "(%d) %s():%u .stl.bth.psn                          0x%08x\n",
		pid, fn, ln, FI_OPX_HFI1_PACKET_PSN(hdr));

	fprintf(stderr, "(%d) %s():%u\n", pid, fn, ln);
	fprintf(stderr, "(%d) %s():%u .match.slid ......................... 0x%04x\n",
		pid, fn, ln, hdr->match.slid);
	fprintf(stderr, "(%d) %s():%u .match.origin_rx                      0x%04x\n",
		pid, fn, ln, hdr->match.origin_rx);
	fprintf(stderr, "(%d) %s():%u .match.ofi_data ..................... 0x%08x\n",
		pid, fn, ln, hdr->match.ofi_data);
	fprintf(stderr, "(%d) %s():%u .match.ofi_tag                        0x%016lx\n",
		pid, fn, ln, hdr->match.ofi_tag);
	fprintf(stderr, "(%d) %s():%u\n", pid, fn, ln);
	switch (hdr->stl.bth.opcode) {
		case FI_OPX_HFI_BTH_OPCODE_UD:
			fprintf(stderr, "(%d) %s():%u .ud.opcode .......................... 0x%02x (%s)\n",
				pid, fn, ln, hdr->ud.opcode,
				opx_hfi1_ud_opcode_to_string(hdr->ud.opcode));
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT:
		case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT:
		case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ:
		case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ:
			fprintf(stderr, "(%d) %s():%u .inject.subctxt_message_length .............. 0x%02x\n",
				pid, fn, ln, hdr->inject.subctxt_message_length);
			fprintf(stderr, "(%d) %s():%u .inject.app_data_u64[0]               0x%016lx\n",
				pid, fn, ln, hdr->inject.app_data_u64[0]);
			fprintf(stderr, "(%d) %s():%u .inject.app_data_u64[1] ............. 0x%016lx\n",
				pid, fn, ln, hdr->inject.app_data_u64[1]);
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER:
		case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER:
		case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ:
		case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ:
			fprintf(stderr, "(%d) %s():%u .send.xfer_bytes_tail ............... 0x%02x\n",
				pid, fn, ln, hdr->send.xfer_bytes_tail);
			fprintf(stderr, "(%d) %s():%u .send.payload_qws_total               0x%04x\n",
				pid, fn, ln, hdr->send.payload_qws_total);
			fprintf(stderr, "(%d) %s():%u .send.xfer_tail ..................... 0x%016lx\n",
				pid, fn, ln, hdr->send.xfer_tail);
			break;
		case FI_OPX_HFI_BTH_OPCODE_RZV_CTS:
			fprintf(stderr, "(%d) %s():%u .cts.origin ......................... 0x%x\n",
				pid, fn, ln, hdr->cts.origin_rx);
			fprintf(stderr, "(%d) %s():%u .cts.target.vaddr.ntidpairs           0x%x\n",
				pid, fn, ln, hdr->cts.target.vaddr.ntidpairs);
			fprintf(stderr, "(%d) %s():%u .cts.target.opcode .................. 0x%x (%s)\n",
				pid, fn, ln, hdr->cts.target.opcode,
				opx_hfi1_dput_opcode_to_string(hdr->cts.target.opcode));
			break;
		case FI_OPX_HFI_BTH_OPCODE_RZV_DATA:
			const uint8_t opcode = FI_OPX_HFI_DPUT_GET_OPCODE(hdr->dput.target.opcode_origin_rx);
			fprintf(stderr, "(%d) %s():%u .dput.target.opcode_origin_rx ................. 0x%x (%s)\n",
				pid, fn, ln, opcode,
				opx_hfi1_dput_opcode_to_string(opcode));
			fprintf(stderr, "(%d) %s():%u .dput.target.bytes                    0x%x\n",
				pid, fn, ln, hdr->dput.target.bytes);
			fprintf(stderr, "(%d) %s():%u .dput.target.last_bytes ............. 0x%x\n",
				pid, fn, ln, hdr->dput.target.last_bytes);
			if (opcode == FI_OPX_HFI_DPUT_OPCODE_RZV ||
			    opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG ||
				opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID) {
				fprintf(stderr, "(%d) %s():%u .dput.target.rzv.completion_vaddr     %p\n",
					pid, fn, ln,
					(void *) hdr->dput.target.rzv.completion_vaddr);
				fprintf(stderr, "(%d) %s():%u .dput.target.rzv.rbuf ............... %p\n",
					pid, fn, ln,
					(void *) fi_opx_dput_rbuf_in(hdr->dput.target.rzv.rbuf));
			} else if (opcode == FI_OPX_HFI_DPUT_OPCODE_FENCE) {
				fprintf(stderr, "(%d) %s():%u .dput.target.fence.completion_counter %p\n",
					pid, fn, ln,
					(void *) hdr->dput.target.fence.completion_counter);
				fprintf(stderr, "(%d) %s():%u .dput.target.fence.bytes_to_fence ... %lu\n",
					pid, fn, ln, hdr->dput.target.fence.bytes_to_fence);
			} else if (opcode == FI_OPX_HFI_DPUT_OPCODE_GET) {
				fprintf(stderr, "(%d) %s():%u .dput.target.get.rma_request_vaddr    %p\n",
					pid, fn, ln,
					(void *) hdr->dput.target.get.rma_request_vaddr);
				fprintf(stderr, "(%d) %s():%u .dput.target.get.rbuf ............... %p\n",
					pid, fn, ln,
					(void *) fi_opx_dput_rbuf_in(hdr->dput.target.get.rbuf));
			} else if (opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ) {
				fprintf(stderr, "(%d) %s():%u .dput.target.rma.rma_request_vaddr    %p\n",
					pid, fn, ln,
					(void *) hdr->dput.target.rma.rma_request_vaddr);
				fprintf(stderr, "(%d) %s():%u .dput.target.rma.offset ............... %p\n",
					pid, fn, ln,
					(void *) fi_opx_dput_rbuf_in(hdr->dput.target.rma.offset));
			} else if (opcode == FI_OPX_HFI_DPUT_OPCODE_PUT ||
				   opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
				   opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) {
				fprintf(stderr, "(%d) %s():%u .dput.target.mr.key                   0x%016lx\n",
					pid, fn, ln, hdr->dput.target.mr.key);
				fprintf(stderr, "(%d) %s():%u .dput.target.mr.offset .............. 0x%016lx\n",
					pid, fn, ln, hdr->dput.target.mr.offset);
			}
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS:
		case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS:	/* calculate (?) total bytes to be transfered */
		case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ:
		case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ:	/* calculate (?) total bytes to be transfered */
			break;
		case FI_OPX_HFI_BTH_OPCODE_RMA_RTS:
			fprintf(stderr, "(%d) %s():%u .rma_rts.opcode ......................... 0x%x\n",
				pid, fn, ln, hdr->rma_rts.opcode);
			fprintf(stderr, "(%d) %s():%u .rma_rts.target.vaddr.ntidpairs           0x%x\n",
				pid, fn, ln, hdr->rma_rts.ntidpairs);
			fprintf(stderr, "(%d) %s():%u .rma_rts.niov ........................... 0x%x\n",
				pid, fn, ln, hdr->rma_rts.niov);
			fprintf(stderr, "(%d) %s():%u .rma_rts.key ............................ 0x%x\n",
				pid, fn, ln, hdr->rma_rts.key);
			fprintf(stderr, "(%d) %s():%u .rma_rts.rma_request_vaddr .............. 0x%x\n",
				pid, fn, ln, hdr->rma_rts.rma_request_vaddr);
			break;
		default:
			break;
	}
	fprintf(stderr, "(%d) %s():%u\n", pid, fn, ln);
	fprintf(stderr, "(%d) %s():%u ==== QWs 4-7 : [%016lx %016lx %016lx %016lx]\n", pid, fn, ln, qw[4], qw[5], qw[6], qw[7]);

	return;
}
#endif

#endif /* _FI_PROV_OPX_HFI1_PACKET_H_ */
