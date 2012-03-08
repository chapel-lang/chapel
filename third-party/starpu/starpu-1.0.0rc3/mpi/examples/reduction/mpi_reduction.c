/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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

extern void init_cpu_func(void *descr[], void *cl_arg);
extern void redux_cpu_func(void *descr[], void *cl_arg);
extern void dot_cpu_func(void *descr[], void *cl_arg);

static struct starpu_codelet init_codelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {init_cpu_func, NULL},
	.nbuffers = 1,
	.name = "init_codelet"
};

static struct starpu_codelet redux_codelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {redux_cpu_func, NULL},
	.nbuffers = 2,
	.name = "redux_codelet"
};

static struct starpu_codelet dot_codelet =
{
	.where = STARPU_CPU,
	.cpu_funcs = {dot_cpu_func, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_REDUX},
	.name = "dot_codelet"
};

/* Returns the MPI node number where data indexes index is */
int my_distrib(int x, int nb_nodes)
{
	return x % nb_nodes;
}

int main(int argc, char **argv)
{
        int my_rank, size, x, y;
        long int *vector;
	long int dot, sum=0;
        starpu_data_handle_t *handles;
	starpu_data_handle_t dot_handle;

	int nb_elements, step;

	int ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	starpu_mpi_initialize_extended(&my_rank, &size);

	nb_elements = size*8000;
	step = 4;

	vector = (long int *) malloc(nb_elements*sizeof(vector[0]));
        for(x = 0; x < nb_elements; x+=step)
	{
		int mpi_rank = my_distrib(x/step, size);
		if (mpi_rank == my_rank)
		{
			for(y=0 ; y<step ; y++)
			{
				vector[x+y] = x+y+1;
			}
		}
        }
	if (my_rank == 0) {
		dot = 14;
		sum = (nb_elements * (nb_elements + 1)) / 2;
		sum+= dot;
		starpu_variable_data_register(&dot_handle, 0, (uintptr_t)&dot, sizeof(dot));
	}
	else
	{
		starpu_variable_data_register(&dot_handle, -1, (uintptr_t)NULL, sizeof(dot));
	}


	handles = (starpu_data_handle_t *) malloc(nb_elements*sizeof(handles[0]));
        for(x = 0; x < nb_elements; x+=step)
	{
		int mpi_rank = my_distrib(x/step, size);
		if (mpi_rank == my_rank)
		{
			/* Owning data */
			starpu_vector_data_register(&handles[x], 0, (uintptr_t)&(vector[x]), step, sizeof(vector[0]));
		}
		else
		{
			starpu_vector_data_register(&handles[x], -1, (uintptr_t)NULL, step, sizeof(vector[0]));
		}
		if (handles[x])
		{
			starpu_data_set_rank(handles[x], mpi_rank);
			starpu_data_set_tag(handles[x], x);
		}
	}

	starpu_data_set_rank(dot_handle, 0);
	starpu_data_set_tag(dot_handle, nb_elements+1);
	starpu_data_set_reduction_methods(dot_handle, &redux_codelet, &init_codelet);

	for (x = 0; x < nb_elements; x+=step)
	{
		starpu_mpi_insert_task(MPI_COMM_WORLD,
				       &dot_codelet,
				       STARPU_R, handles[x],
				       STARPU_REDUX, dot_handle,
				       0);
	}
	starpu_mpi_redux_data(MPI_COMM_WORLD, dot_handle);

        fprintf(stderr, "Waiting ...\n");
        starpu_task_wait_for_all();

        for(x = 0; x < nb_elements; x+=step)
	{
		if (handles[x]) starpu_data_unregister(handles[x]);
	}
	if (dot_handle)
	{
		starpu_data_unregister(dot_handle);
	}
	free(vector);
	free(handles);

	starpu_mpi_shutdown();
	starpu_shutdown();

	if (my_rank == 0)
	{
                fprintf(stderr, "[%d] sum=%ld\n", my_rank, sum);
                fprintf(stderr, "[%d] dot=%ld\n", my_rank, dot);
		fprintf(stderr, "%s when computing reduction\n", (sum == dot) ? "Success" : "Error");
        }

	return 0;
}

