/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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

  Copyright(c) 2015 Intel Corporation.

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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */


#ifndef _PSMI_IN_USER_H
#error psm_verbs_mr.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_VERBS_MR_H
#define _PSMI_VERBS_MR_H

#include <infiniband/verbs.h>
#ifdef RNDV_MOD
#include <psm_rndv_mod.h>
#endif

#define MR_CACHE_MODE_NONE 0	// user space MRs, but no caching
#define MR_CACHE_MODE_KERNEL 1	// kernel MR cache in rendezvous module
#define MR_CACHE_MODE_USER 2	// user space MR cache (demo quality only)
#define MR_CACHE_MODE_RV 3	// kernel MRs for kernel rendezvous module QPs
#define MR_CACHE_MODE_VALID(mode) ((unsigned)(mode) <= 3)

// This performs memory registration for RDMA Rendezvous when PSM3_RDMA enabled
// Priority registration calls ere those immediately before the data transfer
// hence delaying their registration directly delays IOs.
// Non-priority calls are those registering the whole IO
// prior to sending/receiving the CTS.  Delays in non-priority
// calls have less direct impacts on IO delays.
// Numbers and size limits for priority registrations are able to be directly
// estimated since limits on outstanding RDMAs and their size constrains them.
// Non-priority registrations and sizes are a function of application design
// and how many conurrent MPI_ISend/IRecv are outstanding and of what size.
// Given defaults, priority registrations limits will be 160 entries of
// 128K each for a total of < 20MB of the kernel cache.
// All non-priority registrations eventually get used for IOs and become
// a priority use case, so attempting to track whether each entry in the cache
// is a priority or non-priority entry is tricky, especially since there
// can be both priority and non-priority references.  So instead of attempting
// to track the amount of priority and non-priority entries, we simply
// track current inuse entries and only allow non-priority registrations when
// we have a reasonable amount of headroom.  This way most priority
// registrations will succeed.

// the pointer to psm2_verbs_mr itself is the handle for subsequenent release
struct psm2_verbs_mr {
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
#ifdef RNDV_MOD
		// when cache_mode = MR_CACHE_MODE_KERNEL
		psm2_rv_mr_t rv_mr;	// internally we can get addr, length and pd from here
#endif
		// when cache_mode = MR_CACHE_MODE_NONE or MR_CACHE_MODE_USER
		struct ibv_mr *ibv_mr;	// internally we can get addr, length and pd from here
	} mr;
	struct psm2_mr_cache *cache;	// TBD could have caller pass to release
	uint32_t refcount;
	// this structure will be used as a search key too, so must include
	// addr and length directly since search key object won't have an mr ptr
	// also addr is used in callers to translate remote addr returned in CTS
	void *addr;
	uint64_t length;
	uint8_t access;
	// below is for queue of cache entries available for reuse (refcount==0)
	// only used when cache_mode==1
	TAILQ_ENTRY(psm2_verbs_mr) next;
};
typedef struct psm2_verbs_mr *psm2_verbs_mr_t;

// cache is kept opaque since it has some rbtree fields in it
struct psm2_mr_cache;
typedef struct psm2_mr_cache *psm2_mr_cache_t;

extern psm2_mr_cache_t psm2_verbs_alloc_mr_cache(psm2_ep_t ep,
				uint32_t num_entries, uint8_t cache_mode,
				uint32_t pri_entries, uint64_t pri_size);
// pick a flag value unused by verbs.h
#define IBV_ACCESS_IS_GPU_ADDR 0x10000000
// pd can be the verbs_ep.pd or NULL to use the RV module's kernel pd
extern psm2_verbs_mr_t psm2_verbs_reg_mr(psm2_mr_cache_t cache,
				bool priority, struct ibv_pd *pd,
				void *addr, uint64_t length, int access);
static inline psm2_verbs_mr_t psm2_verbs_ref_mr(psm2_verbs_mr_t mr) {
	mr->refcount++;
	return mr;
}
extern int psm2_verbs_release_mr(psm2_verbs_mr_t mrc);
extern void psm2_verbs_free_mr_cache(psm2_mr_cache_t cache);
void ips_tid_mravail_callback(struct ips_proto *proto);

#endif // _PSMI_VERBS_MR_H
