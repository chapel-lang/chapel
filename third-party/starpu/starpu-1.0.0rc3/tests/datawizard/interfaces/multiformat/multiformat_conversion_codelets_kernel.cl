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
#include "multiformat_types.h"
__kernel void cpu_to_opencl_opencl(__global struct point *src,
				   __global struct struct_of_arrays *dst,
				   unsigned int n)
{
	const unsigned int i = get_global_id(0);
	if (i < n)
	{
		dst->x[i] = src[i].x;
		dst->y[i] = src[i].y;
	}
}
