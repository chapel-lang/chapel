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

/* If you can promise that there is no stride in your matrices, you can define this */
// #define NO_STRIDE

static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
#ifdef NO_STRIDE
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream);
#endif
#endif
#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED);
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event);
static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event);
#endif

static struct starpu_data_copy_methods matrix_copy_data_methods_s =
{
	.ram_to_ram = copy_ram_to_ram,
	.ram_to_spu = NULL,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = copy_ram_to_cuda,
	.cuda_to_ram = copy_cuda_to_ram,
	.ram_to_cuda_async = copy_ram_to_cuda_async,
	.cuda_to_ram_async = copy_cuda_to_ram_async,
	.cuda_to_cuda = copy_cuda_to_cuda,
#ifdef NO_STRIDE
	.cuda_to_cuda_async = copy_cuda_to_cuda_async,
#endif
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

static void register_matrix_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static void *matrix_handle_to_pointer(starpu_data_handle_t data_handle, uint32_t node);
static ssize_t allocate_matrix_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void free_matrix_buffer_on_node(void *data_interface, uint32_t node);
static size_t matrix_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_matrix_interface_crc32(starpu_data_handle_t handle);
static int matrix_compare(void *data_interface_a, void *data_interface_b);
static void display_matrix_interface(starpu_data_handle_t handle, FILE *f);
#ifdef STARPU_USE_GORDON
static int convert_matrix_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss);
#endif

struct starpu_data_interface_ops _starpu_interface_matrix_ops =
{
	.register_data_handle = register_matrix_handle,
	.allocate_data_on_node = allocate_matrix_buffer_on_node,
	.handle_to_pointer = matrix_handle_to_pointer,
	.free_data_on_node = free_matrix_buffer_on_node,
	.copy_methods = &matrix_copy_data_methods_s,
	.get_size = matrix_interface_get_size,
	.footprint = footprint_matrix_interface_crc32,
	.compare = matrix_compare,
#ifdef STARPU_USE_GORDON
	.convert_to_gordon = convert_matrix_to_gordon,
#endif
	.interfaceid = STARPU_MATRIX_INTERFACE_ID,
	.interface_size = sizeof(struct starpu_matrix_interface),
	.display = display_matrix_interface,
};

#ifdef STARPU_USE_GORDON
static int convert_matrix_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss)
{
	size_t elemsize = GET_MATRIX_ELEMSIZE(interface);
	uint32_t nx = STARPU_MATRIX_GET_NX(interface);
	uint32_t ny = STARPU_MATRIX_GET_NY(interface);
	uint32_t ld = STARPU_MATRIX_GET_LD(interface);

	*ptr = STARPU_MATRIX_GET_PTR(interface);

	/* The gordon_stride_init function may use a contiguous buffer
 	 * in case nx = ld (in that case, (*ss).size = elemsize*nx*ny */
	*ss = gordon_stride_init(ny, nx*elemsize, ld*elemsize);

	return 0;
}
#endif

static void register_matrix_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_matrix_interface *local_interface = (struct starpu_matrix_interface *)
			starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->ptr = matrix_interface->ptr;
                        local_interface->dev_handle = matrix_interface->dev_handle;
                        local_interface->offset = matrix_interface->offset;
			local_interface->ld  = matrix_interface->ld;
		}
		else
		{
			local_interface->ptr = 0;
			local_interface->dev_handle = 0;
			local_interface->offset = 0;
			local_interface->ld  = 0;
		}

		local_interface->nx = matrix_interface->nx;
		local_interface->ny = matrix_interface->ny;
		local_interface->elemsize = matrix_interface->elemsize;
	}
}

static void *matrix_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return (void*) matrix_interface->ptr;
}


/* declare a new data with the matrix interface */
void starpu_matrix_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
			uintptr_t ptr, uint32_t ld, uint32_t nx,
			uint32_t ny, size_t elemsize)
{
	struct starpu_matrix_interface matrix_interface =
	{
		.ptr = ptr,
		.ld = ld,
		.nx = nx,
		.ny = ny,
		.elemsize = elemsize,
                .dev_handle = ptr,
                .offset = 0
	};

	starpu_data_register(handleptr, home_node, &matrix_interface, &_starpu_interface_matrix_ops);
}

static uint32_t footprint_matrix_interface_crc32(starpu_data_handle_t handle)
{
	return starpu_crc32_be(starpu_matrix_get_nx(handle), starpu_matrix_get_ny(handle));
}

static int matrix_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_matrix_interface *matrix_a = (struct starpu_matrix_interface *) data_interface_a;
	struct starpu_matrix_interface *matrix_b = (struct starpu_matrix_interface *) data_interface_b;

	/* Two matricess are considered compatible if they have the same size */
	return ((matrix_a->nx == matrix_b->nx)
			&& (matrix_a->ny == matrix_b->ny)
			&& (matrix_a->elemsize == matrix_b->elemsize));
}

static void display_matrix_interface(starpu_data_handle_t handle, FILE *f)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	fprintf(f, "%u\t%u\t", matrix_interface->nx, matrix_interface->ny);
}

static size_t matrix_interface_get_size(starpu_data_handle_t handle)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	size_t size;
	size = (size_t)matrix_interface->nx*matrix_interface->ny*matrix_interface->elemsize;

	return size;
}

/* offer an access to the data parameters */
uint32_t starpu_matrix_get_nx(starpu_data_handle_t handle)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return matrix_interface->nx;
}

uint32_t starpu_matrix_get_ny(starpu_data_handle_t handle)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return matrix_interface->ny;
}

uint32_t starpu_matrix_get_local_ld(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return matrix_interface->ld;
}

uintptr_t starpu_matrix_get_local_ptr(starpu_data_handle_t handle)
{
	unsigned node;
	node = _starpu_get_local_memory_node();

	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));

	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, node);

	return matrix_interface->ptr;
}

size_t starpu_matrix_get_elemsize(starpu_data_handle_t handle)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	return matrix_interface->elemsize;
}

/* memory allocation/deallocation primitives for the matrix interface */

/* returns the size of the allocated area */
static ssize_t allocate_matrix_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	uintptr_t addr = 0, handle = 0;
	unsigned fail = 0;
	ssize_t allocated_memory;

#ifdef STARPU_USE_CUDA
	cudaError_t status;
#endif

	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *) data_interface_;

	uint32_t nx = matrix_interface->nx;
	uint32_t ny = matrix_interface->ny;
	uint32_t ld = nx; // by default
	size_t elemsize = matrix_interface->elemsize;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);

	switch(kind)
	{
		case STARPU_CPU_RAM:
			handle = addr = (uintptr_t)malloc((size_t)nx*ny*elemsize);
			if (!addr)
				fail = 1;

			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			status = cudaMalloc((void **)&addr, (size_t)nx*ny*elemsize);
			if (!addr || status != cudaSuccess)
			{
				if (STARPU_UNLIKELY(status != cudaErrorMemoryAllocation))
					 STARPU_CUDA_REPORT_ERROR(status);

				fail = 1;
			}
			handle = addr;

			ld = nx;

			break;
#endif
#ifdef STARPU_USE_OPENCL
	        case STARPU_OPENCL_RAM:
			{
                                int ret;
				cl_mem mem;
                                ret = starpu_opencl_allocate_memory(&mem, nx*ny*elemsize, CL_MEM_READ_WRITE);
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

	if (!fail)
	{
		/* allocation succeeded */
		allocated_memory = (size_t)nx*ny*elemsize;

		/* update the data properly in consequence */
		matrix_interface->ptr = addr;
		matrix_interface->dev_handle = handle;
                matrix_interface->offset = 0;
		matrix_interface->ld = ld;
	}
	else
	{
		/* allocation failed */
		allocated_memory = -ENOMEM;
	}

	return allocated_memory;
}

static void free_matrix_buffer_on_node(void *data_interface, uint32_t node)
{
	struct starpu_matrix_interface *matrix_interface = (struct starpu_matrix_interface *) data_interface;

#ifdef STARPU_USE_CUDA
	cudaError_t status;
#endif

	enum starpu_node_kind kind = starpu_node_get_kind(node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			free((void*)matrix_interface->ptr);
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			status = cudaFree((void*)matrix_interface->ptr);
			if (STARPU_UNLIKELY(status))
				STARPU_CUDA_REPORT_ERROR(status);

			break;
#endif
#ifdef STARPU_USE_OPENCL
                case STARPU_OPENCL_RAM:
			clReleaseMemObject((void *)matrix_interface->dev_handle);
                        break;
#endif
		default:
			STARPU_ABORT();
	}
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, enum cudaMemcpyKind kind, int is_async, cudaStream_t stream)
{
	struct starpu_matrix_interface *src_matrix = src_interface;
	struct starpu_matrix_interface *dst_matrix = dst_interface;

	size_t elemsize = src_matrix->elemsize;
	cudaError_t cures;

#if 0
	struct cudaMemcpy3DParms p;
	memset(&p, 0, sizeof(p));

	p.srcPtr = make_cudaPitchedPtr((char *)src_matrix->ptr, src_matrix->ld * elemsize, src_matrix->ld * elemsize, src_matrix->ny);
	p.dstPtr = make_cudaPitchedPtr((char *)dst_matrix->ptr, dst_matrix->ld * elemsize, dst_matrix->ld * elemsize, dst_matrix->ny);
	p.extent = make_cudaExtent(src_matrix->nx * elemsize, src_matrix->ny, 1);
	p.kind = kind;

	if (is_async)
	{
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpy3DAsync(&p, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		if (!cures)
			return -EAGAIN;
	}

	cures = cudaMemcpy3D(&p);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);
#else

	if (is_async)
	{
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpy2DAsync((char *)dst_matrix->ptr, dst_matrix->ld*elemsize,
			(char *)src_matrix->ptr, src_matrix->ld*elemsize,
			src_matrix->nx*elemsize, src_matrix->ny, kind, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		if (!cures)
			return -EAGAIN;
	}

	cures = cudaMemcpy2D((char *)dst_matrix->ptr, dst_matrix->ld*elemsize,
		(char *)src_matrix->ptr, src_matrix->ld*elemsize,
		src_matrix->nx*elemsize, src_matrix->ny, kind);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);
#endif

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, (size_t)src_matrix->nx*src_matrix->ny*src_matrix->elemsize);

	return 0;
}

/* XXX this is broken : We need to properly call cudaDeviceEnablePeerAccess(), and avoid crossing NUMA nodes... */
#ifdef NO_STRIDE
static int copy_cuda_peer(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, int is_async, cudaStream_t stream)
{
	struct starpu_matrix_interface *src_matrix = src_interface;
	struct starpu_matrix_interface *dst_matrix = dst_interface;

	size_t elemsize = src_matrix->elemsize;
	cudaError_t cures;

	int src_dev = _starpu_memory_node_to_devid(src_node);
	int dst_dev = _starpu_memory_node_to_devid(dst_node);


#if 0
	/* That code is not even working!! */
	struct cudaExtent extent = make_cudaExtent(128, 128, 128);

	cures = cudaSetDevice(src_dev);
	STARPU_ASSERT(cures == cudaSuccess);

	struct cudaPitchedPtr mem_device1;
	cures = cudaMalloc3D(&mem_device1, extent);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	cures = cudaSetDevice(dst_dev);
	STARPU_ASSERT(cures == cudaSuccess);

	struct cudaPitchedPtr mem_device2;
	cures = cudaMalloc3D(&mem_device2, extent);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

	struct cudaMemcpy3DPeerParms p;
	memset(&p, 0, sizeof(p));
	p.srcDevice = src_dev;
	p.dstDevice = dst_dev;
	p.srcPtr = mem_device1;
	p.dstPtr = mem_device2;
	p.extent = extent;

	fprintf(stderr,"%u %u\n", p.srcDevice, p.dstDevice);
	fprintf(stderr,"%p %p\n", p.srcArray, p.dstArray);
	fprintf(stderr,"%p %lu %lu %lu\n", p.srcPtr.ptr, p.srcPtr.pitch, p.srcPtr.xsize, p.srcPtr.ysize);
	fprintf(stderr,"%p %lu %lu %lu\n", p.dstPtr.ptr, p.dstPtr.pitch, p.dstPtr.xsize, p.dstPtr.ysize);
	fprintf(stderr,"%lu %lu %lu\n", p.srcPos.x, p.srcPos.y, p.srcPos.z);
	fprintf(stderr,"%lu %lu %lu\n", p.dstPos.x, p.dstPos.y, p.dstPos.z);
	fprintf(stderr,"%lu %lu %lu\n", p.extent.width, p.extent.height, p.extent.depth);
	cures = cudaMemcpy3DPeer(&p);
	if (STARPU_UNLIKELY(cures))
	        STARPU_CUDA_REPORT_ERROR(cures);
#endif

#if 0
	struct cudaMemcpy3DPeerParms p;
	memset(&p, 0, sizeof(p));

	p.srcDevice = src_dev;
	p.dstDevice = dst_dev;
	p.srcPtr = make_cudaPitchedPtr((char *)src_matrix->ptr, src_matrix->ld * elemsize, src_matrix->nx * elemsize, src_matrix->ny);
	p.dstPtr = make_cudaPitchedPtr((char *)dst_matrix->ptr, dst_matrix->ld * elemsize, dst_matrix->nx * elemsize, dst_matrix->ny);
	p.extent = make_cudaExtent(src_matrix->nx * elemsize, src_matrix->ny, 1);

#if 1
	fprintf(stderr,"%u %u\n", p.srcDevice, p.dstDevice);
	fprintf(stderr,"%p %p\n", p.srcArray, p.dstArray);
	fprintf(stderr,"%p %lu %lu %lu\n", p.srcPtr.ptr, p.srcPtr.pitch, p.srcPtr.xsize, p.srcPtr.ysize);
	fprintf(stderr,"%p %lu %lu %lu\n", p.dstPtr.ptr, p.dstPtr.pitch, p.dstPtr.xsize, p.dstPtr.ysize);
	fprintf(stderr,"%lu %lu %lu\n", p.srcPos.x, p.srcPos.y, p.srcPos.z);
	fprintf(stderr,"%lu %lu %lu\n", p.dstPos.x, p.dstPos.y, p.dstPos.z);
	fprintf(stderr,"%lu %lu %lu\n", p.extent.width, p.extent.height, p.extent.depth);
#endif

	cures = cudaMemcpy3DPeerAsync(&p, stream);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);
	cudaThreadSynchronize();

	if (is_async)
	{
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpy3DPeerAsync(&p, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		if (!cures)
			return -EAGAIN;
	}

	cures = cudaMemcpy3DPeer(&p);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);

#else
	/* XXX FIXME !!*/
	STARPU_ASSERT(src_matrix->nx == src_matrix->ld);
	STARPU_ASSERT(dst_matrix->nx == dst_matrix->ld);

	if (is_async)
	{
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		cures = cudaMemcpyPeerAsync((char *)dst_matrix->ptr, dst_dev, (char *)src_matrix->ptr, src_dev, dst_matrix->nx*dst_matrix->ny*elemsize, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		if (!cures)
			return -EAGAIN;
	}

	cures = cudaMemcpyPeer((char *)dst_matrix->ptr, dst_dev, (char *)src_matrix->ptr, src_dev, dst_matrix->nx*dst_matrix->ny*elemsize);
	if (STARPU_UNLIKELY(cures))
		STARPU_CUDA_REPORT_ERROR(cures);
#endif

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, (size_t)src_matrix->nx*src_matrix->ny*src_matrix->elemsize);

	return 0;
}
#endif

static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost, 0, 0);
}

static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice, 0, 0);
}

static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	if (src_node == dst_node)
		return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice, 0, 0);
	else
	{
		/* XXX not implemented */
		STARPU_ABORT();
		return 0;
	}
}

static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost, 1, stream);
}

static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice, 1, stream);
}

#ifdef NO_STRIDE
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, cudaStream_t stream)
{
	if (src_node == dst_node)
		return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice, 1, stream);
	else
		return copy_cuda_peer(src_interface, src_node, dst_interface, dst_node, 1, stream);
}
#endif
#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_matrix_interface *src_matrix = src_interface;
	struct starpu_matrix_interface *dst_matrix = dst_interface;
        int err,ret;

	STARPU_ASSERT_MSG((src_matrix->ld == src_matrix->nx) && (dst_matrix->ld == dst_matrix->nx), "XXX non contiguous buffers are not properly supported in OpenCL yet. (TODO)");

	err = starpu_opencl_copy_ram_to_opencl_async_sync((void*)src_matrix->ptr, src_node, (cl_mem)dst_matrix->dev_handle, dst_node,
                                                           src_matrix->nx*src_matrix->ny*src_matrix->elemsize,
                                                           dst_matrix->offset, (cl_event*)_event, &ret);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_matrix->nx*src_matrix->ny*src_matrix->elemsize);

	return ret;
}

static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, void *_event)
{
	struct starpu_matrix_interface *src_matrix = src_interface;
	struct starpu_matrix_interface *dst_matrix = dst_interface;
        int err, ret;

	STARPU_ASSERT_MSG((src_matrix->ld == src_matrix->nx) && (dst_matrix->ld == dst_matrix->nx), "XXX non contiguous buffers are not properly supported in OpenCL yet. (TODO)");

        err = starpu_opencl_copy_opencl_to_ram_async_sync((cl_mem)src_matrix->dev_handle, src_node, (void*)dst_matrix->ptr, dst_node,
                                                           src_matrix->nx*src_matrix->ny*src_matrix->elemsize,
                                                           src_matrix->offset, (cl_event*)_event, &ret);

        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, src_matrix->nx*src_matrix->ny*src_matrix->elemsize);

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

/* as not all platform easily have a  lib installed ... */
static int copy_ram_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	struct starpu_matrix_interface *src_matrix = (struct starpu_matrix_interface *) src_interface;
	struct starpu_matrix_interface *dst_matrix = (struct starpu_matrix_interface *) dst_interface;

	unsigned y;
	uint32_t nx = dst_matrix->nx;
	uint32_t ny = dst_matrix->ny;
	size_t elemsize = dst_matrix->elemsize;

	uint32_t ld_src = src_matrix->ld;
	uint32_t ld_dst = dst_matrix->ld;

	uintptr_t ptr_src = src_matrix->ptr;
	uintptr_t ptr_dst = dst_matrix->ptr;


	for (y = 0; y < ny; y++)
	{
		uint32_t src_offset = y*ld_src*elemsize;
		uint32_t dst_offset = y*ld_dst*elemsize;

		memcpy((void *)(ptr_dst + dst_offset),
			(void *)(ptr_src + src_offset), nx*elemsize);
	}

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, (size_t)nx*ny*elemsize);

	return 0;
}
