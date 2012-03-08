/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
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

extern struct starpu_codelet scal_codelet;
extern struct starpu_opencl_program opencl_program;

void scal_func_cpu(void *buffers[], void *cl_arg);

#ifdef STARPU_USE_CUDA
void scal_func_cuda(void *buffers[], void *cl_arg);
#endif

#ifdef STARPU_USE_OPENCL
void scal_func_opencl(void *buffers[], void *_args);
#endif

