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

#include "opa_user.h"
#include "opa_queue.h"

#include "psm_log.h"
#include "psm_perf.h"

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
#include "psm_ep.h"
#include "psm_lock.h"
#include "psm_stats.h"
#include "psm2_mock_testing.h"

#undef _PSMI_IN_USER_H

#define PSMI_VERNO_MAKE(major, minor) ((((major)&0xff)<<8)|((minor)&0xff))
#define PSMI_VERNO  PSMI_VERNO_MAKE(PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR)
#define PSMI_VERNO_GET_MAJOR(verno) (((verno)>>8) & 0xff)
#define PSMI_VERNO_GET_MINOR(verno) (((verno)>>0) & 0xff)

int psmi_verno_client();
int psmi_verno_isinteroperable(uint16_t verno);
int MOCKABLE(psmi_isinitialized)();
MOCK_DCL_EPILOGUE(psmi_isinitialized);

psm2_error_t psmi_poll_internal(psm2_ep_t ep, int poll_amsh);
psm2_error_t psmi_mq_wait_internal(psm2_mq_req_t *ireq);

int psmi_get_current_proc_location();

extern int psmi_epid_ver;
extern int psmi_allow_routers;
extern uint32_t non_dw_mul_sdma;
extern psmi_lock_t psmi_creation_lock;
extern psm2_ep_t psmi_opened_endpoint;

extern int psmi_affinity_shared_file_opened;
extern uint64_t *shared_affinity_ptr;
extern char *affinity_shm_name;

extern sem_t *sem_affinity_shm_rw;
extern int psmi_affinity_semaphore_open;
extern char *sem_affinity_shm_rw_name;

PSMI_ALWAYS_INLINE(
int
_psmi_get_epid_version()) {
	return psmi_epid_ver;
}

#define PSMI_EPID_VERSION_SHM 				0
#define PSMI_EPID_SHM_ONLY				1
#define PSMI_EPID_IPS_SHM				0
#define PSMI_EPID_VERSION 				_psmi_get_epid_version()
#define PSMI_MAX_EPID_VERNO_SUPPORTED			4
#define PSMI_MIN_EPID_VERNO_SUPPORTED			3
#define PSMI_EPID_VERNO_DEFAULT				3	// allows 3 or 4 based on NIC
#define PSMI_EPID_V3					3	// IB UD
#define PSMI_EPID_V4					4	// Eth UD

#define PSMI_EPID_GET_LID(epid) ((PSMI_EPID_GET_EPID_VERSION(epid) == PSMI_EPID_V3) ? \
								 (int)PSMI_EPID_GET_LID_V3(epid)      \
							   : (int)PSMI_EPID_GET_LID_V4(epid))
// for V3 we use low 16 and next 16 should be zero
// for V4 we have network in low 32 bits
#define PSMI_GET_SUBNET_ID(gid_hi) (gid_hi & 0xffffffff)


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

extern int is_cuda_enabled;
extern int is_gdr_copy_enabled;
extern int is_gpudirect_enabled; // only for use during parsing of other params
extern int _device_support_unified_addr;
extern int _device_support_gpudirect;
extern int _gpu_p2p_supported;
extern int my_gpu_device;
extern int cuda_lib_version;

extern CUcontext cu_ctxt;
extern void *psmi_cuda_lib;

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
			psmi_handle_error(PSMI_EP_NORETURN,		\
			PSM2_INTERNAL_ERR, "Failed to set/synchronize"	\
			" CUDA context.\n");				\
		}							\
		psmi_count_##func++;					\
		cudaerr = psmi_##func(args);				\
		if (cudaerr != CUDA_SUCCESS) {				\
			_HFI_ERROR(					\
				"CUDA failure: %s() (at %s:%d)"		\
				"returned %d\n",			\
				#func, __FILE__, __LINE__, cudaerr);	\
			psmi_handle_error(				\
				PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function.\n");\
		}							\
	} while (0)

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
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_EP_DEVICE_FAILURE,
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

	/* Check if all devices support Unified Virtual Addressing. */
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
			psmi_handle_error(PSMI_EP_NORETURN,		\
				PSM2_INTERNAL_ERR, "Failed to "		\
				"set/synchronize CUDA context.\n");	\
		}							\
		psmi_count_##func++;					\
		cudaerr = psmi_##func(args);				\
		if (cudaerr != CUDA_SUCCESS && cudaerr != except_err) {	\
			if (cu_ctxt == NULL)				\
				_HFI_ERROR(				\
				"Check if CUDA is initialized"	\
				"before psm2_ep_open call \n");		\
			_HFI_ERROR(					\
				"CUDA failure: %s() (at %s:%d)"		\
				"returned %d\n",			\
				#func, __FILE__, __LINE__, cudaerr);	\
			psmi_handle_error(				\
				PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function.\n");\
		} else if (cudaerr == except_err) { \
			_HFI_DBG( \
				"CUDA non-zero return value: %s() (at %s:%d)"		\
				"returned %d\n",			\
				#func, __FILE__, __LINE__, cudaerr);	\
		} \
	} while (0)

#define PSMI_CUDA_CHECK_EVENT(event, cudaerr) do {			\
		psmi_count_cuEventQuery++;				\
		cudaerr = psmi_cuEventQuery(event);			\
		if ((cudaerr != CUDA_SUCCESS) &&			\
		    (cudaerr != CUDA_ERROR_NOT_READY)) {		\
			_HFI_ERROR(					\
				"CUDA failure: %s() returned %d\n",	\
				"cuEventQuery", cudaerr);		\
			psmi_handle_error(				\
				PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function.\n");\
		}							\
	} while (0)

#define PSMI_CUDA_DLSYM(psmi_cuda_lib,func) do {                        \
	psmi_##func = dlsym(psmi_cuda_lib, STRINGIFY(func));            \
	if (!psmi_##func) {               				\
		psmi_handle_error(PSMI_EP_NORETURN,                     \
			       PSM2_INTERNAL_ERR,                       \
			       " Unable to resolve %s symbol"		\
			       " in CUDA libraries.\n",STRINGIFY(func));\
	}                                                               \
} while (0)

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

PSMI_ALWAYS_INLINE(
int
_psmi_is_gdr_copy_enabled())
{
        return is_gdr_copy_enabled;
}

#define PSMI_IS_GDR_COPY_ENABLED _psmi_is_gdr_copy_enabled()

#define PSMI_IS_CUDA_MEM(p) _psmi_is_cuda_mem(p)
extern void psm2_get_gpu_bars(void);

struct ips_cuda_hostbuf {
	STAILQ_ENTRY(ips_cuda_hostbuf) req_next;
	STAILQ_ENTRY(ips_cuda_hostbuf) next;
	uint32_t size, offset, bytes_read;
	/* This flag indicates whether a chb is
	 * pulled from a mpool or dynamically
	 * allocated using calloc. */
	uint8_t is_tempbuf;
	CUevent copy_status;
	psm2_mq_req_t req;
	void *host_buf;
	CUdeviceptr gpu_buf;
};

struct ips_cuda_hostbuf_mpool_cb_context {
	unsigned bufsz;
};
void psmi_cuda_hostbuf_alloc_func(int is_alloc, void *context, void *obj);

#define CUDA_HOSTBUFFER_LIMITS {				\
	    .env = "PSM3_CUDA_BOUNCEBUFFERS_MAX",		\
	    .descr = "Max CUDA bounce buffers (in MB)",		\
	    .env_level = PSMI_ENVVAR_LEVEL_HIDDEN,		\
	    .minval = 1,					\
	    .maxval = 1<<30,					\
	    .mode[PSMI_MEMMODE_NORMAL]  = {  16, 256 },		\
	    .mode[PSMI_MEMMODE_MINIMAL] = {   1,   1 },		\
	    .mode[PSMI_MEMMODE_LARGE]   = {  32, 512 }		\
	}

extern uint32_t gpudirect_send_limit;
extern uint32_t gpudirect_recv_limit;
extern uint32_t cuda_thresh_rndv;
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

uint64_t gpu_cache_evict;

// Only valid if called for a GPU buffer
#define PSMI_USE_GDR_COPY_RECV(len) ((len) >=1 && (len) <= gdr_copy_limit_recv)

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

#endif /* PSM_CUDA */

#define COMPILE_TIME_ASSERT(NAME,COND) extern char NAME[1/ COND]

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _PSMI_USER_H */
