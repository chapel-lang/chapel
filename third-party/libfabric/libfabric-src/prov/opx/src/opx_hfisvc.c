/*
 * Copyright (C) 2025 Cornelis Networks.
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

#include "rdma/opx/opx_hfisvc.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/opx_tracer.h"

#if HAVE_HFISVC

#include <infiniband/hfi1dv.h>
#include <infiniband/verbs.h>
#include <infiniband/hfisvc_client.h>

#include "rdma/opx/opx_hfi1_rdma_core.h"

#endif

/**
 * @brief When OPX_HFISVC_DEBUG is defined in the build, we'll put out lots of
 * logs via the OPX_HFISVC_DEBUG_LOG macro. The logging can be turned off
 * at runtime by defining the env. variable OPX_HFISVC_LOG_DISABLE in the run
 * command. This is useful for not needing to rebuild entirely in order to
 * toggle logging on/off.
 */
int opx_hfisvc_log_enabled = 1;

int opx_hfisvc_deferred_recv_rts(union fi_opx_hfi1_deferred_work *work)
{
#if HAVE_HFISVC
	struct opx_hfisvc_recv_rts_params *params = &work->hfisvc_rts_params;

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV -- RENDEZVOUS RTS HFISVC (deferred) (begin) context %p\n",
		params->context);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-RTS-HFISVC");

	struct fi_opx_ep   *opx_ep	    = params->opx_ep;
	struct opx_context *context	    = params->context;
	const uint32_t	    niov	    = params->niov;
	uint32_t	    cur_iov	    = params->cur_iov;
	uint32_t	    sbuf_lid	    = params->sbuf_lid;
	uint32_t	    sbuf_client_key = params->sbuf_client_key;
	uint8_t		   *recv_buf	    = (uint8_t *) params->recv_buf;

	struct hfisvc_client_completion completion = {
		.flags		= HFISVC_CLIENT_COMPLETION_FLAG_CQ,
		.cq.app_context = (uint64_t) context,
		.cq.handle	= *opx_ep->hfisvc.cq_completion_queue,
	};

	int rc	       = FI_SUCCESS;
	int read_count = 0;

	for (int i = cur_iov; i < niov; ++i) {
		const uint32_t sbuf_access_key = params->iovs[i].access_key;
		const uint64_t sbuf_len	       = params->iovs[i].len;
		const uint64_t sbuf_offset     = params->iovs[i].offset;

		rc = (*opx_ep->domain->hfisvc.cmd_rdma_read_va)(
			opx_ep->hfisvc.command_queue, completion, 0ul /* flags */, sbuf_lid, sbuf_client_key, sbuf_len,
			0ul /* immediate data */, sbuf_access_key, sbuf_offset, recv_buf);

		if (rc != FI_SUCCESS) {
			params->cur_iov	 = i;
			params->recv_buf = (void *) recv_buf;
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_recv_rts.eagain_hfisvc);
			OPX_HFISVC_DEBUG_LOG(
				"[%d/%d] rdma_read failed with rc=%d context=%p recv_buf=%p sbuf_key=%u, sbuf_access_key=%u sbuf_len=%lu\n",
				i + 1, niov, rc, context, recv_buf, sbuf_client_key, sbuf_access_key, sbuf_len);
			rc = -FI_EAGAIN;
			break;
		}
		++read_count;

		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_recv_rts.rdma_read);

		OPX_HFISVC_DEBUG_LOG(
			"[%d/%d] Successfully issued rdma_read context=%p recv_buf=%p sbuf_key=%u, sbuf_access_key=%u sbuf_len=%lu\n",
			i + 1, niov, context, recv_buf, sbuf_client_key, sbuf_access_key, sbuf_len);
		recv_buf += sbuf_len;
	}

	if (read_count) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.doorbell_ring.deferred_work);
		__attribute__((unused)) int doorbell_rc =
			(*opx_ep->domain->hfisvc.doorbell)(opx_ep->domain->hfisvc.ctx);
		assert(doorbell_rc == 0);
	}

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV -- RENDEZVOUS RTS HFISVC (deferred) (end) context %p\n",
		params->context);

	OPX_TRACER_TRACE(rc ? OPX_TRACER_END_EAGAIN : OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS-HFISVC");

	return rc;
#else
	return 0;
#endif
}

int opx_hfisvc_deferred_recv_rts_enqueue(struct fi_opx_ep *opx_ep, struct opx_context *context, const uint32_t niov,
					 const uint32_t sbuf_client_key, const uint32_t sbuf_lid, const void *recv_buf,
					 const union opx_hfisvc_iov *iovs)
{
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	if (OFI_UNLIKELY(work == NULL)) {
		OPX_HFISVC_DEBUG_LOG("Error allocating deferred work for hfisvc recv rts\n");
		return -FI_ENOMEM;
	}
	struct opx_hfisvc_recv_rts_params *params = &work->hfisvc_rts_params;
	params->work_elem.slist_entry.next	  = NULL;
	params->work_elem.completion_action	  = NULL;
	params->work_elem.payload_copy		  = NULL;
	params->work_elem.work_fn		  = opx_hfisvc_deferred_recv_rts;
	params->work_elem.work_type		  = OPX_WORK_TYPE_HFISVC;
	params->work_elem.complete		  = false;
	params->opx_ep				  = opx_ep;
	params->context				  = context;
	params->niov				  = niov;
	params->cur_iov				  = 0;
	params->sbuf_client_key			  = sbuf_client_key;
	params->sbuf_lid			  = sbuf_lid;
	params->recv_buf			  = (void *) recv_buf;

	for (int i = 0; i < niov; ++i) {
		params->iovs[i] = iovs[i];
	}

	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[OPX_WORK_TYPE_HFISVC]);

	return FI_SUCCESS;
}
