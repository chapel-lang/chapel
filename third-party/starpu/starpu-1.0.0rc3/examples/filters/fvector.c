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

#include <starpu.h>

#define NX    21
#define PARTS 3

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

void cpu_func(void *buffers[], void *cl_arg)
{
        unsigned i;
        int *factor = (int *) cl_arg;

        /* length of the vector */
        unsigned n = STARPU_VECTOR_GET_NX(buffers[0]);
        /* local copy of the vector pointer */
        int *val = (int *)STARPU_VECTOR_GET_PTR(buffers[0]);

        for (i = 0; i < n; i++)
                val[i] *= *factor;
}

int main(int argc, char **argv)
{
	unsigned i;
        int vector[NX];
        starpu_data_handle_t handle;
        int factor=1;
	int ret;

        struct starpu_codelet cl =
	{
                .where = STARPU_CPU,
                .cpu_funcs = {cpu_func, NULL},
                .nbuffers = 1,
		.modes = {STARPU_RW}
        };

        for(i=0 ; i<NX ; i++) vector[i] = i;
        FPRINTF(stderr,"IN  Vector: ");
        for(i=0 ; i<NX ; i++) FPRINTF(stderr, "%5d ", vector[i]);
        FPRINTF(stderr,"\n");

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		exit(77);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* Declare data to StarPU */
	starpu_vector_data_register(&handle, 0, (uintptr_t)vector, NX, sizeof(vector[0]));

        /* Partition the vector in PARTS sub-vectors */
	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = PARTS
	};
	starpu_data_partition(handle, &f);

        /* Submit a task on each sub-vector */
	for (i=0; i<starpu_data_get_nb_children(handle); i++)
	{
                starpu_data_handle_t sub_handle = starpu_data_get_sub_data(handle, 1, i);
                struct starpu_task *task = starpu_task_create();

                factor *= 10;
		task->handles[0] = sub_handle;
                task->cl = &cl;
                task->synchronous = 1;
                task->cl_arg = &factor;
                task->cl_arg_size = sizeof(factor);

		ret = starpu_task_submit(task);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	starpu_data_unpartition(handle, 0);
        starpu_data_unregister(handle);
	starpu_shutdown();

        FPRINTF(stderr,"OUT Vector: ");
        for(i=0 ; i<NX ; i++) FPRINTF(stderr, "%5d ", vector[i]);
        FPRINTF(stderr,"\n");

	return 0;

enodev:
	starpu_shutdown();
	return 77;
}
