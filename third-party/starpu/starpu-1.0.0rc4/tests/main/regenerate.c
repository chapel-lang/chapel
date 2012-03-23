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
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <starpu.h>
#include "../helper.h"

static unsigned ntasks = 65536;
static unsigned cnt = 0;

static unsigned completed = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void callback(void *arg __attribute__ ((unused)))
{
	struct starpu_task *task = starpu_task_get_current();

	cnt++;

	if (cnt == ntasks)
	{
		task->regenerate = 0;
		FPRINTF(stderr, "Stop !\n");

		_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
		completed = 1;
		_STARPU_PTHREAD_COND_SIGNAL(&cond);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
	}
}

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

static void parse_args(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "i:")) != -1)
	switch(c)
	{
		case 'i':
			ntasks = atoi(optarg);
			break;
	}
}

int main(int argc, char **argv)
{
	//	unsigned i;
	double timing;
	struct timeval start;
	struct timeval end;
	int ret;

	parse_args(argc, argv);

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	struct starpu_task task;

	starpu_task_init(&task);

	task.cl = &dummy_codelet;
	task.regenerate = 1;
	task.detach = 1;

	task.callback_func = callback;

	FPRINTF(stderr, "#tasks : %u\n", ntasks);

	gettimeofday(&start, NULL);

	ret = starpu_task_submit(&task);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	if (!completed)
		_STARPU_PTHREAD_COND_WAIT(&cond, &mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	gettimeofday(&end, NULL);

	timing = (double)((end.tv_sec - start.tv_sec)*1000000
				+ (end.tv_usec - start.tv_usec));

	FPRINTF(stderr, "Total: %f secs\n", timing/1000000);
	FPRINTF(stderr, "Per task: %f usecs\n", timing/ntasks);

	starpu_shutdown();

	/* Cleanup the statically allocated tasks after shutdown, as StarPU is still working on it after the callback */
	starpu_task_deinit(&task);

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
