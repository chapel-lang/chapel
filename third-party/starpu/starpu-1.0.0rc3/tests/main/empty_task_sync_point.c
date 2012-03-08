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

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <starpu.h>
#include "../helper.h"

static starpu_tag_t tagA = 0x0042;
static starpu_tag_t tagB = 0x1042;
static starpu_tag_t tagC = 0x2042;
static starpu_tag_t tagD = 0x3042;
static starpu_tag_t tagE = 0x4042;
static starpu_tag_t tagF = 0x5042;

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet dummy_codelet =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {dummy_func, NULL},
	.cuda_funcs = {dummy_func, NULL},
	.opencl_funcs = {dummy_func, NULL},
	.model = NULL,
	.nbuffers = 0
};

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* {A,B,C} -> D -> {E,F}, D is empty */
	struct starpu_task *taskA = starpu_task_create();
	taskA->cl = &dummy_codelet;
	taskA->use_tag = 1;
	taskA->tag_id = tagA;

	struct starpu_task *taskB = starpu_task_create();
	taskB->cl = &dummy_codelet;
	taskB->use_tag = 1;
	taskB->tag_id = tagB;

	struct starpu_task *taskC = starpu_task_create();
	taskC->cl = &dummy_codelet;
	taskC->use_tag = 1;
	taskC->tag_id = tagC;

	struct starpu_task *taskD = starpu_task_create();
	taskD->cl = NULL;
	taskD->use_tag = 1;
	taskD->tag_id = tagD;
	starpu_tag_declare_deps(tagD, 3, tagA, tagB, tagC);

	struct starpu_task *taskE = starpu_task_create();
	taskE->cl = &dummy_codelet;
	taskE->use_tag = 1;
	taskE->tag_id = tagE;
	starpu_tag_declare_deps(tagE, 1, tagD);

	struct starpu_task *taskF = starpu_task_create();
	taskF->cl = &dummy_codelet;
	taskF->use_tag = 1;
	taskF->tag_id = tagF;
	starpu_tag_declare_deps(tagF, 1, tagD);

	ret = starpu_task_submit(taskA); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskB); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskC); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskD); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskE); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(taskF); STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	starpu_tag_t tag_array[2] = {tagE, tagF};
	ret = starpu_tag_wait_array(2, tag_array);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_tag_wait_array");

	starpu_shutdown();

	return EXIT_SUCCESS;
}
