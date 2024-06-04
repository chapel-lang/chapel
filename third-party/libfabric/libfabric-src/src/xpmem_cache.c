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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ofi_xpmem.h>

#if HAVE_XPMEM
static int xpmem_cache_add_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	struct xpmem_addr *xpmem_addr = (struct xpmem_addr *) entry->info.handle;

	entry->info.mapped_addr = xpmem_attach(*xpmem_addr,
					       entry->info.iov.iov_len, NULL);
	if (entry->info.mapped_addr == (void *) -1)
		return -FI_EIO;

	return FI_SUCCESS;
}

static void xpmem_cache_delete_region(struct ofi_mr_cache *cache,
				     struct ofi_mr_entry *entry)
{
	xpmem_detach(entry->info.mapped_addr);
}

/**
 * @brief Open an xpmem cache
 *
 * @param cache[in] the xpmem cache
 * @return int 0 on success, negative value otherwise.
 */
int ofi_xpmem_cache_open(struct ofi_mr_cache **cache)
{
	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {0};
	int ret;

	memory_monitors[FI_HMEM_SYSTEM] = xpmem_monitor;

	*cache = calloc(1, sizeof(*(*cache)));
	if (!*cache) {
		ret = -FI_ENOMEM;
		goto out;
	}

	(*cache)->add_region = xpmem_cache_add_region;
	(*cache)->delete_region = xpmem_cache_delete_region;
	ret = ofi_mr_cache_init(NULL, memory_monitors,
				*cache);
	if (ret)
		goto cleanup;

	FI_INFO(&core_prov, FI_LOG_CORE,
		"xpmem cache enabled, max_cnt: %zu max_size: %zu\n",
		cache_params.max_cnt, cache_params.max_size);
	return FI_SUCCESS;

cleanup:
	free(*cache);
	*cache = NULL;
out:
	return ret;
}

/**
 * @brief Destroy the xpmem cache
 *
 * @param cache the xpmem cache
 */
void ofi_xpmem_cache_destroy(struct ofi_mr_cache *cache)
{
	ofi_mr_cache_cleanup(cache);
	free(cache);
}

/**
 * @brief Given the remote address exposed by XPMEM and the peer id,
 * search to see if we have already cached the address returned by
 * xpmem_attach(). If not, then trigger an xpmem_attach() operation (done
 * through the cache mechanism) and cache the mapped address.
 *
 * @param[in] cache the xpmem cache
 * @param[in] iov the remote address to be XPMEM attached if necessary.
 * @param[in] peer_id ID of the peer
 * @param[out] mr_entry the matched mr_entry of the XPMEM mapped_addr.
 * @return int 0 on success, negative value otherwise.
 */
int ofi_xpmem_cache_search(struct ofi_mr_cache *cache, struct iovec *iov,
			   uint64_t peer_id, struct ofi_mr_entry **mr_entry,
			   struct xpmem_client *xpmem)
{
	int ret;
	struct ofi_mr_info info;
	struct ofi_mr_entry *entry;
	struct xpmem_addr xpmem_addr;

	xpmem_addr.apid = xpmem->apid;
	xpmem_addr.offset = (uintptr_t)iov->iov_base;
	memcpy(&info.iov, iov, sizeof(*iov));
	info.iface = FI_HMEM_SYSTEM;
	info.peer_id = peer_id;

	memcpy(&info.handle, &xpmem_addr, sizeof(xpmem_addr));

	ret = ofi_mr_cache_search(cache, &info, &entry);
	if (ret)
		goto out;

	*mr_entry = entry;
out:
	return ret;
}

#else

int ofi_xpmem_cache_search(struct ofi_mr_cache *cache, struct iovec *iov,
			   uint64_t peer_id, struct ofi_mr_entry **mr_entry,
			   struct xpmem_client *xpmem)
{
	return -FI_ENOSYS;
}

int ofi_xpmem_cache_open(struct ofi_mr_cache **cache)
{
	*cache = NULL;
	return FI_SUCCESS;
}

void ofi_xpmem_cache_destroy(struct ofi_mr_cache *cache)
{
}

#endif /* HAVE_XPMEM */
