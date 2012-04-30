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
void
bad_1(void)
{
	int i;
	for (i = 0; i < bar(a,b); i++)
	{
		do_stg();
	}

	for (i = 0; i < foo(); i++)
	{
		do_stg_else();
	}
}

void
good_1(void)
{
	int i, max;
	max = bar();
	for (i = foo(); i < max; i++)
	{
		do_stg();
	}
}
