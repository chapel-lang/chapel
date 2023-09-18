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


struct tcpx_cm_context *tcpx_alloc_cm_ctx(fid_t fid, enum tcpx_cm_state state)
{
	struct tcpx_cm_context *cm_ctx;
	struct tcpx_ep *ep;

	cm_ctx = calloc(1, sizeof(*cm_ctx));
	if (!cm_ctx)
		return NULL;

	cm_ctx->fid.fclass = TCPX_CLASS_CM;
	cm_ctx->hfid = fid;
	if (fid && fid->fclass == FI_CLASS_EP) {
		ep = container_of(cm_ctx->hfid, struct tcpx_ep,
				  util_ep.ep_fid.fid);
		assert(!ep->fid);
		ep->cm_ctx = cm_ctx;
	}
	cm_ctx->state = state;
	return cm_ctx;
}

void tcpx_free_cm_ctx(struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_ep *ep;

	assert(cm_ctx->fid.fclass == TCPX_CLASS_CM);
	if (cm_ctx->hfid && cm_ctx->hfid->fclass == FI_CLASS_EP) {
		ep = container_of(cm_ctx->hfid, struct tcpx_ep,
				  util_ep.ep_fid.fid);
		ep->cm_ctx = NULL;
	}

	free(cm_ctx);
}

/* The underlying socket has the POLLIN event set.  The entire
 * CM message should be readable, as it fits within a single MTU
 * and is the first data transferred over the socket.
 */
static ssize_t rx_cm_data(SOCKET fd, int type, struct tcpx_cm_context *cm_ctx)
{
	size_t data_size = 0;
	ssize_t ret;

	ret = ofi_recv_socket(fd, &cm_ctx->msg.hdr, sizeof(cm_ctx->msg.hdr), 0);
	if (ret != sizeof(cm_ctx->msg.hdr)) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Failed to read cm header\n");
		ret = ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
		goto out;
	}

	if (cm_ctx->msg.hdr.version != TCPX_CTRL_HDR_VERSION) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cm protocol version mismatch\n");
		ret = -FI_ENOPROTOOPT;
		goto out;
	}

	if (cm_ctx->msg.hdr.type != type &&
	    cm_ctx->msg.hdr.type != ofi_ctrl_nack) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"unexpected cm message type, expected %d or %d got: %d\n",
			type, ofi_ctrl_nack, cm_ctx->msg.hdr.type);
		ret = -FI_ECONNREFUSED;
		goto out;
	}

	data_size = ntohs(cm_ctx->msg.hdr.seg_size);
	if (data_size) {
		if (data_size > TCPX_MAX_CM_DATA_SIZE)
			data_size = TCPX_MAX_CM_DATA_SIZE;

		ret = ofi_recv_socket(fd, cm_ctx->msg.data, data_size, 0);
		if ((size_t) ret != data_size) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to read cm data\n");
			ret = ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;
			data_size = 0;
			goto out;
		}

		if (data_size > TCPX_MAX_CM_DATA_SIZE) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Discarding unexpected cm data\n");
			ofi_discard_socket(fd, data_size - TCPX_MAX_CM_DATA_SIZE);
		}
	}

	if (cm_ctx->msg.hdr.type == ofi_ctrl_nack) {
		FI_INFO(&tcpx_prov, FI_LOG_EP_CTRL,
			"Connection refused from remote\n");
		ret = -FI_ECONNREFUSED;
		goto out;
	}

	ret = 0;
out:
	cm_ctx->cm_data_sz = data_size;
	return ret;
}

/* The underlying socket has the POLLOUT event set.  It is ready
 * to accept outbound data.  We expect to transfer the entire CM
 * message as it fits into a single MTU and is the first data
 * transferred over the socket.
 */
static int tx_cm_data(SOCKET fd, uint8_t type, struct tcpx_cm_context *cm_ctx)
{
	ssize_t ret;

	memset(&cm_ctx->msg.hdr, 0, sizeof(cm_ctx->msg.hdr));
	cm_ctx->msg.hdr.version = TCPX_CTRL_HDR_VERSION;
	cm_ctx->msg.hdr.type = type;
	cm_ctx->msg.hdr.seg_size = htons((uint16_t) cm_ctx->cm_data_sz);
	cm_ctx->msg.hdr.conn_data = 1; /* tests endianess mismatch at peer */

	ret = ofi_send_socket(fd, &cm_ctx->msg, sizeof(cm_ctx->msg.hdr) +
			      cm_ctx->cm_data_sz, MSG_NOSIGNAL);
	if ((size_t) ret != sizeof(cm_ctx->msg.hdr) + cm_ctx->cm_data_sz)
		return ofi_sockerr() ? -ofi_sockerr() : -FI_EIO;

	return FI_SUCCESS;
}

static int tcpx_ep_add_fd(struct tcpx_ep *ep)
{
	int ret;

	if (ep->util_ep.rx_cq) {
		ret = ofi_wait_add_fd(ep->util_ep.rx_cq->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to rx_cq\n");
			return ret;
		}
	}

	if (ep->util_ep.tx_cq) {
		ret = ofi_wait_add_fd(ep->util_ep.tx_cq->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to tx_cq\n");
			return ret;
		}
	}

	if (ep->util_ep.rx_cntr) {
		ret = ofi_wait_add_fd(ep->util_ep.rx_cntr->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to rx_cntr\n");
			return ret;
		}
	}

	if (ep->util_ep.tx_cntr) {
		ret = ofi_wait_add_fd(ep->util_ep.tx_cntr->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to tx_cntr\n");
			return ret;
		}
	}

	if (ep->util_ep.wr_cntr) {
		ret = ofi_wait_add_fd(ep->util_ep.wr_cntr->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to wr_cntr\n");
			return ret;
		}
	}

	if (ep->util_ep.rd_cntr) {
		ret = ofi_wait_add_fd(ep->util_ep.rd_cntr->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to rd_cntr\n");
			return ret;
		}
	}

	if (ep->util_ep.rem_wr_cntr) {
		ret = ofi_wait_add_fd(ep->util_ep.rem_wr_cntr->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to rem_wr_cntr\n");
			return ret;
		}
	}

	if (ep->util_ep.rem_rd_cntr) {
		ret = ofi_wait_add_fd(ep->util_ep.rem_rd_cntr->wait,
				      ep->bsock.sock, POLLIN, tcpx_try_func,
				      (void *) &ep->util_ep,
				      &ep->util_ep.ep_fid.fid);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Failed to add fd to rem_rd_cntr\n");
			return ret;
		}
	}
	return 0;
}

static int tcpx_ep_enable(struct tcpx_ep *ep,
			  struct fi_eq_cm_entry *cm_entry,
			  size_t cm_entry_sz)

{
	int ret = 0;

	if (!ep->util_ep.rx_cq && !ep->util_ep.tx_cq) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"ep must be bound to cq's\n");
		return -FI_ENOCQ;
	}

	ofi_mutex_lock(&ep->lock);
	if (ep->state != TCPX_CONNECTING && ep->state != TCPX_ACCEPTING) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"ep is in invalid state\n");
		ret = -FI_EINVAL;
		goto unlock;
	}

	ep->state = TCPX_CONNECTED;
	ofi_mutex_unlock(&ep->lock);

	ret = tcpx_ep_add_fd(ep);
	if (ret)
		return ret;

	ret = (int) fi_eq_write(&ep->util_ep.eq->eq_fid, FI_CONNECTED, cm_entry,
				cm_entry_sz, 0);
	if (ret < 0) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");
		return ret;
	}

	return 0;

unlock:
	ofi_mutex_unlock(&ep->lock);
	return ret;
}

static void tcpx_cm_recv_resp(struct util_wait *wait,
			      struct tcpx_cm_context *cm_ctx)
{
	struct fi_eq_cm_entry *cm_entry;
	struct tcpx_ep *ep;
	ssize_t ret;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Handling accept from server\n");
	assert(cm_ctx->hfid->fclass == FI_CLASS_EP);
	ep = container_of(cm_ctx->hfid, struct tcpx_ep, util_ep.ep_fid.fid);

	ret = rx_cm_data(ep->bsock.sock, ofi_ctrl_connresp, cm_ctx);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return;

		enum fi_log_level level = (ret == -FI_ECONNREFUSED) ?
				FI_LOG_INFO : FI_LOG_WARN;
		FI_LOG(&tcpx_prov, level, FI_LOG_EP_CTRL,
			"Failed to receive connect response\n");
		ofi_wait_del_fd(wait, ep->bsock.sock);
		goto err1;
	}

	ret = ofi_wait_del_fd(wait, ep->bsock.sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Could not remove fd from wait\n");
		goto err1;
	}

	cm_entry = calloc(1, sizeof(*cm_entry) + cm_ctx->cm_data_sz);
	if (!cm_entry) {
		ret = FI_ENOMEM;
		goto err1;
	}

	cm_entry->fid = cm_ctx->hfid;
	memcpy(cm_entry->data, cm_ctx->msg.data, cm_ctx->cm_data_sz);

	ep->hdr_bswap = (cm_ctx->msg.hdr.conn_data == 1) ?
			tcpx_hdr_none : tcpx_hdr_bswap;

	ret = tcpx_ep_enable(ep, cm_entry,
			     sizeof(*cm_entry) + cm_ctx->cm_data_sz);
	if (ret)
		goto err2;

	free(cm_entry);
	tcpx_free_cm_ctx(cm_ctx);
	return;

err2:
	free(cm_entry);
err1:
	ofi_mutex_lock(&ep->lock);
	tcpx_ep_disable(ep, -ret, cm_ctx->msg.data, cm_ctx->cm_data_sz);
	ofi_mutex_unlock(&ep->lock);
	tcpx_free_cm_ctx(cm_ctx);
}

int tcpx_eq_wait_try_func(void *arg)
{
	return FI_SUCCESS;
}

static void tcpx_cm_send_resp(struct util_wait *wait,
			      struct tcpx_cm_context *cm_ctx)
{
	struct fi_eq_cm_entry cm_entry = {0};
	struct tcpx_ep *ep;
	int ret;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Send connect (accept) response\n");
	assert(cm_ctx->hfid->fclass == FI_CLASS_EP);
	ep = container_of(cm_ctx->hfid, struct tcpx_ep, util_ep.ep_fid.fid);

	ret = tx_cm_data(ep->bsock.sock, ofi_ctrl_connresp, cm_ctx);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return;
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Failed to send connect (accept) response\n");
		goto delfd;
	}

	ret = ofi_wait_del_fd(wait, ep->bsock.sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Could not remove fd from wait\n");
		goto disable;
	}

	cm_entry.fid = cm_ctx->hfid;

	ret = tcpx_ep_enable(ep, &cm_entry, sizeof(cm_entry));
	if (ret)
		goto disable;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Connection Accept Successful\n");
	tcpx_free_cm_ctx(cm_ctx);
	return;

delfd:
	ofi_wait_del_fd(wait, ep->bsock.sock);
disable:
	ofi_mutex_lock(&ep->lock);
	tcpx_ep_disable(ep, -ret, NULL, 0);
	ofi_mutex_unlock(&ep->lock);
	tcpx_free_cm_ctx(cm_ctx);
}

static void tcpx_cm_recv_req(struct util_wait *wait,
			     struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_conn_handle *handle;
	struct fi_eq_cm_entry *cm_entry;
	socklen_t len;
	ssize_t ret;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "Server receive connect request\n");
	handle  = container_of(cm_ctx->hfid, struct tcpx_conn_handle, fid);

	ret = rx_cm_data(handle->sock, ofi_ctrl_connreq, cm_ctx);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return;
		ofi_wait_del_fd(wait, handle->sock);
		goto err1;
	}

	ret = ofi_wait_del_fd(wait, handle->sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"fd deletion from ofi_wait failed\n");
		goto err1;
	}

	cm_entry = calloc(1, sizeof(*cm_entry) + cm_ctx->cm_data_sz);
	if (!cm_entry)
		goto err1;

	cm_entry->fid = &handle->pep->util_pep.pep_fid.fid;
	cm_entry->info = fi_dupinfo(handle->pep->info);
	if (!cm_entry->info)
		goto err2;

	cm_entry->info->dest_addrlen = handle->pep->info->src_addrlen;
	len = (socklen_t) cm_entry->info->dest_addrlen;

	free(cm_entry->info->dest_addr);
	cm_entry->info->dest_addr = malloc(len);
	if (!cm_entry->info->dest_addr)
		goto err3;

	ret = ofi_getpeername(handle->sock, cm_entry->info->dest_addr, &len);
	if (ret)
		goto err3;

	handle->endian_match = (cm_ctx->msg.hdr.conn_data == 1);
	cm_entry->info->handle = &handle->fid;
	memcpy(cm_entry->data, cm_ctx->msg.data, cm_ctx->cm_data_sz);
	cm_ctx->state = TCPX_CM_REQ_RVCD;

	ret = (int) fi_eq_write(&handle->pep->util_pep.eq->eq_fid,
				FI_CONNREQ, cm_entry,
				sizeof(*cm_entry) + cm_ctx->cm_data_sz, 0);
	if (ret < 0) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL, "Error writing to EQ\n");
		goto err3;
	}

	free(cm_entry);
	tcpx_free_cm_ctx(cm_ctx);
	return;
err3:
	fi_freeinfo(cm_entry->info);
err2:
	free(cm_entry);
err1:
	ofi_close_socket(handle->sock);
	tcpx_free_cm_ctx(cm_ctx);
	free(handle);
}

static void tcpx_cm_send_req(struct util_wait *wait,
			     struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_ep *ep;
	socklen_t len;
	int status, ret = FI_SUCCESS;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "client send connreq\n");
	ep = container_of(cm_ctx->hfid, struct tcpx_ep, util_ep.ep_fid.fid);

	len = sizeof(status);
	ret = getsockopt(ep->bsock.sock, SOL_SOCKET, SO_ERROR,
			 (char *) &status, &len);
	if (ret < 0 || status) {
		ret = (ret < 0)? -ofi_sockerr() : -status;
		FI_WARN_SPARSE(&tcpx_prov, FI_LOG_EP_CTRL,
				"connection failure (sockerr %d)\n", ret);
		goto delfd;
	}

	ret = tx_cm_data(ep->bsock.sock, ofi_ctrl_connreq, cm_ctx);
	if (ret)
		goto delfd;

	ret = ofi_wait_del_fd(wait, ep->bsock.sock);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"Could not remove fd from wait: %s\n",
			fi_strerror(-ret));
		goto disable;
	}

	cm_ctx->state = TCPX_CM_REQ_SENT;
	ret = ofi_wait_add_fd(wait, ep->bsock.sock, POLLIN,
			      tcpx_eq_wait_try_func, NULL, cm_ctx);
	if (ret)
		goto disable;

	return;

delfd:
	ofi_wait_del_fd(wait, ep->bsock.sock);
disable:
	ofi_mutex_lock(&ep->lock);
	tcpx_ep_disable(ep, -ret, NULL, 0);
	ofi_mutex_unlock(&ep->lock);
	tcpx_free_cm_ctx(cm_ctx);
}

static void tcpx_accept(struct util_wait *wait,
			struct tcpx_cm_context *cm_ctx)
{
	struct tcpx_conn_handle *handle;
	struct tcpx_cm_context *rx_req_cm_ctx;
	struct tcpx_pep *pep;
	SOCKET sock;
	int ret;

	FI_DBG(&tcpx_prov, FI_LOG_EP_CTRL, "accepting connection\n");
	assert(cm_ctx->hfid->fclass == FI_CLASS_PEP);
	pep = container_of(cm_ctx->hfid, struct tcpx_pep, util_pep.pep_fid.fid);

	sock = accept(pep->sock, NULL, 0);
	if (sock < 0) {
		if (!OFI_SOCK_TRY_ACCEPT_AGAIN(ofi_sockerr())) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"accept error: %d\n", ofi_sockerr());
		}
		return;
	}

	handle = calloc(1, sizeof(*handle));
	if (!handle) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
			"cannot allocate memory \n");
		goto err1;
	}

	rx_req_cm_ctx = tcpx_alloc_cm_ctx(&handle->fid, TCPX_CM_WAIT_REQ);
	if (!rx_req_cm_ctx)
		goto err2;

	handle->sock = sock;
	handle->fid.fclass = FI_CLASS_CONNREQ;
	handle->pep = pep;

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
	switch (cm_ctx->state) {
	case TCPX_CM_LISTENING:
		assert(cm_ctx->hfid->fclass == FI_CLASS_PEP);
		tcpx_accept(wait, cm_ctx);
		break;
	case TCPX_CM_CONNECTING:
		assert((cm_ctx->hfid->fclass == FI_CLASS_EP) &&
		       (container_of(cm_ctx->hfid, struct tcpx_ep,
				     util_ep.ep_fid.fid)->state ==
							  TCPX_CONNECTING));
		tcpx_cm_send_req(wait, cm_ctx);
		break;
	case TCPX_CM_WAIT_REQ:
		assert(cm_ctx->hfid->fclass == FI_CLASS_CONNREQ);
		tcpx_cm_recv_req(wait, cm_ctx);
		break;
	case TCPX_CM_RESP_READY:
		assert((cm_ctx->hfid->fclass == FI_CLASS_EP) &&
		       (container_of(cm_ctx->hfid, struct tcpx_ep,
				     util_ep.ep_fid.fid)->state ==
							  TCPX_ACCEPTING));
		tcpx_cm_send_resp(wait, cm_ctx);
		break;
	case TCPX_CM_REQ_SENT:
		assert((cm_ctx->hfid->fclass == FI_CLASS_EP) &&
		       (container_of(cm_ctx->hfid, struct tcpx_ep,
				     util_ep.ep_fid.fid)->state ==
							  TCPX_CONNECTING));
		tcpx_cm_recv_resp(wait, cm_ctx);
		break;
	default:
		break;
	}
}

void tcpx_conn_mgr_run(struct util_eq *util_eq)
{
	struct util_wait_fd *wait_fd;
	struct tcpx_eq *eq;
	struct fid *fid;
	struct ofi_epollfds_event events[MAX_POLL_EVENTS];
	int count, i;

	assert(util_eq->wait != NULL);
	wait_fd = container_of(util_eq->wait, struct util_wait_fd, util_wait);

	eq = container_of(util_eq, struct tcpx_eq, util_eq);
	ofi_mutex_lock(&eq->close_lock);
	count = (wait_fd->util_wait.wait_obj == FI_WAIT_FD) ?
		ofi_epoll_wait(wait_fd->epoll_fd, events, MAX_POLL_EVENTS, 0) :
		ofi_pollfds_wait(wait_fd->pollfds, events, MAX_POLL_EVENTS, 0);
	if (count < 0)
		goto unlock;

	for (i = 0; i < count; i++) {
		/* skip wake up signals */
		if (&wait_fd->util_wait.wait_fid.fid == events[i].data.ptr)
			continue;

		fid = events[i].data.ptr;
		if (fid->fclass == TCPX_CLASS_CM)
			process_cm_ctx(util_eq->wait, events[i].data.ptr);
	}
unlock:
	ofi_mutex_unlock(&eq->close_lock);
}
