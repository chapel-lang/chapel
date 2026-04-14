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

#include "psm_user.h"

#ifdef PSM_ONEAPI
#include <dlfcn.h>
#include <level_zero/ze_api.h>
#include <level_zero/loader/ze_loader.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <sys/poll.h>
#include "psm2_hal.h"
#include "psm_mq_internal.h"
#include "ptl_am/psm_am_internal.h"
#include "ptl_ips/ips_proto.h"
#include "ptl_ips/ips_expected_proto.h"
#include "psmi_wrappers.h"
#include <fcntl.h>
#ifdef HAVE_DRM
#include <sys/ioctl.h>
#include <drm/i915_drm.h>
#endif
#ifdef HAVE_LIBDRM
#include <sys/ioctl.h>
#include <libdrm/i915_drm.h>
#endif
#ifdef PSM_HAVE_PIDFD
#include <sys/syscall.h>
#endif

// if defined, use malloc for pipeline copy bounce buffers
// otherwise, use zeMemAllocHost
//#define PSM3_USE_ONEAPI_MALLOC

// if defined, do not use zexDriverImportExternalPointer for malloced pipeline
// copy bounce buffers
// otherwise, use zexDriverImportExternalPointer when malloc buffer
//#define PSM3_NO_ONEAPI_IMPORT

// default value for PSM3_GPU_THRESH_RNDV
#define PSM3_ONEAPI_ZE_GPU_THRESH_RNDV 8000
// default value for PSM3_GPU_RNDV_NIC_WINDOW when using OneApi Level Zero GPU
#define PSM3_ONEAPI_ZE_RNDV_NIC_WINDOW_DEFAULT "131072:524287,262144:1048575,524288"
// default value for PSM3_GPUDIRECT_RDMA_SEND_LIMIT
#define PSM3_ONEAPI_ZE_GPUDIRECT_RDMA_SEND_LIMIT_DEFAULT UINT_MAX
// default value for PSM3_GPUDIRECT_RDMA_RECV_LIMIT
#define PSM3_ONEAPI_ZE_GPUDIRECT_RDMA_RECV_LIMIT_DEFAULT 1
// default value for PSM3_MQ_RNDV_SHM_GPU_THRESH
// Threshold for GPU rendezvous (aka scale-up transfer vs via CPU shared mem
#define PSM3_ONEAPI_ZE_MQ_RNDV_SHM_GPU_THRESH 127

struct psm3_oneapi_ze_dev_ctxt {
	ze_device_handle_t dev;
	int dev_index; /* Index in psm3_oneapi_ze_devices[] */
	uint32_t ordinal; /* CmdQGrp ordinal for the 1st copy_only engine */
	uint32_t index;   /* Cmdqueue index within the CmdQGrp */
	uint32_t num_queues; /* Number of queues in the CmdQGrp */
	// for most sync copies
	ze_command_queue_handle_t cq;	// NULL if psm3_oneapi_ze_immed_sync_copy
	ze_command_list_handle_t cl;
	// fields below are only used for large DTOD sync copy so can do 2
	// parallel async copies then wait for both
	ze_event_handle_t copy_status0;
	ze_event_handle_t copy_status1;
	ze_command_list_handle_t async_cl0;
	ze_command_list_handle_t async_cl1;
	ze_command_queue_handle_t async_cq0;// NULL if psm3_oneapi_ze_immed_sync_copy
	ze_command_queue_handle_t async_cq1;// NULL if psm3_oneapi_ze_immed_sync_copy
	ze_event_pool_handle_t event_pool;
};

static ze_driver_handle_t psm3_oneapi_ze_driver = NULL;
static struct psm3_oneapi_ze_dev_ctxt psm3_oneapi_ze_devices[MAX_ZE_DEVICES];
static int psm3_num_oneapi_ze_devices = 0;
static struct psm3_oneapi_ze_dev_ctxt *psm3_oneapi_ze_cur_dev = NULL;

/* ZE Loader(zel) And Runtime(ze) Library */
static void *psm3_oneapi_ze_lib;
static ze_api_version_t psm3_oneapi_ze_api_version = 0;
static zel_version_t psm3_oneapi_ze_lib_version = { };

/* This is a global oneapi_ze context
 */
static ze_context_handle_t psm3_oneapi_ze_context = NULL;

#ifndef PSM_HAVE_PIDFD
static int psm3_ze_dev_fds[MAX_ZE_DEVICES];
static int psm3_num_ze_dev_fds;
#endif
static int psm3_oneapi_ze_immed_sync_copy;
static int psm3_oneapi_ze_immed_async_copy;
static unsigned psm3_oneapi_parallel_dtod_copy_thresh;

#ifdef PSM_HAVE_RNDV_MOD
// PSM3_RV_GPU_IGNORE_ALLOC_ID allows internal testing of GPU caching in RV
// =0 -> default, alloc_id used to identify new buffers which have same
// 		virt addr as an existing cache entry.  In which case a cache miss
// 		and invalidation of the old cache entry occurs.
// =1 -> an alloc_id of 0 is always used.  This has been demonstrated to
// 		cause false cache hits which can lead to landing data in safe but
// 		incorrect pages.  Useful only for development experiments and tests.
// =2 -> for cache miss performance testing.  This will use a different alloc_id
// 		per IO which will force cache invalidation on every IO.  So no
// 		MR/mmap cache hits will occur, but all the normal MR handling will
// 		occur just as if there was a miss when running in normal mode
static int psm3_oneapi_ze_ignore_alloc_id;	// PSM3_RV_GPU_IGNORE_ALLOC_ID
static uint64_t psm3_oneapi_ze_fake_alloc_id;	// for when PSM3_RV_GPU_IGNORE_ALLOC_ID==2
#endif

/* function pointers from dlopen access to oneapi_Ze shared library */
#define PSM3_ZE_SYM_FP(name) PSM3_CONCAT(psm3_oneapi_ze_, name)
static ze_result_t (*PSM3_ZE_SYM_FP(zeInit))(ze_init_flags_t flags);
static ze_result_t (*PSM3_ZE_SYM_FP(zeDriverGet))(uint32_t *pCount, ze_driver_handle_t *phDrivers);
static ze_result_t (*PSM3_ZE_SYM_FP(zeDeviceGet))(ze_driver_handle_t hDriver, uint32_t *pCount, ze_device_handle_t *phDevices);
static ze_result_t (*PSM3_ZE_SYM_FP(zeDevicePciGetPropertiesExt))(ze_device_handle_t hDevice, ze_pci_ext_properties_t *pPciProperties);
#ifndef PSM3_NO_ONEAPI_IMPORT
static ze_result_t (*PSM3_ZE_SYM_FP(zeDriverGetExtensionFunctionAddress))(ze_driver_handle_t hDriver, const char *name, void **ppFunctionAddress);
static ze_result_t (*PSM3_ZE_SYM_FP(zexDriverImportExternalPointer))(ze_driver_handle_t hDriver, void *ptr, size_t size);
static ze_result_t (*PSM3_ZE_SYM_FP(zexDriverReleaseImportedPointer))(ze_driver_handle_t hDriver, void *ptr);
#endif
static ze_result_t (*PSM3_ZE_SYM_FP(zeContextCreate))(ze_driver_handle_t hDriver, const ze_context_desc_t *desc, ze_context_handle_t *phContext);
static ze_result_t (*PSM3_ZE_SYM_FP(zeContextDestroy))(ze_context_handle_t hContext);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandQueueCreate))(ze_context_handle_t hContext, ze_device_handle_t hDevice,const ze_command_queue_desc_t *desc, ze_command_queue_handle_t *phCommandQueue);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandQueueDestroy))(ze_command_queue_handle_t hCommandQueue);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandQueueExecuteCommandLists))(ze_command_queue_handle_t hCommandQueue, uint32_t numCommandLists, ze_command_list_handle_t *phCommandLists, ze_fence_handle_t hFence);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandQueueSynchronize))(ze_command_queue_handle_t hCommandQueue, uint64_t timeout);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListCreate))(ze_context_handle_t hContext, ze_device_handle_t hDevice, const ze_command_list_desc_t *desc, ze_command_list_handle_t *phCommandList);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListDestroy))(ze_command_list_handle_t hCommandList);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListClose))(ze_command_list_handle_t hCommandList);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListReset))(ze_command_list_handle_t hCommandList);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListCreateImmediate))(ze_context_handle_t hContext, ze_device_handle_t hDevice, const ze_command_queue_desc_t *desc, ze_command_list_handle_t *phCommandList);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListAppendMemoryCopy))(ze_command_list_handle_t hCommandList, void *dstptr, const void *srcptr, size_t size, ze_event_handle_t hSignalEvent, uint32_t numWaitEvents, ze_event_handle_t *phWaitEvents);
static ze_result_t (*PSM3_ZE_SYM_FP(zeCommandListAppendSignalEvent))(ze_command_list_handle_t hCommandList, ze_event_handle_t hEvent);
static ze_result_t (*PSM3_ZE_SYM_FP(zeDeviceCanAccessPeer))(ze_device_handle_t hDevice, ze_device_handle_t hPeerDevice, ze_bool_t *value);
static ze_result_t (*PSM3_ZE_SYM_FP(zeDeviceGetCommandQueueGroupProperties))(ze_device_handle_t hDevice, uint32_t *pCount, ze_command_queue_group_properties_t *pCommandQueueGroupProperties);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemAllocHost))(ze_context_handle_t hContext, const ze_host_mem_alloc_desc_t *host_desc, size_t size, size_t alignment, void **pptr);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemAllocDevice))(ze_context_handle_t hContext, const ze_device_mem_alloc_desc_t *device_desc, size_t size, size_t alignment, ze_device_handle_t hDevice, void **pptr);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemFree))(ze_context_handle_t hContext, void *ptr);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemGetIpcHandle))(ze_context_handle_t hContext, const void *ptr, ze_ipc_mem_handle_t *pIpcHandle);
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemPutIpcHandle))(ze_context_handle_t hContext, ze_ipc_mem_handle_t handle);
#endif
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemOpenIpcHandle))(ze_context_handle_t hContext,ze_device_handle_t hDevice, ze_ipc_mem_handle_t handle, ze_ipc_memory_flags_t flags, void **pptr);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemCloseIpcHandle))(ze_context_handle_t hContext, const void *ptr);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemGetAddressRange))(ze_context_handle_t hContext, const void *ptr, void **pBase, size_t *pSize);
static ze_result_t (*PSM3_ZE_SYM_FP(zeMemGetAllocProperties))(ze_context_handle_t hContext, const void *ptr, ze_memory_allocation_properties_t *pMemAllocProperties, ze_device_handle_t *phDevice);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventPoolCreate))(ze_context_handle_t hContext, const ze_event_pool_desc_t *desc, uint32_t numDevices, ze_device_handle_t *phDevices, ze_event_pool_handle_t *phEventPool);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventPoolDestroy))(ze_event_pool_handle_t hEventPool);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventCreate))(ze_event_pool_handle_t hEventPool, const ze_event_desc_t *desc, ze_event_handle_t *phEvent);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventDestroy))(ze_event_handle_t hEvent);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventQueryStatus))(ze_event_handle_t hEvent);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventHostSynchronize))(ze_event_handle_t hEvent, uint64_t timeout);
static ze_result_t (*PSM3_ZE_SYM_FP(zeEventHostReset))(ze_event_handle_t hEvent);
static ze_result_t (*PSM3_ZE_SYM_FP(zelLoaderGetVersions))(size_t *num_elems, zel_component_version_t *versions);

/* statistics counting each oneapi_ze call PSM3 makes */
#define PSM3_ZE_SYM_COUNT(name) PSM3_CONCAT(psm3_oneapi_ze_count_, name)
static uint64_t PSM3_ZE_SYM_COUNT(zeInit);
static uint64_t PSM3_ZE_SYM_COUNT(zeDriverGet);
static uint64_t PSM3_ZE_SYM_COUNT(zeDeviceGet);
static uint64_t PSM3_ZE_SYM_COUNT(zeDevicePciGetPropertiesExt);
#ifndef PSM3_NO_ONEAPI_IMPORT
static uint64_t PSM3_ZE_SYM_COUNT(zeDriverGetExtensionFunctionAddress);
static uint64_t PSM3_ZE_SYM_COUNT(zexDriverImportExternalPointer);
static uint64_t PSM3_ZE_SYM_COUNT(zexDriverReleaseImportedPointer);
#endif
static uint64_t PSM3_ZE_SYM_COUNT(zeContextCreate);
static uint64_t PSM3_ZE_SYM_COUNT(zeContextDestroy);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandQueueCreate);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandQueueDestroy);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandQueueExecuteCommandLists);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandQueueSynchronize);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListCreate);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListDestroy);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListClose);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListReset);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListCreateImmediate);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListAppendMemoryCopy);
static uint64_t PSM3_ZE_SYM_COUNT(zeCommandListAppendSignalEvent);
static uint64_t PSM3_ZE_SYM_COUNT(zeDeviceCanAccessPeer);
static uint64_t PSM3_ZE_SYM_COUNT(zeDeviceGetCommandQueueGroupProperties);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemAllocHost);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemAllocDevice);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemFree);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemGetIpcHandle);
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
static uint64_t PSM3_ZE_SYM_COUNT(zeMemPutIpcHandle);
#endif
static uint64_t PSM3_ZE_SYM_COUNT(zeMemOpenIpcHandle);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemCloseIpcHandle);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemGetAddressRange);
static uint64_t PSM3_ZE_SYM_COUNT(zeMemGetAllocProperties);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventPoolCreate);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventPoolDestroy);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventCreate);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventDestroy);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventQueryStatus);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventHostSynchronize);
static uint64_t PSM3_ZE_SYM_COUNT(zeEventHostReset);
static uint64_t PSM3_ZE_SYM_COUNT(zelLoaderGetVersions);

static const char* psm3_oneapi_ze_result_to_string(const ze_result_t result) {
#define ZE_RESULT_CASE(RES) case ZE_RESULT_##RES: return STRINGIFY(RES)

	switch (result) {
	ZE_RESULT_CASE(SUCCESS);
	ZE_RESULT_CASE(NOT_READY);
	ZE_RESULT_CASE(ERROR_UNINITIALIZED);
	ZE_RESULT_CASE(ERROR_DEVICE_LOST);
	ZE_RESULT_CASE(ERROR_INVALID_ARGUMENT);
	ZE_RESULT_CASE(ERROR_OUT_OF_HOST_MEMORY);
	ZE_RESULT_CASE(ERROR_OUT_OF_DEVICE_MEMORY);
	ZE_RESULT_CASE(ERROR_MODULE_BUILD_FAILURE);
	ZE_RESULT_CASE(ERROR_INSUFFICIENT_PERMISSIONS);
	ZE_RESULT_CASE(ERROR_NOT_AVAILABLE);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_VERSION);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_FEATURE);
	ZE_RESULT_CASE(ERROR_INVALID_NULL_HANDLE);
	ZE_RESULT_CASE(ERROR_HANDLE_OBJECT_IN_USE);
	ZE_RESULT_CASE(ERROR_INVALID_NULL_POINTER);
	ZE_RESULT_CASE(ERROR_INVALID_SIZE);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_SIZE);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_ALIGNMENT);
	ZE_RESULT_CASE(ERROR_INVALID_SYNCHRONIZATION_OBJECT);
	ZE_RESULT_CASE(ERROR_INVALID_ENUMERATION);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_ENUMERATION);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_IMAGE_FORMAT);
	ZE_RESULT_CASE(ERROR_INVALID_NATIVE_BINARY);
	ZE_RESULT_CASE(ERROR_INVALID_GLOBAL_NAME);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_NAME);
	ZE_RESULT_CASE(ERROR_INVALID_FUNCTION_NAME);
	ZE_RESULT_CASE(ERROR_INVALID_GROUP_SIZE_DIMENSION);
	ZE_RESULT_CASE(ERROR_INVALID_GLOBAL_WIDTH_DIMENSION);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_ARGUMENT_INDEX);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_ARGUMENT_SIZE);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE);
	ZE_RESULT_CASE(ERROR_INVALID_COMMAND_LIST_TYPE);
	ZE_RESULT_CASE(ERROR_OVERLAPPING_REGIONS);
	ZE_RESULT_CASE(ERROR_UNKNOWN);
	default:
		return "Unknown error";
	}

#undef ZE_RESULT_CASE
}

#define PSM3_ONEAPI_ZE_CALL(func, args...) do { \
	ze_result_t result; \
	PSM3_CONCAT(psm3_oneapi_ze_count_, func)++; \
	result = PSM3_CONCAT(psm3_oneapi_ze_, func)(args);	\
	if(result != ZE_RESULT_SUCCESS) { \
		_HFI_ERROR( "OneAPI Level Zero failure: %s() (at %s:%d)" \
			" returned 0x%x: %s\n", \
			#func, __FILE__, __LINE__, result, \
			psm3_oneapi_ze_result_to_string(result)); \
		psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, \
			"Error returned from OneAPI Level Zero function %s.\n", #func); \
	}							\
} while (0)

// resolve a OneAPI Level Zero shared library symbol
#define PSM3_ONEAPI_ZE_DLSYM(lib_ptr, func) do { \
	PSM3_CONCAT(psm3_oneapi_ze_, func) = dlsym(lib_ptr, STRINGIFY(func)); \
	if (!PSM3_CONCAT(psm3_oneapi_ze_, func)) { \
		psm3_handle_error(PSMI_EP_NORETURN, \
			PSM2_INTERNAL_ERR, \
			"Unable to resolve %s symbol " \
			"in OneAPI Level Zero library.\n", STRINGIFY(func)); \
	} \
} while (0)

static int psm3_oneapi_ze_lib_load()
{
	psm2_error_t err = PSM2_OK;
	char *dlerr;

	PSM2_LOG_MSG("entering");
	_HFI_VDBG("Loading OneAPI Level Zero library.\n");

	psm3_oneapi_ze_lib = dlopen("libze_loader.so.1", RTLD_LAZY);
	if (!psm3_oneapi_ze_lib) {
		dlerr = dlerror();
		_HFI_ERROR(
			"Unable to open libze_loader.so.1.  Error %s\n",
			dlerr ? dlerr : "no dlerror()");
		goto fail;
	}

	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeInit);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeDriverGet);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeDeviceGet);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeDevicePciGetPropertiesExt);
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeDriverGetExtensionFunctionAddress);
#endif
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeContextCreate);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeContextDestroy);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandQueueCreate);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandQueueDestroy);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandQueueExecuteCommandLists);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandQueueSynchronize);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListCreate);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListDestroy);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListClose);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListReset);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListCreateImmediate);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListAppendMemoryCopy);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeCommandListAppendSignalEvent);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeDeviceCanAccessPeer);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeDeviceGetCommandQueueGroupProperties);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemAllocHost);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemAllocDevice);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemFree);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemGetIpcHandle);
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemPutIpcHandle);
#endif
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemOpenIpcHandle);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemCloseIpcHandle);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemGetAddressRange);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeMemGetAllocProperties);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventPoolCreate);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventPoolDestroy);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventCreate);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventDestroy);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventQueryStatus);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventHostSynchronize);
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zeEventHostReset);

	/* ze loader API */
	PSM3_ONEAPI_ZE_DLSYM(psm3_oneapi_ze_lib, zelLoaderGetVersions);

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	if (psm3_oneapi_ze_lib)
		dlclose(psm3_oneapi_ze_lib);
	err = psm3_handle_error(PSMI_EP_NORETURN,
		PSM2_INTERNAL_ERR,
		"Unable to load OneAPI Level Zero library.\n");
	return err;
}

static void psm3_oneapi_ze_stats_register()
{
#define PSM3_ONEAPI_ZE_COUNT_DECLU64(func) \
	PSMI_STATS_DECLU64(#func, NULL, &PSM3_CONCAT(psm3_oneapi_ze_count_, func))

	struct psmi_stats_entry ze_entries[] = {
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeInit),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeDriverGet),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeDeviceGet),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeDevicePciGetPropertiesExt),
#ifndef PSM3_NO_ONEAPI_IMPORT
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeDriverGetExtensionFunctionAddress),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zexDriverImportExternalPointer),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zexDriverReleaseImportedPointer),
#endif
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeContextCreate),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeContextDestroy),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueCreate),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueDestroy),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueExecuteCommandLists),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandQueueSynchronize),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListCreate),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListDestroy),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListClose),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListReset),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListCreateImmediate),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListAppendMemoryCopy),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeCommandListAppendSignalEvent),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeDeviceCanAccessPeer),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeDeviceGetCommandQueueGroupProperties),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemAllocHost),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemAllocDevice),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemFree),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemGetIpcHandle),
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemPutIpcHandle),
#endif
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemOpenIpcHandle),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemCloseIpcHandle),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemGetAddressRange),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeMemGetAllocProperties),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventPoolCreate),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventPoolDestroy),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventCreate),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventDestroy),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventQueryStatus),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventHostSynchronize),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zeEventHostReset),
		PSM3_ONEAPI_ZE_COUNT_DECLU64(zelLoaderGetVersions)
	};
#undef PSM3_ONEAPI_ZE_COUNT_DECLU64

	psm3_stats_register_type("PSM_OneAPI_ZE_call_statistics",
		"Count of OneAPI Level Zero calls per API entry point for the whole process.\n"
		"When using an Intel(r) GPU, PSM3 may call Level Zero "
		"APIs to access or transfer application buffers in GPU memory.",
		PSMI_STATSTYPE_GPU,
		ze_entries, PSMI_HOWMANY(ze_entries), NULL,
		&psm3_oneapi_ze_count_zeInit, NULL); /* context must != NULL */
}

static void psm3_oneapi_ze_find_copy_only_engine(ze_device_handle_t dev,
						struct psm3_oneapi_ze_dev_ctxt *ctxt)
{
	uint32_t count = 0;
	ze_command_queue_group_properties_t *props = NULL;
	int i;
	int done = 0;

	/* Set the default */
	ctxt->ordinal = 0;
	ctxt->index = 0;
	ctxt->num_queues = 1;
	PSM3_ONEAPI_ZE_CALL(zeDeviceGetCommandQueueGroupProperties, dev,
				&count, NULL);
	props = psmi_calloc(PSMI_EP_NONE, UNDEFINED, count, sizeof(*props));
	if (!props) {
		_HFI_ERROR("Failed to allocate mem for CmdQ Grp\n");
		return;
	}
	PSM3_ONEAPI_ZE_CALL(zeDeviceGetCommandQueueGroupProperties, dev,
				&count, props);

	// pick the last command queue group which supports copy but not compute.
	// For PVC this will be the xeLink copy engine which will also
	// have numQueues >1 (TBD - perhaps only select if it has numQueues>1).
	// This ordinal is then supplied to create Command Queues and Command Lists.
	for (i = count - 1; i >= 0; i--) {
		_HFI_DBG("GPU Queue Group %d: copy=%d Compute=%d num_queues=%d\n", i,
			(props[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY) != 0,
			(props[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE) != 0,
			(int)props[i].numQueues);
		if (! done && (props[i].flags &
						ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY) &&
			!(props[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE)) {
			ctxt->ordinal = i;
			ctxt->num_queues = props[i].numQueues;
			done = 1;
			if (_HFI_DBG_ON) {
				_HFI_DBG_ALWAYS("Selected GPU copy engine %d\n", i);
			} else {
				break;
			}
		}
	}
	psmi_free(props);
}

// for pipelined async GPU memcpy
// *p_cq is left as NULL when psm3_oneapi_ze_immed_async_copy enabled
static void psm3_oneapi_ze_async_cmd_create(struct psm3_oneapi_ze_dev_ctxt *ctxt,
		ze_command_queue_handle_t *p_cq, ze_command_list_handle_t *p_cl)
{
	psmi_assert(! *p_cl);
	if (psm3_oneapi_ze_immed_async_copy) {
		ze_command_queue_desc_t cq_desc = {
			.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
			.flags = 0,
			.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS,
			.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL
		};
		cq_desc.ordinal = ctxt->ordinal;
		cq_desc.index = ctxt->index++;
		ctxt->index %= ctxt->num_queues;
		PSM3_ONEAPI_ZE_CALL(zeCommandListCreateImmediate,
			psm3_oneapi_ze_context, ctxt->dev, &cq_desc, p_cl);
	} else {
		if (! *p_cq) {
			ze_command_queue_desc_t cq_desc = {
				.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
				.flags = 0,
				.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS,
				.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL
			};
			cq_desc.ordinal = ctxt->ordinal;
			cq_desc.index = ctxt->index++;
			ctxt->index %= ctxt->num_queues;
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueCreate,
					psm3_oneapi_ze_context, ctxt->dev, &cq_desc, p_cq);
		}
		ze_command_list_desc_t cl_desc = {
			.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
			.flags = 0
		};
		cl_desc.commandQueueGroupOrdinal = ctxt->ordinal;
		PSM3_ONEAPI_ZE_CALL(zeCommandListCreate,
			psm3_oneapi_ze_context, ctxt->dev, &cl_desc, p_cl);
	}
}

// create command queue for use in psm3_oneapi_ze_memcpy for sync memcpy
static void psm3_oneapi_ze_cmd_create(ze_device_handle_t dev, struct psm3_oneapi_ze_dev_ctxt *ctxt)
{
	ze_command_queue_desc_t ze_cq_desc = {
		.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
		.flags = 0,
		//.mode set below
		.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL,
	};

	psm3_oneapi_ze_find_copy_only_engine(dev, ctxt);
	ze_cq_desc.ordinal = ctxt->ordinal;
	ze_cq_desc.index = ctxt->index;

	if (psm3_oneapi_ze_immed_sync_copy) {
		ze_cq_desc.mode = ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS;
		PSM3_ONEAPI_ZE_CALL(zeCommandListCreateImmediate, psm3_oneapi_ze_context,
			dev, &ze_cq_desc, &ctxt->cl);
	} else {
		ze_command_list_desc_t ze_cl_desc = {
			.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
			.flags = 0
		};
		ze_cq_desc.mode = ZE_COMMAND_QUEUE_MODE_DEFAULT;

		PSM3_ONEAPI_ZE_CALL(zeCommandQueueCreate, psm3_oneapi_ze_context,
			dev, &ze_cq_desc, &ctxt->cq);

		ze_cl_desc.commandQueueGroupOrdinal = ctxt->ordinal;
		PSM3_ONEAPI_ZE_CALL(zeCommandListCreate, psm3_oneapi_ze_context,
			dev, &ze_cl_desc, &ctxt->cl);
	}
	ctxt->dev = dev;

	if (psm3_oneapi_parallel_dtod_copy_thresh < UINT_MAX) {
		// create resources for dual copy mechanism
		ze_event_pool_desc_t pool_desc = {
				.stype = ZE_STRUCTURE_TYPE_EVENT_POOL_DESC,
				.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE,
				.count = 2
		};
		ze_event_desc_t event_desc = {
				.stype = ZE_STRUCTURE_TYPE_EVENT_DESC,
				.signal = ZE_EVENT_SCOPE_FLAG_HOST,
				.wait = ZE_EVENT_SCOPE_FLAG_HOST,
		};
		PSM3_ONEAPI_ZE_CALL(zeEventPoolCreate,
				psm3_oneapi_ze_context, &pool_desc, 0, NULL, &ctxt->event_pool);

		event_desc.index = 0;
		PSM3_ONEAPI_ZE_CALL(zeEventCreate, ctxt->event_pool, &event_desc,
				&ctxt->copy_status0);

		event_desc.index = 1;
		PSM3_ONEAPI_ZE_CALL(zeEventCreate, ctxt->event_pool, &event_desc,
				&ctxt->copy_status1);

		psm3_oneapi_ze_async_cmd_create(ctxt, &ctxt->async_cq0,
				&ctxt->async_cl0);
		psm3_oneapi_ze_async_cmd_create(ctxt, &ctxt->async_cq1,
				&ctxt->async_cl1);
	}
}

static void psm3_oneapi_ze_cmd_create_all(void)
{
	int i;
	struct psm3_oneapi_ze_dev_ctxt *ctxt;
	ze_context_desc_t ctxtDesc = { ZE_STRUCTURE_TYPE_CONTEXT_DESC, NULL, 0 };

	if (!psm3_oneapi_ze_context)
		PSM3_ONEAPI_ZE_CALL(zeContextCreate, psm3_oneapi_ze_driver, &ctxtDesc,
				&psm3_oneapi_ze_context);

	for (i = 0; i < psm3_num_oneapi_ze_devices; i++) {
		ctxt = &psm3_oneapi_ze_devices[i];

		if (!ctxt->cl) {
			psm3_oneapi_ze_cmd_create(ctxt->dev, ctxt);
			_HFI_DBG("Initialized cmd queues for ze_device[%d] %p\n",
						i, ctxt->dev);
		}
	}
	if (psm3_num_oneapi_ze_devices > 0)
		psm3_oneapi_ze_cur_dev = &psm3_oneapi_ze_devices[0];
}

static void psm3_oneapi_ze_cmd_destroy_all(void)
{
	int i;
	struct psm3_oneapi_ze_dev_ctxt *ctxt;

	for (i = 0; i < psm3_num_oneapi_ze_devices; i++) {
		ctxt = &psm3_oneapi_ze_devices[i];

		if (ctxt->async_cl1 != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeCommandListDestroy, ctxt->async_cl1);
			ctxt->async_cl1 = NULL;
		}
		if (ctxt->async_cq1 != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueDestroy, ctxt->async_cq1);
			ctxt->async_cq1 = NULL;
		}
		if (ctxt->async_cl0 != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeCommandListDestroy, ctxt->async_cl0);
			ctxt->async_cl0 = NULL;
		}
		if (ctxt->async_cq0 != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueDestroy, ctxt->async_cq0);
			ctxt->async_cq0 = NULL;
		}
		if (ctxt->copy_status1 != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeEventDestroy, ctxt->copy_status1);
			ctxt->copy_status1 = NULL;
		}
		if (ctxt->copy_status0 != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeEventDestroy, ctxt->copy_status0);
			ctxt->copy_status0 = NULL;
		}
		if (ctxt->event_pool != NULL) {
			PSM3_ONEAPI_ZE_CALL(zeEventPoolDestroy, ctxt->event_pool);
			ctxt->event_pool = NULL;
		}
		if (ctxt->cl) {
			PSM3_ONEAPI_ZE_CALL(zeCommandListDestroy, ctxt->cl);
			ctxt->cl = NULL;
		}
		if (ctxt->cq) {
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueDestroy, ctxt->cq);
			ctxt->cq = NULL;
		}
	}
	psm3_oneapi_ze_cur_dev = NULL;

	/* Also destroy psm3_oneapi_ze_context */
	if (psm3_oneapi_ze_context) {
		PSM3_ONEAPI_ZE_CALL(zeContextDestroy, psm3_oneapi_ze_context);
		psm3_oneapi_ze_context = NULL;
	}
}

/*
 * get OneAPI alloc_id for a GPU address
 *
 * The address should be part of a buffer allocated from an OneAPI
 * library call (zeMemAllocDevice() or zeMemAllocHost()).
 * The alloc_id changes on each OneAPI allocation call. PSM3/rv uses the
 * alloc_id to determine if a cache hit is a potentially stale entry which
 * should be invalidated.
 */
static uint64_t psm3_oneapi_ze_get_alloc_id(void *addr, uint8_t *type)
{
	ze_memory_allocation_properties_t mem_props = {
		.stype = ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES
	};
	ze_device_handle_t device;

	PSM3_ONEAPI_ZE_CALL(zeMemGetAllocProperties, psm3_oneapi_ze_context,
				addr, &mem_props, &device);
	if (type)
		*type = (uint8_t)mem_props.type;
	/*
	 * id is unique across all allocates on all devices within a given
	 * process
	 */
	return mem_props.id;
}

//***************************************************************************
//OneAPI Level Zero support for IPC handles
//IPC Handles are used both for PSM3 shm intranode copies via xeLink
//as well as for dma_buf use during MR creation for GPU Direct DMA and RDMA
#ifndef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
static int psm3_oneapi_ze_ipc_handle_cached(const void *buf,
				ze_ipc_mem_handle_t ipc_handle)
{
	static int first = 1;
	static int cached = 0;
	ze_ipc_mem_handle_t tmp_ipc_handle;
	int tmp_fd;

	/* Only detect the first time */
	if (!first)
		return cached;

	PSM3_ONEAPI_ZE_CALL(zeMemGetIpcHandle, psm3_oneapi_ze_context,
				buf, &tmp_ipc_handle);
	tmp_fd = *(uint32_t *)tmp_ipc_handle.data;
	if (tmp_fd == *(uint32_t *)ipc_handle.data)
		cached = 1;
	else
		close(tmp_fd);

	first = 0;
	_HFI_VDBG("fd %u tmp_fd %d cached %d\n", *(uint32_t *)ipc_handle.data,
						tmp_fd, cached);

	return cached;
}
#endif

#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
#define ONEAPI_PUTQUEUE_SIZE -1

// queue for delayed Put to get better GetIpcHandle performance
// while having an upper bound on number of active Ipc Handles
// sized based on PSM3_ONEAPI_PUTQUEUE_SIZE
struct {
	psmi_lock_t lock;
	struct oneapi_handle_array {
		uint8_t valid;
		ze_ipc_mem_handle_t ze_ipc_handle;
	} *array;
	unsigned index;	// where to add next entry and remove oldest
	int size;	// number of slots in queue, -1 disables put
} psm3_oneapi_ze_putqueue;
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */

static psm2_error_t psm3_oneapi_ze_putqueue_alloc(void)
{
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	union psmi_envvar_val env;
	psm3_getenv("PSM3_ONEAPI_PUTQUEUE_SIZE",
				"How many Ipc Handle Puts to queue for shm send and nic Direct GPU Access [-1 disables Put, 0 disables queue]",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)ONEAPI_PUTQUEUE_SIZE, &env);
	_HFI_DBG("OneApi PutQueue Size=%d\n", env.e_int);
	psm3_oneapi_ze_putqueue.size = env.e_int;
	if (env.e_int > 0) {
		psm3_oneapi_ze_putqueue.array = (struct oneapi_handle_array *)psmi_calloc(
										PSMI_EP_NONE, UNDEFINED, env.e_int,
										sizeof(*psm3_oneapi_ze_putqueue.array));
		if (! psm3_oneapi_ze_putqueue.array)
			return PSM2_NO_MEMORY;
		psm3_oneapi_ze_putqueue.index = 0;
		psmi_init_lock(&psm3_oneapi_ze_putqueue.lock);
	}
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
	return PSM2_OK;
}

#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
static void psm3_oneapi_ze_get_dmabuf_fd(const void *buf, uint64_t *handle_fd)
{
	ze_memory_allocation_properties_t mem_props = {};
	ze_device_handle_t device_ptr;
	ze_external_memory_export_fd_t export_fd = {};

	export_fd.stype = ZE_STRUCTURE_TYPE_EXTERNAL_MEMORY_EXPORT_FD;
	export_fd.flags = ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF;

	mem_props.stype = ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES;
	mem_props.pNext = &export_fd;

	PSM3_ONEAPI_ZE_CALL(zeMemGetAllocProperties, psm3_oneapi_ze_context,
			    buf, &mem_props, &device_ptr);
	*handle_fd = export_fd.fd;
}
#endif

#ifdef PSM_HAVE_RNDV_MOD
static void psm3_oneapi_ze_get_ipc_handle(const void *buf, ze_ipc_mem_handle_t *ipc_handle, uint64_t *handle_fd)
{
	PSM3_ONEAPI_ZE_CALL(zeMemGetIpcHandle, psm3_oneapi_ze_context,
		(const void *)buf, ipc_handle);
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	psm3_oneapi_ze_get_dmabuf_fd(buf, handle_fd);
#else
	*handle_fd = *(uint32_t *)ipc_handle->data;
#endif
}
#endif /* PSM_HAVE_RNDV_MOD */

static void psm3_oneapi_ze_put_ipc_handle(const void *buf, ze_ipc_mem_handle_t ipc_handle)
{
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	if (! psm3_oneapi_ze_putqueue.array) {	// queue disabled
		if (psm3_oneapi_ze_putqueue.size >= 0)	// negative size disables Put
			PSM3_ONEAPI_ZE_CALL(zeMemPutIpcHandle, psm3_oneapi_ze_context, ipc_handle);
		return;
	}
	PSMI_LOCK(psm3_oneapi_ze_putqueue.lock);
	if (psm3_oneapi_ze_putqueue.array[psm3_oneapi_ze_putqueue.index].valid) {
		// Put the oldest one to make room for new entry
		ze_ipc_mem_handle_t tmp_ipc_handle =
			psm3_oneapi_ze_putqueue.array[psm3_oneapi_ze_putqueue.index].ze_ipc_handle;
		PSM3_ONEAPI_ZE_CALL(zeMemPutIpcHandle, psm3_oneapi_ze_context, tmp_ipc_handle);
	}
	// queue the new one
	psm3_oneapi_ze_putqueue.array[psm3_oneapi_ze_putqueue.index].valid = 1;
	psm3_oneapi_ze_putqueue.array[psm3_oneapi_ze_putqueue.index++].ze_ipc_handle = ipc_handle;
	psm3_oneapi_ze_putqueue.index %= psm3_oneapi_ze_putqueue.size;
	PSMI_UNLOCK(psm3_oneapi_ze_putqueue.lock);
#else /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
	// for older Agama with handle "cache" but no reference counting
	// no way to put handle without affecting all IOs using that buffer
	// on ATS w/o Agama handle cache, no benefit to holding onto fd so close
	if (!psm3_oneapi_ze_ipc_handle_cached(buf, ipc_handle))
		close(*(uint32_t *)ipc_handle.data);
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
}

static void psm3_oneapi_ze_putqueue_free(void)
{
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
#if 0 // we are shutting down, so don't worry about Putting the queued handles
	int i;

	// no need for lock, destroying object, no more callers
	for (i=0; i < psm3_oneapi_ze_putqueue.size; i++) {
		if (psm3_oneapi_ze_putqueue.array[i].valid) {
			ze_ipc_mem_handle_t ipc_handle = psm3_oneapi_ze_putqueue.array[i].ze_ipc_handle;
			PSM3_ONEAPI_ZE_CALL(zeMemPutIpcHandle, psm3_oneapi_ze_context, ipc_handle);
		}
	}
#endif /* 0 */
	if (psm3_oneapi_ze_putqueue.array) {
		psmi_free(psm3_oneapi_ze_putqueue.array);
		psm3_oneapi_ze_putqueue.array = NULL;
		psmi_destroy_lock(&psm3_oneapi_ze_putqueue.lock);
	}
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
}
//***************************************************************************

static psm2_error_t psm3_oneapi_ze_initialize(void)
{
	psm2_error_t err = PSM2_OK;
	uint32_t ze_driver_count = 1;
	uint32_t ze_device_count = 0;
	ze_device_handle_t devices[MAX_ZE_DEVICES];
	zel_component_version_t *zel_comps = NULL;
	size_t num_zel_comps;
	int i;
	union psmi_envvar_val env;

	PSM2_LOG_MSG("entering");
	_HFI_DBG("Init Level Zero library.\n");

	psm3_oneapi_ze_stats_register();
	err = psm3_oneapi_ze_lib_load();
	if (err != PSM2_OK)
		goto fail;

	psm3_getenv("PSM3_ONEAPI_IMMED_SYNC_COPY",
				"Use Immediate CommandList for synchronous copy to/from GPU]",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)1, &env);
	psm3_oneapi_ze_immed_sync_copy = env.e_int;

	psm3_getenv("PSM3_ONEAPI_IMMED_ASYNC_COPY",
				"Use Immediate CommandList for asynchronous pipeline copy to/from GPU]",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)1, &env);
	psm3_oneapi_ze_immed_async_copy = env.e_int;

	psm3_getenv("PSM3_ONEAPI_PARALLEL_DTOD_COPY_THRESH",
				"Use parallel CommandLists for GPU to GPU copy larger than threshold",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)(256*1024-1), &env);
	// no benefit below 128K-1, plus the copy is spilt at a 64K boundary
	psm3_oneapi_parallel_dtod_copy_thresh = max(128*1024-1, env.e_uint);


	PSM3_ONEAPI_ZE_CALL(zeInit, ZE_INIT_FLAG_GPU_ONLY);

	/* Need to query count before alloc array */
	PSM3_ONEAPI_ZE_CALL(zelLoaderGetVersions, &num_zel_comps, NULL);
	if (num_zel_comps > 0) {
		zel_comps = (zel_component_version_t *)psmi_calloc(
				PSMI_EP_NONE, UNDEFINED, sizeof(zel_component_version_t),
				num_zel_comps);
		PSM3_ONEAPI_ZE_CALL(zelLoaderGetVersions, &num_zel_comps, zel_comps);

		/* Loop looking for "loader" name */
		for (i = 0; i < num_zel_comps; i++) {
			if (!strncmp(zel_comps[i].component_name, "loader", sizeof("loader"))){
				psm3_oneapi_ze_lib_version = zel_comps[i].component_lib_version;
				psm3_oneapi_ze_api_version	= zel_comps[i].spec_version;
				break;
			}
		}
		psmi_free(zel_comps);
		if (i == num_zel_comps) {
			_HFI_DBG("WARNING: 'loader' not found among the %zd components reported"
				" by zelLoaderGetVersions, unable to report Level-Zero version",
				num_zel_comps);
		}
	} else {
		_HFI_DBG("WARNING: no components reported by zelLoaderGetVersions,"
			" unable to report Level-Zero version");
	}

	PSM3_ONEAPI_ZE_CALL(zeDriverGet, &ze_driver_count, &psm3_oneapi_ze_driver);
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSM3_ONEAPI_ZE_CALL(zeDriverGetExtensionFunctionAddress, psm3_oneapi_ze_driver, "zexDriverImportExternalPointer", (void **)&psm3_oneapi_ze_zexDriverImportExternalPointer);
	PSM3_ONEAPI_ZE_CALL(zeDriverGetExtensionFunctionAddress, psm3_oneapi_ze_driver, "zexDriverReleaseImportedPointer", (void **)&psm3_oneapi_ze_zexDriverReleaseImportedPointer);
#endif

	PSM3_ONEAPI_ZE_CALL(zeDeviceGet, psm3_oneapi_ze_driver, &ze_device_count, NULL);
	if (ze_device_count > MAX_ZE_DEVICES)
		ze_device_count = MAX_ZE_DEVICES;
	PSM3_ONEAPI_ZE_CALL(zeDeviceGet, psm3_oneapi_ze_driver, &ze_device_count, devices);

	ze_context_desc_t ctxtDesc = { ZE_STRUCTURE_TYPE_CONTEXT_DESC, NULL, 0 };
	PSM3_ONEAPI_ZE_CALL(zeContextCreate, psm3_oneapi_ze_driver, &ctxtDesc, &psm3_oneapi_ze_context);
	_HFI_DBG("ze_driver %p %u devices first device %p ze_context %p\n",
			psm3_oneapi_ze_driver, ze_device_count, devices[0], psm3_oneapi_ze_context);

	for (i = 0; i < ze_device_count; i++) {
		psm3_oneapi_ze_devices[i].dev_index = i;
		psm3_oneapi_ze_cmd_create(devices[i], &psm3_oneapi_ze_devices[i]);
		_HFI_DBG("Initialized cmd queues for ze_device[%d] %p\n",
				i, psm3_oneapi_ze_devices[i].dev);
	}

	psm3_num_oneapi_ze_devices = ze_device_count;
	if (psm3_num_oneapi_ze_devices > 0)
		psm3_oneapi_ze_cur_dev = &psm3_oneapi_ze_devices[0];

	err = psm3_oneapi_ze_putqueue_alloc();
	if (err != PSM2_OK)
		goto fail;

#ifndef PSM_HAVE_PIDFD
	psm3_num_ze_dev_fds = 0;
#endif

#ifdef PSM_HAVE_RNDV_MOD
	// these env only needed when rv being used, since hidden, always parse
	{
		union psmi_envvar_val env;

		psm3_getenv("PSM3_RV_GPU_IGNORE_ALLOC_ID",
				"Disable use of alloc_id to identify GPU MRs to invalidate in RV GPU cache. 1=ignore, 2=use fake id to get 100% miss",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)0, &env);
		psm3_oneapi_ze_ignore_alloc_id = env.e_int;
	}
#endif /* PSM_HAVE_RNDV_MOD */

	if (! psm3_gpu_thresh_rndv) // sockets HAL could set new default
		psm3_gpu_thresh_rndv = PSM3_ONEAPI_ZE_GPU_THRESH_RNDV;
	psm3_gpu_rndv_nic_window_default = PSM3_ONEAPI_ZE_RNDV_NIC_WINDOW_DEFAULT;
	psm3_gpu_gpudirect_rdma_send_limit_default = PSM3_ONEAPI_ZE_GPUDIRECT_RDMA_SEND_LIMIT_DEFAULT;
	psm3_gpu_gpudirect_rdma_recv_limit_default = PSM3_ONEAPI_ZE_GPUDIRECT_RDMA_RECV_LIMIT_DEFAULT;
	psm3_gpu_mq_rndv_shm_gpu_thresh_default = PSM3_ONEAPI_ZE_MQ_RNDV_SHM_GPU_THRESH;

	PSM2_LOG_MSG("leaving");
	return err;
fail:
	err = psm3_handle_error(PSMI_EP_NORETURN,
		PSM2_INTERNAL_ERR,
		"Unable to initialize PSM3 OneAPI Level Zero support.\n");
	return err;
}

static void psm3_oneapi_ze_finalize(void)
{
	psm3_stats_deregister_type(PSMI_STATSTYPE_GPU, &psm3_oneapi_ze_count_zeInit);
	/*
	 * Trying to destroy command list, queue, and context will result in
	 *  segfaults here.
	 */
	/*psm3_oneapi_ze_putqueue_free();
	psm3_oneapi_ze_cmd_destroy();
	if (psm3_oneapi_ze_context) {
		PSM3_ONEAPI_ZE_CALL(zeContextDestroy, psm3_oneapi_ze_context);
		psm3_oneapi_ze_context = NULL;
	} */
}

// called on every EP open
static void psm3_oneapi_ze_ep_open(void)
{
	/* Make sure ze_context and command queue/list are available.
	 * They could be destroyed when final EP is closed
	 * If another endpoint is opened after that, the code here can
	 * recreate the context, command queue and list.
	 */
	if (!psm3_oneapi_ze_cur_dev)
		psm3_oneapi_ze_cmd_create_all();
}

// called on final EP close
static void psm3_oneapi_ze_ep_close(void)
{
	/*
	 * It would be ideal to destroy the global command list, queue, and
	 * context in psm3_finalize() (via psm3_oneapi_ze_finalize).
	 * Unfortunately, it will cause segfaults in Level-zero library.
	 */
	psm3_oneapi_ze_putqueue_free();
	psm3_oneapi_ze_cmd_destroy_all();
}

static void psm3_oneapi_ze_identify(char *accel_vers, size_t size)
{
	char ze_api_ver[64] = "unknown";
	char ze_loader_ver[64] = "unknown";

	if (psm3_oneapi_ze_api_version)
		snprintf(ze_api_ver, sizeof(ze_api_ver), "%d.%d",
			ZE_MAJOR_VERSION(psm3_oneapi_ze_api_version), ZE_MINOR_VERSION(psm3_oneapi_ze_api_version));
	if (psm3_oneapi_ze_lib_version.major || psm3_oneapi_ze_lib_version.minor || psm3_oneapi_ze_lib_version.patch)
		snprintf(ze_loader_ver, sizeof(ze_loader_ver), "v%d.%d.%d",
			psm3_oneapi_ze_lib_version.major, psm3_oneapi_ze_lib_version.minor, psm3_oneapi_ze_lib_version.patch);
	snprintf(accel_vers, size, "%s %s Level-Zero Runtime %s (%s) built against interface %d.%d\n",
		psm3_get_mylabel(), psm3_ident_tag,
		ze_api_ver, ze_loader_ver,
		ZE_MAJOR_VERSION(ZE_API_VERSION_CURRENT), ZE_MINOR_VERSION(ZE_API_VERSION_CURRENT));
}

static void psm3_oneapi_ze_verify_GPU_capabilities(void)
{
	// nothing to do
}

static int psm3_oneapi_ze_p2p_supported()
{
	static int p2p_supported = -1; // -1 indicates "unset"
	uint32_t num_devices = 0;
	uint32_t dev;
	ze_device_handle_t devices[MAX_ZE_DEVICES];

	if (likely(p2p_supported > -1)) return p2p_supported;

	p2p_supported = 0;

	PSM3_ONEAPI_ZE_CALL(zeDeviceGet, psm3_oneapi_ze_driver, &num_devices, NULL);
	if (num_devices > MAX_ZE_DEVICES)
		num_devices = MAX_ZE_DEVICES;
	PSM3_ONEAPI_ZE_CALL(zeDeviceGet, psm3_oneapi_ze_driver, &num_devices, devices);

	for (dev = 0; dev < num_devices; dev++) {
		ze_device_handle_t device;
		device = devices[dev];

		if (num_devices > 1 && device != psm3_oneapi_ze_cur_dev->dev) {
			ze_bool_t canAccessPeer = 0;

			PSM3_ONEAPI_ZE_CALL(zeDeviceCanAccessPeer, psm3_oneapi_ze_cur_dev->dev,
					device, &canAccessPeer);
			if (canAccessPeer != 1)
				_HFI_DBG("ONEAPI device %d does not support P2P from current device (Non-fatal error)\n", dev);
			else
				p2p_supported |= (1 << dev);
		} else {
			/* Always support p2p on the same GPU */
			psm3_my_gpu_device = dev;
			p2p_supported |= (1 << dev);
		}
	}

	return p2p_supported;
}

static int psm3_oneapi_ze_gpudirect_supported(void)
{
	/* Is there any OneAPI Level Zero device property that can indicate this? */
	return 1;
}


static void psm3_oneapi_ze_get_pci_addr(uint32_t *domain_p, uint32_t *bus_p,
							uint32_t *dev_p, uint32_t *func_p)
{
	ze_pci_ext_properties_t PciProperties;

	_HFI_DBG("%d Level Zero GPUs found\n", psm3_num_oneapi_ze_devices);
	if (! psm3_num_oneapi_ze_devices)
		return;

	// caling middleware will have limited GPUs visible to process
	PSM3_ONEAPI_ZE_CALL(zeDevicePciGetPropertiesExt,
						psm3_oneapi_ze_devices[0].dev, &PciProperties);
	*domain_p = PciProperties.address.domain;
	*bus_p = PciProperties.address.bus;
	*dev_p = PciProperties.address.device;
	*func_p = PciProperties.address.function;
}

#ifdef PSM_HAVE_RNDV_MOD
static uint64_t psm3_oneapi_ze_min_bar_size(void)
{
	// implement later
	return 0;
}

static psm2_error_t psm3_oneapi_ze_check_phys_addr(uint64_t phys_addr)
{
	return PSM2_OK;
}

static void psm3_oneapi_ze_roundup_gdrcopy(unsigned long buf, size_t size,
							uintptr_t *pageaddr_p, uint64_t *pagelen_p)
{
	PSM3_ONEAPI_ZE_CALL(zeMemGetAddressRange, psm3_oneapi_ze_context,
							(const void *)buf, (void **)pageaddr_p, pagelen_p);
}

#ifdef PSM_HAVE_REG_MR
static void psm3_oneapi_ze_roundup_rv_reg_mr(struct psm2_ep *ep,
							void **addr_p, uint64_t *length_p, int access)
{
#define MAX_USER_MR_SIZE (32 * 1024)
	void *base;
	size_t len;
	uint64_t page_offset;

	PSM3_ONEAPI_ZE_CALL(zeMemGetAddressRange, psm3_oneapi_ze_context,
			(const void *)*addr_p, &base, &len);
	/*
	 * Need to register MR with base address and total length.
	 * However, for Mellanox cards, the max buffer size for a
	 * user MR registered through the rv module is 32k bytes.
	 * Otherwise, it will fail with IB_WC_MW_BIND_ERR. For fast
	 * registration MR through RV (kernel MR and GPU MR), there
	 * is also a upper limit (max_fast_reg_page_list_len) imposed
	 * by the underlying RDMA device (eg 256MB for mlx5).
	 */
	if (strncasecmp(ep->dev_name, "mlx5_0", 3) == 0 &&
		!(access & IBV_ACCESS_KERNEL)) {
		if (len > MAX_USER_MR_SIZE) {
			/*
			 * Register the first 32k if the buffer stays in the
			 * range. Otherwise, align the buffer to page boundary.
			 */
			if (((char *)*addr_p + *length_p) <=
				((char *)base + MAX_USER_MR_SIZE)) {
				*addr_p = base;
				*length_p = MAX_USER_MR_SIZE;
			} else {
				page_offset = ((uint64_t)*addr_p) &
					      GPU_PAGE_OFFSET_MASK;
				*addr_p = (void *)
					  ROUNDDOWN64P2((uint64_t)*addr_p,
							PSMI_GPU_PAGESIZE);
				*length_p = *length_p + page_offset;
			}
		} else {
			/* Register the entire buffer */
			*addr_p = base;
			*length_p = len;
		}
	} else {
		uint64_t start, end;
		uint64_t mr_len;
		uint64_t offset;
		uint64_t limit = ep->verbs_ep.max_fmr_size;

		/* Buffer end + 1 */
		end = (uint64_t)base + len;
		/* Offset of the requested buffer chunk */
		offset = (uint64_t)*addr_p - (uint64_t)base;
                /*
		 * Start address of next MR.
		 * The idea is to avoid fragment the entire buffer as few times
		 * as possible to avoid overlapped MRs and increae cache hit
		 * rate. Therefore, we can't just start from page boundary of
		 * the requested buffer address:
		 * start = ROUNDDOWN64P2((uint64_t)*addr_p, PSMI_GPU_PAGESIZE);
		 */
		start = (uint64_t)base + (offset / limit) * limit;
		mr_len = end - start;
		if (mr_len > limit)
			mr_len = limit;
		/*
		 * If the chunk does not cross the (start + mr_len) boundary,
		 * register the max chunk size or the remainder of the entire
		 * buffer. Otherwise, align the buffer to page size and just
		 * register the requested chunk size plus the offset.
		 */
		if (((uint64_t)*addr_p + *length_p) <= (start + mr_len)) {
			*addr_p = (void *)start;
			*length_p = mr_len;
		} else {
			page_offset = ((uint64_t)*addr_p) &
				      GPU_PAGE_OFFSET_MASK;
			*addr_p = (void *)ROUNDDOWN64P2((uint64_t)*addr_p,
							PSMI_GPU_PAGESIZE);
			*length_p = *length_p + page_offset;
		}
	}
}

// add OneAPI Level Zero specific information to the mparams in prep for the
// RV_IOCTL_REG_MEM ioctl to rv
// for reg_mr the gpu_specific->ze_alloc_id is obtained in caller and
// retained in the psm2_verbs_mr_t for future cache hit checks
static int psm3_oneapi_ze_init_rv_reg_mr_params(
				void *addr, uint64_t length, int access,
				struct rv_mem_params *mparams,
				union psm3_verbs_mr_gpu_specific *gpu_specific,
				union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad)
{
	// need to save off ipc_handle here for cleanup below
	psm3_oneapi_ze_get_ipc_handle(addr, &scratchpad->ze_ipc_handle,
							&scratchpad->ze_handle_fd);
	mparams->in.ipc_handle = (uint32_t)scratchpad->ze_handle_fd;
	if (!mparams->in.ipc_handle) {
		_HFI_ERROR("zeMemGetIpcHandle for %p returned empty handle: 0x%02x%02x%02x%02x %02x%02x%02x%02x\n",
				addr, scratchpad->ze_ipc_handle.data[0],
				scratchpad->ze_ipc_handle.data[1],
				scratchpad->ze_ipc_handle.data[2],
				scratchpad->ze_ipc_handle.data[3],
				scratchpad->ze_ipc_handle.data[4],
				scratchpad->ze_ipc_handle.data[5],
				scratchpad->ze_ipc_handle.data[6],
				scratchpad->ze_ipc_handle.data[7]);
		// tends to mean out of fd's
		return ENOSPC;
	}
	mparams->in.alloc_id = psm3_oneapi_ze_ignore_alloc_id?
			(psm3_oneapi_ze_ignore_alloc_id==1?
				0:psm3_oneapi_ze_fake_alloc_id++)
			:gpu_specific->ze_alloc_id;
	mparams->in.base_addr = gpu_specific->ze_base_addr;
	return 0;
}
#endif /* PSM_HAVE_REG_MR */

// add OneAPI Level Zero specific information to the params in prep for the
// RV_IOCTL_PIN_MMAP ioctl to rv
// for pin_mmap the alloc_id is obtained here and there is no caching in the
// caller.
static int psm3_oneapi_ze_init_rv_pin_mmap_params(
				void *addr, uint64_t length, int access,
				struct rv_gpu_mem_params *params,
				union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad)
{
	uint64_t alloc_id;

	psm3_oneapi_ze_get_ipc_handle(addr, &scratchpad->ze_ipc_handle, &scratchpad->ze_handle_fd);
	params->in.ipc_handle = (uint32_t)scratchpad->ze_handle_fd;
	if (!params->in.ipc_handle) {
		_HFI_ERROR("No ipc_handle: 0x%02x%02x%02x%02x %02x%02x%02x%02x\n",
				scratchpad->ze_ipc_handle.data[0],
				scratchpad->ze_ipc_handle.data[1],
				scratchpad->ze_ipc_handle.data[2],
				scratchpad->ze_ipc_handle.data[3],
				scratchpad->ze_ipc_handle.data[4],
				scratchpad->ze_ipc_handle.data[5],
				scratchpad->ze_ipc_handle.data[6],
				scratchpad->ze_ipc_handle.data[7]);
		return EFAULT;
	}
	alloc_id = psm3_oneapi_ze_get_alloc_id(addr, NULL);
	// id is unique across all allocs on all devices in a process
	params->in.alloc_id = psm3_oneapi_ze_ignore_alloc_id?
				(psm3_oneapi_ze_ignore_alloc_id==1?
					0:psm3_oneapi_ze_fake_alloc_id++)
				:alloc_id;
	_HFI_VDBG("addr 0x%"PRIx64" length %"PRIu64" id %"PRIu64" access 0x%x\n",
				(uint64_t)addr, length, alloc_id, access);
	return 0;
}

// cleanup OneAPI Level Zero specific scratchpad from
// psm3_oneapi_ze_init_rv_reg_mr_params or
// psm3_oneapi_ze_init_rv_pin_mmap_params
// called on success or error path, makes sure not to polute errno
// as it can reflect the earlier error for the error path in caller.
static void psm3_oneapi_ze_rv_reg_mmap_cleanup(
				void *addr, uint64_t length, int access,
				union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad)
{
	if (scratchpad->ze_handle_fd) {
		int save_errno = errno;
		psm3_oneapi_ze_put_ipc_handle((const void *)addr, scratchpad->ze_ipc_handle);
		// no need to clear scratchpad
		errno = save_errno;
	}
}
#endif /* PSM_HAVE_RNDV_MOD */

#ifdef PSM_HAVE_REG_MR
// compare GPU specific fields in verbs MR cache entry
static int psm3_oneapi_ze_cmp_mr(const union psm3_verbs_mr_gpu_specific *a,
		const union psm3_verbs_mr_gpu_specific *b)
{
	if (a->ze_alloc_id < b->ze_alloc_id)
		return -1;
	else if (a->ze_alloc_id > b->ze_alloc_id)
		return 1;
	else
		return 0;
}

// initialize GPU specific fields in verbs MR cache entry
static void psm3_oneapi_ze_init_mr(void *addr, uint64_t length, int access,
				union psm3_verbs_mr_gpu_specific *gpu_specific)
{
	void *base = NULL;
	size_t len;

	if (access & IBV_ACCESS_IS_GPU_ADDR)
		PSM3_ONEAPI_ZE_CALL(zeMemGetAddressRange,
				    psm3_oneapi_ze_context, (const void *)addr,
				    &base, &len);
	gpu_specific->ze_base_addr = (uint64_t)base;
	gpu_specific->ze_alloc_id = (access & IBV_ACCESS_IS_GPU_ADDR)?
				    psm3_oneapi_ze_get_alloc_id(addr, NULL) : 0;
}
#endif /* PSM_HAVE_REG_MR */

static void psm3_oneapi_ze_fetch_ctxt(void)
{
	// nothing to do
}

// ensure psm3_cu_ctxt reflects our most recent psm3_cu_ctxt
static void psm3_oneapi_ze_refresh_ctxt(void)
{
	// nothing to do
}

static void psm3_oneapi_ze_register_hostmem(void *buf, uint32_t size)
{
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSM3_ONEAPI_ZE_CALL(zexDriverImportExternalPointer, psm3_oneapi_ze_driver,
				buf, size);
#endif
}

static void psm3_oneapi_ze_unregister_hostmem(void *buf)
{
#ifndef PSM3_NO_ONEAPI_IMPORT
	ze_result_t result;
	//PSM3_ONEAPI_ZE_CALL(zexDriverReleaseImportedPointer, psm3_oneapi_ze_driver,
	//		buf);
	psm3_oneapi_ze_count_zexDriverReleaseImportedPointer++;
	result = psm3_oneapi_ze_zexDriverReleaseImportedPointer(psm3_oneapi_ze_driver,
				buf);
	if (result != ZE_RESULT_SUCCESS) {
		_HFI_DBG("OneAPI Level Zero failure: zexDriverReleaseImportedPointer returned %d: %s\n", result, psm3_oneapi_ze_result_to_string(result));
	}
#endif
}

/*
 * Two usages:
 *   (1) ctxt == NULL: check if the buffer is allocated from Level-zero.
 *       In this case, change psm3_oneapi_ze_cur_dev if device has changed.
 *   (2) ctxt != NULL: try to get the device context.
 *       In this case, don't change psm3_oneapi_ze_cur_dev.
 */
PSMI_ALWAYS_INLINE(
int
psm3_is_oneapi_ze_mem(const void *ptr, struct psm3_oneapi_ze_dev_ctxt **ctxt))
{
	ze_memory_allocation_properties_t mem_props = {
		ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES
	};
	ze_device_handle_t dev;
	ze_result_t result;
	int ret = 0;

	psm3_oneapi_ze_count_zeMemGetAllocProperties++;
	result = psm3_oneapi_ze_zeMemGetAllocProperties(psm3_oneapi_ze_context, ptr, &mem_props,
						&dev);
	if (result == ZE_RESULT_SUCCESS &&
		(mem_props.type != ZE_MEMORY_TYPE_UNKNOWN)) {
		ret = 1;
		_HFI_VDBG("ptr %p type %d dev %p oneapi_ze_cur_dev %p\n",
				ptr, mem_props.type, dev, psm3_oneapi_ze_cur_dev->dev);
		/*
		 * Check if the gpu device has changed.
		 * If we are trying to get the device context (!ctxt),
		 * don't change psm3_oneapi_ze_cur_dev.
		 * If the buffer is allocated through zeMemAllocHost,
		 * there will be no device associated with it (dev == NULL).
		 * In this case, use the current device context.
		 */
		if (!dev) {
			if (ctxt)
				*ctxt = psm3_oneapi_ze_cur_dev;
			return ret;
		}
		if (ctxt || (!ctxt && dev != psm3_oneapi_ze_cur_dev->dev)) {
			int i;

			for (i = 0; i < psm3_num_oneapi_ze_devices; i++) {
				if (psm3_oneapi_ze_devices[i].dev == dev) {
					if (ctxt)
						*ctxt = &psm3_oneapi_ze_devices[i];
					else
						psm3_oneapi_ze_cur_dev = &psm3_oneapi_ze_devices[i];
					break;
				}
			}
			_HFI_VDBG("check ze_device[%d-%d] for dev %p: match %s\n",
				  0, psm3_num_oneapi_ze_devices - 1, dev,
				  i == psm3_num_oneapi_ze_devices ? "not found" :
					"found");
		}
	}

	return ret;
}

/*
 * Determine if the memory is on the GPU device. If so, get the ctxt.
 * Don't change psm3_oneapi_ze_cur_dev.
 */
static int
psm3_is_oneapi_ze_dev_mem(const void *ptr,
			  struct psm3_oneapi_ze_dev_ctxt **ctxt)
{
	ze_memory_allocation_properties_t mem_props = {
		ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES
	};
	ze_device_handle_t dev;
	ze_result_t result;
	int ret = 0;

	psm3_oneapi_ze_count_zeMemGetAllocProperties++;
	result = psm3_oneapi_ze_zeMemGetAllocProperties(psm3_oneapi_ze_context,
							ptr, &mem_props, &dev);
	if (result == ZE_RESULT_SUCCESS &&
	    (mem_props.type == ZE_MEMORY_TYPE_DEVICE)) {
		ret = 1;
		_HFI_VDBG("ptr %p type %d dev %p\n", ptr, mem_props.type, dev);
		if (ctxt) {
			int i;

			*ctxt = NULL;
			for (i = 0; i < psm3_num_oneapi_ze_devices; i++) {
				if (psm3_oneapi_ze_devices[i].dev == dev) {
					*ctxt = &psm3_oneapi_ze_devices[i];
					break;
				}
			}
			_HFI_VDBG("check ze_device[%d-%d] for dev %p: match %s\n",
				  0, psm3_num_oneapi_ze_devices - 1, dev,
				  i == psm3_num_oneapi_ze_devices ?
					"not found" : "found");
		}
	}

	return ret;
}

PSMI_ALWAYS_INLINE(
struct psm3_oneapi_ze_dev_ctxt *
psm3_oneapi_ze_dev_ctxt_get(const void *ptr))
{
	struct psm3_oneapi_ze_dev_ctxt *ctxt = NULL;

	psm3_is_oneapi_ze_mem(ptr, &ctxt);

	return ctxt;
}

static int psm3_oneapi_ze_is_gpu_mem(const void *ptr)
{
	return psm3_is_oneapi_ze_mem(ptr, NULL);
}

static void psm3_oneapi_ze_prepare_HtoD_memcpys(struct ips_protoexp *protoexp)
{
	int i;

	for (i = 0; i < MAX_ZE_DEVICES; i++)
		protoexp->gpu_specific.ze_cq_recvs[i] = NULL;
}

static void psm3_oneapi_ze_prepare_DtoH_memcpys(struct ips_proto *proto)
{
	int i;

	for (i = 0; i < MAX_ZE_DEVICES; i++)
		proto->gpu_specific.ze_cq_sends[i] = NULL;
}

static void psm3_oneapi_ze_shutdown_HtoD_memcpys(struct ips_protoexp *protoexp)
{
	int i;

	for (i = 0; i < MAX_ZE_DEVICES; i++) {
		if (protoexp->gpu_specific.ze_cq_recvs[i]) {
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueDestroy, protoexp->gpu_specific.ze_cq_recvs[i]);
			protoexp->gpu_specific.ze_cq_recvs[i] = NULL;
		}                  
	}
}

static void psm3_oneapi_ze_shutdown_DtoH_memcpys(struct ips_proto *proto)
{
	int i;

	for (i = 0; i < MAX_ZE_DEVICES; i++) {
		if (proto->gpu_specific.ze_cq_sends[i]) {
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueDestroy, proto->gpu_specific.ze_cq_sends[i]);
			proto->gpu_specific.ze_cq_sends[i] = NULL;
		}
	}           
}

static void psm3_oneapi_ze_memcpy_HtoD_start(struct ips_protoexp *protoexp,
									struct ips_gpu_hostbuf *ghb, uint32_t len)
{
	ze_event_pool_desc_t pool_desc = {
		.stype = ZE_STRUCTURE_TYPE_EVENT_POOL_DESC,
		.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE,
		.count = 1
	};
	ze_event_desc_t event_desc = {
		.stype = ZE_STRUCTURE_TYPE_EVENT_DESC,
		.signal = ZE_EVENT_SCOPE_FLAG_HOST,
		.wait = ZE_EVENT_SCOPE_FLAG_HOST,
		.index = 0
	};
	struct psm3_oneapi_ze_dev_ctxt *ctxt;
	int inx;
		
	ctxt = psm3_oneapi_ze_dev_ctxt_get(ghb->gpu_buf);
	if (!ctxt) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
						"%s HTOD: unknown GPU device for addr %p\n",
						__FUNCTION__, ghb->gpu_buf);
		return; /* NOT REACHED */
	}
	if (ghb->gpu_specific.ze_event_pool == NULL) {
		PSM3_ONEAPI_ZE_CALL(zeEventPoolCreate,
				psm3_oneapi_ze_context, &pool_desc, 0, NULL, &ghb->gpu_specific.ze_event_pool);
	}
	if (ghb->gpu_specific.ze_copy_status == NULL) {
		PSM3_ONEAPI_ZE_CALL(zeEventCreate,
			ghb->gpu_specific.ze_event_pool, &event_desc, &ghb->gpu_specific.ze_copy_status);
	}
	inx = ctxt->dev_index;
	if (! ghb->gpu_specific.ze_command_lists[inx]) {
		psm3_oneapi_ze_async_cmd_create(ctxt,
				&protoexp->gpu_specific.ze_cq_recvs[inx], &ghb->gpu_specific.ze_command_lists[inx]);
	}
	ghb->gpu_specific.ze_cur_dev_inx = inx;
	PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ghb->gpu_specific.ze_command_lists[inx],
		ghb->gpu_buf, ghb->host_buf, len, ghb->gpu_specific.ze_copy_status, 0, NULL);
	if (! psm3_oneapi_ze_immed_async_copy) {
		PSM3_ONEAPI_ZE_CALL(zeCommandListClose, ghb->gpu_specific.ze_command_lists[inx]);
		PSM3_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists,
			protoexp->gpu_specific.ze_cq_recvs[inx], 1, &ghb->gpu_specific.ze_command_lists[inx], NULL);
	}
}

static void psm3_oneapi_ze_memcpy_DtoH_start(struct ips_proto *proto,
									struct ips_gpu_hostbuf *ghb, uint32_t len)
{
	ze_event_pool_desc_t pool_desc = {
		.stype = ZE_STRUCTURE_TYPE_EVENT_POOL_DESC,
		.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE,
		.count = 1
	};
	ze_event_desc_t event_desc = {
		.stype = ZE_STRUCTURE_TYPE_EVENT_DESC,
		.signal = ZE_EVENT_SCOPE_FLAG_HOST,
		.wait = ZE_EVENT_SCOPE_FLAG_HOST,
		.index = 0
	};
	struct psm3_oneapi_ze_dev_ctxt *ctxt;
	int inx;
	
	ctxt = psm3_oneapi_ze_dev_ctxt_get(ghb->gpu_buf);
	if (!ctxt) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"%s DTOH: unknown GPU device for addr %p\n",
					__FUNCTION__, ghb->gpu_buf);
		return; /* NOT REACHED */
	}
	if (ghb->gpu_specific.ze_event_pool == NULL) {
		PSM3_ONEAPI_ZE_CALL(zeEventPoolCreate,
			psm3_oneapi_ze_context, &pool_desc, 0, NULL, &ghb->gpu_specific.ze_event_pool);
	}
	if (ghb->gpu_specific.ze_copy_status == NULL) {
		PSM3_ONEAPI_ZE_CALL(zeEventCreate, ghb->gpu_specific.ze_event_pool, &event_desc,
			&ghb->gpu_specific.ze_copy_status);
	}
	inx = ctxt->dev_index;
	if (! ghb->gpu_specific.ze_command_lists[inx]) {
		psm3_oneapi_ze_async_cmd_create(ctxt, &proto->gpu_specific.ze_cq_sends[inx],
				&ghb->gpu_specific.ze_command_lists[inx]);
	}
	ghb->gpu_specific.ze_cur_dev_inx = inx;
	PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ghb->gpu_specific.ze_command_lists[inx],
		ghb->host_buf, ghb->gpu_buf, len, ghb->gpu_specific.ze_copy_status, 0, NULL);
	if (! psm3_oneapi_ze_immed_async_copy) {
		PSM3_ONEAPI_ZE_CALL(zeCommandListClose, ghb->gpu_specific.ze_command_lists[inx]);
		PSM3_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists,
			proto->gpu_specific.ze_cq_sends[inx], 1, &ghb->gpu_specific.ze_command_lists[inx], NULL);
	}
}

static int psm3_oneapi_ze_memcpy_done(struct ips_gpu_hostbuf *ghb)
{
	ze_result_t result;
	psm3_oneapi_ze_count_zeEventQueryStatus++;

	result = psm3_oneapi_ze_zeEventQueryStatus(ghb->gpu_specific.ze_copy_status);
	if (result == ZE_RESULT_SUCCESS) {
		return 1;
	} else if (result == ZE_RESULT_NOT_READY) {
		return 0;
	} else {
		_HFI_ERROR("OneAPI Level Zero failure: %s() (at %s:%d) returned 0x%x: %s\n",
			"zeEventQueryStatus",  __FILE__, __LINE__, result,
			psm3_oneapi_ze_result_to_string(result));
		psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Error returned from OneAPI Level Zero function %s.\n",
			"zeEventQueryStatus");
	}
	return 0;
}

// when allocating bounce buffers either malloc w/Import or
// zeMemAllocHost can be used.  zeMemAllocHost tends to perform
// better in the subsequent GPU copy's AppendMemoryCopy.  However
// zeMemAllocHost results in a GPU-like address which requires dmabuf
// so we can't use zeMemAllocHost for DMA to/from the bounce buffer
// unless rv is available to handle GPU addresses (eg. PSM3_GPUDIRECT=1)

static void *psm3_oneapi_ze_host_alloc_malloc(unsigned size)
{
	void *ret_ptr = psmi_malloc(PSMI_EP_NONE, UNDEFINED, size);
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSM3_ONEAPI_ZE_CALL(zexDriverImportExternalPointer, psm3_oneapi_ze_driver, ret_ptr, size);
#endif
	return ret_ptr;
}

static void psm3_oneapi_ze_host_free_malloc(void *ptr)
{
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSM3_ONEAPI_ZE_CALL(zexDriverReleaseImportedPointer, psm3_oneapi_ze_driver, ptr);
#endif
	psmi_free(ptr);
}

#ifndef PSM3_USE_ONEAPI_MALLOC
static void *psm3_oneapi_ze_host_alloc_zemem(unsigned size)
{
	void *ret_ptr;
	ze_host_mem_alloc_desc_t host_desc = {
		.stype = ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC,
		.flags = ZE_MEMORY_ACCESS_CAP_FLAG_RW
	};
	PSM3_ONEAPI_ZE_CALL(zeMemAllocHost, psm3_oneapi_ze_context,
						&host_desc, size, 8, &ret_ptr);
	return ret_ptr;
}

static void psm3_oneapi_ze_host_free_zemem(void *ptr)
{
	PSM3_ONEAPI_ZE_CALL(zeMemFree, psm3_oneapi_ze_context, ptr);
}

static void *(*psm3_oneapi_ze_host_alloc_ptr)(unsigned size) = psm3_oneapi_ze_host_alloc_malloc;
static void (*psm3_oneapi_ze_host_free_ptr)(void *ptr) = psm3_oneapi_ze_host_free_malloc;
static int psm3_oneapi_ze_using_zemem_alloc = 0;
#endif /* PSM3_USE_ONEAPI_MALLOC */

// this is only called if GPU Direct is enabled in rv such that
// GDR Copy and/or RDMA MRs can provide GPU-like addresses to rv
static void psm3_oneapi_ze_using_rv_for_mrs(void)
{
#ifndef PSM3_USE_ONEAPI_MALLOC
	psm3_oneapi_ze_host_alloc_ptr = psm3_oneapi_ze_host_alloc_zemem;
	psm3_oneapi_ze_host_free_ptr = psm3_oneapi_ze_host_free_zemem;
	psm3_oneapi_ze_using_zemem_alloc = 1;
#endif
}

static void psm3_oneapi_ze_host_alloc(void **ret_ptr, uint32_t size)
{
#ifdef PSM3_USE_ONEAPI_MALLOC
	*ret_ptr = psm3_oneapi_ze_host_alloc_malloc(size);
#else
	*ret_ptr = (*psm3_oneapi_ze_host_alloc_ptr)(size);
#endif
}

static void psm3_oneapi_ze_host_free(void *ptr)
{
#ifdef PSM3_USE_ONEAPI_MALLOC
	psm3_oneapi_ze_host_free_malloc(ptr);
#else
	(*psm3_oneapi_ze_host_free_ptr)(ptr);
#endif
}

static void psm3_oneapi_ze_hostbuf_lazy_init(struct ips_gpu_hostbuf *ghb)
{
	int i;

	ghb->gpu_specific.ze_event_pool = NULL;
	ghb->gpu_specific.ze_copy_status = NULL;
	for (i = 0; i < MAX_ZE_DEVICES; i++)
		ghb->gpu_specific.ze_command_lists[i] = NULL;
}

static void psm3_oneapi_ze_hostbuf_reset(struct ips_gpu_hostbuf *ghb)
{
	if (! psm3_oneapi_ze_immed_async_copy) {
		PSM3_ONEAPI_ZE_CALL(zeCommandListReset,
				ghb->gpu_specific.ze_command_lists[ghb->gpu_specific.ze_cur_dev_inx]);
	}
	PSM3_ONEAPI_ZE_CALL(zeEventHostReset, ghb->gpu_specific.ze_copy_status);
}

static void psm3_oneapi_ze_hostbuf_destroy(struct ips_gpu_hostbuf *ghb)
{
	int i;

	if (ghb->gpu_specific.ze_copy_status != NULL) {
		PSM3_ONEAPI_ZE_CALL(zeEventDestroy, ghb->gpu_specific.ze_copy_status);
	}
	if (ghb->host_buf != NULL) {
		psm3_oneapi_ze_host_free(ghb->host_buf);
	}
	if (ghb->gpu_specific.ze_event_pool != NULL) {
		PSM3_ONEAPI_ZE_CALL(zeEventPoolDestroy, ghb->gpu_specific.ze_event_pool);
	}
	for (i = 0; i < MAX_ZE_DEVICES; i++) {
		if (ghb->gpu_specific.ze_command_lists[i]) {
			PSM3_ONEAPI_ZE_CALL( zeCommandListDestroy, ghb->gpu_specific.ze_command_lists[i]);
			ghb->gpu_specific.ze_command_lists[i] = NULL;
		}
	}
}

// synchronous GPU memcpy
static void psm3_oneapi_ze_memcpy_internal(void *dstptr, const void *srcptr, size_t size)
{
	struct psm3_oneapi_ze_dev_ctxt *ctxt, *dst_ctxt = NULL,
				       *src_ctxt = NULL;

	psmi_assert(size > 0);
	if (!psm3_is_oneapi_ze_dev_mem(dstptr, &dst_ctxt) &&
	    !psm3_is_oneapi_ze_dev_mem(srcptr, &src_ctxt)) {
		/*
		 * If both buffers are in host memory, don't use Level-zero
		 * library call as memcpy may be faster and Level-zero copy
		 * may leak resources at this point.
		 */
		memcpy(dstptr, srcptr, size);
		return;
	}
	ctxt = dst_ctxt ? dst_ctxt : src_ctxt;
	if (!ctxt) {
		_HFI_ERROR("dst %p src %p: no ctxt for copying\n",
			   dstptr, srcptr);
		return;
	}
	if (psm3_oneapi_ze_immed_sync_copy) {
		PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
	} else {
		PSM3_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->cl);
		PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
		PSM3_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->cl);
		PSM3_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->cq,
					1, &ctxt->cl, NULL);
		PSM3_ONEAPI_ZE_CALL(zeCommandQueueSynchronize, ctxt->cq, UINT32_MAX);
	}
}

// synchronous GPU memcpy DTOD (xeLink)
static void psm3_oneapi_ze_memcpy_DTOD(void *dstptr, const void *srcptr, size_t size)
{
	struct psm3_oneapi_ze_dev_ctxt *ctxt;

	psmi_assert(size > 0);
	ctxt = psm3_oneapi_ze_dev_ctxt_get(dstptr);
	if (!ctxt) {
		_HFI_ERROR("dst %p src %p not GPU buf for copying\n",
				dstptr, srcptr);
		return;
	}
	if (size <= psm3_oneapi_parallel_dtod_copy_thresh) {
		if (psm3_oneapi_ze_immed_sync_copy) {
			PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
		} else {
			PSM3_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->cl);
			PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
			PSM3_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->cl);
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->cq,
					1, &ctxt->cl, NULL);
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueSynchronize, ctxt->cq, UINT32_MAX);
		}
	} else {
		// for large DTOD copies, start 2 parallel commands
		// then wait for both
		size_t size0 = ROUNDUP64P2(size/2, 64*1024);
		size_t size1 = size - size0;

		if (psm3_oneapi_ze_immed_sync_copy) {
			PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl0,
					dstptr, srcptr, size0, ctxt->copy_status0, 0, NULL);

			PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl1,
					(void*)((uintptr_t)dstptr+size0),
					(void*)((uintptr_t)srcptr+size0), size1, ctxt->copy_status1,
					0, NULL);
		} else {
			PSM3_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->async_cl0);
			PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl0,
					dstptr, srcptr, size0, ctxt->copy_status0, 0, NULL);
			PSM3_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->async_cl0);
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->async_cq0,
					1, &ctxt->async_cl0, NULL);

			PSM3_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->async_cl1);
			PSM3_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl1,
					(void*)((uintptr_t)dstptr+size0),
					(void*)((uintptr_t)srcptr+size0), size1, ctxt->copy_status1,
					0, NULL);
			PSM3_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->async_cl1);
			PSM3_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->async_cq1,
					1, &ctxt->async_cl1, NULL);
		}
		// 2nd copy may be slightly smaller so waity for it first so
		// can potentially hide its Reset latency while 1st copy completes
		PSM3_ONEAPI_ZE_CALL(zeEventHostSynchronize, ctxt->copy_status1, UINT32_MAX);
		PSM3_ONEAPI_ZE_CALL(zeEventHostReset, ctxt->copy_status1);

		PSM3_ONEAPI_ZE_CALL(zeEventHostSynchronize, ctxt->copy_status0, UINT32_MAX);
		PSM3_ONEAPI_ZE_CALL(zeEventHostReset, ctxt->copy_status0);
	}
}

static void psm3_oneapi_ze_memcpy_DtoD(void *dstptr, const void *srcptr, uint32_t len)
{
	psm3_oneapi_ze_memcpy_DTOD(dstptr, srcptr, len);
}

static void psm3_oneapi_ze_memcpy_HtoD(void *dstptr, const void *srcptr, uint32_t len)
{
	psm3_oneapi_ze_memcpy_internal(dstptr, srcptr, len);
}

static void psm3_oneapi_ze_memcpy_DtoH(void *dstptr, const void *srcptr, uint32_t len)
{
	psm3_oneapi_ze_memcpy_internal(dstptr, srcptr, len);
}

static void psm3_oneapi_ze_memcpy(void *dstptr, const void *srcptr, uint32_t len)
{
	psm3_oneapi_ze_memcpy_internal(dstptr, srcptr, len);
}

static void psm3_oneapi_ze_synchronize_memcpy(void)
{
	/* Not needed for OneAPI Level Zero */
}

static void psm3_oneapi_ze_mark_buf_synchronous(const void *buf)
{
	/* not needed for OneAPI ZE */
}

static int psm3_oneapi_ze_gpu_addr_send_mr(struct psm2_mq_req *mqreq)
{
#ifdef PSM3_USE_ONEAPI_MALLOC
	// HOST_ALLOC memory treated as CPU memory for Verbs MRs
	return (mqreq->is_buf_gpu_mem && ! mqreq->gpu_hostbuf_used);
#else
	// HOST_ALLOC memory treated as GPU memory for Verbs MRs
	/// Note: gpu_hostbuf_used" only set if is_buf_gpu_mem
	return mqreq->is_buf_gpu_mem &&
			(! mqreq->gpu_hostbuf_used || psm3_oneapi_ze_using_zemem_alloc );
#endif
}

static int psm3_oneapi_ze_gpu_addr_recv_mr(struct ips_tid_recv_desc *tidrecvc,
							int gpu_hostbuf_used)
{
#ifdef PSM3_USE_ONEAPI_MALLOC
	// HOST_ALLOC memory treated as CPU memory for Verbs MRs
	return tidrecvc->is_ptr_gpu_backed;
#else
	// HOST_ALLOC memory treated as GPU memory for Verbs MRs
	/// Note: gpu_hostbuf_used" only set if is_buf_gpu_mem
	return tidrecvc->is_ptr_gpu_backed
			|| (gpu_hostbuf_used && psm3_oneapi_ze_using_zemem_alloc);
#endif
}

//***************************************************************************
//OneAPI Level Zero support for PSM3_DEVICES "shm", via an IPC handle cache and
//OneAPI Level Zero IPC
//In platforms with xeLink between GPUs, OneAPI Level Zero IPC will use xeLink.

#define ONEAPI_MEMHANDLE_CACHE_SIZE 64

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
/*
 * rbtree cruft
 */
struct _cl_map_item;

struct psm3_oneapi_ze_memhandle_cache;
typedef struct psm3_oneapi_ze_memhandle_cache *psm3_oneapi_ze_memhandle_cache_t;

typedef struct
{
	unsigned long           start;           /* start(base) virtual address
												in peer process */
	uint32_t                ze_handle;       /* Sender's GEM handle or fd */
	uint64_t                alloc_id;        /* ze alloc_id */
	void                    *buf_ptr;        /* buffer pointer in this
												process */
	psm2_epid_t             epid;
	struct _cl_map_item*    i_prev;          /* idle queue previous */
	struct _cl_map_item*    i_next;          /* idle queue next */
	psm3_oneapi_ze_memhandle_cache_t cache;           /* only for gem_handle close */
}__attribute__ ((aligned (128))) psm3_rbtree_oneapi_ze_memhandle_cache_mapitem_pl_t;

typedef struct {
	uint32_t                nelems;          /* number of elements in the cache */
} psm3_rbtree_oneapi_ze_memhandle_cache_map_pl_t;

static psm2_error_t psm3_oneapi_ze_memhandle_mpool_alloc(
					psm3_oneapi_ze_memhandle_cache_t cache, uint32_t memcache_size);
static void psm3_oneapi_ze_memhandle_delete(void *buf_ptr);

/*
 * Custom comparator
 */
typedef psm3_rbtree_oneapi_ze_memhandle_cache_mapitem_pl_t psm3_oneapi_ze_cache_item;

static int psm3_oneapi_ze_cache_key_cmp(const psm3_oneapi_ze_cache_item *a,
						const psm3_oneapi_ze_cache_item *b)
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

	// The sender has used zeMemGetAddressRange to normalize the address
	// so we can simply compare the start address of the allocation.
	// Note zeMemOpenIpcHandle only needs the start address as well, so we
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
#define RBTREE_MI_PL    psm3_rbtree_oneapi_ze_memhandle_cache_mapitem_pl_t
#define RBTREE_MAP_PL   psm3_rbtree_oneapi_ze_memhandle_cache_map_pl_t
#define RBTREE_CMP(a, b, c) psm3_oneapi_ze_cache_key_cmp((a), (b))
#define RBTREE_ASSERT   psmi_assert
#define RBTREE_MAP_COUNT(PAYLOAD_PTR)     ((PAYLOAD_PTR)->nelems)
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
struct psm3_oneapi_ze_memhandle_cache {
	cl_qmap_t map;
	mpool_t mpool;
	uint32_t size;
	psm2_mq_stats_t *stats;
};

static void psm3_print_oneapi_ze_memhandle_cache_stats(psm2_mq_stats_t *stats)
{
	_HFI_DBG("limit=%lu,maxelems=%lu,hit=%lu,miss=%lu,evict=%lu,remove=%lu,clear=%lu\n",
		stats->gpu_ipc_cache_limit, stats->gpu_ipc_cache_max_nelems,
		stats->gpu_ipc_cache_hit, stats->gpu_ipc_cache_miss,
		stats->gpu_ipc_cache_evict, stats->gpu_ipc_cache_remove,
		stats->gpu_ipc_cache_clear);
}

/*
 * This is the callback function when mempool are resized or destroyed.
 * Upon calling cache free mpool is destroyed which in turn calls this callback
 * which helps in closing all memhandles.
 * TBD - only called for !is_alloc when destroying so could avoid keeping
 * cache pointer in memcache_item.  But when GEM_CLOSE is not needed
 * memhandle_delete won't need destroyng flag and can remove cache pointer then
 */
static void
psm3_oneapi_ze_memhandle_cache_alloc_func(int is_alloc, void* context, void* obj)
{
	cl_map_item_t* memcache_item = (cl_map_item_t*)obj;
	if (!is_alloc) {
		if(memcache_item->payload.start)
			psm3_oneapi_ze_memhandle_delete(memcache_item->payload.buf_ptr);
	}
}

/*
 * Creating mempool for ze memhandle cache nodes.
 */
static psm2_error_t
psm3_oneapi_ze_memhandle_mpool_alloc(psm3_oneapi_ze_memhandle_cache_t cache,
							uint32_t memcache_size)
{
	psm2_error_t err;
	if (memcache_size < 1)
		return PSM2_PARAM_ERR;

	cache->size = memcache_size;
	/* Creating a memory pool of size PSM3_ONEAPI_MEMCACHE_SIZE
	 * which includes the Root and NIL items
	 */
	cache->mpool = psm3_mpool_create_for_gpu(sizeof(cl_map_item_t),
					cache->size,
					cache->size, 0,
					UNDEFINED, NULL, NULL,
					psm3_oneapi_ze_memhandle_cache_alloc_func,
					NULL);
	if (cache->mpool == NULL) {
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				"Couldn't allocate ONEAPI host receive buffer pool");
		return err;
	}
	return PSM2_OK;
}

/*
 * allocate and initialize memhandle cache
 * including rbtree.
 */
static psm2_error_t psm3_oneapi_ze_memhandle_cache_alloc(psm3_oneapi_ze_memhandle_cache_t *cachep,
									uint32_t memcache_size,
									psm2_mq_stats_t *stats)
{
	cl_map_item_t *root = NULL, *nil_item = NULL;

	*cachep = (psm3_oneapi_ze_memhandle_cache_t)psmi_calloc(
						NULL, UNDEFINED, 1, sizeof(**cachep));
	if (! *cachep)
		return PSM2_NO_MEMORY;

	psm2_error_t err = psm3_oneapi_ze_memhandle_mpool_alloc(
					*cachep, memcache_size);
	if (err != PSM2_OK)
		return err;

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

static void psm3_oneapi_ze_memhandle_cache_free(psm3_oneapi_ze_memhandle_cache_t cache)
{
	psm3_print_oneapi_ze_memhandle_cache_stats(cache->stats);

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
psm3_oneapi_ze_idleq_insert(psm3_oneapi_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
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
psm3_oneapi_ze_idleq_remove_last(psm3_oneapi_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
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
psm3_oneapi_ze_idleq_remove(psm3_oneapi_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (LAST(cache) == memcache_item) {
		psm3_oneapi_ze_idleq_remove_last(cache, memcache_item);
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
psm3_oneapi_ze_idleq_reorder(psm3_oneapi_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (FIRST(cache) == memcache_item && LAST(cache) == memcache_item ) {
		return;
	}
	psm3_oneapi_ze_idleq_remove(cache, memcache_item);
	psm3_oneapi_ze_idleq_insert(cache, memcache_item);
	return;
}

/*
 * After a successful cache hit, item is validated by doing a
 * memcmp on the handle stored and the handle we receive from the
 * sender. If the validation fails the item is removed from the idleq,
 * the rbtree, is put back into the mpool and ZeMemCloseIpcHandle function
 * is called.
 * Level Zero's alloc_id will be unique per allocation, even if the allocation
 * was at the same address.  In some cases, but not always, the ipc_handle
 * will also be different.  So we validate both, although just checking alloc_id
 * would be sufficient.
 */

static psm2_error_t
psm3_oneapi_ze_memhandle_cache_validate(psm3_oneapi_ze_memhandle_cache_t cache,
				cl_map_item_t* memcache_item,
				uintptr_t sbuf, uint32_t handle,
				psm2_epid_t epid, uint64_t alloc_id)
{
	psmi_assert(!psm3_epid_cmp_internal(epid, memcache_item->payload.epid));
	psmi_assert(sbuf == memcache_item->payload.start);
	if (handle == memcache_item->payload.ze_handle &&
		alloc_id == memcache_item->payload.alloc_id) {
		return PSM2_OK;
	}
	_HFI_DBG("cache remove stale entry: new start=%lu,handle=%u,alloc_id=%lu\n",
			sbuf, handle, alloc_id);

	cache->stats->gpu_ipc_cache_remove++;
	ips_cl_qmap_remove_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems--;
	psm3_oneapi_ze_memhandle_delete(memcache_item->payload.buf_ptr);
	psm3_oneapi_ze_idleq_remove(cache, memcache_item);
	memset(memcache_item, 0, sizeof(*memcache_item));
	psm3_mpool_put(memcache_item);
	return PSM2_OK_NO_PROGRESS;
}

/*
 * Current eviction policy: Least Recently Used.
 */
static void
psm3_oneapi_ze_memhandle_cache_evict(psm3_oneapi_ze_memhandle_cache_t cache)
{
	cache->stats->gpu_ipc_cache_evict++;
	cl_map_item_t *p_item = LAST(cache);
	_HFI_VDBG("Removing (epid=%s,start=%lu,dev_ptr=%p,it=%p) from ze_memhandle_cachemap.\n",
			psm3_epid_fmt_internal(p_item->payload.epid, 0), p_item->payload.start,
			p_item->payload.buf_ptr, p_item);
	ips_cl_qmap_remove_item(&cache->map, p_item);
	cache->stats->gpu_ipc_cache_nelems--;
	psm3_oneapi_ze_memhandle_delete(p_item->payload.buf_ptr);
	psm3_oneapi_ze_idleq_remove_last(cache, p_item);
	memset(p_item, 0, sizeof(*p_item));
	psm3_mpool_put(p_item);
}

static psm2_error_t
psm3_oneapi_ze_memhandle_cache_register(psm3_oneapi_ze_memhandle_cache_t cache,
				uintptr_t sbuf, uint32_t handle,
				psm2_epid_t epid,
				void *buf_ptr, uint64_t alloc_id)
{
	if (NELEMS(cache) == cache->size)
		psm3_oneapi_ze_memhandle_cache_evict(cache);

	cl_map_item_t* memcache_item = psm3_mpool_get(cache->mpool);
	/* memcache_item cannot be NULL as we evict
	 * before the call to mpool_get. Check has
	 * been fixed to help with klockwork analysis.
	 */
	if (memcache_item == NULL)
		return PSM2_NO_MEMORY;
	memcache_item->payload.start = sbuf;
	memcache_item->payload.ze_handle = handle;
	memcache_item->payload.buf_ptr = buf_ptr;
	memcache_item->payload.alloc_id = alloc_id;
	memcache_item->payload.epid = epid;
	memcache_item->payload.cache = cache;
	ips_cl_qmap_insert_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems++;
	if (cache->stats->gpu_ipc_cache_nelems > cache->stats->gpu_ipc_cache_max_nelems)
		cache->stats->gpu_ipc_cache_max_nelems = cache->stats->gpu_ipc_cache_nelems;
	psm3_oneapi_ze_idleq_insert(cache, memcache_item);
	_HFI_VDBG("registered: handle %u sbuf 0x%lx ptr %p alloc_id %lu\n",
			handle, sbuf, buf_ptr, alloc_id);
	return PSM2_OK;
}

#ifndef PSM_HAVE_PIDFD
static inline psm2_error_t psm3_oneapi_ze_prepare_fds_for_ipc_import(
		uint32_t gem_handle, int device_index, int *ipc_fd,
		psm2_epaddr_t epaddr)
{
	am_epaddr_t *am_epaddr = (am_epaddr_t*)epaddr;
	int fd;
	struct drm_prime_handle open_fd = {0, 0, -1};

	if (device_index >= psm3_num_oneapi_ze_devices || device_index < 0) {
		_HFI_ERROR("psm3_oneapi_ze_memhandle_acquire received invalid device_index from peer: %d\n",
			device_index);
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"device_index "
			"invalid - received from peer: %d",
			device_index);
		return PSM2_INTERNAL_ERR;
	}
	fd = am_epaddr->gpu_specific.ze_peer_fds[device_index];
	psm3_oneapi_ze_cur_dev = &psm3_oneapi_ze_devices[device_index];
	open_fd.flags = DRM_CLOEXEC | DRM_RDWR;
	open_fd.handle = gem_handle;
	if (ioctl(fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &open_fd) < 0) {
		_HFI_ERROR("ioctl failed for DRM_IOCTL_PRIME_HANDLE_TO_FD: %s\n", strerror(errno));
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"ioctl "
			"failed for DRM_IOCTL_PRIME_HANDLE_TO_FD errno=%d",
			errno);
		return PSM2_INTERNAL_ERR;
	}
	*ipc_fd = open_fd.fd;

	return PSM2_OK;
}
#else
static inline psm2_error_t psm3_oneapi_ze_prepare_fds_for_ipc_import(
		uint32_t handle, int device_index, int *ipc_fd,
		psm2_epaddr_t epaddr)
{
	int fd;
	am_epaddr_t *am_epaddr = (am_epaddr_t *)epaddr;

	fd = syscall(__NR_pidfd_getfd, am_epaddr->gpu_specific.ze_pidfd, handle, 0);
	if (fd < 0) {
		_HFI_ERROR("pidfd_getfd failed %d: %s\n", fd, strerror(errno));
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"pidfd_getfd failed errno=%d (%s)",
			errno, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	*ipc_fd = fd;

	return PSM2_OK;
}
#endif /* PSM_HAVE_PIDFD */
#endif /* defined(HAVE_DRM) || defined(HAVE_LIBDRM) */

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
static void *psm3_oneapi_ze_import_ipc_buf(uint32_t fd, uint8_t alloc_type)
{
	ze_external_memory_import_fd_t import_desc = {};
	void *ze_ipc_buf = NULL;

	import_desc.stype = ZE_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMPORT_FD;
	import_desc.flags = ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF;
	import_desc.fd = fd;

	switch(alloc_type) {
	case ZE_MEMORY_TYPE_HOST:
	{
		ze_host_mem_alloc_desc_t host_desc = {};

		host_desc.stype = ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC;
		host_desc.pNext = &import_desc;
		/* size & alignment are not used since this is an import.*/
		PSM3_ONEAPI_ZE_CALL(zeMemAllocHost, psm3_oneapi_ze_context, &host_desc,
					0, 0, &ze_ipc_buf);
	}
		break;
	case ZE_MEMORY_TYPE_DEVICE:
	{
		ze_device_mem_alloc_desc_t dev_desc = {};

		dev_desc.stype = ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC;
		dev_desc.pNext = &import_desc;
		/* size & alignment are not used since this is an import. */
		PSM3_ONEAPI_ZE_CALL(zeMemAllocDevice, psm3_oneapi_ze_context, &dev_desc,
					0, 0, psm3_oneapi_ze_cur_dev->dev, &ze_ipc_buf);
	}
		break;
	default:
		_HFI_ERROR("Invalid alloc_type %u for fd %u\n",
				alloc_type, fd);
		return NULL;
	}

	return ze_ipc_buf;
}
#endif /* defined(HAVE_DRM) || defined(HAVE_LIBDRM) */

/*
 * The key used to search the cache is the senders buf address pointer and
 * epid.  The sender will have used zeMemGetAddressRange
 * to find the start of the memory containing the buffer (supplied as sbuf)
 * Upon match, we must validate the entry we find and may need to replace it.
 */
static void *
psm3_oneapi_ze_memhandle_acquire(psm3_oneapi_ze_memhandle_cache_t cache,
			uintptr_t sbuf, uint32_t handle,
			psm2_epaddr_t epaddr, int device_index,
			uint64_t alloc_id, uint8_t alloc_type)
{
	void *buf_ptr = NULL;
	psm2_epid_t epid = epaddr->epid;
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	int ipc_fd = -1;
#endif
	_HFI_VDBG("sbuf=%lu,handle=%u,epid=%s\n",
			sbuf, handle, psm3_epid_fmt_internal(epid, 0));
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)

	if (!cache) {
		if (psm3_oneapi_ze_prepare_fds_for_ipc_import(handle, device_index, &ipc_fd,
							epaddr) == PSM2_OK) {
			buf_ptr = psm3_oneapi_ze_import_ipc_buf(ipc_fd, alloc_type);
			if (ipc_fd >= 0) {
				if (close(ipc_fd) < 0) {
					_HFI_ERROR("close failed for ipc_fd: %s\n", strerror(errno));
					psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
						"close "
						"failed for ipc_fd %d errno=%d",
						ipc_fd, errno);
					return NULL;
				}
			}
		}
		return buf_ptr;
	}

	psm3_oneapi_ze_cache_item key = {
		.start = (unsigned long) sbuf,
		.epid = epid
	};

	/*
	 * preconditions:
	 *  1) buffer [start,epid) may or may not be in cache->map already
	 *  2) there are no duplicate entries in cache->map
	 * postconditions:
	 *  1) buffer is in cache->map with same handle, epid, alloc_id
	 *  2) there are no duplicate entries in cache->map
	 *
	 * The key used to search the cache is the senders buf address pointer
	 * and epid.
	 * Upon a succesful hit in the cache, additional validation is required
	 * as the handle or alloc_id could be stale.
	 */
	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, &key);
	if (p_item->payload.start) {
		// confirm the entry for sbuf matches the handle and is not stale
		if (psm3_oneapi_ze_memhandle_cache_validate(cache, p_item, sbuf, handle,
							epid, alloc_id) == PSM2_OK) {
			cache->stats->gpu_ipc_cache_hit++;
			psm3_oneapi_ze_idleq_reorder(cache, p_item);
			return p_item->payload.buf_ptr;
		}

		// buffer found was stale am_oneapi_memhandle_cache_validate()
		// closed and removed existing entry.
		// Should find no more duplicates
#ifdef PSM_DEBUG
		p_item = ips_cl_qmap_searchv(&cache->map, &key);
		psmi_assert(! p_item->payload.start);
#endif
	}
	cache->stats->gpu_ipc_cache_miss++;

	if (psm3_oneapi_ze_prepare_fds_for_ipc_import(handle, device_index, &ipc_fd,
						epaddr) == PSM2_OK) {
		buf_ptr = psm3_oneapi_ze_import_ipc_buf(ipc_fd, alloc_type);
		if (ipc_fd >= 0) {
			if (close(ipc_fd) < 0) {
				_HFI_ERROR("close failed for ipc_fd: %s\n", strerror(errno));
				psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"close "
					"failed for ipc_fd %d errno=%d",
					ipc_fd, errno);
				return NULL;
			}
		}
		if (!buf_ptr)
			return NULL;
	} else {
		return NULL;
	}

	psm3_oneapi_ze_memhandle_cache_register(cache, sbuf, handle, epid, buf_ptr,
					alloc_id);
	return buf_ptr;
#else // if no drm, set up to return NULL as oneapi ipc handles don't work without drm
// TBD - caller will assert when we return NULL, so should it be a build error
// if DRM not available?  What works without DRM?
	buf_ptr = NULL;
	return buf_ptr;
#endif // defined(HAVE_DRM) || defined(HAVE_LIBDRM)

}

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
static void psm3_oneapi_ze_memhandle_delete(void *buf_ptr)
{
	/* Release the reference to the buffer */
	PSM3_ONEAPI_ZE_CALL(zeMemFree, psm3_oneapi_ze_context, buf_ptr);

#ifndef PSM_HAVE_PIDFD
	/*
	 * If pidfd is not used, we need to call GEM_CLOSE ioctl to remove the
	 * GEM handle from the handle cache of the peer device file's
	 * private file data in the kernel to avoid handle leak. However, we
	 * will have a potential risk condition that will fail a later request:
	 * (1) 3 requests with buf1, buf2, and buf1 are sent from sender side.
	 *     Requests 1 and 3 uses the same buffer and therefore have the
	 *     same gem_handle1.
	 * (2) buf1 is received and put into cache;
	 * (3) buf2 is received and buf1 is evicted from cache due to some
	 *     condition (small cache size). As a result, gem_handle1 is closed
	 *     through GEM_CLOSE ioctl. buf2 is put into cache.
	 * (4) Request 3 (with buf1) is received and HANDLE_TO_FD ioctl will
	 *     fail because the gem_handle has been removed from peer device
	 *     file's handle cache.
	 * For this reason, we prefer to leak the GEM handle over calling
	 * GEM_CLOSE.
	 */
#endif
}
#endif /* HAVE_DRM or HAVE_LIBDRM */

static void
psm3_oneapi_ze_memhandle_release(psm3_oneapi_ze_memhandle_cache_t cache,
			void *buf_ptr)
{
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	if (!cache)
		psm3_oneapi_ze_memhandle_delete(buf_ptr);
#endif // defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	return;
}

// end of oneAPI Level Zero IPC MemHandle Cache
//***************************************************************************

// IPC Handle management for OneAPI Level Zero

#ifndef PSM_HAVE_PIDFD
/*
 * psm3_onapi_ze_init_fds - initialize the file descriptors (ze_dev_fds) 
 *
 * Open the file descriptors for our GPUs (psm3_ze_dev_fds[])
 *
 * The file descriptors are used in intra-node communication to pass to peers
 * via socket with sendmsg/recvmsg SCM_RIGHTS message type.
 *
 */

static psm2_error_t psm3_onapi_ze_init_fds(void)
{
	const char *dev_dir = "/dev/dri/by-path/";
	const char *suffix = "-render";
	DIR *dir;
	struct dirent *ent = NULL;
	char dev_name[NAME_MAX];
	int i = 0, ret;

	if (psm3_num_ze_dev_fds)
		return PSM2_OK;

	dir = opendir(dev_dir);
	if (dir == NULL)
		return PSM2_INTERNAL_ERR;

	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.' ||
			strstr(ent->d_name, suffix) == NULL)
			continue;

		memset(dev_name, 0, sizeof(dev_name));
		ret = snprintf(dev_name, NAME_MAX, "%s%s", dev_dir, ent->d_name);
		if (ret < 0 || ret >= NAME_MAX) {
			_HFI_INFO("GPU dev name too long: %s%s\n", dev_dir, ent->d_name);
			goto err;
		}

		psm3_ze_dev_fds[i] = open(dev_name, O_RDWR);
		if (psm3_ze_dev_fds[i] == -1) {
			_HFI_INFO("Failed to open %s GPU dev FD: %s\n", dev_name,
						strerror(errno));
			goto err;
		}
		_HFI_DBG("Opened %s GPU dev FD: %d\n", dev_name,
				psm3_ze_dev_fds[i]);
		i++;
		psm3_num_ze_dev_fds++;
	}
	(void) closedir(dir);
	_HFI_DBG("Opened %d GPU dev FDs\n", psm3_num_ze_dev_fds);
	return PSM2_OK;

err:
	(void) closedir(dir);
	return PSM2_INTERNAL_ERR;
}

/*
 * psm3_oneapi_ze_get_dev_fds - fetch device file descriptors
 *
 * Returns a pointer to ze_dev_fds while putting the number
 * of fds into the in/out nfds parameter
 *
 */

static int *psm3_oneapi_ze_get_dev_fds(int *nfds)
{
	*nfds = psm3_num_ze_dev_fds;
	return psm3_ze_dev_fds;
}

/*
 * psm3_oneapi_ze_sendmsg_fds - send device file descriptors over socket w/ sendmsg
 *
 * Prepares message of type SCM_RIGHTS, copies file descriptors as payload,
 * and sends over socket via sendmsg while creating appropriate fd numbers
 * for dest (effectively a dup(2) of our file descriptor)
 *
 * returns -errno on error or number of bytes sent (>0) on success
 */

static int psm3_oneapi_ze_sendmsg_fds(int sock, int *fds, int nfds, psm2_epid_t epid)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	int64_t peer_id = *(int64_t *)&epid;
	char *ctrl_buf;
	size_t ctrl_size;
	int ret;

	ctrl_size = sizeof(*fds) * nfds;
	ctrl_buf = (char *)psmi_calloc(NULL, UNDEFINED, 1, CMSG_SPACE(ctrl_size));
	if (!ctrl_buf)
		return -ENOMEM;

	iov.iov_base = &peer_id;
	iov.iov_len = sizeof(peer_id);

	memset(&msg, 0, sizeof(msg));
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(ctrl_size);

	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(ctrl_size);
	memcpy(CMSG_DATA(cmsg), fds, ctrl_size);

	ret = sendmsg(sock, &msg, 0);
	if (ret < 0)
		ret = -errno;
	else if (! ret)
		ret = -EAGAIN;

	psmi_free(ctrl_buf);
	return ret;
}

/*
 * psm3_oneapi_ze_recvmsg_fds - receive device file descriptors from socket w/ recvmsg
 *
 * Prepares message buffer of type SCM_RIGHTS, receives message from socket
 * via recvmsg, and copies device file descriptors to in/out parameter.
 * The received file descriptors are usable in our process and need to
 * be closed when done being used
 *
 * returns -errno on error or number of bytes received (>0) on success
 */

static int psm3_oneapi_ze_recvmsg_fd(int sock, int *fds, int nfds, psm2_epid_t epid)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	int64_t peer_id = *(int64_t *)&epid;
	char *ctrl_buf;
	size_t ctrl_size;
	int ret;

	ctrl_size = sizeof(*fds) * nfds;
	ctrl_buf = (char *)psmi_calloc(NULL, UNDEFINED, 1, CMSG_SPACE(ctrl_size));
	if (!ctrl_buf)
		return -ENOMEM;

	iov.iov_base = &peer_id;
	iov.iov_len = sizeof(peer_id);

	memset(&msg, 0, sizeof(msg));
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(ctrl_size);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	ret = recvmsg(sock, &msg, 0);
	if (ret < 0) {
		ret = -errno;
	} else if (ret != sizeof(peer_id)) {
		_HFI_CONNDBG("recvmsg from: %s returns %d expect %u\n",
						psm3_epid_fmt_addr(epid, 0), ret,
						(unsigned)sizeof(peer_id) );
		ret = -EAGAIN;
		goto out;
	}

	psmi_assert(!(msg.msg_flags & (MSG_TRUNC | MSG_CTRUNC)));
	cmsg = CMSG_FIRSTHDR(&msg);
	psmi_assert(cmsg && cmsg->cmsg_len == CMSG_LEN(ctrl_size) &&
		cmsg->cmsg_level == SOL_SOCKET &&
		cmsg->cmsg_type == SCM_RIGHTS && CMSG_DATA(cmsg));
	memcpy(fds, CMSG_DATA(cmsg), ctrl_size);
out:
	psmi_free(ctrl_buf);
	return ret;
}

/*
 * psm3_onapi_ze_init_ipc_socket - initialize ipc socket in ep
 *
 * Set up the AF_UNIX ipc socket in the ep for listen mode. Name it
 * using our epid, and bind it.
 *
 */

static psm2_error_t psm3_onapi_ze_init_ipc_socket(struct ptl_am *ptl)
{
	psm2_error_t err = PSM2_OK;
	int ret;
	struct sockaddr_un sockaddr = {0};
	socklen_t len = sizeof(sockaddr);

	if ((ptl->gpu_specific.ze_ipc_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		_HFI_ERROR("error creating GPU dev FDs AF_UNIX sock: %s\n",
					strerror(errno));
		err =  PSM2_INTERNAL_ERR;
		goto fail;
	}

	sockaddr.sun_family = AF_UNIX;
	snprintf(sockaddr.sun_path, 108, "/dev/shm/psm3_shm.ze_sock2.%ld.%s",
				(long int) getuid(), psm3_epid_fmt_internal(ptl->epid, 0));
	ptl->gpu_specific.ze_listen_sockname = psmi_strdup(NULL, sockaddr.sun_path);
	if (ptl->gpu_specific.ze_listen_sockname == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}

	if ((ret = bind(ptl->gpu_specific.ze_ipc_socket, (struct sockaddr *) &sockaddr, len)) < 0) {
		_HFI_ERROR("error binding GPU dev FDs AF_UNIX sock to %s: %s\n",
					sockaddr.sun_path, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	if ((ret = listen(ptl->gpu_specific.ze_ipc_socket, 256)) < 0) {
		_HFI_ERROR("error listening on GPU dev FDs AF_UNIX sock %s: %s\n",
					sockaddr.sun_path, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	return PSM2_OK;

fail:
	if (ptl->gpu_specific.ze_ipc_socket >= 0)
		close(ptl->gpu_specific.ze_ipc_socket);
	ptl->gpu_specific.ze_ipc_socket = -1;
	if (ptl->gpu_specific.ze_listen_sockname)
		psmi_free(ptl->gpu_specific.ze_listen_sockname);
	ptl->gpu_specific.ze_listen_sockname = NULL;
	return err;
}

/*
 * psm3_oneapi_ze_receive_dev_fds - receive the dev fds on the listen socket
 *
 * Set up the listen socket to be polled for POLLIN. When the event is
 * received, accept for the new socket and then read the peer epid,
 * and locate the epaddr for it. Then receive the dev fds to be stored
 * in the am_epaddr.
 *
 * returns:
 *		PSM2_OK - GPU dev FDs received from a peer
 *		PSM2_OK_NO_PROGRESS - nothing received
 *		other - error
 */

static psm2_error_t psm3_oneapi_ze_receive_dev_fds(struct ptl_am *ptl)
{
	psm2_error_t err = PSM2_OK;
	struct pollfd fdset;
	int newsock = -1;

	fdset.fd = ptl->gpu_specific.ze_ipc_socket;
	fdset.events = POLLIN;

	if (poll(&fdset, 1, 0) <= 0)
		return PSM2_OK_NO_PROGRESS;

	{
		struct sockaddr_un sockaddr = {0};
		socklen_t len = sizeof(sockaddr);
		int nfds = psm3_num_ze_dev_fds;
		int nread;
		psm2_epid_t epid;
		psm2_epaddr_t epaddr;
		am_epaddr_t *am_epaddr;

		newsock = accept(ptl->gpu_specific.ze_ipc_socket, (struct sockaddr *)&sockaddr, &len);
		if (newsock < 0) {
			_HFI_ERROR("GPU dev FDs AF_UNIX accept failed: %s\n",
						strerror(errno));
			err =  PSM2_INTERNAL_ERR;
			goto fail;
		} else {
			int ret;
			// technically we could get less than we asked for and need to
			// call recv again in future but our transfers are small enough
			// we should get it all
			if ((nread = recv(newsock, &epid, sizeof(epid), 0)) < 0) {
				_HFI_ERROR("GPU dev FDs AF_UNIX recv failed: %s\n",
							strerror(errno));
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			if (nread != sizeof(epid)) {
				_HFI_ERROR("GPU dev FDs AF_UNIX recv incomplete: %d\n", nread);
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			// we only poll for recv FDs after processing a am_shm connect
			// so the epid should always be known
			if ((epaddr = psm3_epid_lookup(ptl->ep, epid)) == NULL) {
				_HFI_ERROR("Peer Unknown, unable to receive GPU dev FDs from: %s\n",
								psm3_epid_fmt_addr(epid, 0));
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			am_epaddr = (am_epaddr_t *)epaddr;
			am_epaddr->gpu_specific.ze_num_peer_fds = nfds;
			ret = psm3_oneapi_ze_recvmsg_fd(newsock, am_epaddr->gpu_specific.ze_peer_fds, nfds, ptl->epid);
			if (ret <= 0) {
				_HFI_ERROR("Unable to recvmsg %d GPU dev FDs from: %s: %s\n",
								nfds, psm3_epid_fmt_addr(epid, 0),
								strerror(-ret));
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			_HFI_CONNDBG("%d GPU dev FDs Received from: %s\n",
								nfds, psm3_epid_fmt_addr(epid, 0));
		}
	}

fail:
	if (newsock >= 0)
		close(newsock);
	return err;
}

/*
 * psm3_oneapi_ze_send_dev_fds - do next step to send the dev fds to the peer's
 *		listen socket
 *
 * Check the connected state and proceed accordingly:
 * - ZE_SOCK_NOT_CONNECTED
 *     We have not done anything yet, so connect and send our epid,
 *     followed by the dev fds. Set state to ZE_SOCK_DEV_FDS_SENT
 * - ZE_SOCK_DEV_FDS_SENT
 *     The dev fds have been sent. Issue ioctl to see if the output
 *     queue has been emptied indicating that the peer has read the data.
 *     If so, set state to ZE_SOCK_DEV_FDS_SENT_AND_RECD.
 * - ZE_SOCK_DEV_FDS_SENT_AND_RECD
 *     We are done, just return.
 *
 * returns:
 *		PSM2_OK - next step completed
 *		PSM2_OK_NO_PROGRESS - nothing to do
 *		other - error
 */

static psm2_error_t psm3_oneapi_ze_send_dev_fds(struct ptl_am *ptl, struct am_epaddr *am_epaddr)
{
	switch (am_epaddr->gpu_specific.ze_sock_connected_state) {
		case ZE_SOCK_DEV_FDS_SENT_AND_RECD:
			return PSM2_OK_NO_PROGRESS;
			break;

		case ZE_SOCK_DEV_FDS_SENT:
		{
			int pending;

			psmi_assert(am_epaddr->gpu_specific.ze_sock >= 0);
			if_pf (ioctl(am_epaddr->gpu_specific.ze_sock, SIOCOUTQ, &pending) != 0) {
				return	psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"error sending dev FDs: %s\n", strerror(errno));
			}
			if (pending == 0) {
				am_epaddr->gpu_specific.ze_sock_connected_state = ZE_SOCK_DEV_FDS_SENT_AND_RECD;
				_HFI_CONNDBG("GPU dev FDs Send Completed to: %s\n",
								psm3_epid_fmt_addr(am_epaddr->epaddr.epid, 0));
				close(am_epaddr->gpu_specific.ze_sock);
				am_epaddr->gpu_specific.ze_sock = -1;
				return PSM2_OK;
			}
			// be paranoid just in case 1st call to send_dev_fds for given
			// epaddr gets here
			if (! ptl->gpu_specific.ze_need_dev_fds_poll)
				_HFI_CONNDBG("restart GPU dev FDs poll\n");
			ptl->gpu_specific.ze_need_dev_fds_poll = 1;
			return PSM2_OK_NO_PROGRESS;
			break;
		}

		case ZE_SOCK_NOT_CONNECTED:
		{
			struct sockaddr_un sockaddr = {0};
			socklen_t len = sizeof(sockaddr);
			psm2_epid_t peer_epid = am_epaddr->epaddr.epid;
			int *fds, nfds;

			if (!ptl->gpu_specific.ze_need_dev_fds_poll)
				_HFI_CONNDBG("restart GPU dev FDs poll\n");
			ptl->gpu_specific.ze_need_dev_fds_poll = 1;

			fds = psm3_oneapi_ze_get_dev_fds(&nfds);

			if ((am_epaddr->gpu_specific.ze_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
				_HFI_ERROR("error creating GPU dev FDs AF_UNIX sock: %s\n",
							strerror(errno));
				goto fail;
			}

			sockaddr.sun_family = AF_UNIX;
			snprintf(sockaddr.sun_path, 108, "/dev/shm/psm3_shm.ze_sock2.%ld.%s",
				(long int) getuid(), psm3_epid_fmt_internal(peer_epid, 0));

			if (connect(am_epaddr->gpu_specific.ze_sock, (struct sockaddr *) &sockaddr, len) < 0) {
				_HFI_ERROR("GPU dev FDs connect to %s (via %s) failed: %s\n",
								psm3_epid_fmt_addr(am_epaddr->epaddr.epid, 0),
								sockaddr.sun_path,  strerror(errno));
				goto fail;
			} else {
				int ret;
				ret = send(am_epaddr->gpu_specific.ze_sock, &ptl->epid, sizeof(ptl->epid), 0);
				if (ret < 0) {
					_HFI_ERROR("GPU dev FDs send to %s (via %s) failed: %s\n",
							psm3_epid_fmt_addr(am_epaddr->epaddr.epid, 0),
							sockaddr.sun_path, strerror(errno));
					goto fail;
				}
		
				ret = psm3_oneapi_ze_sendmsg_fds(am_epaddr->gpu_specific.ze_sock, fds, nfds, peer_epid);
				if (ret <= 0) {
					/* ret is -errno */
					_HFI_ERROR("GPU dev FDs sendmsg to %s (via %s) failed: %s\n",
							psm3_epid_fmt_addr(am_epaddr->epaddr.epid, 0),
							sockaddr.sun_path,  strerror(-ret));
					goto fail;
				}
				am_epaddr->gpu_specific.ze_sock_connected_state = ZE_SOCK_DEV_FDS_SENT;
				_HFI_CONNDBG("%d GPU dev FDs Posted Send to: %s (via %s)\n",
						nfds, psm3_epid_fmt_addr(am_epaddr->epaddr.epid, 0),
						sockaddr.sun_path);
				return PSM2_OK;
			}
			/* NOTREACHED */
			break;
		}

		default:
			return PSM2_INTERNAL_ERR;
			break;
	}
	/* NOTREACHED */
	return PSM2_INTERNAL_ERR;

fail:
	if (am_epaddr->gpu_specific.ze_sock >= 0)
		close(am_epaddr->gpu_specific.ze_sock);
	am_epaddr->gpu_specific.ze_sock = -1;
	return PSM2_INTERNAL_ERR;
}

// simple test if dev_fds bi-dir exchange completed for given epaddr
// 1 = yes, 0 = no
static int psm3_oneapi_ze_dev_fds_exchanged(struct am_epaddr *am_epaddr)
{
	return (am_epaddr->gpu_specific.ze_sock_connected_state == ZE_SOCK_DEV_FDS_SENT_AND_RECD
			&& am_epaddr->gpu_specific.ze_num_peer_fds) ;
}

/*
 * psm3_oneapi_ze_check_dev_fds_exchanged - check that dev fds have been bi-dir exchanged
 * with given peer. Poll to try and move forward as needed.
 *
 * connect state ZE_SOCK_DEV_FDS_SENT_AND_RECD indicates peer has received
 * our send of dev_fds
 *
 * num_peer_fds indicates if we received peer's fds.
 *
 * if both are satisfied, exchange is complete, return PSM2_OK
 *
 *Returns:
 *   PSM2_OK - both are done
 *   PSM2_OK_NO_PROGRESS - more work needed
 *   other - error
 */
static psm2_error_t psm3_oneapi_ze_check_dev_fds_exchanged(struct ptl_am *ptl, struct am_epaddr *am_epaddr)
{
	psm2_error_t err;
	psm2_error_t ret;

	psmi_assert(am_epaddr);
	psmi_assert(! psm3_epid_zero_internal(am_epaddr->epaddr.epid));

	if (psm3_oneapi_ze_dev_fds_exchanged(am_epaddr))
		return PSM2_OK;

	if (am_epaddr->cstate_outgoing != AMSH_CSTATE_OUTGOING_ESTABLISHED
		&& am_epaddr->cstate_incoming != AMSH_CSTATE_INCOMING_ESTABLISHED)
		return PSM2_OK_NO_PROGRESS;

	// try to move forward 1 step
	err = psm3_oneapi_ze_send_dev_fds(ptl, am_epaddr);
	if (am_epaddr->gpu_specific.ze_sock_connected_state == ZE_SOCK_DEV_FDS_SENT_AND_RECD)
		err = PSM2_OK;
	else /* err will be NO_PROGRESS or worse */
		err = psm3_error_cmp(err, PSM2_OK_NO_PROGRESS);

	// only poll recv if we need to
	ret = PSM2_OK_NO_PROGRESS;	// keep KW happy
	if (am_epaddr->gpu_specific.ze_num_peer_fds == 0) 
		ret = psm3_oneapi_ze_receive_dev_fds(ptl);
	if (am_epaddr->gpu_specific.ze_num_peer_fds) 
		ret = PSM2_OK;

	/* worst err, NO_PROGRESS is worse than PSM2_OK */
	return psm3_error_cmp(ret, err);
}

// check if all successful epid/epaddr in req have exchanged GPU dev FDs
// when called it assumes all the good epid have completed so it does not
// check failed epid and just treats them as done for this phase
// return:
//	PSM2_OK - all that can be done are done
//	PSM2_OK_NO_PROGRESS - more to be done
static psm2_error_t
psm3_oneapi_ze_shm_ep_connreq_poll_dev_fds(struct ptl_am *ptl, struct am_ptl_connection_req *req)
{
	int num_left = 0;
	int i;

	for (i = 0; i < req->numep; i++) {
		if (req->epid_mask[i] == AMSH_CMASK_NONE)
			continue;
		if (req->epid_mask[i] != AMSH_CMASK_DONE || req->errors[i])
			continue;
		psmi_assert(req->epaddr[i]);
		psmi_assert(! psm3_epid_zero_internal(req->epaddr[i]->epid));
		if (PSM2_OK != psm3_oneapi_ze_check_dev_fds_exchanged(ptl, (struct am_epaddr *)(req->epaddr[i])))
			num_left++;
	}
	if (num_left == 0)
		return PSM2_OK;
	else
		return PSM2_OK_NO_PROGRESS;	// not done everyone yet
}

/*
 * psm3_oneapi_ze_poll_dev_fds_exchanged - poll to make forward progress on
 * GPU dev FDs exchange
 *
 * Loop through the epaddrs in am_ep and check_dev_fds_exchanged
 *
 * Returns:
 *		PSM2_OK - we found some work to do and made progress
 *		PSM2_OK_NO_PROGRESS - didn't find anything to do
 *		other - error
 */

static psm2_error_t psm3_oneapi_ze_poll_dev_fds_exchange(struct ptl_am *ptl)
{
	psm2_error_t err = PSM2_OK_NO_PROGRESS;
	psm2_error_t ret;
	int i;
	int num_left = 0;

	err = psm3_oneapi_ze_receive_dev_fds(ptl);

	for (i = 0; i <= ptl->max_ep_idx; i++) {
		am_epaddr_t *am_epaddr = (am_epaddr_t *)ptl->am_ep[i].epaddr;

		if (!am_epaddr || psm3_epid_zero_internal(ptl->am_ep[i].epid))
			continue;

		if (psm3_oneapi_ze_dev_fds_exchanged(am_epaddr))
			continue;

		num_left++;	// causes one extra poll if complete now below, but no harm

		// don't try if uni-dir REQ/REP is incomplete
		if (am_epaddr->cstate_outgoing != AMSH_CSTATE_OUTGOING_ESTABLISHED
			&& am_epaddr->cstate_incoming != AMSH_CSTATE_INCOMING_ESTABLISHED)
			continue;

		// try to move forward 1 step
		ret = psm3_oneapi_ze_send_dev_fds(ptl, am_epaddr);
		if (ret > PSM2_OK_NO_PROGRESS)
			err = psm3_error_cmp(ret, err);
		else if (ret == PSM2_OK && err == PSM2_OK_NO_PROGRESS)
			err = ret;
	}
	if (num_left == 0 && ptl->gpu_specific.ze_need_dev_fds_poll)
		_HFI_CONNDBG("stop GPU dev FDs poll\n");
	ptl->gpu_specific.ze_need_dev_fds_poll = (num_left != 0);

	return err;
}

static void psm3_oneapi_ze_sock_detach(struct ptl_am *ptl)
{
	if (ptl->gpu_specific.ze_ipc_socket >= 0)
		close(ptl->gpu_specific.ze_ipc_socket);
	ptl->gpu_specific.ze_ipc_socket = -1;
	if (ptl->gpu_specific.ze_listen_sockname) {
		unlink(ptl->gpu_specific.ze_listen_sockname);
		psmi_free(ptl->gpu_specific.ze_listen_sockname);
	}
	ptl->gpu_specific.ze_listen_sockname = NULL;
}
#endif /* not PSM_HAVE_PIDFD */

static psm2_error_t psm3_oneapi_ze_shm_init(struct ptl_am *ptl,
								psm2_mq_stats_t *stats)
{
#ifndef PSM_HAVE_PIDFD
	psm2_error_t err;

	ptl->gpu_specific.ze_ipc_socket = -1;
	if ((err = psm3_onapi_ze_init_ipc_socket(ptl)) != PSM2_OK)
		return err;
	if ((err = psm3_onapi_ze_init_fds()) != PSM2_OK)
		return err;
#endif

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	// TBD - should we have generic names for these env variables
	// PSM3_GPU_MEMCACHE_ENABLED, PSM3_GPU_MEMCACHE_SIZE?
	union psmi_envvar_val env_memcache_enabled;
	psm3_getenv("PSM3_ONEAPI_MEMCACHE_ENABLED",
			"PSM oneapi ipc memhandle cache enabled (default is enabled)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1, &env_memcache_enabled);
	if (env_memcache_enabled.e_uint) {
		union psmi_envvar_val env_memcache_size;
		psm3_getenv("PSM3_ONEAPI_MEMCACHE_SIZE",
				"Size of the oneapi ipc memhandle cache ",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)ONEAPI_MEMHANDLE_CACHE_SIZE,
				&env_memcache_size);
		return psm3_oneapi_ze_memhandle_cache_alloc(
					(psm3_oneapi_ze_memhandle_cache_t *)&ptl->memhandle_cache,
					env_memcache_size.e_uint, stats);
#endif
	}
	return PSM2_OK;
}

static void psm3_oneapi_ze_shm_finalize(struct ptl_am *ptl)
{
#ifndef PSM_HAVE_PIDFD
	psm3_oneapi_ze_sock_detach(ptl);
#endif
	if (ptl->memhandle_cache)
		psm3_oneapi_ze_memhandle_cache_free(ptl->memhandle_cache);
	ptl->memhandle_cache = NULL;
	return;
}

static psm2_error_t psm3_oneapi_ze_shm_epaddr_add(struct ptl_am *ptl, struct am_epaddr *am_epaddr)
{
#ifdef PSM_HAVE_PIDFD
	am_epaddr->gpu_specific.ze_pidfd = syscall(SYS_pidfd_open, ptl->am_ep[am_epaddr->shmidx].pid, 0);
	if (am_epaddr->gpu_specific.ze_pidfd < 0) {
		_HFI_ERROR("pidfd_open failed: pid %u, ret %d (%s)\n",
				ptl->am_ep[am_epaddr->shmidx].pid,
				am_epaddr->gpu_specific.ze_pidfd,
				strerror(errno));
		return PSM2_NO_MEMORY;
	}
#else
	am_epaddr->gpu_specific.ze_num_peer_fds = 0;
	{
		int i;
		for (i=0; i < MAX_ZE_DEVICES; i++)
			am_epaddr->gpu_specific.ze_peer_fds[i] = -1;
	}
	am_epaddr->gpu_specific.ze_sock_connected_state = ZE_SOCK_NOT_CONNECTED;
	am_epaddr->gpu_specific.ze_sock = -1;
#endif
	return PSM2_OK;
}

static void psm3_oneapi_ze_shm_epaddr_free(struct am_epaddr *am_epaddr)
{
#ifdef PSM_HAVE_PIDFD
	if (am_epaddr->gpu_specific.ze_pidfd >= 0)
		close(am_epaddr->gpu_specific.ze_pidfd);
#else
	{
		int i;
		for (i=0; i < MAX_ZE_DEVICES; i++)
			if (am_epaddr->gpu_specific.ze_peer_fds[i] >= 0)
				close(am_epaddr->gpu_specific.ze_peer_fds[i]);
	}
	if (am_epaddr->gpu_specific.ze_sock >= 0)
		close(am_epaddr->gpu_specific.ze_sock);
#endif
}

static int psm3_oneapi_ze_shm_dev_fds_needed()
{
#ifndef PSM_HAVE_PIDFD
	return 1;
#else
	return 0;
#endif
}

static void psm3_oneapi_ze_shm_dev_fds_send(struct ptl_am *ptl, struct am_epaddr *am_epaddr)
{
#ifndef PSM_HAVE_PIDFD
	psm3_oneapi_ze_send_dev_fds(ptl, am_epaddr);
#endif
}

static psm2_error_t psm3_oneapi_ze_shm_dev_fds_connreq_poll(struct ptl_am *ptl, struct am_ptl_connection_req *req)
{
#ifndef PSM_HAVE_PIDFD
	return psm3_oneapi_ze_shm_ep_connreq_poll_dev_fds(ptl, req);
#else
	return PSM2_OK;
#endif
}

static psm2_error_t psm3_oneapi_ze_shm_dev_fds_check_exchanged(struct ptl_am *ptl, struct am_ptl_connection_req *req, int index)
{
#ifndef PSM_HAVE_PIDFD
	// late connect establish, check once to
	// see if have GPU dev fds, if not, this one
	// missed the timelimit and timesout
	if (req->op == AM_PTL_OP_CONNECT)
		_HFI_CONNDBG("late established, special GPU dev FDs poll\n");
	if (req->op == AM_PTL_OP_CONNECT &&
			PSM2_OK != psm3_oneapi_ze_check_dev_fds_exchanged(ptl, (struct am_epaddr *)(req->epaddr[index])))
		return PSM2_OK_NO_PROGRESS;
	else
#endif
		return PSM2_OK;
}

static psm2_error_t psm3_oneapi_ze_shm_dev_fds_poll(struct ptl_am *ptl, psm2_error_t res)
{
#ifndef PSM_HAVE_PIDFD
	// play err safe, callers ignore errors or expect just OK or NO_PROGRESS
	if (ptl->gpu_specific.ze_need_dev_fds_poll
			&& psm3_oneapi_ze_poll_dev_fds_exchange(ptl) != PSM2_OK_NO_PROGRESS)
		return PSM2_OK;
#endif
	return res;
}

// On Sender, place the IPC handle in the RTS
// We put offset in the basic "args" parameters and the actual
// IPC handle as payload due to it's size
// Callers expect payload_size >0 when using GPU IPC and key off non-zero
// payload size in RTS to identify a GPU IPC RTS
// Save in the req the needed information about IPC resources allocated here
// so psm3_oneapi_ze_process_cts and release them.
static psm2_error_t psm3_oneapi_ze_shm_build_rts(struct ptl_am *ptl,
			psm2_mq_req_t req, int *narg_p,
			psm2_amarg_t *args, void **payload_p, size_t *payload_size_p,
			union am_gpu_rts_payload *info)
{
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
#ifndef PSM_HAVE_PIDFD
	int fd;
	int *devfds;
	int numfds;
	int device_index = 0;
	struct drm_prime_handle open_fd = {0, 0, 0};
#endif
	uint64_t handle_fd = 0;
	size_t total;
	void *buf_base_ptr;
	uint64_t alloc_id;
	void *buf = req->req_data.buf;

#ifndef PSM_HAVE_PIDFD
	devfds = psm3_oneapi_ze_get_dev_fds(&numfds);
	device_index = psm3_oneapi_ze_cur_dev - psm3_oneapi_ze_devices; /* index (offset) in table */
	args[5].u32w0 = device_index;
	fd = devfds[device_index];
#endif
	PSM3_ONEAPI_ZE_CALL(zeMemGetAddressRange, psm3_oneapi_ze_context, buf, &buf_base_ptr, &total);

	/* Offset in GPU buffer from which we copy data, we have to
	 * send it separetly because this offset is lost
	 * when zeMemGetIpcHandle is called */
	req->gpu_specific.ze_ipc_offset = (uint32_t)((uintptr_t)buf - (uintptr_t)buf_base_ptr);
	args[2].u32w0 = (uint32_t)req->gpu_specific.ze_ipc_offset;
	alloc_id = psm3_oneapi_ze_get_alloc_id(buf_base_ptr, &info->ze.ze_alloc_type);
#ifndef PSM_HAVE_PIDFD
	args[5].u32w1 = (uint32_t)alloc_id; /* 32-bit for now  */
#else
	args[5].u64w0 = alloc_id;
#endif

	PSM3_ONEAPI_ZE_CALL(zeMemGetIpcHandle, psm3_oneapi_ze_context,
				(const void *)buf_base_ptr, &req->gpu_specific.ze_ipc_handle);
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	psm3_oneapi_ze_get_dmabuf_fd((const void *)buf, &handle_fd);
#else
	memcpy(&handle_fd, &req->gpu_specific.ze_ipc_handle, sizeof(uint32_t));
#endif
	req->gpu_specific.ze_handle_attached = 1;
#ifndef PSM_HAVE_PIDFD
	open_fd.fd = (uint32_t)handle_fd;
	if (ioctl(fd, DRM_IOCTL_PRIME_FD_TO_HANDLE, &open_fd) < 0) {
		_HFI_ERROR("ioctl failed for DRM_IOCTL_PRIME_FD_TO_HANDLE: for fd %d: %s", open_fd.fd, strerror(errno));
		psm3_handle_error(ptl->ep, PSM2_INTERNAL_ERR,
			"ioctl failed for DRM_IOCTL_PRIME_FD_TO_HANDLE for fd %d: errno=%d",
			open_fd.fd, errno);
		return PSM2_INTERNAL_ERR;
	}
	_HFI_VDBG("FD_TO_HANDLE: buf %p total 0x%lx base %p alloc_id %lu gem_handle %u\n",
			buf, total, buf_base_ptr, alloc_id, open_fd.handle);
	info->ze.ze_handle = open_fd.handle;
	*narg_p = 6;
	*payload_p = (void *)info;
	*payload_size_p = sizeof(struct am_oneapi_ze_rts_payload);
	// for DRM approach once we have the open_fd we could
	// PutIpcHandle(ipc_handle) since open_fd has a reference
	// however since that is a legacy mode, we focus on the
	// prefered mode and have both delay the Put until CTS received
#else
	info->ze.ze_handle = (uint32_t)handle_fd;
	*narg_p = 6;
	*payload_p = (void *)info;
	*payload_size_p = sizeof(struct am_oneapi_ze_rts_payload);
#endif /* PSM_HAVE_PIDFD */
	return PSM2_OK;
#else // if no drm, error out as oneapi ipc handles don't work without drm
	return PSM2_INTERNAL_ERR;
#endif // defined(HAVE_DRM) || defined(HAVE_LIBDRM)
}

// On receiver, pull IPC information out of the RTS which our peer build using
// psm3_oneapi_ze_shm_build_rts.  Information is saved to the req for subsequent
// processing after tag matching via psm3_oneapi_ze_shm_rtsmatch
static void psm3_oneapi_ze_shm_process_rts(psm2_mq_req_t req, void *buf,
				size_t len, int narg, psm2_amarg_t *args)
{
	struct am_oneapi_ze_rts_payload *info;

	psmi_assert(narg == 6);
	info = (struct am_oneapi_ze_rts_payload *)buf;
	psmi_assert(len == sizeof(struct am_oneapi_ze_rts_payload));
	req->gpu_specific.ze_handle = info->ze_handle;
	req->gpu_specific.ze_alloc_type = info->ze_alloc_type;
	req->gpu_specific.ze_handle_attached = 1;
	req->gpu_specific.ze_ipc_offset = args[2].u32w0;
#ifndef PSM_HAVE_PIDFD
	req->gpu_specific.ze_device_index = args[5].u32w0;
	req->gpu_specific.ze_alloc_id = args[5].u32w1;
#else
	req->gpu_specific.ze_alloc_id = args[5].u64w0;
#endif
}

// On receiver, use GPU IPC to copy data from the sender to this process
// This is called on the receiver after psm3_oneapi_ze_process_rts has parsed
// the incoming RTS and tag matching has matched the RTS with a receive buffer
// and populated the req with information about the matched receiver buffer
static int psm3_oneapi_ze_shm_rtsmatch(struct ptl_am *ptl, psm2_mq_req_t req)
{
	if (req->gpu_specific.ze_handle_attached) {
		void *buf_ptr = psm3_oneapi_ze_memhandle_acquire(
							ptl->memhandle_cache,
							req->rts_sbuf - req->gpu_specific.ze_ipc_offset, req->gpu_specific.ze_handle,
							req->rts_peer,
#ifndef PSM_HAVE_PIDFD
							req->gpu_specific.ze_device_index, req->gpu_specific.ze_alloc_id,
#else
							0, req->gpu_specific.ze_alloc_id,
#endif
							req->gpu_specific.ze_alloc_type);
		psmi_assert_always(buf_ptr != NULL);
		buf_ptr = (uint8_t *)buf_ptr + req->gpu_specific.ze_ipc_offset;
		/* zeMemcpy into the receive side buffer
		 * based on its location */
		_HFI_VDBG("Copying src %p (offset 0x%x) dst %p msg_len %u\n",
				buf_ptr, req->gpu_specific.ze_ipc_offset,
				req->req_data.buf, req->req_data.recv_msglen);
		if (req->is_buf_gpu_mem) {
			/*PSM3_GPU_MEMCPY_DTOD*/
			psm3_oneapi_ze_memcpy_DtoD(req->req_data.buf, buf_ptr,
					req->req_data.recv_msglen);
			// can skip sychronize, it's a noop for oneapi_ze
			//PSM3_GPU_SYNCHRONIZE_MEMCPY();
			//psm3_oneapi_ze_synchronize_memcpy();
		} else {
			/*PSM3_GPU_MEMCPY_DTOH*/
			psm3_oneapi_ze_memcpy_DtoH(req->req_data.buf, buf_ptr,
				req->req_data.recv_msglen);
		}
		psm3_oneapi_ze_memhandle_release(ptl->memhandle_cache,
					(uint8_t *)buf_ptr - req->gpu_specific.ze_ipc_offset);
		req->gpu_specific.ze_handle_attached = 0;
		return 1;
	}
	return 0;
}

// On sender, we have now received the CTS corresponding to an RTS
// we may have built in psm3_oneapi_ze_build_rts.  All we need to do here is release
// the resources we allocated in psm3_oneapi_ze_build_rts.  We saved the  necessary
// information tracking those resources in the send req.
// Returns:
// 	0 - the req was not for a GPU IO
// 	1 - the req was for a GPU IO and we have released the resources
static int psm3_oneapi_ze_shm_process_cts(psm2_mq_req_t req)
{
	if (req->gpu_specific.ze_handle_attached) {
		psm3_oneapi_ze_put_ipc_handle(req->req_data.buf - req->gpu_specific.ze_ipc_offset,
							req->gpu_specific.ze_ipc_handle);
		req->gpu_specific.ze_handle_attached = 0;
		return 1;
	}
	return 0;
}
// end of RTS and CTS processing functions for PSM3_DEVICES "shm"
//***************************************************************************

static psm2_error_t psm3_oneapi_ze_get_cuda_permitted(struct psm2_ep *ep, bool *enable)
{
	*enable = true;
	return PSM2_OK;
}

static psm2_error_t psm3_oneapi_ze_set_cuda_permitted(struct psm2_ep *ep, bool enable)
{
	return PSM2_OK;
}

static bool psm3_oneapi_ze_is_memcpy_permitted(struct psm2_ep *ep)
{
	return true;
}

struct psm3_gpu_hal psm3_oneapi_ze_hal = {
	.type = "oneapi-ze",
#ifdef PSM_HAVE_RNDV_MOD
#if defined(RV_GPU_ABI_VER_MINOR_0) && defined(RV_GPU_ABI_VER_MAJOR_1) && defined(RV_GPU_ABI_VER_MINOR_1)
		// RV GPU API <= 1.0 does not have track GPU alloc_id
		// RV GPU API <= 1.1 requires munmap_unpin
		// so if RV GPU API <= 1.1, do not allow GPUDirect
	.rv_major_rev_fail = RV_GPU_ABI_VER_MAJOR_1,
	.rv_minor_rev_fail = RV_GPU_ABI_VER_MINOR_1,
#else
	/* not defined if compile against older RV header */
#error "Intel GPU Support requires version 1.1 or newer rv_user_ioctls.h header"
#endif

	.rv_capability_expected = RV_CAP_INTEL_GPU,
	.hal_cap_expected = PSM_HAL_CAP_INTEL_GPU,
#endif /* PSM_HAVE_RNDV_MOD */

	.ghfp_initialize = psm3_oneapi_ze_initialize,
	.ghfp_finalize = psm3_oneapi_ze_finalize,
	.ghfp_ep_open = psm3_oneapi_ze_ep_open,
	.ghfp_ep_close = psm3_oneapi_ze_ep_close,
	.ghfp_identify = psm3_oneapi_ze_identify,
	.ghfp_verify_GPU_capabilities = psm3_oneapi_ze_verify_GPU_capabilities,
	.ghfp_p2p_supported = psm3_oneapi_ze_p2p_supported,
	.ghfp_gpudirect_supported = psm3_oneapi_ze_gpudirect_supported,
	.ghfp_using_rv_for_mrs = psm3_oneapi_ze_using_rv_for_mrs,
	.ghfp_get_pci_addr = psm3_oneapi_ze_get_pci_addr,
#ifdef PSM_HAVE_RNDV_MOD
	.ghfp_min_bar_size = psm3_oneapi_ze_min_bar_size,
	.ghfp_check_phys_addr = psm3_oneapi_ze_check_phys_addr,
	.ghfp_roundup_gdrcopy = psm3_oneapi_ze_roundup_gdrcopy,
#ifdef PSM_HAVE_REG_MR
	.ghfp_roundup_rv_reg_mr = psm3_oneapi_ze_roundup_rv_reg_mr,
	.ghfp_init_rv_reg_mr_params = psm3_oneapi_ze_init_rv_reg_mr_params,
#endif
	.ghfp_init_rv_pin_mmap_params = psm3_oneapi_ze_init_rv_pin_mmap_params,
	.ghfp_rv_reg_mmap_cleanup = psm3_oneapi_ze_rv_reg_mmap_cleanup,
#endif /* PSM_HAVE_RNDV_MOD */
#ifdef PSM_HAVE_REG_MR
	.ghfp_cmp_mr = psm3_oneapi_ze_cmp_mr,
	.ghfp_init_mr = psm3_oneapi_ze_init_mr,
#endif
	.ghfp_fetch_ctxt = psm3_oneapi_ze_fetch_ctxt,
	.ghfp_refresh_ctxt = psm3_oneapi_ze_refresh_ctxt,
	.ghfp_register_hostmem = psm3_oneapi_ze_register_hostmem,
	.ghfp_unregister_hostmem = psm3_oneapi_ze_unregister_hostmem,
	.ghfp_is_gpu_mem = psm3_oneapi_ze_is_gpu_mem,
	.ghfp_prepare_HtoD_memcpys = psm3_oneapi_ze_prepare_HtoD_memcpys,
	.ghfp_prepare_DtoH_memcpys = psm3_oneapi_ze_prepare_DtoH_memcpys,
	.ghfp_shutdown_HtoD_memcpys = psm3_oneapi_ze_shutdown_HtoD_memcpys,
	.ghfp_shutdown_DtoH_memcpys = psm3_oneapi_ze_shutdown_DtoH_memcpys,
	.ghfp_memcpy_HtoD_start = psm3_oneapi_ze_memcpy_HtoD_start,
	.ghfp_memcpy_DtoH_start = psm3_oneapi_ze_memcpy_DtoH_start,
	.ghfp_memcpy_done = psm3_oneapi_ze_memcpy_done,
	.ghfp_hostbuf_lazy_init = psm3_oneapi_ze_hostbuf_lazy_init,
	.ghfp_hostbuf_reset = psm3_oneapi_ze_hostbuf_reset,
	.ghfp_hostbuf_destroy = psm3_oneapi_ze_hostbuf_destroy,
	.ghfp_memcpy_DtoD = psm3_oneapi_ze_memcpy_DtoD,
	.ghfp_memcpy_HtoD = psm3_oneapi_ze_memcpy_HtoD,
	.ghfp_memcpy_DtoH = psm3_oneapi_ze_memcpy_DtoH,
	.ghfp_memcpy = psm3_oneapi_ze_memcpy,
	.ghfp_synchronize_memcpy = psm3_oneapi_ze_synchronize_memcpy,
	.ghfp_mark_buf_synchronous = psm3_oneapi_ze_mark_buf_synchronous,
	.ghfp_host_alloc = psm3_oneapi_ze_host_alloc,
	.ghfp_host_free = psm3_oneapi_ze_host_free,
	.ghfp_gpu_addr_send_mr = psm3_oneapi_ze_gpu_addr_send_mr,
	.ghfp_gpu_addr_recv_mr = psm3_oneapi_ze_gpu_addr_recv_mr,
	// functions for PSM3_DEVICES "shm" RTS/CTS processing to enable
	// use of GPU specific scale-up transfers within the given server
	.ghfp_shm_init = psm3_oneapi_ze_shm_init,
	.ghfp_shm_finalize = psm3_oneapi_ze_shm_finalize,
	.ghfp_shm_epaddr_add = psm3_oneapi_ze_shm_epaddr_add,
	.ghfp_shm_epaddr_free = psm3_oneapi_ze_shm_epaddr_free,
	.ghfp_shm_dev_fds_needed = psm3_oneapi_ze_shm_dev_fds_needed,
	.ghfp_shm_dev_fds_send = psm3_oneapi_ze_shm_dev_fds_send,
	.ghfp_shm_dev_fds_connreq_poll = psm3_oneapi_ze_shm_dev_fds_connreq_poll,
	.ghfp_shm_dev_fds_check_exchanged = psm3_oneapi_ze_shm_dev_fds_check_exchanged,
	.ghfp_shm_dev_fds_poll = psm3_oneapi_ze_shm_dev_fds_poll,
	.ghfp_shm_build_rts = psm3_oneapi_ze_shm_build_rts,
	.ghfp_shm_process_rts = psm3_oneapi_ze_shm_process_rts,
	.ghfp_shm_rtsmatch = psm3_oneapi_ze_shm_rtsmatch,
	.ghfp_shm_process_cts = psm3_oneapi_ze_shm_process_cts,
	.ghfp_get_cuda_permitted = psm3_oneapi_ze_get_cuda_permitted,
	.ghfp_set_cuda_permitted = psm3_oneapi_ze_set_cuda_permitted,
	.ghfp_is_memcpy_permitted = psm3_oneapi_ze_is_memcpy_permitted,
};

#endif /* PSM_ONEAPI */
