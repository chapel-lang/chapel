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

#include <starpu.h>
#include <mpi.h>

#define _DISPLAY(fmt, args ...) do { \
		int _display_rank; MPI_Comm_rank(MPI_COMM_WORLD, &_display_rank);	\
		fprintf(stderr, "[%d][%s] " fmt , _display_rank, __func__ ,##args); 	\
		fflush(stderr); } while(0)

/*
 *	Codelet to create a neutral element
 */
void init_cpu_func(void *descr[], void *cl_arg)
{
	long int *dot = (long int *)STARPU_VARIABLE_GET_PTR(descr[0]);
	*dot = 0;
	_DISPLAY("Init dot\n");
}

/*
 *	Codelet to perform the reduction of two elements
 */
void redux_cpu_func(void *descr[], void *cl_arg)
{
	long int *dota = (long int *)STARPU_VARIABLE_GET_PTR(descr[0]);
	long int *dotb = (long int *)STARPU_VARIABLE_GET_PTR(descr[1]);

	*dota = *dota + *dotb;
	_DISPLAY("Calling redux %ld=%ld+%ld\n", *dota, *dota-*dotb, *dotb);
}

/*
 *	Dot product codelet
 */
void dot_cpu_func(void *descr[], void *cl_arg)
{
	long int *local_x = (long int *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned n = STARPU_VECTOR_GET_NX(descr[0]);

	long int *dot = (long int *)STARPU_VARIABLE_GET_PTR(descr[1]);

//	_DISPLAY("Before dot=%ld (adding %d elements...)\n", *dot, n);
	unsigned i;
	for (i = 0; i < n; i++)
	{
//		_DISPLAY("Adding %ld\n", local_x[i]);
		*dot += local_x[i];
	}
//	_DISPLAY("After dot=%ld\n", *dot);
}

