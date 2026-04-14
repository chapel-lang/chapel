/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2024 Intel Corporation.

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

  Copyright(c) 2024 Intel Corporation.

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

/* Copyright (c) 2003-2024 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
#include <stdbool.h>
#include <assert.h>

#include "psm_user.h"

#ifdef PSM_CUDA
#include <dlfcn.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <driver_types.h>

#if CUDA_VERSION < 7000
#error Please update CUDA driver, required minimum version is 7.0
#endif

#include "psm2_hal.h"
#include "psm_mq_internal.h"
#include "ptl_am/psm_am_internal.h"
#include "ptl_ips/ips_proto.h"
#include "ptl_ips/ips_expected_proto.h"

// cuCtxSetFlags(CU_CTX_SYNC_MEMOPS) was introduced in CUDA 12.1.0
#define PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS (CUDA_VERSION >= 12010)

// if defined, do not use cuMemHostRegister for malloced pipeline
// copy bounce buffers
// otherwise, use cuMemHostRegister when malloc buffer
//#define PSM3_NO_CUDA_REGISTER

// default value for PSM3_GPU_THRESH_RNDV
#define PSM3_CUDA_GPU_THRESH_RNDV 8000
// default value for PSM3_GPU_RNDV_NIC_WINDOW when using Cuda GPU
#define PSM3_CUDA_RNDV_NIC_WINDOW_DEFAULT "2097152"
// default value for PSM3_GPUDIRECT_RDMA_SEND_LIMIT
#define PSM3_CUDA_GPUDIRECT_RDMA_SEND_LIMIT_DEFAULT UINT_MAX
// default value for PSM3_GPUDIRECT_RDMA_RECV_LIMIT
#define PSM3_CUDA_GPUDIRECT_RDMA_RECV_LIMIT_DEFAULT UINT_MAX
// default value for PSM3_MQ_RNDV_SHM_GPU_THRESH
// Threshold for GPU rendezvous (aka scale-up transfer vs via CPU shared mem
#define PSM3_CUDA_MQ_RNDV_SHM_GPU_THRESH 63

/* CUDA Driver Library */
static void *psm3_cuda_lib;
static int psm3_cuda_lib_version;
/* CUDA Runtime (psm3_cudart) Library */
static void *psm3_cudart_lib;
static int psm3_cuda_runtime_ver;

/* This is a global cuda context
 * stored to provide hints during a cuda failure
 * due to a null cuda context.
 */
CUcontext psm3_cu_ctxt;

#ifdef PSM_HAVE_RNDV_MOD
static int psm3_cuda_gpu_pin_check;	// PSM3_GPU_PIN_CHECK
static uint64_t *psm3_cuda_gpu_bars;
static int psm3_cuda_num_gpu_bars = 0;
static uint64_t psm3_cuda_min_gpu_bar_size;

static uint64_t psm3_cuda_get_nvidia_bar_addr(int domain, int bus, int slot);
#endif

typedef enum {
	PSM3_CPE_REJECT = 0,
	PSM3_CPE_IGNORE = 1,
	PSM3_CPE_OBEY = 2,
} psm3_cuda_permitted_enforcement_t;

static psm3_cuda_permitted_enforcement_t psm3_cuda_permitted_enforcement = PSM3_CPE_IGNORE;

typedef enum {
	PSM3_CUDA_SYNC_CTX         = 0,
	PSM3_CUDA_SYNC_PTR         = 1,
	PSM3_CUDA_SYNC_PTR_RELAXED = 2,
	PSM3_CUDA_SYNC_NONE        = 3,
} psm3_cuda_sync_mode_t;

static psm3_cuda_sync_mode_t psm3_cuda_sync_mode = PSM3_CUDA_SYNC_CTX;

/* function pointers from dlopen access to cuda shared library */
#define PSM3_CUDA_SYM_FP(name) PSM3_CONCAT(psm3_cuda_, name)
static CUresult (*PSM3_CUDA_SYM_FP(cuInit))(unsigned int  Flags );
static CUresult (*PSM3_CUDA_SYM_FP(cuCtxDetach))(CUcontext c);
static CUresult (*PSM3_CUDA_SYM_FP(cuCtxGetCurrent))(CUcontext *c);
static CUresult (*PSM3_CUDA_SYM_FP(cuCtxSetCurrent))(CUcontext c);
#if PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS
static CUresult (*PSM3_CUDA_SYM_FP(cuCtxSetFlags))(unsigned int flags);
#endif
static CUresult (*PSM3_CUDA_SYM_FP(cuPointerGetAttribute))(void *data, CUpointer_attribute pa, CUdeviceptr p);
static CUresult (*PSM3_CUDA_SYM_FP(cuPointerGetAttributes))(unsigned int count, CUpointer_attribute *attrs, void **data, CUdeviceptr ptr);
static CUresult (*PSM3_CUDA_SYM_FP(cuPointerSetAttribute))(void *data, CUpointer_attribute pa, CUdeviceptr p);
static CUresult (*PSM3_CUDA_SYM_FP(cuDeviceCanAccessPeer))(int *canAccessPeer, CUdevice dev, CUdevice peerDev);
static CUresult (*PSM3_CUDA_SYM_FP(cuDeviceGet))(CUdevice* device, int  ordinal);
static CUresult (*PSM3_CUDA_SYM_FP(cuDeviceGetAttribute))(int* pi, CUdevice_attribute attrib, CUdevice dev);
static CUresult (*PSM3_CUDA_SYM_FP(cuDriverGetVersion))(int* driverVersion);
static CUresult (*PSM3_CUDA_SYM_FP(cuDeviceGetCount))(int* count);
static CUresult (*PSM3_CUDA_SYM_FP(cuStreamCreate))(CUstream* phStream, unsigned int Flags);
static CUresult (*PSM3_CUDA_SYM_FP(cuStreamDestroy))(CUstream phStream);
static CUresult (*PSM3_CUDA_SYM_FP(cuStreamSynchronize))(CUstream phStream);
static CUresult (*PSM3_CUDA_SYM_FP(cuEventCreate))(CUevent* phEvent, unsigned int Flags);
static CUresult (*PSM3_CUDA_SYM_FP(cuEventDestroy))(CUevent hEvent);
static CUresult (*PSM3_CUDA_SYM_FP(cuEventQuery))(CUevent hEvent);
static CUresult (*PSM3_CUDA_SYM_FP(cuEventRecord))(CUevent hEvent, CUstream hStream);
static CUresult (*PSM3_CUDA_SYM_FP(cuEventSynchronize))(CUevent hEvent);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemRetainAllocationHandle))(CUmemGenericAllocationHandle *h, CUdeviceptr p);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemRelease))(CUmemGenericAllocationHandle h);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemHostAlloc))(void** pp, size_t bytesize, unsigned int Flags);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemFreeHost))(void* p);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemHostRegister))(void* p, size_t bytesize, unsigned int Flags);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemHostUnregister))(void* p);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemcpy))(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemcpyDtoD))(CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemcpyDtoH))(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemcpyHtoD))(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemcpyDtoHAsync))(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemcpyHtoDAsync))(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
static CUresult (*PSM3_CUDA_SYM_FP(cuIpcGetMemHandle))(CUipcMemHandle* pHandle, CUdeviceptr dptr);
static CUresult (*PSM3_CUDA_SYM_FP(cuIpcOpenMemHandle))(CUdeviceptr* pdptr, CUipcMemHandle handle, unsigned int Flags);
static CUresult (*PSM3_CUDA_SYM_FP(cuIpcCloseMemHandle))(CUdeviceptr dptr);
static CUresult (*PSM3_CUDA_SYM_FP(cuMemGetAddressRange))(CUdeviceptr* pbase, size_t* psize, CUdeviceptr dptr);
static CUresult (*PSM3_CUDA_SYM_FP(cuDevicePrimaryCtxGetState))(CUdevice dev, unsigned int* flags, int* active);
static CUresult (*PSM3_CUDA_SYM_FP(cuDevicePrimaryCtxRetain))(CUcontext* pctx, CUdevice dev);
static CUresult (*PSM3_CUDA_SYM_FP(cuCtxGetDevice))(CUdevice* device);
static CUresult (*PSM3_CUDA_SYM_FP(cuDevicePrimaryCtxRelease))(CUdevice device);
static CUresult (*PSM3_CUDA_SYM_FP(cuGetErrorString))(CUresult error, const char **pStr);
static cudaError_t (*PSM3_CUDA_SYM_FP(cudaRuntimeGetVersion))(int* runtimeVersion);

/* statistics counting each cuda call PSM3 makes */
#define PSM3_CUDA_SYM_COUNT(name) PSM3_CONCAT(psm3_cuda_count_, name)
static uint64_t PSM3_CUDA_SYM_COUNT(cuInit);
static uint64_t PSM3_CUDA_SYM_COUNT(cuCtxDetach);
static uint64_t PSM3_CUDA_SYM_COUNT(cuCtxGetCurrent);
static uint64_t PSM3_CUDA_SYM_COUNT(cuCtxSetCurrent);
#if PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS
static uint64_t PSM3_CUDA_SYM_COUNT(cuCtxSetFlags);
#endif
static uint64_t PSM3_CUDA_SYM_COUNT(cuPointerGetAttribute);
static uint64_t PSM3_CUDA_SYM_COUNT(cuPointerGetAttributes);
static uint64_t PSM3_CUDA_SYM_COUNT(cuPointerSetAttribute);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDeviceCanAccessPeer);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDeviceGet);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDeviceGetAttribute);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDriverGetVersion);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDeviceGetCount);
static uint64_t PSM3_CUDA_SYM_COUNT(cuStreamCreate);
static uint64_t PSM3_CUDA_SYM_COUNT(cuStreamDestroy);
static uint64_t PSM3_CUDA_SYM_COUNT(cuStreamSynchronize);
static uint64_t PSM3_CUDA_SYM_COUNT(cuEventCreate);
static uint64_t PSM3_CUDA_SYM_COUNT(cuEventDestroy);
static uint64_t PSM3_CUDA_SYM_COUNT(cuEventQuery);
static uint64_t PSM3_CUDA_SYM_COUNT(cuEventRecord);
static uint64_t PSM3_CUDA_SYM_COUNT(cuEventSynchronize);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemRetainAllocationHandle);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemRelease);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemHostAlloc);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemFreeHost);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemHostRegister);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemHostUnregister);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemcpy);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemcpyDtoD);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemcpyDtoH);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemcpyHtoD);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemcpyDtoHAsync);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemcpyHtoDAsync);
static uint64_t PSM3_CUDA_SYM_COUNT(cuIpcGetMemHandle);
static uint64_t PSM3_CUDA_SYM_COUNT(cuIpcOpenMemHandle);
static uint64_t PSM3_CUDA_SYM_COUNT(cuIpcCloseMemHandle);
static uint64_t PSM3_CUDA_SYM_COUNT(cuMemGetAddressRange);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDevicePrimaryCtxGetState);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDevicePrimaryCtxRetain);
static uint64_t PSM3_CUDA_SYM_COUNT(cuCtxGetDevice);
static uint64_t PSM3_CUDA_SYM_COUNT(cuDevicePrimaryCtxRelease);
static uint64_t PSM3_CUDA_SYM_COUNT(cuGetErrorString);
static uint64_t PSM3_CUDA_SYM_COUNT(cudaRuntimeGetVersion);

/* Set the context-level SYNC_MEMOPS flag (as opposed to the pointer
 * attribute)
 */
static void psm3_cuda_ctx_set_sync_memops(void)
{
#if PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS
	if (psm3_cuda_sync_mode != PSM3_CUDA_SYNC_CTX)
		return;
	CUresult err = PSM3_CUDA_SYM_FP(cuCtxSetFlags)(CU_CTX_SYNC_MEMOPS);
	if_pf (err != CUDA_SUCCESS)
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Failed to set CUDA context flag: SYNC_MEMOPS\n");
#endif
}

static int psm3_cuda_check_set_cuda_ctxt(void)
{
	CUresult err;
	CUcontext tmpctxt = {0};

	if (unlikely(!PSM3_CUDA_SYM_FP(cuCtxGetCurrent) || !PSM3_CUDA_SYM_FP(cuCtxSetCurrent)))
		return 0;

	err = PSM3_CUDA_SYM_FP(cuCtxGetCurrent)(&tmpctxt);
	if (likely(!err)) {
		if (unlikely(!tmpctxt && psm3_cu_ctxt)) {
			err = PSM3_CUDA_SYM_FP(cuCtxSetCurrent)(psm3_cu_ctxt);
			if (likely(!err))
				psm3_cuda_ctx_set_sync_memops();
			return !!err;
		} else if (unlikely(tmpctxt && !psm3_cu_ctxt)) {
			psm3_cu_ctxt = tmpctxt;
			psm3_cuda_ctx_set_sync_memops();
		}
	}
	return 0;
}

/* Make sure have a real GPU job.  Set psm3_cu_ctxt if available */
PSMI_ALWAYS_INLINE(
int psm3_cuda_check_have_cuda_ctxt(void))
{
	if (! psm3_cu_ctxt) {
		if (unlikely(psm3_cuda_check_set_cuda_ctxt())) {
			psm3_handle_error(PSMI_EP_NORETURN,
			PSM2_INTERNAL_ERR, "Failed to set/synchronize"
			" CUDA context.\n");
		}
	}
	return (psm3_cu_ctxt != NULL);
}

/**
 * execute the specified function and return the result without error handling
 */
#define PSM3_CUDA_EXEC_ASSUME_CONTEXT(func, args...) \
	({ \
		PSM3_CONCAT(psm3_cuda_count_, func)++; \
		(CUresult)PSM3_CONCAT(psm3_cuda_, func)(args); \
	})

#define PSM3_CUDA_EXEC(func, args...) \
	({ \
		if (unlikely(psm3_cuda_check_set_cuda_ctxt())) { \
			psm3_handle_error( \
				PSMI_EP_NORETURN, \
				PSM2_INTERNAL_ERR, \
				"Failed to set/synchronize CUDA context.\n"); \
		} \
		PSM3_CUDA_EXEC_ASSUME_CONTEXT(func, args); \
	})

/**
 * apply boilerplate non-fatal error handling to the indicated error
 */
#define PSM3_CUDA_ERROR(func, cudaerr, log_level) \
	do { \
		const char *pStr = NULL; \
		PSM3_CUDA_SYM_COUNT(cuGetErrorString)++; \
		PSM3_CUDA_SYM_FP(cuGetErrorString)(cudaerr, &pStr); \
		_HFI_##log_level( \
			"CUDA failure: %s() (at %s:%d) returned %d: %s\n", \
			#func, __FILE__, __LINE__, cudaerr, \
			pStr ? pStr : "Unknown"); \
	} while (0)

/**
 * check for errors, do necessary boilerplate, then fail hard
 */
#define PSM3_CUDA_CHECK(func, cudaerr) \
	do { \
		if (cudaerr != CUDA_SUCCESS) { \
			PSM3_CUDA_ERROR(func, cudaerr, ERROR); \
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, \
				"Error returned from CUDA function %s.\n", #func); \
		} \
	} while (0)

/**
 * execute the CUDA function and handle any errors with failure
 */
#define PSM3_CUDA_CALL(func, args...) \
	do { \
		CUresult cudaerr = PSM3_CUDA_EXEC(func, args); \
		PSM3_CUDA_CHECK(func, cudaerr); \
	} while (0)

/**
 * Similar to PSM3_CUDA_CALL() except does not error out
 * if func(args) returns CUDA_SUCCESS or except_err
 *
 * Invoker must provide 'CUresult cudaerr' in invoked scope
 * so invoker can inspect whether cudaerr == CUDA_SUCCESS or
 * cudaerr == except_err after expanded code is executed.
 *
 * As except_err is an allowed value, message is printed at
 * DBG level.
 */
#define PSM3_CUDA_CALL_EXCEPT(except_err, func, args...) \
	({ \
		CUresult cudaerr; \
		do { \
			cudaerr = PSM3_CUDA_EXEC(func, args); \
			if (cudaerr == except_err) { \
				PSM3_CUDA_ERROR(func, cudaerr, ERROR); \
				break; \
			} \
			PSM3_CUDA_CHECK(func, cudaerr); \
		} while (0); \
		cudaerr; \
	})

#define PSM3_CUDA_CHECK_EVENT(event, cudaerr) do {			\
		PSM3_CUDA_SYM_COUNT(cuEventQuery)++;				\
		cudaerr = PSM3_CUDA_SYM_FP(cuEventQuery)(event);			\
		if ((cudaerr != CUDA_SUCCESS) && (cudaerr != CUDA_ERROR_NOT_READY)) { \
			const char *pStr = NULL;			\
			PSM3_CUDA_SYM_COUNT(cuGetErrorString)++;			\
			PSM3_CUDA_SYM_FP(cuGetErrorString)(cudaerr, &pStr);		\
			_HFI_ERROR(					\
				"CUDA failure: %s() (at %s:%d) returned %d: %s\n",	\
				"cuEventQuery", __FILE__, __LINE__, cudaerr,		\
				pStr?pStr:"Unknown");			\
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Error returned from CUDA function cuEventQuery.\n");\
		}							\
	} while (0)

// resolve a cuda shared library symbol
#define PSM3_CUDA_DLSYM(psm3_cuda_lib,func) do {                \
	PSM3_CONCAT(psm3_cuda_, func) = dlsym(psm3_cuda_lib, STRINGIFY(func));   \
	if (!PSM3_CONCAT(psm3_cuda_, func)) {                                    \
		psm3_handle_error(PSMI_EP_NORETURN,                     \
				PSM2_INTERNAL_ERR,                              \
				" Unable to resolve %s symbol"                  \
				" in CUDA libraries.\n",STRINGIFY(func));       \
	}                                                           \
} while (0)

static int psm3_cuda_lib_load()
{
	psm2_error_t err = PSM2_OK;
	char *dlerr;

	PSM2_LOG_MSG("entering");
	_HFI_DBG("Loading CUDA library.\n");

	psm3_cuda_lib = dlopen("libcuda.so.1", RTLD_LAZY);
	if (!psm3_cuda_lib) {
		dlerr = dlerror();
		_HFI_ERROR("Unable to open libcuda.so.1.  Error %s\n",
				dlerr ? dlerr : "no dlerror()");
		goto fail;
	}

	PSM3_CUDA_SYM_FP(cuDriverGetVersion) = dlsym(psm3_cuda_lib, "cuDriverGetVersion");

	if (!PSM3_CUDA_SYM_FP(cuDriverGetVersion)) {
		_HFI_ERROR
			("Unable to resolve symbols in CUDA libraries.\n");
		goto fail;
	}
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuGetErrorString);// for PSM3_CUDA_CALL

	PSM3_CUDA_CALL(cuDriverGetVersion, &psm3_cuda_lib_version);
	if (psm3_cuda_lib_version < 7000) {
		_HFI_ERROR("Please update CUDA driver, required minimum version is 7.0\n");
		goto fail;
	}

	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuInit);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuCtxGetCurrent);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuCtxDetach);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuCtxSetCurrent);
#if PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuCtxSetFlags);
#endif
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuPointerGetAttribute);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuPointerGetAttributes);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuPointerSetAttribute);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDeviceCanAccessPeer);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDeviceGetAttribute);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDeviceGet);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDeviceGetCount);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuStreamCreate);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuStreamDestroy);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuStreamSynchronize);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuEventCreate);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuEventDestroy);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuEventQuery);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuEventRecord);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuEventSynchronize);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemRetainAllocationHandle);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemRelease);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemHostAlloc);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemFreeHost);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemHostRegister);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemHostUnregister);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemcpy);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemcpyDtoD);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemcpyDtoH);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemcpyHtoD);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemcpyDtoHAsync);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemcpyHtoDAsync);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuIpcGetMemHandle);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuIpcOpenMemHandle);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuIpcCloseMemHandle);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuMemGetAddressRange);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDevicePrimaryCtxGetState);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDevicePrimaryCtxRetain);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuDevicePrimaryCtxRelease);
	PSM3_CUDA_DLSYM(psm3_cuda_lib, cuCtxGetDevice);

	/* CUDA Runtime */
	psm3_cudart_lib = dlopen("libcudart.so", RTLD_LAZY);
	if (!psm3_cudart_lib) {
		dlerr = dlerror();
		_HFI_ERROR("Unable to open libcudart.so.  Error %s\n",
				dlerr ? dlerr : "no dlerror()");
		goto fail;
	}
	PSM3_CUDA_DLSYM(psm3_cudart_lib, cudaRuntimeGetVersion);

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	if (psm3_cuda_lib)
		dlclose(psm3_cuda_lib);
	if (psm3_cudart_lib)
		dlclose(psm3_cudart_lib);
	err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, "Unable to load CUDA library.\n");
	return err;
}

static void psm3_cuda_stats_register()
{
#define PSM3_CUDA_COUNT_DECLU64(func) \
	PSMI_STATS_DECLU64(#func, NULL, &PSM3_CONCAT(psm3_cuda_count_, func))

	struct psmi_stats_entry entries[] = {
		PSM3_CUDA_COUNT_DECLU64(cuInit),
		PSM3_CUDA_COUNT_DECLU64(cuCtxDetach),
		PSM3_CUDA_COUNT_DECLU64(cuCtxGetCurrent),
		PSM3_CUDA_COUNT_DECLU64(cuCtxSetCurrent),
#if PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS
		PSM3_CUDA_COUNT_DECLU64(cuCtxSetFlags),
#endif
		PSM3_CUDA_COUNT_DECLU64(cuPointerGetAttribute),
		PSM3_CUDA_COUNT_DECLU64(cuPointerGetAttributes),
		PSM3_CUDA_COUNT_DECLU64(cuPointerSetAttribute),
		PSM3_CUDA_COUNT_DECLU64(cuDeviceCanAccessPeer),
		PSM3_CUDA_COUNT_DECLU64(cuDeviceGet),
		PSM3_CUDA_COUNT_DECLU64(cuDeviceGetAttribute),
		PSM3_CUDA_COUNT_DECLU64(cuDriverGetVersion),
		PSM3_CUDA_COUNT_DECLU64(cuDeviceGetCount),
		PSM3_CUDA_COUNT_DECLU64(cuStreamCreate),
		PSM3_CUDA_COUNT_DECLU64(cuStreamDestroy),
		PSM3_CUDA_COUNT_DECLU64(cuStreamSynchronize),
		PSM3_CUDA_COUNT_DECLU64(cuEventCreate),
		PSM3_CUDA_COUNT_DECLU64(cuEventDestroy),
		PSM3_CUDA_COUNT_DECLU64(cuEventQuery),
		PSM3_CUDA_COUNT_DECLU64(cuEventRecord),
		PSM3_CUDA_COUNT_DECLU64(cuEventSynchronize),
		PSM3_CUDA_COUNT_DECLU64(cuMemRetainAllocationHandle),
		PSM3_CUDA_COUNT_DECLU64(cuMemRelease),
		PSM3_CUDA_COUNT_DECLU64(cuMemHostAlloc),
		PSM3_CUDA_COUNT_DECLU64(cuMemFreeHost),
		PSM3_CUDA_COUNT_DECLU64(cuMemHostRegister),
		PSM3_CUDA_COUNT_DECLU64(cuMemHostUnregister),
		PSM3_CUDA_COUNT_DECLU64(cuMemcpy),
		PSM3_CUDA_COUNT_DECLU64(cuMemcpyDtoD),
		PSM3_CUDA_COUNT_DECLU64(cuMemcpyDtoH),
		PSM3_CUDA_COUNT_DECLU64(cuMemcpyHtoD),
		PSM3_CUDA_COUNT_DECLU64(cuMemcpyDtoHAsync),
		PSM3_CUDA_COUNT_DECLU64(cuMemcpyHtoDAsync),
		PSM3_CUDA_COUNT_DECLU64(cuIpcGetMemHandle),
		PSM3_CUDA_COUNT_DECLU64(cuIpcOpenMemHandle),
		PSM3_CUDA_COUNT_DECLU64(cuIpcCloseMemHandle),
		PSM3_CUDA_COUNT_DECLU64(cuMemGetAddressRange),
		PSM3_CUDA_COUNT_DECLU64(cuDevicePrimaryCtxGetState),
		PSM3_CUDA_COUNT_DECLU64(cuDevicePrimaryCtxRetain),
		PSM3_CUDA_COUNT_DECLU64(cuCtxGetDevice),
		PSM3_CUDA_COUNT_DECLU64(cuDevicePrimaryCtxRelease),
		PSM3_CUDA_COUNT_DECLU64(cuGetErrorString),
		PSM3_CUDA_COUNT_DECLU64(cudaRuntimeGetVersion),
	};
#undef PSM3_CUDA_COUNT_DECLU64

	psm3_stats_register_type("PSM_Cuda_call_statistics",
		"Count of CUDA calls per API entry point for the whole process.\n"
		"When using an NVIDIA GPU, PSM3 may call lower level CUDA "
		"APIs to access or transfer application buffers in GPU memory.",
			PSMI_STATSTYPE_GPU,
			entries, PSMI_HOWMANY(entries), NULL,
			&PSM3_CUDA_SYM_COUNT(cuInit), NULL); /* context must != NULL */
}

#ifdef PSM_HAVE_RNDV_MOD
static void psm3_cuda_get_bars(void)
{
	int num_devices, dev;
	union psmi_envvar_val env;

	psm3_getenv("PSM3_GPU_PIN_CHECK",
			"Enable sanity check of physical addresses mapped into GPU BAR space (Enabled by default)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)1, &env);
	psm3_cuda_gpu_pin_check = env.e_int;

	PSM3_CUDA_CALL(cuDeviceGetCount, &num_devices);
	psm3_cuda_gpu_bars = psmi_calloc(PSMI_EP_NONE, UNDEFINED, num_devices, sizeof(psm3_cuda_gpu_bars[0]));
	if (! psm3_cuda_gpu_bars)
		return;	// psmi_calloc will have exited for Out of Memory

	if (psm3_cuda_gpu_pin_check)
		psm3_cuda_num_gpu_bars = num_devices;

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		int domain, bus, slot;

		PSM3_CUDA_CALL(cuDeviceGet, &device, dev);
		PSM3_CUDA_CALL(cuDeviceGetAttribute,
				&domain,
				CU_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID,
				device);
		PSM3_CUDA_CALL(cuDeviceGetAttribute,
				&bus,
				CU_DEVICE_ATTRIBUTE_PCI_BUS_ID,
				device);
		PSM3_CUDA_CALL(cuDeviceGetAttribute,
				&slot,
				CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID,
				device);
		psm3_cuda_gpu_bars[dev] = psm3_cuda_get_nvidia_bar_addr(domain, bus, slot);
	}
}
#endif /* PSM_HAVE_RNDV_MOD */

static void psm3_cuda_init_env_cpe(void)
{
	union psmi_envvar_val val;

	int ret = psm3_getenv_range("PSM3_CUDA_PERMITTED_ENFORCEMENT",
		"Enforcement policy for the CUDA_PERMITTED endpoint flag\n",
		"  0: REJECT attempts to modify as an error\n"
		"  1: IGNORE attempts to modify, feigning success (default)\n"
		"  2: OBEY by restricting CUDA usage",
		PSMI_ENVVAR_LEVEL_HIDDEN,
		PSMI_ENVVAR_TYPE_UINT,
		(union psmi_envvar_val)PSM3_CPE_IGNORE,
		(union psmi_envvar_val)PSM3_CPE_REJECT,
		(union psmi_envvar_val)PSM3_CPE_OBEY,
		NULL,
		NULL,
		&val);

	if (!ret)
		psm3_cuda_permitted_enforcement = (psm3_cuda_permitted_enforcement_t)val.e_uint;
}

static void psm3_cuda_init_env_sync(void)
{
	union psmi_envvar_val val;

	int ret = psm3_getenv_range("PSM3_CUDA_SYNC",
		"Policy for setting synchroniation attributes.\n",
		"  0 CTX: attempt to set context-level SYNC_MEMOPS on CUDA 12.1 or better\n"
		"      otherwise, set pointer-level SYNC_MEMOPS\n"
		"  1 PTR: always set pointer-level SYNC_MEMOPS\n"
		"  2 PTR_RELAXED: always set pointer-level SYNC_MEMOPS,\n"
		"      but ignore 801 (not supported, expected for VMM allocs)\n"
		"  3 NONE: never set SYNC_MEMOPS\n",
		PSMI_ENVVAR_LEVEL_HIDDEN,
		PSMI_ENVVAR_TYPE_UINT,
		(union psmi_envvar_val)PSM3_CUDA_SYNC_CTX,
		(union psmi_envvar_val)PSM3_CUDA_SYNC_CTX,
		(union psmi_envvar_val)PSM3_CUDA_SYNC_NONE,
		NULL,
		NULL,
		&val);

	if (!ret)
		psm3_cuda_sync_mode = (psm3_cuda_sync_mode_t)val.e_uint;
}

static psm2_error_t psm3_cuda_initialize(void)
{
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");
	_HFI_DBG("Enabling CUDA support.\n");

	psm3_cuda_stats_register();

	err = psm3_cuda_lib_load();
	if (err != PSM2_OK)
		goto fail;

	PSM3_CUDA_CALL(cuInit, 0);

	PSM3_CUDA_CALL(cudaRuntimeGetVersion, &psm3_cuda_runtime_ver);

#ifdef PSM_HAVE_RNDV_MOD
	psm3_cuda_get_bars();
#endif
	if (! psm3_gpu_thresh_rndv) // sockets HAL could set new default
		psm3_gpu_thresh_rndv = PSM3_CUDA_GPU_THRESH_RNDV;
	psm3_gpu_rndv_nic_window_default = PSM3_CUDA_RNDV_NIC_WINDOW_DEFAULT;
	psm3_gpu_gpudirect_rdma_send_limit_default = PSM3_CUDA_GPUDIRECT_RDMA_SEND_LIMIT_DEFAULT;
	psm3_gpu_gpudirect_rdma_recv_limit_default = PSM3_CUDA_GPUDIRECT_RDMA_RECV_LIMIT_DEFAULT;
	psm3_gpu_mq_rndv_shm_gpu_thresh_default = PSM3_CUDA_MQ_RNDV_SHM_GPU_THRESH;

	psm3_cuda_init_env_cpe();
	psm3_cuda_init_env_sync();

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, "Unable to initialize PSM3 CUDA support.\n");
	return err;
}

static void psm3_cuda_finalize(void)
{
	psm3_stats_deregister_type(PSMI_STATSTYPE_GPU, &PSM3_CUDA_SYM_COUNT(cuInit));
}

static void psm3_cuda_ep_open(void)
{
	// nothing to do
}

static void psm3_cuda_ep_close(void)
{
	// nothing to do
}

static void psm3_cuda_identify(char *accel_vers, size_t size)
{
	char cudart_ver[64] = "unknown";
	if (psm3_cuda_runtime_ver)
		snprintf(cudart_ver, sizeof(cudart_ver), "%d.%d",
			psm3_cuda_runtime_ver / 1000, (psm3_cuda_runtime_ver % 1000) / 10);
	snprintf(accel_vers, size, "%s %s CUDA Runtime %s built against interface %d.%d\n",
		psm3_get_mylabel(), psm3_ident_tag,
		cudart_ver, CUDA_VERSION / 1000, (CUDA_VERSION % 1000) / 10);
}

static int psm3_cuda_p2p_supported()
{
	static int p2p_supported = -1; // -1 indicates "unset"
	if (likely(p2p_supported > -1))
		return p2p_supported;

	p2p_supported = 0;

	/* Check which devices the current device has p2p access to. */
	CUdevice  current_device;
	CUcontext current_context;
	int num_devices, dev_idx;
	PSM3_CUDA_CALL(cuDeviceGetCount, &num_devices);

	if (num_devices > 1) {
		PSM3_CUDA_CALL(cuCtxGetCurrent, &current_context);
		if (current_context == NULL) {
			_HFI_INFO("Unable to find active CUDA context, assuming P2P not supported\n");
			return 0;
		}
		PSM3_CUDA_CALL(cuCtxGetDevice, &current_device);
	}

	for (dev_idx = 0; dev_idx < num_devices; dev_idx++) {
		CUdevice device;
		PSM3_CUDA_CALL(cuDeviceGet, &device, dev_idx);

		if (num_devices > 1 && device != current_device) {
			int canAccessPeer = 0;
			PSM3_CUDA_CALL(cuDeviceCanAccessPeer, &canAccessPeer,
					current_device, device);

			if (canAccessPeer != 1)
				_HFI_DBG("CUDA device %d does not support P2P from current device (Non-fatal error)\n", dev_idx);
			else
				p2p_supported |= (1 << dev_idx);
		} else {
			/* Always support p2p on the same GPU */
			psm3_my_gpu_device = dev_idx;
			p2p_supported |= (1 << dev_idx);
		}
	}

	_HFI_DBG("returning (0x%x), device 0x%x (%d)\n", p2p_supported, (1 << psm3_my_gpu_device), psm3_my_gpu_device);
	return p2p_supported;
}

static int psm3_cuda_gpudirect_supported()
{
	static int device_support_gpudirect = -1; // -1 indicates unset
	
	if (likely(device_support_gpudirect > -1)) return device_support_gpudirect;

	int num_devices, dev;

	/* Check if all devices support GPU Direct RDMA based on version. */
	PSM3_CUDA_CALL(cuDeviceGetCount, &num_devices);

	device_support_gpudirect = 1;

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		PSM3_CUDA_CALL(cuDeviceGet, &device, dev);

		int major;
		PSM3_CUDA_CALL(cuDeviceGetAttribute, &major,
				CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device);
		if (major < 3) {
			device_support_gpudirect = 0;
			_HFI_INFO("CUDA device %d does not support GPUDirect RDMA (Non-fatal error)\n", dev);
		}
	}

	return device_support_gpudirect;
}

static void psm3_cuda_using_rv_for_mrs(void)
{
	// nothing to do
}

static void psm3_cuda_verify_GPU_capabilities(void)
{
	static int device_support_unified_addr = -1; // -1 indicates "unchecked"
	// we confirm the GPU supports unified addressing since this
	// allows a GPU address alone to be sufficient to identify the GPU device
	if (likely(device_support_unified_addr > -1)) return;

	int num_devices, dev;

	/* Check if all devices support Unified Virtual Addressing. */
	PSM3_CUDA_CALL(cuDeviceGetCount, &num_devices);

	device_support_unified_addr = 1;

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		PSM3_CUDA_CALL(cuDeviceGet, &device, dev);
		int unifiedAddressing;
		PSM3_CUDA_CALL(cuDeviceGetAttribute,
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

static void psm3_cuda_get_pci_addr(uint32_t *domain_p, uint32_t *bus_p,
							uint32_t *dev_p, uint32_t *func_p)
{
	int domain, bus, dev;
	int num_devices;
	CUdevice device;

	PSM3_CUDA_CALL(cuDeviceGetCount, &num_devices);
		_HFI_DBG("%d Cuda GPUs found\n", num_devices);
	if (! num_devices)
		return;

	if (num_devices == 1) {
		PSM3_CUDA_CALL(cuDeviceGet, &device, 0);
	} else {
		// all GPUs will be visible to process, see if app chose one first
		CUcontext ctxt = {0};
		if (! PSM3_CUDA_SYM_FP(cuCtxGetCurrent) || PSM3_CUDA_SYM_FP(cuCtxGetCurrent)(&ctxt) || ! ctxt) {
			_HFI_DBG("Unable to get Cuda ctxt\n");
			//PSM3_CUDA_CALL(cuDeviceGet, &device, 0);
			return;
		} else {
			PSM3_CUDA_CALL(cuCtxGetDevice, &device);
		}
	}
	_HFI_DBG("Using Cuda GPU %d\n", device);
	PSM3_CUDA_CALL(cuDeviceGetAttribute,
							&domain, CU_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID, device);
	PSM3_CUDA_CALL(cuDeviceGetAttribute,
							&bus, CU_DEVICE_ATTRIBUTE_PCI_BUS_ID, device);
	PSM3_CUDA_CALL(cuDeviceGetAttribute,
							&dev, CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID, device);
	*domain_p = domain;
	*bus_p = bus;
	*dev_p = dev;
	*func_p = 0;
}

#ifdef PSM_HAVE_RNDV_MOD
// The second BAR address is where the GPU will map GPUDirect memory.
// The beginning of this BAR is reserved for non-GPUDirect uses.
// However, it has been observed that in some multi-process
// pinning failures, HED-2035, the nvidia_p2p_get_pages can foul up
// it's IOMMU after which the next successful pin will incorrectly
// return the 1st physical address of the BAR for the pinned pages.
// In this case it will report this same physical address for other GPU virtual
// addresses and cause RDMA to use the wrong memory.
// As a workaround, we gather the Region 1 BAR address start for each
// GPU and if we see this address returned as the phys_addr of a mmapped
// GPUDirect Copy or the iova of a GPU MR we fail the job before it can
// corrupt any more application data.
static uint64_t psm3_cuda_get_nvidia_bar_addr(int domain, int bus, int slot)
{
	char sysfs[100];
	int ret;
	FILE *f;
	unsigned long long start_addr, end_addr, bar_size;

	ret = snprintf(sysfs, sizeof(sysfs),
		"/sys/class/pci_bus/%04x:%02x/device/%04x:%02x:%02x.0/resource",
		domain, bus, domain, bus, slot);
	psmi_assert_always(ret < sizeof(sysfs));
	f = fopen(sysfs, "r");
	if (! f) {
		if (psm3_cuda_gpu_pin_check) {
			_HFI_ERROR("Unable to open %s for GPU BAR Address: %s\n",
				sysfs, strerror(errno));
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to get GPU BAR address\n");
		}
		return 0;
	}
	// for each BAR region, start, end and flags are listed in hex
	// nVidia uses the 2nd BAR region (aka Region #1) to map peer to peer
	// accesses into it's potentially larger GPU local memory space
	ret = fscanf(f, "%*x %*x %*x %llx %llx", &start_addr, &end_addr);
	if (ret != 2) {
		if (psm3_cuda_gpu_pin_check) {
			_HFI_ERROR("Unable to get GPU BAR Address from %s: %s\n",
				sysfs, strerror(errno));
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to get GPU BAR address\n");
		}
		fclose(f);
		return 0;
	}
	fclose(f);

	bar_size = (end_addr - start_addr) + 1;
	_HFI_DBG("GPU BAR Addr from %s is 0x%llx - 0x%llx (size 0x%llx)\n", sysfs, start_addr, end_addr, bar_size);
	if (! psm3_cuda_min_gpu_bar_size || bar_size < psm3_cuda_min_gpu_bar_size)
		psm3_cuda_min_gpu_bar_size = bar_size;
	return start_addr;
}

static uint64_t psm3_cuda_min_bar_size(void)
{
	// for ONEAPI can return 0 for now, implement later
	return psm3_cuda_min_gpu_bar_size;
}

static psm2_error_t psm3_cuda_check_phys_addr(uint64_t phys_addr)
{
	int i;
	for (i=0; i < psm3_cuda_num_gpu_bars; i++) {
		if (phys_addr == psm3_cuda_gpu_bars[i]) {
			_HFI_ERROR("Incorrect Physical Address (0x%"PRIx64") returned by nVidia driver.  PSM3 exiting to avoid data corruption.  Job may be rerun with PSM3_GPUDIRECT=0 to avoid this issue.\n",
				phys_addr);
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Incorrect Physical Address returned by nVidia driver\n");
			psmi_assert_always(0);
			return PSM2_INTERNAL_ERR;
		}
	}
	return PSM2_OK;
}

static void psm3_cuda_roundup_gdrcopy(unsigned long buf, size_t size,
							uintptr_t *pageaddr_p, uint64_t *pagelen_p)
{
	*pageaddr_p = buf & GPU_PAGE_MASK;
	*pagelen_p = (uint64_t) (PSMI_GPU_PAGESIZE +
						((buf + size - 1) & GPU_PAGE_MASK) - *pageaddr_p);
}

#ifdef PSM_HAVE_REG_MR
static void psm3_cuda_roundup_rv_reg_mr(struct psm2_ep *ep,
							void **addr_p, uint64_t *length_p, int access)
{
	uint64_t addr_in = (uint64_t)*addr_p;

	*addr_p   = (void *)ROUNDDOWN64P2(addr_in, PSMI_GPU_PAGESIZE);
	*length_p = ROUNDUP64P2(addr_in + *length_p, PSMI_GPU_PAGESIZE) - (uint64_t)*addr_p;
}

// add Cuda specific information to the mparams in prep for the
// RV_IOCTL_REG_MEM ioctl to rv
// For Cuda, no additional information is needed
static int psm3_cuda_init_rv_reg_mr_params(
				void *addr, uint64_t length, int access,
				struct rv_mem_params *mparams,
				union psm3_verbs_mr_gpu_specific *gpu_specific,
				union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad)
{
	// nothing to do
	return 0;
}
#endif // PSM_HAVE_REG_MR

// add Cuda specific information to the params in prep for the
// RV_IOCTL_PIN_MMAP ioctl to rv
// For Cuda, no additional information is needed
static int psm3_cuda_init_rv_pin_mmap_params(
				void *addr, uint64_t length, int access,
				struct rv_gpu_mem_params *params,
				union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad)
{
	// nothing to do
	return 0;
}

// cleanup Cuda specific scratchpad from
// psm3_cuda_init_rv_reg_mr_params or
// psm3_cuda_init_rv_pin_mmap_params
// called on success or error path, makes sure not to polute errno
// as it can reflect the earlier error for the error path in caller.
static void psm3_cuda_rv_reg_mmap_cleanup(
				void *addr, uint64_t length, int access,
				union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad)
{
	// nothing to do
}
#endif /* PSM_HAVE_RNDV_MOD */

#ifdef PSM_HAVE_REG_MR
// compare GPU specific fields in verbs MR cache entry
static int psm3_cuda_cmp_mr(const union psm3_verbs_mr_gpu_specific *a,
		const union psm3_verbs_mr_gpu_specific *b)
{
	// nothing to do
	return 0;
}

// initialize GPU specific fields in verbs MR cache entry
static void psm3_cuda_init_mr(void *addr, uint64_t length, int access,
				union psm3_verbs_mr_gpu_specific *gpu_specific)
{
	// nothing to do
}
#endif /* PSM_HAVE_REG_MR */

static void psm3_cuda_fetch_ctxt(void)
{
	PSM3_CUDA_CALL(cuCtxGetCurrent, &psm3_cu_ctxt);
}

// ensure psm3_cu_ctxt reflects our most recent psm3_cu_ctxt
static void psm3_cuda_refresh_ctxt(void)
{
	if (psm3_cu_ctxt)
		PSM3_CUDA_CALL(cuCtxSetCurrent, psm3_cu_ctxt);
}

static void psm3_cuda_register_hostmem(void *buf, uint32_t size)
{
#ifndef PSM3_NO_CUDA_REGISTER
	// By registering memory with Cuda, we make
	// cuMemcpy run faster for copies
	if (psm3_cuda_check_have_cuda_ctxt()) {
		PSM3_CUDA_CALL(cuMemHostRegister,
					buf, size, CU_MEMHOSTALLOC_PORTABLE);
	}
#endif
}

static void psm3_cuda_unregister_hostmem(void *buf)
{
#ifndef PSM3_NO_CUDA_REGISTER
	if (psm3_cu_ctxt) {
		/* ignore NOT_REGISTERED in case cuda initialized late */
		/* ignore other errors as context could be destroyed before this */
		CUresult cudaerr = PSM3_CUDA_EXEC_ASSUME_CONTEXT(cuMemHostUnregister, buf);
		if (cudaerr)
			PSM3_CUDA_ERROR(cuMemHostUnregister, cudaerr, DBG);
	}
#endif
}

static int psm3_cuda_is_gpu_mem(const void *ptr)
{
	CUresult cudaerr;
	CUpointer_attribute attrs[] = {
		CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
		CU_POINTER_ATTRIBUTE_IS_MANAGED,
	};
	CUmemorytype mt = 0;
	uint64_t managed = 0;
	void *resp[] = { &mt, &managed };

	static_assert(PSMI_HOWMANY(attrs) == PSMI_HOWMANY(resp),
		"attribute count must equal response count");

	cudaerr = PSM3_CUDA_SYM_FP(cuPointerGetAttributes)(
		PSMI_HOWMANY(attrs), attrs, resp, (CUdeviceptr)ptr);
	PSM3_CUDA_SYM_COUNT(cuPointerGetAttributes) += 1;
	return cudaerr == CUDA_SUCCESS && mt == CU_MEMORYTYPE_DEVICE && !managed;
}

static void psm3_cuda_prepare_HtoD_memcpys(struct ips_protoexp *protoexp)
{
	protoexp->gpu_specific.cudastream_recv = NULL;
}

static void psm3_cuda_prepare_DtoH_memcpys(struct ips_proto *proto)
{
	proto->gpu_specific.cudastream_send = NULL;
}

static void psm3_cuda_shutdown_HtoD_memcpys(struct ips_protoexp *protoexp)
{
	if (protoexp->gpu_specific.cudastream_recv != NULL) {
			PSM3_CUDA_CALL(cuStreamDestroy, protoexp->gpu_specific.cudastream_recv);
	}
}

static void psm3_cuda_shutdown_DtoH_memcpys(struct ips_proto *proto)
{
	if (proto->gpu_specific.cudastream_send) {
		PSM3_CUDA_CALL(cuStreamDestroy, proto->gpu_specific.cudastream_send);
	}
}

static void psm3_cuda_memcpy_HtoD_start(struct ips_protoexp *protoexp,
								struct ips_gpu_hostbuf *ghb, uint32_t len)
{
	if (protoexp->gpu_specific.cudastream_recv == NULL) {
		PSM3_CUDA_CALL(cuStreamCreate, &protoexp->gpu_specific.cudastream_recv,
						CU_STREAM_NON_BLOCKING);
	}
	PSM3_CUDA_CALL(cuMemcpyHtoDAsync, (CUdeviceptr)ghb->gpu_buf, ghb->host_buf,
						len, protoexp->gpu_specific.cudastream_recv);
	if (ghb->gpu_specific.cuda_copy_status == NULL) {
		PSM3_CUDA_CALL(cuEventCreate, &ghb->gpu_specific.cuda_copy_status, CU_EVENT_DEFAULT);
	}
	PSM3_CUDA_CALL(cuEventRecord, ghb->gpu_specific.cuda_copy_status, protoexp->gpu_specific.cudastream_recv);
}

static void psm3_cuda_memcpy_DtoH_start(struct ips_proto *proto,
								struct ips_gpu_hostbuf *ghb, uint32_t len)
{
	if (proto->gpu_specific.cudastream_send == NULL) {
		PSM3_CUDA_CALL(cuStreamCreate, &proto->gpu_specific.cudastream_send,
						CU_STREAM_NON_BLOCKING);
	}
	if (ghb->gpu_specific.cuda_copy_status == NULL) {
		PSM3_CUDA_CALL(cuEventCreate, &ghb->gpu_specific.cuda_copy_status, CU_EVENT_DEFAULT);
	}
	PSM3_CUDA_CALL(cuMemcpyDtoHAsync, ghb->host_buf, (CUdeviceptr)ghb->gpu_buf,
					len, proto->gpu_specific.cudastream_send);
	PSM3_CUDA_CALL(cuEventRecord, ghb->gpu_specific.cuda_copy_status, proto->gpu_specific.cudastream_send);
}

static int psm3_cuda_memcpy_done(struct ips_gpu_hostbuf *ghb)
{
	CUresult status;
	PSM3_CUDA_CHECK_EVENT(ghb->gpu_specific.cuda_copy_status, status);
	return (status == CUDA_SUCCESS);
}

static void psm3_cuda_hostbuf_lazy_init(struct ips_gpu_hostbuf *ghb)
{
	ghb->gpu_specific.cuda_copy_status = NULL;
}

static void psm3_cuda_hostbuf_reset(struct ips_gpu_hostbuf *ghb)
{
	// nothing to do
}

static void psm3_cuda_hostbuf_destroy(struct ips_gpu_hostbuf *ghb)
{
	if (ghb->gpu_specific.cuda_copy_status != NULL) {
		PSM3_CUDA_CALL(cuEventDestroy, ghb->gpu_specific.cuda_copy_status);
	}
	if (ghb->host_buf != NULL) {
		PSM3_CUDA_CALL(cuMemFreeHost, ghb->host_buf);
	}
}

static void psm3_cuda_memcpy_DtoD(void *dstptr, const void *srcptr, uint32_t len)
{
	PSM3_CUDA_CALL(cuMemcpyDtoD, (CUdeviceptr)dstptr, (CUdeviceptr)srcptr, len);
}

static void psm3_cuda_memcpy_HtoD(void *dstptr, const void *srcptr, uint32_t len)
{
	PSM3_CUDA_CALL(cuMemcpyHtoD, (CUdeviceptr)dstptr, srcptr, len);
}

static void psm3_cuda_memcpy_DtoH(void *dstptr, const void *srcptr, uint32_t len)
{
	PSM3_CUDA_CALL(cuMemcpyDtoH, dstptr, (CUdeviceptr)srcptr, len);
}

static void psm3_cuda_memcpy(void *dstptr, const void *srcptr, uint32_t len)
{
	PSM3_CUDA_CALL(cuMemcpy, (CUdeviceptr)dstptr, (CUdeviceptr)srcptr, len);
}

static void psm3_cuda_synchronize_memcpy(void)
{
	PSM3_CUDA_CALL(cuStreamSynchronize, 0);
}

/*
 * CUDA documentation dictates the use of SYNC_MEMOPS attribute when the buffer
 * pointer received into PSM has been allocated by the application and is the
 * target of GPUDirect DMA operations.
 *
 * Normally, CUDA is permitted to implicitly execute synchronous memory
 * operations as asynchronous operations, relying on commands arriving via CUDA
 * for proper sequencing.  GDR, however, bypasses CUDA, enabling races, e.g.
 * cuMemcpy sequenced before a GDR operation.
 *
 * SYNC_MEMOPS avoids this optimization.
 *
 * Note that allocations via the "VMM" API, i.e. cuMemCreate, do not support the
 * SYNC_MEMOPS pointer attribute, and will return 801 (not supported). If we're
 * using the newer context-level sync flag available in CUDA 12.1+ to avoid this
 * issue, we will not set the pointer-level sync flag here.
 */
static void psm3_cuda_mark_buf_synchronous(const void *buf)
{
	bool check_for_not_supported = false;

	switch (psm3_cuda_sync_mode) {
	case PSM3_CUDA_SYNC_CTX:
#if PSM3_CUDA_HAVE_CTX_SYNC_MEMOPS
		// sync set at the context-level; nothing to do here
		return;
#else
		// otherwise, intentional fall through to PTR behavior
#endif
	case PSM3_CUDA_SYNC_PTR:
		// pointer level sync, handling all errors
		break;
	case PSM3_CUDA_SYNC_PTR_RELAXED:
		// pointer level sync, ignoring not supported
		check_for_not_supported = true;
		break;
	case PSM3_CUDA_SYNC_NONE:
		return;
	}

	CUresult cudaerr;
	int true_flag = 1;

	cudaerr = PSM3_CUDA_EXEC(cuPointerSetAttribute,
		&true_flag, CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, (CUdeviceptr)buf);

	if_pf (check_for_not_supported && cudaerr == CUDA_ERROR_NOT_SUPPORTED) {
#ifdef PSM_DEBUG
		// query the handle just to be sure it is in fact a VMM alloc
		CUmemGenericAllocationHandle h;
		PSM3_CUDA_CALL(cuMemRetainAllocationHandle, &h, (CUdeviceptr)buf);
		PSM3_CUDA_CALL(cuMemRelease, h);
#endif
		return;
	}

	PSM3_CUDA_CHECK(cuPointerSetAttribute, cudaerr);
	return;
}

static void psm3_cuda_host_alloc(void **ret_ptr, uint32_t size)
{
	PSM3_CUDA_CALL(cuMemHostAlloc, (void **)ret_ptr, size,
					CU_MEMHOSTALLOC_PORTABLE);
}

static void psm3_cuda_host_free(void *ptr)
{
	PSM3_CUDA_CALL(cuMemFreeHost, (void *)ptr);
}

static int psm3_cuda_gpu_addr_send_mr(struct psm2_mq_req *mqreq)
{
	return mqreq->is_buf_gpu_mem && ! mqreq->gpu_hostbuf_used;
}

static int psm3_cuda_gpu_addr_recv_mr(struct ips_tid_recv_desc *tidrecvc,
							int gpu_hostbuf_used)
{
	return tidrecvc->is_ptr_gpu_backed;
}

//***************************************************************************
//cuda support for PSM3_DEVICES "shm", via an IPC handle cache and Cuda IPC
//In platforms with NVLINK between GPUs, Cuda IPC will use NVLINK.

#define CUDA_MEMHANDLE_CACHE_SIZE 64

/*
 * rbtree cruft
 */
struct _cl_map_item;

typedef struct
{
	unsigned long			start;			/* start virtual address */
	CUipcMemHandle			cuda_ipc_handle; /* cuda ipc mem handle */
	CUdeviceptr				cuda_ipc_dev_ptr;/* Cuda device pointer */
	psm2_epid_t				epid;
	struct _cl_map_item*	i_prev;			/* idle queue previous */
	struct _cl_map_item*	i_next;			/* idle queue next */
}__attribute__ ((aligned (128))) psm3_rbtree_cuda_memhandle_cache_mapitem_pl_t;

typedef struct {
	uint32_t		nelems;	/* number of elements in the cache */
} psm3_rbtree_cuda_memhandle_cache_map_pl_t;

/*
 * Custom comparator
 */
typedef psm3_rbtree_cuda_memhandle_cache_mapitem_pl_t psm3_cuda_cache_item;

static int psm3_cuda_cache_key_cmp(const psm3_cuda_cache_item *a, const psm3_cuda_cache_item *b)
{
	// we use epid as part of cache key so multi-ep and multi-process jobs
	// can have a better cache hit rate.  In some cases we may end up with
	// cache entries for the same buffer with different epid's all within the
	// same multi-ep rank, but this does no harm other than to waste some
	// cache space.  By including epid in key_cmp we have a chance to have
	// separate cache entries for the same sbuf address in different
	// sender's GPU virtual address space.
	switch (psm3_epid_cmp_internal(a->epid, b->epid)) {
	case -1: return -1;
	case 1: return 1;
	default:
		break;
	}

	// The sender has used cuMemGetAddressRange to normalize the address
	// so we can simply compare the start address of the allocation.
	// Note cuIpcOpenMemHandle only needs the start address as well, so we
	// ignore length
	if (a->start < b->start)
		return -1;
	if (b->start < a->start)
		return 1;

	return 0;
}


/*
 * Necessary rbtree cruft
 */
#define RBTREE_MI_PL    psm3_rbtree_cuda_memhandle_cache_mapitem_pl_t
#define RBTREE_MAP_PL   psm3_rbtree_cuda_memhandle_cache_map_pl_t
#define RBTREE_CMP(a, b, c) psm3_cuda_cache_key_cmp((a), (b))
#define RBTREE_ASSERT   psmi_assert
#define RBTREE_MAP_COUNT(PAYLOAD_PTR)   ((PAYLOAD_PTR)->nelems)
#define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR

#include "psm3_rbtree.h"
#include "psm3_rbtree.c"

/*
 * Convenience rbtree cruft
 */
#define NELEMS(cache)	((cache)->map.payload.nelems)

#define IHEAD(cache)	((cache)->map.root)
#define LAST(cache)	(IHEAD(cache)->payload.i_prev)
#define FIRST(cache)	(IHEAD(cache)->payload.i_next)
#define INEXT(x)	((x)->payload.i_next)
#define IPREV(x)	((x)->payload.i_prev)

/*
 * Actual module data
 */
struct psm3_cuda_memhandle_cache {
	cl_qmap_t map;
	mpool_t mpool;
	uint32_t size;
	psm2_mq_stats_t *stats;
};
typedef struct psm3_cuda_memhandle_cache *psm3_cuda_memhandle_cache_t;

static void psm3_print_cuda_memhandle_cache_stats(psm2_mq_stats_t *stats)
{
	_HFI_DBG("limit=%lu,maxelems=%lu,hit=%lu,miss=%lu,evict=%lu,remove=%lu,clear=%lu\n",
		stats->gpu_ipc_cache_limit, stats->gpu_ipc_cache_max_nelems,
		stats->gpu_ipc_cache_hit, stats->gpu_ipc_cache_miss,
		stats->gpu_ipc_cache_evict, stats->gpu_ipc_cache_remove,
		stats->gpu_ipc_cache_clear);
}

/*
 * This is the callback function when mempool are resized or destroyed.
 * Upon calling cache fini mpool is detroyed which in turn calls this callback
 * which helps in closing all memhandles.
 */
static void
psm3_cuda_memhandle_cache_alloc_func(int is_alloc, void* context, void* obj)
{
	cl_map_item_t* memcache_item = (cl_map_item_t*)obj;
	if (!is_alloc) {
		if(memcache_item->payload.start)
			PSM3_CUDA_CALL(cuIpcCloseMemHandle,
						memcache_item->payload.cuda_ipc_dev_ptr);
	}
}

/*
 * Creating mempool for cuda memhandle cache nodes.
 */
static psm2_error_t
psm3_cuda_memhandle_mpool_alloc(psm3_cuda_memhandle_cache_t cache,
							uint32_t memcache_size)
{
	psm2_error_t err;
	if (memcache_size < 1)
		return PSM2_PARAM_ERR;

	cache->size = memcache_size;
	/* Creating a memory pool of size PSM3_CUDA_MEMCACHE_SIZE
	 * which includes the Root and NIL items
	 */
	cache->mpool = psm3_mpool_create_for_gpu(sizeof(cl_map_item_t),
					cache->size,
					cache->size, 0,
					UNDEFINED, NULL, NULL,
					psm3_cuda_memhandle_cache_alloc_func,
					NULL);
	if (cache->mpool == NULL) {
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				"Couldn't allocate CUDA host receive buffer pool");
		return err;
	}
	return PSM2_OK;
}

/*
 * allocate and initialize memhandle cache
 * including rbtree.
 */
static psm2_error_t psm3_cuda_memhandle_cache_alloc(
				psm3_cuda_memhandle_cache_t *cachep, uint32_t memcache_size,
				psm2_mq_stats_t *stats)
{
	cl_map_item_t *root = NULL, *nil_item = NULL;

	*cachep = (psm3_cuda_memhandle_cache_t)psmi_calloc(
					NULL, UNDEFINED, 1, sizeof(**cachep));
	if (! *cachep)
		return PSM2_NO_MEMORY;

	psm2_error_t err = psm3_cuda_memhandle_mpool_alloc(*cachep, memcache_size);
	if (err != PSM2_OK)
		goto fail;

	root = (cl_map_item_t *)psmi_calloc(NULL, UNDEFINED, 1, sizeof(cl_map_item_t));
	if (root == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}
	nil_item = (cl_map_item_t *)psmi_calloc(NULL, UNDEFINED, 1, sizeof(cl_map_item_t));
	if (nil_item == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}

	nil_item->payload.start = 0;
	nil_item->payload.epid = psm3_epid_zeroed_internal();
	ips_cl_qmap_init(&(*cachep)->map,root,nil_item);
	NELEMS(*cachep) = 0;

	(*cachep)->stats = stats;

	stats->gpu_ipc_cache_limit = memcache_size;
	stats->gpu_ipc_cache_nelems = 0;
	stats->gpu_ipc_cache_max_nelems = 0;
	stats->gpu_ipc_cache_hit = 0;
	stats->gpu_ipc_cache_miss = 0;
	stats->gpu_ipc_cache_evict = 0;
	stats->gpu_ipc_cache_remove = 0;
	stats->gpu_ipc_cache_clear = 0;

	return PSM2_OK;

fail:
	if (nil_item)
		psmi_free(nil_item);
	if (root)
		psmi_free(root);
	if ((*cachep)->mpool)
		psm3_mpool_destroy((*cachep)->mpool);
	psmi_free(*cachep);
	return err;
}

static void psm3_cuda_memhandle_cache_free(psm3_cuda_memhandle_cache_t cache)
{
	psm3_print_cuda_memhandle_cache_stats(cache->stats);

	if (cache->map.nil_item)
		psmi_free(cache->map.nil_item);
	if (cache->map.root)
		psmi_free(cache->map.root);
	if (cache->mpool)
		psm3_mpool_destroy(cache->mpool);
	psmi_free(cache);
}

/*
 * Insert at the head of Idleq.
 */
static void
psm3_cuda_idleq_insert(psm3_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (FIRST(cache) == NULL) {
		FIRST(cache) = memcache_item;
		LAST(cache) = memcache_item;
		return;
	}
	INEXT(FIRST(cache)) = memcache_item;
	IPREV(memcache_item) = FIRST(cache);
	FIRST(cache) = memcache_item;
	INEXT(FIRST(cache)) = NULL;
	return;
}

/*
 * Remove least recent used element.
 */
static void
psm3_cuda_idleq_remove_last(psm3_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (!INEXT(memcache_item)) {
		LAST(cache) = NULL;
		FIRST(cache) = NULL;
	} else {
		LAST(cache) = INEXT(memcache_item);
		IPREV(LAST(cache)) = NULL;
	}
	// Null-out now-removed memcache_item's next and prev pointers out of
	// an abundance of caution
	INEXT(memcache_item) = IPREV(memcache_item) = NULL;
}

static void
psm3_cuda_idleq_remove(psm3_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (LAST(cache) == memcache_item) {
		psm3_cuda_idleq_remove_last(cache, memcache_item);
	} else if (FIRST(cache) == memcache_item) {
		FIRST(cache) = IPREV(memcache_item);
		INEXT(FIRST(cache)) = NULL;
	} else {
		INEXT(IPREV(memcache_item)) = INEXT(memcache_item);
		IPREV(INEXT(memcache_item)) = IPREV(memcache_item);
	}
	// Null-out now-removed memcache_item's next and prev pointers out of
	// an abundance of caution
	INEXT(memcache_item) = IPREV(memcache_item) = NULL;
}

static void
psm3_cuda_idleq_reorder(psm3_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (FIRST(cache) == memcache_item && LAST(cache) == memcache_item ) {
		return;
	}
	psm3_cuda_idleq_remove(cache, memcache_item);
	psm3_cuda_idleq_insert(cache, memcache_item);
	return;
}

/*
 * After a successful cache hit, item is validated by doing a
 * memcmp on the handle stored and the handle we receive from the
 * sender. If the validation fails the item is removed from the idleq,
 * the rbtree, is put back into the mpool and cuIpcCloseMemHandle function
 * is called.
 * Cuda ipcMemHandles for distinct allocations are unique, even if the
 * allocation was at the same address.  So this check catches stale cache
 * entries.
 */
static psm2_error_t
psm3_cuda_memhandle_cache_validate(psm3_cuda_memhandle_cache_t cache,
				cl_map_item_t* memcache_item,
				uintptr_t sbuf, CUipcMemHandle* handle,
				psm2_epid_t epid)
{
	psmi_assert(!psm3_epid_cmp_internal(epid, memcache_item->payload.epid));
	psmi_assert(sbuf == memcache_item->payload.start);
	if (0 == memcmp(handle, &memcache_item->payload.cuda_ipc_handle,
			sizeof(CUipcMemHandle))) {
		return PSM2_OK;
	}
	_HFI_DBG("cache collision: new entry start=%lu\n", sbuf);

	cache->stats->gpu_ipc_cache_remove++;
	ips_cl_qmap_remove_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems--;
	PSM3_CUDA_CALL(cuIpcCloseMemHandle,
				memcache_item->payload.cuda_ipc_dev_ptr);
	psm3_cuda_idleq_remove(cache, memcache_item);
	memset(memcache_item, 0, sizeof(*memcache_item));
	psm3_mpool_put(memcache_item);
	return PSM2_OK_NO_PROGRESS;
}

/*
 * Current eviction policy: Least Recently Used.
 */
static void
psm3_cuda_memhandle_cache_evict(psm3_cuda_memhandle_cache_t cache)
{
	cache->stats->gpu_ipc_cache_evict++;
	cl_map_item_t *p_item = LAST(cache);
	_HFI_VDBG("Removing (epid=%s,start=%lu,dev_ptr=0x%llX,it=%p) from cuda_memhandle_cachemap.\n",
			psm3_epid_fmt_internal(p_item->payload.epid, 0), p_item->payload.start,
			p_item->payload.cuda_ipc_dev_ptr, p_item);
	ips_cl_qmap_remove_item(&cache->map, p_item);
	cache->stats->gpu_ipc_cache_nelems--;
	PSM3_CUDA_CALL(cuIpcCloseMemHandle, p_item->payload.cuda_ipc_dev_ptr);
	psm3_cuda_idleq_remove_last(cache, p_item);
	memset(p_item, 0, sizeof(*p_item));
	psm3_mpool_put(p_item);
}

static psm2_error_t
psm3_cuda_memhandle_cache_register(psm3_cuda_memhandle_cache_t cache,
				uintptr_t sbuf, CUipcMemHandle* handle,
				psm2_epid_t epid,
				CUdeviceptr cuda_ipc_dev_ptr)
{
	if (NELEMS(cache) == cache->size)
		psm3_cuda_memhandle_cache_evict(cache);

	cl_map_item_t* memcache_item = psm3_mpool_get(cache->mpool);
	/* memcache_item cannot be NULL as we evict
	 * before the call to mpool_get. Check has
	 * been fixed to help with klockwork analysis.
	 */
	if (memcache_item == NULL)
		return PSM2_NO_MEMORY;
	memcache_item->payload.start = sbuf;
	memcache_item->payload.cuda_ipc_handle = *handle;
	memcache_item->payload.cuda_ipc_dev_ptr = cuda_ipc_dev_ptr;
	memcache_item->payload.epid = epid;
	ips_cl_qmap_insert_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems++;
	if (cache->stats->gpu_ipc_cache_nelems > cache->stats->gpu_ipc_cache_max_nelems)
		cache->stats->gpu_ipc_cache_max_nelems = cache->stats->gpu_ipc_cache_nelems;
	psm3_cuda_idleq_insert(cache, memcache_item);
	return PSM2_OK;
}

static void psm3_cuda_memhandle_cache_clear(psm3_cuda_memhandle_cache_t cache)
{
	_HFI_DBG("Closing all handles, clearing cuda_memhandle_cachemap and idleq. NELEMS=%u\n", NELEMS(cache));
	while (NELEMS(cache)) {
		psm3_cuda_memhandle_cache_evict(cache);
	}
	cache->stats->gpu_ipc_cache_clear++;
	_HFI_DBG("Closed all handles, cleared cuda_memhandle_cachemap and idleq. NELEMS=%u\n", NELEMS(cache));
}

/*
 * The key used to search the cache is the senders buf address pointer and
 * epid.  The sender will have used cuMemGetAddressRange
 * to find the start of the memory containing the buffer (supplied as sbuf).
 * Upon match, we must validate the entry we find and may need to replace it.
 */
static CUdeviceptr
psm3_cuda_memhandle_acquire(psm3_cuda_memhandle_cache_t cache,
				uintptr_t sbuf, CUipcMemHandle* handle,
				psm2_epid_t epid)
{
	_HFI_VDBG("sbuf=%lu,handle=%p,epid=%s\n",
			sbuf, handle, psm3_epid_fmt_internal(epid, 0));

	CUdeviceptr cuda_ipc_dev_ptr;
	if(! cache) {
		PSM3_CUDA_CALL(cuIpcOpenMemHandle, &cuda_ipc_dev_ptr,
				*handle, CU_IPC_MEM_LAZY_ENABLE_PEER_ACCESS);
		return cuda_ipc_dev_ptr;
	}

	psm3_cuda_cache_item key = {
		.start = (unsigned long) sbuf,
		.epid = epid
	};

	/*
	 * preconditions:
	 *  1) buffer [start,epid) may or may not be in cachemap already
	 *  2) there are no duplicate entries in cachemap
	 * postconditions:
	 *  1) buffer is in cachemap with same handle, epid
	 *  2) there are no duplicate entries in cachemap
	 *
	 * The key used to search the cache is the senders buf address pointer
	 * and epid.
	 * Upon a succesful hit in the cache, additional validation is required
	 * as the handle could be stale.
	 */
	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, &key);
	if (p_item->payload.start) {
		// confirm the entry for sbuf matches the handle and is not stale
		if (psm3_cuda_memhandle_cache_validate(cache, p_item, sbuf, handle, epid) == PSM2_OK) {
			cache->stats->gpu_ipc_cache_hit++;
			psm3_cuda_idleq_reorder(cache, p_item);
			return p_item->payload.cuda_ipc_dev_ptr;
		}

		// buffer found was stale psm3_cuda_memhandle_cache_validate()
		// closed and removed existing entry.
		// Should find no more duplicates
#ifdef PSM_DEBUG
		p_item = ips_cl_qmap_searchv(&cache->map, &key);
		psmi_assert(! p_item->payload.start);
#endif
	}
	cache->stats->gpu_ipc_cache_miss++;

	CUresult cudaerr = PSM3_CUDA_CALL_EXCEPT(
		CUDA_ERROR_ALREADY_MAPPED,
		cuIpcOpenMemHandle,
		&cuda_ipc_dev_ptr,
		*handle,
		CU_IPC_MEM_LAZY_ENABLE_PEER_ACCESS);

	if (cudaerr == CUDA_ERROR_ALREADY_MAPPED) {
		// remote memory already mapped. Close all handles, clear cache,
		// and try again
		psm3_cuda_memhandle_cache_clear(cache);
		PSM3_CUDA_CALL(cuIpcOpenMemHandle, &cuda_ipc_dev_ptr, *handle,
			CU_IPC_MEM_LAZY_ENABLE_PEER_ACCESS);
	}

	psm3_cuda_memhandle_cache_register(cache, sbuf, handle,
							epid, cuda_ipc_dev_ptr);
	return cuda_ipc_dev_ptr;
}

static void
psm3_cuda_memhandle_release(psm3_cuda_memhandle_cache_t cache,
						CUdeviceptr cuda_ipc_dev_ptr)
{
	if(! cache)
		PSM3_CUDA_CALL(cuIpcCloseMemHandle, cuda_ipc_dev_ptr);
	return;
}
// end of CUDA IPC MemHandle Cache
//***************************************************************************


// RTS and CTS processing functions for PSM3_DEVICES "shm" to pass
// Cuda IPC handles and permit use of NVLINK for intra-node transfers
static psm2_error_t psm3_cuda_shm_init(struct ptl_am *ptl,
								psm2_mq_stats_t *stats)
{
	// TBD - should we have generic names for these env variables
	// PSM3_GPU_MEMCACHE_ENABLED, PSM3_GPU_MEMCACHE_SIZE?
	union psmi_envvar_val env_memcache_enabled;

	psm3_getenv("PSM3_CUDA_MEMCACHE_ENABLED",
			"PSM cuda ipc memhandle cache enabled (default is enabled)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1, &env_memcache_enabled);
	if (env_memcache_enabled.e_uint) {
		union psmi_envvar_val env_memcache_size;

		psm3_getenv("PSM3_CUDA_MEMCACHE_SIZE",
				"Size of the cuda ipc memhandle cache ",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)CUDA_MEMHANDLE_CACHE_SIZE,
				&env_memcache_size);
		return psm3_cuda_memhandle_cache_alloc(
					(psm3_cuda_memhandle_cache_t*)&ptl->memhandle_cache,
					env_memcache_size.e_uint, stats);
	}
	return PSM2_OK;
}

static void psm3_cuda_shm_finalize(struct ptl_am *ptl)
{
	if (ptl->memhandle_cache)
		psm3_cuda_memhandle_cache_free((psm3_cuda_memhandle_cache_t)ptl->memhandle_cache);
	ptl->memhandle_cache = NULL;
	return;
}

static psm2_error_t psm3_cuda_shm_epaddr_add(struct ptl_am *ptl,
											struct am_epaddr *am_epaddr)
{
	// nothing to do
	return PSM2_OK;
}

static void psm3_cuda_shm_epaddr_free(struct am_epaddr *am_epaddr)
{
	// nothing to do
}

static int psm3_cuda_shm_dev_fds_needed()
{
	// don't need to exchange dev_fds
	return 0;
}

static void psm3_cuda_shm_dev_fds_send(struct ptl_am *ptl, struct am_epaddr *am_epaddr)
{
	// nothing to do
}

static psm2_error_t psm3_cuda_shm_dev_fds_connreq_poll(struct ptl_am *ptl, struct am_ptl_connection_req *req)
{
	// nothing to do
	return PSM2_OK;
}

static psm2_error_t psm3_cuda_shm_dev_fds_check_exchanged(struct ptl_am *ptl, struct am_ptl_connection_req *req, int index)
{
	// nothing to do
	return PSM2_OK;
}

static psm2_error_t psm3_cuda_shm_dev_fds_poll(struct ptl_am *ptl, psm2_error_t res)
{
	// nothing to do
	return res;
}

// On Sender, place the IPC handle in the RTS
// We put offset in the basic "args" parameters and the actual
// IPC handle as payload due to it's size
// Callers expect payload_size >0 when using GPU IPC and key off non-zero
// payload size in RTS to identify a GPU IPC RTS
// Save in the req the needed information about IPC resources allocated here
// so psm3_cuda_process_cts and release them.
static psm2_error_t psm3_cuda_shm_build_rts(struct ptl_am *ptl,
			psm2_mq_req_t req, int *narg_p,
			psm2_amarg_t *args, void **payload_p, size_t *payload_size_p,
			union am_gpu_rts_payload *info)
{
	CUdeviceptr buf_base_ptr;
	void *buf = req->req_data.buf;
	PSM3_CUDA_CALL(cuMemGetAddressRange, &buf_base_ptr, NULL, (CUdeviceptr)buf);

	/* Offset in GPU buffer from which we copy data, we have to
	 * send it separetly because this offset is lost
	 * when cuIpcGetMemHandle is called */
	req->gpu_specific.cuda_ipc_offset = (uint32_t)((uintptr_t)buf - (uintptr_t)buf_base_ptr);
	args[2].u32w0 = (uint32_t)req->gpu_specific.cuda_ipc_offset;

	PSM3_CUDA_CALL(cuIpcGetMemHandle, &req->gpu_specific.cuda_ipc_handle, (CUdeviceptr) buf);
	*narg_p = 5;
	*payload_p = (void*)&req->gpu_specific.cuda_ipc_handle;
	*payload_size_p = sizeof(CUipcMemHandle);
	req->gpu_specific.cuda_ipc_handle_attached = 1;
	return PSM2_OK;
}

// On receiver, pull IPC information out of the RTS which our peer build using
// psm3_cuda_shm_build_rts.  Information is saved to the req for subsequent
// processing after tag matching via psm3_cuda_shm_rtsmatch
static void psm3_cuda_shm_process_rts(psm2_mq_req_t req, void *buf, size_t len,
				int narg, psm2_amarg_t *args)
{
	req->gpu_specific.cuda_ipc_handle = *((CUipcMemHandle*)buf);
	psmi_assert(len == sizeof(CUipcMemHandle));
	req->gpu_specific.cuda_ipc_handle_attached = 1;
	req->gpu_specific.cuda_ipc_offset = args[2].u32w0;
}

// On receiver, use GPU IPC to copy data from the sender to this process
// This is called on the receiver after psm3_cuda_process_rts has parsed the
// incoming RTS and tag matching has matched the RTS with a receive buffer and
// populated the req with information about the matched receiver buffer
static int psm3_cuda_shm_rtsmatch(struct ptl_am *ptl, psm2_mq_req_t req)
{
	if (req->gpu_specific.cuda_ipc_handle_attached) {
		CUdeviceptr cuda_ipc_dev_ptr = psm3_cuda_memhandle_acquire(
								ptl->memhandle_cache,
								req->rts_sbuf - req->gpu_specific.cuda_ipc_offset,
								(CUipcMemHandle*)&req->gpu_specific.cuda_ipc_handle,
								req->rts_peer->epid);
		cuda_ipc_dev_ptr = cuda_ipc_dev_ptr + req->gpu_specific.cuda_ipc_offset;
		/* cuMemcpy into the receive side buffer
		 * based on its location */
		if (req->is_buf_gpu_mem) {
			/*PSM3_GPU_MEMCPY_DTOD*/
			psm3_cuda_memcpy_DtoD(req->req_data.buf, (void*)cuda_ipc_dev_ptr,
						req->req_data.recv_msglen);
			//PSM3_GPU_SYNCHRONIZE_MEMCPY();
			psm3_cuda_synchronize_memcpy();
		} else {
			/*PSM3_GPU_MEMCPY_DTOH*/
			psm3_cuda_memcpy_DtoH(req->req_data.buf, (void*)cuda_ipc_dev_ptr,
				req->req_data.recv_msglen);
		}
		psm3_cuda_memhandle_release(ptl->memhandle_cache,
					cuda_ipc_dev_ptr - req->gpu_specific.cuda_ipc_offset);
		req->gpu_specific.cuda_ipc_handle_attached = 0;
		return 1;
	}
	return 0;
}

// On sender, we have now received the CTS corresponding to an RTS
// we may have built in psm3_cuda_build_rts.  All we need to do here is release
// the resources we allocated in psm3_cuda_build_rts.  We saved the  necessary
// information tracking those resources in the send req.
// Returns:
// 	0 - the req was not for a GPU IO
// 	1 - the req was for a GPU IO and we have released the resources
static int psm3_cuda_shm_process_cts(psm2_mq_req_t req)
{
	if (req->gpu_specific.cuda_ipc_handle_attached) {
		// no need to release any Cuda resources
		req->gpu_specific.cuda_ipc_handle_attached = 0;
		return 1;
	}
	return 0;
}
// end of RTS and CTS processing functions for PSM3_DEVICES "shm"
//***************************************************************************

static psm2_error_t psm3_cuda_get_cuda_permitted(struct psm2_ep *ep, bool *enable)
{
	switch (psm3_cuda_permitted_enforcement) {
	case PSM3_CPE_REJECT:
		_HFI_DBG("GET(CUDA_PERMITTED) rejected\n");
		return PSM2_PARAM_ERR;
	case PSM3_CPE_IGNORE:
	case PSM3_CPE_OBEY:
		*enable = ep->gpu_specific.cuda_permitted;
		return PSM2_OK;
	}

	_HFI_ERROR("PSM3_CUDA_PERMITTED_ENFORCEMENT invalid: %u\n",
		psm3_cuda_permitted_enforcement);
	return PSM2_PARAM_ERR;
}

static psm2_error_t psm3_cuda_set_cuda_permitted(struct psm2_ep *ep, bool enable)
{
	switch (psm3_cuda_permitted_enforcement) {
	case PSM3_CPE_REJECT:
		_HFI_DBG("SET(CUDA_PERMITTED) rejected\n");
		return PSM2_PARAM_ERR;
	case PSM3_CPE_IGNORE:
	case PSM3_CPE_OBEY:
		ep->gpu_specific.cuda_permitted = enable;
		return PSM2_OK;
	}

	_HFI_ERROR("PSM3_CUDA_PERMITTED_ENFORCEMENT invalid: %u\n",
		psm3_cuda_permitted_enforcement);
	return PSM2_PARAM_ERR;
}

static bool psm3_cuda_is_memcpy_permitted(struct psm2_ep *ep)
{
	switch (psm3_cuda_permitted_enforcement) {
	case PSM3_CPE_REJECT:
		// REJECT behaves as though the CUDA_PERMITTED option doesn't exist,
		// so behave as per legacy and allow memcpy
		return true;
	case PSM3_CPE_IGNORE:
		// IGNORE behaves as though CUDA_PERMITTED is always true
		return true;
	case PSM3_CPE_OBEY:
		// OBEY requires we honor the config set by the user
		return ep->gpu_specific.cuda_permitted;
	}

	_HFI_ERROR("PSM3_CUDA_PERMITTED_ENFORCEMENT invalid: %u\n",
		psm3_cuda_permitted_enforcement);
	return true;
}

struct psm3_gpu_hal psm3_cuda_hal = {
	.type = "cuda",
#ifdef PSM_HAVE_RNDV_MOD
	.rv_major_rev_fail = 0,
	.rv_minor_rev_fail = 0,
	.rv_capability_expected = RV_CAP_NVIDIA_GPU,
	.hal_cap_expected = PSM_HAL_CAP_NVIDIA_GPU,
#endif
	.ghfp_initialize = psm3_cuda_initialize,
	.ghfp_finalize = psm3_cuda_finalize,
	.ghfp_ep_open = psm3_cuda_ep_open,
	.ghfp_ep_close = psm3_cuda_ep_close,
	.ghfp_identify = psm3_cuda_identify,
	.ghfp_verify_GPU_capabilities = psm3_cuda_verify_GPU_capabilities,
	.ghfp_p2p_supported = psm3_cuda_p2p_supported,
	.ghfp_gpudirect_supported = psm3_cuda_gpudirect_supported,
	.ghfp_using_rv_for_mrs = psm3_cuda_using_rv_for_mrs,
	.ghfp_get_pci_addr = psm3_cuda_get_pci_addr,
#ifdef PSM_HAVE_RNDV_MOD
	.ghfp_min_bar_size = psm3_cuda_min_bar_size,
	.ghfp_check_phys_addr = psm3_cuda_check_phys_addr,
	.ghfp_roundup_gdrcopy = psm3_cuda_roundup_gdrcopy,
#ifdef PSM_HAVE_REG_MR
	.ghfp_roundup_rv_reg_mr = psm3_cuda_roundup_rv_reg_mr,
	.ghfp_init_rv_reg_mr_params = psm3_cuda_init_rv_reg_mr_params,
#endif
	.ghfp_init_rv_pin_mmap_params = psm3_cuda_init_rv_pin_mmap_params,
	.ghfp_rv_reg_mmap_cleanup = psm3_cuda_rv_reg_mmap_cleanup,
#endif /* PSM_HAVE_RNDV_MOD */
#ifdef PSM_HAVE_REG_MR
	.ghfp_cmp_mr = psm3_cuda_cmp_mr,
	.ghfp_init_mr = psm3_cuda_init_mr,
#endif
	.ghfp_fetch_ctxt = psm3_cuda_fetch_ctxt,
	.ghfp_refresh_ctxt = psm3_cuda_refresh_ctxt,
	.ghfp_register_hostmem = psm3_cuda_register_hostmem,
	.ghfp_unregister_hostmem = psm3_cuda_unregister_hostmem,
	.ghfp_is_gpu_mem = psm3_cuda_is_gpu_mem,
	.ghfp_prepare_HtoD_memcpys = psm3_cuda_prepare_HtoD_memcpys,
	.ghfp_prepare_DtoH_memcpys = psm3_cuda_prepare_DtoH_memcpys,
	.ghfp_shutdown_HtoD_memcpys = psm3_cuda_shutdown_HtoD_memcpys,
	.ghfp_shutdown_DtoH_memcpys = psm3_cuda_shutdown_DtoH_memcpys,
	.ghfp_memcpy_HtoD_start = psm3_cuda_memcpy_HtoD_start,
	.ghfp_memcpy_DtoH_start = psm3_cuda_memcpy_DtoH_start,
	.ghfp_memcpy_done = psm3_cuda_memcpy_done,
	.ghfp_hostbuf_lazy_init = psm3_cuda_hostbuf_lazy_init,
	.ghfp_hostbuf_reset = psm3_cuda_hostbuf_reset,
	.ghfp_hostbuf_destroy = psm3_cuda_hostbuf_destroy,
	.ghfp_memcpy_DtoD = psm3_cuda_memcpy_DtoD,
	.ghfp_memcpy_HtoD = psm3_cuda_memcpy_HtoD,
	.ghfp_memcpy_DtoH = psm3_cuda_memcpy_DtoH,
	.ghfp_memcpy = psm3_cuda_memcpy,
	.ghfp_synchronize_memcpy = psm3_cuda_synchronize_memcpy,
	.ghfp_mark_buf_synchronous = psm3_cuda_mark_buf_synchronous,
	.ghfp_host_alloc = psm3_cuda_host_alloc,
	.ghfp_host_free = psm3_cuda_host_free,
	.ghfp_gpu_addr_send_mr = psm3_cuda_gpu_addr_send_mr,
	.ghfp_gpu_addr_recv_mr = psm3_cuda_gpu_addr_recv_mr,
	// functions for PSM3_DEVICES "shm" RTS/CTS processing to enable
	// use of GPU specific scale-up transfers within the given server
	.ghfp_shm_init = psm3_cuda_shm_init,
	.ghfp_shm_finalize = psm3_cuda_shm_finalize,
	.ghfp_shm_epaddr_add = psm3_cuda_shm_epaddr_add,
	.ghfp_shm_epaddr_free = psm3_cuda_shm_epaddr_free,
	.ghfp_shm_dev_fds_needed = psm3_cuda_shm_dev_fds_needed,
	.ghfp_shm_dev_fds_send = psm3_cuda_shm_dev_fds_send,
	.ghfp_shm_dev_fds_connreq_poll = psm3_cuda_shm_dev_fds_connreq_poll,
	.ghfp_shm_dev_fds_check_exchanged = psm3_cuda_shm_dev_fds_check_exchanged,
	.ghfp_shm_dev_fds_poll = psm3_cuda_shm_dev_fds_poll,
	.ghfp_shm_build_rts = psm3_cuda_shm_build_rts,
	.ghfp_shm_process_rts = psm3_cuda_shm_process_rts,
	.ghfp_shm_rtsmatch = psm3_cuda_shm_rtsmatch,
	.ghfp_shm_process_cts = psm3_cuda_shm_process_cts,
	.ghfp_get_cuda_permitted = psm3_cuda_get_cuda_permitted,
	.ghfp_set_cuda_permitted = psm3_cuda_set_cuda_permitted,
	.ghfp_is_memcpy_permitted = psm3_cuda_is_memcpy_permitted,
};

#endif /* PSM_CUDA */
