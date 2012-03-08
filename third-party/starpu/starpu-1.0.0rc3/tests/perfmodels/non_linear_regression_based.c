/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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

#include <config.h>
#include <starpu.h>
#include "../helper.h"

#ifdef STARPU_USE_CUDA
static void memset_cuda(void *descr[], void *arg)
{
	STARPU_SKIP_IF_VALGRIND;

	int *ptr = (int *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	cudaMemset(ptr, 42, n);
	cudaThreadSynchronize();
}
#endif

static void memset_cpu(void *descr[], void *arg)
{
	STARPU_SKIP_IF_VALGRIND;

	int *ptr = (int *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	memset(ptr, 42, n);
}

static struct starpu_perfmodel model =
{
	.type = STARPU_NL_REGRESSION_BASED,
	.symbol = "non_linear_memset_regression_based"
};

static struct starpu_codelet memset_cl =
{
	.where = STARPU_CUDA|STARPU_CPU,
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {memset_cuda, NULL},
#endif
	.cpu_funcs = {memset_cpu, NULL},
	.model = &model,
	.nbuffers = 1,
	.modes = {STARPU_W}
};

static void test_memset(int nelems)
{
	starpu_data_handle_t handle;

	starpu_vector_data_register(&handle, -1, (uintptr_t)NULL, nelems, sizeof(int));

	int nloops = 200;
	int loop;
	for (loop = 0; loop < nloops; loop++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = &memset_cl;
		task->handles[0] = handle;

		int ret = starpu_task_submit(task);
		if (ret == -ENODEV)
			exit(STARPU_TEST_SKIPPED);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	starpu_data_unregister(handle);
}

int main(int argc, char **argv)
{
	int ret;

	struct starpu_conf conf;
	starpu_conf_init(&conf);

	conf.sched_policy_name = "eager";
	conf.calibrate = 2;

	ret = starpu_init(&conf);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	int slog;
	for (slog = 8; slog < 25; slog++)
	{
		int size = 1 << slog;
		test_memset(size);
	}

	starpu_shutdown();

	return EXIT_SUCCESS;
}
