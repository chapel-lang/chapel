/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#include <starpu.h>
#include "../helper.h"

static unsigned nloops = 1000;

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet cpu_codelet =
{
        .where = STARPU_CPU,
        .cpu_funcs = {dummy_func, NULL},
        .model = NULL,
        .nbuffers = 1,
	.modes = {STARPU_RW}
};

static struct starpu_codelet gpu_codelet =
{
        .where = STARPU_CUDA|STARPU_OPENCL,
        .cuda_funcs = {dummy_func, NULL},
        .opencl_funcs = {dummy_func, NULL},
        .model = NULL,
        .nbuffers = 1,
	.modes = {STARPU_RW}
};


int main(int argc, char **argv)
{
        float foo;
	starpu_data_handle_t float_array_handle;
        int i, ret;

        ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	if (starpu_worker_get_count_by_type(STARPU_CUDA_WORKER) == 0 && starpu_worker_get_count_by_type(STARPU_OPENCL_WORKER) == 0)
	{
		FPRINTF(stderr, "This application requires a CUDA or OpenCL Worker\n");
		starpu_shutdown();
		return STARPU_TEST_SKIPPED;
	}

        foo = 0.0f;
	starpu_variable_data_register(&float_array_handle, 0, (uintptr_t)&foo, sizeof(foo));

        for (i = 0; i < nloops; i++)
        {
		struct starpu_task *task_cpu, *task_gpu;
                int ret;

		task_cpu = starpu_task_create();
		task_gpu = starpu_task_create();

		task_cpu->cl = &cpu_codelet;
		task_cpu->callback_func = NULL;
		task_cpu->handles[0] = float_array_handle;

		task_gpu->cl = &gpu_codelet;
		task_gpu->callback_func = NULL;
		task_gpu->handles[0] = float_array_handle;

		ret = starpu_task_submit(task_cpu);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

		ret = starpu_task_submit(task_gpu);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
        }

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");
	starpu_data_unregister(float_array_handle);
        starpu_shutdown();

        return EXIT_SUCCESS;

enodev:
	starpu_data_unregister(float_array_handle);
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
