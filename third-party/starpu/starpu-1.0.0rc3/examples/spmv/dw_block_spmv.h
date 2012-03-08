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

#ifndef __DW_BLOCK_SPMV_H__
#define __DW_BLOCK_SPMV_H__

#include <semaphore.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <cblas.h>

#include <starpu.h>

#ifdef STARPU_USE_CUDA
#include <cublas.h>
#endif

void cpu_block_spmv(void *descr[], void *_args);

#ifdef STARPU_USE_CUDA
void cublas_block_spmv(void *descr[], void *_args);
#endif /* STARPU_USE_CUDA */

#endif /* __DW_BLOCK_SPMV_H__ */
