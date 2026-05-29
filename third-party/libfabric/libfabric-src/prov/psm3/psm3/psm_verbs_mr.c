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

// This performs memory registration for RDMA Rendezvous
// It also tracks MRs in use and allows existing MRs to be shared.
#include <sys/types.h>
#include "psm_user.h"   // sets PSM_HAVE_REG_MR
                        // and includes hal_verbs/verbs_ep.h and psm_verbs_mr.h

#ifdef PSM_HAVE_REG_MR
// cache_mode MR_CACHE_MODE_USER_NOINVAL, is a PoC and overly simple.
// This approach is only viable for some microbenchmarks and simple apps.
// For more complex apps the lack of invalidate hooks into memory free may
// lead to application data corruption through the use of stale cached MRs.
// However such hooks are not reliably possible until the 4.17+ kernels.
//
// The kernel RV module hooks into mmu_notfier for invalidate, mmu_notifier is
// used by hypervisors and hence is a complete and reliable way to cache MRs.

#ifdef UMR_CACHE
// cache_mode MR_CACHE_MODE_USER (UMR) is available when UMR_CACHE is defined.
// Controls and fields related to UMR are in psm2_mr_cache.umr_cache (umrc).
// UMR makes use of userfaultfd to provide events so MRs are invalidated
// on application memory free.  The events occur via messages delivered
// by polling and reading a userfaultfd file descriptor.  In some cases the
// kernel generation of userfaultfd events may block the main app thread which
// is calling free, malloc, mmap, mremap, etc.  So the events are polled and
// read in a PSM3 background thread.  The userfaultfd, its threads and
// related controls are in psm_uffd.c.  Comments there provide further details
// on behavior and locking model.
//
//        Concurrency and Locking model:
// The details of userfaultfd are tricky, see psm_uffd.c for more details.
// Within the MR cache itself there are 4 main runtime functions involved:
// - psm3_verbs_reg_mr
// - psm3_verbs_release_mr
// - psm3_verbs_umrc_uffd_callback
// - psm3_verbs_umrc_worker_dereg
// In addition psm3_alloc_mr_cache and psm3_free_mr_cache must avoid
// races at job start (EP open) and shutdown (EP close)
//
// The protection of various resources is covered by the following locks:
// creation_lock - held by caller during psm3_alloc_mr_cache and
//		psm3_free_mr_cache.  This prevents progress thread (rcvThread)
//		operations and hence prevents new calls to psm3_verbs_reg_mr and
//		psm3_verbs_release_mr during EP open and close.
// mq->progress_lock - held by caller during psm3_verbs_reg_mr and
//		psm3_verbs_release_mr.  This prevents races with progress thread
//		(rcvThread) and main thread starting and finishing IOs and calling
//		psm3_verbs_reg_mr and psm3_verbs_release_mr. Also held during
//		psm3_alloc_mr_cache and psm3_free_mr_cache, but in those cases
//		creation_lock also held.
// uffd_lock - held during psm3_verbs_umrc_uffd_callback.  Since the callback
//		is in uffd handler, the callback cannot get progress_lock and any
//		resources manipulated by the callback must be protected by this lock.
//		While holding this lock, no memory realloc or free calls are permitted
//		(this avoids potential deadlock from kernel pausing uffd handler due
//		to a memory free event from userfaultfd).
// For other cache modes, creation_lock and progress_lock are sufficient
// and progress_lock protects all MR cache resources.
// Note that in multi-EP/QP/Rail jobs uffd_callbacks can occur due to activity
// on other EPs/threads.
//
// Resource Protection:
// cache.map, avail_list, inval_list, dereg_list - uffd_lock
//    including any counters for checking cache size (nelems, inuse, etc)
// mrc->refcount, mrc->inval, mrc->reg, mrc->next (list entry) - uffd_lock
// uffd event queue and count - uffd_lock
// mr pool and related counters - progress_lock
//    including alloced_entries, registered_bytes, etc
// dereg_queued_cnt - an atomic to simplify its usage
// statistics are typically incremented in only 1 place so are protected
//		by the lock held where they are incremented. Stats thread
//		only reads them which is generally safe on x86 CPUs.
// uffd regions, dereg queue - uffd_lock
// uffd_tracker alloc/free, thread start/stop, callback_list - creation_lock
//
// To avoid memory free during uffd handler callbacks, the final ibv_dereg_mr
// (and mpool_put) is deferred to a separate worker thread which can safely get
// the progress_lock and/or invoke free.
//
// If an event queue model is chosen, the actual event handling is done at
// the start of the next psm3_verbs_reg_mr call, ensuring no false hits, but
// adding overhead in the critical path (but perhaps reducing the latency
// jitter of doing these operations in the handler).  However the ibv_dereg_mr
// is still deferred to a worker to get this expensive verbs call off the
// critical path.
#endif /* UMR_CACHE */

#include "psm2_hal.h"
#ifdef PSM_FI
#include "ips_config.h"
#endif
#include "psm_mq_internal.h"
#ifdef UMR_CACHE
#include <linux/userfaultfd.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <poll.h>
#include "psm_uffd.h"
#endif


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
#ifdef UMR_CACHE
// RBTREE_ITERATOR is a time/space trade off, 16 more bytes per cl_map_item_t
// but first, successor and predecessor faster.  Also helps RBTREE_AUGMENT's
// ips_cl_qmap_search_successor function.
#define RBTREE_ITERATOR
// RBTREE_AUGMENT is necessary so ips_cl_qmap_search can handle MRs with
// overlapping addr/addr + length ranges.  ips_cl_qmap_search and
// ips_cl_qmap_search_successor are used for UMR invalidate upon uffd callback
#define RBTREE_AUGMENT
#define RBTREE_EMIT_IPS_CL_QMAP_SEARCH_SUCCESSOR
#endif /* UMR_CACHE */

#ifdef PSM_DEBUG
#define RBTREE_ITERATOR
#define RBTREE_VALIDATE
#define RBTREE_VALIDATE_ENABLE psm3_mr_cache_debug
#define RBTREE_VALIDATE_PRINT _HFI_INFO
#endif

#include "psm3_rbtree.h"

#ifdef UMR_CACHE
struct psm2_umrc_event {
	uint64_t addr;
	uint64_t length;
	uint8_t reason;
};
typedef struct psm2_umrc_event *psm2_umrc_event_t;

struct psm2_umr_cache {
	psm2_umrc_event_t event_queue;
	uint32_t queue_depth; // allocated size of queue
	uint32_t queue_cnt;	// events event_queue[0 to queue_cnt] are queued & valid

	// number of dereg queued to uffd_dereg_worker
	PSM3_ATOMIC_DECL(uint32_t, dereg_queued_cnt);

	// INVAL mrc (IO inflight, but vaddr freed)
	TAILQ_HEAD(inval_list, psm3_verbs_mr) inval_list;

	struct {
		uint64_t remove;			// removed due to memory free (invalidate)
		uint64_t inval;				// memory free while IO in flight
		uint64_t failed_uffd_reg;	// uffd_register failures, should be none
			// stats for event_queue
		uint64_t max_queue_cnt;
		uint64_t queue_full;
			// stats for dereg worker queue
		uint64_t max_dereg_queued_cnt;
	} stats;
};
typedef struct psm2_umr_cache *psm2_umr_cache_t;
#endif /* UMR_CACHE */

struct psm2_mr_cache {
	uint32_t limit_entries;
	uint64_t limit_bytes;
	// limits on inuse when do non-priority registration to allow headroom
	// for priority registrations
	uint32_t limit_nonpri_inuse;
	uint64_t limit_nonpri_inuse_bytes;
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
	uint64_t limit_nonpri_gpu_inuse_bytes;
#endif
	psm3_rv_t rv;
	int cmd_fd;
#endif
	struct ibv_pd *pd;
	psm2_ep_t ep;
	uint8_t cache_mode;	// MR_CACHE_MODE_*
	psm3_verbs_mr_t (*reg_mr_fn)(psm2_mr_cache_t cache, bool priority,
									psm3_verbs_mr_t key);
	void (*release_mr_fn)(psm3_verbs_mr_t mrc);
	int access;	// value to OR into access on all reg_mr
	cl_qmap_t map;
	cl_map_item_t root;
	cl_map_item_t nil_item;
	// Below is for queue of cache entries available for reuse (refcount==0)
	// Only used when cache_mode==MR_CACHE_MODE_USER*
	// Available entries are added at end of list and reused from start.
	// Hence having aging of cached entries.
	// Aging helps reduce some of the corruption risk for USER_NOINVAL,
	// but is not a full solution.  Good enough for simple NOINVAL experiments.
	// For MR_CACHE_MODE_USER cache entries are properly removed on app memory
	// free so no risk.
	TAILQ_HEAD(avail_list, psm3_verbs_mr) avail_list;
	mpool_t mr_pool;	// pool of MRs
	// for non-UMR alloced_entries equals nelems and inuse
	uint32_t alloced_entries;	// total current allocated entries
	uint32_t max_alloced_entries;	// max observed allocated entries
	// when not MR_CACHE_MODE_USER* registered_bytes equals inuse_bytes
	uint64_t registered_bytes;	// total current registered bytes
	uint64_t max_registered_bytes;		// max observed registered bytes
	// some statistics for user space
	uint64_t hit;
	uint64_t miss;
	uint64_t rejected;		// rejected non-priority registration
	uint64_t evict;			// evictions to make space (MR_CACHE_MODE_USER*)
	uint64_t full;			// failed registration (tends to be priority)
	uint64_t full_pri;
	uint64_t full_pri_recv;
	uint64_t full_pri_send;
	uint64_t full_nonpri;
	uint64_t full_nonpri_recv;
	uint64_t full_nonpri_send;
	uint64_t failed;		// other failures, should be none
	uint64_t failed_alloc_mr;	// alloc_mr failures, should be none
	uint64_t failed_reg_mr;		// reg_mr failures, should be none
#ifndef PSM_HAVE_RNDV_MOD
	uint64_t failed_gpu_reg;	// unsupported GPU registration
#endif
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
#ifdef PSM_HAVE_GPU
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
#ifdef PSM_HAVE_GPU
	struct psm3_rv_gpu_cache_stats rv_gpu_stats;	// GPU statistics from rv module
									// will remain 0 if rv not open
#endif
#endif
#ifdef UMR_CACHE
	/* only used when MR_CACHE_MODE_USER */
	struct psm2_umr_cache umr_cache;
#endif // UMR_CACHE
};

unsigned psm3_mr_cache_debug;

// MR_CACHE_MODE_NONE
#ifdef PSM_HAVE_RNDV_MOD
// MR_CACHE_MODE_KERNEL
// MR_CACHE_MODE_RV
#endif
// MR_CACHE_MODE_USER_NOINVAL
static psm3_verbs_mr_t psm3_verbs_reg_mr_not_user(psm2_mr_cache_t cache,
					 bool priority, psm3_verbs_mr_t key);
#ifdef UMR_CACHE
// MR_CACHE_MODE_USER
static psm3_verbs_mr_t psm3_verbs_reg_mr_user(psm2_mr_cache_t cache,
					 bool priority, psm3_verbs_mr_t key);
#endif

// MR_CACHE_MODE_NONE
#ifdef PSM_HAVE_RNDV_MOD
// MR_CACHE_MODE_KERNEL
// MR_CACHE_MODE_RV
#endif
static void psm3_verbs_release_mr_not_user(psm3_verbs_mr_t mrc);
#ifdef UMR_CACHE
// MR_CACHE_MODE_USER
static void psm3_verbs_release_mr_user(psm3_verbs_mr_t mrc);
#endif
// MR_CACHE_MODE_USER_NOINVAL
static void psm3_verbs_release_mr_user_noinval(psm3_verbs_mr_t mrc);

#ifdef UMR_CACHE
static psm2_error_t psm3_verbs_umrc_init(psm2_ep_t ep, psm2_mr_cache_t cache);
static psm2_error_t psm3_verbs_umrc_fini(psm2_mr_cache_t cache);
static void psm3_verbs_umrc_event_queue_process(psm2_mr_cache_t cache);
#endif /* UMR_CACHE */

static void register_cache_stats(psm2_mr_cache_t cache);

static int mr_cache_key_cmp(const struct psm3_verbs_mr *a,
			    const struct psm3_verbs_mr *b,
			    bool is_search)
{
	// In general, to match, addr, length and access must match.
	// However, during lookup, for better performance, if the request MR
	// ("a" here) is a subregion of an existing MR ("b" here), it should
	// be treated as a match.
	// we sort by addr then length so UMR interval search for invalidate
	// works.
	// Note: due to the nature of the RB being used, which does not track
	// subtree bounds, the following code for search is not guaranteed to
	// always match subsets, and could result in seemingly non-
	// deterministic behavior. The check is constrained in search only as
	// a simple best-effort optimization. A better solution is to adopt
	// the interval-tree approach used in kernel (including RV module).
	bool is_subset = false;

	if (is_search)
		is_subset = (a->addr >= b->addr) &&
			    (((uint64_t)a->addr + a->length) <=
			     ((uint64_t)b->addr + b->length));
	if (!is_subset) {
		/* Not a subregion */
		if (a->addr < b->addr)
			return -1;
		else if (a->addr > b->addr)
			return 1;
		if (a->length < b->length)
			return -1;
		else if (a->length > b->length)
			return 1;
	}
	if (a->access < b->access)
		return -1;
	else if (a->access > b->access)
		return 1;
#ifdef PSM_HAVE_GPU
	return PSM3_GPU_CMP_MR(&a->gpu_specific, &b->gpu_specific);
#else
	return 0;
#endif
}

// rbtree.c uses these defines to establish some of it's code and
// then provides all the rbtree manipulation functions
// we want to control the compare funciton so we define RBTREE_CMP and thus
// must define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR to avoid compiler errors
#define RBTREE_CMP(a, b, c) mr_cache_key_cmp((a), (b), (c))
#define RBTREE_ASSERT                     psmi_assert
#define RBTREE_MAP_COUNT(PAYLOAD_PTR)     ((PAYLOAD_PTR)->nelems)
#define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR

#ifdef UMR_CACHE
#define RBTREE_EMIT_IPS_CL_QMAP_FIRST
#ifdef PSM_DEBUG
#define RBTREE_EMIT_IPS_CL_QMAP_DUMP
#endif /* PSM_DEBUG */

#else /* UMR_CACHE */
#define RBTREE_NO_EMIT_IPS_CL_QMAP_SEARCH
#endif /* UMR_CACHE */

#if defined(UMR_CACHE) || defined(PSM_DEBUG)
// this is used to enable ips_cl_qmap_search() in psm3_rbtree.c
// also used in debug logging
static unsigned long mr_cache_get_leftmost(const struct psm3_verbs_mr *mrc)
{
	return (uintptr_t)mrc->addr;
}
static unsigned long mr_cache_get_rightmost(const struct psm3_verbs_mr *mrc)
{
	return (uintptr_t)mrc->addr + mrc->length;
}
#define RBTREE_GET_LEFTMOST(PAYLOAD_PTR) mr_cache_get_leftmost(PAYLOAD_PTR)
#define RBTREE_GET_RIGHTMOST(PAYLOAD_PTR) mr_cache_get_rightmost(PAYLOAD_PTR)
#endif

#ifdef PSM_DEBUG
//#define RBTREE_PRINT _HFI_INFO
#define RBTREE_DUMP_PRINT _HFI_INFO
#define RBTREE_FMT "0x%"PRIx64
#endif

#include "psm3_rbtree.c"

// ---------------------------------------------------------------------------
// cache mrc and overall state consistency checking
//
// the ASSERT_MRC_*_LOCK macros get the psm3_uffd_lock and release it so
// various lists can be safely checked.  When running a UMR_CACHE enabled
// build without MR_CACHE_MODE_USER, these macros are still safe to use
// since the psm3_uffd_lock will be initialized in a load time constructor
// and be valid, although unnecessary for such cache modes.

#ifdef PSM_DEBUG
#define ASSERT_ON_MAP(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) \
			psmi_assert(ips_cl_qmap_searchv(&(cache)->map, mrc)); \
	} while (0)

	// The mrc may not have addr/length initialized and for MR_CACHE_MODE_USER
	// there could be a new MR on the list with same adddr/length while this mrc
	// is in INVAL or DEREG.  So we must do a simple linear search of the map.
	// && "NOT_ON_MAP" helps to identify the assert's intent
#define ASSERT_NOT_ON_MAP(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug ) { \
			cl_map_item_t *myp; \
			for (myp = ips_cl_qmap_first(&(cache)->map); \
					myp != (cache)->map.nil_item; \
					myp = ips_cl_qmap_successor(&(cache)->map, myp)) { \
				psmi_assert(mrc != &myp->payload && "NOT_ON_MAP"); \
			} \
		} \
	} while (0)

#define ASSERT_NOT_ON_AVAIL_LIST(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) \
			TAILQ_NOT_ON_ASSERT(&(cache)->avail_list, psm3_verbs_mr, mrc, next); \
	} while (0)

#ifdef UMR_CACHE
#define ASSERT_NOT_ON_INVAL_LIST(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) \
			TAILQ_NOT_ON_ASSERT(&(cache)->umr_cache.inval_list, psm3_verbs_mr, \
								 mrc, next); \
	} while (0)
#else /* UMR_CACHE */
#define ASSERT_NOT_ON_INVAL_LIST(cache, mrc)
#endif /* UMR_CACHE */

#define ASSERT_ON_AVAIL_LIST(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) { \
			TAILQ_ON_ASSERT(&(cache)->avail_list, psm3_verbs_mr, mrc, next); \
			ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
		} \
	} while (0)

	// this gives a more helpful message prior to the assert
#undef ASSERT_EQ
#define ASSERT_EQ(a, b) do { \
		if_pf ((a) != (b)) { \
			_HFI_INFO("%s (%"PRIu64") != %s (%"PRIu64")\n", \
				STRINGIFY(a), (uint64_t)(a), STRINGIFY(b), (uint64_t)(b)); \
			psmi_assert((a) == (b)); \
		} \
	} while (0)
#undef ASSERT_LE
#define ASSERT_LE(a, b) do { \
		if_pf ((a) > (b)) { \
			_HFI_INFO("%s (%"PRIu64") > %s (%"PRIu64")\n", \
				STRINGIFY(a), (uint64_t)(a), STRINGIFY(b), (uint64_t)(b)); \
			psmi_assert((a) <= (b)); \
		} \
	} while (0)

#ifdef UMR_CACHE
#define ASSERT_ON_INVAL_LIST(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) { \
			TAILQ_ON_ASSERT(&(cache)->umr_cache.inval_list, psm3_verbs_mr, \
							mrc, next); \
			ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
		} \
	} while (0)

#define ASSERT_MRC_INVAL_SET(mrc) psmi_assert((mrc)->inval)
#define ASSERT_MRC_INVAL_NOT_SET(mrc) psmi_assert(! (mrc)->inval)

#define ASSERT_HAVE_UFFD_REG(cache, mrc) do { \
		if_pf ((cache)->cache_mode == MR_CACHE_MODE_USER) \
			psmi_assert((mrc)->reg); \
	} while (0)

#define ASSERT_NO_UFFD_REG(cache, mrc) psmi_assert(! (mrc)->reg)

	// for MR_CACHE_MODE_USER can have some in INVAL or DEREG state not on map
	// all other modes retain all allocated entries on map
#define ASSERT_COUNTS_CONSISTENT(cache) do { \
		if_pf ((cache)->cache_mode == MR_CACHE_MODE_USER) \
			ASSERT_LE((cache)->map.payload.nelems, (cache)->alloced_entries); \
		else \
			ASSERT_EQ((cache)->map.payload.nelems, (cache)->alloced_entries); \
		ASSERT_LE((cache)->alloced_entries, (cache)->limit_entries); \
		ASSERT_LE((cache)->registered_bytes, (cache)->limit_bytes); \
	} while (0)

#define ASSERT_CACHE_EMPTY(cache) do { \
		psmi_assert(TAILQ_EMPTY(&(cache)->avail_list)); \
		psmi_assert(TAILQ_EMPTY(&(cache)->umr_cache.inval_list)); \
		psmi_assert(! (cache)->map.payload.nelems); \
		psmi_assert(! (cache)->alloced_entries); \
		psmi_assert(! (cache)->registered_bytes); \
		ASSERT_COUNTS_CONSISTENT(cache); \
	} while (0);

#else /* UMR_CACHE */

#define ASSERT_ON_INVAL_LIST(cache, mrc) psmi_assert(0);
#define ASSERT_MRC_INVAL_SET(mrc)
#define ASSERT_MRC_INVAL_NOT_SET(mrc)
#define ASSERT_HAVE_UFFD_REG(cache, mrc)
#define ASSERT_NO_UFFD_REG(cache, mrc)
#define ASSERT_COUNTS_CONSISTENT(cache) do { \
		ASSERT_EQ((cache)->map.payload.nelems, (cache)->alloced_entries); \
		ASSERT_LE((cache)->map.payload.nelems, (cache)->limit_entries); \
		ASSERT_LE((cache)->registered_bytes, (cache)->limit_bytes); \
	} while (0);
#define ASSERT_CACHE_EMPTY(cache) do { \
		psmi_assert(TAILQ_EMPTY(&(cache)->avail_list)); \
		psmi_assert(! (cache)->alloced_entries); \
		psmi_assert(! (cache)->map.payload.nelems); \
		psmi_assert(! (cache)->registered_bytes); \
		ASSERT_COUNTS_CONSISTENT(cache); \
	} while (0);

#endif /* UMR_CACHE */

// asserts vs logical mrc states, see psm_verbs_mr.h for summary of states

// this not valid after free_mr puts back in pool
#define ASSERT_MRC_FREE(cache, mrc) do { \
		psmi_assert(! (mrc)->refcount); \
		ASSERT_MRC_INVAL_NOT_SET(mrc); \
		psmi_assert(! (mrc)->mr.mr_ptr); \
		ASSERT_NO_UFFD_REG(cache, mrc); \
		ASSERT_NOT_ON_MAP(cache, mrc); \
		ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
	} while (0)

#ifdef UMR_CACHE
// must use this for MR_CACHE_MODE_USER if don't already have uffd_lock
#define ASSERT_MRC_FREE_LOCK(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) { \
			PSMI_LOCK(psm3_uffd_lock); \
			ASSERT_MRC_FREE(cache, mrc); \
			PSMI_UNLOCK(psm3_uffd_lock); \
		} \
	} while (0)
#else
#define ASSERT_MRC_FREE_LOCK(cache, mrc) ASSERT_MRC_FREE(cache, mrc)
#endif

#define ASSERT_MRC_INUSE(cache, mrc) do { \
		psmi_assert((mrc)->refcount); \
		ASSERT_MRC_INVAL_NOT_SET(mrc); \
		psmi_assert((mrc)->mr.mr_ptr); \
		ASSERT_HAVE_UFFD_REG(cache, mrc); \
		ASSERT_ON_MAP(cache, mrc); \
		ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
	} while (0)

#define ASSERT_MRC_CACHED(cache, mrc) do { \
		psmi_assert(! (mrc)->refcount); \
		ASSERT_MRC_INVAL_NOT_SET(mrc); \
		psmi_assert((mrc)->mr.mr_ptr); \
		ASSERT_HAVE_UFFD_REG(cache, mrc); \
		ASSERT_ON_MAP(cache, mrc); \
		ASSERT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
	} while (0)

#ifdef UMR_CACHE
// special handling for shutdown where MR_CACHE_MODE_USER uffd regions already
// flushed
#define ASSERT_MRC_CACHED_FLUSHED(cache, mrc) do { \
		psmi_assert(! (mrc)->refcount); \
		ASSERT_MRC_INVAL_NOT_SET(mrc); \
		psmi_assert((mrc)->mr.mr_ptr); \
		ASSERT_NO_UFFD_REG(cache, mrc); \
		ASSERT_ON_MAP(cache, mrc); \
		ASSERT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
	} while (0)
#else /* UMR_CACHE */
// INVAL and UFFD_REG macros will be noop, so can use ASSERT_MRC_CACHED
#define ASSERT_MRC_CACHED_FLUSHED(cache, mrc) ASSERT_MRC_CACHED(cache, mrc)
#endif /* UMR_CACHE */

// this is only valid just before dereg_mr, when MR has all other resources
// gone except for the mrc->mr itself
#define ASSERT_MRC_PRE_DEREG(cache, mrc) do { \
		psmi_assert(! (mrc)->refcount); \
		ASSERT_MRC_INVAL_NOT_SET(mrc); \
		psmi_assert((mrc)->mr.mr_ptr); \
		ASSERT_NO_UFFD_REG(cache, mrc); \
		ASSERT_NOT_ON_MAP(cache, mrc); \
		ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
	} while (0)

#ifdef UMR_CACHE
#define ASSERT_MRC_PRE_DEREG_LOCK(cache, mrc) do { \
		if_pf (psm3_mr_cache_debug) { \
			PSMI_LOCK(psm3_uffd_lock); \
			ASSERT_MRC_PRE_DEREG(cache, mrc); \
			PSMI_UNLOCK(psm3_uffd_lock); \
		} \
	} while (0)
#else
#define ASSERT_MRC_PRE_DEREG_LOCK(cache, mrc) ASSERT_MRC_PRE_DEREG(cache, mrc);
#endif

#ifdef UMR_CACHE
// state specific to MR_CACHE_MODE_USER
#define ASSERT_MRC_INUSE_OR_CACHED(cache, mrc) do { \
		/* mrc->refcount can have any value */ \
		ASSERT_MRC_INVAL_NOT_SET(mrc); \
		psmi_assert((mrc)->mr.mr_ptr); \
		ASSERT_HAVE_UFFD_REG(cache, mrc); \
		ASSERT_ON_MAP(cache, mrc); \
		if_pf ((mrc)->refcount) /* INUSE */ \
			ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
		else /* CACHED */ \
			ASSERT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
	} while (0)

// state specific to MR_CACHE_MODE_USER
#define ASSERT_MRC_INVAL(cache, mrc) do { \
		psmi_assert((mrc)->refcount); \
		ASSERT_MRC_INVAL_SET(mrc); \
		psmi_assert((mrc)->mr.mr_ptr); \
		ASSERT_NO_UFFD_REG(cache, mrc); \
		ASSERT_NOT_ON_MAP(cache, mrc); \
		ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
		ASSERT_ON_INVAL_LIST(cache, mrc); \
	} while (0)

// state specific to MR_CACHE_MODE_USER
#define ASSERT_MRC_INUSE_OR_INVAL(cache, mrc) do { \
		psmi_assert((mrc)->refcount); \
		/* either value for mrc->inval valid */ \
		psmi_assert((mrc)->mr.mr_ptr); \
		if_pf ((mrc)->inval) { /* INVAL */ \
			ASSERT_NO_UFFD_REG(cache, mrc); \
			ASSERT_NOT_ON_MAP(cache, mrc); \
			ASSERT_ON_INVAL_LIST(cache, mrc); \
		} else { /* INUSE */ \
			ASSERT_HAVE_UFFD_REG(cache, mrc); \
			ASSERT_ON_MAP(cache, mrc); \
			ASSERT_NOT_ON_INVAL_LIST(cache, mrc); \
		} \
		ASSERT_NOT_ON_AVAIL_LIST(cache, mrc); \
	} while (0)

// state specific to MR_CACHE_MODE_USER
#define ASSERT_MRC_DEREG(cache, mrc) ASSERT_MRC_PRE_DEREG(cache, mrc)

#define ASSERT_MRC_DEREG_LOCK(cache, mrc) ASSERT_MRC_PRE_DEREG_LOCK(cache, mrc)
#endif /* UMR_CACHE */

#else /* PSM_DEBUG */
#define ASSERT_ON_MAP(cache, mrc)
#define ASSERT_NOT_ON_MAP(cache, mrc)
#define ASSERT_NOT_ON_AVAIL_LIST(cache, mrc)
#define ASSERT_NOT_ON_INVAL_LIST(cache, mrc)
#define ASSERT_ON_AVAIL_LIST(cache, mrc)
#define ASSERT_ON_INVAL_LIST(cache, mrc)
#define ASSERT_MRC_INVAL_SET(mrc)
#define ASSERT_MRC_INVAL_NOT_SET(mrc)
#define ASSERT_HAVE_UFFD_REG(cache, mrc)
#define ASSERT_NO_UFFD_REG(cache, mrc)
#define ASSERT_COUNTS_CONSISTENT(cache)
#define ASSERT_CACHE_EMPTY(cache)
#define ASSERT_MRC_FREE(cache, mrc)
#define ASSERT_MRC_FREE_LOCK(cache, mrc)
#define ASSERT_MRC_INUSE(cache, mrc)
#define ASSERT_MRC_CACHED(cache, mrc)
#define ASSERT_MRC_CACHED_FLUSHED(cache, mrc)
#define ASSERT_MRC_PRE_DEREG(cache, mrc)
#define ASSERT_MRC_PRE_DEREG_LOCK(cache, mrc)
#define ASSERT_MRC_INUSE_OR_CACHED(cache, mrc)
#define ASSERT_MRC_INVAL(cache, mrc)
#define ASSERT_MRC_INUSE_OR_INVAL(cache, mrc)
#define ASSERT_MRC_DEREG(cache, mrc)
#define ASSERT_MRC_DEREG_LOCK(cache, mrc)
#endif /* PSM_DEBUG */

// ---------------------------------------------------------------------------
// Statistics helper functions and macros

// "if" a little faster than "max()" for typical case of no change to max_stat
#define UPDATE_MAX_STAT(new_val, max_stat) \
	do { \
		if_pf ((new_val) > (max_stat)) \
			(max_stat) = (new_val); \
	} while (0)

#define INC_STAT(cache, stat, max_stat) \
	do { \
		if_pf (++((cache)->stat) > (cache)->max_stat) \
			(cache)->max_stat = (cache)->stat; \
	} while(0)

#define ADD_STAT(cache, adder, stat, max_stat) \
	do { \
		if_pf (((cache)->stat += (adder)) > (cache)->max_stat) \
			(cache)->max_stat = (cache)->stat; \
	} while(0)

// adjust statistics for an MR moving into INUSE state
static void update_stats_inc_inuse(psm2_mr_cache_t cache, uint64_t length,
					int access)
{
	INC_STAT(cache, inuse, max_inuse);
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
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
#ifdef PSM_HAVE_GPU
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			ADD_STAT(cache, length, gpu_inuse_recv_bytes, max_gpu_inuse_recv_bytes);
		else
#endif
#endif
			ADD_STAT(cache, length, inuse_recv_bytes, max_inuse_recv_bytes);
	} else {
		INC_STAT(cache, inuse_send, max_inuse_send);
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			ADD_STAT(cache, length, gpu_inuse_send_bytes, max_gpu_inuse_send_bytes);
		else
#endif
#endif
			ADD_STAT(cache, length, inuse_send_bytes, max_inuse_send_bytes);
	}
}

// adjust statistics for an MR moving out of INUSE or INVAL state
// (N/A to INUSE->INVAL transition)
static void update_stats_dec_inuse(psm2_mr_cache_t cache, uint64_t length,
					int access)
{
	cache->inuse--;
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
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
#ifdef PSM_HAVE_GPU
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			cache->gpu_inuse_recv_bytes -= length;
		else
#endif
#endif
			cache->inuse_recv_bytes -= length;
	} else {
		cache->inuse_send--;
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
		if (access & IBV_ACCESS_IS_GPU_ADDR)
			cache->gpu_inuse_send_bytes -= length;
		else
#endif
#endif
			cache->inuse_send_bytes -= length;
	}
}

// adjust statistics for psm3_verbs_reg_mr failure due to cache full
static void update_stats_inc_full(psm2_mr_cache_t cache, bool priority,
									int access)
{
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
}

// ---------------------------------------------------------------------------
// Cache initialization

#ifdef PSM_HAVE_RNDV_MOD
// MR_CACHE_MODE_KERNEL
// MR_CACHE_MODE_RV
// set limit_nonpri_inuse_bytes and limit_nonpri_gpu_inuse_bytes
psm2_error_t set_cache_limit_nonpri_rv_kern(psm2_mr_cache_t cache,
							uint32_t limit_entries,
							uint32_t pri_entries, uint64_t pri_size
#ifdef PSM_HAVE_GPU
							, uint64_t gpu_pri_size
#endif
							)
{
	psm2_ep_t ep = cache->ep;

	// TBD - could make this a warning and set limit_nonpri_inuse_bytes=0
	// then depend on transfers queuing and retrying until
	// reg_mr cache space is available
	if ((uint64_t)ep->rv_mr_cache_size*MEGABYTE < pri_size) {
		_HFI_ERROR("PSM3_RV_MR_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
			ep->rv_mr_cache_size, (pri_size + MEGABYTE-1)/MEGABYTE);
		return PSM2_PARAM_ERR;
	}
	cache->limit_nonpri_inuse_bytes = (uint64_t)ep->rv_mr_cache_size*MEGABYTE - pri_size;
#ifdef PSM_HAVE_GPU
	if (PSM3_GPU_IS_ENABLED) {
		// For GPU, due to GdrCopy, we can't undersize cache.
		// Otherwise RDMA MRs could consume all the
		// cache space and leave a gdrcopy pin/mmap stuck
		// retrying indefinitely.  If we want to allow undersize
		// GPU cache, we need to have gdrcopy pin/mmap failures
		// also invoke progress functions to release MRs
		if (PSM3_GPU_MIN_BAR_SIZE()) {
			uint64_t max_recommend = PSM3_GPU_MIN_BAR_SIZE() - 32*MEGABYTE;
			if ((uint64_t)ep->rv_gpu_cache_size*MEGABYTE >= max_recommend) {
				_HFI_INFO("Warning: PSM3_RV_GPU_CACHE_SIZE=%u too large for smallest GPU's BAR size of %"PRIu64" (< %"PRIu64" total of endpoint-rail-qp recommended)\n",
					ep->rv_gpu_cache_size,
					(PSM3_GPU_MIN_BAR_SIZE() + MEGABYTE-1)/MEGABYTE,
					max_recommend/MEGABYTE);
			}
		}
		if ((uint64_t)ep->rv_gpu_cache_size*MEGABYTE < gpu_pri_size) {
			_HFI_ERROR("PSM3_RV_GPU_CACHE_SIZE=%u too small, require >= %"PRIu64"\n",
				ep->rv_gpu_cache_size, (gpu_pri_size + MEGABYTE-1)/MEGABYTE);
			return PSM2_PARAM_ERR;
		}
		cache->limit_nonpri_gpu_inuse_bytes = (uint64_t)ep->rv_gpu_cache_size*MEGABYTE - gpu_pri_size;
	}
	_HFI_MMDBG("CPU cache %u GPU cache %u\n", ep->rv_mr_cache_size,
					ep->rv_gpu_cache_size);
#endif /* PSM_HAVE_GPU */
	return PSM2_OK;
}
#endif // PSM_HAVE_RNDV_MOD

#ifdef UMR_CACHE
// MR_CACHE_MODE_USER
#endif
// MR_CACHE_MODE_USER_NOINVAL
// set limit_nonpri_inuse_bytes, limit_nonpri_gpu_inuse_bytes N/A leave 0
psm2_error_t set_cache_limit_nonpri_user(psm2_mr_cache_t cache,
							uint32_t limit_entries,
							uint32_t pri_entries, uint64_t pri_size
#ifdef PSM_HAVE_GPU
							, uint64_t gpu_pri_size
#endif
							)
{
	cache->limit_nonpri_inuse_bytes = cache->limit_bytes - pri_size;
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
	// N/A to GPU
	cache->limit_nonpri_gpu_inuse_bytes = 0;
#endif
#endif
	return PSM2_OK;
}

// MR_CACHE_MODE_NONE
// set limit_nonpri_inuse_bytes, limit_nonpri_gpu_inuse_bytes N/A leave 0
psm2_error_t set_cache_limit_nonpri_none(psm2_mr_cache_t cache,
							uint32_t limit_entries,
							uint32_t pri_entries, uint64_t pri_size
#ifdef PSM_HAVE_GPU
							, uint64_t gpu_pri_size
#endif
							)
{
	cache->limit_nonpri_inuse_bytes = UINT64_MAX;
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
	// N/A to GPU
	cache->limit_nonpri_gpu_inuse_bytes = 0;
#endif
#endif
	return PSM2_OK;
}

// ep is used for RNDV_MOD, memory tracking and stats
psm2_mr_cache_t psm3_verbs_alloc_mr_cache(psm2_ep_t ep,
							uint32_t limit_entries, uint8_t cache_mode,
							uint32_t limit_size_mb,
							uint32_t pri_entries, uint64_t pri_size
#ifdef PSM_HAVE_GPU
							, uint64_t gpu_pri_size
#endif
							)
{
	struct psm2_mr_cache *cache;
	psm2_error_t err;

#ifdef PSM_DEBUG
	union psmi_envvar_val env_val;

	psm3_getenv("PSM3_MR_CACHE_DEBUG",
			"Enable MR Cache debug consistency checks (0 disables)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			(union psmi_envvar_val)0, &env_val);
	psm3_mr_cache_debug= env_val.e_uint;
#endif

	cache = (struct psm2_mr_cache *)psmi_calloc(ep, DESCRIPTORS,
							sizeof(*cache), 1);
	if (! cache)
		return NULL;

	// limit_entries for a pool must be power of 2
	limit_entries = max(limit_entries, pri_entries);
	limit_entries = psm3_next_power2(limit_entries);
	cache->limit_entries = limit_entries;
	if (MR_CACHE_USER_CACHING(cache_mode))
		cache->limit_bytes = (uint64_t)limit_size_mb * MEGABYTE;
	else
		cache->limit_bytes = UINT64_MAX;
	cache->alloced_entries = 0;
	cache->max_alloced_entries = 0;
	cache->registered_bytes = 0;
	cache->max_registered_bytes = 0;
	cache->cache_mode = cache_mode;
	cache->access = ep->mr_access?IBV_ACCESS_REMOTE_WRITE:0;
	// we leave headroom for priority registrations
	cache->limit_nonpri_inuse = limit_entries - pri_entries;
	cache->ep = ep;

	switch (cache_mode) {
	case MR_CACHE_MODE_NONE:
		cache->reg_mr_fn = psm3_verbs_reg_mr_not_user;
		cache->release_mr_fn = psm3_verbs_release_mr_not_user;
		err = set_cache_limit_nonpri_none(cache, limit_entries, pri_entries, pri_size
#ifdef PSM_HAVE_GPU
										, gpu_pri_size
#endif
										);
		break;
#ifdef PSM_HAVE_RNDV_MOD
	case MR_CACHE_MODE_KERNEL:
	case MR_CACHE_MODE_RV:
		cache->reg_mr_fn = psm3_verbs_reg_mr_not_user;
		cache->release_mr_fn = psm3_verbs_release_mr_not_user;
		err = set_cache_limit_nonpri_rv_kern(cache, limit_entries, pri_entries, pri_size
#ifdef PSM_HAVE_GPU
										, gpu_pri_size
#endif
										);
		break;
#endif /* PSM_HAVE_RDNV_MOD */
#ifdef UMR_CACHE
	case MR_CACHE_MODE_USER:
		cache->reg_mr_fn = psm3_verbs_reg_mr_user;
		cache->release_mr_fn = psm3_verbs_release_mr_user;
		err = set_cache_limit_nonpri_user(cache, limit_entries, pri_entries, pri_size
#ifdef PSM_HAVE_GPU
										, gpu_pri_size
#endif
										);
		break;
#endif /* UMR_CACHE */
	case MR_CACHE_MODE_USER_NOINVAL:
		cache->reg_mr_fn = psm3_verbs_reg_mr_not_user;
		cache->release_mr_fn = psm3_verbs_release_mr_user_noinval;
		err = set_cache_limit_nonpri_user(cache, limit_entries, pri_entries, pri_size
#ifdef PSM_HAVE_GPU
										, gpu_pri_size
#endif
										);
		break;
	default:
		psmi_assert_always(0);
		err = PSM2_INTERNAL_ERR;
		break;
	}
	if (err)
		goto fail;

#ifdef PSM_HAVE_RNDV_MOD
	cache->rv = ep->rv;
	cache->cmd_fd = ep->cmd_fd;
#endif // PSM_HAVE_RNDV_MOD
	cache->pd = ep->pd;
#if defined(PSM_HAVE_RNDV_MOD) && defined(PSM_HAVE_GPU)
	_HFI_MMDBG("cache alloc: limit_entries=%u limit_bytes=%"PRIu64" limit_nonpri_inuse=%u limit_nonpri_inuse_bytes=%"PRIu64" limit_nonpri_gpu_inuse_bytes=%"PRIu64", pri_entries=%u pri_size=%"PRIu64" gpu_pri_size=%"PRIu64"\n",
			cache->limit_entries, cache->limit_bytes, cache->limit_nonpri_inuse,
			cache->limit_nonpri_inuse_bytes, cache->limit_nonpri_gpu_inuse_bytes,
			pri_entries, pri_size, gpu_pri_size);
#else
	_HFI_MMDBG("cache alloc: limit_entries=%u limit_bytes=%"PRIu64" limit_nonpri_inuse=%u limit_nonpri_inuse_bytes=%"PRIu64", pri_entries=%u pri_size=%"PRIu64"\n",
			cache->limit_entries, cache->limit_bytes, cache->limit_nonpri_inuse,
			cache->limit_nonpri_inuse_bytes, pri_entries, pri_size);
#endif
	// limit_entries must be power of 2>= obj per chunk which is also power of 2
	cache->mr_pool = psm3_mpool_create(sizeof(cl_map_item_t),
						min(128, limit_entries), limit_entries, 0,
						DESCRIPTORS, NULL, NULL);
	if (! cache->mr_pool)
		goto fail;
	//nil_item already zeroed by calloc
	//memset(&cache->nil_item.payload, 0, sizeof(cache->nil_item.payload));
	ips_cl_qmap_init(&cache->map, &cache->root, &cache->nil_item);
	TAILQ_INIT(&cache->avail_list);
#ifdef UMR_CACHE
	// done last since events could start arriving due to other EPs
	if (ep->mr_cache_mode == MR_CACHE_MODE_USER) {
		if (psm3_verbs_umrc_init(ep, cache) != PSM2_OK)
			goto fail;
	}
#endif /* UMR_CACHE */
	ASSERT_CACHE_EMPTY(cache);

	register_cache_stats(cache);
	return cache;

fail:
	if (cache) {
		if (cache->mr_pool)
			psm3_mpool_destroy(cache->mr_pool);
		psmi_free(cache);
	}
	return NULL;
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
		case MR_CACHE_MODE_USER:
			return 1;
		case MR_CACHE_MODE_RV:
			return psmi_hal_has_cap(PSM_HAL_CAP_USER_MR);
		default:	// unexpected
			return 0;
	}
}

// ---------------------------------------------------------------------------
// Cache lower level basic operations needed as part of psm3_verbs_reg_mr and
// psm3_verbs_release_mr, UMR_CACHE invalidate, and MR cache shutdown

// checks for space for a non-priority registration or CACHED->INUSE transition
static inline int have_nonpri_space(psm2_mr_cache_t cache, uint64_t length, int access)
{
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
	if (access & IBV_ACCESS_IS_GPU_ADDR)
		return (cache->inuse < cache->limit_nonpri_inuse
			&& cache->gpu_inuse_bytes + length < cache->limit_nonpri_gpu_inuse_bytes);
	else
#endif
#endif
		return (cache->inuse < cache->limit_nonpri_inuse
			&& cache->inuse_bytes + length < cache->limit_nonpri_inuse_bytes);
}

// after a miss, checks cache against its limit and indicates how to proceed
// ENOMEM - at non-priority limit
// ENOSPC - need to evict some
// 0 - can allocate a fresh mrc
static int check_cache_size(psm2_mr_cache_t cache, bool priority,
							psm3_verbs_mr_t key)
{
	if (! priority && ! have_nonpri_space(cache, key->length, key->access)) {
		_HFI_MMDBG("rejected: no headroom for non-priority miss "MRC_FMT"\n",
					MR_OUT_MRC(key));
		cache->rejected++;
		return ENOMEM;
	}
	if (cache->registered_bytes + key->length > cache->limit_bytes) {
		_HFI_MMDBG("need evict: bytes for miss "MRC_FMT" registered %"PRIu64" limit %"PRIu64"\n",
				MR_OUT_MRC(key), cache->registered_bytes, cache->limit_bytes);
		return ENOSPC;	// must evict
	}

	// we only reuse entries from avail_list once cache is at size limit
	// this helps improve cache hit rate.
	if (cache->alloced_entries >= cache->limit_entries) {
		_HFI_MMDBG("need evict: entries for miss "MRC_FMT" alloced %u limit %u\n",
				MR_OUT_MRC(key), cache->alloced_entries,cache->limit_entries);
		return ENOSPC;	// must evict
	} else {
		return 0;	// can just alloc a new MR
	}
}

// allocate an mrc from the pool
// caller must complete initialization of mrc
// caller must NOT hold uffd_lock, caller must hold progress_lock
static psm3_verbs_mr_t alloc_mr(psm2_mr_cache_t cache)
{
	PSMI_LOCK_ASSERT(cache->ep->mq->progress_lock);
	// assert caller properly checked we have space
	psmi_assert(cache->alloced_entries < cache->limit_entries);
	cl_map_item_t *p_item = (cl_map_item_t *)psm3_mpool_get(cache->mr_pool);
	if (! p_item) {	// keep KW happy, should not happen, we check max above
		_HFI_ERROR("unexpected cache pool allocate failure\n");
		cache->failed++;
		cache->failed_alloc_mr++;
		return NULL;
	}
	INC_STAT(cache, alloced_entries, max_alloced_entries);
	// the mpool does not zero item's it allocates, so must do it ourself
	p_item->payload.mr.mr_ptr = NULL;
	p_item->payload.refcount = 0;
#ifdef UMR_CACHE
	p_item->payload.reg = NULL;
	p_item->payload.inval = 0;
#endif
	ASSERT_MRC_FREE_LOCK(cache, &p_item->payload);
	return &p_item->payload;
}

// free an mrc to the pool
// caller must have already dereg_mr, removed from all lists and from map
// caller must NOT hold uffd_lock, caller must hold progress_lock
static inline void free_mr(psm2_mr_cache_t cache, psm3_verbs_mr_t mrc)
{
	PSMI_LOCK_ASSERT(cache->ep->mq->progress_lock);
	ASSERT_MRC_FREE_LOCK(cache, mrc);
	psm3_mpool_put(container_of(mrc, cl_map_item_t, payload));
	cache->alloced_entries--;
}

// add the MR to the cache map (after a cache miss)
// caller must hold the uffd_lock when MR_CACHE_MODE_USER
void insert_mr(psm2_mr_cache_t cache, psm3_verbs_mr_t mrc, bool priority)
{
	cl_map_item_t *p_item = container_of(mrc, cl_map_item_t, payload);

	cache->miss++;
	ips_cl_qmap_insert_item(&cache->map, p_item);
	UPDATE_MAX_STAT(cache->map.payload.nelems, cache->max_nelems);
	update_stats_inc_inuse(cache, mrc->length, mrc->access);
	_HFI_MMDBG("registered new MR pri %d "MRC_FMT" ref %u ptr %p nelems %u\n",
				priority, MR_OUT_MRC(mrc), mrc->refcount, mrc,
				cache->map.payload.nelems);
	ASSERT_MRC_INUSE(cache, mrc);
}

// remove the MR from the cache map
// caller must also ensure not on avail_list before or soon after this call
// for MR_CACHE_MODE_USER, caller must hold uffd_lock (except during shutdown)
static inline void remove_mr(psm2_mr_cache_t cache, psm3_verbs_mr_t mrc)
{
	ASSERT_ON_MAP(cache, mrc);
	ASSERT_NOT_ON_INVAL_LIST(cache, mrc);
	ips_cl_qmap_remove_item(&cache->map,
								container_of(mrc, cl_map_item_t, payload));
}

// search cache for an entry, on hit return the item
// If hit, but can't start new non-priority IO, returns NULL and errno=ENOMEM
// returns:
//	mrc - cache hit, mrc now INUSE
//	NULL, errno=ENOMEM - hit but can't move another MR to INUSE
//	NULL, errno=ENOENT - miss
// for MR_CACHE_MODE_USER, caller must hold uffd_lock
static psm3_verbs_mr_t lookup_mr(psm2_mr_cache_t cache, bool priority,
								psm3_verbs_mr_t key)

{
	psm3_verbs_mr_t mrc;
	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, key);
	if (p_item->payload.mr.mr_ptr) {
		psmi_assert(p_item != cache->map.nil_item);
		mrc = &p_item->payload;
		if (! mrc->refcount) {
			if (! priority && ! have_nonpri_space(cache, key->length, key->access)) {
				_HFI_MMDBG("rejected: no headroom for non-priority hit "MRC_FMT
							" ptr %p\n", MR_OUT_MRC(key), mrc);
				cache->rejected++;
				errno = ENOMEM;
				return NULL;
			}
			// CACHED->INUSE
			ASSERT_MRC_CACHED(cache, mrc);
			// it was an entry on avail_list, take off list
			TAILQ_REMOVE(&cache->avail_list, mrc, next);
			update_stats_inc_inuse(cache, mrc->length, mrc->access);
		} else {
			// INUSE->INUSE
			ASSERT_MRC_INUSE(cache, mrc);
		}
		cache->hit++;
		mrc->refcount++;
		UPDATE_MAX_STAT(mrc->refcount, cache->max_refcount);
		_HFI_MMDBG("hit: MR "MRC_FMT" ref %u ptr %p\n",
					MR_OUT_MRC(key), mrc->refcount, mrc);
		ASSERT_MRC_INUSE(cache, mrc);
		return mrc;
	}
	psmi_assert(p_item == cache->map.nil_item);
	errno = ENOENT;
	return NULL;
}

// get an MR from the avail_list
// returns:
//	mrc - MR taken from avail_list and removed from map, caller must dereg_mr
//	NULL, errno=ENOMEM - cache full (avail_list empty)
// for MR_CACHE_MODE_USER, caller must hold uffd_lock
static psm3_verbs_mr_t get_avail_mr(psm2_mr_cache_t cache, bool priority,
								psm3_verbs_mr_t key)
{
	psm3_verbs_mr_t mrc;

	// we only have items on avail_list when MR_CACHE_MODE_USER*
	mrc = TAILQ_FIRST(&cache->avail_list);
	if (! mrc) {
		_HFI_MMDBG("full: none avail for "MRC_FMT"\n", MR_OUT_MRC(key));
		update_stats_inc_full(cache, priority, key->access);
		errno = ENOMEM;
		return NULL;
	}
	// CACHED->FREE on way to INUSE for a new MR
	ASSERT_MRC_CACHED(cache, mrc);
	TAILQ_REMOVE(&cache->avail_list, mrc, next);
	remove_mr(mrc->cache, mrc);
	_HFI_MMDBG("evict MR "MRC_FMT" ptr %p for "MRC_FMT"\n",
				MR_OUT_MRC(mrc), mrc, MR_OUT_MRC(key));
	// caller will dereg_mr
	return mrc;
}

// deregister MR and clear mr_ptr
// On failure to deregister, still clear mr_ptr (leak the fouled up MR)
// caller must ensure removed from avail_list and inval_list before this call
// caller must NOT hold uffd_lock, caller must hold progress_lock
static void dereg_mr(psm2_mr_cache_t cache, psm3_verbs_mr_t mrc)
{
	int ret;

	PSMI_LOCK_ASSERT(cache->ep->mq->progress_lock);
	ASSERT_MRC_PRE_DEREG_LOCK(cache, mrc);
	_HFI_MMDBG("dereg MR "MRC_FMT"\n", MR_OUT_MRC(mrc));
#ifdef PSM_HAVE_RNDV_MOD
	// should not happen
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL
		|| cache->cache_mode == MR_CACHE_MODE_RV) {
		ret = psm3_rv_dereg_mem(cache->rv, mrc->mr.rv_mr);
		if (ret)
			_HFI_ERROR("psm3_rv_dereg_mem failed: "MRC_FMT": %s\n",
				MR_OUT_MRC(mrc), strerror(errno));
	} else
#endif
	{
		ret = ibv_dereg_mr(mrc->mr.ibv_mr);
		if (ret)
			_HFI_ERROR("ibv_dereg_mr failed: "MRC_FMT": %s\n",
				MR_OUT_MRC(mrc), strerror(ret));
	}
	mrc->mr.mr_ptr = NULL;
	cache->registered_bytes -= mrc->length;
}

// register the MR and prepare the remaining mrc fields for addition
// to the cache.  Caller must add mr to map.
// returns:
//     mrc - exact same mrc given, registration successful
//     NULL - unable to register MR
// returning mrc done as a convenience to simplify flow in caller
// can't ASSERT_MRC_state at end of this function since on success caller may
// have more work to finish registration
// caller must NOT hold uffd_lock
static psm3_verbs_mr_t prep_and_reg_mr(psm2_mr_cache_t cache,
								psm3_verbs_mr_t mrc, bool priority,
								psm3_verbs_mr_t key)
{
	int save_errno;
	static int fail_cnt = 0; /* Number of failed priority reg_mr requests */

	ASSERT_MRC_FREE_LOCK(cache, mrc);
#ifdef PSM_HAVE_RNDV_MOD
	/* need cmd_fd for access to ucontext when convert user pd into kernel pd */
	if (cache->cache_mode == MR_CACHE_MODE_KERNEL) {
		// user space QPs for everything
		mrc->mr.rv_mr = psm3_rv_reg_mem(cache->rv, cache->cmd_fd, cache->pd,
										key->addr, key->length, key->access
#ifdef PSM_HAVE_GPU
										, &key->gpu_specific
#endif
										);
		if (! mrc->mr.rv_mr) {
			save_errno = errno;
#ifdef PSM_HAVE_GPU
			if (save_errno == ENOMEM && priority)
				(void)psm3_gpu_evict_some(cache->ep, key->length, key->access);
#endif
			goto failed_reg_mr;
		}
		mrc->iova = mrc->mr.rv_mr->iova;
		mrc->lkey = mrc->mr.rv_mr->lkey;
		mrc->rkey = mrc->mr.rv_mr->rkey;
	} else if (cache->cache_mode == MR_CACHE_MODE_RV) {
		/* For kernel MR (used by kernel QP), need to check the buffer size */
		if ((key->access & IBV_ACCESS_RDMA) &&
		     key->length > cache->ep->verbs_ep.max_fmr_size) {
			if (priority)
				_HFI_ERROR("Req MR "MRC_FMT" too large: max_fmr_size(0x%"PRIx64")\n",
					   MR_OUT_MRC(key), cache->ep->verbs_ep.max_fmr_size);
			save_errno = EINVAL;
			goto failed_reg_mr;
		}
		// kernel QP for RDMA, user QP for send DMA
		mrc->mr.rv_mr = psm3_rv_reg_mem(cache->rv, cache->cmd_fd,
										(key->access&IBV_ACCESS_RDMA)?NULL
													:cache->pd,
										key->addr, key->length, key->access
#ifdef PSM_HAVE_GPU
										, &key->gpu_specific
#endif
										);
		if (! mrc->mr.rv_mr) {
			save_errno = errno;
#ifdef PSM_HAVE_GPU
			if (save_errno == ENOMEM && priority)
				(void)psm3_gpu_evict_some(cache->ep, key->length, key->access);
#endif
			goto failed_reg_mr;
		}
		mrc->iova = mrc->mr.rv_mr->iova;
		mrc->lkey = mrc->mr.rv_mr->lkey;
		mrc->rkey = mrc->mr.rv_mr->rkey;
	} else
#endif /* PSM_HAVE_RNDV_MOD */
	{
		// user space QPs for everything
		mrc->mr.ibv_mr = ibv_reg_mr(cache->pd, key->addr, key->length, key->access);
		if (! mrc->mr.ibv_mr) {
			save_errno = EINVAL;
			goto failed_reg_mr;
		}
		mrc->iova = (uintptr_t)key->addr;
		mrc->lkey = mrc->mr.ibv_mr->lkey;
		mrc->rkey = mrc->mr.ibv_mr->rkey;
	}
	mrc->cache = cache;
	mrc->refcount = 1;
	mrc->addr = key->addr;
	mrc->length = key->length;
	mrc->access = key->access;
#ifdef PSM_HAVE_GPU
	mrc->gpu_specific = key->gpu_specific;
#endif
	ADD_STAT(cache, mrc->length, registered_bytes, max_registered_bytes);
	/* Reset the fail counter */
	fail_cnt = 0;
	return mrc;

failed_reg_mr:
	if (priority) {
		/* Print the first failure */
		if (!fail_cnt)
			_HFI_ERROR("reg_mr failed: "MRC_FMT": %s\n",
				   MR_OUT_MRC(key), strerror(save_errno));
		fail_cnt++;
		/* Print a warning after consecutive failures */
		if (fail_cnt == psm3_reg_mr_warn_cnt)
			_HFI_ERROR("reg_mr failed %d times in a row.\n",
				   psm3_reg_mr_warn_cnt);
		/* Bail out if it fails too many times */
		if (fail_cnt >= psm3_reg_mr_fail_limit)
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "reg_mr failed for too many times.\n");
	}
	cache->failed++;
	cache->failed_reg_mr++;
	free_mr(cache, mrc);
	errno = save_errno;
	return NULL;
}

// ---------------------------------------------------------------------------
// Cache basic functional operations (psm3_verbs_reg_mr and
// psm3_verbs_release_mr with cache lookup/handling)

// MR_CACHE_MODE_NONE
#ifdef PSM_HAVE_RNDV_MOD
// MR_CACHE_MODE_KERNEL
// MR_CACHE_MODE_RV
#endif
// MR_CACHE_MODE_USER_NOINVAL
static psm3_verbs_mr_t psm3_verbs_reg_mr_not_user(psm2_mr_cache_t cache,
					 bool priority, psm3_verbs_mr_t key)
{
	psm3_verbs_mr_t mrc = lookup_mr(cache, priority, key);
	if (mrc)
		return mrc;	// hit
	else if (!mrc && errno != ENOENT)
		return NULL; // no space

	// miss, get an mrc to use
	errno = check_cache_size(cache, priority, key);
	if (errno == ENOMEM) {
		return NULL; // no space
	} else if (errno == ENOSPC) {
		// evict some (applicable to USER_NOINVAL)
		while (1) {
			mrc = get_avail_mr(cache, priority, key);
			if (! mrc)
				goto done; // no space
			dereg_mr(cache, mrc);
			cache->evict++;
			if (cache->registered_bytes + key->length <= cache->limit_bytes)
				break;
			free_mr(cache, mrc);
		}
	} else {
		mrc = alloc_mr(cache);	// get a fresh one
		if (! mrc)
			goto done;
	}

	// now register new MR using mrc, FREE->INUSE
	mrc = prep_and_reg_mr(cache, mrc, priority, key);
	if (mrc)
		insert_mr(cache, mrc, priority);
done:
	ASSERT_COUNTS_CONSISTENT(cache);
	return mrc;
}

#ifdef UMR_CACHE
// MR_CACHE_MODE_USER
static psm3_verbs_mr_t psm3_verbs_reg_mr_user(psm2_mr_cache_t cache,
					 bool priority, psm3_verbs_mr_t key)
{
	psmi_assert(cache->cache_mode == MR_CACHE_MODE_USER);
	PSMI_LOCK(psm3_uffd_lock);
	if (cache->umr_cache.event_queue)
		psm3_verbs_umrc_event_queue_process(cache);
	psm3_verbs_mr_t mrc = lookup_mr(cache, priority, key);
	if (mrc) {
		PSMI_UNLOCK(psm3_uffd_lock);
		return mrc;	// hit
	} else if (!mrc && errno != ENOENT) {
		PSMI_UNLOCK(psm3_uffd_lock);
		return NULL; // no space
	}

	// miss, get an mrc to use
	errno = check_cache_size(cache, priority, key);
	if (errno == ENOMEM) {
		PSMI_UNLOCK(psm3_uffd_lock);
		return NULL; // no space
	} else if (errno == ENOSPC) {
		// evict some
		while (1) {
			mrc = get_avail_mr(cache, priority, key);
			if (! mrc) {
				PSMI_UNLOCK(psm3_uffd_lock);
				goto done; // no space
			}
			if (mrc->reg) {
				psm3_uffd_deregister((uintptr_t)mrc->addr, mrc->length,
								mrc->reg, PSM3_UFFD_EVENT_NONE, 0, UINT64_MAX);
				mrc->reg = NULL;
			}
			PSMI_UNLOCK(psm3_uffd_lock);
			// we have only reference to mrc
			dereg_mr(cache, mrc);
			cache->evict++;
			if (cache->registered_bytes + key->length <= cache->limit_bytes)
				break;
			free_mr(cache, mrc);
			PSMI_LOCK(psm3_uffd_lock);
		}
	} else {
		PSMI_UNLOCK(psm3_uffd_lock);
		// we have only reference to mrc, no need to hold locks
		mrc = alloc_mr(cache);	// get a fresh one
		if (! mrc)
			goto done;
	}

	// now register new MR using mrc, FREE->INUSE
	mrc = prep_and_reg_mr(cache, mrc, priority, key);
	if (mrc) {
		psmi_assert(! mrc->reg);
		PSMI_LOCK(psm3_uffd_lock);
		mrc->reg = psm3_uffd_register((uintptr_t)key->addr, key->length);
		if (! mrc->reg) {
			// can't get uffd region registered, caller will try again later
			mrc->refcount--;
			PSMI_UNLOCK(psm3_uffd_lock);
			cache->failed++;
			cache->umr_cache.stats.failed_uffd_reg++;
			dereg_mr(cache, mrc);
			free_mr(cache, mrc);
			errno = ENOMEM;
			mrc = NULL;
			goto done;
		}
		insert_mr(cache, mrc, priority);
		PSMI_UNLOCK(psm3_uffd_lock);
	}

done:
	ASSERT_COUNTS_CONSISTENT(cache);
	return mrc;
}
#endif /* UMR_CACHE */

// each attempt will increment exactly one of: hit, miss, rejected, full, failed
struct psm3_verbs_mr * psm3_verbs_reg_mr(psm2_mr_cache_t cache, bool priority,
				void *addr, uint64_t length, int access)
{

	PSMI_LOCK_ASSERT(cache->ep->mq->progress_lock);
	psmi_assert(cache->pd);
	if (! cache->pd)
		return NULL;
	ASSERT_COUNTS_CONSISTENT(cache);

#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_reg_mr, "reg_mr",
				"MR cache full, any request type",
				1, IPS_FAULTINJ_REG_MR);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_reg_mr, cache->ep, "")) {
			update_stats_inc_full(cache, priority, access);
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(!priority && PSM3_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_nonpri_reg_mr, "nonpri_reg_mr",
				"MR cache full, non-priority request",
				1, IPS_FAULTINJ_NONPRI_REG_MR);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_nonpri_reg_mr, cache->ep, "")) {
			cache->rejected++;
			errno = ENOMEM;
			return NULL;
		}
	}
	if_pf(priority && PSM3_FAULTINJ_ENABLED_EP(cache->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_pri_reg_mr, "pri_reg_mr",
				"MR cache full, priority request",
				1, IPS_FAULTINJ_PRI_REG_MR);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_pri_reg_mr, cache->ep, "")) {
			update_stats_inc_full(cache, priority, access);
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
		cache->failed_gpu_reg++;
		errno = EINVAL;
		return NULL;
	}
#else
#ifdef PSM_HAVE_GPU
	psmi_assert(!!(access & IBV_ACCESS_IS_GPU_ADDR) == (PSM3_IS_GPU_MEM(addr)));
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
		PSM3_GPU_ROUNDUP_RV_REG_MR(cache->ep, &addr, &length, access);
	} else
#endif /* PSM_HAVE_GPU */
#endif /* PSM_HAVE_RNDV_MOD */
	{
		/* Round up cpu buffers */
		uint64_t start = (uint64_t)addr;
		uint64_t end = ROUNDUP64P2(start + length, PSMI_PAGESIZE);

		start = ROUNDDOWN64P2(start, PSMI_PAGESIZE);
		addr = (void *)start;
		length = end - start;
	}

	struct psm3_verbs_mr key = { // our search key
		.addr = addr,
		.length = length,
		.access = access,
	};
#ifdef PSM_HAVE_GPU
	PSM3_GPU_INIT_MR(addr, length, access, &key.gpu_specific);
#endif
	_HFI_MMDBG("pri %d "MRC_FMT"\n", priority, MR_OUT_MRC(&key));

	return (*cache->reg_mr_fn)(cache, priority, &key);
}

// done as a macro so calling function reported in message
#define LOG_FREE_MRC(mrc, msg) do { \
	_HFI_MMDBG("freeing %sMR "MRC_FMT" ref %u ptr %p nelems %u\n", msg, \
			MR_OUT_MRC(mrc), mrc->refcount, mrc, \
			mrc->cache->map.payload.nelems); \
} while (0)

// MR_CACHE_MODE_NONE
#ifdef PSM_HAVE_RNDV_MOD
// MR_CACHE_MODE_KERNEL
// MR_CACHE_MODE_RV
#endif
static void psm3_verbs_release_mr_not_user(psm3_verbs_mr_t mrc)
{
	psm2_mr_cache_t cache = mrc->cache;

	ASSERT_MRC_INUSE(cache, mrc);
	mrc->refcount--;
	if (!mrc->refcount) {
		// INUSE->FREE
		update_stats_dec_inuse(cache, mrc->length, mrc->access);
		LOG_FREE_MRC(mrc, "");
		remove_mr(cache, mrc);
		dereg_mr(cache, mrc);
		free_mr(cache, mrc);
	}
	ASSERT_COUNTS_CONSISTENT(cache);
}

#ifdef UMR_CACHE
// MR_CACHE_MODE_USER
static void psm3_verbs_release_mr_user(psm3_verbs_mr_t mrc)
{
	psm2_mr_cache_t cache = mrc->cache;

	PSMI_LOCK_ASSERT(cache->ep->mq->progress_lock);
	PSMI_LOCK(psm3_uffd_lock);

	ASSERT_MRC_INUSE_OR_INVAL(cache, mrc);
	mrc->refcount--;
	if (!mrc->refcount) {
		update_stats_dec_inuse(cache, mrc->length, mrc->access);
		if (!mrc->inval) {
			// INUSE->CACHED
			TAILQ_INSERT_TAIL(&cache->avail_list, mrc, next);
			ASSERT_MRC_CACHED(cache, mrc);
		} else {
			// INVAL->FREE
			LOG_FREE_MRC(mrc, "Inval ");
			mrc->inval = 0;
			TAILQ_REMOVE(&cache->umr_cache.inval_list, mrc, next);
			PSMI_UNLOCK(psm3_uffd_lock);
			dereg_mr(cache, mrc);
			free_mr(cache, mrc);
			ASSERT_COUNTS_CONSISTENT(cache);
			return;
		}
	}
	PSMI_UNLOCK(psm3_uffd_lock);
	ASSERT_COUNTS_CONSISTENT(cache);
}
#endif /* UMR_CACHE */

// MR_CACHE_MODE_USER_NOINVAL
static void psm3_verbs_release_mr_user_noinval(psm3_verbs_mr_t mrc)
{
	psm2_mr_cache_t cache = mrc->cache;

	ASSERT_MRC_INUSE(cache, mrc);
	mrc->refcount--;
	if (!mrc->refcount) {
		update_stats_dec_inuse(cache, mrc->length, mrc->access);
		// INUSE->CACHED
		// put on avail_list to be reclaimed if needed
		TAILQ_INSERT_TAIL(&cache->avail_list, mrc, next);
		ASSERT_MRC_CACHED(cache, mrc);
	}
	ASSERT_COUNTS_CONSISTENT(cache);
}

void psm3_verbs_release_mr(struct psm3_verbs_mr *mrc)
{
	if (! mrc) {
		//errno = EINVAL;
		return;
	}
	if (! mrc->refcount) {
		//errno = ENXIO;
		return;
	}
	_HFI_MMDBG("releasing MR "MRC_FMT" ref %u ptr %p\n",
				MR_OUT_MRC(mrc), mrc->refcount, mrc);
	ASSERT_COUNTS_CONSISTENT(mrc->cache);
	(*mrc->cache->release_mr_fn)(mrc);
}

// ---------------------------------------------------------------------------
// Cache finalization for shutdown

void psm3_verbs_free_mr_cache(psm2_mr_cache_t cache)
{
	// don't pollute stats with our shutdown activity
#ifdef PSM_HAVE_GPU
#ifdef PSM_HAVE_RNDV_MOD
	if (cache->rv && PSM3_GPU_IS_ENABLED)
		psm3_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE,
					&cache->rv_gpu_stats);
#endif
#endif
	psm3_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE, cache);

#ifdef UMR_CACHE
	if (cache->cache_mode == MR_CACHE_MODE_USER) {
		if (PSM2_OK != psm3_verbs_umrc_fini(cache))
			return; // IOs in flight, leak the rest, let process exit cleanup
	}
	// no more uffd events nor callbacks for this cache, don't need uffd_lock
#endif /* UMR_CACHE */

	// free all cached entries, should find no INUSE or INVAL
	while (cache->map.payload.nelems) {
		cl_map_item_t *p_item = __cl_map_root(&cache->map);
		psmi_assert(p_item != cache->map.nil_item);
		// CACHED->FREE
		psm3_verbs_mr_t mrc = &p_item->payload;
		ASSERT_MRC_CACHED_FLUSHED(cache, mrc);
		if_pt (mrc->mr.mr_ptr) { // conservative, should always be true
			if (mrc->refcount) {
				_HFI_ERROR("unreleased MR in psm3_verbs_free_mr_cache "MRC_FMT"\n",
							MR_OUT_MRC(mrc));
				return; // leak the rest, let process exit cleanup
			}
			LOG_FREE_MRC(mrc, "");
			mrc->refcount = 0;
			TAILQ_REMOVE(&cache->avail_list, mrc, next);
			remove_mr(cache, mrc);
			dereg_mr(cache, mrc);
			free_mr(cache, mrc);
		}
	}
	ASSERT_CACHE_EMPTY(cache);
	psm3_mpool_destroy(cache->mr_pool);
	psmi_free(cache);
}

#ifdef UMR_CACHE
// ----------------------------------------------------------------------------
// UserFaultfd initialization. registration and event polling/processing

#ifdef PSM_DEBUG
// Confirm no MRs from first to p_stop overlap (not including p_stop).
// To check all MRs pass cache->map.nil_item as p_stop.
static void assert_all_gone(psm2_mr_cache_t cache, cl_map_item_t *p_stop,
							uint64_t addr, uint64_t length)
{
	if (! psm3_mr_cache_debug)
		return;

	cl_map_item_t *p_item = ips_cl_qmap_first(&cache->map);
	int missed = 0;
	while (p_item != p_stop) {
		psm3_verbs_mr_t mrc = &p_item->payload;
		// confirm ! overlap
		if((addr < (uintptr_t)mrc->addr + mrc->length && addr + length
				> (uintptr_t)mrc->addr)) {
			_HFI_INFO("event "MR_RANGE_FMT" missed "MRC_FMT"\n",
						MR_OUT_RANGE(addr, length), MR_OUT_MRC(mrc));
			missed++;
		}
		p_item = ips_cl_qmap_successor(&cache->map, p_item);
	}
	if (missed) {
		_HFI_INFO("missed %d\n", missed);
		ips_cl_qmap_dump(&cache->map);
		psmi_assert(0);
	}
}
#else
#define assert_all_gone(cache, p_stop, addr, length)
#endif /* PSM_DEBUG */

// invalidate an MR in the cache.
// Called as part of processing uffd events.
static void invalidate_mr(psm2_mr_cache_t cache, psm3_verbs_mr_t mrc,
							uint8_t reason, uint64_t addr, uint64_t length)
{
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	_HFI_MMDBG("invalidate "MRC_FMT" reason=%u, ref %u ptr %p\n",
				MR_OUT_MRC(mrc), reason, mrc->refcount, mrc);
	ASSERT_MRC_INUSE_OR_CACHED(cache, mrc);
	if (mrc->reg) {	// be paranoid, should always be true
		psm3_uffd_deregister((uintptr_t)mrc->addr, mrc->length, mrc->reg,
								reason, addr, length);
		mrc->reg = NULL;
	}
	// always take off map so mrc will not appear in psm3_verbs_reg_mr search
	remove_mr(cache, mrc);
	if (!mrc->refcount) {
		uint32_t cnt;
		// CACHED->DEREG
		TAILQ_REMOVE(&cache->avail_list, mrc, next);
		cache->umr_cache.stats.remove++;
		// can't risk a free() in ibv_dereg_mr so do in worker
		ASSERT_MRC_DEREG(cache, mrc);
		cnt = PSM3_ATOMIC_INC(cache->umr_cache.dereg_queued_cnt);
		if (cnt > cache->umr_cache.stats.max_dereg_queued_cnt)
			cache->umr_cache.stats.max_dereg_queued_cnt = cnt;
		psm3_uffd_queue_dereg(mrc);
	} else {
		// cannot be deleted now so mark as inval, INUSE->INVAL
		cache->umr_cache.stats.inval++;
		mrc->inval = 1;
		TAILQ_INSERT_TAIL(&cache->umr_cache.inval_list, mrc, next);
		ASSERT_MRC_INVAL(cache, mrc);
	}
}

// process a uffd event which indicates a given addr/length has changed or
// freed its virtual address.  So all cached MRs which overlap that address
// range must now be invalidated.
// caller has psm3_uffd_lock, must be careful not to alloc/free memory
// during this call
static void psm3_verbs_umrc_event_process(psm2_mr_cache_t cache, uint64_t addr,
								uint64_t length, uint8_t reason)
{
	uint64_t endp1 = addr + length;
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	cl_map_item_t *p_item;
	_HFI_MMDBG("cache=%p "MR_RANGE_FMT" reason=%u nelms=%d\n",
				cache, MR_OUT_RANGE(addr, length), reason,
				cache->map.payload.nelems);
	// In many cases the matching MRs will be clustered as immediate successors
	// of the first match, so using ips_cl_qmap_search_successor can
	// provide an optimization.  In the worst case search_successor could
	// be 2(log N) [go up whole tree, then search down right leg of whole tree]
	// vs the (log N) of ips_cl_qmap_search, but our use of RBTREE_ITERATOR
	// should help reduce that worse case.
	cl_map_item_t *p_next;
	for (p_item = ips_cl_qmap_search(&cache->map, addr, endp1);

			p_item != cache->map.nil_item;
			p_item = p_next)
	{
		psm3_verbs_mr_t mrc = &p_item->payload;
		p_next = ips_cl_qmap_search_successor(&cache->map, p_item, addr, endp1);
		// assert overlaps
		psmi_assert(addr < (uintptr_t)mrc->addr + mrc->length
					&& endp1 > (uintptr_t)mrc->addr);
		// confirm search() returned 1st which overlaps
		assert_all_gone(cache, p_item, addr, length);
		invalidate_mr(cache, mrc, reason, addr, length);
	}
	assert_all_gone(cache, cache->map.nil_item, addr, length);
}

// caller has psm3_uffd_lock, must be careful not to alloc/free memory
// during this call
static void psm3_verbs_umrc_event_queue_process(psm2_mr_cache_t cache)
{
	uint64_t i;

	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	for (i=0; i<cache->umr_cache.queue_cnt; i++) {
		psm2_umrc_event_t e = &cache->umr_cache.event_queue[i];
		psm3_verbs_umrc_event_process(cache, e->addr, e->length, e->reason);
	}
	cache->umr_cache.queue_cnt = 0;
}

// for MR_CACHE_MODE_USER, initialize cache->umr_cache
static psm2_error_t psm3_verbs_umrc_init(psm2_ep_t ep, psm2_mr_cache_t cache)
{
	psm2_error_t err;
	psm2_umr_cache_t umrc = &cache->umr_cache;
	static int have_value = 0;
	static unsigned saved;
	union psmi_envvar_val env_val;

	TAILQ_INIT(&cache->umr_cache.inval_list);

	// TBD - should we allow to be specified per Multi-EP open?
	if (have_value) {
		// only fetch on 1st use
		env_val.e_uint = saved;
	} else {
		// event queue may improve handling of uffd events in some cases
		psm3_getenv("PSM3_MR_CACHE_EVENT_QUEUE",
			"User MR Cache event queue size (0 disables)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &env_val);
		saved = env_val.e_uint;
		have_value = 1;
	}
	if (env_val.e_uint) {
		umrc->queue_depth = env_val.e_uint;
		umrc->event_queue = (struct psm2_umrc_event *)psmi_calloc(PSMI_EP_NONE, UNDEFINED,
							sizeof(struct psm2_umrc_event), umrc->queue_depth);
		if (! umrc->event_queue) {
			_HFI_ERROR("Couldn't allocate memory for event_queue.\n");
			return PSM2_NO_MEMORY;
		}
	}
	if ((err = psm3_uffd_add_callback(cache, cache->limit_entries))) {
		if (umrc->event_queue)
			psmi_free(umrc->event_queue);
		umrc->event_queue = NULL;
	}

	return err;
}

// deregister all uffd_regions for entries in cache
// Only entries in cache->map can have a mrc->reg, so just loop on cache->map.
static void flush_uffd_regions(psm2_mr_cache_t cache, int do_dereg)
{
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	cl_map_item_t *p_item = ips_cl_qmap_first(&cache->map);
	uint32_t flushed = 0;

	_HFI_MMDBG("Flush UFFD regions do_dereg=%d\n", do_dereg);
	for (;
		 p_item != cache->map.nil_item;
		 p_item = ips_cl_qmap_successor(&cache->map, p_item)) {
		psm3_verbs_mr_t mrc = &p_item->payload;
		if (mrc->reg) {
			if (do_dereg)
				psm3_uffd_deregister((uintptr_t)mrc->addr, mrc->length,
								mrc->reg, PSM3_UFFD_EVENT_NONE, 0, UINT64_MAX);
			mrc->reg = NULL;
			flushed++;
		}
	}
	_HFI_MMDBG("Done Flush UFFD regions: %u flushed\n", flushed);
}

// for MR_CACHE_MODE_USER, finalize cache->umr_cache
// only return an error if we find IOs still in flight
//
// An opened EP may consist of multiple rails or QPs each with an MR cache.
// If we are not the last, we must flush_uffd_regions because once our caller
// calls ibv_dereg_mr, we risk getting real page faults for those regions.
// UMR_FAST_FLUSH enables an optimization to short cut some of this for the
// final MR cache's free.
static psm2_error_t psm3_verbs_umrc_fini(psm2_mr_cache_t cache)
{
	psm3_verbs_mr_t mrc;
	psm2_umr_cache_t umrc = &cache->umr_cache;

	PSMI_LOCK_ASSERT(psm3_creation_lock);
	PSMI_LOCK_ASSERT(cache->ep->mq->progress_lock);
	PSMI_LOCK(psm3_uffd_lock);
	// process here just so don't attempt to unregister invalidated MRs
	// process events so we know which MRs don't need unregister calls
	if (umrc->event_queue)
		psm3_verbs_umrc_event_queue_process(cache);

#ifdef UMR_FAST_FLUSH
	flush_uffd_regions(cache, psm3_uffd_get_num_callback() > 1);
#else
	flush_uffd_regions(cache, 1);
#endif

	psm3_uffd_remove_callback(cache);
	PSMI_UNLOCK(psm3_uffd_lock);
	// no more events and no more event callbacks coming

	// need to wait for all MRs from this cache instance to be mpool_put
	// yielding progress_lock should be ok since caller also has creation_lock
	// and that will prevent rcvThread from accessing this ep
	while (PSM3_ATOMIC_GET(cache->umr_cache.dereg_queued_cnt))
		PSMI_YIELD(cache->ep->mq->progress_lock);
	// no more events, event callbacks nor dereg_worker callbacks coming

	// if we are last, stop uffd threads and destroy tracker
	psm3_uffd_tracker_fini();

	if (umrc->event_queue) {
		psmi_assert(!cache->umr_cache.queue_cnt);
		psmi_free(umrc->event_queue);
		umrc->event_queue = NULL;
	}

	// check inval_list after cleaning up tracker.  If check fails, we
	// avoid other surprises such as real page faults from userfaultfd.
	// MRs on inval_list will only experience ibv_dereg_mr and mpool_put
	// if IO completes so don't need tracker anymore.

	// don't really need lock since no more events or callbacks coming,
	// but play it safe
	PSMI_LOCK(psm3_uffd_lock);
	// any left on inval_list implies we freed mr cache with IOs in flight
	mrc = TAILQ_FIRST(&cache->umr_cache.inval_list);
	if (mrc) {
		psmi_assert(mrc->refcount);
		_HFI_ERROR("unreleased MR with IO in flight "MRC_FMT"\n",
					MR_OUT_MRC(mrc));
		PSMI_UNLOCK(psm3_uffd_lock);
		return PSM2_INTERNAL_ERR;	// leak the rest, let process exit cleanup
	}
	psmi_assert(TAILQ_EMPTY(&cache->umr_cache.inval_list));
	PSMI_UNLOCK(psm3_uffd_lock);

	return PSM2_OK;
}

// ----------------------------------------------------------------------------
// UserFaultfd event and dreg_worker callbacks

// process a uffd event immediately or by queueing on event queue
// caller has psm3_uffd_lock, must be careful not to realloc/free memory
// during this call
void psm3_verbs_umrc_uffd_callback(psm2_mr_cache_t cache, uint64_t addr,
									uint64_t len, uint8_t reason)
{
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	if (cache->umr_cache.event_queue) {
		psm2_umr_cache_t umrc = &cache->umr_cache;
		// get an unused event from our pool, since we queue events
		// and then process all in order, we can simply index our pool
		if (umrc->queue_cnt < umrc->queue_depth) {
			psm2_umrc_event_t e = &umrc->event_queue[umrc->queue_cnt];
			INC_STAT(cache, umr_cache.queue_cnt, umr_cache.stats.max_queue_cnt);
			e->addr = addr;
			e->length = len;
			e->reason = reason;
			_HFI_MMDBG("uffd event queued: cache=%p "MR_RANGE_FMT" reason=%u queue_cnt=%u\n",
						cache, MR_OUT_RANGE(addr, len), reason,
						umrc->queue_cnt);
			return;
		}
		// event queue full must process immediately
		_HFI_MMDBG("unable to queue uffd event: event queue full\n");
		umrc->stats.queue_full++;
	}
	_HFI_MMDBG("uffd event: cache=%p "MR_RANGE_FMT" reason=%u\n",
				cache, MR_OUT_RANGE(addr, len), reason);
	psm3_verbs_umrc_event_process(cache, addr, len, reason);
}

// callback from uffd worker thread to complete dereg
// MR_CACHE_MODE_USER, so must be ibv_mr
// When called the mrc itself is now off the dereg_list so this routine is the
// only reference and can't race with any other threads trying to access mrc
void psm3_verbs_umrc_worker_dereg(struct psm3_verbs_mr *mrc)
{
	psm2_mr_cache_t cache = mrc->cache;
	int err;

	// we chose to do ibv_dereg_mr outside the progress_lock to reduce
	// jitter.  However, that means we can't use dereg_mr(), so replicate its
	// MR_CACHE_MODE_USER code here
	ASSERT_MRC_PRE_DEREG_LOCK(cache, mrc);
	_HFI_MMDBG("dereg MR "MRC_FMT" ptr %p \n", MR_OUT_MRC(mrc), mrc);
	err = ibv_dereg_mr(mrc->mr.ibv_mr);
	if (err)
		_HFI_ERROR("ibv_dereg_mr failed: "MRC_FMT": %s\n",
			MR_OUT_MRC(mrc), strerror(err));
	mrc->mr.mr_ptr = NULL;
	PSMI_LOCK(cache->ep->mq->progress_lock);
	cache->registered_bytes -= mrc->length;
	free_mr(cache, mrc);
	PSMI_UNLOCK(cache->ep->mq->progress_lock);

	// must do this last so on EP close, free_mr_cache can wait until
	// all MRs are fully dereg to ensure EP doesn't close verbs while MR
	// calls in progress
	PSM3_ATOMIC_DEC(cache->umr_cache.dereg_queued_cnt);
}
#endif /* UMR_CACHE */

// ----------------------------------------------------------------------------
// Statistics Reporting

// accessor functions for statistics
#define CACHE_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = (psm2_mr_cache_t)context; \
		return cache->stat; \
    }
// avoids showing redundant or N/A stats when not MR_CACHE_MODE_USER*
#define USER_CACHE_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = (psm2_mr_cache_t)context; \
		return MR_CACHE_USER_CACHING(cache->cache_mode) ? cache->stat : 0; \
    }

// avoids showing redundant or N/A stats when not MR_CACHE_MODE_USER
#define UMR_CACHE_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = (psm2_mr_cache_t)context; \
		return (cache->cache_mode == MR_CACHE_MODE_USER) ? cache->stat : 0; \
    }

CACHE_STAT_FUNC(mr_cache_mode, cache_mode)
CACHE_STAT_FUNC(mr_cache_access, ep->mr_access)
CACHE_STAT_FUNC(mr_cache_limit_entries, limit_entries)
USER_CACHE_STAT_FUNC(mr_cache_limit_bytes, limit_bytes)
CACHE_STAT_FUNC(mr_cache_nelems, map.payload.nelems)
CACHE_STAT_FUNC(mr_cache_max_nelems, max_nelems)
UMR_CACHE_STAT_FUNC(mr_cache_alloced_entries, alloced_entries)
UMR_CACHE_STAT_FUNC(mr_cache_max_alloced_entries, max_alloced_entries)
USER_CACHE_STAT_FUNC(mr_cache_registered_bytes, registered_bytes)
USER_CACHE_STAT_FUNC(mr_cache_max_registered_bytes, max_registered_bytes)
CACHE_STAT_FUNC(mr_cache_limit_nonpri_inuse, limit_nonpri_inuse)
CACHE_STAT_FUNC(mr_cache_inuse, inuse)
CACHE_STAT_FUNC(mr_cache_max_inuse, max_inuse)
CACHE_STAT_FUNC(mr_cache_inuse_recv, inuse_recv)
CACHE_STAT_FUNC(mr_cache_max_inuse_recv, max_inuse_recv)
CACHE_STAT_FUNC(mr_cache_inuse_send, inuse_send)
CACHE_STAT_FUNC(mr_cache_max_inuse_send, max_inuse_send)
CACHE_STAT_FUNC(mr_cache_max_refcount, max_refcount)
#ifdef RBTREE_VALIDATE
CACHE_STAT_FUNC(mr_cache_tree_depth, map.depth)
CACHE_STAT_FUNC(mr_cache_tree_max_depth, map.max_depth)
#endif
#ifdef UMR_CACHE
// these will be zero when not MR_CACHE_MODE_USER
CACHE_STAT_FUNC(mr_cache_umrc_queue_depth, umr_cache.queue_depth)
CACHE_STAT_FUNC(mr_cache_umrc_queue_cnt, umr_cache.queue_cnt)
static uint64_t mr_cache_umrc_dereg_queued_cnt(void *context)
{
	psm2_mr_cache_t cache = (psm2_mr_cache_t)context;
	return PSM3_ATOMIC_GET(cache->umr_cache.dereg_queued_cnt);
}
#endif /* UMR_CACHE */
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
static uint64_t mr_cache_rv_limit_size(void *context)
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
	return cache->rv_stats.limit_cache_size;
}

#define CACHE_RV_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = (psm2_mr_cache_t)context; \
		return cache->rv_stats.stat; \
    }

CACHE_RV_STAT_FUNC(mr_cache_rv_size, cache_size/MEGABYTE)
CACHE_RV_STAT_FUNC(mr_cache_rv_max_size, max_cache_size/MEGABYTE)
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

#ifdef PSM_HAVE_GPU
static uint64_t mr_cache_rv_gpu_limit_size(void *context)
{
	psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats);
	if (cache->rv && PSM3_GPU_IS_ENABLED ) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
		(void)psm3_rv_gpu_get_cache_stats(cache->rv, &cache->rv_gpu_stats);
	}
	return cache->rv_gpu_stats.limit_cache_size;
}

#define CACHE_RV_GPU_STAT_FUNC(func, stat) \
    static uint64_t func(void *context) \
    { \
		psm2_mr_cache_t cache = container_of(context, struct psm2_mr_cache, rv_gpu_stats); \
		return cache->rv_gpu_stats.stat; \
    }

CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size, cache_size/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size_reg, cache_size_reg/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size_mmap, cache_size_mmap/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_size_both, cache_size_both/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size, max_cache_size/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size_reg, max_cache_size_reg/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size_mmap, max_cache_size_mmap/MEGABYTE)
CACHE_RV_GPU_STAT_FUNC(mr_cache_rv_gpu_max_size_both, max_cache_size_both/MEGABYTE)
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
#endif /* PSM_HAVE_GPU */

#endif // PSM_HAVE_RNDV_MOD

static void register_cache_stats(psm2_mr_cache_t cache)
{
	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL_HELP("User Space MR Cache Configuration:"),
		PSMI_STATS_DECL("cache_mode",
				"MR cache mode",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_mode, NULL),
		PSMI_STATS_DECL("mr_access",
				"When register MR for send, should inbound recv access be allowed",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_access, NULL),
		PSMI_STATS_DECL_FUNC("limit_entries",
				"Limit of MRs in cache",
				mr_cache_limit_entries),
		PSMI_STATS_DECL_FUNC("limit_bytes",
				"Limit of total MR bytes in cache",
				mr_cache_limit_bytes),
		PSMI_STATS_DECL("limit_nonpri_inuse",
				"Limit of non-priority MRs with an active IO in cache",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_limit_nonpri_inuse, NULL),
		PSMI_STATS_DECL("limit_nonpri_inuse_bytes",
				"Limit of total registered non-priority MR bytes in cache",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &cache->limit_nonpri_inuse_bytes),
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECL("limit_nonpri_gpu_inuse_bytes",
				"Limit of total registered non-priority GPU MR bytes in cache",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &cache->limit_nonpri_gpu_inuse_bytes),
#endif
#endif
#ifdef UMR_CACHE
		PSMI_STATS_DECL_FUNC("umrc_uffd_queue_depth",
				"Memory event queue depth",
				mr_cache_umrc_queue_depth),
#endif
		// -----------------------------------------------------------
		PSMI_STATS_DECL_HELP("User Space MR Cache Statistics:"),
		PSMI_STATS_DECL_FUNC("nelems",
				"Current MRs in cache",
				mr_cache_nelems),
		PSMI_STATS_DECL_FUNC("max_nelems",
				"Max observed MRs in cache",
				mr_cache_max_nelems),
		PSMI_STATS_DECL_FUNC("alloced_entries",
				"Current allocated cache entries",
				mr_cache_alloced_entries),
		PSMI_STATS_DECL_FUNC("max_alloced_entries",
				"Max observed allocated cache entries",
				mr_cache_max_alloced_entries),
		PSMI_STATS_DECL_FUNC("registered_bytes",
				"Current total MR bytes in cache",
				mr_cache_registered_bytes),
		PSMI_STATS_DECL_FUNC("max_registered_bytes",
				"Max observed total MR bytes in cache",
				mr_cache_max_registered_bytes),
		PSMI_STATS_DECL_FUNC("inuse",
				"Current MRs with an active IO",
				mr_cache_inuse),
		PSMI_STATS_DECL_FUNC("max_inuse",
				"Max observed MRs with an active IO",
				mr_cache_max_inuse),
		// if cache->access recv and send inuse stats will remain 0
		PSMI_STATS_DECL_FUNC("inuse_recv",
				"Current MRs with an active inbound RDMA",
				mr_cache_inuse_recv),
		PSMI_STATS_DECL_FUNC("max_inuse_recv",
				"Max observed MRs with an active inbound RDMA",
				mr_cache_max_inuse_recv),
		PSMI_STATS_DECL_FUNC("inuse_send",
				"Current MRs with an active outbound DMA or RDMA",
				mr_cache_inuse_send),
		PSMI_STATS_DECL_FUNC("max_inuse_send",
				"Max observed MRs with an active outbound DMA or RDMA",
				mr_cache_max_inuse_send),
		PSMI_STATS_DECLU64("inuse_bytes",
				"Current registered MR bytes with an active IO",
				&cache->inuse_bytes),
		PSMI_STATS_DECLU64("max_inuse_bytes",
				"Max observed registered MR bytes with an active IO",
				&cache->max_inuse_bytes),
		// if cache->access recv and send inuse stats will remain 0
		PSMI_STATS_DECLU64("inuse_recv_bytes",
				"Current registered MR bytes with an active receive IO",
				&cache->inuse_recv_bytes),
		PSMI_STATS_DECLU64("max_inuse_recv_bytes",
				"Max observed registered MR bytes with an active receive IO",
				&cache->max_inuse_recv_bytes),
		PSMI_STATS_DECLU64("inuse_send_bytes",
				"Current register MR bytes with an active send IO",
				&cache->inuse_send_bytes),
		PSMI_STATS_DECLU64("max_inuse_send_bytes",
				"Max observed registered MR bytes with an active send IO",
				&cache->max_inuse_send_bytes),
#ifdef UMR_CACHE
		PSMI_STATS_DECLU64("umrc_remove",
				"MRs removed from cache due to application memory free",
				&cache->umr_cache.stats.remove),
		PSMI_STATS_DECLU64("umrc_inval",
				"MRs with application memory free while IO in flight",
				&cache->umr_cache.stats.inval),
		PSMI_STATS_DECL_FUNC("umrc_uffd_queue",
				"Memory event current queue depth",
				mr_cache_umrc_queue_cnt),
		PSMI_STATS_DECLU64("umrc_uffd_max_queue",
				"Memory event max observed queue depth",
				&cache->umr_cache.stats.max_queue_cnt),
		PSMI_STATS_DECLU64("umrc_uffd_queue_full",
				"Memory event queue overflow",
				&cache->umr_cache.stats.queue_full),
		PSMI_STATS_DECL_FUNC("umrc_uffd_dereg_queue",
				"dereg worker current queue depth",
				mr_cache_umrc_dereg_queued_cnt),
		PSMI_STATS_DECLU64("umrc_uffd_max_dereg_queue",
				"dereg worker max observed queue depth",
				&cache->umr_cache.stats.max_dereg_queued_cnt),
#endif /* UMR_CACHE */
#ifdef PSM_HAVE_RNDV_MOD
#ifdef PSM_HAVE_GPU
		PSMI_STATS_DECLU64("gpu_inuse_bytes",
				"Current registered GPU MR bytes with an active IO",
				&cache->gpu_inuse_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_bytes",
				"Max observed registered GPU MR bytes with an active IO",
				&cache->max_gpu_inuse_bytes),
		// if cache->access recv and send inuse stats will remain 0
		PSMI_STATS_DECLU64("gpu_inuse_recv_bytes",
				"Current registered GPU MR bytes with an active receive IO",
				&cache->gpu_inuse_recv_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_recv_bytes",
				"Max observed registered GPU MR bytes with an active receive IO",
				&cache->max_gpu_inuse_recv_bytes),
		PSMI_STATS_DECLU64("gpu_inuse_send_bytes",
				"Current registered GPU MR bytes with an active send IO",
				&cache->gpu_inuse_send_bytes),
		PSMI_STATS_DECLU64("max_gpu_inuse_send_bytes",
				"Max observed registered GPU MR bytes with an active send IO",
				&cache->max_gpu_inuse_send_bytes),
#endif
#endif
		PSMI_STATS_DECL_FUNC("max_refcount",
				"Max observed concurrent IOs on a single MR",
				mr_cache_max_refcount),
#ifdef RBTREE_VALIDATE
		PSMI_STATS_DECL_FUNC("tree_depth",
				"Current MR search tree depth",
				mr_cache_tree_depth),
		PSMI_STATS_DECL_FUNC("tree_max_depth",
				"Max observed MR search tree depth",
				mr_cache_tree_max_depth),
#endif
		PSMI_STATS_DECLU64("hit",
				"Number of hits",
				&cache->hit),
		PSMI_STATS_DECL("hit_%",
				"Cache hit rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_hit_rate, NULL),
		PSMI_STATS_DECLU64("miss",
				"Number of misses",
				&cache->miss),
		PSMI_STATS_DECL("miss_%",
				"Cache miss rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_miss_rate, NULL),
		PSMI_STATS_DECLU64("evict",
				"Number removed from cache due to no space",
				&cache->evict),
		PSMI_STATS_DECLU64("rejected",
				"Non-priority MR registrations rejected due to space",
				&cache->rejected),
		PSMI_STATS_DECLU64("full",
				"MR registrations which failed due to space",
				&cache->full),
		// if cache->access recv and send full stats will remain 0
		// otherwise full_pri and nonpri remain 0
		PSMI_STATS_DECLU64("full_pri",
				"Priority MR registrations which failed due to space",
				&cache->full_pri),
		PSMI_STATS_DECLU64("full_pri_recv",
				"Priority MR registrations for receive IO failed due to space",
				&cache->full_pri_recv),
		PSMI_STATS_DECLU64("full_pri_send",
				"Priority MR registrations for send IO which failed due space",
				&cache->full_pri_send),
		PSMI_STATS_DECLU64("full_nonpri",
				"Non-priority MR registrations which failed due space",
				&cache->full_nonpri),
		PSMI_STATS_DECLU64("full_nonpri_recv",
				"Non-priority MR registrations for receive IO which failed due space",
				&cache->full_nonpri_recv),
		PSMI_STATS_DECLU64("full_nonpri_send",
				"Non-priority MR registrations for send IO which failed due to space",
				&cache->full_nonpri_send),
		PSMI_STATS_DECLU64("failed",
				"Count of unexpected failures to register or allocate cache entries",
				&cache->failed),
		PSMI_STATS_DECLU64("failed_alloc_mr",
				"Count of unexpected failures to allocate cache entries",
				&cache->failed_alloc_mr),
		PSMI_STATS_DECLU64("failed_reg_mr",
				"Count of unexpected failures to register MR",
				&cache->failed_reg_mr),
#ifndef PSM_HAVE_RNDV_MOD
		PSMI_STATS_DECLU64("failed_gpu_reg",
				"Count of invalid attempts to register GPU memory",
				&cache->failed_gpu_reg),
#endif
#ifdef UMR_CACHE
		PSMI_STATS_DECLU64("failed_uffd_reg",
				"Count of failures to register userfault fd region",
				&cache->umr_cache.stats.failed_uffd_reg),
#endif
#ifdef PSM_HAVE_RNDV_MOD
		// -----------------------------------------------------------
		PSMI_STATS_DECL_HELP("Kernel RV Cache Configuration:"),
		PSMI_STATS_DECL_FUNC("rv_limit",
				"Max allowed cache size in MBs",
				mr_cache_rv_limit_size),
		// -----------------------------------------------------------
		PSMI_STATS_DECL_HELP("Kernel RV Cache Statistics:"),
		PSMI_STATS_DECL_FUNC("rv_size",
				"Current cache size in MBs",
				mr_cache_rv_size),
		PSMI_STATS_DECL_FUNC("rv_max_size",
				"Max observed cache size in MBs",
				mr_cache_rv_max_size),
		PSMI_STATS_DECL_FUNC("rv_nelems",
				"Current MRs in cache",
				mr_cache_rv_nelems),
		PSMI_STATS_DECL_FUNC("rv_max_nelems",
				"Max observed MRs in cache",
				mr_cache_rv_max_nelems),
		PSMI_STATS_DECL_FUNC("rv_inuse",
				"Current MRs in use",
				mr_cache_rv_inuse),
		PSMI_STATS_DECL_FUNC("rv_max_inuse",
				"Max observed MRs in use",
				mr_cache_rv_max_inuse),
		PSMI_STATS_DECLU64("rv_inuse_bytes",
				"Current MR bytes in use",
				(uint64_t*)&cache->rv_stats.inuse_bytes),
		PSMI_STATS_DECLU64("rv_max_inuse_bytes",
				"Max observed MR bytes in use",
				(uint64_t*)&cache->rv_stats.max_inuse_bytes),
		PSMI_STATS_DECL_FUNC("rv_max_refcount",
				"Max observed concurrent IOs on a single MR",
				mr_cache_rv_max_refcount),
		PSMI_STATS_DECLU64("rv_hit",
				"Number of hits",
				(uint64_t*)&cache->rv_stats.hit),
		PSMI_STATS_DECL("rv_hit_%",
				"Cache hit rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_hit_rate, NULL),
		PSMI_STATS_DECLU64("rv_miss",
				"Number of miss with succesful add",
				(uint64_t*)&cache->rv_stats.miss),
		PSMI_STATS_DECL("rv_miss_%",
				"Cache miss rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_miss_rate, NULL),
		PSMI_STATS_DECLU64("rv_full",
				"Number of miss but can't add since no space",
				(uint64_t*)&cache->rv_stats.full),
		PSMI_STATS_DECLU64("rv_failed",
				"Number of miss but can't add since reg_mr failed",
				(uint64_t*)&cache->rv_stats.failed),
		PSMI_STATS_DECLU64("rv_remove",
				"Number removed from cache due to application memory free",
				(uint64_t*)&cache->rv_stats.remove),
		PSMI_STATS_DECLU64("rv_evict",
				"Number removed from cache due to no space",
				(uint64_t*)&cache->rv_stats.evict),
#endif // PSM_HAVE_RNDV_MOD
	};
	psm3_stats_register_type("MR_Cache_Statistics",
#ifdef PSM_HAVE_RNDV_MOD
			"User space (User) and Kernel RV MR cache for an endpoint in the process.\n"
#else
			"User space (User) MR cache for an endpoint in the process\n"
#endif
			"PSM3 retains a user space MR cache entry for each MR "
			"currently in use.  In some cases a given MR may be "
			"concurrently used (refcount) by more than one IO or "
			"by multiple window size chunks of the same message buffer.\n"
#ifdef UMR_CACHE
			"When enabled the PSM3 user space MR cache may also "
			"retain MRs after use to optimize future messages "
			"which may reuse the same application buffer.\n"
#endif
#ifdef PSM_HAVE_RNDV_MOD
			"When enabled PSM3 may use the Kernel RV module to "
			"cache MRs for use by both user space and kernel DMA "
			"and RDMA transfers.  RV may retain MRs after use to "
			"optimize future messages.\n"
#endif
			"The cache(s) are configured with a limit on their "
			"size in terms of bytes cached and number of entries. "
			"This helps to avoid excessive pinning of memory or "
			"excessive use of NIC MR resources.\n"
			"If the cache reaches it's size limit and a new entry "
			"needs to be added, an older entry is 'evicted' to "
			"make room.  When the application frees memory which "
			"matches a cached MR, the MR is 'removed' once all "
			"outstanding IOs are done (typically there are none).\n"
			"Each new IO checks the cache for an existing entry. "
			"IOs which get a 'hit' are able to use an existing "
			"entry and incur less overhead.  IOs which 'miss' "
			"will need to pin memory and create a new entry.\n"
			"Applications which observe a poor 'hit' rate may "
			"be inefficiently freeing and reallocating buffers "
			"and may actually perform better without RDMA.\n"
			"PSM3 attempts to allocate cache entries as early "
			"and as large as possible, however such allocations "
			"are non-priority' and are not allowed to consume the "
			"the last remaining space in the cache.\n"
			"Statistics for MR send vs recv use are only tracked "
			"when mr_access is 0.\n",
					PSMI_STATSTYPE_MR_CACHE,
					entries,
					PSMI_HOWMANY(entries),
					psm3_epid_fmt_internal(cache->ep->epid, 0), cache,
					cache->ep->dev_name);
#ifdef PSM_HAVE_GPU
#ifdef PSM_HAVE_RNDV_MOD
	struct psmi_stats_entry gpu_entries[] = {
		PSMI_STATS_DECL_HELP("Kernel RV GPU Cache Configuration:"),
		PSMI_STATS_DECL_FUNC("rv_gpu_limit",
				"max allowed GPU cache size in MBs",
				mr_cache_rv_gpu_limit_size),
		// -----------------------------------------------------------
		PSMI_STATS_DECL_HELP("Kernel RV GPU Cache Statistics:"),
		PSMI_STATS_DECL_FUNC("rv_gpu_size",
				"Current cache size in MBs",
				mr_cache_rv_gpu_size),
		PSMI_STATS_DECL_FUNC("rv_gpu_size_reg",
				"Current cache size for MRs in MBs",
				mr_cache_rv_gpu_size_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_size_mmap",
				"Current cache size for mmap in MBs",
				mr_cache_rv_gpu_size_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_size_both",
				"Current cache size for MR&mmap in MBs",
				mr_cache_rv_gpu_size_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size",
				"Max observed cache size in MBs",
				mr_cache_rv_gpu_max_size),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size_reg",
				"Max observed cache size for MRs in MBs",
				mr_cache_rv_gpu_max_size_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size_mmap",
				"Max observed cache size for mmap in MBs",
				mr_cache_rv_gpu_max_size_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_size_both",
				"Max observed cache size for MR&mmap in MBs",
				mr_cache_rv_gpu_max_size_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems",
				"Current entries in cache",
				mr_cache_rv_gpu_nelems),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems_reg",
				"Current MR entries in cache",
				mr_cache_rv_gpu_nelems_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems_mmap",
				"Current mmap entries in cache",
				mr_cache_rv_gpu_nelems_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_nelems_both",
				"Current MR&mmap entries in cache",
				mr_cache_rv_gpu_nelems_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems",
				"Max observed entries in cache",
				mr_cache_rv_gpu_max_nelems),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems_reg",
				"Max observed MR entries in cache",
				mr_cache_rv_gpu_max_nelems_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems_mmap",
				"Max observed mmap entries in cache",
				mr_cache_rv_gpu_max_nelems_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_nelems_both",
				"Max observed MR&mmap entries in cache",
				mr_cache_rv_gpu_max_nelems_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse",
				"Current entries in use",
				mr_cache_rv_gpu_inuse),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse_reg",
				"Current MR entries in use",
				mr_cache_rv_gpu_inuse_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse_mmap",
				"Current mmap entries in use",
				mr_cache_rv_gpu_inuse_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_inuse_both",
				"Current MR&mmap entries in use",
				mr_cache_rv_gpu_inuse_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse",
				"Max observed entries in use",
				mr_cache_rv_gpu_max_inuse),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse_reg",
				"Max observed MR entries in use",
				mr_cache_rv_gpu_max_inuse_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse_mmap",
				"Max observed mmap entries in use",
				mr_cache_rv_gpu_max_inuse_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_inuse_both",
				"Max observed MR&mmap entries in use",
				mr_cache_rv_gpu_max_inuse_both),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes",
				"Current bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.inuse_bytes),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes_reg",
				"Current MR bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.inuse_bytes_reg),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes_mmap",
				"Current mmap bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.inuse_bytes_mmap),
		PSMI_STATS_DECLU64("rv_gpu_inuse_bytes_both",
				"Current MR&mmap bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.inuse_bytes_both),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes",
				"Max observed bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes_reg",
				"Max observed MR bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes_reg),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes_mmap",
				"Max observed mmap bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes_mmap),
		PSMI_STATS_DECLU64("rv_gpu_max_inuse_bytes_both",
				"Max observed MR&mmap bytes in use",
				(uint64_t*)&cache->rv_gpu_stats.max_inuse_bytes_both),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount",
				"Max observed concurrent IOs on a single entry",
				mr_cache_rv_gpu_max_refcount),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount_reg",
				"Max observed concurrent IOs on a single MR entry",
				mr_cache_rv_gpu_max_refcount_reg),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount_mmap",
				"Max observed concurrent IOs on a single mmap entry",
				mr_cache_rv_gpu_max_refcount_mmap),
		PSMI_STATS_DECL_FUNC("rv_gpu_max_refcount_both",
				"Max observed concurrent IOs on a single MR&mmap entry",
				mr_cache_rv_gpu_max_refcount_both),
		PSMI_STATS_DECLU64("rv_gpu_hit",
				"Overall Number of hits",
				(uint64_t*)&cache->rv_gpu_stats.hit),
		PSMI_STATS_DECL("rv_gpu_hit_%",
				"Overall Cache hit rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_reg",
				"Number of MR hits",
				(uint64_t*)&cache->rv_gpu_stats.hit_reg),
		PSMI_STATS_DECL("rv_gpu_hit_reg_%",
				"Cache reg_mr hit rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_reg, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_add_reg",
				"Number of reg_mr hits on mmapped pages",
				(uint64_t*)&cache->rv_gpu_stats.hit_add_reg),
		PSMI_STATS_DECL("rv_gpu_hit_add_reg_%",
				"Cache reg_mr hit on mmap rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_add_reg, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_mmap",
				"Number of mmap hits",
				(uint64_t*)&cache->rv_gpu_stats.hit_mmap),
		PSMI_STATS_DECL("rv_gpu_hit_mmap_%",
				"Cache mmap hit rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_mmap, NULL),
		PSMI_STATS_DECLU64("rv_gpu_hit_add_mmap",
				"Number of mmap hits on MR pages",
				(uint64_t*)&cache->rv_gpu_stats.hit_add_mmap),
		PSMI_STATS_DECL("rv_gpu_hit_add_mmap_%",
				"Cache mmap hit on MR pages rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_hit_rate_add_mmap, NULL),
		PSMI_STATS_DECLU64("rv_gpu_miss",
				"Overall Number of misses",
				(uint64_t*)&cache->rv_gpu_stats.miss),
		PSMI_STATS_DECL("rv_gpu_miss_%",
				"Overall Cache miss rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_miss_rate, NULL),
		PSMI_STATS_DECLU64("rv_gpu_miss_reg",
				"Number of misses for reg_mr",
				(uint64_t*)&cache->rv_gpu_stats.miss_reg),
		PSMI_STATS_DECL("rv_gpu_miss_reg_%",
				"Cache reg_mr miss rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_miss_rate_reg, NULL),
		PSMI_STATS_DECLU64("rv_gpu_miss_mmap",
				"Number of misses for mmap",
				(uint64_t*)&cache->rv_gpu_stats.miss_mmap),
		PSMI_STATS_DECL("rv_gpu_miss_mmap_%",
				"Cache mmap miss rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				mr_cache_rv_gpu_miss_rate_mmap, NULL),
		PSMI_STATS_DECLU64("rv_gpu_full",
				"Number of miss but can't add since no space",
				(uint64_t*)&cache->rv_gpu_stats.full),
		PSMI_STATS_DECLU64("rv_gpu_full_reg",
				"Number of miss but can't add MR since no space",
				(uint64_t*)&cache->rv_gpu_stats.full_reg),
		PSMI_STATS_DECLU64("rv_gpu_full_mmap",
				"Number of miss but can't add mmap since no space",
				(uint64_t*)&cache->rv_gpu_stats.full_mmap),
		PSMI_STATS_DECLU64("rv_gpu_failed_pin",
				"Number of miss but can't add since failed to pin pages",
				(uint64_t*)&cache->rv_gpu_stats.failed_pin),
		PSMI_STATS_DECLU64("rv_gpu_failed_reg",
				"Number of miss but can't add MR since failed reg_mr",
				(uint64_t*)&cache->rv_gpu_stats.failed_reg),
		PSMI_STATS_DECLU64("rv_gpu_failed_mmap",
				"Number of miss but can't add mmap since failed to mmap",
				(uint64_t*)&cache->rv_gpu_stats.failed_mmap),
		PSMI_STATS_DECLU64("rv_gpu_remove",
				"Number of entries removed due to application memory free",
				(uint64_t*)&cache->rv_gpu_stats.remove),
		PSMI_STATS_DECLU64("rv_gpu_remove_reg",
				"Number of MRs removed due to application memory free",
				(uint64_t*)&cache->rv_gpu_stats.remove_reg),
		PSMI_STATS_DECLU64("rv_gpu_remove_mmap",
				"Number of mmap removed due to application memory free",
				(uint64_t*)&cache->rv_gpu_stats.remove_mmap),
		PSMI_STATS_DECLU64("rv_gpu_remove_both",
				"Number of MR&mmap removed due to application memory free",
				(uint64_t*)&cache->rv_gpu_stats.remove_both),
		PSMI_STATS_DECLU64("rv_gpu_evict",
				"Number of entries removed from cache due to no space",
				(uint64_t*)&cache->rv_gpu_stats.evict),
		PSMI_STATS_DECLU64("rv_gpu_evict_reg",
				"Number of MRs removed from cache due to no space",
				(uint64_t*)&cache->rv_gpu_stats.evict_reg),
		PSMI_STATS_DECLU64("rv_gpu_evict_mmap",
				"Number of mmap removed from cache due to no space",
				(uint64_t*)&cache->rv_gpu_stats.evict_mmap),
		PSMI_STATS_DECLU64("rv_gpu_evict_both",
				"Number of MR&mmap removed from cache due to no space",
				(uint64_t*)&cache->rv_gpu_stats.evict_both),
		PSMI_STATS_DECLU64("rv_gpu_inval_mr",
				"Number of MR invalidated due to application memory free during IO",
				(uint64_t*)&cache->rv_gpu_stats.inval_mr),
		PSMI_STATS_DECLU64("rv_post_write",
				"Total CPU+GPU RDMA writes successfully posted",
				(uint64_t*)&cache->rv_gpu_stats.post_write),
		PSMI_STATS_DECLU64("rv_post_write_bytes",
				"Total CPU+GPU RDMA write bytes successfully posted",
				(uint64_t*)&cache->rv_gpu_stats.post_write_bytes),
		PSMI_STATS_DECLU64("rv_gpu_post_write",
				"Number of GPU RDMA writes successfully posted",
				(uint64_t*)&cache->rv_gpu_stats.gpu_post_write),
		PSMI_STATS_DECLU64("rv_gpu_post_write_bytes",
				"Number of GPU RDMA write bytes successfully posted",
				(uint64_t*)&cache->rv_gpu_stats.gpu_post_write_bytes),
	};
	if (cache->rv && PSM3_GPU_IS_ENABLED && cache->ep->rv_gpu_cache_size) {
		psm3_stats_register_type("MR_GPU_Cache_Statistics",
			"Kernel RV GPU MR and mmap cache for an endpoint in the process\n"
			"When Direct GPU transfers are enabled, an additional "
			"GPU MR and mmap cache is retained in the RV kernel "
			"module.\n"
			"The behaviors for limits, eviction, removal, hit and "
			"miss are the same as described above for the "
			"RV MR cache for CPU buffers.\n"
			"For GPU buffers, the kernel RV GPU cache entries can "
			"represent memory pinned for DMA/RDMA (MRs) or "
			"for direct GPU copy (mmap).  Some entries can be "
			"'both'.  Similarly an existing entry of one type "
			"can be promoted ('add_reg' or 'add_mmap') "
			"to 'both'.\n",
					PSMI_STATSTYPE_MR_CACHE,
					gpu_entries,
					PSMI_HOWMANY(gpu_entries),
					psm3_epid_fmt_internal(cache->ep->epid, 0),
					&cache->rv_gpu_stats, cache->ep->dev_name);
	}
#endif /* PSM_HAVE_RNDV_MOD */
#endif /* PSM_HAVE_GPU */
}

#endif /* PSM_HAVE_REG_MR */
