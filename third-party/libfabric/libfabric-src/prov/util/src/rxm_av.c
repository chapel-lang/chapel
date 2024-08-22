/*
 * Copyright (c) 2018-2022 Intel Corporation. All rights reserved.
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

#include "ofi_util.h"
#include "uthash.h"


size_t rxm_av_max_peers(struct rxm_av *av)
{
	size_t cnt;

	ofi_mutex_lock(&av->util_av.lock);
	cnt = av->peer_pool->entry_cnt;
	ofi_mutex_unlock(&av->util_av.lock);
	return cnt;
}

void *rxm_av_alloc_conn(struct rxm_av *av)
{
	void *conn_ctx;
	ofi_mutex_lock(&av->util_av.lock);
	conn_ctx = ofi_buf_alloc(av->conn_pool);
	ofi_mutex_unlock(&av->util_av.lock);
	return conn_ctx;
}

void rxm_av_free_conn(struct rxm_av *av, void *conn_ctx)
{
	ofi_mutex_lock(&av->util_av.lock);
	ofi_buf_free(conn_ctx);
	ofi_mutex_unlock(&av->util_av.lock);
}

static int rxm_addr_compare(struct ofi_rbmap *map, void *key, void *data)
{
	return memcmp(&((struct util_peer_addr *) data)->addr, key,
		container_of(map, struct rxm_av, addr_map)->util_av.addrlen);
}

static struct util_peer_addr *
rxm_alloc_peer(struct rxm_av *av, const void *addr)
{
	struct util_peer_addr *peer;

	assert(ofi_mutex_held(&av->util_av.lock));
	peer = ofi_ibuf_alloc(av->peer_pool);
	if (!peer)
		return NULL;

	peer->av = av;
	peer->index = (int) ofi_buf_index(peer);
	peer->fi_addr = FI_ADDR_NOTAVAIL;
	peer->refcnt = 1;
	memcpy(&peer->addr, addr, av->util_av.addrlen);

	if (ofi_rbmap_insert(&av->addr_map, &peer->addr, peer, &peer->node)) {
		ofi_ibuf_free(peer);
		peer = NULL;
	}

	return peer;
}

static void rxm_free_peer(struct util_peer_addr *peer)
{
	assert(ofi_mutex_held(&peer->av->util_av.lock));
	assert(!peer->refcnt);
	ofi_rbmap_delete(&peer->av->addr_map, peer->node);
	ofi_ibuf_free(peer);
}

struct util_peer_addr *
util_get_peer(struct rxm_av *av, const void *addr)
{
	struct util_peer_addr *peer;
	struct ofi_rbnode *node;

	ofi_mutex_lock(&av->util_av.lock);
	node = ofi_rbmap_find(&av->addr_map, (void *) addr);
	if (node) {
		peer = node->data;
		peer->refcnt++;
	} else {
		peer = rxm_alloc_peer(av, addr);
	}

	ofi_mutex_unlock(&av->util_av.lock);
	return peer;
}

static void util_deref_peer(struct util_peer_addr *peer)
{
	assert(ofi_mutex_held(&peer->av->util_av.lock));
	if (--peer->refcnt == 0)
		rxm_free_peer(peer);
}

void util_put_peer(struct util_peer_addr *peer)
{
	struct rxm_av *av;

	av = peer->av;
	ofi_mutex_lock(&av->util_av.lock);
	util_deref_peer(peer);
	ofi_mutex_unlock(&av->util_av.lock);
}

void rxm_ref_peer(struct util_peer_addr *peer)
{
	ofi_mutex_lock(&peer->av->util_av.lock);
	peer->refcnt++;
	ofi_mutex_unlock(&peer->av->util_av.lock);
}

static void
rxm_set_av_context(struct rxm_av *av, fi_addr_t fi_addr,
		   struct util_peer_addr *peer)
{
	struct util_peer_addr **peer_ctx;

	peer_ctx = ofi_av_addr_context(&av->util_av, fi_addr);
	*peer_ctx = peer;
}

static void
rxm_put_peer_addr(struct rxm_av *av, fi_addr_t fi_addr)
{
	struct util_peer_addr **peer;

	peer = ofi_av_addr_context(&av->util_av, fi_addr);
	if (--(*peer)->refcnt == 0)
		rxm_free_peer(*peer);

	rxm_set_av_context(av, fi_addr, NULL);
}

static int
rxm_av_add_peers(struct rxm_av *av, const void *addr, size_t count,
		 fi_addr_t *fi_addr)
{
	struct util_peer_addr *peer;
	const void *cur_addr;
	fi_addr_t cur_fi_addr;
	size_t i;

	for (i = 0; i < count; i++) {
		cur_addr = ((char *) addr + i * av->util_av.addrlen);
		peer = util_get_peer(av, cur_addr);
		if (!peer)
			goto err;

		peer->fi_addr = fi_addr ? fi_addr[i] :
				ofi_av_lookup_fi_addr(&av->util_av, cur_addr);

		/* lookup can fail if prior AV insertion failed */
		if (peer->fi_addr != FI_ADDR_NOTAVAIL)
			rxm_set_av_context(av, peer->fi_addr, peer);
	}
	return 0;

err:
	while (i--) {
		if (fi_addr) {
			cur_fi_addr = fi_addr[i];
		} else {
			cur_addr = ((char *) addr + i * av->util_av.addrlen);
			cur_fi_addr = ofi_av_lookup_fi_addr(&av->util_av,
							    cur_addr);
		}
		if (cur_fi_addr != FI_ADDR_NOTAVAIL) {
			ofi_mutex_lock(&av->util_av.lock);
			rxm_put_peer_addr(av, cur_fi_addr);
			ofi_mutex_unlock(&av->util_av.lock);
		}
	}
	return -FI_ENOMEM;
}

static int rxm_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	struct util_av_entry *av_entry;
	struct util_peer_addr **peer;
	struct util_ep *util_ep;
	struct dlist_entry *item;
	struct rxm_av *av;
	ssize_t i;

	if (flags)
		return -FI_EINVAL;

	av = container_of(av_fid, struct rxm_av, util_av.av_fid);

	/*
	 * It's more efficient to remove addresses from high to low index.
	 * We assume that addresses are removed in the same order that they were
	 * added -- i.e. fi_addr passed in here was also passed into insert.
	 * Thus, we walk through the array backwards.
	 */
	ofi_mutex_lock(&av->util_av.lock);
	for (i = count - 1; i >= 0; i--) {
		FI_INFO(av->util_av.prov, FI_LOG_AV,
			"fi_addr %" PRIu64 "\n", fi_addr[i]);
		av_entry = ofi_bufpool_get_ibuf(av->util_av.av_entry_pool,
						fi_addr[i]);
		if (!av_entry)
			continue;

		if (av->util_av.remove_handler) {
			/* The remove_handler may call back into the AV to
			 * remove the provider's reference on the peer address.
			 * We need to drop the lock on the AV in case the
			 * handler tries to acquire it, plus to avoid nesting
			 * the ep_list_lock under the AV lock.  Increment
			 * the reference count on the peer, so that it's still
			 * valid to pass into the handler and isn't freed by
			 * another thread after we drop the AV lock.
			*/
			peer = ofi_av_addr_context(&av->util_av, fi_addr[i]);
			(*peer)->refcnt++;
			ofi_mutex_unlock(&av->util_av.lock);

			ofi_genlock_lock(&av->util_av.ep_list_lock);
			dlist_foreach(&av->util_av.ep_list, item) {
				util_ep = container_of(item, struct util_ep,
						       av_entry);
				av->util_av.remove_handler(util_ep, *peer);
			}
			ofi_genlock_unlock(&av->util_av.ep_list_lock);

			ofi_mutex_lock(&av->util_av.lock);
			util_deref_peer(*peer);
		}

		if (!ofi_atomic_dec32(&av_entry->use_cnt)) {
			rxm_put_peer_addr(av, fi_addr[i]);
			HASH_DELETE(hh, av->util_av.hash, av_entry);
			ofi_ibuf_free(av_entry);
		}
	}
	ofi_mutex_unlock(&av->util_av.lock);

	return 0;
}

static int rxm_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			 fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct rxm_av *av;
	int ret;

	av = container_of(av_fid, struct rxm_av, util_av.av_fid.fid);
	ret = ofi_ip_av_insert(av_fid, addr, count, fi_addr, flags, context);
	if (ret < 0)
		return ret;

	if (!av->util_av.eq)
		count = ret;

	ret = rxm_av_add_peers(av, addr, count, fi_addr);
	if (ret) {
		/* If insert was async, ofi_ip_av_insert() will have written
		 * an event to the EQ with the number of insertions.  For
		 * correctness we need to delay writing the event to the EQ
		 * until all processing has completed.  This should be done
		 * when separating the rxm av from the util av.  For now,
		 * assume synchronous operation (most common case) and fail
		 * the insert.  This could leave a bogus entry on the EQ.
		 * But the app should detect that insert failed and is likely
		 * to abort.
		 */
		rxm_av_remove(av_fid, fi_addr, count, flags);
		return ret;
	}

	return av->util_av.eq ? 0 : (int) count;
}

static int rxm_av_insertsym(struct fid_av *av_fid, const char *node,
			    size_t nodecnt, const char *service, size_t svccnt,
			    fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct rxm_av *av;
	void *addr;
	size_t addrlen, count;
	int ret;

	av = container_of(av_fid, struct rxm_av, util_av.av_fid.fid);
	ret = ofi_verify_av_insert(&av->util_av, flags, context);
	if (ret)
		return ret;

	ret = ofi_ip_av_sym_getaddr(&av->util_av, node, nodecnt, service,
				    svccnt, &addr, &addrlen);
	if (ret <= 0)
		return ret;

	count = ret;
	ret = ofi_ip_av_insertv(&av->util_av, addr, addrlen, count, fi_addr, flags,
				context);
	if (ret > 0 && ret < count)
		count = ret;

	ret = rxm_av_add_peers(av, addr, count, fi_addr);
	if (ret) {
		/* See comment in rxm_av_insert. */
		rxm_av_remove(av_fid, fi_addr, count, flags);
		return ret;
	}

	free(addr);
	return av->util_av.eq ? 0 : (int) count;
}

int rxm_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		     fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return rxm_av_insertsym(av, node, 1, service, 1, fi_addr, flags, context);
}

static const char *
rxm_av_straddr(struct fid_av *av_fid, const void *addr, char *buf, size_t *len)
{
	return ofi_ip_av_straddr(av_fid, addr, buf, len);
}

int rxm_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
		  void *addr, size_t *addrlen)
{
	return ofi_ip_av_lookup(av_fid, fi_addr, addr, addrlen);
}

int rxm_av_set(struct fid_av *av_fid, struct fi_av_set_attr *attr,
               struct fid_av_set **av_set_fid, void *context)
{
	struct rxm_av *rxm_av;

	rxm_av = container_of(av_fid, struct rxm_av, util_av.av_fid);
	if (!rxm_av->util_coll_av)
		return -FI_ENOSYS;

	return fi_av_set(rxm_av->util_coll_av, attr, av_set_fid, context);
}

static int rxm_av_close(struct fid *av_fid)
{
	struct rxm_av *av;
	int ret;

	av = container_of(av_fid, struct rxm_av, util_av.av_fid.fid);
	if (av->util_coll_av)
		fi_close((fid_t) av->util_coll_av);

	if (av->offload_coll_av)
		fi_close((fid_t) av->offload_coll_av);

	ret = ofi_av_close(&av->util_av);
	if (ret)
		return ret;

	ofi_rbmap_cleanup(&av->addr_map);
	ofi_bufpool_destroy(av->conn_pool);
	ofi_bufpool_destroy(av->peer_pool);
	free(av);
	return 0;
}

static struct fi_ops rxm_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_av_close,
	.bind = ofi_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av rxm_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = rxm_av_insert,
	.insertsvc = rxm_av_insertsvc,
	.insertsym = rxm_av_insertsym,
	.remove = rxm_av_remove,
	.lookup = rxm_av_lookup,
	.straddr = rxm_av_straddr,
	.av_set = rxm_av_set,
};

int rxm_util_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		     struct fid_av **fid_av, void *context, size_t conn_size,
		     void (*remove_handler)(struct util_ep *util_ep,
					    struct util_peer_addr *peer))
{
	struct util_domain *domain;
	struct util_av_attr util_attr;
	struct rxm_av *av;
	int ret;

	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;

	ret = ofi_bufpool_create(&av->peer_pool, sizeof(struct util_peer_addr),
				 0, 0, 0, OFI_BUFPOOL_INDEXED |
				 OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto free;

	ret = ofi_bufpool_create(&av->conn_pool, conn_size, 0, 0, 0, 0);
	if (ret)
		goto destroy1;

	ofi_rbmap_init(&av->addr_map, rxm_addr_compare);
	domain = container_of(domain_fid, struct util_domain, domain_fid);

	util_attr.context_len = sizeof(struct util_peer_addr *);
	util_attr.flags = 0;
	util_attr.addrlen = ofi_sizeof_addr_format(domain->addr_format);
	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

	ret = ofi_av_init(domain, attr, &util_attr, &av->util_av, context);
	if (ret)
		goto destroy2;

	av->util_av.av_fid.fid.ops = &rxm_av_fi_ops;
	av->util_av.av_fid.ops = &rxm_av_ops;
	av->util_av.remove_handler = remove_handler;
	*fid_av = &av->util_av.av_fid;
	return 0;

destroy2:
	ofi_bufpool_destroy(av->conn_pool);
destroy1:
	ofi_bufpool_destroy(av->peer_pool);
free:
	free(av);
	return ret;
}
