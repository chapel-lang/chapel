/*
 * Copyright (c) 2017-2022 Intel Corporation. All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#include <ofi_prov.h>
#include <ofi_iov.h>
#include "xnet.h"
#include <errno.h>


static int xnet_pep_close(struct fid *fid)
{
	struct xnet_pep *pep;
	int ret;

	pep = container_of(fid, struct xnet_pep, util_pep.pep_fid.fid);
	/* TODO: We need to abort any outstanding active connection requests.
	 * The xnet_conn_handle points back to the pep and will dereference
	 * the freed memory if we continue.
	 */

	if (pep->state == XNET_LISTENING) {
		ofi_genlock_lock(&pep->progress->ep_lock);
		if (xnet_io_uring) {
			ret = xnet_uring_cancel(pep->progress,
						&pep->progress->rx_uring,
						&pep->pollin_sockctx,
						&pep->util_pep.pep_fid);
		} else {
			xnet_halt_sock(pep->progress, pep->sock);
			ret = 0;
		}
		ofi_genlock_unlock(&pep->progress->ep_lock);
		if (ret)
			return ret;
	}

	ofi_close_socket(pep->sock);
	ofi_pep_close(&pep->util_pep);
	fi_freeinfo(pep->info);
	free(pep);
	return 0;
}

static int xnet_pep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct xnet_pep *pep;

	pep = container_of(fid, struct xnet_pep, util_pep.pep_fid.fid);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		return ofi_pep_bind_eq(&pep->util_pep,
				       container_of(bfid, struct util_eq,
						    eq_fid.fid), flags);
	default:
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"invalid FID class for binding\n");
		return -FI_EINVAL;
	}
}

static struct fi_ops xnet_pep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = xnet_pep_close,
	.bind = xnet_pep_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int xnet_bind_to_port_range(SOCKET sock, void* src_addr, size_t addrlen)
{
	int ret, i, rand_port_number;
	static uint32_t seed;
	if (!seed)
		seed = ofi_generate_seed();

	rand_port_number = ofi_xorshift_random_r(&seed) %
			   (xnet_ports.high + 1 - xnet_ports.low) +
			   xnet_ports.low;

	for (i = xnet_ports.low; i <= xnet_ports.high; i++, rand_port_number++) {
		if (rand_port_number > xnet_ports.high)
			rand_port_number = xnet_ports.low;

		ofi_addr_set_port(src_addr, (uint16_t) rand_port_number);
		ret = bind(sock, src_addr, (socklen_t) addrlen);
		if (ret) {
			if (ofi_sockerr() == EADDRINUSE)
				continue;

			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"failed to bind listener: %s\n",
				strerror(ofi_sockerr()));
			return -ofi_sockerr();
		}
		break;
	}
	return (i <= xnet_ports.high) ? FI_SUCCESS : -FI_EADDRNOTAVAIL;
}

static int xnet_pep_sock_create(struct xnet_pep *pep)
{
	int ret, af;

	switch (pep->info->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
	case FI_SOCKADDR_IN6:
		af = ((struct sockaddr *)pep->info->src_addr)->sa_family;
		break;
	default:
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"invalid source address format\n");
		return -FI_EINVAL;
	}

	pep->sock = ofi_socket(af, SOCK_STREAM, 0);
	if (pep->sock == INVALID_SOCKET) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"failed to create listener: %s\n",
			strerror(ofi_sockerr()));
		return -FI_EIO;
	}
	ret = xnet_setup_socket(pep->sock, pep->info);
	if (ret)
		goto err;

	if (!xnet_io_uring)
		xnet_set_zerocopy(pep->sock);
	ret = fi_fd_nonblock(pep->sock);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"failed to set listener socket to nonblocking\n");
		goto err;
	}

	if (ofi_addr_get_port(pep->info->src_addr) != 0 || xnet_ports.high == 0) {
		ret = bind(pep->sock, pep->info->src_addr,
			  (socklen_t) pep->info->src_addrlen);
		if (ret)
			ret = -ofi_sockerr();
	} else {
		ret = xnet_bind_to_port_range(pep->sock, pep->info->src_addr,
					      pep->info->src_addrlen);
	}

	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"failed to bind listener: %s\n",
			strerror(ofi_sockerr()));
		goto err;
	}
	return FI_SUCCESS;
err:
	ofi_close_socket(pep->sock);
	pep->sock = INVALID_SOCKET;
	return ret;
}

static int xnet_pep_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct xnet_pep *pep;

	if ((addrlen != sizeof(struct sockaddr_in)) &&
	    (addrlen != sizeof(struct sockaddr_in6)))
		return -FI_EINVAL;

	pep = container_of(fid, struct xnet_pep,
				util_pep.pep_fid);

	if (pep->sock != INVALID_SOCKET) {
		ofi_close_socket(pep->sock);
		pep->sock = INVALID_SOCKET;
	}

	if (pep->info->src_addr) {
		free(pep->info->src_addr);
		pep->info->src_addrlen = 0;
	}

	pep->info->src_addr = mem_dup(addr, addrlen);
	if (!pep->info->src_addr)
		return -FI_ENOMEM;
	pep->info->src_addrlen = addrlen;

	return xnet_pep_sock_create(pep);
}

static int xnet_pep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct xnet_pep *pep;
	size_t addrlen_in = *addrlen;
	int ret;

	pep = container_of(fid, struct xnet_pep, util_pep.pep_fid);
	ret = ofi_getsockname(pep->sock, addr, (socklen_t *) addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen) ? -FI_ETOOSMALL: FI_SUCCESS;
}

int xnet_listen(struct xnet_pep *pep, struct xnet_progress *progress)
{
	int ret;

	if (pep->state != XNET_IDLE) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"passive endpoint is not idle\n");
		return -FI_EINVAL;
	}

	/* arbitrary backlog value to support larger scale jobs */
	if (listen(pep->sock, 4096)) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"socket listen failed\n");
		return -ofi_sockerr();
	}

	ofi_genlock_lock(&progress->ep_lock);
	if (xnet_io_uring) {
		ret = xnet_uring_pollin_add(progress, pep->sock, true,
					    &pep->pollin_sockctx);
	} else {
		ret = xnet_monitor_sock(progress, pep->sock, POLLIN,
					&pep->util_pep.pep_fid.fid);
	}

	if (!ret) {
		pep->progress = progress;
		pep->state = XNET_LISTENING;
	}
	ofi_genlock_unlock(&progress->ep_lock);

	return ret;
}

static int xnet_pep_listen(struct fid_pep *pep_fid)
{
	struct xnet_pep *pep;
	struct xnet_eq *eq;

	pep = container_of(pep_fid, struct xnet_pep, util_pep.pep_fid);
	eq = container_of(pep->util_pep.eq, struct xnet_eq, util_eq);
	return xnet_listen(pep, &eq->progress);
}

static int xnet_pep_reject(struct fid_pep *pep, fid_t fid_handle,
			   const void *param, size_t paramlen)
{
	struct xnet_cm_msg msg;
	struct xnet_conn_handle *conn;
	ssize_t size_ret;
	int ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "rejecting connection\n");
	conn = container_of(fid_handle, struct xnet_conn_handle, fid);
	/* If we created an endpoint, it owns the socket */
	if (conn->sock == INVALID_SOCKET)
		goto free;

	memset(&msg.hdr, 0, sizeof(msg.hdr));
	msg.hdr.version = XNET_CTRL_HDR_VERSION;
	msg.hdr.type = ofi_ctrl_nack;
	msg.hdr.seg_size = htons((uint16_t) paramlen);
	if (paramlen)
		memcpy(&msg.data, param, paramlen);

	size_ret = ofi_send_socket(conn->sock, &msg,
				   sizeof(msg.hdr) + paramlen, MSG_NOSIGNAL);
	if ((size_t) size_ret != sizeof(msg.hdr) + paramlen)
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"sending of reject message failed\n");

	ofi_shutdown(conn->sock, SHUT_RDWR);
	ret = ofi_close_socket(conn->sock);
	if (ret)
		return ret;

free:
	free(conn);
	return FI_SUCCESS;
}

static struct fi_ops_cm xnet_pep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = xnet_pep_setname,
	.getname = xnet_pep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = xnet_pep_listen,
	.accept = fi_no_accept,
	.reject = xnet_pep_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static int xnet_pep_getopt(fid_t fid, int level, int optname,
			   void *optval, size_t *optlen)
{
	if ( level != FI_OPT_ENDPOINT ||
	     optname != FI_OPT_CM_DATA_SIZE)
		return -FI_ENOPROTOOPT;

	if (*optlen < sizeof(size_t)) {
		*optlen = sizeof(size_t);
		return -FI_ETOOSMALL;
	}

	*((size_t *) optval) = XNET_MAX_CM_DATA_SIZE;
	*optlen = sizeof(size_t);
	return FI_SUCCESS;
}

static struct fi_ops_ep xnet_pep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.getopt = xnet_pep_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int xnet_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_pep **pep_fid, void *context)
{
	struct xnet_pep *pep;
	int ret;

	if (!info) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,"invalid info\n");
		return -FI_EINVAL;
	}

	ret = ofi_prov_check_info(&xnet_util_prov, fabric->api_version, info);
	if (ret)
		return ret;

	pep = calloc(1, sizeof(*pep));
	if (!pep)
		return -FI_ENOMEM;

	ret = ofi_pep_init(fabric, info, &pep->util_pep, context);
	if (ret)
		goto err1;

	pep->util_pep.pep_fid.fid.ops = &xnet_pep_fi_ops;
	pep->util_pep.pep_fid.cm = &xnet_pep_cm_ops;
	pep->util_pep.pep_fid.ops = &xnet_pep_ops;
	ofi_sockctx_init(&pep->pollin_sockctx, &pep->util_pep.pep_fid);

	pep->info = fi_dupinfo(info);
	if (!pep->info) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	pep->sock = INVALID_SOCKET;
	pep->state = XNET_IDLE;

	if (info->src_addr) {
		ret = xnet_pep_sock_create(pep);
		if (ret)
			goto err3;
	}

	*pep_fid = &pep->util_pep.pep_fid;
	return FI_SUCCESS;
err3:
	fi_freeinfo(pep->info);
err2:
	ofi_pep_close(&pep->util_pep);
err1:
	free(pep);
	return ret;
}
