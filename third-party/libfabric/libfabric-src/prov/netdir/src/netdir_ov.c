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

#include "netdir_ov.h"
#include "netdir_log.h"
#include "netdir_util.h"
#include "netdir_iface.h"

#include "netdir_queue.h"

OFI_ND_NB_BUF_IMP(nd_eq_event);
OFI_ND_NB_BUF_IMP(nd_buf_fini);

volatile nd_buf_fini *nd_buf_fini_head = 0;

LONG nd_async_progress = 0;

void CALLBACK domain_io_cb(DWORD err, DWORD bytes, LPOVERLAPPED ov)
{
	assert(ov);

	InterlockedIncrement(&nd_async_progress);

	nd_event_base *base = container_of(ov, nd_event_base, ov);

	ND_LOG_DEBUG(FI_LOG_EP_CTRL,
		"IO callback: err: %s, bytes: %d\n",
		ofi_nd_error_str(err), bytes);

	if (err) {
		assert(base->err_cb);
		base->err_cb(base, bytes, err);
	}
	else {
		assert(base->event_cb);
		base->event_cb(base, bytes);
	}

	InterlockedDecrement(&nd_async_progress);

	return;
}

static void ofi_nd_util_mr_ov_free(struct nd_event_base* base)
{
	assert(base);
	ofi_nd_util_ov *ov = container_of(base, ofi_nd_util_ov, base);
	ov->cnt = 2;
	ResetEvent(ov->base.ov.hEvent);
	ND_BUF_FREE(ofi_nd_util_ov, ov);
}

static void ofi_nd_util_mr_ov_event(struct nd_event_base* base, DWORD bytes)
{
	OFI_UNUSED(bytes);

	ofi_nd_util_ov *ov = container_of(base, ofi_nd_util_ov, base);

	if (!InterlockedDecrement(&ov->cnt))
		ov->base.free(&ov->base);
}

static void ofi_nd_util_mr_ov_err(struct nd_event_base* base, DWORD bytes,
				  DWORD error)
{
	OFI_UNUSED(bytes);
	OFI_UNUSED(error);

	ofi_nd_util_ov *ov = container_of(base, ofi_nd_util_ov, base);

	if (!InterlockedDecrement(&ov->cnt))
		ov->base.free(&ov->base);
}

static ND_BUF_CHUNK(ofi_nd_util_ov)
*ofi_nd_alloc_ov_chunk(ND_BUF_FOOTER(ofi_nd_util_ov) *footer, size_t *count)
{
	OFI_UNUSED(footer);

	ND_BUF_CHUNK(ofi_nd_util_ov) *chunk = malloc(sizeof(*chunk));
	if (!chunk)
		return 0;
	assert(count);
	*count = countof(chunk->item);
	memset(chunk, 0, sizeof(*chunk));

	size_t i;
	for (i = 0; i < countof(chunk->item); i++) {
		chunk->item[i].data.cnt = 2;
		chunk->item[i].data.base.free = ofi_nd_util_mr_ov_free;
		chunk->item[i].data.base.event_cb = ofi_nd_util_mr_ov_event;
		chunk->item[i].data.base.err_cb = ofi_nd_util_mr_ov_err;
		chunk->item[i].data.base.ov.hEvent = CreateEvent(0, TRUE, FALSE, 0);
		if (!chunk->item[i].data.base.ov.hEvent ||
		    chunk->item[i].data.base.ov.hEvent == INVALID_HANDLE_VALUE)
			goto fn_fail;
	}

	return chunk;

fn_fail:
	for (i = 0; i < countof(chunk->item); i++) {
		if (chunk->item[i].data.base.ov.hEvent &&
		    chunk->item[i].data.base.ov.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(chunk->item[i].data.base.ov.hEvent);
	}
	free(chunk);
	return 0;
}

static void ofi_nd_free_ov_chunk(struct nd_buf_chunk_ofi_nd_util_ov *chunk)
{
	assert(chunk);
	size_t i;
	for(i = 0; i < countof(chunk->item); i++)
		if (chunk->item[i].data.base.ov.hEvent &&
		    chunk->item[i].data.base.ov.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(chunk->item[i].data.base.ov.hEvent);
	free(chunk);
}

OFI_ND_NB_BUF_IMP_ALLOC(ofi_nd_util_ov, ofi_nd_alloc_ov_chunk, ofi_nd_free_ov_chunk);

#endif /* _WIN32 */

