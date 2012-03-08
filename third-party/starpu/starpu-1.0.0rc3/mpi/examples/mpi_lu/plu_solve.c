/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
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
#include <math.h>
#include "pxlu.h"

/*
 *	Various useful functions
 */

static double frobenius_norm(TYPE *v, unsigned n)
{
        double sum2 = 0.0;

        /* compute sqrt(Sum(|x|^2)) */

        unsigned i,j;
        for (j = 0; j < n; j++)
        for (i = 0; i < n; i++)
        {
                double a = fabsl((double)v[i+n*j]);
                sum2 += a*a;
        }

        return sqrt(sum2);
}

void STARPU_PLU(display_data_content)(TYPE *data, unsigned blocksize)
{
	if (!STARPU_PLU(display_flag)())
		return;

	fprintf(stderr, "DISPLAY BLOCK\n");

	unsigned i, j;
	for (j = 0; j < blocksize; j++)
	{
		for (i = 0; i < blocksize; i++)
		{
			fprintf(stderr, "%f ", data[j+i*blocksize]);
		}
		fprintf(stderr, "\n");
	}

	fprintf(stderr, "****\n");
}

void STARPU_PLU(extract_upper)(unsigned block_size, TYPE *inblock, TYPE *outblock)
{
	unsigned li, lj;
	for (lj = 0; lj < block_size; lj++)
	{
		/* Upper block diag is 1 */
		outblock[lj*(block_size + 1)] = (TYPE)1.0;

		for (li = lj + 1; li < block_size; li++)
		{
			outblock[lj + li*block_size] = inblock[lj + li*block_size];
		}
	}
}

void STARPU_PLU(extract_lower)(unsigned block_size, TYPE *inblock, TYPE *outblock)
{
	unsigned li, lj;
	for (lj = 0; lj < block_size; lj++)
	{
		for (li = 0; li <= lj; li++)
		{
			outblock[lj + li*block_size] = inblock[lj + li*block_size];
		}
	}
}

/*
 *	Compute Ax = y
 */

static void STARPU_PLU(compute_ax_block)(unsigned block_size, TYPE *block_data, TYPE *sub_x, TYPE *sub_y)
{
	fprintf(stderr, "block data %p sub x %p sub y %p\n", block_data, sub_x, sub_y);
	CPU_GEMV("N", block_size, block_size, 1.0, block_data, block_size, sub_x, 1, 1.0, sub_y, 1);
}

static void STARPU_PLU(compute_ax_block_upper)(unsigned size, unsigned nblocks,
				 TYPE *block_data, TYPE *sub_x, TYPE *sub_y)
{
	unsigned block_size = size/nblocks;

	/* Take a copy of the upper part of the diagonal block */
	TYPE *upper_block_copy = calloc((block_size)*(block_size), sizeof(TYPE));
	STARPU_PLU(extract_upper)(block_size, block_data, upper_block_copy);
		
	STARPU_PLU(compute_ax_block)(block_size, upper_block_copy, sub_x, sub_y);
	
	free(upper_block_copy);
}

static void STARPU_PLU(compute_ax_block_lower)(unsigned size, unsigned nblocks,
				 TYPE *block_data, TYPE *sub_x, TYPE *sub_y)
{
	unsigned block_size = size/nblocks;

	/* Take a copy of the upper part of the diagonal block */
	TYPE *lower_block_copy = calloc((block_size)*(block_size), sizeof(TYPE));
	STARPU_PLU(extract_lower)(block_size, block_data, lower_block_copy);

	STARPU_PLU(compute_ax_block)(size/nblocks, lower_block_copy, sub_x, sub_y);
	
	free(lower_block_copy);
}

void STARPU_PLU(compute_lux)(unsigned size, TYPE *x, TYPE *y, unsigned nblocks, int rank)
{
	/* Create temporary buffers where all MPI processes are going to
	 * compute Ui x = yi where Ai is the matrix containing the blocks of U
	 * affected to process i, and 0 everywhere else. We then have y as the
	 * sum of all yi. */
	TYPE *yi = calloc(size, sizeof(TYPE));

	fprintf(stderr, "Compute LU\n");

	unsigned block_size = size/nblocks;

	/* Compute UiX = Yi */
	unsigned long i,j;
	for (j = 0; j < nblocks; j++)
	{
		if (get_block_rank(j, j) == rank)
		{
			TYPE *block_data = STARPU_PLU(get_block)(j, j);
			TYPE *sub_x = &x[j*(block_size)];
			TYPE *sub_yi = &yi[j*(block_size)];

			STARPU_PLU(compute_ax_block_upper)(size, nblocks, block_data, sub_x, sub_yi);
		}

		for (i = j + 1; i < nblocks; i++)
		{
			if (get_block_rank(i, j) == rank)
			{
				/* That block belongs to the current MPI process */
				TYPE *block_data = STARPU_PLU(get_block)(i, j);
				TYPE *sub_x = &x[i*(block_size)];
				TYPE *sub_yi = &yi[j*(block_size)];

				STARPU_PLU(compute_ax_block)(size/nblocks, block_data, sub_x, sub_yi);
			}
		}
	}

	/* Grab Sum Yi in X */
	MPI_Reduce(yi, x, size, MPI_TYPE, MPI_SUM, 0, MPI_COMM_WORLD);
	memset(yi, 0, size*sizeof(TYPE));

//	unsigned ind;
//	if (rank == 0)
//	{
//		fprintf(stderr, "INTERMEDIATE\n");
//		for (ind = 0; ind < STARPU_MIN(10, size); ind++)
//		{
//			fprintf(stderr, "x[%d] = %f\n", ind, (float)x[ind]);
//		}
//		fprintf(stderr, "****\n");
//	}

	/* Everyone needs x */
	int bcst_ret;
	bcst_ret = MPI_Bcast(&x, size, MPI_TYPE, 0, MPI_COMM_WORLD);
	STARPU_ASSERT(bcst_ret == MPI_SUCCESS);

	/* Compute LiX = Yi (with X = UX) */
	for (j = 0; j < nblocks; j++)
	{
		if (j > 0)
		for (i = 0; i < j; i++)
		{
			if (get_block_rank(i, j) == rank)
			{
				/* That block belongs to the current MPI process */
				TYPE *block_data = STARPU_PLU(get_block)(i, j);
				TYPE *sub_x = &x[i*(block_size)];
				TYPE *sub_yi = &yi[j*(block_size)];

				STARPU_PLU(compute_ax_block)(size/nblocks, block_data, sub_x, sub_yi);
			}
		}

		if (get_block_rank(j, j) == rank)
		{
			TYPE *block_data = STARPU_PLU(get_block)(j, j);
			TYPE *sub_x = &x[j*(block_size)];
			TYPE *sub_yi = &yi[j*(block_size)];

			STARPU_PLU(compute_ax_block_lower)(size, nblocks, block_data, sub_x, sub_yi);
		}
	}

	/* Grab Sum Yi in Y */
	MPI_Reduce(yi, y, size, MPI_TYPE, MPI_SUM, 0, MPI_COMM_WORLD);

	free(yi);
}



/*
 *	Allocate a contiguous matrix on node 0 and fill it with the whole
 *	content of the matrix distributed accross all nodes.
 */

TYPE *STARPU_PLU(reconstruct_matrix)(unsigned size, unsigned nblocks)
{
//	fprintf(stderr, "RECONSTRUCT MATRIX size %d nblocks %d\n", size, nblocks);

	TYPE *bigmatrix = calloc(size*size, sizeof(TYPE));

	unsigned block_size = size/nblocks;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	unsigned bi, bj;
	for (bj = 0; bj < nblocks; bj++)
	for (bi = 0; bi < nblocks; bi++)
	{
		TYPE *block;

		int block_rank = get_block_rank(bi, bj);
		
		if (block_rank == 0)
		{
			block = STARPU_PLU(get_block)(bi, bj);
		}
		else {
			MPI_Status status;

			if (rank == 0)
			{
				block = calloc(block_size*block_size, sizeof(TYPE));

				int ret = MPI_Recv(block, block_size*block_size, MPI_TYPE, block_rank, 0, MPI_COMM_WORLD, &status);
				STARPU_ASSERT(ret == MPI_SUCCESS);
			}
			else if (rank == block_rank) {
				block = STARPU_PLU(get_block)(bi, bj);
				int ret = MPI_Send(block, block_size*block_size, MPI_TYPE, 0, 0, MPI_COMM_WORLD);
				STARPU_ASSERT(ret == MPI_SUCCESS);
			}
		}

		if (rank == 0)
		{
			unsigned j, i;
			for (j = 0; j < block_size; j++)
			for (i = 0; i < block_size; i++)
			{
				bigmatrix[(j + bj*block_size)+(i+bi*block_size)*size] =
									block[j+i*block_size];
			}

			if (get_block_rank(bi, bj) != 0)
				free(block);
		}
	}

	return bigmatrix;
}

/* x and y must be valid (at least) on 0 */
void STARPU_PLU(compute_ax)(unsigned size, TYPE *x, TYPE *y, unsigned nblocks, int rank)
{
	unsigned block_size = size/nblocks;

	/* Send x to everyone */
	int bcst_ret;
	bcst_ret = MPI_Bcast(&x, size, MPI_TYPE, 0, MPI_COMM_WORLD);
	STARPU_ASSERT(bcst_ret == MPI_SUCCESS);

	/* Create temporary buffers where all MPI processes are going to
	 * compute Ai x = yi where Ai is the matrix containing the blocks of A
	 * affected to process i, and 0 everywhere else. We then have y as the
	 * sum of all yi. */
	TYPE *yi = calloc(size, sizeof(TYPE));

	/* Compute Aix = yi */
	unsigned long i,j;
	for (j = 0; j < nblocks; j++)
	{
		for (i = 0; i < nblocks; i++)
		{
			if (get_block_rank(i, j) == rank)
			{
				/* That block belongs to the current MPI process */
				TYPE *block_data = STARPU_PLU(get_block)(i, j);
				TYPE *sub_x = &x[i*block_size];
				TYPE *sub_yi = &yi[j*block_size];

				STARPU_PLU(compute_ax_block)(block_size, block_data, sub_x, sub_yi);
			}
		}
	}

	/* Compute the Sum of all yi = y */
	MPI_Reduce(yi, y, size, MPI_TYPE, MPI_SUM, 0, MPI_COMM_WORLD);

	fprintf(stderr, "RANK %d - FOO 1 y[0] %f\n", rank, y[0]);

	free(yi);
}

void STARPU_PLU(compute_lu_matrix)(unsigned size, unsigned nblocks, TYPE *Asaved)
{
	TYPE *all_r = STARPU_PLU(reconstruct_matrix)(size, nblocks);

	unsigned display = STARPU_PLU(display_flag)();

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
	        TYPE *L = malloc((size_t)size*size*sizeof(TYPE));
	        TYPE *U = malloc((size_t)size*size*sizeof(TYPE));
	
	        memset(L, 0, size*size*sizeof(TYPE));
	        memset(U, 0, size*size*sizeof(TYPE));
	
	        /* only keep the lower part */
		unsigned i, j;
	        for (j = 0; j < size; j++)
	        {
	                for (i = 0; i < j; i++)
	                {
	                        L[j+i*size] = all_r[j+i*size];
	                }
	
	                /* diag i = j */
	                L[j+j*size] = all_r[j+j*size];
	                U[j+j*size] = 1.0;
	
	                for (i = j+1; i < size; i++)
	                {
	                        U[j+i*size] = all_r[j+i*size];
	                }
	        }
	
		STARPU_PLU(display_data_content)(L, size);
		STARPU_PLU(display_data_content)(U, size);
	
	        /* now A_err = L, compute L*U */
	        CPU_TRMM("R", "U", "N", "U", size, size, 1.0f, U, size, L, size);
	
		if (display)
			fprintf(stderr, "\nLU\n");

		STARPU_PLU(display_data_content)(L, size);
	
	        /* compute "LU - A" in L*/
	        CPU_AXPY(size*size, -1.0, Asaved, 1, L, 1);
	
	        TYPE err = CPU_ASUM(size*size, L, 1);
	        int max = CPU_IAMAX(size*size, L, 1);
	
		if (display)
			fprintf(stderr, "DISPLAY ERROR\n");

		STARPU_PLU(display_data_content)(L, size);
	
	        fprintf(stderr, "(A - LU) Avg error : %e\n", err/(size*size));
	        fprintf(stderr, "(A - LU) Max error : %e\n", L[max]);
	
		double residual = frobenius_norm(L, size);
		double matnorm = frobenius_norm(Asaved, size);
	
		fprintf(stderr, "||A-LU|| / (||A||*N) : %e\n", residual/(matnorm*size));
	}
}

