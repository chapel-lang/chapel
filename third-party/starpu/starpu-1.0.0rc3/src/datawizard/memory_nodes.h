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

#ifndef __MEMORY_NODES_H__
#define __MEMORY_NODES_H__

#include <starpu.h>
#include <common/config.h>
#include <datawizard/coherency.h>
#include <datawizard/memalloc.h>


#define _STARPU_MEMORY_NODE_TUPLE(node1,node2) (node1 | (node2 << 4))
#define _STARPU_MEMORY_NODE_TUPLE_FIRST(tuple) (tuple & 0x0F)
#define _STARPU_MEMORY_NODE_TUPLE_SECOND(tuple) (tuple & 0xF0)

struct _starpu_cond_and_mutex
{
        pthread_cond_t *cond;
        pthread_mutex_t *mutex;
};

struct _starpu_mem_node_descr
{
	unsigned nnodes;
	enum starpu_node_kind nodes[STARPU_MAXNODES];

	/* Get the device id associated to this node, or -1 if not applicable */
	int devid[STARPU_MAXNODES];

	unsigned nworkers[STARPU_MAXNODES];

	// TODO move this 2 lists outside struct _starpu_mem_node_descr
	/* Every worker is associated to a condition variable on which the
	 * worker waits when there is task available. It is possible that
	 * multiple worker share the same condition variable, so we maintain a
	 * list of all these condition variables so that we can wake up all
	 * worker attached to a memory node that are waiting on a task. */
	pthread_rwlock_t conditions_rwlock;
	struct _starpu_cond_and_mutex conditions_attached_to_node[STARPU_MAXNODES][STARPU_NMAXWORKERS];
	struct _starpu_cond_and_mutex conditions_all[STARPU_MAXNODES*STARPU_NMAXWORKERS];
	/* the number of queues attached to each node */
	unsigned total_condition_count;
	unsigned condition_count[STARPU_MAXNODES];

};

void _starpu_init_memory_nodes(void);
void _starpu_deinit_memory_nodes(void);
void _starpu_set_local_memory_node_key(unsigned *node);
unsigned _starpu_get_local_memory_node(void);
void _starpu_memory_node_worker_add(unsigned node);
unsigned _starpu_memory_node_workers(unsigned node);
unsigned _starpu_register_memory_node(enum starpu_node_kind kind, int devid);
//void _starpu_memory_node_attach_queue(struct starpu_jobq_s *q, unsigned nodeid);
void _starpu_memory_node_register_condition(pthread_cond_t *cond, pthread_mutex_t *mutex, unsigned memory_node);

enum starpu_node_kind _starpu_node_get_kind(uint32_t node);
int _starpu_memory_node_to_devid(unsigned node);

struct _starpu_mem_node_descr *_starpu_get_memory_node_description(void);

#endif // __MEMORY_NODES_H__
