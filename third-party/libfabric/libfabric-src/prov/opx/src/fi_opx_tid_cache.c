/*
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (C) 2022-2024 Cornelis Networks.
 *
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2017-2019 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2020 Cisco Systems, Inc. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 *
 * Below is the Copyrights and Statements Corresponding the the util code we utilize
 * Copyright (c) 2017-2019 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2019-2021 Amazon.com, Inc. or its affiliates. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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
 *
 * Cornelis Networks commentary on copied code.
 *
 * We've found what we think are unique requirements for using the util
 * cache/monitor code on small TID registrations including future work
 * to move the cache from the domain to per-endpoint specific.
 *
 * So we've been copying and modifying and attributing code from util
 * into OPX for the time being.
 *
 */
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "fi_opx_tid.h"
#include "fi_opx_tid_cache.h"
#include <ofi_iov.h>

static const char *OPX_TID_CACHE_ENTRY_STATUS[] = {
	[OPX_TID_CACHE_ENTRY_NOT_FOUND]	    = "OPX_TID_CACHE_ENTRY_NOT_FOUND",
	[OPX_TID_CACHE_ENTRY_FOUND]	    = "OPX_TID_CACHE_ENTRY_FOUND",
	[OPX_TID_CACHE_ENTRY_OVERLAP_LEFT]  = "OPX_TID_CACHE_ENTRY_OVERLAP_LEFT",
	[OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT] = "OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT",
	[OPX_TID_CACHE_ENTRY_IN_USE]	    = "OPX_TID_CACHE_ENTRY_IN_USE"};

#ifndef NDEBUG
#define OPX_DEBUG_UCNT(entryp)                                                                                         \
	do {                                                                                                           \
		const uint64_t entry_vaddr =                                                                           \
			entryp ? ((struct opx_tid_mr *) (entryp)->data)->tid_info.tid_vaddr : 0UL;                     \
		const uint64_t entry_length =                                                                          \
			entryp ? ((struct opx_tid_mr *) (entryp)->data)->tid_info.tid_length : 0UL;                    \
		const int32_t entry_use_cnt = entryp ? ((struct ofi_mr_entry *) (entryp))->use_cnt : 0X0BAD;           \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_UCNT (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n", \
		       entryp, entryp ? entryp->data : NULL, (void *) entry_vaddr,                                     \
		       (void *) (entry_vaddr + entry_length), entry_length, entry_length, entry_use_cnt);              \
	} while (0)
#else
#define OPX_DEBUG_UCNT(entryp)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_ENTRY(info)                                                                                    \
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_ENTRY [%p - %p] (len: %zu/%#lX)\n", info->iov.iov_base, \
	       (char *) info->iov.iov_base + info->iov.iov_len, info->iov.iov_len, info->iov.iov_len)
#else
#define OPX_DEBUG_ENTRY(info)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_EXIT(entryp, ret)                                                                                    \
	do {                                                                                                           \
		const uint64_t entry_vaddr =                                                                           \
			entryp ? ((struct opx_tid_mr *) (entryp)->data)->tid_info.tid_vaddr : 0UL;                     \
		const uint64_t entry_length =                                                                          \
			entryp ? ((struct opx_tid_mr *) (entryp)->data)->tid_info.tid_length : 0UL;                    \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu,%#lX) rc %d (%s)\n", \
		       entryp, entryp ? entryp->data : NULL, (void *) entry_vaddr,                                     \
		       (void *) (entry_vaddr + entry_length), entry_length, entry_length, ret,                         \
		       OPX_TID_CACHE_ENTRY_STATUS[ret]);                                                               \
		OPX_DEBUG_UCNT(entryp);                                                                                \
	} while (0)
#else
#define OPX_DEBUG_EXIT(entryp, ret)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_ENTRY2(entryp, ret)                                                                             \
	do {                                                                                                      \
		const uint64_t entry_vaddr =                                                                      \
			entryp ? ((struct opx_tid_mr *) (entryp)->data)->tid_info.tid_vaddr : 0UL;                \
		const uint64_t entry_length =                                                                     \
			entryp ? ((struct opx_tid_mr *) (entryp)->data)->tid_info.tid_length : 0UL;               \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                                             \
		       "OPX_DEBUG_ENTRY (%p/%p) [%p - %p] (len: %zu,%#lX) rc %d (%s)\n", entryp,                  \
		       entryp ? entryp->data : NULL, (void *) entry_vaddr, (void *) (entry_vaddr + entry_length), \
		       entry_length, entry_length, ret, OPX_TID_CACHE_ENTRY_STATUS[ret]);                         \
		OPX_DEBUG_UCNT(entryp);                                                                           \
	} while (0)
#else
#define OPX_DEBUG_ENTRY2(entryp, ret)
#endif

/* RBMAP compare functions. */

/*
 * COPIED FROM util_mr_find_overlap which was static and so it was
 * inaccessible to OPX direct calls.
 *
 * Copied here for debug (env var OPX_FIND_WITHIN)
 */
#ifndef NDEBUG
static int opx_util_mr_find_within(struct ofi_rbmap *map, void *key, void *data)
{
	struct ofi_mr_entry *entry = data;
	struct ofi_mr_info  *info  = key;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_MR,
		     "OPX_DEBUG_ENTRY KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX)\n",
		     info->iov.iov_base, (char *) (info->iov.iov_base) + info->iov.iov_len, info->iov.iov_len,
		     info->iov.iov_len, entry->info.iov.iov_base,
		     (char *) (entry->info.iov.iov_base) + entry->info.iov.iov_len, entry->info.iov.iov_len,
		     entry->info.iov.iov_len);

	if (ofi_iov_shifted_left(&info->iov, &entry->info.iov)) {
		return -1;
	}
	if (ofi_iov_shifted_right(&info->iov, &entry->info.iov)) {
		return 1;
	}

	return 0;
}
#endif

/* Register/TID Update (pin) the pages.
 *
 * Hold the cache->lock across registering the TIDs  */
__OPX_FORCE_INLINE__
int opx_register_tid_region(uint64_t tid_vaddr, uint64_t tid_length, enum fi_hmem_iface tid_iface, uint64_t tid_device,
			    struct fi_opx_ep *opx_ep, struct opx_mr_tid_info *tid_info)
{
	uint64_t flags = (uint64_t) OPX_HMEM_KERN_MEM_TYPE[tid_iface];

	/* Parameters must be aligned for expected receive */
	assert(tid_vaddr == (tid_vaddr & -(int64_t) OPX_TID_PAGE_SIZE[tid_iface]));
	assert(tid_length == (tid_length & -(int64_t) OPX_TID_PAGE_SIZE[tid_iface]));

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&opx_ep->tid_domain->tid_cache->lock) == EBUSY);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "vaddr %p, length %lu\n", (void *) tid_vaddr, tid_length);

	uint32_t length_chunk = (uint32_t) tid_length;
	assert((tid_info->tid_length == 0) && (tid_info->tid_vaddr == 0));

	uint64_t *tidlist = (uint64_t *) &tid_info->info[0];

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_updates);
	FI_OPX_DEBUG_COUNTERS_INC_COND(tid_iface > FI_HMEM_SYSTEM, opx_ep->debug_counters.hmem.tid_update);

	uint32_t tidcnt_chunk = FI_OPX_MAX_DPUT_TIDPAIRS;
	int	 ret	      = opx_hfi1_wrapper_update_tid(opx_ep->hfi, tid_vaddr, /* input */
							    &length_chunk,	    /* input/output*/
							    (uint64_t) tidlist, /* input/output ptr cast as uint64_t */
							    &tidcnt_chunk,	/* output */
							    flags);

	if (ret) { // ERROR, no TIDs were registered
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_update_fail);
		return ret;
	}

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_update_success);
	FI_OPX_DEBUG_COUNTERS_INC_COND(length_chunk < tid_length,
				       opx_ep->debug_counters.expected_receive.tid_update_success_partial);

	tid_info->tid_vaddr  = tid_vaddr;
	tid_info->tid_length = length_chunk;
	tid_info->ninfo	     = tidcnt_chunk; /* appended or replaced */

	opx_tid_regen_pairs(tid_info->tid_length, tid_info->ninfo, tid_info->info, &tid_info->npairs, tid_info->pairs,
			    FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep));

	return FI_SUCCESS;
}

/* Free the tids in the cache entry
 *
 * Hold the cache->lock across deregistering the TIDs  */
void opx_deregister_tid_region(struct fi_opx_ep *opx_ep, struct opx_mr_tid_info *const tid_info)
{
	uint32_t  old_ntidinfo = tid_info->ninfo;
	uint64_t *old_tidlist  = (uint64_t *) &tid_info->info[0];
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_ENTRY vaddr %p, length %lu, ninfo %u \n",
	       (void *) tid_info->tid_vaddr, tid_info->tid_length, old_ntidinfo);

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&opx_ep->tid_domain->tid_cache->lock) == EBUSY);

	opx_hfi1_wrapper_free_tid(opx_ep->hfi, (uint64_t) old_tidlist, old_ntidinfo);
}

/*
 * COPIED FROM util_mr_find_overlap which was static and so it was
 * inaccessible to OPX direct calls.
 *
 * Copied here because OPX prefers it over find within
 */
static int opx_util_mr_find_overlap(struct ofi_rbmap *map, void *key, void *data)
{
	struct ofi_mr_entry *entry = data;
	struct ofi_mr_info  *info  = key;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_MR,
		     "OPX_DEBUG_ENTRY KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		     info->iov.iov_base, (char *) (info->iov.iov_base) + info->iov.iov_len, info->iov.iov_len,
		     info->iov.iov_len, entry->info.iov.iov_base,
		     (char *) (entry->info.iov.iov_base) + entry->info.iov.iov_len, entry->info.iov.iov_len,
		     entry->info.iov.iov_len, entry->use_cnt);
	if (ofi_iov_left(&info->iov, &entry->info.iov)) {
		return -1;
	}
	if (ofi_iov_right(&info->iov, &entry->info.iov)) {
		return 1;
	}

	OPX_DEBUG_EXIT(entry, OPX_TID_CACHE_ENTRY_FOUND);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_MR,
		     "OPX_DEBUG_EXIT KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		     info->iov.iov_base, (char *) (info->iov.iov_base) + info->iov.iov_len, info->iov.iov_len,
		     info->iov.iov_len, entry->info.iov.iov_base,
		     (char *) (entry->info.iov.iov_base) + entry->info.iov.iov_len, entry->info.iov.iov_len,
		     entry->info.iov.iov_len, entry->use_cnt);
	return 0;
}

/* Call directly instead of callback
 *
 * Hold the cache->lock across delete/deregistering the TIDs */
void opx_tid_cache_delete_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	struct opx_tid_mr *opx_mr = (struct opx_tid_mr *) entry->data;

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&opx_mr->opx_ep->tid_domain->tid_cache->lock) == EBUSY);

	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY entry %p, data %p opx_domain %p, endpoint %p\n", entry,
	       opx_mr, opx_mr->domain, opx_mr->opx_ep);
	OPX_DEBUG_ENTRY2(entry, OPX_TID_CACHE_ENTRY_FOUND);

	struct opx_mr_tid_info *const tid_reuse_cache = &opx_mr->tid_info;
	struct fi_opx_ep *const	      opx_ep	      = opx_mr->opx_ep;
	const void *const	      iov_base	      = entry->info.iov.iov_base;
	const size_t		      iov_len	      = entry->info.iov.iov_len;
	assert(entry->use_cnt == 0);
	/* Is this region current?  deregister it */
	if ((tid_reuse_cache->tid_length == iov_len) && (tid_reuse_cache->tid_vaddr == (uint64_t) iov_base)) {
		FI_DBG(cache->domain->prov, FI_LOG_MR, "ENTRY cache %p, entry %p, data %p, iov_base %p, iov_len %zu\n",
		       cache, entry, opx_mr, iov_base, iov_len);
		opx_deregister_tid_region(opx_ep, tid_reuse_cache);
	} else {
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "ENTRY OPX_TID_IS INVALID cache %p, entry %p, data %p, iov_base %p, iov_len %zu, tid_info->tid_vaddr %p tid_info->tid_length=%zu\n",
		       cache, entry, opx_mr, iov_base, iov_len, (void *) tid_reuse_cache->tid_vaddr,
		       tid_reuse_cache->tid_length);
	}

	memset(opx_mr, 0x00, sizeof(*opx_mr));
}

/* Cache static inlines */

__OPX_FORCE_INLINE__
int opx_tid_inc_use_cnt(struct ofi_mr_entry *entry)
{
#ifdef OPX_TID_DEBUG_USECNT
	fprintf(stderr, "(%d) %s:%s():%d [%p-%p/%lu] Entry %p Incrementing use_cnt %d -> %d\n", getpid(), __FILE__,
		__func__, __LINE__, entry->info.iov.iov_base,
		(void *) ((uintptr_t) entry->info.iov.iov_base + entry->info.iov.iov_len), entry->info.iov.iov_len,
		entry, entry->use_cnt, entry->use_cnt + 1);
#endif
	if (entry->use_cnt++ == 0) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "(%p/%p) remove lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n", entry,
		       entry->data, entry->info.iov.iov_base,
		       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->use_cnt);
		dlist_remove_init(&(entry)->list_entry);
	}
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu/%#lX) use_cnt %x\n", entry,
	       entry ? entry->data : NULL, entry->info.iov.iov_base,
	       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->use_cnt);
	return entry->use_cnt;
}

__OPX_FORCE_INLINE__
int opx_tid_dec_use_cnt(struct ofi_mr_entry *entry)
{
#ifdef OPX_TID_DEBUG_USECNT
	if (entry->use_cnt == 0) {
		fprintf(stderr,
			"(%d) %s:%s():%d [%p-%p/%lu] Entry %p Decrementing use_cnt %d -> %d, ERROR, Negative use_cnt!\n",
			getpid(), __FILE__, __func__, __LINE__, entry->info.iov.iov_base,
			(void *) ((uintptr_t) entry->info.iov.iov_base + entry->info.iov.iov_len),
			entry->info.iov.iov_len, entry, entry->use_cnt, entry->use_cnt - 1);
		abort();
	}
	fprintf(stderr, "(%d) %s:%s():%d [%p-%p/%lu] Entry %p Decrementing use_cnt %d -> %d\n", getpid(), __FILE__,
		__func__, __LINE__, entry->info.iov.iov_base,
		(void *) ((uintptr_t) entry->info.iov.iov_base + entry->info.iov.iov_len), entry->info.iov.iov_len,
		entry, entry->use_cnt, entry->use_cnt - 1);
#endif
	entry->use_cnt--;
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu/%#lX) use_cnt %x\n", entry,
	       entry ? entry->data : NULL, entry->info.iov.iov_base,
	       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->use_cnt);
	return entry->use_cnt;
}

/* Copied from ofi_mr_cache_full */
__OPX_FORCE_INLINE__
bool opx_tid_cache_full(struct ofi_mr_cache *cache)
{
	/* OPX is limited by the max number of TIDs
	 * being < cache_params.max_cnt but
	   not by size (cache_params.max_size) */
	FI_DBG(cache->domain->prov, FI_LOG_MR, "cache->cached_cnt %zu, cache_params.max_cnt %zu\n", cache->cached_cnt,
	       cache_params.max_cnt);
	if (OFI_UNLIKELY(cache->cached_cnt >= cache_params.max_cnt)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"FULL cache->cached_cnt %zu, cache_params.max_cnt %zu, cache->cached_size %zu, cache_params.max_size %zu\n",
			cache->cached_cnt, cache_params.max_cnt, cache->cached_size, cache_params.max_size);
	}

	return (cache->cached_cnt >= cache_params.max_cnt);
}

/*
 * COPIED FROM ofi_mr_cache_init which was static and so it was
 * inaccessible to OPX direct calls.
 */
__OPX_FORCE_INLINE__
int opx_tid_cache_init(struct util_domain *domain, struct ofi_mem_monitor **monitors, struct ofi_mr_cache *cache)
{
	int ret;

	if (!cache_params.max_cnt || !cache_params.max_size) {
		return -FI_ENOSPC;
	}

	pthread_mutex_init(&cache->lock, NULL);
	dlist_init(&cache->lru_list);
	dlist_init(&cache->dead_region_list);
	cache->cached_cnt    = 0;
	cache->cached_size   = 0;
	cache->uncached_cnt  = 0;
	cache->uncached_size = 0;
	cache->search_cnt    = 0;
	cache->delete_cnt    = 0;
	cache->hit_cnt	     = 0;
	cache->notify_cnt    = 0;
	cache->domain	     = domain;
	ofi_atomic_inc32(&domain->ref);

#ifndef NDEBUG
	ofi_rbmap_init(&cache->tree, getenv("OPX_FIND_WITHIN") ? opx_util_mr_find_within : opx_util_mr_find_overlap);
#else
	ofi_rbmap_init(&cache->tree, opx_util_mr_find_overlap);
#endif
	ret = ofi_monitors_add_cache(monitors, cache);
	if (ret) {
		goto destroy;
	}

	ret = ofi_bufpool_create(&cache->entry_pool, sizeof(struct ofi_mr_entry) + cache->entry_data_size, 16, 131072,
				 0, 0);
	if (ret) {
		goto del;
	}

	return 0;
del:
	ofi_monitors_del_cache(cache);
destroy:
	ofi_rbmap_cleanup(&cache->tree);
	ofi_atomic_dec32(&cache->domain->ref);
	pthread_mutex_destroy(&cache->lock);
	cache->domain = NULL;
	return ret;
}

/*
 * COPIED FROM ofi_mr_rbt_find which was static and so it was
 * inaccessible to OPX direct calls.
 */

__OPX_FORCE_INLINE__
struct ofi_mr_entry *opx_mr_rbt_find(struct ofi_rbmap *tree, const struct ofi_mr_info *key)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_find(tree, (void *) key);
	if (!node) {
		return NULL;
	}
	return node->data;
}

/*
 * COPIED FROM util_mr_entry_alloc which was static and so it was
 * inaccessible to OPX direct calls.
 */
__OPX_FORCE_INLINE__
struct ofi_mr_entry *opx_mr_entry_alloc(struct ofi_mr_cache *cache)
{
	struct ofi_mr_entry *entry;

	pthread_mutex_lock(&cache->lock);
	entry = ofi_buf_alloc(cache->entry_pool);
	pthread_mutex_unlock(&cache->lock);

	return entry;
}

__OPX_FORCE_INLINE__
int opx_mr_entry_alloc_init(struct ofi_mr_cache *cache, const struct ofi_mr_info *info, struct fi_opx_ep *opx_ep,
			    struct ofi_mr_entry **entry, struct opx_mr_tid_info **tid_info)
{
	*entry = opx_mr_entry_alloc(cache);

	if (OFI_UNLIKELY(!(*entry))) {
		FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY FI_NOMEM [%p - %p] (len: %zu/%#lX) \n",
		       info->iov.iov_base, (char *) info->iov.iov_base + info->iov.iov_len, info->iov.iov_len,
		       info->iov.iov_len);
		return -FI_ENOMEM;
	}

	(*entry)->node	  = NULL;
	(*entry)->info	  = *info;
	(*entry)->use_cnt = 0;
	dlist_init(&((*entry)->list_entry));

	struct opx_tid_mr     *opx_mr	  = (struct opx_tid_mr *) (*entry)->data;
	struct opx_tid_domain *opx_domain = container_of(cache->domain, struct opx_tid_domain, util_domain);

	opx_mr->domain = opx_domain;
	opx_mr->opx_ep = opx_ep; /* regions are ep specific (ioctl(fd-from-ep-context)) */

	*tid_info = &opx_mr->tid_info;

	(*tid_info)->ninfo	= 0;
	(*tid_info)->npairs	= 0;
	(*tid_info)->tid_vaddr	= 0UL;
	(*tid_info)->tid_length = 0UL;

#ifndef NDEBUG
	// Poison the TID pairs with values that would cause a failure
	// if they were accidentally used
	for (int i = 0; i < FI_OPX_MAX_DPUT_TIDPAIRS; ++i) {
		(*tid_info)->info[i]  = -1U;
		(*tid_info)->pairs[i] = -1U;
	}
#endif
	return FI_SUCCESS;
}

/*
 * COPIED FROM util_mr_uncache_entry_storage which was static and so it was
 * inaccessible to OPX direct calls.
 */

__OPX_FORCE_INLINE__
void opx_mr_uncache_entry_storage(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	OPX_DEBUG_ENTRY2(entry, OPX_TID_CACHE_ENTRY_FOUND);
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

/*
 * COPIED FROM util_mr_entry_free which was static and so it was
 * inaccessible to OPX direct calls.
 */

/* We cannot hold the monitor lock when freeing an entry.  This call
 * will result in freeing memory, which can generate a uffd event
 * (e.g. UNMAP).  If we hold the monitor lock, the uffd thread will
 * hang trying to acquire it in order to read the event, and this thread
 * will itself be blocked until the uffd event is read.
 *
 * COPIED FROM util_mr_free_entry which was static and so it was
 * inaccessible to OPX direct calls.
 */
__OPX_FORCE_INLINE__
void opx_cache_free_entry(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY free (%p/%p) [%p - %p] (len: %zu/%#lX) use_cnt %x\n",
	       entry, entry ? entry->data : NULL, entry->info.iov.iov_base,
	       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->use_cnt);

	assert(!entry->node);

	pthread_mutex_lock(&cache->lock);
	opx_tid_cache_delete_region(cache, entry);
	OPX_DEBUG_ENTRY((&entry->info));
	OPX_DEBUG_ENTRY2(entry, OPX_TID_CACHE_ENTRY_FOUND);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "entry %p use_cnt %x\n", entry, entry->use_cnt);
	OPX_BUF_FREE(entry);
	pthread_mutex_unlock(&cache->lock);
	OPX_DEBUG_EXIT(((struct ofi_mr_entry *) NULL), OPX_TID_CACHE_ENTRY_NOT_FOUND);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_EXIT\n");
}

__OPX_FORCE_INLINE__
int opx_register_tid_region_retryable(struct ofi_mr_cache *cache, uint64_t tid_vaddr, uint64_t tid_length,
				      enum fi_hmem_iface tid_iface, uint64_t tid_device, struct fi_opx_ep *opx_ep,
				      struct opx_mr_tid_info *tid_info)
{
	int ret;

	/* Hold the cache->lock across registering the TIDs  */
	pthread_mutex_lock(&cache->lock);
	{
		ret = opx_register_tid_region(tid_vaddr, tid_length, tid_iface, tid_device, opx_ep, tid_info);
	}
	pthread_mutex_unlock(&cache->lock);

	/* If no TIDs were available, flush the cache's dead list and retry.
	   Note that we can only try flushing the dead list, and must NOT free
	   any LRU entries, as we may be in the midst of collecting a range of
	   entries who's use counts we have not yet incremented, and we can't
	   have them freed out from under us right now. */
	if (OFI_UNLIKELY(ret == -FI_ENOSPC)) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_flush_not_lru);
		if (opx_tid_cache_flush_all(cache, false, false) > 0) {
			FI_OPX_DEBUG_COUNTERS_INC(
				opx_ep->debug_counters.expected_receive.tid_cache_flush_not_lru_helped);
			pthread_mutex_lock(&cache->lock);
			{
				ret = opx_register_tid_region(tid_vaddr, tid_length, tid_iface, tid_device, opx_ep,
							      tid_info);
			}
			pthread_mutex_unlock(&cache->lock);
		}
	}

	return ret;
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
 *
 * COPIED FROM util_mr_cache_create which was static and so it was
 * inaccessible to OPX direct calls.
 *
 * mm_lock should be held when calling this function
 */
__OPX_FORCE_INLINE__
int opx_tid_cache_crte(struct ofi_mr_cache *cache, const struct ofi_mr_info *info, struct ofi_mr_entry **entry,
		       struct fi_opx_ep *opx_ep)
{
	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&mm_lock) == EBUSY);

	struct ofi_mem_monitor *monitor = cache->monitors[info->iface];
	assert(monitor);

	// Check for a full cache before we even try to register
	if (OFI_UNLIKELY(opx_tid_cache_full(cache))) {
		/* Note that we can only try flushing the dead list, and must NOT free
		   any LRU entries, as we may be in the midst of collecting a range of
		   entries who's use counts we have not yet incremented, and we can't
		   have them freed out from under us right now. */
		int freed_entries = opx_tid_cache_flush(cache, false);

		if (freed_entries == 0 || opx_tid_cache_full(cache)) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_full);
			FI_WARN(fi_opx_global.prov, FI_LOG_MR, "CACHE FULL UNCACHED ERROR\n");
			return -FI_EAGAIN;
		}
	}

	OPX_DEBUG_ENTRY(info);
	/* drop the mm lock across alloc/register */
	pthread_mutex_unlock(&mm_lock);
	struct opx_mr_tid_info *tid_info;
	int			ret = opx_mr_entry_alloc_init(cache, info, opx_ep, entry, &tid_info);
	if (OFI_UNLIKELY(ret)) {
		assert(ret == -FI_ENOMEM);
		/* re-acquire mm_lock */
		pthread_mutex_lock(&mm_lock);
		return -FI_ENOMEM;
	}

	const size_t register_max_len = opx_ep->hfi->ctrl->__hfi_tidexpcnt * OPX_TID_PAGE_SIZE[info->iface];
	ret			      = opx_register_tid_region_retryable(cache, (uint64_t) info->iov.iov_base,
									  MIN(info->iov.iov_len, register_max_len), info->iface, info->device,
									  opx_ep, tid_info);

	/* re-acquire mm_lock */
	pthread_mutex_lock(&mm_lock);

	if (ret) {
		/* Failed, tid_info->ninfo will be zero */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "opx_register_tid_region failed with return code %d (%s), FREE node %p\n", ret, strerror(ret),
		       (*entry)->node);
		goto error;
	}

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "NEW vaddr [%#lx - %#lx] length %lu, tid vaddr [%#lx - %#lx] , tid length %lu\n",
	       (uint64_t) info->iov.iov_base, (uint64_t) info->iov.iov_base + (uint64_t) info->iov.iov_len,
	       (uint64_t) info->iov.iov_len, tid_info->tid_vaddr, tid_info->tid_vaddr + tid_info->tid_length,
	       tid_info->tid_length);

	(*entry)->info.iov.iov_base = (void *) tid_info->tid_vaddr;
	(*entry)->info.iov.iov_len  = tid_info->tid_length;

	ret = ofi_rbmap_insert(&cache->tree, (void *) &(*entry)->info, (void *) *entry, &(*entry)->node);

	if (OFI_UNLIKELY(ret)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "ofi_rbmap_insert returned %d (%s) %p\n", ret, strerror(ret),
		       (*entry)->node);
		goto error;
	}
	cache->cached_cnt++;
	cache->cached_size += tid_info->tid_length;

	ret = ofi_monitor_subscribe(monitor, info->iov.iov_base, info->iov.iov_len, &(*entry)->hmem_info);
	if (OFI_UNLIKELY(ret)) {
		opx_mr_uncache_entry_storage(cache, *entry);
		cache->uncached_cnt++;
		cache->uncached_size += (*entry)->info.iov.iov_len;
		FI_WARN(fi_opx_global.prov, FI_LOG_MR, "MONITOR SUBSCRIBE FAILED UNCACHED ERROR\n");
		goto error;
	}
	OPX_DEBUG_EXIT((*entry), OPX_TID_CACHE_ENTRY_FOUND);
	return FI_SUCCESS;
error:
	/* Create failed to initialize the entry but it
	   exists...caller should delete the entry */
	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY ERROR [%p - %p] (len: %zu/%#lX) \n", info->iov.iov_base,
	       (char *) info->iov.iov_base + info->iov.iov_len, info->iov.iov_len, info->iov.iov_len);
	tid_info->npairs = 0; /* error == no tid pairs */
	OPX_DEBUG_EXIT((*entry), OPX_TID_CACHE_ENTRY_NOT_FOUND);
	return ret; // TODO - handle case for free
}

__OPX_FORCE_INLINE__
int opx_tid_register_and_cache_entry(struct ofi_mr_cache *cache, const struct ofi_mr_info *info,
				     struct ofi_mr_entry **entry, struct fi_opx_ep *opx_ep)
{
	int ret = opx_tid_cache_crte(cache, info, entry, opx_ep);
	if (OFI_UNLIKELY(ret != FI_SUCCESS && (*entry) != NULL)) {
		/*crte returns an entry even if tid update failed */
		/* Unlock for free/return */
		pthread_mutex_unlock(&mm_lock);
		opx_cache_free_entry(cache, *entry);
		*entry = NULL;
		pthread_mutex_lock(&mm_lock);
	}

	return ret;
}

/* Find is similar to search but it does not increment use_cnt.
 */
__OPX_FORCE_INLINE__
enum opx_tid_cache_entry_status opx_tid_cache_find(struct fi_opx_ep *opx_ep, const struct ofi_mr_info *info,
						   struct ofi_mr_entry **entry)
{
	enum opx_tid_cache_entry_status ret;

	OPX_DEBUG_ENTRY(info);

	struct ofi_mr_cache *cache = opx_ep->tid_domain->tid_cache;
	cache->search_cnt++;
	*entry				      = opx_mr_rbt_find(&cache->tree, info);
	const struct opx_tid_mr *const opx_mr = (*entry) ? (struct opx_tid_mr *) (*entry)->data : NULL;
	if (!*entry) {
		ret = OPX_TID_CACHE_ENTRY_NOT_FOUND;
	} else if (OFI_UNLIKELY(opx_mr->opx_ep != opx_ep)) {
		/* In use by an endpoint other than the requestor */

		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"OPX_ENTRY_IN_USE [%p - %p] (len: %zu/%#lX) ENTRY(%p/%p) ENDPOINT(mr:%p, input:%p)\n",
			info->iov.iov_base, (char *) info->iov.iov_base + info->iov.iov_len, info->iov.iov_len,
			info->iov.iov_len, (*entry), (*entry) ? (*entry)->data : NULL, opx_mr ? opx_mr->opx_ep : NULL,
			opx_ep);
		/* One-time message per endpoint.
		 * Future support - cache per endpoint
		 * and this code goes away. */
		if (opx_ep->use_expected_tid_rzv) {
			OPX_TID_CACHE_DEBUG_FPRINTF(
				"## %s:%u OPX_TID_CACHE_DEBUG Unsupported: OPX_ENTRY_IN_USE by another endpoint. Disabling TID (FI_OPX_TID_DISABLE) on this endpoint\n",
				__func__, __LINE__);
		}
		opx_ep->use_expected_tid_rzv = 0;
		ret			     = OPX_TID_CACHE_ENTRY_IN_USE;

	} else if (ofi_iov_within(&info->iov, &(*entry)->info.iov)) {
		ret = OPX_TID_CACHE_ENTRY_FOUND;
	} else if (info->iov.iov_base >= (*entry)->info.iov.iov_base) {
		// The search IOV starts within the range of the cached IOV, and
		// the end of the search IOV is after the end of the cached IOV
		assert(info->iov.iov_base <= ofi_iov_end(&(*entry)->info.iov));
		assert(ofi_iov_end(&info->iov) > ofi_iov_end(&(*entry)->info.iov));
		ret = OPX_TID_CACHE_ENTRY_OVERLAP_LEFT;
	} else {
		// The search IOV starts before the range of the cached IOV,
		// and ends at some point after the start of the cached IOV.
		assert(info->iov.iov_base < (*entry)->info.iov.iov_base);
		assert(ofi_iov_end(&info->iov) > (*entry)->info.iov.iov_base);
		ret = OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT;
	}

	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "INFO [%p - %p] (len: %zu/%#lX) ENTRY(%p/%p) ENDPOINT(mr:%p, input:%p)\n",
	       info->iov.iov_base, (char *) info->iov.iov_base + info->iov.iov_len, info->iov.iov_len,
	       info->iov.iov_len, (*entry), (*entry) ? (*entry)->data : NULL, opx_mr ? opx_mr->opx_ep : NULL, opx_ep);
	OPX_DEBUG_EXIT((*entry), ret);
	return ret;
}

__OPX_FORCE_INLINE__
void opx_tid_set_offset_and_copy_pairs(uintptr_t buf_vaddr, size_t buf_length,
				       const struct opx_mr_tid_info *const tid_entry,
				       struct opx_tid_addr_block	  *tid_addr_block)
{
	assert(tid_entry->tid_vaddr <= buf_vaddr);
	assert((tid_entry->tid_vaddr + tid_entry->tid_length >= buf_vaddr));

	// 1. Find index of first pair to use
	uintptr_t tid_entry_addr = tid_entry->tid_vaddr;
	uint32_t  cur_pair_len	 = (uint32_t) FI_OPX_EXP_TID_GET(tid_entry->pairs[0], LEN) * OPX_HFI1_TID_PAGESIZE;
	uintptr_t cur_pair_end	 = tid_entry_addr + cur_pair_len;
	int	  pair_index	 = 0;

	/* Note that cur_pair_end represents the address of the first byte
	   *after* the last byte of the current pair's address range.
	   e.g. tid_entry_addr = 0x10000
		cur_pair_len   = 0x01000
	   then cur_pair_end   = 0x11000
	   but the range of tid_entry_addr would be 0x10000-0x10FFF.
	   So if cur_pair_end == buf_vaddr, buf_vaddr is still beyond the
	   the address range represented by this pair, and we need to move on
	   to the next one. */
	while (cur_pair_end <= buf_vaddr) {
		assert(pair_index < tid_entry->npairs);
		tid_entry_addr += cur_pair_len;
		cur_pair_len =
			(uint32_t) FI_OPX_EXP_TID_GET(tid_entry->pairs[++pair_index], LEN) * OPX_HFI1_TID_PAGESIZE;
		cur_pair_end += cur_pair_len;
	}
	assert(pair_index < tid_entry->npairs);

	// 2. Set first_pair_tid_addr & offset
	tid_addr_block->target_iov.iov_base = (void *) tid_entry_addr;
	tid_addr_block->offset		    = buf_vaddr - tid_entry_addr;

	// 3. Copy the first pair (we'll always copy at least one pair)
	tid_addr_block->pairs[0] = tid_entry->pairs[pair_index++];

	uint32_t pairs_copied = 1;

	tid_entry_addr	  = cur_pair_end;
	uintptr_t buf_end = buf_vaddr + buf_length;

	// 4. Copy remaining pairs while we have not exhausted the tid entry or input buf length
	while (tid_entry_addr < buf_end && pair_index < tid_entry->npairs) {
		tid_addr_block->pairs[pairs_copied++] = tid_entry->pairs[pair_index];
		cur_pair_len =
			(uint32_t) FI_OPX_EXP_TID_GET(tid_entry->pairs[pair_index++], LEN) * OPX_HFI1_TID_PAGESIZE;
		tid_entry_addr += cur_pair_len;
	}

	// 5. Set npairs_copied
	tid_addr_block->npairs = pairs_copied;

	// 6. Set length of buf covered by the pairs in this tid entry
	tid_addr_block->target_iov.iov_len = (tid_entry_addr >= buf_end) ?
						     (buf_length + tid_addr_block->offset) :
						     (tid_entry_addr - (uintptr_t) tid_addr_block->target_iov.iov_base);
}

__OPX_FORCE_INLINE__
enum opx_tid_cache_entry_status opx_tid_cache_build_overlap_chain(struct fi_opx_ep *opx_ep, struct ofi_mr_cache *cache,
								  struct ofi_mr_info			find_info,
								  const enum opx_tid_cache_entry_status initial_find,
								  struct ofi_mr_entry		       *initial_entry,
								  struct opx_tid_cache_chain	       *result)
{
	assert(initial_find == OPX_TID_CACHE_ENTRY_OVERLAP_LEFT || initial_find == OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT);

	enum opx_tid_cache_entry_status find	  = initial_find;
	struct ofi_mr_entry	       *cur_entry = initial_entry;
	struct ofi_mr_entry	       *right_overlap[OPX_MAX_TID_COUNT];
	uint32_t			right_entries = 0;

	result->entry_count    = 0;
	result->range.iov_base = find_info.iov.iov_base;
	result->range.iov_len  = find_info.iov.iov_len;

	do {
		uintptr_t cur_entry_end = (uintptr_t) cur_entry->info.iov.iov_base + cur_entry->info.iov.iov_len;
		ssize_t	  overlap_bytes;
		if (find == OPX_TID_CACHE_ENTRY_OVERLAP_LEFT) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_overlap_left);
			assert(cur_entry->info.iov.iov_base <= find_info.iov.iov_base);
			result->entries[result->entry_count++] = cur_entry;
			overlap_bytes	       = (ssize_t) cur_entry_end - (uintptr_t) find_info.iov.iov_base;
			find_info.iov.iov_base = (void *) ((uintptr_t) find_info.iov.iov_base + overlap_bytes);
		} else {
			assert(find == OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT);
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_overlap_right);
			assert(cur_entry->info.iov.iov_base > find_info.iov.iov_base);
			uintptr_t find_info_end = (uintptr_t) find_info.iov.iov_base + find_info.iov.iov_len;
			if (OFI_UNLIKELY(find_info_end > cur_entry_end)) {
				// Disregard any right overlap entries we found previously
				right_overlap[0]      = cur_entry;
				right_entries	      = 1;
				find_info.iov.iov_len = cur_entry_end - (uintptr_t) find_info.iov.iov_base;
				overlap_bytes	      = cur_entry->info.iov.iov_len;
			} else {
				right_overlap[right_entries++] = cur_entry;
				overlap_bytes = (ssize_t) find_info_end - (uintptr_t) cur_entry->info.iov.iov_base;
			}
		}

		// If no overlap bytes, we should have returned
		// NOT_FOUND on the previous find attempt and already
		// exited the loop instead of iterating and getting here.
		assert(overlap_bytes > 0);

		// If overlap bytes is >= find_info.iov.iov_len, we should
		// have returned FOUND instead of an overlap
		assert(overlap_bytes < find_info.iov.iov_len);

		find_info.iov.iov_len -= overlap_bytes;

		find = opx_tid_cache_find(opx_ep, &find_info, &cur_entry);
	} while (find == OPX_TID_CACHE_ENTRY_OVERLAP_LEFT || find == OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT);

	if (OFI_UNLIKELY(find == OPX_TID_CACHE_ENTRY_IN_USE)) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_found_entry_in_use);
		return OPX_TID_CACHE_ENTRY_IN_USE;
	}

	if (find == OPX_TID_CACHE_ENTRY_FOUND) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_hit);
		result->entries[result->entry_count++] = cur_entry;

		// We need to copy in the right entries in reverse order, because
		// the lower the index in the right_overlap array, the further
		// "right" in the address range the entries are
		for (int i = right_entries - 1; i >= 0; i--) {
			result->entries[result->entry_count++] = right_overlap[i];
		}
		uintptr_t result_range_end = ((uintptr_t) result->entries[result->entry_count - 1]->info.iov.iov_base +
					      result->entries[result->entry_count - 1]->info.iov.iov_len);

		size_t total_len = result_range_end - (uintptr_t) result->range.iov_base;

		result->range.iov_len = MIN(result->range.iov_len, total_len);

		return OPX_TID_CACHE_ENTRY_FOUND;
	}

	assert(find == OPX_TID_CACHE_ENTRY_NOT_FOUND);

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_miss);

	cur_entry = NULL;
	int rc	  = opx_tid_register_and_cache_entry(cache, &find_info, &cur_entry, opx_ep);

	if (rc == FI_SUCCESS) {
		result->entries[result->entry_count++] = cur_entry;
	}

	// If we only found right overlap entries, and we failed to create
	// a new entry for the uncached left portion of the address range,
	// then we can't proceed.
	if (result->entry_count == 0) {
		assert(right_entries > 0);
		return OPX_TID_CACHE_ENTRY_NOT_FOUND;
	}

	uintptr_t result_range_end = ((uintptr_t) result->entries[result->entry_count - 1]->info.iov.iov_base +
				      result->entries[result->entry_count - 1]->info.iov.iov_len);

	// If the address range of the new entry we just created ends at the start
	// of the left-most right overlap entry, then we can add all the right
	// overlap entries to our result chain.
	if (right_entries > 0 && result_range_end == (uintptr_t) right_overlap[right_entries - 1]->info.iov.iov_base) {
		for (int i = right_entries - 1; i >= 0; i--) {
			result->entries[result->entry_count++] = right_overlap[i];
		}

		result_range_end =
			((uintptr_t) right_overlap[0]->info.iov.iov_base + right_overlap[0]->info.iov.iov_len);
	}

	size_t total_len      = result_range_end - (uintptr_t) result->range.iov_base;
	result->range.iov_len = MIN(result->range.iov_len, total_len);

	return OPX_TID_CACHE_ENTRY_FOUND;
}

__OPX_FORCE_INLINE__
void opx_tid_cache_combine_chain_entries(struct opx_tid_cache_chain *overlap_chain,
					 struct fi_opx_hmem_iov	    *cur_addr_range,
					 struct opx_tid_addr_block  *tid_addr_block)
{
	opx_tid_inc_use_cnt(overlap_chain->entries[0]);

	struct opx_mr_tid_info *cached_tid_entry = &((struct opx_tid_mr *) overlap_chain->entries[0]->data)->tid_info;

	opx_tid_set_offset_and_copy_pairs(cur_addr_range->buf, cur_addr_range->len, cached_tid_entry, tid_addr_block);

	uintptr_t target_iov_end = (uintptr_t) tid_addr_block->target_iov.iov_base + tid_addr_block->target_iov.iov_len;
	uintptr_t overlap_range_end = (uintptr_t) overlap_chain->range.iov_base + +overlap_chain->range.iov_len;

	tid_addr_block->target_iov.iov_len += overlap_range_end - target_iov_end;

	for (int i = 1; i < overlap_chain->entry_count; i++) {
		cached_tid_entry = &((struct opx_tid_mr *) overlap_chain->entries[i]->data)->tid_info;
		assert(cached_tid_entry->tid_length != 0);
		assert(cached_tid_entry->npairs != 0);

		opx_tid_inc_use_cnt(overlap_chain->entries[i]);

		uint32_t cur_npairs = tid_addr_block->npairs;
		for (int j = 0; j < cached_tid_entry->npairs; j++) {
			tid_addr_block->pairs[cur_npairs + j] = cached_tid_entry->pairs[j];
		}
		tid_addr_block->npairs += cached_tid_entry->npairs;
	}

	uintptr_t buf_end = cur_addr_range->buf + cur_addr_range->len;

	tid_addr_block->target_iov.iov_len =
		MIN(buf_end, overlap_range_end) - (uintptr_t) tid_addr_block->target_iov.iov_base;
}

/*
 * COPIED FROM ofi_mr_cache_delete for opx debug
 *
 * mm_lock should be held when callng this function
 */

__OPX_FORCE_INLINE__
int opx_tid_cache_close_region(struct ofi_mr_cache *tid_cache, struct ofi_mr_entry *entry)
{
	/* TODO ... fix? */
	OPX_DEBUG_ENTRY2(entry, OPX_TID_CACHE_ENTRY_FOUND);
	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&mm_lock) == EBUSY);

	// Start of opx_tid_cache_delete
	FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY delete [%p - %p] (len: %zu,%#lX)\n",
	       entry->info.iov.iov_base, (char *) entry->info.iov.iov_base + entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->info.iov.iov_len);

	tid_cache->delete_cnt++;

	const int use_cnt = opx_tid_dec_use_cnt(entry);

	if (use_cnt == 0) {
		OPX_DEBUG_UCNT(entry);
		FI_DBG(tid_cache->domain->prov, FI_LOG_MR,
		       "node %p, (%p/%p) insert lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n", entry->node, entry,
		       entry->data, entry->info.iov.iov_base,
		       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->use_cnt);
		if (!entry->node) {
			tid_cache->uncached_cnt--;
			tid_cache->uncached_size -= entry->info.iov.iov_len;
			pthread_mutex_unlock(&mm_lock);
			opx_cache_free_entry(tid_cache, entry);
			/* re-acquire mm_lock */
			pthread_mutex_lock(&mm_lock);
			return 0;
		}
		FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "LRU entry %p\n", entry);
		dlist_insert_tail(&entry->list_entry, &tid_cache->lru_list);
	}
	OPX_DEBUG_UCNT(entry);

	return 0;
}

/****************************************************
 *
 * Dump Cache functions
 * - opx_tid_cache_dump_entry
 * - opx_tid_cache_dump_entries
 * - opx_tid_cache_dump_dlist
 * - opx_tid_cache_dump_cache
 *
 * These functions are for dumping the TID cache in
 * a debug situation. The functions are not actually
 * used/called from anywhere, just available in case
 * a dev wants to temporarily dump the cache contents
 * while debugging.
 *
 ****************************************************/
void opx_tid_cache_dump_entry(struct ofi_rbnode *root, struct ofi_mr_entry *entry)
{
	fprintf(stderr, "(%d) %s:%s():%d ===== Entry Node %p %s parent %p, Left %p Right %p Color %s Data %p =====\n",
		getpid(), __FILE__, __func__, __LINE__, root,
		(root->parent) ? ((root == root->parent->left) ? "is a LEFT child of" : "is a RIGHT child of") :
				 "has no",
		root->parent, root->left, root->right, root->color ? "RED" : "BLACK", entry);

	fprintf(stderr, "(%d) %s:%s():%d Key: %p-%p (%lu bytes)\n", getpid(), __FILE__, __func__, __LINE__,
		entry->info.iov.iov_base, (void *) ((uintptr_t) entry->info.iov.iov_base + entry->info.iov.iov_len),
		entry->info.iov.iov_len);

	fprintf(stderr, "(%d) %s:%s():%d Use count: %d\n", getpid(), __FILE__, __func__, __LINE__, entry->use_cnt);
	struct opx_mr_tid_info *tid_info = &((struct opx_tid_mr *) entry->data)->tid_info;

	fprintf(stderr, "(%d) %s:%s():%d Tid Info vaddr: %p-%p (%lu bytes)\n", getpid(), __FILE__, __func__, __LINE__,
		(void *) tid_info->tid_vaddr, (void *) ((uintptr_t) tid_info->tid_vaddr + tid_info->tid_length),
		tid_info->tid_length);

	fprintf(stderr, "(%d) %s:%s():%d Tid Info ninfo=%u\n", getpid(), __FILE__, __func__, __LINE__, tid_info->ninfo);
	for (int i = 0; i < tid_info->ninfo; ++i) {
		fprintf(stderr, "(%d) %s:%s():%d\tinfo[%d]=%08X\n", getpid(), __FILE__, __func__, __LINE__, i,
			tid_info->info[i]);
	}
	fprintf(stderr, "(%d) %s:%s():%d Tid Info npairs=%u\n", getpid(), __FILE__, __func__, __LINE__,
		tid_info->npairs);
	for (int i = 0; i < tid_info->npairs; ++i) {
		fprintf(stderr, "(%d) %s:%s():%d\tpairs[%d]=%08X\n", getpid(), __FILE__, __func__, __LINE__, i,
			tid_info->pairs[i]);
	}
}

void opx_tid_cache_dump_entries(struct ofi_rbnode *root, struct ofi_rbnode *sentinal)
{
	if (root == sentinal) {
		return;
	}

	opx_tid_cache_dump_entry(root, (struct ofi_mr_entry *) root->data);
	opx_tid_cache_dump_entries(root->left, sentinal);
	opx_tid_cache_dump_entries(root->right, sentinal);
}

void opx_tid_cache_dump_dlist(struct dlist_entry *dl_entry)
{
	struct ofi_mr_entry *entry;

	if (!dlist_empty(dl_entry)) {
		dlist_foreach_container (dl_entry, struct ofi_mr_entry, entry, list_entry) {
			opx_tid_cache_dump_entry(entry->node, entry);
		}
	} else {
		fprintf(stderr, "(%d) %s:%s():%d\t<Empty>\n", getpid(), __FILE__, __func__, __LINE__);
	}
}

void opx_tid_cache_dump_cache(struct fi_opx_ep *opx_ep, struct ofi_mr_cache *tid_cache)
{
	struct ofi_rbnode *root = ofi_rbmap_get_root(&tid_cache->tree);

	opx_tid_cache_dump_entries(root, &tid_cache->tree.sentinel);

	fprintf(stderr, "(%d) %s:%s():%d\t====LRU List====\n", getpid(), __FILE__, __func__, __LINE__);

	opx_tid_cache_dump_dlist(&tid_cache->lru_list);

	fprintf(stderr, "(%d) %s:%s():%d\t====Dead List====\n", getpid(), __FILE__, __func__, __LINE__);

	opx_tid_cache_dump_dlist(&tid_cache->dead_region_list);
}

/****************************************************
 * Main entry points for external callers
 * - opx_tid_cache_setup
 * - opx_deregister_for_rzv
 * - opx_register_for_rzv
 * - opx_tid_cache_flush
 ****************************************************/

/* @brief Setup the MR cache.
 *
 * This function enables the MR cache using the util MR cache code.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 */

int opx_tid_cache_setup(struct ofi_mr_cache **cache, struct opx_tid_domain *domain)
{
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cache %p, domain %p\n", *cache, domain);

	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {[FI_HMEM_SYSTEM] = default_monitor,
								 [FI_HMEM_CUDA]	  = default_cuda_monitor,
								 [FI_HMEM_ROCR]	  = default_rocr_monitor,
								 [FI_HMEM_ZE]	  = default_ze_monitor,

								 /* Not supported in OPX */
								 [FI_HMEM_NEURON]    = NULL,
								 [FI_HMEM_SYNAPSEAI] = NULL};
	int			err;

	/* Both Open MPI (and possibly other MPI implementations) and
	 * Libfabric use the same live binary patching to enable memory
	 * monitoring, but the patching technique only allows a single
	 * "winning" patch.  The Libfabric memhooks monitor will not
	 * overwrite a previous patch, but instead return
	 * -FI_EALREADY.  There are three cases of concern, and in all
	 * but one of them, we can avoid changing the default monitor.
	 *
	 * (1) Upper layer does not patch, such as Open MPI 4.0 and
	 * earlier.  In this case, the default monitor will be used,
	 * as the default monitor is either not the memhooks monitor
	 * (because the user specified a different monitor) or the
	 * default monitor is the memhooks monitor, but we were able
	 * to install the patches.  We will use the default monitor in
	 * this case.
	 *
	 * (2) Upper layer does patch, but does not export a memory
	 * monitor, such as Open MPI 4.1.0 and 4.1.1.  In this case,
	 * if the default memory monitor is not memhooks, we will use
	 * the default monitor.  If the default monitor is memhooks,
	 * the patch will fail to apply, and we will change the
	 * requested monitor to UFFD to avoid a broken configuration.
	 * If the user explicitly requested memhooks, we will return
	 * an error, as we can not satisfy that request.
	 *
	 * (3) Upper layer does patch and exports a memory monitor,
	 * such as Open MPI 4.1.2 and later.  In this case, the
	 * default monitor will have been changed from the memhooks
	 * monitor to the imported monitor, so we will use the
	 * imported monitor.
	 *
	 * The only known cases in which we will not use the default
	 * monitor are Open MPI 4.1.0/4.1.1.
	 *
	 * It is possible that this could be better handled at the
	 * mem_monitor level in Libfabric, but so far we have not
	 * reached agreement on how that would work.
	 *
	 * Copied from efa provider
	 */
	if (default_monitor == memhooks_monitor) {
		err = memhooks_monitor->start(memhooks_monitor);
		if (err == -FI_EALREADY) {
			if (cache_params.monitor) {
				FI_WARN(&fi_opx_provider, FI_LOG_MR,
					"Memhooks monitor requested via FI_MR_CACHE_MONITOR, but memhooks failed to\n"
					"install.  No working monitor availale.\n");
				return -FI_ENOSYS;
			}
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"Detected potential memhooks monitor conflict. Switching to UFFD.\n");
			memory_monitors[FI_HMEM_SYSTEM] = uffd_monitor;
		}
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "memhooks_monitor\n");
	} else if (default_monitor == NULL) {
		/* TODO: Fail if we don't find a system monitor.  This
		 * is a debatable decision, as the VERBS provider
		 * falls back to a no-cache mode in this case.  We
		 * fail the domain creation because the rest of the MR
		 * code hasn't been audited to deal with a NULL
		 * monitor.
		 */
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "No default SYSTEM monitor available.\n");
		return -FI_ENOSYS;
	} else if (default_monitor == uffd_monitor) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "uffd_monitor\n");
	} else {
		if (default_monitor == cuda_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "cuda_monitor is unsupported in opx\n");
		} else if (default_monitor == cuda_ipc_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "cuda_ipc_monitor is unsupported in opx\n");
		} else if (default_monitor == rocr_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "rocr_monitor is unsupported in opx\n");
		} else if (default_monitor == ze_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "ze_monitor is unsupported in opx\n");
		} else if (default_monitor == import_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "import_monitor enabled\n");
		} else {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "unknown monitor is unsupported in opx\n");
		}
		if (default_monitor != import_monitor) {
			return -FI_ENOSYS;
		}
	}

	*cache = (struct ofi_mr_cache *) calloc(1, sizeof(struct ofi_mr_cache));
	if (!*cache) {
		return -FI_ENOMEM;
	}

	/* Set size of an OPX entry->data :
	 * struct ofi_mr_entry *entry;
	 * struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)entry->data;
	 */
	(*cache)->entry_data_size = sizeof(struct opx_tid_mr);
	(*cache)->add_region	  = opx_tid_cache_add_abort;
	(*cache)->delete_region	  = opx_tid_cache_delete_abort;
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cache %p, domain %p\n", *cache, domain);
	/* Override env vars we don't support */
	if (!cache_params.max_cnt) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "Overriding FI_MR_CACHE_MAX_COUNT 0 to be %zu\n",
			(size_t) OPX_MAX_TID_COUNT);
		cache_params.max_cnt = OPX_MAX_TID_COUNT;
	}
	/* Default is 1024 and it is too small... but let other suspected overrides be used */
	if (cache_params.max_cnt == 1024) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "Overriding FI_MR_CACHE_MAX_COUNT default==1024 to be %zu\n",
		       (size_t) OPX_MAX_TID_COUNT);
		cache_params.max_cnt = OPX_MAX_TID_COUNT;
	}
	err = opx_tid_cache_init(&domain->util_domain, memory_monitors, *cache);
	if (err) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "OPX TID cache init failed: %s\n", fi_strerror(err));
		free(*cache);
		*cache = NULL;
		return err;
	}

	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX TID cache enabled, max_cnt: %zu max_size: %zu\n", cache_params.max_cnt,
	       cache_params.max_size);
	FI_DBG(&fi_opx_provider, FI_LOG_MR,
	       "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
	       (*cache)->cached_cnt, (*cache)->cached_size, (*cache)->uncached_cnt, (*cache)->uncached_size,
	       (*cache)->search_cnt, (*cache)->delete_cnt, (*cache)->hit_cnt, (*cache)->notify_cnt);

	return 0;
}

/* De-register (lazy) a memory region on TID rendezvous completion */
void opx_deregister_for_rzv(struct fi_opx_ep *opx_ep, const uint64_t tid_vaddr, const int64_t tid_length)
{
	struct opx_tid_domain *tid_domain = opx_ep->domain->tid_domain;
	struct ofi_mr_cache   *tid_cache  = tid_domain->tid_cache;

	struct ofi_mr_entry *entry	    = NULL;
	struct ofi_mr_info   info	    = {0};
	int		     ncache_entries = 0;
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_ENTRY tid vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
	       tid_vaddr, tid_vaddr + tid_length, tid_length, tid_length);

	pthread_mutex_lock(&mm_lock);

	uintptr_t tid_end	   = (uintptr_t) (tid_vaddr + tid_length);
	ssize_t	  remaining_length = tid_length;
	/* Just find (one page) from left to right and close */
	info.iov.iov_base = (void *) tid_vaddr;
	info.iov.iov_len  = 1;

	while (remaining_length) {
		enum opx_tid_cache_entry_status find = opx_tid_cache_find(opx_ep, &info, &entry);
		if (find == OPX_TID_CACHE_ENTRY_IN_USE) {
			/* Impossible on deregister.. no - MPICH does this.
			 * find() disabled expected receive on THIS ep but
			 * whatever ep registered the memory is still ok.
			 * We can proceed to close the region, assuming
			 * MPICH knows what it wants since close will
			 * use the ep in the registered mr. */
			static int onetime = 1;
			if (onetime) {
				OPX_TID_CACHE_DEBUG_FPRINTF(
					"## %s:%u OPX_TID_CACHE_DEBUG OPX_ENTRY_IN_USE.  Closing a region for a different endpoint\n",
					__func__, __LINE__);
			}
			onetime = 0;
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"OPX_ENTRY_IN_USE in %s.  Closing a region for a different endpoint\n", __func__);
			find = OPX_TID_CACHE_ENTRY_FOUND;
		}
		const struct opx_mr_tid_info *const found_tid_entry =
			entry ? &((struct opx_tid_mr *) entry->data)->tid_info : NULL;
		if (OFI_UNLIKELY(find == OPX_TID_CACHE_ENTRY_NOT_FOUND || !found_tid_entry)) {
			fprintf(stderr,
				"Assert find ret=%u %s : ncache_entries %u, entry %p, found_tid_entry %p, remaining_length %lu/%#lX, iov base %p, iov len %lu/%#lX\n",
				find, OPX_TID_CACHE_ENTRY_STATUS[find], ncache_entries, entry, found_tid_entry,
				remaining_length, remaining_length, info.iov.iov_base, info.iov.iov_len,
				info.iov.iov_len);
			fprintf(stderr, "Assert dereg iov [%p - %p] %lu/%#lX\n", (char *) tid_vaddr,
				(char *) (tid_vaddr) + (uint64_t) tid_length, (uint64_t) tid_length,
				(uint64_t) tid_length);
			if (found_tid_entry) {
				fprintf(stderr, "Assert found? iov [%p - %p] %lu/%#lX\n",
					(char *) found_tid_entry->tid_vaddr,
					(char *) (found_tid_entry->tid_vaddr + found_tid_entry->tid_length),
					found_tid_entry->tid_length, found_tid_entry->tid_length);
			}
			abort();
		}
		/* How much of this entry did we use (handle leading overlap) */
		uintptr_t found_entry_end = found_tid_entry->tid_vaddr + found_tid_entry->tid_length;
		ssize_t	  adj;
#ifdef OPX_TID_DEBUG_USECNT
		fprintf(stderr,
			"(%d) %s:%s():%d [%p-%p (%lu bytes)] find result=%d (%s), find_info=%p-%p (%lu bytes), found entry %p %p-%p (%lu bytes), use cnt=%d, remaining_length=%ld\n",
			getpid(), __FILE__, __func__, __LINE__, (void *) tid_vaddr, (void *) (tid_vaddr + tid_length),
			tid_length, find, OPX_TID_CACHE_ENTRY_STATUS[find], info.iov.iov_base,
			(void *) ((uintptr_t) info.iov.iov_base + info.iov.iov_len), info.iov.iov_len, entry,
			(void *) found_tid_entry->tid_vaddr, (void *) found_entry_end, found_tid_entry->tid_length,
			entry->use_cnt, remaining_length);
#endif
		if (find == OPX_TID_CACHE_ENTRY_FOUND || find == OPX_TID_CACHE_ENTRY_OVERLAP_LEFT) {
			adj		  = MIN(remaining_length, found_entry_end - (uintptr_t) info.iov.iov_base);
			info.iov.iov_base = (void *) ((uintptr_t) info.iov.iov_base + adj);
			assert(adj == remaining_length || info.iov.iov_base == (void *) found_entry_end);
		} else {
			assert(find == OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT);
			// Since we were only looking for a 1-byte long key, if we get an
			// overlap right, it must mean the entry starts at the same address
			assert(found_tid_entry->tid_vaddr == (uintptr_t) info.iov.iov_base);

			adj = MIN(tid_end, found_entry_end) - found_tid_entry->tid_vaddr;

			assert(adj > 0);
			info.iov.iov_base = (void *) ((uintptr_t) info.iov.iov_base + adj);
		}
		ncache_entries++;
		/* Force the invalidation and put it on the dead list */
		opx_tid_cache_close_region(tid_cache, entry);
		/* increment past found region for next find */
		remaining_length -= adj;
		info.iov.iov_len = MIN(remaining_length, OPX_HFI1_TID_PAGESIZE);
		assert(remaining_length >= 0);
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj, info.iov.iov_base,
		       (char *) (info.iov.iov_base) + remaining_length, remaining_length, remaining_length);
	}
	/* Flush the dead list, don't flush the lru list (false) */
	opx_tid_cache_flush(tid_cache, false);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_EXIT %u entries closed\n", ncache_entries);
	pthread_mutex_unlock(&mm_lock);
}

__OPX_FORCE_INLINE__
int opx_tid_get_tids_for_range(struct fi_opx_ep *opx_ep, struct fi_opx_hmem_iov *cur_addr_range,
			       struct opx_tid_addr_block *tid_addr_block)
{
	int ret = FI_SUCCESS;

	struct opx_tid_domain *tid_domain = opx_ep->domain->tid_domain;
	struct ofi_mr_cache   *tid_cache  = tid_domain->tid_cache;
	struct ofi_mr_entry   *entry	  = NULL;
	struct ofi_mr_info     find_info  = {0};

	struct fi_opx_hmem_iov cur_tid_range;
	opx_tid_range(cur_addr_range, &cur_tid_range);

	find_info.iov.iov_base = (void *) cur_tid_range.buf;
	find_info.iov.iov_len  = cur_tid_range.len;
	find_info.iface	       = cur_tid_range.iface;
	find_info.device       = cur_tid_range.device;

	pthread_mutex_lock(&mm_lock);
	enum opx_tid_cache_entry_status find = opx_tid_cache_find(opx_ep, &find_info, &entry);
	if (OFI_UNLIKELY(find == OPX_TID_CACHE_ENTRY_IN_USE)) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_found_entry_in_use);
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "FAILED OPX_ENTRY_IN_USE tid vaddr [%p - %p] length %lu/%#lX, find vaddr [%p - %p] length %lu/%#lX\n",
		       entry->info.iov.iov_base,
		       (void *) ((uintptr_t) entry->info.iov.iov_base + entry->info.iov.iov_len),
		       entry->info.iov.iov_len, entry->info.iov.iov_len, find_info.iov.iov_base,
		       (void *) ((uintptr_t) find_info.iov.iov_base + find_info.iov.iov_len), find_info.iov.iov_len,
		       find_info.iov.iov_len);
		ret = -FI_EPERM;
		goto register_end;
	}

	tid_addr_block->npairs = 0;

	/* Three possible cases:
	 * - Not found: Register and cache an entry
	 * - Found: Use the found entry
	 * - Overlap: Find all other entries that also overlap this range,
	 *            possibly registering/caching an entry for the first
	 *            uncovered portion of the range. Then combine all the
	 *            usable entries into a single block.
	 */
	if (find == OPX_TID_CACHE_ENTRY_NOT_FOUND) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_miss);
		assert(entry == NULL);

		/* No entry found, create it. */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_NOT_FOUND\n");
		int rc = opx_tid_register_and_cache_entry(tid_cache, &find_info, &entry, opx_ep);

		/* opx_register_tid_region was done in add region, check result */
		if (rc != FI_SUCCESS) {
			if (rc >= 0) {
				OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u return %d (%s) errno=%d (%s)\n", __func__,
							    __LINE__, rc, strerror(rc), errno, strerror(errno));
			} else {
				OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u return %d (negative error code) errno=%d (%s)\n",
							    __func__, __LINE__, rc, errno, strerror(errno));
			}
			ret = -FI_EAGAIN;
			goto register_end;
		}

		opx_tid_inc_use_cnt(entry);

		struct opx_mr_tid_info *cached_tid_entry = &((struct opx_tid_mr *) entry->data)->tid_info;
		opx_tid_set_offset_and_copy_pairs(cur_addr_range->buf, cur_addr_range->len, cached_tid_entry,
						  tid_addr_block);
	} else if (find == OPX_TID_CACHE_ENTRY_FOUND) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_hit);
		struct opx_mr_tid_info *cached_tid_entry = &((struct opx_tid_mr *) entry->data)->tid_info;
		assert(cached_tid_entry->tid_length != 0);

		/* Entry was found.  Our search is completely contained in this region */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_FOUND\n");
		opx_tid_inc_use_cnt(entry);

		opx_tid_set_offset_and_copy_pairs(cur_addr_range->buf, cur_addr_range->len, cached_tid_entry,
						  tid_addr_block);
	} else { // OVERLAP_LEFT or OVERLAP_RIGHT
		struct opx_tid_cache_chain overlap_chain;
		find = opx_tid_cache_build_overlap_chain(opx_ep, tid_cache, find_info, find, entry, &overlap_chain);

		if (OFI_UNLIKELY(find == OPX_TID_CACHE_ENTRY_IN_USE)) {
			ret = -FI_EPERM;
			goto register_end;
		}

		if (find == OPX_TID_CACHE_ENTRY_NOT_FOUND) {
			ret = -FI_EAGAIN;
			goto register_end;
		}

		assert(find == OPX_TID_CACHE_ENTRY_FOUND);
		assert(overlap_chain.entry_count > 0);

		opx_tid_cache_combine_chain_entries(&overlap_chain, cur_addr_range, tid_addr_block);
	}
	assert(tid_addr_block->target_iov.iov_base <= (void *) cur_addr_range->buf);
	assert(ofi_iov_end(&tid_addr_block->target_iov) >= (void *) cur_addr_range->buf);

register_end:
	pthread_mutex_unlock(&mm_lock);
	return ret;
}

__OPX_FORCE_INLINE__
int opx_tid_get_tids_for_initial_range(struct fi_opx_ep *opx_ep, struct fi_opx_hmem_iov *cur_addr_range,
				       struct opx_tid_addr_block *tid_addr_block)
{
	int ret = opx_tid_get_tids_for_range(opx_ep, cur_addr_range, tid_addr_block);

	// If we failed, try flushing the LRU to free up some TIDs and retry.
	if (ret == -FI_EAGAIN) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_flush_lru);
		if (opx_tid_cache_flush_all(opx_ep->domain->tid_domain->tid_cache, true, false)) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_cache_flush_lru_helped);
			ret = opx_tid_get_tids_for_range(opx_ep, cur_addr_range, tid_addr_block);
		}
	}

	return ret;
}

__OPX_FORCE_INLINE__
int opx_tid_get_tids_for_remaining_range(struct fi_opx_ep *opx_ep, struct fi_opx_hmem_iov *cur_addr_range,
					 struct opx_tid_addr_block *tid_addr_block, uintptr_t cur_addr_range_end,
					 uintptr_t target_range_end)
{
	struct fi_opx_hmem_iov next_addr_range = {.buf	  = target_range_end,
						  .len	  = cur_addr_range_end - target_range_end,
						  .iface  = cur_addr_range->iface,
						  .device = cur_addr_range->device};

	struct opx_tid_addr_block next_tid_block = {};

	int ret_and_val = -1;
	int ret;

	while (target_range_end < cur_addr_range_end &&
	       (ret = opx_tid_get_tids_for_range(opx_ep, &next_addr_range, &next_tid_block) == FI_SUCCESS)) {
		// If we got at least one FI_SUCCESS, then we want to ultimately
		// return FI_SUCCESS, even if we get a subsequent failure.
		ret_and_val = 0;

		assert(next_tid_block.offset == 0);
		assert(next_tid_block.npairs > 0);
		assert(next_tid_block.target_iov.iov_base == (void *) target_range_end);

		int pair_idx = tid_addr_block->npairs;
		for (int i = 0; i < next_tid_block.npairs; i++) {
			tid_addr_block->pairs[pair_idx++] = next_tid_block.pairs[i];
		}
		tid_addr_block->npairs = pair_idx;

		tid_addr_block->target_iov.iov_len += next_tid_block.target_iov.iov_len;

		target_range_end = ((uintptr_t) next_tid_block.target_iov.iov_base) + next_tid_block.target_iov.iov_len;

		next_addr_range.buf = target_range_end;
		next_addr_range.len = cur_addr_range_end - target_range_end;
	}

	return ret & ret_and_val;
}

int opx_register_for_rzv(struct fi_opx_ep *opx_ep, struct fi_opx_hmem_iov *cur_addr_range,
			 struct opx_tid_addr_block *tid_addr_block)
{
	uintptr_t cur_addr_range_end = cur_addr_range->buf + cur_addr_range->len;

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.reg_for_rzv_get_initial);
	int ret = opx_tid_get_tids_for_initial_range(opx_ep, cur_addr_range, tid_addr_block);

	if (ret != FI_SUCCESS) {
		return ret;
	}

	uintptr_t target_end = ((uintptr_t) tid_addr_block->target_iov.iov_base) + tid_addr_block->target_iov.iov_len;

	if (target_end >= cur_addr_range_end) {
		return FI_SUCCESS;
	}

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.reg_for_rzv_get_remaining);
	return opx_tid_get_tids_for_remaining_range(opx_ep, cur_addr_range, tid_addr_block, cur_addr_range_end,
						    target_end);
}

int opx_tid_cache_flush_all(struct ofi_mr_cache *cache, const bool flush_lru, const bool flush_all)
{
	struct dlist_entry   free_list;
	struct ofi_mr_entry *entry;

	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY (%u/%u)\n", flush_lru, flush_all);

	dlist_init(&free_list);

	pthread_mutex_lock(&mm_lock);

	/*
	 * Due to MR cache's deferred de-registration,
	 * ofi_mr_cache_notify() only moves the region
	 * entry to * the dead_region_list.
	 * Now mark the TID cache invalid (to distinguish
	 * dead from lru in the future) and move to
	 * the free list
	 *
	 */
#ifndef NDEBUG
	/* Pure debug - what's on the dead region list and is the use count 0? */
	if (dlist_empty(&cache->dead_region_list)) {
		FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY no dead\n");
	} else {
		dlist_foreach_container (&cache->dead_region_list, struct ofi_mr_entry, entry, list_entry) {
			struct opx_tid_mr *const      opx_mr   = (struct opx_tid_mr *) entry->data;
			struct opx_mr_tid_info *const tid_info = &opx_mr->tid_info;
			FI_DBG(cache->domain->prov, FI_LOG_MR,
			       "OPX_DEBUG_ENTRY dead(invalid)->free entry %p, data %p opx_domain %p, endpoint %p, TID [%p - %p] (len: %zu,%#lX) emtru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep, (char *) tid_info->tid_vaddr,
			       (char *) tid_info->tid_vaddr + tid_info->tid_length, tid_info->tid_length,
			       tid_info->tid_length, entry->info.iov.iov_base,
			       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
			       entry->info.iov.iov_len, entry->use_cnt);
			assert(entry->use_cnt == 0);
		}
	}
#endif
	/* Always free the dead list */
	dlist_splice_tail(&free_list, &cache->dead_region_list);

	/* lru is a list of regions that are still active, optionally
	 * free one, or more if the cache is full.
	 */
	bool flush_once = flush_lru;
	while ((flush_all || flush_once) && !dlist_empty(&cache->lru_list)) {
		dlist_pop_front(&cache->lru_list, struct ofi_mr_entry, entry, list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR, "(%p/%p) pop lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n", entry,
		       entry->data, entry->info.iov.iov_base,
		       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->use_cnt);
		assert(entry->use_cnt == 0);
		dlist_init(&entry->list_entry);
		opx_mr_uncache_entry_storage(cache, entry);
		dlist_insert_tail(&entry->list_entry, &free_list);

		flush_once = opx_tid_cache_full(cache);
	}

	pthread_mutex_unlock(&mm_lock);

	int freed_entries = 0;
	/* Free dead and selected lru entries */
	while (!dlist_empty(&free_list)) {
		dlist_pop_front(&free_list, struct ofi_mr_entry, entry, list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY flush free (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n", entry,
		       entry ? entry->data : NULL, entry->info.iov.iov_base,
		       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->use_cnt);
		opx_cache_free_entry(cache, entry);
		++freed_entries;
	}

	return freed_entries;
}

/* Copied from opx_tid_cache_flush
 *
 * Purge all entries for the specified endpoint from the cache,
 * ignoring use counts.
 *
 * A null endpoint is all endpoints
 */
void opx_tid_cache_purge_ep(struct ofi_mr_cache *cache, struct fi_opx_ep *opx_ep)
{
	struct dlist_entry   free_list;
	struct ofi_mr_entry *entry;
	/* Not using a cache, return */
	if (cache == NULL) {
		return;
	}
	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY (%p)\n", opx_ep);

	dlist_init(&free_list);

	pthread_mutex_lock(&mm_lock);

	/*
	 * Due to MR cache's deferred de-registration,
	 * ofi_mr_cache_notify() only moves the region
	 * entry to * the dead_region_list.
	 * Now mark the TID cache invalid (to distinguish
	 * dead from lru in the future) and move to
	 * the free list
	 *
	 */
#ifndef NDEBUG
	/* Pure debug - what's on the dead region list and is the use count 0? */
	if (dlist_empty(&cache->dead_region_list)) {
		FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY no dead\n");
	} else {
		dlist_foreach_container (&cache->dead_region_list, struct ofi_mr_entry, entry, list_entry) {
			struct opx_tid_mr *const      opx_mr   = (struct opx_tid_mr *) entry->data;
			struct opx_mr_tid_info *const tid_info = &opx_mr->tid_info;
			FI_DBG(cache->domain->prov, FI_LOG_MR,
			       "OPX_DEBUG_ENTRY dead(invalid)->free entry %p, data %p opx_domain %p, endpoint %p, TID [%p - %p] (len: %zu,%#lX) emtru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep, (char *) tid_info->tid_vaddr,
			       (char *) tid_info->tid_vaddr + tid_info->tid_length, tid_info->tid_length,
			       tid_info->tid_length, entry->info.iov.iov_base,
			       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
			       entry->info.iov.iov_len, entry->use_cnt);
			assert(entry->use_cnt == 0);
		}
	}
#endif
	dlist_splice_tail(&free_list, &cache->dead_region_list);

	/* lru is a list of regions that are still active,
	 * find entries for this endpoint and free them,
	 * ignoring the use count
	 */
	if (!dlist_empty(&cache->lru_list)) {
		/* Empty (lru) list for non-matching entries */
		struct dlist_entry copy_lru_list;
		dlist_init(&copy_lru_list);

		while (!dlist_empty(&cache->lru_list)) {
			dlist_pop_front(&cache->lru_list, struct ofi_mr_entry, entry, list_entry);
			FI_DBG(cache->domain->prov, FI_LOG_MR, "(%p/%p) pop lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, entry->data, entry->info.iov.iov_base,
			       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
			       entry->info.iov.iov_len, entry->use_cnt);
			struct opx_tid_mr *const opx_mr = (struct opx_tid_mr *) entry->data;
			if ((opx_ep == NULL) || (opx_mr->opx_ep == opx_ep)) {
				/* matching entries go on the free list */
				__attribute__((__unused__)) struct opx_mr_tid_info *const tid_info = &opx_mr->tid_info;
				if (entry->use_cnt > 0) {
					FI_WARN(cache->domain->prov, FI_LOG_MR,
						"Entry %p on endpoint %p was in use on exit\n", entry, opx_mr->opx_ep);
				}
				FI_DBG(cache->domain->prov, FI_LOG_MR,
				       "lru matching entry %p, data %p opx_domain %p, endpoint %p, TID [%p - %p] (len: %zu,%#lX) emtru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
				       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep, (char *) tid_info->tid_vaddr,
				       (char *) tid_info->tid_vaddr + tid_info->tid_length, tid_info->tid_length,
				       tid_info->tid_length, entry->info.iov.iov_base,
				       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len,
				       entry->info.iov.iov_len, entry->info.iov.iov_len, entry->use_cnt);
				dlist_init(&entry->list_entry);
				opx_mr_uncache_entry_storage(cache, entry);
				dlist_insert_tail(&entry->list_entry, &free_list);
			} else {
				/* non-matching entries go on the local list */
				FI_DBG(cache->domain->prov, FI_LOG_MR,
				       "lru non-matching entry %p, data %p opx_domain %p, endpoint %p\n", entry, opx_mr,
				       opx_mr->domain, opx_mr->opx_ep);
				dlist_init(&entry->list_entry);
				dlist_insert_tail(&entry->list_entry, &copy_lru_list);
			}
		}
		if (!dlist_empty(&copy_lru_list)) {
			dlist_init(&cache->lru_list);
			dlist_splice_tail(&cache->lru_list, &copy_lru_list);
		}
	}
	pthread_mutex_unlock(&mm_lock);

	/* Free dead and selected lru entries */
	while (!dlist_empty(&free_list)) {
		dlist_pop_front(&free_list, struct ofi_mr_entry, entry, list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY flush free (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n", entry,
		       entry ? entry->data : NULL, entry->info.iov.iov_base,
		       (char *) entry->info.iov.iov_base + entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->use_cnt);
		opx_cache_free_entry(cache, entry);
	}

	return;
}

void opx_tid_cache_cleanup(struct ofi_mr_cache *cache)
{
	/* If we don't have a domain, initialization failed */
	if (!cache->domain) {
		return;
	}

	FI_INFO(cache->domain->prov, FI_LOG_MR,
		"MR cache stats: "
		"searches %zu, deletes %zu, hits %zu notify %zu\n",
		cache->search_cnt, cache->delete_cnt, cache->hit_cnt, cache->notify_cnt);

	/* Try the nice flush */
	opx_tid_cache_flush_all(cache, true, true);

	/* Try forcing it (fini abnormal exit) for all eps (NULL) */
	opx_tid_cache_purge_ep(cache, NULL);

	pthread_mutex_destroy(&cache->lock);
	ofi_monitors_del_cache(cache);
	ofi_rbmap_cleanup(&cache->tree);
	ofi_atomic_dec32(&cache->domain->ref);
	ofi_bufpool_destroy(cache->entry_pool);
	assert(cache->cached_cnt == 0);
	assert(cache->cached_size == 0);
	assert(cache->uncached_cnt == 0);
	assert(cache->uncached_size == 0);
}

int opx_tid_cache_add_abort(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	OFI_UNUSED(cache);
	OFI_UNUSED(entry);
	fprintf(stderr, "%s unexpected function call\n", __func__);
	abort();
	return 0;
}

void opx_tid_cache_delete_abort(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	OFI_UNUSED(cache);
	OFI_UNUSED(entry);
	fprintf(stderr, "%s unexpected function call\n", __func__);
	abort();
}
