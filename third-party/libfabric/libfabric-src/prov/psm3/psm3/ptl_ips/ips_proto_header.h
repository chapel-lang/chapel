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

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef _IPS_PROTO_HEADER_H
#define _IPS_PROTO_HEADER_H

/* Although defined as macros, the *_BITS values below are NOT meant to be
   changed.  They are defined this way so that their values are written in
   exactly one place.  These macros are used in struct ips_message_header
   below, as well as in the active messages code for the purpose of
   establishing how many arguments/handlers are supported, and to assert that
   values written into the header fields are not too large for the number of
   bits available. The preprocessor check below ensures less than 32 bits are
   used.
 */

/* Number of bits to use for the amhdr_len field. */
#define IPS_AM_HDR_LEN_BITS 4

/* Number of bits to use for the amhdr_hidx field.  Bounds the number of
 * handlers supported (1 << IPS_AM_HDR_HIDX_BITS). */
#define IPS_AM_HDR_HIDX_BITS 8

/* Number of bits to use for the amhdr_nargs field.  Bounds the number of
   arguments supported (1 << IPS_AM_HDR_NARGS_BITS). */
#define IPS_AM_HDR_NARGS_BITS 4

#if (IPS_AM_HDR_LEN_BITS + IPS_AM_HDR_HIDX_BITS + IPS_AM_HDR_NARGS_BITS) > 32
#error "Bad IPS header definition: AM fields must use 32 bits or less"
#endif

/* Number of AM arguments that can be packets into struct_ips_message_header.
   Remaining arguments up to the max (1 << IPS_AM_HDR_NARGS_BITS) are placed in
   the data payload. */
#define IPS_AM_HDR_NARGS  \
	(sizeof(((struct ips_message_header *)0)->data) / sizeof(psm2_amarg_t))

/* The actual size of the message header is determined by three paramters:
 * IPS_HEADER_QUEUE_IWORDS (fixed at 5 by hardware)
 *    OPA words contain LRH and BTH
 * IPS_HEADER_QUEUE_HWORDS (fixed at 2 by ips protocol)
 *    IPS hardware words contain ips-protocol-specific data
 * IPS_HEADER_QUEUE_UWORDS (fixed at 7 by ips protocol)
 *    IPS user words contain ips-protocol-specific data
 *
 * The header message size is determined to as IWORDS + HWORDS + UWORDS
 */
struct ips_message_header {
	__be16 lrh[4];
	__be32 bth[3];

	// 32b alignment
	struct hfi_kdeth khdr;		/* little endian */

	/* fields below this point are in host byte order */
	// 32b alignment
	struct {
		__u32 flags:6;
		__u32 connidx:26;	/* connection idx */
	} PACK_SUFFIX;

	// 64b alignment
	union {
		struct {
			struct {
				// next psn sender expects to recv
				// acks all packets up to ack_seq_num-1
				__u32 ack_seq_num:31;
				__u32 reserved:1;
			} PACK_SUFFIX;

			// 32b alignment
			union {
				struct {	/* for active message */
					__u32 amhdr_len:IPS_AM_HDR_LEN_BITS;
					__u32 amhdr_nargs:IPS_AM_HDR_NARGS_BITS;
					__u32 amhdr_hidx:IPS_AM_HDR_HIDX_BITS;
				} PACK_SUFFIX;
				__u32 mdata;	/* for misc data */
			};

			// 64b alignment
			/* Inline arguments and/or message payload  */
			union {
				ptl_arg_t data[2];
				__u32 uwords[4];
			};
		} PACK_SUFFIX;

		/* for message header packet only */
		struct {
			__u32 pad1;
			__u32 tag[3];	/* 96 bits psm tag */
			ptl_arg_t hdr_data;
		} PACK_SUFFIX;

	};
} PACK_SUFFIX;
/* desc_genc is up to 32 bits, RDMA immediate data
 * only has room for 16 bits
 */
#define IPS_HDR_RDESCID_GENC_MASK 0xffff

/*
 * OpCodes in BTH[0], 24-31 bits. Order is important!!!
 */
#define OPCODE_RESERVED			0xC0	/* reserved */
/* TINY to ERR_CHK_RDMA_RESP are level 2 packets */
/* sending queue keeps a copy and resends if timeout waiting for ack */
/* order and reliability maintained */
#define OPCODE_TINY			0xC1	/* 0 <= msglen <= 8 */
#define OPCODE_SHORT			0xC2	/* 8 < msglen <= MTU */
#define OPCODE_EAGER			0xC3	/* eager packet */
#define OPCODE_LONG_RTS			0xC4	/* ready to send */
#define OPCODE_LONG_CTS			0xC5	/* confirm to send */
#define OPCODE_LONG_DATA		0xC6	/* long data packets */
#define OPCODE_ERR_CHK_RDMA		0xC7	/* RDMA error recovery */
#define OPCODE_ERR_CHK_RDMA_RESP 0xC8	/* RDMA error recovery response */
/* ACK to ERR_CHK_GEN are "level 0 control packets" state machine driven send */
/* reissue if given state persists */
/* duplicates can occur with no consequences */
#define OPCODE_ACK			0xC9	/* explicit ACK packet */
#define OPCODE_NAK			0xCA	/* explicit NAK packet */
#define OPCODE_BECN			0xCB	/* congestion control */
#define OPCODE_ERR_CHK			0xCC	/* query eager receiving */
//					0xCD	/* reserved */
/* CONNECT_REQUEST to DISCONNECT_REPLY are "level 1 control packets" */
/* timer based resend, but rebuild on fly when resend */
/* consumer must deal with duplicates */
#define OPCODE_CONNECT_REQUEST		0xCE	/* connect request */
#define OPCODE_CONNECT_REPLY		0xCF	/* connect reply */
#define OPCODE_DISCONNECT_REQUEST	0xD0	/* disconnect request */
#define OPCODE_DISCONNECT_REPLY		0xD1	/* disconnect reply */
/* AM_REQUEST_NOREPLY to AM_REPLY are level 2 packets */
/* sending queue keeps a copy and resends if timeout waiting for ack */
/* order and reliability maintained */
#define OPCODE_AM_REQUEST_NOREPLY	0xD2	/* AM request w/o reply */
#define OPCODE_AM_REQUEST		0xD3	/* AM request */
#define OPCODE_AM_REPLY			0xD4	/* AM reply */
#define OPCODE_FUTURE_FROM		0xD5	/* reserved for expansion */
#define OPCODE_FUTURE_TO		0xDF	/* reserved for expansion */

#endif /* _IPS_PROTO_HEADER_H */
