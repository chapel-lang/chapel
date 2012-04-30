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

#include "lu_kernels_model.h"

/*
 * As a convention, in that file, buffers[0] is represented by A,
 * 				  buffers[1] is B ...
 */

/*
 *	Number of flops of Gemm 
 */

/* #define USE_PERTURBATION	1 */


#ifdef USE_PERTURBATION
#define PERTURBATE(a)	((starpu_drand48()*2.0f*(AMPL) + 1.0f - (AMPL))*(a))
#else
#define PERTURBATE(a)	(a)
#endif

/* 
 *
 *	Generic models
 *
 */

double task_11_cost(struct starpu_task *task, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/537.5);

	return PERTURBATE(cost);
}

double task_12_cost(struct starpu_task *task, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

/*	double cost = ((n*n*n)/1744.695); */
	double cost = ((n*n*n)/3210.80);

	/* fprintf(stderr, "task 12 predicts %e\n", cost); */
	return PERTURBATE(cost);
}


double task_21_cost(struct starpu_task *task, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

/*	double cost = ((n*n*n)/1744.695); */
	double cost = ((n*n*n)/3691.53);

	/* fprintf(stderr, "task 12 predicts %e\n", cost); */
	return PERTURBATE(cost);
}



double task_22_cost(struct starpu_task *task, unsigned nimpl)
{
	uint32_t nx, ny, nz;

	nx = starpu_matrix_get_nx(task->handles[2]);
	ny = starpu_matrix_get_ny(task->handles[2]);
	nz = starpu_matrix_get_ny(task->handles[0]);

	double cost = ((nx*ny*nz)/4110.0);

	return PERTURBATE(cost);
}

/* 
 *
 *	Models for CUDA
 *
 */


double task_11_cost_cuda(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/1853.7806);

/*	printf("CUDA task 11 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}

double task_12_cost_cuda(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/42838.5718);

/*	printf("CUDA task 12 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}


double task_21_cost_cuda(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/49208.667);

/*	printf("CUDA task 21 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}



double task_22_cost_cuda(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t nx, ny, nz;

	nx = starpu_matrix_get_nx(task->handles[2]);
	ny = starpu_matrix_get_ny(task->handles[2]);
	nz = starpu_matrix_get_ny(task->handles[0]);

	double cost = ((nx*ny*nz)/57523.560);

/*	printf("CUDA task 22 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}

/* 
 *
 *	Models for CPUs
 *
 */

double task_11_cost_cpu(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/537.5);

/*	printf("CPU task 11 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}

double task_12_cost_cpu(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/6668.224);

/*	printf("CPU task 12 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}


double task_21_cost_cpu(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t n;

	n = starpu_matrix_get_nx(task->handles[0]);

	double cost = ((n*n*n)/6793.8423);

/*	printf("CPU task 21 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}



double task_22_cost_cpu(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	uint32_t nx, ny, nz;

	nx = starpu_matrix_get_nx(task->handles[2]);
	ny = starpu_matrix_get_ny(task->handles[2]);
	nz = starpu_matrix_get_ny(task->handles[0]);

	double cost = ((nx*ny*nz)/4203.0175);

/*	printf("CPU task 22 ; predict %e\n", cost); */
	return PERTURBATE(cost);
}

struct starpu_perfmodel model_11 =
{
	.cost_function = task_11_cost,
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = task_11_cost_cpu },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = task_11_cost_cuda }
	},
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = "lu_model_11_atlas"
#elif defined(STARPU_GOTO)
	.symbol = "lu_model_11_goto"
#else
	.symbol = "lu_model_11"
#endif
};

struct starpu_perfmodel model_12 =
{
	.cost_function = task_12_cost,
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = task_12_cost_cpu },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = task_12_cost_cuda }
	},
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = "lu_model_12_atlas"
#elif defined(STARPU_GOTO)
	.symbol = "lu_model_12_goto"
#else
	.symbol = "lu_model_12"
#endif
};

struct starpu_perfmodel model_21 =
{
	.cost_function = task_21_cost,
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = task_21_cost_cpu },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = task_21_cost_cuda }
	},
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = "lu_model_21_atlas"
#elif defined(STARPU_GOTO)
	.symbol = "lu_model_21_goto"
#else
	.symbol = "lu_model_21"
#endif
};

struct starpu_perfmodel model_22 =
{
	.cost_function = task_22_cost,
	.per_arch =
	{
		[STARPU_CPU_DEFAULT][0] = { .cost_function = task_22_cost_cpu },
		[STARPU_CUDA_DEFAULT][0] = { .cost_function = task_22_cost_cuda }
	},
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = "lu_model_22_atlas"
#elif defined(STARPU_GOTO)
	.symbol = "lu_model_22_goto"
#else
	.symbol = "lu_model_22"
#endif
};
