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

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <starpu.h>
#include "../helper.h"

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

static void callback(void *tag)
{
//	fflush(stderr);
//	FPRINTF(stderr, "Callback for tag %p\n", tag);
//	fflush(stderr);
}

static struct starpu_task *create_dummy_task(starpu_tag_t tag)
{
	struct starpu_task *task = starpu_task_create();

	task->cl = &dummy_codelet;
	task->cl_arg = NULL;
	task->callback_func = callback;
	task->callback_arg = (void *)tag;

	task->use_tag = 1;
	task->tag_id = tag;

	return task;
}

#define tagA	((starpu_tag_t)0x42)
#define tagB	((starpu_tag_t)0x12300)

#define tagC	((starpu_tag_t)0x32)
#define tagD	((starpu_tag_t)0x52)
#define tagE	((starpu_tag_t)0x19999)
#define tagF	((starpu_tag_t)0x2312)
#define tagG	((starpu_tag_t)0x1985)

#define tagH	((starpu_tag_t)0x32234)
#define tagI	((starpu_tag_t)0x5234)
#define tagJ	((starpu_tag_t)0x199)
#define tagK	((starpu_tag_t)0x231234)
#define tagL	((starpu_tag_t)0x2345)

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	FPRINTF(stderr, "{ A } -> { B }\n");
	fflush(stderr);

	struct starpu_task *taskA, *taskB;

	taskA = create_dummy_task(tagA);
	taskB = create_dummy_task(tagB);

	/* B depends on A */
	starpu_tag_declare_deps(tagB, 1, tagA);

	ret = starpu_task_submit(taskB); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskA); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");

	ret = starpu_tag_wait(tagB); STARPU_CHECK_RETURN_VALUE(ret, "starpu_wait");

	FPRINTF(stderr, "{ C, D, E, F } -> { G }\n");

	struct starpu_task *taskC, *taskD, *taskE, *taskF, *taskG;

	taskC = create_dummy_task(tagC);
	taskD = create_dummy_task(tagD);
	taskE = create_dummy_task(tagE);
	taskF = create_dummy_task(tagF);
	taskG = create_dummy_task(tagG);

	/* NB: we could have used starpu_tag_declare_deps_array instead */
	starpu_tag_declare_deps(tagG, 4, tagC, tagD, tagE, tagF);

	ret = starpu_task_submit(taskC); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskD); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskG); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskE); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskF); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");

	ret = starpu_tag_wait(tagG); STARPU_CHECK_RETURN_VALUE(ret, "starpu_tag_wait");

	FPRINTF(stderr, "{ H, I } -> { J, K, L }\n");

	struct starpu_task *taskH, *taskI, *taskJ, *taskK, *taskL;

	taskH = create_dummy_task(tagH);
	taskI = create_dummy_task(tagI);
	taskJ = create_dummy_task(tagJ);
	taskK = create_dummy_task(tagK);
	taskL = create_dummy_task(tagL);

	starpu_tag_declare_deps(tagJ, 2, tagH, tagI);
	starpu_tag_declare_deps(tagK, 2, tagH, tagI);
	starpu_tag_declare_deps(tagL, 2, tagH, tagI);

	starpu_tag_t tagJKL[3] = {tagJ, tagK, tagL};

	ret = starpu_task_submit(taskH); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskI); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskJ); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskK); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");
	ret = starpu_task_submit(taskL); STARPU_CHECK_RETURN_VALUE(ret, "starpu_submit");

	ret = starpu_tag_wait_array(3, tagJKL); STARPU_CHECK_RETURN_VALUE(ret, "starpu_tag_wait_array");

	starpu_shutdown();

	return EXIT_SUCCESS;
}
