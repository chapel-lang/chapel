/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <starpu.h>

static unsigned nblocks = 8192;
static unsigned entries_per_bock = 1024;

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

#define TYPE		double
#define TYPE_MAX	DBL_MAX
#define TYPE_MIN	DBL_MIN

static TYPE *x;
static starpu_data_handle_t *x_handles;

/* The first element (resp. second) stores the min element (resp. max). */
static TYPE minmax[2];
static starpu_data_handle_t minmax_handle;

/*
 *	Codelet to create a neutral element
 */

static void minmax_neutral_cpu_func(void *descr[], void *cl_arg)
{
	TYPE *array = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]);

	/* Initialize current min to the greatest possible value. */
	array[0] = TYPE_MAX;

	/* Initialize current max to the smallest possible value. */
	array[1] = TYPE_MIN;
}

static struct starpu_codelet minmax_init_codelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {minmax_neutral_cpu_func, NULL},
	.nbuffers = 1
};

/*
 *	Codelet to perform the reduction of two elements
 */

void minmax_redux_cpu_func(void *descr[], void *cl_arg)
{
	TYPE *array_dst = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[0]);
	TYPE *array_src = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[1]);

	/* Compute the min value */
	TYPE min_dst = array_dst[0];
	TYPE min_src = array_src[0];
	array_dst[0] = STARPU_MIN(min_dst, min_src);

	/* Compute the max value */
	TYPE max_dst = array_dst[1];
	TYPE max_src = array_src[1];
	array_dst[1] = STARPU_MAX(max_dst, max_src);
}

static struct starpu_codelet minmax_redux_codelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {minmax_redux_cpu_func, NULL},
	.nbuffers = 2
};

/*
 *	Compute max/min within a vector and update the min/max value
 */

void minmax_cpu_func(void *descr[], void *cl_arg)
{
	/* The array containing the values */
	TYPE *local_array = (TYPE *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	TYPE *minmax = (TYPE *)STARPU_VARIABLE_GET_PTR(descr[1]);

	TYPE local_min = minmax[0];
	TYPE local_max = minmax[1];

	/* Compute the min and the max elements in the array */
	unsigned i;
	for (i = 0; i < n; i++)
	{
		TYPE val = local_array[i];
		local_min = STARPU_MIN(local_min, val);
		local_max = STARPU_MAX(local_max, val);
	}

	minmax[0] = local_min;
	minmax[1] = local_max;
}

static struct starpu_codelet minmax_codelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {minmax_cpu_func, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_REDUX}
};

/*
 *	Tasks initialization
 */

int main(int argc, char **argv)
{
	unsigned long i;
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	unsigned long nelems = nblocks*entries_per_bock;
	size_t size = nelems*sizeof(TYPE);

	x = (TYPE *) malloc(size);
	x_handles = (starpu_data_handle_t *) calloc(nblocks, sizeof(starpu_data_handle_t));
	
	assert(x && x_handles);

	/* Initialize the vector with random values */
        starpu_srand48(0);
	for (i = 0; i < nelems; i++)
		x[i] = (TYPE)starpu_drand48();
	
	unsigned block;
	for (block = 0; block < nblocks; block++)
	{
		uintptr_t block_start = (uintptr_t)&x[entries_per_bock*block];
		starpu_vector_data_register(&x_handles[block], 0, block_start,
						entries_per_bock, sizeof(TYPE));
	}

	/* Initialize current min */
	minmax[0] = TYPE_MAX;

	/* Initialize current max */
	minmax[1] = TYPE_MIN;

	starpu_variable_data_register(&minmax_handle, 0, (uintptr_t)minmax, 2*sizeof(TYPE));

	/* Set the methods to define neutral elements and to perform the reduction operation */
	starpu_data_set_reduction_methods(minmax_handle, &minmax_redux_codelet, &minmax_init_codelet);

	for (block = 0; block < nblocks; block++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = &minmax_codelet;

		task->handles[0] = x_handles[block];
		task->handles[1] = minmax_handle;

		int ret = starpu_task_submit(task);
		if (ret)
		{
			STARPU_ASSERT(ret == -ENODEV);
			FPRINTF(stderr, "This test can only run on CPUs, but there are no CPU workers (this is not a bug).\n");
			return 77;
		}
	}

	for (block = 0; block < nblocks; block++)
	{
		starpu_data_unregister(x_handles[block]);
	}
	starpu_data_unregister(minmax_handle);

	FPRINTF(stderr, "Min : %e\n", minmax[0]);
	FPRINTF(stderr, "Max : %e\n", minmax[1]);

	STARPU_ASSERT(minmax[0] <= minmax[1]);

	free(x);
	free(x_handles);
	starpu_shutdown();

	return 0;
}
