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
#include <math.h>
#include "helper.h"

void func_cpu(void *descr[], __attribute__ ((unused)) void *_args)
{
	int node;
	int rank;

        starpu_codelet_unpack_args(_args, &node);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	FPRINTF(stderr, "Expected node: %d - Actual node: %d\n", node, rank);

	assert(node == rank);
}

struct starpu_codelet mycodelet_r_w =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 2,
	.modes = {STARPU_R, STARPU_W}
};

struct starpu_codelet mycodelet_rw_r =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 2,
	.modes = {STARPU_RW, STARPU_R}
};

struct starpu_codelet mycodelet_rw_rw =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 2,
	.modes = {STARPU_RW, STARPU_RW}
};

struct starpu_codelet mycodelet_w_r =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 2,
	.modes = {STARPU_W, STARPU_R}
};

struct starpu_codelet mycodelet_r_r =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 2,
	.modes = {STARPU_R, STARPU_R}
};

int main(int argc, char **argv)
{
        int ret, rank, size, err, node;
        int x0=32, x1=23;
        starpu_data_handle_t data_handlesx0;
        starpu_data_handle_t data_handlesx1;

	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	ret = starpu_mpi_initialize_extended(&rank, &size);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_initialize_extended");

        if (size != 2)
	{
		if (rank == 0) FPRINTF(stderr, "We need exactly 2 processes.\n");
                starpu_mpi_shutdown();
                starpu_shutdown();
                return STARPU_TEST_SKIPPED;
        }

        if (rank == 0)
	{
                starpu_variable_data_register(&data_handlesx0, 0, (uintptr_t)&x0, sizeof(x0));
                starpu_data_set_rank(data_handlesx0, rank);
		starpu_data_set_tag(data_handlesx0, 0);
                starpu_variable_data_register(&data_handlesx1, -1, (uintptr_t)NULL, sizeof(int));
                starpu_data_set_rank(data_handlesx1, 1);
		starpu_data_set_tag(data_handlesx1, 1);
        }
        else if (rank == 1)
	{
                starpu_variable_data_register(&data_handlesx1, 0, (uintptr_t)&x1, sizeof(x1));
                starpu_data_set_rank(data_handlesx1, rank);
		starpu_data_set_tag(data_handlesx1, 1);
                starpu_variable_data_register(&data_handlesx0, -1, (uintptr_t)NULL, sizeof(int));
                starpu_data_set_rank(data_handlesx0, 0);
		starpu_data_set_tag(data_handlesx0, 0);
        }

	node = starpu_data_get_rank(data_handlesx1);
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_r_w,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_R, data_handlesx0, STARPU_W, data_handlesx1,
				     0);
        assert(err == 0);

	node = starpu_data_get_rank(data_handlesx0);
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_rw_r,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_RW, data_handlesx0, STARPU_R, data_handlesx1,
				     0);
        assert(err == 0);

        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_rw_rw,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_RW, data_handlesx0, STARPU_RW, data_handlesx1,
				     0);
        assert(err == -EINVAL);

	node = 1;
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_rw_rw,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_RW, data_handlesx0, STARPU_RW, data_handlesx1, STARPU_EXECUTE_ON_NODE, node,
				     0);
        assert(err == 0);

	node = 0;
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_rw_rw,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_RW, data_handlesx0, STARPU_RW, data_handlesx1, STARPU_EXECUTE_ON_NODE, node,
				     0);
        assert(err == 0);

	node = 0;
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_r_r,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_R, data_handlesx0, STARPU_R, data_handlesx1, STARPU_EXECUTE_ON_NODE, node,
				     0);
        assert(err == 0);

        /* Here the value specified by the property STARPU_EXECUTE_ON_NODE is
           going to overwrite the node even though the data model clearly specifies
           which node is going to execute the codelet */
	node = 0;
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_r_w,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_R, data_handlesx0, STARPU_W, data_handlesx1, STARPU_EXECUTE_ON_NODE, node,
				     0);
        assert(err == 0);

        /* Here the value specified by the property STARPU_EXECUTE_ON_NODE is
           going to overwrite the node even though the data model clearly specifies
           which node is going to execute the codelet */
	node = 0;
        err = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet_w_r,
				     STARPU_VALUE, &node, sizeof(node),
				     STARPU_W, data_handlesx0, STARPU_R, data_handlesx1, STARPU_EXECUTE_ON_NODE, node,
				     0);
        assert(err == 0);

	fprintf(stderr, "Waiting ...\n");
        starpu_task_wait_for_all();
	starpu_mpi_shutdown();
	starpu_shutdown();

	return 0;
}

