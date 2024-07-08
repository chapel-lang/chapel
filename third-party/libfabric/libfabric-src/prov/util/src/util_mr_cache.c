/*
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2017-2019 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (c) 2020 Cisco Systems, Inc. All rights reserved.
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

#include <config.h>
#include <stdlib.h>
#include <ofi_util.h>
#include <ofi_iov.h>
#include <ofi_mr.h>
#include <ofi_list.h>
#include <ofi_tree.h>
#include <ofi_enosys.h>


struct ofi_mr_cache_params cache_params = {
	.max_cnt = 1024,
	.cuda_monitor_enabled = true,
	.rocr_monitor_enabled = true,
	.ze_monitor_enabled = true,
};

static int util_mr_find_within(struct ofi_rbmap *map, void *key, void *data)
{
	struct ofi_mr_entry *entry = data;
	struct ofi_mr_info *info = key;

	if (info->peer_id < entry->info.peer_id)
		return -1;
	if (info->peer_id > entry->info.peer_id)
		return 1;

	if (ofi_iov_shifted_left(&info->iov, &entry->info.iov))
		return -1;
	if (ofi_iov_shifted_right(&info->iov, &entry->info.iov))
		return 1;

	return 0;
}

static int util_mr_find_overlap(struct ofi_rbmap *map, void *key, void *data)
{
	struct ofi_mr_entry *entry = data;
	struct ofi_mr_info *info = key;

	if (info->peer_id < entry->info.peer_id)
		return -1;
	if (info->peer_id > entry->info.peer_id)
		return 1;

	if (ofi_iov_left(&info->iov, &entry->info.iov))
		return -1;
	if (ofi_iov_right(&info->iov, &entry->info.iov))
		return 1;

	return 0;
}

static struct ofi_mr_entry *util_mr_entry_alloc(struct ofi_mr_cache *cache)
{
	struct ofi_mr_entry *entry;

	pthread_mutex_lock(&cache->lock);
	entry = ofi_buf_alloc(cache->entry_pool);
	pthread_mutex_unlock(&cache->lock);
	return entry;
}

static void util_mr_entry_free(struct ofi_mr_cache *cache,
			       struct ofi_mr_entry *entry)
{
	pthread_mutex_lock(&cache->lock);
	ofi_buf_free(entry);
	pthread_mutex_unlock(&cache->lock);
}

/* We cannot hold the monitor lock when freeing an entry.  This call
 * will result in freeing memory, which can generate a uffd event
 * (e.g. UNMAP).  If we hold the monitor lock, the uffd thread will
 * hang trying to acquire it in order to read the event, and this thread
 * will itself be blocked until the uffd event is read.
 */
static void util_mr_free_entry(struct ofi_mr_cache *cache,
			       struct ofi_mr_entry *entry)
{
	FI_DBG(cache->prov, FI_LOG_MR, "free %p (len: %zu)\n",
	       entry->info.iov.iov_base, entry->info.iov.iov_len);

	assert(!entry->node);
	cache->delete_region(cache, entry);
	util_mr_entry_free(cache, entry);
}

static void util_mr_uncache_entry_storage(struct ofi_mr_cache *cache,
					  struct ofi_mr_entry *entry)
{
	/* Without subscription context, we might unsubscribe from
	 * an address range in use by another region. As a result,
	 * we remain subscribed. This may result in extra
	 * notification events, but is harmless to correct operation.
	 */

	ofi_rbmap_delete(&cache->tree, entry->node);
	entry->node = NULL;

	cache->cached_cnt--;
	cache->cached_size -= entry->info.iov.iov_len;
}

static void util_mr_uncache_entry(struct ofi_mr_cache *cache,
				  struct ofi_mr_entry *entry)
{
	util_mr_uncache_entry_storage(cache, entry);

	if (entry->use_cnt == 0) {
		dlist_remove(&entry->list_entry);
		dlist_insert_tail(&entry->list_entry, &cache->dead_region_list);
	} else {
		cache->uncached_cnt++;
		cache->uncached_size += entry->info.iov.iov_len;
	}
}

static struct ofi_mr_entry *ofi_mr_rbt_find(struct ofi_rbmap *tree,
					    const struct ofi_mr_info *key)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_find(tree, (void *) key);
	if (!node)
		return NULL;

	return node->data;
}

static struct ofi_mr_entry *ofi_mr_rbt_overlap(struct ofi_rbmap *tree,
					       const struct iovec *key)
{
	struct ofi_rbnode *node;
	struct ofi_mr_info info = {0};

	info.iov = *key;

	node = ofi_rbmap_search(tree, (void *) &info,
				util_mr_find_overlap);
	if (!node)
		return NULL;

	return node->data;
}

/* Caller must hold ofi_mem_monitor lock as well as unsubscribe from the region */
void ofi_mr_cache_notify(struct ofi_mr_cache *cache, const void *addr, size_t len)
{
	struct ofi_mr_entry *entry;
	struct iovec iov;

	cache->notify_cnt++;
	iov.iov_base = (void *) addr;
	iov.iov_len = len;

	for (entry = ofi_mr_rbt_overlap(&cache->tree, &iov); entry;
	     entry = ofi_mr_rbt_overlap(&cache->tree, &iov))
		util_mr_uncache_entry(cache, entry);
}

/* Function to remove dead regions and prune MR cache size.
 * Returns true if any entries were flushed from the cache.
 */
bool ofi_mr_cache_flush(struct ofi_mr_cache *cache, bool flush_lru)
{
	struct dlist_entry free_list;
	struct ofi_mr_entry *entry;
	bool entries_freed;

	dlist_init(&free_list);

	pthread_mutex_lock(&mm_lock);

	dlist_splice_tail(&free_list, &cache->dead_region_list);

	while (flush_lru && !dlist_empty(&cache->lru_list)) {
		dlist_pop_front(&cache->lru_list, struct ofi_mr_entry,
				entry, list_entry);
		dlist_init(&entry->list_entry);
		util_mr_uncache_entry_storage(cache, entry);
		dlist_insert_tail(&entry->list_entry, &free_list);

		flush_lru = ofi_mr_cache_full(cache);
	}

	pthread_mutex_unlock(&mm_lock);

	entries_freed = !dlist_empty(&free_list);

	while(!dlist_empty(&free_list)) {
		dlist_pop_front(&free_list, struct ofi_mr_entry,
				entry, list_entry);
		FI_DBG(cache->prov, FI_LOG_MR, "flush %p (len: %zu)\n",
			entry->info.iov.iov_base, entry->info.iov.iov_len);
		util_mr_free_entry(cache, entry);
	}

	return entries_freed;
}

void ofi_mr_cache_delete(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	FI_DBG(cache->prov, FI_LOG_MR, "delete %p (len: %zu)\n",
	       entry->info.iov.iov_base, entry->info.iov.iov_len);

	pthread_mutex_lock(&mm_lock);
	cache->delete_cnt++;

	if (--entry->use_cnt == 0) {
		if (!entry->node) {
			cache->uncached_cnt--;
			cache->uncached_size -= entry->info.iov.iov_len;
			pthread_mutex_unlock(&mm_lock);
			util_mr_free_entry(cache, entry);
			return;
		}
		dlist_insert_tail(&entry->list_entry, &cache->lru_list);
	}
	pthread_mutex_unlock(&mm_lock);
}

/*
 * We cannot hold the monitor lock when allocating and registering the
 * mr_entry without creating a potential deadlock situation with the
 * memory monitor needing to acquire the same lock.  The underlying
 * calls may allocate memory, which can result in the monitor needing
 * to handle address mapping changes.  To handle this, we build the
 * new entry, then check under lock that a conflict with another thread
 * hasn't occurred.  If a conflict occurred, we return -EAGAIN and
 * restart the entire operation.
 */
static int
util_mr_cache_create(struct ofi_mr_cache *cache, const struct ofi_mr_info *info,
		     struct ofi_mr_entry **entry)
{
	struct ofi_mr_entry *cur;
	int ret;
	struct ofi_mem_monitor *monitor = cache->monitors[info->iface];

	assert(monitor);

	FI_DBG(cache->prov, FI_LOG_MR, "create %p (len: %zu)\n",
	       info->iov.iov_base, info->iov.iov_len);

	*entry = util_mr_entry_alloc(cache);
	if (!*entry)
		return -FI_ENOMEM;

	(*entry)->node = NULL;
	(*entry)->info = *info;
	(*entry)->use_cnt = 1;

	ret = cache->add_region(cache, *entry);
	if (ret)
		goto free;

	pthread_mutex_lock(&mm_lock);
	cur = ofi_mr_rbt_find(&cache->tree, info);
	if (cur) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (ofi_mr_cache_full(cache)) {
		cache->uncached_cnt++;
		cache->uncached_size += info->iov.iov_len;
	} else {
		if (ofi_rbmap_insert(&cache->tree, (void *) &(*entry)->info,
				     (void *) *entry, &(*entry)->node)) {
			ret = -FI_ENOMEM;
			goto unlock;
		}
		cache->cached_cnt++;
		cache->cached_size += info->iov.iov_len;

		ret = ofi_monitor_subscribe(monitor, info->iov.iov_base,
					    info->iov.iov_len,
					    &(*entry)->hmem_info);
		if (ret) {
			util_mr_uncache_entry_storage(cache, *entry);
			cache->uncached_cnt++;
			cache->uncached_size += (*entry)->info.iov.iov_len;
		}
	}
	pthread_mutex_unlock(&mm_lock);
	return 0;

unlock:
	pthread_mutex_unlock(&mm_lock);
free:
	util_mr_free_entry(cache, *entry);
	return ret;
}

int ofi_mr_cache_search(struct ofi_mr_cache *cache, const struct ofi_mr_info *info,
			struct ofi_mr_entry **entry)
{
	struct ofi_mem_monitor *monitor;
	bool flush_lru;
	int ret;

	monitor = cache->monitors[info->iface];
	if (!monitor) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"MR cache disabled for %s memory\n",
			fi_tostr(&info->iface, FI_TYPE_HMEM_IFACE));
		return -FI_ENOSYS;
	}

	FI_DBG(cache->prov, FI_LOG_MR, "search %p (len: %zu)\n",
	       info->iov.iov_base, info->iov.iov_len);

	do {
		pthread_mutex_lock(&mm_lock);
		flush_lru = ofi_mr_cache_full(cache);
		if (flush_lru || !dlist_empty(&cache->dead_region_list)) {
			pthread_mutex_unlock(&mm_lock);
			ofi_mr_cache_flush(cache, flush_lru);
			pthread_mutex_lock(&mm_lock);
		}

		cache->search_cnt++;
		*entry = ofi_mr_rbt_find(&cache->tree, info);

		if (*entry &&
		    ofi_iov_within(&info->iov, &(*entry)->info.iov) &&
		    monitor->valid(monitor, info, *entry))
			goto hit;

		/* Purge regions that overlap with new region */
		while (*entry) {
			util_mr_uncache_entry(cache, *entry);
			*entry = ofi_mr_rbt_find(&cache->tree, info);
		}
		pthread_mutex_unlock(&mm_lock);

		ret = util_mr_cache_create(cache, info, entry);
		if (ret && ret != -FI_EAGAIN) {
			if (ofi_mr_cache_flush(cache, true))
				ret = -FI_EAGAIN;
		}
	} while (ret == -FI_EAGAIN);

	return ret;

hit:
	cache->hit_cnt++;
	if ((*entry)->use_cnt++ == 0)
		dlist_remove_init(&(*entry)->list_entry);
	pthread_mutex_unlock(&mm_lock);
	return 0;
}

struct ofi_mr_entry *ofi_mr_cache_find(struct ofi_mr_cache *cache,
				       const struct fi_mr_attr *attr,
				       uint64_t flags)
{
	struct ofi_mr_info info;
	struct ofi_mr_entry *entry;
	struct ofi_mem_monitor *monitor;

	assert(attr->iov_count == 1);
	FI_DBG(cache->prov, FI_LOG_MR, "find %p (len: %zu)\n",
	       attr->mr_iov->iov_base, attr->mr_iov->iov_len);

	pthread_mutex_lock(&mm_lock);

	if (!dlist_empty(&cache->dead_region_list)) {
		pthread_mutex_unlock(&mm_lock);
		ofi_mr_cache_flush(cache, false);
		pthread_mutex_lock(&mm_lock);
	}

	cache->search_cnt++;

	info.peer_id = 0;
	ofi_mr_info_get_iov_from_mr_attr(&info, attr, flags);
	entry = ofi_mr_rbt_find(&cache->tree, &info);
	if (!entry) {
		goto unlock;
	}

	monitor = cache->monitors[entry->info.iface];

	if (ofi_iov_within(attr->mr_iov, &entry->info.iov) &&
	    monitor->valid(monitor, entry->info.iov.iov_base, entry)) {
		cache->hit_cnt++;
		if ((entry)->use_cnt++ == 0)
			dlist_remove_init(&(entry)->list_entry);
	} else {
		while (entry) {
			util_mr_uncache_entry(cache, entry);
			entry = ofi_mr_rbt_find(&cache->tree, &entry->info);
		}
	}

unlock:
	pthread_mutex_unlock(&mm_lock);
	return entry;
}

int ofi_mr_cache_reg(struct ofi_mr_cache *cache, const struct fi_mr_attr *attr,
		     struct ofi_mr_entry **entry, uint64_t flags)
{
	int ret;

	assert(attr->iov_count == 1);
	FI_DBG(cache->prov, FI_LOG_MR, "reg %p (len: %zu)\n",
	       attr->mr_iov->iov_base, attr->mr_iov->iov_len);

	*entry = util_mr_entry_alloc(cache);
	if (!*entry)
		return -FI_ENOMEM;

	pthread_mutex_lock(&mm_lock);
	cache->uncached_cnt++;
	cache->uncached_size += attr->mr_iov->iov_len;
	pthread_mutex_unlock(&mm_lock);

	ofi_mr_info_get_iov_from_mr_attr(&(*entry)->info, attr, flags);
	(*entry)->use_cnt = 1;
	(*entry)->node = NULL;

	ret = cache->add_region(cache, *entry);
	if (ret)
		goto buf_free;

	return 0;

buf_free:
	util_mr_entry_free(cache, *entry);
	pthread_mutex_lock(&mm_lock);
	cache->uncached_cnt--;
	cache->uncached_size -= attr->mr_iov->iov_len;
	pthread_mutex_unlock(&mm_lock);
	return ret;
}

void ofi_mr_cache_cleanup(struct ofi_mr_cache *cache)
{
	/* If we don't have a prov, initialization failed */
	if (!cache->prov)
		return;

	FI_INFO(cache->prov, FI_LOG_MR, "MR cache stats: "
		"searches %zu, deletes %zu, hits %zu notify %zu\n",
		cache->search_cnt, cache->delete_cnt, cache->hit_cnt,
		cache->notify_cnt);

	while (ofi_mr_cache_flush(cache, true))
		;

	pthread_mutex_destroy(&cache->lock);
	ofi_monitors_del_cache(cache);
	ofi_rbmap_cleanup(&cache->tree);
	if (cache->domain)
		ofi_atomic_dec32(&cache->domain->ref);
	ofi_bufpool_destroy(cache->entry_pool);
	assert(cache->cached_cnt == 0);
	assert(cache->cached_size == 0);
	assert(cache->uncached_cnt == 0);
	assert(cache->uncached_size == 0);
}

/* Monitors array must be of size OFI_HMEM_MAX. */
int ofi_mr_cache_init(struct util_domain *domain,
		      struct ofi_mem_monitor **monitors,
		      struct ofi_mr_cache *cache)
{
	int ret;

	assert(cache->add_region && cache->delete_region);
	if (!cache_params.max_cnt || !cache_params.max_size)
		return -FI_ENOSPC;

	pthread_mutex_init(&cache->lock, NULL);
	dlist_init(&cache->lru_list);
	dlist_init(&cache->dead_region_list);
	cache->cached_cnt = 0;
	cache->cached_size = 0;
	cache->cached_max_cnt = cache_params.max_cnt;
	cache->cached_max_size = cache_params.max_size;
	cache->uncached_cnt = 0;
	cache->uncached_size = 0;
	cache->search_cnt = 0;
	cache->delete_cnt = 0;
	cache->hit_cnt = 0;
	cache->notify_cnt = 0;
	cache->domain = domain;
	if (domain) {
		cache->prov = domain->prov;
		ofi_atomic_inc32(&domain->ref);
	} else {
		cache->prov = (const struct fi_provider *) &core_prov;
	}

	ofi_rbmap_init(&cache->tree, util_mr_find_within);
	ret = ofi_monitors_add_cache(monitors, cache);
	if (ret)
		goto destroy;

	ret = ofi_bufpool_create(&cache->entry_pool,
				 sizeof(struct ofi_mr_entry) +
				 cache->entry_data_size,
				 16, 0, 0, 0);
	if (ret)
		goto del;

	return 0;
del:
	ofi_monitors_del_cache(cache);
destroy:
	ofi_rbmap_cleanup(&cache->tree);
	if (domain) {
		ofi_atomic_dec32(&cache->domain->ref);
		cache->domain = NULL;
	}
	pthread_mutex_destroy(&cache->lock);
	cache->prov = NULL;
	return ret;
}



static int ofi_close_cache_fid(struct fid *fid)
{
	free(fid);
	return 0;
}

static int ofi_bind_cache_fid(struct fid *fid, struct fid *bfid,
			      uint64_t flags)
{
	if (flags || bfid->fclass != FI_CLASS_MEM_MONITOR)
		return -FI_EINVAL;

	return ofi_monitor_import(bfid);
}

static struct fi_ops ofi_mr_cache_ops = {
	.size = sizeof(struct fi_ops),
	.close = ofi_close_cache_fid,
	.bind = ofi_bind_cache_fid,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
	.tostr = fi_no_tostr,
	.ops_set = fi_no_ops_set,
};

int ofi_open_mr_cache(uint32_t version, void *attr, size_t attr_len,
		      uint64_t flags, struct fid **fid, void *context)
{
	struct fid *cache_fid;

	if (FI_VERSION_LT(version, FI_VERSION(1, 13)) || attr_len)
		return -FI_EINVAL;

	if (flags)
		return -FI_EBADFLAGS;

	cache_fid = calloc(1, sizeof(*cache_fid));
	if (!cache_fid)
		return -FI_ENOMEM;

	cache_fid->fclass = FI_CLASS_MR_CACHE;
	cache_fid->context = context;
	cache_fid->ops = &ofi_mr_cache_ops;
	*fid = cache_fid;
	return 0;
}
