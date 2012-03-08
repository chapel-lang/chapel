/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
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

#ifndef __XLU_KERNELS_H__
#define __XLU_KERNELS_H__

#include <starpu.h>

void STARPU_LU(cpu_pivot)(void *descr[], void *_args);
void STARPU_LU(cpu_u11_pivot)(void *descr[], void *_args);
void STARPU_LU(cpu_u11)(void *descr[], void *_args);
void STARPU_LU(cpu_u12)(void *descr[], void *_args);
void STARPU_LU(cpu_u21)(void *descr[], void *_args);
void STARPU_LU(cpu_u22)(void *descr[], void *_args);

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_pivot)(void *descr[], void *_args);
void STARPU_LU(cublas_u11_pivot)(void *descr[], void *_args);
void STARPU_LU(cublas_u11)(void *descr[], void *_args);
void STARPU_LU(cublas_u12)(void *descr[], void *_args);
void STARPU_LU(cublas_u21)(void *descr[], void *_args);
void STARPU_LU(cublas_u22)(void *descr[], void *_args);
#endif

extern struct starpu_codelet cl11;
extern struct starpu_codelet cl11_pivot;
extern struct starpu_codelet cl12;
extern struct starpu_codelet cl21;
extern struct starpu_codelet cl22;
extern struct starpu_codelet cl_pivot;

#endif /* __XLU_KERNELS_H__ */
