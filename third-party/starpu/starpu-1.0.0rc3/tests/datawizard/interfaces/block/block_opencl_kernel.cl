/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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
__kernel void block_opencl(__global int *block,
			   int nx, int ny, int nz,
			   int ldy, int ldz,
			   int factor, __global int *err)
{
        const int id = get_global_id(0);
	if (id > 0)
		return;

	unsigned int i, j, k;
	int val = 0;
	for (k = 0; k < nz; k++)
	{
		for (j = 0; j < ny; j++)
		{
			for (i = 0; i < nx; i++)
			{
                                if (block[(k*ldz)+(j*ldy)+i] != factor * val)
				{
					*err = 1;
					return;
				}
				else
				{
					block[(k*ldz)+(j*ldy)+i] *= -1;
					val++;
				}
			}
		}
	}
}
