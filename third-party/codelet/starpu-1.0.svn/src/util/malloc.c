/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#include <errno.h>

#include <core/workers.h>
#include <common/config.h>
#include <starpu.h>
#include <starpu_data.h>
#include <starpu_cuda.h>
#include <drivers/opencl/driver_opencl.h>

#if (defined(STARPU_USE_CUDA) && !defined(HAVE_CUDA_MEMCPY_PEER))// || defined(STARPU_USE_OPENCL)
struct malloc_pinned_codelet_struct
{
	void **ptr;
	size_t dim;
};
#endif

//#ifdef STARPU_USE_OPENCL
//static void malloc_pinned_opencl_codelet(void *buffers[] STARPU_ATTRIBUTE_UNUSED, void *arg)
//{
//	struct malloc_pinned_codelet_struct *s = arg;
//        //        *(s->ptr) = malloc(s->dim);
//        starpu_opencl_allocate_memory((void **)(s->ptr), s->dim, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR);
//}
//#endif

#if defined(STARPU_USE_CUDA) && !defined(HAVE_CUDA_MEMCPY_PEER)
static void malloc_pinned_cuda_codelet(void *buffers[] STARPU_ATTRIBUTE_UNUSED, void *arg)
{
	struct malloc_pinned_codelet_struct *s = arg;

	cudaError_t cures;
	cures = cudaHostAlloc((void **)(s->ptr), s->dim, cudaHostAllocPortable);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);
}
#endif

#if (defined(STARPU_USE_CUDA) && !defined(HAVE_CUDA_MEMCPY_PEER))// || defined(STARPU_USE_OPENCL)
static struct starpu_perfmodel malloc_pinned_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "malloc_pinned"
};

static struct starpu_codelet malloc_pinned_cl =
{
	.cuda_funcs = {malloc_pinned_cuda_codelet, NULL},
//#ifdef STARPU_USE_OPENCL
//	.opencl_funcs = {malloc_pinned_opencl_codelet, NULL},
//#endif
	.nbuffers = 0,
	.model = &malloc_pinned_model
};
#endif

int starpu_malloc(void **A, size_t dim)
{
	if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
		return -EDEADLK;

	STARPU_ASSERT(A);

	if (_starpu_can_submit_cuda_task())
	{
#ifdef STARPU_USE_CUDA
#ifdef HAVE_CUDA_MEMCPY_PEER
		cudaError_t cures;
		cures = cudaHostAlloc(A, dim, cudaHostAllocPortable);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
#else
		int push_res;

		struct malloc_pinned_codelet_struct s =
		{
			.ptr = A,
			.dim = dim
		};

                malloc_pinned_cl.where = STARPU_CUDA;
		struct starpu_task *task = starpu_task_create();
			task->callback_func = NULL;
			task->cl = &malloc_pinned_cl;
			task->cl_arg = &s;

		task->synchronous = 1;

		_starpu_exclude_task_from_dag(task);

		push_res = starpu_task_submit(task);
		STARPU_ASSERT(push_res != -ENODEV);
#endif
#endif
	}
//	else if (_starpu_can_submit_opencl_task())
//	{
//#ifdef STARPU_USE_OPENCL
//		int push_res;
//
//		struct malloc_pinned_codelet_struct s =
//		{
//			.ptr = A,
//			.dim = dim
//		};
//
//                malloc_pinned_cl.where = STARPU_OPENCL;
//		struct starpu_task *task = starpu_task_create();
//			task->callback_func = NULL;
//			task->cl = &malloc_pinned_cl;
//			task->cl_arg = &s;
//
//		task->synchronous = 1;
//
//		_starpu_exclude_task_from_dag(task);
//
//		push_res = starpu_task_submit(task);
//		STARPU_ASSERT(push_res != -ENODEV);
//#endif
//        }
        else
	{
		*A = malloc(dim);
	}

	STARPU_ASSERT(*A);

	return 0;
}

#if defined(STARPU_USE_CUDA) && !defined(HAVE_CUDA_MEMCPY_PEER)
static void free_pinned_cuda_codelet(void *buffers[] STARPU_ATTRIBUTE_UNUSED, void *arg)
{
	cudaError_t cures;
	cures = cudaFreeHost(arg);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);
}
#endif

//#ifdef STARPU_USE_OPENCL
//static void free_pinned_opencl_codelet(void *buffers[] STARPU_ATTRIBUTE_UNUSED, void *arg)
//{
//        //        free(arg);
//        int err = clReleaseMemObject(arg);
//        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
//}
//#endif

#if (defined(STARPU_USE_CUDA) && !defined(HAVE_CUDA_MEMCPY_PEER)) // || defined(STARPU_USE_OPENCL)
static struct starpu_perfmodel free_pinned_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "free_pinned"
};

static struct starpu_codelet free_pinned_cl =
{
	.cuda_funcs = {free_pinned_cuda_codelet, NULL},
//#ifdef STARPU_USE_OPENCL
//	.opencl_funcs = {free_pinned_opencl_codelet, NULL},
//#endif
	.nbuffers = 0,
	.model = &free_pinned_model
};
#endif

int starpu_free(void *A)
{
	if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
		return -EDEADLK;

#ifdef STARPU_USE_CUDA
	if (_starpu_can_submit_cuda_task())
	{
#ifndef HAVE_CUDA_MEMCPY_PEER
	if (!_starpu_is_initialized())
	{
#endif
		/* This is especially useful when starpu_free is called from
 		 * the GCC-plugin. starpu_shutdown will probably have already
		 * been called, so we will not be able to submit a task. */
		cudaError_t err = cudaFreeHost(A);
		if (STARPU_UNLIKELY(err))
			STARPU_CUDA_REPORT_ERROR(err);
#ifndef HAVE_CUDA_MEMCPY_PEER
	}
	else
	{
		int push_res;

                free_pinned_cl.where = STARPU_CUDA;
		struct starpu_task *task = starpu_task_create();
			task->callback_func = NULL;
			task->cl = &free_pinned_cl;
			task->cl_arg = A;

		task->synchronous = 1;

		_starpu_exclude_task_from_dag(task);

		push_res = starpu_task_submit(task);
		STARPU_ASSERT(push_res != -ENODEV);
	}
#endif
//	else if (_starpu_can_submit_opencl_task())
//	{
//#ifdef STARPU_USE_OPENCL
//		int push_res;
//
//                free_pinned_cl.where = STARPU_OPENCL;
//		struct starpu_task *task = starpu_task_create();
//			task->callback_func = NULL;
//			task->cl = &free_pinned_cl;
//			task->cl_arg = A;
//
//		task->synchronous = 1;
//
//		_starpu_exclude_task_from_dag(task);
//
//		push_res = starpu_task_submit(task);
//		STARPU_ASSERT(push_res != -ENODEV);
//#endif
//	}
	} else
#endif
	{
		free(A);
	}

	return 0;
}
