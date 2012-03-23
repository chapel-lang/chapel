/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2011  Centre National de la Recherche Scientifique
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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <starpu.h>
#include "../helper.h"

pthread_t threads[16];

static unsigned ntasks = 65536;
static unsigned nthreads = 2;

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

void *thread_func(void *arg __attribute__((unused)))
{
	int i, ret;

	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = &dummy_codelet;
		task->cl_arg = NULL;
		task->callback_func = NULL;
		task->callback_arg = NULL;
		
		int ret = starpu_task_submit(task);
		STARPU_ASSERT(!ret);
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	return NULL;
}

static void usage(char **argv)
{
	FPRINTF(stderr, "%s [-i ntasks] [-t nthreads] [-h]\n", argv[0]);
	exit(-1);
}

static void parse_args(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "i:t:h")) != -1)
	switch(c)
	{
		case 'i':
			ntasks = atoi(optarg);
			break;
		case 't':
			nthreads = atoi(optarg);
			break;
		case 'h':
			usage(argv);
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

#ifdef STARPU_SLOW_MACHINE
	ntasks /= 10;
#endif

	FPRINTF(stderr, "#tasks : %u\n", ntasks);

	gettimeofday(&start, NULL);

	unsigned t;
	for (t = 0; t < nthreads; t++)
	{
		int ret = pthread_create(&threads[t], NULL, thread_func, NULL);
		STARPU_ASSERT(ret == 0);
	}

	for (t = 0; t < nthreads; t++)
	{
		int ret = pthread_join(threads[t], NULL);
		STARPU_ASSERT(ret == 0);
	}

	gettimeofday(&end, NULL);

	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	FPRINTF(stderr, "Total: %f secs\n", timing/1000000);
	FPRINTF(stderr, "Per task: %f usecs\n", timing/(nthreads*ntasks));

	starpu_shutdown();

	return EXIT_SUCCESS;
}
