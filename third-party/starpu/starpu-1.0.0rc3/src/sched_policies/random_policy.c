/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

/* Policy attributing tasks randomly to workers */

#include <core/workers.h>
#include <sched_policies/fifo_queues.h>

static unsigned nworkers;

static pthread_cond_t sched_cond[STARPU_NMAXWORKERS];
static pthread_mutex_t sched_mutex[STARPU_NMAXWORKERS];

static int _random_push_task(struct starpu_task *task, unsigned prio)
{
	/* find the queue */
	unsigned worker;

	unsigned selected = 0;

	double alpha_sum = 0.0;

	for (worker = 0; worker < nworkers; worker++)
	{
		enum starpu_perf_archtype perf_arch = starpu_worker_get_perf_archtype(worker);
		alpha_sum += starpu_worker_get_relative_speedup(perf_arch);
	}

	double random = starpu_drand48()*alpha_sum;
//	_STARPU_DEBUG("my rand is %e\n", random);

	double alpha = 0.0;
	for (worker = 0; worker < nworkers; worker++)
	{
		enum starpu_perf_archtype perf_arch = starpu_worker_get_perf_archtype(worker);
		double worker_alpha = starpu_worker_get_relative_speedup(perf_arch);

		if (alpha + worker_alpha > random && starpu_worker_can_execute_task(worker, task, 0))
		{
			/* we found the worker */
			selected = worker;
			break;
		}

		alpha += worker_alpha;
	}

	/* we should now have the best worker in variable "selected" */
	return starpu_push_local_task(selected, task, prio);
}

static int random_push_task(struct starpu_task *task)
{
	return _random_push_task(task, !!task->priority);
}

static void initialize_random_policy(struct starpu_machine_topology *topology,
				     __attribute__ ((unused)) struct starpu_sched_policy *_policy)
{
	starpu_srand48(time(NULL));

	nworkers = topology->nworkers;

	unsigned workerid;
	for (workerid = 0; workerid < nworkers; workerid++)
	{
		_STARPU_PTHREAD_MUTEX_INIT(&sched_mutex[workerid], NULL);
		_STARPU_PTHREAD_COND_INIT(&sched_cond[workerid], NULL);

		starpu_worker_set_sched_condition(workerid, &sched_cond[workerid], &sched_mutex[workerid]);
	}
}

struct starpu_sched_policy _starpu_sched_random_policy =
{
	.init_sched = initialize_random_policy,
	.deinit_sched = NULL,
	.push_task = random_push_task,
	.pop_task = NULL,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = NULL,
	.policy_name = "random",
	.policy_description = "weighted random"
};
