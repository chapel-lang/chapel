/*
 * Copyright (c) 2017-2020 Intel Corporation. All rights reserved.
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
#include "tcpx.h"
#include <poll.h>
#include <sys/types.h>
#include <ofi_util.h>


static int rx_cm_data(SOCKET fd, struct ofi_ctrl_hdr *hdr,
		      int type, struct tcpx_cm_context *cm_ctx)
{
	size_t data_size = 0;
	ssize_t ret;

	ret = ofi_recv_socket(fd, hdr, sizeof(*hdr), MSG_WAITALL);
	if (ret != sizeof(*hdr)) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Failed to read cm header\n");
		ret = ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
		goto out;
	}

	if (hdr->version != TCPX_CTRL_HDR_VERSION) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cm protocol version mismatch\n");
		ret = -FI_ENOPROTOOPT;
		goto out;
	}

	if (hdr->type != type) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"unexpected cm message type\n");
		ret = -FI_ECONNREFUSED;
		goto out;
	}

	data_size = MIN(ntohs(hdr->seg_size), TCPX_MAX_CM_DATA_SIZE);
	if (data_size) {
		ret = ofi_recv_socket(fd, cm_ctx->cm_data, data_size,
				      MSG_WAITALL);
		if ((size_t) ret != data_size) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to read cm data\n");
			ret = ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
			data_size = 0;
			goto out;
		}

		if (ntohs(hdr->seg_size) > TCPX_MAX_CM_DATA_SIZE) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Discarding unexpected cm data\n");
			ofi_discard_socket(fd, ntohs(hdr->seg_size) -
					   TCPX_MAX_CM_DATA_SIZE);
		}
	}
	ret = 0;
out:
	cm_ctx->cm_data_sz = data_size;
	return ret;
}

static int tx_cm_data(SOCKET fd, uint8_t type, struct tcpx_cm_context *cm_ctx)
{
	struct ofi_ctrl_hdr hdr;
	ssize_t ret;

	memset(&hdr, 0, sizeof(hdr));
	hdr.version = TCPX_CTRL_HDR_VERSION;
	hdr.type = type;
	hdr.seg_size = htons((uint16_t) cm_ctx->cm_data_sz);
	hdr.conn_data = 1; /* For testing endianess mismatch at peer */

	ret = ofi_send_socket(fd, &hdr, sizeof(hdr), MSG_NOSIGNAL);
	if (ret != sizeof(hdr))
		goto err;

	if (cm_ctx->cm_data_sz) {
		ret = ofi_send_socket(fd, cm_ctx->cm_data,
				      cm_ctx->cm_data_sz, MSG_NOSIGNAL);
		if ((size_t) ret != cm_ctx->cm_data_sz)
			goto err;
	}

	return FI_SUCCESS;
err:
	return ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
}

static int tcpx_ep_enable_xfers(struct tcpx_ep *ep)
{
	int ret;

	fastlock_acquire(&ep->lock);
	if (ep->cm_state != TCPX_EP_CONNECTING) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"ep is in invalid state\n");
		ret = -FI_EINVAL;
		goto unlock;
	}

	ret = fi_fd_nonblock(ep->sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"failed to set socket to nonblocking\n");
		goto unlock;
	}
	ep->cm_state = TCPX_EP_CONNECTED;
	fastlock_release(&ep->lock);

	if (ep->util_ep.rx_cq) {
		ret = ofi_wait_add_fd(ep->util_ep.rx_cq->wait,
				      ep->sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
	}

	if (ep->util_ep.tx_cq) {
		ret = ofi_wait_add_fd(ep->util_ep.tx_cq->wait,
				      ep->sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
	}

	return ret;
unlock:
	fastlock_release(&ep->lock);
	return ret;
}

static int proc_conn_resp(struct tcpx_cm_context *cm_ctx,
			  struct tcpx_ep *ep)
{
	struct ofi_ctrl_hdr conn_resp;
	struct fi_eq_cm_entry *cm_entry;
	ssize_t len;
	int ret = FI_SUCCESS;

	ret = rx_cm_data(ep->sock, &conn_resp, ofi_ctrl_connresp, cm_ctx);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Failed to receive connect response\n");
		return ret;
	}

	cm_entry = calloc(1, sizeof(*cm_entry) + cm_ctx->cm_data_sz);
	if (!cm_entry)
		return -FI_ENOMEM;

	cm_entry->fid = cm_ctx->fid;
	memcpy(cm_entry->data, cm_ctx->cm_data, cm_ctx->cm_data_sz);

	ep->hdr_bswap = (conn_resp.conn_data == 1) ?
			tcpx_hdr_none : tcpx_hdr_bswap;

	ret = tcpx_ep_enable_xfers(ep);
	if (ret)
		goto err;

	len = fi_eq_write(&ep->util_ep.eq->eq_fid, FI_CONNECTED, cm_entry,
			  sizeof(*cm_entry) + cm_ctx->cm_data_sz, 0);
	if (len < 0)
		ret = (int) len;

err:
	free(cm_entry);
	return ret;
}

int tcpx_eq_wait_try_func(void *arg)
{
	return FI_SUCCESS;
}

static void client_recv_connresp(struct util_wait *wait,
				 struct tcpx_cm_context *cm_ctx)
{
	struct fi_eq_err_entry err_entry;
	struct tcpx_ep *ep;
	ssize_t ret;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Handling accept from server\n");
	assert(cm_ctx->fid->fclass == FI_CLASS_EP);
	ep = container_of(cm_ctx->fid, struct tcpx_ep, util_ep.ep_fid.fid);

	ret = ofi_wait_del_fd(wait, ep->sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Could not remove fd from wait\n");
		goto err;
	}

	ret = proc_conn_resp(cm_ctx, ep);
	if (ret)
		goto err;

	free(cm_ctx);
	return;
err:
	memset(&err_entry, 0, sizeof err_entry);
	err_entry.fid = cm_ctx->fid;
	err_entry.context = cm_ctx->fid->context;
	err_entry.err = -ret;
	if (cm_ctx->cm_data_sz) {
		err_entry.err_data = calloc(1, cm_ctx->cm_data_sz);
		if (err_entry.err_data) {
			memcpy(err_entry.err_data, cm_ctx->cm_data,
			       cm_ctx->cm_data_sz);
			err_entry.err_data_size = cm_ctx->cm_data_sz;
		}
	}
	free(cm_ctx);

	/* `err_entry.err_data` must live until it is passed to user */
	ret = fi_eq_write(&ep->util_ep.eq->eq_fid, FI_SHUTDOWN,
			  &err_entry, sizeof(err_entry), UTIL_FLAG_ERROR);
	if (ret < 0)
		free(err_entry.err_data);
}

static void server_send_cm_accept(struct util_wait *wait,
				  struct tcpx_cm_context *cm_ctx)
{
	struct fi_eq_cm_entry cm_entry = {0};
	struct fi_eq_err_entry err_entry;
	struct tcpx_ep *ep;
	int ret;

	assert(cm_ctx->fid->fclass == FI_CLASS_EP);
	ep = container_of(cm_ctx->fid, struct tcpx_ep, util_ep.ep_fid.fid);

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Send connect (accept) response\n");
	ret = tx_cm_data(ep->sock, ofi_ctrl_connresp, cm_ctx);
	if (ret)
		goto err;

	cm_entry.fid =  cm_ctx->fid;
	ret = (int) fi_eq_write(&ep->util_ep.eq->eq_fid, FI_CONNECTED,
				&cm_entry, sizeof(cm_entry), 0);
	if (ret < 0)
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");

	ret = ofi_wait_del_fd(wait, ep->sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Could not remove fd from wait\n");
		goto err;
	}

	ret = tcpx_ep_enable_xfers(ep);
	if (ret)
		goto err;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Connection Accept Successful\n");
	free(cm_ctx);
	return;
err:
	memset(&err_entry, 0, sizeof err_entry);
	err_entry.fid = cm_ctx->fid;
	err_entry.context = cm_ctx->fid->context;
	err_entry.err = -ret;

	free(cm_ctx);
	fi_eq_write(&ep->util_ep.eq->eq_fid, FI_SHUTDOWN,
		    &err_entry, sizeof(err_entry), UTIL_FLAG_ERROR);
}

static void server_recv_connreq(struct util_wait *wait,
				struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_conn_handle *handle;
	struct fi_eq_cm_entry *cm_entry;
	struct ofi_ctrl_hdr conn_req;
	socklen_t len;
	int ret;

	assert(cm_ctx->fid->fclass == FI_CLASS_CONNREQ);
	handle  = container_of(cm_ctx->fid, struct tcpx_conn_handle, handle);

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Server receive connect request\n");
	ret = rx_cm_data(handle->sock, &conn_req, ofi_ctrl_connreq, cm_ctx);
	if (ret)
		goto err1;

	cm_entry = calloc(1, sizeof(*cm_entry) + cm_ctx->cm_data_sz);
	if (!cm_entry)
		goto err1;

	cm_entry->fid = &handle->pep->util_pep.pep_fid.fid;
	cm_entry->info = fi_dupinfo(handle->pep->info);
	if (!cm_entry->info)
		goto err2;

	len = cm_entry->info->dest_addrlen = handle->pep->info->src_addrlen;
	cm_entry->info->dest_addr = malloc(len);
	if (!cm_entry->info->dest_addr)
		goto err3;

	ret = ofi_getpeername(handle->sock, cm_entry->info->dest_addr, &len);
	if (ret)
		goto err3;

	handle->endian_match = (conn_req.conn_data == 1);
	cm_entry->info->handle = &handle->handle;
	memcpy(cm_entry->data, cm_ctx->cm_data, cm_ctx->cm_data_sz);

	ret = (int) fi_eq_write(&handle->pep->util_pep.eq->eq_fid, FI_CONNREQ, cm_entry,
				sizeof(*cm_entry) + cm_ctx->cm_data_sz, 0);
	if (ret < 0) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");
		goto err3;
	}
	ret = ofi_wait_del_fd(wait, handle->sock);
	if (ret)
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"fd deletion from ofi_wait failed\n");
	free(cm_entry);
	free(cm_ctx);
	return;
err3:
	fi_freeinfo(cm_entry->info);
err2:
	free(cm_entry);
err1:
	ofi_wait_del_fd(wait, handle->sock);
	ofi_close_socket(handle->sock);
	free(cm_ctx);
	free(handle);
}

static void client_send_connreq(struct util_wait *wait,
				struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_ep *ep;
	struct fi_eq_err_entry err_entry;
	socklen_t len;
	int status, ret = FI_SUCCESS;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "client send connreq\n");
	assert(cm_ctx->fid->fclass == FI_CLASS_EP);

	ep = container_of(cm_ctx->fid, struct tcpx_ep, util_ep.ep_fid.fid);

	len = sizeof(status);
	ret = getsockopt(ep->sock, SOL_SOCKET, SO_ERROR, (char *) &status, &len);
	if (ret < 0 || status) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "connection failure\n");
		ret = (ret < 0)? -ofi_sockerr() : status;
		goto err;
	}

	ret = tx_cm_data(ep->sock, ofi_ctrl_connreq, cm_ctx);
	if (ret)
		goto err;

	ret = ofi_wait_del_fd(wait, ep->sock);
	if (ret)
		goto err;

	cm_ctx->type = CLIENT_RECV_CONNRESP;
	ret = ofi_wait_add_fd(wait, ep->sock, POLLIN,
			      tcpx_eq_wait_try_func, NULL, cm_ctx);
	if (ret)
		goto err;

	return;
err:
	memset(&err_entry, 0, sizeof err_entry);
	err_entry.fid = cm_ctx->fid;
	err_entry.context = cm_ctx->fid->context;
	err_entry.err = -ret;

	free(cm_ctx);
	fi_eq_write(&ep->util_ep.eq->eq_fid, FI_SHUTDOWN,
		    &err_entry, sizeof(err_entry), UTIL_FLAG_ERROR);
}

static void server_sock_accept(struct util_wait *wait,
			       struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_conn_handle *handle;
	struct tcpx_cm_context *rx_req_cm_ctx;
	struct tcpx_pep *pep;
	SOCKET sock;
	int ret;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Received Connreq\n");
	assert(cm_ctx->fid->fclass == FI_CLASS_PEP);
	pep = container_of(cm_ctx->fid, struct tcpx_pep, util_pep.pep_fid.fid);

	sock = accept(pep->sock, NULL, 0);
	if (sock < 0) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"accept error: %d\n", ofi_sockerr());
		return;
	}

	handle = calloc(1, sizeof(*handle));
	if (!handle) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory \n");
		goto err1;
	}

	rx_req_cm_ctx = calloc(1, sizeof(*rx_req_cm_ctx));
	if (!rx_req_cm_ctx)
		goto err2;

	handle->sock = sock;
	handle->handle.fclass = FI_CLASS_CONNREQ;
	handle->pep = pep;
	rx_req_cm_ctx->fid = &handle->handle;
	rx_req_cm_ctx->type = SERVER_RECV_CONNREQ;

	ret = ofi_wait_add_fd(wait, sock, POLLIN,
			      tcpx_eq_wait_try_func,
			      NULL, (void *) rx_req_cm_ctx);
	if (ret)
		goto err3;

	return;
err3:
	free(rx_req_cm_ctx);
err2:
	free(handle);
err1:
	ofi_close_socket(sock);
}

static void process_cm_ctx(struct util_wait *wait,
			   struct tcpx_cm_context *cm_ctx)
{
	switch (cm_ctx->type) {
	case SERVER_SOCK_ACCEPT:
		server_sock_accept(wait,cm_ctx);
		break;
	case CLIENT_SEND_CONNREQ:
		client_send_connreq(wait, cm_ctx);
		break;
	case SERVER_RECV_CONNREQ:
		server_recv_connreq(wait, cm_ctx);
		break;
	case SERVER_SEND_CM_ACCEPT:
		server_send_cm_accept(wait, cm_ctx);
		break;
	case CLIENT_RECV_CONNRESP:
		client_recv_connresp(wait, cm_ctx);
		break;
	default:
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"should never end up here\n");
	}
}

/* The implementation assumes that the EQ does not share a wait set with
 * a CQ.  This is true for internally created wait sets, but not if the
 * application manages the wait set.  To fix, we need to distinguish
 * whether the wait_context references a fid or tcpx_cm_context.
 */
void tcpx_conn_mgr_run(struct util_eq *eq)
{
	struct util_wait_fd *wait_fd;
	struct tcpx_eq *tcpx_eq;
	void *wait_contexts[MAX_POLL_EVENTS];
	int num_fds = 0, i;

	assert(eq->wait != NULL);

	wait_fd = container_of(eq->wait, struct util_wait_fd,
			       util_wait);

	tcpx_eq = container_of(eq, struct tcpx_eq, util_eq);
	fastlock_acquire(&tcpx_eq->close_lock);
	num_fds = (wait_fd->util_wait.wait_obj == FI_WAIT_FD) ?
		  ofi_epoll_wait(wait_fd->epoll_fd, wait_contexts,
				 MAX_POLL_EVENTS, 0) :
		  ofi_pollfds_wait(wait_fd->pollfds, wait_contexts,
				   MAX_POLL_EVENTS, 0);
	if (num_fds < 0) {
		fastlock_release(&tcpx_eq->close_lock);
		return;
	}

	for ( i = 0; i < num_fds; i++) {
		/* skip wake up signals */
		if (&wait_fd->util_wait.wait_fid.fid == wait_contexts[i])
			continue;

		process_cm_ctx(eq->wait, (struct tcpx_cm_context *)
			       wait_contexts[i]);
	}
	fastlock_release(&tcpx_eq->close_lock);
}
