/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
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

#ifndef __SCHED_POLICY_H__
#define __SCHED_POLICY_H__

#include <starpu.h>
#include <core/workers.h>

#include <starpu_scheduler.h>

struct _starpu_machine_config;
struct starpu_sched_policy *_starpu_get_sched_policy(void);

void _starpu_init_sched_policy(struct _starpu_machine_config *config);
void _starpu_deinit_sched_policy(struct _starpu_machine_config *config);

int _starpu_push_task(struct _starpu_job *task);
/* pop a task that can be executed on the worker */
struct starpu_task *_starpu_pop_task(struct _starpu_worker *worker);
/* pop every task that can be executed on the worker */
struct starpu_task *_starpu_pop_every_task(void);
void _starpu_sched_post_exec_hook(struct starpu_task *task);

void _starpu_wait_on_sched_event(void);

struct starpu_task *_starpu_create_conversion_task(starpu_data_handle_t handle,
						   unsigned int node);

void _starpu_sched_pre_exec_hook(struct starpu_task *task);

#endif // __SCHED_POLICY_H__
