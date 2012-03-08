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

#include <starpu.h>
#include "core/workers.h"
#include "../helper.h"

static int can_always_execute(unsigned workerid,
			      struct starpu_task *task,
			      unsigned nimpl)
{
	(void) workerid;
	(void) task;
	(void) nimpl;

	return 1;
}

static int can_never_execute(unsigned workerid,
			     struct starpu_task *task,
			     unsigned nimpl)
{
	(void) workerid;
	(void) task;
	(void) nimpl;

	return 0;
}

static void fake(void *buffers[], void *args)
{
	(void) buffers;
	(void) args;
}

static struct starpu_codelet cl =
{
	.where        = STARPU_CPU | STARPU_CUDA | STARPU_OPENCL,
	.cpu_funcs    = { fake, NULL},
	.cuda_funcs   = { fake, NULL},
	.opencl_funcs = { fake, NULL},
	.nbuffers     = 0
};

int
main(void)
{
	int ret;
	struct starpu_task *task;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;

	task = starpu_task_create();
	task->cl = &cl;
	task->destroy = 0;

	cl.can_execute = NULL;
	ret = _starpu_worker_exists(task);
	if (!ret)
		return EXIT_FAILURE;

	cl.can_execute = can_always_execute;
	ret = _starpu_worker_exists(task);
	if (!ret)
		return EXIT_FAILURE;

	cl.can_execute = can_never_execute;
	ret = _starpu_worker_exists(task);
	if (ret)
		return EXIT_FAILURE;

	starpu_task_destroy(task);
	starpu_shutdown();

	return EXIT_SUCCESS;
}
