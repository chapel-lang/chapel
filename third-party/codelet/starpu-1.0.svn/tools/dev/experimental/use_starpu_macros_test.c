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

static void
foo(void)
{
	abort(); /* STARPU_ABORT() */
}

static void
bar(struct starpu_task *task)
{
	assert(task && task->cl); /* STARPU_ASSERT(task && task->cl) */
}

/*
 * STARPU_MIN
 */
static int
min(int a, int b)
{
	return a<b?a:b; /* return STARPU_MIN(a, b); */
}

static int
min2(int a, int b)
{
	int res;
	res = a<b?a:b; /* res = STARPU_MIN(a,b); */
	return res;
}

static int
min3(int a, int b)
{
	int res = a<b?a:b; /* int res = a<b?a:b; */
	return res;
}

static int
min4(int a, int b)
{
	return a>b?b:a; /* return STARPU_MIN(a,b); */
}

static int
min5(int a, int b)
{
	int res;
	res = a>b?b:a; /* res = STARPU_MIN(a,b); */
}

static int
min6(int a, int b)
{
	int res = a>b?b:a; /* int res = STARPU_MIN(a, b); */
	return res;
}

/*
 * STARPU_MAX
 */
static int
max(int a, int b)
{
	return a<b?b:a; /* return STARPU_MAX(a, b); */
}

static int
max2(int a, int b)
{
	int res;
	res = a<b?b:a; /* res = STARPU_MAX(a, b); */
	return res;
}

static int
max3(int a, int b)
{
	int res = a<b?b:a; /* int res = STARPU_MAX(a, b); */
	return res;
}

static int
max4(int a, int b)
{
	return a>b?a:b; /* return STARPU_MAX(a, b); */
}

static int
max5(int a, int b)
{
	int res;
	res = a>b?a:b; /* res = STARPU_MAX(a, b); */
	return res;
}

static int
max6(int a, int b)
{
	int res = a>b?a:b; /* int res = STARPU_MAX(a, b); */
	return res;
}
