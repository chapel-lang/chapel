/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates.
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

#if HAVE_SYNAPSEAI

#include <dlfcn.h>
#include "habanalabs/synapse_api.h"

struct synapseai_ops {
	synStatus (*synInitialize)(void);
	synStatus (*synDestroy)(void);
	synStatus (*synDeviceGetCount)(uint32_t* pCount);
	int (*hcclLookupDMABuff)(uint64_t addr, uint64_t size, int* fd);
};

static void *synapseai_handle;
static struct synapseai_ops synapseai_ops;

int synapseai_dl_init(void)
{
	synapseai_handle = dlopen("libSynapse.so", RTLD_NOW);
	if (!synapseai_handle) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libSynapse.so\n");
		return -FI_ENOSYS;
	}

	synapseai_ops.synInitialize = dlsym(synapseai_handle, "synInitialize");
	if (!synapseai_ops.synInitialize) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find synInitialize\n");
		goto err;
	}

	synapseai_ops.synDestroy = dlsym(synapseai_handle, "synDestroy");
	if (!synapseai_ops.synDestroy) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find synDestroy\n");
		goto err;
	}

	synapseai_ops.synDeviceGetCount = dlsym(synapseai_handle, "synDeviceGetCount");
	if (!synapseai_ops.synDeviceGetCount) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find synDeviceGetCount\n");
		goto err;
	}

	synapseai_ops.hcclLookupDMABuff = dlsym(synapseai_handle, "hcclLookupDMABuff");
	if (!synapseai_ops.hcclLookupDMABuff) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hcclLookupDMABuff\n");
		goto err;
	}

	return FI_SUCCESS;
err:
	dlclose(synapseai_handle);
	return -FI_ENODATA;
}

/**
 * @brief Initialize SynapseAI hmem interface by dlopen the SynapseAI library
 *
 * @return On success, return 0. On failure, return negative error code.
 */
int synapseai_init(void)
{
	int err;
	synStatus status;
	uint32_t device_count = 0;

	err = synapseai_dl_init();
	if (err)
		return err;

	status = synapseai_ops.synInitialize();
	if (status != synSuccess) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"synInitialize failed: %d\n", status);
		return -FI_EIO;
	}

	status = synapseai_ops.synDeviceGetCount(&device_count);

	/*
	 * TODO: Starting from here we should call synDestroy before
	 * returning error to free the resources allocated in synInitialize,
	 * but the destroy call hangs on instances without
	 * a habana device
	 */

	if (status != synSuccess) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"synDeviceGetCount failed: %d\n", status);
		return -FI_EIO;
	}

	if (device_count == 0)
		return -FI_ENOSYS;

	return FI_SUCCESS;
}

/**
 * @brief Clean up SynapseAI intereface (dlclose)
 * @return return 0.
 */
int synapseai_cleanup(void)
{
	dlclose(synapseai_handle);
	return FI_SUCCESS;
}

int synapseai_copy_to_hmem(uint64_t device, void *dest, const void *src,
                           size_t size)
{
	return -FI_ENOSYS;
}

int synapseai_copy_from_hmem(uint64_t device, void *dest, const void *src,
                             size_t size)
{
	return -FI_ENOSYS;
}

bool synapseai_is_addr_valid(const void *addr, uint64_t *device,
                             uint64_t *flags)
{
	return false;
}

int synapseai_host_register(void *ptr, size_t size)
{
	return FI_SUCCESS;
}

int synapseai_host_unregister(void *ptr)
{
	return FI_SUCCESS;
}

/**
 * @brief Get the dma-buf fd of Gaudi memory region if it was registrated for EFA peer direct
 *
 * @param addr[in] the device buffer address
 * @param size[in] the device buffer size (in bytes)
 * @param fd[out] the dma-buf fd
 * @param offset[out] the offset within the dma-buf object
 * @return int On success, return 0. On failure, return a negative error code.
 */
int synapseai_get_dmabuf_fd(const void *addr, uint64_t size, int *fd,
			    uint64_t *offset)
{
	int ret;
	ret = synapseai_ops.hcclLookupDMABuff((uintptr_t)addr, size, fd);
	if (*fd < 0) {
		FI_WARN(&core_prov, FI_LOG_CORE,
				"hcclLookupDMABuff failed, ret: %d\n", ret);
		return -FI_EIO;
	}

	/*
	 * The assumption is that hcclLookupDMABuff() would fail for any addr
	 * that is not the starting address of the dma-buf object. Otherwise we
	 * need a low level op to get the base address of the dma-buf object.
	 */
	*offset = 0;

	return FI_SUCCESS;
}

#else
int synapseai_init(void)
{
	return -FI_ENOSYS;
}

int synapseai_cleanup(void)
{
	return -FI_ENOSYS;
}

int synapseai_copy_to_hmem(uint64_t device, void *dest, const void *src,
                           size_t size)
{
	return -FI_ENOSYS;
}

int synapseai_copy_from_hmem(uint64_t device, void *dest, const void *src,
                             size_t size)
{
	return -FI_ENOSYS;
}

bool synapseai_is_addr_valid(const void *addr, uint64_t *device,
                             uint64_t *flags)
{
	return false;
}

int synapseai_host_register(void *ptr, size_t size)
{
	return -FI_ENOSYS;
}

int synapseai_host_unregister(void *ptr)
{
	return -FI_ENOSYS;
}

int synapseai_get_dmabuf_fd(const void *addr, uint64_t size, int *fd,
			    uint64_t *offset)
{
	return -FI_ENOSYS;
}
#endif /* HAVE_SYNAPSEAI */
