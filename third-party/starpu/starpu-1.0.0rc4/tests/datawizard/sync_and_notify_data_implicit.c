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

#include <config.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <starpu.h>

#ifdef STARPU_USE_GORDON
#include <gordon.h>
#endif

#include "../helper.h"

#define N_DEF	100
#define K_DEF	256

static int n=N_DEF;
static int k=K_DEF;

/*
 * In this test, we maintain a vector v = (a,b,c).
 *
 * Each iteration consists of:
 *  - increment a n times
 *  - sync v in ram
 *  - incrementer b
 *  - notify the modification of v
 *  - incrementer c n times
 *  - sync v 
 *
 * At the end, we have to make sure that if we did k iterations,
 *  v == (kn, k, kn)
 */

#ifdef STARPU_USE_CUDA
void cuda_codelet_incA(void *descr[], __attribute__ ((unused)) void *_args);
void cuda_codelet_incC(void *descr[], __attribute__ ((unused)) void *_args);
#endif

#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
void opencl_codelet_incA(void *descr[], __attribute__ ((unused)) void *_args);
void opencl_codelet_incC(void *descr[], __attribute__ ((unused)) void *_args);
struct starpu_opencl_program opencl_code;
#endif

#define VECTORSIZE	16

starpu_data_handle_t v_handle;
static unsigned v[VECTORSIZE] __attribute__((aligned(128))) = {0, 0, 0, 0};

void cpu_codelet_incA(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	unsigned *val = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);
	val[0]++;
}

void cpu_codelet_incC(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	unsigned *val = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);
	val[2]++;
}

/* increment a = v[0] */
static struct starpu_codelet cl_inc_a =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL|STARPU_GORDON,
	.cpu_funcs = {cpu_codelet_incA, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_codelet_incA, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {opencl_codelet_incA, NULL},
#endif
#ifdef STARPU_USE_GORDON
	.gordon_func = kernel_incA_id,
#endif
	.nbuffers = 1,
	.modes = {STARPU_RW}
};

/* increment c = v[2] */
struct starpu_codelet cl_inc_c =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL|STARPU_GORDON,
	.cpu_funcs = {cpu_codelet_incC, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_codelet_incC, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {opencl_codelet_incC, NULL},
#endif
#ifdef STARPU_USE_GORDON
	.gordon_func = kernel_incC_id,
#endif
	.nbuffers = 1,
	.modes = {STARPU_RW}
};

int main(int argc, char **argv)
{
	int ret;

#ifdef STARPU_SLOW_MACHINE
	n /= 10;
	k /= 8;
#endif

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_USE_GORDON
	unsigned elf_id = gordon_register_elf_plugin("./datawizard/sync_and_notify_data_gordon_kernels.spuelf");
	gordon_load_plugin_on_all_spu(elf_id);

	unsigned kernel_incA_id = gordon_register_kernel(elf_id, "incA");
	gordon_load_kernel_on_all_spu(kernel_incA_id);

	unsigned kernel_incC_id = gordon_register_kernel(elf_id, "incC");
	gordon_load_kernel_on_all_spu(kernel_incC_id);

	FPRINTF(stderr, "kernel incA %d incC %d elf %d\n", kernel_incA_id, kernel_incC_id, elf_id);
#endif

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_load_opencl_from_file("tests/datawizard/sync_and_notify_data_opencl_codelet.cl", &opencl_code, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif

        starpu_vector_data_register(&v_handle, 0, (uintptr_t)v, VECTORSIZE, sizeof(unsigned));

	unsigned iter;
	for (iter = 0; iter < k; iter++)
	{
		int ret;
		unsigned ind;
		for (ind = 0; ind < n; ind++)
		{
			struct starpu_task *task = starpu_task_create();
			task->cl = &cl_inc_a;
			task->handles[0] = v_handle;

			ret = starpu_task_submit(task);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}

		/* synchronize v in RAM */
		ret = starpu_data_acquire(v_handle, STARPU_RW);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire");

		/* increment b */
		v[1]++;

		starpu_data_release(v_handle);

		for (ind = 0; ind < n; ind++)
		{
			struct starpu_task *task = starpu_task_create();
			task->cl = &cl_inc_c;
			task->handles[0] = v_handle;

			ret = starpu_task_submit(task);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}

	}

	ret = starpu_data_acquire(v_handle, STARPU_RW);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire");

	FPRINTF(stderr, "V = {%u, %u, %u}\n", v[0], v[1], v[2]);

	starpu_data_release(v_handle);
	starpu_data_unregister(v_handle);
	starpu_shutdown();

	ret = EXIT_SUCCESS;
	if ((v[0] != n*k) || (v[1] != k) || (v[2] != n*k))
	{
		FPRINTF(stderr, "Incorrect result\n");
		ret = EXIT_FAILURE;
	}
	STARPU_RETURN(ret);

enodev:
	starpu_data_unregister(v_handle);
	starpu_shutdown();
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	return STARPU_TEST_SKIPPED;
}
