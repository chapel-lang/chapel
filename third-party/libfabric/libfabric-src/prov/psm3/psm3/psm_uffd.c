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
#include <sys/types.h>
#include "psm_user.h"   // sets PSM_HAVE_REG_MR
                        // and includes hal_verbs/verbs_ep.h and psm_verbs_mr.h

#ifdef UMR_CACHE
// This implements the MR invalidation notification mechanism for
// MR_CACHE_MODE_USER.
//
// Central to any MR caching mechanism is knowing when an MR must be
// invalidated.  While the MR has the physical pages pinned, the application
// and OS can free the virtual address and later reallocate that same
// virtual address with a different set of physical pages.  Since our
// MR cache only checks virtual addresses to identify a cache hit, this
// can result in a false MR cache hit.  The result being that an old
// stale MR is used and the data lands in the old physical pages.
// This situation is not a security issue since the old MR's pages are
// still pinned and dedicated to the original process.  However the new data
// is not placed where the application expects it and the application
// on IO completion will not see the expected data.
//
// For user space MR caching, a notification mechanism must be established
// so the MR cache knows when virtual addresses are freed by the OS.  It is
// important to note that simply calling free() does not always free the
// virtual address as the libc heap maintains a pool of pre-alloocated
// memory which it can quickly supply in the next malloc() or realloc()
// call.  So it can be valid for an MR to survive past the free() and
// actually be valid for use when the next malloc() gets the exact same
// virtual address.  In this scenario the kernel was never called and the
// physical pages remain the same (since the MR has them pinned so they will
// not be swapped out).  Also important to note that a variety of system calls
// can maniupate the memory for a process (brk, sbrk, mmap, mremap, etc) and
// these may be used by libc as well as alternate heap management libraries.
//
// The best existing mechanism for getting the necessary notifications is
// User Fault FD (UFFD).  This mechanism was originally designed to allow user
// space hypervisors to manage the memory of VMs, but it has the necessary
// mechanisms in place to accomidate MR cache notifications.
// However, it has some specific aspects of its design which must be carefully
// considered to obtain a truely reliable and safe MR caching mechanism.
//
// At the heart of User Fault FD is a file descriptor opened via the
// __NR_userfaultfd syscall.  This fd will receive notifications on memory
// events related to the process.  Only a single such file descriptor may
// monitor a given process.  Attempts to create a second file descriptor via the
// syscall will fail.  In the PSM3 design this file is poll()ed in a specific
// thread dedicated to this purpose.  It is opened via O_CLOEXEC since exec()
// could launch a process which is not aware of the User Fault FD and how to
// use it.
//
// Userfaultfd allows various notifications to be registered for, and the
// three of interest to PSM3 are:
// UFFD_FEATURE_EVENT_UNMAP - memory is unmapped from the process via
// 		munmap, brk, sbrk or mremap system calls.  This is the typical way
// 		that memory is freed.
// UFFD_FEATURE_EVENT_REMAP - memory is remapped via the mremap system call.
//		This may be used to move, grow or shrink a memory region.
// UFFD_FEATURE_EVENT_REMOVE - madvise is removed from a memory region
//
// Userfaultfd also requires selection of what virtual address ranges are
// to be monitored, via UFFDIO_REGISTER and UFFDIO_UNREGISTER.  However,
// User Fault fd does not do any reference counting for these ranges.
// So if a given range is REGISTERed twice, the first UNREGISTER will stop
// it from being monitored.  It is valid to REGISTER a given range of
// addresses more than once.  The kernel will examine the VMAs in the
// given range and mark all those not already marked for monitoring.
// It is also valid to UNREGISTER a range which is not being monitored,
// in which case the kernel will examine the VMAs in the given range and unmark
// any which are marked so they stop being monitored.  In both these situations
// no error or other indication is reported by the kernel.  However it is
// invalid to REGISTER or UNREGISTER virtual addresess which are not part of
// the process.  REGISTER and UNREGISTER work on page boundaries, while MRs
// are on byte boundaries.  So a given range may include or overlap more than
// 1 MR.  A given range could also include other heap allocations unrelated to
// IO buffers and without MRs (for example PSM3 malloc calls).
//
// In order to get notifications, PSM3 ensures the virtual memory used by
// every cached MR is REGISTERed and when MRs are removed, it ensures any
// virtual memory which is no longer part of any cached MR is UNREGISTERed.
//
// Given the nature of the MR cache, where MRs may be overlapping, this
// means that PSM3 must track all the portions of ranges which need to be
// monitored, and do it's own reference counting so that UNREGISTER calls
// only occur when no more cached MRs are referencing a given virtual address
// range.  Since MRs can overlap in many ways, PSM3 needs to track it's
// reference counts in potentially smaller regions where each region is
// entirely included in the affected MRs.  PSM3 does this via the
// struct psm3_uffd_region and a RB tree map of them which can be searched
// by virtual address range.  As MRs are added to the MR cache, these
// regions may need to be split so any given region is entirely in or entirely
// out of a given MR.  No region may be partially overlapping a given MR.
// While in theory, as MRs are freed, the regions could be rejoined, no
// such attempt is made as the additional tracking and effort to identify
// which two regions are now entirely in the same set of MRs would add
// overhead and reduce performance.
//
// When a monitored memory event occurs for a REGISTERed range of virtual
// addresses. A notification occurs. The notification appears on the UFFD
// file descriptor as read()able data.  As such the poll() will indicate a
// POLLIN for the file descriptor and a read() system call may obtain one or
// more struct uffd_msg.  PSM3 uses O_NONBLOCK so it can read from this
// until no more notifications are available and then return to poll()ing.
// This allows the PSM3 UFFD thread to monitor both for UFFD notifications and
// for the request to terminate the thread (delivered via a pipe() fd).
//
// The handling of concurrency for the UFFD is tricky, as UFFD was designed
// for hypervisors.  When a UFFD event occurs, the thread which generated the
// notification is paused by the kernel, much like a page fault would pause
// a typical user space process which the kernel was directly managing.
// The notification is generated and the thread remains paused until the
// uffd_msg is read from the UFFD, at which time the thread resumes.
// This creates a critical window where PSM3 must properly update the
// MR cache to ensure no future lookups get false MR cache hits.  Races
// are prevented by PSM3 getting a psm3_uffd_lock prior to reading the UFFD
// and holding it while performing the necessary notification processing.
//
// This aspect of UFFD introduces a complication which requires great care.
// If the UFFD thread itself causes a notification event, it will be paused,
// and since the UFFD thread is the only PSM3 thread reading the UFFD, this
// would be a deadlock. To avoid this situation, PSM3 must be careful not
// to perform any operations in the UFFD thread which could cause notifications.
// This means no free(), realloc(), madvise() or other library or OS calls
// which may perform these operations.  However, in practice, this
// situation can only occur if the memory being freed happens to share the
// same page as an MR which is REGISTERed for monitoring.
//
// Another factor is that UFFD was designed for page fault handling in a
// hypervisor.  PSM3 avoids this situation by only REGISTERing regions
// which it has already registered for MRs (ibv_reg_mr), and hence pinned the
// memory.  Similarly PSM3 must UNREGISTER memory prior to ibv_dereg_mr.
// This approach avoids PSM3 ever receiving a true page fault notification,
// which PSM3 is not prepared to handle.  However, analysis has revealed
// there is one page fault which PSM3 must handle, namely page faults
// for stack growth.  Such faults occur for memory ranges not yet
// REGISTERed.  Fortunately PSM3 can handle these by simply zeroing the page.
//
// Detailed analysis of the 4.18 kernel (RHEL 8.x series) has uncovered
// when and how the 3 notifications of interest behave.
// UFFD_FEATURE_EVENT_UNMAP - This is the most common notification of interest
//			It simply indicates that virtual addresses (kernel VMAs) have
//			been removed from the process.  Various system calls can trigger
//			this such as brk, sbrk, munmap, mremap, etc.
//			The notification occurs after the VMAs are gone, so there is no
//			need to UNREGISTER and any such attempts will get errors since the
//			virtual address is no longer valid.
//			In kernel, userfaultfd_munmap_prep gathers list of events,
//			userfaultfd_munmap_complete issues events after VMAs gone.
// UFFD_FEATURE_EVENT_REMAP - This occurs as a result of an mremap system call
//			mremap can yield 3 outcomes:
//				1. memory is moved (MREMAP_FIXED or MREMAP_MAYMOVE).  In this
//					case from.addr != to.addr and the virtual address for
//					the physical pages has changed.  While the physical pages
//					may remain, the change to virtual address implies any
//					previous MRs for those virtual addresses are now stale.
//				2. virtual memory is shrunk.  In this case from.addr == to.addr
//					and PSM3 can ignore this notification.  An EVENT_UMNAP
//					will also occur and PSM3 can use that to invalidate
//					the appropriate MRs in the cache.
//				3. virtual memory is grown.  In this case from.addr == to.addr
//					and PSM3 can ignore this notification.  All the existing
//					virtual addresses and their MRs remain valid.
//			The notification occurs after the VMAs are gone, so there is no
//			need to UNREGISTER and any such attempts will get errors since the
//			virtual address is no longer valid.
//			In kernel mremap_userfaultfd_prep just tracks uffd to notify,
//			mremap_userfaultfd_complete issues EVENT_REMAP after old VMA gone.
//			When memory is shrunk mremap also uses userfaultfd_munmap_prep to
//			gather list of events, userfaultfd_munmap_complete issues
//			EVENT_MUNMAP after VMAs gone.
// UFFD_FEATURE_EVENT_REMOVE - occurs as a result of an madvise system call.
// 			madvise can cause holes to be punched in mmapped files, but this
// 			may not be allowed when VM_LOCKED is set on the VMA.  An MR should
// 			have its pages pinned and it's vma VM_LOCKED, in which case this
// 			event should not happen.  From kernel code study, if this event
// 			does happen, the event is issued before the hole is punched,
// 			however the VMAs will remain until shortly after the event.
// 			It seems that invites a race where a page fault on these pages
// 			could occur if PSM3 ibv_dereg_mr before the vma is gone.
// 			This event has not been observed in empirical testing, so the
// 			implementation is conservative.  When this event occurs, PSM3 will
// 			explicitly UNREGISTER the regions but ignore any error reported.
// 			Such errors may indicate the race occurred such that
// 			the vma is already gone.
//			In kernel madvise_remove or madvise_dontneed_free calls
//			userfaultfd_remove to issue the EVENT_REMOVE.
//			dontneed_free definitely frees VMA.
// 			madvise_remove seems to free it via vfs_fallocate PUNCH_HOLE
//			If madvise fails, PSM3 may get this event even though VMA remains,
//			so we are conservative here and invalidate the MR and UNREGISTER.
//
// Locking Model:
// The psm3_uffd_lock is a small scope lock which protects
// the tracker's regions and callbacks.  This lock is held during
// event processing and when the MR cache is being searched for a hit,
// adding or removing regions to the uffd tracker.
//
// The MR cache itself is also protected by the progress lock for it's EP.
// The psm3_uffd_lock also ensures that the act of reading a UFFD event and
// invalidating the resulting MR cache entries is atomic, so a stale MR can't
// be used. The kernel UFFD pause of the affected process ensures it can't
// race with the receipt of the UFFD event.
//
// The UFFD tracker's creation, destruction and thread start/stop are
// protected by the psm3_creation_lock which is held whenever a new EP
// is being opened or closed and hence when the MR cache for an EP is
// being created or destroyed.
//
// To avoid free in psm3_uffd_deregister, a psm3_mpool or free list of regions
// is maintained.  This allows psm3_uffd_register to allocate from the pool
// and psm3_uffd_deregister can release back into the pool.
// USE_MPOOL decides if a mpool or free list is used.  The mpool has an extra
// 8 bytes of overhead per uffd_region_t and must have a limit specified when
// created, but it's allocations will be in larger chunks and hence may be more
// efficient. The free list approach may incur a similar (or larger) memory
// overhead as libc must track information for every heap allocation.
//
// In experimentation, the smaller malloc's from the free list approach may
// allow the heap to be better organized by libc and may actually result in
// fewer munmap calls for application buffer free/alloc.
//
// For USE_MPOOL, the mpool is sized very large, but with a large chunk size so
// the overhead of tracking chunks is low.  Due to region fragmentation it is
// hard to predict the maximum regions per MR, so a conservative number
// (4) is used when sizing the mpool.  If the mpool is exhausted, errors will
// be reported to the caller and the psm3_verbs_reg_mr will fail, causing the
// ips protocol to use a different strategy, retry later or register a smaller
// MR for a smaller chunk of the IO.
//
// In both approaches, the allocation of a new uffd_region (psm3_mpool_get or
// when free list empty) may call calloc or malloc, but review of libc/malloc.c
// indicates these will not call munmap nor mremap.  So growth of the pool or
// new uffd_regions during psm3_uffd_register should be safe.

// Given the design of psm3_rbtree.c, a given file can implement only a
// single rbtree type.  So this file must be separate from psm_verbs_mr.c
// however it's functions are only used by psm_verbs_mr.c.  This is a logical
// organization anyway as all the code in this file is specific to UFFD
// while psm_verbs_mr.c supports a variety of MR cache modes.

// These tests are a good way to generate uffd unmap events:
//      ./run_mpi_stress 2 -n 50 -L 40 -M 40 -drc -w 100  H H
//      IMB-MPI1 Bcast Barrier AllReduce Reduce
// In some cases, growing the MR cache (PSM3_MR_CACHE_SIZE_MB=4096) may cause
// more opportunities for MR invalidation.

#ifdef PSM_HAVE_REG_MR
#include "psm2_hal.h"
#ifdef PSM_FI
#include "ips_config.h"
#endif
#include <linux/userfaultfd.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <poll.h>
#include "psm_mq_internal.h"
#include "psm_uffd.h"

//#undef _HFI_MMDBG
//#define _HFI_MMDBG printf

// FAST_UFFD_REGISTER reduces kernel calls by doing 1 REGISTER across
// whole new MR as opposed to just REGISTERing the missing regions. This may
// be more efficient and the kernel should ignore redundant REGISTER portions
#define FAST_UFFD_REGISTER

// FATAL_UFFD_REGISTER determines if uffd REGISTER call errors should be fatal
// for the job or merely reported so PSM3 can retry the MR registration later.
// We don't expect REGISTER errors so it's simpler to just fail as its likely
// the job will get stuck anyway repeatedly retrying to register the MR.
// A REGISTER error probably implies a bug in this file or kernel userfaultfd.
#define FATAL_UFFD_REGISTER

// FAST_UFFD_UNREGISTER reduces kernel calls by consolidating adjacent
// uffd regions into a single UNREGISTER instead of 1 per region.
// This may be more efficient, but only benefits MR cache eviction for space.
// When enabled, in simple tests, as much as a 25% reduction in UNREGISTER
// calls was observed.
#define FAST_UFFD_UNREGISTER

// PSMI_MAX_QPS >= PSMI_MAX_RAILS, but both currently represent
// limit on ep's and are equal
#define MAX_UFFD_CALLBACKS PSMI_MAX_QPS

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

struct _cl_map_item;
struct psm3_uffd_region {
#ifndef USE_MPOOL
	SLIST_ENTRY(_cl_map_item) next;	// for tracker->free_regions list
#endif
	uint64_t addr;
	uint64_t endp1;		// end + 1 (eg. addr+length)
	uint32_t refcount;	// ok since 1 ref per MR and max_entries is 32b
#ifdef PSM_DEBUG
	uint32_t old_refcount; // for self verification of ref counting
#endif
};

// Since rbtree.h and rbtree.c are designed to be included, and declare
// some hardcoded type names (cl_map_item_t and cl_qmap_t), we must limit
// our data type declarations which use those types to this .c file

// this will be the payload of a cl_qmap_t
struct psm3_reg_tracker_map_pl {
	uint64_t	nelems;	// number of regions
};

// rbtree.h uses these 2 well known defines to create the payload for
// cl_map_item_t and cl_qmap_t structures
#define RBTREE_MI_PL  struct psm3_uffd_region
#define RBTREE_MAP_PL struct psm3_reg_tracker_map_pl
// RBTREE_ITERATOR is a time/space trade off, 16 more bytes per cl_map_item_t
// but first, successor and predecessor faster
#define RBTREE_ITERATOR
#ifdef PSM_DEBUG
#define RBTREE_VALIDATE
#define RBTREE_VALIDATE_ENABLE psm3_mr_cache_debug
#define RBTREE_VALIDATE_PRINT _HFI_INFO
#endif

#include "psm3_rbtree.h"

#if 0
// not needed, ips_cl_qmap_insert can use get_leftmost
static int reg_tracker_key_cmp(const struct psm3_uffd_region *a,
							const struct psm3_uffd_region *b)
{
	if (a->addr < b->addr)
		return -1;
	else if (a->addr > b->addr)
		return 1;
	return 0;
}
#endif

static unsigned long reg_tracker_get_leftmost(const struct psm3_uffd_region *reg)
{
	return (uintptr_t)reg->addr;
}

static unsigned long reg_tracker_get_rightmost(const struct psm3_uffd_region *reg)
{
	return (uintptr_t)reg->endp1;
}

// rbtree.c uses these defines to establish some of it's code and
// then provides all the rbtree manipulation functions.

// we don't need RBTREE_CMP nor the ips_cl_qmap_searchv
//#define RBTREE_CMP(a,b) reg_tracker_key_cmp((a), (b))
#define RBTREE_ASSERT                     psmi_assert
#define RBTREE_MAP_COUNT(PAYLOAD_PTR)     ((PAYLOAD_PTR)->nelems)
// this is used to enable ips_cl_qmap_search() in psm3_rbtree.c
#define RBTREE_GET_LEFTMOST(PAYLOAD_PTR) reg_tracker_get_leftmost(PAYLOAD_PTR)
#define RBTREE_GET_RIGHTMOST(PAYLOAD_PTR) reg_tracker_get_rightmost(PAYLOAD_PTR)
#ifdef PSM_DEBUG
#define RBTREE_EMIT_IPS_CL_QMAP_FIRST
//#define RBTREE_PRINT _HFI_INFO
//#define RBTREE_DUMP_PRINT _HFI_INFO
#define RBTREE_FMT "0x%"PRIx64
#endif

#include "psm3_rbtree.c"

// to simplify our own debug prints
// MR_RANGE_FMT defined in psm_verbs_mr.h
//#define MR_RANGE_FMT "0x%"PRIx64":0x%"PRIx64" (len 0x%"PRIx64")"
#define UFFD_RANGE_FMT MR_RANGE_FMT
#define UFFD_OUT_RANGE(addr, endp1) (uint64_t)(addr), (uint64_t)(endp1)-1, \
								(uint64_t)(endp1)-(uint64_t)(addr)
#define UFFD_OUT_REGION(reg) UFFD_OUT_RANGE((reg)->addr, (reg)->endp1)
#define UFFD_OUT_ITEM(p_item) UFFD_OUT_REGION(&((p_item)->payload))

// for handler thread and worker thread
struct uffd_thread {
	int pipefd[2];
	pthread_t thread;
};

psmi_lock_t psm3_uffd_lock;

static void __attribute__ ((constructor)) __psmi_uffd_lock_constructor(void)
{
        psmi_init_lock(&psm3_uffd_lock);
}

// FORCE_UNREGISTER does an UNREGISTER even for INVALIDATE events just
// to be safe.  However the resulting error is ignored. This can help
// if a wierd app frees only part of an IO buffer.
enum {
	PSM3_UFFD_NO_FORCE_UNREGISTER = 0,
	PSM3_UFFD_FORCE_UNREGISTER_INDIVIDUAL = 1,
	PSM3_UFFD_FORCE_UNREGISTER_CONSOLIDATE = 2,
};

// The creation and destruction of the tracker, and the start and stop
// of it's thread is protected by the psm3_creation_lock which is held
// during psm3_verbs_alloc_mr_cache and psm3_verbs_free_mr_cache as part
// of opening and closing an EP.
//
// Since changes to the callback_list only occur while holding both the
// psm3_creation_lock and the psm3_uffd_lock, read access to the callback_list
// or num_callback can safely be done while holding either lock.
struct psm3_uffd_tracker {
	int fd;	/* userfaultfd */
	uint32_t page_size;
	cl_qmap_t map;
	cl_map_item_t root;
	cl_map_item_t nil_item;
#ifdef USE_MPOOL
	mpool_t reg_pool;	// pool of cl_map_item_t (uffd_region)
#else
	SLIST_HEAD(, _cl_map_item) free_regions;
#endif
	psm2_mr_cache_t callback_list[MAX_UFFD_CALLBACKS];
	unsigned num_callback;	// total count of registered callbacks
	unsigned last_callback;	// highest used entry in callback_list
	struct uffd_thread uffd_handler;
	struct uffd_thread uffd_worker;
	TAILQ_HEAD(dereg_list, psm3_verbs_mr) dereg_list;
	uint8_t force_unregister;
#ifdef USE_MPOOL
	uint64_t limit_nelems;	// max regions allowed in reg_pool
#endif
	struct { // statistics
		uint64_t max_nelems;
		uint64_t failed_alloc_region;
		uint64_t register_region;
		uint64_t failed_register_region;
		uint64_t unregister_region;
		uint64_t failed_unregister_region;
		uint64_t ignored_failed_unregister_region;
		uint64_t ignored_unregister_region;
		uint64_t max_refcount;
		uint64_t entire_mr;
		uint64_t split;
		uint64_t fill_gap;
		uint64_t remove;
		uint64_t evt_remap;
		uint64_t evt_remove;
		uint64_t evt_unmap;
		uint64_t evt_pagefault;
	} stats;
};
typedef struct psm3_uffd_tracker *psm3_uffd_tracker_t;
static psm3_uffd_tracker_t tracker;

static void register_uffd_stats(void);

#ifdef PSM_DEBUG
// self validation of UFFD region map
static void uffd_save_refcount(void);
static int uffd_check_dec_refcount(uint64_t addr, uint64_t endp1,
									const char *caller);
static int uffd_validate_region(uint64_t addr, uint64_t endp1, int chk_inc,
								const char *caller);
static int uffd_validate_map(int dump, const char *caller);
#endif

// --------------------------------------------------------------------------
// psm3_uffd_region allocate and free and pool create/destroy

static cl_map_item_t *alloc_map_item(void)
{
#ifdef PSM_FI
	// simulate inability to allocate a map item (aka psm3_uffd_region)
	if_pf(PSM3_FAULTINJ_ENABLED()) {
		PSM3_FAULTINJ_STATIC_DECL(fi_uffd_reg, "uffd_reg",
			"out of uffd regions",
			1, IPS_FAULTINJ_UFFD_REG);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_uffd_reg, NULL, "")) {
			errno = ENOMEM;
			return NULL;
		}
	}
#endif
#ifdef USE_MPOOL
	return (cl_map_item_t *)psm3_mpool_get(tracker->reg_pool);
#else
	cl_map_item_t *p_item = SLIST_FIRST(&tracker->free_regions);
	if (p_item)
		SLIST_REMOVE_HEAD(&tracker->free_regions, payload.next);
	else
		p_item = (cl_map_item_t *)psmi_calloc(NULL, DESCRIPTORS,
						sizeof(*p_item), 1);
	return p_item;
#endif
}

// since this call is used during uffd event handling, it cannot
// call libc free nor any mechanism which may free memory via
// sbrk, brk, munmap, mremap, etc
static inline void free_map_item(cl_map_item_t *p_item)
{
#ifdef USE_MPOOL
	psm3_mpool_put(p_item);
#else
	SLIST_INSERT_HEAD(&tracker->free_regions, p_item, payload.next);
#endif
}

static psm2_error_t map_item_pool_init(uint32_t max_mrs)
{
#ifdef USE_MPOOL
	// max_entries for a pool must be power of 2
	// max_mrs could vary per EP, but must size pool on 1st call
	uint32_t max_entries = max_mrs * 4;
	tracker->limit_nelems = psm3_next_power2(max_entries * MAX_UFFD_CALLBACKS);
	tracker->reg_pool = psm3_mpool_create(sizeof(cl_map_item_t),
						min(4096, max_entries), tracker->limit_nelems,
                        0, DESCRIPTORS, NULL, NULL);
	if (!tracker->reg_pool) {
		_HFI_ERROR("failed to create uffd region pool.  limit %"PRIu64"\n",
						tracker->limit_nelems);
		return PSM2_NO_MEMORY;
	}
#else
	SLIST_INIT(&tracker->free_regions);
#endif
	return PSM2_OK;
}

static inline void map_item_pool_fini(void)
{
#ifdef USE_MPOOL
	psm3_mpool_destroy(tracker->reg_pool);
#else
	cl_map_item_t *p_item;
	while (NULL != (p_item = SLIST_FIRST(&tracker->free_regions))) {
		SLIST_REMOVE_HEAD(&tracker->free_regions, payload.next);
		psmi_free(p_item);
	}
#endif
}

// --------------------------------------------------------------------------
// REGISTER and UNREGISTER kernel calls

// REGISTER to get uffd events for a memory range
static int uffd_register(uint64_t addr, uint64_t endp1)
{
	int ret = 0;
	struct uffdio_register reg;

#ifdef PSM_FI
#ifndef FATAL_UFFD_REGISTER
	// simulate inability to allocate a map item (aka psm3_uffd_region)
	if_pf(PSM3_FAULTINJ_ENABLED()) {
		PSM3_FAULTINJ_STATIC_DECL(fi_uffd_reg, "uffd_register",
				"unable to REGISTER uffd region",
				1, IPS_FAULTINJ_UFFD_REGISTER);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_uffd_reg, NULL, "")) {
			tracker->stats.failed_register_region++;
			return ENOMEM;
		}
	}
#endif
#endif
	reg.range.start = addr;
	reg.range.len = endp1 - addr;
	reg.mode = UFFDIO_REGISTER_MODE_MISSING;
	if_pf (-1 == ioctl(tracker->fd, UFFDIO_REGISTER, &reg)) {
		ret = errno;
		tracker->stats.failed_register_region++;
#ifdef FATAL_UFFD_REGISTER
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"Unable to REGISTER address range "UFFD_RANGE_FMT": %s\n",
					UFFD_OUT_RANGE(addr, endp1), strerror(ret));
#endif
	} else {
		tracker->stats.register_region++;
		_HFI_MMDBG("REGISTER "UFFD_RANGE_FMT"\n",
				UFFD_OUT_RANGE(reg.range.start, reg.range.start+reg.range.len));
	}
	return ret;
}

// UNREGISTER uffd events for a memory range
static int uffd_unregister(uint64_t addr, uint64_t endp1, int ignore_err)
{
	int ret = 0;
	struct uffdio_range range;

	_HFI_MMDBG("UFFD unregister "UFFD_RANGE_FMT" ignore %d\n",
				UFFD_OUT_RANGE(addr, endp1), ignore_err);
	range.start = addr;
	range.len = endp1 - addr;
	if_pf (-1 == ioctl(tracker->fd, UFFDIO_UNREGISTER, &range)) {
		if (ignore_err) {
			tracker->stats.ignored_failed_unregister_region++;
			_HFI_MMDBG("UNREGISTER failed ignored: "UFFD_RANGE_FMT": %s\n",
						UFFD_OUT_RANGE(addr, endp1), strerror(ret));
		} else {
			ret = errno;
			tracker->stats.failed_unregister_region++;
			_HFI_ERROR("UNREGISTER failed: "UFFD_RANGE_FMT": %s\n",
						UFFD_OUT_RANGE(addr, endp1), strerror(ret));
		}
	} else {
		tracker->stats.unregister_region++;
		if (ignore_err)
			tracker->stats.ignored_unregister_region++;
		_HFI_MMDBG("UNREGISTER "UFFD_RANGE_FMT"\n",
					UFFD_OUT_RANGE(range.start, range.start+range.len));
	}
	return ret;
}

#ifdef PSM_DEBUG
// --------------------------------------------------------------------------
// range overlap detection and checking

// do the 2 address ranges overlap in any way
static  inline
int overlaps(uint64_t a_addr, uint64_t a_endp1, uint64_t b_addr,
				uint64_t b_endp1)
{
	return(a_addr < b_endp1 && a_endp1 > b_addr);
}

static int check_overlaps(int expect, uint64_t a_addr, uint64_t a_endp1,
			uint64_t b_addr, uint64_t b_endp1)
{
	if (expect != overlaps(a_addr, a_endp1, b_addr, b_endp1)) {
		_HFI_ERROR("UFFD Overlap issue: expect %s "UFFD_RANGE_FMT" "UFFD_RANGE_FMT"\n",
					expect?"Overlap":"No Overlap",
					UFFD_OUT_RANGE(a_addr, a_endp1),
					UFFD_OUT_RANGE(b_addr, b_endp1));
		return 0;
	}
	return 1;
}

#define uffd_assert_overlaps(a_addr, a_endp1, b_addr, b_endp1) \
	psmi_assert(check_overlaps(1, a_addr, a_endp1, b_addr, b_endp1))
#define uffd_assert_no_overlap(a_addr, a_endp1, b_addr, b_endp1) \
	psmi_assert(check_overlaps(0, a_addr, a_endp1, b_addr, b_endp1))

#else /* PSM_DEBUG */

#define uffd_assert_overlaps(a_addr, a_endp1, b_addr, b_endp1)
#define uffd_assert_no_overlap(a_addr, a_endp1, b_addr, b_endp1)
#endif /* PSM_DEBUG */

#ifdef PSM_DEBUG
static inline
int overlaps_item(cl_map_item_t *a, cl_map_item_t *b)
{
	return overlaps(a->payload.addr, a->payload.endp1,
					b->payload.addr, b->payload.endp1);
}

static int check_overlaps_item(int expect, cl_map_item_t *a, cl_map_item_t *b)
{
	if (expect != overlaps_item(a, b)) {
		_HFI_ERROR("UFFD Overlap issue: expect %s "UFFD_RANGE_FMT" "UFFD_RANGE_FMT"\n",
					expect?"Overlap":"No Overlap",
					UFFD_OUT_RANGE(a->payload.addr, a->payload.endp1),
					UFFD_OUT_RANGE(b->payload.addr, b->payload.endp1));
		return 0;
	}
	return 1;
}
#define uffd_assert_overlaps_item(a, b) \
		psmi_assert(check_overlaps_item(1, a, b))
#define uffd_assert_no_overlap_item(a, b) \
		psmi_assert(check_overlaps_item(0, a, b))

#else /* PSM_DEBUG */

#define uffd_assert_overlaps_item(a, b)
#define uffd_assert_no_overlap_item(a, b)
#endif /* PSM_DEBUG */

// --------------------------------------------------------------------------
// track REGISTER and UNREGISTER status via reference counted psm3_uffd_region's
// and issue the required kernel calls

static void inc_refcount(psm3_uffd_region_t reg)
{
	if (++(reg->refcount) > tracker->stats.max_refcount)
		tracker->stats.max_refcount = reg->refcount;
}

// this is only used to correct overincrement of refcount
static inline void dec_refcount(psm3_uffd_region_t reg)
{
	reg->refcount--;
	psmi_assert(reg->refcount);
}

// create a new region
// new region is reference counted
//
// Errors during allocation of a psm3_uffd_region result in a NULL return
// and caller must cleanup and retry later. Errors during REGISTER can be fatal.
//
// Can't ignore the error and proceed as we would have issues as
// there may end up being REGISTERed memory without an MR or an MR without
// registered memory.  Which can risk getting page faults PSM3 can't handle
// or not getting required events to invalidate MRs.
static
cl_map_item_t *
register_region(uint64_t start, uint64_t endp1)
{
	cl_map_item_t *p_item;
	psm3_uffd_region_t reg;

	psmi_assert(start < endp1);

	p_item = alloc_map_item();
	if (! p_item) {
		tracker->stats.failed_alloc_region++;
		_HFI_MMDBG("Region pool allocate failure\n");
		return NULL;
	}
	reg = &p_item->payload;
	reg->addr = start;
	reg->endp1 = endp1;
	reg->refcount = 1;
#ifdef PSM_DEBUG
	reg->old_refcount = 0;
#endif

#ifndef FAST_UFFD_REGISTER
	if_pf (uffd_register(start, endp1)) {
		free_map_item(p_item);
		return NULL;
	}
#endif
	if_pf (!tracker->stats.max_refcount)
		tracker->stats.max_refcount = 1;
	ips_cl_qmap_insert_item(&tracker->map, p_item);
	tracker->stats.max_nelems = max(tracker->stats.max_nelems,
										tracker->map.payload.nelems);
	return p_item;
}

// Handle uffd_unregister for an event (INVALIDATE or REMOVE)
// Will explicitly unregister portion outside the triggering event and report
// any errors.
// force indicates if will unregister portions inside the triggering event.
// Errors for any portion inside triggering event are ignored (but counted).
// Not valid to be called for PSM3_UFFD_EVENT_NONE.
static int uffd_unregister_for_event(uint64_t addr, uint64_t endp1, int force,
									 uint64_t evt_addr, uint64_t evt_endp1)
{
	int ret = 0;
	if (addr < evt_addr) {
		// before the event address range
		_HFI_MMDBG("UNREGISTER before event: "UFFD_RANGE_FMT"\n",
					UFFD_OUT_RANGE(addr, evt_addr));
		if (uffd_unregister(addr, evt_addr, 0))
			ret = 1;
		addr = evt_addr;
	}
	if (endp1 > evt_endp1) {
		// after the event address range
		_HFI_MMDBG("UNREGISTER after event: "UFFD_RANGE_FMT"\n",
					UFFD_OUT_RANGE(evt_endp1, endp1));
		if (uffd_unregister(evt_endp1, endp1, 0))
			ret = 1;
		endp1 = evt_endp1;
	}
	if (force) {
		_HFI_MMDBG("force UNREGISTER "UFFD_RANGE_FMT" event "UFFD_RANGE_FMT"\n",
					UFFD_OUT_RANGE(addr, endp1),
					UFFD_OUT_RANGE(evt_addr, evt_endp1));
		ret |= uffd_unregister(addr, endp1, 1);
	}
	return ret;
}

// unregister the specified address range for the given reason.
// For NONE, an explicit UNREGISTER always occurs.
// For INVALIDATE and REMOVE any portion of the address range outside
// the event is explicitly UNREGISTERed.
// For INVALIDATE, any portion inside the event address range is only
// UNREGISTERed if PSM3_MR_CACHE_FORCE_UNREGISTER is enabled.
// For REMOVE, any portion inside the event address range is always
// UNREGISTERed but errors are ignored (but counted).
static int uffd_unregister_range(uint64_t addr, uint64_t endp1, uint8_t reason,
									 uint64_t evt_addr, uint64_t evt_endp1)
{
	switch (reason) {
	case PSM3_UFFD_EVENT_NONE: // must UNREGISTER
		return uffd_unregister(addr, endp1, 0);
		break;
	case PSM3_UFFD_EVENT_INVALIDATE: // kernel implicitly UNREGISTERed
		return uffd_unregister_for_event(addr, endp1,
									tracker->force_unregister,
									evt_addr, evt_endp1);
		break;
	case PSM3_UFFD_EVENT_REMOVE: // conservative, UNREGISTER but ignore errors
		return uffd_unregister_for_event(addr, endp1, 1,
									evt_addr, evt_endp1);
		break;
	default:
		psmi_assert_always(0);
		return 0;
		break;
	}
}

// UNREGISTER a region in an MR and return the region to our pool.
// failures are reported, but as much as possible is still done to
// cleanup. Caller is not expected to attempt to recover from failure.
//
// reason indicates why we are unregistering the MR and regions in it.
// For INVALIDATE and REMOVE reason, we must explicitly remove any portion
// of the region which is outside the event range as the kernel will have
// only unregistered the address range corresponding to the event.
//
// evt_addr and evt_endp1 only used when reason is not NONE
// return 0 on success, !=0 on error
static int
unregister_region(cl_map_item_t *p_item, uint8_t reason, uint64_t evt_addr,
					uint64_t evt_endp1)
{
	int ret = uffd_unregister_range(p_item->payload.addr, p_item->payload.endp1,
								reason, evt_addr, evt_endp1);
	ips_cl_qmap_remove_item(&tracker->map, p_item);
	free_map_item(p_item);
	return ret;
}

// evt_addr and evt_endp1 only used when reason is not NONE
// return 0 on success, !=0 on error
static
int release_region(cl_map_item_t *p_item, uint8_t reason, uint64_t evt_addr,
					uint64_t evt_endp1)
{
	if (! --(p_item->payload.refcount))
		return unregister_region(p_item, reason, evt_addr, evt_endp1);
	return 0;
}

// create a new region to fill a gap between regions
// new region is reference counted
// returns NULL if unable to allocate a psm3_uffd_region
static
cl_map_item_t *
fill_gap(uint64_t start, uint64_t endp1)
{
	cl_map_item_t *p_item;
	tracker->stats.fill_gap++;
	_HFI_MMDBG("uffd fill_gap "UFFD_RANGE_FMT"\n", UFFD_OUT_RANGE(start, endp1));
	p_item = register_region(start, endp1);
	psmi_assert(uffd_validate_map(0, "After fill_gap"));
	return p_item;
}

// split our tracking of a region into two
// no need to change REGISTER as we already have whole region registered
// returns new item which is right half of original p
// caller must decide which half to refcount
// returns NULL if unable to allocate a psm3_uffd_region without changing
// existing p
static
cl_map_item_t *
split(cl_map_item_t *p, uint64_t addr)
{
	cl_map_item_t *p_item;
	_HFI_MMDBG("uffd split "UFFD_RANGE_FMT" at 0x%"PRIx64"\n",
				UFFD_OUT_ITEM(p), addr);
	p_item = alloc_map_item();
	if (! p_item) {
		tracker->stats.failed_alloc_region++;
		_HFI_MMDBG("Region pool allocate failure\n");
		return NULL;
	}

	// p_item becomes right of old p, including addr
	p_item->payload.addr = addr;
	p_item->payload.endp1 = p->payload.endp1;
	p_item->payload.refcount = p->payload.refcount;
#ifdef PSM_DEBUG
	p_item->payload.old_refcount = p->payload.old_refcount;
#endif
	// p reduced to portion left of addr
	p->payload.endp1 = addr;
	ips_cl_qmap_insert_item(&tracker->map, p_item);
	tracker->stats.max_nelems = max(tracker->stats.max_nelems,
										tracker->map.payload.nelems);
	tracker->stats.split++;
	psmi_assert(uffd_validate_map(0, "After split"));
	return p_item;
}

// adjust REGISTER as needed to track a new MR from addr to addr + length-1
// returns a region within the registration which will help speed up dereg
// caller must hold psm3_uffd_lock
psm3_uffd_region_t psm3_uffd_register(uint64_t addr, uint64_t length)
{
	psm3_uffd_region_t reg;
	cl_map_item_t *p_tmp;
	// in case we have to cleanup after a failure, keep track of
	// the 1st and last region we touched
	cl_map_item_t *p_first = NULL, *p_last = NULL;
#ifdef FAST_UFFD_REGISTER
	int need_register = 0;
#endif

	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	psmi_assert(tracker);

	uint64_t endp1 = addr + length;
	addr = PSMI_ALIGNDOWN(addr, tracker->page_size);
	endp1 = PSMI_ALIGNUP(endp1, tracker->page_size);
	//length = endp1 - addr; // unused

	_HFI_MMDBG(UFFD_RANGE_FMT"\n", UFFD_OUT_RANGE(addr, endp1));

	psmi_assert(uffd_validate_map(0, "Before uffd_register"));
#ifdef PSM_DEBUG
	uffd_save_refcount();
#endif

	cl_map_item_t *p_item = ips_cl_qmap_search(&tracker->map, addr, endp1);
	if (p_item == tracker->map.nil_item) {
		// nothing overlaps, must cover entire MR with a new REGISTER
		p_item = register_region(addr, endp1);
		if (p_item) {
			tracker->stats.entire_mr++;
			reg = &p_item->payload;
#ifdef FAST_UFFD_REGISTER
			if (uffd_register(addr, endp1)) {
				p_first = p_last = p_item;
				goto fail;
			}
#endif
			psmi_assert(uffd_validate_region(addr, endp1, 1, "After uffd_register entire"));
			psmi_assert(uffd_validate_map(0, "After uffd_register entire"));
		} else {
			reg = NULL;
			psmi_assert(uffd_validate_map(0, "After failed uffd_register entire"));
		}
		return reg;
	}

	// we found a region which covers at least part of our MR
	uffd_assert_overlaps(p_item->payload.addr, p_item->payload.endp1,
							addr, endp1);
#ifdef PSM_DEBUG
	// search() should return lowest region which matches interval
	p_tmp = ips_cl_qmap_predecessor(&tracker->map, p_item);
	if (p_tmp != tracker->map.nil_item) {
		uffd_assert_no_overlap(p_tmp->payload.addr, p_tmp->payload.endp1,
							addr, endp1);
	}
#endif

	if (p_item->payload.addr > addr) {
		// there is space to the left of p_item we need to handle
		if (! (p_first = fill_gap(addr, p_item->payload.addr)))
			goto fail;
		p_last = p_first;
#ifdef FAST_UFFD_REGISTER
		need_register = 1;
#endif
	} else if (p_item->payload.addr < addr) {
		// p_item goes father left than we need, split it
		if (! (p_item = split(p_item, addr)))
			goto fail;
		// p_item is now right half of split
	} else {
		// p_item exactly matches our left
		psmi_assert(p_item->payload.addr == addr);
	}

	// we will be using at least the left portion of p_item
	reg = &p_item->payload;
	inc_refcount(reg);
	if (! p_first)
		p_first = p_item;
	psmi_assert(p_first->payload.addr == addr);
	p_last = p_item;

	// check to our right
	if (reg->endp1 > endp1) {
		// region goes father right than we need, split it
		if (! (p_tmp = split(p_item, endp1)))
			goto fail;
		// already refcounted p_item above, drop count on unused right part
		dec_refcount(&p_tmp->payload);
		psmi_assert(p_last == p_item);
	} else if (reg->endp1 < endp1) {
		// there is space to the right of our p_item we need to handle
		uint64_t prev_endp1 = reg->endp1;

		cl_map_item_t *p_this = p_item;
		// cover everything to the right of the found item, inc refcount as go
		do {
			p_tmp = ips_cl_qmap_successor(&tracker->map, p_this);
			psmi_assert(p_tmp == tracker->map.nil_item
						|| check_overlaps_item(0, p_tmp, p_this));
			if (p_tmp == tracker->map.nil_item
				|| p_tmp->payload.addr >= endp1) {
				// no next or next is entirely after our right
				// complete the right of our MR
				if (! (p_tmp = fill_gap(prev_endp1, endp1)))
					goto fail;
				p_last = p_tmp;
#ifdef FAST_UFFD_REGISTER
				need_register = 1;
#endif
				break;
			}
			if (p_tmp->payload.addr != prev_endp1) {
				cl_map_item_t *p;
				// fill gap between prev and p_tmp
				if (! (p = fill_gap(prev_endp1, p_tmp->payload.addr)))
					goto fail;
				p_last = p;
#ifdef FAST_UFFD_REGISTER
				need_register = 1;
#endif
			}
			if (p_tmp->payload.endp1 < endp1) {
				// p_tmp overlaps MR but is not enough to cover our right
				inc_refcount(&p_tmp->payload);
				prev_endp1 = p_tmp->payload.endp1;
				p_last = p_this = p_tmp;
				continue;
			} else if (p_tmp->payload.endp1 > endp1) {
				// p_tmp covers more than our right
				psmi_assert(p_tmp->payload.addr < endp1);
				if (! split(p_tmp, endp1))
					goto fail;
				inc_refcount(&p_tmp->payload); // we use left half
				p_last = p_tmp;
				break;
			} else {
				// p_tmp covers exactly what we need
				inc_refcount(&p_tmp->payload);
				p_last = p_tmp;
				break;
			}
		} while (1);
		psmi_assert(p_last->payload.endp1 == endp1);
	} else {
		// region exactly matches our right
		psmi_assert(reg->endp1 == endp1);
	}
#ifdef FAST_UFFD_REGISTER
	if (need_register && uffd_register(addr, endp1))
		goto fail;
#endif
	psmi_assert(uffd_validate_map(0, "After uffd_register parts"));
	psmi_assert(uffd_validate_region(addr, endp1, 1, "After uffd_register parts"));
	// return any region which is part of our MR
	return reg;

fail:
	// release all the regions we touched before the error
	// p_first to p_last inclusive
	if (p_first) {
		psmi_assert(p_last);
		for (p_tmp = p_first; p_tmp != p_last; p_tmp = p_item) {
			psmi_assert(p_tmp != tracker->map.nil_item);
			p_item = ips_cl_qmap_successor(&tracker->map, p_tmp);
			release_region(p_tmp, PSM3_UFFD_EVENT_NONE, 0, UINT64_MAX);
		}
		release_region(p_last, PSM3_UFFD_EVENT_NONE, 0, UINT64_MAX);
	}
	psmi_assert(uffd_validate_map(0, "After failed uffd_register parts"));
	return NULL;
}

#ifdef FAST_UFFD_UNREGISTER
// release all regions between addr and last_endp1 where p_item is a
// region overlapping that range.
// For regions which now have a refcount of 0, they are UNREGISTERed
// but the UNREGISTER calls are consolidated among adjacent regions.
// This is only valid for PSM3_UFFD_EVENT_NONE or INVALIDATE.
static int psm3_uffd_deregister_range(cl_map_item_t *p_item, uint64_t addr,
										uint64_t last_endp1, uint8_t reason,
										uint64_t evt_addr, uint64_t evt_endp1)
{
	int err = 0;
	uint64_t start = 0;
	uint64_t endp1 = 0;
	cl_map_item_t *p_tmp;
	psmi_assert(reason == PSM3_UFFD_EVENT_NONE
				|| ( evt_addr && evt_addr < evt_endp1));

	// find first item in addr/last_endp1 range
	for (p_tmp = ips_cl_qmap_predecessor(&tracker->map, p_item);
			p_tmp != tracker->map.nil_item && p_tmp->payload.addr >= addr;
			p_tmp = ips_cl_qmap_predecessor(&tracker->map, p_tmp)) {
		p_item = p_tmp;
	}
	psmi_assert(p_tmp == tracker->map.nil_item
			|| check_overlaps(0, p_tmp->payload.addr, p_tmp->payload.endp1,
								addr, last_endp1));
	psmi_assert(p_item != tracker->map.nil_item);
	psmi_assert(check_overlaps(1, p_item->payload.addr, p_item->payload.endp1,
								addr, last_endp1));

	// release and UNREGISTER starting at p_item to last_endp1
	for (;
			p_item != tracker->map.nil_item
						&& p_item->payload.addr < last_endp1;
			p_item = p_tmp) {
		p_tmp = ips_cl_qmap_successor(&tracker->map, p_item);
		if (! --(p_item->payload.refcount)) {
			if (! start) {
				start = p_item->payload.addr;
			} else {
				_HFI_MMDBG("consolidate UNREGISTER "UFFD_RANGE_FMT"\n",
							UFFD_OUT_RANGE(start, p_item->payload.endp1));
			}
			endp1 = p_item->payload.endp1;
			ips_cl_qmap_remove_item(&tracker->map, p_item);
			free_map_item(p_item);
		} else {
			// unregister those we found so far
			if (start) {
				if (uffd_unregister_range(start, endp1, reason, evt_addr,
										  evt_endp1))
					err++;
				start=0;
			}
		}
	}
	// unregister any remaining portion
	if (start) {
		if (uffd_unregister_range(start, endp1, reason, evt_addr, evt_endp1))
			err++;
	}
	return err;
}
#endif /* FAST_UFFD_UNREGISTER */

// reg is any region from when we registered, helps avoid a search.
// caller must hold psm3_uffd_lock
//
// Errors during UNREGISTER are fatal, because they can result in a
// partially UNREGISTERed MR.  We chose to make this fatal as it probably
// implies a bug in this file or kernel userfaultfd.  If we tried to proceed
// we would have to leak the MR and it's pages because if we deregistered the
// MR we would risk getting page faults PSM3 can't handle.  Such leaks could
// accumulate and cause job performance issues, out of memory or hangs.
//
// When such errors occur, we still try to UNREGISTER all parts of the address
// range so that the full list of problematic UNREGISTER can be reported prior
// to the final fatal error from this routine.  This info may help debug.
//
// For UMR_FAST_FLUSH this function is not used during final EP close.
// So effectvely, UNREGISTEER errors which might have occurred during final EP
// close are ignored,
void psm3_uffd_deregister(uint64_t addr, uint64_t length,
						psm3_uffd_region_t reg, uint8_t reason,
						uint64_t evt_addr, uint64_t evt_length)
{
	unsigned err = 0;
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	psmi_assert(tracker);

	uint64_t endp1 = addr + length;
	addr = PSMI_ALIGNDOWN(addr, tracker->page_size);
	endp1 = PSMI_ALIGNUP(endp1, tracker->page_size);
	//length = endp1 - addr; // unused
	uint64_t evt_endp1 = evt_addr + evt_length;

	psmi_assert(reason == PSM3_UFFD_EVENT_NONE
				|| ( evt_addr && evt_addr < evt_endp1));

	_HFI_MMDBG(UFFD_RANGE_FMT"\n", UFFD_OUT_RANGE(addr, endp1));

	psmi_assert(uffd_validate_map(0, "Before uffd_deregister"));
#ifdef PSM_DEBUG
	uffd_save_refcount();
#endif
	psmi_assert(uffd_validate_region(addr, endp1, 0, "Before uffd_deregister"));

	uffd_assert_overlaps(reg->addr, reg->endp1, addr, endp1);
	cl_map_item_t *p_item = container_of(reg, cl_map_item_t, payload);
#ifdef FAST_UFFD_UNREGISTER
	// Only optimize the evict (EVENT_NONE) scenario.  For REMOVE we are
	// conservative and call UNREGISTER per region as some may race and fail.
	// For INVALIDATE, the kernel has already done the UNREGISTER prior to
	// issuing the event.
	if (reason == PSM3_UFFD_EVENT_NONE
		|| (tracker->force_unregister == PSM3_UFFD_FORCE_UNREGISTER_CONSOLIDATE
			&& reason == PSM3_UFFD_EVENT_INVALIDATE)) {
		err = psm3_uffd_deregister_range(p_item, addr, endp1, reason,
											evt_addr, evt_endp1);
		goto done;
	}
#endif
	cl_map_item_t *p_tmp;
	for (p_tmp = ips_cl_qmap_predecessor(&tracker->map, p_item);
		p_tmp != tracker->map.nil_item && p_tmp->payload.addr >= addr;) {
		cl_map_item_t *p_tmp2 = ips_cl_qmap_predecessor(&tracker->map, p_tmp);
		if (release_region(p_tmp, reason, evt_addr, evt_endp1))
			err++;
		p_tmp = p_tmp2;
	}
	psmi_assert(p_tmp == tracker->map.nil_item
			|| check_overlaps(0, p_tmp->payload.addr, p_tmp->payload.endp1,
								addr, endp1));
	for (p_tmp = ips_cl_qmap_successor(&tracker->map, p_item);
			p_tmp != tracker->map.nil_item && p_tmp->payload.endp1 <= endp1;) {
		cl_map_item_t *p_tmp2 = ips_cl_qmap_successor(&tracker->map, p_tmp);
		uffd_assert_no_overlap_item(p_tmp, p_item);
		if (release_region(p_tmp, reason, evt_addr, evt_endp1))
			err++;
		p_tmp = p_tmp2;
	}
	psmi_assert(p_tmp == tracker->map.nil_item
			|| check_overlaps(0, p_tmp->payload.addr, p_tmp->payload.endp1,
								addr, endp1));
	if (release_region(p_item, reason, evt_addr, evt_endp1))
		err++;
#ifdef FAST_UFFD_UNREGISTER
done:
#endif
	psmi_assert(uffd_validate_map(0, "After uffd_deregister"));
	psmi_assert(uffd_check_dec_refcount(addr, endp1, "After uffd_deregister"));
	if (err) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to UNREGISTER user fault fd for %u portions of address range "
				UFFD_RANGE_FMT" reason %d event "UFFD_RANGE_FMT"\n",
				err, UFFD_OUT_RANGE(addr, endp1), reason,
				UFFD_OUT_RANGE(evt_addr, evt_endp1));
	}
}

// --------------------------------------------------------------------------
// Monitor for events and process them

// zero a page (for fault handler)
// returns 0 on success or error code
// kernel places ioctl return values in errno and zp.zeropage
//	EAGAIN - page mmap changing, try again
//	EAGAIN - len returned by mfill_zeropage != range.len
//	EFAULT - invalid &zp
//	EINVAL - invalid bits in zp.mode
//	EINVAL - addr to addr+len-1 is invalid virt addr or not PAGE_MASK aligned
//	ESRCH - mmget_not_zero returned 0
static int psm3_uffd_zeropage(uint64_t addr)
{
	int ret = 0;
	struct uffdio_zeropage zp;

	zp.range.start = PSMI_ALIGNDOWN(addr, tracker->page_size);
	zp.range.len = tracker->page_size;
	zp.mode = 0;
	zp.zeropage = 0;
	if (-1 == ioctl(tracker->fd, UFFDIO_ZEROPAGE, &zp)) {
		if (zp.zeropage)
			ret = -zp.zeropage;
		else
			ret = errno;
	}
	_HFI_MMDBG("addr=0x%"PRIx64" zp range "UFFD_RANGE_FMT
				" errno=%d zeropage=%"PRId64"\n", addr,
				UFFD_OUT_RANGE(zp.range.start, zp.range.start+zp.range.len),
				ret, (int64_t)zp.zeropage);
	return ret;
}

// For actual MR cache entries we should never get a page fault
// because we pin the memory via ibv_reg_mr prior to REGISTER and
// UNREGISTER prior to unpinning via ibv_dereg_mr.
// However, when the stack needs to grow, since we are using uffd, we
// are implicitly registered for the pagefault event and must handle it.
// Since it is stack growth, we just need to ensure a fresh zeroed page
// is made available.
// The flags we may get:
//		WRITE - write to a missing page
//		0 - read from a missing page
// for a stack growth we will typically get a WRITE fault because the
// stack's contents are undefined until written so a good app should
// write a stack variable before reading.  However, if the app reads a
// page before writing we still zero it but output a message.
static void psm3_uffd_handle_pagefault(uint64_t addr, uint64_t flags)
{
	int ret;

	if (flags & UFFD_PAGEFAULT_FLAG_WP) {
		// we didn't request the UFFD_FEATURE_PAGEFAULT_FLAG_WP
		_HFI_ERROR("uffd unexpected write protect pagefault: "
					"addr: 0x%"PRIx64" flags 0x%"PRIx64"\n", addr, flags);
		return;
	}
	if (! (flags & UFFD_PAGEFAULT_FLAG_WRITE))
		_HFI_ERROR("uffd unexpected read pagefault: "
					"addr: 0x%"PRIx64" flags 0x%"PRIx64"\n", addr, flags);

	do {
		ret = psm3_uffd_zeropage(addr);
		if (ret == EAGAIN)
			usleep(1); // give OS some time
	} while (EAGAIN == ret);
	if (ret)
		_HFI_ERROR("uffd error zeroing page: 0x%"PRIx64": %s\n",
										addr, strerror(ret));
}

static void psm3_uffd_do_callback(uint64_t addr, uint64_t len, uint8_t reason)
{
	int i;

	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	psmi_assert(tracker);
	psmi_assert(addr);
	psmi_assert(len);
	if (! tracker->num_callback)	// should not happen
		return;

	_HFI_MMDBG("event: %d "UFFD_RANGE_FMT"\n",
				reason, UFFD_OUT_RANGE(addr, addr+len));
	for (i=0; i <= tracker->last_callback; i++) {
		if (tracker->callback_list[i]) {
			psm3_verbs_umrc_uffd_callback(tracker->callback_list[i], addr, len,
											reason);
		}
	}
}

// read events from uffd and process them
static void psm3_uffd_event(void)
{
	struct uffd_msg msg;
	int ret;

	do {
		PSMI_LOCK(psm3_uffd_lock);
		// once all uffd events read, read() returns -1 w/ errno == EAGAIN
		// unexpected to get > 0 bytes and less than sizeof(msg)
		ret = read(tracker->fd, &msg, sizeof(msg));
		if (ret == sizeof(msg)) {
			switch (msg.event) {
			case UFFD_EVENT_REMOVE:
				tracker->stats.evt_remove++;
				// remove.end is remove.start + length (eg. really end+1)
				psm3_uffd_do_callback(msg.arg.remove.start,
								msg.arg.remove.end - msg.arg.remove.start,
								PSM3_UFFD_EVENT_REMOVE);
				break;
			case UFFD_EVENT_UNMAP:
				tracker->stats.evt_unmap++;
				// remove.end is remove.start + length (eg. really end+1)
				psm3_uffd_do_callback(msg.arg.remove.start,
								msg.arg.remove.end - msg.arg.remove.start,
								PSM3_UFFD_EVENT_INVALIDATE);
				break;
			case UFFD_EVENT_REMAP:
				tracker->stats.evt_remap++;
				if (msg.arg.remap.from != msg.arg.remap.to)
					psm3_uffd_do_callback(msg.arg.remap.from,
								msg.arg.remap.len,
								PSM3_UFFD_EVENT_INVALIDATE);
				break;
			case UFFD_EVENT_PAGEFAULT:
				tracker->stats.evt_pagefault++;
				psm3_uffd_handle_pagefault(msg.arg.pagefault.address,
								msg.arg.pagefault.flags);
				break;
			default:
				_HFI_ERROR("uffd unexpected event\n");
				break;
			}
		}
		PSMI_UNLOCK(psm3_uffd_lock);
		if (ret > 0 && ret != sizeof(msg)) {
			_HFI_ERROR("uffd read incomplete: Got %d bytes, expected %u\n",
						ret, (unsigned)sizeof(msg));
		}
	} while (ret == sizeof(msg));
}

// --------------------------------------------------------------------------
// userfaultfd handler thread

// We must handle uffd events on a separate thread, because the thread
// causing the event is paused until the event is read.

// main body of uffd handler thread
static void *uffd_handler(void *arg)
{
	psm3_uffd_tracker_t tracker = (psm3_uffd_tracker_t)arg;
	struct pollfd fds[2];
	int fd_pipe = tracker->uffd_handler.pipefd[0];
	int ret;

	fds[0].fd = fd_pipe;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	fds[1].fd = tracker->fd;
	fds[1].events = POLLIN;
	fds[1].revents = 0;
	int nfds = sizeof(fds)/sizeof(fds[0]);
	while (1) {
		ret = poll(&fds[0], nfds, -1);
		if_pf(ret < 0) {
			if (errno == EINTR) {
				_HFI_MMDBG("got signal, keep polling\n");
			} else {
				psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"userfaultfd poll() error: %s", strerror(errno));
				break;
			}
		}
		if (fds[0].revents) {
			_HFI_MMDBG("close thread: revents=0x%x\n", fds[0].revents);
			break;
		}
		if (fds[1].revents & POLLIN) {
			psm3_uffd_event();
		}
	}
	close(fd_pipe);
	PSM2_LOG_MSG("leaving");
	pthread_exit(NULL);
}

// --------------------------------------------------------------------------
// Worker thread to perform ibv_dereg_mr outside of uffd thread

// The worker thread is manditory, because even if psm3_verbs_mr.c is using an
// event queue, we can have queue overflow and be forced to do invalidate in the
// uffd_handler callback.  In which case we must defer dereg to somewhere else.
// In theory we could defer it to just before or after event processing
// in psm3_verbs_reg_mr, but that is likely to have a worse performance
// impact than having this dereg worker thread.

void psm3_uffd_queue_dereg(psm3_verbs_mr_t mrc)
{
	uint8_t msg = 1;
	int was_empty;
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	was_empty = TAILQ_EMPTY(&tracker->dereg_list);
	TAILQ_INSERT_TAIL(&tracker->dereg_list, mrc, next);
	_HFI_MMDBG("queue dereg %p%s\n", mrc, was_empty?" kick":"");
	if (was_empty) {
		if (write(tracker->uffd_worker.pipefd[1],(const void *)&msg,
					sizeof(msg)) == -1)
			_HFI_ERROR("unable to write to uffd worker thread\n");
	}
}

// main body of uffd dreg worker thread
static void *uffd_dereg_worker(void *arg)
{
	psm3_uffd_tracker_t tracker = (psm3_uffd_tracker_t)arg;
	struct pollfd fds[1];
	int fd_pipe = tracker->uffd_worker.pipefd[0];
	int ret;

	fds[0].fd = fd_pipe;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int nfds = sizeof(fds)/sizeof(fds[0]);
	while (1) {
		ret = poll(&fds[0], nfds, -1);
		if_pf(ret < 0) {
			if (errno == EINTR) {
				_HFI_MMDBG("got signal, keep polling\n");
			} else {
				psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"userfaultfd worker poll() error: %s", strerror(errno));
				break;
			}
		}
		if (fds[0].revents) {
			uint8_t msg;
			ret = read(fd_pipe, &msg, sizeof(msg));
			if (ret == sizeof(msg)) {
				struct psm3_verbs_mr *mrc;
				if (msg == 0) {
					_HFI_MMDBG("close thread: revents=0x%x\n", fds[0].revents);
					break;
				}
				do {
					PSMI_LOCK(psm3_uffd_lock);
					mrc = TAILQ_FIRST(&tracker->dereg_list);
					if (mrc) {
						TAILQ_REMOVE(&tracker->dereg_list, mrc, next);
						PSMI_UNLOCK(psm3_uffd_lock);
						psm3_verbs_umrc_worker_dereg(mrc);
					} else  {
						PSMI_UNLOCK(psm3_uffd_lock);
					}
				} while (mrc);
			} else if (ret > 0 && ret != sizeof(msg)) {
				_HFI_ERROR("uffd worker read incomplete: Got %d bytes, expected %u\n",
						ret, (unsigned)sizeof(msg));
			}
		}
	}
	close(fd_pipe);
	PSM2_LOG_MSG("leaving");
	pthread_exit(NULL);
}

// --------------------------------------------------------------------------
// UFFD tracker singleton initialize/finalize
// and callback registration

// create one of the UFFD threads
static psm2_error_t psm3_uffd_thread_init(struct uffd_thread *thrd, void *(*thread_main) (void *))
{
	int err = PSM2_OK;

	PSMI_LOCK_ASSERT(psm3_creation_lock);

#ifdef PSMI_PLOCK_IS_NOLOCK
	psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"#define PSMI_PLOCK_IS_NOLOCK not allowed with "
					"with PSM3_CACHE_MODE=%d", MR_CACHE_MODE_USER);
#endif

	if (pipe(thrd->pipefd) != 0) {
		psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"Cannot create a pipe for uffd thread: %s\n",
						strerror(errno));
		goto fail;
	}
	if (pthread_create(&thrd->thread, NULL, thread_main, tracker)) {
		int save_errno = errno;
		close(thrd->pipefd[0]);
		close(thrd->pipefd[1]);
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
						"Cannot start uffd thread: %s\n",
						strerror(save_errno));
		goto fail;
	}

	return PSM2_OK;

fail:
	return err;
}

// finalize one of the UFFD threads
static void psm3_uffd_thread_fini(struct uffd_thread* thrd)
{
	PSMI_LOCK_ASSERT(psm3_creation_lock);

	uint8_t emsg = 0;
	if (write(thrd->pipefd[1],(const void *)&emsg, sizeof(emsg)) == -1 ||
			close(thrd->pipefd[1]) == -1) {
		_HFI_MMDBG("unable to close pipe to uffd thread cleanly\n");
		pthread_cancel(thrd->thread); // try to cancel
	}
	pthread_join(thrd->thread, NULL);
}

// allocate and initialize the tracker singleton
// max_mrs must be a power of 2
psm2_error_t psm3_uffd_tracker_init(uint32_t max_mrs)
{
	struct uffdio_api api;
	union psmi_envvar_val env_val;

	PSMI_LOCK_ASSERT(psm3_creation_lock);
	tracker = (psm3_uffd_tracker_t)psmi_calloc(NULL, DESCRIPTORS,
							sizeof(*tracker), 1);
	if (! tracker) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to allocate memory for uffd tracker: errno %d: %s\n",
				errno, strerror(errno));
		return PSM2_NO_MEMORY;
	}
	psm3_getenv("PSM3_MR_CACHE_FORCE_UNREGISTER",
			"Force UFFD UNREGISTER calls for invalidate events (0 disables, 1=individual, 2=consolidate)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &env_val);
	tracker->force_unregister = env_val.e_uint;

	if (PSM2_OK != map_item_pool_init(max_mrs))
		goto fail_free;

	tracker->fd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
	if (tracker->fd < 0) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to create userfaultfd for uffd tracker: errno %d: %s\n",
				errno, strerror(errno));
		goto fail_mpool;
	}

	api.api = UFFD_API;
	api.features = UFFD_FEATURE_EVENT_UNMAP | UFFD_FEATURE_EVENT_REMOVE | UFFD_FEATURE_EVENT_REMAP;
	if (ioctl(tracker->fd, UFFDIO_API, &api) < 0) {
		_HFI_ERROR( "Unable to subscribe for userfaultfd events: %s\n", strerror(errno));
		goto fail;
	}
	if (api.api != UFFD_API) {
		_HFI_ERROR( "Unexpected API returned from userfaultfd API: %llu, expected: %llu\n",
				(long long unsigned) api.api, (long long unsigned)UFFD_API);
		goto fail;
	}
	_HFI_MMDBG("uffd fd=%d\n", tracker->fd);

	tracker->page_size = PSMI_PAGESIZE;
	tracker->last_callback = -1;
	//nil_item already zeroed by calloc
	//memset(&tracker->nil_item.payload, 0, sizeof(tracker->nil_item.payload));
	ips_cl_qmap_init(&tracker->map, &tracker->root, &tracker->nil_item);
	TAILQ_INIT(&tracker->dereg_list);
	// stats zeroed by calloc
	psmi_assert(uffd_validate_map(0, "After Alloc"));
	if (PSM2_OK != psm3_uffd_thread_init(&tracker->uffd_handler, uffd_handler))
		goto fail;
	if (PSM2_OK != psm3_uffd_thread_init(&tracker->uffd_worker, uffd_dereg_worker))
		goto fail_stop;

	register_uffd_stats();
	return PSM2_OK;

fail_stop:
	psm3_uffd_thread_fini(&tracker->uffd_handler);
fail:
	close(tracker->fd);
fail_mpool:
	map_item_pool_fini();
fail_free:
	psmi_free(tracker);
	tracker = NULL;
	return PSM2_INTERNAL_ERR;
}

#ifndef UMR_FAST_FLUSH
// flush_uffd_regions() in our caller should have uffd_deregister all regions
// so we actually should find nothing to do here
static void free_all_uffd_regions(void)
{
	while (tracker->map.payload.nelems) {
		if (psm3_mr_cache_debug)
			_HFI_INFO("free nelems=%"PRIu64"\n", tracker->map.payload.nelems);
		else
			_HFI_MMDBG("free nelems=%"PRIu64"\n", tracker->map.payload.nelems);
		cl_map_item_t *p_item = __cl_map_root(&tracker->map);
		psmi_assert(p_item != tracker->map.nil_item);
		uint64_t addr = p_item->payload.addr;
		uint64_t endp1 = p_item->payload.endp1;
		if (unregister_region(p_item, PSM3_UFFD_EVENT_NONE, 0, 0)) {
			_HFI_ERROR("Unable to UNREGISTER uffd "UFFD_RANGE_FMT"\n",
						UFFD_OUT_RANGE(addr, endp1));
		}
	}
	psmi_assert(uffd_validate_map(0, "After Free"));
}
#endif /* UMR_FAST_FLUSH */

// finalize and free the tracker singleton
void psm3_uffd_tracker_fini(void)
{
	PSMI_LOCK_ASSERT(psm3_creation_lock);
	psmi_assert(tracker);
	if (tracker->num_callback)
		return;

	// don't pollute stats with our shutdown activities
	psm3_stats_deregister_type(PSMI_STATSTYPE_MR_CACHE, tracker);

	psmi_assert(uffd_validate_map(1, "Before Free"));
	_HFI_MMDBG("free nelems=%"PRIu64"\n", tracker->map.payload.nelems);

#ifdef UMR_FAST_FLUSH
	// for fast flush the close(tracker->fd) below will UNREGISTER all regions
	// and the mpool_destroy will free all the uffd_regions so we can skip the
	// explicit free
#else
	// all caches should have been freed and unregistered all their MRs
	// however on error there may still be some remaining, so try again
	free_all_uffd_regions();
#endif /* UMR_FAST_FLUSH */

	// no regions, so we can stop thread without fear of an event pausing app
	psm3_uffd_thread_fini(&tracker->uffd_handler);
	psm3_uffd_thread_fini(&tracker->uffd_worker);
	psmi_assert(TAILQ_EMPTY(&tracker->dereg_list));

	_HFI_MMDBG("close fd=%d\n", tracker->fd);
	close(tracker->fd);

	// do this after close so free of pool can't cause events, especially
	// when UMR_FAST_FLUSH has skipped final UNREGISTER
	map_item_pool_fini();

	psmi_free(tracker);
	tracker = NULL;
}

psm2_error_t psm3_uffd_add_callback(psm2_mr_cache_t cache, uint32_t max_mrs)
{
	int i, avail, last;

	PSMI_LOCK_ASSERT(psm3_creation_lock);
	psmi_assert(cache);
	if (! tracker) {
		if (PSM2_OK != psm3_uffd_tracker_init(max_mrs))
			return PSM2_INTERNAL_ERR;
	} else if (tracker->num_callback >= MAX_UFFD_CALLBACKS)
		return PSM2_TOO_MANY_ENDPOINTS;

	PSMI_LOCK(psm3_uffd_lock);	// for races with notification processing
#ifdef PSM_DEBUG
	for (i=0; i < MAX_UFFD_CALLBACKS; i++) {
		psmi_assert(tracker->callback_list[i] != cache);
		psmi_assert(i <= tracker->last_callback || ! tracker->callback_list[i]);
	}
#endif

	// find 1st available and recompute last
	for (i=0, avail = -1, last = -1; i < MAX_UFFD_CALLBACKS; i++) {
		if (! tracker->callback_list[i]) {
			if (avail < 0) {	// first available, will use this slot
				avail = i;
				last = i;
			}
		} else {
			last = i;
		}
	}
	if_pf(avail < 0 || avail >= MAX_UFFD_CALLBACKS) {
		PSMI_UNLOCK(psm3_uffd_lock);
		return psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Unable to find any available UFFD callbacks: %d\n", avail);
	}
	psmi_assert(tracker->num_callback || last == 0);
	psmi_assert(tracker->num_callback || avail == 0);
	tracker->callback_list[avail] = cache;
	tracker->num_callback++;
	tracker->last_callback = last;
	PSMI_UNLOCK(psm3_uffd_lock);	// for races with notification processing
	return PSM2_OK;
}

void psm3_uffd_remove_callback(psm2_mr_cache_t cache)
{
	int i, match, last;

	PSMI_LOCK_ASSERT(psm3_creation_lock);
	PSMI_LOCK_ASSERT(psm3_uffd_lock);
	psmi_assert(tracker);
	psmi_assert(cache);
	psmi_assert(tracker->num_callback);

	// find match and recompute last
	for (i=0, match = -1, last = -1; i < MAX_UFFD_CALLBACKS; i++) {
		if (tracker->callback_list[i] == cache) {
			tracker->callback_list[i] = NULL;
			match = i;
		} else {
			tracker->last_callback = i;
		}
	}
	if_pf(match < 0 || match >= MAX_UFFD_CALLBACKS) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Unable to find any match in UFFD callbacks: %d\n", match);
		return;
	}
	tracker->num_callback--;
	tracker->last_callback = last;
	psmi_assert(tracker->num_callback || last == -1);
}

// caller must hold creation_lock or uffd_lock
unsigned psm3_uffd_get_num_callback(void)
{
	if (! tracker)
		return 0;
	return (tracker->num_callback);
}

#ifdef PSM_DEBUG
// --------------------------------------------------------------------------
// UFFD region map self consistency checking

// These functions allow for self consistency checking to insure our
// internal region lists are being tracked properly

/* save refcount to old_refcount for all overlapping regions */
static void uffd_save_refcount(void)
{
	if (! psm3_mr_cache_debug)
		return;
	// do for all regions so can check those before and after
	// desired range
	cl_map_item_t *p_item = ips_cl_qmap_first(&tracker->map);
	for (; p_item != tracker->map.nil_item;
			p_item = ips_cl_qmap_successor(&tracker->map, p_item)) {
		p_item->payload.old_refcount = p_item->payload.refcount;
	}
}

/* confirm refcount was decremented for all remaining overlapping regions */
static int uffd_check_dec_refcount(uint64_t addr, uint64_t endp1,
									const char *caller)
{
	if (! psm3_mr_cache_debug)
		return 1;

	int ret = 1;
	cl_map_item_t *p_item = ips_cl_qmap_search(&tracker->map, addr, endp1);
	// check region immediately before
	if (p_item != tracker->map.nil_item) {
		cl_map_item_t *p_tmp = ips_cl_qmap_predecessor(&tracker->map, p_item);
		if (p_tmp != tracker->map.nil_item
			&& p_tmp->payload.old_refcount != p_tmp->payload.refcount) {
			_HFI_INFO("UFFD incorrect refcount %s expect %u got %u "UFFD_RANGE_FMT
					  " for registration "UFFD_RANGE_FMT"\n",
					caller, p_tmp->payload.old_refcount,
					p_tmp->payload.refcount, UFFD_OUT_ITEM(p_tmp),
					UFFD_OUT_RANGE(addr, endp1));
			ret = 0;
		}
	}

	for (; p_item != tracker->map.nil_item && p_item->payload.addr < endp1;
			p_item = ips_cl_qmap_successor(&tracker->map, p_item)) {
		if (p_item->payload.old_refcount-1 != p_item->payload.refcount) {
			_HFI_INFO("UFFD incorrect refcount %s expect %u got %u "UFFD_RANGE_FMT
					  " for registration "UFFD_RANGE_FMT"\n",
					caller, p_item->payload.old_refcount-1,
					p_item->payload.refcount, UFFD_OUT_ITEM(p_item),
					UFFD_OUT_RANGE(addr, endp1));
			ret = 0;
		}
	}

	// check region immediately after (now p_item)
	if (p_item != tracker->map.nil_item
		&& p_item->payload.old_refcount != p_item->payload.refcount) {
			_HFI_INFO("UFFD incorrect refcount %s expect %u got %u "UFFD_RANGE_FMT
					  " for registration "UFFD_RANGE_FMT"\n",
					caller, p_item->payload.old_refcount,
					p_item->payload.refcount, UFFD_OUT_ITEM(p_item),
					UFFD_OUT_RANGE(addr, endp1));
		ret = 0;
	}

	if (! ret) {
		if (! uffd_validate_map(1, caller))
			_HFI_INFO("UFFD map invalid %s\n", caller);
	}
	return ret;
}

/* returns 1 if region is registered, 0 if not fully covered */
static int uffd_validate_region(uint64_t addr, uint64_t endp1, int chk_inc,
								const char *caller)
{
	if (! psm3_mr_cache_debug)
		return 1;

	int ret = 1;
	cl_map_item_t *p_item = ips_cl_qmap_search(&tracker->map, addr, endp1);
	uint64_t tmp_addr = addr;

	// check region immediately before
	if (chk_inc && p_item != tracker->map.nil_item) {
		cl_map_item_t *p_tmp = ips_cl_qmap_predecessor(&tracker->map, p_item);
		if (p_tmp != tracker->map.nil_item
			&& p_tmp->payload.old_refcount != p_tmp->payload.refcount) {
			_HFI_INFO("UFFD incorrect refcount %s expect %u got %u "UFFD_RANGE_FMT
					" for registration "UFFD_RANGE_FMT"\n",
					caller, p_tmp->payload.old_refcount,
					p_tmp->payload.refcount,
					UFFD_OUT_ITEM(p_tmp), UFFD_OUT_RANGE(addr, endp1));
			ret = 0;
		}
	}

	for (; p_item != tracker->map.nil_item && p_item->payload.addr < endp1;
			p_item = ips_cl_qmap_successor(&tracker->map, p_item)) {
		if (p_item->payload.addr != tmp_addr || p_item->payload.endp1 > endp1) {
			_HFI_INFO("UFFD bad region %s "UFFD_RANGE_FMT" for registration "
						UFFD_RANGE_FMT"\n",
						caller, UFFD_OUT_ITEM(p_item),
						UFFD_OUT_RANGE(addr, endp1));
			ret = 0;
		}
		if (chk_inc > 0 && p_item->payload.old_refcount+1 != p_item->payload.refcount) {
			_HFI_INFO("UFFD incorrect refcount %s expect %u got %u "UFFD_RANGE_FMT
						" for registration "UFFD_RANGE_FMT"\n",
						caller, p_item->payload.old_refcount+1,
						p_item->payload.refcount, UFFD_OUT_ITEM(p_item),
						UFFD_OUT_RANGE(addr, endp1));
			ret = 0;
		}
		tmp_addr = p_item->payload.endp1;
	}
	if (tmp_addr != endp1) {
		_HFI_INFO("UFFD bad uncovered %s 0x%16"PRIx64
					" for registration "UFFD_RANGE_FMT"\n",
					caller, tmp_addr, UFFD_OUT_RANGE(addr, endp1));
		ret = 0;
	}

	// check region immediately after (now p_item)
	if (p_item != tracker->map.nil_item
		&& p_item->payload.old_refcount != p_item->payload.refcount) {
			_HFI_INFO("UFFD incorrect refcount %s expect %u got %u "UFFD_RANGE_FMT
					" for registration "UFFD_RANGE_FMT"\n",
					caller, p_item->payload.old_refcount,
					p_item->payload.refcount, UFFD_OUT_ITEM(p_item),
					UFFD_OUT_RANGE(addr, endp1));
		ret = 0;
	}

	if (! ret) {
		if (! uffd_validate_map(1, caller))
			_HFI_INFO("UFFD map invalid %s\n", caller);
	}
	return ret;
}

/* returns 1 if map is valid, 0 if unexpected region overlap found */
static int uffd_validate_map(int dump, const char *caller)
{
	if (! (dump && _HFI_MMDBG_ON) && ! psm3_mr_cache_debug)
		return 1;

	cl_map_item_t *p_prev = ips_cl_qmap_first(&tracker->map);
	cl_map_item_t *p_tmp;
	int ret = 1;

	_HFI_MMDBG("validate %s\n", caller);
	if (dump) {
		_HFI_MMDBG("UFFD Regions %s nelems: %"PRIu64"\n",
				caller, tracker->map.payload.nelems);
		_HFI_MMDBG("      Addr              End        Refcount\n");
	}
	// walk entire map, validating regions, p_prev is one before p_tmp
	// except for 1st loop where they are equal so we can dump 1st entry
	for (p_tmp = p_prev;
			p_tmp != tracker->map.nil_item;
			p_tmp = ips_cl_qmap_successor(&tracker->map, p_tmp)) {
		int overlaps;
		overlaps = (p_tmp != p_prev && overlaps_item(p_tmp, p_prev));
		if (overlaps)
			ret = 0;
		if (! p_tmp->payload.refcount)
			ret = 0;
		if (dump) {
			_HFI_MMDBG("0x%16"PRIx64":0x%16"PRIx64" %10u %s\n",
				p_tmp->payload.addr, p_tmp->payload.endp1-1,
				p_tmp->payload.refcount, overlaps?"OVERLAP":"");
		} else if (overlaps) {
			_HFI_INFO("UFFD 0x%16"PRIx64":0x%16"PRIx64" overlaps 0x%"PRIx64":0x%"PRIx64"\n",
				p_prev->payload.addr, p_prev->payload.endp1-1,
				p_tmp->payload.addr, p_tmp->payload.endp1-1);
			uffd_validate_map(1, caller);
			break;
		} else if (! p_tmp->payload.refcount) {
			_HFI_INFO("UFFD 0x%16"PRIx64":0x%16"PRIx64" zero refcount\n",
				p_tmp->payload.addr, p_tmp->payload.endp1-1);
			uffd_validate_map(1, caller);
			break;
		}
		p_prev = p_tmp;
	}
	_HFI_MMDBG("Done validate %s\n", caller);
	return ret;
}
#endif /* PSM_DEBUG */

// --------------------------------------------------------------------------
// Statistics registration

#ifdef RBTREE_VALIDATE
#define UFFD_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		psm3_uffd_tracker_t tracker = (psm3_uffd_tracker_t)context; \
		return tracker->stat; \
	}

UFFD_STAT_FUNC(uffd_tree_depth, map.depth)
UFFD_STAT_FUNC(uffd_tree_max_depth, map.max_depth)
#undef UFFD_STAT_FUNC
#endif

static void register_uffd_stats(void)
{
	struct psmi_stats_entry entries[] = {
#ifdef USE_MPOOL
		// -------------------------------------------------------------------
		PSMI_STATS_DECL_HELP("User Fault FD Configuration:"),
		PSMI_STATS_DECLU64("limit_regions",
				"Limit of registered uffd memory regions",
				&tracker->limit_nelems),
#endif
		// -------------------------------------------------------------------
		PSMI_STATS_DECL_HELP("User Fault FD Event Statistics:"),
		PSMI_STATS_DECLU64("evt_remove",
				"Memory removal event notifications",
				&tracker->stats.evt_remove),
		PSMI_STATS_DECLU64("evt_unmap",
				"Memory unmap event notifications",
				&tracker->stats.evt_unmap),
		PSMI_STATS_DECLU64("evt_remap",
				"Memory remap event notifications",
				&tracker->stats.evt_remap),
		PSMI_STATS_DECLU64("evt_pagefault",
				"Memory pagefault event notifications",
				&tracker->stats.evt_pagefault),
		// -------------------------------------------------------------------
		PSMI_STATS_DECL_HELP("User Fault FD Region Registration Statistics:"),
		PSMI_STATS_DECLU64("regions",
				"Current registered uffd memory regions",
				&tracker->map.payload.nelems),
		PSMI_STATS_DECLU64("max_regions",
				"Max observed registered uffd memory regions",
				&tracker->stats.max_nelems),
		PSMI_STATS_DECLU64("failed_alloc_region",
				"Total failed allocate of uffd memory region",
				&tracker->stats.failed_alloc_region),
		PSMI_STATS_DECLU64("max_refcount",
				"Max observed MR refernces to a uffd memory regions",
				&tracker->stats.max_refcount),
#ifdef RBTREE_VALIDATE
		PSMI_STATS_DECL_FUNC("tree_depth",
				"Current uffd memory region search tree depth",
				uffd_tree_depth),
		PSMI_STATS_DECL_FUNC("tree_max_depth",
				"Max observed uffd memory region search tree depth",
				uffd_tree_max_depth),
#endif
		PSMI_STATS_DECLU64("register",
				"Total successful uffd calls to REGISTER a memory region",
				&tracker->stats.register_region),
		PSMI_STATS_DECLU64("failed_register",
				"Total failed uffd calls to REGISTER a memory region",
				&tracker->stats.failed_register_region),
		PSMI_STATS_DECLU64("unregister",
				"Total successful uffd calls to UNREGISTER a memory region",
				&tracker->stats.unregister_region),
		PSMI_STATS_DECLU64("failed_unregister",
				"Total failed uffd calls to UNREGISTER a memory region",
				&tracker->stats.failed_unregister_region),
		PSMI_STATS_DECLU64("ignored_failed_unregister",
				"Total ignored failures of  uffd calls to UNREGISTER a memory region",
				&tracker->stats.ignored_failed_unregister_region),
		PSMI_STATS_DECLU64("ignored_unregister",
				"Subset of successful uffd calls to UNREGISTER where would have ignored errors. This does not necessarily indicate an issue as the kernel will accept UNREGISTER calls for any valid address range, even if already UNREGISTERed.",
				&tracker->stats.ignored_unregister_region),
		PSMI_STATS_DECLU64("entire_mr",
				"Number of regions registered for an entire MR",
				&tracker->stats.entire_mr),
		PSMI_STATS_DECLU64("fill_gap",
				"Number of regions registered to fill gaps",
				&tracker->stats.fill_gap),
		PSMI_STATS_DECLU64("split",
				"Number of regions split",
				&tracker->stats.split),
	};
	psm3_stats_register_type("UFFD_Statistics",
			"When the PSM3 user space MR cache is enabled to retain MRs "
			"after use, a Linux User Fault FD (UFFD) is established to get "
			"memory events and trigger MR removal for MRs which correspond "
			"to memory the application has freed.\n"
			"The UFFD tracker must track all regions of memory which "
			"correspond for the virtual addresses of cached MRs."
			"When such memory is freed or remapped by the linux kernel "
			"the UFFD receives events and must ensure the corresponding MRs "
			"are removed from the user space MR cache.",
						PSMI_STATSTYPE_MR_CACHE,
						entries, PSMI_HOWMANY(entries),
						NULL, tracker, NULL);
}

#endif /* PSM_HAVE_REG_MR */
#endif /* UMR_CACHE */
