/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#ifndef __STACK_QUEUES_H__
#define __STACK_QUEUES_H__

#include <starpu.h>
#include <common/config.h>
#include <core/jobs.h>

struct _starpu_stack_jobq
{
	/* the actual list */
	struct _starpu_job_list *jobq;

	/* the number of tasks currently in the queue */
	unsigned njobs;

	/* the number of tasks that were processed */
	unsigned nprocessed;

	/* only meaningful if the queue is only used by a single worker */
	double exp_start; /* Expected start date of first task in the queue */
	double exp_end; /* Expected end date of last task in the queue */
	double exp_len; /* Expected duration of the set of tasks in the queue */
};

struct _starpu_stack_jobq *_starpu_create_stack(void);

void _starpu_stack_push_task(struct _starpu_stack_jobq *stack, pthread_mutex_t *sched_mutex, pthread_cond_t *sched_cond, struct _starpu_job *task);

struct _starpu_job *_starpu_stack_pop_task(struct _starpu_stack_jobq *stack, pthread_mutex_t *sched_mutex, int workerid);

void _starpu_init_stack_queues_mechanisms(void);


unsigned _starpu_get_stack_njobs(struct _starpu_stack_jobq *stack);
unsigned _starpu_get_stack_nprocessed(struct _starpu_stack_jobq *stack);


#endif // __STACK_QUEUES_H__
