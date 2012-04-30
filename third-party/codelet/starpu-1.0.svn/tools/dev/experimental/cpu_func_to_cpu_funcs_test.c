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

#include <stdio.h>
#include <sdtdlib.h>
#include "lol.h"

/*
 * Old format
 */
struct starpu_codelet cl1 = {
	.where = STARPU_CPU,
	.cpu_func = foo
};

/*
 * New format : it must not be changed !
 */
struct starpu_codelet cl2 = {
	.cpu_funcs = {foo, NULL}
};

/*
 * Maybe we added the cpu_funcs fields, but forgot to remove the cpu_func one.
 */
struct starpu_codelet cl3 = {
	.cpu_func = foo,
	.cpu_funcs = { foo, NULL }
};

/*
 * Old multiimplementations format, but not terminated by NULL
 * XXX : NULL is not added.
 */
struct starpu_codelet cl4 = {
	.cpu_func = STARPU_MULTIPLE_CPU_IMPLEMENTATIONS,
	.cpu_funcs = { foo, bar }
};

/*
 * Old multiimplementations format, terminated by NULL
 */
struct starpu_codelet cl5 = {
	.cpu_func = STARPU_MULTIPLE_CPU_IMPLEMENTATIONS,
	.cpu_funcs = { foo, bar, NULL }
};
