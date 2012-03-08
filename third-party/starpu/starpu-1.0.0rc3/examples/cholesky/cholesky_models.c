/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2011  Télécom-SudParis
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

/*
 * As a convention, in that file, buffers[0] is represented by A,
 * 				  buffers[1] is B ...
 */

/*
 *	Number of flops of Gemm 
 */

#include <starpu.h>
#include "cholesky.h"

/* #define USE_PERTURBATION	1 */

#ifdef USE_PERTURBATION
#define PERTURBATE(a)	((starpu_drand48()*2.0f*(AMPL) + 1.0f - (AMPL))*(a))
#else
#define PERTURBATE(a)	(a)
#endif

static double cpu_chol_task_11_cost(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = (((double)(n)*n*n)/1000.0f*0.894/0.79176);

#ifdef STARPU_MODEL_DEBUG
	FPRINTF(stdout, "cpu_chol_task_11_cost n %d cost %e\n", n, cost);
#endif

	return PERTURBATE(cost);
}

static double cuda_chol_task_11_cost(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = (((double)(n)*n*n)/50.0f/10.75/5.088633/0.9883);

#ifdef STARPU_MODEL_DEBUG
	FPRINTF(stdout, "cuda_chol_task_11_cost n %d cost %e\n", n, cost);
#endif

	return PERTURBATE(cost);
}

static double cpu_chol_task_21_cost(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = (((double)(n)*n*n)/7706.674/0.95/0.9965);

#ifdef STARPU_MODEL_DEBUG
	FPRINTF(stdout, "cpu_chol_task_21_cost n %d cost %e\n", n, cost);
#endif

	return PERTURBATE(cost);
}

static double cuda_chol_task_21_cost(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = (((double)(n)*n*n)/50.0f/10.75/87.29520);

#ifdef STARPU_MODEL_DEBUG
	FPRINTF(stdout, "cuda_chol_task_21_cost n %d cost %e\n", n, cost);
#endif

	return PERTURBATE(cost);
}

static double cpu_chol_task_22_cost(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = (((double)(n)*n*n)/50.0f/10.75/8.0760);

#ifdef STARPU_MODEL_DEBUG
	FPRINTF(stdout, "cpu_chol_task_22_cost n %d cost %e\n", n, cost);
#endif

	return PERTURBATE(cost);
}

static double cuda_chol_task_22_cost(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = (((double)(n)*n*n)/50.0f/10.75/76.30666);

#ifdef STARPU_MODEL_DEBUG
	FPRINTF(stdout, "cuda_chol_task_22_cost n %d cost %e\n", n, cost);
#endif

	return PERTURBATE(cost);
}

struct starpu_perfmodel chol_model_11 =
{
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = cpu_chol_task_11_cost },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = cuda_chol_task_11_cost }
	},
	.type = STARPU_HISTORY_BASED,
	.symbol = "chol_model_11"
};

struct starpu_perfmodel chol_model_21 =
{
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = cpu_chol_task_21_cost },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = cuda_chol_task_21_cost }
	},
	.type = STARPU_HISTORY_BASED,
	.symbol = "chol_model_21"
};

struct starpu_perfmodel chol_model_22 =
{
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = cpu_chol_task_22_cost },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = cuda_chol_task_22_cost }
	},
	.type = STARPU_HISTORY_BASED,
	.symbol = "chol_model_22"
};
