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
#include <starpu_scheduler.h>
#include <common/config.h>

/* By default we have a binary type of priority: either a task is a priority
 * task (level 1) or it is not (level 0). */
static int sched_min_prio = 0;
static int sched_max_prio = 1;

/* The scheduling policy may set its own priority bounds in case it supports
 * different priority levels. These methods should only be called from the
 * scheduling policy. */
void starpu_sched_set_min_priority(int min_prio)
{
	sched_min_prio = min_prio;
}

void starpu_sched_set_max_priority(int max_prio)
{
	sched_max_prio = max_prio;
}

/* Returns the minimum priority level supported by the scheduling policy. */
int starpu_sched_get_min_priority(void)
{
	return sched_min_prio;
}

/* Returns the maximum priority level supported by the scheduling policy. */
int starpu_sched_get_max_priority(void)
{
	return sched_max_prio;
}
