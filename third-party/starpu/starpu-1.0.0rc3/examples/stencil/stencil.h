/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#ifndef __STENCIL_H__
#define __STENCIL_H__

#include <stdlib.h>
#include <stdio.h>
#include <starpu.h>
#include <starpu_top.h>
#ifdef STARPU_USE_CUDA
#include <starpu_cuda.h>
#endif

#ifndef __CUDACC__
#ifdef STARPU_USE_MPI
#include <mpi.h>
#include <starpu_mpi.h>
#endif
#endif

#define LIFE

#ifdef LIFE
#define TYPE	unsigned char
extern void life_update(int bz, const TYPE *old, TYPE *newp, int nx, int ny, int nz, int ldy, int ldz, int iter);
#else
#define TYPE	float
#endif

#define K	1

#define NDIRS 2
extern struct starpu_top_data* starpu_top_init_loop;
extern struct starpu_top_data* starpu_top_achieved_loop;


/* Split only on the z axis to make things simple */
typedef enum
{
	B = 0,
	T = 1
} direction;

/* Description of a domain block */
struct block_description
{
	/* Which MPI node should process that block ? */
	unsigned mpi_node;
	
	unsigned preferred_worker;

	unsigned bz;


	/* For each of the following buffers, there are two (0/1) buffers to
	 * make new/old switch costless. */

	/* This is the computation buffer for this block, it includes
	 * neighbours' border to make computation easier */
	TYPE *layers[2];
	starpu_data_handle_t layers_handle[2];

	/* This is the "save" buffer, i.e. a copy of our neighbour's border.
	 * This one is used for CPU/GPU or MPI communication (rather than the
	 * whole domain block) */
	TYPE *boundaries[NDIRS][2];
	starpu_data_handle_t boundaries_handle[NDIRS][2];

	/* Shortcut pointer to the neighbours */
	struct block_description *boundary_blocks[NDIRS];
};

#define TAG_INIT_TASK			((starpu_tag_t)1)

starpu_tag_t TAG_FINISH(int z);
starpu_tag_t TAG_START(int z, int dir);
int MPI_TAG0(int z, int iter, int dir);
int MPI_TAG1(int z, int iter, int dir);

#define MIN(a,b)	((a)<(b)?(a):(b))

void create_blocks_array(unsigned sizex, unsigned sizey, unsigned sizez, unsigned nbz);
struct block_description *get_block_description(int z);
void assign_blocks_to_mpi_nodes(int world_size);
void allocate_memory_on_node(int rank);
void assign_blocks_to_workers(int rank);
void create_tasks(int rank);
void wait_end_tasks(int rank);
void check(int rank);

void display_memory_consumption(int rank);

unsigned get_block_mpi_node(int z);
unsigned get_block_size(int z);
unsigned get_bind_tasks(void);

unsigned get_nbz(void);
unsigned get_niter(void);
unsigned get_ticks(void);

unsigned global_workerid(unsigned local_workerid);

void create_task_update(unsigned iter, unsigned z, unsigned local_rank);
void create_task_save(unsigned iter, unsigned z, int dir, unsigned local_rank);

extern int starpu_mpi_initialize(void);
extern int starpu_mpi_shutdown(void);

/* kernels */
extern struct starpu_codelet cl_update;
extern struct starpu_codelet save_cl_bottom;
extern struct starpu_codelet save_cl_top;

extern unsigned update_per_worker[STARPU_NMAXWORKERS];
extern unsigned top_per_worker[STARPU_NMAXWORKERS];
extern unsigned bottom_per_worker[STARPU_NMAXWORKERS];

extern struct timeval start;
extern int who_runs_what_len;
extern int *who_runs_what;
extern int *who_runs_what_index;
extern struct timeval *last_tick;

#ifndef _externC
#define _externC
#endif

_externC void cuda_life_update_host(int bz, const TYPE *old, TYPE *newp, int nx, int ny, int nz, int ldy, int ldz, int iter);
_externC void cuda_shadow_host(int bz, TYPE *ptr, int nx, int ny, int nz, int ldy, int ldz, int i);

_externC void opencl_shadow_init(void);
_externC void opencl_shadow_free(void);
_externC void opencl_shadow_host(int bz, TYPE *ptr, int nx, int ny, int nz, int ldy, int ldz, int i);
_externC void opencl_life_init(void);
_externC void opencl_life_free(void);
_externC void opencl_life_update_host(int bz, const TYPE *old, TYPE *newp, int nx, int ny, int nz, int ldy, int ldz, int iter);

#endif /* __STENCIL_H__ */
