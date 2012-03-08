/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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

#undef NDEBUG
#include <assert.h>

#include <starpu.h>
#include <stdlib.h>
#include "../helper.h"

static void cpu_task(void **buffers, void *args)
{
	int *numbers;
	int i;
	size_t size;

	numbers = (int *) STARPU_VECTOR_GET_PTR(buffers[0]);
	starpu_codelet_unpack_args (args, &size);

	for(i = 0; i < size; i++)
	{
		numbers[i] = i;
	}
}

#ifdef STARPU_USE_CUDA
static void cuda_task(void **buffers, void *args)
{
	int *numbers;
	int i;
	size_t size;

	numbers = (int *)STARPU_VECTOR_GET_PTR(buffers[0]);
	starpu_codelet_unpack_args (args, &size);

	for(i = 0; i < size; i++)
	{
		cudaMemcpy(&numbers[i], &i, sizeof(int), cudaMemcpyHostToDevice);
	}
}
#endif

static struct starpu_codelet cl =
{
	.where = STARPU_CPU | STARPU_CUDA,
	.cpu_funcs = {cpu_task, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_task, NULL},
#endif
	.nbuffers = 1,
	.modes = {STARPU_W}
};

int main(int argc, char *argv[])
{
	int err, ret;
	size_t i;
	int *pointer;
	starpu_data_handle_t handle;
	static const size_t count = 123;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;

	err = starpu_malloc((void **)&pointer, count * sizeof(int));
	STARPU_ASSERT((err == 0) && (pointer != NULL));

	starpu_variable_data_register(&handle, 0, (uintptr_t)pointer,
				      sizeof(int));
	STARPU_ASSERT(starpu_handle_to_pointer(handle, 0) == pointer);
	starpu_data_unregister(handle);

	starpu_vector_data_register(&handle, 0, (uintptr_t)pointer,
				    count, sizeof(int));
	STARPU_ASSERT(starpu_handle_to_pointer(handle, 0) == pointer);
	starpu_data_unregister(handle);

	starpu_matrix_data_register(&handle, 0, (uintptr_t)pointer, 0,
				    count, 1, sizeof(int));
	STARPU_ASSERT(starpu_handle_to_pointer(handle, 0) == pointer);
	starpu_data_unregister(handle);

	starpu_free(pointer);
	pointer = NULL;

	/* Lazy allocation.  */
	starpu_vector_data_register(&handle, -1, 0 /* NULL */,
				    count, sizeof(int));
	STARPU_ASSERT(starpu_handle_to_pointer(handle, 0) == NULL);

	/* Pass the handle to a task.  */
	err = starpu_insert_task(&cl,
			   STARPU_W, handle,
			   STARPU_VALUE, &count, sizeof(count),
			   0);
	if (err == -ENODEV)
		return STARPU_TEST_SKIPPED;

	/* Acquire the handle, forcing a local allocation.  */
	starpu_data_acquire(handle, STARPU_R);

	/* Make sure we have a local pointer to it.  */
	pointer = (int *) starpu_handle_to_pointer(handle, 0);
	STARPU_ASSERT(pointer != NULL);
	for(i = 0; i < count; i++)
	{
		int *numbers = (int *)pointer;
		STARPU_ASSERT(numbers[i] == i);
	}
	starpu_data_release(handle);

	starpu_data_unregister(handle);

	starpu_shutdown();

	return EXIT_SUCCESS;
}
