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
#include "netdir_cq.h"
#include "netdir_iface.h"
#include "netdir_unexp.h"

#include "rdma/fabric.h"
#include "ofi_util.h"

static int ofi_nd_cq_close(struct fid *fid);
static ssize_t ofi_nd_cq_read(struct fid_cq *cq, void *buf, size_t count);
static ssize_t ofi_nd_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
				  fi_addr_t *src_addr);
static ssize_t ofi_nd_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
				 uint64_t flags);
static ssize_t ofi_nd_cq_sread(struct fid_cq *cq, void *buf, size_t count,
			       const void *cond, int timeout);
static ssize_t ofi_nd_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
				   fi_addr_t *src_addr, const void *cond,
				   int timeout);
static const char *ofi_nd_cq_strerror(struct fid_cq *cq, int prov_errno,
				      const void *err_data, char *buf,
				      size_t len);
ssize_t ofi_nd_ep_injectdata(struct fid_ep *ep, const void *buf, size_t len,
			     uint64_t data, fi_addr_t dest_addr);

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_cq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_CQ,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

static struct fi_ops_cq ofi_nd_cq_ops = {
	.size = sizeof(ofi_nd_cq_ops),
	.read = ofi_nd_cq_read,
	.readfrom = ofi_nd_cq_readfrom,
	.readerr = ofi_nd_cq_readerr,
	.sread = ofi_nd_cq_sread,
	.sreadfrom = ofi_nd_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = ofi_nd_cq_strerror
};

typedef struct nd_cq_action {
	nd_event_base	base;
	struct nd_cq	*cq;
} nd_cq_action;

OFI_ND_NB_BUF(nd_cq_action);

OFI_ND_NB_BUF_IMP(nd_cq_action);
OFI_ND_NB_BUF_IMP(nd_cq_entry);

OFI_ND_NB_BUF_IMP(nd_send_entry);
OFI_ND_NB_BUF_IMP(nd_sge);

/* State-Event matrix callbacks. Must have the following signature 
 * "void <func_name>(nd_cq_entry*, void*)" */
static inline void ofi_nd_handle_unknown(nd_cq_entry *entry, void *misc);
static inline void ofi_nd_unexp_2_read(nd_cq_entry *entry, void *unexpected);
static inline void ofi_nd_read_2_send_ack(nd_cq_entry *entry, void *res);
static inline void ofi_nd_event_2_cq(nd_cq_entry *entry, void *misc);
static inline void ofi_nd_unexp_ack_2_cq(nd_cq_entry *entry, void *unexpected);

/* Auxillary functions to ensure workability of callbacks of S-E matrix
 * and are used implicitly inside these callbakcs */
static void ofi_nd_unexp_2_cq(nd_cq_entry *entry, nd_unexpected_entry *unexp);
static void ofi_nd_read_2_cq(nd_cq_entry *entry, ND2_RESULT *result);

#define UNKNWN ofi_nd_handle_unknown
#define UNEXP_2_READ ofi_nd_unexp_2_read
#define READ_2_SENDACK ofi_nd_read_2_send_ack
#define EVENT_2_CQ ofi_nd_event_2_cq
#define UNEXP_ACK_2_CQ ofi_nd_unexp_ack_2_cq

typedef void(*cq_matrix_cb)(struct nd_cq_entry *entry, void *misc);

static cq_matrix_cb cq_matrix[MAX_STATE][MAX_EVENT] = {
	{ EVENT_2_CQ,	UNEXP_2_READ,	UNKNWN		},
	{ UNKNWN,	READ_2_SENDACK,	UNKNWN		},
	{ UNKNWN,	UNKNWN,		UNEXP_ACK_2_CQ	}
};

void ofi_nd_dispatch_cq_event(ofi_nd_cq_event event, nd_cq_entry *entry, void *misc)
{
	ofi_nd_cq_state state = entry->state;
	cq_matrix_cb cb = cq_matrix[state][event];
	ND_LOG_DEBUG(FI_LOG_EP_DATA, "Dispatch Event - %d:%d\n",
		     state, event);
	cb(entry, misc);
}

static int ofi_nd_cq_close(struct fid *fid)
{
	assert(fid->fclass == FI_CLASS_CQ);
	if (fid->fclass != FI_CLASS_CQ)
		return -FI_EINVAL;

	struct nd_cq *cq = container_of(fid, struct nd_cq, fid.fid);

	if (cq->iocp && cq->iocp != INVALID_HANDLE_VALUE)
		CloseHandle(cq->iocp);
	if (cq->err && cq->err != INVALID_HANDLE_VALUE)
		CloseHandle(cq->err);

	free(cq);

	return FI_SUCCESS;
}

int ofi_nd_cq_open(struct fid_domain *pdomain, struct fi_cq_attr *attr,
		   struct fid_cq **pcq_fid, void *context)
{
	OFI_UNUSED(context);

	assert(pdomain);
	assert(pdomain->fid.fclass == FI_CLASS_DOMAIN);

	if (pdomain->fid.fclass != FI_CLASS_DOMAIN)
		return -FI_EINVAL;

	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_cq_action));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_cq_entry));

	if (pdomain->fid.fclass != FI_CLASS_DOMAIN)
		return -FI_EINVAL;

	HRESULT hr;

	if (attr) {
		if (attr->wait_obj != FI_WAIT_NONE &&
		    attr->wait_obj != FI_WAIT_UNSPEC)
			return -FI_EBADFLAGS;
	}

	struct nd_cq *cq = (struct nd_cq*)calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	struct nd_cq def = {
		.fid = {
			.fid = ofi_nd_fid,
			.ops = &ofi_nd_cq_ops
		},
		.format = attr ? attr->format : FI_CQ_FORMAT_CONTEXT
	};

	*cq = def;

	if (cq->format == FI_CQ_FORMAT_UNSPEC) {
		cq->format = FI_CQ_FORMAT_CONTEXT;
		if (attr)
			attr->format = cq->format;
	}

	struct nd_domain *domain = container_of(pdomain, struct nd_domain, fid);
	assert(domain->adapter);
	assert(domain->adapter_file);

	cq->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!cq->iocp || cq->iocp == INVALID_HANDLE_VALUE) {
		hr = -FI_EINVAL;
		goto hr_fail;
	}
	cq->err = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!cq->err || cq->err == INVALID_HANDLE_VALUE) {
		hr = -FI_EINVAL;
		goto hr_fail;
	}

	*pcq_fid = &cq->fid;

	return FI_SUCCESS;

hr_fail:
	ofi_nd_cq_close(&cq->fid.fid);
	ND_LOG_WARN(FI_LOG_CQ, ofi_nd_strerror((DWORD)hr, NULL));
	return H2F(hr);
}

static uint64_t ofi_nd_cq_sanitize_flags(uint64_t flags)
{
	return (flags & (FI_SEND | FI_RECV | FI_RMA | FI_ATOMIC |
		FI_MSG | FI_TAGGED |
		FI_READ | FI_WRITE |
		FI_REMOTE_READ | FI_REMOTE_WRITE |
		FI_REMOTE_CQ_DATA | FI_MULTI_RECV));
}

static void ofi_nd_cq_ov2buf(struct nd_cq *cq, OVERLAPPED_ENTRY *ov,
			     void* buf, ULONG count)
{
	ULONG i;
	struct nd_msgprefix *prefix;

	switch (cq->format) {
	case FI_CQ_FORMAT_CONTEXT:
		{
			struct fi_cq_entry *entry = (struct fi_cq_entry*)buf;
			for (i = 0; i < count; i++) {
				struct nd_cq_entry *cqen = container_of(ov[i].lpOverlapped, struct nd_cq_entry, base.ov);
				entry[i].op_context = cqen->context;
				ofi_nd_free_cq_entry(cqen);
			}
		}
		break;
	case FI_CQ_FORMAT_MSG:
		{
			struct fi_cq_msg_entry *entry = (struct fi_cq_msg_entry*)buf;
			for (i = 0; i < count; i++) {
				struct nd_cq_entry *cqen = container_of(ov[i].lpOverlapped, struct nd_cq_entry, base.ov);
				entry[i].op_context = cqen->context;
				entry[i].flags = ofi_nd_cq_sanitize_flags(cqen->flags);
				/* for send/receive operations there message header used,
				   and common size of transferred message is bit
				   bigger, in this case decrement transferred message
				   size by header size */
				size_t header_len = (cqen->result.RequestType == Nd2RequestTypeSend ||
						     cqen->result.RequestType == Nd2RequestTypeReceive) ?
					sizeof(prefix->header) : 0;
				entry[i].len = cqen->result.BytesTransferred - header_len;
				ofi_nd_free_cq_entry(cqen);
			}
		}
		break;
	case FI_CQ_FORMAT_DATA:
		{
			struct fi_cq_data_entry *entry = (struct fi_cq_data_entry*)buf;
			for (i = 0; i < count; i++) {
				struct nd_cq_entry *cqen = container_of(ov[i].lpOverlapped, struct nd_cq_entry, base.ov);
				entry[i].op_context = cqen->context;
				entry[i].flags = ofi_nd_cq_sanitize_flags(cqen->flags);
				size_t header_len = (cqen->result.RequestType == Nd2RequestTypeSend ||
						     cqen->result.RequestType == Nd2RequestTypeReceive) ?
					sizeof(prefix->header) : 0;
				entry[i].len = cqen->result.BytesTransferred - header_len;
				entry[i].buf = cqen->buf;
				ofi_nd_free_cq_entry(cqen);
			}
		}
		break;
	case FI_CQ_FORMAT_TAGGED:
		{
			struct fi_cq_tagged_entry *entry = (struct fi_cq_tagged_entry*)buf;
			for (i = 0; i < count; i++) {
				struct nd_cq_entry *cqen = container_of(ov[i].lpOverlapped, struct nd_cq_entry, base.ov);
				entry[i].op_context = cqen->context;
				entry[i].flags = ofi_nd_cq_sanitize_flags(cqen->flags);
				size_t header_len = (cqen->result.RequestType == Nd2RequestTypeSend ||
						     cqen->result.RequestType == Nd2RequestTypeReceive) ?
					sizeof(prefix->header) : 0;
				entry[i].len = cqen->result.BytesTransferred - header_len;
				entry[i].buf = cqen->buf;
				entry[i].tag = 0;
				ofi_nd_free_cq_entry(cqen);
			}
		}
		break;
	default:
		ND_LOG_WARN(FI_LOG_CQ, "incorrect CQ format: %d\n", cq->format);
		break;
	}
}

static ssize_t ofi_nd_cq_read(struct fid_cq *pcq, void *buf, size_t count)
{
	assert(pcq);
	assert(pcq->fid.fclass == FI_CLASS_CQ);

	if (pcq->fid.fclass != FI_CLASS_CQ)
		return -FI_EINVAL;

	struct nd_cq *cq = container_of(pcq, struct nd_cq, fid);

	ULONG cnt = (ULONG)count;
	ULONG dequeue = 0;
	ssize_t res = 0;
	OVERLAPPED_ENTRY _ov[256];

	if (!cq->count)
		return -FI_EAGAIN;

	OVERLAPPED_ENTRY *ov = (cnt <= countof(_ov)) ?
		_ov : malloc(cnt * sizeof(*ov));

	if (!ov) {
		ND_LOG_WARN(FI_LOG_CQ, "failed to allocate OV\n");
		return -FI_ENOMEM;
	}

	assert(cq->iocp && cq->iocp != INVALID_HANDLE_VALUE);
	if (!GetQueuedCompletionStatusEx(cq->iocp, ov, cnt, &dequeue, 0, FALSE) ||
	    !dequeue) {
		res = cq->count ? -FI_EAVAIL : -FI_EAGAIN;
		goto fn_complete;
	}

	ofi_nd_cq_ov2buf(cq, ov, buf, dequeue);
	res = (ssize_t)dequeue;
	InterlockedAdd(&cq->count, -(LONG)dequeue);
	assert(cq->count >= 0);

fn_complete:
	if (ov != _ov)
		free(ov);
	return res;
}

static ssize_t ofi_nd_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
				  fi_addr_t *src_addr)
{
	size_t i;
	for(i = 0; i < count; i++)
		src_addr[i] = FI_ADDR_NOTAVAIL;
	return ofi_nd_cq_read(cq, buf, count);
}

static ssize_t ofi_nd_cq_readerr(struct fid_cq *pcq, struct fi_cq_err_entry *buf,
				 uint64_t flags)
{
	assert(pcq);
	assert(pcq->fid.fclass == FI_CLASS_CQ);
	assert(buf);

	OFI_UNUSED(flags);

	if (pcq->fid.fclass != FI_CLASS_CQ)
		return -FI_EINVAL;

	struct nd_cq *cq = container_of(pcq, struct nd_cq, fid);

	ULONG_PTR key = 0;
	DWORD bytes;
	OVERLAPPED *ov;

	if (!cq->count)
		return -FI_EAGAIN;

	assert(cq->err && cq->err != INVALID_HANDLE_VALUE);
	if (!GetQueuedCompletionStatus(cq->err, &bytes, &key, &ov, 0))
		return -FI_EAGAIN;

	struct nd_cq_entry *entry = container_of(ov, struct nd_cq_entry, base.ov);

	buf->op_context = entry->result.RequestContext;
	buf->flags = entry->flags;
	buf->len = entry->len;
	buf->buf = entry->buf;
	buf->data = entry->data;
	buf->tag = 0; /* while tagged send/recv isn't added */
	buf->olen = 0;
	buf->err = -H2F(entry->result.Status);
	buf->prov_errno = entry->result.Status;
	buf->err_data_size = 0;

	InterlockedDecrement(&cq->count);
	assert(cq->count >= 0);

	return 0;
}

static ssize_t ofi_nd_cq_sread(struct fid_cq *pcq, void *buf, size_t count,
			       const void *cond, int timeout)
{
	assert(pcq);
	assert(pcq->fid.fclass == FI_CLASS_CQ);

	OFI_UNUSED(cond);

	if (pcq->fid.fclass != FI_CLASS_CQ)
		return -FI_EINVAL;

	struct nd_cq *cq = container_of(pcq, struct nd_cq, fid);

	ULONG cnt = (ULONG)count;
	ULONG dequeue = 0;
	ssize_t res = 0;
	OVERLAPPED_ENTRY _ov[256];

	OVERLAPPED_ENTRY *ov = (cnt <= countof(_ov)) ?
		_ov : malloc(cnt * sizeof(*ov));

	if (!ov) {
		ND_LOG_WARN(FI_LOG_CQ, "failed to allocate OV\n");
		return -FI_ENOMEM;
	}

	LONG zero = 0;
	OFI_ND_TIMEOUT_INIT(timeout);

	do {
		do {
			if (!WaitOnAddress(
				&cq->count, &zero, sizeof(cq->count),
				(DWORD)timeout) && timeout >= 0) {
				res = -FI_EAGAIN;
				goto fn_complete;
			}
		} while (!cq->count && !OFI_ND_TIMEDOUT());

		if (!cq->count) {
			res = -FI_EAGAIN;
			goto fn_complete;
		}



		assert(cq->iocp && cq->iocp != INVALID_HANDLE_VALUE);
		if (!GetQueuedCompletionStatusEx(cq->iocp, ov, cnt, &dequeue, 0, FALSE) ||
		    !dequeue) {
			if (cq->count) {
				res = -FI_EAVAIL;
				goto fn_complete;
			}
			else {
				continue;
			}
		}

		ofi_nd_cq_ov2buf(cq, ov, buf, dequeue);
		res = (ssize_t)dequeue;
		InterlockedAdd(&cq->count, -(LONG)dequeue);
		assert(cq->count >= 0);
		goto fn_complete;
	} while (!OFI_ND_TIMEDOUT());

fn_complete:
	if (ov != _ov)
		free(ov);
	return res;
}

static ssize_t ofi_nd_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
				   fi_addr_t *src_addr, const void *cond,
				   int timeout)
{
	size_t i;
	for (i = 0; i < count; i++)
		src_addr[i] = FI_ADDR_NOTAVAIL;
	return ofi_nd_cq_sread(cq, buf, count, cond, timeout);
}

static const char *ofi_nd_cq_strerror(struct fid_cq *cq, int prov_errno,
				      const void *err_data, char *buf,
				      size_t len)
{
	OFI_UNUSED(cq);
	OFI_UNUSED(err_data);

	if (buf && len)
		return strncpy(buf, fi_strerror(-prov_errno), len);
	return fi_strerror(-prov_errno);
}

static void ofi_nd_cq_event_free(nd_event_base *base)
{
	nd_cq_action *action = container_of(base, nd_cq_action, base);
	ND_BUF_FREE(nd_cq_action, action);
}

void ofi_nd_handle_unknown(nd_cq_entry *entry, void *unexp)
{
	OFI_UNUSED(entry);
	OFI_UNUSED(unexp);

	ND_LOG_DEBUG(FI_LOG_CQ, "Unknown event-state, "
		     "the event can't be handled\n");

	/* Shouldn't go here */
	assert(0);

	return;
}

void ofi_nd_event_2_cq(nd_cq_entry *entry, void *misc)
{
	/* Memory region is set in CQ entry only in case of RMA operation.
	 * Use this fact to realize what kind of operation is completed */
	if (!entry->mr_count)
		ofi_nd_unexp_2_cq(entry, (nd_unexpected_entry *)misc);
	else
		ofi_nd_read_2_cq(entry, (ND2_RESULT *)misc);
}

/* do NOT release unexpected here, becuase it just an allocated on stack entry */
void ofi_nd_unexp_ack_2_cq(nd_cq_entry *entry, void *unexpected)
{
	nd_unexpected_entry *unexp = (nd_unexpected_entry *)unexpected;
	struct nd_ep *ep = unexp->ep;
	ND2_RESULT *result = &unexp->result;

	struct nd_cq_entry *parent_entry = entry->aux_entry;

	if (ep->cntr_send) {
		if (result->Status != S_OK) {
			InterlockedIncrement64(&ep->cntr_send->err);
		}
		InterlockedIncrement64(&ep->cntr_send->counter);
		WakeByAddressAll((void*)&ep->cntr_send->counter);
	}

	int notify = ofi_nd_util_completion_blackmagic(
		ep->info->tx_attr->op_flags, ep->send_flags, parent_entry->flags) ||
		result->Status != S_OK;

	if (notify) {
		PostQueuedCompletionStatus(
			parent_entry->result.Status == S_OK ? ep->cq_send->iocp : ep->cq_send->err,
			0, 0, &parent_entry->base.ov);
		InterlockedIncrement(&ep->cq_send->count);
		WakeByAddressAll((void*)&ep->cq_send->count);
	}
	else { /* if notification is not requested - just free entry */
		ofi_nd_free_cq_entry(parent_entry);
	}

	/* This CQ is no longer needed for us, let's release it.
	 * Set address of parent entry (that's used for initial
	 * send opertation) to NULL, it's just to not release it
	 * during free parent CQ entry below */
	entry->aux_entry = NULL;
	ofi_nd_free_cq_entry(entry);
}

void ofi_nd_unexp_2_cq(nd_cq_entry *entry, nd_unexpected_entry *unexp)
{
	assert(entry);
	assert(unexp);
	assert(unexp->ep);
	assert(unexp->ep->fid.fid.fclass == FI_CLASS_EP ||
		unexp->ep->fid.fid.fclass == FI_CLASS_SRX_CTX);

	/* copy data to user's buffer */
	size_t i;
	char *buf = unexp->buf->received_buf.data;

	size_t len = unexp->result.BytesTransferred - sizeof(unexp->buf->header);
	for (i = 0; i < entry->iov_cnt && len; i++) {
		size_t cp = min(entry->iov[i].iov_len, len);
		memcpy(entry->iov[i].iov_base, buf, cp);
		len -= cp;
		buf += cp;
	}

	entry->data = unexp->buf->header.data; /* copy send data */

	int status = (unexp->result.Status == S_OK &&
		(unexp->result.BytesTransferred - sizeof(unexp->buf->header)) <= entry->len) ?
		S_OK : (unexp->result.Status != S_OK ?
			H2F(unexp->result.Status) : FI_ETRUNC);

	struct nd_ep *ep = unexp->ep;

	ofi_nd_release_unexp_entry(unexp);

	if (ep->cntr_recv) {
		if (status != S_OK) {
			InterlockedIncrement64(&ep->cntr_recv->err);
		}
		InterlockedIncrement64(&ep->cntr_recv->counter);
		WakeByAddressAll((void*)&ep->cntr_recv->counter);
	}

	int notify = ofi_nd_util_completion_blackmagic(
		ep->info->rx_attr->op_flags,
		ep->recv_flags, entry->flags);

	if (status == S_OK) {
		if (notify && ep->cq_recv) {
			PostQueuedCompletionStatus(
				ep->cq_recv->iocp, 0, 0, &entry->base.ov);
			InterlockedIncrement(&ep->cq_recv->count);
			WakeByAddressAll((void*)&ep->cq_recv->count);
		}
		else { /* if notification is not requested - just free entry */
			ofi_nd_free_cq_entry(entry);
		}
	}
	else {
		if (ep->cq_recv) {
			PostQueuedCompletionStatus(
				ep->cq_recv->err, 0, 0, &entry->base.ov);
			InterlockedIncrement(&ep->cq_recv->count);
			WakeByAddressAll((void*)&ep->cq_recv->count);
		}
		else { /* TODO add warning here */
			ofi_nd_free_cq_entry(entry);
		}
	}
}

void ofi_nd_unexp_2_read(nd_cq_entry *entry, void *unexpected)
{
	nd_unexpected_entry *unexp = (nd_unexpected_entry *)unexpected;

	assert(entry);
	assert(unexp);
	assert(unexp->ep);
	assert(unexp->ep->fid.fid.fclass == FI_CLASS_EP ||
	       unexp->ep->fid.fid.fclass == FI_CLASS_SRX_CTX);

	size_t location_cnt = unexp->buf->header.location_cnt;
	struct nd_msg_location *locations = unexp->buf->received_buf.locations;
	struct nd_ep *ep = unexp->ep;
	HRESULT hr;
	size_t i;

	ofi_nd_release_unexp_entry(unexp);

	struct nd_cq_entry *rma_entries[ND_MSG_IOV_LIMIT];

	struct iovec from_iovecs[ND_MSG_IOV_LIMIT];
	for (i = 0; i < location_cnt; i++) {
		from_iovecs[i].iov_base = (void *)locations[i].addr;
		from_iovecs[i].iov_len = locations[i].len;
	}

	struct iovec new_iovecs[ND_MSG_INTERNAL_IOV_LIMIT];
	size_t new_iovecs_count = 0;
	size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT];
	size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT];
	uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT];

	ofi_nd_repack_iovecs(from_iovecs, location_cnt,
			     entry->iov, entry->iov_cnt,
			     new_iovecs, &new_iovecs_count,
			     from_split_map, to_split_map, remote_addr);
	assert(new_iovecs_count <= ND_MSG_INTERNAL_IOV_LIMIT);

	entry->wait_completion.comp_count = 0;
	entry->wait_completion.total_count = new_iovecs_count;

	InitializeCriticalSection(&entry->wait_completion.comp_lock);

	for (i = 0; i < new_iovecs_count; i++) {
		rma_entries[i] = ofi_nd_buf_alloc_nd_cq_entry();
		if (!rma_entries[i])
			goto fn_fail_alloc;
		memset(rma_entries[i], 0, sizeof(*rma_entries[i]));

		rma_entries[i]->len = entry->len;
		rma_entries[i]->data = entry->data;
		rma_entries[i]->flags = entry->flags;
		rma_entries[i]->domain = entry->domain;
		rma_entries[i]->context = entry->context;
		rma_entries[i]->iov[0].iov_base = new_iovecs[i].iov_base;
		rma_entries[i]->iov[0].iov_len = new_iovecs[i].iov_len;
		rma_entries[i]->iov_cnt = 1;
		rma_entries[i]->seq = entry->seq;
		/* Store native CQ entry not to be forgotten for free */
		rma_entries[i]->aux_entry = entry;
		rma_entries[i]->rma_location.count = location_cnt;
		rma_entries[i]->rma_location.locations = locations;

		void *tobuf = rma_entries[i]->iov[0].iov_base;
		ULONG tobuf_len = (ULONG)rma_entries[i]->iov[0].iov_len;

		hr = ep->domain->adapter->lpVtbl->CreateMemoryRegion(
			ep->domain->adapter, &IID_IND2MemoryRegion,
			ep->domain->adapter_file, (void**)&rma_entries[i]->mr[0]);
		if (FAILED(hr))
			goto fn_fail;
		rma_entries[i]->mr_count = 1;

		hr = ofi_nd_util_register_mr(
			rma_entries[i]->mr[0], tobuf, tobuf_len,
			ND_MR_FLAG_ALLOW_LOCAL_WRITE |
			ND_MR_FLAG_ALLOW_REMOTE_READ |
			ND_MR_FLAG_ALLOW_REMOTE_WRITE);
		if (FAILED(hr))
			goto fn_fail;

		ND2_SGE sge = {
			.Buffer = tobuf,
			.BufferLength = tobuf_len,
			.MemoryRegionToken = rma_entries[i]->mr[0]->lpVtbl->GetLocalToken(rma_entries[i]->mr[0])
		};

		rma_entries[i]->state = LARGE_MSG_RECV_REQ;

		hr = ep->qp->lpVtbl->Read(ep->qp, rma_entries[i], &sge, 1,
			remote_addr[i], locations[from_split_map[i]].remote_mr_token, 0);
		if (FAILED(hr))
			goto fn_fail;
	}
	return;

fn_fail_alloc:
	while (i--)
		ofi_nd_free_cq_entry(rma_entries[i]);
	/* TODO: generate cq_err if RMA read fails */
	ND_LOG_WARN(FI_LOG_EP_DATA, ofi_nd_strerror((DWORD)hr, NULL));
	return;
fn_fail:
	while (i)
		ofi_nd_free_cq_entry(rma_entries[i--]);
	/* TODO: generate cq_err if RMA read fails */
	ND_LOG_WARN(FI_LOG_EP_DATA, ofi_nd_strerror((DWORD)hr, NULL));
}

void ofi_nd_read_2_cq(nd_cq_entry *entry, ND2_RESULT *result)
{
	struct nd_ep *ep = (struct nd_ep*)result->QueuePairContext;

	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);
	assert(entry);

	if (entry->inline_buf) {
		assert(result->BytesTransferred <= (ULONG)gl_data.inline_thr);
		assert(!entry->mr);
		memcpy(entry->iov[0].iov_base, entry->inline_buf->buffer,
			result->BytesTransferred);
	}

	if (ep->cntr_read) {
		if (result->Status != S_OK) {
			InterlockedIncrement64(&ep->cntr_read->err);
		}
		InterlockedIncrement64(&ep->cntr_read->counter);
		WakeByAddressAll((void*)&ep->cntr_read->counter);
	}

	int notify = ofi_nd_util_completion_blackmagic(
		ep->info->rx_attr->op_flags, ep->recv_flags, entry->flags) ||
		result->Status != S_OK;

	if (notify) {
		PostQueuedCompletionStatus(
			entry->result.Status == S_OK ? ep->cq_recv->iocp : ep->cq_recv->err,
			0, 0, &entry->base.ov);
		InterlockedIncrement(&ep->cq_recv->count);
		WakeByAddressAll((void*)&ep->cq_recv->count);
	}
	else { /* if notification is not requested - just free entry */
		ofi_nd_free_cq_entry(entry);
	}
}

void ofi_nd_read_2_send_ack(nd_cq_entry *entry, void *res)
{
	ND2_RESULT *result = res;
	struct nd_ep *ep = (struct nd_ep*)result->QueuePairContext;
	size_t i;

	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);
	assert(entry);

	HRESULT hr;
	struct nd_cq_entry *ack_entry = ofi_nd_buf_alloc_nd_cq_entry();
	if (!ack_entry) {
		ND_LOG_WARN(FI_LOG_EP_DATA, "Unable to allocate buffer for CQ entry");
		return;
	}
	memset(ack_entry, 0, sizeof(*ack_entry));

	ack_entry->data = entry->data;
	ack_entry->flags = entry->flags;
	ack_entry->domain = entry->domain;
	ack_entry->context = entry->context;
	ack_entry->iov_cnt = entry->iov_cnt;
	ack_entry->seq = entry->seq;
	ack_entry->state = NORMAL_STATE;

	ack_entry->prefix = __ofi_nd_buf_alloc_nd_msgprefix(
		&ep->domain->msgfooter);
	if (!ack_entry->prefix) {
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}

	nd_flow_cntrl_flags flow_control_flags = {
		.req_ack = 0,
		.ack = 0,
		.empty = 0
	};

	struct nd_msgheader header_def = {
		.data = entry->data,
		.event = LARGE_MSG_ACK,
		.flags = flow_control_flags,
		.location_cnt = entry->rma_location.count
	};
	ack_entry->prefix->header = header_def;
	ack_entry->event = LARGE_MSG_ACK;
	ack_entry->flow_cntrl_flags = flow_control_flags;

	ack_entry->notify_buf = __ofi_nd_buf_alloc_nd_notifybuf(
		&ep->domain->notifybuf);
	if (!ack_entry->notify_buf) {
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}

	/* Fill the header by values of received header from
	 * originator of large message transmission */
	for (i = 0; i < entry->rma_location.count; i++) {
		struct nd_msg_location *parent_location =
			&entry->rma_location.locations[i];
		struct nd_msg_location location_def = {
			.addr = parent_location->addr,
			.len = parent_location->len,
			.remote_mr_token = parent_location->remote_mr_token,
		};

		ack_entry->notify_buf->location[i] = location_def;
	}

	/* Generate CQ to notify that data successfuly read
	 * and can be obtained by user. Use intial CQ for that */
	ofi_nd_read_2_cq(entry->aux_entry, result);

	/* Set intial CQ entry to NULL just to avoid releasing of
	 * CQ entry for which CQ event haven't generated yet to
	 * requestor of large message RECV operation */
	entry->aux_entry = NULL;
	ofi_nd_free_cq_entry(entry);

	/* Gracefully complete receiving large message -
	 * ACK to peer should be sent */
	ND2_SGE sge[2] = {
		{
			.Buffer = &ack_entry->prefix->header,
			.BufferLength = (ULONG)sizeof(ack_entry->prefix->header),
			.MemoryRegionToken = ack_entry->prefix->token
		},
		{
			.Buffer = &ack_entry->notify_buf->location,
			.BufferLength = (ULONG)(sizeof(*ack_entry->notify_buf->location) * entry->rma_location.count),
			.MemoryRegionToken = ack_entry->notify_buf->token
		}
	};

	nd_sge *sge_entry = ofi_nd_buf_alloc_nd_sge();
	if (!sge_entry) {
		ND_LOG_WARN(FI_LOG_EP_DATA, "SGE entry buffer can't be allocated");
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}
	memset(sge_entry, 0, sizeof(*sge_entry));

	sge_entry->count = 2;
	for (i = 0; i < sge_entry->count; i++)
		sge_entry->entries[i] = sge[i];
	
	nd_send_entry *send_entry = ofi_nd_buf_alloc_nd_send_entry();
	if (!send_entry) {
		ND_LOG_WARN(FI_LOG_EP_DATA, "Send entry buffer can't be allocated");
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}
	memset(send_entry, 0, sizeof(*send_entry));

	send_entry->cq_entry = ack_entry;
	send_entry->sge = sge_entry;
	send_entry->ep = ep;
	
	/* Push the transmission of ACK into
	 * the Send Queue for furhter handling */
	ack_entry->send_entry = send_entry;
	ofi_nd_queue_push(&ep->send_queue, &send_entry->queue_item);

	/* Let's progress Send Queue for current EP if possible */
	ofi_nd_ep_progress(ep);

	return;

fn_fail:
	ofi_nd_free_cq_entry(ack_entry);
	ND_LOG_WARN(FI_LOG_EP_DATA, ofi_nd_strerror((DWORD)hr, NULL));
}

void ofi_nd_send_ack(nd_cq_entry *entry, struct nd_ep *ep)
{
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);
	assert(entry);

	HRESULT hr;
	struct nd_cq_entry *ack_entry = NULL;
	struct nd_queue_item *qentry = NULL;
	nd_send_entry *send_entry = NULL;

	EnterCriticalSection(&ep->send_op.send_lock);
	if (ofi_nd_queue_peek(&ep->send_queue, &qentry)) {
		send_entry = container_of(qentry, nd_send_entry, queue_item);
		struct nd_msgheader *header = (struct nd_msgheader *)
			send_entry->sge->entries[0].Buffer;
		header->flags.ack = 1;
	}
	else {
		ack_entry = ofi_nd_buf_alloc_nd_cq_entry();
		if (!ack_entry) {
			ND_LOG_WARN(FI_LOG_EP_DATA, "Unable to allocate buffer "
				"for CQ entry");
			return;
		}
		memset(ack_entry, 0, sizeof(*ack_entry));

		ack_entry->data = entry->data;
		ack_entry->flags = entry->flags;
		ack_entry->domain = entry->domain;
		ack_entry->context = entry->context;
		ack_entry->iov_cnt = entry->iov_cnt;
		ack_entry->seq = entry->seq;
		ack_entry->state = NORMAL_STATE;

		ack_entry->prefix = __ofi_nd_buf_alloc_nd_msgprefix(
			&ep->domain->msgfooter);
		if (!ack_entry->prefix) {
			hr = ND_NO_MEMORY;
			goto fn_fail;
		}

		nd_flow_cntrl_flags flow_control_flags = {
			.req_ack = 0,
			.ack = 1,
			.empty = 1
		};

		struct nd_msgheader header_def = {
			.data = entry->data,
			.event = NORMAL_EVENT,
			.flags = flow_control_flags,
			.location_cnt = 0
		};
		ack_entry->prefix->header = header_def;
		ack_entry->event = NORMAL_EVENT;
		ack_entry->flow_cntrl_flags = flow_control_flags;

		ND2_SGE sge = {
			.Buffer = &ack_entry->prefix->header,
			.BufferLength = (ULONG)sizeof(ack_entry->prefix->header),
			.MemoryRegionToken = ack_entry->prefix->token
		};

		hr = ep->qp->lpVtbl->Send(ep->qp, ack_entry, &sge, 1, 0);
		if (FAILED(hr))
			ND_LOG_WARN(FI_LOG_CQ, "Send failed from Send Queue\n");
	}
	LeaveCriticalSection(&ep->send_op.send_lock);

	/* Let's progress Send Queue for current EP if possible */
	ofi_nd_ep_progress(ep);

	return;
fn_fail:
	ofi_nd_free_cq_entry(ack_entry);
}

void ofi_nd_repack_iovecs(const struct iovec *from_iovecs, const size_t from_count,
			  const struct iovec *to_iovecs, const size_t to_count,
			  struct iovec new_iovecs[ND_MSG_INTERNAL_IOV_LIMIT],
			  size_t *new_count,
			  size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
			  size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
			  uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT])
{
	size_t from_iter = 0;
	size_t to_iter = 0;
	size_t new_iter = 0;
	size_t to_offset = 0;
	size_t from_offset = 0;

	for(;;) {
		new_iovecs[new_iter].iov_base = (char *)to_iovecs[to_iter].iov_base + to_offset;
		remote_addr[new_iter] =
			(uint64_t)((char *)from_iovecs[from_iter].iov_base + from_offset);
		from_split_map[new_iter] = from_iter;
		to_split_map[new_iter] = to_iter;
		ND_LOG_DEBUG(FI_LOG_EP_DATA, "\nFL = %lu, FO = %lu, FI = %lu "
			     "\nTL = %lu, TO = %lu, TI = %lu\n",
			     from_iovecs[from_iter].iov_len, from_offset, from_iter,
			     to_iovecs[to_iter].iov_len, to_offset, to_iter);

		if (from_iovecs[from_iter].iov_len - from_offset < to_iovecs[to_iter].iov_len - to_offset) {
			new_iovecs[new_iter].iov_len = from_iovecs[from_iter].iov_len - from_offset;
			to_offset += from_iovecs[from_iter].iov_len - from_offset;
			from_iter++;
			from_offset = 0;
		}
		else if (to_iovecs[to_iter].iov_len - to_offset < from_iovecs[from_iter].iov_len - from_offset) {
			new_iovecs[new_iter].iov_len = to_iovecs[to_iter].iov_len - to_offset;
			from_offset += to_iovecs[to_iter].iov_len - to_offset;
			to_iter++;
			to_offset = 0;
		}
		else {
			new_iovecs[new_iter].iov_len = to_iovecs[to_iter].iov_len;
			from_iter++;
			to_iter++;
			to_offset = 0;
			from_offset = 0;
		}

		new_iter++;
		/* Check that whether some iovecs was emptied */
		if ((from_iter == from_count) && (!from_offset) ||
		    ((to_iter == to_count) && (!to_offset)))
			break;
	}
	*new_count = new_iter;
}

#endif /* _WIN32 */

