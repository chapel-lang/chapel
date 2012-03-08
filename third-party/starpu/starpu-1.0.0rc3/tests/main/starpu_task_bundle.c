/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  Inria
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

#define NB_BUNDLE 10
#define NB_ITERATION 5

void func_cpu(void *descr[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	float *x = (float *) STARPU_VARIABLE_GET_PTR(descr[0]);
	float factor;

	factor = *(float *) args;
        *x *= factor;
}

struct starpu_codelet codelet =
{
	.modes = {STARPU_RW},
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 1
};

int main(int argc, char **argv)
{
        int i, j, ret;

	float data[NB_BUNDLE];
	float factors[NB_BUNDLE];
        starpu_data_handle_t handles[NB_BUNDLE];

	struct starpu_task *task[NB_ITERATION];

	starpu_task_bundle_t bundles[NB_BUNDLE];

	for (i = 0; i < NB_BUNDLE; i++)
	{
		data[i] = i + 1;
		factors[i] = NB_BUNDLE - i;
	}

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	for (i = 0; i < NB_BUNDLE; i++)
		starpu_variable_data_register(&handles[i], 0, (uintptr_t)&data[i], sizeof(float));

        FPRINTF(stderr, "VALUES:");
	for (i = 0; i < NB_BUNDLE; i++)
		FPRINTF(stderr, " %f (%f)", data[i], factors[i]);
        FPRINTF(stderr, "\n");

	for (i = 0; i < NB_BUNDLE; i++)
	{
		starpu_task_bundle_create(&bundles[i]);

		for (j = 0; j < NB_ITERATION; j++)
		{
			task[j] = starpu_task_create();

			task[j]->cl = &codelet;

			task[j]->cl_arg = &factors[i];
			task[j]->cl_arg_size = sizeof(float);

			task[j]->handles[0] = handles[i];

			ret = starpu_task_bundle_insert(bundles[i], task[j]);
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_insert");
		}

		ret = starpu_task_bundle_remove(bundles[i], task[NB_ITERATION / 2]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_remove");

		for (j = 0; j < NB_ITERATION; j++)
		{
			ret = starpu_task_submit(task[j]);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}

		starpu_task_bundle_close(bundles[i]);
	}

        ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

        for(i = 0; i < NB_BUNDLE ; i++)
	{
                ret = starpu_data_acquire(handles[i], STARPU_R);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire");
        }

        FPRINTF(stderr, "VALUES:");
	for (i = 0; i < NB_BUNDLE; i++)
		FPRINTF(stderr, " %f (%f)", data[i], factors[i]);
        FPRINTF(stderr, "\n");

        for(i = 0; i < NB_BUNDLE ; i++)
	{
                starpu_data_release(handles[i]);
		starpu_data_unregister(handles[i]);
	}

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	starpu_shutdown();
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	return STARPU_TEST_SKIPPED;
}
