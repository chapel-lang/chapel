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


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sched.h>
#include <cuda.h>
#include <cublas.h>
#include <cblas.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>

#include <sys/time.h>

int GPU_LD	=	2048;
int CPU_LD	=	2048;
int MATRIXSIZE	=	1024;
int pinned 	=	0;
int htod	=	0;
int ITER	=	100;

#define CPUBUFFERSIZE	(4*CPU_LD*CPU_LD)
#define GPUBUFFERSIZE	(4*GPU_LD*GPU_LD)

float *h_A;
void * d_A;

float *A, *B, *C;

unsigned cuda_initialized = 0;


static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-gpu-ld") == 0)
		{
		        char *argptr;
			GPU_LD = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-cpu-ld") == 0)
		{
		        char *argptr;
			CPU_LD = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-size") == 0)
		{
		        char *argptr;
			MATRIXSIZE = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-pin") == 0)
		{
			pinned = 1;
		}

		if (strcmp(argv[i], "-HtoD") == 0)
		{
			htod = 1;
		}

		if (strcmp(argv[i], "-iter") == 0)
		{
		        char *argptr;
			ITER = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-h") == 0)
		{
			printf("usage : %s [-pin] [-HtoD] [-size size] [-cpu-ld ld] [-gpu-ld ld] [-iter n]\n", argv[0]);
		}
	}

	STARPU_ASSERT(CPU_LD >= MATRIXSIZE);
	STARPU_ASSERT(GPU_LD >= MATRIXSIZE);
}



void bind_thread(int cpu)
{
	/* bind the thread to a cpu */
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu, &mask);

	sched_setaffinity(0, sizeof(cpu_set_t), &mask);
}

void benchmark_memcpy(void)
{
	unsigned count;

	struct timeval tv_start, tv_end;
	unsigned long long usecs;

	double bytes = 4.0*MATRIXSIZE*MATRIXSIZE*ITER;

	cublasInit();

	if (pinned)
	{
		cuMemAllocHost((void **)&h_A, CPUBUFFERSIZE);
	}
	else
	{
		h_A = malloc(CPUBUFFERSIZE);
	}

	STARPU_ASSERT(h_A);

	/* malloc a buffer on the device */
	cublasAlloc(GPU_LD*GPU_LD, sizeof(float), &d_A);
	STARPU_ASSERT(d_A);

	gettimeofday(&tv_start, NULL);	

	if (!pinned)
	{
		/* pageable memory  */
		if (!htod)
		{
			for (count = 0; count < ITER; count++)
			{
				cublasGetMatrix(MATRIXSIZE, MATRIXSIZE, sizeof(float),
					(void *)d_A, GPU_LD, h_A, CPU_LD);
				cuCtxSynchronize();
			}
		
		}
		else 
		{
			for (count = 0; count < ITER; count++)
			{
				cublasSetMatrix(MATRIXSIZE, MATRIXSIZE, sizeof(float),
					h_A, CPU_LD, (void *)d_A, GPU_LD);
				cuCtxSynchronize();
			}
		}
	}
	else
	{
		if (!htod)
		{
			for (count = 0; count < ITER; count++)
			{
				cublasGetMatrix(MATRIXSIZE, MATRIXSIZE, sizeof(float),
					d_A, GPU_LD, h_A, CPU_LD);
				cuCtxSynchronize();
			}
		}
		else
		{
			for (count = 0; count < ITER; count++)
			{
				cublasSetMatrix(MATRIXSIZE, MATRIXSIZE, sizeof(float),
					h_A, CPU_LD, d_A, GPU_LD);
				cuCtxSynchronize();
			}
		}
	
	}

	gettimeofday(&tv_end, NULL);
	usecs = (tv_end.tv_usec - tv_start.tv_usec) + 1000000*(tv_end.tv_sec - tv_start.tv_sec);
	printf("%2.2f\n", bytes/usecs);

	if (pinned)
	{
		cuMemFreeHost(&h_A);
	}
	else
	{
		free(h_A);
	}
}

int main(int argc, char **argv)
{
	parse_args(argc, argv);
	
	bind_thread(0);

//	printf("Memcpy alone\n");
	benchmark_memcpy();

	return EXIT_SUCCESS;
}
