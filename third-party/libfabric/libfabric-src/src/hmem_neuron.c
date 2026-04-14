/*
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright (c) 2021 Amazon.com, Inc. or its affiliates.
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

#if HAVE_NEURON

#include <dlfcn.h>
#include "nrt/nrt.h"
#include "nrt/nrt_experimental.h"

struct neuron_ops {
	NRT_STATUS (*nrt_tensor_allocate)(nrt_tensor_placement_t tensor_placement,
					  int logical_nc_id, size_t size,
					  const char *name, nrt_tensor_t **tensor);
	void (*nrt_tensor_free)(nrt_tensor_t **tensor);
	void *(*nrt_tensor_get_va)(const nrt_tensor_t *tensor);
	NRT_STATUS (*nrt_memcpy_to_device)(void *dest, const void *src, size_t size);
	NRT_STATUS (*nrt_get_dmabuf_fd)(uint64_t va, uint64_t size, int* fd);
	NRT_STATUS (*nrt_get_total_nc_count)(uint32_t *nc_count);
};

static void *neuron_handle;
static struct neuron_ops neuron_ops;

static int neuron_dl_init(void)
{
	neuron_handle = dlopen("libnrt.so.1", RTLD_NOW);
	if (!neuron_handle) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libnrt.so.1\n");
		return -FI_ENOSYS;
	}

	neuron_ops.nrt_tensor_allocate = dlsym(neuron_handle, "nrt_tensor_allocate");
	if (!neuron_ops.nrt_tensor_allocate) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find nrt_tensor_allocate\n");
		goto err;
	}

	neuron_ops.nrt_tensor_free = dlsym(neuron_handle, "nrt_tensor_free");
	if (!neuron_ops.nrt_tensor_free) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find nrt_tensor_free\n");
		goto err;
	}

	neuron_ops.nrt_tensor_get_va = dlsym(neuron_handle, "nrt_tensor_get_va");
	if (!neuron_ops.nrt_tensor_get_va) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find nrt_tensor_get_va\n");
		goto err;
	}

	neuron_ops.nrt_memcpy_to_device = dlsym(neuron_handle, "nrt_memcpy_to_device");
	if (!neuron_ops.nrt_memcpy_to_device) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find nrt_memcpy_to_device\n");
		goto err;
	}

	neuron_ops.nrt_get_dmabuf_fd = dlsym(neuron_handle, "nrt_get_dmabuf_fd");
	if (!neuron_ops.nrt_get_dmabuf_fd) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to find nrt_get_dmabuf_fd, "
			"dmabuf feature will not be used for Neuron devices\n");
	}

	neuron_ops.nrt_get_total_nc_count = dlsym(neuron_handle, "nrt_get_total_nc_count");
	if (!neuron_ops.nrt_get_total_nc_count) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find nrt_get_total_nc_count");
		goto err;
	}

	return FI_SUCCESS;
err:
	dlclose(neuron_handle);
	return -FI_ENODATA;
}

static int neuron_dl_close(void)
{
	dlclose(neuron_handle);
	return FI_SUCCESS;
}

int neuron_copy_to_dev(uint64_t device, void *dev, const void *host, size_t size)
{
	NRT_STATUS ret;

	ret = neuron_ops.nrt_memcpy_to_device(dev, host, size);
	if (ret == NRT_SUCCESS)
		return FI_SUCCESS;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to copy from host memory "
		"to AWS neuron: %d\n", ret);
	return -FI_EIO;
}

int neuron_copy_from_dev(uint64_t device, void *host, const void *dev, size_t size)
{
	FI_WARN_ONCE(&core_prov, FI_LOG_CORE,
		     "Copies from AWS Neuron to host memory are not supported.");
	return -FI_ENOSYS;
}

int neuron_host_register(void *ptr, size_t size)
{
	return FI_SUCCESS;
}

int neuron_host_unregister(void *ptr)
{
	return FI_SUCCESS;
}

int neuron_hmem_init(void)
{
	int ret;
	uint32_t total_nc_count;

	ret = neuron_dl_init();
	if (ret)
		return ret;

	/* Note that nrt_get_total_nc_count() is one of the few neuron functions
	 * that can be called before nrt_init()
	 */
	if (neuron_ops.nrt_get_total_nc_count(&total_nc_count) != NRT_SUCCESS) {
		return -FI_ENOSYS;
	}

	if (total_nc_count == 0) {
		return -FI_ENOSYS;
	}

	FI_INFO(&core_prov, FI_LOG_CORE, "number of neuron cores: %d\n", total_nc_count);
	return FI_SUCCESS;
}

int neuron_hmem_cleanup(void)
{
	neuron_dl_close();
	return FI_SUCCESS;
}

void *neuron_alloc(void **handle, size_t size)
{
	NRT_STATUS ret;
	nrt_tensor_t **tensor;
	void *ptr;

	tensor = (nrt_tensor_t **)handle;

	ret = neuron_ops.nrt_tensor_allocate(NRT_TENSOR_PLACEMENT_DEVICE, 0,
					     size, "libfabric", tensor);

	if (ret != NRT_SUCCESS) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to allocate nrt tensor: %d\n", ret);
		return NULL;
	}

	ptr = neuron_ops.nrt_tensor_get_va(*tensor);
	if (!ptr)
		neuron_ops.nrt_tensor_free(tensor);

	return ptr;
}

void neuron_free(void **handle)
{
	neuron_ops.nrt_tensor_free((nrt_tensor_t **)handle);
}

/**
 * @brief Get the dma-buf fd of Neuron memory region if it was registered for EFA peer direct
 *
 * @param addr[in] the device buffer address
 * @param size[in] the device buffer size (in bytes)
 * @param fd[out] the dma-buf fd
 * @param offset[out] the offset within the dma-buf object
 * @return int On success, return 0. On failure, return a negative error code
 */
int neuron_get_dmabuf_fd(const void *addr, uint64_t size, int *fd,
			 uint64_t *offset)
{
	NRT_STATUS ret;

	/* nrt_get_dmabuf_fd symbol doesn't exist in Neuron Runtime */
	if (!neuron_ops.nrt_get_dmabuf_fd) {
		return -FI_EOPNOTSUPP;
	}

	ret = neuron_ops.nrt_get_dmabuf_fd((uintptr_t)addr, size, fd);

	if (ret == NRT_SUCCESS) {
		/*
		 * The assumption is that nrt_get_dmabuf_fd() would fail for
		 * any addr that is not the starting address of the dma-buf
		 * object. Otherwise we need a low level op to get the base
		 * address of the dma-buf object.
		 */
		*offset = 0;
		return FI_SUCCESS;
	} else if (ret == NRT_RESOURCE) {
		/* real error from Neuron */
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to retrieve dmabuf_fd: %d\n", ret);
		return -FI_EINVAL;
	} else {
		/* fallback to mem registration using ibv_reg_mr */
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to retrieve dmabuf_fd: %d\n", ret);
		return -FI_EOPNOTSUPP;
	}
}

int neuron_put_dmabuf_fd(int fd)
{
	close(fd);
	return FI_SUCCESS;
}

#else

int neuron_copy_to_dev(uint64_t device, void *dev, const void *host, size_t size)
{
	return -FI_ENOSYS;
}

int neuron_copy_from_dev(uint64_t device, void *host, const void *dev, size_t size)
{
	return -FI_ENOSYS;
}

int neuron_host_register(void *ptr, size_t size)
{
	return -FI_ENOSYS;
}

int neuron_host_unregister(void *ptr)
{
	return -FI_ENOSYS;
}

int neuron_hmem_init(void)
{
	return -FI_ENOSYS;
}

int neuron_hmem_cleanup(void)
{
	return -FI_ENOSYS;
}

void *neuron_alloc(void **handle, size_t size)
{
	return NULL;
}

void neuron_free(void **handle)
{
	return;
}

int neuron_get_dmabuf_fd(const void *addr, uint64_t size, int *fd,
			 uint64_t *offset)
{
	return -FI_ENOSYS;
}

int neuron_put_dmabuf_fd(int fd)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_NEURON */
