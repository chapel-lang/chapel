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

#if HAVE_LIBCUDA

#include <cuda.h>
#include <cuda_runtime.h>

struct cuda_ops {
	cudaError_t (*cudaMemcpy)(void *dst, const void *src, size_t count,
				  enum cudaMemcpyKind kind);
	const char *(*cudaGetErrorName)(cudaError_t error);
	const char *(*cudaGetErrorString)(cudaError_t error);
	CUresult (*cuPointerGetAttribute)(void *data,
					  CUpointer_attribute attribute,
					  CUdeviceptr ptr);
	cudaError_t (*cudaHostRegister)(void *ptr, size_t size,
					unsigned int flags);
	cudaError_t (*cudaHostUnregister)(void *ptr);
	cudaError_t (*cudaGetDeviceCount)(int *count);
};

static int hmem_cuda_use_gdrcopy;

#ifdef ENABLE_CUDA_DLOPEN

#include <dlfcn.h>

static void *cudart_handle;
static void *cuda_handle;
static struct cuda_ops cuda_ops;

#else

static struct cuda_ops cuda_ops = {
	.cudaMemcpy = cudaMemcpy,
	.cudaGetErrorName = cudaGetErrorName,
	.cudaGetErrorString = cudaGetErrorString,
	.cuPointerGetAttribute = cuPointerGetAttribute,
	.cudaHostRegister = cudaHostRegister,
	.cudaHostUnregister = cudaHostUnregister,
	.cudaGetDeviceCount = cudaGetDeviceCount,
};

#endif /* ENABLE_CUDA_DLOPEN */

cudaError_t ofi_cudaMemcpy(void *dst, const void *src, size_t count,
			   enum cudaMemcpyKind kind)
{
	return cuda_ops.cudaMemcpy(dst, src, count, kind);
}

const char *ofi_cudaGetErrorName(cudaError_t error)
{
	return cuda_ops.cudaGetErrorName(error);
}

const char *ofi_cudaGetErrorString(cudaError_t error)
{
	return cuda_ops.cudaGetErrorString(error);
}

CUresult ofi_cuPointerGetAttribute(void *data, CUpointer_attribute attribute,
				   CUdeviceptr ptr)
{
	return cuda_ops.cuPointerGetAttribute(data, attribute, ptr);
}

cudaError_t ofi_cudaHostRegister(void *ptr, size_t size, unsigned int flags)
{
	return cuda_ops.cudaHostRegister(ptr, size, flags);
}

cudaError_t ofi_cudaHostUnregister(void *ptr)
{
	return cuda_ops.cudaHostUnregister(ptr);
}

static cudaError_t ofi_cudaGetDeviceCount(int *count)
{
	return cuda_ops.cudaGetDeviceCount(count);
}

int cuda_copy_to_dev(uint64_t device, void *dev, const void *host, size_t size)
{
	if (hmem_cuda_use_gdrcopy) {
		cuda_gdrcopy_to_dev(device, dev, host, size);
		return FI_SUCCESS;
	}

	cudaError_t cuda_ret;

	cuda_ret = ofi_cudaMemcpy(dev, host, size, cudaMemcpyHostToDevice);
	if (cuda_ret == cudaSuccess)
		return 0;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform cudaMemcpy: %s:%s\n",
		ofi_cudaGetErrorName(cuda_ret),
		ofi_cudaGetErrorString(cuda_ret));

	return -FI_EIO;
}

int cuda_copy_from_dev(uint64_t device, void *host, const void *dev, size_t size)
{
	if (hmem_cuda_use_gdrcopy) {
		cuda_gdrcopy_from_dev(device, host, dev, size);
		return FI_SUCCESS;
	}

	cudaError_t cuda_ret;

	cuda_ret = ofi_cudaMemcpy(host, dev, size, cudaMemcpyDeviceToHost);
	if (cuda_ret == cudaSuccess)
		return 0;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform cudaMemcpy: %s:%s\n",
		ofi_cudaGetErrorName(cuda_ret),
		ofi_cudaGetErrorString(cuda_ret));

	return -FI_EIO;
}

int cuda_dev_register(struct fi_mr_attr *mr_attr, uint64_t *handle)
{
	if (hmem_cuda_use_gdrcopy)
		return cuda_gdrcopy_dev_register(mr_attr, handle);

	*handle = mr_attr->device.cuda;
	return FI_SUCCESS;
}

int cuda_dev_unregister(uint64_t handle)
{
	if (hmem_cuda_use_gdrcopy)
		return cuda_gdrcopy_dev_unregister(handle);

	return FI_SUCCESS;
}

static int cuda_hmem_dl_init(void)
{
#ifdef ENABLE_CUDA_DLOPEN
	/* Assume failure to dlopen CUDA runtime is caused by the library not
	 * being found. Thus, CUDA is not supported.
	 */
	cudart_handle = dlopen("libcudart.so", RTLD_NOW);
	if (!cudart_handle) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libcudart.so\n");
		return -FI_ENOSYS;
	}

	cuda_handle = dlopen("libcuda.so", RTLD_NOW);
	if (!cuda_handle) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libcuda.so\n");
		goto err_dlclose_cudart;
	}

	cuda_ops.cudaMemcpy = dlsym(cudart_handle, "cudaMemcpy");
	if (!cuda_ops.cudaMemcpy) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find cudaMemcpy\n");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaGetErrorName = dlsym(cudart_handle, "cudaGetErrorName");
	if (!cuda_ops.cudaGetErrorName) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find cudaGetErrorName\n");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaGetErrorString = dlsym(cudart_handle,
					    "cudaGetErrorString");
	if (!cuda_ops.cudaGetErrorString) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find cudaGetErrorString\n");
		goto err_dlclose_cuda;
	}

	cuda_ops.cuPointerGetAttribute = dlsym(cuda_handle,
					       "cuPointerGetAttribute");
	if (!cuda_ops.cuPointerGetAttribute) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find cuPointerGetAttribute\n");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaHostRegister = dlsym(cudart_handle, "cudaHostRegister");
	if (!cuda_ops.cudaHostRegister) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find cudaHostRegister\n");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaHostUnregister = dlsym(cudart_handle,
					    "cudaHostUnregister");
	if (!cuda_ops.cudaHostUnregister) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find cudaHostUnregister\n");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaGetDeviceCount = dlsym(cudart_handle,
					    "cudaGetDeviceCount");
	if (!cuda_ops.cudaGetDeviceCount) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find cudaGetDeviceCount\n");
		goto err_dlclose_cuda;
	}

	return FI_SUCCESS;

err_dlclose_cuda:
	dlclose(cuda_handle);
err_dlclose_cudart:
	dlclose(cudart_handle);

	return -FI_ENODATA;
#else
	return FI_SUCCESS;
#endif /* ENABLE_CUDA_DLOPEN */
}

static void cuda_hmem_dl_cleanup(void)
{
#ifdef ENABLE_CUDA_DLOPEN
	dlclose(cuda_handle);
	dlclose(cudart_handle);
#endif
}

static int cuda_hmem_verify_devices(void)
{
	int device_count;
	cudaError_t cuda_ret;

	/* Verify CUDA compute-capable devices are present on the host. */
	cuda_ret = ofi_cudaGetDeviceCount(&device_count);
	switch (cuda_ret) {
	case cudaSuccess:
		break;

	case cudaErrorNoDevice:
		return -FI_ENOSYS;

	default:
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform cudaGetDeviceCount: %s:%s\n",
			ofi_cudaGetErrorName(cuda_ret),
			ofi_cudaGetErrorString(cuda_ret));
		return -FI_EIO;
	}

	if (device_count == 0)
		return -FI_ENOSYS;

	return FI_SUCCESS;
}

int cuda_hmem_init(void)
{
	int ret;
	int gdrcopy_ret;

	ret = cuda_hmem_dl_init();
	if (ret != FI_SUCCESS)
		return ret;

	ret = cuda_hmem_verify_devices();
	if (ret != FI_SUCCESS)
		goto dl_cleanup;

	gdrcopy_ret = cuda_gdrcopy_hmem_init();
	if (gdrcopy_ret == FI_SUCCESS) {
		hmem_cuda_use_gdrcopy = 1;
		fi_param_define(NULL, "hmem_cuda_use_gdrcopy", FI_PARAM_BOOL,
				"Use gdrcopy to copy data to/from GPU memory");
		fi_param_get_bool(NULL, "hmem_cuda_use_gdrcopy",
				  &hmem_cuda_use_gdrcopy);
	} else {
		hmem_cuda_use_gdrcopy = 0;
		if (gdrcopy_ret != -FI_ENOSYS)
			FI_WARN(&core_prov, FI_LOG_CORE,
				"gdrcopy initialization failed! gdrcopy will not be used.\n");
	}

	return ret;

dl_cleanup:
	cuda_hmem_dl_cleanup();

	return ret;
}

int cuda_hmem_cleanup(void)
{
	cuda_hmem_dl_cleanup();
	if (hmem_cuda_use_gdrcopy)
		cuda_gdrcopy_hmem_cleanup();
	return FI_SUCCESS;
}

bool cuda_is_addr_valid(const void *addr)
{
	CUresult cuda_ret;
	unsigned int data;

	cuda_ret = ofi_cuPointerGetAttribute(&data,
					     CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
					     (CUdeviceptr)addr);
	switch (cuda_ret) {
	case CUDA_SUCCESS:
		if (data == CU_MEMORYTYPE_DEVICE)
			return true;
		break;

	/* Returned if the buffer is not associated with the CUcontext support
	 * unified virtual addressing. Since host buffers may fall into this
	 * category, this is not treated as an error.
	 */
	case CUDA_ERROR_INVALID_VALUE:
		break;

	/* Returned if cuInit() has not been called. This can happen if support
	 * for CUDA is enabled but the user has not made a CUDA call. This is
	 * not treated as an error.
	 */
	case CUDA_ERROR_NOT_INITIALIZED:
		break;

	/* Returned if the CUcontext does not support unified virtual
	 * addressing.
	 */
	case CUDA_ERROR_INVALID_CONTEXT:
		FI_WARN(&core_prov, FI_LOG_CORE,
			"CUcontext does not support unified virtual addressining\n");
		break;

	default:
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unhandle cuPointerGetAttribute return code: ret=%d\n",
			cuda_ret);
		break;
	}

	return false;
}

int cuda_host_register(void *ptr, size_t size)
{
	cudaError_t cuda_ret;

	cuda_ret = ofi_cudaHostRegister(ptr, size, cudaHostRegisterDefault);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform cudaHostRegister: %s:%s\n",
		ofi_cudaGetErrorName(cuda_ret),
		ofi_cudaGetErrorString(cuda_ret));

	return -FI_EIO;
}

int cuda_host_unregister(void *ptr)
{
	cudaError_t cuda_ret;

	cuda_ret = ofi_cudaHostUnregister(ptr);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Failed to perform cudaHostUnregister: %s:%s\n",
		ofi_cudaGetErrorName(cuda_ret),
		ofi_cudaGetErrorString(cuda_ret));

	return -FI_EIO;
}

#else

int cuda_copy_to_dev(uint64_t device, void *dev, const void *host, size_t size)
{
	return -FI_ENOSYS;
}

int cuda_copy_from_dev(uint64_t device, void *host, const void *dev, size_t size)
{
	return -FI_ENOSYS;
}

int cuda_hmem_init(void)
{
	return -FI_ENOSYS;
}

int cuda_hmem_cleanup(void)
{
	return -FI_ENOSYS;
}

bool cuda_is_addr_valid(const void *addr)
{
	return false;
}

int cuda_host_register(void *ptr, size_t size)
{
	return -FI_ENOSYS;
}

int cuda_host_unregister(void *ptr)
{
	return -FI_ENOSYS;
}

int cuda_dev_register(struct fi_mr_attr *mr_attr, uint64_t *handle)
{
	return FI_SUCCESS;
}

int cuda_dev_unregister(uint64_t handle)
{
	return FI_SUCCESS;
}

#endif /* HAVE_LIBCUDA */
