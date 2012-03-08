/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
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

/* This creates (and submits) an empty task that unlocks a tag once all its
 * dependencies are fulfilled. */
/* TODO it would be nice to have such a function without sync_tag in case we
 * just want to execute the callback. */
void starpu_create_sync_task(starpu_tag_t sync_tag, unsigned ndeps, starpu_tag_t *deps,
				void (*callback)(void *), void *callback_arg)
{
	starpu_tag_declare_deps_array(sync_tag, ndeps, deps);

	/* We create an empty task */
	struct starpu_task *sync_task = starpu_task_create();

	sync_task->use_tag = 1;
	sync_task->tag_id = sync_tag;

	sync_task->callback_func = callback;
	sync_task->callback_arg = callback_arg;

	/* This task does nothing */
	sync_task->cl = NULL;

	int sync_ret = starpu_task_submit(sync_task);
	STARPU_ASSERT(!sync_ret);
}
