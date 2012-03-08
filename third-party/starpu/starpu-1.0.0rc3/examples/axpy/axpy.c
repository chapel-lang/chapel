/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include <common/blas.h>

#ifdef STARPU_USE_CUDA
#include <cublas.h>
#endif

#define TYPE	float
#define AXPY	SAXPY
#define CUBLASAXPY	cublasSaxpy

#define N	(16*1024*1024)

#define NBLOCKS	8

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

TYPE *vec_x, *vec_y;

/* descriptors for StarPU */
starpu_data_handle_t handle_y, handle_x;

void axpy_cpu(void *descr[], __attribute__((unused)) void *arg)
{
	TYPE alpha = *((TYPE *)arg);

	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	TYPE *block_x = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *block_y = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);

	AXPY((int)n, alpha, block_x, 1, block_y, 1);
}

#ifdef STARPU_USE_CUDA
void axpy_gpu(void *descr[], __attribute__((unused)) void *arg)
{
	TYPE alpha = *((TYPE *)arg);

	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	TYPE *block_x = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *block_y = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);

	CUBLASAXPY((int)n, alpha, block_x, 1, block_y, 1);
	cudaThreadSynchronize();
}
#endif

static struct starpu_codelet axpy_cl =
{
        .where =
#ifdef STARPU_USE_CUDA
                STARPU_CUDA|
#endif
                STARPU_CPU,

	.cpu_funcs = {axpy_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {axpy_gpu, NULL},
#endif
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_RW}
};

int main(int argc, char **argv)
{
	int ret;

	/* Initialize StarPU */
	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_helper_cublas_init();

	/* This is equivalent to
		vec_a = malloc(N*sizeof(TYPE));
		vec_b = malloc(N*sizeof(TYPE));
	*/
	starpu_malloc((void **)&vec_x, N*sizeof(TYPE));
	assert(vec_x);

	starpu_malloc((void **)&vec_y, N*sizeof(TYPE));
	assert(vec_y);

	unsigned i;
	for (i = 0; i < N; i++)
	{
		vec_x[i] = 1.0f; /*(TYPE)starpu_drand48(); */
		vec_y[i] = 4.0f; /*(TYPE)starpu_drand48(); */
	}

	FPRINTF(stderr, "BEFORE x[0] = %2.2f\n", vec_x[0]);
	FPRINTF(stderr, "BEFORE y[0] = %2.2f\n", vec_y[0]);

	/* Declare the data to StarPU */
	starpu_vector_data_register(&handle_x, 0, (uintptr_t)vec_x, N, sizeof(TYPE));
	starpu_vector_data_register(&handle_y, 0, (uintptr_t)vec_y, N, sizeof(TYPE));

	/* Divide the vector into blocks */
	struct starpu_data_filter block_filter =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = NBLOCKS
	};

	starpu_data_partition(handle_x, &block_filter);
	starpu_data_partition(handle_y, &block_filter);

	TYPE alpha = 3.41;

	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);

	unsigned b;
	for (b = 0; b < NBLOCKS; b++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = &axpy_cl;

		task->cl_arg = &alpha;

		task->handles[0] = starpu_data_get_sub_data(handle_x, 1, b);
		task->handles[1] = starpu_data_get_sub_data(handle_y, 1, b);

		ret = starpu_task_submit(task);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	starpu_task_wait_for_all();

	starpu_data_unpartition(handle_x, 0);
	starpu_data_unpartition(handle_y, 0);
	starpu_data_unregister(handle_x);
	starpu_data_unregister(handle_y);

	gettimeofday(&end, NULL);
        double timing = (double)((end.tv_sec - start.tv_sec)*1000000 +
                                        (end.tv_usec - start.tv_usec));

	FPRINTF(stderr, "timing -> %2.2f us %2.2f MB/s\n", timing, 3*N*sizeof(TYPE)/timing);

	FPRINTF(stderr, "AFTER y[0] = %2.2f (ALPHA = %2.2f)\n", vec_y[0], alpha);

	starpu_free((void *)vec_x);
	starpu_free((void *)vec_y);

	/* Stop StarPU */
	starpu_shutdown();

	return 0;
}
