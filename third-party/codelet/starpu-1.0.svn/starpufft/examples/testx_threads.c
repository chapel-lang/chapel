/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#include <complex.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include <starpu.h>

#include <starpu_config.h>
#include "starpufft.h"

#include <fftw3.h>

#define SIGN (-1)
/* #define SIGN (1) */

int main(int argc, char *argv[])
{
	int i;
	struct timeval begin, end;
	int size;
	size_t bytes;
	int n = 0, m = 0;
	_FFTW(plan) fftw_plan;
	double timing;
	char *num;
	int num_threads = 1;

	_FFTW(init_threads)();

	num = getenv("NUM_THREADS");
	if (num)
		num_threads = atoi(num);
	_FFTW(plan_with_nthreads)(num_threads);

	if (argc < 2 || argc > 3)
	{
		fprintf(stderr,"need one or two size of vector\n");
		exit(EXIT_FAILURE);
	}

	if (argc == 2)
	{
		n = atoi(argv[1]);

		/* 1D */
		size = n;
	}
	else if (argc == 3)
	{
		n = atoi(argv[1]);
		m = atoi(argv[2]);

		/* 2D */
		size = n * m;
	}
	else
	{
		assert(0);
	}

	bytes = size * sizeof(_FFTW(complex));

	_FFTW(complex) *in = _FFTW(malloc)(size * sizeof(*in));
	starpu_srand48(0);
	for (i = 0; i < size; i++)
		in[i] = starpu_drand48() + I * starpu_drand48();

	_FFTW(complex) *out_fftw = _FFTW(malloc)(size * sizeof(*out_fftw));

	if (argc == 2)
	{
		fftw_plan = _FFTW(plan_dft_1d)(n, in, out_fftw, SIGN, FFTW_ESTIMATE);

	}
	else if (argc == 3)
	{
		fftw_plan = _FFTW(plan_dft_2d)(n, m, in, out_fftw, SIGN, FFTW_ESTIMATE);
	}
	else
	{
		assert(0);
	}

	gettimeofday(&begin, NULL);
	_FFTW(execute)(fftw_plan);
	gettimeofday(&end, NULL);
	_FFTW(destroy_plan)(fftw_plan);
	timing = (double)((end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec));
	printf("FFTW with %d threads took %2.2f ms (%2.2f MB/s)\n\n", num_threads, timing/1000, bytes/(timing*num_threads));

	printf("\n");

	return EXIT_SUCCESS;
}
