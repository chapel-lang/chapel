/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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
#include <pthread.h>
#include "../helper.h"

#ifdef STARPU_SLOW_MACHINE
#define NTASKS	1000
#else
#define NTASKS	10000
#endif
#define VECTORSIZE	1024
#define TASKDURATION	24U

#define SYMBOL	"sleep"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static unsigned finished = 0;
static unsigned cnt = NTASKS;

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

static void codelet_sleep(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	usleep(TASKDURATION);
}

static struct starpu_perfmodel model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = NULL /* to be defined later */
};

static struct starpu_codelet cl =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {codelet_sleep, NULL},
	.cuda_funcs = {codelet_sleep, NULL},
        .opencl_funcs = {codelet_sleep, NULL},
	.nbuffers = 1,
	.modes = {STARPU_R},
	.model =  &model
};

static char symbolname[128];

int main(int argc, char **argv)
{
	int ret;
	starpu_data_handle_t handle;
	float *buffer;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* create data */
	starpu_malloc((void **)&buffer, NTASKS*VECTORSIZE*sizeof(char));

	/* declare data to StarPU */
	starpu_vector_data_register(&handle, 0, (uintptr_t)buffer,
					NTASKS*VECTORSIZE, sizeof(char));

	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = NTASKS
	};

	starpu_data_partition(handle, &f);

	snprintf(symbolname, 128, "overlap_sleep_%d_%u", VECTORSIZE, TASKDURATION);

	model.symbol = symbolname;

	unsigned iter;
	for (iter = 0; iter < NTASKS; iter++)
	{
		struct starpu_task *task = starpu_task_create();
		task->cl = &cl;

		task->handles[0] = starpu_data_get_sub_data(handle, 1, iter);

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

	starpu_data_unpartition(handle, 0);
	starpu_data_unregister(handle);
	starpu_free(buffer);
	starpu_shutdown();

	STARPU_RETURN(EXIT_SUCCESS);

enodev:
	starpu_data_unregister(handle);
	starpu_free(buffer);
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	STARPU_RETURN(STARPU_TEST_SKIPPED);
}
