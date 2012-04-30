/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

static int create_task_11(starpu_data_handle_t dataA, unsigned k)
{
	int ret;
	struct starpu_task *task = starpu_task_create();
	task->cl = &cl11;

	/* which sub-data is manipulated ? */
	task->handles[0] = starpu_data_get_sub_data(dataA, 2, k, k);

	/* this is an important task */
	if (!no_prio)
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_12(starpu_data_handle_t dataA, unsigned k, unsigned j)
{
	int ret;
	struct starpu_task *task = starpu_task_create();
	task->cl = &cl12;

	/* which sub-data is manipulated ? */
	task->handles[0] = starpu_data_get_sub_data(dataA, 2, k, k);
	task->handles[1] = starpu_data_get_sub_data(dataA, 2, j, k);

	if (!no_prio && (j == k+1))
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_21(starpu_data_handle_t dataA, unsigned k, unsigned i)
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &cl21;

	/* which sub-data is manipulated ? */
	task->handles[0] = starpu_data_get_sub_data(dataA, 2, k, k);
	task->handles[1] = starpu_data_get_sub_data(dataA, 2, k, i);

	if (!no_prio && (i == k+1))
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

static int create_task_22(starpu_data_handle_t dataA, unsigned k, unsigned i, unsigned j)
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &cl22;

	/* which sub-data is manipulated ? */
	task->handles[0] = starpu_data_get_sub_data(dataA, 2, k, i);
	task->handles[1] = starpu_data_get_sub_data(dataA, 2, j, k);
	task->handles[2] = starpu_data_get_sub_data(dataA, 2, j, i);

	if (!no_prio &&  (i == k + 1) && (j == k +1) )
		task->priority = STARPU_MAX_PRIO;

	ret = starpu_task_submit(task);
	if (ret != -ENODEV) STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return ret;
}

/*
 *	code to bootstrap the factorization
 */

static int dw_codelet_facto_v3(starpu_data_handle_t dataA, unsigned nblocks)
{
	struct timeval start;
	struct timeval end;
	int ret;

	/* create all the DAG nodes */
	unsigned i,j,k;

	gettimeofday(&start, NULL);

	for (k = 0; k < nblocks; k++)
	{
		ret = create_task_11(dataA, k);
		if (ret == -ENODEV) return ret;

		for (i = k+1; i<nblocks; i++)
		{
		     ret = create_task_12(dataA, k, i);
		     if (ret == -ENODEV) return ret;
		     ret = create_task_21(dataA, k, i);
		     if (ret == -ENODEV) return ret;
		}

		for (i = k+1; i<nblocks; i++)
		     for (j = k+1; j<nblocks; j++) {
			  ret = create_task_22(dataA, k, i, j);
			  if (ret == -ENODEV) return ret;
		     }
	}

	/* stall the application until the end of computations */
	starpu_task_wait_for_all();

	gettimeofday(&end, NULL);

	double timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
	FPRINTF(stderr, "Computation took (in ms)\n");
	FPRINTF(stdout, "%2.2f\n", timing/1000);

	unsigned n = starpu_matrix_get_nx(dataA);
	double flop = (2.0f*n*n*n)/3.0f;
	FPRINTF(stderr, "Synthetic GFlops : %2.2f\n", (flop/timing/1000.0f));
	return 0;
}

int STARPU_LU(lu_decomposition)(TYPE *matA, unsigned size, unsigned ld, unsigned nblocks)
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

	int ret = dw_codelet_facto_v3(dataA, nblocks);

	/* gather all the data */
	starpu_data_unpartition(dataA, 0);
	starpu_data_unregister(dataA);
	return ret;
}
