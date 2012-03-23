/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010, 2011  Université de Bordeaux 1
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

/*
 * Conjugate gradients for Sparse matrices
 */

#include "dw_sparse_cg.h"
#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

static struct starpu_task *create_task(starpu_tag_t id)
{
	struct starpu_codelet *cl = calloc(1,sizeof(struct starpu_codelet));

	struct starpu_task *task = starpu_task_create();
		task->cl = cl;
		task->cl_arg = NULL;
		task->use_tag = 1;
		task->tag_id = id;

	return task;
}

static void create_data(float **_nzvalA, float **_vecb, float **_vecx, uint32_t *_nnz, uint32_t *_nrow, uint32_t **_colind, uint32_t **_rowptr)
{
	/* we need a sparse symetric (definite positive ?) matrix and a "dense" vector */
	
	/* example of 3-band matrix */
	float *nzval;
	uint32_t nnz;
	uint32_t *colind;
	uint32_t *rowptr;

	nnz = 3*size-2;

	nzval = malloc(nnz*sizeof(float));
	colind = malloc(nnz*sizeof(uint32_t));
	rowptr = malloc(size*sizeof(uint32_t));

	assert(nzval);
	assert(colind);
	assert(rowptr);


	/* fill the matrix */
	unsigned row;
	unsigned pos = 0;
	for (row = 0; row < size; row++)
	{
		rowptr[row] = pos;

		if (row > 0)
		{
			nzval[pos] = 1.0f;
			colind[pos] = row-1;
			pos++;
		}
		
		nzval[pos] = 5.0f;
		colind[pos] = row;
		pos++;

		if (row < size - 1)
		{
			nzval[pos] = 1.0f;
			colind[pos] = row+1;
			pos++;
		}
	}

	*_nnz = nnz;
	*_nrow = size;
	*_nzvalA = nzval;
	*_colind = colind;
	*_rowptr = rowptr;

	STARPU_ASSERT(pos == nnz);
	
	/* initiate the 2 vectors */
	float *invec, *outvec;
	invec = malloc(size*sizeof(float));
	assert(invec);

	outvec = malloc(size*sizeof(float));
	assert(outvec);

	/* fill those */
	unsigned ind;
	for (ind = 0; ind < size; ind++)
	{
		invec[ind] = 2.0f;
		outvec[ind] = 0.0f;
	}

	*_vecb = invec;
	*_vecx = outvec;
}

void init_problem(void)
{
	/* create the sparse input matrix */
	float *nzval;
	float *vecb;
	float *vecx;
	uint32_t nnz;
	uint32_t nrow;
	uint32_t *colind;
	uint32_t *rowptr;

	create_data(&nzval, &vecb, &vecx, &nnz, &nrow, &colind, &rowptr);

	conjugate_gradient(nzval, vecb, vecx, nnz, nrow, colind, rowptr);
}

/*
 *	cg initialization phase 
 */

void init_cg(struct cg_problem *problem) 
{
	int ret;

	problem->i = 0;

	/* r = b  - A x */
	struct starpu_task *task1 = create_task(1UL);
	task1->cl->where = STARPU_CPU;
	task1->cl->cpu_funcs[0] = cpu_codelet_func_1;
	task1->cl->nbuffers = 4;
	task1->cl->modes[0] = STARPU_R;
	task1->cl->modes[1] = STARPU_R;
	task1->cl->modes[2] = STARPU_W;
	task1->cl->modes[3] = STARPU_R;

	task1->handles[0] = problem->ds_matrixA;
	task1->handles[1] = problem->ds_vecx;
	task1->handles[2] = problem->ds_vecr;
	task1->handles[3] = problem->ds_vecb;

	/* d = r */
	struct starpu_task *task2 = create_task(2UL);
	task2->cl->where = STARPU_CPU;
	task2->cl->cpu_funcs[0] = cpu_codelet_func_2;
	task2->cl->nbuffers = 2;
	task2->cl->modes[0] = STARPU_W;
	task2->cl->modes[1] = STARPU_R;

	task2->handles[0] = problem->ds_vecd;
	task2->handles[1] = problem->ds_vecr;

	starpu_tag_declare_deps((starpu_tag_t)2UL, 1, (starpu_tag_t)1UL);

	/* delta_new = trans(r) r */
	struct starpu_task *task3 = create_task(3UL);
	task3->cl->where = STARPU_CUDA|STARPU_CPU;
#ifdef STARPU_USE_CUDA
	task3->cl->cuda_funcs[0] = cublas_codelet_func_3;
#endif
	task3->cl->cpu_funcs[0] = cpu_codelet_func_3;
	task3->cl_arg = problem;
	task3->cl->nbuffers = 1;
	task3->cl->modes[0] = STARPU_R;
	task3->handles[0] = problem->ds_vecr;

	task3->callback_func = iteration_cg;
	task3->callback_arg = problem;
	
	/* XXX 3 should only depend on 1 ... */
	starpu_tag_declare_deps((starpu_tag_t)3UL, 1, (starpu_tag_t)2UL);

	/* launch the computation now */
	ret = starpu_task_submit(task1);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task2);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task3);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
}

/*
 *	the inner iteration of the cg algorithm 
 *		the codelet code launcher is its own callback !
 */

void launch_new_cg_iteration(struct cg_problem *problem)
{
	int ret;

	unsigned iter = problem->i;

	unsigned long long maskiter = (iter*1024);

	/* q = A d */
	struct starpu_task *task4 = create_task(maskiter | 4UL);
	task4->cl->where = STARPU_CPU;
	task4->cl->cpu_funcs[0] = cpu_codelet_func_4;
	task4->cl->nbuffers = 3;
	task4->cl->modes[0] = STARPU_R;
	task4->cl->modes[1] = STARPU_R;
	task4->cl->modes[2] = STARPU_W;

	task4->handles[0] = problem->ds_matrixA;
	task4->handles[1] = problem->ds_vecd;
	task4->handles[2] = problem->ds_vecq;

	/* alpha = delta_new / ( trans(d) q )*/
	struct starpu_task *task5 = create_task(maskiter | 5UL);
	task5->cl->where = STARPU_CUDA|STARPU_CPU;
#ifdef STARPU_USE_CUDA
	task5->cl->cuda_funcs[0] = cublas_codelet_func_5;
#endif
	task5->cl->cpu_funcs[0] = cpu_codelet_func_5;
	task5->cl_arg = problem;
	task5->cl->nbuffers = 2;
	task5->cl->modes[0] = STARPU_R;
	task5->cl->modes[1] = STARPU_R;

	task5->handles[0] = problem->ds_vecd;
	task5->handles[1] = problem->ds_vecq;

	starpu_tag_declare_deps((starpu_tag_t)(maskiter | 5UL), 1, (starpu_tag_t)(maskiter | 4UL));

	/* x = x + alpha d */
	struct starpu_task *task6 = create_task(maskiter | 6UL);
	task6->cl->where = STARPU_CUDA|STARPU_CPU;
#ifdef STARPU_USE_CUDA
	task6->cl->cuda_funcs[0] = cublas_codelet_func_6;
#endif
	task6->cl->cpu_funcs[0] = cpu_codelet_func_6;
	task6->cl_arg = problem;
	task6->cl->nbuffers = 2;
	task6->cl->modes[0] = STARPU_RW;
	task6->cl->modes[1] = STARPU_R;

	task6->handles[0] = problem->ds_vecx;
	task6->handles[1] = problem->ds_vecd;

	starpu_tag_declare_deps((starpu_tag_t)(maskiter | 6UL), 1, (starpu_tag_t)(maskiter | 5UL));

	/* r = r - alpha q */
	struct starpu_task *task7 = create_task(maskiter | 7UL);
	task7->cl->where = STARPU_CUDA|STARPU_CPU;
#ifdef STARPU_USE_CUDA
	task7->cl->cuda_funcs[0] = cublas_codelet_func_7;
#endif
	task7->cl->cpu_funcs[0] = cpu_codelet_func_7;
	task7->cl_arg = problem;
	task7->cl->nbuffers = 2;
	task7->cl->modes[0] = STARPU_RW;
	task7->cl->modes[1] = STARPU_R;

	task7->handles[0] = problem->ds_vecr;
	task7->handles[1] = problem->ds_vecq;

	starpu_tag_declare_deps((starpu_tag_t)(maskiter | 7UL), 1, (starpu_tag_t)(maskiter | 6UL));

	/* update delta_* and compute beta */
	struct starpu_task *task8 = create_task(maskiter | 8UL);
	task8->cl->where = STARPU_CUDA|STARPU_CPU;
#ifdef STARPU_USE_CUDA
	task8->cl->cuda_funcs[0] = cublas_codelet_func_8;
#endif
	task8->cl->cpu_funcs[0] = cpu_codelet_func_8;
	task8->cl_arg = problem;
	task8->cl->nbuffers = 1;
	task8->cl->modes[0] = STARPU_R;
	task8->handles[0] = problem->ds_vecr;

	starpu_tag_declare_deps((starpu_tag_t)(maskiter | 8UL), 1, (starpu_tag_t)(maskiter | 7UL));

	/* d = r + beta d */
	struct starpu_task *task9 = create_task(maskiter | 9UL);
	task9->cl->where = STARPU_CUDA|STARPU_CPU;
#ifdef STARPU_USE_CUDA
	task9->cl->cuda_funcs[0] = cublas_codelet_func_9;
#endif
	task9->cl->cpu_funcs[0] = cpu_codelet_func_9;
	task9->cl_arg = problem;
	task9->cl->nbuffers = 2;
	task9->cl->modes[0] = STARPU_RW;
	task9->cl->modes[1] = STARPU_R;

	task9->handles[0] = problem->ds_vecd;
	task9->handles[1] = problem->ds_vecr;

	starpu_tag_declare_deps((starpu_tag_t)(maskiter | 9UL), 1, (starpu_tag_t)(maskiter | 8UL));

	task9->callback_func = iteration_cg;
	task9->callback_arg = problem;
	
	/* launch the computation now */
	ret = starpu_task_submit(task4);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task5);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task6);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task7);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task8);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	ret = starpu_task_submit(task9);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
}

void iteration_cg(void *problem)
{
	struct cg_problem *pb = problem;

	FPRINTF(stdout, "i : %d (MAX %d)\n\tdelta_new %f (%f)\n", pb->i, MAXITER, pb->delta_new, sqrt(pb->delta_new / pb->size));

	if ((pb->i < MAXITER) && 
		(pb->delta_new > pb->epsilon) )
	{
		if (pb->i % 1000 == 0)
			FPRINTF(stdout, "i : %d\n\tdelta_new %f (%f)\n", pb->i, pb->delta_new, sqrt(pb->delta_new / pb->size));

		pb->i++;

		/* we did not reach the stop condition yet */
		launch_new_cg_iteration(problem);
	}
	else
	{
		/* we may stop */
		FPRINTF(stdout, "We are done ... after %d iterations \n", pb->i - 1);
		FPRINTF(stdout, "i : %d\n\tdelta_new %2.5f\n", pb->i, pb->delta_new);
		sem_post(pb->sem);
	}
}

/*
 *	initializing the problem 
 */

void conjugate_gradient(float *nzvalA, float *vecb, float *vecx, uint32_t nnz,
			unsigned nrow, uint32_t *colind, uint32_t *rowptr)
{
	/* first register all the data structures to StarPU */

	starpu_data_handle_t ds_matrixA;
	starpu_data_handle_t ds_vecx, ds_vecb;
	starpu_data_handle_t ds_vecr, ds_vecd, ds_vecq; 

	/* first the user-allocated data */
	starpu_csr_data_register(&ds_matrixA, 0, nnz, nrow, 
			(uintptr_t)nzvalA, colind, rowptr, 0, sizeof(float));
	starpu_vector_data_register(&ds_vecx, 0, (uintptr_t)vecx, nrow, sizeof(float));
	starpu_vector_data_register(&ds_vecb, 0, (uintptr_t)vecb, nrow, sizeof(float));

	/* then allocate the algorithm intern data */
	float *ptr_vecr, *ptr_vecd, *ptr_vecq;

	unsigned i;
	ptr_vecr = malloc(nrow*sizeof(float));
	ptr_vecd = malloc(nrow*sizeof(float));
	ptr_vecq = malloc(nrow*sizeof(float));

	for (i = 0; i < nrow; i++)
	{
		ptr_vecr[i] = 0.0f;
		ptr_vecd[i] = 0.0f;
		ptr_vecq[i] = 0.0f;
	}

	FPRINTF(stdout, "nrow = %u \n", nrow);

	/* and register them as well */
	starpu_vector_data_register(&ds_vecr, 0, (uintptr_t)ptr_vecr, nrow, sizeof(float));
	starpu_vector_data_register(&ds_vecd, 0, (uintptr_t)ptr_vecd, nrow, sizeof(float));
	starpu_vector_data_register(&ds_vecq, 0, (uintptr_t)ptr_vecq, nrow, sizeof(float));

	/* we now have the complete problem */
	struct cg_problem problem;

	problem.ds_matrixA = ds_matrixA;
	problem.ds_vecx    = ds_vecx;
	problem.ds_vecb    = ds_vecb;
	problem.ds_vecr    = ds_vecr;
	problem.ds_vecd    = ds_vecd;
	problem.ds_vecq    = ds_vecq;

	problem.epsilon = EPSILON;
	problem.size = nrow;
	problem.delta_old = 1.0;
	problem.delta_new = 1.0; /* just to make sure we do at least one iteration */

	/* we need a semaphore to synchronize with callbacks */
	sem_t sem;
	sem_init(&sem, 0, 0U);
	problem.sem  = &sem;

	init_cg(&problem);

	sem_wait(&sem);
	sem_destroy(&sem);

	print_results(vecx, nrow);

	starpu_data_unregister(ds_matrixA);
	starpu_data_unregister(ds_vecx);
	starpu_data_unregister(ds_vecb);
	starpu_data_unregister(ds_vecr);
	starpu_data_unregister(ds_vecd);
	starpu_data_unregister(ds_vecq);
}


void do_conjugate_gradient(float *nzvalA, float *vecb, float *vecx, uint32_t nnz,
			unsigned nrow, uint32_t *colind, uint32_t *rowptr)
{
	/* start the runtime */
	int ret;

	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		exit(77);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_helper_cublas_init();

	conjugate_gradient(nzvalA, vecb, vecx, nnz, nrow, colind, rowptr);
}
