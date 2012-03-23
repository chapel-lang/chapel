/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#include "stencil.h"

/* Heart of the stencil computation: compute a new state from an old one. */

void life_update(int bz, const TYPE *old, TYPE *newp, int nx, int ny, int nz, int ldy, int ldz, int iter)
{
	unsigned x, y, z, num, alive;

	for (z = iter; z < nz - iter; z++)
	{
		for (y = K; y < ny - K; y++)
		{
			for (x = K; x < nx - K; x++)
			{
				num = 0
                                        + old[x+(y+1)*ldy+(z+0)*ldz]
                                        + old[x+(y+1)*ldy+(z+1)*ldz]
                                        + old[x+(y+0)*ldy+(z+1)*ldz]
                                        + old[x+(y-1)*ldy+(z+1)*ldz]
                                        + old[x+(y-1)*ldy+(z+0)*ldz]
                                        + old[x+(y-1)*ldy+(z-1)*ldz]
                                        + old[x+(y+0)*ldy+(z-1)*ldz]
                                        + old[x+(y+1)*ldy+(z-1)*ldz]
					;
				alive = old[x+y*ldy+z*ldz];
				alive = (alive && num == 2) || num == 3;
				newp[x+y*ldy+z*ldz] = alive;
			}
		}
	}
}
