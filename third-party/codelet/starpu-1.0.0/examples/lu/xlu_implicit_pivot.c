/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#include "xlu.h"
#include "xlu_kernels.h"

static unsigned no_prio = 0;

/*
 *	Construct the DAG
 */

static int create_task_pivot(starpu_data_handle_t *dataAp, unsigned nblocks,
			     struct piv_s *piv_description,
			     unsigned k, unsigned i,
			     starpu_data_handle_t (* get_block)(starpu_data_handle_t *, unsigned, unsigned, unsigned))
{
	int ret;

	struct starpu_task *task = starpu_task_create();

	task->cl = &cl_pivot;

	/* which sub-data is manipulated ? */
	task->handles[0] = get_block(dataAp, nblocks, k, i);

	task->cl_arg = &piv_description[k];

	/* this is an important task */
	if (!no_prio && (i == k+1))
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_11_pivot(starpu_data_handle_t *dataAp, unsigned nblocks,
				unsigned k, struct piv_s *piv_description,
				starpu_data_handle_t (* get_block)(starpu_data_handle_t *, unsigned, unsigned, unsigned))
{
	int ret;

	struct starpu_task *task = starpu_task_create();

	task->cl = &cl11_pivot;

	task->cl_arg = &piv_description[k];

	/* which sub-data is manipulated ? */
	task->handles[0] = get_block(dataAp, nblocks, k, k);

	/* this is an important task */
	if (!no_prio)
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_12(starpu_data_handle_t *dataAp, unsigned nblocks, unsigned k, unsigned j,
			  starpu_data_handle_t (* get_block)(starpu_data_handle_t *, unsigned, unsigned, unsigned))
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &cl12;

	/* which sub-data is manipulated ? */
	task->handles[0] = get_block(dataAp, nblocks, k, k);
	task->handles[1] = get_block(dataAp, nblocks, j, k);

	if (!no_prio && (j == k+1))
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_21(starpu_data_handle_t *dataAp, unsigned nblocks, unsigned k, unsigned i,
			  starpu_data_handle_t (* get_block)(starpu_data_handle_t *, unsigned, unsigned, unsigned))
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &cl21;

	/* which sub-data is manipulated ? */
	task->handles[0] = get_block(dataAp, nblocks, k, k);
	task->handles[1] = get_block(dataAp, nblocks, k, i);

	if (!no_prio && (i == k+1))
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_22(starpu_data_handle_t *dataAp, unsigned nblocks, unsigned k, unsigned i, unsigned j,
			  starpu_data_handle_t (* get_block)(starpu_data_handle_t *, unsigned, unsigned, unsigned))
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &cl22;

	/* which sub-data is manipulated ? */
	task->handles[0] = get_block(dataAp, nblocks, k, i);
	task->handles[1] = get_block(dataAp, nblocks, j, k);
	task->handles[2] = get_block(dataAp, nblocks, j, i);

	if (!no_prio &&  (i == k + 1) && (j == k +1) )
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

/*
 *	code to bootstrap the factorization
 */

static int dw_codelet_facto_pivot(starpu_data_handle_t *dataAp,
				  struct piv_s *piv_description,
				  unsigned nblocks,
				  starpu_data_handle_t (* get_block)(starpu_data_handle_t *, unsigned, unsigned, unsigned),
				  double *timing)
{
	struct timeval start;
	struct timeval end;
	int ret;

	gettimeofday(&start, NULL);

	/* create all the DAG nodes */
	unsigned i,j,k;
	for (k = 0; k < nblocks; k++)
	{
	     ret = create_task_11_pivot(dataAp, nblocks, k, piv_description, get_block);
	     if (ret == -ENODEV) return ret;

		for (i = 0; i < nblocks; i++)
		{
			if (i != k)
			{
			     ret = create_task_pivot(dataAp, nblocks, piv_description, k, i, get_block);
			     if (ret == -ENODEV) return ret;
			}
		}

		for (i = k+1; i<nblocks; i++)
		{
		     ret = create_task_12(dataAp, nblocks, k, i, get_block);
		     if (ret == -ENODEV) return ret;
		     ret = create_task_21(dataAp, nblocks, k, i, get_block);
		     if (ret == -ENODEV) return ret;
		}

		for (i = k+1; i<nblocks; i++)
		     for (j = k+1; j<nblocks; j++)
		     {
			  ret = create_task_22(dataAp, nblocks, k, i, j, get_block);
			  if (ret == -ENODEV) return ret;
		     }
	}

	/* stall the application until the end of computations */
	starpu_task_wait_for_all();

	gettimeofday(&end, NULL);

	*timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
	return 0;
}

starpu_data_handle_t get_block_with_striding(starpu_data_handle_t *dataAp,
			unsigned nblocks __attribute__((unused)), unsigned j, unsigned i)
{
	/* we use filters */
	return starpu_data_get_sub_data(*dataAp, 2, j, i);
}


int STARPU_LU(lu_decomposition_pivot)(TYPE *matA, unsigned *ipiv, unsigned size, unsigned ld, unsigned nblocks)
{
	starpu_data_handle_t dataA;

	/* monitor and partition the A matrix into blocks :
	 * one block is now determined by 2 unsigned (i,j) */
	starpu_matrix_data_register(&dataA, 0, (uintptr_t)matA, ld, size, size, sizeof(TYPE));

	struct starpu_data_filter f =
	{
		.filter_func = starpu_vertical_block_filter_func,
		.nchildren = nblocks
	};

	struct starpu_data_filter f2 =
	{
		.filter_func = starpu_block_filter_func,
		.nchildren = nblocks
	};

	starpu_data_map_filters(dataA, 2, &f, &f2);

	unsigned i;
	for (i = 0; i < size; i++)
		ipiv[i] = i;

	struct piv_s *piv_description = malloc(nblocks*sizeof(struct piv_s));
	unsigned block;
	for (block = 0; block < nblocks; block++)
	{
		piv_description[block].piv = ipiv;
		piv_description[block].first = block * (size / nblocks);
		piv_description[block].last = (block + 1) * (size / nblocks);
	}

	double timing;
	int ret = dw_codelet_facto_pivot(&dataA, piv_description, nblocks, get_block_with_striding, &timing);

	FPRINTF(stderr, "Computation took (in ms)\n");
	FPRINTF(stderr, "%2.2f\n", timing/1000);

	unsigned n = starpu_matrix_get_nx(dataA);
	double flop = (2.0f*n*n*n)/3.0f;
	FPRINTF(stderr, "Synthetic GFlops : %2.2f\n", (flop/timing/1000.0f));

	/* gather all the data */
	starpu_data_unpartition(dataA, 0);
	starpu_data_unregister(dataA);

	free(piv_description);
	return ret;
}


starpu_data_handle_t get_block_with_no_striding(starpu_data_handle_t *dataAp, unsigned nblocks, unsigned j, unsigned i)
{
	/* dataAp is an array of data handle */
	return dataAp[i+j*nblocks];
}

int STARPU_LU(lu_decomposition_pivot_no_stride)(TYPE **matA, unsigned *ipiv, unsigned size, unsigned ld, unsigned nblocks)
{
	starpu_data_handle_t *dataAp = malloc(nblocks*nblocks*sizeof(starpu_data_handle_t));

	/* monitor and partition the A matrix into blocks :
	 * one block is now determined by 2 unsigned (i,j) */
	unsigned bi, bj;
	for (bj = 0; bj < nblocks; bj++)
	for (bi = 0; bi < nblocks; bi++)
	{
		starpu_matrix_data_register(&dataAp[bi+nblocks*bj], 0,
			(uintptr_t)matA[bi+nblocks*bj], size/nblocks,
			size/nblocks, size/nblocks, sizeof(TYPE));
	}

	unsigned i;
	for (i = 0; i < size; i++)
		ipiv[i] = i;

	struct piv_s *piv_description = malloc(nblocks*sizeof(struct piv_s));
	unsigned block;
	for (block = 0; block < nblocks; block++)
	{
		piv_description[block].piv = ipiv;
		piv_description[block].first = block * (size / nblocks);
		piv_description[block].last = (block + 1) * (size / nblocks);
	}

	double timing;
	int ret = dw_codelet_facto_pivot(dataAp, piv_description, nblocks, get_block_with_no_striding, &timing);

	FPRINTF(stderr, "Computation took (in ms)\n");
	FPRINTF(stderr, "%2.2f\n", timing/1000);

	unsigned n = starpu_matrix_get_nx(dataAp[0])*nblocks;
	double flop = (2.0f*n*n*n)/3.0f;
	FPRINTF(stderr, "Synthetic GFlops : %2.2f\n", (flop/timing/1000.0f));

	for (bj = 0; bj < nblocks; bj++)
	for (bi = 0; bi < nblocks; bi++)
	{
		starpu_data_unregister(dataAp[bi+nblocks*bj]);
	}
	free(dataAp);
	return ret;
}
