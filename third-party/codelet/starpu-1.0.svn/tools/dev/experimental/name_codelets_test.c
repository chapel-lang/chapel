/*
 * StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
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
struct starpu_codelet good_beginning = {
	.where = STARPU_CPU,
	.name = "lol"
};

struct starpu_codelet good_middle = { 
	.where = STARPU_CPU,
	.name = "lol",
	.cpu_funcs = { foo, NULL }
};
struct starpu_codelet good_end = {
	.where = STARPU_CPU,
	.name = "lol"
};

struct starpu_codelet bad =
{
	.where = STARPU_CPU,
};

static struct starpu_codelet bad_static =
{
	.where = STARPU_CPU,
};
