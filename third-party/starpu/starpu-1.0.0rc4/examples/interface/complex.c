/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
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

#include <starpu.h>
#include "complex_interface.h"
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif

#ifdef STARPU_USE_CUDA
extern void copy_complex_codelet_cuda(void *descr[], __attribute__ ((unused)) void *_args);
#endif
#ifdef STARPU_USE_OPENCL
extern void copy_complex_codelet_opencl(void *buffers[], void *args);
#endif

void compare_complex_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	int nx1 = STARPU_COMPLEX_GET_NX(descr[0]);
	double *real1 = STARPU_COMPLEX_GET_REAL(descr[0]);
	double *imaginary1 = STARPU_COMPLEX_GET_IMAGINARY(descr[0]);

	int nx2 = STARPU_COMPLEX_GET_NX(descr[1]);
	double *real2 = STARPU_COMPLEX_GET_REAL(descr[1]);
	double *imaginary2 = STARPU_COMPLEX_GET_IMAGINARY(descr[1]);

	int compare = (nx1 == nx2);
	if (nx1 == nx2)
	{
		int i;
		for(i=0 ; i<nx1 ; i++)
		{
			if (real1[i] != real2[i] || imaginary1[i] != imaginary2[i])
			{
				compare = 0;
				break;
			}
		}
	}
	fprintf(stderr, "Complex numbers are%s similar\n", compare==0 ? " NOT" : "");
}

void display_complex_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	int nx = STARPU_COMPLEX_GET_NX(descr[0]);
	double *real = STARPU_COMPLEX_GET_REAL(descr[0]);
	double *imaginary = STARPU_COMPLEX_GET_IMAGINARY(descr[0]);
	int i;

	for(i=0 ; i<nx ; i++)
	{
		fprintf(stderr, "Complex[%d] = %3.2f + %3.2f i\n", i, real[i], imaginary[i]);
	}
}

struct starpu_codelet cl_display =
{
	.cpu_funcs = {display_complex_codelet, NULL},
	.nbuffers = 1,
	.modes = {STARPU_R}
};

struct starpu_codelet cl_copy =
{
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {copy_complex_codelet_cuda, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {copy_complex_codelet_opencl, NULL},
#endif
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_W}
};

struct starpu_codelet cl_compare =
{
	.cpu_funcs = {compare_complex_codelet, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_R}
};

#ifdef STARPU_USE_OPENCL
struct starpu_opencl_program opencl_program;
#endif

int main(int argc, char **argv)
{
	int ret = 0;
	starpu_data_handle_t handle1;
	starpu_data_handle_t handle2;

	double real = 45.0;
	double imaginary = 12.0;
	double copy_real = 78.0;
	double copy_imaginary = 78.0;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_USE_OPENCL
	ret = starpu_opencl_load_opencl_from_file("examples/interface/complex_kernels.cl",
						  &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif
	starpu_complex_data_register(&handle1, 0, &real, &imaginary, 1);
	starpu_complex_data_register(&handle2, 0, &copy_real, &copy_imaginary, 1);

	ret = starpu_insert_task(&cl_display, STARPU_R, handle1, 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	ret = starpu_insert_task(&cl_display, STARPU_R, handle2, 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	ret = starpu_insert_task(&cl_compare,
				 STARPU_R, handle1,
				 STARPU_R, handle2,
				 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	ret = starpu_insert_task(&cl_copy,
				 STARPU_R, handle1,
				 STARPU_W, handle2,
				 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	ret = starpu_insert_task(&cl_display, STARPU_R, handle1, 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	ret = starpu_insert_task(&cl_display, STARPU_R, handle2, 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");


	ret = starpu_insert_task(&cl_compare,
				 STARPU_R, handle1,
				 STARPU_R, handle2,
				 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	starpu_task_wait_for_all();

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_unload_opencl(&opencl_program);
        STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_unload_opencl");
#endif
	starpu_shutdown();
	return 0;

enodev:
#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_unload_opencl(&opencl_program);
        STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_unload_opencl");
#endif
	starpu_data_unregister(handle1);
	starpu_data_unregister(handle2);
	starpu_shutdown();
	return 77;
}
