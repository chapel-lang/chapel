/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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
#include <core/jobs.h>
#include <core/task.h>
#include <common/utils.h>

struct starpu_task *_starpu_create_task_alias(struct starpu_task *task)
{
	struct starpu_task *task_dup = (struct starpu_task *) malloc(sizeof(struct starpu_task));
	STARPU_ASSERT(task_dup);

	/* XXX perhaps this is a bit too much overhead and we should only copy
	 * part of the structure ? */
	memcpy(task_dup, task, sizeof(struct starpu_task));

	return task_dup;
}
