/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

/*
 * XXX : These values should not be changed. If you really understand all that
 * BCSR stuff, feel free to write a better example :)
 */

/* Size of the matrix */
#define WIDTH          4
#define HEIGHT         4
#define SIZE           (WIDTH * HEIGHT)

/* Size of the blocks */
#define R              2
#define C              2
#define BLOCK_SIZE     (R*C)

/* The matrix is simply 0 1 2... There are SIZE-1 non zero values... */
#define NNZ            (SIZE-1)

/* ... and SIZE/BLOCK_SIZE non zero blocks */
#define NNZ_BLOCKS     (SIZE/BLOCK_SIZE)


#ifdef STARPU_USE_CPU
static void test_bcsr_cpu_func(void *buffers[], void *args);
#endif /* !STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
extern void test_bcsr_cuda_func(void *buffers[], void *_args);
#endif
#ifdef STARPU_USE_OPENCL
extern void test_bcsr_opencl_func(void *buffers[], void *args);
#endif


static int nzval[NNZ];
static int nzval2[NNZ];

static uint32_t colind[NNZ_BLOCKS];
static uint32_t colind2[NNZ_BLOCKS];

static uint32_t rowptr[1+WIDTH/R];
static uint32_t rowptr2[1+WIDTH/R];

static starpu_data_handle_t bcsr_handle;
static starpu_data_handle_t bcsr2_handle;


struct test_config bcsr_config =
{
#ifdef STARPU_USE_CPU
	.cpu_func      = test_bcsr_cpu_func,
#endif /* !STARPU_USE_CPU */
#ifdef STARPU_USE_CUDA
	.cuda_func     = test_bcsr_cuda_func,
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	.opencl_func   = test_bcsr_opencl_func,
#endif /* !STARPU_USE_OPENCL */
	.handle        = &bcsr_handle,
	.dummy_handle  = &bcsr2_handle,
	.copy_failed   = 0,
	.name          = "bcsr_interface"
};

static void
register_data(void)
{
	int i;

	for (i = 0; i < NNZ; i++)
		nzval[i] = i;

	colind[0] = 0;
	colind[1] = 2;
	colind[2] = 0;
	colind[3] = 2;

	rowptr[0] = 0;
	rowptr[1] = 2;
	rowptr[2] = 4;
	
	starpu_bcsr_data_register(&bcsr_handle,
				  0,
				  NNZ_BLOCKS,
				  HEIGHT/R,
				  (uintptr_t) nzval,
				  colind,
				  rowptr,
				  0,
				  R,
				  C,
				  sizeof(nzval[0]));

	starpu_bcsr_data_register(&bcsr2_handle,
				  0,
				  NNZ_BLOCKS,
				  HEIGHT/R,
				  (uintptr_t) nzval2,
				  colind2,
				  rowptr2,
				  0,
				  R,
				  C,
				  sizeof(nzval2[0]));
}

static void
unregister_data(void)
{
	starpu_data_unregister(bcsr_handle);
	starpu_data_unregister(bcsr2_handle);
}

static void
test_bcsr_cpu_func(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;

	int *val;
	int factor;
	int i;

	uint32_t nnz = STARPU_BCSR_GET_NNZ(buffers[0]);
	val = (int *) STARPU_BCSR_GET_NZVAL(buffers[0]);
	factor = *(int *) args;

	for (i = 0; i < nnz; i++)
	{
		if (val[i] != i * factor)
		{
			bcsr_config.copy_failed = 1;
			return;
		}
		val[i] *= -1;
	}

	/* Check colind */
	uint32_t *col = STARPU_BCSR_GET_COLIND(buffers[0]);
	for (i = 0; i < NNZ_BLOCKS; i++)
		if (col[i] != colind[i])
			bcsr_config.copy_failed = 1;

	/* Check rowptr */
	uint32_t *row = STARPU_BCSR_GET_ROWPTR(buffers[0]);
	for (i = 0; i < 1 + WIDTH/R; i++)
		if (row[i] != rowptr[i])
			bcsr_config.copy_failed = 1;
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
		return STARPU_TEST_SKIPPED;

	register_data();

	summary = run_tests(&bcsr_config);
	if (!summary)
		exit(EXIT_FAILURE);

	unregister_data();

	starpu_shutdown();

	data_interface_test_summary_print(stderr, summary);

	return data_interface_test_summary_success(summary);
}

