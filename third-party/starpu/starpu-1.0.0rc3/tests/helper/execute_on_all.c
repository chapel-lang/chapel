/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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
#include <unistd.h>
#include <errno.h>
#include <starpu.h>
#include <stdlib.h>
#include "../helper.h"

void func(void *arg)
{
	int *ptr = (int *) arg;
	STARPU_ASSERT(*ptr == 0x42);
}

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	int arg = 0x42;

	starpu_execute_on_each_worker(func, &arg, STARPU_CPU|STARPU_CUDA|STARPU_OPENCL);

	starpu_execute_on_each_worker(func, &arg, STARPU_CPU);

	starpu_execute_on_each_worker(func, &arg, STARPU_CUDA);

        starpu_execute_on_each_worker(func, &arg, STARPU_OPENCL);

	starpu_shutdown();

	return EXIT_SUCCESS;
}
