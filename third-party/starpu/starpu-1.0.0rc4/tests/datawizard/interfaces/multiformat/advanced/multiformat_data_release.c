/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011-2012  INRIA
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
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

static int vector[NX];
static starpu_data_handle_t handle;

#define ENTER() do { FPRINTF(stderr, "Entering %s\n", __func__); } while (0)

extern struct stats global_stats;


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

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
static void
create_and_submit(int where)
{
	static struct starpu_codelet cl =
	{
		.modes = { STARPU_RW },
#ifdef STARPU_USE_CUDA
		.cuda_funcs   = {cuda_func, NULL},
#endif
#ifdef STARPU_USE_OPENCL
		.opencl_funcs = {opencl_func, NULL},
#endif
		.nbuffers    = 1
	};
	cl.where = where;

	struct starpu_task *task = starpu_task_create();
	task->cl = &cl;
	task->handles[0] = handle;

	/* We need to be sure the data has been copied to the GPU at the end 
	 * of this function */
	task->synchronous = 1;
	if (starpu_task_submit(task) == -ENODEV)
		exit(STARPU_TEST_SKIPPED);
}
#endif

static int
test(void)
{
	struct stats expected_stats;
	memset(&expected_stats, 0, sizeof(expected_stats));

#ifdef STARPU_USE_CUDA
	create_and_submit(STARPU_CUDA);
	starpu_data_acquire(handle, STARPU_RW);

	expected_stats.cuda = 1;
	expected_stats.cpu_to_cuda = 1;
	expected_stats.cuda_to_cpu = 1;

	starpu_data_release(handle);
	if (compare_stats(&global_stats, &expected_stats) != 0)
	{
		FPRINTF(stderr, "CUDA failed\n");
		print_stats(&global_stats);
		FPRINTF(stderr ,"\n");
		print_stats(&expected_stats);
		return -ENODEV;
	}
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
	create_and_submit(STARPU_OPENCL);
	starpu_data_acquire(handle, STARPU_RW);
	expected_stats.opencl = 1;
	expected_stats.cpu_to_opencl = 1;
	expected_stats.opencl_to_cpu = 1;

	starpu_data_release(handle);
	if (compare_stats(&global_stats, &expected_stats) != 0)
	{
		FPRINTF(stderr, "OPENCL failed\n");
		print_stats(&global_stats);
		FPRINTF(stderr ,"\n");
		print_stats(&expected_stats);
		return -ENODEV;
	}
#endif /* !STARPU_USE_OPENCL */

	return 0;
}

int
main(void)
{
#ifdef STARPU_USE_CPU
	int ret;
	struct starpu_conf conf =
	{
		.ncpus = -1,
		.ncuda = 1,
		.nopencl = 1
	};
	memset(&global_stats, 0, sizeof(global_stats));
	ret = starpu_init(&conf);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	register_handle();

	int err = test();

	unregister_handle();
	starpu_shutdown();

	switch (err)
	{
		case -ENODEV:
			return STARPU_TEST_SKIPPED;
		case 0:
			return EXIT_SUCCESS;
		default:
			return EXIT_FAILURE;
	}
#else /* ! STARPU_USE_CPU */
	/* Without the CPU, there is no point in using the multiformat
	 * interface, so this test is pointless. */
	return STARPU_TEST_SKIPPED;
#endif
}
