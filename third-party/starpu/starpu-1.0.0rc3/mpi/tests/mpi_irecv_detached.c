/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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
#include <common/utils.h>
#include "helper.h"

#define NITER	2048
#define SIZE	16

float *tab;
starpu_data_handle_t tab_handle;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void callback(void *arg __attribute__((unused)))
{
	unsigned *received = arg;

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	*received = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
}


int main(int argc, char **argv)
{
	int ret, rank, size;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size != 2)
	{
		if (rank == 0)
			FPRINTF(stderr, "We need exactly 2 processes.\n");

		MPI_Finalize();
		return STARPU_TEST_SKIPPED;
	}

	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	ret = starpu_mpi_initialize();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_initialize");

	tab = malloc(SIZE*sizeof(float));

	starpu_vector_data_register(&tab_handle, 0, (uintptr_t)tab, SIZE, sizeof(float));

	unsigned nloops = NITER;
	unsigned loop;

	int other_rank = (rank + 1)%2;

	for (loop = 0; loop < nloops; loop++)
	{
		if (rank == 0)
		{
			starpu_mpi_send(tab_handle, other_rank, loop, MPI_COMM_WORLD);
		}
		else
		{
			int received = 0;
			starpu_mpi_irecv_detached(tab_handle, other_rank, loop, MPI_COMM_WORLD, callback, &received);

			_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
			while (!received)
				_STARPU_PTHREAD_COND_WAIT(&cond, &mutex);
			_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
		}
	}

	starpu_mpi_shutdown();
	starpu_shutdown();

	MPI_Finalize();

	return 0;
}
