/*
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2017-2019 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (c) 2020 Cisco Systems, Inc. All rights reserved.
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


struct ofi_mr_cache_params cache_params = {
	.max_cnt = 1024,
};

static int util_mr_find_within(struct ofi_rbmap *map, void *key, void *data)
{
	struct ofi_mr_entry *entry = data;
	struct ofi_mr_info *info = key;

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
	FI_DBG(cache->domain->prov, FI_LOG_MR, "free %p (len: %zu)\n",
	       entry->info.iov.iov_base, entry->info.iov.iov_len);

	assert(!entry->storage_context);
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

	cache->storage.erase(&cache->storage, entry);
	cache->cached_cnt--;
	cache->cached_size -= entry->info.iov.iov_len;
}

static void util_mr_uncache_entry(struct ofi_mr_cache *cache,
				  struct ofi_mr_entry *entry)
{
	util_mr_uncache_entry_storage(cache, entry);

	if (entry->use_cnt == 0) {
		dlist_remove(&entry->list_entry);
		dlist_insert_tail(&entry->list_entry, &cache->flush_list);
	} else {
		cache->uncached_cnt++;
		cache->uncached_size += entry->info.iov.iov_len;
	}
}

/* Caller must hold ofi_mem_monitor lock as well as unsubscribe from the region */
void ofi_mr_cache_notify(struct ofi_mr_cache *cache, const void *addr, size_t len)
{
	struct ofi_mr_entry *entry;
	struct iovec iov;

	cache->notify_cnt++;
	iov.iov_base = (void *) addr;
	iov.iov_len = len;

	for (entry = cache->storage.overlap(&cache->storage, &iov); entry;
	     entry = cache->storage.overlap(&cache->storage, &iov))
		util_mr_uncache_entry(cache, entry);
}

bool ofi_mr_cache_flush(struct ofi_mr_cache *cache)
{
	struct ofi_mr_entry *entry;

	pthread_mutex_lock(&cache->monitor->lock);
	while (!dlist_empty(&cache->flush_list)) {
		dlist_pop_front(&cache->flush_list, struct ofi_mr_entry,
				entry, list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR, "flush %p (len: %zu)\n",
		       entry->info.iov.iov_base, entry->info.iov.iov_len);
		pthread_mutex_unlock(&cache->monitor->lock);

		util_mr_free_entry(cache, entry);
		pthread_mutex_lock(&cache->monitor->lock);
	}

	if (dlist_empty(&cache->lru_list)) {
		pthread_mutex_unlock(&cache->monitor->lock);
		return false;
	}

	do {
		dlist_pop_front(&cache->lru_list, struct ofi_mr_entry,
				entry, list_entry);
		dlist_init(&entry->list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR, "flush %p (len: %zu)\n",
		       entry->info.iov.iov_base, entry->info.iov.iov_len);

		util_mr_uncache_entry_storage(cache, entry);
		pthread_mutex_unlock(&cache->monitor->lock);

		util_mr_free_entry(cache, entry);
		pthread_mutex_lock(&cache->monitor->lock);

	} while (!dlist_empty(&cache->lru_list) &&
		 ((cache->cached_cnt >= cache_params.max_cnt) ||
		  (cache->cached_size >= cache_params.max_size)));
	pthread_mutex_unlock(&cache->monitor->lock);

	return true;
}

void ofi_mr_cache_delete(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	FI_DBG(cache->domain->prov, FI_LOG_MR, "delete %p (len: %zu)\n",
	       entry->info.iov.iov_base, entry->info.iov.iov_len);

	pthread_mutex_lock(&cache->monitor->lock);
	cache->delete_cnt++;

	if (--entry->use_cnt == 0) {
		if (!entry->storage_context) {
			cache->uncached_cnt--;
			cache->uncached_size -= entry->info.iov.iov_len;
			pthread_mutex_unlock(&cache->monitor->lock);
			util_mr_free_entry(cache, entry);
			return;
		}
		dlist_insert_tail(&entry->list_entry, &cache->lru_list);
	}
	pthread_mutex_unlock(&cache->monitor->lock);
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

	FI_DBG(cache->domain->prov, FI_LOG_MR, "create %p (len: %zu)\n",
	       info->iov.iov_base, info->iov.iov_len);

	*entry = util_mr_entry_alloc(cache);
	if (!*entry)
		return -FI_ENOMEM;

	(*entry)->storage_context = NULL;
	(*entry)->info = *info;
	(*entry)->use_cnt = 1;

	ret = cache->add_region(cache, *entry);
	if (ret)
		goto free;

	pthread_mutex_lock(&cache->monitor->lock);
	cur = cache->storage.find(&cache->storage, info);
	if (cur) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if ((cache->cached_cnt >= cache_params.max_cnt) ||
	    (cache->cached_size >= cache_params.max_size)) {
		cache->uncached_cnt++;
		cache->uncached_size += info->iov.iov_len;
	} else {
		if (cache->storage.insert(&cache->storage,
					  &(*entry)->info, *entry)) {
			ret = -FI_ENOMEM;
			goto unlock;
		}
		cache->cached_cnt++;
		cache->cached_size += info->iov.iov_len;

		ret = ofi_monitor_subscribe(cache->monitor, info->iov.iov_base,
					    info->iov.iov_len);
		if (ret) {
			util_mr_uncache_entry_storage(cache, *entry);
			cache->uncached_cnt++;
			cache->uncached_size += (*entry)->info.iov.iov_len;
		} else {
			(*entry)->subscribed = 1;
		}
	}
	pthread_mutex_unlock(&cache->monitor->lock);
	return 0;

unlock:
	pthread_mutex_unlock(&cache->monitor->lock);
free:
	util_mr_free_entry(cache, *entry);
	return ret;
}

int ofi_mr_cache_search(struct ofi_mr_cache *cache, const struct fi_mr_attr *attr,
			struct ofi_mr_entry **entry)
{
	struct ofi_mr_info info;
	int ret;

	assert(attr->iov_count == 1);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "search %p (len: %zu)\n",
	       attr->mr_iov->iov_base, attr->mr_iov->iov_len);

	info.iov = *attr->mr_iov;

	do {
		pthread_mutex_lock(&cache->monitor->lock);

		if ((cache->cached_cnt >= cache_params.max_cnt) ||
		    (cache->cached_size >= cache_params.max_size)) {
			pthread_mutex_unlock(&cache->monitor->lock);
			ofi_mr_cache_flush(cache);
			pthread_mutex_lock(&cache->monitor->lock);
		}

		cache->search_cnt++;
		*entry = cache->storage.find(&cache->storage, &info);
		if (*entry && ofi_iov_within(attr->mr_iov, &(*entry)->info.iov))
			goto hit;

		/* Purge regions that overlap with new region */
		while (*entry) {
			/* New entry will expand range of subscription */
			(*entry)->subscribed = 0;
			util_mr_uncache_entry(cache, *entry);
			*entry = cache->storage.find(&cache->storage, &info);
		}
		pthread_mutex_unlock(&cache->monitor->lock);

		ret = util_mr_cache_create(cache, &info, entry);
		if (ret && ret != -FI_EAGAIN) {
			if (ofi_mr_cache_flush(cache))
				ret = -FI_EAGAIN;
		}
	} while (ret == -FI_EAGAIN);

	return ret;

hit:
	cache->hit_cnt++;
	if ((*entry)->use_cnt++ == 0)
		dlist_remove_init(&(*entry)->list_entry);
	pthread_mutex_unlock(&cache->monitor->lock);
	return 0;
}

struct ofi_mr_entry *ofi_mr_cache_find(struct ofi_mr_cache *cache,
				       const struct fi_mr_attr *attr)
{
	struct ofi_mr_info info;
	struct ofi_mr_entry *entry;

	assert(attr->iov_count == 1);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "find %p (len: %zu)\n",
	       attr->mr_iov->iov_base, attr->mr_iov->iov_len);

	pthread_mutex_lock(&cache->monitor->lock);
	cache->search_cnt++;

	info.iov = *attr->mr_iov;
	entry = cache->storage.find(&cache->storage, &info);
	if (!entry) {
		goto unlock;
	}

	if (!ofi_iov_within(attr->mr_iov, &entry->info.iov)) {
		entry = NULL;
		goto unlock;
	}

	cache->hit_cnt++;
	if ((entry)->use_cnt++ == 0)
		dlist_remove_init(&(entry)->list_entry);

unlock:
	pthread_mutex_unlock(&cache->monitor->lock);
	return entry;
}

int ofi_mr_cache_reg(struct ofi_mr_cache *cache, const struct fi_mr_attr *attr,
		     struct ofi_mr_entry **entry)
{
	int ret;

	assert(attr->iov_count == 1);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "reg %p (len: %zu)\n",
	       attr->mr_iov->iov_base, attr->mr_iov->iov_len);

	*entry = util_mr_entry_alloc(cache);
	if (!*entry)
		return -FI_ENOMEM;

	pthread_mutex_lock(&cache->monitor->lock);
	cache->uncached_cnt++;
	cache->uncached_size += attr->mr_iov->iov_len;
	pthread_mutex_unlock(&cache->monitor->lock);

	(*entry)->info.iov = *attr->mr_iov;
	(*entry)->use_cnt = 1;
	(*entry)->storage_context = NULL;

	ret = cache->add_region(cache, *entry);
	if (ret)
		goto buf_free;

	return 0;

buf_free:
	util_mr_entry_free(cache, *entry);
	pthread_mutex_lock(&cache->monitor->lock);
	cache->uncached_cnt--;
	cache->uncached_size -= attr->mr_iov->iov_len;
	pthread_mutex_unlock(&cache->monitor->lock);
	return ret;
}

void ofi_mr_cache_cleanup(struct ofi_mr_cache *cache)
{
	/* If we don't have a domain, initialization failed */
	if (!cache->domain)
		return;

	FI_INFO(cache->domain->prov, FI_LOG_MR, "MR cache stats: "
		"searches %zu, deletes %zu, hits %zu notify %zu\n",
		cache->search_cnt, cache->delete_cnt, cache->hit_cnt,
		cache->notify_cnt);

	while (ofi_mr_cache_flush(cache))
		;

	pthread_mutex_destroy(&cache->lock);
	ofi_monitor_del_cache(cache);
	cache->storage.destroy(&cache->storage);
	ofi_atomic_dec32(&cache->domain->ref);
	ofi_bufpool_destroy(cache->entry_pool);
	assert(cache->cached_cnt == 0);
	assert(cache->cached_size == 0);
	assert(cache->uncached_cnt == 0);
	assert(cache->uncached_size == 0);
}

static void ofi_mr_rbt_destroy(struct ofi_mr_storage *storage)
{
	ofi_rbmap_destroy(storage->storage);
}

static struct ofi_mr_entry *ofi_mr_rbt_find(struct ofi_mr_storage *storage,
					    const struct ofi_mr_info *key)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_find(storage->storage, (void *) key);
	if (!node)
		return NULL;

	return node->data;
}

static struct ofi_mr_entry *ofi_mr_rbt_overlap(struct ofi_mr_storage *storage,
					       const struct iovec *key)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_search(storage->storage, (void *) key,
				util_mr_find_overlap);
	if (!node)
		return NULL;

	return node->data;
}

static int ofi_mr_rbt_insert(struct ofi_mr_storage *storage,
			     struct ofi_mr_info *key,
			     struct ofi_mr_entry *entry)
{
	assert(!entry->storage_context);
	return ofi_rbmap_insert(storage->storage, (void *) key, (void *) entry,
				(struct ofi_rbnode **) &entry->storage_context);
}

static int ofi_mr_rbt_erase(struct ofi_mr_storage *storage,
			    struct ofi_mr_entry *entry)
{
	assert(entry->storage_context);
	ofi_rbmap_delete(storage->storage,
			 (struct ofi_rbnode *) entry->storage_context);
	entry->storage_context = NULL;
	return 0;
}

static int ofi_mr_cache_init_rbt(struct ofi_mr_cache *cache)
{
	cache->storage.storage = ofi_rbmap_create(util_mr_find_within);
	if (!cache->storage.storage)
		return -FI_ENOMEM;

	cache->storage.overlap = ofi_mr_rbt_overlap;
	cache->storage.destroy = ofi_mr_rbt_destroy;
	cache->storage.find = ofi_mr_rbt_find;
	cache->storage.insert = ofi_mr_rbt_insert;
	cache->storage.erase = ofi_mr_rbt_erase;
	return 0;
}

static int ofi_mr_cache_init_storage(struct ofi_mr_cache *cache)
{
	int ret;

	switch (cache->storage.type) {
	case OFI_MR_STORAGE_DEFAULT:
	case OFI_MR_STORAGE_RBT:
		ret = ofi_mr_cache_init_rbt(cache);
		break;
	case OFI_MR_STORAGE_USER:
		ret = (cache->storage.storage && cache->storage.overlap &&
		      cache->storage.destroy && cache->storage.find &&
		      cache->storage.insert && cache->storage.erase) ?
			0 : -FI_EINVAL;
		break;
	default:
		ret = -FI_EINVAL;
		break;
	}

	return ret;
}

int ofi_mr_cache_init(struct util_domain *domain,
		      struct ofi_mem_monitor *monitor,
		      struct ofi_mr_cache *cache)
{
	int ret;

	assert(cache->add_region && cache->delete_region);
	if (!cache_params.max_cnt || !cache_params.max_size)
		return -FI_ENOSPC;

	pthread_mutex_init(&cache->lock, NULL);
	dlist_init(&cache->lru_list);
	dlist_init(&cache->flush_list);
	cache->cached_cnt = 0;
	cache->cached_size = 0;
	cache->uncached_cnt = 0;
	cache->uncached_size = 0;
	cache->search_cnt = 0;
	cache->delete_cnt = 0;
	cache->hit_cnt = 0;
	cache->notify_cnt = 0;
	cache->domain = domain;
	ofi_atomic_inc32(&domain->ref);

	ret = ofi_mr_cache_init_storage(cache);
	if (ret)
		goto dec;

	ret = ofi_monitor_add_cache(monitor, cache);
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
	ofi_monitor_del_cache(cache);
destroy:
	cache->storage.destroy(&cache->storage);
dec:
	ofi_atomic_dec32(&cache->domain->ref);
	pthread_mutex_destroy(&cache->lock);
	cache->domain = NULL;
	return ret;
}
