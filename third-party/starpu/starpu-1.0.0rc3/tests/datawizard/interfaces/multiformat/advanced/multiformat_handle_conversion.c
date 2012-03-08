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

#define DEBUG 0

#if DEBUG
#define SYNCHRONOUS 1 /* Easier to debug with synchronous tasks */
#define ENTER() do { FPRINTF(stderr, "Entering %s\n", __func__); } while (0)
#else
#define SYNCHRONOUS 0
#define ENTER()
#endif

extern struct stats global_stats;

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
static void
create_and_submit_tasks(int where, starpu_data_handle_t handles[])
{
	FPRINTF(stderr, "***** Starting Task 1\n");
	struct starpu_codelet cl =
	{
		.modes        = { STARPU_RW },
		.nbuffers     = 1,
		.where        = where
	};

#ifdef STARPU_USE_CUDA
	if (where & STARPU_CUDA)
		cl.cuda_funcs[0] = cuda_func;
#endif
#ifdef STARPU_USE_OPENCL
	if (where & STARPU_OPENCL)
		cl.opencl_funcs[0] = opencl_func;
#endif

	struct starpu_task *task = starpu_task_create();
	task->synchronous = SYNCHRONOUS;
	task->cl = &cl;
	task->handles[0] = handles[0];
	assert(starpu_task_submit(task) == 0);

#ifdef STARPU_USE_CPU
	FPRINTF(stderr, "***** Starting Task 2\n");
	struct starpu_codelet cl2 =
	{
		.modes = { STARPU_RW },
		.cpu_funcs = {cpu_func, NULL},
		.nbuffers = 1,
		.where = STARPU_CPU,
	};

	struct starpu_task *task2 = starpu_task_create();
	task2->synchronous = SYNCHRONOUS;
	task2->cl = &cl2;
	task2->handles[0] = handles[1];
	assert(starpu_task_submit(task2) == 0);
#endif /* !STARPU_USE_CPU */

	FPRINTF(stderr, "***** Starting Task 3\n");
	struct starpu_codelet cl3 =
	{
		.modes = { STARPU_RW, STARPU_RW },
		.nbuffers    = 2,
		.where       = where
	};

#ifdef STARPU_USE_CUDA
	if (where & STARPU_CUDA)
		cl3.cuda_funcs[0] = cuda_func;
#endif
#ifdef STARPU_USE_OPENCL
	if (where & STARPU_OPENCL)
		cl3.opencl_funcs[0] = opencl_func;
#endif

	struct starpu_task *task3 = starpu_task_create();
	task3->synchronous = SYNCHRONOUS;
	task3->cl = &cl3;
	task3->handles[0] = handles[0];
	task3->handles[1] = handles[1];
	assert(starpu_task_submit(task3) == 0);

	assert(starpu_task_wait_for_all() == 0);

	FPRINTF(stderr, "***** End of all tasks\n");
	return;
}
#endif

/* XXX Just a little bit of copy/pasta here... */
#ifdef STARPU_USE_CUDA
static int
test_cuda(void)
{
	int i;
	int vector1[NX];
	int vector2[NX];
	starpu_data_handle_t handles[2];

	for (i = 0; i < NX; i++)
	{
		vector1[i] = i;
		vector2[i] = i;
	}

	starpu_multiformat_data_register(&handles[0], 0, vector1, NX, &ops);
	starpu_multiformat_data_register(&handles[1], 0, vector2, NX, &ops);

	memset(&global_stats, 0, sizeof(global_stats));
	create_and_submit_tasks(STARPU_CUDA, handles);

	starpu_data_unregister(handles[0]);
	starpu_data_unregister(handles[1]);

#if DEBUG
	print_stats(&global_stats);
#endif

	struct stats expected_stats;
#ifdef STARPU_USE_CPU
	expected_stats.cpu = 1;
#endif /* !STARPU_USE_CPU */
#ifdef STARPU_USE_OPENCL
	expected_stats.opencl = 0;
	expected_stats.cpu_to_opencl = 0;
	expected_stats.opencl_to_cpu = 0;
#endif /* !STARPU_USE_OPENCL */
	expected_stats.cuda = 2;
	expected_stats.cpu_to_cuda = 2;
	expected_stats.cuda_to_cpu = 2;

	return compare_stats(&expected_stats, &global_stats);
}
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static int
test_opencl(void)
{
	int i;
	int vector1[NX];
	int vector2[NX];
	starpu_data_handle_t handles[2];

	for (i = 0; i < NX; i++)
	{
		vector1[i] = i;
		vector2[i] = i;
	}

	starpu_multiformat_data_register(&handles[0], 0, vector1, NX, &ops);
	starpu_multiformat_data_register(&handles[1], 0, vector2, NX, &ops);

	memset(&global_stats, 0, sizeof(global_stats));
	create_and_submit_tasks(STARPU_OPENCL, handles);

	starpu_data_unregister(handles[0]);
	starpu_data_unregister(handles[1]);

#if DEBUG
	print_stats(&global_stats);
#endif

	struct stats expected_stats;
#ifdef STARPU_USE_CPU
	expected_stats.cpu = 1;
#endif /* !STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
	expected_stats.cuda = 0;
	expected_stats.cpu_to_cuda = 0;
	expected_stats.cuda_to_cpu = 0;
#endif /* !STARPU_USE_CUDA */
	expected_stats.opencl = 2;
	expected_stats.cpu_to_opencl = 2;
	expected_stats.opencl_to_cpu = 2;

	return compare_stats(&expected_stats, &global_stats);
}
#endif /* !STARPU_USE_OPENCL */

int
main(void)
{
#ifdef STARPU_USE_CPU
	int ret;
	struct starpu_conf conf =
	{
		.ncpus   = -1,
		.ncuda   = 2,
		.nopencl = 1
	};

	ret = starpu_init(&conf);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	unsigned int ncpu  = starpu_cpu_worker_get_count();
	if (ncpu == 0)
	{
		FPRINTF(stderr, "No CPUS, cannot run this test.\n");
		return STARPU_TEST_SKIPPED;
	}
	unsigned int ncuda = starpu_cuda_worker_get_count();
	unsigned int nopencl = starpu_opencl_worker_get_count();

#ifdef STARPU_USE_OPENCL
	if (nopencl > 0 && test_opencl() != 0)
	{
		FPRINTF(stderr, "OPENCL FAILED\n");
		return EXIT_FAILURE;
	}
#endif
#ifdef STARPU_USE_CUDA
	if (ncuda > 0 && test_cuda() != 0)
	{
		FPRINTF(stderr, "CUDA FAILED \n");
		return EXIT_FAILURE;
	}
#endif

	starpu_shutdown();

	if (ncuda == 0 && nopencl == 0)
		return STARPU_TEST_SKIPPED;
	else
		return EXIT_SUCCESS;
#else /* !STARPU_USE_CPU */
	/* Without the CPU, there is no point in using the multiformat
	 * interface, so this test is pointless. */

	return STARPU_TEST_SKIPPED;
#endif
}
