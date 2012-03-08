/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#ifndef __STARPU_DATA_H__
#define __STARPU_DATA_H__

#include <starpu.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct _starpu_data_state;
typedef struct _starpu_data_state* starpu_data_handle_t;

enum starpu_access_mode
{
	STARPU_NONE=0,
	STARPU_R=(1<<0),
	STARPU_W=(1<<1),
	STARPU_RW=(STARPU_R|STARPU_W),
	STARPU_SCRATCH=(1<<2),
	STARPU_REDUX=(1<<3)
};

struct starpu_buffer_descr
{
	starpu_data_handle_t handle;
	enum starpu_access_mode mode;
};

struct starpu_data_interface_ops;

/* Destroy the data handle, in case we don't need to update the value of the
 * data in the home node, we can use starpu_data_unregister_no_coherency
 * instead. */
void starpu_data_unregister(starpu_data_handle_t handle);
void starpu_data_unregister_no_coherency(starpu_data_handle_t handle);

/* Destroy all data replicates. After data invalidation, the first access to
 * the handle must be performed in write-only mode. */
void starpu_data_invalidate(starpu_data_handle_t handle);

void starpu_data_advise_as_important(starpu_data_handle_t handle, unsigned is_important);

int starpu_data_acquire(starpu_data_handle_t handle, enum starpu_access_mode mode);
int starpu_data_acquire_cb(starpu_data_handle_t handle, enum starpu_access_mode mode, void (*callback)(void *), void *arg);
#ifdef __GCC__
#  define STARPU_DATA_ACQUIRE_CB(handle, mode, code) do \
	{ \						\
		void callback(void *arg)		\
		{					\
			code;				\
			starpu_data_release(handle);  	\
		}			      		\
		starpu_data_acquire_cb(handle, mode, callback, NULL);	\
	}						\
	while(0)
#endif

void starpu_data_release(starpu_data_handle_t handle);

int starpu_malloc(void **A, size_t dim);
int starpu_free(void *A);

/* XXX These macros are provided to avoid breaking old codes. But consider
 * these function names as deprecated. */
#define starpu_data_malloc_pinned_if_possible	starpu_malloc
#define starpu_data_free_pinned_if_possible	starpu_free

int starpu_data_request_allocation(starpu_data_handle_t handle, uint32_t node);

int starpu_data_prefetch_on_node(starpu_data_handle_t handle, unsigned node, unsigned async);


enum starpu_node_kind
{
	STARPU_UNUSED     = 0x00,
	STARPU_CPU_RAM    = 0x01,
	STARPU_CUDA_RAM   = 0x02,
	STARPU_OPENCL_RAM = 0x03,
	STARPU_SPU_LS     = 0x04
};

unsigned starpu_worker_get_memory_node(unsigned workerid);
unsigned starpu_memory_nodes_get_count(void);
enum starpu_node_kind starpu_node_get_kind(uint32_t node);


/* It is possible to associate a mask to a piece of data (and its children) so
 * that when it is modified, it is automatically transfered into those memory
 * node. For instance a (1<<0) write-through mask means that the CUDA workers will
 * commit their changes in main memory (node 0). */
void starpu_data_set_wt_mask(starpu_data_handle_t handle, uint32_t wt_mask);

void starpu_data_set_sequential_consistency_flag(starpu_data_handle_t handle, unsigned flag);
unsigned starpu_data_get_default_sequential_consistency_flag(void);
void starpu_data_set_default_sequential_consistency_flag(unsigned flag);

/* Query the status of the handle on the specified memory node. */
void starpu_data_query_status(starpu_data_handle_t handle, int memory_node, int *is_allocated, int *is_valid, int *is_requested);

struct starpu_codelet;

void starpu_data_set_reduction_methods(starpu_data_handle_t handle, struct starpu_codelet *redux_cl, struct starpu_codelet *init_cl);

int starpu_data_set_rank(starpu_data_handle_t handle, int rank);
int starpu_data_get_rank(starpu_data_handle_t handle);

int starpu_data_set_tag(starpu_data_handle_t handle, int tag);
int starpu_data_get_tag(starpu_data_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __STARPU_DATA_H__ */
