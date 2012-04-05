/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2011  Centre National de la Recherche Scientifique
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

static int dummy_copy(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
#ifdef STARPU_USE_CUDA
static int dummy_cuda_copy_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
#endif
#ifdef STARPU_USE_OPENCL
static int dummy_opencl_copy_async(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, void *_event);
#endif

static struct starpu_data_copy_methods void_copy_data_methods_s =
{
	.ram_to_ram = dummy_copy,
	.ram_to_spu = dummy_copy,
#ifdef STARPU_USE_CUDA
	.ram_to_cuda = dummy_copy,
	.cuda_to_ram = dummy_copy,
	.cuda_to_cuda = dummy_copy,
	.ram_to_cuda_async = dummy_cuda_copy_async,
	.cuda_to_ram_async = dummy_cuda_copy_async,
	.cuda_to_cuda_async = dummy_cuda_copy_async,
#endif
#ifdef STARPU_USE_OPENCL
	.ram_to_opencl = dummy_copy,
	.opencl_to_ram = dummy_copy,
        .ram_to_opencl_async = dummy_opencl_copy_async,
	.opencl_to_ram_async = dummy_opencl_copy_async,
#endif
	.cuda_to_spu = dummy_copy,
	.spu_to_ram = dummy_copy,
	.spu_to_cuda = dummy_copy,
	.spu_to_spu = dummy_copy
};

static void register_void_handle(starpu_data_handle_t handle, uint32_t home_node, void *data_interface);
static ssize_t allocate_void_buffer_on_node(void *data_interface_, uint32_t dst_node);
static void free_void_buffer_on_node(void *data_interface, uint32_t node);
static size_t void_interface_get_size(starpu_data_handle_t handle);
static uint32_t footprint_void_interface_crc32(starpu_data_handle_t handle);
static int void_compare(void *data_interface_a, void *data_interface_b);
static void display_void_interface(starpu_data_handle_t handle, FILE *f);

static struct starpu_data_interface_ops interface_void_ops =
{
	.register_data_handle = register_void_handle,
	.allocate_data_on_node = allocate_void_buffer_on_node,
	.free_data_on_node = free_void_buffer_on_node,
	.copy_methods = &void_copy_data_methods_s,
	.get_size = void_interface_get_size,
	.footprint = footprint_void_interface_crc32,
	.compare = void_compare,
	.interfaceid = STARPU_VOID_INTERFACE_ID,
	.interface_size = 0,
	.display = display_void_interface
};

static void register_void_handle(starpu_data_handle_t handle STARPU_ATTRIBUTE_UNUSED,
				uint32_t home_node STARPU_ATTRIBUTE_UNUSED,
				void *data_interface STARPU_ATTRIBUTE_UNUSED)
{
	/* Since there is no real data to register, we don't do anything */
}

/* declare a new data with the void interface */
void starpu_void_data_register(starpu_data_handle_t *handleptr)
{
	starpu_data_register(handleptr, 0, NULL, &interface_void_ops);
}


static uint32_t footprint_void_interface_crc32(starpu_data_handle_t handle STARPU_ATTRIBUTE_UNUSED)
{
	return 0;
}

static int void_compare(void *data_interface_a STARPU_ATTRIBUTE_UNUSED,
			void *data_interface_b STARPU_ATTRIBUTE_UNUSED)
{
	/* There is no allocation required, and therefore nothing to cache
	 * anyway. */
	return 1;
}

static void display_void_interface(starpu_data_handle_t handle STARPU_ATTRIBUTE_UNUSED, FILE *f)
{
	fprintf(f, "void\t");
}

static size_t void_interface_get_size(starpu_data_handle_t handle STARPU_ATTRIBUTE_UNUSED)
{
	return 0;
}

/* memory allocation/deallocation primitives for the void interface */

/* returns the size of the allocated area */
static ssize_t allocate_void_buffer_on_node(void *data_interface STARPU_ATTRIBUTE_UNUSED,
					uint32_t dst_node STARPU_ATTRIBUTE_UNUSED)
{
	/* Successfuly allocated 0 bytes */
	return 0;
}

static void free_void_buffer_on_node(void *data_interface STARPU_ATTRIBUTE_UNUSED ,
					uint32_t node STARPU_ATTRIBUTE_UNUSED)
{
	/* There is no buffer actually */
}

static int dummy_copy(void *src_interface STARPU_ATTRIBUTE_UNUSED,
			unsigned src_node STARPU_ATTRIBUTE_UNUSED,
			void *dst_interface STARPU_ATTRIBUTE_UNUSED,
			unsigned dst_node STARPU_ATTRIBUTE_UNUSED)
{
	return 0;
}

#ifdef STARPU_USE_CUDA
static int dummy_cuda_copy_async(void *src_interface STARPU_ATTRIBUTE_UNUSED,
				unsigned src_node STARPU_ATTRIBUTE_UNUSED,
				void *dst_interface STARPU_ATTRIBUTE_UNUSED,
				unsigned dst_node STARPU_ATTRIBUTE_UNUSED,
				cudaStream_t stream __attribute__ ((unused)))
{
	return 0;
}
#endif // STARPU_USE_CUDA

#ifdef STARPU_USE_OPENCL
static int dummy_opencl_copy_async(void *src_interface STARPU_ATTRIBUTE_UNUSED,
					unsigned src_node STARPU_ATTRIBUTE_UNUSED,
					void *dst_interface STARPU_ATTRIBUTE_UNUSED,
					unsigned dst_node STARPU_ATTRIBUTE_UNUSED,
					void *_event STARPU_ATTRIBUTE_UNUSED)
{
	return 0;
}
#endif // STARPU_USE_OPENCL
