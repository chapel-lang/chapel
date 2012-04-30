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
bad_0(void)
{
	cudaError_t ret;
	ret = cudaMalloc(&addr, size);

}

static int
bad_1(void)
{
	cudaError_t cures;
	cures = cudaMemcpy(NULL, NULL, 0);

	return 0;
}

static void
bad_2(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	st = cudaMemcpy(dst, src, size);
}

static void
good_0(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	if (st)
		do_stg_good();
}

static void
good_1(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	if (!st)
		report_error();
	else
		lol();
}

static void
good_2(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	if (STARPU_UNLIKELY(!st))
		report_error();
}

static void
good_3(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	if (STARPU_UNLIKELY(!st))
		report_error();
	else
		foo();
}

static void
good_4(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	if (st != cudaSuccess)
		error();
}

static void
good_5(void)
{
	cudaError_t st;
	st = cudaMemcpy(dst, src, size);
	if (st == cudaSuccess)
		cool();
}


static void
no_assignment_bad_0(void)
{
	cudaGetLastError();
}

static void
no_assignment_bad_1(void)
{
	cudaMemcpy(dst, src, size);
}

static void
no_assignment_good_0(void)
{
	(void) cudaGetLastError();
}
