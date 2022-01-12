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
#include "netdir_unexp.h"

#include "rdma/fabric.h"
#include "rdma/fi_endpoint.h"

#include "ofi.h"
#include "ofi_util.h"

static ssize_t ofi_nd_srx_recv(struct fid_ep *ep, void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, void *context);
static ssize_t ofi_nd_srx_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
				 uint64_t flags);
static ssize_t ofi_nd_srx_recvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
			       size_t count, fi_addr_t src_addr, void *context);
static ssize_t ofi_nd_no_send(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, void *context);
static ssize_t ofi_nd_no_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
				 uint64_t flags);
static ssize_t ofi_nd_no_sendv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
			       size_t count, fi_addr_t dest_addr, void *context);
static ssize_t ofi_nd_no_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
				fi_addr_t dest_addr);
static ssize_t ofi_nd_no_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
				  uint64_t data, fi_addr_t dest_addr, void *context);
static ssize_t ofi_nd_no_injectdata(struct fid_ep *ep, const void *buf, size_t len,
				    uint64_t data, fi_addr_t dest_addr);
static int ofi_nd_srx_close(struct fid *fid);
static ssize_t ofi_nd_srx_cancel(fid_t fid, void *context);

struct fi_ops_msg ofi_nd_srx_msg = {
	.size = sizeof(ofi_nd_srx_msg),
	.recv = ofi_nd_srx_recv,
	.recvv = ofi_nd_srx_recvv,
	.recvmsg = ofi_nd_srx_recvmsg,
	.send = ofi_nd_no_send,
	.sendv = ofi_nd_no_sendv,
	.sendmsg = ofi_nd_no_sendmsg,
	.inject = ofi_nd_no_inject,
	.senddata = ofi_nd_no_senddata,
	.injectdata = ofi_nd_no_injectdata
};

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_srx_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_SRX_CTX,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

static struct fi_ops_ep ofi_nd_ep_ops = {
	.size = sizeof(ofi_nd_ep_ops),
	.cancel = ofi_nd_srx_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left
};

int ofi_nd_srx_ctx(struct fid_domain *pdomain,
		   struct fi_rx_attr *attr, struct fid_ep **rx_ep,
		   void *context)
{
	OFI_UNUSED(attr);
	struct nd_domain *domain = container_of(pdomain, struct nd_domain, fid);
	struct nd_srx *srx = (struct nd_srx*) calloc(1, sizeof(*srx));
	if (!srx)
		return -FI_ENOMEM;

	struct nd_srx def = {
		.fid = {
			.fid = {
				.fclass = FI_CLASS_SRX_CTX,
				.context = context,
				.ops = &ofi_nd_fi_ops
			},
			.ops = &ofi_nd_ep_ops,
			.msg = &ofi_nd_srx_msg
		},
		.domain = domain,
		.attr = {
			.caps = FI_MSG | FI_RECV,
			.mode = 0,
			.op_flags = 0,
			.comp_order = FI_ORDER_STRICT,
			.total_buffered_recv = 0,
			.size = (size_t)gl_data.inline_thr,
			.iov_limit = (size_t)min(domain->ainfo.MaxReceiveSge, ND_MSG_IOV_LIMIT) - 1
		}
	};

	*srx = def;
	/* TODO */
	dlist_init(&srx->received);

	*rx_ep = &srx->fid;

	InitializeCriticalSection(&srx->prepost_lock);

	return FI_SUCCESS;
}

static ssize_t ofi_nd_srx_recvmsg(struct fid_ep *pep, const struct fi_msg *msg,
				  uint64_t flags)
{
	assert(pep->fid.fclass == FI_CLASS_SRX_CTX);
	assert(msg);

	if (pep->fid.fclass != FI_CLASS_SRX_CTX)
		return -FI_EINVAL;

	size_t i;
	size_t len = 0;

	struct nd_srx *srx = container_of(pep, struct nd_srx, fid);

	if (msg->iov_count > min(srx->domain->ainfo.MaxReceiveSge, ND_MSG_IOV_LIMIT) - 1)
		return -FI_EINVAL;

	for (i = 0; i < msg->iov_count; i++) {
		if (msg->msg_iov[i].iov_len && !msg->msg_iov[i].iov_base)
			return -FI_EINVAL;
		len += msg->msg_iov[i].iov_len;
	}

	struct nd_cq_entry *entry = ofi_nd_buf_alloc_nd_cq_entry();
	if (!entry)
		return -FI_ENOMEM;
	memset(entry, 0, sizeof(*entry));

	entry->buf = (msg->iov_count == 1) ? msg->msg_iov[0].iov_base : 0;
	entry->len = len;
	entry->data = msg->data;
	entry->flags = flags | FI_MSG | FI_RECV;
	entry->domain = srx->domain;
	entry->context = msg->context;
	entry->iov_cnt = msg->iov_count;
	entry->seq = InterlockedAdd64(&srx->domain->msg_cnt, 1);

	for (i = 0; i < msg->iov_count; i++) {
		entry->iov[i] = msg->msg_iov[i];
	}

	/* store allocated entry in 1st byte of internal data of context */
	if (msg->context)
		ND_FI_CONTEXT(msg->context) = entry;

	ofi_nd_queue_push(&srx->prepost, &entry->queue_item);

	ofi_nd_srx_match(srx);

	return FI_SUCCESS;
}

static ssize_t ofi_nd_srx_recvv(struct fid_ep *pep, const struct iovec *iov,
			       void **desc,
			       size_t count, fi_addr_t src_addr, void *context)
{
	struct fi_msg msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = src_addr,
		.context = context,
		.data = 0
	};

	assert(pep->fid.fclass == FI_CLASS_SRX_CTX);

	if (pep->fid.fclass != FI_CLASS_SRX_CTX)
		return -FI_EINVAL;

	struct nd_srx *rx_ctx  = container_of(pep, struct nd_srx, fid);

	return ofi_nd_srx_recvmsg(pep, &msg, rx_ctx->attr.op_flags);
}

static ssize_t ofi_nd_srx_recv(struct fid_ep *pep, void *buf, size_t len,
			       void *desc, fi_addr_t src_addr, void *context)
{
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = len
	};

	return ofi_nd_srx_recvv(pep, &iov, &desc, 1, src_addr, context);
}

static int ofi_nd_srx_close(struct fid *fid)
{
	assert(fid);
	assert(fid->fclass == FI_CLASS_SRX_CTX);

	if (fid->fclass != FI_CLASS_SRX_CTX)
		return -FI_EINVAL;

	struct nd_srx *srx = container_of(fid, struct nd_srx, fid.fid);

	DeleteCriticalSection(&srx->prepost_lock);
	if (srx->srx)
		srx->srx->lpVtbl->Release(srx->srx);
	free(srx);

	return FI_SUCCESS;
}

static ssize_t ofi_nd_no_send(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, void *context)
{
	OFI_UNUSED(ep);
	OFI_UNUSED(buf);
	OFI_UNUSED(len);
	OFI_UNUSED(desc);
	OFI_UNUSED(src_addr);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}

static ssize_t ofi_nd_no_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
				 uint64_t flags)
{
	OFI_UNUSED(ep_fid);
	OFI_UNUSED(msg);
	OFI_UNUSED(flags);
	return -FI_ENOSYS;
}

static ssize_t ofi_nd_no_sendv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
			       size_t count, fi_addr_t dest_addr, void *context)
{
	OFI_UNUSED(ep_fid);
	OFI_UNUSED(iov);
	OFI_UNUSED(desc);
	OFI_UNUSED(count);
	OFI_UNUSED(dest_addr);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}
static ssize_t ofi_nd_no_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
				fi_addr_t dest_addr)
{
	OFI_UNUSED(ep_fid);
	OFI_UNUSED(buf);
	OFI_UNUSED(len);
	OFI_UNUSED(dest_addr);
	return -FI_ENOSYS;
}

static ssize_t ofi_nd_no_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
				  uint64_t data, fi_addr_t dest_addr, void *context)
{
	OFI_UNUSED(ep);
	OFI_UNUSED(buf);
	OFI_UNUSED(len);
	OFI_UNUSED(desc);
	OFI_UNUSED(data);
	OFI_UNUSED(dest_addr);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}

static ssize_t ofi_nd_no_injectdata(struct fid_ep *ep, const void *buf, size_t len,
			     uint64_t data, fi_addr_t dest_addr)
{
	OFI_UNUSED(ep);
	OFI_UNUSED(buf);
	OFI_UNUSED(len);
	OFI_UNUSED(data);
	OFI_UNUSED(dest_addr);
	return -FI_ENOSYS;
}

static ssize_t ofi_nd_srx_cancel(fid_t fid, void *context)
{
	assert(fid);
	assert(fid->fclass == FI_CLASS_SRX_CTX);
	assert(context);

	if (!context) {
		ND_LOG_WARN(FI_LOG_EP_DATA, "Context is NULL \n");
		return -FI_EINVAL;
	}

	return ofi_nd_cq_cancel(fid, context);
}

#endif /* _WIN32 */

