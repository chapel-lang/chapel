/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet dummy_codelet =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {dummy_func, NULL},
	.cuda_funcs = {dummy_func, NULL},
        .opencl_funcs = {dummy_func, NULL},
	.nbuffers = 0
};

static int create_dummy_task(starpu_tag_t tag)
{
	struct starpu_task *task = starpu_task_create();

	task->use_tag = 1;
	task->tag_id = tag;
	task->cl = &dummy_codelet;
	
	int ret = starpu_task_submit(task);
	return ret;
}

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_tag_t sync_tags[NITER];

	unsigned iter;
	for (iter = 0; iter < NITER; iter++)
	{
		starpu_tag_t sync_tag = (starpu_tag_t)iter*100;

		sync_tags[iter] = sync_tag;

		unsigned ndeps = 10;
		starpu_tag_t deps[ndeps];

		unsigned d;
		for (d = 0; d < ndeps; d++)
		{
			deps[d] = sync_tag + d + 1; 

			ret = create_dummy_task(deps[d]);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}

		starpu_create_sync_task(sync_tag, ndeps, deps, NULL, NULL);
	}

	/* Wait all the synchronization tasks */
	ret = starpu_tag_wait_array(NITER, sync_tags);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_tag_wait_array");

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
