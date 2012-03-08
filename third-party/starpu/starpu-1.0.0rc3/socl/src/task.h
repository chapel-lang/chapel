/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010,2011 University of Bordeaux
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

#ifndef SOCL_TASK_H
#define SOCL_TASK_H

#include "socl.h"

starpu_task task_create();
void task_dependency_add(starpu_task task, cl_uint num_events, cl_event *events);

starpu_task task_create_cpu(void (*callback)(void*), void *arg, int free_arg);

/**
 * Associate a StarPU task to a command and submit it
 *
 * When the task terminates, the command is set as terminated too
 */
cl_int task_submit_ex(starpu_task task, cl_command cmd);
#define task_submit(task,cmd) task_submit_ex(task, (cl_command)cmd)

/**
 * Add task dependencies
 */
void task_depends_on(starpu_task task, cl_uint num_events, cl_event *events);

#endif /* SOCL_TASK_H */
