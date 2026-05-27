/*
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (C) 2024-2024 Cornelis Networks.
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
 * The hmem cache uses the util cache/monitor code to implement a
 * ofi_mr_cache with OPX specific requirements for HMEM memory regions.
 *
 * So we've been copying and modifying and attributing code from util
 * into OPX for the time being.
 *
 */

#ifdef OPX_HMEM

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_domain.h"
#include "opx_hmem_cache.h"
#include "rdma/opx/fi_opx_hmem.h"
#include <ofi_iov.h>

/* Structure overview
 *
 * OPX has a cache of memory regions which may be used for GDRcopy
 *
 *	struct ofi_mr_cache *cache
 *
 * The OPX cache is currently attached to a HMEM domain.
 *
 *	cache = hmem_domain->hmem_cache;
 *
 * Each memory region has an entry in the cache which can be found
 *
 *	struct ofi_mr_entry *entry
 *	struct ofi_mr_info info;
 *
 * LSR TODO / TH TODO Should we be using the page aligned memory region [virtual address + length]??
 *	info.iov.iov_base = iov.iov_base;
 *	info.iov.iov_len = iov.iov_len;
 *
 * The entry data is the OPX memory region (mr)
 *
 *      struct opx_mr *opx_mr = (struct opx_mr *)entry->data;
 *
 * The memory region (mr) has info for that mr that is
 * registered and unregistered
 *
 */

#define OPX_HMEM_CACHE_MAX_COUNT 2048
#define OPX_HMEM_CACHE_MAX_SIZE	 134217728

#ifndef NDEBUG
#define OPX_DEBUG_EXIT(entryp)                                                                                         \
	do {                                                                                                           \
		const uint64_t entry_vaddr =                                                                           \
			entryp ? (uint64_t) (((struct fi_opx_mr *) (entryp)->data)->iov.iov_base) : 0UL;               \
		const uint64_t entry_length  = entryp ? ((struct fi_opx_mr *) (entryp)->data)->iov.iov_len : 0UL;      \
		const int32_t  entry_use_cnt = entryp ? ((struct ofi_mr_entry *) (entryp))->use_cnt : 0X0BAD;          \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n", \
		       entryp, entryp ? entryp->data : NULL, (void *) entry_vaddr,                                     \
		       (void *) (entry_vaddr + entry_length), entry_length, entry_length, entry_use_cnt);              \
	} while (0)
#else
#define OPX_DEBUG_EXIT(entryp)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_ENTRY(entryp)                                                                                   \
	do {                                                                                                      \
		const uint64_t entry_vaddr =                                                                      \
			entryp ? (uint64_t) (((struct fi_opx_mr *) (entryp)->data)->iov.iov_base) : 0UL;          \
		const uint64_t entry_length  = entryp ? ((struct fi_opx_mr *) (entryp)->data)->iov.iov_len : 0UL; \
		const int32_t  entry_use_cnt = entryp ? ((struct ofi_mr_entry *) (entryp))->use_cnt : 0X0BAD;     \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                                             \
		       "OPX_DEBUG_ENTRY (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n", entryp,                  \
		       entryp ? entryp->data : NULL, (void *) entry_vaddr, (void *) (entry_vaddr + entry_length), \
		       entry_length, entry_length, entry_use_cnt);                                                \
	} while (0)
#else
#define OPX_DEBUG_ENTRY(entryp)
#endif

static int opx_mr_cache_close(fid_t fid)
{
	struct fi_opx_mr    *opx_mr = (struct fi_opx_mr *) fid;
	struct ofi_mr_entry *entry  = container_of(opx_mr, struct ofi_mr_entry, data);
	ofi_mr_cache_delete(opx_mr->domain->hmem_domain->hmem_cache, entry);

	return 0;
}

static struct fi_ops opx_mr_cache_ops = {
	.size	  = sizeof(struct fi_ops),
	.close	  = opx_mr_cache_close,
	.bind	  = fi_no_bind,
	.control  = fi_no_control,
	.ops_open = fi_no_ops_open,
};

#ifndef NDEBUG
// Derived from the libfabric utility mr function util_mr_find_within
static int opx_hmem_mr_find_within(struct ofi_rbmap *map, void *key, void *data)
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

// Derived from libfabric utility mr function util_mr_find_overlap
static int opx_hmem_mr_find_overlap(struct ofi_rbmap *map, void *key, void *data)
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

	OPX_DEBUG_EXIT(entry);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_MR,
		     "OPX_DEBUG_EXIT KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		     info->iov.iov_base, (char *) (info->iov.iov_base) + info->iov.iov_len, info->iov.iov_len,
		     info->iov.iov_len, entry->info.iov.iov_base,
		     (char *) (entry->info.iov.iov_base) + entry->info.iov.iov_len, entry->info.iov.iov_len,
		     entry->info.iov.iov_len, entry->use_cnt);
	return 0;
}

/*
 * COPIED FROM ofi_mr_cache_init because it needs to be able to set the
 * compare function. util_mr_find_within checks info->peer_id and OPX
 * does not currently use this field.
 */
__OPX_FORCE_INLINE__
int opx_hmem_cache_init(struct util_domain *domain, struct ofi_mem_monitor **monitors, struct ofi_mr_cache *cache)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "GDRCOPY-CACHE-INIT");
	int ret;

	assert(cache->add_region && cache->delete_region);
	if (!cache_params.max_cnt || !cache_params.max_size) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "GDRCOPY-CACHE-INIT");
		return -FI_ENOSPC;
	}

	pthread_mutex_init(&cache->lock, NULL);
	dlist_init(&cache->lru_list);
	dlist_init(&cache->dead_region_list);
	cache->cached_cnt      = 0;
	cache->cached_size     = 0;
	cache->cached_max_cnt  = cache_params.max_cnt;
	cache->cached_max_size = cache_params.max_size;
	cache->uncached_cnt    = 0;
	cache->uncached_size   = 0;
	cache->search_cnt      = 0;
	cache->delete_cnt      = 0;
	cache->hit_cnt	       = 0;
	cache->notify_cnt      = 0;
	cache->domain	       = domain;
	cache->prov	       = &fi_opx_provider;
	ofi_atomic_inc32(&domain->ref);

#ifndef NDEBUG
	ofi_rbmap_init(&cache->tree, getenv("OPX_FIND_WITHIN") ? opx_hmem_mr_find_within : opx_hmem_mr_find_overlap);
#else
	ofi_rbmap_init(&cache->tree, opx_hmem_mr_find_overlap);
#endif
	ret = ofi_monitors_add_cache(monitors, cache);
	if (ret) {
		goto destroy;
	}

	ret = ofi_bufpool_create(&cache->entry_pool, sizeof(struct ofi_mr_entry) + cache->entry_data_size, 16, 0, 0, 0);
	if (ret) {
		goto del;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "GDRCOPY-CACHE-INIT");
	return 0;
del:
	ofi_monitors_del_cache(cache);
destroy:
	OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "GDRCOPY-CACHE-INIT");
	ofi_rbmap_cleanup(&cache->tree);
	ofi_atomic_dec32(&cache->domain->ref);
	pthread_mutex_destroy(&cache->lock);
	cache->domain = NULL;
	cache->prov   = NULL;
	return ret;
}

/****************************************************
 * Main entry points for external callers
 * - opx_hmem_cache_setup
 * - opx_hmem_cache_add_region
 * - opx_hmem_cache_delete_region
 ****************************************************/

/* @brief Setup the HMEM MR cache.
 *
 * This function enables the MR cache using the util MR cache code
 * for HMEM memory regions.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 *
 * Note: This is modeled after opx_tid_cache_setup.
 */

int opx_hmem_cache_setup(struct ofi_mr_cache **cache, struct opx_hmem_domain *domain)
{
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cache %p, domain %p\n", *cache, domain);

	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {
		[FI_HMEM_SYSTEM] = default_monitor,
		[FI_HMEM_CUDA]	 = cuda_monitor,
		[FI_HMEM_ROCR]	 = rocr_monitor,
	};
	int err;

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
	} else if (default_monitor == cuda_monitor) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "cuda_monitor\n");
	} else if (default_monitor == rocr_monitor) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "rocr_monitor\n");
	} else {
		if (default_monitor == cuda_ipc_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "cuda_ipc_monitor is unsupported in opx\n");
		} else if (default_monitor == ze_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "ze_monitor is unsupported in opx\n");
		} else if (default_monitor == rocr_ipc_monitor) {
			FI_WARN(&fi_opx_provider, FI_LOG_MR, "rocr_ipc_monitor is unsupported in opx\n");
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
	 * struct fi_opx_mr *opx_mr = (struct fi_opx_mr *)entry->data;
	 */
	(*cache)->entry_data_size = sizeof(struct fi_opx_mr);
	(*cache)->add_region	  = opx_hmem_cache_add_region;
	(*cache)->delete_region	  = opx_hmem_cache_delete_region;
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cache %p, domain %p\n", *cache, domain);
	/* Override env vars we don't support */
	if (!cache_params.max_cnt) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "Overriding FI_MR_CACHE_MAX_COUNT 0 to be %zu\n",
			(size_t) OPX_HMEM_CACHE_MAX_COUNT);
		cache_params.max_cnt = OPX_HMEM_CACHE_MAX_COUNT;
	}
	if (!cache_params.max_size) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "Overriding FI_MR_CACHE_MAX_SIZE 0 to be %zu\n",
			(size_t) OPX_HMEM_CACHE_MAX_SIZE);
		cache_params.max_size = OPX_HMEM_CACHE_MAX_SIZE;
	}

	err = opx_hmem_cache_init(&domain->util_domain, memory_monitors, *cache);
	if (err) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "OPX HMEM cache init failed: %s\n", fi_strerror(err));
		free(*cache);
		*cache = NULL;
		return err;
	}

	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX HMEM cache enabled, max_cnt: %zu max_size: %zu\n",
	       cache_params.max_cnt, cache_params.max_size);
	FI_DBG(&fi_opx_provider, FI_LOG_MR,
	       "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
	       (*cache)->cached_cnt, (*cache)->cached_size, (*cache)->uncached_cnt, (*cache)->uncached_size,
	       (*cache)->search_cnt, (*cache)->delete_cnt, (*cache)->hit_cnt, (*cache)->notify_cnt);

	return 0;
}

/* @brief Initialize the fi_opx_mr for the HMEM MR cache.
 *
 * This function initializes the MR cache entry and completes the device
 * memory registration.  It is called from add_region function pointer.
 *
 * @param cache		The ofi_mr_cache to which the entry will be added.
 * @param domain	The entry containing the MR to initialize.
 * @return 0 on success, fi_errno on failure.
 *
 * Note: efa_mr_cache_entry_reg was referenced when implementing this.
 */

int opx_hmem_cache_add_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	int		  err;
	uint64_t	  access = FI_SEND | FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE;
	struct fi_opx_mr *opx_mr = (struct fi_opx_mr *) entry->data;

	opx_mr->mr_fid.mem_desc		    = opx_mr;
	opx_mr->mr_fid.fid.fclass	    = FI_CLASS_MR;
	opx_mr->mr_fid.fid.context	    = NULL;
	opx_mr->mr_fid.fid.ops		    = &opx_mr_cache_ops;
	opx_mr->mr_fid.key		    = FI_KEY_NOTAVAIL;
	opx_mr->iov			    = entry->info.iov;
	opx_mr->attr.mr_iov		    = &opx_mr->iov;
	opx_mr->attr.iov_count		    = FI_OPX_IOV_LIMIT;
	opx_mr->attr.offset		    = 0; // set in the normal path
	opx_mr->attr.access		    = access;
	opx_mr->attr.iface		    = entry->info.iface;
	opx_mr->hmem_unified		    = 0;
	opx_mr->flags			    = entry->info.flags;
	opx_mr->attr.requested_key	    = 0;
	struct opx_hmem_domain *hmem_domain = (struct opx_hmem_domain *) cache->domain;
	opx_mr->domain			    = hmem_domain->opx_domain;
	opx_mr->base_addr = hmem_domain->opx_domain->mr_mode & FI_MR_VIRT_ADDR ? 0 : entry->info.iov.iov_base;

	assert((opx_mr->attr.iface == FI_HMEM_CUDA && cuda_is_gdrcopy_enabled()) || opx_mr->attr.iface == FI_HMEM_ROCR);

	(opx_mr->attr.iface == FI_HMEM_CUDA) ? (opx_mr->attr.device.cuda = entry->info.device) : (void) 0;

	/* FLush the cache so that if there are entries on the dead region list
	 * with the same page as we are about to register, they are unregistered first.
	 */
	ofi_mr_cache_flush(cache, false);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "HMEM-DEV-HANDLE-REGISTER");
	err = ofi_hmem_dev_register(opx_mr->attr.iface, entry->info.iov.iov_base, entry->info.iov.iov_len,
				    &opx_mr->hmem_dev_reg_handle);

	if (OFI_UNLIKELY(err)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_ERROR, "HMEM-DEV-HANDLE-REGISTER");
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"Unable to register handle for GPU memory. err: %d buf: %p len: %zu\n", err,
			entry->info.iov.iov_base, entry->info.iov.iov_len);
		// When gdrcopy pin buf failed, fallback to cudaMemcpy and return without caching
		opx_mr->hmem_dev_reg_handle = 0UL;
	} else {
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "HMEM-DEV-HANDLE-REGISTER");
	}

	opx_mr->mr_fid.mem_desc = opx_mr;

	return FI_SUCCESS;
}

/* @brief Cleanup the fi_opx_mr from the HMEM MR cache.
 *
 * This function cleans up the MR cache entry and completes the device
 * memory deregistration.  It is called from delete_region function pointer.
 *
 * @param cache		The ofi_mr_cache from which the entry will be deleted.
 * @param domain	The entry containing the MR to cleanup.
 *
 * Note: efa_mr_cache_entry_reg was referenced when implementing this.
 */

void opx_hmem_cache_delete_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	struct fi_opx_mr *opx_mr = (struct fi_opx_mr *) entry->data;

	HASH_DEL(opx_mr->domain->mr_hashmap, opx_mr);

	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY entry %p, data %p opx_domain %p\n", entry, opx_mr,
	       opx_mr->domain);
	OPX_DEBUG_ENTRY(entry);

#ifndef NDEBUG
	const void *const iov_base = entry->info.iov.iov_base;
	const size_t	  iov_len  = entry->info.iov.iov_len;
#endif
	assert(entry->use_cnt == 0);

	/* Is this region current?  deregister it */
	assert((opx_mr->iov.iov_len == iov_len) && (opx_mr->iov.iov_base == iov_base));
	FI_DBG(cache->domain->prov, FI_LOG_MR, "ENTRY cache %p, entry %p, data %p, iov_base %p, iov_len %zu\n", cache,
	       entry, opx_mr, iov_base, iov_len);
	if (opx_mr->hmem_dev_reg_handle) {
		/* Hold the cache->lock across unregister call */
		pthread_mutex_lock(&cache->lock);
		assert(opx_mr->attr.iface == FI_HMEM_CUDA || opx_mr->attr.iface == FI_HMEM_ROCR);
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "GDRCOPY-DEV-UNREGISTER");
		int err = ofi_hmem_dev_unregister(opx_mr->attr.iface, opx_mr->hmem_dev_reg_handle);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "GDRCOPY-DEV-UNREGISTER");
		pthread_mutex_unlock(&cache->lock);
		if (OFI_UNLIKELY(err)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"Unable to de-register device reg handle, hmem_dev_reg_handle=%lu\n",
				opx_mr->hmem_dev_reg_handle);
		}
	}
	opx_mr->hmem_dev_reg_handle = 0UL;

	if (opx_mr->domain->mr_mode & OFI_MR_SCALABLE) {
		int ret = fi_opx_ref_dec(&opx_mr->domain->ref_cnt, "domain");
		if (ret) {
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"Attempted to decrement reference counter when counter value was already zero, freeing opx_mr and returning error");
		}
	}

/* Intentionally setting opx_mr to non-valid value to allow easier debug of
 * an attempt to access the opx_mr after it's been deleted */
#ifndef NDEBUG
	memset(opx_mr, 0xAA, sizeof(*opx_mr));
#endif
}

#endif
