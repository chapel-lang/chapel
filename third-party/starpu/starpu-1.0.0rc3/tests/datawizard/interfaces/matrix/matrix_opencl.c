/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */
#include <config.h>
#include <starpu.h>
#include <starpu_opencl.h>
#include "../test_interfaces.h"

#define KERNEL_LOCATION "tests/datawizard/interfaces/matrix/matrix_opencl_kernel.cl"

extern struct test_config matrix_config;
static struct starpu_opencl_program matrix_program;

void test_matrix_opencl_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	int id, devid, factor, ret;
	unsigned int n;

        cl_int             err;
	cl_kernel          kernel;
	cl_command_queue   queue;
	cl_event           event;
	cl_context         context;
	cl_mem             val, fail;

	ret = starpu_opencl_load_opencl_from_file(KERNEL_LOCATION,
						  &matrix_program,
						  NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");

	factor = *(int *)args;
	n = STARPU_MATRIX_GET_NX(buffers[0]);
	n*= STARPU_MATRIX_GET_NY(buffers[0]);
	val = (cl_mem)STARPU_MATRIX_GET_DEV_HANDLE(buffers[0]);

	id = starpu_worker_get_id();
	devid = starpu_worker_get_devid(id);
	starpu_opencl_get_context(devid, &context);

	err = starpu_opencl_load_kernel(&kernel,
					&queue,
					&matrix_program,
					"matrix_opencl",
					devid);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	fail = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR,
		sizeof(int), &matrix_config.copy_failed, &err);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	/* Setting args */
	int nargs;
	nargs = starpu_opencl_set_kernel_args(&err, &kernel,
					sizeof(val), &val,
					sizeof(n), &n,
					sizeof(fail), &fail,
					sizeof(factor), &factor,
					0);
	if (nargs != 4)
		STARPU_OPENCL_REPORT_ERROR(err);
	{
		size_t global=n;
		size_t local;
                size_t s;
                cl_device_id device;

                starpu_opencl_get_device(devid, &device);

                err = clGetKernelWorkGroupInfo (kernel,
						device,
						CL_KERNEL_WORK_GROUP_SIZE,
						sizeof(local),
						&local,
						&s);
                if (err != CL_SUCCESS)
			STARPU_OPENCL_REPORT_ERROR(err);

                if (local > global)
			local = global;

		err = clEnqueueNDRangeKernel(queue,
					kernel,
					1,
					NULL,
					&global,
					&local,
					0,
					NULL,
					&event);

		if (err != CL_SUCCESS)
			STARPU_OPENCL_REPORT_ERROR(err);
	}

	err = clEnqueueReadBuffer(queue,
				  fail,
				  CL_TRUE,
				  0, 
				  sizeof(int),
				  &matrix_config.copy_failed,
				  0,
				  NULL,
				  NULL);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	clFinish(queue);
	starpu_opencl_collect_stats(event);
	clReleaseEvent(event);

	starpu_opencl_release_kernel(kernel);
        starpu_opencl_unload_opencl(&matrix_program);
}

