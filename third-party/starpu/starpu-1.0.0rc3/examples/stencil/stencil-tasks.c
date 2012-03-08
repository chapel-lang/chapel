/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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

#include "stencil.h"

#define BIND_LAST 1

/*
 * Schedule tasks for updates and saves
 */

/*
 * NB: iter = 0: initialization phase, TAG_U(z, 0) = TAG_INIT
 *
 * dir is -1 or +1.
 */

#if 0
# define DEBUG(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)
#else
# define DEBUG(fmt, ...) 
#endif

/*
 *	SAVE
 */

/* R(z) = R(z+d) = local, just call the save kernel */
static void create_task_save_local(unsigned iter, unsigned z, int dir, unsigned local_rank)
{
	struct starpu_task *save_task = starpu_task_create();
	struct block_description *descr = get_block_description(z);

	save_task->cl = (dir == -1)?&save_cl_bottom:&save_cl_top;
	save_task->cl_arg = descr;

	/* Saving our border... */
	save_task->handles[0] = descr->layers_handle[0];
	save_task->handles[1] = descr->layers_handle[1];

	/* ... to the neighbour's copy */
	struct block_description *neighbour = descr->boundary_blocks[(1+dir)/2];
	save_task->handles[2] = neighbour->boundaries_handle[(1-dir)/2][0];
	save_task->handles[3] = neighbour->boundaries_handle[(1-dir)/2][1];

	/* Bind */
	if (iter <= BIND_LAST)
		save_task->execute_on_a_specific_worker = get_bind_tasks();
	save_task->workerid = descr->preferred_worker;

	int ret = starpu_task_submit(save_task);
	if (ret)
	{
		fprintf(stderr, "Could not submit task save: %d\n", ret);
		STARPU_ASSERT(0);
	}
}

/* R(z) = local & R(z+d) != local */
/* We need to send our save over MPI */

static void send_done(void *arg)
{
	uintptr_t z = (uintptr_t) arg;
	DEBUG("DO SEND %d\n", (int)z);
}

#ifdef STARPU_USE_MPI
/* Post MPI send */
static void create_task_save_mpi_send(unsigned iter, unsigned z, int dir, unsigned local_rank)
{
	struct block_description *descr = get_block_description(z);
	STARPU_ASSERT(descr->mpi_node == local_rank);

	struct block_description *neighbour = descr->boundary_blocks[(1+dir)/2];
	int dest = neighbour->mpi_node;
	STARPU_ASSERT(neighbour->mpi_node != local_rank);

	/* Send neighbour's border copy to the neighbour */
	starpu_data_handle_t handle0 = neighbour->boundaries_handle[(1-dir)/2][0];
	starpu_data_handle_t handle1 = neighbour->boundaries_handle[(1-dir)/2][1];

	starpu_mpi_isend_detached(handle0, dest, MPI_TAG0(z, iter, dir), MPI_COMM_WORLD, send_done, (void*)(uintptr_t)z);
	starpu_mpi_isend_detached(handle1, dest, MPI_TAG1(z, iter, dir), MPI_COMM_WORLD, send_done, (void*)(uintptr_t)z);
}

/* R(z) != local & R(z+d) = local */
/* We need to receive over MPI */

static void recv_done(void *arg)
{
	uintptr_t z = (uintptr_t) arg;
	DEBUG("DO RECV %d\n", (int)z);
}

/* Post MPI recv */
static void create_task_save_mpi_recv(unsigned iter, unsigned z, int dir, unsigned local_rank)
{
	struct block_description *descr = get_block_description(z);
	STARPU_ASSERT(descr->mpi_node != local_rank);

	struct block_description *neighbour = descr->boundary_blocks[(1+dir)/2];
	int source = descr->mpi_node;
	STARPU_ASSERT(neighbour->mpi_node == local_rank);

	/* Receive our neighbour's border in our neighbour copy */
	starpu_data_handle_t handle0 = neighbour->boundaries_handle[(1-dir)/2][0];
	starpu_data_handle_t handle1 = neighbour->boundaries_handle[(1-dir)/2][1];

	starpu_mpi_irecv_detached(handle0, source, MPI_TAG0(z, iter, dir), MPI_COMM_WORLD, recv_done, (void*)(uintptr_t)z);
	starpu_mpi_irecv_detached(handle1, source, MPI_TAG1(z, iter, dir), MPI_COMM_WORLD, recv_done, (void*)(uintptr_t)z);
}
#endif /* STARPU_USE_MPI */

/*
 * Schedule saving boundaries of blocks to communication buffers
 */
void create_task_save(unsigned iter, unsigned z, int dir, unsigned local_rank)
{
	unsigned node_z = get_block_mpi_node(z);
	unsigned node_z_and_d = get_block_mpi_node(z+dir);

#ifdef STARPU_USE_MPI
	if (node_z == local_rank)
	{
		/* Save data from update */
		create_task_save_local(iter, z, dir, local_rank);
		if (node_z_and_d != local_rank)
		{ /* R(z) = local & R(z+d) != local, We have to send the data */
			create_task_save_mpi_send(iter, z, dir, local_rank);
		}

	}
	else
	{	/* node_z != local_rank, this MPI node doesn't have the saved data */
		if (node_z_and_d == local_rank)
		{
			create_task_save_mpi_recv(iter, z, dir, local_rank);
		}
		else
		{ /* R(z) != local & R(z+d) != local We don't have
			      the saved data and don't need it, we shouldn't
			      even have been called! */
			STARPU_ASSERT(0);
		}
	}
#else /* !STARPU_USE_MPI */
	STARPU_ASSERT((node_z == local_rank) && (node_z_and_d == local_rank));
	create_task_save_local(iter, z, dir, local_rank);
#endif /* STARPU_USE_MPI */
}

/*
 * Schedule update computation in computation buffer
 */

void create_task_update(unsigned iter, unsigned z, unsigned local_rank)
{
	STARPU_ASSERT(iter != 0);

	struct starpu_task *task = starpu_task_create();

	unsigned niter = get_niter();

	/* We are going to synchronize with the last tasks */
	if (iter == niter)
	{
		task->detach = 0;
		task->use_tag = 1;
		task->tag_id = TAG_FINISH(z);
	}

	unsigned old_layer = (K*(iter-1)) % 2;
	unsigned new_layer = (old_layer + 1) % 2;

	struct block_description *descr = get_block_description(z);
	task->handles[0] = descr->layers_handle[new_layer];
	task->handles[1] = descr->layers_handle[old_layer];

	task->handles[2] = descr->boundaries_handle[T][new_layer];
	task->handles[3] = descr->boundaries_handle[T][old_layer];

	task->handles[4] = descr->boundaries_handle[B][new_layer];
	task->handles[5] = descr->boundaries_handle[B][old_layer];

	task->cl = &cl_update;
	task->cl_arg = descr;

	if (iter <= BIND_LAST)
		task->execute_on_a_specific_worker = get_bind_tasks();
	task->workerid = descr->preferred_worker;

	int ret = starpu_task_submit(task);
	if (ret)
	{
		fprintf(stderr, "Could not submit task update block: %d\n", ret);
		STARPU_ASSERT(0);
	}
}

/* Dummy empty codelet taking one buffer */
static void null_func(void *descr[] __attribute__((unused)), void *arg __attribute__((unused))) { }
static struct starpu_codelet null =
{
	.modes = { STARPU_W, STARPU_W },
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
	.cpu_funcs = {null_func, NULL},
	.cuda_funcs = {null_func, NULL},
	.opencl_funcs = {null_func, NULL},
	.nbuffers = 2
};

void create_start_task(int z, int dir)
{
	/* Dumb task depending on the init task and simulating writing the
	   neighbour buffers, to avoid communications and computation running
	   before we start measuring time */
	struct starpu_task *wait_init = starpu_task_create();
	struct block_description *descr = get_block_description(z);
	starpu_tag_t tag_init = TAG_INIT_TASK;
	wait_init->cl = &null;
	wait_init->use_tag = 1;
	wait_init->tag_id = TAG_START(z, dir);
	wait_init->handles[0] = descr->boundaries_handle[(1 + dir) / 2][0];
	wait_init->handles[1] = descr->boundaries_handle[(1 + dir) / 2][1];
	starpu_tag_declare_deps_array(wait_init->tag_id, 1, &tag_init);

	int ret = starpu_task_submit(wait_init);
	if (ret)
	{
		fprintf(stderr, "Could not submit task initial wait: %d\n", ret);
		STARPU_ASSERT(0);
	}
}


/*
 * Create all the tasks
 */
void create_tasks(int rank)
{
	unsigned iter;
	unsigned bz;
	int niter = get_niter();
	int nbz = get_nbz();

	for (bz = 0; bz < nbz; bz++)
	{
		if ((get_block_mpi_node(bz) == rank) || (get_block_mpi_node(bz+1) == rank))
			create_start_task(bz, +1);
		if ((get_block_mpi_node(bz) == rank) || (get_block_mpi_node(bz-1) == rank))
			create_start_task(bz, -1);
	}

	for (iter = 0; iter <= niter; iter++)
	for (bz = 0; bz < nbz; bz++)
	{
		if ((iter > 0) && (get_block_mpi_node(bz) == rank))
			create_task_update(iter, bz, rank);

		if (iter != niter)
		{
			if ((get_block_mpi_node(bz) == rank) || (get_block_mpi_node(bz+1) == rank))
				create_task_save(iter, bz, +1, rank);
	
			if ((get_block_mpi_node(bz) == rank) || (get_block_mpi_node(bz-1) == rank))
				create_task_save(iter, bz, -1, rank);
		}
	}
}

/*
 * Wait for termination
 */
void wait_end_tasks(int rank)
{
	unsigned bz;
	int nbz = get_nbz();

	for (bz = 0; bz < nbz; bz++)
	{
		if (get_block_mpi_node(bz) == rank)
		{
			/* Wait for the task producing block "bz" */
			starpu_tag_wait(TAG_FINISH(bz));

			/* Get the result back to memory */
			struct block_description *block = get_block_description(bz);
			starpu_data_acquire(block->layers_handle[0], STARPU_R);
			starpu_data_acquire(block->layers_handle[1], STARPU_R);
		}
	}
}

