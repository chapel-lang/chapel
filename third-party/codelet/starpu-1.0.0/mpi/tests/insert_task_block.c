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
	unsigned *matrix = (unsigned *)STARPU_MATRIX_GET_PTR(descr[0]);
	int nx = (int)STARPU_MATRIX_GET_NX(descr[0]);
	int ny = (int)STARPU_MATRIX_GET_NY(descr[0]);
	int ld = (int)STARPU_MATRIX_GET_LD(descr[0]);

        int i, j;
        unsigned sum=0;

	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < ny; j++)
		{
                        sum += matrix[i+j*ld];
                }
        }
	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < ny; j++)
		{
                        matrix[i+j*ld] = sum;///(nx*ny);
                }
        }
}

struct starpu_codelet mycodelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {func_cpu, NULL},
        .nbuffers = 1,
	.modes = {STARPU_RW}
};

#define SIZE       6
#define BLOCKS     3

/* Returns the MPI node number where data indexes index is */
int my_distrib(int x, int y, int nb_nodes)
{
        return x % nb_nodes;
}


int main(int argc, char **argv)
{
        int rank, size, x, y;
        int ret, value=0;
        unsigned matrix[SIZE*SIZE];
        starpu_data_handle_t data_handles[SIZE][SIZE];

	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	ret = starpu_mpi_initialize_extended(&rank, &size);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_initialize_extended");

        for(x = 0; x < SIZE; x++)
	{
                for (y = 0; y < SIZE; y++)
		{
                        matrix[x+y*SIZE] = rank*100 + value;
                        value++;
                }
        }
#if 1
        for(x = 0; x < SIZE; x++) {
                FPRINTF(stdout, "[%d] ", rank);
                for (y = 0; y < SIZE; y++) {
                        FPRINTF(stdout, "%3d ", matrix[x+y*SIZE]);
                }
                FPRINTF(stdout, "\n");
        }
#endif

        for(x = 0; x < BLOCKS ;  x++)
	{
                for (y = 0; y < BLOCKS; y++)
		{
                        int mpi_rank = my_distrib(x, y, size);
                        if (mpi_rank == rank)
			{
                                //FPRINTF(stderr, "[%d] Owning data[%d][%d]\n", rank, x, y);
                                starpu_matrix_data_register(&data_handles[x][y], 0, (uintptr_t)&(matrix[((SIZE/BLOCKS)*x) + ((SIZE/BLOCKS)*y) * SIZE]),
                                                            SIZE, SIZE/BLOCKS, SIZE/BLOCKS, sizeof(unsigned));
                        }
                        else if (rank == mpi_rank+1 || rank == mpi_rank-1)
			{
                                /* I don't own that index, but will need it for my computations */
                                //FPRINTF(stderr, "[%d] Neighbour of data[%d][%d]\n", rank, x, y);
                                starpu_matrix_data_register(&data_handles[x][y], -1, (uintptr_t)&(matrix[((SIZE/BLOCKS)*x) + ((SIZE/BLOCKS)*y) * SIZE]),
                                                            SIZE, SIZE/BLOCKS, SIZE/BLOCKS, sizeof(unsigned));
                        }
                        else
			{
                                /* I know it's useless to allocate anything for this */
                                data_handles[x][y] = NULL;
                        }
                        if (data_handles[x][y])
			{
                                starpu_data_set_rank(data_handles[x][y], mpi_rank);
                                starpu_data_set_tag(data_handles[x][y], (y*BLOCKS)+x);
			}
                }
        }

        for(x = 0; x < BLOCKS; x++)
	{
                for (y = 0; y < BLOCKS; y++)
		{
                        ret = starpu_mpi_insert_task(MPI_COMM_WORLD, &mycodelet,
						     STARPU_RW, data_handles[x][y],
						     0);
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_insert_task");

                }
        }

        FPRINTF(stderr, "Waiting ...\n");
        starpu_task_wait_for_all();

        for(x = 0; x < BLOCKS; x++)
	{
                for (y = 0; y < BLOCKS; y++)
		{
                        if (data_handles[x][y])
                                starpu_data_unregister(data_handles[x][y]);
                }
        }

	starpu_mpi_shutdown();
	starpu_shutdown();

#if 1
        for(x = 0; x < SIZE; x++)
	{
                FPRINTF(stdout, "[%d] ", rank);
                for (y = 0; y < SIZE; y++) {
                        FPRINTF(stdout, "%3d ", matrix[x+y*SIZE]);
                }
                FPRINTF(stdout, "\n");
        }
#endif

	return 0;
}
