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

  Copyright(c) 2016 Intel Corporation.

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

#ifndef _IPS_PROTO_PARAMS_H
#define _IPS_PROTO_PARAMS_H

/*
 * send method: dma, pio;
 * recv method: tid, egr;
 *
 * send-recv mode combinations: 1=on, 0=off
 * A: dma:1, pio=1, tid=1, egr=1;
 * B: dma:0, pio=1, tid=1, egr=1;
 * C: dma:1, pio=0, tid=1, egr=1;
 * D: dma:1, pio=1, tid=0, egr=1;
 * E: dma:0, pio=1, tid=0, egr=1;
 * F: dma:1, pio=0, tid=0, egr=1;
 *
 * message packet type:
 * T: tiny; S: short; E: eager;
 * LR: long rts; LC: long cts; LD: long data;
 * ED: expected data; EC: expected completion;
 * C: ctrl msg;
 *
 * send,recv method for each packet type and each send-recv mode
 * -------------------------------------------------------------------
 * |    |  A       | B       | C       | D       | E       | F       |
 * -------------------------------------------------------------------
 * | T  |  pio,egr | pio,egr | dma,egr | pio,egr | pio,egr | dma,egr |
 * -------------------------------------------------------------------
 * | S  |  pio,egr | pio,egr | dma,egr | pio,egr | pio,egr | dma,egr |
 * -------------------------------------------------------------------
 * | E  |  pio,egr | pio,egr | dma,egr | pio,egr | pio,egr | dma,egr |<threshold
 * -------------------------------------------------------------------
 * | E  |  dma,egr | pio,egr | dma,egr | dma,egr | pio,egr | dma,egr |>threshold
 * -------------------------------------------------------------------
 * | LR |  pio,egr | pio,egr | dma,egr | pio,egr | pio,egr | dma,egr |
 * -------------------------------------------------------------------
 * | LC |  pio,egr | pio,egr | dma,egr | pio,egr | pio,egr | dma,egr |
 * -------------------------------------------------------------------
 * | LD |  x       | x       | x       | pio,egr | pio,egr | dma,egr |<threshold
 * -------------------------------------------------------------------
 * | LD |  x       | x       | x       | dma,egr | pio,egr | dma,egr |>threshold
 * -------------------------------------------------------------------
 * | ED |  dma,tid | pio,tid | dma,tid | x       | x       | x       |
 * -------------------------------------------------------------------
 * | EC |  pio,egr | pio,egr | dma,egr | x       | x       | x       |
 * -------------------------------------------------------------------
 * | C  |  pio,egr | pio,egr | dma,egr | pio,egr | pio,egr | dma,egr |
 * -------------------------------------------------------------------
 */

/* Constants */
#define BYTE2DWORD_SHIFT 2
#define LOWER_16_BITS 0xFFFF
#define PSM_CACHE_LINE_BYTES 64
#define PSM2_FLOW_CREDITS 64
#define PSM_CRC_SIZE_IN_BYTES 8

/*
 * version of protocol header (known to chip also).
 * This value for OPA is defined in spec.
 */
#define IPS_PROTO_VERSION 0x1

/* time conversion macros */
#define us_2_cycles(us) nanosecs_to_cycles(1000ULL*(us))
#define ms_2_cycles(ms)  nanosecs_to_cycles(1000000ULL*(ms))
#define sec_2_cycles(sec) nanosecs_to_cycles(1000000000ULL*(sec))

/* Per-flow flags */
#define IPS_FLOW_FLAG_NAK_SEND	    0x01
#define IPS_FLOW_FLAG_PENDING_ACK   0x02
#define IPS_FLOW_FLAG_PENDING_NAK   0x04
#define IPS_FLOW_FLAG_GEN_BECN      0x08
#define IPS_FLOW_FLAG_SKIP_CTS      0x20

/* tid session expected send flags  */
#define EXP_SEND_FLAG_CLEAR_ALL 0x00
#define EXP_SEND_FLAG_FREE_TIDS 0x01

#define TIMEOUT_INFINITE 0xFFFFFFFFFFFFFFFFULL	/* 64 bit all-one's  */

/*
 * scb flags for wire,
 * Only the lower 6 bits are wire-protocol options
 */
#define IPS_SEND_FLAG_NONE              0x00
#define IPS_SEND_FLAG_BLOCKING		0x01	/* blocking send */
#define IPS_SEND_FLAG_PKTCKSUM          0x02	/* Has packet checksum */
#define IPS_SEND_FLAG_AMISTINY		0x04	/* AM is tiny, exclusive */

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
/* This flag is used to indicate to the reciever when
 * the send is issued on a device buffer. This helps in
 * selecting TID path on the recieve side regardless of
 * the receive buffers locality. It is used
 * in a special case where the send is on a device
 * buffer and the receive is on a host buffer.
 */
#define IPS_SEND_FLAG_USER_BUF_GPU      0x08
#endif

#define IPS_SEND_FLAG_PROTO_OPTS        0x3f	/* only 6bits wire flags */

/* scb flags */
#define IPS_SEND_FLAG_PENDING		0x0100
#define IPS_SEND_FLAG_PERSISTENT	0x0200
#define IPS_SEND_FLAG_NO_LMC		0x0400

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
/* This flag is used to indicate if the send is on
 * a GPU buffer. This helps PIO/SDMA paths to detect
 * if payload is GPU buffer without having to call
 * cudaGetPointerAttribute.
 */
#define IPS_SEND_FLAG_PAYLOAD_BUF_GPU   0x0800
#endif

#ifdef PSM_HAVE_REG_MR
#define IPS_SEND_FLAG_SEND_MR          0x1000
#endif

#ifdef PSM_SOCKETS
#define IPS_SEND_FLAG_TCP_REMAINDER	0x2000	/* TCP data was partially sent out */
#endif

#ifdef PSM_ONEAPI
#define IPS_SEND_FLAG_USE_GDRCOPY	0x4000
#endif

/* 0x10000000, interrupt when done */
#define IPS_SEND_FLAG_INTR		(1<<HFI_KHDR_INTR_SHIFT)


/* 0x80000000, request ack (normal) */
#define IPS_SEND_FLAG_ACKREQ		(1<<HFI_BTH_ACK_SHIFT)

/* proto flags */
#define IPS_PROTO_FLAG_SPIO		0x02	/* all spio, no dma */
#define IPS_PROTO_FLAG_RCVTHREAD	0x04	/* psm recv thread is on */
#define IPS_PROTO_FLAG_LOOPBACK		0x08	/* psm loopback over hfi */
#define IPS_PROTO_FLAG_CKSUM            0x10	/* psm checksum is on */

/* Coalesced ACKs (On by default) */
#define IPS_PROTO_FLAG_COALESCE_ACKS    0x20

/* Use Path Record query (off by default) */
#define IPS_PROTO_FLAG_QUERY_PATH_REC   0x40

/* Path selection policies:
 *
 * (a) Adaptive - Dynamically determine the least loaded paths using various
 * feedback mechanism - Completion time via ACKs, NAKs, CCA using BECNs.
 *
 * (b) Static schemes  -
 *     (i) static_src  - Use path keyed off source context
 *    (ii) static_dest - Use path keyed off destination context
 *   (iii) static_base - Use only the base lid path - default till Oct'09.
 *
 * The default is adaptive. If a zero lmc network is used then there exists
 * just one path between endpoints the (b)(iii) case above.
 *
 */

#define IPS_PROTO_FLAG_PPOLICY_ADAPTIVE 0x200
#define IPS_PROTO_FLAG_PPOLICY_STATIC_SRC 0x400
#define IPS_PROTO_FLAG_PPOLICY_STATIC_DST 0x800
#define IPS_PROTO_FLAG_PPOLICY_STATIC_BASE 0x1000

/* All static policies */
#define IPS_PROTO_FLAG_PPOLICY_STATIC 0x1c00


#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
/* Use RNDV (TID) for all message sizes */
//#define IPS_PROTO_FLAG_ALWAYS_RNDV		0x10000	// unused
/* Use GPUDirect RDMA for SDMA */
#define IPS_PROTO_FLAG_GPUDIRECT_RDMA_SEND	0x20000
/* Use GPUDirect RDMA for TID */
#define IPS_PROTO_FLAG_GPUDIRECT_RDMA_RECV	0x40000
#endif

// These flags (8 bits) select the TID/RDMA expected protocol options.
// Most are single bit flags except for the low 2 bits (RDMA_MASK)
// which select one of 4 possible HAL specific modes (including 0 == disable).
// Proper use of the FLAG_ENABLED is (flags & FLAG_ENABLED) which
// will be true if any of the 3 RDMA modes are selected.
// For some HALs (verbs), the low 2 bits become ep->wiremode and are shared in
// the connection establishment protocol so if they change meaning, the
// ips_proto_connect.c protocol and version must be reviewed and updated
// accordingly
#define IPS_PROTOEXP_FLAG_ENABLED	     0x03    /* any of the 3 modes */
/* up to 3 HAL specific modes, 0 disables RDMA */
#define IPS_PROTOEXP_FLAG_RDMA_MASK	     0x03

#ifdef PSM_VERBS
// verbs specific RDMA modes
#define IPS_PROTOEXP_FLAG_RDMA_KERNEL        0x01    /* kernel RV module RDMA */
#define IPS_PROTOEXP_FLAG_RDMA_USER          0x02    /* user RC QP for RDMA only */
#define IPS_PROTOEXP_FLAG_RDMA_USER_RC       0x03    /* user RC QP eager & RDMA */
#define IPS_PROTOEXP_FLAG_USER_RC_QP(flag) ((flag)&0x02) /* either RC QP mode */
#define IPS_PROTOEXP_FLAG_KERNEL_QP(flag) \
		(((flag)&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_KERNEL)
#endif

// These are additional flags only applicable when RDMA is enabled.
// If needed their values could be changed to permit additional bits for
// RDMA modes.
// PSM3_RTS_CTS_INTERLEAVE set's it's flag and TID_DEBUG and CTS_SERIALIZED
// are automatically when when appropriate.  None go on the wire.
#define IPS_PROTOEXP_FLAG_RTS_CTS_INTERLEAVE 0x08    /* Interleave RTS handling */
#define IPS_PROTOEXP_FLAG_CTS_SERIALIZED     0x10    /* CTS serialized */

#endif /* _IPS_PROTO_PARAMS_H */
