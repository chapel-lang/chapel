/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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
#include <math.h>

/* Manage block and tags allocation */

static struct block_description *blocks;
static unsigned sizex, sizey, sizez;
static unsigned nbz;
static unsigned *block_sizes_z;

/*
 *	Tags for various codelet completion
 */

/*
 * common tag format:
 */
static starpu_tag_t tag_common(int z, int dir, int type)
{
	return (((((starpu_tag_t)type) << 4) | ((dir+1)/2)) << 32)|(starpu_tag_t)z;
}

/* Completion of last update tasks */
starpu_tag_t TAG_FINISH(int z)
{
	z = (z + nbz)%nbz;

	starpu_tag_t tag = tag_common(z, 0, 1);
	return tag;
}

/* Completion of the save codelet for MPI send/recv */
starpu_tag_t TAG_START(int z, int dir)
{
	z = (z + nbz)%nbz;

	starpu_tag_t tag = tag_common(z, dir, 2);
	return tag;
}

/*
 * common MPI tag format:
 * iter is actually not needed for coherency, but it makes debugging easier
 */
static int mpi_tag_common(int z, int iter, int dir, int buffer)
{
	return (((((iter << 12)|z)<<4) | ((1+dir)/2))<<4)|buffer;
}

int MPI_TAG0(int z, int iter, int dir)
{
	z = (z + nbz)%nbz;

	int tag = mpi_tag_common(z, iter, dir, 0);

	return tag;
}

int MPI_TAG1(int z, int iter, int dir)
{
	z = (z + nbz)%nbz;

	int tag = mpi_tag_common(z, iter, dir, 1);

	return tag;
}



/*
 *	Block descriptors
 */

/* Compute the size of the different blocks */
static void compute_block_sizes(void)
{
	block_sizes_z = (unsigned *) malloc(nbz*sizeof(unsigned));
	STARPU_ASSERT(block_sizes_z);

	/* Perhaps the last chunk is smaller */
	unsigned default_block_size = (sizez+nbz-1)/nbz;
	unsigned remaining = sizez;

	unsigned b;
	for (b = 0; b < nbz; b++)
	{
		block_sizes_z[b] = MIN(default_block_size, remaining);
		remaining -= block_sizes_z[b];
	}

	STARPU_ASSERT(remaining == 0);
}

unsigned get_block_size(int bz)
{
	return block_sizes_z[bz];
}

struct block_description *get_block_description(int z)
{
	z = (z + nbz)%nbz;

	STARPU_ASSERT(&blocks[z]);

	return &blocks[z];
}

unsigned get_block_mpi_node(int z)
{
	z = (z + nbz)%nbz;
	return blocks[z].mpi_node;
}

void create_blocks_array(unsigned _sizex, unsigned _sizey, unsigned _sizez, unsigned _nbz)
{
	/* Store the parameters */
	nbz = _nbz;
	sizex = _sizex;
	sizey = _sizey;
	sizez = _sizez;

	/* Create a grid of block descriptors */
	blocks = (struct block_description *) calloc(nbz, sizeof(struct block_description));
	STARPU_ASSERT(blocks);

	/* What is the size of the different blocks ? */
	compute_block_sizes();

	unsigned bz;
	for (bz = 0; bz < nbz; bz++)
	{
		struct block_description * block =
				get_block_description(bz);

		/* Which block is it ? */
		block->bz = bz;

		/* For simplicity, we store which are the neighbours blocks */
		block->boundary_blocks[B] = get_block_description((bz-1+nbz)%nbz);
		block->boundary_blocks[T] = get_block_description((bz+1)%nbz);
	}
}

/*
 *	Initialization of the blocks
 */

void assign_blocks_to_workers(int rank)
{
	unsigned bz;

	/* NB: perhaps we could count a GPU as multiple workers */

	/* how many workers are there ? */
	/*unsigned nworkers = starpu_worker_get_count();*/

	/* how many blocks are on that MPI node ? */
	unsigned nblocks = 0;
	for (bz = 0; bz < nbz; bz++)
	{
		struct block_description *block =
				get_block_description(bz);

		if (block->mpi_node == rank)
			nblocks++;
	}

	/* how many blocks per worker ? */
	/*unsigned nblocks_per_worker = (nblocks + nworkers - 1)/nworkers;*/

	/* we now attribute up to nblocks_per_worker blocks per workers */
	unsigned attributed = 0;
	for (bz = 0; bz < nbz; bz++)
	{
		struct block_description *block =
				get_block_description(bz);

		if (block->mpi_node == rank)
		{
			unsigned workerid;
			/* Manage initial block distribution between CPU and GPU */
		#if 0
			#if 1
			/* GPUs then CPUs */
			if (attributed < 3*18)
				workerid = attributed / 18;
			else
				workerid = 3+ (attributed - 3*18) / 2;
			#else
			/* GPUs interleaved with CPUs */
			if ((attributed % 20) <= 1)
				workerid = 3 + attributed / 20;
			else if (attributed < 60)
				workerid = attributed / 20;
			else
				workerid = (attributed - 60)/2 + 6;
			#endif
		#else
			/* Only GPUS */
			workerid = (attributed / 21) % 3;
		#endif
			/*= attributed/nblocks_per_worker;*/

			block->preferred_worker = workerid;

			attributed++;
		}
	}
}



void assign_blocks_to_mpi_nodes(int world_size)
{
	unsigned nzblocks_per_process = (nbz + world_size - 1) / world_size;

	unsigned bz;
	for (bz = 0; bz < nbz; bz++)
	{
		struct block_description *block =
				get_block_description(bz);

		block->mpi_node = bz / nzblocks_per_process;
	}
}

static size_t allocated = 0;

static void allocate_block_on_node(starpu_data_handle_t *handleptr, TYPE **ptr, unsigned nx, unsigned ny, unsigned nz)
{
	int ret;
	size_t block_size = nx*ny*nz*sizeof(TYPE);

	/* Allocate memory */
#if 1
	ret = starpu_malloc((void **)ptr, block_size);
	STARPU_ASSERT(ret == 0);
#else
	*ptr = malloc(block_size);
	STARPU_ASSERT(*ptr);
#endif

	allocated += block_size;

	/* Fill the blocks with 0 */
	memset(*ptr, 0, block_size);

	/* Register it to StarPU */
	starpu_block_data_register(handleptr, 0, (uintptr_t)*ptr, nx, nx*ny, nx, ny, nz, sizeof(TYPE));
}

void display_memory_consumption(int rank)
{
	fprintf(stderr, "%lu MB of memory were allocated on node %d\n", allocated/(1024*1024), rank);
}

void allocate_memory_on_node(int rank)
{
	unsigned bz;
	for (bz = 0; bz < nbz; bz++)
	{
		struct block_description *block = get_block_description(bz);

		unsigned node = block->mpi_node;

		unsigned size_bz = block_sizes_z[bz];
	
		/* Main blocks */
		if (node == rank)
		{
			allocate_block_on_node(&block->layers_handle[0], &block->layers[0],
						(sizex + 2*K), (sizey + 2*K), (size_bz + 2*K));
#ifdef LIFE
			unsigned x, y, z;
			unsigned sum = 0;
			for (x = 0; x < sizex; x++)
				for (y = 0; y < sizey; y++)
					for (z = 0; z < size_bz; z++)
						/* Just random data */
						sum += block->layers[0][(K+x)+(K+y)*(sizex + 2*K)+(K+z)*(sizex+2*K)*(sizey+2*K)] = (int)((x/7.+y/13.+(bz*size_bz + z)/17.) * 10.) % 2;
/*			printf("block %d starts with %d/%d alive\n", bz, sum, sizex*sizey*size_bz);*/
#endif
			allocate_block_on_node(&block->layers_handle[1], &block->layers[1],
						(sizex + 2*K), (sizey + 2*K), (size_bz + 2*K));
		}

		/* Boundary blocks : Top */
		unsigned top_node = block->boundary_blocks[T]->mpi_node;
		if ((node == rank) || (top_node == rank))
		{
			allocate_block_on_node(&block->boundaries_handle[T][0], &block->boundaries[T][0],
						(sizex + 2*K), (sizey + 2*K), K);
			allocate_block_on_node(&block->boundaries_handle[T][1], &block->boundaries[T][1],
						(sizex + 2*K), (sizey + 2*K), K);
		} 

		/* Boundary blocks : Bottom */
		unsigned bottom_node = block->boundary_blocks[B]->mpi_node;
		if ((node == rank) || (bottom_node == rank))
		{
			allocate_block_on_node(&block->boundaries_handle[B][0], &block->boundaries[B][0],
						(sizex + 2*K), (sizey + 2*K), K);
			allocate_block_on_node(&block->boundaries_handle[B][1], &block->boundaries[B][1],
						(sizex + 2*K), (sizey + 2*K), K);
		} 
	}
}

/* check how many cells are alive */
void check(int rank)
{
	unsigned bz;
	for (bz = 0; bz < nbz; bz++)
	{
		struct block_description *block = get_block_description(bz);

		unsigned node = block->mpi_node;

		/* Main blocks */
		if (node == rank)
		{
			unsigned size_bz = block_sizes_z[bz];
#ifdef LIFE
			unsigned x, y, z;
			unsigned sum = 0;
			for (x = 0; x < sizex; x++)
				for (y = 0; y < sizey; y++)
					for (z = 0; z < size_bz; z++)
						sum += block->layers[0][(K+x)+(K+y)*(sizex + 2*K)+(K+z)*(sizex+2*K)*(sizey+2*K)];
			printf("block %d got %d/%d alive\n", bz, sum, sizex*sizey*size_bz);
#endif
		}
	}
}
