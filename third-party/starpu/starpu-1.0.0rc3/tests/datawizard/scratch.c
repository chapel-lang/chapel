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
#include <stdlib.h>
#include "../helper.h"

#define NLOOPS		128
#define VECTORSIZE	1024

static unsigned *A;
starpu_data_handle_t A_handle, B_handle;

//static unsigned var = 0;

#ifdef STARPU_USE_CUDA
extern void cuda_f(void *descr[], __attribute__ ((unused)) void *_args);
#endif

static void cpu_f(void *descr[], __attribute__ ((unused)) void *_args)
{
	STARPU_SKIP_IF_VALGRIND;

	unsigned *v = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned *tmp = (unsigned *)STARPU_VECTOR_GET_PTR(descr[1]);

	unsigned nx = STARPU_VECTOR_GET_NX(descr[0]);
	size_t elemsize = STARPU_VECTOR_GET_ELEMSIZE(descr[0]);

	memcpy(tmp, v, nx*elemsize);

	unsigned i;
	for (i = 0; i < nx; i++)
	{
		v[i] = tmp[i] + 1;
	}
}

static struct starpu_codelet cl_f =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {cpu_f, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_f, NULL},
#endif
	.nbuffers = 2,
	.modes = {STARPU_RW, STARPU_SCRATCH}
};

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	A = (unsigned *) calloc(VECTORSIZE, sizeof(unsigned));

	starpu_vector_data_register(&A_handle, 0, (uintptr_t)A, VECTORSIZE, sizeof(unsigned));
	starpu_vector_data_register(&B_handle, -1, (uintptr_t)NULL, VECTORSIZE, sizeof(unsigned));

	unsigned loop;
	for (loop = 0; loop < NLOOPS; loop++)
	{
		struct starpu_task *task_f = starpu_task_create();
		task_f->cl = &cl_f;
		task_f->handles[0] = A_handle;
		task_f->handles[1] = B_handle;

		ret = starpu_task_submit(task_f);
		if (ret == -ENODEV) goto enodev;
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	starpu_data_unregister(A_handle);
	starpu_data_unregister(B_handle);
	starpu_shutdown();

	/* Check result */
	unsigned i;
	ret = EXIT_SUCCESS;
	for (i = 0; i < VECTORSIZE; i++)
	{
		if (A[i] != NLOOPS)
		{
			ret = EXIT_FAILURE;
			break;
		}
	}

	free(A);
	STARPU_RETURN(ret);

enodev:
	starpu_data_unregister(A_handle);
	starpu_data_unregister(B_handle);
	starpu_shutdown();
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	fprintf(stderr, "WARNING: No one can execute this task\n");
	return STARPU_TEST_SKIPPED;
}
