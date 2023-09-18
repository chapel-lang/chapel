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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "ofi_hmem.h"
#include "ofi.h"

#if HAVE_ROCR

#include <hsa/hsa_ext_amd.h>

struct rocr_ops {
	hsa_status_t (*hsa_memory_copy)(void *dst, const void *src,
					size_t size);
	hsa_status_t (*hsa_amd_pointer_info)(void *ptr,
					     hsa_amd_pointer_info_t *info,
					     void *(*alloc)(size_t),
					     uint32_t *num_agents_accessible,
					     hsa_agent_t **accessible);
	hsa_status_t (*hsa_init)(void);
	hsa_status_t (*hsa_shut_down)(void);
	hsa_status_t (*hsa_status_string)(hsa_status_t status,
					  const char **status_string);
	hsa_status_t (*hsa_amd_dereg_dealloc_cb)(void *ptr,
						 hsa_amd_deallocation_callback_t cb);
	hsa_status_t (*hsa_amd_reg_dealloc_cb)(void *ptr,
					       hsa_amd_deallocation_callback_t cb,
					       void *user_data);
	hsa_status_t (*hsa_amd_memory_lock)(void *host_ptr, size_t size,
					    hsa_agent_t *agents, int num_agents,
					    void **agent_ptr);
	hsa_status_t (*hsa_amd_memory_unlock)(void *host_ptr);
	hsa_status_t (*hsa_agent_get_info)(hsa_agent_t agent,
					   hsa_agent_info_t attribute,
					   void *value);
};

#if ENABLE_ROCR_DLOPEN

#include <dlfcn.h>

static void *rocr_handle;
static struct rocr_ops rocr_ops;

#else

static struct rocr_ops rocr_ops = {
	.hsa_memory_copy = hsa_memory_copy,
	.hsa_amd_pointer_info = hsa_amd_pointer_info,
	.hsa_init = hsa_init,
	.hsa_shut_down = hsa_shut_down,
	.hsa_status_string = hsa_status_string,
	.hsa_amd_dereg_dealloc_cb =
		hsa_amd_deregister_deallocation_callback,
	.hsa_amd_reg_dealloc_cb =
		hsa_amd_register_deallocation_callback,
	.hsa_amd_memory_lock = hsa_amd_memory_lock,
	.hsa_amd_memory_unlock = hsa_amd_memory_unlock,
	.hsa_agent_get_info = hsa_agent_get_info,
};

#endif /* ENABLE_ROCR_DLOPEN */

hsa_status_t ofi_hsa_amd_memory_lock(void *host_ptr, size_t size,
				     hsa_agent_t *agents, int num_agents,
				     void **agent_ptr)
{
	return rocr_ops.hsa_amd_memory_lock(host_ptr, size, agents, num_agents,
					    agent_ptr);
}

hsa_status_t ofi_hsa_amd_memory_unlock(void *host_ptr)
{
	return rocr_ops.hsa_amd_memory_unlock(host_ptr);
}

hsa_status_t ofi_hsa_memory_copy(void *dst, const void *src, size_t size)
{
	return rocr_ops.hsa_memory_copy(dst, src, size);
}

hsa_status_t ofi_hsa_amd_pointer_info(void *ptr, hsa_amd_pointer_info_t *info,
				      void *(*alloc)(size_t),
				      uint32_t *num_agents_accessible,
				      hsa_agent_t **accessible)
{
	return rocr_ops.hsa_amd_pointer_info(ptr, info, alloc,
					     num_agents_accessible, accessible);
}

hsa_status_t ofi_hsa_init(void)
{
	return rocr_ops.hsa_init();
}

hsa_status_t ofi_hsa_shut_down(void)
{
	return rocr_ops.hsa_shut_down();
}

hsa_status_t ofi_hsa_status_string(hsa_status_t status,
				   const char **status_string)
{
	return rocr_ops.hsa_status_string(status, status_string);
}

const char *ofi_hsa_status_to_string(hsa_status_t status)
{
	const char *str;
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_status_string(status, &str);
	if (hsa_ret != HSA_STATUS_SUCCESS)
		return "unknown error";

	return str;
}

hsa_status_t ofi_hsa_amd_dereg_dealloc_cb(void *ptr,
					  hsa_amd_deallocation_callback_t cb)
{
	return rocr_ops.hsa_amd_dereg_dealloc_cb(ptr, cb);
}

hsa_status_t ofi_hsa_amd_reg_dealloc_cb(void *ptr,
					hsa_amd_deallocation_callback_t cb,
					void *user_data)
{
	return rocr_ops.hsa_amd_reg_dealloc_cb(ptr, cb, user_data);
}

static hsa_status_t ofi_hsa_agent_get_info(hsa_agent_t agent,
					   hsa_agent_info_t attribute,
					   void *value)
{
	return rocr_ops.hsa_agent_get_info(agent, attribute, value);
}

static int rocr_memcpy(void *dest, const void *src, size_t size)
{
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_memory_copy(dest, src, size);
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return 0;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform hsa_memory_copy: %s\n",
		ofi_hsa_status_to_string(hsa_ret));

	return -FI_EIO;
}

static int rocr_host_memory_ptr(void *host_ptr, void **ptr)
{
	hsa_amd_pointer_info_t info = {
		.size = sizeof(info),
	};
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_amd_pointer_info((void *)host_ptr, &info, NULL, NULL,
					   NULL);
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform hsa_amd_pointer_info: %s\n",
			ofi_hsa_status_to_string(hsa_ret));

		return -FI_EIO;
	}

	if (info.type != HSA_EXT_POINTER_TYPE_LOCKED)
		*ptr = host_ptr;
	else
		*ptr = (void *) ((uintptr_t) info.agentBaseAddress +
				 (uintptr_t) host_ptr -
				 (uintptr_t) info.hostBaseAddress);

	return FI_SUCCESS;
}

int rocr_copy_from_dev(uint64_t device, void *dest, const void *src,
		       size_t size)
{
	int ret;
	void *dest_memcpy_ptr;

	ret = rocr_host_memory_ptr(dest, &dest_memcpy_ptr);
	if (ret != FI_SUCCESS)
		return ret;

	ret = rocr_memcpy(dest_memcpy_ptr, src, size);

	return ret;
}

int rocr_copy_to_dev(uint64_t device, void *dest, const void *src,
		     size_t size)
{
	int ret;
	void *src_memcpy_ptr;

	ret = rocr_host_memory_ptr((void *) src, &src_memcpy_ptr);
	if (ret != FI_SUCCESS)
		return ret;

	ret = rocr_memcpy(dest, src_memcpy_ptr, size);

	return ret;
}

bool rocr_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags)
{
	hsa_amd_pointer_info_t hsa_info = {
		.size = sizeof(hsa_info),
	};
	hsa_device_type_t hsa_dev_type;
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_amd_pointer_info((void *)addr, &hsa_info, NULL, NULL,
					   NULL);
	if (hsa_ret == HSA_STATUS_SUCCESS) {
		hsa_ret = ofi_hsa_agent_get_info(hsa_info.agentOwner,
						 HSA_AGENT_INFO_DEVICE,
						 (void *) &hsa_dev_type);
		if (hsa_ret == HSA_STATUS_SUCCESS) {
			if (hsa_dev_type == HSA_DEVICE_TYPE_GPU) {
				//TODO get device pointer/id
				if (flags)
					*flags = FI_HMEM_DEVICE_ONLY;
				return true;
			}
		} else {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Failed to perform hsa_agent_get_info: %s\n",
				ofi_hsa_status_to_string(hsa_ret));
		}
	} else {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform hsa_amd_pointer_info: %s\n",
			ofi_hsa_status_to_string(hsa_ret));
	}

	return false;
}

static int rocr_hmem_dl_init(void)
{
#if ENABLE_ROCR_DLOPEN
	/* Assume if dlopen fails, the ROCR library could not be found. Do not
	 * treat this as an error.
	 */
	rocr_handle = dlopen("libhsa-runtime64.so", RTLD_NOW);
	if (!rocr_handle) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Unable to dlopen libhsa-runtime64.so\n");
		return -FI_ENOSYS;
	}

	rocr_ops.hsa_memory_copy = dlsym(rocr_handle, "hsa_memory_copy");
	if (!rocr_ops.hsa_memory_copy) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_memory_copy\n");
		goto err;
	}

	rocr_ops.hsa_amd_pointer_info = dlsym(rocr_handle,
					      "hsa_amd_pointer_info");
	if (!rocr_ops.hsa_amd_pointer_info) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_amd_pointer_info\n");
		goto err;
	}

	rocr_ops.hsa_init = dlsym(rocr_handle, "hsa_init");
	if (!rocr_ops.hsa_init) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find hsa_init\n");
		goto err;
	}

	rocr_ops.hsa_shut_down = dlsym(rocr_handle, "hsa_shut_down");
	if (!rocr_ops.hsa_shut_down) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_shut_down\n");
		goto err;
	}

	rocr_ops.hsa_status_string = dlsym(rocr_handle, "hsa_status_string");
	if (!rocr_ops.hsa_status_string) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_status_string\n");
		goto err;
	}

	rocr_ops.hsa_amd_dereg_dealloc_cb =
		dlsym(rocr_handle, "hsa_amd_deregister_deallocation_callback");
	if (!rocr_ops.hsa_amd_dereg_dealloc_cb) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_amd_deregister_deallocation_callback\n");
		goto err;
	}

	rocr_ops.hsa_amd_reg_dealloc_cb =
		dlsym(rocr_handle, "hsa_amd_register_deallocation_callback");
	if (!rocr_ops.hsa_amd_reg_dealloc_cb) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_amd_register_deallocation_callback\n");
		goto err;
	}

	rocr_ops.hsa_amd_memory_lock = dlsym(rocr_handle,
					     "hsa_amd_memory_lock");
	if (!rocr_ops.hsa_amd_memory_lock) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_amd_memory_lock\n");
		goto err;
	}

	rocr_ops.hsa_amd_memory_unlock = dlsym(rocr_handle,
					       "hsa_amd_memory_unlock");
	if (!rocr_ops.hsa_amd_memory_lock) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_amd_memory_unlock\n");
		goto err;
	}

	rocr_ops.hsa_agent_get_info = dlsym(rocr_handle, "hsa_agent_get_info");
	if (!rocr_ops.hsa_agent_get_info) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find hsa_agent_get_info\n");
		goto err;
	}

	return FI_SUCCESS;

err:
	dlclose(rocr_handle);

	return -FI_ENODATA;
#else
	return FI_SUCCESS;
#endif /* ENABLE_ROCR_DLOPEN */
}

static void rocr_hmem_dl_cleanup(void)
{
#if ENABLE_ROCR_DLOPEN
	dlclose(rocr_handle);
#endif
}

int rocr_hmem_init(void)
{
	hsa_status_t hsa_ret;
	int ret;
	int log_level;

	ret = rocr_hmem_dl_init();
	if (ret != FI_SUCCESS)
		return ret;

	hsa_ret = ofi_hsa_init();
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return FI_SUCCESS;

	/* Treat HSA_STATUS_ERROR_OUT_OF_RESOURCES as ROCR not being supported
	 * instead of an error. This ROCR error is typically returned if no
	 * devices are supported.
	 */
	if (hsa_ret == HSA_STATUS_ERROR_OUT_OF_RESOURCES) {
		log_level = FI_LOG_INFO;
		ret = -FI_ENOSYS;
	} else {
		log_level = FI_LOG_WARN;
		ret = -FI_EIO;
	}

	FI_LOG(&core_prov, log_level, FI_LOG_CORE,
	       "Failed to perform hsa_init: %s\n",
	       ofi_hsa_status_to_string(hsa_ret));

	rocr_hmem_dl_cleanup();

	return ret;
}

int rocr_hmem_cleanup(void)
{
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_shut_down();
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform hsa_shut_down: %s\n",
			ofi_hsa_status_to_string(hsa_ret));
		return -FI_ENODATA;
	}

	rocr_hmem_dl_cleanup();

	return FI_SUCCESS;
}

int rocr_host_register(void *ptr, size_t size)
{
	hsa_status_t hsa_ret;
	void *tmp;

	hsa_ret = ofi_hsa_amd_memory_lock(ptr, size, NULL, 0, &tmp);
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return FI_SUCCESS;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform hsa_amd_memory_lock: %s\n",
		ofi_hsa_status_to_string(hsa_ret));

	return -FI_EIO;
}

int rocr_host_unregister(void *ptr)
{
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_amd_memory_unlock(ptr);
	if (hsa_ret == HSA_STATUS_SUCCESS)
		return FI_SUCCESS;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform hsa_amd_memory_unlock: %s\n",
		ofi_hsa_status_to_string(hsa_ret));

	return -FI_EIO;
}

int rocr_get_base_addr(const void *ptr, void **base, size_t *size)
{
	hsa_amd_pointer_info_t hsa_info = {
		.size = sizeof(hsa_info),
	};
	hsa_status_t hsa_ret;

	hsa_ret = ofi_hsa_amd_pointer_info((void *)ptr, &hsa_info, NULL, NULL,
					   NULL);
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform hsa_amd_pointer_info: %s\n",
			ofi_hsa_status_to_string(hsa_ret));
		return -FI_EIO;
	}

	*base = hsa_info.agentBaseAddress;
	*size = hsa_info.sizeInBytes;

	return FI_SUCCESS;
}

#else

int rocr_copy_from_dev(uint64_t device, void *dest, const void *src,
		       size_t size)
{
	return -FI_ENOSYS;
}

int rocr_copy_to_dev(uint64_t device, void *dest, const void *src,
		     size_t size)
{
	return -FI_ENOSYS;
}

int rocr_hmem_init(void)
{
	return -FI_ENOSYS;
}

int rocr_hmem_cleanup(void)
{
	return -FI_ENOSYS;
}

bool rocr_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags)
{
	return false;
}

int rocr_host_register(void *ptr, size_t size)
{
	return -FI_ENOSYS;
}

int rocr_host_unregister(void *ptr)
{
	return -FI_ENOSYS;
}

int rocr_get_base_addr(const void *ptr, void **base, size_t *size)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_ROCR */
