/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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

#include <math.h>
#include <starpu.h>
#include <starpu_profiling.h>
#include <drivers/driver_common/driver_common.h>
#include <common/utils.h>
#include <core/debug.h>
#include "driver_cpu.h"
#include <core/sched_policy.h>

static int execute_job_on_cpu(struct _starpu_job *j, struct _starpu_worker *cpu_args, int is_parallel_task, int rank, enum starpu_perf_archtype perf_arch)
{
	int ret;
	int profiling = starpu_profiling_status_get();
	struct timespec codelet_start, codelet_end;

	struct starpu_task *task = j->task;
	struct starpu_codelet *cl = task->cl;

	STARPU_ASSERT(cl);

	if (rank == 0)
	{
		ret = _starpu_fetch_task_input(j, 0);
		if (ret != 0)
		{
			/* there was not enough memory so the codelet cannot be executed right now ... */
			/* push the codelet back and try another one ... */
			return -EAGAIN;
		}
	}

	if (is_parallel_task)
		_STARPU_PTHREAD_BARRIER_WAIT(&j->before_work_barrier);

	/* Give profiling variable */
	_starpu_driver_start_job(cpu_args, j, &codelet_start, rank, profiling);

	/* In case this is a Fork-join parallel task, the worker does not
	 * execute the kernel at all. */
	if ((rank == 0) || (cl->type != STARPU_FORKJOIN))
	{
		_starpu_cl_func_t func = _starpu_task_get_cpu_nth_implementation(cl, j->nimpl);
		if (is_parallel_task && cl->type == STARPU_FORKJOIN)
			/* bind to parallel worker */
			_starpu_bind_thread_on_cpus(cpu_args->config, _starpu_get_combined_worker_struct(j->combined_workerid));
		STARPU_ASSERT(func);
		func(task->interfaces, task->cl_arg);
		if (is_parallel_task && cl->type == STARPU_FORKJOIN)
			/* rebind to single CPU */
			_starpu_bind_thread_on_cpu(cpu_args->config, cpu_args->bindid);
	}

	_starpu_driver_end_job(cpu_args, j, perf_arch, &codelet_end, rank, profiling);

	if (is_parallel_task)
		_STARPU_PTHREAD_BARRIER_WAIT(&j->after_work_barrier);

	if (rank == 0)
	{
		_starpu_driver_update_job_feedback(j, cpu_args,
				perf_arch, &codelet_start, &codelet_end, profiling);
		_starpu_push_task_output(j, 0);
	}

	return 0;
}

void *_starpu_cpu_worker(void *arg)
{
	struct _starpu_worker *cpu_arg = (struct _starpu_worker *) arg;
	unsigned memnode = cpu_arg->memory_node;
	int workerid = cpu_arg->workerid;
	int devid = cpu_arg->devid;

#ifdef STARPU_USE_FXT
	_starpu_fxt_register_thread(cpu_arg->bindid);
#endif
	_STARPU_TRACE_WORKER_INIT_START(_STARPU_FUT_CPU_KEY, devid, memnode);

	_starpu_bind_thread_on_cpu(cpu_arg->config, cpu_arg->bindid);

        _STARPU_DEBUG("cpu worker %d is ready on logical cpu %d\n", devid, cpu_arg->bindid);

	_starpu_set_local_memory_node_key(&memnode);

	_starpu_set_local_worker_key(cpu_arg);

	snprintf(cpu_arg->name, sizeof(cpu_arg->name), "CPU %d", devid);
	snprintf(cpu_arg->short_name, sizeof(cpu_arg->short_name), "CPU %d", devid);

	cpu_arg->status = STATUS_UNKNOWN;

	_STARPU_TRACE_WORKER_INIT_END

        /* tell the main thread that we are ready */
	_STARPU_PTHREAD_MUTEX_LOCK(&cpu_arg->mutex);
	cpu_arg->worker_is_initialized = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&cpu_arg->ready_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&cpu_arg->mutex);

        struct _starpu_job *j;
	struct starpu_task *task;

	int res;

	while (_starpu_machine_is_running())
	{
		_STARPU_TRACE_START_PROGRESS(memnode);
		_starpu_datawizard_progress(memnode, 1);
		_STARPU_TRACE_END_PROGRESS(memnode);

		_STARPU_PTHREAD_MUTEX_LOCK(cpu_arg->sched_mutex);

		task = _starpu_pop_task(cpu_arg);

                if (!task)
		{
			if (_starpu_worker_can_block(memnode))
				_starpu_block_worker(workerid, cpu_arg->sched_cond, cpu_arg->sched_mutex);

			_STARPU_PTHREAD_MUTEX_UNLOCK(cpu_arg->sched_mutex);

			continue;
		};

		_STARPU_PTHREAD_MUTEX_UNLOCK(cpu_arg->sched_mutex);

		STARPU_ASSERT(task);
		j = _starpu_get_job_associated_to_task(task);

		/* can a cpu perform that task ? */
		if (!_STARPU_CPU_MAY_PERFORM(j))
		{
			/* put it and the end of the queue ... XXX */
			_starpu_push_task(j);
			continue;
		}

		int rank = 0;
		int is_parallel_task = (j->task_size > 1);

		enum starpu_perf_archtype perf_arch;

		/* Get the rank in case it is a parallel task */
		if (is_parallel_task)
		{
			/* We can release the fake task */
			STARPU_ASSERT(task != j->task);
			free(task);

			_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);
			rank = j->active_task_alias_count++;
			_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

			struct _starpu_combined_worker *combined_worker;
			combined_worker = _starpu_get_combined_worker_struct(j->combined_workerid);

			cpu_arg->combined_workerid = j->combined_workerid;
			cpu_arg->worker_size = combined_worker->worker_size;
			cpu_arg->current_rank = rank;
			perf_arch = combined_worker->perf_arch;
		}
		else
		{
			cpu_arg->combined_workerid = cpu_arg->workerid;
			cpu_arg->worker_size = 1;
			cpu_arg->current_rank = 0;
			perf_arch = cpu_arg->perf_arch;
		}

		_starpu_set_current_task(j->task);
		cpu_arg->current_task = j->task;

                res = execute_job_on_cpu(j, cpu_arg, is_parallel_task, rank, perf_arch);

		_starpu_set_current_task(NULL);
		cpu_arg->current_task = NULL;

		if (res)
		{
			switch (res)
			{
				case -EAGAIN:
					_starpu_push_task(j);
					continue;
				default:
					STARPU_ASSERT(0);
			}
		}

		if (rank == 0)
			_starpu_handle_job_termination(j);
        }

	_STARPU_TRACE_WORKER_DEINIT_START

	_starpu_handle_all_pending_node_data_requests(memnode);

	/* In case there remains some memory that was automatically
	 * allocated by StarPU, we release it now. Note that data
	 * coherency is not maintained anymore at that point ! */
	_starpu_free_all_automatically_allocated_buffers(memnode);

	_STARPU_TRACE_WORKER_DEINIT_END(_STARPU_FUT_CPU_KEY);

	pthread_exit(NULL);
	return NULL;
}
