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

#include "rdma/fabric.h"
#include "rdma/fi_endpoint.h"

#include "ofi.h"
#include "ofi_util.h"

static int ofi_nd_ep_control(struct fid *fid, int command, void *arg);
static int ofi_nd_ep_close(struct fid *fid);
static int ofi_nd_ep_bind(fid_t ep, fid_t cq, uint64_t flags);
static int ofi_nd_ep_getname(fid_t fid, void *addr, size_t *addrlen);
static int ofi_nd_ep_getpeer(struct fid_ep *fid, void *addr, size_t *addrlen);
static int ofi_nd_ep_connect(struct fid_ep *ep, const void *addr,
			     const void *param, size_t paramlen);
static int ofi_nd_ep_accept(struct fid_ep *ep, const void *param,
			    size_t paramlen);
static int ofi_nd_ep_shutdown(struct fid_ep *ep, uint64_t flags);

static ND_BUF_CHUNK(nd_ep_msgprefix)
	*ofi_nd_ep_alloc_chunk(ND_BUF_FOOTER(nd_ep_msgprefix) *footer,
			       size_t *count);
static void ofi_nd_ep_free_chunk(ND_BUF_CHUNK(nd_ep_msgprefix) *chunk);

static void ofi_nd_ep_disconnected_free(struct nd_event_base* base);
static void ofi_nd_ep_disconnected(struct nd_event_base* base, DWORD bytes);
static void ofi_nd_ep_disconnected_err(struct nd_event_base* base, DWORD bytes, DWORD err);
static ssize_t ofi_nd_ep_cancel(fid_t fid, void *context);


static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_ep_close,
	.bind = ofi_nd_ep_bind,
	.control = ofi_nd_ep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cm ofi_nd_cm_ops = {
	.size = sizeof(ofi_nd_cm_ops),
	.setname = fi_no_setname,
	.getname = ofi_nd_ep_getname,
	.getpeer = ofi_nd_ep_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = ofi_nd_ep_shutdown,
	.join = fi_no_join,
};

extern struct fi_ops_msg ofi_nd_ep_msg;
extern struct fi_ops_rma ofi_nd_ep_rma;

static struct fi_ops_ep ofi_nd_ep_ops = {
	.size = sizeof(ofi_nd_ep_ops),
	.cancel = ofi_nd_ep_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

typedef struct nd_ep_connect_data {
	struct {
		struct nd_msg_location send_res;
	} flow_control;

	struct {
		void	*param;
		size_t	paramlen;
	} user_data;

	struct {
		void	*data;
		size_t	size;
	} total_conn_data;
} nd_ep_connect_data;

typedef struct nd_ep_connect {
	nd_event_base		base;
	struct nd_ep		*ep;
	struct nd_eq		*eq;
	IND2Connector		*connector;
	int			active;
} nd_ep_connect;

typedef struct nd_ep_completed {
	nd_event_base	base;
	struct nd_ep		*ep;
	struct nd_eq		*eq;
	IND2Connector		*connector;
} nd_ep_completed;

OFI_ND_NB_BUF(nd_ep_connect);
OFI_ND_NB_BUF(nd_ep_completed);

OFI_ND_NB_BUF_IMP(nd_ep_connect);
OFI_ND_NB_BUF_IMP(nd_ep_completed);

OFI_ND_NB_BUF_TYPED(nd_connreq, struct nd_connreq);

int ofi_nd_endpoint(struct fid_domain *pdomain, struct fi_info *info,
	struct fid_ep **ep_fid, void *context)
{
	assert(info);
	assert(pdomain);
	assert(pdomain->fid.fclass == FI_CLASS_DOMAIN);

	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_ep_connect));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_ep_completed));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(ofi_nd_util_ov));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_send_entry));
	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_sge));

	HRESULT hr;

	struct nd_domain *domain = container_of(pdomain, struct nd_domain, fid);
	struct nd_connreq *connreq = 0;
	struct nd_ep *ep = (struct nd_ep*) calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	struct nd_ep def = {
		.fid = {
			.fid = {
				.fclass = FI_CLASS_EP,
				.context = context,
				.ops = &ofi_nd_fi_ops
			},
			.ops = &ofi_nd_ep_ops,
			.cm = &ofi_nd_cm_ops,
			.msg = &ofi_nd_ep_msg,
			.rma = &ofi_nd_ep_rma
		},
		.info = fi_dupinfo(info),
		.domain = domain,
		.eq = domain->eq,
		.disconnect_ov = {
			.free = ofi_nd_ep_disconnected_free,
			.event_cb = ofi_nd_ep_disconnected,
			.err_cb = ofi_nd_ep_disconnected_err
		}
	};

	*ep = def;

	/* Initialzie flow control counter */
	ep->send_op.used_counter = 0;
	InitializeCriticalSection(&ep->send_op.send_lock);

	if (info->handle) {
		assert(info->handle->fclass == FI_CLASS_CONNREQ);
		if (info->handle->fclass != FI_CLASS_CONNREQ)
			return -FI_EINVAL;
		connreq = container_of(info->handle, struct nd_connreq,
				       handle);
	}

	InitializeCriticalSection(&ep->prepost_lock);

	assert(domain->adapter);

	if (connreq) {
		assert(connreq->connector);
		ep->connector = connreq->connector;
		ND_BUF_FREE(nd_connreq, connreq);
		ep->fid.cm->accept = ofi_nd_ep_accept;
	}
	else {
		hr = domain->adapter->lpVtbl->CreateConnector(domain->adapter,
							&IID_IND2Connector,
							domain->adapter_file,
							(void**)&ep->connector);
		if (FAILED(hr))
			goto fn_fail;

		hr = ep->connector->lpVtbl->Bind(ep->connector,
						 &domain->addr.addr,
						 (ULONG)ofi_sizeofaddr(&domain->addr.addr));
		if (FAILED(hr))
			goto fn_fail;

		ep->fid.cm->connect = ofi_nd_ep_connect;
	}

	dlist_insert_tail(&ep->entry, &domain->ep_list);

	/* do NOT create real ND endpoint here: we could not know
	how CQ will be attached here */

	*ep_fid = &ep->fid;
	hr = ofi_nd_unexp_init(ep);

	return 0;

fn_fail:
	ofi_nd_ep_close(&domain->fid.fid);
	ND_LOG_WARN(FI_LOG_EP_CTRL, ofi_nd_strerror((DWORD)hr, NULL));
	return H2F(hr);
}

static int ofi_nd_ep_control(struct fid *fid, int command, void *arg)
{
	OFI_UNUSED(arg);

	assert(fid->fclass == FI_CLASS_EP);

	HRESULT hr;

	if (command != FI_ENABLE)
		return -FI_EINVAL;

	struct nd_ep *ep = container_of(fid, struct nd_ep, fid.fid);

	if (ep->qp)
		return FI_SUCCESS; /* already enabled */

	hr = ep->domain->adapter->lpVtbl->CreateQueuePair(
		ep->domain->adapter, &IID_IND2QueuePair,
		(IUnknown*)ep->domain->cq,
		(IUnknown*)ep->domain->cq,
		ep,
		ep->domain->ainfo.MaxReceiveQueueDepth,
		ep->domain->ainfo.MaxInitiatorQueueDepth,
		ep->domain->ainfo.MaxReceiveSge,
		ep->domain->ainfo.MaxInitiatorSge,
		0, (void**)&ep->qp);
	if (FAILED(hr))
		return H2F(hr);

	/* Initialzie unexpected functionality */
	InitializeCriticalSection(&ep->unexpected.unexp_lock);
	ofi_nd_unexp_run(ep);

	return FI_SUCCESS;
}

static int ofi_nd_ep_close(struct fid *fid)
{
	ND_LOG_DEBUG(FI_LOG_EP_CTRL, "closing ep\n");

	assert(fid->fclass == FI_CLASS_EP);

	struct nd_ep *ep = container_of(fid, struct nd_ep, fid.fid);

	ofi_nd_ep_shutdown(&ep->fid, 0);

	int res;
	if (ep->connector) {
		res = (int)ep->connector->lpVtbl->Release(ep->connector);
		ND_LOG_DEBUG(FI_LOG_EP_CTRL, "ep->connector ref count: %d\n", res);
	}
	if (ep->qp) {
		res = (int)ep->qp->lpVtbl->Release(ep->qp);
		ND_LOG_DEBUG(FI_LOG_EP_CTRL, "ep->qp ref count: %d\n", res);
	}
	if (ep->info)
		fi_freeinfo(ep->info);

	DeleteCriticalSection(&ep->prepost_lock);
	/* Release Critical Section for unexpected events */
	DeleteCriticalSection(&ep->unexpected.unexp_lock);

	/* Retrieve this endpoint from domain EP list */
	dlist_remove(&ep->entry);
	DeleteCriticalSection(&ep->send_op.send_lock);
	free(ep);
	ep = NULL;

	return 0;
}

static void ofi_nd_ep_completed_free(nd_event_base *base)
{
	assert(base);

	nd_ep_completed *compl = container_of(base, nd_ep_completed, base);
	assert(compl->connector);
	compl->connector->lpVtbl->Release(compl->connector);
	ND_BUF_FREE(nd_ep_completed, compl);
}

static void ofi_nd_ep_completed(nd_event_base *base, DWORD bytes)
{
	OFI_UNUSED(bytes);
	assert(base);
	assert(base->free);

	nd_ep_completed *compl = container_of(base, nd_ep_completed, base);
	assert(compl->connector);

	base->free(base);
}

static void ofi_nd_ep_completed_err(nd_event_base *base, DWORD bytes,
				    DWORD error)
{
	OFI_UNUSED(bytes);
	assert(base);
	assert(base->free);

	nd_ep_completed *compl = container_of(base, nd_ep_completed, base);

	struct nd_eq_event *err = ND_BUF_ALLOC(nd_eq_event);
	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "failed to allocate error event\n");
		goto fn_completed;
	}

	memset(err, 0, sizeof(*err));
	err->error.err = -H2F(error);
	err->error.prov_errno = (int)error;
	err->error.fid = &compl->ep->fid.fid;
	ofi_nd_eq_push_err(compl->eq, err);

fn_completed:
	base->free(base);
}

static void ofi_nd_ep_accepted_free(nd_event_base *base)
{
	assert(base);

	nd_ep_connect *connect = container_of(base, nd_ep_connect, base);
	if (connect->connector)
		connect->connector->lpVtbl->Release(connect->connector);
	ND_BUF_FREE(nd_ep_connect, connect);
}

static void ofi_nd_ep_accepted(nd_event_base *base, DWORD bytes)
{
	assert(base);
	OFI_UNUSED(bytes);

	HRESULT hr;
	ULONG len = 0;
	nd_ep_connect *connect = container_of(base, nd_ep_connect, base);
	struct nd_eq_event *err;
	nd_ep_completed *compl = NULL;

	assert(connect->connector);
	assert(connect->ep);
	assert(connect->eq);

	struct nd_eq_event *ev = ND_BUF_ALLOC(nd_eq_event);
	if (!ev) {
		hr = ND_NO_MEMORY;
		goto fn_fail_ev;
	}
	memset(ev, 0, sizeof(*ev));
	ev->eq_event = FI_CONNECTED;

	hr = connect->connector->lpVtbl->GetPrivateData(
		connect->connector, NULL, &len);

	if (connect->active) {
		hr = connect->connector->lpVtbl->GetPrivateData(
			connect->connector, NULL, &len);

		if (FAILED(hr) && hr != ND_BUFFER_OVERFLOW) {
			ND_LOG_WARN(FI_LOG_EP_CTRL,
				   "failed to get connection data\n");
			goto fn_fail_data;
		}

		if (len) {
			ev->data = malloc(len);
			if (!ev->data) {
				ND_LOG_WARN(FI_LOG_EP_CTRL,
					   "failed to allocate connection data\n");
				hr = ND_NO_MEMORY;
				ev->len = 0;
				goto fn_fail_data;
			}

			hr = connect->connector->lpVtbl->GetPrivateData(
				connect->connector, ev->data, &len);
			if (FAILED(hr)) {
				ND_LOG_WARN(FI_LOG_EP_CTRL,
					   "failed to copy connection data\n");
				free(ev->data);
				ev->len = 0;
				goto fn_fail_data;
			}
		}
		ev->len = (size_t)len;

		compl = ND_BUF_ALLOC(nd_ep_completed);
		if (!compl) {
			ND_LOG_WARN(FI_LOG_EP_CTRL,
				   "failed to allocate connection-complete event\n");
			goto fn_fail_data;
		}
		memset(compl, 0 , sizeof(*compl));
		compl->base.event_cb = ofi_nd_ep_completed;
		compl->base.err_cb = ofi_nd_ep_completed_err;
		compl->base.free = ofi_nd_ep_completed_free;
		compl->ep = connect->ep;
		compl->eq = connect->eq;
		compl->connector = connect->connector;
		connect->connector->lpVtbl->AddRef(connect->connector);

		hr = connect->connector->lpVtbl->CompleteConnect(connect->connector,
								 &compl->base.ov);
		if (FAILED(hr)) {
			ND_LOG_WARN(FI_LOG_EP_CTRL,
				   "failed to complete connection\n");
			ND_BUF_FREE(nd_ep_completed, compl);
			goto fn_fail_compl;
		}
	}

	ND_LOG_DEBUG(FI_LOG_EP_CTRL, "register disconnect notification: %p\n",
		    &connect->ep->disconnect_ov.ov);
	hr = connect->connector->lpVtbl->NotifyDisconnect(
		connect->connector, &connect->ep->disconnect_ov.ov);
	if (FAILED(hr)) {
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "failed to notify disconnect\n");
		ND_BUF_FREE(nd_ep_completed, compl);
		goto fn_fail_compl;
	}

	struct fi_eq_cm_entry *cm = (struct fi_eq_cm_entry*)&ev->operation;
	cm->fid = &connect->ep->fid.fid;
	ofi_nd_eq_push(connect->eq, ev);
	ofi_nd_ep_accepted_free(&connect->base);
	connect->ep->connected = 1;
	return;

fn_fail_compl:
	if (len) {
		free(ev->data);
		ev->len = 0;
	}
	connect->connector->lpVtbl->Release(connect->connector);

fn_fail_data:
	ofi_nd_buf_free_nd_eq_event(ev);

fn_fail_ev:
	err = ofi_nd_buf_alloc_nd_eq_event();
	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "failed to allocate error event\n");
		ofi_nd_ep_accepted_free(&connect->base);
		return;
	}
	memset(err, 0, sizeof(*err));
	err->error.err = -H2F(hr);
	err->error.prov_errno = (int)hr;
	err->error.fid = &connect->ep->fid.fid;
	ofi_nd_eq_push_err(connect->eq, err);
	ofi_nd_ep_accepted_free(&connect->base);
}

static void ofi_nd_ep_rejected(nd_event_base *base, DWORD bytes, DWORD error)
{
	assert(base);
	OFI_UNUSED(bytes);

	nd_ep_connect *connect = container_of(base, nd_ep_connect, base);

	assert(connect->connector);
	assert(connect->ep);
	assert(connect->eq);

	HRESULT hr = S_OK;

	struct nd_eq_event *err = ofi_nd_buf_alloc_nd_eq_event();
	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "failed to allocate error event\n");
		ofi_nd_ep_accepted_free(&connect->base);
		return;
	}
	memset(err, 0, sizeof(*err));
	err->error.err = -H2F(error);
	err->error.prov_errno = (int)error;
	err->error.fid = &connect->ep->fid.fid;
	ofi_nd_eq_push_err(connect->eq, err);

	if (error == ND_CONNECTION_REFUSED) {
		ULONG len = 0;
		hr = connect->connector->lpVtbl->GetPrivateData(
			connect->connector, NULL, &len);

		if (FAILED(hr) && hr != ND_BUFFER_OVERFLOW) {
			ND_LOG_WARN(FI_LOG_EP_CTRL,
				   "failed to get connection data\n");
			goto fn_complete;
		}

		if (len) {
			err->error.err_data = malloc((size_t)len);
			if (!err->error.err_data) {
				ND_LOG_WARN(FI_LOG_EP_CTRL,
					   "failed to allocate connection data\n");
				hr = ND_NO_MEMORY;
				goto fn_complete;
			}
			hr = connect->connector->lpVtbl->GetPrivateData(
				connect->connector, err->error.err_data, &len);

			if (FAILED(hr)) {
				ND_LOG_WARN(FI_LOG_EP_CTRL,
					   "failed to copy connection data\n");
				goto fn_complete;
			}
			err->error.err_data_size = (size_t)len;
		}
	}

fn_complete:
	ofi_nd_ep_accepted_free(&connect->base);
}

static int ofi_nd_ep_connect(struct fid_ep *pep, const void *addr,
			     const void *param, size_t paramlen)
{
	assert(pep->fid.fclass == FI_CLASS_EP);

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	if (!addr)
		return -FI_EINVAL;

	int res = fi_enable(&ep->fid);
	if (res)
		return res;

	assert(ep->connector);
	assert(ep->qp);

	HRESULT hr;

	struct nd_ep_connect *wait = ofi_nd_buf_alloc_nd_ep_connect();
	if (!wait)
		return -FI_ENOMEM;

	memset(wait, 0, sizeof(*wait));
	wait->ep = ep;
	wait->eq = ep->eq;
	wait->connector = ep->connector;
	wait->base.event_cb = ofi_nd_ep_accepted;
	wait->base.err_cb = ofi_nd_ep_rejected;
	wait->base.free = ofi_nd_ep_accepted_free;
	wait->active = 1;
	ep->connector->lpVtbl->AddRef(ep->connector);

	hr = ep->connector->lpVtbl->Connect(
		ep->connector, (IUnknown*)ep->qp,
		(struct sockaddr*)addr, (ULONG)ofi_sizeofaddr((struct sockaddr*)addr),
		ep->domain->ainfo.MaxInboundReadLimit,
		ep->domain->ainfo.MaxOutboundReadLimit,
		param, (ULONG)paramlen, &wait->base.ov);
	return H2F(hr);
}

static int ofi_nd_ep_accept(struct fid_ep *pep, const void *param, size_t paramlen)
{
	assert(pep->fid.fclass == FI_CLASS_EP);

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid);

	int res = fi_enable(&ep->fid);
	if (res)
		return res;

	assert(ep->connector);
	assert(ep->qp);

	HRESULT hr;

	struct nd_ep_connect *accept = ofi_nd_buf_alloc_nd_ep_connect();
	if (!accept)
		return -FI_ENOMEM;

	memset(accept, 0, sizeof(*accept));
	accept->ep = ep;
	accept->eq = ep->eq;
	accept->connector = ep->connector;
	accept->base.event_cb = ofi_nd_ep_accepted;
	accept->base.err_cb = ofi_nd_ep_rejected;
	accept->base.free = ofi_nd_ep_accepted_free;
	accept->connector->lpVtbl->AddRef(accept->connector);

	ND_LOG_DEBUG(FI_LOG_EP_CTRL, "sending accept message\n");

	hr = ep->connector->lpVtbl->Accept(
		ep->connector, (IUnknown*)ep->qp,
		ep->domain->ainfo.MaxInboundReadLimit,
		ep->domain->ainfo.MaxOutboundReadLimit,
		param, (ULONG)paramlen, &accept->base.ov);
	if (FAILED(hr))
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to send accept message: %x\n",
			   hr);

	return H2F(hr);
}

static int ofi_nd_ep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	assert(fid && fid->fclass == FI_CLASS_EP);

	if (fid->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	HRESULT hr;
	ULONG len = (ULONG)*addrlen;
	struct nd_ep *ep = container_of(fid, struct nd_ep, fid.fid);

	if (!ep->connector)
		return -FI_EOPBADSTATE;

	hr = ep->connector->lpVtbl->GetLocalAddress(ep->connector,
						    (struct sockaddr *)addr,
						    &len);
	if (*addrlen < len) {
		ND_LOG_INFO(FI_LOG_EP_CTRL,
			    "Provided buffer (size = %"PRIu64") is too small, required = %"PRIu64,
			    addrlen, len);
		*addrlen = (size_t)len;
		return -FI_ETOOSMALL;
	}
	*addrlen = (size_t)len;

	return H2F(hr);
}

static int ofi_nd_ep_getpeer(struct fid_ep *pep, void *addr, size_t *addrlen)
{
	assert(pep);
	assert(pep->fid.fclass == FI_CLASS_EP);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	HRESULT hr;
	ULONG len = (ULONG)*addrlen;
	struct nd_ep *ep = container_of(pep, struct nd_ep, fid.fid);

	if (!ep->connector)
		return -FI_EOPBADSTATE;

	hr = ep->connector->lpVtbl->GetPeerAddress(ep->connector,
		(struct sockaddr*)addr, &len);

	*addrlen = (size_t)len;

	return H2F(hr);
}

static int ofi_nd_ep_bind(fid_t pep, fid_t bfid, uint64_t flags)
{
	assert(pep->fclass == FI_CLASS_EP);

	if (pep->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid.fid);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		ep->eq = container_of(bfid, struct nd_eq, fid.fid);
		return FI_SUCCESS;
	case FI_CLASS_CQ:
		if (flags & FI_TRANSMIT) {
			ep->cq_send = container_of(bfid, struct nd_cq, fid.fid);
			ep->send_flags = flags;
		}
		if (flags & FI_RECV) {
			ep->cq_recv = container_of(bfid, struct nd_cq, fid.fid);
			ep->recv_flags = flags;
		}
		if (flags & FI_REMOTE_READ || flags & FI_REMOTE_WRITE)
			return -FI_EBADFLAGS;
		return FI_SUCCESS;
	case FI_CLASS_CNTR:
		if (flags & FI_SEND)
			ep->cntr_send = container_of(bfid, struct nd_cntr, fid.fid);
		if (flags & FI_RECV)
			ep->cntr_recv = container_of(bfid, struct nd_cntr, fid.fid);
		if (flags & FI_READ)
			ep->cntr_read = container_of(bfid, struct nd_cntr, fid.fid);
		if (flags & FI_WRITE)
			ep->cntr_write = container_of(bfid, struct nd_cntr, fid.fid);
		if (flags & FI_REMOTE_READ || flags & FI_REMOTE_WRITE)
			return -FI_EBADFLAGS;
		return FI_SUCCESS;
	case FI_CLASS_SRX_CTX:
		ep->srx = container_of(bfid, struct nd_srx, fid.fid);
		return FI_SUCCESS;
	default:
		ND_LOG_WARN(FI_LOG_EP_CTRL,
			   "ofi_nd_ep_bind: unknown bind class: %d",
			   (int)bfid->fclass);
		return -FI_EINVAL;
	}
}

static int ofi_nd_ep_shutdown(struct fid_ep *pep, uint64_t flags)
{
	assert(pep);
	assert(pep->fid.fclass == FI_CLASS_EP);

	OFI_UNUSED(flags);

	if (pep->fid.fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	struct nd_ep *ep = container_of(pep, struct nd_ep, fid.fid);

	if (!ep->qp)
		return FI_SUCCESS;

	ofi_nd_unexp_fini(ep);

	HRESULT hr = S_OK;
	ofi_nd_util_ov *ov = NULL;
	if (ep->connected) {
		ep->connected = 0;

		ov = ND_BUF_ALLOC(ofi_nd_util_ov);
		if (!ov)
			return -FI_ENOMEM;

		hr = ep->connector->lpVtbl->Disconnect(ep->connector, &ov->base.ov);
		if (FAILED(hr))
			goto fn_fail;

		hr = ofi_nd_util_ov_wait(ep->connector, ov);
	}

	return H2F(hr);

fn_fail:
	if (ov)
		ND_BUF_FREE(ofi_nd_util_ov, ov);
	return H2F(hr);
}

static void ofi_nd_ep_disconnected_free(struct nd_event_base* base)
{
	OFI_UNUSED(base);
}

static void ofi_nd_ep_disconnected(struct nd_event_base* base, DWORD bytes)
{
	OFI_UNUSED(bytes);

	struct nd_ep *ep = container_of(base, struct nd_ep, disconnect_ov);
	assert(ep->fid.fid.fclass == FI_CLASS_EP);

	ep->connected = 0;
	
	struct nd_eq_event *ev = ND_BUF_ALLOC(nd_eq_event);
	if (!ev) {
		return;
	}
	memset(ev, 0, sizeof(*ev));
	struct fi_eq_cm_entry *cm = (struct fi_eq_cm_entry*)&ev->operation;
	ev->eq_event = FI_SHUTDOWN;
	cm->fid = &ep->fid.fid;
	ofi_nd_eq_push(ep->eq, ev);

	//ofi_nd_ep_shutdown(&ep->fid, 0);
}

static void ofi_nd_ep_disconnected_err(struct nd_event_base* base, DWORD bytes,
				       DWORD err)
{
	if (err == STATUS_CONNECTION_DISCONNECTED) {
		ofi_nd_ep_disconnected(base, bytes);
	}
	else {
		struct nd_ep *ep = container_of(base, struct nd_ep, disconnect_ov);

		struct nd_eq_event *ev = ND_BUF_ALLOC(nd_eq_event);
		if (!ev) {
			return;
		}
		memset(ev, 0, sizeof(*ev));
		ev->eq_event = FI_SHUTDOWN;
		ev->error.err = H2F(err);
		ev->error.prov_errno = err;
		ev->error.fid = &ep->fid.fid;
		ofi_nd_eq_push_err(ep->eq, ev);
	}
}

static ssize_t ofi_nd_ep_cancel(fid_t fid, void *context)
{
	assert(fid);
	assert(fid->fclass == FI_CLASS_EP);
	assert(context);

	if (!context) {
		ND_LOG_WARN(FI_LOG_EP_DATA, "Context is NULL \n");
		return -FI_EINVAL;
	}

	return ofi_nd_cq_cancel(fid, context);
}

#endif /* _WIN32 */

