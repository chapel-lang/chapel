/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
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

#ifndef __DRIVER_CUDA_H__
#define __DRIVER_CUDA_H__

#include <assert.h>
#include <math.h>
#include <stdio.h>

#ifdef STARPU_USE_CUDA
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cublas.h>
#endif

#include <starpu.h>
#include <common/config.h>

#include <core/jobs.h>
#include <core/task.h>
#include <datawizard/datawizard.h>
#include <core/perfmodel/perfmodel.h>

#include <common/fxt.h>

unsigned _starpu_get_cuda_device_count(void);

#ifdef STARPU_USE_CUDA
void _starpu_init_cuda(void);
void *_starpu_cuda_worker(void *);
cudaStream_t starpu_cuda_get_local_transfer_stream(void);
#endif

#endif //  __DRIVER_CUDA_H__

