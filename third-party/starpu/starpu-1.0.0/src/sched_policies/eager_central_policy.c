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

/*
 *	This is just the trivial policy where every worker use the same
 *	JOB QUEUE.
 */

#include <core/workers.h>
#include <sched_policies/fifo_queues.h>

/* the former is the actual queue, the latter some container */
static struct _starpu_fifo_taskq *fifo;

static pthread_cond_t sched_cond;
static pthread_mutex_t sched_mutex;

static void initialize_eager_center_policy(struct starpu_machine_topology *topology,
		   __attribute__ ((unused)) struct starpu_sched_policy *_policy)
{
	/* there is only a single queue in that trivial design */
	fifo = _starpu_create_fifo();

	_STARPU_PTHREAD_MUTEX_INIT(&sched_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&sched_cond, NULL);

	unsigned workerid;
	for (workerid = 0; workerid < topology->nworkers; workerid++)
		starpu_worker_set_sched_condition(workerid, &sched_cond, &sched_mutex);
}

static void deinitialize_eager_center_policy(__attribute__ ((unused)) struct starpu_machine_topology *topology,
		   __attribute__ ((unused)) struct starpu_sched_policy *_policy)
{
	//STARPU_ASSERT(_starpu_fifo_empty(fifo));

	/* deallocate the job queue */
	_starpu_destroy_fifo(fifo);
}

static int push_task_eager_policy(struct starpu_task *task)
{
	return _starpu_fifo_push_task(fifo, &sched_mutex, &sched_cond, task);
}

static struct starpu_task *pop_every_task_eager_policy(void)
{
	return _starpu_fifo_pop_every_task(fifo, &sched_mutex, starpu_worker_get_id());
}

static struct starpu_task *pop_task_eager_policy(void)
{
	return _starpu_fifo_pop_task(fifo, starpu_worker_get_id());
}

struct starpu_sched_policy _starpu_sched_eager_policy =
{
	.init_sched = initialize_eager_center_policy,
	.deinit_sched = deinitialize_eager_center_policy,
	.push_task = push_task_eager_policy,
	.pop_task = pop_task_eager_policy,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = pop_every_task_eager_policy,
	.policy_name = "eager",
	.policy_description = "greedy policy"
};
