/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

/* gcc build:

   gcc -fopenmp -O2 -g vector_scal.c -o vector_scal $(pkg-config --cflags starpu-1.0) $(pkg-config --libs starpu-1.0)

 */

#include <starpu.h>
#include <stdio.h>
#include <limits.h>

#define	NX	2048000
#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

void scal_cpu_func(void *buffers[], void *_args)
{
	unsigned i;
	float *factor = _args, f = *factor;
	struct starpu_vector_interface *vector = buffers[0];
	unsigned n = STARPU_VECTOR_GET_NX(vector);
	float *val = (float *)STARPU_VECTOR_GET_PTR(vector);

	FPRINTF(stderr, "running task with %d CPUs.\n", starpu_combined_worker_get_size());

#pragma omp parallel for num_threads(starpu_combined_worker_get_size())
	for (i = 0; i < n; i++) {
		float v = val[i];
		int j;
		for (j = 0; j < 100; j++)
			v = v * f;
		val[i] = v;
	}
}

static struct starpu_perfmodel vector_scal_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "vector_scale_parallel"
};

static struct starpu_codelet cl =
{
	.modes = { STARPU_RW },
	.where = STARPU_CPU,
	.type = STARPU_FORKJOIN,
	.max_parallelism = INT_MAX,
	.cpu_funcs = {scal_cpu_func, NULL},
	.nbuffers = 1,
	.model = &vector_scal_model,
};

int main(int argc, char **argv)
{
	struct starpu_conf conf;
	float *vector;
	unsigned i;
	int ret;

	vector = malloc(NX*sizeof(*vector));

	for (i = 0; i < NX; i++)
                vector[i] = (i+1.0f);

	FPRINTF(stderr, "BEFORE: First element was %f\n", vector[0]);
	FPRINTF(stderr, "BEFORE: Last element was %f\n", vector[NX-1]);

	starpu_conf_init(&conf);

	/* Most OpenMP implementations do not support concurrent parallel
	 * sections, so only create one big worker */
	conf.single_combined_worker = 1;
	conf.sched_policy_name = "pheft";

	ret = starpu_init(&conf);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_data_handle_t vector_handle;
	starpu_vector_data_register(&vector_handle, 0, (uintptr_t)vector, NX, sizeof(vector[0]));

	float factor = 1.001;

	for (i = 0; i < 100; i++) {
		struct starpu_task *task = starpu_task_create();

		task->cl = &cl;

		task->handles[0] = vector_handle;
		task->cl_arg = &factor;
		task->cl_arg_size = sizeof(factor);

		ret = starpu_task_submit(task);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	starpu_data_unregister(vector_handle);

	/* terminate StarPU, no task can be submitted after */
	starpu_shutdown();

	FPRINTF(stderr, "AFTER: First element is %f\n", vector[0]);
	FPRINTF(stderr, "AFTER: Last element is %f\n", vector[NX-1]);

	return 0;
}
