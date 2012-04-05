/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010, 2011  Université de Bordeaux 1
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

#include "spmv.h"

unsigned nblocks = 4;
uint32_t size = 4*1024*1024;

starpu_data_handle_t sparse_matrix;
starpu_data_handle_t vector_in, vector_out;

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-size") == 0)
		{
			char *argptr;
			size = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nblocks") == 0)
		{
			char *argptr;
			nblocks = strtol(argv[++i], &argptr, 10);
		}
	}
}

/* This filter function takes a CSR matrix, and divides it into nparts with the
 * same number of non-zero entries. */
static void csr_filter_func(void *father_interface, void *child_interface, struct starpu_data_filter *f, unsigned id, unsigned nparts)
{
	struct starpu_csr_interface *csr_father = (struct starpu_csr_interface *) father_interface;
	struct starpu_csr_interface *csr_child = (struct starpu_csr_interface *) child_interface;

	uint32_t nrow = csr_father->nrow;
	size_t elemsize = csr_father->elemsize;
	uint32_t firstentry = csr_father->firstentry;

	/* Every sub-parts should contain the same number of non-zero entries */
	uint32_t chunk_size = (nrow + nparts - 1)/nparts;
	uint32_t *rowptr = csr_father->rowptr;

	uint32_t first_index = id*chunk_size - firstentry;
	uint32_t local_firstentry = rowptr[first_index];
	
	uint32_t child_nrow = STARPU_MIN(chunk_size, nrow - id*chunk_size);
	uint32_t local_nnz = rowptr[first_index + child_nrow] - rowptr[first_index]; 
	
	csr_child->nnz = local_nnz;
	csr_child->nrow = child_nrow;
	csr_child->firstentry = local_firstentry;
	csr_child->elemsize = elemsize;
	
	if (csr_father->nzval)
	{
		csr_child->rowptr = &csr_father->rowptr[first_index];
		csr_child->colind = &csr_father->colind[local_firstentry];
		csr_child->nzval = csr_father->nzval + local_firstentry * elemsize;
	}
}

/* partition the CSR matrix along a block distribution */
static struct starpu_data_filter csr_f =
{
	.filter_func = csr_filter_func,
	/* This value is defined later on */
	.nchildren = -1,
	/* the children also use a csr interface */
};

static struct starpu_data_filter vector_f =
{
	.filter_func = starpu_block_filter_func_vector,
	/* This value is defined later on */
	.nchildren = -1,
};

static struct starpu_codelet spmv_cl =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {spmv_kernel_cpu, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {spmv_kernel_cuda, NULL},
#endif
#ifdef STARPU_USE_OPENCL
        .opencl_funcs = {spmv_kernel_opencl, NULL},
#endif
	.nbuffers = 3,
	.modes = {STARPU_R, STARPU_R, STARPU_W},
	.model = NULL
};

int main(int argc, char **argv)
{
	int ret;
	unsigned part;
	double timing;
	struct timeval start, end;
	unsigned row, pos;
	unsigned ind;

	/* CSR matrix description */
	float *nzval;
	uint32_t nnz;
	uint32_t *colind;
	uint32_t *rowptr;
	
	/* Input and Output vectors */
	float *vector_in_ptr;
	float *vector_out_ptr;

	/*
	 *	Parse command-line arguments
	 */
	parse_args(argc, argv);

	/*
	 *	Launch StarPU
	 */
	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/*
	 *	Create a 3-band sparse matrix as input example
	 */
	nnz = 3*size-2;
	starpu_malloc((void **)&nzval, nnz*sizeof(float));
	starpu_malloc((void **)&colind, nnz*sizeof(uint32_t));
	starpu_malloc((void **)&rowptr, (size+1)*sizeof(uint32_t));
	assert(nzval && colind && rowptr);

	/* fill the matrix */
	for (row = 0, pos = 0; row < size; row++)
	{
		rowptr[row] = pos;

		if (row > 0)
		{
			nzval[pos] = 1.0f;
			colind[pos] = row-1;
			pos++;
		}
		
		nzval[pos] = 5.0f;
		colind[pos] = row;
		pos++;

		if (row < size - 1)
		{
			nzval[pos] = 1.0f;
			colind[pos] = row+1;
			pos++;
		}
	}

	STARPU_ASSERT(pos == nnz);

	rowptr[size] = nnz;
	
	/* initiate the 2 vectors */
	starpu_malloc((void **)&vector_in_ptr, size*sizeof(float));
	starpu_malloc((void **)&vector_out_ptr, size*sizeof(float));
	assert(vector_in_ptr && vector_out_ptr);

	/* fill them */
	for (ind = 0; ind < size; ind++)
	{
		vector_in_ptr[ind] = 2.0f;
		vector_out_ptr[ind] = 0.0f;
	}

	/*
	 *	Register the CSR matrix and the 2 vectors
	 */
	starpu_csr_data_register(&sparse_matrix, 0, nnz, size, (uintptr_t)nzval, colind, rowptr, 0, sizeof(float));
	starpu_vector_data_register(&vector_in, 0, (uintptr_t)vector_in_ptr, size, sizeof(float));
	starpu_vector_data_register(&vector_out, 0, (uintptr_t)vector_out_ptr, size, sizeof(float));

	/*
	 *	Partition the CSR matrix and the output vector
	 */
	csr_f.nchildren = nblocks;
	vector_f.nchildren = nblocks;
	starpu_data_partition(sparse_matrix, &csr_f);
	starpu_data_partition(vector_out, &vector_f);

	/*
	 *	If we use OpenCL, we need to compile the SpMV kernel
	 */
#ifdef STARPU_USE_OPENCL
	compile_spmv_opencl_kernel();
#endif

	gettimeofday(&start, NULL);

	/*
	 *	Create and submit StarPU tasks
	 */
	for (part = 0; part < nblocks; part++)
	{
		struct starpu_task *task = starpu_task_create();
		task->cl = &spmv_cl;
	
		task->handles[0] = starpu_data_get_sub_data(sparse_matrix, 1, part);
		task->handles[1] = vector_in;
		task->handles[2] = starpu_data_get_sub_data(vector_out, 1, part);
	
		ret = starpu_task_submit(task);
		if (STARPU_UNLIKELY(ret == -ENODEV))
		{
			FPRINTF(stderr, "No worker may execute this task\n");
			exit(0);
		}
	}

	starpu_task_wait_for_all();
	gettimeofday(&end, NULL);

	/*
	 *	Unregister the CSR matrix and the output vector
	 */
	starpu_data_unpartition(sparse_matrix, 0);
	starpu_data_unpartition(vector_out, 0);

	/*
	 *	Unregister data
	 */
	starpu_data_unregister(sparse_matrix);
	starpu_data_unregister(vector_in);
	starpu_data_unregister(vector_out);

	/*
	 *	Display the result
	 */
	for (row = 0; row < STARPU_MIN(size, 16); row++)
	{
                FPRINTF(stdout, "%2.2f\t%2.2f\n", vector_in_ptr[row], vector_out_ptr[row]);
	}

	starpu_free(nzval);
	starpu_free(colind);
	starpu_free(rowptr);
	starpu_free(vector_in_ptr);
	starpu_free(vector_out_ptr);

	/*
	 *	Stop StarPU
	 */
	starpu_shutdown();

	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
	FPRINTF(stderr, "Computation took (in ms)\n");
	FPRINTF(stdout, "%2.2f\n", timing/1000);

	return 0;
}
