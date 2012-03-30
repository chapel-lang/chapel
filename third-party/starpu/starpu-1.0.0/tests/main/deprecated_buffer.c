/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
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

#include <config.h>
#include <starpu.h>
#include "../helper.h"

void cpu_codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	int *valin = (int *)STARPU_VARIABLE_GET_PTR(descr[0]);
	int *valout = (int *)STARPU_VARIABLE_GET_PTR(descr[1]);

	*valout = *valin;
}

struct starpu_codelet cl_with_mode =
{
	.name = "with_mode",
	.where = STARPU_CPU,
	.cpu_funcs = {cpu_codelet, NULL},
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_W},
};

struct starpu_codelet cl_without_mode =
{
	.name = "without_mode",
	.where = STARPU_CPU,
	.cpu_funcs = {cpu_codelet, NULL},
	.nbuffers = 2
};

int submit_codelet_insert_task(struct starpu_codelet cl, starpu_data_handle_t handles0, starpu_data_handle_t handles1)
{
	int ret;

	ret = starpu_insert_task(&cl,
				 STARPU_R, handles0,
				 STARPU_W, handles1,
				 0);
	if (ret == -ENODEV) return ret;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

	starpu_task_wait_for_all();
	return 0;
}

int submit_codelet_with_buffers(struct starpu_codelet cl, starpu_data_handle_t handles0, starpu_data_handle_t handles1)
{
	int ret;
	struct starpu_task *task;

	task = starpu_task_create();
	task->cl = &cl;
	task->buffers[0].handle = handles0;
	task->buffers[0].mode = STARPU_R;
	task->buffers[1].handle = handles1;
	task->buffers[1].mode = STARPU_W;

	ret = starpu_task_submit(task);
	if (ret == -ENODEV) return ret;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	starpu_task_wait_for_all();
	return 0;
}

int submit_codelet_with_handles(struct starpu_codelet cl, starpu_data_handle_t handles0, starpu_data_handle_t handles1)
{
	int ret;
	struct starpu_task *task;

	task = starpu_task_create();
	task->cl = &cl;
	task->handles[0] = handles0;
	task->handles[1] = handles1;

	ret = starpu_task_submit(task);
	if (ret == -ENODEV) return ret;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	starpu_task_wait_for_all();
	return 0;
}

struct submit_task_func
{
	int (*func)(struct starpu_codelet cl, starpu_data_handle_t handles0, starpu_data_handle_t handles1);
	char *name;
};

int submit_codelet(struct starpu_codelet cl, struct submit_task_func func)
{
	int x=42, y=14;
	starpu_data_handle_t handles[2];
	int ret;

	starpu_variable_data_register(&handles[0], 0, (uintptr_t)&x, sizeof(x));
	starpu_variable_data_register(&handles[1], 0, (uintptr_t)&y, sizeof(y));

	ret = func.func(cl, handles[0], handles[1]);
	starpu_data_unregister(handles[0]);
	starpu_data_unregister(handles[1]);

	if (ret == -ENODEV) return ret;

	FPRINTF(stderr, "%s when executing codelet <%s> with func <%s>\n", x==y?"success":"error", cl.name, func.name);
	return (x != y);
}

int main(int argc, char **argv)
{
	int ret;
	struct submit_task_func insert_task = { .func = submit_codelet_insert_task, .name = "insert_task" };
	struct submit_task_func with_buffers = { .func = submit_codelet_with_buffers, .name = "with_buffers" };
	struct submit_task_func with_handles = { .func = submit_codelet_with_handles, .name = "with_handles" };

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	ret = submit_codelet(cl_with_mode, insert_task);
	if (ret == -ENODEV)
	{
		starpu_shutdown();
		fprintf(stderr, "WARNING: No one can execute this task\n");
		return STARPU_TEST_SKIPPED;
	}

	if (!ret)
	{
		ret = submit_codelet(cl_with_mode, with_buffers);
	}
	if (!ret)
	{
		ret = submit_codelet(cl_with_mode, with_handles);
	}
	if (!ret)
	{
		ret = submit_codelet(cl_without_mode, insert_task);
	}
	if (!ret)
	{
		ret = submit_codelet(cl_without_mode, with_buffers);
	}
	// We do not test the combination cl_without_mode with_handles as it is not expected to work

	starpu_shutdown();

	STARPU_RETURN(ret);
}
