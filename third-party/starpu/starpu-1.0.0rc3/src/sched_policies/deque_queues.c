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

/* Deque queues, ready for use by schedulers */

#include <starpu.h>
#include <common/config.h>
#include <core/workers.h>
#include <sched_policies/deque_queues.h>
#include <errno.h>
#include <common/utils.h>

struct _starpu_deque_jobq *_starpu_create_deque(void)
{
	struct _starpu_deque_jobq *deque;
	deque = (struct _starpu_deque_jobq *) malloc(sizeof(struct _starpu_deque_jobq));

	/* note that not all mechanisms (eg. the semaphore) have to be used */
	deque->jobq = _starpu_job_list_new();
	deque->njobs = 0;
	deque->nprocessed = 0;

	deque->exp_start = starpu_timing_now();
	deque->exp_len = 0.0;
	deque->exp_end = deque->exp_start;

	return deque;
}

void _starpu_destroy_deque(struct _starpu_deque_jobq *deque)
{
	_starpu_job_list_delete(deque->jobq);
	free(deque);
}

unsigned _starpu_get_deque_njobs(struct _starpu_deque_jobq *deque_queue)
{
	return deque_queue->njobs;
}

int _starpu_get_deque_nprocessed(struct _starpu_deque_jobq *deque_queue)
{
	return deque_queue->nprocessed;
}

struct starpu_task *_starpu_deque_pop_task(struct _starpu_deque_jobq *deque_queue, int workerid)
{
	struct _starpu_job *j = NULL;

	if ((deque_queue->njobs == 0) && _starpu_machine_is_running())
	{
		return NULL;
	}

	/* TODO find a task that suits workerid */
	for (j  = _starpu_job_list_begin(deque_queue->jobq);
	     j != _starpu_job_list_end(deque_queue->jobq);
	     j  = _starpu_job_list_next(j))
	{
		unsigned nimpl;
		STARPU_ASSERT(j);

		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
			if (starpu_worker_can_execute_task(workerid, j->task, nimpl))
			{
				j->nimpl = nimpl;
				j = _starpu_job_list_pop_front(deque_queue->jobq);
				_STARPU_TRACE_JOB_POP(j, 0);
				return j->task;
			}
	}

	return NULL;
}

struct _starpu_job_list *_starpu_deque_pop_every_task(struct _starpu_deque_jobq *deque_queue, pthread_mutex_t *sched_mutex, int workerid)
{
	struct _starpu_job_list *new_list, *old_list;

	/* block until some task is available in that queue */
	_STARPU_PTHREAD_MUTEX_LOCK(sched_mutex);

	if (deque_queue->njobs == 0)
	{
		new_list = NULL;
	}
	else
	{
		/* there is a task */
		old_list = deque_queue->jobq;
		new_list = _starpu_job_list_new();

		unsigned new_list_size = 0;

		struct _starpu_job *i;
		struct _starpu_job *next_job;
		/* note that this starts at the _head_ of the list, so we put
 		 * elements at the back of the new list */
		for(i = _starpu_job_list_begin(old_list);
			i != _starpu_job_list_end(old_list);
			i  = next_job)
		{
			unsigned nimpl;
			next_job = _starpu_job_list_next(i);

			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
			if (starpu_worker_can_execute_task(workerid, i->task, nimpl))
			{
				/* this elements can be moved into the new list */
				new_list_size++;

				_starpu_job_list_erase(old_list, i);
				_starpu_job_list_push_back(new_list, i);
				i->nimpl = nimpl;
			}
		}

		if (new_list_size == 0)
		{
			/* the new list is empty ... */
			_starpu_job_list_delete(new_list);
			new_list = NULL;
		}
		else
		{
			deque_queue->njobs -= new_list_size;
		}
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(sched_mutex);

	return new_list;
}
