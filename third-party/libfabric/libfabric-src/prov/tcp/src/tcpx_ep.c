/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
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
#include "tcpx.h"
#include <errno.h>

extern struct fi_ops_rma tcpx_rma_ops;
extern struct fi_ops_msg tcpx_msg_ops;
extern struct fi_ops_tagged tcpx_tagged_ops;

static struct ofi_sockapi tcpx_sockapi =
{
	.send = ofi_sockapi_send_socket,
	.sendv = ofi_sockapi_sendv_socket,
	.recv = ofi_sockapi_recv_socket,
	.recvv = ofi_sockapi_recvv_socket,
};

void tcpx_hdr_none(struct tcpx_base_hdr *hdr)
{
	/* no-op */
}

void tcpx_hdr_bswap(struct tcpx_base_hdr *hdr)
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

#ifdef MSG_ZEROCOPY
static void tcpx_set_zerocopy(SOCKET sock)
{
	int val = 1;

	if (tcpx_zerocopy_size == SIZE_MAX)
		return;

	(void) setsockopt(sock, SOL_SOCKET, SO_ZEROCOPY, &val, sizeof(val));
}

static void tcpx_config_bsock(struct ofi_bsock *bsock)
{
	int ret, val = 0;
	socklen_t len = sizeof(val);

	if (tcpx_zerocopy_size == SIZE_MAX)
		return;

	ret = getsockopt(bsock->sock, SOL_SOCKET, SO_ZEROCOPY, &val, &len);
	if (!ret && val) {
		bsock->zerocopy_size = tcpx_zerocopy_size;
		FI_INFO(&tcpx_prov, FI_LOG_EP_CTRL,
			"zero copy enabled for transfers > %zu\n",
			bsock->zerocopy_size);
	}
}
#else
#define tcpx_set_zerocopy(sock)
#define tcpx_config_bsock(bsock)
#endif

#ifdef IP_BIND_ADDRESS_NO_PORT
static void tcpx_set_no_port(SOCKET sock)
{
	int val = 1;

	(void) setsockopt(sock, IPPROTO_IP, IP_BIND_ADDRESS_NO_PORT,
			  &val, sizeof(val));
}
#else
#define tcpx_set_no_port(sock)
#endif

static int tcpx_setup_socket(SOCKET sock, struct fi_info *info)
{
	int ret, optval = 1;

	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval,
			 sizeof(optval));
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,"setsockopt reuseaddr failed\n");
		return -ofi_sockerr();
	}

	/* Do not enable nodelay for bulk data traffic class, unless nodelay
	 * has explicitly been requested.
	 */
	if (tcpx_nodelay && !((tcpx_nodelay < 0) &&
	    (info->fabric_attr->api_version >= FI_VERSION(1, 9) &&
	    info->tx_attr->tclass == FI_TC_BULK_DATA))) {

		ret = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
				 (char *) &optval, sizeof(optval));
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"setsockopt nodelay failed\n");
			return -ofi_sockerr();
		}
	}

	ret = fi_fd_nonblock(sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"failed to set socket to nonblocking\n");
		return ret;
	}

	return 0;
}

static int tcpx_ep_connect(struct fid_ep *ep_fid, const void *addr,
			   const void *param, size_t paramlen)
{
	struct tcpx_ep *ep;
	struct tcpx_cm_context *cm_ctx;
	int ret;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	if (!addr || (ep->bsock.sock == INVALID_SOCKET) ||
	    (paramlen > TCPX_MAX_CM_DATA_SIZE) || (ep->state != TCPX_IDLE))
		return -FI_EINVAL;

	cm_ctx = tcpx_alloc_cm_ctx(&ep_fid->fid, TCPX_CM_CONNECTING);
	if (!cm_ctx) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory\n");
		return -FI_ENOMEM;
	}

	ep->state = TCPX_CONNECTING;
	ret = connect(ep->bsock.sock, (struct sockaddr *) addr,
		      (socklen_t) ofi_sizeofaddr(addr));
	if (ret && !OFI_SOCK_TRY_CONN_AGAIN(ofi_sockerr())) {
		ep->state = TCPX_IDLE;
		ret =  -ofi_sockerr();
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"connect failure %d(%s)\n", -ret, fi_strerror(-ret));
		goto free;
	}

	if (paramlen) {
		cm_ctx->cm_data_sz = paramlen;
		memcpy(cm_ctx->msg.data, param, paramlen);
	}

	ret = ofi_wait_add_fd(ep->util_ep.eq->wait, ep->bsock.sock,
			      POLLOUT, tcpx_eq_wait_try_func, NULL, cm_ctx);
	if (ret)
		goto disable;

	return 0;

disable:
	ofi_mutex_lock(&ep->lock);
	tcpx_ep_disable(ep, -ret, NULL, 0);
	ofi_mutex_unlock(&ep->lock);
free:
	tcpx_free_cm_ctx(cm_ctx);
	return ret;
}

static int
tcpx_ep_accept(struct fid_ep *ep_fid, const void *param, size_t paramlen)
{
	struct tcpx_ep *ep;
	struct tcpx_cm_context *cm_ctx;
	struct tcpx_conn_handle *handle;
	int ret;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	handle = ep->handle;
	if (ep->bsock.sock == INVALID_SOCKET || ep->state != TCPX_RCVD_REQ ||
	    !handle || (handle->fid.fclass != FI_CLASS_CONNREQ))
		return -FI_EINVAL;

	ep->handle = NULL;
	cm_ctx = tcpx_alloc_cm_ctx(&ep->util_ep.ep_fid.fid, TCPX_CM_RESP_READY);
	if (!cm_ctx) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory \n");
		return -FI_ENOMEM;
	}

	ep->state = TCPX_ACCEPTING;

	if (paramlen) {
		cm_ctx->cm_data_sz = paramlen;
		memcpy(cm_ctx->msg.data, param, paramlen);
	}

	ret = ofi_wait_add_fd(ep->util_ep.eq->wait, ep->bsock.sock,
			      POLLOUT, tcpx_eq_wait_try_func, NULL, cm_ctx);
	if (ret)
		goto free;

	free(handle);
	return 0;

free:
	ep->state = TCPX_RCVD_REQ;
	tcpx_free_cm_ctx(cm_ctx);
	return ret;
}

/* must hold ep->lock */
static void tcpx_ep_flush_queue(struct slist *queue,
				struct tcpx_cq *cq)
{
	struct tcpx_xfer_entry *xfer_entry;

	while (!slist_empty(queue)) {
		xfer_entry = container_of(queue->head, struct tcpx_xfer_entry,
					  entry);
		slist_remove_head(queue);
		tcpx_cq_report_error(&cq->util_cq, xfer_entry, FI_ECANCELED);
		tcpx_free_xfer(cq, xfer_entry);
	}
}

static void tcpx_ep_flush_all_queues(struct tcpx_ep *ep)
{
	struct tcpx_cq *cq;

	assert(ofi_mutex_held(&ep->lock));
	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);
	if (ep->cur_tx.entry) {
		ep->hdr_bswap(&ep->cur_tx.entry->hdr.base_hdr);
		tcpx_cq_report_error(&cq->util_cq, ep->cur_tx.entry,
				     FI_ECANCELED);
		tcpx_free_xfer(cq, ep->cur_tx.entry);
		ep->cur_tx.entry = NULL;
	}

	tcpx_ep_flush_queue(&ep->tx_queue, cq);
	tcpx_ep_flush_queue(&ep->priority_queue, cq);
	tcpx_ep_flush_queue(&ep->rma_read_queue, cq);
	tcpx_ep_flush_queue(&ep->need_ack_queue, cq);
	tcpx_ep_flush_queue(&ep->async_queue, cq);

	cq = container_of(ep->util_ep.rx_cq, struct tcpx_cq, util_cq);
	if (ep->cur_rx.entry) {
		tcpx_cq_report_error(&cq->util_cq, ep->cur_rx.entry,
				     FI_ECANCELED);
		tcpx_free_xfer(cq, ep->cur_rx.entry);
	}
	tcpx_reset_rx(ep);
	tcpx_ep_flush_queue(&ep->rx_queue, cq);
	ofi_bsock_discard(&ep->bsock);
}

void tcpx_ep_disable(struct tcpx_ep *ep, int cm_err, void* err_data,
                     size_t err_data_size)
{
	struct util_wait_fd *wait;
	struct fi_eq_cm_entry cm_entry = {0};
	struct fi_eq_err_entry err_entry = {0};
	int ret;

	assert(ofi_mutex_held(&ep->lock));
	switch (ep->state) {
	case TCPX_RCVD_REQ:
		break;
	case TCPX_CONNECTED:
		/* We need to remove the socket from the CQ's fdset,
		 * or the CQ will be left in a 'signaled' state.  This
		 * can result in threads spinning on the CQs fdset.
		 */
		if (ep->util_ep.tx_cq) {
			wait = container_of(ep->util_ep.tx_cq->wait,
					    struct util_wait_fd, util_wait);
			ofi_wait_fdset_del(wait, ep->bsock.sock);
		}

		if (ep->util_ep.rx_cq) {
			wait = container_of(ep->util_ep.rx_cq->wait,
					    struct util_wait_fd, util_wait);
			ofi_wait_fdset_del(wait, ep->bsock.sock);
		}
		/* fall through */
	case TCPX_ACCEPTING:
	case TCPX_CONNECTING:
		wait = container_of(ep->util_ep.eq->wait,
				    struct util_wait_fd, util_wait);
		ofi_wait_fdset_del(wait, ep->bsock.sock);
		break;

	default:
		return;
	}

	ret = ofi_shutdown(ep->bsock.sock, SHUT_RDWR);
	if (ret && ofi_sockerr() != ENOTCONN)
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA, "shutdown failed\n");

	tcpx_ep_flush_all_queues(ep);

	if (cm_err) {
		err_entry.err = cm_err;
		err_entry.fid = &ep->util_ep.ep_fid.fid;
		err_entry.context = ep->util_ep.ep_fid.fid.context;
		if (err_data && err_data_size > 0) {
			err_entry.err_data = mem_dup(err_data, err_data_size);
			if (err_entry.err_data)
				err_entry.err_data_size = err_data_size;
		}
		(void) fi_eq_write(&ep->util_ep.eq->eq_fid, FI_SHUTDOWN,
				   &err_entry, sizeof(err_entry),
				   UTIL_FLAG_ERROR);
	} else {
		cm_entry.fid = &ep->util_ep.ep_fid.fid;
		(void) fi_eq_write(&ep->util_ep.eq->eq_fid, FI_SHUTDOWN,
				   &cm_entry, sizeof(cm_entry), 0);
	}
	ep->state = TCPX_DISCONNECTED;
}

static int tcpx_ep_shutdown(struct fid_ep *ep_fid, uint64_t flags)
{
	struct tcpx_ep *ep;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);

	ofi_mutex_lock(&ep->lock);
	(void) ofi_bsock_flush_sync(&ep->bsock);
	tcpx_ep_disable(ep, 0, NULL, 0);
	ofi_mutex_unlock(&ep->lock);

	return FI_SUCCESS;
}

static int tcpx_bind_to_port_range(SOCKET sock, void* src_addr, size_t addrlen)
{
	int ret, i, rand_port_number;
	static uint32_t seed;
	if (!seed)
		seed = ofi_generate_seed();

	rand_port_number = ofi_xorshift_random_r(&seed) %
			   (port_range.high + 1 - port_range.low) + port_range.low;

	for (i = port_range.low; i <= port_range.high; i++, rand_port_number++) {
		if (rand_port_number > port_range.high)
			rand_port_number = port_range.low;

		ofi_addr_set_port(src_addr, (uint16_t) rand_port_number);
		ret = bind(sock, src_addr, (socklen_t) addrlen);
		if (ret) {
			if (ofi_sockerr() == EADDRINUSE)
				continue;

			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"failed to bind listener: %s\n",
				strerror(ofi_sockerr()));
			return -ofi_sockerr();
		}
		break;
	}
	return (i <= port_range.high) ? FI_SUCCESS : -FI_EADDRNOTAVAIL;
}

static int tcpx_pep_sock_create(struct tcpx_pep *pep)
{
	int ret, af;

	switch (pep->info->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
	case FI_SOCKADDR_IN6:
		af = ((struct sockaddr *)pep->info->src_addr)->sa_family;
		break;
	default:
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"invalid source address format\n");
		return -FI_EINVAL;
	}

	pep->sock = ofi_socket(af, SOCK_STREAM, 0);
	if (pep->sock == INVALID_SOCKET) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"failed to create listener: %s\n",
			strerror(ofi_sockerr()));
		return -FI_EIO;
	}
	ret = tcpx_setup_socket(pep->sock, pep->info);
	if (ret)
		goto err;

	tcpx_set_zerocopy(pep->sock);
	ret = fi_fd_nonblock(pep->sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"failed to set listener socket to nonblocking\n");
		goto err;
	}

	if (ofi_addr_get_port(pep->info->src_addr) != 0 || port_range.high == 0) {
		ret = bind(pep->sock, pep->info->src_addr,
			  (socklen_t) pep->info->src_addrlen);
		if (ret)
			ret = -ofi_sockerr();
	} else {
		ret = tcpx_bind_to_port_range(pep->sock, pep->info->src_addr,
					      pep->info->src_addrlen);
	}

	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
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

static int tcpx_ep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct tcpx_ep *ep;
	size_t addrlen_in = *addrlen;
	int ret;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid);
	ret = ofi_getsockname(ep->bsock.sock, addr, (socklen_t *) addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen)? -FI_ETOOSMALL: FI_SUCCESS;
}

static int tcpx_ep_getpeer(struct fid_ep *ep_fid, void *addr, size_t *addrlen)
{
	struct tcpx_ep *ep;
	size_t addrlen_in = *addrlen;
	int ret;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	ret = ofi_getpeername(ep->bsock.sock, addr, (socklen_t *) addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen) ? -FI_ETOOSMALL: FI_SUCCESS;
}

static struct fi_ops_cm tcpx_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = tcpx_ep_getname,
	.getpeer = tcpx_ep_getpeer,
	.connect = tcpx_ep_connect,
	.listen = fi_no_listen,
	.accept = tcpx_ep_accept,
	.reject = fi_no_reject,
	.shutdown = tcpx_ep_shutdown,
	.join = fi_no_join,
};

void tcpx_reset_rx(struct tcpx_ep *ep)
{
	ep->cur_rx.handler = NULL;
	ep->cur_rx.entry = NULL;
	ep->cur_rx.hdr_done = 0;
	ep->cur_rx.hdr_len = sizeof(ep->cur_rx.hdr.base_hdr);
	OFI_DBG_SET(ep->cur_rx.hdr.base_hdr.version, 0);
}

static void tcpx_ep_cancel_rx(struct tcpx_ep *ep, void *context)
{
	struct slist_entry *cur, *prev;
	struct tcpx_xfer_entry *xfer_entry;
	struct tcpx_cq *cq;

	assert(ofi_mutex_held(&ep->lock));

	/* To cancel an active receive, we would need to flush the socket of
	 * all data associated with that message.  Since some of that data
	 * may not have arrived yet, this would require additional state
	 * tracking and complexity.  Fail the cancel in this case, since
	 * the receive is already in process anyway.
	 */
	slist_foreach(&ep->rx_queue, cur, prev) {
		xfer_entry = container_of(cur, struct tcpx_xfer_entry, entry);
		if (xfer_entry->context == context) {
			if (ep->cur_rx.entry != xfer_entry)
				goto found;
			break;
		}
	}

	return;

found:
	cq = container_of(ep->util_ep.rx_cq, struct tcpx_cq, util_cq);

	slist_remove(&ep->rx_queue, cur, prev);
	ep->rx_avail++;
	tcpx_cq_report_error(&cq->util_cq, xfer_entry, FI_ECANCELED);
	tcpx_free_xfer(cq, xfer_entry);
}

/* We currently only support canceling receives, which is the common case.
 * Canceling an operation from the other queues is not trivial,
 * especially if the operation has already been initiated.
 */
static ssize_t tcpx_ep_cancel(fid_t fid, void *context)
{
	struct tcpx_ep *ep;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);

	ofi_mutex_lock(&ep->lock);
	tcpx_ep_cancel_rx(ep, context);
	ofi_mutex_unlock(&ep->lock);

	return 0;
}

/* Hold progress lock around removing ep from progress list and
 * removing the socket from the pollfd set to ensure that all
 * progress is halted at once and we don't pick up any data.
 */
#define tcpx_halt_progress(ep, comp_obj)				\
	do {								\
		if (comp_obj) {						\
			ofi_mutex_lock(&comp_obj->ep_list_lock);	\
			fid_list_remove(&comp_obj->ep_list, NULL,	\
					&ep->util_ep.ep_fid.fid);	\
			ofi_wait_del_fd(comp_obj->wait, ep->bsock.sock);\
			ofi_mutex_unlock(&comp_obj->ep_list_lock);	\
			ofi_atomic_dec32(&comp_obj->ref);		\
		}							\
	} while (0)

static void tcpx_ep_halt(struct tcpx_ep *ep)
{
	struct tcpx_eq *eq;

	eq = ep->util_ep.eq ?
	     container_of(ep->util_ep.eq, struct tcpx_eq, util_eq) : NULL;

	/* eq->close_lock protects from processing stale connection events */
	if (eq)
		ofi_mutex_lock(&eq->close_lock);

	tcpx_halt_progress(ep, ep->util_ep.rx_cq);
	tcpx_halt_progress(ep, ep->util_ep.tx_cq);
	tcpx_halt_progress(ep, ep->util_ep.rx_cntr);
	tcpx_halt_progress(ep, ep->util_ep.tx_cntr);
	tcpx_halt_progress(ep, ep->util_ep.wr_cntr);
	tcpx_halt_progress(ep, ep->util_ep.rd_cntr);
	tcpx_halt_progress(ep, ep->util_ep.rem_wr_cntr);
	tcpx_halt_progress(ep, ep->util_ep.rem_rd_cntr);

	/* There's no ep_list with an EQ.  We only need to remove the
	 * socket from the pollfd set to disable progress.
	 */
	if (ep->util_ep.eq && ep->util_ep.eq->wait)
		ofi_wait_del_fd(ep->util_ep.eq->wait, ep->bsock.sock);

	if (eq)
		ofi_mutex_unlock(&eq->close_lock);
}

static int tcpx_ep_close(struct fid *fid)
{
	struct tcpx_ep *ep;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);

	tcpx_ep_halt(ep);

	/* Lock not technically needed, since we're freeing the EP.  But it's
	 * harmless to acquire and silences static code analysis tools.
	 */
	ofi_mutex_lock(&ep->lock);
	tcpx_ep_flush_all_queues(ep);
	ofi_mutex_unlock(&ep->lock);

	if (ep->util_ep.eq) {
		ofi_eq_remove_fid_events(ep->util_ep.eq,
					 &ep->util_ep.ep_fid.fid);
	}

	if (ep->fid && ep->fid->fclass == TCPX_CLASS_CM)
		tcpx_free_cm_ctx(ep->cm_ctx);

	ofi_close_socket(ep->bsock.sock);
	if (ep->util_ep.eq)
		ofi_atomic_dec32(&ep->util_ep.eq->ref);
	ofi_atomic_dec32(&ep->util_ep.domain->ref);
	ofi_mutex_destroy(&ep->util_ep.lock);
	ofi_mutex_destroy(&ep->lock);

	free(ep);
	return 0;
}

static int tcpx_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct tcpx_ep *ep;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);
	switch (command) {
	case FI_ENABLE:
		if ((ofi_needs_rx(ep->util_ep.caps) && !ep->util_ep.rx_cq) ||
		    (ofi_needs_tx(ep->util_ep.caps) && !ep->util_ep.tx_cq)) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"missing needed CQ binding\n");
			return -FI_ENOCQ;
		}
		break;
	default:
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "unsupported command\n");
		return -FI_ENOSYS;
	}
	return FI_SUCCESS;
}

static int tcpx_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct tcpx_ep *ep;
	struct tcpx_rx_ctx *rx_ctx;
	int ret;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);

	if (bfid->fclass == FI_CLASS_SRX_CTX) {
		rx_ctx = container_of(bfid, struct tcpx_rx_ctx, rx_fid.fid);
		ep->srx_ctx = rx_ctx;
		return FI_SUCCESS;
	}

	ret = ofi_ep_bind(&ep->util_ep, bfid, flags);
	if (!ret && (bfid->fclass == FI_CLASS_CNTR))
		ep->report_success = tcpx_report_cntr_success;

	return ret;
}

static struct fi_ops tcpx_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_ep_close,
	.bind = tcpx_ep_bind,
	.control = tcpx_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int tcpx_ep_getopt(fid_t fid, int level, int optname,
			  void *optval, size_t *optlen)
{
	struct tcpx_ep *ep;

	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (*optlen < sizeof(size_t)) {
			*optlen = sizeof(size_t);
			return -FI_ETOOSMALL;
		}
		ep = container_of(fid, struct tcpx_ep,
				  util_ep.ep_fid.fid);
		*((size_t *) optval) = ep->min_multi_recv_size;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_CM_DATA_SIZE:
		if (*optlen < sizeof(size_t)) {
			*optlen = sizeof(size_t);
			return -FI_ETOOSMALL;
		}
		*((size_t *) optval) = TCPX_MAX_CM_DATA_SIZE;
		*optlen = sizeof(size_t);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}
	return FI_SUCCESS;
}

int tcpx_ep_setopt(fid_t fid, int level, int optname,
		   const void *optval, size_t optlen)
{
	struct tcpx_ep *ep;

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);
	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		ep->min_multi_recv_size = *(size_t *) optval;
		FI_INFO(&tcpx_prov, FI_LOG_EP_CTRL,
			"FI_OPT_MIN_MULTI_RECV set to %zu\n",
			ep->min_multi_recv_size);
		break;
	case OFI_OPT_TCP_FI_ADDR:
		if (optlen != sizeof(fi_addr_t))
			return -FI_EINVAL;
		ep->src_addr = *(fi_addr_t *) optval;
		break;
	default:
		return -ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

static struct fi_ops_ep tcpx_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = tcpx_ep_cancel,
	.getopt = tcpx_ep_getopt,
	.setopt = tcpx_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int tcpx_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct tcpx_ep *ep;
	struct tcpx_pep *pep;
	struct tcpx_conn_handle *handle;
	int ret;

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = ofi_endpoint_init(domain, &tcpx_util_prov, info, &ep->util_ep,
				context, NULL);
	if (ret)
		goto err1;

	ofi_bsock_init(&ep->bsock, &tcpx_sockapi, tcpx_staging_sbuf_size,
		       tcpx_prefetch_rbuf_size);
	if (info->handle) {
		if (((fid_t) info->handle)->fclass == FI_CLASS_PEP) {
			pep = container_of(info->handle, struct tcpx_pep,
					   util_pep.pep_fid.fid);

			ep->bsock.sock = pep->sock;
			pep->sock = INVALID_SOCKET;
		} else {
			ep->state = TCPX_RCVD_REQ;
			handle = container_of(info->handle,
					      struct tcpx_conn_handle, fid);
			/* EP now owns socket */
			ep->bsock.sock = handle->sock;
			handle->sock = INVALID_SOCKET;
			ep->hdr_bswap = handle->endian_match ?
					tcpx_hdr_none : tcpx_hdr_bswap;
			/* Save handle, but we only free if user calls accept.
			 * Otherwise, user will call reject, which will free it.
			 */
			ep->handle = handle;

			ret = tcpx_setup_socket(ep->bsock.sock, info);
			if (ret)
				goto err3;
		}
	} else {
		ep->bsock.sock = ofi_socket(ofi_get_sa_family(info), SOCK_STREAM, 0);
		if (ep->bsock.sock == INVALID_SOCKET) {
			ret = -ofi_sockerr();
			goto err2;
		}

		ret = tcpx_setup_socket(ep->bsock.sock, info);
		if (ret)
			goto err3;

		tcpx_set_zerocopy(ep->bsock.sock);

		if (info->src_addr && (!ofi_is_any_addr(info->src_addr) ||
					ofi_addr_get_port(info->src_addr))) {

			if (!ofi_addr_get_port(info->src_addr)) {
				tcpx_set_no_port(ep->bsock.sock);
			}

			ret = bind(ep->bsock.sock, info->src_addr,
				(socklen_t) info->src_addrlen);
			if (ret) {
				FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "bind failed\n");
				ret = -ofi_sockerr();
				goto err3;
			}
		}
	}

	ret = ofi_mutex_init(&ep->lock);
	if (ret)
		goto err3;

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
	ep->min_multi_recv_size = TCPX_MIN_MULTI_RECV;
	tcpx_config_bsock(&ep->bsock);
	ep->report_success = tcpx_report_success;

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &tcpx_ep_fi_ops;
	(*ep_fid)->ops = &tcpx_ep_ops;
	(*ep_fid)->cm = &tcpx_cm_ops;
	(*ep_fid)->msg = &tcpx_msg_ops;
	(*ep_fid)->rma = &tcpx_rma_ops;
	(*ep_fid)->tagged = &tcpx_tagged_ops;

	return 0;
err3:
	ofi_close_socket(ep->bsock.sock);
err2:
	ofi_endpoint_close(&ep->util_ep);
err1:
	free(ep);
	return ret;
}

static int tcpx_pep_fi_close(struct fid *fid)
{
	struct tcpx_pep *pep;

	pep = container_of(fid, struct tcpx_pep, util_pep.pep_fid.fid);
	if (pep->util_pep.eq)
		ofi_wait_del_fd(pep->util_pep.eq->wait, pep->sock);

	ofi_close_socket(pep->sock);
	ofi_pep_close(&pep->util_pep);
	fi_freeinfo(pep->info);
	free(pep);
	return 0;
}

static int tcpx_pep_fi_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct tcpx_pep *pep;

	pep = container_of(fid, struct tcpx_pep, util_pep.pep_fid.fid);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		return ofi_pep_bind_eq(&pep->util_pep,
				       container_of(bfid, struct util_eq,
						    eq_fid.fid), flags);
	default:
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"invalid FID class for binding\n");
		return -FI_EINVAL;
	}
}

static struct fi_ops tcpx_pep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_pep_fi_close,
	.bind = tcpx_pep_fi_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int tcpx_pep_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct tcpx_pep *pep;

	if ((addrlen != sizeof(struct sockaddr_in)) &&
	    (addrlen != sizeof(struct sockaddr_in6)))
		return -FI_EINVAL;

	pep = container_of(fid, struct tcpx_pep,
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

	return tcpx_pep_sock_create(pep);
}

static int tcpx_pep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct tcpx_pep *pep;
	size_t addrlen_in = *addrlen;
	int ret;

	pep = container_of(fid, struct tcpx_pep, util_pep.pep_fid);
	ret = ofi_getsockname(pep->sock, addr, (socklen_t *) addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen) ? -FI_ETOOSMALL: FI_SUCCESS;
}

static int tcpx_pep_listen(struct fid_pep *pep_fid)
{
	struct tcpx_pep *pep;
	int ret;

	pep = container_of(pep_fid, struct tcpx_pep, util_pep.pep_fid);

	/* arbitrary backlog value to support larger scale jobs */
	if (listen(pep->sock, 4096)) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"socket listen failed\n");
		return -ofi_sockerr();
	}

	ret = ofi_wait_add_fd(pep->util_pep.eq->wait, pep->sock,
			      POLLIN, tcpx_eq_wait_try_func,
			      NULL, &pep->cm_ctx);

	return ret;
}

static int tcpx_pep_reject(struct fid_pep *pep, fid_t fid_handle,
			   const void *param, size_t paramlen)
{
	struct tcpx_cm_msg msg;
	struct tcpx_conn_handle *handle;
	ssize_t size_ret;
	int ret;

	handle = container_of(fid_handle, struct tcpx_conn_handle, fid);
	/* If we created an endpoint, it owns the socket */
	if (handle->sock == INVALID_SOCKET)
		goto free;

	memset(&msg.hdr, 0, sizeof(msg.hdr));
	msg.hdr.version = TCPX_CTRL_HDR_VERSION;
	msg.hdr.type = ofi_ctrl_nack;
	msg.hdr.seg_size = htons((uint16_t) paramlen);
	if (paramlen)
		memcpy(&msg.data, param, paramlen);

	size_ret = ofi_send_socket(handle->sock, &msg,
				   sizeof(msg.hdr) + paramlen, MSG_NOSIGNAL);
	if ((size_t) size_ret != sizeof(msg.hdr) + paramlen)
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"sending of reject message failed\n");

	ofi_shutdown(handle->sock, SHUT_RDWR);
	ret = ofi_close_socket(handle->sock);
	if (ret)
		return ret;

free:
	free(handle);
	return FI_SUCCESS;
}

static struct fi_ops_cm tcpx_pep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = tcpx_pep_setname,
	.getname = tcpx_pep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = tcpx_pep_listen,
	.accept = fi_no_accept,
	.reject = tcpx_pep_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static int  tcpx_pep_getopt(fid_t fid, int level, int optname,
			    void *optval, size_t *optlen)
{
	if ( level != FI_OPT_ENDPOINT ||
	     optname != FI_OPT_CM_DATA_SIZE)
		return -FI_ENOPROTOOPT;

	if (*optlen < sizeof(size_t)) {
		*optlen = sizeof(size_t);
		return -FI_ETOOSMALL;
	}

	*((size_t *) optval) = TCPX_MAX_CM_DATA_SIZE;
	*optlen = sizeof(size_t);
	return FI_SUCCESS;
}

static struct fi_ops_ep tcpx_pep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.getopt = tcpx_pep_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int tcpx_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_pep **pep_fid, void *context)
{
	struct tcpx_pep *pep;
	int ret;

	if (!info) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,"invalid info\n");
		return -FI_EINVAL;
	}

	ret = ofi_prov_check_info(&tcpx_util_prov, fabric->api_version, info);
	if (ret)
		return ret;

	pep = calloc(1, sizeof(*pep));
	if (!pep)
		return -FI_ENOMEM;

	ret = ofi_pep_init(fabric, info, &pep->util_pep, context);
	if (ret)
		goto err1;

	pep->util_pep.pep_fid.fid.ops = &tcpx_pep_fi_ops;
	pep->util_pep.pep_fid.cm = &tcpx_pep_cm_ops;
	pep->util_pep.pep_fid.ops = &tcpx_pep_ops;

	pep->info = fi_dupinfo(info);
	if (!pep->info) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	pep->cm_ctx.fid.fclass = TCPX_CLASS_CM;
	pep->cm_ctx.hfid = &pep->util_pep.pep_fid.fid;
	pep->cm_ctx.state = TCPX_CM_LISTENING;
	pep->cm_ctx.cm_data_sz = 0;
	pep->sock = INVALID_SOCKET;

	if (info->src_addr) {
		ret = tcpx_pep_sock_create(pep);
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
