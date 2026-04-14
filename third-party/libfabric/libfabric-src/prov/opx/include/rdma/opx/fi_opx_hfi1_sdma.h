/*
 * Copyright (C) 2022-2025 by Cornelis Networks.
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
#ifndef _FI_OPX_HFI1_SDMA_H_
#define _FI_OPX_HFI1_SDMA_H_

#include <rdma/hfi/hfi1_user.h>

#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_reliability.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_hfi1_transport.h"

#define OPX_SDMA_FILL_INDEX_INVALID	    (0xFFFF)
#define OPX_SDMA_REQUEST_IOVS		    (FI_OPX_HFI1_SDMA_WE_IOVS + 1)
#define OPX_SDMA_TID_DATA_IOV_COUNT	    (2)
#define OPX_SDMA_TID_IOV_COUNT		    (OPX_SDMA_TID_DATA_IOV_COUNT + 1)
#define OPX_SDMA_NONTID_DATA_IOV_COUNT	    (1)
#define OPX_SDMA_NONTID_IOV_COUNT	    (OPX_SDMA_NONTID_DATA_IOV_COUNT + 1)
#define OPX_SDMA_REPLAY_DATA_IOV_COUNT	    (1)
#define OPX_SDMA_REPLAY_IOV_COUNT	    (OPX_SDMA_REPLAY_DATA_IOV_COUNT + 1)
#define OPX_SDMA_HFI_MAX_IOVS_PER_WRITE	    (128)
#define OPX_SDMA_HFI_DEFAULT_IOVS_PER_WRITE (64)

OPX_COMPILE_TIME_ASSERT((OPX_SDMA_HFI_MAX_IOVS_PER_WRITE + 1) <= OPX_COUNTERS_WRITEV_MAX,
			"OPX_COUNTERS_WRITEV_MAX should be OPX_SDMA_HFI_MAX_IOVS_PER_WRITE + 1!\n");
OPX_COMPILE_TIME_ASSERT((OPX_SDMA_HFI_DEFAULT_IOVS_PER_WRITE + 1) <= OPX_SDMA_HFI_MAX_IOVS_PER_WRITE,
			"OPX_SDMA_HFI_DEFAULT_IOVS_PER_WRITE should be <= OPX_SDMA_HFI_MAX_IOVS_PER_WRITE!\n");

// Driver limit of the number of TIDs that can be used in a single SDMA request
#define OPX_SDMA_MAX_TIDS_PER_REQUEST (1024)

#ifndef OPX_SDMA_MAX_PKTS_BOUNCE_BUF
#define OPX_SDMA_MAX_PKTS_BOUNCE_BUF (32)
#endif

#ifndef OPX_SDMA_DEFAULT_WRITEVS_PER_CYCLE
#define OPX_SDMA_DEFAULT_WRITEVS_PER_CYCLE (1)
#endif

#ifndef OPX_SDMA_MAX_WRITEVS_PER_CYCLE
#define OPX_SDMA_MAX_WRITEVS_PER_CYCLE (1024)
#endif

/*
 * Length of bounce buffer in a single SDMA Work Entry.
 */
#define FI_OPX_HFI1_SDMA_WE_BUF_LEN (OPX_SDMA_MAX_PKTS_BOUNCE_BUF * OPX_HFI1_MAX_PKT_SIZE)

#define OPX_SDMA_MEMINFO_SIZE	  (136)
#define OPX_SDMA_MEMINFO_SIZE_QWS (OPX_SDMA_MEMINFO_SIZE >> 3)

OPX_COMPILE_TIME_ASSERT((OPX_SDMA_MEMINFO_SIZE & 0x7) == 0, "OPX_SDMA_MEMINFO_SIZE must be a multiple of 8!");
#ifdef OPX_HMEM
OPX_COMPILE_TIME_ASSERT(sizeof(struct sdma_req_meminfo) == OPX_SDMA_MEMINFO_SIZE,
			"OPX_SDMA_MEMINFO_SIZE should be sizeof(struct sdma_req_meminfo)");

#endif

static const uint16_t OPX_SDMA_REQ_SET_MEMINFO[2] = {0,
#ifdef OPX_HMEM
						     ((uint16_t) 1) << HFI1_SDMA_REQ_MEMINFO_SHIFT
#else
						     0
#endif
};

static const size_t OPX_SDMA_REQ_HDR_SIZE[2] = {FI_OPX_HFI1_SDMA_HDR_SIZE,
						FI_OPX_HFI1_SDMA_HDR_SIZE + OPX_SDMA_MEMINFO_SIZE};

struct fi_opx_hfi1_sdma_header_vec {
	union {
		struct {
#ifdef OPX_HMEM
			uint64_t padding[OPX_SDMA_MEMINFO_SIZE_QWS];
#endif
			struct sdma_req_info req_info; // 8 bytes
		} sys;

		struct {
			struct sdma_req_info req_info; // 8 bytes

#ifdef OPX_HMEM
			struct sdma_req_meminfo req_meminfo; // 136 bytes
#endif
		} hmem;
	};
	union opx_hfi1_txe_scb_union scb;
};

static const size_t OPX_SDMA_REQ_INFO_OFFSET[2] = {offsetof(struct fi_opx_hfi1_sdma_header_vec, sys.req_info),
						   offsetof(struct fi_opx_hfi1_sdma_header_vec, hmem.req_info)};

#define OPX_SDMA_REQ_INFO_PTR(header_vec, set_meminfo) \
	((struct sdma_req_info *) ((uintptr_t) (header_vec) + OPX_SDMA_REQ_INFO_OFFSET[(set_meminfo)]))

enum opx_sdma_comp_state {
	OPX_SDMA_COMP_FREE = 0,
	OPX_SDMA_COMP_PENDING_WRITEV,
	OPX_SDMA_COMP_QUEUED,
	OPX_SDMA_COMP_COMPLETE,
	OPX_SDMA_COMP_ERROR
};

struct opx_sdma_request {
	/* ==== CACHELINE 0 ==== */
	struct opx_sdma_request	  *next;
	void			  *requester;
	enum opx_sdma_comp_state  *comp_state;
	struct opx_sdma_comp_entry comp_entry;
	uint8_t			   is_sdma_we;
	uint8_t			   unused_0[3];
	uint16_t		   fill_index;
	uint8_t			   num_iovs;
	uint8_t			   set_meminfo;

	uint64_t unused_1;

	/* ==== CACHELINE 1 ==== */
	struct iovec			   iovecs[OPX_SDMA_REQUEST_IOVS];
	struct fi_opx_hfi1_sdma_header_vec header_vec; // 72 bytes 9B or 80 bytes 16B, plus 136 bytes (OPX_HMEM)
};
OPX_COMPILE_TIME_ASSERT(offsetof(struct opx_sdma_request, iovecs) == FI_OPX_CACHE_LINE_SIZE,
			"Offset of opx_sdma_request->iovecs should start at cacheline 1!");

struct fi_opx_hfi1_sdma_packet {
	uint64_t			     length;
	struct fi_opx_reliability_tx_replay *replay;
};

struct fi_opx_hfi1_sdma_work_entry {
	/* ==== CACHELINE 0 ==== */
	struct fi_opx_hfi1_sdma_work_entry *next;
	struct fi_opx_completion_counter   *cc;
	union fi_opx_reliability_tx_psn	   *psn_ptr;
	struct {
		int		   device;
		enum fi_hmem_iface iface;
	} hmem;

	enum opx_sdma_comp_state comp_state;
	uint32_t		 total_payload;
	uint32_t		 num_packets;

	opx_lid_t dlid;

	uint16_t subctxt_rx;
	bool	 in_use;
	bool	 use_bounce_buf;
	bool	 pending_bounce_buf;
	uint8_t	 unused_byte_padding[3];

	uint64_t first_ack_time_ns;

	/* ==== CACHELINE 1 ==== */
	struct fi_opx_hfi1_sdma_packet packets[OPX_HFI1_SDMA_MAX_PKTS_TID];

	struct {
		struct fi_opx_completion_counter cc;
		uint32_t			 use_count;
		uint32_t			 unused;
		uint8_t				 buf[FI_OPX_HFI1_SDMA_WE_BUF_LEN];
	} bounce_buf;
};
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_hfi1_sdma_work_entry, packets) == FI_OPX_CACHE_LINE_SIZE,
			"Offset of fi_opx_hfi1_sdma_work_entry->packets should start at cacheline 1!");

struct fi_opx_hfi1_sdma_replay_work_entry {
	struct fi_opx_hfi1_sdma_replay_work_entry *next;
	struct fi_opx_reliability_tx_replay	  *replay;
	enum opx_sdma_comp_state		   comp_state;
	uint32_t				   unused;
};
OPX_COMPILE_TIME_ASSERT(!(sizeof(struct fi_opx_hfi1_sdma_replay_work_entry) & 7),
			"sizeof(struct fi_opx_hfi1_sdma_work_entry) should be multiple of 8");

void fi_opx_hfi1_sdma_hit_zero(struct fi_opx_completion_counter *cc);
void fi_opx_hfi1_sdma_bounce_buf_hit_zero(struct fi_opx_completion_counter *cc);
void fi_opx_hfi1_sdma_handle_errors(struct fi_opx_ep *opx_ep, int writev_rc, struct iovec *iovs, const int num_iovs,
				    const char *file, const char *func, const int line);
int  fi_opx_hfi1_dput_sdma_pending_completion(union fi_opx_hfi1_deferred_work *work);
void opx_hfi1_sdma_process_requests(struct fi_opx_ep *opx_ep);
void opx_hfi1_sdma_process_pending(struct fi_opx_ep *opx_ep);

__OPX_FORCE_INLINE__
bool fi_opx_hfi1_sdma_use_sdma(struct fi_opx_ep *opx_ep, uint64_t total_bytes, const uint32_t opcode,
			       const uint64_t is_hmem, const bool is_shm)
{
	/* This function should never be called for fence and error/truncation
	   opcodes. All other DPUT_OPCODEs are now supported for SDMA. */
	assert(opcode != FI_OPX_HFI_DPUT_OPCODE_FENCE && opcode != FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC);

	return !is_shm &&
	       (opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID || total_bytes >= opx_ep->tx->sdma_min_payload_bytes) &&
	       opx_ep->tx->use_sdma;
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_hfi1_sdma_queue_has_room(struct fi_opx_ep *opx_ep, const uint64_t iovs_needed)
{
	if (opx_ep->tx->sdma_request_queue.num_reqs >= opx_ep->tx->sdma_request_queue.slots_avail) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_fill_index);
		return 0;
	}

	if ((opx_ep->tx->sdma_request_queue.num_iovs + iovs_needed) > opx_ep->tx->sdma_request_queue.max_iovs) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_iov_limit);
		return 0;
	}

	return 1;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_init_cc(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_dput_params *params, const uint64_t length)
{
	struct fi_opx_completion_counter *cc =
		(struct fi_opx_completion_counter *) ofi_buf_alloc(opx_ep->rma_counter_pool);
	assert(cc);
	cc->next	       = NULL;
	cc->initial_byte_count = length;
	cc->byte_counter       = length;
	cc->cq		       = NULL;
	cc->container	       = (void *) params;
	cc->cntr	       = NULL;
	cc->hit_zero	       = fi_opx_hfi1_sdma_hit_zero;
	params->cc	       = cc;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_dput_sdma_init(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_dput_params *params, const uint64_t length,
				const uint32_t tidoffset, const uint32_t ntidpairs, const uint32_t *const tidpairs,
				const uint64_t is_hmem, const enum opx_hfi1_type hfi1_type)
{
	if (!fi_opx_hfi1_sdma_use_sdma(opx_ep, length, params->opcode, is_hmem, params->is_shm)) {
		if (hfi1_type == OPX_HFI1_JKR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_jkr;
		} else if (hfi1_type == OPX_HFI1_CYR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_cyr;
		} else if (hfi1_type == OPX_HFI1_JKR_9B) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_jkr_9B;
		} else if (hfi1_type == OPX_HFI1_WFR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_wfr;
		}
		params->work_elem.work_type = params->is_shm ? OPX_WORK_TYPE_SHM : OPX_WORK_TYPE_PIO;
		return;
	}

	params->sdma_no_bounce_buf = (length >= opx_ep->tx->sdma_bounce_buf_threshold) || (is_hmem) ||
				     (params->opcode == FI_OPX_HFI_DPUT_OPCODE_GET) ||
				     (params->opcode == FI_OPX_HFI_DPUT_OPCODE_PUT) ||
				     (params->opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ) ||
				     (params->opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH) ||
				     (params->opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH);

	if (!params->sdma_no_bounce_buf) {
		assert(params->origin_byte_counter);
	}
	params->user_cc = params->cc;
	fi_opx_hfi1_sdma_init_cc(opx_ep, params, length);

	slist_init(&params->sdma_reqs);

	params->sdma_we	       = NULL;
	params->sdma_reqs_used = 0;
	if (ntidpairs) {
		params->ntidpairs	 = ntidpairs;
		params->tidoffset	 = tidoffset;
		params->tididx		 = -1U;
		params->tid_iov.iov_len	 = ntidpairs * sizeof(uint32_t);
		params->tid_iov.iov_base = &params->tidpairs[0];
		for (int idx = 0; idx < ntidpairs; idx++) {
			params->tidpairs[idx] = tidpairs[idx];
		}
		OPX_DEBUG_TIDS("CTS tid_iov", ntidpairs, params->tidpairs);

		if (hfi1_type == OPX_HFI1_JKR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_tid_jkr;
		} else if (hfi1_type == OPX_HFI1_CYR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_tid_cyr;
		} else if (hfi1_type == OPX_HFI1_JKR_9B) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_tid_jkr_9B;
		} else if (hfi1_type == OPX_HFI1_WFR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_tid_wfr;
		}
	} else {
		if (hfi1_type == OPX_HFI1_JKR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_jkr;
		} else if (hfi1_type == OPX_HFI1_CYR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_cyr;
		} else if (hfi1_type == OPX_HFI1_JKR_9B) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_jkr_9B;
		} else if (hfi1_type == OPX_HFI1_WFR) {
			params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_wfr;
		}
	}
	params->work_elem.work_type = OPX_WORK_TYPE_SDMA;
}

__OPX_FORCE_INLINE__
bool fi_opx_hfi1_sdma_has_unsent_packets(struct fi_opx_hfi1_sdma_work_entry *we)
{
	return we->num_packets && we->comp_state != OPX_SDMA_COMP_QUEUED &&
	       we->comp_state != OPX_SDMA_COMP_PENDING_WRITEV;
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_sdma_work_entry *fi_opx_hfi1_sdma_get_idle_we(struct fi_opx_ep *opx_ep)
{
	struct fi_opx_hfi1_sdma_work_entry *entry =
		(struct fi_opx_hfi1_sdma_work_entry *) ofi_buf_alloc(opx_ep->tx->sdma_work_pool);

	if (!entry) {
		return NULL;
	}

	entry->next		    = NULL;
	entry->comp_state	    = OPX_SDMA_COMP_FREE;
	entry->num_packets	    = 0;
	entry->total_payload	    = 0;
	entry->psn_ptr		    = NULL;
	entry->in_use		    = true;
	entry->pending_bounce_buf   = false;
	entry->first_ack_time_ns    = 0;
	entry->bounce_buf.use_count = 0;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SDMA_WE -- giving WE %p\n", entry);
	return entry;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_return_we(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_work_entry *we)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SDMA_WE -- returned WE %p\n", we);
	assert(we->next == NULL);
	assert(we->in_use);
	we->in_use = false;

	OPX_BUF_FREE(we);
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_sdma_work_entry *opx_sdma_get_new_work_entry(struct fi_opx_ep *opx_ep, uint16_t *reqs_used,
								struct slist			   *sdma_reqs,
								struct fi_opx_hfi1_sdma_work_entry *current)
{
	// Get a new SDMA work entry. First try to get an idle one if
	// we're not already using too many.
	struct fi_opx_hfi1_sdma_work_entry *sdma_we;
	if ((*reqs_used) < FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ) {
		sdma_we = fi_opx_hfi1_sdma_get_idle_we(opx_ep);
		if (sdma_we) {
			++(*reqs_used);
			assert(sdma_we->next == NULL);
			OPX_TRACER_TRACE_SDMA(OPX_TRACER_INSTANT, "GET_IDLE_WE");
			return sdma_we;
		}
	}

	// No idle entries available, or we've already been allocated the max.
	// See if one of our existing entries is available for re-use.
	sdma_we					 = (struct fi_opx_hfi1_sdma_work_entry *) sdma_reqs->head;
	struct fi_opx_hfi1_sdma_work_entry *prev = NULL;

	while (sdma_we && sdma_we != current) {
		if ((sdma_we->comp_state == OPX_SDMA_COMP_COMPLETE || sdma_we->comp_state == OPX_SDMA_COMP_ERROR) &&
		    !sdma_we->pending_bounce_buf) {
			slist_remove(sdma_reqs, (struct slist_entry *) sdma_we, (struct slist_entry *) prev);
			sdma_we->next		   = NULL;
			sdma_we->comp_state	   = OPX_SDMA_COMP_FREE;
			sdma_we->num_packets	   = 0;
			sdma_we->total_payload	   = 0;
			sdma_we->psn_ptr	   = NULL;
			sdma_we->first_ack_time_ns = 0;
			++sdma_we->bounce_buf.use_count;
			OPX_TRACER_TRACE_SDMA(OPX_TRACER_INSTANT, "GET_REUSED_WE");
			return sdma_we;
		}
		prev	= sdma_we;
		sdma_we = sdma_we->next;
	}

	OPX_TRACER_TRACE_SDMA(OPX_TRACER_INSTANT, "GET_NO_WE");
	return NULL;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_init_we(struct fi_opx_hfi1_sdma_work_entry *we, struct fi_opx_completion_counter *cc,
			      uint16_t dlid, uint16_t subctxt_rx, enum fi_hmem_iface iface, int hmem_device)
{
	we->cc		= cc;
	we->dlid	= dlid;
	we->subctxt_rx	= subctxt_rx;
	we->comp_state	= OPX_SDMA_COMP_FREE;
	we->hmem.iface	= iface;
	we->hmem.device = hmem_device;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_add_packet(struct fi_opx_hfi1_sdma_work_entry *we, struct fi_opx_reliability_tx_replay *replay,
				 uint64_t payload_bytes)
{
	assert(payload_bytes <= OPX_HFI1_PKT_SIZE);
	assert(we->num_packets < OPX_HFI1_SDMA_MAX_PKTS_TID);

	we->packets[we->num_packets].replay = replay;
	we->packets[we->num_packets].length = payload_bytes;
	we->num_packets++;

	we->total_payload += payload_bytes;
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- Add_packet %u, payload_bytes=%ld, total_payload=%d\n",
		we->num_packets, payload_bytes, we->total_payload);
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_set_meminfo(struct sdma_req_info *req_info, const uint64_t set_meminfo,
				  const enum fi_hmem_iface iface, const uint64_t device)
{
#ifdef OPX_HMEM
	if (set_meminfo) {
		// We currently only ever use one payload IOV that would require
		// setting meminfo, and it will be the fist one:
		// index 0 (the first payload IOV, or iov[1]).
		const unsigned		 meminfo_idx = 0;
		const unsigned		 type	     = OPX_HMEM_KERN_MEM_TYPE[iface];
		struct sdma_req_meminfo *meminfo     = (struct sdma_req_meminfo *) (req_info + 1);
		meminfo->types			     = 0;
		HFI1_MEMINFO_TYPE_ENTRY_SET(meminfo->types, meminfo_idx, type);

		meminfo->context[0]  = device; // meminfo_idx
		meminfo->context[1]  = 0;
		meminfo->context[2]  = 0;
		meminfo->context[3]  = 0;
		meminfo->context[4]  = 0;
		meminfo->context[5]  = 0;
		meminfo->context[6]  = 0;
		meminfo->context[7]  = 0;
		meminfo->context[8]  = 0;
		meminfo->context[9]  = 0;
		meminfo->context[10] = 0;
		meminfo->context[11] = 0;
		meminfo->context[12] = 0;
		meminfo->context[13] = 0;
		meminfo->context[14] = 0;
		meminfo->context[15] = 0;
	}
#endif
}

__OPX_FORCE_INLINE__
int opx_hfi1_sdma_enqueue_request(struct fi_opx_ep *opx_ep, void *requester,
				  enum opx_sdma_comp_state     *requester_comp_state,
				  union opx_hfi1_txe_scb_union *source_scb, struct iovec *iovs, const uint16_t num_iovs,
				  const uint16_t num_packets, const uint16_t frag_size, const uint16_t req_control_bits,
				  const enum fi_hmem_iface hmem_iface, const uint64_t hmem_device,
				  const uint64_t last_packet_bytes, const uint32_t kdeth, const uint8_t is_sdma_we)
{
	assert(!(frag_size & 0x3F)); // frag_size should be multiple of 64
	assert(num_iovs <= FI_OPX_HFI1_SDMA_WE_IOVS && num_iovs > 0);

	struct opx_sdma_request *request = (struct opx_sdma_request *) ofi_buf_alloc(opx_ep->tx->sdma_request_pool);

	if (OFI_UNLIKELY(request == NULL)) {
		FI_WARN(&fi_opx_provider, FI_LOG_EP_DATA, "ERROR allocating opx_sdma_request!\n");
		return -FI_ENOMEM;
	}

	const uint8_t set_meminfo = (hmem_iface > FI_HMEM_SYSTEM) ? 1 : 0;

	struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&request->header_vec, set_meminfo);
	fi_opx_hfi1_sdma_set_meminfo(req_info, set_meminfo, hmem_iface, hmem_device);

	req_info->ctrl = req_control_bits | ((num_iovs + 1) << HFI1_SDMA_REQ_IOVCNT_SHIFT) |
			 OPX_SDMA_REQ_SET_MEMINFO[set_meminfo];
	req_info->fragsize = frag_size;
	req_info->npkts	   = num_packets;
	req_info->comp_idx = OPX_SDMA_FILL_INDEX_INVALID;

	request->num_iovs    = num_iovs + 1;
	request->requester   = requester;
	request->is_sdma_we  = is_sdma_we;
	request->comp_state  = requester_comp_state;
	request->fill_index  = OPX_SDMA_FILL_INDEX_INVALID;
	request->set_meminfo = set_meminfo;

	/* Set the Acknowledge Request Bit if we're only sending one packet */
	uint64_t set_ack_bit = (num_packets == 1) ? (uint64_t) htonl(0x80000000) : 0;

	request->iovecs[0].iov_base = req_info;

	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		request->header_vec.scb.scb_9B = (source_scb->scb_9B);
		request->header_vec.scb.scb_9B.hdr.qw_9B[2] |= ((uint64_t) kdeth << 32) | set_ack_bit;
		request->header_vec.scb.scb_9B.hdr.qw_9B[4] |= (last_packet_bytes << 32);
		request->iovecs[0].iov_len = OPX_SDMA_REQ_HDR_SIZE[set_meminfo];
	} else {
		request->header_vec.scb.scb_16B = (source_scb->scb_16B);
		request->header_vec.scb.scb_16B.hdr.qw_16B[3] |= ((uint64_t) kdeth << 32) | set_ack_bit;
		request->header_vec.scb.scb_16B.hdr.qw_16B[5] |= (last_packet_bytes << 32);
		request->iovecs[0].iov_len = OPX_SDMA_REQ_HDR_SIZE[set_meminfo] + 8; // extra QWORD in 16B LRH
	}

	for (int i = 0; i < num_iovs; ++i) {
		request->iovecs[i + 1] = iovs[i];
	}

	slist_insert_tail((struct slist_entry *) request, &opx_ep->tx->sdma_request_queue.list);
	++opx_ep->tx->sdma_request_queue.num_reqs;
	opx_ep->tx->sdma_request_queue.num_iovs += request->num_iovs;

	*requester_comp_state = OPX_SDMA_COMP_PENDING_WRITEV;

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
int opx_hfi1_sdma_enqueue_replay(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_replay_work_entry *we,
				 struct fi_opx_reliability_tx_replay *replay, uint64_t payload_bytes)
{
	assert(replay->use_iov);
	assert(replay->iov->iov_len == payload_bytes);

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.replay_requests);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== Enqueuing replay for SDMA Send\n");
	return opx_hfi1_sdma_enqueue_request(
		opx_ep, we, &we->comp_state, &replay->scb, replay->iov, OPX_SDMA_REPLAY_DATA_IOV_COUNT,
		1,			       // num_packets,
		(payload_bytes + 63) & 0xFFC0, // Frag_size
		FI_OPX_HFI1_SDMA_REQ_HEADER_EAGER_FIXEDBITS, replay->hmem_iface, replay->hmem_device,
		replay->scb.scb_9B.hdr.dput.target.bytes, // last packet bytes
		0,					  // kdeth tid info unused for replays
		0					  // Not an SDMA WE
	);
}

__OPX_FORCE_INLINE__
uint16_t opx_hfi1_sdma_register_replays(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_work_entry *we,
					const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_completion_counter *cc;
	void				 *replay_back_ptr;
	if (we->use_bounce_buf) {
		we->bounce_buf.cc.initial_byte_count = we->total_payload;
		we->bounce_buf.cc.byte_counter	     = we->total_payload;
		we->bounce_buf.cc.container	     = (void *) we;
		we->bounce_buf.cc.next		     = we->cc;
		we->bounce_buf.cc.hit_zero	     = fi_opx_hfi1_sdma_bounce_buf_hit_zero;
		cc				     = &we->bounce_buf.cc;
		we->pending_bounce_buf		     = true;
		replay_back_ptr			     = (void *) we;
	} else {
		cc		= we->cc;
		replay_back_ptr = NULL;
	}

	/* Since we already verified that enough PSNs were available for
	   the send we're about to do, we shouldn't need to check the
	   returned PSN here before proceeding */
	int32_t psn;
	psn = fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid, opx_ep->reli_service, we->dlid, we->subctxt_rx,
					     &we->psn_ptr, we->num_packets);

	uint32_t fragsize = 0;
	for (int i = 0; i < we->num_packets; ++i) {
		fragsize = MAX(fragsize, we->packets[i].length);
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
			we->packets[i].replay->scb.scb_9B.hdr.qw_9B[2] |= (uint64_t) htonl((uint32_t) psn);
		} else {
			we->packets[i].replay->scb.scb_16B.hdr.qw_16B[3] |= (uint64_t) htonl((uint32_t) psn);
		}

		we->packets[i].replay->sdma_we_use_count = we->bounce_buf.use_count;
		we->packets[i].replay->sdma_we		 = replay_back_ptr;
		we->packets[i].replay->hmem_iface	 = we->hmem.iface;
		we->packets[i].replay->hmem_device	 = we->hmem.device;
		fi_opx_reliability_service_replay_register_with_update(opx_ep->reli_service, we->psn_ptr,
								       we->packets[i].replay, cc, we->packets[i].length,
								       reliability, hfi1_type);
		psn = (psn + 1) & MAX_PSN;
	}

	/* Frag size must be a multiple of 64. Round up if it's not already */
	fragsize = (fragsize + 63) & 0xFFC0;

	return fragsize;
}

__OPX_FORCE_INLINE__
void opx_hfi1_sdma_enqueue_dput(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_work_entry *we, uint32_t fragsize,
				uint64_t last_packet_bytes)
{
	struct iovec payload_iov = {.iov_base = we->packets[0].replay->iov->iov_base,
				    .iov_len  = (we->total_payload + 3) & -4};

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.nontid_requests);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== Enqueuing non-tid request for SDMA Send.\n");
	opx_hfi1_sdma_enqueue_request(opx_ep, we, &we->comp_state, &we->packets[0].replay->scb, &payload_iov,
				      OPX_SDMA_NONTID_DATA_IOV_COUNT, we->num_packets, fragsize,
				      FI_OPX_HFI1_SDMA_REQ_HEADER_EAGER_FIXEDBITS, we->hmem.iface, we->hmem.device,
				      last_packet_bytes,
				      0,  // kdeth tid info unused
				      1); // This is an SDMA work entry
}

__OPX_FORCE_INLINE__
void opx_hfi1_sdma_enqueue_dput_tid(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_work_entry *we, uint32_t fragsize,
				    uint64_t last_packet_bytes, struct iovec *tid_iov, /* not an array */
				    uint32_t start_tid_idx, uint32_t end_tid_idx, uint32_t tidOMshift,
				    uint32_t tidoffset)
{
	uint32_t *tidpairs = NULL;

	/* tid packet lengths should have been aligned.*/
	assert(start_tid_idx < tid_iov->iov_len / sizeof(uint32_t));
	assert(end_tid_idx < tid_iov->iov_len / sizeof(uint32_t));
	assert(end_tid_idx >= start_tid_idx);
	assert(((end_tid_idx - start_tid_idx) + 1) <= OPX_SDMA_MAX_TIDS_PER_REQUEST);
	tidpairs = (uint32_t *) tid_iov->iov_base;

	uint32_t tidpair = tidpairs[start_tid_idx];
	uint32_t kdeth	 = (FI_OPX_HFI1_KDETH_TIDCTRL & FI_OPX_EXP_TID_GET((tidpair), CTRL))
			 << FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT;
	kdeth |= (FI_OPX_HFI1_KDETH_TID & FI_OPX_EXP_TID_GET((tidpair), IDX)) << FI_OPX_HFI1_KDETH_TID_SHIFT;
	kdeth |= tidOMshift;
	kdeth |= tidoffset;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "kdeth %#X, tid    [%u]=%#8.8X LEN %u, CTRL %u, IDX %u, offset %#X %#X\n", kdeth, 0, tidpair,
	       (int) FI_OPX_EXP_TID_GET((tidpair), LEN), (int) FI_OPX_EXP_TID_GET((tidpair), CTRL),
	       (int) FI_OPX_EXP_TID_GET((tidpair), IDX), tidoffset,
	       tidOMshift ? tidoffset << KDETH_OM_LARGE_SHIFT : tidoffset << KDETH_OM_SMALL_SHIFT);

	size_t	     tid_iov_len	 = ((end_tid_idx - start_tid_idx) + 1) * sizeof(uint32_t);
	struct iovec payload_tid_iovs[2] = {
		{.iov_base = we->packets[0].replay->iov->iov_base, .iov_len = (we->total_payload + 3) & -4},
		{.iov_base = &tidpairs[start_tid_idx], .iov_len = tid_iov_len}};

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.tid_requests);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== Enqueuing tid request for SDMA Send\n");
	opx_hfi1_sdma_enqueue_request(opx_ep, we, &we->comp_state, &we->packets[0].replay->scb, payload_tid_iovs,
				      OPX_SDMA_TID_DATA_IOV_COUNT, we->num_packets, fragsize,
				      FI_OPX_HFI1_SDMA_REQ_HEADER_EXPECTED_FIXEDBITS, we->hmem.iface, we->hmem.device,
				      last_packet_bytes, kdeth,
				      1); // This is an SDMA work entry
}

__OPX_FORCE_INLINE__
void opx_hfi1_sdma_flush(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_work_entry *we, struct slist *sdma_reqs,
			 const bool use_tid, struct iovec *tid_iov, uint32_t start_tid_idx, uint32_t end_tid_idx,
			 uint32_t tidOMshift, uint32_t tidoffset, enum ofi_reliability_kind reliability,
			 const enum opx_hfi1_type hfi1_type)
{
	assert(we->comp_state == OPX_SDMA_COMP_FREE);
	assert(we->num_packets > 0);

	/* no padding for tid, should have been aligned.*/
	assert(!use_tid || (use_tid && we->total_payload == ((we->total_payload) & -4)));

	uint32_t fragsize	   = opx_hfi1_sdma_register_replays(opx_ep, we, reliability, hfi1_type);
	uint64_t last_packet_bytes = we->packets[we->num_packets - 1].length;

	assert(last_packet_bytes != 0);

	if (use_tid) {
		opx_hfi1_sdma_enqueue_dput_tid(opx_ep, we, fragsize, last_packet_bytes, tid_iov, start_tid_idx,
					       end_tid_idx, tidOMshift, tidoffset);
	} else {
		opx_hfi1_sdma_enqueue_dput(opx_ep, we, fragsize, last_packet_bytes);
	}

	assert(we->comp_state == OPX_SDMA_COMP_PENDING_WRITEV);
	assert(we->next == NULL);
	slist_insert_tail((struct slist_entry *) we, sdma_reqs);
}

#endif /* _FI_OPX_HFI1_SDMA_H_ */
