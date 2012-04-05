/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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

/*
 * This test stress the memory allocation system and should force StarPU to
 * reclaim memory from time to time.
 */

#include <assert.h>
#include <starpu.h>
#include <common/config.h>
#ifdef STARPU_HAVE_HWLOC
#include <hwloc.h>
#endif
#include "../helper.h"

#ifdef STARPU_SLOW_MACHINE
#  define BLOCK_SIZE (64*1024)
static unsigned ntasks = 250;
#else
#  define BLOCK_SIZE (64*1024*1024)
static unsigned ntasks = 1000;
#endif


#ifdef STARPU_HAVE_HWLOC
static uint64_t get_total_memory_size(void)
{
	hwloc_topology_t hwtopology;
	hwloc_topology_init(&hwtopology);
	hwloc_topology_load(hwtopology);
	hwloc_obj_t root = hwloc_get_root_obj(hwtopology);

	return root->memory.total_memory;
}
#endif

static void dummy_func(void *descr[], __attribute__ ((unused)) void *_args)
{
}

static struct starpu_codelet dummy_cl =
{
	.cpu_funcs = {dummy_func, NULL},
	.cuda_funcs = {dummy_func, NULL},
	.opencl_funcs = {dummy_func, NULL},
	.nbuffers = 3,
	.modes = {STARPU_RW, STARPU_R, STARPU_R}
};

/* Number of chunks */
static int mb = 16;

int main(int argc, char **argv)
{
	int i, ret;
	int taskid;

#ifdef STARPU_HAVE_HWLOC
	/* We allocate 50% of the memory */
	uint64_t total_size = get_total_memory_size();

	/* On x86_64-freebsd8.2, hwloc 1.3 returns 0 as the total memory
	 * size, so sanity-check what we have.  */
	if (total_size > 0)
		mb = (int)((0.50 * total_size)/(BLOCK_SIZE));
#endif

	/* An optional argument indicates the number of MB to allocate */
	if (argc > 1)
		mb = atoi(argv[1]);

	if (2*mb > ntasks)
		ntasks = 2*mb;

#ifdef STARPU_SLOW_MACHINE
	mb /= 100;
	if (mb == 0)
		mb = 1;
	ntasks /= 100;
#endif

	FPRINTF(stderr, "Allocate %d buffers and create %u tasks\n", mb, ntasks);

        ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	float **host_ptr_array;
	starpu_data_handle_t *handle_array;

	host_ptr_array = (float **) calloc(mb, sizeof(float *));
	handle_array = (starpu_data_handle_t *) calloc(mb, sizeof(starpu_data_handle_t));

	/* Register mb buffers of 1MB */
	for (i = 0; i < mb; i++)
	{
		host_ptr_array[i] = (float *) malloc(BLOCK_SIZE);
		if (host_ptr_array[i] == NULL)
		{
			mb = i;
			FPRINTF(stderr, "Cannot allocate more than %d buffers\n", mb);
			break;
		}
		starpu_variable_data_register(&handle_array[i], 0, (uintptr_t)host_ptr_array[i], BLOCK_SIZE);
		STARPU_ASSERT(handle_array[i]);
	}

	for (taskid = 0; taskid < ntasks; taskid++)
	{
		struct starpu_task *task = starpu_task_create();
		task->cl = &dummy_cl;
		task->handles[0] = handle_array[taskid%mb];
		task->handles[1] = handle_array[(taskid+1)%mb];
		task->handles[2] = handle_array[(taskid+2)%mb];

		ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	for (i = 0; i < mb; i++)
	{
		starpu_data_unregister(handle_array[i]);
		free(host_ptr_array[i]);
	}

	free(host_ptr_array);
	free(handle_array);

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
