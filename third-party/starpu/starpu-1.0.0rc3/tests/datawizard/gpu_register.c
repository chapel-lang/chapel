/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
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
#include <starpu_opencl.h>
#include <starpu_cuda.h>
#include "../helper.h"
#include "scal.h"

int main(int argc, char **argv)
{
	int ret;
#ifdef STARPU_USE_CUDA
#if CUDART_VERSION >= 4000
	unsigned *foo_gpu;
	unsigned *foo;
	starpu_data_handle_t handle;
	int n, i, size, pieces;
	int devid;
	unsigned workerid;
	int chosen = -1;
	cudaError_t cures;
#endif
#endif
	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_USE_CUDA
#if CUDART_VERSION >= 4000 /* We need thread-safety of CUDA */
	/* TODO OpenCL, too */
	for (workerid = 0; workerid < starpu_worker_get_count(); workerid++) {
		if (starpu_worker_get_type(workerid) == STARPU_CUDA_WORKER) {
			chosen = workerid;
			break;
		}
	}

	if (chosen == -1)
		return STARPU_TEST_SKIPPED;

#ifdef STARPU_USE_OPENCL
	ret = starpu_opencl_load_opencl_from_file("tests/datawizard/scal_opencl.cl", &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif

	n = starpu_worker_get_count();
	size = 10 * n;

	devid = starpu_worker_get_devid(chosen);
	cudaSetDevice(devid);
	cudaMalloc((void**)&foo_gpu, size * sizeof(*foo_gpu));

	foo = calloc(size, sizeof(*foo));
	for (i = 0; i < size; i++)
		foo[i] = i;

	cures = cudaMemcpy(foo_gpu, foo, size * sizeof(*foo_gpu), cudaMemcpyHostToDevice);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	starpu_vector_data_register(&handle, starpu_worker_get_memory_node(chosen), (uintptr_t)foo_gpu, size, sizeof(*foo_gpu));

	/* Broadcast the data to force in-place partitioning */
	for (i = 0; i < n; i++)
		starpu_data_prefetch_on_node(handle, starpu_worker_get_memory_node(i), 0);

	/* Even with just one worker, split in at least two */
	if (n == 1)
		pieces = 2;
	else
		pieces = n;

	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = pieces,
	};

	starpu_data_partition(handle, &f);

	for (i = 0; i < pieces; i++) {
		struct starpu_task *task = starpu_task_create();

		task->handles[0] = starpu_data_get_sub_data(handle, 1, i);
		task->cl = &scal_codelet;
		task->execute_on_a_specific_worker = 1;
		task->workerid = i%n;

		ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	starpu_data_unpartition(handle, starpu_worker_get_memory_node(chosen));
	starpu_data_unregister(handle);

	cudaSetDevice(devid);
	cures = cudaMemcpy(foo, foo_gpu, size * sizeof(*foo_gpu), cudaMemcpyDeviceToHost);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	starpu_shutdown();

	for (i = 0; i < size; i++) {
		if (foo[i] != i*2) {
			fprintf(stderr,"value %d is %d instead of %d\n", i, foo[i], 2*i);
			return EXIT_FAILURE;
		}
	}

        return EXIT_SUCCESS;

enodev:
	starpu_data_unregister(handle);
#endif
#endif
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
