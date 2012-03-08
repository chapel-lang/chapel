/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#include <pthread.h>
#include <core/workers.h>
#include <common/utils.h>

#define NMAXHOOKS	16

struct progression_hook
{
	unsigned (*func)(void *arg);
	void *arg;
	unsigned active;
};

/* protect the hook table */
static pthread_rwlock_t progression_hook_rwlock = PTHREAD_RWLOCK_INITIALIZER;

static struct progression_hook hooks[NMAXHOOKS] = {{NULL, NULL, 0}};
static int active_hook_cnt = 0;

int starpu_progression_hook_register(unsigned (*func)(void *arg), void *arg)
{
	int hook;
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&progression_hook_rwlock);
	for (hook = 0; hook < NMAXHOOKS; hook++)
	{
		if (!hooks[hook].active)
		{
			/* We found an empty slot */
			hooks[hook].func = func;
			hooks[hook].arg = arg;
			hooks[hook].active = 1;
			active_hook_cnt++;

			_STARPU_PTHREAD_RWLOCK_UNLOCK(&progression_hook_rwlock);

			return hook;
		}
	}

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&progression_hook_rwlock);

	starpu_wake_all_blocked_workers();

	/* We could not find an empty slot */
	return -1;
}

void starpu_progression_hook_deregister(int hook_id)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&progression_hook_rwlock);

	if (hooks[hook_id].active)
		active_hook_cnt--;

	hooks[hook_id].active = 0;

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&progression_hook_rwlock);
}

unsigned _starpu_execute_registered_progression_hooks(void)
{
	/* If there is no hook registered, we short-cut loop. */
	_STARPU_PTHREAD_RWLOCK_RDLOCK(&progression_hook_rwlock);
	int no_hook = (active_hook_cnt == 0);
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&progression_hook_rwlock);

	if (no_hook)
		return 1;

	/* By default, it is possible to block, but if some progression hooks
	 * requires that it's not blocking, we disable blocking. */
	unsigned may_block = 1;

	unsigned hook;
	for (hook = 0; hook < NMAXHOOKS; hook++)
	{
		unsigned active;

		_STARPU_PTHREAD_RWLOCK_RDLOCK(&progression_hook_rwlock);
		active = hooks[hook].active;
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&progression_hook_rwlock);

		unsigned may_block_hook = 1;

		if (active)
			may_block_hook = hooks[hook].func(hooks[hook].arg);

		/* As soon as one hook tells that the driver cannot be
		 * blocking, we don't allow it. */
		if (!may_block_hook)
			may_block = 0;
	}

	return may_block;
}
