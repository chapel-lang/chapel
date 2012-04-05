/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010-2012  Centre National de la Recherche Scientifique
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

#include <pthread.h>
#include <starpu.h>

#define NTASKS	32000
#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

struct starpu_task_list sched_list;

static pthread_cond_t sched_cond;
static pthread_mutex_t sched_mutex;

static void init_dummy_sched(struct starpu_machine_topology *topology,
			struct starpu_sched_policy *policy)
{
	/* Create a linked-list of tasks and a condition variable to protect it */
	starpu_task_list_init(&sched_list);

	pthread_mutex_init(&sched_mutex, NULL);
	pthread_cond_init(&sched_cond, NULL);

	unsigned workerid;
	for (workerid = 0; workerid < topology->nworkers; workerid++)
		starpu_worker_set_sched_condition(workerid, &sched_cond, &sched_mutex);

	FPRINTF(stderr, "Initialising Dummy scheduler\n");
}

static void deinit_dummy_sched(struct starpu_machine_topology *topology,
				struct starpu_sched_policy *policy)
{
	STARPU_ASSERT(starpu_task_list_empty(&sched_list));

	pthread_cond_destroy(&sched_cond);
	pthread_mutex_destroy(&sched_mutex);

	FPRINTF(stderr, "Destroying Dummy scheduler\n");
}

static int push_task_dummy(struct starpu_task *task)
{
	pthread_mutex_lock(&sched_mutex);

	starpu_task_list_push_front(&sched_list, task);

	pthread_cond_signal(&sched_cond);

	pthread_mutex_unlock(&sched_mutex);

	return 0;
}

/* The mutex associated to the calling worker is already taken by StarPU */
static struct starpu_task *pop_task_dummy(void)
{
	/* NB: In this simplistic strategy, we assume that all workers are able
	 * to execute all tasks, otherwise, it would have been necessary to go
	 * through the entire list until we find a task that is executable from
	 * the calling worker. So we just take the head of the list and give it
	 * to the worker. */
	return starpu_task_list_pop_back(&sched_list);
}

static struct starpu_sched_policy dummy_sched_policy =
{
	.init_sched = init_dummy_sched,
	.deinit_sched = deinit_dummy_sched,
	.push_task = push_task_dummy,
	.pop_task = pop_task_dummy,
	.post_exec_hook = NULL,
	.pop_every_task = NULL,
	.policy_name = "dummy",
	.policy_description = "dummy scheduling strategy"
};

static struct starpu_conf conf =
{
	.sched_policy_name = NULL,
	.sched_policy = &dummy_sched_policy,
	.ncpus = -1,
	.ncuda = -1,
        .nopencl = -1,
	.nspus = -1,
	.use_explicit_workers_bindid = 0,
	.use_explicit_workers_cuda_gpuid = 0,
	.use_explicit_workers_opencl_gpuid = 0,
	.calibrate = 0
};

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet dummy_codelet =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {dummy_func, NULL},
	.cuda_funcs = {dummy_func, NULL},
        .opencl_funcs = {dummy_func, NULL},
	.model = NULL,
	.nbuffers = 0
};


int main(int argc, char **argv)
{
	int ntasks = NTASKS;
	int ret;

	ret = starpu_init(&conf);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_SLOW_MACHINE
	ntasks /= 100;
#endif

	unsigned i;
	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task *task = starpu_task_create();
	
		task->cl = &dummy_codelet;
		task->cl_arg = NULL;
	
		ret = starpu_task_submit(task);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	starpu_task_wait_for_all();

	starpu_shutdown();

	return 0;
}
