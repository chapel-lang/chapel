/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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

#include "SobolQRNG/sobol.h"
#include "SobolQRNG/sobol_gold.h"
#include "pi.h"
#include <sys/time.h>

#ifdef STARPU_USE_CUDA
void cuda_kernel(void **descr, void *cl_arg);
#endif

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

/* default value */
static unsigned ntasks = 1024;

static void cpu_kernel(void *descr[], void *cl_arg)
{
	unsigned *directions = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned nx = NSHOT_PER_TASK;

	TYPE *random_numbers = malloc(2*nx*sizeof(TYPE));
	sobolCPU(2*nx/n_dimensions, n_dimensions, directions, random_numbers);

	TYPE *random_numbers_x = &random_numbers[0];
	TYPE *random_numbers_y = &random_numbers[nx];

	unsigned current_cnt = 0;

	unsigned i;
	for (i = 0; i < nx; i++)
	{
		TYPE x = random_numbers_x[i];
		TYPE y = random_numbers_y[i];

		TYPE dist = (x*x + y*y);

		unsigned success = (dist <= 1.0);
		current_cnt += success;
	}

	unsigned *cnt = (unsigned *)STARPU_VECTOR_GET_PTR(descr[1]);
	*cnt = current_cnt;

	free(random_numbers);
}

/* The amount of work does not depend on the data size at all :) */
static size_t size_base(struct starpu_task *task, unsigned nimpl)
{
	return NSHOT_PER_TASK;
}

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-ntasks") == 0)
		{
			char *argptr;
			ntasks = strtol(argv[++i], &argptr, 10);
		}
	}
}

int main(int argc, char **argv)
{
	unsigned i;
	int ret;

	parse_args(argc, argv);

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/* Initialize the random number generator */
	unsigned *sobol_qrng_directions = malloc(n_dimensions*n_directions*sizeof(unsigned));
	STARPU_ASSERT(sobol_qrng_directions);

	initSobolDirectionVectors(n_dimensions, sobol_qrng_directions);

	/* Any worker may use that array now */
	starpu_data_handle_t sobol_qrng_direction_handle;
	starpu_vector_data_register(&sobol_qrng_direction_handle, 0,
		(uintptr_t)sobol_qrng_directions, n_dimensions*n_directions, sizeof(unsigned));

	unsigned *cnt_array = malloc(ntasks*sizeof(unsigned));
	STARPU_ASSERT(cnt_array);
	starpu_data_handle_t cnt_array_handle;
	starpu_vector_data_register(&cnt_array_handle, 0, (uintptr_t)cnt_array, ntasks, sizeof(unsigned));

	/* Use a write-through policy : when the data is modified on an
	 * accelerator, we know that it will only be modified once and be
	 * accessed by the CPU later on */
	starpu_data_set_wt_mask(cnt_array_handle, (1<<0));

	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = ntasks
	};
	
	starpu_data_partition(cnt_array_handle, &f);

	static struct starpu_perfmodel model =
	{
		.type = STARPU_HISTORY_BASED,
		.size_base = size_base,
		.symbol = "monte_carlo_pi"
	};

	struct starpu_codelet cl =
	{
		.where = STARPU_CPU|STARPU_CUDA,
		.cpu_funcs = {cpu_kernel, NULL},
#ifdef STARPU_USE_CUDA
		.cuda_funcs = {cuda_kernel, NULL},
#endif
		.nbuffers = 2,
		.modes = {STARPU_R, STARPU_W},
		.model = &model
	};

	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);

	for (i = 0; i < ntasks; i++)
	{
		struct starpu_task *task = starpu_task_create();

		task->cl = &cl;

		STARPU_ASSERT(starpu_data_get_sub_data(cnt_array_handle, 1, i));

		task->handles[0] = sobol_qrng_direction_handle;
		task->handles[1] = starpu_data_get_sub_data(cnt_array_handle, 1, i);

		int ret = starpu_task_submit(task);
		STARPU_ASSERT(!ret);
	}

	starpu_task_wait_for_all();

	/* Get the cnt_array back in main memory */
	starpu_data_unpartition(cnt_array_handle, 0);
	starpu_data_unregister(cnt_array_handle);
	starpu_data_unregister(sobol_qrng_direction_handle);

	/* Count the total number of entries */
	unsigned long total_cnt = 0;
	for (i = 0; i < ntasks; i++)
		total_cnt += cnt_array[i];

	gettimeofday(&end, NULL);

	double timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	unsigned long total_shot_cnt = ntasks * NSHOT_PER_TASK;

	/* Total surface : Pi * r^ 2 = Pi*1^2, total square surface : 2^2 = 4, probability to impact the disk: pi/4 */
	FPRINTF(stderr, "Pi approximation : %f (%ld / %ld)\n", ((TYPE)total_cnt*4)/(total_shot_cnt), total_cnt, total_shot_cnt);
	FPRINTF(stderr, "Total time : %f ms\n", timing/1000.0);
	FPRINTF(stderr, "Speed : %f GShot/s\n", total_shot_cnt/(1e3*timing));

	if (!getenv("STARPU_SSILENT")) starpu_display_codelet_stats(&cl);

	starpu_shutdown();

	return 0;
}
