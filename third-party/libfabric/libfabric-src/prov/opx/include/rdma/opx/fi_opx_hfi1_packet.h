/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2024 Cornelis Networks.
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
#include <arpa/inet.h>		/* only for fi_opx_addr_dump ... */
#include <sys/user.h>

#include "ofi_mem.h"
#include "rdma/fabric.h"	/* only for 'fi_addr_t' ... which is a typedef to uint64_t */
#include "rdma/opx/fi_opx_addr.h"

#if defined(__riscv) && defined(__riscv_xlen) && (__riscv_xlen == 64)
#ifndef PAGE_SIZE
/* 4K pages default */
#define PAGE_SIZE 4096
#endif
#endif

#define FI_OPX_ADDR_SEP_RX_MAX			(4)
#define FI_OPX_HFI1_PACKET_MTU			(8192)
#define OPX_HFI1_TID_PAGESIZE			(PAGE_SIZE) /* assume 4K, no hugepages*/
#define FI_OPX_HFI1_PACKET_IMM			(16)

/* opcodes (0x00..0xBF) are reserved */
#define FI_OPX_HFI_BTH_OPCODE_INVALID			(0xC0)
#define FI_OPX_HFI_BTH_OPCODE_RZV_CTS			(0xC1)
#define FI_OPX_HFI_BTH_OPCODE_RZV_DATA			(0xC2)
#define FI_OPX_HFI_BTH_OPCODE_RMA			(0xC3)
#define FI_OPX_HFI_BTH_OPCODE_ATOMIC			(0xC4)
#define FI_OPX_HFI_BTH_OPCODE_ACK			(0xC5)
#define FI_OPX_HFI_BTH_OPCODE_UD			(0xC6)	/* unreliabile datagram */
/* opcodes (0xC7..0xEF) are unused */
#define FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH		(0xF7)
#define FI_OPX_HFI_BTH_OPCODE_MSG_INJECT		(0xF8)
#define FI_OPX_HFI_BTH_OPCODE_MSG_EAGER			(0xF9)
#define FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST	(0xFA)
#define FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS		(0xFB)
#define FI_OPX_HFI_BTH_OPCODE_TAG_INJECT		(0xFC)
#define FI_OPX_HFI_BTH_OPCODE_TAG_EAGER			(0xFD)
#define FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST	(0xFE)
#define FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS		(0xFF)


#define FI_OPX_HFI1_PACKET_SLID(packet_hdr)				\
	(((packet_hdr).qw[0] & 0xFFFF000000000000ul) >> 48)

#define FI_OPX_HFI1_PACKET_PSN(packet_hdr)					\
	(((packet_hdr)->stl.bth.opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA)	\
		? ntohl((packet_hdr)->stl.bth.psn) & 0x00FFFFFF			\
		: (packet_hdr)->reliability.psn)

#define FI_OPX_HFI1_PACKET_ORIGIN_TX(packet_hdr)				\
	(((packet_hdr)->stl.bth.opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA)	\
		? (packet_hdr)->dput.target.origin_tx				\
		: (packet_hdr)->reliability.origin_tx)

#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING			(0x01)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK			(0x02)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK			(0x03)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT			(0x04)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT_ACK		(0x05)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH		(0x06)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK		(0x07)
#define FI_OPX_HFI_UD_OPCODE_RELIABILITY_NOOP			(0x08)

#define FI_OPX_HFI_DPUT_OPCODE_RZV				(0x00)
#define FI_OPX_HFI_DPUT_OPCODE_PUT				(0x01)
#define FI_OPX_HFI_DPUT_OPCODE_GET				(0x02)
#define FI_OPX_HFI_DPUT_OPCODE_FENCE				(0x03)
#define FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH			(0x04)
#define FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH		(0x05)
#define FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG			(0x06)
#define FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC			(0x07)
#define FI_OPX_HFI_DPUT_OPCODE_RZV_TID				(0x08)

/* KDETH header consts */

#define FI_OPX_HFI1_KDETH_VERSION		(0x1)
#define FI_OPX_HFI1_KDETH_VERSION_SHIFT		(30)		/* a.k.a. "HFI_KHDR_KVER_SHIFT" */
#define FI_OPX_HFI1_KDETH_VERSION_OFF_MASK	((FI_OPX_HFI1_KDETH_VERSION << FI_OPX_HFI1_KDETH_VERSION_SHIFT) - 1)

#define FI_OPX_HFI1_KDETH_TIDCTRL		(0x3)
#define FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT 	(26)
#define FI_OPX_HFI1_KDETH_TIDCTL_MASK		(FI_OPX_HFI1_KDETH_TIDCTRL << FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT)

#define FI_OPX_HFI1_KDETH_TID			(0x3ff)
#define FI_OPX_HFI1_KDETH_TID_SHIFT		(16)
#define FI_OPX_HFI1_KDETH_TID_MASK		(FI_OPX_HFI1_KDETH_TID << FI_OPX_HFI1_KDETH_TID_SHIFT)

#define HFI_KHDR_OFFSET_MASK 0x7fff
#define HFI_KHDR_OM_SHIFT 15
#define HFI_KHDR_TID_SHIFT 16
#define HFI_KHDR_TID_MASK 0x3ff
#define HFI_KHDR_TIDCTRL_SHIFT 26
#define HFI_KHDR_TIDCTRL_MASK 0x3
#define HFI_KHDR_INTR_SHIFT 28
#define HFI_KHDR_SH_SHIFT 29
#define HFI_KHDR_KVER_SHIFT 30
#define HFI_KHDR_KVER_MASK 0x3

struct fi_opx_hfi1_stl_packet_hdr {

	/* == quadword 0 == */
	union {
		uint64_t		qw[1];
		uint32_t		dw[2];
		uint16_t		w[4];
		struct {
			uint16_t	flags;	/* lrh.w[0] - big-endian! */
			uint16_t	dlid;	/* lrh.w[1] - big-endian! */
			uint16_t	pktlen;	/* lrh.w[2] - big-endian! */
			uint16_t	slid;	/* lrh.w[3] - big-endian! */
		} __attribute__((packed));
	} lrh;

	/* == quadword 1 == */
	union {
		uint32_t		dw[3];
		uint16_t		w[6];
		uint8_t			hw[12];
		struct {
			uint8_t		opcode;	/* bth.hw[0] */
			uint8_t		bth_1;	/* bth.hw[1] */
			uint16_t	pkey;	/* bth.w[1]  - big-endian! */
			uint8_t		ecn;	/* bth.hw[4] (FECN, BECN, and reserved) */
			uint8_t		qp;	/* bth.hw[5] */
			uint8_t		unused; /* bth.hw[6] -----> inject::message_length, send::xfer_bytes_tail */
			uint8_t		rx;	/* bth.hw[7] */

	/* == quadword 2 == */
			uint32_t	psn;	/* bth.dw[2] ..... the 'psn' field is unused for 'eager' packets -----> reliability::psn, etc */
		} __attribute__((packed));
	} bth;

	union {
		uint32_t		dw[3];
		uint16_t		w[6];
		uint8_t			hw[12];
		struct {
			uint32_t	offset_ver_tid;	/* kdeth.dw[0]  .... the 'offset' field is unused for 'eager' packets */

	/* == quadword 3 == */
			uint16_t	jkey;		/* kdeth.w[2]  */
			uint16_t	hcrc;		/* kdeth.w[3]  */
			uint32_t	unused;		/* kdeth.dw[2] -----> immediate data (32b) */
		} __attribute__((packed));
	} kdeth;

	/* == quadword 4,5,6 == */
	uint64_t			unused[3];

} __attribute__((__packed__));


/*
 * Define fields in the KDETH header so we can update the header
 * template.
 */
#define KDETH_OFFSET_SHIFT        0
#define KDETH_OFFSET_MASK         0x7fff
#define KDETH_OM_SHIFT            15
#define KDETH_OM_MASK             0x1
#define KDETH_TID_SHIFT           16
#define KDETH_TID_MASK            0x3ff
#define KDETH_TIDCTRL_SHIFT       26
#define KDETH_TIDCTRL_MASK        0x3
#define KDETH_INTR_SHIFT          28
#define KDETH_INTR_MASK           0x1
#define KDETH_SH_SHIFT            29
#define KDETH_SH_MASK             0x1
#define KDETH_KVER_SHIFT          30
#define KDETH_KVER_MASK           0x3
#define KDETH_JKEY_SHIFT          0x0
#define KDETH_JKEY_MASK           0xff
#define KDETH_HCRC_UPPER_SHIFT    16
#define KDETH_HCRC_UPPER_MASK     0xff
#define KDETH_HCRC_LOWER_SHIFT    24
#define KDETH_HCRC_LOWER_MASK     0xff

#include "opa_byteorder.h"

#define KDETH_GET(val, field)						\
	(((__le32_to_cpu((val))) >> KDETH_##field##_SHIFT) & KDETH_##field##_MASK)
#define KDETH_SET(dw, field, val) do {					\
		u32 dwval = __le32_to_cpu(dw);				\
		dwval &= ~(KDETH_##field##_MASK << KDETH_##field##_SHIFT); \
		dwval |= (((val) & KDETH_##field##_MASK) << \
			  KDETH_##field##_SHIFT);			\
		dw = __cpu_to_le32(dwval);				\
	} while (0)

#define KDETH_RESET(dw, field, val) ({ dw = 0; KDETH_SET(dw, field, val); })

/* KDETH OM multipliers and switch over point */
#define KDETH_OM_SMALL     4
#define KDETH_OM_SMALL_SHIFT     2
#define KDETH_OM_LARGE     64
#define KDETH_OM_LARGE_SHIFT     6
#define KDETH_OM_MAX_SIZE  (1 << ((KDETH_OM_LARGE / KDETH_OM_SMALL) + 1))

#define FI_OPX_EXP_TID_TIDLEN_MASK   0x7FFULL
#define FI_OPX_EXP_TID_TIDLEN_SHIFT  0
#define FI_OPX_EXP_TID_TIDCTRL_MASK  0x3ULL
#define FI_OPX_EXP_TID_TIDCTRL_SHIFT 20
#define FI_OPX_EXP_TID_TIDIDX_MASK   0x3FFULL
#define FI_OPX_EXP_TID_TIDIDX_SHIFT  22
#define FI_OPX_EXP_TID_GET(tid, field) (((tid) >> FI_OPX_EXP_TID_TID##field##_SHIFT) & FI_OPX_EXP_TID_TID##field##_MASK)
#define FI_OPX_EXP_TID_SET(field, value)			\
	(((value) & FI_OPX_EXP_TID_TID##field##_MASK) <<	\
	 FI_OPX_EXP_TID_TID##field##_SHIFT)
#define FI_OPX_EXP_TID_CLEAR(tid, field) ({					\
		(tid) &= ~(FI_OPX_EXP_TID_TID##field##_MASK <<			\
			   FI_OPX_EXP_TID_TID##field##_SHIFT);			\
		})
#define FI_OPX_EXP_TID_RESET(tid, field, value) do {				\
		FI_OPX_EXP_TID_CLEAR(tid, field);				\
		(tid) |= FI_OPX_EXP_TID_SET(field, (value));			\
	} while (0)

#define FI_OPX_PKT_RZV_FLAGS_SHIFT		(16)
#define FI_OPX_PKT_RZV_FLAGS_NONCONTIG		(1ul)
#define FI_OPX_PKT_RZV_FLAGS_NONCONTIG_MASK	(FI_OPX_PKT_RZV_FLAGS_NONCONTIG << FI_OPX_PKT_RZV_FLAGS_SHIFT)

#ifndef NDEBUG
static inline
void fi_opx_hfi1_dump_stl_packet_hdr (struct fi_opx_hfi1_stl_packet_hdr * hdr,
		const char * fn, const unsigned ln) {

#if __GNUC__ > 9 
#pragma GCC diagnostic ignored "=Waddress-of-packed-member"
#endif
	const uint64_t * const qw = (uint64_t *)hdr;
#if __GNUC__ > 9 
#pragma GCC diagnostic pop	
#endif

	fprintf(stderr, "%s():%u ==== dump stl packet header @ %p [%016lx %016lx %016lx %016lx]\n", fn, ln, hdr, qw[0], qw[1], qw[2], qw[3]);
	fprintf(stderr, "%s():%u .lrh.flags .............     0x%04hx\n", fn, ln, hdr->lrh.flags);
	fprintf(stderr, "%s():%u .lrh.dlid ..............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh.dlid, hdr->lrh.dlid, ntohs(hdr->lrh.dlid));
	fprintf(stderr, "%s():%u .lrh.pktlen ............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh.pktlen, hdr->lrh.pktlen, ntohs(hdr->lrh.pktlen));
	fprintf(stderr, "%s():%u .lrh.slid ..............     0x%04hx (be: %5hu, le: %5hu)\n", fn, ln, hdr->lrh.slid, hdr->lrh.slid, ntohs(hdr->lrh.slid));
	fprintf(stderr, "%s():%u\n", fn, ln);
	fprintf(stderr, "%s():%u .bth.opcode ............       0x%02x \n", fn, ln, hdr->bth.opcode);
	fprintf(stderr, "%s():%u .bth.bth_1 .............       0x%02x \n", fn, ln, hdr->bth.bth_1);
	fprintf(stderr, "%s():%u .bth.pkey ..............     0x%04hx \n", fn, ln, hdr->bth.pkey);
	fprintf(stderr, "%s():%u .bth.ecn ...............       0x%02x \n", fn, ln, hdr->bth.ecn);
	fprintf(stderr, "%s():%u .bth.qp ................       0x%02x \n", fn, ln, hdr->bth.qp);
	fprintf(stderr, "%s():%u .bth.unused ............       0x%02x \n", fn, ln, hdr->bth.unused);
	fprintf(stderr, "%s():%u .bth.rx ................       0x%02x \n", fn, ln, hdr->bth.rx);
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



/**
 * \brief HFI1 packet header
 *
 * The HFI1 packet header is consumed in many places and sometimes overloaded
 * for cache and memory allocation reasons.
 */
union fi_opx_hfi1_packet_hdr {


	uint64_t				qw[7];

	struct fi_opx_hfi1_stl_packet_hdr	stl;

	struct {
		/* == quadword 0 == */
		uint16_t			reserved_0[3];
		uint16_t			slid;

		/* == quadword 1 == */
		uint64_t			reserved_1;

		/* == quadword 2 == */
		uint32_t			psn		: 24;
		uint32_t			origin_tx	:  8;
		uint8_t				unused;
		uint8_t				reserved_2[3];

		/* == quadword 3,4,5,6 == */
		uint64_t			reserved_n[4];

	} __attribute__((__packed__)) reliability;


	struct {
		/* == quadword 0 == */
		uint16_t	reserved_0[3];
		uint16_t	slid;			/* used for FI_DIRECTED_RECV; identifies the node - big-endian! */

		/* == quadword 1 == */
		uint64_t	reserved_1;

		/* == quadword 2 == */
		uint8_t		reserved_2[3];
		uint8_t		origin_tx;		/* used for FI_DIRECTED_RECV; identifies the endpoint on the node */
		uint8_t		reserved_3;
		uint8_t		unused;
		uint16_t	reserved_4;

		/* == quadword 3 == */
		uint32_t	reserved_5;
		uint32_t	ofi_data;		/* used for FI_RX_CQ_DATA */

		/* == quadword 4 == */
		uint64_t	reserved_6;

		/* == quadword 5 == */
		uint64_t	reserved_7;

		/* == quadword 6 == */
		uint64_t	ofi_tag;

	} __attribute__((__packed__)) match;


	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		message_length;		/* only need 5 bits; maximum inject message size is 16 bytes */
		uint8_t		reserved_2;

		/* == quadword 2 == */
		uint64_t	reserved_3;

		/* == quadword 3 == */
		uint64_t	reserved_4;

		/* == quadword 4,5 == */
		union {
			uint8_t		app_data_u8[16];
			uint16_t	app_data_u16[8];
			uint32_t	app_data_u32[4];
			uint64_t	app_data_u64[2];
		};

		/* == quadword 6 == */
		uint64_t	reserved_6;

	} __attribute__((__packed__)) inject;


	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		xfer_bytes_tail;	/* only need 4 bits; maximum tail size is 8 bytes (or is it 7?) */
		uint8_t		reserved_2;

		/* == quadword 2 == */
		uint64_t	reserved_3;

		/* == quadword 3 == */
		uint64_t	reserved_4;

		/* == quadword 4 == */
		uint16_t	unused[3];
		uint16_t	payload_qws_total;	/* TODO - use stl.lrh.pktlen instead (num dws); only need 11 bits; maximum number of payload qw is 10240 / 8 = 1280 */

		/* == quadword 5 == */
		uint64_t	xfer_tail;

		/* == quadword 6 == */
		uint64_t	reserved_6;

	} __attribute__((__packed__)) send;

	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		xfer_bytes_tail;	/* Maximum tail size is 16 bytes */
		uint8_t		reserved_2;

		/* == quadword 2 == */
		uint32_t	reserved_3;		/* stl.bth.psn */
		uint32_t	payload_bytes_total;	/* stl.kdeth.offset_ver_tid (unused for eager packets),
							   Total length of payload across all mp-eager packets */

		/* == quadword 3 == */
		uint64_t	reserved_4;		/* stl.kdeth.jkey & stl.kdeth.hcrc, match.ofi_data */

		/* == quadword 4, 5 == */
		uint64_t	xfer_tail[2];

		/* == quadword 6 == */
		uint64_t	reserved_6;		/* match.ofi_tag */

	} __attribute__((__packed__)) mp_eager_first;

	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		xfer_bytes_tail;	/* Maximum tail size is 16 bytes */
		uint8_t		reserved_2;

		/* == quadword 2 == */
		uint64_t	reserved_3;		/* stl.bth.psn */

		/* == quadword 3 == */
		uint64_t	reserved_4;		/* stl.kdeth.jkey & stl.kdeth.hcrc */

		/* == quadword 4, 5 == */
		uint64_t	xfer_tail[2];

		/* == quadword 6 == */
		uint32_t	payload_offset;
		uint32_t	mp_egr_uid;

	} __attribute__((__packed__)) mp_eager_nth;

	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		origin_rx;
		uint8_t		reserved_2;

		/* == quadword 2 == */
		uint64_t	reserved_3;

		/* == quadword 3 == */
		uint64_t	reserved_4;

		/* == quadword 4 == */
		uint16_t	origin_rs;
		uint8_t		flags;
		uint8_t		unused[3];
		uint16_t	niov;			/* number of non-contiguous buffers */

		/* == quadword 5 == */
		uint64_t	message_length;		/* total length in bytes of all non-contiguous buffers and immediate data */

		/* == quadword 6 == */
		uint64_t	reserved_6;

	} __attribute__((__packed__)) rendezvous;


	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		origin_rx;
		uint8_t		reserved_2;

		/* == quadword 2 == */
		uint64_t	reserved_3;

		/* == quadword 3 == */
		uint64_t	reserved_4;

		union {
			uint8_t	opcode;
			struct {
				/* == quadword 4 == */
				uint8_t		opcode;
				uint8_t		unused0;
				uint16_t	unused1;
				uint16_t	ntidpairs;	/* number of tidpairs described in the packet payload */
				uint16_t	niov;		/* number of non-contiguous buffers described in the packet payload */

				/* == quadword 5,6 == */
				uintptr_t	origin_byte_counter_vaddr;
				uintptr_t	target_context_vaddr;
			} vaddr;
			struct {
				/* == quadword 4 == */
				uint8_t		opcode;
				uint8_t		unused0;
				uint16_t	unused1;
				uint8_t		dt;
				uint8_t		op;
				uint16_t	niov;		/* number of non-contiguous buffers described in the packet payload */

				/* == quadword 5,6 == */
				uintptr_t	rma_request_vaddr;
				uint64_t	key;
			} mr;
			struct {
				/* == quadword 4 == */
				uint8_t		opcode;
				uint8_t		unused0;
				uint16_t	unused1;
				uint8_t		unused2;
				uint8_t		unused3;
				uint16_t	unused4;	/* number of non-contiguous buffers described in the packet payload */

				/* == quadword 5,6 == */
				uintptr_t	completion_counter;
				uint64_t	bytes_to_fence;
			} fence;
		} target;

	} __attribute__((__packed__)) cts;

	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		origin_rx;
		uint8_t		reserved_o2;

		/* == quadword 2 == */
		uint64_t	reserved_3;

		/* == quadword 3 == */
		uint64_t	reserved_4;

		union {
			/* == quadword 4 == */
			/*  Common fields   */
			struct {
				uint8_t		opcode;
				uint8_t		origin_tx;
				uint8_t		dt;
				uint8_t		op;
				uint16_t	last_bytes;
				uint16_t	bytes;

				uint64_t	reserved[2]; /* op-specific */
			};

			struct {
				/* == quadword 4 == */
				uint64_t	reserved; /* Common fields */

				/* == quadword 5 == */
				uintptr_t	rma_request_vaddr;
				/* == quadword 6 == */
				uintptr_t	rbuf;
			} get;

			struct {
				/* == quadword 4 == */
				uint64_t	reserved; /* Common fields */

				/* == quadword 5 == */
				uintptr_t	completion_vaddr; /* struct fi_opx_rzv_completion * */
				/* == quadword 6 == */
				uintptr_t	rbuf;
			} rzv;

			struct {
				/* == quadword 4 == */
				uint64_t	reserved; /* Common fields */

				/* == quadword 5,6 == */
				uintptr_t	key;
				uintptr_t	offset;
			} mr;

			struct {
				/* == quadword 4 == */
				uint64_t	reserved; /* Common fields */

				/* == quadword 5,6 == */
				uintptr_t	completion_counter;
				uint64_t	bytes_to_fence;
			} fence;
		} target;

	} __attribute__((__packed__)) dput;



	struct {
		/* == quadword 0 == */
		uint64_t	reserved_0;

		/* == quadword 1 == */
		uint16_t	reserved_1[3];
		uint8_t		opcode;
		uint8_t		reserved_2;

		/* == quadword 2,3,4,5,6 == */
		uint64_t	reserved_n[5];

	} __attribute__((__packed__)) ud;

	struct {
		/* == quadword 0 == */
		uint16_t	reserved_0[3];
		uint16_t	slid;			/* stl.lrh.slid */

		/* == quadword 1 == */
		uint64_t	reserved_1;

		/* == quadword 2 == */
		uint32_t	range_count;		/* stl.bth.psn */
		uint8_t		origin_reliability_rx;	/* stl.kdeth.offset */
		uint8_t		reserved_2[3];

		/* == quadword 3 == */
		uint32_t	reserved_3;
		uint32_t	unused;

		/* == quadword 4,5,6 == */
		uint64_t	psn_count;
		uint64_t	psn_start;
		uint64_t	key;			/* fi_opx_reliability_service_flow_key */

	} __attribute__((__packed__)) service;		/* "reliability service" */
} __attribute__((__aligned__(8)));

static_assert(((offsetof(union fi_opx_hfi1_packet_hdr, rendezvous.flags) % 8) * 8) == FI_OPX_PKT_RZV_FLAGS_SHIFT,
		"struct fi_opx_hfi1_packet_hdr.rendezvous.flags offset inconsistent with FLAGS_SHIFT!");

static inline
fi_opx_uid_t fi_opx_hfi1_packet_hdr_uid (const union fi_opx_hfi1_packet_hdr * const hdr) {

	const union fi_opx_uid uid =
	{
		.endpoint_id = hdr->reliability.origin_tx,	/* node-scoped endpoint id */
		.lid = hdr->match.slid			/* job-scoped node id */
	};

	return uid.fi;
}


static inline size_t
fi_opx_hfi1_packet_hdr_message_length (const union fi_opx_hfi1_packet_hdr * const hdr)
{
	size_t message_length = 0;
	switch (hdr->stl.bth.opcode) {
		case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT:
		case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT:
			message_length = hdr->inject.message_length;
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER:
		case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER:
			message_length = hdr->send.xfer_bytes_tail + hdr->send.payload_qws_total * sizeof(uint64_t);
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST:
		case FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST:
			message_length = hdr->mp_eager_first.payload_bytes_total & FI_OPX_HFI1_KDETH_VERSION_OFF_MASK;
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS:
		case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS:
			//assert(hdr->rendezvous.niov == 1);
			message_length = hdr->rendezvous.message_length;
			break;
		default:
			fprintf(stderr, "%s:%s():%d abort. hdr->stl.bth.opcode = %02x (%u)\n", __FILE__, __func__, __LINE__, hdr->stl.bth.opcode, hdr->stl.bth.opcode); abort();
			break;
	}

	return message_length;
}

static inline
void fi_opx_hfi1_dump_packet_hdr (const union fi_opx_hfi1_packet_hdr * const hdr,
		const char * fn, const unsigned ln) {

	const uint64_t * const qw = (uint64_t *)hdr;
	const pid_t pid = getpid();

	fprintf(stderr, "(%d) %s():%u ==== dump packet header @ %p [%016lx %016lx %016lx %016lx]\n", pid, fn, ln, hdr, qw[0], qw[1], qw[2], qw[3]);
	fprintf(stderr, "(%d) %s():%u .stl.lrh.flags ...........     0x%04hx\n", pid, fn, ln, hdr->stl.lrh.flags);
	fprintf(stderr, "(%d) %s():%u .stl.lrh.dlid ............     0x%04hx (be: %5hu, le: %5hu)\n", pid, fn, ln, hdr->stl.lrh.dlid, hdr->stl.lrh.dlid, ntohs(hdr->stl.lrh.dlid));
	fprintf(stderr, "(%d) %s():%u .stl.lrh.pktlen ..........     0x%04hx (be: %5hu, le: %5hu)\n", pid, fn, ln, hdr->stl.lrh.pktlen, hdr->stl.lrh.pktlen, ntohs(hdr->stl.lrh.pktlen));
	fprintf(stderr, "(%d) %s():%u .stl.lrh.slid ............     0x%04hx (be: %5hu, le: %5hu)\n", pid, fn, ln, hdr->stl.lrh.slid, hdr->stl.lrh.slid, ntohs(hdr->stl.lrh.slid));
	fprintf(stderr, "(%d) %s():%u\n", pid, fn, ln);
	fprintf(stderr, "(%d) %s():%u .stl.bth.opcode ..........     0x%02x \n", pid, fn, ln, hdr->stl.bth.opcode);

	fprintf(stderr, "(%d) %s():%u .match.slid ..............     0x%04x \n", pid, fn, ln, hdr->match.slid);
	fprintf(stderr, "(%d) %s():%u .match.origin_tx .........     0x%02x \n", pid, fn, ln, hdr->match.origin_tx);
	fprintf(stderr, "(%d) %s():%u .match.ofi_data ..........     0x%08x \n", pid, fn, ln, hdr->match.ofi_data);
	fprintf(stderr, "(%d) %s():%u .match.ofi_tag ...........     0x%016lx \n", pid, fn, ln, hdr->match.ofi_tag);

	switch (hdr->stl.bth.opcode) {
		case FI_OPX_HFI_BTH_OPCODE_MSG_INJECT:
		case FI_OPX_HFI_BTH_OPCODE_TAG_INJECT:
			fprintf(stderr, "(%d) %s():%u .inject.message_length ...     0x%02x \n", pid, fn, ln, hdr->inject.message_length);
			fprintf(stderr, "(%d) %s():%u .inject.app_data_u64[0] ..     0x%016lx \n", pid, fn, ln, hdr->inject.app_data_u64[0]);
			fprintf(stderr, "(%d) %s():%u .inject.app_data_u64[1] ..     0x%016lx \n", pid, fn, ln, hdr->inject.app_data_u64[1]);
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_EAGER:
		case FI_OPX_HFI_BTH_OPCODE_TAG_EAGER:
			fprintf(stderr, "(%d) %s():%u .send.xfer_bytes_tail ....     0x%02x \n", pid, fn, ln, hdr->send.xfer_bytes_tail);
			fprintf(stderr, "(%d) %s():%u .send.payload_qws_total ..     0x%04x \n", pid, fn, ln, hdr->send.payload_qws_total);
			fprintf(stderr, "(%d) %s():%u .send.xfer_tail ..........     0x%016lx \n", pid, fn, ln, hdr->send.xfer_tail);
			break;
		case FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS:
		case FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS:	/* calculate (?) total bytes to be transfered */
			//break;
		default:
			fprintf(stderr, "(%d) %s():%u ==== QWs 4-7 : [%016lx %016lx %016lx %016lx]\n", pid, fn, ln, qw[4], qw[5], qw[6], qw[7]);
			break;
	}

	return;
}

union cacheline {
	uint64_t			qw[8];
	uint32_t			dw[16];
	uint8_t				byte[64];
};

union fi_opx_hfi1_dput_iov {
	uint64_t	qw[6];
	struct {
		uintptr_t			rbuf;
		uintptr_t			sbuf;
		uint64_t			bytes;
		uint64_t			rbuf_device;
		uint64_t			sbuf_device;
		enum fi_hmem_iface		rbuf_iface;
		enum fi_hmem_iface		sbuf_iface;
	};
};

struct fi_opx_hfi1_dput_fetch {
	uintptr_t			fetch_rbuf;
	uintptr_t			rma_request_vaddr;
};

union fi_opx_hfi1_dput_rbuf {
	uintptr_t ptr;
	uint32_t dw[2];
};

struct fi_opx_hmem_iov {
	uintptr_t buf;
	uint64_t len;
	uint64_t device;
	enum fi_hmem_iface iface;
} __attribute__((__packed__));

#define FI_OPX_MAX_HMEM_IOV ((FI_OPX_HFI1_PACKET_MTU - sizeof(uintptr_t)) / sizeof(struct fi_opx_hmem_iov))
#define FI_OPX_MAX_DPUT_IOV ((FI_OPX_HFI1_PACKET_MTU / sizeof(union fi_opx_hfi1_dput_iov) - 4) + 3)

#define FI_OPX_MAX_DPUT_TIDPAIRS ((FI_OPX_HFI1_PACKET_MTU - sizeof(union fi_opx_hfi1_dput_iov) - (2 * sizeof(uint32_t)))/sizeof(uint32_t))

union fi_opx_hfi1_rzv_rts_immediate_info {
	uint64_t	qw0;
	struct {
		uint8_t	byte_count;	/* only need 3 bits (0..7 bytes) */
		uint8_t	qw_count;	/* only need 3 bits (0..7 quadwords) */
		uint8_t	block_count;	/* only need 1 bits (0 or 1) */
		uint8_t	end_block_count;/* only need 1 bits (0 or 1) */
		uint32_t unused;
	};
};

union fi_opx_hfi1_packet_payload {
	uint8_t				byte[FI_OPX_HFI1_PACKET_MTU];
	union {
		struct {
			/* ==== CACHE LINE 0 ==== */

			uintptr_t		src_vaddr;
			uint64_t		src_blocks;		/* number of 64-byte data blocks to transfer */
			uint64_t		src_device_id;
			uint64_t		src_iface;
			uint64_t		immediate_info;
			uintptr_t		origin_byte_counter_vaddr;
			uint64_t		unused[2];

			/* ==== CACHE LINE 1 ==== */
			union {
				struct {
					uint8_t		immediate_byte[8];
					uint64_t	immediate_qw[7];
				};
				
				union cacheline	cache_line_1;
			};

			/* ==== CACHE LINE 2-127 ==== */

			union cacheline	immediate_block[FI_OPX_HFI1_PACKET_MTU / sizeof(union cacheline) - 2];

		} contiguous;
		struct {
			/* ==== CACHE LINE 0 ==== */

			uintptr_t		origin_byte_counter_vaddr;
			struct fi_opx_hmem_iov	iov[2];

			/* ==== CACHE LINE 1-127 (for 8k mtu) ==== */
			struct fi_opx_hmem_iov	iov_ext[FI_OPX_MAX_HMEM_IOV - 2];
			size_t			unused;

		} noncontiguous;
	} rendezvous;

	struct {
		union fi_opx_hfi1_dput_iov	iov[FI_OPX_MAX_DPUT_IOV];
	} cts;

	/* tid_cts extends cts*/
	struct {
		union fi_opx_hfi1_dput_iov	iov[1];
		uint32_t  tid_offset;
		uint32_t  ntidpairs;
		uint32_t  tidpairs[FI_OPX_MAX_DPUT_TIDPAIRS];
	} tid_cts;

} __attribute__((__aligned__(32)));

static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.contiguous.immediate_byte) == 64,
		"struct fi_opx_hfi1_packet_payload.rendezvous.contiguous.immediate_byte should be aligned on cacheline 1!");
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.contiguous.immediate_block) == 128,
		"struct fi_opx_hfi1_packet_payload.rendezvous.contiguous.immediate_block should be aligned on cacheline 2!");
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.noncontiguous.iov) == 8,
		"struct fi_opx_hfi1_packet_payload.rendezvous.noncontiguous.iov should be 8 bytes into cacheline 0!");
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.noncontiguous.iov_ext) == 64,
		"struct fi_opx_hfi1_packet_payload.rendezvous.noncontiguous.iov_ext should be aligned on cacheline 1!");
static_assert(offsetof(union fi_opx_hfi1_packet_payload, rendezvous.noncontiguous.unused) == (FI_OPX_HFI1_PACKET_MTU - 8),
		"struct fi_opx_hfi1_packet_payload.rendezvous.noncontiguous.unused should end at packet MTU!");



struct fi_opx_hfi1_ue_packet_slist;
struct fi_opx_hfi1_ue_packet {
	/* == CACHE LINE 0 == */
	struct fi_opx_hfi1_ue_packet			*next;
	struct fi_opx_hfi1_ue_packet			*prev;

	struct {
		struct fi_opx_hfi1_ue_packet		*next;
		struct fi_opx_hfi1_ue_packet		*prev;
		struct fi_opx_hfi1_ue_packet_slist 	*ht;
	} tag_ht;

	struct {
		uint32_t				rank;
		uint32_t				rank_inst;
	} daos_info;

	/* Copies of tag and origin_uid_fi so that
	 * packet can be matched only accessing the
	 * first cacheline */
	uint64_t					tag;
	fi_opx_uid_t					origin_uid_fi;

	uint32_t					unused_cacheline0;

	/* == CACHE LINE 1 == */
	uint64_t					unused_cacheline1;
	union fi_opx_hfi1_packet_hdr		hdr;

	/* == CACHE LINE 2 == */
	union fi_opx_hfi1_packet_payload	payload;
} __attribute__((__packed__)) __attribute__((aligned(64)));

static_assert(offsetof(struct fi_opx_hfi1_ue_packet, unused_cacheline1) == 64,
		"struct fi_opx_hfi1_ue_packet->unused_cacheline1 should be aligned on cache boundary!");
static_assert(offsetof(struct fi_opx_hfi1_ue_packet, payload) == 128,
		"struct fi_opx_hfi1_ue_packet->payload should be aligned on cache boundary!");

struct fi_opx_hfi1_ue_packet_slist {
	struct fi_opx_hfi1_ue_packet *	head;
	struct fi_opx_hfi1_ue_packet *	tail;
	uint64_t			length;
};

static inline void fi_opx_hfi1_ue_packet_slist_init (struct fi_opx_hfi1_ue_packet_slist* list)
{
	list->head = list->tail = NULL;
	list->length = 0;
}

static inline int fi_opx_hfi1_ue_packet_slist_empty (struct fi_opx_hfi1_ue_packet_slist* list)
{
	return !list->head;
}

static inline void fi_opx_hfi1_ue_packet_slist_insert_tail (struct fi_opx_hfi1_ue_packet *item,
		struct fi_opx_hfi1_ue_packet_slist* list)
{
	assert(item->next == NULL);
	if (fi_opx_hfi1_ue_packet_slist_empty(list)) {
		assert(list->length == 0);
		item->prev = NULL;
		list->head = item;
	} else {
		assert(list->length > 0);
		item->prev = list->tail;
		list->tail->next = item;
	}
	++list->length;

	list->tail = item;
}

/*
 * Remove item from the list, but don't free it.
 * Return the removed item.
 */
static inline
struct fi_opx_hfi1_ue_packet *fi_opx_hfi1_ue_packet_slist_pop_item (struct fi_opx_hfi1_ue_packet *item,
								    struct fi_opx_hfi1_ue_packet_slist *list)
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
static inline
struct fi_opx_hfi1_ue_packet *fi_opx_hfi1_ue_packet_slist_remove_item (struct fi_opx_hfi1_ue_packet *item,
								       struct fi_opx_hfi1_ue_packet_slist *list)
{
	struct fi_opx_hfi1_ue_packet *next_item = item->next;
	struct fi_opx_hfi1_ue_packet *prev = item->prev;
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

#endif /* _FI_PROV_OPX_HFI1_PACKET_H_ */
