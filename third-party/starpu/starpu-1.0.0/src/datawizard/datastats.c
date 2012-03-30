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

#include <starpu.h>
#include <datawizard/datastats.h>
#include <datawizard/coherency.h>
#include <common/config.h>

#ifdef STARPU_DATA_STATS
/* measure the cache hit ratio for each node */
static unsigned hit_cnt[STARPU_MAXNODES];
static unsigned miss_cnt[STARPU_MAXNODES];
#endif

void _starpu_msi_cache_hit(unsigned node __attribute__ ((unused)))
{
#ifdef STARPU_DATA_STATS
	hit_cnt[node]++;
#endif
}

void _starpu_msi_cache_miss(unsigned node __attribute__ ((unused)))
{
#ifdef STARPU_DATA_STATS
	miss_cnt[node]++;
#endif
}

void _starpu_display_msi_stats(void)
{
#ifdef STARPU_DATA_STATS
	unsigned node;
	unsigned total_hit_cnt = 0;
	unsigned total_miss_cnt = 0;

	fprintf(stderr, "MSI cache stats :\n");

	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		total_hit_cnt += hit_cnt[node];
		total_miss_cnt += miss_cnt[node];
	}

	fprintf(stderr, "TOTAL MSI stats\thit %u (%2.2f \%%)\tmiss %u (%2.2f \%%)\n", total_hit_cnt, (100.0f*total_hit_cnt)/(total_hit_cnt+total_miss_cnt), total_miss_cnt, (100.0f*total_miss_cnt)/(total_hit_cnt+total_miss_cnt));

	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		if (hit_cnt[node]+miss_cnt[node])
		{
			fprintf(stderr, "memory node %d\n", node);
			fprintf(stderr, "\thit : %u (%2.2f \%%)\n", hit_cnt[node], (100.0f*hit_cnt[node])/(hit_cnt[node]+miss_cnt[node]));
			fprintf(stderr, "\tmiss : %u (%2.2f \%%)\n", miss_cnt[node], (100.0f*miss_cnt[node])/(hit_cnt[node]+miss_cnt[node]));
		}
	}
#endif
}

/* measure the efficiency of our allocation cache */

#ifdef STARPU_DATA_STATS
static unsigned alloc_cnt[STARPU_MAXNODES];
static unsigned alloc_cache_hit_cnt[STARPU_MAXNODES];
#endif

void _starpu_allocation_cache_hit(unsigned node __attribute__ ((unused)))
{
#ifdef STARPU_DATA_STATS
	alloc_cache_hit_cnt[node]++;
#endif
}

void _starpu_data_allocation_inc_stats(unsigned node __attribute__ ((unused)))
{
#ifdef STARPU_DATA_STATS
	alloc_cnt[node]++;
#endif
}

void _starpu_display_alloc_cache_stats(void)
{
#ifdef STARPU_DATA_STATS
	fprintf(stderr, "Allocation cache stats:\n");
	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		if (alloc_cnt[node])
		{
			fprintf(stderr, "memory node %d\n", node);
			fprintf(stderr, "\ttotal alloc : %u\n", alloc_cnt[node]);
			fprintf(stderr, "\tcached alloc: %u (%2.2f \%%)\n",
				alloc_cache_hit_cnt[node], (100.0f*alloc_cache_hit_cnt[node])/(alloc_cnt[node]));
		}
	}
#endif
}

/* measure the amount of data transfers between each pair of nodes */
#ifdef STARPU_DATA_STATS
static size_t comm_amount[STARPU_MAXNODES][STARPU_MAXNODES];
#endif /* STARPU_DATA_STATS */

void _starpu_comm_amounts_inc(unsigned src  __attribute__ ((unused)), unsigned dst  __attribute__ ((unused)), size_t size  __attribute__ ((unused)))
{
#ifdef STARPU_DATA_STATS
	comm_amount[src][dst] += size;
#endif /* STARPU_DATA_STATS */
}

void _starpu_display_comm_amounts(void)
{
#ifdef STARPU_DATA_STATS
	unsigned src, dst;

	size_t sum = 0;

	for (dst = 0; dst < STARPU_MAXNODES; dst++)
		for (src = 0; src < STARPU_MAXNODES; src++)
		{
			sum += comm_amount[src][dst];
			sum += comm_amount[dst][src];
		}

	fprintf(stderr, "\nData transfers stats:\nTOTAL transfers %f MB\n", (float)sum/1024/1024);

	for (dst = 0; dst < STARPU_MAXNODES; dst++)
		for (src = dst + 1; src < STARPU_MAXNODES; src++)
		{
			if (comm_amount[src][dst])
				fprintf(stderr, "\t%d <-> %d\t%f MB\n\t\t%d -> %d\t%f MB\n\t\t%d -> %d\t%f MB\n",
					src, dst, ((float)comm_amount[src][dst] + (float)comm_amount[dst][src])/(1024*1024),
					src, dst, ((float)comm_amount[src][dst])/(1024*1024),
					dst, src, ((float)comm_amount[dst][src])/(1024*1024));
		}
#endif
}

#ifdef STARPU_MEMORY_STATUS
void _starpu_display_data_stats(void)
{
	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		_starpu_display_data_stats_by_node(node);
	}
}

void _starpu_display_data_handle_stats(starpu_data_handle_t handle)
{
	unsigned node;

	fprintf(stderr, "#-----\n");
	fprintf(stderr, "Data : %p\n", handle);
	fprintf(stderr, "Size : %d\n", (int)handle->data_size);
	fprintf(stderr, "\n");

	fprintf(stderr, "#--\n");
	fprintf(stderr, "Data access stats\n");
	fprintf(stderr, "/!\\ Work Underway\n");
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		if (handle->stats_direct_access[node]+handle->stats_loaded_shared[node]
		    +handle->stats_invalidated[node]+handle->stats_loaded_owner[node])
		{
			fprintf(stderr, "Node #%d\n", node);
			fprintf(stderr, "\tDirect access : %d\n", handle->stats_direct_access[node]);
			/* XXX Not Working yet. */
			if (handle->stats_shared_to_owner[node])
				fprintf(stderr, "\t\tShared to Owner : %d\n", handle->stats_shared_to_owner[node]);
			fprintf(stderr, "\tLoaded (Owner) : %d\n", handle->stats_loaded_owner[node]);
			fprintf(stderr, "\tLoaded (Shared) : %d\n", handle->stats_loaded_shared[node]);
			fprintf(stderr, "\tInvalidated (was Owner) : %d\n\n", handle->stats_invalidated[node]);
		}
	}
}

void _starpu_handle_stats_cache_hit(starpu_data_handle_t handle, unsigned node)
{
	handle->stats_direct_access[node]++;
}

void _starpu_handle_stats_loaded_shared(starpu_data_handle_t handle, unsigned node)
{
	handle->stats_loaded_shared[node]++;
}

void _starpu_handle_stats_loaded_owner(starpu_data_handle_t handle, unsigned node)
{
	handle->stats_loaded_owner[node]++;
}

void _starpu_handle_stats_shared_to_owner(starpu_data_handle_t handle, unsigned node)
{
	handle->stats_shared_to_owner[node]++;
}

void _starpu_handle_stats_invalidated(starpu_data_handle_t handle, unsigned node)
{
	handle->stats_invalidated[node]++;
}

#endif
