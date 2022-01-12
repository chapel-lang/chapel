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
#include "netdir_cq.h"
#include "netdir_log.h"
#include "netdir_iface.h"

#include "rdma/fabric.h"
#include "rdma/fi_endpoint.h"

#include "ofi.h"
#include "ofi_util.h"

static ssize_t
ofi_nd_ep_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
	       fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context);
static ssize_t
ofi_nd_ep_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
		void *context);
static ssize_t
ofi_nd_ep_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		  uint64_t flags);
static ssize_t
ofi_nd_ep_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context);
static ssize_t
ofi_nd_ep_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
		 size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		 void *context);
static ssize_t
ofi_nd_ep_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		   uint64_t flags);
static ssize_t
ofi_nd_ep_inject(struct fid_ep *ep, const void *buf, size_t len,
		 fi_addr_t dest_addr, uint64_t addr, uint64_t key);
static ssize_t
ofi_nd_ep_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		    uint64_t data, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		    void *context);
static ssize_t
ofi_nd_ep_writeinjectdata(struct fid_ep *ep, const void *buf, size_t len,
			  uint64_t data, fi_addr_t dest_addr, uint64_t addr,
			  uint64_t key);
ssize_t ofi_nd_ep_injectdata(struct fid_ep *ep, const void *buf, size_t len,
			     uint64_t data, fi_addr_t dest_addr);
static void
ofi_nd_split_msg_iov_2_rma_iov(const struct fi_rma_iov *rma_iovecs, const size_t rma_count,
			       const struct iovec *msg_iovecs, const size_t msg_count,
			       struct fi_rma_iov res_iovecs[ND_MSG_INTERNAL_IOV_LIMIT], size_t *res_count,
			       size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
			       size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
			       uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT]);

struct fi_ops_rma ofi_nd_ep_rma = {
	.size = sizeof(ofi_nd_ep_rma),
	.read = ofi_nd_ep_read,
	.readv = ofi_nd_ep_readv,
	.readmsg = ofi_nd_ep_readmsg,
	.write = ofi_nd_ep_write,
	.writev = ofi_nd_ep_writev,
	.writemsg = ofi_nd_ep_writemsg,
	.inject = ofi_nd_ep_inject,
	.writedata = ofi_nd_ep_writedata,
	.injectdata = ofi_nd_ep_writeinjectdata
};


static ssize_t
ofi_nd_ep_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
	       fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = len
	};
	return ofi_nd_ep_readv(ep, &iov, &desc, 1, src_addr, addr, key, context);
}

static ssize_t
ofi_nd_ep_readv(struct fid_ep *pep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
		void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = iov[0].iov_len,
		.key = key
	};

	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = src_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0
	};

	assert(pep->fid.fclass == FI_CLASS_EP);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	return ofi_nd_ep_readmsg(pep, &msg, ep->info->rx_attr->op_flags);
}

static ssize_t
ofi_nd_ep_readmsg(struct fid_ep *pep, const struct fi_msg_rma *msg,
		  uint64_t flags)
{
	assert(pep->fid.fclass == FI_CLASS_EP);
	assert(msg);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	size_t msg_len = 0, rma_len = 0, i;
	HRESULT hr;

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	if (!ep->qp)
		return -FI_EOPBADSTATE;

	for (i = 0; i < msg->iov_count; i++) {
		if (msg->msg_iov[i].iov_len && !msg->msg_iov[i].iov_base)
			return -FI_EINVAL;
		msg_len += msg->msg_iov[i].iov_len;
	}

	for (i = 0; i < msg->rma_iov_count; i++) {
		if (msg->rma_iov[i].len && !msg->rma_iov[i].addr)
			return -FI_EINVAL;
		rma_len += msg->rma_iov[i].len;
	}

	/* Check the following: */
	if ((msg_len != rma_len) || /* - msg and rma len are correlated */
	    /* - iov counts are less or equal than supported */
	    (msg->iov_count > ND_MSG_IOV_LIMIT ||
	     msg->rma_iov_count > ND_MSG_IOV_LIMIT) ||
	    /* - transmitted length is less or equal than max possible */
	    (msg_len > ep->domain->info->ep_attr->max_msg_size))
		return -FI_EINVAL;

	struct nd_cq_entry *main_entry = ofi_nd_buf_alloc_nd_cq_entry();
	if (!main_entry)
		return -FI_ENOMEM;
	memset(main_entry, 0, sizeof(*main_entry));
	main_entry->data = msg->data;
	main_entry->flags = flags;
	main_entry->domain = ep->domain;
	main_entry->context = msg->context;
	main_entry->seq = InterlockedAdd64(&ep->domain->msg_cnt, 1);

	/* since write operation can't be canceled, set NULL into
	 * the 1st byte of internal data of context */
	if (msg->context)
		ND_FI_CONTEXT(msg->context) = 0;

	struct fi_rma_iov rma_iovecs[ND_MSG_INTERNAL_IOV_LIMIT];
	size_t rma_count = 0;
	size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT];
	size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT];
	uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT];

	ofi_nd_split_msg_iov_2_rma_iov(msg->rma_iov, msg->rma_iov_count,
				       msg->msg_iov, msg->iov_count,
				       rma_iovecs, &rma_count,
				       from_split_map, to_split_map, remote_addr);

	assert(rma_count <= ND_MSG_INTERNAL_IOV_LIMIT);

	main_entry->wait_completion.comp_count = 0;
	main_entry->wait_completion.total_count = rma_count;

	InitializeCriticalSection(&main_entry->wait_completion.comp_lock);

	struct nd_cq_entry *entries[ND_MSG_IOV_LIMIT];

	for (i = 0; i < rma_count; i++) {
		entries[i] = ofi_nd_buf_alloc_nd_cq_entry();
		if (!entries[i])
			goto fn_fail;
		memset(entries[i], 0, sizeof(*entries[i]));

		entries[i]->data = msg->data;
		entries[i]->flags = flags;
		entries[i]->domain = ep->domain;
		entries[i]->context = msg->context;
		entries[i]->seq = main_entry->seq;
		entries[i]->aux_entry = main_entry;

		hr = ep->domain->adapter->lpVtbl->CreateMemoryRegion(
			ep->domain->adapter, &IID_IND2MemoryRegion,
			ep->domain->adapter_file, (void**)&entries[i]->mr[0]);
		if (FAILED(hr))
			goto fn_fail;
		entries[i]->mr_count = 1;

		hr = ofi_nd_util_register_mr(
			entries[i]->mr[0],
			(const void *)remote_addr[i],
			rma_iovecs[i].len,
			ND_MR_FLAG_ALLOW_LOCAL_WRITE |
			ND_MR_FLAG_ALLOW_REMOTE_READ |
			ND_MR_FLAG_ALLOW_REMOTE_WRITE);
		if (FAILED(hr))
			goto fn_fail;

		ND2_SGE sge = {
			.Buffer = (void *)remote_addr[i],
			.BufferLength = (ULONG)rma_iovecs[i].len,
			.MemoryRegionToken = (UINT32)(uintptr_t)msg->desc[to_split_map[i]]
		};

		hr = ep->qp->lpVtbl->Read(ep->qp, entries[i], &sge, 1,
			(UINT64)rma_iovecs[i].addr, (UINT32)rma_iovecs[i].key, 0);
		if (FAILED(hr))
			goto fn_fail;
	}

	return FI_SUCCESS;

fn_fail:
	while (i-- > 0)
		ofi_nd_free_cq_entry(entries[i]);
	ND_LOG_WARN(FI_LOG_EP_DATA, ofi_nd_strerror((DWORD)hr, NULL));
	return H2F(hr);
}

static ssize_t
ofi_nd_ep_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len
	};
	return ofi_nd_ep_writev(ep, &iov, &desc, 1, dest_addr, addr, key, context);
}

static ssize_t
ofi_nd_ep_writev(struct fid_ep *pep, const struct iovec *iov, void **desc,
		 size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		 void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = iov[0].iov_len,
		.key = key
	};

	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0
	};

	assert(pep->fid.fclass == FI_CLASS_EP);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	return ofi_nd_ep_writemsg(pep, &msg, ep->info->tx_attr->op_flags);
}

static ssize_t
ofi_nd_ep_writemsg(struct fid_ep *pep, const struct fi_msg_rma *msg,
		   uint64_t flags)
{
	assert(pep->fid.fclass == FI_CLASS_EP);
	assert(msg);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	size_t msg_len = 0, rma_len = 0, i;
	HRESULT hr;

	struct nd_cq_entry *entries[ND_MSG_IOV_LIMIT];
	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	if (!ep->qp)
		return -FI_EOPBADSTATE;

	for (i = 0; i < msg->iov_count; i++) {
		if (msg->msg_iov[i].iov_len && !msg->msg_iov[i].iov_base)
			return -FI_EINVAL;
		msg_len += msg->msg_iov[i].iov_len;
	}

	if ((msg_len > ep->domain->info->ep_attr->max_msg_size) &&
	    (flags & FI_INJECT))
		return -FI_EINVAL;

	for (i = 0; i < msg->rma_iov_count; i++) {
		if (msg->rma_iov[i].len && !msg->rma_iov[i].addr) 
			return -FI_EINVAL;
		rma_len += msg->rma_iov[i].len;
	}

	/* Check the following: */
	if ((msg_len != rma_len) || /* - msg and rma len are correlated */
	    /* - iov counts are less or equal than supported */
	    ((msg->iov_count > ND_MSG_IOV_LIMIT ||
	      msg->rma_iov_count > ND_MSG_IOV_LIMIT)) ||
	    /* - transmitted length is less or equal than max possible */
	    (msg_len > ep->domain->info->ep_attr->max_msg_size) ||
	    /* - if INJECT, data should be inlined */
	    ((flags & FI_INJECT) &&
	     (msg_len > ep->domain->info->tx_attr->inject_size)))
		return -FI_EINVAL;

	struct nd_cq_entry *main_entry = ofi_nd_buf_alloc_nd_cq_entry();
	if (!main_entry)
		return -FI_ENOMEM;
	memset(main_entry, 0, sizeof(*main_entry));
	main_entry->data = msg->data;
	main_entry->flags = flags;
	main_entry->domain = ep->domain;
	main_entry->context = msg->context;
	main_entry->seq = InterlockedAdd64(&ep->domain->msg_cnt, 1);

	/* since write operation can't be canceled, set NULL into
	* the 1st byte of internal data of context */
	if (msg->context)
		ND_FI_CONTEXT(msg->context) = 0;

	/* TODO */
	if (msg_len > (size_t)gl_data.inline_thr) {
		struct fi_rma_iov rma_iovecs[ND_MSG_INTERNAL_IOV_LIMIT];
		size_t rma_count = 0;
		size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT];
		size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT];
		uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT];

		ofi_nd_split_msg_iov_2_rma_iov(msg->rma_iov, msg->rma_iov_count,
					       msg->msg_iov, msg->iov_count,
					       rma_iovecs, &rma_count,
					       from_split_map, to_split_map, remote_addr);

		assert(rma_count <= ND_MSG_INTERNAL_IOV_LIMIT);

		main_entry->wait_completion.comp_count = 0;
		main_entry->wait_completion.total_count = rma_count;

		InitializeCriticalSection(&main_entry->wait_completion.comp_lock);

		for (i = 0; i < rma_count; i++) {
			entries[i] = ofi_nd_buf_alloc_nd_cq_entry();
			if (!entries[i])
				goto fn_fail;
			memset(entries[i], 0, sizeof(*entries[i]));

			entries[i]->data = msg->data;
			entries[i]->flags = flags;
			entries[i]->domain = ep->domain;
			entries[i]->context = msg->context;
			entries[i]->seq = main_entry->seq;
			entries[i]->aux_entry = main_entry;

			ND2_SGE sge = {
				.Buffer = (void *)remote_addr[i],
				.BufferLength = (ULONG)rma_iovecs[i].len,
				.MemoryRegionToken = (UINT32)(uintptr_t)msg->desc[to_split_map[i]]
			};

			hr = ep->qp->lpVtbl->Write(ep->qp, entries[i], &sge, 1,
				(UINT64)rma_iovecs[i].addr, (UINT32)rma_iovecs[i].key, 0);
			if (FAILED(hr))
				goto fn_fail;
		}

		return FI_SUCCESS;
	}
	else {
		if (msg_len) {
			main_entry->inline_buf = __ofi_nd_buf_alloc_nd_inlinebuf(&ep->domain->inlinebuf);
			if (!main_entry->inline_buf)
				return -FI_ENOMEM;

			char *buf = (char*)main_entry->inline_buf->buffer;
			for (i = 0; i < msg->iov_count; i++) {
				memcpy(buf, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len);
				buf += msg->msg_iov[i].iov_len;
			}
		}

		for (i = 0; i < msg->rma_iov_count; i++) {
			char *buf = (char *)main_entry->inline_buf->buffer;

			entries[i] = ofi_nd_buf_alloc_nd_cq_entry();
			if (!entries[i])
				goto fn_fail;
			memset(entries[i], 0, sizeof(*entries[i]));

			entries[i]->data = msg->data;
			entries[i]->flags = flags;
			entries[i]->domain = ep->domain;
			entries[i]->context = msg->context;
			entries[i]->seq = main_entry->seq;
			entries[i]->aux_entry = main_entry;

			ND2_SGE sge = {
				.Buffer = (void *)(buf + msg->rma_iov[i].len),
				.BufferLength = (ULONG)msg->rma_iov[i].len,
				.MemoryRegionToken = main_entry->inline_buf->token
			};

			hr = ep->qp->lpVtbl->Write(ep->qp, entries[i], &sge, 1,
				(UINT64)msg->rma_iov[i].addr, (UINT32)msg->rma_iov[i].key, 0);
			if (FAILED(hr))
				goto fn_fail;
		}

		return FI_SUCCESS;
	}
fn_fail:
	while (i-- > 0)
		ofi_nd_free_cq_entry(entries[i]);
	ND_LOG_WARN(FI_LOG_EP_DATA, ofi_nd_strerror((DWORD)hr, NULL));
	return H2F(hr);
}

static ssize_t
ofi_nd_ep_inject(struct fid_ep *pep, const void *buf, size_t len,
		 fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len
	};

	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key
	};

	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = 0,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = 0,
		.data = 0
	};

	return ofi_nd_ep_writemsg(pep, &msg, FI_INJECT);
}

static ssize_t
ofi_nd_ep_writedata(struct fid_ep *pep, const void *buf, size_t len, void *desc,
		    uint64_t data, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		    void *context)
{
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len
	};

	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key
	};

	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = &desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = data
	};

	assert(pep->fid.fclass == FI_CLASS_EP);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	return ofi_nd_ep_writemsg(pep, &msg, ep->info->tx_attr->op_flags | FI_REMOTE_CQ_DATA);
}

static ssize_t
ofi_nd_ep_writeinjectdata(struct fid_ep *ep, const void *buf, size_t len,
			  uint64_t data, fi_addr_t dest_addr, uint64_t addr,
			  uint64_t key)
{
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len
	};

	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key
	};

	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = 0,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = 0,
		.data = data
	};

	return ofi_nd_ep_writemsg(ep, &msg, FI_INJECT | FI_REMOTE_CQ_DATA);
}

void ofi_nd_read_event(ND2_RESULT *result)
{
	assert(result);
	assert(result->RequestType == Nd2RequestTypeRead);

	nd_cq_entry *entry = (nd_cq_entry*)result->RequestContext;
	assert(entry);

	ND_LOG_EVENT_INFO(entry);

	/* Check whether the operation is complex, i.e. read operation
	 * may consists from several subtasks of read */
	if (entry->aux_entry) {
		EnterCriticalSection(&entry->aux_entry->wait_completion.comp_lock);
		entry->aux_entry->wait_completion.comp_count++;
		ND_LOG_DEBUG(FI_LOG_EP_DATA, "READ Event comp_count = %d, total_count = %d\n",
			entry->aux_entry->wait_completion.comp_count,
			entry->aux_entry->wait_completion.total_count);
		if (entry->aux_entry->wait_completion.comp_count < entry->aux_entry->wait_completion.total_count) {
			/* Should wait some remaining completion events about read operation */
			LeaveCriticalSection(&entry->aux_entry->wait_completion.comp_lock);
			entry->aux_entry = NULL;
			ofi_nd_free_cq_entry(entry);
			return;
		}
		LeaveCriticalSection(&entry->aux_entry->wait_completion.comp_lock);
	}

	/*TODO: Handle erroneous case "result->Status != S_OK" */
	ofi_nd_dispatch_cq_event(entry->state == LARGE_MSG_RECV_REQ ?
		LARGE_MSG_REQ : NORMAL_EVENT, entry, result);
}

void ofi_nd_write_event(ND2_RESULT *result)
{
	assert(result);
	assert(result->RequestType == Nd2RequestTypeWrite);

	nd_cq_entry *entry = (nd_cq_entry*)result->RequestContext;
	assert(entry);

	struct nd_ep *ep = (struct nd_ep*)result->QueuePairContext;
	assert(ep);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	ND_LOG_EVENT_INFO(entry);

	/* Check whether the operation is complex, i.e. write operation
	 * may consist from several subtasks of write */
	if (entry->aux_entry) {
		EnterCriticalSection(&entry->aux_entry->wait_completion.comp_lock);
		entry->aux_entry->wait_completion.comp_count++;

		if (entry->aux_entry->wait_completion.comp_count < entry->aux_entry->wait_completion.total_count) {
			/* Should wait some remaining completion events about write operation */
			LeaveCriticalSection(&entry->aux_entry->wait_completion.comp_lock);
			entry->aux_entry = NULL;
			ofi_nd_free_cq_entry(entry);
			return;
		}
		LeaveCriticalSection(&entry->aux_entry->wait_completion.comp_lock);
	}

	if (!entry->context) {
		/* This means that this write was an internal event,
		 * just release it */
		ofi_nd_free_cq_entry(entry);
		return;
	}

	if (entry->flags & FI_REMOTE_CQ_DATA) {
		if (ofi_nd_ep_injectdata(
			&ep->fid, 0, 0, entry->data,
			FI_ADDR_UNSPEC) != FI_SUCCESS)
			ND_LOG_WARN(FI_LOG_CQ, "failed to write-inject");
	}

	if (ep->cntr_write) {
		if (result->Status != S_OK) {
			InterlockedIncrement64(&ep->cntr_write->err);
		}
		InterlockedIncrement64(&ep->cntr_write->counter);
		WakeByAddressAll((void*)&ep->cntr_write->counter);
	}

	int notify = ofi_nd_util_completion_blackmagic(
		ep->info->tx_attr->op_flags, ep->send_flags, entry->flags) ||
		result->Status != S_OK;

	if (notify) {
		PostQueuedCompletionStatus(
			entry->result.Status == S_OK ? ep->cq_send->iocp : ep->cq_send->err,
			0, 0, &entry->base.ov);
		InterlockedIncrement(&ep->cq_send->count);
		WakeByAddressAll((void*)&ep->cq_send->count);
	}
	else { /* if notification is not requested - just free entry */
		ofi_nd_free_cq_entry(entry);
	}
}

void ofi_nd_split_msg_iov_2_rma_iov(const struct fi_rma_iov *rma_iovecs, const size_t rma_count,
				    const struct iovec *msg_iovecs, const size_t msg_count,
				    struct fi_rma_iov res_iovecs[ND_MSG_INTERNAL_IOV_LIMIT],
				    size_t *res_count,
				    size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
				    size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
				    uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT])
{
	size_t i;

	struct iovec from_rma_iovecs[ND_MSG_IOV_LIMIT];
	size_t from_rma_count = rma_count;

	struct iovec res_msg_iovecs[ND_MSG_IOV_LIMIT];
	size_t res_msg_count = 0;


	/* Convert RMA iovecs to MSG iovecs to be able to reuse
	 * them in @ofi_nd_repack_iovecs */
	for (i = 0; i < rma_count; i++) {
		from_rma_iovecs[i].iov_base = (void *)rma_iovecs[i].addr;
		from_rma_iovecs[i].iov_len = rma_iovecs[i].len;
	}

	ofi_nd_repack_iovecs(from_rma_iovecs, from_rma_count,
			     msg_iovecs, msg_count,
			     res_msg_iovecs, &res_msg_count,
			     from_split_map, to_split_map, remote_addr);

	/* Extract MSG iov to RMA iovecs and returns them */
	for (i = 0; i < res_msg_count; i++) {
		res_iovecs[i].addr = remote_addr[i];
		res_iovecs[i].len = res_msg_iovecs[i].iov_len;
		res_iovecs[i].key = rma_iovecs[from_split_map[i]].key;

		remote_addr[i] = (uint64_t)res_msg_iovecs[i].iov_base;
	}

	*res_count = res_msg_count;
}

#endif /* _WIN32 */

