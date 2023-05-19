/*
 * Copyright (C) 2022 Cornelis Networks.
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

struct fi_opx_tid_domain;

struct fi_opx_tid_mr {
//	struct id_mr		mr_fid;
	struct fi_opx_tid_domain	*domain;
//	UT_hash_handle	        hh;
//	const void		*buf;
//	size_t			len;
//	size_t			offset;
//	uint64_t		access;
//	uint64_t		flags;
//	uint64_t		cntr_bflags;
//	struct fi_opx_cntr	*cntr;
	struct fi_opx_ep	*ep;
	/* Used only in MR cache */
	struct ofi_mr_entry	*entry;
};


/* @brief Setup the MR cache.
 *
 * This function enables the MR cache using the util MR cache code.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The EFA domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 */
int opx_setup_tid_cache(struct ofi_mr_cache **cache, struct fi_opx_tid_domain *domain);

static inline int opx_tid_cache_flush(struct fi_opx_tid_domain *opx_tid_domain, bool flag)
{
	FI_DBG(&fi_opx_provider, FI_LOG_MR,"\n");
	return ofi_mr_cache_flush(opx_tid_domain->tid_cache, flag);
}
static inline int opx_tid_cache_open_region(struct fi_opx_tid_domain *opx_tid_domain,
					    const void *buf,
					    size_t len,
					    uint64_t requested_key,
					    struct fi_opx_ep *opx_ep,
					    struct fi_opx_tid_mr **p_opx_tid_mr)
{
	struct fi_opx_tid_mr *opx_tid_mr = NULL;
	int ret;
	*p_opx_tid_mr = NULL;

	FI_DBG(&fi_opx_provider, FI_LOG_MR,"buf %p, len %zu, KEY %#lX\n",buf, len, requested_key);
	assert(opx_tid_domain && buf && len && p_opx_tid_mr);

	struct ofi_mr_entry *entry = NULL;
	struct ofi_mr_info info;
	opx_tid_cache_flush(opx_tid_domain, false);

	info.iov.iov_base = (void *)buf;
	info.iov.iov_len = len;
	info.iface = FI_HMEM_SYSTEM;
	info.device = 0xDEAD; /* not used */

	/* search and return (or create) the entry.  refcount is incremented */
	do {
		ret = ofi_mr_cache_search(opx_tid_domain->tid_cache, &info, &entry);
		FI_DBG(&fi_opx_provider, FI_LOG_MR," ret %d entry %p\n",ret, entry);
		if (ret && ret != -FI_EAGAIN) {
			if (opx_tid_cache_flush(opx_tid_domain, true))
				ret = -FI_EAGAIN;
			FI_DBG(&fi_opx_provider, FI_LOG_MR," ret %d entry %p\n",ret, entry);
		}
	} while (ret == -FI_EAGAIN);

	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR,"NO ENTRY ret %d entry %p\n",ret, entry);
		return ret;
	}

	opx_tid_mr = (struct fi_opx_tid_mr *)entry->data;
	opx_tid_mr->entry = entry;
	assert(opx_tid_mr->ep == NULL || opx_tid_mr->ep == opx_ep);
	opx_tid_mr->ep = opx_ep;
	FI_DBG(&fi_opx_provider, FI_LOG_MR,"ENTRY opx_tid_mr %p, entry %p, entry->data %p, endpoint %p, use count %d, KEY %#lX\n",opx_tid_mr, entry, entry->data, opx_tid_mr->ep, entry->use_cnt, requested_key);
	assert(opx_tid_mr->ep == opx_ep);
	*p_opx_tid_mr = opx_tid_mr;
	return 0;
}

static inline int opx_tid_cache_close_region(struct fi_opx_tid_mr *opx_tid_mr)
{
	struct ofi_mr_entry	*entry = opx_tid_mr->entry;
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "ENTRY domain %p, cache %p, opx_tid_mr %p, entry %p, iov_base %p, iov_len %zd, use count %d\n", opx_tid_mr->domain, opx_tid_mr->domain->tid_cache, opx_tid_mr, entry, entry ? entry->info.iov.iov_base : NULL, entry ? entry->info.iov.iov_len : -99UL, entry ? entry->use_cnt : -99);
	ofi_mr_cache_delete(opx_tid_mr->domain->tid_cache, entry);
	return 0;
}


#endif /* _FI_PROV_OPX_TID_CACHE_H_ */
