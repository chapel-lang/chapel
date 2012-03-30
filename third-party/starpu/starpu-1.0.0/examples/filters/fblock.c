/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#include <starpu.h>
#include <starpu_opencl.h>

#define NX    5
#define NY    4
#define NZ    3
#define PARTS 2

#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

extern void cpu_func(void *buffers[], void *cl_arg);

#ifdef STARPU_USE_CUDA
extern void cuda_func(void *buffers[], void *cl_arg);
#endif

#ifdef STARPU_USE_OPENCL
extern void opencl_func(void *buffers[], void *cl_arg);
#endif

void print_block(int *block, int nx, int ny, int nz, unsigned ldy, unsigned ldz)
{
        int i, j, k;
        FPRINTF(stderr, "block=%p nx=%d ny=%d nz=%d ldy=%u ldz=%u\n", block, nx, ny, nz, ldy, ldz);
        for(k=0 ; k<nz ; k++)
	{
                for(j=0 ; j<ny ; j++)
		{
                        for(i=0 ; i<nx ; i++)
			{
                                FPRINTF(stderr, "%2d ", block[(k*ldz)+(j*ldy)+i]);
                        }
                        FPRINTF(stderr,"\n");
                }
                FPRINTF(stderr,"\n");
        }
        FPRINTF(stderr,"\n");
}

void print_data(starpu_data_handle_t block_handle)
{
	int *block = (int *)starpu_block_get_local_ptr(block_handle);
	int nx = starpu_block_get_nx(block_handle);
	int ny = starpu_block_get_ny(block_handle);
	int nz = starpu_block_get_nz(block_handle);
	unsigned ldy = starpu_block_get_local_ldy(block_handle);
	unsigned ldz = starpu_block_get_local_ldz(block_handle);

        print_block(block, nx, ny, nz, ldy, ldz);
}

#ifdef STARPU_USE_OPENCL
struct starpu_opencl_program opencl_program;
#endif

int main(int argc, char **argv)
{
        int *block,n=0;
        int i, j, k;
	int ret;

        block = (int*)malloc(NX*NY*NZ*sizeof(block[0]));
        assert(block);
        for(k=0 ; k<NZ ; k++)
	{
                for(j=0 ; j<NY ; j++)
		{
                        for(i=0 ; i<NX ; i++)
			{
                                block[(k*NX*NY)+(j*NX)+i] = n++;
                        }
                }
        }

	starpu_data_handle_t handle;
	struct starpu_codelet cl =
	{
                .where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
                .cpu_funcs = {cpu_func, NULL},
#ifdef STARPU_USE_CUDA
                .cuda_funcs = {cuda_func, NULL},
#endif
#ifdef STARPU_USE_OPENCL
                .opencl_funcs = {opencl_func, NULL},
#endif
		.nbuffers = 1,
                .modes = {STARPU_RW}
	};

        ret = starpu_init(NULL);
	if (ret == -ENODEV)
		exit(77);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

#ifdef STARPU_USE_OPENCL
        ret = starpu_opencl_load_opencl_from_file("examples/filters/fblock_opencl_kernel.cl", &opencl_program, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_opencl_load_opencl_from_file");
#endif

        /* Declare data to StarPU */
        starpu_block_data_register(&handle, 0, (uintptr_t)block, NX, NX*NY, NX, NY, NZ, sizeof(int));
        FPRINTF(stderr, "IN  Block\n");
        print_data(handle);

        /* Partition the block in PARTS sub-blocks */
	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func_block,
		.nchildren = PARTS
	};
        starpu_data_partition(handle, &f);

        FPRINTF(stderr,"Nb of partitions : %d\n",starpu_data_get_nb_children(handle));

        for(i=0 ; i<starpu_data_get_nb_children(handle) ; i++)
        {
                starpu_data_handle_t sblock = starpu_data_get_sub_data(handle, 1, i);
                FPRINTF(stderr, "Sub block %d\n", i);
                print_data(sblock);
        }

        /* Submit a task on each sub-block */
        for(i=0 ; i<starpu_data_get_nb_children(handle) ; i++)
        {
                int ret,multiplier=i;
                struct starpu_task *task = starpu_task_create();

                FPRINTF(stderr,"Dealing with sub-block %d\n", i);
                task->cl = &cl;
                task->synchronous = 1;
                task->callback_func = NULL;
                task->handles[0] = starpu_data_get_sub_data(handle, 1, i);
                task->cl_arg = &multiplier;

                ret = starpu_task_submit(task);
                if (ret)
		{
                        FPRINTF(stderr, "Error when submitting task\n");
                        exit(ret);
                }
        }

        /* Unpartition the data, unregister it from StarPU and shutdown */
        starpu_data_unpartition(handle, 0);
        print_data(handle);
        starpu_data_unregister(handle);

        /* Print result block */
        FPRINTF(stderr, "OUT Block\n");
        print_block(block, NX, NY, NZ, NX, NX*NY);

	free(block);

	starpu_shutdown();
	return 0;
}
