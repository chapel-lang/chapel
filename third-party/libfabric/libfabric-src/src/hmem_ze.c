/*
 * Copyright (c) 2020-2021 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "ofi_hmem.h"
#include "ofi.h"

#if HAVE_ZE

#include <stdio.h>
#include <dirent.h>
#include <level_zero/ze_api.h>

static ze_driver_handle_t driver;
static ze_context_handle_t context;
static ze_device_handle_t devices[ZE_MAX_DEVICES];
static ze_command_queue_handle_t cmd_queue[ZE_MAX_DEVICES];
static int num_devices = 0;
static int ordinals[ZE_MAX_DEVICES];
static int indices[ZE_MAX_DEVICES];
static int dev_fds[ZE_MAX_DEVICES];
static ze_device_uuid_t dev_uuids[ZE_MAX_DEVICES];
static bool p2p_enabled = false;
static bool host_reg_enabled = true;

static ze_command_queue_desc_t cq_desc = {
	.stype		= ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
	.pNext		= NULL,
	.ordinal	= 0,
	.index		= 0,
	.flags		= 0,
	.mode		= ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS,
	.priority	= ZE_COMMAND_QUEUE_PRIORITY_NORMAL,
};

static ze_command_list_desc_t cl_desc = {
	.stype				= ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
	.pNext				= NULL,
	.commandQueueGroupOrdinal	= 0,
	.flags				= 0,
};

struct libze_ops {
	ze_result_t (*zeInit)(ze_init_flags_t flags);
	ze_result_t (*zeDriverGet)(uint32_t *pCount,
				   ze_driver_handle_t *phDrivers);
	ze_result_t (*zeDeviceGet)(ze_driver_handle_t hDriver,
				   uint32_t *pCount,
				   ze_device_handle_t *phDevices);
	ze_result_t (*zeDeviceGetCommandQueueGroupProperties)(ze_device_handle_t hDevice,
			uint32_t *pCount,
			ze_command_queue_group_properties_t *pCommandQueueGroupProperties);
	ze_result_t (*zeDeviceCanAccessPeer)(ze_device_handle_t hDevice,
					     ze_device_handle_t hPeerDevice,
					     ze_bool_t *value);
	ze_result_t (*zeContextCreate)(ze_driver_handle_t hDriver,
				       const ze_context_desc_t *desc,
				       ze_context_handle_t *phContext);
	ze_result_t (*zeContextDestroy)(ze_context_handle_t hContext);
	ze_result_t (*zeCommandQueueCreate)(ze_context_handle_t hContext,
					    ze_device_handle_t hDevice,
					    const ze_command_queue_desc_t *desc,
					    ze_command_queue_handle_t *phCommandQueue);
	ze_result_t (*zeCommandQueueDestroy)(ze_command_queue_handle_t hCommandQueue);
	ze_result_t (*zeCommandQueueExecuteCommandLists)(
					ze_command_queue_handle_t hCommandQueue,
					uint32_t numCommandLists,
					ze_command_list_handle_t *phCommandLists,
					ze_fence_handle_t hFence);
	ze_result_t (*zeCommandListCreate)(ze_context_handle_t hContext,
					   ze_device_handle_t hDevice,
					   const ze_command_list_desc_t *desc,
					   ze_command_list_handle_t *phCommandList);
	ze_result_t (*zeCommandListDestroy)(ze_command_list_handle_t hCommandList);
	ze_result_t (*zeCommandListClose)(ze_command_list_handle_t hCommandList);
	ze_result_t (*zeCommandListAppendMemoryCopy)(
				ze_command_list_handle_t hCommandList,
				void *dstptr, const void *srcptr, size_t size,
				ze_event_handle_t hSignalEvent,
				uint32_t numWaitEvents,
				ze_event_handle_t *phWaitEvents);
	ze_result_t (*zeMemGetAllocProperties)(
				ze_context_handle_t hContext, const void *ptr,
				ze_memory_allocation_properties_t *pMemAllocProperties,
				ze_device_handle_t *phDevice);
	ze_result_t (*zeMemGetAddressRange)(
				ze_context_handle_t hContext, const void *ptr,
				void **pBase, size_t *pSize);
	ze_result_t (*zeMemGetIpcHandle)(ze_context_handle_t hContext,
					 const void *ptr,
					 ze_ipc_mem_handle_t *pIpcHandle);
	ze_result_t (*zeMemOpenIpcHandle)(ze_context_handle_t hContext,
					  ze_device_handle_t hDevice,
					  ze_ipc_mem_handle_t handle,
					  ze_ipc_memory_flags_t flags,
					  void **pptr);
	ze_result_t (*zeMemCloseIpcHandle)(ze_context_handle_t hContext,
					   const void *ptr);
	ze_result_t (*zeDeviceGetProperties)(ze_device_handle_t hDevice,
					     ze_device_properties_t *pDeviceProperties);
	ze_result_t (*zeDriverGetExtensionFunctionAddress)(
				ze_driver_handle_t hDriver, const char *name,
				void **ppFunctionAddress);
	ze_result_t (*zexDriverImportExternalPointer)(ze_driver_handle_t hDriver,
						      void *ptr, size_t len);
	ze_result_t (*zexDriverReleaseImportedPointer)(ze_driver_handle_t hDriver,
						       void *ptr);
};

#if ENABLE_ZE_DLOPEN

#include <dlfcn.h>

static void *libze_handle;
static struct libze_ops libze_ops;

#else

static struct libze_ops libze_ops = {
	.zeInit = zeInit,
	.zeDriverGet = zeDriverGet,
	.zeDeviceGet = zeDeviceGet,
	.zeDeviceGetCommandQueueGroupProperties = zeDeviceGetCommandQueueGroupProperties,
	.zeDeviceCanAccessPeer = zeDeviceCanAccessPeer,
	.zeContextCreate = zeContextCreate,
	.zeContextDestroy = zeContextDestroy,
	.zeCommandQueueCreate = zeCommandQueueCreate,
	.zeCommandQueueDestroy = zeCommandQueueDestroy,
	.zeCommandQueueExecuteCommandLists = zeCommandQueueExecuteCommandLists,
	.zeCommandListCreate = zeCommandListCreate,
	.zeCommandListDestroy = zeCommandListDestroy,
	.zeCommandListClose = zeCommandListClose,
	.zeCommandListAppendMemoryCopy = zeCommandListAppendMemoryCopy,
	.zeMemGetAllocProperties = zeMemGetAllocProperties,
	.zeMemGetAddressRange = zeMemGetAddressRange,
	.zeMemGetIpcHandle = zeMemGetIpcHandle,
	.zeMemOpenIpcHandle = zeMemOpenIpcHandle,
	.zeMemCloseIpcHandle = zeMemCloseIpcHandle,
	.zeDeviceGetProperties = zeDeviceGetProperties,
	.zeDriverGetExtensionFunctionAddress = zeDriverGetExtensionFunctionAddress,
};

#endif /* ENABLE_ZE_DLOPEN */

ze_result_t ofi_zeInit(ze_init_flags_t flags)
{
	return (*libze_ops.zeInit)(flags);
}

ze_result_t ofi_zeDriverGet(uint32_t *pCount, ze_driver_handle_t *phDrivers)
{
	return (*libze_ops.zeDriverGet)(pCount, phDrivers);
}

ze_result_t ofi_zeDeviceGet(ze_driver_handle_t hDriver, uint32_t *pCount,
			    ze_device_handle_t *phDevices)
{
	return (*libze_ops.zeDeviceGet)(hDriver, pCount, phDevices);
}

ze_result_t ofi_zeDeviceGetCommandQueueGroupProperties(ze_device_handle_t hDevice,
	       uint32_t *pCount,
	       ze_command_queue_group_properties_t *pCommandQueueGroupProperties)
{
	return (*libze_ops.zeDeviceGetCommandQueueGroupProperties)(hDevice,
					pCount, pCommandQueueGroupProperties);
}

ze_result_t ofi_zeDeviceCanAccessPeer(ze_device_handle_t hDevice,
				      ze_device_handle_t hPeerDevice,
				      ze_bool_t *value)
{
	return (*libze_ops.zeDeviceCanAccessPeer)(hDevice, hPeerDevice, value);
}

ze_result_t ofi_zeContextCreate(ze_driver_handle_t hDriver,
				const ze_context_desc_t *desc,
				ze_context_handle_t *phContext)
{
	return (*libze_ops.zeContextCreate)(hDriver, desc, phContext);
}

ze_result_t ofi_zeContextDestroy(ze_context_handle_t hContext)
{
	return (*libze_ops.zeContextDestroy)(hContext);
}

ze_result_t ofi_zeCommandQueueCreate(ze_context_handle_t hContext,
				     ze_device_handle_t hDevice,
				     const ze_command_queue_desc_t *desc,
				     ze_command_queue_handle_t *phCommandQueue)
{
	return (*libze_ops.zeCommandQueueCreate)(hContext, hDevice, desc,
						 phCommandQueue);
}

ze_result_t ofi_zeCommandQueueDestroy(ze_command_queue_handle_t hCommandQueue)
{
	return (*libze_ops.zeCommandQueueDestroy)(hCommandQueue);
}

ze_result_t ofi_zeCommandQueueExecuteCommandLists(
				ze_command_queue_handle_t hCommandQueue,
				uint32_t numCommandLists,
				ze_command_list_handle_t *phCommandLists,
				ze_fence_handle_t hFence)
{
	return (*libze_ops.zeCommandQueueExecuteCommandLists)(
				hCommandQueue, numCommandLists, phCommandLists,
				hFence);
}

ze_result_t ofi_zeCommandListCreate(ze_context_handle_t hContext,
				    ze_device_handle_t hDevice,
				    const ze_command_list_desc_t *desc,
				    ze_command_list_handle_t *phCommandList)
{
	return (*libze_ops.zeCommandListCreate)(hContext, hDevice, desc,
						phCommandList);
}

ze_result_t ofi_zeCommandListDestroy(ze_command_list_handle_t hCommandList)
{
	return (*libze_ops.zeCommandListDestroy)(hCommandList);
}

ze_result_t ofi_zeCommandListClose(ze_command_list_handle_t hCommandList)
{
	return (*libze_ops.zeCommandListClose)(hCommandList);
}

ze_result_t ofi_zeCommandListAppendMemoryCopy(
				ze_command_list_handle_t hCommandList,
				void *dstptr, const void *srcptr, size_t size,
				ze_event_handle_t hSignalEvent,
				uint32_t numWaitEvents,
				ze_event_handle_t *phWaitEvents)
{
	return (*libze_ops.zeCommandListAppendMemoryCopy)(
				hCommandList, dstptr, srcptr, size, hSignalEvent,
				numWaitEvents, phWaitEvents);
}

ze_result_t ofi_zeMemGetAllocProperties(ze_context_handle_t hContext,
					const void *ptr,
					ze_memory_allocation_properties_t
						*pMemAllocProperties,
					ze_device_handle_t *phDevice)
{
	return (*libze_ops.zeMemGetAllocProperties)(
					hContext, ptr, pMemAllocProperties,
					phDevice);
}

ze_result_t ofi_zeMemGetAddressRange(ze_context_handle_t hContext,
				     const void *ptr, void **pBase,
				     size_t *pSize)
{
	return (*libze_ops.zeMemGetAddressRange)(hContext, ptr, pBase, pSize);
}

ze_result_t ofi_zeMemGetIpcHandle(ze_context_handle_t hContext, const void *ptr,
				  ze_ipc_mem_handle_t *pIpcHandle)
{
	return (*libze_ops.zeMemGetIpcHandle)(hContext, ptr, pIpcHandle);
}

ze_result_t ofi_zeMemOpenIpcHandle(ze_context_handle_t hContext,
				   ze_device_handle_t hDevice,
				   ze_ipc_mem_handle_t handle,
				   ze_ipc_memory_flags_t flags,
				   void **pptr)
{
	return (*libze_ops.zeMemOpenIpcHandle)(hContext, hDevice, handle, flags,
					       pptr);
}

ze_result_t ofi_zeMemCloseIpcHandle(ze_context_handle_t hContext,
				    const void *ptr)
{
	return (*libze_ops.zeMemCloseIpcHandle)(hContext, ptr);
}

ze_result_t ofi_zeDeviceGetProperties(ze_device_handle_t hDevice,
				      ze_device_properties_t *pDeviceProperties)
{
	return (*libze_ops.zeDeviceGetProperties)(hDevice, pDeviceProperties);
}

ze_result_t ofi_zeDriverGetExtensionFunctionAddress(ze_driver_handle_t hDriver,
						    const char *name,
						    void **ppFuncAddress)
{
	return (*libze_ops.zeDriverGetExtensionFunctionAddress)(hDriver, name,
							        ppFuncAddress);
}

ze_result_t ofi_zexDriverImportExternalPointer(ze_driver_handle_t hDriver,
					       void *ptr, size_t len)
{
	return (*libze_ops.zexDriverImportExternalPointer)(hDriver, ptr, len);
}

ze_result_t ofi_zexDriverReleaseImportedPointer(ze_driver_handle_t hDriver,
						void *ptr)
{
	return (*libze_ops.zexDriverReleaseImportedPointer)(hDriver, ptr);
}

#if HAVE_DRM || HAVE_LIBDRM

#if HAVE_DRM
#include <drm/i915_drm.h>
#else
#include <libdrm/i915_drm.h>
#endif

#include <sys/ioctl.h>
#include <stdio.h>

static int ze_hmem_init_fds(void)
{
	const char *dev_dir = "/dev/dri/by-path/";
	const char *suffix = "-render";
	DIR *dir;
	struct dirent *ent = NULL;
	char dev_name[NAME_MAX];
	int i = 0, ret;

	dir = opendir(dev_dir);
	if (dir == NULL)
		return -FI_EIO;

	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.' ||
		    strstr(ent->d_name, suffix) == NULL)
			continue;

		memset(dev_name, 0, sizeof(dev_name));
		ret = snprintf(dev_name, NAME_MAX, "%s%s", dev_dir, ent->d_name);
		if (ret < 0 || ret >= NAME_MAX)
			goto err;

		dev_fds[i] = open(dev_name, O_RDWR);
		if (dev_fds[i] == -1)
			goto err;
		i++;
	}
	(void) closedir(dir);
	return FI_SUCCESS;

err:
	(void) closedir(dir);
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed open device %d\n", i);
	return -FI_EIO;
}

int ze_hmem_get_shared_handle(int dev_fd, void *dev_buf, int *ze_fd,
			      void **handle)
{
	struct drm_prime_handle open_fd = {0, 0, 0};
	ze_ipc_mem_handle_t ze_handle;
	int ret;

	assert(dev_fd != -1);
	ret = ze_hmem_get_handle(dev_buf, (void **) &ze_handle);
	if (ret)
		return ret;

	memcpy(ze_fd, &ze_handle, sizeof(*ze_fd));
	memcpy(&open_fd.fd, &ze_handle, sizeof(open_fd.fd));
	ret = ioctl(dev_fd, DRM_IOCTL_PRIME_FD_TO_HANDLE, &open_fd);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"ioctl call failed on get, err %d\n", errno);
		return -FI_EINVAL;
	}

	*(int *) handle = open_fd.handle;
	return FI_SUCCESS;
}

int ze_hmem_open_shared_handle(int dev_fd, void **handle, int *ze_fd,
			       uint64_t device, void **ipc_ptr)
{
	struct drm_prime_handle open_fd = {0, 0, 0};
	ze_ipc_mem_handle_t ze_handle;
	int ret;

	open_fd.flags = DRM_CLOEXEC | DRM_RDWR;
	open_fd.handle = *(int *) handle;

	ret = ioctl(dev_fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &open_fd);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"ioctl call failed on open, err %d\n", errno);
		return -FI_EINVAL;
	}

	*ze_fd = open_fd.fd;
	memset(&ze_handle, 0, sizeof(ze_handle));
	memcpy(&ze_handle, &open_fd.fd, sizeof(open_fd.fd));
	return ze_hmem_open_handle((void **) &ze_handle, device, ipc_ptr);
}

bool ze_hmem_p2p_enabled(void)
{
	return !ofi_hmem_p2p_disabled() && p2p_enabled;
}

#else

static int ze_hmem_init_fds(void)
{
	return FI_SUCCESS;
}

int ze_hmem_get_shared_handle(int dev_fd, void *dev_buf, int *ze_fd,
			      void **handle)
{
	return -FI_ENOSYS;
}
int ze_hmem_open_shared_handle(int dev_fd, void **handle, int *ze_fd,
			       uint64_t device, void **ipc_ptr)
{
	return -FI_ENOSYS;
}

bool ze_hmem_p2p_enabled(void)
{
	return false;
}

#endif //HAVE_DRM || HAVE_LIBDRM

static int ze_hmem_dl_init(void)
{
#if ENABLE_ZE_DLOPEN
	libze_handle = dlopen("libze_loader.so", RTLD_NOW);
	if (!libze_handle) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libze_loader.so\n");
		goto err_out;
	}

	libze_ops.zeInit = dlsym(libze_handle, "zeInit");
	if (!libze_ops.zeInit) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeInit\n");
		goto err_dlclose;
	}

	libze_ops.zeDriverGet = dlsym(libze_handle, "zeDriverGet");
	if (!libze_ops.zeDriverGet) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeDriverGet\n");
		goto err_dlclose;
	}

	libze_ops.zeDeviceGet = dlsym(libze_handle, "zeDeviceGet");
	if (!libze_ops.zeDeviceGet) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeDeviceGet\n");
		goto err_dlclose;
	}

	libze_ops.zeDeviceGetCommandQueueGroupProperties = dlsym(libze_handle,
				"zeDeviceGetCommandQueueGroupProperties");
	if (!libze_ops.zeDeviceGetCommandQueueGroupProperties) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find zeDeviceGetCommandQueueGroupProperties\n");
		goto err_dlclose;
	}

	libze_ops.zeDeviceCanAccessPeer = dlsym(libze_handle, "zeDeviceCanAccessPeer");
	if (!libze_ops.zeDeviceCanAccessPeer) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeDeviceCanAccessPeer\n");
		goto err_dlclose;
	}

	libze_ops.zeContextCreate = dlsym(libze_handle, "zeContextCreate");
	if (!libze_ops.zeContextCreate) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeContextCreate\n");
		goto err_dlclose;
	}

	libze_ops.zeContextDestroy = dlsym(libze_handle, "zeContextDestroy");
	if (!libze_ops.zeContextDestroy) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeContextDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandQueueCreate = dlsym(libze_handle, "zeCommandQueueCreate");
	if (!libze_ops.zeCommandQueueCreate) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandQueueCreate\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandQueueDestroy = dlsym(libze_handle, "zeCommandQueueDestroy");
	if (!libze_ops.zeCommandQueueDestroy) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandQueueDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandQueueExecuteCommandLists = dlsym(libze_handle, "zeCommandQueueExecuteCommandLists");
	if (!libze_ops.zeCommandQueueExecuteCommandLists) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandQueueExecuteCommandLists\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListCreate = dlsym(libze_handle, "zeCommandListCreate");
	if (!libze_ops.zeCommandListCreate) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandListCreate\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListDestroy = dlsym(libze_handle, "zeCommandListDestroy");
	if (!libze_ops.zeCommandListDestroy) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandListDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListClose = dlsym(libze_handle, "zeCommandListClose");
	if (!libze_ops.zeCommandListClose) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandListClose\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListAppendMemoryCopy = dlsym(libze_handle, "zeCommandListAppendMemoryCopy");
	if (!libze_ops.zeCommandListAppendMemoryCopy) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeCommandListAppendMemoryCopy\n");
		goto err_dlclose;
	}

	libze_ops.zeMemGetAllocProperties = dlsym(libze_handle, "zeMemGetAllocProperties");
	if (!libze_ops.zeMemGetAllocProperties) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeMemGetAllocProperties\n");
		goto err_dlclose;
	}

	libze_ops.zeMemGetAddressRange = dlsym(libze_handle, "zeMemGetAddressRange");
	if (!libze_ops.zeMemGetAddressRange) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeMemGetAddressRange\n");
		goto err_dlclose;
	}

	libze_ops.zeMemGetIpcHandle = dlsym(libze_handle, "zeMemGetIpcHandle");
	if (!libze_ops.zeMemGetIpcHandle) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeMemGetIpcHandle\n");
		goto err_dlclose;
	}

	libze_ops.zeMemOpenIpcHandle = dlsym(libze_handle, "zeMemOpenIpcHandle");
	if (!libze_ops.zeMemOpenIpcHandle) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeMemOpenIpcHandle\n");
		goto err_dlclose;
	}

	libze_ops.zeMemCloseIpcHandle = dlsym(libze_handle, "zeMemCloseIpcHandle");
	if (!libze_ops.zeMemCloseIpcHandle) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeMemCloseIpcHandle\n");
		goto err_dlclose;
	}

	libze_ops.zeDeviceGetProperties = dlsym(libze_handle, "zeDeviceGetProperties");
	if (!libze_ops.zeDeviceGetProperties) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find zeDeviceGetProperties\n");
		goto err_dlclose;
	}

	libze_ops.zeDriverGetExtensionFunctionAddress =
		dlsym(libze_handle, "zeDriverGetExtensionFunctionAddress");
	if (!libze_ops.zeDriverGetExtensionFunctionAddress) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find zeDriverGetExtensionFunctionAddress\n");
		goto err_dlclose;
	}

	return FI_SUCCESS;

err_dlclose:
	dlclose(libze_handle);

err_out:
	return -FI_ENODATA;

#else
	return FI_SUCCESS;
#endif /* ENABLE_ZE_DLOPEN */
}

static void ze_hmem_dl_cleanup(void)
{
#if ENABLE_ZE_DLOPEN
	dlclose(libze_handle);
#endif
}

static int ze_hmem_find_copy_only_engine(int device_num, int *ordinal, int *index)
{
	ze_result_t ze_ret;
	uint32_t cq_grp_count = 0;
	ze_command_queue_group_properties_t *cq_grp_props = NULL;
	int i = 0, j = 0;

	ze_ret = ofi_zeDeviceGetCommandQueueGroupProperties(devices[device_num],
							    &cq_grp_count, NULL);
	if (ze_ret)
		goto out;

	cq_grp_props = calloc(cq_grp_count, sizeof(*cq_grp_props));

	ze_ret = ofi_zeDeviceGetCommandQueueGroupProperties(devices[device_num],
							    &cq_grp_count,
							    cq_grp_props);
	if (ze_ret)
		goto out;

	i = *ordinal;
	j = *index;

	/* Use user specified engine group if it is valid */
	if (i != -1 && i >= 0 && i < cq_grp_count) {
		if (j < 0 || j >= cq_grp_props[i].numQueues) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"ZE device %d: invalid engine index %d for "
				"group %d, use default.\n", device_num, j, i);
			j = 0;
		}
		goto out;
	}

	if (i != -1)
		FI_WARN(&core_prov, FI_LOG_CORE,
			"ZE device %d: invalid engine group %d, use default.\n",
			device_num, i);

	/* Auto select the first copy-only engine group if possible */
	j = 0;
	for (i = 0; i < cq_grp_count; i++) {
		if (cq_grp_props[i].flags &
		    ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY &&
		    !(cq_grp_props[i].flags &
		      ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE)) {
			break;
		}
	}

out:
	free(cq_grp_props);
	*ordinal = i == cq_grp_count ? 0 : i;
	*index = j;
	return ze_ret;
}

/*
 * Some L0 calls may segfault when called from a "destructor" if any
 * HMEM-capable DL provider is enabled due to premature unloading of GPU
 * specific L0 library. Before a permanent fix is available, don't make
 * L0 calls insider a "destructor".
 */
static int ze_hmem_cleanup_internal(int fini_workaround)
{
	int i, ret = FI_SUCCESS;

	for (i = 0; i < num_devices; i++) {
		if (!fini_workaround) {
			if (cmd_queue[i] &&
			    ofi_zeCommandQueueDestroy(cmd_queue[i])) {
				FI_WARN(&core_prov, FI_LOG_CORE,
					"Failed to destroy ZE cmd_queue\n");
				ret = -FI_EINVAL;
			}
		}
		if (dev_fds[i] != -1) {
			close(dev_fds[i]);
			dev_fds[i] = -1;
		}
	}

	if (!fini_workaround) {
		if (ofi_zeContextDestroy(context))
			ret = -FI_EINVAL;
	}

	ze_hmem_dl_cleanup();
	return ret;
}

int ze_hmem_cleanup(void)
{
	return ze_hmem_cleanup_internal(1);
}

int ze_hmem_init(void)
{
	ze_context_desc_t context_desc = {0};
	ze_device_properties_t dev_prop = {0};
	ze_result_t ze_ret;
	ze_bool_t access;
	uint32_t count, i;
	bool p2p = true;
	int ret;
	char *enginestr = NULL;
	int ordinal = -1;
	int index = 0;

	fi_param_define(NULL, "hmem_ze_copy_engine", FI_PARAM_STRING,
                        "Specify GPU engine used for copy operation: <group>, "
			"<group>.<index> (default: 1st copy-only engine)");
        fi_param_get_str(NULL, "log_level", &enginestr);
	if (enginestr)
		sscanf(enginestr, "%d.%d", &ordinal, &index);

	ret = ze_hmem_dl_init();
	if (ret)
		return ret;

	ze_ret = ofi_zeInit(ZE_INIT_FLAG_GPU_ONLY);
	if (ze_ret)
		return -FI_EIO;

	count = 1;
	ze_ret = ofi_zeDriverGet(&count, &driver);
	if (ze_ret)
		return -FI_EIO;

	ze_ret = ofi_zeDriverGetExtensionFunctionAddress(
			driver, "zexDriverImportExternalPointer",
			(void *)&libze_ops.zexDriverImportExternalPointer);
	if (ze_ret)
		host_reg_enabled = false;

	ze_ret = ofi_zeDriverGetExtensionFunctionAddress(
			driver, "zexDriverReleaseImportedPointer",
			(void *)&libze_ops.zexDriverReleaseImportedPointer);
	if (ze_ret)
		host_reg_enabled = false;

	ze_ret = ofi_zeContextCreate(driver, &context_desc, &context);
	if (ze_ret)
		return -FI_EIO;

	for (i = 0; i < ZE_MAX_DEVICES; dev_fds[i++] = -1)
		;

	count = 0;
	ze_ret = ofi_zeDeviceGet(driver, &count, NULL);
	if (ze_ret || count > ZE_MAX_DEVICES)
		goto err;

	ze_ret = ofi_zeDeviceGet(driver, &count, devices);
	if (ze_ret)
		goto err;

	ret = ze_hmem_init_fds();
	if (ret)
		goto err;

	for (num_devices = 0; num_devices < count; num_devices++) {
		ze_ret = ofi_zeDeviceGetProperties(devices[num_devices],
						   &dev_prop);
		if (ze_ret)
			goto err;

		memcpy(&dev_uuids[num_devices], &dev_prop.uuid,
		       sizeof(*dev_uuids));

		ordinals[num_devices] = ordinal;
		indices[num_devices] = index;
		ze_ret = ze_hmem_find_copy_only_engine(num_devices,
						       &ordinals[num_devices],
						       &indices[num_devices]);
		if (ze_ret)
			goto err;

		for (i = 0; i < count; i++) {
			if (ofi_zeDeviceCanAccessPeer(devices[num_devices],
					devices[i], &access) || !access)
				p2p = false;
		}
	}

	p2p_enabled = p2p;
	return FI_SUCCESS;

err:
	(void) ze_hmem_cleanup_internal(0);
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to initialize ZE driver resources\n");

	return -FI_EIO;
}

int ze_hmem_copy(uint64_t device, void *dst, const void *src, size_t size)
{
	ze_command_list_handle_t cmd_list;
	ze_result_t ze_ret;
	int dev_id = (int) device;

	/* Host memory allocated via ZE */
	if (dev_id < 0) {
		memcpy(dst, src, size);
		return 0;
	}

	if (!cmd_queue[device]) {
		cq_desc.ordinal = ordinals[device];
		cq_desc.index = indices[device];
		ze_ret = ofi_zeCommandQueueCreate(context,
						  devices[device],
						  &cq_desc,
						  &cmd_queue[device]);
		if (ze_ret)
			goto err;
	}

	cl_desc.commandQueueGroupOrdinal = ordinals[dev_id];
	ze_ret = ofi_zeCommandListCreate(context, devices[dev_id], &cl_desc,
					 &cmd_list);
	if (ze_ret)
		goto err;

	ze_ret = ofi_zeCommandListAppendMemoryCopy(cmd_list, dst, src, size,
						   NULL, 0, NULL);
	if (ze_ret)
		goto free;

	ze_ret = ofi_zeCommandListClose(cmd_list);
	if (ze_ret)
		goto free;

	ze_ret = ofi_zeCommandQueueExecuteCommandLists(cmd_queue[dev_id], 1,
						       &cmd_list, NULL);

free:
	if (!ofi_zeCommandListDestroy(cmd_list) && !ze_ret)
		return FI_SUCCESS;
err:
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform ze copy (%d)\n", ze_ret);

	return -FI_EIO;
}

bool ze_hmem_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags)
{
	ze_result_t ze_ret;
	ze_memory_allocation_properties_t mem_props = {0};
	ze_device_properties_t dev_prop = {0};
	ze_device_handle_t device_ptr;
	int i;

	ze_ret = ofi_zeMemGetAllocProperties(context, addr, &mem_props,
					     &device_ptr);
	if (ze_ret)
		return false;

	if (flags)
		*flags = mem_props.type == ZE_MEMORY_TYPE_DEVICE ?
			 FI_HMEM_DEVICE_ONLY : 0;

	if (!device)
		return true;

	ze_ret = ofi_zeDeviceGetProperties(device_ptr, &dev_prop);
	if (ze_ret)
		return false;

	for (i = 0, *device = 0; i < num_devices; i++) {
		if (!memcmp(&dev_prop.uuid, &dev_uuids[i],
			    sizeof(*dev_uuids))) {
			*device = i;
			return true;
		}
	}

	assert(1);
	return true;
}

int ze_hmem_get_handle(void *dev_buf, void **handle)
{
	ze_result_t ze_ret;

	ze_ret = ofi_zeMemGetIpcHandle(context, dev_buf,
				       (ze_ipc_mem_handle_t *) handle);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Unable to get handle\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

int ze_hmem_open_handle(void **handle, uint64_t device, void **ipc_ptr)
{
	ze_result_t ze_ret;
	int dev_id = (int) device;

	/* only device memory is supported */
	assert(dev_id >= 0);

	ze_ret = ofi_zeMemOpenIpcHandle(context, devices[dev_id],
					*((ze_ipc_mem_handle_t *) handle),
					0, ipc_ptr);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unable to open memory handle\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

int ze_hmem_close_handle(void *ipc_ptr)
{
	ze_result_t ze_ret;

	ze_ret = ofi_zeMemCloseIpcHandle(context, ipc_ptr);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unable to close memory handle\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

int ze_hmem_get_ipc_handle_size(size_t *size)
{
	*size = sizeof(ze_ipc_mem_handle_t);
	return FI_SUCCESS;
}

int ze_hmem_get_base_addr(const void *ptr, void **base, size_t *size)
{
	ze_result_t ze_ret;

	ze_ret = ofi_zeMemGetAddressRange(context, ptr, base, size);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Could not get base addr\n");
		return -FI_EINVAL;
	}
	return FI_SUCCESS;
}

int ze_hmem_get_id(const void *ptr, uint64_t *id)
{
	ze_result_t ze_ret;
	ze_memory_allocation_properties_t mem_props;
	ze_device_handle_t device;

	mem_props.stype = ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES;
	mem_props.pNext = NULL;
	ze_ret = ofi_zeMemGetAllocProperties(context, ptr, &mem_props, &device);
	if (ze_ret || mem_props.type == ZE_MEMORY_TYPE_UNKNOWN) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Could not get memory id\n");
		return -FI_EINVAL;
	}

	*id = mem_props.id;
	return FI_SUCCESS;
}

int *ze_hmem_get_dev_fds(int *nfds)
{
	*nfds = num_devices;
	return dev_fds;
}

int ze_hmem_host_register(void *ptr, size_t size)
{
	ze_result_t ze_ret;

	if (host_reg_enabled) {
		ze_ret = ofi_zexDriverImportExternalPointer(driver, ptr, size);
		if (ze_ret) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Failed to import host memory: ptr %p size %zd",
				ptr, size);
		}
	}
	return FI_SUCCESS;
}

int ze_hmem_host_unregister(void *ptr)
{
	ze_result_t ze_ret;

	if (host_reg_enabled) {
		ze_ret = ofi_zexDriverReleaseImportedPointer(driver, ptr);
		if (ze_ret) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Failed to release imported memory: ptr %p", ptr);
		}
	}
	return FI_SUCCESS;
}

#else

int ze_hmem_init(void)
{
	return -FI_ENOSYS;
}

int ze_hmem_cleanup(void)
{
	return -FI_ENOSYS;
}

int ze_hmem_copy(uint64_t device, void *dst, const void *src, size_t size)
{
	return -FI_ENOSYS;
}

bool ze_hmem_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags)
{
	return false;
}

int ze_hmem_get_handle(void *dev_buf, void **handle)
{
	return -FI_ENOSYS;
}

int ze_hmem_open_handle(void **handle, uint64_t device, void **ipc_ptr)
{
	return -FI_ENOSYS;
}

int ze_hmem_get_shared_handle(int dev_fd, void *dev_buf, int *ze_fd,
			      void **handle)
{
	return -FI_ENOSYS;
}

int ze_hmem_open_shared_handle(int dev_fd, void **handle, int *ze_fd,
			       uint64_t device, void **ipc_ptr)
{
	return -FI_ENOSYS;
}

int ze_hmem_close_handle(void *ipc_ptr)
{
	return -FI_ENOSYS;
}

bool ze_hmem_p2p_enabled(void)
{
	return false;
}

int ze_hmem_get_ipc_handle_size(size_t *size)
{
	return -FI_ENOSYS;
}

int ze_hmem_get_base_addr(const void *ptr, void **base, size_t *size)
{
	return -FI_ENOSYS;
}

int ze_hmem_get_id(const void *ptr, uint64_t *id)
{
	return -FI_ENOSYS;
}

int *ze_hmem_get_dev_fds(int *nfds)
{
	*nfds = 0;
	return NULL;
}

int ze_hmem_host_register(void *ptr, size_t size)
{
	return FI_SUCCESS;
}

int ze_hmem_host_unregister(void *ptr)
{
	return FI_SUCCESS;
}

#endif /* HAVE_ZE */
