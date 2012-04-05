/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010, 2012  Université de Bordeaux 1
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

/* This example shows how to make a task depend on either of a series of tasks.
 *
 * For each i, we submit i tasks of type A, which fill the i-th variable, and i
 * tasks of type B, which check that the i-th variable is filled.  Thanks to
 * tag dependency, B tasks are scheduled as soon as one of the corresponding A
 * task is finished.
 */

#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

#include <starpu.h>

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)
#define TAG(i, iter)	((starpu_tag_t)  (((uint64_t)i)<<32 | (iter)) )

void cpu_codelet_A(void *descr[], void *_args)
{
	int *arg = _args;
	*arg = 1;
	fprintf(stderr,"A");
}

void cpu_codelet_B(void *descr[], void *_args)
{
	int *arg = _args;
	if (*arg != 1)
		exit(EXIT_FAILURE);
	fprintf(stderr,"B");
}

struct starpu_codelet cl_A = {
	.cpu_funcs = { cpu_codelet_A, NULL},
	.cuda_funcs = { cpu_codelet_A, NULL},
	.opencl_funcs = { cpu_codelet_A, NULL},
	.nbuffers = 0,
};

struct starpu_codelet cl_B = {
	.cpu_funcs = { cpu_codelet_B, NULL},
	.cuda_funcs = { cpu_codelet_B, NULL},
	.opencl_funcs = { cpu_codelet_B, NULL},
	.nbuffers = 0,
};

#define Ni	64

static unsigned ni = Ni;

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-iter") == 0)
		{
		        char *argptr;
			ni = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-h") == 0)
		{
			printf("usage : %s [-iter iter]\n", argv[0]);
		}
	}
}

int main(int argc __attribute__((unused)) , char **argv __attribute__((unused)))
{
	unsigned i, j;
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		exit(77);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	parse_args(argc, argv);

	FPRINTF(stderr, "ITER : %u\n", ni);

	{
		int array[ni];

		memset(array, 0, sizeof(array));

		for (i = 1; i < ni; i++)
		{
			for (j = 1; j < i; j++) {
				struct starpu_task *task_A = starpu_task_create();
				task_A->cl = &cl_A;
				task_A->cl_arg = &array[i];
				task_A->use_tag = 1;
				task_A->tag_id = TAG(0, i);

				ret = starpu_task_submit(task_A);
				if (ret == -ENODEV) goto enodev;
				STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
			}

			for (j = 1; j < i; j++) {
				struct starpu_task *task_B = starpu_task_create();
				task_B->cl = &cl_B;
				task_B->cl_arg = &array[i];
				task_B->use_tag = 1;
				task_B->tag_id = TAG(j, i);

				starpu_tag_declare_deps(TAG(j, i), 1, TAG(0, i));

				ret = starpu_task_submit(task_B);
				if (ret == -ENODEV) goto enodev;
				STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
			}
		}

		starpu_task_wait_for_all();
	}

	for (i = 1; i < ni; i++)
	{
		for (j = 0; j < i; j++)
			starpu_tag_remove(TAG(j, i));
	}

enodev:
	starpu_shutdown();

	FPRINTF(stderr, "TEST DONE ...\n");

	if (ret == -ENODEV) return 77; else return 0;
}
