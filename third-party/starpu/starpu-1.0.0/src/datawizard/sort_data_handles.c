/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#include <datawizard/filters.h>
#include <datawizard/sort_data_handles.h>

/* To avoid deadlocks in case we have multiple tasks accessing the same piece
 * of data  (eg. task T1 needs A and B, and T2 needs B and A), we need to lock
 * them in order, so that we need a total order over data. We must also not
 * lock a child before its parent. */

static void find_data_path(struct _starpu_data_state *data, unsigned path[])
{
	unsigned depth = data->depth;
	struct _starpu_data_state *current = data;

	/* Compute the path from the root to the data */
	unsigned level; /* level is the distance between the node and the current node */
	for (level = 0; level < depth; level++)
	{
		STARPU_ASSERT(data);
		path[depth - level - 1] = current->sibling_index;
		current = data->father_handle;
	}
}

static int _compar_data_paths(const unsigned pathA[], unsigned depthA,
				const unsigned pathB[], unsigned depthB)
{
	unsigned level;
	unsigned depth = STARPU_MIN(depthA, depthB);

	for (level = 0; level < depth; level++)
	{
		if (pathA[level] != pathB[level])
			return (pathA[level] < pathB[level])?-1:1;
	}

	/* If this is the same path */
	if (depthA == depthB)
		return 0;

	/* A is a subdata of B or B is a subdata of A, so the smallest one is
	 * the father of the other (we take this convention). */
	return (depthA < depthB)?-1:1;
}

/* A comparision function between two handles makes it possible to use qsort to
 * sort a list of handles */
static int _starpu_compar_handles(const struct starpu_buffer_descr *descrA,
				  const struct starpu_buffer_descr *descrB)
{
	struct _starpu_data_state *dataA = descrA->handle;
	struct _starpu_data_state *dataB = descrB->handle;

	/* Perhaps we have the same piece of data */
	if (dataA == dataB)
	{
		/* Process write requests first, this is needed for proper
		 * locking, see _submit_job_enforce_data_deps,
		 * _starpu_fetch_task_input, and _starpu_push_task_output  */
		if (descrA->mode & STARPU_W)
		{
			if (descrB->mode & STARPU_W)
				/* Both A and B write, take the reader first */
				if (descrA->mode & STARPU_R)
					return -1;
				else
					return 1;
			else
				/* Only A writes, take it first */
				return -1;
		} else
			/* A doesn't write, take B before */
			return 1;
	}

	/* In case we have data/subdata from different trees */
	if (dataA->root_handle != dataB->root_handle)
		return ((dataA->root_handle < dataB->root_handle)?-1:1);

	/* Things get more complicated: we need to find the location of dataA
	 * and dataB within the tree. */
	unsigned dataA_path[dataA->depth - 1];
	unsigned dataB_path[dataB->depth - 1];

	find_data_path(dataA, dataA_path);
	find_data_path(dataB, dataB_path);

	return _compar_data_paths(dataA_path, dataA->depth, dataB_path, dataB->depth);
}

static int _starpu_compar_buffer_descr(const void *_descrA, const void *_descrB)
{
	const struct starpu_buffer_descr *descrA = (const struct starpu_buffer_descr *) _descrA;
	const struct starpu_buffer_descr *descrB = (const struct starpu_buffer_descr *) _descrB;

	return _starpu_compar_handles(descrA, descrB);
}

/* The descr array will be overwritten, so this must be a copy ! */
void _starpu_sort_task_handles(struct starpu_buffer_descr descr[], unsigned nbuffers)
{
	qsort(descr, nbuffers, sizeof(struct starpu_buffer_descr), _starpu_compar_buffer_descr);
}
