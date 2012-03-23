/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

#include <starpu.h>
#include <common/config.h>
#include <datawizard/coherency.h>
#include <datawizard/copy_driver.h>
#include <datawizard/filters.h>

#include <starpu_hash.h>

#include <starpu_cuda.h>
#include <starpu_opencl.h>
#include <drivers/opencl/driver_opencl.h>

static int copy_ram_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
#endif
#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
#endif

static struct starpu_data_copy_methods csr_copy_data_methods_s =
{
	.ram_to_ram = copy_ram_to_ram,
	.ram_to_spu = NULL,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = copy_ram_to_cuda,
	.cuda_to_ram = copy_cuda_to_ram,
	.cuda_to_cuda = copy_cuda_to_cuda,
	.ram_to_cuda_async = copy_ram_to_cuda_async,
	.cuda_to_ram_async = copy_cuda_to_ram_async,
	.cuda_to_cuda_async = copy_cuda_to_cuda_async,
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

static void register_csr_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static ssize_t allocate_csr_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void free_csr_buffer_on_node(void *data_interface, uint32_t node);
static size_t csr_interface_get_size(starpu_data_handle_t handle);
static int csr_compare(void *data_interface_a, void *data_interface_b);
static uint32_t footprint_csr_interface_crc32(starpu_data_handle_t handle);

static struct starpu_data_interface_ops interface_csr_ops =
{
	.register_data_handle = register_csr_handle,
	.allocate_data_on_node = allocate_csr_buffer_on_node,
	.free_data_on_node = free_csr_buffer_on_node,
	.copy_methods = &csr_copy_data_methods_s,
	.get_size = csr_interface_get_size,
	.interfaceid = STARPU_CSR_INTERFACE_ID,
	.interface_size = sizeof(struct starpu_csr_interface),
	.footprint = footprint_csr_interface_crc32,
	.compare = csr_compare,
};

static void register_csr_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_csr_interface *local_interface = (struct starpu_csr_interface *)
			starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->nzval = csr_interface->nzval;
			local_interface->colind = csr_interface->colind;
		}
		else
		{
			local_interface->nzval = 0;
			local_interface->colind = NULL;
		}

		local_interface->rowptr = csr_interface->rowptr;
		local_interface->nnz = csr_interface->nnz;
		local_interface->nrow = csr_interface->nrow;
		local_interface->firstentry = csr_interface->firstentry;
		local_interface->elemsize = csr_interface->elemsize;

	}
}

/* declare a new data with the BLAS interface */
void starpu_csr_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
		uint32_t nnz, uint32_t nrow, uintptr_t nzval, uint32_t *colind, uint32_t *rowptr, uint32_t firstentry, size_t elemsize)
{
	struct starpu_csr_interface csr_interface =
	{
		.nnz = nnz,
		.nrow = nrow,
		.nzval = nzval,
		.colind = colind,
		.rowptr = rowptr,
		.firstentry = firstentry,
		.elemsize = elemsize
	};

	starpu_data_register(handleptr, home_node, &csr_interface, &interface_csr_ops);
}

static uint32_t footprint_csr_interface_crc32(starpu_data_handle_t handle)
{
	return starpu_crc32_be(starpu_csr_get_nnz(handle), 0);
}

static int csr_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_csr_interface *csr_a = (struct starpu_csr_interface *) data_interface_a;
	struct starpu_csr_interface *csr_b = (struct starpu_csr_interface *) data_interface_b;

	/* Two matricess are considered compatible if they have the same size */
	return ((csr_a->nnz == csr_b->nnz)
			&& (csr_a->nrow == csr_b->nrow)
			&& (csr_a->elemsize == csr_b->elemsize));
}

/* offer an access to the data parameters */
uint32_t starpu_csr_get_nnz(starpu_data_handle_t handle)
{
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return csr_interface->nnz;
}

uint32_t starpu_csr_get_nrow(starpu_data_handle_t handle)
{
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return csr_interface->nrow;
}

uint32_t starpu_csr_get_firstentry(starpu_data_handle_t handle)
{
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return csr_interface->firstentry;
}

size_t starpu_csr_get_elemsize(starpu_data_handle_t handle)
{
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return csr_interface->elemsize;
}

uintptr_t starpu_csr_get_local_nzval(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return csr_interface->nzval;
}

uint32_t *starpu_csr_get_local_colind(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return csr_interface->colind;
}

uint32_t *starpu_csr_get_local_rowptr(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return csr_interface->rowptr;
}

static size_t csr_interface_get_size(starpu_data_handle_t handle)
{
	size_t size;

	uint32_t nnz = starpu_csr_get_nnz(handle);
	uint32_t nrow = starpu_csr_get_nrow(handle);
	size_t elemsize = starpu_csr_get_elemsize(handle);

	size = nnz*elemsize + nnz*sizeof(uint32_t) + (nrow+1)*sizeof(uint32_t);

	return size;
}

/* memory allocation/deallocation primitives for the BLAS interface */

/* returns the size of the allocated area */
static ssize_t allocate_csr_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	uintptr_t addr_nzval = 0;
	uint32_t *addr_colind = NULL, *addr_rowptr = NULL;
	ssize_t allocated_memory;

	/* we need the 3 arrays to be allocated */
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *) data_interface_;

	uint32_t nnz = csr_interface->nnz;
	uint32_t nrow = csr_interface->nrow;
	size_t elemsize = csr_interface->elemsize;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);

	switch(kind)
	{
		case STARPU_CPU_RAM:
			addr_nzval = (uintptr_t)malloc(nnz*elemsize);
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
			cudaMalloc((void **)&addr_nzval, nnz*elemsize);
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

                                ret = starpu_opencl_allocate_memory(&ptr, nnz*elemsize, CL_MEM_READ_WRITE);
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
			STARPU_ABORT();
	}

	/* allocation succeeded */
	allocated_memory =
		nnz*elemsize + nnz*sizeof(uint32_t) + (nrow+1)*sizeof(uint32_t);

	/* update the data properly in consequence */
	csr_interface->nzval = addr_nzval;
	csr_interface->colind = addr_colind;
	csr_interface->rowptr = addr_rowptr;

	return allocated_memory;

fail_rowptr:
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void *)addr_colind);
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
		{
			cudaError_t err;
			err = cudaFree((void*)addr_colind);
			if (STARPU_UNLIKELY(err != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(err);
			break;
		}
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			clReleaseMemObject((void*)addr_colind);
			break;
#endif
		default:
			STARPU_ABORT();
	}

fail_colind:
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void *)addr_nzval);
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
		{
			cudaError_t err;
			err = cudaFree((void*)addr_nzval);
			if (STARPU_UNLIKELY(err != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(err);
			break;
		}
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			clReleaseMemObject((void*)addr_nzval);
			break;
#endif
		default:
			STARPU_ABORT();
	}

fail_nzval:

	/* allocation failed */
	return -ENOMEM;
}

static void free_csr_buffer_on_node(void *data_interface, uint32_t node)
{
	struct starpu_csr_interface *csr_interface = (struct starpu_csr_interface *) data_interface;

	enum starpu_node_kind kind = starpu_node_get_kind(node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void*)csr_interface->nzval);
			free((void*)csr_interface->colind);
			free((void*)csr_interface->rowptr);
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
		{
			cudaError_t err;
			err = cudaFree((void*)csr_interface->nzval);
			if (STARPU_UNLIKELY(err != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(err);
			err = cudaFree((void*)csr_interface->colind);
			if (STARPU_UNLIKELY(err != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(err);
			err = cudaFree((void*)csr_interface->rowptr);
			if (STARPU_UNLIKELY(err != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(err);
			break;
		}
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			clReleaseMemObject((void*)csr_interface->nzval);
			clReleaseMemObject((void*)csr_interface->colind);
			clReleaseMemObject((void*)csr_interface->rowptr);
			break;
#endif
		default:
			STARPU_ABORT();
	}
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, enum cudaMemcpyKind kind)
{
	struct starpu_csr_interface *src_csr = src_interface;
	struct starpu_csr_interface *dst_csr = dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

	cudaError_t cures;

	cures = cudaMemcpy((char *)dst_csr->nzval, (char *)src_csr->nzval, nnz*elemsize, kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpy((char *)dst_csr->colind, (char *)src_csr->colind, nnz*sizeof(uint32_t), kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpy((char *)dst_csr->rowptr, (char *)src_csr->rowptr, (nrow+1)*sizeof(uint32_t), kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}

static int copy_cuda_common_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, enum cudaMemcpyKind kind, cudaStream_t stream)
{
	struct starpu_csr_interface *src_csr = src_interface;
	struct starpu_csr_interface *dst_csr = dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

	cudaError_t cures;

	int synchronous_fallback = 0;

	_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
	cures = cudaMemcpyAsync((char *)dst_csr->nzval, (char *)src_csr->nzval, nnz*elemsize, kind, stream);
	if (cures)
	{
		synchronous_fallback = 1;
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpy((char *)dst_csr->nzval, (char *)src_csr->nzval, nnz*elemsize, kind);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	if (!synchronous_fallback)
	{
		cures = cudaMemcpyAsync((char *)dst_csr->colind, (char *)src_csr->colind, nnz*sizeof(uint32_t), kind, stream);
	}

	if (synchronous_fallback || cures != cudaSuccess)
	{
		synchronous_fallback = 1;
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpy((char *)dst_csr->colind, (char *)src_csr->colind, nnz*sizeof(uint32_t), kind);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	if (!synchronous_fallback)
	{
		cures = cudaMemcpyAsync((char *)dst_csr->rowptr, (char *)src_csr->rowptr, (nrow+1)*sizeof(uint32_t), kind, stream);
	}

	if (synchronous_fallback || cures != cudaSuccess)
	{
		synchronous_fallback = 1;
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpy((char *)dst_csr->rowptr, (char *)src_csr->rowptr, (nrow+1)*sizeof(uint32_t), kind);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	if (synchronous_fallback)
	{
		_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));
		return 0;
	}
	else
	{
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		return -EAGAIN;
	}
}

static int copy_cuda_peer(void *src_interface STARPU_ATTRIBUTE_UNUSED, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface STARPU_ATTRIBUTE_UNUSED, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
#ifdef HAVE_CUDA_MEMCPY_PEER
	struct starpu_csr_interface *src_csr = src_interface;
	struct starpu_csr_interface *dst_csr = dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

	int src_dev = _starpu_memory_node_to_devid(src_node);
	int dst_dev = _starpu_memory_node_to_devid(dst_node);

	cudaError_t cures;

	cures = cudaMemcpyPeer((char *)dst_csr->nzval, dst_dev, (char *)src_csr->nzval, src_dev, nnz*elemsize);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpyPeer((char *)dst_csr->colind, dst_dev, (char *)src_csr->colind, src_dev, nnz*sizeof(uint32_t));
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaMemcpyPeer((char *)dst_csr->rowptr, dst_dev, (char *)src_csr->rowptr, src_dev, (nrow+1)*sizeof(uint32_t));
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
#else
	STARPU_ABORT();
	return 0;
#endif
}

static int copy_cuda_peer_async(void *src_interface STARPU_ATTRIBUTE_UNUSED, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface STARPU_ATTRIBUTE_UNUSED, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream STARPU_ATTRIBUTE_UNUSED)
{
#ifdef HAVE_CUDA_MEMCPY_PEER
	struct starpu_csr_interface *src_csr = src_interface;
	struct starpu_csr_interface *dst_csr = dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

	cudaError_t cures;

	int src_dev = _starpu_memory_node_to_devid(src_node);
	int dst_dev = _starpu_memory_node_to_devid(dst_node);

	int synchronous_fallback = 0;

	_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
	cures = cudaMemcpyPeerAsync((char *)dst_csr->nzval, dst_dev, (char *)src_csr->nzval, src_dev, nnz*elemsize, stream);
	if (cures)
	{
		synchronous_fallback = 1;
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpyPeer((char *)dst_csr->nzval, dst_dev, (char *)src_csr->nzval, src_dev, nnz*elemsize);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	if (!synchronous_fallback)
	{
		cures = cudaMemcpyPeerAsync((char *)dst_csr->colind, dst_dev, (char *)src_csr->colind, src_dev, nnz*sizeof(uint32_t), stream);
	}

	if (synchronous_fallback || cures != cudaSuccess)
	{
		synchronous_fallback = 1;
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpyPeer((char *)dst_csr->colind, dst_dev, (char *)src_csr->colind, src_dev, nnz*sizeof(uint32_t));
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	if (!synchronous_fallback)
	{
		cures = cudaMemcpyPeerAsync((char *)dst_csr->rowptr, dst_dev, (char *)src_csr->rowptr, src_dev, (nrow+1)*sizeof(uint32_t), stream);
	}

	if (synchronous_fallback || cures != cudaSuccess)
	{
		synchronous_fallback = 1;
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpyPeer((char *)dst_csr->rowptr, dst_dev, (char *)src_csr->rowptr, src_dev, (nrow+1)*sizeof(uint32_t));
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	if (synchronous_fallback)
	{
		_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));
		return 0;
	}
	else
	{
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		return -EAGAIN;
	}
#else
	/* Illegal without Peer tranfers */
	STARPU_ABORT();
	return 0;
#endif
}

static int copy_cuda_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost);
}

static int copy_ram_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice);
}

static int copy_cuda_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node)
{
	if (src_node == dst_node)
		return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice);
	else
		return copy_cuda_peer(src_interface, src_node, dst_interface, dst_node);
}

static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	return copy_cuda_common_async(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost, stream);
}

static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	return copy_cuda_common_async(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice, stream);
}

static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	if (src_node == dst_node)
		return copy_cuda_common_async(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice, stream);
	else
		return copy_cuda_peer_async(src_interface, src_node, dst_interface, dst_node, stream);
}

#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_csr_interface *src_csr = src_interface;
	struct starpu_csr_interface *dst_csr = dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

        int err;

        err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_csr->nzval, src_node, (void *)dst_csr->nzval, dst_node, nnz*elemsize, 0, NULL);
	if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_csr->colind, src_node, (void *)dst_csr->colind, dst_node, nnz*sizeof(uint32_t), 0, NULL);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

        err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_csr->rowptr, src_node, (void *)dst_csr->rowptr, dst_node, (nrow+1)*sizeof(uint32_t), 0, NULL);
	if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}

static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_csr_interface *src_csr = src_interface;
	struct starpu_csr_interface *dst_csr = dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

        int err;

        err = starpu_opencl_copy_ram_to_opencl((void *)src_csr->nzval, src_node, (cl_mem)dst_csr->nzval, dst_node, nnz*elemsize, 0, NULL);
	if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	err = starpu_opencl_copy_ram_to_opencl((void *)src_csr->colind, src_node, (cl_mem)dst_csr->colind, dst_node, nnz*sizeof(uint32_t), 0, NULL);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

        err = starpu_opencl_copy_ram_to_opencl((void *)src_csr->rowptr, src_node, (cl_mem)dst_csr->rowptr, dst_node, (nrow+1)*sizeof(uint32_t), 0, NULL);
	if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}
#endif // STARPU_USE_OPENCL

/* as not all platform easily have a BLAS lib installed ... */
static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_csr_interface *src_csr = (struct starpu_csr_interface *) src_interface;
	struct starpu_csr_interface *dst_csr = (struct starpu_csr_interface *) dst_interface;

	uint32_t nnz = src_csr->nnz;
	uint32_t nrow = src_csr->nrow;
	size_t elemsize = src_csr->elemsize;

	memcpy((void *)dst_csr->nzval, (void *)src_csr->nzval, nnz*elemsize);

	memcpy((void *)dst_csr->colind, (void *)src_csr->colind, nnz*sizeof(uint32_t));

	memcpy((void *)dst_csr->rowptr, (void *)src_csr->rowptr, (nrow+1)*sizeof(uint32_t));

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nnz*elemsize + (nnz+nrow+1)*sizeof(uint32_t));

	return 0;
}
