/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#include <starpu.h>
#include "../helper.h"
#include <common/config.h>

#if !defined(STARPU_HAVE_UNSETENV) || !defined(STARPU_USE_CPU)
#warning unsetenv is not defined or no cpu are available. Skipping test
int main(int argc, char **argv)
{
	return STARPU_TEST_SKIPPED;
}
#else

static void dummy_func(void *descr[], void *arg)
{
}

static struct starpu_codelet gpu_only_cl =
{
	.cuda_funcs = {dummy_func, NULL},
	.opencl_funcs = {dummy_func, NULL},
	.model = NULL,
	.nbuffers = 0
};

int main(int argc, char **argv)
{
	int ret;

	/* We force StarPU to use 1 CPU only */
	unsetenv("STARPU_NCUDA");
	unsetenv("STARPU_NOPENCL");
	unsetenv("STARPU_NCPUS");
	struct starpu_conf conf;
	memset(&conf, 0, sizeof(conf));
	conf.ncpus = 1;

	ret = starpu_init(&conf);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	struct starpu_task *task = starpu_task_create();
	task->cl = &gpu_only_cl;

	/* Only a CUDA device could execute that task ! */
	ret = starpu_task_submit(task);
	STARPU_ASSERT(ret == -ENODEV);

	task->destroy = 0;
	starpu_task_destroy(task);

	struct starpu_task *task_specific = starpu_task_create();
	task_specific->cl = &gpu_only_cl;
	task_specific->execute_on_a_specific_worker = 1;
	task_specific->workerid = 0;

	/* Only a CUDA device could execute that task ! */
	ret = starpu_task_submit(task_specific);
	STARPU_ASSERT(ret == -ENODEV);

	task_specific->destroy = 0;
	starpu_task_destroy(task_specific);

	starpu_shutdown();

	return EXIT_SUCCESS;
}
#endif
