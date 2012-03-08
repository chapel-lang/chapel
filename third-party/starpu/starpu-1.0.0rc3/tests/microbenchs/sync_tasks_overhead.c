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

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include <starpu.h>
#include "../helper.h"

static unsigned ntasks = 65536;

static void dummy_func(void *descr[] __attribute__ ((unused)), void *arg __attribute__ ((unused)))
{
}

static struct starpu_codelet dummy_codelet = 
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL|STARPU_GORDON,
	.cpu_funcs = {dummy_func, NULL},
	.cuda_funcs = {dummy_func, NULL},
        .opencl_funcs = {dummy_func, NULL},
#ifdef STARPU_USE_GORDON
	.gordon_func = 0, /* this will be defined later */
#endif
	.model = NULL,
	.nbuffers = 0
};

static void init_gordon_kernel(void)
{
#ifdef STARPU_USE_GORDON
	unsigned elf_id = 
		gordon_register_elf_plugin("./microbenchs/null_kernel_gordon.spuelf");
	gordon_load_plugin_on_all_spu(elf_id);

	unsigned gordon_null_kernel =
		gordon_register_kernel(elf_id, "empty_kernel");
	gordon_load_kernel_on_all_spu(gordon_null_kernel);

	dummy_codelet.gordon_func = gordon_null_kernel;
#endif
}

int inject_one_task(void)
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &dummy_codelet;
	task->cl_arg = NULL;
	task->callback_func = NULL;
	task->synchronous = 1;

	ret = starpu_task_submit(task);
	return ret;

}

static void parse_args(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "i:")) != -1)
	switch(c)
	{
		case 'i':
			ntasks = atoi(optarg);
			break;
	}

	
}

int main(int argc, char **argv)
{
	int ret;
	unsigned i;
	double timing;
	struct timeval start;
	struct timeval end;

#ifdef STARPU_SLOW_MACHINE
	ntasks = 128;
#endif

	parse_args(argc, argv);

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	init_gordon_kernel();

	fprintf(stderr, "#tasks : %u\n", ntasks);

	gettimeofday(&start, NULL);
	for (i = 0; i < ntasks; i++)
	{
		ret = inject_one_task();
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}
	gettimeofday(&end, NULL);

	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	fprintf(stderr, "Total: %f secs\n", timing/1000000);
	fprintf(stderr, "Per task: %f usecs\n", timing/ntasks);

        {
                char *output_dir = getenv("STARPU_BENCH_DIR");
                char *bench_id = getenv("STARPU_BENCH_ID");

                if (output_dir && bench_id)
		{
                        char file[1024];
                        FILE *f;

                        sprintf(file, "%s/sync_tasks_overhead_total.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing/1000000);
                        fclose(f);

                        sprintf(file, "%s/sync_tasks_overhead_per_task.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing/ntasks);
                        fclose(f);
                }
        }

	starpu_shutdown();

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
