/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 * Copyright (C) 2011  Télécom-SudParis
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

/* FIFO queues, ready for use by schedulers */

#include <pthread.h>
#include <sched_policies/fifo_queues.h>
#include <errno.h>
#include <common/utils.h>
#include <core/task.h>
#include <core/workers.h>

struct _starpu_fifo_taskq *_starpu_create_fifo(void)
{
	struct _starpu_fifo_taskq *fifo;
	fifo = (struct _starpu_fifo_taskq *) malloc(sizeof(struct _starpu_fifo_taskq));

	/* note that not all mechanisms (eg. the semaphore) have to be used */
	starpu_task_list_init(&fifo->taskq);
	fifo->ntasks = 0;
	fifo->nprocessed = 0;

	fifo->exp_start = starpu_timing_now();
	fifo->exp_len = 0.0;
	fifo->exp_end = fifo->exp_start;

	return fifo;
}

void _starpu_destroy_fifo(struct _starpu_fifo_taskq *fifo)
{
	free(fifo);
}

int _starpu_fifo_empty(struct _starpu_fifo_taskq *fifo)
{
	return fifo->ntasks == 0;
}

/* TODO: revert front/back? */
int _starpu_fifo_push_task(struct _starpu_fifo_taskq *fifo_queue, pthread_mutex_t *sched_mutex, pthread_cond_t *sched_cond, struct starpu_task *task)
{
	_STARPU_PTHREAD_MUTEX_LOCK(sched_mutex);

	_STARPU_TRACE_JOB_PUSH(task, 0);
	/* TODO: if prio, put at back */
	starpu_task_list_push_front(&fifo_queue->taskq, task);
	fifo_queue->ntasks++;
	fifo_queue->nprocessed++;

	_STARPU_PTHREAD_COND_SIGNAL(sched_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(sched_mutex);

	return 0;
}

struct starpu_task *_starpu_fifo_pop_task(struct _starpu_fifo_taskq *fifo_queue, int workerid)
{
	struct starpu_task *task;

	for (task  = starpu_task_list_begin(&fifo_queue->taskq);
	     task != starpu_task_list_end(&fifo_queue->taskq);
	     task  = starpu_task_list_next(task))
	{
		unsigned nimpl;
		STARPU_ASSERT(task);

		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
			if (starpu_worker_can_execute_task(workerid, task, nimpl))
			{
				_starpu_get_job_associated_to_task(task)->nimpl = nimpl;
				starpu_task_list_erase(&fifo_queue->taskq, task);
				fifo_queue->ntasks--;
				_STARPU_TRACE_JOB_POP(task, 0);
				return task;
			}
	}

	return NULL;
}

/* pop every task that can be executed on the calling driver */
struct starpu_task *_starpu_fifo_pop_every_task(struct _starpu_fifo_taskq *fifo_queue, pthread_mutex_t *sched_mutex, int workerid)
{
	struct starpu_task_list *old_list;
	unsigned size;

	struct starpu_task *new_list = NULL;
	struct starpu_task *new_list_tail = NULL;

	_STARPU_PTHREAD_MUTEX_LOCK(sched_mutex);

	size = fifo_queue->ntasks;

	if (size > 0)
	{
		old_list = &fifo_queue->taskq;
		unsigned new_list_size = 0;

		struct starpu_task *task, *next_task;
		/* note that this starts at the _head_ of the list, so we put
 		 * elements at the back of the new list */
		task = starpu_task_list_front(old_list);
		while (task)
		{
			unsigned nimpl;
			next_task = task->next;

			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
			if (starpu_worker_can_execute_task(workerid, task, nimpl))
			{
				/* this elements can be moved into the new list */
				new_list_size++;

				starpu_task_list_erase(old_list, task);

				if (new_list_tail)
				{
					new_list_tail->next = task;
					task->prev = new_list_tail;
					task->next = NULL;
					new_list_tail = task;
				}
				else
				{
					new_list = task;
					new_list_tail = task;
					task->prev = NULL;
					task->next = NULL;
				}
				_starpu_get_job_associated_to_task(task)->nimpl = nimpl;
				break;
			}

			task = next_task;
		}

		fifo_queue->ntasks -= new_list_size;
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(sched_mutex);

	return new_list;
}
