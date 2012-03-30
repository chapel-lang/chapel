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

void stencil5_cpu(void *descr[], __attribute__ ((unused)) void *_args)
{
	unsigned *xy = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	unsigned *xm1y = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[1]);
	unsigned *xp1y = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[2]);
	unsigned *xym1 = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[3]);
	unsigned *xyp1 = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[4]);

        //        fprintf(stdout, "VALUES: %d %d %d %d %d\n", *xy, *xm1y, *xp1y, *xym1, *xyp1);
        *xy = (*xy + *xm1y + *xp1y + *xym1 + *xyp1) / 5;
}

struct starpu_codelet stencil5_cl =
{
	.where = STARPU_CPU,
	.cpu_funcs = {stencil5_cpu, NULL},
        .nbuffers = 5,
	.modes = {STARPU_RW, STARPU_R, STARPU_R, STARPU_R, STARPU_R}
};

#define NITER_DEF 500
#define X         20
#define Y         20

int display = 0;
int niter = NITER_DEF;

/* Returns the MPI node number where data indexes index is */
int my_distrib(int x, int y, int nb_nodes)
{
	/* Block distrib */
	return ((int)(x / sqrt(nb_nodes) + (y / sqrt(nb_nodes)) * sqrt(nb_nodes))) % nb_nodes;
}


static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-iter") == 0)
		{
			char *argptr;
			niter = strtol(argv[++i], &argptr, 10);
		}
		if (strcmp(argv[i], "-display") == 0)
		{
			display = 1;
		}
	}
}

int main(int argc, char **argv)
{
        int my_rank, size, x, y, loop;
        int value=0, mean=0;
        unsigned matrix[X][Y];
        starpu_data_handle_t data_handles[X][Y];

	int ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	starpu_mpi_initialize_extended(&my_rank, &size);
        parse_args(argc, argv);

        for(x = 0; x < X; x++)
	{
                for (y = 0; y < Y; y++)
		{
                        matrix[x][y] = (my_rank+1)*10 + value;
                        value++;
                        mean += matrix[x][y];
                }
        }
        mean /= value;

        for(x = 0; x < X; x++)
	{
                for (y = 0; y < Y; y++)
		{
                        int mpi_rank = my_distrib(x, y, size);
                        if (mpi_rank == my_rank)
			{
                                //fprintf(stderr, "[%d] Owning data[%d][%d]\n", my_rank, x, y);
                                starpu_variable_data_register(&data_handles[x][y], 0, (uintptr_t)&(matrix[x][y]), sizeof(unsigned));
                        }
			else if (my_rank == my_distrib(x+1, y, size) || my_rank == my_distrib(x-1, y, size)
			      || my_rank == my_distrib(x, y+1, size) || my_rank == my_distrib(x, y-1, size))
			{
                                /* I don't own that index, but will need it for my computations */
                                //fprintf(stderr, "[%d] Neighbour of data[%d][%d]\n", my_rank, x, y);
                                starpu_variable_data_register(&data_handles[x][y], -1, (uintptr_t)NULL, sizeof(unsigned));
                        }
                        else
			{
                                /* I know it's useless to allocate anything for this */
                                data_handles[x][y] = NULL;
                        }
                        if (data_handles[x][y])
			{
                                starpu_data_set_rank(data_handles[x][y], mpi_rank);
                                starpu_data_set_tag(data_handles[x][y], (y*X)+x);
			}
                }
        }

        for(loop=0 ; loop<niter; loop++)
	{
                for (x = 1; x < X-1; x++)
		{
                        for (y = 1; y < Y-1; y++)
			{
                                starpu_mpi_insert_task(MPI_COMM_WORLD, &stencil5_cl, STARPU_RW, data_handles[x][y],
                                                       STARPU_R, data_handles[x-1][y], STARPU_R, data_handles[x+1][y],
                                                       STARPU_R, data_handles[x][y-1], STARPU_R, data_handles[x][y+1],
                                                       0);
                        }
                }
        }
        fprintf(stderr, "Waiting ...\n");
        starpu_task_wait_for_all();

	starpu_mpi_shutdown();
	starpu_shutdown();

        if (display)
	{
                fprintf(stdout, "[%d] mean=%d\n", my_rank, mean);
                for(x = 0; x < X; x++)
		{
                        fprintf(stdout, "[%d] ", my_rank);
                        for (y = 0; y < Y; y++)
			{
                                fprintf(stdout, "%3d ", matrix[x][y]);
                        }
                        fprintf(stdout, "\n");
                }
        }

	return 0;
}
