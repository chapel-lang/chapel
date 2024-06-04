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

extern struct fi_ops_rma xnet_rma_ops;
extern struct fi_ops_msg xnet_msg_ops;
extern struct fi_ops_tagged xnet_tagged_ops;

static const char *const xnet_opstr[] = {
	[xnet_op_msg] = "msg",
	[xnet_op_tag] = "tagged",
	[xnet_op_read_req] = "read req",
	[xnet_op_read_rsp]  = "read resp",
	[xnet_op_write] = "write",
	[xnet_op_tag_rts] = "tag rts",
	[xnet_op_cts] = "cts",
	[xnet_op_data] = "rndv data",
};

static const char *xnet_op_str(uint8_t op)
{
	if (op < ARRAY_SIZE(xnet_opstr))
		return xnet_opstr[op];
	return "unknown";
}

void xnet_hdr_none(struct xnet_ep *ep, struct xnet_base_hdr *hdr)
{
	/* no-op */
}

void xnet_hdr_bswap(struct xnet_ep *ep, struct xnet_base_hdr *hdr)
{
	uint64_t *cur;
	int i, cnt;

	hdr->flags = ntohs(hdr->flags);
	hdr->size = ntohll(hdr->size);

	cnt = (hdr->hdr_size - sizeof(*hdr)) >> 3;
	cur = (uint64_t *) (hdr + 1);
	for (i = 0; i < cnt; i++)
		cur[i] = ntohll(cur[i]);
}

void xnet_hdr_trace(struct xnet_ep *ep, struct xnet_base_hdr *hdr)
{
	uint64_t tag;
	const char *dir;

	if (hdr->op == xnet_op_tag || hdr->op == xnet_op_tag_rts) {
		tag = (hdr->flags & XNET_REMOTE_CQ_DATA) ?
			((struct xnet_tag_data_hdr *) hdr)->tag :
			((struct xnet_tag_hdr *) hdr)->tag;
	} else {
		tag = 0;
	}

	dir = (hdr == &ep->cur_rx.hdr.base_hdr) ? "Rx" : "Tx";

	FI_TRACE(&xnet_prov, FI_LOG_EP_DATA, "%s op:%s tag:0x%zx flags:0x%x "
		 "op_data:0x%x hdr_size:%d data_size:%zu\n", dir,
		 xnet_op_str(hdr->op), tag, hdr->flags, hdr->op_data,
		 hdr->hdr_size, hdr->size - hdr->hdr_size);
}

void xnet_hdr_bswap_trace(struct xnet_ep *ep, struct xnet_base_hdr *hdr)
{
	xnet_hdr_bswap(ep, hdr);
	xnet_hdr_trace(ep, hdr);
}

#ifdef MSG_ZEROCOPY
void xnet_set_zerocopy(SOCKET sock)
{
	int val = 1;

	if (xnet_zerocopy_size == SIZE_MAX)
		return;

	(void) setsockopt(sock, SOL_SOCKET, SO_ZEROCOPY, &val, sizeof(val));
}

static void xnet_config_bsock(struct ofi_bsock *bsock)
{
	int ret, val = 0;
	socklen_t len = sizeof(val);

	if (xnet_zerocopy_size == SIZE_MAX)
		return;

	ret = getsockopt(bsock->sock, SOL_SOCKET, SO_ZEROCOPY, &val, &len);
	if (!ret && val) {
		bsock->zerocopy_size = xnet_zerocopy_size;
		FI_INFO(&xnet_prov, FI_LOG_EP_CTRL,
			"zero copy enabled for transfers > %zu\n",
			bsock->zerocopy_size);
	}
}
#else
void xnet_set_zerocopy(SOCKET sock)
{
	OFI_UNUSED(sock);
}

#define xnet_config_bsock(bsock)
#endif

#ifdef IP_BIND_ADDRESS_NO_PORT
static void xnet_set_no_port(SOCKET sock)
{
	int val = 1;

	(void) setsockopt(sock, IPPROTO_IP, IP_BIND_ADDRESS_NO_PORT,
			  &val, sizeof(val));
}
#else
#define xnet_set_no_port(sock)
#endif

int xnet_setup_socket(SOCKET sock, struct fi_info *info)
{
	int ret, optval = 1;

	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval,
			 sizeof(optval));
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,"setsockopt reuseaddr failed\n");
		return -ofi_sockerr();
	}

	/* Do not enable nodelay for bulk data traffic class, unless nodelay
	 * has explicitly been requested.
	 */
	if (xnet_nodelay && !((xnet_nodelay < 0) &&
	    (info->fabric_attr->api_version >= FI_VERSION(1, 9) &&
	    info->tx_attr->tclass == FI_TC_BULK_DATA))) {

		ret = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
				 (char *) &optval, sizeof(optval));
		if (ret) {
			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"setsockopt nodelay failed\n");
			return -ofi_sockerr();
		}
	}

	ret = fi_fd_nonblock(sock);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"failed to set socket to nonblocking\n");
		return ret;
	}

	return 0;
}

static int xnet_monitor_ep(struct xnet_progress *progress, struct xnet_ep *ep)
{
	if (xnet_io_uring) {
		assert(!(ep->pollflags & POLLOUT));
		return xnet_uring_pollin_add(progress, ep->bsock.sock,
					     false, &ep->bsock.pollin_sockctx);
	}

	return xnet_monitor_sock(progress, ep->bsock.sock, ep->pollflags,
				 &ep->util_ep.ep_fid.fid);
}

static int xnet_ep_connect(struct fid_ep *ep_fid, const void *addr,
			   const void *param, size_t paramlen)
{
	struct xnet_progress *progress;
	struct xnet_ep *ep;
	int ret;


	ofi_straddr_dbg(&xnet_prov, FI_LOG_EP_CTRL, "connecting to", addr);
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	if (!addr || (ep->bsock.sock == INVALID_SOCKET) ||
	    (paramlen > XNET_MAX_CM_DATA_SIZE) || (ep->state != XNET_IDLE))
		return -FI_EINVAL;

	ep->cm_msg->hdr.version = XNET_CTRL_HDR_VERSION;
	ep->cm_msg->hdr.type = ofi_ctrl_connreq;
	ep->cm_msg->hdr.conn_data = 1; /* tests endianess mismatch at peer */
	if (paramlen) {
		memcpy(ep->cm_msg->data, param, paramlen);
		ep->cm_msg->hdr.seg_size = htons((uint16_t) paramlen);
	}

	ep->addr = mem_dup(addr, ofi_sizeofaddr(addr));
	if (!ep->addr)
		return -FI_ENOMEM;

	ep->state = XNET_CONNECTING;
	ret = ofi_bsock_connect(&ep->bsock, ep->addr,
				(socklen_t) ofi_sizeofaddr(ep->addr));
	if (ret) {
		if (ret == -OFI_EINPROGRESS_URING)
			return 0;

		/* FIXME: handle EAGAIN */

		if (!OFI_SOCK_TRY_CONN_AGAIN(-ret)) {
			ep->state = XNET_IDLE;
			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"connect failure %d(%s)\n", -ret,
				fi_strerror(-ret));
			return ret;
		}
	}

	progress = xnet_ep2_progress(ep);
	ofi_genlock_lock(&progress->ep_lock);
	ep->pollflags = POLLOUT;
	ret = xnet_monitor_ep(progress, ep);
	ofi_genlock_unlock(&progress->ep_lock);
	if (ret)
		goto disable;

	return 0;

disable:
	ofi_genlock_lock(&progress->ep_lock);
	xnet_ep_disable(ep, -ret, NULL, 0);
	ofi_genlock_unlock(&progress->ep_lock);
	return ret;
}

static int
xnet_ep_accept(struct fid_ep *ep_fid, const void *param, size_t paramlen)
{
	struct xnet_progress *progress;
	struct xnet_ep *ep;
	struct xnet_conn_handle *conn;
	struct fi_eq_cm_entry cm_entry;
	int ret;

	FI_DBG(&xnet_prov, FI_LOG_EP_CTRL, "accepting endpoint connection\n");
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	conn = ep->conn;
	if (ep->bsock.sock == INVALID_SOCKET || ep->state != XNET_ACCEPTING ||
	    !conn || (conn->fid.fclass != FI_CLASS_CONNREQ) ||
	    (paramlen > XNET_MAX_CM_DATA_SIZE))
		return -FI_EINVAL;

	ep->conn = NULL;

	assert(ep->cm_msg);
	ep->cm_msg->hdr.version = XNET_CTRL_HDR_VERSION;
	ep->cm_msg->hdr.type = ofi_ctrl_connresp;
	ep->cm_msg->hdr.conn_data = 1; /* tests endianess mismatch at peer */
	if (paramlen) {
		memcpy(ep->cm_msg->data, param, paramlen);
		ep->cm_msg->hdr.seg_size = htons((uint16_t) paramlen);
	}

	ret = xnet_send_cm_msg(ep);
	if (ret)
		return ret;

	free(ep->cm_msg);
	ep->cm_msg = NULL;
	ep->state = XNET_CONNECTED;
	assert(!ofi_bsock_readable(&ep->bsock) && !ep->cur_rx.handler);

	progress = xnet_ep2_progress(ep);
	ofi_genlock_lock(&progress->ep_lock);
	ep->pollflags = POLLIN;
	ret = xnet_monitor_ep(progress, ep);
	ofi_genlock_unlock(&progress->ep_lock);
	if (ret)
		return ret;

	cm_entry.fid = &ep->util_ep.ep_fid.fid;
	cm_entry.info = NULL;
	ret = xnet_eq_write(ep->util_ep.eq, FI_CONNECTED, &cm_entry,
			    sizeof(cm_entry), 0);
	if (ret < 0) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");
		return ret;
	}

	/* Only free conn on success; on failure, app may try to reject */
	free(conn);
	return 0;
}

static void
xnet_flush_xfer_queue(struct xnet_progress *progress, struct slist *queue,
		      struct ofi_byte_idx *idx)
{
	struct xnet_xfer_entry *xfer_entry;

	assert(xnet_progress_locked(progress));
	while (!slist_empty(queue)) {
		xfer_entry = container_of(queue->head, struct xnet_xfer_entry,
					  entry);
		if (xfer_entry->ctrl_flags & XNET_NEED_CTS) {
			assert(idx);
			assert(xfer_entry->hdr.base_hdr.op == xnet_op_tag_rts);
			ofi_byte_idx_remove(idx, xfer_entry->hdr.base_hdr.op_data);
		}
		slist_remove_head(queue);
		xnet_report_error(xfer_entry, FI_ECANCELED);
		xnet_free_xfer(progress, xfer_entry);
	}
}

static void
xnet_flush_byte_idx(struct xnet_progress *progress, struct ofi_byte_idx *idx)
{
	struct xnet_xfer_entry *xfer_entry;
	uint8_t i;

	if (!idx->data)
		return;

	for (i = 1; i < UINT8_MAX; i++) {
		xfer_entry = ofi_byte_idx_lookup(idx, i);
		if (xfer_entry) {
			xnet_report_error(xfer_entry, FI_ECANCELED);
			xnet_free_xfer(progress, xfer_entry);
		}
	}
}

static void xnet_ep_flush_all_queues(struct xnet_ep *ep)
{
	struct xnet_progress *progress;
	int ret;

	progress = xnet_ep2_progress(ep);
	assert(xnet_progress_locked(progress));

	ret = xnet_uring_cancel(progress, &progress->tx_uring,
				&ep->bsock.tx_sockctx,
				&ep->util_ep.ep_fid);
	if (ret)
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "Failed to cancel TX uring\n");

	ret = xnet_uring_cancel(progress, &progress->rx_uring,
				&ep->bsock.rx_sockctx,
				&ep->util_ep.ep_fid);
	if (ret)
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "Failed to cancel RX uring\n");

	ret = xnet_uring_cancel(progress, &progress->rx_uring,
				&ep->bsock.pollin_sockctx,
				&ep->util_ep.ep_fid);
	if (ret)
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "Failed to cancel POLLIN uring\n");

	if (ep->cur_tx.entry) {
		ep->hdr_bswap(ep, &ep->cur_tx.entry->hdr.base_hdr);
		if (ep->cur_tx.entry->ctrl_flags & XNET_NEED_CTS) {
			assert(ep->cur_tx.entry->hdr.base_hdr.op == xnet_op_tag_rts);
			ofi_byte_idx_remove(&ep->rts_queue,
					    ep->cur_tx.entry->hdr.base_hdr.op_data);
		}
		xnet_report_error(ep->cur_tx.entry, FI_ECANCELED);
		xnet_free_xfer(xnet_ep2_progress(ep), ep->cur_tx.entry);
		ep->cur_tx.entry = NULL;
	}

	xnet_flush_xfer_queue(progress, &ep->tx_queue, &ep->rts_queue);
	xnet_flush_xfer_queue(progress, &ep->priority_queue, NULL);
	xnet_flush_xfer_queue(progress, &ep->rma_read_queue, NULL);
	xnet_flush_xfer_queue(progress, &ep->need_ack_queue, NULL);
	xnet_flush_xfer_queue(progress, &ep->async_queue, NULL);
	xnet_flush_byte_idx(progress, &ep->rts_queue);
	xnet_flush_byte_idx(progress, &ep->cts_queue);

	/* Saved messages are on the saved_msg queue and flushed by the srx */
	if (ep->cur_rx.entry &&
	    !(ep->cur_rx.entry->ctrl_flags & XNET_SAVED_XFER)) {
		xnet_report_error(ep->cur_rx.entry, FI_ECANCELED);
		xnet_free_xfer(xnet_ep2_progress(ep), ep->cur_rx.entry);
	}
	xnet_reset_rx(ep);
	xnet_flush_xfer_queue(progress, &ep->rx_queue, NULL);
	ep->rx_avail = 0;
	ofi_bsock_discard(&ep->bsock);
}

void xnet_ep_disable(struct xnet_ep *ep, int cm_err, void* err_data,
                     size_t err_data_size)
{
	struct fi_eq_cm_entry cm_entry = {0};
	struct fi_eq_err_entry err_entry = {0};
	int ret;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	switch (ep->state) {
	case XNET_CONNECTING:
	case XNET_REQ_SENT:
	case XNET_CONNECTED:
		break;
	default:
		return;
	};

	ep->state = XNET_DISCONNECTED;
	dlist_remove_init(&ep->unexp_entry);
	if (!xnet_io_uring)
		xnet_halt_sock(xnet_ep2_progress(ep), ep->bsock.sock);

	ret = ofi_shutdown(ep->bsock.sock, SHUT_RDWR);
	if (ret && ofi_sockerr() != ENOTCONN)
		FI_WARN(&xnet_prov, FI_LOG_EP_DATA, "shutdown failed\n");

	xnet_ep_flush_all_queues(ep);

	if (cm_err) {
		err_entry.err = cm_err;
		err_entry.fid = &ep->util_ep.ep_fid.fid;
		err_entry.context = ep->util_ep.ep_fid.fid.context;
		if (err_data && err_data_size > 0) {
			err_entry.err_data = mem_dup(err_data, err_data_size);
			if (err_entry.err_data)
				err_entry.err_data_size = err_data_size;
		}
		(void) xnet_eq_write(ep->util_ep.eq, FI_SHUTDOWN,
				     &err_entry, sizeof(err_entry),
				     UTIL_FLAG_ERROR);
	} else {
		cm_entry.fid = &ep->util_ep.ep_fid.fid;
		(void) xnet_eq_write(ep->util_ep.eq, FI_SHUTDOWN,
				     &cm_entry, sizeof(cm_entry), 0);
	}
}

static int xnet_ep_shutdown(struct fid_ep *ep_fid, uint64_t flags)
{
	struct xnet_ep *ep;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	(void) ofi_bsock_flush_sync(&ep->bsock);
	xnet_ep_disable(ep, 0, NULL, 0);
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);

	return FI_SUCCESS;
}

static int xnet_ep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct xnet_ep *ep;
	size_t addrlen_in = *addrlen;
	int ret;

	ep = container_of(fid, struct xnet_ep, util_ep.ep_fid);
	ret = ofi_getsockname(ep->bsock.sock, addr, (socklen_t *) addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen)? -FI_ETOOSMALL: FI_SUCCESS;
}

static int xnet_ep_getpeer(struct fid_ep *ep_fid, void *addr, size_t *addrlen)
{
	struct xnet_ep *ep;
	size_t addrlen_in = *addrlen;
	int ret;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	ret = ofi_getpeername(ep->bsock.sock, addr, (socklen_t *) addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen) ? -FI_ETOOSMALL: FI_SUCCESS;
}

static struct fi_ops_cm xnet_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = xnet_ep_getname,
	.getpeer = xnet_ep_getpeer,
	.connect = xnet_ep_connect,
	.listen = fi_no_listen,
	.accept = xnet_ep_accept,
	.reject = fi_no_reject,
	.shutdown = xnet_ep_shutdown,
	.join = fi_no_join,
};

void xnet_reset_rx(struct xnet_ep *ep)
{
	ep->cur_rx.handler = NULL;
	ep->cur_rx.entry = NULL;
	ep->cur_rx.hdr_done = 0;
	ep->cur_rx.hdr_len = sizeof(ep->cur_rx.hdr.base_hdr);
	ep->cur_rx.claim_ctx = NULL;
	OFI_DBG_SET(ep->cur_rx.hdr.base_hdr.version, 0);
}

static void xnet_ep_cancel_rx(struct xnet_ep *ep, void *context)
{
	struct slist_entry *cur, *prev;
	struct xnet_xfer_entry *xfer_entry;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));

	/* To cancel an active receive, we would need to flush the socket of
	 * all data associated with that message.  Since some of that data
	 * may not have arrived yet, this would require additional state
	 * tracking and complexity.  Fail the cancel in this case, since
	 * the receive is already in process anyway.
	 */
	slist_foreach(&ep->rx_queue, cur, prev) {
		xfer_entry = container_of(cur, struct xnet_xfer_entry, entry);
		if (xfer_entry->context == context) {
			if (ep->cur_rx.entry != xfer_entry)
				goto found;
			break;
		}
	}

	return;

found:
	slist_remove(&ep->rx_queue, cur, prev);
	ep->rx_avail++;
	xnet_report_error(xfer_entry, FI_ECANCELED);
	xnet_free_xfer(xnet_ep2_progress(ep), xfer_entry);
}

/* We currently only support canceling receives, which is the common case.
 * Canceling an operation from the other queues is not trivial,
 * especially if the operation has already been initiated.
 */
static ssize_t xnet_ep_cancel(fid_t fid, void *context)
{
	struct xnet_ep *ep;

	ep = container_of(fid, struct xnet_ep, util_ep.ep_fid.fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	xnet_ep_cancel_rx(ep, context);
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);

	return 0;
}

static int xnet_ep_close(struct fid *fid)
{
	struct xnet_progress *progress;
	struct xnet_ep *ep;

	ep = container_of(fid, struct xnet_ep, util_ep.ep_fid.fid);

	progress = xnet_ep2_progress(ep);
	ofi_genlock_lock(&progress->ep_lock);
	ep->state = XNET_DISCONNECTED;
	dlist_remove_init(&ep->unexp_entry);
	if (!xnet_io_uring)
		xnet_halt_sock(progress, ep->bsock.sock);
	ofi_close_socket(ep->bsock.sock);
	xnet_ep_flush_all_queues(ep);
	ofi_genlock_unlock(&progress->ep_lock);

	if (ep->bsock.tx_sockctx.uring_sqe_inuse ||
	    ep->bsock.rx_sockctx.uring_sqe_inuse ||
	    ep->bsock.pollin_sockctx.uring_sqe_inuse)
		return -FI_EBUSY;

	free(ep->cm_msg);
	free(ep->addr);

	ofi_endpoint_close(&ep->util_ep);
	free(ep);
	return 0;
}

static int xnet_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct xnet_ep *ep;

	ep = container_of(fid, struct xnet_ep, util_ep.ep_fid.fid);
	switch (command) {
	case FI_ENABLE:
		if ((ofi_needs_rx(ep->util_ep.caps) && !ep->util_ep.rx_cq) ||
		    (ofi_needs_tx(ep->util_ep.caps) && !ep->util_ep.tx_cq)) {
			FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
				"missing needed CQ binding\n");
			return -FI_ENOCQ;
		}
		break;
	default:
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "unsupported command\n");
		return -FI_ENOSYS;
	}
	return FI_SUCCESS;
}

static int xnet_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct xnet_domain *domain;
	struct xnet_ep *ep;
	struct xnet_eq *eq;
	struct xnet_srx *srx;
	int ret;

	ep = container_of(fid, struct xnet_ep, util_ep.ep_fid.fid);

	switch (bfid->fclass) {
	case FI_CLASS_SRX_CTX:
		srx = container_of(bfid, struct xnet_srx, rx_fid.fid);
		ep->srx = srx;
		if (!ep->profile)
			ep->profile = srx->profile;
		return FI_SUCCESS;
	case FI_CLASS_EQ:
		/* msg endpoints created by an rdm endpoint will not/cannot
		 * go through this path without creating a potential deadlock
		 * as a result of nested locking.  This is because we would
		 * be holding the progress lock when the rdm ep creates and
		 * configures the msg ep.
		 */
		eq = container_of(bfid, struct xnet_eq, util_eq.eq_fid.fid);
		domain = container_of(ep->util_ep.domain, struct xnet_domain,
				      util_domain.domain_fid.fid);
		ret = xnet_add_domain_progress(eq, domain);
		if (ret)
			break;
		/* fall through */
	default:
		ret = ofi_ep_bind(&ep->util_ep, bfid, flags);
		break;
	}

	return ret;
}

static struct fi_ops xnet_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = xnet_ep_close,
	.bind = xnet_ep_bind,
	.control = xnet_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int xnet_ep_getopt(fid_t fid, int level, int optname,
			  void *optval, size_t *optlen)
{
	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_CM_DATA_SIZE:
		if (*optlen < sizeof(size_t)) {
			*optlen = sizeof(size_t);
			return -FI_ETOOSMALL;
		}
		*((size_t *) optval) = XNET_MAX_CM_DATA_SIZE;
		*optlen = sizeof(size_t);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}
	return FI_SUCCESS;
}

int xnet_ep_setopt(fid_t fid, int level, int optname,
		   const void *optval, size_t optlen)
{
	return -FI_ENOPROTOOPT;
}

static struct fi_ops_ep xnet_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = xnet_ep_cancel,
	.getopt = xnet_ep_getopt,
	.setopt = xnet_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int xnet_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct xnet_ep *ep;
	struct xnet_pep *pep;
	struct xnet_conn_handle *conn;
	int ret;

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = ofi_endpoint_init(domain, &xnet_util_prov, info, &ep->util_ep,
				context, NULL);
	if (ret)
		goto err1;

	assert(info->ep_attr->type == FI_EP_MSG);
	ofi_bsock_init(&ep->bsock, &xnet_ep2_progress(ep)->sockapi,
		       xnet_staging_sbuf_size, xnet_prefetch_rbuf_size,
		       &ep->util_ep.ep_fid);
	if (info->handle) {
		if (((fid_t) info->handle)->fclass == FI_CLASS_PEP) {
			pep = container_of(info->handle, struct xnet_pep,
					   util_pep.pep_fid.fid);

			ep->bsock.sock = pep->sock;
			pep->sock = INVALID_SOCKET;
		} else {
			ep->state = XNET_ACCEPTING;
			conn = container_of(info->handle,
					    struct xnet_conn_handle, fid);
			/* EP now owns socket */
			ep->bsock.sock = conn->sock;
			conn->sock = INVALID_SOCKET;
			if (xnet_trace_msg) {
				ep->hdr_bswap = conn->endian_match ?
						xnet_hdr_trace : xnet_hdr_bswap_trace;
			} else {
				ep->hdr_bswap = conn->endian_match ?
						xnet_hdr_none : xnet_hdr_bswap;
			}
			/* Save handle, but we only free if user calls accept.
			 * Otherwise, user will call reject, which will free it.
			 */
			ep->conn = conn;

			ret = xnet_setup_socket(ep->bsock.sock, info);
			if (ret)
				goto err3;
		}
	} else {
		ep->bsock.sock = ofi_socket(ofi_get_sa_family(info), SOCK_STREAM, 0);
		if (ep->bsock.sock == INVALID_SOCKET) {
			ret = -ofi_sockerr();
			goto err2;
		}

		ret = xnet_setup_socket(ep->bsock.sock, info);
		if (ret)
			goto err3;

		if (!xnet_io_uring)
			xnet_set_zerocopy(ep->bsock.sock);

		if (info->src_addr && (!ofi_is_any_addr(info->src_addr) ||
					ofi_addr_get_port(info->src_addr))) {

			if (!ofi_addr_get_port(info->src_addr)) {
				xnet_set_no_port(ep->bsock.sock);
			}

			ret = bind(ep->bsock.sock, info->src_addr,
				(socklen_t) info->src_addrlen);
			if (ret) {
				FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "bind failed\n");
				ret = -ofi_sockerr();
				goto err3;
			}
		}
	}

	ep->cm_msg = calloc(1, sizeof(*ep->cm_msg));
	if (!ep->cm_msg) {
		ret = -FI_ENOMEM;
		goto err3;
	}

	dlist_init(&ep->unexp_entry);
	slist_init(&ep->rx_queue);
	slist_init(&ep->tx_queue);
	slist_init(&ep->priority_queue);
	slist_init(&ep->rma_read_queue);
	slist_init(&ep->need_ack_queue);
	slist_init(&ep->async_queue);

	if (info->ep_attr->rx_ctx_cnt != FI_SHARED_CONTEXT)
		ep->rx_avail = (int) info->rx_attr->size;

	ep->cur_rx.hdr_done = 0;
	ep->cur_rx.hdr_len = sizeof(ep->cur_rx.hdr.base_hdr);
	xnet_config_bsock(&ep->bsock);

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &xnet_ep_fi_ops;
	(*ep_fid)->ops = &xnet_ep_ops;
	(*ep_fid)->cm = &xnet_cm_ops;
	(*ep_fid)->msg = &xnet_msg_ops;
	(*ep_fid)->rma = &xnet_rma_ops;
	(*ep_fid)->tagged = &xnet_tagged_ops;
	(*ep_fid)->fid.ops->ops_open = xnet_ep_ops_open;
	return 0;

err3:
	ofi_close_socket(ep->bsock.sock);
err2:
	ofi_endpoint_close(&ep->util_ep);
err1:
	free(ep);
	return ret;
}
