/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 INRIA
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

#include "custom_types.h"

/*
 * The first n/2 values of x are actual xs. The last N/2 values are ys.
 */
__kernel void custom_opencl_conversion(__global struct point *aop,
				       __global float *x,
				       int nx)
{
        const int i = get_global_id(0);
	if (i < nx/2)
		x[i] = aop[i].x;
	else if (i < nx)
		x[i] = aop[i-nx/2].y;

}
