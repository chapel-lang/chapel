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
	struct fi_opx_ep	       *opx_ep = params->opx_ep;

	assert(params->work_elem.work_type == OPX_WORK_TYPE_LAST);

	struct fi_opx_hfi1_sdma_work_entry *we = (struct fi_opx_hfi1_sdma_work_entry *) params->sdma_reqs.head;
	while (we) {
		// If we're using the SDMA WE bounce buffer, we need to wait for
		// the hit_zero to mark the work element as complete. The replay
		// iovecs are pointing to the SDMA WE bounce buffers, so we can't
		// free the SDMA WEs until the replays are cleared.
		if (we->comp_state == OPX_SDMA_COMP_PENDING_WRITEV) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_pending_writev);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
			return -FI_EAGAIN;
		} else if (we->comp_state == OPX_SDMA_COMP_QUEUED) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_pending_sdma_completion);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
			return -FI_EAGAIN;
		} else if (!params->work_elem.complete && we->use_bounce_buf) {
			FI_OPX_DEBUG_COUNTERS_INC(work->dput.opx_ep->debug_counters.sdma.eagain_pending_dc);
			return -FI_EAGAIN;
		}
		assert(we->comp_state == OPX_SDMA_COMP_COMPLETE || we->comp_state == OPX_SDMA_COMP_ERROR);

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
		assert((*params->origin_byte_counter) >= params->origin_bytes_sent);
		*params->origin_byte_counter -= params->origin_bytes_sent;
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

void fi_opx_hfi1_sdma_handle_errors(struct fi_opx_ep *opx_ep, int writev_rc, struct iovec *iovs, const int num_iovs,
				    const char *file, const char *func, const int line)
{
	const pid_t pid = getpid();

	if (errno == ECOMM || errno == EINTR || errno == EFAULT) {
		int err =
			fi_opx_context_check_status(opx_ep->hfi, OPX_SW_HFI1_TYPE, opx_ep, OPX_IS_CTX_SHARING_ENABLED);
		if (err != FI_SUCCESS) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Link down detected\n");
			return;
		}
	}

	fprintf(stderr, "(%d) %s:%s():%d ERROR: SDMA Abort errno=%d (%s)\n", pid, file, func, line, errno,
		strerror(errno));
	fprintf(stderr,
		"(%d) ===================================== SDMA_WE -- "
		"called writev rc=%d Params were: "
		"fd=%d iovecs=%p num_iovs=%d \n",
		pid, writev_rc, opx_ep->hfi->fd_cdev, iovs, num_iovs);
	fprintf(stderr, "(%d) hfi->info.sdma.queue_size == %0hu\n", pid, opx_ep->hfi->info.sdma.queue_size);
	fprintf(stderr, "(%d) hfi->info.sdma.fill_index == %0hu\n", pid, opx_ep->hfi->info.sdma.fill_index);
	fprintf(stderr, "(%d) hfi->info.sdma.done_index == %0hu\n", pid, opx_ep->hfi->info.sdma.done_index);
	fprintf(stderr, "(%d) hfi->info.sdma.available  == %0hu\n", pid, opx_ep->hfi->info.sdma.available_counter);
	fprintf(stderr, "(%d) hfi->info.sdma.completion_queue == %p\n", pid, opx_ep->hfi->info.sdma.completion_queue);

	struct iovec *iov_ptr = iovs;
	int	      req_num = 0;

	do {
		struct fi_opx_hfi1_sdma_header_vec *header_vec =
			(struct fi_opx_hfi1_sdma_header_vec *) iov_ptr[0].iov_base;
		size_t		      header_vec_len = iov_ptr[0].iov_len;
		uint8_t		      meminfo_set    = (header_vec_len == OPX_SDMA_REQ_HDR_SIZE[1]) ? 1 : 0;
		struct sdma_req_info *req_info	     = OPX_SDMA_REQ_INFO_PTR(header_vec, meminfo_set);
		uint8_t req_info_iovs = (req_info->ctrl >> HFI1_SDMA_REQ_IOVCNT_SHIFT) & HFI1_SDMA_REQ_IOVCNT_MASK;
		uint8_t req_info_meminfo_set =
#ifdef OPX_HMEM
			(req_info->ctrl >> HFI1_SDMA_REQ_MEMINFO_SHIFT) & HFI1_SDMA_REQ_MEMINFO_MASK;
#else
			0;
#endif
		uint8_t req_info_opcode = (req_info->ctrl >> HFI1_SDMA_REQ_OPCODE_SHIFT) & HFI1_SDMA_REQ_OPCODE_MASK;

		fprintf(stderr,
			"(%d) [%d] header_vec=%p len=%lu meminfo_set=%hhu req_meminfo_set=%hhu req_iovs=%hhu req_opcode=%hhu/(%s)\n",
			pid, req_num, header_vec, header_vec_len, meminfo_set, req_info_meminfo_set, req_info_iovs,
			req_info_opcode, (req_info_opcode == EAGER) ? "EAGER (non-TID)" : "EXPECTED (TID)");
		fprintf(stderr, "(%d) [%d]    req.npkts=%hu req.fragsize=%hu req.cmp_idx=%hu req.ctrl=%#04hX\n", pid,
			req_num, req_info->npkts, req_info->fragsize, req_info->comp_idx, req_info->ctrl);

		enum fi_hmem_iface hmem_iface  = FI_HMEM_SYSTEM;
		uint64_t	   hmem_device = 0ul;
#ifdef OPX_HMEM
		if (meminfo_set) {
			struct sdma_req_meminfo *meminfo = (struct sdma_req_meminfo *) (req_info + 1);

			// We currently only ever use one payload IOV that would require
			// setting meminfo, and it will be the fist one:
			// index 0 (the first payload IOV, or iov[1]).
			const unsigned meminfo_idx   = 0;
			const unsigned hfi1_mem_type = HFI1_MEMINFO_TYPE_ENTRY_GET(meminfo->types, meminfo_idx);
			hmem_iface		     = (enum fi_hmem_iface) OPX_HMEM_OFI_MEM_TYPE[hfi1_mem_type];
			hmem_device		     = meminfo->context[0];
			fprintf(stderr,
				"(%d) [%d] hmem_iface=%u hmem_device=%lu meminfo->types=%#16.16llX meminfo->context[0]=%#16.16llX meminfo->context[15]=%#16.16llX\n",
				pid, req_num, hmem_iface, hmem_device, meminfo->types, meminfo->context[0],
				meminfo->context[15]);
		}
#endif
		fprintf(stderr, "(%d) [%d] PBC: %#16.16lX\n", pid, req_num, header_vec->scb.scb_9B.qw0);

		if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			fi_opx_hfi1_dump_packet_hdr(&header_vec->scb.scb_9B.hdr, OPX_SW_HFI1_TYPE, func, line);
		} else {
			fi_opx_hfi1_dump_packet_hdr(&header_vec->scb.scb_16B.hdr, OPX_SW_HFI1_TYPE, func, line);
		}

		fprintf(stderr, "(%d) [%d] req data iov=%p len=%lu\n", pid, req_num, iov_ptr[1].iov_base,
			iov_ptr[1].iov_len);

		if (hmem_iface == FI_HMEM_SYSTEM) {
			fprintf(stderr, "(%d) [%d] First 8 bytes of %p == %#16.16lX\n", pid, req_num,
				iov_ptr[1].iov_base, *((uint64_t *) iov_ptr[1].iov_base));
		} else {
			uint64_t first_qw;
			opx_copy_from_hmem(hmem_iface, hmem_device, OPX_HMEM_NO_HANDLE, &first_qw, iov_ptr[1].iov_base,
					   sizeof(uint64_t), OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);
			fprintf(stderr, "(%d) [%d] First 8 bytes of %p == %#16.16lX\n", pid, req_num,
				iov_ptr[1].iov_base, first_qw);
		}

		if (req_info_iovs > 2) {
			if (req_info_opcode == EAGER) {
				fprintf(stderr,
					"(%d) [%d] ERROR: Request specifies 3 IOVs, but opcode is set to EAGER!\n", pid,
					req_num);
			}
			fprintf(stderr, "(%d) [%d] tid iov=%p len=%lu pairs=%lu\n", pid, req_num, iov_ptr[2].iov_base,
				iov_ptr[2].iov_len, iov_ptr[2].iov_len / sizeof(uint32_t));
			if (iov_ptr[2].iov_len < sizeof(uint32_t)) {
				fprintf(stderr,
					"(%d) [%d] ERROR: Request opcode is set to EXPECTED (TID), but TID IOV's length is < minimum!\n",
					pid, req_num);
			}
			uint32_t kdeth	       = (uint32_t) (header_vec->scb.scb_9B.hdr.qw_9B[2] >> 32);
			uint32_t tidctrl       = (kdeth >> FI_OPX_HFI1_KDETH_TIDCTRL_SHIFT) & FI_OPX_HFI1_KDETH_TIDCTRL;
			uint32_t tididx	       = (kdeth >> FI_OPX_HFI1_KDETH_TID_SHIFT) & FI_OPX_HFI1_KDETH_TID;
			uint32_t tidOMshift    = (kdeth >> KDETH_OM_SHIFT) & KDETH_OM_MASK;
			uint32_t tidoffset     = (kdeth >> KDETH_OFFSET_SHIFT) & KDETH_OFFSET_MASK;
			uint32_t actual_offset = tidoffset
						 << (tidOMshift ? KDETH_OM_LARGE_SHIFT : KDETH_OM_SMALL_SHIFT);

			fprintf(stderr,
				"(%d) [%d] kdeth=%08X tidctrl=%08X tididx=%08X tidOMshift=%08X tidoffset=%08X actual offset=%08X\n",
				pid, req_num, kdeth, tidctrl, tididx, tidOMshift, tidoffset, actual_offset);

			uint32_t *tidpairs = (uint32_t *) iov_ptr[2].iov_base;
			for (int j = 0; j < (iov_ptr[2].iov_len / sizeof(uint32_t)); ++j) {
				fprintf(stderr, "(%d) [%d] tid    [%u]=%#8.8X LEN %u, CTRL %u, IDX %u\n", pid, req_num,
					j, tidpairs[j], (int) FI_OPX_EXP_TID_GET((tidpairs[j]), LEN),
					(int) FI_OPX_EXP_TID_GET((tidpairs[j]), CTRL),
					(int) FI_OPX_EXP_TID_GET((tidpairs[j]), IDX));
			}
		} else if (req_info_opcode == EXPECTED) {
			fprintf(stderr,
				"(%d) [%d] ERROR: Request opcode is set to EXPECTED (TID), but only has 2 IOVs (missing TID IOV)!\n",
				pid, req_num);
		}

#ifdef OPX_SDMA_DEBUG
		ssize_t retry_rc = writev(opx_ep->hfi->fd_cdev, iov_ptr, req_info_iovs);

		if (retry_rc > 0) {
			fprintf(stderr, "(%d) [%d] Retry succeeded!\n", pid, req_num);
		} else {
			fprintf(stderr, "(%d) [%d] Retry FAILED retry_rc=%ld errno=%d (%s)\n", pid, req_num, retry_rc,
				errno, strerror(errno));
		}
#endif
		++req_num;
		iov_ptr += req_info_iovs;
	} while (iov_ptr < (iovs + num_iovs));

	FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "SDMA Error, not handled, aborting\n");
	abort();
}

void opx_hfi1_sdma_process_pending(struct fi_opx_ep *opx_ep)
{
	struct slist *queue = &opx_ep->tx->sdma_pending_queue;

#ifdef OPX_DEBUG_COUNTERS_SDMA
	static uint64_t current_bps	    = 0;
	static uint64_t current_start_ns    = 0;
	static uint64_t current_end_ns	    = 0;
	static uint64_t current_total_ns    = 0;
	static uint64_t current_total_bytes = 0;
	static uint64_t current_count	    = 0;
#endif

	struct opx_sdma_request *request = (struct opx_sdma_request *) queue->head;
	while (request && request->comp_entry.status != QUEUED) {
		slist_remove_head(queue);
		assert(request->fill_index != OPX_SDMA_FILL_INDEX_INVALID);
		assert(*request->comp_state == OPX_SDMA_COMP_QUEUED);
		if (OFI_UNLIKELY(request->comp_entry.status == ERROR)) {
			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"===================================== SDMA Request (%p) -- Found error in queued entry, status=%d, error=%d\n",
				request, request->comp_entry.status, request->comp_entry.errcode);
			*request->comp_state = OPX_SDMA_COMP_ERROR;
		} else {
			assert(request->comp_entry.status == COMPLETE);
			*request->comp_state = OPX_SDMA_COMP_COMPLETE;
#ifdef OPX_DEBUG_COUNTERS_SDMA
			assert(request->comp_entry.start_time_ns >= current_start_ns);
			assert(request->comp_entry.end_time_ns >= current_end_ns);
			assert(request->comp_entry.end_time_ns >= request->comp_entry.start_time_ns);
			OPX_COUNTERS_RECORD_MEASURE(request->comp_entry.end_time_ns - request->comp_entry.start_time_ns,
						    opx_ep->debug_counters.sdma.completion);
			if (request->comp_entry.start_time_ns != current_start_ns) {
				if (request->comp_entry.start_time_ns >= current_end_ns) {
					// No overlap, record current interval and start new one
					// |----- Interval 1 ------|
					//                         |---- Interval 2 -----|
					if (current_count) {
						current_bps = (current_total_bytes * OPX_COUNTERS_NS_PER_SEC) /
							      current_total_ns;
						OPX_COUNTERS_RECORD_MEASURE(current_bps,
									    opx_ep->debug_counters.sdma.send_bw);
					}

					current_start_ns    = request->comp_entry.start_time_ns;
					current_end_ns	    = request->comp_entry.end_time_ns;
					current_total_ns    = current_end_ns - current_start_ns;
					current_count	    = 1;
					current_total_bytes = request->iovecs[1].iov_len;
				} else {
					//  Case 1: request->comp_entry.end_time_ns == current_end_ns
					//  Overlap, shorter interval.
					//  |----- Interval 1 ------|
					//  |--X--|---- Interval 2 -|
					//
					//  Case 2: request->comp_entry.end_time_ns > current_end_ns
					//  Overlapping, shifted interval
					//  |----- Interval 1 ------|
					//  |--X--|----- Interval 2 ------|
					//
					//  Record bytes sent over interval X, and start a
					//  new interval initialized with the bytes sent so
					//  far for the overlap of Intervals 1 & 2.
					double current_interval_bytes_per_ns =
						((double) current_total_bytes) / current_total_ns;
					uint64_t record_interval_ns =
						request->comp_entry.start_time_ns - current_start_ns;
					uint64_t record_interval_bytes =
						current_interval_bytes_per_ns * record_interval_ns;
					uint64_t record_interval_bps =
						(record_interval_bytes * OPX_COUNTERS_NS_PER_SEC) / record_interval_ns;
					OPX_COUNTERS_RECORD_MEASURE(record_interval_bps,
								    opx_ep->debug_counters.sdma.send_bw);

					current_start_ns = request->comp_entry.start_time_ns;
					current_end_ns	 = request->comp_entry.end_time_ns;
					current_total_ns = current_end_ns - current_start_ns;
					current_count	 = 1;
					current_total_bytes -= record_interval_bytes;
					current_total_bytes += request->iovecs[1].iov_len;
				}
			} else {
				//  Case 1: request->comp_entry.end_time_ns != current_end_ns
				// New longer interval
				// |----- Interval 1 ------|
				// |----------- Interval 2 -----------|
				//
				//  Case 2: request->comp_entry.end_time_ns == current_end_ns
				// |----- Interval 1 ------|
				// |----- Interval 2 ------|
				// Just add count/bytes
				current_end_ns	 = request->comp_entry.end_time_ns;
				current_total_ns = current_end_ns - current_start_ns;
				++current_count;
				current_total_bytes += request->iovecs[1].iov_len;
			}
			if (request->is_sdma_we) {
				struct fi_opx_hfi1_sdma_work_entry *sdma_we =
					(struct fi_opx_hfi1_sdma_work_entry *) request->requester;
				if (sdma_we->first_ack_time_ns) {
					assert(sdma_we->first_ack_time_ns > request->comp_entry.start_time_ns);
					assert(sdma_we->first_ack_time_ns < request->comp_entry.end_time_ns);
					OPX_COUNTERS_RECORD_MEASURE(request->comp_entry.end_time_ns -
									    sdma_we->first_ack_time_ns,
								    opx_ep->debug_counters.sdma.early_acks_delta);
				}
			}
#endif
		}
		OPX_BUF_FREE(request);
		request = (struct opx_sdma_request *) queue->head;
	}
}

__OPX_FORCE_INLINE__
int opx_hfi1_sdma_writev(struct fi_opx_ep *opx_ep, struct iovec *iovecs, int iovs_used, uint16_t avail,
			 uint16_t fill_index, const char *file, const char *func, const int line)
{
	opx_ep->hfi->info.sdma.fill_index	 = fill_index;
	opx_ep->hfi->info.sdma.available_counter = avail;

	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(writev_start_ns);
	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(writev_end_ns);
	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(writev_time_ns);

	OPX_COUNTERS_TIME_NS(writev_start_ns, &opx_ep->debug_counters);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "WRITEV");
	ssize_t writev_rc = writev(opx_ep->hfi->fd_cdev, iovecs, iovs_used);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "WRITEV");

	OPX_COUNTERS_TIME_NS(writev_end_ns, &opx_ep->debug_counters);
	OPX_COUNTERS_STORE_VAL(writev_time_ns, writev_end_ns - writev_start_ns);

	OPX_COUNTERS_RECORD_MEASURE(writev_time_ns, opx_ep->debug_counters.sdma.writev[iovs_used]);
	OPX_COUNTERS_RECORD_MEASURE(writev_time_ns, opx_ep->debug_counters.sdma.writev_all);

	if (writev_rc <= 0) {
		fi_opx_hfi1_sdma_handle_errors(opx_ep, writev_rc, iovecs, iovs_used, file, func, line);
	}

	return (writev_rc);
}

void opx_hfi1_sdma_process_requests(struct fi_opx_ep *opx_ep)
{
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.proc_reqs_calls);

	if (!opx_ep->hfi->info.sdma.available_counter) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.proc_reqs_no_fill_slots);
		return;
	}

	struct opx_sdma_queue *queue = &opx_ep->tx->sdma_request_queue;
	assert(!slist_empty(&queue->list));

	struct iovec iovecs[OPX_SDMA_HFI_MAX_IOVS_PER_WRITE];
	int	     iovs_used	= 0;
	int	     iovs_free	= opx_ep->tx->sdma_max_iovs_per_writev;
	uint16_t     avail	= opx_ep->hfi->info.sdma.available_counter;
	uint16_t     fill_index = opx_ep->hfi->info.sdma.fill_index;

	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(sdma_start_ns);
	OPX_COUNTERS_TIME_NS(sdma_start_ns, &opx_ep->debug_counters);

	while (!slist_empty(&queue->list) && avail) {
		struct opx_sdma_request *request = (struct opx_sdma_request *) slist_remove_head(&queue->list);
		assert(*request->comp_state == OPX_SDMA_COMP_PENDING_WRITEV);
		assert(request->fill_index == OPX_SDMA_FILL_INDEX_INVALID);

#ifdef OPX_SDMA_DEBUG
		// For debugging SDMA issues, issue one writev per request, so
		// that in the event of a failure, it's easy to identify the
		// offending request.
		if (iovs_used)
#else
		if (iovs_free < request->num_iovs)
#endif
		{
			int err = opx_hfi1_sdma_writev(opx_ep, iovecs, iovs_used, avail, fill_index, __FILE__, __func__,
						       __LINE__);
			if (err < 0) {
				/* Error occured in writev. Add the request back to queue */
				slist_insert_head((struct slist_entry *) request, &queue->list);
				return;
			}
			iovs_used = 0;
			iovs_free = opx_ep->tx->sdma_max_iovs_per_writev;
			OPX_COUNTERS_TIME_NS(sdma_start_ns, &opx_ep->debug_counters);
		}

		struct sdma_req_info *req_info = OPX_SDMA_REQ_INFO_PTR(&request->header_vec, request->set_meminfo);
		req_info->comp_idx	       = fill_index;
		request->fill_index	       = fill_index;
		OPX_TRACER_TRACE_SDMA(OPX_TRACER_BEGIN, "SDMA_COMPLETE_%hu", fill_index);

		assert(opx_ep->hfi->info.sdma.queued_entries[fill_index] == NULL);
		request->comp_entry.status  = QUEUED;
		request->comp_entry.errcode = 0;
		OPX_COUNTERS_STORE_VAL(request->comp_entry.start_time_ns, sdma_start_ns);
		opx_ep->hfi->info.sdma.queued_entries[fill_index] = (void *) &request->comp_entry;

		fill_index = (fill_index + 1) % (opx_ep->hfi->info.sdma.queue_size);
		--avail;

		for (int i = 0; i < request->num_iovs; ++i) {
			iovecs[iovs_used + i] = request->iovecs[i];
		}
		iovs_used += request->num_iovs;
		iovs_free -= request->num_iovs;
		queue->num_iovs -= request->num_iovs;
		--queue->num_reqs;

		*(request->comp_state) = OPX_SDMA_COMP_QUEUED;

		slist_insert_tail((struct slist_entry *) request, &opx_ep->tx->sdma_pending_queue);
	}

	assert(iovs_used);

	opx_hfi1_sdma_writev(opx_ep, iovecs, iovs_used, avail, fill_index, __FILE__, __func__, __LINE__);

	queue->slots_avail = avail;
}
