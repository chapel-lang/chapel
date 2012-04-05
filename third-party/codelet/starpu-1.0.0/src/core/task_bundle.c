/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
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

#include <starpu.h>
#include <starpu_task_bundle.h>
#include <core/task_bundle.h>
#include <starpu_scheduler.h>
#include <common/config.h>
#include <common/utils.h>
#include <common/list.h>

/* Initialize a task bundle */
void starpu_task_bundle_create(starpu_task_bundle_t *bundle)
{
	*bundle = (starpu_task_bundle_t) malloc(sizeof(struct _starpu_task_bundle));
	STARPU_ASSERT(*bundle);

	_STARPU_PTHREAD_MUTEX_INIT(&(*bundle)->mutex, NULL);
	/* Of course at the beginning a bundle is open,
	 * user can insert and remove tasks from it */
	(*bundle)->closed = 0;

	/* Start with an empty list */
	(*bundle)->list = NULL;

}

int starpu_task_bundle_insert(starpu_task_bundle_t bundle, struct starpu_task *task)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&bundle->mutex);

	if (bundle->closed)
	{
		/* The bundle is closed, we cannot add task anymore */
		_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
		return -EPERM;
	}

	if (task->status != STARPU_TASK_INVALID)
	{
		/* The task has already been submitted, it's too late to put it
		 * into a bundle now. */
		_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
		return -EINVAL;
	}

	/* Insert a task at the end of the bundle */
	struct _starpu_task_bundle_entry *entry;
	entry = (struct _starpu_task_bundle_entry *) malloc(sizeof(struct _starpu_task_bundle_entry));
	STARPU_ASSERT(entry);
	entry->task = task;
	entry->next = NULL;

	if (!bundle->list)
	{
		bundle->list = entry;
	}
	else
	{
		struct _starpu_task_bundle_entry *item;
		item = bundle->list;
		while (item->next)
			item = item->next;

		item->next = entry;
	}

	/* Mark the task as belonging the bundle */
	task->bundle = bundle;

	_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
	return 0;
}

int starpu_task_bundle_remove(starpu_task_bundle_t bundle, struct starpu_task *task)
{
	struct _starpu_task_bundle_entry *item;

	_STARPU_PTHREAD_MUTEX_LOCK(&bundle->mutex);

	item = bundle->list;

	/* List is empty, there is no way the task
	 * belong to it */
	if (!item)
	{
		_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
		return -ENOENT;
	}

	STARPU_ASSERT(task->bundle == bundle);
	task->bundle = NULL;

	if (item->task == task)
	{
		/* Remove the first element */
		bundle->list = item->next;
		free(item);

		/* If the list is now empty, deinitialize the bundle */
		if (bundle->closed && bundle->list == NULL)
		{
			_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
			_starpu_task_bundle_destroy(bundle);
			return 0;
		}

		_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
		return 0;
	}

	/* Go through the list until we find the right task,
	 * then we delete it */
	while (item->next)
	{
		struct _starpu_task_bundle_entry *next;
		next = item->next;

		if (next->task == task)
		{
			/* Remove the next element */
			item->next = next->next;
			_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
			free(next);
			return 0;
		}

		item = next;
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);

	/* We could not find the task in the bundle */
	return -ENOENT;
}

/* Close a bundle. No task can be added to a closed bundle. Tasks can still be
 * removed from a closed bundle. A closed bundle automatically gets
 * deinitialized when it becomes empty. A closed bundle cannot be reopened. */
void starpu_task_bundle_close(starpu_task_bundle_t bundle)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&bundle->mutex);

	/* If the bundle is already empty, we deinitialize it now as the
	 * user closed it and thus don't intend to insert new tasks in it. */
	if (bundle->list == NULL)
	{
		_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);
		_starpu_task_bundle_destroy(bundle);
		return;
	}

	/* Mark the bundle as closed */
	bundle->closed = 1;

	_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);

}

void _starpu_task_bundle_destroy(starpu_task_bundle_t bundle)
{
	/* Remove all entries from the bundle (which is likely to be empty) */
	while (bundle->list)
	{
		struct _starpu_task_bundle_entry *entry = bundle->list;
		bundle->list = bundle->list->next;
		free(entry);
	}

	_STARPU_PTHREAD_MUTEX_DESTROY(&bundle->mutex);

	free(bundle);
}

void _insertion_handle_sorted(struct _starpu_handle_list **listp, starpu_data_handle_t handle, enum starpu_access_mode mode)
{
	STARPU_ASSERT(listp);

	struct _starpu_handle_list *list = *listp;

	/* If the list is empty or the handle's address the smallest among the
	 * list, we insert it as first element */
	if (!list || list->handle > handle)
	{
		struct _starpu_handle_list *link = (struct _starpu_handle_list *) malloc(sizeof(struct _starpu_handle_list));
		STARPU_ASSERT(link);
		link->handle = handle;
		link->mode = mode;
		link->next = list;
		*listp = link;
		return;
	}

	struct _starpu_handle_list *prev = list;

	/* Look for the same handle if already present in the list.
	 * Else place it right before the smallest following handle */
	while (list && (handle >= list->handle))
	{
		prev = list;
		list = list->next;
	}

	if (prev->handle == handle)
	{
		/* The handle is already in the list, the merge both the access modes */
		prev->mode = (enum starpu_access_mode) ((int) prev->mode | (int) mode);
	}
	else
	{
		/* The handle was not in the list, we insert it after 'prev', thus right before
		 * 'list' which is the smallest following handle */
		struct _starpu_handle_list *link = (struct _starpu_handle_list *) malloc(sizeof(struct _starpu_handle_list));
		STARPU_ASSERT(link);
		link->handle = handle;
		link->mode = mode;
		link->next = prev->next;
		prev->next = link;
	}
}
