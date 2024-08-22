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

#ifndef _PSM_UFFD_H
#define _PSM_UFFD_H

#include "stdint.h"
#include "psm_user.h"   // sets PSM_HAVE_REG_MR
                        // and includes hal_verbs/verbs_ep.h and psm_verbs_mr.h

#ifdef UMR_CACHE
#ifdef PSM_HAVE_REG_MR
// TBD - key off HAVE_LINUX_USERFAULTFD_H in configure to enable UMR_CACHE

// an mpool or a simple free list may be chosen.
// See psm_uffd.c for a discussion of the trade-offs.
//#define USE_MPOOL

// Testing shows minimal time in job shutdown in psm3_verbs_free_mr_cache
// but, if want to explore this optimization can enable UMR_FAST_FLUSH.
// When UMR_FAST_FLUSH is enabled, the final psm3_verbs_free_mr_cache
// skips the UNREGISTER and mpool_put of uffd_regions and depends on
// close(tracker->fd) and mpool_destroy to clean them all up
#ifdef USE_MPOOL
#ifndef PSM_DEBUG
//#define UMR_FAST_FLUSH
#endif
#endif

// Indicates the high level reason for a callback so can be passed
// through to psm3_uffd_deregistsr
enum {
	PSM3_UFFD_EVENT_NONE=0,	// deregister unrelated to event
	PSM3_UFFD_EVENT_INVALIDATE,	// UNMAP or REMAP triggered invalidation
	PSM3_UFFD_EVENT_REMOVE,	// REMOVE triggered invalidation
};

extern psmi_lock_t psm3_uffd_lock;

struct psm3_uffd_region;	// opaque
typedef struct psm3_uffd_region *psm3_uffd_region_t;

// tracker created on 1st add_callback
psm2_error_t psm3_uffd_add_callback(psm2_mr_cache_t cache, uint32_t max_mrs);
void psm3_uffd_remove_callback(psm2_mr_cache_t cache);
unsigned psm3_uffd_get_num_callback(void);
// fini destroys tracker if no more MR caches with callbacks
void psm3_uffd_tracker_fini(void);

// region returned on register must be supplied for corresponding deregister
// this speeds up the search in deregister
psm3_uffd_region_t psm3_uffd_register(uint64_t addr, uint64_t length);
void psm3_uffd_deregister(uint64_t addr, uint64_t length,
						 psm3_uffd_region_t reg, uint8_t reason,
						uint64_t evt_addr, uint64_t evt_length);
void psm3_uffd_queue_dereg(psm3_verbs_mr_t mrc);

#endif // PSM_HAVE_REG_MR
#endif // UMR_CACHE
#endif // _PSM_UFFD_H
