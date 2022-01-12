/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifdef _WIN32

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include "ndspi.h"

#include "netdir.h"
#include "netdir_cq.h"
#include "netdir_log.h"
#include "netdir_util.h"
#include "netdir_queue.h"
#include "netdir_iface.h"
#include "netdir_queue.h"
#include "netdir_unexp.h"

#include <windows.h>

#define PREPOSTLEN (sizeof(nd_unexpected_buf) + gl_data.inline_thr)

static ND_BUF_CHUNK(nd_unexpected_entry) *ofi_nd_unexp_alloc_chunk(
	ND_BUF_FOOTER(nd_unexpected_entry) *footer, size_t* count);
static void ofi_nd_unexp_free_chunk(ND_BUF_CHUNK(nd_unexpected_entry) *chunk);

OFI_ND_NB_BUF(nd_unexpected_ctx);
OFI_ND_NB_BUF(nd_unexpected_entry);
OFI_ND_NB_BUF_IMP(nd_unexpected_ctx);
OFI_ND_NB_BUF_IMP_ALLOC(nd_unexpected_entry,
			ofi_nd_unexp_alloc_chunk,
			ofi_nd_unexp_free_chunk);

HRESULT ofi_nd_unexp_init(struct nd_ep *ep)
{
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_unexpected_ctx));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_unexpected_entry));

	int i;
	HRESULT hr;
	int total_count = (gl_data.prepost_cnt +
		gl_data.flow_control_cnt) * gl_data.prepost_buf_cnt;

	if (ep->unexpected.active)
		return S_OK;

	dlist_init(&ep->unexpected.received);

	ep->unexpected.unexpected = malloc(
		total_count * sizeof(*ep->unexpected.unexpected)
	);
	if (!ep->unexpected.unexpected)
		return ND_NO_MEMORY;

	size_t len = PREPOSTLEN * total_count;

	char* tmp = (char*)calloc(1, len);
	if (!tmp)
		return ND_NO_MEMORY;

	hr = ep->domain->adapter->lpVtbl->CreateMemoryRegion(
		ep->domain->adapter, &IID_IND2MemoryRegion,
		ep->domain->adapter_file, (void**)&ep->unexpected.mr);
	if (FAILED(hr))
		return hr;

	hr = ofi_nd_util_register_mr(ep->unexpected.mr, tmp,
				     len, ND_MR_FLAG_ALLOW_LOCAL_WRITE);
	if (FAILED(hr))
		return hr;

	ep->unexpected.token =
		ep->unexpected.mr->lpVtbl->GetLocalToken(ep->unexpected.mr);

	for (i = 0; i < total_count; i++) {
		ep->unexpected.unexpected[i] =
			(struct nd_unexpected_buf*)(tmp + (PREPOSTLEN * i));
	}

	InterlockedIncrement(&ep->unexpected.active);

	return S_OK;
}

HRESULT ofi_nd_unexp_fini(struct nd_ep *ep)
{
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	int total_count = (gl_data.prepost_cnt +
		gl_data.flow_control_cnt) * gl_data.prepost_buf_cnt;

	if (InterlockedDecrement(&ep->unexpected.active))
		return S_OK;


	ND_LOG_INFO(FI_LOG_EP_CTRL, "finalize unexpected queue\n"
		    "total_count = %d\n",
		    total_count);

	InterlockedAdd(&ep->shutdown, total_count);
	if (ep->qp) {
		ep->qp->lpVtbl->Flush(ep->qp);
		/* wait until all preposted entries are canceled (GetResult()
		 * ND2_RESULT entries with Status == STATUS_CANCELLED) */
		while (InterlockedAdd(&ep->shutdown, 0) > 0)
			/* yields execution to another thread
			 * that is ready to run on: */
			if (!SwitchToThread()) /* - the current processor */
				Sleep(0); /* - the another processor */
	}

	if (ep->unexpected.mr) {
		ofi_nd_util_unregister_mr(ep->unexpected.mr);
		ep->unexpected.mr->lpVtbl->Release(ep->unexpected.mr);
	}

	if (ep->unexpected.unexpected) {
		/* Free allocated a piece of memory for unexpected events */
		if (ep->unexpected.unexpected[0])
			free(ep->unexpected.unexpected[0]);
		/* Free allocated set of unexpected entries */
		free(ep->unexpected.unexpected);
	}

	return S_OK;
}

static ND_BUF_CHUNK(nd_unexpected_entry)
*ofi_nd_unexp_alloc_chunk(ND_BUF_FOOTER(nd_unexpected_entry) *footer, size_t* count)
{
	OFI_UNUSED(footer);

	ND_BUF_CHUNK(nd_unexpected_entry) *chunk = malloc(sizeof(*chunk));
	if (!chunk)
		return 0;
	assert(count);
	*count = countof(chunk->item);
	memset(chunk, 0, sizeof(*chunk));

	char *tmp = malloc(countof(chunk->item) * PREPOSTLEN);
	if (!tmp)
		goto fn_fail;

	size_t i;
	for (i = 0; i < countof(chunk->item); i++) {
		chunk->item[i].data.buf = (struct nd_unexpected_buf*)(tmp + (PREPOSTLEN * i));
	}

	return chunk;

fn_fail:
	free(chunk);
	return 0;
}

static void ofi_nd_unexp_free_chunk(ND_BUF_CHUNK(nd_unexpected_entry) *chunk)
{
	assert(chunk);

	if (chunk->item[0].data.buf)
		free(chunk->item[0].data.buf);
	free(chunk);
}

static int ofi_nd_return_true(struct dlist_entry *item, const void *arg)
{
	OFI_UNUSED(item);
	OFI_UNUSED(arg);
	return 1;
}

void ofi_nd_unexp_match(struct nd_ep *ep)
{
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	int done = 0;
	do {
		nd_cq_entry *entry = NULL;
		nd_cq_entry *ep_entry = NULL;
		nd_cq_entry *srx_entry = NULL;
		nd_unexpected_entry *unexp = NULL;

		if (ep->srx)
			EnterCriticalSection(&ep->srx->prepost_lock);
		else
			EnterCriticalSection(&ep->prepost_lock);

		struct nd_queue_item *ep_qentry = NULL;
		struct nd_queue_item *srx_qentry = NULL;

		struct dlist_entry *qunexp = NULL;

		if ((ofi_nd_queue_peek(&ep->prepost, &ep_qentry) ||
		    (ep->srx && ofi_nd_queue_peek(&ep->srx->prepost, &srx_qentry))) &&
		    (!dlist_empty(&ep->unexpected.received))) {
			qunexp = dlist_find_first_match(
				&ep->unexpected.received, ofi_nd_return_true, 0);
			unexp = container_of(
				qunexp,
				nd_unexpected_entry, ep_list);

			if(ep_qentry)
				ep_entry = container_of(ep_qentry, nd_cq_entry, queue_item);
			if(srx_qentry)
				srx_entry = container_of(srx_qentry, nd_cq_entry, queue_item);

			if (ep_entry && srx_entry) {
				if (ep_entry->seq < srx_entry->seq) {
					entry = ep_entry;
					ofi_nd_queue_pop(&ep->prepost, &ep_qentry);
				}
				else {
					entry = srx_entry;
					ofi_nd_queue_pop(&ep->srx->prepost, &srx_qentry);
				}
			}
			else if (ep_entry) {
				entry = ep_entry;
				ofi_nd_queue_pop(&ep->prepost, &ep_qentry);
			}
			else {
				entry = srx_entry;
				ofi_nd_queue_pop(&ep->srx->prepost, &srx_qentry);
			}

			dlist_remove(qunexp);
			/* remove element from srx queue */
			if(unexp->ep->srx)
				dlist_remove(&unexp->srx_list);
		}
		else {
			done = 1;
		}

		if (ep->srx)
			LeaveCriticalSection(&ep->srx->prepost_lock);
		else
			LeaveCriticalSection(&ep->prepost_lock);
		if (!done) {
			/* Set event that was received */
			entry->event = unexp->buf->header.event;
			ND_LOG_EVENT_INFO(entry);
			if (unexp->buf->header.flags.req_ack)
				ofi_nd_send_ack(entry, ep);
			ofi_nd_dispatch_cq_event(unexp->buf->header.event, entry, unexp);
		}
	} while (!done);
}

void ofi_nd_srx_match(struct nd_srx *srx)
{
	assert(srx);
	assert(srx->fid.fid.fclass == FI_CLASS_SRX_CTX);

	int done = 0;
	do {
		nd_cq_entry *entry = NULL;
		nd_unexpected_entry *unexp = NULL;

		EnterCriticalSection(&srx->prepost_lock);

		struct nd_queue_item *qentry = NULL;

		if (ofi_nd_queue_peek(&srx->prepost, &qentry) &&
		    !dlist_empty(&srx->received)) {

			entry = container_of(qentry, nd_cq_entry, queue_item);
			ofi_nd_queue_pop(&srx->prepost, &qentry);

			struct dlist_entry *qunexp = dlist_remove_first_match(
				&srx->received, ofi_nd_return_true, 0);

			unexp = container_of(
				qunexp, nd_unexpected_entry, srx_list);
			/* remove element from ep queue */
			dlist_remove(&unexp->ep_list);
		}
		else {
			done = 1;
		}
		LeaveCriticalSection(&srx->prepost_lock);

		if (!done) {
			if (unexp->buf->header.flags.req_ack)
				ofi_nd_send_ack(entry, unexp->ep);
			ofi_nd_dispatch_cq_event(unexp->buf->header.event, entry, unexp);
		}
	} while (!done);
}

void ofi_nd_unexp_event(ND2_RESULT *result)
{
	assert(result);
	assert(result->RequestType == Nd2RequestTypeReceive);

	struct nd_ep *ep = (struct nd_ep *)result->QueuePairContext;
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	nd_unexpected_ctx *ctx = (nd_unexpected_ctx *)result->RequestContext;
	struct nd_unexpected_buf *buf = ctx->entry;
	assert(ctx);

	if (ep->shutdown || result->Status == STATUS_CANCELLED) { 
		/* shutdown mode */
		ND_BUF_FREE(nd_unexpected_ctx, ctx);
		InterlockedDecrement(&ep->shutdown);
		return;
	}

	if (ctx->entry->header.flags.ack) {
		if (ctx->entry->header.flags.empty) {
			/* Just drop this received unexpected entry
			 * since no CQ are posted to be cogherent with
			 * this unexp and this unexp isn't carrying
			 * payload data in itself */
			ND_BUF_FREE(nd_unexpected_ctx, ctx);

			ofi_nd_unexp_repost(ep, buf);
			ep->send_op.flags.is_send_blocked = 0;
			return;
		}

		ep->send_op.flags.is_send_blocked = 0;
	}

	nd_unexpected_entry *entry = ND_BUF_ALLOC(nd_unexpected_entry);
	/* do NOT zero mem for entry: buf points to real buffer */
	if (!entry) {
		ND_LOG_WARN(FI_LOG_EP_DATA, "Failed to allocate 'unexpected' buffer");
		return;
	}

	entry->result = *result;
	assert(entry->buf);
	assert(result->BytesTransferred <= PREPOSTLEN);
	memcpy(entry->buf, ctx->entry, result->BytesTransferred);

	entry->ep = ep;

	if (ep->srx) {
		EnterCriticalSection(&ep->srx->prepost_lock);
		dlist_insert_tail(&entry->srx_list, &ep->srx->received);
	}
	else {
		EnterCriticalSection(&ep->prepost_lock);
	}
	dlist_insert_tail(&entry->ep_list, &ep->unexpected.received);

	if (ep->srx) {
		LeaveCriticalSection(&ep->srx->prepost_lock);
	}
	else {
		LeaveCriticalSection(&ep->prepost_lock);
	}

	ND_BUF_FREE(nd_unexpected_ctx, ctx);
	ofi_nd_unexp_repost(ep, buf);
#if 0
	ep->unexpected.used_counter++;
	if (ep->unexpected.used_counter == (size_t)gl_data.total_avail) {
		ep->unexpected.used_counter = 0;
		ofi_nd_unexp_payload_run(ep);
	}
#endif
	ofi_nd_unexp_match(ep);
}

void ofi_nd_unexp_service_event(ND2_RESULT *result)
{
	assert(result);
	assert(result->RequestType == Nd2RequestTypeReceive);

	struct nd_ep *ep = (struct nd_ep *)result->QueuePairContext;
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	struct nd_queue_item *ep_qentry = NULL;

	nd_unexpected_ctx *ctx = (nd_unexpected_ctx *)result->RequestContext;
	assert(ctx);

	if (ep->shutdown || result->Status == STATUS_CANCELLED) {
		/* shutdown mode */
		ND_BUF_FREE(nd_unexpected_ctx, ctx);
		InterlockedDecrement(&ep->shutdown);
		return;
	}

	struct nd_unexpected_buf *unexp_buf = ctx->entry;
	assert(unexp_buf);
	if (unexp_buf->header.flags.ack)
		ep->send_op.flags.is_send_blocked = 0;

	if (OFI_ND_IS_SERVICE_EVENT(unexp_buf->header.event) &&
	    (ofi_nd_queue_peek(&ep->internal_prepost, &ep_qentry))) {
		if (ep_qentry) {
			nd_unexpected_entry unexp = { 0 };
			nd_cq_entry *ep_entry = NULL;

			unexp.result = *result;
			unexp.ep = ep;

			ND_LOG_DEBUG(FI_LOG_EP_CTRL, "Received internal event, let's "
				     "try to process it\n");
			ep_entry = container_of(ep_qentry, nd_cq_entry, queue_item);
			ofi_nd_queue_pop(&ep->internal_prepost, &ep_qentry);
			/* Set event that was received */
			ep_entry->event = unexp_buf->header.event;
			ND_LOG_EVENT_INFO(ep_entry);
			if (unexp_buf->header.flags.req_ack)
				ofi_nd_send_ack(ep_entry, ep);
			ofi_nd_dispatch_cq_event(unexp_buf->header.event, ep_entry, &unexp);

			/* just zero out it, because it should have been freed above */
			ep_entry = NULL;
		}
		else {
			/* Shouldn't happen */
			ND_LOG_WARN(FI_LOG_EP_CTRL, "Received internal event, but "
				"internal queue is empty\n");
			assert(0);
		}
	}

	ND_BUF_FREE(nd_unexpected_ctx, ctx);
	ofi_nd_unexp_repost(ep, unexp_buf);
#if 0
	ep->unexpected.used_counter++;
	if (ep->unexpected.used_counter == (size_t)gl_data.total_avail) {
		ep->unexpected.used_counter = 0;
		ofi_nd_unexp_payload_run(ep);
	}
#endif
}

HRESULT ofi_nd_unexp_repost(struct nd_ep *ep, struct nd_unexpected_buf *entry)
{
	assert(entry);
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	HRESULT hr;

	nd_unexpected_ctx *ctx = ND_BUF_ALLOC(nd_unexpected_ctx);
	if (!ctx)
		return ND_NO_MEMORY;
	memset(ctx, 0, sizeof(*ctx));
	ctx->entry = entry;
	ctx->ep = ep;

	assert(ep->unexpected.mr);
	assert(ep->unexpected.token);
	ND2_SGE sge = {
		.Buffer = (void*)entry,
		.BufferLength = (ULONG)(sizeof(nd_unexpected_buf) +
			MIN(sizeof(struct nd_msg_location) * ND_MSG_IOV_LIMIT,
			    gl_data.inline_thr)),
		.MemoryRegionToken = ep->unexpected.token
	};

	assert(ep->qp);
	hr = ep->qp->lpVtbl->Receive(ep->qp, ctx, &sge, 1);
	return hr;
}

HRESULT ofi_nd_unexp_run(struct nd_ep *ep)
{
	int i;
	int total_count = (gl_data.prepost_cnt +
		gl_data.flow_control_cnt) * gl_data.prepost_buf_cnt;

	for (i = 0; i < total_count; i++)
		ofi_nd_unexp_repost(ep, ep->unexpected.unexpected[i]);

	return S_OK;
}

void ofi_nd_release_unexp_entry(nd_unexpected_entry *unexp)
{
	ND_BUF_FREE(nd_unexpected_entry, unexp);
}

#endif /* _WIN32 */

