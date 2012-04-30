/*
 * StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
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
good_0(void)
{
	struct starpu_task *task;
	task = starpu_task_create();
	int ret = starpu_task_submit(task);
	if (ret == -ENODEV)
	{
		fprintf(stderr, "fail\n");
		starpu_task_destroy(task);
	}
}

static void
bad_0(void)
{
	struct starpu_task *task1, *task2;

	task1 = starpu_task_create();
	int ret = starpu_task_submit(task1);
	if (ret == -ENODEV)
	{
		fprintf(stderr, "Fail\n");
	}

	task2 = starpu_task_create();
	ret = starpu_task_submit(task2);
	if (ret == -ENODEV)
	{
		fprintf(stderr, "Fail\n");
	}
}

static void
bad_unlikely(void)
{
	struct starpu_task *task;

	task = starpu_task_create();

	int ret = starpu_task_submit(task);
	if (STARPU_UNLIKELY(ret == -ENODEV))
	{
		error();
		return 1;
	}

	starpu_task_destroy(task);
}
