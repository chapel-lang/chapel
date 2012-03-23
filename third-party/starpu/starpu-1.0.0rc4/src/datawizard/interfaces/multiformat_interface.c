/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011-2012  Institut National de Recherche en Informatique et Automatique
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
#include <core/task.h>

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

static struct starpu_data_copy_methods multiformat_copy_data_methods_s =
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

static void register_multiformat_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static ssize_t allocate_multiformat_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void *multiformat_handle_to_pointer(starpu_data_handle_t data_handle, uint32_t node);
static void free_multiformat_buffer_on_node(void *data_interface, uint32_t node);
static size_t multiformat_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_multiformat_interface_crc32(starpu_data_handle_t handle);
static int multiformat_compare(void *data_interface_a, void *data_interface_b);
static void display_multiformat_interface(starpu_data_handle_t handle, FILE *f);
static uint32_t starpu_multiformat_get_nx(starpu_data_handle_t handle);
#ifdef STARPU_USE_GORDON
static int convert_multiformat_to_gordon(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss);
#endif

static struct starpu_multiformat_data_interface_ops*
get_mf_ops(void *data_interface)
{
	struct starpu_multiformat_interface *mf;
	mf = (struct starpu_multiformat_interface *) data_interface;

	return mf->ops;
}

static struct starpu_data_interface_ops interface_multiformat_ops =
{
	.register_data_handle  = register_multiformat_handle,
	.allocate_data_on_node = allocate_multiformat_buffer_on_node,
	.handle_to_pointer     = multiformat_handle_to_pointer,
	.free_data_on_node     = free_multiformat_buffer_on_node,
	.copy_methods          = &multiformat_copy_data_methods_s,
	.get_size              = multiformat_interface_get_size,
	.footprint             = footprint_multiformat_interface_crc32,
	.compare               = multiformat_compare,
#ifdef STARPU_USE_GORDON
	.convert_to_gordon     = NULL,
#endif
	.interfaceid           = STARPU_MULTIFORMAT_INTERFACE_ID,
	.interface_size        = sizeof(struct starpu_multiformat_interface),
	.display               = display_multiformat_interface,
	.is_multiformat        = 1,
	.get_mf_ops            = get_mf_ops
};

static void *multiformat_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	STARPU_ASSERT(starpu_data_test_if_allocated_on_node(handle, node));
	struct starpu_multiformat_interface *multiformat_interface =
		(struct starpu_multiformat_interface *) starpu_data_get_interface_on_node(handle, node);

	switch(starpu_node_get_kind(node))
	{
		case STARPU_CPU_RAM:
			return multiformat_interface->cpu_ptr;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			return multiformat_interface->cuda_ptr;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			return multiformat_interface->opencl_ptr;
#endif
		default:
			STARPU_ASSERT(0);
	}
	return NULL;
}

static void register_multiformat_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct starpu_multiformat_interface *multiformat_interface;
	multiformat_interface = (struct starpu_multiformat_interface *) data_interface;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct starpu_multiformat_interface *local_interface =
			(struct starpu_multiformat_interface *) starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->cpu_ptr    = multiformat_interface->cpu_ptr;
#ifdef STARPU_USE_CUDA
			local_interface->cuda_ptr   = multiformat_interface->cuda_ptr;
#endif
#ifdef STARPU_USE_OPENCL
			local_interface->opencl_ptr = multiformat_interface->opencl_ptr;
#endif
		}
		else
		{
			local_interface->cpu_ptr    = NULL;
#ifdef STARPU_USE_CUDA
			local_interface->cuda_ptr   = NULL;
#endif
#ifdef STARPU_USE_OPENCL
			local_interface->opencl_ptr = NULL;
#endif
		}
		local_interface->nx = multiformat_interface->nx;
		local_interface->ops = multiformat_interface->ops;
	}
}

void starpu_multiformat_data_register(starpu_data_handle_t *handleptr,
				      uint32_t home_node,
				      void *ptr,
				      uint32_t nobjects,
				      struct starpu_multiformat_data_interface_ops *format_ops)
{
	_starpu_codelet_check_deprecated_fields(format_ops->cpu_to_opencl_cl);
	_starpu_codelet_check_deprecated_fields(format_ops->opencl_to_cpu_cl);
	_starpu_codelet_check_deprecated_fields(format_ops->cpu_to_cuda_cl);
	_starpu_codelet_check_deprecated_fields(format_ops->cuda_to_cpu_cl);

	struct starpu_multiformat_interface multiformat =
	{
		.cpu_ptr    = ptr,
		.cuda_ptr   = NULL,
		.opencl_ptr = NULL,
		.nx         = nobjects,
		.ops        = format_ops
	};

	starpu_data_register(handleptr, home_node, &multiformat, &interface_multiformat_ops);
}

static uint32_t footprint_multiformat_interface_crc32(starpu_data_handle_t handle)
{
	return starpu_crc32_be(starpu_multiformat_get_nx(handle), 0);
}

static int multiformat_compare(void *data_interface_a, void *data_interface_b)
{
	struct starpu_multiformat_interface *multiformat_a = (struct starpu_multiformat_interface *) data_interface_a;
	struct starpu_multiformat_interface *multiformat_b = (struct starpu_multiformat_interface *) data_interface_b;

	return ((multiformat_a->nx == multiformat_b->nx)
			&& (multiformat_a->ops->cpu_elemsize == multiformat_b->ops->cpu_elemsize)
#ifdef STARPU_USE_CUDA
			&& (multiformat_a->ops->cuda_elemsize == multiformat_b->ops->cuda_elemsize)
#endif
#ifdef STARPU_USE_OPENCL
			&& (multiformat_a->ops->opencl_elemsize == multiformat_b->ops->opencl_elemsize)
#endif
		);
}

static void display_multiformat_interface(starpu_data_handle_t handle, FILE *f)
{
	/* TODO */
	struct starpu_multiformat_interface *multiformat_interface;
	multiformat_interface = (struct starpu_multiformat_interface *)
		starpu_data_get_interface_on_node(handle, 0);

	fprintf(f, "%u\t", multiformat_interface->nx);
}

/* XXX : returns CPU size */
static size_t multiformat_interface_get_size(starpu_data_handle_t handle)
{
	size_t size;
	struct starpu_multiformat_interface *multiformat_interface;
	multiformat_interface = (struct starpu_multiformat_interface *) starpu_data_get_interface_on_node(handle, 0);
	size = multiformat_interface->nx * multiformat_interface->ops->cpu_elemsize;
	return size;
}

uint32_t starpu_multiformat_get_nx(starpu_data_handle_t handle)
{
	struct starpu_multiformat_interface *multiformat_interface;
	multiformat_interface = (struct starpu_multiformat_interface *) starpu_data_get_interface_on_node(handle, 0);
	return multiformat_interface->nx;
}

static void free_multiformat_buffer_on_node(void *data_interface, uint32_t node)
{
	struct starpu_multiformat_interface *multiformat_interface;
	multiformat_interface = (struct starpu_multiformat_interface *) data_interface;
	enum starpu_node_kind kind = starpu_node_get_kind(node);

	switch(kind)
	{
		case STARPU_CPU_RAM:
			free(multiformat_interface->cpu_ptr);
			multiformat_interface->cpu_ptr = NULL;
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			if (multiformat_interface->cpu_ptr)
			{
				cudaFree(multiformat_interface->cpu_ptr);
				multiformat_interface->cpu_ptr = NULL;
			}
			if (multiformat_interface->cuda_ptr)
			{
				cudaFree(multiformat_interface->cuda_ptr);
				multiformat_interface->cuda_ptr = NULL;
			}
			break;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			STARPU_ASSERT_MSG(0, "XXX multiformat not supported on OpenCL yet (TODO)");
			break;
#endif
		default:
			STARPU_ABORT();
	}
}

static ssize_t allocate_multiformat_buffer_on_node(void *data_interface_, uint32_t dst_node)
{
	struct starpu_multiformat_interface *multiformat_interface;
	multiformat_interface = (struct starpu_multiformat_interface *) data_interface_;
	unsigned fail = 0;
	uintptr_t addr = 0;
	ssize_t allocated_memory = 0;

	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);
	switch(kind)
	{
		case STARPU_CPU_RAM:
			allocated_memory = multiformat_interface->nx * multiformat_interface->ops->cpu_elemsize;
			addr = (uintptr_t)malloc(allocated_memory);
			if (!addr)
			{
				fail = 1;
			}
			else
			{
				multiformat_interface->cpu_ptr = (void *) addr;
			}

#ifdef STARPU_USE_CUDA
			multiformat_interface->cuda_ptr = malloc(multiformat_interface->nx * multiformat_interface->ops->cuda_elemsize);
			STARPU_ASSERT(multiformat_interface->cuda_ptr != NULL);
#endif
#ifdef STARPU_USE_OPENCL
			multiformat_interface->opencl_ptr = malloc(multiformat_interface->nx * multiformat_interface->ops->opencl_elemsize);
			STARPU_ASSERT(multiformat_interface->opencl_ptr != NULL);
#endif
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			{
				allocated_memory = multiformat_interface->nx * multiformat_interface->ops->cuda_elemsize;
				cudaError_t status = cudaMalloc((void **)&addr, allocated_memory);
				if (STARPU_UNLIKELY(status))
				{
					STARPU_CUDA_REPORT_ERROR(status);
				}
				else
				{
					multiformat_interface->cuda_ptr = (void *)addr;
				}

				allocated_memory = multiformat_interface->nx * multiformat_interface->ops->cpu_elemsize;
				status = cudaMalloc((void **)&multiformat_interface->cpu_ptr, allocated_memory);
				if (STARPU_UNLIKELY(status != cudaSuccess))
					STARPU_CUDA_REPORT_ERROR(status);
				break;
			}
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			{
                                int ret;
				cl_mem ptr;
				allocated_memory = multiformat_interface->nx * multiformat_interface->ops->opencl_elemsize;
                                ret = starpu_opencl_allocate_memory(&ptr, allocated_memory, CL_MEM_READ_WRITE);
                                addr = (uintptr_t)ptr;
				if (ret)
				{
					fail = 1;
				}
				else
				{
					multiformat_interface->opencl_ptr = (void *)addr;

				}

				ret = starpu_opencl_allocate_memory(&ptr,
							multiformat_interface->nx * multiformat_interface->ops->cpu_elemsize,
							CL_MEM_READ_WRITE);
				addr = (uintptr_t)ptr;
				if (ret)
				{
					fail = 1;
				}
				else
				{
					multiformat_interface->cpu_ptr = (void *) addr;
				}
				
				break;
			}
#endif
		default:
			STARPU_ABORT();
	}

	if (fail)
		return -ENOMEM;

	return allocated_memory;
}




/*
 * Copy methods
 */
static int copy_ram_to_ram(void *src_interface, unsigned src_node __attribute__ ((unused)),
			   void *dst_interface, unsigned dst_node __attribute__ ((unused)))
{
	struct starpu_multiformat_interface *src_multiformat;
	struct starpu_multiformat_interface *dst_multiformat;

	src_multiformat = (struct starpu_multiformat_interface *) src_interface;
	dst_multiformat = (struct starpu_multiformat_interface *) dst_interface;

	STARPU_ASSERT(src_multiformat != NULL);
	STARPU_ASSERT(dst_multiformat != NULL);
	STARPU_ASSERT(dst_multiformat->ops != NULL);

	size_t size = dst_multiformat->nx * dst_multiformat->ops->cpu_elemsize;
	memcpy(dst_multiformat->cpu_ptr, src_multiformat->cpu_ptr, size);

	return 0;
}

#ifdef STARPU_USE_CUDA
static int copy_cuda_common(void *src_interface, unsigned src_node __attribute__ ((unused)),
			    void *dst_interface, unsigned dst_node __attribute__ ((unused)),
			    enum cudaMemcpyKind kind)
{
	struct starpu_multiformat_interface *src_multiformat;
	struct starpu_multiformat_interface *dst_multiformat;

	src_multiformat = (struct starpu_multiformat_interface *) src_interface;
	dst_multiformat = (struct starpu_multiformat_interface *) dst_interface;

	size_t size;

	cudaError_t status;

	switch (kind)
	{
		case cudaMemcpyHostToDevice:
		{
			size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
			if (src_multiformat->cuda_ptr == NULL)
			{
				src_multiformat->cuda_ptr = malloc(size);
				if (src_multiformat->cuda_ptr == NULL)
					return -ENOMEM;
			}
			status = cudaMemcpy(dst_multiformat->cpu_ptr, src_multiformat->cpu_ptr, size, kind);
			if (STARPU_UNLIKELY(status))
			{
				STARPU_CUDA_REPORT_ERROR(status);
			}
			break;
		}
		case cudaMemcpyDeviceToHost:
		{
			size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
			status = cudaMemcpy(dst_multiformat->cuda_ptr, src_multiformat->cuda_ptr, size, kind);
			if (STARPU_UNLIKELY(status))
				STARPU_CUDA_REPORT_ERROR(status);

			break;
		}
		case cudaMemcpyDeviceToDevice:
		{
			size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
			status = cudaMemcpy(dst_multiformat->cuda_ptr, src_multiformat->cuda_ptr, size, kind);
			if (STARPU_UNLIKELY(status))
				STARPU_CUDA_REPORT_ERROR(status);
			break;
		}
		default:
			STARPU_ASSERT(0);
	}

	return 0;
}

static int copy_ram_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyHostToDevice);
}

static int copy_cuda_to_ram(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node)
{
	return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToHost);
}

static int copy_cuda_common_async(void *src_interface, unsigned src_node __attribute__ ((unused)),
				  void *dst_interface, unsigned dst_node __attribute__ ((unused)),
				  cudaStream_t stream, enum cudaMemcpyKind kind)
{
	struct starpu_multiformat_interface *src_multiformat;
	struct starpu_multiformat_interface *dst_multiformat;

	src_multiformat = (struct starpu_multiformat_interface *) src_interface;
	dst_multiformat = (struct starpu_multiformat_interface *) dst_interface;

	size_t size;
	cudaError_t status;

	switch (kind)
	{
		case cudaMemcpyHostToDevice:
		{
			size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
			if (src_multiformat->cuda_ptr == NULL)
			{
				src_multiformat->cuda_ptr = malloc(size);
				if (src_multiformat->cuda_ptr == NULL)
					return -ENOMEM;
			}

			status = cudaMemcpyAsync(dst_multiformat->cpu_ptr, src_multiformat->cpu_ptr, size, kind, stream);
			if (STARPU_UNLIKELY(status))
			{
				STARPU_CUDA_REPORT_ERROR(status);
			}
			break;
		}
		case cudaMemcpyDeviceToHost:
		{
			size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
			status = cudaMemcpy(dst_multiformat->cuda_ptr, src_multiformat->cuda_ptr, size, kind);
			if (STARPU_UNLIKELY(status))
				STARPU_CUDA_REPORT_ERROR(status);

			break;
		}
		case cudaMemcpyDeviceToDevice:
		{
			size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
			status = cudaMemcpyAsync(dst_multiformat->cuda_ptr, src_multiformat->cuda_ptr, size, kind, stream);
			if (STARPU_UNLIKELY(status))
				STARPU_CUDA_REPORT_ERROR(status);
			break;
		}
		default:
			STARPU_ASSERT(0);
	}

	return 0;
}

static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	return copy_cuda_common_async(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyHostToDevice);
}

static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node, cudaStream_t stream)
{
	return copy_cuda_common_async(src_interface, src_node, dst_interface, dst_node, stream, cudaMemcpyDeviceToHost);
}

#ifdef HAVE_CUDA_MEMCPY_PEER
static int copy_cuda_peer_common(void *src_interface, unsigned src_node,
				void *dst_interface, unsigned dst_node,
				cudaStream_t stream)
{
	struct starpu_multiformat_interface *src_multiformat;
	struct starpu_multiformat_interface *dst_multiformat;

	src_multiformat = (struct starpu_multiformat_interface *) src_interface;
	dst_multiformat = (struct starpu_multiformat_interface *) dst_interface;

	STARPU_ASSERT(src_multiformat != NULL);
	STARPU_ASSERT(dst_multiformat != NULL);
	STARPU_ASSERT(src_multiformat->ops != NULL);

	cudaError_t status;
	int size = src_multiformat->nx * src_multiformat->ops->cuda_elemsize;
	int src_dev = _starpu_memory_node_to_devid(src_node);
	int dst_dev = _starpu_memory_node_to_devid(dst_node);

	if (stream)
	{
		_STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
		status = cudaMemcpyPeerAsync(dst_multiformat->cuda_ptr, dst_dev,
					     src_multiformat->cuda_ptr, src_dev,
					     size, stream);
		_STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
		/* All good ! Still, returning -EAGAIN, because we will need to
                   check the transfert completion later */
		if (status == cudaSuccess)
			return -EAGAIN;
	}

	/* Either a synchronous transfert was requested, or the asynchronous one
           failed. */
	status = cudaMemcpyPeer(dst_multiformat->cuda_ptr, dst_dev,
				src_multiformat->cuda_ptr, src_dev,
				size);
	if (STARPU_UNLIKELY(status != cudaSuccess))
		STARPU_CUDA_REPORT_ERROR(status);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, size);

	return 0;
}
#endif
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *dst_interface, unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	if (src_node == dst_node)
	{
		return copy_cuda_common(src_interface, src_node, dst_interface, dst_node, cudaMemcpyDeviceToDevice);
	}
	else
	{
#ifdef HAVE_CUDA_MEMCPY_PEER
		return copy_cuda_peer_common(src_interface, src_node,
					     dst_interface, dst_node,
					     NULL);
#else
		STARPU_ASSERT(0);
#endif
	}
}

static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node,
				   void *dst_interface, unsigned dst_node,
				   cudaStream_t stream)
{
	if (src_node == dst_node)
	{
		return copy_cuda_common_async(src_interface, src_node,
					      dst_interface, dst_node,
					      stream, cudaMemcpyDeviceToDevice);
	}
	else
	{
#ifdef HAVE_CUDA_MEMCPY_PEER
		return copy_cuda_peer_common(src_interface, src_node,
					     dst_interface, dst_node,
					     stream);
#else
		STARPU_ASSERT(0);
#endif
	}
}
#endif /* STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node,
				    void *dst_interface, unsigned dst_node,
				    void *_event)
{
	int err, ret;
	size_t size;
	struct starpu_multiformat_interface *src_multiformat;
	struct starpu_multiformat_interface *dst_multiformat;

	src_multiformat = (struct starpu_multiformat_interface *) src_interface;
	dst_multiformat = (struct starpu_multiformat_interface *) dst_interface;

	STARPU_ASSERT(src_multiformat != NULL);
	STARPU_ASSERT(dst_multiformat != NULL);
	STARPU_ASSERT(src_multiformat->ops != NULL);

	size = src_multiformat->nx * src_multiformat->ops->opencl_elemsize;


	err = starpu_opencl_copy_ram_to_opencl_async_sync(src_multiformat->cpu_ptr,
							   src_node,
							   (cl_mem) dst_multiformat->cpu_ptr,
							   dst_node,
							   size,
							   0,
							   (cl_event *) _event,
							   &ret);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, size);
	return ret;
}

static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node,
				    void *dst_interface, unsigned dst_node,
				    void *_event)
{
	int err, ret;
	size_t size;
	struct starpu_multiformat_interface *src_multiformat;
	struct starpu_multiformat_interface *dst_multiformat;

	src_multiformat = (struct starpu_multiformat_interface *) src_interface;
	dst_multiformat = (struct starpu_multiformat_interface *) dst_interface;

	STARPU_ASSERT(src_multiformat != NULL);
	STARPU_ASSERT(dst_multiformat != NULL);
	STARPU_ASSERT(src_multiformat->ops != NULL);
	STARPU_ASSERT(dst_multiformat->ops != NULL);

	size = src_multiformat->nx * src_multiformat->ops->opencl_elemsize;

	if (dst_multiformat->opencl_ptr == NULL) {
		/* XXX : it is weird that we might have to allocate memory here... */
		dst_multiformat->opencl_ptr = malloc(dst_multiformat->nx * dst_multiformat->ops->opencl_elemsize);
	}
	err = starpu_opencl_copy_opencl_to_ram_async_sync((cl_mem)src_multiformat->opencl_ptr,
							   src_node,
							   dst_multiformat->opencl_ptr,
							   dst_node,
							   size,
                                                           0,
							   (cl_event *)_event,
							   &ret);
        if (STARPU_UNLIKELY(err))
                STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_TRACE_DATA_COPY(src_node, dst_node, size);


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

static int copy_opencl_to_opencl(void *src_interface, unsigned src_node,
                                 void *dst_interface, unsigned dst_node)
{
	(void) src_interface;
	(void) dst_interface;
	(void) src_node;
	(void) dst_node;

	STARPU_ASSERT_MSG(0, "XXX multiformat copy OpenCL-OpenCL not supported yet (TODO)");
	return 0;
}
#endif
