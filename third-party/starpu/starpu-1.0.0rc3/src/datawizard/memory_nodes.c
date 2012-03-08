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

#include <pthread.h>
#include <common/config.h>
#include <core/sched_policy.h>
#include <datawizard/datastats.h>
#include <common/fxt.h>
#include "copy_driver.h"
#include "memalloc.h"

static struct _starpu_mem_node_descr descr;
static pthread_key_t memory_node_key;

void _starpu_init_memory_nodes(void)
{
	/* there is no node yet, subsequent nodes will be
	 * added using _starpu_register_memory_node */
	descr.nnodes = 0;

	pthread_key_create(&memory_node_key, NULL);

	unsigned i;
	for (i = 0; i < STARPU_MAXNODES; i++)
	{
		descr.nodes[i] = STARPU_UNUSED;
		descr.nworkers[i] = 0;
	}

	_starpu_init_mem_chunk_lists();
	_starpu_init_data_request_lists();

	pthread_rwlock_init(&descr.conditions_rwlock, NULL);
	descr.total_condition_count = 0;
}

void _starpu_deinit_memory_nodes(void)
{
	_starpu_deinit_data_request_lists();
	_starpu_deinit_mem_chunk_lists();

	pthread_key_delete(memory_node_key);
}

void _starpu_set_local_memory_node_key(unsigned *node)
{
	pthread_setspecific(memory_node_key, node);
}

unsigned _starpu_get_local_memory_node(void)
{
	unsigned *memory_node;
	memory_node = (unsigned *) pthread_getspecific(memory_node_key);

	/* in case this is called by the programmer, we assume the RAM node
	   is the appropriate memory node ... so we return 0 XXX */
	if (STARPU_UNLIKELY(!memory_node))
		return 0;

	return *memory_node;
}

void _starpu_memory_node_worker_add(unsigned node)
{
	descr.nworkers[node]++;
}

unsigned _starpu_memory_node_workers(unsigned node)
{
	return descr.nworkers[node];
}

struct _starpu_mem_node_descr *_starpu_get_memory_node_description(void)
{
	return &descr;
}

enum starpu_node_kind starpu_node_get_kind(uint32_t node)
{
	return descr.nodes[node];
}

int _starpu_memory_node_to_devid(unsigned node)
{
	return descr.devid[node];
}

unsigned starpu_memory_nodes_get_count(void)
{
	return descr.nnodes;
}

unsigned _starpu_register_memory_node(enum starpu_node_kind kind, int devid)
{
	unsigned nnodes;
	/* ATOMIC_ADD returns the new value ... */
	nnodes = STARPU_ATOMIC_ADD(&descr.nnodes, 1);

	descr.nodes[nnodes-1] = kind;
	_STARPU_TRACE_NEW_MEM_NODE(nnodes-1);

	descr.devid[nnodes-1] = devid;

	/* for now, there is no condition associated to that newly created node */
	descr.condition_count[nnodes-1] = 0;

	return (nnodes-1);
}

/* TODO move in a more appropriate file  !! */
/* Register a condition variable associated to worker which is associated to a
 * memory node itself. */
void _starpu_memory_node_register_condition(pthread_cond_t *cond, pthread_mutex_t *mutex, unsigned nodeid)
{
	unsigned cond_id;
	unsigned nconds_total, nconds;

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&descr.conditions_rwlock);

	/* we only insert the queue if it's not already in the list */
	nconds = descr.condition_count[nodeid];
	for (cond_id = 0; cond_id < nconds; cond_id++)
	{
		if (descr.conditions_attached_to_node[nodeid][cond_id].cond == cond)
		{
			STARPU_ASSERT(descr.conditions_attached_to_node[nodeid][cond_id].mutex == mutex);

			/* the condition is already in the list */
			_STARPU_PTHREAD_RWLOCK_UNLOCK(&descr.conditions_rwlock);
			return;
		}
	}

	/* it was not found locally */
	descr.conditions_attached_to_node[nodeid][cond_id].cond = cond;
	descr.conditions_attached_to_node[nodeid][cond_id].mutex = mutex;
	descr.condition_count[nodeid]++;

	/* do we have to add it in the global list as well ? */
	nconds_total = descr.total_condition_count;
	for (cond_id = 0; cond_id < nconds_total; cond_id++)
	{
		if (descr.conditions_all[cond_id].cond == cond)
		{
			/* the queue is already in the global list */
			_STARPU_PTHREAD_RWLOCK_UNLOCK(&descr.conditions_rwlock);
			return;
		}
	}

	/* it was not in the global list either */
	descr.conditions_all[nconds_total].cond = cond;
	descr.conditions_all[nconds_total].mutex = mutex;
	descr.total_condition_count++;

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&descr.conditions_rwlock);
}

unsigned starpu_worker_get_memory_node(unsigned workerid)
{
	struct _starpu_machine_config *config = _starpu_get_machine_config();

	/* This workerid may either be a basic worker or a combined worker */
	unsigned nworkers = config->topology.nworkers;

	if (workerid < config->topology.nworkers)
		return config->workers[workerid].memory_node;

	/* We have a combined worker */
	unsigned ncombinedworkers = config->topology.ncombinedworkers;
	STARPU_ASSERT(workerid < ncombinedworkers + nworkers);
	return config->combined_workers[workerid - nworkers].memory_node;

}
