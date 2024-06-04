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
#include "fi_opx_tid_cache.h"
#include "fi_opx_tid.h"
#include <ofi_iov.h>

/* Structure overview
 *
 * OPX has a cache of memory regions which may be used for RZV
 *
 *      struct ofi_mr_cache *cache
 *
 * The OPX cache is currently attached to a TID domain linked to
 * the endpoint for convenience. (It will later move to endpoints)
 *
 *      cache = opx_ep->tid_domain->tid_cache;
 *
 * Each memory region has an entry in the cache which can be found
 * with the page aligned memory region [virtual address + length]
 *
 *      struct ofi_mr_entry *entry
 *
 *      struct ofi_mr_info info;
 *	info.iov.iov_base = (void *) tid_vaddr;
 *      info.iov.iov_len = tid_length;
 *
 * 	opx_tid_cache_find(opx_ep, &info, &entry, 0);
 *
 * The entry data is the OPX TID memory region (mr)
 *
 *      struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)entry->data;
 *
 * The TID memory region (mr) has TID info for that mr that is
 * registered/ioctl(update) and deregistered/ioctl(free)
 *
 *      struct opx_mr_tid_info * tid_info = &opx_mr->tid_info;
 *
 * A RZV buffer may be composed of multiple entries/mr's/tid_info's that
 * are reference counted (use_cnt) and registered/deregistered separately.
 *
 */
struct opx_mr_tid_info {
	uint64_t tid_vaddr;
	uint64_t tid_length;
	uint32_t ninfo;
	uint32_t npairs;
	uint32_t invalid; /* mmu notify */
	/* tidinfo is used on TID update/free ioctl */
	uint32_t info[FI_OPX_MAX_DPUT_TIDPAIRS];
	/* tidpairs combine CTRL 1 & 2 into CTRL 3 tidpairs for SDMA use */
	uint32_t pairs[FI_OPX_MAX_DPUT_TIDPAIRS];
};

struct opx_tid_mr {
	struct opx_tid_domain  *domain;
	struct fi_opx_ep       *opx_ep;
	struct opx_mr_tid_info  tid_info;
};


#ifndef NDEBUG
#define OPX_DEBUG_UCNT(entryp)                                                          \
	do {                                                                            \
		const uint64_t entry_vaddr =                                            \
			entryp ? ((struct opx_tid_mr *)(entryp)->data)               \
					 ->tid_info.tid_vaddr :                      \
				       0UL;                                             \
		const uint64_t entry_length =                                           \
			entryp ? ((struct opx_tid_mr *)(entryp)->data)               \
					 ->tid_info.tid_length :                     \
				       0UL;                                             \
		const int32_t entry_use_cnt =                                           \
			entryp ? ((struct ofi_mr_entry *)(entryp))->use_cnt :           \
				       0X0BAD;                                          \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                   \
		       "OPX_DEBUG_UCNT (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n", \
		       entryp, entryp ? entryp->data : NULL,                            \
		       (void *)entry_vaddr,                                             \
		       (void *)(entry_vaddr + entry_length), entry_length,              \
		       entry_length, entry_use_cnt);                                    \
	} while (0)
#else
#define OPX_DEBUG_UCNT(entryp)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_ENTRY(info)                                  \
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,                  \
	       "OPX_DEBUG_ENTRY [%p - %p] (len: %zu/%#lX)\n",  \
	       info->iov.iov_base,                             \
	       (char *)info->iov.iov_base + info->iov.iov_len, \
	       info->iov.iov_len, info->iov.iov_len)
#else
#define OPX_DEBUG_ENTRY(info)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_EXIT(entryp, ret)                                                     \
	do {                                                                            \
		const uint64_t entry_vaddr =                                            \
			entryp ? ((struct opx_tid_mr *)(entryp)->data)               \
					 ->tid_info.tid_vaddr :                      \
				       0UL;                                             \
		const uint64_t entry_length =                                           \
			entryp ? ((struct opx_tid_mr *)(entryp)->data)               \
					 ->tid_info.tid_length :                     \
				       0UL;                                             \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                   \
		       "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu,%#lX) rc %d (%s)\n", \
		       entryp, entryp ? entryp->data : NULL,                            \
		       (void *)entry_vaddr,                                             \
		       (void *)(entry_vaddr + entry_length), entry_length,              \
		       entry_length, ret,                                               \
		       ret == OPX_ENTRY_FOUND ?                                         \
				     "OPX_ENTRY_FOUND" :                                \
				     (ret == OPX_ENTRY_NOT_FOUND ?                      \
					      "OPX_ENTRY_NOT_FOUND" :                   \
				              (ret == OPX_ENTRY_OVERLAP ?               \
					               "OPX_ENTRY_OVERLAP" :            \
					               (ret == OPX_ENTRY_IN_USE ?       \
						               "OPX_ENTRY_IN_USE" :     \
						               "ERRPR"))));             \
		OPX_DEBUG_UCNT(entryp);                                                 \
	} while (0)
#else
#define OPX_DEBUG_EXIT(entryp, ret)
#endif

#ifndef NDEBUG
#define OPX_DEBUG_ENTRY2(entryp, ret)                                                    \
	do {                                                                             \
		const uint64_t entry_vaddr =                                             \
			entryp ? ((struct opx_tid_mr *)(entryp)->data)                \
					 ->tid_info.tid_vaddr :                       \
				       0UL;                                              \
		const uint64_t entry_length =                                            \
			entryp ? ((struct opx_tid_mr *)(entryp)->data)                \
					 ->tid_info.tid_length :                      \
				       0UL;                                              \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                    \
		       "OPX_DEBUG_ENTRY (%p/%p) [%p - %p] (len: %zu,%#lX) rc %d (%s)\n", \
		       entryp, entryp ? entryp->data : NULL,                             \
		       (void *)entry_vaddr,                                              \
		       (void *)(entry_vaddr + entry_length), entry_length,               \
		       entry_length, ret,                                                \
		       ret == OPX_ENTRY_FOUND ?                                          \
				     "OPX_ENTRY_FOUND" :                                 \
				     (ret == OPX_ENTRY_NOT_FOUND ?                       \
					      "OPX_ENTRY_NOT_FOUND" :                    \
					      (ret == OPX_ENTRY_OVERLAP ?                \
						       "OPX_ENTRY_OVERLAP" :             \
						       "ERRPR")));                       \
		OPX_DEBUG_UCNT(entryp);                                                  \
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
	struct ofi_mr_info *info = key;

	FI_DBG_TRACE(
	    fi_opx_global.prov, FI_LOG_MR,
	    "OPX_DEBUG_ENTRY KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX)\n",
	    info->iov.iov_base,
	    (char *)(info->iov.iov_base) + info->iov.iov_len,
	    info->iov.iov_len, info->iov.iov_len, entry->info.iov.iov_base,
	    (char *)(entry->info.iov.iov_base) + entry->info.iov.iov_len,
	    entry->info.iov.iov_len, entry->info.iov.iov_len);

	if (ofi_iov_shifted_left(&info->iov, &entry->info.iov))
		return -1;
	if (ofi_iov_shifted_right(&info->iov, &entry->info.iov))
		return 1;

	return 0;
}
#endif

void opx_regen_tidpairs(struct fi_opx_ep *opx_ep,
		   struct opx_mr_tid_info *const tid_reuse_cache)
{
	uint32_t *tidinfo = (uint32_t *)&tid_reuse_cache->info[0];
	uint32_t ntidinfo = tid_reuse_cache->ninfo;
	uint32_t *tidpairs = &tid_reuse_cache->pairs[0];
	tid_reuse_cache->npairs = 0;
	size_t accumulated_len = 0;
	int32_t tid_idx = 0, pair_idx = -1;
	unsigned int npages = 0;
	OPX_DEBUG_TIDS("Input tidinfo", ntidinfo, tidinfo);
	uint32_t tid_length = tid_reuse_cache->tid_length;
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY tid_idx %u, ntidinfo %u, accumulated_len %zu, length_pages %u\n",
	       tid_idx, ntidinfo, accumulated_len, tid_length);
	/* Combine ctrl 1/2 tids into single ctrl 3 tid pair */
	while ((tid_idx < ntidinfo) && (accumulated_len < tid_length)) {
#ifdef OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
		uint32_t len = FI_OPX_EXP_TID_GET(tidinfo[tid_idx], LEN);
		FI_OPX_DEBUG_COUNTERS_INC_COND(
			(len == 1),
			opx_ep->debug_counters.expected_receive.tid_buckets[0]);
		FI_OPX_DEBUG_COUNTERS_INC_COND(
			(len == 2),
			opx_ep->debug_counters.expected_receive.tid_buckets[1]);
		FI_OPX_DEBUG_COUNTERS_INC_COND(
			(len > 2 && len < 128),
			opx_ep->debug_counters.expected_receive.tid_buckets[2]);
		FI_OPX_DEBUG_COUNTERS_INC_COND(
			(len >= 128),
			opx_ep->debug_counters.expected_receive.tid_buckets[3]);
#endif
		size_t tid_pages = FI_OPX_EXP_TID_GET(tidinfo[tid_idx], LEN);
		size_t tid_pages_len = tid_pages * OPX_HFI1_TID_PAGESIZE;
		uint64_t tid_ctrl = FI_OPX_EXP_TID_GET(tidinfo[tid_idx], CTRL);
		/* Starts with CTRL 1 *or* it's the first entry (tid_idx == 0)
		   and starts with ONLY CTRL 2, just accumulate it, no previous
		   CTRL 1 to pair */
		if (tid_idx == 0 || tid_ctrl == 1) {
			npages += (int) tid_pages;
			accumulated_len += tid_pages_len;
			pair_idx++;
			tidpairs[pair_idx] = tidinfo[tid_idx];
		} else { /* possible CTRL 1/2 tid pair */
			assert(tid_ctrl == 2);
			npages += tid_pages;
			accumulated_len += tid_pages_len;
			if ((FI_OPX_EXP_TID_GET(tidinfo[tid_idx - 1], IDX) !=
					FI_OPX_EXP_TID_GET(tidinfo[tid_idx], IDX))
				|| (FI_OPX_EXP_TID_GET(tidinfo[tid_idx - 1], CTRL) != 1)
				|| ((FI_OPX_EXP_TID_GET(tidinfo[tid_idx - 1], LEN) +
					tid_pages) > 512)) {
				/* Can't combine into CTRL 3 if :
					- not the same IDX or
					- previous was not CTRL 1 or
					- combined LEN > 512

					Offset field (OFFSET): For expected receive packets this offset is added to the address field
					associated with the specified TID to determine a physical address. This physical address is then
					used to DMA the data portion of the received packet to system memory. If OM is 0 the 15-bit
					OFFSET can address a 128KB mapping in DW multiples. If OM is 1 the 15-bit OFFSET can address a
					2MB mapping in 64B multiples.

					512 pages is 2MB.  So even if a "tid pair" *seems* to be available, it won't work over 512 pages
					so keep ctrl 1 tid and ctrl 2 tid separate, do not optimize into ctrl 3 tidpair
					*/
				pair_idx++;
				tidpairs[pair_idx] = tidinfo[tid_idx];
			} else {
				FI_OPX_EXP_TID_RESET(tidpairs[pair_idx], CTRL, 0x3);
				int32_t len = tid_pages +
					FI_OPX_EXP_TID_GET(tidinfo[tid_idx - 1], LEN);
				FI_OPX_EXP_TID_RESET(tidpairs[pair_idx], LEN, len);
			}
		}
		tid_idx++;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "tid_idx %u, ntidinfo %u, accumulated_len %zu, tid_length %u\n",
		       tid_idx, ntidinfo, accumulated_len, tid_length);
	}
#ifdef OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
	uint32_t first_pair_len = FI_OPX_EXP_TID_GET(tidpairs[0], LEN);
	FI_OPX_DEBUG_COUNTERS_INC_COND_N(
		(opx_ep->debug_counters.expected_receive.first_tidpair_minlen ==
		 0),
		first_pair_len,
		opx_ep->debug_counters.expected_receive.first_tidpair_minlen);
	FI_OPX_DEBUG_COUNTERS_MIN_OF(
		opx_ep->debug_counters.expected_receive.first_tidpair_minlen,
		first_pair_len);
	FI_OPX_DEBUG_COUNTERS_MAX_OF(
		opx_ep->debug_counters.expected_receive.first_tidpair_maxlen,
		first_pair_len);
#endif
	tid_reuse_cache->npairs = pair_idx + 1;
	OPX_DEBUG_TIDS("Regen tidpairs", tid_reuse_cache->npairs,
		       &tid_reuse_cache->pairs[0]);
	(void) npages;
}


/* Register/TID Update (pin) the pages.
 *
 * Hold the cache->lock across registering the TIDs  */
int opx_register_tid_region(uint64_t tid_vaddr, uint64_t tid_length,
			enum fi_hmem_iface tid_iface,
			uint64_t tid_device,
			struct fi_opx_ep *opx_ep,
			struct opx_mr_tid_info *const tid_reuse_cache)
{
	uint64_t flags = (uint64_t) OPX_HMEM_KERN_MEM_TYPE[tid_iface];

	/* Parameters must be aligned for expected receive */
	assert(tid_vaddr == (tid_vaddr & -(int64_t)OPX_TID_PAGE_SIZE[tid_iface]));
	assert(tid_length == (tid_length & -(int64_t)OPX_TID_PAGE_SIZE[tid_iface]));

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&opx_ep->tid_domain->tid_cache->lock) == EBUSY);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "vaddr %p, length %lu\n",
		(void *)tid_vaddr, tid_length);

	/* TODO: Remove opx_ep - we aren't registering for an ep, it's domain-wide */
	struct _hfi_ctrl *ctx = opx_ep->hfi->ctrl;
	const uint32_t max_tidcnt = ctx->__hfi_tidexpcnt;
	assert(max_tidcnt <= OPX_MAX_TID_COUNT);

	const uint64_t max_tidlen = max_tidcnt * OPX_TID_PAGE_SIZE[tid_iface];
	if (OFI_UNLIKELY(tid_length > max_tidlen)) {
		/* This is somewhat arbitrary - if we "chunk" the TID updates we might be able
		 * to do larger buffers using multiple update calls. */
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"OPX_DEBUG_EXIT Max TID length exceeded, %lu > %lu\n",
			tid_length, max_tidlen);
		OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, "UPDATE LENGTH EXCEEDED");
		return -1;
	}

	uint32_t tidcnt = (uint32_t) (tid_length / OPX_TID_PAGE_SIZE[tid_iface]);
	if (OFI_UNLIKELY(tidcnt > max_tidcnt)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"OPX_DEBUG_EXIT Max TIDs exceeded, %u > %u\n",
			tidcnt, max_tidcnt);
		OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, "UPDATE NTIDS EXCEEDED");
		OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u OPX_TID_CACHE_DEBUG Update number of TIDs (%u) exceeded\n",
			__func__, __LINE__, tidcnt);
		return -1;
	}

	/* Eventually we might need to "chunk" updates, thus the naming here */
	uint32_t length_chunk = (uint32_t) tid_length;
	/* new (cumulative) vaddr/length of this operation*/
	uint64_t new_vaddr = tid_vaddr;
	assert((tid_reuse_cache->tid_length == 0) &&
	       (tid_reuse_cache->tid_vaddr == 0));

	uint64_t *tidlist = (uint64_t *)&tid_reuse_cache->info[0];

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY buffer range [%#lx - %#lx] length %lu %u, new range [%#lx - %#lx] length %u, tidcnt %u, tidlist %p iface %u flags %#lx\n",
	       tid_vaddr, tid_vaddr + tid_length, tid_length, length_chunk,
	       new_vaddr, new_vaddr + length_chunk, length_chunk,
	       tidcnt, tidlist, tid_iface, flags);

	if (tid_iface == FI_HMEM_CUDA) {
		int err = cuda_set_sync_memops((void *) tid_vaddr);
		if (OFI_UNLIKELY(err != 0)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"cuda_set_sync_memops(%p) FAILED (returned %d)\n",
				(void *) tid_vaddr, err);
			return -1;
		}
	}

	uint32_t tidcnt_chunk;
	/* return code is ignored in favor of length/tidcnt checks
	 * because the ioctl can "succeed" (return code 0) within
	 * resource limitations and the updated length/tidcnt will
	 * show the actual results which can be a reduction
	 * from the requested length/tidcnt. */
	opx_hfi_update_tid(
		ctx, tid_vaddr, /* input */
		&length_chunk, /* input/output*/
		(uint64_t)tidlist, /* input/output ptr cast as uint64_t */
		&tidcnt_chunk, /* output */
		flags);
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_updates);
	FI_OPX_DEBUG_COUNTERS_INC_COND(tid_iface > FI_HMEM_SYSTEM, opx_ep->debug_counters.hmem.tid_update);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "opx_hfi_update_tid return length %u, tidcnt %u\n", length_chunk,
	       tidcnt_chunk);
	/* update failed, soft (partial length update) or
	 * hard (-1 ioctl & 0 length)  or
	   soft (tidcnt 0) */
	if (OFI_UNLIKELY(((uint64_t)length_chunk < tid_length) || (tidcnt_chunk == 0))) {
		/* errors generally mean we hit the TID resource limit */
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.tid_resource_limit);
		FI_OPX_DEBUG_COUNTERS_INC_COND(length_chunk < tid_length,
			opx_ep->debug_counters.expected_receive.tid_resource_limit_length_chunk_short);
		FI_OPX_DEBUG_COUNTERS_INC_COND(tidcnt_chunk == 0,
			opx_ep->debug_counters.expected_receive.tid_resource_limit_tidcnt_chunk_zero);
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"OPX_DEBUG_EXIT opx_hfi_update_tid failed on vaddr %#lX, length %lu/%u, tidcnt %u\n",
			tid_vaddr, tid_length, length_chunk, tidcnt_chunk);
		if (tidcnt_chunk == 0) {
			/* The return length is untouched, so update it
			   for the recovery calculations below */
			length_chunk = 0;
		}
		/* flush the cache to recover resources  */
		pthread_mutex_unlock(&opx_ep->tid_domain->tid_cache->lock);
		opx_tid_cache_flush_all(opx_ep->tid_domain->tid_cache, true, true);
		pthread_mutex_lock(&opx_ep->tid_domain->tid_cache->lock);

		/* Attempt one recovery ioctl()*/
		uint32_t new_length_chunk = tid_length - length_chunk;
		uint32_t new_tidcnt_chunk = 0;
		/* Frustrating mix of uint32_t/uint64_t*/
		uint32_t *new_tidinfo = &tid_reuse_cache->info[tidcnt_chunk];
		opx_hfi_update_tid(
			ctx, (tid_vaddr + length_chunk), /* input */
			&new_length_chunk, /* input/output*/
			(uint64_t)new_tidinfo, /* input/output ptr cast as uint64_t */
			&new_tidcnt_chunk, /* output */
			flags);
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.expected_receive.tid_updates);
		FI_OPX_DEBUG_COUNTERS_INC_COND(tid_iface > FI_HMEM_SYSTEM,
			opx_ep->debug_counters.hmem.tid_update);
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "opx_hfi_update_tid return length %u, tidcnt %u\n",
		       new_length_chunk, new_tidcnt_chunk);
		if (OFI_UNLIKELY((length_chunk + new_length_chunk) < tid_length) ||
				(new_tidcnt_chunk == 0)) {
#ifdef OPX_IOCTL_DEBUG
			fprintf(stderr,
				"## FAILED RECOVERY opx_hfi_update_tid failed on vaddr %#lX, length %lu/%u, tidcnt %u\n",
				tid_vaddr, tid_length, length_chunk, tidcnt_chunk);
			fprintf(stderr,
				"## FAILED RECOVERY opx_hfi_update_tid failed on vaddr %#lX, length %lu/%u, tidcnt %u\n",
				(tid_vaddr + length_chunk),
				(OPX_TID_PAGE_SIZE[tid_iface] * tidcnt) - length_chunk,
				new_length_chunk, new_tidcnt_chunk);
#endif
			OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, "UPDATE/NEW FAILED");
			/* free first partial update, it's not useful */
			if (length_chunk) {
				OPX_FPRINTF_TIDS("Partially updated tidinfo",
						 (tidcnt_chunk + new_tidcnt_chunk),
						 &tid_reuse_cache->info[0]);
				opx_hfi_free_tid(ctx, (uint64_t)tidlist, tidcnt_chunk);
			}
			OPX_TID_CACHE_DEBUG_FPRINTF(
				"## %s:%u OPX_TID_CACHE_DEBUG (UPDATE/NEW) opx_hfi_update_tid failed on vaddr %#lX, length %lu/%u, tidcnt %u\n",
				__func__, __LINE__, tid_vaddr, tid_length, length_chunk,
				tidcnt_chunk);
			/* free second partial update, it's not useful */
			if (new_length_chunk) {
				opx_hfi_free_tid(ctx, (uint64_t)new_tidinfo, new_tidcnt_chunk);
			}
			OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u OPX_TID_CACHE_DEBUG (UPDATE/NEW) opx_hfi_update_tid failed on vaddr %#lX/%#lX, length %lu/%u, tidcnt %u\n",
				__func__, __LINE__, tid_vaddr, tid_vaddr + length_chunk, tid_length, new_length_chunk,
				new_tidcnt_chunk);
			return -1;
		}
#ifdef OPX_IOCTL_DEBUG
		else {
			fprintf(stderr,
				"## SUCCESS RECOVERY opx_hfi_update_tid on vaddr %#lX, length %lu/%u, tidcnt %u\n",
				tid_vaddr, tid_length, length_chunk, tidcnt_chunk);
			fprintf(stderr,
				"## SUCCESS RECOVERY opx_hfi_update_tid on vaddr %#lX, length %lu/%u, tidcnt %u\n",
				(tid_vaddr + length_chunk),
				(OPX_TID_PAGE_SIZE[tid_iface] * tidcnt) - length_chunk,
				new_length_chunk, new_tidcnt_chunk);
		}
#endif
		/* Successfully recovered */
		tidcnt_chunk += new_tidcnt_chunk;
		length_chunk += new_length_chunk;
		OPX_FPRINTF_TIDS("Recovered partially updated tidinfo",
						 tidcnt_chunk,
						 &tid_reuse_cache->info[0]);
	} else if (length_chunk > tid_length) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
			"opx_hfi_update_tid gave larger than requested range! requested length %lu, return length %u, tidcnt %u\n",
			tid_length, length_chunk, tidcnt_chunk);
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive
					.tid_resource_limit_length_chunk_long);
	}
	assert(tidcnt_chunk <= FI_OPX_MAX_DPUT_TIDPAIRS);
	OPX_DEBUG_TIDS("Updated tidinfo", tidcnt_chunk,
		       (&(tid_reuse_cache->info[0])));
	tid_reuse_cache->tid_vaddr = new_vaddr;
	tid_reuse_cache->tid_length = length_chunk;
	tid_reuse_cache->ninfo += tidcnt_chunk; /* appended or replaced */
	tid_reuse_cache->invalid = 0;

	OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, "UPDATE/NEW");

	/* open (maybe create/flush) and close a cache entry
	uint64_t key = new_vaddr;
	opx_tid_cache_open_region(opx_ep->tid_domain,  (void*)new_vaddr, new_length, key, opx_ep, &opx_ep->tid_mr);

	opx_tid_cache_close_region(opx_ep->tid_mr);
	*/
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_EXIT UPDATED TIDs vaddr [%#lx - %#lx] length %lu, tid vaddr [%#lx - %#lx] , tid length %lu, number of TIDs %u\n",
	       tid_vaddr,
	       tid_vaddr + tid_length, tid_length,
	       tid_reuse_cache->tid_vaddr,
	       tid_reuse_cache->tid_vaddr + tid_reuse_cache->tid_length,
	       tid_reuse_cache->tid_length, tid_reuse_cache->ninfo);

	opx_regen_tidpairs(opx_ep, tid_reuse_cache);
	return 0;
}


/* Free the tids in the cache entry
 *
 * Hold the cache->lock across deregistering the TIDs  */
void opx_deregister_tid_region(struct fi_opx_ep *opx_ep,
			  struct opx_mr_tid_info *const tid_reuse_cache)
{
	struct _hfi_ctrl *ctx = opx_ep->hfi->ctrl;
	uint32_t old_ntidinfo = tid_reuse_cache->ninfo;
	uint64_t *old_tidlist = (uint64_t *)&tid_reuse_cache->info[0];
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY vaddr %p, length %lu, opx_hfi_free_tid %u tidpairs\n",
	       (void *)tid_reuse_cache->tid_vaddr, tid_reuse_cache->tid_length,
	       old_ntidinfo);

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&opx_ep->tid_domain->tid_cache->lock) == EBUSY);

	opx_hfi_free_tid(ctx, (uint64_t)old_tidlist, old_ntidinfo);
}

/*
* COPIED FROM util_mr_find_overlap which was static and so it was
* inaccessible to OPX direct calls.
*
* Copied here because OPX prefers it over find within
*/
static int opx_util_mr_find_overlap(struct ofi_rbmap *map, void *key,
				    void *data)
{
	struct ofi_mr_entry *entry = data;
	struct ofi_mr_info *info = key;

	FI_DBG_TRACE(
	    fi_opx_global.prov, FI_LOG_MR,
	    "OPX_DEBUG_ENTRY KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
	    info->iov.iov_base,
	    (char *)(info->iov.iov_base) + info->iov.iov_len,
	    info->iov.iov_len, info->iov.iov_len, entry->info.iov.iov_base,
	    (char *)(entry->info.iov.iov_base) + entry->info.iov.iov_len,
	    entry->info.iov.iov_len, entry->info.iov.iov_len,
	    entry->use_cnt);
	if (ofi_iov_left(&info->iov, &entry->info.iov))
		return -1;
	if (ofi_iov_right(&info->iov, &entry->info.iov))
		return 1;

	OPX_DEBUG_EXIT(entry, OPX_ENTRY_FOUND);

	FI_DBG_TRACE(
	    fi_opx_global.prov, FI_LOG_MR,
	    "OPX_DEBUG_EXIT KEY [%p - %p] (len: %zu,%#lX)  ENTRY [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
	    info->iov.iov_base,
	    (char *)(info->iov.iov_base) + info->iov.iov_len,
	    info->iov.iov_len, info->iov.iov_len, entry->info.iov.iov_base,
	    (char *)(entry->info.iov.iov_base) + entry->info.iov.iov_len,
	    entry->info.iov.iov_len, entry->info.iov.iov_len,
	    entry->use_cnt);
	return 0;
}

/* Call directly instead of callback
 *
 * Hold the cache->lock across delete/deregistering the TIDs */
void opx_tid_cache_delete_region(struct ofi_mr_cache *cache,
				 struct ofi_mr_entry *entry)
{
	struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)entry->data;

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&opx_mr->opx_ep->tid_domain->tid_cache->lock) == EBUSY);

	FI_DBG(cache->domain->prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY entry %p, data %p opx_domain %p, endpoint %p\n",
	       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep);
	OPX_DEBUG_ENTRY2(entry, OPX_ENTRY_FOUND);

	struct opx_mr_tid_info *const tid_reuse_cache =
		&opx_mr->tid_info;
	struct fi_opx_ep *const opx_ep = opx_mr->opx_ep;
	const void *const iov_base = entry->info.iov.iov_base;
	const size_t iov_len = entry->info.iov.iov_len;
	assert(entry->use_cnt == 0);
	/* Is this region current?  deregister it */
	if (!tid_reuse_cache->invalid &&
	    (tid_reuse_cache->tid_length == iov_len) &&
	    (tid_reuse_cache->tid_vaddr == (uint64_t)iov_base)) {
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "ENTRY cache %p, entry %p, data %p, iov_base %p, iov_len %zu\n",
		       cache, entry, opx_mr, iov_base, iov_len);
		opx_deregister_tid_region(opx_ep, tid_reuse_cache);
	} else {
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "ENTRY OPX_TID_IS_INVALID==%u cache %p, entry %p, data %p, iov_base %p, iov_len %zu\n",
		       tid_reuse_cache->invalid, cache, entry, opx_mr, iov_base, iov_len);
	}

	memset(opx_mr, 0x00, sizeof(*opx_mr));
}

/* Cache static inlines */

__OPX_FORCE_INLINE__
int opx_tid_inc_use_cnt(struct ofi_mr_entry *entry)
{
	if (entry->use_cnt++ == 0) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR,
		       "(%p/%p) remove lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		       entry, entry->data,
		       entry->info.iov.iov_base,
		       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->use_cnt);
		dlist_remove_init(&(entry)->list_entry);
	}
	FI_DBG(&fi_opx_provider, FI_LOG_MR,
	       "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu/%#lX) use_cnt %x\n",
	       entry, entry ? entry->data : NULL, entry->info.iov.iov_base,
	       (char *)entry->info.iov.iov_base + entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->info.iov.iov_len,
	       entry->use_cnt);
	return entry->use_cnt;
}

__OPX_FORCE_INLINE__
int opx_tid_dec_use_cnt(struct ofi_mr_entry *entry)
{
	entry->use_cnt--;
	FI_DBG(&fi_opx_provider, FI_LOG_MR,
	       "OPX_DEBUG_EXIT (%p/%p) [%p - %p] (len: %zu/%#lX) use_cnt %x\n",
	       entry, entry ? entry->data : NULL, entry->info.iov.iov_base,
	       (char *)entry->info.iov.iov_base + entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->info.iov.iov_len,
	       entry->use_cnt);
	return entry->use_cnt;
}


/* Copied from ofi_mr_cache_full */
__OPX_FORCE_INLINE__
bool opx_tid_cache_full(struct ofi_mr_cache *cache)
{
	/* OPX is limited by the max number of TIDs
	 * being < cache_params.max_cnt but
	   not by size (cache_params.max_size) */
	FI_DBG(cache->domain->prov, FI_LOG_MR,
	       "cache->cached_cnt %zu, cache_params.max_cnt %zu\n",
	       cache->cached_cnt, cache_params.max_cnt);
	if (cache->cached_cnt >= cache_params.max_cnt)
	{
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
			"FULL cache->cached_cnt %zu, cache_params.max_cnt %zu, cache->cached_size %zu, cache_params.max_size %zu\n",
			cache->cached_cnt, cache_params.max_cnt,
			cache->cached_size, cache_params.max_size);
	}

	return (cache->cached_cnt >= cache_params.max_cnt);
}

/*
 * COPIED FROM ofi_mr_cache_init which was static and so it was
 * inaccessible to OPX direct calls.
 */
__OPX_FORCE_INLINE__
int opx_tid_cache_init(struct util_domain *domain,
			  struct ofi_mem_monitor **monitors,
			  struct ofi_mr_cache *cache)
{
	int ret;

	if (!cache_params.max_cnt || !cache_params.max_size)
		return -FI_ENOSPC;

	pthread_mutex_init(&cache->lock, NULL);
	dlist_init(&cache->lru_list);
	dlist_init(&cache->dead_region_list);
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

#ifndef NDEBUG
	ofi_rbmap_init(&cache->tree, getenv("OPX_FIND_WITHIN") ?
						   opx_util_mr_find_within :
						   opx_util_mr_find_overlap);
#else
	ofi_rbmap_init(&cache->tree, opx_util_mr_find_overlap);
#endif
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
struct ofi_mr_entry *opx_mr_rbt_find(struct ofi_rbmap *tree,
				     const struct ofi_mr_info *key)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_find(tree, (void *)key);
	if (!node)
		return NULL;
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
	FI_DBG(cache->domain->prov, FI_LOG_MR, "entry %p use_cnt %x\n", entry,
	       entry->use_cnt);
#ifndef NDEBUG
	/* OPX_BUF_FREE memsets to 0x3C3C3C, clear it now */
	assert((entry->use_cnt == 0) || (entry->use_cnt == 0x3c3c3c3c));
	memset(entry, 0x00, sizeof(*entry));
#endif
	pthread_mutex_unlock(&cache->lock);
	return entry;
}

/*
 * COPIED FROM util_mr_uncache_entry_storage which was static and so it was
 * inaccessible to OPX direct calls.
 */

__OPX_FORCE_INLINE__
void opx_mr_uncache_entry_storage(struct ofi_mr_cache *cache,
				     struct ofi_mr_entry *entry)
{
	OPX_DEBUG_ENTRY2(entry, OPX_ENTRY_FOUND);
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
void opx_cache_free_entry(struct ofi_mr_cache *cache,
				  struct ofi_mr_entry *entry)
{
	FI_DBG(cache->domain->prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY free (%p/%p) [%p - %p] (len: %zu/%#lX) use_cnt %x\n",
	       entry, entry ? entry->data : NULL, entry->info.iov.iov_base,
	       (char *)entry->info.iov.iov_base + entry->info.iov.iov_len,
	       entry->info.iov.iov_len, entry->info.iov.iov_len,
	       entry->use_cnt);

	assert(!entry->node);

	pthread_mutex_lock(&cache->lock);
	opx_tid_cache_delete_region(cache, entry);
	OPX_DEBUG_ENTRY((&entry->info));
	OPX_DEBUG_ENTRY2(entry, OPX_ENTRY_FOUND);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "entry %p use_cnt %x\n", entry,
	       entry->use_cnt);
	OPX_BUF_FREE(entry);
	pthread_mutex_unlock(&cache->lock);
	OPX_DEBUG_EXIT(((struct ofi_mr_entry *)NULL), OPX_ENTRY_NOT_FOUND);
	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_EXIT\n");
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
int opx_tid_cache_crte(struct ofi_mr_cache *cache,
		       const struct ofi_mr_info *info,
		       struct ofi_mr_entry **entry, struct fi_opx_ep *opx_ep)
{
	/*struct ofi_mr_entry *cur;*/
	int ret;
	// TODO - Original behavior is struct ofi_mem_monitor *monitor = cache->monitors[info->iface];
	// Hardcoding to FI_HMEM_SYSTEM
	struct ofi_mem_monitor *monitor = cache->monitors[FI_HMEM_SYSTEM];

	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&mm_lock) == EBUSY);
	assert(monitor);

	OPX_DEBUG_ENTRY(info);
	/* drop the mm lock across alloc/register */
	pthread_mutex_unlock(&mm_lock);
	*entry = opx_mr_entry_alloc(cache);
	if (!*entry) {
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY FI_NOMEM [%p - %p] (len: %zu/%#lX) \n",
		       info->iov.iov_base,
		       (char *)info->iov.iov_base + info->iov.iov_len,
		       info->iov.iov_len, info->iov.iov_len);
		/* re-acquire mm_lock */
		pthread_mutex_lock(&mm_lock);
		return -FI_ENOMEM;
	}
	assert((*entry)->use_cnt == 0);

	(*entry)->node = NULL;
	(*entry)->info = *info;
	(*entry)->use_cnt = 0;
	dlist_init(&((*entry)->list_entry));
	opx_tid_inc_use_cnt(*entry);/* traceable */

	struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)(*entry)->data;
	struct opx_tid_domain *opx_domain = container_of(
		cache->domain, struct opx_tid_domain, util_domain);
	FI_DBG(cache->domain->prov, FI_LOG_MR,
	       "ENTRY cache %p, entry %p, data %p, iov_base %p, iov_len %zu\n",
	       cache, (*entry), opx_mr, (*entry)->info.iov.iov_base,
	       (*entry)->info.iov.iov_len);
	memset(opx_mr, 0x00, sizeof(*opx_mr));
	opx_mr->domain = opx_domain;
	opx_mr->opx_ep = opx_ep; /* regions are ep specific (ioctl(fd-from-ep-context)) */

	FI_DBG(cache->domain->prov, FI_LOG_MR,
	       "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
	       (cache)->cached_cnt, (cache)->cached_size, (cache)->uncached_cnt,
	       (cache)->uncached_size, (cache)->search_cnt, (cache)->delete_cnt,
	       (cache)->hit_cnt, (cache)->notify_cnt);

	struct opx_mr_tid_info *tid_info = &opx_mr->tid_info;

	tid_info->ninfo = 0;
	tid_info->npairs = 0;
	tid_info->tid_vaddr = 0UL;
	tid_info->tid_length = 0UL;

#ifndef NDEBUG
	for (int i = 0; i < FI_OPX_MAX_DPUT_TIDPAIRS; ++i) {
		tid_info->info[i] = -1U;
		tid_info->pairs[i] = -1U;
	}
#endif

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "opx_register_tid_region vaddr %#lX, length %#lX\n",
	       (uint64_t)info->iov.iov_base, (uint64_t)info->iov.iov_len);
	/* Hold the cache->lock across registering the TIDs  */
	pthread_mutex_lock(&cache->lock);
	if (opx_register_tid_region((uint64_t)info->iov.iov_base,
				    (uint64_t)info->iov.iov_len,
				    info->iface, info->device,
				    opx_ep, tid_info)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "opx_register_tid_region failed\n");
		/* Failed, tid_info->ninfo will be zero */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "FREE node %p\n",
		       (*entry)->node);
		pthread_mutex_unlock(&cache->lock);
		/* re-acquire mm_lock */
		pthread_mutex_lock(&mm_lock);
		goto error;
	}
	pthread_mutex_unlock(&cache->lock);
	/* re-acquire mm_lock */
	pthread_mutex_lock(&mm_lock);

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "NEW vaddr [%#lx - %#lx] length %lu, tid vaddr [%#lx - %#lx] , tid length %lu\n",
	       (uint64_t)info->iov.iov_base,
	       (uint64_t)info->iov.iov_base + (uint64_t)info->iov.iov_len,
	       (uint64_t)info->iov.iov_len, tid_info->tid_vaddr,
	       tid_info->tid_vaddr + tid_info->tid_length,
	       tid_info->tid_length);

	if (opx_tid_cache_full(cache)) {
		opx_tid_cache_flush_all(cache, true, true);
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "CACHE FULL flushed\n");
	}
	if (opx_tid_cache_full(cache)) {
		cache->uncached_cnt++;
		cache->uncached_size += info->iov.iov_len;
		FI_WARN(fi_opx_global.prov, FI_LOG_MR, "CACHE FULL UNCACHED ERROR\n");
		goto error;
	} else {
		if (ofi_rbmap_insert(&cache->tree, (void *)&(*entry)->info,
				     (void *)*entry, &(*entry)->node)) {
			ret = -FI_ENOMEM;
			FI_DBG(fi_opx_global.prov, FI_LOG_MR, "FI_ENOMEM %p\n",
			       (*entry)->node);
			goto error;
		}
		cache->cached_cnt++;
		cache->cached_size += info->iov.iov_len;

		ret = ofi_monitor_subscribe(monitor, info->iov.iov_base,
					    info->iov.iov_len,
					    &(*entry)->hmem_info);
		if (ret) {
			opx_mr_uncache_entry_storage(cache, *entry);
			cache->uncached_cnt++;
			cache->uncached_size += (*entry)->info.iov.iov_len;
			FI_WARN(fi_opx_global.prov, FI_LOG_MR, "MONITOR SUBSCRIBE FAILED UNCACHED ERROR\n");
			goto error;
		}
	}
	OPX_DEBUG_EXIT((*entry), 0);
	return 0;
error:
	/* Create failed to initialize the entry but it
	   exists...caller should delete the entry */
	FI_DBG(cache->domain->prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY ERROR [%p - %p] (len: %zu/%#lX) \n",
	       info->iov.iov_base,
	       (char *)info->iov.iov_base + info->iov.iov_len,
	       info->iov.iov_len, info->iov.iov_len);
	assert((*entry)->use_cnt == 1);
	opx_tid_dec_use_cnt(*entry);/* traceable */
	tid_info->ninfo = 0; /* error == no tid pairs */
	OPX_DEBUG_EXIT((*entry), 2);
	return 0; //TODO - handle case for free
}

/* Find is similar to search but it does not increment use_cnt.
 */
__OPX_FORCE_INLINE__
int opx_tid_cache_find(struct fi_opx_ep *opx_ep,
		       const struct ofi_mr_info *info,
		       struct ofi_mr_entry **entry)
{
	int ret = -1;

	OPX_DEBUG_ENTRY(info);

	struct ofi_mr_cache *cache = opx_ep->tid_domain->tid_cache;
	cache->search_cnt++;
	*entry = opx_mr_rbt_find(&cache->tree, info);
	const struct opx_tid_mr *const opx_mr = (*entry)? (struct opx_tid_mr *)(*entry)->data : NULL;
	if (!*entry) {
		ret = OPX_ENTRY_NOT_FOUND;
	} else if (ofi_iov_within(&info->iov, &(*entry)->info.iov)) {
		if (opx_mr->opx_ep != opx_ep) {
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,"OPX_ENTRY_IN_USE %p/%p\n",opx_mr? opx_mr->opx_ep:NULL, opx_ep);
			goto in_use;
		}
		ret = OPX_ENTRY_FOUND;
	} else {
		if (opx_mr->opx_ep != opx_ep) {
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,"OPX_ENTRY_IN_USE %p/%p\n",opx_mr? opx_mr->opx_ep:NULL, opx_ep);
			goto in_use;
		}
		ret = OPX_ENTRY_OVERLAP;
	}

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,"INFO [%p - %p] (len: %zu/%#lX) ENTRY(%p/%p) ENDPOINT(mr:%p, input:%p)\n",
	       info->iov.iov_base,
	       (char *)info->iov.iov_base + info->iov.iov_len,
	       info->iov.iov_len, info->iov.iov_len,
	       (*entry), (*entry) ? (*entry)->data : NULL,
	       opx_mr? opx_mr->opx_ep:NULL, opx_ep);
	OPX_DEBUG_EXIT((*entry), ret);
	return ret;
in_use:
	/* In use by an endpoint other than the requestor */

	FI_WARN(fi_opx_global.prov, FI_LOG_MR,
		"OPX_ENTRY_IN_USE [%p - %p] (len: %zu/%#lX) ENTRY(%p/%p) ENDPOINT(mr:%p, input:%p)\n",
	       info->iov.iov_base,
	       (char *)info->iov.iov_base + info->iov.iov_len,
	       info->iov.iov_len, info->iov.iov_len,
	       (*entry), (*entry) ? (*entry)->data : NULL,
	       opx_mr? opx_mr->opx_ep:NULL, opx_ep);
	/* One-time message per endpoint.
	 * Future support - cache per endpoint
	 * and this code goes away. */
	if(opx_ep->use_expected_tid_rzv) {
		OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u OPX_TID_CACHE_DEBUG Unsupported: OPX_ENTRY_IN_USE by another endpoint. Disabling expected receive rendezvous (FI_OPX_EXPECTED_RECEIVE_ENABLE) on this endpoint\n",
					    __func__,__LINE__);
	}
	opx_ep->use_expected_tid_rzv = 0;
	ret =  OPX_ENTRY_IN_USE;
	OPX_DEBUG_EXIT((*entry), ret);
	return ret;
}

__OPX_FORCE_INLINE__
int opx_return_offset_for_new_cache_entry(
	uint64_t tid_vaddr, uint64_t tid_length,
	const struct opx_mr_tid_info *const tid_info,
	int *return_index, int *return_offset, int *return_last_index)
{
	int ret = -FI_EINVAL;
	*return_index = 0;
	*return_offset = 0;
	*return_last_index = tid_info->npairs - 1;

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "The range of the found region is [%p - %p] and number of tid pairs is %u and the length is %lu pages\n",
	       (char *)tid_info->tid_vaddr,
	       (char *)tid_info->tid_vaddr + tid_info->tid_length,
	       tid_info->npairs,
	       tid_info->tid_length / OPX_HFI1_TID_PAGESIZE);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "The range of the input region is [%p - %p] with offset %lu pages and the length is %lu pages\n",
	       (char *)tid_vaddr, (char *)tid_vaddr + tid_length,
	       (tid_vaddr - tid_info->tid_vaddr) / OPX_HFI1_TID_PAGESIZE,
	       tid_length / OPX_HFI1_TID_PAGESIZE);

	assert(tid_vaddr == (tid_vaddr & -(int64_t)OPX_HFI1_TID_PAGESIZE));

	assert(tid_info->tid_length);
	assert(tid_info->npairs);

	__attribute__((__unused__))
	const uint64_t new_entry_vaddr = tid_info->tid_vaddr;
	__attribute__((__unused__)) const uint64_t new_entry_end_vaddr =
		tid_info->tid_vaddr + tid_info->tid_length;

	assert((tid_vaddr >= new_entry_vaddr) &&
	       (tid_vaddr <= new_entry_end_vaddr));

	if ((tid_vaddr == tid_info->tid_vaddr) &&
	    (tid_length >= tid_info->tid_length)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "MATCHING SUCCES(0) return_index %u, return_offset %u, return_last_index %u\n",
		       *return_index, *return_offset, *return_last_index);
		return 0;
	}

	uint32_t leading_pages_to_walk =
		(tid_vaddr - tid_info->tid_vaddr) / OPX_HFI1_TID_PAGESIZE;

	/* Find the starting tidpair index and the offset into that tidpair for the number of pages
	 * the new regions has for a leading offset into the found region */
	for (int i = 0; i < tid_info->npairs; i++) {
		const uint32_t *const pair = &tid_info->pairs[i];
		uint32_t npages = (uint32_t)FI_OPX_EXP_TID_GET(*pair, LEN);
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "Leading pages left to walk is now %u and the tidpair[%d] is npages %u\n",
		       leading_pages_to_walk, i, npages);
		if (leading_pages_to_walk < npages) {
			*return_index = i;
			*return_offset = leading_pages_to_walk;
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,
			       "The first tidpair to use is tidpair[%d], The page offset into this tidpair is %d pages out of %d pages\n",
			       *return_index, *return_offset, npages);
			ret = 0;
			break;
		}
		leading_pages_to_walk -= npages;
	}

	/* Found the start, now find the last tidpair for the length of the new input region */
	uint32_t region_pages_to_walk =
		(tid_length / OPX_HFI1_TID_PAGESIZE) + *return_offset;
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "The value of the new region pages_to_walk is %u, total with offset %u\n",
	       (unsigned int)(tid_length / OPX_HFI1_TID_PAGESIZE),
	       region_pages_to_walk);

	for (int i = *return_index; i < tid_info->npairs; i++) {
		*return_last_index = i;
		const uint32_t *const pair = &tid_info->pairs[i];
		uint32_t npages = (uint32_t)FI_OPX_EXP_TID_GET(*pair, LEN);
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "The number of new region pages left to walk is now %u and the tidpair[%d] is npages %u\n",
		       region_pages_to_walk, i, npages);
		if (region_pages_to_walk <= npages) {
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,
			       "The last tidpair to use is tidpair[%d] \n",
			       *return_last_index);
			break;
		}
		region_pages_to_walk -= npages;
	}

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "%s(%d) return_index %u, return_offset %u, return_last_index %u\n",
	       ret ? "FAILED" : "SUCCESS", ret, *return_index, *return_offset,
	       *return_last_index);
	return ret;
}

/*
 * COPIED FROM ofi_mr_cache_delete for opx debug
 *
 * mm_lock should be held when callng this function
 */

__OPX_FORCE_INLINE__
int opx_tid_cache_close_region(struct ofi_mr_cache *tid_cache,
			       struct ofi_mr_entry *entry,
			       bool invalidate)
{
	/* TODO ... fix? */
	OPX_DEBUG_ENTRY2(entry, OPX_ENTRY_FOUND);
	/* Assert precondition that the lock is held with a trylock assert */
	assert(pthread_mutex_trylock(&mm_lock) == EBUSY);

	// Start of opx_tid_cache_delete
	FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY delete [%p - %p] (len: %zu,%#lX)\n",
	       entry->info.iov.iov_base,
	       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
	       entry->info.iov.iov_len,
	       entry->info.iov.iov_len);

	tid_cache->delete_cnt++;

#ifndef NDEBUG
	const int use_cnt = opx_tid_dec_use_cnt(entry);/* traceable */
#else
	const int use_cnt = --entry->use_cnt;
#endif
	struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)entry->data;
	struct opx_mr_tid_info *const tid_info = &opx_mr->tid_info;

	if(invalidate) {
		/* Invalidate and deregister it.
		 * Any ongoing RDMA will fail.
		 * Any new RDMA will not use it and will fallback.
		 * It will be killed and cleaned up on use_cnt 0 */
		struct fi_opx_ep *const opx_ep = opx_mr->opx_ep;

		FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "OPX_TID_IS_INVALID %u->1, (%p/%p) insert lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		       tid_info->invalid,
		       entry, entry->data,
		       entry->info.iov.iov_base,
		       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->use_cnt);

		/* drop mm_lock */
		pthread_mutex_unlock(&mm_lock);

		/* Hold the cache->lock across de-registering the TIDs  */
		pthread_mutex_lock(&tid_cache->lock);
		opx_deregister_tid_region(opx_ep, tid_info);
		tid_info->invalid = 1; /* prevent double deregister later */
		pthread_mutex_unlock(&tid_cache->lock);

		/* re-acquire mm_lock */
		pthread_mutex_lock(&mm_lock);

	}

	if (use_cnt == 0) {
		OPX_DEBUG_UCNT(entry);
		FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "invalidate %u, invalid %u, node %p, (%p/%p) insert lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		       invalidate, tid_info->invalid, entry->node,
		       entry, entry->data,
		       entry->info.iov.iov_base,
		       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->use_cnt);
		if (!entry->node) {
			tid_cache->uncached_cnt--;
			tid_cache->uncached_size -= entry->info.iov.iov_len;
			pthread_mutex_unlock(&mm_lock);
			opx_cache_free_entry(tid_cache, entry);
			/* re-acquire mm_lock */
			pthread_mutex_lock(&mm_lock);
			return 0;
		}
		if(tid_info->invalid) { /* it's dead, not just "least recently used */
			FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "DEAD entry %p\n",entry);
			opx_mr_uncache_entry_storage(tid_cache, entry);
			dlist_insert_tail(&entry->list_entry, &tid_cache->dead_region_list);
		}
		else {
			FI_DBG(tid_cache->domain->prov, FI_LOG_MR, "LRU entry %p\n",entry);
			dlist_insert_tail(&entry->list_entry, &tid_cache->lru_list);
		}
	}
	OPX_DEBUG_UCNT(entry);

	return 0;
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

int opx_tid_cache_setup(struct ofi_mr_cache **cache,
			struct opx_tid_domain *domain)
{
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cache %p, domain %p\n", *cache,
	       domain);

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
		if (default_monitor == cuda_monitor)
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"cuda_monitor is unsupported in opx\n");
		else if (default_monitor == cuda_ipc_monitor)
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"cuda_ipc_monitor is unsupported in opx\n");
		else if (default_monitor == rocr_monitor)
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"rocr_monitor is unsupported in opx\n");
		else if (default_monitor == ze_monitor)
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"ze_monitor is unsupported in opx\n");
		else if (default_monitor == import_monitor)
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"import_monitor enabled\n");
		else
			FI_WARN(&fi_opx_provider, FI_LOG_MR,
				"unknown monitor is unsupported in opx\n");
		if (default_monitor != import_monitor) {
			return -FI_ENOSYS;
		}
	}

	*cache = (struct ofi_mr_cache *)calloc(1, sizeof(struct ofi_mr_cache));
	if (!*cache)
		return -FI_ENOMEM;

	/* Set size of an OPX entry->data :
	 * struct ofi_mr_entry *entry;
	 * struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)entry->data;
	 */
	(*cache)->entry_data_size = sizeof(struct opx_tid_mr);
	(*cache)->add_region = opx_tid_cache_add_abort;
	(*cache)->delete_region = opx_tid_cache_delete_abort;
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "cache %p, domain %p\n", *cache,
	       domain);
	/* Override env vars we don't support */
	if (!cache_params.max_cnt) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR,
			"Overriding FI_MR_CACHE_MAX_COUNT 0 to be %zu\n",(size_t)OPX_MAX_TID_COUNT);
		cache_params.max_cnt = OPX_MAX_TID_COUNT;
	}
	/* Default is 1024 and it is too small... but let other suspected overrides be used */
	if (cache_params.max_cnt == 1024) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR,
			"Overriding FI_MR_CACHE_MAX_COUNT default==1024 to be %zu\n",(size_t)OPX_MAX_TID_COUNT);
		cache_params.max_cnt = OPX_MAX_TID_COUNT;
	}
	err = opx_tid_cache_init(&domain->util_domain, memory_monitors,
				    *cache);
	if (err) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR,
			"OPX TID cache init failed: %s\n", fi_strerror(err));
		free(*cache);
		*cache = NULL;
		return err;
	}

	FI_DBG(&fi_opx_provider, FI_LOG_MR,
	       "OPX TID cache enabled, max_cnt: %zu max_size: %zu\n",
	       cache_params.max_cnt, cache_params.max_size);
	FI_DBG(&fi_opx_provider, FI_LOG_MR,
	       "cached_cnt    %zu, cached_size   %zu, uncached_cnt  %zu, uncached_size %zu, search_cnt    %zu, delete_cnt    %zu, hit_cnt       %zu, notify_cnt    %zu\n",
	       (*cache)->cached_cnt, (*cache)->cached_size,
	       (*cache)->uncached_cnt, (*cache)->uncached_size,
	       (*cache)->search_cnt, (*cache)->delete_cnt, (*cache)->hit_cnt,
	       (*cache)->notify_cnt);

	return 0;
}

/* De-register (lazy, unless invalidate is true) a memory region on TID rendezvous completion */
void opx_deregister_for_rzv(struct fi_opx_ep *opx_ep, const uint64_t tid_vaddr,
			    const int64_t tid_length, bool invalidate)
{
	struct opx_tid_domain *tid_domain = opx_ep->domain->tid_domain;
	struct ofi_mr_cache *tid_cache = tid_domain->tid_cache;

	struct ofi_mr_entry *entry = NULL;
	struct ofi_mr_info info = {0};
	int ncache_entries = 0;
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY tid vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
	       tid_vaddr, tid_vaddr + tid_length, tid_length, tid_length);

	pthread_mutex_lock(&mm_lock);

	int64_t remaining_length = tid_length;
	/* Just find (one page) from left to right and close */
	info.iov.iov_base = (void *)tid_vaddr;
	info.iov.iov_len = OPX_HFI1_TID_PAGESIZE;

	while (remaining_length) {
		int find = opx_tid_cache_find(opx_ep, &info, &entry);
		if (find == OPX_ENTRY_IN_USE) {
			/* Impossible on deregister.. no - MPICH does this.
			 * find() disabled expected receive on THIS ep but
			 * whatever ep registered the memory is still ok.
			 * We can proceed to close the region, assuming
			 * MPICH knows what it wants since close will
			 * use the ep in the registered mr. */
			static int onetime = 1;
			if(onetime){
				OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u OPX_TID_CACHE_DEBUG OPX_ENTRY_IN_USE.  Closing a region for a different endpoint\n",__func__, __LINE__);
			}
			onetime = 0;
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"OPX_ENTRY_IN_USE in %s.  Closing a region for a different endpoint\n",
				__func__);
			find = OPX_ENTRY_FOUND;
		}
		const struct opx_mr_tid_info *const found_tid_entry =
			entry ? &((struct opx_tid_mr *)entry->data)
					 ->tid_info :
				      NULL;
		if (find != OPX_ENTRY_FOUND || !found_tid_entry) {
			fprintf(stderr,
				"Assert find ret=%u %s : ncache_entries %u, entry %p, found_tid_entry %p, remaining_length %lu/%#lX, iov base %p, iov len %lu/%#lX\n",
				find,
				find == OPX_ENTRY_FOUND ?
					      "OPX_ENTRY_FOUND" :
					      (find == OPX_ENTRY_NOT_FOUND ?
						       "OPX_ENTRY_NOT_FOUND" :
						       (find == OPX_ENTRY_OVERLAP ?
								"OPX_ENTRY_OVERLAP" :
								"ERRPR")),
				ncache_entries, entry, found_tid_entry,
				remaining_length, remaining_length,
				info.iov.iov_base, info.iov.iov_len,
				info.iov.iov_len);
			fprintf(stderr, "Assert dereg iov [%p - %p] %lu/%#lX\n",
				(char *)tid_vaddr,
				(char *)(tid_vaddr) + (uint64_t)tid_length,
				(uint64_t)tid_length, (uint64_t)tid_length);
			if (found_tid_entry)
				fprintf(stderr,
					"Assert found? iov [%p - %p] %lu/%#lX\n",
					(char *)found_tid_entry->tid_vaddr,
					(char *)(found_tid_entry->tid_vaddr +
						 found_tid_entry->tid_length),
					found_tid_entry->tid_length,
					found_tid_entry->tid_length);
			abort();
		}
		/* How much of this entry did we use (handle leading overlap) */
		const uint64_t adj =
			MIN(remaining_length, ((found_tid_entry->tid_vaddr +
						found_tid_entry->tid_length) -
					       (uint64_t)info.iov.iov_base));
		ncache_entries++;
		/* Force the invalidation and put it on the dead list */
		opx_tid_cache_close_region(tid_cache, entry, invalidate);
		/* increment past found region for next find */
		remaining_length -= adj;
		info.iov.iov_base = (char *)info.iov.iov_base + adj;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj,
		       info.iov.iov_base,
		       (char *)(info.iov.iov_base) + remaining_length,
		       remaining_length, remaining_length);
	}
	/* Flush the dead list, don't flush the lru list (false) */
	opx_tid_cache_flush(tid_cache, false);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_EXIT %u entries closed\n",
	       ncache_entries);
	pthread_mutex_unlock(&mm_lock);
}

/* opx_process_entry()
 *
 *  Process the entry found in a region
 *  for hole or overlap.
 *
 *  Input:
 *       tid_cache being used
 *       find return code
 *       info used on find by caller
 *       entry found by caller
 *       vaddr of remaining region
 *       length of remaining region
 *
 * returns:
 *
 *  Updated vaddr/length with next entry to find.
 *
 *  OPX_ENTRY_NOT_FOUND - create entry
 *      Update info with entry to be created.
 *
 *  OPX_ENTRY_FOUND     - use entry (overlap)
 *      Update entry with actual entry to use
 */
int opx_process_entry(struct fi_opx_ep *opx_ep, int find,
		      struct ofi_mr_info *inout_info,
		      struct ofi_mr_entry **inout_entry, uint64_t *vaddr,
		      int64_t *length)
{
	struct opx_mr_tid_info *input_tid_info = (*inout_entry) ?
			      &((struct opx_tid_mr *)(*inout_entry)->data)->tid_info : NULL;

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "rc %u(%s), input info [%p - %p]  %lu/%#lX, input entry [%p - %p]  %lu/%#lX, remaining vaddr [%p - %p] %lu/%#lX\n",
	       find,
	       find == OPX_ENTRY_FOUND ? "OPX_ENTRY_FOUND" :
					       (find == OPX_ENTRY_NOT_FOUND ?
							"OPX_ENTRY_NOT_FOUND" :
							(find == OPX_ENTRY_OVERLAP ?
								 "OPX_ENTRY_OVERLAP" :
								 "ERROR")),
	       (char *)inout_info->iov.iov_base,
	       (char *)inout_info->iov.iov_base + inout_info->iov.iov_len,
	       inout_info->iov.iov_len, inout_info->iov.iov_len,
	       input_tid_info ? (char *)input_tid_info->tid_vaddr : NULL,
	       input_tid_info ? (char *)input_tid_info->tid_vaddr +
					 input_tid_info->tid_length :
				       NULL,
	       input_tid_info ? input_tid_info->tid_length : 0UL,
	       input_tid_info ? input_tid_info->tid_length : 0UL,
	       (void *)(*vaddr), (void *)((*vaddr) + *length), *length,
	       *length);

	assert(inout_info->iov.iov_base == (void *)(*vaddr));
	assert(inout_info->iov.iov_len == *length);

	/* NOT FOUND mean no more overlap so create the
	 * the remaining region by using the dummy
	 * storage to pretend to find overlap past
	 * our region */
	if (find == OPX_ENTRY_NOT_FOUND || !input_tid_info) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_NOT_FOUND\n");
		const uint64_t adj = *length;
		*vaddr += adj;
		*length -= adj;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj,
		       (char *)*vaddr, (char *)(*vaddr) + *length, *length,
		       *length);
		OPX_DEBUG_EXIT(((struct ofi_mr_entry *)NULL),
			       OPX_ENTRY_NOT_FOUND);
		return OPX_ENTRY_NOT_FOUND;
	}
	/* process the found or overlap region */
	assert(input_tid_info);
	assert(input_tid_info->tid_length != 0);

	/* FOUND entry (completely in region) so use it */
	if (find == OPX_ENTRY_FOUND) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_FOUND\n");
		assert(inout_info->iov.iov_base >=
		       (void *)(input_tid_info->tid_vaddr));
		assert(inout_info->iov.iov_len <= input_tid_info->tid_length);
		/*
		"Found" means it's contained in the region, so if
		the region is  0xf5000-0xf8000, then ranges
		0xf6000-0xf7000, 0xf6000-0xf8000, 0xf7000-0xf8000
		are "Found".

		BUT the way we use opx_process_next_entry, I
		believe it will always be at the head of the
		region so I'm asserting.

		If I'm wrong we switch to :
		const uint64_t adj =
			MIN(*length, ((found_tid_entry->tid_vaddr +
				       found_tid_entry->tid_length) -
				      *vaddr));

		*/
		assert(inout_info->iov.iov_base == (void *)(input_tid_info->tid_vaddr));
		assert(inout_info->iov.iov_base == (void *)*vaddr);
		const uint64_t adj = *length;

		*vaddr += adj;
		*length -= adj;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj,
		       (char *)*vaddr, (char *)(*vaddr) + *length, *length,
		       *length);
		/* entry was correct */
		OPX_DEBUG_EXIT(((struct ofi_mr_entry *)NULL),
			       OPX_ENTRY_NOT_FOUND);
		return OPX_ENTRY_FOUND;
	}

	/* Overlap*/
	assert(find == OPX_ENTRY_OVERLAP);

	/* Leading overlap, use it */
	if (input_tid_info->tid_vaddr <= *vaddr) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "LEADING OPX_ENTRY_OVERLAP\n");
		const uint64_t adj = MIN(*length,
					 ((input_tid_info->tid_vaddr + input_tid_info->tid_length) - *vaddr));
		*length -= adj;
		*vaddr += adj;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj,
		       (char *)*vaddr, (char *)(*vaddr) + *length, *length,
		       *length);
		/* entry was correct */
		OPX_DEBUG_EXIT((*inout_entry), OPX_ENTRY_FOUND);
		return OPX_ENTRY_FOUND;
	}

	/* Possible overlap OR hole */

	struct ofi_mr_entry *find_new_entry = NULL;
	struct ofi_mr_info find_new_entry_info = {0};
	struct opx_mr_tid_info *find_tid_info;

	/* Loop and do smaller finds for real left-most hole/overlap */
	find_new_entry_info.iov.iov_base = (void *)(*vaddr);
	find_new_entry_info.iov.iov_len = (uint64_t)input_tid_info->tid_vaddr - *vaddr;

	do {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "info [%p - %p]  %lu/%#lX, entry [%p - %p] %lu/%#lX, find_info [%p - %p] %lu/%#lX, vaddr [%p - %p] %lu/%#lX\n",
		       (char *)inout_info->iov.iov_base,
		       (char *)inout_info->iov.iov_base + inout_info->iov.iov_len,
		       inout_info->iov.iov_len, inout_info->iov.iov_len,
		       input_tid_info ? (char *)input_tid_info->tid_vaddr : NULL,
		       input_tid_info ? (char *)input_tid_info->tid_vaddr +
						 input_tid_info->tid_length :
					       NULL,
		       input_tid_info ? input_tid_info->tid_length : 0UL,
		       input_tid_info ? input_tid_info->tid_length : 0UL,
		       (char*)find_new_entry_info.iov.iov_base, (char*)find_new_entry_info.iov.iov_base + find_new_entry_info.iov.iov_len,
		       find_new_entry_info.iov.iov_len, find_new_entry_info.iov.iov_len,
		       (void *)(*vaddr), (void *)((*vaddr) + *length), *length,
		       *length);
		find = opx_tid_cache_find(opx_ep, &find_new_entry_info,
					  &find_new_entry);
		if (find == OPX_ENTRY_IN_USE) {
			return OPX_ENTRY_IN_USE;
		}
		find_tid_info =
			find_new_entry ? &((struct opx_tid_mr *)find_new_entry->data)
						  ->tid_info :
					       NULL;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "info [%p - %p]  %lu/%#lX, entry [%p - %p] %lu/%#lX, find_info [%p - %p] %lu/%#lX, find_tid_info [%p - %p] %lu/%#lX, vaddr [%p - %p] %lu/%#lX\n",
		       (char *)inout_info->iov.iov_base,
		       (char *)inout_info->iov.iov_base + inout_info->iov.iov_len,
		       inout_info->iov.iov_len, inout_info->iov.iov_len,
		       input_tid_info ? (char *)input_tid_info->tid_vaddr : NULL,
		       input_tid_info ? (char *)input_tid_info->tid_vaddr +
						 input_tid_info->tid_length :
					       NULL,
		       input_tid_info ? input_tid_info->tid_length : 0UL,
		       input_tid_info ? input_tid_info->tid_length : 0UL,
		       (char*)find_new_entry_info.iov.iov_base, (char*)find_new_entry_info.iov.iov_base + find_new_entry_info.iov.iov_len,
		       find_new_entry_info.iov.iov_len, find_new_entry_info.iov.iov_len,
		       find_tid_info ? (char *)find_tid_info->tid_vaddr : NULL,
		       find_tid_info ? (char *)find_tid_info->tid_vaddr +
						  find_tid_info->tid_length :
						NULL,
		       find_tid_info ? find_tid_info->tid_length : 0UL,
		       find_tid_info ? find_tid_info->tid_length : 0UL,
		       (void *)(*vaddr), (void *)((*vaddr) + *length), *length,
		       *length);

		/* Real (left-most) hole */
		if (find == OPX_ENTRY_NOT_FOUND || !find_tid_info) {
			assert(find_new_entry_info.iov.iov_base == (void *)(*vaddr));
			inout_info->iov.iov_len = find_new_entry_info.iov.iov_len;
			if(!(find_new_entry_info.iov.iov_len < *length)) {
				FI_WARN(fi_opx_global.prov, FI_LOG_MR,"iov_len %zu, *length %lu\n",
					find_new_entry_info.iov.iov_len, *length);
			}
			const uint64_t adj = find_new_entry_info.iov.iov_len;
			*vaddr += adj;
			*length -= adj;
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,
			       "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj,
			       (char *)*vaddr, (char *)(*vaddr) + *length, *length,
			       *length);
			OPX_DEBUG_EXIT(((struct ofi_mr_entry *)NULL),
				       OPX_ENTRY_NOT_FOUND);
			return OPX_ENTRY_NOT_FOUND;
		}
		assert((find == OPX_ENTRY_OVERLAP) || (find == OPX_ENTRY_FOUND));
		assert(find_tid_info);
		/* Real (left-most) overlap starting at iov_base */
		if ((uint64_t)find_tid_info->tid_vaddr <= *vaddr) {
			const uint64_t adj = MIN(*length,
						 ((find_tid_info->tid_vaddr + find_tid_info->tid_length) - *vaddr));
			assert(adj < *length);
			*vaddr += adj;
			*length -= adj;
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,
			       "adj %lu, vaddr [%p - %p] %lu/%#lX\n", adj,
			       (char *)*vaddr, (char *)(*vaddr) + *length, *length,
			       *length);
			/* new entry found, set it */
			(*inout_entry) = find_new_entry;
			OPX_DEBUG_EXIT((*inout_entry), OPX_ENTRY_FOUND);
			return OPX_ENTRY_FOUND;
		}
		/*
		 * Smaller hole/overlap, loop with smaller finds until
		 * we find the real hole or left-most overlap
		 */
		find_new_entry_info.iov.iov_len = (uint64_t)find_tid_info->tid_vaddr - *vaddr;
		assert(find_new_entry_info.iov.iov_len);
	} while (1);
	assert(0);
	return -1 ;
}

int opx_register_for_rzv(struct fi_opx_hfi1_rx_rzv_rts_params *params,
			 const uint64_t tid_vaddr, const uint64_t tid_length,
			 const enum fi_hmem_iface tid_iface,
			 const uint64_t tid_device)
{
	struct fi_opx_ep *opx_ep = params->opx_ep;
	struct opx_tid_domain *tid_domain = opx_ep->domain->tid_domain;
	struct ofi_mr_cache *tid_cache = tid_domain->tid_cache;
	struct ofi_mr_entry *entry = NULL;
	struct ofi_mr_info find_info = {0};
	int first_tid_index = -1, last_tid_index = -1, page_offset_in_tid = -1;

	assert(tid_vaddr == (tid_vaddr & -(int64_t)OPX_TID_PAGE_SIZE[tid_iface]));
	assert(tid_length == (tid_length & -(int64_t)OPX_TID_PAGE_SIZE[tid_iface]));

	pthread_mutex_lock(&mm_lock);

	find_info.iov.iov_base = (void *)tid_vaddr;
	find_info.iov.iov_len = tid_length;
	find_info.iface = tid_iface;
	find_info.device = tid_device;

	FI_DBG(fi_opx_global.prov, FI_LOG_MR,
	       "OPX_DEBUG_ENTRY tid vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
	       tid_vaddr, tid_vaddr + tid_length, tid_length, tid_length);
	int find = opx_tid_cache_find(opx_ep, &find_info, &entry);
	if (find == OPX_ENTRY_IN_USE) {
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "FAILED OPX_ENTRY_IN_USE "
		       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
		       "find vaddr [%#lx - %#lx] length %lu/%#lX\n",
		       tid_vaddr, tid_vaddr + tid_length,
		       tid_length, tid_length,
		       (uint64_t)find_info.iov.iov_base,
		       (uint64_t)find_info.iov.iov_base +
			       find_info.iov.iov_len,
		       find_info.iov.iov_len,
		       find_info.iov.iov_len);
		pthread_mutex_unlock(&mm_lock);
		/* Should be nothing to deregister at this point */
		return -FI_EPERM;
	}

	assert(find == OPX_ENTRY_FOUND || find == OPX_ENTRY_OVERLAP ||
	       find == OPX_ENTRY_NOT_FOUND);
	/* Two simple cases -
	 * - new entry
	 * - found entry
	 *
	 * Third case -
	 * - multple entries
	 */
	if (find == OPX_ENTRY_NOT_FOUND) {
		/* Flush the dead list, don't flush the lru list (false) */
		opx_tid_cache_flush(tid_cache, false);
		/* No entry found, create it. */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_NOT_FOUND\n");
		opx_tid_cache_crte(tid_cache, &find_info, &entry, opx_ep);
		struct opx_mr_tid_info *cached_tid_entry =
			&((struct opx_tid_mr *)entry->data)->tid_info;

		/* opx_register_tid_region was done in add region, check result */
		if (cached_tid_entry->ninfo == 0) { /* failed */
			OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u return -FI_EFAULT\n",
				__func__, __LINE__);
			/*crte returns an entry even if tid update failed */
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,
			       "OPX_DEBUG_EXIT FI_EFAULT (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, entry->data, entry->info.iov.iov_base,
			       (char *)entry->info.iov.iov_base +
				       entry->info.iov.iov_len,
			       entry->info.iov.iov_len, entry->info.iov.iov_len,
			       entry->use_cnt);
			/* Unlock for free/return */
			pthread_mutex_unlock(&mm_lock);
			opx_cache_free_entry(tid_cache, entry);
			return -FI_EFAULT;
		}

		/* Copy the tid info to params list for further modifications */
		params->ntidpairs = cached_tid_entry->npairs;
		assert(params->ntidpairs != 0);
		memcpy(params->tidpairs, &cached_tid_entry->pairs[0],
		       (cached_tid_entry->npairs *
			sizeof(cached_tid_entry->pairs[0])));
		params->tid_offset = 0;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "tid_offset %u/%#X\n",
		       params->tid_offset, params->tid_offset);
		OPX_DEBUG_TIDS("RTS tidpairs", params->ntidpairs,
			       params->tidpairs);
	} else if (find == OPX_ENTRY_FOUND) {
		struct opx_mr_tid_info *cached_tid_entry =
			&((struct opx_tid_mr *)entry->data)->tid_info;
		assert(cached_tid_entry->tid_length != 0);

		if (cached_tid_entry->invalid) {
			/* TID was invalidated while still in use and not deleted,
			   can't user or re-register it until it's dead. */
			/* Unlock for failed return */
			pthread_mutex_unlock(&mm_lock);
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"OPX_DEBUG_EXIT OPX_TID_IS_INVALID FI_EINVAL TIDs "
				"tid vaddr [%#lx - %#lx] length %lu/%#lX,  "
				"found [%#lx - %#lx] length %lu/%#lX\n",
				tid_vaddr, tid_vaddr + tid_length,
				tid_length, tid_length,
				cached_tid_entry->tid_vaddr,
				cached_tid_entry->tid_vaddr + cached_tid_entry->tid_length,
				cached_tid_entry->tid_length,
				cached_tid_entry->tid_length);
			return -FI_EINVAL;
		}
		/* Entry was found.  Our search is completely contained in this region */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_FOUND\n");
		opx_tid_inc_use_cnt(entry);

		OPX_DEBUG_TIDS("REUSE FULL LIST",
			       cached_tid_entry->npairs,
			       &cached_tid_entry->pairs[0]);
		opx_return_offset_for_new_cache_entry(
			(uint64_t)find_info.iov.iov_base,
			(uint64_t)find_info.iov.iov_len, cached_tid_entry,
			&first_tid_index, &page_offset_in_tid, &last_tid_index);
		OPX_DEBUG_TIDS("REUSE SUBSET LIST",
			       (last_tid_index - first_tid_index + 1),
			       &cached_tid_entry->pairs[first_tid_index]);

		/* Copy the tid info to params list for further modifications */
		params->ntidpairs = last_tid_index - first_tid_index + 1;
		assert(params->ntidpairs != 0);
		memcpy(params->tidpairs,
		       &cached_tid_entry->pairs[first_tid_index],
		       params->ntidpairs * sizeof(params->tidpairs[0]));
		params->tid_offset =
			page_offset_in_tid * OPX_HFI1_TID_PAGESIZE;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "tid_offset %u/%#X\n",
		       params->tid_offset, params->tid_offset);
		OPX_DEBUG_TIDS("RTS tidpairs", params->ntidpairs,
			       params->tidpairs);
	} else if (find == OPX_ENTRY_OVERLAP) {
		/* Multiple entries to process - loop */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_ENTRY_OVERLAP\n");

		const struct opx_mr_tid_info *const overlap_tid_entry =
			&((struct opx_tid_mr *)entry->data)->tid_info;
		assert(overlap_tid_entry->tid_length != 0);

		if (overlap_tid_entry->invalid) {
			/* TID was invalidated while still in use and not deleted,
			   can't user or re-register it until it's dead. */
			/* Unlock for failed return */
			pthread_mutex_unlock(&mm_lock);
			FI_WARN(fi_opx_global.prov, FI_LOG_MR,
				"OPX_DEBUG_EXIT OPX_TID_IS_INVALID FI_EINVAL TIDs "
				"tid vaddr [%#lx - %#lx] length %lu/%#lX,  "
				"found [%#lx - %#lx] length %lu/%#lX\n",
				tid_vaddr, tid_vaddr + tid_length,
				tid_length, tid_length,
				overlap_tid_entry->tid_vaddr,
				overlap_tid_entry->tid_vaddr + overlap_tid_entry->tid_length,
				overlap_tid_entry->tid_length,
				overlap_tid_entry->tid_length);
			return -FI_EINVAL;
		}
		/* Partial/overlapping memory region found */
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,
		       "OVERLAP TIDs "
		       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
		       "overlap vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
		       tid_vaddr, tid_vaddr + tid_length,
		       tid_length, tid_length,
		       overlap_tid_entry->tid_vaddr,
		       overlap_tid_entry->tid_vaddr + overlap_tid_entry->tid_length,
		       overlap_tid_entry->tid_length,
		       overlap_tid_entry->tid_length);

		uint64_t remaining_vaddr = tid_vaddr;
		int64_t remaining_length = tid_length;
		uint32_t ntidpairs = 0;
		/* This loop handles the more complicated
		   combinations of holes and overlap */
		bool once = true;
		while (remaining_length) {
			/* process previos find results */
			find = opx_process_entry(opx_ep, find,
						      &find_info, &entry,
						      &remaining_vaddr,
						      &remaining_length);
			if(find == OPX_ENTRY_IN_USE) {
				FI_DBG(fi_opx_global.prov, FI_LOG_MR,
				       "FAILED OPX_ENTRY_IN_USE "
				       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
				       "find vaddr [%#lx - %#lx] length %lu/%#lX, "
				       "deregister vaddr [%#lx - %#lx] length %lu/%#lX\n",
				       tid_vaddr, tid_vaddr + tid_length,
				       tid_length, tid_length,
				       (uint64_t)find_info.iov.iov_base,
				       (uint64_t)find_info.iov.iov_base + find_info.iov.iov_len,
				       find_info.iov.iov_len,
				       find_info.iov.iov_len, tid_vaddr,
				       tid_vaddr + tid_length -
				       (remaining_length + find_info.iov.iov_len),
				       tid_length - (remaining_length + find_info.iov.iov_len),
				       tid_length - (remaining_length + find_info.iov.iov_len));
				/* Unlock for deregister and free/return */
				pthread_mutex_unlock(&mm_lock);
				opx_deregister_for_rzv(
				    opx_ep, tid_vaddr,
				    tid_length - (remaining_length + find_info.iov.iov_len),
				    false);
				return -FI_EPERM;
			}
			if (find == OPX_ENTRY_NOT_FOUND) {
				/* Flush the dead list, don't flush the lru list (false) */
				if(once) {
					once = false;
					opx_tid_cache_flush(tid_cache, false);
				}
				FI_DBG(fi_opx_global.prov, FI_LOG_MR,
				       "NEXT OPX_ENTRY_NOT_FOUND TIDs "
				       "remaining vaddr [%#lx - %#lx] length %lu/%#lX, "
				       "not found vaddr [%#lx - %#lx] %lu/%#lX\n",
				       remaining_vaddr, remaining_vaddr + remaining_length,
				       remaining_length, remaining_length,
				       (uint64_t)find_info.iov.iov_base,
				       (uint64_t)find_info.iov.iov_base +
					       find_info.iov.iov_len,
				       find_info.iov.iov_len,
				       find_info.iov.iov_len);
				/* Create the missing hole */
				struct ofi_mr_entry *create_entry = NULL;
				int rc = opx_tid_cache_crte(tid_cache,
							    &find_info,
							    &create_entry,
							    opx_ep);
				if (rc) { /* failed */
					OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u return -FI_EFAULT\n",
						__func__, __LINE__);
					FI_DBG(fi_opx_global.prov, FI_LOG_MR,
					       "FAILED "
					       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
					       "failed vaddr [%#lx - %#lx] , tid length %lu/%#lX, "
					       "deregister vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
					       tid_vaddr, tid_vaddr + tid_length,
					       tid_length, tid_length,
					       (uint64_t)find_info.iov.iov_base,
					       (uint64_t)find_info.iov.iov_base +
						       find_info.iov.iov_len,
					       find_info.iov.iov_len,
					       find_info.iov.iov_len, tid_vaddr,
					       tid_vaddr + tid_length -
						       (remaining_length +
							find_info.iov.iov_len),
					       tid_length -
						       (remaining_length +
							find_info.iov.iov_len),
					       tid_length -
						       (remaining_length +
							find_info.iov.iov_len));
					/* Unlock for deregister and free/return */
					pthread_mutex_unlock(&mm_lock);
					opx_deregister_for_rzv(
						opx_ep, tid_vaddr,
						tid_length -
							(remaining_length +
							 find_info.iov.iov_len),
						false);
					/*crte may return an entry */
					if (create_entry) {
						FI_DBG(fi_opx_global.prov,
						       FI_LOG_MR,
						       "OPX_DEBUG_EXIT FI_EFAULT (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
						       create_entry,
						       create_entry->data,
						       create_entry->info.iov
							       .iov_base,
						       (char *)create_entry->info
								       .iov
								       .iov_base +
							       create_entry->info
								       .iov
								       .iov_len,
						       create_entry->info.iov
							       .iov_len,
						       create_entry->info.iov
							       .iov_len,
						       create_entry->use_cnt);
						opx_cache_free_entry(
							tid_cache,
							create_entry);
					}
					return -FI_EFAULT;
				}
				assert(create_entry);
				/* opx_register_tid_region was done in add region, check result */
				const struct opx_mr_tid_info
					*const create_tid_entry =
						&((struct opx_tid_mr *)
							  create_entry->data)
							 ->tid_info;
				if (create_tid_entry->ninfo ==
				    0) { /* failed */
					OPX_TID_CACHE_DEBUG_FPRINTF("## %s:%u return -FI_EFAULT\n",
						__func__, __LINE__);
					FI_DBG(fi_opx_global.prov, FI_LOG_MR,
					       "FAILED "
					       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
					       "failed vaddr [%#lx - %#lx] , tid length %lu/%#lX, "
					       "deregister vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
					       tid_vaddr, tid_vaddr + tid_length,
					       tid_length, tid_length,
					       (uint64_t)find_info.iov.iov_base,
					       (uint64_t)find_info.iov.iov_base +
						       find_info.iov.iov_len,
					       find_info.iov.iov_len,
					       find_info.iov.iov_len, tid_vaddr,
					       tid_vaddr + tid_length -
						       (remaining_length +
							find_info.iov.iov_len),
					       tid_length -
						       (remaining_length +
							find_info.iov.iov_len),
					       tid_length -
						       (remaining_length +
							find_info.iov.iov_len));
					/* Unlock for deregister and free/return */
					pthread_mutex_unlock(&mm_lock);
					opx_deregister_for_rzv(
						opx_ep, tid_vaddr,
						tid_length -
							(remaining_length +
							 find_info.iov.iov_len),
						false);
					/*crte returned an entry even if tid update failed */
					FI_DBG(fi_opx_global.prov, FI_LOG_MR,
					       "OPX_DEBUG_EXIT FI_EFAULT (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
					       create_entry, create_entry->data,
					       create_entry->info.iov.iov_base,
					       (char *)create_entry->info.iov
							       .iov_base +
						       create_entry->info.iov
							       .iov_len,
					       create_entry->info.iov.iov_len,
					       create_entry->info.iov.iov_len,
					       create_entry->use_cnt);
					opx_cache_free_entry(tid_cache,
							      create_entry);
					return -FI_EFAULT;
				}

				/* Copy the tid info to params list for further modifications */
				const uint32_t created_ntidpairs =
					(int)create_tid_entry->npairs;
				const uint32_t *created_tidpairs =
					&create_tid_entry->pairs[0];
				OPX_DEBUG_TIDS("Created tidpairs",
					       created_ntidpairs,
					       created_tidpairs);
				memcpy(&params->tidpairs[ntidpairs],
				       created_tidpairs,
				       (created_ntidpairs * sizeof(uint32_t)));
				if (ntidpairs == 0) {
					params->tid_offset = 0;
				}
				ntidpairs += created_ntidpairs;

			} else if (find == OPX_ENTRY_FOUND) {
				/* Use the found overlap*/
				const struct opx_mr_tid_info
					*const found_tid_entry =
						&((struct opx_tid_mr *)
							  entry->data)
							 ->tid_info;
				if (found_tid_entry->invalid) {
					/* TID was invalidated while still in use and not deleted,
					   can't user or re-register it until it's dead. */
					/* Unlock for failed return */
					pthread_mutex_unlock(&mm_lock);
					FI_WARN(fi_opx_global.prov, FI_LOG_MR,
						"OPX_DEBUG_EXIT OPX_TID_IS_INVALID FI_EINVAL TIDs "
						"vaddr [%#lx - %#lx] length %lu/%#lX "
						"found [%#lx - %#lx] length %lu/%#lX\n",
						tid_vaddr, tid_vaddr + tid_length, tid_length, tid_length,
						found_tid_entry->tid_vaddr,
						found_tid_entry->tid_vaddr + found_tid_entry->tid_length,
						found_tid_entry->tid_length,
						found_tid_entry->tid_length);
					return -FI_EINVAL;
				}
				opx_tid_inc_use_cnt(entry);
				FI_DBG(fi_opx_global.prov, FI_LOG_MR,
				       "NEXT OPX_ENTRY_FOUND TIDs "
				       "remaining vaddr [%#lx - %#lx] length %lu/%#lX, "
				       "found vaddr [%#lx - %#lx] %lu/%#lX\n",
				       remaining_vaddr, remaining_vaddr + remaining_length,
				       remaining_length, remaining_length,
				       found_tid_entry->tid_vaddr,
				       found_tid_entry->tid_vaddr +
					       found_tid_entry->tid_length,
				       found_tid_entry->tid_length,
				       found_tid_entry->tid_length);
				first_tid_index = 0;
				last_tid_index =
					(int)found_tid_entry->npairs;
				page_offset_in_tid = 0;
				OPX_DEBUG_TIDS("OVERLAP REUSE FULL LIST",
					       found_tid_entry->npairs,
					       &found_tid_entry->pairs[0]);
				if ((found_tid_entry->tid_vaddr <
				     remaining_vaddr) ||
				    (remaining_length <
				     found_tid_entry->tid_length)) {
					/* Not using all the found entry */
					opx_return_offset_for_new_cache_entry(
						(uint64_t)find_info.iov.iov_base,
						find_info.iov.iov_len,
						found_tid_entry,
						&first_tid_index,
						&page_offset_in_tid,
						&last_tid_index);
				}
				OPX_DEBUG_TIDS(
					"OVERLAP REUSE SUBSET LIST",
					(last_tid_index - first_tid_index + 1),
					&found_tid_entry->pairs[first_tid_index]);
				const uint32_t found_ntidpairs =
					last_tid_index - first_tid_index + 1;
				const uint32_t *found_tidpairs = &found_tid_entry->pairs[first_tid_index];
				assert(found_ntidpairs && found_tidpairs &&
				       params && params->tidpairs);

				if (ntidpairs == 0) {
					params->tid_offset =
						page_offset_in_tid *
						OPX_HFI1_TID_PAGESIZE;
				}

				memcpy(&params->tidpairs[ntidpairs],
				       found_tidpairs,
				       (found_ntidpairs * sizeof(uint32_t)));
				ntidpairs += found_ntidpairs;
				assert(ntidpairs < FI_OPX_MAX_DPUT_TIDPAIRS);
			} else { /* Impossible? */
				OPX_TID_CACHE_DEBUG_FPRINTF( "## %s:%u return -FI_EFAULT\n",
					__func__, __LINE__);
				FI_DBG(fi_opx_global.prov, FI_LOG_MR,
				       "IMPOSSIBLE? "
				       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
				       "failed vaddr [%#lx - %#lx] , tid length %lu/%#lX, "
				       "deregister vaddr [%#lx - %#lx] , tid length %lu/%#lX\n",
				       tid_vaddr, tid_vaddr + tid_length,
				       tid_length, tid_length,
				       (uint64_t)find_info.iov.iov_base,
				       (uint64_t)find_info.iov.iov_base +
					       find_info.iov.iov_len,
				       find_info.iov.iov_len,
				       find_info.iov.iov_len, tid_vaddr,
				       tid_vaddr + tid_length -
					       remaining_length,
				       tid_length - remaining_length,
				       tid_length - remaining_length);
				/* Unlock for deregister and free/return */
				pthread_mutex_unlock(&mm_lock);
				opx_deregister_for_rzv(
					opx_ep, tid_vaddr,
					tid_length - remaining_length,
					false);
				FI_DBG(fi_opx_global.prov, FI_LOG_MR,
				       "OPX_DEBUG_EXIT -FI_EFAULT\n");
				return -FI_EFAULT;
			}
			if (remaining_length) {
				/* Find for next loop/process */
				find_info.iov.iov_base =
					(void *)remaining_vaddr;
				find_info.iov.iov_len = remaining_length;
				find = opx_tid_cache_find(opx_ep, &find_info,
							  &entry);
				if (find == OPX_ENTRY_IN_USE) {
					FI_DBG(fi_opx_global.prov, FI_LOG_MR,
					       "FAILED OPX_ENTRY_IN_USE "
					       "tid vaddr [%#lx - %#lx] length %lu/%#lX, "
					       "find vaddr [%#lx - %#lx] length %lu/%#lX, "
					       "deregister vaddr [%#lx - %#lx] length %lu/%#lX\n",
					       tid_vaddr, tid_vaddr + tid_length,
					       tid_length, tid_length,
					       (uint64_t)find_info.iov.iov_base,
					       (uint64_t)find_info.iov.iov_base +
						       find_info.iov.iov_len,
					       find_info.iov.iov_len,
					       find_info.iov.iov_len, tid_vaddr,
					       tid_vaddr + tid_length -
						       (remaining_length +
							find_info.iov.iov_len),
					       tid_length -
						       (remaining_length +
							find_info.iov.iov_len),
					       tid_length -
						       (remaining_length +
							find_info.iov.iov_len));
					/* Unlock for deregister and free/return */
					pthread_mutex_unlock(&mm_lock);
					opx_deregister_for_rzv(
						opx_ep, tid_vaddr,
						tid_length -
							(remaining_length +
							 find_info.iov.iov_len),
						false);
					return -FI_EPERM;
				}
			}
		}
		params->ntidpairs = ntidpairs;
		OPX_DEBUG_TIDS("RTS tidpairs", params->ntidpairs,
			       params->tidpairs);
	}
	pthread_mutex_unlock(&mm_lock);
	FI_DBG(fi_opx_global.prov, FI_LOG_MR, "OPX_DEBUG_EXIT 0\n");
	return 0;
}

void opx_tid_cache_flush_all(struct ofi_mr_cache *cache,const bool flush_lru,const bool flush_all)
{
	struct dlist_entry free_list;
	struct ofi_mr_entry *entry;

	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY (%u/%u)\n",
	       flush_lru, flush_all);

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
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY no dead\n");
	} else {
		dlist_foreach_container(&cache->dead_region_list, struct ofi_mr_entry, entry,
				list_entry) {
			struct opx_tid_mr *const opx_mr = (struct opx_tid_mr *)entry->data;
			struct opx_mr_tid_info *const tid_info =
				&opx_mr->tid_info;
			FI_DBG(cache->domain->prov, FI_LOG_MR,
			       "OPX_DEBUG_ENTRY dead(invalid)->free entry %p, data %p opx_domain %p, endpoint %p, TID [%p - %p] (len: %zu,%#lX) emtru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep,
			       (char*)tid_info->tid_vaddr,
			       (char*)tid_info->tid_vaddr + tid_info->tid_length,
			       tid_info->tid_length,
			       tid_info->tid_length,
			       entry->info.iov.iov_base,
			       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
			       entry->info.iov.iov_len,
			       entry->info.iov.iov_len,
			       entry->use_cnt);
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
		dlist_pop_front(&cache->lru_list, struct ofi_mr_entry, entry,
				list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "(%p/%p) pop lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		       entry, entry->data,
		       entry->info.iov.iov_base,
		       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->info.iov.iov_len,
		       entry->use_cnt);
		assert(entry->use_cnt == 0);
		dlist_init(&entry->list_entry);
		opx_mr_uncache_entry_storage(cache, entry);
		dlist_insert_tail(&entry->list_entry, &free_list);

		flush_once = opx_tid_cache_full(cache);
	}

	pthread_mutex_unlock(&mm_lock);

	/* Free dead and selected lru entries */
	while (!dlist_empty(&free_list)) {
		dlist_pop_front(&free_list, struct ofi_mr_entry, entry,
				list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY flush free (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		       entry, entry ? entry->data : NULL,
		       entry->info.iov.iov_base,
		       (char *)entry->info.iov.iov_base +
			       entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->use_cnt);
		opx_cache_free_entry(cache, entry);
	}

	return ;
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
	struct dlist_entry free_list;
	struct ofi_mr_entry *entry;
	/* Not using a cache, return */
	if (cache == NULL) {
		return;
	}
	FI_DBG(cache->domain->prov, FI_LOG_MR, "OPX_DEBUG_ENTRY (%p)\n",
	       opx_ep);

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
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY no dead\n");
	} else {
		dlist_foreach_container(&cache->dead_region_list, struct ofi_mr_entry, entry,
				list_entry) {
			struct opx_tid_mr *const opx_mr = (struct opx_tid_mr *)entry->data;
			struct opx_mr_tid_info *const tid_info =
				&opx_mr->tid_info;
			FI_DBG(cache->domain->prov, FI_LOG_MR,
			       "OPX_DEBUG_ENTRY dead(invalid)->free entry %p, data %p opx_domain %p, endpoint %p, TID [%p - %p] (len: %zu,%#lX) emtru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep,
			       (char*)tid_info->tid_vaddr,
			       (char*)tid_info->tid_vaddr + tid_info->tid_length,
			       tid_info->tid_length,
			       tid_info->tid_length,
			       entry->info.iov.iov_base,
			       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
			       entry->info.iov.iov_len,
			       entry->info.iov.iov_len,
			       entry->use_cnt);
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

			dlist_pop_front(&cache->lru_list, struct ofi_mr_entry, entry,
					list_entry);
			FI_DBG(cache->domain->prov, FI_LOG_MR,
			       "(%p/%p) pop lru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
			       entry, entry->data,
			       entry->info.iov.iov_base,
			       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
			       entry->info.iov.iov_len,
			       entry->info.iov.iov_len,
			       entry->use_cnt);
			struct opx_tid_mr *const opx_mr = (struct opx_tid_mr *)entry->data;
			if ((opx_ep == NULL ) || (opx_mr->opx_ep == opx_ep)) {
				/* matching entries go on the free list */
				__attribute__((__unused__)) struct opx_mr_tid_info *const tid_info =
					&opx_mr->tid_info;
				if (entry->use_cnt > 0) {
					FI_WARN(cache->domain->prov, FI_LOG_MR,"Entry %p on endpoint %p was in use on exit\n", entry, opx_mr->opx_ep);
				}
				FI_DBG(cache->domain->prov, FI_LOG_MR,
				       "lru matching entry %p, data %p opx_domain %p, endpoint %p, TID [%p - %p] (len: %zu,%#lX) emtru [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
				       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep,
				       (char*)tid_info->tid_vaddr,
				       (char*)tid_info->tid_vaddr + tid_info->tid_length,
				       tid_info->tid_length,
				       tid_info->tid_length,
				       entry->info.iov.iov_base,
				       (char*)entry->info.iov.iov_base + entry->info.iov.iov_len,
				       entry->info.iov.iov_len,
				       entry->info.iov.iov_len,
				       entry->use_cnt);
				dlist_init(&entry->list_entry);
				opx_mr_uncache_entry_storage(cache, entry);
				dlist_insert_tail(&entry->list_entry, &free_list);
			} else {
				/* non-matching entries go on the local list */
				FI_DBG(cache->domain->prov, FI_LOG_MR,
				       "lru non-matching entry %p, data %p opx_domain %p, endpoint %p\n",
				       entry, opx_mr, opx_mr->domain, opx_mr->opx_ep);
				dlist_init(&entry->list_entry);
				dlist_insert_tail(&entry->list_entry, &copy_lru_list);

			}
		}
		if (!dlist_empty(&copy_lru_list)){
			dlist_init(&cache->lru_list);
			dlist_splice_tail(&cache->lru_list, &copy_lru_list);
		}
	}
	pthread_mutex_unlock(&mm_lock);

	/* Free dead and selected lru entries */
	while (!dlist_empty(&free_list)) {
		dlist_pop_front(&free_list, struct ofi_mr_entry, entry,
				list_entry);
		FI_DBG(cache->domain->prov, FI_LOG_MR,
		       "OPX_DEBUG_ENTRY flush free (%p/%p) [%p - %p] (len: %zu,%#lX) use_cnt %x\n",
		       entry, entry ? entry->data : NULL,
		       entry->info.iov.iov_base,
		       (char *)entry->info.iov.iov_base +
			       entry->info.iov.iov_len,
		       entry->info.iov.iov_len, entry->info.iov.iov_len,
		       entry->use_cnt);
		opx_cache_free_entry(cache, entry);
	}

	return ;
}

void opx_tid_cache_cleanup(struct ofi_mr_cache *cache)
{
	/* If we don't have a domain, initialization failed */
	if (!cache->domain)
		return;

	FI_INFO(cache->domain->prov, FI_LOG_MR, "MR cache stats: "
		"searches %zu, deletes %zu, hits %zu notify %zu\n",
		cache->search_cnt, cache->delete_cnt, cache->hit_cnt,
		cache->notify_cnt);

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

int opx_tid_cache_add_abort(){
	fprintf(stderr, "%s unexpected function call\n", __func__);
	abort();
	return 0;
}

void opx_tid_cache_delete_abort(){
	fprintf(stderr, "%s unexpected function call\n", __func__);
	abort();
}
