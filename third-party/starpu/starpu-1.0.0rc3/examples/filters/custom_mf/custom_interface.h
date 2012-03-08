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
#ifndef __CUSTOM_INTERFACE_H__
#define __CUSTOM_INTERFACE_H__
#include <starpu.h>
struct custom_data_interface
{
	void *cpu_ptr;
	void *cuda_ptr;
	void *opencl_ptr;
	struct starpu_multiformat_data_interface_ops *ops;
	uint32_t nx;
};

void custom_data_register(starpu_data_handle_t *handle,
				 uint32_t home_node,
				 void *ptr,
				 uint32_t nx,
				 struct starpu_multiformat_data_interface_ops* ops);

#define CUSTOM_GET_NX(interface) (((struct custom_data_interface*)(interface))->nx)
#define CUSTOM_GET_CPU_PTR(interface) (((struct custom_data_interface*)(interface))->cpu_ptr)

#ifdef STARPU_USE_CUDA
#define CUSTOM_GET_X_PTR(interface) (((struct custom_data_interface*)(interface))->cuda_ptr)
#define CUSTOM_GET_Y_PTR(interface) \
	(((struct custom_data_interface*)(interface))->cuda_ptr)+ \
	CUSTOM_GET_NX((interface))
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
#define CUSTOM_GET_OPENCL_X_PTR(interface) (((struct custom_data_interface *)(interface))->opencl_ptr)
#endif

#endif /* ! __CUSTOM_INTERFACE_H__ */
