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

// cache_mode MR_CACHE_MODE_USER_NOINVAL, is a PoC and overly simple.
// This approach is only viable for
// some microbenchmarks and simple apps.  For more complex apps the lack of
// invalidate hooks into memory free may lead to memory corruption.
// However such hooks are not reliably possible until the 4.17+ kernels.
// The kernel RV module hooks into mmu_notfiers for invalidate.  These are also
// used by hypervisors and hence are complete and reliable.

#include <sys/types.h>
#include "psm_user.h"	// pulls in hal_verbs/verbs_ep.h and psm_verbs_mr.h
#include "psm2_hal.h"
#ifdef PSM_FI
#include "ips_config.h"
#endif
#ifdef UMR_CACHE
#include "psm_mq_internal.h"
#endif

#ifdef PSM_HAVE_REG_MR

//#undef _HFI_MMDBG
//#define _HFI_MMDBG printf

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
#define RBTREE_MI_PL  struct psm3_verbs_mr
#define RBTREE_MAP_PL struct psm2_mr_cache_map_pl
#include "psm3_rbtree.h"

struct psm2_mr_cache {
	uint32_t max_entries;
	// limits to allow headroom for priority registrations
	uint32_t limit_inuse;
	uint64_t limit_inuse_bytes;
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t limit_gpu_inuse_bytes;
#endif
	psm3_rv_t rv;
	int cmd_fd;
#endif
	struct ibv_pd *pd;
	psm2_ep_t ep;
	uint8_t cache_mode;	// MR_CACHE_MODE_*
	int access;	// value to OR into access on all reg_mr
	cl_qmap_t map;
	cl_map_item_t root;
	cl_map_item_t nil_item;
	// Below is for queue of cache entries available for reuse (refcount==0)
	// only used when cache_mode==MR_CACHE_MODE_USER*
	// Available entries are added at end of list and reused from start.
	// Hence having aging of cached entries.
	// Aging helps reduce some of the corruption risk,
	// but is not a full solution.  Good enough for the PoC
	TAILQ_HEAD(avail_list, psm3_verbs_mr) avail_list;
	mpool_t mr_pool;	// pool of MRs
	// some statistics for user space
	uint64_t hit;
	uint64_t miss;
	uint64_t rejected;		// rejected non-priority registration
	uint64_t full;			// failed registration (tends to be priority)
	uint64_t full_pri;
	uint64_t full_pri_recv;
	uint64_t full_pri_send;
	uint64_t full_nonpri;
	uint64_t full_nonpri_recv;
	uint64_t full_nonpri_send;
	uint64_t failed;		// other failures, should be none
	uint32_t inuse;		// entry count in use
	uint32_t max_inuse;
	uint64_t inuse_bytes;
	uint64_t max_inuse_bytes;
	uint32_t inuse_recv;		// entry count in use
	uint32_t max_inuse_recv;
	uint64_t inuse_recv_bytes;
	uint64_t max_inuse_recv_bytes;
	uint32_t inuse_send;		// entry count in use
	uint32_t max_inuse_send;
	uint64_t inuse_send_bytes;
	uint64_t max_inuse_send_bytes;
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	uint64_t gpu_inuse_bytes;
	uint64_t max_gpu_inuse_bytes;
	uint64_t gpu_inuse_recv_bytes;
	uint64_t max_gpu_inuse_recv_bytes;
	uint64_t gpu_inuse_send_bytes;
	uint64_t max_gpu_inuse_send_bytes;
#endif
#endif
	uint32_t max_nelems;
	uint32_t max_refcount;
#ifdef PSM_HAVE_RNDV_MOD
	struct psm3_rv_cache_stats rv_stats;	// statistics from rv module
									// will remain 0 if rv not open
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	struct psm3_rv_gpu_cache_stats rv_gpu_stats;	// GPU statistics from rv module
									// will remain 0 if rv not open
#endif
#endif
#ifdef UMR_CACHE
	struct psm2_umr_cache umr_cache;
	TAILQ_HEAD(umrc_event_list, psm2_umrc_event) umrc_event_list;
#endif // UMR_CACHE
};

static int mr_cache_key_cmp(const struct psm3_verbs_mr *a,
							const struct psm3_verbs_mr *b)
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
#ifndef UMR_CACHE
#define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR
#endif
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
CACHE_STAT_FUNC(mr_cache_access, ep->mr_access)
CACHE_STAT_FUNC(mr_cache_max_entries, max_entries)
CACHE_STAT_FUNC(mr_cache_nelems, map.payload.nelems)
CACHE_STAT_FUNC(mr_cache_max_nelems, max_nelems)
CACHE_STAT_FUNC(mr_cache_limit_inuse, limit_inuse)
CACHE_STAT_FUNC(mr_cache_inuse, inuse)
CACHE_STAT_FUNC(mr_cache_max_inuse, max_inuse)
CACHE_STAT_FUNC(mr_cache_inuse_recv, inuse_recv)
CACHE_STAT_FUNC(mr_cache_max_inuse_recv, max_inuse_recv)
CACHE_STAT_FUNC(mr_cache_inuse_send, inuse_send)
CACHE_STAT_FUNC(mr_cache_max_inuse_send, max_inuse_send)
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

#ifdef PSM_HAVE_RNDV_MOD
static uint64_t mr_cache_rv_size(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	if (cache->rv) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		(void)psm3_rv_get_cache_stats(cache->rv, &cache->rv_stats);
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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
static uint64_t mr_cache_rv_gpu_size(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	if (cache->rv && PSMI_IS_GPU_ENABLED ) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		(void)psm3_rv_gpu_get_cache_stats(cache->rv, &cache->rv_gpu_stats);
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
#endif // PSM_CUDA || PSM_ONEAPI

#endif // PSM_HAVE_RNDV_MOD

#ifdef UMR_CACHE
static int psm3_verbs_umrc_prepare(psm2_ep_t ep, psm2_mr_cache_t cache);
static void psm3_verbs_umrc_event_queue_process(psm2_mr_cache_t cache);
#endif

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
psm2_mr_cache_t psm3_verbs_alloc_mr_cache(psm2_ep_t ep,
							uint32_t max_entries, uint8_t cache_mode,
							uint32_t pri_entries, uint64_t pri_size
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
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
	cache->access = ep->mr_access?IBV_ACCESS_REMOTE_WRITE:0;
	// we leave headroom for priority registrations
	cache->limit_inuse = max_entries - pri_entries;
	cache->ep = ep;
#ifdef PSM_HAVE_RNDV_MOD
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL
		|| cache->cache_mode == MR_CACHE_MODE_RV) {
		// TBD - could make this a warning and set limit_inuse_bytes=0
		// then depend on transfers queuing and retrying until
		// reg_mr cache space is available
		if ((uint64_t)ep->rv_mr_cache_size*MEGABYTE < pri_size) {
			_HFI_ERROR("PSM3_RV_MR_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
				ep->rv_mr_cache_size, (pri_size + MEGABYTE-1)/MEGABYTE);
			psmi_free(cache);
			return NULL;
		}
		cache->limit_inuse_bytes = (uint64_t)ep->rv_mr_cache_size*MEGABYTE - pri_size;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (PSMI_IS_GPU_ENABLED) {
			// For GPU, due to GdrCopy, we can't undersize cache.
			// Otherwise RDMA MRs could consume all the
			// cache space and leave a gdrcopy pin/mmap stuck
			// retrying indefinitely.  If we want to allow undersize
			// GPU cache, we need to have gdrcopy pin/mmap failures
			// also invoke progress functions to release MRs
			if (psm3_min_gpu_bar_size()) {
				uint64_t max_recommend = psm3_min_gpu_bar_size() - 32*MEGABYTE;
				if ((uint64_t)ep->rv_gpu_cache_size*MEGABYTE >= max_recommend) {
					_HFI_INFO("Warning: PSM3_RV_GPU_CACHE_SIZE=%u too large for smallest GPU's BAR size of %"PRIu64" (< %"PRIu64" total of endpoint-rail-qp recommended)\n",
						ep->rv_gpu_cache_size,
						(psm3_min_gpu_bar_size() + MEGABYTE-1)/MEGABYTE,
						max_recommend/MEGABYTE);
				}
			}
			if ((uint64_t)ep->rv_gpu_cache_size*MEGABYTE < gpu_pri_size) {
				_HFI_ERROR("PSM3_RV_GPU_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
					ep->rv_gpu_cache_size, (gpu_pri_size + MEGABYTE-1)/MEGABYTE);
				psmi_free(cache);
				return NULL;
			}
			cache->limit_gpu_inuse_bytes = (uint64_t)ep->rv_gpu_cache_size*MEGABYTE - gpu_pri_size;
		}
		_HFI_MMDBG("CPU cache %u GPU cache %u\n", ep->rv_mr_cache_size, ep->rv_gpu_cache_size);
#endif /* PSM_CUDA || PSM_ONEAPI */
	} else
#endif // PSM_HAVE_RNDV_MOD
		cache->limit_inuse_bytes = UINT64_MAX;	// no limit, just count inuse
#ifdef PSM_HAVE_RNDV_MOD
	cache->rv = ep->rv;
	cache->cmd_fd = ep->cmd_fd;
#endif // PSM_HAVE_RNDV_MOD
	cache->pd = ep->pd;
#ifdef UMR_CACHE
	if (ep->mr_cache_mode == MR_CACHE_MODE_USER) {
		if (psm3_verbs_umrc_prepare(ep, cache) != PSM2_OK)
			return NULL;
	}
#endif
#if defined(PSM_HAVE_RNDV_MOD) && (defined(PSM_CUDA) || defined(PSM_ONEAPI))
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
	cache->mr_pool = psm3_mpool_create(sizeof(cl_map_item_t),
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
		PSMI_STATS_DECL("mr_access", MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_access, NULL),
		PSMI_STATS_DECLU64("inuse_bytes", &cache->inuse_bytes),
		PSMI_STATS_DECL_FUNC("limit_entries", mr_cache_max_entries),
		PSMI_STATS_DECL_FUNC("nelems", mr_cache_nelems),
		PSMI_STATS_DECL_FUNC("max_nelems", mr_cache_max_nelems),
		PSMI_STATS_DECL("limit_inuse",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_limit_inuse, NULL),
		PSMI_STATS_DECL_FUNC("inuse", mr_cache_inuse),
		PSMI_STATS_DECL_FUNC("max_inuse", mr_cache_max_inuse),
		// if cache->access recv and send inuse stats will remain 0
		PSMI_STATS_DECL_FUNC("inuse_recv", mr_cache_inuse_recv),
		PSMI_STATS_DECL_FUNC("max_inuse_recv", mr_cache_max_inuse_recv),
		PSMI_STATS_DECL_FUNC("inuse_send", mr_cache_inuse_send),
		PSMI_STATS_DECL_FUNC("max_inuse_send", mr_cache_max_inuse_send),
		PSMI_STATS_DECL("limit_inuse_bytes",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &cache->limit_inuse_bytes),
		PSMI_STATS_DECLU64("inuse_bytes", &cache->inuse_bytes),
		PSMI_STATS_DECLU64("max_inuse_bytes", &cache->max_inuse_bytes),
		// if cache->access recv and send inuse stats will remain 0
		PSMI_STATS_DECLU64("inuse_recv_bytes", &cache->inuse_recv_bytes),
		PSMI_STATS_DECLU64("max_inuse_recv_bytes", &cache->max_inuse_recv_bytes),
		PSMI_STATS_DECLU64("inuse_send_bytes", &cache->inuse_send_bytes),
		PSMI_STATS_DECLU64("max_inuse_send_bytes", &cache->max_inuse_send_bytes),
#ifdef UMR_CACHE
		PSMI_STATS_DECLU64("umrc_evict", &cache->umr_cache.stats.evict),
		PSMI_STATS_DECLU64("umrc_uffd_remove", &cache->umr_cache.stats.remove),
		PSMI_STATS_DECLU64("umrc_uffd_unmap", &cache->umr_cache.stats.unmap),
		PSMI_STATS_DECLU64("umrc_uffd_remap", &cache->umr_cache.stats.remap),
#endif
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		PSMI_STATS_DECL("limit_gpu_inuse_bytes",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &cache->limit_gpu_inuse_bytes),
		PSMI_STATS_DECLU64("gpu_inuse_bytes", &cache->gpu_inuse_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_bytes", &cache->max_gpu_inuse_bytes),
		// if cache->access recv and send inuse stats will remain 0
		PSMI_STATS_DECLU64("gpu_inuse_recv_bytes", &cache->gpu_inuse_recv_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_recv_bytes", &cache->max_gpu_inuse_recv_bytes),
		PSMI_STATS_DECLU64("gpu_inuse_send_bytes", &cache->gpu_inuse_send_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_send_bytes", &cache->max_gpu_inuse_send_bytes),
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
		// if cache->access recv and send full stats will remain 0
		// otherwise full_pri and nonpri remain 0
		PSMI_STATS_DECLU64("full_pri", &cache->full_pri),
		PSMI_STATS_DECLU64("full_pri_recv", &cache->full_pri_recv),
		PSMI_STATS_DECLU64("full_pri_send", &cache->full_pri_send),
		PSMI_STATS_DECLU64("full_nonpri", &cache->full_nonpri),
		PSMI_STATS_DECLU64("full_nonpri_recv", &cache->full_nonpri_recv),
		PSMI_STATS_DECLU64("full_nonpri_send", &cache->full_nonpri_send),
		PSMI_STATS_DECLU64("failed", &cache->failed),
#ifdef PSM_HAVE_RNDV_MOD
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
#endif // PSM_HAVE_RNDV_MOD
	};
	psm3_stats_register_type("MR_Cache_Statistics",
					PSMI_STATSTYPE_MR_CACHE,
					entries,
					PSMI_HOWMANY(entries),
					psm3_epid_fmt_internal(ep->epid, 0), cache, ep->dev_name);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#ifdef PSM_HAVE_RNDV_MOD
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
	if (cache->rv && PSMI_IS_GPU_ENABLED && ep->rv_gpu_cache_size)
		psm3_stats_register_type("MR_GPU_Cache_Statistics",
					PSMI_STATSTYPE_MR_CACHE,
					gpu_entries,
					PSMI_HOWMANY(gpu_entries),
					psm3_epid_fmt_internal(ep->epid, 0), &cache->rv_gpu_stats,
					ep->dev_name);
#endif /* PSM_HAVE_RNDV_MOD */
#endif /* PSM_CUDA || PSM_ONEAPI */

	return cache;
}

int psm3_verbs_mr_cache_allows_user_mr(psm2_mr_cache_t cache)
{
	if (!cache)
		return 0;
	switch (cache->cache_mode) {
		case MR_CACHE_MODE_NONE:
			return 0;
		case MR_CACHE_MODE_KERNEL:
			return psmi_hal_has_cap(PSM_HAL_CAP_USER_MR);
		case MR_CACHE_MODE_USER_NOINVAL:
#ifdef UMR_CACHE
		case MR_CACHE_MODE_USER:
#endif
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
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		ADD_STAT(cache, length, gpu_inuse_bytes, max_gpu_inuse_bytes);
	else
#endif
#endif
		ADD_STAT(cache, length, inuse_bytes, max_inuse_bytes);
	if (cache->access)	// can't distinguish recv vs send
		return;
	if (access & IBV_ACCESS_REMOTE_WRITE) {
		INC_STAT(cache, inuse_recv, max_inuse_recv);
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			ADD_STAT(cache, length, gpu_inuse_recv_bytes, max_gpu_inuse_recv_bytes);
		else
#endif
#endif
			ADD_STAT(cache, length, inuse_recv_bytes, max_inuse_recv_bytes);
	} else {
		INC_STAT(cache, inuse_send, max_inuse_send);
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			ADD_STAT(cache, length, gpu_inuse_send_bytes, max_gpu_inuse_send_bytes);
		else
#endif
#endif
			ADD_STAT(cache, length, inuse_send_bytes, max_inuse_send_bytes);
	}
}

static void update_stats_dec_inuse(psm2_mr_cache_t cache, uint64_t length,
					int access)
{
	cache->inuse--;
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		cache->gpu_inuse_bytes -= length;
	else
#endif
#endif
		cache->inuse_bytes -= length;
	if (cache->access)	// can't distinguish recv vs send
		return;
	if (access & IBV_ACCESS_REMOTE_WRITE) {
		cache->inuse_recv--;
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			cache->gpu_inuse_recv_bytes -= length;
		else
#endif
#endif
			cache->inuse_recv_bytes -= length;
	} else {
		cache->inuse_send--;
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			cache->gpu_inuse_send_bytes -= length;
		else
#endif
#endif
			cache->inuse_send_bytes -= length;
	}
}

// checks for space for a non-priority registration
static inline int have_space(psm2_mr_cache_t cache, uint64_t length, int access)
{
#ifdef PSM_HAVE_RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		return (cache->inuse < cache->limit_inuse
			&& cache->gpu_inuse_bytes + length < cache->limit_gpu_inuse_bytes);
	else
#endif
#endif
		return (cache->inuse < cache->limit_inuse
			&& cache->inuse_bytes + length < cache->limit_inuse_bytes);
}

// each attempt will increment exactly one of: hit, miss, rejected, full, failed
struct psm3_verbs_mr * psm3_verbs_reg_mr(psm2_mr_cache_t cache, bool priority,
				void *addr, uint64_t length, int access)
{
	psm3_verbs_mr_t mrc;
#ifdef PSM_ONEAPI
	void *base;
	size_t len;
#endif

	psmi_assert(cache->pd);
	if (! cache->pd)
		return NULL;

#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_reg_mr, "reg_mr",
				"MR cache full, any request type",
				1, IPS_FAULTINJ_REG_MR);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_reg_mr, cache->ep, "")) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(!priority && PSM3_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_nonpri_reg_mr, "nonpri_reg_mr",
				"MR cache full, non-priority request",
				1, IPS_FAULTINJ_NONPRI_REG_MR);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_nonpri_reg_mr, cache->ep, "")) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(priority && PSM3_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_pri_reg_mr, "pri_reg_mr",
				"MR cache full, priority request",
				1, IPS_FAULTINJ_PRI_REG_MR);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_pri_reg_mr, cache->ep, "")) {
			cache->failed++;
			errno = ENOMEM;
			return NULL;
		}
	}
#endif // PSM_FI
	access |= IBV_ACCESS_LOCAL_WRITE|cache->access; // manditory flag
#ifdef PSM_HAVE_RNDV_MOD
	// drop IBV_ACCESS_RDMA flag if all MRs are for user space QP use.
	// (this flag is IBV_ACCESS_KERNEL for builds enabled with RNDV_MOD,
	// 0 otherwise)
	if (cache->cache_mode != MR_CACHE_MODE_RV)
		access &= ~IBV_ACCESS_RDMA;
#else
	psmi_assert(IBV_ACCESS_RDMA == 0);
#endif
#ifndef PSM_HAVE_RNDV_MOD
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
		_HFI_ERROR("unsupported GPU memory registration\n");
		cache->failed++;
		errno = EINVAL;
		return NULL;
	}
#else
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	psmi_assert(!!(access & IBV_ACCESS_IS_GPU_ADDR) == (PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(addr)));
#ifdef PSM_ONEAPI
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
#define MAX_USER_MR_SIZE (32 * 1024)
		PSMI_ONEAPI_ZE_CALL(zeMemGetAddressRange, ze_context,
				    (const void *)addr, &base, &len);
		/*
		 * Need to register MR with base address and total length.
		 * However, for Mellanox cards, the max buffer size for a
		 * user MR registered through the rv module is 32k bytes.
		 * Otherwise, it will fail with IB_WC_MW_BIND_ERR.
		 */
		if (strncasecmp(cache->ep->dev_name, "mlx5_0", 3) == 0 &&
		    !(access & IBV_ACCESS_KERNEL) && len > MAX_USER_MR_SIZE) {
			/* Change only if the buffer stays in the first 32k */
			if (((char *)addr + length) <= ((char *)base + MAX_USER_MR_SIZE)) {
				addr = base;
				length = MAX_USER_MR_SIZE;
			}
		} else {
			/* Kernel MR or user MR with total size <= MAX_USER_MR_SIZE */
			addr = base;
			length = len;
		}
	}
#endif
#endif
#endif
	struct psm3_verbs_mr key = { // our search key
		.addr = addr,
		.length = length,
		.access = access
	};

#ifdef UMR_CACHE
	if (cache->cache_mode == MR_CACHE_MODE_USER) {
		cache->umr_cache.lock = cache->ep->verbs_ep.umrc.thread;
		cache->umr_cache.event_queue = cache->ep->verbs_ep.umrc.event_queue;
		if (cache->umr_cache.event_queue)
			psm3_verbs_umrc_event_queue_process(cache);
	}
#endif // UMR_CACHE
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
	// we only have items on avail_list when MR_CACHE_MODE_USER*
	if (cache->map.payload.nelems >= cache->max_entries) {
		int ret;
		mrc = TAILQ_FIRST(&cache->avail_list);
		if (! mrc) {
			_HFI_MMDBG("user space MR cache full\n");
			cache->full++;
			if (priority) {
				if (cache->access)
					cache->full_pri++;
				else if (access & IBV_ACCESS_REMOTE_WRITE)
					cache->full_pri_recv++;
				else
					cache->full_pri_send++;
			} else {
				if (cache->access)
					cache->full_nonpri++;
				else if (access & IBV_ACCESS_REMOTE_WRITE)
					cache->full_nonpri_recv++;
				else
					cache->full_nonpri_send++;
			}
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
#ifdef UMR_CACHE
 		psm3_verbs_umrc_unregister(&cache->umr_cache, (uintptr_t)addr, length);
#endif
#ifdef PSM_HAVE_RNDV_MOD
		if (cache->cache_mode == MR_CACHE_MODE_KERNEL
			|| cache->cache_mode == MR_CACHE_MODE_RV)	// should not happen
			ret = psm3_rv_dereg_mem(cache->rv, mrc->mr.rv_mr);
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
			psm3_mpool_put(p_item);
			return NULL;
		}
		mrc->mr.mr_ptr = NULL;
	} else {
		// allocate a new item
		p_item = (cl_map_item_t *)psm3_mpool_get(cache->mr_pool);
		if (! p_item) {	// keep KW happy, should not happen, we check max above
			_HFI_ERROR("unexpected cache pool allocate failure\n");
			cache->failed++;
			return NULL;
		}
		mrc = &p_item->payload;
		// we initialize mrc below
		cache->max_nelems = max(cache->max_nelems, cache->map.payload.nelems+1);
	}
#ifdef PSM_HAVE_RNDV_MOD
	/* need cmd_fd for access to ucontext when converting user pd into kernel pd */
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL) {
		// user space QPs for everything
		mrc->mr.rv_mr = psm3_rv_reg_mem(cache->rv, cache->cmd_fd, cache->pd, addr, length, access);
		if (! mrc->mr.rv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				if (priority) {
					(void)psm3_gpu_evict_some(cache->ep, length, access);
					if (cache->access)
						cache->full_pri++;
					else if (access & IBV_ACCESS_REMOTE_WRITE)
						cache->full_pri_recv++;
					else
						cache->full_pri_send++;
				} else {
					if (cache->access)
						cache->full_nonpri++;
					else if (access & IBV_ACCESS_REMOTE_WRITE)
						cache->full_nonpri_recv++;
					else
						cache->full_nonpri_send++;
				}
#endif /* PSM_CUDA || PSM_ONEAPI */
			} else {
				_HFI_ERROR("reg_mr failed; %s acc 0x%x\n", strerror(errno), access);
				cache->failed++;
			}
			psm3_mpool_put(p_item);
			errno = save_errno;
			return NULL;
		}
		mrc->iova = mrc->mr.rv_mr->iova;
		mrc->lkey = mrc->mr.rv_mr->lkey;
		mrc->rkey = mrc->mr.rv_mr->rkey;
	} else if (cache->cache_mode == MR_CACHE_MODE_RV) {
		// kernel QP for RDMA, user QP for send DMA
		mrc->mr.rv_mr = psm3_rv_reg_mem(cache->rv, cache->cmd_fd, (access&IBV_ACCESS_RDMA)?NULL:cache->pd, addr, length, access);
		if (! mrc->mr.rv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				if (priority) {
					(void)psm3_gpu_evict_some(cache->ep, length, access);
					if (cache->access)
						cache->full_pri++;
					else if (access & IBV_ACCESS_REMOTE_WRITE)
						cache->full_pri_recv++;
					else
						cache->full_pri_send++;
				} else {
					if (cache->access)
						cache->full_nonpri++;
					else if (access & IBV_ACCESS_REMOTE_WRITE)
						cache->full_nonpri_recv++;
					else
						cache->full_nonpri_send++;
				}
#endif /* PSM_CUDA || PSM_ONEAPI */
			} else {
				_HFI_ERROR("reg_mr failed; %s acc 0x%x\n", strerror(errno), access);
				cache->failed++;
			}
			psm3_mpool_put(p_item);
			errno = save_errno;
			return NULL;
		}
		mrc->iova = mrc->mr.rv_mr->iova;
		mrc->lkey = mrc->mr.rv_mr->lkey;
		mrc->rkey = mrc->mr.rv_mr->rkey;
	} else
#endif /* PSM_HAVE_RNDV_MOD */
	{
		// user space QPs for everything
		mrc->mr.ibv_mr = ibv_reg_mr(cache->pd, addr, length, access);
		if (! mrc->mr.ibv_mr) {
			int save_errno = errno;
			if (errno == ENOMEM) {
				cache->full++;
				if (priority) {
					if (cache->access)
						cache->full_pri++;
					else if (access & IBV_ACCESS_REMOTE_WRITE)
						cache->full_pri_recv++;
					else
						cache->full_pri_send++;
				} else {
					if (cache->access)
						cache->full_nonpri++;
					else if (access & IBV_ACCESS_REMOTE_WRITE)
						cache->full_nonpri_recv++;
					else
						cache->full_nonpri_send++;
				}
			} else {
				_HFI_ERROR("reg_mr failed; %s acc 0x%x\n", strerror(errno), access);
				cache->failed++;
			}
			psm3_mpool_put(p_item);
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
	update_stats_inc_inuse(cache, length, access);
#ifdef UMR_CACHE
	if (cache->cache_mode == MR_CACHE_MODE_USER) {
		mrc->umrc_not_reuse = 0;
		mrc->umrc_reg = (psm3_verbs_umrc_register(&cache->umr_cache, (uintptr_t)addr, length) == PSM2_OK);
	}
#endif // UMR_CACHE
	_HFI_MMDBG("registered new MR pri %d addr %p len %"PRIu64" access 0x%x ref %u ptr %p nelems %u\n",
		priority, addr, length, access, mrc->refcount, mrc, cache->map.payload.nelems);
	return mrc;
}

int psm3_verbs_release_mr(struct psm3_verbs_mr *mrc)
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
		update_stats_dec_inuse(mrc->cache, mrc->length, mrc->access);
#ifdef UMR_CACHE
		if (mrc->cache->cache_mode == MR_CACHE_MODE_USER) {
			if (!mrc->umrc_not_reuse)
				// put on avail_list
				TAILQ_INSERT_TAIL(&mrc->cache->avail_list, mrc, next);
			else {
				// not re-used and should be removed from avail_list
				cl_map_item_t *p_item = container_of(mrc, cl_map_item_t, payload);
				if (!TAILQ_EMPTY(&mrc->cache->avail_list))
					TAILQ_REMOVE(&mrc->cache->avail_list, mrc, next);
				ips_cl_qmap_remove_item(&mrc->cache->map, p_item);
				ret = ibv_dereg_mr(mrc->mr.ibv_mr);
				if (ret) {
					// nasty choice, do we leak the MR or leak the cache entry
					// we chose to leak the MR and free the cache entry
					_HFI_ERROR("unexpected dreg_mr failure on %s: %s\n", mrc->cache->ep->dev_name, strerror(errno));
					errno = EIO;
					ret = -1;
				}
				mrc->mr.mr_ptr = NULL;
				psm3_mpool_put(p_item);
			}
		} else
#endif
		if (mrc->cache->cache_mode == MR_CACHE_MODE_USER_NOINVAL) {
			// if refcount now zero, put on avail_list to be reclaimed if needed
			TAILQ_INSERT_TAIL(&mrc->cache->avail_list, mrc, next);
		} else {
			_HFI_MMDBG("freeing MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p nelems %u\n",
				mrc->addr, mrc->length, mrc->access, mrc->refcount, mrc,
				mrc->cache->map.payload.nelems);
			cl_map_item_t *p_item = container_of(mrc, cl_map_item_t, payload);
			ips_cl_qmap_remove_item(&mrc->cache->map, p_item);
#ifdef PSM_HAVE_RNDV_MOD
			if (mrc->cache->cache_mode == MR_CACHE_MODE_KERNEL
					|| mrc->cache->cache_mode == MR_CACHE_MODE_RV)
				ret = psm3_rv_dereg_mem(mrc->cache->rv, mrc->mr.rv_mr);
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
			psm3_mpool_put(p_item);
		}
	}
	return ret;
}

void psm3_verbs_free_mr_cache(psm2_mr_cache_t cache)
{
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#ifdef PSM_HAVE_RNDV_MOD
	if (cache->rv && PSMI_IS_GPU_ENABLED)
		psm3_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE,
					&cache->rv_gpu_stats);
#endif
#endif
#ifdef UMR_CACHE
	if (cache->cache_mode == MR_CACHE_MODE_USER && cache->umr_cache.event_queue)
		psm3_verbs_umrc_event_queue_process(cache);
#endif
	psm3_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE, cache);
	while (cache->map.payload.nelems) {
		cl_map_item_t *p_item = __cl_map_root(&cache->map);
		psmi_assert(p_item != cache->map.nil_item);
		psm3_verbs_mr_t mrc = &p_item->payload;
		psmi_assert(mrc->mr.mr_ptr);
		if (mrc->mr.mr_ptr) {
			int ret;
			_HFI_MMDBG("free MR addr %p len %"PRIu64" access 0x%x ref %u ptr %p\n",
				mrc->addr, mrc->length, mrc->access, mrc->refcount, mrc);
			if (mrc->refcount) {
				_HFI_ERROR("unreleased MR in psm3_verbs_free_mr_cache addr %p len %"PRIu64" access 0x%x\n",
					mrc->addr, mrc->length, mrc->access);
				return; // leak the rest, let process exit cleanup
			}
			mrc->refcount = 0;
			cl_map_item_t *p_item = container_of(mrc, cl_map_item_t, payload);
			ips_cl_qmap_remove_item(&cache->map, p_item);
			TAILQ_REMOVE(&cache->avail_list, mrc, next);
#ifdef PSM_HAVE_RNDV_MOD
			if (cache->cache_mode == MR_CACHE_MODE_KERNEL
					|| cache->cache_mode == MR_CACHE_MODE_RV)
				ret = psm3_rv_dereg_mem(cache->rv, mrc->mr.rv_mr);
			else
#endif
				ret = ibv_dereg_mr(mrc->mr.ibv_mr);
			if (ret)
				_HFI_ERROR("unexpected dreg_mr failure on %s: %s\n", mrc->cache->ep->dev_name, strerror(errno));
			mrc->mr.mr_ptr = NULL;
			psm3_mpool_put(p_item);
		}
	}
	psmi_assert(TAILQ_EMPTY(&cache->avail_list));
	psmi_assert(! cache->map.payload.nelems);

	psm3_mpool_destroy(cache->mr_pool);
	psmi_free(cache);
}

#ifdef UMR_CACHE

// this is used to enable ips_cl_qmap_search() in psm3_rbtree.c
// if RBTREE_CMP was previously defined
#define ENABLE_OVERLAPPING_SEARCH

static uint32_t umrc_access;
static unsigned long RBTREE_GET_LEFTMOST(psm3_verbs_mr_t mrc)
{
	if (mrc->access == umrc_access)
		return (uintptr_t)mrc->addr;
	else
		return 0;
}
static unsigned long RBTREE_GET_RIGHTMOST(psm3_verbs_mr_t mrc)
{
	if (mrc->access == umrc_access)
		return (uintptr_t)mrc->addr + mrc->length;
	else
		return ULONG_MAX;
}
#include "psm3_rbtree.c"

static void psm3_verbs_umrc_event_process(psm2_mr_cache_t cache, uint32_t event, uint64_t addr, uint64_t length)
{
	int ret;
	psm3_verbs_mr_t mrc;
	cl_map_item_t *p_item;

	_HFI_MMDBG("cache=%p fd=%d ADDR=0x%lx length=0x%lx uffd event=0x%x nelms=%d\n",
				cache, *cache->umr_cache.fd, addr, length, event, cache->map.payload.nelems);
	umrc_access = IBV_ACCESS_LOCAL_WRITE | cache->access;
	do {
	next_search:
		p_item = ips_cl_qmap_search(&cache->map, (uintptr_t)addr, (uintptr_t)addr + length);
		if (p_item != cache->map.nil_item) {
			mrc = &p_item->payload;
			if (mrc->mr.mr_ptr) {
				if (!mrc->refcount) {
					if (mrc->umrc_reg)
						ret = psm3_verbs_umrc_unregister(&cache->umr_cache, (uintptr_t)mrc->addr, mrc->length);
					_HFI_MMDBG("event_process: cache=%p mrc->addr=%p mrc->length=%lx mrc->access=%d event=%x fd=%d\n",
								cache, mrc->addr, mrc->length, mrc->access, event, *cache->umr_cache.fd);
					ret = ibv_dereg_mr(mrc->mr.ibv_mr);
					if (ret)
						_HFI_ERROR("Unexpected dreg_mr failure: %s\n", strerror(errno));
					update_stats_dec_inuse(mrc->cache, mrc->length, mrc->access);
					ips_cl_qmap_remove_item(&cache->map, p_item);
					TAILQ_REMOVE(&cache->avail_list, mrc, next);
					mrc->mr.mr_ptr = NULL;
					psm3_mpool_put(p_item);
					cache->umr_cache.stats.evict++;
				} else {
					// should be unregister anyway
					if (mrc->umrc_reg)
						psm3_verbs_umrc_unregister(&cache->umr_cache, (uintptr_t)mrc->addr, mrc->length);
					// cannot be deleted now and marked as not re-used
					mrc->umrc_not_reuse = 1;
					mrc->access = 0; // mr cache entry will not appear in search
					_HFI_MMDBG("marked as not_reused cache=%p mrc->refcount=%d mrc->addr=%p mrc->length=%lx mrc->access=%d event=%x\n",
								cache, mrc->refcount, mrc->addr, mrc->length, mrc->access, event);
				}
				umrc_access = IBV_ACCESS_LOCAL_WRITE | cache->access;
			}
		} else if (!(umrc_access & IBV_ACCESS_REMOTE_WRITE)) {
				// we seached for send (! remote write), now search for
				// a recv (remote write) entry matching addr/length
				umrc_access |= IBV_ACCESS_REMOTE_WRITE;
				goto next_search;
		}
	} while(p_item != cache->map.nil_item);
}

static void psm3_verbs_umrc_event_queue_process(psm2_mr_cache_t cache)
{
	psm2_umrc_event_t umrc_event;

	TAILQ_FOREACH(umrc_event, &cache->umrc_event_list, next) {
		psm3_verbs_umrc_event_process(cache, umrc_event->event, umrc_event->addr, umrc_event->length);
		TAILQ_REMOVE(&cache->umrc_event_list, umrc_event, next);
		cache->umr_cache.queue_cnt--;
	}
}

// TBD rename as umrc_init
static int psm3_verbs_umrc_prepare(psm2_ep_t ep, psm2_mr_cache_t cache)
{
	psm2_umr_cache_t umr_cache = &cache->umr_cache;

	umr_cache->fd = &ep->verbs_ep.umrc.fd;
	umr_cache->page_size = PSMI_PAGESIZE;
	umr_cache->mr_cache = cache;
	ep->verbs_ep.umrc.mr_cache = cache;
	if (ep->verbs_ep.umrc.event_queue) {
		umr_cache->event_queue = ep->verbs_ep.umrc.event_queue;
		umr_cache->mm_events = (struct psm2_umrc_event *)psmi_calloc(PSMI_EP_NONE, UNDEFINED,
							sizeof(struct psm2_umrc_event), UMR_CACHE_QUEUE_DEPTH);
		if (! umr_cache->mm_events) {
			_HFI_ERROR("Couldn't allocate memory for mm_events. MR cache disabled\n");
			return ENOMEM;
		}
		TAILQ_INIT(&cache->umrc_event_list);
	}
	_HFI_MMDBG("Init cache=%p fd=%d\n", cache, ep->verbs_ep.umrc.fd);

	return PSM2_OK;
}

void psm3_verbs_umrc_free(psm2_umrc_t umrc)
{
	psm2_umr_cache_t umr_cache = &((psm2_mr_cache_t)umrc->mr_cache)->umr_cache;
	psmi_free(umr_cache->mm_events);
	umr_cache->mm_events = NULL;
}

static inline void psm3_verbs_uffd_collect(psm2_umrc_t umrc, uint32_t event, uint64_t addr, uint64_t len)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)umrc->mr_cache;
	psm2_umrc_event_t mm_event;

	if (cache->umr_cache.lock)
 		PSMI_LOCK(cache->ep->mq->progress_lock);
	if (cache->umr_cache.event_queue) {
		if (TAILQ_EMPTY(&cache->umrc_event_list))
			mm_event = cache->umr_cache.mm_events;
		else {
			if (cache->umr_cache.queue_cnt < UMR_CACHE_QUEUE_DEPTH)
				mm_event = TAILQ_LAST(&cache->umrc_event_list, umrc_event_list) +1;
			else {
				if (cache->umr_cache.lock)
					PSMI_UNLOCK(cache->ep->mq->progress_lock);
				_HFI_ERROR("max_event_queue exceeded maximum supported\n");
				return;
			}
		}
		mm_event->addr = addr;
		mm_event->length = len;
		mm_event->event = event;
		cache->umr_cache.queue_cnt++;
		TAILQ_INSERT_TAIL(&cache->umrc_event_list, mm_event, next);
		_HFI_MMDBG("** uffd event ** cache=%p addr=0x%lx len=0x%lx uffd event=0x%x fd=%d queue_cnt=%u\n",
					cache, addr, len, event, *cache->umr_cache.fd, cache->umr_cache.queue_cnt);
	} else {
		_HFI_MMDBG("** uffd event ** cache=%p addr=0x%lx len=0x%lx uffd event=0x%x fd=%d\n",
				cache, addr, len, event, *cache->umr_cache.fd);
		psm3_verbs_umrc_event_process(cache, event, addr, len);
	}
	if (cache->umr_cache.lock)
 		PSMI_UNLOCK(cache->ep->mq->progress_lock);
}

void psm3_verbs_uffd_event(psm2_ep_t ep)
{
	psm2_mr_cache_t cache = ep->verbs_ep.umrc.mr_cache;
	psm2_umrc_t  umrc = &ep->verbs_ep.umrc;
	struct uffd_msg msg;
	int ret;

	do {
		ret = read(ep->verbs_ep.umrc.fd, &msg, sizeof(msg));
		if (ret != sizeof(msg)) {
			if (errno != EAGAIN)
				break;
			continue;
		}
		switch (msg.event) {
		case UFFD_EVENT_REMOVE:
			cache->umr_cache.stats.remove++;
			psm3_verbs_uffd_collect(umrc, msg.event, msg.arg.remove.start,
							msg.arg.remove.end - msg.arg.remove.start);
			break;
		case UFFD_EVENT_UNMAP:
			cache->umr_cache.stats.unmap++;
			psm3_verbs_uffd_collect(umrc, msg.event, msg.arg.remove.start,
							msg.arg.remove.end - msg.arg.remove.start);
			break;
		case UFFD_EVENT_REMAP:
			cache->umr_cache.stats.remap++;
			psm3_verbs_uffd_collect(umrc, msg.event, msg.arg.remap.from, msg.arg.remap.len);
			break;
		default:
			_HFI_ERROR("uffd unexpected event\n");
			break;
		}
	} while (ret > 0);
}
#endif // UMR_CACHE
#endif // PSM_HAVE_REG_MR
