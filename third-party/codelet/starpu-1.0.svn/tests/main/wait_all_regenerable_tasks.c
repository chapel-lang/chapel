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

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <starpu.h>
#include "../helper.h"

static unsigned ntasks = 1024;

static void callback(void *arg)
{
	struct starpu_task *task = starpu_task_get_current();

	unsigned *cnt = (unsigned *) arg;

	(*cnt)++;

	if (*cnt == ntasks)
		task->regenerate = 0;
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

#define K	128

int main(int argc, char **argv)
{
	int ret;
	double timing;
	struct timeval start;
	struct timeval end;

	parse_args(argc, argv);

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	struct starpu_task task[K];
	unsigned cnt[K];;

	int i;
	for (i = 0; i < K; i++)
	{
		starpu_task_init(&task[i]);
		cnt[i] = 0;

		task[i].cl = &dummy_codelet;
		task[i].regenerate = 1;
		task[i].detach = 1;

		task[i].callback_func = callback;
		task[i].callback_arg = &cnt[i];
	}

	FPRINTF(stderr, "#tasks : %d x %u tasks\n", K, ntasks);

	gettimeofday(&start, NULL);
	
	for (i = 0; i < K; i++)
	{
		ret = starpu_task_submit(&task[i]);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	for (i = 0; i < K; i++)
		starpu_task_deinit(&task[i]);

	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	gettimeofday(&end, NULL);

	/* Check that all the tasks have been properly executed */
	unsigned total_cnt = 0;
	for (i = 0; i < K; i++)
		total_cnt += cnt[i];

	STARPU_ASSERT(total_cnt == K*ntasks);

	timing = (double)((end.tv_sec - start.tv_sec)*1000000
				+ (end.tv_usec - start.tv_usec));

	FPRINTF(stderr, "Total: %f secs\n", timing/1000000);
	FPRINTF(stderr, "Per task: %f usecs\n", timing/(K*ntasks));

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
