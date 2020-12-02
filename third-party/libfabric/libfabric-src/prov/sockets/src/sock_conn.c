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

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <poll.h>
#include <limits.h>

#include "sock.h"
#include "sock_util.h"
#include "ofi_file.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_EP_CTRL, __VA_ARGS__)

ssize_t sock_conn_send_src_addr(struct sock_ep_attr *ep_attr, struct sock_tx_ctx *tx_ctx,
				struct sock_conn *conn)
{
	int ret;
	uint64_t total_len;
	struct sock_op tx_op = { 0 };

	tx_op.op = SOCK_OP_CONN_MSG;
	SOCK_LOG_DBG("New conn msg on TX: %p using conn: %p\n", tx_ctx, conn);

	total_len = 0;
	tx_op.src_iov_len = sizeof(union ofi_sock_ip);
	total_len = tx_op.src_iov_len + sizeof(struct sock_op_send);

	sock_tx_ctx_start(tx_ctx);
	if (ofi_rbavail(&tx_ctx->rb) < total_len) {
		ret = -FI_EAGAIN;
		goto err;
	}

	sock_tx_ctx_write_op_send(tx_ctx, &tx_op, 0, (uintptr_t) NULL, 0, 0,
				   ep_attr, conn);

	ofi_straddr_dbg(&sock_prov, FI_LOG_EP_CTRL, "sending src addr: ",
			ep_attr->src_addr);
	sock_tx_ctx_write(tx_ctx, ep_attr->src_addr, sizeof(union ofi_sock_ip));
	sock_tx_ctx_commit(tx_ctx);
	conn->address_published = 1;
	return 0;

err:
	sock_tx_ctx_abort(tx_ctx);
	return ret;
}

int sock_conn_map_init(struct sock_ep *ep, int init_size)
{
	struct sock_conn_map *map = &ep->attr->cmap;
	int ret;
	map->table = calloc(init_size, sizeof(*map->table));
	if (!map->table)
		return -FI_ENOMEM;

	map->epoll_ctxs = calloc(init_size, sizeof(*map->epoll_ctxs));
	if (!map->epoll_ctxs)
		goto err1;

	ret = ofi_epoll_create(&map->epoll_set);
	if (ret < 0) {
		SOCK_LOG_ERROR("failed to create epoll set, "
			       "error - %d (%s)\n", ret,
			       strerror(ret));
		goto err2;
	}

	fastlock_init(&map->lock);
	map->used = 0;
	map->size = init_size;
	return 0;

err2:
	free(map->epoll_ctxs);
err1:
	free(map->table);
	return -FI_ENOMEM;
}

static int sock_conn_map_increase(struct sock_conn_map *map, int new_size)
{
	void *_table;

	_table = realloc(map->table, new_size * sizeof(*map->table));
	if (!_table) {
		SOCK_LOG_ERROR("*** realloc failed, use FI_SOCKETS_DEF_CONN_MAP_SZ for"
			"specifying conn-map-size\n");
		return -FI_ENOMEM;
	}

	map->size = new_size;
	map->table = _table;
	return 0;
}

void sock_conn_map_destroy(struct sock_ep_attr *ep_attr)
{
	int i;
	struct sock_conn_map *cmap = &ep_attr->cmap;
	for (i = 0; i < cmap->used; i++) {
		if (cmap->table[i].sock_fd != -1) {
			sock_pe_poll_del(ep_attr->domain->pe, cmap->table[i].sock_fd);
			sock_conn_release_entry(cmap, &cmap->table[i]);
		}
	}
	free(cmap->table);
	cmap->table = NULL;
	free(cmap->epoll_ctxs);
	cmap->epoll_ctxs = NULL;
	cmap->epoll_ctxs_sz = 0;
	cmap->used = cmap->size = 0;
	ofi_epoll_close(cmap->epoll_set);
	fastlock_destroy(&cmap->lock);
}

void sock_conn_release_entry(struct sock_conn_map *map, struct sock_conn *conn)
{
	ofi_epoll_del(map->epoll_set, conn->sock_fd);
	ofi_close_socket(conn->sock_fd);

	conn->address_published = 0;
        conn->connected = 0;
        conn->sock_fd = -1;
}

static int sock_conn_get_next_index(struct sock_conn_map *map)
{
	int i;
	for (i = 0; i < map->size; i++) {
		if (map->table[i].sock_fd == -1)
			return i;
	}
	return -1;
}

static struct sock_conn *sock_conn_map_insert(struct sock_ep_attr *ep_attr,
				union ofi_sock_ip *addr, int conn_fd,
				int addr_published)
{
	int index;
	struct sock_conn_map *map = &ep_attr->cmap;

	if (map->size == map->used) {
		index = sock_conn_get_next_index(map);
		if (index < 0) {
			if (sock_conn_map_increase(map, map->size * 2))
				return NULL;
			index = map->used;
			map->used++;
		}
	} else {
		index = map->used;
		map->used++;
	}

	map->table[index].av_index = FI_ADDR_NOTAVAIL;
	map->table[index].connected = 1;
	map->table[index].addr = *addr;
	map->table[index].sock_fd = conn_fd;
	map->table[index].ep_attr = ep_attr;
	sock_set_sockopts(conn_fd, SOCK_OPTS_NONBLOCK |
	                  (ep_attr->ep_type == FI_EP_MSG ?
	                   SOCK_OPTS_KEEPALIVE : 0));

	if (ofi_epoll_add(map->epoll_set, conn_fd, OFI_EPOLL_IN, &map->table[index]))
		SOCK_LOG_ERROR("failed to add to epoll set: %d\n", conn_fd);

	map->table[index].address_published = addr_published;
	sock_pe_poll_add(ep_attr->domain->pe, conn_fd);
	return &map->table[index];
}

int fd_set_nonblock(int fd)
{
	int ret;

	ret = fi_fd_nonblock(fd);
	if (ret)
	    SOCK_LOG_ERROR("fi_fd_nonblock failed, errno: %d\n",
			   ret);

	return ret;
}

#if !defined __APPLE__ && !defined _WIN32
void sock_set_sockopt_keepalive(int sock)
{
	int optval;

	/* Keepalive is disabled: now leave */
	if (!sock_keepalive_enable)
		return;

	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)))
			SOCK_LOG_ERROR("setsockopt keepalive enable failed: %s\n",
			               strerror(errno));

	if (sock_keepalive_time != INT_MAX) {
		optval = sock_keepalive_time;
		if (setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, &optval, sizeof(optval)))
			SOCK_LOG_ERROR("setsockopt keepalive time failed: %s\n",
			               strerror(errno));
	}

	if (sock_keepalive_intvl != INT_MAX) {
		optval = sock_keepalive_intvl;
		if (setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, &optval, sizeof(optval)))
			SOCK_LOG_ERROR("setsockopt keepalive intvl failed: %s\n",
			               strerror(errno));
	}

	if (sock_keepalive_probes != INT_MAX) {
		optval = sock_keepalive_probes;
		if (setsockopt(sock, SOL_TCP, TCP_KEEPCNT, &optval, sizeof(optval)))
			SOCK_LOG_ERROR("setsockopt keepalive intvl failed: %s\n",
			               strerror(errno));
	}
}
#else
#define sock_set_sockopt_keepalive(sock) do {} while (0)
#endif

static void sock_set_sockopt_reuseaddr(int sock)
{
	int optval;
	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
		SOCK_LOG_ERROR("setsockopt reuseaddr failed\n");
}

void sock_set_sockopts(int sock, int sock_opts)
{
	int optval;
	optval = 1;

	sock_set_sockopt_reuseaddr(sock);
	if (sock_opts & SOCK_OPTS_KEEPALIVE)
		sock_set_sockopt_keepalive(sock);
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)))
		SOCK_LOG_ERROR("setsockopt nodelay failed\n");

	if (sock_opts & SOCK_OPTS_NONBLOCK)
		fd_set_nonblock(sock);
}

int sock_conn_stop_listener_thread(struct sock_conn_listener *conn_listener)
{
	conn_listener->do_listen = 0;

	fastlock_acquire(&conn_listener->signal_lock);
	fd_signal_set(&conn_listener->signal);
	fastlock_release(&conn_listener->signal_lock);

	if (conn_listener->listener_thread &&
	    pthread_join(conn_listener->listener_thread, NULL)) {
		SOCK_LOG_DBG("pthread join failed\n");
	}

	fd_signal_free(&conn_listener->signal);
	ofi_epoll_close(conn_listener->emap);
	fastlock_destroy(&conn_listener->signal_lock);

	return 0;
}

static void *sock_conn_listener_thread(void *arg)
{
	struct sock_conn_listener *conn_listener = arg;
	struct sock_conn_handle *conn_handle;
	void *ep_contexts[SOCK_EPOLL_WAIT_EVENTS];
	struct sock_ep_attr *ep_attr;
	int num_fds, i, conn_fd;
	union ofi_sock_ip remote;
	socklen_t addr_size;

	while (conn_listener->do_listen) {
		num_fds = ofi_epoll_wait(conn_listener->emap, ep_contexts,
		                        SOCK_EPOLL_WAIT_EVENTS, -1);
		if (num_fds < 0) {
			SOCK_LOG_ERROR("poll failed : %s\n", strerror(errno));
			continue;
		}

		fastlock_acquire(&conn_listener->signal_lock);
		for (i = 0; i < num_fds; i++) {
			conn_handle = ep_contexts[i];

			if (conn_handle == NULL) { /* signal event */
				fd_signal_reset(&conn_listener->signal);
				continue;
			}

			memset(&remote, 0, sizeof remote);
			addr_size = sizeof(remote);
			conn_fd = accept(conn_handle->sock, &remote.sa,
			                 &addr_size);
			SOCK_LOG_DBG("CONN: accepted conn-req: %d\n", conn_fd);
			ofi_straddr_dbg(&sock_prov, FI_LOG_EP_CTRL,
					"accepted peer addr: ", &remote.sa);

			if (conn_fd < 0) {
				SOCK_LOG_ERROR("failed to accept: %s\n",
				               strerror(ofi_sockerr()));
				continue;
			}

			ep_attr = container_of(conn_handle, struct sock_ep_attr, conn_handle);
			fastlock_acquire(&ep_attr->cmap.lock);
			sock_conn_map_insert(ep_attr, &remote, conn_fd, 1);
			fastlock_release(&ep_attr->cmap.lock);
			sock_pe_signal(ep_attr->domain->pe);
		}
		fastlock_release(&conn_listener->signal_lock);
	}

	return NULL;
}

int sock_conn_start_listener_thread(struct sock_conn_listener *conn_listener)
{
	int ret;

	fastlock_init(&conn_listener->signal_lock);

	ret = ofi_epoll_create(&conn_listener->emap);
	if (ret < 0) {
		SOCK_LOG_ERROR("failed to create epoll set\n");
		goto err1;
	}

	ret = fd_signal_init(&conn_listener->signal);
	if (ret < 0) {
		SOCK_LOG_ERROR("failed to init signal\n");
		goto err2;
	}

	ret = ofi_epoll_add(conn_listener->emap,
	                   conn_listener->signal.fd[FI_READ_FD],
	                   OFI_EPOLL_IN, NULL);
	if (ret != 0){
		SOCK_LOG_ERROR("failed to add signal fd to epoll\n");
		goto err3;
	}

	conn_listener->do_listen = 1;
	ret = pthread_create(&conn_listener->listener_thread, NULL,
	                     sock_conn_listener_thread, conn_listener);
	if (ret < 0) {
		SOCK_LOG_ERROR("failed to create conn listener thread\n");
		goto err3;
	}
	return 0;

err3:
	conn_listener->do_listen = 0;
	fd_signal_free(&conn_listener->signal);
err2:
	ofi_epoll_close(conn_listener->emap);
err1:
	fastlock_destroy(&conn_listener->signal_lock);
	return ret;
}

int sock_conn_listen(struct sock_ep_attr *ep_attr)
{
	int listen_fd, ret;
	socklen_t addr_size;
	union ofi_sock_ip addr;
	struct sock_conn_handle *conn_handle = &ep_attr->conn_handle;

	listen_fd = ofi_socket(ep_attr->src_addr->sa.sa_family,
				SOCK_STREAM, IPPROTO_TCP);
	if (listen_fd == INVALID_SOCKET)
		return -ofi_sockerr();

	sock_set_sockopts(listen_fd, SOCK_OPTS_NONBLOCK);

	addr = *ep_attr->src_addr;
	if (ep_attr->ep_type == FI_EP_MSG)
		ofi_addr_set_port(&addr.sa, 0);

	ret = bind(listen_fd, &addr.sa, ofi_sizeofaddr(&addr.sa));
	if (ret) {
		SOCK_LOG_ERROR("failed to bind listener: %s\n",
			       strerror(ofi_sockerr()));
		ofi_straddr_log(&sock_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
				"bind failed to addr: ", &addr.sa);
		ret = -ofi_sockerr();
		goto err;
	}

	addr_size = sizeof(addr);
	ret = ofi_getsockname(listen_fd, &addr.sa, &addr_size);
	if (ret) {
		ret = -ofi_sockerr();
		goto err;
	}

	ep_attr->msg_src_port = ofi_addr_get_port(&addr.sa);
	if (!ofi_addr_get_port(&ep_attr->src_addr->sa))
		ofi_addr_set_port(&ep_attr->src_addr->sa, ep_attr->msg_src_port);

	ofi_straddr_dbg(&sock_prov, FI_LOG_EP_CTRL, "listening at addr: ",
			&addr.sa);
	ret = listen(listen_fd, sock_cm_def_map_sz);
	if (ret) {
		SOCK_LOG_ERROR("failed to listen socket: %s\n",
			       strerror(ofi_sockerr()));
		ret = -ofi_sockerr();
		goto err;
	}

	conn_handle->sock = listen_fd;
	conn_handle->do_listen = 1;

	fastlock_acquire(&ep_attr->domain->conn_listener.signal_lock);
	ret = ofi_epoll_add(ep_attr->domain->conn_listener.emap,
	                   conn_handle->sock, OFI_EPOLL_IN, conn_handle);
	fd_signal_set(&ep_attr->domain->conn_listener.signal);
	fastlock_release(&ep_attr->domain->conn_listener.signal_lock);
	if (ret) {
		SOCK_LOG_ERROR("failed to add fd to pollset: %d\n", ret);
		goto err;
	}

	return 0;
err:
	if (listen_fd != INVALID_SOCKET) {
		ofi_close_socket(listen_fd);
		conn_handle->sock = INVALID_SOCKET;
		conn_handle->do_listen = 0;
	}

	return ret;
}

int sock_ep_connect(struct sock_ep_attr *ep_attr, fi_addr_t index,
		    struct sock_conn **conn)
{
	int conn_fd = -1, ret;
	int do_retry = sock_conn_retry;
	struct sock_conn *new_conn;
	union ofi_sock_ip addr;
	socklen_t lon;
	int valopt = 0;
	struct pollfd poll_fd;

	if (ep_attr->ep_type == FI_EP_MSG) {
		/* Need to check that destination address has been
		   passed to endpoint */
		assert(ep_attr->dest_addr);
		addr = *ep_attr->dest_addr;
		ofi_addr_set_port(&addr.sa, ep_attr->msg_dest_port);
	} else {
		fastlock_acquire(&ep_attr->av->table_lock);
		addr = ep_attr->av->table[index].addr;
		fastlock_release(&ep_attr->av->table_lock);
	}

do_connect:
	fastlock_acquire(&ep_attr->cmap.lock);
	*conn = sock_ep_lookup_conn(ep_attr, index, &addr);
	fastlock_release(&ep_attr->cmap.lock);

	if (*conn != SOCK_CM_CONN_IN_PROGRESS)
		return FI_SUCCESS;

	conn_fd = ofi_socket(addr.sa.sa_family, SOCK_STREAM, 0);
	if (conn_fd == -1) {
		SOCK_LOG_ERROR("failed to create conn_fd, errno: %d\n",
			       ofi_sockerr());
		*conn = NULL;
		return -FI_EOTHER;
	}

	ret = fd_set_nonblock(conn_fd);
	if (ret) {
		SOCK_LOG_ERROR("failed to set conn_fd nonblocking\n");
		*conn = NULL;
		ofi_close_socket(conn_fd);
		return -FI_EOTHER;
	}

	ofi_straddr_dbg(&sock_prov, FI_LOG_EP_CTRL, "connecting to addr: ",
			&addr.sa);
	ret = connect(conn_fd, &addr.sa, ofi_sizeofaddr(&addr.sa));
	if (ret < 0) {
		if (OFI_SOCK_TRY_CONN_AGAIN(ofi_sockerr())) {
			poll_fd.fd = conn_fd;
			poll_fd.events = POLLOUT;

			ret = poll(&poll_fd, 1, sock_conn_timeout);
			if (ret < 0) {
				SOCK_LOG_DBG("poll failed\n");
				goto retry;
			}

			lon = sizeof(int);
			ret = getsockopt(conn_fd, SOL_SOCKET, SO_ERROR,
					 (void*)(&valopt), &lon);
			if (ret < 0) {
				SOCK_LOG_DBG("getsockopt failed: %d, %d\n",
					     ret, conn_fd);
				goto retry;
			}

			if (valopt) {
				SOCK_LOG_DBG("Error in connection() "
					     "%d - %s - %d\n",
					     valopt, strerror(valopt), conn_fd);
				goto retry;
			}
			goto out;
		} else {
			SOCK_LOG_DBG("Timeout or error() - %s: %d\n",
				     strerror(ofi_sockerr()), conn_fd);
			goto retry;
		}
	} else {
		goto out;
	}

retry:
	do_retry--;
	if (!do_retry)
		goto err;

	if (conn_fd != -1) {
		ofi_close_socket(conn_fd);
		conn_fd = -1;
	}

	SOCK_LOG_ERROR("Connect error, retrying - %s - %d\n",
		       strerror(ofi_sockerr()), conn_fd);
        goto do_connect;

out:
	fastlock_acquire(&ep_attr->cmap.lock);
	new_conn = sock_conn_map_insert(ep_attr, &addr, conn_fd, 0);
	if (!new_conn) {
		fastlock_release(&ep_attr->cmap.lock);
		goto err;
	}
	new_conn->av_index = (ep_attr->ep_type == FI_EP_MSG) ?
			     FI_ADDR_NOTAVAIL : index;
	*conn = ofi_idm_lookup(&ep_attr->av_idm, index);
	if (*conn == SOCK_CM_CONN_IN_PROGRESS) {
		if (ofi_idm_set(&ep_attr->av_idm, index, new_conn) < 0)
			SOCK_LOG_ERROR("ofi_idm_set failed\n");
		*conn = new_conn;
	}
	fastlock_release(&ep_attr->cmap.lock);
	return FI_SUCCESS;

err:
	ofi_close_socket(conn_fd);
	*conn = NULL;
	return (OFI_SOCK_TRY_CONN_AGAIN(ofi_sockerr()) ? -FI_EAGAIN :
							 -ofi_sockerr());
}
