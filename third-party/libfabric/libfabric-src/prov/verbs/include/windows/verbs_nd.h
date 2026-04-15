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

#ifndef _VERBS_ND_H_
#define _VERBS_ND_H_

#include <assert.h>
#include "ndspi.h"
#include "verbs_ofi.h"


HRESULT nd_startup();
void nd_shutdown();

int nd_is_valid_addr(const SOCKADDR *addr);
int nd_addr_cmp(const void *vaddr1, const void *vaddr2);
int nd_is_same_file(const wchar_t *path1, const wchar_t *path2);
int nd_file_exists(const wchar_t *path);
int nd_is_directory(const wchar_t *path);

char *nd_error_str(HRESULT hr);

static inline int hresult2fi(HRESULT hr)
{
	switch (hr) {
	case S_OK:
	case ND_PENDING:
		return FI_SUCCESS;
	case ND_BUFFER_OVERFLOW:
		return -FI_EOVERFLOW;
	case ND_CONNECTION_REFUSED:
		return -FI_ECONNREFUSED;
	case ND_TIMEOUT:
		return -FI_ETIMEDOUT;
	default:
		return -FI_EOTHER;
	}
}

#define ND_DISCONNECTED 0xc000020C // Undocumented ND error code.

typedef HRESULT (*dll_can_unload_now)(void);
typedef HRESULT (*dll_get_class_object)(REFCLSID rclsid, REFIID rrid,
					LPVOID *ppv);

struct nd_module {
	const wchar_t *path;
	HMODULE module;
	dll_can_unload_now can_unload_now;
	dll_get_class_object get_class_object;
};

struct nd_factory {
	WSAPROTOCOL_INFOW protocol;
	IClassFactory *class_factory;
	IND2Provider *provider;
	struct nd_module *module;
	SOCKET_ADDRESS_LIST *addr_list;
};

struct nd_adapter {
	union {
		struct sockaddr addr;
		struct sockaddr_in addr4;
		struct sockaddr_in6 addr6;
	} address;
	ND2_ADAPTER_INFO info;
	IND2Adapter *adapter;
	struct nd_factory *factory;
	const char *name;
	struct ibv_device device;
	struct ibv_context context;
};

struct nd_infrastructure {
	struct {
		struct nd_module *modules;
		size_t count;
	} providers;

	struct {
		struct nd_factory *factory;
		size_t count;
	} class_factories;

	struct {
		struct nd_adapter *adapter;
		struct nd_adapter **adapter_list;
		struct ibv_device **device_list;
		struct ibv_context **context_list;
		size_t count;
	} adapters;
};
extern struct nd_infrastructure nd_infra;

static inline struct nd_adapter *
nd_get_adapter_by_context(struct ibv_context *context)
{
	for (int i = 0; i < nd_infra.adapters.count; ++i) {
		if (nd_infra.adapters.context_list[i] == context) {
			return nd_infra.adapters.adapter_list[i];
		}
	}
	return NULL;
}

static inline struct ibv_context *
nd_get_context_by_device(struct ibv_device *device)
{
	for (int i = 0; i < nd_infra.adapters.count; ++i) {
		if (nd_infra.adapters.device_list[i] == device) {
			return nd_infra.adapters.context_list[i];
		}
	}
	return NULL;
}

typedef void (*nd_event_handler)(struct nd_event_base *base, DWORD bytes);
typedef void (*nd_error_handler)(struct nd_event_base *base, DWORD bytes,
				 DWORD error);

struct nd_event_base {
	OVERLAPPED ov;
	nd_event_handler event_cb;
	nd_error_handler error_cb;
	ofi_mutex_t lock;
	pthread_cond_t cond;
	uint32_t cb_pending;
};

HRESULT nd_cancel_pending(struct nd_event_base *event, IND2Overlapped *ov);

void CALLBACK nd_io_cb(DWORD error, DWORD bytes, LPOVERLAPPED ov);

// Used in conjunction with IND2Listener::GetConnectionRequest()
struct nd_cm_listen_event {
	struct nd_event_base base;
	IND2Connector *connector;
	struct rdma_cm_id *listen_id;
};

void nd_cm_connreq_event(struct nd_event_base *base, DWORD bytes);
void nd_cm_connreq_error(struct nd_event_base *base, DWORD bytes, DWORD error);

// Used in conjunction with
//   IND2Connector::Connect()
//   IND2Connector::Accept()
//   IND2Connector::CompleteConnect()
//   IND2Connector::Disconnect()
//   IND2Connector::NotifyDisconnect()
enum nd_cm_event_type {
	ND_CM_NONE,
	ND_CM_CONNECT,
	ND_CM_ACCEPT,
	ND_CM_COMPLETE,
	ND_CM_DISCONNECT,
	ND_CM_DISCONNECTED
};

struct nd_cm_connect_event {
	struct nd_event_base base;
	enum nd_cm_event_type type;
	IND2Connector *connector;
	struct rdma_cm_id *id;
	struct rdma_event_channel *channel;
	IND2QueuePair *qp;
	struct {
		const void *private_data;
		uint8_t private_data_len;
		uint8_t responder_resources;
		uint8_t initiator_depth;
	} param;
};

void nd_cm_connect_ack(struct nd_event_base *base, DWORD bytes);
void nd_cm_connect_nack(struct nd_event_base *base, DWORD bytes, DWORD error);

// Used in conjunction with IND2CompletionQueue::Notify
void nd_cq_notify_event(struct nd_event_base *base, DWORD bytes);
void nd_cq_notify_error(struct nd_event_base *base, DWORD bytes, DWORD error);

// Wrapper structs
struct nd_cm_id {
	struct rdma_cm_id id;
	HANDLE ov_file;
	IND2Connector *connector;
	struct nd_cm_connect_event connect_event;
	struct nd_cm_connect_event peer_event;
	IND2Listener *listener;
	struct nd_cm_listen_event listen_event;
	ofi_atomic32_t cm_events_pending;
};

struct nd_event_channel {
	struct rdma_event_channel channel;
	struct dlistfd_head q;
};

struct nd_cm_event {
	struct rdma_cm_event event;
	struct dlist_entry entry;
};

struct nd_cq {
	struct ibv_cq cq;
	HANDLE ov_file;
	IND2CompletionQueue *nd2cq;
	struct nd_event_base notification;
	struct dlist_entry entry;
};

struct nd_mr {
	struct ibv_mr mr;
	HANDLE ov_file;
	IND2MemoryRegion *region;
};

struct nd_qp {
	struct ibv_qp qp;
	IND2QueuePair *nd2qp;
	uint32_t max_inline_data;
};

struct nd_comp_channel {
	struct ibv_comp_channel channel;
	ofi_mutex_t q_lock;
	struct dlistfd_head q;
};

#endif
