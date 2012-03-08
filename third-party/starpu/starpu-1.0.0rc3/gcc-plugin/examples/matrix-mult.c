/* StarPU --- Runtime system for heterogeneous multicore architectures.
   Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique
   Copyright (C) 2010 Sylvain Gault

   StarPU is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or (at
   your option) any later version.

   StarPU is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  */

/* The classical matrix multiplication example, implemented as a StarPU
   task invoked for different slices of the matrices.  Each invocation may
   of course execute in parallel.  */

#ifndef STARPU_GCC_PLUGIN
# error must be compiled with the StarPU GCC plug-in
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>


/* Definition of the StarPU task and its CPU implementation.  */

static void matmul (const float *A, const float *B, float *C,
		    size_t nx, size_t ny, size_t nz)
  __attribute__ ((task));

static void matmul_cpu (const float *A, const float *B, float *C,
			size_t nx, size_t ny, size_t nz)
  __attribute__ ((task_implementation ("cpu", matmul)));


static void
matmul_cpu (const float *A, const float *B, float *C,
	    size_t nx, size_t ny, size_t nz)
{
  size_t i, j, k;

  for (j = 0; j < ny; j++)
    for (i = 0; i < nx; i++)
      {
	for (k = 0; k < nz; k++)
	  C[j * nx + i] += A[j * nz + k] * B[k * nx + i];
      }
}



static void print_matrix (const float *M, size_t nslicesx,
			  size_t nslicesy, size_t bxdim,
			  size_t bydim)
  __attribute__ ((unused));

static void
print_matrix (const float *M, size_t nslicesx, size_t nslicesy, size_t bxdim,
	      size_t bydim)
{
  size_t a, b, i, j;

  for (b = 0; b < nslicesy; b++)
    for (j = 0; j < bydim; j++)
      {
	for (a = 0; a < nslicesx; a++)
	  {
	    for (i = 0; i < bxdim; i++)
	      printf ("%f ",
		      M[b * nslicesx * bxdim * bydim + j * bxdim +
			a * bxdim * bydim + i]);
	  }
	printf ("\b\n");
      }

  printf ("\n");
}


static float
my_rand (void)
{
  return (float) rand () / (float) RAND_MAX;
}

static double
mean (const double *v, size_t size)
{
  double sum = 0;
  size_t i;

  for (i = 0; i < size; i++)
    sum += v[i];

  return sum / size;
}


static double
stddev (const double *v, size_t size)
{
  double m = mean (v, size);
  double sqsum = 0;
  size_t i;

  for (i = 0; i < size; i++)
    sqsum += (v[i] - m) * (v[i] - m);

  return sqrt (sqsum / size);
}


int
main (int argc, char **argv)
{
  int mloop, nloop = 0;
  size_t i, j, k;
  size_t nslicesx;
  size_t nslicesy;
  size_t nslicesz;
  size_t xdim, ydim, zdim;
  size_t bxdim, bydim, bzdim;
  struct timeval start_all, end_all;
  struct timeval start_register, end_register;
  struct timeval start_tasks, end_tasks;
  struct timeval start_unregister, end_unregister;
  struct timeval start_compute, end_compute;


  if (argc < 4)
    {
      fprintf (stderr, "Usage: %s NLOOPS MATRIX-SIZE NSLICES\n", argv[0]);
      return EXIT_FAILURE;
    }

  mloop = nloop = atoi (argv[1]);
  zdim = ydim = xdim = atoi (argv[2]);
  nslicesz = nslicesy = nslicesx = atoi (argv[3]);
  bxdim = xdim / nslicesx;
  bydim = ydim / nslicesy;
  bzdim = zdim / nslicesz;

  if (xdim % nslicesx)
    {
      fprintf (stderr, "MATRIX-SIZE must be a multiple of NSLICES\n");
      return EXIT_FAILURE;
    }

  fprintf (stderr, "running %d loops with %lux%lux%lu matrices and %lux%lux%lu blocks...\n",
	   nloop,
	   xdim, ydim, zdim,
	   bxdim, bydim, bzdim);

  double computetime[nloop];
  double starttaskstime[nloop];

#pragma starpu initialize

  gettimeofday (&start_all, NULL);

  float *A, *B, *C;

  A = malloc (zdim * ydim * sizeof *A);
  B = malloc (xdim * zdim * sizeof *B);
  C = malloc (xdim * ydim * sizeof *C);

  srand (time (NULL));
  for (i = 0; i < zdim * ydim; i++)
    A[i] = my_rand () * 100;

  for (i = 0; i < xdim * zdim; i++)
    B[i] = my_rand () * 100;

#if 0
  print_matrix (A, nslicesz, nslicesy, bzdim, bydim);
  print_matrix (B, nslicesx, nslicesz, bxdim, bzdim);
#endif

  for (i = 0; i < xdim * ydim; i++)
    C[i] = 0;

  gettimeofday (&start_register, NULL);
  for (i = 0; i < nslicesy; i++)
    for (j = 0; j < nslicesz; j++)
#pragma starpu register &A[i*zdim*bydim + j*bzdim*bydim] (bzdim * bydim)

  for (i = 0; i < nslicesz; i++)
    for (j = 0; j < nslicesx; j++)
#pragma starpu register &B[i*xdim*bzdim + j*bxdim*bzdim] (bxdim * bzdim)

  for (i = 0; i < nslicesy; i++)
    for (j = 0; j < nslicesx; j++)
#pragma starpu register &C[i*xdim*bydim + j*bxdim*bydim] (bxdim * bydim)


  gettimeofday (&end_register, NULL);

  while (nloop--)
    {
      gettimeofday (&start_tasks, NULL);
      gettimeofday (&start_compute, NULL);
      for (i = 0; i < nslicesy; i++)
	for (j = 0; j < nslicesx; j++)
	  for (k = 0; k < nslicesz; k++)
	    /* Make an asynchronous call to `matmul', leading to the
	       instantiation of a StarPU task executing in parallel.  */
	    matmul (&A[i * zdim * bydim + k * bzdim * bydim],
		    &B[k * xdim * bzdim + j * bxdim * bzdim],
		    &C[i * xdim * bydim + j * bxdim * bydim], bxdim,
		    bydim, bzdim);

      gettimeofday (&end_tasks, NULL);
      starttaskstime[nloop] =
	(end_tasks.tv_sec - start_tasks.tv_sec) + (end_tasks.tv_usec -
						   start_tasks.tv_usec) /
	1000000.0;

      /* Wait for the asynchronous calls to complete.  */
#pragma starpu wait

      gettimeofday (&end_compute, NULL);
      computetime[nloop] =
	(end_compute.tv_sec - start_compute.tv_sec) + (end_compute.tv_usec -
						       start_compute.
						       tv_usec) / 1000000.0;
    }

#if 0
  print_matrix (C, nslicesx, nslicesy, bxdim, bydim);
#endif

  gettimeofday (&start_unregister, NULL);
  for (i = 0; i < nslicesy; i++)
    for (j = 0; j < nslicesz; j++)
#pragma starpu unregister &A[i*zdim*bydim + j*bzdim*bydim]


  for (i = 0; i < nslicesz; i++)
    for (j = 0; j < nslicesx; j++)
#pragma starpu unregister &B[i*xdim*bzdim + j*bxdim*bzdim]

  for (i = 0; i < nslicesy; i++)
    for (j = 0; j < nslicesx; j++)
#pragma starpu unregister &C[i*xdim*bydim + j*bxdim*bydim]

  gettimeofday (&end_unregister, NULL);
  gettimeofday (&end_all, NULL);

#pragma starpu shutdown

  printf ("total: %f\n",
	  (end_all.tv_sec - start_all.tv_sec) + (end_all.tv_usec -
						 start_all.tv_usec) /
	  1000000.0);
  printf ("register: %f\n",
	  (end_register.tv_sec - start_register.tv_sec) +
	  (end_register.tv_usec - start_register.tv_usec) / 1000000.0);
  printf ("unregister: %f\n",
	  (end_unregister.tv_sec - start_unregister.tv_sec) +
	  (end_unregister.tv_usec - start_unregister.tv_usec) / 1000000.0);

  printf ("mean task launch : %f\n", mean (starttaskstime, mloop));
  printf ("std task launch: %f\n", stddev (starttaskstime, mloop));

  printf ("mean compute: %f\n", mean (computetime, mloop));
  printf ("std compute: %f\n", stddev (computetime, mloop));
  printf ("Compute performance : %f GFLOPS\n",
	  .002 * xdim * ydim * zdim / (mean (computetime, mloop) * 1000000));

  return EXIT_SUCCESS;
}
