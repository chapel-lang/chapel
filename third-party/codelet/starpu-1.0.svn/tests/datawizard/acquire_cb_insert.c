/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
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
#include "../helper.h"

#define N 16
#define M 4
#define X 2

void which_index_cpu(void *descr[], void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	int *x0 = (int *)STARPU_VARIABLE_GET_PTR(descr[0]);

	/* A real case would actually compute something */
	*x0 = X;
}

struct starpu_codelet which_index =
{
	.where = STARPU_CPU,
	.cpu_funcs = {which_index_cpu, NULL},
        .nbuffers = 1,
	.modes = {STARPU_W}
};

void work_cpu(void *descr[], void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	int i, n = STARPU_VECTOR_GET_NX(descr[0]);
	float *x0 = (float *)STARPU_VECTOR_GET_PTR(descr[0]);

	for (i = 0; i < n; i++)
		x0[i] = i + 1;
}

struct starpu_codelet work =
{
	.where = STARPU_CPU,
	.cpu_funcs = {work_cpu, NULL},
        .nbuffers = 1,
	.modes = {STARPU_W}
};

static int x;
static starpu_data_handle_t x_handle, f_handle;

void callback(void *arg)
{
	starpu_insert_task(&work, STARPU_W, starpu_data_get_sub_data(f_handle, 1, x), 0);
	starpu_data_release(x_handle);
}

int main(int argc, char **argv)
{
        int i, ret;
	float *f;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* Declare x */
	starpu_variable_data_register(&x_handle, 0, (uintptr_t)&x, sizeof(x));

	/* Allocate and Declare f */
	ret = starpu_malloc((void**)&f, N * sizeof(*f));
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_malloc");
	memset(f, 0, N * sizeof(*f));
	starpu_vector_data_register(&f_handle, 0, (uintptr_t)f, N, sizeof(*f));

	/* Partition f */
	struct starpu_data_filter filter =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = M,
	};
	starpu_data_partition(f_handle, &filter);

	/* Compute which portion we will work on */
        ret = starpu_insert_task(&which_index, STARPU_W, x_handle, 0);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	/* And submit the corresponding task */
#ifdef __GCC__
	STARPU_DATA_ACQUIRE_CB(
			x_handle,
			STARPU_R,
			starpu_insert_task(&work, STARPU_W, starpu_data_get_sub_data(f_handle, 1, x), 0)
			);
#else
	starpu_data_acquire_cb(x_handle, STARPU_W, callback, NULL);
#endif

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");
	starpu_data_unpartition(f_handle, 0);
	starpu_data_unregister(f_handle);
	starpu_data_unregister(x_handle);

        FPRINTF(stderr, "VALUES: %d", x);
        for(i=0 ; i<N ; i++)
	{
		FPRINTF(stderr, " %f", f[i]);
        }
	FPRINTF(stderr, "\n");

	ret = EXIT_SUCCESS;
	if (f[X*(N/M)] != 1 || f[X*(N/M)+1] != 2 ||
	    f[X*(N/M)+2] != 3 || f[X*(N/M)+3] != 4)
		ret = EXIT_FAILURE;

	starpu_free(f);
	starpu_shutdown();
	STARPU_RETURN(ret);

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
