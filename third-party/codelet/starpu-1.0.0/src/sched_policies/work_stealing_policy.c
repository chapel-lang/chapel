/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 * Copyright (C) 2012	Inria
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

/* Work stealing policy */

#include <float.h>

#include <core/workers.h>
#include <sched_policies/deque_queues.h>

static unsigned nworkers;
static unsigned last_pop_worker;
static unsigned last_push_worker;
static struct _starpu_deque_jobq *queue_array[STARPU_NMAXWORKERS];

static pthread_mutex_t global_sched_mutex;
static pthread_cond_t global_sched_cond;

/**
 * Keep track of the work performed from the beginning of the algorithm to make
 * better decisions about which queue to select when stealing or deferring work
 */
static int performed_total;

#ifdef USE_OVERLOAD

/**
 * Minimum number of task we wait for being processed before we start assuming
 * on which worker the computation would be faster.
 */
static int calibration_value = 0;

#endif /* USE_OVERLOAD */


/**
 * Return a worker from which a task can be stolen.
 * Selecting a worker is done in a round-robin fashion, unless
 * the worker previously selected doesn't own any task,
 * then we return the first non-empty worker.
 */
static unsigned select_victim_round_robin(void)
{
	unsigned worker = last_pop_worker;

	/* If the worker's queue is empty, let's try
	 * the next ones */
	while (!queue_array[worker]->njobs)
	{
		worker = (worker + 1) % nworkers;
		if (worker == last_pop_worker)
		{
			/* We got back to the first worker,
			 * don't go in infinite loop */
			break;
		}
	}

	last_pop_worker = (worker + 1) % nworkers;

	return worker;
}

/**
 * Return a worker to whom add a task.
 * Selecting a worker is done in a round-robin fashion.
 */
static unsigned select_worker_round_robin(void)
{
	unsigned worker = last_push_worker;

	last_push_worker = (last_push_worker + 1) % nworkers;

	return worker;
}

#ifdef USE_OVERLOAD

/**
 * Return a ratio helpful to determine whether a worker is suitable to steal
 * tasks from or to put some tasks in its queue.
 *
 * \return	a ratio with a positive or negative value, describing the current state of the worker :
 * 		a smaller value implies a faster worker with an relatively emptier queue : more suitable to put tasks in
 * 		a bigger value implies a slower worker with an reletively more replete queue : more suitable to steal tasks from
 */
static float overload_metric(unsigned id)
{
	float execution_ratio = 0.0f;
	float current_ratio = 0.0f;

	int nprocessed = _starpu_get_deque_nprocessed(queue_array[id]);
	unsigned njobs = _starpu_get_deque_njobs(queue_array[id]);

	/* Did we get enough information ? */
	if (performed_total > 0 && nprocessed > 0)
	{
		/* How fast or slow is the worker compared to the other workers */
		execution_ratio = (float) nprocessed / performed_total;
		/* How replete is its queue */
		current_ratio = (float) njobs / nprocessed;
	}
	else
	{
		return 0.0f;
	}

	return (current_ratio - execution_ratio);
}

/**
 * Return the most suitable worker from which a task can be stolen.
 * The number of previously processed tasks, total and local,
 * and the number of tasks currently awaiting to be processed
 * by the tasks are taken into account to select the most suitable
 * worker to steal task from.
 */
static unsigned select_victim_overload(void)
{
	unsigned worker;
	float  worker_ratio;
	unsigned best_worker = 0;
	float best_ratio = FLT_MIN;	

	/* Don't try to play smart until we get
	 * enough informations. */
	if (performed_total < calibration_value)
		return select_victim_round_robin();

	for (worker = 0; worker < nworkers; worker++)
	{
		worker_ratio = overload_metric(worker);

		if (worker_ratio > best_ratio)
		{
			best_worker = worker;
			best_ratio = worker_ratio;
		}
	}

	return best_worker;
}

/**
 * Return the most suitable worker to whom add a task.
 * The number of previously processed tasks, total and local,
 * and the number of tasks currently awaiting to be processed
 * by the tasks are taken into account to select the most suitable
 * worker to add a task to.
 */
static unsigned select_worker_overload(void)
{
	unsigned worker;
	float  worker_ratio;
	unsigned best_worker = 0;
	float best_ratio = FLT_MAX;

	/* Don't try to play smart until we get
	 * enough informations. */
	if (performed_total < calibration_value)
		return select_worker_round_robin();

	for (worker = 0; worker < nworkers; worker++)
	{
		worker_ratio = overload_metric(worker);

		if (worker_ratio < best_ratio)
		{
			best_worker = worker;
			best_ratio = worker_ratio;
		}
	}

	return best_worker;
}

#endif /* USE_OVERLOAD */


/**
 * Return a worker from which a task can be stolen.
 * This is a phony function used to call the right
 * function depending on the value of USE_OVERLOAD.
 */
static inline unsigned select_victim(void)
{
#ifdef USE_OVERLOAD
	return select_victim_overload();
#else
	return select_victim_round_robin();
#endif /* USE_OVERLOAD */
}

/**
 * Return a worker from which a task can be stolen.
 * This is a phony function used to call the right
 * function depending on the value of USE_OVERLOAD.
 */
static inline unsigned select_worker(void)
{
#ifdef USE_OVERLOAD
	return select_worker_overload();
#else
	return select_worker_round_robin();
#endif /* USE_OVERLOAD */
}


#ifdef STARPU_DEVEL
#warning TODO rewrite ... this will not scale at all now ...
#warning and the overload versions are useless with a global mutex ...
#endif

/**
 * Return a task to execute.
 * If possible from the calling worker queue, else
 * stealing from an other.
 * For now mutex must be locked before calling this function.
 */
static struct starpu_task *ws_pop_task(void)
{
	struct starpu_task *task;
	struct _starpu_deque_jobq *q;

	int workerid = starpu_worker_get_id();

	STARPU_ASSERT(workerid != -1);

	q = queue_array[workerid];

	task = _starpu_deque_pop_task(q, workerid);
	if (task)
	{
		/* there was a local task */
		performed_total++;
		q->nprocessed++;
		q->njobs--;
		return task;
	}

	/* we need to steal someone's job */
	unsigned victim = select_victim();
	struct _starpu_deque_jobq *victimq = queue_array[victim];

	task = _starpu_deque_pop_task(victimq, workerid);
	if (task)
	{
		_STARPU_TRACE_WORK_STEALING(q, workerid);
		performed_total++;

		/* Beware : we have to increase the number of processed tasks of
		 * the stealer, not the victim ! */
		q->nprocessed++;
		victimq->njobs--;
	}

	return task;
}

/**
 * Push a task in the calling worker's queue.
 * If the calling thread is not a worker, push
 * the task in a worker chosen on the fly.
 */
static int ws_push_task(struct starpu_task *task)
{
	struct _starpu_deque_jobq *deque_queue;
	struct _starpu_job *j = _starpu_get_job_associated_to_task(task); 
	int workerid = starpu_worker_get_id();

	_STARPU_PTHREAD_MUTEX_LOCK(&global_sched_mutex);

	/* If the current thread is not a worker but
	 * the main thread (-1), we find the better one to
	 * put task on its queue */
	if (workerid == -1)
		workerid = select_worker();

	deque_queue = queue_array[workerid];

	_STARPU_TRACE_JOB_PUSH(task, 0);
	_starpu_job_list_push_back(deque_queue->jobq, j);
	deque_queue->njobs++;

	_STARPU_PTHREAD_COND_SIGNAL(&global_sched_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&global_sched_mutex);

	return 0;
}

/**
 * Initializing the work stealing scheduler.
 */
static void initialize_ws_policy(struct starpu_machine_topology *topology,
		__attribute__ ((unused)) struct starpu_sched_policy *_policy)
{
	unsigned workerid;

	nworkers = topology->nworkers;
	last_pop_worker = 0;
	last_push_worker = 0;

	/**
	 * The first WS_POP_TASK will increase PERFORMED_TOTAL though no task was actually performed yet,
	 * we need to initialize it at -1.
	 */
	performed_total = -1;

	_STARPU_PTHREAD_MUTEX_INIT(&global_sched_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&global_sched_cond, NULL);

	for (workerid = 0; workerid < nworkers; workerid++)
	{
		queue_array[workerid] = _starpu_create_deque();

		/**
		 * The first WS_POP_TASK will increase NPROCESSED though no task was actually performed yet,
		 * we need to initialize it at -1.
		 */
		queue_array[workerid]->nprocessed = -1;
		queue_array[workerid]->njobs = 0;

		starpu_worker_set_sched_condition(workerid, &global_sched_cond, &global_sched_mutex);

#ifdef USE_OVERLOAD
		enum starpu_perf_archtype perf_arch;
		perf_arch = starpu_worker_get_perf_archtype(workerid);
		calibration_value += (unsigned int) starpu_worker_get_relative_speedup(perf_arch);
#endif /* USE_OVERLOAD */
	}
}

struct starpu_sched_policy _starpu_sched_ws_policy =
{
	.init_sched = initialize_ws_policy,
	.deinit_sched = NULL,
	.push_task = ws_push_task,
	.pop_task = ws_pop_task,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = NULL,
	.policy_name = "ws",
	.policy_description = "work stealing"
};
