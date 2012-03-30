/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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
#include "../../helper.h"
#include <datawizard/coherency.h>

static int check_copy(starpu_data_handle_t handle, char *header)
{
	void *old_interface, *new_interface;
	starpu_data_handle_t new_handle;
	int ret=0;

	starpu_data_register_same(&new_handle, handle);

	if (!getenv("STARPU_SSILENT") && new_handle->ops->display)
	{
		fprintf(stderr, "%s: ", header);
		new_handle->ops->display(new_handle, stderr);
		fprintf(stderr, "\n");
	}

	old_interface = starpu_data_get_interface_on_node(handle, 0);
	new_interface = starpu_data_get_interface_on_node(new_handle, 0);

	if (new_handle->ops->compare(old_interface, new_interface) == 0)
	{
		FPRINTF(stderr, "Error when copying %s data\n", header);
		assert(0);
		ret = 1;
	}
	starpu_data_unregister(handle);
	starpu_data_unregister(new_handle);
	return ret;
}

int main(int argc, char **argv)
{
	int ret;
	starpu_data_handle_t handle;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	{
		int x=42;
		starpu_variable_data_register(&handle, 0, (uintptr_t)&x, sizeof(x));
		ret = check_copy(handle, "variable");
	}

	if (ret == 0)
	{
		int xx[] = {12, 23, 45};
		starpu_vector_data_register(&handle, 0, (uintptr_t)xx, 3, sizeof(xx[0]));
		ret = check_copy(handle, "vector");
	}

	if (ret == 0)
	{
		int NX=3;
		int NY=2;
		int matrix[NX][NY];
		starpu_matrix_data_register(&handle, 0, (uintptr_t)matrix, NX, NX, NY, sizeof(matrix[0]));
		ret = check_copy(handle, "matrix");
	}

	if (ret == 0)
	{
		int NX=3;
		int NY=2;
		int NZ=4;
		int block[NX*NY*NZ];
		starpu_block_data_register(&handle, 0, (uintptr_t)block, NX, NX*NY, NX, NY, NZ, sizeof(block[0]));
		ret = check_copy(handle, "block");
	}

	if (ret == 0)
	{
		uint32_t nnz = 2;
		unsigned nrow = 5;
		float nzvalA[20];
		uint32_t colind[1];
		uint32_t rowptr[2];
		starpu_csr_data_register(&handle, 0, nnz, nrow, (uintptr_t)nzvalA, colind, rowptr, 0, sizeof(float));
		ret = check_copy(handle, "csr");
	}

	starpu_shutdown();
	return ret;
}


