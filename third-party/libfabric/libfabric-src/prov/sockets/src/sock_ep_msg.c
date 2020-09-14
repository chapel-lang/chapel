/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
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

#include "config.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <limits.h>

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_EP_CTRL, __VA_ARGS__)


static int sock_ep_cm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct sock_ep *sock_ep = NULL;
	struct sock_pep *sock_pep = NULL;
	size_t len;

	switch (fid->fclass) {
	case FI_CLASS_EP:
	case FI_CLASS_SEP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		if (sock_ep->attr->is_enabled == 0)
			return -FI_EOPBADSTATE;

		len = MIN(*addrlen, ofi_sizeofaddr(&sock_ep->attr->src_addr->sa));
		memcpy(addr, sock_ep->attr->src_addr, len);
		*addrlen = ofi_sizeofaddr(&sock_ep->attr->src_addr->sa);
		break;
	case FI_CLASS_PEP:
		sock_pep = container_of(fid, struct sock_pep, pep.fid);
		if (!sock_pep->name_set)
			return -FI_EOPBADSTATE;

		len = MIN(*addrlen, ofi_sizeofaddr(&sock_pep->src_addr.sa));
		memcpy(addr, &sock_pep->src_addr, len);
		*addrlen = ofi_sizeofaddr(&sock_pep->src_addr.sa);
		break;
	default:
		SOCK_LOG_ERROR("Invalid argument\n");
		return -FI_EINVAL;
	}

	return (len == *addrlen) ? 0 : -FI_ETOOSMALL;
}

static int sock_pep_create_listener(struct sock_pep *pep)
{
	int ret;
	socklen_t addr_size;

	pep->cm.sock = ofi_socket(pep->src_addr.sa.sa_family,
				  SOCK_STREAM, IPPROTO_TCP);
	if (pep->cm.sock == INVALID_SOCKET)
		return -ofi_sockerr();

	sock_set_sockopts(pep->cm.sock, SOCK_OPTS_NONBLOCK);

	ret = bind(pep->cm.sock, &pep->src_addr.sa,
		   ofi_sizeofaddr(&pep->src_addr.sa));
	if (ret) {
		SOCK_LOG_ERROR("failed to bind listener: %s\n",
			       strerror(ofi_sockerr()));
		ret = -ofi_sockerr();
		goto err;
	}

	addr_size = sizeof(pep->src_addr);
	if (ofi_getsockname(pep->cm.sock, &pep->src_addr.sa, &addr_size) ==
	    SOCKET_ERROR) {
		ret = -ofi_sockerr();
		goto err;
	}

	if (listen(pep->cm.sock, sock_cm_def_map_sz)) {
		SOCK_LOG_ERROR("failed to listen socket: %s\n",
			       strerror(ofi_sockerr()));
		ret = -ofi_sockerr();
		goto err;
	}

	pep->cm.do_listen = 1;
	pep->name_set = 1;
	return 0;
err:
	if (pep->cm.sock) {
		ofi_close_socket(pep->cm.sock);
		pep->cm.sock = INVALID_SOCKET;
	}

	return ret;
}

static int sock_ep_cm_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct sock_ep *sock_ep = NULL;
	struct sock_pep *sock_pep = NULL;

	if (!addrlen || addrlen != ofi_sizeofaddr(addr))
		return -FI_EINVAL;

	switch (fid->fclass) {
	case FI_CLASS_EP:
	case FI_CLASS_SEP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		if (sock_ep->attr->conn_handle.do_listen)
			return -FI_EINVAL;
		memcpy(sock_ep->attr->src_addr, addr, addrlen);
		return sock_conn_listen(sock_ep->attr);
	case FI_CLASS_PEP:
		sock_pep = container_of(fid, struct sock_pep, pep.fid);
		if (sock_pep->cm.listener_thread)
			return -FI_EINVAL;
		memcpy(&sock_pep->src_addr, addr, addrlen);
		return sock_pep_create_listener(sock_pep);
	default:
		SOCK_LOG_ERROR("Invalid argument\n");
		return -FI_EINVAL;
	}
}

static int sock_ep_cm_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen)
{
	struct sock_ep *sock_ep;
	size_t len;

	sock_ep = container_of(ep, struct sock_ep, ep);
	len = MIN(*addrlen, ofi_sizeofaddr(&sock_ep->attr->dest_addr->sa));
	memcpy(addr, sock_ep->attr->dest_addr, len);
	*addrlen = ofi_sizeofaddr(&sock_ep->attr->dest_addr->sa);
	return (len == *addrlen) ? 0 : -FI_ETOOSMALL;
}

static int sock_cm_send(int fd, const void *buf, int len)
{
	int ret, done = 0;

	while (done != len) {
		ret = ofi_send_socket(fd, (const char*) buf + done,
				      len - done, MSG_NOSIGNAL);
		if (ret < 0) {
			if (OFI_SOCK_TRY_SND_RCV_AGAIN(ofi_sockerr()))
				continue;
			SOCK_LOG_ERROR("failed to write to fd: %s\n",
				       strerror(ofi_sockerr()));
			return -FI_EIO;
		}
		done += ret;
	}
	return 0;
}

static int sock_cm_recv(int fd, void *buf, int len)
{
	int ret, done = 0;
	while (done != len) {
		ret = ofi_recv_socket(fd, (char*) buf + done, len - done, 0);
		if (ret <= 0) {
			if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
				continue;
			SOCK_LOG_ERROR("failed to read from fd: %s\n",
				       strerror(ofi_sockerr()));
			return -FI_EIO;
		}
		done += ret;
	}
	return 0;
}

static void sock_ep_cm_monitor_handle(struct sock_ep_cm_head *cm_head,
                                      struct sock_conn_req_handle *handle,
                                      uint32_t events)
{
	int ret;

	fastlock_acquire(&cm_head->signal_lock);
	if (handle->monitored)
		goto unlock;

	/* Mark the handle as monitored before adding it to the pollset */
	handle->monitored = 1;
	ret = ofi_epoll_add(cm_head->emap, handle->sock_fd,
	                   events, handle);
	if (ret) {
		SOCK_LOG_ERROR("failed to monitor fd %d: %d\n",
		               handle->sock_fd, ret);
		handle->monitored = 0;
	} else {
		fd_signal_set(&cm_head->signal);
	}
unlock:
	fastlock_release(&cm_head->signal_lock);
}

static void
sock_ep_cm_unmonitor_handle_locked(struct sock_ep_cm_head *cm_head,
                                   struct sock_conn_req_handle *handle,
                                   int close_socket)
{
	int ret;

	if (handle->monitored) {
		ret = ofi_epoll_del(cm_head->emap, handle->sock_fd);
		if (ret)
			SOCK_LOG_ERROR("failed to unmonitor fd %d: %d\n",
			               handle->sock_fd, ret);
		handle->monitored = 0;
	}

	/* Multiple threads might call sock_ep_cm_unmonitor_handle() at the
	 * same time. Some caution is required to prevent a socket from being
	 * close concurrently, which could cause an unexpected socket to be
	 * closed by mistake. */
	if (close_socket && handle->sock_fd != INVALID_SOCKET) {
		ofi_close_socket(handle->sock_fd);
		handle->sock_fd = INVALID_SOCKET;
	}
}

static void sock_ep_cm_unmonitor_handle(struct sock_ep_cm_head *cm_head,
                                       struct sock_conn_req_handle *handle,
                                       int close_socket)
{
	fastlock_acquire(&cm_head->signal_lock);
	sock_ep_cm_unmonitor_handle_locked(cm_head, handle, close_socket);
	fastlock_release(&cm_head->signal_lock);
}

static void sock_ep_cm_shutdown_report(struct sock_ep *ep, int send_shutdown)
{
	struct fi_eq_cm_entry cm_entry = {0};
	struct sock_conn_hdr msg = {0};
	enum sock_cm_state old_state;

	fastlock_acquire(&ep->attr->cm.lock);
	old_state = ep->attr->cm.state;
	switch (ep->attr->cm.state) {
	case SOCK_CM_STATE_REQUESTED:
	/* fallthrough */
	case SOCK_CM_STATE_CONNECTED:
		ep->attr->cm.state = SOCK_CM_STATE_DISCONNECTED;
		break;
	case SOCK_CM_STATE_DISCONNECTED:
		/* Nothing to do, already disconnected */
		break;
	default:
		assert(0);
		break;
	}
	fastlock_release(&ep->attr->cm.lock);

	switch (old_state) {
	case SOCK_CM_STATE_CONNECTED:
		if (send_shutdown) {
			msg.type = SOCK_CONN_SHUTDOWN;
			if (sock_cm_send(ep->attr->cm.sock, &msg, sizeof(msg)))
				SOCK_LOG_DBG("failed to send shutdown msg\n");
		}

		cm_entry.fid = &ep->ep.fid;
		SOCK_LOG_DBG("reporting FI_SHUTDOWN\n");
		if (sock_eq_report_event(ep->attr->eq, FI_SHUTDOWN,
					 &cm_entry, sizeof(cm_entry), 0))
			SOCK_LOG_ERROR("Error in writing to EQ\n");
		break;
	case SOCK_CM_STATE_REQUESTED:
		SOCK_LOG_DBG("reporting FI_REJECT\n");
		if (sock_eq_report_error(ep->attr->eq, &ep->ep.fid, NULL, 0,
					 FI_ECONNREFUSED, -FI_ECONNREFUSED,
					 NULL, 0))
			SOCK_LOG_ERROR("Error in writing to EQ\n");
		break;

	case SOCK_CM_STATE_DISCONNECTED:
		/* Nothing to do, already disconnected */
		break;
	default:
		assert(0);
		break;
	}
}

static void sock_ep_cm_shutdown_handler(struct sock_ep_cm_head *cm_head,
                                        struct sock_conn_hdr *hdr,
                                        struct sock_conn_req_handle *handle)
{
	struct sock_ep *ep = handle->ep;
	assert(ep);

	assert(hdr->type == SOCK_CONN_SHUTDOWN);
	sock_ep_cm_shutdown_report(ep, 0);
	sock_ep_cm_unmonitor_handle_locked(cm_head, handle, 1);
}

static void sock_ep_cm_report_connect_fail(struct sock_ep *ep,
					   void *param, size_t paramlen)
{
	int do_report = 0;

	fastlock_acquire(&ep->attr->cm.lock);
	if (ep->attr->cm.state == SOCK_CM_STATE_REQUESTED) {
		do_report = 1;
		ep->attr->cm.state = SOCK_CM_STATE_DISCONNECTED;
	}
	fastlock_release(&ep->attr->cm.lock);

	if (do_report) {
		SOCK_LOG_DBG("reporting FI_REJECT\n");
		if (sock_eq_report_error(ep->attr->eq, &ep->ep.fid, NULL, 0,
					 FI_ECONNREFUSED, -FI_ECONNREFUSED,
					 param, paramlen))
			SOCK_LOG_ERROR("Error in writing to EQ\n");
	}
}

/* Caller must hold `cm_head::signal_lock` */
static void sock_ep_cm_add_to_msg_list(struct sock_ep_cm_head *cm_head,
				       struct sock_conn_req_handle *handle)
{
	dlist_insert_tail(&handle->entry, &cm_head->msg_list);
	fd_signal_set(&cm_head->signal);
}

static void sock_ep_cm_connect_handler(struct sock_ep_cm_head *cm_head,
                                       struct sock_conn_hdr *hdr,
                                       struct sock_conn_req_handle *handle)
{
	int sock_fd = handle->sock_fd;
	struct sock_ep *ep = handle->ep;
	void *param = NULL;
	struct fi_eq_cm_entry *cm_entry = NULL;
	int cm_data_sz, response_port;

	assert(hdr->type == SOCK_CONN_ACCEPT
	       || hdr->type == SOCK_CONN_REJECT);

	cm_data_sz = ntohs(hdr->cm_data_sz);
	response_port = ntohs(hdr->port);
	if (cm_data_sz) {
		param = calloc(1, cm_data_sz);
		if (!param)
			goto err;

		if (sock_cm_recv(sock_fd, param, cm_data_sz))
			goto err;
	}

	if (hdr->type == SOCK_CONN_REJECT) {
		sock_ep_cm_report_connect_fail(handle->ep, param, cm_data_sz);
		sock_ep_cm_unmonitor_handle_locked(cm_head, handle, 1);
	} else {
		cm_entry = calloc(1, sizeof(*cm_entry) + SOCK_EP_MAX_CM_DATA_SZ);
		if (!cm_entry)
			goto err;

		cm_entry->fid = &ep->ep.fid;
		memcpy(&cm_entry->data, param, cm_data_sz);
		ep->attr->cm.state = SOCK_CM_STATE_CONNECTED;
		ep->attr->cm.sock = sock_fd;
		ep->attr->msg_dest_port = response_port;
		SOCK_LOG_DBG("got accept - port: %d\n", response_port);

		SOCK_LOG_DBG("Reporting FI_CONNECTED\n");
		if (sock_eq_report_event(ep->attr->eq, FI_CONNECTED, cm_entry,
					 sizeof(*cm_entry) + cm_data_sz, 0))
			SOCK_LOG_ERROR("Error in writing to EQ\n");
	}
	goto out;
err:
	SOCK_LOG_ERROR("io failed : %s\n", strerror(ofi_sockerr()));
	sock_ep_cm_report_connect_fail(handle->ep, NULL, 0);
	sock_ep_cm_unmonitor_handle_locked(cm_head, handle, 1);
	handle->ep->attr->info.handle = NULL;
	/* Register handle for later deletion */
	handle->state = SOCK_CONN_HANDLE_DELETED;
	/* `cm_head::signal_lock` has already been held
	 * in `sock_ep_cm_thread` function */
	sock_ep_cm_add_to_msg_list(cm_head, handle);
out:
	free(param);
	free(cm_entry);
}

static struct sock_conn_req_handle *sock_ep_cm_new_handle(void)
{
	struct sock_conn_req_handle *handle;

	handle = calloc(1, sizeof(*handle));
	if (handle) {
		pthread_mutex_init(&handle->finalized_mutex, NULL);
		pthread_cond_init(&handle->finalized_cond, NULL);
		handle->state = SOCK_CONN_HANDLE_ACTIVE;
	}
	return handle;
}

static int sock_ep_cm_connect(struct fid_ep *ep, const void *addr,
			      const void *param, size_t paramlen)
{
	struct sock_conn_req *req = NULL;
	struct sock_ep_cm_head *cm_head = NULL;
	struct sock_conn_req_handle *handle = NULL;
	int sock_fd, ret;
	struct sock_ep *_ep;
	struct sock_eq *_eq;

	_ep = container_of(ep, struct sock_ep, ep);
	_eq = _ep->attr->eq;
	if (!_eq || !addr || (paramlen > SOCK_EP_MAX_CM_DATA_SZ))
		return -FI_EINVAL;

	if (!_ep->attr->conn_handle.do_listen && sock_conn_listen(_ep->attr))
		return -FI_EINVAL;

	if (!_ep->attr->dest_addr) {
		_ep->attr->dest_addr = calloc(1, sizeof(*_ep->attr->dest_addr));
		if (!_ep->attr->dest_addr)
			return -FI_ENOMEM;
	}
	memcpy(_ep->attr->dest_addr, addr, ofi_sizeofaddr(addr));

	req = calloc(1, sizeof(*req));
	if (!req)
		return -FI_ENOMEM;

	handle = sock_ep_cm_new_handle();
	if (!handle) {
		ret = -FI_ENOMEM;
		goto err;
	}

	req->hdr.type = SOCK_CONN_REQ;
	req->hdr.port = htons(_ep->attr->msg_src_port);
	req->hdr.cm_data_sz = htons(paramlen);
	req->caps = _ep->attr->info.caps;
	memcpy(&req->src_addr, _ep->attr->src_addr,
	       ofi_sizeofaddr(&_ep->attr->src_addr->sa));
	memcpy(&handle->dest_addr, addr, ofi_sizeofaddr(addr));

	cm_head = &_ep->attr->domain->cm_head;
	_ep->attr->info.handle = (void*) handle;
	handle->ep = _ep;
	handle->req = req;
	if (paramlen) {
		handle->paramlen = paramlen;
		memcpy(handle->cm_data, param, paramlen);
	}

	sock_fd = ofi_socket(handle->dest_addr.sa.sa_family, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		SOCK_LOG_ERROR("no socket\n");
		ret = -ofi_sockerr();
		goto err;
	}

	ofi_straddr_dbg(&sock_prov, FI_LOG_EP_CTRL, "Connecting to address",
			&handle->dest_addr);
	sock_set_sockopts(sock_fd, SOCK_OPTS_KEEPALIVE);
	ret = connect(sock_fd, &handle->dest_addr.sa,
		      ofi_sizeofaddr(&handle->dest_addr.sa));
	if (ret < 0) {
		SOCK_LOG_ERROR("connect failed : %s\n",
			       strerror(ofi_sockerr()));
		ret = -ofi_sockerr();
		goto close_socket;
	}

	ret = sock_cm_send(sock_fd, req, sizeof(*req));
	if (ret)
		goto close_socket;

	if (handle->paramlen) {
		ret = sock_cm_send(sock_fd, handle->cm_data, handle->paramlen);
		if (ret)
			goto close_socket;
	}

	/* Monitor the connection */
	_ep->attr->cm.state = SOCK_CM_STATE_REQUESTED;
	handle->sock_fd = sock_fd;
	sock_ep_cm_monitor_handle(cm_head, handle, OFI_EPOLL_IN);

	return 0;
close_socket:
	SOCK_LOG_ERROR("io failed : %s\n", strerror(errno));
	ofi_close_socket(sock_fd);
err:
	_ep->attr->info.handle = NULL;
	free(req);
	free(handle);
	return ret;
}

static int sock_ep_cm_accept(struct fid_ep *ep, const void *param, size_t paramlen)
{
	int ret;
	struct sock_ep_cm_head *cm_head = NULL;
	struct sock_conn_req_handle *handle;
	struct sock_ep_attr *ep_attr;
	struct fi_eq_cm_entry cm_entry;
	struct sock_conn_hdr reply;
	struct sock_ep *_ep;

	_ep = container_of(ep, struct sock_ep, ep);
	if (!_ep->attr->eq || paramlen > SOCK_EP_MAX_CM_DATA_SZ)
		return -FI_EINVAL;

	if (!_ep->attr->conn_handle.do_listen && sock_conn_listen(_ep->attr))
		return -FI_EINVAL;

	handle = container_of(_ep->attr->info.handle,
			      struct sock_conn_req_handle, handle);
	if (!handle || handle->handle.fclass != FI_CLASS_CONNREQ) {
		SOCK_LOG_ERROR("invalid handle for cm_accept\n");
		return -FI_EINVAL;
	}

	handle->ep = _ep;
	handle->paramlen = 0;
	handle->state = SOCK_CONN_HANDLE_ACCEPTED;
	if (paramlen) {
		handle->paramlen = paramlen;
		memcpy(handle->cm_data, param, paramlen);
	}
	cm_head = &_ep->attr->domain->cm_head;
	ep_attr = handle->ep->attr;
	ep_attr->msg_dest_port = ntohs(handle->req->hdr.port);

	reply.type = SOCK_CONN_ACCEPT;
	reply.port = htons(ep_attr->msg_src_port);
	reply.cm_data_sz = htons(handle->paramlen);
	ret = sock_cm_send(handle->sock_fd, &reply, sizeof(reply));
	if (ret) {
		SOCK_LOG_ERROR("failed to reply\n");
		return ret;
	}

	if (handle->paramlen) {
		ret = sock_cm_send(handle->sock_fd, handle->cm_data, handle->paramlen);
		if (ret) {
			SOCK_LOG_ERROR("failed to send userdata\n");
			return ret;
		}
	}
	/* Monitor the handle prior to report the event */
	sock_ep_cm_monitor_handle(cm_head, handle, OFI_EPOLL_IN);
	sock_ep_enable(ep);

	memset(&cm_entry, 0, sizeof(cm_entry));
	cm_entry.fid = &handle->ep->ep.fid;
	SOCK_LOG_DBG("reporting FI_CONNECTED\n");
	if (sock_eq_report_event(ep_attr->eq, FI_CONNECTED, &cm_entry,
				 sizeof(cm_entry), 0))
		SOCK_LOG_ERROR("Error in writing to EQ\n");
	ep_attr->cm.state = SOCK_CM_STATE_CONNECTED;
	ep_attr->cm.sock = handle->sock_fd;

	return 0;
}

static int sock_ep_cm_shutdown(struct fid_ep *ep, uint64_t flags)
{
	struct sock_ep *_ep;

	_ep = container_of(ep, struct sock_ep, ep);
	sock_ep_cm_shutdown_report(_ep, 1);

	ofi_close_socket(_ep->attr->cm.sock);
	_ep->attr->cm.sock = INVALID_SOCKET;
	sock_ep_disable(ep);
	return 0;
}

struct fi_ops_cm sock_ep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = sock_ep_cm_setname,
	.getname = sock_ep_cm_getname,
	.getpeer = sock_ep_cm_getpeer,
	.connect = sock_ep_cm_connect,
	.listen = fi_no_listen,
	.accept = sock_ep_cm_accept,
	.reject = fi_no_reject,
	.shutdown = sock_ep_cm_shutdown,
	.join = fi_no_join,
};

int sock_msg_ep(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **ep, void *context)
{
	struct sock_ep *endpoint;
	struct sock_pep *pep;
	int ret;

	ret = sock_alloc_endpoint(domain, info, &endpoint, context, FI_CLASS_EP);
	if (ret)
		return ret;

	if (info && info->handle && info->handle->fclass == FI_CLASS_PEP) {
		pep = container_of(info->handle, struct sock_pep, pep.fid);
		*endpoint->attr->src_addr = pep->src_addr;
	}

	*ep = &endpoint->ep;
	return 0;
}

static int sock_pep_fi_bind(fid_t fid, struct fid *bfid, uint64_t flags)
{
	struct sock_pep *pep;
	struct sock_eq *eq;

	pep = container_of(fid, struct sock_pep, pep.fid);

	if (bfid->fclass != FI_CLASS_EQ)
		return -FI_EINVAL;

	eq = container_of(bfid, struct sock_eq, eq.fid);
	if (pep->sock_fab != eq->sock_fab) {
		SOCK_LOG_ERROR("Cannot bind Passive EP and EQ on different fabric\n");
		return -FI_EINVAL;
	}
	pep->eq = eq;
	return 0;
}

static int sock_pep_fi_close(fid_t fid)
{
	int ret;
	char c = 0;
	struct sock_pep *pep;

	pep = container_of(fid, struct sock_pep, pep.fid);
	pep->cm.do_listen = 0;
	ret = ofi_write_socket(pep->cm.signal_fds[0], &c, 1);
	if (ret != 1)
		SOCK_LOG_DBG("Failed to signal\n");

	if (pep->cm.listener_thread &&
	    pthread_join(pep->cm.listener_thread, NULL)) {
		SOCK_LOG_DBG("pthread join failed\n");
	}

	sock_ep_cm_stop_thread(&pep->cm_head);

	ofi_close_socket(pep->cm.signal_fds[0]);
	ofi_close_socket(pep->cm.signal_fds[1]);

	free(pep);
	return 0;
}

static struct fi_ops sock_pep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_pep_fi_close,
	.bind = sock_pep_fi_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_info *sock_ep_msg_get_info(struct sock_pep *pep,
					    struct sock_conn_req *req)
{
	struct fi_info hints;
	uint64_t requested, supported;

	requested = req->caps & sock_msg_info.caps;
	supported = pep->info.caps & sock_msg_info.caps;
	supported = (supported & FI_RMA) ?
		(supported | FI_REMOTE_READ | FI_REMOTE_WRITE) : supported;
	if ((requested | supported) != supported)
		return NULL;

	hints = pep->info;
	hints.caps = req->caps;
	return sock_fi_info(pep->sock_fab->fab_fid.api_version, FI_EP_MSG,
			    &hints, &pep->src_addr, &req->src_addr);
}

void sock_ep_cm_signal(struct sock_ep_cm_head *cm_head)
{
	fastlock_acquire(&cm_head->signal_lock);
	fd_signal_set(&cm_head->signal);
	fastlock_release(&cm_head->signal_lock);
}

static void sock_ep_cm_process_rejected(struct sock_ep_cm_head *cm_head,
                                        struct sock_conn_req_handle *hreq)
{
	struct sock_conn_hdr reply;

	reply.type = SOCK_CONN_REJECT;
	reply.cm_data_sz = htons(hreq->paramlen);

	SOCK_LOG_DBG("sending reject message\n");
	if (sock_cm_send(hreq->sock_fd, &reply, sizeof(reply))) {
		SOCK_LOG_ERROR("failed to reply\n");
		goto free_handle;
	}

	if (hreq->paramlen && sock_cm_send(hreq->sock_fd, hreq->cm_data,
					   hreq->paramlen)) {
		SOCK_LOG_ERROR("failed to send userdata\n");
		goto free_handle;
	}

free_handle:
	sock_ep_cm_unmonitor_handle(cm_head, hreq, 1);
	free(hreq->req);
	free(hreq);
}

static void sock_ep_cm_process_deleted(struct sock_ep_cm_head *cm_head,
                                       struct sock_conn_req_handle *hreq)
{
	free(hreq->req);
	free(hreq);
}

static void sock_ep_cm_process_finalizing(struct sock_ep_cm_head *cm_head,
                                          struct sock_conn_req_handle *hreq)
{
	sock_ep_cm_unmonitor_handle(cm_head, hreq, 1);

	pthread_mutex_lock(&hreq->finalized_mutex);
	hreq->state = SOCK_CONN_HANDLE_FINALIZED;
	pthread_cond_signal(&hreq->finalized_cond);
	pthread_mutex_unlock(&hreq->finalized_mutex);
}

static struct sock_conn_req_handle *
sock_ep_cm_pop_from_msg_list(struct sock_ep_cm_head *cm_head)
{
	struct dlist_entry *entry;
	struct sock_conn_req_handle *hreq = NULL;

	fastlock_acquire(&cm_head->signal_lock);
	if (!dlist_empty(&cm_head->msg_list)) {
		entry = cm_head->msg_list.next;
		dlist_remove(entry);
		hreq = container_of(entry, struct sock_conn_req_handle, entry);
	}
	fastlock_release(&cm_head->signal_lock);
	return hreq;
}

static void
sock_ep_cm_check_closing_rejected_list(struct sock_ep_cm_head *cm_head)
{
	struct sock_conn_req_handle *hreq;

	while ((hreq = sock_ep_cm_pop_from_msg_list(cm_head)) != NULL) {
		switch (hreq->state) {
		case SOCK_CONN_HANDLE_REJECTED:
			sock_ep_cm_process_rejected(cm_head, hreq);
			break;
		case SOCK_CONN_HANDLE_FINALIZING:
			sock_ep_cm_process_finalizing(cm_head, hreq);
			break;
		case SOCK_CONN_HANDLE_DELETED:
			sock_ep_cm_process_deleted(cm_head, hreq);
			break;
		default:
			assert(0);
			break;
		}
	}
}

static void sock_pep_req_handler(struct sock_ep_cm_head *cm_head,
                                 struct sock_conn_hdr *hdr,
                                 struct sock_conn_req_handle *handle)
{
	int ret, entry_sz;
	struct fi_info *info;
	struct sock_conn_req *conn_req = NULL;
	struct fi_eq_cm_entry *cm_entry = NULL;
	int req_cm_data_sz;

	assert(hdr->type == SOCK_CONN_REQ);

	conn_req = calloc(1, sizeof(*conn_req) + SOCK_EP_MAX_CM_DATA_SZ);
	if (!conn_req) {
		SOCK_LOG_ERROR("cannot allocate memory\n");
		goto err;
	}

	memcpy(&conn_req->hdr, hdr, sizeof(*hdr));

	ret = sock_cm_recv(handle->sock_fd,
	                   &conn_req->src_addr,
	                   sizeof(*conn_req) - sizeof(struct sock_conn_hdr));
	if (ret) {
		SOCK_LOG_ERROR("IO failed\n");
		goto err;
	}

	req_cm_data_sz = ntohs(conn_req->hdr.cm_data_sz);
	if (req_cm_data_sz) {
		ret = sock_cm_recv(handle->sock_fd, conn_req->cm_data,
				   req_cm_data_sz);
		if (ret) {
			SOCK_LOG_ERROR("IO failed for cm-data\n");
			goto err;
		}
	}

	info = sock_ep_msg_get_info(handle->pep, conn_req);
	if (!info) {
		handle->paramlen = 0;
		handle->state = SOCK_CONN_HANDLE_REJECTED;
		/* `cm_head::signal_lock` has already been held
		 * in `sock_ep_cm_thread` function */
		sock_ep_cm_add_to_msg_list(cm_head, handle);

		free(conn_req);
		return;
	}

	cm_entry = calloc(1, sizeof(*cm_entry) + req_cm_data_sz);
	if (!cm_entry) {
		SOCK_LOG_ERROR("cannot allocate memory\n");
		goto err;
	}

	handle->handle.fclass = FI_CLASS_CONNREQ;
	handle->req = conn_req;

	entry_sz = sizeof(*cm_entry) + req_cm_data_sz;
	cm_entry->fid = &handle->pep->pep.fid;
	cm_entry->info = info;
	cm_entry->info->handle = &handle->handle;
	memcpy(cm_entry->data, conn_req->cm_data, req_cm_data_sz);

	sock_ep_cm_unmonitor_handle_locked(cm_head, handle, 0);

	SOCK_LOG_DBG("reporting conn-req to EQ\n");
	if (sock_eq_report_event(handle->pep->eq, FI_CONNREQ, cm_entry, entry_sz, 0))
		SOCK_LOG_ERROR("Error in writing to EQ\n");

	free(cm_entry);
	return;
err:
	ofi_close_socket(handle->sock_fd);
	free(cm_entry);
	free(conn_req);
	free(handle);
}

static void *sock_pep_listener_thread(void *data)
{
	struct sock_pep *pep = (struct sock_pep *) data;
	struct sock_conn_req_handle *handle = NULL;
	struct pollfd poll_fds[2];

	int ret = 0, conn_fd;
	char tmp = 0;

	SOCK_LOG_DBG("Starting listener thread for PEP: %p\n", pep);
	poll_fds[0].fd = pep->cm.sock;
	poll_fds[1].fd = pep->cm.signal_fds[1];
	poll_fds[0].events = poll_fds[1].events = POLLIN;
	while (*((volatile int *) &pep->cm.do_listen)) {
		ret = poll(poll_fds, 2, -1);
		if (ret > 0) {
			if (poll_fds[1].revents & POLLIN) {
				ret = ofi_read_socket(pep->cm.signal_fds[1], &tmp, 1);
				if (ret != 1)
					SOCK_LOG_DBG("Invalid signal\n");
				continue;
			}
		} else {
			break;
		}

		conn_fd = accept(pep->cm.sock, NULL, 0);
		if (conn_fd < 0) {
			SOCK_LOG_ERROR("failed to accept: %d\n", ofi_sockerr());
			continue;
		}

		sock_set_sockopts(conn_fd, SOCK_OPTS_KEEPALIVE);
		handle = sock_ep_cm_new_handle();
		if (!handle) {
			SOCK_LOG_ERROR("cannot allocate memory\n");
			ofi_close_socket(conn_fd);
			break;
		}

		handle->sock_fd = conn_fd;
		handle->pep = pep;

		/* Monitor the connection */
		sock_ep_cm_monitor_handle(&pep->cm_head, handle, OFI_EPOLL_IN);
	}

	SOCK_LOG_DBG("PEP listener thread exiting\n");
	ofi_close_socket(pep->cm.sock);
	return NULL;
}

static int sock_pep_start_listener_thread(struct sock_pep *pep)
{
	if (pthread_create(&pep->cm.listener_thread, NULL,
			   sock_pep_listener_thread, (void *)pep)) {
		SOCK_LOG_ERROR("Couldn't create listener thread\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int sock_pep_listen(struct fid_pep *pep)
{
	struct sock_pep *_pep;
	_pep = container_of(pep, struct sock_pep, pep);
	if (_pep->cm.listener_thread)
		return 0;

	if (sock_ep_cm_start_thread(&_pep->cm_head)) {
		SOCK_LOG_ERROR("Couldn't create listener thread\n");
		return -FI_EINVAL;
	}

	if (!_pep->cm.do_listen && sock_pep_create_listener(_pep)) {
		SOCK_LOG_ERROR("Failed to create pep thread\n");
		return -FI_EINVAL;
	}

	return sock_pep_start_listener_thread(_pep);
}

static int sock_pep_reject(struct fid_pep *pep, fid_t handle,
		const void *param, size_t paramlen)
{
	struct sock_conn_req_handle *hreq;
	struct sock_conn_req *req;
	struct sock_pep *_pep;
	struct sock_ep_cm_head *cm_head;

	_pep = container_of(pep, struct sock_pep, pep);
	hreq = container_of(handle, struct sock_conn_req_handle, handle);
	req = hreq->req;
	if (!req || hreq->handle.fclass != FI_CLASS_CONNREQ ||
	    hreq->state == SOCK_CONN_HANDLE_ACCEPTED)
		return -FI_EINVAL;

	hreq->paramlen = 0;
	if (paramlen) {
		memcpy(hreq->cm_data, param, paramlen);
		hreq->paramlen = paramlen;
	}

	cm_head = &_pep->cm_head;
	hreq->state = SOCK_CONN_HANDLE_REJECTED;
	fastlock_acquire(&cm_head->signal_lock);
	sock_ep_cm_add_to_msg_list(cm_head, hreq);
	fastlock_release(&cm_head->signal_lock);
	return 0;
}

static struct fi_ops_cm sock_pep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = sock_ep_cm_setname,
	.getname = sock_ep_cm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = sock_pep_listen,
	.accept = fi_no_accept,
	.reject = sock_pep_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};


int sock_pep_getopt(fid_t fid, int level, int optname,
		      void *optval, size_t *optlen)
{
	if (level != FI_OPT_ENDPOINT || optname != FI_OPT_CM_DATA_SIZE)
		return -FI_ENOPROTOOPT;

	if (*optlen < sizeof(size_t)) {
		*optlen = sizeof(size_t);
		return -FI_ETOOSMALL;
	}
	*((size_t *) optval) = SOCK_EP_MAX_CM_DATA_SZ;
	*optlen = sizeof(size_t);
	return 0;
}

static struct fi_ops_ep sock_pep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.getopt = sock_pep_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int sock_msg_sep(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **sep, void *context)
{
	int ret;
	struct sock_ep *endpoint;

	ret = sock_alloc_endpoint(domain, info, &endpoint, context, FI_CLASS_SEP);
	if (ret)
		return ret;

	*sep = &endpoint->ep;
	return 0;
}

int sock_msg_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
			struct fid_pep **pep, void *context)
{
	int ret = 0;
	struct sock_pep *_pep;
	struct addrinfo hints, *result;

	assert(info);
	_pep = calloc(1, sizeof(*_pep));
	if (!_pep)
		return -FI_ENOMEM;

	if (info->src_addr) {
		memcpy(&_pep->src_addr, info->src_addr,
			info->src_addrlen);
	} else {
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = ofi_get_sa_family(info);
		if (!hints.ai_family)
			hints.ai_family = AF_INET;

		if (hints.ai_family == AF_INET) {
			ret = getaddrinfo("127.0.0.1", NULL, &hints,
						&result);
		} else if (hints.ai_family == AF_INET6) {
			ret = getaddrinfo("::1", NULL, &hints, &result);
		} else {
			ret = getaddrinfo("localhost", NULL, &hints,
						&result);
		}
		if (ret) {
			ret = -FI_EINVAL;
			SOCK_LOG_DBG("getaddrinfo failed!\n");
			goto err;
		}
		memcpy(&_pep->src_addr, result->ai_addr,
			result->ai_addrlen);
		freeaddrinfo(result);
	}
	_pep->info = *info;

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, _pep->cm.signal_fds);
	if (ret) {
		ret = -ofi_sockerr();
		goto err;
	}

	fd_set_nonblock(_pep->cm.signal_fds[1]);

	_pep->pep.fid.fclass = FI_CLASS_PEP;
	_pep->pep.fid.context = context;
	_pep->pep.fid.ops = &sock_pep_fi_ops;
	_pep->pep.cm = &sock_pep_cm_ops;
	_pep->pep.ops = &sock_pep_ops;

	_pep->sock_fab = container_of(fabric, struct sock_fabric, fab_fid);
	*pep = &_pep->pep;
	return 0;
err:
	free(_pep);
	return ret;
}

static void sock_ep_cm_handle_rx(struct sock_ep_cm_head *cm_head,
                                struct sock_conn_req_handle *handle)
{
	struct sock_conn_hdr hdr;

	if (sock_cm_recv(handle->sock_fd, &hdr, sizeof(hdr))) {
		SOCK_LOG_ERROR("io failed for fd %d\n", handle->sock_fd);
		if (handle->ep) {
			sock_ep_cm_shutdown_report(handle->ep, 0);
		}

		sock_ep_cm_unmonitor_handle_locked(cm_head, handle, 1);
		return;
	}

	switch(hdr.type) {
	case SOCK_CONN_REQ:
		sock_pep_req_handler(cm_head, &hdr, handle);
		break;
	case SOCK_CONN_ACCEPT:
	case SOCK_CONN_REJECT:
		sock_ep_cm_connect_handler(cm_head, &hdr, handle);
		break;
	case SOCK_CONN_SHUTDOWN:
		sock_ep_cm_shutdown_handler(cm_head, &hdr, handle);
		break;
	default:
		SOCK_LOG_ERROR("Unexpected message type %d\n", hdr.type);
		break;
	}
}

static void *sock_ep_cm_thread(void *arg)
{
	int num_fds, i;
	struct sock_ep_cm_head *cm_head = arg;
	void *ep_contexts[SOCK_EPOLL_WAIT_EVENTS];
	struct sock_conn_req_handle *handle;

	while (cm_head->do_listen) {
		sock_ep_cm_check_closing_rejected_list(cm_head);

		num_fds = ofi_epoll_wait(cm_head->emap, ep_contexts,
		                        SOCK_EPOLL_WAIT_EVENTS, -1);
		if (num_fds < 0) {
			SOCK_LOG_ERROR("poll failed : %s\n", strerror(errno));
			continue;
		}

		fastlock_acquire(&cm_head->signal_lock);
		for (i = 0; i < num_fds; i++) {
			handle = ep_contexts[i];

			if (handle == NULL) { /* Signal event */
				fd_signal_reset(&cm_head->signal);
				continue;
			}

			/* ep_contexts[] may report multiple events for the same handle.
			 * Suppose we received 2 elements for 1 handle: the first will
			 * unmonitor the handle, then the second event will have
			 * handle->monitored set to 0
			 */
			if (!handle->monitored) {
				assert(handle->sock_fd == INVALID_SOCKET);
				continue;
			}

			assert(handle->sock_fd != INVALID_SOCKET);
			sock_ep_cm_handle_rx(cm_head, handle);
		}
		fastlock_release(&cm_head->signal_lock);
	}
	return NULL;
}


int sock_ep_cm_start_thread(struct sock_ep_cm_head *cm_head)
{
	assert(cm_head->do_listen == 0);

	fastlock_init(&cm_head->signal_lock);
	dlist_init(&cm_head->msg_list);

	int ret = ofi_epoll_create(&cm_head->emap);
	if (ret < 0) {
		SOCK_LOG_ERROR("failed to create epoll set\n");
		goto err1;
	}

	ret = fd_signal_init(&cm_head->signal);
	if (ret < 0) {
		ret = -errno;
		SOCK_LOG_ERROR("failed to init signal\n");
		goto err2;
	}

	ret = ofi_epoll_add(cm_head->emap,
	                   cm_head->signal.fd[FI_READ_FD],
	                   OFI_EPOLL_IN, NULL);
	if (ret != 0){
		SOCK_LOG_ERROR("failed to add signal fd to epoll\n");
		goto err3;
	}

	cm_head->do_listen = 1;
	ret = pthread_create(&cm_head->listener_thread, 0,
	                     sock_ep_cm_thread, cm_head);
	if (ret) {
		SOCK_LOG_ERROR("failed to create conn listener thread\n");
		goto err3;
	}
	return 0;

err3:
	cm_head->do_listen = 0;
	fd_signal_free(&cm_head->signal);
err2:
	ofi_epoll_close(cm_head->emap);
err1:
	return ret;
}

void sock_ep_cm_wait_handle_finalized(struct sock_ep_cm_head *cm_head,
                                      struct sock_conn_req_handle *handle)
{
	handle->state = SOCK_CONN_HANDLE_FINALIZING;
	fastlock_acquire(&cm_head->signal_lock);
	sock_ep_cm_add_to_msg_list(cm_head, handle);
	fastlock_release(&cm_head->signal_lock);

	pthread_mutex_lock(&handle->finalized_mutex);
	while (handle->state != SOCK_CONN_HANDLE_FINALIZED)
		fi_wait_cond(&handle->finalized_cond,
				&handle->finalized_mutex, -1);
	pthread_mutex_unlock(&handle->finalized_mutex);
}

void sock_ep_cm_stop_thread(struct sock_ep_cm_head *cm_head)
{
	if (cm_head->do_listen == 0)
		return;

	cm_head->do_listen = 0;

	sock_ep_cm_signal(cm_head);

	if (cm_head->listener_thread &&
			pthread_join(cm_head->listener_thread, NULL)) {
		SOCK_LOG_DBG("pthread join failed\n");
	}
	ofi_epoll_close(cm_head->emap);
	fd_signal_free(&cm_head->signal);
	fastlock_destroy(&cm_head->signal_lock);
}
