/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <starpu.h>
#include <stdlib.h>
#include <debug/starpu_debug_helpers.h>
#include "../helper.h"

static size_t vector_size = 1;

static int niter = 1000;
//static unsigned cnt;

//static unsigned finished = 0;

starpu_data_handle_t v_handle;
static unsigned *v;

static char worker_0_name[128];
static char worker_1_name[128];
static uint32_t memory_node_0;
static uint32_t memory_node_1;

struct timeval start;
struct timeval end;

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* Create a piece of data */
	ret = starpu_malloc((void **)&v, vector_size);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_malloc");
	starpu_vector_data_register(&v_handle, 0, (uintptr_t)v, vector_size, 1);

	/* Find a pair of memory nodes */
	if (starpu_cuda_worker_get_count() > 1)
	{
		/* Take the two devices that come first */
		int nworkers = (int)starpu_worker_get_count();

		unsigned found_node_0 = 0;

		int w;
		for (w = 0; w < nworkers; w++)
		{
			if (starpu_worker_get_type(w) == STARPU_CUDA_WORKER)
			{
				if (!found_node_0)
				{
					memory_node_0 = starpu_worker_get_memory_node(w);
					starpu_worker_get_name(w, worker_0_name, 128);
					found_node_0 = 1;
				}
				else
				{
					memory_node_1 = starpu_worker_get_memory_node(w);
					starpu_worker_get_name(w, worker_1_name, 128);
					break;
				}
			}
		}

		fprintf(stderr, "Ping-pong will be done between %s (node %u) and %s (node %u)\n",
					worker_0_name, memory_node_0, worker_1_name, memory_node_1);
	}

	//	unsigned iter;

	/* warm up */
	//	unsigned nwarmupiter = 128;
	_starpu_benchmark_ping_pong(v_handle, memory_node_0, memory_node_1, 128);

	gettimeofday(&start, NULL);

	_starpu_benchmark_ping_pong(v_handle, memory_node_0, memory_node_1, niter);

	gettimeofday(&end, NULL);

	double timing = (double)((end.tv_sec - start.tv_sec)*1000000 +
					(end.tv_usec - start.tv_usec));

	fprintf(stderr, "Took %f ms\n", timing/1000);
	fprintf(stderr, "Avg. transfer time : %f us\n", timing/(2*niter));

	starpu_data_unregister(v_handle);
	starpu_free(v);
	starpu_shutdown();

	return EXIT_SUCCESS;
}
