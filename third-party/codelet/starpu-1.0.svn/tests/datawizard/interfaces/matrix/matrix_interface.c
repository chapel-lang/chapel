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

#define WIDTH  16
#define HEIGHT 16

#ifdef STARPU_USE_CPU
static void test_matrix_cpu_func(void *buffers[], void *args);
#endif /* !STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
extern void test_matrix_cuda_func(void *buffers[], void *_args);
#endif
#ifdef STARPU_USE_OPENCL
extern void test_matrix_opencl_func(void *buffers[], void *args);
#endif


static starpu_data_handle_t matrix_handle;
static starpu_data_handle_t matrix2_handle;

struct test_config matrix_config =
{
#ifdef STARPU_USE_CPU
	.cpu_func      = test_matrix_cpu_func,
#endif /* ! STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
	.cuda_func     = test_matrix_cuda_func,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_func   = test_matrix_opencl_func,
#endif
	.handle        = &matrix_handle,
	.dummy_handle  = &matrix2_handle,
	.copy_failed   = 0,
	.name          = "matrix_interface"
};

static int matrix[WIDTH * HEIGHT];
static int matrix2[WIDTH * HEIGHT];

static void
register_data(void)
{
	int i;
	int size = WIDTH * HEIGHT;
	for (i = 0; i < size; i++)
		matrix[i] = i;

	starpu_matrix_data_register(&matrix_handle,
				    0,
				    (uintptr_t) matrix,
				    WIDTH, /* ld */
				    WIDTH,
				    HEIGHT,
				    sizeof(matrix[0]));
	starpu_matrix_data_register(&matrix2_handle,
				    0,
				    (uintptr_t) matrix2,
				    WIDTH, /* ld */
				    WIDTH,
				    HEIGHT,
				    sizeof(matrix[0]));
}

static void
unregister_data(void)
{
	starpu_data_unregister(matrix_handle);
	starpu_data_unregister(matrix2_handle);
}

static void
test_matrix_cpu_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	int *val;
	int factor;
	int i;
	unsigned int nx, ny;

	nx = STARPU_MATRIX_GET_NX(buffers[0]);
	ny = STARPU_MATRIX_GET_NY(buffers[0]);
	val = (int *) STARPU_MATRIX_GET_PTR(buffers[0]);
	factor = *(int *) args;

	for (i = 0; i < nx*ny; i++)
	{
		if (val[i] != i * factor)
		{
			matrix_config.copy_failed = 1;
			return;
		}
		val[i] *= -1;
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

	if (starpu_init(&conf) == -ENODEV || starpu_cpu_worker_get_count() == 0)
		goto enodev;

	register_data();

	summary = run_tests(&matrix_config);
	if (!summary)
		exit(EXIT_FAILURE);

	unregister_data();

	starpu_shutdown();

	data_interface_test_summary_print(stderr, summary);

	return data_interface_test_summary_success(summary);

enodev:
	return STARPU_TEST_SKIPPED;
}
