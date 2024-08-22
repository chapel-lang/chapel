/*
 * Copyright (c) 2017-2022 Intel Corporation. All rights reserved.
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

#include <rdma/fi_errno.h>

#include <ofi_prov.h>
#include "xnet.h"
#include <poll.h>
#include <sys/types.h>
#include <ofi_util.h>


/* Must be castable to struct fi_eq_cm_entry */
struct xnet_cm_entry {
	fid_t			fid;
	struct fi_info		*info;
	uint8_t			data[XNET_MAX_CM_DATA_SIZE];
};

/* The underlying socket has the POLLIN event set.  The entire
 * CM message should be readable, as it fits within a single MTU
 * and is the first data transferred over the socket.
 */
static int
xnet_recv_cm_msg(SOCKET sock, struct xnet_cm_msg *msg)
{
	size_t len;
	ssize_t ret;

	len = sizeof(msg->hdr);
	ret = ofi_recv_socket(sock, &msg->hdr, len, 0);
	if ((size_t) ret != len) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"Failed to read cm header, ret: %zd, sockerr: %d\n",
			ret, ofi_sockerr());
		msg->hdr.seg_size = 0;
		return ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
	}

	return 0;
}

static int
xnet_handle_cm_msg(SOCKET sock, struct xnet_cm_msg *msg, uint8_t exp_msg)
{
	size_t len;
	ssize_t ret;

	if (msg->hdr.version != XNET_CTRL_HDR_VERSION) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"cm protocol version mismatch\n");
		ret = -FI_ENOPROTOOPT;
		goto err;
	}

	if (msg->hdr.type != exp_msg && msg->hdr.type != ofi_ctrl_nack) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"unexpected message, expected %d or %d got: %d\n",
			exp_msg, ofi_ctrl_nack, msg->hdr.type);
		ret = -FI_ECONNREFUSED;
		goto err;
	}

	len = ntohs(msg->hdr.seg_size);
	if (len) {
		if (len > XNET_MAX_CM_DATA_SIZE) {
			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"cm data size is too large, seg_size: %zu\n",
				len);
			ret = -FI_ENOPROTOOPT;
			goto err;
		}

		ret = ofi_recv_socket(sock, msg->data, len, 0);
		if ((size_t) ret != len) {
			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"Failed to read cm data, ret: %zd, sockerr: %d\n",
				ret, ofi_sockerr());
			ret = ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
			goto err;
		}
	}

	if (msg->hdr.type == ofi_ctrl_nack) {
		FI_INFO(&xnet_prov, FI_LOG_EP_CTRL,
			"Connection refused from remote\n");
		return -FI_ECONNREFUSED;
	}

	return 0;

err:
	/* For any unexpected error, we have no cm data to report */
	msg->hdr.seg_size = 0;
	return ret;
}

/* The underlying socket has the POLLOUT event set.  It is ready
 * to accept outbound data.  We expect to transfer the entire CM
 * message as it fits into a single MTU and is the first data
 * transferred over the socket.
 */
int xnet_send_cm_msg(struct xnet_ep *ep)
{
	size_t len;
	ssize_t ret;

	len = sizeof(ep->cm_msg->hdr) + ntohs(ep->cm_msg->hdr.seg_size);

	ret = ofi_send_socket(ep->bsock.sock, ep->cm_msg, len, MSG_NOSIGNAL);
	if ((size_t) ret != len)
		return ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;

	return FI_SUCCESS;
}

static int xnet_req_done_internal(struct xnet_ep *ep)
{
	struct xnet_cm_entry cm_entry;
	uint16_t len;
	int ret;

	if (xnet_trace_msg) {
		ep->hdr_bswap = (ep->cm_msg->hdr.conn_data == 1) ?
				xnet_hdr_trace : xnet_hdr_bswap_trace;
	} else {
		ep->hdr_bswap = (ep->cm_msg->hdr.conn_data == 1) ?
				xnet_hdr_none : xnet_hdr_bswap;
	}

	len = ntohs(ep->cm_msg->hdr.seg_size);
	cm_entry.fid = &ep->util_ep.ep_fid.fid;
	cm_entry.info = NULL;
	if (len)
		memcpy(cm_entry.data, ep->cm_msg->data, len);

	ret = xnet_eq_write(ep->util_ep.eq, FI_CONNECTED, &cm_entry,
			    sizeof(struct fi_eq_cm_entry) + len, 0);
	if (ret < 0) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");
		return ret;
	}

	assert(!ofi_bsock_readable(&ep->bsock) && !ep->cur_rx.handler);
	ep->state = XNET_CONNECTED;
	free(ep->cm_msg);
	ep->cm_msg = NULL;
	free(ep->addr);
	ep->addr = NULL;
	return 0;
}

void xnet_req_done(struct xnet_ep *ep)
{
	ssize_t ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "connect request done\n");
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));

	ret = xnet_recv_cm_msg(ep->bsock.sock, ep->cm_msg);
	if (ret == 0)
		ret = xnet_handle_cm_msg(ep->bsock.sock, ep->cm_msg, ofi_ctrl_connresp);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return; /* This shouldn't happen */

		enum fi_log_level level = (ret == -FI_ECONNREFUSED) ?
				FI_LOG_INFO : FI_LOG_WARN;
		FI_LOG(&xnet_prov, level, FI_LOG_EP_CTRL,
			"Failed to receive connect response\n");
		goto disable;
	}

	ret = xnet_req_done_internal(ep);
	if (ret)
		goto disable;
	return;

disable:
	xnet_ep_disable(ep, -ret, ep->cm_msg->data,
			ntohs(ep->cm_msg->hdr.seg_size));
}

void xnet_uring_req_done(struct xnet_ep *ep, int res)
{
	size_t len;
	ssize_t ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "connect request done\n");
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));

	len = sizeof(ep->cm_msg->hdr);
	if (res < 0)
		ret = res;
	else if (res == len)
		ret = xnet_handle_cm_msg(ep->bsock.sock, ep->cm_msg,
					 ofi_ctrl_connresp);
	else
		ret = -FI_EIO;

	if (ret) {
		/* FIXME: handle EAGAIN */

		enum fi_log_level level = (res == -FI_ECONNREFUSED) ?
				FI_LOG_INFO : FI_LOG_WARN;
		FI_LOG(&xnet_prov, level, FI_LOG_EP_CTRL,
			"Failed to receive connect response\n");
		goto disable;
	}

	ep->pollflags = POLLIN;
	ret = xnet_uring_pollin_add(xnet_ep2_progress(ep), ep->bsock.sock,
				    false, &ep->bsock.pollin_sockctx);
	if (ret)
		goto disable;

	ret = xnet_req_done_internal(ep);
	if (ret)
		goto disable;
	return;

disable:
	xnet_ep_disable(ep, -ret, ep->cm_msg->data,
			ntohs(ep->cm_msg->hdr.seg_size));

}

void xnet_handle_conn(struct xnet_conn_handle *conn, bool error)
{
	struct xnet_cm_msg msg;
	struct xnet_cm_entry cm_entry;
	socklen_t len;
	uint16_t datalen;
	int ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "Receiving connect request\n");
	assert(xnet_progress_locked(conn->pep->progress));

	if (error) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "socket error\n");
		goto close;
	}

	ret = xnet_recv_cm_msg(conn->sock, &msg);
	if (ret == 0)
		ret = xnet_handle_cm_msg(conn->sock, &msg, ofi_ctrl_connreq);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return;
		goto close;
	}

	cm_entry.fid = &conn->pep->util_pep.pep_fid.fid;
	cm_entry.info = fi_dupinfo(conn->pep->info);
	if (!cm_entry.info)
		goto close;

	cm_entry.info->dest_addrlen = conn->pep->info->src_addrlen;
	len = (socklen_t) cm_entry.info->dest_addrlen;

	free(cm_entry.info->dest_addr);
	cm_entry.info->dest_addr = malloc(len);
	if (!cm_entry.info->dest_addr)
		goto freeinfo;

	ret = ofi_getpeername(conn->sock, cm_entry.info->dest_addr, &len);
	if (ret)
		goto freeinfo;

	ofi_straddr_dbg(&xnet_prov, FI_LOG_EP_CTRL, "conn req from dest",
				    cm_entry.info->dest_addr);
	ofi_straddr_dbg(&xnet_prov, FI_LOG_EP_CTRL, "conn req for src",
				    cm_entry.info->src_addr);
	conn->endian_match = (msg.hdr.conn_data == 1);
	cm_entry.info->handle = &conn->fid;
	datalen = ntohs(msg.hdr.seg_size);
	if (datalen)
		memcpy(cm_entry.data, msg.data, datalen);

	ret = xnet_eq_write(conn->pep->util_pep.eq,
			    FI_CONNREQ, &cm_entry,
			    sizeof(struct fi_eq_cm_entry) + datalen, 0);
	if (ret < 0) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");
		goto freeinfo;
	}

	return;

freeinfo:
	fi_freeinfo(cm_entry.info);
close:
	ofi_close_socket(conn->sock);
	free(conn);
}

void xnet_connect_done(struct xnet_ep *ep)
{
	struct xnet_progress *progress;
	socklen_t len;
	int status, ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "socket connected, sending req\n");
	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));

	len = sizeof(status);
	ret = getsockopt(ep->bsock.sock, SOL_SOCKET, SO_ERROR,
			 (char *) &status, &len);
	if (ret < 0 || status) {
		ret = (ret < 0)? -ofi_sockerr() : -status;
		FI_WARN_SPARSE(&xnet_prov, FI_LOG_EP_CTRL,
				"connection failure (sockerr %d)\n", ret);
		goto disable;
	}

	ret = xnet_send_cm_msg(ep);
	if (ret)
		goto disable;

	ep->state = XNET_REQ_SENT;
	ep->pollflags = POLLIN;
	ofi_dynpoll_mod(&progress->epoll_fd, ep->bsock.sock,
			ep->pollflags, &ep->util_ep.ep_fid.fid);
	xnet_signal_progress(progress);
	return;

disable:
	xnet_ep_disable(ep, -ret, NULL, 0);
}

void xnet_accept_sock(struct xnet_pep *pep)
{
	struct xnet_conn_handle *conn;
	int ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "accepting socket\n");
	assert(xnet_progress_locked(pep->progress));

	conn = calloc(1, sizeof(*conn));
	if (!conn) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory\n");
		return;
	}

	conn->fid.fclass = FI_CLASS_CONNREQ;
	/* TODO: We need to hold a reference on the pep to defer destruction */
	conn->pep = pep;
	conn->sockapi = &pep->progress->sockapi;
	ofi_sockctx_init(&conn->rx_sockctx, &conn->fid);

	ret = conn->sockapi->accept(conn->sockapi, pep->sock, NULL, 0,
				    &conn->rx_sockctx);
	if (ret < 0) {
		conn->sock = INVALID_SOCKET;
		if (ret == -OFI_EINPROGRESS_URING)
			return;

		/* FIXME: handle EAGAIN */

		if (!OFI_SOCK_TRY_ACCEPT_AGAIN(-ret)) {
			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"accept error: %d\n", -ret);
		}
		goto free;
	}

	conn->sock = ret;
	ret = xnet_monitor_sock(pep->progress, conn->sock, POLLIN, &conn->fid);
	if (ret)
		goto close;

	return;

close:
	ofi_close_socket(conn->sock);
free:
	free(conn);
}
