/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
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

void tcpx_hdr_none(struct tcpx_base_hdr *hdr)
{
	/* no-op */
}

void tcpx_hdr_bswap(struct tcpx_base_hdr *hdr)
{
	struct ofi_rma_iov *rma_iov;
	uint8_t *ptr = (uint8_t *)hdr + sizeof(*hdr);
	int i;

	hdr->flags = ntohs(hdr->flags);
	hdr->size = ntohll(hdr->size);

	if (hdr->flags & OFI_REMOTE_CQ_DATA) {
		*((uint64_t *)ptr) = ntohll(*((uint64_t *) ptr));
		ptr += sizeof(uint64_t);
	}

	rma_iov = (struct ofi_rma_iov *)ptr;
	for ( i = 0; i < hdr->rma_iov_cnt; i++) {
		rma_iov[i].addr = ntohll(rma_iov[i].addr);
		rma_iov[i].len = ntohll(rma_iov[i].len);
		rma_iov[i].key = ntohll(rma_iov[i].key);
	}
}

static int tcpx_setup_socket(SOCKET sock)
{
	int ret, optval = 1;

	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval,
			 sizeof(optval));
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,"setsockopt reuseaddr failed\n");
		return ret;
	}

	ret = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &optval,
			 sizeof(optval));
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,"setsockopt nodelay failed\n");
		return ret;
	}

	return ret;
}

static int tcpx_ep_connect(struct fid_ep *ep, const void *addr,
			   const void *param, size_t paramlen)
{
	struct tcpx_ep *tcpx_ep = container_of(ep, struct tcpx_ep, util_ep.ep_fid);
	struct tcpx_cm_context *cm_ctx;
	int ret;

	if (!addr || !tcpx_ep->sock || paramlen > TCPX_MAX_CM_DATA_SIZE)
		return -FI_EINVAL;

	cm_ctx = calloc(1, sizeof(*cm_ctx));
	if (!cm_ctx) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory \n");
		return -FI_ENOMEM;
	}

	ret = connect(tcpx_ep->sock, (struct sockaddr *) addr,
		      (socklen_t) ofi_sizeofaddr(addr));
	if (ret && ofi_sockerr() != FI_EINPROGRESS) {
		ret =  -ofi_sockerr();
		goto err;
	}

	cm_ctx->fid = &tcpx_ep->util_ep.ep_fid.fid;
	cm_ctx->type = CLIENT_SEND_CONNREQ;

	if (paramlen) {
		cm_ctx->cm_data_sz = paramlen;
		memcpy(cm_ctx->cm_data, param, paramlen);
	}

	ret = ofi_wait_add_fd(tcpx_ep->util_ep.eq->wait, tcpx_ep->sock,
			      POLLOUT, tcpx_eq_wait_try_func, NULL,cm_ctx);
	if (ret)
		goto err;

	return 0;
err:
	free(cm_ctx);
	return ret;
}

static int tcpx_ep_accept(struct fid_ep *ep, const void *param, size_t paramlen)
{
	struct tcpx_ep *tcpx_ep = container_of(ep, struct tcpx_ep, util_ep.ep_fid);
	struct tcpx_cm_context *cm_ctx;
	int ret;

	if (tcpx_ep->sock == INVALID_SOCKET)
		return -FI_EINVAL;

	cm_ctx = calloc(1, sizeof(*cm_ctx));
	if (!cm_ctx) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory \n");
		return -FI_ENOMEM;
	}

	cm_ctx->fid = &tcpx_ep->util_ep.ep_fid.fid;
	cm_ctx->type = SERVER_SEND_CM_ACCEPT;
	if (paramlen) {
		cm_ctx->cm_data_sz = paramlen;
		memcpy(cm_ctx->cm_data, param, paramlen);
	}

	ret = ofi_wait_add_fd(tcpx_ep->util_ep.eq->wait, tcpx_ep->sock,
			      POLLOUT, tcpx_eq_wait_try_func, NULL, cm_ctx);
	if (ret)
		free(cm_ctx);

	return ret;
}

static int tcpx_ep_shutdown(struct fid_ep *ep, uint64_t flags)
{
	struct tcpx_ep *tcpx_ep;
	int ret;

	tcpx_ep = container_of(ep, struct tcpx_ep, util_ep.ep_fid);

	ret = ofi_shutdown(tcpx_ep->sock, SHUT_RDWR);
	if (ret && ofi_sockerr() != ENOTCONN) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA, "ep shutdown unsuccessful\n");
	}

	fastlock_acquire(&tcpx_ep->lock);
	ret = tcpx_ep_shutdown_report(tcpx_ep, &ep->fid);
	fastlock_release(&tcpx_ep->lock);
	if (ret)
		FI_WARN(&tcpx_prov, FI_LOG_EP_DATA, "Error writing to EQ\n");

	return ret;
}

static int tcpx_bind_to_port_range(SOCKET sock, void* src_addr, size_t addrlen)
{
	int ret, i, rand_port_number;

	rand_port_number = rand() % (port_range.high + 1 - port_range.low) +
			   port_range.low;

	for (i = port_range.low; i <= port_range.high; i++, rand_port_number++) {
		if (rand_port_number > port_range.high)
			rand_port_number = port_range.low;

		ofi_addr_set_port(src_addr, rand_port_number);
		ret = bind(sock, src_addr, (socklen_t) addrlen);
		if (ret) {
			if (errno == EADDRINUSE)
				continue;

			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"failed to bind listener: %s\n",
				strerror(ofi_sockerr()));
			return -errno;
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
	ret = tcpx_setup_socket(pep->sock);
	if (ret) {
		goto err;
	}
	if (ofi_addr_get_port(pep->info->src_addr) != 0 || port_range.high == 0)
		ret = bind(pep->sock, pep->info->src_addr,
			  (socklen_t) pep->info->src_addrlen);
	else
		ret = tcpx_bind_to_port_range(pep->sock, pep->info->src_addr,
					      pep->info->src_addrlen);

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
	struct tcpx_ep *tcpx_ep;
	size_t addrlen_in = *addrlen;
	int ret;

	tcpx_ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid);
	ret = ofi_getsockname(tcpx_ep->sock, addr, (socklen_t *)addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen)? -FI_ETOOSMALL: FI_SUCCESS;
}

static int tcpx_ep_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen)
{
	struct tcpx_ep *tcpx_ep;
	size_t addrlen_in = *addrlen;
	int ret;

	tcpx_ep = container_of(ep, struct tcpx_ep, util_ep.ep_fid);
	ret = ofi_getpeername(tcpx_ep->sock, addr, (socklen_t *)addrlen);
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

void tcpx_rx_msg_release(struct tcpx_xfer_entry *rx_entry)
{
	struct tcpx_cq *tcpx_cq;

	assert(rx_entry->hdr.base_hdr.op_data == TCPX_OP_MSG_RECV);

	if (rx_entry->ep->srx_ctx) {
		tcpx_srx_xfer_release(rx_entry->ep->srx_ctx, rx_entry);
	} else {
		tcpx_cq = container_of(rx_entry->ep->util_ep.rx_cq,
				       struct tcpx_cq, util_cq);
		tcpx_xfer_entry_release(tcpx_cq, rx_entry);
	}
}

static void tcpx_ep_release_queue(struct slist *queue,
				  struct tcpx_cq *tcpx_cq)
{
	struct tcpx_xfer_entry *xfer_entry;

	while (!slist_empty(queue)) {
		xfer_entry = container_of(queue->head, struct tcpx_xfer_entry,
					  entry);
		slist_remove_head(queue);
		tcpx_cq_report_error(&tcpx_cq->util_cq, xfer_entry, FI_ECANCELED);
		tcpx_xfer_entry_release(tcpx_cq, xfer_entry);
	}
}

static void tcpx_ep_tx_rx_queues_release(struct tcpx_ep *ep)
{
	struct tcpx_cq *tcpx_cq;

	fastlock_acquire(&ep->lock);
	tcpx_cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);
	tcpx_ep_release_queue(&ep->tx_queue, tcpx_cq);
	tcpx_ep_release_queue(&ep->rma_read_queue, tcpx_cq);
	tcpx_ep_release_queue(&ep->tx_rsp_pend_queue, tcpx_cq);

	tcpx_cq = container_of(ep->util_ep.rx_cq, struct tcpx_cq, util_cq);
	tcpx_ep_release_queue(&ep->rx_queue, tcpx_cq);
	fastlock_release(&ep->lock);
}

/**
 * Release the ep from polling
 */
void tcpx_ep_wait_fd_del(struct tcpx_ep *ep)
{
	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "releasing ep=%p\n", ep);

	struct tcpx_eq *eq;

	eq = container_of(ep->util_ep.eq, struct tcpx_eq,
			  util_eq);

	/* eq->close_lock protects from processing stale connection events */
	fastlock_acquire(&eq->close_lock);
	if (ep->util_ep.rx_cq)
		ofi_wait_del_fd(ep->util_ep.rx_cq->wait, ep->sock);

	if (ep->util_ep.tx_cq)
		ofi_wait_del_fd(ep->util_ep.tx_cq->wait, ep->sock);

	if (ep->util_ep.eq->wait)
		ofi_wait_del_fd(ep->util_ep.eq->wait, ep->sock);

	fastlock_release(&eq->close_lock);
}

static int tcpx_ep_close(struct fid *fid)
{
	struct tcpx_ep *ep = container_of(fid, struct tcpx_ep,
					  util_ep.ep_fid.fid);

	tcpx_ep_tx_rx_queues_release(ep);

	tcpx_ep_wait_fd_del(ep); /* ensure that everything is really released */

	ofi_eq_remove_fid_events(ep->util_ep.eq, &ep->util_ep.ep_fid.fid);
	ofi_close_socket(ep->sock);
	ofi_endpoint_close(&ep->util_ep);
	fastlock_destroy(&ep->lock);

	free(ep);
	return 0;
}

static int tcpx_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct tcpx_ep *ep;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);
	switch (command) {
	case FI_ENABLE:
		if (!ep->util_ep.rx_cq || !ep->util_ep.tx_cq)
			return -FI_ENOCQ;
		break;
	default:
		return -FI_ENOSYS;
	}
	return FI_SUCCESS;
}

static int tcpx_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct tcpx_ep *tcpx_ep;
	struct tcpx_rx_ctx *rx_ctx;

	tcpx_ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);

	if (bfid->fclass == FI_CLASS_SRX_CTX) {
		rx_ctx = container_of(bfid, struct tcpx_rx_ctx, rx_fid.fid);
		tcpx_ep->srx_ctx = rx_ctx;
		return FI_SUCCESS;
	}

	return ofi_ep_bind(&tcpx_ep->util_ep, bfid, flags);
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

	if (level != FI_OPT_ENDPOINT ||
	    optname != FI_OPT_MIN_MULTI_RECV)
		return -ENOPROTOOPT;

	if (optlen != sizeof(size_t))
		return -FI_EINVAL;

	ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);
	ep->min_multi_recv_size = *(size_t *) optval;

	FI_INFO(&tcpx_prov, FI_LOG_EP_CTRL,
		"FI_OPT_MIN_MULTI_RECV set to %zu\n", ep->min_multi_recv_size);
	return FI_SUCCESS;
}

static struct fi_ops_ep tcpx_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
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

	if (info->handle) {
		if (((fid_t) info->handle)->fclass == FI_CLASS_PEP) {
			pep = container_of(info->handle, struct tcpx_pep,
					   util_pep.pep_fid.fid);

			ep->sock = pep->sock;
			pep->sock = INVALID_SOCKET;
		} else {
			handle = container_of(info->handle,
					      struct tcpx_conn_handle, handle);
			ep->sock = handle->sock;
			ep->hdr_bswap = handle->endian_match ?
					tcpx_hdr_none : tcpx_hdr_bswap;
			free(handle);

			ret = tcpx_setup_socket(ep->sock);
			if (ret)
				goto err3;
		}
	} else {
		ep->sock = ofi_socket(ofi_get_sa_family(info), SOCK_STREAM, 0);
		if (ep->sock == INVALID_SOCKET) {
			ret = -ofi_sockerr();
			goto err2;
		}

		ret = tcpx_setup_socket(ep->sock);
		if (ret)
			goto err3;
	}

	ep->cm_state = TCPX_EP_CONNECTING;
	ret = fastlock_init(&ep->lock);
	if (ret)
		goto err3;

	ep->stage_buf.size = STAGE_BUF_SIZE;
	ep->stage_buf.len = 0;
	ep->stage_buf.off = 0;

	slist_init(&ep->rx_queue);
	slist_init(&ep->tx_queue);
	slist_init(&ep->rma_read_queue);
	slist_init(&ep->tx_rsp_pend_queue);

	ep->cur_rx_msg.done_len = 0;
	ep->cur_rx_msg.hdr_len = sizeof(ep->cur_rx_msg.hdr.base_hdr);
	ep->min_multi_recv_size = TCPX_MIN_MULTI_RECV;

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &tcpx_ep_fi_ops;
	(*ep_fid)->ops = &tcpx_ep_ops;
	(*ep_fid)->cm = &tcpx_cm_ops;
	(*ep_fid)->msg = &tcpx_msg_ops;
	(*ep_fid)->rma = &tcpx_rma_ops;

	ep->get_rx_entry[ofi_op_msg] = tcpx_get_rx_entry_op_msg;
	ep->get_rx_entry[ofi_op_tagged] = tcpx_get_rx_entry_op_invalid;
	ep->get_rx_entry[ofi_op_read_req] = tcpx_get_rx_entry_op_read_req;
	ep->get_rx_entry[ofi_op_read_rsp] = tcpx_get_rx_entry_op_read_rsp;
	ep->get_rx_entry[ofi_op_write] = tcpx_get_rx_entry_op_write;
	return 0;
err3:
	ofi_close_socket(ep->sock);
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
	struct tcpx_pep *tcpx_pep = container_of(fid, struct tcpx_pep,
						 util_pep.pep_fid.fid);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		return ofi_pep_bind_eq(&tcpx_pep->util_pep,
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
	struct tcpx_pep *tcpx_pep;

	if ((addrlen != sizeof(struct sockaddr_in)) &&
	    (addrlen != sizeof(struct sockaddr_in6)))
		return -FI_EINVAL;

	tcpx_pep = container_of(fid, struct tcpx_pep,
				util_pep.pep_fid);

	if (tcpx_pep->sock != INVALID_SOCKET) {
		ofi_close_socket(tcpx_pep->sock);
		tcpx_pep->sock = INVALID_SOCKET;
	}

	if (tcpx_pep->info->src_addr) {
		free(tcpx_pep->info->src_addr);
		tcpx_pep->info->src_addrlen = 0;
	}

	tcpx_pep->info->src_addr = mem_dup(addr, addrlen);
	if (!tcpx_pep->info->src_addr)
		return -FI_ENOMEM;
	tcpx_pep->info->src_addrlen = addrlen;

	return tcpx_pep_sock_create(tcpx_pep);
}

static int tcpx_pep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct tcpx_pep *tcpx_pep;
	size_t addrlen_in = *addrlen;
	int ret;

	tcpx_pep = container_of(fid, struct tcpx_pep, util_pep.pep_fid);
	ret = ofi_getsockname(tcpx_pep->sock, addr, (socklen_t *)addrlen);
	if (ret)
		return -ofi_sockerr();

	return (addrlen_in < *addrlen) ? -FI_ETOOSMALL: FI_SUCCESS;
}

static int tcpx_pep_listen(struct fid_pep *pep)
{
	struct tcpx_pep *tcpx_pep;
	int ret;

	tcpx_pep = container_of(pep,struct tcpx_pep, util_pep.pep_fid);

	if (listen(tcpx_pep->sock, SOMAXCONN)) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"socket listen failed\n");
		return -ofi_sockerr();
	}

	ret = ofi_wait_add_fd(tcpx_pep->util_pep.eq->wait, tcpx_pep->sock,
			      POLLIN, tcpx_eq_wait_try_func,
			      NULL, &tcpx_pep->cm_ctx);

	return ret;
}

static int tcpx_pep_reject(struct fid_pep *pep, fid_t handle,
			   const void *param, size_t paramlen)
{
	struct ofi_ctrl_hdr hdr;
	struct tcpx_conn_handle *tcpx_handle;
	int ret;

	tcpx_handle = container_of(handle, struct tcpx_conn_handle, handle);

	memset(&hdr, 0, sizeof(hdr));
	hdr.version = TCPX_CTRL_HDR_VERSION;
	hdr.type = ofi_ctrl_nack;
	hdr.seg_size = htons((uint16_t) paramlen);

	ret = ofi_send_socket(tcpx_handle->sock, &hdr,
			      sizeof(hdr), MSG_NOSIGNAL);

	if ((ret == sizeof(hdr)) && paramlen)
		(void) ofi_send_socket(tcpx_handle->sock, param,
				       paramlen, MSG_NOSIGNAL);

	ofi_shutdown(tcpx_handle->sock, SHUT_RDWR);
	ret = ofi_close_socket(tcpx_handle->sock);
	if (ret)
		return ret;

	free(tcpx_handle);
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
		    struct fid_pep **pep, void *context)
{
	struct tcpx_pep *_pep;
	int ret;

	if (!info) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,"invalid info\n");
		return -FI_EINVAL;
	}

	ret = ofi_check_info(&tcpx_util_prov, tcpx_util_prov.info,
			     fabric->api_version, info);
	if (ret)
		return ret;

	_pep = calloc(1, sizeof(*_pep));
	if (!_pep)
		return -FI_ENOMEM;

	ret = ofi_pep_init(fabric, info, &_pep->util_pep, context);
	if (ret)
		goto err1;

	_pep->util_pep.pep_fid.fid.ops = &tcpx_pep_fi_ops;
	_pep->util_pep.pep_fid.cm = &tcpx_pep_cm_ops;
	_pep->util_pep.pep_fid.ops = &tcpx_pep_ops;

	_pep->info = fi_dupinfo(info);
	if (!_pep->info)
		goto err2;

	_pep->cm_ctx.fid = &_pep->util_pep.pep_fid.fid;
	_pep->cm_ctx.type = SERVER_SOCK_ACCEPT;
	_pep->cm_ctx.cm_data_sz = 0;
	_pep->sock = INVALID_SOCKET;

	if (info->src_addr) {
		ret = tcpx_pep_sock_create(_pep);
		if (ret)
			goto err3;
	}

	*pep = &_pep->util_pep.pep_fid;
	return FI_SUCCESS;
err3:
	fi_freeinfo(_pep->info);
err2:
	ofi_pep_close(&_pep->util_pep);
err1:
	free(_pep);
	return ret;
}
