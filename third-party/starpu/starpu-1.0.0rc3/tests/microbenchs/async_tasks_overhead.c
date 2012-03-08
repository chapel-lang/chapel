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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <starpu.h>
#include <starpu_profiling.h>
#include "../helper.h"

static unsigned ntasks = 65536;

//static unsigned finished = 0;

static double cumulated = 0.0;
static double cumulated_push = 0.0;
static double cumulated_pop = 0.0;

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

//static void inject_one_task(void)
//{
//	struct starpu_task *task = starpu_task_create();
//
//	task->cl = &dummy_codelet;
//	task->cl_arg = NULL;
//	task->detach = 0;
//
//	int ret = starpu_task_submit(task);
//	STARPU_ASSERT(!ret);
//}

static struct starpu_conf conf =
{
	.sched_policy_name = NULL,
	.ncpus = -1,
	.ncuda = -1,
        .nopencl = -1,
	.nspus = -1,
	.use_explicit_workers_bindid = 0,
	.use_explicit_workers_cuda_gpuid = 0,
	.use_explicit_workers_opencl_gpuid = 0,
	.calibrate = 0
};

static void usage(char **argv)
{
	fprintf(stderr, "%s [-i ntasks] [-p sched_policy] [-h]\n", argv[0]);
	exit(-1);
}

static void parse_args(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "i:p:h")) != -1)
	switch(c)
	{
		case 'i':
			ntasks = atoi(optarg);
			break;
		case 'p':
			conf.sched_policy_name = optarg;
			break;
		case 'h':
			usage(argv);
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

	parse_args(argc, argv);

	ret = starpu_init(&conf);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	init_gordon_kernel();

	starpu_profiling_status_set(STARPU_PROFILING_ENABLE);

	fprintf(stderr, "#tasks : %u\n", ntasks);

	/* Create an array of tasks */
	struct starpu_task **tasks = (struct starpu_task **) malloc(ntasks*sizeof(struct starpu_task *));

	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task *task = starpu_task_create();
		task->cl = &dummy_codelet;
		task->cl_arg = NULL;
		task->detach = 0;
		tasks[i] = task;
	}

	gettimeofday(&start, NULL);
	for (i = 0; i < ntasks; i++)
	{
		int ret = starpu_task_submit(tasks[i]);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	gettimeofday(&end, NULL);

	/* Read profiling feedback */
	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task_profiling_info *info;
		info = tasks[i]->profiling_info;

		double queued = starpu_timing_timespec_delay_us(&info->push_end_time, &info->pop_end_time);
		double length = starpu_timing_timespec_delay_us(&info->submit_time, &info->end_time);
		double push_duration = starpu_timing_timespec_delay_us(&info->push_start_time, &info->push_end_time);
		double pop_duration = starpu_timing_timespec_delay_us(&info->pop_start_time, &info->pop_end_time);
		starpu_task_destroy(tasks[i]);
		cumulated += (length - queued);
		cumulated_push += push_duration;
		cumulated_pop += pop_duration;
	}

	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	fprintf(stderr, "Total: %f secs\n", timing/1000000);
	fprintf(stderr, "Per task: %f usecs\n", timing/ntasks);
	fprintf(stderr, "Per task (except scheduler): %f usecs\n", cumulated/ntasks);
	fprintf(stderr, "Per task (push): %f usecs\n", cumulated_push/ntasks);
	fprintf(stderr, "Per task (pop): %f usecs\n", cumulated_pop/ntasks);

        {
                char *output_dir = getenv("STARPU_BENCH_DIR");
                char *bench_id = getenv("STARPU_BENCH_ID");

                if (output_dir && bench_id)
		{
                        char file[1024];
                        FILE *f;

                        sprintf(file, "%s/async_tasks_overhead_total.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing/1000000);
                        fclose(f);

                        sprintf(file, "%s/async_tasks_overhead_per_task.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing/ntasks);
                        fclose(f);
                }
        }

	starpu_shutdown();
	free(tasks);

	return EXIT_SUCCESS;

enodev:
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	return STARPU_TEST_SKIPPED;
}
