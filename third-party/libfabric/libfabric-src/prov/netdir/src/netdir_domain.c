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

#include "netdir.h"
#include "netdir_ov.h"
#include "netdir_log.h"
#include "netdir_util.h"
#include "netdir_iface.h"
#include "netdir_unexp.h"
#include "netdir_cq.h"

#include "ofi.h"
#include "ofi_util.h"
#include "ofi_enosys.h"
#include "rdma/fabric.h"
#include "rdma/fi_domain.h"

static int ofi_nd_domain_close(fid_t fid);
static int ofi_nd_domain_bind(struct fid *fid, struct fid *bfid,
			      uint64_t flags);

struct nd_msgchunk {
	IND2MemoryRegion		*mr;
	ND_BUF_CHUNK(nd_msgprefix)	chunk;
};

struct nd_inlinechunk {
	IND2MemoryRegion		*mr;
	char*				base;
	ND_BUF_CHUNK(nd_inlinebuf)	chunk;
};

struct nd_notifychunk {
	IND2MemoryRegion		*mr;
	ND_BUF_CHUNK(nd_notifybuf)	chunk;
};

static ND_BUF_CHUNK(nd_msgprefix)
*ofi_nd_alloc_msgprefix_chunk(ND_BUF_FOOTER(nd_msgprefix) *footer,
		       size_t *count);
static void ofi_nd_free_msgprefix_chunk(ND_BUF_CHUNK(nd_msgprefix) *chunk);

static ND_BUF_CHUNK(nd_inlinebuf)
*ofi_nd_alloc_inlinebuf_chunk(ND_BUF_FOOTER(nd_inlinebuf) *footer, size_t *count);
static void ofi_nd_free_inlinebuf_chunk(ND_BUF_CHUNK(nd_inlinebuf) *pchunk);

static ND_BUF_CHUNK(nd_notifybuf)
*ofi_nd_alloc_notifybuf_chunk(ND_BUF_FOOTER(nd_notifybuf) *footer, size_t *count);
static void ofi_nd_free_notifybuf_chunk(ND_BUF_CHUNK(nd_notifybuf) *pchunk);

static HRESULT ofi_nd_domain_notify(struct nd_domain *domain);
static void ofi_nd_domain_event(struct nd_event_base* base, DWORD bytes);
static void ofi_nd_domain_err(struct nd_event_base* base, DWORD bytes, DWORD err);

static struct fi_ops_domain ofi_nd_domain_ops = {
	.size = sizeof(ofi_nd_domain_ops),
	.av_open = fi_no_av_open,
	.cq_open = ofi_nd_cq_open,
	.endpoint = ofi_nd_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = ofi_nd_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = ofi_nd_srx_ctx
};

static struct fi_ops_mr ofi_nd_mr_ops = {
	.size = sizeof(ofi_nd_mr_ops),
	.reg = ofi_nd_mr_reg,
	.regv = ofi_nd_mr_regv,
	.regattr = ofi_nd_mr_regattr
};

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_domain_close,
	.bind = ofi_nd_domain_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_DOMAIN,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

static int ofi_nd_domain_close(fid_t fid)
{
	assert(fid->fclass == FI_CLASS_DOMAIN);

	struct nd_domain *domain = container_of(fid, struct nd_domain, fid.fid);

	DWORD ref = 0;
#if 0
	domain->do_progress = 0;
	pthread_join(domain->progress_thread, NULL);
#endif

	if (domain->cq) {
		domain->cq->lpVtbl->CancelOverlappedRequests(domain->cq);
		while (!domain->cq_canceled || nd_async_progress)
			SwitchToThread();
		domain->cq->lpVtbl->Release(domain->cq);
	}
	if (domain->info)
		fi_freeinfo(domain->info);
	if (domain->adapter_file && domain->adapter_file != INVALID_HANDLE_VALUE)
		CloseHandle(domain->adapter_file);
	if (domain->adapter) {
		ref = domain->adapter->lpVtbl->Release(domain->adapter);
		ND_LOG_DEBUG(FI_LOG_EP_CTRL, "domain->adapter ref count: %d\n", ref);
	}

	__ofi_nd_buf_fini_nd_msgprefix(&domain->msgfooter);
	__ofi_nd_buf_fini_nd_inlinebuf(&domain->inlinebuf);

	free(domain);

	return 0;
}

int ofi_nd_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		       struct fid_domain **pdomain, void *context)
{
	OFI_UNUSED(context);

	assert(fabric);
	assert(fabric->fid.fclass == FI_CLASS_FABRIC);
	assert(info);
	assert(info->domain_attr);
	assert(info->domain_attr->name);

	if (!info || !info->domain_attr || !info->domain_attr->name)
		return -FI_EINVAL;

	HRESULT hr;
	int res;
	struct sockaddr* addr;

	struct nd_domain *domain = (struct nd_domain*)calloc(1, sizeof(*domain));
	if (!domain)
		return -FI_ENOMEM;

	struct nd_domain def = {
		.fid = {
			.fid = ofi_nd_fid,
			.ops = &ofi_nd_domain_ops,
			.mr = &ofi_nd_mr_ops
		},
		.info = fi_dupinfo(info),
		.msgfooter = {
			.alloc_chunk = ofi_nd_alloc_msgprefix_chunk,
			.free_chunk = ofi_nd_free_msgprefix_chunk
		},
		.inlinebuf = {
			.alloc_chunk = ofi_nd_alloc_inlinebuf_chunk,
			.free_chunk = ofi_nd_free_inlinebuf_chunk
		},
		.notifybuf = {
			.alloc_chunk = ofi_nd_alloc_notifybuf_chunk,
			.free_chunk = ofi_nd_free_notifybuf_chunk
		}
	};

	*domain = def;

	dlist_init(&domain->ep_list);
#if 0
	domain->do_progress = 1;
	if (pthread_create(&domain->progress_thread, NULL,
		ofi_nd_progress, domain)) {
		ofi_nd_domain_close(&domain->fid.fid);
		return -FI_ENOMEM;;
	}
#endif

	res = ofi_nd_lookup_adapter(info->domain_attr->name, &domain->adapter, &addr);
	if (res || !domain->adapter) {
		ofi_nd_domain_close(&domain->fid.fid);
		return res;
	}

	memcpy(&domain->addr, addr, ofi_sizeofaddr(addr));

	hr = domain->adapter->lpVtbl->CreateOverlappedFile(domain->adapter,
							   &domain->adapter_file);

	if (FAILED(hr))
		goto hr_failed;

	if (!BindIoCompletionCallback(domain->adapter_file, domain_io_cb, 0)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto hr_failed;
	}

	domain->ainfo.InfoVersion = ND_VERSION_2;
	ULONG len = sizeof(domain->ainfo);
	hr = domain->adapter->lpVtbl->Query(domain->adapter, &domain->ainfo,
					    &len);
	if (FAILED(hr))
		goto hr_failed;

	hr = domain->adapter->lpVtbl->CreateCompletionQueue(
		domain->adapter, &IID_IND2CompletionQueue, domain->adapter_file,
		domain->ainfo.MaxCompletionQueueDepth, 0, 0,
		(void**)&domain->cq);
	if (FAILED(hr))
		goto hr_failed;

	*pdomain = &domain->fid;

	ND_LOG_DEBUG(FI_LOG_DOMAIN, "domain notification OV: %p\n", &domain->ov.ov);
	hr = ofi_nd_domain_notify(domain);
	if (FAILED(hr))
		goto hr_failed;

	return FI_SUCCESS;

hr_failed:
	ofi_nd_domain_close(&domain->fid.fid);
	return H2F(hr);
}

static int ofi_nd_domain_bind(struct fid *fid, struct fid *bfid,
			      uint64_t flags)
{
	assert(fid->fclass == FI_CLASS_DOMAIN);

	struct nd_domain *domain = container_of(fid, struct nd_domain, fid.fid);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		domain->eq = container_of(bfid, struct nd_eq, fid.fid);
		domain->eq_flags = flags;
		break;
	default:
		ND_LOG_WARN(FI_LOG_DOMAIN,
			   "ofi_nd_domain_bind: incorrect bind object class: %d",
			   bfid->fclass);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static ND_BUF_CHUNK(nd_msgprefix)
*ofi_nd_alloc_msgprefix_chunk(ND_BUF_FOOTER(nd_msgprefix) *footer, size_t *count)
{
	struct nd_domain *dom = container_of(footer, struct nd_domain, msgfooter);
	assert(dom->fid.fid.fclass == FI_CLASS_DOMAIN);

	HRESULT hr;
	size_t i;
	UINT32 token;

	struct nd_msgchunk *chunk = malloc(sizeof(*chunk));
	if (!chunk)
		return 0;
	memset(chunk, 0, sizeof(*chunk));
	assert(count);
	*count = countof(chunk->chunk.item);

	assert(dom->adapter);

	hr = dom->adapter->lpVtbl->CreateMemoryRegion(
		dom->adapter, &IID_IND2MemoryRegion, dom->adapter_file, (void**)&chunk->mr);
	if (FAILED(hr))
		goto fn_fail;

	hr = ofi_nd_util_register_mr(
		chunk->mr, &chunk->chunk, sizeof(chunk->chunk),
		ND_MR_FLAG_ALLOW_LOCAL_WRITE);
	if (FAILED(hr))
		goto fn_fail_mr;

	token = chunk->mr->lpVtbl->GetLocalToken(chunk->mr);

	for (i = 0; i < countof(chunk->chunk.item); i++)
		chunk->chunk.item[i].data.token = token;

	return &chunk->chunk;

fn_fail_mr:
	chunk->mr->lpVtbl->Release(chunk->mr);
fn_fail:
	free(chunk);
	return 0;
}

static void ofi_nd_free_msgprefix_chunk(ND_BUF_CHUNK(nd_msgprefix) *pchunk)
{
	assert(pchunk);

	struct nd_msgchunk *chunk = container_of(pchunk, struct nd_msgchunk, chunk);
	if (chunk->mr) {
		ofi_nd_util_unregister_mr(chunk->mr);
		chunk->mr->lpVtbl->Release(chunk->mr);
	}
	free(chunk);
}

static ND_BUF_CHUNK(nd_inlinebuf)
*ofi_nd_alloc_inlinebuf_chunk(ND_BUF_FOOTER(nd_inlinebuf) *footer, size_t *count)
{
	struct nd_domain *dom = container_of(footer, struct nd_domain, inlinebuf);
	assert(dom->fid.fid.fclass == FI_CLASS_DOMAIN);

	HRESULT hr;
	size_t i;
	UINT32 token;

	struct nd_inlinechunk *chunk = malloc(sizeof(*chunk));
	if (!chunk)
		return 0;
	memset(chunk, 0, sizeof(*chunk));
	assert(count);
	*count = countof(chunk->chunk.item);

	size_t len = gl_data.inline_thr * countof(chunk->chunk.item);
	chunk->base = malloc(len);
	if (!chunk->base)
		goto fn_fail;

	assert(dom->adapter);

	hr = dom->adapter->lpVtbl->CreateMemoryRegion(
		dom->adapter, &IID_IND2MemoryRegion, dom->adapter_file, (void**)&chunk->mr);
	if (FAILED(hr))
		goto fn_fail_base;

	hr = ofi_nd_util_register_mr(
		chunk->mr, chunk->base, len,
		ND_MR_FLAG_ALLOW_LOCAL_WRITE |
		ND_MR_FLAG_ALLOW_REMOTE_READ |
		ND_MR_FLAG_ALLOW_REMOTE_WRITE);
	if (FAILED(hr))
		goto fn_fail_mr;

	token = chunk->mr->lpVtbl->GetLocalToken(chunk->mr);

	for (i = 0; i < countof(chunk->chunk.item); i++) {
		chunk->chunk.item[i].data.token = token;
		chunk->chunk.item[i].data.buffer = chunk->base + (i * gl_data.inline_thr);
	}

	return &chunk->chunk;

fn_fail_mr:
	chunk->mr->lpVtbl->Release(chunk->mr);
fn_fail_base:
	free(chunk->base);
fn_fail:
	free(chunk);
	return 0;
}

static void ofi_nd_free_inlinebuf_chunk(ND_BUF_CHUNK(nd_inlinebuf) *pchunk)
{
	assert(pchunk);

	struct nd_inlinechunk *chunk = container_of(pchunk, struct nd_inlinechunk, chunk);
	if (chunk->mr) {
		ofi_nd_util_unregister_mr(chunk->mr);
		chunk->mr->lpVtbl->Release(chunk->mr);
	}
	if (chunk->base)
		free(chunk->base);
	free(chunk);
}

static ND_BUF_CHUNK(nd_notifybuf)
*ofi_nd_alloc_notifybuf_chunk(ND_BUF_FOOTER(nd_notifybuf) *footer, size_t *count)
{
	struct nd_domain *dom = container_of(footer, struct nd_domain, notifybuf);
	assert(dom->fid.fid.fclass == FI_CLASS_DOMAIN);

	HRESULT hr;
	size_t i;
	UINT32 token;

	struct nd_notifychunk *chunk = malloc(sizeof(*chunk));
	if (!chunk)
		return 0;
	memset(chunk, 0, sizeof(*chunk));
	assert(count);
	*count = countof(chunk->chunk.item);

	assert(dom->adapter);

	hr = dom->adapter->lpVtbl->CreateMemoryRegion(
		dom->adapter, &IID_IND2MemoryRegion, dom->adapter_file, (void**)&chunk->mr);
	if (FAILED(hr))
		goto fn_fail;

	hr = ofi_nd_util_register_mr(
		chunk->mr, &chunk->chunk, sizeof(chunk->chunk),
		ND_MR_FLAG_ALLOW_LOCAL_WRITE);
	if (FAILED(hr))
		goto fn_fail_mr;

	token = chunk->mr->lpVtbl->GetLocalToken(chunk->mr);

	for (i = 0; i < countof(chunk->chunk.item); i++)
		chunk->chunk.item[i].data.token = token;

	return &chunk->chunk;

fn_fail_mr:
	chunk->mr->lpVtbl->Release(chunk->mr);
	fn_fail:
	free(chunk);
	return 0;
}

static void ofi_nd_free_notifybuf_chunk(ND_BUF_CHUNK(nd_notifybuf) *pchunk)
{
	assert(pchunk);

	struct nd_notifychunk *chunk = container_of(pchunk, struct nd_notifychunk, chunk);
	if (chunk->mr) {
		ofi_nd_util_unregister_mr(chunk->mr);
		chunk->mr->lpVtbl->Release(chunk->mr);
	}
	free(chunk);
}

static HRESULT ofi_nd_domain_notify(struct nd_domain *domain)
{
	assert(domain);
	assert(domain->fid.fid.fclass == FI_CLASS_DOMAIN);
	assert(domain->cq);

	nd_event_base ov = {
		.event_cb = ofi_nd_domain_event,
		.err_cb = ofi_nd_domain_err
	};

	domain->ov = ov;
	return domain->cq->lpVtbl->Notify(domain->cq, ND_CQ_NOTIFY_ANY, &domain->ov.ov);
}

static void ofi_nd_domain_event(struct nd_event_base* base, DWORD bytes)
{
	OFI_UNUSED(bytes);

	assert(base);
	struct nd_domain *domain = container_of(base, struct nd_domain, ov);

	assert(domain->fid.fid.fclass == FI_CLASS_DOMAIN);
	assert(domain->cq);

	ND2_RESULT result[256];
	DWORD count;
	nd_unexpected_ctx *ctx;
	do {
		count = domain->cq->lpVtbl->GetResults(domain->cq, result, countof(result));
		size_t i;
		for (i = 0; i < count; i++) {
			ND_LOG_DEBUG(FI_LOG_EP_DATA, "Domain event is %d with status %s\n",
				     result[i].RequestType,
				     ofi_nd_error_str(result[i].Status));
			switch (result[i].RequestType) {
			case Nd2RequestTypeReceive:
				ctx = (nd_unexpected_ctx *)result[i].RequestContext;
				if (!OFI_ND_IS_SERVICE_EVENT(ctx->entry->header.event))
					ofi_nd_unexp_event(&result[i]);
				else
					ofi_nd_unexp_service_event(&result[i]);
				break;
			case Nd2RequestTypeSend:
				ofi_nd_send_event(&result[i]);
				break;
			case Nd2RequestTypeRead:
				ofi_nd_read_event(&result[i]);
				break;
			case Nd2RequestTypeWrite:
				ofi_nd_write_event(&result[i]);
				break;
			default:
				/* shouldn't go here */
				NODEFAULT;
			}

			/* Let's walk through sending queue to send data 
			 * that are ready to be transmitted */
			struct nd_ep *ep = (struct nd_ep*)result[i].QueuePairContext;
			ofi_nd_ep_progress(ep);
		}
	} while (count == countof(result));

	ofi_nd_domain_notify(domain);
}

static void ofi_nd_domain_err(struct nd_event_base* base, DWORD bytes, DWORD err)
{
	OFI_UNUSED(err);
	if (err == STATUS_CANCELLED) {
		struct nd_domain *domain = container_of(base, struct nd_domain, ov);

		assert(domain->fid.fid.fclass == FI_CLASS_DOMAIN);
		assert(domain->cq);
		domain->cq_canceled = 1;
		return;
	}

	ofi_nd_domain_event(base, bytes);
}

#endif /* _WIN32 */

