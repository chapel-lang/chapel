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

#include <assert.h>
#include <string.h>
#include "rdma/opx/fi_opx_hfi1_sdma.h"

void fi_opx_hfi1_sdma_hit_zero(struct fi_opx_completion_counter *cc)
{
	assert(cc->byte_counter == 0);
	struct fi_opx_hfi1_dput_params *params = (struct fi_opx_hfi1_dput_params *) cc->container;
	if (params->work_elem.complete) {
		FI_WARN(&fi_opx_provider, FI_LOG_EP_DATA,
			"SDMA Work Entry hit zero more than once! cc->byte_counter = %ld\n", cc->byte_counter);
		assert(0);
		return;
	}

	assert(params->sdma_we == NULL || !fi_opx_hfi1_sdma_has_unsent_packets(params->sdma_we));

	assert(!cc->next);

	// Set the work element to complete so it can be removed from the work pending queue and freed
	params->work_elem.complete = true;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== SDMA HIT ZERO\n");
}

void fi_opx_hfi1_sdma_bounce_buf_hit_zero(struct fi_opx_completion_counter *cc)
{
	assert(cc->byte_counter == 0);
	struct fi_opx_hfi1_sdma_work_entry *sdma_we = (struct fi_opx_hfi1_sdma_work_entry *) cc->container;
	assert(sdma_we->pending_bounce_buf);
	sdma_we->pending_bounce_buf = false;

	if (cc->next) {
		assert(cc->next->byte_counter >= cc->initial_byte_count);
		cc->next->byte_counter -= cc->initial_byte_count;
		if (cc->next->byte_counter == 0) {
			cc->next->hit_zero(cc->next);
		}
		cc->next = NULL;
	}
}

int fi_opx_hfi1_dput_sdma_pending_completion(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_dput_params *params = &work->dput;
	struct fi_opx_ep * opx_ep = params->opx_ep;

	assert(params->work_elem.low_priority);

	fi_opx_hfi1_sdma_poll_completion(opx_ep);

	struct fi_opx_hfi1_sdma_work_entry *we = (struct fi_opx_hfi1_sdma_work_entry *) params->sdma_reqs.head;
	while (we) {
		// If we're using the SDMA WE bounce buffer, we need to wait for
		// the hit_zero to mark the work element as complete. The replay
		// iovecs are pointing to the SDMA WE bounce buffers, so we can't
		// free the SDMA WEs until the replays are cleared.
		if (!params->work_elem.complete && we->use_bounce_buf) {
			FI_OPX_DEBUG_COUNTERS_INC(work->dput.opx_ep->debug_counters.sdma.eagain_pending_dc);
			return -FI_EAGAIN;
		}
		enum hfi1_sdma_comp_state we_status = fi_opx_hfi1_sdma_get_status(opx_ep, we);
		if (we_status == QUEUED) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_pending_writev);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
			return -FI_EAGAIN;
		}
		assert(we_status == COMPLETE);

		slist_remove_head(&params->sdma_reqs);
		we->next = NULL;
		fi_opx_hfi1_sdma_return_we(opx_ep, we);
		we = (struct fi_opx_hfi1_sdma_work_entry *) params->sdma_reqs.head;
	}

	assert(slist_empty(&params->sdma_reqs));

	if (!params->work_elem.complete) {
		assert(params->sdma_no_bounce_buf);
		FI_OPX_DEBUG_COUNTERS_INC(work->dput.opx_ep->debug_counters.sdma.eagain_pending_dc);
		return -FI_EAGAIN;
	}

	if (params->origin_byte_counter) {
		// If we're not doing delivery_competion, then origin_byte_counter
		// should have already been zero'd and NULL'd at the end of do_dput_sdma(...)
		assert(params->sdma_no_bounce_buf);
		*params->origin_byte_counter = 0;
		params->origin_byte_counter = NULL;
	}

	if (params->user_cc) {
		assert(params->user_cc->byte_counter >= params->cc->initial_byte_count);
		params->user_cc->byte_counter -= params->cc->initial_byte_count;
		if (params->user_cc->byte_counter == 0) {
			params->user_cc->hit_zero(params->user_cc);
		}
	}
	OPX_BUF_FREE(params->cc);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== PENDING DPUT %u COMPLETE\n", work->work_elem.complete);
	return FI_SUCCESS;
}

void fi_opx_hfi1_sdma_handle_errors(struct fi_opx_ep *opx_ep,
				    struct fi_opx_hfi1_sdma_work_entry* we,
				    const char *file,
				    const char *func,
				    const int line)
{
	const pid_t pid = getpid();

	fprintf(stderr, "(%d) %s:%s():%d ERROR: SDMA Abort errno=%d (%s)\n",
		pid, file, func, line, errno, strerror(errno));
	fprintf(stderr, "(%d) ===================================== SDMA_WE -- "
			"called writev rc=%ld, num_pkts=%u Params were: "
			"fd=%d iovecs=%p num_iovs=%d \n",
		pid, we->writev_rc, we->num_packets, opx_ep->hfi->fd, we->iovecs, we->num_iovs);
	fprintf(stderr, "(%d) hfi->info.sdma.queue_size == %0hu\n", pid, opx_ep->hfi->info.sdma.queue_size);
	fprintf(stderr, "(%d) hfi->info.sdma.fill_index == %0hu\n", pid, opx_ep->hfi->info.sdma.fill_index);
	fprintf(stderr, "(%d) hfi->info.sdma.done_index == %0hu\n", pid, opx_ep->hfi->info.sdma.done_index);
	fprintf(stderr, "(%d) hfi->info.sdma.available  == %0hu\n", pid, opx_ep->hfi->info.sdma.available_counter);
	fprintf(stderr, "(%d) hfi->info.sdma.completion_queue == %p\n", pid, opx_ep->hfi->info.sdma.completion_queue);
	volatile struct hfi1_sdma_comp_entry * entry = opx_ep->hfi->info.sdma.completion_queue;

	const uint64_t meminfo_set = (we->hmem.iface == FI_HMEM_SYSTEM) ? 0 : 1;
	struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&we->header_vec, meminfo_set);

	fprintf(stderr, "(%d) we->header_vec.npkts=%hu, fragsize=%hu, cmp_idx=%hu, ctrl=%#04hX, status=%#0X, errCode=%#0X\n",
		pid,
		req_info->npkts,
		req_info->fragsize,
		req_info->comp_idx,
		req_info->ctrl,
		entry[req_info->comp_idx].status,
		entry[req_info->comp_idx].errcode);

#ifdef OPX_HMEM
	if (meminfo_set) {
		struct sdma_req_meminfo *meminfo = (struct sdma_req_meminfo *) (req_info + 1);
		fprintf(stderr, "(%d) we->hmem.iface=%u we->hmem.device=%d meminfo->types=%#16.16llX meminfo->context[0]=%#16.16llX meminfo->context[15]=%#16.16llX\n",
			pid,
			we->hmem.iface,
			we->hmem.device,
			meminfo->types,
			meminfo->context[0],
			meminfo->context[15]);
	}
#endif

	// additional check against FI_OPX_HFI1_SDMA_WE_IOVS inserted to address Coverity defect
	for (int i = 0; i < we->num_iovs && i < FI_OPX_HFI1_SDMA_WE_IOVS; i++) {
		fprintf(stderr, "(%d) we->iovecs[%d].base = %p, len = %lu\n", pid, i, we->iovecs[i].iov_base, we->iovecs[i].iov_len);
		if (we->hmem.iface == FI_HMEM_SYSTEM || i == 0) {
			fprintf(stderr, "(%d) First 8 bytes of %p == %#16.16lX\n", pid, we->iovecs[i].iov_base, *((uint64_t *) we->iovecs[i].iov_base));
		} else {
			uint64_t first_qw;
			ofi_copy_from_hmem(we->hmem.iface, we->hmem.device, &first_qw, we->iovecs[i].iov_base, sizeof(uint64_t));
			fprintf(stderr, "(%d) First 8 bytes of %p == %#16.16lX\n", pid, we->iovecs[i].iov_base, first_qw);
		}
		if (i == 2) { /* assume tid iov */
			uint32_t *tidpairs = (uint32_t*)we->iovecs[i].iov_base;
			for (int j = 0; j < we->iovecs[i].iov_len/sizeof(uint32_t); ++j ) {
				fprintf(stderr, "(%d) tid    [%u]=%#8.8X LEN %u, CTRL %u, IDX %u\n",pid,
					j,
					tidpairs[j],
					(int)FI_OPX_EXP_TID_GET((tidpairs[j]),LEN),
					(int)FI_OPX_EXP_TID_GET((tidpairs[j]),CTRL),
					(int)FI_OPX_EXP_TID_GET((tidpairs[j]),IDX));
			}
		}
	}

	fprintf(stderr, "(%d) PBC: %#16.16lX\n", pid, we->header_vec.scb.qw0);
#ifndef NDEBUG
	fi_opx_hfi1_dump_packet_hdr(&we->header_vec.scb.hdr, "fi_opx_hfi1_sdma_handle_errors", 92);
#endif

	FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "SDMA Error, not handled, aborting\n");
	abort();
}

void fi_opx_hfi1_sdma_replay_handle_errors(struct fi_opx_ep *opx_ep,
					   struct fi_opx_hfi1_sdma_replay_work_entry* we,
					   const char *file,
					   const char *func,
					   const int line)
{
	const pid_t pid = getpid();

	fprintf(stderr, "(%d) %s:%s():%d ERROR: SDMA Abort errno=%d (%s)\n",
		pid, file, func, line, errno, strerror(errno));
	fprintf(stderr, "(%d) ===================================== SDMA_WE -- "
			"called writev rc=%ld, num_pkts=%u Params were: fd=%d iovecs=%p num_iovs=%d\n",
		pid, we->writev_rc, we->num_packets, opx_ep->hfi->fd, we->iovecs, we->num_iovs);
	fprintf(stderr, "(%d) hfi->info.sdma.queue_size == %0hu\n", pid, opx_ep->hfi->info.sdma.queue_size);
	fprintf(stderr, "(%d) hfi->info.sdma.fill_index == %0hu\n", pid, opx_ep->hfi->info.sdma.fill_index);
	fprintf(stderr, "(%d) hfi->info.sdma.done_index == %0hu\n", pid, opx_ep->hfi->info.sdma.done_index);
	fprintf(stderr, "(%d) hfi->info.sdma.available  == %0hu\n", pid, opx_ep->hfi->info.sdma.available_counter);
	fprintf(stderr, "(%d) hfi->info.sdma.completion_queue == %p\n", pid, opx_ep->hfi->info.sdma.completion_queue);
	for (int i = 0; i < we->num_iovs; i++) {
		fprintf(stderr, "(%d) we->iovecs[%d].base = %p, len = %lu\n",
			pid, i, we->iovecs[i].iov_base, we->iovecs[i].iov_len);
	}

	volatile struct hfi1_sdma_comp_entry * entry = opx_ep->hfi->info.sdma.completion_queue;
	for (int i = 0; i < we->num_packets; ++ i) {
		struct fi_opx_reliability_tx_replay *replay = we->packets[i].replay;
		const uint64_t meminfo_set = (replay->hmem_iface == FI_HMEM_SYSTEM) ? 0 : 1;
		struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&we->packets[i].header_vec, meminfo_set);

		fprintf(stderr, "(%d) packet[%u/%u], req_info=%p PBC: %#16.16lX, npkts=%hu, fragsize=%hu, "
				"cmp_idx=%hu, ctrl=%#04hX, meminfo set=%lu, status=%#0X, errCode=%#0X\n",
			pid, i, we->num_packets,
			req_info,
			we->packets[i].header_vec.scb.qw0,
			req_info->npkts,
			req_info->fragsize,
			req_info->comp_idx,
			req_info->ctrl,
			meminfo_set,
			entry[req_info->comp_idx].status,
			entry[req_info->comp_idx].errcode);

#ifdef OPX_HMEM
		if (meminfo_set) {
			struct sdma_req_meminfo *meminfo = (struct sdma_req_meminfo *) (req_info + 1);
			fprintf(stderr, "(%d) replay->hmem_iface=%u replay->hmem_device=%lu "
					"meminfo->types=%#16.16llX meminfo->context[0]=%#16.16llX "
					"meminfo->context[15]=%#16.16llX\n",
				pid,
				replay->hmem_iface,
				replay->hmem_device,
				meminfo->types,
				meminfo->context[0],
				meminfo->context[15]);
		}
#endif
#ifndef NDEBUG
		fi_opx_hfi1_dump_packet_hdr(&we->packets[i].header_vec.scb.hdr, func, line);

		unsigned payload_idx = (i * 2) + 1;
		if (replay->use_iov) {
			fprintf(stderr, "(%d) replay->use_iov = true, replay->iov->base = %p, "
					"len = %lu, we->iovecs[%d].iov_base = %p, %lu\n",
					pid, replay->iov->iov_base, replay->iov->iov_len,
					payload_idx,
					we->iovecs[payload_idx].iov_base,
					we->iovecs[payload_idx].iov_len);
			assert(replay->iov->iov_base == we->iovecs[payload_idx].iov_base);
			assert(replay->iov->iov_len == we->iovecs[payload_idx].iov_len);
			uint64_t first_qw;
			if (meminfo_set) {
				ofi_copy_from_hmem(replay->hmem_iface, replay->hmem_device,
						   &first_qw, replay->iov->iov_base, sizeof(uint64_t));
			} else {
				first_qw = *((uint64_t *) replay->iov->iov_base);
			}
			fprintf(stderr, "(%d) First 8 bytes of %p == %#16.16lX\n",
				pid, replay->iov->iov_base, first_qw);
		} else {
			fprintf(stderr, "(%d) replay->use_iov = false, replay->payload = %p\n",
					pid, replay->payload);
			fprintf(stderr, "(%d) First 8 bytes of %p == %#16.16lX\n",
				pid, replay->payload, *((uint64_t *) replay->payload));
		}
#endif
	}

	FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "SDMA Error, not handled, aborting\n");
	abort();
}

