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

#ifndef _PSMI_USER_H
#define _PSMI_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Instead of testing a HAL cap mask bit at runtime (in addition to thresholds),
 * we only test thresholds, especially in the ips_proto_mq.c datapath.
 * To allow for slightly more optimized builds, a few build time capability
 * flags are set which reflect if any of the built-in HALs selected have
 * the potential to support the given feature.  If none do, the code will be
 * omitted.  All HALs must make sure the thresholds are properly set so the
 * feature is disabled when not available, in which case runtime threshold
 * checks will skip the feature.  A good example is the REG_MR capability.
 */

/* This indicates at least 1 HAL in the build can register MRs for use in
 * send DMA or RDMA.
 * If Send DMA is not available, the various eager_thresh controls in ips_proto
 * must be disabled (set to ~0).
 * If RDMA is not available, proto->protoexp must be NULL.
 */
#if defined(PSM_VERBS)
#define PSM_HAVE_REG_MR
#endif

/* This indicates at least 1 HAL in the build can perform Send DMA */
#ifdef PSM_VERBS
#define PSM_HAVE_SDMA
#endif

/* This indicates the user space RC QP reconnection protocol is enabled
 * this should only be tested inside the verbs HAL.  Since it is only
 * set when USE_RC is set, there is no need to also test USE_RC when
 * testing PSM_RC_RECONNECT
 * PSM_RC_RECONNECT_SRQ allows use of SRQ when reconnect enabled.
 */
#if defined(PSM_VERBS) && defined(USE_RC)
#define PSM_RC_RECONNECT
#define PSM_RC_RECONNECT_SRQ
#endif
 
/* This indicates at least 1 HAL in the build can perform RDMA */
#if defined(PSM_VERBS) && (defined(RNDV_MOD) || defined(USE_RC))
#define PSM_HAVE_RDMA
#endif

/* This indicates at least 1 HAL in the build can perform RDMA err_chk */
#if defined(PSM_VERBS) && (defined(RNDV_MOD) || defined(PSM_RC_RECONNECT))
#define PSM_HAVE_RDMA_ERR_CHK
#endif

// psm_config.h will define PSM_HAVE_GPU as needed
#include "psm_config.h"

#ifdef RNDV_MOD
/* This is used to guard all RNDV_MOD code in the main parts of PSM
 * so that RNDV_MOD code is only really enabled when a HAL present is able
 * to take advantage of it
 */
/* This indicates at least 1 HAL in the build can take advantage of the
 * rendezvous module.  This define should be tested outside the individual
 * HALs instead of testing specific HAL flags like PSM_VERBS or PSM_SOCKETS.
 * Thus, when adding a new HAL, the generic code need not be revisited.
 */
#if defined(PSM_VERBS) || (defined(PSM_SOCKETS) && defined(PSM_HAVE_GPU))
#define PSM_HAVE_RNDV_MOD
#endif /* VERBS || (SOCKETS && GPU) */
#endif /* RNDV_MOD */


#if defined(PSM_HAVE_GPU) && defined(PSM_USE_HWLOC)
#define PSM_HAVE_GPU_CENTRIC_AFFINITY
#endif

#include <inttypes.h>
#include <pthread.h>

#include <sched.h>
#include <numa.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>

#include "psm2.h"
#include "psm2_mq.h"

#include "ptl.h"

#include "utils_user.h"
#include "utils_queue.h"

#include "psm_log.h"
#include "psm_perf.h"

#define PSMI_LOCK_NO_OWNER	((pthread_t)(-1))

#define _PSMI_IN_USER_H

/* Opaque hw context pointer used in HAL,
   and defined by each HAL instance. */
typedef void *psmi_hal_hw_context;

#include "psm_help.h"
#include "psm_error.h"
#include "psm_nic_select.h"
#include "psm_context.h"
#include "psm_utils.h"
#include "psm_timer.h"
#include "psm_mpool.h"
#include "gpu/psm_gpu_hal.h"
#ifdef PSM_HAVE_REG_MR
#include "psm_verbs_mr.h"
#ifdef PSM_HAVE_RNDV_MOD
#include "psm_rndv_mod.h"
#endif
#endif
#include "psm_ep.h"
#include "psm_lock.h"
#include "psm_stats.h"
#include "psm2_mock_testing.h"

#undef _PSMI_IN_USER_H

#define PSMI_VERNO_MAKE(major, minor) ((((major)&0xff)<<8)|((minor)&0xff))
#define PSMI_VERNO  PSMI_VERNO_MAKE(PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR)
#define PSMI_VERNO_GET_MAJOR(verno) (((verno)>>8) & 0xff)
#define PSMI_VERNO_GET_MINOR(verno) (((verno)>>0) & 0xff)

extern unsigned int psm3_reg_mr_fail_limit;
extern unsigned int psm3_reg_mr_warn_cnt;

int psm3_verno_client();
int psm3_verno_isinteroperable(uint16_t verno);
int MOCKABLE(psm3_isinitialized)();
MOCK_DCL_EPILOGUE(psm3_isinitialized);

psm2_error_t psm3_poll_internal(psm2_ep_t ep, int poll_amsh, bool force);
psm2_error_t psm3_mq_wait_internal(psm2_mq_req_t *ireq);

int psm3_get_current_proc_location();
// return the largest possible numa ID of a CPU in this system
int psm3_get_max_cpu_numa();

extern int psm3_allow_routers;
extern psmi_lock_t psm3_creation_lock;
extern psm2_ep_t psm3_opened_endpoint;
extern int psm3_opened_endpoint_count;

extern int psm3_affinity_shared_file_opened;
extern uint64_t *psm3_shared_affinity_ptr;
extern uint64_t *psm3_shared_affinity_nic_refcount_ptr;
extern char *psm3_affinity_shm_name;

extern sem_t *psm3_sem_affinity_shm_rw;
extern int psm3_affinity_semaphore_open;
extern char *psm3_sem_affinity_shm_rw_name;
extern void psm3_wake(psm2_ep_t ep);	// wake from psm3_wait

/*
 * Following is the definition of various lock implementations. The choice is
 * made by defining specific lock type in relevant section of psm_config.h
 */
#ifdef PSMI_LOCK_IS_SPINLOCK
#define _PSMI_LOCK_INIT(pl)	psmi_spin_init(&((pl).lock))
#define _PSMI_LOCK_TRY(pl)	psmi_spin_trylock(&((pl).lock))
#define _PSMI_LOCK(pl)		psmi_spin_lock(&((pl).lock))
#define _PSMI_UNLOCK(pl)	psmi_spin_unlock(&((pl).lock))
#define _PSMI_LOCK_ASSERT(pl)
#define _PSMI_UNLOCK_ASSERT(pl)
#define PSMI_LOCK_DISABLED	0

#elif defined(PSMI_LOCK_IS_MUTEXLOCK_DEBUG)

PSMI_ALWAYS_INLINE(
int
_psmi_mutex_trylock_inner(pthread_mutex_t *mutex,
			  const char *curloc, pthread_t *lock_owner
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
			  , int check, const char **lock_owner_loc
#endif
			  ))
{
	psmi_assert_always_loc(*lock_owner != pthread_self(),
			       curloc);
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
	// this is imperfect as the owner's unlock can race with this function
	// so we fetch loc1 and loc2 just before and after our trylock.  Still
	// imperfect, but helps provide insight on frequently contended locks
	const char *loc1 = *lock_owner_loc;
#endif
	int ret = pthread_mutex_trylock(mutex);
	if (ret == 0) {
		*lock_owner = pthread_self();
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
		*lock_owner_loc = curloc;
	} else {
		const char *loc2 = *lock_owner_loc;
		if (check)
			_HFI_VDBG("%s is trying for lock held by %s %s\n", curloc, loc1, loc2);
#endif
	}
	return ret;
}

PSMI_ALWAYS_INLINE(
int
_psmi_mutex_lock_inner(pthread_mutex_t *mutex,
		       const char *curloc, pthread_t *lock_owner
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
		       , const char **lock_owner_loc
#endif
		       ))
{
	psmi_assert_always_loc(*lock_owner != pthread_self(),
			       curloc);
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
	// this is imperfect as the owner's unlock can race with this function
	// so we fetch loc1 and loc2 just before and after our trylock.  Still
	// imperfect, but helps provide insight on frequently contended locks
	const char *loc1 = *lock_owner_loc;
	if (! _psmi_mutex_trylock_inner(mutex, curloc, lock_owner, 0, lock_owner_loc))
		return 0;
	const char *loc2 = *lock_owner_loc;
	_HFI_VDBG("%s is waiting for lock held by %s %s\n", curloc, loc1, loc2);
#endif
	int ret = pthread_mutex_lock(mutex);
	psmi_assert_always_loc(ret != EDEADLK, curloc);
	*lock_owner = pthread_self();
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
	*lock_owner_loc = curloc;
#endif
	return ret;
}

PSMI_ALWAYS_INLINE(
void
_psmi_mutex_unlock_inner(pthread_mutex_t *mutex,
			 const char *curloc, pthread_t *lock_owner
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
			 , const char **lock_owner_loc
#endif
			 ))
{
	psmi_assert_always_loc(*lock_owner == pthread_self(),
			       curloc);
	*lock_owner = PSMI_LOCK_NO_OWNER;
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
	*lock_owner_loc = "NONE";
#endif
	psmi_assert_always_loc(pthread_mutex_unlock(mutex) !=
			       EPERM, curloc);
	return;
}

#define _PSMI_LOCK_INIT(pl)	/* static initialization */
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
#define _PSMI_LOCK_TRY(pl)							\
	    _psmi_mutex_trylock_inner(&((pl).lock), PSMI_CURLOC,		\
					&((pl).lock_owner), 1, &((pl).lock_owner_loc))
#define _PSMI_LOCK(pl)								\
	    _psmi_mutex_lock_inner(&((pl).lock), PSMI_CURLOC,			\
                    &((pl).lock_owner), &((pl).lock_owner_loc))
#define _PSMI_UNLOCK(pl)							\
	    _psmi_mutex_unlock_inner(&((pl).lock), PSMI_CURLOC,			\
                    &((pl).lock_owner), &((pl).lock_owner_loc))
#else
#define _PSMI_LOCK_TRY(pl)							\
	    _psmi_mutex_trylock_inner(&((pl).lock), PSMI_CURLOC,		\
					&((pl).lock_owner))
#define _PSMI_LOCK(pl)								\
	    _psmi_mutex_lock_inner(&((pl).lock), PSMI_CURLOC,			\
                                        &((pl).lock_owner))
#define _PSMI_UNLOCK(pl)							\
	    _psmi_mutex_unlock_inner(&((pl).lock), PSMI_CURLOC,			\
                                        &((pl).lock_owner))
#endif
#define _PSMI_LOCK_ASSERT(pl)							\
	psmi_assert_always((pl).lock_owner == pthread_self());
#define _PSMI_UNLOCK_ASSERT(pl)							\
	psmi_assert_always((pl).lock_owner != pthread_self());
#define PSMI_LOCK_DISABLED	0

#elif defined(PSMI_LOCK_IS_MUTEXLOCK)
#define _PSMI_LOCK_INIT(pl)	/* static initialization */
#define _PSMI_LOCK_TRY(pl)	pthread_mutex_trylock(&((pl).lock))
#define _PSMI_LOCK(pl)		pthread_mutex_lock(&((pl).lock))
#define _PSMI_UNLOCK(pl)	pthread_mutex_unlock(&((pl).lock))
#define PSMI_LOCK_DISABLED	0
#define _PSMI_LOCK_ASSERT(pl)
#define _PSMI_UNLOCK_ASSERT(pl)

#elif defined(PSMI_PLOCK_IS_NOLOCK)
#define _PSMI_LOCK_TRY(pl)	0	/* 0 *only* so progress thread never succeeds */
#define _PSMI_LOCK(pl)
#define _PSMI_UNLOCK(pl)
#define PSMI_LOCK_DISABLED	1
#define _PSMI_LOCK_ASSERT(pl)
#define _PSMI_UNLOCK_ASSERT(pl)
#else
#error No LOCK lock type declared
#endif

#define PSMI_YIELD(pl)							\
	do { _PSMI_UNLOCK((pl)); sched_yield(); _PSMI_LOCK((pl)); } while (0)

#ifdef PSM2_MOCK_TESTING
/* If this is a mocking tests build, all the operations on the locks
 * are routed through functions which may be mocked, if necessary.  */
void MOCKABLE(psmi_mockable_lock_init)(psmi_lock_t *pl);
MOCK_DCL_EPILOGUE(psmi_mockable_lock_init);

int MOCKABLE(psmi_mockable_lock_try)(psmi_lock_t *pl);
MOCK_DCL_EPILOGUE(psmi_mockable_lock_try);

void MOCKABLE(psmi_mockable_lock)(psmi_lock_t *pl);
MOCK_DCL_EPILOGUE(psmi_mockable_lock);

void MOCKABLE(psmi_mockable_unlock)(psmi_lock_t *pl);
MOCK_DCL_EPILOGUE(psmi_mockable_unlock);

void MOCKABLE(psmi_mockable_lock_assert)(psmi_lock_t *pl);
MOCK_DCL_EPILOGUE(psmi_mockable_lock_assert);

void MOCKABLE(psmi_mockable_unlock_assert)(psmi_lock_t *pl);
MOCK_DCL_EPILOGUE(psmi_mockable_unlock_assert);

#define PSMI_LOCK_INIT(pl)	psmi_mockable_lock_init(&(pl))
#define PSMI_LOCK_TRY(pl)	psmi_mockable_lock_try(&(pl))
#define PSMI_LOCK(pl)		psmi_mockable_lock(&(pl))
#define PSMI_UNLOCK(pl)		psmi_mockable_unlock(&(pl))
#define PSMI_LOCK_ASSERT(pl)	psmi_mockable_lock_assert(&(pl))
#define PSMI_UNLOCK_ASSERT(pl)	psmi_mockable_unlock_assert(&(pl))
#else
#define PSMI_LOCK_INIT(pl)	_PSMI_LOCK_INIT(pl)
#define PSMI_LOCK_TRY(pl)	_PSMI_LOCK_TRY(pl)
#define PSMI_LOCK(pl)		_PSMI_LOCK(pl)
#define PSMI_UNLOCK(pl)		_PSMI_UNLOCK(pl)
#define PSMI_LOCK_ASSERT(pl)	_PSMI_LOCK_ASSERT(pl)
#define PSMI_UNLOCK_ASSERT(pl)	_PSMI_UNLOCK_ASSERT(pl)
#endif

#ifdef PSM_PROFILE
void psmi_profile_block() __attribute__ ((weak));
void psmi_profile_unblock() __attribute__ ((weak));
void psmi_profile_reblock(int did_no_progress) __attribute__ ((weak));

#define PSMI_PROFILE_BLOCK()		psmi_profile_block()
#define PSMI_PROFILE_UNBLOCK()		psmi_profile_unblock()
#define PSMI_PROFILE_REBLOCK(noprog)	psmi_profile_reblock(noprog)
#else
#define PSMI_PROFILE_BLOCK()
#define PSMI_PROFILE_UNBLOCK()
#define PSMI_PROFILE_REBLOCK(noprog)
#endif

#define COMPILE_TIME_ASSERT(NAME,COND) extern char NAME[1/ COND]

#ifdef PSM_HAVE_GPU
// Host bounce buffers.  Used during pipelined GPU copies for
// large rendezvous IOs.
struct ips_gpu_hostbuf {
	STAILQ_ENTRY(ips_gpu_hostbuf) req_next;
	STAILQ_ENTRY(ips_gpu_hostbuf) next;
	uint32_t size, offset, bytes_read;
	/* This flag indicates whether a chb is
	 * pulled from a mpool or dynamically
	 * allocated using calloc. */
	uint8_t is_tempbuf;
	uint8_t pad1;
	uint16_t pad2;
	// aligned to 64 bit boundary
	union gpu_hostbuf_gpu_specific gpu_specific;
	psm2_mq_req_t req;
	void* host_buf;
	void* gpu_buf;
};

enum psm2_chb_match_type {
	/* Complete data found in a single chb */
	PSMI_GPU_FULL_MATCH_FOUND = 0,
	/* Data is spread across two chb's */
	PSMI_GPU_SPLIT_MATCH_FOUND = 1,
	/* Data is only partially prefetched */
	PSMI_GPU_PARTIAL_MATCH_FOUND = 2,
	PSMI_GPU_CONTINUE = 3
};
typedef enum psm2_chb_match_type psm2_chb_match_type_t;

void psmi_gpu_hostbuf_alloc_func(int is_alloc, void *context, void *obj);

#define GPU_HOSTBUFFER_LIMITS {				\
	    .env = "PSM3_GPU_BOUNCEBUFFERS_MAX",		\
	    .descr = "Max GPU Pipeline bounce buffers (in MB)",		\
	    .env_level = PSMI_ENVVAR_LEVEL_HIDDEN,		\
	    .minval = 1,					\
	    .maxval = 1<<30,					\
	    .mode[PSMI_MEMMODE_NORMAL]  = {  16, 256 },		\
	    .mode[PSMI_MEMMODE_MINIMAL] = {   1,   1 },		\
	    .mode[PSMI_MEMMODE_LARGE]   = {  32, 512 }		\
	}

struct ips_gpu_hostbuf_mpool_cb_context {
	unsigned bufsz;
};

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _PSMI_USER_H */
