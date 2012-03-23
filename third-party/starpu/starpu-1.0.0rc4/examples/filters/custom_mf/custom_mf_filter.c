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
#include "custom_interface.h"
#include "custom_types.h"
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif /* !STARPU_USE_OPENCL */

#define N 12

#define DEBUG 1

#ifdef STARPU_USE_CUDA
static unsigned int ncuda;
#endif
#ifdef STARPU_USE_OPENCL
static unsigned int nopencl;
#endif


static struct point array_of_structs[N];
static starpu_data_handle_t handle;
static unsigned int nchunks = 6;

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
	.cuda_elemsize = sizeof(struct struct_of_arrays),
	.cpu_to_cuda_cl = &cpu_to_cuda_cl,
	.cuda_to_cpu_cl = &cuda_to_cpu_cl,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_elemsize  = sizeof(struct struct_of_arrays),
	.cpu_to_opencl_cl = &cpu_to_opencl_cl,
	.opencl_to_cpu_cl = &opencl_to_cpu_cl,
#endif
	.cpu_elemsize = sizeof(struct point),
};


static void
custom_filter(void *father, void *child, struct starpu_data_filter *f,
		unsigned id, unsigned nchunks)
{
	struct custom_data_interface *custom_father, *custom_child;
	custom_father = (struct custom_data_interface *) father;
	custom_child = (struct custom_data_interface *) child;

	assert(N % nchunks == 0); // XXX 
	ssize_t chunk_size = N/nchunks;

	if (custom_father->cpu_ptr)
	{
		struct point *tmp = (struct point *) custom_father->cpu_ptr;
		tmp += id * chunk_size;
		custom_child->cpu_ptr = tmp;
	}
#ifdef STARPU_USE_CUDA
	else if (custom_father->cuda_ptr)
	{
		struct struct_of_arrays *soa_father, *soa_child;
		soa_father = (struct struct_of_arrays*) custom_father->cuda_ptr;
		soa_child = (struct struct_of_arrays*) custom_child->cuda_ptr;
		soa_child->x = soa_father->x + chunk_size;
		soa_child->y = soa_father->y + chunk_size;
	}
#endif
#ifdef STARPU_USE_OPENCL
	else if (custom_father->opencl_ptr)
	{
		struct struct_of_arrays *soa_father, *soa_child;
		soa_father = (struct struct_of_arrays*) custom_father->opencl_ptr;
		soa_child = (struct struct_of_arrays*) custom_child->opencl_ptr;
		soa_child->x = soa_father->x + chunk_size;
		soa_child->y = soa_father->y + chunk_size;
	}
#endif /* !STARPU_USE_OPENCL */

	custom_child->ops = custom_father->ops;
	custom_child->nx = chunk_size;
}

static void
register_and_partition_data(void)
{
	int i;
	for (i = 0; i < N; i++)
	{
		array_of_structs[i].x = i+1.0;
		array_of_structs[i].y = 42.0;
	}
	custom_data_register(&handle, 0, &array_of_structs, N, &format_ops);

	struct starpu_data_filter f =
	{
		.filter_func   = custom_filter,
		.nchildren     = nchunks,
		.get_nchildren = NULL,
		.get_child_ops = NULL
	};
	starpu_data_partition(handle, &f);
}

static void
unpartition_and_unregister_data(void)
{
	starpu_data_unpartition(handle, 0);
	starpu_data_unregister(handle);
}

static void
custom_scal_cpu_func(void *buffers[], void *args)
{
	struct point *aos;
	unsigned int n, i;

	aos = CUSTOM_GET_CPU_PTR(buffers[0]);
	n = CUSTOM_GET_NX(buffers[0]);

	for (i = 0; i < n; i++)
		aos[i].x *= aos[i].y;
}

#ifdef STARPU_USE_CUDA
extern void custom_scal_cuda_func(void *buffers[], void *args);
#endif

static struct starpu_codelet cpu_cl =
{
	.where = STARPU_CPU,
	.cpu_funcs = { custom_scal_cpu_func, NULL},
	.nbuffers = 1,
	.modes = { STARPU_RW },
	.name = "codelet_real"
};

#ifdef STARPU_USE_CUDA
static struct starpu_codelet cuda_cl =
{
	.where = STARPU_CUDA,
	.cuda_funcs = { custom_scal_cuda_func, NULL },
	.nbuffers = 1,
	.modes = { STARPU_RW },
	.name = "cuda_codelet"
};
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
extern void custom_scal_opencl_func(void *buffers[], void *args);

static struct starpu_codelet opencl_cl =
{
	.where = STARPU_OPENCL,
	.opencl_funcs = { custom_scal_opencl_func, NULL },
	.nbuffers = 1,
	.modes = { STARPU_RW },
	.name = "opencl_codelet"
};
#endif /* !STARPU_USE_OPENCL */

static int
create_and_submit_tasks(void)
{
	int err;
	unsigned int i;
	for (i = 0; i < nchunks; i++)
	{
		struct starpu_task *task = starpu_task_create();
		switch (i%3)
		{
		case 0:
			task->cl = &cpu_cl;
			break;
		case 1:
#ifdef STARPU_USE_CUDA
			if (ncuda > 0)
				task->cl = &cuda_cl;
			else
#endif
				task->cl = &cpu_cl;
			break;
		case 2:
#ifdef STARPU_USE_OPENCL
			if (nopencl > 0)
				task->cl = &opencl_cl;
			else
#endif
				task->cl = &cpu_cl;
			break;
		default:
			/* We should never get here */
			assert(0);
		}

		task->handles[0] = starpu_data_get_sub_data(handle, 1, i);
		err = starpu_task_submit(task);
		if (err != 0)
			return err;
	}


	err = starpu_task_wait_for_all();
	if (err != 0)
		return err;

	return 0;
}

#if DEBUG
static void
print_it(void)
{
	int i;
	for (i = 0; i < N; i++)
	{
		FPRINTF(stderr, "(%.2f, %.2f) ",
			array_of_structs[i].x,
			array_of_structs[i].y);
	}
	FPRINTF(stderr, "\n");
}
#endif

static int
check_it(void)
{
	int i;
	for (i = 0; i < N; i++)
	{
		float expected_value = (i + 1.0)*42.0;
		if (array_of_structs[i].x != expected_value)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#ifdef STARPU_USE_OPENCL
struct starpu_opencl_program opencl_program;
struct starpu_opencl_program opencl_conversion_program;
#endif /* !STARPU_USE_OPENCL */

int
main(void)
{
#ifndef STARPU_USE_CPU
	return 77;
#else
	int err;

	err = starpu_init(NULL);
	if (err == -ENODEV)
		goto enodev;

#ifdef STARPU_USE_CUDA
	ncuda = starpu_cuda_worker_get_count();
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	nopencl = starpu_opencl_worker_get_count();
	if (nopencl > 0)
	{
		char *f1 = "examples/filters/custom_mf/custom_opencl.cl";
		char *f2 = "examples/filters/custom_mf/conversion_opencl.cl";
		err = starpu_opencl_load_opencl_from_file(f1, &opencl_program,
							  NULL);
		assert(err == 0);
		err = starpu_opencl_load_opencl_from_file(f2,
						&opencl_conversion_program,
						NULL);
		assert(err == 0);
	}
#endif /* !STARPU_USE_OPENCL */

	register_and_partition_data();
#if DEBUG
	print_it();
#endif
	err = create_and_submit_tasks();
	if (err != 0)
	{
		FPRINTF(stderr, "create_submit_task : %s\n",
			strerror(-err));
		return EXIT_FAILURE;
	}
	unpartition_and_unregister_data();
#if DEBUG
	print_it();
#endif

#ifdef STARPU_USE_OPENCL
	if (nopencl > 0)
	{
        	err = starpu_opencl_unload_opencl(&opencl_program);
		assert(err == 0);
		err = starpu_opencl_unload_opencl(&opencl_conversion_program);
		assert(err == 0);
	}
#endif /* !STARPU_USE_OPENCL */
	starpu_shutdown();		
	print_it();
	return check_it();


enodev:
	return 77;
#endif
}
