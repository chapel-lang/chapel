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

#include <datawizard/memalloc.h>
#include <datawizard/footprint.h>
#include <starpu_cuda.h>
#include <starpu_opencl.h>

/* This per-node RW-locks protect mc_list and memchunk_cache entries */
static pthread_rwlock_t mc_rwlock[STARPU_MAXNODES];

/* This per-node RW-locks protect lru_list */
static pthread_rwlock_t lru_rwlock[STARPU_MAXNODES];

/* Last Recently used memory chunkgs */
static struct _starpu_mem_chunk_lru_list *starpu_lru_list[STARPU_MAXNODES];

/* Potentially in use memory chunks */
static struct _starpu_mem_chunk_list *mc_list[STARPU_MAXNODES];

/* Explicitly caches memory chunks that can be reused */
static struct _starpu_mem_chunk_list *memchunk_cache[STARPU_MAXNODES];

/* When reclaiming memory to allocate, we reclaim MAX(what_is_to_reclaim_on_device, data_size_coefficient*data_size) */
const unsigned starpu_memstrategy_data_size_coefficient=2;

static void starpu_lru(unsigned node);

void _starpu_init_mem_chunk_lists(void)
{
	unsigned i;
	for (i = 0; i < STARPU_MAXNODES; i++)
	{
		_STARPU_PTHREAD_RWLOCK_INIT(&mc_rwlock[i], NULL);
		_STARPU_PTHREAD_RWLOCK_INIT(&lru_rwlock[i], NULL);
		mc_list[i] = _starpu_mem_chunk_list_new();
		starpu_lru_list[i] = _starpu_mem_chunk_lru_list_new();
		memchunk_cache[i] = _starpu_mem_chunk_list_new();
	}
}

void _starpu_deinit_mem_chunk_lists(void)
{
	unsigned i;
	for (i = 0; i < STARPU_MAXNODES; i++)
	{
		_starpu_mem_chunk_list_delete(mc_list[i]);
		_starpu_mem_chunk_list_delete(memchunk_cache[i]);
		_starpu_mem_chunk_lru_list_delete(starpu_lru_list[i]);
	}
}

/*
 *	Manipulate subtrees
 */

static void lock_all_subtree(starpu_data_handle_t handle)
{
	if (handle->nchildren == 0)
	{
		/* this is a leaf */
		while (_starpu_spin_trylock(&handle->header_lock))
			_starpu_datawizard_progress(_starpu_get_local_memory_node(), 0);
	}
	else
	{
		/* lock all sub-subtrees children */
		unsigned child;
		for (child = 0; child < handle->nchildren; child++)
		{
			lock_all_subtree(&handle->children[child]);
		}
	}
}

static void unlock_all_subtree(starpu_data_handle_t handle)
{
	if (handle->nchildren == 0)
	{
		/* this is a leaf */
		_starpu_spin_unlock(&handle->header_lock);
	}
	else
	{
		/* lock all sub-subtrees children
		 * Note that this is done in the reverse order of the
		 * lock_all_subtree so that we avoid deadlock */
		unsigned i;
		for (i =0; i < handle->nchildren; i++)
		{
			unsigned child = handle->nchildren - 1 - i;
			unlock_all_subtree(&handle->children[child]);
		}
	}
}

static unsigned may_free_subtree(starpu_data_handle_t handle, unsigned node)
{
	/* we only free if no one refers to the leaf */
	uint32_t refcnt = _starpu_get_data_refcnt(handle, node);
	if (refcnt)
		return 0;

	if (!handle->nchildren)
		return 1;

	/* look into all sub-subtrees children */
	unsigned child;
	for (child = 0; child < handle->nchildren; child++)
	{
		unsigned res;
		res = may_free_subtree(&handle->children[child], node);
		if (!res) return 0;
	}

	/* no problem was found */
	return 1;
}

static void transfer_subtree_to_node(starpu_data_handle_t handle, unsigned src_node,
				     unsigned dst_node)
{
	unsigned i;
	unsigned last = 0;
	unsigned cnt;
	int ret;

	if (handle->nchildren == 0)
	{
		struct _starpu_data_replicate *src_replicate = &handle->per_node[src_node];
		struct _starpu_data_replicate *dst_replicate = &handle->per_node[dst_node];

		/* this is a leaf */
		switch(src_replicate->state)
		{
		case STARPU_OWNER:
			/* the local node has the only copy */
			/* the owner is now the destination_node */
			src_replicate->state = STARPU_INVALID;
			dst_replicate->state = STARPU_OWNER;

#ifdef STARPU_DEVEL
#warning we should use requests during memory reclaim
#endif
			/* TODO use request !! */
			/* Take temporary references on the replicates */
			_starpu_spin_checklocked(&handle->header_lock);
			src_replicate->refcnt++;
			dst_replicate->refcnt++;
			handle->busy_count+=2;

			ret = _starpu_driver_copy_data_1_to_1(handle, src_replicate, dst_replicate, 0, NULL, 1);
			STARPU_ASSERT(ret == 0);

			src_replicate->refcnt--;
			dst_replicate->refcnt--;
			STARPU_ASSERT(handle->busy_count >= 2);
			handle->busy_count -= 2;
			_starpu_data_check_not_busy(handle);

			break;
		case STARPU_SHARED:
			/* some other node may have the copy */
			src_replicate->state = STARPU_INVALID;

			/* count the number of copies */
			cnt = 0;
			for (i = 0; i < STARPU_MAXNODES; i++)
			{
				if (handle->per_node[i].state == STARPU_SHARED)
				{
					cnt++;
					last = i;
				}
			}
			STARPU_ASSERT(cnt > 0);

			if (cnt == 1)
				handle->per_node[last].state = STARPU_OWNER;

			break;
		case STARPU_INVALID:
			/* nothing to be done */
			break;
		default:
			STARPU_ABORT();
			break;
		}
	}
	else
	{
		/* lock all sub-subtrees children */
		unsigned child;
		for (child = 0; child < handle->nchildren; child++)
		{
			transfer_subtree_to_node(&handle->children[child],
							src_node, dst_node);
		}
	}
}

static size_t free_memory_on_node(struct _starpu_mem_chunk *mc, uint32_t node)
{
	size_t freed = 0;

	STARPU_ASSERT(mc->ops);
	STARPU_ASSERT(mc->ops->free_data_on_node);

	starpu_data_handle_t handle = mc->data;

	/* Does this memory chunk refers to a handle that does not exist
	 * anymore ? */
	unsigned data_was_deleted = mc->data_was_deleted;

	struct _starpu_data_replicate *replicate = mc->replicate;

//	while (_starpu_spin_trylock(&handle->header_lock))
//		_starpu_datawizard_progress(_starpu_get_local_memory_node());

#ifdef STARPU_DEVEL
#warning can we block here ?
#endif
//	_starpu_spin_lock(&handle->header_lock);

	if (mc->automatically_allocated &&
		(!handle || data_was_deleted || replicate->refcnt == 0))
	{
		if (handle && !data_was_deleted)
			STARPU_ASSERT(replicate->allocated);

#if defined(STARPU_USE_CUDA) && defined(HAVE_CUDA_MEMCPY_PEER)
		if (starpu_node_get_kind(node) == STARPU_CUDA_RAM)
		{
			/* To facilitate the design of interface, we set the
			 * proper CUDA device in case it is needed. This avoids
			 * having to set it again in the free method of each
			 * interface. */
			cudaError_t err = cudaSetDevice(_starpu_memory_node_to_devid(node));
			STARPU_ASSERT(err == cudaSuccess);
		}
#endif

		mc->ops->free_data_on_node(mc->chunk_interface, node);

		if (handle && !data_was_deleted)
		{
			replicate->allocated = 0;

			/* XXX why do we need that ? */
			replicate->automatically_allocated = 0;
		}

		freed = mc->size;

		if (handle && !data_was_deleted)
			STARPU_ASSERT(replicate->refcnt == 0);
	}

//	_starpu_spin_unlock(&handle->header_lock);

	return freed;
}



static size_t do_free_mem_chunk(struct _starpu_mem_chunk *mc, unsigned node)
{
	size_t size;

	mc->replicate->mc=NULL;

	/* free the actual buffer */
	size = free_memory_on_node(mc, node);

	/* remove the mem_chunk from the list */
	_starpu_mem_chunk_list_erase(mc_list[node], mc);

	free(mc->chunk_interface);
	_starpu_mem_chunk_delete(mc);

	return size;
}

/* This function is called for memory chunks that are possibly in used (ie. not
 * in the cache). They should therefore still be associated to a handle. */
static size_t try_to_free_mem_chunk(struct _starpu_mem_chunk *mc, unsigned node)
{
	size_t freed = 0;

	starpu_data_handle_t handle;
	handle = mc->data;
	STARPU_ASSERT(handle);

	/* This data should be written through to this node, avoid dropping it! */
	if (handle->wt_mask & (1<<node))
		return 0;

	/* REDUX memchunk */
	if (mc->relaxed_coherency == 2)
	{
		/* TODO: reduce it back to e.g. main memory */
	}
	else
	/* Either it's a "relaxed coherency" memchunk (SCRATCH), or it's a
	 * memchunk that could be used with filters. */
	if (mc->relaxed_coherency == 1)
	{
		STARPU_ASSERT(mc->replicate);

		while (_starpu_spin_trylock(&handle->header_lock))
			_starpu_datawizard_progress(_starpu_get_local_memory_node(), 0);

		if (mc->replicate->refcnt == 0)
		{
			/* Note taht there is no need to transfer any data or
			 * to update the status in terms of MSI protocol
			 * because this memchunk is associated to a replicate
			 * in "relaxed coherency" mode. */
			freed = do_free_mem_chunk(mc, node);
		}

		_starpu_spin_unlock(&handle->header_lock);
	}
	else
	{
		/* try to lock all the leafs of the subtree */
		lock_all_subtree(handle);

		/* check if they are all "free" */
		if (may_free_subtree(handle, node))
		{
			STARPU_ASSERT(handle->per_node[node].refcnt == 0);

#ifdef STARPU_MEMORY_STATUS
			if (handle->per_node[node].state == STARPU_OWNER)
				_starpu_handle_stats_invalidated(handle, node);
			/* else XXX Considering only owner to invalidate */
#endif

			/* in case there was nobody using that buffer, throw it
			 * away after writing it back to main memory */
			transfer_subtree_to_node(handle, node, 0);

#ifdef STARPU_MEMORY_STATUS
			_starpu_handle_stats_loaded_owner(handle, 0);
#endif
			STARPU_ASSERT(handle->per_node[node].refcnt == 0);

			/* now the actual buffer may be freed */
			freed = do_free_mem_chunk(mc, node);
		}

		/* unlock the leafs */
		unlock_all_subtree(handle);
	}
	return freed;
}

#ifdef STARPU_USE_ALLOCATION_CACHE
/* We assume that mc_rwlock[node] is taken. is_already_in_mc_list indicates
 * that the mc is already in the list of buffers that are possibly used, and
 * therefore not in the cache. */
static void reuse_mem_chunk(unsigned node, struct _starpu_data_replicate *new_replicate, struct _starpu_mem_chunk *mc, unsigned is_already_in_mc_list)
{
	/* we found an appropriate mem chunk: so we get it out
	 * of the "to free" list, and reassign it to the new
	 * piece of data */

	if (!is_already_in_mc_list)
	{
		_starpu_mem_chunk_list_erase(memchunk_cache[node], mc);
	}

	struct _starpu_data_replicate *old_replicate = mc->replicate;
	old_replicate->allocated = 0;
	old_replicate->automatically_allocated = 0;
	old_replicate->initialized = 0;

	new_replicate->allocated = 1;
	new_replicate->automatically_allocated = 1;
	new_replicate->initialized = 0;

	STARPU_ASSERT(new_replicate->data_interface);
	STARPU_ASSERT(mc->chunk_interface);
	memcpy(new_replicate->data_interface, mc->chunk_interface, old_replicate->handle->ops->interface_size);

	mc->data = new_replicate->handle;
	mc->data_was_deleted = 0;
	/* mc->ops, mc->size, mc->footprint and mc->interface should be
 	 * unchanged ! */

	/* reinsert the mem chunk in the list of active memory chunks */
	if (!is_already_in_mc_list)
	{
		_starpu_mem_chunk_list_push_front(mc_list[node], mc);
	}
}

static unsigned try_to_reuse_mem_chunk(struct _starpu_mem_chunk *mc, unsigned node, struct _starpu_data_replicate *replicate, unsigned is_already_in_mc_list)
{
	unsigned success = 0;

	starpu_data_handle_t old_data;

	old_data = mc->data;

	STARPU_ASSERT(old_data);

	/* try to lock all the leafs of the subtree */
	lock_all_subtree(old_data);

	/* check if they are all "free" */
	if (may_free_subtree(old_data, node))
	{
		success = 1;

		/* in case there was nobody using that buffer, throw it
		 * away after writing it back to main memory */
		transfer_subtree_to_node(old_data, node, 0);

		/* now replace the previous data */
		reuse_mem_chunk(node, replicate, mc, is_already_in_mc_list);
	}

	/* unlock the leafs */
	unlock_all_subtree(old_data);

	return success;
}

static int _starpu_data_interface_compare(void *data_interface_a, struct starpu_data_interface_ops *ops_a,
                                          void *data_interface_b, struct starpu_data_interface_ops *ops_b)
{
	if (ops_a->interfaceid != ops_b->interfaceid)
		return -1;

	int ret = ops_a->compare(data_interface_a, data_interface_b);

	return ret;
}

/* This function must be called with mc_rwlock[node] taken in write mode */
static struct _starpu_mem_chunk *_starpu_memchunk_cache_lookup_locked(uint32_t node, starpu_data_handle_t handle)
{
	uint32_t footprint = _starpu_compute_data_footprint(handle);

	/* go through all buffers in the cache */
	struct _starpu_mem_chunk *mc;
	for (mc = _starpu_mem_chunk_list_begin(memchunk_cache[node]);
	     mc != _starpu_mem_chunk_list_end(memchunk_cache[node]);
	     mc = _starpu_mem_chunk_list_next(mc))
	{
		if (mc->footprint == footprint)
		{
			/* Is that a false hit ? (this is _very_ unlikely) */
			if (_starpu_data_interface_compare(handle->per_node[node].data_interface, handle->ops, mc->chunk_interface, mc->ops))
				continue;

			/* Cache hit */

			/* Remove from the cache */
			_starpu_mem_chunk_list_erase(memchunk_cache[node], mc);
			return mc;
		}
	}

	/* This is a cache miss */
	return NULL;
}

/* this function looks for a memory chunk that matches a given footprint in the
 * list of mem chunk that need to be freed. This function must be called with
 * mc_rwlock[node] taken in write mode. */
static unsigned try_to_find_reusable_mem_chunk(unsigned node, starpu_data_handle_t data, struct _starpu_data_replicate *replicate, uint32_t footprint)
{
	struct _starpu_mem_chunk *mc, *next_mc;

	/* go through all buffers in the cache */
	mc = _starpu_memchunk_cache_lookup_locked(node, data);
	if (mc)
	{
		/* We found an entry in the cache so we can reuse it */
		reuse_mem_chunk(node, replicate, mc, 0);
		return 1;
	}

	/* now look for some non essential data in the active list */
	for (mc = _starpu_mem_chunk_list_begin(mc_list[node]);
	     mc != _starpu_mem_chunk_list_end(mc_list[node]);
	     mc = next_mc)
	{
		/* there is a risk that the memory chunk is freed before next
		 * iteration starts: so we compute the next element of the list
		 * now */
		next_mc = _starpu_mem_chunk_list_next(mc);

		if (mc->data->is_not_important && (mc->footprint == footprint))
		{
//			fprintf(stderr, "found a candidate ...\n");
			if (try_to_reuse_mem_chunk(mc, node, replicate, 1))
				return 1;
		}
	}

	return 0;
}
#endif

/*
 * Free the memory chuncks that are explicitely tagged to be freed. The
 * mc_rwlock[node] rw-lock should be taken prior to calling this function.
 */
static size_t flush_memchunk_cache(uint32_t node, size_t reclaim)
{
	struct _starpu_mem_chunk *mc, *next_mc;

	size_t freed = 0;

	for (mc = _starpu_mem_chunk_list_begin(memchunk_cache[node]);
	     mc != _starpu_mem_chunk_list_end(memchunk_cache[node]);
	     mc = next_mc)
	{
		next_mc = _starpu_mem_chunk_list_next(mc);

		freed += free_memory_on_node(mc, node);

		_starpu_mem_chunk_list_erase(memchunk_cache[node], mc);

		free(mc->chunk_interface);
		_starpu_mem_chunk_delete(mc);
		if (reclaim && freed>reclaim)
			break;
	}

	return freed;
}

/*
 * Try to free the buffers currently in use on the memory node. If the force
 * flag is set, the memory is freed regardless of coherency concerns (this
 * should only be used at the termination of StarPU for instance). The
 * mc_rwlock[node] rw-lock should be taken prior to calling this function.
 */
static size_t free_potentially_in_use_mc(uint32_t node, unsigned force, size_t reclaim)
{
	size_t freed = 0;

	struct _starpu_mem_chunk *mc, *next_mc;

	for (mc = _starpu_mem_chunk_list_begin(mc_list[node]);
	     mc != _starpu_mem_chunk_list_end(mc_list[node]);
	     mc = next_mc)
	{
		/* there is a risk that the memory chunk is freed
		   before next iteration starts: so we compute the next
		   element of the list now */
		next_mc = _starpu_mem_chunk_list_next(mc);

		if (!force)
		{
			freed += try_to_free_mem_chunk(mc, node);
			#if 1
			if (reclaim && freed > reclaim)
				break;
			#endif
		}
		else
		{
			/* We must free the memory now: note that data
			 * coherency is not maintained in that case ! */
			freed += do_free_mem_chunk(mc, node);
		}
	}

	return freed;
}

static size_t reclaim_memory_generic(uint32_t node, unsigned force, size_t reclaim)
{
	size_t freed = 0;

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&mc_rwlock[node]);

	starpu_lru(node);

	/* remove all buffers for which there was a removal request */
	freed += flush_memchunk_cache(node, reclaim);

	/* try to free all allocated data potentially in use */
	if (reclaim && freed<reclaim)
		freed += free_potentially_in_use_mc(node, force, reclaim);

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[node]);

	return freed;

}

/*
 * This function frees all the memory that was implicitely allocated by StarPU
 * (for the data replicates). This is not ensuring data coherency, and should
 * only be called while StarPU is getting shut down.
 */
size_t _starpu_free_all_automatically_allocated_buffers(uint32_t node)
{
	return reclaim_memory_generic(node, 1, 0);
}

static struct _starpu_mem_chunk *_starpu_memchunk_init(struct _starpu_data_replicate *replicate, size_t size, size_t interface_size, unsigned automatically_allocated)
{
	struct _starpu_mem_chunk *mc = _starpu_mem_chunk_new();
	starpu_data_handle_t handle = replicate->handle;

	STARPU_ASSERT(handle);
	STARPU_ASSERT(handle->ops);

	mc->data = handle;
	mc->size = size;
	mc->footprint = _starpu_compute_data_footprint(handle);
	mc->ops = handle->ops;
	mc->data_was_deleted = 0;
	mc->automatically_allocated = automatically_allocated;
	mc->relaxed_coherency = replicate->relaxed_coherency;
	mc->replicate = replicate;
	mc->replicate->mc = mc;

	/* Save a copy of the interface */
	mc->chunk_interface = malloc(interface_size);
	STARPU_ASSERT(mc->chunk_interface);
	memcpy(mc->chunk_interface, replicate->data_interface, interface_size);

	return mc;
}

static void register_mem_chunk(struct _starpu_data_replicate *replicate, size_t size, unsigned automatically_allocated)
{
	unsigned dst_node = replicate->memory_node;

	struct _starpu_mem_chunk *mc;

	/* the interface was already filled by ops->allocate_data_on_node */
	size_t interface_size = replicate->handle->ops->interface_size;

	/* Put this memchunk in the list of memchunk in use */
	mc = _starpu_memchunk_init(replicate, size, interface_size, automatically_allocated);

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&mc_rwlock[dst_node]);

	_starpu_mem_chunk_list_push_back(mc_list[dst_node], mc);

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[dst_node]);
}

/* This function is called when the handle is destroyed (eg. when calling
 * unregister or unpartition). It puts all the memchunks that refer to the
 * specified handle into the cache. */
void _starpu_request_mem_chunk_removal(starpu_data_handle_t handle, unsigned node)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&mc_rwlock[node]);

	/* iterate over the list of memory chunks and remove the entry */
	struct _starpu_mem_chunk *mc, *next_mc;
	for (mc = _starpu_mem_chunk_list_begin(mc_list[node]);
	     mc != _starpu_mem_chunk_list_end(mc_list[node]);
	     mc = next_mc)
	{
		next_mc = _starpu_mem_chunk_list_next(mc);

		if (mc->data == handle)
		{
			/* we found the data */
			mc->data_was_deleted = 1;

			/* remove it from the main list */
			_starpu_mem_chunk_list_erase(mc_list[node], mc);

			/* put it in the list of buffers to be removed */
			_starpu_mem_chunk_list_push_front(memchunk_cache[node], mc);

			/* Note that we do not stop here because there can be
			 * multiple replicates associated to the same handle on
			 * the same memory node.  */
		}
	}

	/* there was no corresponding buffer ... */
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[node]);
}

static size_t _starpu_get_global_mem_size(int dst_node)
{
	enum starpu_node_kind kind = starpu_node_get_kind(dst_node);
	size_t global_mem_size;

	switch(kind)
	{
		case STARPU_CPU_RAM:
		{
			/* We should probably never get here : if there is no
 			 * space left in RAM, the operating system should swap
			 * to disk for us. */
			STARPU_ABORT();
		}
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_RAM:
		{
			int devid = _starpu_memory_node_to_devid(dst_node);
			global_mem_size = starpu_cuda_get_global_mem_size(devid);
			break;
		}
#endif
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_RAM:
		{
			int devid = _starpu_memory_node_to_devid(dst_node);
			global_mem_size = starpu_opencl_get_global_mem_size(devid);
			break;
		}
#endif
		default:
			STARPU_ABORT();
	}
	return global_mem_size;
}

/*
 * In order to allocate a piece of data, we try to reuse existing buffers if
 * its possible.
 *	1 - we try to reuse a memchunk that is explicitely unused.
 *	2 - we go through the list of memory chunks and find one that is not
 *	referenced and that has the same footprint to reuse it.
 *	3 - we call the usual driver's alloc method
 *	4 - we go through the list of memory chunks and release those that are
 *	not referenced (or part of those).
 *
 */

static ssize_t _starpu_allocate_interface(starpu_data_handle_t handle, struct _starpu_data_replicate *replicate, uint32_t dst_node, unsigned is_prefetch)
{
	unsigned attempts = 0;
	ssize_t allocated_memory;

	_starpu_spin_checklocked(&handle->header_lock);

	_starpu_data_allocation_inc_stats(dst_node);

#ifdef STARPU_USE_ALLOCATION_CACHE
	/* perhaps we can directly reuse a buffer in the free-list */
	uint32_t footprint = _starpu_compute_data_footprint(handle);

	_STARPU_TRACE_START_ALLOC_REUSE(dst_node);
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&mc_rwlock[dst_node]);

	if (try_to_find_reusable_mem_chunk(dst_node, handle, replicate, footprint))
	{
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[dst_node]);
		_starpu_allocation_cache_hit(dst_node);
		ssize_t data_size = _starpu_data_get_size(handle);
		return data_size;
	}

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[dst_node]);
	_STARPU_TRACE_END_ALLOC_REUSE(dst_node);
#endif

	do
	{
		STARPU_ASSERT(handle->ops);
		STARPU_ASSERT(handle->ops->allocate_data_on_node);

		_STARPU_TRACE_START_ALLOC(dst_node);
		STARPU_ASSERT(replicate->data_interface);

#if defined(STARPU_USE_CUDA) && defined(HAVE_CUDA_MEMCPY_PEER)
		if (starpu_node_get_kind(dst_node) == STARPU_CUDA_RAM)
		{
			/* To facilitate the design of interface, we set the
			 * proper CUDA device in case it is needed. This avoids
			 * having to set it again in the malloc method of each
			 * interface. */
			cudaError_t err = cudaSetDevice(_starpu_memory_node_to_devid(dst_node));
			STARPU_ASSERT(err == cudaSuccess);
		}
#endif

		allocated_memory = handle->ops->allocate_data_on_node(replicate->data_interface, dst_node);
		_STARPU_TRACE_END_ALLOC(dst_node);

		if (allocated_memory == -ENOMEM)
		{
			size_t reclaim = 0.25*_starpu_get_global_mem_size(dst_node);
			if (starpu_memstrategy_data_size_coefficient*handle->data_size > reclaim)
				reclaim = starpu_memstrategy_data_size_coefficient*handle->data_size;

			/* Take temporary reference on the replicate */
			replicate->refcnt++;
			handle->busy_count++;
			_starpu_spin_unlock(&handle->header_lock);

			_STARPU_TRACE_START_MEMRECLAIM(dst_node);
			if (is_prefetch) {
				_STARPU_PTHREAD_RWLOCK_WRLOCK(&mc_rwlock[dst_node]);
				flush_memchunk_cache(dst_node, reclaim);
				_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[dst_node]);
			} else
				reclaim_memory_generic(dst_node, 0, reclaim);
			_STARPU_TRACE_END_MEMRECLAIM(dst_node);

		        while (_starpu_spin_trylock(&handle->header_lock))
		                _starpu_datawizard_progress(_starpu_get_local_memory_node(), 0);

			replicate->refcnt--;
			STARPU_ASSERT(replicate->refcnt >= 0);
			STARPU_ASSERT(handle->busy_count > 0);
			handle->busy_count--;
			_starpu_data_check_not_busy(handle);
		}

	}
	while((allocated_memory == -ENOMEM) && attempts++ < 2);

	return allocated_memory;
}

int _starpu_allocate_memory_on_node(starpu_data_handle_t handle, struct _starpu_data_replicate *replicate, unsigned is_prefetch)
{
	ssize_t allocated_memory;

	unsigned dst_node = replicate->memory_node;

	STARPU_ASSERT(handle);

	/* A buffer is already allocated on the node */
	if (replicate->allocated)
		return 0;

	STARPU_ASSERT(replicate->data_interface);
	allocated_memory = _starpu_allocate_interface(handle, replicate, dst_node, is_prefetch);

	/* perhaps we could really not handle that capacity misses */
	if (allocated_memory == -ENOMEM)
		return -ENOMEM;

	register_mem_chunk(replicate, allocated_memory, 1);

	replicate->allocated = 1;
	replicate->automatically_allocated = 1;

	if (dst_node == 0)
	{
		void *ptr = starpu_handle_to_pointer(handle, 0);
		if (ptr != NULL)
		{
			_starpu_data_register_ram_pointer(handle, ptr);
		}
	}

	return 0;
}

unsigned starpu_data_test_if_allocated_on_node(starpu_data_handle_t handle, uint32_t memory_node)
{
	return handle->per_node[memory_node].allocated;
}

void _starpu_memchunk_recently_used(struct _starpu_mem_chunk *mc, unsigned node)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&lru_rwlock[node]);
	struct _starpu_mem_chunk_lru *mc_lru=_starpu_mem_chunk_lru_new();
	mc_lru->mc=mc;
	_starpu_mem_chunk_lru_list_push_front(starpu_lru_list[node],mc_lru);
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&lru_rwlock[node]);
}

/* The mc_rwlock[node] rw-lock should be taken prior to calling this function.*/
static void _starpu_memchunk_recently_used_move(struct _starpu_mem_chunk *mc, unsigned node)
{
	/* XXX Sometimes the memchunk is not in the list... */
	struct _starpu_mem_chunk *mc_iter;
	for (mc_iter = _starpu_mem_chunk_list_begin(mc_list[node]);
	     mc_iter != _starpu_mem_chunk_list_end(mc_list[node]);
	     mc_iter = _starpu_mem_chunk_list_next(mc_iter) )
	{
		if (mc_iter==mc)
		{
			_starpu_mem_chunk_list_erase(mc_list[node], mc);
			_starpu_mem_chunk_list_push_back(mc_list[node], mc);
			return;
		}

	}
}

static void starpu_lru(unsigned node)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&lru_rwlock[node]);
	while (!_starpu_mem_chunk_lru_list_empty(starpu_lru_list[node]))
	{
		struct _starpu_mem_chunk_lru *mc_lru=_starpu_mem_chunk_lru_list_front(starpu_lru_list[node]);
		_starpu_memchunk_recently_used_move(mc_lru->mc, node);
		_starpu_mem_chunk_lru_list_erase(starpu_lru_list[node], mc_lru);
		_starpu_mem_chunk_lru_delete(mc_lru);
	}
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&lru_rwlock[node]);
}


#ifdef STARPU_MEMORY_STATUS
void _starpu_display_data_stats_by_node(int node)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&mc_rwlock[node]);

	if (!_starpu_mem_chunk_list_empty(mc_list[node]))
	{
		fprintf(stderr, "#-------\n");
		fprintf(stderr, "Data on Node #%d\n",node);

		struct _starpu_mem_chunk *mc;

		for (mc = _starpu_mem_chunk_list_begin(mc_list[node]);
		     mc != _starpu_mem_chunk_list_end(mc_list[node]);
		     mc = _starpu_mem_chunk_list_next(mc))
		{
			_starpu_display_data_handle_stats(mc->data);
		}

	}

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&mc_rwlock[node]);
}
#endif
