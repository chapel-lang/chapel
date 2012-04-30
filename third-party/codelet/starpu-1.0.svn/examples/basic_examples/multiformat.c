/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011-2012 Institut National de Recherche en Informatique et Automatique
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
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif
#include "multiformat_types.h"

static int ncpu = 0;
#ifdef STARPU_USE_CUDA
static int ncuda = 0;
#endif
#ifdef STARPU_USE_OPENCL
static int nopencl = 0;
#endif

static struct point array_of_structs[N_ELEMENTS];
static starpu_data_handle_t array_of_structs_handle;

static void
multiformat_scal_cpu_func(void *buffers[], void *args)
{
	struct point *aos;
	unsigned int n, i;

	aos = (struct point *) STARPU_MULTIFORMAT_GET_CPU_PTR(buffers[0]);
	n = STARPU_MULTIFORMAT_GET_NX(buffers[0]);

	for (i = 0; i < n; i++)
	{
		aos[i].x *= aos[i].y;
	}
}

#ifdef STARPU_USE_CUDA
extern struct starpu_codelet cpu_to_cuda_cl;
extern struct starpu_codelet cuda_to_cpu_cl;
#endif

#ifdef STARPU_USE_OPENCL
extern struct starpu_codelet cpu_to_opencl_cl;
extern struct starpu_codelet opencl_to_cpu_cl;
#endif

static struct starpu_multiformat_data_interface_ops format_ops =
{
#ifdef STARPU_USE_CUDA
	.cuda_elemsize = 2* sizeof(float),
	.cpu_to_cuda_cl = &cpu_to_cuda_cl,
	.cuda_to_cpu_cl = &cuda_to_cpu_cl,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_elemsize = 2 * sizeof(float),
	.cpu_to_opencl_cl = &cpu_to_opencl_cl,
	.opencl_to_cpu_cl = &opencl_to_cpu_cl,
#endif
	.cpu_elemsize = sizeof(struct point),

};

#ifdef STARPU_USE_CUDA
extern void multiformat_scal_cuda_func(void *buffers[], void *arg);
#endif
#ifdef STARPU_USE_OPENCL
extern void multiformat_scal_opencl_func(void *buffers[], void *arg);
#endif

#ifdef STARPU_USE_CPU
static struct starpu_codelet cpu_cl =
{
	.where = STARPU_CPU,
	.cpu_funcs = {multiformat_scal_cpu_func, NULL},
	.nbuffers = 1,
	.modes = { STARPU_RW },
	.name = "codelet_real"
};
#endif /* !STARPU_USE_CPU */

#ifdef STARPU_USE_CUDA
static struct starpu_codelet cuda_cl =
{
	.where = STARPU_CUDA,
	.cuda_funcs = { multiformat_scal_cuda_func, NULL },
	.nbuffers = 1,
	.modes = { STARPU_RW },
	.name = "cuda_codelet"
};
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static struct starpu_codelet opencl_cl =
{
	.where = STARPU_OPENCL,
	.opencl_funcs = { multiformat_scal_opencl_func, NULL },
	.nbuffers = 1,
	.modes = { STARPU_RW },
	.name = "opencl_codelet"
};
#endif /* !STARPU_USE_OPENCL */

/*
 * Main functions 
 */
static void
init_problem_data(void)
{
	int i; 
	for (i = 0; i < N_ELEMENTS; i++)
	{
		array_of_structs[i].x = 1.0 + i;
		array_of_structs[i].y = 42.0;
	}
}

static void
register_data(void)
{
	starpu_multiformat_data_register(&array_of_structs_handle,
					 0,
					 &array_of_structs,
					 N_ELEMENTS,
					 &format_ops);
}

static int
create_and_submit_task(unsigned int dev)
{
	struct starpu_task *task = starpu_task_create();
	switch (dev)
	{
#ifdef STARPU_USE_CPU
		case STARPU_CPU:
			task->cl = &cpu_cl;
			break;
#endif
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA:
			task->cl = &cuda_cl;
			break;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL:
			task->cl = &opencl_cl;
			break;
#endif
		default:
			assert(0);
	}
	task->synchronous = 1;
	task->handles[0] = array_of_structs_handle;
	task->cl_arg = NULL;
	task->cl_arg_size = 0;
	return starpu_task_submit(task);
}

static void
create_and_submit_tasks(void)
{
	int err;

#ifdef STARPU_USE_CUDA
	if (ncuda > 0)
	{
		err = create_and_submit_task(STARPU_CUDA);
		if (err != 0)
		{
			FPRINTF(stderr, "Cuda : %s\n", strerror(-err));
			return;
		}
	}
#endif

#ifdef STARPU_USE_CPU
	if (ncpu > 0)
	{
		err = create_and_submit_task(STARPU_CPU);
		if (err != 0)
		{
			FPRINTF(stderr, "CPU : %s\n", strerror(-err));
			return;
		}
	}
#endif

#ifdef STARPU_USE_OPENCL
	if (nopencl > 0)
	{
		err = create_and_submit_task(STARPU_OPENCL);
		if (err != 0)
		{
			FPRINTF(stderr, "OpenCL : %s\n", strerror(-err));
			return;
		}
	}
#endif /* !STARPU_USE_OPENCL */
}

static void
unregister_data(void)
{
	starpu_data_unregister(array_of_structs_handle);
}

static void
print_it(void)
{
	int i;
	for (i = 0; i < N_ELEMENTS; i++)
	{
		FPRINTF(stderr, "(%.2f %.2f) ",
			array_of_structs[i].x,
			array_of_structs[i].y);
	}
	FPRINTF(stderr, "\n");
}

static int
check_it(void)
{
	int i;
	for (i = 0; i < N_ELEMENTS; i++)
	{
		float expected_value = i + 1.0;
#ifdef STARPU_USE_CUDA
		if (ncuda > 0)
			expected_value *= array_of_structs[i].y;
#endif
#ifdef STARPU_USE_OPENCL
		if (nopencl > 0)
			expected_value *= array_of_structs[i].y;
#endif
		expected_value *= array_of_structs[i].y;
		if (array_of_structs[i].x != expected_value)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#ifdef STARPU_USE_OPENCL
struct starpu_opencl_program opencl_program;
struct starpu_opencl_program opencl_conversion_program;
#endif

static int
gpus_available()
{
#ifdef STARPU_USE_CUDA
	if (ncuda > 0)
		return 1;
#endif
#ifdef STARPU_USE_OPENCL
	if (nopencl > 0)
		return 1;
#endif

	return 0;
}

int
main(void)
{
#ifdef STARPU_USE_CPU
	int ret;

	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	ncpu = starpu_cpu_worker_get_count();
#ifdef STARPU_USE_CUDA
	ncuda = starpu_cuda_worker_get_count();
#endif
#ifdef STARPU_USE_OPENCL
	nopencl = starpu_opencl_worker_get_count();
#endif

	if (ncpu == 0 || !gpus_available())
	{
		starpu_shutdown();
		return 77;
	}

#ifdef STARPU_USE_OPENCL
	ret = starpu_opencl_load_opencl_from_file("examples/basic_examples/multiformat_opencl_kernel.cl",
						  &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
	ret = starpu_opencl_load_opencl_from_file("examples/basic_examples/multiformat_conversion_codelets_opencl_kernel.cl", 
						  &opencl_conversion_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif
	init_problem_data();

	print_it();

	register_data();

	create_and_submit_tasks();

	unregister_data();

	print_it();

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_unload_opencl(&opencl_program);
        STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_unload_opencl");
        starpu_opencl_unload_opencl(&opencl_conversion_program);
#endif
	starpu_shutdown();


	return check_it();
#else
	/* Without the CPU, there is no point in using the multiformat
	 * interface, so this test is pointless. */
	return 77;
#endif
}
