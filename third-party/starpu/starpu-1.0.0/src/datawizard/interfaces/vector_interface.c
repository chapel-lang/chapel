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

static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node);
static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node, cudaStream_t stream);
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node, cudaStream_t stream);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node,					void *dst_interface, unsigned dst_node, cudaStream_t stream);
#endif
#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node);
static int copy_opencl_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node);
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node, void *_event);
static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node, void *_event);
#endif

static struct starpu_data_copy_methods vector_copy_data_methods_s =
{
	.ram_to_ram = copy_ram_to_ram,
	.ram_to_spu = NULL,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = copy_ram_to_cuda,
	.cuda_to_ram = copy_cuda_to_ram,
	.ram_to_cuda_async = copy_ram_to_cuda_async,
	.cuda_to_ram_async = copy_cuda_to_ram_async,
	.cuda_to_cuda = copy_cuda_to_cuda,
	.cuda_to_cuda_async = copy_cuda_to_cuda_async,
#endif
#ifdef STARPU_USE_OPENCL
	.ram_to_opencl = copy_ram_to_opencl,
	.opencl_to_ram = copy_opencl_to_ram,
	.opencl_to_opencl = copy_opencl_to_opencl,
        .ram_to_opencl_async = copy_ram_to_opencl_async,
	.opencl_to_ram_async = copy_opencl_to_ram_async,
#endif
	.cuda_to_spu = NULL,
	.spu_to_ram = NULL,
	.spu_to_cuda = NULL,
	.spu_to_spu = NULL
};

static void register_vector_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static ssize_t allocate_vector_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void *vector_handle_to_pointer(starpu_data_handle_t data_handle, uint32_t node);
static void free_vector_buffer_on_node(void *data_interface, uint32_t node);
static size_t vector_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_vector_interface_crc32(starpu_data_handle_t handle);
static int vector_compare(void *data_interface_a, void *data_interface_b);
static void display_vector_interface(starpu_data_handle_t handle, FILE *f);
#ifdef STARPU_USE_GORDON
static int convert_vector_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss);
#endif

static struct starpu_data_interface_ops interface_vector_ops =
{
	.register_data_handle = register_vector_handle,
	.allocate_data_on_node = allocate_vector_buffer_on_node,
	.handle_to_pointer = vector_handle_to_pointer,
	.free_data_on_node = free_vector_buffer_on_node,
	.copy_methods = &vector_copy_data_methods_s,
	.get_size = vector_interface_get_size,
	.footprint = footprint_vector_interface_crc32,
	.compare = vector_compare,
#ifdef STARPU_USE_GORDON
	.convert_to_gordon = convert_vector_to_gordon,
#endif
	.interfaceid = STARPU_VECTOR_INTERFACE_ID,
	.interface_size = sizeof(struct starpu_vector_interface),
	.display = display_vector_interface,
};

static void *vector_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return (void*) vector_interface->ptr;
}

static void register_vector_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_vector_interface *local_interface = (struct starpu_vector_interface *)
			starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->ptr = vector_interface->ptr;
                        local_interface->dev_handle = vector_interface->dev_handle;
                        local_interface->offset = vector_interface->offset;
		}
		else
		{
			local_interface->ptr = 0;
                        local_interface->dev_handle = 0;
                        local_interface->offset = 0;
		}

		local_interface->nx = vector_interface->nx;
		local_interface->elemsize = vector_interface->elemsize;
	}
}

#ifdef STARPU_USE_GORDON
int convert_vector_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss)
{
	struct starpu_vector_interface *vector_interface = interface;

	*ptr = vector_interface->ptr;
	(*ss).size = vector_interface->nx * vector_interface->elemsize;

	return 0;
}
#endif

/* declare a new data with the vector interface */
void starpu_vector_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
                        uintptr_t ptr, uint32_t nx, size_t elemsize)
{
	struct starpu_vector_interface vector =
	{
		.ptr = ptr,
		.nx = nx,
		.elemsize = elemsize,
                .dev_handle = ptr,
                .offset = 0
	};

	starpu_data_register(handleptr, home_node, &vector, &interface_vector_ops);
}


static uint32_t footprint_vector_interface_crc32(starpu_data_handle_t handle)
{
	return starpu_crc32_be(starpu_vector_get_nx(handle), 0);
}

static int vector_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_vector_interface *vector_a = (struct starpu_vector_interface *) data_interface_a;
	struct starpu_vector_interface *vector_b = (struct starpu_vector_interface *) data_interface_b;

	/* Two vectors are considered compatible if they have the same size */
	return ((vector_a->nx == vector_b->nx)
			&& (vector_a->elemsize == vector_b->elemsize));
}

static void display_vector_interface(starpu_data_handle_t handle, FILE *f)
{
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	fprintf(f, "%u\t", vector_interface->nx);
}

static size_t vector_interface_get_size(starpu_data_handle_t handle)
{
	size_t size;
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	size = vector_interface->nx*vector_interface->elemsize;

	return size;
}

/* offer an access to the data parameters */
uint32_t starpu_vector_get_nx(starpu_data_handle_t handle)
{
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return vector_interface->nx;
}

uintptr_t starpu_vector_get_local_ptr(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return vector_interface->ptr;
}

size_t starpu_vector_get_elemsize(starpu_data_handle_t handle)
{
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return vector_interface->elemsize;
}

/* memory allocation/deallocation primitives for the vector interface */

/* returns the size of the allocated area */
static ssize_t allocate_vector_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *) data_interface_;

	unsigned fail = 0;
	uintptr_t addr = 0, handle = 0;
	ssize_t allocated_memory;

	uint32_t nx = vector_interface->nx;
	size_t elemsize = vector_interface->elemsize;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);

#ifdef STARPU_USE_CUDA
	cudaError_t status;
#endif

	switch(kind)
	{
		case STARPU_CPU_RAM:
			addr = handle = (uintptr_t)malloc(nx*elemsize);
			if (!addr)
				fail = 1;
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			status = cudaMalloc((void **)&addr, nx*elemsize);
			if (!addr || (status != cudaSuccess))
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
                                ret = starpu_opencl_allocate_memory(&mem, nx*elemsize, CL_MEM_READ_WRITE);
				handle = (uintptr_t)mem;
				if (ret)
				{
					fail = 1;
				}
				break;
			}
#endif
		default:
			STARPU_ABORT();
	}

	if (fail)
		return -ENOMEM;

	/* allocation succeeded */
	allocated_memory = nx*elemsize;

	/* update the data properly in consequence */
	vector_interface->ptr = addr;
	vector_interface->dev_handle = handle;
        vector_interface->offset = 0;

	return allocated_memory;
}

static void free_vector_buffer_on_node(void *data_interface, uint32_t node)
{
	struct starpu_vector_interface *vector_interface = (struct starpu_vector_interface *) data_interface;

#ifdef STARPU_USE_CUDA
	cudaError_t cures;
#endif

	enum starpu_node_kind kind = starpu_node_get_kind(node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void*)vector_interface->ptr);
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			cures = cudaFree((void*)vector_interface->ptr);
			STARPU_ASSERT(cures == cudaSuccess);
			break;
#endif
#ifdef STARPU_USE_OPENCL
                case STARPU_OPENCL_RAM:
			clReleaseMemObject((cl_mem)vector_interface->dev_handle);
                        break;
#endif
		default:
			STARPU_ABORT();
	}
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, enum cudaMemcpyKind kind)
{
	struct starpu_vector_interface *src_vector = src_interface;
	struct starpu_vector_interface *dst_vector = dst_interface;

	cudaError_t cures;

	cures = cudaMemcpy((char *)dst_vector->ptr, (char *)src_vector->ptr, src_vector->nx*src_vector->elemsize, kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_vector->nx*src_vector->elemsize);

	return 0;
}

#ifdef HAVE_CUDA_MEMCPY_PEER
static int copy_cuda_peer_common(void *src_interface, unsigned src_node,
				void *dst_interface, unsigned dst_node,
				int is_async, cudaStream_t stream)
{
	cudaError_t cures;

	struct starpu_vector_interface *src_vector = src_interface;
	struct starpu_vector_interface *dst_vector = dst_interface;

	size_t length = src_vector->nx*src_vector->elemsize;

	int src_dev = _starpu_memory_node_to_devid(src_node);
	int dst_dev = _starpu_memory_node_to_devid(dst_node);

	if (is_async)
	{
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpyPeerAsync((char *)dst_vector->ptr, dst_dev,
						(char *)src_vector->ptr, src_dev,
						length, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		if (!cures)
			return -EAGAIN;
	}

	cures = cudaMemcpyPeer((char *)dst_vector->ptr, dst_dev,
				(char *)src_vector->ptr, src_dev, length);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, length);

	return 0;
}
#endif

static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost);
}

static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice);
}

static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	if (src_node == dst_node)
	{
		return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice);
	}
	else
	{
#ifdef HAVE_CUDA_MEMCPY_PEER
		return copy_cuda_peer_common(src_interface, src_node, dst_interface, dst_node, 0, 0);
#else
		/* This is illegal without cudaMemcpyPeer */
		STARPU_ABORT();
		return 0;
#endif
	}
}

static int copy_cuda_async_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
					void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED,
					cudaStream_t stream, enum cudaMemcpyKind kind)
{
	struct starpu_vector_interface *src_vector = src_interface;
	struct starpu_vector_interface *dst_vector = dst_interface;

	cudaError_t cures;

	_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
	cures = cudaMemcpyAsync((char *)dst_vector->ptr, (char *)src_vector->ptr, src_vector->nx*src_vector->elemsize, kind, stream);
	_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
	if (cures)
	{
		/* do it in a synchronous fashion */
		cures = cudaMemcpy((char *)dst_vector->ptr, (char *)src_vector->ptr, src_vector->nx*src_vector->elemsize, kind);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);

		return 0;
	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_vector->nx*src_vector->elemsize);

	return -EAGAIN;
}

static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	if (src_node == dst_node)
	{
		return copy_cuda_async_common(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyDeviceToDevice);
	}
	else
	{
#ifdef HAVE_CUDA_MEMCPY_PEER
		return copy_cuda_peer_common(src_interface, src_node, dst_interface, dst_node, 1, stream);
#else
		/* This is illegal without cudaMemcpyPeer */
		STARPU_ABORT();
		return 0;
#endif
	}
}

static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
					void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	return copy_cuda_async_common(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyDeviceToHost);
}

static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
					void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	return copy_cuda_async_common(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyHostToDevice);
}

#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
                                    void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_vector_interface *src_vector = src_interface;
	struct starpu_vector_interface *dst_vector = dst_interface;
        int err, ret;

	err = starpu_opencl_copy_ram_to_opencl((void*)src_vector->ptr, src_node, (cl_mem)dst_vector->dev_handle, dst_node,
					       src_vector->nx*src_vector->elemsize,
					       dst_vector->offset, (cl_event*)_event, &ret);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_vector->nx*src_vector->elemsize);

	return ret;
}

static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
                                    void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_vector_interface *src_vector = src_interface;
	struct starpu_vector_interface *dst_vector = dst_interface;
        int err, ret;

	err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_vector->dev_handle, src_node, (void*)dst_vector->ptr, dst_node, src_vector->nx*src_vector->elemsize,
					       src_vector->offset, (cl_event *)_event, &ret);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_vector->nx*src_vector->elemsize);

	return ret;
}

static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
                              void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
        return copy_ram_to_opencl_async(src_interface, src_node, dst_interface, dst_node, NULL);
}

static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
        return copy_opencl_to_ram_async(src_interface, src_node, dst_interface, dst_node, NULL);
}

static int copy_opencl_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
                              void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
        int err;

	struct starpu_vector_interface *src_vector = src_interface;
	struct starpu_vector_interface *dst_vector = dst_interface;

	cl_command_queue cq;
	starpu_opencl_get_current_queue(&cq);

	size_t size = src_vector->nx*src_vector->elemsize;
	cl_event event;

	err = clEnqueueCopyBuffer(cq, (cl_mem)src_vector->dev_handle, (cl_mem)dst_vector->dev_handle, src_vector->offset, dst_vector->offset, size, 0, NULL, &event);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	err = clWaitForEvents(1, &event);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	err = clReleaseEvent(event);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_vector->nx*src_vector->elemsize);

	return 0;
}


#endif // STARPU_USE_OPENCL

static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
					void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_vector_interface *src_vector = (struct starpu_vector_interface *) src_interface;
	struct starpu_vector_interface *dst_vector = (struct starpu_vector_interface *) dst_interface;

	uint32_t nx = dst_vector->nx;
	size_t elemsize = dst_vector->elemsize;

	uintptr_t ptr_src = src_vector->ptr;
	uintptr_t ptr_dst = dst_vector->ptr;

	memcpy((void *)ptr_dst, (void *)ptr_src, nx*elemsize);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, nx*elemsize);

	return 0;
}
