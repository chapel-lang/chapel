/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 Institut National de Recherche en Informatique et Automatique
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
#ifndef STARPU_GCC_PLUGIN
# error must be compiled with the StarPU GCC plug-in
#endif

/* This examples showcases features of the StarPU GCC plug-in.  It defines a
   "vector scaling" task with multiple CPU implementations, an OpenCL
   implementation, and a CUDA implementation.   */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


/* Declare and define the standard CPU implementation.  */

static void vector_scal (unsigned int size, float vector[size], float factor)
  __attribute__ ((task));

static void vector_scal_cpu (unsigned int size, float vector[size], float factor)
  __attribute__ ((task_implementation ("cpu", vector_scal)));

static void
vector_scal_cpu (unsigned int size, float vector[size], float factor)
{
  unsigned int i;
  for (i = 0; i < size; i++)
    vector[i] *= factor;
}


#ifdef __SSE__
/* The SSE-capable CPU implementation.  */

#include <xmmintrin.h>

static void vector_scal_sse (unsigned int size, float vector[size], float factor)
  __attribute__ ((task_implementation ("cpu", vector_scal)));

static void
vector_scal_sse (unsigned int size, float vector[size], float factor)
{
  unsigned int n_iterations = size / 4;

  __m128 *VECTOR = (__m128 *) vector;
  __m128 _FACTOR __attribute__ ((aligned (16)));
  _FACTOR = _mm_set1_ps (factor);

  unsigned int i;
  for (i = 0; i < n_iterations; i++)
    VECTOR[i] = _mm_mul_ps (_FACTOR, VECTOR[i]);

  unsigned int remainder = size % 4;
  if (remainder != 0)
    {
      unsigned int start = 4 * n_iterations;
      for (i = start; i < start + remainder; ++i)
	vector[i] = factor * vector[i];
    }
}
#endif /* __SSE__ */


/* Declaration and definition of the OpenCL implementation.  */

#ifdef STARPU_USE_OPENCL

#include <starpu_opencl.h>

/* The OpenCL programs, loaded from `main'.  */
static struct starpu_opencl_program cl_programs;

static void vector_scal_opencl (unsigned int size, float vector[size], float factor)
  __attribute__ ((task_implementation ("opencl", vector_scal)));

static void
vector_scal_opencl (unsigned int size, float vector[size], float factor)
{
  int id, devid, err;
  cl_kernel kernel;
  cl_command_queue queue;
  cl_event event;

  cl_mem val = (cl_mem) vector;

  id = starpu_worker_get_id ();
  devid = starpu_worker_get_devid (id);

  /* Prepare to invoke the kernel.  In the future, this will be largely
     automated.  */
  err = starpu_opencl_load_kernel (&kernel, &queue, &cl_programs,
				   "vector_mult_opencl", devid);
  if (err != CL_SUCCESS)
    STARPU_OPENCL_REPORT_ERROR (err);

  err = clSetKernelArg (kernel, 0, sizeof (val), &val);
  err |= clSetKernelArg (kernel, 1, sizeof (size), &size);
  err |= clSetKernelArg (kernel, 2, sizeof (factor), &factor);
  if (err)
    STARPU_OPENCL_REPORT_ERROR (err);

  size_t global = size, local = 1;
  err = clEnqueueNDRangeKernel (queue, kernel, 1, NULL, &global, &local, 0,
				NULL, &event);
  if (err != CL_SUCCESS)
    STARPU_OPENCL_REPORT_ERROR (err);

  clFinish (queue);
  starpu_opencl_collect_stats (event);
  clReleaseEvent (event);

  starpu_opencl_release_kernel (kernel);
}

#endif


#ifdef STARPU_USE_CUDA

/* Declaration of the CUDA implementation.  The definition itself is in the
   `.cu' file itself.  */

extern void vector_scal_cuda (unsigned int size, float vector[size], float factor)
  __attribute__ ((task_implementation ("cuda", vector_scal)));

#endif


#define EPSILON 1e-3
static bool
check (size_t size, float vector[size], float factor)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      if (fabs(vector[i] - i * factor) > i*factor*EPSILON)
        {
          fprintf(stderr, "%.2f != %.2f\n", vector[i], i*factor);
          return false;
        }
    }
  return true;
}


int
main (void)
{
  bool valid;

#pragma starpu initialize

#ifdef STARPU_USE_OPENCL
  starpu_opencl_load_opencl_from_file ("vector_scal_opencl_kernel.cl",
				       &cl_programs, "");
#endif

#define NX     0x100000
#define FACTOR 3.14

  {
    float vector[NX] __attribute__ ((heap_allocated));

#pragma starpu register vector

    size_t i;
    for (i = 0; i < NX; i++)
      vector[i] = (float) i;

    vector_scal (NX, vector, FACTOR);

#pragma starpu wait

#pragma starpu unregister vector

    valid = check (NX, vector, FACTOR);

  } /* VECTOR is automatically freed here.  */

#pragma starpu shutdown

  return valid ? EXIT_SUCCESS : EXIT_FAILURE;
}
