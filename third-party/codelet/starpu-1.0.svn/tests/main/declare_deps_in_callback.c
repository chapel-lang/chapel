
/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <starpu.h>
#include "../helper.h"

#define NLOOPS	128

static void callback(void *arg)
{
	struct starpu_task *taskA, *taskB;
	int ret;

	taskA = starpu_task_get_current();
	taskB = (struct starpu_task *) arg;

	starpu_task_declare_deps_array(taskB, 1, &taskA);
	ret = starpu_task_submit(taskB);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
}

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet dummy_codelet = 
{
	.cpu_funcs = {dummy_func, NULL},
	.cuda_funcs = {dummy_func, NULL},
	.opencl_funcs = {dummy_func, NULL},
	.model = NULL,
	.nbuffers = 0
};

static struct starpu_task *create_dummy_task(void)
{
	struct starpu_task *task = starpu_task_create();

	task->cl = &dummy_codelet;
	task->cl_arg = NULL;

	return task;
}

int main(int argc, char **argv)
{
	int ret;
	unsigned loop;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	struct starpu_task *taskA, *taskB;

	for (loop = 0; loop < NLOOPS; loop++)
	{
		taskA = create_dummy_task();
		taskB = create_dummy_task();

		taskA->callback_func = callback;
		taskA->callback_arg = taskB;

		ret = starpu_task_submit(taskA);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
