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

#include <starpu.h>
#include <pthread.h>
#include <sys/time.h>

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

starpu_data_handle_t handle;

void cpu_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	int *val = (int *)STARPU_VARIABLE_GET_PTR(descr[0]);

	*val += 1;
}

struct starpu_codelet cl =
{
	.modes = { STARPU_RW },
	.where = STARPU_CPU,
	.cpu_funcs = {cpu_codelet, NULL},
	.nbuffers = 1
};

void callback_func(void *callback_arg)
{
	int ret;

	struct starpu_task *task = starpu_task_create();
	task->cl = &cl;
	task->handles[0] = handle;

	ret = starpu_task_submit(task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
}

int main(int argc, char **argv)
{
	int v=40;
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_variable_data_register(&handle, 0, (uintptr_t)&v, sizeof(int));

	struct starpu_task *task = starpu_task_create();
	task->cl = &cl;
	task->callback_func = callback_func;
	task->callback_arg = NULL;
	task->handles[0] = handle;

	ret = starpu_task_submit(task);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	starpu_task_wait_for_all();
	starpu_data_unregister(handle);

	FPRINTF(stderr, "v -> %d\n", v);

	starpu_shutdown();

	return 0;

enodev:
	starpu_shutdown();
	return 77;
}
