/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#endif
#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event);
static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event);
#endif

static const struct starpu_data_copy_methods block_copy_data_methods_s =
{
	.ram_to_ram = copy_ram_to_ram,
	.ram_to_spu = NULL,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = copy_ram_to_cuda,
	.cuda_to_ram = copy_cuda_to_ram,
	.ram_to_cuda_async = copy_ram_to_cuda_async,
	.cuda_to_ram_async = copy_cuda_to_ram_async,
	.cuda_to_cuda = copy_cuda_to_cuda,
#endif
#ifdef STARPU_USE_OPENCL
	.ram_to_opencl = copy_ram_to_opencl,
	.opencl_to_ram = copy_opencl_to_ram,
        .ram_to_opencl_async = copy_ram_to_opencl_async,
	.opencl_to_ram_async = copy_opencl_to_ram_async,
#endif
	.cuda_to_spu = NULL,
	.spu_to_ram = NULL,
	.spu_to_cuda = NULL,
	.spu_to_spu = NULL
};


static void register_block_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static void *block_handle_to_pointer(starpu_data_handle_t data_handle, uint32_t node);
static ssize_t allocate_block_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void free_block_buffer_on_node(void *data_interface, uint32_t node);
static size_t block_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_block_interface_crc32(starpu_data_handle_t handle);
static int block_compare(void *data_interface_a, void *data_interface_b);
static void display_block_interface(starpu_data_handle_t handle, FILE *f);
#ifdef STARPU_USE_GORDON
static int convert_block_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss);
#endif

static struct starpu_data_interface_ops interface_block_ops =
{
	.register_data_handle = register_block_handle,
	.allocate_data_on_node = allocate_block_buffer_on_node,
	.handle_to_pointer = block_handle_to_pointer,
	.free_data_on_node = free_block_buffer_on_node,
	.copy_methods = &block_copy_data_methods_s,
	.get_size = block_interface_get_size,
	.footprint = footprint_block_interface_crc32,
	.compare = block_compare,
#ifdef STARPU_USE_GORDON
	.convert_to_gordon = convert_block_to_gordon,
#endif
	.interfaceid = STARPU_BLOCK_INTERFACE_ID,
	.interface_size = sizeof(struct starpu_block_interface),
	.display = display_block_interface,
};

#ifdef STARPU_USE_GORDON
int convert_block_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss)
{
	/* TODO */
	STARPU_ABORT();

	return 0;
}
#endif

static void *block_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return (void*) block_interface->ptr;
}

static void register_block_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_block_interface *block_interface = (struct starpu_block_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_block_interface *local_interface = (struct starpu_block_interface *)
			starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->ptr = block_interface->ptr;
                        local_interface->dev_handle = block_interface->dev_handle;
                        local_interface->offset = block_interface->offset;
			local_interface->ldy  = block_interface->ldy;
			local_interface->ldz  = block_interface->ldz;
		}
		else
		{
			local_interface->ptr = 0;
                        local_interface->dev_handle = 0;
                        local_interface->offset = 0;
			local_interface->ldy  = 0;
			local_interface->ldz  = 0;
		}

		local_interface->nx = block_interface->nx;
		local_interface->ny = block_interface->ny;
		local_interface->nz = block_interface->nz;
		local_interface->elemsize = block_interface->elemsize;
	}
}

/* declare a new data with the BLAS interface */
void starpu_block_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
			uintptr_t ptr, uint32_t ldy, uint32_t ldz, uint32_t nx,
			uint32_t ny, uint32_t nz, size_t elemsize)
{
	struct starpu_block_interface block_interface =
	{
		.ptr = ptr,
                .dev_handle = ptr,
                .offset = 0,
		.ldy = ldy,
		.ldz = ldz,
		.nx = nx,
		.ny = ny,
		.nz = nz,
		.elemsize = elemsize
	};

	starpu_data_register(handleptr, home_node, &block_interface, &interface_block_ops);
}

static uint32_t footprint_block_interface_crc32(starpu_data_handle_t handle)
{
	uint32_t hash;

	hash = starpu_crc32_be(starpu_block_get_nx(handle), 0);
	hash = starpu_crc32_be(starpu_block_get_ny(handle), hash);
	hash = starpu_crc32_be(starpu_block_get_nz(handle), hash);

	return hash;
}

static int block_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_block_interface *block_a = (struct starpu_block_interface *) data_interface_a;
	struct starpu_block_interface *block_b = (struct starpu_block_interface *) data_interface_b;

	/* Two matricess are considered compatible if they have the same size */
	return ((block_a->nx == block_b->nx)
			&& (block_a->ny == block_b->ny)
			&& (block_a->nz == block_b->nz)
			&& (block_a->elemsize == block_b->elemsize));
}

static void display_block_interface(starpu_data_handle_t handle, FILE *f)
{
	struct starpu_block_interface *block_interface;

	block_interface = (struct starpu_block_interface *) starpu_data_get_interface_on_node(handle, 0);

	fprintf(f, "%u\t%u\t%u\t", block_interface->nx, block_interface->ny, block_interface->nz);
}

static size_t block_interface_get_size(starpu_data_handle_t handle)
{
	size_t size;
	struct starpu_block_interface *block_interface;

	block_interface = (struct starpu_block_interface *) starpu_data_get_interface_on_node(handle, 0);

	size = block_interface->nx*block_interface->ny*block_interface->nz*block_interface->elemsize;

	return size;
}

/* offer an access to the data parameters */
uint32_t starpu_block_get_nx(starpu_data_handle_t handle)
{
	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return block_interface->nx;
}

uint32_t starpu_block_get_ny(starpu_data_handle_t handle)
{
	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return block_interface->ny;
}

uint32_t starpu_block_get_nz(starpu_data_handle_t handle)
{
	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return block_interface->nz;
}

uint32_t starpu_block_get_local_ldy(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return block_interface->ldy;
}

uint32_t starpu_block_get_local_ldz(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return block_interface->ldz;
}

uintptr_t starpu_block_get_local_ptr(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return block_interface->ptr;
}

size_t starpu_block_get_elemsize(starpu_data_handle_t handle)
{
	struct starpu_block_interface *block_interface = (struct starpu_block_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return block_interface->elemsize;
}


/* memory allocation/deallocation primitives for the BLOCK interface */

/* returns the size of the allocated area */
static ssize_t allocate_block_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	uintptr_t addr = 0, handle = 0;
	unsigned fail = 0;
	ssize_t allocated_memory;

#ifdef STARPU_USE_CUDA
	cudaError_t status;
#endif
	struct starpu_block_interface *dst_block = (struct starpu_block_interface *) data_interface_;

	uint32_t nx = dst_block->nx;
	uint32_t ny = dst_block->ny;
	uint32_t nz = dst_block->nz;
	size_t elemsize = dst_block->elemsize;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);

	switch(kind)
	{
		case STARPU_CPU_RAM:
			handle = addr = (uintptr_t)malloc(nx*ny*nz*elemsize);
			if (!addr)
				fail = 1;

			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			status = cudaMalloc((void **)&addr, nx*ny*nz*elemsize);

			//_STARPU_DEBUG("cudaMalloc -> addr %p\n", addr);

			if (!addr || status != cudaSuccess)
			{
				if (STARPU_UNLIKELY(status != cudaErrorMemoryAllocation))
					STARPU_CUDA_REPORT_ERROR(status);

				fail = 1;
			}
			handle = addr;

			break;
#endif
#ifdef STARPU_USE_OPENCL
	        case STARPU_OPENCL_RAM:
			{
                                int ret;
				cl_mem mem;
                                ret = starpu_opencl_allocate_memory(&mem, nx*ny*nz*elemsize, CL_MEM_READ_WRITE);
				handle = (uintptr_t)mem;
				if (ret)
				{
					fail = 1;
				}
				break;
			}
#endif
		default:
			STARPU_ASSERT(0);
	}

	if (!fail)
	{
		/* allocation succeeded */
		allocated_memory = nx*ny*nz*elemsize;

		/* update the data properly in consequence */
		dst_block->ptr = addr;
		dst_block->dev_handle = handle;
                dst_block->offset = 0;
		dst_block->ldy = nx;
		dst_block->ldz = nx*ny;
	}
	else
	{
		/* allocation failed */
		allocated_memory = -ENOMEM;
	}

	return allocated_memory;
}

static void free_block_buffer_on_node(void *data_interface, uint32_t node)
{
	struct starpu_block_interface *block_interface = (struct starpu_block_interface *) data_interface;

#ifdef STARPU_USE_CUDA
	cudaError_t status;
#endif

	enum starpu_node_kind kind = starpu_node_get_kind(node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void*)block_interface->ptr);
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			status = cudaFree((void*)block_interface->ptr);
			if (STARPU_UNLIKELY(status))
				STARPU_CUDA_REPORT_ERROR(status);

			break;
#endif
#ifdef STARPU_USE_OPENCL
                case STARPU_OPENCL_RAM:
			clReleaseMemObject((void *)block_interface->dev_handle);
                        break;
#endif
		default:
			STARPU_ASSERT(0);
	}
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, enum cudaMemcpyKind kind)
{
	struct starpu_block_interface *src_block = src_interface;
	struct starpu_block_interface *dst_block = dst_interface;

	uint32_t nx = src_block->nx;
	uint32_t ny = src_block->ny;
	uint32_t nz = src_block->nz;
	size_t elemsize = src_block->elemsize;

	cudaError_t cures;

	if ((nx == src_block->ldy) && (src_block->ldy == dst_block->ldy))
	{
		/* Is that a single contiguous buffer ? */
		if (((nx*ny) == src_block->ldz) && (src_block->ldz == dst_block->ldz))
		{
                        cures = cudaMemcpy((char *)dst_block->ptr, (char *)src_block->ptr,
                                           nx*ny*nz*elemsize, kind);
                        if (STARPU_UNLIKELY(cures))
                                STARPU_CUDA_REPORT_ERROR(cures);
                }
		else
		{
			/* Are all plans contiguous */
                        cures = cudaMemcpy2D((char *)dst_block->ptr, dst_block->ldz*elemsize,
                                             (char *)src_block->ptr, src_block->ldz*elemsize,
                                             nx*ny*elemsize, nz, kind);
                        if (STARPU_UNLIKELY(cures))
                                STARPU_CUDA_REPORT_ERROR(cures);
                }
	}
	else
	{
		/* Default case: we transfer all lines one by one: ny*nz transfers */
		unsigned layer;
		for (layer = 0; layer < src_block->nz; layer++)
		{
			uint8_t *src_ptr = ((uint8_t *)src_block->ptr) + layer*src_block->ldz*src_block->elemsize;
			uint8_t *dst_ptr = ((uint8_t *)dst_block->ptr) + layer*dst_block->ldz*dst_block->elemsize;

			cures = cudaMemcpy2D((char *)dst_ptr, dst_block->ldy*elemsize,
                                             (char *)src_ptr, src_block->ldy*elemsize,
                                             nx*elemsize, ny, kind);

			if (STARPU_UNLIKELY(cures))
				STARPU_CUDA_REPORT_ERROR(cures);
		}
	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_block->nx*src_block->ny*src_block->elemsize*src_block->elemsize);

	return 0;
}

static int copy_cuda_async_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream, enum cudaMemcpyKind kind)
{
	struct starpu_block_interface *src_block = src_interface;
	struct starpu_block_interface *dst_block = dst_interface;

	uint32_t nx = src_block->nx;
	uint32_t ny = src_block->ny;
	uint32_t nz = src_block->nz;
	size_t elemsize = src_block->elemsize;

	cudaError_t cures;

	int ret;

	/* We may have a contiguous buffer for the entire block, or contiguous
	 * plans within the block, we can avoid many small transfers that way */
	if ((nx == src_block->ldy) && (src_block->ldy == dst_block->ldy))
	{
		/* Is that a single contiguous buffer ? */
		if (((nx*ny) == src_block->ldz) && (src_block->ldz == dst_block->ldz))
		{
			_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
			cures = cudaMemcpyAsync((char *)dst_block->ptr, (char *)src_block->ptr,
					nx*ny*nz*elemsize, kind, stream);
			_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
			if (STARPU_UNLIKELY(cures))
			{
				cures = cudaMemcpy((char *)dst_block->ptr, (char *)src_block->ptr,
					nx*ny*nz*elemsize, kind);
				if (STARPU_UNLIKELY(cures))
					STARPU_CUDA_REPORT_ERROR(cures);

				ret = 0;
			}
			else
			{
				ret = -EAGAIN;
			}

		}
		else
		{
			/* Are all plans contiguous */
			_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
			cures = cudaMemcpy2DAsync((char *)dst_block->ptr, dst_block->ldz*elemsize,
					(char *)src_block->ptr, src_block->ldz*elemsize,
					nx*ny*elemsize, nz, kind, stream);
			_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
			if (STARPU_UNLIKELY(cures))
			{
				cures = cudaMemcpy2D((char *)dst_block->ptr, dst_block->ldz*elemsize,
						(char *)src_block->ptr, src_block->ldz*elemsize,
						nx*ny*elemsize, nz, kind);
				if (STARPU_UNLIKELY(cures))
					STARPU_CUDA_REPORT_ERROR(cures);

				ret = 0;
			}
			else
			{
				ret = -EAGAIN;
			}
		}
	}
	else
	{
		/* Default case: we transfer all lines one by one: ny*nz transfers */
		unsigned layer;
		for (layer = 0; layer < src_block->nz; layer++)
		{
			uint8_t *src_ptr = ((uint8_t *)src_block->ptr) + layer*src_block->ldz*src_block->elemsize;
			uint8_t *dst_ptr = ((uint8_t *)dst_block->ptr) + layer*dst_block->ldz*dst_block->elemsize;

			_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
			cures = cudaMemcpy2DAsync((char *)dst_ptr, dst_block->ldy*elemsize,
                                                  (char *)src_ptr, src_block->ldy*elemsize,
                                                  nx*elemsize, ny, kind, stream);
			_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);

			if (STARPU_UNLIKELY(cures))
			{
				/* I don't know how to do that "better" */
				goto no_async_default;
			}

		}

		ret = -EAGAIN;

	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_block->nx*src_block->ny*src_block->nz*src_block->elemsize);

	return ret;

no_async_default:

	{
	unsigned layer;
	for (layer = 0; layer < src_block->nz; layer++)
	{
		uint8_t *src_ptr = ((uint8_t *)src_block->ptr) + layer*src_block->ldz*src_block->elemsize;
		uint8_t *dst_ptr = ((uint8_t *)dst_block->ptr) + layer*dst_block->ldz*dst_block->elemsize;

		cures = cudaMemcpy2D((char *)dst_ptr, dst_block->ldy*elemsize,
                                     (char *)src_ptr, src_block->ldy*elemsize,
                                     nx*elemsize, ny, kind);

		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);
	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_block->nx*src_block->ny*src_block->nz*src_block->elemsize);
	return 0;
	}
}

static int copy_cuda_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node)
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

static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	return copy_cuda_async_common(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyDeviceToHost);
}

static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	return copy_cuda_async_common(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyHostToDevice);
}
#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_block_interface *src_block = src_interface;
	struct starpu_block_interface *dst_block = dst_interface;
        int err, ret = 0;

	uint32_t nx = src_block->nx;
	uint32_t ny = src_block->ny;

	/* We may have a contiguous buffer for the entire block, or contiguous
	 * plans within the block, we can avoid many small transfers that way */
	if ((nx == src_block->ldy) && (src_block->ldy == dst_block->ldy))
	{
		/* Is that a single contiguous buffer ? */
		if (((nx*ny) == src_block->ldz) && (src_block->ldz == dst_block->ldz))
		{
                        err = starpu_opencl_copy_ram_to_opencl_async_sync((void*)src_block->ptr, src_node, (cl_mem)dst_block->dev_handle, dst_node,
                                                                           src_block->nx*src_block->ny*src_block->nz*src_block->elemsize,
                                                                           dst_block->offset, (cl_event*)_event, &ret);
                        if (STARPU_UNLIKELY(err))
                                STARPU_OPENCL_REPORT_ERROR(err);
                }
		else
		{
			/* Are all plans contiguous */
                        STARPU_ASSERT_MSG(0, "XXX non contiguous buffers are not properly supported in OpenCL yet. (TODO)");
                }
        }
	else
	{
		/* Default case: we transfer all lines one by one: ny*nz transfers */
		unsigned layer;
		for (layer = 0; layer < src_block->nz; layer++)
		{
                        unsigned j;
                        for(j=0 ; j<src_block->ny ; j++)
			{
                                void *ptr = (void*)src_block->ptr+(layer*src_block->ldz*src_block->elemsize)+(j*src_block->ldy*src_block->elemsize);
                                err = starpu_opencl_copy_ram_to_opencl(ptr, src_node, (cl_mem)dst_block->dev_handle, dst_node,
                                                                        src_block->nx*src_block->elemsize,
                                                                        layer*dst_block->ldz*dst_block->elemsize + j*dst_block->ldy*dst_block->elemsize
                                                                        + dst_block->offset, NULL);
                                if (STARPU_UNLIKELY(err))
                                        STARPU_OPENCL_REPORT_ERROR(err);
                        }

                        //                        int *foo = (int *)(src_block->ptr+(layer*src_block->ldz*src_block->elemsize));
                        //                        fprintf(stderr, "layer %d --> value %d\n", layer, foo[1]);
                        //                        const size_t buffer_origin[3] = {layer*src_block->ldz*src_block->elemsize, 0, 0};
                        //                        //const size_t buffer_origin[3] = {0, 0, 0};
                        //                        const size_t host_origin[3] = {layer*dst_block->ldz*dst_block->elemsize+dst_block->offset, 0, 0};
                        //                        size_t region[3] = {src_block->nx*src_block->elemsize,src_block->ny, 1};
                        //                        size_t buffer_row_pitch=region[0];
                        //                        size_t buffer_slice_pitch=region[1] * buffer_row_pitch;
                        //                        size_t host_row_pitch=region[0];
                        //                        size_t host_slice_pitch=region[1] * host_row_pitch;
                        //
                        //                        _starpu_opencl_copy_rect_ram_to_opencl((void *)src_block->ptr, src_node, (cl_mem)dst_block->dev_handle, dst_node,
                        //                                                               buffer_origin, host_origin, region,
                        //                                                               buffer_row_pitch, buffer_slice_pitch,
                        //                                                               host_row_pitch, host_slice_pitch, NULL);
                }
        }

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_block->nx*src_block->ny*src_block->nz*src_block->elemsize);

	return ret;
}

static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_block_interface *src_block = src_interface;
	struct starpu_block_interface *dst_block = dst_interface;
        int err, ret = 0;

	/* We may have a contiguous buffer for the entire block, or contiguous
	 * plans within the block, we can avoid many small transfers that way */
	if ((src_block->nx == src_block->ldy) && (src_block->ldy == dst_block->ldy))
	{
		/* Is that a single contiguous buffer ? */
		if (((src_block->nx*src_block->ny) == src_block->ldz) && (src_block->ldz == dst_block->ldz))
		{
                        err = starpu_opencl_copy_opencl_to_ram_async_sync((cl_mem)src_block->dev_handle, src_node, (void*)dst_block->ptr, dst_node,
                                                                           src_block->nx*src_block->ny*src_block->nz*src_block->elemsize,
                                                                           src_block->offset, (cl_event*)_event, &ret);
                        if (STARPU_UNLIKELY(err))
                                STARPU_OPENCL_REPORT_ERROR(err);
                }
                else
		{
			/* Are all plans contiguous */
                        STARPU_ASSERT_MSG(0, "XXX non contiguous buffers are not properly supported in OpenCL yet. (TODO)");
                }
        }
	else
	{
		/* Default case: we transfer all lines one by one: ny*nz transfers */
                /* XXX non contiguous buffers are not properly supported yet. (TODO) */
		unsigned layer;
		for (layer = 0; layer < src_block->nz; layer++)
		{
                        unsigned j;
                        for(j=0 ; j<src_block->ny ; j++)
			{
                                void *ptr = (void *)dst_block->ptr+(layer*dst_block->ldz*dst_block->elemsize)+(j*dst_block->ldy*dst_block->elemsize);
                                err = starpu_opencl_copy_opencl_to_ram((void*)src_block->dev_handle, src_node, ptr, dst_node,
                                                                        src_block->nx*src_block->elemsize,
                                                                        layer*src_block->ldz*src_block->elemsize+j*src_block->ldy*src_block->elemsize+
                                                                        src_block->offset, NULL);
				if (STARPU_UNLIKELY(err))
					STARPU_OPENCL_REPORT_ERROR(err);
                        }
                        //                        const size_t buffer_origin[3] = {src_block->offset, 0, 0};
                        //                        const size_t host_origin[3] = {layer*src_block->ldz*src_block->elemsize, 0, 0};
                        //                        size_t region[3] = {src_block->nx*src_block->elemsize,src_block->ny, 1};
                        //                        size_t buffer_row_pitch=region[0];
                        //                        size_t buffer_slice_pitch=region[1] * buffer_row_pitch;
                        //                        size_t host_row_pitch=region[0];
                        //                        size_t host_slice_pitch=region[1] * host_row_pitch;
                        //
                        //                        _starpu_opencl_copy_rect_opencl_to_ram((cl_mem)src_block->dev_handle, src_node, (void *)dst_block->ptr, dst_node,
                        //                                                               buffer_origin, host_origin, region,
                        //                                                               buffer_row_pitch, buffer_slice_pitch,
                        //                                                               host_row_pitch, host_slice_pitch, NULL);
                }
        }

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_block->nx*src_block->ny*src_block->nz*src_block->elemsize);

	return ret;
}

static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
        return copy_ram_to_opencl_async(src_interface, src_node, dst_interface, dst_node, NULL);
}

static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
        return copy_opencl_to_ram_async(src_interface, src_node, dst_interface, dst_node, NULL);
}

#endif

/* as not all platform easily have a BLAS lib installed ... */
static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_block_interface *src_block = (struct starpu_block_interface *) src_interface;
	struct starpu_block_interface *dst_block = (struct starpu_block_interface *) dst_interface;

	uint32_t nx = dst_block->nx;
	uint32_t ny = dst_block->ny;
	uint32_t nz = dst_block->nz;
	size_t elemsize = dst_block->elemsize;

	uint32_t ldy_src = src_block->ldy;
	uint32_t ldz_src = src_block->ldz;
	uint32_t ldy_dst = dst_block->ldy;
	uint32_t ldz_dst = dst_block->ldz;

	uintptr_t ptr_src = src_block->ptr;
	uintptr_t ptr_dst = dst_block->ptr;

	unsigned y, z;
	for (z = 0; z < nz; z++)
	{
		for (y = 0; y < ny; y++)
		{
			uint32_t src_offset = (y*ldy_src + z*ldz_src)*elemsize;
			uint32_t dst_offset = (y*ldy_dst + z*ldz_dst)*elemsize;

			memcpy((void *)(ptr_dst + dst_offset),
				(void *)(ptr_src + src_offset), nx*elemsize);
		}
	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nx*ny*nz*elemsize);

	return 0;
}
