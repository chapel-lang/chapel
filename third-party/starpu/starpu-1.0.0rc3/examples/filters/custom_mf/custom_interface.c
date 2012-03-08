/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
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
#include <starpu_hash.h>
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif
#include "custom_interface.h"
#include "custom_types.h"

static int copy_ram_to_ram(void *src_interface, unsigned src_node,
			   void *dst_interface, unsigned dst_node);
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node,
			    void *dst_interface, unsigned dst_node);
static int copy_cuda_to_ram(void *src_interface, unsigned src_node,
			    void *dst_interface, unsigned dst_node);
static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node,
				  void *dst_interface, unsigned dst_node,
				  cudaStream_t stream);
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node,
				  void *dst_interface, unsigned dst_node,
				  cudaStream_t stream);
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node,
			     void *dst_interface, unsigned dst_node);
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node,
				   void *dst_interface, unsigned dst_node,
				   cudaStream_t stream);
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node,
			      void *dst_interface, unsigned dst_node);
static int copy_opencl_to_ram(void *src_interface, unsigned src_node,
			      void *dst_interface, unsigned dst_node);
static int copy_opencl_to_opencl(void *src_interface, unsigned src_node,
				 void *dst_interface, unsigned dst_node);
static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node,
				    void *dst_interface, unsigned dst_node,
				    void *event);
static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node,
				    void *dst_interface, unsigned dst_node,
				    void *event);
#endif /* !STARPU_USE_OPENCL */

static const struct starpu_data_copy_methods custom_copy_data_methods_s =
{
	.ram_to_ram = copy_ram_to_ram,
	.ram_to_spu = NULL,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda        = copy_ram_to_cuda,
	.cuda_to_ram        = copy_cuda_to_ram,
	.ram_to_cuda_async  = copy_ram_to_cuda_async,
	.cuda_to_ram_async  = copy_cuda_to_ram_async,
	.cuda_to_cuda       = copy_cuda_to_cuda,
	.cuda_to_cuda_async = copy_cuda_to_cuda_async,
#endif
#ifdef STARPU_USE_OPENCL
	.ram_to_opencl       = copy_ram_to_opencl,
	.opencl_to_ram       = copy_opencl_to_ram,
	.opencl_to_opencl    = copy_opencl_to_opencl,
        .ram_to_opencl_async = copy_ram_to_opencl_async,
	.opencl_to_ram_async = copy_opencl_to_ram_async,
#endif
	.cuda_to_spu = NULL,
	.spu_to_ram  = NULL,
	.spu_to_cuda = NULL,
	.spu_to_spu  = NULL
};

static void     register_custom_handle(starpu_data_handle_t handle,
				       uint32_t home_node,
				       void *data_interface);
static ssize_t  allocate_custom_buffer_on_node(void *data_interface_,
					       uint32_t dst_node);
static void*    custom_handle_to_pointer(starpu_data_handle_t data_handle,
					 uint32_t node);
static void     free_custom_buffer_on_node(void *data_interface, uint32_t node);
static size_t   custom_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_custom_interface_crc32(starpu_data_handle_t handle);
static int      custom_compare(void *data_interface_a, void *data_interface_b);
static void     display_custom_interface(starpu_data_handle_t handle, FILE *f);
static uint32_t custom_get_nx(starpu_data_handle_t handle);


static struct starpu_multiformat_data_interface_ops*get_mf_ops(void *data_interface)
{
	struct custom_data_interface *custom;
	custom = (struct custom_data_interface *) data_interface;

	return custom->ops;
}

static struct starpu_data_interface_ops interface_custom_ops =
{
	.register_data_handle  = register_custom_handle,
	.allocate_data_on_node = allocate_custom_buffer_on_node,
	.handle_to_pointer     = custom_handle_to_pointer,
	.free_data_on_node     = free_custom_buffer_on_node,
	.copy_methods          = &custom_copy_data_methods_s,
	.get_size              = custom_interface_get_size,
	.footprint             = footprint_custom_interface_crc32,
	.compare               = custom_compare,
#ifdef STARPU_USE_GORDON
	.convert_to_gordon     = NULL,
#endif
	.interfaceid           = -1,
	.interface_size        = sizeof(struct custom_data_interface),
	.display               = display_custom_interface,
	.is_multiformat        = 1,
	.get_mf_ops            = get_mf_ops
};

static void
register_custom_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface)
{
	struct custom_data_interface *custom_interface;
	custom_interface = (struct custom_data_interface *) data_interface;

	unsigned node;
	unsigned nnodes = starpu_memory_nodes_get_count();
	for (node = 0; node < nnodes; node++)
	{
		struct custom_data_interface *local_interface =
			(struct custom_data_interface *) starpu_data_get_interface_on_node(handle, node);

		if (node == home_node)
		{
			local_interface->cpu_ptr    = custom_interface->cpu_ptr;
#ifdef STARPU_USE_CUDA
			local_interface->cuda_ptr   = custom_interface->cuda_ptr;
#endif
#ifdef STARPU_USE_OPENCL
			local_interface->opencl_ptr = custom_interface->opencl_ptr;
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
		local_interface->nx = custom_interface->nx;
		local_interface->ops = custom_interface->ops;
	}
}

static ssize_t allocate_custom_buffer_on_node(void *data_interface, uint32_t node)
{
	ssize_t size = 0;
	struct custom_data_interface *custom_interface;
	custom_interface = (struct custom_data_interface *) data_interface;

	switch(starpu_node_get_kind(node))
	{
	case STARPU_CPU_RAM:
		size = custom_interface->nx * custom_interface->ops->cpu_elemsize;
		custom_interface->cpu_ptr = (void*) malloc(size);
		if (!custom_interface->cpu_ptr)
			return -ENOMEM;
#ifdef STARPU_USE_CUDA
		custom_interface->cuda_ptr = (void *) malloc(size);
		if (!custom_interface->cuda_ptr)
		{
			free(custom_interface->cpu_ptr);
			custom_interface->cpu_ptr = NULL;
			return -ENOMEM;
		}
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
		custom_interface->opencl_ptr = malloc(size);
		if (custom_interface->cuda_ptr == NULL)
		{
			free(custom_interface->cpu_ptr);
#ifdef STARPU_USE_CUDA
			free(custom_interface->cuda_ptr);
#endif /* !STARPU_USE_CUDA */
			return -ENOMEM;
		}
#endif /* !STARPU_USE_OPENCL */
			
		break;
#ifdef STARPU_USE_CUDA
	case STARPU_CUDA_RAM:
	{
		cudaError_t err;
		size = custom_interface->nx * custom_interface->ops->cpu_elemsize;
		err = cudaMalloc(&custom_interface->cuda_ptr, size);
		if (err != cudaSuccess)
			return -ENOMEM;

		err = cudaMalloc(&custom_interface->cpu_ptr, size);
		if (err != cudaSuccess)
		{
			cudaFree(custom_interface->cuda_ptr);
			return -ENOMEM;
		}
		break;
	}
#endif
#ifdef STARPU_USE_OPENCL
	case STARPU_OPENCL_RAM:
	{
		cl_int err;
		cl_mem memory;
		ssize_t size = custom_interface->nx * custom_interface->ops->cpu_elemsize;
		err = starpu_opencl_allocate_memory(&memory, size, CL_MEM_READ_WRITE);
		if (err != CL_SUCCESS)
			STARPU_OPENCL_REPORT_ERROR(err);

		custom_interface->opencl_ptr = memory;

		break;
	}
#endif /* !STARPU_USE_OPENCL */
	default:
		assert(0);
	}

	/* XXX We may want to return cpu_size + cuda_size + ... */
	return size;
}

static void free_custom_buffer_on_node(void *data_interface, uint32_t node)
{
	struct custom_data_interface *custom_interface;
	custom_interface = (struct custom_data_interface *) data_interface;

	switch(starpu_node_get_kind(node))
	{
	case STARPU_CPU_RAM:
		if (custom_interface->cpu_ptr != NULL)
		{
			free(custom_interface->cpu_ptr);
			custom_interface->cpu_ptr = NULL;
		}
#ifdef STARPU_USE_CUDA
		if (custom_interface->cuda_ptr != NULL)
		{
			free(custom_interface->cuda_ptr);
			custom_interface->cuda_ptr = NULL;
		}
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
		if (custom_interface->opencl_ptr != NULL)
		{
			free(custom_interface->opencl_ptr);
			custom_interface->opencl_ptr = NULL;
		}
#endif /* !STARPU_USE_OPENCL */
		break;
#ifdef STARPU_USE_CUDA
	case STARPU_CUDA_RAM:
		if (custom_interface->cpu_ptr != NULL)
		{
			cudaError_t err;
			err = cudaFree(custom_interface->cpu_ptr);
			if (err != cudaSuccess)
				fprintf(stderr, "cudaFree failed...\n");
		}
		if (custom_interface->cuda_ptr != NULL)
		{
			cudaError_t err;
			err = cudaFree(custom_interface->cuda_ptr);
			if (err != cudaSuccess)
				fprintf(stderr, "cudaFree failed...\n");
		}
		break;
#endif /* !STARPU_USE_CUDA */
	default:
		assert(0);
	}
}

static void*
custom_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	struct custom_data_interface *data_interface =
		(struct custom_data_interface *) starpu_data_get_interface_on_node(handle, node);


	switch(starpu_node_get_kind(node))
	{
		case STARPU_CPU_RAM:
			return data_interface->cpu_ptr;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
			return data_interface->cuda_ptr;
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
			return data_interface->opencl_ptr;
#endif
		default:
			assert(0);
	}
}

static size_t custom_interface_get_size(starpu_data_handle_t handle)
{
	size_t size;
	struct custom_data_interface *data_interface;

	data_interface = (struct custom_data_interface *)
				starpu_data_get_interface_on_node(handle, 0);
	size = data_interface->nx * data_interface->ops->cpu_elemsize;
	return size;
}

static uint32_t footprint_custom_interface_crc32(starpu_data_handle_t handle)
{
	return starpu_crc32_be(custom_get_nx(handle), 0);
}

static int custom_compare(void *data_interface_a, void *data_interface_b)
{
	/* TODO */
	assert(0);
}

static void display_custom_interface(starpu_data_handle_t handle, FILE *f)
{
	/* TODO */
	assert(0);
}

static uint32_t
custom_get_nx(starpu_data_handle_t handle)
{
	struct custom_data_interface *data_interface;
	data_interface = (struct custom_data_interface *)
				starpu_data_get_interface_on_node(handle, 0);
	return data_interface->nx;
}


void custom_data_register(starpu_data_handle_t *handle,
				 uint32_t home_node,
				 void *ptr,
				 uint32_t nx,
				 struct starpu_multiformat_data_interface_ops *format_ops)
{
	/* XXX Deprecated fields ? */
	struct custom_data_interface custom =
	{
		.cpu_ptr = ptr,
#ifdef STARPU_USE_CUDA
		.cuda_ptr = NULL,
#endif
#ifdef STARPU_USE_OPENCL
		.opencl_ptr = NULL,
#endif
		.nx  = nx,
		.ops = format_ops
	};

	if (interface_custom_ops.interfaceid == -1) {
		interface_custom_ops.interfaceid = starpu_data_interface_get_next_id();
	}
	starpu_data_register(handle, home_node, &custom, &interface_custom_ops);
}

static int copy_ram_to_ram(void *src_interface, unsigned src_node,
			   void *dst_interface, unsigned dst_node)
{
	/* TODO */
	assert(0);
}
#ifdef STARPU_USE_CUDA
static int copy_ram_to_cuda(void *src_interface, unsigned src_node,
			    void *dst_interface, unsigned dst_node)
{
	/* TODO */
	assert(0);
}
static int copy_cuda_to_ram(void *src_interface, unsigned src_node,
			    void *dst_interface, unsigned dst_node)
{
	/* TODO */
	assert(0);
}

static int
copy_cuda_common_async(void *src_interface, unsigned src_node,
		       void *dst_interface, unsigned dst_node,
		       cudaStream_t stream, enum cudaMemcpyKind kind)
{
	struct custom_data_interface *src_custom, *dst_custom;

	src_custom = (struct custom_data_interface *) src_interface;
	dst_custom = (struct custom_data_interface *) dst_interface;

	ssize_t size = 0;
	cudaError_t err;

	switch (kind)
	{
	case cudaMemcpyHostToDevice:
	{
		size = src_custom->nx * src_custom->ops->cpu_elemsize;
		if (dst_custom->cpu_ptr == NULL)
		{
			err = cudaMalloc(&dst_custom->cpu_ptr, size);
			assert(err == cudaSuccess);
		}

		err = cudaMemcpyAsync(dst_custom->cpu_ptr,
				      src_custom->cpu_ptr,
				      size, kind, stream);
		assert(err == cudaSuccess);


		err = cudaMalloc(&dst_custom->cuda_ptr, size);
		assert(err == cudaSuccess);
		break;
	}
	case cudaMemcpyDeviceToHost:
		size = 2*src_custom->nx*sizeof(float);
		if (dst_custom->cuda_ptr == NULL)
		{
			dst_custom->cuda_ptr = malloc(size);
			if (dst_custom->cuda_ptr == NULL)
				return -ENOMEM;
		}
		err = cudaMemcpyAsync(dst_custom->cuda_ptr,
				      src_custom->cuda_ptr,
				      size, kind, stream);
		assert(err == cudaSuccess);
		break;
	default:
		assert(0);
	}

	return 0;
}

static int copy_ram_to_cuda_async(void *src_interface, unsigned src_node,
				  void *dst_interface, unsigned dst_node,
				  cudaStream_t stream)
{
	return copy_cuda_common_async(src_interface, src_node,
				      dst_interface, dst_node,
				      stream, cudaMemcpyHostToDevice);
}
static int copy_cuda_to_ram_async(void *src_interface, unsigned src_node,
				  void *dst_interface, unsigned dst_node,
				  cudaStream_t stream)
{
	return copy_cuda_common_async(src_interface, src_node,
				      dst_interface, dst_node,
				      stream, cudaMemcpyDeviceToHost);
}
static int copy_cuda_to_cuda(void *src_interface, unsigned src_node,
			     void *dst_interface, unsigned dst_node)
{
	assert(0);
}
static int copy_cuda_to_cuda_async(void *src_interface, unsigned src_node,
				   void *dst_interface, unsigned dst_node,
				   cudaStream_t stream)
{
	assert(0);
}
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static int copy_ram_to_opencl(void *src_interface, unsigned src_node,
			      void *dst_interface, unsigned dst_node)
{
	(void) src_interface;
	(void) src_node;
	(void) dst_interface;
	(void) dst_node;
	return 0;
}

static int copy_opencl_to_ram(void *src_interface, unsigned src_node,
			      void *dst_interface, unsigned dst_node)
{
	(void) src_interface;
	(void) src_node;
	(void) dst_interface;
	(void) dst_node;
	return 0;
}

static int copy_opencl_to_opencl(void *src_interface, unsigned src_node,
				 void *dst_interface, unsigned dst_node)
{
	(void) src_interface;
	(void) src_node;
	(void) dst_interface;
	(void) dst_node;
	return 0;
}

static int copy_ram_to_opencl_async(void *src_interface, unsigned src_node,
				    void *dst_interface, unsigned dst_node,
				    void *event)
{
	ssize_t size;
	struct custom_data_interface *src_custom, *dst_custom;

	src_custom = (struct custom_data_interface *) src_interface;
	dst_custom = (struct custom_data_interface *) dst_interface;

	/*
	 * Opencl stuff.
	 */
	cl_context context;
	cl_command_queue queue;
	int id = starpu_worker_get_id();
	int devid = starpu_worker_get_devid(id);
	starpu_opencl_get_queue(devid, &queue);
	starpu_opencl_get_context(devid, &context);

	/* Real stuff */
	int err;
	cl_int ret;

	size = src_custom->nx * 2 * sizeof(float);
	if (dst_custom->cpu_ptr == NULL)
	{
		ret = starpu_opencl_allocate_memory((cl_mem*)&dst_custom->cpu_ptr,
				size, CL_MEM_READ_WRITE);
		assert(ret == CL_SUCCESS);
	}
	err = starpu_opencl_copy_ram_to_opencl_async_sync(src_custom->cpu_ptr,
							  src_node,
							  dst_custom->cpu_ptr,
							  dst_node,
							  size,
							  0,
							  NULL,
							  &ret);
	assert(err == 0);
	return 0;
}

static int copy_opencl_to_ram_async(void *src_interface, unsigned src_node,
				    void *dst_interface, unsigned dst_node,
				    void *event)
{
	ssize_t size;
	struct custom_data_interface *src_custom, *dst_custom;

	src_custom = (struct custom_data_interface *) src_interface;
	dst_custom = (struct custom_data_interface *) dst_interface;

	/*
	 * Opencl stuff.
	 */
	cl_context context;
	cl_command_queue queue;
	int id = starpu_worker_get_id();
	int devid = starpu_worker_get_devid(id);
	starpu_opencl_get_queue(devid, &queue);
	starpu_opencl_get_context(devid, &context);

	/* real stuff */
	int err;
	cl_int ret;
	size = src_custom->nx * 2 * sizeof(float);
	if (!dst_custom->opencl_ptr)
	{
		dst_custom->opencl_ptr = malloc(size);
		assert(dst_custom->opencl_ptr != NULL);
	}

	err = starpu_opencl_copy_opencl_to_ram_async_sync(
			src_custom->opencl_ptr,
			src_node,
			dst_custom->opencl_ptr,
			dst_node,
			size,
			0,
			NULL,
			&ret);
	assert(err == 0);
	return 0;
}
#endif /* !STARPU_USE_OPENCL */
