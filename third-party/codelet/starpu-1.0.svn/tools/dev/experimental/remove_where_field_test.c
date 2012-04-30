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

struct dummy_struct
{
	int where;
};


/* Simple example : remove the where field */
struct starpu_codelet cl = {
	.cuda_func = bar,
	.where = STARPU_CPU | STARPU_OPENCL,
	.cpu_func = foo
};


void
dummy(void)
{
	cl.where = STARPU_CUDA;
	starpu_codelet_t *clp = &cl;
	clp->where = STARPU_CPU; /* Must be removed */

	struct dummy_struct ds;
	struct dummy_struct *dsp = &ds;
	ds.where = 12;   /* Must not be removed */
	dsp->where = 12; /* Must not be removed */ 
}
