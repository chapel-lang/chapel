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

#include "netdir.h"

#include "netdir_ov.h"
#include "netdir_iface.h"

#include "ofi.h"
#include "ofi_util.h"

static int ofi_nd_mr_close(struct fid *fid);

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_mr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_MR,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

typedef struct ofi_nd_mr_ov {
	nd_event_base	base;
	struct nd_eq	*eq;
	fid_t		fid;
	void		*context;
	LONG		cnt;
} ofi_nd_mr_ov;

static void ofi_nd_mr_ov_free(struct nd_event_base* base);
static void ofi_nd_mr_ov_event(struct nd_event_base* base, DWORD bytes);
static void ofi_nd_mr_ov_err(struct nd_event_base* base, DWORD bytes, DWORD err);

OFI_ND_NB_BUF_TYPED(nd_mr, struct nd_mr);
OFI_ND_NB_BUF_IMP(nd_mr);

OFI_ND_NB_BUF_TYPED(mr_ov, ofi_nd_mr_ov);
OFI_ND_NB_BUF_IMP(mr_ov);

static inline void ofi_nd_mr_fini_handler()
{
	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_mr));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(mr_ov));
}

int ofi_nd_mr_reg(struct fid *fid, const void *buf, size_t len,
		  uint64_t access, uint64_t offset, uint64_t requested_key,
		  uint64_t flags, struct fid_mr **pmr, void *context)
{
	OFI_UNUSED(requested_key);

	assert(fid->fclass == FI_CLASS_DOMAIN);
	assert(!offset);

	HRESULT hr;

	ofi_nd_mr_fini_handler();

	if (fid->fclass != FI_CLASS_DOMAIN)
		return -FI_EINVAL;
	if (offset)
		return -FI_EINVAL;
	if (flags)
		return -FI_EINVAL;

	struct nd_domain *domain = container_of(fid, struct nd_domain, fid.fid);

	assert(domain->adapter);
	assert(domain->adapter_file);

	struct nd_mr *mr = ofi_nd_buf_alloc_nd_mr();
	if (!mr)
		return -FI_ENOMEM;

	struct nd_mr def = {
		.fid = {
			.fid = ofi_nd_fid
		}
	};

	*mr = def;

	hr = domain->adapter->lpVtbl->CreateMemoryRegion(
		domain->adapter, &IID_IND2MemoryRegion, domain->adapter_file,
		(void**)&mr->mr);

	if (FAILED(hr))
		goto fn_fail;

	ULONG ind2flag = 0;

	if (access & FI_REMOTE_READ)
		ind2flag |= ND_MR_FLAG_ALLOW_REMOTE_READ;
	if (access & FI_REMOTE_WRITE)
		ind2flag |= ND_MR_FLAG_ALLOW_REMOTE_WRITE;
	if ((access & FI_WRITE) || (access & FI_RECV))
		ind2flag |= ND_MR_FLAG_ALLOW_LOCAL_WRITE;

	/* there is bug in mlx4 module: it always generates
	   IO completion (even for cases when hEvent value
	   of OVERLAPPED structure is initialized). To
	   workaround this we have to use dynamically allocated
	   ov */
	ofi_nd_mr_ov *ov = ND_BUF_ALLOC(mr_ov);
	if (!ov) {
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}
	memset(ov, 0, sizeof(*ov));

	ofi_nd_mr_ov ovdef = {
		.base = {
			.free = ofi_nd_mr_ov_free,
			.event_cb = ofi_nd_mr_ov_event,
			.err_cb = ofi_nd_mr_ov_err
		},
		.eq = domain->eq,
		.fid = &mr->fid.fid,
		.context = context
	};

	*ov = ovdef;
	if (!(domain->eq_flags & FI_MR_COMPLETE)) {
		ov->cnt = 2;
		ov->base.ov.hEvent = CreateEvent(0, TRUE, FALSE, NULL);
	}

	hr = mr->mr->lpVtbl->Register(mr->mr, buf, len, ind2flag, &ov->base.ov);
	if (FAILED(hr)) {
		ofi_nd_mr_ov_free(&ov->base);
		goto fn_fail;
	}

	if (!(domain->eq_flags & FI_MR_COMPLETE)) {
		/* sync memory registration */
		hr = mr->mr->lpVtbl->GetOverlappedResult(mr->mr, &ov->base.ov, TRUE);
		if (!InterlockedDecrement(&ov->cnt))
			ofi_nd_mr_ov_free(&ov->base);
		if (FAILED(hr))
			goto fn_fail;
		mr->fid.key = mr->mr->lpVtbl->GetRemoteToken(mr->mr);
		mr->fid.mem_desc = (void *)(uintptr_t)mr->mr->lpVtbl->GetLocalToken(mr->mr);
	}
	else {
		/* async memory registration */
		hr = mr->mr->lpVtbl->Register(
			mr->mr, buf, len, ind2flag, &ov->base.ov);
		if (FAILED(hr)) {
			ofi_nd_mr_ov_free(&ov->base);
			goto fn_fail;
		}
	}

	*pmr = &mr->fid;

	return FI_SUCCESS;

fn_fail:
	ofi_nd_mr_close(&mr->fid.fid);
	return H2F(hr);
}

int ofi_nd_mr_regv(struct fid *fid, const struct iovec *iov,
		   size_t count, uint64_t access,
		   uint64_t offset, uint64_t requested_key,
		   uint64_t flags, struct fid_mr **mr, void *context)
{
	OFI_UNUSED(fid);
	OFI_UNUSED(iov);
	OFI_UNUSED(count);
	OFI_UNUSED(access);
	OFI_UNUSED(offset);
	OFI_UNUSED(requested_key);
	OFI_UNUSED(flags);
	OFI_UNUSED(fid);
	OFI_UNUSED(mr);
	OFI_UNUSED(context);

	/* This functionality wasn't implemented due to impossibility
	 * to do it by means of ND services. To avoid problems in future,
	 * just to not implement it until no support from ND */

	ofi_nd_mr_fini_handler();
	assert(0);
	return FI_SUCCESS;
}

int ofi_nd_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		      uint64_t flags, struct fid_mr **mr)
{
	OFI_UNUSED(fid);
	OFI_UNUSED(attr);
	OFI_UNUSED(flags);
	OFI_UNUSED(mr);

	ofi_nd_mr_fini_handler();
	assert(0);
	return FI_SUCCESS;
}

static int ofi_nd_mr_close(struct fid *fid)
{
	ND_LOG_DEBUG(FI_LOG_MR, "closing mr\n");
	assert(fid->fclass == FI_CLASS_MR);
	if (fid->fclass != FI_CLASS_MR)
		return -FI_EINVAL;

	struct nd_mr *mr = container_of(fid, struct nd_mr, fid.fid);

	if (mr->mr)
		mr->mr->lpVtbl->Release(mr->mr);
	if (mr->wnd)
		mr->wnd->lpVtbl->Release(mr->wnd);

	ofi_nd_buf_free_nd_mr(mr);

	return FI_SUCCESS;
}

static void ofi_nd_mr_ov_free(struct nd_event_base* base)
{
	ofi_nd_mr_ov *ov = container_of(base, ofi_nd_mr_ov, base);
	if (ov->base.ov.hEvent && ov->base.ov.hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(ov->base.ov.hEvent);

	ofi_nd_buf_free_mr_ov(ov);
}

static void ofi_nd_mr_ov_event(struct nd_event_base* base, DWORD bytes)
{
	OFI_UNUSED(bytes);

	HRESULT hr;

	ofi_nd_mr_ov *ov = container_of(base, ofi_nd_mr_ov, base);

	if (ov->cnt) { /* this is sync mr reg operation */
		if (!InterlockedDecrement(&ov->cnt))
			ofi_nd_mr_ov_free(&ov->base);
		return;
	}

	assert(ov->eq);
	assert(ov->fid);
	assert(ov->fid->fclass == FI_CLASS_MR);

	struct nd_mr *mr = container_of(ov->fid, struct nd_mr, fid.fid);
	assert(mr->mr);
	mr->fid.key = mr->mr->lpVtbl->GetRemoteToken(mr->mr);
	mr->fid.mem_desc = (void *)(uintptr_t)mr->mr->lpVtbl->GetLocalToken(mr->mr);

	struct fi_eq_entry entry = {.fid = ov->fid, .context = ov->context};
	ofi_nd_mr_ov_free(base);

	struct nd_eq_event *err;
	struct nd_eq_event *ev = ofi_nd_buf_alloc_nd_eq_event();
	if (!ev) {
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}
	memset(ev, 0, sizeof(*ev));
	ev->eq_event = FI_MR_COMPLETE;
	ev->operation = entry;
	ofi_nd_eq_push(ov->eq, ev);
	return;

fn_fail:
	err = ofi_nd_buf_alloc_nd_eq_event();
	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "failed to allocate error event\n");
		return;
	}
	memset(err, 0, sizeof(*err));
	err->error.err = -H2F(hr);
	err->error.prov_errno = (int)hr;
	err->error.fid = ov->fid;
	ofi_nd_eq_push_err(ov->eq, err);
}

static void ofi_nd_mr_ov_err(struct nd_event_base* base, DWORD bytes,
			     DWORD error)
{
	OFI_UNUSED(bytes);

	ofi_nd_mr_ov *ov = container_of(base, ofi_nd_mr_ov, base);

	assert(ov->eq);
	assert(ov->fid);
	assert(ov->fid->fclass == FI_CLASS_MR);

	struct nd_mr *mr = container_of(ov->fid, struct nd_mr, fid.fid);
	assert(mr->mr);

	struct fi_eq_err_entry entry = {
		.fid = ov->fid,
		.context = ov->context,
		.err = H2F(error),
		.prov_errno = error
	};

	ofi_nd_mr_ov_free(base);

	struct nd_eq_event *err = ofi_nd_buf_alloc_nd_eq_event();
	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "failed to allocate error event\n");
		return;
	}
	memset(err, 0, sizeof(*err));
	err->error = entry;
	ofi_nd_eq_push_err(ov->eq, err);
}


#endif /* _WIN32 */

