/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 INRIA
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

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
extern struct stats global_stats;
static int vector[NX]; static starpu_data_handle_t handle;
#endif

#ifdef STARPU_USE_CUDA
static int ncuda;
static int cuda_worker;
#endif
#ifdef STARPU_USE_OPENCL
static int nopencl;
static int opencl_worker;
#endif

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
static struct starpu_codelet cl =
{
	.where = STARPU_CUDA|STARPU_OPENCL,
	.modes = { STARPU_RW },
#ifdef STARPU_USE_CUDA
	.cuda_funcs = { cuda_func, NULL },
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = { opencl_func, NULL },
#endif
	.nbuffers = 1,
};

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

static int
create_and_submit_tasks(void)
{
	struct starpu_task *task;

	task = starpu_task_create();
	task->cl = &cl;
	task->handles[0] = handle;
	task->execute_on_a_specific_worker = 1;

#ifdef STARPU_USE_CUDA
	if (ncuda > 0)
	{
		task->workerid = cuda_worker;
	}
	else
#endif
#ifdef STARPU_USE_OPENCL
	if (nopencl > 0)
	{
		task->workerid = opencl_worker;
	}
	else
#endif
	{
		return -ENODEV;
	}

	return starpu_task_submit(task);
}
#endif

int
main(void)
{
#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
	int err;
	err = starpu_init(NULL);
	if (err == -ENODEV)
		goto enodev;

#ifdef STARPU_USE_CUDA
	ncuda = starpu_worker_get_ids_by_type(STARPU_CUDA_WORKER,
						&cuda_worker, 1);
	if (ncuda < 0)
		ncuda = 1;
#endif
#ifdef STARPU_USE_OPENCL
	nopencl = starpu_worker_get_ids_by_type(STARPU_OPENCL_WORKER,
						&opencl_worker, 1);
	if (nopencl < 0)
		nopencl = 1;
#endif

	reset_stats(&global_stats);
	register_handle();
	err = create_and_submit_tasks();
	unregister_handle();
	starpu_shutdown();

	if (err == -ENODEV)
		goto enodev;

#if defined(STARPU_USE_CUDA)
	if (global_stats.cuda == 1)
	{
		if (global_stats.cpu_to_cuda == 1 &&
		    global_stats.cuda_to_cpu == 1)
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
#endif /* !STARPU_USE_CUDA */
#if defined(STARPU_USE_OPENCL)
	if (global_stats.opencl == 1)
	{
		if (global_stats.cpu_to_opencl == 1 &&
		    global_stats.opencl_to_cpu == 1)
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;

	}
#endif /* !STARPU_USE_OPENCL */

	/* We should not get here */
	return EXIT_FAILURE;

enodev:
#endif
	return STARPU_TEST_SKIPPED;
}
