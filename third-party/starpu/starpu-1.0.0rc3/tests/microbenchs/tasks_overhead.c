/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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
#include <pthread.h>

#include <starpu.h>
#include "../helper.h"

starpu_data_handle_t data_handles[8];
float *buffers[8];

static unsigned ntasks = 65536;
static unsigned nbuffers = 0;

struct starpu_task *tasks;

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
	.nbuffers = 0,
	.modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_RW, STARPU_RW, STARPU_RW, STARPU_RW, STARPU_RW}
};

int inject_one_task(void)
{
	struct starpu_task *task = starpu_task_create();

	task->cl = &dummy_codelet;
	task->cl_arg = NULL;
	task->callback_func = NULL;
	task->synchronous = 1;

	int ret;
	ret = starpu_task_submit(task);
	return ret;
}

static void parse_args(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "i:b:h")) != -1)
	switch(c)
	{
		case 'i':
			ntasks = atoi(optarg);
			break;
		case 'b':
			nbuffers = atoi(optarg);
			dummy_codelet.nbuffers = nbuffers;
			break;
		case 'h':
			fprintf(stderr, "Usage: %s [-i ntasks] [-b nbuffers] [-h]\n", argv[0]);
			break;
	}
}

int main(int argc, char **argv)
{
	int ret;
	unsigned i;

	double timing_submit;
	struct timeval start_submit;
	struct timeval end_submit;

	double timing_exec;
	struct timeval start_exec;
	struct timeval end_exec;

	parse_args(argc, argv);

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	unsigned buffer;
	for (buffer = 0; buffer < nbuffers; buffer++)
	{
		buffers[buffer] = (float *) malloc(16*sizeof(float));
		starpu_vector_data_register(&data_handles[buffer], 0, (uintptr_t)buffers[buffer], 16, sizeof(float));
	}

	fprintf(stderr, "#tasks : %u\n#buffers : %u\n", ntasks, nbuffers);

	/* submit tasks (but don't execute them yet !) */
	tasks = (struct starpu_task *) calloc(1, ntasks*sizeof(struct starpu_task));

	gettimeofday(&start_submit, NULL);
	for (i = 0; i < ntasks; i++)
	{
		starpu_task_init(&tasks[i]);
		tasks[i].callback_func = NULL;
		tasks[i].cl = &dummy_codelet;
		tasks[i].cl_arg = NULL;
		tasks[i].synchronous = 0;
		tasks[i].use_tag = 1;
		tasks[i].tag_id = (starpu_tag_t)i;

		/* we have 8 buffers at most */
		for (buffer = 0; buffer < nbuffers; buffer++)
		{
			tasks[i].handles[buffer] = data_handles[buffer];
		}
	}

	gettimeofday(&start_submit, NULL);
	for (i = 1; i < ntasks; i++)
	{
		starpu_tag_declare_deps((starpu_tag_t)i, 1, (starpu_tag_t)(i-1));

		ret = starpu_task_submit(&tasks[i]);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	/* submit the first task */
	ret = starpu_task_submit(&tasks[0]);
	if (ret == -ENODEV) goto enodev;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	gettimeofday(&end_submit, NULL);

	/* wait for the execution of the tasks */
	gettimeofday(&start_exec, NULL);
	ret = starpu_tag_wait((starpu_tag_t)(ntasks - 1));
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_tag_wait");
	gettimeofday(&end_exec, NULL);

	for (buffer = 0; buffer < nbuffers; buffer++)
	{
		starpu_data_unregister(data_handles[buffer]);
	}

	timing_submit = (double)((end_submit.tv_sec - start_submit.tv_sec)*1000000 + (end_submit.tv_usec - start_submit.tv_usec));
	timing_exec = (double)((end_exec.tv_sec - start_exec.tv_sec)*1000000 + (end_exec.tv_usec - start_exec.tv_usec));

	fprintf(stderr, "Total submit: %f secs\n", timing_submit/1000000);
	fprintf(stderr, "Per task submit: %f usecs\n", timing_submit/ntasks);
	fprintf(stderr, "\n");
	fprintf(stderr, "Total execution: %f secs\n", timing_exec/1000000);
	fprintf(stderr, "Per task execution: %f usecs\n", timing_exec/ntasks);
	fprintf(stderr, "\n");
	fprintf(stderr, "Total: %f secs\n", (timing_submit+timing_exec)/1000000);
	fprintf(stderr, "Per task: %f usecs\n", (timing_submit+timing_exec)/ntasks);

        {
                char *output_dir = getenv("STARPU_BENCH_DIR");
                char *bench_id = getenv("STARPU_BENCH_ID");

                if (output_dir && bench_id)
		{
                        char file[1024];
                        FILE *f;

                        sprintf(file, "%s/tasks_overhead_total_submit.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing_submit/1000000);
                        fclose(f);

                        sprintf(file, "%s/tasks_overhead_per_task_submit.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing_submit/ntasks);
                        fclose(f);

                        sprintf(file, "%s/tasks_overhead_total_execution.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing_exec/1000000);
                        fclose(f);

                        sprintf(file, "%s/tasks_overhead_per_task_execution.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, timing_exec/ntasks);
                        fclose(f);

                        sprintf(file, "%s/tasks_overhead_total_submit_execution.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, (timing_submit+timing_exec)/1000000);
                        fclose(f);

                        sprintf(file, "%s/tasks_overhead_per_task_submit_execution.dat", output_dir);
                        f = fopen(file, "a");
                        fprintf(f, "%s\t%f\n", bench_id, (timing_submit+timing_exec)/ntasks);
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
