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

#include <dlfcn.h>
#include <ctype.h>
#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_mq_internal.h"

static int psm3_verno_major = PSM2_VERNO_MAJOR;
static int psm3_verno_minor = PSM2_VERNO_MINOR;
static int psm3_verno = PSMI_VERNO_MAKE(PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR);
static int psm3_verno_client_val;
uint8_t  psm3_addr_fmt;	// PSM3_ADDR_FMT
int psm3_allow_routers;	// PSM3_ALLOW_ROUTERS

char *psm3_allow_subnets[PSMI_MAX_SUBNETS];	// PSM3_SUBNETS
int psm3_num_allow_subnets;
unsigned int psm3_addr_per_nic = 1;

const char *psm3_nic_wildcard = NULL;

const char *psm3_nic_speed_wildcard = NULL;
uint64_t psm3_nic_speed_max_found = 0;

// Special psmi_refcount values
#define PSMI_NOT_INITIALIZED    0
#define PSMI_FINALIZED         -1

// PSM2 doesn't support transitioning out of the PSMI_FINALIZED state
// once psmi_refcount is set to PSMI_FINALIZED, any further attempts to change
// psmi_refcount should be treated as an error
static int psmi_refcount = PSMI_NOT_INITIALIZED;

/* Global lock used for endpoint creation and destroy
 * (in functions psm3_ep_open and psm3_ep_close) and also
 * for synchronization with recv_thread (so that recv_thread
 * will not work on an endpoint which is in a middle of closing). */
psmi_lock_t psm3_creation_lock;

int psm3_affinity_semaphore_open = 0;
char *psm3_sem_affinity_shm_rw_name;
sem_t *psm3_sem_affinity_shm_rw = NULL;

int psm3_affinity_shared_file_opened = 0;
char *psm3_affinity_shm_name;
uint64_t *psm3_shared_affinity_ptr;

uint32_t psm3_cpu_model;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
int is_gdr_copy_enabled;
uint32_t gdr_copy_limit_send;
uint32_t gdr_copy_limit_recv;
int is_gpudirect_enabled = 0;
int _device_support_gpudirect = -1; // -1 indicates "unset". See device_support_gpudirect().
int is_driver_gpudirect_enabled;
uint32_t cuda_thresh_rndv = CUDA_THRESH_RNDV;
uint64_t psm3_gpu_cache_evict;	// in bytes
#endif

#ifdef PSM_CUDA
int is_cuda_enabled;
int _device_support_unified_addr = -1; // -1 indicates "unchecked". See verify_device_support_unified_addr().
int _gpu_p2p_supported = -1; // -1 indicates "unset". see gpu_p2p_supported().
int my_gpu_device = 0;
int cuda_lib_version;

void *psmi_cuda_lib;
#endif

#ifdef PSM_ONEAPI
int is_oneapi_ze_enabled;
int my_gpu_device = 0;
int _gpu_p2p_supported = -1; // -1 indicates "unset". see gpu_p2p_supported().

ze_context_handle_t ze_context = NULL;
ze_driver_handle_t ze_driver = NULL;
struct ze_dev_ctxt ze_devices[MAX_ZE_DEVICES];
int num_ze_devices = 0;
struct ze_dev_ctxt *cur_ze_dev = NULL;

void *psmi_oneapi_ze_lib;
#endif // PSM_ONEAPI

#ifdef PSM_CUDA
CUresult (*psmi_cuInit)(unsigned int  Flags );
CUresult (*psmi_cuCtxDetach)(CUcontext c);
CUresult (*psmi_cuCtxGetCurrent)(CUcontext *c);
CUresult (*psmi_cuCtxSetCurrent)(CUcontext c);
CUresult (*psmi_cuPointerGetAttribute)(void *data, CUpointer_attribute pa, CUdeviceptr p);
CUresult (*psmi_cuPointerSetAttribute)(void *data, CUpointer_attribute pa, CUdeviceptr p);
CUresult (*psmi_cuDeviceCanAccessPeer)(int *canAccessPeer, CUdevice dev, CUdevice peerDev);
CUresult (*psmi_cuDeviceGet)(CUdevice* device, int  ordinal);
CUresult (*psmi_cuDeviceGetAttribute)(int* pi, CUdevice_attribute attrib, CUdevice dev);
CUresult (*psmi_cuDriverGetVersion)(int* driverVersion);
CUresult (*psmi_cuDeviceGetCount)(int* count);
CUresult (*psmi_cuStreamCreate)(CUstream* phStream, unsigned int Flags);
CUresult (*psmi_cuStreamDestroy)(CUstream phStream);
CUresult (*psmi_cuStreamSynchronize)(CUstream phStream);
CUresult (*psmi_cuEventCreate)(CUevent* phEvent, unsigned int Flags);
CUresult (*psmi_cuEventDestroy)(CUevent hEvent);
CUresult (*psmi_cuEventQuery)(CUevent hEvent);
CUresult (*psmi_cuEventRecord)(CUevent hEvent, CUstream hStream);
CUresult (*psmi_cuEventSynchronize)(CUevent hEvent);
CUresult (*psmi_cuMemHostAlloc)(void** pp, size_t bytesize, unsigned int Flags);
CUresult (*psmi_cuMemFreeHost)(void* p);
CUresult (*psmi_cuMemcpy)(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount);
CUresult (*psmi_cuMemcpyDtoD)(CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount);
CUresult (*psmi_cuMemcpyDtoH)(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
CUresult (*psmi_cuMemcpyHtoD)(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
CUresult (*psmi_cuMemcpyDtoHAsync)(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
CUresult (*psmi_cuMemcpyHtoDAsync)(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
CUresult (*psmi_cuIpcGetMemHandle)(CUipcMemHandle* pHandle, CUdeviceptr dptr);
CUresult (*psmi_cuIpcOpenMemHandle)(CUdeviceptr* pdptr, CUipcMemHandle handle, unsigned int Flags);
CUresult (*psmi_cuIpcCloseMemHandle)(CUdeviceptr dptr);
CUresult (*psmi_cuMemGetAddressRange)(CUdeviceptr* pbase, size_t* psize, CUdeviceptr dptr);
CUresult (*psmi_cuDevicePrimaryCtxGetState)(CUdevice dev, unsigned int* flags, int* active);
CUresult (*psmi_cuDevicePrimaryCtxRetain)(CUcontext* pctx, CUdevice dev);
CUresult (*psmi_cuCtxGetDevice)(CUdevice* device);
CUresult (*psmi_cuDevicePrimaryCtxRelease)(CUdevice device);
CUresult (*psmi_cuGetErrorString)(CUresult error, const char **pStr);

uint64_t psmi_count_cuInit;
uint64_t psmi_count_cuCtxDetach;
uint64_t psmi_count_cuCtxGetCurrent;
uint64_t psmi_count_cuCtxSetCurrent;
uint64_t psmi_count_cuPointerGetAttribute;
uint64_t psmi_count_cuPointerSetAttribute;
uint64_t psmi_count_cuDeviceCanAccessPeer;
uint64_t psmi_count_cuDeviceGet;
uint64_t psmi_count_cuDeviceGetAttribute;
uint64_t psmi_count_cuDriverGetVersion;
uint64_t psmi_count_cuDeviceGetCount;
uint64_t psmi_count_cuStreamCreate;
uint64_t psmi_count_cuStreamDestroy;
uint64_t psmi_count_cuStreamSynchronize;
uint64_t psmi_count_cuEventCreate;
uint64_t psmi_count_cuEventDestroy;
uint64_t psmi_count_cuEventQuery;
uint64_t psmi_count_cuEventRecord;
uint64_t psmi_count_cuEventSynchronize;
uint64_t psmi_count_cuMemHostAlloc;
uint64_t psmi_count_cuMemFreeHost;
uint64_t psmi_count_cuMemcpy;
uint64_t psmi_count_cuMemcpyDtoD;
uint64_t psmi_count_cuMemcpyDtoH;
uint64_t psmi_count_cuMemcpyHtoD;
uint64_t psmi_count_cuMemcpyDtoHAsync;
uint64_t psmi_count_cuMemcpyHtoDAsync;
uint64_t psmi_count_cuIpcGetMemHandle;
uint64_t psmi_count_cuIpcOpenMemHandle;
uint64_t psmi_count_cuIpcCloseMemHandle;
uint64_t psmi_count_cuMemGetAddressRange;
uint64_t psmi_count_cuDevicePrimaryCtxGetState;
uint64_t psmi_count_cuDevicePrimaryCtxRetain;
uint64_t psmi_count_cuCtxGetDevice;
uint64_t psmi_count_cuDevicePrimaryCtxRelease;
uint64_t psmi_count_cuGetErrorString;

int psmi_cuda_lib_load()
{
	psm2_error_t err = PSM2_OK;
	char *dlerr;

	PSM2_LOG_MSG("entering");
	_HFI_VDBG("Loading CUDA library.\n");

	psmi_cuda_lib = dlopen("libcuda.so.1", RTLD_LAZY);
	if (!psmi_cuda_lib) {
		dlerr = dlerror();
		_HFI_ERROR("Unable to open libcuda.so.  Error %s\n",
				dlerr ? dlerr : "no dlerror()");
		goto fail;
	}

	psmi_cuDriverGetVersion = dlsym(psmi_cuda_lib, "cuDriverGetVersion");

	if (!psmi_cuDriverGetVersion) {
		_HFI_ERROR
			("Unable to resolve symbols in CUDA libraries.\n");
		goto fail;
	}
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuGetErrorString);// for PSMI_CUDA_CALL

	PSMI_CUDA_CALL(cuDriverGetVersion, &cuda_lib_version);
	if (cuda_lib_version < 7000) {
		_HFI_ERROR("Please update CUDA driver, required minimum version is 7.0\n");
		goto fail;
	}

	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuInit);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuCtxGetCurrent);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuCtxDetach);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuCtxSetCurrent);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuPointerGetAttribute);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuPointerSetAttribute);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDeviceCanAccessPeer);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDeviceGetAttribute);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDeviceGet);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDeviceGetCount);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuStreamCreate);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuStreamDestroy);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuStreamSynchronize);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuEventCreate);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuEventDestroy);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuEventQuery);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuEventRecord);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuEventSynchronize);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemHostAlloc);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemFreeHost);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemcpy);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemcpyDtoD);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemcpyDtoH);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemcpyHtoD);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemcpyDtoHAsync);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemcpyHtoDAsync);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuIpcGetMemHandle);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuIpcOpenMemHandle);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuIpcCloseMemHandle);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuMemGetAddressRange);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDevicePrimaryCtxGetState);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDevicePrimaryCtxRetain);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuDevicePrimaryCtxRelease);
	PSMI_CUDA_DLSYM(psmi_cuda_lib, cuCtxGetDevice);

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	if (psmi_cuda_lib)
		dlclose(psmi_cuda_lib);
	err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, "Unable to load CUDA library.\n");
	return err;
}

static void psmi_cuda_stats_register()
{
#define PSMI_CUDA_COUNT_DECLU64(func) \
	PSMI_STATS_DECLU64(#func, &psmi_count_##func)

	struct psmi_stats_entry entries[] = {
		PSMI_CUDA_COUNT_DECLU64(cuInit),
		PSMI_CUDA_COUNT_DECLU64(cuCtxDetach),
		PSMI_CUDA_COUNT_DECLU64(cuCtxGetCurrent),
		PSMI_CUDA_COUNT_DECLU64(cuCtxSetCurrent),
		PSMI_CUDA_COUNT_DECLU64(cuPointerGetAttribute),
		PSMI_CUDA_COUNT_DECLU64(cuPointerSetAttribute),
		PSMI_CUDA_COUNT_DECLU64(cuDeviceCanAccessPeer),
		PSMI_CUDA_COUNT_DECLU64(cuDeviceGet),
		PSMI_CUDA_COUNT_DECLU64(cuDeviceGetAttribute),
		PSMI_CUDA_COUNT_DECLU64(cuDriverGetVersion),
		PSMI_CUDA_COUNT_DECLU64(cuDeviceGetCount),
		PSMI_CUDA_COUNT_DECLU64(cuStreamCreate),
		PSMI_CUDA_COUNT_DECLU64(cuStreamDestroy),
		PSMI_CUDA_COUNT_DECLU64(cuStreamSynchronize),
		PSMI_CUDA_COUNT_DECLU64(cuEventCreate),
		PSMI_CUDA_COUNT_DECLU64(cuEventDestroy),
		PSMI_CUDA_COUNT_DECLU64(cuEventQuery),
		PSMI_CUDA_COUNT_DECLU64(cuEventRecord),
		PSMI_CUDA_COUNT_DECLU64(cuEventSynchronize),
		PSMI_CUDA_COUNT_DECLU64(cuMemHostAlloc),
		PSMI_CUDA_COUNT_DECLU64(cuMemFreeHost),
		PSMI_CUDA_COUNT_DECLU64(cuMemcpy),
		PSMI_CUDA_COUNT_DECLU64(cuMemcpyDtoD),
		PSMI_CUDA_COUNT_DECLU64(cuMemcpyDtoH),
		PSMI_CUDA_COUNT_DECLU64(cuMemcpyHtoD),
		PSMI_CUDA_COUNT_DECLU64(cuMemcpyDtoHAsync),
		PSMI_CUDA_COUNT_DECLU64(cuMemcpyHtoDAsync),
		PSMI_CUDA_COUNT_DECLU64(cuIpcGetMemHandle),
		PSMI_CUDA_COUNT_DECLU64(cuIpcOpenMemHandle),
		PSMI_CUDA_COUNT_DECLU64(cuIpcCloseMemHandle),
		PSMI_CUDA_COUNT_DECLU64(cuMemGetAddressRange),
		PSMI_CUDA_COUNT_DECLU64(cuDevicePrimaryCtxGetState),
		PSMI_CUDA_COUNT_DECLU64(cuDevicePrimaryCtxRetain),
		PSMI_CUDA_COUNT_DECLU64(cuCtxGetDevice),
		PSMI_CUDA_COUNT_DECLU64(cuDevicePrimaryCtxRelease),
		PSMI_CUDA_COUNT_DECLU64(cuGetErrorString),
	};
#undef PSMI_CUDA_COUNT_DECLU64

	psm3_stats_register_type("PSM_Cuda_call_statistics",
			PSMI_STATSTYPE_GPU,
			entries, PSMI_HOWMANY(entries), NULL,
			&psmi_count_cuInit, NULL); /* context must != NULL */
}
#endif // PSM_CUDA

#ifdef PSM_ONEAPI
ze_result_t (*psmi_zeInit)(ze_init_flags_t flags);
ze_result_t (*psmi_zeDriverGet)(uint32_t *pCount, ze_driver_handle_t *phDrivers);
ze_result_t (*psmi_zeDeviceGet)(ze_driver_handle_t hDriver, uint32_t *pCount, ze_device_handle_t *phDevices);
ze_result_t (*psmi_zeContextCreate)(ze_driver_handle_t hDriver, const ze_context_desc_t *desc, ze_context_handle_t *phContext);
ze_result_t (*psmi_zeContextDestroy)(ze_context_handle_t hContext);
ze_result_t (*psmi_zeCommandQueueCreate)(ze_context_handle_t hContext, ze_device_handle_t hDevice,const ze_command_queue_desc_t *desc, ze_command_queue_handle_t *phCommandQueue);
ze_result_t (*psmi_zeCommandQueueDestroy)(ze_command_queue_handle_t hCommandQueue);
ze_result_t (*psmi_zeCommandQueueExecuteCommandLists)(ze_command_queue_handle_t hCommandQueue, uint32_t numCommandLists, ze_command_list_handle_t *phCommandLists, ze_fence_handle_t hFence);
ze_result_t (*psmi_zeCommandQueueSynchronize)(ze_command_queue_handle_t hCommandQueue, uint64_t timeout);
ze_result_t (*psmi_zeCommandListCreate)(ze_context_handle_t hContext, ze_device_handle_t hDevice, const ze_command_list_desc_t *desc, ze_command_list_handle_t *phCommandList);
ze_result_t (*psmi_zeCommandListDestroy)(ze_command_list_handle_t hCommandList);
ze_result_t (*psmi_zeCommandListClose)(ze_command_list_handle_t hCommandList);
ze_result_t (*psmi_zeCommandListReset)(ze_command_list_handle_t hCommandList);
ze_result_t (*psmi_zeCommandListCreateImmediate)(ze_device_handle_t hDevice, const ze_command_queue_desc_t * altdesc, ze_command_list_handle_t * phCommandList);
ze_result_t (*psmi_zeCommandListAppendMemoryCopy)(ze_command_list_handle_t hCommandList, void *dstptr, const void *srcptr, size_t size, ze_event_handle_t hSignalEvent, uint32_t numWaitEvents, ze_event_handle_t *phWaitEvents);
ze_result_t (*psmi_zeCommandListAppendSignalEvent)(ze_command_list_handle_t hCommandList, ze_event_handle_t hEvent);
ze_result_t (*psmi_zeDeviceCanAccessPeer)(ze_device_handle_t hDevice, ze_device_handle_t hPeerDevice, ze_bool_t *value);
ze_result_t (*psmi_zeDeviceGetCommandQueueGroupProperties)(ze_device_handle_t hDevice, uint32_t *pCount, ze_command_queue_group_properties_t *pCommandQueueGroupProperties);
ze_result_t (*psmi_zeMemAllocHost)(ze_context_handle_t hContext, const ze_host_mem_alloc_desc_t *host_desc, size_t size, size_t alignment, void **pptr);
ze_result_t (*psmi_zeMemFree)(ze_context_handle_t hContext, void *ptr);
ze_result_t (*psmi_zeMemGetIpcHandle)(ze_context_handle_t hContext, const void *ptr, ze_ipc_mem_handle_t *pIpcHandle);
ze_result_t (*psmi_zeMemOpenIpcHandle)(ze_context_handle_t hContext,ze_device_handle_t hDevice, ze_ipc_mem_handle_t handle, ze_ipc_memory_flags_t flags, void **pptr);
ze_result_t (*psmi_zeMemCloseIpcHandle)(ze_context_handle_t hContext, const void *ptr);
ze_result_t (*psmi_zeMemGetAddressRange)(ze_context_handle_t hContext, const void *ptr, void **pBase, size_t *pSize);
ze_result_t (*psmi_zeMemGetAllocProperties)(ze_context_handle_t hContext, const void *ptr, ze_memory_allocation_properties_t *pMemAllocProperties, ze_device_handle_t *phDevice);
ze_result_t (*psmi_zeEventPoolCreate)(ze_context_handle_t hContext, const ze_event_pool_desc_t *desc, uint32_t numDevices, ze_device_handle_t *phDevices, ze_event_pool_handle_t *phEventPool);
ze_result_t (*psmi_zeEventPoolDestroy)(ze_event_pool_handle_t hEventPool);
ze_result_t (*psmi_zeEventCreate)(ze_event_pool_handle_t hEventPool, const ze_event_desc_t *desc, ze_event_handle_t *phEvent);
ze_result_t (*psmi_zeEventDestroy)(ze_event_handle_t hEvent);
ze_result_t (*psmi_zeEventQueryStatus)(ze_event_handle_t hEvent);
ze_result_t (*psmi_zeEventHostSynchronize)(ze_event_handle_t hEvent, uint64_t timeout);
ze_result_t (*psmi_zeEventHostReset)(ze_event_handle_t hEvent);

uint64_t psmi_count_zeInit;
uint64_t psmi_count_zeDriverGet;
uint64_t psmi_count_zeDeviceGet;
uint64_t psmi_count_zeContextCreate;
uint64_t psmi_count_zeContextDestroy;
uint64_t psmi_count_zeCommandQueueCreate;
uint64_t psmi_count_zeCommandQueueDestroy;
uint64_t psmi_count_zeCommandQueueExecuteCommandLists;
uint64_t psmi_count_zeCommandQueueSynchronize;
uint64_t psmi_count_zeCommandListCreate;
uint64_t psmi_count_zeCommandListDestroy;
uint64_t psmi_count_zeCommandListClose;
uint64_t psmi_count_zeCommandListReset;
uint64_t psmi_count_zeCommandListCreateImmediate;
uint64_t psmi_count_zeCommandListAppendMemoryCopy;
uint64_t psmi_count_zeCommandListAppendSignalEvent;
uint64_t psmi_count_zeDeviceCanAccessPeer;
uint64_t psmi_count_zeDeviceGetCommandQueueGroupProperties;
uint64_t psmi_count_zeMemAllocHost;
uint64_t psmi_count_zeMemFree;
uint64_t psmi_count_zeMemGetIpcHandle;
uint64_t psmi_count_zeMemOpenIpcHandle;
uint64_t psmi_count_zeMemCloseIpcHandle;
uint64_t psmi_count_zeMemGetAddressRange;
uint64_t psmi_count_zeMemGetAllocProperties;
uint64_t psmi_count_zeEventPoolCreate;
uint64_t psmi_count_zeEventPoolDestroy;
uint64_t psmi_count_zeEventCreate;
uint64_t psmi_count_zeEventDestroy;
uint64_t psmi_count_zeEventQueryStatus;
uint64_t psmi_count_zeEventHostSynchronize;
uint64_t psmi_count_zeEventHostReset;

int psmi_oneapi_ze_load()
{
	psm2_error_t err = PSM2_OK;
	char *dlerr;

	PSM2_LOG_MSG("entering");
	_HFI_VDBG("Loading OneAPI Level Zero library.\n");

	psmi_oneapi_ze_lib = dlopen("libze_loader.so", RTLD_LAZY);
	if (!psmi_oneapi_ze_lib) {
		dlerr = dlerror();
		_HFI_ERROR(
			"Unable to open libze_loader.so.  Error %s\n",
			dlerr ? dlerr : "no dlerror()");
		goto fail;
	}

	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeInit);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeDriverGet);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeDeviceGet);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeContextCreate);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeContextDestroy);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandQueueCreate);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandQueueDestroy);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandQueueExecuteCommandLists);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandQueueSynchronize);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListCreate);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListDestroy);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListClose);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListReset);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListCreateImmediate);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListAppendMemoryCopy);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeCommandListAppendSignalEvent);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeDeviceCanAccessPeer);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeDeviceGetCommandQueueGroupProperties);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemAllocHost);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemFree);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemGetIpcHandle);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemOpenIpcHandle);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemCloseIpcHandle);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemGetAddressRange);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeMemGetAllocProperties);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventPoolCreate);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventPoolDestroy);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventCreate);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventDestroy);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventQueryStatus);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventHostSynchronize);
	PSMI_ONEAPI_ZE_DLSYM(psmi_oneapi_ze_lib, zeEventHostReset);

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	if (psmi_oneapi_ze_lib)
		dlclose(psmi_oneapi_ze_lib);
	err = psm3_handle_error(PSMI_EP_NORETURN,
		PSM2_INTERNAL_ERR,
		"Unable to load OneAPI Level Zero library.\n");
	return err;
}

static void psmi_oneapi_ze_stats_register()
{
#define PSMI_ONEAPI_ZE_COUNT_DECLU64(func) \
	PSMI_STATS_DECLU64(#func, &psmi_count_##func)

	struct psmi_stats_entry ze_entries[] = {
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeInit),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeDriverGet),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeDeviceGet),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeContextCreate),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeContextDestroy),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueCreate),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueDestroy),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueExecuteCommandLists),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueSynchronize),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListCreate),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListDestroy),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListClose),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListReset),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListCreateImmediate),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListAppendMemoryCopy),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeCommandListAppendSignalEvent),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeDeviceCanAccessPeer),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeDeviceGetCommandQueueGroupProperties),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemAllocHost),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemFree),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemGetIpcHandle),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemOpenIpcHandle),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemCloseIpcHandle),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemGetAddressRange),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeMemGetAllocProperties),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventPoolCreate),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventPoolDestroy),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventCreate),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventDestroy),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventQueryStatus),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventHostSynchronize),
		PSMI_ONEAPI_ZE_COUNT_DECLU64(zeEventHostReset)
	};
#undef PSMI_ONEAPI_ZE_COUNT_DECLU64

	psm3_stats_register_type(
		"PSM_OneAPI_ZE_call_statistics",
		PSMI_STATSTYPE_GPU,
		ze_entries, PSMI_HOWMANY(ze_entries), NULL,
		&psmi_count_zeInit, NULL); /* context must != NULL */
}
#endif // PSM_ONEAPI

/*
 * Bit field that contains capability set.
 * Each bit represents different capability.
 * It is supposed to be filled with logical OR
 * on conditional compilation basis
 * along with future features/capabilities.
 */
uint64_t psm3_capabilities_bitset = PSM2_MULTI_EP_CAP | PSM2_LIB_REFCOUNT_CAP;

int psm3_verno_client()
{
	return psm3_verno_client_val;
}

/* This function is used to determine whether the current library build can
 * successfully communicate with another library that claims to be version
 * 'verno'.
 *
 * PSM 2.x is always ABI compatible, but this checks to see if two different
 * versions of the library can coexist.
 */
int psm3_verno_isinteroperable(uint16_t verno)
{
	if (PSMI_VERNO_GET_MAJOR(verno) != PSM2_VERNO_MAJOR)
		return 0;

	return 1;
}

int MOCKABLE(psm3_isinitialized)()
{
	return (psmi_refcount > 0);
}
MOCK_DEF_EPILOGUE(psm3_isinitialized);

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
static void psmi_gpu_init(void)
{
	union psmi_envvar_val env_enable_gdr_copy;
	psm3_getenv("PSM3_GDRCOPY",
				"Enable (set envvar to 1) for gdr copy support in PSM (Enabled by default)",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)1, &env_enable_gdr_copy);
	is_gdr_copy_enabled = env_enable_gdr_copy.e_int;

	union psmi_envvar_val env_cuda_thresh_rndv;
	psm3_getenv("PSM3_CUDA_THRESH_RNDV",
				"RNDV protocol is used for GPU send message sizes greater than the threshold",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)cuda_thresh_rndv, &env_cuda_thresh_rndv);
	cuda_thresh_rndv = env_cuda_thresh_rndv.e_uint;


	union psmi_envvar_val env_gdr_copy_limit_send;
	psm3_getenv("PSM3_GDRCOPY_LIMIT_SEND",
				"GDR Copy is turned off on the send side"
				" for message sizes greater than the limit"
#ifndef OPA
				" or larger than 1 MTU\n",
#else
				"\n",
#endif
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)GDR_COPY_LIMIT_SEND, &env_gdr_copy_limit_send);
	gdr_copy_limit_send = env_gdr_copy_limit_send.e_int;

	if (gdr_copy_limit_send < 8 || gdr_copy_limit_send > cuda_thresh_rndv)
		gdr_copy_limit_send = max(GDR_COPY_LIMIT_SEND, cuda_thresh_rndv);

	union psmi_envvar_val env_gdr_copy_limit_recv;
	psm3_getenv("PSM3_GDRCOPY_LIMIT_RECV",
				"GDR Copy is turned off on the recv side"
				" for message sizes greater than the limit\n",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)GDR_COPY_LIMIT_RECV, &env_gdr_copy_limit_recv);
	gdr_copy_limit_recv = env_gdr_copy_limit_recv.e_int;

	if (gdr_copy_limit_recv < 8)
		gdr_copy_limit_recv = GDR_COPY_LIMIT_RECV;

	if (!is_gdr_copy_enabled)
		gdr_copy_limit_send = gdr_copy_limit_recv = 0;
}
#endif /* PSM_CUDA || PSM_ONEAPI */

#ifdef PSM_CUDA
int psmi_cuda_initialize()
{
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");
	_HFI_VDBG("Enabling CUDA support.\n");

	psmi_cuda_stats_register();

	err = psmi_cuda_lib_load();
	if (err != PSM2_OK)
		goto fail;

	PSMI_CUDA_CALL(cuInit, 0);

#ifdef PSM_HAVE_RNDV_MOD
	psm2_get_gpu_bars();
#endif

	psmi_gpu_init();

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, "Unable to initialize PSM3 CUDA support.\n");
	return err;
}
#endif // PSM_CUDA

#ifdef PSM_ONEAPI

static void psmi_oneapi_cmd_create(ze_device_handle_t dev, struct ze_dev_ctxt *ctxt)
{
	ze_command_queue_desc_t ze_cq_desc = {
		.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
		.flags = 0,
		.mode = ZE_COMMAND_QUEUE_MODE_DEFAULT,
		.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL,
		.ordinal = 0 /* this must be less than device_properties.numAsyncComputeEngines */
	};
	ze_command_list_desc_t ze_cl_desc = {
		.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
		.flags = 0
	};

	PSMI_ONEAPI_ZE_CALL(zeCommandQueueCreate, ze_context, dev,
			    &ze_cq_desc, &ctxt->cq);
	PSMI_ONEAPI_ZE_CALL(zeCommandListCreate, ze_context, dev, &ze_cl_desc,
			    &ctxt->cl);
	ctxt->dev = dev;
}

void psmi_oneapi_cmd_create_all(void)
{
	int i;
	struct ze_dev_ctxt *ctxt;

	for (i = 0; i < num_ze_devices; i++) {
		ctxt = &ze_devices[i];

		if (!ctxt->cq || !ctxt->cl)
			psmi_oneapi_cmd_create(ctxt->dev, ctxt);
	}
	if (num_ze_devices > 0)
		cur_ze_dev = &ze_devices[0];
}

void psmi_oneapi_cmd_destroy_all(void)
{
	int i;
	struct ze_dev_ctxt *ctxt;

	for (i = 0; i < num_ze_devices; i++) {
		ctxt = &ze_devices[i];

		if (ctxt->cl) {
			PSMI_ONEAPI_ZE_CALL(zeCommandListDestroy, ctxt->cl);
			ctxt->cl = NULL;
		}
		if (ctxt->cq) {
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueDestroy, ctxt->cq);
			ctxt->cq = NULL;
		}
	}
	cur_ze_dev = NULL;
}

int psmi_oneapi_ze_initialize()
{
	psm2_error_t err = PSM2_OK;
	uint32_t ze_driver_count = 1;
	uint32_t  ze_device_count = 0;
	ze_device_handle_t devices[MAX_ZE_DEVICES];
	int i;

	PSM2_LOG_MSG("entering");
	_HFI_VDBG("Init Level Zero library.\n");

	psmi_oneapi_ze_stats_register();
	err = psmi_oneapi_ze_load();
	if (err != PSM2_OK)
		goto fail;

	PSMI_ONEAPI_ZE_CALL(zeInit, ZE_INIT_FLAG_GPU_ONLY);

	PSMI_ONEAPI_ZE_CALL(zeDriverGet, &ze_driver_count, &ze_driver);
	PSMI_ONEAPI_ZE_CALL(zeDeviceGet, ze_driver, &ze_device_count, NULL);
	if (ze_device_count > MAX_ZE_DEVICES)
		ze_device_count = MAX_ZE_DEVICES;
	PSMI_ONEAPI_ZE_CALL(zeDeviceGet, ze_driver, &ze_device_count, devices);

	ze_context_desc_t ctxtDesc = { ZE_STRUCTURE_TYPE_CONTEXT_DESC, NULL, 0 };
	PSMI_ONEAPI_ZE_CALL(zeContextCreate, ze_driver, &ctxtDesc, &ze_context);
	_HFI_VDBG("ze_driver %p first device %p ze_context %p\n",
		   ze_driver, &devices[0], ze_context);

	for (i = 0; i < ze_device_count; i++)
		psmi_oneapi_cmd_create(devices[i], &ze_devices[i]);

	num_ze_devices = ze_device_count;
	if (num_ze_devices > 0)
		cur_ze_dev = &ze_devices[0];

	psmi_gpu_init();

	psm3_num_ze_dev_fds = 0;

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	err = psm3_handle_error(PSMI_EP_NORETURN,
		PSM2_INTERNAL_ERR,
		"Unable to initialize PSM3 OneAPI Level Zero support.\n");
	return err;
}
#endif // PSM_ONEAPI

/* parse PSM3_SUBNETS to get a list of subnets we'll consider */
static
psm2_error_t
psmi_parse_subnets(const char *subnets)
{
	char *tempstr = NULL;
	char *e, *ee, *b;
	psm2_error_t err = PSM2_OK;
	int len;
	int i = 0;

	psmi_assert_always(subnets != NULL);
	len = strlen(subnets) + 1;

	tempstr = (char *)psmi_calloc(PSMI_EP_NONE, UNDEFINED, 1, len);
	if (tempstr == NULL)
		goto fail;

	strncpy(tempstr, subnets, len);
	ee = tempstr + len;	// very end of subnets string
	for (e=tempstr, i=0; e < ee && *e && i < PSMI_MAX_SUBNETS; e++) {
		char *p;

		while (*e && isspace(*e))
			e++;
		b = e;	// begining of subnet
		while (*e && *e != ',' )
			e++;
		*e = '\0';	// mark end
		// skip whitespace at end of subnet
		for (p = e-1; p >= b && isspace(*p); p--)
			*p = '\0';
		if (*b) {
			psm3_allow_subnets[i] = psmi_strdup(PSMI_EP_NONE, b);
			if (! psm3_allow_subnets[i]) {
				err = PSM2_NO_MEMORY;
				goto fail;
			}
			_HFI_DBG("PSM3_SUBNETS Entry %d = '%s'\n",
					i, psm3_allow_subnets[i]);
			i++;
		}
	}
	if ( e < ee && *e)
		_HFI_INFO("More than %d entries in PSM3_SUBNETS, ignoring extra entries\n", PSMI_MAX_SUBNETS);
	psm3_num_allow_subnets = i;
	_HFI_DBG("PSM3_SUBNETS Num subnets = %d\n", psm3_num_allow_subnets);
fail:
	if (tempstr != NULL)
		psmi_free(tempstr);
	return err;

}

static
void psmi_parse_nic_var()
{
	union psmi_envvar_val env_nic;
	psm3_getenv("PSM3_NIC",
		"Device Unit number or name or wildcard (-1 or 'any' autodetects)",
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		(union psmi_envvar_val)"any", &env_nic);
	//autodetect
	if (0 == strcasecmp(env_nic.e_str, "any")) {
		//so this disables filtering
		psm3_nic_wildcard = NULL;
		return;
	}
	char *endptr = NULL;
	int unit = strtol(env_nic.e_str, &endptr, 10);
	//Unit decimal number
	if ((env_nic.e_str != endptr)&&(*endptr == '\0'))
	{
		//filter equals device name
		psm3_nic_wildcard = psm3_sysfs_unit_dev_name(unit);
		return;
	}
	unit = strtol(env_nic.e_str, &endptr, 16);
	//Unit hex number
	if ((env_nic.e_str != endptr)&&(*endptr == '\0'))
	{
		//filter equals device name
		psm3_nic_wildcard = psm3_sysfs_unit_dev_name(unit);
		return;
	}
	//Unit name or wildcard
	psm3_nic_wildcard = env_nic.e_str;
}

psm2_error_t psm3_init(int *major, int *minor)
{
	psm2_error_t err = PSM2_OK;
	union psmi_envvar_val env_tmask;
	int devid_enabled[PTL_MAX_INIT];

	psm3_stats_initialize();

	psm3_mem_stats_register();

	psmi_log_initialize();

	PSM2_LOG_MSG("entering");

	/* When PSM_PERF is enabled, the following code causes the
	   PMU to be programmed to measure instruction cycles of the
	   TX/RX speedpaths of PSM. */
	GENERIC_PERF_INIT();
	GENERIC_PERF_SET_SLOT_NAME(PSM_TX_SPEEDPATH_CTR, "TX");
	GENERIC_PERF_SET_SLOT_NAME(PSM_RX_SPEEDPATH_CTR, "RX");

	if (psmi_refcount > 0) {
		psmi_refcount++;
		goto update;
	}

	if (psmi_refcount == PSMI_FINALIZED) {
		err = PSM2_IS_FINALIZED;
		goto fail;
	}

	if (major == NULL || minor == NULL) {
		err = PSM2_PARAM_ERR;
		goto fail;
	}

	psmi_init_lock(&psm3_creation_lock);

#ifdef PSM_DEBUG
	if (!getenv("PSM3_NO_WARN")) {
		_HFI_ERROR(
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *DEBUG* BUILD.\n");
		fprintf(stderr,
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *DEBUG* BUILD.\n");
	}
#endif

#ifdef PSM_PROFILE
	if (!getenv("PSM3_NO_WARN")) {
		_HFI_ERROR(
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *PROFILE* BUILD.\n");
		fprintf(stderr,
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *PROFILE* BUILD.\n");
	}
#endif

#ifdef PSM_FI
	/* Make sure we complain if fault injection is enabled */
	if (getenv("PSM3_FI") && !getenv("PSM3_NO_WARN"))
		fprintf(stderr,
			"!!! WARNING !!! YOU ARE RUNNING WITH FAULT INJECTION ENABLED!\n");
#endif /* #ifdef PSM_FI */

	/* Make sure, as an internal check, that this version knows how to detect
	 * compatibility with other library versions it may communicate with */
	if (psm3_verno_isinteroperable(psm3_verno) != 1) {
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"psm3_verno_isinteroperable() not updated for current version!");
		goto fail;
	}

	/* The only way to not support a client is if the major number doesn't
	 * match */
	if (*major != PSM2_VERNO_MAJOR && *major != PSM2_VERNO_COMPAT_MAJOR) {
		err = psm3_handle_error(NULL, PSM2_INIT_BAD_API_VERSION,
					"This library does not implement version %d.%d",
					*major, *minor);
		goto fail;
	}

	/* Make sure we don't keep track of a client that claims a higher version
	 * number than we are */
	psm3_verno_client_val =
	    min(PSMI_VERNO_MAKE(*major, *minor), psm3_verno);

	/* Check to see if we need to set Architecture flags to something
	 * besides big core Xeons */
	cpuid_t id;
	psm3_cpu_model = CPUID_MODEL_UNDEFINED;

	/* First check to ensure Genuine Intel */
	get_cpuid(0x0, 0, &id);
	if(id.ebx == CPUID_GENUINE_INTEL_EBX
		&& id.ecx == CPUID_GENUINE_INTEL_ECX
		&& id.edx == CPUID_GENUINE_INTEL_EDX)
	{
		/* Use cpuid with EAX=1 to get processor info */
		get_cpuid(0x1, 0, &id);
		psm3_cpu_model = CPUID_GENUINE_INTEL;
	}

	if( (psm3_cpu_model == CPUID_GENUINE_INTEL) &&
		(id.eax & CPUID_FAMILY_MASK) == CPUID_FAMILY_XEON)
	{
		psm3_cpu_model = ((id.eax & CPUID_MODEL_MASK) >> 4) |
				((id.eax & CPUID_EXMODEL_MASK) >> 12);
	}

	psmi_refcount++;
	/* psm3_dbgmask lives in libhfi.so */
	psm3_getenv("PSM3_TRACEMASK",
		    "Mask flags for tracing",
		    PSMI_ENVVAR_LEVEL_USER,
		    PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)__HFI_DEBUG_DEFAULT_STR, &env_tmask);
	psm3_dbgmask = psm3_parse_val_pattern(env_tmask.e_str, __HFI_DEBUG_DEFAULT,
			__HFI_DEBUG_DEFAULT);

	/* The "real thing" is done in hfi_proto.c as a constructor function, but
	 * we getenv it here to report what we're doing with the setting */
	{
		extern int psm3_malloc_no_mmap;
		union psmi_envvar_val env_mmap;
		char *env = getenv("PSM3_DISABLE_MMAP_MALLOC");
		int broken = (env && *env && !psm3_malloc_no_mmap);
		psm3_getenv("PSM3_DISABLE_MMAP_MALLOC",
			    broken ? "Skipping mmap disable for malloc()" :
			    "Disable mmap for malloc()",
			    PSMI_ENVVAR_LEVEL_USER,
			    PSMI_ENVVAR_TYPE_YESNO,
			    (union psmi_envvar_val)0, &env_mmap);
		if (broken)
			_HFI_ERROR
			    ("Couldn't successfully disable mmap in mallocs "
			     "with mallopt()\n");
	}

	{
		union psmi_envvar_val env_addr_fmt;
		psm3_getenv("PSM3_ADDR_FMT",
					"Select address format for NICs and EPID",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)PSMI_ADDR_FMT_DEFAULT, &env_addr_fmt);
		if (env_addr_fmt.e_int > PSMI_MAX_ADDR_FMT_SUPPORTED) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  " The max epid version supported in this version of PSM3 is %u \n"
					  "Please upgrade PSM3 \n",
					  PSMI_MAX_ADDR_FMT_SUPPORTED);
			goto fail_unref;
		} else if ( env_addr_fmt.e_int != PSMI_ADDR_FMT_DEFAULT
			&& (env_addr_fmt.e_int < PSMI_MIN_ADDR_FMT_SUPPORTED
				|| ! PSMI_IPS_ADDR_FMT_IS_VALID(env_addr_fmt.e_int))
			) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  " Invalid value provided through PSM3_ADDR_FMT %d\n", env_addr_fmt.e_int);
			goto fail_unref;
		}
		psm3_addr_fmt = env_addr_fmt.e_int;
	}
	{
		union psmi_envvar_val env_addr_per_nic;
		psm3_getenv("PSM3_ADDR_PER_NIC",
					"Number of addresses per NIC to present",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)1, &env_addr_per_nic);
		if (env_addr_per_nic.e_uint > PSMI_MAX_RAILS) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "PSM3_ADDR_PER_NIC exceeds %u\n",
					  PSMI_MAX_RAILS);
			goto fail;
		}
		psm3_addr_per_nic = env_addr_per_nic.e_uint;
	}
	{
		union psmi_envvar_val env_allow_routers;
		psm3_getenv("PSM3_ALLOW_ROUTERS",
					"Disable check for Ethernet subnet equality between nodes\n"
					" allows routers between nodes and assumes single network plane for multi-rail\n",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)0, &env_allow_routers);
		psm3_allow_routers = env_allow_routers.e_int;
	}
	{
		union psmi_envvar_val env_subnets;
		psm3_getenv("PSM3_SUBNETS",
			"List of comma separated patterns for IPv4 and IPv6 subnets to consider",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)PSMI_SUBNETS_DEFAULT, &env_subnets);

		if ((err = psmi_parse_subnets(env_subnets.e_str)))
			goto fail_unref;
	}
	psmi_parse_nic_var();


	{
		/* get PSM3_NIC_SPEED
		 * "any" - allow any and all NIC speeds
		 * "max" - among the non-filtered NICs, identify fastest and
		 *	filter all NICs with lower speeds (default)
		 * # - select only NICs which match the given speed
		 *		(in bits/sec)
		 * pattern - a pattern which may contain one or more numeric
		 *		speed values
		 */
		union psmi_envvar_val env_speed;
		psm3_getenv("PSM3_NIC_SPEED",
			"NIC speed selection criteria ('any', 'max' or pattern of exact speeds)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"max", &env_speed);
		psm3_nic_speed_wildcard = env_speed.e_str;
	}

	if (getenv("PSM3_DIAGS")) {
		_HFI_INFO("Running diags...\n");
		if (psm3_diags()) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, " diags failure \n");
			goto fail_unref;
		}
	}

	psm3_multi_ep_init();

#ifdef PSM_FI
	psm3_faultinj_init();
#endif /* #ifdef PSM_FI */

	psm3_epid_init();

	if ((err = psm3_parse_devices(devid_enabled)))
		goto fail_unref;

	int rc = psm3_hal_initialize(devid_enabled);

	if (rc)
	{
		err = PSM2_INTERNAL_ERR;
		goto fail_unref;
	}

#ifdef PSM_DSA
	if (psm3_device_is_enabled(devid_enabled, PTL_DEVID_AMSH)) {
		if (psm3_dsa_init()) {
			err = PSM2_INTERNAL_ERR;
			goto fail_unref;
		}
	}
#endif

#ifdef PSM_CUDA
	union psmi_envvar_val env_enable_cuda;
	psm3_getenv("PSM3_CUDA",
			"Enable (set envvar to 1) for cuda support in PSM (Disabled by default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)0, &env_enable_cuda);
	// order important, always parse gpudirect
	is_cuda_enabled = psmi_parse_gpudirect() || env_enable_cuda.e_int;

	if (PSMI_IS_GPU_ENABLED) {
		err = psmi_cuda_initialize();
		if (err != PSM2_OK)
#ifdef PSM_DSA
			goto fail_undsa;
#else
			goto fail_unref;
#endif
	}
#endif

#ifdef PSM_ONEAPI
	union psmi_envvar_val env_enable_oneapi;
	psm3_getenv("PSM3_ONEAPI_ZE",
			"Enable (set envvar to 1) for OneAPI Level Zero (ZE) support in PSM (Disabled by default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)0, &env_enable_oneapi);
	is_oneapi_ze_enabled = psmi_parse_gpudirect() || env_enable_oneapi.e_int;

	if (PSMI_IS_GPU_ENABLED) {
		err = psmi_oneapi_ze_initialize();
		if (err != PSM2_OK) {
#ifdef PSM_DSA
			goto fail_undsa;
#else
			goto fail_unref;
#endif
		}
	}
#endif //PSM_ONEAPI

update:
	*major = (int)psm3_verno_major;
	*minor = (int)psm3_verno_minor;
fail:
	_HFI_DBG("psmi_refcount=%d,err=%u\n", psmi_refcount, err);

	PSM2_LOG_MSG("leaving");
	return err;
#if defined(PSM_DSA) && (defined(PSM_CUDA) || defined(PSM_ONEAPI))
fail_undsa:
	psm3_dsa_fini();
#endif
fail_unref:
	psmi_refcount--;
	goto fail;
}

/* convert return value for various device queries into
 * a psm2_error_t.  Only used for functions requesting NIC details
 * -3 -> PSM2_EP_NO_DEVICE, 0 -> PSM2_OK, other -> PSM2_INTERNAL_ERR
 */
static inline psm2_error_t unit_query_ret_to_err(int ret)
{
	switch (ret) {
	case -3:
		return PSM2_EP_NO_DEVICE;
		break;
	case 0:
		return PSM2_OK;
		break;
	default:
		return PSM2_INTERNAL_ERR;
		break;
	}
}

psm2_error_t psm3_info_query(psm2_info_query_t q, void *out,
			       size_t nargs, psm2_info_query_arg_t args[])
{
	static const size_t expected_arg_cnt[PSM2_INFO_QUERY_LAST] =
	{
		0, /* PSM2_INFO_QUERY_NUM_UNITS         */
		0, /* PSM2_INFO_QUERY_NUM_PORTS         */
		1, /* PSM2_INFO_QUERY_UNIT_STATUS       */
		2, /* PSM2_INFO_QUERY_UNIT_PORT_STATUS  */
		1, /* PSM2_INFO_QUERY_NUM_FREE_CONTEXTS */
		1, /* PSM2_INFO_QUERY_NUM_CONTEXTS      */
		0, /* was PSM2_INFO_QUERY_CONFIG        */
		0, /* was PSM2_INFO_QUERY_THRESH        */
		0, /* was PSM2_INFO_QUERY_DEVICE_NAME   */
		0, /* was PSM2_INFO_QUERY_MTU           */
		0, /* was PSM2_INFO_QUERY_LINK_SPEED    */
		0, /* was PSM2_INFO_QUERY_NETWORK_TYPE  */
		0, /* PSM2_INFO_QUERY_FEATURE_MASK      */
		2, /* PSM2_INFO_QUERY_UNIT_NAME         */
		0, /* was PSM2_INFO_QUERY_UNIT_SYS_PATH */
		2, /* PSM2_INFO_QUERY_UNIT_PCI_BUS      */
		4, /* PSM2_INFO_QUERY_UNIT_SUBNET_NAME  */
		2, /* PSM2_INFO_QUERY_UNIT_DEVICE_ID    */
		2, /* PSM2_INFO_QUERY_UNIT_DEVICE_VERSION */
		2, /* PSM2_INFO_QUERY_UNIT_VENDOR_ID    */
		2, /* PSM2_INFO_QUERY_UNIT_DRIVER       */
		2, /* PSM2_INFO_QUERY_PORT_SPEED        */
		0, /* PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT */
		4, /* PSM2_INFO_QUERY_UNIT_ADDR_NAME    */
	};
	psm2_error_t rv = PSM2_INTERNAL_ERR;

	if (q >= 0 && q < PSM2_INFO_QUERY_LAST && nargs != expected_arg_cnt[q])
		return PSM2_PARAM_ERR;

	switch (q)
	{
	case PSM2_INFO_QUERY_NUM_UNITS:
		*((uint32_t*)out) = psmi_hal_get_num_units_();
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_NUM_PORTS:
		*((uint32_t*)out) = psmi_hal_get_num_ports_();
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_STATUS:
		*((uint32_t*)out) = psmi_hal_get_unit_active(args[0].unit);
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_PORT_STATUS:
		*((uint32_t*)out) = psmi_hal_get_port_active(args[0].unit,
								args[1].port);
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_NUM_FREE_CONTEXTS:
		*((uint32_t*)out) = psmi_hal_get_num_free_contexts(args[0].unit);
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_NUM_CONTEXTS:
		*((uint32_t*)out) = psmi_hal_get_num_contexts(args[0].unit);
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_FEATURE_MASK:
		{
#ifdef PSM_CUDA
		*((uint32_t*)out) = PSM2_INFO_QUERY_FEATURE_CUDA;
#elif defined(PSM_ONEAPI)
		*((uint32_t*)out) = PSM2_INFO_QUERY_FEATURE_ONEAPI;
#else
		*((uint32_t*)out) = 0;
#endif /* PSM_CUDA */
		}
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_NAME:
		{
			char         *hfiName       = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        hfiNameLength = args[1].length;

			snprintf(hfiName, hfiNameLength, "%s", psmi_hal_get_unit_name(unit));
			rv = PSM2_OK;
		}
		break;
	case PSM2_INFO_QUERY_UNIT_PCI_BUS:
		{
			uint32_t     *pciBus        = (uint32_t *)out;
			uint32_t      unit          = args[0].unit;

			if (args[1].length != (sizeof(uint32_t)*4)) break;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_pci_bus(unit, pciBus,
							pciBus+1, pciBus+2, pciBus+3));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_SUBNET_NAME:
		{
			char         *subnetName       = (char*)out;
			uint32_t      unit          = args[0].unit;
			uint32_t      port          = args[1].port;
			uint32_t      addr_index    = args[2].addr_index;
			size_t        subnetNameLength = args[3].length;

			if (psmi_hal_get_unit_active(unit) <= 0) break;

			if (psmi_hal_get_port_subnet_name(unit, port, addr_index,
						subnetName, subnetNameLength))
				break;
			rv = PSM2_OK;
		}
		break;
	case PSM2_INFO_QUERY_UNIT_DEVICE_ID:
		{
			char         *devId         = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_device_id(unit, devId, len));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_DEVICE_VERSION:
		{
			char         *devVer        = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_device_version(unit, devVer, len));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_VENDOR_ID:
		{
			char         *venId         = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_vendor_id(unit, venId, len));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_DRIVER:
		{
			char         *driver        = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_driver(unit, driver, len));
		}
		break;
	case PSM2_INFO_QUERY_PORT_SPEED:
		{
			uint64_t *speed = (uint64_t *)out;
			uint32_t  unit  = args[0].unit;
			uint32_t  port  = args[1].port;

			if (port == 0) port = 1; /* VERBS_PORT */

			if (unit == -1) {
				// query for unit -1 returns max speed of all candidate NICs
				*speed = 0;
				for (unit = 0; unit < psmi_hal_get_num_units_(); unit++) {
					uint64_t unit_speed;
					if (psmi_hal_get_port_lid(unit, port, 0/*addr_index*/) <= 0)
						continue;
					if (0 <= psmi_hal_get_port_speed(unit, port, &unit_speed))
						*speed = max(*speed, unit_speed);
				}
				rv = (*speed) ? PSM2_OK : PSM2_EP_NO_DEVICE;
			} else {
				if (psmi_hal_get_port_active(unit, port) <= 0) break;

				rv = unit_query_ret_to_err(psmi_hal_get_port_speed(unit, port, speed));
			}
		}
		break;
	case PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT:
		*((uint32_t*)out) = psm3_addr_per_nic;
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_ADDR_NAME:
		{
			char         *addrName       = (char*)out;
			uint32_t      unit          = args[0].unit;
			//uint32_t      port          = args[1].port;	/* ignored, should be VERBS_PORT */
			uint32_t      addr_index    = args[2].addr_index;
			size_t        addrNameLength = args[3].length;

			if (psm3_addr_per_nic > 1)
				snprintf(addrName, addrNameLength, "%s-%u", psmi_hal_get_unit_name(unit), addr_index);
			else
				snprintf(addrName, addrNameLength, "%s", psmi_hal_get_unit_name(unit));
			rv = PSM2_OK;
		}
		break;
	default:
		return 	PSM2_IQ_INVALID_QUERY;
	}

	return rv;
}

uint64_t psm3_get_capability_mask(uint64_t req_cap_mask)
{
	return (psm3_capabilities_bitset & req_cap_mask);
}

psm2_error_t psm3_finalize(void)
{
	struct psmi_eptab_iterator itor;
	char *hostname;
	psm2_ep_t ep;

	PSM2_LOG_MSG("entering");

	_HFI_DBG("psmi_refcount=%d\n", psmi_refcount);
	PSMI_ERR_UNLESS_INITIALIZED(NULL);
	psmi_assert(psmi_refcount > 0);
	psmi_refcount--;

	if (psmi_refcount > 0) {
		return PSM2_OK;
	}

	/* When PSM_PERF is enabled, the following line causes the
	   instruction cycles gathered in the current run to be dumped
	   to stderr. */
	GENERIC_PERF_DUMP(stderr);
	ep = psm3_opened_endpoint;
	while (ep != NULL) {
		psm2_ep_t saved_ep = ep->user_ep_next;
		psm3_ep_close(ep, PSM2_EP_CLOSE_GRACEFUL,
			     2 * PSMI_MIN_EP_CLOSE_TIMEOUT);
		psm3_opened_endpoint = ep = saved_ep;
	}

#ifdef PSM_DSA
	psm3_dsa_fini();	// noop if didn't successfully psm3_dsa_init
#endif

#ifdef PSM_FI
	psm3_faultinj_fini();
#endif /* #ifdef PSM_FI */

	/* De-allocate memory for any allocated space to store hostnames */
	psm3_epid_itor_init(&itor, PSMI_EP_HOSTNAME);
	while ((hostname = psm3_epid_itor_next(&itor)))
		psmi_free(hostname);
	psm3_epid_itor_fini(&itor);

	psm3_epid_fini();

	/* unmap shared mem object for affinity */
	if (psm3_affinity_shared_file_opened) {
		/*
		 * Start critical section to decrement ref count and unlink
		 * affinity shm file.
		 */
		psmi_sem_timedwait(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);

		psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION] -= 1;
		if (psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION] <= 0) {
			_HFI_VDBG("Unlink shm file for NIC affinity as there are no more users\n");
			shm_unlink(psm3_affinity_shm_name);
		} else {
			_HFI_VDBG("Number of affinity shared memory users left=%ld\n",
				  psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION]);
		}

		msync(psm3_shared_affinity_ptr, PSMI_PAGESIZE, MS_SYNC);

		/* End critical section */
		psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);

		munmap(psm3_shared_affinity_ptr, PSMI_PAGESIZE);
		psm3_shared_affinity_ptr = NULL;
		psmi_free(psm3_affinity_shm_name);
		psm3_affinity_shm_name = NULL;
		psm3_affinity_shared_file_opened = 0;
	}

	if (psm3_affinity_semaphore_open) {
		_HFI_VDBG("Closing and Unlinking Semaphore: %s.\n", psm3_sem_affinity_shm_rw_name);
		sem_close(psm3_sem_affinity_shm_rw);
		psm3_sem_affinity_shm_rw = NULL;
		sem_unlink(psm3_sem_affinity_shm_rw_name);
		psmi_free(psm3_sem_affinity_shm_rw_name);
		psm3_sem_affinity_shm_rw_name = NULL;
		psm3_affinity_semaphore_open = 0;
	}

	psm3_hal_finalize();
#ifdef PSM_CUDA
	if (PSMI_IS_GPU_ENABLED)
		psm3_stats_deregister_type(PSMI_STATSTYPE_GPU, &psmi_count_cuInit);
#elif defined(PSM_ONEAPI)
	if (PSMI_IS_GPU_ENABLED) {
		psm3_stats_deregister_type(PSMI_STATSTYPE_GPU, &psmi_count_zeInit);
		/*
		 * Trying to destroy command list and queue will result in
		 *  segfaults here.
		 */
		//psmi_oneapi_cmd_destroy();
		if (ze_context) {
			PSMI_ONEAPI_ZE_CALL(zeContextDestroy, ze_context);
			ze_context = NULL;
		}
	}
#endif // PSM_CUDA

	psmi_refcount = PSMI_FINALIZED;
	PSM2_LOG_MSG("leaving");
	psmi_log_fini();

	psm3_stats_finalize();

	psmi_heapdebug_finalize();

	return PSM2_OK;
}

/*
 * Function exposed in >= 1.05
 */
psm2_error_t
psm3_map_nid_hostname(int num, const psm2_nid_t *nids, const char **hostnames)
{
	int i;
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");

	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (nids == NULL || hostnames == NULL) {
		err = PSM2_PARAM_ERR;
		goto fail;
	}

	for (i = 0; i < num; i++) {
		if ((err = psm3_epid_set_hostname(nids[i], hostnames[i], 1)))
			break;
	}

fail:
	PSM2_LOG_MSG("leaving");
	return err;
}

void psm3_epaddr_setlabel(psm2_epaddr_t epaddr, char const *epaddr_label)
{
	PSM2_LOG_MSG("entering");
	PSM2_LOG_MSG("leaving");
	return;			/* ignore this function */
}

void psm3_epaddr_setctxt(psm2_epaddr_t epaddr, void *ctxt)
{
	uint64_t optlen = sizeof(void *);
	/* Eventually deprecate this API to use set/get opt as this is unsafe. */
	PSM2_LOG_MSG("entering");
	psm3_setopt(PSM2_COMPONENT_CORE, (const void *)epaddr,
		   PSM2_CORE_OPT_EP_CTXT, (const void *)ctxt, optlen);
	PSM2_LOG_MSG("leaving");
}

void *psm3_epaddr_getctxt(psm2_epaddr_t epaddr)
{
	psm2_error_t err;
	uint64_t optlen = sizeof(void *);
	void *result = NULL;

	PSM2_LOG_MSG("entering");
	/* Eventually deprecate this API to use set/get opt as this is unsafe. */
	err = psm3_getopt(PSM2_COMPONENT_CORE, (const void *)epaddr,
			 PSM2_CORE_OPT_EP_CTXT, (void *)&result, &optlen);

	PSM2_LOG_MSG("leaving");

	if (err == PSM2_OK)
		return result;
	else
		return NULL;
}

psm2_error_t
psm3_setopt(psm2_component_t component, const void *component_obj,
	     int optname, const void *optval, uint64_t optlen)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	switch (component) {
	case PSM2_COMPONENT_CORE:
		rv = psm3_core_setopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_MQ:
		/* Use the deprecated MQ set/get opt for now which does not use optlen */
		rv = psm3_mq_setopt((psm2_mq_t) component_obj, optname, optval);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_AM:
		/* Hand off to active messages */
		rv = psm3_am_setopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_IB:
		/* Hand off to IPS ptl to set option */
		rv = psm3_ptl_ips.setopt(component_obj, optname, optval,
					   optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	}

	/* Unrecognized/unknown component */
	rv = psm3_handle_error(NULL, PSM2_PARAM_ERR, "Unknown component %u",
				 component);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t
psm3_getopt(psm2_component_t component, const void *component_obj,
	     int optname, void *optval, uint64_t *optlen)
{
	psm2_error_t rv;

	PSM2_LOG_MSG("entering");
	switch (component) {
	case PSM2_COMPONENT_CORE:
		rv = psm3_core_getopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_MQ:
		/* Use the deprecated MQ set/get opt for now which does not use optlen */
		rv = psm3_mq_getopt((psm2_mq_t) component_obj, optname, optval);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_AM:
		/* Hand off to active messages */
		rv = psm3_am_getopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_IB:
		/* Hand off to IPS ptl to set option */
		rv = psm3_ptl_ips.getopt(component_obj, optname, optval,
					   optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	}

	/* Unrecognized/unknown component */
	rv = psm3_handle_error(NULL, PSM2_PARAM_ERR, "Unknown component %u",
				 component);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t psm3_poll_noop(ptl_t *ptl, int replyonly, bool force)
{
	PSM2_LOG_MSG("entering");
	PSM2_LOG_MSG("leaving");
	return PSM2_OK_NO_PROGRESS;
}

psm2_error_t psm3_poll(psm2_ep_t ep)
{
	psm2_error_t err1 = PSM2_OK, err2 = PSM2_OK;
	psm2_ep_t tmp;

	PSM2_LOG_MSG("entering");

	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK(ep->mq->progress_lock);

	tmp = ep;
	do {
		err1 = ep->ptl_amsh.ep_poll(ep->ptl_amsh.ptl, 0, 0);	/* poll reqs & reps */
		if (err1 > PSM2_OK_NO_PROGRESS) {	/* some error unrelated to polling */
			PSMI_UNLOCK(ep->mq->progress_lock);
			PSM2_LOG_MSG("leaving");
			return err1;
		}

		err2 = ep->ptl_ips.ep_poll(ep->ptl_ips.ptl, 0, 0);	/* get into ips_do_work */
		if (err2 > PSM2_OK_NO_PROGRESS) {	/* some error unrelated to polling */
			PSMI_UNLOCK(ep->mq->progress_lock);
			PSM2_LOG_MSG("leaving");
			return err2;
		}
		ep = ep->mctxt_next;
	} while (ep != tmp);

	/* This is valid because..
	 * PSM2_OK & PSM2_OK_NO_PROGRESS => PSM2_OK
	 * PSM2_OK & PSM2_OK => PSM2_OK
	 * PSM2_OK_NO_PROGRESS & PSM2_OK => PSM2_OK
	 * PSM2_OK_NO_PROGRESS & PSM2_OK_NO_PROGRESS => PSM2_OK_NO_PROGRESS */
	PSMI_UNLOCK(ep->mq->progress_lock);
	PSM2_LOG_MSG("leaving");
	return (err1 & err2);
}

psm2_error_t psm3_poll_internal(psm2_ep_t ep, int poll_amsh, bool force)
{
	psm2_error_t err1 = PSM2_OK_NO_PROGRESS;
	psm2_error_t err2;
	psm2_ep_t tmp;

	PSM2_LOG_MSG("entering");
	PSMI_LOCK_ASSERT(ep->mq->progress_lock);

	tmp = ep;
	do {
		if (poll_amsh) {
			err1 = ep->ptl_amsh.ep_poll(ep->ptl_amsh.ptl, 0, force);	/* poll reqs & reps */
			if (err1 > PSM2_OK_NO_PROGRESS) { /* some error unrelated to polling */
				PSM2_LOG_MSG("leaving");
				return err1;
			}
		}

		err2 = ep->ptl_ips.ep_poll(ep->ptl_ips.ptl, 0, force);	/* get into ips_do_work */
		if (err2 > PSM2_OK_NO_PROGRESS) { /* some error unrelated to polling */
			PSM2_LOG_MSG("leaving");
			return err2;
		}

		ep = ep->mctxt_next;
	} while (ep != tmp);
	PSM2_LOG_MSG("leaving");
	return (err1 & err2);
}
#ifdef PSM_PROFILE
/* These functions each have weak symbols */
void psmi_profile_block()
{
	;			/* empty for profiler */
}

void psmi_profile_unblock()
{
	;			/* empty for profiler */
}

void psmi_profile_reblock(int did_no_progress)
{
	;			/* empty for profiler */
}
#endif
