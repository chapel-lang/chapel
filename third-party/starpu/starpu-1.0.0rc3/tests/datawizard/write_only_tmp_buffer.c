/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <starpu.h>
#include <stdlib.h>
#include "../helper.h"

#define VECTORSIZE	1024

starpu_data_handle_t v_handle;

#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>

static void opencl_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	cl_mem buf = (cl_mem)STARPU_VECTOR_GET_DEV_HANDLE(descr[0]);
        char ptr = 42;
        cl_command_queue queue;
        int id = starpu_worker_get_id();
        int devid = starpu_worker_get_devid(id);

        starpu_opencl_get_queue(devid, &queue);
        clEnqueueWriteBuffer(queue, buf, CL_TRUE, 0, sizeof(char), &ptr, 0, NULL, NULL);
}
#endif

#ifdef STARPU_USE_CUDA
static void cuda_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	char *buf = (char *)STARPU_VECTOR_GET_PTR(descr[0]);

	cudaMemset(buf, 42, 1);
}
#endif

static void cpu_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	char *buf = (char *)STARPU_VECTOR_GET_PTR(descr[0]);

	*buf = 42;
}

static void display_var(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	char *buf = (char *)STARPU_VECTOR_GET_PTR(descr[0]);
	if (*buf != 42)
	{
		fprintf(stderr, "Value = %c (should be %c)\n", *buf, 42);
		exit(-1);
	}
}

static struct starpu_codelet cl =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_codelet_null, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {opencl_codelet_null, NULL},
#endif
	.nbuffers = 1,
	.modes = {STARPU_W}
};

static struct starpu_codelet display_cl =
{
	.where = STARPU_CPU,
	.cpu_funcs = {display_var, NULL},
	.nbuffers = 1,
	.modes = {STARPU_R}
};


int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* The buffer should never be explicitely allocated */
	starpu_vector_data_register(&v_handle, (uint32_t)-1, (uintptr_t)NULL, VECTORSIZE, sizeof(char));

	struct starpu_task *task = starpu_task_create();
	task->cl = &cl;
	task->handles[0] = v_handle;
	task->detach = 0;

	ret = starpu_task_submit(task);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	ret = starpu_task_wait(task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

	task = starpu_task_create();
	task->cl = &display_cl;
	task->handles[0] = v_handle;
	task->detach = 0;

	ret = starpu_task_submit(task);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	ret = starpu_task_wait(task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

	/* this should get rid of automatically allocated buffers */
	starpu_data_unregister(v_handle);
	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	starpu_data_unregister(v_handle);
	starpu_shutdown();
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	return STARPU_TEST_SKIPPED;
}
