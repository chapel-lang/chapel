/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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
#include <core/sched_policy.h>
#include <core/dependencies/data_concurrency.h>

/* We assume that the job will not disappear under our hands */
void _starpu_notify_dependencies(struct _starpu_job *j)
{
	STARPU_ASSERT(j);
	STARPU_ASSERT(j->task);

	/* unlock tasks depending on that task */
	_starpu_notify_task_dependencies(j);

	/* unlock tags depending on that task */
	if (j->task->use_tag)
		_starpu_notify_tag_dependencies(j->tag);

}
