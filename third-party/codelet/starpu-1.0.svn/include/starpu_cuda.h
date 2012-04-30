/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#ifndef __STARPU_CUDA_H__
#define __STARPU_CUDA_H__

#include <starpu_config.h>

#if defined STARPU_USE_CUDA && !defined STARPU_DONT_INCLUDE_CUDA_HEADERS
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cublas.h>

#ifdef __cplusplus
extern "C"
{
#endif

void starpu_cublas_report_error(const char *func, const char *file, int line, cublasStatus status);
#define STARPU_CUBLAS_REPORT_ERROR(status) \
	starpu_cublas_report_error(__starpu_func__, __FILE__, __LINE__, status)

void starpu_cuda_report_error(const char *func, const char *file, int line, cudaError_t status);
#define STARPU_CUDA_REPORT_ERROR(status) \
	starpu_cuda_report_error(__starpu_func__, __FILE__, __LINE__, status)

size_t starpu_cuda_get_global_mem_size(int devid);
cudaStream_t starpu_cuda_get_local_stream(void);

const struct cudaDeviceProp *starpu_cuda_get_device_properties(unsigned workerid);

#ifdef __cplusplus
}
#endif

#endif /* STARPU_USE_CUDA && !STARPU_DONT_INCLUDE_CUDA_HEADERS */
#endif /* __STARPU_CUDA_H__ */

