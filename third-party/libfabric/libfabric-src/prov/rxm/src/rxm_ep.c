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
#include "ofi.h"
#include <ofi_util.h>
#include <ofi_coll.h>

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

static int rxm_match_unexp_msg(struct dlist_entry *item, const void *arg)
{
	struct rxm_recv_match_attr *attr = (struct rxm_recv_match_attr *)arg;
	struct rxm_unexp_msg *unexp_msg =
		container_of(item, struct rxm_unexp_msg, entry);
	return ofi_match_addr(attr->addr, unexp_msg->addr);
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
	return ofi_match_addr(attr->addr, unexp_msg->addr) &&
		ofi_match_tag(attr->tag, attr->ignore, unexp_msg->tag);
}

static int rxm_buf_reg(struct ofi_bufpool_region *region)
{
	struct rxm_buf_pool *pool = region->pool->attr.context;
	struct rxm_ep *rxm_ep = pool->rxm_ep;
	struct rxm_domain *rxm_domain;
	int ret;
	bool hmem_enabled = !!(rxm_ep->util_ep.caps & FI_HMEM);

	if (hmem_enabled) {
		ret = ofi_hmem_host_register(region->mem_region,
					region->pool->region_size);
		if (ret != FI_SUCCESS)
			return ret;
	}

	if ((pool->type == RXM_BUF_POOL_TX_INJECT) ||
	    !pool->rxm_ep->msg_mr_local)
		return 0;

	rxm_domain = container_of(pool->rxm_ep->util_ep.domain,
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

static void rxm_buf_init(struct ofi_bufpool_region *region, void *buf)
{
	struct rxm_buf_pool *pool = region->pool->attr.context;
	struct rxm_pkt *pkt;
	struct rxm_rx_buf *rx_buf;
	struct rxm_tx_base_buf *tx_base_buf;
	struct rxm_tx_eager_buf *tx_eager_buf;
	struct rxm_tx_sar_buf *tx_sar_buf;
	struct rxm_tx_rndv_buf *tx_rndv_buf;
	struct rxm_tx_atomic_buf *tx_atomic_buf;
	struct rxm_rma_buf *rma_buf;
	void *mr_desc;
	uint8_t type;

	if ((pool->type != RXM_BUF_POOL_TX_INJECT) &&
	    pool->rxm_ep->msg_mr_local) {
		mr_desc = fi_mr_desc((struct fid_mr *) region->context);
	} else {
		mr_desc = NULL;
	}

	switch (pool->type) {
	case RXM_BUF_POOL_RX:
		rx_buf = buf;
		rx_buf->ep = pool->rxm_ep;

		rx_buf->hdr.desc = mr_desc;
		pkt = NULL;
		type = rxm_ctrl_eager; /* This can be any value */
		break;
	case RXM_BUF_POOL_TX:
		tx_eager_buf = buf;
		tx_eager_buf->hdr.state = RXM_TX;

		tx_eager_buf->hdr.desc = mr_desc;
		pkt = &tx_eager_buf->pkt;
		type = rxm_ctrl_eager;
		break;
	case RXM_BUF_POOL_TX_INJECT:
		tx_base_buf = buf;
		tx_base_buf->hdr.state = RXM_INJECT_TX;

		pkt = &tx_base_buf->pkt;
		type = rxm_ctrl_eager;
		break;
	case RXM_BUF_POOL_TX_SAR:
		tx_sar_buf = buf;
		tx_sar_buf->hdr.state = RXM_SAR_TX;

		tx_sar_buf->hdr.desc = mr_desc;
		pkt = &tx_sar_buf->pkt;
		type = rxm_ctrl_seg;
		break;
	case RXM_BUF_POOL_TX_CREDIT:
		tx_base_buf = buf;
		tx_base_buf->hdr.state = RXM_CREDIT_TX;

		tx_base_buf->hdr.desc = mr_desc;
		pkt = &tx_base_buf->pkt;
		type = rxm_ctrl_credit;
		break;
	case RXM_BUF_POOL_TX_RNDV_REQ:
		tx_rndv_buf = buf;

		tx_rndv_buf->hdr.desc = mr_desc;
		pkt = &tx_rndv_buf->pkt;
		type = rxm_ctrl_rndv_req;
		break;
	case RXM_BUF_POOL_TX_ATOMIC:
		tx_atomic_buf = buf;

		tx_atomic_buf->hdr.desc = mr_desc;
		pkt = &tx_atomic_buf->pkt;
		type = rxm_ctrl_atomic;
		break;
	case RXM_BUF_POOL_TX_RNDV_RD_DONE:
		tx_base_buf = buf;
		tx_base_buf->pkt.hdr.op = ofi_op_msg;

		tx_base_buf->hdr.desc = mr_desc;
		pkt = &tx_base_buf->pkt;
		type = rxm_ctrl_rndv_rd_done;
		break;
	case RXM_BUF_POOL_TX_RNDV_WR_DONE:
		tx_base_buf = buf;
		tx_base_buf->pkt.hdr.op = ofi_op_msg;

		tx_base_buf->hdr.desc = mr_desc;
		pkt = &tx_base_buf->pkt;
		type = rxm_ctrl_rndv_wr_done;
		break;
	case RXM_BUF_POOL_TX_RNDV_WR_DATA:
		tx_base_buf = buf;
		tx_base_buf->pkt.hdr.op = ofi_op_msg;

		tx_base_buf->hdr.desc = mr_desc;
		pkt = &tx_base_buf->pkt;
		type = rxm_ctrl_rndv_wr_data;
		break;
	case RXM_BUF_POOL_RMA:
		rma_buf = buf;
		rma_buf->pkt.hdr.op = ofi_op_msg;
		rma_buf->hdr.state = RXM_RMA;

		rma_buf->hdr.desc = mr_desc;
		pkt = &rma_buf->pkt;
		type = rxm_ctrl_eager;
		break;
	default:
		assert(0);
		pkt = NULL;
		break;
	}

	if (pkt) {
		pkt->ctrl_hdr.version = RXM_CTRL_VERSION;
		pkt->hdr.version = OFI_OP_VERSION;
		pkt->ctrl_hdr.type = type;
	}
}

static void rxm_buf_close(struct ofi_bufpool_region *region)
{
	struct rxm_buf_pool *pool = region->pool->attr.context;
	struct rxm_ep *rxm_ep = pool->rxm_ep;
	bool hmem_enabled = !!(rxm_ep->util_ep.caps & FI_HMEM);

	if (hmem_enabled)
		ofi_hmem_host_unregister(region->mem_region);

	if ((rxm_ep->msg_mr_local) && (pool->type != RXM_BUF_POOL_TX_INJECT)) {
		/* We would get a (fid_mr *) in context but
		 * it is safe to cast it into (fid *) */
		fi_close(region->context);
	}
}

static void rxm_buf_pool_destroy(struct rxm_buf_pool *pool)
{
	/* This indicates whether the pool is allocated or not */
	if (pool->rxm_ep) {
		ofi_bufpool_destroy(pool->pool);
	}
}

static int rxm_buf_pool_create(struct rxm_ep *rxm_ep, size_t size,
			       size_t max_cnt, size_t chunk_count,
			       struct rxm_buf_pool *pool,
			       enum rxm_buf_pool_type type)
{
	int ret;
	struct ofi_bufpool_attr attr = {
		.size		= size,
		.alignment	= 16,
		.max_cnt	= max_cnt,
		.chunk_cnt	= chunk_count,
		.alloc_fn	= rxm_buf_reg,
		.free_fn	= rxm_buf_close,
		.init_fn	= rxm_buf_init,
		.context	= pool,
		.flags		= OFI_BUFPOOL_NO_TRACK | OFI_BUFPOOL_HUGEPAGES,
	};

	pool->rxm_ep = rxm_ep;
	pool->type = type;
	ret = ofi_bufpool_create_attr(&attr, &pool->pool);
	if (ret)
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to create buf pool\n");

	return ret;
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
	}
	// TODO cleanup recv_list and unexp msg list
}

static int rxm_ep_txrx_pool_create(struct rxm_ep *rxm_ep)
{
	int ret, i;
	size_t entry_sizes[] = {
		[RXM_BUF_POOL_RX] = rxm_eager_limit +
				    sizeof(struct rxm_rx_buf),
		[RXM_BUF_POOL_TX] = rxm_eager_limit +
				    sizeof(struct rxm_tx_eager_buf),
		[RXM_BUF_POOL_TX_INJECT] = rxm_ep->inject_limit +
					   sizeof(struct rxm_tx_base_buf),
		[RXM_BUF_POOL_TX_RNDV_RD_DONE] = sizeof(struct rxm_tx_base_buf),
		[RXM_BUF_POOL_TX_RNDV_WR_DONE] = sizeof(struct rxm_tx_base_buf),
		[RXM_BUF_POOL_TX_RNDV_REQ] = sizeof(struct rxm_rndv_hdr) +
					 rxm_ep->buffered_min +
					 sizeof(struct rxm_tx_rndv_buf),
		[RXM_BUF_POOL_TX_RNDV_WR_DATA] = sizeof(struct rxm_rndv_hdr) +
						   sizeof(struct rxm_tx_base_buf),
		[RXM_BUF_POOL_TX_ATOMIC] = rxm_eager_limit +
					 sizeof(struct rxm_tx_atomic_buf),
		[RXM_BUF_POOL_TX_SAR] = rxm_eager_limit +
					sizeof(struct rxm_tx_sar_buf),
		[RXM_BUF_POOL_TX_CREDIT] = sizeof(struct rxm_tx_base_buf),
		[RXM_BUF_POOL_RMA] = rxm_eager_limit +
				     sizeof(struct rxm_rma_buf),
	};

	dlist_init(&rxm_ep->repost_ready_list);

	rxm_ep->buf_pools = calloc(1, RXM_BUF_POOL_MAX *
				      sizeof(*rxm_ep->buf_pools));
	if (!rxm_ep->buf_pools)
		return -FI_ENOMEM;

	for (i = RXM_BUF_POOL_START; i < RXM_BUF_POOL_MAX; i++) {
		if ((i == RXM_BUF_POOL_TX_INJECT) &&
		    (rxm_ep->util_ep.domain->threading != FI_THREAD_SAFE))
			continue;

		ret = rxm_buf_pool_create(rxm_ep, entry_sizes[i],
					  (i == RXM_BUF_POOL_RX ||
					   i == RXM_BUF_POOL_TX_ATOMIC) ? 0 :
					  rxm_ep->rxm_info->tx_attr->size,
					  1024,
					  &rxm_ep->buf_pools[i], i);
		if (ret)
			goto err;
	}

	return FI_SUCCESS;

err:
	while (--i >= RXM_BUF_POOL_START)
		rxm_buf_pool_destroy(&rxm_ep->buf_pools[i]);
	free(rxm_ep->buf_pools);
	return ret;
}

static void rxm_ep_txrx_pool_destroy(struct rxm_ep *rxm_ep)
{
	size_t i;

	for (i = RXM_BUF_POOL_START; i < RXM_BUF_POOL_MAX; i++)
		rxm_buf_pool_destroy(&rxm_ep->buf_pools[i]);
	free(rxm_ep->buf_pools);
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

static void rxm_ep_rx_queue_close(struct rxm_ep *rxm_ep)
{
	rxm_recv_queue_close(&rxm_ep->trecv_queue);
	rxm_recv_queue_close(&rxm_ep->recv_queue);
}

/* It is safe to call this function, even if `rxm_ep_txrx_res_open`
 * has not yet been called */
static void rxm_ep_txrx_res_close(struct rxm_ep *rxm_ep)
{
	rxm_ep_rx_queue_close(rxm_ep);
	if (rxm_ep->multi_recv_pool)
		ofi_bufpool_destroy(rxm_ep->multi_recv_pool);
	if (rxm_ep->buf_pools)
		rxm_ep_txrx_pool_destroy(rxm_ep);
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
	struct fi_collective_addr *c_addr;

	if (!(flags & FI_COLLECTIVE))
		return -FI_ENOSYS;

	c_addr = (struct fi_collective_addr *) addr;
	return ofi_join_collective(ep, c_addr->coll_addr, c_addr->set, flags,
				   mc, context);
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

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
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
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return entry != NULL;
}

static ssize_t rxm_ep_cancel(fid_t fid_ep, void *context)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(fid_ep, struct rxm_ep, util_ep.ep_fid);
	if (!rxm_ep_cancel_recv(rxm_ep, &rxm_ep->recv_queue, context))
		rxm_ep_cancel_recv(rxm_ep, &rxm_ep->trecv_queue, context);

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
		if (rxm_ep->buf_pools) {
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
		if (rxm_ep->buf_pools) {
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

/* Caller must hold recv_queue->lock */
static struct rxm_rx_buf *
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

static int rxm_handle_unexp_sar(struct rxm_recv_queue *recv_queue,
				struct rxm_recv_entry *recv_entry,
				struct rxm_rx_buf *rx_buf)
{
	struct rxm_recv_match_attr match_attr;
	struct dlist_entry *entry;
	bool last;
	ssize_t ret;

	ret = rxm_handle_rx_buf(rx_buf);
	last = rxm_sar_get_seg_type(&rx_buf->pkt.ctrl_hdr) == RXM_SAR_SEG_LAST;
	if (ret || last)
		return ret;

	match_attr.addr = recv_entry->addr;
	match_attr.tag = recv_entry->tag;
	match_attr.ignore = recv_entry->ignore;

	dlist_foreach_container_safe(&recv_queue->unexp_msg_list,
					struct rxm_rx_buf, rx_buf,
					unexp_msg.entry, entry) {
		if (!recv_queue->match_unexp(&rx_buf->unexp_msg.entry,
						&match_attr))
			continue;
		/* Handle unordered completions from MSG provider */
		if ((rx_buf->pkt.ctrl_hdr.msg_id != recv_entry->sar.msg_id) ||
			((rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_seg)))
			continue;

		if (!rx_buf->conn) {
			rx_buf->conn = rxm_key2conn(rx_buf->ep,
							rx_buf->pkt.ctrl_hdr.conn_id);
		}
		if (recv_entry->sar.conn != rx_buf->conn)
			continue;
		rx_buf->recv_entry = recv_entry;
		dlist_remove(&rx_buf->unexp_msg.entry);
		last = rxm_sar_get_seg_type(&rx_buf->pkt.ctrl_hdr) ==
		       RXM_SAR_SEG_LAST;
		ret = rxm_handle_rx_buf(rx_buf);
		if (ret || last)
			break;
	}
	return ret;

}

static void rxm_ep_discard_recv(struct rxm_ep *rxm_ep, struct rxm_rx_buf *rx_buf,
			       void *context)
{
	RXM_DBG_ADDR_TAG(FI_LOG_EP_DATA, "Discarding message",
			 rx_buf->unexp_msg.addr, rx_buf->unexp_msg.tag);

	rxm_cq_write(rxm_ep->util_ep.rx_cq, context, FI_TAGGED | FI_RECV,
		     0, NULL, rx_buf->pkt.hdr.data, rx_buf->pkt.hdr.tag);
	rxm_rx_buf_free(rx_buf);
}

static void
rxm_ep_peek_recv(struct rxm_ep *rxm_ep, fi_addr_t addr, uint64_t tag,
		 uint64_t ignore, void *context, uint64_t flags,
		 struct rxm_recv_queue *recv_queue)
{
	struct rxm_rx_buf *rx_buf;
	int ret;

	RXM_DBG_ADDR_TAG(FI_LOG_EP_DATA, "Peeking message", addr, tag);

	rxm_ep_do_progress(&rxm_ep->util_ep);

	rx_buf = rxm_get_unexp_msg(recv_queue, addr, tag, ignore);
	if (!rx_buf) {
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Message not found\n");
		ret = ofi_cq_write_error_peek(rxm_ep->util_ep.rx_cq, tag,
					      context);
		if (ret)
			FI_WARN(&rxm_prov, FI_LOG_CQ, "Error writing to CQ\n");
		return;
	}

	FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Message found\n");

	if (flags & FI_DISCARD) {
		dlist_remove(&rx_buf->unexp_msg.entry);
		rxm_ep_discard_recv(rxm_ep, rx_buf, context);
		return;
	}

	if (flags & FI_CLAIM) {
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Marking message for Claim\n");
		((struct fi_context *)context)->internal[0] = rx_buf;
		dlist_remove(&rx_buf->unexp_msg.entry);
	}

	rxm_cq_write(rxm_ep->util_ep.rx_cq, context, FI_TAGGED | FI_RECV,
		     rx_buf->pkt.hdr.size, NULL,
		     rx_buf->pkt.hdr.data, rx_buf->pkt.hdr.tag);
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

static struct rxm_recv_entry *
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

/*
 * We don't expect to have unexpected messages when the app is using
 * multi-recv buffers.  Optimize for that case.
 *
 * If there are unexpected messages waiting when we post a mult-recv buffer,
 * we trim off the start of the buffer, treat it as a normal buffer, and pair
 * it with an unexpected message.  We continue doing this until either no
 * unexpected messages are left or the multi-recv buffer has been consumed.
 */
static ssize_t
rxm_ep_post_mrecv(struct rxm_ep *ep, const struct iovec *iov,
		 void **desc, void *context, uint64_t op_flags)
{
	struct rxm_recv_entry *recv_entry;
	struct rxm_rx_buf *rx_buf;
	struct iovec cur_iov = *iov;
	int ret;

	do {
		recv_entry = rxm_recv_entry_get(ep, &cur_iov, desc, 1,
						FI_ADDR_UNSPEC, 0, 0, context,
						op_flags, &ep->recv_queue);
		if (!recv_entry) {
			ret = -FI_ENOMEM;
			break;
		}

		rx_buf = rxm_get_unexp_msg(&ep->recv_queue, recv_entry->addr, 0,  0);
		if (!rx_buf) {
			dlist_insert_tail(&recv_entry->entry,
					  &ep->recv_queue.recv_list);
			return 0;
		}

		dlist_remove(&rx_buf->unexp_msg.entry);
		rx_buf->recv_entry = recv_entry;
		recv_entry->flags &= ~FI_MULTI_RECV;
		recv_entry->total_len = MIN(cur_iov.iov_len, rx_buf->pkt.hdr.size);
		recv_entry->rxm_iov.iov[0].iov_len = recv_entry->total_len;

		cur_iov.iov_base = (uint8_t *) cur_iov.iov_base + recv_entry->total_len;
		cur_iov.iov_len -= recv_entry->total_len;

		if (rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_seg)
			ret = rxm_handle_rx_buf(rx_buf);
		else
			ret = rxm_handle_unexp_sar(&ep->recv_queue, recv_entry,
						   rx_buf);

	} while (!ret && cur_iov.iov_len >= ep->min_multi_recv_size);

	if ((cur_iov.iov_len < ep->min_multi_recv_size) ||
	    (ret && cur_iov.iov_len != iov->iov_len)) {
		rxm_cq_write(ep->util_ep.rx_cq, context, FI_MULTI_RECV,
			     0, NULL, 0, 0);
	}

	return ret;
}

static ssize_t
rxm_ep_post_recv(struct rxm_ep *rxm_ep, const struct iovec *iov,
		 void **desc, size_t count, fi_addr_t src_addr,
		 void *context, uint64_t op_flags)
{
	struct rxm_recv_entry *recv_entry;
	struct rxm_rx_buf *rx_buf;

	assert(count <= rxm_ep->rxm_info->rx_attr->iov_limit);
	if (op_flags & FI_MULTI_RECV)
		return rxm_ep_post_mrecv(rxm_ep, iov, desc, context, op_flags);

	recv_entry = rxm_recv_entry_get(rxm_ep, iov, desc, count, src_addr,
					0, 0, context, op_flags,
					&rxm_ep->recv_queue);
	if (!recv_entry)
		return -FI_EAGAIN;

	rx_buf = rxm_get_unexp_msg(&rxm_ep->recv_queue, recv_entry->addr, 0,  0);
	if (!rx_buf) {
		dlist_insert_tail(&recv_entry->entry,
				  &rxm_ep->recv_queue.recv_list);
		return FI_SUCCESS;
	}

	dlist_remove(&rx_buf->unexp_msg.entry);
	rx_buf->recv_entry = recv_entry;

	if (rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_seg)
		return rxm_handle_rx_buf(rx_buf);
	else
		return rxm_handle_unexp_sar(&rxm_ep->recv_queue, recv_entry,
					    rx_buf);
}

static ssize_t
rxm_ep_recv_common(struct rxm_ep *rxm_ep, const struct iovec *iov,
		   void **desc, size_t count, fi_addr_t src_addr,
		   void *context, uint64_t op_flags)
{
	ssize_t ret;

	assert(rxm_ep->util_ep.rx_cq);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_ep_post_recv(rxm_ep, iov, desc, count, src_addr,
			       context, op_flags);
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t
rxm_ep_buf_recv(struct rxm_ep *rxm_ep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t src_addr,
		void *context, uint64_t flags)
{
	struct rxm_recv_entry *recv_entry;
	struct fi_recv_context *recv_ctx = context;
	struct rxm_rx_buf *rx_buf;
	ssize_t ret = 0;

	context = recv_ctx->context;
	rx_buf = container_of(recv_ctx, struct rxm_rx_buf, recv_context);

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	if (flags & FI_CLAIM) {
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA,
			"Claiming buffered receive\n");

		recv_entry = rxm_recv_entry_get(rxm_ep, iov, desc, count,
						src_addr, 0, 0, context,
						flags, &rxm_ep->recv_queue);
		if (!recv_entry) {
			ret = -FI_EAGAIN;
			goto unlock;
		}

		recv_entry->comp_flags |= FI_CLAIM;

		rx_buf->recv_entry = recv_entry;
		ret = rxm_handle_rx_buf(rx_buf);
	} else {
		assert(flags & FI_DISCARD);
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Discarding buffered receive\n");
		rxm_rx_buf_free(rx_buf);
	}
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t
rxm_ep_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct rxm_ep *rxm_ep = container_of(ep_fid, struct rxm_ep,
					     util_ep.ep_fid.fid);

	if (rxm_ep->rxm_info->mode & FI_BUFFERED_RECV)
		return rxm_ep_buf_recv(rxm_ep, msg->msg_iov, msg->desc,
				       msg->iov_count, msg->addr, msg->context,
				       flags | rxm_ep->util_ep.rx_msg_flags);

	return rxm_ep_recv_common(rxm_ep, msg->msg_iov, msg->desc,
				  msg->iov_count, msg->addr, msg->context,
				  flags | rxm_ep->util_ep.rx_msg_flags);

}

static ssize_t rxm_ep_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			   void *desc, fi_addr_t src_addr, void *context)
{
	struct rxm_ep *rxm_ep =
		container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	struct iovec iov = {
		.iov_base	= buf,
		.iov_len	= len,
	};

	return rxm_ep_recv_common(rxm_ep, &iov, &desc, 1, src_addr,
				  context, rxm_ep->util_ep.rx_op_flags);
}

static ssize_t rxm_ep_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
			    void **desc, size_t count, fi_addr_t src_addr,
			    void *context)
{
	struct rxm_ep *rxm_ep = container_of(ep_fid, struct rxm_ep,
					     util_ep.ep_fid.fid);

	return rxm_ep_recv_common(rxm_ep, iov, desc, count, src_addr,
				  context, rxm_ep->util_ep.rx_op_flags);
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

static ssize_t
rxm_ep_msg_inject_send(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		       struct rxm_pkt *tx_pkt, size_t pkt_size,
		       ofi_cntr_inc_func cntr_inc_func)
{
	FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Posting inject with length: %zu"
	       " tag: 0x%" PRIx64 "\n", pkt_size, tx_pkt->hdr.tag);

	assert((tx_pkt->hdr.flags & FI_REMOTE_CQ_DATA) || !tx_pkt->hdr.flags);
	assert(pkt_size <= rxm_ep->inject_limit);

	ssize_t ret = fi_inject(rxm_conn->msg_ep, tx_pkt, pkt_size, 0);
	if (ret == -FI_EAGAIN)
		rxm_ep_do_progress(&rxm_ep->util_ep);
	return ret;
}

static ssize_t
rxm_ep_msg_normal_send(struct rxm_conn *rxm_conn, struct rxm_pkt *tx_pkt,
		       size_t pkt_size, void *desc, void *context)
{
	FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Posting send with length: %zu"
	       " tag: 0x%" PRIx64 "\n", pkt_size, tx_pkt->hdr.tag);

	assert((tx_pkt->hdr.flags & FI_REMOTE_CQ_DATA) || !tx_pkt->hdr.flags);

	return fi_send(rxm_conn->msg_ep, tx_pkt, pkt_size, desc, 0, context);
}

static ssize_t
rxm_alloc_rndv_buf(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		   void *context, uint8_t count, const struct iovec *iov,
		   void **desc, size_t data_len, uint64_t data,
		   uint64_t flags, uint64_t tag, uint8_t op,
		   enum fi_hmem_iface iface, uint64_t device,
		   struct rxm_tx_rndv_buf **rndv_buf)
{
	struct fid_mr *rxm_mr_msg_mr[RXM_IOV_LIMIT];
	struct fid_mr **mr_iov;
	size_t len, i;
	ssize_t ret;

	*rndv_buf = rxm_tx_buf_alloc(rxm_ep, RXM_BUF_POOL_TX_RNDV_REQ);
	if (!*rndv_buf) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Ran out of buffers from RNDV buffer pool\n");
		return -FI_EAGAIN;
	}

	rxm_ep_format_tx_buf_pkt(rxm_conn, data_len, op, data, tag,
				 flags, &(*rndv_buf)->pkt);
	(*rndv_buf)->pkt.ctrl_hdr.msg_id = ofi_buf_index(*rndv_buf);
	(*rndv_buf)->app_context = context;
	(*rndv_buf)->flags = flags;
	(*rndv_buf)->count = count;

	if (!rxm_ep->rdm_mr_local) {
		ret = rxm_msg_mr_regv(rxm_ep, iov, (*rndv_buf)->count, data_len,
				      rxm_ep->rndv_ops->tx_mr_access,
				      (*rndv_buf)->mr);
		if (ret)
			goto err;
		mr_iov = (*rndv_buf)->mr;
	} else {
		for (i = 0; i < count; i++)
			rxm_mr_msg_mr[i] = ((struct rxm_mr *) desc[i])->msg_mr;

		mr_iov = rxm_mr_msg_mr;
	}

	if (rxm_ep->rndv_ops == &rxm_rndv_ops_write) {
		(*rndv_buf)->write_rndv.conn = rxm_conn;
		for (i = 0; i < count; i++) {
			(*rndv_buf)->write_rndv.iov[i] = iov[i];
			(*rndv_buf)->write_rndv.desc[i] = fi_mr_desc(mr_iov[i]);
		}
	}

	rxm_rndv_hdr_init(rxm_ep, &(*rndv_buf)->pkt.data, iov,
			  (*rndv_buf)->count, mr_iov);

	len = sizeof(struct rxm_pkt) + sizeof(struct rxm_rndv_hdr);

	if (rxm_ep->rxm_info->mode & FI_BUFFERED_RECV) {
		ret = ofi_copy_from_hmem_iov(rxm_pkt_rndv_data(&(*rndv_buf)->pkt),
					     rxm_ep->buffered_min, iface,
					     device, iov, count, 0);
		assert(ret == rxm_ep->buffered_min);

		len += rxm_ep->buffered_min;
	}

	return len;

err:
	ofi_buf_free(*rndv_buf);
	return ret;
}

static ssize_t
rxm_ep_rndv_tx_send(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		   struct rxm_tx_rndv_buf *tx_buf, size_t pkt_size)
{
	ssize_t ret;

	RXM_UPDATE_STATE(FI_LOG_EP_DATA, tx_buf, RXM_RNDV_TX);
	if (pkt_size <= rxm_ep->inject_limit) {
		if (rxm_ep->rndv_ops == &rxm_rndv_ops_write)
			RXM_UPDATE_STATE(FI_LOG_EP_DATA, tx_buf, RXM_RNDV_WRITE_DATA_WAIT);
		else
			RXM_UPDATE_STATE(FI_LOG_EP_DATA, tx_buf, RXM_RNDV_READ_DONE_WAIT);
		ret = rxm_ep_msg_inject_send(rxm_ep, rxm_conn, &tx_buf->pkt,
					     pkt_size, ofi_cntr_inc_noop);
	} else {
		tx_buf->hdr.state = RXM_RNDV_TX;

		ret = rxm_ep_msg_normal_send(rxm_conn, &tx_buf->pkt, pkt_size,
					     tx_buf->hdr.desc, tx_buf);
	}
	if (ret)
		goto err;
	return FI_SUCCESS;
err:
	FI_DBG(&rxm_prov, FI_LOG_EP_DATA,
	       "Transmit for MSG provider failed\n");
	if (!rxm_ep->rdm_mr_local)
		rxm_msg_mr_closev(tx_buf->mr, tx_buf->count);
	ofi_buf_free(tx_buf);
	return ret;
}

static size_t
rxm_ep_sar_calc_segs_cnt(struct rxm_ep *rxm_ep, size_t data_len)
{
	return (data_len + rxm_eager_limit - 1) / rxm_eager_limit;
}

static struct rxm_tx_sar_buf *
rxm_ep_sar_tx_prepare_segment(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			      void *app_context, size_t total_len,
			      size_t seg_len, size_t seg_no, uint64_t data,
			      uint64_t flags, uint64_t tag, uint8_t op,
			      enum rxm_sar_seg_type seg_type, uint64_t *msg_id)
{
	struct rxm_tx_sar_buf *tx_buf;

	tx_buf = rxm_tx_buf_alloc(rxm_ep, RXM_BUF_POOL_TX_SAR);
	if (!tx_buf) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Ran out of buffers from SAR buffer pool\n");
		return NULL;
	};

	rxm_ep_format_tx_buf_pkt(rxm_conn, total_len, op, data, tag, flags,
				 &tx_buf->pkt);
	if (seg_type == RXM_SAR_SEG_FIRST) {
		*msg_id = tx_buf->pkt.ctrl_hdr.msg_id = ofi_buf_index(tx_buf);
	} else {
		tx_buf->pkt.ctrl_hdr.msg_id = *msg_id;
	}
	tx_buf->pkt.ctrl_hdr.seg_size = seg_len;
	tx_buf->pkt.ctrl_hdr.seg_no = seg_no;
	tx_buf->app_context = app_context;
	tx_buf->flags = flags;
	rxm_sar_set_seg_type(&tx_buf->pkt.ctrl_hdr, seg_type);

	return tx_buf;
}

static void
rxm_ep_sar_tx_cleanup(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		      struct rxm_tx_sar_buf *tx_buf)
{
	struct rxm_tx_sar_buf *first_tx_buf;

	first_tx_buf = ofi_bufpool_get_ibuf(rxm_ep->
					    buf_pools[RXM_BUF_POOL_TX_SAR].pool,
					    tx_buf->pkt.ctrl_hdr.msg_id);
	ofi_buf_free(first_tx_buf);
	ofi_buf_free(tx_buf);
}

static ssize_t
rxm_ep_sar_tx_prepare_and_send_segment(struct rxm_ep *rxm_ep,
		struct rxm_conn *rxm_conn, void *app_context, size_t data_len,
		size_t remain_len, uint64_t msg_id, size_t seg_len,
		size_t seg_no, size_t segs_cnt, uint64_t data, uint64_t flags,
		uint64_t tag, uint8_t op, const struct iovec *iov,
		uint8_t count, size_t *iov_offset,
		struct rxm_tx_sar_buf **out_tx_buf,
		enum fi_hmem_iface iface, uint64_t device)
{
	struct rxm_tx_sar_buf *tx_buf;
	enum rxm_sar_seg_type seg_type = RXM_SAR_SEG_MIDDLE;
	ssize_t ret __attribute__((unused));

	if (seg_no == (segs_cnt - 1)) {
		seg_type = RXM_SAR_SEG_LAST;
		assert(remain_len <= seg_len);
		seg_len = remain_len;
	}

	tx_buf = rxm_ep_sar_tx_prepare_segment(rxm_ep, rxm_conn, app_context,
					       data_len, seg_len, seg_no, data,
					       flags, tag, op, seg_type, &msg_id);
	if (!tx_buf) {
		*out_tx_buf = NULL;
		return -FI_EAGAIN;
	}

	ret = ofi_copy_from_hmem_iov(tx_buf->pkt.data, seg_len, iface, device,
				     iov, count, *iov_offset);
	assert(ret == seg_len);

	*iov_offset += seg_len;

	*out_tx_buf = tx_buf;

	return fi_send(rxm_conn->msg_ep, &tx_buf->pkt, sizeof(struct rxm_pkt) +
		       tx_buf->pkt.ctrl_hdr.seg_size, tx_buf->hdr.desc, 0, tx_buf);
}

static ssize_t
rxm_ep_sar_tx_send(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		   void *context, uint8_t count, const struct iovec *iov,
		   size_t data_len, size_t segs_cnt, uint64_t data,
		   uint64_t flags, uint64_t tag, uint8_t op,
		   enum fi_hmem_iface iface, uint64_t device)
{
	struct rxm_tx_sar_buf *tx_buf, *first_tx_buf;
	size_t i, iov_offset = 0, remain_len = data_len;
	ssize_t ret;
	struct rxm_deferred_tx_entry *def_tx;
	uint64_t msg_id = 0;

	assert(segs_cnt >= 2);

	first_tx_buf = rxm_ep_sar_tx_prepare_segment(rxm_ep, rxm_conn, context,
						     data_len, rxm_eager_limit,
						     0, data, flags, tag, op,
						     RXM_SAR_SEG_FIRST, &msg_id);
	if (!first_tx_buf)
		return -FI_EAGAIN;

	ret = ofi_copy_from_hmem_iov(first_tx_buf->pkt.data, rxm_eager_limit,
				     iface, device, iov, count, iov_offset);
	assert(ret == rxm_eager_limit);

	iov_offset += rxm_eager_limit;

	ret = fi_send(rxm_conn->msg_ep, &first_tx_buf->pkt,
		      sizeof(struct rxm_pkt) + first_tx_buf->pkt.ctrl_hdr.seg_size,
		      first_tx_buf->hdr.desc, 0, first_tx_buf);
	if (ret) {
		if (ret == -FI_EAGAIN)
			rxm_ep_do_progress(&rxm_ep->util_ep);
		ofi_buf_free(first_tx_buf);
		return ret;
	}

	remain_len -= rxm_eager_limit;

	for (i = 1; i < segs_cnt; i++) {
		ret = rxm_ep_sar_tx_prepare_and_send_segment(
					rxm_ep, rxm_conn, context, data_len, remain_len,
					msg_id, rxm_eager_limit, i, segs_cnt, data,
					flags, tag, op, iov, count, &iov_offset, &tx_buf,
					iface, device);
		if (ret) {
			if (ret == -FI_EAGAIN)
				goto defer;
			goto free;
		}
		remain_len -= rxm_eager_limit;
	}

	return 0;

free:
	ofi_buf_free(first_tx_buf);
	return ret;
defer:
	def_tx = rxm_ep_alloc_deferred_tx_entry(rxm_ep,
			rxm_conn, RXM_DEFERRED_TX_SAR_SEG);
	if (!def_tx) {
		if (tx_buf)
			ofi_buf_free(tx_buf);
		return -FI_ENOMEM;
	}
	memcpy(def_tx->sar_seg.payload.iov,
		iov, sizeof(*iov) * count);
	def_tx->sar_seg.payload.count = count;
	def_tx->sar_seg.payload.cur_iov_offset = iov_offset;
	def_tx->sar_seg.payload.tag = tag;
	def_tx->sar_seg.payload.data = data;
	def_tx->sar_seg.cur_seg_tx_buf = tx_buf;
	def_tx->sar_seg.app_context = context;
	def_tx->sar_seg.flags = flags;
	def_tx->sar_seg.op = op;
	def_tx->sar_seg.next_seg_no = i;
	def_tx->sar_seg.segs_cnt = segs_cnt;
	def_tx->sar_seg.total_len = data_len;
	def_tx->sar_seg.remain_len = remain_len;
	def_tx->sar_seg.msg_id = msg_id;
	def_tx->sar_seg.iface = iface;
	def_tx->sar_seg.device = device;
	rxm_ep_enqueue_deferred_tx_queue(def_tx);
	return 0;
}

static ssize_t
rxm_ep_emulate_inject(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		      const void *buf, size_t len, size_t pkt_size,
		      uint64_t data, uint64_t flags, uint64_t tag,
		      uint8_t op)
{
	struct rxm_tx_eager_buf *tx_buf;
	ssize_t ret;
	enum fi_hmem_iface iface = FI_HMEM_SYSTEM;
	const struct iovec iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};

	tx_buf = rxm_tx_buf_alloc(rxm_ep, RXM_BUF_POOL_TX);
	if (!tx_buf) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Ran out of buffers from Eager buffer pool\n");
		return -FI_EAGAIN;
	}
	/* This is needed so that we don't report bogus context in fi_cq_err_entry */
	tx_buf->app_context = NULL;

	rxm_ep_format_tx_buf_pkt(rxm_conn, len, op, data, tag, flags, &tx_buf->pkt);

	ret = ofi_copy_from_hmem_iov(tx_buf->pkt.data, len, iface, 0, &iov, 1,
				     0);
	assert(ret == len);

	tx_buf->flags = flags;

	ret = rxm_ep_msg_normal_send(rxm_conn, &tx_buf->pkt, pkt_size,
				     tx_buf->hdr.desc, tx_buf);
	if (ret) {
		if (ret == -FI_EAGAIN)
			rxm_ep_do_progress(&rxm_ep->util_ep);
		ofi_buf_free(tx_buf);
	}
	return ret;
}

static ssize_t
rxm_ep_inject_send_fast(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			const void *buf, size_t len, struct rxm_pkt *inject_pkt)
{
	size_t pkt_size = sizeof(struct rxm_pkt) + len;
	ssize_t ret;

	assert(len <= rxm_ep->rxm_info->tx_attr->inject_size);

	if (pkt_size <= rxm_ep->inject_limit && !rxm_ep->util_ep.tx_cntr) {
		inject_pkt->hdr.size = len;
		memcpy(inject_pkt->data, buf, len);
		ret = rxm_ep_msg_inject_send(rxm_ep, rxm_conn, inject_pkt,
					     pkt_size,
					     rxm_ep->util_ep.tx_cntr_inc);
	} else {
		ret = rxm_ep_emulate_inject(rxm_ep, rxm_conn, buf, len,
					    pkt_size, inject_pkt->hdr.data,
					    inject_pkt->hdr.flags,
					    inject_pkt->hdr.tag,
					    inject_pkt->hdr.op);
	}
	return ret;
}

static ssize_t
rxm_ep_inject_send(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		   const void *buf, size_t len, uint64_t data,
		   uint64_t flags, uint64_t tag, uint8_t op)
{
	struct rxm_tx_base_buf *tx_buf;
	size_t pkt_size = sizeof(struct rxm_pkt) + len;
	ssize_t ret;

	assert(len <= rxm_ep->rxm_info->tx_attr->inject_size);

	if (pkt_size <= rxm_ep->inject_limit &&
	    !rxm_ep->util_ep.tx_cntr) {
		tx_buf = rxm_tx_buf_alloc(rxm_ep, RXM_BUF_POOL_TX_INJECT);
		if (!tx_buf) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"Ran out of eager inject buffers\n");
			ret = -FI_EAGAIN;
			goto unlock;
		}
		rxm_ep_format_tx_buf_pkt(rxm_conn, len, op, data, tag,
					 flags, &tx_buf->pkt);
		memcpy(tx_buf->pkt.data, buf, len);

		ret = rxm_ep_msg_inject_send(rxm_ep, rxm_conn, &tx_buf->pkt,
					     pkt_size,
					     rxm_ep->util_ep.tx_cntr_inc);
		ofi_buf_free(tx_buf);
	} else {
		ret = rxm_ep_emulate_inject(rxm_ep, rxm_conn, buf, len,
					    pkt_size, data, flags, tag, op);
	}
unlock:
	return ret;

}

static bool
rxm_use_direct_send(struct rxm_ep *ep, size_t iov_count, uint64_t flags)
{
	return ep->enable_direct_send && !(flags & FI_INJECT) &&
		(iov_count < ep->msg_info->tx_attr->iov_limit);
}

static ssize_t
rxm_direct_send(struct rxm_ep *ep, struct rxm_conn *rxm_conn,
		struct rxm_tx_eager_buf *tx_buf,
		const struct iovec *iov, void **desc, size_t count)
{
	struct iovec send_iov[RXM_IOV_LIMIT];
	void *send_desc[RXM_IOV_LIMIT];
	struct rxm_mr *mr;
	ssize_t ret;
	int i;

	send_iov[0].iov_base = &tx_buf->pkt;
	send_iov[0].iov_len = sizeof(tx_buf->pkt);
	memcpy(send_iov + 1, iov, sizeof(*iov) * count);

	if (ep->msg_mr_local) {
		send_desc[0] = tx_buf->hdr.desc;

		for (i = 0; i < count; i++) {
			assert(desc[i]);
			mr = desc[i];
			send_desc[i + 1] = fi_mr_desc(mr->msg_mr);
		}

		ret = fi_sendv(rxm_conn->msg_ep, send_iov, send_desc,
			       count + 1, 0, tx_buf);
	} else {
		ret = fi_sendv(rxm_conn->msg_ep, send_iov, NULL,
			       count + 1, 0, tx_buf);
	}
	return ret;
}

static ssize_t
rxm_ep_send_common(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		   const struct iovec *iov, void **desc, size_t count,
		   void *context, uint64_t data, uint64_t flags, uint64_t tag,
		   uint8_t op)
{
	struct rxm_tx_eager_buf *eager_buf;
	struct rxm_tx_rndv_buf *rndv_buf;
	size_t data_len, total_len;
	ssize_t ret;
	enum fi_hmem_iface iface;
	uint64_t device;

	data_len = ofi_total_iov_len(iov, count);
	total_len = sizeof(struct rxm_pkt) + data_len;

	assert(count <= rxm_ep->rxm_info->tx_attr->iov_limit);
	assert((!(flags & FI_INJECT) &&
		(data_len > rxm_ep->rxm_info->tx_attr->inject_size)) ||
	       (data_len <= rxm_ep->rxm_info->tx_attr->inject_size));

	iface = rxm_mr_desc_to_hmem_iface_dev(desc, count, &device);

	if (data_len <= rxm_eager_limit) {
		eager_buf = rxm_tx_buf_alloc(rxm_ep, RXM_BUF_POOL_TX);
		if (!eager_buf) {
			FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
				"Ran out of buffers from Eager buffer pool\n");
			return -FI_EAGAIN;
		}

		eager_buf->app_context = context;
		eager_buf->flags = flags;
		rxm_ep_format_tx_buf_pkt(rxm_conn, data_len, op, data, tag,
					 flags, &eager_buf->pkt);

		if (rxm_use_direct_send(rxm_ep, count, flags)) {
			ret = rxm_direct_send(rxm_ep, rxm_conn, eager_buf,
					      iov, desc, count);
		} else {
			ret = ofi_copy_from_hmem_iov(eager_buf->pkt.data,
						     eager_buf->pkt.hdr.size,
						     iface, device, iov,
						     count, 0);
			assert(ret == eager_buf->pkt.hdr.size);

			ret = rxm_ep_msg_normal_send(rxm_conn, &eager_buf->pkt,
						     total_len,
						     eager_buf->hdr.desc,
						     eager_buf);
		}
		if (ret) {
			if (ret == -FI_EAGAIN)
				rxm_ep_do_progress(&rxm_ep->util_ep);
			ofi_buf_free(eager_buf);
		}
	} else if (data_len <= rxm_ep->sar_limit) {
		ret = rxm_ep_sar_tx_send(rxm_ep, rxm_conn, context,
					 count, iov, data_len,
					 rxm_ep_sar_calc_segs_cnt(rxm_ep, data_len),
					 data, flags, tag, op, iface, device);
	} else {
		ret = rxm_alloc_rndv_buf(rxm_ep, rxm_conn, context,
					 (uint8_t) count, iov, desc,
					 data_len, data, flags, tag, op,
					 iface, device, &rndv_buf);
		if (ret >= 0)
			ret = rxm_ep_rndv_tx_send(rxm_ep, rxm_conn,
						  rndv_buf, ret);
	}

	return ret;
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
			   def_tx_entry->rxm_ep->util_ep.tx_cntr,
			   def_tx_entry->sar_seg.app_context, ret);
}

/* Returns FI_SUCCESS if the SAR deferred TX queue is empty,
 * otherwise, it returns -FI_EAGAIN or error from MSG provider */
static ssize_t
rxm_ep_progress_sar_deferred_segments(struct rxm_deferred_tx_entry *def_tx_entry)
{
	ssize_t ret = 0;
	struct rxm_tx_sar_buf *tx_buf = def_tx_entry->sar_seg.cur_seg_tx_buf;

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
		def_tx_entry->sar_seg.remain_len -= rxm_eager_limit;

		if (def_tx_entry->sar_seg.next_seg_no ==
		    def_tx_entry->sar_seg.segs_cnt) {
			assert(rxm_sar_get_seg_type(&tx_buf->pkt.ctrl_hdr) ==
			       RXM_SAR_SEG_LAST);
			return 0;
		}
	}

	while (def_tx_entry->sar_seg.next_seg_no !=
	       def_tx_entry->sar_seg.segs_cnt) {
		ret = rxm_ep_sar_tx_prepare_and_send_segment(
				def_tx_entry->rxm_ep, def_tx_entry->rxm_conn,
				def_tx_entry->sar_seg.app_context,
				def_tx_entry->sar_seg.total_len,
				def_tx_entry->sar_seg.remain_len,
				def_tx_entry->sar_seg.msg_id, rxm_eager_limit,
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
		def_tx_entry->sar_seg.remain_len -= rxm_eager_limit;
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

	if (rxm_conn->handle.state != RXM_CMAP_CONNECTED)
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
						   def_tx_entry->rxm_ep->util_ep.rx_cntr,
						   def_tx_entry->rndv_ack.rx_buf->
						   recv_entry->context, ret);
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
						   def_tx_entry->rxm_ep->util_ep.tx_cntr,
						   def_tx_entry->rndv_done.tx_buf, ret);
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
						   def_tx_entry->rxm_ep->util_ep.rx_cntr,
						   def_tx_entry->rndv_read.rx_buf->
							recv_entry->context, ret);
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
						   def_tx_entry->rxm_ep->util_ep.rx_cntr,
						   def_tx_entry->rndv_write.tx_buf, ret);
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
					 FI_PRIORITY);
			if (ret) {
				if (ret != -FI_EAGAIN) {
					rxm_cq_write_error(
						def_tx_entry->rxm_ep->util_ep.rx_cq,
						def_tx_entry->rxm_ep->util_ep.rx_cntr,
						def_tx_entry->rndv_read.rx_buf->
							recv_entry->context, ret);
				}
				return;
			}
			break;
		}

		rxm_ep_dequeue_deferred_tx_queue(def_tx_entry);
		free(def_tx_entry);
	}
}

static ssize_t
rxm_ep_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, msg->addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, msg->msg_iov, msg->desc,
				 msg->iov_count, msg->context, msg->data,
				 flags | rxm_ep->util_ep.tx_msg_flags, 0, ofi_op_msg);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_send(struct fid_ep *ep_fid, const void *buf, size_t len,
			   void *desc, fi_addr_t dest_addr, void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	struct iovec iov = {
		.iov_base = (void *) buf,
		.iov_len = len,
	};
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, &iov, &desc, 1, context,
				  0, rxm_ep->util_ep.tx_op_flags, 0, ofi_op_msg);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
			    void **desc, size_t count, fi_addr_t dest_addr,
			    void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, iov, desc, count, context,
				  0, rxm_ep->util_ep.tx_op_flags, 0, ofi_op_msg);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
			     fi_addr_t dest_addr)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_inject_send(rxm_ep, rxm_conn, buf, len, 0,
				  rxm_ep->util_ep.inject_op_flags,
				  0, ofi_op_msg);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_inject_fast(struct fid_ep *ep_fid, const void *buf,
				  size_t len, fi_addr_t dest_addr)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		return ret;

	return rxm_ep_inject_send_fast(rxm_ep, rxm_conn, buf, len,
				       rxm_conn->inject_pkt);
}

static ssize_t rxm_ep_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			       void *desc, uint64_t data, fi_addr_t dest_addr,
			       void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	struct iovec iov = {
		.iov_base = (void *) buf,
		.iov_len = len,
	};
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, &iov, &desc, 1, context, data,
				 rxm_ep->util_ep.tx_op_flags | FI_REMOTE_CQ_DATA,
				 0, ofi_op_msg);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
				 uint64_t data, fi_addr_t dest_addr)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_inject_send(rxm_ep, rxm_conn, buf, len, data,
				  rxm_ep->util_ep.inject_op_flags |
				  FI_REMOTE_CQ_DATA, 0, ofi_op_msg);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_injectdata_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
				      uint64_t data, fi_addr_t dest_addr)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		return ret;

	rxm_conn->inject_data_pkt->hdr.data = data;

	return rxm_ep_inject_send_fast(rxm_ep, rxm_conn, buf, len,
				       rxm_conn->inject_data_pkt);
}

static struct fi_ops_msg rxm_ops_msg = {
	.size = sizeof(struct fi_ops_msg),
	.recv = rxm_ep_recv,
	.recvv = rxm_ep_recvv,
	.recvmsg = rxm_ep_recvmsg,
	.send = rxm_ep_send,
	.sendv = rxm_ep_sendv,
	.sendmsg = rxm_ep_sendmsg,
	.inject = rxm_ep_inject,
	.senddata = rxm_ep_senddata,
	.injectdata = rxm_ep_injectdata,
};

static struct fi_ops_msg rxm_ops_msg_thread_unsafe = {
	.size = sizeof(struct fi_ops_msg),
	.recv = rxm_ep_recv,
	.recvv = rxm_ep_recvv,
	.recvmsg = rxm_ep_recvmsg,
	.send = rxm_ep_send,
	.sendv = rxm_ep_sendv,
	.sendmsg = rxm_ep_sendmsg,
	.inject = rxm_ep_inject_fast,
	.senddata = rxm_ep_senddata,
	.injectdata = rxm_ep_injectdata_fast,
};

static ssize_t
rxm_ep_post_trecv(struct rxm_ep *rxm_ep, const struct iovec *iov,
		 void **desc, size_t count, fi_addr_t src_addr,
		 uint64_t tag, uint64_t ignore, void *context,
		 uint64_t op_flags)
{
	struct rxm_recv_entry *recv_entry;
	struct rxm_rx_buf *rx_buf;

	assert(count <= rxm_ep->rxm_info->rx_attr->iov_limit);

	recv_entry = rxm_recv_entry_get(rxm_ep, iov, desc, count, src_addr,
					tag, ignore, context, op_flags,
					&rxm_ep->trecv_queue);
	if (!recv_entry)
		return -FI_EAGAIN;

	rx_buf = rxm_get_unexp_msg(&rxm_ep->trecv_queue, recv_entry->addr,
				   recv_entry->tag, recv_entry->ignore);
	if (!rx_buf) {
		dlist_insert_tail(&recv_entry->entry,
				  &rxm_ep->trecv_queue.recv_list);
		return FI_SUCCESS;
	}

	dlist_remove(&rx_buf->unexp_msg.entry);
	rx_buf->recv_entry = recv_entry;

	if (rx_buf->pkt.ctrl_hdr.type != rxm_ctrl_seg)
		return rxm_handle_rx_buf(rx_buf);
	else
		return rxm_handle_unexp_sar(&rxm_ep->trecv_queue, recv_entry,
					    rx_buf);
}

static ssize_t
rxm_ep_trecv_common(struct rxm_ep *rxm_ep, const struct iovec *iov,
		   void **desc, size_t count, fi_addr_t src_addr,
		   uint64_t tag, uint64_t ignore, void *context,
		   uint64_t op_flags)
{
	ssize_t ret;

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_ep_post_trecv(rxm_ep, iov, desc, count, src_addr,
			        tag, ignore, context, op_flags);
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t
rxm_ep_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
		uint64_t flags)
{
	struct rxm_ep *rxm_ep;
	struct rxm_recv_entry *recv_entry;
	struct fi_recv_context *recv_ctx;
	struct rxm_rx_buf *rx_buf;
	void *context = msg->context;
	ssize_t ret = 0;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	flags |= rxm_ep->util_ep.rx_msg_flags;

	if (!(flags & (FI_CLAIM | FI_PEEK)) &&
	    !(rxm_ep->rxm_info->mode & FI_BUFFERED_RECV)) {
		return rxm_ep_trecv_common(rxm_ep, msg->msg_iov, msg->desc,
					   msg->iov_count, msg->addr,
					   msg->tag, msg->ignore, context,
					   flags);
	}

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	if (rxm_ep->rxm_info->mode & FI_BUFFERED_RECV) {
		recv_ctx = msg->context;
		context = recv_ctx->context;
		rx_buf = container_of(recv_ctx, struct rxm_rx_buf, recv_context);

		if (flags & FI_CLAIM) {
			FI_DBG(&rxm_prov, FI_LOG_EP_DATA,
			       "Claiming buffered receive\n");
			goto claim;
		}

		assert(flags & FI_DISCARD);
		FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Discarding buffered receive\n");
		rxm_rx_buf_free(rx_buf);
		goto unlock;
	}

	if (flags & FI_PEEK) {
		rxm_ep_peek_recv(rxm_ep, msg->addr, msg->tag, msg->ignore,
				 context, flags, &rxm_ep->trecv_queue);
		goto unlock;
	}

	rx_buf = ((struct fi_context *) context)->internal[0];
	assert(rx_buf);
	FI_DBG(&rxm_prov, FI_LOG_EP_DATA, "Claim message\n");

	if (flags & FI_DISCARD) {
		rxm_ep_discard_recv(rxm_ep, rx_buf, context);
		goto unlock;
	}

claim:
	assert (flags & FI_CLAIM);
	recv_entry = rxm_recv_entry_get(rxm_ep, msg->msg_iov, msg->desc,
					msg->iov_count, msg->addr,
					msg->tag, msg->ignore, context, flags,
					&rxm_ep->trecv_queue);
	if (!recv_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (rxm_ep->rxm_info->mode & FI_BUFFERED_RECV)
		recv_entry->comp_flags |= FI_CLAIM;

	rx_buf->recv_entry = recv_entry;
	ret = rxm_handle_rx_buf(rx_buf);

unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
			    void *desc, fi_addr_t src_addr, uint64_t tag,
			    uint64_t ignore, void *context)
{
	struct rxm_ep *rxm_ep;
	struct iovec iov = {
		.iov_base	= buf,
		.iov_len	= len,
	};

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_trecv_common(rxm_ep, &iov, &desc, 1, src_addr, tag, ignore,
				  context, rxm_ep->util_ep.rx_op_flags);
}

static ssize_t rxm_ep_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t src_addr,
			     uint64_t tag, uint64_t ignore, void *context)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_trecv_common(rxm_ep, iov, desc, count, src_addr, tag,
				   ignore, context, rxm_ep->util_ep.rx_op_flags);
}

static ssize_t
rxm_ep_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
		uint64_t flags)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, msg->addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, msg->msg_iov, msg->desc,
				  msg->iov_count, msg->context, msg->data,
				  flags | rxm_ep->util_ep.tx_msg_flags, msg->tag,
				  ofi_op_tagged);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
			    void *desc, fi_addr_t dest_addr, uint64_t tag,
			    void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	struct iovec iov = {
		.iov_base = (void *) buf,
		.iov_len = len,
	};
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, &iov, &desc, 1, context, 0,
				 rxm_ep->util_ep.tx_op_flags, tag, ofi_op_tagged);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t dest_addr,
			     uint64_t tag, void *context)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, iov, desc, count, context, 0,
				 rxm_ep->util_ep.tx_op_flags, tag, ofi_op_tagged);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			      fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_inject_send(rxm_ep, rxm_conn, buf, len, 0,
				  rxm_ep->util_ep.inject_op_flags, tag,
				  ofi_op_tagged);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_tinject_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
				   fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		return ret;

	rxm_conn->tinject_pkt->hdr.tag = tag;

	return rxm_ep_inject_send_fast(rxm_ep, rxm_conn, buf, len,
				       rxm_conn->tinject_pkt);
}

static ssize_t rxm_ep_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
				void *desc, uint64_t data, fi_addr_t dest_addr,
				uint64_t tag, void *context)
{
	struct rxm_conn *rxm_conn;
	struct iovec iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_send_common(rxm_ep, rxm_conn, &iov, &desc, 1, context, data,
				 rxm_ep->util_ep.tx_op_flags | FI_REMOTE_CQ_DATA,
				 tag, ofi_op_tagged);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
				  uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	ret = rxm_ep_inject_send(rxm_ep, rxm_conn, buf, len, data,
				  rxm_ep->util_ep.inject_op_flags |
				  FI_REMOTE_CQ_DATA, tag, ofi_op_tagged);
unlock:
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return ret;
}

static ssize_t rxm_ep_tinjectdata_fast(struct fid_ep *ep_fid, const void *buf, size_t len,
				       uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		return ret;

	rxm_conn->tinject_data_pkt->hdr.tag = tag;
	rxm_conn->tinject_data_pkt->hdr.data = data;

	return rxm_ep_inject_send_fast(rxm_ep, rxm_conn, buf, len,
				       rxm_conn->tinject_data_pkt);
}

static struct fi_ops_tagged rxm_ops_tagged = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = rxm_ep_trecv,
	.recvv = rxm_ep_trecvv,
	.recvmsg = rxm_ep_trecvmsg,
	.send = rxm_ep_tsend,
	.sendv = rxm_ep_tsendv,
	.sendmsg = rxm_ep_tsendmsg,
	.inject = rxm_ep_tinject,
	.senddata = rxm_ep_tsenddata,
	.injectdata = rxm_ep_tinjectdata,
};

static struct fi_ops_tagged rxm_ops_tagged_thread_unsafe = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = rxm_ep_trecv,
	.recvv = rxm_ep_trecvv,
	.recvmsg = rxm_ep_trecvmsg,
	.send = rxm_ep_tsend,
	.sendv = rxm_ep_tsendv,
	.sendmsg = rxm_ep_tsendmsg,
	.inject = rxm_ep_tinject_fast,
	.senddata = rxm_ep_tsenddata,
	.injectdata = rxm_ep_tinjectdata_fast,
};

static struct fi_ops_collective rxm_ops_collective = {
	.size = sizeof(struct fi_ops_collective),
	.barrier = ofi_ep_barrier,
	.broadcast = ofi_ep_broadcast,
	.alltoall = fi_coll_no_alltoall,
	.allreduce = ofi_ep_allreduce,
	.allgather = ofi_ep_allgather,
	.reduce_scatter = fi_coll_no_reduce_scatter,
	.reduce = fi_coll_no_reduce,
	.scatter = ofi_ep_scatter,
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

static int rxm_ep_msg_res_close(struct rxm_ep *rxm_ep)
{
	int ret = 0;

	if (rxm_ep->srx_ctx) {
		ret = fi_close(&rxm_ep->srx_ctx->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, \
				"Unable to close msg shared ctx\n");
		}
	}

	fi_freeinfo(rxm_ep->msg_info);
	return ret;
}

static int rxm_listener_close(struct rxm_ep *rxm_ep)
{
	int ret, retv = 0;

	if (rxm_ep->msg_pep) {
		ret = fi_close(&rxm_ep->msg_pep->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close msg pep\n");
			retv = ret;
		}
	}
	if (rxm_ep->msg_eq) {
		ret = fi_close(&rxm_ep->msg_eq->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to close msg EQ\n");
			retv = ret;
		}
	}
	return retv;
}

static int rxm_ep_close(struct fid *fid)
{
	int ret, retv = 0;
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(fid, struct rxm_ep, util_ep.ep_fid.fid);
	if (rxm_ep->cmap)
		rxm_cmap_free(rxm_ep->cmap);

	ret = rxm_listener_close(rxm_ep);
	if (ret)
		retv = ret;

	rxm_ep_txrx_res_close(rxm_ep);
	ret = rxm_ep_msg_res_close(rxm_ep);
	if (ret)
		retv = ret;

	if (rxm_ep->msg_cq) {
		ret = fi_close(&rxm_ep->msg_cq->fid);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Unable to close msg CQ\n");
			retv = ret;
		}
	}

	ofi_endpoint_close(&rxm_ep->util_ep);
	fi_freeinfo(rxm_ep->rxm_info);
	free(rxm_ep);
	return retv;
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
	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = fi_trywait(rxm_fabric->msg_fabric, fids, 1);
	ofi_ep_lock_release(&rxm_ep->util_ep);
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

static int rxm_msg_cq_fd_needed(struct rxm_ep *rxm_ep)
{
	return (rxm_needs_atomic_progress(rxm_ep->rxm_info) ||
		(rxm_ep->util_ep.tx_cq && rxm_ep->util_ep.tx_cq->wait) ||
		(rxm_ep->util_ep.rx_cq && rxm_ep->util_ep.rx_cq->wait) ||
		(rxm_ep->util_ep.tx_cntr && rxm_ep->util_ep.tx_cntr->wait) ||
		(rxm_ep->util_ep.rx_cntr && rxm_ep->util_ep.rx_cntr->wait) ||
		(rxm_ep->util_ep.wr_cntr && rxm_ep->util_ep.wr_cntr->wait) ||
		(rxm_ep->util_ep.rd_cntr && rxm_ep->util_ep.rd_cntr->wait) ||
		(rxm_ep->util_ep.rem_wr_cntr && rxm_ep->util_ep.rem_wr_cntr->wait) ||
		(rxm_ep->util_ep.rem_rd_cntr && rxm_ep->util_ep.rem_rd_cntr->wait));
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
	struct rxm_domain *rxm_domain;
	struct fi_cq_attr cq_attr = { 0 };
	struct util_cq *cq_list[] = {
		rxm_ep->util_ep.tx_cq,
		rxm_ep->util_ep.rx_cq,
	};
	struct util_cntr *cntr_list[] = {
		rxm_ep->util_ep.tx_cntr,
		rxm_ep->util_ep.rx_cntr,
		rxm_ep->util_ep.rd_cntr,
		rxm_ep->util_ep.wr_cntr,
		rxm_ep->util_ep.rem_rd_cntr,
		rxm_ep->util_ep.rem_wr_cntr,
	};
	int i, ret;

	cq_attr.size = rxm_ep->msg_info->rx_attr->size;
	if (rxm_ep->msg_info->ep_attr->rx_ctx_cnt != FI_SHARED_CONTEXT)
		cq_attr.size *= ofi_universe_size;
	cq_attr.size += rxm_ep->msg_info->tx_attr->size * ofi_universe_size;
	cq_attr.format = FI_CQ_FORMAT_DATA;
	cq_attr.wait_obj = rxm_get_wait_obj(rxm_ep);

	rxm_domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
				  util_domain);

	ret = fi_cq_open(rxm_domain->msg_domain, &cq_attr, &rxm_ep->msg_cq,
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

static void rxm_ep_sar_init(struct rxm_ep *rxm_ep)
{
	struct rxm_domain *domain;
	size_t param;

	/* SAR segment size is capped at 64k. */
	if (rxm_eager_limit > UINT16_MAX)
		goto disable_sar;

	domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
			      util_domain);
	if (domain->dyn_rbuf) {
		FI_INFO(&rxm_prov, FI_LOG_CORE, "Dynamic receive buffer "
			"enabled, disabling SAR protocol\n");
		goto disable_sar;
	}

	if (!fi_param_get_size_t(&rxm_prov, "sar_limit", &param)) {
		if (param <= rxm_eager_limit) {
			FI_WARN(&rxm_prov, FI_LOG_CORE,
				"Requested SAR limit (%zd) less or equal to "
				"eager limit (%zd) - disabling.",
				param, rxm_eager_limit);
			goto disable_sar;
		}

		rxm_ep->sar_limit = param;
	} else {
		rxm_ep->sar_limit = rxm_eager_limit * 8;
	}

	return;

disable_sar:
	rxm_ep->sar_limit = rxm_eager_limit;
}

static void rxm_config_direct_send(struct rxm_ep *ep)
{
	int ret = 0;

	if (ep->msg_mr_local == ep->rdm_mr_local)
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
	ofi_atomic_initialize32(&rxm_ep->atomic_tx_credits,
				rxm_ep->rxm_info->tx_attr->size);

	rxm_ep->msg_mr_local = ofi_mr_local(rxm_ep->msg_info);
	rxm_ep->rdm_mr_local = ofi_mr_local(rxm_ep->rxm_info);

	rxm_ep->inject_limit = rxm_ep->msg_info->tx_attr->inject_size;

	/* Favor a default buffered_min size that's small enough to be
	 * injected by FI_EP_MSG provider */
	assert(!rxm_ep->buffered_min);
	if (rxm_ep->inject_limit >
	    (sizeof(struct rxm_pkt) + sizeof(struct rxm_rndv_hdr)))
		rxm_ep->buffered_min = MIN((rxm_ep->inject_limit -
					    (sizeof(struct rxm_pkt) +
					     sizeof(struct rxm_rndv_hdr))),
					   rxm_eager_limit);

	assert(!rxm_ep->min_multi_recv_size);
	rxm_ep->min_multi_recv_size = rxm_eager_limit;

	assert(!rxm_ep->buffered_limit);
	rxm_ep->buffered_limit = rxm_eager_limit;

	rxm_ep_sar_init(rxm_ep);
	rxm_config_direct_send(rxm_ep);

 	FI_INFO(&rxm_prov, FI_LOG_CORE,
		"Settings:\n"
		"\t\t MR local: MSG - %d, RxM - %d\n"
		"\t\t Completions per progress: MSG - %zu\n"
	        "\t\t Buffered min: %zu\n"
	        "\t\t Min multi recv size: %zu\n"
	        "\t\t FI_EP_MSG provider inject size: %zu\n"
	        "\t\t rxm inject size: %zu\n"
		"\t\t Protocol limits: Eager: %zu, "
				      "SAR: %zu\n",
		rxm_ep->msg_mr_local, rxm_ep->rdm_mr_local,
		rxm_ep->comp_per_progress, rxm_ep->buffered_min,
		rxm_ep->min_multi_recv_size, rxm_ep->inject_limit,
		rxm_ep->rxm_info->tx_attr->inject_size,
		rxm_eager_limit, rxm_ep->sar_limit);
}

static int rxm_ep_txrx_res_open(struct rxm_ep *rxm_ep)
{
	int ret;

	ret = rxm_ep_txrx_pool_create(rxm_ep);
	if (ret)
		return ret;

	dlist_init(&rxm_ep->deferred_tx_conn_queue);

	ret = rxm_ep_rx_queue_init(rxm_ep);
	if (ret)
		goto err;

	return FI_SUCCESS;
err:
	rxm_ep_txrx_pool_destroy(rxm_ep);
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
	int ret;
	struct rxm_ep *rxm_ep
		= container_of(fid, struct rxm_ep, util_ep.ep_fid.fid);

	switch (command) {
	case FI_ENABLE:
		ret = rxm_ep_enable_check(rxm_ep);
		if (ret)
			return ret;

		/* Ensure atomics progress thread isn't started at this point.
		 * The progress thread should be started only after MSG CQ is
		 * opened to keep it simple (avoids progressing only MSG EQ first
		 * and then progressing both MSG EQ and MSG CQ once the latter
		 * is opened) */
		assert(!(rxm_ep->rxm_info->caps & FI_ATOMIC) ||
		       !rxm_ep->cmap || !rxm_ep->cmap->cm_thread);

		ret = rxm_ep_msg_cq_open(rxm_ep);
		if (ret)
			return ret;

		/* fi_listen should be called before cmap alloc as cmap alloc
		 * calls fi_getname on pep which would succeed only if fi_listen
		 * was called first */
		ret = fi_listen(rxm_ep->msg_pep);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"unable to set msg PEP to listen state\n");
			return ret;
		}

		ret = rxm_conn_cmap_alloc(rxm_ep);
		if (ret)
			return ret;

		/* At the time of enabling endpoint, FI_OPT_BUFFERED_MIN,
		 * FI_OPT_BUFFERED_LIMIT should have been frozen so we can
		 * create the rendezvous protocol message pool with the right
		 * size */
		ret = rxm_ep_txrx_res_open(rxm_ep);
		if (ret)
			return ret;

		if (rxm_ep->srx_ctx) {
			ret = rxm_prepost_recv(rxm_ep, rxm_ep->srx_ctx);
			if (ret) {
				rxm_cmap_free(rxm_ep->cmap);
				FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
					"unable to prepost recv bufs\n");
				goto err;
			}
		}
		break;
	default:
		return -FI_ENOSYS;
	}
	return 0;
err:
	rxm_ep_txrx_res_close(rxm_ep);
	return ret;
}

static struct fi_ops rxm_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_ep_close,
	.bind = ofi_ep_fid_bind,
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
				     &ep->srx_ctx, NULL);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Unable to open shared receive context\n");
			goto err1;
		}
	}

	ret = rxm_listener_open(ep);
	if (ret)
		goto err2;

	/* Zero out the port as we will create multiple MSG EPs for a
	 * single RXM EP, and we don't want address conflicts.
	 */
	if (ep->msg_info->src_addr)
		ofi_addr_set_port(ep->msg_info->src_addr, 0);

	return 0;
err2:
	if (ep->srx_ctx) {
		fi_close(&ep->srx_ctx->fid);
		ep->srx_ctx = NULL;
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
	(*def_tx_entry)->rndv_read.rxm_iov.count = count;

	return 0;
}

static ssize_t
rxm_prepare_deferred_rndv_write(struct rxm_deferred_tx_entry **def_tx_entry,
			       size_t index, struct iovec *iov,
			       void *desc[RXM_IOV_LIMIT], size_t count,
			       void *buf)
{
	uint8_t i;
	struct rxm_tx_rndv_buf *tx_buf = buf;
	struct rxm_ep *rxm_ep = tx_buf->write_rndv.conn->handle.cmap->ep;

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
	(*def_tx_entry)->rndv_write.rxm_iov.count = count;

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
	} else {
		ret = ofi_endpoint_init(domain, &rxm_util_prov, info,
					&rxm_ep->util_ep, context,
					&rxm_ep_progress);
	}

	if (ret)
		goto err1;

	ret = rxm_open_core_res(rxm_ep);
	if (ret)
		goto err2;

	rxm_ep_settings_init(rxm_ep);

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

	if (rxm_ep->util_ep.domain->threading != FI_THREAD_SAFE) {
		(*ep_fid)->msg = &rxm_ops_msg_thread_unsafe;
		(*ep_fid)->tagged = &rxm_ops_tagged_thread_unsafe;
	} else {
		(*ep_fid)->msg = &rxm_ops_msg;
		(*ep_fid)->tagged = &rxm_ops_tagged;
	}
	(*ep_fid)->rma = &rxm_ops_rma;

	if (rxm_ep->rxm_info->caps & FI_ATOMIC)
		(*ep_fid)->atomic = &rxm_ops_atomic;

	return 0;
err2:
	ofi_endpoint_close(&rxm_ep->util_ep);
err1:
	if (rxm_ep->rxm_info)
		fi_freeinfo(rxm_ep->rxm_info);
	free(rxm_ep);
	return ret;
}
