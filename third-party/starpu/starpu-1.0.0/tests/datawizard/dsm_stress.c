/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#define N	10000

#define VECTORSIZE	1024

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static unsigned finished = 0;

static unsigned cnt = N;

starpu_data_handle_t v_handle, v_handle2;
static unsigned *v;
static unsigned *v2;

static void callback(void *arg)
{
	unsigned res = STARPU_ATOMIC_ADD(&cnt, -1);

	if (res == 0)
	{
		_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
		finished = 1;
		_STARPU_PTHREAD_COND_SIGNAL(&cond);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
	}
}



static void cuda_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static void opencl_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static void cpu_codelet_null(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static enum starpu_access_mode select_random_mode(void)
{
	int r = rand();

	switch (r % 3)
	{
		case 0:
			return STARPU_R;
		case 1:
			return STARPU_W;
		case 2:
			return STARPU_RW;
	};
	return STARPU_RW;
}

static struct starpu_codelet cl_r_r =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_R}
};

static struct starpu_codelet cl_r_w =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_W}
};

static struct starpu_codelet cl_r_rw =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_RW}
};

static struct starpu_codelet cl_w_r =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_W, STARPU_R}
};

static struct starpu_codelet cl_w_w =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_W, STARPU_W}
};

static struct starpu_codelet cl_w_rw =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_W, STARPU_RW}
};

static struct starpu_codelet cl_rw_r =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_RW, STARPU_R}
};

static struct starpu_codelet cl_rw_w =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_RW, STARPU_W}
};

static struct starpu_codelet cl_rw_rw =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {cpu_codelet_null, NULL},
	.cuda_funcs = {cuda_codelet_null, NULL},
        .opencl_funcs = {opencl_codelet_null, NULL},
	.nbuffers = 2,
	.modes = {STARPU_RW, STARPU_RW}
};


int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	ret = starpu_malloc((void **)&v, VECTORSIZE*sizeof(unsigned));
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_malloc");
	ret = starpu_malloc((void **)&v2, VECTORSIZE*sizeof(unsigned));
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_malloc");

	starpu_vector_data_register(&v_handle, 0, (uintptr_t)v, VECTORSIZE, sizeof(unsigned));
	starpu_vector_data_register(&v_handle2, 0, (uintptr_t)v2, VECTORSIZE, sizeof(unsigned));

	unsigned iter;
	for (iter = 0; iter < N; iter++)
	{
		struct starpu_task *task = starpu_task_create();

		task->handles[0] = v_handle;
		task->handles[1] = v_handle2;

		enum starpu_access_mode mode0 = select_random_mode();
		enum starpu_access_mode mode1 = select_random_mode();

		if (mode0 == STARPU_R && mode1 == STARPU_R)
			task->cl = &cl_r_r;
		else if (mode0 == STARPU_R && mode1 == STARPU_W)
			task->cl = &cl_r_w;
		else if (mode0 == STARPU_R && mode1 == STARPU_RW)
			task->cl = &cl_r_rw;
		else if (mode0 == STARPU_W && mode1 == STARPU_R)
			task->cl = &cl_w_r;
		else if (mode0 == STARPU_W && mode1 == STARPU_W)
			task->cl = &cl_w_w;
		else if (mode0 == STARPU_W && mode1 == STARPU_RW)
			task->cl = &cl_w_rw;
		else if (mode0 == STARPU_RW && mode1 == STARPU_R)
			task->cl = &cl_rw_r;
		else if (mode0 == STARPU_RW && mode1 == STARPU_W)
			task->cl = &cl_rw_w;
		else if (mode0 == STARPU_RW && mode1 == STARPU_RW)
			task->cl = &cl_rw_rw;

		task->callback_func = callback;
		task->callback_arg = NULL;

		int ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	if (!finished)
		_STARPU_PTHREAD_COND_WAIT(&cond, &mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	starpu_data_unregister(v_handle);
	starpu_data_unregister(v_handle2);
	starpu_free(v);
	starpu_free(v2);
	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	starpu_data_unregister(v_handle);
	starpu_data_unregister(v_handle2);
	starpu_free(v);
	starpu_free(v2);
	starpu_shutdown();
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	return STARPU_TEST_SKIPPED;
}
