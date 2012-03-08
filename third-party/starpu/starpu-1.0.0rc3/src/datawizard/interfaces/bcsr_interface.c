/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#include <starpu.h>
#include <common/config.h>

#include <datawizard/coherency.h>
#include <datawizard/copy_driver.h>
#include <datawizard/filters.h>
#include <starpu_hash.h>

#include <starpu_cuda.h>
#include <starpu_opencl.h>
#include <drivers/opencl/driver_opencl.h>

/*
 * BCSR : blocked CSR, we use blocks of size (r x c)
 */

static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#endif
#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#endif

static const struct starpu_data_copy_methods bcsr_copy_data_methods_s =
{
	.ram_to_ram = copy_ram_to_ram,
	.ram_to_spu = NULL,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = copy_ram_to_cuda,
	.cuda_to_ram = copy_cuda_to_ram,
	.cuda_to_cuda = copy_cuda_to_cuda,
#endif
#ifdef STARPU_USE_OPENCL
	.ram_to_opencl = copy_ram_to_opencl,
	.opencl_to_ram = copy_opencl_to_ram,
#endif
	.cuda_to_spu = NULL,
	.spu_to_ram = NULL,
	.spu_to_cuda = NULL,
	.spu_to_spu = NULL
};

static void register_bcsr_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static ssize_t allocate_bcsr_buffer_on_node(void *data_interface, uint32_t dst_node);
static void free_bcsr_buffer_on_node(void *data_interface, uint32_t node);
static size_t bcsr_interface_get_size(starpu_data_handle_t handle);
static int bcsr_compare(void *data_interface_a, void *data_interface_b);
static uint32_t footprint_bcsr_interface_crc32(starpu_data_handle_t handle);


static struct starpu_data_interface_ops interface_bcsr_ops =
{
	.register_data_handle = register_bcsr_handle,
	.allocate_data_on_node = allocate_bcsr_buffer_on_node,
	.free_data_on_node = free_bcsr_buffer_on_node,
	.copy_methods = &bcsr_copy_data_methods_s,
	.get_size = bcsr_interface_get_size,
	.interfaceid = STARPU_BCSR_INTERFACE_ID,
	.interface_size = sizeof(struct starpu_bcsr_interface),
	.footprint = footprint_bcsr_interface_crc32,
	.compare = bcsr_compare
};

static void register_bcsr_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_bcsr_interface *bcsr_interface = (struct starpu_bcsr_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_bcsr_interface *local_interface = (struct starpu_bcsr_interface *)
			starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->nzval = bcsr_interface->nzval;
			local_interface->colind = bcsr_interface->colind;
			local_interface->rowptr = bcsr_interface->rowptr;
		}
		else
		{
			local_interface->nzval = 0;
			local_interface->colind = NULL;
			local_interface->rowptr = NULL;
		}

		local_interface->nnz = bcsr_interface->nnz;
		local_interface->nrow = bcsr_interface->nrow;
		local_interface->firstentry = bcsr_interface->firstentry;
		local_interface->r = bcsr_interface->r;
		local_interface->c = bcsr_interface->c;
		local_interface->elemsize = bcsr_interface->elemsize;
	}
}

void starpu_bcsr_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
		uint32_t nnz, uint32_t nrow, uintptr_t nzval, uint32_t *colind,
		uint32_t *rowptr, uint32_t firstentry,
		uint32_t r, uint32_t c, size_t elemsize)
{
	struct starpu_bcsr_interface bcsr_interface =
	{
		.nzval = nzval,
		.colind = colind,
		.rowptr = rowptr,
		.nnz = nnz,
		.nrow = nrow,
		.firstentry = firstentry,
		.r = r,
		.c = c,
		.elemsize = elemsize
	};

	starpu_data_register(handleptr, home_node, &bcsr_interface, &interface_bcsr_ops);
}

static uint32_t footprint_bcsr_interface_crc32(starpu_data_handle_t handle)
{
	uint32_t hash;

	hash = starpu_crc32_be(starpu_bcsr_get_nnz(handle), 0);
	hash = starpu_crc32_be(starpu_bcsr_get_c(handle), hash);
	hash = starpu_crc32_be(starpu_bcsr_get_r(handle), hash);

	return hash;
}

static int bcsr_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_bcsr_interface *bcsr_a = (struct starpu_bcsr_interface *) data_interface_a;
	struct starpu_bcsr_interface *bcsr_b = (struct starpu_bcsr_interface *) data_interface_b;

	/* Two matricess are considered compatible if they have the same size */
	return ((bcsr_a->nnz == bcsr_b->nnz)
			&& (bcsr_a->nrow == bcsr_b->nrow)
			&& (bcsr_a->r == bcsr_b->r)
			&& (bcsr_a->c == bcsr_b->c)
			&& (bcsr_a->elemsize == bcsr_b->elemsize));
}

/* offer an access to the data parameters */
uint32_t starpu_bcsr_get_nnz(starpu_data_handle_t handle)
{
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->nnz;
}

uint32_t starpu_bcsr_get_nrow(starpu_data_handle_t handle)
{
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->nrow;
}

uint32_t starpu_bcsr_get_firstentry(starpu_data_handle_t handle)
{
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->firstentry;
}

uint32_t starpu_bcsr_get_r(starpu_data_handle_t handle)
{
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->r;
}

uint32_t starpu_bcsr_get_c(starpu_data_handle_t handle)
{
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->c;
}

size_t starpu_bcsr_get_elemsize(starpu_data_handle_t handle)
{
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->elemsize;
}

uintptr_t starpu_bcsr_get_local_nzval(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return data_interface->nzval;
}

uint32_t *starpu_bcsr_get_local_colind(starpu_data_handle_t handle)
{
	/* XXX 0 */
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->colind;
}

uint32_t *starpu_bcsr_get_local_rowptr(starpu_data_handle_t handle)
{
	/* XXX 0 */
	struct starpu_bcsr_interface *data_interface = (struct starpu_bcsr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return data_interface->rowptr;
}


static size_t bcsr_interface_get_size(starpu_data_handle_t handle)
{
	size_t size;

	uint32_t nnz = starpu_bcsr_get_nnz(handle);
	uint32_t nrow = starpu_bcsr_get_nrow(handle);
	uint32_t r = starpu_bcsr_get_r(handle);
	uint32_t c = starpu_bcsr_get_c(handle);
	size_t elemsize = starpu_bcsr_get_elemsize(handle);

	size = nnz*r*c*elemsize + nnz*sizeof(uint32_t) + (nrow+1)*sizeof(uint32_t);

	return size;
}


/* memory allocation/deallocation primitives for the BLAS interface */

/* returns the size of the allocated area */
static ssize_t allocate_bcsr_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	uintptr_t addr_nzval = 0;
	uint32_t *addr_colind = NULL, *addr_rowptr = NULL;
	ssize_t allocated_memory;

	/* we need the 3 arrays to be allocated */
	struct starpu_bcsr_interface *bcsr_interface = (struct starpu_bcsr_interface *) data_interface_;

	uint32_t nnz = bcsr_interface->nnz;
	uint32_t nrow = bcsr_interface->nrow;
	size_t elemsize = bcsr_interface->elemsize;

	uint32_t r = bcsr_interface->r;
	uint32_t c = bcsr_interface->c;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);

	switch(kind)
	{
		case STARPU_CPU_RAM:
			addr_nzval = (uintptr_t)malloc(nnz*r*c*elemsize);
			if (!addr_nzval)
				goto fail_nzval;

			addr_colind = (uint32_t *) malloc(nnz*sizeof(uint32_t));
			if (!addr_colind)
				goto fail_colind;

			addr_rowptr = (uint32_t *) malloc((nrow+1)*sizeof(uint32_t));
			if (!addr_rowptr)
				goto fail_rowptr;

			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			cudaMalloc((void **)&addr_nzval, nnz*r*c*elemsize);
			if (!addr_nzval)
				goto fail_nzval;

			cudaMalloc((void **)&addr_colind, nnz*sizeof(uint32_t));
			if (!addr_colind)
				goto fail_colind;

			cudaMalloc((void **)&addr_rowptr, (nrow+1)*sizeof(uint32_t));
			if (!addr_rowptr)
				goto fail_rowptr;

			break;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
		{
			int ret;
			cl_mem ptr;

			ret = starpu_opencl_allocate_memory(&ptr, nnz*r*c*elemsize, CL_MEM_READ_WRITE);
			addr_nzval = (uintptr_t)ptr;
			if (ret) goto fail_nzval;

			ret = starpu_opencl_allocate_memory(&ptr, nnz*sizeof(uint32_t), CL_MEM_READ_WRITE);
			addr_colind = (void*) ptr;
			if (ret) goto fail_colind;

			ret = starpu_opencl_allocate_memory(&ptr, (nrow+1)*sizeof(uint32_t), CL_MEM_READ_WRITE);
			addr_rowptr = (void*) ptr;
			if (ret) goto fail_rowptr;

			break;
		}
#endif
		default:
			STARPU_ASSERT(0);
	}

	/* allocation succeeded */
	allocated_memory =
		nnz*r*c*elemsize + nnz*sizeof(uint32_t) + (nrow+1)*sizeof(uint32_t);

	/* update the data properly in consequence */
	bcsr_interface->nzval = addr_nzval;
	bcsr_interface->colind = addr_colind;
	bcsr_interface->rowptr = addr_rowptr;

	return allocated_memory;

fail_rowptr:
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void *)addr_colind);
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			cudaFree((void*)addr_colind);
			break;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			clReleaseMemObject((void*)addr_colind);
			break;
#endif
		default:
			STARPU_ASSERT(0);
	}

fail_colind:
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void *)addr_nzval);
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			cudaFree((void*)addr_nzval);
			break;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			clReleaseMemObject((void*)addr_nzval);
			break;
#endif
		default:
			STARPU_ASSERT(0);
	}

fail_nzval:

	/* allocation failed */
	return -ENOMEM;
}

static void free_bcsr_buffer_on_node(void *data_interface, uint32_t node)
{
	struct starpu_bcsr_interface *bcsr_interface = (struct starpu_bcsr_interface *) data_interface;

	enum starpu_node_kind kind = starpu_node_get_kind(node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void*)bcsr_interface->nzval);
			free((void*)bcsr_interface->colind);
			free((void*)bcsr_interface->rowptr);
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			cudaFree((void*)bcsr_interface->nzval);
			cudaFree((void*)bcsr_interface->colind);
			cudaFree((void*)bcsr_interface->rowptr);
			break;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			clReleaseMemObject((void*)bcsr_interface->nzval);
			clReleaseMemObject((void*)bcsr_interface->colind);
			clReleaseMemObject((void*)bcsr_interface->rowptr);
			break;
#endif
		default:
			STARPU_ASSERT(0);
	}
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, enum cudaMemcpyKind kind)
{
	struct starpu_bcsr_interface *src_bcsr = src_interface;
	struct starpu_bcsr_interface *dst_bcsr = dst_interface;

	uint32_t nnz = src_bcsr->nnz;
	uint32_t nrow = src_bcsr->nrow;
	size_t elemsize = src_bcsr->elemsize;

	uint32_t r = src_bcsr->r;
	uint32_t c = src_bcsr->c;

	cudaError_t cures;

	cures = cudaMemcpy((char *)dst_bcsr->nzval, (char *)src_bcsr->nzval, nnz*r*c*elemsize, kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpy((char *)dst_bcsr->colind, (char *)src_bcsr->colind, nnz*sizeof(uint32_t), kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpy((char *)dst_bcsr->rowptr, (char *)src_bcsr->rowptr, (nrow+1)*sizeof(uint32_t), kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*r*c*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}

static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost);
}

static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice);
}

static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice);
}
#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_bcsr_interface *src_bcsr = src_interface;
	struct starpu_bcsr_interface *dst_bcsr = dst_interface;

	uint32_t nnz = src_bcsr->nnz;
	uint32_t nrow = src_bcsr->nrow;
	size_t elemsize = src_bcsr->elemsize;

	uint32_t r = src_bcsr->r;
	uint32_t c = src_bcsr->c;

        int err;

	err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_bcsr->nzval, src_node, (void *)dst_bcsr->nzval, dst_node, nnz*r*c*elemsize, 0, NULL);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_bcsr->colind, src_node, (void *)dst_bcsr->colind, dst_node, nnz*sizeof(uint32_t), 0, NULL);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_bcsr->rowptr, src_node, (void *)dst_bcsr->rowptr, dst_node, (nrow+1)*sizeof(uint32_t), 0, NULL);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*r*c*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}

static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_bcsr_interface *src_bcsr = src_interface;
	struct starpu_bcsr_interface *dst_bcsr = dst_interface;

	uint32_t nnz = src_bcsr->nnz;
	uint32_t nrow = src_bcsr->nrow;
	size_t elemsize = src_bcsr->elemsize;

	uint32_t r = src_bcsr->r;
	uint32_t c = src_bcsr->c;

        int err;

	err = starpu_opencl_copy_ram_to_opencl((void *)src_bcsr->nzval, src_node, (cl_mem)dst_bcsr->nzval, dst_node, nnz*r*c*elemsize, 0, NULL);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	err = starpu_opencl_copy_ram_to_opencl((void *)src_bcsr->colind, src_node, (cl_mem)dst_bcsr->colind, dst_node, nnz*sizeof(uint32_t), 0, NULL);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	err = starpu_opencl_copy_ram_to_opencl((void *)src_bcsr->rowptr, src_node, (cl_mem)dst_bcsr->rowptr, dst_node, (nrow+1)*sizeof(uint32_t), 0, NULL);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*r*c*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}
#endif // STARPU_USE_OPENCL

/* as not all platform easily have a BLAS lib installed ... */
static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_bcsr_interface *src_bcsr = (struct starpu_bcsr_interface *) src_interface;
	struct starpu_bcsr_interface *dst_bcsr = (struct starpu_bcsr_interface *) dst_interface;

	uint32_t nnz = src_bcsr->nnz;
	uint32_t nrow = src_bcsr->nrow;
	size_t elemsize = src_bcsr->elemsize;

	uint32_t r = src_bcsr->r;
	uint32_t c = src_bcsr->c;

	memcpy((void *)dst_bcsr->nzval, (void *)src_bcsr->nzval, nnz*elemsize*r*c);

	memcpy((void *)dst_bcsr->colind, (void *)src_bcsr->colind, nnz*sizeof(uint32_t));

	memcpy((void *)dst_bcsr->rowptr, (void *)src_bcsr->rowptr, (nrow+1)*sizeof(uint32_t));

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize*r*c + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}
