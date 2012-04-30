/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
 * Copyright (C) 2011  Télécom-SudParis
 * Copyright (C) 2012  Inria
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

#ifndef __STARPU_TASK_BUNDLE_H__
#define __STARPU_TASK_BUNDLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct starpu_task;

/* starpu_task_bundle_t
 * ==================
 * Purpose
 * =======
 * Opaque structure describing a list of tasks that should be scheduled
 * on the same worker whenever it's possible.
 * It must be considered as a hint given to the scheduler as there is no guarantee that
 * they will be executed on the same worker.
 */
typedef struct _starpu_task_bundle *starpu_task_bundle_t;

/* starpu_task_bundle_create
 * =========================
 * Purpose
 * =======
 * Factory function creating a bundle, when the call return,
 * memory needed is allocated and the bundle is ready to use.
 *
 * Arguments
 * =========
 * bundle		(output)
 * 			Bundle to create and initialize.
 */
void starpu_task_bundle_create(starpu_task_bundle_t *bundle);

/* starpu_task_bundle_insert
 * =========================
 * Purpose
 * =======
 * Insert a task in a bundle. Until the task is removed from the bundle
 * its expected length and data transfer time will be considered along
 * those of the other tasks of the bundle.
 * This function mustn't be called if the bundle is already closed and/or
 * the task is already submitted.
 *
 * Return value
 * ============
 * On success, it returns 0.
 * There are two cases of error :
 * 	- if the bundle is already closed it returns -EPERM
 * 	- if the task was already submitted it return -EINVAL.
 *
 * Arguments
 * =========
 * bundle		(input)
 * 			Bundle where to insert the task.
 *
 * task			(input)
 * 			Task to insert.
 */
int starpu_task_bundle_insert(starpu_task_bundle_t bundle, struct starpu_task *task);

/* starpu_task_bundle_remove
 * =========================
 * Purpose
 * =======
 * Remove the tasks passed as argument from the bundle.
 * Of course the task must have been previously inserted in the bundle.
 * This function mustn't be called if the bundle is already closed and/or
 * the task is already submitted. Doing so would result in undefined behaviour.
 *
 * Return value
 * ============
 * On success, it returns 0.
 * If the bundle is already closed it returns -ENOENT.
 *
 * Arguments
 * =========
 * bundle		(input)
 * 			Bundle containing the task.
 *
 * task			(input)
 * 			The task to remove.
 */
int starpu_task_bundle_remove(starpu_task_bundle_t bundle, struct starpu_task *task);

/* starpu_task_bundle_close
 * =========================
 * Purpose
 * =======
 * Calling this functions informs the runtime that the user
 * won't modify the bundle anymore, it means no more
 * inserting or removing a task.
 * Thus the runtime can destroy it when needed.
 *
 * Arguments
 * =========
 * bundle		(input)
 * 			Bundle to close.
 */
void starpu_task_bundle_close(starpu_task_bundle_t bundle);

#ifdef __cplusplus
}
#endif

#endif // __STARPU_TASK_BUNDLE_H__
