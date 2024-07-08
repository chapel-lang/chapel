/*
 * Copyright (c) 2013-2014 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#if HAVE_GETIFADDRS
#include <net/if.h>
#include <ifaddrs.h>
#endif

#include "ofi_util.h"
#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_EP_CTRL, __VA_ARGS__)

extern struct fi_ops_rma sock_ep_rma;
extern struct fi_ops_msg sock_ep_msg_ops;
extern struct fi_ops_tagged sock_ep_tagged;
extern struct fi_ops_atomic sock_ep_atomic;

extern struct fi_ops_cm sock_ep_cm_ops;
extern struct fi_ops_ep sock_ep_ops;
extern struct fi_ops sock_ep_fi_ops;
extern struct fi_ops_ep sock_ctx_ep_ops;

static void sock_tx_ctx_close(struct sock_tx_ctx *tx_ctx)
{
	if (tx_ctx->comp.send_cq)
		sock_cq_remove_tx_ctx(tx_ctx->comp.send_cq, tx_ctx);

	if (tx_ctx->comp.send_cntr)
		sock_cntr_remove_tx_ctx(tx_ctx->comp.send_cntr, tx_ctx);

	if (tx_ctx->comp.read_cntr)
		sock_cntr_remove_tx_ctx(tx_ctx->comp.read_cntr, tx_ctx);

	if (tx_ctx->comp.write_cntr)
		sock_cntr_remove_tx_ctx(tx_ctx->comp.write_cntr, tx_ctx);
}

static void sock_rx_ctx_close(struct sock_rx_ctx *rx_ctx)
{
	if (rx_ctx->comp.recv_cq)
		sock_cq_remove_rx_ctx(rx_ctx->comp.recv_cq, rx_ctx);

	if (rx_ctx->comp.recv_cntr)
		sock_cntr_remove_rx_ctx(rx_ctx->comp.recv_cntr, rx_ctx);

	if (rx_ctx->comp.rem_read_cntr)
		sock_cntr_remove_rx_ctx(rx_ctx->comp.rem_read_cntr, rx_ctx);

	if (rx_ctx->comp.rem_write_cntr)
		sock_cntr_remove_rx_ctx(rx_ctx->comp.rem_write_cntr, rx_ctx);
}

static int sock_ctx_close(struct fid *fid)
{
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	switch (fid->fclass) {
	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(fid, struct sock_tx_ctx, fid.ctx.fid);
		sock_pe_remove_tx_ctx(tx_ctx);
		ofi_atomic_dec32(&tx_ctx->ep_attr->num_tx_ctx);
		ofi_atomic_dec32(&tx_ctx->domain->ref);
		sock_tx_ctx_close(tx_ctx);
		sock_tx_ctx_free(tx_ctx);
		break;

	case FI_CLASS_RX_CTX:
		rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);
		sock_pe_remove_rx_ctx(rx_ctx);
		ofi_atomic_dec32(&rx_ctx->ep_attr->num_rx_ctx);
		ofi_atomic_dec32(&rx_ctx->domain->ref);
		sock_rx_ctx_close(rx_ctx);
		sock_rx_ctx_free(rx_ctx);
		break;

	case FI_CLASS_STX_CTX:
		tx_ctx = container_of(fid, struct sock_tx_ctx, fid.stx.fid);
		ofi_atomic_dec32(&tx_ctx->domain->ref);
		sock_pe_remove_tx_ctx(tx_ctx);
		sock_tx_ctx_free(tx_ctx);
		break;

	case FI_CLASS_SRX_CTX:
		rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);
		ofi_atomic_dec32(&rx_ctx->domain->ref);
		sock_pe_remove_rx_ctx(rx_ctx);
		sock_rx_ctx_free(rx_ctx);
		break;

	default:
		SOCK_LOG_ERROR("Invalid fid\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int sock_ctx_bind_cq(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct sock_cq *sock_cq;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	if ((flags | SOCK_EP_CQ_FLAGS) != SOCK_EP_CQ_FLAGS) {
		SOCK_LOG_ERROR("Invalid cq flag\n");
		return -FI_EINVAL;
	}
	sock_cq = container_of(bfid, struct sock_cq, cq_fid.fid);
	switch (fid->fclass) {
	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(fid, struct sock_tx_ctx, fid.ctx);
		if (flags & FI_SEND) {
			tx_ctx->comp.send_cq = sock_cq;
			if (flags & FI_SELECTIVE_COMPLETION)
				tx_ctx->comp.send_cq_event = 1;
		}

		sock_cq_add_tx_ctx(sock_cq, tx_ctx);
		break;

	case FI_CLASS_RX_CTX:
		rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);
		if (flags & FI_RECV) {
			rx_ctx->comp.recv_cq = sock_cq;
			if (flags & FI_SELECTIVE_COMPLETION)
				rx_ctx->comp.recv_cq_event = 1;
		}

		sock_cq_add_rx_ctx(sock_cq, rx_ctx);
		break;

	default:
		SOCK_LOG_ERROR("Invalid fid\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int sock_ctx_bind_cntr(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct sock_cntr *cntr;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	if ((flags | SOCK_EP_CNTR_FLAGS) != SOCK_EP_CNTR_FLAGS) {
		SOCK_LOG_ERROR("Invalid cntr flag\n");
		return -FI_EINVAL;
	}

	cntr = container_of(bfid, struct sock_cntr, cntr_fid.fid);
	switch (fid->fclass) {
	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(fid, struct sock_tx_ctx, fid.ctx.fid);
		if (flags & FI_SEND) {
			tx_ctx->comp.send_cntr = cntr;
			sock_cntr_add_tx_ctx(cntr, tx_ctx);
		}

		if (flags & FI_READ) {
			tx_ctx->comp.read_cntr = cntr;
			sock_cntr_add_tx_ctx(cntr, tx_ctx);
		}

		if (flags & FI_WRITE) {
			tx_ctx->comp.write_cntr = cntr;
			sock_cntr_add_tx_ctx(cntr, tx_ctx);
		}
		break;

	case FI_CLASS_RX_CTX:
		rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);
		if (flags & FI_RECV) {
			rx_ctx->comp.recv_cntr = cntr;
			sock_cntr_add_rx_ctx(cntr, rx_ctx);
		}

		if (flags & FI_REMOTE_READ) {
			rx_ctx->comp.rem_read_cntr = cntr;
			sock_cntr_add_rx_ctx(cntr, rx_ctx);
		}

		if (flags & FI_REMOTE_WRITE) {
			rx_ctx->comp.rem_write_cntr = cntr;
			sock_cntr_add_rx_ctx(cntr, rx_ctx);
		}
		break;

	default:
		SOCK_LOG_ERROR("Invalid fid\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int sock_ctx_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		return sock_ctx_bind_cq(fid, bfid, flags);

	case FI_CLASS_CNTR:
		return sock_ctx_bind_cntr(fid, bfid, flags);

	case FI_CLASS_MR:
		return 0;

	default:
		SOCK_LOG_ERROR("Invalid bind()\n");
		return -FI_EINVAL;
	}

}

static int sock_ctx_enable(struct fid_ep *ep)
{
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	switch (ep->fid.fclass) {
	case FI_CLASS_RX_CTX:
		rx_ctx = container_of(ep, struct sock_rx_ctx, ctx.fid);
		sock_pe_add_rx_ctx(rx_ctx->domain->pe, rx_ctx);

		if (!rx_ctx->ep_attr->conn_handle.do_listen &&
		    sock_conn_listen(rx_ctx->ep_attr)) {
			SOCK_LOG_ERROR("failed to create listener\n");
		}
		rx_ctx->enabled = 1;
		return 0;

	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(ep, struct sock_tx_ctx, fid.ctx.fid);
		sock_pe_add_tx_ctx(tx_ctx->domain->pe, tx_ctx);

		if (!tx_ctx->ep_attr->conn_handle.do_listen &&
		    sock_conn_listen(tx_ctx->ep_attr)) {
			SOCK_LOG_ERROR("failed to create listener\n");
		}
		tx_ctx->enabled = 1;
		return 0;

	default:
		SOCK_LOG_ERROR("Invalid CTX\n");
		break;
	}
	return -FI_EINVAL;
}

int sock_getopflags(struct fi_tx_attr *tx_attr, struct fi_rx_attr *rx_attr,
			uint64_t *flags)
{
	if ((*flags & FI_TRANSMIT) && (*flags & FI_RECV)) {
		SOCK_LOG_ERROR("Both Tx/Rx flags cannot be specified\n");
		return -FI_EINVAL;
	} else if (tx_attr && (*flags & FI_TRANSMIT)) {
		*flags = tx_attr->op_flags;
	} else if (rx_attr && (*flags & FI_RECV)) {
		*flags = rx_attr->op_flags;
	} else {
		SOCK_LOG_ERROR("Tx/Rx flags not specified\n");
		return -FI_EINVAL;
	}
	return 0;
}

int sock_setopflags(struct fi_tx_attr *tx_attr, struct fi_rx_attr *rx_attr,
			uint64_t flags)
{
	if ((flags & FI_TRANSMIT) && (flags & FI_RECV)) {
		SOCK_LOG_ERROR("Both Tx/Rx flags cannot be specified\n");
		return -FI_EINVAL;
	} else if (tx_attr && (flags & FI_TRANSMIT)) {
		tx_attr->op_flags = flags;
		tx_attr->op_flags &= ~FI_TRANSMIT;
		if (!(flags & (FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE |
		     FI_DELIVERY_COMPLETE)))
			tx_attr->op_flags |= FI_TRANSMIT_COMPLETE;
	} else if (rx_attr && (flags & FI_RECV)) {
		rx_attr->op_flags = flags;
		rx_attr->op_flags &= ~FI_RECV;
	} else {
		SOCK_LOG_ERROR("Tx/Rx flags not specified\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int sock_ctx_control(struct fid *fid, int command, void *arg)
{
	struct fid_ep *ep;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;
	int ret;

	switch (fid->fclass) {
	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(fid, struct sock_tx_ctx, fid.ctx.fid);
		switch (command) {
		case FI_GETOPSFLAG:
			ret = sock_getopflags(&tx_ctx->attr, NULL, (uint64_t *) arg);
			if (ret)
				return -EINVAL;
			break;
		case FI_SETOPSFLAG:
			ret = sock_setopflags(&tx_ctx->attr, NULL, *(uint64_t *) arg);
			if (ret)
				return -EINVAL;
			break;
		case FI_ENABLE:
			ep = container_of(fid, struct fid_ep, fid);
			return sock_ctx_enable(ep);
			break;
		default:
			return -FI_ENOSYS;
		}
		break;

	case FI_CLASS_RX_CTX:
	case FI_CLASS_SRX_CTX:
		rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);
		switch (command) {
		case FI_GETOPSFLAG:
			ret = sock_getopflags(NULL, &rx_ctx->attr, (uint64_t *) arg);
			if (ret)
				return -EINVAL;
			break;
		case FI_SETOPSFLAG:
			ret = sock_setopflags(NULL, &rx_ctx->attr, *(uint64_t *) arg);
			if (ret)
				return -EINVAL;
			break;
		case FI_ENABLE:
			ep = container_of(fid, struct fid_ep, fid);
			return sock_ctx_enable(ep);
			break;
		default:
			return -FI_ENOSYS;
		}
		break;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static struct fi_ops sock_ctx_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_ctx_close,
	.bind = sock_ctx_bind,
	.control = sock_ctx_control,
	.ops_open = fi_no_ops_open,
};

static int sock_ctx_getopt(fid_t fid, int level, int optname,
		       void *optval, size_t *optlen)
{
	struct sock_rx_ctx *rx_ctx;
	rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);

	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (*optlen < sizeof(size_t))
			return -FI_ETOOSMALL;
		*(size_t *)optval = rx_ctx->min_multi_recv;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_CM_DATA_SIZE:
		if (*optlen < sizeof(size_t))
			return -FI_ETOOSMALL;
		*((size_t *) optval) = SOCK_EP_MAX_CM_DATA_SZ;
		*optlen = sizeof(size_t);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static int sock_ctx_setopt(fid_t fid, int level, int optname,
		       const void *optval, size_t optlen)
{
	struct sock_rx_ctx *rx_ctx;
	rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);

	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		rx_ctx->min_multi_recv = *(size_t *)optval;
		break;

	default:
		return -ENOPROTOOPT;
	}
	return 0;
}

static ssize_t sock_rx_ctx_cancel(struct sock_rx_ctx *rx_ctx, void *context)
{
	struct dlist_entry *entry;
	ssize_t ret = -FI_ENOENT;
	struct sock_rx_entry *rx_entry;
	struct sock_pe_entry pe_entry;

	ofi_mutex_lock(&rx_ctx->lock);
	for (entry = rx_ctx->rx_entry_list.next;
	     entry != &rx_ctx->rx_entry_list; entry = entry->next) {

		rx_entry = container_of(entry, struct sock_rx_entry, entry);
		if (rx_entry->is_busy)
			continue;

		if ((uintptr_t) context == rx_entry->context) {
			if (rx_ctx->comp.recv_cq) {
				memset(&pe_entry, 0, sizeof(pe_entry));
				pe_entry.comp = &rx_ctx->comp;
				pe_entry.tag = rx_entry->tag;
				pe_entry.context = rx_entry->context;
				pe_entry.flags = (FI_MSG | FI_RECV);
				if (rx_entry->is_tagged)
					pe_entry.flags |= FI_TAGGED;

				if (sock_cq_report_error(pe_entry.comp->recv_cq,
							 &pe_entry, 0, FI_ECANCELED,
							 -FI_ECANCELED, NULL, 0)) {
					SOCK_LOG_ERROR("failed to report error\n");
				}
			}

			if (rx_ctx->comp.recv_cntr)
				fi_cntr_adderr(&rx_ctx->comp.recv_cntr->cntr_fid, 1);

			dlist_remove(&rx_entry->entry);
			sock_rx_release_entry(rx_entry);
			ret = 0;
			break;
		}
	}
	ofi_mutex_unlock(&rx_ctx->lock);
	return ret;
}

static ssize_t sock_ep_cancel(fid_t fid, void *context)
{
	struct sock_rx_ctx *rx_ctx = NULL;
	struct sock_ep *sock_ep;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		rx_ctx = sock_ep->attr->rx_ctx;
		break;

	case FI_CLASS_RX_CTX:
	case FI_CLASS_SRX_CTX:
		rx_ctx = container_of(fid, struct sock_rx_ctx, ctx.fid);
		break;

	case FI_CLASS_TX_CTX:
	case FI_CLASS_STX_CTX:
		return -FI_ENOENT;

	default:
		SOCK_LOG_ERROR("Invalid ep type\n");
		return -FI_EINVAL;
	}

	return sock_rx_ctx_cancel(rx_ctx, context);
}

size_t sock_get_tx_size(size_t size)
{
	return roundup_power_of_two(size * SOCK_EP_TX_ENTRY_SZ) /
					SOCK_EP_TX_ENTRY_SZ;
}

static ssize_t sock_rx_size_left(struct fid_ep *ep)
{
	struct sock_rx_ctx *rx_ctx;
	struct sock_ep *sock_ep;

	switch (ep->fid.fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(ep, struct sock_ep, ep);
		rx_ctx = sock_ep->attr->rx_ctx;
		break;

	case FI_CLASS_RX_CTX:
	case FI_CLASS_SRX_CTX:
		rx_ctx = container_of(ep, struct sock_rx_ctx, ctx);
		break;

	default:
		SOCK_LOG_ERROR("Invalid ep type\n");
		return -FI_EINVAL;
	}

	return rx_ctx->enabled ? rx_ctx->num_left : -FI_EOPBADSTATE;
}

static ssize_t sock_tx_size_left(struct fid_ep *ep)
{
	struct sock_ep *sock_ep;
	struct sock_tx_ctx *tx_ctx;
	ssize_t num_left = 0;

	switch (ep->fid.fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(ep, struct sock_ep, ep);
		tx_ctx = sock_ep->attr->tx_ctx;
		break;

	case FI_CLASS_TX_CTX:
		tx_ctx = container_of(ep, struct sock_tx_ctx, fid.ctx);
		break;

	default:
		SOCK_LOG_ERROR("Invalid EP type\n");
		return -FI_EINVAL;
	}

	if (!tx_ctx->enabled)
		return -FI_EOPBADSTATE;

	ofi_mutex_lock(&tx_ctx->rb_lock);
	num_left = ofi_rbavail(&tx_ctx->rb)/SOCK_EP_TX_ENTRY_SZ;
	ofi_mutex_unlock(&tx_ctx->rb_lock);
	return num_left;
}

struct fi_ops_ep sock_ctx_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = sock_ep_cancel,
	.getopt = sock_ctx_getopt,
	.setopt = sock_ctx_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = sock_rx_size_left,
	.tx_size_left = sock_tx_size_left,
};

static int sock_eq_fid_match(struct dlist_entry *entry, const void *arg)
{
	struct sock_eq_entry *sock_eq_entry;
	struct fi_eq_entry *eq_entry;
	fid_t fid = (fid_t)arg;

	sock_eq_entry = container_of(entry, struct sock_eq_entry, entry);
	/* fi_eq_entry, fi_eq_cm_entry and fi_eq_err_entry all
	 * have fid_t as first member */
	eq_entry = (struct fi_eq_entry *)sock_eq_entry->event;
	return (fid == eq_entry->fid);
}

static void sock_ep_clear_eq_list(struct dlistfd_head *list,
				  struct fid_ep *ep_fid)
{
	struct dlist_entry *entry;

	while (!dlistfd_empty(list)) {
		entry = dlist_remove_first_match(&list->list, sock_eq_fid_match,
						 ep_fid);
		if (!entry)
			break;
		dlistfd_reset(list);
		free(container_of(entry, struct sock_eq_entry, entry));
	}
}

static int sock_ep_close(struct fid *fid)
{
	struct sock_conn_req_handle *handle;
	struct sock_ep *sock_ep;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		break;

	case FI_CLASS_SEP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		break;

	default:
		return -FI_EINVAL;
	}

	if (sock_ep->is_alias) {
		ofi_atomic_dec32(&sock_ep->attr->ref);
		return 0;
	}
	if (ofi_atomic_get32(&sock_ep->attr->ref) ||
	    ofi_atomic_get32(&sock_ep->attr->num_rx_ctx) ||
	    ofi_atomic_get32(&sock_ep->attr->num_tx_ctx))
		return -FI_EBUSY;

	if (sock_ep->attr->ep_type == FI_EP_MSG) {
		if (sock_ep->attr->info.handle) {
			handle = container_of(sock_ep->attr->info.handle,
		                          struct sock_conn_req_handle, handle);
			sock_ep_cm_wait_handle_finalized(&sock_ep->attr->domain->cm_head,
			                                 handle);
			free(handle->req);
			free(handle);
		}
	} else {
		if (sock_ep->attr->av)
			ofi_atomic_dec32(&sock_ep->attr->av->ref);
	}
	if (sock_ep->attr->av) {
		ofi_mutex_lock(&sock_ep->attr->av->list_lock);
		fid_list_remove(&sock_ep->attr->av->ep_list,
				&sock_ep->attr->lock, &sock_ep->ep.fid);
		ofi_mutex_unlock(&sock_ep->attr->av->list_lock);
	}

	pthread_mutex_lock(&sock_ep->attr->domain->pe->list_lock);
	if (sock_ep->attr->tx_shared) {
		ofi_mutex_lock(&sock_ep->attr->tx_ctx->lock);
		dlist_remove(&sock_ep->attr->tx_ctx_entry);
		ofi_mutex_unlock(&sock_ep->attr->tx_ctx->lock);
	}

	if (sock_ep->attr->rx_shared) {
		ofi_mutex_lock(&sock_ep->attr->rx_ctx->lock);
		dlist_remove(&sock_ep->attr->rx_ctx_entry);
		ofi_mutex_unlock(&sock_ep->attr->rx_ctx->lock);
	}
	pthread_mutex_unlock(&sock_ep->attr->domain->pe->list_lock);

	if (sock_ep->attr->conn_handle.do_listen) {
		ofi_mutex_lock(&sock_ep->attr->domain->conn_listener.signal_lock);
		ofi_epoll_del(sock_ep->attr->domain->conn_listener.epollfd,
		             sock_ep->attr->conn_handle.sock);
		sock_ep->attr->domain->conn_listener.removed_from_epollfd = true;
		ofi_mutex_unlock(&sock_ep->attr->domain->conn_listener.signal_lock);
		ofi_close_socket(sock_ep->attr->conn_handle.sock);
		sock_ep->attr->conn_handle.do_listen = 0;
	}

	ofi_mutex_destroy(&sock_ep->attr->cm.lock);

	if (sock_ep->attr->eq) {
		ofi_mutex_lock(&sock_ep->attr->eq->lock);
		sock_ep_clear_eq_list(&sock_ep->attr->eq->list,
				      &sock_ep->ep);
		/* Any err_data if present would be freed by
		 * sock_eq_clean_err_data_list when EQ is closed */
		sock_ep_clear_eq_list(&sock_ep->attr->eq->err_list,
				      &sock_ep->ep);
		ofi_mutex_unlock(&sock_ep->attr->eq->lock);
	}

	if (sock_ep->attr->fclass != FI_CLASS_SEP) {
		if (!sock_ep->attr->tx_shared)
			sock_pe_remove_tx_ctx(sock_ep->attr->tx_array[0]);

		sock_tx_ctx_close(sock_ep->attr->tx_array[0]);
		sock_tx_ctx_free(sock_ep->attr->tx_array[0]);
	}

	if (sock_ep->attr->fclass != FI_CLASS_SEP) {
		if (!sock_ep->attr->rx_shared)
			sock_pe_remove_rx_ctx(sock_ep->attr->rx_array[0]);

		sock_rx_ctx_close(sock_ep->attr->rx_array[0]);
		sock_rx_ctx_free(sock_ep->attr->rx_array[0]);
	}

	free(sock_ep->attr->tx_array);
	free(sock_ep->attr->rx_array);

	if (sock_ep->attr->src_addr)
		free(sock_ep->attr->src_addr);
	if (sock_ep->attr->dest_addr)
		free(sock_ep->attr->dest_addr);

	ofi_mutex_lock(&sock_ep->attr->domain->pe->lock);
	ofi_idm_reset(&sock_ep->attr->av_idm, NULL);
	sock_conn_map_destroy(sock_ep->attr);
	ofi_mutex_unlock(&sock_ep->attr->domain->pe->lock);

	ofi_atomic_dec32(&sock_ep->attr->domain->ref);
	ofi_mutex_destroy(&sock_ep->attr->lock);
	free(sock_ep->attr);
	free(sock_ep);
	return 0;
}

static int sock_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	int ret;
	size_t i;
	struct sock_ep *ep;
	struct sock_eq *eq;
	struct sock_cq *cq;
	struct sock_av *av;
	struct sock_cntr *cntr;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	ret = ofi_ep_bind_valid(&sock_prov, bfid, flags);
	if (ret)
		return ret;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(fid, struct sock_ep, ep.fid);
		break;

	case FI_CLASS_SEP:
		ep = container_of(fid, struct sock_ep, ep.fid);
		break;

	default:
		return -FI_EINVAL;
	}

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct sock_eq, eq.fid);
		ep->attr->eq = eq;
		break;

	case FI_CLASS_MR:
		return 0;

	case FI_CLASS_CQ:
		cq = container_of(bfid, struct sock_cq, cq_fid.fid);
		if (ep->attr->domain != cq->domain)
			return -FI_EINVAL;

		if (flags & FI_SEND) {
			for (i = 0; i < ep->attr->ep_attr.tx_ctx_cnt; i++) {
				tx_ctx = ep->attr->tx_array[i];

				if (!tx_ctx)
					continue;

				ret = sock_ctx_bind_cq(&tx_ctx->fid.ctx.fid, bfid, flags);
				if (ret)
					return ret;
			}
		}

		if (flags & FI_RECV) {
			for (i = 0; i < ep->attr->ep_attr.rx_ctx_cnt; i++) {
				rx_ctx = ep->attr->rx_array[i];

				if (!rx_ctx)
					continue;

				ret = sock_ctx_bind_cq(&rx_ctx->ctx.fid, bfid, flags);
				if (ret)
					return ret;
			}
		}
		break;

	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct sock_cntr, cntr_fid.fid);
		if (ep->attr->domain != cntr->domain)
			return -FI_EINVAL;

		if (flags & FI_SEND || flags & FI_WRITE || flags & FI_READ) {
			for (i = 0; i < ep->attr->ep_attr.tx_ctx_cnt; i++) {
				tx_ctx = ep->attr->tx_array[i];

				if (!tx_ctx)
					continue;

				ret = sock_ctx_bind_cntr(&tx_ctx->fid.ctx.fid, bfid, flags);
				if (ret)
					return ret;
			}
		}

		if (flags & FI_RECV || flags & FI_REMOTE_READ ||
		    flags & FI_REMOTE_WRITE) {
			for (i = 0; i < ep->attr->ep_attr.rx_ctx_cnt; i++) {
				rx_ctx = ep->attr->rx_array[i];

				if (!rx_ctx)
					continue;

				ret = sock_ctx_bind_cntr(&rx_ctx->ctx.fid, bfid, flags);
				if (ret)
					return ret;
			}
		}
		break;

	case FI_CLASS_AV:
		av = container_of(bfid, struct sock_av, av_fid.fid);
		if (ep->attr->domain != av->domain)
			return -FI_EINVAL;

		ep->attr->av = av;
		ofi_atomic_inc32(&av->ref);

		if (ep->attr->tx_ctx &&
		    ep->attr->tx_ctx->fid.ctx.fid.fclass == FI_CLASS_TX_CTX) {
			ep->attr->tx_ctx->av = av;
		}

		if (ep->attr->rx_ctx &&
		    ep->attr->rx_ctx->ctx.fid.fclass == FI_CLASS_RX_CTX)
			ep->attr->rx_ctx->av = av;

		for (i = 0; i < ep->attr->ep_attr.tx_ctx_cnt; i++) {
			if (ep->attr->tx_array[i])
				ep->attr->tx_array[i]->av = av;
		}

		for (i = 0; i < ep->attr->ep_attr.rx_ctx_cnt; i++) {
			if (ep->attr->rx_array[i])
				ep->attr->rx_array[i]->av = av;
		}
		ofi_mutex_lock(&av->list_lock);
		ret = fid_list_insert(&av->ep_list, &ep->attr->lock, &ep->ep.fid);
		if (ret) {
			SOCK_LOG_ERROR("Error in adding fid in the EP list\n");
			ofi_mutex_unlock(&av->list_lock);
			return ret;
		}
		ofi_mutex_unlock(&av->list_lock);
		break;

	case FI_CLASS_STX_CTX:
		tx_ctx = container_of(bfid, struct sock_tx_ctx, fid.stx.fid);
		ofi_mutex_lock(&tx_ctx->lock);
		dlist_insert_tail(&ep->attr->tx_ctx_entry, &tx_ctx->ep_list);
		ofi_mutex_unlock(&tx_ctx->lock);

		ep->attr->tx_ctx->use_shared = 1;
		ep->attr->tx_ctx->stx_ctx = tx_ctx;
		break;

	case FI_CLASS_SRX_CTX:
		rx_ctx = container_of(bfid, struct sock_rx_ctx, ctx);
		ofi_mutex_lock(&rx_ctx->lock);
		dlist_insert_tail(&ep->attr->rx_ctx_entry, &rx_ctx->ep_list);
		ofi_mutex_unlock(&rx_ctx->lock);

		ep->attr->rx_ctx->use_shared = 1;
		ep->attr->rx_ctx->srx_ctx = rx_ctx;
		break;

	default:
		return -ENOSYS;
	}

	return 0;
}

static int sock_ep_control(struct fid *fid, int command, void *arg)
{
	int ret;
	struct fid_ep *ep_fid;
	struct fi_alias *alias;
	struct sock_ep *sock_ep, *new_ep;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		break;

	case FI_CLASS_SEP:
		sock_ep = container_of(fid, struct sock_ep, ep.fid);
		break;

	default:
		return -FI_EINVAL;
	}

	switch (command) {
	case FI_ALIAS:
		alias = (struct fi_alias *)arg;
		new_ep = calloc(1, sizeof(*new_ep));
		if (!new_ep)
			return -FI_ENOMEM;

		memcpy(&new_ep->tx_attr, &sock_ep->tx_attr, sizeof(struct fi_tx_attr));
		memcpy(&new_ep->rx_attr, &sock_ep->rx_attr, sizeof(struct fi_rx_attr));
		ret = sock_setopflags(&new_ep->tx_attr, &new_ep->rx_attr,
				       alias->flags);
		if (ret) {
			free(new_ep);
			return -FI_EINVAL;
		}
		new_ep->attr = sock_ep->attr;
		new_ep->is_alias = 1;
		memcpy(&new_ep->ep, &sock_ep->ep, sizeof(struct fid_ep));
		*alias->fid = &new_ep->ep.fid;
		ofi_atomic_inc32(&new_ep->attr->ref);
		break;
	case FI_GETOPSFLAG:
		ret = sock_getopflags(&sock_ep->tx_attr, &sock_ep->rx_attr, (uint64_t *) arg);
		if (ret)
			return -EINVAL;
		break;
	case FI_SETOPSFLAG:
		ret = sock_setopflags(&sock_ep->tx_attr, &sock_ep->rx_attr, *(uint64_t *) arg);
		if (ret)
			return -FI_EINVAL;
		break;
		break;
	case FI_ENABLE:
		ep_fid = container_of(fid, struct fid_ep, fid);
		return sock_ep_enable(ep_fid);

	default:
		return -FI_EINVAL;
	}
	return 0;
}


struct fi_ops sock_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_ep_close,
	.bind = sock_ep_bind,
	.control = sock_ep_control,
	.ops_open = fi_no_ops_open,
};

int sock_ep_enable(struct fid_ep *ep)
{
	size_t i;
	struct sock_ep *sock_ep;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	sock_ep = container_of(ep, struct sock_ep, ep);
	for (i = 0; i < sock_ep->attr->ep_attr.tx_ctx_cnt; i++) {
		tx_ctx = sock_ep->attr->tx_array[i];
		if (tx_ctx) {
			tx_ctx->enabled = 1;
			if (tx_ctx->use_shared) {
				if (tx_ctx->stx_ctx) {
					sock_pe_add_tx_ctx(tx_ctx->domain->pe, tx_ctx->stx_ctx);
					tx_ctx->stx_ctx->enabled = 1;
				}
			} else {
				sock_pe_add_tx_ctx(tx_ctx->domain->pe, tx_ctx);
			}
		}
	}

	for (i = 0; i < sock_ep->attr->ep_attr.rx_ctx_cnt; i++) {
		rx_ctx = sock_ep->attr->rx_array[i];
		if (rx_ctx) {
			rx_ctx->enabled = 1;
			if (rx_ctx->use_shared) {
				if (rx_ctx->srx_ctx) {
					sock_pe_add_rx_ctx(rx_ctx->domain->pe, rx_ctx->srx_ctx);
					rx_ctx->srx_ctx->enabled = 1;
				}
			} else {
				sock_pe_add_rx_ctx(rx_ctx->domain->pe, rx_ctx);
			}
		}
	}

	if (sock_ep->attr->ep_type != FI_EP_MSG && !sock_ep->attr->conn_handle.do_listen) {
		int ret = sock_conn_listen(sock_ep->attr);
		if (ret) {
			SOCK_LOG_ERROR("cannot start connection thread\n");
			return ret;
		}
	}
	sock_ep->attr->is_enabled = 1;
	return 0;
}

int sock_ep_disable(struct fid_ep *ep)
{
	size_t i;
	struct sock_ep *sock_ep;

	sock_ep = container_of(ep, struct sock_ep, ep);

	if (sock_ep->attr->tx_ctx &&
	    sock_ep->attr->tx_ctx->fid.ctx.fid.fclass == FI_CLASS_TX_CTX) {
		sock_ep->attr->tx_ctx->enabled = 0;
	}

	if (sock_ep->attr->rx_ctx &&
	    sock_ep->attr->rx_ctx->ctx.fid.fclass == FI_CLASS_RX_CTX) {
		sock_ep->attr->rx_ctx->enabled = 0;
	}

	for (i = 0; i < sock_ep->attr->ep_attr.tx_ctx_cnt; i++) {
		if (sock_ep->attr->tx_array[i])
			sock_ep->attr->tx_array[i]->enabled = 0;
	}

	for (i = 0; i < sock_ep->attr->ep_attr.rx_ctx_cnt; i++) {
		if (sock_ep->attr->rx_array[i])
			sock_ep->attr->rx_array[i]->enabled = 0;
	}
	sock_ep->attr->is_enabled = 0;
	return 0;
}

static int sock_ep_getopt(fid_t fid, int level, int optname,
		       void *optval, size_t *optlen)
{
	struct sock_ep *sock_ep;
	sock_ep = container_of(fid, struct sock_ep, ep.fid);

	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		*(size_t *)optval = sock_ep->attr->min_multi_recv;
		*optlen = sizeof(size_t);
		break;

	case FI_OPT_CM_DATA_SIZE:
		if (*optlen < sizeof(size_t)) {
			*optlen = sizeof(size_t);
			return -FI_ETOOSMALL;
		}
		*((size_t *) optval) = SOCK_EP_MAX_CM_DATA_SZ;
		*optlen = sizeof(size_t);
		break;

	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static int sock_ep_setopt(fid_t fid, int level, int optname,
		       const void *optval, size_t optlen)
{
	size_t i;
	struct sock_ep *sock_ep;
	sock_ep = container_of(fid, struct sock_ep, ep.fid);

	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:

		sock_ep->attr->min_multi_recv = *(size_t *)optval;
		for (i = 0; i < sock_ep->attr->ep_attr.rx_ctx_cnt; i++) {
			if (sock_ep->attr->rx_array[i] != NULL) {
				sock_ep->attr->rx_array[i]->min_multi_recv =
					sock_ep->attr->min_multi_recv;
			}
		}
		break;

	default:
		return -ENOPROTOOPT;
	}
	return 0;
}

static int sock_ep_tx_ctx(struct fid_ep *ep, int index, struct fi_tx_attr *attr,
			  struct fid_ep **tx_ep, void *context)
{
	struct sock_ep *sock_ep;
	struct sock_tx_ctx *tx_ctx;

	sock_ep = container_of(ep, struct sock_ep, ep);
	if (sock_ep->attr->fclass != FI_CLASS_SEP ||
		index >= (int)sock_ep->attr->ep_attr.tx_ctx_cnt)
		return -FI_EINVAL;

	if (attr) {
		if (ofi_check_tx_attr(&sock_prov, sock_ep->attr->info.tx_attr,
				      attr, 0) ||
		    ofi_check_attr_subset(&sock_prov,
					  sock_ep->attr->info.tx_attr->caps,
					  attr->caps & ~OFI_IGNORED_TX_CAPS)) {
			return -FI_ENODATA;
		}
		tx_ctx = sock_tx_ctx_alloc(attr, context, 0);
	} else {
		tx_ctx = sock_tx_ctx_alloc(&sock_ep->tx_attr, context, 0);
	}
	if (!tx_ctx)
		return -FI_ENOMEM;

	tx_ctx->tx_id = (uint16_t) index;
	tx_ctx->ep_attr = sock_ep->attr;
	tx_ctx->domain = sock_ep->attr->domain;
	if (tx_ctx->rx_ctrl_ctx && tx_ctx->rx_ctrl_ctx->is_ctrl_ctx)
		tx_ctx->rx_ctrl_ctx->domain = sock_ep->attr->domain;
	tx_ctx->av = sock_ep->attr->av;
	dlist_insert_tail(&sock_ep->attr->tx_ctx_entry, &tx_ctx->ep_list);

	tx_ctx->fid.ctx.fid.ops = &sock_ctx_ops;
	tx_ctx->fid.ctx.ops = &sock_ctx_ep_ops;
	tx_ctx->fid.ctx.msg = &sock_ep_msg_ops;
	tx_ctx->fid.ctx.tagged = &sock_ep_tagged;
	tx_ctx->fid.ctx.rma = &sock_ep_rma;
	tx_ctx->fid.ctx.atomic = &sock_ep_atomic;

	*tx_ep = &tx_ctx->fid.ctx;
	sock_ep->attr->tx_array[index] = tx_ctx;
	ofi_atomic_inc32(&sock_ep->attr->num_tx_ctx);
	ofi_atomic_inc32(&sock_ep->attr->domain->ref);
	return 0;
}

static int sock_ep_rx_ctx(struct fid_ep *ep, int index, struct fi_rx_attr *attr,
		    struct fid_ep **rx_ep, void *context)
{
	struct sock_ep *sock_ep;
	struct sock_rx_ctx *rx_ctx;

	sock_ep = container_of(ep, struct sock_ep, ep);
	if (sock_ep->attr->fclass != FI_CLASS_SEP ||
		index >= (int)sock_ep->attr->ep_attr.rx_ctx_cnt)
		return -FI_EINVAL;

	if (attr) {
		if (ofi_check_rx_attr(&sock_prov, &sock_ep->attr->info, attr, 0) ||
		    ofi_check_attr_subset(&sock_prov, sock_ep->attr->info.rx_attr->caps,
					  attr->caps & ~OFI_IGNORED_RX_CAPS)) {
			return -FI_ENODATA;
		}
		rx_ctx = sock_rx_ctx_alloc(attr, context, 0);
	} else {
		rx_ctx = sock_rx_ctx_alloc(&sock_ep->rx_attr, context, 0);
	}
	if (!rx_ctx)
		return -FI_ENOMEM;

	rx_ctx->rx_id = (uint16_t) index;
	rx_ctx->ep_attr = sock_ep->attr;
	rx_ctx->domain = sock_ep->attr->domain;
	rx_ctx->av = sock_ep->attr->av;
	dlist_insert_tail(&sock_ep->attr->rx_ctx_entry, &rx_ctx->ep_list);

	rx_ctx->ctx.fid.ops = &sock_ctx_ops;
	rx_ctx->ctx.ops = &sock_ctx_ep_ops;
	rx_ctx->ctx.msg = &sock_ep_msg_ops;
	rx_ctx->ctx.tagged = &sock_ep_tagged;

	rx_ctx->min_multi_recv = sock_ep->attr->min_multi_recv;
	*rx_ep = &rx_ctx->ctx;
	sock_ep->attr->rx_array[index] = rx_ctx;
	ofi_atomic_inc32(&sock_ep->attr->num_rx_ctx);
	ofi_atomic_inc32(&sock_ep->attr->domain->ref);
	return 0;
}

struct fi_ops_ep sock_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = sock_ep_cancel,
	.getopt = sock_ep_getopt,
	.setopt = sock_ep_setopt,
	.tx_ctx = sock_ep_tx_ctx,
	.rx_ctx = sock_ep_rx_ctx,
	.rx_size_left = sock_rx_size_left,
	.tx_size_left = sock_tx_size_left,
};

static int sock_verify_tx_attr(const struct fi_tx_attr *attr)
{
	if (!attr)
		return 0;

	if (attr->inject_size > SOCK_EP_MAX_INJECT_SZ)
		return -FI_ENODATA;

	if (sock_get_tx_size(attr->size) > sock_get_tx_size(SOCK_EP_TX_SZ))
		return -FI_ENODATA;

	if (attr->iov_limit > SOCK_EP_MAX_IOV_LIMIT)
		return -FI_ENODATA;

	if (attr->rma_iov_limit > SOCK_EP_MAX_IOV_LIMIT)
		return -FI_ENODATA;

	return 0;
}

int sock_stx_ctx(struct fid_domain *domain,
		 struct fi_tx_attr *attr, struct fid_stx **stx, void *context)
{
	struct sock_domain *dom;
	struct sock_tx_ctx *tx_ctx;

	if (attr && sock_verify_tx_attr(attr))
		return -FI_EINVAL;

	dom = container_of(domain, struct sock_domain, dom_fid);

	tx_ctx = sock_stx_ctx_alloc(attr ? attr : &sock_stx_attr, context);
	if (!tx_ctx)
		return -FI_ENOMEM;

	tx_ctx->domain = dom;
	if (tx_ctx->rx_ctrl_ctx && tx_ctx->rx_ctrl_ctx->is_ctrl_ctx)
		tx_ctx->rx_ctrl_ctx->domain = dom;

	tx_ctx->fid.stx.fid.ops = &sock_ctx_ops;
	tx_ctx->fid.stx.ops = &sock_ep_ops;
	ofi_atomic_inc32(&dom->ref);

	*stx = &tx_ctx->fid.stx;
	return 0;
}

static int sock_verify_rx_attr(const struct fi_rx_attr *attr)
{
	if (!attr)
		return 0;

	if ((attr->msg_order | SOCK_EP_MSG_ORDER) != SOCK_EP_MSG_ORDER)
		return -FI_ENODATA;

	if ((attr->comp_order | SOCK_EP_COMP_ORDER) != SOCK_EP_COMP_ORDER)
		return -FI_ENODATA;

	if (attr->total_buffered_recv > SOCK_EP_MAX_BUFF_RECV)
		return -FI_ENODATA;

	if (sock_get_tx_size(attr->size) > sock_get_tx_size(SOCK_EP_TX_SZ))
		return -FI_ENODATA;

	if (attr->iov_limit > SOCK_EP_MAX_IOV_LIMIT)
		return -FI_ENODATA;

	return 0;
}

int sock_srx_ctx(struct fid_domain *domain,
		 struct fi_rx_attr *attr, struct fid_ep **srx, void *context)
{
	struct sock_domain *dom;
	struct sock_rx_ctx *rx_ctx;

	if (attr && sock_verify_rx_attr(attr))
		return -FI_EINVAL;

	dom = container_of(domain, struct sock_domain, dom_fid);
	rx_ctx = sock_rx_ctx_alloc(attr ? attr : &sock_srx_attr, context, 0);
	if (!rx_ctx)
		return -FI_ENOMEM;

	rx_ctx->domain = dom;
	rx_ctx->ctx.fid.fclass = FI_CLASS_SRX_CTX;

	rx_ctx->ctx.fid.ops = &sock_ctx_ops;
	rx_ctx->ctx.ops = &sock_ctx_ep_ops;
	rx_ctx->ctx.msg = &sock_ep_msg_ops;
	rx_ctx->ctx.tagged = &sock_ep_tagged;
	rx_ctx->enabled = 1;

	/* default config */
	rx_ctx->min_multi_recv = SOCK_EP_MIN_MULTI_RECV;
	*srx = &rx_ctx->ctx;
	ofi_atomic_inc32(&dom->ref);
	return 0;
}

#if HAVE_GETIFADDRS
static char *sock_get_fabric_name(struct sockaddr *src_addr)
{
	int ret;
        struct ifaddrs *ifaddrs, *ifa;
	char *fabric_name = NULL;
	union ofi_sock_ip net_in_addr;
	char netbuf[OFI_ADDRSTRLEN];
	int prefix_len;

	ret = ofi_getifaddrs(&ifaddrs);
	if (ret)
		return NULL;

	for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL || !(ifa->ifa_flags & IFF_UP))
			continue;

		if (ofi_equals_ipaddr(ifa->ifa_addr, src_addr)) {
			prefix_len = (int) ofi_mask_addr(&net_in_addr.sa,
							 ifa->ifa_addr,
							 ifa->ifa_netmask);

			switch (net_in_addr.sa.sa_family) {
			case AF_INET:
				inet_ntop(AF_INET,
					&((struct sockaddr_in *)&net_in_addr)->sin_addr,
					netbuf, sizeof(netbuf));
				break;
			case AF_INET6:
				inet_ntop(AF_INET6,
					&((struct sockaddr_in6 *)&net_in_addr)->sin6_addr,
					netbuf, sizeof(netbuf));
				break;
			default:
				snprintf(netbuf, sizeof(netbuf), "%s", "<unknown>");
				netbuf[sizeof(netbuf)-1] = '\0';
				break;
			}
			snprintf(netbuf + strlen(netbuf), sizeof(netbuf) - strlen(netbuf),
				 "%s%d", "/", prefix_len);
			netbuf[sizeof(netbuf)-1] = '\0';
			fabric_name = strdup(netbuf);
			goto out;
		}
	}
out:
	freeifaddrs(ifaddrs);
	return fabric_name;
}

char *sock_get_domain_name(struct sockaddr *src_addr)
{
	int ret;
        struct ifaddrs *ifaddrs, *ifa;
	char *domain_name = NULL;

	ret = ofi_getifaddrs(&ifaddrs);
	if (ret)
		return NULL;

	for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL || !(ifa->ifa_flags & IFF_UP))
			continue;

		if (ofi_equals_ipaddr(ifa->ifa_addr, src_addr)) {
			domain_name = strdup(ifa->ifa_name);
			goto out;
		}
	}
out:
	freeifaddrs(ifaddrs);
	return domain_name;
}
#else
static char *sock_get_fabric_name(struct sockaddr *src_addr)
{
	return NULL;
}

char *sock_get_domain_name(struct sockaddr *src_addr)
{
	return NULL;
}
#endif

static void sock_set_fabric_attr(void *src_addr, const struct fi_fabric_attr *hint_attr,
				 struct fi_fabric_attr *attr)
{
	struct sock_fabric *fabric;

	*attr = sock_fabric_attr;
	if (hint_attr && hint_attr->fabric) {
		attr->fabric = hint_attr->fabric;
	} else {
		fabric = sock_fab_list_head();
		attr->fabric = fabric ? &fabric->fab_fid : NULL;
	}

	/* reverse lookup network address from node and assign it as fabric name */
	attr->name = sock_get_fabric_name(src_addr);
	if (!attr->name)
		attr->name = strdup(sock_fab_name);

	if (hint_attr) {
		attr->prov_name = strdup(hint_attr->prov_name);
		attr->api_version = hint_attr->api_version;
	}
}

static void sock_set_domain_attr(uint32_t api_version, void *src_addr,
				 const struct fi_domain_attr *hint_attr,
				 struct fi_domain_attr *attr)
{
	struct sock_domain *domain;

	domain = sock_dom_list_head();
	attr->domain = domain ? &domain->dom_fid : NULL;
	if (!hint_attr) {
		*attr = sock_domain_attr;

		if (FI_VERSION_LT(api_version, FI_VERSION(1, 5)))
			attr->mr_mode = FI_MR_SCALABLE;
		goto out;
	}

	if (hint_attr->domain) {
		domain = container_of(hint_attr->domain,
				      struct sock_domain, dom_fid);
		*attr = domain->attr;
		attr->domain = hint_attr->domain;
		goto out;
	}

	*attr = *hint_attr;
	if (attr->threading == FI_THREAD_UNSPEC)
		attr->threading = sock_domain_attr.threading;
	if (attr->control_progress == FI_PROGRESS_UNSPEC)
		attr->control_progress = sock_domain_attr.control_progress;
	if (attr->data_progress == FI_PROGRESS_UNSPEC)
		attr->data_progress = sock_domain_attr.data_progress;
	if (FI_VERSION_LT(api_version, FI_VERSION(1, 5))) {
		if (attr->mr_mode == FI_MR_UNSPEC)
			attr->mr_mode = FI_MR_SCALABLE;
	} else {
		if ((attr->mr_mode != FI_MR_BASIC) &&
		    (attr->mr_mode != FI_MR_SCALABLE))
			attr->mr_mode = 0;
	}

	if (attr->cq_cnt == 0)
		attr->cq_cnt = sock_domain_attr.cq_cnt;
	if (attr->ep_cnt == 0)
		attr->ep_cnt = sock_domain_attr.ep_cnt;
	if (attr->tx_ctx_cnt == 0)
		attr->tx_ctx_cnt = sock_domain_attr.tx_ctx_cnt;
	if (attr->rx_ctx_cnt == 0)
		attr->rx_ctx_cnt = sock_domain_attr.rx_ctx_cnt;
	if (attr->max_ep_tx_ctx == 0)
		attr->max_ep_tx_ctx = sock_domain_attr.max_ep_tx_ctx;
	if (attr->max_ep_rx_ctx == 0)
		attr->max_ep_rx_ctx = sock_domain_attr.max_ep_rx_ctx;
	if (attr->max_ep_stx_ctx == 0)
		attr->max_ep_stx_ctx = sock_domain_attr.max_ep_stx_ctx;
	if (attr->max_ep_srx_ctx == 0)
		attr->max_ep_srx_ctx = sock_domain_attr.max_ep_srx_ctx;
	if (attr->cntr_cnt == 0)
		attr->cntr_cnt = sock_domain_attr.cntr_cnt;
	if (attr->mr_iov_limit == 0)
		attr->mr_iov_limit = sock_domain_attr.mr_iov_limit;

	attr->mr_key_size = sock_domain_attr.mr_key_size;
	attr->cq_data_size = sock_domain_attr.cq_data_size;
	attr->resource_mgmt = sock_domain_attr.resource_mgmt;
out:
	/* reverse lookup interface from node and assign it as domain name */
	attr->name = sock_get_domain_name(src_addr);
	if (!attr->name)
		attr->name = strdup(sock_fab_name);
}


struct fi_info *sock_fi_info(uint32_t version, enum fi_ep_type ep_type,
			     const struct fi_info *hints, void *src_addr,
			     void *dest_addr)
{
	struct fi_info *info;

	info = fi_dupinfo(hints);
	if (!info)
		return NULL;
	free(info->src_addr);
	free(info->dest_addr);
	info->src_addr = NULL;
	info->dest_addr = NULL;
	info->src_addrlen = 0;
	info->dest_addrlen = 0;

	assert(src_addr);
	info->src_addr = calloc(1, ofi_sizeofaddr(src_addr));
	if (!info->src_addr)
		goto err;

	info->mode = SOCK_MODE;
	memcpy(info->src_addr, src_addr, ofi_sizeofaddr(src_addr));
	info->src_addrlen = ofi_sizeofaddr(info->src_addr);
	if (info->src_addrlen == sizeof(struct sockaddr_in6))
		info->addr_format = FI_SOCKADDR_IN6;
	else
		info->addr_format = FI_SOCKADDR_IN;

	if (dest_addr) {
		info->dest_addr = calloc(1, ofi_sizeofaddr(dest_addr));
		if (!info->dest_addr)
			goto err;
		info->dest_addrlen = ofi_sizeofaddr(dest_addr);
		memcpy(info->dest_addr, dest_addr, info->dest_addrlen);
	}

	if (hints) {
		sock_set_domain_attr(version, info->src_addr, hints->domain_attr,
				     info->domain_attr);
		sock_set_fabric_attr(info->src_addr, hints->fabric_attr, info->fabric_attr);
	} else {
		sock_set_domain_attr(version, info->src_addr, NULL,
				     info->domain_attr);
		sock_set_fabric_attr(info->src_addr, NULL, info->fabric_attr);
	}

	info->ep_attr->type = ep_type;
	return info;
err:
	fi_freeinfo(info);
	return NULL;
}

int sock_get_src_addr_from_hostname(union ofi_sock_ip *src_addr,
				    const char *service, uint16_t sa_family)
{
	int ret;
	struct addrinfo ai, *rai = NULL;
	char hostname[HOST_NAME_MAX];

	memset(&ai, 0, sizeof(ai));
	ai.ai_family = sa_family;
	ai.ai_socktype = SOCK_STREAM;

	ofi_getnodename(sa_family, hostname, sizeof(hostname));
	ret = getaddrinfo(hostname, service, &ai, &rai);
	if (ret) {
		SOCK_LOG_DBG("getaddrinfo failed!\n");
		return -FI_EINVAL;
	}
	memcpy(src_addr, rai->ai_addr, rai->ai_addrlen);
	freeaddrinfo(rai);
	return 0;
}

static int sock_ep_assign_src_addr(struct sock_ep *sock_ep, struct fi_info *info)
{
	sock_ep->attr->src_addr = calloc(1, sizeof(union ofi_sock_ip));
	if (!sock_ep->attr->src_addr)
		return -FI_ENOMEM;

	if (info && info->dest_addr)
		return sock_get_src_addr(info->dest_addr, sock_ep->attr->src_addr);
	else
		return sock_get_src_addr_from_hostname(sock_ep->attr->src_addr,
						       NULL, 0);
}

int sock_alloc_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct sock_ep **ep, void *context, size_t fclass)
{
	int ret;
	struct sock_ep *sock_ep;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;
	struct sock_domain *sock_dom;

	assert(info);
	sock_dom = container_of(domain, struct sock_domain, dom_fid);

	sock_ep = calloc(1, sizeof(*sock_ep));
	if (!sock_ep)
		return -FI_ENOMEM;

	switch (fclass) {
	case FI_CLASS_EP:
		sock_ep->ep.fid.fclass = FI_CLASS_EP;
		sock_ep->ep.fid.context = context;
		sock_ep->ep.fid.ops = &sock_ep_fi_ops;

		sock_ep->ep.ops = &sock_ep_ops;
		sock_ep->ep.cm = &sock_ep_cm_ops;
		sock_ep->ep.msg = &sock_ep_msg_ops;
		sock_ep->ep.rma = &sock_ep_rma;
		sock_ep->ep.tagged = &sock_ep_tagged;
		sock_ep->ep.atomic = &sock_ep_atomic;
		break;

	case FI_CLASS_SEP:
		sock_ep->ep.fid.fclass = FI_CLASS_SEP;
		sock_ep->ep.fid.context = context;
		sock_ep->ep.fid.ops = &sock_ep_fi_ops;

		sock_ep->ep.ops = &sock_ep_ops;
		sock_ep->ep.cm = &sock_ep_cm_ops;
		break;

	default:
		ret = -FI_EINVAL;
		goto err1;
	}

	sock_ep->attr = (struct sock_ep_attr *) calloc(1, sizeof(struct sock_ep_attr));
	if (!sock_ep->attr) {
		ret = -FI_ENOMEM;
		goto err1;
	}
	sock_ep->attr->fclass = fclass;
	*ep = sock_ep;

	sock_ep->attr->info.caps = info->caps;
	sock_ep->attr->info.addr_format = info->addr_format;

	if (info->ep_attr) {
		sock_ep->attr->ep_type = info->ep_attr->type;
		sock_ep->attr->ep_attr.tx_ctx_cnt = info->ep_attr->tx_ctx_cnt;
		sock_ep->attr->ep_attr.rx_ctx_cnt = info->ep_attr->rx_ctx_cnt;
	}

	if (info->src_addr) {
		sock_ep->attr->src_addr = calloc(1, sizeof(*sock_ep->
							   attr->src_addr));
		if (!sock_ep->attr->src_addr) {
			ret = -FI_ENOMEM;
			goto err2;
		}
		memcpy(sock_ep->attr->src_addr, info->src_addr,
			info->src_addrlen);
	}

	if (info->dest_addr) {
		sock_ep->attr->dest_addr = calloc(1, sizeof(*sock_ep->
							    attr->dest_addr));
		if (!sock_ep->attr->dest_addr) {
			ret = -FI_ENOMEM;
			goto err2;
		}
		memcpy(sock_ep->attr->dest_addr, info->dest_addr,
			info->dest_addrlen);
	}

	if (info->tx_attr) {
		sock_ep->tx_attr = *info->tx_attr;
		if (!(sock_ep->tx_attr.op_flags & (FI_INJECT_COMPLETE |
			FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)))
			sock_ep->tx_attr.op_flags |= FI_TRANSMIT_COMPLETE;
		sock_ep->tx_attr.size = sock_ep->tx_attr.size ?
			sock_ep->tx_attr.size : SOCK_EP_TX_SZ;
	}

	if (info->rx_attr)
		sock_ep->rx_attr = *info->rx_attr;
	sock_ep->attr->info.handle = info->handle;

	if (!sock_ep->attr->src_addr && sock_ep_assign_src_addr(sock_ep, info)) {
		SOCK_LOG_ERROR("failed to get src_address\n");
		ret = -FI_EINVAL;
		goto err2;
	}

	ofi_atomic_initialize32(&sock_ep->attr->ref, 0);
	ofi_atomic_initialize32(&sock_ep->attr->num_tx_ctx, 0);
	ofi_atomic_initialize32(&sock_ep->attr->num_rx_ctx, 0);
	ofi_mutex_init(&sock_ep->attr->lock);

	if (sock_ep->attr->ep_attr.tx_ctx_cnt == FI_SHARED_CONTEXT)
		sock_ep->attr->tx_shared = 1;
	if (sock_ep->attr->ep_attr.rx_ctx_cnt == FI_SHARED_CONTEXT)
		sock_ep->attr->rx_shared = 1;

	if (sock_ep->attr->fclass != FI_CLASS_SEP) {
		sock_ep->attr->ep_attr.tx_ctx_cnt = 1;
		sock_ep->attr->ep_attr.rx_ctx_cnt = 1;
	}

	sock_ep->attr->tx_array = calloc(sock_ep->attr->ep_attr.tx_ctx_cnt,
				   sizeof(struct sock_tx_ctx *));
	if (!sock_ep->attr->tx_array) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	sock_ep->attr->rx_array = calloc(sock_ep->attr->ep_attr.rx_ctx_cnt,
				   sizeof(struct sock_rx_ctx *));
	if (!sock_ep->attr->rx_array) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	if (sock_ep->attr->fclass != FI_CLASS_SEP) {
		/* default tx ctx */
		tx_ctx = sock_tx_ctx_alloc(&sock_ep->tx_attr, context,
					   sock_ep->attr->tx_shared);
		if (!tx_ctx) {
			ret = -FI_ENOMEM;
			goto err2;
		}
		tx_ctx->ep_attr = sock_ep->attr;
		tx_ctx->domain = sock_dom;
		if (tx_ctx->rx_ctrl_ctx && tx_ctx->rx_ctrl_ctx->is_ctrl_ctx)
			tx_ctx->rx_ctrl_ctx->domain = sock_dom;
		tx_ctx->tx_id = 0;
		dlist_insert_tail(&sock_ep->attr->tx_ctx_entry, &tx_ctx->ep_list);
		sock_ep->attr->tx_array[0] = tx_ctx;
		sock_ep->attr->tx_ctx = tx_ctx;

		/* default rx_ctx */
		rx_ctx = sock_rx_ctx_alloc(&sock_ep->rx_attr, context,
					   sock_ep->attr->rx_shared);
		if (!rx_ctx) {
			ret = -FI_ENOMEM;
			goto err2;
		}
		rx_ctx->ep_attr = sock_ep->attr;
		rx_ctx->domain = sock_dom;
		rx_ctx->rx_id = 0;
		dlist_insert_tail(&sock_ep->attr->rx_ctx_entry, &rx_ctx->ep_list);
		sock_ep->attr->rx_array[0] = rx_ctx;
		sock_ep->attr->rx_ctx = rx_ctx;
	}

	/* default config */
	sock_ep->attr->min_multi_recv = SOCK_EP_MIN_MULTI_RECV;

	memcpy(&sock_ep->attr->info, info, sizeof(struct fi_info));

	sock_ep->attr->domain = sock_dom;
	ofi_mutex_init(&sock_ep->attr->cm.lock);

	if (sock_conn_map_init(sock_ep, sock_cm_def_map_sz)) {
		SOCK_LOG_ERROR("failed to init connection map\n");
		ret = -FI_EINVAL;
		goto err2;
	}

	ofi_atomic_inc32(&sock_dom->ref);
	return 0;

err2:
	if (sock_ep->attr) {
		free(sock_ep->attr->src_addr);
		free(sock_ep->attr->dest_addr);
		free(sock_ep->attr);
	}
err1:
	free(sock_ep);
	return ret;
}

void sock_ep_remove_conn(struct sock_ep_attr *attr, struct sock_conn *conn)
{
	if (attr->cmap.used <= 0 || conn->sock_fd == -1)
		return;
	sock_pe_poll_del(attr->domain->pe, conn->sock_fd);
	sock_conn_release_entry(&attr->cmap, conn);
}

struct sock_conn *sock_ep_lookup_conn(struct sock_ep_attr *attr, fi_addr_t index,
				      union ofi_sock_ip *addr)
{
	int i;
	uint64_t idx;
	char buf[8];
	struct sock_conn *conn;

	idx = (attr->ep_type == FI_EP_MSG) ? index : index & attr->av->mask;

	conn = ofi_idm_lookup(&attr->av_idm, (int) idx);
	if (conn && conn != SOCK_CM_CONN_IN_PROGRESS) {
		/* Verify that the existing connection is still usable, and
		 * that the peer didn't restart.
		 */
		if (conn->connected == 0 ||
		    (sock_comm_peek(conn, buf, 8) == 0 && conn->connected == 0)) {
			sock_ep_remove_conn(attr, conn);
			ofi_straddr_log(&sock_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
					"Peer disconnected", &addr->sa);
			return NULL;
		}
		if (conn->av_index != FI_ADDR_NOTAVAIL)
			assert(conn->av_index == idx);
		else
			conn->av_index = idx;
		return conn;
	}

	for (i = 0; i < attr->cmap.used; i++) {
		if (!attr->cmap.table[i].connected)
			continue;

		if (ofi_equals_sockaddr(&attr->cmap.table[i].addr.sa, &addr->sa) &&
			attr->cmap.table[i].av_index == idx) {
			conn = &attr->cmap.table[i];
			break;
		}
	}
	if (conn && conn != SOCK_CM_CONN_IN_PROGRESS) {
		if (conn->connected == 0 ||
		    (sock_comm_peek(conn, buf, 8) == 0 && conn->connected == 0)) {
			sock_ep_remove_conn(attr, conn);
			ofi_straddr_log(&sock_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
					"Peer disconnected", &addr->sa);
			return NULL;
		}
		if (conn->av_index != FI_ADDR_NOTAVAIL)
			assert(conn->av_index == idx);
		else
			conn->av_index = idx;
	}
	return conn;
}

int sock_ep_get_conn(struct sock_ep_attr *attr, struct sock_tx_ctx *tx_ctx,
		     fi_addr_t index, struct sock_conn **pconn)
{
	struct sock_conn *conn;
	uint64_t av_index = (attr->ep_type == FI_EP_MSG) ?
			    0 : (index & attr->av->mask);
	union ofi_sock_ip *addr;
	int ret = FI_SUCCESS;

	if (attr->ep_type == FI_EP_MSG)
		addr = attr->dest_addr;
	else {
		ofi_mutex_lock(&attr->av->table_lock);
		addr = &attr->av->table[av_index].addr;
		ofi_mutex_unlock(&attr->av->table_lock);
	}

	ofi_mutex_lock(&attr->cmap.lock);
	conn = sock_ep_lookup_conn(attr, av_index, addr);
	if (!conn) {
		conn = SOCK_CM_CONN_IN_PROGRESS;
		if (ofi_idm_set(&attr->av_idm, (int) av_index, conn) < 0)
			SOCK_LOG_ERROR("ofi_idm_set failed\n");
	}
	ofi_mutex_unlock(&attr->cmap.lock);

	if (conn == SOCK_CM_CONN_IN_PROGRESS)
		ret = sock_ep_connect(attr, av_index, &conn);

	if (!conn) {
		SOCK_LOG_ERROR("Unable to find connection entry. "
			       "Error in connecting: %s\n",
			       fi_strerror(-ret));
		ofi_straddr_log(&sock_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
				"Unable to connect to", &addr->sa);
		return -FI_ENOENT;
	}

	*pconn = conn;
	return conn->address_published ?
	       0 : (int) sock_conn_send_src_addr(attr, tx_ctx, conn);
}
