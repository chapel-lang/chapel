/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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
#include "mm_to_bcsr.h"

/* Some debug functions */

static void print_block(tmp_block_t *block, unsigned r, unsigned c)
{
	printf(" **** block %d %d **** \n", block->i, block->j);

	unsigned i, j;
	for (j = 0; j < r; j++)
	{
		for (i = 0; i < c; i++)
		{
			printf("%2.2f\t", block->val[i + j*c]);
		}
		printf("\n");
	}
}

static void print_all_blocks(tmp_block_t *block_list, unsigned r, unsigned c)
{
	tmp_block_t *current_block = block_list;

	while(current_block)
	{
		print_block(current_block, r, c);

		current_block = current_block->next;
	}
}

static void print_bcsr(bcsr_t *bcsr)
{
	fprintf(stderr, "** BSCR **\n");
	fprintf(stderr, "non zero - blocks = %d\n", bcsr->nnz_blocks);
	fprintf(stderr, "nrows - blocks = %d\n", bcsr->nrows_blocks);
	fprintf(stderr, "block size : c %d r %d\n", bcsr->c, bcsr->r);
}

static unsigned count_blocks(tmp_block_t *block_list)
{
	unsigned count = 0;
	tmp_block_t *current_block = block_list;

	while(current_block)
	{
		count++;
		current_block = current_block->next;
	}

	return count;
}

static unsigned count_row_blocks(tmp_block_t *block_list)
{
	unsigned maxrow = 0;
	tmp_block_t *current_block = block_list;

	while(current_block)
	{
		if (current_block->j > maxrow)
			maxrow = current_block->j;

		current_block = current_block->next;
	}

	return (maxrow+1);
}



/* Find the block that corresponds to (i,j) if it exists in the list */

static tmp_block_t *search_block(tmp_block_t *block_list, unsigned i, unsigned j)
{
	tmp_block_t *current_block = block_list;
	/* printf("search %d %d\n", i, j); */

	while (current_block)
	{
		if ((current_block->i == i) && (current_block->j == j)) 
		{
			/* we found the block */
			return current_block;
		}

		current_block = current_block->next;
	};

	/* no entry was found ... */
	return NULL;
}

static tmp_block_t *create_block(unsigned c, unsigned r)
{
	tmp_block_t *block;

	block = malloc(sizeof(tmp_block_t));
	block->val = calloc(c*r, sizeof(float));

	return block;
}

/* determine if next block is bigger in lexical order */
static unsigned next_block_is_bigger(tmp_block_t *block, unsigned i, unsigned j)
{
	tmp_block_t *next = block->next;

	if (next)
	{
		/* we evaluate lexical order */
		if (next->j < j)
			return 0;

		if (next->j > j)
			return 1;

		/* next->j == j */
		return (next->i > i);
	}

	/* this is the last block, so it's bigger */
	return 1;
}

/* we insert a block in the list, directly at the appropriate place */
static void insert_block(tmp_block_t *block, tmp_block_t **block_list, unsigned i, unsigned j)
{
	/* insert block at the beginning of the list */
	/*block->next = *block_list;
	*block_list = block; */

	/* insert the block in lexicographical order */
	/* first find an element that is bigger, then insert the block just before it */
	tmp_block_t *current_block = *block_list;

	if (!current_block)
	{
		/* list was empty */
		*block_list = block;
		block->next = NULL;
		return;
	}

	while (current_block)
	{
		if (next_block_is_bigger(current_block, i, j))
		{
			/* insert block here */
			block->next = current_block->next;
			current_block->next = block;
			return;
		}

		current_block = current_block->next;
	};

	/* should not be reached ! */
}

/* we add an element to the list of blocks, it is either added to an existing block or in a block specifically created if there was none */
static void insert_elem(tmp_block_t **block_list, unsigned abs_i, unsigned abs_j, float val, unsigned c, unsigned r)
{
	/* we are looking for the block that contains (abs_i, abs_j) (abs = absolute) */
	unsigned i,j;

	i = abs_i / c;
	j = abs_j / r;

	tmp_block_t *block;

	block = search_block(*block_list, i, j);

	if (!block)
	{
		/* the block does not exist yet */
		/* create it */
		block = create_block(c, r);

		block->i = i;
		block->j = j;
		
		/* printf("create block %d %d !\n", i, j); */

		/* insert it in the block list */
		insert_block(block, block_list, i, j);
	}

	/* now insert the value in the corresponding block */
	unsigned local_i, local_j, local_index;

	local_i = abs_i % c;
	local_j = abs_j % r;
	local_index = local_j * c + local_i;
	
	block->val[local_index] = val;
}

/* transform a list of values (with coordinates) into a list of blocks that are easily processed into BCSR */
static tmp_block_t * mm_to_blocks(int nz, unsigned *I, unsigned *J, float *val, unsigned c, unsigned r)
{
	int elem;

	/* at first, the list of block is empty */
	tmp_block_t *block_list = NULL;

	for (elem = 0; elem < nz; elem++)
	{
		insert_elem(&block_list, I[elem], J[elem], val[elem], c, r);
	}

	return block_list;
}

static void fill_bcsr(tmp_block_t *block_list, unsigned c, unsigned r, bcsr_t *bcsr)
{
	unsigned block = 0;
	unsigned current_offset = 0;
	size_t block_size = c*r*sizeof(float);

	tmp_block_t *current_block = block_list;

	while(current_block)
	{
		/* copy the val from the block to the contiguous area in the BCSR */
		memcpy(&bcsr->val[current_offset], current_block->val, block_size);

		/* write the the index of the block 
		 * XXX should it be in blocks ? */
		bcsr->colind[block] = current_block->i;

		if ((bcsr->rowptr[current_block->j] == 0) && (current_block->j != 0))
		{
			/* this is the first element of the line */
			bcsr->rowptr[current_block->j] = block;
		}

		block++;
		current_offset = block*c*r;
		current_block = current_block->next;
	};

	/* for all lines where there were no block at all (XXX), fill the 0 in rowptr */
	/* the first row must start at 0 ? */
	bcsr->rowptr[0] = 0;

	unsigned row;
	for (row = 1; row < bcsr->nrows_blocks; row++)
	{
		if (bcsr->rowptr[row] == 0) 
			bcsr->rowptr[row] = bcsr->rowptr[row-1];
	}

	bcsr->rowptr[bcsr->nrows_blocks] = bcsr->nnz_blocks;
}

static bcsr_t * blocks_to_bcsr(tmp_block_t *block_list, unsigned c, unsigned r)
{
	unsigned nblocks;

	/* print_all_blocks(block_list, r, c); */

	nblocks = count_blocks(block_list);

	bcsr_t *bcsr = malloc(sizeof(bcsr_t));

	bcsr->nnz_blocks = nblocks;
	bcsr->r = r;
	bcsr->c = c;
	
	unsigned nrows_blocks = count_row_blocks(block_list);
	bcsr->nrows_blocks = nrows_blocks;

	bcsr->val = malloc(nblocks*r*c*sizeof(float));
	bcsr->colind = malloc(nblocks*sizeof(unsigned));
	bcsr->rowptr = calloc((nrows_blocks + 1), sizeof(unsigned));

	fill_bcsr(block_list, c, r, bcsr);

	return bcsr;
}

bcsr_t *mm_to_bcsr(unsigned nz, unsigned *I, unsigned *J, float *val, unsigned c, unsigned r)
{
	bcsr_t *bcsr;
   	tmp_block_t *block_list;

	block_list = mm_to_blocks(nz, I, J, val, c, r);
	bcsr = blocks_to_bcsr(block_list, c, r);

	print_bcsr(bcsr);

	return bcsr;
}

bcsr_t *mm_file_to_bcsr(char *filename, unsigned c, unsigned r)
{
	FILE *f;
	MM_typecode matcode;
	int ret_code;
	int M, N;
	int nz;   
	int i;
	unsigned *I, *J;
	float *val;

	bcsr_t *bcsr;

	if ((f = fopen(filename, "r")) == NULL) 
		exit(1);

	if (mm_read_banner(f, &matcode) != 0)
	{
		printf("Could not process Matrix Market banner.\n");
		exit(1);
	}

	/*  This is how one can screen matrix types if their application */
	/*  only supports a subset of the Matrix Market data types.      */
	
	if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&  mm_is_sparse(matcode) )
	{
		printf("Sorry, this application does not support ");
		printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
		exit(1);
	}
	
	/* find out size of sparse matrix .... */
	
	if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
		exit(1);
	
	
	/* reseve memory for matrices */
	
	I = malloc(nz * sizeof(unsigned));
	J = malloc(nz * sizeof(unsigned));
	/* XXX float ! */
	val = (float *) malloc(nz * sizeof(float));
	
	for (i=0; i<nz; i++)
	{
		fscanf(f, "%d %d %f\n", &I[i], &J[i], &val[i]);
		I[i]--;  /* adjust from 1-based to 0-based */
		J[i]--;
	}
	
	if (f !=stdin) fclose(f);
	
	bcsr = mm_to_bcsr((unsigned)nz, I, J, val, c, r);

	free(I);
	free(J);
	free(val);

	return bcsr;
}
