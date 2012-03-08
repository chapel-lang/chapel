/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <starpu.h>
#include <stdlib.h>
#include <pthread.h>
#include "../helper.h"

#ifdef STARPU_SLOW_MACHINE
#define N		100
#else
#define N		1000
#endif

#define VECTORSIZE	1024

starpu_data_handle_t v_handle;
static unsigned *v;

static void callback(void *arg)
{
	unsigned node = (unsigned)(uintptr_t) arg;

	starpu_data_prefetch_on_node(v_handle, node, 1);
}

static void codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
//	fprintf(stderr, "pif\n");
//	fflush(stderr);
}

static struct starpu_codelet cl_r =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {codelet_null, NULL},
	.cuda_funcs = {codelet_null, NULL},
	.opencl_funcs = {codelet_null, NULL},
	.nbuffers = 1,
	.modes = {STARPU_R}
};

static struct starpu_codelet cl_w =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {codelet_null, NULL},
	.cuda_funcs = {codelet_null, NULL},
	.opencl_funcs = {codelet_null, NULL},
	.nbuffers = 1,
	.modes = {STARPU_W}
};

static struct starpu_codelet cl_rw =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {codelet_null, NULL},
	.cuda_funcs = {codelet_null, NULL},
	.opencl_funcs = {codelet_null, NULL},
	.nbuffers = 1,
	.modes = {STARPU_RW}
};

static struct starpu_codelet *select_codelet_with_random_mode(void)
{
	int r = rand();

	switch (r % 3)
	{
		case 0:
			return &cl_r;
		case 1:
			return &cl_w;
		case 2:
			return &cl_rw;
	};
	return &cl_rw;
}

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_malloc((void **)&v, VECTORSIZE*sizeof(unsigned));
	starpu_vector_data_register(&v_handle, 0, (uintptr_t)v, VECTORSIZE, sizeof(unsigned));

	unsigned nworker = starpu_worker_get_count();

	unsigned iter, worker;
	for (iter = 0; iter < N; iter++)
	{
		for (worker = 0; worker < nworker; worker++)
		{
			/* synchronous prefetch */
			unsigned node = starpu_worker_get_memory_node(worker);
			ret = starpu_data_prefetch_on_node(v_handle, node, 0);
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_prefetch_on_node");

			/* execute a task */
			struct starpu_task *task = starpu_task_create();

			task->handles[0] = v_handle;
			task->cl = select_codelet_with_random_mode();
			task->synchronous = 1;

			int ret = starpu_task_submit(task);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}
	}

	for (iter = 0; iter < N; iter++)
	{
		for (worker = 0; worker < nworker; worker++)
		{
			/* asynchronous prefetch */
			unsigned node = starpu_worker_get_memory_node(worker);
			ret = starpu_data_prefetch_on_node(v_handle, node, 1);
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_prefetch_on_node");

			/* execute a task */
			struct starpu_task *task = starpu_task_create();

			task->handles[0] = v_handle;
			task->cl = select_codelet_with_random_mode();
			task->callback_func = callback;
			task->callback_arg = (void*)(uintptr_t) starpu_worker_get_memory_node((worker+1)%nworker);

			task->synchronous = 0;

			int ret = starpu_task_submit(task);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	starpu_data_unregister(v_handle);
	starpu_free(v);
	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	starpu_free(v);
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
