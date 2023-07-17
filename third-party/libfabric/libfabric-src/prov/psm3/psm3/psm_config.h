/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2018 Intel Corporation.

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

  Copyright(c) 2018 Intel Corporation.

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

#ifndef PSM_CONFIG_H
#define PSM_CONFIG_H

/* This header cannot include other headers.  Low level routines
 * depend on it not pulling in macros or inline functions for heap or HAL
 */

/*
 * The following flags can be used instead of `make` switches in order to
 * change behavior achieved when using `make` without parameters.
 */

#ifndef RDPMC_PERF_FRAMEWORK
/* #define RDPMC_PERF_FRAMEWORK */
#endif

#ifndef PSM2_MOCK_TESTING
/* #define PSM2_MOCK_TESTING */
#endif

#ifndef PSM_CUDA
/* #define PSM_CUDA */
/* #define NVIDIA_GPU_DIRECT */
#endif

#ifndef PSM_ONEAPI
/* #define PSM_ONEAPI */
/* #define INTEL_GPU_DIRECT */
#endif

#ifndef PSM3_BRAKE_DEBUG
/* #define PSM3_BRAKE_DEBUG */
#endif

#ifndef PSM_DEBUG
/* #define PSM_DEBUG */
/* #define _HFI_DEBUGGING 1 */
/* #define _FORTIFY_SOURCE 2 */
#endif

#ifndef PSM_HEAP_DEBUG
/* #define PSM_HEAP_DEBUG */
#endif

#ifndef PSM_PROFILE
/* #define PSM_PROFILE */
#endif

#define PSMI_MIN_EP_CONNECT_TIMEOUT	(2 * SEC_ULL)
#define PSMI_MIN_EP_CLOSE_TIMEOUT	(1 * SEC_ULL)
#define PSMI_MAX_EP_CLOSE_TIMEOUT	(2 * SEC_ULL)

#define PSMI_MIN_EP_CLOSE_GRACE_INTERVAL (1 * SEC_ULL)
#define PSMI_MAX_EP_CLOSE_GRACE_INTERVAL (2 * SEC_ULL)


#define PSMI_MAX_RAILS		32 /* Max number of unique devices */
				      			/* also sets PSMX3_MAX_UNITS in psmx3.h */
#define PSMI_MAX_QPS		32 /* Max number of total QPs (QPs/NIC * RAILs) */
								/* must be >= PSMI_MAX_RAILS */

#define AFFINITY_SHM_BASENAME			"/psm3_nic_affinity_shm"
#define AFFINITY_SHM_REF_COUNT_LOCATION		0
#define AFFINITY_SHM_HFI_INDEX_LOCATION		1
#define SEM_AFFINITY_SHM_RW_BASENAME		"/psm3_nic_affinity_shm_rw_mutex"

#define PSMI_RCVTHREAD_FLAGS	0x1
/**<
 * Default setting for Receive thread
 *
 * 0x0 disables rcvthread by default
 * 0x1 enables ips receive thread by default
 */

/*
 * Define one of these below.
 *
 * Spinlock gives the best performance and makes sense with the progress thread
 * only because the progress thread does a "trylock" and then goes back to
 * sleep in a poll.
 *
 * Mutexlock should be used for experimentation while the more useful
 * mutexlock-debug should be enabled during development to catch potential
 * errors.
 */
#ifdef PSM_DEBUG
#define PSMI_LOCK_IS_MUTEXLOCK_DEBUG
#else
#define PSMI_LOCK_IS_SPINLOCK
/* #define PSMI_LOCK_IS_MUTEXLOCK */
/* #define PSMI_LOCK_IS_MUTEXLOCK_DEBUG */
/* #define PSMI_PLOCK_IS_NOLOCK */
#endif

#ifdef PSM_CUDA
/* XXX TODO: Getting the gpu page size from driver at init time */
#define PSMI_GPU_PAGESIZE 65536

#elif defined(PSM_ONEAPI)

#define PSMI_GPU_PAGESIZE 4096

#endif // PSM_CUDA

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define CUDA_WINDOW_PREFETCH_DEFAULT	2
#define CUDA_SMALLHOSTBUF_SZ	(256*1024)
#define GPU_PAGE_OFFSET_MASK (PSMI_GPU_PAGESIZE -1)
#define GPU_PAGE_MASK ~GPU_PAGE_OFFSET_MASK
/* All GPU transfers beyond this threshold use
 * RNDV protocol. It is mostly a send side knob.
 */
#define CUDA_THRESH_RNDV 8000

#define GPUDIRECT_THRESH_RV 3

#define GDR_COPY_LIMIT_SEND 128
#define GDR_COPY_LIMIT_RECV 64000

#endif /* PSM_CUDA || PSM_ONEAPI */


#define PSM_MQ_NIC_MAX_TINY		8	/* max TINY payload allowed */
#define PSM_MQ_NIC_MAX_RNDV_WINDOW	(4 * 1024 * 1024) /* max rndv window */

#define MQ_SHM_THRESH_RNDV 16000

#define NUM_HASH_BUCKETS 64
#define HASH_THRESHOLD 65
#define NUM_HASH_CONFIGS 3
#define NUM_MQ_SUBLISTS (NUM_HASH_CONFIGS + 1)

#define REMOVE_ENTRY 1


/* Keep timer stats */
#define PSMI_TIMER_STATS	0


/* Psm context */
#define HAL_CONTEXT_OPEN_RETRY_MAX 3


/*
 * By default, PSMI_DEVICES_DEFAULT establishes the bind order a component is
 * tested for reachability to each peer.  First self, then shm and finally
 * hfi.  The order should really only affect endpoints that happen to be on
 * the same node.  PSM will correctly detect that two endpoints are on the same
 * node even though they may be using different host interfaces.
 */
#define PSMI_DEVICES_DEFAULT	"self,shm,nic"

/* Lock */
#define PSMI_USE_PTHREAD_SPINLOCKS	0

/* Utils */
#define PSMI_EPID_TABSIZE_CHUNK		128
#define PSMI_EPID_TABLOAD_FACTOR	((float)0.7)

#define	PSMI_EP_HOSTNAME_LEN	64	/* hostname only */

#define PSM3_FAULTINJ_SPEC_NAMELEN	32

#endif /* PSM_CONFIG_H */
