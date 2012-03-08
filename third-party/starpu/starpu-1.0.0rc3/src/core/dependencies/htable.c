/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

#include <core/dependencies/htable.h>
#include <string.h>

void *_starpu_htbl_search_tag(struct _starpu_htbl_node *htbl, starpu_tag_t tag)
{
	unsigned currentbit;
	struct _starpu_htbl_node *current_htbl = htbl;

	/* 000000000001111 with _STARPU_HTBL_NODE_SIZE 1's */
	starpu_tag_t mask = (1<<_STARPU_HTBL_NODE_SIZE)-1;

	for(currentbit = 0; currentbit < _STARPU_TAG_SIZE; currentbit+=_STARPU_HTBL_NODE_SIZE)
	{
	//	printf("search : current bit = %d \n", currentbit);
		if (STARPU_UNLIKELY(current_htbl == NULL))
			return NULL;

		/* 0000000000001111
		 *     | currentbit
		 * 0000111100000000 = offloaded_mask
		 *         |last_currentbit
		 * */

		unsigned last_currentbit =
			_STARPU_TAG_SIZE - (currentbit + _STARPU_HTBL_NODE_SIZE);
		starpu_tag_t offloaded_mask = mask << last_currentbit;
		unsigned current_index =
			(tag & (offloaded_mask)) >> (last_currentbit);

		current_htbl = current_htbl->children[current_index];
	}

	return current_htbl;
}

/*
 * returns the previous value of the tag, or NULL else
 */

void *_starpu_htbl_insert_tag(struct _starpu_htbl_node **htbl, starpu_tag_t tag, void *entry)
{
	unsigned currentbit;
	struct _starpu_htbl_node **current_htbl_ptr = htbl;
	struct _starpu_htbl_node *previous_htbl_ptr = NULL;

	/* 000000000001111 with _STARPU_HTBL_NODE_SIZE 1's */
	starpu_tag_t mask = (1<<_STARPU_HTBL_NODE_SIZE)-1;

	for(currentbit = 0; currentbit < _STARPU_TAG_SIZE; currentbit+=_STARPU_HTBL_NODE_SIZE)
	{
		if (*current_htbl_ptr == NULL)
		{
			/* TODO pad to change that 1 into 16 ? */
			*current_htbl_ptr = (struct _starpu_htbl_node *) calloc(1, sizeof(struct _starpu_htbl_node));
			STARPU_ASSERT(*current_htbl_ptr);

			if (previous_htbl_ptr)
				previous_htbl_ptr->nentries++;
		}

		/* 0000000000001111
		 *     | currentbit
		 * 0000111100000000 = offloaded_mask
		 *         |last_currentbit
		 * */

		unsigned last_currentbit =
			_STARPU_TAG_SIZE - (currentbit + _STARPU_HTBL_NODE_SIZE);
		starpu_tag_t offloaded_mask = mask << last_currentbit;
		unsigned current_index =
			(tag & (offloaded_mask)) >> (last_currentbit);

		previous_htbl_ptr = *current_htbl_ptr;
		current_htbl_ptr =
			&((*current_htbl_ptr)->children[current_index]);
	}

	/* current_htbl either contains NULL or a previous entry
	 * we overwrite it anyway */
	void *old_entry = *current_htbl_ptr;
	*current_htbl_ptr = (struct _starpu_htbl_node *) entry;

	if (!old_entry)
		previous_htbl_ptr->nentries++;

	return old_entry;
}

/* returns the entry corresponding to the tag and remove it from the htbl */
void *_starpu_htbl_remove_tag(struct _starpu_htbl_node **htbl, starpu_tag_t tag)
{
	/* NB : if the entry is "NULL", we assume this means it is not present XXX */
	unsigned currentbit;
	struct _starpu_htbl_node **current_htbl_ptr_parent = htbl;
	struct _starpu_htbl_node *current_htbl_ptr = *current_htbl_ptr_parent;

	/* remember the path to the tag */
	struct _starpu_htbl_node *path[(_STARPU_TAG_SIZE + _STARPU_HTBL_NODE_SIZE - 1)/(_STARPU_HTBL_NODE_SIZE)];
	struct _starpu_htbl_node **path_parent[(_STARPU_TAG_SIZE + _STARPU_HTBL_NODE_SIZE - 1)/(_STARPU_HTBL_NODE_SIZE)];

	/* 000000000001111 with _STARPU_HTBL_NODE_SIZE 1's */
	starpu_tag_t mask = (1<<_STARPU_HTBL_NODE_SIZE)-1;
	int level, maxlevel;
	unsigned tag_is_present = 1;

	for(currentbit = 0, level = 0; currentbit < _STARPU_TAG_SIZE; currentbit+=_STARPU_HTBL_NODE_SIZE, level++)
	{
		path_parent[level] = current_htbl_ptr_parent;
		path[level] = current_htbl_ptr;

		if (STARPU_UNLIKELY(!current_htbl_ptr))
		{
			tag_is_present = 0;
			break;
		}

		/* 0000000000001111
		 *     | currentbit
		 * 0000111100000000 = offloaded_mask
		 *         |last_currentbit
		 * */

		unsigned last_currentbit =
			_STARPU_TAG_SIZE - (currentbit + _STARPU_HTBL_NODE_SIZE);
		starpu_tag_t offloaded_mask = mask << last_currentbit;
		unsigned current_index =
			(tag & (offloaded_mask)) >> (last_currentbit);

		current_htbl_ptr_parent = 
			&current_htbl_ptr->children[current_index];
		current_htbl_ptr = *current_htbl_ptr_parent;
	}

	maxlevel = level;
	if (STARPU_UNLIKELY(!current_htbl_ptr))
		tag_is_present = 0;

	void *old_entry = current_htbl_ptr;

	if (tag_is_present)
	{
		/* the tag was in the htbl, so we have to unroll the search
 		 * to remove possibly useless htbl (internal) nodes */
		for (level = maxlevel - 1; level >= 0; level--)
		{
			path[level]->nentries--;

			/* TODO use likely statements ... */

			/* in case we do not remove that node, we do decrease its parents
 			 * number of entries */
			if (path[level]->nentries > 0)
				break;

			/* we remove this node */
			free(path[level]);
			*(path_parent[level]) = NULL;
		}
	}

	/* we return the entry if there was one */
	return old_entry;
}

void _starpu_htbl_clear_tags(struct _starpu_htbl_node **htbl, unsigned level, void (*free_entry)(void *))
{
	unsigned i;
	struct _starpu_htbl_node *tbl = *htbl;

	if (!tbl)
		return;

	if (level * _STARPU_HTBL_NODE_SIZE < _STARPU_TAG_SIZE) {
		for (i = 0; i < 1<<_STARPU_HTBL_NODE_SIZE; i++)
			_starpu_htbl_clear_tags(&tbl->children[i], level + 1, free_entry);
		free(tbl);
	} else
		free_entry(tbl);
	*htbl = NULL;
}
