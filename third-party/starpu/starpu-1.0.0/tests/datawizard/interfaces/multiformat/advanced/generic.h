/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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
#ifndef MULTIFORMAT_GENERIC_H
#define MULTIFORMAT_GENERIC_H

#define NX 16

#ifdef STARPU_USE_CPU
void cpu_func(void *buffers[], void *args);
#endif /* !STARPU_USE_CPU */

#ifdef STARPU_USE_CUDA
void cuda_func(void *buffers[], void *args);
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
void opencl_func(void *buffers[], void *args);
#endif /* !STARPU_USE_OPENCL */
extern struct starpu_multiformat_data_interface_ops ops;
/* Counting the calls to the codelets */
struct stats
{
#ifdef STARPU_USE_CPU
	unsigned int cpu;
#endif
#ifdef STARPU_USE_CUDA
	unsigned int cuda;
	unsigned int cpu_to_cuda;
	unsigned int cuda_to_cpu;
#endif
#ifdef STARPU_USE_OPENCL
	unsigned int opencl;
	unsigned int cpu_to_opencl;
	unsigned int opencl_to_cpu;
#endif
};

void print_stats(struct stats *);
void reset_stats(struct stats *);
int  compare_stats(struct stats *, struct stats *);

#endif /* !MULTIFORMAT_GENERIC_H */
