/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#include <starpu.h>
#include <pthread.h>
#include <sys/time.h>

static unsigned niter = 50000;
#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

#ifdef STARPU_USE_CUDA
extern void cuda_codelet(void *descr[], __attribute__ ((unused)) void *_args);
#endif

#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
extern void opencl_codelet(void *descr[], __attribute__ ((unused)) void *_args);
struct starpu_opencl_program opencl_program;
#endif

void cpu_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	float *val = (float *)STARPU_VECTOR_GET_PTR(descr[0]);

	val[0] += 1.0f; val[1] += 1.0f;
}

int main(int argc, char **argv)
{
	int ret = 0;

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_SLOW_MACHINE
	niter /= 100;
#endif
	if (argc == 2)
		niter = atoi(argv[1]);

	float float_array[4] __attribute__ ((aligned (16))) = { 0.0f, 0.0f, 0.0f, 0.0f};

	starpu_data_handle_t float_array_handle;
	starpu_vector_data_register(&float_array_handle, 0 /* home node */,
			(uintptr_t)&float_array, 4, sizeof(float));

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_load_opencl_from_file("examples/incrementer/incrementer_kernels_opencl_kernel.cl", &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif

	struct starpu_codelet cl =
	{
		.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
		.cpu_funcs = {cpu_codelet, NULL},
#ifdef STARPU_USE_CUDA
		.cuda_funcs = {cuda_codelet, NULL},
#endif
#ifdef STARPU_USE_OPENCL
		.opencl_funcs = {opencl_codelet, NULL},
#endif
		.nbuffers = 1,
		.modes = {STARPU_RW}
	};

	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);

	unsigned i;
	for (i = 0; i < niter; i++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = &cl;

		task->callback_func = NULL;

		task->handles[0] = float_array_handle;

		int ret = starpu_task_submit(task);
		if (STARPU_UNLIKELY(ret == -ENODEV))
		{
			FPRINTF(stderr, "No worker may execute this task\n");
			exit(0);
		}
	}

	starpu_task_wait_for_all();

	/* update the array in RAM */
	starpu_data_unregister(float_array_handle);

	gettimeofday(&end, NULL);

	FPRINTF(stderr, "array -> %f, %f, %f, %f\n", float_array[0],
                float_array[1], float_array[2], float_array[3]);

	if (float_array[0] != niter || float_array[0] != float_array[1] + float_array[2] + float_array[3])
	{
		FPRINTF(stderr, "Incorrect result\n");
		ret = 1;
	}

	double timing = (double)((end.tv_sec - start.tv_sec)*1000000 +
					(end.tv_usec - start.tv_usec));

	FPRINTF(stderr, "%u elems took %f ms\n", niter, timing/1000);

	starpu_shutdown();

	return ret;
}
