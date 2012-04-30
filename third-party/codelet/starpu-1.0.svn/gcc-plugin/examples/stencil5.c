/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Centre National de la Recherche Scientifique
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef STARPU_GCC_PLUGIN
# error must be compiled with the StarPU GCC plug-in
#endif

/* Definition of the StarPU task and its CPU implementation.  */
static void stencil5(float *xy, const float *xm1y, const float *xp1y, const float *xym1, const float *xyp1)
	__attribute__ ((task));

static void stencil5_cpu(float *xy, const float *xm1y, const float *xp1y, const float *xym1, const float *xyp1)
	__attribute__ ((task_implementation ("cpu", stencil5)));

static void stencil5_cpu(float *xy, const float *xm1y, const float *xp1y, const float *xym1, const float *xyp1)
{
	*xy = (*xy + *xm1y + *xp1y + *xym1 + *xyp1) / 5;
}

#define NITER_DEF 20000
#define X         10
#define Y         10

int display = 0;
int niter = NITER_DEF;

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-iter") == 0) {
			char *argptr;
			niter = strtol(argv[++i], &argptr, 10);
		}
		if (strcmp(argv[i], "-display") == 0) {
			display = 1;
		}
	}
}

static float my_rand (void)
{
	return (float) rand () / (float) RAND_MAX;
}

int main(int argc, char **argv)
{
        int x, y;
        float mean=0;
        float matrix[X][Y];

        parse_args(argc, argv);

	srand (time (NULL));
        for(x = 0; x < X; x++) {
                for (y = 0; y < Y; y++) {
                        matrix[x][y] = my_rand () * 100;
                        mean += matrix[x][y];
                }
        }
        mean /= (x*y);

        if (display) {
                fprintf(stdout, "mean=%f\n", mean);
                for(x = 0; x < X; x++) {
                        for (y = 0; y < Y; y++) {
                                fprintf(stdout, "%3f ", matrix[x][y]);
                        }
                        fprintf(stdout, "\n");
                }
        }

#pragma starpu initialize

        for(x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
#pragma starpu register &matrix[x][y] 1
		}
	}

	while(niter--) {
                for (x = 1; x < X-1; x++) {
                        for (y = 1; y < Y-1; y++) {
                                stencil5(&matrix[x][y], &matrix[x-1][y], &matrix[x+1][y],
					 &matrix[x][y-1], &matrix[x][y+1]);
                        }
                }
        }

#pragma starpu wait

        for(x = 0; x < X; x++) {
                for (y = 0; y < Y; y++) {
#pragma starpu unregister &matrix[x][y]
                }
        }

#pragma starpu shutdown

        if (display) {
                fprintf(stdout, "mean=%f\n", mean);
                for(x = 0; x < X; x++) {
                        for (y = 0; y < Y; y++) {
                                fprintf(stdout, "%3f ", matrix[x][y]);
                        }
                        fprintf(stdout, "\n");
                }
        }

	return EXIT_SUCCESS;
}
