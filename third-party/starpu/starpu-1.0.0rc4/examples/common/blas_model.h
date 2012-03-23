/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

#ifndef __BLAS_MODEL_H__
#define __BLAS_MODEL_H__

#include <starpu.h>

double gemm_cost(struct starpu_task *task, unsigned nimpl);

static struct starpu_perfmodel starpu_sgemm_model =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = "sgemm_atlas"
#elif defined(STARPU_GOTO)
	.symbol = "sgemm_goto"
#else
	.symbol = "sgemm"
#endif
};

static struct starpu_perfmodel starpu_sgemm_model_common =
{
	.cost_function = gemm_cost,
	.type = STARPU_COMMON,
};

static struct starpu_perfmodel starpu_dgemm_model =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = "dgemm_atlas"
#elif defined(STARPU_GOTO)
	.symbol = "dgemm_goto"
#else
	.symbol = "dgemm"
#endif
};

static struct starpu_perfmodel starpu_dgemm_model_common =
{
	.cost_function = gemm_cost,
	.type = STARPU_COMMON,
};

#endif /* __BLAS_MODEL_H__ */
