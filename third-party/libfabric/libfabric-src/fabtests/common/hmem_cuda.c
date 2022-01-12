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

#ifdef HAVE_CUDA_RUNTIME_H

#include <dlfcn.h>
#include <stdio.h>
#include <cuda_runtime.h>

struct cuda_ops {
	cudaError_t (*cudaMemcpy)(void *dst, const void *src, size_t count,
				  enum cudaMemcpyKind kind);
	cudaError_t (*cudaMalloc)(void **ptr, size_t size);
	cudaError_t (*cudaFree)(void *ptr);
	cudaError_t (*cudaMemset)(void *ptr, int value, size_t count);
	const char *(*cudaGetErrorName)(cudaError_t error);
	const char *(*cudaGetErrorString)(cudaError_t error);
};

static struct cuda_ops cuda_ops;
static void *cudart_handle;

#define CUDA_ERR(err, fmt, ...) \
	FT_ERR(fmt ": %s %s", ##__VA_ARGS__, cuda_ops.cudaGetErrorName(err), \
	       cuda_ops.cudaGetErrorString(err))

int ft_cuda_init(void)
{
	cudart_handle = dlopen("libcudart.so", RTLD_NOW);
	if (!cudart_handle) {
		FT_ERR("Failed to dlopen libcudart.so");
		goto err;
	}

	cuda_ops.cudaMemcpy = dlsym(cudart_handle, "cudaMemcpy");
	if (!cuda_ops.cudaMemcpy) {
		FT_ERR("Failed to find cudaMemcpy");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaMalloc = dlsym(cudart_handle, "cudaMalloc");
	if (!cuda_ops.cudaMalloc) {
		FT_ERR("Failed to find cudaMalloc");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaFree = dlsym(cudart_handle, "cudaFree");
	if (!cuda_ops.cudaFree) {
		FT_ERR("Failed to find cudaFree");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaMemset = dlsym(cudart_handle, "cudaMemset");
	if (!cuda_ops.cudaMemset) {
		FT_ERR("Failed to find cudaMemset");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaGetErrorName = dlsym(cudart_handle, "cudaGetErrorName");
	if (!cuda_ops.cudaGetErrorName) {
		FT_ERR("Failed to find cudaGetErrorName");
		goto err_dlclose_cuda;
	}

	cuda_ops.cudaGetErrorString = dlsym(cudart_handle,
					    "cudaGetErrorString");
	if (!cuda_ops.cudaGetErrorString) {
		FT_ERR("Failed to find cudaGetErrorString");
		goto err_dlclose_cuda;
	}

	return FI_SUCCESS;

err_dlclose_cuda:
	dlclose(cudart_handle);
err:
	return -FI_ENODATA;
}

int ft_cuda_cleanup(void)
{
	dlclose(cudart_handle);
	return FI_SUCCESS;
}

int ft_cuda_alloc(uint64_t device, void **buf, size_t size)
{
	cudaError_t cuda_ret;

	cuda_ret = cuda_ops.cudaMalloc(buf, size);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	CUDA_ERR(cuda_ret, "cudaMalloc failed");

	return -FI_ENOMEM;
}

int ft_cuda_free(void *buf)
{
	cudaError_t cuda_ret;

	cuda_ret = cuda_ops.cudaFree(buf);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	CUDA_ERR(cuda_ret, "cudaFree failed");

	return -FI_EIO;
}

int ft_cuda_memset(uint64_t device, void *buf, int value, size_t size)
{
	cudaError_t cuda_ret;

	cuda_ret = cuda_ops.cudaMemset(buf, value, size);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	CUDA_ERR(cuda_ret, "cudaMemset failed");

	return -FI_EIO;
}

int ft_cuda_copy_to_hmem(uint64_t device, void *dst, const void *src,
			 size_t size)
{
	cudaError_t cuda_ret;

	cuda_ret = cuda_ops.cudaMemcpy(dst, src, size, cudaMemcpyHostToDevice);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	CUDA_ERR(cuda_ret, "cudaMemcpy failed");

	return -FI_EIO;
}

int ft_cuda_copy_from_hmem(uint64_t device, void *dst, const void *src,
			   size_t size)
{
	cudaError_t cuda_ret;

	cuda_ret = cuda_ops.cudaMemcpy(dst, src, size, cudaMemcpyDeviceToHost);
	if (cuda_ret == cudaSuccess)
		return FI_SUCCESS;

	CUDA_ERR(cuda_ret, "cudaMemcpy failed");

	return -FI_EIO;
}

#else

int ft_cuda_init(void)
{
	return -FI_ENOSYS;
}

int ft_cuda_cleanup(void)
{
	return -FI_ENOSYS;
}

int ft_cuda_alloc(uint64_t device, void **buf, size_t size)
{
	return -FI_ENOSYS;
}

int ft_cuda_free(void *buf)
{
	return -FI_ENOSYS;
}

int ft_cuda_memset(uint64_t device, void *buf, int value, size_t size)
{
	return -FI_ENOSYS;
}

int ft_cuda_copy_to_hmem(uint64_t device, void *dst, const void *src,
			 size_t size)
{
	return -FI_ENOSYS;
}

int ft_cuda_copy_from_hmem(uint64_t device, void *dst, const void *src,
			   size_t size)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_CUDA_RUNTIME_H */
