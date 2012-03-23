/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
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
#include "complex_interface.h"

extern struct starpu_opencl_program opencl_program;

void copy_complex_codelet_opencl(void *buffers[], void *_args)
{
	(void) _args;

	int id, devid;
        cl_int err;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_event event;

	/* length of the vector */
	unsigned n = STARPU_COMPLEX_GET_NX(buffers[0]);
	/* OpenCL copy of the vector pointer */
	cl_mem i_real      = (cl_mem) STARPU_COMPLEX_GET_REAL(buffers[0]);
	cl_mem i_imaginary = (cl_mem) STARPU_COMPLEX_GET_IMAGINARY(buffers[0]);
	cl_mem o_real      = (cl_mem) STARPU_COMPLEX_GET_REAL(buffers[1]);
	cl_mem o_imaginary = (cl_mem) STARPU_COMPLEX_GET_IMAGINARY(buffers[1]);

	id = starpu_worker_get_id();
	devid = starpu_worker_get_devid(id);

	err = starpu_opencl_load_kernel(&kernel, &queue, &opencl_program, "complex_copy_opencl", devid);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	err = clSetKernelArg(kernel, 0, sizeof(o_real), &o_real);
	err|= clSetKernelArg(kernel, 1, sizeof(o_imaginary), &o_imaginary);
	err|= clSetKernelArg(kernel, 2, sizeof(i_real), &i_real);
	err|= clSetKernelArg(kernel, 3, sizeof(i_imaginary), &i_imaginary);
	err|= clSetKernelArg(kernel, 4, sizeof(n), &n);
	if (err)
		STARPU_OPENCL_REPORT_ERROR(err);

	{
		size_t global=n;
		size_t local;
                size_t s;
                cl_device_id device;

                starpu_opencl_get_device(devid, &device);

                err = clGetKernelWorkGroupInfo (kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, &s);
                if (err != CL_SUCCESS)
			STARPU_OPENCL_REPORT_ERROR(err);
                if (local > global)
			local=global;

		err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, &local, 0, NULL, &event);
		if (err != CL_SUCCESS)
			STARPU_OPENCL_REPORT_ERROR(err);
	}

	clFinish(queue);
	starpu_opencl_collect_stats(event);
	clReleaseEvent(event);

	starpu_opencl_release_kernel(kernel);
}
