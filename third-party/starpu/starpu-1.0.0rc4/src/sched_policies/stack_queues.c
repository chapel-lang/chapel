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

/* Stack queues, ready for use by schedulers */

#include <starpu.h>
#include <sched_policies/stack_queues.h>
#include <errno.h>
#include <common/utils.h>

/* keep track of the total number of jobs to be scheduled to avoid infinite
 * polling when there are really few jobs in the overall queue */
static unsigned total_number_of_jobs;

void _starpu_init_stack_queues_mechanisms(void)
{
	total_number_of_jobs = 0;
}

struct _starpu_stack_jobq *_starpu_create_stack(void)
{
	struct _starpu_stack_jobq *stack;
	stack = (struct _starpu_stack_jobq *) malloc(sizeof(struct _starpu_stack_jobq));

	stack->jobq = _starpu_job_list_new();
	stack->njobs = 0;
	stack->nprocessed = 0;

	stack->exp_start = starpu_timing_now();
	stack->exp_len = 0.0;
	stack->exp_end = stack->exp_start;

	return stack;
}

unsigned _starpu_get_stack_njobs(struct _starpu_stack_jobq *stack_queue)
{
	return stack_queue->njobs;
}

unsigned _starpu_get_stack_nprocessed(struct _starpu_stack_jobq *stack_queue)
{
	return stack_queue->nprocessed;
}

void _starpu_stack_push_task(struct _starpu_stack_jobq *stack_queue, pthread_mutex_t *sched_mutex, pthread_cond_t *sched_cond, struct _starpu_job *task)
{
	_STARPU_PTHREAD_MUTEX_LOCK(sched_mutex);
	total_number_of_jobs++;

	_STARPU_TRACE_JOB_PUSH(task, 0);
	if (task->task->priority)
		_starpu_job_list_push_back(stack_queue->jobq, task);
	else
		_starpu_job_list_push_front(stack_queue->jobq, task);
	stack_queue->njobs++;
	stack_queue->nprocessed++;

	_STARPU_PTHREAD_COND_SIGNAL(sched_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(sched_mutex);
}

struct _starpu_job *_starpu_stack_pop_task(struct _starpu_stack_jobq *stack_queue, pthread_mutex_t *sched_mutex, int workerid __attribute__ ((unused)))
{
	struct _starpu_job *j = NULL;

	if (stack_queue->njobs == 0)
		return NULL;

	/* TODO find a task that suits workerid */
	if (stack_queue->njobs > 0)
	{
		/* there is a task */
		j = _starpu_job_list_pop_back(stack_queue->jobq);

		STARPU_ASSERT(j);
		stack_queue->njobs--;

		_STARPU_TRACE_JOB_POP(j, 0);

		/* we are sure that we got it now, so at worst, some people thought
		 * there remained some work and will soon discover it is not true */
		_STARPU_PTHREAD_MUTEX_LOCK(sched_mutex);
		total_number_of_jobs--;
		_STARPU_PTHREAD_MUTEX_UNLOCK(sched_mutex);
	}

	return j;

}
