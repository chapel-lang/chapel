/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
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

#include "cg.h"
#include <math.h>
#include <limits.h>

#if 0
static void print_vector_from_descr(unsigned nx, TYPE *v)
{
	unsigned i;
	for (i = 0; i < nx; i++)
	{
		fprintf(stderr, "%2.2e ", v[i]);
	}
	fprintf(stderr, "\n");
}


static void print_matrix_from_descr(unsigned nx, unsigned ny, unsigned ld, TYPE *mat)
{
	unsigned i, j;
	for (j = 0; j < nx; j++)
	{
		for (i = 0; i < ny; i++)
		{
			fprintf(stderr, "%2.2e ", mat[j+i*ld]);
		}
		fprintf(stderr, "\n");
	}
}
#endif


/*
 *	Reduction accumulation methods
 */

#ifdef STARPU_USE_CUDA
static void accumulate_variable_cuda(void *descr[], void *cl_arg)
{
	TYPE *v_dst = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]);
	TYPE *v_src = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[1]);
 
	cublasaxpy(1, (TYPE)1.0, v_src, 1, v_dst, 1);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void accumulate_variable_cpu(void *descr[], void *cl_arg)
{
	TYPE *v_dst = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]);
	TYPE *v_src = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[1]);
 
	*v_dst = *v_dst + *v_src;
}

static struct starpu_perfmodel accumulate_variable_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "accumulate_variable"
};

struct starpu_codelet accumulate_variable_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {accumulate_variable_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {accumulate_variable_cuda, NULL},
#endif
	.nbuffers = 2,
	.model = &accumulate_variable_model
};

#ifdef STARPU_USE_CUDA
static void accumulate_vector_cuda(void *descr[], void *cl_arg)
{
	TYPE *v_dst = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v_src = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	cublasaxpy(n, (TYPE)1.0, v_src, 1, v_dst, 1);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void accumulate_vector_cpu(void *descr[], void *cl_arg)
{
	TYPE *v_dst = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v_src = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	AXPY(n, (TYPE)1.0, v_src, 1, v_dst, 1);
}

static struct starpu_perfmodel accumulate_vector_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "accumulate_vector"
};

struct starpu_codelet accumulate_vector_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {accumulate_vector_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {accumulate_vector_cuda, NULL},
#endif
	.nbuffers = 2,
	.model = &accumulate_vector_model
};

/*
 *	Reduction initialization methods
 */

#ifdef STARPU_USE_CUDA
extern void zero_vector(TYPE *x, unsigned nelems);

static void bzero_variable_cuda(void *descr[], void *cl_arg)
{
	TYPE *v = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]);

	zero_vector(v, 1);
 
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void bzero_variable_cpu(void *descr[], void *cl_arg)
{
	TYPE *v = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]);
	*v = (TYPE)0.0;
}

static struct starpu_perfmodel bzero_variable_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "bzero_variable"
};

struct starpu_codelet bzero_variable_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {bzero_variable_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {bzero_variable_cuda, NULL},
#endif
	.nbuffers = 1,
	.model = &bzero_variable_model
};

#ifdef STARPU_USE_CUDA
static void bzero_vector_cuda(void *descr[], void *cl_arg)
{
	TYPE *v = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	zero_vector(v, n);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void bzero_vector_cpu(void *descr[], void *cl_arg)
{
	TYPE *v = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	memset(v, 0, n*sizeof(TYPE));
}

static struct starpu_perfmodel bzero_vector_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "bzero_vector"
};

struct starpu_codelet bzero_vector_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {bzero_vector_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {bzero_vector_cuda, NULL},
#endif
	.nbuffers = 1,
	.model = &bzero_vector_model
};

/*
 *	DOT kernel : s = dot(v1, v2)
 */

#ifdef STARPU_USE_CUDA
extern void dot_host(TYPE *x, TYPE *y, unsigned nelems, TYPE *dot);

static void dot_kernel_cuda(void *descr[], void *cl_arg)
{
	TYPE *dot = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]); 
	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[2]);

	unsigned n = STARPU_VECTOR_GET_NX(descr[1]);

	/* Contrary to cublasSdot, this function puts its result directly in
	 * device memory, so that we don't have to transfer that value back and
	 * forth. */
	dot_host(v1, v2, n, dot);
}
#endif

static void dot_kernel_cpu(void *descr[], void *cl_arg)
{
	TYPE *dot = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]); 
	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[2]);

	unsigned n = STARPU_VECTOR_GET_NX(descr[1]);

	TYPE local_dot = 0.0;
	/* Note that we explicitely cast the result of the DOT kernel because
	 * some BLAS library will return a double for sdot for instance. */
	local_dot = (TYPE)DOT(n, v1, 1, v2, 1);

	*dot = *dot + local_dot;
}

static struct starpu_perfmodel dot_kernel_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "dot_kernel"
};

static struct starpu_codelet dot_kernel_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {dot_kernel_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {dot_kernel_cuda, NULL},
#endif
	.nbuffers = 3,
	.model = &dot_kernel_model
};

int dot_kernel(starpu_data_handle_t v1,
	       starpu_data_handle_t v2,
	       starpu_data_handle_t s,
	       unsigned nblocks,
	       int use_reduction)
{
	int ret;

	/* Blank the accumulation variable */
	ret = starpu_insert_task(&bzero_variable_cl, STARPU_W, s, 0);
	if (ret == -ENODEV) return ret;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	unsigned b;
	for (b = 0; b < nblocks; b++)
	{
		ret = starpu_insert_task(&dot_kernel_cl,
					 use_reduction?STARPU_REDUX:STARPU_RW, s,
					 STARPU_R, starpu_data_get_sub_data(v1, 1, b),
					 STARPU_R, starpu_data_get_sub_data(v2, 1, b),
					 0);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");
	}
	return 0;
}

/*
 *	SCAL kernel : v1 = p1 v1
 */

#ifdef STARPU_USE_CUDA
static void scal_kernel_cuda(void *descr[], void *cl_arg)
{
	TYPE p1;
	starpu_codelet_unpack_args(cl_arg, &p1);

	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	/* v1 = p1 v1 */
	TYPE alpha = p1;
	cublasscal(n, alpha, v1, 1);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void scal_kernel_cpu(void *descr[], void *cl_arg)
{
	TYPE alpha;
	starpu_codelet_unpack_args(cl_arg, &alpha);

	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	/* v1 = alpha v1 */
	SCAL(n, alpha, v1, 1);
}

static struct starpu_perfmodel scal_kernel_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "scal_kernel"
};

static struct starpu_codelet scal_kernel_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {scal_kernel_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {scal_kernel_cuda, NULL},
#endif
	.nbuffers = 1,
	.model = &scal_kernel_model
};

/*
 *	GEMV kernel : v1 = p1 * v1 + p2 * M v2 
 */

#ifdef STARPU_USE_CUDA
static void gemv_kernel_cuda(void *descr[], void *cl_arg)
{
	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[2]);
	TYPE *M = (TYPE *)STARPU_MATRIX_GET_PTR(descr[1]);

	unsigned ld = STARPU_MATRIX_GET_LD(descr[1]);
	unsigned nx = STARPU_MATRIX_GET_NX(descr[1]);
	unsigned ny = STARPU_MATRIX_GET_NY(descr[1]);
 
	TYPE alpha, beta;
	starpu_codelet_unpack_args(cl_arg, &beta, &alpha);

	/* Compute v1 = alpha M v2 + beta v1 */
	cublasgemv('N', nx, ny, alpha, M, ld, v2, 1, beta, v1, 1);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void gemv_kernel_cpu(void *descr[], void *cl_arg)
{
	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[2]);
	TYPE *M = (TYPE *)STARPU_MATRIX_GET_PTR(descr[1]);

	unsigned ld = STARPU_MATRIX_GET_LD(descr[1]);
	unsigned nx = STARPU_MATRIX_GET_NX(descr[1]);
	unsigned ny = STARPU_MATRIX_GET_NY(descr[1]);

	TYPE alpha, beta;
	starpu_codelet_unpack_args(cl_arg, &beta, &alpha);

	int worker_size = starpu_combined_worker_get_size();

	if (worker_size > 1)
	{
		/* Parallel CPU task */
		int rank = starpu_combined_worker_get_rank();
		
		int block_size = (ny + worker_size - 1)/worker_size;
		int new_nx = STARPU_MIN(nx, block_size*(rank+1)) - block_size*rank;

		nx = new_nx;
		v1 = &v1[block_size*rank];
		M = &M[block_size*rank];
	}

	/* Compute v1 = alpha M v2 + beta v1 */
	GEMV("N", nx, ny, alpha, M, ld, v2, 1, beta, v1, 1);
}

static struct starpu_perfmodel gemv_kernel_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "gemv_kernel"
};

static struct starpu_codelet gemv_kernel_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.type = STARPU_SPMD,
	.max_parallelism = INT_MAX,
	.cpu_funcs = {gemv_kernel_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {gemv_kernel_cuda, NULL},
#endif
	.nbuffers = 3,
	.model = &gemv_kernel_model
};

int gemv_kernel(starpu_data_handle_t v1,
		starpu_data_handle_t matrix,
		starpu_data_handle_t v2,
		TYPE p1, TYPE p2,
		unsigned nblocks,
		int use_reduction)
{
	unsigned b1, b2;
	int ret;

	for (b2 = 0; b2 < nblocks; b2++)
	{
		ret = starpu_insert_task(&scal_kernel_cl,
					 STARPU_RW, starpu_data_get_sub_data(v1, 1, b2),
					 STARPU_VALUE, &p1, sizeof(p1),
					 0);
		if (ret == -ENODEV) return ret;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");
	}

	for (b2 = 0; b2 < nblocks; b2++)
	{
		for (b1 = 0; b1 < nblocks; b1++)
		{
			TYPE one = 1.0;
			ret = starpu_insert_task(&gemv_kernel_cl,
						 use_reduction?STARPU_REDUX:STARPU_RW,	starpu_data_get_sub_data(v1, 1, b2),
						 STARPU_R,	starpu_data_get_sub_data(matrix, 2, b2, b1),
						 STARPU_R,	starpu_data_get_sub_data(v2, 1, b1),
						 STARPU_VALUE,	&one,	sizeof(one),
						 STARPU_VALUE,	&p2,	sizeof(p2),
						 0);
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");
		}
	}
	return 0;
}

/*
 *	AXPY + SCAL kernel : v1 = p1 * v1 + p2 * v2 
 */
#ifdef STARPU_USE_CUDA
static void scal_axpy_kernel_cuda(void *descr[], void *cl_arg)
{
	TYPE p1, p2;
	starpu_codelet_unpack_args(cl_arg, &p1, &p2);

	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);

	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	/* Compute v1 = p1 * v1 + p2 * v2.
	 *	v1 = p1 v1
	 *	v1 = v1 + p2 v2
	 */
	cublasscal(n, p1, v1, 1);
	cublasaxpy(n, p2, v2, 1, v1, 1);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void scal_axpy_kernel_cpu(void *descr[], void *cl_arg)
{
	TYPE p1, p2;
	starpu_codelet_unpack_args(cl_arg, &p1, &p2);

	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);

	unsigned nx = STARPU_VECTOR_GET_NX(descr[0]);
 
	/* Compute v1 = p1 * v1 + p2 * v2.
	 *	v1 = p1 v1
	 *	v1 = v1 + p2 v2
	 */
	SCAL(nx, p1, v1, 1);
	AXPY(nx, p2, v2, 1, v1, 1);
}

static struct starpu_perfmodel scal_axpy_kernel_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "scal_axpy_kernel"
};

static struct starpu_codelet scal_axpy_kernel_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {scal_axpy_kernel_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {scal_axpy_kernel_cuda, NULL},
#endif
	.nbuffers = 2,
	.model = &scal_axpy_kernel_model
};

int scal_axpy_kernel(starpu_data_handle_t v1, TYPE p1,
		     starpu_data_handle_t v2, TYPE p2,
		     unsigned nblocks)
{
	int ret;
	unsigned b;
	for (b = 0; b < nblocks; b++)
	{
		ret = starpu_insert_task(&scal_axpy_kernel_cl,
					 STARPU_RW, starpu_data_get_sub_data(v1, 1, b),
					 STARPU_R,  starpu_data_get_sub_data(v2, 1, b),
					 STARPU_VALUE, &p1, sizeof(p1),
					 STARPU_VALUE, &p2, sizeof(p2),
					 0);
		if (ret == -ENODEV) return ret;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");
	}
	return 0;
}


/*
 *	AXPY kernel : v1 = v1 + p1 * v2 
 */
#ifdef STARPU_USE_CUDA
static void axpy_kernel_cuda(void *descr[], void *cl_arg)
{
	TYPE p1;
	starpu_codelet_unpack_args(cl_arg, &p1);

	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);

	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);
 
	/* Compute v1 = v1 + p1 * v2.
	 */
	cublasaxpy(n, p1, v2, 1, v1, 1);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

static void axpy_kernel_cpu(void *descr[], void *cl_arg)
{
	TYPE p1;
	starpu_codelet_unpack_args(cl_arg, &p1);

	TYPE *v1 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	TYPE *v2 = (TYPE *)STARPU_VECTOR_GET_PTR(descr[1]);

	unsigned nx = STARPU_VECTOR_GET_NX(descr[0]);
 
	/* Compute v1 = p1 * v1 + p2 * v2.
	 */
	AXPY(nx, p1, v2, 1, v1, 1);
}

static struct starpu_perfmodel axpy_kernel_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "axpy_kernel"
};

static struct starpu_codelet axpy_kernel_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {axpy_kernel_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {axpy_kernel_cuda, NULL},
#endif
	.nbuffers = 2,
	.model = &axpy_kernel_model
};

int axpy_kernel(starpu_data_handle_t v1,
		starpu_data_handle_t v2, TYPE p1,
		unsigned nblocks)
{
	int ret;
	unsigned b;
	for (b = 0; b < nblocks; b++)
	{
		ret = starpu_insert_task(&axpy_kernel_cl,
					 STARPU_RW, starpu_data_get_sub_data(v1, 1, b),
					 STARPU_R,  starpu_data_get_sub_data(v2, 1, b),
					 STARPU_VALUE, &p1, sizeof(p1),
					 0);
		if (ret == -ENODEV) return ret;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");
	}
	return 0;
}

int copy_handle(starpu_data_handle_t dst, starpu_data_handle_t src, unsigned nblocks)
{
	unsigned b;
	for (b = 0; b < nblocks; b++)
		starpu_data_cpy(starpu_data_get_sub_data(dst, 1, b), starpu_data_get_sub_data(src, 1, b), 1, NULL, NULL);
	return 0;
}
