/*
 * Copyright (C) 2022-2024 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_TID_CACHE_H_
#define _FI_PROV_OPX_TID_CACHE_H_

#include "config.h"
#include <ofi_util.h>
#include "rdma/opx/fi_opx_tid_domain.h"
#include "fi_opx_tid.h"

/* @brief Setup the MR cache.
 *
 * This function enables the MR cache using the util MR cache code.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The EFA domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 */
int opx_tid_cache_setup(struct ofi_mr_cache **cache, struct opx_tid_domain *domain);

int  opx_tid_cache_add_abort(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);
void opx_tid_cache_delete_abort(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);

enum opx_tid_cache_entry_status {
	OPX_TID_CACHE_ENTRY_NOT_FOUND = 0,
	OPX_TID_CACHE_ENTRY_FOUND,
	OPX_TID_CACHE_ENTRY_OVERLAP_LEFT,
	OPX_TID_CACHE_ENTRY_OVERLAP_RIGHT,
	OPX_TID_CACHE_ENTRY_IN_USE,
	OPX_TID_CACHE_ENTRY_LAST
};

struct opx_tid_cache_chain {
	uint32_t	     entry_count;
	struct iovec	     range;
	struct ofi_mr_entry *entries[OPX_MAX_TID_COUNT];
};

/* Flush cache entries */
int opx_tid_cache_flush_all(struct ofi_mr_cache *cache, const bool flush_lru, const bool flush_all);

__OPX_FORCE_INLINE__
int opx_tid_cache_flush(struct ofi_mr_cache *cache, const bool flush_lru)
{
	/* Nothing to do, early exit */
	if (dlist_empty(&cache->dead_region_list) && (!flush_lru || dlist_empty(&cache->lru_list))) {
		return 0;
	}

	pthread_mutex_unlock(&mm_lock);

	/* Flush dead list and possibly one lru entry */
	int freed_entries = opx_tid_cache_flush_all(cache, flush_lru, false);

	pthread_mutex_lock(&mm_lock);
	return freed_entries;
}

/* Purge all entries for the specified endpoint */
void opx_tid_cache_purge_ep(struct ofi_mr_cache *cache, struct fi_opx_ep *opx_ep);

/* Cleanup the cache at exit/finalize */
void opx_tid_cache_cleanup(struct ofi_mr_cache *cache);

/* De-register (lazy, unless force is true) a memory region on TID rendezvous completion */
void opx_deregister_for_rzv(struct fi_opx_ep *opx_ep, const uint64_t tid_vaddr, const int64_t tid_length);

/* Register a memory region for TID rendezvous,
 * return 0 on success
 * returns non-zero on failure
 */
int opx_register_for_rzv(struct fi_opx_ep *opx_ep, struct fi_opx_hmem_iov *cur_addr_range,
			 struct opx_tid_addr_block *tid_addr_block);
#endif /* _FI_PROV_OPX_TID_CACHE_H_ */
