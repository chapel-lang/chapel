/*
 * Copyright (c) 2022 Canon Medical Research USA, Inc.  All rights reserved.
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

#include <assert.h>
#include "ndspi.h"
#include "verbs_nd.h"

void CALLBACK nd_io_cb(DWORD error, DWORD bytes, LPOVERLAPPED ov)
{
	struct nd_event_base *base;

	assert(ov);
	base = container_of(ov, struct nd_event_base, ov);
	ofi_mutex_lock(&base->lock);

	VRB_DBG(FI_LOG_EP_CTRL, "IO callback: error: %s, bytes: %d, ov: %p\n",
		nd_error_str(error), bytes, ov);

	if (error) {
		assert(base->error_cb);
		base->error_cb(base, bytes, error);
	} else {
		assert(base->event_cb);
		base->event_cb(base, bytes);
	}

	assert(base->cb_pending);
	--base->cb_pending;
	ofi_mutex_unlock(&base->lock);
	pthread_cond_broadcast(&base->cond);
}

HRESULT nd_cancel_pending(struct nd_event_base *event, IND2Overlapped *ov)
{
	HRESULT hr;

	ofi_mutex_lock(&event->lock);
	if (event->cb_pending) {
		hr = ov->lpVtbl->CancelOverlappedRequests(ov);
		if (FAILED(hr)) {
			errno = hresult2fi(hr);
			ofi_mutex_unlock(&event->lock);
			return errno;
		}

		while (event->cb_pending) {
			ofi_pthread_wait_cond(&event->cond, &event->lock,
					      INFINITE);
		}
	}
	ofi_mutex_unlock(&event->lock);

	return 0;
}

struct nd_cm_event *nd_allocate_cm_event(struct rdma_cm_id *id,
					 enum rdma_cm_event_type type)
{
	struct nd_cm_event *cm_event;

	// TODO: Find an alternative to memory allocation in
	// asynchronous completion routines
	cm_event = calloc(1, sizeof(*cm_event));
	assert(cm_event);
	cm_event->event.id = id;
	cm_event->event.event = type;

	return cm_event;
}

void nd_get_read_limits(IND2Connector *connector, struct nd_cm_event *event)
{
	ULONG in = 0;
	ULONG out = 0;
	HRESULT hr;

	hr = connector->lpVtbl->GetReadLimits(connector, &in, &out);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::GetReadLimits: hr=0x%08lx\n",
	       hr);

	assert(in <= 255);
	event->event.param.conn.responder_resources = (uint8_t)in;
	assert(out <= 255);
	event->event.param.conn.initiator_depth = (uint8_t)out;
}

void nd_get_connection_data(IND2Connector *connector, struct nd_cm_event *event)
{
	ULONG len = 0;
	HRESULT hr;

	hr = connector->lpVtbl->GetPrivateData(connector, NULL, &len);
	FI_LOG(&vrb_prov,
	       (FAILED(hr) && (hr != ND_BUFFER_OVERFLOW)) ? FI_LOG_WARN :
								  FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::GetPrivateData: hr=0x%08lx\n",
	       hr);

	if ((SUCCEEDED(hr) || hr == ND_BUFFER_OVERFLOW) && len) {
		event->event.param.conn.private_data_len = (uint8_t)len;
		event->event.param.conn.private_data = malloc(len);
		if (event->event.param.conn.private_data) {
			hr = connector->lpVtbl->GetPrivateData(
				connector,
				(void *)event->event.param.conn.private_data,
				&len);
			FI_LOG(&vrb_prov,
			       FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
			       FI_LOG_EP_CTRL,
			       "IND2Connector::GetPrivateData: hr=0x%08lx\n",
			       hr);
			if (FAILED(hr)) {
				free(event->event.param.conn.private_data);
				event->event.param.conn.private_data = NULL;
				event->event.param.conn.private_data_len = 0;
			}
		} else {
			event->event.param.conn.private_data_len = 0;
			VRB_WARN(
				FI_LOG_EP_CTRL,
				"Failed to allocate memory for connection data.\n");
		}
	} else {
		event->event.param.conn.private_data_len = 0;
		event->event.param.conn.private_data = NULL;
	}
}

static void nd_insert_cm_event(struct rdma_event_channel *channel,
			       struct nd_cm_event *event)
{
	struct nd_event_channel *ch_nd;

	ch_nd = container_of(channel, struct nd_event_channel, channel);
	VRB_DBG(FI_LOG_EQ, "EQ SET event:%p %p %d\n", event->event.id,
		event->event.listen_id, event->event.event);
	dlistfd_insert_tail(&event->entry, &ch_nd->q);
}

// Used in conjunction with IND2Listener::GetConnectionRequest()
void nd_cm_connreq_event(struct nd_event_base *base, DWORD bytes)
{
	struct nd_cm_event *cm_event;
	struct nd_cm_listen_event *event;
	IND2Connector *connector;
	struct rdma_cm_id *id;
	int ret;
	struct nd_cm_id *id_nd;
	struct nd_cm_id *listen_id_nd;
	ULONG len;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	event = container_of(base, struct nd_cm_listen_event, base);
	connector = event->connector;

	ret = rdma_create_id(event->listen_id->channel, &id,
			     event->listen_id->context, RDMA_PS_TCP);
	assert(!ret);
	id->verbs = event->listen_id->verbs;
	id_nd = container_of(id, struct nd_cm_id, id);
	listen_id_nd = container_of(event->listen_id, struct nd_cm_id, id);
	id_nd->connector = listen_id_nd->connector;
	id_nd->connect_event.connector = listen_id_nd->connect_event.connector;
	id_nd->peer_event.connector = listen_id_nd->peer_event.connector;
	id_nd->listen_event.connector = listen_id_nd->listen_event.connector;

	cm_event = nd_allocate_cm_event(id, RDMA_CM_EVENT_CONNECT_REQUEST);
	cm_event->event.listen_id = event->listen_id;

	nd_get_read_limits(connector, cm_event);
	nd_get_connection_data(connector, cm_event);

	len = sizeof(id->route.addr.src_addr);
	hr = connector->lpVtbl->GetLocalAddress(connector,
						&id->route.addr.src_addr, &len);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::GetLocalAddress: hr=0x%08lx\n",
	       hr);

	len = sizeof(id->route.addr.dst_addr);
	hr = connector->lpVtbl->GetPeerAddress(connector,
					       &id->route.addr.dst_addr, &len);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL, "IND2Connector::GetPeerAddress: hr=0x%08lx\n",
	       hr);

	nd_insert_cm_event(id->channel, cm_event);

	// Need to obtain a new connector for the listening id.
	ret = rdma_bind_addr(&listen_id_nd->id,
			     &listen_id_nd->id.route.addr.src_addr);
	assert(!ret);

	ofi_mutex_lock(&listen_id_nd->listen_event.base.lock);
	++listen_id_nd->listen_event.base.cb_pending;
	hr = listen_id_nd->listener->lpVtbl->GetConnectionRequest(
		listen_id_nd->listener, (IUnknown *)listen_id_nd->connector,
		&listen_id_nd->listen_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL,
	       "IND2Listener::GetConnectionRequest: hr=0x%08lx; ov=%p\n", hr,
	       &listen_id_nd->listen_event.base.ov);
	if (FAILED(hr))
		--listen_id_nd->listen_event.base.cb_pending;
	ofi_mutex_unlock(&listen_id_nd->listen_event.base.lock);
}

void nd_cm_connreq_error(struct nd_event_base *base, DWORD bytes, DWORD error)
{
	struct nd_cm_listen_event *event;
	struct nd_cm_event *cm_event;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (error == ND_CANCELED)
		return;

	event = container_of(base, struct nd_cm_listen_event, base);
	cm_event = nd_allocate_cm_event(event->listen_id,
					RDMA_CM_EVENT_CONNECT_ERROR);
	cm_event->event.listen_id = event->listen_id;
	cm_event->event.status = error;

	nd_insert_cm_event(event->listen_id->channel, cm_event);
}

// Used in conjunction with
//   IND2Connector::Connect()
//   IND2Connector::Accept()
//   IND2Connector::CompleteConnect()
//   IND2Connector::NotifyDisconnect()
//   IND2Connector::Disconnect()
static void nd_handle_cm_connect(IND2Connector *connector,
				 struct rdma_cm_id *id, IND2QueuePair *qp)
{
	HRESULT hr;
	struct nd_cm_id *id_nd;
	struct nd_cm_event tmp_event = { 0 };

	id_nd = container_of(id, struct nd_cm_id, id);
	id_nd->connect_event.type = ND_CM_COMPLETE;

	nd_get_read_limits(connector, &tmp_event);
	id_nd->connect_event.param.responder_resources =
		tmp_event.event.param.conn.responder_resources;
	id_nd->connect_event.param.initiator_depth =
		tmp_event.event.param.conn.initiator_depth;

	nd_get_connection_data(connector, &tmp_event);
	id_nd->connect_event.param.private_data_len =
		tmp_event.event.param.conn.private_data_len;
	id_nd->connect_event.param.private_data =
		tmp_event.event.param.conn.private_data;

	ofi_mutex_lock(&id_nd->connect_event.base.lock);
	++id_nd->connect_event.base.cb_pending;
	hr = connector->lpVtbl->CompleteConnect(connector,
						&id_nd->connect_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL,
	       "IND2Connector::CompleteConnect: hr=0x%08lx; ov=%p\n", hr,
	       &id_nd->connect_event.base.ov);
	if (FAILED(hr))
		--id_nd->connect_event.base.cb_pending;
	ofi_mutex_unlock(&id_nd->connect_event.base.lock);
}

static void nd_notify_disconnect(struct rdma_cm_id *id,
				 IND2Connector *connector)
{
	HRESULT hr;
	struct nd_cm_id *id_nd = container_of(id, struct nd_cm_id, id);
	id_nd->peer_event.type = ND_CM_DISCONNECTED;

	ofi_mutex_lock(&id_nd->peer_event.base.lock);
	++id_nd->peer_event.base.cb_pending;
	hr = connector->lpVtbl->NotifyDisconnect(connector,
						 &id_nd->peer_event.base.ov);
	FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
	       FI_LOG_EP_CTRL,
	       "IND2Connector::NotifyDisconnect: hr=0x%08lx; ov=%p\n", hr,
	       &id_nd->peer_event.base.ov);
	if (FAILED(hr))
		--id_nd->peer_event.base.cb_pending;
	ofi_mutex_unlock(&id_nd->peer_event.base.lock);
}

void nd_cm_connect_ack(struct nd_event_base *base, DWORD bytes)
{
	struct nd_cm_connect_event *event;
	IND2Connector *connector;
	struct rdma_cm_id *id;
	IND2QueuePair *qp;
	struct nd_cm_event *cm_event = NULL;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	event = container_of(base, struct nd_cm_connect_event, base);
	connector = event->connector;
	id = event->id;
	qp = event->qp;

	switch (event->type) {
	case ND_CM_CONNECT:
		nd_handle_cm_connect(connector, id, qp);
		return;

	case ND_CM_COMPLETE:
		cm_event = nd_allocate_cm_event(id, RDMA_CM_EVENT_ESTABLISHED);

		cm_event->event.param.conn.responder_resources =
			event->param.responder_resources;
		cm_event->event.param.conn.initiator_depth =
			event->param.initiator_depth;
		cm_event->event.param.conn.private_data_len =
			event->param.private_data_len;
		cm_event->event.param.conn.private_data =
			event->param.private_data;
		nd_insert_cm_event(event->channel, cm_event);
		nd_notify_disconnect(id, connector);
		break;

	case ND_CM_ACCEPT:
		cm_event = nd_allocate_cm_event(id, RDMA_CM_EVENT_ESTABLISHED);
		nd_insert_cm_event(event->channel, cm_event);
		nd_notify_disconnect(id, connector);
		break;

	case ND_CM_DISCONNECT:
	case ND_CM_DISCONNECTED:
		cm_event = nd_allocate_cm_event(id, RDMA_CM_EVENT_DISCONNECTED);
		nd_insert_cm_event(event->channel, cm_event);
		break;

	default:
		break;
	}
}

void nd_cm_connect_nack(struct nd_event_base *base, DWORD bytes, DWORD error)
{
	struct nd_cm_connect_event *event;
	struct nd_cm_event *cm_event = NULL;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	event = container_of(base, struct nd_cm_connect_event, base);

	if (error == ND_CANCELED)
		return;

	if ((event->type == ND_CM_DISCONNECTED) && (error == ND_DISCONNECTED)) {
		nd_cm_connect_ack(base, bytes);
		return;
	}

	switch (event->type) {
	case ND_CM_CONNECT:
	case ND_CM_ACCEPT:
		cm_event = nd_allocate_cm_event(event->id,
						RDMA_CM_EVENT_REJECTED);
		cm_event->event.status = ECONNREFUSED;

		nd_get_read_limits(event->connector, cm_event);
		nd_get_connection_data(event->connector, cm_event);

		hr = event->qp->lpVtbl->Flush(event->qp);
		FI_LOG(&vrb_prov, FAILED(hr) ? FI_LOG_WARN : FI_LOG_DEBUG,
		       FI_LOG_EP_CTRL, "IND2QueuePair::Flush: hr=0x%08lx\n",
		       hr);
		nd_insert_cm_event(event->channel, cm_event);
		break;

	case ND_CM_COMPLETE:
	case ND_CM_DISCONNECT:
		cm_event = nd_allocate_cm_event(event->id,
						RDMA_CM_EVENT_CONNECT_ERROR);
		cm_event->event.status = error;
		nd_insert_cm_event(event->channel, cm_event);
		break;

	default:
		break;
	}
}

// Used in conjunction with IND2CompletionQueue::Notify
void nd_cq_notify_event(struct nd_event_base *base, DWORD bytes)
{
	struct nd_cq *cq_nd;
	struct nd_comp_channel *ch_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	cq_nd = container_of(base, struct nd_cq, notification);
	ch_nd = container_of(cq_nd->cq.channel, struct nd_comp_channel,
			     channel);

	ofi_mutex_lock(&ch_nd->q_lock);
	dlistfd_insert_tail(&cq_nd->entry, &ch_nd->q);
	ofi_mutex_unlock(&ch_nd->q_lock);
}

void nd_cq_notify_error(struct nd_event_base *base, DWORD bytes, DWORD error)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (error == ND_CANCELED) {
		nd_cq_notify_event(base, bytes);
	} else {
		VRB_WARN(FI_LOG_CQ, "Unknown error: %s, bytes %d, ov: %p\n",
			 nd_error_str(error), bytes, base);
	}
}
