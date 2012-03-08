/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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
#include <starpu_cuda.h>
#include <stdlib.h>
#include "../helper.h"

#ifdef STARPU_SLOW_MACHINE
#define NLOOPS		100
#else
#define NLOOPS		1000
#endif
#define VECTORSIZE	1024

static starpu_data_handle_t v_handle;

/*
 *	Memset
 */

#ifdef STARPU_USE_CUDA
static void cuda_memset_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	char *buf = (char *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned length = STARPU_VECTOR_GET_NX(descr[0]);

	cudaMemsetAsync(buf, 42, length, starpu_cuda_get_local_stream());
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void cpu_memset_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	char *buf = (char *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned length = STARPU_VECTOR_GET_NX(descr[0]);

	memset(buf, 42, length);
}

static struct starpu_codelet memset_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {cpu_memset_codelet, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_memset_codelet, NULL},
#endif
	.nbuffers = 1,
	.modes = {STARPU_W}
};

/*
 *	Check content
 */

static void cpu_check_content_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	char *buf = (char *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned length = STARPU_VECTOR_GET_NX(descr[0]);

	unsigned i;
	for (i = 0; i < length; i++)
	{
		if (buf[i] != 42)
		{
			FPRINTF(stderr, "buf[%u] is %c while it should be %c\n", i, buf[i], 42);
			exit(-1);
		}
	}
}

static struct starpu_codelet check_content_cl =
{
	.where = STARPU_CPU,
	.cpu_funcs = {cpu_check_content_codelet, NULL},
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

	unsigned loop;
	for (loop = 0; loop < NLOOPS; loop++)
	{
		struct starpu_task *memset_task;
		struct starpu_task *check_content_task;

		memset_task = starpu_task_create();
		memset_task->cl = &memset_cl;
		memset_task->handles[0] = v_handle;
		memset_task->detach = 0;

		ret = starpu_task_submit(memset_task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

		ret = starpu_task_wait(memset_task);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

		check_content_task = starpu_task_create();
		check_content_task->cl = &check_content_cl;
		check_content_task->handles[0] = v_handle;
		check_content_task->detach = 0;

		ret = starpu_task_submit(check_content_task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

		ret = starpu_task_wait(check_content_task);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait");

		starpu_data_invalidate(v_handle);
	}

	/* this should get rid of automatically allocated buffers */
	starpu_data_unregister(v_handle);

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	starpu_data_unregister(v_handle);
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
