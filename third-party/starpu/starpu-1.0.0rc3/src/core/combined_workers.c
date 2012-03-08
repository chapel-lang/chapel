/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
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

#include <stdlib.h> // for qsort

#include <starpu.h>
#include <common/config.h>
#include <core/workers.h>

#include <sched.h>

#ifdef __MINGW32__
#include <windows.h>
#endif

#ifndef HWLOC_BITMAP_H
/* hwloc <1.1 does not offer the bitmap API yet */
#define hwloc_bitmap_alloc hwloc_cpuset_alloc
#define hwloc_bitmap_or hwloc_cpuset_or
#endif

static int compar_int(const void *pa, const void *pb)
{
	int a = *((int *)pa);
	int b = *((int *)pb);

	return a > b;
}

static void sort_workerid_array(int nworkers, int workerid_array[])
{
	qsort(workerid_array, nworkers, sizeof(int), compar_int);
}

/* Create a new worker id for a combination of workers. This method should
 * typically be called at the initialization of the scheduling policy. This
 * worker should be the combination of the list of id's contained in the
 * workerid_array array which has nworkers entries. This function returns
 * the identifier of the combined worker in case of success, a negative value
 * is returned otherwise. */
int starpu_combined_worker_assign_workerid(int nworkers, int workerid_array[])
{
	int new_workerid;

	/* Return the number of actual workers. */
	struct _starpu_machine_config *config = _starpu_get_machine_config();

	int basic_worker_count = (int)config->topology.nworkers;
	int combined_worker_id = (int)config->topology.ncombinedworkers;

	/* We sort the ids */
	sort_workerid_array(nworkers, workerid_array);

	/* Test that all workers are not combined workers already. */
	int i;
	for (i = 0; i < nworkers; i++)
	{
		int id = workerid_array[i];

		/* We only combine CPUs */
		STARPU_ASSERT(config->workers[id].perf_arch == STARPU_CPU_DEFAULT);
		STARPU_ASSERT(config->workers[id].worker_mask == STARPU_CPU);

		/* We only combine valid "basic" workers */
		if ((id < 0) || (id >= basic_worker_count))
			return -EINVAL;
	}

	/* Get an id for that combined worker. Note that this is not thread
	 * safe because thhis method should only be called when the scheduler
	 * is being initialized. */
	new_workerid = basic_worker_count + combined_worker_id;
	config->topology.ncombinedworkers++;

#if 0
	fprintf(stderr, "COMBINED WORKERS ");
	for (i = 0; i < nworkers; i++)
	{
		fprintf(stderr, "%d ", workerid_array[i]);
	}
	fprintf(stderr, "into worker %d\n", new_workerid);
#endif

	struct _starpu_combined_worker *combined_worker =
		&config->combined_workers[combined_worker_id];

	combined_worker->worker_size = nworkers;
	combined_worker->perf_arch = (enum starpu_perf_archtype) (STARPU_CPU_DEFAULT + nworkers - 1);
	combined_worker->worker_mask = STARPU_CPU;

	/* We assume that the memory node should either be that of the first
	 * entry, and it is very likely that every worker in the combination
	 * should be on the same memory node.*/
	int first_id = workerid_array[0];
	combined_worker->memory_node = config->workers[first_id].memory_node;

	/* Save the list of combined workers */
	memcpy(&combined_worker->combined_workerid, workerid_array, nworkers*sizeof(int));

	/* Note that we maintain both the cpu_set and the hwloc_cpu_set so that
	 * the application is not forced to use hwloc when it is available. */
#ifdef __GLIBC__
	CPU_ZERO(&combined_worker->cpu_set);
#endif /* __GLIBC__ */

#ifdef STARPU_HAVE_HWLOC
	combined_worker->hwloc_cpu_set = hwloc_bitmap_alloc();
#endif

	for (i = 0; i < nworkers; i++)
	{
#if defined(__GLIBC__) || defined(STARPU_HAVE_HWLOC)
		int id = workerid_array[i];
#ifdef __GLIBC__
#ifdef CPU_OR
		CPU_OR(&combined_worker->cpu_set,
			&combined_worker->cpu_set,
			&config->workers[id].initial_cpu_set);
#else
		int j;
		for (j = 0; j < CPU_SETSIZE; j++)
		{
			if (CPU_ISSET(j, &config->workers[id].initial_cpu_set))
				CPU_SET(j, &combined_worker->cpu_set);
		}
#endif
#endif /* __GLIBC__ */

#ifdef STARPU_HAVE_HWLOC
		hwloc_bitmap_or(combined_worker->hwloc_cpu_set,
				combined_worker->hwloc_cpu_set,
				config->workers[id].initial_hwloc_cpu_set);
#endif
#endif
	}

	return new_workerid;
}

int starpu_combined_worker_get_description(int workerid, int *worker_size, int **combined_workerid)
{
	/* Check that this is the id of a combined worker */
	struct _starpu_combined_worker *worker;
	worker = _starpu_get_combined_worker_struct(workerid);
	STARPU_ASSERT(worker);

	if (worker_size)
		*worker_size = worker->worker_size;

	if (combined_workerid)
		*combined_workerid = worker->combined_workerid;

	return 0;
}
