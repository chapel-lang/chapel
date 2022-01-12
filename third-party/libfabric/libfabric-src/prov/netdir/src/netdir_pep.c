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

#include <ws2spi.h>
#include <winsock2.h>
#include <windows.h>

#include "netdir.h"

#include "ofi.h"
#include "ofi_osd.h"
#include "ofi_util.h"

#include "netdir_ov.h"
#include "netdir_log.h"
#include "netdir_iface.h"

static int ofi_nd_pep_getname(fid_t fid, void *addr, size_t *addrlen);
static int ofi_nd_pep_close(struct fid *fid);
static int ofi_nd_pep_listen(struct fid_pep *pep);
static int ofi_nd_pep_bind(struct fid *fid, struct fid *bfid, uint64_t flags);
static int ofi_nd_pep_reject(struct fid_pep *ppep, fid_t handle,
			     const void *param, size_t paramlen);

static void ofi_nd_pep_connreq_free(nd_event_base *base);
static void ofi_nd_pep_connreq(nd_event_base *base, DWORD bytes);
static void ofi_nd_pep_connreq_err(nd_event_base *base, DWORD err,
				   DWORD bytes);
static int ofi_nd_pep_getopt(struct fid *ep, int level, int optname,
			void *optval, size_t *optlen);

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_pep_close,
	.bind = ofi_nd_pep_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_PEP,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

static struct fi_ops_cm ofi_nd_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = ofi_nd_pep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = ofi_nd_pep_listen,
	.accept = fi_no_accept,
	.reject = ofi_nd_pep_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static struct fi_ops_ep ofi_nd_pep_ops = {
	.size = sizeof(ofi_nd_pep_ops),
	.cancel = fi_no_cancel,
	.getopt = ofi_nd_pep_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

typedef struct nd_pep_connreq {
	nd_event_base		base;
	struct nd_eq		*eq;
	struct fi_info		*info;
	IND2Connector		*connector;
	fid_t			fid;
} nd_pep_connreq;

static nd_event_base nd_pep_connreq_base_def = {
	.free = ofi_nd_pep_connreq_free,
	.event_cb = ofi_nd_pep_connreq,
	.err_cb = ofi_nd_pep_connreq_err
};

OFI_ND_NB_BUF(nd_pep_connreq);
OFI_ND_NB_BUF_IMP(nd_pep_connreq);
OFI_ND_NB_BUF_TYPED(nd_connreq, struct nd_connreq);
OFI_ND_NB_BUF_IMP(nd_connreq);

int ofi_nd_passive_endpoint(struct fid_fabric *fabric, struct fi_info *info,
			    struct fid_pep **ppep, void *context)
{
	OFI_UNUSED(context);
	OFI_UNUSED(fabric);

	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_connreq));

	assert(info);
	assert(fabric);
	assert(fabric->fid.fclass == FI_CLASS_FABRIC);

	struct nd_pep *pep = (struct nd_pep*)calloc(1, sizeof(*pep));
	if (!pep)
		return -FI_ENOMEM;

	struct nd_pep def = {
		.fid = {
			.fid = {
				.fclass = FI_CLASS_PEP,
				.context = context,
				.ops = &ofi_nd_fi_ops
			},
			.ops = &ofi_nd_pep_ops,
			.cm = &ofi_nd_cm_ops
		},
		.info = fi_dupinfo(info)
	};

	*pep = def;
	*ppep = &pep->fid;

	return FI_SUCCESS;
}

static int ofi_nd_pep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	assert(fid && fid->fclass == FI_CLASS_PEP);

	if (fid->fclass != FI_CLASS_PEP)
		return -FI_EINVAL;

	HRESULT hr;
	ULONG len = (ULONG)*addrlen;
	struct nd_pep *pep = container_of(fid, struct nd_pep, fid.fid);

	if (!pep->listener)
		return -FI_EOPBADSTATE;

	hr = pep->listener->lpVtbl->GetLocalAddress(pep->listener,
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

static int ofi_nd_pep_close(struct fid *fid)
{
	assert(fid);
	assert(fid->fclass == FI_CLASS_PEP);

	struct nd_pep *pep = container_of(fid, struct nd_pep, fid.fid);

	int ref;
	if (pep->listener) {
		ref = (int)pep->listener->lpVtbl->Release(pep->listener);
		ND_LOG_DEBUG(FI_LOG_EP_CTRL, "pep->listener ref count: %d\n", ref);
	}
	if (pep->adapter) {
		ref = (int)pep->adapter->lpVtbl->Release(pep->adapter);
		ND_LOG_DEBUG(FI_LOG_EP_CTRL, "pep->adapter ref count: %d\n", ref);
	}
	if (pep->adapter_file && pep->adapter_file != INVALID_HANDLE_VALUE)
		CloseHandle(pep->adapter_file);
	if (pep->info)
		fi_freeinfo(pep->info);

	free(pep);

	return FI_SUCCESS;
}

static void ofi_nd_pep_connreq_free(nd_event_base *base)
{
	assert(base);

	nd_pep_connreq *connreq = container_of(base, nd_pep_connreq, base);
	if (connreq->connector)
		connreq->connector->lpVtbl->Release(connreq->connector);
	ofi_nd_buf_free_nd_pep_connreq(connreq);
}

static void ofi_nd_pep_connreq(nd_event_base *base, DWORD bytes)
{
	assert(base);
	OFI_UNUSED(bytes);

	HRESULT hr;
	ULONG len;
	nd_pep_connreq *connreq = container_of(base, nd_pep_connreq, base);
	struct nd_eq_event *err = 0;

	assert(connreq->connector);
	assert(connreq->eq);
	assert(connreq->fid);
	assert(connreq->info);

	struct nd_eq_event *ev = ND_BUF_ALLOC(nd_eq_event);
	if (!ev) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to allocate event\n");
		hr = ND_NO_MEMORY;
		goto fn_fail_ev;
	}
	memset(ev, 0, sizeof(*ev));

	ev->eq_event = FI_CONNREQ;

	struct fi_eq_cm_entry *cmev = (struct fi_eq_cm_entry*)&ev->operation;
	cmev->fid = connreq->fid;
	cmev->info = fi_dupinfo(connreq->info);
	if (!cmev->info) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to copy info\n");
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}

	struct nd_connreq *handle = ND_BUF_ALLOC(nd_connreq);
	if (!handle) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to allocate handle\n");
		hr = ND_NO_MEMORY;
		goto fn_fail;
	}
	memset(handle, 0, sizeof(*handle));
	handle->handle.fclass = FI_CLASS_CONNREQ;
	handle->connector = connreq->connector;
	handle->connector->lpVtbl->AddRef(handle->connector);
	cmev->info->handle = &handle->handle;

	hr = connreq->connector->lpVtbl->GetPrivateData(
		connreq->connector, NULL, &len);
	if (FAILED(hr) && hr != ND_BUFFER_OVERFLOW) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to get private data\n");
		goto fn_fail_handle;
	}

	if (len) {
		ev->data = malloc(len);
		if (!ev->data) {
			ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to allocate private data\n");
			ev->len = 0;
			goto fn_fail_handle;
		}

		hr = connreq->connector->lpVtbl->GetPrivateData(
			connreq->connector, ev->data, &len);
		if (FAILED(hr)) {
			ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to copy private data\n");
			free(ev->data);
			ev->len = 0;
			goto fn_fail_handle;
		}
	}
	ev->len = (size_t)len;

	ofi_nd_eq_push(connreq->eq, ev);
	ofi_nd_pep_connreq_free(&connreq->base);
	return;

fn_fail_handle:
	handle->connector->lpVtbl->Release(handle->connector);
	ND_BUF_FREE(nd_connreq, handle);
fn_fail:
	ofi_nd_eq_free_event(ev);
fn_fail_ev:
	err = ND_BUF_ALLOC(nd_eq_event);
	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to allocate error\n");
		return;
	}
	memset(err, 0, sizeof(*err));
	err->error.err = -H2F(hr);
	err->error.prov_errno = (int)hr;
	err->error.fid = connreq->fid;
	ofi_nd_eq_push_err(connreq->eq, err);
	ofi_nd_pep_connreq_free(&connreq->base);
}

static void ofi_nd_pep_connreq_err(nd_event_base *base, DWORD error, DWORD bytes)
{
	assert(base);
	OFI_UNUSED(bytes);

	nd_pep_connreq *connreq = container_of(base, nd_pep_connreq, base);
	struct nd_eq_event *err = 0;

	assert(connreq->connector);
	assert(connreq->eq);
	assert(connreq->fid);
	assert(connreq->info);

	err = ofi_nd_buf_alloc_nd_eq_event();

	if (!err) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to allocate error\n");
		return;
	}
	memset(err, 0, sizeof(*err));
	err->error.err = FI_EOTHER;
	err->error.prov_errno = (int)error;
	err->error.fid = connreq->fid;
	ofi_nd_eq_push_err(connreq->eq, err);
	ofi_nd_pep_connreq_free(&connreq->base);
}

static int ofi_nd_pep_listen(struct fid_pep *ppep)
{
	assert(ppep);

	int res = FI_SUCCESS;
	HRESULT hr;

	if (ppep->fid.fclass != FI_CLASS_PEP)
		return -FI_EINVAL;

	struct nd_pep *pep = container_of(ppep, struct nd_pep, fid);

	assert(pep->info);
	assert(pep->info->domain_attr);
	assert(pep->info->domain_attr->name);

	struct sockaddr* addr;

	if (!pep->adapter) {
		struct sockaddr* listen_addr = NULL;
		size_t listen_addr_len = 0;

		res = ofi_nd_lookup_adapter(pep->info->domain_attr->name,
					    &pep->adapter, &addr);
		if (res != FI_SUCCESS)
			return res;
		assert(pep->adapter);

		hr = pep->adapter->lpVtbl->CreateOverlappedFile(pep->adapter,
								&pep->adapter_file);
		if (FAILED(hr))
			return H2F(hr);
		assert(pep->adapter_file &&
			      pep->adapter_file != INVALID_HANDLE_VALUE);

		BindIoCompletionCallback(pep->adapter_file, domain_io_cb, 0);

		hr = pep->adapter->lpVtbl->CreateListener(pep->adapter,
							  &IID_IND2Listener,
							  pep->adapter_file,
							  (void**)&pep->listener);
		if (FAILED(hr))
			return H2F(hr);
		assert(pep->listener);

		if (pep->info->src_addr) {
			/* uses address that is specified in fi_info */
			listen_addr = pep->info->src_addr;
			listen_addr_len = pep->info->src_addrlen;
		}
		else {
			/* uses address on which provider are open */
			listen_addr = addr;
			listen_addr_len = ofi_sizeofaddr(addr);
		}

		hr = pep->listener->lpVtbl->Bind(pep->listener,
					listen_addr,
					(ULONG)sizeof(*listen_addr));
		if (FAILED(hr))
			return H2F(hr);

		hr = pep->listener->lpVtbl->Listen(pep->listener, 0);
		if (FAILED(hr))
			return H2F(hr);
	}
	assert(pep->adapter);

	ND_REGISTER_FINI(ND_BUF_FINIPTR(nd_pep_connreq));

	nd_pep_connreq *conn = ofi_nd_buf_alloc_nd_pep_connreq();
	if (!conn)
		return -FI_ENOMEM;
	memset(conn, 0, sizeof(*conn));

	conn->base = nd_pep_connreq_base_def;

	hr = pep->adapter->lpVtbl->CreateConnector(pep->adapter,
						   &IID_IND2Connector,
						   pep->adapter_file,
						   (void**)&conn->connector);
	if (FAILED(hr))
		return H2F(hr);

	conn->eq = pep->eq;
	conn->info = pep->info;
	conn->fid = &pep->fid.fid;

	hr = pep->listener->lpVtbl->GetConnectionRequest(pep->listener,
		(IUnknown*)conn->connector,
		&conn->base.ov);
	if (FAILED(hr)) {
		ND_LOG_WARN(FI_LOG_EP_CTRL, "failed to get connection request\n");
	}

	return H2F(hr);
}

static int ofi_nd_pep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	OFI_UNUSED(flags);

	if (fid->fclass != FI_CLASS_PEP)
		return -FI_EINVAL;
	if (bfid->fclass != FI_CLASS_EQ)
		return -FI_EINVAL;

	struct nd_pep *pep = container_of(fid, struct nd_pep, fid.fid);
	struct nd_eq *eq = container_of(bfid, struct nd_eq, fid.fid);

	pep->eq = eq;

	return FI_SUCCESS;
}

static int ofi_nd_pep_reject(struct fid_pep *ppep, fid_t handle,
			     const void *param, size_t paramlen)
{
	assert(ppep);

	if (ppep->fid.fclass != FI_CLASS_PEP)
		return -FI_EINVAL;
	if (handle->fclass != FI_CLASS_CONNREQ)
		return -FI_EINVAL;

	struct nd_connreq *connreq = container_of(handle, struct nd_connreq, handle);

	assert(connreq->connector);
	connreq->connector->lpVtbl->Reject(connreq->connector, param,
					   (ULONG)paramlen);

	connreq->connector->lpVtbl->Release(connreq->connector);
	ofi_nd_buf_free_nd_connreq(connreq);

	return FI_SUCCESS;
}

static int ofi_nd_pep_getopt(struct fid *ep, int level, int optname,
			void *optval, size_t *optlen)
{
	OFI_UNUSED(ep);

	assert(level == FI_OPT_ENDPOINT &&
		optname == FI_OPT_CM_DATA_SIZE);
	assert(optval);
	assert(optlen);

	if (level != FI_OPT_ENDPOINT || optname != FI_OPT_CM_DATA_SIZE)
		return -FI_ENOPROTOOPT;

	if (*optlen < sizeof(size_t)) {
		*optlen = sizeof(size_t);
		return -FI_ETOOSMALL;
	}

	*((size_t *)optval) = ND_EP_MAX_CM_DATA_SIZE;
	*optlen = sizeof(size_t);

	return 0;
}

#endif /* _WIN32 */

