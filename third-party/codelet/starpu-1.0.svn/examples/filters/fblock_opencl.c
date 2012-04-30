/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

#define CHECK_CL_SET_KERNEL_ARG(kernel, n, size, ptr)       \
do                                                          \
{							    \
	int err;                                            \
	err = clSetKernelArg(kernel, n, size, ptr);         \
	if (err != CL_SUCCESS)                              \
       		STARPU_OPENCL_REPORT_ERROR(err);            \
} while (0)

extern struct starpu_opencl_program opencl_program;

void opencl_func(void *buffers[], void *cl_arg)
{
	int id, devid, err;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_event event;

        int *factor = cl_arg;
	cl_mem block = (cl_mem)STARPU_BLOCK_GET_DEV_HANDLE(buffers[0]);
	unsigned offset = STARPU_BLOCK_GET_OFFSET(buffers[0]);
	int nx = (int)STARPU_BLOCK_GET_NX(buffers[0]);
	int ny = (int)STARPU_BLOCK_GET_NY(buffers[0]);
	int nz = (int)STARPU_BLOCK_GET_NZ(buffers[0]);
        unsigned ldy = STARPU_BLOCK_GET_LDY(buffers[0]);
        unsigned ldz = STARPU_BLOCK_GET_LDZ(buffers[0]);

	id = starpu_worker_get_id();
	devid = starpu_worker_get_devid(id);

	err = starpu_opencl_load_kernel(&kernel, &queue, &opencl_program, "fblock_opencl", devid);
	if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

	CHECK_CL_SET_KERNEL_ARG(kernel, 0, sizeof(block), &block);
	CHECK_CL_SET_KERNEL_ARG(kernel, 1, sizeof(offset), &offset);
	CHECK_CL_SET_KERNEL_ARG(kernel, 2, sizeof(nx), &nx);
	CHECK_CL_SET_KERNEL_ARG(kernel, 3, sizeof(ny), &ny);
	CHECK_CL_SET_KERNEL_ARG(kernel, 4, sizeof(nz), &nz);
	CHECK_CL_SET_KERNEL_ARG(kernel, 5, sizeof(ldy), &ldy);
	CHECK_CL_SET_KERNEL_ARG(kernel, 6, sizeof(ldz), &ldz);
	CHECK_CL_SET_KERNEL_ARG(kernel, 7, sizeof(*factor), factor);

	{
		size_t global=nx*ny*nz;
		err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, NULL, 0, NULL, &event);
		if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
	}

	clFinish(queue);
	starpu_opencl_collect_stats(event);
	clReleaseEvent(event);

	starpu_opencl_release_kernel(kernel);
}

