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

/* Interface implemented by Packet Transport layers such as
 * ips and active messages.
 *
 * This interface can be volatile, it is never seen by PSM clients, and it will
 * probably change as the AM ptl is developed.
 */

#ifndef PSM_PTL_H
#define PSM_PTL_H
#include <inttypes.h>
#include <psm2.h>
#include <psm2_mq.h>
#include <psm2_am.h>
#include <psm_help.h>

/* struct ptl is an incomplete type, and it serves as a generic or opaque
   container.  It should remain an incomplete type in the entire psm
   source base. concrete ptl types need to have a suffix such as ptl_self,
   ptl_ips. */
struct ptl;
typedef struct ptl ptl_t;

struct ptl_ctl;
typedef struct ptl_ctl ptl_ctl_t;

struct ptl_mq_req;
typedef struct ptl_mq_req ptl_mq_req_t;

struct ips_proto;
typedef struct ips_proto ips_proto_t;

/* To be filled in statically by all PTLs */
struct ptl_ctl_init {
	size_t(*sizeof_ptl) (void);

	psm2_error_t(*init) (const psm2_ep_t ep, ptl_t *ptl, ptl_ctl_t *ctl);

	psm2_error_t(*fini) (ptl_t *ptl, int force, uint64_t timeout_ns);

	psm2_error_t
	    (*setopt) (const void *component_obj, int optname,
		       const void *optval, uint64_t optlen);

	psm2_error_t
	    (*getopt) (const void *component_obj, int optname,
		       void *optval, uint64_t *optlen);
};

struct ptl_ctl_rcvthread {
	uint32_t(*is_enabled) (const ptl_t *ptl);
	void(*transfer_ownership) (ptl_t *from_ptl, ptl_t *to_ptl);
};

typedef
struct ptl_arg {
	union {
		struct {
			uint16_t u16w3;
			uint16_t u16w2;
			uint16_t u16w1;
			uint16_t u16w0;
		} PACK_SUFFIX;
		struct {
			uint32_t u32w1;
			uint32_t u32w0;
		} PACK_SUFFIX;
		uint64_t u64w0;
		uint64_t u64;
		void *uptr;
	};
} PACK_SUFFIX ptl_arg_t;

/* can be tracked per protocol, only fully tracked and reported
 * for ips_proto at this time but by defining here we can later track
 * for shm and maybe self protocols too and we avoid a branch in
 * psm3_mq_handle_envelope
 */
struct ptl_strategy_stats {
	uint64_t tiny_cpu_isend;
	uint64_t tiny_cpu_isend_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t tiny_gdrcopy_isend;
	uint64_t tiny_gdrcopy_isend_bytes;
	uint64_t tiny_cuCopy_isend;
	uint64_t tiny_cuCopy_isend_bytes;
#endif
	uint64_t tiny_cpu_send;
	uint64_t tiny_cpu_send_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t tiny_gdrcopy_send;
	uint64_t tiny_gdrcopy_send_bytes;
	uint64_t tiny_cuCopy_send;
	uint64_t tiny_cuCopy_send_bytes;
#endif

	uint64_t tiny_cpu_recv;
	uint64_t tiny_cpu_recv_bytes;
	uint64_t tiny_sysbuf_recv;	/* to unexpected Q sysbuf */ /* incl 0 byte */
	uint64_t tiny_sysbuf_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t tiny_gdrcopy_recv;
	uint64_t tiny_gdrcopy_recv_bytes;
	uint64_t tiny_cuCopy_recv;
	uint64_t tiny_cuCopy_recv_bytes;
#endif

	uint64_t short_copy_cpu_isend;
	uint64_t short_copy_cpu_isend_bytes;
	uint64_t short_dma_cpu_isend;
	uint64_t short_dma_cpu_isend_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t short_gdrcopy_isend;
	uint64_t short_gdrcopy_isend_bytes;
	uint64_t short_cuCopy_send;
	uint64_t short_cuCopy_send_bytes;
	uint64_t short_gdr_send;
	uint64_t short_gdr_send_bytes;
#endif
	uint64_t short_copy_cpu_send;
	uint64_t short_copy_cpu_send_bytes;
	uint64_t short_dma_cpu_send;
	uint64_t short_dma_cpu_send_bytes;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t short_gdrcopy_send;
	uint64_t short_gdrcopy_send_bytes;
	uint64_t short_cuCopy_isend;
	uint64_t short_cuCopy_isend_bytes;
	uint64_t short_gdr_isend;
	uint64_t short_gdr_isend_bytes;
#endif

	uint64_t short_cpu_recv;
	uint64_t short_cpu_recv_bytes;
	uint64_t short_sysbuf_recv;	/* to unexpected Q sysbuf */
	uint64_t short_sysbuf_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t short_gdrcopy_recv;
	uint64_t short_gdrcopy_recv_bytes;
	uint64_t short_cuCopy_recv;
	uint64_t short_cuCopy_recv_bytes;
#endif

	uint64_t eager_copy_cpu_isend;
	uint64_t eager_copy_cpu_isend_bytes;
	uint64_t eager_dma_cpu_isend;
	uint64_t eager_dma_cpu_isend_bytes;
	uint64_t eager_sysbuf_recv;	/* to unexpected Q sysbuf */
	uint64_t eager_sysbuf_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t eager_cuCopy_isend;
	uint64_t eager_cuCopy_isend_bytes;
	uint64_t eager_gdr_isend;
	uint64_t eager_gdr_isend_bytes;
#endif
	uint64_t eager_copy_cpu_send;
	uint64_t eager_copy_cpu_send_bytes;
	uint64_t eager_dma_cpu_send;
	uint64_t eager_dma_cpu_send_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t eager_cuCopy_send;
	uint64_t eager_cuCopy_send_bytes;
	uint64_t eager_gdr_send;
	uint64_t eager_gdr_send_bytes;
#endif

	uint64_t eager_cpu_recv;
	uint64_t eager_cpu_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t eager_gdrcopy_recv;
	uint64_t eager_gdrcopy_recv_bytes;
	uint64_t eager_cuCopy_recv;
	uint64_t eager_cuCopy_recv_bytes;
#endif

	uint64_t rndv_cpu_isend;
	uint64_t rndv_cpu_isend_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_gpu_isend;
	uint64_t rndv_gpu_isend_bytes;
#endif
	uint64_t rndv_cpu_send;
	uint64_t rndv_cpu_send_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_gpu_send;
	uint64_t rndv_gpu_send_bytes;
#endif

	/* Payload in RTS for small sync send */
	uint64_t rndv_rts_cpu_recv;
	uint64_t rndv_rts_cpu_recv_bytes;
	uint64_t rndv_rts_sysbuf_recv;
	uint64_t rndv_rts_sysbuf_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_rts_cuCopy_recv;
	uint64_t rndv_rts_cuCopy_recv_bytes;
#endif

	/* Payload in RTS approach used by sender */
	/* this approach uses a LONG DATA CTS, but sends no more data */
	uint64_t rndv_rts_copy_cpu_send;	/* per CTS  (1 per RTS) */
	uint64_t rndv_rts_copy_cpu_send_bytes;

	/* LONG DATA approach selected by receiver */
	uint64_t rndv_long_cpu_recv;	/* per RTS */
	uint64_t rndv_long_cpu_recv_bytes;
	uint64_t rndv_long_gpu_recv;	/* per RTS */
	uint64_t rndv_long_gpu_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_long_cuCopy_recv;
	uint64_t rndv_long_cuCopy_recv_bytes;
	uint64_t rndv_long_gdrcopy_recv;
	uint64_t rndv_long_gdrcopy_recv_bytes;
#endif

	/* LONG DATA approach used by sender after LONG selected by receiver */
	/* LONG DATA only uses 1 CTS per RTS */
	uint64_t rndv_long_copy_cpu_send;	/* per CTS  (1 per RTS) */
	uint64_t rndv_long_copy_cpu_send_bytes;
	uint64_t rndv_long_dma_cpu_send;	/* per CTS  (1 per RTS) */
	uint64_t rndv_long_dma_cpu_send_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_long_cuCopy_send;	/* per CTS  (1 per RTS) */
	uint64_t rndv_long_cuCopy_send_bytes;
	uint64_t rndv_long_gdrcopy_send;	/* per CTS  (1 per RTS) */
	uint64_t rndv_long_gdrcopy_send_bytes;
	uint64_t rndv_long_gdr_send;	/* per CTS  (1 per RTS) */ /* SDMA */
	uint64_t rndv_long_gdr_send_bytes;		/* SDMA */
#endif

	/* RDMA approach selected by receiver */
	uint64_t rndv_rdma_cpu_recv;	/* per RTS */
	uint64_t rndv_rdma_cpu_recv_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_rdma_gdr_recv;	/* per RTS */
	uint64_t rndv_rdma_gdr_recv_bytes;
	uint64_t rndv_rdma_hbuf_recv;	/* per RTS */
	uint64_t rndv_rdma_hbuf_recv_bytes;
#endif

	/* RDMA approach used by sender after RDMA selected by receiver */
	/* RDMA may use >= 1 CTS per RTS */
	uint64_t rndv_rdma_cpu_send;	/* per CTS */
	uint64_t rndv_rdma_cpu_send_bytes;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t rndv_rdma_gdr_send;	/* per CTS */
	uint64_t rndv_rdma_gdr_send_bytes;
	uint64_t rndv_rdma_hbuf_send;	/* per CTS */
	uint64_t rndv_rdma_hbuf_send_bytes;
#endif
};

#include "ptl_self/ptl_fwd.h"
#include "ptl_ips/ptl_fwd.h"
#include "ptl_am/ptl_fwd.h"

/* To be filled in as part of ptl_init */
struct ptl_ctl {
	ptl_t *ptl;		/* pointer to ptl */
	psm2_ep_t ep;		/* pointer to ep */

	/* EP-specific stuff */
	 psm2_error_t(*ep_poll) (ptl_t *ptl, int replyonly, bool force);

	/* PTL-level connect
	 *
	 * This PTL-level is slightly different from the top-level PSM connect.
	 *
	 * pre 1: Caller has masked off epids in epid array that are already
	 *        connected at the PSM level.
	 *
	 * post 0: PTL has allocate all epaddrs and whatever internal ptladdr
	 *         that ptl needs.
	 * post 1: PTL marks error[i] as UNREACHABLE if PTL can't get to epid[i]
	 * post 2: PTL marks error[i] as UNKNOWN for all epid[i] that couldn't
	 *         be connected before a timeout occurred.
	 * post 3: PTL returns OK if all epids are either OK or UNREACHABLE
	 * post 4: PTL defines content or epaddr[i] only if epaddr[i] is OK.
	 */
	 psm2_error_t(*ep_connect) (ptl_t *ptl,
				   int num_ep,
				   const psm2_epid_t input_array_of_epid[],
				   const int array_of_epid_mask[],
				   psm2_error_t output_array_of_errors[],
				   psm2_epaddr_t output_array_of_epddr[],
				   uint64_t timeout_ns);

	 psm2_error_t (*ep_disconnect)(ptl_t *ptl,
				       int force,
				       int num_ep,
				       psm2_epaddr_t input_array_of_epaddr[],
				       const int array_of_epaddr_mask[],
				       psm2_error_t output_array_of_errors[],
				       uint64_t timeout_ns);

	/* MQ stuff */
	 psm2_error_t(*mq_send) (psm2_mq_t mq, psm2_epaddr_t dest,
				uint32_t flags, psm2_mq_tag_t *stag,
				const void *buf, uint32_t len);
	 psm2_error_t(*mq_isend) (psm2_mq_t mq, psm2_epaddr_t dest,
				  uint32_t flags_user, uint32_t flags_internal,
				  psm2_mq_tag_t *stag, const void *buf,
				  uint32_t len, void *ctxt, psm2_mq_req_t *req);

#if 0	// unused code, specific to QLogic MPI
	int (*epaddr_stats_num) (void);
	int (*epaddr_stats_init) (char *desc[], uint16_t *flags);
	int (*epaddr_stats_get) (psm2_epaddr_t epaddr, uint64_t *stats);
#endif

	/* AM stuff */
	 psm2_error_t(*am_get_parameters) (psm2_ep_t ep,
					  struct psm2_am_parameters *
					  parameters);
	 psm2_error_t(*am_short_request) (psm2_epaddr_t epaddr,
					 psm2_handler_t handler,
					 psm2_amarg_t *args, int nargs,
					 void *src, size_t len, int flags,
					 psm2_am_completion_fn_t completion_fn,
					 void *completion_ctxt);
	 psm2_error_t(*am_short_reply) (psm2_am_token_t token,
				       psm2_handler_t handler,
				       psm2_amarg_t *args, int nargs, void *src,
				       size_t len, int flags,
				       psm2_am_completion_fn_t completion_fn,
				       void *completion_ctxt);
	/* Long messages currently unsupported */
#if 0
	 psm2_error_t(*am_long_request) (psm2_epaddr_t epaddr,
					psm2_handler_t handler,
					psm2_amarg_t *args, int nargs,
					void *src, size_t len, void *dest,
					int flags);
	 psm2_error_t(*am_long_reply) (psm2_am_token_t token,
				      psm2_handler_t handler, psm2_amarg_t *args,
				      int nargs, void *src, size_t len,
				      void *dest, int flags);
#endif
};
#endif
