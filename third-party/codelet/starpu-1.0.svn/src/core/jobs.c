/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

#include <starpu.h>
#include <core/jobs.h>
#include <core/task.h>
#include <core/workers.h>
#include <core/dependencies/data_concurrency.h>
#include <common/config.h>
#include <common/utils.h>
#include <profiling/profiling.h>
#include <profiling/bound.h>
#include <starpu_top.h>
#include <top/starpu_top_core.h>

size_t _starpu_job_get_data_size(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, unsigned nimpl, struct _starpu_job *j)
{
	struct starpu_task *task = j->task;

	if (model && model->per_arch[arch][nimpl].size_base) {
		return model->per_arch[arch][nimpl].size_base(task, arch, nimpl);
	} else if (model && model->size_base) {
		return model->size_base(task, nimpl);
	} else {
		unsigned nbuffers = task->cl->nbuffers;
		size_t size = 0;

		unsigned buffer;
		for (buffer = 0; buffer < nbuffers; buffer++)
		{
			starpu_data_handle_t handle = task->handles[buffer];
			size += _starpu_data_get_size(handle);
		}
		return size;
	}
}

/* we need to identify each task to generate the DAG. */
static unsigned job_cnt = 0;

void _starpu_exclude_task_from_dag(struct starpu_task *task)
{
	struct _starpu_job *j = _starpu_get_job_associated_to_task(task);

	j->exclude_from_dag = 1;
}

/* create an internal struct _starpu_job structure to encapsulate the task */
struct _starpu_job* __attribute__((malloc)) _starpu_job_create(struct starpu_task *task)
{
	struct _starpu_job *job;
        _STARPU_LOG_IN();

	job = _starpu_job_new();

	job->nimpl =0; /* best implementation */
	job->task = task;

	job->footprint_is_computed = 0;
	job->submitted = 0;
	job->terminated = 0;

#ifndef STARPU_USE_FXT
	if (_starpu_bound_recording || _starpu_top_status_get())
#endif
		job->job_id = STARPU_ATOMIC_ADD(&job_cnt, 1);
#ifdef STARPU_USE_FXT
	/* display all tasks by default */
        job->model_name = NULL;
#endif
	job->exclude_from_dag = 0;

	job->reduction_task = 0;

	_starpu_cg_list_init(&job->job_successors);

	job->implicit_dep_handle = NULL;

	_STARPU_PTHREAD_MUTEX_INIT(&job->sync_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&job->sync_cond, NULL);

	job->bound_task = NULL;

	/* By default we have sequential tasks */
	job->task_size = 1;

	if (task->use_tag)
		_starpu_tag_declare(task->tag_id, job);

        _STARPU_LOG_OUT();
	return job;
}

void _starpu_job_destroy(struct _starpu_job *j)
{
	/* Wait for any code that was still working on the job (and was
	 * probably our waker) */
	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);
	_STARPU_PTHREAD_COND_DESTROY(&j->sync_cond);
	_STARPU_PTHREAD_MUTEX_DESTROY(&j->sync_mutex);

	if (j->task_size > 1)
	{
		_STARPU_PTHREAD_BARRIER_DESTROY(&j->before_work_barrier);
		_STARPU_PTHREAD_BARRIER_DESTROY(&j->after_work_barrier);
	}

	_starpu_cg_list_deinit(&j->job_successors);

	_starpu_job_delete(j);
}

void _starpu_wait_job(struct _starpu_job *j)
{
	STARPU_ASSERT(j->task);
	STARPU_ASSERT(!j->task->detach);
        _STARPU_LOG_IN();

	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);

	/* We wait for the flag to have a value of 2 which means that both the
	 * codelet's implementation and its callback have been executed. That
	 * way, _starpu_wait_job won't return until the entire task was really
	 * executed (so that we cannot destroy the task while it is still being
	 * manipulated by the driver). */
	while (j->terminated != 2)
		_STARPU_PTHREAD_COND_WAIT(&j->sync_cond, &j->sync_mutex);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);
        _STARPU_LOG_OUT();
}

void _starpu_handle_job_termination(struct _starpu_job *j)
{
	struct starpu_task *task = j->task;

	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);

	task->status = STARPU_TASK_FINISHED;

	/* We must have set the j->terminated flag early, so that it is
	 * possible to express task dependencies within the callback
	 * function. A value of 1 means that the codelet was executed but that
	 * the callback is not done yet. */
	j->terminated = 1;

	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

	/* Task does not have a cl, but has explicit data dependencies, we need
	 * to tell them that we will not exist any more before notifying the
	 * tasks waiting for us */
	if (j->implicit_dep_handle)
		_starpu_release_data_enforce_sequential_consistency(j->task, j->implicit_dep_handle);

	/* in case there are dependencies, wake up the proper tasks */
	_starpu_notify_dependencies(j);

	/* the callback is executed after the dependencies so that we may remove the tag
 	 * of the task itself */
	if (task->callback_func)
	{
		int profiling = starpu_profiling_status_get();
		if (profiling && task->profiling_info)
			_starpu_clock_gettime(&task->profiling_info->callback_start_time);

		/* so that we can check whether we are doing blocking calls
		 * within the callback */
		_starpu_set_local_worker_status(STATUS_CALLBACK);


		/* Perhaps we have nested callbacks (eg. with chains of empty
		 * tasks). So we store the current task and we will restore it
		 * later. */
		struct starpu_task *current_task = starpu_task_get_current();

		_starpu_set_current_task(task);

		_STARPU_TRACE_START_CALLBACK(j);
		task->callback_func(task->callback_arg);
		_STARPU_TRACE_END_CALLBACK(j);

		_starpu_set_current_task(current_task);

		_starpu_set_local_worker_status(STATUS_UNKNOWN);

		if (profiling && task->profiling_info)
			_starpu_clock_gettime(&task->profiling_info->callback_end_time);
	}

	/* If the job was executed on a combined worker there is no need for the
	 * scheduler to process it : the task structure doesn't contain any valuable
	 * data as it's not linked to an actual worker */
	if (j->task_size == 1)
		_starpu_sched_post_exec_hook(task);

	_STARPU_TRACE_TASK_DONE(j);

	/* NB: we do not save those values before the callback, in case the
	 * application changes some parameters eventually (eg. a task may not
	 * be generated if the application is terminated). */
	int destroy = task->destroy;
	int detach = task->detach;
	int regenerate = task->regenerate;

	/* we do not desallocate the job structure if some is going to
	 * wait after the task */
	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);
	/* A value of 2 is put to specify that not only the codelet but
	 * also the callback were executed. */
	j->terminated = 2;
	_STARPU_PTHREAD_COND_BROADCAST(&j->sync_cond);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

	if (detach)
	{
		/* no one is going to synchronize with that task so we release
		 * the data structures now. In case the job was already locked
		 * by the caller, it is its responsability to destroy the task.
		 * */
		if (destroy)
			_starpu_task_destroy(task);
	}

	if (regenerate)
	{
		STARPU_ASSERT_MSG(detach && !destroy && !task->synchronous, "Regenerated task must be detached, and not have detroy=1 or synchronous=1");

		/* We reuse the same job structure */
		int ret = _starpu_submit_job(j);
		STARPU_ASSERT(!ret);
	}
	_starpu_decrement_nsubmitted_tasks();
	_starpu_decrement_nready_tasks();
}

/* This function is called when a new task is submitted to StarPU
 * it returns 1 if the tag deps are not fulfilled, 0 otherwise */
static unsigned _starpu_not_all_tag_deps_are_fulfilled(struct _starpu_job *j)
{
	unsigned ret;

	if (!j->task->use_tag)
	{
		/* this task does not use tags, so we can go on */
		return 0;
	}

	struct _starpu_tag *tag = j->tag;

	struct _starpu_cg_list *tag_successors = &tag->tag_successors;

	_starpu_spin_lock(&tag->lock);

	if (tag_successors->ndeps != tag_successors->ndeps_completed)
	{
		tag->state = STARPU_BLOCKED;
                j->task->status = STARPU_TASK_BLOCKED_ON_TAG;
		ret = 1;
	}
	else
	{
		/* existing deps (if any) are fulfilled */
		/* If the same tag is being signaled by several tasks, do not
		 * clear a DONE state. If it's the same job submitted several
		 * times with the same tag, we have to do it */
		if (j->submitted == 2 || tag->state != STARPU_DONE)
			tag->state = STARPU_READY;
		/* already prepare for next run */
		tag_successors->ndeps_completed = 0;
		ret = 0;
	}

	_starpu_spin_unlock(&tag->lock);
	return ret;
}

static unsigned _starpu_not_all_task_deps_are_fulfilled(struct _starpu_job *j)
{
	unsigned ret;

	struct _starpu_cg_list *job_successors = &j->job_successors;

	if (!j->submitted || (job_successors->ndeps != job_successors->ndeps_completed))
	{
                j->task->status = STARPU_TASK_BLOCKED_ON_TASK;
		ret = 1;
	}
	else
	{
		/* existing deps (if any) are fulfilled */
		/* already prepare for next run */
		job_successors->ndeps_completed = 0;
		ret = 0;
	}

	return ret;
}

/*
 *	In order, we enforce tag, task and data dependencies. The task is
 *	passed to the scheduler only once all these constraints are fulfilled.
 *
 *	The job mutex has to be taken for atomicity with task submission, and
 *	is released here.
 */
unsigned _starpu_enforce_deps_and_schedule(struct _starpu_job *j)
{
	unsigned ret;
        _STARPU_LOG_IN();

	/* enfore tag dependencies */
	if (_starpu_not_all_tag_deps_are_fulfilled(j))
	{
		_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);
                _STARPU_LOG_OUT_TAG("not_all_tag_deps_are_fulfilled");
		return 0;
        }

	/* enfore task dependencies */
	if (_starpu_not_all_task_deps_are_fulfilled(j))
	{
		_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);
                _STARPU_LOG_OUT_TAG("not_all_task_deps_are_fulfilled");
		return 0;
        }
	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

	/* enforce data dependencies */
	if (_starpu_submit_job_enforce_data_deps(j))
	{
                _STARPU_LOG_OUT_TAG("enforce_data_deps");
		return 0;
        }

	ret = _starpu_push_task(j);

        _STARPU_LOG_OUT();
	return ret;
}

/* Tag deps are already fulfilled */
unsigned _starpu_enforce_deps_starting_from_task(struct _starpu_job *j)
{
	unsigned ret;

	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);
	/* enfore task dependencies */
	if (_starpu_not_all_task_deps_are_fulfilled(j))
	{
		_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);
		return 0;
	}
	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

	/* enforce data dependencies */
	if (_starpu_submit_job_enforce_data_deps(j))
		return 0;

	ret = _starpu_push_task(j);

	return ret;
}

/* This function must be called with worker->sched_mutex taken */
struct starpu_task *_starpu_pop_local_task(struct _starpu_worker *worker)
{
	struct starpu_task *task = NULL;

	if (!starpu_task_list_empty(&worker->local_tasks))
		task = starpu_task_list_pop_back(&worker->local_tasks);

	return task;
}

int _starpu_push_local_task(struct _starpu_worker *worker, struct starpu_task *task, int back)
{
	/* Check that the worker is able to execute the task ! */
	STARPU_ASSERT(task && task->cl);
	if (STARPU_UNLIKELY(!(worker->worker_mask & task->cl->where)))
		return -ENODEV;

	_STARPU_PTHREAD_MUTEX_LOCK(worker->sched_mutex);

	if (back)
		starpu_task_list_push_back(&worker->local_tasks, task);
	else
		starpu_task_list_push_front(&worker->local_tasks, task);

	_STARPU_PTHREAD_COND_BROADCAST(worker->sched_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(worker->sched_mutex);

	return 0;
}

const char *_starpu_get_model_name(struct _starpu_job *j)
{
	if (!j)
		return NULL;

	struct starpu_task *task = j->task;
        if (task && task->cl) {
            if (task->cl->model && task->cl->model->symbol)
                return task->cl->model->symbol;
	    else
		return task->cl->name;
	} else
	{
#ifdef STARPU_USE_FXT
                return j->model_name;
#else
                return NULL;
#endif
        }
}
