/*
 * StarPU --- Runtime system for heterogeneous multicore architectures.
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
#include <stdio.h>
#include <valgrind/valgrind.h>

void
cpu_func_0(void *buffers[], void *args)
{
	do_stg();
}

void empty_func(void *buffers[], void *args)
{

}

void good_0(void *buffers[], void *args)
{
	STARPU_SKIP_IF_VALGRIND;


}
