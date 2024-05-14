/*
 * Copyright (c) 2013-2020 Intel Corporation. All rights reserved.
 * Copyright (c) 2020 Cisco Systems, Inc.  All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <inttypes.h>
#include <math.h>

#include <rdma/fabric.h>
#include <rdma/fi_collective.h>
#include <ofi.h>
#include <ofi_util.h>

#include "rxm.h"

static int rxm_match_noop(struct dlist_entry *item, const void *arg)
{
	OFI_UNUSED(item);
	OFI_UNUSED(arg);
	return 1;
}

static int rxm_match_recv_entry(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *) arg;
	struct rxm_recv_entry *recv_entry =
		container_of(item, struct rxm_recv_entry, entry);
	return ofi_match_addr(recv_entry->addr, attr->addr);
}

static int rxm_match_recv_entry_tag(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *) arg;
	struct rxm_recv_entry *recv_entry =
		container_of(item, struct rxm_recv_entry, entry);
	return ofi_match_tag(recv_entry->tag, recv_entry->ignore, attr->tag);
}

static int rxm_match_recv_entry_tag_addr(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *) arg;
	struct rxm_recv_entry *recv_entry =
		container_of(item, struct rxm_recv_entry, entry);
	return ofi_match_addr(recv_entry->addr, attr->addr) &&
		ofi_match_tag(recv_entry->tag, recv_entry->ignore, attr->tag);
}

static int rxm_match_recv_entry_context(struct dlist_entry *item, const void *context)
{
	struct rxm_recv_entry *recv_entry =
		container_of(item, struct rxm_recv_entry, entry);
	return recv_entry->context == context;
}

static fi_addr_t rxm_get_unexp_addr(struct rxm_unexp_msg *unexp_msg)
{
	struct rxm_rx_buf *rx_buf;

	rx_buf = container_of(unexp_msg, struct rxm_rx_buf, unexp_msg);
	return (unexp_msg->addr != FI_ADDR_UNSPEC) ?
		unexp_msg->addr : rx_buf->conn->peer->fi_addr;
}

static int rxm_match_unexp_msg(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *)arg;
	struct rxm_unexp_msg *unexp_msg =
		container_of(item, struct rxm_unexp_msg, entry);
	return ofi_match_addr(attr->addr, rxm_get_unexp_addr(unexp_msg));
}

static int rxm_match_unexp_msg_tag(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *) arg;
	struct rxm_unexp_msg *unexp_msg =
		container_of(item, struct rxm_unexp_msg, entry);
	return ofi_match_tag(attr->tag, attr->ignore, unexp_msg->tag);
}

static int rxm_match_unexp_msg_tag_addr(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *) arg;
	struct rxm_unexp_msg *unexp_msg =
		container_of(item, struct rxm_unexp_msg, entry);
	return ofi_match_addr(attr->addr, rxm_get_unexp_addr(unexp_msg)) &&
		ofi_match_tag(attr->tag, attr->ignore, unexp_msg->tag);
}

static int rxm_buf_reg(struct ofi_bufpool_region *region)
{
	struct rxm_ep *rxm_ep = region->pool->attr.context;
	struct rxm_domain *rxm_domain;
	int ret;
	bool hmem_enabled = !!(rxm_ep->util_ep.caps & FI_HMEM);

	if (hmem_enabled) {
		ret = ofi_hmem_host_register(region->mem_region,
					     region->pool->region_size);
		if (ret != FI_SUCCESS)
			return ret;
	}

	if (!rxm_ep->msg_mr_local)
		return 0;

	rxm_domain = container_of(rxm_ep->util_ep.domain,
				  struct rxm_domain, util_domain);

	ret = rxm_msg_mr_reg_internal(rxm_domain, region->mem_region,
				      region->pool->region_size,
				      FI_SEND | FI_RECV | FI_READ | FI_WRITE,
				      OFI_MR_NOCACHE,
				      (struct fid_mr **) &region->context);

	if (ret != FI_SUCCESS) {
		if (hmem_enabled)
			ofi_hmem_host_unregister(region->mem_region);
	}

	return ret;
}

static void rxm_init_rx_buf(struct ofi_bufpool_region *region, void *buf)
{
	struct rxm_ep *ep = region->pool->attr.context;
	struct rxm_rx_buf *rx_buf = buf;

	rx_buf->hdr.desc = ep->msg_mr_local ?
			   fi_mr_desc((struct fid_mr *) region->context) : NULL;
	rx_buf->ep = ep;
	rx_buf->data = &rx_buf->pkt.data;
}

static void rxm_init_tx_buf(struct ofi_bufpool_region *region, void *buf)
{
	struct rxm_ep *ep = region->pool->attr.context;
	struct rxm_tx_buf *tx_buf = buf;

	tx_buf->hdr.desc = ep->msg_mr_local ?
			   fi_mr_desc((struct fid_mr *) region->context) : NULL;

	tx_buf->pkt.ctrl_hdr.version = RXM_CTRL_VERSION;
	tx_buf->pkt.hdr.version = OFI_OP_VERSION;
}

static void rxm_buf_close(struct ofi_bufpool_region *region)
{
	struct rxm_ep *ep = region->pool->attr.context;

	if (ep->util_ep.caps & FI_HMEM)
		ofi_hmem_host_unregister(region->mem_region);

	if (ep->msg_mr_local) {
		/* We would get a (fid_mr *) in context but
		 * it is safe to cast it into (fid *) */
		fi_close(region->context);
	}
}

static void rxm_recv_entry_init(struct rxm_recv_entry *entry, void *arg)
{
	struct rxm_recv_queue *recv_queue = arg;

	assert(recv_queue->type != RXM_RECV_QUEUE_UNSPEC);

	entry->recv_queue = recv_queue;
	entry->sar.msg_id = RXM_SAR_RX_INIT;
	entry->sar.total_recv_len = 0;
	/* set it to NULL to differentiate between regular ACKs and those
	 * sent with FI_INJECT */
	entry->rndv.tx_buf = NULL;
	entry->comp_flags = FI_RECV;

	if (recv_queue->type == RXM_RECV_QUEUE_MSG)
		entry->comp_flags |= FI_MSG;
	else
		entry->comp_flags |= FI_TAGGED;
}

static int rxm_recv_queue_init(struct rxm_ep *rxm_ep,  struct rxm_recv_queue *recv_queue,
			       size_t size, enum rxm_recv_queue_type type)
{
	recv_queue->rxm_ep = rxm_ep;
	recv_queue->type = type;
	recv_queue->fs = rxm_recv_fs_create(size, rxm_recv_entry_init,
					    recv_queue);
	if (!recv_queue->fs)
		return -FI_ENOMEM;

	dlist_init(&recv_queue->recv_list);
	dlist_init(&recv_queue->unexp_msg_list);
	if (type == RXM_RECV_QUEUE_MSG) {
		if (rxm_ep->rxm_info->caps & FI_DIRECTED_RECV) {
			recv_queue->match_recv = rxm_match_recv_entry;
			recv_queue->match_unexp = rxm_match_unexp_msg;
		} else {
			recv_queue->match_recv = rxm_match_noop;
			recv_queue->match_unexp = rxm_match_noop;
		}
	} else {
		if (rxm_ep->rxm_info->caps & FI_DIRECTED_RECV) {
			recv_queue->match_recv = rxm_match_recv_entry_tag_addr;
			recv_queue->match_unexp = rxm_match_unexp_msg_tag_addr;
		} else {
			recv_queue->match_recv = rxm_match_recv_entry_tag;
			recv_queue->match_unexp = rxm_match_unexp_msg_tag;
		}
	}

	return 0;
}

static void rxm_recv_queue_close(struct rxm_recv_queue *recv_queue)
{
	/* It indicates that the recv_queue were allocated */
	if (recv_queue->fs) {
		rxm_recv_fs_free(recv_queue->fs);
		recv_queue->fs = NULL;
	}
	// TODO cleanup recv_list and unexp msg list
}

static int rxm_ep_create_pools(struct rxm_ep *rxm_ep)
{
	struct ofi_bufpool_attr attr = {0};
	int ret;

	attr.size = rxm_buffer_size + sizeof(struct rxm_rx_buf);
	attr.alignment = 16;
	attr.chunk_cnt = 1024;
	attr.alloc_fn = rxm_buf_reg;
	attr.free_fn = rxm_buf_close;
	attr.init_fn = rxm_init_rx_buf;
	attr.context = rxm_ep;
	attr.flags = OFI_BUFPOOL_NO_TRACK;

	ret = ofi_bufpool_create_attr(&attr, &rxm_ep->rx_pool);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to create rx buf pool\n");
		return ret;
	}

	attr.size = rxm_buffer_size + sizeof(struct rxm_tx_buf);
	attr.init_fn = rxm_init_tx_buf;
	ret = ofi_bufpool_create_attr(&attr, &rxm_ep->tx_pool);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to create tx buf pool\n");
		goto free_rx_pool;
	}

	ret = ofi_bufpool_create(&rxm_ep->coll_pool,
				 sizeof(struct rxm_coll_buf), 16, 0, 1024,
				 OFI_BUFPOOL_NO_TRACK);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to create peer xfer context pool\n");
		goto free_tx_pool;
	}
	return 0;

free_tx_pool:
	ofi_bufpool_destroy(rxm_ep->tx_pool);

free_rx_pool:
	ofi_bufpool_destroy(rxm_ep->rx_pool);
	rxm_ep->rx_pool = NULL;
	return ret;
}

static int rxm_multi_recv_pool_init(struct rxm_ep *rxm_ep)
{
	struct ofi_bufpool_attr attr = {
		.size		= sizeof(struct rxm_recv_entry),
		.alignment	= 16,
		.max_cnt	= 0,
		.chunk_cnt	= 16,
		.alloc_fn	= NULL,
		.init_fn	= NULL,
		.context	= rxm_ep,
		.flags		= OFI_BUFPOOL_NO_TRACK,
	};

	return ofi_bufpool_create_attr(&attr, &rxm_ep->multi_recv_pool);
}

static int rxm_ep_rx_queue_init(struct rxm_ep *rxm_ep)
{
	int ret;

	ret = rxm_recv_queue_init(rxm_ep, &rxm_ep->recv_queue,
				  rxm_ep->rxm_info->rx_attr->size,
				  RXM_RECV_QUEUE_MSG);
	if (ret)
		return ret;

	ret = rxm_recv_queue_init(rxm_ep, &rxm_ep->trecv_queue,
				  rxm_ep->rxm_info->rx_attr->size,
				  RXM_RECV_QUEUE_TAGGED);
	if (ret)
		goto err_recv_tag;

	ret = rxm_multi_recv_pool_init(rxm_ep);
	if (ret)
		goto err_multi;

	return FI_SUCCESS;

err_multi:
	rxm_recv_queue_close(&rxm_ep->trecv_queue);
err_recv_tag:
	rxm_recv_queue_close(&rxm_ep->recv_queue);
	return ret;
}

/* It is safe to call this function, even if `rxm_ep_txrx_res_open`
 * has not yet been called */
static void rxm_ep_txrx_res_close(struct rxm_ep *ep)
{
	rxm_recv_queue_close(&ep->trecv_queue);
	rxm_recv_queue_close(&ep->recv_queue);

	if (ep->multi_recv_pool) {
		ofi_bufpool_destroy(ep->multi_recv_pool);
		ep->multi_recv_pool = NULL;
	}
	if (ep->rx_pool) {
		ofi_bufpool_destroy(ep->rx_pool);
		ep->rx_pool = NULL;
	}
	if (ep->tx_pool) {
		ofi_bufpool_destroy(ep->tx_pool);
		ep->tx_pool = NULL;
	}
	if (ep->coll_pool) {
		ofi_bufpool_destroy(ep->coll_pool);
		ep->coll_pool = NULL;
	}
}

static int rxm_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(fid, struct rxm_ep, util_ep.ep_fid.fid);
	return fi_setname(&rxm_ep->msg_pep->fid, addr, addrlen);
}

static int rxm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(fid, struct rxm_ep, util_ep.ep_fid.fid);
	return fi_getname(&rxm_ep->msg_pep->fid, addr, addrlen);
}

static int rxm_join_coll(struct fid_ep *ep, const void *addr, uint64_t flags,
		    struct fid_mc **mc, void *context)
{
	struct rxm_ep *rxm_ep;

	if (!(flags & FI_COLLECTIVE))
		return -FI_ENOSYS;

	rxm_ep = container_of(ep, struct rxm_ep, util_ep.ep_fid);

	return fi_join(rxm_ep->util_coll_ep, addr, flags, mc, context);
}

static struct fi_ops_cm rxm_ops_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = rxm_setname,
	.getname = rxm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = rxm_join_coll,
};

static struct rxm_eager_ops def_eager_ops = {
	.comp_tx = rxm_finish_eager_send,
	.handle_rx = rxm_handle_eager,
};

static struct rxm_eager_ops coll_eager_ops = {
	.comp_tx = rxm_finish_coll_eager_send,
	.handle_rx = rxm_handle_coll_eager,
};

static bool rxm_ep_cancel_recv(struct rxm_ep *rxm_ep,
			       struct rxm_recv_queue *recv_queue, void *context)
{
	struct fi_cq_err_entry err_entry;
	struct rxm_recv_entry *recv_entry;
	struct dlist_entry *entry;
	int ret;

	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	entry = dlist_remove_first_match(&recv_queue->recv_list,
					 rxm_match_recv_entry_context,
					 context);
	if (!entry)
		goto unlock;

	recv_entry = container_of(entry, struct rxm_recv_entry, entry);
	memset(&err_entry, 0, sizeof(err_entry));
	err_entry.op_context = recv_entry->context;
	err_entry.flags |= recv_entry->comp_flags;
	err_entry.tag = recv_entry->tag;
	err_entry.err = FI_ECANCELED;
	err_entry.prov_errno = -FI_ECANCELED;
	rxm_recv_entry_release(recv_entry);
	ret = ofi_cq_write_error(rxm_ep->util_ep.rx_cq, &err_entry);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ, "Error writing to CQ\n");
		assert(0);
	}

unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return entry != NULL;
}

static ssize_t rxm_ep_cancel(fid_t fid_ep, void *context)
{
	struct rxm_ep *ep;

	ep = container_of(fid_ep, struct rxm_ep, util_ep.ep_fid);

	if (rxm_passthru_info(ep->rxm_info))
		return fi_cancel(&ep->msg_srx->fid, context);

	if (!rxm_ep_cancel_recv(ep, &ep->trecv_queue, context))
		rxm_ep_cancel_recv(ep, &ep->recv_queue, context);

	return 0;
}

static int rxm_ep_getopt(fid_t fid, int level, int optname, void *optval,
			 size_t *optlen)
{
	struct rxm_ep *rxm_ep =
		container_of(fid, struct rxm_ep, util_ep.ep_fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		assert(sizeof(rxm_ep->min_multi_recv_size) == sizeof(size_t));
		*(size_t *)optval = rxm_ep->min_multi_recv_size;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_BUFFERED_MIN:
		assert(sizeof(rxm_ep->buffered_min) == sizeof(size_t));
		*(size_t *)optval = rxm_ep->buffered_min;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_BUFFERED_LIMIT:
		assert(sizeof(rxm_ep->buffered_limit) == sizeof(size_t));
		*(size_t *)optval = rxm_ep->buffered_limit;
		*optlen = sizeof(size_t);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}
	return FI_SUCCESS;
}

static int rxm_ep_setopt(fid_t fid, int level, int optname,
			 const void *optval, size_t optlen)
{
	struct rxm_ep *rxm_ep =
		container_of(fid, struct rxm_ep, util_ep.ep_fid);
	int ret = FI_SUCCESS;

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		rxm_ep->min_multi_recv_size = *(size_t *)optval;
		FI_INFO(&rxm_prov, FI_LOG_CORE,
			"FI_OPT_MIN_MULTI_RECV set to %zu\n",
			rxm_ep->min_multi_recv_size);
		break;
	case FI_OPT_BUFFERED_MIN:
		if (rxm_ep->rx_pool) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"Endpoint already enabled. Can't set opt now!\n");
			ret = -FI_EOPBADSTATE;
		} else if (*(size_t *)optval > rxm_ep->buffered_limit) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Invalid value for FI_OPT_BUFFERED_MIN: %zu "
			"( > FI_OPT_BUFFERED_LIMIT: %zu)\n",
			*(size_t *)optval, rxm_ep->buffered_limit);
			ret = -FI_EINVAL;
		} else {
			rxm_ep->buffered_min = *(size_t *)optval;
			FI_INFO(&rxm_prov, FI_LOG_CORE,
				"FI_OPT_BUFFERED_MIN set to %zu\n",
				rxm_ep->buffered_min);
		}
		break;
	case FI_OPT_BUFFERED_LIMIT:
		if (rxm_ep->rx_pool) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"Endpoint already enabled. Can't set opt now!\n");
			ret = -FI_EOPBADSTATE;
		/* We do not check for maximum as we allow sizes up to SIZE_MAX */
		} else if (*(size_t *)optval < rxm_ep->buffered_min) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Invalid value for FI_OPT_BUFFERED_LIMIT: %zu"
			" ( < FI_OPT_BUFFERED_MIN: %zu)\n",
			*(size_t *)optval, rxm_ep->buffered_min);
			ret = -FI_EINVAL;
		} else {
			rxm_ep->buffered_limit = *(size_t *)optval;
			FI_INFO(&rxm_prov, FI_LOG_CORE,
				"FI_OPT_BUFFERED_LIMIT set to %zu\n",
				rxm_ep->buffered_limit);
		}
		break;
	case FI_OPT_CUDA_API_PERMITTED:
		if (!hmem_ops[FI_HMEM_CUDA].initialized) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"FI_OPT_CUDA_API_PERMITTED cannot be set "
				"when CUDA library or CUDA device is not "
				"available.");
			ret = -FI_EINVAL;
			break;
		}

		/* if direct send is enabled, we directly pass CUDA buffer to
		 * the msg endpoint, therefore we do not need to copy the buffer
		 * and we do not need to call CUDA API.
		 */
		ret = rxm_ep->enable_direct_send ? FI_SUCCESS : -FI_EOPNOTSUPP;
		break;

	default:
		ret = -FI_ENOPROTOOPT;
	}
	return ret;
}

static struct fi_ops_ep rxm_ops_ep = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = rxm_ep_cancel,
	.getopt = rxm_ep_getopt,
	.setopt = rxm_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};


/* Caller must hold recv_queue->lock -- TODO which lock? */
struct rxm_rx_buf *
rxm_get_unexp_msg(struct rxm_recv_queue *recv_queue, fi_addr_t addr,
		  uint64_t tag, uint64_t ignore)
{
	struct rxm_recv_match_attr match_attr;
	struct dlist_entry *entry;

	if (dlist_empty(&recv_queue->unexp_msg_list))
		return NULL;

	match_attr.addr = addr;
	match_attr.tag = tag;
	match_attr.ignore = ignore;

	entry = dlist_find_first_match(&recv_queue->unexp_msg_list,
				       recv_queue->match_unexp, &match_attr);
	if (!entry)
		return NULL;

	RXM_DBG_ADDR_TAG(FI_LOG_EP_DATA, "Match for posted recv found in unexp"
			 " msg list\n", match_attr.addr, match_attr.tag);

	return container_of(entry, struct rxm_rx_buf, unexp_msg.entry);
}

static void rxm_recv_entry_init_common(struct rxm_recv_entry *recv_entry,
		const struct iovec *iov, void **desc, size_t count,
		fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		void *context, uint64_t flags,
		struct rxm_recv_queue *recv_queue)
{
	size_t i;

	assert(!recv_entry->rndv.tx_buf);
	recv_entry->rxm_iov.count = (uint8_t) count;
	recv_entry->addr = src_addr;
	recv_entry->context = context;
	recv_entry->flags = flags;
	recv_entry->ignore = ignore;
	recv_entry->tag = tag;

	recv_entry->sar.msg_id = RXM_SAR_RX_INIT;
	recv_entry->sar.total_recv_len = 0;
	recv_entry->total_len = 0;

	for (i = 0; i < count; i++) {
		recv_entry->rxm_iov.iov[i] = iov[i];
		recv_entry->total_len += iov[i].iov_len;
		if (desc && desc[i])
			recv_entry->rxm_iov.desc[i] = desc[i];
		else
			recv_entry->rxm_iov.desc[i] = NULL;
	}
}

struct rxm_recv_entry *
rxm_recv_entry_get(struct rxm_ep *rxm_ep, const struct iovec *iov,
		   void **desc, size_t count, fi_addr_t src_addr,
		   uint64_t tag, uint64_t ignore, void *context,
		   uint64_t flags, struct rxm_recv_queue *recv_queue)
{
	struct rxm_recv_entry *recv_entry;

	if (ofi_freestack_isempty(recv_queue->fs))
		return NULL;

	recv_entry = ofi_freestack_pop(recv_queue->fs);

	rxm_recv_entry_init_common(recv_entry, iov, desc, count, src_addr, tag,
			    ignore, context, flags, recv_queue);

	return recv_entry;
}

struct rxm_recv_entry *
rxm_multi_recv_entry_get(struct rxm_ep *rxm_ep, const struct iovec *iov,
		   void **desc, size_t count, fi_addr_t src_addr,
		   uint64_t tag, uint64_t ignore, void *context,
		   uint64_t flags)
{
	struct rxm_recv_entry *recv_entry;

	recv_entry = ofi_buf_alloc(rxm_ep->multi_recv_pool);

	rxm_recv_entry_init_common(recv_entry, iov, desc, count, src_addr, tag,
			    ignore, context, flags, NULL);

	recv_entry->comp_flags = FI_MSG | FI_RECV;
	return recv_entry;
}

struct rxm_tx_buf *rxm_get_tx_buf(struct rxm_ep *ep)
{
	struct rxm_tx_buf *buf;

	assert(ofi_genlock_held(&ep->util_ep.lock));
	if (!ep->tx_credit)
		return NULL;

	buf = ofi_buf_alloc(ep->tx_pool);
	if (buf) {
		OFI_DBG_SET(buf->user_tx, true);
		ep->tx_credit--;
	}
	return buf;
}

void rxm_free_tx_buf(struct rxm_ep *ep, struct rxm_tx_buf *buf)
{
	assert(ofi_genlock_held(&ep->util_ep.lock));
	assert(buf->user_tx);
	OFI_DBG_SET(buf->user_tx, false);
	ep->tx_credit++;
	ofi_buf_free(buf);
}

struct rxm_coll_buf *rxm_get_coll_buf(struct rxm_ep *ep)
{
	assert(ofi_genlock_held(&ep->util_ep.lock));
	return ofi_buf_alloc(ep->coll_pool);
}

void rxm_free_coll_buf(struct rxm_ep *ep, struct rxm_coll_buf *buf)
{
	assert(ofi_genlock_held(&ep->util_ep.lock));
	ofi_buf_free(buf);
}

void rxm_rndv_hdr_init(struct rxm_ep *rxm_ep, void *buf,
			      const struct iovec *iov, size_t count,
			      struct fid_mr **mr)
{
	struct rxm_rndv_hdr *rndv_hdr = (struct rxm_rndv_hdr *)buf;
	size_t i;

	for (i = 0; i < count && mr[i]; i++) {
		rndv_hdr->iov[i].addr = RXM_MR_VIRT_ADDR(rxm_ep->msg_info) ?
			(uintptr_t)iov[i].iov_base : 0;
		rndv_hdr->iov[i].len = (uint64_t)iov[i].iov_len;
		rndv_hdr->iov[i].key = fi_mr_key(mr[i]);
	}
	rndv_hdr->count = (uint8_t)count;
}

static void
rxm_ep_sar_tx_cleanup(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		      struct rxm_tx_buf *tx_buf)
{
	struct rxm_tx_buf *first_tx_buf;

	first_tx_buf = ofi_bufpool_get_ibuf(rxm_ep->tx_pool,
					    tx_buf->pkt.ctrl_hdr.msg_id);
	rxm_free_tx_buf(rxm_ep, first_tx_buf);
	rxm_free_tx_buf(rxm_ep, tx_buf);
}

struct rxm_deferred_tx_entry *
rxm_ep_alloc_deferred_tx_entry(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			       enum rxm_deferred_tx_entry_type type)
{
	struct rxm_deferred_tx_entry *def_tx_entry;

	def_tx_entry = calloc(1, sizeof(*def_tx_entry));
	if (!def_tx_entry)
		return NULL;

	def_tx_entry->rxm_ep = rxm_ep;
	def_tx_entry->rxm_conn = rxm_conn;
	def_tx_entry->type = type;
	dlist_init(&def_tx_entry->entry);

	return def_tx_entry;
}

static void
rxm_ep_sar_handle_segment_failure(struct rxm_deferred_tx_entry *def_tx_entry,
				ssize_t ret)
{
	rxm_ep_sar_tx_cleanup(def_tx_entry->rxm_ep, def_tx_entry->rxm_conn,
			      def_tx_entry->sar_seg.cur_seg_tx_buf);
	rxm_cq_write_error(def_tx_entry->rxm_ep->util_ep.tx_cq,
			   def_tx_entry->rxm_ep->util_ep.cntrs[CNTR_TX],
			   def_tx_entry->sar_seg.app_context, (int) ret);
}

/* Returns FI_SUCCESS if the SAR deferred TX queue is empty,
 * otherwise, it returns -FI_EAGAIN or error from MSG provider */
static ssize_t
rxm_ep_progress_sar_deferred_segments(struct rxm_deferred_tx_entry *def_tx_entry)
{
	ssize_t ret = 0;
	struct rxm_tx_buf *tx_buf = def_tx_entry->sar_seg.cur_seg_tx_buf;

	if (tx_buf) {
		ret = fi_send(def_tx_entry->rxm_conn->msg_ep, &tx_buf->pkt,
			      sizeof(tx_buf->pkt) + tx_buf->pkt.ctrl_hdr.seg_size,
			      tx_buf->hdr.desc, 0, tx_buf);
		if (ret) {
			if (ret != -FI_EAGAIN) {
				rxm_ep_sar_handle_segment_failure(def_tx_entry,
								  ret);
			}
			return ret;
		}

		def_tx_entry->sar_seg.next_seg_no++;
		def_tx_entry->sar_seg.remain_len -= rxm_buffer_size;

		if (def_tx_entry->sar_seg.next_seg_no ==
		    def_tx_entry->sar_seg.segs_cnt) {
			assert(rxm_sar_get_seg_type(&tx_buf->pkt.ctrl_hdr) ==
			       RXM_SAR_SEG_LAST);
			return 0;
		}
	}

	while (def_tx_entry->sar_seg.next_seg_no !=
	       def_tx_entry->sar_seg.segs_cnt) {
		ret = rxm_send_segment(
				def_tx_entry->rxm_ep, def_tx_entry->rxm_conn,
				def_tx_entry->sar_seg.app_context,
				def_tx_entry->sar_seg.total_len,
				def_tx_entry->sar_seg.remain_len,
				def_tx_entry->sar_seg.msg_id, rxm_buffer_size,
				def_tx_entry->sar_seg.next_seg_no,
				def_tx_entry->sar_seg.segs_cnt,
				def_tx_entry->sar_seg.payload.data,
				def_tx_entry->sar_seg.flags,
				def_tx_entry->sar_seg.payload.tag,
				def_tx_entry->sar_seg.op,
				def_tx_entry->sar_seg.payload.iov,
				def_tx_entry->sar_seg.payload.count,
				&def_tx_entry->sar_seg.payload.cur_iov_offset,
				&def_tx_entry->sar_seg.cur_seg_tx_buf,
				def_tx_entry->sar_seg.iface,
				def_tx_entry->sar_seg.device);
		if (ret) {
			if (ret != -FI_EAGAIN) {
				rxm_ep_sar_handle_segment_failure(def_tx_entry,
								  ret);
			}

			return ret;
		}
		def_tx_entry->sar_seg.next_seg_no++;
		def_tx_entry->sar_seg.remain_len -= rxm_buffer_size;
	}

	return 0;
}

void rxm_ep_progress_deferred_queue(struct rxm_ep *rxm_ep,
				    struct rxm_conn *rxm_conn)
{
	struct rxm_deferred_tx_entry *def_tx_entry;
	struct iovec iov;
	struct fi_msg msg;
	ssize_t ret = 0;

	if (rxm_conn->state != RXM_CM_CONNECTED)
		return;

	while (!dlist_empty(&rxm_conn->deferred_tx_queue) && !ret) {
		def_tx_entry = container_of(rxm_conn->deferred_tx_queue.next,
					    struct rxm_deferred_tx_entry, entry);
		switch (def_tx_entry->type) {
		case RXM_DEFERRED_TX_RNDV_ACK:
			ret = fi_send(def_tx_entry->rxm_conn->msg_ep,
				      &def_tx_entry->rndv_ack.rx_buf->
					recv_entry->rndv.tx_buf->pkt,
				      def_tx_entry->rndv_ack.pkt_size,
				      def_tx_entry->rndv_ack.rx_buf->recv_entry->
					rndv.tx_buf->hdr.desc,
				      0, def_tx_entry->rndv_ack.rx_buf);
			if (ret) {
				if (ret == -FI_EAGAIN)
					return;
				rxm_cq_write_error(def_tx_entry->rxm_ep->util_ep.rx_cq,
						   def_tx_entry->rxm_ep->util_ep.cntrs[CNTR_RX],
						   def_tx_entry->rndv_ack.rx_buf->
						   recv_entry->context, (int) ret);
			}
			if (def_tx_entry->rndv_ack.rx_buf->recv_entry->rndv
				    .tx_buf->pkt.ctrl_hdr
				    .type == rxm_ctrl_rndv_rd_done)
				RXM_UPDATE_STATE(FI_LOG_EP_DATA,
						 def_tx_entry->rndv_ack.rx_buf,
						 RXM_RNDV_READ_DONE_SENT);
			else
				RXM_UPDATE_STATE(FI_LOG_EP_DATA,
						 def_tx_entry->rndv_ack.rx_buf,
						 RXM_RNDV_WRITE_DATA_SENT);
			break;
		case RXM_DEFERRED_TX_RNDV_DONE:
			ret = fi_send(def_tx_entry->rxm_conn->msg_ep,
				      &def_tx_entry->rndv_done.tx_buf->write_rndv.done_buf->pkt,
				      sizeof(struct rxm_pkt),
				      def_tx_entry->rndv_done.tx_buf->write_rndv.done_buf->hdr.desc,
				      0, def_tx_entry->rndv_done.tx_buf);
			if (ret) {
				if (ret == -FI_EAGAIN)
					return;
				rxm_cq_write_error(def_tx_entry->rxm_ep->util_ep.tx_cq,
						   def_tx_entry->rxm_ep->util_ep.cntrs[CNTR_TX],
						   def_tx_entry->rndv_done.tx_buf, (int) ret);
			}
			RXM_UPDATE_STATE(FI_LOG_EP_DATA,
					 def_tx_entry->rndv_done.tx_buf,
					 RXM_RNDV_WRITE_DONE_SENT);
			break;
		case RXM_DEFERRED_TX_RNDV_READ:
			ret = rxm_ep->rndv_ops->xfer(
				def_tx_entry->rxm_conn->msg_ep,
				def_tx_entry->rndv_read.rxm_iov.iov,
				def_tx_entry->rndv_read.rxm_iov.desc,
				def_tx_entry->rndv_read.rxm_iov.count, 0,
				def_tx_entry->rndv_read.rma_iov.addr,
				def_tx_entry->rndv_read.rma_iov.key,
				def_tx_entry->rndv_read.rx_buf);
			if (ret) {
				if (ret == -FI_EAGAIN)
					return;
				rxm_cq_write_error(def_tx_entry->rxm_ep->util_ep.rx_cq,
						   def_tx_entry->rxm_ep->util_ep.cntrs[CNTR_RX],
						   def_tx_entry->rndv_read.rx_buf->
							recv_entry->context, (int) ret);
			}
			break;
		case RXM_DEFERRED_TX_RNDV_WRITE:
			ret = rxm_ep->rndv_ops->xfer(
				def_tx_entry->rxm_conn->msg_ep,
				def_tx_entry->rndv_write.rxm_iov.iov,
				def_tx_entry->rndv_write.rxm_iov.desc,
				def_tx_entry->rndv_write.rxm_iov.count, 0,
				def_tx_entry->rndv_write.rma_iov.addr,
				def_tx_entry->rndv_write.rma_iov.key,
				def_tx_entry->rndv_write.tx_buf);
			if (ret) {
				if (ret == -FI_EAGAIN)
					return;
				rxm_cq_write_error(def_tx_entry->rxm_ep->util_ep.rx_cq,
						   def_tx_entry->rxm_ep->util_ep.cntrs[CNTR_RX],
						   def_tx_entry->rndv_write.tx_buf, (int) ret);
			}
			break;
		case RXM_DEFERRED_TX_SAR_SEG:
			ret = rxm_ep_progress_sar_deferred_segments(def_tx_entry);
			if (ret == -FI_EAGAIN)
				return;
			break;
		case RXM_DEFERRED_TX_ATOMIC_RESP:
			ret = rxm_atomic_send_respmsg(rxm_ep,
					def_tx_entry->rxm_conn,
					def_tx_entry->atomic_resp.tx_buf,
					def_tx_entry->atomic_resp.len);
			if (ret == -FI_EAGAIN)
				return;
			break;
		case RXM_DEFERRED_TX_CREDIT_SEND:
			iov.iov_base = &def_tx_entry->credit_msg.tx_buf->pkt;
			iov.iov_len = sizeof(def_tx_entry->credit_msg.tx_buf->pkt);

			msg.addr = 0;
			msg.context = def_tx_entry->credit_msg.tx_buf;
			msg.data = 0;
			msg.desc = &def_tx_entry->credit_msg.tx_buf->hdr.desc;
			msg.iov_count = 1;
			msg.msg_iov = &iov;

			ret = fi_sendmsg(def_tx_entry->rxm_conn->msg_ep, &msg,
					 OFI_PRIORITY);
			if (ret) {
				if (ret != -FI_EAGAIN) {
					rxm_cq_write_error(
						def_tx_entry->rxm_ep->util_ep.rx_cq,
						def_tx_entry->rxm_ep->util_ep.cntrs[CNTR_RX],
						def_tx_entry->rndv_read.rx_buf->
							recv_entry->context, (int) ret);
				}
				return;
			}
			break;
		}

		rxm_dequeue_deferred_tx(def_tx_entry);
		free(def_tx_entry);
	}
}

static int rxm_ep_init_coll_req(struct rxm_ep *rxm_ep, int coll_op, uint64_t flags,
				void *context, struct rxm_coll_buf **req,
				struct fid_ep **coll_ep)
{
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	(*req) = rxm_get_coll_buf(rxm_ep);
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);

	if (!(*req))
		return -FI_EAGAIN;

	(*req)->ep = rxm_ep;
	(*req)->flags = flags;
	(*req)->app_context = context;

	if (flags & FI_PEER_TRANSFER)
		(*coll_ep) = rxm_ep->util_coll_ep;
	else if (rxm_ep->offload_coll_mask & BIT(coll_op))
		(*coll_ep) = rxm_ep->offload_coll_ep;
	else
		(*coll_ep) = rxm_ep->util_coll_ep;

	return 0;
}

static inline void rxm_ep_free_coll_req(struct rxm_ep *rxm_ep,
					struct rxm_coll_buf *req)
{
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	rxm_free_coll_buf(rxm_ep, req);
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
}

ssize_t rxm_ep_barrier2(struct fid_ep *ep, fi_addr_t coll_addr,
			uint64_t flags, void *context)
{
	struct rxm_ep *rxm_ep;
	struct fid_ep *coll_ep;
	struct rxm_coll_buf *req;
	ssize_t ret;

        rxm_ep = container_of(ep, struct rxm_ep, util_ep.ep_fid.fid);

	ret = rxm_ep_init_coll_req(rxm_ep, FI_BARRIER, flags, context,
				   &req, &coll_ep);
	if (ret)
		return ret;

	flags &= ~FI_PEER_TRANSFER;

	ret = fi_barrier2(coll_ep, coll_addr, flags, req);
	if (ret)
		rxm_ep_free_coll_req(rxm_ep, req);

	return ret;
}

ssize_t rxm_ep_barrier(struct fid_ep *ep, fi_addr_t coll_addr, void *context)
{
	return rxm_ep_barrier2(ep, coll_addr, 0, context);
}

ssize_t rxm_ep_allreduce(struct fid_ep *ep, const void *buf, size_t count,
			 void *desc, void *result, void *result_desc,
			 fi_addr_t coll_addr, enum fi_datatype datatype,
			 enum fi_op op, uint64_t flags, void *context)
{
	struct rxm_ep *rxm_ep;
	struct fid_ep *coll_ep;
	struct rxm_coll_buf *req;
	ssize_t ret;

        rxm_ep = container_of(ep, struct rxm_ep, util_ep.ep_fid.fid);

	ret = rxm_ep_init_coll_req(rxm_ep, FI_ALLREDUCE, flags, context,
				   &req, &coll_ep);
	if (ret)
		return ret;

	flags &= ~FI_PEER_TRANSFER;

	ret = fi_allreduce(coll_ep, buf, count, desc, result, result_desc,
			   coll_addr, datatype, op, flags, req);
	if (ret)
		rxm_ep_free_coll_req(rxm_ep, req);

	return ret;
}

ssize_t rxm_ep_allgather(struct fid_ep *ep, const void *buf, size_t count,
			 void *desc, void *result, void *result_desc,
			 fi_addr_t coll_addr, enum fi_datatype datatype,
			 uint64_t flags, void *context)
{
	struct rxm_ep *rxm_ep;
	struct fid_ep *coll_ep;
	struct rxm_coll_buf *req;
	ssize_t ret;

        rxm_ep = container_of(ep, struct rxm_ep, util_ep.ep_fid.fid);

	ret = rxm_ep_init_coll_req(rxm_ep, FI_ALLGATHER, flags, context,
				   &req, &coll_ep);
	if (ret)
		return ret;

	flags &= ~FI_PEER_TRANSFER;

	ret = fi_allgather(coll_ep, buf, count, desc, result, result_desc,
			   coll_addr, datatype, flags, req);
	if (ret)
		rxm_ep_free_coll_req(rxm_ep, req);

	return ret;
}

ssize_t rxm_ep_scatter(struct fid_ep *ep, const void *buf, size_t count,
		       void *desc, void *result, void *result_desc,
		       fi_addr_t coll_addr, fi_addr_t root_addr,
		       enum fi_datatype datatype, uint64_t flags,
		       void *context)
{
	struct rxm_ep *rxm_ep;
	struct fid_ep *coll_ep;
	struct rxm_coll_buf *req;
	ssize_t ret;

        rxm_ep = container_of(ep, struct rxm_ep, util_ep.ep_fid.fid);

	ret = rxm_ep_init_coll_req(rxm_ep, FI_SCATTER, flags, context,
				   &req, &coll_ep);
	if (ret)
		return ret;

	flags &= ~FI_PEER_TRANSFER;

	ret = fi_scatter(coll_ep, buf, count, desc, result, result_desc,
			 coll_addr, root_addr, datatype, flags, req);
	if (ret)
		rxm_ep_free_coll_req(rxm_ep, req);

	return ret;
}

ssize_t rxm_ep_broadcast(struct fid_ep *ep, void *buf, size_t count,
			 void *desc, fi_addr_t coll_addr, fi_addr_t root_addr,
			 enum fi_datatype datatype, uint64_t flags,
			 void *context)
{
	struct rxm_ep *rxm_ep;
	struct fid_ep *coll_ep;
	struct rxm_coll_buf *req;
	ssize_t ret;

        rxm_ep = container_of(ep, struct rxm_ep, util_ep.ep_fid.fid);

	ret = rxm_ep_init_coll_req(rxm_ep, FI_BROADCAST, flags, context,
				   &req, &coll_ep);
	if (ret)
		return ret;

	flags &= ~FI_PEER_TRANSFER;

	ret = fi_broadcast(coll_ep, buf, count, desc, coll_addr, root_addr,
			   datatype, flags, req);
	if (ret)
		rxm_ep_free_coll_req(rxm_ep, req);

	return ret;
}

static struct fi_ops_collective rxm_ops_collective = {
	.size = sizeof(struct fi_ops_collective),
	.barrier = rxm_ep_barrier,
	.barrier2 = rxm_ep_barrier2,
	.broadcast = rxm_ep_broadcast,
	.alltoall = fi_coll_no_alltoall,
	.allreduce = rxm_ep_allreduce,
	.allgather = rxm_ep_allgather,
	.reduce_scatter = fi_coll_no_reduce_scatter,
	.reduce = fi_coll_no_reduce,
	.scatter = rxm_ep_scatter,
	.gather = fi_coll_no_gather,
	.msg = fi_coll_no_msg,
};

static struct fi_ops_collective rxm_ops_collective_none = {
	.size = sizeof(struct fi_ops_collective),
	.barrier = fi_coll_no_barrier,
	.broadcast = fi_coll_no_broadcast,
	.alltoall = fi_coll_no_alltoall,
	.allreduce = fi_coll_no_allreduce,
	.allgather = fi_coll_no_allgather,
	.reduce_scatter = fi_coll_no_reduce_scatter,
	.reduce = fi_coll_no_reduce,
	.scatter = fi_coll_no_scatter,
	.gather = fi_coll_no_gather,
	.msg = fi_coll_no_msg,
};


static int rxm_listener_close(struct rxm_ep *ep)
{
	int ret;

	if (ep->msg_pep) {
		ret = fi_close(&ep->msg_pep->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close msg pep\n");
			return ret;
		}
		ep->msg_pep = NULL;
	}

	if (ep->msg_eq) {
		ret = fi_close(&ep->msg_eq->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close msg EQ\n");
			return ret;
		}
		ep->msg_eq = NULL;
	}
	return 0;
}

static int rxm_ep_close(struct fid *fid)
{
	struct rxm_ep *ep;
	int ret;

	ep = container_of(fid, struct rxm_ep, util_ep.ep_fid.fid);

	/* Stop listener thread to halt event processing before closing all
	 * connections.
	 */
	rxm_stop_listen(ep);
	rxm_freeall_conns(ep);
	ret = rxm_listener_close(ep);
	if (ret)
		return ret;

	rxm_ep_txrx_res_close(ep);
	if (ep->msg_srx) {
		ret = fi_close(&ep->msg_srx->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, \
				"Unable to close msg shared ctx\n");
			return ret;
		}
		ep->msg_srx = NULL;
	}

	if (ep->msg_cq) {
		ret = fi_close(&ep->msg_cq->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close msg CQ\n");
			return ret;
		}
		ep->msg_cq = NULL;
	}

	if (ep->util_coll_ep) {
		ret = fi_close(&ep->util_coll_ep->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close util coll EP\n");
			return ret;
		}
		ep->util_coll_ep = NULL;
	}

	if (ep->offload_coll_ep) {
		ret = fi_close(&ep->offload_coll_ep->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close offload coll EP\n");
			return ret;
		}
		ep->offload_coll_ep = NULL;
	}

	free(ep->inject_pkt);
	ofi_endpoint_close(&ep->util_ep);
	fi_freeinfo(ep->msg_info);
	fi_freeinfo(ep->rxm_info);
	free(ep);
	return 0;
}

static int rxm_ep_trywait_cq(void *arg)
{
	struct rxm_fabric *rxm_fabric;
	fid_t fid = arg;
	struct rxm_ep *rxm_ep = fid->context;
	struct fid *fids[1] = { &rxm_ep->msg_cq->fid };
	int ret;

	rxm_fabric = container_of(rxm_ep->util_ep.domain->fabric,
				  struct rxm_fabric, util_fabric);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);
	ret = fi_trywait(rxm_fabric->msg_fabric, fids, 1);
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static int rxm_ep_trywait_eq(void *arg)
{
	struct rxm_fabric *rxm_fabric;
	fid_t fid = arg;
	struct rxm_ep *rxm_ep = fid->context;
	struct fid *fids[1] = { &rxm_ep->msg_eq->fid };

	rxm_fabric = container_of(rxm_ep->util_ep.domain->fabric,
				  struct rxm_fabric, util_fabric);
	return fi_trywait(rxm_fabric->msg_fabric, fids, 1);
}

static int rxm_ep_wait_fd_add(struct rxm_ep *rxm_ep, struct util_wait *wait)
{
	int ret;

	ret = ofi_wait_add_fid(wait, &rxm_ep->msg_cq->fid, POLLIN,
			       rxm_ep_trywait_cq);

	if (ret || (rxm_ep->util_ep.domain->data_progress == FI_PROGRESS_AUTO &&
	    !(rxm_ep->util_ep.caps & FI_ATOMIC)))
		return ret;

	return ofi_wait_add_fid(wait, &rxm_ep->msg_eq->fid, POLLIN,
				rxm_ep_trywait_eq);
}

static bool rxm_needs_atomic_progress(const struct fi_info *info)
{
	return (info->caps & FI_ATOMIC) && info->domain_attr &&
		info->domain_attr->data_progress == FI_PROGRESS_AUTO;
}

static int rxm_msg_cq_fd_needed(struct rxm_ep *rxm_ep)
{
	return (rxm_needs_atomic_progress(rxm_ep->rxm_info) ||
		(rxm_ep->util_ep.tx_cq && rxm_ep->util_ep.tx_cq->wait) ||
		(rxm_ep->util_ep.rx_cq && rxm_ep->util_ep.rx_cq->wait) ||
		(rxm_ep->util_ep.cntrs[CNTR_TX] && rxm_ep->util_ep.cntrs[CNTR_TX]->wait) ||
		(rxm_ep->util_ep.cntrs[CNTR_RX] && rxm_ep->util_ep.cntrs[CNTR_RX]->wait) ||
		(rxm_ep->util_ep.cntrs[CNTR_WR] && rxm_ep->util_ep.cntrs[CNTR_WR]->wait) ||
		(rxm_ep->util_ep.cntrs[CNTR_RD] && rxm_ep->util_ep.cntrs[CNTR_RD]->wait) ||
		(rxm_ep->util_ep.cntrs[CNTR_REM_WR] && rxm_ep->util_ep.cntrs[CNTR_REM_WR]->wait) ||
		(rxm_ep->util_ep.cntrs[CNTR_REM_RD] && rxm_ep->util_ep.cntrs[CNTR_REM_RD]->wait));
}

static enum fi_wait_obj rxm_get_wait_obj(struct rxm_ep *ep)
{
	if (!rxm_msg_cq_fd_needed(ep))
		return FI_WAIT_NONE;

	if ((def_tcp_wait_obj != FI_WAIT_UNSPEC) &&
	    !strncasecmp(ep->msg_info->fabric_attr->prov_name, "tcp",
			 strlen("tcp"))) {
		return def_tcp_wait_obj;
	}
	return def_wait_obj;
}

static int rxm_ep_msg_cq_open(struct rxm_ep *rxm_ep)
{
	struct rxm_domain *domain;
	struct fi_cq_attr cq_attr = { 0 };
	struct util_cq *cq_list[] = {
		rxm_ep->util_ep.tx_cq,
		rxm_ep->util_ep.rx_cq,
	};
	struct util_cntr *cntr_list[] = {
		rxm_ep->util_ep.cntrs[CNTR_TX],
		rxm_ep->util_ep.cntrs[CNTR_RX],
		rxm_ep->util_ep.cntrs[CNTR_RD],
		rxm_ep->util_ep.cntrs[CNTR_WR],
		rxm_ep->util_ep.cntrs[CNTR_REM_RD],
		rxm_ep->util_ep.cntrs[CNTR_REM_WR],
	};
	int i, ret;

	domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
			      util_domain);

	cq_attr.size = rxm_ep->msg_info->rx_attr->size;
	if (rxm_ep->msg_info->ep_attr->rx_ctx_cnt != FI_SHARED_CONTEXT)
		cq_attr.size *= ofi_universe_size;
	cq_attr.size += rxm_ep->msg_info->tx_attr->size * ofi_universe_size;
	cq_attr.format = FI_CQ_FORMAT_DATA;
	cq_attr.wait_obj = rxm_get_wait_obj(rxm_ep);

	ret = fi_cq_open(domain->msg_domain, &cq_attr, &rxm_ep->msg_cq,
			 rxm_ep);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to open MSG CQ\n");
		return ret;
	}

	if (cq_attr.wait_obj != FI_WAIT_FD)
		return 0;

	for (i = 0; i < sizeof(cq_list) / sizeof(*cq_list); i++) {
		if (cq_list[i] && cq_list[i]->wait) {
			ret = rxm_ep_wait_fd_add(rxm_ep, cq_list[i]->wait);
			if (ret)
				goto err;
		}
	}

	for (i = 0; i < sizeof(cntr_list) / sizeof(*cntr_list); i++) {
		if (cntr_list[i] && cntr_list[i]->wait) {
			ret = rxm_ep_wait_fd_add(rxm_ep, cntr_list[i]->wait);
			if (ret)
				goto err;
		}
	}
	return 0;
err:
	fi_close(&rxm_ep->msg_cq->fid);
	rxm_ep->msg_cq = NULL;
	return ret;
}

static void rxm_ep_init_proto(struct rxm_ep *ep)
{
	size_t param;

	if (ep->eager_limit < rxm_buffer_size)
		ep->eager_limit = rxm_buffer_size;

	/* SAR segment size is capped at 64k. */
	if (ep->eager_limit > UINT16_MAX) {
		ep->sar_limit = ep->eager_limit;
		return;
	}

	if (!fi_param_get_size_t(&rxm_prov, "sar_limit", &param)) {
		if (param <= ep->eager_limit)
			ep->sar_limit = ep->eager_limit;
		else
			ep->sar_limit = param;
	} else {
		ep->sar_limit = ep->eager_limit * 8;
	}
}

/* Direct send works with verbs, provided that msg_mr_local == rdm_mr_local.
 * However, it fails consistently on HFI, with the receiving side getting
 * corrupted data beyond the first iov.  Only enable if MR_LOCAL is not
 * required (feature of tcp provider).
 */
static void rxm_config_direct_send(struct rxm_ep *ep)
{
	int ret = 1;

	if (ep->msg_mr_local)
		return;

	fi_param_get_bool(&rxm_prov, "enable_direct_send", &ret);
	ep->enable_direct_send = (ret != 0);
}


static void rxm_ep_settings_init(struct rxm_ep *rxm_ep)
{
	size_t max_prog_val;

	assert(rxm_ep->msg_info);

	max_prog_val = MIN(rxm_ep->msg_info->tx_attr->size,
			   rxm_ep->msg_info->rx_attr->size) / 2;
	rxm_ep->comp_per_progress = (rxm_ep->comp_per_progress > max_prog_val) ?
				    max_prog_val : rxm_ep->comp_per_progress;

	rxm_ep->msg_mr_local = ofi_mr_local(rxm_ep->msg_info);
	rxm_ep->rdm_mr_local = ofi_mr_local(rxm_ep->rxm_info);

	rxm_ep->inject_limit = rxm_ep->msg_info->tx_attr->inject_size;
	rxm_ep->tx_credit = rxm_ep->rxm_info->tx_attr->size;

	/* Favor a default buffered_min size that's small enough to be
	 * injected by FI_EP_MSG provider */
	assert(!rxm_ep->buffered_min);
	if (rxm_ep->inject_limit >
	    (sizeof(struct rxm_pkt) + sizeof(struct rxm_rndv_hdr)))
		rxm_ep->buffered_min = MIN((rxm_ep->inject_limit -
					    (sizeof(struct rxm_pkt) +
					     sizeof(struct rxm_rndv_hdr))),
					   rxm_buffer_size);

	assert(!rxm_ep->min_multi_recv_size);
	rxm_ep->min_multi_recv_size = rxm_buffer_size;

	assert(!rxm_ep->buffered_limit);
	rxm_ep->buffered_limit = rxm_buffer_size;

	rxm_config_direct_send(rxm_ep);
	rxm_ep_init_proto(rxm_ep);

 	FI_INFO(&rxm_prov, FI_LOG_CORE,
		"Settings:\n"
		"\t\t MR local: MSG - %d, RxM - %d\n"
		"\t\t Completions per progress: MSG - %zu\n"
	        "\t\t Buffered min: %zu\n"
	        "\t\t Min multi recv size: %zu\n"
	        "\t\t inject size: %zu\n"
		"\t\t Protocol limits: Eager: %zu, SAR: %zu\n",
		rxm_ep->msg_mr_local, rxm_ep->rdm_mr_local,
		rxm_ep->comp_per_progress, rxm_ep->buffered_min,
		rxm_ep->min_multi_recv_size, rxm_ep->inject_limit,
		rxm_ep->eager_limit, rxm_ep->sar_limit);
}

static int rxm_ep_txrx_res_open(struct rxm_ep *rxm_ep)
{
	int ret;

	ret = rxm_ep_create_pools(rxm_ep);
	if (ret)
		return ret;

	dlist_init(&rxm_ep->deferred_queue);

	ret = rxm_ep_rx_queue_init(rxm_ep);
	if (ret)
		goto err;

	return FI_SUCCESS;
err:
	ofi_bufpool_destroy(rxm_ep->coll_pool);
	ofi_bufpool_destroy(rxm_ep->rx_pool);
	ofi_bufpool_destroy(rxm_ep->tx_pool);
	rxm_ep->coll_pool = NULL;
	rxm_ep->rx_pool = NULL;
	rxm_ep->tx_pool = NULL;
	return ret;
}

static int rxm_ep_enable_check(struct rxm_ep *rxm_ep)
{
	if (!rxm_ep->util_ep.av)
		return -FI_EOPBADSTATE;

	if (ofi_needs_tx(rxm_ep->rxm_info->caps) && !rxm_ep->util_ep.tx_cq) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "missing Tx CQ\n");
		return -FI_ENOCQ;
	}

	if (rxm_ep->util_ep.rx_cq)
		return 0;

	if (ofi_needs_rx(rxm_ep->rxm_info->caps)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "missing Rx CQ\n");
		return -FI_ENOCQ;
	}

	if (rxm_ep->rxm_info->domain_attr->cq_data_size) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "user hinted that CQ data "
			"may be used but endpoint is missing recv CQ\n");
		return -FI_ENOCQ;
	}

	return 0;
}

static int rxm_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct rxm_ep *ep;
	int ret;

	ep = container_of(fid, struct rxm_ep, util_ep.ep_fid.fid);

	switch (command) {
	case FI_ENABLE:
		ret = rxm_ep_enable_check(ep);
		if (ret)
			return ret;

		/* Ensure atomics progress thread isn't started at this point.
		 * The progress thread should be started only after MSG CQ is
		 * opened to keep it simple (avoids progressing only MSG EQ first
		 * and then progressing both MSG EQ and MSG CQ once the latter
		 * is opened) */
		assert(!(ep->rxm_info->caps & FI_ATOMIC) || !ep->cm_thread);

		ret = rxm_ep_msg_cq_open(ep);
		if (ret)
			return ret;

		if (rxm_passthru_info(ep->rxm_info)) {
			ret = fi_ep_bind(ep->msg_srx, &ep->msg_cq->fid, FI_RECV);
			if (ret)
				return ret;
		}

		/* At the time of enabling endpoint, FI_OPT_BUFFERED_MIN,
		 * FI_OPT_BUFFERED_LIMIT should have been frozen so we can
		 * create the rendezvous protocol message pool with the right
		 * size */
		ret = rxm_ep_txrx_res_open(ep);
		if (ret)
			return ret;

		if (ep->msg_srx && !rxm_passthru_info(ep->rxm_info)) {
			ret = rxm_prepost_recv(ep, ep->msg_srx);
			if (ret)
				goto err;
		}

		ret = rxm_start_listen(ep);
		if (ret)
			goto err;

		break;
	default:
		return -FI_ENOSYS;
	}
	return 0;

err:
	/* TODO: cleanup all allocated resources on error */
	rxm_ep_txrx_res_close(ep);
	return ret;
}

static int rxm_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct rxm_ep *rxm_ep;
	struct rxm_av *rxm_av;
	struct rxm_cq *rxm_cq;
	struct rxm_eq *rxm_eq;
	int ret, retv = 0;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);

        ret = ofi_ep_bind(&rxm_ep->util_ep, bfid, flags);
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_AV:
		rxm_av = container_of(bfid, struct rxm_av, util_av.av_fid.fid);
		if (rxm_ep->util_coll_ep && rxm_av->util_coll_av) {
			ret = ofi_ep_fid_bind(&rxm_ep->util_coll_ep->fid,
					      &rxm_av->util_coll_av->fid,
					      flags);
			if (ret)
				retv = ret;
		}
		if (rxm_ep->offload_coll_ep && rxm_av->offload_coll_av) {
			ret = ofi_ep_fid_bind(&rxm_ep->offload_coll_ep->fid,
					      &rxm_av->offload_coll_av->fid,
					      flags);
			if (ret)
				retv = ret;
		}
		break;

	case FI_CLASS_CQ:
		rxm_cq = container_of(bfid, struct rxm_cq, util_cq.cq_fid.fid);
		if (rxm_ep->util_coll_ep && rxm_cq->util_coll_cq) {
			ret = ofi_ep_fid_bind(&rxm_ep->util_coll_ep->fid,
					      &rxm_cq->util_coll_cq->fid,
					      flags);
			if (ret)
				retv = ret;
		}
		if (rxm_ep->offload_coll_ep && rxm_cq->offload_coll_cq) {
			ret = ofi_ep_fid_bind(&rxm_ep->offload_coll_ep->fid,
					      &rxm_cq->offload_coll_cq->fid,
					      flags);
			if (ret)
				retv = ret;
		}
		break;

	case FI_CLASS_EQ:
		rxm_eq = container_of(bfid, struct rxm_eq, util_eq.eq_fid.fid);
		if (rxm_ep->util_coll_ep && rxm_eq->util_coll_eq) {
			ret = ofi_ep_fid_bind(&rxm_ep->util_coll_ep->fid,
					      &rxm_eq->util_coll_eq->fid,
					      flags);
			if (ret)
				retv = ret;
		}
		if (rxm_ep->offload_coll_ep && rxm_eq->offload_coll_eq) {
			ret = ofi_ep_fid_bind(&rxm_ep->offload_coll_ep->fid,
					      &rxm_eq->offload_coll_eq->fid,
					      flags);
			if (ret)
				retv = ret;
		}
		break;
	}

	return retv;
}

static struct fi_ops rxm_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_ep_close,
	.bind = rxm_ep_bind,
	.control = rxm_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int rxm_listener_open(struct rxm_ep *rxm_ep)
{
	struct fi_eq_attr eq_attr = {
		.wait_obj = FI_WAIT_UNSPEC,
		.flags = FI_WRITE,
	};
	struct rxm_fabric *rxm_fabric;
	int ret;

	rxm_fabric = container_of(rxm_ep->util_ep.domain->fabric,
				  struct rxm_fabric, util_fabric);
	ret = fi_eq_open(rxm_fabric->msg_fabric, &eq_attr, &rxm_ep->msg_eq,
			 rxm_ep);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Unable to open msg EQ\n");
		return ret;
	}

	ret = fi_passive_ep(rxm_fabric->msg_fabric, rxm_ep->msg_info,
			    &rxm_ep->msg_pep, rxm_ep);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Unable to open msg PEP\n");
		goto err;
	}

	ret = fi_pep_bind(rxm_ep->msg_pep, &rxm_ep->msg_eq->fid, 0);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to bind msg PEP to msg EQ\n");
		goto err;
	}

	return 0;
err:
	rxm_listener_close(rxm_ep);
	return ret;
}

static int rxm_open_core_res(struct rxm_ep *ep)
{
	struct rxm_domain *domain;
	int ret;

	domain = container_of(ep->util_ep.domain, struct rxm_domain, util_domain);
	ret = ofi_get_core_info(domain->util_domain.fabric->fabric_fid.api_version,
				NULL, NULL, 0, &rxm_util_prov, ep->rxm_info,
				NULL, rxm_info_to_core, &ep->msg_info);
	if (ret)
		return ret;

	if (ep->msg_info->ep_attr->rx_ctx_cnt == FI_SHARED_CONTEXT) {
		ret = fi_srx_context(domain->msg_domain, ep->msg_info->rx_attr,
				     &ep->msg_srx, NULL);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to open shared receive context\n");
			goto err1;
		}
	}

	ret = rxm_listener_open(ep);
	if (ret)
		goto err2;

	return 0;
err2:
	if (ep->msg_srx) {
		fi_close(&ep->msg_srx->fid);
		ep->msg_srx = NULL;
	}
err1:
	fi_freeinfo(ep->msg_info);
	ep->msg_info = NULL;
	return ret;
}

static ssize_t
rxm_prepare_deferred_rndv_read(struct rxm_deferred_tx_entry **def_tx_entry,
			       size_t index, struct iovec *iov,
			       void *desc[RXM_IOV_LIMIT], size_t count,
			       void *buf)
{
	uint8_t i;
	struct rxm_rx_buf *rx_buf = buf;

	*def_tx_entry = rxm_ep_alloc_deferred_tx_entry(rx_buf->ep, rx_buf->conn,
						       RXM_DEFERRED_TX_RNDV_READ);
	if (!*def_tx_entry)
		return -FI_ENOMEM;

	(*def_tx_entry)->rndv_read.rx_buf = rx_buf;
	(*def_tx_entry)->rndv_read.rma_iov.addr =
			rx_buf->remote_rndv_hdr->iov[index].addr;
	(*def_tx_entry)->rndv_read.rma_iov.key =
			rx_buf->remote_rndv_hdr->iov[index].key;

	for (i = 0; i < count; i++) {
		(*def_tx_entry)->rndv_read.rxm_iov.iov[i] = iov[i];
		(*def_tx_entry)->rndv_read.rxm_iov.desc[i] = desc[i];
	}
	(*def_tx_entry)->rndv_read.rxm_iov.count = (uint8_t) count;

	return 0;
}

static ssize_t
rxm_prepare_deferred_rndv_write(struct rxm_deferred_tx_entry **def_tx_entry,
			       size_t index, struct iovec *iov,
			       void *desc[RXM_IOV_LIMIT], size_t count,
			       void *buf)
{
	uint8_t i;
	struct rxm_tx_buf *tx_buf = buf;
	struct rxm_ep *rxm_ep = tx_buf->write_rndv.conn->ep;

	*def_tx_entry = rxm_ep_alloc_deferred_tx_entry(rxm_ep, tx_buf->write_rndv.conn,
						       RXM_DEFERRED_TX_RNDV_WRITE);
	if (!*def_tx_entry)
		return -FI_ENOMEM;

	(*def_tx_entry)->rndv_write.tx_buf = tx_buf;
	(*def_tx_entry)->rndv_write.rma_iov.addr =
			tx_buf->write_rndv.remote_hdr.iov[index].addr;
	(*def_tx_entry)->rndv_write.rma_iov.key =
			tx_buf->write_rndv.remote_hdr.iov[index].key;

	for (i = 0; i < count; i++) {
		(*def_tx_entry)->rndv_write.rxm_iov.iov[i] = iov[i];
		(*def_tx_entry)->rndv_write.rxm_iov.desc[i] = desc[i];
	}
	(*def_tx_entry)->rndv_write.rxm_iov.count = (uint8_t) count;

	return 0;
}

struct rxm_rndv_ops rxm_rndv_ops_read = {
	.rx_mr_access = FI_READ,
	.tx_mr_access = FI_REMOTE_READ,
	.handle_rx = rxm_rndv_read,
	.xfer = fi_readv,
	.defer_xfer = rxm_prepare_deferred_rndv_read
};

struct rxm_rndv_ops rxm_rndv_ops_write = {
	.rx_mr_access = FI_REMOTE_WRITE,
	.tx_mr_access = FI_WRITE,
	.handle_rx = rxm_rndv_send_wr_data,
	.xfer = fi_writev,
	.defer_xfer = rxm_prepare_deferred_rndv_write
};

int rxm_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep_fid, void *context)
{
	struct rxm_ep *rxm_ep;
	struct rxm_domain *rxm_domain;
	struct rxm_fabric *rxm_fabric;
	struct fi_peer_transfer_context peer_context = {
		.size = sizeof(peer_context),
	};
	int ret;

	rxm_ep = calloc(1, sizeof(*rxm_ep));
	if (!rxm_ep)
		return -FI_ENOMEM;

	rxm_ep->rxm_info = fi_dupinfo(info);
	if (!rxm_ep->rxm_info) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	if (fi_param_get_int(&rxm_prov, "comp_per_progress",
			     (int *)&rxm_ep->comp_per_progress))
		rxm_ep->comp_per_progress = 1;

	if (rxm_ep->rxm_info->caps & FI_COLLECTIVE) {
		ret = ofi_endpoint_init(domain, &rxm_util_prov, info,
					&rxm_ep->util_ep, context,
					&rxm_ep_progress_coll);
		if (ret)
			goto err1;

		rxm_domain = container_of(domain, struct rxm_domain,
					  util_domain.domain_fid);
		rxm_fabric = container_of(rxm_domain->util_domain.fabric,
					  struct rxm_fabric,
					  util_fabric.fabric_fid);
		peer_context.ep = &rxm_ep->util_ep.ep_fid;
		peer_context.info = info;
		ret = fi_endpoint(rxm_domain->util_coll_domain,
				  rxm_fabric->util_coll_info,
				  &rxm_ep->util_coll_ep,
				  &peer_context);
		if (ret)
			goto err2;

		rxm_ep->util_coll_peer_xfer_ops = peer_context.peer_ops;

		peer_context.peer_ops = NULL;
		if (rxm_domain->offload_coll_mask) {
			ret = fi_endpoint(rxm_domain->offload_coll_domain,
					  rxm_fabric->offload_coll_info,
					  &rxm_ep->offload_coll_ep,
					  &peer_context);
			if (ret)
				goto err2;

			rxm_ep->offload_coll_peer_xfer_ops =
				peer_context.peer_ops;
			rxm_ep->offload_coll_mask =
				rxm_domain->offload_coll_mask;
		}
	} else {
		ret = ofi_endpoint_init(domain, &rxm_util_prov, info,
					&rxm_ep->util_ep, context,
					&rxm_ep_progress);
		if (ret)
			goto err1;
	}

	ret = rxm_open_core_res(rxm_ep);
	if (ret)
		goto err2;

	rxm_ep_settings_init(rxm_ep);

	rxm_ep->inject_pkt = calloc(1, sizeof(*rxm_ep->inject_pkt) +
				       rxm_ep->inject_limit);
	if (!rxm_ep->inject_pkt) {
		ret = -FI_ENOMEM;
		goto err2;
	}
	rxm_ep->inject_pkt->ctrl_hdr.version = RXM_CTRL_VERSION;
	rxm_ep->inject_pkt->ctrl_hdr.type = rxm_ctrl_eager;
	rxm_ep->inject_pkt->hdr.version = OFI_OP_VERSION;

	*ep_fid = &rxm_ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &rxm_ep_fi_ops;
	(*ep_fid)->ops = &rxm_ops_ep;
	(*ep_fid)->cm = &rxm_ops_cm;

	if(rxm_ep->rxm_info->caps & FI_COLLECTIVE) {
		(*ep_fid)->collective = &rxm_ops_collective;
		rxm_ep->eager_ops = &coll_eager_ops;
	} else {
		(*ep_fid)->collective = &rxm_ops_collective_none;
		rxm_ep->eager_ops = &def_eager_ops;
	}

	if (rxm_use_write_rndv)
		rxm_ep->rndv_ops = &rxm_rndv_ops_write;
	else
		rxm_ep->rndv_ops = &rxm_rndv_ops_read;
	dlist_init(&rxm_ep->rndv_wait_list);

	if (rxm_passthru_info(info)) {
		(*ep_fid)->msg = &rxm_msg_thru_ops;
		(*ep_fid)->rma = &rxm_rma_thru_ops;
		(*ep_fid)->tagged = &rxm_tagged_thru_ops;
		rxm_ep->handle_comp = rxm_thru_comp;
		rxm_ep->handle_comp_error = rxm_thru_comp_error;
	} else {
		(*ep_fid)->msg = &rxm_msg_ops;
		(*ep_fid)->rma = &rxm_rma_ops;
		(*ep_fid)->tagged = &rxm_tagged_ops;
		rxm_ep->handle_comp = rxm_handle_comp;
		rxm_ep->handle_comp_error = rxm_handle_comp_error;
	}

	if (rxm_ep->rxm_info->caps & FI_ATOMIC)
		(*ep_fid)->atomic = &rxm_ops_atomic;

	dlist_init(&rxm_ep->loopback_list);

	return 0;
err2:
	if (rxm_ep->util_coll_ep)
		fi_close(&rxm_ep->util_coll_ep->fid);
	if (rxm_ep->offload_coll_ep)
		fi_close(&rxm_ep->offload_coll_ep->fid);
	ofi_endpoint_close(&rxm_ep->util_ep);
err1:
	if (rxm_ep->rxm_info)
		fi_freeinfo(rxm_ep->rxm_info);
	free(rxm_ep);
	return ret;
}
