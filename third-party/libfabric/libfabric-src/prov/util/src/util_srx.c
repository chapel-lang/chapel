/*
 * Copyright (c) Intel Corporation, Inc.  All rights reserved.
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

#include <stdlib.h>

#include "ofi_enosys.h"
#include "ofi_iov.h"
#include "ofi_util.h"

static struct util_rx_entry *util_alloc_rx_entry(struct util_srx_ctx *srx)
{
	return (struct util_rx_entry *) ofi_buf_alloc(srx->rx_pool);
}

static inline struct iovec *util_srx_iov(struct util_rx_entry *rx_entry)
{
	return (struct iovec *) ((char *) rx_entry + sizeof(*rx_entry));
}

static inline void **util_srx_desc(struct util_srx_ctx *srx,
				   struct util_rx_entry *rx_entry)
{
	return (void **) ((char *) util_srx_iov(rx_entry) +
			(sizeof(struct iovec) * srx->iov_limit));
}

static void util_init_rx_entry(struct util_rx_entry *entry,
			       const struct iovec *iov, void **desc,
			       size_t count, fi_addr_t addr, void *context,
			       uint64_t tag, uint64_t flags)
{
	entry->status = RX_ENTRY_POSTED;
	memcpy(entry->peer_entry.iov, iov, sizeof(*iov) * count);
	if (desc)
		memcpy(entry->peer_entry.desc, desc, sizeof(*desc) * count);
	else
		memset(entry->peer_entry.desc, 0, sizeof(*desc) * count);

	entry->peer_entry.count = count;
	entry->peer_entry.addr = addr;
	entry->peer_entry.context = context;
	entry->peer_entry.tag = tag;
	entry->peer_entry.flags = flags;
	entry->peer_entry.msg_size = ofi_total_iov_len(iov, count);
}

static struct util_rx_entry *util_get_recv_entry(struct util_srx_ctx *srx,
		const struct iovec *iov, void **desc, size_t count,
		fi_addr_t addr, void *context, uint64_t tag, uint64_t ignore,
		uint64_t flags)
{
	struct util_rx_entry *entry;

	entry = util_alloc_rx_entry(srx);
	if (!entry)
		return NULL;

	util_init_rx_entry(entry, iov, desc, count, addr, context, tag, flags);

	entry->peer_entry.owner_context = NULL;

	entry->multi_recv_ref = 0;
	entry->ignore = ignore;
	entry->seq_no = srx->rx_seq_no++;

	return entry;
}

static struct util_rx_entry *util_init_unexp(struct util_srx_ctx *srx,
			struct fi_peer_match_attr *attr, uint64_t flags)
{
	struct util_rx_entry *util_entry;

	util_entry = util_alloc_rx_entry(srx);
	if (!util_entry)
		return NULL;

	util_entry->status = RX_ENTRY_UNEXP;
	util_entry->peer_entry.owner_context = NULL;
	util_entry->peer_entry.msg_size = attr->msg_size;
	util_entry->peer_entry.addr = attr->addr;
	util_entry->peer_entry.tag = attr->tag;
	util_entry->peer_entry.flags = flags;

	return util_entry;
}

static bool util_adjust_multi_recv(struct util_srx_ctx *srx,
		struct fi_peer_rx_entry *rx_entry, size_t len)
{
	size_t left;
	void *new_base;

	left = rx_entry->iov[0].iov_len - len;

	new_base = (void *) ((uintptr_t) rx_entry->iov[0].iov_base + len);
	rx_entry->iov[0].iov_len = left;
	rx_entry->iov[0].iov_base = new_base;
	rx_entry->msg_size = left;

	return left < srx->min_multi_recv_size;
}

static struct util_rx_entry *util_process_multi_recv(struct util_srx_ctx *srx,
		struct slist *queue, struct fi_peer_match_attr *attr,
		struct util_rx_entry *owner_entry)
{
	struct util_rx_entry *util_entry;

	util_entry = util_get_recv_entry(srx,
					 owner_entry->peer_entry.iov,
					 owner_entry->peer_entry.desc,
					 owner_entry->peer_entry.count,
					 attr->addr,
					 owner_entry->peer_entry.context,
					 owner_entry->peer_entry.tag,
					 owner_entry->ignore,
					 owner_entry->peer_entry.flags &
					 (~FI_MULTI_RECV));
	if (!util_entry)
		return NULL;

	if (util_adjust_multi_recv(srx, &owner_entry->peer_entry,
				   attr->msg_size)) {
		util_entry->status = RX_ENTRY_MATCHED;
		slist_remove_head(queue);
	}

	util_entry->peer_entry.owner_context = owner_entry;
	owner_entry->multi_recv_ref++;

	return util_entry;
}

static int util_match_msg(struct fid_peer_srx *srx,
			  struct fi_peer_match_attr *attr,
			  struct fi_peer_rx_entry **rx_entry)
{
	struct util_srx_ctx *srx_ctx;
	struct util_rx_entry *util_entry;
	struct slist_entry *head;
	int ret = FI_SUCCESS;

	srx_ctx = srx->ep_fid.fid.context;
	if (slist_empty(&srx_ctx->msg_queue)) {
		util_entry = util_init_unexp(srx_ctx, attr, FI_MSG | FI_RECV);
		if (!util_entry)
			return -FI_ENOMEM;
		util_entry->peer_entry.srx = srx;
		ret = -FI_ENOENT;
	} else {
		head = srx_ctx->msg_queue.head;
		util_entry = container_of(head, struct util_rx_entry, s_entry);
		assert(util_entry->status == RX_ENTRY_POSTED);
		if (util_entry->peer_entry.flags & FI_MULTI_RECV) {
			util_entry = util_process_multi_recv(srx_ctx,
				&srx_ctx->msg_queue, attr, util_entry);
			if (!util_entry) {
				FI_WARN(&core_prov, FI_LOG_EP_CTRL,
					"cannot allocate multi receive "
					"buffer\n");
				return -FI_ENOMEM;
			}
		} else {
			util_entry->status = RX_ENTRY_MATCHED;
			(void) slist_remove_head(&srx_ctx->msg_queue);
		}
		util_entry->peer_entry.srx = srx;
		srx_ctx->update_func(srx_ctx, util_entry);
	}
	util_entry->peer_entry.msg_size = MIN(util_entry->peer_entry.msg_size,
					      attr->msg_size);
	*rx_entry = &util_entry->peer_entry;
	return ret;
}

static int util_get_msg(struct fid_peer_srx *srx,
			struct fi_peer_match_attr *attr,
			struct fi_peer_rx_entry **rx_entry)
{
	struct util_srx_ctx *srx_ctx;
	struct util_rx_entry *util_entry, *any_entry;
	struct slist *queue;

	srx_ctx = srx->ep_fid.fid.context;
	assert(ofi_genlock_held(srx_ctx->lock));

	queue = attr->addr == FI_ADDR_UNSPEC ? NULL:
		ofi_array_at(&srx_ctx->src_recv_queues, attr->addr);

	if (!queue || slist_empty(queue))
		return util_match_msg(srx, attr, rx_entry);

	util_entry = container_of(queue->head, struct util_rx_entry, s_entry);
	if (!slist_empty(&srx_ctx->msg_queue)) {
		any_entry = container_of(&srx_ctx->msg_queue.head,
					 struct util_rx_entry, s_entry);
		if (any_entry->seq_no <= util_entry->seq_no) {
			queue = &srx_ctx->msg_queue;
			util_entry = any_entry;
		}
	}

	util_entry->status = RX_ENTRY_MATCHED;

	if (util_entry->peer_entry.flags & FI_MULTI_RECV) {
		util_entry = util_process_multi_recv(srx_ctx, queue, attr,
						     util_entry);
		if (!util_entry) {
			FI_WARN(&core_prov, FI_LOG_EP_CTRL,
				"cannot allocate multi receive buffer\n");
			return -FI_ENOMEM;
		}
	} else {
		(void) slist_remove_head(queue);
	}
	util_entry->peer_entry.srx = srx;
	srx_ctx->update_func(srx_ctx, util_entry);
	*rx_entry = &util_entry->peer_entry;
	return FI_SUCCESS;
}

static int util_match_tag(struct fid_peer_srx *srx,
			  struct fi_peer_match_attr *attr,
			  struct fi_peer_rx_entry **rx_entry)
{
	struct util_srx_ctx *srx_ctx;
	struct util_rx_entry *util_entry;
	struct slist_entry *item, *prev;
	int ret = FI_SUCCESS;

	srx_ctx = srx->ep_fid.fid.context;
	slist_foreach(&srx_ctx->tag_queue, item, prev) {
		util_entry = container_of(item, struct util_rx_entry, s_entry);
		assert(util_entry->status == RX_ENTRY_POSTED);
		if (ofi_match_tag(util_entry->peer_entry.tag,
				  util_entry->ignore, attr->tag)) {
			util_entry->status = RX_ENTRY_MATCHED;
			util_entry->peer_entry.srx = srx;
			srx_ctx->update_func(srx_ctx, util_entry);
			slist_remove(&srx_ctx->tag_queue, item, prev);
			goto out;
		}
	}

	util_entry = util_init_unexp(srx_ctx, attr, FI_TAGGED | FI_RECV);
	if (!util_entry)
		return -FI_ENOMEM;
	ret = -FI_ENOENT;
	util_entry->peer_entry.srx = srx;
out:
	util_entry->peer_entry.msg_size = MIN(util_entry->peer_entry.msg_size,
					      attr->msg_size);
	*rx_entry = &util_entry->peer_entry;
	return ret;
}

static int util_get_tag(struct fid_peer_srx *srx,
			struct fi_peer_match_attr *attr,
			struct fi_peer_rx_entry **rx_entry)
{
	struct util_srx_ctx *srx_ctx;
	struct slist *queue;
	struct slist_entry *any_item, *any_prev;
	struct slist_entry *item, *prev;
	struct util_rx_entry *util_entry, *any_entry;
	int ret = FI_SUCCESS;

	srx_ctx = srx->ep_fid.fid.context;
	assert(ofi_genlock_held(srx_ctx->lock));

	queue = attr->addr == FI_ADDR_UNSPEC ? NULL:
		ofi_array_at(&srx_ctx->src_trecv_queues, attr->addr);

	if (!queue || slist_empty(queue))
		return util_match_tag(srx, attr, rx_entry);

	slist_foreach(queue, item, prev) {
		util_entry = container_of(item, struct util_rx_entry, s_entry);
		if (ofi_match_tag(util_entry->peer_entry.tag,
				  util_entry->ignore, attr->tag))
			goto check_any;
	}
	return util_match_tag(srx, attr, rx_entry);

check_any:
	slist_foreach(&srx_ctx->tag_queue, any_item, any_prev) {
		any_entry = container_of(any_item, struct util_rx_entry,
					 peer_entry);
		if (any_entry->seq_no > util_entry->seq_no)
			break;

		if (ofi_match_tag(any_entry->peer_entry.tag, any_entry->ignore,
				  attr->tag)) {
			queue = &srx_ctx->tag_queue;
			util_entry = any_entry;
			item = any_item;
			prev = any_prev;
			break;
		}
	}
	util_entry->status = RX_ENTRY_MATCHED;
	util_entry->peer_entry.srx = srx;
	srx_ctx->update_func(srx_ctx, util_entry);
	*rx_entry = &util_entry->peer_entry;
	slist_remove(queue, item, prev);
	return ret;
}

static int util_queue_msg(struct fi_peer_rx_entry *rx_entry)
{
	struct util_srx_ctx *srx_ctx = rx_entry->srx->ep_fid.fid.context;
	struct util_rx_entry *util_entry;
	struct util_unexp_peer *unexp_peer;

	assert(ofi_genlock_held(srx_ctx->lock));

	util_entry = container_of(rx_entry, struct util_rx_entry, peer_entry);
	assert(util_entry->status == RX_ENTRY_UNEXP);
	if (rx_entry->addr == FI_ADDR_UNSPEC) {
		dlist_insert_tail(&util_entry->d_entry,
				  &srx_ctx->unspec_unexp_msg_queue);
	} else {
		unexp_peer = ofi_array_at(&srx_ctx->src_unexp_peers,
					  rx_entry->addr);
		assert(unexp_peer);
		slist_insert_tail(&util_entry->s_entry, &unexp_peer->msg_queue);
		if (!unexp_peer->cnt++)
			dlist_insert_tail(&unexp_peer->entry,
					  &srx_ctx->unexp_peers);
	}
	return FI_SUCCESS;
}

static int util_queue_tag(struct fi_peer_rx_entry *rx_entry)
{
	struct util_srx_ctx *srx_ctx = rx_entry->srx->ep_fid.fid.context;
	struct util_rx_entry *util_entry;
	struct util_unexp_peer *unexp_peer;

	assert(ofi_genlock_held(srx_ctx->lock));

	util_entry = container_of(rx_entry, struct util_rx_entry, peer_entry);
	assert(util_entry->status == RX_ENTRY_UNEXP);
	if (rx_entry->addr == FI_ADDR_UNSPEC) {
		dlist_insert_tail(&util_entry->d_entry,
				  &srx_ctx->unspec_unexp_tag_queue);
	} else {
		unexp_peer = ofi_array_at(&srx_ctx->src_unexp_peers,
					  rx_entry->addr);
		assert(unexp_peer);
		slist_insert_tail(&util_entry->s_entry, &unexp_peer->tag_queue);
		if (!unexp_peer->cnt++)
			dlist_insert_tail(&unexp_peer->entry,
					  &srx_ctx->unexp_peers);

	}
	return FI_SUCCESS;
}

static void util_free_entry(struct fi_peer_rx_entry *entry)
{
	struct util_srx_ctx *srx;
	struct util_unexp_peer *unexp_peer;
	struct slist *queue;
	struct slist_entry *item, *prev;
	struct util_rx_entry *util_entry, *owner_entry;

	srx = (struct util_srx_ctx *) entry->srx->ep_fid.fid.context;
	assert(ofi_genlock_held(srx->lock));

	util_entry = container_of(entry, struct util_rx_entry, peer_entry);
	if (entry->owner_context) {
		owner_entry = (struct util_rx_entry *) entry->owner_context;
		if (!--owner_entry->multi_recv_ref &&
		    owner_entry->peer_entry.msg_size < srx->min_multi_recv_size) {
			if (ofi_peer_cq_write(srx->cq,
					      owner_entry->peer_entry.context,
					      FI_MULTI_RECV, 0, NULL, 0, 0,
					      FI_ADDR_NOTAVAIL)) {
				FI_WARN(&core_prov, FI_LOG_EP_CTRL,
					"cannot write MULTI_RECV completion\n");
			}
			ofi_buf_free(owner_entry);
		}
	}

	if (util_entry->status == RX_ENTRY_UNEXP) {
		if (util_entry->peer_entry.addr == FI_ADDR_UNSPEC) {
			dlist_remove(&util_entry->d_entry);
		} else {
			unexp_peer = ofi_array_at(&srx->src_unexp_peers,
						  util_entry->peer_entry.addr);
			queue = util_entry->peer_entry.flags & FI_TAGGED ?
					&unexp_peer->tag_queue :
					&unexp_peer->msg_queue;

			slist_foreach(queue, item, prev) {
				if ((struct util_rx_entry *) item == util_entry)
					slist_remove(queue, item, prev);
			}

			if (!--unexp_peer->cnt) {
				assert(slist_empty(&unexp_peer->msg_queue) &&
				       slist_empty(&unexp_peer->tag_queue));
				dlist_remove(&unexp_peer->entry);
			}
		}
	}

	ofi_buf_free(util_entry);
}

static void util_foreach_unspec(struct fid_peer_srx *srx,
		fi_addr_t (*get_addr)(struct fi_peer_rx_entry *))
{
	struct util_srx_ctx *srx_ctx;
	struct util_rx_entry *rx_entry;
	struct util_unexp_peer *unexp_peer;
	struct dlist_entry *tmp;

	srx_ctx = srx->ep_fid.fid.context;

	assert(ofi_genlock_held(srx_ctx->lock));
	dlist_foreach_container_safe(&srx_ctx->unspec_unexp_msg_queue,
				     struct util_rx_entry, rx_entry, d_entry,
				     tmp) {
		rx_entry->peer_entry.addr = get_addr(&rx_entry->peer_entry);
		if (rx_entry->peer_entry.addr == FI_ADDR_UNSPEC)
			continue;

		dlist_remove(&rx_entry->d_entry);
		unexp_peer = ofi_array_at(&srx_ctx->src_unexp_peers,
					  rx_entry->peer_entry.addr);
		assert(unexp_peer);
		slist_insert_tail(&rx_entry->s_entry, &unexp_peer->msg_queue);
		if (!unexp_peer->cnt++)
			dlist_insert_tail(&unexp_peer->entry,
					  &srx_ctx->unexp_peers);
	}

	dlist_foreach_container_safe(&srx_ctx->unspec_unexp_tag_queue,
				     struct util_rx_entry, rx_entry, d_entry,
				     tmp) {
		rx_entry->peer_entry.addr = get_addr(&rx_entry->peer_entry);
		if (rx_entry->peer_entry.addr == FI_ADDR_UNSPEC)
			continue;

		dlist_remove(&rx_entry->d_entry);
		unexp_peer = ofi_array_at(&srx_ctx->src_unexp_peers,
					  rx_entry->peer_entry.addr);
		assert(unexp_peer);
		slist_insert_tail(&rx_entry->s_entry, &unexp_peer->tag_queue);
		if (!unexp_peer->cnt++)
			dlist_insert_tail(&unexp_peer->entry,
					  &srx_ctx->unexp_peers);
	}
}

static struct fi_ops_srx_owner util_srx_owner_ops = {
	.size = sizeof(struct fi_ops_srx_owner),
	.get_msg = util_get_msg,
	.get_tag = util_get_tag,
	.queue_msg = util_queue_msg,
	.queue_tag = util_queue_tag,
	.foreach_unspec_addr = util_foreach_unspec,
	.free_entry = util_free_entry,
};

static struct util_rx_entry *util_search_peer_msg(struct util_unexp_peer *peer)
{
	struct util_rx_entry *rx_entry;

	assert(peer);
	if (slist_empty(&peer->msg_queue))
		return NULL;

	rx_entry = (struct util_rx_entry *) slist_remove_head(&peer->msg_queue);
	if (!--peer->cnt) {
		assert(slist_empty(&peer->msg_queue));
		dlist_remove(&peer->entry);
	}
	return rx_entry;
}

static struct util_rx_entry *util_search_unexp_msg(struct util_srx_ctx *srx,
						   fi_addr_t addr)
{
	struct util_rx_entry *rx_entry;
	struct util_unexp_peer *unexp_peer;

	if (addr == FI_ADDR_UNSPEC) {
		if (!dlist_empty(&srx->unspec_unexp_msg_queue)) {
			dlist_pop_front(&srx->unspec_unexp_msg_queue,
					struct util_rx_entry, rx_entry,
					d_entry);
			return rx_entry;
		}

		dlist_foreach_container(&srx->unexp_peers,
					struct util_unexp_peer, unexp_peer,
					entry) {
			rx_entry = util_search_peer_msg(unexp_peer);
			if (rx_entry)
				return rx_entry;
		}
		return NULL;
	}

	return util_search_peer_msg(ofi_array_at(&srx->src_unexp_peers, addr));
}

static bool util_unexp_mrecv(struct util_srx_ctx *srx,
			     struct util_rx_entry *mrecv_entry,
			     struct util_rx_entry *rx_entry)
{
	mrecv_entry->multi_recv_ref++;
	rx_entry->peer_entry.owner_context = mrecv_entry;

	rx_entry->peer_entry.iov[0].iov_base =
			mrecv_entry->peer_entry.iov->iov_base;
	rx_entry->peer_entry.iov->iov_len =
			MIN(mrecv_entry->peer_entry.iov->iov_len,
			    rx_entry->peer_entry.msg_size);
	*rx_entry->peer_entry.desc = mrecv_entry->peer_entry.desc[0];

	rx_entry->peer_entry.count = 1;
	rx_entry->peer_entry.addr = mrecv_entry->peer_entry.addr;
	rx_entry->peer_entry.context = mrecv_entry->peer_entry.context;
	rx_entry->peer_entry.tag = mrecv_entry->peer_entry.tag;
	rx_entry->peer_entry.flags |= mrecv_entry->peer_entry.flags &
					~FI_MULTI_RECV;
	rx_entry->peer_entry.msg_size = rx_entry->peer_entry.iov->iov_len;

	return util_adjust_multi_recv(srx, &mrecv_entry->peer_entry,
				      rx_entry->peer_entry.msg_size);

}

static ssize_t util_generic_mrecv(struct util_srx_ctx *srx,
		const struct iovec *iov, void **desc, size_t iov_count,
		fi_addr_t addr, void *context, uint64_t flags)
{
	struct util_rx_entry *rx_entry, *mrecv_entry;
	struct slist *queue;
	bool buf_done = false;
	ssize_t ret = FI_SUCCESS;

	assert(flags & FI_MULTI_RECV && iov_count == 1);
	addr = srx->dir_recv ? addr : FI_ADDR_UNSPEC;

	ofi_genlock_lock(srx->lock);
	mrecv_entry = util_get_recv_entry(srx, iov, desc, iov_count, addr,
					  context, 0, 0,
					  flags | FI_MSG | FI_RECV);
	if (!mrecv_entry) {
		ret = -FI_ENOMEM;
		goto out;
	}

	mrecv_entry->peer_entry.msg_size = ofi_total_iov_len(iov, iov_count);

	rx_entry = util_search_unexp_msg(srx, addr);
	while (rx_entry) {
		rx_entry->status = RX_ENTRY_MATCHED;
		buf_done = util_unexp_mrecv(srx, mrecv_entry, rx_entry);

		srx->update_func(srx, rx_entry);
		ret = rx_entry->peer_entry.srx->peer_ops->start_msg(
							&rx_entry->peer_entry);
		if (ret || buf_done)
			goto out;

		rx_entry = util_search_unexp_msg(srx, addr);
	}

	queue = addr == FI_ADDR_UNSPEC ? &srx->msg_queue:
		ofi_array_at(&srx->src_recv_queues, addr);
	assert(queue);
	slist_insert_tail(&mrecv_entry->s_entry, queue);
out:
	ofi_genlock_unlock(srx->lock);
	return ret;
}

static struct util_rx_entry *util_search_peer_tag(struct util_unexp_peer *peer,
				uint64_t tag, uint64_t ignore, bool remove)
{
	struct util_rx_entry *rx_entry;
	struct slist_entry *item, *prev;

	assert(peer);
	if (slist_empty(&peer->tag_queue))
		return NULL;

	slist_foreach(&peer->tag_queue, item, prev) {
		rx_entry = (struct util_rx_entry *) item;
		if (!ofi_match_tag(tag, ignore, rx_entry->peer_entry.tag))
			continue;

		if (remove) {
			slist_remove(&peer->tag_queue, item, prev);
			if (!--peer->cnt) {
				assert(slist_empty(&peer->tag_queue));
				dlist_remove(&peer->entry);
			}
		}
		return rx_entry;
	}
	return NULL;
}

static struct util_rx_entry *util_search_unexp_tag(struct util_srx_ctx *srx,
		fi_addr_t addr, uint64_t tag, uint64_t ignore, bool remove)
{
	struct util_rx_entry *rx_entry;
	struct util_unexp_peer *unexp_peer;

	if (addr == FI_ADDR_UNSPEC) {
		dlist_foreach_container(&srx->unspec_unexp_tag_queue,
					struct util_rx_entry, rx_entry,
					d_entry) {
			if (!ofi_match_tag(tag, ignore,
			    rx_entry->peer_entry.tag))
				continue;

			if (remove)
				dlist_remove(&rx_entry->d_entry);

			return rx_entry;
		}

		dlist_foreach_container(&srx->unexp_peers,
				struct util_unexp_peer, unexp_peer, entry) {
			rx_entry = util_search_peer_tag(unexp_peer, tag,
							ignore, remove);
			if (rx_entry)
				return rx_entry;
		}
		return NULL;
	}

	return util_search_peer_tag(ofi_array_at(&srx->src_unexp_peers, addr),
				    tag, ignore, remove);
}

static ssize_t util_srx_peek(struct util_srx_ctx *srx, const struct iovec *iov,
			     void **desc, size_t iov_count, fi_addr_t addr,
			     void *context, uint64_t tag, uint64_t ignore,
			     uint64_t flags)
{
	struct util_rx_entry *rx_entry;
	int ret = FI_SUCCESS;

	rx_entry = util_search_unexp_tag(srx, addr, tag, ignore,
					 flags & (FI_CLAIM | FI_DISCARD));
	if (!rx_entry) {
		FI_DBG(&core_prov, FI_LOG_EP_CTRL, "Message not found\n");
		return ofi_cq_write_error_peek(srx->cq, tag, context);
	}
	FI_DBG(&core_prov, FI_LOG_EP_CTRL, "Message found\n");

	if (flags & (FI_CLAIM | FI_DISCARD)) {
		if (flags & FI_DISCARD) {
			ret = rx_entry->peer_entry.srx->peer_ops->discard_tag(
							&rx_entry->peer_entry);
			if (ret) {
				FI_WARN(&core_prov, FI_LOG_EP_CTRL,
					"Error discarding message with peer\n");
			}
			ofi_buf_free(rx_entry);
		}
		((struct fi_context *)context)->internal[0] = rx_entry;
	}

	return ofi_cq_write(srx->cq, context, rx_entry->peer_entry.flags,
			    rx_entry->peer_entry.msg_size, NULL,
			    rx_entry->peer_entry.cq_data,
			    rx_entry->peer_entry.tag);
}

ssize_t util_srx_generic_trecv(struct fid_ep *ep_fid, const struct iovec *iov,
			       void **desc, size_t iov_count, fi_addr_t addr,
			       void *context, uint64_t tag, uint64_t ignore,
			       uint64_t flags)
{
	struct util_srx_ctx *srx;
	struct util_rx_entry *rx_entry;
	struct slist *queue;
	ssize_t ret = FI_SUCCESS;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);
	assert(iov_count <= srx->iov_limit);
	addr = srx->dir_recv ? addr : FI_ADDR_UNSPEC;

	ofi_genlock_lock(srx->lock);
	if (flags & FI_PEEK) {
		ret = util_srx_peek(srx, iov, desc, iov_count, addr,
				    context, tag, ignore, flags);
		goto out;
	}

	if (flags & FI_DISCARD) {
		assert(flags & FI_CLAIM);
		rx_entry = (struct util_rx_entry *)
				(((struct fi_context *) context)->internal[0]);
		ret = rx_entry->peer_entry.srx->peer_ops->discard_tag(
							&rx_entry->peer_entry);
		if (ret) {
			FI_WARN(&core_prov, FI_LOG_EP_CTRL,
				"Error discarding message with peer\n");
		}
		ret = ofi_cq_write(srx->cq, context, FI_TAGGED | FI_RECV,
				    rx_entry->peer_entry.msg_size, NULL, 0,
				    rx_entry->peer_entry.tag);
		ofi_buf_free(rx_entry);
		goto out;
	}

	if (flags & FI_CLAIM) {
		rx_entry = (struct util_rx_entry *)
				(((struct fi_context *) context)->internal[0]);
	} else {
		rx_entry = util_search_unexp_tag(srx, addr, tag, ignore, true);
		if (!rx_entry) {
			queue = addr == FI_ADDR_UNSPEC ? &srx->tag_queue:
				ofi_array_at(&srx->src_trecv_queues, addr);
			assert(queue);
			rx_entry = util_get_recv_entry(srx, iov, desc,
						iov_count, addr, context, tag,
						ignore,
						flags | FI_TAGGED | FI_RECV);
			if (!rx_entry)
				ret = -FI_ENOMEM;
			else
				slist_insert_tail(&rx_entry->s_entry, queue);
			goto out;
		}
	}
	util_init_rx_entry(rx_entry, iov, desc, iov_count, addr, context, tag,
			   flags | FI_TAGGED | FI_RECV);

	srx->update_func(srx, rx_entry);
	ret = rx_entry->peer_entry.srx->peer_ops->start_tag(
					&rx_entry->peer_entry);
out:
	ofi_genlock_unlock(srx->lock);
	return ret;
}

ssize_t util_srx_generic_recv(struct fid_ep *ep_fid, const struct iovec *iov,
			      void **desc, size_t iov_count, fi_addr_t addr,
			      void *context, uint64_t flags)
{
	struct util_srx_ctx *srx;
	struct util_rx_entry *rx_entry;
	struct slist *queue;
	ssize_t ret = FI_SUCCESS;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	if (flags & FI_MULTI_RECV)
		return util_generic_mrecv(srx, iov, desc, iov_count, addr,
					  context, flags);

	assert(iov_count <= srx->iov_limit);
	addr = srx->dir_recv ? addr : FI_ADDR_UNSPEC;

	ofi_genlock_lock(srx->lock);
	rx_entry = util_search_unexp_msg(srx, addr);
	if (!rx_entry) {
		queue = addr == FI_ADDR_UNSPEC ? &srx->msg_queue :
			ofi_array_at(&srx->src_recv_queues, addr);
		assert(queue);
		rx_entry = util_get_recv_entry(srx, iov, desc, iov_count, addr,
					       context, 0, 0,
					       flags | FI_MSG | FI_RECV);
		if (!rx_entry)
			ret = -FI_ENOMEM;
		else
			slist_insert_tail(&rx_entry->s_entry, queue);
		goto out;
	}

	util_init_rx_entry(rx_entry, iov, desc, iov_count, addr, context, 0,
			   flags | FI_MSG | FI_RECV);

	srx->update_func(srx, rx_entry);
	ret = rx_entry->peer_entry.srx->peer_ops->start_msg(
						&rx_entry->peer_entry);
out:
	ofi_genlock_unlock(srx->lock);
	return ret;
}

static ssize_t util_srx_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
				uint64_t flags)
{
	struct util_srx_ctx *srx;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	return util_srx_generic_recv(ep_fid, msg->msg_iov, msg->desc,
				     msg->iov_count, msg->addr, msg->context,
				     flags | srx->rx_msg_flags);
}

static ssize_t util_srx_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
			      void **desc, size_t count, fi_addr_t src_addr,
			      void *context)
{
	struct util_srx_ctx *srx;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	return util_srx_generic_recv(ep_fid, iov, desc, count, src_addr,
				    context, srx->rx_op_flags);
}

static ssize_t util_srx_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			     void *desc, fi_addr_t src_addr, void *context)
{
	struct iovec iov;
	struct util_srx_ctx *srx;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	iov.iov_base = buf;
	iov.iov_len = len;

	return util_srx_generic_recv(ep_fid, &iov, &desc, 1, src_addr, context,
				     srx->rx_op_flags);
}

struct fi_ops_msg util_srx_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = util_srx_recv,
	.recvv = util_srx_recvv,
	.recvmsg = util_srx_recvmsg,
	.send = fi_no_msg_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = fi_no_msg_sendmsg,
	.inject = fi_no_msg_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

static ssize_t util_srx_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, uint64_t tag,
			      uint64_t ignore, void *context)
{
	struct iovec iov;
	struct util_srx_ctx *srx;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	iov.iov_base = buf;
	iov.iov_len = len;

	return util_srx_generic_trecv(ep_fid, &iov, &desc, 1, src_addr, context,
				      tag, ignore, srx->rx_op_flags);
}

static ssize_t util_srx_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
			       void **desc, size_t count, fi_addr_t src_addr,
			       uint64_t tag, uint64_t ignore, void *context)
{
	struct util_srx_ctx *srx;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	return util_srx_generic_trecv(ep_fid, iov, desc, count, src_addr,
				      context, tag, ignore, srx->rx_op_flags);
}

static ssize_t util_srx_trecvmsg(struct fid_ep *ep_fid,
			const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct util_srx_ctx *srx;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	return util_srx_generic_trecv(ep_fid, msg->msg_iov, msg->desc,
				      msg->iov_count, msg->addr, msg->context,
				      msg->tag, msg->ignore,
				      flags | srx->rx_msg_flags);
}

struct fi_ops_tagged util_srx_tag_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = util_srx_trecv,
	.recvv = util_srx_trecvv,
	.recvmsg = util_srx_trecvmsg,
	.send = fi_no_tagged_send,
	.sendv = fi_no_tagged_sendv,
	.sendmsg = fi_no_tagged_sendmsg,
	.inject = fi_no_tagged_inject,
	.senddata = fi_no_tagged_senddata,
	.injectdata = fi_no_tagged_injectdata,
};

int util_srx_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct util_srx_ctx *srx;

	if (flags != FI_RECV || bfid->fclass != FI_CLASS_CQ)
		return -FI_EINVAL;

	srx = container_of(fid, struct util_srx_ctx, peer_srx.ep_fid.fid);
	srx->cq = container_of(bfid, struct util_cq, cq_fid.fid);
	ofi_atomic_inc32(&srx->cq->ref);
	return FI_SUCCESS;
}

static int util_cancel_entry(struct util_srx_ctx *srx, uint64_t flags,
			     struct util_rx_entry *rx_entry)
{
	struct fi_cq_err_entry err_entry = {0};
	int ret;

	err_entry.op_context = rx_entry->peer_entry.context;
	err_entry.flags = flags;
	if (rx_entry->peer_entry.flags & FI_MULTI_RECV) {
		assert(rx_entry->multi_recv_ref == 0);
		err_entry.flags |= FI_MULTI_RECV;
	}
	err_entry.tag = rx_entry->peer_entry.tag;
	err_entry.err = FI_ECANCELED;
	err_entry.prov_errno = -FI_ECANCELED;

	ret = ofi_peer_cq_write_error(srx->cq, &err_entry);
	ofi_buf_free(rx_entry);
	return ret ? ret : 1;
}

static int util_cleanup_queues(struct ofi_dyn_arr *arr, void *list,
			       void *context)
{
	struct slist *queue = list;
	struct slist_entry *item;
	struct util_rx_entry *rx_entry;

	while (!slist_empty(queue)) {
		item = slist_remove_head(queue);
		rx_entry = container_of(item, struct util_rx_entry, s_entry);
		ofi_buf_free(rx_entry);
	}
	return FI_SUCCESS;
}

int util_srx_close(struct fid *fid)
{
	struct util_srx_ctx *srx;
	struct util_unexp_peer *unexp_peer;
	struct util_rx_entry *rx_entry;
	struct slist_entry *entry;

	srx = container_of(fid, struct util_srx_ctx, peer_srx.ep_fid.fid);
	if (!srx)
		return -FI_EINVAL;

	ofi_genlock_lock(srx->lock);
	(void)ofi_array_iter(&srx->src_recv_queues, srx, util_cleanup_queues);
	(void)ofi_array_iter(&srx->src_trecv_queues, srx, util_cleanup_queues);
	ofi_array_destroy(&srx->src_recv_queues);
	ofi_array_destroy(&srx->src_trecv_queues);

	while (!slist_empty(&srx->msg_queue)) {
		entry = slist_remove_head(&srx->msg_queue);
		ofi_buf_free(container_of(entry, struct util_rx_entry,
					  s_entry));
	}

	while (!slist_empty(&srx->tag_queue)) {
		entry = slist_remove_head(&srx->tag_queue);
		ofi_buf_free(container_of(entry, struct util_rx_entry,
					  s_entry));
	}

	while (!dlist_empty(&srx->unspec_unexp_msg_queue)) {
		dlist_pop_front(&srx->unspec_unexp_msg_queue,
				struct util_rx_entry, rx_entry, d_entry);
		rx_entry->peer_entry.srx->peer_ops->discard_msg(
							&rx_entry->peer_entry);
		ofi_buf_free(rx_entry);
	}

	while (!dlist_empty(&srx->unspec_unexp_tag_queue)) {
		dlist_pop_front(&srx->unspec_unexp_tag_queue,
				struct util_rx_entry, rx_entry, d_entry);
		rx_entry->peer_entry.srx->peer_ops->discard_tag(
							&rx_entry->peer_entry);
		ofi_buf_free(rx_entry);
	}

	while (!dlist_empty(&srx->unexp_peers)) {
		dlist_pop_front(&srx->unexp_peers, struct util_unexp_peer,
				unexp_peer, entry);
		while (!slist_empty(&unexp_peer->msg_queue)) {
			rx_entry = container_of(slist_remove_head(
				&unexp_peer->msg_queue), struct util_rx_entry,
				s_entry);
			rx_entry->peer_entry.srx->peer_ops->discard_msg(
							&rx_entry->peer_entry);
			ofi_buf_free(rx_entry);
			unexp_peer->cnt--;
		}
		while (!slist_empty(&unexp_peer->tag_queue)) {
			rx_entry = container_of(slist_remove_head(
				&unexp_peer->tag_queue), struct util_rx_entry,
				s_entry);
			rx_entry->peer_entry.srx->peer_ops->discard_tag(
							&rx_entry->peer_entry);
			ofi_buf_free(rx_entry);
			unexp_peer->cnt--;
		}
		assert(!unexp_peer->cnt);
	}

	ofi_array_destroy(&srx->src_unexp_peers);

	ofi_atomic_dec32(&srx->cq->ref);
	ofi_bufpool_destroy(srx->rx_pool);

	ofi_genlock_unlock(srx->lock);
	free(srx);

	return FI_SUCCESS;
}

static struct fi_ops util_srx_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = util_srx_close,
	.bind = util_srx_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int util_cancel_recv(struct util_srx_ctx *srx, struct slist *queue,
			    uint64_t flags, void *context)
{
	struct slist_entry *item, *prev;
	struct util_rx_entry *rx_entry;

	assert(ofi_genlock_held(srx->lock));
	slist_foreach(queue, item, prev) {
		rx_entry = container_of(item, struct util_rx_entry, s_entry);
		if (rx_entry->peer_entry.context == context) {
			/* With multi-recv, cancellation can only be processed
			 * if the multi-recv buffer is no longer in use. If the
			 * buffer is still in use, return EAGAIN back for user
			 * to retry cancel request. */
			if (rx_entry->peer_entry.flags & FI_MULTI_RECV &&
			    rx_entry->multi_recv_ref)
				return -FI_EAGAIN;
			slist_remove(queue, item, prev);
			util_cancel_entry(srx, flags, rx_entry);
			return FI_SUCCESS;
		}
	}
	return -FI_ENOENT;
}

static int util_cancel_src(struct ofi_dyn_arr *arr, void *list, void *context)
{
	struct util_srx_ctx *srx;
	struct slist *queue = list;
	uint64_t flags;

	srx = container_of(arr, struct util_srx_ctx, src_trecv_queues);

	flags = arr == &srx->src_trecv_queues ?
		FI_TAGGED | FI_RECV : FI_MSG | FI_RECV;

	return (int) (util_cancel_recv(srx, queue, flags, context) == FI_SUCCESS);
}

static ssize_t util_srx_cancel(fid_t ep_fid, void *context)
{
	struct util_srx_ctx *srx;
	ssize_t ret;

	srx = container_of(ep_fid, struct util_srx_ctx, peer_srx.ep_fid);

	ofi_genlock_lock(srx->lock);
	ret = util_cancel_recv(srx, &srx->tag_queue, FI_TAGGED | FI_RECV,
			       context);
	if (ret != -FI_ENOENT)
		goto out;

	ret = util_cancel_recv(srx, &srx->msg_queue, FI_MSG | FI_RECV, context);
	if (ret != -FI_ENOENT)
		goto out;

	if (ofi_array_iter(&srx->src_trecv_queues, context, util_cancel_src) ||
	    ofi_array_iter(&srx->src_recv_queues, context, util_cancel_src)) {
		/* nothing to do, always return success */
	}

	ret = FI_SUCCESS;

out:
	ofi_genlock_unlock(srx->lock);
	return ret;
}

static int util_srx_getopt(fid_t fid, int level, int optname,
		           void *optval, size_t *optlen)
{
	struct util_srx_ctx *srx =
		container_of(fid, struct util_srx_ctx, peer_srx.ep_fid.fid);

	if ((level != FI_OPT_ENDPOINT) || (optname != FI_OPT_MIN_MULTI_RECV))
		return -FI_ENOPROTOOPT;

	*(size_t *)optval = srx->min_multi_recv_size;
	*optlen = sizeof(size_t);

	return FI_SUCCESS;
}

static int util_srx_setopt(fid_t fid, int level, int optname,
		           const void *optval, size_t optlen)
{
	struct util_srx_ctx *srx =
		container_of(fid, struct util_srx_ctx, peer_srx.ep_fid.fid);

	if ((level != FI_OPT_ENDPOINT) || (optname != FI_OPT_MIN_MULTI_RECV))
		return -FI_ENOPROTOOPT;

	srx->min_multi_recv_size = *(size_t *)optval;

	return FI_SUCCESS;
}

static struct fi_ops_ep util_srx_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = util_srx_cancel,
	.getopt = util_srx_getopt,
	.setopt = util_srx_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static void util_rx_entry_init(struct ofi_bufpool_region *region, void *buf)
{
	struct util_rx_entry *rx_entry = (struct util_rx_entry *) buf;
	struct util_srx_ctx *srx = (struct util_srx_ctx *)
					region->pool->attr.context;

	rx_entry->peer_entry.iov = util_srx_iov(rx_entry);
	rx_entry->peer_entry.desc = util_srx_desc(srx, rx_entry);
}

static void util_srx_init_slist(struct ofi_dyn_arr *arr, void *item)
{
	slist_init((struct slist *) item);
}

static void util_srx_init_unexp_peer(struct ofi_dyn_arr *arr, void *item)
{
	struct util_unexp_peer *unexp_peer = item;

	slist_init(&unexp_peer->msg_queue);
	slist_init(&unexp_peer->tag_queue);
	unexp_peer->cnt = 0;
}

int util_ep_srx_context(struct util_domain *domain, size_t rx_size,
			size_t iov_limit, size_t default_min_multi_recv,
			ofi_update_func_t update_func,
			struct ofi_genlock *lock, struct fid_ep **rx_ep)
{
	struct util_srx_ctx *srx;
	struct ofi_bufpool_attr pool_attr = {0};
	int ret = FI_SUCCESS;

	srx = calloc(1, sizeof(*srx));
	if (!srx)
		return -FI_ENOMEM;

	ofi_array_init(&srx->src_unexp_peers, sizeof(struct util_unexp_peer),
		       util_srx_init_unexp_peer);
	dlist_init(&srx->unspec_unexp_msg_queue);
	dlist_init(&srx->unspec_unexp_tag_queue);
	dlist_init(&srx->unexp_peers);

	ofi_array_init(&srx->src_recv_queues, sizeof(struct slist),
		       util_srx_init_slist);
	ofi_array_init(&srx->src_trecv_queues, sizeof(struct slist),
		       util_srx_init_slist);

	slist_init(&srx->msg_queue);
	slist_init(&srx->tag_queue);

	//each entry has the iovs and descriptors stored at the end of the entry
	//calculate how much space each entry needs based on provider iov limits
	pool_attr.size = sizeof(struct util_rx_entry) +
		(sizeof(struct iovec) + sizeof(void *)) * iov_limit;
	pool_attr.alignment = 16;
	pool_attr.chunk_cnt = rx_size,
	pool_attr.alloc_fn = NULL;
	pool_attr.free_fn = NULL;
	pool_attr.init_fn = util_rx_entry_init;
	pool_attr.context = srx;
	ret = ofi_bufpool_create_attr(&pool_attr, &srx->rx_pool);
	if (ret) {
		free(srx);
		return ret;
	}

	srx->min_multi_recv_size = default_min_multi_recv;
	srx->iov_limit = iov_limit;
	srx->dir_recv = domain->info_domain_caps & FI_DIRECTED_RECV;
	srx->update_func = update_func;
	srx->lock = lock;

	srx->peer_srx.owner_ops = &util_srx_owner_ops;
	srx->peer_srx.peer_ops = NULL;

	srx->peer_srx.ep_fid.fid.fclass = FI_CLASS_SRX_CTX;
	srx->peer_srx.ep_fid.fid.context = srx;
	srx->peer_srx.ep_fid.fid.ops = &util_srx_fid_ops;
	srx->peer_srx.ep_fid.ops = &util_srx_ops;

	srx->peer_srx.ep_fid.msg = &util_srx_msg_ops;
	srx->peer_srx.ep_fid.tagged = &util_srx_tag_ops;
	*rx_ep = &srx->peer_srx.ep_fid;

	domain->srx = &srx->peer_srx;
	return FI_SUCCESS;
}
