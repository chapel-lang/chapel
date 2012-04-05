/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
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

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

static unsigned niter = 50000;

extern void cpu_codelet(void *descr[], __attribute__ ((unused)) void *_args);

#ifdef STARPU_USE_CUDA
extern void cuda_codelet(void *descr[], __attribute__ ((unused)) void *_args);
#endif

#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
extern void opencl_codelet(void *descr[], __attribute__ ((unused)) void *_args);
struct starpu_opencl_program opencl_program;
#endif

int main(int argc, char **argv)
{
	unsigned i;
        float foo;
	starpu_data_handle_t float_array_handle;
	struct starpu_codelet cl = {};
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_SLOW_MACHINE
	niter /= 100;
#endif
        if (argc == 2) niter = atoi(argv[1]);
        foo = 0.0f;

	starpu_variable_data_register(&float_array_handle, 0 /* home node */,
                                      (uintptr_t)&foo, sizeof(float));

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_load_opencl_from_file("examples/basic_examples/variable_kernels_opencl_kernel.cl", &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif

	cl.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL;
        cl.cpu_funcs[0] = cpu_codelet;
#ifdef STARPU_USE_CUDA
        cl.cuda_funcs[0] = cuda_codelet;
#endif
#ifdef STARPU_USE_OPENCL
        cl.opencl_funcs[0] = opencl_codelet;
#endif
        cl.nbuffers = 1;
	cl.modes[0] = STARPU_RW;
        cl.model = NULL;

	for (i = 0; i < niter; i++)
	{
		struct starpu_task *task = starpu_task_create();
                int ret;

		task->cl = &cl;

		task->callback_func = NULL;

		task->handles[0] = float_array_handle;

		ret = starpu_task_submit(task);
		if (STARPU_UNLIKELY(ret == -ENODEV))
		{
			FPRINTF(stderr, "No worker may execute this task\n");
			exit(0);
		}
	}

	starpu_task_wait_for_all();

	/* update the array in RAM */
	starpu_data_unregister(float_array_handle);

	FPRINTF(stderr, "variable -> %f\n", foo);

	starpu_shutdown();

	return 0;

enodev:
	starpu_shutdown();
	return 77;
}
