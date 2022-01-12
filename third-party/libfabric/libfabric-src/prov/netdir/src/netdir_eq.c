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
#include "netdir_log.h"
#include "netdir_iface.h"

#include "ofi_util.h"

static int ofi_nd_eq_close(struct fid *fid);
static ssize_t ofi_nd_eq_read(struct fid_eq *eq, uint32_t *event,
			      void *buf, size_t len, uint64_t flags);
static ssize_t ofi_nd_eq_write(struct fid_eq *eq, uint32_t ev,
			       const void *buf, size_t len, uint64_t flags);
static ssize_t ofi_nd_eq_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf,
				 uint64_t flags);
static ssize_t ofi_nd_eq_sread(struct fid_eq *eq, uint32_t *event,
			       void *buf, size_t len, int timeout,
			       uint64_t flags);
static const char *ofi_nd_eq_strerror(struct fid_eq *eq, int prov_errno,
				      const void *err_data, char *buf,
				      size_t len);

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_eq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_eq ofi_nd_eq_ops = {
	.size = sizeof(ofi_nd_eq_ops),
	.read = ofi_nd_eq_read,
	.readerr = ofi_nd_eq_readerr,
	.write = ofi_nd_eq_write,
	.sread = ofi_nd_eq_sread,
	.strerror = ofi_nd_eq_strerror
};

int ofi_nd_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		   struct fid_eq **peq, void *context)
{
	assert(fabric);
	assert(fabric->fid.fclass == FI_CLASS_FABRIC);

	if (attr) {
		if (attr->wait_obj != FI_WAIT_NONE && attr->wait_obj != FI_WAIT_UNSPEC)
			return -FI_EBADFLAGS;
	}

	struct nd_eq *eq = (struct nd_eq*)calloc(1, sizeof(*eq));
	if (!eq)
		return -FI_ENOMEM;

	struct nd_eq def = {
		.fid = {
			.fid = {
				.fclass = FI_CLASS_EQ,
				.context = context,
				.ops = &ofi_nd_fi_ops
			},
			.ops = &ofi_nd_eq_ops
		}
	};

	*eq = def;

	InitializeCriticalSection(&eq->lock);

	eq->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!eq->iocp || eq->iocp == INVALID_HANDLE_VALUE) {
		ofi_nd_eq_close(&eq->fid.fid);
		return H2F(HRESULT_FROM_WIN32(GetLastError()));
	}

	eq->err = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!eq->err || eq->err == INVALID_HANDLE_VALUE) {
		ofi_nd_eq_close(&eq->fid.fid);
		return H2F(HRESULT_FROM_WIN32(GetLastError()));
	}

	*peq = &eq->fid;
	
	return FI_SUCCESS;
}

static int ofi_nd_eq_close(struct fid *fid)
{
	assert(fid->fclass == FI_CLASS_EQ);

	struct nd_eq *eq = container_of(fid, struct nd_eq, fid.fid);

	if (eq->iocp && eq->iocp != INVALID_HANDLE_VALUE)
		CloseHandle(eq->iocp);
	if (eq->err && eq->err != INVALID_HANDLE_VALUE)
		CloseHandle(eq->err);

	DeleteCriticalSection(&eq->lock);

	free(eq);
	return FI_SUCCESS;
}

static inline ssize_t ofi_nd_eq_ev2buf(struct nd_eq_event *ev,
				       void *buf, size_t len)
{
	assert(ev);

	size_t copylen = 0;
	char* dst = (char *)buf;

	if (!ev->is_custom) {
		switch (ev->eq_event) {
		case FI_CONNREQ:
		case FI_CONNECTED:
		case FI_SHUTDOWN:
			copylen = min(sizeof(struct fi_eq_cm_entry), len);
			break;
		case FI_AV_COMPLETE:
		case FI_MR_COMPLETE:
			copylen = min(sizeof(struct fi_eq_entry), len);
			break;
		default:
			ND_LOG_WARN(FI_LOG_EQ, "unknown event type: %d\n",
				   ev->eq_event);
			copylen = min(sizeof(struct fi_eq_entry), len);
			break;
		}
	}

	if (copylen)
		memcpy(dst, &ev->operation, copylen);

	if (ev->len) {
		assert(ev->data);
		if (len > copylen) {
			dst += copylen;
			memcpy(dst, ev->data, min(len - copylen, ev->len));
			copylen += min(len - copylen, ev->len);
		}
	}
	return (ssize_t)copylen;
}

static ssize_t ofi_nd_eq_read(struct fid_eq *peq, uint32_t *pev,
			      void *buf, size_t len, uint64_t flags)
{
	assert(peq);
	assert(pev);
	assert(peq->fid.fclass == FI_CLASS_EQ);

	struct nd_eq *eq = container_of(peq, struct nd_eq, fid);

	DWORD bytes;
	ULONG_PTR key;
	OVERLAPPED *ov;
	ssize_t res = 0;

	struct nd_eq_event *ev = 0;

	if (!eq->count)
		return -FI_EAGAIN;

	/* we have to use critical section here because concurrent thread
	   may read event with FI_PEEK flag */
	EnterCriticalSection(&eq->lock);

	/* check again because it may be changed on critical section barrier */
	if (!eq->count) {
		res = -FI_EAGAIN;
		goto fn_complete;
	}

	/* if there is peeked item - use it, else - try to read from queue */
	if (eq->peek) {
		ev = eq->peek;
	}
	else {
		assert(eq->iocp);
		if (GetQueuedCompletionStatus(eq->iocp, &bytes, &key, &ov, 0)) {
			ev = container_of(ov, struct nd_eq_event, ov);
		}
	}

	/* in case if no event available, but counter is non-zero - error available */
	if (!ev && eq->count) {
		res = -FI_EAVAIL;
		goto fn_complete;
	}

	res = ofi_nd_eq_ev2buf(ev, buf, len);
	*pev = ev->eq_event;

	if (flags & FI_PEEK) {
		eq->peek = ev;
		/* we updated peek ptr, notify other waiters about this */
		WakeByAddressAll((void*)&eq->count);
	}
	else {
		eq->peek = NULL;
		InterlockedDecrement(&eq->count);
		assert(eq->count >= 0);
	}

fn_complete:
	LeaveCriticalSection(&eq->lock);
	return res;
}

static ssize_t ofi_nd_eq_readerr(struct fid_eq *peq,
				 struct fi_eq_err_entry *buf, uint64_t flags)
{
	assert(peq);
	assert(peq->fid.fclass == FI_CLASS_EQ);
	assert(buf);

	OFI_UNUSED(flags);

	struct nd_eq *eq = container_of(peq, struct nd_eq, fid);

	DWORD bytes;
	ULONG_PTR key;
	OVERLAPPED *ov;

	struct nd_eq_event *ev = NULL;

	if (!eq->errdata) {
		free(eq->errdata);
		eq->errdata = NULL;
	}

	assert(eq->err);
	if (!GetQueuedCompletionStatus(eq->err, &bytes, &key, &ov, 0))
		return -FI_EAGAIN;

	InterlockedDecrement(&eq->count);
	assert(eq->count >= 0);
	ev = container_of(ov, struct nd_eq_event, ov);

	if (buf->err_data && buf->err_data_size) {
		memcpy(buf, &ev->error, offsetof(struct fi_eq_err_entry, err_data));
		buf->err_data_size = MIN(buf->err_data_size, ev->error.err_data_size);
		memcpy(buf->err_data, ev->error.err_data, buf->err_data_size);
		/* to be sure that the errdata in EQ is NULL */
		eq->errdata = NULL;
	} else {
		/* for compatibility purposes (release < 1.5 or passed err_data_size is 0) */
		memcpy(buf, &ev->error, sizeof(ev->error));
		eq->errdata = ev->error.err_data;
	}

	return 0;
}

static ssize_t ofi_nd_eq_sread(struct fid_eq *peq, uint32_t *pev,
			       void *buf, size_t len, int timeout,
			       uint64_t flags)
{
	assert(peq);
	assert(pev);
	assert(peq->fid.fclass == FI_CLASS_EQ);

	struct nd_eq *eq = container_of(peq, struct nd_eq, fid);

	DWORD bytes;
	ULONG_PTR key;
	OVERLAPPED *ov;
	ssize_t res = 0;

	struct nd_eq_event *ev = 0;

	LONG zero = 0;

	for (;;) {
		do {
			if (!WaitOnAddress(
				&eq->count, &zero, sizeof(eq->count),
				(DWORD)timeout) && timeout >= 0)
				return -FI_EAGAIN;
		} while (!eq->count);

		/* we have to use critical section here because concurrent thread
		may read event with FI_PEEK flag */
		EnterCriticalSection(&eq->lock);

		if (!eq->count) {
			LeaveCriticalSection(&eq->lock);
			if (timeout >= 0)
				return -FI_EAGAIN;
			else
				continue;
		}

		/* if there is peeked item - use it, else - try to read from queue */
		if (eq->peek) {
			ev = eq->peek;
		}
		else {
			assert(eq->iocp);
			if (GetQueuedCompletionStatus(
				eq->iocp, &bytes, &key, &ov, 0)) {
				ev = container_of(ov, struct nd_eq_event, ov);
			}
		}

		/* in case if no event available, but counter is non-zero - error available */
		if (!ev && eq->count) {
			res = -FI_EAVAIL;
			goto fn_complete;
		}

		res = ofi_nd_eq_ev2buf(ev, buf, len);
		*pev = ev->eq_event;

		if (flags & FI_PEEK) {
			eq->peek = ev;
			/* we updated peek ptr, notify other waiters about this */
			WakeByAddressAll((void*)&eq->count);
		}
		else {
			eq->peek = NULL;
			InterlockedDecrement(&eq->count);
			assert(eq->count >= 0);
		}

fn_complete:
		LeaveCriticalSection(&eq->lock);
		return res;
	}
}

static const char *ofi_nd_eq_strerror(struct fid_eq *eq, int prov_errno,
				      const void *err_data, char *buf, size_t len)
{
	OFI_UNUSED(eq);
	OFI_UNUSED(err_data);

	if (buf && len)
		return strncpy(buf, fi_strerror(-prov_errno), len);
	return fi_strerror(-prov_errno);
}

static ssize_t ofi_nd_eq_write(struct fid_eq *peq, uint32_t ev,
			       const void *buf, size_t len, uint64_t flags)
{
	OFI_UNUSED(flags);

	assert(peq);
	assert(peq->fid.fclass == FI_CLASS_EQ);

	struct nd_eq *eq = container_of(peq, struct nd_eq, fid);

	nd_eq_event *custom = ofi_nd_buf_alloc_nd_eq_event();
	if (!custom)
		return -FI_ENOMEM;
	memset(custom, 0, sizeof(*custom));

	custom->is_custom = 1;
	custom->eq_event = ev;
	if (len) {
		assert(buf);
		custom->data = malloc(len);
		if (!custom->data) {
			ofi_nd_eq_free_event(custom);
			return -FI_ENOMEM;
		}
		custom->len = len;
		memcpy(custom->data, buf, len);
	}

	ofi_nd_eq_push(eq, custom);

	return len;
}


#endif /* _WIN32 */

