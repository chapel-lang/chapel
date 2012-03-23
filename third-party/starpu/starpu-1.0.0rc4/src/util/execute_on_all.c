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

#include <starpu.h>
#include <common/config.h>
#include <core/jobs.h>
#include <core/task.h>

struct wrapper_func_args
{
	void (*func)(void *);
	void *arg;
};

static void wrapper_func(void *buffers[] __attribute__ ((unused)), void *_args)
{
	struct wrapper_func_args *args = (struct wrapper_func_args *) _args;
	args->func(args->arg);
}

/* execute func(arg) on each worker that matches the "where" flag */
void starpu_execute_on_each_worker(void (*func)(void *), void *arg, uint32_t where)
{
	int ret;
	unsigned worker;
	unsigned nworkers = starpu_worker_get_count();
	struct starpu_task *tasks[STARPU_NMAXWORKERS];

	/* create a wrapper codelet */
	struct starpu_codelet wrapper_cl =
	{
		.where = where,
		.cuda_funcs = {wrapper_func, NULL},
		.cpu_funcs = {wrapper_func, NULL},
		.opencl_funcs = {wrapper_func, NULL},
		/* XXX we do not handle Cell .. */
		.nbuffers = 0
	};

	struct wrapper_func_args args =
	{
		.func = func,
		.arg = arg
	};

	for (worker = 0; worker < nworkers; worker++)
	{
		tasks[worker] = starpu_task_create();

		tasks[worker]->cl = &wrapper_cl;
		tasks[worker]->cl_arg = &args;

		tasks[worker]->execute_on_a_specific_worker = 1;
		tasks[worker]->workerid = worker;

		tasks[worker]->detach = 0;
		tasks[worker]->destroy = 0;

#ifdef STARPU_USE_FXT
                struct _starpu_job *job = _starpu_get_job_associated_to_task(tasks[worker]);
                job->model_name = "execute_on_all_wrapper";
#endif

		_starpu_exclude_task_from_dag(tasks[worker]);

		ret = starpu_task_submit(tasks[worker]);
		if (ret == -ENODEV)
		{
			/* if the worker is not able to execute this tasks, we
			 * don't insist as this means the worker is not
			 * designated by the "where" bitmap */
			starpu_task_destroy(tasks[worker]);
			tasks[worker] = NULL;
		}
	}

	for (worker = 0; worker < nworkers; worker++)
	{
		if (tasks[worker])
		{
			ret = starpu_task_wait(tasks[worker]);
			STARPU_ASSERT(!ret);
			starpu_task_destroy(tasks[worker]);
		}
	}
}
