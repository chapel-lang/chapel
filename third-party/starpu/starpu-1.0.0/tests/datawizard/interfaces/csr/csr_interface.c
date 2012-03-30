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

#define WIDTH  8
#define HEIGHT 4
#define SIZE   (WIDTH * HEIGHT)
#define NNZ    (SIZE-1)

#ifdef STARPU_USE_CPU
static void test_csr_cpu_func(void *buffers[], void *args);
#endif /* !STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
extern void test_csr_cuda_func(void *buffers[], void *_args);
#endif
#ifdef STARPU_USE_OPENCL
extern void test_csr_opencl_func(void *buffers[], void *args);
#endif


static int nzval[NNZ];
static int nzval2[NNZ];

static uint32_t colind[NNZ];
static uint32_t colind2[NNZ];

static uint32_t rowptr[HEIGHT+1];
static uint32_t rowptr2[HEIGHT+1];

static starpu_data_handle_t csr_handle;
static starpu_data_handle_t csr2_handle;

struct test_config csr_config =
{
#ifdef STARPU_USE_CPU
	.cpu_func      = test_csr_cpu_func,
#endif /* ! STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
	.cuda_func     = test_csr_cuda_func,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_func   = test_csr_opencl_func,
#endif
	.handle        = &csr_handle,
	.dummy_handle  = &csr2_handle,
	.copy_failed   = 0,
	.name          = "csr_interface"
};

static void
register_data(void)
{
	int i;
	for (i = 1; i < SIZE; i++)
	{
		nzval[i-1] = i;
		nzval2[i-1] = 42;

		colind[i-1] = i % WIDTH;
		colind2[i-1] = colind[i];
	}

	rowptr[0] = 1;
	rowptr2[0] = 1;
	for (i = 1; i < HEIGHT; i++)
	{
		rowptr[i] = i * WIDTH;
		rowptr2[i] = rowptr[i];
	}
	rowptr[HEIGHT] = NNZ + 1;
	rowptr2[HEIGHT] = rowptr[HEIGHT];

	starpu_csr_data_register(&csr_handle,
				 0,
				 NNZ,
				 HEIGHT,
				 (uintptr_t) nzval,
				 colind,
				 rowptr,
				 0,
				 sizeof(nzval[0]));
	starpu_csr_data_register(&csr2_handle,
				 0,
				 NNZ,
				 HEIGHT,
				 (uintptr_t) nzval2,
				 colind2,
				 rowptr2,
				 0,
				 sizeof(nzval2[0]));
}

static void
unregister_data(void)
{
	starpu_data_unregister(csr_handle);
	starpu_data_unregister(csr2_handle);
}

static void
test_csr_cpu_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	int *val;
	int factor;
	int i;

	uint32_t nnz = STARPU_CSR_GET_NNZ(buffers[0]);
	val = (int *) STARPU_CSR_GET_NZVAL(buffers[0]);
	factor = *(int *) args;

	for (i = 0; i < nnz; i++)
	{
		if (val[i] != (i+1) * factor)
		{
			csr_config.copy_failed = 1;
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

	summary = run_tests(&csr_config);
	if (!summary)
		exit(EXIT_FAILURE);

	unregister_data();

	starpu_shutdown();

	data_interface_test_summary_print(stderr, summary);

	return data_interface_test_summary_success(summary);

enodev:
	return STARPU_TEST_SKIPPED;
}
