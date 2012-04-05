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

/* Returns the MPI node number where data indexes index is */
int my_distrib(int x, int y, int nb_nodes)
{
        return (x+y) % nb_nodes;
}

void cpu_codelet(void *descr[], void *_args)
{
	float *block;
	unsigned nx = STARPU_MATRIX_GET_NY(descr[0]);
	unsigned ld = STARPU_MATRIX_GET_LD(descr[0]);
	unsigned i,j;
	int rank;
	float factor;

	block = (float *)STARPU_MATRIX_GET_PTR(descr[0]);
        starpu_codelet_unpack_args(_args, &rank);
	factor = block[0];

	//fprintf(stderr,"rank %d factor %f\n", rank, factor);
	for (j = 0; j < nx; j++)
	{
		for (i = 0; i < nx; i++)
		{
			//fprintf(stderr,"rank %d factor %f --> %f %f\n", rank, factor, block[j+i*ld], block[j+i*ld]*factor);
			block[j+i*ld] *= factor;
		}
	}
}

static struct starpu_codelet cl =
{
	.where = STARPU_CPU,
	.cpu_funcs = {cpu_codelet, NULL},
	.nbuffers = 1,
	.modes = {STARPU_RW},
};

int main(int argc, char **argv)
{
        int rank, nodes;
	float ***bmat = NULL;
        starpu_data_handle_t *data_handles;

	unsigned i,j,x,y;

	unsigned nblocks=4;
	unsigned block_size=2;
	unsigned size = nblocks*block_size;
	unsigned ld = size / nblocks;

	int ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	starpu_mpi_initialize_extended(&rank, &nodes);

	if (rank == 0)
	{
		/* Allocate the matrix */
		int block_number=10;
		bmat = malloc(nblocks * sizeof(float *));
		for(x=0 ; x<nblocks ; x++)
		{
			bmat[x] = malloc(nblocks * sizeof(float *));
			for(y=0 ; y<nblocks ; y++)
			{
				float value=0.0;
				starpu_malloc((void **)&bmat[x][y], block_size*block_size*sizeof(float));
				for (i = 0; i < block_size; i++)
				{
					for (j = 0; j < block_size; j++)
					{
						bmat[x][y][j +i*block_size] = block_number + value;
						value++;
					}
				}
				block_number += 10;
			}
		}
	}

#if 0
	// Print matrix
	if (rank == 0)
	{
		fprintf(stderr, "Input matrix\n");
		for(x=0 ; x<nblocks ; x++)
		{
			for(y=0 ; y<nblocks ; y++)
			{
				for (j = 0; j < block_size; j++)
				{
					for (i = 0; i < block_size; i++)
					{
						fprintf(stderr, "%2.2f\t", bmat[x][y][j+i*block_size]);
					}
					fprintf(stderr,"\n");
				}
				fprintf(stderr,"\n");
			}
		}
	}
#endif

	/* Allocate data handles and register data to StarPU */
        data_handles = malloc(nblocks*nblocks*sizeof(starpu_data_handle_t *));
        for(x = 0; x < nblocks ;  x++)
	{
                for (y = 0; y < nblocks; y++)
		{
			int mpi_rank = my_distrib(x, y, nodes);
			if (rank == 0)
			{
				starpu_matrix_data_register(&data_handles[x+y*nblocks], 0, (uintptr_t)bmat[x][y],
							    ld, size/nblocks, size/nblocks, sizeof(float));
			}
			else if ((mpi_rank == rank) || ((rank == mpi_rank+1 || rank == mpi_rank-1)))
			{
				/* I own that index, or i will need it for my computations */
				//fprintf(stderr, "[%d] Owning or neighbor of data[%d][%d]\n", rank, x, y);
				starpu_matrix_data_register(&data_handles[x+y*nblocks], -1, (uintptr_t)NULL,
							    ld, size/nblocks, size/nblocks, sizeof(float));
			}
			else
			{
				/* I know it's useless to allocate anything for this */
				data_handles[x+y*nblocks] = NULL;
			}
                        if (data_handles[x+y*nblocks])
			{
                                starpu_data_set_rank(data_handles[x+y*nblocks], mpi_rank);
                                starpu_data_set_tag(data_handles[x+y*nblocks], (y*nblocks)+x);
			}
                }
        }

	/* Scatter the matrix among the nodes */
	starpu_mpi_scatter_detached(data_handles, nblocks*nblocks, 0, MPI_COMM_WORLD);

	/* Calculation */
	for(x = 0; x < nblocks*nblocks ;  x++)
	{
		if (data_handles[x])
		{
			int owner = starpu_data_get_rank(data_handles[x]);
			if (owner == rank)
			{
				//fprintf(stderr,"[%d] Computing on data[%d]\n", rank, x);
				starpu_insert_task(&cl,
						   STARPU_VALUE, &rank, sizeof(rank),
						   STARPU_RW, data_handles[x],
						   0);
			}
		}
	}

	/* Gather the matrix on main node */
	starpu_mpi_gather_detached(data_handles, nblocks*nblocks, 0, MPI_COMM_WORLD);

	/* Unregister matrix from StarPU */
	for(x=0 ; x<nblocks*nblocks ; x++)
	{
		if (data_handles[x])
		{
			starpu_data_unregister(data_handles[x]);
		}
	}

#if 0
	// Print matrix
	if (rank == 0)
	{
		fprintf(stderr, "Output matrix\n");
		for(x=0 ; x<nblocks ; x++)
		{
			for(y=0 ; y<nblocks ; y++)
			{
				for (j = 0; j < block_size; j++)
				{
					for (i = 0; i < block_size; i++)
					{
						fprintf(stderr, "%2.2f\t", bmat[x][y][j+i*block_size]);
					}
					fprintf(stderr,"\n");
				}
				fprintf(stderr,"\n");
			}
		}
	}
#endif

	// Free memory
        free(data_handles);
	if (rank == 0)
	{
		for(x=0 ; x<nblocks ; x++)
		{
			for(y=0 ; y<nblocks ; y++)
			{
				starpu_free((void *)bmat[x][y]);
			}
			free(bmat[x]);
		}
		free(bmat);
	}


	starpu_mpi_shutdown();
	starpu_shutdown();
	return 0;
}
