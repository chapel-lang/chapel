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
	int (*hcclLookupDMABuff)(uint64_t addr, uint64_t size, int* fd);
};

static void *synapseai_handle;
static struct synapseai_ops synapseai_ops;

/**
 * @brief Initialize SynapseAI hmem interface by dlopen the SynapseAI library
 * 
 * @return On success, return 0. On failure, return negative error code.
 */
int synapseai_init(void)
{
	synapseai_handle = dlopen("libSynapse.so", RTLD_NOW);
	if (!synapseai_handle) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libSynapse.so\n");
		return -FI_ENOSYS;
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

int synapseai_get_handle(void *dev_buf, void **handle)
{
	return -FI_ENOSYS;
}

int synapseai_open_handle(void **handle, uint64_t device, void **ipc_ptr)
{
	return -FI_ENOSYS;
}

int synapseai_close_handle(void *ipc_ptr)
{
	return -FI_ENOSYS;
}

int synapseai_host_register(void *ptr, size_t size)
{
	return -FI_ENOSYS;
}

int synapseai_host_unregister(void *ptr)
{
	return -FI_ENOSYS;
}

int synapseai_get_base_addr(const void *ptr, void **base, size_t *size)
{
	return -FI_ENOSYS;
}

bool synapseai_is_ipc_enabled(void)
{
	return false;
}

/**
 * @brief Get the dma-buf fd of Gaudi memory region if it was registrated for EFA peer direct
 * 
 * @param addr[in] the device buffer address
 * @param size[in] the device buffer size (in bytes)
 * @param fd[out] the dma-buf fd 
 * @return int On success, return 0. On failure, return a negative error code.
 */
int synapseai_get_dmabuf_fd(uint64_t addr, uint64_t size, int* fd)
{
	int ret;
	ret = synapseai_ops.hcclLookupDMABuff(addr, size, fd);
	if (*fd < 0) {
		FI_WARN(&core_prov, FI_LOG_CORE,
				"hcclLookupDMABuff failed, ret: %d\n", ret);
		return -FI_EIO;
	}
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

int synapseai_get_handle(void *dev_buf, void **handle)
{
	return -FI_ENOSYS;
}

int synapseai_open_handle(void **handle, uint64_t device, void **ipc_ptr)
{
	return -FI_ENOSYS;
}

int synapseai_close_handle(void *ipc_ptr)
{
	return -FI_ENOSYS;
}

int synapseai_host_register(void *ptr, size_t size)
{
	return -FI_ENOSYS;
}

int synapseai_host_unregister(void *ptr)
{
	return -FI_ENOSYS;
}

int synapseai_get_base_addr(const void *ptr, void **base, size_t *size)
{
	return -FI_ENOSYS;
}

bool synapseai_is_ipc_enabled(void)
{
	return false;
}

int synapseai_get_dmabuf_fd(uint64_t addr, uint64_t size, int* fd)
{
	return -FI_ENOSYS;
}
#endif /* HAVE_SYNAPSEAI */