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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

#ifndef PSMI_AM_H
#define PSMI_AM_H

#include "am_config.h"
#include "../psm_am_internal.h"

#define AMSH_DIRBLOCK_SIZE 128

#ifdef PSM_ONEAPI
/* sock_connected_state state definitions */
#define ZE_SOCK_NOT_CONNECTED			0
#define ZE_SOCK_DEV_FDS_SENT			1
#define ZE_SOCK_DEV_FDS_SENT_AND_RECD	2
#endif

typedef
struct am_epaddr {
	/*
	 * epaddr must be the first field to have the same address as this
	 * structure
	 */
	struct psm2_epaddr epaddr;

	uint16_t shmidx;
	uint16_t return_shmidx;
#ifdef PSM_ONEAPI
	int num_peer_fds;
	int peer_fds[MAX_ZE_DEVICES];
	int sock_connected_state;
	int sock;
#endif

	uint32_t cstate_outgoing:3;
	uint32_t cstate_incoming:3;
	uint32_t pid:22;
	/*
	 * Device number of GPU used by given EP, only used when CUDA is
	 * enabled. There is no gain from #ifdefing it out, since it does not
	 * use any extra space.
	 */
	uint32_t gpuid:4;
} am_epaddr_t;

/* Up to NSHORT_ARGS are supported via am_pkt_short_t; the remaining
   arguments are passed using space in am_pkt_bulk_t.  One additional argument
   is added for passing the internal ptl_am handler index. */
#define NSHORT_ARGS 6
#define NBULK_ARGS  (PSMI_AM_MAX_ARGS - NSHORT_ARGS + 1)

typedef
struct amsh_am_token {
	struct psmi_am_token tok;

	ptl_t *ptl;	  /**> What PTL was it received on */
	psm2_mq_t mq;	  /**> What matched queue is this for ? */
	uint16_t shmidx;  /**> what shmidx sent this */
} amsh_am_token_t;

typedef void (*psmi_handler_fn_t) (void *token, psm2_amarg_t *args, int nargs,
				   void *src, size_t len);

typedef struct psmi_handlertab {
	psmi_handler_fn_t fn;
} psmi_handlertab_t;

#define PSMI_AM_CONN_REQ    1
#define PSMI_AM_CONN_REP    2
#define PSMI_AM_DISC_REQ    3
#define PSMI_AM_DISC_REP    4

#define PSMI_KASSIST_OFF       0x0
#define PSMI_KASSIST_CMA_GET   0x1
#define PSMI_KASSIST_CMA_PUT   0x2

#define PSMI_KASSIST_CMA       0x3
#define PSMI_KASSIST_GET       0x1
#define PSMI_KASSIST_PUT       0x2
#define PSMI_KASSIST_MASK      0x3

int psm3_epaddr_pid(psm2_epaddr_t epaddr);

/*
 * Eventually, we will allow users to register handlers as "don't reply", which
 * may save on some of the buffering requirements
 */
#define PSMI_HANDLER_NEEDS_REPLY(handler)    1
#define PSMI_VALIDATE_REPLY(handler)    assert(PSMI_HANDLER_NEEDS_REPLY(handler))

int psm3_amsh_poll(ptl_t *ptl, int replyonly);

/* Shared memory AM, forward decls */
int
psm3_amsh_short_request(ptl_t *ptl, psm2_epaddr_t epaddr,
			psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			const void *src, size_t len, int flags);

void
psm3_amsh_short_reply(amsh_am_token_t *tok,
		      psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		      const void *src, size_t len, int flags);

int
psm3_amsh_long_request(ptl_t *ptl, psm2_epaddr_t epaddr,
		       psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		       const void *src, size_t len, void *dest, int flags);

void
psm3_amsh_long_reply(amsh_am_token_t *tok,
		     psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		     const void *src, size_t len, void *dest, int flags);

void psm3_am_mq_handler(void *toki, psm2_amarg_t *args, int narg, void *buf,
			size_t len);

void psm3_am_mq_handler(void *toki, psm2_amarg_t *args, int narg, void *buf,
			size_t len);
void psm3_am_mq_handler_data(void *toki, psm2_amarg_t *args, int narg,
			     void *buf, size_t len);
void psm3_am_mq_handler_complete(void *toki, psm2_amarg_t *args, int narg,
				 void *buf, size_t len);
void psm3_am_mq_handler_rtsmatch(void *toki, psm2_amarg_t *args, int narg,
				 void *buf, size_t len);
void psm3_am_mq_handler_rtsdone(void *toki, psm2_amarg_t *args, int narg,
				void *buf, size_t len);
void psm3_am_handler(void *toki, psm2_amarg_t *args, int narg, void *buf,
		     size_t len);

/* AM over shared memory (forward decls) */
psm2_error_t
psm3_amsh_am_get_parameters(psm2_ep_t ep, struct psm2_am_parameters *parameters);

psm2_error_t
psm3_amsh_am_short_request(psm2_epaddr_t epaddr,
			   psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			   void *src, size_t len, int flags,
			   psm2_am_completion_fn_t completion_fn,
			   void *completion_ctxt);

psm2_error_t
psm3_amsh_am_short_reply(psm2_am_token_t tok,
			 psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			 void *src, size_t len, int flags,
			 psm2_am_completion_fn_t completion_fn,
			 void *completion_ctxt);

#define amsh_conn_handler_hidx	 1
#define mq_handler_hidx          2
#define mq_handler_data_hidx     3
#define mq_handler_rtsmatch_hidx 4
#define mq_handler_rtsdone_hidx  5
#define am_handler_hidx          6

#define AMREQUEST_SHORT 0
#define AMREQUEST_LONG  1
#define AMREPLY_SHORT   2
#define AMREPLY_LONG    3
#define AM_IS_REPLY(x)     ((x)&0x2)
#define AM_IS_REQUEST(x)   (!AM_IS_REPLY(x))
#define AM_IS_LONG(x)      ((x)&0x1)
#define AM_IS_SHORT(x)     (!AM_IS_LONG(x))

#define AM_FLAG_SRC_ASYNC   0x1
#define AM_FLAG_SRC_TEMP    0x2

/*
 * Request Fifo.
 */
typedef
struct am_reqq {
	struct am_reqq *next;

	ptl_t *ptl;
	psm2_epaddr_t epaddr;
	int amtype;
	psm2_handler_t handler;
	psm2_amarg_t args[8];
	int nargs;
	uint32_t len;
	void *src;
	void *dest;
	int amflags;
	int flags;
} am_reqq_t;

struct am_reqq_fifo_t {
	am_reqq_t *first;
	am_reqq_t **lastp;
};

psm2_error_t psm3_am_reqq_drain(ptl_t *ptl);
void psm3_am_reqq_add(int amtype, ptl_t *ptl, psm2_epaddr_t epaddr,
		      psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		      void *src, size_t len, void *dest, int flags);

/*
 * Shared memory Active Messages, implementation derived from
 * Lumetta, Mainwaring, Culler.  Multi-Protocol Active Messages on a Cluster of
 * SMP's. Supercomputing 1997.
 *
 * We support multiple endpoints in shared memory, but we only support one
 * shared memory context with up to AMSH_MAX_LOCAL_PROCS local endpoints. Some
 * structures are endpoint specific (as denoted * with amsh_ep_) and others are
 * specific to the single shared memory context * (amsh_ global variables).
 *
 * Each endpoint maintains a shared request block and a shared reply block.
 * Each block is composed of queues for small, medium and large messages.
 */

#define QFREE      0
#define QUSED      1
#define QREADY     2
#define QREADYMED  3
#define QREADYLONG 4

#define QISEMPTY(flag) (flag < QREADY)
#if defined(__x86_64__) || defined(__i386__)
#  define _QMARK_FLAG_FENCE()  asm volatile("" : : : "memory")	/* compilerfence */
#else
#  error No _QMARK_FLAG_FENCE() defined for this platform
#endif

#define _QMARK_FLAG(pkt_ptr, _flag)		\
	do {					\
		_QMARK_FLAG_FENCE();		\
		(pkt_ptr)->flag = (_flag);	\
	} while (0)

#define QMARKFREE(pkt_ptr)  _QMARK_FLAG(pkt_ptr, QFREE)
#define QMARKREADY(pkt_ptr) _QMARK_FLAG(pkt_ptr, QREADY)
#define QMARKUSED(pkt_ptr)  _QMARK_FLAG(pkt_ptr, QUSED)

#define AMFMT_SYSTEM       1
#define AMFMT_SHORT_INLINE 2
#define AMFMT_SHORT        3
#define AMFMT_LONG         4
#define AMFMT_LONG_END     5

#define AMSH_CMASK_NONE    0
#define AMSH_CMASK_PREREQ  1
#define AMSH_CMASK_POSTREQ 2
#define AMSH_CMASK_DONE    3

#define AMSH_CSTATE_OUTGOING_NONE 		1
#define AMSH_CSTATE_OUTGOING_REPLIED 		2
#define AMSH_CSTATE_OUTGOING_ESTABLISHED 	3
#define AMSH_CSTATE_OUTGOING_DISC_REPLIED 	4
#define AMSH_CSTATE_OUTGOING_DISC_REQUESTED 	5

#define AMSH_CSTATE_INCOMING_NONE 		1
#define AMSH_CSTATE_INCOMING_DISC_REQUESTED 	4
#define AMSH_CSTATE_INCOMING_ESTABLISHED 	5

#define AMSH_PID_UNKNOWN			0

/**********************************
 * Shared memory packet formats
 **********************************/
typedef
struct am_pkt_short {
	uint32_t flag;	      /**> Packet state */
	union {
		uint32_t bulkidx; /**> index in bulk packet queue */
		uint32_t length;  /**> length when no bulkidx used */
	};
	uint16_t shmidx;      /**> index in shared segment */
	uint16_t type;
	uint16_t nargs;
	uint16_t handleridx;

	psm2_amarg_t args[NSHORT_ARGS];	/* AM arguments */

	/* We eventually will expose up to 8 arguments, but this isn't implemented
	 * For now.  >6 args will probably require a medium instead of a short */
} __attribute__ ((aligned(64)))
am_pkt_short_t;
PSMI_STRICT_SIZE_DECL(am_pkt_short_t, 64);

typedef struct am_pkt_bulk {
	uint32_t flag;
	uint32_t idx;
	uintptr_t dest;		/* Destination pointer in "longs" */
	uint32_t dest_off;	/* Destination pointer offset */
	uint32_t len;		/* Destination length within offset */
	psm2_amarg_t args[NBULK_ARGS];	/* Additional "spillover" for >6 args */
	uint8_t payload[0];
} am_pkt_bulk_t;
/* No strict size decl, used for mediums and longs */

/****************************************************
 * Shared memory header and block control structures
 ***************************************************/

/* Each pkt queue has the same header format, although the queue
 * consumers don't use the 'head' index in the same manner. */
typedef struct am_ctl_qhdr {
	uint32_t head;		/* Touched only by 1 consumer */
	uint8_t _pad0[64 - 4];

	pthread_spinlock_t lock;
	uint32_t tail;		/* XXX candidate for fetch-and-incr */
	uint32_t elem_cnt;
	uint32_t elem_sz;
	uint8_t _pad1[64 - 3 * 4 - sizeof(pthread_spinlock_t)];
} am_ctl_qhdr_t;
PSMI_STRICT_SIZE_DECL(am_ctl_qhdr_t, 128);

/* Each process has a reply qhdr and a request qhdr */
typedef struct am_ctl_blockhdr {
	volatile am_ctl_qhdr_t shortq;
	volatile am_ctl_qhdr_t longbulkq;
} am_ctl_blockhdr_t;
PSMI_STRICT_SIZE_DECL(am_ctl_blockhdr_t, 128 * 2);

/* We cache the "shorts" because that's what we poll on in the critical path.
 * We take care to always update these pointers whenever the segment is remapped.
 */
typedef struct am_ctl_qshort_cache {
	volatile am_pkt_short_t *base;
	volatile am_pkt_short_t *head;
	volatile am_pkt_short_t *end;
} am_ctl_qshort_cache_t;

/******************************************
 * Shared segment local directory (global)
 ******************************************
 *
 * Each process keeps a directory for where request and reply structures are
 * located at its peers.  This directory must be re-initialized every time the
 * shared segment moves in the VM, and the segment moves every time we remap()
 * for additional memory.
 */
struct amsh_qdirectory {
	am_ctl_blockhdr_t *qreqH;
	am_pkt_short_t *qreqFifoShort;
	am_pkt_bulk_t *qreqFifoLong;

	am_ctl_blockhdr_t *qrepH;
	am_pkt_short_t *qrepFifoShort;
	am_pkt_bulk_t *qrepFifoLong;
} __attribute__ ((aligned(64)));

/******************************************
 * Shared fifo element counts and sizes
 ******************************************
 * These values are context-wide, they can only be set early on and can't be *
 * modified at runtime.  All endpoints are expected to use the same values.
 */
typedef
struct amsh_qinfo {
	int qreqFifoShort;
	int qreqFifoLong;

	int qrepFifoShort;
	int qrepFifoLong;
} amsh_qinfo_t;

/******************************************
 * Per-endpoint structures (ep-local)
 ******************************************
 * Each endpoint keeps its own information as to where it resides in the
 * directory, and maintains its own cached copies of where the short header
 * resides in shared memory.
 *
 * This structure is carefully arranged to optimize cache locality and
 * performance.  Do not modify without careful and thorough analysis.
 */
struct am_ctl_nodeinfo {
	uint16_t psm_verno;
	volatile uint16_t is_init;
	volatile pid_t pid;
	psm2_epid_t epid;
	psm2_epaddr_t epaddr;
	uintptr_t amsh_shmbase;
	amsh_qinfo_t amsh_qsizes;
	uint32_t amsh_features;
	struct amsh_qdirectory qdir;
#ifdef PSM_ONEAPI
	int num_peer_fds;
	int peer_fds[MAX_ZE_DEVICES];
#endif
} __attribute__((aligned(64)));

struct ptl_am {
	psm2_ep_t ep;
	psm2_epid_t epid;
	psm2_epaddr_t epaddr;
	ptl_ctl_t *ctl;

	int connect_phase;
	int connect_outgoing;
	int connect_incoming;

	int zero_polls;
	int amsh_only_polls;
	int max_ep_idx, am_ep_size;
	int psmi_kassist_mode;
	char *amsh_keyname;

	/* These three items carefully picked to fit in one cache line. */
	am_ctl_qshort_cache_t reqH;
	am_ctl_qshort_cache_t repH;
	struct am_reqq_fifo_t psmi_am_reqq_fifo;

	am_pkt_short_t amsh_empty_shortpkt;

	struct am_ctl_nodeinfo *self_nodeinfo;
	struct am_ctl_nodeinfo *am_ep;
} __attribute__((aligned(64)));

#endif
