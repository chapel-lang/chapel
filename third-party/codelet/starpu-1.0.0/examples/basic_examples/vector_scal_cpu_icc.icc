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

/*
 * This example complements vector_scale.c: here we implement a CPU version.
 */

#include <starpu.h>
#ifdef __SSE__
#include <xmmintrin.h>
#endif

/* This kernel takes a buffer and scales it by a constant factor */
void scal_cpu_func(void *buffers[], void *cl_arg)
{
	unsigned i;
	float *factor = (float *) cl_arg;

	/*
	 * The "buffers" array matches the task->handles array: for instance
	 * task->handles[0] is a handle that corresponds to a data with
	 * vector "interface", so that the first entry of the array in the
	 * codelet  is a pointer to a structure describing such a vector (ie.
	 * struct starpu_vector_interface *). Here, we therefore manipulate
	 * the buffers[0] element as a vector: nx gives the number of elements
	 * in the array, ptr gives the location of the array (that was possibly
	 * migrated/replicated), and elemsize gives the size of each elements.
	 */

	struct starpu_vector_interface *vector = (struct starpu_vector_interface *) buffers[0];

	/* length of the vector */
	unsigned n = STARPU_VECTOR_GET_NX(vector);

	/* get a pointer to the local copy of the vector : note that we have to
	 * cast it in (float *) since a vector could contain any type of
	 * elements so that the .ptr field is actually a uintptr_t */
	float *val = (float *)STARPU_VECTOR_GET_PTR(vector);

	/* scale the vector */
	for (i = 0; i < n; i++)
		val[i] *= *factor;
}

#ifdef __SSE__
void scal_sse_func(void *buffers[], void *cl_arg)
{
	float *vector = (float *) STARPU_VECTOR_GET_PTR(buffers[0]);
	unsigned int n = STARPU_VECTOR_GET_NX(buffers[0]);
	unsigned int n_iterations = n/4;

	__m128 *VECTOR = (__m128*) vector;
	__m128 FACTOR __attribute__((aligned(16)));
	float factor = *(float *) cl_arg;
	FACTOR = _mm_set1_ps(factor);

	unsigned int i;	
	for (i = 0; i < n_iterations; i++)
		VECTOR[i] = _mm_mul_ps(FACTOR, VECTOR[i]);

	unsigned int remainder = n%4;
	if (remainder != 0)
	{
		unsigned int start = 4 * n_iterations;
		for (i = start; i < start+remainder; ++i)
		{
			vector[i] = factor * vector[i];
		}
	}
}
#endif
