/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
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
#include <common/utils.h>
#include <util/starpu_data_cpy.h>
#include <core/task.h>
#include <datawizard/datawizard.h>

void starpu_data_set_reduction_methods(starpu_data_handle_t handle,
				       struct starpu_codelet *redux_cl,
				       struct starpu_codelet *init_cl)
{
	_starpu_spin_lock(&handle->header_lock);

	_starpu_codelet_check_deprecated_fields(redux_cl);
	_starpu_codelet_check_deprecated_fields(init_cl);

	unsigned child;
	for (child = 0; child < handle->nchildren; child++)
	{
		/* make sure that the flags are applied to the children as well */
		struct _starpu_data_state *child_handle = &handle->children[child];
		if (child_handle->nchildren > 0)
			starpu_data_set_reduction_methods(child_handle, redux_cl, init_cl);
	}

	handle->redux_cl = redux_cl;
	handle->init_cl = init_cl;

	_starpu_spin_unlock(&handle->header_lock);
}

void _starpu_redux_init_data_replicate(starpu_data_handle_t handle, struct _starpu_data_replicate *replicate, int workerid)
{
	STARPU_ASSERT(replicate);
	STARPU_ASSERT(replicate->allocated);

	struct starpu_codelet *init_cl = handle->init_cl;
	STARPU_ASSERT(init_cl);

	_starpu_cl_func_t init_func = NULL;

	/* TODO Check that worker may execute the codelet */

	switch (starpu_worker_get_type(workerid))
	{
		case STARPU_CPU_WORKER:
			init_func = _starpu_task_get_cpu_nth_implementation(init_cl, 0);
			break;
		case STARPU_CUDA_WORKER:
			init_func = _starpu_task_get_cuda_nth_implementation(init_cl, 0);
			break;
		case STARPU_OPENCL_WORKER:
			init_func = _starpu_task_get_opencl_nth_implementation(init_cl, 0);
			break;
		default:
			STARPU_ABORT();
			break;
	}

	STARPU_ASSERT(init_func);

	init_func(&replicate->data_interface, NULL);

	replicate->initialized = 1;
}

/* Enable reduction mode. This function must be called with the header lock
 * taken. */
void _starpu_data_start_reduction_mode(starpu_data_handle_t handle)
{
	STARPU_ASSERT(handle->reduction_refcnt == 0);

	unsigned worker;

	unsigned nworkers = starpu_worker_get_count();
	for (worker = 0; worker < nworkers; worker++)
	{
		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_worker[worker];
		replicate->initialized = 0;
		replicate->relaxed_coherency = 2;
		if (replicate->mc)
			replicate->mc->relaxed_coherency = 2;
	}
}

//#define NO_TREE_REDUCTION

/* Force reduction. The lock should already have been taken.  */
void _starpu_data_end_reduction_mode(starpu_data_handle_t handle)
{
	unsigned worker;
	unsigned node;
	unsigned empty; /* Whether the handle is initially unallocated */

	/* Put every valid replicate in the same array */
	unsigned replicate_count = 0;
	starpu_data_handle_t replicate_array[1 + STARPU_NMAXWORKERS];

	_starpu_spin_checklocked(&handle->header_lock);

	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		if (handle->per_node[node].state != STARPU_INVALID)
			break;
	}
	empty = node == STARPU_MAXNODES;

#ifndef NO_TREE_REDUCTION
	if (!empty)
		/* Include the initial value into the reduction tree */
		replicate_array[replicate_count++] = handle;
#endif

	/* Register all valid per-worker replicates */
	unsigned nworkers = starpu_worker_get_count();
	for (worker = 0; worker < nworkers; worker++)
	{
		if (handle->per_worker[worker].initialized)
		{
			/* Make sure the replicate is not removed */
			handle->per_worker[worker].refcnt++;

			uint32_t home_node = starpu_worker_get_memory_node(worker);
			starpu_data_register(&handle->reduction_tmp_handles[worker],
				home_node, handle->per_worker[worker].data_interface, handle->ops);

			starpu_data_set_sequential_consistency_flag(handle->reduction_tmp_handles[worker], 0);

			replicate_array[replicate_count++] = handle->reduction_tmp_handles[worker];
		}
		else
		{
			handle->reduction_tmp_handles[worker] = NULL;
		}
	}

#ifndef NO_TREE_REDUCTION
	if (empty) {
		/* Only the final copy will touch the actual handle */
		handle->reduction_refcnt = 1;
	} else {
		unsigned step = 1;
		handle->reduction_refcnt = 0;
		while (step < replicate_count)
		{
			/* Each stage will touch the actual handle */
			handle->reduction_refcnt++;
			step *= 2;
		}
	}
#else
	/* We know that in this reduction algorithm there is exactly one task per valid replicate. */
	handle->reduction_refcnt = replicate_count + empty;
#endif

//	fprintf(stderr, "REDUX REFCNT = %d\n", handle->reduction_refcnt);

	if (replicate_count >
#ifndef NO_TREE_REDUCTION
			!empty
#else
			0
#endif
			)
	{
		/* Temporarily unlock the handle */
		_starpu_spin_unlock(&handle->header_lock);

#ifndef NO_TREE_REDUCTION
		/* We will store a pointer to the last task which should modify the
		 * replicate */
		struct starpu_task *last_replicate_deps[replicate_count];
		memset(last_replicate_deps, 0, replicate_count*sizeof(struct starpu_task *));
		struct starpu_task *redux_tasks[replicate_count];

		/* Redux step-by-step for step from 1 to replicate_count/2, i.e.
		 * 1-by-1, then 2-by-2, then 4-by-4, etc. */
		unsigned step;
		unsigned redux_task_idx = 0;
		for (step = 1; step < replicate_count; step *=2)
		{
			unsigned i;
			for (i = 0; i < replicate_count; i+=2*step)
			{
				if (i + step < replicate_count)
				{
					/* Perform the reduction between replicates i
					 * and i+step and put the result in replicate i */
					struct starpu_task *redux_task = starpu_task_create();

					/* Mark these tasks so that StarPU does not block them
					 * when they try to access the handle (normal tasks are
					 * data requests to that handle are frozen until the
					 * data is coherent again). */
					struct _starpu_job *j = _starpu_get_job_associated_to_task(redux_task);
					j->reduction_task = 1;

					redux_task->cl = handle->redux_cl;
					STARPU_ASSERT(redux_task->cl);

					redux_task->handles[0] = replicate_array[i];
					redux_task->cl->modes[0] = STARPU_RW;

					redux_task->handles[1] = replicate_array[i+step];
					redux_task->cl->modes[1] = STARPU_R;

					int ndeps = 0;
					struct starpu_task *task_deps[2];

					if (last_replicate_deps[i])
						task_deps[ndeps++] = last_replicate_deps[i];

					if (last_replicate_deps[i+step])
						task_deps[ndeps++] = last_replicate_deps[i+step];

					/* i depends on this task */
					last_replicate_deps[i] = redux_task;

					/* we don't perform the reduction until both replicates are ready */
					starpu_task_declare_deps_array(redux_task, ndeps, task_deps);

					/* We cannot submit tasks here : we do
					 * not want to depend on tasks that have
					 * been completed, so we juste store
					 * this task : it will be submitted
					 * later. */
					redux_tasks[redux_task_idx++] = redux_task;
				}
			}
		}

		if (empty)
			/* The handle was empty, we just need to copy the reduced value. */
			_starpu_data_cpy(handle, replicate_array[0], 1, NULL, 0, 1, last_replicate_deps[0]);

		/* Let's submit all the reduction tasks. */
		unsigned i;
		for (i = 0; i < redux_task_idx; i++)
		{
			int ret = starpu_task_submit(redux_tasks[i]);
			STARPU_ASSERT(ret == 0);
		}
#else
		if (empty) {
			struct starpu_task *redux_task = starpu_task_create();

			/* Mark these tasks so that StarPU does not block them
			 * when they try to access the handle (normal tasks are
			 * data requests to that handle are frozen until the
			 * data is coherent again). */
			struct _starpu_job *j = _starpu_get_job_associated_to_task(redux_task);
			j->reduction_task = 1;

			redux_task->cl = handle->init_cl;
			STARPU_ASSERT(redux_task->cl);
#ifdef STARPU_DEVEL
#  warning the mode should already be set in the codelet. Only check they are valid?
#endif
			redux_task->cl->modes[0] = STARPU_W;
			redux_task->handles[0] = handle;

			int ret = starpu_task_submit(redux_task);
			STARPU_ASSERT(!ret);
		}

		/* Create a set of tasks to perform the reduction */
		unsigned replicate;
		for (replicate = 0; replicate < replicate_count; replicate++)
		{
			struct starpu_task *redux_task = starpu_task_create();

			/* Mark these tasks so that StarPU does not block them
			 * when they try to access the handle (normal tasks are
			 * data requests to that handle are frozen until the
			 * data is coherent again). */
			struct _starpu_job *j = _starpu_get_job_associated_to_task(redux_task);
			j->reduction_task = 1;

			redux_task->cl = handle->redux_cl;
			STARPU_ASSERT(redux_task->cl);

#ifdef STARPU_DEVEL
#  warning the modes should already be set in the codelet. Only check they are valid?
#endif
			redux_task->cl->modes[0] = STARPU_RW;
			redux_task->cl->modes[1] = STARPU_R;

			redux_task->handles[0] = handle;
			redux_task->handles[1] = replicate_array[replicate];

			int ret = starpu_task_submit(redux_task);
			STARPU_ASSERT(!ret);
		}
#endif
	/* Get the header lock back */
	_starpu_spin_lock(&handle->header_lock);

	}

	for (worker = 0; worker < nworkers; worker++)
	{
		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_worker[worker];
		replicate->relaxed_coherency = 1;
		if (replicate->mc)
			replicate->mc->relaxed_coherency = 1;
	}
}

void _starpu_data_end_reduction_mode_terminate(starpu_data_handle_t handle)
{
	unsigned nworkers = starpu_worker_get_count();

//	fprintf(stderr, "_starpu_data_end_reduction_mode_terminate\n");
	unsigned worker;

	_starpu_spin_checklocked(&handle->header_lock);

	for (worker = 0; worker < nworkers; worker++)
	{
		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_worker[worker];
		replicate->initialized = 0;

		if (handle->reduction_tmp_handles[worker])
		{
//			fprintf(stderr, "unregister handle %p\n", handle);
			handle->reduction_tmp_handles[worker]->lazy_unregister = 1;
			starpu_data_unregister_no_coherency(handle->reduction_tmp_handles[worker]);
			handle->per_worker[worker].refcnt--;
			/* TODO put in cache */
		}
	}
}
