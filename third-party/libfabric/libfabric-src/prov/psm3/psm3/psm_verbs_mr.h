/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2023 Intel Corporation.

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

  Copyright(c) 2023 Intel Corporation.

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

/* Copyright (c) 2003-2023 Intel Corporation. All rights reserved. */


#ifndef _PSMI_IN_USER_H
#error psm_verbs_mr.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_VERBS_MR_H
#define _PSMI_VERBS_MR_H

#ifdef PSM_HAVE_REG_MR

#include <infiniband/verbs.h>
#ifdef PSM_HAVE_RNDV_MOD
#include <psm_rndv_mod.h>
#define IBV_ACCESS_RDMA IBV_ACCESS_KERNEL
#else /* PSM_HAVE_RNDV_MODE */
#define IBV_ACCESS_RDMA 0
// pick a flag value unused by verbs.h
#define IBV_ACCESS_IS_GPU_ADDR 0x10000000
#endif /* PSM_HAVE_RNDV_MODE */

#define MR_CACHE_MODE_NONE 0	// user space MRs, but no caching
#define MR_CACHE_MODE_KERNEL 1	// kernel MR cache in rendezvous module
#define MR_CACHE_MODE_USER 2	// user space MR cache w invalidate
#define MR_CACHE_MODE_USER_NOINVAL 3 // user space MR cache w/o invalidate
#define MR_CACHE_MODE_RV 4	// kernel MRs for kernel rendezvous module QPs

#ifdef UMR_CACHE
#define MR_CACHE_MODE_VALID(mode) ((unsigned)(mode) <= MR_CACHE_MODE_RV)
#else
#define MR_CACHE_MODE_VALID(mode) ((unsigned)(mode) <= MR_CACHE_MODE_RV \
									&& (unsigned)(mode) != MR_CACHE_MODE_USER)
#endif

// does the cache mode include user space caching (as opposed to just reference
// counting MRs for IOs in flight)
#define MR_CACHE_USER_CACHING(mode) ((mode) == MR_CACHE_MODE_USER_NOINVAL \
										|| (mode) == MR_CACHE_MODE_USER)

// This performs memory registration for Send DMA and RDMA Rendezvous when
// Send DMA or PSM3_RDMA is enabled.
// Priority registration calls are those immediately before the data transfer
// hence delaying their registration directly delays IOs.
// Non-priority calls are those registering the whole IO
// prior to sending/receiving the CTS.  Delays in non-priority
// calls have less direct impacts on IO delays.
// Send DMA registrations are also considered non-priority and if there is no
// space the caller will use another send strategy.
// Numbers and size limits for priority registrations are able to be directly
// estimated since limits on outstanding RDMAs and their size constrains them.
// Non-priority registrations and sizes are a function of application design
// and how many conurrent MPI_ISend/IRecv are outstanding and of what size.
// Given defaults, priority registrations limits will be 160 entries of
// 128K each for a total of < 20MB of the kernel cache.
// All non-priority RDMA registrations eventually get used for IOs and become
// a priority use case, so attempting to track whether each entry in the cache
// is a priority or non-priority entry is tricky, especially since there
// can be both priority and non-priority references.  So instead of attempting
// to track the amount of priority and non-priority entries, we simply
// track current inuse entries and only allow non-priority registrations when
// we have a reasonable amount of headroom.  This way most priority
// registrations will succeed.
//
// access indicates the purpose and permissions for the MR
// IBV_ACCESS_RDMA - send RDMA
// IBV_ACCESS_RDMA|IBV_ACCESS_REMOTE_WRITE - recv RDMA
// 0 - send DMA
// | IBV_ACCESS_IS_GPU_ADDR - GPU variation of any of the above 3
// When using RV kernel QPs, IBV_ACCESS_RDMA (== IBV_ACCESS_KERNEL) is passed
// to RV.  Otherwise, it's is omitted and a user space accessible MR is created.
// When using only user space QPs, we allow send RDMA and send DMA to share MRs.
//
// By default, the psm2_mr_cache only tracks in use MRs for reference counting
// and RV is used to actually do the caching.  However MR_CACHE_MODE_USER*
// allows actual user space caching and does not use RV.
#ifdef UMR_CACHE
// UMR_CACHE define enables user space caching with userfaultfd providing
// events to allow MR invalidate on application memory free.
#endif

// States for a cache entry (psm3_verbs_mr_t mrc)
// FREE - on mpool, not on any lists, no MR
// INUSE - on map, have MR, refcount>0, IO in flight
// CACHED - on map, have MR, refcount==0, on avail_list
#ifdef UMR_CACHE
// INVAL - have MR, refcount>0, IO in flight, on inval_list, umrc_inval==1
// DEREG - have MR, refcount==0, on dereg_list
#endif
//
//  Transition     Event            Actions
// FREE->INUSE   reg_mr miss  mpool_get, reg_mr, add to map, refcount=1
// CACHED->INUSE reg_mr miss  sort of a CACHED->FREE->INUSE w/o mpool put/get
// CACHED->INUSE reg_mr hit   take off avail_list, refcount++
// INUSE->INUSE  reg_mr hit   refcount++
// INUSE->INUSE  release      refcount--
// INUSE->CACHED release      add to avail list
// INUSE->FREE   release      remove from map, dereg_mr, mpool_put
// INUSE         shutown      error and leak
// CACHED->FREE  shutdown     remove from map, remove from avail, dereg_mr
#ifdef UMR_CACHE
// INUSE->INVAL  uffd event   remove from map, put on inval_list
// INVAL         shutown      error and leak
// INVAL->FREE   release      dereg_mr, mpool_put, remove from inval_list
// CACHED->DEREG uffd event   remove from map, put on dereg_list
// DEREG->FREE   dereg done   remove from dereg_list, ibv_dereg_mr, mpool_put
//                            mrc disassociated with MR prior to ibv_dereg_mr
#endif

// the pointer to psm3_verbs_mr itself is the handle for subsequenent release
struct psm3_verbs_mr {
	// fields for use by caller
	// TBD - review use, we have rkey/lkey here and in mr itself, don't need both
	uint64_t iova;	// used by caller
	uint32_t lkey;	// used by caller
	uint32_t rkey;	// used by caller
	// private fields below are not for use by caller
	// for kernel rendezvous this might just be a kernel handle and this
	// information may be private in the kernel
	union {
		void *mr_ptr;		// for simple test of != NULL or clearing to NULL
#ifdef PSM_HAVE_RNDV_MOD
		// when cache_mode = MR_CACHE_MODE_KERNEL or MR_CACHE_MODE_RV
		psm3_rv_mr_t rv_mr;	// internally we can get addr, length and pd from here
#endif
		// when cache_mode = MR_CACHE_MODE_NONE or MR_CACHE_MODE_USER*
		struct ibv_mr *ibv_mr;	// internally we can get addr, length and pd from here
	} mr;
	struct psm2_mr_cache *cache;	// TBD could have caller pass to release
	uint32_t refcount;
#ifdef UMR_CACHE
	uint32_t inval; // INVAL, will remove when last IO done
	struct psm3_uffd_region *reg;	// one region in MR, optimizes dereg
#endif
	// this structure will be used as a search key too, so must include
	// addr and length directly since search key object won't have an mr ptr
	// also addr is used in callers to translate remote addr returned in CTS
	void *addr;
	uint64_t length;
	uint32_t access;
#if defined(PSM_ONEAPI)
	uint64_t alloc_id;
#endif
	// below is for queue of cache entries available for reuse (refcount==0)
	// only used when cache_mode==1
	TAILQ_ENTRY(psm3_verbs_mr) next;
};
typedef struct psm3_verbs_mr *psm3_verbs_mr_t;

// cache is kept opaque since it has some rbtree fields in it
struct psm2_mr_cache;
typedef struct psm2_mr_cache *psm2_mr_cache_t;

extern unsigned psm3_mr_cache_debug;

// to simplify our own debug prints
#define MR_RANGE_FMT "0x%"PRIx64":0x%"PRIx64" (len 0x%"PRIx64")"
#define MR_OUT_RANGE(addr, len) (uint64_t)(uintptr_t)(addr), \
					(uint64_t)(uintptr_t)(addr)+(uint64_t)(len)-1, \
					(uint64_t)(len)
#ifdef PSM_ONEAPI
#define MRC_FMT "0x%"PRIx64":0x%"PRIx64" (len 0x%"PRIx64") id %"PRIu64 \
				" access 0x%x"
#define MR_OUT_MRC(mrc) MR_OUT_RANGE((mrc)->addr, (mrc)->length), \
								 (mrc)->alloc_id, (mrc)->access
#else
#define MRC_FMT "0x%"PRIx64":0x%"PRIx64" (len 0x%"PRIx64") access 0x%x"
#define MR_OUT_MRC(mrc) MR_OUT_RANGE((mrc)->addr, (mrc)->length), (mrc)->access
#endif

extern psm2_mr_cache_t psm3_verbs_alloc_mr_cache(psm2_ep_t ep,
				uint32_t limit_entries, uint8_t cache_mode,
				uint32_t limit_size_mb,
				uint32_t limit_pri_entries, uint64_t pri_size
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint64_t gpu_pri_size
#endif
				);
extern int psm3_verbs_mr_cache_allows_user_mr(psm2_mr_cache_t cache);

extern psm3_verbs_mr_t psm3_verbs_reg_mr(psm2_mr_cache_t cache, bool priority,
				void *addr, uint64_t length, int access);
static inline psm3_verbs_mr_t psm3_verbs_ref_mr(psm3_verbs_mr_t mr)
{
	mr->refcount++;
	_HFI_MMDBG("inc ref MR "MRC_FMT" ref %d ptr %p\n",
				MR_OUT_MRC(mr), mr->refcount, mr);
	return mr;
}
extern void psm3_verbs_release_mr(psm3_verbs_mr_t mrc);
#ifdef PSM_HAVE_RNDV_MOD
// can the given MR be used for user space send DMA
static inline int psm3_verbs_user_space_mr(struct psm3_verbs_mr *mrc)
{
	psmi_assert(mrc);
	psmi_assert(mrc->refcount);
	return ! (mrc->access & IBV_ACCESS_KERNEL);
}
#endif
extern void psm3_verbs_free_mr_cache(psm2_mr_cache_t cache);
// this technically belongs in an ptl_ips header, but only ips_proto_header.h
// is included in psm_mq_recv.c so we put it here since its MR related as
// opposed to exposing psm_mq_recv.c to all the ptl_ips internals in ips_proto.h
void ips_tid_mravail_callback(struct ips_proto *proto);

#ifdef UMR_CACHE
// uffd callbacks into psm_verbs_mr.c
void psm3_verbs_umrc_uffd_callback(psm2_mr_cache_t cache, uint64_t addr,
								uint64_t len, uint8_t reason);
void psm3_verbs_umrc_worker_dereg(struct psm3_verbs_mr *mrc);
#endif

#endif // PSM_HAVE_REG_MR
#endif // _PSMI_VERBS_MR_H
