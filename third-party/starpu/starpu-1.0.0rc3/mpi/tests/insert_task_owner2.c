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

#include <starpu_mpi.h>
#include <starpu_mpi_datatype.h>
#include <math.h>
#include "helper.h"

void func_cpu(void *descr[], __attribute__ ((unused)) void *_args)
{
	int *x0 = (int *)STARPU_VARIABLE_GET_PTR(descr[0]);
	int *x1 = (int *)STARPU_VARIABLE_GET_PTR(descr[1]);
	int *x2 = (int *)STARPU_VARIABLE_GET_PTR(descr[2]);
	int *y = (int *)STARPU_VARIABLE_GET_PTR(descr[3]);

//        FPRINTF(stderr, "-------> CODELET VALUES: %d %d %d %d\n", *x0, *x1, *x2, *y);
//
//        *x2 = 45;
//        *y = 144;
//
        FPRINTF(stderr, "-------> CODELET VALUES: %d %d %d %d\n", *x0, *x1, *x2, *y);
        *y = (*x0 + *x1) * 100;
        *x1 = 12;
        *x2 = 24;
        *x0 = 36;
        FPRINTF(stderr, "-------> CODELET VALUES: %d %d %d %d\n", *x0, *x1, *x2, *y);
}

struct starpu_codelet mycodelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 4,
	.modes = {STARPU_R, STARPU_RW, STARPU_W, STARPU_W}
};

int main(int argc, char **argv)
{
        int rank, size, err;
        int x[3], y=0;
        int i, ret;
        starpu_data_handle_t data_handles[4];

	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	ret = starpu_mpi_initialize_extended(&rank, &size);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_initialize_extended");

        if (rank == 0)
	{
                for(i=0 ; i<3 ; i++)
		{
                        x[i] = 10*(i+1);
                        starpu_variable_data_register(&data_handles[i], 0, (uintptr_t)&x[i], sizeof(x[i]));
                }
                y = -1;
                starpu_variable_data_register(&data_handles[3], -1, (uintptr_t)NULL, sizeof(int));
        }
        else if (rank == 1)
	{
                for(i=0 ; i<3 ; i++)
		{
                        x[i] = -1;
                        starpu_variable_data_register(&data_handles[i], -1, (uintptr_t)NULL, sizeof(int));
                }
                y=200;
                starpu_variable_data_register(&data_handles[3], 0, (uintptr_t)&y, sizeof(int));
        } else
	{
                for(i=0 ; i<4 ; i++)
                        starpu_variable_data_register(&data_handles[i], -1, (uintptr_t)NULL, sizeof(int));
	}
        FPRINTF(stderr, "[%d][init] VALUES: %d %d %d %d\n", rank, x[0], x[1], x[2], y);

	for(i=0 ; i<3 ; i++)
	{
		starpu_data_set_rank(data_handles[i], 0);
		starpu_data_set_tag(data_handles[i], i);
	}
	starpu_data_set_rank(data_handles[3], 1);
	starpu_data_set_tag(data_handles[3], 3);

        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet,
                                     STARPU_R, data_handles[0], STARPU_RW, data_handles[1],
                                     STARPU_W, data_handles[2],
                                     STARPU_W, data_handles[3],
                                     STARPU_EXECUTE_ON_NODE, 1, 0);
	STARPU_CHECK_RETURN_VALUE(err, "starpu_mpi_insert_task");
        starpu_task_wait_for_all();

        int *values = malloc(4 * sizeof(int *));
        for(i=0 ; i<4 ; i++)
	{
                starpu_mpi_get_data_on_node_detached(MPI_COMM_WORLD, data_handles[i], 0, NULL, NULL);
		if (rank == 0) {
			starpu_data_acquire(data_handles[i], STARPU_R);
			values[i] = *((int *)starpu_mpi_handle_to_ptr(data_handles[i]));
		}
        }
        FPRINTF(stderr, "[%d][local ptr] VALUES: %d %d %d %d\n", rank, values[0], values[1], values[2], values[3]);
        FPRINTF(stderr, "[%d][end] VALUES: %d %d %d %d\n", rank, x[0], x[1], x[2], y);

	starpu_mpi_shutdown();
	starpu_shutdown();

	return 0;
}

