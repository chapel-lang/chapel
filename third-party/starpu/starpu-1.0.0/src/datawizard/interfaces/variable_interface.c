/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

static int copy_ram_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#endif
#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_opencl_to_opencl(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event);
static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event);
#endif

static struct starpu_data_copy_methods variable_copy_data_methods_s =
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
	.opencl_to_opencl = copy_opencl_to_opencl,
        .ram_to_opencl_async = copy_ram_to_opencl_async,
	.opencl_to_ram_async = copy_opencl_to_ram_async,
#endif
	.cuda_to_spu = NULL,
	.spu_to_ram = NULL,
	.spu_to_cuda = NULL,
	.spu_to_spu = NULL
};

static void register_variable_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static ssize_t allocate_variable_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void *variable_handle_to_pointer(starpu_data_handle_t data_handle, uint32_t node);
static void free_variable_buffer_on_node(void *data_interface, uint32_t node);
static size_t variable_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_variable_interface_crc32(starpu_data_handle_t handle);
static int variable_compare(void *data_interface_a, void *data_interface_b);
static void display_variable_interface(starpu_data_handle_t handle, FILE *f);
#ifdef STARPU_USE_GORDON
static int convert_variable_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss);
#endif

static struct starpu_data_interface_ops interface_variable_ops =
{
	.register_data_handle = register_variable_handle,
	.allocate_data_on_node = allocate_variable_buffer_on_node,
	.handle_to_pointer = variable_handle_to_pointer,
	.free_data_on_node = free_variable_buffer_on_node,
	.copy_methods = &variable_copy_data_methods_s,
	.get_size = variable_interface_get_size,
	.footprint = footprint_variable_interface_crc32,
	.compare = variable_compare,
#ifdef STARPU_USE_GORDON
	.convert_to_gordon = convert_variable_to_gordon,
#endif
	.interfaceid = STARPU_VARIABLE_INTERFACE_ID,
	.interface_size = sizeof(struct starpu_variable_interface),
	.display = display_variable_interface,
};

static void *variable_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	return (void*) STARPU_VARIABLE_GET_PTR(starpu_data_get_interface_on_node(handle, node));
}

static void register_variable_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_variable_interface *local_interface = (struct starpu_variable_interface *)
			starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->ptr = STARPU_VARIABLE_GET_PTR(data_interface);
		}
		else
		{
			local_interface->ptr = 0;
		}

		local_interface->elemsize = STARPU_VARIABLE_GET_ELEMSIZE(data_interface);
	}
}

#ifdef STARPU_USE_GORDON
int convert_variable_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss)
{
	*ptr = STARPU_VARIABLE_GET_PTR(interface);
	(*ss).size = STARPU_VARIABLE_GET_ELEMSIZE(interface);

	return 0;
}
#endif

/* declare a new data with the variable interface */
void starpu_variable_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
                        uintptr_t ptr, size_t elemsize)
{
	struct starpu_variable_interface variable =
	{
		.ptr = ptr,
		.elemsize = elemsize
	};

	starpu_data_register(handleptr, home_node, &variable, &interface_variable_ops);
}


static uint32_t footprint_variable_interface_crc32(starpu_data_handle_t handle)
{
	return starpu_crc32_be(starpu_variable_get_elemsize(handle), 0);
}

static int variable_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_variable_interface *variable_a = (struct starpu_variable_interface *) data_interface_a;
	struct starpu_variable_interface *variable_b = (struct starpu_variable_interface *) data_interface_b;

	/* Two variables are considered compatible if they have the same size */
	return (variable_a->elemsize == variable_b->elemsize);
}

static void display_variable_interface(starpu_data_handle_t handle, FILE *f)
{
	struct starpu_variable_interface *variable_interface = (struct starpu_variable_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	fprintf(f, "%ld\t", (long)variable_interface->elemsize);
}

static size_t variable_interface_get_size(starpu_data_handle_t handle)
{
	struct starpu_variable_interface *variable_interface = (struct starpu_variable_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return variable_interface->elemsize;
}

uintptr_t starpu_variable_get_local_ptr(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	return STARPU_VARIABLE_GET_PTR(starpu_data_get_interface_on_node(handle, node));
}

size_t starpu_variable_get_elemsize(starpu_data_handle_t handle)
{
	return STARPU_VARIABLE_GET_ELEMSIZE(starpu_data_get_interface_on_node(handle, 0));
}

/* memory allocation/deallocation primitives for the variable interface */

/* returns the size of the allocated area */
static ssize_t allocate_variable_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	struct starpu_variable_interface *variable_interface = (struct starpu_variable_interface *) data_interface_;

	unsigned fail = 0;
	uintptr_t addr = 0;
	ssize_t allocated_memory;

	size_t elemsize = variable_interface->elemsize;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);

#ifdef STARPU_USE_CUDA
	cudaError_t status;
#endif

	switch(kind)
	{
		case STARPU_CPU_RAM:
			addr = (uintptr_t)malloc(elemsize);
			if (!addr)
				fail = 1;
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			status = cudaMalloc((void **)&addr, elemsize);
			if (!addr || (status != cudaSuccess))
			{
				if (STARPU_UNLIKELY(status != cudaErrorMemoryAllocation))
					STARPU_CUDA_REPORT_ERROR(status);

				fail = 1;
			}
			break;
#endif
#ifdef STARPU_USE_OPENCL
	        case STARPU_OPENCL_RAM:
			{
                                int ret;
				cl_mem ptr;
                                ret = starpu_opencl_allocate_memory(&ptr, elemsize, CL_MEM_READ_WRITE);
                                addr = (uintptr_t)ptr;
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
	allocated_memory = elemsize;

	/* update the data properly in consequence */
	variable_interface->ptr = addr;

	return allocated_memory;
}

static void free_variable_buffer_on_node(void *data_interface, uint32_t node)
{
	enum starpu_node_kind kind = starpu_node_get_kind(node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void*)STARPU_VARIABLE_GET_PTR(data_interface));
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			cudaFree((void*)STARPU_VARIABLE_GET_PTR(data_interface));
			break;
#endif
#ifdef STARPU_USE_OPENCL
                case STARPU_OPENCL_RAM:
                        clReleaseMemObject((void*)STARPU_VARIABLE_GET_PTR(data_interface));
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
	struct starpu_variable_interface *src_variable = src_interface;
	struct starpu_variable_interface *dst_variable = dst_interface;

	cudaError_t cures;
	cures = cudaMemcpy((char *)dst_variable->ptr, (char *)src_variable->ptr, src_variable->elemsize, kind);

	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_variable->elemsize);

	return 0;
}


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
		int src_dev = _starpu_memory_node_to_devid(src_node);
		int dst_dev = _starpu_memory_node_to_devid(dst_node);

		struct starpu_variable_interface *src_variable = src_interface;
		struct starpu_variable_interface *dst_variable = dst_interface;

		cudaError_t cures;
		cures = cudaMemcpyPeer((char *)dst_variable->ptr, dst_dev, (char *)src_variable->ptr, src_dev, src_variable->elemsize);
		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);

		_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_variable->elemsize);

#else
		/* This is illegal without support for cudaMemcpyPeer */
		STARPU_ABORT();
#endif
		return 0;
	}
}

static int copy_cuda_async_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED,
					void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED,
					cudaStream_t stream, enum cudaMemcpyKind kind)
{
	struct starpu_variable_interface *src_variable = src_interface;
	struct starpu_variable_interface *dst_variable = dst_interface;

	cudaError_t cures;
	_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
	cures = cudaMemcpyAsync((char *)dst_variable->ptr, (char *)src_variable->ptr, src_variable->elemsize, kind, stream);
	_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
	if (cures)
	{
		/* do it in a synchronous fashion */
		cures = cudaMemcpy((char *)dst_variable->ptr, (char *)src_variable->ptr, src_variable->elemsize, kind);

		if (STARPU_UNLIKELY(cures))
			STARPU_CUDA_REPORT_ERROR(cures);

		return 0;
	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_variable->elemsize);

	return -EAGAIN;
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

static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node,					void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	if (src_node == dst_node)
	{
		return copy_cuda_async_common(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyDeviceToDevice);
	}
	else
	{
#ifdef HAVE_CUDA_MEMCPY_PEER
		int src_dev = _starpu_memory_node_to_devid(src_node);
		int dst_dev = _starpu_memory_node_to_devid(dst_node);

		struct starpu_variable_interface *src_variable = src_interface;
		struct starpu_variable_interface *dst_variable = dst_interface;

		size_t length = src_variable->elemsize;

		cudaError_t cures;
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpyPeerAsync((char *)dst_variable->ptr, dst_dev, (char *)src_variable->ptr, src_dev, length, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		if (cures)
		{
			/* sychronous fallback */
			cures = cudaMemcpyPeer((char *)dst_variable->ptr, dst_dev, (char *)src_variable->ptr, src_dev, length);
			if (STARPU_UNLIKELY(cures))
				STARPU_CUDA_REPORT_ERROR(cures);

			return 0;
		}

		_STARPU_TRACE_DATA_COPY(src_node, dst_node, length);

		return -EAGAIN;
#else
		/* This is illegal without cudaMemcpyPeer */
		STARPU_ABORT();
		return 0;
#endif
	}
}


#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface,
                                    unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_variable_interface *src_variable = src_interface;
	struct starpu_variable_interface *dst_variable = dst_interface;
        int err,ret;

        err = starpu_opencl_copy_ram_to_opencl((void*)src_variable->ptr, src_node, (cl_mem)dst_variable->ptr, dst_node, src_variable->elemsize,
					       0, (cl_event*)_event, &ret);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_variable->elemsize);

	return ret;
}

static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_variable_interface *src_variable = src_interface;
	struct starpu_variable_interface *dst_variable = dst_interface;
        int err, ret;

	err = starpu_opencl_copy_opencl_to_ram((cl_mem)src_variable->ptr, src_node, (void*)dst_variable->ptr, dst_node, src_variable->elemsize,
					       0, (cl_event*)_event, &ret);

        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_variable->elemsize);

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

static int copy_opencl_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	cl_int err;

	struct starpu_variable_interface *src_variable = src_interface;
	struct starpu_variable_interface *dst_variable = dst_interface;

	cl_mem src_ptr = (cl_mem)src_variable->ptr;
	cl_mem dst_ptr = (cl_mem)dst_variable->ptr;

	cl_command_queue cq;
	starpu_opencl_get_current_queue(&cq);
	cl_event event;

	STARPU_ASSERT(src_variable->elemsize == dst_variable->elemsize);
	err= clEnqueueCopyBuffer(cq, src_ptr, dst_ptr, 0, 0, src_variable->elemsize, 0, NULL, &event);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	err = clWaitForEvents(1, &event);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	err = clReleaseEvent(event);
	if (STARPU_UNLIKELY(err))
		STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_variable->elemsize);

	return 0;
}

#endif

static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_variable_interface *src_variable = (struct starpu_variable_interface *) src_interface;
	struct starpu_variable_interface *dst_variable = (struct starpu_variable_interface *) dst_interface;

	size_t elemsize = dst_variable->elemsize;

	uintptr_t ptr_src = src_variable->ptr;
	uintptr_t ptr_dst = dst_variable->ptr;

	memcpy((void *)ptr_dst, (void *)ptr_src, elemsize);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, elemsize);

	return 0;
}
