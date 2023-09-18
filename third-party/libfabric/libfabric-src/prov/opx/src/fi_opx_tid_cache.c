/*
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (C) 2022-2023 Cornelis Networks.
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
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "fi_opx_tid_cache.h"
#include "fi_opx_tid.h"

/* ofi_mr.h callback functions */
void opx_tid_cache_delete_region(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry);
int opx_tid_cache_add_region(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry);


/* @brief Setup the MR cache.
 *
 * This function enables the MR cache using the util MR cache code.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 */
int opx_setup_tid_cache(struct ofi_mr_cache **cache, struct fi_opx_tid_domain *domain)
{
	FI_DBG(&fi_opx_provider, FI_LOG_MR,"cache %p, domain %p\n", *cache, domain);

	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {
		[FI_HMEM_SYSTEM] = default_monitor,
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
		FI_WARN(&fi_opx_provider, FI_LOG_MR,
			 "No default SYSTEM monitor available.\n");
		return -FI_ENOSYS;
	} else if (default_monitor == uffd_monitor) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR, "uffd_monitor\n");
	} else {
		if (default_monitor == cuda_monitor) FI_WARN(&fi_opx_provider, FI_LOG_MR, "cuda_monitor is unsupported in opx\n");
		else if (default_monitor == cuda_ipc_monitor) FI_WARN(&fi_opx_provider, FI_LOG_MR, "cuda_ipc_monitor is unsupported in opx\n");
		else if (default_monitor == rocr_monitor) FI_WARN(&fi_opx_provider, FI_LOG_MR, "rocr_monitor is unsupported in opx\n");
		else if (default_monitor == ze_monitor) FI_WARN(&fi_opx_provider, FI_LOG_MR, "ze_monitor is unsupported in opx\n");
		else if (default_monitor == import_monitor) FI_WARN(&fi_opx_provider, FI_LOG_MR, "import_monitor enabled\n");
		else  FI_WARN(&fi_opx_provider, FI_LOG_MR, "unknown monitor is unsupported in opx\n");
		if (default_monitor != import_monitor) {
			return -FI_ENOSYS;
		}
	}

	*cache = (struct ofi_mr_cache *)calloc(1, sizeof(struct ofi_mr_cache));
	if (!*cache)
		return -FI_ENOMEM;

	/* Set size of MR entry->data
	 * struct ofi_mr_entry *entry;
	 * struct fi_opx_tid_mr *opx_mr = (struct fi_opx_tid_mr *)entry->data;
	 */
	(*cache)->entry_data_size = sizeof(struct fi_opx_tid_mr);
	(*cache)->add_region = opx_tid_cache_add_region;
	(*cache)->delete_region = opx_tid_cache_delete_region;
	FI_DBG(&fi_opx_provider, FI_LOG_MR,"cache %p, domain %p\n", *cache, domain);
	/* Override env vars we don't support */
	if(!cache_params.max_cnt) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "Overriding FI_MR_CACHE_MAX_COUNT 0 to be 1\n");
		cache_params.max_cnt = 1; /* Only 1 reuse cache currently required */
	}
	if(!cache_params.max_size){
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "Overriding FI_MR_CACHE_MAX_SIZE 0 to be 1K pages\n");
		cache_params.max_size = 4096*1024; /* arbitrary 1K pages */
	}
	err = ofi_mr_cache_init(&domain->util_domain, memory_monitors,
				*cache);
	if (err) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "OPX TID cache init failed: %s\n",
		         fi_strerror(err));
		free(*cache);
		*cache = NULL;
		return err;
	}

	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX TID cache enabled, max_cnt: %zu max_size: %zu\n",
		 cache_params.max_cnt, cache_params.max_size);
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
		(*cache)->cached_cnt      ,(*cache)->cached_size     ,(*cache)->uncached_cnt    ,(*cache)->uncached_size   ,(*cache)->search_cnt      ,(*cache)->delete_cnt      ,(*cache)->hit_cnt         ,(*cache)->notify_cnt      );

	return 0;
}

int opx_tid_cache_add_region(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry)
{
	int ret = 0;
	struct fi_opx_tid_mr *opx_mr = (struct fi_opx_tid_mr *)entry->data;
	struct fi_opx_tid_domain *opx_domain = container_of(cache->domain, struct fi_opx_tid_domain,
							util_domain);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "ENTRY cache %p, entry %p, data %p, iov_base %p, iov_len %zu\n", cache, entry, opx_mr, entry->info.iov.iov_base, entry->info.iov.iov_len);
	memset(opx_mr, 0x00, sizeof(*opx_mr));
	opx_mr->domain = opx_domain;
/* Not cacheing MR's
	struct ofi_mr_map	*  mr_map = &opx_domain->util_domain.mr_map;
	uint64_t key = (uint64_t ) entry->info.iov.iov_base;
	const struct fi_mr_attr attr = { .mr_iov = &entry->info.iov,
		.iov_count = 1,
		.requested_key = key};
	ofi_genlock_lock(&opx_domain->util_domain.lock);
	int err = ofi_mr_map_insert(mr_map, &attr,&key, NULL);
	ofi_genlock_unlock(&opx_domain->util_domain.lock);
	if (err) {
		FI_WARN(cache->domain->prov, FI_LOG_MR,
			"Unable to insert MR entry (%#lX) into util map (%d)\n", key, err);
	}
*/
	FI_DBG(cache->domain->prov, FI_LOG_MR, "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
		(cache)->cached_cnt      ,(cache)->cached_size     ,(cache)->uncached_cnt    ,(cache)->uncached_size   ,(cache)->search_cnt      ,(cache)->delete_cnt      ,(cache)->hit_cnt         ,(cache)->notify_cnt      );
	return ret;
}

void opx_tid_cache_delete_region(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry)
{
	FI_DBG(cache->domain->prov, FI_LOG_MR,"\n");
	struct fi_opx_tid_mr *opx_mr = (struct fi_opx_tid_mr *)entry->data;
	FI_DBG(cache->domain->prov, FI_LOG_MR,"fi_opx_tid_mr %p\n", opx_mr);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "opx_domain %p, endpoint %p\n", opx_mr->domain, opx_mr->ep);
	struct fi_opx_ep	*ep = opx_mr->ep;
	struct fi_opx_tid_reuse_cache *const tid_reuse_cache = ep->tid_reuse_cache;
	const void * const iov_base = entry->info.iov.iov_base;
	const size_t iov_len = entry->info.iov.iov_len;
	FI_DBG(cache->domain->prov, FI_LOG_MR, "ENTRY cache %p, entry %p, data %p, iov_base %p(%#lX), iov_len %zu(%lu)\n", cache, entry, opx_mr, iov_base, OPX_TID_VADDR(tid_reuse_cache), iov_len, OPX_TID_LENGTH(tid_reuse_cache));


	/* Is this region current?  Mark it invalid */
	if ((OPX_TID_LENGTH(tid_reuse_cache) == iov_len) && (OPX_TID_VADDR(tid_reuse_cache) ==  (uint64_t)iov_base)) {
		assert(OPX_TID_REFCOUNT(tid_reuse_cache) == 0);
		assert(!OPX_TID_IS_INVALID(tid_reuse_cache));
		OPX_TID_INVALID(tid_reuse_cache);
	}

/* Not cacheing MR's
	int err;
	struct fi_opx_tid_domain * opx_domain = opx_mr->domain;
	struct ofi_mr_map	*  mr_map = &opx_domain->util_domain.mr_map;
	uint64_t		key = opx_mr->mr_fid.key;
	ofi_genlock_lock(&opx_domain->util_domain.lock);
	err = ofi_mr_map_remove(mr_map, key);
	ofi_genlock_unlock(&opx_domain->util_domain.lock);

	if (err) {
		FI_WARN(cache->domain->prov, FI_LOG_MR,
			"Unable to remove MR entry from util map (%d)\n", err);
	}
*/
	memset(opx_mr, 0x00, sizeof(*opx_mr));
	FI_DBG(cache->domain->prov, FI_LOG_MR, "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
		(cache)->cached_cnt      ,(cache)->cached_size     ,(cache)->uncached_cnt    ,(cache)->uncached_size   ,(cache)->search_cnt      ,(cache)->delete_cnt      ,(cache)->hit_cnt         ,(cache)->notify_cnt      );
}
