/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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

#include <config.h>
#include <starpu.h>
#include <starpu_opencl.h>
#include "../../../helper.h"

#define KERNEL_LOCATION "tests/datawizard/interfaces/multiformat/multiformat_conversion_codelets_kernel.cl"
static struct starpu_opencl_program opencl_conversion_program;

void cpu_to_opencl_opencl_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	(void) args;
	int id, devid, ret;
        cl_int err;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_event event;

	unsigned n = STARPU_MULTIFORMAT_GET_NX(buffers[0]);
	cl_mem src = (cl_mem) STARPU_MULTIFORMAT_GET_CPU_PTR(buffers[0]);
	cl_mem dst = (cl_mem) STARPU_MULTIFORMAT_GET_OPENCL_PTR(buffers[0]);

	id = starpu_worker_get_id();
	devid = starpu_worker_get_devid(id);

	ret = starpu_opencl_load_opencl_from_file(KERNEL_LOCATION,
						  &opencl_conversion_program,
						  NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");

	err = starpu_opencl_load_kernel(&kernel,
					&queue,
					&opencl_conversion_program,
					"cpu_to_opencl_opencl",
					devid);

	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	err = clSetKernelArg(kernel, 0, sizeof(src), &src);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	err = clSetKernelArg(kernel, 1, sizeof(dst), &dst);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	err = clSetKernelArg(kernel, 2, sizeof(n), &n);
	if (err != CL_SUCCESS)
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

	clFinish(queue);
	starpu_opencl_collect_stats(event);
	clReleaseEvent(event);

	starpu_opencl_release_kernel(kernel);
        starpu_opencl_unload_opencl(&opencl_conversion_program);
}
