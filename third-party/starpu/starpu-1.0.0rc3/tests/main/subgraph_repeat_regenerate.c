/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#include <sys/time.h>
#include <starpu.h>
#include <pthread.h>

#include "../helper.h"

static unsigned niter = 16384;

/*
 *
 *		    /-->B--\
 *		    |      |
 *	     -----> A      D---\--->
 *		^   |      |   |
 *		|   \-->C--/   |
 *		|              |
 *		\--------------/
 *
 *	- {B, C} depend on A
 *	- D depends on {B, C}
 *	- A, B, C and D are resubmitted at the end of the loop (or not)
 */

static struct starpu_task taskA, taskB, taskC, taskD;

static unsigned loop_cnt = 0;
static unsigned check_cnt = 0;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
	(void) STARPU_ATOMIC_ADD(&check_cnt, 1);
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

static void callback_task_D(void *arg __attribute__((unused)))
{
	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	loop_cnt++;

	if (loop_cnt == niter)
	{
		/* We are done */
		taskD.regenerate = 0;
		_STARPU_PTHREAD_COND_SIGNAL(&cond);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
	}
	else
	{
		int ret;
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
		/* Let's go for another iteration */
		ret = starpu_task_submit(&taskA);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}
}

int main(int argc, char **argv)
{
//	unsigned i;
//	double timing;
//	struct timeval start;
//	struct timeval end;
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* Implicit data dependencies and regeneratable tasks are not compatible */
	starpu_data_set_default_sequential_consistency_flag(0);

	starpu_task_init(&taskA);
	taskA.cl = &dummy_codelet;
	taskA.cl_arg = &taskA;
	taskA.regenerate = 0; /* this task will be explicitely resubmitted if needed */

	starpu_task_init(&taskB);
	taskB.cl = &dummy_codelet;
	taskB.cl_arg = &taskB;
	taskB.regenerate = 1;

	starpu_task_init(&taskC);
	taskC.cl = &dummy_codelet;
	taskC.cl_arg = &taskC;
	taskC.regenerate = 1;

	starpu_task_init(&taskD);
	taskD.cl = &dummy_codelet;
	taskD.cl_arg = &taskD;
	taskD.callback_func = callback_task_D;
	taskD.regenerate = 1;

	struct starpu_task *depsBC_array[1] = {&taskA};
	starpu_task_declare_deps_array(&taskB, 1, depsBC_array);
	starpu_task_declare_deps_array(&taskC, 1, depsBC_array);

	struct starpu_task *depsD_array[2] = {&taskB, &taskC};
	starpu_task_declare_deps_array(&taskD, 2, depsD_array);

	ret = starpu_task_submit(&taskA); if (ret == -ENODEV) goto enodev; STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(&taskB); if (ret == -ENODEV) goto enodev; STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(&taskC); if (ret == -ENODEV) goto enodev; STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(&taskD); if (ret == -ENODEV) goto enodev; STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	/* Wait for the termination of all loops */
	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	if (loop_cnt < niter)
		_STARPU_PTHREAD_COND_WAIT(&cond, &mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	STARPU_ASSERT(check_cnt == (4*loop_cnt));

	starpu_shutdown();

	/* Cleanup the statically allocated tasks after shutdown, as StarPU is still working on it after the callback */
	starpu_task_deinit(&taskA);
	starpu_task_deinit(&taskB);
	starpu_task_deinit(&taskC);
	starpu_task_deinit(&taskD);

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}

