/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#include <math.h>
#include <assert.h>
#include <sys/time.h>
#include <starpu.h>
#include <common/blas.h>

#ifdef STARPU_USE_CUDA
#include <cuda.h>
#include <cublas.h>
#endif

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

/*
 *	Conjugate Gradient
 *
 *	Input:
 *		- matrix A
 *		- vector b
 *		- vector x (starting value)
 *		- int i_max, error tolerance eps < 1.
 *	Ouput:
 *		- vector x
 *
 *	Pseudo code:
 *
 *		i <- 0
 *		r <- b - Ax
 *		d <- r
 *		delta_new <- dot(r,r)
 *		delta_0 <- delta_new
 *	
 *		while (i < i_max && delta_new > eps^2 delta_0)
 *		{
 *			q <- Ad
 *			alpha <- delta_new/dot(d, q)
 *			x <- x + alpha d
 *	
 *			If (i is divisible by 50)
 *				r <- b - Ax
 *			else
 *				r <- r - alpha q
 *			
 *			delta_old <- delta_new
 *			delta_new <- dot(r,r)
 *			beta <- delta_new/delta_old
 *			d <- r + beta d
 *			i <- i + 1
 *		}
 *	
 */

#include "cg.h"

static int long long n = 1024;
static int nblocks = 8;
static int use_reduction = 1;

static starpu_data_handle_t A_handle, b_handle, x_handle;
static TYPE *A, *b, *x;

static int i_max = 4000;
static double eps = (10e-14);

static starpu_data_handle_t r_handle, d_handle, q_handle;
static TYPE *r, *d, *q;

static starpu_data_handle_t dtq_handle, rtr_handle;
static TYPE dtq, rtr;

extern struct starpu_codelet accumulate_variable_cl;
extern struct starpu_codelet accumulate_vector_cl;
extern struct starpu_codelet bzero_variable_cl;
extern struct starpu_codelet bzero_vector_cl;

/*
 *	Generate Input data
 */

static void generate_random_problem(void)
{
	int i, j;

	starpu_malloc((void **)&A, n*n*sizeof(TYPE));
	starpu_malloc((void **)&b, n*sizeof(TYPE));
	starpu_malloc((void **)&x, n*sizeof(TYPE));
	assert(A && b && x);

	for (j = 0; j < n; j++)
	{
		b[j] = (TYPE)1.0;
		x[j] = (TYPE)0.0;

		/* We take Hilbert matrix that is not well conditionned but definite positive: H(i,j) = 1/(1+i+j) */
		for (i = 0; i < n; i++)
		{
			A[n*j + i] = (TYPE)(1.0/(1.0+i+j));
		}
	}

	/* Internal vectors */
	starpu_malloc((void **)&r, n*sizeof(TYPE));
	starpu_malloc((void **)&d, n*sizeof(TYPE));
	starpu_malloc((void **)&q, n*sizeof(TYPE));
	assert(r && d && q);

	memset(r, 0, n*sizeof(TYPE));
	memset(d, 0, n*sizeof(TYPE));
	memset(q, 0, n*sizeof(TYPE));
}

static void free_data(void)
{
	starpu_free(A);
	starpu_free(b);
	starpu_free(x);
	starpu_free(r);
	starpu_free(d);
	starpu_free(q);
}

static void register_data(void)
{
	starpu_matrix_data_register(&A_handle, 0, (uintptr_t)A, n, n, n, sizeof(TYPE));
	starpu_vector_data_register(&b_handle, 0, (uintptr_t)b, n, sizeof(TYPE));
	starpu_vector_data_register(&x_handle, 0, (uintptr_t)x, n, sizeof(TYPE));

	starpu_vector_data_register(&r_handle, 0, (uintptr_t)r, n, sizeof(TYPE));
	starpu_vector_data_register(&d_handle, 0, (uintptr_t)d, n, sizeof(TYPE));
	starpu_vector_data_register(&q_handle, 0, (uintptr_t)q, n, sizeof(TYPE));

	starpu_variable_data_register(&dtq_handle, 0, (uintptr_t)&dtq, sizeof(TYPE));
	starpu_variable_data_register(&rtr_handle, 0, (uintptr_t)&rtr, sizeof(TYPE));

	if (use_reduction)
	{
		starpu_data_set_reduction_methods(q_handle, &accumulate_vector_cl, &bzero_vector_cl);
		starpu_data_set_reduction_methods(r_handle, &accumulate_vector_cl, &bzero_vector_cl);
	
		starpu_data_set_reduction_methods(dtq_handle, &accumulate_variable_cl, &bzero_variable_cl);
		starpu_data_set_reduction_methods(rtr_handle, &accumulate_variable_cl, &bzero_variable_cl);
	}
}

static void unregister_data(void)
{
	starpu_data_unpartition(A_handle, 0);
	starpu_data_unpartition(b_handle, 0);
	starpu_data_unpartition(x_handle, 0);

	starpu_data_unpartition(r_handle, 0);
	starpu_data_unpartition(d_handle, 0);
	starpu_data_unpartition(q_handle, 0);

	starpu_data_unregister(A_handle);
	starpu_data_unregister(b_handle);
	starpu_data_unregister(x_handle);

	starpu_data_unregister(r_handle);
	starpu_data_unregister(d_handle);
	starpu_data_unregister(q_handle);

	starpu_data_unregister(dtq_handle);
	starpu_data_unregister(rtr_handle);
}

/*
 *	Data partitioning filters
 */

struct starpu_data_filter vector_filter;
struct starpu_data_filter matrix_filter_1;
struct starpu_data_filter matrix_filter_2;

static void partition_data(void)
{
	assert(n % nblocks == 0);

	/*
	 *	Partition the A matrix
	 */

	/* Partition into contiguous parts */
	matrix_filter_1.filter_func = starpu_block_filter_func;
	matrix_filter_1.nchildren = nblocks;
	/* Partition into non-contiguous parts */
	matrix_filter_2.filter_func = starpu_vertical_block_filter_func;
	matrix_filter_2.nchildren = nblocks;

	/* A is in FORTRAN ordering, starpu_data_get_sub_data(A_handle, 2, i,
	 * j) designates the block in column i and row j. */
	starpu_data_map_filters(A_handle, 2, &matrix_filter_1, &matrix_filter_2);

	/*
	 *	Partition the vectors
	 */

	vector_filter.filter_func = starpu_block_filter_func_vector;
	vector_filter.nchildren = nblocks;

	starpu_data_partition(b_handle, &vector_filter);
	starpu_data_partition(x_handle, &vector_filter);
	starpu_data_partition(r_handle, &vector_filter);
	starpu_data_partition(d_handle, &vector_filter);
	starpu_data_partition(q_handle, &vector_filter);
}

/*
 *	Debug
 */

#if 0
static void display_vector(starpu_data_handle_t handle, TYPE *ptr)
{
	unsigned block_size = n / nblocks;

	unsigned b, ind;
	for (b = 0; b < nblocks; b++)
	{
		starpu_data_acquire(starpu_data_get_sub_data(handle, 1, b), STARPU_R);
		for (ind = 0; ind < block_size; ind++)
		{
			FPRINTF(stderr, "%2.2e ", ptr[b*block_size + ind]);
		}
		FPRINTF(stderr, "| ");
		starpu_data_release(starpu_data_get_sub_data(handle, 1, b));
	}
	FPRINTF(stderr, "\n");
}

static void display_matrix(void)
{
	unsigned i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			FPRINTF(stderr, "%2.2e ", A[j*n + i]);
		}
		FPRINTF(stderr, "\n");
	}
}
#endif

/*
 *	Main loop
 */

static int cg(void)
{
	double delta_new, delta_old, delta_0;
	double alpha, beta;

	int i = 0;
	int ret;

	/* r <- b */
	ret = copy_handle(r_handle, b_handle, nblocks);
	if (ret == -ENODEV) return ret;

	/* r <- r - A x */
	ret = gemv_kernel(r_handle, A_handle, x_handle, 1.0, -1.0, nblocks, use_reduction); 
	if (ret == -ENODEV) return ret;

	/* d <- r */
	ret = copy_handle(d_handle, r_handle, nblocks);
	if (ret == -ENODEV) return ret;

	/* delta_new = dot(r,r) */
	ret = dot_kernel(r_handle, r_handle, rtr_handle, nblocks, use_reduction);
	if (ret == -ENODEV) return ret;

	starpu_data_acquire(rtr_handle, STARPU_R);
	delta_new = rtr;
	delta_0 = delta_new;
	starpu_data_release(rtr_handle);

	FPRINTF(stderr, "*************** INITIAL ************ \n");
	FPRINTF(stderr, "Delta 0: %e\n", delta_new);

	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);

	while ((i < i_max) && ((double)delta_new > (double)(eps*eps*delta_0)))
	{
		/* q <- A d */
		gemv_kernel(q_handle, A_handle, d_handle, 0.0, 1.0, nblocks, use_reduction);

		/* dtq <- dot(d,q) */
		dot_kernel(d_handle, q_handle, dtq_handle, nblocks, use_reduction);

		/* alpha = delta_new / dtq */
		starpu_data_acquire(dtq_handle, STARPU_R);
		alpha = delta_new/dtq;
		starpu_data_release(dtq_handle);
		
		/* x <- x + alpha d */
		axpy_kernel(x_handle, d_handle, alpha, nblocks);

		if ((i % 50) == 0)
		{
			/* r <- b */
			copy_handle(r_handle, b_handle, nblocks);
		
			/* r <- r - A x */
			gemv_kernel(r_handle, A_handle, x_handle, 1.0, -1.0, nblocks, use_reduction); 
		}
		else
		{
			/* r <- r - alpha q */
			axpy_kernel(r_handle, q_handle, -alpha, nblocks);
		}

		/* delta_new = dot(r,r) */
		dot_kernel(r_handle, r_handle, rtr_handle, nblocks, use_reduction);

		starpu_data_acquire(rtr_handle, STARPU_R);
		delta_old = delta_new;
		delta_new = rtr;
		beta = delta_new / delta_old;
		starpu_data_release(rtr_handle);

		/* d <- beta d + r */
		scal_axpy_kernel(d_handle, beta, r_handle, 1.0, nblocks);

		if ((i % 10) == 0)
		{
			/* We here take the error as ||r||_2 / (n||b||_2) */
			double error = sqrt(delta_new/delta_0)/(1.0*n);
			FPRINTF(stderr, "*****************************************\n");
			FPRINTF(stderr, "iter %d DELTA %e - %e\n", i, delta_new, error);
		}

		i++;
	}

	gettimeofday(&end, NULL);

	double timing = (double)(((double)end.tv_sec - (double)start.tv_sec)*10e6 + ((double)end.tv_usec - (double)start.tv_usec));
	FPRINTF(stderr, "Total timing : %2.2f seconds\n", timing/10e6);
	FPRINTF(stderr, "Seconds per iteration : %2.2e\n", timing/10e6/i);
	return 0;
}

static int check(void)
{
	return 0;
}

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
	        if (strcmp(argv[i], "-n") == 0)
		{
			n = (int long long)atoi(argv[++i]);
			continue;
		}

	        if (strcmp(argv[i], "-maxiter") == 0)
		{
			i_max = atoi(argv[++i]);
			continue;
		}

	        if (strcmp(argv[i], "-nblocks") == 0)
		{
			nblocks = atoi(argv[++i]);
			continue;
		}

	        if (strcmp(argv[i], "-no-reduction") == 0)
		{
			use_reduction = 0;
			continue;
		}

	        if (strcmp(argv[i], "-h") == 0)
		{
			FPRINTF(stderr, "usage: %s [-h] [-nblocks #blocks] [-n problem_size] [-no-reduction] [-maxiter i]\n", argv[0]);
			exit(-1);
			continue;
		}
        }
}

int main(int argc, char **argv)
{
	int ret;

#ifdef STARPU_SLOW_MACHINE
	i_max = 16;
#endif

	parse_args(argc, argv);

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_helper_cublas_init();

	generate_random_problem();
	register_data();
	partition_data();

	ret = cg();
	if (ret == -ENODEV) goto enodev;

	ret = check();

	starpu_task_wait_for_all();
	unregister_data();
	free_data();
	starpu_helper_cublas_shutdown();
	starpu_shutdown();

	return ret;

enodev:
	starpu_shutdown();
	return 77;
}
