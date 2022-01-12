/*
 * Copyright (c) 2020 Intel Corporation. All rights reserved.
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

#include <stdio.h>
#include "hmem.h"
#include "shared.h"

#if HAVE_LIBZE

#include <dlfcn.h>
#include <level_zero/ze_api.h>

#define ZE_MAX_DEVICES 4

static ze_context_handle_t context;
static ze_device_handle_t devices[ZE_MAX_DEVICES];
static ze_command_queue_handle_t cmd_queue[ZE_MAX_DEVICES];
static int num_devices = 0;

static const ze_command_queue_desc_t cq_desc = {
	.stype		= ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
	.pNext		= NULL,
	.ordinal	= 0,
	.index		= 0,
	.flags		= 0,
	.mode		= ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS,
	.priority	= ZE_COMMAND_QUEUE_PRIORITY_NORMAL,
};

static const ze_command_list_desc_t cl_desc = {
	.stype				= ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
	.pNext				= NULL,
	.commandQueueGroupOrdinal	= 0,
	.flags				= 0,
};

static const ze_device_mem_alloc_desc_t device_desc = {
	.stype		= ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC,
	.pNext		= NULL,
	.flags		= 0,
	.ordinal	= 0,
};

static void *libze_handle;
static struct libze_ops {
	ze_result_t (*zeInit)(ze_init_flags_t flags);
	ze_result_t (*zeDriverGet)(uint32_t *pCount,
				   ze_driver_handle_t *phDrivers);
	ze_result_t (*zeDeviceGet)(ze_driver_handle_t hDriver,
				   uint32_t *pCount,
				   ze_device_handle_t *phDevices);
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
	ze_result_t (*zeCommandListAppendMemoryFill)(
				ze_command_list_handle_t hCommandList,
				void *ptr, const void *pattern,
				size_t pattern_size, size_t size,
				ze_event_handle_t hSignalEvent,
				uint32_t numWaitEvents,
				ze_event_handle_t *phWaitEvents);
	ze_result_t (*zeMemAllocDevice)(
				ze_context_handle_t hContext,
				const ze_device_mem_alloc_desc_t *device_desc,
				size_t size, size_t alignment, ze_device_handle_t hDevice,
				void *pptr);
	ze_result_t (*zeMemFree)(ze_context_handle_t hContext, void *ptr);
} libze_ops;

static int init_libze_ops(void)
{
	libze_handle = dlopen("libze_loader.so", RTLD_NOW);
	if (!libze_handle) {
		FT_ERR("Failed to dlopen libze_loader.so\n");
		goto err_out;
	}

	libze_ops.zeInit = dlsym(libze_handle, "zeInit");
	if (!libze_ops.zeInit) {
		FT_ERR("Failed to find zeInit\n");
		goto err_dlclose;
	}

	libze_ops.zeDriverGet = dlsym(libze_handle, "zeDriverGet");
	if (!libze_ops.zeDriverGet) {
		FT_ERR("Failed to find zeDriverGet\n");
		goto err_dlclose;
	}

	libze_ops.zeDeviceGet = dlsym(libze_handle, "zeDeviceGet");
	if (!libze_ops.zeDeviceGet) {
		FT_ERR("Failed to find zeDeviceGet\n");
		goto err_dlclose;
	}

	libze_ops.zeDeviceCanAccessPeer = dlsym(libze_handle, "zeDeviceCanAccessPeer");
	if (!libze_ops.zeDeviceCanAccessPeer) {
		FT_ERR("Failed to find zeDeviceCanAccessPeer\n");
		goto err_dlclose;
	}

	libze_ops.zeContextCreate = dlsym(libze_handle, "zeContextCreate");
	if (!libze_ops.zeContextCreate) {
		FT_ERR("Failed to find zeContextCreate\n");
		goto err_dlclose;
	}

	libze_ops.zeContextDestroy = dlsym(libze_handle, "zeContextDestroy");
	if (!libze_ops.zeContextDestroy) {
		FT_ERR("Failed to find zeContextDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeContextDestroy = dlsym(libze_handle, "zeContextDestroy");
	if (!libze_ops.zeContextDestroy) {
		FT_ERR("Failed to find zeContextDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandQueueCreate = dlsym(libze_handle, "zeCommandQueueCreate");
	if (!libze_ops.zeCommandQueueCreate) {
		FT_ERR("Failed to find zeCommandQueueCreate\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandQueueDestroy = dlsym(libze_handle, "zeCommandQueueDestroy");
	if (!libze_ops.zeCommandQueueDestroy) {
		FT_ERR("Failed to find zeCommandQueueDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandQueueExecuteCommandLists = dlsym(libze_handle, "zeCommandQueueExecuteCommandLists");
	if (!libze_ops.zeCommandQueueExecuteCommandLists) {
		FT_ERR("Failed to find zeCommandQueueExecuteCommandLists\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListCreate = dlsym(libze_handle, "zeCommandListCreate");
	if (!libze_ops.zeCommandListCreate) {
		FT_ERR("Failed to find zeCommandListCreate\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListDestroy = dlsym(libze_handle, "zeCommandListDestroy");
	if (!libze_ops.zeCommandListDestroy) {
		FT_ERR("Failed to find zeCommandListDestroy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListClose = dlsym(libze_handle, "zeCommandListClose");
	if (!libze_ops.zeCommandListClose) {
		FT_ERR("Failed to find zeCommandListClose\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListAppendMemoryCopy = dlsym(libze_handle, "zeCommandListAppendMemoryCopy");
	if (!libze_ops.zeCommandListAppendMemoryCopy) {
		FT_ERR("Failed to find zeCommandListAppendMemoryCopy\n");
		goto err_dlclose;
	}

	libze_ops.zeCommandListAppendMemoryFill = dlsym(libze_handle, "zeCommandListAppendMemoryFill");
	if (!libze_ops.zeCommandListAppendMemoryFill) {
		FT_ERR("Failed to find zeCommandListAppendMemoryFill\n");
		goto err_dlclose;
	}

	libze_ops.zeMemAllocDevice = dlsym(libze_handle, "zeMemAllocDevice");
	if (!libze_ops.zeMemAllocDevice) {
		FT_ERR("Failed to find zeMemAllocDevice\n");
		goto err_dlclose;
	}

	libze_ops.zeMemFree = dlsym(libze_handle, "zeMemFree");
	if (!libze_ops.zeMemFree) {
		FT_ERR("Failed to find zeMemFree\n");
		goto err_dlclose;
	}
	return FI_SUCCESS;

err_dlclose:
	dlclose(libze_handle);

err_out:
	return -FI_ENODATA;
}

static void cleanup_libze_ops(void)
{
	dlclose(libze_handle);
}

int ft_ze_init(void)
{
	ze_driver_handle_t driver;
	ze_context_desc_t context_desc = {0};
	ze_result_t ze_ret;
	uint32_t count;

	if (init_libze_ops())
		return -FI_EIO;

	ze_ret = (*libze_ops.zeInit)(ZE_INIT_FLAG_GPU_ONLY);
	if (ze_ret)
		return -FI_EIO;

	count = 1;
	ze_ret = (*libze_ops.zeDriverGet)(&count, &driver);
	if (ze_ret)
		return -FI_EIO;

	ze_ret = (*libze_ops.zeContextCreate)(driver, &context_desc, &context);
	if (ze_ret)
		return -FI_EIO;

	count = 0;
	ze_ret = (*libze_ops.zeDeviceGet)(driver, &count, NULL);
	if (ze_ret || count > ZE_MAX_DEVICES)
		goto err;;

	ze_ret = (*libze_ops.zeDeviceGet)(driver, &count, devices);
	if (ze_ret)
		goto err;

	for (num_devices = 0; num_devices < count; num_devices++) {
		ze_ret = (*libze_ops.zeCommandQueueCreate)(
					context, devices[num_devices], &cq_desc,
					&cmd_queue[num_devices]);
		if (ze_ret)
			goto err;
	}

	return FI_SUCCESS;

err:
	(void) ft_ze_cleanup();
	return -FI_EIO;
}

int ft_ze_cleanup(void)
{
	int i, ret = FI_SUCCESS;

	for (i = 0; i < num_devices; i++) {
		if (cmd_queue[i] &&
		    (*libze_ops.zeCommandQueueDestroy)(cmd_queue[i]))
			ret = -FI_EINVAL;
	}

	if ((*libze_ops.zeContextDestroy)(context))
		return -FI_EINVAL;

	cleanup_libze_ops();
	return ret;
}

int ft_ze_alloc(uint64_t device, void **buf, size_t size)
{
	return (*libze_ops.zeMemAllocDevice)(context, &device_desc, size, 16,
					     devices[device], buf) ?
			-FI_EINVAL : 0;
}

int ft_ze_free(void *buf)
{
	return (*libze_ops.zeMemFree)(context, buf) ? -FI_EINVAL : FI_SUCCESS;
}

int ft_ze_memset(uint64_t device, void *buf, int value, size_t size)
{
	ze_command_list_handle_t cmd_list;
	ze_result_t ze_ret;

	ze_ret = (*libze_ops.zeCommandListCreate)(context, devices[device],
						  &cl_desc, &cmd_list);
	if (ze_ret)
		return -FI_EIO;

	ze_ret = (*libze_ops.zeCommandListAppendMemoryFill)(
					cmd_list, buf, &value, sizeof(value),
					size, NULL, 0, NULL);
	if (ze_ret)
		goto free;

	ze_ret = (*libze_ops.zeCommandListClose)(cmd_list);
	if (ze_ret)
		goto free;

	ze_ret = (*libze_ops.zeCommandQueueExecuteCommandLists)(
					cmd_queue[device], 1, &cmd_list, NULL);

free:
	if (!(*libze_ops.zeCommandListDestroy)(cmd_list) && !ze_ret)
		return FI_SUCCESS;

	return -FI_EINVAL;
}

int ft_ze_copy(uint64_t device, void *dst, const void *src, size_t size)
{
	ze_command_list_handle_t cmd_list;
	ze_result_t ze_ret;

	ze_ret = (*libze_ops.zeCommandListCreate)(context, devices[device],
						  &cl_desc, &cmd_list);
	if (ze_ret)
		return -FI_EIO;

	ze_ret = (*libze_ops.zeCommandListAppendMemoryCopy)(
					cmd_list, dst, src, size, NULL, 0, NULL);
	if (ze_ret)
		goto free;

	ze_ret = (*libze_ops.zeCommandListClose)(cmd_list);
	if (ze_ret)
		goto free;

	ze_ret = (*libze_ops.zeCommandQueueExecuteCommandLists)(
					cmd_queue[device], 1, &cmd_list, NULL);

free:
	if (!(*libze_ops.zeCommandListDestroy)(cmd_list) && !ze_ret)
		return FI_SUCCESS;

	return -FI_EINVAL;
}

#else

int ft_ze_init(void)
{
	return -FI_ENOSYS;
}

int ft_ze_cleanup(void)
{
	return -FI_ENOSYS;
}

int ft_ze_alloc(uint64_t device, void **buf, size_t size)
{
	return -FI_ENOSYS;
}

int ft_ze_free(void *buf)
{
	return -FI_ENOSYS;
}

int ft_ze_memset(uint64_t device, void *buf, int value, size_t size)
{
	return -FI_ENOSYS;
}

int ft_ze_copy(uint64_t device, void *dst, const void *src, size_t size)
{
	return -FI_ENOSYS;
}


#endif /* HAVE_LIBZE */
