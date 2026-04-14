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

static int ofi_hmem_system_base_addr(const void *addr, size_t len,
				     void **base_addr, size_t *base_length)
{
	ssize_t page_size;
	size_t page_mask;
	void *start;
	void *end;

	page_size = ofi_get_addr_page_size(addr);
	if (page_size < 0)
		return page_size;

	page_mask = page_size - 1;
	start = (void *) ((uintptr_t) addr  & ~page_mask);

	end = (void *) ((uintptr_t) addr + len);
	page_size = ofi_get_addr_page_size(end);
	if (page_size < 0)
		return page_size;

	page_mask = page_size - 1;
	end = (void *) (((uintptr_t) end + page_mask) & ~page_mask);

	*base_addr = start;
	*base_length = (uintptr_t) end - (uintptr_t) start;

	assert(*base_length >= len);
	assert((uintptr_t) *base_addr <= (uintptr_t) addr);

	return FI_SUCCESS;
}

static int ofi_hmem_no_dev_register(const void *addr, size_t size,
				    uint64_t *handle)
{
	return -FI_ENOSYS;
}

static int ofi_hmem_no_dev_unregister(uint64_t handle)
{
	return -FI_ENOSYS;
}

static int ofi_hmem_no_dev_reg_copy_to_hmem(uint64_t handle, void *dest,
					    const void *src, size_t size)
{
	return -FI_ENOSYS;
}

static int ofi_hmem_no_dev_reg_copy_from_hmem(uint64_t handle, void *dest,
					      const void *src, size_t size)
{
	return -FI_ENOSYS;
}

static int ofi_hmem_system_dev_register(const void *addr, size_t size,
					uint64_t *handle)
{
	*handle = (uint64_t) addr;
	return FI_SUCCESS;
}

static int ofi_hmem_system_dev_unregister(uint64_t handle)
{
	return FI_SUCCESS;
}

static int ofi_hmem_system_dev_reg_copy(uint64_t handle, void *dest,
					const void *src, size_t size)
{
	memcpy(dest, src, size);
	return FI_SUCCESS;
}

struct ofi_hmem_ops hmem_ops[] = {
	[FI_HMEM_SYSTEM] = {
		.initialized = true,
		.init = ofi_hmem_init_noop,
		.cleanup = ofi_hmem_cleanup_noop,
		.copy_to_hmem = ofi_memcpy,
		.copy_from_hmem = ofi_memcpy,
		.create_async_copy_event = ofi_no_create_async_copy_event,
		.free_async_copy_event = ofi_no_free_async_copy_event,
		.async_copy_to_hmem = ofi_no_async_memcpy,
		.async_copy_from_hmem = ofi_no_async_memcpy,
		.async_copy_query = ofi_no_async_copy_query,
		.get_handle = ofi_hmem_no_get_handle,
		.open_handle = ofi_hmem_no_open_handle,
		.close_handle = ofi_hmem_no_close_handle,
		.host_register = ofi_hmem_host_register_noop,
		.host_unregister = ofi_hmem_host_unregister_noop,
		.get_base_addr = ofi_hmem_system_base_addr,
		.is_ipc_enabled = ofi_hmem_no_is_ipc_enabled,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
		.dev_register = ofi_hmem_system_dev_register,
		.dev_unregister = ofi_hmem_system_dev_unregister,
		.dev_reg_copy_to_hmem = ofi_hmem_system_dev_reg_copy,
		.dev_reg_copy_from_hmem = ofi_hmem_system_dev_reg_copy,
		.get_dmabuf_fd = ofi_hmem_no_get_dmabuf_fd,
		.put_dmabuf_fd = ofi_hmem_no_put_dmabuf_fd,
	},
	[FI_HMEM_CUDA] = {
		.initialized = false,
		.init = cuda_hmem_init,
		.cleanup = cuda_hmem_cleanup,
		.copy_to_hmem = cuda_copy_to_dev,
		.copy_from_hmem = cuda_copy_from_dev,
		.create_async_copy_event = ofi_no_create_async_copy_event,
		.free_async_copy_event = ofi_no_free_async_copy_event,
		.async_copy_to_hmem = ofi_no_async_memcpy,
		.async_copy_from_hmem = ofi_no_async_memcpy,
		.async_copy_query = ofi_no_async_copy_query,
		.is_addr_valid = cuda_is_addr_valid,
		.get_handle = cuda_get_handle,
		.open_handle = cuda_open_handle,
		.close_handle = cuda_close_handle,
		.host_register = cuda_host_register,
		.host_unregister = cuda_host_unregister,
		.get_base_addr = cuda_get_base_addr,
		.is_ipc_enabled = cuda_is_ipc_enabled,
		.get_ipc_handle_size = cuda_get_ipc_handle_size,
		.dev_register = cuda_dev_register,
		.dev_unregister = cuda_dev_unregister,
		.dev_reg_copy_to_hmem = cuda_dev_reg_copy_to_hmem,
		.dev_reg_copy_from_hmem = cuda_dev_reg_copy_from_hmem,
		.get_dmabuf_fd = cuda_get_dmabuf_fd,
		.put_dmabuf_fd = cuda_put_dmabuf_fd,
	},
	[FI_HMEM_ROCR] = {
		.initialized = false,
		.init = rocr_hmem_init,
		.cleanup = rocr_hmem_cleanup,
		.copy_to_hmem = rocr_copy_to_dev,
		.copy_from_hmem = rocr_copy_from_dev,
		.create_async_copy_event = rocr_create_async_copy_event,
		.free_async_copy_event = rocr_free_async_copy_event,
		.async_copy_to_hmem = rocr_async_copy_to_dev,
		.async_copy_from_hmem = rocr_async_copy_from_dev,
		.async_copy_query = rocr_async_copy_query,
		.is_addr_valid = rocr_is_addr_valid,
		.get_handle = rocr_get_handle,
		.open_handle = rocr_open_handle,
		.close_handle = rocr_close_handle,
		.host_register = rocr_host_register,
		.host_unregister = rocr_host_unregister,
		.get_base_addr = rocr_get_base_addr,
		.is_ipc_enabled = rocr_is_ipc_enabled,
		.get_ipc_handle_size = rocr_get_ipc_handle_size,
		.dev_register = rocr_dev_register,
		.dev_unregister = rocr_dev_unregister,
		.dev_reg_copy_to_hmem = rocr_dev_reg_copy_to_hmem,
		.dev_reg_copy_from_hmem = rocr_dev_reg_copy_from_hmem,
		.get_dmabuf_fd = rocr_hmem_get_dmabuf_fd,
		.put_dmabuf_fd = rocr_hmem_put_dmabuf_fd,
	},
	[FI_HMEM_ZE] = {
		.initialized = false,
		.init = ze_hmem_init,
		.cleanup = ze_hmem_cleanup,
		.copy_to_hmem = ze_hmem_copy,
		.copy_from_hmem = ze_hmem_copy,
		.create_async_copy_event = ofi_no_create_async_copy_event,
		.free_async_copy_event = ofi_no_free_async_copy_event,
		.async_copy_to_hmem = ofi_no_async_memcpy,
		.async_copy_from_hmem = ofi_no_async_memcpy,
		.async_copy_query = ofi_no_async_copy_query,
		.is_addr_valid = ze_hmem_is_addr_valid,
		.get_handle = ze_hmem_get_handle,
		.open_handle = ze_hmem_open_handle,
		.close_handle = ze_hmem_close_handle,
		.host_register = ze_hmem_host_register,
		.host_unregister = ze_hmem_host_unregister,
		.get_base_addr = ze_hmem_get_base_addr,
		.is_ipc_enabled = ze_hmem_p2p_enabled,
		.get_ipc_handle_size = ze_hmem_get_ipc_handle_size,
		.dev_register = ze_dev_register,
		.dev_unregister = ze_dev_unregister,
		.dev_reg_copy_to_hmem = ze_dev_reg_copy_to_hmem,
		.dev_reg_copy_from_hmem = ze_dev_reg_copy_from_hmem,
		.get_dmabuf_fd = ze_hmem_get_dmabuf_fd,
		.put_dmabuf_fd = ofi_hmem_no_put_dmabuf_fd,
	},
	[FI_HMEM_NEURON] = {
		.initialized = false,
		.init = neuron_hmem_init,
		.cleanup = neuron_hmem_cleanup,
		.copy_to_hmem = neuron_copy_to_dev,
		.copy_from_hmem = neuron_copy_from_dev,
		.create_async_copy_event = ofi_no_create_async_copy_event,
		.free_async_copy_event = ofi_no_free_async_copy_event,
		.async_copy_to_hmem = ofi_no_async_memcpy,
		.async_copy_from_hmem = ofi_no_async_memcpy,
		.async_copy_query = ofi_no_async_copy_query,
		.get_handle = ofi_hmem_no_get_handle,
		.open_handle = ofi_hmem_no_open_handle,
		.close_handle = ofi_hmem_no_close_handle,
		.host_register = neuron_host_register,
		.host_unregister = neuron_host_unregister,
		.get_base_addr = ofi_hmem_no_base_addr,
		.is_ipc_enabled = ofi_hmem_no_is_ipc_enabled,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
		.dev_register = ofi_hmem_no_dev_register,
		.dev_unregister = ofi_hmem_no_dev_unregister,
		.dev_reg_copy_to_hmem = ofi_hmem_no_dev_reg_copy_to_hmem,
		.dev_reg_copy_from_hmem = ofi_hmem_no_dev_reg_copy_from_hmem,
		.get_dmabuf_fd = neuron_get_dmabuf_fd,
		.put_dmabuf_fd = neuron_put_dmabuf_fd,
	},
	[FI_HMEM_SYNAPSEAI] = {
		.initialized = false,
		.init = synapseai_init,
		.cleanup = synapseai_cleanup,
		.copy_to_hmem = synapseai_copy_to_hmem,
		.copy_from_hmem = synapseai_copy_from_hmem,
		.create_async_copy_event = ofi_no_create_async_copy_event,
		.free_async_copy_event = ofi_no_free_async_copy_event,
		.async_copy_to_hmem = ofi_no_async_memcpy,
		.async_copy_from_hmem = ofi_no_async_memcpy,
		.async_copy_query = ofi_no_async_copy_query,
		.get_handle = ofi_hmem_no_get_handle,
		.open_handle = ofi_hmem_no_open_handle,
		.close_handle = ofi_hmem_no_close_handle,
		.host_register = synapseai_host_register,
		.host_unregister = synapseai_host_unregister,
		.get_base_addr = ofi_hmem_no_base_addr,
		.is_ipc_enabled = ofi_hmem_no_is_ipc_enabled,
		.get_ipc_handle_size = ofi_hmem_no_get_ipc_handle_size,
		.dev_register = ofi_hmem_no_dev_register,
		.dev_unregister = ofi_hmem_no_dev_unregister,
		.dev_reg_copy_to_hmem = ofi_hmem_no_dev_reg_copy_to_hmem,
		.dev_reg_copy_from_hmem = ofi_hmem_no_dev_reg_copy_from_hmem,
		.get_dmabuf_fd = synapseai_get_dmabuf_fd,
		.put_dmabuf_fd = ofi_hmem_no_put_dmabuf_fd,
	},
};

bool ofi_hmem_any_ipc_enabled(void)
{
	int iface;

	for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++)
		if (ofi_hmem_is_initialized(iface) && ofi_hmem_is_ipc_enabled(iface))
			return true;
	return false;
}

int ofi_create_async_copy_event(enum fi_hmem_iface iface, uint64_t device,
				ofi_hmem_async_event_t *event)
{
	return hmem_ops[iface].create_async_copy_event(device, event);
}

int ofi_free_async_copy_event(enum fi_hmem_iface iface, uint64_t device,
			      ofi_hmem_async_event_t event)
{
	return hmem_ops[iface].free_async_copy_event(device, event);
}

static inline int ofi_async_copy_to_hmem(enum fi_hmem_iface iface,
			uint64_t device, void *dest, const void *src,
			size_t size, ofi_hmem_async_event_t event)
{
	return hmem_ops[iface].async_copy_to_hmem(device, dest, src, size,
						  event);
}

static inline int ofi_async_copy_from_hmem(enum fi_hmem_iface iface,
			uint64_t device, void *dest, const void *src,
			size_t size, ofi_hmem_async_event_t event)
{
	return hmem_ops[iface].async_copy_from_hmem(device, dest, src, size,
						    event);
}

static ssize_t
ofi_async_copy_hmem_iov_buf(enum fi_hmem_iface hmem_iface, uint64_t device,
			    const struct iovec *hmem_iov,
			    size_t hmem_iov_count,
			    size_t hmem_iov_offset, void *buf,
			    size_t size, int dir, ofi_hmem_async_event_t event)
{
	uint64_t done = 0, len;
	char *hmem_buf;
	size_t i;
	int ret;

	if (!event || hmem_iov_count > MAX_NUM_ASYNC_OP)
		return -FI_EINVAL;

	for (i = 0; i < hmem_iov_count && size; i++) {
		len = ofi_iov_bytes_to_copy(&hmem_iov[i], &size,
					    &hmem_iov_offset, &hmem_buf);
		if (!len)
			continue;

		/* this will initiate all iov copies under the same event.
		 * Which means completion will occur when all copies have
		 * completed.
		 */
		if (dir == OFI_COPY_BUF_TO_IOV)
			ret = ofi_async_copy_to_hmem(hmem_iface, device, hmem_buf,
						(char *)buf + done, len, event);
		else
			ret = ofi_async_copy_from_hmem(hmem_iface, device,
						(char *)buf + done, hmem_buf,
						len, event);
		if (ret)
			return ret;

		done += len;
	}
	return done;
}

static ssize_t ofi_copy_hmem_iov_buf(enum fi_hmem_iface hmem_iface, uint64_t device,
				     const struct iovec *hmem_iov,
				     size_t hmem_iov_count,
				     size_t hmem_iov_offset, void *buf,
				     size_t size, int dir)
{
	uint64_t done = 0, len;
	char *hmem_buf;
	size_t i;
	int ret;

	for (i = 0; i < hmem_iov_count && size; i++) {
		len = ofi_iov_bytes_to_copy(&hmem_iov[i], &size,
					    &hmem_iov_offset, &hmem_buf);
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

		done += len;
	}
	return done;
}

static ssize_t ofi_dev_reg_copy_hmem_iov_buf(enum fi_hmem_iface hmem_iface, uint64_t handle,
					      const struct iovec *hmem_iov,
					      size_t hmem_iov_count,
					      size_t hmem_iov_offset, void *buf,
					      size_t size, int dir)
{
	uint64_t done = 0, len;
	char *hmem_buf;
	size_t i;
	int ret;

	for (i = 0; i < hmem_iov_count && size; i++) {
		len = ofi_iov_bytes_to_copy(&hmem_iov[i], &size,
					    &hmem_iov_offset, &hmem_buf);
		if (!len)
			continue;

		if (dir == OFI_COPY_BUF_TO_IOV)
			ret = ofi_hmem_dev_reg_copy_to_hmem(hmem_iface, handle, hmem_buf,
							    (char *)buf + done, len);
		else
			ret = ofi_hmem_dev_reg_copy_from_hmem(hmem_iface, handle,
							      (char *)buf + done, hmem_buf, len);
		if (ret)
			return ret;

		done += len;
	}
	return done;
}

static ssize_t ofi_copy_mr_iov(struct ofi_mr **mr, const struct iovec *iov,
		size_t iov_count, size_t offset, void *buf,
		size_t size, int dir)
{
	uint64_t done = 0, len;
	uint64_t hmem_iface, hmem_device, hmem_flags;
	void *hmem_data;
	char *hmem_buf;
	size_t i;
	int ret;

	for (i = 0; i < iov_count && size; i++) {
		len = ofi_iov_bytes_to_copy(&iov[i], &size, &offset, &hmem_buf);
		if (!len)
			continue;

		if (mr && mr[i]) {
			hmem_iface = mr[i]->iface;
			hmem_flags = mr[i]->flags;
			hmem_device = mr[i]->device;
			hmem_data = mr[i]->hmem_data;
		} else {
			hmem_iface = FI_HMEM_SYSTEM;
			hmem_flags = 0;
			hmem_device = 0;
			hmem_data = NULL;
		}

		if (hmem_flags & OFI_HMEM_DATA_DEV_REG_HANDLE) {
			if (dir == OFI_COPY_BUF_TO_IOV)
				ofi_hmem_dev_reg_copy_to_hmem(
					hmem_iface, (uint64_t) hmem_data,
					hmem_buf, (char *) buf + done, len);
			else
				ofi_hmem_dev_reg_copy_from_hmem(
					hmem_iface, (uint64_t) hmem_data,
					(char *) buf + done, hmem_buf, len);
			ret = FI_SUCCESS;
		} else if (dir == OFI_COPY_BUF_TO_IOV)
			ret = ofi_copy_to_hmem(hmem_iface, hmem_device, hmem_buf,
						(char *)buf + done, len);
		else
			ret = ofi_copy_from_hmem(hmem_iface, hmem_device,
						(char *)buf + done, hmem_buf,
						len);
		if (ret)
			return ret;

		done += len;
	}
	return done;
}

ssize_t ofi_copy_from_mr_iov(void *dest, size_t size, struct ofi_mr **mr,
			     const struct iovec *iov, size_t iov_count,
			     uint64_t iov_offset)
{
	return ofi_copy_mr_iov(mr, iov, iov_count, iov_offset, dest, size,
			       OFI_COPY_IOV_TO_BUF);
}

ssize_t ofi_copy_to_mr_iov(struct ofi_mr **mr, const struct iovec *iov,
		       size_t iov_count, uint64_t iov_offset,
		       const void *src, size_t size)
{
	return ofi_copy_mr_iov(mr, iov, iov_count, iov_offset,
			       (void *) src, size, OFI_COPY_BUF_TO_IOV);
}


ssize_t ofi_async_copy_from_hmem_iov(void *dest, size_t size,
			       enum fi_hmem_iface hmem_iface, uint64_t device,
			       const struct iovec *hmem_iov,
			       size_t hmem_iov_count,
			       uint64_t hmem_iov_offset,
			       ofi_hmem_async_event_t event)
{
	return ofi_async_copy_hmem_iov_buf(hmem_iface, device, hmem_iov,
				hmem_iov_count, hmem_iov_offset,
				dest, size, OFI_COPY_IOV_TO_BUF, event);
}

ssize_t ofi_async_copy_to_hmem_iov(enum fi_hmem_iface hmem_iface, uint64_t device,
			     const struct iovec *hmem_iov,
			     size_t hmem_iov_count, uint64_t hmem_iov_offset,
			     const void *src, size_t size,
			     ofi_hmem_async_event_t event)
{
	return ofi_async_copy_hmem_iov_buf(hmem_iface, device, hmem_iov,
				hmem_iov_count, hmem_iov_offset,
				(void *) src, size, OFI_COPY_BUF_TO_IOV,
				event);
}

int ofi_async_copy_query(enum fi_hmem_iface iface,
			 ofi_hmem_async_event_t event)
{
	return hmem_ops[iface].async_copy_query(event);
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

ssize_t ofi_dev_reg_copy_from_hmem_iov(void *dest, size_t size,
				       enum fi_hmem_iface hmem_iface, uint64_t handle,
				       const struct iovec *hmem_iov,
				       size_t hmem_iov_count,
				       uint64_t hmem_iov_offset)
{
	return ofi_dev_reg_copy_hmem_iov_buf(hmem_iface, handle, hmem_iov,
					     hmem_iov_count, hmem_iov_offset,
					     dest, size, OFI_COPY_IOV_TO_BUF);
}

ssize_t ofi_dev_reg_copy_to_hmem_iov(enum fi_hmem_iface hmem_iface, uint64_t handle,
				     const struct iovec *hmem_iov,
				     size_t hmem_iov_count, uint64_t hmem_iov_offset,
				     const void *src, size_t size)
{
	return ofi_dev_reg_copy_hmem_iov_buf(hmem_iface, handle, hmem_iov,
					     hmem_iov_count, hmem_iov_offset,
					     (void *) src, size, OFI_COPY_BUF_TO_IOV);
}

int ofi_hmem_get_handle(enum fi_hmem_iface iface, void *base_addr,
			size_t size, void **handle)
{
	return hmem_ops[iface].get_handle(base_addr, size, handle);
}

int ofi_hmem_open_handle(enum fi_hmem_iface iface, void **handle,
			size_t size, uint64_t device, void **mapped_addr)
{
	return hmem_ops[iface].open_handle(handle, size, device,
					   mapped_addr);
}

int ofi_hmem_close_handle(enum fi_hmem_iface iface, void *mapped_addr,
			  void **handle)
{
	return hmem_ops[iface].close_handle(mapped_addr, handle);
}

int ofi_hmem_get_base_addr(enum fi_hmem_iface iface, const void *addr,
			   size_t len, void **base_addr, size_t *base_length)
{
	return hmem_ops[iface].get_base_addr(addr, len, base_addr, base_length);
}

bool ofi_hmem_is_initialized(enum fi_hmem_iface iface)
{
	return hmem_ops[iface].initialized;
}

void ofi_hmem_set_iface_filter(const char* iface_filter_str, bool* filter)
{
	int iface;
	char* entry = NULL;
	const char* token = ";";
	const char* iface_labels[ARRAY_SIZE(hmem_ops)] = {
		"system",
		"cuda",
		"rocr",
		"ze",
		"neuron",
		"synapseai"
	};
	char *iface_filter_str_copy = strdup(iface_filter_str);
	char *saveptr;

	memset(filter, false, sizeof(bool) * ARRAY_SIZE(hmem_ops));

	/* always enable system hmem interface */
	filter[FI_HMEM_SYSTEM] = true;

	entry = strtok_r(iface_filter_str_copy, token, &saveptr);
	while (entry != NULL) {
		for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++) {
			if (!strcasecmp(iface_labels[iface], entry)) {
				filter[iface] = true;
				break;
			}
		}

		if (iface >= ARRAY_SIZE(hmem_ops)) {
			FI_WARN(&core_prov, FI_LOG_CORE,
					"unknown HMEM interface specified in FI_HMEM, entry=\"%s\"\n",
					entry);
		}

		entry = strtok_r(NULL, token, &saveptr);
	}

	free(iface_filter_str_copy);
}

void ofi_hmem_init(void)
{
	int iface, ret;
	int disable_p2p = 0;
	char* hmem_filter = NULL;
	bool filter_hmem_ifaces = false;
	bool iface_filter_array[ARRAY_SIZE(hmem_ops)];

	fi_param_define(NULL, "hmem", FI_PARAM_STRING,
			"List of hmem interfaces to attempt to initialize (default: all available interfaces)");
	fi_param_get_str(NULL, "hmem", &hmem_filter);

	if (hmem_filter) {
		if (strlen(hmem_filter) != 0) {
			ofi_hmem_set_iface_filter(hmem_filter, iface_filter_array);
			filter_hmem_ifaces = true;
		} else {
			FI_WARN(&core_prov, FI_LOG_CORE,
					"zero-length FI_HMEM provided, enabling all interfaces\n");
		}
	}

	for (iface = 0; iface < ARRAY_SIZE(hmem_ops); iface++) {
		if (filter_hmem_ifaces && !iface_filter_array[iface])
			continue;

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

/* Perform addition device memory registration to enable additional, potentially
 * more optimized, memcpy operations. While optimizes will be device specific,
 * such implementations may enable load/store access to device memory across
 * PCIe.
 *
 * -FI_ENOSYS is returned if a device does not support this registration and
 * subsequent copy routines.
 */
int ofi_hmem_dev_register(enum fi_hmem_iface iface, const void *addr,
			  size_t size, uint64_t *handle)
{
	return hmem_ops[iface].dev_register(addr, size, handle);
}

int ofi_hmem_dev_unregister(enum fi_hmem_iface iface, uint64_t handle)
{
	return hmem_ops[iface].dev_unregister(handle);
}

int ofi_hmem_dev_reg_copy_to_hmem(enum fi_hmem_iface iface, uint64_t handle,
				  void *dest, const void *src, size_t size)
{
	return hmem_ops[iface].dev_reg_copy_to_hmem(handle, dest, src, size);
}

int ofi_hmem_dev_reg_copy_from_hmem(enum fi_hmem_iface iface, uint64_t handle,
				    void *dest, const void *src, size_t size)
{
	return hmem_ops[iface].dev_reg_copy_from_hmem(handle, dest, src, size);
}

int ofi_hmem_get_dmabuf_fd(enum fi_hmem_iface iface, const void *addr,
			   uint64_t size, int *fd, uint64_t *offset)
{
	return hmem_ops[iface].get_dmabuf_fd(addr, size, fd, offset);
}

int ofi_hmem_put_dmabuf_fd(enum fi_hmem_iface iface, int fd)
{
	return hmem_ops[iface].put_dmabuf_fd(fd);
}
