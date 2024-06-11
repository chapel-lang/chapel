/*
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.
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

#include <ofi_mr.h>
#include <ofi_util.h>
#include <ofi_hmem.h>

static int ipc_cache_add_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	int ret;

	ret = ofi_hmem_open_handle(entry->info.iface, (void **)&entry->info.handle,
				   entry->info.iov.iov_len, entry->info.device,
				   &entry->info.mapped_addr);
	if (ret == -FI_EALREADY) {
		/*
		 * There is a chance we can get the -FI_EALREADY from the
		 * ofi_hmem_open_handle call. For cuda, the case that this
		 * can happen is as follows. The sender gets handle from a
		 * block of memory. Then the sending side frees the memory.
		 * The sending side then cudaMalloc again and gets the same base
		 * address. However, it cudaMalloc a block that is larger than
		 * the one in the cache. The cache will return that memory is not
		 * found and the ofi_hmem_open_handle will be called again.
		 * However, that will fail with cudaErrorAlreadyMapped.
		 * Therefore we need to unmap all overlapping regions and retry.
		 * ofi_mr_cache_search already move all overlapping regions to
		 * the dead_region_list via `until_mr_uncache_entry`.
		 * We need to flush the cache to purge the entries
		 * and close the handles.
		 */
		ofi_mr_cache_flush(cache, false);
		ret = ofi_hmem_open_handle(entry->info.iface, (void **)&entry->info.handle,
						entry->info.iov.iov_len, entry->info.device,
						&entry->info.mapped_addr);
	}
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to open hmem handle, addr: %p, len: %lu\n",
			entry->info.iov.iov_base, entry->info.iov.iov_len);
	}
	return ret;
}

static void ipc_cache_delete_region(struct ofi_mr_cache *cache,
				     struct ofi_mr_entry *entry)
{
	ofi_hmem_close_handle(entry->info.iface,
			      entry->info.mapped_addr);
}

/**
 * @brief Open an ipc cache
 *
 * @param cache[in] the ipc cache
 * @param domain[in] the domain that the cache is attached to.
 * @param iface[in] the hmem iface of the ipc
 * @return int 0 on success, negative value otherwise.
 */
int ofi_ipc_cache_open(struct ofi_mr_cache **cache,
			struct util_domain *domain)
{
	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {0};
	int ret;

	if (!ofi_hmem_is_ipc_enabled(FI_HMEM_CUDA) &&
		!ofi_hmem_is_ipc_enabled(FI_HMEM_ROCR))
		return FI_SUCCESS;

	memory_monitors[FI_HMEM_CUDA] = cuda_ipc_monitor;
	memory_monitors[FI_HMEM_ROCR] = rocr_ipc_monitor;

	*cache = calloc(1, sizeof(*(*cache)));
	if (!*cache) {
		ret = -FI_ENOMEM;
		goto out;
	}

	(*cache)->add_region = ipc_cache_add_region;
	(*cache)->delete_region = ipc_cache_delete_region;
	ret = ofi_mr_cache_init(domain, memory_monitors,
				*cache);
	if (ret)
		goto cleanup;

	FI_INFO(&core_prov, FI_LOG_CORE,
		"ipc cache enabled, max_cnt: %zu max_size: %zu\n",
		cache_params.max_cnt, cache_params.max_size);
	return FI_SUCCESS;

cleanup:
	free(*cache);
	*cache = NULL;
out:
	return ret;
}

/**
 * @brief Destroy the ipc cache
 *
 * @param cache the ipc cache
 */
void ofi_ipc_cache_destroy(struct ofi_mr_cache *cache)
{
	ofi_mr_cache_cleanup(cache);
	free(cache);
}

/**
 * @brief Given ipc_info (with handle and the iov of the device allocation),
 * assign the mapped_addr the mapped address of the handle.
 * Each (handle, mapped_addr) pair is stored in ofi_mr_entry.info.ipc_info as
 * part of each mr entry.
 * In a cache hit, the mapped_addr is retrieved from the matched mr entry. Otherwise,
 * the mapped_addr is obtained by opening the ipc handle.
 *
 * @param[in] cache the ipc cache
 * @param[in] ipc_info the information of the ipc to be mapped.
 * @param[out] mr_entry the matched mr_entry of the ipc_info and mapped_addr.
 * @return int 0 on success, negative value otherwise.
 */
int ofi_ipc_cache_search(struct ofi_mr_cache *cache, uint64_t peer_id,
			 struct ipc_info *ipc_info,
			 struct ofi_mr_entry **mr_entry)
{
	struct ofi_mr_info info = {0};
	struct ofi_mr_entry *entry;
	int ret;
	size_t ipc_handle_size;

	info.iov.iov_base = (void *) (uintptr_t) ipc_info->base_addr;
	info.iov.iov_len = ipc_info->base_length;
	info.iface = ipc_info->iface;
	info.peer_id = peer_id;

	ipc_handle_size = ofi_hmem_get_ipc_handle_size(info.iface);
	assert(ipc_handle_size);

	memcpy(&info.handle, &ipc_info->ipc_handle, ipc_handle_size);

	ret = ofi_mr_cache_search(cache, &info, &entry);
	if (ret)
		goto out;

	*mr_entry = entry;
out:
	return ret;
}

