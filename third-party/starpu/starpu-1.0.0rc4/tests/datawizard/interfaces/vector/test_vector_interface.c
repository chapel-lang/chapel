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
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */
#include <config.h>
#include <starpu.h>
#include "../test_interfaces.h"
#include "../../../helper.h"

/* Prototypes */
static void register_data(void);
static void unregister_data(void);
static void test_vector_cpu_func(void *buffers[], void *args);
#ifdef STARPU_USE_CUDA
extern void test_vector_cuda_func(void *buffers[], void *_args);
#endif
#ifdef STARPU_USE_OPENCL
extern void test_vector_opencl_func(void *buffers[], void *args);
#endif


static starpu_data_handle_t vector_handle;
static starpu_data_handle_t vector2_handle;

struct test_config vector_config =
{
	.cpu_func      = test_vector_cpu_func,
#ifdef STARPU_USE_CUDA
	.cuda_func     = test_vector_cuda_func,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_func   = test_vector_opencl_func,
#endif
	.handle        = &vector_handle,
	.dummy_handle  = &vector2_handle,
	.copy_failed   = 0,
	.name          = "vector_interface"
};

#define VECTOR_SIZE 123
static int vector[VECTOR_SIZE];
static int vector2[VECTOR_SIZE];

static void
register_data(void)
{
	/* Initializing data */
	int i;
	for (i = 0; i < VECTOR_SIZE; i++)
		vector[i] = i;

	/* Registering data */
	starpu_vector_data_register(&vector_handle,
                                    0,
                                    (uintptr_t)vector,
				    VECTOR_SIZE,
				    sizeof(int));
	starpu_vector_data_register(&vector2_handle,
                                    0,
                                    (uintptr_t)vector2,
				    VECTOR_SIZE,
				    sizeof(int));
}

static void
unregister_data(void)
{
	starpu_data_unregister(vector_handle);
	starpu_data_unregister(vector2_handle);
}

static void test_vector_cpu_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	unsigned n = STARPU_VECTOR_GET_NX(buffers[0]);
	int *val = (int *) STARPU_VECTOR_GET_PTR(buffers[0]);
	int factor = *(int*)args;
	unsigned int i;
	for (i = 0; i < n; i++)
	{
		if (val[i] != i*factor)
		{
			vector_config.copy_failed = 1;
			return;
		}
		val[i] = -val[i];
	}
}

int
main(void)
{
	data_interface_test_summary *summary;
	struct starpu_conf conf =
	{
		.ncpus   = -1,
		.ncuda   = 2,
		.nopencl = 1
	};

	if (starpu_init(&conf) == -ENODEV)
		goto enodev;

	register_data();

	summary = run_tests(&vector_config);
	if (!summary)
		exit(EXIT_FAILURE);

	unregister_data();

	starpu_shutdown();

	data_interface_test_summary_print(stderr, summary);

	return data_interface_test_summary_success(summary);

enodev:
	return STARPU_TEST_SKIPPED;
}
