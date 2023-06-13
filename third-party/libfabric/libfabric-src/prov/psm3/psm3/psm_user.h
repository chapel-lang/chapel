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

/* This indicates at least 1 HAL in the build can perform RDMA */
#ifdef PSM_VERBS
#define PSM_HAVE_RDMA
#endif
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
#if defined(PSM_VERBS) || (defined(PSM_SOCKETS) && (defined(PSM_CUDA) || defined(PSM_ONEAPI)))
#define PSM_HAVE_RNDV_MOD
#endif /* UD || (UDP & CUDA) */
#endif /* RNDV_MOD */


#include "psm_config.h"
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

#ifdef PSM_CUDA
#ifndef PSM_CUDA_MOCK
#include <cuda.h>
#include <driver_types.h>

#if CUDA_VERSION < 7000
#error Please update CUDA driver, required minimum version is 7.0
#endif
#else
// included in stand-alone unit test that does not use real CUDA functions
#include "psmi_cuda_mock.h"
#endif /* PSM_CUDA_MOCK */
#elif defined(PSM_ONEAPI)
#include <level_zero/ze_api.h>
#endif


#define PSMI_LOCK_NO_OWNER	((pthread_t)(-1))

#define _PSMI_IN_USER_H

/* Opaque hw context pointer used in HAL,
   and defined by each HAL instance. */
typedef void *psmi_hal_hw_context;

#include "psm_help.h"
#include "psm_error.h"
#include "psm_context.h"
#include "psm_utils.h"
#include "psm_timer.h"
#include "psm_mpool.h"
#ifdef PSM_HAVE_REG_MR
#include "psm_verbs_mr.h"
#ifdef RNDV_MOD
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

int psm3_verno_client();
int psm3_verno_isinteroperable(uint16_t verno);
int MOCKABLE(psm3_isinitialized)();
MOCK_DCL_EPILOGUE(psm3_isinitialized);

psm2_error_t psm3_poll_internal(psm2_ep_t ep, int poll_amsh, bool force);
psm2_error_t psm3_mq_wait_internal(psm2_mq_req_t *ireq);

int psm3_get_current_proc_location();

extern int psm3_allow_routers;
extern uint32_t non_dw_mul_sdma;
extern psmi_lock_t psm3_creation_lock;
extern psm2_ep_t psm3_opened_endpoint;
extern int psm3_opened_endpoint_count;

extern int psm3_affinity_shared_file_opened;
extern uint64_t *psm3_shared_affinity_ptr;
extern char *psm3_affinity_shm_name;

extern sem_t *psm3_sem_affinity_shm_rw;
extern int psm3_affinity_semaphore_open;
extern char *psm3_sem_affinity_shm_rw_name;

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
			  const char *curloc, pthread_t *lock_owner))
{
	psmi_assert_always_loc(*lock_owner != pthread_self(),
			       curloc);
	int ret = pthread_mutex_trylock(mutex);
	if (ret == 0)
		*lock_owner = pthread_self();
	return ret;
}

PSMI_ALWAYS_INLINE(
int
_psmi_mutex_lock_inner(pthread_mutex_t *mutex,
		       const char *curloc, pthread_t *lock_owner))
{
	psmi_assert_always_loc(*lock_owner != pthread_self(),
			       curloc);
	int ret = pthread_mutex_lock(mutex);
	psmi_assert_always_loc(ret != EDEADLK, curloc);
	*lock_owner = pthread_self();
	return ret;
}

PSMI_ALWAYS_INLINE(
void
_psmi_mutex_unlock_inner(pthread_mutex_t *mutex,
			 const char *curloc, pthread_t *lock_owner))
{
	psmi_assert_always_loc(*lock_owner == pthread_self(),
			       curloc);
	*lock_owner = PSMI_LOCK_NO_OWNER;
	psmi_assert_always_loc(pthread_mutex_unlock(mutex) !=
			       EPERM, curloc);
	return;
}

#define _PSMI_LOCK_INIT(pl)	/* static initialization */
#define _PSMI_LOCK_TRY(pl)							\
	    _psmi_mutex_trylock_inner(&((pl).lock), PSMI_CURLOC,		\
					&((pl).lock_owner))
#define _PSMI_LOCK(pl)								\
	    _psmi_mutex_lock_inner(&((pl).lock), PSMI_CURLOC,			\
                                        &((pl).lock_owner))
#define _PSMI_UNLOCK(pl)							\
	    _psmi_mutex_unlock_inner(&((pl).lock), PSMI_CURLOC,			\
                                        &((pl).lock_owner))
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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
extern int is_gdr_copy_enabled;
/* This limit dictates when the sender turns off
 * GDR Copy and uses SDMA. The limit needs to be less than equal
 * CUDA RNDV threshold.
 * set to 0 if GDR Copy disabled
 */
extern uint32_t gdr_copy_limit_send;
/* This limit dictates when the reciever turns off
 * GDR Copy. The limit needs to be less than equal
 * CUDA RNDV threshold.
 * set to 0 if GDR Copy disabled
 */
extern uint32_t gdr_copy_limit_recv;
extern int is_gpudirect_enabled; // only for use during parsing of other params
extern int _device_support_gpudirect;
extern uint32_t gpudirect_rdma_send_limit;
extern uint32_t gpudirect_rdma_recv_limit;
extern uint32_t cuda_thresh_rndv;

struct ips_gpu_hostbuf {
	STAILQ_ENTRY(ips_gpu_hostbuf) req_next;
	STAILQ_ENTRY(ips_gpu_hostbuf) next;
	uint32_t size, offset, bytes_read;
	/* This flag indicates whether a chb is
	 * pulled from a mpool or dynamically
	 * allocated using calloc. */
	uint8_t is_tempbuf;
#ifdef PSM_CUDA
	CUevent copy_status;
#elif defined(PSM_ONEAPI)
	ze_event_pool_handle_t event_pool;
	ze_command_list_handle_t command_list;
	ze_event_handle_t copy_status;
#endif
	psm2_mq_req_t req;
	void* host_buf;
	void* gpu_buf;
};
#endif

#ifdef PSM_CUDA

extern int is_cuda_enabled;
extern int _device_support_unified_addr;
extern int _gpu_p2p_supported;
extern int my_gpu_device;
extern int cuda_lib_version;

extern CUcontext cu_ctxt;
extern void *psmi_cuda_lib;
#endif // PSM_CUDA

#ifdef PSM_ONEAPI

#define MAX_ZE_DEVICES 8

extern int psm3_ze_dev_fds[MAX_ZE_DEVICES];
int psmi_oneapi_ze_initialize(void);
int psm3_ze_init_fds(void);
int *psm3_ze_get_dev_fds(int *nfds);
int psm3_ze_get_num_dev_fds(void);

extern int is_oneapi_ze_enabled;
extern int _gpu_p2p_supported;
extern int my_gpu_device;
extern int psm3_num_ze_dev_fds;

struct ze_dev_ctxt {
	ze_device_handle_t dev;
	ze_command_queue_handle_t cq;
	ze_command_list_handle_t cl;
};

extern ze_context_handle_t ze_context;
extern ze_driver_handle_t ze_driver;
extern struct ze_dev_ctxt ze_devices[MAX_ZE_DEVICES];
extern int num_ze_devices;
extern struct ze_dev_ctxt *cur_ze_dev;

const char* psmi_oneapi_ze_result_to_string(const ze_result_t result);
psm2_error_t psm3_sock_detach(ptl_t *ptl_gen);
psm2_error_t psm3_ze_init_ipc_socket(ptl_t *ptl_gen);
psm2_error_t psm3_receive_ze_dev_fds(ptl_t *ptl_gen);
psm2_error_t psm3_send_dev_fds(ptl_t *ptl_gen, psm2_epaddr_t epaddr);
psm2_error_t psm3_check_dev_fds_exchanged(ptl_t *ptl_gen);
int psm3_get_peer_fds_index(psm2_ep_t ep, psm2_epid_t peer_epid);
int psm3_create_peer_fds_entry(psm2_ep_t ep, psm2_epid_t peer_epid);

void psmi_oneapi_ze_memcpy(void *dstptr, const void *srcptr, size_t size);

static inline
int device_support_gpudirect()
{
	if (likely(_device_support_gpudirect > -1)) return _device_support_gpudirect;

	/* Is there any device property that can indicate this? */
	_device_support_gpudirect = 1;
	return _device_support_gpudirect;
}

#endif // PSM_ONEAPI

#ifdef PSM_CUDA
extern CUresult (*psmi_cuInit)(unsigned int  Flags );
extern CUresult (*psmi_cuCtxDetach)(CUcontext c);
extern CUresult (*psmi_cuCtxGetCurrent)(CUcontext *c);
extern CUresult (*psmi_cuCtxSetCurrent)(CUcontext c);
extern CUresult (*psmi_cuPointerGetAttribute)(void *data, CUpointer_attribute pa, CUdeviceptr p);
extern CUresult (*psmi_cuPointerSetAttribute)(void *data, CUpointer_attribute pa, CUdeviceptr p);
extern CUresult (*psmi_cuDeviceCanAccessPeer)(int *canAccessPeer, CUdevice dev, CUdevice peerDev);
extern CUresult (*psmi_cuDeviceGet)(CUdevice* device, int  ordinal);
extern CUresult (*psmi_cuDeviceGetAttribute)(int* pi, CUdevice_attribute attrib, CUdevice dev);
extern CUresult (*psmi_cuDriverGetVersion)(int* driverVersion);
extern CUresult (*psmi_cuDeviceGetCount)(int* count);
extern CUresult (*psmi_cuStreamCreate)(CUstream* phStream, unsigned int Flags);
extern CUresult (*psmi_cuStreamDestroy)(CUstream phStream);
extern CUresult (*psmi_cuStreamSynchronize)(CUstream phStream);
extern CUresult (*psmi_cuEventCreate)(CUevent* phEvent, unsigned int Flags);
extern CUresult (*psmi_cuEventDestroy)(CUevent hEvent);
extern CUresult (*psmi_cuEventQuery)(CUevent hEvent);
extern CUresult (*psmi_cuEventRecord)(CUevent hEvent, CUstream hStream);
extern CUresult (*psmi_cuEventSynchronize)(CUevent hEvent);
extern CUresult (*psmi_cuMemHostAlloc)(void** pp, size_t bytesize, unsigned int Flags);
extern CUresult (*psmi_cuMemFreeHost)(void* p);
extern CUresult (*psmi_cuMemcpy)(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount);
extern CUresult (*psmi_cuMemcpyDtoD)(CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount);
extern CUresult (*psmi_cuMemcpyDtoH)(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
extern CUresult (*psmi_cuMemcpyHtoD)(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
extern CUresult (*psmi_cuMemcpyDtoHAsync)(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
extern CUresult (*psmi_cuMemcpyHtoDAsync)(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
extern CUresult (*psmi_cuIpcGetMemHandle)(CUipcMemHandle* pHandle, CUdeviceptr dptr);
extern CUresult (*psmi_cuIpcOpenMemHandle)(CUdeviceptr* pdptr, CUipcMemHandle handle, unsigned int Flags);
extern CUresult (*psmi_cuIpcCloseMemHandle)(CUdeviceptr dptr);
extern CUresult (*psmi_cuMemGetAddressRange)(CUdeviceptr* pbase, size_t* psize, CUdeviceptr dptr);
extern CUresult (*psmi_cuDevicePrimaryCtxGetState)(CUdevice dev, unsigned int* flags, int* active);
extern CUresult (*psmi_cuDevicePrimaryCtxRetain)(CUcontext* pctx, CUdevice dev);
extern CUresult (*psmi_cuCtxGetDevice)(CUdevice* device);
extern CUresult (*psmi_cuDevicePrimaryCtxRelease)(CUdevice device);
extern CUresult (*psmi_cuGetErrorString)(CUresult error, const char **pStr);
#endif // PSM_CUDA

#ifdef PSM_ONEAPI
extern ze_result_t (*psmi_zeInit)(ze_init_flags_t flags);
extern ze_result_t (*psmi_zeDriverGet)(uint32_t *pCount, ze_driver_handle_t *phDrivers);
extern ze_result_t (*psmi_zeDeviceGet)(ze_driver_handle_t hDriver, uint32_t *pCount, ze_device_handle_t *phDevices);
extern ze_result_t (*psmi_zeContextCreate)(ze_driver_handle_t hDriver, const ze_context_desc_t *desc, ze_context_handle_t *phContext);
extern ze_result_t (*psmi_zeContextDestroy)(ze_context_handle_t hContext);
extern ze_result_t (*psmi_zeCommandQueueCreate)(ze_context_handle_t hContext, ze_device_handle_t hDevice,const ze_command_queue_desc_t *desc, ze_command_queue_handle_t *phCommandQueue);
extern ze_result_t (*psmi_zeCommandQueueDestroy)(ze_command_queue_handle_t hCommandQueue);
extern ze_result_t (*psmi_zeCommandQueueExecuteCommandLists)(ze_command_queue_handle_t hCommandQueue, uint32_t numCommandLists, ze_command_list_handle_t *phCommandLists, ze_fence_handle_t hFence);
extern ze_result_t (*psmi_zeCommandQueueSynchronize)(ze_command_queue_handle_t hCommandQueue, uint64_t timeout);
extern ze_result_t (*psmi_zeCommandListCreate)(ze_context_handle_t hContext, ze_device_handle_t hDevice, const ze_command_list_desc_t *desc, ze_command_list_handle_t *phCommandList);
extern ze_result_t (*psmi_zeCommandListDestroy)(ze_command_list_handle_t hCommandList);
extern ze_result_t (*psmi_zeCommandListClose)(ze_command_list_handle_t hCommandList);
extern ze_result_t (*psmi_zeCommandListReset)(ze_command_list_handle_t hCommandList);
extern ze_result_t (*psmi_zeCommandListCreateImmediate)(ze_device_handle_t hDevice, const ze_command_queue_desc_t * altdesc, ze_command_list_handle_t * phCommandList);
extern ze_result_t (*psmi_zeCommandListAppendMemoryCopy)(ze_command_list_handle_t hCommandList, void *dstptr, const void *srcptr, size_t size, ze_event_handle_t hSignalEvent, uint32_t numWaitEvents, ze_event_handle_t *phWaitEvents);
extern ze_result_t (*psmi_zeCommandListAppendSignalEvent)(ze_command_list_handle_t hCommandList, ze_event_handle_t hEvent);
extern ze_result_t (*psmi_zeDeviceCanAccessPeer)(ze_device_handle_t hDevice, ze_device_handle_t hPeerDevice, ze_bool_t *value);
extern ze_result_t (*psmi_zeDeviceGetCommandQueueGroupProperties)(ze_device_handle_t hDevice, uint32_t *pCount, ze_command_queue_group_properties_t *pCommandQueueGroupProperties);
extern ze_result_t (*psmi_zeMemAllocHost)(ze_context_handle_t hContext, const ze_host_mem_alloc_desc_t *host_desc, size_t size, size_t alignment, void **pptr);
extern ze_result_t (*psmi_zeMemFree)(ze_context_handle_t hContext, void *ptr);
extern ze_result_t (*psmi_zeMemGetIpcHandle)(ze_context_handle_t hContext, const void *ptr, ze_ipc_mem_handle_t *pIpcHandle);
extern ze_result_t (*psmi_zeMemOpenIpcHandle)(ze_context_handle_t hContext,ze_device_handle_t hDevice, ze_ipc_mem_handle_t handle, ze_ipc_memory_flags_t flags, void **pptr);
extern ze_result_t (*psmi_zeMemCloseIpcHandle)(ze_context_handle_t hContext, const void *ptr);
extern ze_result_t (*psmi_zeMemGetAddressRange)(ze_context_handle_t hContext, const void *ptr, void **pBase, size_t *pSize);
extern ze_result_t (*psmi_zeMemGetAllocProperties)(ze_context_handle_t hContext, const void *ptr, ze_memory_allocation_properties_t *pMemAllocProperties, ze_device_handle_t *phDevice);
extern ze_result_t (*psmi_zeEventPoolCreate)(ze_context_handle_t hContext, const ze_event_pool_desc_t *desc, uint32_t numDevices, ze_device_handle_t *phDevices, ze_event_pool_handle_t *phEventPool);
extern ze_result_t (*psmi_zeEventPoolDestroy)(ze_event_pool_handle_t hEventPool);
extern ze_result_t (*psmi_zeEventCreate)(ze_event_pool_handle_t hEventPool, const ze_event_desc_t *desc, ze_event_handle_t *phEvent);
extern ze_result_t (*psmi_zeEventDestroy)(ze_event_handle_t hEvent);
extern ze_result_t (*psmi_zeEventQueryStatus)(ze_event_handle_t hEvent);
extern ze_result_t (*psmi_zeEventHostSynchronize)(ze_event_handle_t hEvent, uint64_t timeout);
extern ze_result_t (*psmi_zeEventHostReset)(ze_event_handle_t hEvent);

#endif // PSM_ONEAPI

#ifdef PSM_CUDA
extern uint64_t psmi_count_cuInit;
extern uint64_t psmi_count_cuCtxDetach;
extern uint64_t psmi_count_cuCtxGetCurrent;
extern uint64_t psmi_count_cuCtxSetCurrent;
extern uint64_t psmi_count_cuPointerGetAttribute;
extern uint64_t psmi_count_cuPointerSetAttribute;
extern uint64_t psmi_count_cuDeviceCanAccessPeer;
extern uint64_t psmi_count_cuDeviceGet;
extern uint64_t psmi_count_cuDeviceGetAttribute;
extern uint64_t psmi_count_cuDriverGetVersion;
extern uint64_t psmi_count_cuDeviceGetCount;
extern uint64_t psmi_count_cuStreamCreate;
extern uint64_t psmi_count_cuStreamDestroy;
extern uint64_t psmi_count_cuStreamSynchronize;
extern uint64_t psmi_count_cuEventCreate;
extern uint64_t psmi_count_cuEventDestroy;
extern uint64_t psmi_count_cuEventQuery;
extern uint64_t psmi_count_cuEventRecord;
extern uint64_t psmi_count_cuEventSynchronize;
extern uint64_t psmi_count_cuMemHostAlloc;
extern uint64_t psmi_count_cuMemFreeHost;
extern uint64_t psmi_count_cuMemcpy;
extern uint64_t psmi_count_cuMemcpyDtoD;
extern uint64_t psmi_count_cuMemcpyDtoH;
extern uint64_t psmi_count_cuMemcpyHtoD;
extern uint64_t psmi_count_cuMemcpyDtoHAsync;
extern uint64_t psmi_count_cuMemcpyHtoDAsync;
extern uint64_t psmi_count_cuIpcGetMemHandle;
extern uint64_t psmi_count_cuIpcOpenMemHandle;
extern uint64_t psmi_count_cuIpcCloseMemHandle;
extern uint64_t psmi_count_cuMemGetAddressRange;
extern uint64_t psmi_count_cuDevicePrimaryCtxGetState;
extern uint64_t psmi_count_cuDevicePrimaryCtxRetain;
extern uint64_t psmi_count_cuCtxGetDevice;
extern uint64_t psmi_count_cuDevicePrimaryCtxRelease;
extern uint64_t psmi_count_cuGetErrorString;
#endif // PSM_CUDA

#ifdef PSM_ONEAPI
extern uint64_t psmi_count_zeInit;
extern uint64_t psmi_count_zeDriverGet;
extern uint64_t psmi_count_zeDeviceGet;
extern uint64_t psmi_count_zeContextCreate;
extern uint64_t psmi_count_zeContextDestroy;
extern uint64_t psmi_count_zeCommandQueueCreate;
extern uint64_t psmi_count_zeCommandQueueDestroy;
extern uint64_t psmi_count_zeCommandQueueExecuteCommandLists;
extern uint64_t psmi_count_zeCommandQueueSynchronize;
extern uint64_t psmi_count_zeCommandListCreate;
extern uint64_t psmi_count_zeCommandListDestroy;
extern uint64_t psmi_count_zeCommandListClose;
extern uint64_t psmi_count_zeCommandListReset;
extern uint64_t psmi_count_zeCommandListCreateImmediate;
extern uint64_t psmi_count_zeCommandListAppendMemoryCopy;
extern uint64_t psmi_count_zeCommandListAppendSignalEvent;
extern uint64_t psmi_count_zeDeviceCanAccessPeer;
extern uint64_t psmi_count_zeDeviceGetCommandQueueGroupProperties;
extern uint64_t psmi_count_zeMemAllocHost;
extern uint64_t psmi_count_zeMemFree;
extern uint64_t psmi_count_zeMemGetIpcHandle;
extern uint64_t psmi_count_zeMemOpenIpcHandle;
extern uint64_t psmi_count_zeMemCloseIpcHandle;
extern uint64_t psmi_count_zeMemGetAddressRange;
extern uint64_t psmi_count_zeMemGetAllocProperties;
extern uint64_t psmi_count_zeEventPoolCreate;
extern uint64_t psmi_count_zeEventPoolDestroy;
extern uint64_t psmi_count_zeEventCreate;
extern uint64_t psmi_count_zeEventDestroy;
extern uint64_t psmi_count_zeEventQueryStatus;
extern uint64_t psmi_count_zeEventHostSynchronize;
extern uint64_t psmi_count_zeEventHostReset;
#endif // PSM_ONEAPI

#ifdef PSM_CUDA
static int check_set_cuda_ctxt(void)
{
	CUresult err;
	CUcontext tmpctxt = {0};

	if (unlikely(!psmi_cuCtxGetCurrent || !psmi_cuCtxSetCurrent))
		return 0;

	err = psmi_cuCtxGetCurrent(&tmpctxt);
	if (likely(!err)) {
		if (unlikely(!tmpctxt && cu_ctxt)) {
			err = psmi_cuCtxSetCurrent(cu_ctxt);
			return !!err;
		} else if (unlikely(tmpctxt && !cu_ctxt)) {
			cu_ctxt = tmpctxt;
		}
	}
	return 0;
}


#define PSMI_CUDA_CALL(func, args...) do {				\
		CUresult cudaerr;					\
		if (unlikely(check_set_cuda_ctxt())) {			\
			psm3_handle_error(PSMI_EP_NORETURN,		\
			PSM2_INTERNAL_ERR, "Failed to set/synchronize"	\
			" CUDA context.\n");				\
		}							\
		psmi_count_##func++;					\
		cudaerr = psmi_##func(args);				\
		if (cudaerr != CUDA_SUCCESS) {				\
			const char *pStr = NULL;			\
			psmi_count_cuGetErrorString++;			\
			psmi_cuGetErrorString(cudaerr, &pStr);		\
			_HFI_ERROR(					\
				"CUDA failure: %s() (at %s:%d)"		\
				"returned %d: %s\n",			\
				#func, __FILE__, __LINE__, cudaerr,	\
				pStr?pStr:"Unknown");			\
			psm3_handle_error(				\
				PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function.\n");\
		}							\
	} while (0)
#endif // PSM_CUDA

#ifdef PSM_ONEAPI

#define PSMI_ONEAPI_ZE_CALL(func, args...) do { \
	ze_result_t result; \
	psmi_count_##func++; \
	result = psmi_##func(args);	\
	if(result != ZE_RESULT_SUCCESS) { \
		_HFI_ERROR( "OneAPI Level Zero failure: %s() (at %s:%d) " \
			"returned %d(%s)\n", \
			#func, __FILE__, __LINE__, result, psmi_oneapi_ze_result_to_string(result)); \
		psm3_handle_error( PSMI_EP_NORETURN, \
			PSM2_INTERNAL_ERR, \
			"Error returned from OneAPI Level Zero function %s.\n", STRINGIFY(func)); \
	} \
} while (0)

void psmi_oneapi_cmd_create_all(void);
void psmi_oneapi_cmd_destroy_all(void);

PSMI_ALWAYS_INLINE(
struct ze_dev_ctxt *
psmi_oneapi_dev_ctxt_get(const void *ptr))
{
	ze_memory_allocation_properties_t mem_props = {
		ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES
	};
	ze_device_handle_t dev;
	ze_result_t result;
	struct ze_dev_ctxt *ret = NULL;
	int i;

	psmi_count_zeMemGetAllocProperties++;
	result = psmi_zeMemGetAllocProperties(ze_context, ptr, &mem_props,
					      &dev);
	if (result == ZE_RESULT_SUCCESS &&
	    mem_props.type == ZE_MEMORY_TYPE_DEVICE) {
		for (i = 0; i < num_ze_devices; i++) {
			if (ze_devices[i].dev == dev) {
				ret = &ze_devices[i];
				break;
			}
		}
	}

	return ret;
}

PSMI_ALWAYS_INLINE(
int
_psmi_is_oneapi_ze_mem(const void *ptr))
{
	ze_memory_allocation_properties_t mem_props = {
		ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES
	};
	ze_device_handle_t dev;
	ze_result_t result;
	int ret = 0;

	psmi_count_zeMemGetAllocProperties++;
	result = psmi_zeMemGetAllocProperties(ze_context, ptr, &mem_props,
					      &dev);
	if (result == ZE_RESULT_SUCCESS &&
	    mem_props.type == ZE_MEMORY_TYPE_DEVICE) {
		ret = 1;
		_HFI_VDBG("dev %p ze_device %p\n", dev, cur_ze_dev->dev);
		/* Check if the gpu device has changed. */
		if (dev != cur_ze_dev->dev) {
			int i;

			for (i = 0; i < num_ze_devices; i++) {
				if (ze_devices[i].dev == dev) {
					cur_ze_dev = &ze_devices[i];
					break;
				}
			}
		}
	}

	return ret;
}

#define PSMI_IS_ONEAPI_ZE_ENABLED likely(is_oneapi_ze_enabled)
#define PSMI_IS_ONEAPI_ZE_DISABLED unlikely(!is_oneapi_ze_enabled)
#define PSMI_IS_ONEAPI_ZE_MEM(ptr) _psmi_is_oneapi_ze_mem(ptr)

#endif // PSM_ONEAPI

#ifdef PSM_CUDA
PSMI_ALWAYS_INLINE(
void verify_device_support_unified_addr())
{
	if (likely(_device_support_unified_addr > -1)) return;

	int num_devices, dev;

	/* Check if all devices support Unified Virtual Addressing. */
	PSMI_CUDA_CALL(cuDeviceGetCount, &num_devices);

	_device_support_unified_addr = 1;

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		PSMI_CUDA_CALL(cuDeviceGet, &device, dev);
		int unifiedAddressing;
		PSMI_CUDA_CALL(cuDeviceGetAttribute,
				&unifiedAddressing,
				CU_DEVICE_ATTRIBUTE_UNIFIED_ADDRESSING,
				device);

		if (unifiedAddressing !=1) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_EP_DEVICE_FAILURE,
				"CUDA device %d does not support Unified Virtual Addressing.\n",
				dev);
		}
	}

	return;
}

PSMI_ALWAYS_INLINE(
int device_support_gpudirect())
{
	if (likely(_device_support_gpudirect > -1)) return _device_support_gpudirect;

	int num_devices, dev;

	/* Check if all devices support GPU Direct RDMA based on version. */
	PSMI_CUDA_CALL(cuDeviceGetCount, &num_devices);

	_device_support_gpudirect = 1;

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		PSMI_CUDA_CALL(cuDeviceGet, &device, dev);

		int major;
		PSMI_CUDA_CALL(cuDeviceGetAttribute,
				&major,
				CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR,
				device);
		if (major < 3) {
			_device_support_gpudirect = 0;
			_HFI_INFO("CUDA device %d does not support GPUDirect RDMA (Non-fatal error)\n", dev);
		}
	}

	return _device_support_gpudirect;
}

PSMI_ALWAYS_INLINE(
int gpu_p2p_supported())
{
	if (likely(_gpu_p2p_supported > -1)) return _gpu_p2p_supported;

	if (unlikely(!is_cuda_enabled)) {
		_gpu_p2p_supported=0;
		return 0;
	}

	int num_devices, dev;
	CUcontext c;

	/* Check which devices the current device has p2p access to. */
	CUdevice current_device;
	PSMI_CUDA_CALL(cuDeviceGetCount, &num_devices);
	_gpu_p2p_supported = 0;

	if (num_devices > 1) {
		PSMI_CUDA_CALL(cuCtxGetCurrent, &c);
		if (c == NULL) {
			_HFI_INFO("Unable to find active CUDA context, assuming P2P not supported\n");
			return 0;
		}
		PSMI_CUDA_CALL(cuCtxGetDevice, &current_device);
	}

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		PSMI_CUDA_CALL(cuDeviceGet, &device, dev);

		if (num_devices > 1 && device != current_device) {
			int canAccessPeer = 0;
			PSMI_CUDA_CALL(cuDeviceCanAccessPeer, &canAccessPeer,
					current_device, device);

			if (canAccessPeer != 1)
				_HFI_DBG("CUDA device %d does not support P2P from current device (Non-fatal error)\n", dev);
			else
				_gpu_p2p_supported |= (1 << device);
		} else {
			/* Always support p2p on the same GPU */
			my_gpu_device = device;
			_gpu_p2p_supported |= (1 << device);
		}
	}

	return _gpu_p2p_supported;
}

/**
 * Similar to PSMI_CUDA_CALL() except does not error out
 * if func(args) returns CUDA_SUCCESS or except_err
 *
 * Invoker must provide 'CUresult cudaerr' in invoked scope
 * so invoker can inspect whether cudaerr == CUDA_SUCCESS or
 * cudaerr == except_err after expanded code is executed.
 *
 * As except_err is an allowed value, message is printed at
 * DBG level.
 */
#define PSMI_CUDA_CALL_EXCEPT(except_err, func, args...) do {		\
		if (unlikely(check_set_cuda_ctxt())) {			\
			psm3_handle_error(PSMI_EP_NORETURN,		\
				PSM2_INTERNAL_ERR, "Failed to "		\
				"set/synchronize CUDA context.\n");	\
		}							\
		psmi_count_##func++;					\
		cudaerr = psmi_##func(args);				\
		if (cudaerr != CUDA_SUCCESS && cudaerr != except_err) {	\
			const char *pStr = NULL;			\
			psmi_count_cuGetErrorString++;			\
			psmi_cuGetErrorString(cudaerr, &pStr);		\
			if (cu_ctxt == NULL)				\
				_HFI_ERROR(				\
				"Check if CUDA is initialized"	\
				"before psm3_ep_open call \n");		\
			_HFI_ERROR(					\
				"CUDA failure: %s() (at %s:%d)"		\
				"returned %d: %s\n",			\
				#func, __FILE__, __LINE__, cudaerr,	\
				pStr?pStr:"Unknown");			\
			psm3_handle_error(				\
				PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function.\n");\
		} else if (cudaerr == except_err) { \
			const char *pStr = NULL;			\
			psmi_count_cuGetErrorString++;			\
			psmi_cuGetErrorString(cudaerr, &pStr);		\
			_HFI_DBG( \
				"CUDA non-zero return value: %s() (at %s:%d)"		\
				"returned %d: %s\n",			\
				#func, __FILE__, __LINE__, cudaerr,	\
				pStr?pStr:"Unknown");			\
		} \
	} while (0)

#define PSMI_CUDA_CHECK_EVENT(event, cudaerr) do {			\
		psmi_count_cuEventQuery++;				\
		cudaerr = psmi_cuEventQuery(event);			\
		if ((cudaerr != CUDA_SUCCESS) &&			\
		    (cudaerr != CUDA_ERROR_NOT_READY)) {		\
			const char *pStr = NULL;			\
			psmi_count_cuGetErrorString++;			\
			psmi_cuGetErrorString(cudaerr, &pStr);		\
			_HFI_ERROR(					\
				"CUDA failure: %s() returned %d: %s\n",	\
				"cuEventQuery", cudaerr,		\
				pStr?pStr:"Unknown");			\
			psm3_handle_error(				\
				PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function.\n");\
		}							\
	} while (0)

#define PSMI_CUDA_DLSYM(psmi_cuda_lib,func) do {                        \
	psmi_##func = dlsym(psmi_cuda_lib, STRINGIFY(func));            \
	if (!psmi_##func) {               				\
		psm3_handle_error(PSMI_EP_NORETURN,                     \
			       PSM2_INTERNAL_ERR,                       \
			       " Unable to resolve %s symbol"		\
			       " in CUDA libraries.\n",STRINGIFY(func));\
	}                                                               \
} while (0)
#endif // PSM_CUDA

#ifdef PSM_ONEAPI

PSMI_ALWAYS_INLINE(
int gpu_p2p_supported())
{

	uint32_t num_devices = 0;
	uint32_t dev;
	ze_device_handle_t devices[MAX_ZE_DEVICES];

	if (likely(_gpu_p2p_supported > -1)) return _gpu_p2p_supported;

	if (unlikely(!is_oneapi_ze_enabled)) {
		_gpu_p2p_supported=0;
		return 0;
	}

	_gpu_p2p_supported = 0;

	PSMI_ONEAPI_ZE_CALL(zeDeviceGet, ze_driver, &num_devices, NULL);
	if (num_devices > MAX_ZE_DEVICES)
		num_devices = MAX_ZE_DEVICES;
	PSMI_ONEAPI_ZE_CALL(zeDeviceGet, ze_driver, &num_devices, devices);

	for (dev = 0; dev < num_devices; dev++) {
		ze_device_handle_t device;
		device = devices[dev];

		if (num_devices > 1 && device != cur_ze_dev->dev) {
			ze_bool_t canAccessPeer = 0;

			PSMI_ONEAPI_ZE_CALL(zeDeviceCanAccessPeer, cur_ze_dev->dev,
					device, &canAccessPeer);
			if (canAccessPeer != 1)
				_HFI_DBG("ONEAPI device %d does not support P2P from current device (Non-fatal error)\n", dev);
			else
				_gpu_p2p_supported |= (1 << dev);
		} else {
			/* Always support p2p on the same GPU */
			my_gpu_device = dev;
			_gpu_p2p_supported |= (1 << dev);
		}
	}

	return _gpu_p2p_supported;
}

#define PSMI_ONEAPI_ZE_DLSYM(lib_ptr, func) do { \
	psmi_##func = dlsym(lib_ptr, STRINGIFY(func)); \
	if (!psmi_##func) { \
		psm3_handle_error(PSMI_EP_NORETURN, \
			PSM2_INTERNAL_ERR, \
			"Unable to resolve %s symbol " \
			"in OneAPI Level Zero library.\n", STRINGIFY(func)); \
	} \
} while (0)

static inline
int _psm3_oneapi_ze_memcpy_done(const struct ips_gpu_hostbuf *ghb)
{
	ze_result_t result;
	psmi_count_zeEventQueryStatus++;

	result = psmi_zeEventQueryStatus(ghb->copy_status);
	if (result == ZE_RESULT_SUCCESS) {
		return 1;
	} else if (result == ZE_RESULT_NOT_READY) {
		return 0;
	} else {
		_HFI_ERROR( "OneAPI LZ failure: %s() returned %d(%s)\n",
			__FUNCTION__, result,
			psmi_oneapi_ze_result_to_string(result));
		psm3_handle_error( PSMI_EP_NORETURN,
			PSM2_INTERNAL_ERR,
			"Error returned from OneAPI LZ function %s.\n",
			__FUNCTION__);
	}
	return 0;
}

#endif // PSM_ONEAPI

#ifdef PSM_CUDA
PSMI_ALWAYS_INLINE(
int
_psmi_is_cuda_mem(const void *ptr))
{
	CUresult cres;
	CUmemorytype mt;
	unsigned uvm = 0;
	psmi_count_cuPointerGetAttribute++;
	cres = psmi_cuPointerGetAttribute(
		&mt, CU_POINTER_ATTRIBUTE_MEMORY_TYPE, (CUdeviceptr) ptr);
	if ((cres == CUDA_SUCCESS) && (mt == CU_MEMORYTYPE_DEVICE)) {
		psmi_count_cuPointerGetAttribute++;
		cres = psmi_cuPointerGetAttribute(
			&uvm, CU_POINTER_ATTRIBUTE_IS_MANAGED, (CUdeviceptr) ptr);
		if ((cres == CUDA_SUCCESS) && (uvm == 0))
			return 1;
		else
			return 0;
	} else
		return 0;
}

#define PSMI_IS_CUDA_ENABLED  likely(is_cuda_enabled)
#define PSMI_IS_CUDA_DISABLED unlikely(!is_cuda_enabled)
#define PSMI_IS_CUDA_MEM(p) _psmi_is_cuda_mem(p)
extern void psm2_get_gpu_bars(void);

/*
 * CUDA documentation dictates the use of SYNC_MEMOPS attribute
 * when the buffer pointer received into PSM has been allocated
 * by the application. This guarantees that all memory operations
 * to this region of memory (used by multiple layers of the stack)
 * always synchronize.
 */
static inline
void psmi_cuda_set_attr_sync_memops(const void *ubuf)
{
	int true_flag = 1;

	PSMI_CUDA_CALL(cuPointerSetAttribute, &true_flag,
		       CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, (CUdeviceptr) ubuf);
}

static inline
int _psm3_cuda_memcpy_done(const struct ips_gpu_hostbuf *chb)
{
	CUresult status;
	PSMI_CUDA_CHECK_EVENT(chb->copy_status, status);
	return (status == CUDA_SUCCESS);
}

#endif /* PSM_CUDA */

#define COMPILE_TIME_ASSERT(NAME,COND) extern char NAME[1/ COND]

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)

extern uint64_t psm3_gpu_cache_evict;

enum psm2_chb_match_type {
	/* Complete data found in a single chb */
	PSMI_CUDA_FULL_MATCH_FOUND = 0,
	/* Data is spread across two chb's */
	PSMI_CUDA_SPLIT_MATCH_FOUND = 1,
	/* Data is only partially prefetched */
	PSMI_CUDA_PARTIAL_MATCH_FOUND = 2,
	PSMI_CUDA_CONTINUE = 3
};
typedef enum psm2_chb_match_type psm2_chb_match_type_t;

void psmi_gpu_hostbuf_alloc_func(int is_alloc, void *context, void *obj);

#define GPU_HOSTBUFFER_LIMITS {				\
	    .env = "PSM3_CUDA_BOUNCEBUFFERS_MAX",		\
	    .descr = "Max CUDA bounce buffers (in MB)",		\
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

PSMI_ALWAYS_INLINE(
int
_psmi_is_gdr_copy_enabled())
{
        return is_gdr_copy_enabled;
}

// Only valid if called for a GPU buffer
#define PSMI_USE_GDR_COPY_RECV(len) ((len) >=1 && (len) <= gdr_copy_limit_recv)
#define PSMI_IS_GDR_COPY_ENABLED _psmi_is_gdr_copy_enabled()
#define PSM3_IS_BUFFER_GPU_MEM(buf, len) \
    ((len) && PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(buf))
#endif

#ifdef PSM_CUDA

#define PSM3_GPU_PREPARE_HTOD_MEMCPYS(protoexp)                       \
	do {                                                          \
		protoexp->cudastream_recv = NULL;                     \
	} while (0)
#define PSM3_GPU_PREPARE_DTOH_MEMCPYS(proto)                          \
	do {                                                          \
		proto->cudastream_send = NULL;                        \
	} while (0)
#define PSM3_GPU_SHUTDOWN_HTOD_MEMCPYS(protoexp)                      \
	do {                                                          \
		if (protoexp->cudastream_recv != NULL) {              \
			PSMI_CUDA_CALL(cuStreamDestroy,               \
				protoexp->cudastream_recv);           \
		}                                                     \
	} while (0)
#define PSM3_GPU_SHUTDOWN_DTOH_MEMCPYS(proto)                         \
	do {                                                          \
		if (proto->cudastream_send) {                         \
			PSMI_CUDA_CALL(cuStreamDestroy,               \
				proto->cudastream_send);              \
		}                                                     \
	} while (0)
#define PSM3_GPU_MEMCPY_HTOD_START(protoexp, ghb, len)                \
	do {                                                          \
		if (protoexp->cudastream_recv == NULL) {              \
			PSMI_CUDA_CALL(cuStreamCreate,                \
				&protoexp->cudastream_recv,           \
				CU_STREAM_NON_BLOCKING);              \
		}                                                     \
		PSMI_CUDA_CALL(cuMemcpyHtoDAsync,                     \
			(CUdeviceptr)ghb->gpu_buf, ghb->host_buf,     \
			len, protoexp->cudastream_recv);              \
		if (ghb->copy_status == NULL) {                       \
			PSMI_CUDA_CALL(cuEventCreate,                 \
				&ghb->copy_status, CU_EVENT_DEFAULT); \
		}                                                     \
		PSMI_CUDA_CALL(cuEventRecord, ghb->copy_status,       \
			protoexp->cudastream_recv);                   \
	} while (0)
#define PSM3_GPU_MEMCPY_DTOH_START(proto, ghb, len, bufsz)            \
	do {                                                          \
		if (proto->cudastream_send == NULL) {                 \
			PSMI_CUDA_CALL(cuStreamCreate,                \
				&proto->cudastream_send,              \
				CU_STREAM_NON_BLOCKING);              \
		}                                                     \
		if (ghb->host_buf == NULL && bufsz) {                 \
			PSM3_GPU_HOST_ALLOC(&ghb->host_buf, bufsz);   \
		}                                                     \
		if (ghb->copy_status == NULL) {                       \
			PSMI_CUDA_CALL(cuEventCreate,                 \
				&ghb->copy_status, CU_EVENT_DEFAULT); \
		}                                                     \
		PSMI_CUDA_CALL(cuMemcpyDtoHAsync,                     \
			ghb->host_buf, (CUdeviceptr)ghb->gpu_buf,     \
			len, proto->cudastream_send);                 \
		PSMI_CUDA_CALL(cuEventRecord, ghb->copy_status,       \
			proto->cudastream_send);                      \
	} while (0)
#define PSM3_GPU_MEMCPY_DONE(ghb) \
	_psm3_cuda_memcpy_done(ghb)
#define PSM3_GPU_HOSTBUF_LAZY_INIT(ghb)                               \
	do {                                                          \
		ghb->copy_status = NULL;                              \
		ghb->host_buf = NULL;                                 \
	} while (0)
#define PSM3_GPU_HOSTBUF_FORCE_INIT(ghb, bufsz)                       \
	do {                                                          \
		PSM3_GPU_HOST_ALLOC(&ghb->host_buf, bufsz);           \
		PSMI_CUDA_CALL(cuEventCreate,                         \
			&ghb->copy_status, CU_EVENT_DEFAULT);         \
	} while (0)
#define PSM3_GPU_HOSTBUF_RESET(ghb)                                   \
	do {                                                          \
	} while (0)
#define PSM3_GPU_HOSTBUF_DESTROY(ghb)                                 \
	do {                                                          \
		if (ghb->copy_status != NULL) {                       \
			PSMI_CUDA_CALL(cuEventDestroy,                \
				ghb->copy_status);                    \
		}                                                     \
		if (ghb->host_buf != NULL) {                          \
			PSMI_CUDA_CALL(cuMemFreeHost,                 \
				ghb->host_buf);                       \
		}                                                     \
	} while (0)
#define PSM3_GPU_MEMCPY_DTOD(dstptr, srcptr, len) \
	do { PSMI_CUDA_CALL(cuMemcpyDtoD, (CUdeviceptr)(dstptr), (CUdeviceptr)(srcptr), (len)); } while (0)
#define PSM3_GPU_MEMCPY_HTOD(dstptr, srcptr, len) \
	do { PSMI_CUDA_CALL(cuMemcpyHtoD, (CUdeviceptr)(dstptr), (srcptr), (len)); } while (0)
#define PSM3_GPU_SYNCHRONIZE_MEMCPY() \
	do {PSMI_CUDA_CALL(cuStreamSynchronize, 0);} while (0)
#define PSM3_GPU_HOST_ALLOC(ret_ptr, size)                            \
	do {                                                          \
		PSMI_CUDA_CALL(cuMemHostAlloc, (void **)(ret_ptr),    \
			(size),CU_MEMHOSTALLOC_PORTABLE);             \
	} while (0)
#define PSM3_MARK_BUF_SYNCHRONOUS(buf) do { psmi_cuda_set_attr_sync_memops(buf); } while (0)
#define PSM3_GPU_MEMCPY_DTOH(dstptr, srcptr, len) \
	do { PSMI_CUDA_CALL(cuMemcpyDtoH, dstptr, (CUdeviceptr)(srcptr), len); } while (0)
#define PSM3_GPU_MEMCPY(dstptr, srcptr, len) \
	do { PSMI_CUDA_CALL(cuMemcpy, (CUdeviceptr)(dstptr), (CUdeviceptr)(srcptr), len); } while (0)
#define PSMI_IS_GPU_ENABLED  PSMI_IS_CUDA_ENABLED
#define PSMI_IS_GPU_DISABLED PSMI_IS_CUDA_DISABLED
#define PSMI_IS_GPU_MEM(x) PSMI_IS_CUDA_MEM(x)

#elif defined(PSM_ONEAPI)

#define PSM3_GPU_PREPARE_HTOD_MEMCPYS(protoexp)                       \
	do {                                                          \
		protoexp->cq_recv = NULL;                             \
	} while (0)
#define PSM3_GPU_PREPARE_DTOH_MEMCPYS(proto)                          \
	do {                                                          \
		proto->cq_send = NULL;                                \
	} while (0)
#define PSM3_GPU_SHUTDOWN_HTOD_MEMCPYS(protoexp)                      \
	do {                                                          \
		if (protoexp->cq_recv) {                              \
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueDestroy,    \
				protoexp->cq_recv);                   \
		}                                                     \
	} while (0)
#define PSM3_GPU_SHUTDOWN_DTOH_MEMCPYS(proto)                         \
	do {                                                          \
		if (proto->cq_send) {                                 \
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueDestroy,    \
				proto->cq_send);                      \
		}                                                     \
	} while (0)
#define PSM3_GPU_MEMCPY_HTOD_START(protoexp, ghb, len)                \
	do {                                                          \
		ze_command_queue_desc_t cq_desc = {                   \
			.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,\
			.flags = 0,                                   \
			.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS,   \
			.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL, \
			.ordinal = 0                                  \
		};                                                    \
		ze_event_pool_desc_t pool_desc = {                    \
			.stype = ZE_STRUCTURE_TYPE_EVENT_POOL_DESC,   \
			.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE,     \
			.count = 1                                    \
		};                                                    \
		ze_event_desc_t event_desc = {                        \
			.stype = ZE_STRUCTURE_TYPE_EVENT_DESC,        \
			.signal = ZE_EVENT_SCOPE_FLAG_HOST,           \
			.wait = ZE_EVENT_SCOPE_FLAG_HOST,             \
			.index = 0                                    \
		};                                                    \
		ze_command_list_desc_t cl_desc = {                    \
			.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC, \
			.flags = 0                                    \
		};                                                    \
		struct ze_dev_ctxt *ctxt;                             \
		                                                      \
		ctxt = psmi_oneapi_dev_ctxt_get(ghb->gpu_buf);        \
		if (!ctxt)                                            \
			psm3_handle_error(PSMI_EP_NORETURN,           \
					  PSM2_INTERNAL_ERR,          \
					  "%s HTOD: no dev ctxt\n",   \
					  __FUNCTION__);             \
		if (protoexp->cq_recv == NULL) {                      \
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueCreate,     \
				ze_context, ctxt->dev, &cq_desc,      \
				&protoexp->cq_recv);                  \
		}                                                     \
		if (ghb->event_pool == NULL) {                        \
			PSMI_ONEAPI_ZE_CALL(zeEventPoolCreate,        \
				ze_context, &pool_desc, 0, NULL,      \
				&ghb->event_pool);                    \
		}                                                     \
		if (ghb->copy_status == NULL) {                       \
			PSMI_ONEAPI_ZE_CALL(zeEventCreate,            \
				ghb->event_pool, &event_desc,         \
				&ghb->copy_status);                   \
		}                                                     \
		if (ghb->command_list == NULL) {                      \
			PSMI_ONEAPI_ZE_CALL(zeCommandListCreate,      \
				ze_context, ctxt->dev, &cl_desc,      \
				&ghb->command_list);                  \
		}                                                     \
		PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy,    \
			ghb->command_list,                            \
			ghb->gpu_buf, ghb->host_buf, len,             \
			ghb->copy_status, 0, NULL);                   \
		PSMI_ONEAPI_ZE_CALL(zeCommandListClose,               \
			ghb->command_list);                           \
		PSMI_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists,\
			protoexp->cq_recv,                            \
			1, &ghb->command_list, NULL);                 \
	} while (0)
#define PSM3_GPU_MEMCPY_DTOH_START(proto, ghb, len, bufsz)            \
	do {                                                          \
		ze_command_queue_desc_t cq_desc = {                   \
			.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,\
			.flags = 0,                                   \
			.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS,   \
			.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL, \
			.ordinal = 0                                  \
		};                                                    \
		ze_event_pool_desc_t pool_desc = {                    \
			.stype = ZE_STRUCTURE_TYPE_EVENT_POOL_DESC,   \
			.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE,     \
			.count = 1                                    \
		};                                                    \
		ze_event_desc_t event_desc = {                        \
			.stype = ZE_STRUCTURE_TYPE_EVENT_DESC,        \
			.signal = ZE_EVENT_SCOPE_FLAG_HOST,           \
			.wait = ZE_EVENT_SCOPE_FLAG_HOST,             \
			.index = 0                                    \
		};                                                    \
		ze_command_list_desc_t cl_desc = {                    \
			.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC, \
			.flags = 0                                    \
		};                                                    \
		struct ze_dev_ctxt *ctxt;                             \
		                                                      \
		ctxt = psmi_oneapi_dev_ctxt_get(ghb->gpu_buf);        \
		if (!ctxt)                                            \
			psm3_handle_error(PSMI_EP_NORETURN,           \
					  PSM2_INTERNAL_ERR,          \
					  "%s DTOH: no dev ctxt\n",   \
					  __FUNCTION__);              \
		if (proto->cq_send == NULL) {                         \
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueCreate,     \
				ze_context, ctxt->dev, &cq_desc,      \
				&proto->cq_send);                     \
		}                                                     \
		if (ghb->event_pool == NULL) {                        \
			PSMI_ONEAPI_ZE_CALL(zeEventPoolCreate,        \
				ze_context, &pool_desc, 0, NULL,      \
				&ghb->event_pool);                    \
		}                                                     \
		if (ghb->copy_status == NULL) {                       \
			PSMI_ONEAPI_ZE_CALL(zeEventCreate,            \
				ghb->event_pool, &event_desc,         \
				&ghb->copy_status);                   \
		}                                                     \
		if (ghb->command_list == NULL) {                      \
			PSMI_ONEAPI_ZE_CALL(zeCommandListCreate,      \
				ze_context, ctxt->dev, &cl_desc,      \
				&ghb->command_list);                  \
		}                                                     \
		if (ghb->host_buf == NULL && bufsz) {                 \
			PSM3_GPU_HOST_ALLOC(&ghb->host_buf, bufsz);   \
		}                                                     \
		PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy,    \
			ghb->command_list,                            \
			ghb->host_buf, ghb->gpu_buf, len,             \
			ghb->copy_status, 0, NULL);                   \
		PSMI_ONEAPI_ZE_CALL(zeCommandListClose,               \
			ghb->command_list);                           \
		PSMI_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists,\
			proto->cq_send,                               \
			1, &ghb->command_list, NULL);                 \
	} while (0)
#define PSM3_GPU_MEMCPY_DONE(ghb) \
	_psm3_oneapi_ze_memcpy_done(ghb)
#define PSM3_GPU_HOSTBUF_LAZY_INIT(ghb)                               \
	do {                                                          \
		ghb->event_pool = NULL;                               \
		ghb->copy_status = NULL;                              \
		ghb->command_list = NULL;                             \
		ghb->host_buf = NULL;                                 \
	} while (0)
#define PSM3_GPU_HOSTBUF_FORCE_INIT(ghb, bufsz)                       \
	do {                                                          \
		ze_event_pool_desc_t pool_desc = {                    \
			.stype = ZE_STRUCTURE_TYPE_EVENT_POOL_DESC,   \
			.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE,     \
			.count = 1                                    \
		};                                                    \
		ze_event_desc_t event_desc = {                        \
			.stype = ZE_STRUCTURE_TYPE_EVENT_DESC,        \
			.signal = ZE_EVENT_SCOPE_FLAG_HOST,           \
			.wait = ZE_EVENT_SCOPE_FLAG_HOST,             \
			.index = 0                                    \
		};                                                    \
		ze_command_list_desc_t cl_desc = {                    \
			.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC, \
			.flags = 0                                    \
		};                                                    \
		struct ze_dev_ctxt *ctxt;                             \
		                                                      \
		ctxt = psmi_oneapi_dev_ctxt_get(ghb->gpu_buf);        \
		if (!ctxt)                                            \
			psm3_handle_error(PSMI_EP_NORETURN,           \
					  PSM2_INTERNAL_ERR,          \
					  "%s F_INIT: no dev ctxt\n", \
					  __FUNCTION__);              \
		PSMI_ONEAPI_ZE_CALL(zeEventPoolCreate,                \
			ze_context, &pool_desc, 0, NULL,              \
			&ghb->event_pool);                            \
		PSMI_ONEAPI_ZE_CALL(zeEventCreate,                    \
			ghb->event_pool, &event_desc,                 \
			&ghb->copy_status);                           \
		PSMI_ONEAPI_ZE_CALL(zeCommandListCreate,              \
			ze_context, ctxt->dev, &cl_desc,              \
			&ghb->command_list);                          \
		PSM3_GPU_HOST_ALLOC(&ghb->host_buf, bufsz);           \
	} while (0)
#define PSM3_GPU_HOSTBUF_RESET(ghb)                                   \
	do {                                                          \
		PSMI_ONEAPI_ZE_CALL(zeCommandListReset,               \
			ghb->command_list);                           \
		PSMI_ONEAPI_ZE_CALL(zeEventHostReset,                 \
			ghb->copy_status);                            \
	} while (0)
#define PSM3_GPU_HOSTBUF_DESTROY(ghb)                                 \
	do {                                                          \
		if (ghb->copy_status != NULL) {                       \
			PSMI_ONEAPI_ZE_CALL(zeEventDestroy,           \
				ghb->copy_status);                    \
		}                                                     \
		if (ghb->host_buf != NULL) {                          \
			psmi_free(ghb->host_buf);                     \
		}                                                     \
		if (ghb->event_pool != NULL) {                        \
			PSMI_ONEAPI_ZE_CALL(zeEventPoolDestroy,       \
				ghb->event_pool);                     \
		}                                                     \
		if (ghb->command_list != NULL) {                      \
			PSMI_ONEAPI_ZE_CALL(zeCommandListDestroy,     \
				ghb->command_list);                   \
		}                                                     \
	} while (0)
#define PSM3_GPU_MEMCPY_DTOD(dstptr, srcptr, len) \
	do { psmi_oneapi_ze_memcpy(dstptr, srcptr, len); } while(0)
#define PSM3_GPU_MEMCPY_HTOD(dstptr, srcptr, len) \
	do { psmi_oneapi_ze_memcpy(dstptr, srcptr, len); } while(0)
#define PSM3_GPU_SYNCHRONIZE_MEMCPY() \
	do { /* not needed for OneAPI ZE */ } while (0)
#define PSM3_GPU_HOST_ALLOC(ret_ptr, size)                            \
	do {                                                          \
		*ret_ptr = psmi_malloc(PSMI_EP_NONE, UNDEFINED, size);\
	} while (0)
#define PSM3_MARK_BUF_SYNCHRONOUS(buf) do { /* not needed for OneAPI ZE */ } while (0)
#define PSM3_GPU_MEMCPY_DTOH(dstptr, srcptr, len) \
	do { psmi_oneapi_ze_memcpy(dstptr, srcptr, len); } while (0)
#define PSM3_GPU_MEMCPY(dstptr, srcptr, len) \
	do { psmi_oneapi_ze_memcpy(dstptr, srcptr, len); } while (0)
#define PSMI_IS_GPU_ENABLED  PSMI_IS_ONEAPI_ZE_ENABLED
#define PSMI_IS_GPU_DISABLED PSMI_IS_ONEAPI_ZE_DISABLED
#define PSMI_IS_GPU_MEM(x) PSMI_IS_ONEAPI_ZE_MEM(x)

#endif /* elif PSM_ONEAPI */


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _PSMI_USER_H */
