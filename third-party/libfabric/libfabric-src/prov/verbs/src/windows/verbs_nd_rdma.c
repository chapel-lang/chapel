/*
 * Copyright (c) 2022 Canon Medical Research USA, Inc.  All rights reserved.
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

#include <assert.h>
#include "ndspi.h"
#include "verbs_nd.h"

struct ibv_context **rdma_get_devices(int *num_devices)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (num_devices) {
		assert(nd_infra.adapters.count <= INT_MAX);
		*num_devices = (int)nd_infra.adapters.count;
	}
	return nd_infra.adapters.context_list;
}

void rdma_free_devices(struct ibv_context **list)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	// Since we are keeping state statically, there is nothing to free.
}

struct rdma_event_channel *rdma_create_event_channel(void)
{
	struct nd_event_channel *channel_nd;
	int ret;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	channel_nd = calloc(1, sizeof(*channel_nd));
	if (!channel_nd) {
		errno = ENOMEM;
		return NULL;
	}

	ret = dlistfd_head_init(&channel_nd->q);
	if (ret) {
		free(channel_nd);
		errno = ret;
		return NULL;
	}
	channel_nd->channel.fd = fd_signal_get(&channel_nd->q.signal);

	return &channel_nd->channel;
}

void rdma_destroy_event_channel(struct rdma_event_channel *channel)
{
	struct nd_event_channel *channel_nd;
	struct nd_cm_event *entry_nd;
	struct rdma_cm_event *event;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!channel) {
		errno = EINVAL;
		return;
	}

	channel_nd = container_of(channel, struct nd_event_channel, channel);
	while (!dlistfd_empty(&channel_nd->q)) {
		entry_nd = container_of(channel_nd->q.list.next,
					struct nd_cm_event, entry);
		event = &entry_nd->event;
		dlistfd_remove(channel_nd->q.list.next, &channel_nd->q);

		if (event->param.conn.private_data) {
			free((void *)event->param.conn.private_data);
		}
		free(event);
	}
	dlistfd_head_free(&channel_nd->q);

	free(channel_nd);
}

int rdma_create_id(struct rdma_event_channel *channel, struct rdma_cm_id **id,
		   void *context, enum rdma_port_space ps)
{
	struct nd_cm_id *id_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!id) {
		errno = EINVAL;
		return -1;
	}
	if (ps != RDMA_PS_TCP) {
		errno = EINVAL;
		return -1;
	}

	id_nd = calloc(1, sizeof(*id_nd));
	if (!id_nd) {
		errno = ENOMEM;
		return -1;
	}
	*id = &id_nd->id;

	(*id)->channel = channel;
	(*id)->context = context;
	(*id)->ps = ps;
	(*id)->qp_type = IBV_QPT_RC;

	id_nd->ov_file = INVALID_HANDLE_VALUE;
	ofi_atomic_initialize32(&id_nd->cm_events_pending, 0);

	ofi_mutex_init(&id_nd->connect_event.base.lock);
	pthread_cond_init(&id_nd->connect_event.base.cond, 0);
	id_nd->connect_event.base.event_cb = nd_cm_connect_ack;
	id_nd->connect_event.base.error_cb = nd_cm_connect_nack;
	id_nd->connect_event.id = *id;
	id_nd->connect_event.channel = channel;

	ofi_mutex_init(&id_nd->peer_event.base.lock);
	pthread_cond_init(&id_nd->peer_event.base.cond, 0);
	id_nd->peer_event.base.event_cb = nd_cm_connect_ack;
	id_nd->peer_event.base.error_cb = nd_cm_connect_nack;
	id_nd->peer_event.id = *id;
	id_nd->peer_event.channel = channel;

	ofi_mutex_init(&id_nd->listen_event.base.lock);
	pthread_cond_init(&id_nd->listen_event.base.cond, 0);
	id_nd->listen_event.base.event_cb = nd_cm_connreq_event;
	id_nd->listen_event.base.error_cb = nd_cm_connreq_error;
	id_nd->listen_event.listen_id = *id;

	return 0;
}

static void remove_cm_id_events_from_channel(struct rdma_cm_id *id)
{
	struct nd_cm_event *entry_nd;
	struct dlist_entry *tmp;
	struct nd_event_channel *ch_nd =
		container_of(id->channel, struct nd_event_channel, channel);

	dlist_foreach_container_safe(&ch_nd->q.list, struct nd_cm_event,
				     entry_nd, entry, tmp)
	{
		if (entry_nd->event.id == id) {
			dlistfd_remove(&entry_nd->entry, &ch_nd->q);
			free((void *)entry_nd->event.param.conn.private_data);
			free(&entry_nd->event);
		}
	}
}

int rdma_destroy_id(struct rdma_cm_id *id)
{
	struct nd_cm_id *id_nd;
	int32_t compare;
	int ret = 0;
	ULONG refcnt;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!id) {
		errno = EINVAL;
		return -1;
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	if (nd_cancel_pending(&id_nd->connect_event.base,
			      (IND2Overlapped *)id_nd->connector) ||
	    nd_cancel_pending(&id_nd->peer_event.base,
			      (IND2Overlapped *)id_nd->connector) ||
	    nd_cancel_pending(&id_nd->listen_event.base,
			      (IND2Overlapped *)id_nd->listener))
		return -1;

	while (compare = ofi_atomic_get32(&id_nd->cm_events_pending)) {
		WaitOnAddress(&id_nd->cm_events_pending, &compare,
			      sizeof(id_nd->cm_events_pending), INFINITE);
	}

	if (id->channel) {
		remove_cm_id_events_from_channel(id);
	}

	if (id_nd->listener) {
		ret = id_nd->listener->lpVtbl->Release(id_nd->listener);
		assert(!ret);
	}
	if (id_nd->connector) {
		refcnt = id_nd->connector->lpVtbl->Release(id_nd->connector);
		assert(!refcnt);
	}

	if (id->qp) {
		ibv_destroy_qp(id->qp);
		id->qp = NULL;
	}
	// Should also destroy any associated SRQ here.

	if (id_nd->ov_file != INVALID_HANDLE_VALUE) {
		ret = CloseHandle(id_nd->ov_file);
		assert(ret);
	}

	pthread_cond_destroy(&id_nd->listen_event.base.cond);
	ofi_mutex_destroy(&id_nd->listen_event.base.lock);
	pthread_cond_destroy(&id_nd->peer_event.base.cond);
	ofi_mutex_destroy(&id_nd->peer_event.base.lock);
	pthread_cond_destroy(&id_nd->connect_event.base.cond);
	ofi_mutex_destroy(&id_nd->connect_event.base.lock);
	free(id_nd);

	return 0;
}

int rdma_migrate_id(struct rdma_cm_id *id, struct rdma_event_channel *channel)
{
	struct nd_cm_id *id_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!channel)) {
		errno = EINVAL;
		return -1;
	}

	id->channel = channel;
	id_nd = container_of(id, struct nd_cm_id, id);
	id_nd->connect_event.channel = channel;
	id_nd->peer_event.channel = channel;
	return 0;
}

int rdma_bind_addr(struct rdma_cm_id *id, struct sockaddr *addr)
{
	char name[INET6_ADDRSTRLEN];
	struct nd_adapter *adapter = NULL;
	int i;
	struct nd_cm_id *id_nd;
	HRESULT hr;
	int ret;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!addr)) {
		errno = EINVAL;
		return -1;
	}

	if (!inet_ntop(addr->sa_family, ofi_get_ipaddr(addr), name,
		       INET6_ADDRSTRLEN)) {
		return -1;
	}

	for (i = 0; i < nd_infra.adapters.count; ++i) {
		if (!strncmp(name, nd_infra.adapters.adapter[i].name,
			     strlen(name)) ||
		    !strncmp(name, "0.0.0.0", strlen(name))) {
			id->verbs = nd_infra.adapters.context_list[i];
			adapter = nd_infra.adapters.adapter_list[i];
			id->route.addr.src_addr = *addr;
			break;
		}
		if (!strncmp(name, "127.0.0.1", strlen(name)) &&
		    (nd_infra.adapters.adapter[i].info.AdapterFlags &
		     ND_ADAPTER_FLAG_LOOPBACK_CONNECTIONS_SUPPORTED)) {
			id->route.addr.src_addr = *addr;
			break;
		}
	}
	if (i == nd_infra.adapters.count) {
		errno = ENODEV;
		return -1;
	}
	if (!adapter) {
		return 0;
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	hr = adapter->adapter->lpVtbl->CreateOverlappedFile(adapter->adapter,
							    &id_nd->ov_file);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL,
	       "IND2Adapter::CreateOverlappedFile: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		return -1;
	}

	assert(id_nd->ov_file && id_nd->ov_file != INVALID_HANDLE_VALUE);

	BindIoCompletionCallback(id_nd->ov_file, nd_io_cb, 0);

	hr = adapter->adapter->lpVtbl->CreateConnector(
		adapter->adapter, &IID_IND2Connector, id_nd->ov_file,
		(void **)&id_nd->connector);
	id_nd->listen_event.connector = id_nd->connector;
	id_nd->connect_event.connector = id_nd->connector;
	id_nd->peer_event.connector = id_nd->connector;
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Adapter::CreateConnector: hr=0x%08lx\n",
	       hr);
	if (FAILED(hr)) {
		ret = CloseHandle(id_nd->ov_file);
		assert(ret);
		errno = hresult2fi(hr);
		return -1;
	}

	return 0;
}

int rdma_resolve_addr(struct rdma_cm_id *id, struct sockaddr *src_addr,
		      struct sockaddr *dst_addr, int timeout_ms)
{
	struct nd_cm_id *id_nd;
	HRESULT hr;
	ULONG addrlen;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!dst_addr)) {
		errno = EINVAL;
		return -1;
	}

	if (!id->verbs) {
		if (!src_addr) {
			if (nd_infra.adapters.count == 0) {
				errno = ENODEV;
				return -1;
			}
			src_addr = &nd_infra.adapters.adapter[0].address.addr;
		}

		if (rdma_bind_addr(id, src_addr)) {
			return -1;
		}
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	hr = id_nd->connector->lpVtbl->Bind(id_nd->connector,
					    &id->route.addr.src_addr,
					    sizeof(id->route.addr.src_addr));
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::Bind: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		return -1;
	}

	addrlen = sizeof(id->route.addr.src_addr);
	hr = id_nd->connector->lpVtbl->GetLocalAddress(
		id_nd->connector, &id->route.addr.src_addr, &addrlen);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::GetLocalAddress: hr=0x%08lx\n",
	       hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		return -1;
	}

	memcpy(&id->route.addr.dst_addr, dst_addr, ofi_sizeofaddr(dst_addr));
	return 0;
}

int rdma_resolve_route(struct rdma_cm_id *id, int timeout_ms)
{
	struct nd_cm_event *entry_nd;
	struct nd_event_channel *ch_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!id->channel)) {
		errno = EINVAL;
		return -1;
	}

	entry_nd = calloc(1, sizeof(*entry_nd));
	if (!entry_nd) {
		errno = ENOMEM;
		return -1;
	}

	entry_nd->event.id = id;
	entry_nd->event.listen_id = NULL;
	entry_nd->event.event = RDMA_CM_EVENT_ROUTE_RESOLVED;
	entry_nd->event.status = 0;

	ch_nd = container_of(id->channel, struct nd_event_channel, channel);
	dlistfd_insert_tail(&entry_nd->entry, &ch_nd->q);
	VRB_DBG(FI_LOG_EQ, "EQ SET event:%p %p %d\n", entry_nd->event.id,
		entry_nd->event.listen_id, entry_nd->event.event);

	return 0;
}

int rdma_listen(struct rdma_cm_id *id, int backlog)
{
	struct nd_adapter *adapter;
	struct nd_cm_id *id_nd;
	HRESULT hr;
	ULONG addrlen;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!id) {
		errno = EINVAL;
		return -1;
	}

	adapter = nd_get_adapter_by_context(id->verbs);
	if (!adapter) {
		errno = ENODEV;
		return -1;
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	if (id_nd->listener) {
		errno = EADDRINUSE;
		return -1;
	}

	hr = adapter->adapter->lpVtbl->CreateListener(
		adapter->adapter, &IID_IND2Listener, id_nd->ov_file,
		(void **)&id_nd->listener);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Adapter::CreateListener: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		return -1;
	}

	hr = id_nd->listener->lpVtbl->Bind(id_nd->listener,
					   &id->route.addr.src_addr,
					   sizeof(id->route.addr.src_addr));
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Listener::Bind: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err1;
	}

	addrlen = sizeof(id->route.addr.src_addr);
	hr = id_nd->listener->lpVtbl->GetLocalAddress(
		id_nd->listener, &id->route.addr.src_addr, &addrlen);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Listener::GetLocalAddress: hr=0x%08lx\n",
	       hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err1;
	}

	hr = id_nd->listener->lpVtbl->Listen(id_nd->listener, backlog);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Listener::Listen: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err1;
	}

	ofi_mutex_lock(&id_nd->listen_event.base.lock);
	++id_nd->listen_event.base.cb_pending;
	hr = id_nd->listener->lpVtbl->GetConnectionRequest(
		id_nd->listener, (IUnknown *)id_nd->connector,
		&id_nd->listen_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL,
	       "IND2Listener::GetConnectionRequest: hr=0x%08lx; ov=%p\n", hr,
	       &id_nd->listen_event.base.ov);
	if (FAILED(hr)) {
		--id_nd->listen_event.base.cb_pending;
		errno = hresult2fi(hr);
		ofi_mutex_unlock(&id_nd->listen_event.base.lock);
		goto err1;
	}
	ofi_mutex_unlock(&id_nd->listen_event.base.lock);

	return 0;
err1:
	id_nd->listener->lpVtbl->Release(id_nd->listener);
	id_nd->listener = NULL;
	return -1;
}

int rdma_connect(struct rdma_cm_id *id, struct rdma_conn_param *conn_param)
{
	struct nd_cm_id *id_nd;
	struct nd_qp *qp_nd;
	HRESULT hr;
	int ret = 0;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!conn_param)) {
		errno = EINVAL;
		return -1;
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	qp_nd = container_of(id->qp, struct nd_qp, qp);

	id_nd->connect_event.type = ND_CM_CONNECT;

	ofi_mutex_lock(&id_nd->connect_event.base.lock);
	++id_nd->connect_event.base.cb_pending;
	hr = id_nd->connector->lpVtbl->Connect(
		id_nd->connector, (IUnknown *)qp_nd->nd2qp,
		&id->route.addr.dst_addr, sizeof(id->route.addr.dst_addr),
		conn_param->responder_resources, conn_param->initiator_depth,
		conn_param->private_data, conn_param->private_data_len,
		&id_nd->connect_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::Connect: hr=0x%08lx; ov=%p\n",
	       hr, &id_nd->connect_event.base.ov);
	if (FAILED(hr)) {
		--id_nd->connect_event.base.cb_pending;
		errno = hresult2fi(hr);
		ret = -1;
	}
	ofi_mutex_unlock(&id_nd->connect_event.base.lock);

	return 0;
}

int rdma_accept(struct rdma_cm_id *id, struct rdma_conn_param *conn_param)
{
	struct nd_cm_id *id_nd;
	struct nd_qp *qp_nd;
	HRESULT hr;
	int ret = 0;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!conn_param)) {
		errno = EINVAL;
		return -1;
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	qp_nd = container_of(id->qp, struct nd_qp, qp);

	id_nd->connect_event.type = ND_CM_ACCEPT;

	ofi_mutex_lock(&id_nd->connect_event.base.lock);
	++id_nd->connect_event.base.cb_pending;
	hr = id_nd->connector->lpVtbl->Accept(
		id_nd->connector, (IUnknown *)qp_nd->nd2qp,
		conn_param->responder_resources, conn_param->initiator_depth,
		conn_param->private_data, conn_param->private_data_len,
		&id_nd->connect_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::Accept: hr=0x%08lx; ov=%p\n", hr,
	       &id_nd->connect_event.base.ov);
	if (FAILED(hr)) {
		--id_nd->connect_event.base.cb_pending;
		errno = hresult2fi(hr);
		ret = -1;
	}
	ofi_mutex_unlock(&id_nd->connect_event.base.lock);

	return ret;
}

int rdma_reject(struct rdma_cm_id *id, const void *private_data,
		uint8_t private_data_len)
{
	struct nd_cm_id *id_nd;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!id) {
		errno = EINVAL;
		return -1;
	}

	id_nd = container_of(id, struct nd_cm_id, id);
	hr = id_nd->connector->lpVtbl->Reject(id_nd->connector, private_data,
					      private_data_len);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::Reject: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		return -1;
	}

	return 0;
}

int rdma_disconnect(struct rdma_cm_id *id)
{
	struct nd_cm_id *id_nd;
	HRESULT hr;
	int ret = 0;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!id) {
		errno = EINVAL;
		return -1;
	}

	id_nd = container_of(id, struct nd_cm_id, id);

	if (nd_cancel_pending(&id_nd->peer_event.base,
			      (IND2Overlapped *)id_nd->connector))
		return -1;

	ofi_mutex_lock(&id_nd->connect_event.base.lock);
	id_nd->connect_event.type = ND_CM_DISCONNECT;
	++id_nd->connect_event.base.cb_pending;
	hr = id_nd->connector->lpVtbl->Disconnect(
		id_nd->connector, &id_nd->connect_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::Disconnect: hr=0x%08lx, ov=%p\n",
	       hr, &id_nd->connect_event.base.ov);
	if (FAILED(hr)) {
		--id_nd->connect_event.base.cb_pending;
		errno = hresult2fi(hr);
		ret = -1;
	}
	ofi_mutex_unlock(&id_nd->connect_event.base.lock);

	return ret;
}

int rdma_get_cm_event(struct rdma_event_channel *channel,
		      struct rdma_cm_event **event)
{
	struct nd_event_channel *ch_nd;
	char byte;
	int ret;
	struct nd_cm_event *entry_nd;
	struct nd_cm_id *id_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!channel) || (!event)) {
		errno = EINVAL;
		return -1;
	}

	ch_nd = container_of(channel, struct nd_event_channel, channel);

	// Check to see if the channel fd has been set to
	// non-blocking mode and we would block.
	if ((ofi_recv_socket(ch_nd->channel.fd, &byte, 1, MSG_PEEK) == -1) &&
	    (WSAGetLastError() == WSAEWOULDBLOCK)) {
		errno = EAGAIN;
		return -1;
	}

	ret = dlistfd_wait_avail(&ch_nd->q, -1);
	if (ret != 1) {
		errno = ret;
		return -1;
	}
	entry_nd = container_of(ch_nd->q.list.next, struct nd_cm_event, entry);
	*event = &entry_nd->event;
	if (entry_nd->event.id) {
		id_nd = container_of(entry_nd->event.id, struct nd_cm_id, id);
		ofi_atomic_inc32(&id_nd->cm_events_pending);
	}
	VRB_DBG(FI_LOG_EQ, "EQ GET event:%p %p %d\n", entry_nd->event.id,
		entry_nd->event.listen_id, entry_nd->event.event);
	dlistfd_remove(ch_nd->q.list.next, &ch_nd->q);

	return 0;
}

int rdma_ack_cm_event(struct rdma_cm_event *event)
{
	struct nd_cm_id *id_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!event) {
		errno = EINVAL;
		return -1;
	}

	if (event->id) {
		VRB_DBG(FI_LOG_EQ, "EQ ACK event:%p %p %d\n", event->id,
			event->listen_id, event->event);
		id_nd = container_of(event->id, struct nd_cm_id, id);
		ofi_atomic_dec32(&id_nd->cm_events_pending);
		WakeByAddressAll(&id_nd->cm_events_pending);
	}

	if (event->param.conn.private_data) {
		free((void *)event->param.conn.private_data);
	}
	free(event);

	return 0;
}

int rdma_create_qp(struct rdma_cm_id *id, struct ibv_pd *pd,
		   struct ibv_qp_init_attr *qp_init_attr)
{
	struct nd_cm_id *id_nd;
	struct nd_qp *qp_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!id) || (!qp_init_attr)) {
		errno = EINVAL;
		return -1;
	}

	if ((!pd) && (!id->pd)) {
		errno = EINVAL;
		return -1;
	}

	if (!qp_init_attr->recv_cq || !qp_init_attr->send_cq) {
		errno = EINVAL;
		return -1;
	}

	if (id->qp) {
		return 0;
	}

	if (pd) {
		id->pd = pd;
	}

	id->qp = ibv_create_qp(id->pd, qp_init_attr);
	if (!id->qp) {
		return -1;
	}
	id_nd = container_of(id, struct nd_cm_id, id);
	qp_nd = container_of(id->qp, struct nd_qp, qp);
	id_nd->connect_event.qp = qp_nd->nd2qp;
	id_nd->peer_event.qp = qp_nd->nd2qp;

	return 0;
}

void rdma_destroy_ep(struct rdma_cm_id *id)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	rdma_destroy_id(id);
}

__be16 rdma_get_src_port(struct rdma_cm_id *id) // Used with XRC
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return 0;
}

const char *rdma_event_str(enum rdma_cm_event_type event) // Used with XRC
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return NULL;
}
