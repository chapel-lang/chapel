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

#include <starpu.h>

void cpu_codelet(void *descr[], void *_args)
{
	float *block = (float *)STARPU_BLOCK_GET_PTR(descr[0]);
	int nx = (int)STARPU_BLOCK_GET_NX(descr[0]);
	int ny = (int)STARPU_BLOCK_GET_NY(descr[0]);
	int nz = (int)STARPU_BLOCK_GET_NZ(descr[0]);
        unsigned ldy = STARPU_BLOCK_GET_LDY(descr[0]);
        unsigned ldz = STARPU_BLOCK_GET_LDZ(descr[0]);
        float *multiplier = (float *)_args;
        unsigned i, j, k;

        for(k=0; k<nz ; k++)
	{
                for(j=0; j<ny ; j++)
		{
                        for(i=0; i<nx ; i++)
                                block[(k*ldz)+(j*ldy)+i] *= *multiplier;
                }
        }
}

