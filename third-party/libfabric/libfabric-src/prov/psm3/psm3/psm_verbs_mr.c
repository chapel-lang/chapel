/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */
// This performs memory registration for RDMA Rendezvous
// It also tracks MRs in use and allows existing MRs to be shared.

// in cache_mode MR_CACHE_MODE_USER, as a PoC we keep the cache overly simple.
// This approach is only viable for
// some microbenchmarks and simple apps.  For more complex apps the lack of
// invalidate hooks into memory free may lead to memory corruption.
// However such hooks are not reliably possible until the 4.17+ kernels.
// The kernel RV module hooks into mmu_notfiers for invalidate.  These are also
// used by hypervisors and hence are complete and reliable.

#include <sys/types.h>
#include "psm_user.h"	// pulls in psm_verbs_ep.h and psm_verbs_mr.h
#ifdef RNDV_MOD
#include "psm_rndv_mod.h"
#endif
#ifdef PSM_FI
#include "ips_config.h"
#endif

//#undef _HFI_MMDBG
//#define _HFI_MMDBG printf

#ifdef min
#undef min
#endif
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifdef max
#undef max
#endif
#define max(a, b) ((a) > (b) ? (a) : (b))

#define MEGABYTE (1024*1024)

#ifndef container_of
/*
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) \
	((type *) ((uint8_t *)(ptr) - offsetof(type, member)))
#endif


// Since rbtree.h and rbtree.c are designed to be included, and declare
// some hardcoded type names (cl_map_item_t and cl_qmap_t), we must limit
// our data type declarations which use those types to this .c file

// this will be the payload of a cl_qmap_t
struct psm2_mr_cache_map_pl {
	uint32_t	nelems;	// number of elements in cache
};

// rbtree.h uses these 2 well known defines to create the payload for
// cl_map_item_t and cl_qmap_t structures
#define RBTREE_MI_PL  struct psm2_verbs_mr
#define RBTREE_MAP_PL struct psm2_mr_cache_map_pl
#include "rbtree.h"

struct psm2_mr_cache {
	uint32_t max_entries;
	// limits to allow headroom for priority registrations
	uint32_t limit_inuse;
	uint64_t limit_inuse_bytes;
#ifdef RNDV_MOD
	psm2_rv_t rv;
	int cmd_fd;
#endif
#ifdef PSM_FI
	psm2_ep_t ep;
#endif
	uint8_t cache_mode;	// MR_CACHE_MODE_*
	cl_qmap_t map;
	cl_map_item_t root;
	cl_map_item_t nil_item;
	// Below is for queue of cache entries available for reuse (refcount==0)
	// only used when cache_mode==MR_CACHE_MODE_USER.
	// Available entries are added at end of list and reused from start.
	// Hence having aging of cached entries.
	// Aging helps reduce some of the corruption risk,
	// but is not a full solution.  Good enough for the PoC
	TAILQ_HEAD(avail_list, psm2_verbs_mr) avail_list;
	mpool_t mr_pool;	// pool of MRs
	// some statistics for user space
	uint64_t hit;
	uint64_t miss;
	uint64_t rejected;		// rejected non-priority registration
	uint64_t full;			// failed registration (tends to be priority)
	uint64_t failed;		// other failures, should be none
	uint32_t inuse;		// entry count in use
	uint32_t max_inuse;
	uint64_t inuse_bytes;
	uint64_t max_inuse_bytes;
	uint32_t max_nelems;
	uint32_t max_refcount;
#ifdef RNDV_MOD
	struct psm2_rv_cache_stats rv_stats;	// statistics from rv module
									// will remain 0 if rv not open
#endif
};

static int mr_cache_key_cmp(const struct psm2_verbs_mr *a,
							const struct psm2_verbs_mr *b)
{
	// to match addr, length and access must match
	// we require exact match to avoid the issue of a release of the larger
	// MR while smaller overlapping MR still in use, just in case an
	// allocator frees the extra memory not in the smaller MR
	// this may be paranoid, TBD if should treat a smaller MR as a match
	// of a larger subset MR.
	if (a->access < b->access)
		return -1;
	else if (a->access > b->access)
		return 1;
	if (a->addr < b->addr)
		return -1;
	else if (a->addr > b->addr)
		return 1;
	if (a->length < b->length)
		return -1;
	else if (a->length > b->length)
		return 1;
	return 0;
}

// rbtree.c uses these defines to establish some of it's code and
// then provides all the rbtree manipulation functions
// we want to control the compare funciton so we define RBTREE_CMP and thus
// must define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR to avoid compiler errors
#define RBTREE_CMP(a,b) mr_cache_key_cmp((a), (b))
#define RBTREE_ASSERT                     psmi_assert
#define RBTREE_MAP_COUNT(PAYLOAD_PTR)     ((PAYLOAD_PTR)->nelems)
#define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR
#include "rbtree.c"

// TBD - move to a utility macro header
// taken fron IbAccess imath.h and imath.c
static uint32_t
ones64(uint64_t x)
{
	x -= ((x >> 1) & 0x5555555555555555ULL);
	x = (((x >> 2) & 0x3333333333333333ULL) + (x & 0x3333333333333333ULL));
	x = (((x >> 4) + x) & 0x0f0f0f0f0f0f0f0fULL);
	x += (x >> 8);
	x += (x >> 16);
	x += (x >> 32);
	return(x & 0x0000003f);
}

/* log2(x) truncated */
uint32_t
FloorLog2(uint64_t x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	x |= (x >> 32);
	return(ones64(x >> 1));
}

/* log2(x) rounded up if x is not a power of 2 */
uint32_t CeilLog2(uint64_t val)
{
	uint32_t floor2 = FloorLog2(val);
	if ((1ULL << floor2) == val)
		return (floor2);
	else
		return (floor2+1);
}

static inline uint32_t NextPower2(uint64_t x)
{
	return (1 << CeilLog2(x));
}

// accessor functions for statistics
#define CACHE_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = (psm2_mr_cache_t)context; \
		return cache->stat; \
    }


CACHE_STAT_FUNC(mr_cache_mode, cache_mode)
CACHE_STAT_FUNC(mr_cache_max_entries, max_entries)
CACHE_STAT_FUNC(mr_cache_nelems, map.payload.nelems)
CACHE_STAT_FUNC(mr_cache_max_nelems, max_nelems)
CACHE_STAT_FUNC(mr_cache_limit_inuse, limit_inuse)
CACHE_STAT_FUNC(mr_cache_inuse, inuse)
CACHE_STAT_FUNC(mr_cache_max_inuse, max_inuse)
CACHE_STAT_FUNC(mr_cache_max_refcount, max_refcount)
#undef CACHE_STAT_FUNC

static uint64_t mr_cache_hit_rate(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	if (cache->miss)	// all entries start with a miss, then get hits
		return((cache->hit*100)/(cache->miss+cache->hit));
	else
		return 0;
}

static uint64_t mr_cache_miss_rate(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	if (cache->miss)	// all entries start with a miss, then get hits
		return((cache->miss*100)/(cache->miss+cache->hit));
	else
		return 0;
}

#ifdef RNDV_MOD
static uint64_t mr_cache_rv_size(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	if (cache->rv) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		(void)__psm2_rv_get_cache_stats(cache->rv, &cache->rv_stats);
	}
	return cache->rv_stats.cache_size/MEGABYTE;
}

#define CACHE_RV_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = (psm2_mr_cache_t)context; \
		return cache->rv_stats.stat; \
    }

CACHE_RV_STAT_FUNC(mr_cache_rv_max_size, max_cache_size/MEGABYTE)
CACHE_RV_STAT_FUNC(mr_cache_rv_limit_size, limit_cache_size)
CACHE_RV_STAT_FUNC(mr_cache_rv_nelems, count)
CACHE_RV_STAT_FUNC(mr_cache_rv_max_nelems, max_count)
CACHE_RV_STAT_FUNC(mr_cache_rv_inuse, inuse)
CACHE_RV_STAT_FUNC(mr_cache_rv_max_inuse, max_inuse)
CACHE_RV_STAT_FUNC(mr_cache_rv_max_refcount, max_refcount)
#undef CACHE_RV_STAT_FUNC

static uint64_t mr_cache_rv_hit_rate(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	if (cache->rv_stats.miss)	// all entries start with a miss, then get hits
		return((cache->rv_stats.hit*100)/(cache->rv_stats.miss+cache->rv_stats.hit));
	else
		return 0;
}

static uint64_t mr_cache_rv_miss_rate(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	if (cache->rv_stats.miss)	// all entries start with a miss, then get hits
		return((cache->rv_stats.miss*100)/(cache->rv_stats.miss+cache->rv_stats.hit));
	else
		return 0;
}
#endif // RNDV_MOD

#define INC_STAT(cache, stat, max_stat) \
	do { \
		if (++((cache)->stat) > (cache)->max_stat) \
			(cache)->max_stat = (cache)->stat; \
	} while(0)

#define ADD_STAT(cache, adder, stat, max_stat) \
	do { \
		if (((cache)->stat += (adder)) > (cache)->max_stat) \
			(cache)->max_stat = (cache)->stat; \
	} while(0)


// ep is used for RNDV_MOD, memory tracking and stats
psm2_mr_cache_t psm2_verbs_alloc_mr_cache(psm2_ep_t ep,
							uint32_t max_entries, uint8_t cache_mode,
							uint32_t pri_entries, uint64_t pri_size)
{
	struct psm2_mr_cache *cache;

	cache = (struct psm2_mr_cache *)psmi_calloc(ep, DESCRIPTORS,
							sizeof(*cache), 1);
	if (! cache)
		return NULL;
	// max_entries for a pool must be power of 2
	max_entries = max(max_entries, pri_entries);
	max_entries = NextPower2(max_entries);
	cache->max_entries = max_entries;
	cache->cache_mode = cache_mode;
	// we leave headroom for priority registrations
	cache->limit_inuse = max_entries - pri_entries;
#ifdef PSM_FI
	cache->ep = ep;
#endif
#ifdef RNDV_MOD
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL
		|| cache->cache_mode == MR_CACHE_MODE_RV) {
		if (ep->rv_mr_cache_size*MEGABYTE < pri_size) {
			_HFI_ERROR("PSM3_RV_MR_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
				ep->rv_mr_cache_size, (pri_size + MEGABYTE-1)/MEGABYTE);
			return NULL;
		}
		cache->limit_inuse_bytes = ep->rv_mr_cache_size*MEGABYTE - pri_size;
	} else
#endif // RNDV_MOD
		cache->limit_inuse_bytes = UINT64_MAX;	// no limit, just count inuse
#ifdef RNDV_MOD
	cache->rv = ep->verbs_ep.rv;
	cache->cmd_fd = ep->verbs_ep.context->cmd_fd;
#endif // RNDV_MOD
	_HFI_MMDBG("cache alloc: max_entries=%u limit_inuse=%u limit_inuse_bytes=%"PRIu64", pri_entries=%u pri_size=%"PRIu64"\n",
			cache->max_entries, cache->limit_inuse,
			cache->limit_inuse_bytes, pri_entries, pri_size);
	// max_entries must be power of 2>= obj per chunk which is also a power of 2
	cache->mr_pool = psmi_mpool_create(sizeof(cl_map_item_t),
						min(128, max_entries), max_entries, 0,
						DESCRIPTORS, NULL, NULL);
	if (! cache->mr_pool) {
		psmi_free(cache);
		return NULL;
	}
	//nil_item already zeroed by calloc
	//memset(&cache->nil_item.payload, 0, sizeof(cache->nil_item.payload));
	ips_cl_qmap_init(&cache->map, &cache->root, &cache->nil_item);
	TAILQ_INIT(&cache->avail_list);

	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL("cache_mode", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_mode, NULL),
		PSMI_STATS_DECL_FUNC("limit_entries", mr_cache_max_entries),
		PSMI_STATS_DECL_FUNC("nelems", mr_cache_nelems),
		PSMI_STATS_DECL_FUNC("max_nelems", mr_cache_max_nelems),
		PSMI_STATS_DECL("limit_inuse",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_limit_inuse, NULL),
		PSMI_STATS_DECL_FUNC("inuse", mr_cache_inuse),
		PSMI_STATS_DECL_FUNC("max_inuse", mr_cache_max_inuse),
		PSMI_STATS_DECL("limit_inuse_bytes",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &cache->limit_inuse_bytes),
		PSMI_STATS_DECLU64("inuse_bytes", &cache->inuse_bytes),
		PSMI_STATS_DECLU64("max_inuse_bytes", &cache->max_inuse_bytes),
		PSMI_STATS_DECL_FUNC("max_refcount", mr_cache_max_refcount),
		PSMI_STATS_DECLU64("hit", &cache->hit),
		PSMI_STATS_DECL("hit_%",MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_hit_rate, NULL),
		PSMI_STATS_DECLU64("miss", &cache->miss),
		PSMI_STATS_DECL("miss_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_miss_rate, NULL),
		PSMI_STATS_DECLU64("rejected", &cache->rejected),
		PSMI_STATS_DECLU64("full", &cache->full),
		PSMI_STATS_DECLU64("failed", &cache->failed),
#ifdef RNDV_MOD
		PSMI_STATS_DECL_FUNC("rv_size", mr_cache_rv_size),
		PSMI_STATS_DECL_FUNC("rv_max_size", mr_cache_rv_max_size),
		PSMI_STATS_DECL_FUNC("rv_limit", mr_cache_rv_limit_size),
		PSMI_STATS_DECL_FUNC("rv_nelems", mr_cache_rv_nelems),
		PSMI_STATS_DECL_FUNC("rv_max_nelems", mr_cache_rv_max_nelems),
		PSMI_STATS_DECL_FUNC("rv_inuse", mr_cache_rv_inuse),
		PSMI_STATS_DECL_FUNC("rv_max_inuse", mr_cache_rv_max_inuse),
		PSMI_STATS_DECLU64("rv_inuse_bytes", (uint64_t*)&cache->rv_stats.inuse_bytes),
		PSMI_STATS_DECLU64("rv_max_inuse_bytes", (uint64_t*)&cache->rv_stats.max_inuse_bytes),
		PSMI_STATS_DECL_FUNC("rv_max_refcount", mr_cache_rv_max_refcount),
		PSMI_STATS_DECLU64("rv_hit", (uint64_t*)&cache->rv_stats.hit),
		PSMI_STATS_DECL("rv_hit %", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_hit_rate, NULL),
		PSMI_STATS_DECLU64("rv_miss", (uint64_t*)&cache->rv_stats.miss),
		PSMI_STATS_DECL("rv_miss %", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_miss_rate, NULL),
		PSMI_STATS_DECLU64("rv_full", (uint64_t*)&cache->rv_stats.full),
		PSMI_STATS_DECLU64("rv_failed", (uint64_t*)&cache->rv_stats.failed),
		PSMI_STATS_DECLU64("rv_remove", (uint64_t*)&cache->rv_stats.remove),
		PSMI_STATS_DECLU64("rv_evict", (uint64_t*)&cache->rv_stats.evict),
#endif // RNDV_MOD
	};
	psmi_stats_register_type("MR_Cache_Statistics",
					PSMI_STATSTYPE_MR_CACHE,
					entries,
					PSMI_STATS_HOWMANY(entries),
					ep->epid, cache);

	return cache;
}

// checks for space for a non-priority registration
static inline int have_space(psm2_mr_cache_t cache, uint64_t length)
{
	return (cache->inuse < cache->limit_inuse
			&& cache->inuse_bytes + length < cache->limit_inuse_bytes);
}

// each attempt will increment exactly one of: hit, miss, rejected, full, failed
struct psm2_verbs_mr * psm2_verbs_reg_mr(psm2_mr_cache_t cache,
				bool priority, struct ibv_pd *pd,
				void *addr, uint64_t length, int access)
{
	psm2_verbs_mr_t mrc;

#ifdef PSM_FI
	if_pf(PSMI_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_reg_mr, "reg_mr",
				"MR cache full, any request type",
				1, IPS_FAULTINJ_REG_MR);
		if (psmi_faultinj_is_fault(fi_reg_mr)) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(!priority && PSMI_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_nonpri_reg_mr, "nonpri_reg_mr",
				"MR cache full, non-priority request",
				1, IPS_FAULTINJ_NONPRI_REG_MR);
		if (psmi_faultinj_is_fault(fi_nonpri_reg_mr)) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(priority && PSMI_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_pri_reg_mr, "pri_reg_mr",
				"MR cache full, priority request",
				1, IPS_FAULTINJ_PRI_REG_MR);
		if (psmi_faultinj_is_fault(fi_pri_reg_mr)) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
#endif
	access |= IBV_ACCESS_LOCAL_WRITE;	// manditory flag
#ifndef RNDV_MOD
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
		_HFI_ERROR("unsupported GPU memory registration\n");
		cache->failed++;
		errno = EINVAL;
		return NULL;
	}
#endif
	struct psm2_verbs_mr key = { // our search key
		.addr = addr,
		.length = length,
		// only 8 bits in mrc for access
		.access = (access & ~(IBV_ACCESS_IS_GPU_ADDR
#ifdef RNDV_MOD
								|IBV_ACCESS_KERNEL
#endif
								))
	};
	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, &key);
	if (p_item->payload.mr.mr_ptr) {
		psmi_assert(p_item != cache->map.nil_item);
		mrc = &p_item->payload;
		if (! mrc->refcount) {
			if (! priority && ! have_space(cache, length)) {
				_HFI_MMDBG("cache has no headroom for non-priority hit addr %p len %"PRIu64" access 0x%x ptr %p\n",
						addr, length, access, mrc);
				cache->rejected++;
				errno = ENOMEM;
				return NULL;
			}
			// it was an entry on avail_list, take off list
			TAILQ_REMOVE(&cache->avail_list, mrc, next);
			INC_STAT(cache, inuse, max_inuse);
			ADD_STAT(cache, length, inuse_bytes, max_inuse_bytes);
		}
		cache->hit++;
		_HFI_MMDBG("cache hit MR addr %p len %"PRIu64" access 0x%x ptr %p\n",
						addr, length, mrc->access, mrc);
		mrc->refcount++;
		cache->max_refcount = max(cache->max_refcount, mrc->refcount);
		return mrc;
	}
	psmi_assert(p_item == cache->map.nil_item);
	if (! priority && ! have_space(cache, length)) {
		_HFI_MMDBG("cache has no headroom for non-priority miss addr %p len %"PRIu64" access 0x%x\n",
			addr, length, access);
		cache->rejected++;
		errno = ENOMEM;
		return NULL;
	}
	// we only reuse entries from avail_list once cache is full
	// this helps improve cache hit rate.
	// we only have items on avail_list when cache_mode==MR_CACHE_MODE_USER
	if (cache->map.payload.nelems >= cache->max_entries) {
		int ret;
		mrc = TAILQ_FIRST(&cache->avail_list);
		if (! mrc) {
			_HFI_MMDBG("user space MR cache full\n");
			cache->full++;
			errno = ENOMEM;
			return NULL;
		}
		p_item = container_of(mrc, cl_map_item_t, payload);
		psmi_assert(mrc->mr.mr_ptr);
		psmi_assert(! mrc->refcount);
		_HFI_MMDBG("reuse avail MR addr %p len %"PRIu64" access 0x%x ptr %p\n",
					addr, length, mrc->access, mrc);
		ips_cl_qmap_remove_item(&mrc->cache->map, p_item);
		TAILQ_REMOVE(&cache->avail_list, mrc, next);
#ifdef RNDV_MOD
		if (cache->cache_mode == MR_CACHE_MODE_KERNEL
			|| cache->cache_mode == MR_CACHE_MODE_RV)	// should not happen
			ret = __psm2_rv_dereg_mem(cache->rv, mrc->mr.rv_mr);
		else
#endif
			ret = ibv_dereg_mr(mrc->mr.ibv_mr);
		if (ret) {
			_HFI_ERROR("unexpected dreg_mr failure: %s", strerror(errno));
			cache->failed++;
			errno = EIO;
			// MR is fouled up, we leak the MR and free the cache entry
			// caller will try again later
			mrc->mr.mr_ptr = NULL;
			psmi_mpool_put(p_item);
			return NULL;
		}
		mrc->mr.mr_ptr = NULL;
	} else {
		// allocate a new item
		p_item = (cl_map_item_t *)psmi_mpool_get(cache->mr_pool);
		if (! p_item) {	// keep KW happy, should not happen, we check max above
			_HFI_ERROR("unexpected cache pool allocate failure\n");
			cache->failed++;
			return NULL;
		}
		mrc = &p_item->payload;
		// we initialize mrc below
		cache->max_nelems = max(cache->max_nelems, cache->map.payload.nelems+1);
	}
#ifdef RNDV_MOD
	/* need cmd_fd for access to ucontext when converting user pd into kernel pd */
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL) {
		mrc->mr.rv_mr = __psm2_rv_reg_mem(cache->rv, cache->cmd_fd, pd, addr, length, access);
		if (! mrc->mr.rv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
			} else {
				_HFI_ERROR("reg_mr failed; %s", strerror(errno));
				cache->failed++;
			}
			psmi_mpool_put(p_item);
			errno = save_errno;
			return NULL;
		}
		mrc->iova = mrc->mr.rv_mr->iova;
		mrc->lkey = mrc->mr.rv_mr->lkey;
		mrc->rkey = mrc->mr.rv_mr->rkey;
	} else if (cache->cache_mode == MR_CACHE_MODE_RV) {
		mrc->mr.rv_mr = __psm2_rv_reg_mem(cache->rv, cache->cmd_fd, NULL, addr, length, access|IBV_ACCESS_KERNEL);
		if (! mrc->mr.rv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
			} else {
				_HFI_ERROR("reg_mr failed; %s", strerror(errno));
				cache->failed++;
			}
			psmi_mpool_put(p_item);
			errno = save_errno;
			return NULL;
		}
		mrc->iova = mrc->mr.rv_mr->iova;
		mrc->lkey = mrc->mr.rv_mr->lkey;
		mrc->rkey = mrc->mr.rv_mr->rkey;
	} else
#endif
	{
		mrc->mr.ibv_mr = ibv_reg_mr(pd, addr, length, access);
		if (! mrc->mr.ibv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
			} else {
				_HFI_ERROR("reg_mr failed; %s", strerror(errno));
				cache->failed++;
			}
			psmi_mpool_put(p_item);
			errno = save_errno;
			return NULL;
		}
		mrc->iova = (uintptr_t)addr;
		mrc->lkey = mrc->mr.ibv_mr->lkey;
		mrc->rkey = mrc->mr.ibv_mr->rkey;
	}
	cache->miss++;
	mrc->cache = cache;
	mrc->refcount = 1;
	mrc->addr = addr;
	mrc->length = length;
	mrc->access = access;
	ips_cl_qmap_insert_item(&cache->map, p_item);
	INC_STAT(cache, inuse, max_inuse);
	ADD_STAT(cache, length, inuse_bytes, max_inuse_bytes);
	_HFI_MMDBG("registered new MR pri %d addr %p len %"PRIu64" access 0x%x ptr %p nelems %u\n",
					priority, addr, length, mrc->access, mrc,
					cache->map.payload.nelems);
	return mrc;
}

int psm2_verbs_release_mr(struct psm2_verbs_mr *mrc)
{
	int ret = 0;
	if (! mrc) {
		errno = EINVAL;
		return -1;
	}
	if (! mrc->refcount) {
		errno = ENXIO;
		return -1;
	}
	_HFI_MMDBG("releasing MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p\n",
				mrc->addr, mrc->length, mrc->access,
				mrc->refcount, mrc);
	if (mrc->cache->cache_mode == MR_CACHE_MODE_USER) {
		// if refcount now zero, put on avail_list to be reclaimed if needed
		if (! --(mrc->refcount)) {
			mrc->cache->inuse--;
			mrc->cache->inuse_bytes -= mrc->length;
			TAILQ_INSERT_TAIL(&mrc->cache->avail_list, mrc, next);
		}
	} else {
		if (! --(mrc->refcount)) {
			_HFI_MMDBG("freeing MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p nelems %u\n",
						mrc->addr, mrc->length, mrc->access,
						mrc->refcount, mrc, mrc->cache->map.payload.nelems);
			mrc->cache->inuse--;
			mrc->cache->inuse_bytes -= mrc->length;
			cl_map_item_t *p_item = container_of(mrc, cl_map_item_t, payload);
			ips_cl_qmap_remove_item(&mrc->cache->map, p_item);
#ifdef RNDV_MOD
			if (mrc->cache->cache_mode == MR_CACHE_MODE_KERNEL
					|| mrc->cache->cache_mode == MR_CACHE_MODE_RV)
				ret = __psm2_rv_dereg_mem(mrc->cache->rv, mrc->mr.rv_mr);
			else
#endif
				ret = ibv_dereg_mr(mrc->mr.ibv_mr);
			if (ret) {
				// nasty choice, do we leak the MR or leak the cache entry
				// we chose to leak the MR and free the cache entry
				_HFI_ERROR("unexpected dreg_mr failure: %s", strerror(errno));
				errno = EIO;
				ret = -1;
			}
			mrc->mr.mr_ptr = NULL;
			psmi_mpool_put(p_item);
		}
	}
	return ret;
}

void psm2_verbs_free_mr_cache(psm2_mr_cache_t cache)
{
	psmi_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE, cache);
	while (cache->map.payload.nelems) {
		cl_map_item_t *p_item = __cl_map_root(&cache->map);
		psmi_assert(p_item != cache->map.nil_item);
		psm2_verbs_mr_t mrc = &p_item->payload;
		psmi_assert(mrc->mr.mr_ptr);
		if (mrc->mr.mr_ptr) {
			int ret;
			_HFI_MMDBG("free MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p\n",
					mrc->addr, mrc->length, mrc->access,
					mrc->refcount, mrc);
			if (mrc->refcount)
				_HFI_ERROR("unreleased MR in psm2_verbs_free_mr_cache addr %p len %"PRIu64" access 0x%x\n", mrc->addr, mrc->length, mrc->access);
			mrc->refcount = 0;
			cl_map_item_t *p_item = container_of(mrc, cl_map_item_t, payload);
			ips_cl_qmap_remove_item(&cache->map, p_item);
			TAILQ_REMOVE(&cache->avail_list, mrc, next);
#ifdef RNDV_MOD
			if (cache->cache_mode == MR_CACHE_MODE_KERNEL
					|| cache->cache_mode == MR_CACHE_MODE_RV)
				ret = __psm2_rv_dereg_mem(cache->rv, mrc->mr.rv_mr);
			else
#endif
				ret = ibv_dereg_mr(mrc->mr.ibv_mr);
			if (ret)
				_HFI_ERROR("unexpected dreg_mr failure: %s", strerror(errno));
			mrc->mr.mr_ptr = NULL;
			psmi_mpool_put(p_item);
		}
	}
	psmi_assert(TAILQ_EMPTY(&cache->avail_list));
	psmi_assert(! cache->map.payload.nelems);

	psmi_mpool_destroy(cache->mr_pool);
	psmi_free(cache);
}
