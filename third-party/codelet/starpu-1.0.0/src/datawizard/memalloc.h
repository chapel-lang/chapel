/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
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

#ifndef __MEMALLOC_H__
#define __MEMALLOC_H__

#include <starpu.h>
#include <common/config.h>

#include <common/list.h>
#include <datawizard/interfaces/data_interface.h>
#include <datawizard/coherency.h>
#include <datawizard/copy_driver.h>

struct _starpu_data_replicate;

LIST_TYPE(_starpu_mem_chunk,
	starpu_data_handle_t data;
	size_t size;

	uint32_t footprint;

	/* The footprint of the data is not sufficient to determine whether two
	 * pieces of data have the same layout (there could be collision in the
	 * hash function ...) so we still keep a copy of the actual layout (ie.
	 * the data interface) to stay on the safe side. We make a copy of
	 * because when a data is deleted, the memory chunk remains.
	 */
	struct starpu_data_interface_ops *ops;
	void *chunk_interface;
	unsigned automatically_allocated;
	unsigned data_was_deleted;

	/* A buffer that is used for SCRATCH or reduction cannnot be used with
	 * filters. */
	unsigned relaxed_coherency;
	struct _starpu_data_replicate *replicate;
)

/* LRU list */
LIST_TYPE(_starpu_mem_chunk_lru,
	struct _starpu_mem_chunk *mc;
)

void _starpu_init_mem_chunk_lists(void);
void _starpu_deinit_mem_chunk_lists(void);
void _starpu_request_mem_chunk_removal(starpu_data_handle_t handle, unsigned node);
int _starpu_allocate_memory_on_node(starpu_data_handle_t handle, struct _starpu_data_replicate *replicate, unsigned is_prefetch);
size_t _starpu_free_all_automatically_allocated_buffers(uint32_t node);
void _starpu_memchunk_recently_used(struct _starpu_mem_chunk *mc, unsigned node);

void _starpu_display_data_stats_by_node(int node);
#endif
