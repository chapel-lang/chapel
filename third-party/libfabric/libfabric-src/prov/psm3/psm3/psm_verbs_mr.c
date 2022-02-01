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
#include "psm2_hal.h"
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
#include "psm3_rbtree.h"

struct psm2_mr_cache {
	uint32_t max_entries;
	// limits to allow headroom for priority registrations
	uint32_t limit_inuse;
	uint64_t limit_inuse_bytes;
#ifdef RNDV_MOD
#ifdef PSM_CUDA
	uint64_t limit_gpu_inuse_bytes;
#endif
	psm2_rv_t rv;
	int cmd_fd;
#endif
	psm2_ep_t ep;
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
#ifdef RNDV_MOD
#ifdef PSM_CUDA
	uint64_t gpu_inuse_bytes;
	uint64_t max_gpu_inuse_bytes;
#endif
#endif
	uint32_t max_nelems;
	uint32_t max_refcount;
#ifdef RNDV_MOD
	struct psm2_rv_cache_stats rv_stats;	// statistics from rv module
									// will remain 0 if rv not open
#ifdef PSM_CUDA
	struct psm2_rv_gpu_cache_stats rv_gpu_stats;	// GPU statistics from rv module
									// will remain 0 if rv not open
#endif
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
#include "psm3_rbtree.c"

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

#ifdef PSM_CUDA
static uint64_t mr_cache_rv_gpu_size(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	if (cache->rv && PSMI_IS_CUDA_ENABLED ) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		(void)__psm2_rv_gpu_get_cache_stats(cache->rv, &cache->rv_gpu_stats);
	}
	return cache->rv_gpu_stats.cache_size/MEGABYTE;
}

#define CACHE_RV_GPU_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats); \
		return cache->rv_gpu_stats.stat; \
    }

CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size_reg, cache_size_reg/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size_mmap, cache_size_mmap/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size_both, cache_size_both/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size, max_cache_size/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size_reg, max_cache_size_reg/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size_mmap, max_cache_size_mmap/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size_both, max_cache_size_both/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_limit_size, limit_cache_size)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_nelems, count)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_nelems_reg, count_reg)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_nelems_mmap, count_mmap)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_nelems_both, count_both)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_nelems, max_count)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_nelems_reg, max_count_reg)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_nelems_mmap, max_count_mmap)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_nelems_both, max_count_both)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_inuse, inuse)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_inuse_reg, inuse_reg)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_inuse_mmap, inuse_mmap)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_inuse_both, inuse_both)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_inuse, max_inuse)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_inuse_reg, max_inuse_reg)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_inuse_mmap, max_inuse_mmap)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_inuse_both, max_inuse_both)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_refcount, max_refcount)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_refcount_reg, max_refcount_reg)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_refcount_mmap, max_refcount_mmap)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_refcount_both, max_refcount_both)
#undef CACHE_RV_GPU_STAT_FUNC

/* any hit which found an entry, even if partial */
static uint64_t mr_cache_rv_gpu_hit_rate(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	if (cache->rv_gpu_stats.miss)	// all entries start with a miss, then get hits
		return((cache->rv_gpu_stats.hit*100)/(cache->rv_gpu_stats.miss+cache->rv_gpu_stats.hit));
	else
		return 0;
}

/* pure hit, want MR and found entry w/ MR */
static uint64_t mr_cache_rv_gpu_hit_rate_reg(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	// all entries start with a miss or add_reg, then get hits
	if (cache->rv_gpu_stats.miss_reg || cache->rv_gpu_stats.hit_add_reg)
		return((cache->rv_gpu_stats.hit_reg*100)/(cache->rv_gpu_stats.miss_reg+cache->rv_gpu_stats.hit_reg+cache->rv_gpu_stats.hit_add_reg));
	else
		return 0;
}

/* partial hit, want MR and found pinned entry w/o MR */
static uint64_t mr_cache_rv_gpu_hit_rate_add_reg(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	// all entries start with a miss or add_reg, then get hits
	if (cache->rv_gpu_stats.miss_reg || cache->rv_gpu_stats.hit_add_reg)
		return((cache->rv_gpu_stats.hit_add_reg*100)/(cache->rv_gpu_stats.miss_reg+cache->rv_gpu_stats.hit_reg+cache->rv_gpu_stats.hit_add_reg));
	else
		return 0;
}

/* pure hit, want mmap and found entry w/ mmap */
static uint64_t mr_cache_rv_gpu_hit_rate_mmap(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	// all entries start with a miss or add_mmap, then get hits
	if (cache->rv_gpu_stats.miss_mmap || cache->rv_gpu_stats.hit_add_mmap)
		return((cache->rv_gpu_stats.hit_mmap*100)/(cache->rv_gpu_stats.miss_mmap+cache->rv_gpu_stats.hit_mmap+cache->rv_gpu_stats.hit_add_mmap));
	else
		return 0;
}

/* partial hit, want MR and found pinned entry w/o mmap */
static uint64_t mr_cache_rv_gpu_hit_rate_add_mmap(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	// all entries start with a miss or add_mmap, then get hits
	if (cache->rv_gpu_stats.miss_mmap || cache->rv_gpu_stats.hit_add_mmap)
		return((cache->rv_gpu_stats.hit_add_mmap*100)/(cache->rv_gpu_stats.miss_mmap+cache->rv_gpu_stats.hit_mmap+cache->rv_gpu_stats.hit_add_mmap));
	else
		return 0;
}

/* complete miss, no entry found */
static uint64_t mr_cache_rv_gpu_miss_rate(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	if (cache->rv_gpu_stats.miss)	// all entries start with a miss, then get hits
		return((cache->rv_gpu_stats.miss*100)/(cache->rv_gpu_stats.miss+cache->rv_gpu_stats.hit));
	else
		return 0;
}

/* no entry found when want MR */
static uint64_t mr_cache_rv_gpu_miss_rate_reg(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	// all entries start with a miss or add_reg, then get hits
	if (cache->rv_gpu_stats.miss_reg || cache->rv_gpu_stats.hit_add_reg)
		return((cache->rv_gpu_stats.miss_reg*100)/(cache->rv_gpu_stats.miss_reg+cache->rv_gpu_stats.hit_reg+cache->rv_gpu_stats.hit_add_reg));
	else
		return 0;
}

/* no entry found when want mmap */
static uint64_t mr_cache_rv_gpu_miss_rate_mmap(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	// all entries start with a miss or add_reg, then get hits
	if (cache->rv_gpu_stats.miss_mmap || cache->rv_gpu_stats.hit_add_mmap)
		return((cache->rv_gpu_stats.miss_mmap*100)/(cache->rv_gpu_stats.miss_mmap+cache->rv_gpu_stats.hit_mmap+cache->rv_gpu_stats.hit_add_mmap));
	else
		return 0;
}
#endif // PSM_CUDA

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
							uint32_t pri_entries, uint64_t pri_size
#ifdef PSM_CUDA
							, uint64_t gpu_pri_size
#endif
							)
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
	cache->ep = ep;
#ifdef RNDV_MOD
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL
		|| cache->cache_mode == MR_CACHE_MODE_RV) {
		// TBD - could make this a warning and set limit_inuse_bytes=0
		// then depend on transfers queuing and retrying until
		// reg_mr cache space is available
		if ((uint64_t)ep->rv_mr_cache_size*MEGABYTE < pri_size) {
			_HFI_ERROR("PSM3_RV_MR_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
				ep->rv_mr_cache_size, (pri_size + MEGABYTE-1)/MEGABYTE);
			return NULL;
		}
		cache->limit_inuse_bytes = (uint64_t)ep->rv_mr_cache_size*MEGABYTE - pri_size;
#ifdef PSM_CUDA
		if (PSMI_IS_CUDA_ENABLED) {
			// For GPU, due to GdrCopy, we can't undersize cache.
			// Otherwise RDMA MRs could consume all the
			// cache space and leave a gdrcopy pin/mmap stuck
			// retrying indefinitely.  If we want to allow undersize
			// GPU cache, we need to have gdrcopy pin/mmap failures
			// also invoke progress functions to release MRs
			if (__psm2_min_gpu_bar_size()) {
				uint64_t max_recommend = __psm2_min_gpu_bar_size() - 32*MEGABYTE;
				if ((uint64_t)ep->rv_gpu_cache_size*MEGABYTE >= max_recommend) {
					_HFI_INFO("Warning: PSM3_RV_GPU_CACHE_SIZE=%u too large for smallest GPU's BAR size of %"PRIu64" (< %"PRIu64" total of endpoint-rail-qp recommended)\n",
						ep->rv_gpu_cache_size,
						(__psm2_min_gpu_bar_size() + MEGABYTE-1)/MEGABYTE,
						max_recommend/MEGABYTE);
				}
			}
			if ((uint64_t)ep->rv_gpu_cache_size*MEGABYTE < gpu_pri_size) {
				_HFI_ERROR("PSM3_RV_GPU_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
					ep->rv_gpu_cache_size, (gpu_pri_size + MEGABYTE-1)/MEGABYTE);
				return NULL;
			}
			cache->limit_gpu_inuse_bytes = (uint64_t)ep->rv_gpu_cache_size*MEGABYTE - gpu_pri_size;
		}
		_HFI_MMDBG("CPU cache %u GPU cache %u\n", ep->rv_mr_cache_size, ep->rv_gpu_cache_size);
#endif
	} else
#endif // RNDV_MOD
		cache->limit_inuse_bytes = UINT64_MAX;	// no limit, just count inuse
#ifdef RNDV_MOD
	cache->rv = ep->verbs_ep.rv;
	cache->cmd_fd = ep->verbs_ep.context->cmd_fd;
#endif // RNDV_MOD
#if defined(RNDV_MOD) && defined(PSM_CUDA)
	_HFI_MMDBG("cache alloc: max_entries=%u limit_inuse=%u limit_inuse_bytes=%"PRIu64" limit_gpu_inuse_bytes=%"PRIu64", pri_entries=%u pri_size=%"PRIu64" gpu_pri_size=%"PRIu64"\n",
			cache->max_entries, cache->limit_inuse,
			cache->limit_inuse_bytes, cache->limit_gpu_inuse_bytes,
			pri_entries, pri_size, gpu_pri_size);
#else
	_HFI_MMDBG("cache alloc: max_entries=%u limit_inuse=%u limit_inuse_bytes=%"PRIu64", pri_entries=%u pri_size=%"PRIu64"\n",
			cache->max_entries, cache->limit_inuse,
			cache->limit_inuse_bytes, pri_entries, pri_size);
#endif
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
#ifdef RNDV_MOD
#ifdef PSM_CUDA
		PSMI_STATS_DECL("limit_gpu_inuse_bytes",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &cache->limit_gpu_inuse_bytes),
		PSMI_STATS_DECLU64("gpu_inuse_bytes", &cache->gpu_inuse_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_bytes", &cache->max_gpu_inuse_bytes),
#endif
#endif
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
		PSMI_STATS_DECL("rv_hit_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_hit_rate, NULL),
		PSMI_STATS_DECLU64("rv_miss", (uint64_t*)&cache->rv_stats.miss),
		PSMI_STATS_DECL("rv_miss_%", MPSPAWN_STATS_REDUCTION_ALL,
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
					ep->epid, cache, ep->dev_name);
#ifdef PSM_CUDA
#ifdef RNDV_MOD
	struct psmi_stats_entry gpu_entries[] = {
		PSMI_STATS_DECL_FUNC("rv_gpu_size", mr_cache_rv_gpu_size),
		PSMI_STATS_DECL_FUNC("rv_gpu_size_reg", mr_cache_rv_gpu_size_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_size_mmap", mr_cache_rv_gpu_size_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_size_both", mr_cache_rv_gpu_size_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size", mr_cache_rv_gpu_max_size),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size_reg", mr_cache_rv_gpu_max_size_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size_mmap", mr_cache_rv_gpu_max_size_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size_both", mr_cache_rv_gpu_max_size_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_limit", mr_cache_rv_gpu_limit_size),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems", mr_cache_rv_gpu_nelems),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems_reg", mr_cache_rv_gpu_nelems_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems_mmap", mr_cache_rv_gpu_nelems_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems_both", mr_cache_rv_gpu_nelems_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems", mr_cache_rv_gpu_max_nelems),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems_reg", mr_cache_rv_gpu_max_nelems_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems_mmap", mr_cache_rv_gpu_max_nelems_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems_both", mr_cache_rv_gpu_max_nelems_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse", mr_cache_rv_gpu_inuse),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse_reg", mr_cache_rv_gpu_inuse_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse_mmap", mr_cache_rv_gpu_inuse_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse_both", mr_cache_rv_gpu_inuse_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse", mr_cache_rv_gpu_max_inuse),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse_reg", mr_cache_rv_gpu_max_inuse_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse_mmap", mr_cache_rv_gpu_max_inuse_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse_both", mr_cache_rv_gpu_max_inuse_both),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes", (uint64_t*)&cache->rv_gpu_stats.inuse_bytes),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes_reg", (uint64_t*)&cache->rv_gpu_stats.inuse_bytes_reg),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes_mmap", (uint64_t*)&cache->rv_gpu_stats.inuse_bytes_mmap),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes_both", (uint64_t*)&cache->rv_gpu_stats.inuse_bytes_both),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes", (uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes_reg", (uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes_reg),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes_mmap", (uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes_mmap),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes_both", (uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount", mr_cache_rv_gpu_max_refcount),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount_reg", mr_cache_rv_gpu_max_refcount_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount_mmap", mr_cache_rv_gpu_max_refcount_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount_both", mr_cache_rv_gpu_max_refcount_both),
		PSMI_STATS_DECLU64("rv_gpu_hit", (uint64_t*)&cache->rv_gpu_stats.hit),
		PSMI_STATS_DECL("rv_gpu_hit_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_reg", (uint64_t*)&cache->rv_gpu_stats.hit_reg),
		PSMI_STATS_DECL("rv_gpu_hit_reg_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_reg, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_add_reg", (uint64_t*)&cache->rv_gpu_stats.hit_add_reg),
		PSMI_STATS_DECL("rv_gpu_hit_add_reg_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_add_reg, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_mmap", (uint64_t*)&cache->rv_gpu_stats.hit_mmap),
		PSMI_STATS_DECL("rv_gpu_hit_mmap_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_mmap, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_add_mmap", (uint64_t*)&cache->rv_gpu_stats.hit_add_mmap),
		PSMI_STATS_DECL("rv_gpu_hit_add_mmap_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_add_mmap, NULL),
		PSMI_STATS_DECLU64("rv_gpu_miss", (uint64_t*)&cache->rv_gpu_stats.miss),
		PSMI_STATS_DECL("rv_gpu_miss_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_miss_rate, NULL),
		PSMI_STATS_DECLU64("rv_gpu_miss_reg", (uint64_t*)&cache->rv_gpu_stats.miss_reg),
		PSMI_STATS_DECL("rv_gpu_miss_reg_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_miss_rate_reg, NULL),
		PSMI_STATS_DECLU64("rv_gpu_miss_mmap", (uint64_t*)&cache->rv_gpu_stats.miss_mmap),
		PSMI_STATS_DECL("rv_gpu_miss_mmap_%", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_miss_rate_mmap, NULL),
		PSMI_STATS_DECLU64("rv_gpu_full", (uint64_t*)&cache->rv_gpu_stats.full),
		PSMI_STATS_DECLU64("rv_gpu_full_reg", (uint64_t*)&cache->rv_gpu_stats.full_reg),
		PSMI_STATS_DECLU64("rv_gpu_full_mmap", (uint64_t*)&cache->rv_gpu_stats.full_mmap),
		PSMI_STATS_DECLU64("rv_gpu_failed_pin", (uint64_t*)&cache->rv_gpu_stats.failed_pin),
		PSMI_STATS_DECLU64("rv_gpu_failed_reg", (uint64_t*)&cache->rv_gpu_stats.failed_reg),
		PSMI_STATS_DECLU64("rv_gpu_failed_mmap", (uint64_t*)&cache->rv_gpu_stats.failed_mmap),
		PSMI_STATS_DECLU64("rv_gpu_remove", (uint64_t*)&cache->rv_gpu_stats.remove),
		PSMI_STATS_DECLU64("rv_gpu_remove_reg", (uint64_t*)&cache->rv_gpu_stats.remove_reg),
		PSMI_STATS_DECLU64("rv_gpu_remove_mmap", (uint64_t*)&cache->rv_gpu_stats.remove_mmap),
		PSMI_STATS_DECLU64("rv_gpu_remove_both", (uint64_t*)&cache->rv_gpu_stats.remove_both),
		PSMI_STATS_DECLU64("rv_gpu_evict", (uint64_t*)&cache->rv_gpu_stats.evict),
		PSMI_STATS_DECLU64("rv_gpu_evict_reg", (uint64_t*)&cache->rv_gpu_stats.evict_reg),
		PSMI_STATS_DECLU64("rv_gpu_evict_mmap", (uint64_t*)&cache->rv_gpu_stats.evict_mmap),
		PSMI_STATS_DECLU64("rv_gpu_evict_both", (uint64_t*)&cache->rv_gpu_stats.evict_both),
		PSMI_STATS_DECLU64("rv_gpu_inval_mr", (uint64_t*)&cache->rv_gpu_stats.inval_mr),
		PSMI_STATS_DECLU64("rv_post_write", (uint64_t*)&cache->rv_gpu_stats.post_write),
		PSMI_STATS_DECLU64("rv_post_write_bytes", (uint64_t*)&cache->rv_gpu_stats.post_write_bytes),
		PSMI_STATS_DECLU64("rv_gpu_post_write", (uint64_t*)&cache->rv_gpu_stats.gpu_post_write),
		PSMI_STATS_DECLU64("rv_gpu_post_write_bytes", (uint64_t*)&cache->rv_gpu_stats.gpu_post_write_bytes),
	};
	if (cache->rv && PSMI_IS_CUDA_ENABLED)
		psmi_stats_register_type("MR_GPU_Cache_Statistics",
					PSMI_STATSTYPE_MR_CACHE,
					gpu_entries,
					PSMI_STATS_HOWMANY(gpu_entries),
					ep->epid, &cache->rv_gpu_stats,
					ep->dev_name);
#endif
#endif

	return cache;
}

int psm2_verbs_mr_cache_allows_user_mr(psm2_mr_cache_t cache)
{
	if (!cache)
		return 0;
	switch (cache->cache_mode) {
		case MR_CACHE_MODE_NONE:
			return 0;
		case MR_CACHE_MODE_KERNEL:
			return psmi_hal_has_cap(PSM_HAL_CAP_USER_MR);
		case MR_CACHE_MODE_USER:
			return 1;
		case MR_CACHE_MODE_RV:
			return psmi_hal_has_cap(PSM_HAL_CAP_USER_MR);
		default:	// unexpected
			return 0;
	}
}

static void update_stats_inc_inuse(psm2_mr_cache_t cache, uint64_t length,
					int access)
{
	INC_STAT(cache, inuse, max_inuse);
#ifdef RNDV_MOD
#ifdef PSM_CUDA
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		ADD_STAT(cache, length, gpu_inuse_bytes, max_gpu_inuse_bytes);
	else
#endif
#endif
		ADD_STAT(cache, length, inuse_bytes, max_inuse_bytes);
}

static void update_stats_dec_inuse(psm2_mr_cache_t cache, uint64_t length,
					int access)
{
	cache->inuse--;
#ifdef RNDV_MOD
#ifdef PSM_CUDA
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		cache->gpu_inuse_bytes -= length;
	else
#endif
#endif
		cache->inuse_bytes -= length;
}

// checks for space for a non-priority registration
static inline int have_space(psm2_mr_cache_t cache, uint64_t length, int access)
{
#ifdef RNDV_MOD
#ifdef PSM_CUDA
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		return (cache->inuse < cache->limit_inuse
			&& cache->gpu_inuse_bytes + length < cache->limit_gpu_inuse_bytes);
	else
#endif
#endif
		return (cache->inuse < cache->limit_inuse
			&& cache->inuse_bytes + length < cache->limit_inuse_bytes);
}

#ifdef PSM_CUDA
#ifdef RNDV_MOD
// given an ep this returns the "next one".
// It loops through all the multi-rail/multi-QP EPs in a given user opened EP
// 1st, then it goes to the next user opened EP (multi-EP) and loops through
// it's multi-rail/mult-QP EPs.
// When it hits the last rail of the last user opened EP, it goes back to
// the 1st rail of the 1st user opened EP.
// caller must hold creation_lock
static psm2_ep_t next_ep(psm2_ep_t ep)
{
       //mctxt_next is the circular list of rails/QPs in a given user EP
       //mctxt_master is the 1st in the list, when we get back to the 1st
       //go to the next user EP
       ep = ep->mctxt_next;
       if (ep->mctxt_master != ep)
               return ep;
       //user_ep_next is a linked list of user opened EPs.  End of list is NULL
       //when hit end of list, go back to 1st (psmi_opened_endpoint)
       //for each user opened EP, the entry on this list is the 1st rail within
       //the EP
       ep = ep->user_ep_next;
       if (ep)
               return ep;
       else
               return psmi_opened_endpoint;
}

// determine if ep is still valid (can't dereference or trust ep given)
// caller must hold creation_lock
static int valid_ep(psm2_ep_t ep)
{
	psm2_ep_t e1 = psmi_opened_endpoint;

	while (e1) {	// user opened ep's - linear list ending in NULL
		psm2_ep_t e2 = e1;
		//check mtcxt list (multi-rail within user opened ep)
		do {
			if (e2 == ep)
				return 1;
			e2 = e2->mctxt_next;
		} while (e2 != e1);	// circular list
		e1 = e1->user_ep_next;
	}
	return 0;	// not found
}

// advance ep to the next.  However it's possible ep is stale and
// now closed/freed, so make sure it's good.  good_ep is at least one
// known good_ep and lets us avoid search some of the time (or if only 1 EP)
// caller must hold creation_lock
static psm2_ep_t next_valid_ep(psm2_ep_t ep, psm2_ep_t good_ep)
{
	if (ep == good_ep || valid_ep(ep))
		return next_ep(ep);
	else
		return good_ep;
}

/*
 * Evict some space in given cache (only GPU needs this)
 * If nvidia_p2p_get_pages reports out of BAR space (perhaps prematurely),
 * we need to evict from other EPs too.
 * So we rotate among all eps (rails or QPs) in our user opened EP for eviction.
 * length - amount attempted in pin/register which just failed
 * access - indicates if IS_GPU_ADDR or not (rest ignored)
 * returns:
 * 	>0 bytes evicted if some evicted
 * 	-1 if nothing evicted (errno == ENOENT means nothing evictable found)
 * 	ENOENT also used when access is not for GPU
 * The caller will have the progress_lock, we need the creation_lock
 * to walk the list of EPs outside our own MQ.  However creation_lock
 * is above the progress_lock in lock heirarchy, so we use a LOCK_TRY
 * to avoid deadlock in the rare case where another thread
 * has creation_lock and is trying to get progress_lock (such as during
 * open_ep, close_ep or rcvthread).
 */
int64_t psm2_verbs_evict_some(psm2_ep_t ep, uint64_t length, int access)
{
	static __thread psm2_ep_t last_evict_ep;	// among all eps
	static __thread psm2_ep_t last_evict_myuser_ep;	// in my user ep
	int64_t evicted = 0;
	int ret;

	if (! (access & IBV_ACCESS_IS_GPU_ADDR)) {
		errno = ENOENT;
		return -1;	// only need evictions on GPU addresses
	}
	if (! last_evict_ep) {	// first call only
		last_evict_ep = ep;
		last_evict_myuser_ep = ep;
	}
	// 1st try to evict from 1st rail/QP in our opened EP (gdrcopy and MRs)
	ret = __psm2_rv_evict_gpu_amount(ep->mctxt_master->verbs_ep.rv, max(gpu_cache_evict, length), 0);
	if (ret > 0)
		evicted = ret;

	// next rotate among other rails/QPs in our opened ep (MRs)
	last_evict_myuser_ep = last_evict_myuser_ep->mctxt_next;
	if (last_evict_myuser_ep != ep->mctxt_master) {
		ret = __psm2_rv_evict_gpu_amount(last_evict_myuser_ep->verbs_ep.rv, max(gpu_cache_evict, length), 0);
		if (ret > 0)
			evicted += ret;
	}
	if (evicted >= length)
		return evicted;

	// now try other opened EPs
	if (PSMI_LOCK_TRY(psmi_creation_lock))
		goto done;
	// last_evict_ep could point to an ep which has since been closed/freed
 	last_evict_ep = next_valid_ep(last_evict_ep, ep);
	if (last_evict_ep->mctxt_master != ep->mctxt_master) {
		if (!PSMI_LOCK_TRY(last_evict_ep->mq->progress_lock)) {
			ret = __psm2_rv_evict_gpu_amount(last_evict_ep->verbs_ep.rv, max(gpu_cache_evict, length), 0);
			PSMI_UNLOCK(last_evict_ep->mq->progress_lock);
			if (ret > 0)
				evicted += ret;
		}
	} else {
		ret = __psm2_rv_evict_gpu_amount(last_evict_ep->verbs_ep.rv, max(gpu_cache_evict, length), 0);
		if (ret > 0 )
			evicted += ret;
	}
	PSMI_UNLOCK(psmi_creation_lock);
done:
	if (! evicted) {
		errno = ENOENT;
		return -1;
	}
	return evicted;
}
#endif
#endif

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
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_reg_mr, "")) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(!priority && PSMI_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_nonpri_reg_mr, "nonpri_reg_mr",
				"MR cache full, non-priority request",
				1, IPS_FAULTINJ_NONPRI_REG_MR);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_nonpri_reg_mr, "")) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(priority && PSMI_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_pri_reg_mr, "pri_reg_mr",
				"MR cache full, priority request",
				1, IPS_FAULTINJ_PRI_REG_MR);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_pri_reg_mr, "")) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
#endif // PSM_FI
	access |= IBV_ACCESS_LOCAL_WRITE;	// manditory flag
#ifndef RNDV_MOD
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
		_HFI_ERROR("unsupported GPU memory registration\n");
		cache->failed++;
		errno = EINVAL;
		return NULL;
	}
#else
#ifdef PSM_CUDA
	psmi_assert(!!(access & IBV_ACCESS_IS_GPU_ADDR) == (PSMI_IS_CUDA_ENABLED && PSMI_IS_CUDA_MEM(addr)));
#endif
#endif
	struct psm2_verbs_mr key = { // our search key
		.addr = addr,
		.length = length,
		.access = access
	};
	// for user QPs, can share entries with send DMA and send RDMA
#ifdef RNDV_MOD
	if (cache->cache_mode != MR_CACHE_MODE_RV)
		key.access &= ~IBV_ACCESS_RDMA;
#else
	key.access &= ~IBV_ACCESS_RDMA;
#endif

	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, &key);
	if (p_item->payload.mr.mr_ptr) {
		psmi_assert(p_item != cache->map.nil_item);
		mrc = &p_item->payload;
		if (! mrc->refcount) {
			if (! priority && ! have_space(cache, length, access)) {
				_HFI_MMDBG("cache has no headroom for non-priority hit addr %p len %"PRIu64" access 0x%x ptr %p\n",
						addr, length, access, mrc);
				cache->rejected++;
				errno = ENOMEM;
				return NULL;
			}
			// it was an entry on avail_list, take off list
			TAILQ_REMOVE(&cache->avail_list, mrc, next);
			update_stats_inc_inuse(cache, length, access);
		}
		cache->hit++;
		mrc->refcount++;
		_HFI_MMDBG("cache hit MR addr %p len %"PRIu64" access 0x%x ref %d ptr %p\n",
			addr, length, access, mrc->refcount, mrc);
		cache->max_refcount = max(cache->max_refcount, mrc->refcount);
		return mrc;
	}
	psmi_assert(p_item == cache->map.nil_item);
	if (! priority && ! have_space(cache, length, access)) {
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
					addr, length, access, mrc);
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
			_HFI_ERROR("unexpected dreg_mr failure: %s\n", strerror(errno));
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
		// user space QPs for everything, drop IBV_ACCESS_RDMA flag
		mrc->mr.rv_mr = __psm2_rv_reg_mem(cache->rv, cache->cmd_fd, pd, addr, length, access & ~IBV_ACCESS_RDMA);
		if (! mrc->mr.rv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
#ifdef PSM_CUDA
				if (priority)
					(void)psm2_verbs_evict_some(cache->ep, length, access);
#endif
			} else {
				_HFI_ERROR("reg_mr failed; %s acc 0x%x\n", strerror(errno), access);
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
		// kernel QP for RDMA, user QP for send DMA
		mrc->mr.rv_mr = __psm2_rv_reg_mem(cache->rv, cache->cmd_fd, (access&IBV_ACCESS_RDMA)?NULL:pd, addr, length, access);
		if (! mrc->mr.rv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
#ifdef PSM_CUDA
				if (priority)
					(void)psm2_verbs_evict_some(cache->ep, length, access);
#endif
			} else {
				_HFI_ERROR("reg_mr failed; %s acc 0x%x\n", strerror(errno), access);
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
		// user space QPs for everything, drop IBV_ACCESS_RDMA flag
		mrc->mr.ibv_mr = ibv_reg_mr(pd, addr, length, access & ~IBV_ACCESS_RDMA);
		if (! mrc->mr.ibv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
			} else {
				_HFI_ERROR("reg_mr failed; %s acc 0x%x\n", strerror(errno), access);
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
#ifdef RNDV_MOD
	if (cache->cache_mode != MR_CACHE_MODE_RV)
		mrc->access = access & ~IBV_ACCESS_RDMA;
	else
		mrc->access = access;
#else
	mrc->access = access & ~IBV_ACCESS_RDMA;
#endif
	ips_cl_qmap_insert_item(&cache->map, p_item);
	update_stats_inc_inuse(cache, length, access);
	_HFI_MMDBG("registered new MR pri %d addr %p len %"PRIu64" access 0x%x ref %u ptr %p nelems %u\n",
		priority, addr, length, access, mrc->refcount, mrc, cache->map.payload.nelems);
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
	_HFI_MMDBG("releasing MR addr %p len %"PRIu64" access 0x%x ref %u-- ptr %p\n",
		mrc->addr, mrc->length, mrc->access, mrc->refcount, mrc);
	mrc->refcount--;
	if (!mrc->refcount) {
		if (mrc->cache->cache_mode == MR_CACHE_MODE_USER) {
			// if refcount now zero, put on avail_list to be reclaimed if needed
			update_stats_dec_inuse(mrc->cache, mrc->length, mrc->access);
			TAILQ_INSERT_TAIL(&mrc->cache->avail_list, mrc, next);
		} else {
			_HFI_MMDBG("freeing MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p nelems %u\n",
				mrc->addr, mrc->length, mrc->access, mrc->refcount, mrc,
				mrc->cache->map.payload.nelems);
			update_stats_dec_inuse(mrc->cache, mrc->length, mrc->access);
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
				_HFI_ERROR("unexpected dreg_mr failure on %s: %s\n", mrc->cache->ep->dev_name, strerror(errno));
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
#ifdef PSM_CUDA
#ifdef RNDV_MOD
	if (cache->rv && PSMI_IS_CUDA_ENABLED)
		psmi_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE,
					&cache->rv_gpu_stats);
#endif
#endif
	psmi_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE, cache);
	while (cache->map.payload.nelems) {
		cl_map_item_t *p_item = __cl_map_root(&cache->map);
		psmi_assert(p_item != cache->map.nil_item);
		psm2_verbs_mr_t mrc = &p_item->payload;
		psmi_assert(mrc->mr.mr_ptr);
		if (mrc->mr.mr_ptr) {
			int ret;
			_HFI_MMDBG("free MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p\n",
				mrc->addr, mrc->length, mrc->access, mrc->refcount, mrc);
			if (mrc->refcount) {
				_HFI_ERROR("unreleased MR in psm2_verbs_free_mr_cache addr %p len %"PRIu64" access 0x%x\n",
					mrc->addr, mrc->length, mrc->access);
				return; // leak the rest, let process exit cleanup
			}
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
				_HFI_ERROR("unexpected dreg_mr failure on %s: %s\n", mrc->cache->ep->dev_name, strerror(errno));
			mrc->mr.mr_ptr = NULL;
			psmi_mpool_put(p_item);
		}
	}
	psmi_assert(TAILQ_EMPTY(&cache->avail_list));
	psmi_assert(! cache->map.payload.nelems);

	psmi_mpool_destroy(cache->mr_pool);
	psmi_free(cache);
}
