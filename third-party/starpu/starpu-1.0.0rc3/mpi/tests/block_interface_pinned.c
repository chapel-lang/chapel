/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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
#include <stdlib.h>
#include "helper.h"

#define NITER	2048

#define BIGSIZE	64
#define SIZE	64

int main(int argc, char **argv)
{
	int ret, rank, size;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size < 2)
	{
		if (rank == 0)
			FPRINTF(stderr, "We need at least processes.\n");

		MPI_Finalize();
		return STARPU_TEST_SKIPPED;
	}

	/* We only use 2 nodes for that test */
	if (rank >= 2)
	{
		MPI_Finalize();
		return STARPU_TEST_SKIPPED;
	}

	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	ret = starpu_mpi_initialize();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_initialize");

	/* Node 0 will allocate a big block and only register an inner part of
	 * it as the block data, Node 1 will allocate a block of small size and
	 * register it directly. Node 0 and 1 will then exchange the content of
	 * their blocks. */

	float *block;
	starpu_data_handle_t block_handle;

	if (rank == 0)
	{
		starpu_malloc((void **)&block,
				BIGSIZE*BIGSIZE*BIGSIZE*sizeof(float));
		memset(block, 0, BIGSIZE*BIGSIZE*BIGSIZE*sizeof(float));

		/* fill the inner block */
		unsigned i, j, k;
		for (k = 0; k < SIZE; k++)
		for (j = 0; j < SIZE; j++)
		for (i = 0; i < SIZE; i++)
		{
			block[i + j*BIGSIZE + k*BIGSIZE*BIGSIZE] = 1.0f;
		}

		starpu_block_data_register(&block_handle, 0,
			(uintptr_t)block, BIGSIZE, BIGSIZE*BIGSIZE,
			SIZE, SIZE, SIZE, sizeof(float));
	}
	else /* rank == 1 */
	{
		starpu_malloc((void **)&block,
			SIZE*SIZE*SIZE*sizeof(float));
		memset(block, 0, SIZE*SIZE*SIZE*sizeof(float));

		starpu_block_data_register(&block_handle, 0,
			(uintptr_t)block, SIZE, SIZE*SIZE,
			SIZE, SIZE, SIZE, sizeof(float));
	}

	if (rank == 0)
	{
		MPI_Status status;

		ret = starpu_mpi_send(block_handle, 1, 0x42, MPI_COMM_WORLD);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_send");

		ret = starpu_mpi_recv(block_handle, 1, 0x1337, MPI_COMM_WORLD, &status);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_recv");

		/* check the content of the block */
		starpu_data_acquire(block_handle, STARPU_R);
		unsigned i, j, k;
		for (k = 0; k < SIZE; k++)
		for (j = 0; j < SIZE; j++)
		for (i = 0; i < SIZE; i++)
		{
			assert(block[i + j*BIGSIZE + k*BIGSIZE*BIGSIZE] == 33.0f);
		}
		starpu_data_release(block_handle);

	}
	else /* rank == 1 */
	{
		MPI_Status status;

		ret = starpu_mpi_recv(block_handle, 0, 0x42, MPI_COMM_WORLD, &status);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_recv");

		/* check the content of the block and modify it */
		ret = starpu_data_acquire(block_handle, STARPU_RW);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire");

		unsigned i, j, k;
		for (k = 0; k < SIZE; k++)
		for (j = 0; j < SIZE; j++)
		for (i = 0; i < SIZE; i++)
		{
			assert(block[i + j*SIZE + k*SIZE*SIZE] == 1.0f);
			block[i + j*SIZE + k*SIZE*SIZE] = 33.0f;
		}
		starpu_data_release(block_handle);

		ret = starpu_mpi_send(block_handle, 0, 0x1337, MPI_COMM_WORLD);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_send");

	}

	FPRINTF(stdout, "Rank %d is done\n", rank);
	fflush(stdout);

	starpu_mpi_shutdown();
	starpu_shutdown();

	MPI_Finalize();

	return 0;
}
