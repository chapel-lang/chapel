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
#include <starpu_cuda.h>
#include <starpu_hash.h>

#include "complex_interface.h"

double *starpu_complex_get_real(starpu_data_handle_t handle)
{
	struct starpu_complex_interface *complex_interface =
		(struct starpu_complex_interface *) starpu_data_get_interface_on_node(handle, 0);

	return complex_interface->real;
}

double *starpu_complex_get_imaginary(starpu_data_handle_t handle)
{
	struct starpu_complex_interface *complex_interface =
		(struct starpu_complex_interface *) starpu_data_get_interface_on_node(handle, 0);

	return complex_interface->imaginary;
}

int starpu_complex_get_nx(starpu_data_handle_t handle)
{
	struct starpu_complex_interface *complex_interface =
		(struct starpu_complex_interface *) starpu_data_get_interface_on_node(handle, 0);

	return complex_interface->nx;
}

static void complex_register_data_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_complex_interface *complex_interface = (struct starpu_complex_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_complex_interface *local_interface = (struct starpu_complex_interface *)
			starpu_data_get_interface_on_node(handle, node);

		local_interface->real = complex_interface->real;
		local_interface->imaginary = complex_interface->imaginary;
		local_interface->nx = complex_interface->nx;
	}
}

static starpu_ssize_t complex_allocate_data_on_node(void *data_interface, uint32_t node)
{
	struct starpu_complex_interface *complex_interface = (struct starpu_complex_interface *) data_interface;

	unsigned fail = 0;
	double *addr_real = 0;
	double *addr_imaginary = 0;
	ssize_t requested_memory = complex_interface->nx * sizeof(complex_interface->real[0]);

	enum starpu_node_kind kind = starpu_node_get_kind(node);

	switch(kind)
	{
		case STARPU_CPU_RAM:
			addr_real = malloc(requested_memory);
			addr_imaginary = malloc(requested_memory);
			if (!addr_real || !addr_imaginary)
				fail = 1;
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
		{
			cudaError_t status;
			status = cudaMalloc((void **)&addr_real, requested_memory);
			if (!addr_real || (status != cudaSuccess))
			{
				if (STARPU_UNLIKELY(status != cudaErrorMemoryAllocation))
					STARPU_CUDA_REPORT_ERROR(status);

				fail = 1;
			}
			else
			{
				status = cudaMalloc((void **)&addr_imaginary, requested_memory);
				if (!addr_imaginary || (status != cudaSuccess))
				{
					if (STARPU_UNLIKELY(status != cudaErrorMemoryAllocation))
						STARPU_CUDA_REPORT_ERROR(status);

					fail = 1;
				}
			}

			break;
		}
#endif
#ifdef STARPU_USE_OPENCL
	        case STARPU_OPENCL_RAM:
		{
			STARPU_ASSERT(0);
		}
#endif
		default:
			STARPU_ASSERT(0);
	}

	if (fail)
		return -ENOMEM;

	/* update the data properly in consequence */
	complex_interface->real = addr_real;
	complex_interface->imaginary = addr_imaginary;

	return 2*requested_memory;
}

static size_t complex_get_size(starpu_data_handle_t handle)
{
	size_t size;
	struct starpu_complex_interface *complex_interface = (struct starpu_complex_interface *) starpu_data_get_interface_on_node(handle, 0);

	size = complex_interface->nx * 2 * sizeof(double);
	return size;
}

static uint32_t complex_footprint(starpu_data_handle_t handle)
{
	return starpu_crc32_be(starpu_complex_get_nx(handle), 0);
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, enum cudaMemcpyKind kind)
{
	struct starpu_complex_interface *src_complex = src_interface;
	struct starpu_complex_interface *dst_complex = dst_interface;

	cudaError_t cures;

	cures = cudaMemcpy((void *)dst_complex->real, (void *)src_complex->real, src_complex->nx*sizeof(src_complex->real[0]), kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpy((char *)dst_complex->imaginary, (char *)src_complex->imaginary, src_complex->nx*sizeof(src_complex->imaginary[0]), kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	return 0;
}

static int copy_ram_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice);
}

static int copy_cuda_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost);
}
#endif


static const struct starpu_data_copy_methods complex_copy_methods =
{
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = copy_ram_to_cuda,
	.cuda_to_ram = copy_cuda_to_ram,
#endif
};

static struct starpu_data_interface_ops interface_complex_ops =
{
	.register_data_handle = complex_register_data_handle,
	.allocate_data_on_node = complex_allocate_data_on_node,
	.copy_methods = &complex_copy_methods,
	.get_size = complex_get_size,
	.footprint = complex_footprint,
	.interfaceid = -1,
	.interface_size = sizeof(struct starpu_complex_interface),
};

void starpu_complex_data_register(starpu_data_handle_t *handleptr, uint32_t home_node, double *real, double *imaginary, int nx)
{
	struct starpu_complex_interface complex =
	{
		.real = real,
		.imaginary = imaginary,
		.nx = nx
	};

	if (interface_complex_ops.interfaceid == -1)
	{
		interface_complex_ops.interfaceid = starpu_data_interface_get_next_id();
	}

	starpu_data_register(handleptr, home_node, &complex, &interface_complex_ops);
}
