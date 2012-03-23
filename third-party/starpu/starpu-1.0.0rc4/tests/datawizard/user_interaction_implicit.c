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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <starpu.h>
#include <stdlib.h>
#include <pthread.h>
#include "../helper.h"

#define NBUFFERS	16
#define NITER		128

struct data
{
	unsigned index;
	unsigned val;
	starpu_data_handle_t handle;
};

struct data buffers[NBUFFERS];

void callback_sync_data(void *arg)
{
	struct data *data = (struct data *) arg;

	data->val++;

	starpu_data_release(data->handle);
}

int main(int argc, char **argv)
{
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	unsigned b;
	for (b = 0; b < NBUFFERS; b++)
	{
		buffers[b].index = b;
		starpu_variable_data_register(&buffers[b].handle, 0, (uintptr_t)&buffers[b].val, sizeof(unsigned));
	}

	unsigned iter;
	for (iter = 0; iter < NITER; iter++)
	for (b = 0; b < NBUFFERS; b++)
	{
		ret = starpu_data_acquire_cb(buffers[b].handle, STARPU_RW,
					     callback_sync_data, &buffers[b]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire_cb");
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	/* do some cleanup */
	ret = EXIT_SUCCESS;
	for (b = 0; b < NBUFFERS; b++)
	{
		starpu_data_unregister(buffers[b].handle);

		/* check result */
		if (buffers[b].val != NITER)
		{
			FPRINTF(stderr, "buffer[%u] = %u should be %d\n", b, buffers[b].val, NITER);
			ret = EXIT_FAILURE;
		}
	}

	starpu_shutdown();

	return ret;
}
