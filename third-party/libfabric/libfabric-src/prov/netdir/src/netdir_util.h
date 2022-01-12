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

#ifndef _FI_NETDIR_UTIL_H_
#define _FI_NETDIR_UTIL_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <assert.h>

#include "ndspi.h"

#include "rdma/fabric.h"
#include "ofi_mem.h"

#include "netdir.h"
#include "netdir_buf.h"
#include "netdir_ov.h"
#include "netdir_log.h"
#include "netdir_iface.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct ofi_nd_util_ov {
	nd_event_base	base;
	LONG		cnt;
} ofi_nd_util_ov;

OFI_ND_NB_BUF(ofi_nd_util_ov);

static HRESULT ofi_nd_util_ov_wait(void *overlapped, ofi_nd_util_ov *ov)
{
	assert(overlapped);
	assert(ov);

	HRESULT hr = ((IND2Overlapped*)overlapped)->lpVtbl->GetOverlappedResult(overlapped, &ov->base.ov, TRUE);
	if (!InterlockedDecrement(&ov->cnt))
		ov->base.free(&ov->base);
	return hr;
}

static inline HRESULT
ofi_nd_util_register_mr(IND2MemoryRegion *mr, const void *buffer, size_t len, DWORD flags)
{
	HRESULT hr = S_OK;

	ofi_nd_util_ov *ov = ND_BUF_ALLOC(ofi_nd_util_ov);
	if (!ov) {
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}

	hr = mr->lpVtbl->Register(mr, buffer, len, flags, &ov->base.ov);
	if (FAILED(hr))
		goto fn_fail_ov;

	hr = ofi_nd_util_ov_wait(mr, ov);
	if (FAILED(hr))
		goto fn_fail_ov;

	return S_OK;

fn_fail_ov:
	ov->base.free(&ov->base);
fn_fail:
	return hr;
}

static inline HRESULT
ofi_nd_util_unregister_mr(IND2MemoryRegion *mr)
{
	HRESULT hr = S_OK;

	ofi_nd_util_ov *ov = ND_BUF_ALLOC(ofi_nd_util_ov);
	if (!ov) {
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}

	hr = mr->lpVtbl->Deregister(mr, &ov->base.ov);
	if (FAILED(hr))
		goto fn_fail_ov;

	hr = ofi_nd_util_ov_wait(mr, ov);
	if (FAILED(hr))
		goto fn_fail_ov;

	return S_OK;

fn_fail_ov:
	ov->base.free(&ov->base);
fn_fail:
	return hr;
}

static inline int
ofi_nd_util_can_be_inlined(const struct iovec *iov, size_t iovlen)
{
	assert(iov);

	size_t i;
	for (i = 0; i < iovlen; i++)
		if (iov[i].iov_len > (size_t)gl_data.inline_thr)
			return 0;
	return 1;
}

/* return 1 if notification should be */
static inline int
ofi_nd_util_completion_blackmagic(uint64_t info_flags,
				  uint64_t cq_flags,
				  uint64_t op_flags)
{
	OFI_UNUSED(info_flags);
	if ((op_flags & FI_COMPLETION) || 
	    (op_flags & (FI_INJECT_COMPLETE |
			 FI_TRANSMIT_COMPLETE |
			 FI_DELIVERY_COMPLETE)))
		return 1;
	else if (op_flags & FI_INJECT)
		return 0;
	else if (!(cq_flags & FI_SELECTIVE_COMPLETION))
		return 1;
	else
		return 0;
}

static inline
void ofi_nd_free_send_entry(nd_send_entry *entry)
{
	assert(entry);

	if (entry->sge)
		ND_BUF_FREE(nd_sge, entry->sge);

	ND_BUF_FREE(nd_send_entry, entry);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_UTIL_H_ */

