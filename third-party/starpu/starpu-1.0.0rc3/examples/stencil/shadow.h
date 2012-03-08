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

/* Perform replication of data on X and Y edges, to fold the domain on 
   itself through mere replication of the source state. */

/* TODO: rather use a dummy for loop, to assign the job to the threads that will work on it? */
	if (idy == 0)
		for (z = i-1 + idz; z < nz-(i-1); z += stepz)
			for (x = K + idx; x < nx-K; x += stepx)
			{
				unsigned index = x+z*ldz;
				ptr[index+(K-1)*ldy] = ptr[index+(ny-K-1)*ldy];
				ptr[index+(ny-K)*ldy] = ptr[index+K*ldy];
			}

	if (idx == 0)
		for (z = i-1 + idz; z < nz-(i-1); z += stepz)
			for (y = K + idy; y < ny-K; y += stepy)
			{
				unsigned index = y*ldy+z*ldz;
				ptr[(K-1)+index] = ptr[(nx-K-1)+index];
				ptr[(nx-K)+index] = ptr[K+index];
			}

	if (idx == 0 && idy == 0)
		for (z = i-1 + idz; z < nz-(i-1); z += stepz)
		{
			unsigned index = z*ldz;
			ptr[K-1+(K-1)*ldy+index] = ptr[(nx-K-1)+(ny-K-1)*ldy+index];
			ptr[(nx-K)+(K-1)*ldy+index] = ptr[K+(ny-K-1)*ldy+index];
			ptr[(K-1)+(ny-K)*ldy+index] = ptr[(nx-K-1)+K*ldy+index];
			ptr[(nx-K)+(ny-K)*ldy+index] = ptr[K+K*ldy+index];
		}
