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
#include <starpu.h>
#include "../helper.h"

#define NITER	10
#define SIZE	(4*1024*1024*sizeof(float))

static float *data = NULL;

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	unsigned iter;
	for (iter = 0; iter < NITER; iter++)
	{
		ret = starpu_malloc((void **)&data, SIZE);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_malloc");
		starpu_free(data);
	}

	starpu_shutdown();

	return EXIT_SUCCESS;
}
