/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#include <math.h>
#include <starpu.h>
#include <starpu_profiling.h>
#include <profiling/profiling.h>
#include <common/utils.h>
#include <core/debug.h>
#include <drivers/driver_common/driver_common.h>
#include <starpu_top.h>
#include <core/sched_policy.h>
#include <top/starpu_top_core.h>

void _starpu_driver_start_job(struct _starpu_worker *args, struct _starpu_job *j, struct timespec *codelet_start, int rank, int profiling)
{
	struct starpu_task *task = j->task;
	struct starpu_codelet *cl = task->cl;
	struct starpu_task_profiling_info *profiling_info;
	int starpu_top=_starpu_top_status_get();
	int workerid = args->workerid;
	unsigned calibrate_model = 0;

	if (cl->model && cl->model->benchmarking)
		calibrate_model = 1;

	/* If the job is executed on a combined worker there is no need for the
	 * scheduler to process it : it doesn't contain any valuable data
	 * as it's not linked to an actual worker */
	if (j->task_size == 1)
		_starpu_sched_pre_exec_hook(task);

	args->status = STATUS_EXECUTING;
	task->status = STARPU_TASK_RUNNING;

	if (rank == 0)
	{
		cl->per_worker_stats[workerid]++;

		profiling_info = task->profiling_info;

		if ((profiling && profiling_info) || calibrate_model || starpu_top)
		{
			_starpu_clock_gettime(codelet_start);
			_starpu_worker_register_executing_start_date(workerid, codelet_start);
		}
	}

	if (starpu_top)
		_starpu_top_task_started(task,workerid,codelet_start);

	_STARPU_TRACE_START_CODELET_BODY(j);
}

void _starpu_driver_end_job(struct _starpu_worker *args, struct _starpu_job *j, enum starpu_perf_archtype perf_arch STARPU_ATTRIBUTE_UNUSED, struct timespec *codelet_end, int rank, int profiling)
{
	struct starpu_task *task = j->task;
	struct starpu_codelet *cl = task->cl;
	struct starpu_task_profiling_info *profiling_info = task->profiling_info;
	int starpu_top=_starpu_top_status_get();
	int workerid = args->workerid;
	unsigned calibrate_model = 0;

	_STARPU_TRACE_END_CODELET_BODY(j, j->nimpl, perf_arch);

	if (cl->model && cl->model->benchmarking)
		calibrate_model = 1;

	if (rank == 0)
	{
		if ((profiling && profiling_info) || calibrate_model || starpu_top)
			_starpu_clock_gettime(codelet_end);
	}

	if (starpu_top)
	  _starpu_top_task_ended(task,workerid,codelet_end);

	args->status = STATUS_UNKNOWN;
}
void _starpu_driver_update_job_feedback(struct _starpu_job *j, struct _starpu_worker *worker_args,
					enum starpu_perf_archtype perf_arch,
					struct timespec *codelet_start, struct timespec *codelet_end, int profiling)
{
	struct starpu_task_profiling_info *profiling_info = j->task->profiling_info;
	struct timespec measured_ts;
	double measured;
	int workerid = worker_args->workerid;
	struct starpu_codelet *cl = j->task->cl;
	int calibrate_model = 0;
	int updated = 0;

	if (cl->model && cl->model->benchmarking)
		calibrate_model = 1;

	if ((profiling && profiling_info) || calibrate_model)
	{
		starpu_timespec_sub(codelet_end, codelet_start, &measured_ts);
		measured = starpu_timing_timespec_to_us(&measured_ts);

		if (profiling && profiling_info)
		{
			memcpy(&profiling_info->start_time, codelet_start, sizeof(struct timespec));
			memcpy(&profiling_info->end_time, codelet_end, sizeof(struct timespec));

			profiling_info->workerid = workerid;

			_starpu_worker_update_profiling_info_executing(workerid, &measured_ts, 1,
				profiling_info->used_cycles,
				profiling_info->stall_cycles,
				profiling_info->power_consumed);
			updated =  1;
		}

		if (calibrate_model)

			_starpu_update_perfmodel_history(j, j->task->cl->model,  perf_arch, worker_args->devid, measured,j->nimpl);


	}

	if (!updated)
		_starpu_worker_update_profiling_info_executing(workerid, NULL, 1, 0, 0, 0);

	if (profiling_info && profiling_info->power_consumed && cl->power_model && cl->power_model->benchmarking)
	{
		_starpu_update_perfmodel_history(j, j->task->cl->power_model,  perf_arch, worker_args->devid, profiling_info->power_consumed,j->nimpl);
	}
}

/* Workers may block when there is no work to do at all. We assume that the
 * mutex is hold when that function is called. */
void _starpu_block_worker(int workerid, pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	struct timespec start_time, end_time;

	_STARPU_TRACE_WORKER_SLEEP_START
	_starpu_worker_set_status(workerid, STATUS_SLEEPING);

	_starpu_clock_gettime(&start_time);
	_starpu_worker_register_sleeping_start_date(workerid, &start_time);

	_STARPU_PTHREAD_COND_WAIT(cond, mutex);

	_starpu_worker_set_status(workerid, STATUS_UNKNOWN);
	_STARPU_TRACE_WORKER_SLEEP_END
	_starpu_clock_gettime(&end_time);

	int profiling = starpu_profiling_status_get();
	if (profiling)
	{
		struct timespec sleeping_time;
		starpu_timespec_sub(&end_time, &start_time, &sleeping_time);
		_starpu_worker_update_profiling_info_sleeping(workerid, &start_time, &end_time);
	}
}
