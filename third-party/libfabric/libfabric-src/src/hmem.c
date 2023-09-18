/*
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 * (C) Copyright 2020-2021 Intel Corporation. All rights reserved.
 * (C) Copyright 2021 Amazon.com, Inc. or its affiliates.
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
#include "ofi_iov.h"

bool ofi_hmem_disable_p2p = false;

struct ofi_hmem_ops hmem_ops[] = {
	[FI_HMEM_SYSTEM] = {
		.initialized = true,
		.init = ofi_hmem_init_noop,
		.cleanup = ofi_hmem_cleanup_noop,
		.copy_to_hmem = ofi_memcpy,
		.copy_from_hmem = ofi_memcpy,
		.get_handle = ofi_hmem_no_get_handle,
		.open_handle = ofi_hmem_no_open_handle,
		.close_handle = ofi_hmem_no_close_handle,
		.host_register = ofi_hmem_host_register_noop,
		.host_unregister = ofi_hmem_host_unregister_noop,
		.get_base_addr = ofi_hmem_no_base_addr,
		.is_ipc_enabled = ofi_hmem_no_is_ipc_enabled,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
	},
	[FI_HMEM_CUDA] = {
		.initialized = false,
		.init = cuda_hmem_init,
		.cleanup = cuda_hmem_cleanup,
		.copy_to_hmem = cuda_copy_to_dev,
		.copy_from_hmem = cuda_copy_from_dev,
		.is_addr_valid = cuda_is_addr_valid,
		.get_handle = cuda_get_handle,
		.open_handle = cuda_open_handle,
		.close_handle = cuda_close_handle,
		.host_register = cuda_host_register,
		.host_unregister = cuda_host_unregister,
		.get_base_addr = cuda_get_base_addr,
		.is_ipc_enabled = cuda_is_ipc_enabled,
		.get_ipc_handle_size = cuda_get_ipc_handle_size,
	},
	[FI_HMEM_ROCR] = {
		.initialized = false,
		.init = rocr_hmem_init,
		.cleanup = rocr_hmem_cleanup,
		.copy_to_hmem = rocr_copy_to_dev,
		.copy_from_hmem = rocr_copy_from_dev,
		.is_addr_valid = rocr_is_addr_valid,
		.get_handle = ofi_hmem_no_get_handle,
		.open_handle = ofi_hmem_no_open_handle,
		.close_handle = ofi_hmem_no_close_handle,
		.host_register = rocr_host_register,
		.host_unregister = rocr_host_unregister,
		.get_base_addr = rocr_get_base_addr,
		.is_ipc_enabled = ofi_hmem_no_is_ipc_enabled,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
	},
	[FI_HMEM_ZE] = {
		.initialized = false,
		.init = ze_hmem_init,
		.cleanup = ze_hmem_cleanup,
		.copy_to_hmem = ze_hmem_copy,
		.copy_from_hmem = ze_hmem_copy,
		.is_addr_valid = ze_hmem_is_addr_valid,
		.get_handle = ze_hmem_get_handle,
		.open_handle = ze_hmem_open_handle,
		.close_handle = ze_hmem_close_handle,
		.host_register = ze_hmem_host_register,
		.host_unregister = ze_hmem_host_unregister,
		.get_base_addr = ze_hmem_get_base_addr,
		.is_ipc_enabled = ze_hmem_p2p_enabled,
		.get_ipc_handle_size = ze_hmem_get_ipc_handle_size,
	},
	[FI_HMEM_NEURON] = {
		.initialized = false,
		.init = neuron_hmem_init,
		.cleanup = neuron_hmem_cleanup,
		.copy_to_hmem = neuron_copy_to_dev,
		.copy_from_hmem = neuron_copy_from_dev,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
	},
	[FI_HMEM_SYNAPSEAI] = {
		.initialized = false,
		.init = synapseai_init,
		.cleanup = synapseai_cleanup,
		.copy_to_hmem = synapseai_copy_to_hmem,
		.copy_from_hmem = synapseai_copy_from_hmem,
		.get_handle = synapseai_get_handle,
		.open_handle = synapseai_open_handle,
		.close_handle = synapseai_close_handle,
		.host_register = synapseai_host_register,
		.host_unregister = synapseai_host_unregister,
		.get_base_addr = synapseai_get_base_addr,
		.is_ipc_enabled = synapseai_is_ipc_enabled,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
	},
};

static inline int ofi_copy_to_hmem(enum fi_hmem_iface iface, uint64_t device,
				   void *dest, const void *src, size_t size)
{
	return hmem_ops[iface].copy_to_hmem(device, dest, src, size);
}

static inline int ofi_copy_from_hmem(enum fi_hmem_iface iface, uint64_t device,
				     void *dest, const void *src, size_t size)
{
	return hmem_ops[iface].copy_from_hmem(device, dest, src, size);
}

static ssize_t ofi_copy_hmem_iov_buf(enum fi_hmem_iface hmem_iface, uint64_t device,
				     const struct iovec *hmem_iov,
				     size_t hmem_iov_count,
				     uint64_t hmem_iov_offset, void *buf,
				     size_t size, int dir)
{
	uint64_t done = 0, len;
	char *hmem_buf;
	size_t i;
	int ret;

	for (i = 0; i < hmem_iov_count && size; i++) {
		len = hmem_iov[i].iov_len;

		if (hmem_iov_offset > len) {
			hmem_iov_offset -= len;
			continue;
		}

		hmem_buf = (char *)hmem_iov[i].iov_base + hmem_iov_offset;
		len -= hmem_iov_offset;
		hmem_iov_offset = 0;

		len = MIN(len, size);
		if (!len)
			continue;

		if (dir == OFI_COPY_BUF_TO_IOV)
			ret = ofi_copy_to_hmem(hmem_iface, device, hmem_buf,
					       (char *)buf + done, len);
		else
			ret = ofi_copy_from_hmem(hmem_iface, device,
						 (char *)buf + done, hmem_buf,
						 len);

		if (ret)
			return ret;

		size -= len;
		done += len;
	}
	return done;
}

ssize_t ofi_copy_from_hmem_iov(void *dest, size_t size,
			       enum fi_hmem_iface hmem_iface, uint64_t device,
			       const struct iovec *hmem_iov,
			       size_t hmem_iov_count,
			       uint64_t hmem_iov_offset)
{
	return ofi_copy_hmem_iov_buf(hmem_iface, device, hmem_iov,
				     hmem_iov_count, hmem_iov_offset,
				     dest, size, OFI_COPY_IOV_TO_BUF);
}

ssize_t ofi_copy_to_hmem_iov(enum fi_hmem_iface hmem_iface, uint64_t device,
			     const struct iovec *hmem_iov,
			     size_t hmem_iov_count, uint64_t hmem_iov_offset,
			     const void *src, size_t size)
{
	return ofi_copy_hmem_iov_buf(hmem_iface, device, hmem_iov,
				     hmem_iov_count, hmem_iov_offset,
				     (void *) src, size, OFI_COPY_BUF_TO_IOV);
}

int ofi_hmem_get_handle(enum fi_hmem_iface iface, void *base_addr, void **handle)
{
	return hmem_ops[iface].get_handle(base_addr, handle);
}

int ofi_hmem_open_handle(enum fi_hmem_iface iface, void **handle,
			 uint64_t device, void **mapped_addr)
{
	return hmem_ops[iface].open_handle(handle, device, mapped_addr);
}

int ofi_hmem_close_handle(enum fi_hmem_iface iface, void *mapped_addr)
{
	return hmem_ops[iface].close_handle(mapped_addr);
}

int ofi_hmem_get_base_addr(enum fi_hmem_iface iface, const void *addr,
			   void **base_addr, size_t *base_length)
{
	return hmem_ops[iface].get_base_addr(addr, base_addr, base_length);
}

bool ofi_hmem_is_initialized(enum fi_hmem_iface iface)
{
	return hmem_ops[iface].initialized;
}

void ofi_hmem_init(void)
{
	int iface, ret;
	int disable_p2p = 0;

	for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++) {
		ret = hmem_ops[iface].init();
		if (ret != FI_SUCCESS) {
			if (ret == -FI_ENOSYS)
				FI_INFO(&core_prov, FI_LOG_CORE,
					"Hmem iface %s not supported\n",
					fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			else
				FI_WARN(&core_prov, FI_LOG_CORE,
					"Failed to initialize hmem iface %s: %s\n",
					fi_tostr(&iface, FI_TYPE_HMEM_IFACE),
					fi_strerror(-ret));
		} else {
			hmem_ops[iface].initialized = true;
		}
	}

	fi_param_define(NULL, "hmem_disable_p2p", FI_PARAM_BOOL,
			"Disable peer to peer support between device memory and"
			" network devices. (default: no).");

	if (!fi_param_get_bool(NULL, "hmem_disable_p2p", &disable_p2p)) {
		if (disable_p2p == 1)
			ofi_hmem_disable_p2p = true;
	}
}

void ofi_hmem_cleanup(void)
{
	enum fi_hmem_iface iface;

	for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++) {
		if (ofi_hmem_is_initialized(iface))
			hmem_ops[iface].cleanup();
	}
}

enum fi_hmem_iface ofi_get_hmem_iface(const void *addr, uint64_t *device,
				      uint64_t *flags)
{
	int iface;

	/* Since a is_addr_valid function is not implemented for FI_HMEM_SYSTEM,
	 * HMEM iface is skipped. In addition, if no other HMEM ifaces claim the
	 * address as valid, it is assumed the address is FI_HMEM_SYSTEM.
	 */
	for (iface = ARRAY_SIZE(hmem_ops) - 1; iface > FI_HMEM_SYSTEM;
	     iface--) {
		if (ofi_hmem_is_initialized(iface) &&
		    hmem_ops[iface].is_addr_valid(addr, device, flags))
			return iface;
	}

	return FI_HMEM_SYSTEM;
}

int ofi_hmem_host_register(void *addr, size_t size)
{
	int iface, ret;

	for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++) {
		if (!ofi_hmem_is_initialized(iface))
			continue;

		ret = hmem_ops[iface].host_register(addr, size);
		if (ret != FI_SUCCESS)
			goto err;
	}

	return FI_SUCCESS;

err:
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to register host memory with hmem iface %s: %s\n",
		fi_tostr(&iface, FI_TYPE_HMEM_IFACE),
		fi_strerror(-ret));

	for (iface--; iface >= 0; iface--) {
		if (!ofi_hmem_is_initialized(iface))
			continue;

		hmem_ops[iface].host_unregister(addr);
	}

	return ret;
}

int ofi_hmem_host_unregister(void *addr)
{
	int iface, ret;

	for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++) {
		if (!ofi_hmem_is_initialized(iface))
			continue;

		ret = hmem_ops[iface].host_unregister(addr);
		if (ret != FI_SUCCESS)
			goto err;
	}

	return FI_SUCCESS;

err:
	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to unregister host memory with hmem iface %s: %s\n",
		fi_tostr(&iface, FI_TYPE_HMEM_IFACE),
		fi_strerror(-ret));

	return ret;
}

bool ofi_hmem_is_ipc_enabled(enum fi_hmem_iface iface)
{
	return hmem_ops[iface].is_ipc_enabled();
}

size_t ofi_hmem_get_ipc_handle_size(enum fi_hmem_iface iface)
{
	int ret;
	size_t size;

	ret = hmem_ops[iface].get_ipc_handle_size(&size);
	if (ret == FI_SUCCESS)
		return size;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to get ipc handle size with hmem iface %s: %s\n",
		fi_tostr(&iface, FI_TYPE_HMEM_IFACE),
		fi_strerror(-ret));
	return 0;
}
