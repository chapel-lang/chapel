/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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

#include "cholesky.h"
#include "cholesky_kernels.h"

#define __heap  __attribute__ ((heap_allocated))

/*
 *	code to bootstrap the factorization
 *	and construct the DAG
 */
static void dw_cholesky(unsigned nblocks, unsigned size, unsigned ld, float *matA[nblocks][nblocks])
{
	struct timeval start;
	struct timeval end;
        int x, y;

	/* create all the DAG nodes */
	unsigned i,j,k;

        for(x = 0; x < nblocks ;  x++) {
                for (y = 0; y < nblocks; y++) {
#pragma starpu register matA[x][y] size/nblocks*size/nblocks
		}
        }

	gettimeofday(&start, NULL);

	for (k = 0; k < nblocks; k++)
        {
#ifdef STARPU_DEVEL
#  warning deal with prio and models
#endif
//                int prio = STARPU_DEFAULT_PRIO;
//                if (!noprio) prio = STARPU_MAX_PRIO;

		chol_codelet_update_u11(matA[k][k], size/nblocks, ld);

		for (j = k+1; j<nblocks; j++)
		{
//                        prio = STARPU_DEFAULT_PRIO;
//                        if (!noprio&& (j == k+1)) prio = STARPU_MAX_PRIO;

			chol_codelet_update_u21(matA[k][k], matA[k][j], ld, ld, size/nblocks, size/nblocks);

			for (i = k+1; i<nblocks; i++)
			{
				if (i <= j)
                                {
//                                        prio = STARPU_DEFAULT_PRIO;
//                                        if (!noprio && (i == k + 1) && (j == k +1) ) prio = STARPU_MAX_PRIO;

					chol_codelet_update_u22(matA[k][i],
								matA[k][j],
								matA[i][j],
								size/nblocks, size/nblocks, size/nblocks, ld, ld, ld);
                                }
			}
		}
        }

#pragma starpu wait

        for(x = 0; x < nblocks ;  x++) {
                for (y = 0; y < nblocks; y++) {
#pragma starpu unregister matA[x][y]
                }
        }

	gettimeofday(&end, NULL);

	double timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
	fprintf(stderr, "Computation took (in ms)\n");
	fprintf(stdout, "%2.2f\n", timing/1000);
	double flop = (1.0f*size*size*size)/3.0f;
	fprintf(stderr, "Synthetic GFlops : %2.2f\n", (flop/timing/1000.0f));
}

int main(int argc, char **argv)
{
	/* create a simple definite positive symetric matrix example
	 *
	 *	Hilbert matrix : h(i,j) = 1/(i+j+1)
	 * */

	parse_args(argc, argv);

#ifdef STARPU_DEVEL
#  warning todo
#endif
//	struct starpu_conf conf;
//	starpu_conf_init(&conf);
//	conf.sched_policy_name = "heft";
//	conf.calibrate = 1;
#pragma starpu initialize

        starpu_helper_cublas_init();

	float *bmat[nblocks][nblocks] __heap;

	unsigned i,j,x,y;
        for(x=0 ; x<nblocks ; x++)
	{
                for(y=0 ; y<nblocks ; y++)
		{
                        starpu_malloc((void **)&bmat[x][y], BLOCKSIZE*BLOCKSIZE*sizeof(float));
			for (i = 0; i < BLOCKSIZE; i++)
			{
				for (j = 0; j < BLOCKSIZE; j++)
				{
                                        bmat[x][y][j +i*BLOCKSIZE] = (1.0f/(1.0f+(i+(x*BLOCKSIZE)+j+(y*BLOCKSIZE)))) + ((i+(x*BLOCKSIZE) == j+(y*BLOCKSIZE))?1.0f*size:0.0f);
					//mat[j +i*size] = ((i == j)?1.0f*size:0.0f);
				}
			}
		}
	}


        if (display) {
		for(y=0 ; y<nblocks ; y++)
		{
			for(x=0 ; x<nblocks ; x++)
			{
                                printf("Block %d,%d :\n", x, y);
				for (j = 0; j < BLOCKSIZE; j++)
				{
					for (i = 0; i < BLOCKSIZE; i++)
					{
						if (i <= j) {
							printf("%2.2f\t", bmat[y][x][j +i*BLOCKSIZE]);
						}
						else {
							printf(".\t");
						}
					}
					printf("\n");
				}
			}
		}
	}

	dw_cholesky(nblocks, size, size/nblocks, bmat);

        if (display) {
                printf("Results:\n");
		for(y=0 ; y<nblocks ; y++)
		{
			for(x=0 ; x<nblocks ; x++)
			{
                                printf("Block %d,%d :\n", x, y);
				for (j = 0; j < BLOCKSIZE; j++)
				{
					for (i = 0; i < BLOCKSIZE; i++)
					{
						if (i <= j) {
							printf("%2.2f\t", bmat[y][x][j +i*BLOCKSIZE]);
						}
						else {
							printf(".\t");
						}
					}
					printf("\n");
				}
			}
		}
	}

	float rmat[size * size] __heap;
        for(x=0 ; x<nblocks ; x++) {
                for(y=0 ; y<nblocks ; y++) {
                        for (i = 0; i < BLOCKSIZE; i++) {
                                for (j = 0; j < BLOCKSIZE; j++) {
                                        rmat[j+(y*BLOCKSIZE)+(i+(x*BLOCKSIZE))*size] = bmat[x][y][j +i*BLOCKSIZE];
                                }
                        }
                }
        }

	fprintf(stderr, "compute explicit LLt ...\n");
	for (j = 0; j < size; j++)
	{
		for (i = 0; i < size; i++)
		{
			if (i > j) {
				rmat[j+i*size] = 0.0f; // debug
			}
		}
	}
	float test_mat[size * size] __heap;
	SSYRK("L", "N", size, size, 1.0f,
	      rmat, size, 0.0f, test_mat, size);

	fprintf(stderr, "comparing results ...\n");
        if (display) {
                for (j = 0; j < size; j++)
		{
                        for (i = 0; i < size; i++)
			{
                                if (i <= j) {
                                        printf("%2.2f\t", test_mat[j +i*size]);
                                }
                                else {
                                        printf(".\t");
                                }
                        }
                        printf("\n");
                }
        }

	int correctness = 1;
        for(x = 0; x < nblocks ;  x++)
	{
                for (y = 0; y < nblocks; y++)
		{
			for (i = (size/nblocks)*x ; i < (size/nblocks)*x+(size/nblocks); i++)
                                {
                                        for (j = (size/nblocks)*y ; j < (size/nblocks)*y+(size/nblocks); j++)
						{
							if (i <= j)
								{
									float orig = (1.0f/(1.0f+i+j)) + ((i == j)?1.0f*size:0.0f);
									float err = abs(test_mat[j +i*size] - orig);
									if (err > 0.00001) {
										fprintf(stderr, "Error[%d, %d] --> %2.2f != %2.2f (err %2.2f)\n", i, j, test_mat[j +i*size], orig, err);
										correctness = 0;
										break;
									}
								}
						}
                                }
		}
        }

        for(x=0 ; x<nblocks ; x++)
	{
                for(y=0 ; y<nblocks ; y++)
		{
                        starpu_free((void *)bmat[x][y]);
		}
	}

        starpu_helper_cublas_shutdown();
#pragma starpu shutdown

	assert(correctness);
	return 0;
}
