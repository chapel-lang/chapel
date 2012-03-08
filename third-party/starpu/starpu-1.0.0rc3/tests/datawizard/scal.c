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

#include <config.h>
#include <starpu.h>
#include <starpu_opencl.h>
#include "scal.h"
#include "helper.h"

void scal_func_cpu(void *buffers[], void *cl_arg)
{
	STARPU_SKIP_IF_VALGRIND;

	unsigned i;

	struct starpu_vector_interface *vector = (struct starpu_vector_interface *) buffers[0];
	unsigned *val = (unsigned *) STARPU_VECTOR_GET_PTR(vector);
	unsigned n = STARPU_VECTOR_GET_NX(vector);

	/* scale the vector */
	for (i = 0; i < n; i++)
		val[i] *= 2;
}

#ifdef STARPU_USE_OPENCL
struct starpu_opencl_program opencl_program;

void scal_func_opencl(void *buffers[], void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	int id, devid;
        cl_int err;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_event event;

	unsigned n = STARPU_VECTOR_GET_NX(buffers[0]);
	cl_mem val = (cl_mem)STARPU_VECTOR_GET_DEV_HANDLE(buffers[0]);
	unsigned offset = STARPU_VECTOR_GET_OFFSET(buffers[0]);

	id = starpu_worker_get_id();
	devid = starpu_worker_get_devid(id);

	err = starpu_opencl_load_kernel(&kernel, &queue, &opencl_program, "vector_mult_opencl", devid);
	if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

	err = clSetKernelArg(kernel, 0, sizeof(val), &val);
	err |= clSetKernelArg(kernel, 1, sizeof(offset), &offset);
	err |= clSetKernelArg(kernel, 2, sizeof(n), &n);
	if (err) STARPU_OPENCL_REPORT_ERROR(err);

	{
		size_t global=n;
		size_t local;
                size_t s;
                cl_device_id device;

                starpu_opencl_get_device(devid, &device);

                err = clGetKernelWorkGroupInfo (kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, &s);
                if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
                if (local > global) local=global;

		err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, &local, 0, NULL, &event);
		if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
	}

	clFinish(queue);
	starpu_opencl_collect_stats(event);
	clReleaseEvent(event);

	starpu_opencl_release_kernel(kernel);
}
#endif

struct starpu_codelet scal_codelet =
{
        .where = STARPU_CPU
#ifdef STARPU_USE_CUDA
		| STARPU_CUDA
#endif
#ifdef STARPU_USE_OPENCL
		| STARPU_OPENCL
#endif
		,
	.cpu_funcs = { scal_func_cpu, NULL },
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = { scal_func_opencl, NULL },
#endif
#ifdef STARPU_USE_CUDA
	.cuda_funcs = { scal_func_cuda, NULL },
#endif
	.modes = { STARPU_RW },
        .model = NULL,
        .nbuffers = 1
};

