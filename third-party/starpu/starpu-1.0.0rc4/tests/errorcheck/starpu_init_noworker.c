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

#if !defined(STARPU_HAVE_UNSETENV)
#warning unsetenv is not defined. Skipping test
int main(int argc, char **argv)
{
	return STARPU_TEST_SKIPPED;
}
#else
static void unset_env_variables(void)
{
	(void) unsetenv("STARPU_NCPUS");
	(void) unsetenv("STARPU_NCUDA");
	(void) unsetenv("STARPU_NNOPENCL");
}

int main(int argc, char **argv)
{
	int ret;

	unset_env_variables();

	/* We try to initialize StarPU without any worker */
	struct starpu_conf conf =
	{
		.sched_policy_name = NULL, /* default */
		.ncpus = 0,
		.ncuda = 0,
                .nopencl = 0,
		.nspus = 0,
		.use_explicit_workers_bindid = 0,
		.use_explicit_workers_cuda_gpuid = 0,
		.use_explicit_workers_opencl_gpuid = 0,
		.calibrate = 0
	};

	/* starpu_init should return -ENODEV */
	ret = starpu_init(&conf);
	if (ret != -ENODEV)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
#endif
