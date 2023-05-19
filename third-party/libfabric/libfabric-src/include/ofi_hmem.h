/*
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 * (C) Copyright 2020-2021 Intel Corporation. All rights reserved.
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

#ifndef _OFI_HMEM_H_
#define _OFI_HMEM_H_

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <rdma/fi_domain.h>
#include <stdbool.h>

extern bool ofi_hmem_disable_p2p;

#define MAX_IPC_HANDLE_SIZE	64

/*
 * This structure is part of the
 * the shm communication protocol
 * defined in ofi_shm.h.
 * Please make sure the SMR_VERSION are
 * bumped and SMR_CMD_SIZE are large
 * enough, for any changes in this
 * structure.
 */
struct ipc_info {
	uint64_t	iface;
	uint64_t	base_addr;
	uint64_t	base_length;
	uint64_t	device;
	uint64_t	offset;
	uint8_t		ipc_handle[MAX_IPC_HANDLE_SIZE];
};

#if HAVE_CUDA

#include <cuda.h>
#include <cuda_runtime.h>

/* Libfabric supported CUDA operations. */
cudaError_t ofi_cudaMemcpy(void* dst, const void* src, size_t count,
			   enum cudaMemcpyKind kind);
const char *ofi_cudaGetErrorName(cudaError_t error);
const char *ofi_cudaGetErrorString(cudaError_t error);
CUresult ofi_cuPointerGetAttribute(void *data, CUpointer_attribute attribute,
				   CUdeviceptr ptr);
cudaError_t ofi_cudaHostRegister(void *ptr, size_t size, unsigned int flags);
cudaError_t ofi_cudaHostUnregister(void *ptr);
cudaError_t ofi_cudaMalloc(void **ptr, size_t size);
cudaError_t ofi_cudaFree(void *ptr);

#endif /* HAVE_CUDA */

#if HAVE_ROCR

#include <hsa/hsa_ext_amd.h>

/* Libfabric support ROCr operations. */

hsa_status_t ofi_hsa_memory_copy(void *dst, const void *src, size_t size);
hsa_status_t ofi_hsa_amd_pointer_info(void *ptr, hsa_amd_pointer_info_t *info,
				      void *(*alloc)(size_t),
				      uint32_t *num_agents_accessible,
				      hsa_agent_t **accessible);
hsa_status_t ofi_hsa_init(void);
hsa_status_t ofi_hsa_shut_down(void);
hsa_status_t ofi_hsa_status_string(hsa_status_t status,
				   const char **status_string);
const char *ofi_hsa_status_to_string(hsa_status_t status);

hsa_status_t ofi_hsa_amd_dereg_dealloc_cb(void *ptr,
					  hsa_amd_deallocation_callback_t cb);
hsa_status_t ofi_hsa_amd_reg_dealloc_cb(void *ptr,
					hsa_amd_deallocation_callback_t cb,
					void *user_data);

hsa_status_t ofi_hsa_amd_memory_lock(void *host_ptr, size_t size,
				     hsa_agent_t *agents, int num_agents,
				     void **agent_ptr);
hsa_status_t ofi_hsa_amd_memory_unlock(void *host_ptr);

#endif /* HAVE_ROCR */

struct ofi_hmem_ops {
	bool initialized;
	int (*init)(void);
	int (*cleanup)(void);
	int (*copy_to_hmem)(uint64_t device, void *dest, const void *src,
			size_t size);
	int (*copy_from_hmem)(uint64_t device, void *dest, const void *src,
			size_t size);
	bool (*is_addr_valid)(const void *addr, uint64_t *device, uint64_t *flags);
	int (*get_handle)(void *base_addr, void **handle);
	int (*open_handle)(void **handle, uint64_t device, void **mapped_addr);
	int (*close_handle)(void *mapped_addr);
	int (*host_register)(void *addr, size_t size);
	int (*host_unregister)(void *addr);
	int (*get_base_addr)(const void *addr, void **base_addr,
			size_t *base_length);
	bool (*is_ipc_enabled)(void);
	int (*get_ipc_handle_size)(size_t *size);
};

extern struct ofi_hmem_ops hmem_ops[];

int rocr_copy_from_dev(uint64_t device, void *dest, const void *src,
		       size_t size);
int rocr_copy_to_dev(uint64_t device, void *dest, const void *src,
		     size_t size);
int rocr_hmem_init(void);
int rocr_hmem_cleanup(void);
bool rocr_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags);
int rocr_host_register(void *ptr, size_t size);
int rocr_host_unregister(void *ptr);
int rocr_get_base_addr(const void *ptr, void **base, size_t *size);

int cuda_copy_to_dev(uint64_t device, void *dev, const void *host, size_t size);
int cuda_copy_from_dev(uint64_t device, void *host, const void *dev, size_t size);
int cuda_hmem_init(void);
int cuda_hmem_cleanup(void);
bool cuda_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags);
int cuda_host_register(void *ptr, size_t size);
int cuda_host_unregister(void *ptr);
int cuda_dev_register(struct fi_mr_attr *mr_attr, uint64_t *handle);
int cuda_dev_unregister(uint64_t handle);
int cuda_get_handle(void *dev_buf, void **handle);
int cuda_open_handle(void **handle, uint64_t device, void **ipc_ptr);
int cuda_close_handle(void *ipc_ptr);
int cuda_get_base_addr(const void *ptr, void **base, size_t *size);
bool cuda_is_ipc_enabled(void);
int cuda_get_ipc_handle_size(size_t *size);
bool cuda_is_gdrcopy_enabled(void);

void cuda_gdrcopy_to_dev(uint64_t handle, void *dev,
			 const void *host, size_t size);
void cuda_gdrcopy_from_dev(uint64_t handle, void *host,
			   const void *dev, size_t size);
int cuda_gdrcopy_hmem_init(void);
int cuda_gdrcopy_hmem_cleanup(void);
int cuda_gdrcopy_dev_register(struct fi_mr_attr *mr_attr, uint64_t *handle);
int cuda_gdrcopy_dev_unregister(uint64_t handle);
int cuda_set_sync_memops(void *ptr);

#define ZE_MAX_DEVICES 8
int ze_hmem_copy(uint64_t device, void *dst, const void *src, size_t size);
int ze_hmem_init(void);
int ze_hmem_cleanup(void);
bool ze_hmem_is_addr_valid(const void *addr, uint64_t *device, uint64_t *flags);
int ze_hmem_get_handle(void *dev_buf, void **handle);
int ze_hmem_open_handle(void **handle, uint64_t device, void **ipc_ptr);
int ze_hmem_get_shared_handle(int dev_fd, void *dev_buf, int *ze_fd,
			      void **handle);
int ze_hmem_open_shared_handle(int dev_fd, void **handle, int *ze_fd,
			       uint64_t device, void **ipc_ptr);
int ze_hmem_close_handle(void *ipc_ptr);
bool ze_hmem_p2p_enabled(void);
int ze_hmem_get_ipc_handle_size(size_t *size);
int ze_hmem_get_base_addr(const void *ptr, void **base, size_t *size);
int ze_hmem_get_id(const void *ptr, uint64_t *id);
int *ze_hmem_get_dev_fds(int *nfds);
int ze_hmem_host_register(void *ptr, size_t size);
int ze_hmem_host_unregister(void *ptr);

int neuron_copy_to_dev(uint64_t device, void *dev, const void *host, size_t size);
int neuron_copy_from_dev(uint64_t device, void *host, const void *dev, size_t size);
int neuron_hmem_init(void);
int neuron_hmem_cleanup(void);
void *neuron_alloc(void **handle, size_t size);
void neuron_free(void **handle);

int synapseai_init(void);
int synapseai_cleanup(void);
int synapseai_copy_to_hmem(uint64_t device, void *dest, const void *src,
                           size_t size);
int synapseai_copy_from_hmem(uint64_t device, void *dest, const void *src,
                             size_t size);
int synapseai_get_dmabuf_fd(uint64_t addr, uint64_t size, int* fd);
bool synapseai_is_addr_valid(const void *addr, uint64_t *device,
                             uint64_t *flags);
int synapseai_get_handle(void *dev_buf, void **handle);
int synapseai_open_handle(void **handle, uint64_t device, void **ipc_ptr);
int synapseai_close_handle(void *ipc_ptr);
int synapseai_host_register(void *ptr, size_t size);
int synapseai_host_unregister(void *ptr);
int synapseai_get_base_addr(const void *ptr, void **base, size_t *size);
bool synapseai_is_ipc_enabled(void);

static inline int ofi_memcpy(uint64_t device, void *dest, const void *src,
			     size_t size)
{
	memcpy(dest, src, size);
	return FI_SUCCESS;
}

static inline int ofi_hmem_init_noop(void)
{
	return FI_SUCCESS;
}

static inline int ofi_hmem_cleanup_noop(void)
{
	return FI_SUCCESS;
}

static inline int ofi_hmem_no_get_handle(void *base_addr, void **handle)
{
	return -FI_ENOSYS;
}

static inline int
ofi_hmem_no_open_handle(void **handle, uint64_t device, void **mapped_addr)
{
	return -FI_ENOSYS;
}

static inline int ofi_hmem_no_close_handle(void *mapped_addr)
{
	return -FI_ENOSYS;
}

static inline int ofi_hmem_no_get_ipc_handle_size(size_t *size)
{
	return -FI_ENOSYS;
}

static inline int ofi_hmem_host_register_noop(void *addr, size_t size)
{
	return FI_SUCCESS;
}

static inline int ofi_hmem_host_unregister_noop(void *addr)
{
	return FI_SUCCESS;
}

static inline int
ofi_hmem_no_base_addr(const void *addr, void **base_addr, size_t *base_length)
{
	return -FI_ENOSYS;
}

static inline bool ofi_hmem_no_is_ipc_enabled(void)
{
	return false;
}

static inline bool ofi_hmem_p2p_disabled(void)
{
	return ofi_hmem_disable_p2p;
}

ssize_t ofi_copy_from_hmem_iov(void *dest, size_t size,
			       enum fi_hmem_iface hmem_iface, uint64_t device,
			       const struct iovec *hmem_iov,
			       size_t hmem_iov_count, uint64_t hmem_iov_offset);

ssize_t ofi_copy_to_hmem_iov(enum fi_hmem_iface hmem_iface, uint64_t device,
			     const struct iovec *hmem_iov,
			     size_t hmem_iov_count, uint64_t hmem_iov_offset,
			     const void *src, size_t size);

int ofi_hmem_get_handle(enum fi_hmem_iface iface, void *base_addr, void **handle);
int ofi_hmem_open_handle(enum fi_hmem_iface iface, void **handle,
			 uint64_t device, void **mapped_addr);
int ofi_hmem_close_handle(enum fi_hmem_iface iface, void *mapped_addr);
int ofi_hmem_get_base_addr(enum fi_hmem_iface iface, const void *addr,
			   void **base_addr, size_t *base_length);
bool ofi_hmem_is_initialized(enum fi_hmem_iface iface);

void ofi_hmem_init(void);
void ofi_hmem_cleanup(void);
enum fi_hmem_iface ofi_get_hmem_iface(const void *addr, uint64_t *device,
				      uint64_t *flags);
int ofi_hmem_host_register(void *addr, size_t size);
int ofi_hmem_host_unregister(void *addr);
bool ofi_hmem_is_ipc_enabled(enum fi_hmem_iface iface);
size_t ofi_hmem_get_ipc_handle_size(enum fi_hmem_iface iface);

#endif /* _OFI_HMEM_H_ */
