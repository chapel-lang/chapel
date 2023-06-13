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

#ifndef _IPS_SCB_H
#define _IPS_SCB_H

#include "psm2_mock_testing.h"
#include "psm_user.h"
#include "ips_proto_header.h"

/* ips_alloc_scb flags */
#define IPS_SCB_FLAG_NONE	0x0
#define IPS_SCB_FLAG_ADD_BUFFER 0x1

/* macros to update scb */
// TBD - these accessor macros provide limited value, just use the raw fields
// as we already must do for a number of other scb fields
#define ips_scb_opcode(scb)    scb->opcode
#define ips_scb_buffer(scb)    scb->payload
#define ips_scb_length(scb)    scb->payload_size
#define ips_scb_flags(scb)     scb->scb_flags
#define ips_scb_dma_cntr(scb)  scb->dma_cntr
#define ips_scb_epaddr(scb)    scb->epaddr
#define ips_scb_cb(scb)        scb->callback
#define ips_scb_cb_param(scb)  scb->cb_param

#define ips_scb_copy_tag(dst, src)			\
				(dst)[0] = (src)[0];	\
				(dst)[1] = (src)[1];	\
				(dst)[2] = (src)[2];

struct ips_scbbuf;
struct ips_scb;
struct ips_scbctrl;
struct ips_tid_send_desc;

typedef void (*ips_scbctrl_avail_callback_fn_t) (struct ips_scbctrl *,
						 void *context);

STAILQ_HEAD(ips_scb_stailq, ips_scb);
SLIST_HEAD(ips_scb_slist, ips_scb);

struct ips_scbctrl {
	/* Send control blocks for each send */
	uint32_t scb_num;
	uint32_t scb_num_cur;
	 SLIST_HEAD(scb_free, ips_scb) scb_free;
	void *scb_base;
	ips_scbctrl_avail_callback_fn_t scb_avail_callback;
	void *scb_avail_context;

	/* Immediate data for send buffers */
	uint32_t scb_imm_size;
	void *scb_imm_buf;
	psmi_timer *timers;	/* ack/send timers */

	/*
	 * Send buffers (or bounce buffers) to keep user data if we need to
	 * retransmit.
	 */
	uint32_t sbuf_num;
	uint32_t sbuf_num_cur;
	 SLIST_HEAD(sbuf_free, ips_scbbuf) sbuf_free;
	void *sbuf_buf_alloc;
	uint32_t sbuf_buf_size;
	void *sbuf_buf_base;
	void *sbuf_buf_last;
};

struct ips_scbbuf {
	SLIST_ENTRY(ips_scbbuf) next;
};

typedef struct ips_scb ips_scb_t;

struct ips_scb {
	union {
		SLIST_ENTRY(ips_scb) next;
		STAILQ_ENTRY(ips_scb) nextq;
	};
	/* for expediency, control packet SCBs are incompletely initialzed
	 * and only setup scb_flags, ips_lrh, nfrag, chunk_size and frag_size
	 * when FLAG_PKTCKSUM, chksum also valid
	 */
	union {
		void *payload;				// used for UD and UDP
		struct ips_scbbuf *sbuf;	// linkage for free scb's
	};
	uint64_t ack_timeout;	/* in cycles  */
	uint64_t abs_timeout;	/* in cycles  */

	psmi_timer *timer_send;	/* for sending packets */
	psmi_timer *timer_ack;	/* for acking packets */

	/* Used when composing packet */
	psmi_seqnum_t seq_num;	// psn of last packet to xmit as part of this scb
	uint32_t cksum[2]; /* only valid when FLAG_CKSUM */
	uint32_t scb_flags;
	/* When nfrag==1, frag_size and *remaining are undefined.
	 * An scb can describe a large user buffer (nfrag>1) for segmentation
	 * (UDP GSO and OPA send DMA).
	 * When such a buffer needs retransmission, the payload and payload_size
	 * will be advanced to reflect what needs to be retransmitted.
	 * *_remaining also are reduced to reflect what remains.
	 * However nfrag and chunk_size will still reflect their original values
	 * so we can identify that it was originally a large send (nfrag>1)
	 * and know how big it was (chunk_size) for req completion reporting.
	 * During such retransmission, when nfrag_remaining==1, ACKREQ is set.
	 */
	uint32_t payload_size;	/* remaining 1st packet payload size w/o hdr */
	uint32_t chunk_size;	/* total original buffer size */
	/* for nfrag>1, initially chunk_size_remaining = chunk_size. */
	uint32_t chunk_size_remaining; /* remaining buffer size to transmit */
	uint16_t nfrag;		/* total original packets in sequence */
	/* for nfrag>1, initially nfrag_remaining = nfrag */
	uint16_t nfrag_remaining; /* remaining packets to transmit */
	uint32_t frag_size;	/* max packet size in sequence */
#ifdef PSM_HAVE_SDMA
	uint16_t sdma_outstanding;
#endif
	uint16_t opcode;
#ifdef PSM_HAVE_REG_MR
	psm3_verbs_mr_t mr;
#endif
	struct ips_flow *flow;
	struct ips_tid_send_desc *tidsendc;

	struct ips_scbctrl *scbc;
	void *imm_payload;

	union {
		int (*callback) (void *, uint32_t);
		psm2_am_completion_fn_t completion_am;
	};
	void *cb_param;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	psm2_mq_req_t mq_req;		/* back pointer to original request */
#ifdef PSM_ONEAPI
	/* For munmap GDR buffer */
	unsigned long gdr_addr;
	size_t gdr_size;
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */
	struct {
		struct ips_message_header ips_lrh;
	} PSMI_CACHEALIGN;
};


#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define IS_TRANSFER_BUF_GPU_MEM(scb) (ips_scb_flags(scb) & IPS_SEND_FLAG_PAYLOAD_BUF_GPU)
#endif

void psm3_ips_scbctrl_free(ips_scb_t *scb);
int psm3_ips_scbctrl_bufalloc(ips_scb_t *scb);
int psm3_ips_scbctrl_avail(struct ips_scbctrl *scbc);
ips_scb_t *MOCKABLE(psm3_ips_scbctrl_alloc)(struct ips_scbctrl *scbc,
				int scbnum, int len, uint32_t flags);
MOCK_DCL_EPILOGUE(psm3_ips_scbctrl_alloc);
ips_scb_t *MOCKABLE(psm3_ips_scbctrl_alloc_tiny)(struct ips_scbctrl *scbc);
MOCK_DCL_EPILOGUE(psm3_ips_scbctrl_alloc_tiny);

psm2_error_t psm3_ips_scbctrl_init(psm2_ep_t ep,
			     uint32_t numscb, uint32_t numbufs,
			     uint32_t imm_size, uint32_t bufsize,
			     ips_scbctrl_avail_callback_fn_t,
			     void *avail_context, struct ips_scbctrl *);
psm2_error_t psm3_ips_scbctrl_fini(struct ips_scbctrl *);

psm2_error_t ips_scbctrl_writev(struct ips_scb_slist *slist, int fd);

#endif /* _IPS_SCB_H */
