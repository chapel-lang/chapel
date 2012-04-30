/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
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

#define NX    5
#define NY    4
#define PARTS 2

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

void cpu_func(void *buffers[], void *cl_arg)
{
        unsigned i, j;
        int *factor = (int *) cl_arg;

        /* length of the matrix */
        unsigned nx = STARPU_MATRIX_GET_NX(buffers[0]);
        unsigned ny = STARPU_MATRIX_GET_NY(buffers[0]);
        unsigned ld = STARPU_MATRIX_GET_LD(buffers[0]);
        /* local copy of the matrix pointer */
        int *val = (int *)STARPU_MATRIX_GET_PTR(buffers[0]);

        for(j=0; j<ny ; j++)
	{
                for(i=0; i<nx ; i++)
                        val[(j*ld)+i] = *factor;
        }
}

int main(int argc, char **argv)
{
	unsigned i, j, n=1;
        int matrix[NX*NY];
	int ret;

        FPRINTF(stderr,"IN  Matrix: \n");
        for(j=0 ; j<NY ; j++)
	{
                for(i=0 ; i<NX ; i++)
		{
                        matrix[(j*NX)+i] = n++;
                        FPRINTF(stderr, "%2d ", matrix[(j*NX)+i]);
                }
                FPRINTF(stderr,"\n");
        }
        FPRINTF(stderr,"\n");

        starpu_data_handle_t handle;
        struct starpu_codelet cl =
	{
                .where = STARPU_CPU,
                .cpu_funcs = {cpu_func, NULL},
                .nbuffers = 1,
		.modes = {STARPU_RW}
        };

        ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* Declare data to StarPU */
	starpu_matrix_data_register(&handle, 0, (uintptr_t)matrix, NX, NX, NY, sizeof(matrix[0]));

        /* Partition the matrix in PARTS sub-matrices */
	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func,
		.nchildren = PARTS
	};
	starpu_data_partition(handle, &f);

        /* Submit a task on each sub-vector */
	for (i=0; i<starpu_data_get_nb_children(handle); i++)
	{
                struct starpu_task *task = starpu_task_create();
                int factor = i;
		task->handles[0] = starpu_data_get_sub_data(handle, 1, i);
                task->cl = &cl;
                task->synchronous = 1;
                task->cl_arg = &factor;
                task->cl_arg_size = sizeof(factor);

		ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

        /* Unpartition the data, unregister it from StarPU and shutdown */
	starpu_data_unpartition(handle, 0);
        starpu_data_unregister(handle);
	starpu_shutdown();

        /* Print result matrix */
        FPRINTF(stderr,"OUT Matrix: \n");
        for(j=0 ; j<NY ; j++)
	{
                for(i=0 ; i<NX ; i++)
		{
                        FPRINTF(stderr, "%2d ", matrix[(j*NX)+i]);
                }
                FPRINTF(stderr,"\n");
        }
        FPRINTF(stderr,"\n");

	return 0;

enodev:
	starpu_shutdown();
	return 77;
}
