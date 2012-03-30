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

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet dummy_codelet =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
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
	task->detach = 0;

	return task;
}

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	FPRINTF(stderr, "{ A } -> { B }\n");
	fflush(stderr);

	struct starpu_task *taskA, *taskB;

	taskA = create_dummy_task();
	taskB = create_dummy_task();

	/* B depends on A */
	starpu_task_declare_deps_array(taskB, 1, &taskA);

	ret = starpu_task_submit(taskB);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskA); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	ret = starpu_task_wait(taskB); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

	FPRINTF(stderr, "{ C, D, E, F } -> { G }\n");

	struct starpu_task *taskC, *taskD, *taskE, *taskF, *taskG;

	taskC = create_dummy_task();
	taskD = create_dummy_task();
	taskE = create_dummy_task();
	taskF = create_dummy_task();
	taskG = create_dummy_task();

	struct starpu_task *tasksCDEF[4] = {taskC, taskD, taskE, taskF};
	starpu_task_declare_deps_array(taskG, 4, tasksCDEF);

	ret = starpu_task_submit(taskC); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskD); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskG); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskE); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskF); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	ret = starpu_task_wait(taskG); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

	FPRINTF(stderr, "{ H, I } -> { J, K, L }\n");

	struct starpu_task *taskH, *taskI, *taskJ, *taskK, *taskL;

	taskH = create_dummy_task();
	taskI = create_dummy_task();
	taskJ = create_dummy_task();
	taskK = create_dummy_task();
	taskL = create_dummy_task();

	struct starpu_task *tasksHI[2] = {taskH, taskI};

	starpu_task_declare_deps_array(taskJ, 2, tasksHI);
	starpu_task_declare_deps_array(taskK, 2, tasksHI);
	starpu_task_declare_deps_array(taskL, 2, tasksHI);

	ret = starpu_task_submit(taskH); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskI); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskJ); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskK); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskL); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	ret = starpu_task_wait(taskJ); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");
	ret = starpu_task_wait(taskK); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");
	ret = starpu_task_wait(taskL); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

	starpu_task_wait_for_all();

	/* Destroy all the tasks that were not detached */
	starpu_task_destroy(taskA);
	starpu_task_destroy(taskC);
	starpu_task_destroy(taskD);
	starpu_task_destroy(taskE);
	starpu_task_destroy(taskF);
	starpu_task_destroy(taskH);
	starpu_task_destroy(taskI);

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
