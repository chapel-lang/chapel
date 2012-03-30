/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 * Copyright (C) 2012 Inria
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

#ifndef __CORE_TASK_BUNDLE_H__
#define __CORE_TASK_BUNDLE_H__

#if ! defined(_MSC_VER)
#  include <pthread.h>
#endif

/* struct _starpu_task_bundle_entry
 * ================================
 * Purpose
 * =======
 * Structure used to describe a linked list containing tasks in _starpu_task_bundle.
 *
 * Fields
 * ======
 * task			Pointer to the task structure.
 *
 * next			Pointer to the next element in the linked list.
 */

struct _starpu_task_bundle_entry
{
	struct starpu_task *task;
	struct _starpu_task_bundle_entry *next;
};

/* struct _starpu_task_bundle
 * ==========================
 * Purpose
 * =======
 * Structure describing a list of tasks that should be scheduled on the same
 * worker whenever it's possible.
 * It must be considered as a hint given to the scheduler as there is no guarantee that
 * they will be executed on the same worker.
 *
 * Fields
 * ======
 * mutex		Mutex protecting the structure.
 *
 * list			Array of tasks included in the bundle.
 *
 * closed		Used to know if the user is still willing to
 * 			add/remove some tasks in the bundle. Especially useful for
 * 			the runtime to know whether it is safe to destroy a bundle.
 */

struct _starpu_task_bundle
{
	/* Mutex protecting the bundle */
#if defined(_MSC_VER)
	void *mutex;
#else
	pthread_mutex_t mutex;
#endif

	struct _starpu_task_bundle_entry *list;

	int closed;
};

/* struct _starpu_handle_list
 * ==========================
 * Purpose
 * =======
 * Structure describing a list of handles sorted by address to speed-up
 * when looking for an element.
 * The list cannot containes duplicate handles.
 *
 * Fields
 * ======
 * handle		Pointer to the handle structure.
 *
 * access_mode		Total access mode over the whole bundle.
 *
 * next			Pointer to the next element in the linked list.
 */

struct _starpu_handle_list
{
	starpu_data_handle_t handle;
	enum starpu_access_mode mode;
	struct _starpu_handle_list *next;
};

/* _starpu_task_bundle_destroy
 * ==========================
 * Purpose
 * =======
 * Destroy and deinitialize a bundle,
 * memory previoulsy allocated is freed.
 *
 * Arguments
 * =========
 * bundle		(input)
 * 			Bundle to destroy.
 */
void _starpu_task_bundle_destroy(starpu_task_bundle_t bundle);

/* _insertion_handle_sorted
 * ========================
 * Purpose
 * =======
 * Insert an handle in a _starpu_handle_list, elements are sorted
 * in increasing order, considering their physical address.
 * As the list doesn't accept duplicate elements, a handle with the
 * same address as an handle contained in the list is not inserted, but
 * its mode access is merged with the one of the latter.
 *
 * Arguments
 * =========
 * listp		(input, output)
 * 			Pointer to the first element of the list.
 * 			In the case of an empty list or an inserted handle with small address,
 * 			it should have changed when the call returns.
 *
 * handle		(input)
 * 			Handle to insert in the list.
 *
 * mode			(input)
 * 			Access mode of the handle.
 */
void _insertion_handle_sorted(struct _starpu_handle_list **listp, starpu_data_handle_t handle, enum starpu_access_mode mode);

#endif // __CORE_TASK_BUNDLE_H__
