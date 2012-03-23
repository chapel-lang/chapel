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

/* This example shows how to submit a series of tasks in a chain of dependency:
 *
 * ... -> task (i) --> task (i+1) --> ...
 *
 * This is repeated several times
 */

#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

#include <starpu.h>

#ifdef STARPU_USE_GORDON
#include <gordon/null.h>
#endif

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)
#define TAG(i, iter)	((starpu_tag_t)  (((uint64_t)iter)<<32 | (i)) )

struct starpu_codelet cl = {};

#define Ni	64
#define Nk	256

static unsigned ni = Ni, nk = Nk;

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-iter") == 0)
		{
		        char *argptr;
			nk = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-i") == 0)
		{
		        char *argptr;
			ni = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-h") == 0)
		{
			printf("usage : %s [-iter iter] [-i i]\n", argv[0]);
		}
	}
}

void callback_cpu(void *argcb);

static void tag_cleanup_grid(unsigned ni, unsigned iter)
{
	unsigned i;

	for (i = 0; i < ni; i++)
		starpu_tag_remove(TAG(i,iter));
} 

static int create_task_grid(unsigned iter)
{
	int i;
	int ret;

/*	FPRINTF(stderr, "start iter %d ni %d...\n", iter, ni); */

	for (i = 0; i < ni; i++)
	{
		/* create a new task */
		struct starpu_task *task = starpu_task_create();

		task->cl = &cl;
		task->cl_arg = NULL;

		task->use_tag = 1;
		task->tag_id = TAG(i, iter);

		if (i != 0)
			starpu_tag_declare_deps(TAG(i,iter), 1, TAG(i-1,iter));

		ret = starpu_task_submit(task);
		if (ret == -ENODEV) return 77;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}
	return 0;
}

void cpu_codelet(void *descr[] __attribute__ ((unused)), void *_args __attribute__ ((unused)))
{
}

int main(int argc __attribute__((unused)) , char **argv __attribute__((unused)))
{
	unsigned i;
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		exit(77);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_SLOW_MACHINE
	ni /= 4;
	nk /= 16;
#endif

#ifdef STARPU_USE_GORDON
	/* load an empty kernel and get its identifier */
	unsigned gordon_null_kernel = load_gordon_null_kernel();
#endif

	parse_args(argc, argv);

	cl.cpu_funcs[0] = cpu_codelet;
	cl.cuda_funcs[0] = cpu_codelet;
	cl.opencl_funcs[0] = cpu_codelet;
#ifdef STARPU_USE_GORDON
	cl.gordon_func = gordon_null_kernel;
#endif
	cl.nbuffers = 0;

	FPRINTF(stderr, "ITER : %u\n", nk);

	for (i = 0; i < nk; i++)
	{
		ret = create_task_grid(i);
		if (ret == 77) goto enodev;

		starpu_tag_wait(TAG(ni-1, i));

		/* cleanup old grids ... */
		if (i > 1)
			tag_cleanup_grid(ni, i-1);
	}

	starpu_task_wait_for_all();

enodev:
	tag_cleanup_grid(ni, nk-1);

	starpu_shutdown();

	FPRINTF(stderr, "TEST DONE ...\n");

	return ret;
}
