/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
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

#include <starpu.h>
#include <starpu_cuda.h>
#include <common/config.h>

#ifdef STARPU_USE_CUDA
static void init_cublas_func(void *args STARPU_ATTRIBUTE_UNUSED)
{
	cublasStatus cublasst = cublasInit();
	if (STARPU_UNLIKELY(cublasst))
		STARPU_CUBLAS_REPORT_ERROR(cublasst);

#if CUDA_VERSION >= 3010
	cublasSetKernelStream(starpu_cuda_get_local_stream());
#endif
}

static void shutdown_cublas_func(void *args STARPU_ATTRIBUTE_UNUSED)
{
	cublasShutdown();
}
#endif

void starpu_helper_cublas_init(void)
{
#ifdef STARPU_USE_CUDA
	starpu_execute_on_each_worker(init_cublas_func, NULL, STARPU_CUDA);
#endif
}

void starpu_helper_cublas_shutdown(void)
{
#ifdef STARPU_USE_CUDA
	starpu_execute_on_each_worker(shutdown_cublas_func, NULL, STARPU_CUDA);
#endif
}
