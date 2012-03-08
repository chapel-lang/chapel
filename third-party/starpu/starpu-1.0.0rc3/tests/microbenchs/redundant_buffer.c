/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
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
#include "../helper.h"

#define N	10000

#define VECTORSIZE	1024

starpu_data_handle_t v_handle;
static unsigned *v;

static void opencl_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static void cuda_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static void cpu_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static struct starpu_codelet cl =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
	.opencl_funcs = {opencl_codelet_null, NULL},
        .nbuffers = 2,
	.modes = {STARPU_R, STARPU_R}
};


int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_malloc((void **)&v, VECTORSIZE*sizeof(unsigned));

	starpu_vector_data_register(&v_handle, 0, (uintptr_t)v, VECTORSIZE, sizeof(unsigned));

	unsigned iter;
	for (iter = 0; iter < N; iter++)
	{
		struct starpu_task *task = starpu_task_create();
		task->cl = &cl;

		task->handles[0] = v_handle;
		task->handles[1] = v_handle;

		int ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	starpu_data_unregister(v_handle);
	starpu_free(v);
	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	starpu_data_unregister(v_handle);
	starpu_free(v);
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
