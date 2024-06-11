/*
 * Copyright (C) 2022-2024 by Cornelis Networks.
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

#define OPX_SDMA_MEMINFO_SIZE		(136)
#define OPX_SDMA_MEMINFO_SIZE_QWS	(OPX_SDMA_MEMINFO_SIZE >> 3)
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
			uint64_t		padding[OPX_SDMA_MEMINFO_SIZE_QWS];
#endif
			struct sdma_req_info	req_info;	// 8 bytes
		} sys;

		struct {
			struct sdma_req_info	req_info;	// 8 bytes

#ifdef OPX_HMEM
			struct sdma_req_meminfo	req_meminfo;	// 136 bytes
#endif
		} hmem;
	};

	struct fi_opx_hfi1_txe_scb		scb;
};

static const size_t OPX_SDMA_REQ_INFO_OFFSET[2] = {
	offsetof(struct fi_opx_hfi1_sdma_header_vec, sys.req_info),
	offsetof(struct fi_opx_hfi1_sdma_header_vec, hmem.req_info)
};

#define OPX_SDMA_REQ_INFO_PTR(header_vec, set_meminfo)		\
	((struct sdma_req_info *) ((uintptr_t) (header_vec) + OPX_SDMA_REQ_INFO_OFFSET[(set_meminfo)]))

struct fi_opx_hfi1_sdma_packet {
	uint64_t				length;
	struct fi_opx_reliability_tx_replay	*replay;
};
struct fi_opx_hfi1_sdma_replay_packet {
	uint64_t				length;
	struct fi_opx_reliability_tx_replay	*replay;
	struct fi_opx_hfi1_sdma_header_vec	header_vec;
	struct hfi1_sdma_comp_entry		comp_entry; // 8 bytes
};

struct fi_opx_hfi1_sdma_work_entry {
	struct fi_opx_hfi1_sdma_work_entry	*next;
	struct fi_opx_completion_counter	*cc;
	union fi_opx_reliability_tx_psn		*psn_ptr;
	struct fi_opx_hfi1_sdma_header_vec	header_vec; // 208 bytes
	struct hfi1_sdma_comp_entry		comp_entry; // 8 bytes
	ssize_t					writev_rc;
	struct {
		int				device;
		enum fi_hmem_iface		iface;
	} hmem;
	enum hfi1_sdma_comp_state		comp_state;
	uint32_t				total_payload;
	uint32_t				num_packets;
	uint16_t				dlid;
	uint8_t 				num_iovs;
	uint8_t					rs;
	uint8_t					rx;
	bool					in_use;
	bool					use_bounce_buf;
	bool					pending_bounce_buf;
	struct fi_opx_hfi1_sdma_packet		packets[FI_OPX_HFI1_SDMA_MAX_PACKETS];
	struct iovec				iovecs[FI_OPX_HFI1_SDMA_WE_IOVS];
	struct {
		struct fi_opx_completion_counter cc;
		uint32_t			use_count;
		uint32_t			unused;
		uint8_t				buf[FI_OPX_HFI1_SDMA_WE_BUF_LEN];
	} bounce_buf;
};

struct fi_opx_hfi1_sdma_replay_work_entry {
	struct fi_opx_hfi1_sdma_replay_work_entry *next;
	ssize_t					writev_rc;
	enum hfi1_sdma_comp_state		comp_state;
	uint32_t				total_payload;
	uint32_t				num_packets;
	uint8_t 				num_iovs;
	bool					in_use;
	struct fi_opx_hfi1_sdma_replay_packet	packets[FI_OPX_HFI1_SDMA_MAX_PACKETS];
	struct iovec				iovecs[FI_OPX_HFI1_SDMA_REPLAY_WE_IOVS];
};

void fi_opx_hfi1_sdma_hit_zero(struct fi_opx_completion_counter *cc);
void fi_opx_hfi1_sdma_bounce_buf_hit_zero(struct fi_opx_completion_counter *cc);
void fi_opx_hfi1_sdma_handle_errors(struct fi_opx_ep *opx_ep,
				    struct fi_opx_hfi1_sdma_work_entry* we,
				    const char *file,
				    const char *func,
				    const int line);
void fi_opx_hfi1_sdma_replay_handle_errors(struct fi_opx_ep *opx_ep,
					   struct fi_opx_hfi1_sdma_replay_work_entry* we,
					   const char *file,
					   const char *func,
					   const int line);
int fi_opx_hfi1_dput_sdma_pending_completion(union fi_opx_hfi1_deferred_work *work);

__OPX_FORCE_INLINE__
bool fi_opx_hfi1_sdma_use_sdma(struct fi_opx_ep *opx_ep,
				uint64_t total_bytes,
				const uint32_t opcode,
				const uint64_t is_hmem,
				const bool is_intranode)
{
	/* This function should never be called for fence and error/truncation
	   opcodes. All other DPUT_OPCODEs are now supported for SDMA. */
	assert(opcode != FI_OPX_HFI_DPUT_OPCODE_FENCE &&
		opcode != FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC);

	return !is_intranode &&
		(is_hmem || total_bytes >= FI_OPX_SDMA_MIN_LENGTH) &&
		opx_ep->tx->use_sdma;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_init_cc(struct fi_opx_ep *opx_ep,
			      struct fi_opx_hfi1_dput_params *params,
			      const uint64_t length)
{
	struct fi_opx_completion_counter *cc = (struct fi_opx_completion_counter *) ofi_buf_alloc(opx_ep->rma_counter_pool);
	assert(cc);
	cc->next = NULL;
	cc->initial_byte_count = length;
	cc->byte_counter = length;
	cc->cq = NULL;
	cc->container = (void *)params;
	cc->cntr = NULL;
	cc->hit_zero = fi_opx_hfi1_sdma_hit_zero;
	params->cc = cc;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_dput_sdma_init(struct fi_opx_ep *opx_ep,
				struct fi_opx_hfi1_dput_params *params,
				const uint64_t length,
				const uint32_t tidoffset,
				const uint32_t ntidpairs,
				const uint32_t  *const tidpairs,
				const uint64_t is_hmem)
{
	if (!fi_opx_hfi1_sdma_use_sdma(opx_ep, length, params->opcode, is_hmem, params->is_intranode)) {
		return;
	}

	params->sdma_no_bounce_buf = (length >= opx_ep->tx->sdma_bounce_buf_threshold) ||
                            	(is_hmem) ||
                            	(params->opcode == FI_OPX_HFI_DPUT_OPCODE_GET) ||
                            	(params->opcode == FI_OPX_HFI_DPUT_OPCODE_PUT) ||
                            	(params->opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH) ||
                            	(params->opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH);

	if (!params->sdma_no_bounce_buf) {
		assert(params->origin_byte_counter);
	}
	params->user_cc = params->cc;
	fi_opx_hfi1_sdma_init_cc(opx_ep, params, length);

	slist_init(&params->sdma_reqs);

	params->sdma_we = NULL;
	params->sdma_reqs_used = 0;
	if (ntidpairs) {
		params->ntidpairs = ntidpairs;
		params->tidoffset = tidoffset;
		params->tididx = -1U;
		params->tid_iov.iov_len = ntidpairs * sizeof(uint32_t);
		params->tid_iov.iov_base = &params->tidpairs[0];
		for(int idx=0; idx < ntidpairs; idx++) {
			params->tidpairs[idx] = tidpairs[idx];
		}
		OPX_DEBUG_TIDS("CTS tid_iov",ntidpairs,params->tidpairs);

		params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma_tid;
	} else {
		params->work_elem.work_fn = fi_opx_hfi1_do_dput_sdma;
	}
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.total_requests);
}

__OPX_FORCE_INLINE__
bool fi_opx_hfi1_sdma_has_unsent_packets(struct fi_opx_hfi1_sdma_work_entry* we)
{
	return we->num_packets && we->comp_state != QUEUED;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_poll_completion(struct fi_opx_ep *opx_ep)
{
	struct fi_opx_hfi1_context *hfi = opx_ep->hfi;
	uint16_t queue_size = hfi->info.sdma.queue_size;

	while (hfi->info.sdma.available_counter < queue_size) {
		volatile struct hfi1_sdma_comp_entry * entry = &hfi->info.sdma.completion_queue[hfi->info.sdma.done_index];
		if (entry->status == QUEUED) {
			break;
		}

		// Update the status/errcode of the work entry who was using this index
		assert(hfi->info.sdma.queued_entries[hfi->info.sdma.done_index]);
		hfi->info.sdma.queued_entries[hfi->info.sdma.done_index]->status = entry->status;
		hfi->info.sdma.queued_entries[hfi->info.sdma.done_index]->errcode = entry->errcode;
		hfi->info.sdma.queued_entries[hfi->info.sdma.done_index] = NULL;

		assert(entry->status == COMPLETE || entry->status == FREE);
		++hfi->info.sdma.available_counter;
		hfi->info.sdma.done_index = (hfi->info.sdma.done_index + 1) % (queue_size);
		if (hfi->info.sdma.done_index == hfi->info.sdma.fill_index) {
			assert(hfi->info.sdma.available_counter == queue_size);
		}
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND DPUT SDMA POLL COMPLETE\n"); 
}

__OPX_FORCE_INLINE__
enum hfi1_sdma_comp_state fi_opx_hfi1_sdma_get_status(struct fi_opx_ep *opx_ep,
						      struct fi_opx_hfi1_sdma_work_entry *we)
{

	if (we->comp_state != QUEUED) {
		return we->comp_state;
	}

	if (OFI_UNLIKELY(we->comp_entry.status == ERROR)) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SDMA_WE (%p) -- Found error in queued entry, status=%d, error=%d\n",
			we, we->comp_entry.status, we->comp_entry.errcode);
		fi_opx_hfi1_sdma_handle_errors(opx_ep, we, __FILE__, __func__, __LINE__);
		we->comp_state = ERROR;
		return ERROR;
	}

	if (we->comp_entry.status == QUEUED) {
		return QUEUED;
	}

	assert(we->comp_entry.status == COMPLETE);
	assert(we->comp_entry.errcode == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SDMA_WE (%p)-- Finished worked.\n", we);

	we->comp_state = COMPLETE;

	return COMPLETE;
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_sdma_work_entry* fi_opx_hfi1_sdma_get_idle_we(struct fi_opx_ep *opx_ep)
{

	struct fi_opx_hfi1_sdma_work_entry *entry =
		(struct fi_opx_hfi1_sdma_work_entry *) ofi_buf_alloc(opx_ep->tx->sdma_work_pool);

	if (!entry) {
		return NULL;
	}

	entry->next = NULL;
	entry->comp_state = FREE;
	entry->num_iovs = 0;
	entry->num_packets = 0;
	entry->total_payload = 0;
	entry->writev_rc = 0;
	entry->psn_ptr = NULL;
	entry->in_use = true;
	entry->pending_bounce_buf = false;
	entry->bounce_buf.use_count = 0;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- giving WE %p\n", entry);
	return entry;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_return_we(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_work_entry* we) {
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- returned WE %p\n", we);
	assert(we->next == NULL);
	assert(we->in_use);
	we->in_use = false;

	OPX_BUF_FREE(we);
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_sdma_work_entry *opx_sdma_get_new_work_entry(struct fi_opx_ep *opx_ep,
								uint16_t *reqs_used,
								struct slist *sdma_reqs,
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
			return sdma_we;
		}
	}

	// No idle entries available, or we've already been allocated the max.
	// See if one of our existing entries is available for re-use.
	sdma_we = (struct fi_opx_hfi1_sdma_work_entry *) sdma_reqs->head;
	struct fi_opx_hfi1_sdma_work_entry *prev = NULL;

	while (sdma_we && sdma_we != current) {
		enum hfi1_sdma_comp_state sdma_status = fi_opx_hfi1_sdma_get_status(opx_ep, sdma_we);
		if (sdma_status == COMPLETE && !sdma_we->pending_bounce_buf) {
			slist_remove(sdma_reqs,
					(struct slist_entry *) sdma_we,
					(struct slist_entry *) prev);
			sdma_we->next = NULL;
			sdma_we->comp_state = FREE;
			sdma_we->num_iovs = 0;
			sdma_we->num_packets = 0;
			sdma_we->total_payload = 0;
			sdma_we->writev_rc = 0;
			sdma_we->psn_ptr = NULL;
			++sdma_we->bounce_buf.use_count;
			return sdma_we;
		}
		prev = sdma_we;
		sdma_we = sdma_we->next;
	}
	return NULL;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_init_we(struct fi_opx_hfi1_sdma_work_entry* we,
				struct fi_opx_completion_counter *cc,
				uint16_t dlid, uint8_t rs, uint8_t rx,
				enum fi_hmem_iface iface, int hmem_device)
{
	we->cc = cc;
	we->dlid = dlid;
	we->rs = rs;
	we->rx = rx;
	we->comp_entry.status = QUEUED;
	we->comp_entry.errcode = 0;
	we->hmem.iface = iface;
	we->hmem.device = hmem_device;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_add_packet(struct fi_opx_hfi1_sdma_work_entry *we,
				struct fi_opx_reliability_tx_replay *replay,
				uint64_t payload_bytes)
{
	assert(payload_bytes <= FI_OPX_HFI1_PACKET_MTU);
	assert(we->num_packets < FI_OPX_HFI1_SDMA_MAX_PACKETS);

	
	we->packets[we->num_packets].replay = replay;
	we->packets[we->num_packets].length = payload_bytes;
	we->num_packets++;

	we->total_payload += payload_bytes;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- Add_packet %u, payload_bytes=%ld, total_payload=%d\n", we->num_packets, payload_bytes, we->total_payload);
}

__OPX_FORCE_INLINE__
enum hfi1_sdma_comp_state fi_opx_hfi1_sdma_replay_get_status(struct fi_opx_ep *opx_ep,
						      struct fi_opx_hfi1_sdma_replay_work_entry *we)
{

	if (we->comp_state != QUEUED) {
		return we->comp_state;
	}

	for (int i = 0; i < we->num_packets; ++i) {
		if (OFI_UNLIKELY(we->packets[i].comp_entry.status == ERROR)) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== SDMA_WE -- Found error in queued entry, status=%d, error=%d\n",
				     we->packets[i].comp_entry.status, we->packets[i].comp_entry.errcode);
			fi_opx_hfi1_sdma_replay_handle_errors(opx_ep, we, __FILE__, __func__, __LINE__);
			we->comp_state = ERROR;
			return ERROR;
		}
		if (we->packets[i].comp_entry.status == QUEUED) {
			// Found one queued item, have to wait
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"===================================== SDMA_WE -- Found queued comp_entry at %d\n", i);
			return QUEUED;
		}
		assert(we->packets[i].comp_entry.status == COMPLETE);
		assert(we->packets[i].comp_entry.errcode == 0);
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SDMA_WE (%p)-- Finished worked.\n", we);

	we->comp_state = COMPLETE;

	return COMPLETE;
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_sdma_replay_work_entry* fi_opx_hfi1_sdma_replay_get_idle_we(struct fi_opx_ep *opx_ep)
{

	struct fi_opx_hfi1_sdma_replay_work_entry *entry =
		(struct fi_opx_hfi1_sdma_replay_work_entry *) ofi_buf_alloc(opx_ep->tx->sdma_replay_work_pool);

	if (!entry) {
		return NULL;
	}

	entry->next = NULL;
	entry->comp_state = FREE;
	entry->num_iovs = 0;
	entry->num_packets = 0;
	entry->total_payload = 0;
	entry->writev_rc = 0;
	entry->in_use = true;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- giving WE %p\n", entry);
	return entry;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_replay_return_we(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_sdma_replay_work_entry* we) {
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- returned WE %p\n", we);
	assert(we->next == NULL);
	assert(we->in_use);
	we->in_use = false;

	OPX_BUF_FREE(we);
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_set_meminfo(struct sdma_req_info *req_info,
				  const uint64_t set_meminfo,
				  const enum fi_hmem_iface iface,
				  const uint64_t device)
{
#ifdef OPX_HMEM
	if (set_meminfo) {
		// We currently only ever use one payload IOV that would require
		// setting meminfo, and it will be the fist one:
		// index 0 (the first payload IOV, or iov[1]).
		const unsigned meminfo_idx = 0;
		const unsigned type = OPX_HMEM_KERN_MEM_TYPE[iface];
		struct sdma_req_meminfo *meminfo = (struct sdma_req_meminfo *) (req_info + 1);
		meminfo->types = 0;
		HFI1_MEMINFO_TYPE_ENTRY_SET(meminfo->types, meminfo_idx, type);

		meminfo->context[0] = device; // meminfo_idx
		meminfo->context[1] = 0;
		meminfo->context[2] = 0;
		meminfo->context[3] = 0;
		meminfo->context[4] = 0;
		meminfo->context[5] = 0;
		meminfo->context[6] = 0;
		meminfo->context[7] = 0;
		meminfo->context[8] = 0;
		meminfo->context[9] = 0;
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
void fi_opx_hfi1_sdma_replay_add_packet(struct fi_opx_hfi1_sdma_replay_work_entry *we,
				struct fi_opx_reliability_tx_replay *replay,
				uint64_t payload_bytes)
{
	assert(payload_bytes <= FI_OPX_HFI1_PACKET_MTU);
	assert(we->num_packets < FI_OPX_HFI1_SDMA_MAX_PACKETS);


	const int index = we->num_packets;

	// The replay's hmem_iface would be set to system for managed hmem, so
	// we don't need to check for that here.
	const uint64_t set_meminfo = (replay->hmem_iface > FI_HMEM_SYSTEM) ? 1 : 0;

	struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&we->packets[index].header_vec, set_meminfo);
	fi_opx_hfi1_sdma_set_meminfo(req_info, set_meminfo, replay->hmem_iface, replay->hmem_device);

	req_info->ctrl = FI_OPX_HFI1_SDMA_REQ_HEADER_REPLAY_EAGER_FIXEDBITS | OPX_SDMA_REQ_SET_MEMINFO[set_meminfo];
	req_info->fragsize = (payload_bytes + 63) & 0xFFC0;
	req_info->npkts = 1;
	we->packets[index].header_vec.scb.qw0 = replay->scb.qw0;  //PBC_dws
	we->packets[index].header_vec.scb.hdr.qw[0] = replay->scb.hdr.qw[0];
	we->packets[index].header_vec.scb.hdr.qw[1] = replay->scb.hdr.qw[1];
	we->packets[index].header_vec.scb.hdr.qw[2] = replay->scb.hdr.qw[2];
	we->packets[index].header_vec.scb.hdr.qw[3] = replay->scb.hdr.qw[3];
	we->packets[index].header_vec.scb.hdr.qw[4] = replay->scb.hdr.qw[4];
	we->packets[index].header_vec.scb.hdr.qw[5] = replay->scb.hdr.qw[5];
	we->packets[index].header_vec.scb.hdr.qw[6] = replay->scb.hdr.qw[6];
	we->packets[index].replay = replay;
	we->packets[index].length = payload_bytes;
	we->packets[index].comp_entry.status = QUEUED;
	we->packets[index].comp_entry.errcode = 0;

	we->iovecs[we->num_iovs].iov_len = OPX_SDMA_REQ_HDR_SIZE[set_meminfo];
	we->iovecs[we->num_iovs].iov_base = req_info;
	we->num_iovs++;

	we->iovecs[we->num_iovs].iov_len = payload_bytes;
	assert(replay->use_iov);
	assert(replay->iov->iov_len == payload_bytes);
	we->iovecs[we->num_iovs].iov_base = replay->iov->iov_base;
	we->num_iovs++;

	we->num_packets++;

	we->total_payload += payload_bytes;

	assert(we->num_iovs <= FI_OPX_HFI1_SDMA_REPLAY_WE_IOVS);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- Add_packet %u, payload_bytes=%ld, total_payload=%d\n", we->num_packets, payload_bytes, we->total_payload);
}


__OPX_FORCE_INLINE__
void fi_opx_hfi1_sdma_do_sdma_replay(struct fi_opx_ep *opx_ep,
				struct fi_opx_hfi1_sdma_replay_work_entry *we)
{
	assert(we->comp_state == FREE);
	assert(we->num_packets > 0);
	assert(opx_ep->hfi->info.sdma.available_counter >= we->num_packets);
	uint16_t *avail = &opx_ep->hfi->info.sdma.available_counter;
	assert((*avail) >= we->num_packets);
	uint16_t *fill_index = &opx_ep->hfi->info.sdma.fill_index;

	for (int i = 0; i < we->num_packets; ++i) {
		assert((((*avail) < opx_ep->hfi->info.sdma.queue_size) &&
			(*fill_index) != opx_ep->hfi->info.sdma.done_index) ||
		       (((*avail) == opx_ep->hfi->info.sdma.queue_size) &&
			(*fill_index) == opx_ep->hfi->info.sdma.done_index));

		assert(opx_ep->hfi->info.sdma.completion_queue[*fill_index].status == FREE ||
		       opx_ep->hfi->info.sdma.completion_queue[*fill_index].status == COMPLETE);

		const uint64_t set_meminfo = (we->packets[i].replay->hmem_iface == FI_HMEM_SYSTEM) ? 0 : 1;
		struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&we->packets[i].header_vec, set_meminfo);
		req_info->comp_idx = *fill_index;
		assert(opx_ep->hfi->info.sdma.queued_entries[*fill_index] == NULL);
		opx_ep->hfi->info.sdma.queued_entries[*fill_index] = &we->packets[i].comp_entry;
		*fill_index = ((*fill_index) + 1) % (opx_ep->hfi->info.sdma.queue_size);
		--(*avail);
	}

	//rc will be the number of packets being processed concurrently by SDMA, or -1 if error
	ssize_t rc = writev(opx_ep->hfi->fd, we->iovecs, we->num_iovs);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SDMA_WE -- called writev rc=%ld  Params were: fd=%d iovecs=%p num_iovs=%d \n", rc, opx_ep->hfi->fd, we->iovecs, we->num_iovs);

	we->writev_rc = rc;
	if (rc > 0) {
		assert (rc == we->num_packets);
		we->comp_state = QUEUED;
	} else {
		we->comp_state = ERROR;
		fi_opx_hfi1_sdma_replay_handle_errors(opx_ep, we, __FILE__, __func__, __LINE__);
	}
}

__OPX_FORCE_INLINE__
void opx_hfi1_sdma_do_sdma(struct fi_opx_ep *opx_ep,
			      struct fi_opx_hfi1_sdma_work_entry *we,
			      enum ofi_reliability_kind reliability)
{
	assert(we->comp_state == FREE);
	assert(we->num_packets > 0);
	assert(opx_ep->hfi->info.sdma.available_counter > 0);

	const uint64_t set_meminfo =
			#ifdef OPX_HMEM
				(we->hmem.iface > FI_HMEM_SYSTEM) ? 1 : 0;
			#else
				0;
			#endif

	struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&we->header_vec, set_meminfo);
	fi_opx_hfi1_sdma_set_meminfo(req_info, set_meminfo, we->hmem.iface, we->hmem.device);

	int32_t psn;
	unsigned short fragsize = 0; /* packet length varies, track the largest */

	/* Since we already verified that enough PSNs were available for
	   the send we're about to do, we shouldn't need to check the
	   returned PSN here before proceeding */
	psn = fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
					     &opx_ep->reliability->state,
					     we->dlid, we->rx, we->rs,
					     &we->psn_ptr, we->num_packets);

	we->num_iovs = 2; /* request and data */
	we->iovecs[1].iov_len = (we->total_payload + 3) & -4;
	we->iovecs[1].iov_base = we->packets[0].replay->iov[0].iov_base;

	struct fi_opx_completion_counter *cc;
	void *replay_back_ptr;
	if (we->use_bounce_buf) {
		we->bounce_buf.cc.initial_byte_count = we->total_payload;
		we->bounce_buf.cc.byte_counter = we->total_payload;
		we->bounce_buf.cc.container = (void *)we;
		we->bounce_buf.cc.next = we->cc;
		we->bounce_buf.cc.hit_zero = fi_opx_hfi1_sdma_bounce_buf_hit_zero;
		cc = &we->bounce_buf.cc;
		we->pending_bounce_buf = true;
		replay_back_ptr = (void *)we;
	} else {
		cc = we->cc;
		replay_back_ptr = NULL;
	}

	for (int i = 0; i < we->num_packets; ++i) {
		fragsize = MAX(fragsize, we->packets[i].length);
		we->packets[i].replay->scb.hdr.qw[2] |= (uint64_t)htonl((uint32_t)psn);
		we->packets[i].replay->sdma_we_use_count = we->bounce_buf.use_count;
		we->packets[i].replay->sdma_we = replay_back_ptr;
		we->packets[i].replay->hmem_iface = we->hmem.iface;
		we->packets[i].replay->hmem_device = we->hmem.device;
		fi_opx_reliability_client_replay_register_with_update(
			&opx_ep->reliability->state, we->dlid, we->rs, we->rx,
			we->psn_ptr, we->packets[i].replay, cc,
			we->packets[i].length, reliability);
		psn = (psn + 1) & MAX_PSN;
	}

	/* Frag size must be a multiple of 64. Round up if it's not already */
	fragsize = (fragsize + 63) & 0xFFC0;
	req_info->ctrl = FI_OPX_HFI1_SDMA_REQ_HEADER_EAGER_FIXEDBITS |
			(((uint16_t)we->num_iovs) << HFI1_SDMA_REQ_IOVCNT_SHIFT) |
			OPX_SDMA_REQ_SET_MEMINFO[set_meminfo];

	uint16_t *fill_index = &opx_ep->hfi->info.sdma.fill_index;
	uint64_t last_packet_bytes = we->packets[we->num_packets - 1].length;

	req_info->npkts = we->num_packets;
	req_info->comp_idx = *fill_index;
	req_info->fragsize = fragsize;

	we->header_vec.scb = we->packets[0].replay->scb;
	we->header_vec.scb.hdr.qw[4] |= (last_packet_bytes << 32);

	we->iovecs[0].iov_len = OPX_SDMA_REQ_HDR_SIZE[set_meminfo];
	we->iovecs[0].iov_base = req_info;

	opx_ep->hfi->info.sdma.queued_entries[*fill_index] = &we->comp_entry;
	*fill_index = ((*fill_index) + 1) % (opx_ep->hfi->info.sdma.queue_size);
	--opx_ep->hfi->info.sdma.available_counter;

	FI_OPX_DEBUG_COUNTERS_INC(
		opx_ep->debug_counters.sdma.writev_calls[we->num_packets]);
	ssize_t rc = writev(opx_ep->hfi->fd, we->iovecs, we->num_iovs);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== SDMA_WE -- called writev rc=%ld  Params were: fd=%d iovecs=%p num_iovs=%d \n",
	       rc, opx_ep->hfi->fd, we->iovecs, we->num_iovs);

	we->writev_rc = rc;
	if (rc > 0) {
		we->comp_state = QUEUED;
	} else {
		we->comp_state = ERROR;
		fi_opx_hfi1_sdma_handle_errors(opx_ep, we, __FILE__, __func__, __LINE__);
	}
}

__OPX_FORCE_INLINE__
void opx_hfi1_sdma_do_sdma_tid(struct fi_opx_ep *opx_ep,
				  struct fi_opx_hfi1_sdma_work_entry *we,
				  struct iovec *tid_iov, /* not an array */
				  uint32_t tid_idx, uint32_t tidOMshift,
				  uint32_t tidoffset,
				  enum ofi_reliability_kind reliability)
{
	assert(we->comp_state == FREE);
	assert(we->num_packets > 0);
	assert(opx_ep->hfi->info.sdma.available_counter > 0);

	int32_t psn;
	unsigned short fragsize = 0; /* packet length varies, track the largest */
	unsigned int tidiovec_idx = 2; /* tid info iovec*/
	uint32_t *tidpairs = NULL;

	const uint64_t set_meminfo =
			#ifdef OPX_HMEM
				(we->hmem.iface > FI_HMEM_SYSTEM) ? 1 : 0;
			#else
				0;
			#endif

	struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&we->header_vec, set_meminfo);
	fi_opx_hfi1_sdma_set_meminfo(req_info, set_meminfo, we->hmem.iface, we->hmem.device);

	/* Since we already verified that enough PSNs were available for
	   the send we're about to do, we shouldn't need to check the
	   returned PSN here before proceeding */
	psn = fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
					     &opx_ep->reliability->state,
					     we->dlid, we->rx, we->rs,
					     &we->psn_ptr, we->num_packets);

	we->num_iovs = 3; /* request and data and tids*/
	/* no padding for tid, should have been aligned.*/
	assert(we->total_payload == ((we->total_payload) & -4));

	we->iovecs[1].iov_len = (we->total_payload + 3) & -4;
	we->iovecs[1].iov_base = we->packets[0].replay->iov[0].iov_base;

	struct fi_opx_completion_counter *cc;
	void *replay_back_ptr;
	if (we->use_bounce_buf) {
		we->bounce_buf.cc.initial_byte_count = we->total_payload;
		we->bounce_buf.cc.byte_counter = we->total_payload;
		we->bounce_buf.cc.container = (void *) we;
		we->bounce_buf.cc.next = we->cc;
		we->bounce_buf.cc.hit_zero = fi_opx_hfi1_sdma_bounce_buf_hit_zero;
		cc = &we->bounce_buf.cc;
		we->pending_bounce_buf = true;
		replay_back_ptr = (void *) we;
	} else {
		cc = we->cc;
		replay_back_ptr = NULL;
	}

	for (int i = 0; i < we->num_packets; ++i) {
		fragsize = MAX(fragsize, we->packets[i].length);
		we->packets[i].replay->scb.hdr.qw[2] |= (uint64_t)htonl((uint32_t)psn);
		we->packets[i].replay->sdma_we_use_count = we->bounce_buf.use_count;
		we->packets[i].replay->sdma_we = replay_back_ptr;
		we->packets[i].replay->hmem_iface = we->hmem.iface;
		we->packets[i].replay->hmem_device = we->hmem.device;
		fi_opx_reliability_client_replay_register_with_update(
			&opx_ep->reliability->state, we->dlid, we->rs, we->rx,
			we->psn_ptr, we->packets[i].replay, cc,
			we->packets[i].length, reliability);
		psn = (psn + 1) & MAX_PSN;
	}

	/* tid packet lengths should have been aligned.*/
	assert(fragsize == ((fragsize + 63) & 0xFFC0));
	assert(tid_idx < tid_iov->iov_len / sizeof(uint32_t));
	tidpairs = (uint32_t *)tid_iov->iov_base;
	we->iovecs[tidiovec_idx].iov_len = tid_iov->iov_len - (tid_idx * sizeof(uint32_t));
	we->iovecs[tidiovec_idx].iov_base = &tidpairs[tid_idx];
	req_info->ctrl = FI_OPX_HFI1_SDMA_REQ_HEADER_EXPECTED_FIXEDBITS |
			(((uint16_t)we->num_iovs) << HFI1_SDMA_REQ_IOVCNT_SHIFT) |
			OPX_SDMA_REQ_SET_MEMINFO[set_meminfo];

	uint32_t tidpair = tidpairs[tid_idx];
	uint32_t kdeth = (FI_OPX_HFI1_KDETH_TIDCTRL & FI_OPX_EXP_TID_GET((tidpair), CTRL))
			 << FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT;
	kdeth |= (FI_OPX_HFI1_KDETH_TID & FI_OPX_EXP_TID_GET((tidpair), IDX))
		 << FI_OPX_HFI1_KDETH_TID_SHIFT;
	kdeth |= tidOMshift;
	kdeth |= tidoffset;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "kdeth %#X, tid    [%u]=%#8.8X LEN %u, CTRL %u, IDX %u, offset %#X %#X\n",
	       kdeth, 0, tidpair, (int)FI_OPX_EXP_TID_GET((tidpair), LEN),
	       (int)FI_OPX_EXP_TID_GET((tidpair), CTRL),
	       (int)FI_OPX_EXP_TID_GET((tidpair), IDX), tidoffset,
	       tidOMshift ? tidoffset << KDETH_OM_LARGE_SHIFT :
				  tidoffset << KDETH_OM_SMALL_SHIFT);

	uint16_t *fill_index = &opx_ep->hfi->info.sdma.fill_index;
	uint64_t last_packet_bytes = we->packets[we->num_packets-1].length;

	req_info->npkts = we->num_packets;
	req_info->comp_idx = *fill_index;
	req_info->fragsize = fragsize;

	we->header_vec.scb.qw0 = we->packets[0].replay->scb.qw0;  //PBC_dws
	we->header_vec.scb.hdr.qw[0] = we->packets[0].replay->scb.hdr.qw[0];
	we->header_vec.scb.hdr.qw[1] = we->packets[0].replay->scb.hdr.qw[1];
	we->header_vec.scb.hdr.qw[2] = we->packets[0].replay->scb.hdr.qw[2] | ((uint64_t)kdeth << 32);
	we->header_vec.scb.hdr.qw[3] = we->packets[0].replay->scb.hdr.qw[3];
	we->header_vec.scb.hdr.qw[4] = we->packets[0].replay->scb.hdr.qw[4] | (last_packet_bytes << 32);
	we->header_vec.scb.hdr.qw[5] = we->packets[0].replay->scb.hdr.qw[5];
	we->header_vec.scb.hdr.qw[6] = we->packets[0].replay->scb.hdr.qw[6];

	we->iovecs[0].iov_len = OPX_SDMA_REQ_HDR_SIZE[set_meminfo];
	we->iovecs[0].iov_base = req_info;

	opx_ep->hfi->info.sdma.queued_entries[*fill_index] = &we->comp_entry;
	*fill_index = ((*fill_index) + 1) % (opx_ep->hfi->info.sdma.queue_size);
	--opx_ep->hfi->info.sdma.available_counter;

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.writev_calls[we->num_packets]);
	ssize_t rc = writev(opx_ep->hfi->fd, we->iovecs, we->num_iovs);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== SDMA_WE -- called writev rc=%ld  Params were: fd=%d iovecs=%p num_iovs=%d \n",
	       rc, opx_ep->hfi->fd, we->iovecs, we->num_iovs);

	we->writev_rc = rc;
	if (rc > 0) {
		we->comp_state = QUEUED;
	} else {
		we->comp_state = ERROR;
		fi_opx_hfi1_sdma_handle_errors(opx_ep, we, __FILE__, __func__, __LINE__);
	}
}

__OPX_FORCE_INLINE__
void opx_hfi1_sdma_flush(struct fi_opx_ep *opx_ep,
			 struct fi_opx_hfi1_sdma_work_entry *we,
			 struct slist *sdma_reqs,
			 const bool use_tid,
			 struct iovec *tid_iov,
			 uint32_t tid_idx, uint32_t tidOMshift,
			 uint32_t tidoffset,
			 enum ofi_reliability_kind reliability)
{
	if (use_tid) {
		opx_hfi1_sdma_do_sdma_tid(opx_ep, we, tid_iov, tid_idx, tidOMshift,
					     tidoffset, reliability);
	} else {
		opx_hfi1_sdma_do_sdma(opx_ep, we, reliability);
	}

	assert(we->next == NULL);
	slist_insert_tail((struct slist_entry *)we, sdma_reqs);
}

#endif /* _FI_OPX_HFI1_SDMA_H_ */
