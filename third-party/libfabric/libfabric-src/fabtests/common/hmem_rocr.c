/*
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "hmem.h"
#include "shared.h"

#ifdef HAVE_ROCR_RUNTIME_H

#include <dlfcn.h>
#include <stdio.h>
#include <hsa/hsa.h>

struct rocr_ops {
	hsa_status_t (*hsa_memory_copy)(void *dst, const void *src,
					size_t size);
	hsa_status_t (*hsa_init)(void);
	hsa_status_t (*hsa_shut_down)(void);
	hsa_status_t (*hsa_status_string)(hsa_status_t status,
					  const char **status_string);
	hsa_status_t (*hsa_agent_get_info)(hsa_agent_t agent,
					   hsa_agent_info_t attribute,
					   void *value);
	hsa_status_t (*hsa_region_get_info)(hsa_region_t region,
					    hsa_region_info_t attribute,
					    void *value);
	hsa_status_t (*hsa_iterate_agents)
		(hsa_status_t (*cb)(hsa_agent_t agent, void* data), void *data);
	hsa_status_t (*hsa_agent_iterate_regions)
		(hsa_agent_t agent,
		 hsa_status_t (*cb)(hsa_region_t region, void* data),
		 void *data);
	hsa_status_t (*hsa_memory_allocate)(hsa_region_t region, size_t size,
					    void **ptr);
	hsa_status_t (*hsa_memory_free)(void *ptr);
	hsa_status_t (*hsa_amd_memory_fill)(void* ptr, uint32_t value,
					    size_t count);
};

static struct rocr_ops rocr_ops;
static void *rocr_handle;

static const char *hsa_status_to_string(hsa_status_t status)
{
	const char *str;
	hsa_status_t hsa_ret;

	hsa_ret = rocr_ops.hsa_status_string(status, &str);
	if (hsa_ret != HSA_STATUS_SUCCESS)
		return "unknown error";

	return str;
}

#define ROCR_ERR(err, fmt, ...) \
	FT_ERR(fmt ": %s", ##__VA_ARGS__, hsa_status_to_string(err))

static hsa_agent_t gpu_agent;
static hsa_region_t gpu_region;

static hsa_status_t agent_cb(hsa_agent_t agent, void *data)
{
	hsa_status_t hsa_ret;
	hsa_device_type_t hsa_dev_type;

	hsa_ret = rocr_ops.hsa_agent_get_info(agent, HSA_AGENT_INFO_DEVICE,
					      (void *) &hsa_dev_type);

	if (hsa_ret == HSA_STATUS_SUCCESS &&
	    hsa_dev_type == HSA_DEVICE_TYPE_GPU) {
		gpu_agent = agent;
		return HSA_STATUS_INFO_BREAK;
	}

	return hsa_ret;
}

static hsa_status_t region_cb(hsa_region_t region, void *data)
{
	hsa_status_t hsa_ret;
	hsa_region_segment_t hsa_segment;

	hsa_ret = rocr_ops.hsa_region_get_info(region, HSA_REGION_INFO_SEGMENT,
					       &hsa_segment);

	if (hsa_ret == HSA_STATUS_SUCCESS &&
	    hsa_segment == HSA_REGION_SEGMENT_GLOBAL) {
		gpu_region = region;
		return HSA_STATUS_INFO_BREAK;
	}

	return hsa_ret;
}

int ft_rocr_init(void)
{
	hsa_status_t hsa_ret;

	rocr_handle = dlopen("libhsa-runtime64.so", RTLD_NOW);
	if (!rocr_handle) {
		FT_ERR("Failed to dlopen libhsa-runtime64.so");
		goto err;
	}

	rocr_ops.hsa_memory_copy = dlsym(rocr_handle, "hsa_memory_copy");
	if (!rocr_ops.hsa_memory_copy) {
		FT_ERR("Failed to find hsa_memory_copy");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_init = dlsym(rocr_handle, "hsa_init");
	if (!rocr_ops.hsa_init) {
		FT_ERR("Failed to find hsa_init");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_shut_down = dlsym(rocr_handle, "hsa_shut_down");
	if (!rocr_ops.hsa_shut_down) {
		FT_ERR("Failed to find hsa_shut_down");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_status_string = dlsym(rocr_handle, "hsa_status_string");
	if (!rocr_ops.hsa_status_string) {
		FT_ERR("Failed to find hsa_status_string");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_agent_get_info = dlsym(rocr_handle, "hsa_agent_get_info");
	if (!rocr_ops.hsa_agent_get_info) {
		FT_ERR("Failed to find hsa_agent_get_info");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_region_get_info = dlsym(rocr_handle,
					     "hsa_region_get_info");
	if (!rocr_ops.hsa_region_get_info) {
		FT_ERR("Failed to find hsa_region_get_info");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_iterate_agents = dlsym(rocr_handle, "hsa_iterate_agents");
	if (!rocr_ops.hsa_iterate_agents) {
		FT_ERR("Failed to find hsa_iterate_agents");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_agent_iterate_regions =
		dlsym(rocr_handle, "hsa_agent_iterate_regions");
	if (!rocr_ops.hsa_agent_iterate_regions) {
		FT_ERR("Failed to find hsa_agent_iterate_regions");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_memory_allocate =
		dlsym(rocr_handle, "hsa_memory_allocate");
	if (!rocr_ops.hsa_memory_allocate) {
		FT_ERR("Failed to find hsa_memory_allocate");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_memory_free = dlsym(rocr_handle, "hsa_memory_free");
	if (!rocr_ops.hsa_memory_free) {
		FT_ERR("Failed to find hsa_memory_free");
		goto err_dlclose_rocr;
	}

	rocr_ops.hsa_amd_memory_fill = dlsym(rocr_handle,
					     "hsa_amd_memory_fill");
	if (!rocr_ops.hsa_amd_memory_fill) {
		FT_ERR("Failed to find hsa_amd_memory_fill");
		goto err_dlclose_rocr;
	}

	hsa_ret = rocr_ops.hsa_init();
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		ROCR_ERR(hsa_ret, "hsa_init failed");
		goto err_dlclose_rocr;
	}

	hsa_ret = rocr_ops.hsa_iterate_agents(agent_cb, NULL);
	if (hsa_ret != HSA_STATUS_INFO_BREAK) {
		FT_ERR("Failed to find GPU agent");
		goto err_dlclose_rocr;
	}

	hsa_ret = rocr_ops.hsa_agent_iterate_regions(gpu_agent, region_cb,
						     NULL);
	if (hsa_ret != HSA_STATUS_INFO_BREAK) {
		FT_ERR("Failed to find GPU region");
		goto err_dlclose_rocr;
	}

	return FI_SUCCESS;

err_dlclose_rocr:
	dlclose(rocr_handle);
err:
	return -FI_ENODATA;
}

int ft_rocr_cleanup(void)
{
	hsa_status_t hsa_ret;

	hsa_ret = rocr_ops.hsa_shut_down();
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		ROCR_ERR(hsa_ret, "hsa_init failed");
		return -FI_ENODATA;
	}

	dlclose(rocr_handle);

	return FI_SUCCESS;
}

int ft_rocr_alloc(uint64_t device, void **buf, size_t size)
{
	hsa_status_t hsa_ret;

	hsa_ret = rocr_ops.hsa_memory_allocate(gpu_region, size, buf);
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return FI_SUCCESS;

	ROCR_ERR(hsa_ret, "hsa_memory_allocate failed");

	return -FI_ENOMEM;
}

int ft_rocr_free(void *buf)
{
	hsa_status_t hsa_ret;

	hsa_ret = rocr_ops.hsa_memory_free(buf);
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return FI_SUCCESS;

	ROCR_ERR(hsa_ret, "hsa_memory_free failed");

	return -FI_EIO;
}

#define ROCR_MEM_FILL_BYTE_ALIGNMENT 4U

int ft_rocr_memset(uint64_t device, void *buf, int value, size_t size)
{
	unsigned char set_value = value;
	void *mem_fill_ptr;
	size_t mem_fill_size;
	uint32_t mem_fill_value;
	hsa_status_t hsa_ret;
	unsigned char *ptr = buf;
	int ret;

	/* Determine if ROCR memory fill can be used to set device memory. ROCR
	 * memory fill requires 4-byte alignment.
	 */
	mem_fill_ptr = (void *) ALIGN((uintptr_t) buf,
				      ROCR_MEM_FILL_BYTE_ALIGNMENT);

	/* Use ROCR memory copy to fill the start of the buffer until the buffer
	 * is correctly aligned.
	 */
	while (ptr != mem_fill_ptr && size > 0) {
		ret = ft_rocr_memcpy(device, ptr, &set_value, sizeof(*ptr));
		if (ret != FI_SUCCESS)
			return ret;

		size--;
		ptr++;
	}

	/* Use ROCR memory fill to fill the middle of the buffer. */
	if (size >= ROCR_MEM_FILL_BYTE_ALIGNMENT) {
		mem_fill_size = ALIGN_DOWN(size, ROCR_MEM_FILL_BYTE_ALIGNMENT);

		memset(&mem_fill_value, set_value, sizeof(mem_fill_value));

		hsa_ret = rocr_ops.hsa_amd_memory_fill(mem_fill_ptr,
						       mem_fill_value,
						       mem_fill_size /
						       ROCR_MEM_FILL_BYTE_ALIGNMENT);
		if (hsa_ret != HSA_STATUS_SUCCESS) {
			ROCR_ERR(hsa_ret, "hsa_amd_memory_fill failed");
			return -FI_EIO;
		}

		size -= mem_fill_size;
		ptr += mem_fill_size;
	}

	/* Use ROCR memory copy to fill the end of the buffer. */
	while (size > 0) {
		ret = ft_rocr_memcpy(device, ptr, &set_value, sizeof(*ptr));
		if (ret != FI_SUCCESS)
			return ret;

		size--;
		ptr++;
	}

	return FI_SUCCESS;
}

int ft_rocr_memcpy(uint64_t device, void *dst, const void *src, size_t size)
{
	hsa_status_t hsa_ret;

	hsa_ret = rocr_ops.hsa_memory_copy(dst, src, size);
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return FI_SUCCESS;

	ROCR_ERR(hsa_ret, "hsa_memory_copy failed");

	return -FI_EIO;
}

#else

int ft_rocr_init(void)
{
	return -FI_ENOSYS;
}

int ft_rocr_cleanup(void)
{
	return -FI_ENOSYS;
}

int ft_rocr_alloc(uint64_t device, void **buf, size_t size)
{
	return -FI_ENOSYS;
}

int ft_rocr_free(void *buf)
{
	return -FI_ENOSYS;
}

int ft_rocr_memset(uint64_t device, void *buf, int value, size_t size)
{
	return -FI_ENOSYS;
}

int ft_rocr_memcpy(uint64_t device, void *dst, const void *src, size_t size)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_ROCR_RUNTIME_H */
