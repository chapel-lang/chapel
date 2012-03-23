/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

#ifdef STARPU_USE_CUDA
extern "C" void cuda_codelet(void *descr[], __attribute__ ((unused)) void *_args);
#endif

#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
extern "C" void opencl_codelet(void *descr[], __attribute__ ((unused)) void *_args);
struct starpu_opencl_program opencl_program;
#endif

void cpu_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	float *val = (float *)STARPU_VECTOR_GET_PTR(descr[0]);

	val[0] += 1.0f;
	val[1] += 1.0f;
}

int main(int argc, char **argv)
{
	int ret = 0;
	starpu_data_handle_t float_array_handle;
	float float_array[4] __attribute__ ((aligned (16))) = { 0.0f, 0.0f, 0.0f, 0.0f};
        struct starpu_codelet cl;
	unsigned i;
	unsigned niter = 50;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_vector_data_register(&float_array_handle, 0, (uintptr_t)&float_array, 4, sizeof(float));

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_load_opencl_from_file("examples/incrementer/incrementer_kernels_opencl_kernel.cl", &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif

        starpu_codelet_init(&cl);
        cl.cpu_funcs[0] = cpu_codelet;
#ifdef STARPU_USE_CUDA
        cl.cuda_funcs[0] = cuda_codelet;
#endif
#ifdef STARPU_USE_OPENCL
	cl.opencl_funcs[0] = opencl_codelet;
#endif
        cl.nbuffers = 1;
        cl.modes[0] = STARPU_RW;

	for (i = 0; i < niter; i++)
	{
		ret = starpu_insert_task(&cl,
					 STARPU_RW, float_array_handle,
					 0);
                if (STARPU_UNLIKELY(ret == -ENODEV))
                {
			FPRINTF(stderr, "No worker may execute this task\n");
			exit(77);
                }
        }

	starpu_task_wait_for_all();

	/* update the array in RAM */
	starpu_data_unregister(float_array_handle);

	FPRINTF(stderr, "array -> %f, %f, %f, %f\n", float_array[0],
                float_array[1], float_array[2], float_array[3]);

	if (float_array[0] != niter || float_array[0] != float_array[1] + float_array[2] + float_array[3])
	{
		FPRINTF(stderr, "Incorrect result\n");
		ret = 1;
	}

#ifdef STARPU_USE_OPENCL
	ret = starpu_opencl_unload_opencl(&opencl_program);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_unload_opencl");
#endif
	starpu_shutdown();

	return ret;
}
