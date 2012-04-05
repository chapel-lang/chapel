/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <starpu.h>

#include "pxlu.h"
//#include "pxlu_kernels.h"

#ifdef STARPU_HAVE_LIBNUMA
#include <numaif.h>
#endif

static unsigned long size = 16384;
static unsigned nblocks = 16;
static unsigned check = 0;
static unsigned p = 1;
static unsigned q = 1;
static unsigned display = 0;

#ifdef STARPU_HAVE_LIBNUMA
static unsigned numa = 0;
#endif

static size_t allocated_memory = 0;
static size_t allocated_memory_extra = 0;

static starpu_data_handle_t *dataA_handles;
static TYPE **dataA;

/* In order to implement the distributed LU decomposition, we allocate
 * temporary buffers */
#ifdef SINGLE_TMP11
static starpu_data_handle_t tmp_11_block_handle;
static TYPE *tmp_11_block;
#else
static starpu_data_handle_t *tmp_11_block_handles;
static TYPE **tmp_11_block;
#endif
#ifdef SINGLE_TMP1221
static starpu_data_handle_t *tmp_12_block_handles;
static TYPE **tmp_12_block;
static starpu_data_handle_t *tmp_21_block_handles;
static TYPE **tmp_21_block;
#else
static starpu_data_handle_t *(tmp_12_block_handles[2]);
static TYPE **(tmp_12_block[2]);
static starpu_data_handle_t *(tmp_21_block_handles[2]);
static TYPE **(tmp_21_block[2]);
#endif

int get_block_rank(unsigned i, unsigned j);

static void parse_args(int rank, int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-size") == 0) {
			char *argptr;
			size = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nblocks") == 0) {
			char *argptr;
			nblocks = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-check") == 0) {
			check = 1;
		}

		if (strcmp(argv[i], "-display") == 0) {
			display = 1;
		}

		if (strcmp(argv[i], "-numa") == 0) {
#ifdef STARPU_HAVE_LIBNUMA
			numa = 1;
#else
			if (rank == 0)
				fprintf(stderr, "Warning: libnuma is not available\n");
#endif
		}

		if (strcmp(argv[i], "-p") == 0) {
			char *argptr;
			p = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-q") == 0) {
			char *argptr;
			q = strtol(argv[++i], &argptr, 10);
		}
	}
}

unsigned STARPU_PLU(display_flag)(void)
{
	return display;
}

static void fill_block_with_random(TYPE *blockptr, unsigned size, unsigned nblocks)
{
	const unsigned block_size = (size/nblocks);

	unsigned i, j;
	for (i = 0; i < block_size; i++)
	for (j = 0; j < block_size; j++)
	{
		blockptr[j+i*block_size] = (TYPE)starpu_drand48();
	}
}

#ifdef SINGLE_TMP11
starpu_data_handle_t STARPU_PLU(get_tmp_11_block_handle)(void)
{
	return tmp_11_block_handle;
}
#else
starpu_data_handle_t STARPU_PLU(get_tmp_11_block_handle)(unsigned k)
{
	return tmp_11_block_handles[k];
}
#endif

#ifdef SINGLE_TMP1221
starpu_data_handle_t STARPU_PLU(get_tmp_12_block_handle)(unsigned j)
{
	return tmp_12_block_handles[j];
}

starpu_data_handle_t STARPU_PLU(get_tmp_21_block_handle)(unsigned i)
{
	return tmp_21_block_handles[i];
}
#else
starpu_data_handle_t STARPU_PLU(get_tmp_12_block_handle)(unsigned j, unsigned k)
{
	return tmp_12_block_handles[k%2][j];
}

starpu_data_handle_t STARPU_PLU(get_tmp_21_block_handle)(unsigned i, unsigned k)
{
	return tmp_21_block_handles[k%2][i];
}
#endif

static unsigned tmp_11_block_is_needed(int rank, unsigned nblocks, unsigned k)
{
	return 1;
}

static unsigned tmp_12_block_is_needed(int rank, unsigned nblocks, unsigned j)
{
	unsigned i;
	for (i = 1; i < nblocks; i++)
	{
		if (get_block_rank(i, j) == rank)
			return 1;
	}

	return 0;
}

static unsigned tmp_21_block_is_needed(int rank, unsigned nblocks, unsigned i)
{
	unsigned j;
	for (j = 1; j < nblocks; j++)
	{
		if (get_block_rank(i, j) == rank)
			return 1;
	}

	return 0;
}

static void init_matrix(int rank)
{
#ifdef STARPU_HAVE_LIBNUMA
	if (numa)
	{
		fprintf(stderr, "Using INTERLEAVE policy\n");
		unsigned long nodemask = ((1<<0)|(1<<1));
		int ret = set_mempolicy(MPOL_INTERLEAVE, &nodemask, 3);
		if (ret)
			perror("set_mempolicy failed");
	}
#endif

	/* Allocate a grid of data handles, not all of them have to be allocated later on */
	dataA_handles = calloc(nblocks*nblocks, sizeof(starpu_data_handle_t));
	dataA = calloc(nblocks*nblocks, sizeof(TYPE *));
	allocated_memory_extra += nblocks*nblocks*(sizeof(starpu_data_handle_t) + sizeof(TYPE *));

	size_t blocksize = (size_t)(size/nblocks)*(size/nblocks)*sizeof(TYPE);

	/* Allocate all the blocks that belong to this mpi node */
	unsigned long i,j;
	for (j = 0; j < nblocks; j++)
	{
		for (i = 0; i < nblocks; i++)
		{
			TYPE **blockptr = &dataA[j+i*nblocks];
//			starpu_data_handle_t *handleptr = &dataA_handles[j+nblocks*i];
			starpu_data_handle_t *handleptr = &dataA_handles[j+nblocks*i];

			if (get_block_rank(i, j) == rank)
			{
				/* This blocks should be treated by the current MPI process */
				/* Allocate and fill it */
				starpu_malloc((void **)blockptr, blocksize);
				allocated_memory += blocksize;

				//fprintf(stderr, "Rank %d : fill block (i = %d, j = %d)\n", rank, i, j);
				fill_block_with_random(*blockptr, size, nblocks);
				//fprintf(stderr, "Rank %d : fill block (i = %d, j = %d)\n", rank, i, j);
				if (i == j)
				{
					unsigned tmp;
					for (tmp = 0; tmp < size/nblocks; tmp++)
					{
						(*blockptr)[tmp*((size/nblocks)+1)] += (TYPE)10*nblocks;
					}
				}

				/* Register it to StarPU */
				starpu_matrix_data_register(handleptr, 0,
					(uintptr_t)*blockptr, size/nblocks,
					size/nblocks, size/nblocks, sizeof(TYPE));
			}
			else {
				*blockptr = STARPU_POISON_PTR;
				*handleptr = STARPU_POISON_PTR;
			}
		}
	}

	/* Allocate the temporary buffers required for the distributed algorithm */

	unsigned k;

	/* tmp buffer 11 */
#ifdef SINGLE_TMP11
	starpu_malloc((void **)&tmp_11_block, blocksize);
	allocated_memory_extra += blocksize;
	starpu_matrix_data_register(&tmp_11_block_handle, 0, (uintptr_t)tmp_11_block,
			size/nblocks, size/nblocks, size/nblocks, sizeof(TYPE));
#else
	tmp_11_block_handles = calloc(nblocks, sizeof(starpu_data_handle_t));
	tmp_11_block = calloc(nblocks, sizeof(TYPE *));
	allocated_memory_extra += nblocks*(sizeof(starpu_data_handle_t) + sizeof(TYPE *));

	for (k = 0; k < nblocks; k++)
	{
		if (tmp_11_block_is_needed(rank, nblocks, k))
		{
			starpu_malloc((void **)&tmp_11_block[k], blocksize);
			allocated_memory_extra += blocksize;
			STARPU_ASSERT(tmp_11_block[k]);

			starpu_matrix_data_register(&tmp_11_block_handles[k], 0,
				(uintptr_t)tmp_11_block[k],
				size/nblocks, size/nblocks, size/nblocks, sizeof(TYPE));
		}
	}
#endif

	/* tmp buffers 12 and 21 */
#ifdef SINGLE_TMP1221
	tmp_12_block_handles = calloc(nblocks, sizeof(starpu_data_handle_t));
	tmp_21_block_handles = calloc(nblocks, sizeof(starpu_data_handle_t));
	tmp_12_block = calloc(nblocks, sizeof(TYPE *));
	tmp_21_block = calloc(nblocks, sizeof(TYPE *));

	allocated_memory_extra += 2*nblocks*(sizeof(starpu_data_handle_t) + sizeof(TYPE *));
#else
	for (i = 0; i < 2; i++) {
		tmp_12_block_handles[i] = calloc(nblocks, sizeof(starpu_data_handle_t));
		tmp_21_block_handles[i] = calloc(nblocks, sizeof(starpu_data_handle_t));
		tmp_12_block[i] = calloc(nblocks, sizeof(TYPE *));
		tmp_21_block[i] = calloc(nblocks, sizeof(TYPE *));

		allocated_memory_extra += 2*nblocks*(sizeof(starpu_data_handle_t) + sizeof(TYPE *));
	}
#endif
	
	for (k = 0; k < nblocks; k++)
	{
#ifdef SINGLE_TMP1221
		if (tmp_12_block_is_needed(rank, nblocks, k))
		{
			starpu_malloc((void **)&tmp_12_block[k], blocksize);
			allocated_memory_extra += blocksize;
			STARPU_ASSERT(tmp_12_block[k]);

			starpu_matrix_data_register(&tmp_12_block_handles[k], 0,
				(uintptr_t)tmp_12_block[k],
				size/nblocks, size/nblocks, size/nblocks, sizeof(TYPE));
		}

		if (tmp_21_block_is_needed(rank, nblocks, k))
		{
			starpu_malloc((void **)&tmp_21_block[k], blocksize);
			allocated_memory_extra += blocksize;
			STARPU_ASSERT(tmp_21_block[k]);

			starpu_matrix_data_register(&tmp_21_block_handles[k], 0,
				(uintptr_t)tmp_21_block[k],
				size/nblocks, size/nblocks, size/nblocks, sizeof(TYPE));
		}
#else
	for (i = 0; i < 2; i++) {
		if (tmp_12_block_is_needed(rank, nblocks, k))
		{
			starpu_malloc((void **)&tmp_12_block[i][k], blocksize);
			allocated_memory_extra += blocksize;
			STARPU_ASSERT(tmp_12_block[i][k]);
	
			starpu_matrix_data_register(&tmp_12_block_handles[i][k], 0,
				(uintptr_t)tmp_12_block[i][k],
				size/nblocks, size/nblocks, size/nblocks, sizeof(TYPE));
		}

		if (tmp_21_block_is_needed(rank, nblocks, k))
		{
			starpu_malloc((void **)&tmp_21_block[i][k], blocksize);
			allocated_memory_extra += blocksize;
			STARPU_ASSERT(tmp_21_block[i][k]);
	
			starpu_matrix_data_register(&tmp_21_block_handles[i][k], 0,
				(uintptr_t)tmp_21_block[i][k],
				size/nblocks, size/nblocks, size/nblocks, sizeof(TYPE));
		}
	}
#endif
	}

	//display_all_blocks(nblocks, size/nblocks);
}

TYPE *STARPU_PLU(get_block)(unsigned i, unsigned j)
{
	return dataA[j+i*nblocks];
}

int get_block_rank(unsigned i, unsigned j)
{
	/* Take a 2D block cyclic distribution */
	/* NB: p (resp. q) is for "direction" i (resp. j) */
	return (j % q) * p + (i % p);
}

starpu_data_handle_t STARPU_PLU(get_block_handle)(unsigned i, unsigned j)
{
	return dataA_handles[j+i*nblocks];
}

static void display_grid(int rank, unsigned nblocks)
{
	if (!display)
		return;

	//if (rank == 0)
	{
		fprintf(stderr, "2D grid layout (Rank %d): \n", rank);
		
		unsigned i, j;
		for (j = 0; j < nblocks; j++)
		{
			for (i = 0; i < nblocks; i++)
			{
				TYPE *blockptr = STARPU_PLU(get_block)(i, j);
				starpu_data_handle_t handle = STARPU_PLU(get_block_handle)(i, j);

				fprintf(stderr, "%d (data %p handle %p)", get_block_rank(i, j), blockptr, handle);
			}
			fprintf(stderr, "\n");
		}
	}
}

int main(int argc, char **argv)
{
	int rank;
	int world_size;

#if 0
	/*
	 *	Initialization
	 */
	int thread_support;
	if (MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &thread_support) != MPI_SUCCESS) {
		fprintf(stderr,"MPI_Init_thread failed\n");
		exit(1);
	}
	if (thread_support == MPI_THREAD_FUNNELED)
		fprintf(stderr,"Warning: MPI only has funneled thread support, not serialized, hoping this will work\n");
	if (thread_support < MPI_THREAD_FUNNELED)
		fprintf(stderr,"Warning: MPI does not have thread support!\n");
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
#endif

	starpu_srand48((long int)time(NULL));

	parse_args(rank, argc, argv);

	int ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* We disable sequential consistency in this example */
	starpu_data_set_default_sequential_consistency_flag(0);

	starpu_mpi_initialize_extended(&rank, &world_size);

	STARPU_ASSERT(p*q == world_size);

	starpu_helper_cublas_init();

	int barrier_ret = MPI_Barrier(MPI_COMM_WORLD);
	STARPU_ASSERT(barrier_ret == MPI_SUCCESS);

	/*
	 * 	Problem Init
	 */

	init_matrix(rank);

	fprintf(stderr, "Rank %d: allocated (%d + %d) MB = %d MB\n", rank,
                        (int)allocated_memory/(1024*1024),
			(int)allocated_memory_extra/(1024*1024),
                        (int)(allocated_memory+allocated_memory_extra)/(1024*1024));

	display_grid(rank, nblocks);

	TYPE *a_r = NULL;
//	STARPU_PLU(display_data_content)(a_r, size);

	TYPE *x, *y;

	if (check)
	{
		x = calloc(size, sizeof(TYPE));
		STARPU_ASSERT(x);

		y = calloc(size, sizeof(TYPE));
		STARPU_ASSERT(y);

		if (rank == 0)
		{
			unsigned ind;
			for (ind = 0; ind < size; ind++)
				x[ind] = (TYPE)starpu_drand48();
		}

		a_r = STARPU_PLU(reconstruct_matrix)(size, nblocks);

		if (rank == 0)
			STARPU_PLU(display_data_content)(a_r, size);

//		STARPU_PLU(compute_ax)(size, x, y, nblocks, rank);
	}

	barrier_ret = MPI_Barrier(MPI_COMM_WORLD);
	STARPU_ASSERT(barrier_ret == MPI_SUCCESS);

	double timing = STARPU_PLU(plu_main)(nblocks, rank, world_size);

	/*
	 * 	Report performance
	 */

	int reduce_ret;
	double min_timing = timing;
	double max_timing = timing;
	double sum_timing = timing;

	reduce_ret = MPI_Reduce(&timing, &min_timing, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	STARPU_ASSERT(reduce_ret == MPI_SUCCESS);

	reduce_ret = MPI_Reduce(&timing, &max_timing, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	STARPU_ASSERT(reduce_ret == MPI_SUCCESS);

	reduce_ret = MPI_Reduce(&timing, &sum_timing, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	STARPU_ASSERT(reduce_ret == MPI_SUCCESS);

	if (rank == 0)
	{
		fprintf(stderr, "Computation took: %f ms\n", max_timing/1000);
		fprintf(stderr, "\tMIN : %f ms\n", min_timing/1000);
		fprintf(stderr, "\tMAX : %f ms\n", max_timing/1000);
		fprintf(stderr, "\tAVG : %f ms\n", sum_timing/(world_size*1000));

		unsigned n = size;
		double flop = (2.0f*n*n*n)/3.0f;
		fprintf(stderr, "Synthetic GFlops : %2.2f\n", (flop/max_timing/1000.0f));
	}

	/*
	 *	Test Result Correctness
	 */

	if (check)
	{
		/*
		 *	Compute || A - LU ||
		 */

		STARPU_PLU(compute_lu_matrix)(size, nblocks, a_r);

#if 0
		/*
		 *	Compute || Ax - LUx ||
		 */

		unsigned ind;

		y2 = calloc(size, sizeof(TYPE));
		STARPU_ASSERT(y);
		
		if (rank == 0)
		{
			for (ind = 0; ind < size; ind++)
			{
				y2[ind] = (TYPE)0.0;
			}
		}

		STARPU_PLU(compute_lux)(size, x, y2, nblocks, rank);

		/* Compute y2 = y2 - y */
	        CPU_AXPY(size, -1.0, y, 1, y2, 1);
	
	        TYPE err = CPU_ASUM(size, y2, 1);
	        int max = CPU_IAMAX(size, y2, 1);
	
	        fprintf(stderr, "(A - LU)X Avg error : %e\n", err/(size*size));
	        fprintf(stderr, "(A - LU)X Max error : %e\n", y2[max]);
#endif
	}

	/*
	 * 	Termination
	 */

	barrier_ret = MPI_Barrier(MPI_COMM_WORLD);
	STARPU_ASSERT(barrier_ret == MPI_SUCCESS);

	starpu_helper_cublas_shutdown();
	starpu_mpi_shutdown();
	starpu_shutdown();

#if 0
	MPI_Finalize();
#endif

	return 0;
}
