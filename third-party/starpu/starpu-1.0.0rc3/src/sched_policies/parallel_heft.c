/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#include <float.h>
#include <limits.h>
#include <core/workers.h>
#include <core/perfmodel/perfmodel.h>
#include <starpu_parameters.h>
#include <common/barrier.h>
#include <sched_policies/detect_combined_workers.h>

#ifndef DBL_MIN
#define DBL_MIN __DBL_MIN__
#endif

#ifndef DBL_MAX
#define DBL_MAX __DBL_MAX__
#endif

static unsigned nworkers, ncombinedworkers;
//static enum starpu_perf_archtype applicable_perf_archtypes[STARPU_NARCH_VARIATIONS];
//static unsigned napplicable_perf_archtypes = 0;

static pthread_cond_t sched_cond[STARPU_NMAXWORKERS];
static pthread_mutex_t sched_mutex[STARPU_NMAXWORKERS];

static double alpha = _STARPU_DEFAULT_ALPHA;
static double beta = _STARPU_DEFAULT_BETA;
static double _gamma = _STARPU_DEFAULT_GAMMA;
static double idle_power = 0.0;

static double worker_exp_start[STARPU_NMAXWORKERS];
static double worker_exp_end[STARPU_NMAXWORKERS];
static double worker_exp_len[STARPU_NMAXWORKERS];
static int ntasks[STARPU_NMAXWORKERS];

static void parallel_heft_post_exec_hook(struct starpu_task *task)
{
	if (!task->cl || task->execute_on_a_specific_worker)
		return;

	int workerid = starpu_worker_get_id();
	double model = task->predicted;
	double transfer_model = task->predicted_transfer;

	if (isnan(model))
		model = 0.0;

	/* Once we have executed the task, we can update the predicted amount
	 * of work. */
	_STARPU_PTHREAD_MUTEX_LOCK(&sched_mutex[workerid]);
	worker_exp_len[workerid] -= model + transfer_model;
	worker_exp_start[workerid] = starpu_timing_now();
	worker_exp_end[workerid] = worker_exp_start[workerid] + worker_exp_len[workerid];
	ntasks[workerid]--;
	_STARPU_PTHREAD_MUTEX_UNLOCK(&sched_mutex[workerid]);
}

static int push_task_on_best_worker(struct starpu_task *task, int best_workerid, double exp_end_predicted, int prio)
{
	/* make sure someone coule execute that task ! */
	STARPU_ASSERT(best_workerid != -1);

	/* Is this a basic worker or a combined worker ? */
	int nbasic_workers = (int)starpu_worker_get_count();
	int is_basic_worker = (best_workerid < nbasic_workers);

	unsigned memory_node;
	memory_node = starpu_worker_get_memory_node(best_workerid);

	if (starpu_get_prefetch_flag())
		starpu_prefetch_task_input_on_node(task, memory_node);

	int ret = 0;

	if (is_basic_worker)
	{
		task->predicted = exp_end_predicted - worker_exp_end[best_workerid];
		/* TODO */
		task->predicted_transfer = 0;
		_STARPU_PTHREAD_MUTEX_LOCK(&sched_mutex[best_workerid]);
		worker_exp_len[best_workerid] += task->predicted;
		worker_exp_end[best_workerid] = exp_end_predicted;
		worker_exp_start[best_workerid] = exp_end_predicted - worker_exp_len[best_workerid];

		ntasks[best_workerid]++;
		_STARPU_PTHREAD_MUTEX_UNLOCK(&sched_mutex[best_workerid]);

		ret = starpu_push_local_task(best_workerid, task, prio);
	}
	else
	{
		/* This is a combined worker so we create task aliases */
		struct _starpu_combined_worker *combined_worker;
		combined_worker = _starpu_get_combined_worker_struct(best_workerid);
		int worker_size = combined_worker->worker_size;
		int *combined_workerid = combined_worker->combined_workerid;

		struct _starpu_job *j = _starpu_get_job_associated_to_task(task);
		j->task_size = worker_size;
		j->combined_workerid = best_workerid;
		j->active_task_alias_count = 0;
		task->predicted_transfer = 0;

		_STARPU_PTHREAD_BARRIER_INIT(&j->before_work_barrier, NULL, worker_size);
		_STARPU_PTHREAD_BARRIER_INIT(&j->after_work_barrier, NULL, worker_size);

		int i;
		for (i = 0; i < worker_size; i++)
		{
			struct starpu_task *alias = _starpu_create_task_alias(task);
			int local_worker = combined_workerid[i];

			alias->predicted = exp_end_predicted - worker_exp_end[local_worker];
			/* TODO */
			alias->predicted_transfer = 0;

			_STARPU_PTHREAD_MUTEX_LOCK(&sched_mutex[local_worker]);
			worker_exp_len[local_worker] += alias->predicted;
			worker_exp_end[local_worker] = exp_end_predicted;
			worker_exp_start[local_worker] = exp_end_predicted - worker_exp_len[local_worker];

			ntasks[local_worker]++;
			_STARPU_PTHREAD_MUTEX_UNLOCK(&sched_mutex[local_worker]);

			ret |= starpu_push_local_task(local_worker, alias, prio);
		}

	}

	return ret;
}

static double compute_expected_end(int workerid, double length)
{
	if (workerid < (int)nworkers)
	{
		/* This is a basic worker */
		return worker_exp_start[workerid] + worker_exp_len[workerid] + length;
	}
	else
	{
		/* This is a combined worker, the expected end is the end for the latest worker */
		int worker_size;
		int *combined_workerid;
		starpu_combined_worker_get_description(workerid, &worker_size, &combined_workerid);

		double exp_end = DBL_MIN;

		int i;
		for (i = 0; i < worker_size; i++)
		{
			double local_exp_start = worker_exp_start[combined_workerid[i]];
			double local_exp_len = worker_exp_len[combined_workerid[i]];
			double local_exp_end = local_exp_start + local_exp_len + length;
			exp_end = STARPU_MAX(exp_end, local_exp_end);
		}

		return exp_end;
	}
}

static double compute_ntasks_end(int workerid)
{
	enum starpu_perf_archtype perf_arch = starpu_worker_get_perf_archtype(workerid);
	if (workerid < (int)nworkers)
	{
		/* This is a basic worker */
		return ntasks[workerid] / starpu_worker_get_relative_speedup(perf_arch);
	}
	else
	{
		/* This is a combined worker, the expected end is the end for the latest worker */
		int worker_size;
		int *combined_workerid;
		starpu_combined_worker_get_description(workerid, &worker_size, &combined_workerid);

		int ntasks_end=0;

		int i;
		for (i = 0; i < worker_size; i++)
		{
			/* XXX: this is actually bogus: not all pushed tasks are necessarily parallel... */
			ntasks_end = STARPU_MAX(ntasks_end, (int) ((double) ntasks[combined_workerid[i]] / starpu_worker_get_relative_speedup(perf_arch)));
		}

		return ntasks_end;
	}
}

static int _parallel_heft_push_task(struct starpu_task *task, unsigned prio)
{
	unsigned worker;
	int best = -1;

	/* this flag is set if the corresponding worker is selected because
	   there is no performance prediction available yet */
	int forced_best = -1, forced_nimpl = -1;

	double local_task_length[nworkers+ncombinedworkers][STARPU_MAXIMPLEMENTATIONS];
	double local_data_penalty[nworkers+ncombinedworkers][STARPU_MAXIMPLEMENTATIONS];
	double local_power[nworkers+ncombinedworkers][STARPU_MAXIMPLEMENTATIONS];
	double local_exp_end[nworkers+ncombinedworkers][STARPU_MAXIMPLEMENTATIONS];
	double fitness[nworkers+ncombinedworkers][STARPU_MAXIMPLEMENTATIONS];

	double max_exp_end = 0.0;

	int skip_worker[nworkers+ncombinedworkers][STARPU_MAXIMPLEMENTATIONS];

	double best_exp_end = DBL_MAX;
	//double penality_best = 0.0;

	int ntasks_best = -1, nimpl_best = -1;
	double ntasks_best_end = 0.0;
	int calibrating = 0;

	/* A priori, we know all estimations */
	int unknown = 0;

	for (worker = 0; worker < nworkers; worker++)
	{
		/* Sometimes workers didn't take the tasks as early as we expected */
		_STARPU_PTHREAD_MUTEX_LOCK(&sched_mutex[worker]);
		worker_exp_start[worker] = STARPU_MAX(worker_exp_start[worker], starpu_timing_now());
		worker_exp_end[worker] = worker_exp_start[worker] + worker_exp_len[worker];
		if (worker_exp_end[worker] > max_exp_end)
			max_exp_end = worker_exp_end[worker];
		_STARPU_PTHREAD_MUTEX_UNLOCK(&sched_mutex[worker]);
	}

	unsigned nimpl;
	for (worker = 0; worker < (nworkers+ncombinedworkers); worker++)
	{
		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
		{
			if (!starpu_combined_worker_can_execute_task(worker, task, nimpl))
			{
				/* no one on that queue may execute this task */
				skip_worker[worker][nimpl] = 1;
				continue;
			}
			else
			{
				skip_worker[worker][nimpl] = 0;
			}

			enum starpu_perf_archtype perf_arch = starpu_worker_get_perf_archtype(worker);

			local_task_length[worker][nimpl] = starpu_task_expected_length(task, perf_arch,nimpl);

			unsigned memory_node = starpu_worker_get_memory_node(worker);
			local_data_penalty[worker][nimpl] = starpu_task_expected_data_transfer_time(memory_node, task);

			double ntasks_end = compute_ntasks_end(worker);

			if (ntasks_best == -1
			    || (!calibrating && ntasks_end < ntasks_best_end) /* Not calibrating, take better task */
			    || (!calibrating && isnan(local_task_length[worker][nimpl])) /* Not calibrating but this worker is being calibrated */
			    || (calibrating && isnan(local_task_length[worker][nimpl]) && ntasks_end < ntasks_best_end) /* Calibrating, compete this worker with other non-calibrated */
					)
			{
				ntasks_best_end = ntasks_end;
				ntasks_best = worker;
				nimpl_best = nimpl;
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

			local_exp_end[worker][nimpl] = compute_expected_end(worker, local_task_length[worker][nimpl]);

			//fprintf(stderr, "WORKER %d -> length %e end %e\n", worker, local_task_length[worker][nimpl], local_exp_end[worker][nimpl]);

			if (local_exp_end[worker][nimpl] < best_exp_end)
			{
				/* a better solution was found */
				best_exp_end = local_exp_end[worker][nimpl];
				nimpl_best = nimpl;
			}


			local_power[worker][nimpl] = starpu_task_expected_power(task, perf_arch,nimpl);
			//_STARPU_DEBUG("Scheduler parallel heft: task length (%lf) local power (%lf) worker (%u) kernel (%u) \n", local_task_length[worker][nimpl],local_power[worker][nimpl],worker,nimpl);

			if (isnan(local_power[worker][nimpl]))
				local_power[worker][nimpl] = 0.;

		} //end for
	}

	if (unknown) {
		forced_best = ntasks_best;
		forced_nimpl = nimpl_best;
	}

	double best_fitness = -1;


	if (forced_best == -1)
	{
		for (worker = 0; worker < nworkers+ncombinedworkers; worker++)
		{
			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
			{
				if (skip_worker[worker][nimpl])
				{
					/* no one on that queue may execute this task */
					continue;
				}

				fitness[worker][nimpl] = alpha*(local_exp_end[worker][nimpl] - best_exp_end)
						+ beta*(local_data_penalty[worker][nimpl])
						+ _gamma*(local_power[worker][nimpl]);

				if (local_exp_end[worker][nimpl] > max_exp_end)
					/* This placement will make the computation
					 * longer, take into account the idle
					 * consumption of other cpus */
					fitness[worker][nimpl] += _gamma * idle_power * (local_exp_end[worker][nimpl] - max_exp_end) / 1000000.0;

				if (best == -1 || fitness[worker][nimpl] < best_fitness)
				{
					/* we found a better solution */
					best_fitness = fitness[worker][nimpl];
					best = worker;
					nimpl_best = nimpl;
				}

			//	fprintf(stderr, "FITNESS worker %d -> %e local_exp_end %e - local_data_penalty %e\n", worker, fitness[worker][nimpl], local_exp_end[worker][nimpl] - best_exp_end, local_data_penalty[worker][nimpl]);
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
		nimpl_best = forced_nimpl;
		//penality_best = 0.0;
		best_exp_end = compute_expected_end(best, 0);
	}
	else
	{
		//penality_best = local_data_penalty[best][nimpl_best];
		best_exp_end = local_exp_end[best][nimpl_best];
	}


	//_STARPU_DEBUG("Scheduler parallel heft: kernel (%u)\n", nimpl_best);
	_starpu_get_job_associated_to_task(task)->nimpl = nimpl_best;
	/* we should now have the best worker in variable "best" */
	return push_task_on_best_worker(task, best, best_exp_end, prio);
}

static int parallel_heft_push_task(struct starpu_task *task)
{
	if (task->priority == STARPU_MAX_PRIO)
		return _parallel_heft_push_task(task, 1);

	return _parallel_heft_push_task(task, 0);
}

static void initialize_parallel_heft_policy(struct starpu_machine_topology *topology,
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

	_starpu_sched_find_worker_combinations(topology);

	ncombinedworkers = topology->ncombinedworkers;

	unsigned workerid;
	for (workerid = 0; workerid < nworkers; workerid++)
	{
		worker_exp_start[workerid] = starpu_timing_now();
		worker_exp_len[workerid] = 0.0;
		worker_exp_end[workerid] = worker_exp_start[workerid];
		ntasks[workerid] = 0;

		_STARPU_PTHREAD_MUTEX_INIT(&sched_mutex[workerid], NULL);
		_STARPU_PTHREAD_COND_INIT(&sched_cond[workerid], NULL);

		starpu_worker_set_sched_condition(workerid, &sched_cond[workerid], &sched_mutex[workerid]);
	}

	/* We pre-compute an array of all the perfmodel archs that are applicable */
	unsigned total_worker_count = nworkers + ncombinedworkers;

	unsigned used_perf_archtypes[STARPU_NARCH_VARIATIONS];
	memset(used_perf_archtypes, 0, sizeof(used_perf_archtypes));

	for (workerid = 0; workerid < total_worker_count; workerid++)
	{
		enum starpu_perf_archtype perf_archtype = starpu_worker_get_perf_archtype(workerid);
		used_perf_archtypes[perf_archtype] = 1;
	}

//	napplicable_perf_archtypes = 0;

//	int arch;
//	for (arch = 0; arch < STARPU_NARCH_VARIATIONS; arch++)
//	{
//		if (used_perf_archtypes[arch])
//			applicable_perf_archtypes[napplicable_perf_archtypes++] = arch;
//	}
}

/* TODO: use post_exec_hook to fix the expected start */
struct starpu_sched_policy _starpu_sched_parallel_heft_policy =
{
	.init_sched = initialize_parallel_heft_policy,
	.deinit_sched = NULL,
	.push_task = parallel_heft_push_task,
	.pop_task = NULL,
	.pre_exec_hook = NULL,
	.post_exec_hook = parallel_heft_post_exec_hook,
	.pop_every_task = NULL,
	.policy_name = "pheft",
	.policy_description = "parallel HEFT"
};
