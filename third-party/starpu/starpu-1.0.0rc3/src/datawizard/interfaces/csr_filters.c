/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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
#include <common/config.h>
#include <datawizard/filters.h>

void starpu_vertical_block_filter_func_csr(void *father_interface, void *child_interface, STARPU_ATTRIBUTE_UNUSED struct starpu_data_filter *f, unsigned id, unsigned nchunks)
{
	struct starpu_csr_interface *csr_father = (struct starpu_csr_interface *) father_interface;
	struct starpu_csr_interface *csr_child = (struct starpu_csr_interface *) child_interface;

	uint32_t nrow = csr_father->nrow;
	size_t elemsize = csr_father->elemsize;
	uint32_t firstentry = csr_father->firstentry;

	uint32_t chunk_size = (nrow + nchunks - 1)/nchunks;

	uint32_t *rowptr = csr_father->rowptr;

	uint32_t first_index = id*chunk_size - firstentry;
	uint32_t local_firstentry = rowptr[first_index];

	uint32_t child_nrow =
	  STARPU_MIN(chunk_size, nrow - id*chunk_size);

	uint32_t local_nnz = rowptr[first_index + child_nrow] - rowptr[first_index];

	csr_child->nnz = local_nnz;
	csr_child->nrow = child_nrow;
	csr_child->firstentry = local_firstentry;
	csr_child->elemsize = elemsize;

	if (csr_father->nzval)
	{
		csr_child->rowptr = &csr_father->rowptr[first_index];
		csr_child->colind = &csr_father->colind[local_firstentry];
		csr_child->nzval = csr_father->nzval + local_firstentry * elemsize;
	}
}
