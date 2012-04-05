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
#include <starpu_profiling.h>
#include <core/workers.h>
#include <core/jobs.h>
#include <core/task.h>
#include <core/task_bundle.h>
#include <common/config.h>
#include <common/utils.h>
#include <profiling/profiling.h>
#include <profiling/bound.h>
#include <math.h>
#include <string.h>

/* XXX this should be reinitialized when StarPU is shutdown (or we should make
 * sure that no task remains !) */
/* TODO we could make this hierarchical to avoid contention ? */
static pthread_cond_t submitted_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t submitted_mutex = PTHREAD_MUTEX_INITIALIZER;
static long int nsubmitted = 0, nready = 0;

static void _starpu_increment_nsubmitted_tasks(void);

/* This key stores the task currently handled by the thread, note that we
 * cannot use the worker structure to store that information because it is
 * possible that we have a task with a NULL codelet, which means its callback
 * could be executed by a user thread as well. */
static pthread_key_t current_task_key;

void starpu_task_init(struct starpu_task *task)
{
	STARPU_ASSERT(task);

	task->cl = NULL;
	task->cl_arg = NULL;
	task->cl_arg_size = 0;

	task->callback_func = NULL;
	task->callback_arg = NULL;

	task->priority = STARPU_DEFAULT_PRIO;
	task->use_tag = 0;
	task->synchronous = 0;

	task->execute_on_a_specific_worker = 0;

	task->bundle = NULL;

	task->detach = 1;

	/* by default, we do not let StarPU free the task structure since
	 * starpu_task_init is likely to be used only for statically allocated
	 * tasks */
	task->destroy = 0;

	task->regenerate = 0;

	task->status = STARPU_TASK_INVALID;

	task->profiling_info = NULL;

	task->predicted = NAN;
	task->predicted_transfer = NAN;

	task->starpu_private = NULL;
	task->magic = 42;
}

/* Free all the ressources allocated for a task, without deallocating the task
 * structure itself (this is required for statically allocated tasks). */
void starpu_task_deinit(struct starpu_task *task)
{
	STARPU_ASSERT(task);

	/* If a buffer was allocated to store the profiling info, we free it. */
	if (task->profiling_info)
	{
		free(task->profiling_info);
		task->profiling_info = NULL;
	}

	/* If case the task is (still) part of a bundle */
	starpu_task_bundle_t bundle = task->bundle;
	if (bundle)
		starpu_task_bundle_remove(bundle, task);

	struct _starpu_job *j = (struct _starpu_job *)task->starpu_private;

	if (j)
		_starpu_job_destroy(j);
}

struct starpu_task * __attribute__((malloc)) starpu_task_create(void)
{
	struct starpu_task *task;

	task = (struct starpu_task *) calloc(1, sizeof(struct starpu_task));
	STARPU_ASSERT(task);

	starpu_task_init(task);

	/* Dynamically allocated tasks are destroyed by default */
	task->destroy = 1;

	return task;
}

/* Free the ressource allocated during starpu_task_create. This function can be
 * called automatically after the execution of a task by setting the "destroy"
 * flag of the starpu_task structure (default behaviour). Calling this function
 * on a statically allocated task results in an undefined behaviour. */
void _starpu_task_destroy(struct starpu_task *task)
{

   /* If starpu_task_destroy is called in a callback, we just set the destroy
      flag. The task will be destroyed after the callback returns */
   if (task == starpu_task_get_current()
       && _starpu_get_local_worker_status() == STATUS_CALLBACK)
   {

	   task->destroy = 1;

   }
   else
   {
	   starpu_task_deinit(task);
	   /* TODO handle the case of task with detach = 1 and destroy = 1 */
	   /* TODO handle the case of non terminated tasks -> return -EINVAL */
	   free(task);
   }
}

void starpu_task_destroy(struct starpu_task *task)
{
	STARPU_ASSERT(task);
	STARPU_ASSERT_MSG(!task->destroy || !task->detach, "starpu_task_destroy must not be called for task with destroy = 1 and detach = 1");
	_starpu_task_destroy(task);
}

int starpu_task_wait(struct starpu_task *task)
{
        _STARPU_LOG_IN();
	STARPU_ASSERT(task);

	STARPU_ASSERT_MSG(!task->detach, "starpu_task_wait can only be called on tasks with detach = 0");

	if (task->detach || task->synchronous)
	{
		_STARPU_DEBUG("Task is detached or asynchronous. Waiting returns immediately\n");
		_STARPU_LOG_OUT_TAG("einval");
		return -EINVAL;
	}

	if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
	{
		_STARPU_LOG_OUT_TAG("edeadlk");
		return -EDEADLK;
	}

	struct _starpu_job *j = (struct _starpu_job *)task->starpu_private;

	_starpu_wait_job(j);

	/* as this is a synchronous task, the liberation of the job
	   structure was deferred */
	if (task->destroy)
		_starpu_task_destroy(task);

        _STARPU_LOG_OUT();
	return 0;
}

struct _starpu_job *_starpu_get_job_associated_to_task(struct starpu_task *task)
{
	STARPU_ASSERT(task);

	if (!task->starpu_private)
	{
		struct _starpu_job *j = _starpu_job_create(task);
		task->starpu_private = j;
	}

	return (struct _starpu_job *)task->starpu_private;
}

/* NB in case we have a regenerable task, it is possible that the job was
 * already counted. */
int _starpu_submit_job(struct _starpu_job *j)
{
        _STARPU_LOG_IN();
	/* notify bound computation of a new task */
	_starpu_bound_record(j);

	_starpu_increment_nsubmitted_tasks();

	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);

	/* Need to atomically set submitted to 1 and check dependencies, since
	 * this is concucrent with _starpu_notify_cg */
	j->terminated = 0;
	if (!j->submitted)
		j->submitted = 1;
	else
		j->submitted = 2;

	int ret = _starpu_enforce_deps_and_schedule(j);

        _STARPU_LOG_OUT();
        return ret;
}

void _starpu_codelet_check_deprecated_fields(struct starpu_codelet *cl)
{
	if (!cl)
		return;

	int is_where_unset = cl->where == 0;

	/* Check deprecated and unset fields (where, <device>_func,
 	 * <device>_funcs) */

	/* CPU */
	if (cl->cpu_func && cl->cpu_func != STARPU_MULTIPLE_CPU_IMPLEMENTATIONS && cl->cpu_funcs[0])
	{
		fprintf(stderr, "[warning] [struct starpu_codelet] both cpu_func and cpu_funcs are set. Ignoring cpu_func.\n");
		cl->cpu_func = STARPU_MULTIPLE_CPU_IMPLEMENTATIONS;
	}
	if (cl->cpu_func && cl->cpu_func != STARPU_MULTIPLE_CPU_IMPLEMENTATIONS)
	{
		cl->cpu_funcs[0] = cl->cpu_func;
		cl->cpu_func = STARPU_MULTIPLE_CPU_IMPLEMENTATIONS;
	}
	if (cl->cpu_funcs[0] && cl->cpu_func == 0)
	{
		cl->cpu_func = STARPU_MULTIPLE_CPU_IMPLEMENTATIONS;
	}
	if (cl->cpu_funcs[0] && is_where_unset)
	{
		cl->where |= STARPU_CPU;
	}

	/* CUDA */
	if (cl->cuda_func && cl->cuda_func != STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS && cl->cuda_funcs[0])
	{
		fprintf(stderr, "[warning] [struct starpu_codelet] both cuda_func and cuda_funcs are set. Ignoring cuda_func.\n");
		cl->cuda_func = STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS;
	}
	if (cl->cuda_func && cl->cuda_func != STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS)
	{
		cl->cuda_funcs[0] = cl->cuda_func;
		cl->cuda_func = STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS;
	}
	if (cl->cuda_funcs[0] && cl->cuda_func == 0)
	{
		cl->cuda_func = STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS;
	}
	if (cl->cuda_funcs[0] && is_where_unset)
	{
		cl->where |= STARPU_CUDA;
	}

	/* OpenCL */
	if (cl->opencl_func && cl->opencl_func != STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS && cl->opencl_funcs[0])
	{
		fprintf(stderr, "[warning] [struct starpu_codelet] both opencl_func and opencl_funcs are set. Ignoring opencl_func.\n");
		cl->opencl_func = STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS;
	}
	if (cl->opencl_func && cl->opencl_func != STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS)
	{
		cl->opencl_funcs[0] = cl->opencl_func;
		cl->opencl_func = STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS;
	}
	if (cl->opencl_funcs[0] && cl->opencl_func == 0)
	{
		cl->opencl_func = STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS;
	}
	if (cl->opencl_funcs[0] && is_where_unset)
	{
		cl->where |= STARPU_OPENCL;
	}

	/* Gordon */
	if (cl->gordon_func && cl->gordon_func != STARPU_MULTIPLE_GORDON_IMPLEMENTATIONS)
	{
		cl->gordon_funcs[0] = cl->gordon_func;
		cl->gordon_func = STARPU_MULTIPLE_GORDON_IMPLEMENTATIONS;
	}
	if (cl->gordon_funcs[0] && cl->gordon_func == 0)
	{
		cl->gordon_func = STARPU_MULTIPLE_GORDON_IMPLEMENTATIONS;
	}
	if (cl->gordon_funcs[0] && is_where_unset)
	{
		cl->where = STARPU_GORDON;
	}
}

void _starpu_task_check_deprecated_fields(struct starpu_task *task)
{
	if (task->cl)
	{
		unsigned i;
		for(i=0; i<task->cl->nbuffers ; i++)
		{
			if (task->buffers[i].handle && task->handles[i])
			{
				fprintf(stderr, "[warning][struct starpu_task] task->buffers[%u] and task->handles[%u] both set. Ignoring task->buffers[%u] ?\n", i, i, i);
				STARPU_ASSERT(task->buffers[i].mode == task->cl->modes[i]);
				STARPU_ABORT();
			}
			if (task->buffers[i].handle)
			{
				task->handles[i] = task->buffers[i].handle;
				task->cl->modes[i] = task->buffers[i].mode;
			}
		}
	}
}

/* application should submit new tasks to StarPU through this function */
int starpu_task_submit(struct starpu_task *task)
{
	STARPU_ASSERT(task);
	STARPU_ASSERT(task->magic == 42);

	int ret;
	unsigned is_sync = task->synchronous;
        _STARPU_LOG_IN();

	if (is_sync)
	{
		/* Perhaps it is not possible to submit a synchronous
		 * (blocking) task */
                if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
		{
                        _STARPU_LOG_OUT_TAG("EDEADLK");
			return -EDEADLK;
                }

		task->detach = 0;
	}

	_starpu_task_check_deprecated_fields(task);
	_starpu_codelet_check_deprecated_fields(task->cl);

	if (task->cl)
	{
		unsigned i;

		/* Check the type of worker(s) required by the task exist */
		if (!_starpu_worker_exists(task))
		{
                        _STARPU_LOG_OUT_TAG("ENODEV");
			return -ENODEV;
                }

		/* Check buffers */
		STARPU_ASSERT(task->cl->nbuffers <= STARPU_NMAXBUFS);
		for (i = 0; i < task->cl->nbuffers; i++)
		{
			/* Make sure handles are not partitioned */
			STARPU_ASSERT(task->handles[i]->nchildren == 0);
		}

		/* In case we require that a task should be explicitely
		 * executed on a specific worker, we make sure that the worker
		 * is able to execute this task.  */
		if (task->execute_on_a_specific_worker && !starpu_combined_worker_can_execute_task(task->workerid, task, 0))
		{
                        _STARPU_LOG_OUT_TAG("ENODEV");
			return -ENODEV;
                }

		_starpu_detect_implicit_data_deps(task);

		if (task->cl->model)
			_starpu_load_perfmodel(task->cl->model);

		if (task->cl->power_model)
			_starpu_load_perfmodel(task->cl->power_model);
	}

	/* If profiling is activated, we allocate a structure to store the
	 * appropriate info. */
	struct starpu_task_profiling_info *info;
	int profiling = starpu_profiling_status_get();
	info = _starpu_allocate_profiling_info_if_needed(task);
	task->profiling_info = info;

	/* The task is considered as block until we are sure there remains not
	 * dependency. */
	task->status = STARPU_TASK_BLOCKED;


	if (profiling)
		_starpu_clock_gettime(&info->submit_time);

	/* internally, StarPU manipulates a struct _starpu_job * which is a wrapper around a
	* task structure, it is possible that this job structure was already
	* allocated, for instance to enforce task depenencies. */
	struct _starpu_job *j = _starpu_get_job_associated_to_task(task);

	ret = _starpu_submit_job(j);

	if (is_sync)
	{
		_starpu_wait_job(j);
		if (task->destroy)
		     _starpu_task_destroy(task);
	}

        _STARPU_LOG_OUT();
	return ret;
}

/* The StarPU core can submit tasks directly to the scheduler or a worker,
 * skipping dependencies completely (when it knows what it is doing).  */
int _starpu_task_submit_nodeps(struct starpu_task *task)
{
	_starpu_task_check_deprecated_fields(task);
	_starpu_codelet_check_deprecated_fields(task->cl);

	if (task->cl)
	{
		if (task->cl->model)
			_starpu_load_perfmodel(task->cl->model);

		if (task->cl->power_model)
			_starpu_load_perfmodel(task->cl->power_model);
	}

	struct _starpu_job *j = _starpu_get_job_associated_to_task(task);
	_starpu_increment_nsubmitted_tasks();

	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);

	j->submitted = 1;

	if (task->cl)
	{
		/* This would be done by data dependencies checking */
		unsigned i;
		for (i=0 ; i<task->cl->nbuffers ; i++)
		{
			j->ordered_buffers[i].handle = j->task->handles[i];
			j->ordered_buffers[i].mode = j->task->cl->modes[i];
		}
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

	return _starpu_push_task(j);
}

/*
 * worker->sched_mutex must be locked when calling this function.
 */
int _starpu_task_submit_conversion_task(struct starpu_task *task,
					unsigned int workerid)
{
	STARPU_ASSERT(task->cl);
	STARPU_ASSERT(task->execute_on_a_specific_worker);

	_starpu_task_check_deprecated_fields(task);
	_starpu_codelet_check_deprecated_fields(task->cl);

	/* We should factorize that */
	if (task->cl->model)
		_starpu_load_perfmodel(task->cl->model);

	if (task->cl->power_model)
		_starpu_load_perfmodel(task->cl->power_model);

	struct _starpu_job *j = _starpu_get_job_associated_to_task(task);
	_starpu_increment_nsubmitted_tasks();
	_STARPU_PTHREAD_MUTEX_LOCK(&j->sync_mutex);
	j->submitted = 1;
	_starpu_increment_nready_tasks();

	unsigned i;
	for (i=0 ; i<task->cl->nbuffers ; i++)
	{
		j->ordered_buffers[i].handle = j->task->handles[i];
		j->ordered_buffers[i].mode = j->task->cl->modes[i];
	}

        _STARPU_LOG_IN();

	task->status = STARPU_TASK_READY;
	_starpu_profiling_set_task_push_start_time(task);

	unsigned node = starpu_worker_get_memory_node(workerid);
	if (starpu_get_prefetch_flag())
		starpu_prefetch_task_input_on_node(task, node);

	struct _starpu_worker *worker;
	worker = _starpu_get_worker_struct(workerid);
	starpu_task_list_push_front(&worker->local_tasks, task);

	_starpu_profiling_set_task_push_end_time(task);

        _STARPU_LOG_OUT();
	_STARPU_PTHREAD_MUTEX_UNLOCK(&j->sync_mutex);

	return 0;
}

void starpu_codelet_init(struct starpu_codelet *cl)
{
	memset(cl, 0, sizeof(struct starpu_codelet));
}

void starpu_display_codelet_stats(struct starpu_codelet *cl)
{
	unsigned worker;
	unsigned nworkers = starpu_worker_get_count();

	if (cl->name)
		fprintf(stderr, "Statistics for codelet %s\n", cl->name);
	else if (cl->model && cl->model->symbol)
		fprintf(stderr, "Statistics for codelet %s\n", cl->model->symbol);

	unsigned long total = 0;

	for (worker = 0; worker < nworkers; worker++)
		total += cl->per_worker_stats[worker];

	for (worker = 0; worker < nworkers; worker++)
	{
		char name[32];
		starpu_worker_get_name(worker, name, 32);

		fprintf(stderr, "\t%s -> %lu / %lu (%2.2f %%)\n", name, cl->per_worker_stats[worker], total, (100.0f*cl->per_worker_stats[worker])/total);
	}
}

/*
 * We wait for all the tasks that have already been submitted. Note that a
 * regenerable is not considered finished until it was explicitely set as
 * non-regenerale anymore (eg. from a callback).
 */
int starpu_task_wait_for_all(void)
{
	if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
		return -EDEADLK;

	_STARPU_PTHREAD_MUTEX_LOCK(&submitted_mutex);

	_STARPU_TRACE_TASK_WAIT_FOR_ALL;

	while (nsubmitted > 0)
		_STARPU_PTHREAD_COND_WAIT(&submitted_cond, &submitted_mutex);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&submitted_mutex);

	return 0;
}

/*
 * We wait until there is no ready task any more (i.e. StarPU will not be able
 * to progress any more).
 */
int starpu_task_wait_for_no_ready(void)
{
	if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
		return -EDEADLK;

	_STARPU_PTHREAD_MUTEX_LOCK(&submitted_mutex);

	_STARPU_TRACE_TASK_WAIT_FOR_ALL;

	while (nready > 0)
		_STARPU_PTHREAD_COND_WAIT(&submitted_cond, &submitted_mutex);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&submitted_mutex);

	return 0;
}

void _starpu_decrement_nsubmitted_tasks(void)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&submitted_mutex);

	if (--nsubmitted == 0)
		_STARPU_PTHREAD_COND_BROADCAST(&submitted_cond);

	_STARPU_TRACE_UPDATE_TASK_CNT(nsubmitted);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&submitted_mutex);

}

static void _starpu_increment_nsubmitted_tasks(void)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&submitted_mutex);

	nsubmitted++;

	_STARPU_TRACE_UPDATE_TASK_CNT(nsubmitted);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&submitted_mutex);
}

void _starpu_increment_nready_tasks(void)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&submitted_mutex);

	nready++;

	_STARPU_PTHREAD_MUTEX_UNLOCK(&submitted_mutex);
}

void _starpu_decrement_nready_tasks(void)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&submitted_mutex);

	if (--nready == 0)
		_STARPU_PTHREAD_COND_BROADCAST(&submitted_cond);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&submitted_mutex);

}

void _starpu_initialize_current_task_key(void)
{
	pthread_key_create(&current_task_key, NULL);
}

/* Return the task currently executed by the worker, or NULL if this is called
 * either from a thread that is not a task or simply because there is no task
 * being executed at the moment. */
struct starpu_task *starpu_task_get_current(void)
{
	return (struct starpu_task *) pthread_getspecific(current_task_key);
}

void _starpu_set_current_task(struct starpu_task *task)
{
	pthread_setspecific(current_task_key, task);
}

/*
 * Returns 0 if tasks does not use any multiformat handle, 1 otherwise.
 */
int
_starpu_task_uses_multiformat_handles(struct starpu_task *task)
{
	unsigned i;
	for (i = 0; i < task->cl->nbuffers; i++)
	{
		if (_starpu_data_is_multiformat_handle(task->handles[i]))
			return 1;
	}

	return 0;
}

/*
 * Checks whether the given handle needs to be converted in order to be used on
 * the node given as the second argument.
 */
int
_starpu_handle_needs_conversion_task(starpu_data_handle_t handle,
				     unsigned int node)
{
	enum starpu_node_kind node_kind;

	node_kind = starpu_node_get_kind(node);

	/*
	 * Here, we assume that CUDA devices and OpenCL devices use the 
	 * same data structure. A conversion is only needed when moving 
	 * data from a CPU to a GPU, or the other way around.
	 */
	switch (node_kind)
	{
		case STARPU_CPU_RAM:
			switch(starpu_node_get_kind(handle->mf_node))
			{
				case STARPU_CPU_RAM:
					return 0;
				case STARPU_CUDA_RAM:      /* Fall through */
				case STARPU_OPENCL_RAM:
					return 1;
				case STARPU_SPU_LS: /* Not supported */
				default:
					STARPU_ASSERT(0);
			}
			break;
		case STARPU_CUDA_RAM:    /* Fall through */
		case STARPU_OPENCL_RAM:
			switch(starpu_node_get_kind(handle->mf_node))
			{
				case STARPU_CPU_RAM:
					return 1;
				case STARPU_CUDA_RAM:
				case STARPU_OPENCL_RAM:
					return 0;
				case STARPU_SPU_LS: /* Not supported */
				default:
					STARPU_ASSERT(0);
			}
			break;
		case STARPU_SPU_LS:            /* Not supported */
		default:
			STARPU_ASSERT(0);
	}
	/* that instruction should never be reached */
	return -EINVAL;
}

starpu_cpu_func_t _starpu_task_get_cpu_nth_implementation(struct starpu_codelet *cl, unsigned nimpl)
{
	STARPU_ASSERT(cl->cpu_func == STARPU_MULTIPLE_CPU_IMPLEMENTATIONS);
	return cl->cpu_funcs[nimpl];
}

starpu_cuda_func_t _starpu_task_get_cuda_nth_implementation(struct starpu_codelet *cl, unsigned nimpl)
{
	STARPU_ASSERT(cl->cuda_func == STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS);
	return cl->cuda_funcs[nimpl];
}

starpu_opencl_func_t _starpu_task_get_opencl_nth_implementation(struct starpu_codelet *cl, unsigned nimpl)
{
	STARPU_ASSERT(cl->opencl_func == STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS);
	return cl->opencl_funcs[nimpl];
}

starpu_gordon_func_t _starpu_task_get_gordon_nth_implementation(struct starpu_codelet *cl, unsigned nimpl)
{
	STARPU_ASSERT(cl->gordon_func == STARPU_MULTIPLE_GORDON_IMPLEMENTATIONS);
	return cl->gordon_funcs[nimpl];
}
