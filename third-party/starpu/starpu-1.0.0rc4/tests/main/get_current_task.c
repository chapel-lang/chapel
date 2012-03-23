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

#include <config.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <starpu.h>
#include "../helper.h"

static unsigned ntasks = 65536;

static void check_task_func(void *descr[], void *arg)
{
	STARPU_SKIP_IF_VALGRIND;

	/* We check that the returned task is valid from the codelet */
	struct starpu_task *task = (struct starpu_task *) arg;
	STARPU_ASSERT(task == starpu_task_get_current());
}

static void check_task_callback(void *arg)
{
	/* We check that the returned task is valid from the callback */
	struct starpu_task *task = (struct starpu_task *) arg;
	STARPU_ASSERT(task == starpu_task_get_current());
}

static struct starpu_codelet dummy_cl =
{
	.cuda_funcs = {check_task_func, NULL},
	.cpu_funcs = {check_task_func, NULL},
	.opencl_funcs = {check_task_func, NULL},
	.model = NULL,
	.nbuffers = 0
};

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_SLOW_MACHINE
	ntasks /= 10;
#endif

	FPRINTF(stderr, "#tasks : %u\n", ntasks);

	int i;
	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task *task = starpu_task_create();

		/* We check if the function is valid from the codelet or from
		 * the callback */
		task->cl = &dummy_cl;
		task->cl_arg = task;

		task->callback_func = check_task_callback;
		task->callback_arg = task;

		ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	FPRINTF(stderr, "#empty tasks : %u\n", ntasks);

	/* We repeat the same experiment with null codelets */

	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = NULL;

		/* We check if the function is valid from the callback */
		task->callback_func = check_task_callback;
		task->callback_arg = task;

		int ret = starpu_task_submit(task);
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
