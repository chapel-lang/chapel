/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

/* Distributed queues using performance modeling to assign tasks */

#include <limits.h>

#include <core/workers.h>
#include <sched_policies/fifo_queues.h>
#include <core/perfmodel/perfmodel.h>
#include <starpu_parameters.h>

static unsigned nworkers;
static struct _starpu_fifo_taskq *queue_array[STARPU_NMAXWORKERS];

static pthread_cond_t sched_cond[STARPU_NMAXWORKERS];
static pthread_mutex_t sched_mutex[STARPU_NMAXWORKERS];

static double alpha = _STARPU_DEFAULT_ALPHA;
static double beta = _STARPU_DEFAULT_BETA;
static double _gamma = _STARPU_DEFAULT_GAMMA;
static double idle_power = 0.0;

#ifdef STARPU_VERBOSE
static long int total_task_cnt = 0;
static long int ready_task_cnt = 0;
#endif

static int count_non_ready_buffers(struct starpu_task *task, uint32_t node)
{
	int cnt = 0;
	unsigned nbuffers = task->cl->nbuffers;
	unsigned index;

	for (index = 0; index < nbuffers; index++)
	{
		starpu_data_handle_t handle;

		handle = task->handles[index];

		int is_valid;
		starpu_data_query_status(handle, node, NULL, &is_valid, NULL);

		if (!is_valid)
			cnt++;
	}

	return cnt;
}

static struct starpu_task *_starpu_fifo_pop_first_ready_task(struct _starpu_fifo_taskq *fifo_queue, unsigned node)
{
	struct starpu_task *task = NULL, *current;

	if (fifo_queue->ntasks == 0)
		return NULL;

	if (fifo_queue->ntasks > 0)
	{
		fifo_queue->ntasks--;

		task = starpu_task_list_back(&fifo_queue->taskq);

		int first_task_priority = task->priority;

		current = task;

		int non_ready_best = INT_MAX;

		while (current)
		{
			int priority = current->priority;

			if (priority <= first_task_priority)
			{
				int non_ready = count_non_ready_buffers(current, node);
				if (non_ready < non_ready_best)
				{
					non_ready_best = non_ready;
					task = current;

					if (non_ready == 0)
						break;
				}
			}

			current = current->prev;
		}

		starpu_task_list_erase(&fifo_queue->taskq, task);

		_STARPU_TRACE_JOB_POP(task, 0);
	}

	return task;
}

static struct starpu_task *dmda_pop_ready_task(void)
{
	struct starpu_task *task;

	int workerid = starpu_worker_get_id();
	struct _starpu_fifo_taskq *fifo = queue_array[workerid];

	unsigned node = starpu_worker_get_memory_node(workerid);

	task = _starpu_fifo_pop_first_ready_task(fifo, node);
	if (task)
	{
		double model = task->predicted;

		fifo->exp_len -= model;
		fifo->exp_start = starpu_timing_now() + model;
		fifo->exp_end = fifo->exp_start + fifo->exp_len;

#ifdef STARPU_VERBOSE
		if (task->cl)
		{
			int non_ready = count_non_ready_buffers(task, starpu_worker_get_memory_node(workerid));
			if (non_ready == 0)
				ready_task_cnt++;
		}

		total_task_cnt++;
#endif
	}

	return task;
}

static struct starpu_task *dmda_pop_task(void)
{
	struct starpu_task *task;

	int workerid = starpu_worker_get_id();
	struct _starpu_fifo_taskq *fifo = queue_array[workerid];

	task = _starpu_fifo_pop_task(fifo, workerid);
	if (task)
	{
		double model = task->predicted;

		fifo->exp_len -= model;
		fifo->exp_start = starpu_timing_now() + model;
		fifo->exp_end = fifo->exp_start + fifo->exp_len;

#ifdef STARPU_VERBOSE
		if (task->cl)
		{
			int non_ready = count_non_ready_buffers(task, starpu_worker_get_memory_node(workerid));
			if (non_ready == 0)
				ready_task_cnt++;
		}

		total_task_cnt++;
#endif
	}

	return task;
}



static struct starpu_task *dmda_pop_every_task(void)
{
	struct starpu_task *new_list;

	int workerid = starpu_worker_get_id();

	struct _starpu_fifo_taskq *fifo = queue_array[workerid];

	new_list = _starpu_fifo_pop_every_task(fifo, &sched_mutex[workerid], workerid);

	while (new_list)
	{
		double model = new_list->predicted;

		fifo->exp_len -= model;
		fifo->exp_start = starpu_timing_now() + model;
		fifo->exp_end = fifo->exp_start + fifo->exp_len;

		new_list = new_list->next;
	}

	return new_list;
}

static
int _starpu_fifo_push_sorted_task(struct _starpu_fifo_taskq *fifo_queue, pthread_mutex_t *sched_mutex, pthread_cond_t *sched_cond, struct starpu_task *task)
{
	struct starpu_task_list *list = &fifo_queue->taskq;

	_STARPU_PTHREAD_MUTEX_LOCK(sched_mutex);

	_STARPU_TRACE_JOB_PUSH(task, 0);

	if (list->head == NULL)
	{
		list->head = task;
		list->tail = task;
		task->prev = NULL;
		task->next = NULL;
	}
	else
	{
		struct starpu_task *current = list->head;
		struct starpu_task *prev = NULL;

		while (current)
		{
			if (current->priority >= task->priority)
				break;

			prev = current;
			current = current->next;
		}

		if (prev == NULL)
		{
			/* Insert at the front of the list */
			list->head->prev = task;
			task->prev = NULL;
			task->next = list->head;
			list->head = task;
		}
		else
		{
			if (current)
			{
				/* Insert between prev and current */
				task->prev = prev;
				prev->next = task;
				task->next = current;
				current->prev = task;
			}
			else
			{
				/* Insert at the tail of the list */
				list->tail->next = task;
				task->next = NULL;
				task->prev = list->tail;
				list->tail = task;
			}
		}
	}

	fifo_queue->ntasks++;
	fifo_queue->nprocessed++;

	_STARPU_PTHREAD_COND_SIGNAL(sched_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(sched_mutex);

	return 0;
}



static int push_task_on_best_worker(struct starpu_task *task, int best_workerid, double predicted, int prio)
{
	/* make sure someone coule execute that task ! */
	STARPU_ASSERT(best_workerid != -1);

	struct _starpu_fifo_taskq *fifo;
	fifo = queue_array[best_workerid];

	fifo->exp_end += predicted;
	fifo->exp_len += predicted;

	task->predicted = predicted;

	/* TODO predicted_transfer */

	unsigned memory_node = starpu_worker_get_memory_node(best_workerid);

	if (starpu_get_prefetch_flag())
		starpu_prefetch_task_input_on_node(task, memory_node);

	if (prio)
		return _starpu_fifo_push_sorted_task(queue_array[best_workerid],
			&sched_mutex[best_workerid], &sched_cond[best_workerid], task);
	else
		return _starpu_fifo_push_task(queue_array[best_workerid],
			&sched_mutex[best_workerid], &sched_cond[best_workerid], task);
}

/* TODO: factorize with dmda!! */
static int _dm_push_task(struct starpu_task *task, unsigned prio)
{
	/* find the queue */
	struct _starpu_fifo_taskq *fifo;
	unsigned worker;
	int best = -1;

	double best_exp_end = 0.0;
	double model_best = 0.0;

	int ntasks_best = -1;
	double ntasks_best_end = 0.0;
	int calibrating = 0;

	/* A priori, we know all estimations */
	int unknown = 0;

	unsigned best_impl = 0;
	unsigned nimpl;

	for (worker = 0; worker < nworkers; worker++)
	{
		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
		{
			double exp_end;

			fifo = queue_array[worker];

			/* Sometimes workers didn't take the tasks as early as we expected */
			fifo->exp_start = STARPU_MAX(fifo->exp_start, starpu_timing_now());
			fifo->exp_end = fifo->exp_start + fifo->exp_len;

			if (!starpu_worker_can_execute_task(worker, task, nimpl))
			{
				/* no one on that queue may execute this task */
				continue;
			}

			enum starpu_perf_archtype perf_arch = starpu_worker_get_perf_archtype(worker);
			double local_length = starpu_task_expected_length(task, perf_arch, nimpl);
			double ntasks_end = fifo->ntasks / starpu_worker_get_relative_speedup(perf_arch);

			//_STARPU_DEBUG("Scheduler dm: task length (%lf) worker (%u) kernel (%u) \n", local_length,worker,nimpl);

			if (ntasks_best == -1
			    || (!calibrating && ntasks_end < ntasks_best_end) /* Not calibrating, take better task */
			    || (!calibrating && isnan(local_length)) /* Not calibrating but this worker is being calibrated */
			    || (calibrating && isnan(local_length) && ntasks_end < ntasks_best_end) /* Calibrating, compete this worker with other non-calibrated */
				)
			{
				ntasks_best_end = ntasks_end;
				ntasks_best = worker;
				best_impl = nimpl;
			}

			if (isnan(local_length))
				/* we are calibrating, we want to speed-up calibration time
				 * so we privilege non-calibrated tasks (but still
				 * greedily distribute them to avoid dumb schedules) */
				calibrating = 1;

			if (isnan(local_length) || _STARPU_IS_ZERO(local_length))
				/* there is no prediction available for that task
				 * with that arch yet, so switch to a greedy strategy */
				unknown = 1;

			if (unknown)
				continue;

			exp_end = fifo->exp_start + fifo->exp_len + local_length;

			if (best == -1 || exp_end < best_exp_end)
			{
				/* a better solution was found */
				best_exp_end = exp_end;
				best = worker;
				model_best = local_length;
				best_impl = nimpl;
			}
		}
	}

	if (unknown)
	{
		best = ntasks_best;
		model_best = 0.0;
	}

	//_STARPU_DEBUG("Scheduler dm: kernel (%u)\n", best_impl);

	 _starpu_get_job_associated_to_task(task)->nimpl = best_impl;

	/* we should now have the best worker in variable "best" */
	return push_task_on_best_worker(task, best, model_best, prio);
}

static int _dmda_push_task(struct starpu_task *task, unsigned prio)
{
	/* find the queue */
	struct _starpu_fifo_taskq *fifo;
	unsigned worker;
	int best = -1;

	/* this flag is set if the corresponding worker is selected because
	   there is no performance prediction available yet */
	int forced_best = -1;

	double local_task_length[nworkers][STARPU_MAXIMPLEMENTATIONS];
	double local_data_penalty[nworkers][STARPU_MAXIMPLEMENTATIONS];
	double local_power[nworkers][STARPU_MAXIMPLEMENTATIONS];
	double exp_end[nworkers][STARPU_MAXIMPLEMENTATIONS];
	double max_exp_end = 0.0;

	double fitness[nworkers][STARPU_MAXIMPLEMENTATIONS];

	double best_exp_end = 10e240;
	double model_best = 0.0;
	//double penality_best = 0.0;

	int ntasks_best = -1;
	double ntasks_best_end = 0.0;
	int calibrating = 0;

	/* A priori, we know all estimations */
	int unknown = 0;

	unsigned best_impl = 0;
	unsigned nimpl;

	for (worker = 0; worker < nworkers; worker++)
	{
		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
		{
			fifo = queue_array[worker];

			/* Sometimes workers didn't take the tasks as early as we expected */
			fifo->exp_start = STARPU_MAX(fifo->exp_start, starpu_timing_now());
			fifo->exp_end = fifo->exp_start + fifo->exp_len;
			if (fifo->exp_end > max_exp_end)
				max_exp_end = fifo->exp_end;

			if (!starpu_worker_can_execute_task(worker, task, nimpl))
			{
				/* no one on that queue may execute this task */
				continue;
			}

			enum starpu_perf_archtype perf_arch = starpu_worker_get_perf_archtype(worker);
			local_task_length[worker][nimpl] = starpu_task_expected_length(task, perf_arch, nimpl);

			//_STARPU_DEBUG("Scheduler dmda: task length (%lf) worker (%u) kernel (%u) \n", local_task_length[worker][nimpl],worker,nimpl);

			unsigned memory_node = starpu_worker_get_memory_node(worker);
			local_data_penalty[worker][nimpl] = starpu_task_expected_data_transfer_time(memory_node, task);

			double ntasks_end = fifo->ntasks / starpu_worker_get_relative_speedup(perf_arch);

			if (ntasks_best == -1
			    || (!calibrating && ntasks_end < ntasks_best_end) /* Not calibrating, take better task */
			    || (!calibrating && isnan(local_task_length[worker][nimpl])) /* Not calibrating but this worker is being calibrated */
			    || (calibrating && isnan(local_task_length[worker][nimpl]) && ntasks_end < ntasks_best_end) /* Calibrating, compete this worker with other non-calibrated */
				)
			{
				ntasks_best_end = ntasks_end;
				ntasks_best = worker;
				best_impl = nimpl;
			}

			if (isnan(local_task_length[worker][nimpl]))
				/* we are calibrating, we want to speed-up calibration time
				 * so we privilege non-calibrated tasks (but still
				 * greedily distribute them to avoid dumb schedules) */
				calibrating = 1;

			if (isnan(local_task_length[worker][nimpl])
					|| _STARPU_IS_ZERO(local_task_length[worker][nimpl]))
				/* there is no prediction available for that task
				 * with that arch yet, so switch to a greedy strategy */
				unknown = 1;

			if (unknown)
					continue;

			exp_end[worker][nimpl] = fifo->exp_start + fifo->exp_len + local_task_length[worker][nimpl];

			if (exp_end[worker][nimpl] < best_exp_end)
			{
				/* a better solution was found */
				best_exp_end = exp_end[worker][nimpl];
				best_impl = nimpl;
			}

			local_power[worker][nimpl] = starpu_task_expected_power(task, perf_arch, nimpl);
			if (isnan(local_power[worker][nimpl]))
				local_power[worker][nimpl] = 0.;

		 }
	}

	if (unknown)
		forced_best = ntasks_best;

	double best_fitness = -1;

	if (forced_best == -1)
	{
		for (worker = 0; worker < nworkers; worker++)
		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
		{
			if (!starpu_worker_can_execute_task(worker, task, nimpl))
			{
				/* no one on that queue may execute this task */
				continue;
			}

			fitness[worker][nimpl] = alpha*(exp_end[worker][nimpl] - best_exp_end)
				+ beta*(local_data_penalty[worker][nimpl])
				+ _gamma*(local_power[worker][nimpl]);

			if (exp_end[worker][nimpl] > max_exp_end)
			{
				/* This placement will make the computation
				 * longer, take into account the idle
				 * consumption of other cpus */
				fitness[worker][nimpl] += _gamma * idle_power * (exp_end[worker][nimpl] - max_exp_end) / 1000000.0;
			}

			if (best == -1 || fitness[worker][nimpl] < best_fitness)
			{
				/* we found a better solution */
				best_fitness = fitness[worker][nimpl];
				best = worker;
				best_impl = nimpl;

				//			_STARPU_DEBUG("best fitness (worker %d) %e = alpha*(%e) + beta(%e) +gamma(%e)\n", worker, best_fitness, exp_end[worker][nimpl] - best_exp_end, local_data_penalty[worker][nimpl], local_power[worker][nimpl]);
			}
		}
	}

	STARPU_ASSERT(forced_best != -1 || best != -1);

	if (forced_best != -1)
	{
		/* there is no prediction available for that task
		 * with that arch we want to speed-up calibration time
		 * so we force this measurement */
		best = forced_best;
		model_best = 0.0;
		//penality_best = 0.0;
	}
	else
	{
		model_best = local_task_length[best][best_impl];
		//penality_best = local_data_penalty[best][best_impl];
	}


	//_STARPU_DEBUG("Scheduler dmda: kernel (%u)\n", best_impl);
	 _starpu_get_job_associated_to_task(task)->nimpl = best_impl;

	/* we should now have the best worker in variable "best" */
	return push_task_on_best_worker(task, best, model_best, prio);
}

static int dmda_push_sorted_task(struct starpu_task *task)
{
	return _dmda_push_task(task, 1);
}

static int dm_push_task(struct starpu_task *task)
{
	return _dm_push_task(task, 0);
}

static int dmda_push_task(struct starpu_task *task)
{
	return _dmda_push_task(task, 0);
}

static void initialize_dmda_policy(struct starpu_machine_topology *topology,
	 __attribute__ ((unused)) struct starpu_sched_policy *_policy)
{
	nworkers = topology->nworkers;

	const char *strval_alpha = getenv("STARPU_SCHED_ALPHA");
	if (strval_alpha)
		alpha = atof(strval_alpha);

	const char *strval_beta = getenv("STARPU_SCHED_BETA");
	if (strval_beta)
		beta = atof(strval_beta);

	const char *strval_gamma = getenv("STARPU_SCHED_GAMMA");
	if (strval_gamma)
		_gamma = atof(strval_gamma);

	const char *strval_idle_power = getenv("STARPU_IDLE_POWER");
	if (strval_idle_power)
		idle_power = atof(strval_idle_power);

	unsigned workerid;
	for (workerid = 0; workerid < nworkers; workerid++)
	{
		queue_array[workerid] = _starpu_create_fifo();

		_STARPU_PTHREAD_MUTEX_INIT(&sched_mutex[workerid], NULL);
		_STARPU_PTHREAD_COND_INIT(&sched_cond[workerid], NULL);

		starpu_worker_set_sched_condition(workerid, &sched_cond[workerid], &sched_mutex[workerid]);
	}
}

static void initialize_dmda_sorted_policy(struct starpu_machine_topology *topology,
					struct starpu_sched_policy *_policy)
{
	initialize_dmda_policy(topology, _policy);

	/* The application may use any integer */
	starpu_sched_set_min_priority(INT_MIN);
	starpu_sched_set_max_priority(INT_MAX);
}

static void deinitialize_dmda_policy(struct starpu_machine_topology *topology,
	 __attribute__ ((unused)) struct starpu_sched_policy *_policy)
{
	unsigned workerid;
	for (workerid = 0; workerid < topology->nworkers; workerid++)
		_starpu_destroy_fifo(queue_array[workerid]);

	_STARPU_DEBUG("total_task_cnt %ld ready_task_cnt %ld -> %f\n", total_task_cnt, ready_task_cnt, (100.0f*ready_task_cnt)/total_task_cnt);
}

/* TODO: use post_exec_hook to fix the expected start */
struct starpu_sched_policy _starpu_sched_dm_policy =
{
	.init_sched = initialize_dmda_policy,
	.deinit_sched = deinitialize_dmda_policy,
	.push_task = dm_push_task,
	.pop_task = dmda_pop_task,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = dmda_pop_every_task,
	.policy_name = "dm",
	.policy_description = "performance model"
};

struct starpu_sched_policy _starpu_sched_dmda_policy =
{
	.init_sched = initialize_dmda_policy,
	.deinit_sched = deinitialize_dmda_policy,
	.push_task = dmda_push_task,
	.pop_task = dmda_pop_task,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = dmda_pop_every_task,
	.policy_name = "dmda",
	.policy_description = "data-aware performance model"
};

struct starpu_sched_policy _starpu_sched_dmda_sorted_policy =
{
	.init_sched = initialize_dmda_sorted_policy,
	.deinit_sched = deinitialize_dmda_policy,
	.push_task = dmda_push_sorted_task,
	.pop_task = dmda_pop_ready_task,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = dmda_pop_every_task,
	.policy_name = "dmdas",
	.policy_description = "data-aware performance model (sorted)"
};

struct starpu_sched_policy _starpu_sched_dmda_ready_policy =
{
	.init_sched = initialize_dmda_policy,
	.deinit_sched = deinitialize_dmda_policy,
	.push_task = dmda_push_task,
	.pop_task = dmda_pop_ready_task,
	.pre_exec_hook = NULL,
	.post_exec_hook = NULL,
	.pop_every_task = dmda_pop_every_task,
	.policy_name = "dmdar",
	.policy_description = "data-aware performance model (ready)"
};
