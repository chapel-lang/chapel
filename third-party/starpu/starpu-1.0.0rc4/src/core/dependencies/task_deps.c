/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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
#include <common/config.h>
#include <common/utils.h>
#include <core/dependencies/tags.h>
#include <core/dependencies/htable.h>
#include <core/jobs.h>
#include <core/task.h>
#include <core/sched_policy.h>
#include <core/dependencies/data_concurrency.h>
#include <profiling/bound.h>

static struct _starpu_cg *create_cg_task(unsigned ntags, struct _starpu_job *j)
{
	struct _starpu_cg *cg = (struct _starpu_cg *) malloc(sizeof(struct _starpu_cg));
	STARPU_ASSERT(cg);

	cg->ntags = ntags;
	cg->remaining = ntags;
	cg->cg_type = STARPU_CG_TASK;

	cg->succ.job = j;
	j->job_successors.ndeps++;

	return cg;
}

static void _starpu_task_add_succ(struct _starpu_job *j, struct _starpu_cg *cg)
{
	STARPU_ASSERT(j);

	if (_starpu_add_successor_to_cg_list(&j->job_successors, cg))
		/* the task was already completed sooner */
		_starpu_notify_cg(cg);
}

void _starpu_notify_task_dependencies(struct _starpu_job *j)
{
	_starpu_notify_cg_list(&j->job_successors);
}

/* task depends on the tasks in task array */
void _starpu_task_declare_deps_array(struct starpu_task *task, unsigned ndeps, struct starpu_task *task_array[], int check)
{
	if (ndeps == 0)
		return;

	struct _starpu_job *job;

	job = _starpu_get_job_associated_to_task(task);

	if (check)
		STARPU_ASSERT_MSG(!job->submitted || !task->destroy || task->detach, "Task dependencies have to be set before submission");
	else
		STARPU_ASSERT_MSG(job->terminated <= 1, "Task dependencies have to be set before termination");

	struct _starpu_cg *cg = create_cg_task(ndeps, job);

	unsigned i;
	for (i = 0; i < ndeps; i++)
	{
		struct starpu_task *dep_task = task_array[i];

		struct _starpu_job *dep_job;
		dep_job = _starpu_get_job_associated_to_task(dep_task);

		STARPU_ASSERT_MSG(dep_job != job, "A task must not depend on itself.");
		if (check) {
			STARPU_ASSERT_MSG(!dep_job->submitted || !dep_job->task->destroy || dep_job->task->detach, "Unless it is not to be destroyed automatically, a task dependencies have to be set before submission");
			STARPU_ASSERT_MSG(dep_job->submitted != 2, "For resubmited tasks, dependencies have to be set before first re-submission");
			STARPU_ASSERT_MSG(!dep_job->submitted || !dep_job->task->regenerate, "For regenerated tasks, dependencies have to be set before first submission");
		} else
			STARPU_ASSERT_MSG(dep_job->terminated <= 1, "Task dependencies have to be set before termination");

		_STARPU_TRACE_TASK_DEPS(dep_job, job);
		_starpu_bound_task_dep(job, dep_job);

		_starpu_task_add_succ(dep_job, cg);
	}
}

void starpu_task_declare_deps_array(struct starpu_task *task, unsigned ndeps, struct starpu_task *task_array[])
{
	_starpu_task_declare_deps_array(task, ndeps, task_array, 1);
}
