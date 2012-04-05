/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010, 2011-2012  Université de Bordeaux 1
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

#include <stdint.h>

#include <datawizard/datawizard.h>
#include <core/dependencies/data_concurrency.h>
#include <common/uthash.h>
#include <common/starpu_spinlock.h>
#include <core/task.h>

/* Entry in the `registered_handles' hash table.  */
struct handle_entry
{
	UT_hash_handle hh;
	void *pointer;
	starpu_data_handle_t handle;
};

/* Hash table mapping host pointers to data handles.  */
static struct handle_entry *registered_handles;
static struct _starpu_spinlock    registered_handles_lock;
static int _data_interface_number = STARPU_MAX_INTERFACE_ID;

void _starpu_data_interface_init()
{
	_starpu_spin_init(&registered_handles_lock);
}

void _starpu_data_interface_shutdown()
{
	struct handle_entry *entry, *tmp;

	_starpu_spin_destroy(&registered_handles_lock);

	HASH_ITER(hh, registered_handles, entry, tmp)
	{
		HASH_DEL(registered_handles, entry);
		free(entry);
	}

	registered_handles = NULL;
}

/* Register the mapping from PTR to HANDLE.  If PTR is already mapped to
 * some handle, the new mapping shadows the previous one.   */
void _starpu_data_register_ram_pointer(starpu_data_handle_t handle, void *ptr)
{
	struct handle_entry *entry;

	entry = (struct handle_entry *) malloc(sizeof(*entry));
	STARPU_ASSERT(entry != NULL);

	entry->pointer = ptr;
	entry->handle = handle;

	_starpu_spin_lock(&registered_handles_lock);
	HASH_ADD_PTR(registered_handles, pointer, entry);
	_starpu_spin_unlock(&registered_handles_lock);
}

starpu_data_handle_t starpu_data_lookup(const void *ptr)
{
	starpu_data_handle_t result;

	_starpu_spin_lock(&registered_handles_lock);
	{
		struct handle_entry *entry;

		HASH_FIND_PTR(registered_handles, &ptr, entry);
		if(STARPU_UNLIKELY(entry == NULL))
			result = NULL;
		else
			result = entry->handle;
	}
	_starpu_spin_unlock(&registered_handles_lock);

	return result;
}

int
_starpu_data_is_multiformat_handle(starpu_data_handle_t handle)
{
	return handle->ops->is_multiformat;
}
/*
 * Start monitoring a piece of data
 */

static void _starpu_register_new_data(starpu_data_handle_t handle,
					uint32_t home_node, uint32_t wt_mask)
{
	void *ptr;

	STARPU_ASSERT(handle);

	/* initialize the new lock */
	handle->req_list = _starpu_data_requester_list_new();
	handle->refcnt = 0;
	handle->busy_count = 0;
	handle->busy_waiting = 0;
	_STARPU_PTHREAD_MUTEX_INIT(&handle->busy_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&handle->busy_cond, NULL);
	_starpu_spin_init(&handle->header_lock);

	/* first take care to properly lock the data */
	_starpu_spin_lock(&handle->header_lock);

	/* there is no hierarchy yet */
	handle->nchildren = 0;
	handle->root_handle = handle;
	handle->father_handle = NULL;
	handle->sibling_index = 0; /* could be anything for the root */
	handle->depth = 1; /* the tree is just a node yet */
        handle->rank = -1; /* invalid until set */
	handle->tag = -1; /* invalid until set */

	handle->is_not_important = 0;

	handle->sequential_consistency =
		starpu_data_get_default_sequential_consistency_flag();

	_STARPU_PTHREAD_MUTEX_INIT(&handle->sequential_consistency_mutex, NULL);
	handle->last_submitted_mode = STARPU_R;
	handle->last_submitted_writer = NULL;
	handle->last_submitted_readers = NULL;
	handle->post_sync_tasks = NULL;
	handle->post_sync_tasks_cnt = 0;

	/* By default, there are no methods available to perform a reduction */
	handle->redux_cl = NULL;
	handle->init_cl = NULL;

	handle->reduction_refcnt = 0;
	handle->reduction_req_list = _starpu_data_requester_list_new();

#ifdef STARPU_USE_FXT
	handle->last_submitted_ghost_writer_id_is_valid = 0;
	handle->last_submitted_ghost_writer_id = 0;
	handle->last_submitted_ghost_readers_id = NULL;
#endif

	handle->wt_mask = wt_mask;

	/* Store some values directly in the handle not to recompute them all
	 * the time. */
	STARPU_ASSERT(handle->ops->get_size);
	handle->data_size = handle->ops->get_size(handle);
	handle->footprint = _starpu_compute_data_footprint(handle);

	handle->home_node = home_node;

	/* that new data is invalid from all nodes perpective except for the
	 * home node */
	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_node[node];

		replicate->memory_node = node;
		replicate->relaxed_coherency = 0;
		replicate->refcnt = 0;

		if (node == home_node)
		{
			/* this is the home node with the only valid copy */
			replicate->state = STARPU_OWNER;
			replicate->allocated = 1;
			replicate->automatically_allocated = 0;
		}
		else
		{
			/* the value is not available here yet */
			replicate->state = STARPU_INVALID;
			replicate->allocated = 0;
		}
	}

	unsigned worker;
	unsigned nworkers = starpu_worker_get_count();
	for (worker = 0; worker < nworkers; worker++)
	{
		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_worker[worker];
		replicate->allocated = 0;
		replicate->automatically_allocated = 0;
		replicate->state = STARPU_INVALID;
		replicate->refcnt = 0;
		replicate->handle = handle;

		for (node = 0; node < STARPU_MAXNODES; node++)
		{
			replicate->requested[node] = 0;
			replicate->request[node] = NULL;
		}

		/* Assuming being used for SCRATCH for now, patched when entering REDUX mode */
		replicate->relaxed_coherency = 1;
		replicate->initialized = 0;
		replicate->memory_node = starpu_worker_get_memory_node(worker);

		/* duplicate  the content of the interface on node 0 */
		memcpy(replicate->data_interface, handle->per_node[0].data_interface, handle->ops->interface_size);
	}

	/* now the data is available ! */
	_starpu_spin_unlock(&handle->header_lock);

	ptr = starpu_handle_to_pointer(handle, 0);
	if (ptr != NULL)
	{
		_starpu_data_register_ram_pointer(handle, ptr);
	}
}

static starpu_data_handle_t _starpu_data_handle_allocate(struct starpu_data_interface_ops *interface_ops)
{
	starpu_data_handle_t handle = (starpu_data_handle_t) calloc(1, sizeof(struct _starpu_data_state));

	STARPU_ASSERT(handle);

	handle->ops = interface_ops;

	size_t interfacesize = interface_ops->interface_size;

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
#ifdef STARPU_MEMORY_STATUS
		/* Stats initilization */
		handle->stats_direct_access[node]=0;
		handle->stats_loaded_shared[node]=0;
		handle->stats_shared_to_owner[node]=0;
		handle->stats_loaded_owner[node]=0;
		handle->stats_invalidated[node]=0;
#endif

		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_node[node];
		/* relaxed_coherency = 0 */

		replicate->handle = handle;

		replicate->data_interface = calloc(1, interfacesize);
		STARPU_ASSERT(replicate->data_interface);
	}

	unsigned worker;
	unsigned nworkers = starpu_worker_get_count();
	for (worker = 0; worker < nworkers; worker++)
	{
		struct _starpu_data_replicate *replicate;
		replicate = &handle->per_worker[worker];

		replicate->handle = handle;

		replicate->data_interface = calloc(1, interfacesize);
		STARPU_ASSERT(replicate->data_interface);

	}

	return handle;
}

void starpu_data_register(starpu_data_handle_t *handleptr, uint32_t home_node,
				void *data_interface,
				struct starpu_data_interface_ops *ops)
{
	starpu_data_handle_t handle =
		_starpu_data_handle_allocate(ops);

	STARPU_ASSERT(handleptr);
	*handleptr = handle;
	handle->mf_node = home_node;

	int asynchronous_copy_disabled = starpu_asynchronous_copy_disabled();
	if (STARPU_UNLIKELY(asynchronous_copy_disabled))
	{
#ifdef STARPU_USE_CUDA
		ops->copy_methods->ram_to_cuda_async = NULL;
		ops->copy_methods->cuda_to_ram_async = NULL;
		ops->copy_methods->cuda_to_cuda_async = NULL;
#endif
#ifdef STARPU_USE_OPENCL
		ops->copy_methods->ram_to_opencl_async = NULL;
		ops->copy_methods->opencl_to_ram_async = NULL;
		ops->copy_methods->opencl_to_opencl_async = NULL;
#endif
	}

	/* fill the interface fields with the appropriate method */
	STARPU_ASSERT(ops->register_data_handle);
	ops->register_data_handle(handle, home_node, data_interface);

	_starpu_register_new_data(handle, home_node, 0);
}

void starpu_data_register_same(starpu_data_handle_t *handledst, starpu_data_handle_t handlesrc)
{
	void *local_interface = starpu_data_get_interface_on_node(handlesrc, 0);
	starpu_data_register(handledst, -1, local_interface, handlesrc->ops);
}

void *starpu_handle_to_pointer(starpu_data_handle_t handle, uint32_t node)
{
	/* Check whether the operation is supported and the node has actually
	 * been allocated.  */
	if (handle->ops->handle_to_pointer
	    && starpu_data_test_if_allocated_on_node(handle, node))
	{
		return handle->ops->handle_to_pointer(handle, node);
	}

	return NULL;
}

void *starpu_handle_get_local_ptr(starpu_data_handle_t handle)
{
	return starpu_handle_to_pointer(handle,
					_starpu_get_local_memory_node());
}

int starpu_data_get_rank(starpu_data_handle_t handle)
{
	return handle->rank;
}

int starpu_data_set_rank(starpu_data_handle_t handle, int rank)
{
        handle->rank = rank;
        return 0;
}

int starpu_data_get_tag(starpu_data_handle_t handle)
{
	return handle->tag;
}

int starpu_data_set_tag(starpu_data_handle_t handle, int tag)
{
        handle->tag = tag;
        return 0;
}

/*
 * Stop monitoring a piece of data
 */

void _starpu_data_free_interfaces(starpu_data_handle_t handle)
{
	const void *ram_ptr;
	unsigned node;
	unsigned worker;
	unsigned nworkers = starpu_worker_get_count();

	ram_ptr = starpu_handle_to_pointer(handle, 0);

	for (node = 0; node < STARPU_MAXNODES; node++)
		free(handle->per_node[node].data_interface);

	for (worker = 0; worker < nworkers; worker++)
		free(handle->per_worker[worker].data_interface);

	if (ram_ptr != NULL)
	{
		/* Remove the PTR -> HANDLE mapping.  If a mapping from PTR
		 * to another handle existed before (e.g., when using
		 * filters), it becomes visible again.  */
		struct handle_entry *entry;

		_starpu_spin_lock(&registered_handles_lock);
		HASH_FIND_PTR(registered_handles, &ram_ptr, entry);
		STARPU_ASSERT(entry != NULL);

		HASH_DEL(registered_handles, entry);
		free(entry);

		_starpu_spin_unlock(&registered_handles_lock);
	}
}

struct _starpu_unregister_callback_arg
{
	unsigned memory_node;
	starpu_data_handle_t handle;
	unsigned terminated;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

/* Check whether we should tell starpu_data_unregister that the data handle is
 * not busy any more.
 * The header is supposed to be locked */
void _starpu_data_check_not_busy(starpu_data_handle_t handle)
{
	if (!handle->busy_count && handle->busy_waiting)
	{
		_STARPU_PTHREAD_MUTEX_LOCK(&handle->busy_mutex);
		_STARPU_PTHREAD_COND_BROADCAST(&handle->busy_cond);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&handle->busy_mutex);
	}
}

static void _starpu_data_unregister_fetch_data_callback(void *_arg)
{
	int ret;
	struct _starpu_unregister_callback_arg *arg = (struct _starpu_unregister_callback_arg *) _arg;

	starpu_data_handle_t handle = arg->handle;

	STARPU_ASSERT(handle);

	struct _starpu_data_replicate *replicate = &handle->per_node[arg->memory_node];

	ret = _starpu_fetch_data_on_node(handle, replicate, STARPU_R, 0, 0, NULL, NULL);
	STARPU_ASSERT(!ret);

	/* unlock the caller */
	_STARPU_PTHREAD_MUTEX_LOCK(&arg->mutex);
	arg->terminated = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&arg->cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&arg->mutex);
}

/* Unregister the data handle, perhaps we don't need to update the home_node
 * (in that case coherent is set to 0) */
static void _starpu_data_unregister(starpu_data_handle_t handle, unsigned coherent)
{
	STARPU_ASSERT(handle);
	STARPU_ASSERT_MSG(handle->nchildren == 0, "data needs to be unpartitioned before unregistration");

	if (coherent)
	{
		/* If sequential consistency is enabled, wait until data is available */
		_starpu_data_wait_until_available(handle, STARPU_RW);

		/* Fetch data in the home of the data to ensure we have a valid copy
		 * where we registered it */
		int home_node = handle->home_node;
		if (home_node >= 0)
		{
			struct _starpu_unregister_callback_arg arg;
			arg.handle = handle;
			arg.memory_node = (unsigned)home_node;
			arg.terminated = 0;
			_STARPU_PTHREAD_MUTEX_INIT(&arg.mutex, NULL);
			_STARPU_PTHREAD_COND_INIT(&arg.cond, NULL);

			if (!_starpu_attempt_to_submit_data_request_from_apps(handle, STARPU_R,
					_starpu_data_unregister_fetch_data_callback, &arg))
			{
				/* no one has locked this data yet, so we proceed immediately */
				struct _starpu_data_replicate *home_replicate = &handle->per_node[home_node];
				int ret = _starpu_fetch_data_on_node(handle, home_replicate, STARPU_R, 0, 0, NULL, NULL);
				STARPU_ASSERT(!ret);
			}
			else
			{
				_STARPU_PTHREAD_MUTEX_LOCK(&arg.mutex);
				while (!arg.terminated)
					_STARPU_PTHREAD_COND_WAIT(&arg.cond, &arg.mutex);
				_STARPU_PTHREAD_MUTEX_UNLOCK(&arg.mutex);
			}
			_starpu_release_data_on_node(handle, 0, &handle->per_node[home_node]);
		}

		/* If this handle uses a multiformat interface, we may have to convert
		 * this piece of data back into the CPU format.
		 * XXX : This is quite hacky, could we submit a task instead ?
		 */
		if (_starpu_data_is_multiformat_handle(handle) &&
			starpu_node_get_kind(handle->mf_node) != STARPU_CPU_RAM)
		{
			_STARPU_DEBUG("Conversion needed\n");
			void *buffers[1];
			struct starpu_multiformat_interface *format_interface;
			format_interface = (struct starpu_multiformat_interface *) starpu_data_get_interface_on_node(handle, 0);
			struct starpu_codelet *cl = NULL;
			enum starpu_node_kind node_kind = starpu_node_get_kind(handle->mf_node);

			struct starpu_multiformat_data_interface_ops *mf_ops;
			mf_ops = (struct starpu_multiformat_data_interface_ops *) handle->ops->get_mf_ops(format_interface);
			switch (node_kind)
			{
#ifdef STARPU_USE_CUDA
				case STARPU_CUDA_RAM:
					cl = mf_ops->cuda_to_cpu_cl;
					break;
#endif
#ifdef STARPU_USE_OPENCL
				case STARPU_OPENCL_RAM:
					cl = mf_ops->opencl_to_cpu_cl;
					break;
#endif
				case STARPU_CPU_RAM:      /* Impossible ! */
				case STARPU_SPU_LS:       /* Not supported */
				default:
					STARPU_ASSERT(0);
			}
			buffers[0] = format_interface;

			_starpu_cl_func_t func = _starpu_task_get_cpu_nth_implementation(cl, 0);
			STARPU_ASSERT(func);
			func(buffers, NULL);
		}
	}
	else
	{
		/* Should we postpone the unregister operation ? */
		if ((handle->refcnt > 0) && handle->lazy_unregister)
			return;
	}

	_starpu_spin_lock(&handle->header_lock);
	/* Tell holders of references that we're starting waiting */
	handle->busy_waiting = 1;
	_starpu_spin_unlock(&handle->header_lock);

	/* Wait for all requests to finish (notably WT requests) */
	_STARPU_PTHREAD_MUTEX_LOCK(&handle->busy_mutex);
	while (handle->busy_count)
		_STARPU_PTHREAD_COND_WAIT(&handle->busy_cond, &handle->busy_mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&handle->busy_mutex);

	/* Wait for finished requests to release the handle */
	_starpu_spin_lock(&handle->header_lock);
	_starpu_data_free_interfaces(handle);

	/* Destroy the data now */
	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		/* free the data copy in a lazy fashion */
		_starpu_request_mem_chunk_removal(handle, node);
	}

	_starpu_data_requester_list_delete(handle->req_list);
	_starpu_data_requester_list_delete(handle->reduction_req_list);

	_starpu_spin_unlock(&handle->header_lock);

	free(handle);
}

void starpu_data_unregister(starpu_data_handle_t handle)
{
	_starpu_data_unregister(handle, 1);
}

void starpu_data_unregister_no_coherency(starpu_data_handle_t handle)
{
	_starpu_data_unregister(handle, 0);
}

void starpu_data_invalidate(starpu_data_handle_t handle)
{
	STARPU_ASSERT(handle);

	starpu_data_acquire(handle, STARPU_W);

	_starpu_spin_lock(&handle->header_lock);

	unsigned node;
	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		struct _starpu_data_replicate *local = &handle->per_node[node];

		if (local->allocated && local->automatically_allocated)
		{
			/* free the data copy in a lazy fashion */
			_starpu_request_mem_chunk_removal(handle, node);
		}

		local->state = STARPU_INVALID;
	}

	_starpu_spin_unlock(&handle->header_lock);

	starpu_data_release(handle);
}

enum starpu_data_interface_id starpu_handle_get_interface_id(starpu_data_handle_t handle)
{
	return handle->ops->interfaceid;
}

void *starpu_data_get_interface_on_node(starpu_data_handle_t handle, unsigned memory_node)
{
	return handle->per_node[memory_node].data_interface;
}

int starpu_data_interface_get_next_id()
{
	_data_interface_number += 1;
	return _data_interface_number-1;
}
