/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
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

#ifndef __HTABLE_H__
#define __HTABLE_H__

/*
 *	Define a hierarchical table to do the tag matching
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <core/dependencies/tags.h>

#define _STARPU_HTBL_NODE_SIZE	16

struct _starpu_htbl_node
{
	unsigned nentries;
	struct _starpu_htbl_node *children[1<<_STARPU_HTBL_NODE_SIZE];
};

void *_starpu_htbl_search_tag(struct _starpu_htbl_node *htbl, starpu_tag_t tag);
void *_starpu_htbl_insert_tag(struct _starpu_htbl_node **htbl, starpu_tag_t tag, void *entry);
void *_starpu_htbl_remove_tag(struct _starpu_htbl_node **htbl, starpu_tag_t tag);
void _starpu_htbl_clear_tags(struct _starpu_htbl_node **htbl, unsigned level, void (*free_entry)(void*));


#endif
