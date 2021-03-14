/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
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

/*
 * A name server is started on each node as a thread within one of
 * the processes on that node. It maintains a database that maps
 * "services" to "endpoint names". Other processes on the same node
 * talk to this name server to update mapping information.
 *
 * To resolve a "node:service" pair into an provider internal endpoint name
 * that can be used as the input of fi_av_insert, a process needs to make
 * a query to the name server residing on "node".
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <ofi_util.h>
#include <rdma/providers/fi_log.h>
#include <ofi.h>

#include "rbtree.h"

#define OFI_NS_DEFAULT_HOSTNAME	"localhost"


enum {
	OFI_UTIL_NS_ADD,
	OFI_UTIL_NS_DEL,
	OFI_UTIL_NS_QUERY,
	OFI_UTIL_NS_ACK,
};

#define OFI_NS_VERSION 0

/* Send and received in network-byte order */
struct util_ns_cmd {
	uint8_t		version;
	uint8_t		op;
	uint16_t	reserved; /* TODO: this should be the msg len */
	uint32_t	status;
};

const size_t cmd_len = sizeof(struct util_ns_cmd);


static int util_ns_map_add(struct util_ns *ns, void *service_in,
			   void *name_in)
{
	void *name, *service;
	int ret;

	service = calloc(ns->service_len, 1);
	if (!service) {
		ret = -FI_ENOMEM;
		goto err1;
	}
	memcpy(service, service_in, ns->service_len);

	name = calloc(ns->name_len, 1);
	if (!name) {
		ret = -FI_ENOMEM;
		goto err2;
	}
	memcpy(name, name_in, ns->name_len);

	if (rbtFind(ns->map, service)) {
		ret = -FI_EADDRINUSE;
		goto err3;
	}

	if (rbtInsert(ns->map, service, name)) {
		ret = -FI_ENOMEM;
		goto err3;
	}
	return FI_SUCCESS;

err3:
	free(name);
err2:
	free(service);
err1:
	return ret;
}
 
static int util_ns_map_del(struct util_ns *ns, void *service_in,
			   void *name_in)
{
	RbtIterator it;
	int ret = -FI_ENOENT;
	void *service, *name;

        it = rbtFind(ns->map, service_in);
        if (it) {
		rbtKeyValue(ns->map, it, &service, &name);
		if (memcmp(name, name_in, ns->name_len))
			return ret;
		free(service);
		free(name);
		rbtErase(ns->map, it);
		ret = FI_SUCCESS;
	}

	return ret;
}

static int util_ns_map_lookup(struct util_ns *ns, void *service_in,
			      void *name_out)
{
	RbtIterator it;
	void *key, *name;

        it = rbtFind(ns->map, service_in);
	if (!it)
		return -FI_ENOENT;

	rbtKeyValue(ns->map, it, &key, (void **)&name);
	memcpy(name_out, name, ns->name_len);

	if (ns->is_service_wildcard && ns->is_service_wildcard(service_in))
		memcpy(service_in, key, ns->service_len);

	return FI_SUCCESS;
}

static int util_ns_process_cmd(struct util_ns *ns, struct util_ns_cmd *cmd,
			       SOCKET sock)
{
	int ret = FI_SUCCESS;
	size_t io_len = 0;
	void *io_buf, *service, *name;

	if (cmd->version != OFI_NS_VERSION)
		return -FI_ENOSYS;

	switch (cmd->op) {
	case OFI_UTIL_NS_ADD:
	case OFI_UTIL_NS_DEL:
		io_len = ns->name_len + ns->service_len;
		io_buf = calloc(io_len, 1);
		if (!io_buf) {
			ret = -FI_ENOMEM;
			goto out;
		}

		ret = ofi_recvall_socket(sock, io_buf, io_len);
		if (!ret) {
			service = io_buf;
			name = (char *) io_buf + ns->service_len;
			ret = (cmd->op == OFI_UTIL_NS_ADD) ?
			      util_ns_map_add(ns, service, name) :
			      util_ns_map_del(ns, service, name);
		} else {
			ret = -FI_ENODATA;
		}
		break;
	case OFI_UTIL_NS_QUERY:
		io_len = ns->service_len;
		io_buf = calloc(cmd_len + ns->service_len + ns->name_len, 1);
		if (!io_buf) {
			ret = -FI_ENOMEM;
			goto out;
		}

		memcpy(io_buf, cmd, cmd_len);
		cmd = io_buf;
		service = (char *) io_buf + cmd_len;
		name = (char *) service + ns->service_len;

		ret = ofi_recvall_socket(sock, service, io_len);
		if (!ret) {
			cmd->op = OFI_UTIL_NS_ACK;
			cmd->status = htonl(util_ns_map_lookup(ns, service, name));
		} else {
			ret = -FI_ENODATA;
			break;
		}

		if (!cmd->status)
			io_len = cmd_len + ns->service_len + ns->name_len;
		else
			io_len = cmd_len;
		ret = ofi_sendall_socket(sock, io_buf, io_len) ?
		      -FI_ENODATA : FI_SUCCESS;
		break;

	default:
		assert(0);
		ret = -FI_ENODATA;
		goto out;
	}

	free(io_buf);

out:
	FI_INFO(&core_prov, FI_LOG_CORE,
		"Name server processed command - returned %d (%s)\n", ret, fi_strerror(-ret));
	return ret;
}

static void util_ns_close_listen(struct util_ns *ns)
{
	ofi_close_socket(ns->listen_sock);
	ns->listen_sock = INVALID_SOCKET;
}

/*
 * We only start one name server among all peer processes.  We rely
 * on getting an ADDRINUSE error on either bind or listen if another
 * process has started the name server.
 */
static int util_ns_listen(struct util_ns *ns)
{
	struct addrinfo hints = {
		.ai_flags = AI_PASSIVE,
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_STREAM
	};
	struct addrinfo *res, *p;
	char *service;
	int n = 1, ret;

	if (asprintf(&service, "%d", ns->port) < 0)
		return -FI_ENOMEM;

	ret = getaddrinfo(NULL, service, &hints, &res);
	free(service);
	if (ret)
		return -FI_EADDRNOTAVAIL;

	for (p = res; p; p = p->ai_next) {
		ns->listen_sock = ofi_socket(p->ai_family, p->ai_socktype,
					     p->ai_protocol);
		if (ns->listen_sock == INVALID_SOCKET)
			continue;

		(void) setsockopt(ns->listen_sock, SOL_SOCKET, SO_REUSEADDR,
				  (void *) &n, sizeof(n));
		ret = bind(ns->listen_sock, p->ai_addr, (socklen_t) p->ai_addrlen);
		if (!ret)
			break;

		ret = errno;
		util_ns_close_listen(ns);
		if (ret == EADDRINUSE) {
			freeaddrinfo(res);
			return -ret;
		}
	}

	freeaddrinfo(res);

	if (ns->listen_sock == INVALID_SOCKET)
		return -FI_EADDRNOTAVAIL;

	ret = listen(ns->listen_sock, 256);
	if (ret) {
		util_ns_close_listen(ns);
		ret = -errno;
	}

	return ret;
}

static void *util_ns_accept_handler(void *args)
{
	struct util_ns *ns = args;
	SOCKET conn_sock;
	struct util_ns_cmd cmd;
	int ret;

	while (ns->run) {
		conn_sock = accept(ns->listen_sock, NULL, 0);
		if (conn_sock == INVALID_SOCKET)
			break;

		ret = ofi_recvall_socket(conn_sock, &cmd, cmd_len);
		if (!ret)
			util_ns_process_cmd(ns, &cmd, conn_sock);

		ofi_close_socket(conn_sock);
	}

	return NULL;
}

/*
 * Name server API: client side
 */

static SOCKET util_ns_connect_server(struct util_ns *ns, const char *server)
{
	struct addrinfo hints = {
		.ai_family   = AF_UNSPEC,
		.ai_socktype = SOCK_STREAM
	};
	struct addrinfo *res, *p;
	char *service;
	SOCKET sockfd = INVALID_SOCKET;
	int n;

	if (asprintf(&service, "%d", ns->port) < 0)
		return INVALID_SOCKET;

	n = getaddrinfo(server, service, &hints, &res);
	if (n < 0) {
		free(service);
		return INVALID_SOCKET;
	}

	for (p = res; p; p = p->ai_next) {
		sockfd = ofi_socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd != INVALID_SOCKET) {
			if (!connect(sockfd, p->ai_addr, (socklen_t) p->ai_addrlen))
				break;
			ofi_close_socket(sockfd);
			sockfd = INVALID_SOCKET;
		}
	}

	freeaddrinfo(res);
	free(service);

	return sockfd;
}

int ofi_ns_add_local_name(struct util_ns *ns, void *service, void *name)
{
	SOCKET sockfd;
	int ret;
	void *write_buf;
	size_t write_len = 0;
	struct util_ns_cmd cmd = {
		.version = OFI_NS_VERSION,
		.op = OFI_UTIL_NS_ADD,
	};

	if (!ns->is_initialized) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Cannot add local name - name server uninitialized\n");
		return -FI_EINVAL;
	}

	write_buf = calloc(cmd_len + ns->service_len + ns->name_len, 1);
	if (!write_buf) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	memcpy(write_buf, &cmd, cmd_len);
	write_len += cmd_len;
	memcpy((void *)((char *)write_buf + write_len), service,
	       ns->service_len);
	write_len += ns->service_len;
	memcpy((void *)((char *)write_buf + write_len), name,
	       ns->name_len);
	write_len += ns->name_len;

	sockfd = util_ns_connect_server(ns, ns->hostname);
	if (sockfd == INVALID_SOCKET) {
		ret = -FI_ENODATA;
		goto err2;
	}

	ret = ofi_sendall_socket(sockfd, write_buf, write_len) ?
	      -FI_ENODATA : FI_SUCCESS;

	ofi_close_socket(sockfd);
err2:
	free(write_buf);
err1:
	return ret;
}

int ofi_ns_del_local_name(struct util_ns *ns, void *service, void *name)
{
	SOCKET sockfd;
	int ret;
	void *write_buf;
	size_t write_len = 0;
	struct util_ns_cmd cmd = {
		.version = OFI_NS_VERSION,
		.op = OFI_UTIL_NS_DEL
	};

	if (!ns->is_initialized)
		return -FI_EINVAL;

	write_buf = calloc(cmd_len + ns->service_len + ns->name_len, 1);
	if (!write_buf) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	memcpy(write_buf, &cmd, cmd_len);
	write_len += cmd_len;
	memcpy((void *)((char *)write_buf + write_len), service,
	       ns->service_len);
	write_len += ns->service_len;
	memcpy((void *)((char *)write_buf + write_len), name,
	       ns->name_len);
	write_len += ns->name_len;

	sockfd = util_ns_connect_server(ns, ns->hostname);
	if (sockfd == INVALID_SOCKET) {
		ret = -FI_ENODATA;
		goto err2;
	}

	ret = ofi_sendall_socket(sockfd, write_buf, write_len) ?
	      -FI_ENODATA : FI_SUCCESS;

	ofi_close_socket(sockfd);
err2:
	free(write_buf);
err1:
	return ret;
}

void *ofi_ns_resolve_name(struct util_ns *ns, const char *server, void *service)
{
	void *dest_addr = NULL, *io_buf;
	size_t io_len = 0;
	SOCKET sockfd;
	ssize_t ret = 0;
	struct util_ns_cmd cmd = {
		.version = OFI_NS_VERSION,
		.op = OFI_UTIL_NS_QUERY
	};

	if (!ns->is_initialized)
		return NULL;

	sockfd = util_ns_connect_server(ns, server);
	if (sockfd == INVALID_SOCKET)
		goto err1;

	io_buf = calloc(cmd_len + ns->service_len, 1);
	if (!io_buf)
		goto err2;

	memcpy(io_buf, &cmd, cmd_len);
	io_len += cmd_len;
	memcpy((void *)((char *)io_buf + io_len), service,
	       ns->service_len);
	io_len += ns->service_len;

	ret = ofi_sendall_socket(sockfd, io_buf, io_len);
	if (ret)
		goto err3;

	free(io_buf);

	io_len = ns->service_len + ns->name_len;
	io_buf = calloc(io_len, 1);
	if (!io_buf)
		goto err2;

	ret = ofi_recvall_socket(sockfd, &cmd, cmd_len);
	if (ret || cmd.status)
		goto err3;

	ret = ofi_recvall_socket(sockfd, io_buf, io_len);
	if (!ret) {
		dest_addr = calloc(ns->name_len, 1);
		if (!dest_addr)
			goto err3;
		io_len = 0;
		memcpy(service, (char *) io_buf + io_len, ns->service_len);
		io_len += ns->service_len;
		memcpy(dest_addr, (char *) io_buf + io_len, ns->name_len);
	}

err3:
	free(io_buf);
err2:
	ofi_close_socket(sockfd);
err1:
	return dest_addr;
}

/*
 * Name server API: server side
 */

int ofi_ns_start_server(struct util_ns *ns)
{
	int ret;

	assert(ns->is_initialized);
	if (ofi_atomic_inc32(&ns->ref) > 1)
		return 0;

	ns->map = rbtNew(ns->service_cmp);
	if (!ns->map) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	ret = util_ns_listen(ns);
	if (ret) {
		/* EADDRINUSE likely indicates a peer is running the NS */
		if (ret == -FI_EADDRINUSE) {
			rbtDelete(ns->map);
			return 0;
		}
		goto err2;
	}

	ns->run = 1;
	ret = -pthread_create(&ns->thread, NULL,
			      util_ns_accept_handler, (void *) ns);
	if (ret)
		goto err3;

	return 0;

err3:
	ns->run = 0;
	util_ns_close_listen(ns);
err2:
	rbtDelete(ns->map);
err1:
	FI_WARN(&core_prov, FI_LOG_CORE, "Error starting name server\n");
	ofi_atomic_dec32(&ns->ref);
	return ret;
}

void ofi_ns_stop_server(struct util_ns *ns)
{
	SOCKET sock;

	assert(ns->is_initialized);

	if (ofi_atomic_dec32(&ns->ref))
		return;

	if (ns->listen_sock == INVALID_SOCKET)
		return;

	ns->run = 0;
	sock = util_ns_connect_server(ns, ns->hostname);
	if (sock != INVALID_SOCKET)
		ofi_close_socket(sock);
	util_ns_close_listen(ns);
	(void) pthread_join(ns->thread, NULL);
	rbtDelete(ns->map);
}

/* TODO: This isn't thread safe -- neither are start/stop */
void ofi_ns_init(struct util_ns *ns)
{
	assert(ns && ns->name_len && ns->service_len && ns->service_cmp);

	if (ns->is_initialized)
		return;

	ofi_atomic_initialize32(&ns->ref, 0);
	ns->listen_sock = INVALID_SOCKET;
	if (!ns->hostname)
		ns->hostname = OFI_NS_DEFAULT_HOSTNAME;
	ns->is_initialized = 1;
}
