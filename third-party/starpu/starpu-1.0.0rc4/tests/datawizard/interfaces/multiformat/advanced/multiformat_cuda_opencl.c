/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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

#include "generic.h"
#include "../../../../helper.h"

#if defined(STARPU_USE_CUDA) && defined(STARPU_USE_OPENCL)

extern struct stats global_stats;
static int vector[NX];
static starpu_data_handle_t handle;

/*
 * Initially, our vector should be in RAM. It is then used on a CUDA device,
 * then on an OpenCL device, and finally, on a CUDA device again.
 * The following operations should be performed, in this specific order :
 * - CPU -> CUDA conversion
 * - CUDA kernel execution
 * - OpenCL kernel execution
 * - CUDA kernel execution
 * - CUDA -> CPU conversion
 *
 * Note that we will not run any conversion between CUDA and OpenCL, because
 * StarPU assumes that the data structures used on CUDA and OpenCL devices are
 * the same.
 */
static int
test(void)
{
	int ret;
	struct starpu_task *task_cuda, *task_cuda2, *task_opencl;

	static struct starpu_codelet cl_cuda =
	{
		.where     = STARPU_CUDA,
		.cuda_funcs = {cuda_func, NULL},
		.nbuffers  = 1,
		.modes = {STARPU_RW}
	};

	task_cuda = starpu_task_create();
	task_cuda->cl = &cl_cuda;
	task_cuda->handles[0] = handle;
	ret = starpu_task_submit(task_cuda);
	if (ret != 0)
		return 1;

	static struct starpu_codelet cl_opencl =
	{
		.where       = STARPU_OPENCL,
		.opencl_funcs = {opencl_func, NULL},
		.nbuffers    = 1,
		.modes = {STARPU_RW}
	};

	task_opencl = starpu_task_create();
	task_opencl->cl = &cl_opencl;
	task_opencl->handles[0] = handle;
	ret = starpu_task_submit(task_opencl);
	if (ret != 0)
		return 1;

	task_cuda2 = starpu_task_create();
	task_cuda2->cl = &cl_cuda;
	task_cuda2->handles[0] = handle;
	ret = starpu_task_submit(task_cuda2);
	if (ret != 0)
		return 1;

	return 0;
}

static void
register_handle(void)
{
	int i;
	for (i = 0; i < NX; i++)
		vector[i] = i;
	starpu_multiformat_data_register(&handle, 0, vector, NX, &ops);
}

static void
unregister_handle(void)
{
	starpu_data_unregister(handle);
}
#endif /* !(STARPU_USE_CUDA && STARPU_USE_OPENCL) */

int
main(void)
{
#if defined(STARPU_USE_CUDA) && defined(STARPU_USE_OPENCL)
	int ret;
	struct starpu_conf conf =
	{
		.ncpus   = -1,
		.ncuda   = 1,
		.nopencl = 1
	};

	ret = starpu_init(&conf);
	if (ret == -ENODEV)
		goto enodev;

	reset_stats(&global_stats);
	register_handle();
	ret = test();
	unregister_handle();
	starpu_shutdown();

	if (ret != 0)
		return STARPU_TEST_SKIPPED;

	struct stats expected_stats =
	{
#ifdef STARPU_USE_CPU
		.cpu           = 0,
#endif
#ifdef STARPU_USE_CUDA
		.cuda          = 2,
		.cpu_to_cuda   = 1,
		.cuda_to_cpu   = 1,
#endif
#ifdef STARPU_USE_OPENCL
		.opencl        = 1,
		.cpu_to_opencl = 0,
		.opencl_to_cpu = 0
#endif
	};

	ret = compare_stats(&global_stats, &expected_stats);
	if (ret != 0)
	{
		print_stats(&global_stats);
		print_stats(&expected_stats);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
enodev:
	return STARPU_TEST_SKIPPED;
#else
	return STARPU_TEST_SKIPPED;
#endif
}
