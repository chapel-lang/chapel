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

static int variable;
static int variable2;
static starpu_data_handle_t variable_handle;
static starpu_data_handle_t variable2_handle;

/* Codelets */
#ifdef STARPU_USE_CPU
static void test_variable_cpu_func(void *buffers[], void *args);
#endif
#ifdef STARPU_USE_CUDA
extern void test_variable_cuda_func(void *buffers[], void *args);
#endif
#ifdef STARPU_USE_OPENCL
extern void test_variable_opencl_func(void *buffers[], void *args);
#endif

struct test_config variable_config =
{
#ifdef STARPU_USE_CPU
	.cpu_func     = test_variable_cpu_func,
#endif
#ifdef STARPU_USE_CUDA
	.cuda_func    = test_variable_cuda_func,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_func  = test_variable_opencl_func,
#endif
	.handle       = &variable_handle,
	.dummy_handle = &variable2_handle,
	.copy_failed  = 0,
	.name         = "variable_interface"
};

static void
test_variable_cpu_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	int *val;
	int factor;

	val = (int *) STARPU_VARIABLE_GET_PTR(buffers[0]);
	factor = *(int *)args;

	if (*val != 42 * factor)
		variable_config.copy_failed = 1;
	else
		*val *= -1;
}


static
void register_data(void)
{
	variable = 42;
	variable2 = 12;

	starpu_variable_data_register(&variable_handle, 0,
				      (uintptr_t) &variable, sizeof(variable));
	starpu_variable_data_register(&variable2_handle, 0,
				      (uintptr_t) &variable2, sizeof(variable2));
}

static
void unregister_data(void)
{
	starpu_data_unregister(variable_handle);
	starpu_data_unregister(variable2_handle);
}

int
main(void)
{
	int ret;
	data_interface_test_summary *summary;

	struct starpu_conf conf =
	{
		.ncpus = -1,
		.ncuda = 2,
		.nopencl = 1
	};

	ret = starpu_init(&conf);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;

	register_data();

	summary = run_tests(&variable_config);
	if (!summary)
		exit(EXIT_FAILURE);

	unregister_data();

	starpu_shutdown();

	data_interface_test_summary_print(stderr, summary);

	return data_interface_test_summary_success(summary);
}
