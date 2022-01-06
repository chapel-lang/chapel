/*
 * Copyright (c) 2020-2022 Intel Corporation. All rights reserved.
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

#ifdef HAVE_LIBZE

#include <dirent.h>
#include <drm/i915_drm.h>
#include <sys/ioctl.h>
#include <level_zero/ze_api.h>

static ze_context_handle_t context;
static ze_device_handle_t devices[ZE_MAX_DEVICES];
static ze_command_queue_handle_t cmd_queue[ZE_MAX_DEVICES];
static int num_devices = 0;
static int dev_fds[ZE_MAX_DEVICES];
static bool p2p_enabled = false;

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

static int ze_hmem_init_fds(void)
{
	const char *dev_dir = "/dev/dri/by-path";
	const char *suffix = "-render";
	DIR *dir;
	struct dirent *ent = NULL;
	char dev_name[128];
	int i = 0;

	dir = opendir(dev_dir);
	if (dir == NULL)
		return -FI_EIO;

	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.' ||
		    strstr(ent->d_name, suffix) == NULL)
			continue;

		memset(dev_name, 0, sizeof(dev_name));
		strncpy(dev_name, dev_dir, sizeof(dev_name));
		strncat(dev_name, "/",
			sizeof(dev_name) - strlen(dev_name));
		strncat(dev_name, ent->d_name,
			sizeof(dev_name) - strlen(dev_name));
		dev_fds[i] = open(dev_name, O_RDWR);
		if (dev_fds[i] == -1) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Failed open device %d\n", i);
			return -FI_EIO;
		}
		i++;
	}
	return FI_SUCCESS;
}

int ze_hmem_init(void)
{
	ze_driver_handle_t driver;
	ze_context_desc_t context_desc = {0};
	ze_result_t ze_ret;
	ze_bool_t access;
	uint32_t count, i;
	bool p2p = true;
	int ret;

	ze_ret = zeInit(ZE_INIT_FLAG_GPU_ONLY);
	if (ze_ret)
		return -FI_EIO;

	count = 1;
	ze_ret = zeDriverGet(&count, &driver);
	if (ze_ret)
		return -FI_EIO;

	ze_ret = zeContextCreate(driver, &context_desc, &context);
	if (ze_ret)
		return -FI_EIO;

	for (i = 0; i < ZE_MAX_DEVICES; dev_fds[i++] = -1)
		;

	count = 0;
	ze_ret = zeDeviceGet(driver, &count, NULL);
	if (ze_ret || count > ZE_MAX_DEVICES)
		goto err;

	ze_ret = zeDeviceGet(driver, &count, devices);
	if (ze_ret)
		goto err;

	ret = ze_hmem_init_fds();
	if (ret)
		goto err;

	for (num_devices = 0; num_devices < count; num_devices++) {
		ze_ret = zeCommandQueueCreate(context, devices[num_devices], &cq_desc,
					      &cmd_queue[num_devices]);
		if (ze_ret)
			goto err;

		for (i = 0; i < count; i++) {
			if (zeDeviceCanAccessPeer(devices[num_devices],
					devices[i], &access) || !access)
				p2p = false;
		}
	}

	p2p_enabled = p2p;
	return FI_SUCCESS;

err:
	(void) ze_hmem_cleanup();
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to initialize ZE driver resources\n");

	return -FI_EIO;
}

int ze_hmem_cleanup(void)
{
	int i, ret = FI_SUCCESS;

	for (i = 0; i < num_devices; i++) {
		if (cmd_queue[i] && zeCommandQueueDestroy(cmd_queue[i])) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Failed to destroy ZE cmd_queue\n");
			ret = -FI_EINVAL;
		}
		if (dev_fds[i] != -1) {
			close(dev_fds[i]);
			dev_fds[i] = -1;
		}
	}

	if (zeContextDestroy(context))
		return -FI_EINVAL;

	return ret;
}

int ze_hmem_copy(uint64_t device, void *dst, const void *src, size_t size)
{
	ze_command_list_handle_t cmd_list;
	ze_result_t ze_ret;
	int dev_id = (int) device;

	ze_ret = zeCommandListCreate(context, devices[dev_id], &cl_desc, &cmd_list);
	if (ze_ret)
		goto err;

	ze_ret = zeCommandListAppendMemoryCopy(cmd_list, dst, src, size, NULL, 0, NULL);
	if (ze_ret)
		goto free;

	ze_ret = zeCommandListClose(cmd_list);
	if (ze_ret)
		goto free;

	ze_ret = zeCommandQueueExecuteCommandLists(cmd_queue[dev_id], 1,
						   &cmd_list, NULL);

free:
	if (!zeCommandListDestroy(cmd_list) && !ze_ret)
		return FI_SUCCESS;
err:
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform ze copy (%d)\n", ze_ret);

	return -FI_EIO;
}

bool ze_is_addr_valid(const void *addr)
{
	ze_result_t ze_ret;
	ze_memory_allocation_properties_t mem_prop;
	ze_device_handle_t device;
	int i;

	for (i = 0; i < num_devices; i++) {
		ze_ret = zeMemGetAllocProperties(context, addr, &mem_prop,
						 &device);
		if (!ze_ret && mem_prop.type == ZE_MEMORY_TYPE_DEVICE)
			return true;
	}
	return false;
}

int ze_hmem_get_handle(void *dev_buf, void **handle)
{
	ze_result_t ze_ret;

	ze_ret = zeMemGetIpcHandle(context, dev_buf,
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

	ze_ret = zeMemOpenIpcHandle(context, devices[device],
				    *((ze_ipc_mem_handle_t *) handle),
				    0, ipc_ptr);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unable to open memory handle\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

int ze_hmem_get_shared_handle(int dev_fd, void *dev_buf, int *ze_fd,
			      void **handle)
{
	struct drm_prime_handle open_fd = {0, 0, 0};
	ze_ipc_mem_handle_t ze_handle;
	int ret;

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

int ze_hmem_close_handle(void *ipc_ptr)
{
	ze_result_t ze_ret;

	ze_ret = zeMemCloseIpcHandle(context, ipc_ptr);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unable to close memory handle\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

bool ze_hmem_p2p_enabled(void)
{
	return p2p_enabled;
}

int ze_hmem_get_base_addr(const void *ptr, void **base)
{
	ze_result_t ze_ret;
	size_t size;

	ze_ret = zeMemGetAddressRange(context, ptr, base, &size);
	if (ze_ret) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Could not get base addr\n");
		return -FI_EINVAL;
	}
	return FI_SUCCESS;
}

int *ze_hmem_get_dev_fds(int *nfds)
{
	*nfds = num_devices;
	return dev_fds;
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

bool ze_is_addr_valid(const void *addr)
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

int ze_hmem_get_base_addr(const void *ptr, void **base)
{
	return -FI_ENOSYS;
}

int *ze_hmem_get_dev_fds(int *nfds)
{
	*nfds = 0;
	return NULL;
}

#endif /* HAVE_LIBZE */
