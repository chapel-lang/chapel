/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
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

#include <common/config.h>
#include <datawizard/coherency.h>
#include <datawizard/copy_driver.h>
#include <datawizard/write_back.h>
#include <core/dependencies/data_concurrency.h>
#include <profiling/profiling.h>
#include <math.h>

static int link_supports_direct_transfers(starpu_data_handle_t handle, unsigned src_node, unsigned dst_node, unsigned *handling_node);
uint32_t _starpu_select_src_node(starpu_data_handle_t handle, unsigned destination)
{
	int src_node = -1;
	unsigned i;

	unsigned nnodes = starpu_memory_nodes_get_count();

	/* first find a valid copy, either a STARPU_OWNER or a STARPU_SHARED */
	uint32_t node;

	uint32_t src_node_mask = 0;
	size_t size = _starpu_data_get_size(handle);
	double cost = INFINITY;

	for (node = 0; node < nnodes; node++)
	{
		if (handle->per_node[node].state != STARPU_INVALID)
		{
			/* we found a copy ! */
			src_node_mask |= (1<<node);
		}
	}

	/* we should have found at least one copy ! */
	STARPU_ASSERT(src_node_mask != 0);

	/* Without knowing the size, we won't know the cost */
	if (!size)
		cost = 0;

	/* Check whether we have transfer cost for all nodes, if so, take the minimum */
	if (cost)
		for (i = 0; i < nnodes; i++)
		{
			if (src_node_mask & (1<<i))
			{
				double time = _starpu_predict_transfer_time(i, destination, size);
				unsigned handling_node;

				/* Avoid indirect transfers */
				if (!link_supports_direct_transfers(handle, i, destination, &handling_node))
					continue;

				if (_STARPU_IS_ZERO(time))
				{
					/* No estimation, will have to revert to dumb strategy */
					cost = 0.0;
					break;
				}
				else if (time < cost)
				{
					cost = time;
					src_node = i;
				}
			}
		}

	if (cost && src_node != -1)
		/* Could estimate through cost, return that */
		return src_node;

	/* Revert to dumb strategy: take RAM unless only a GPU has it */
	for (i = 0; i < nnodes; i++)
	{
		if (src_node_mask & (1<<i))
		{
			/* this is a potential candidate */
			src_node = i;

			/* however GPU are expensive sources, really !
			 * 	Unless peer transfer is supported.
			 * 	Other should be ok */

			if (
#ifndef HAVE_CUDA_MEMCPY_PEER
					starpu_node_get_kind(i) != STARPU_CUDA_RAM &&
#endif
					starpu_node_get_kind(i) != STARPU_OPENCL_RAM)
				break ;
		}
	}

	STARPU_ASSERT(src_node != -1);

	return src_node;
}

/* this may be called once the data is fetched with header and STARPU_RW-lock hold */
void _starpu_update_data_state(starpu_data_handle_t handle,
			       struct _starpu_data_replicate *requesting_replicate,
			       enum starpu_access_mode mode)
{
	/* There is nothing to do for relaxed coherency modes (scratch or
	 * reductions) */
	if (!(mode & STARPU_RW))
		return;

	unsigned nnodes = starpu_memory_nodes_get_count();

	/* the data is present now */
	unsigned requesting_node = requesting_replicate->memory_node;
	requesting_replicate->requested[requesting_node] = 0;

	if (mode & STARPU_W)
	{
		/* the requesting node now has the only valid copy */
		uint32_t node;
		for (node = 0; node < nnodes; node++)
			handle->per_node[node].state = STARPU_INVALID;

		requesting_replicate->state = STARPU_OWNER;
	}
	else
	{ /* read only */
		if (requesting_replicate->state != STARPU_OWNER)
		{
			/* there was at least another copy of the data */
			uint32_t node;
			for (node = 0; node < nnodes; node++)
			{
				struct _starpu_data_replicate *replicate = &handle->per_node[node];
				if (replicate->state != STARPU_INVALID)
					replicate->state = STARPU_SHARED;
			}
			requesting_replicate->state = STARPU_SHARED;
		}
	}
}

static int worker_supports_direct_access(unsigned node, unsigned handling_node)
{
	if (node == handling_node)
		return 1;

	if (!_starpu_memory_node_workers(handling_node))
		/* No worker to process the request from that node */
		return 0;

	int type = starpu_node_get_kind(node);
	switch (type)
	{
		case STARPU_CUDA_RAM:
#ifdef HAVE_CUDA_MEMCPY_PEER
			/* GPUs not always allow direct remote access: if CUDA4
			 * is enabled, we allow two CUDA devices to communicate. */
			return (starpu_node_get_kind(handling_node) != STARPU_OPENCL_RAM);
#else
			/* Direct GPU-GPU transfers are not allowed in general */
			return 0;
#endif
		case STARPU_OPENCL_RAM:
			return 0;
		default:
			return 1;
	}
}

static int link_supports_direct_transfers(starpu_data_handle_t handle, unsigned src_node, unsigned dst_node, unsigned *handling_node)
{
	(void) handle; // unused

	/* XXX That's a hack until we get cudaMemcpy3DPeerAsync to work !
	 * Perhaps not all data interface provide a direct GPU-GPU transfer
	 * method ! */
#ifdef STARPU_USE_CUDA
	if (src_node != dst_node && starpu_node_get_kind(src_node) == STARPU_CUDA_RAM && starpu_node_get_kind(dst_node) == STARPU_CUDA_RAM)
	{
		const struct starpu_data_copy_methods *copy_methods = handle->ops->copy_methods;
		if (!copy_methods->cuda_to_cuda_async)
			return 0;
	}
#endif

	if (worker_supports_direct_access(src_node, dst_node))
	{
		*handling_node = dst_node;
		return 1;
	}

	if (worker_supports_direct_access(dst_node, src_node))
	{
		*handling_node = src_node;
		return 1;
	}

	return 0;
}

/* Determines the path of a request : each hop is defined by (src,dst) and the
 * node that handles the hop. The returned value indicates the number of hops,
 * and the max_len is the maximum number of hops (ie. the size of the
 * src_nodes, dst_nodes and handling_nodes arrays. */
static int determine_request_path(starpu_data_handle_t handle,
				  unsigned src_node, unsigned dst_node,
				  enum starpu_access_mode mode, int max_len,
				  unsigned *src_nodes, unsigned *dst_nodes,
				  unsigned *handling_nodes)
{
	if (!(mode & STARPU_R))
	{
		/* The destination node should only allocate the data, no transfer is required */
		STARPU_ASSERT(max_len >= 1);
		src_nodes[0] = 0; // ignored
		dst_nodes[0] = dst_node;
		handling_nodes[0] = dst_node;
		return 1;
	}

	unsigned handling_node;
	int link_is_valid = link_supports_direct_transfers(handle, src_node, dst_node, &handling_node);

	if (!link_is_valid)
	{
		/* We need an intermediate hop to implement data staging
		 * through main memory. */
		STARPU_ASSERT(max_len >= 2);

		/* XXX we hardcode 0 as the RAM node ... */

		/* GPU -> RAM */
		src_nodes[0] = src_node;
		dst_nodes[0] = 0;
		handling_nodes[0] = src_node;

		/* RAM -> GPU */
		src_nodes[1] = 0;
		dst_nodes[1] = dst_node;
		handling_nodes[1] = dst_node;

		return 2;
	}
	else
	{
		STARPU_ASSERT(max_len >= 1);

		src_nodes[0] = src_node;
		dst_nodes[0] = dst_node;
		handling_nodes[0] = handling_node;

#ifndef HAVE_CUDA_MEMCPY_PEER
		STARPU_ASSERT(!(mode & STARPU_R) || starpu_node_get_kind(src_node) != STARPU_CUDA_RAM || starpu_node_get_kind(dst_node) != STARPU_CUDA_RAM);
#endif

		return 1;
	}
}

/* handle->lock should be taken. r is returned locked. The node parameter
 * indicate either the source of the request, or the destination for a
 * write-only request. */
static struct _starpu_data_request *_starpu_search_existing_data_request(struct _starpu_data_replicate *replicate, unsigned node, enum starpu_access_mode mode, unsigned is_prefetch)
{
	struct _starpu_data_request *r;

	r = replicate->request[node];

	if (r)
	{
		_starpu_spin_checklocked(&r->handle->header_lock);

		_starpu_spin_lock(&r->lock);

                /* perhaps we need to "upgrade" the request */
		if (is_prefetch < r->prefetch)
			_starpu_update_prefetch_status(r);

		if (mode & STARPU_R)
		{
			/* in case the exisiting request did not imply a memory
			 * transfer yet, we have to take a second refcnt now
			 * for the source, in addition to the refcnt for the
			 * destination
			 * (so that the source remains valid) */
			if (!(r->mode & STARPU_R))
			{
				replicate->refcnt++;
				replicate->handle->busy_count++;
			}

			r->mode = (enum starpu_access_mode) ((int) r->mode | (int) STARPU_R);
		}

		if (mode & STARPU_W)
			r->mode = (enum starpu_access_mode) ((int) r->mode | (int)  STARPU_W);
	}

	return r;
}



/*
 * This function is called when the data is needed on the local node, this
 * returns a pointer to the local copy
 *
 *			R 	STARPU_W 	STARPU_RW
 *	Owner		OK	OK	OK
 *	Shared		OK	1	1
 *	Invalid		2	3	4
 *
 * case 1 : shared + (read)write :
 * 	no data copy but shared->Invalid/Owner
 * case 2 : invalid + read :
 * 	data copy + invalid->shared + owner->shared (STARPU_ASSERT(there is a valid))
 * case 3 : invalid + write :
 * 	no data copy + invalid->owner + (owner,shared)->invalid
 * case 4 : invalid + R/STARPU_W :
 * 	data copy + if (STARPU_W) (invalid->owner + owner->invalid)
 * 		    else (invalid,owner->shared)
 */

struct _starpu_data_request *_starpu_create_request_to_fetch_data(starpu_data_handle_t handle,
								  struct _starpu_data_replicate *dst_replicate,
								  enum starpu_access_mode mode, unsigned is_prefetch,
								  unsigned async,
								  void (*callback_func)(void *), void *callback_arg)
{
	/* This function is called with handle's header lock taken */
	_starpu_spin_checklocked(&handle->header_lock);

	unsigned requesting_node = dst_replicate->memory_node;

	if (dst_replicate->state != STARPU_INVALID)
	{
#ifdef STARPU_MEMORY_STATUS
		enum _starpu_cache_state old_state = dst_replicate->state;
#endif
		/* the data is already available so we can stop */
		_starpu_update_data_state(handle, dst_replicate, mode);
		_starpu_msi_cache_hit(requesting_node);

#ifdef STARPU_MEMORY_STATUS
		_starpu_handle_stats_cache_hit(handle, requesting_node);

		/* XXX Broken ? */
		if (old_state == STARPU_SHARED
		    && dst_replicate->state == STARPU_OWNER)
			_starpu_handle_stats_shared_to_owner(handle, requesting_node);
#endif

		_starpu_memchunk_recently_used(dst_replicate->mc, requesting_node);

		_starpu_spin_unlock(&handle->header_lock);

		if (callback_func)
			callback_func(callback_arg);

                _STARPU_LOG_OUT_TAG("data available");
		return NULL;
	}

	_starpu_msi_cache_miss(requesting_node);

	/* the only remaining situation is that the local copy was invalid */
	STARPU_ASSERT(dst_replicate->state == STARPU_INVALID);

	/* find someone who already has the data */
	uint32_t src_node = 0;

	/* if the data is in write only mode, there is no need for a source */
	if (mode & STARPU_R)
	{
		src_node = _starpu_select_src_node(handle, requesting_node);
		STARPU_ASSERT(src_node != requesting_node);
	}

	/* We can safely assume that there won't be more than 2 hops in the
	 * current implementation */
	unsigned src_nodes[4], dst_nodes[4], handling_nodes[4];
	int nhops = determine_request_path(handle, src_node, requesting_node, mode, 4,
					src_nodes, dst_nodes, handling_nodes);

	STARPU_ASSERT(nhops >= 1 && nhops <= 4);
	struct _starpu_data_request *requests[nhops];

	/* Did we reuse a request for that hop ? */
	int reused_requests[nhops];

	/* Construct an array with a list of requests, possibly reusing existing requests */
	int hop;
	for (hop = 0; hop < nhops; hop++)
	{
		struct _starpu_data_request *r;

		unsigned hop_src_node = src_nodes[hop];
		unsigned hop_dst_node = dst_nodes[hop];
		unsigned hop_handling_node = handling_nodes[hop];

		struct _starpu_data_replicate *hop_src_replicate;
		struct _starpu_data_replicate *hop_dst_replicate;

		/* Only the first request is independant */
		unsigned ndeps = (hop == 0)?0:1;

		hop_src_replicate = &handle->per_node[hop_src_node];
		hop_dst_replicate = (hop != nhops - 1)?&handle->per_node[hop_dst_node]:dst_replicate;

		/* Try to reuse a request if possible */
		r = _starpu_search_existing_data_request(hop_dst_replicate,
				(mode & STARPU_R)?hop_src_node:hop_dst_node,
							 mode, is_prefetch);

		reused_requests[hop] = !!r;

		if (!r)
		{
			/* Create a new request if there was no request to reuse */
			r = _starpu_create_data_request(handle, hop_src_replicate,
							hop_dst_replicate, hop_handling_node,
							mode, ndeps, is_prefetch);
		}

		requests[hop] = r;
	}

	/* Chain these requests */
	for (hop = 0; hop < nhops; hop++)
	{
		struct _starpu_data_request *r;
		r = requests[hop];

		if (hop != nhops - 1)
		{
			if (!reused_requests[hop + 1]) {
				r->next_req[r->next_req_count++] = requests[hop + 1];
				STARPU_ASSERT(r->next_req_count <= STARPU_MAXNODES);
			}
		}
		else
			_starpu_data_request_append_callback(r, callback_func, callback_arg);


		if (reused_requests[hop])
			_starpu_spin_unlock(&r->lock);
	}

	if (!async)
		requests[nhops - 1]->refcnt++;


	/* we only submit the first request, the remaining will be
	 * automatically submitted afterward */
	if (!reused_requests[0])
		_starpu_post_data_request(requests[0], handling_nodes[0]);

	return requests[nhops - 1];
}

int _starpu_fetch_data_on_node(starpu_data_handle_t handle, struct _starpu_data_replicate *dst_replicate,
			       enum starpu_access_mode mode, unsigned detached, unsigned async,
			       void (*callback_func)(void *), void *callback_arg)
{
	uint32_t local_node = _starpu_get_local_memory_node();
        _STARPU_LOG_IN();

	while (_starpu_spin_trylock(&handle->header_lock))
		_starpu_datawizard_progress(local_node, 1);

	if (!detached)
	{
		/* Take a reference which will be released by _starpu_release_data_on_node */
		dst_replicate->refcnt++;
		dst_replicate->handle->busy_count++;
	}

	struct _starpu_data_request *r;
	r = _starpu_create_request_to_fetch_data(handle, dst_replicate, mode,
						 detached, async, callback_func, callback_arg);

	/* If no request was created, the handle was already up-to-date on the
	 * node. In this case, _starpu_create_request_to_fetch_data has already
	 * unlocked the header. */
	if (!r)
		return 0;

	_starpu_spin_unlock(&handle->header_lock);

	int ret = async?0:_starpu_wait_data_request_completion(r, 1);
        _STARPU_LOG_OUT();
        return ret;
}

static int prefetch_data_on_node(starpu_data_handle_t handle, struct _starpu_data_replicate *replicate, enum starpu_access_mode mode)
{
	return _starpu_fetch_data_on_node(handle, replicate, mode, 1, 1, NULL, NULL);
}

static int fetch_data(starpu_data_handle_t handle, struct _starpu_data_replicate *replicate, enum starpu_access_mode mode)
{
	return _starpu_fetch_data_on_node(handle, replicate, mode, 0, 0, NULL, NULL);
}

uint32_t _starpu_get_data_refcnt(starpu_data_handle_t handle, uint32_t node)
{
	return handle->per_node[node].refcnt;
}

size_t _starpu_data_get_size(starpu_data_handle_t handle)
{
	return handle->data_size;
}

uint32_t _starpu_data_get_footprint(starpu_data_handle_t handle)
{
	return handle->footprint;
}

/* in case the data was accessed on a write mode, do not forget to
 * make it accessible again once it is possible ! */
void _starpu_release_data_on_node(starpu_data_handle_t handle, uint32_t default_wt_mask, struct _starpu_data_replicate *replicate)
{
	uint32_t wt_mask;
	wt_mask = default_wt_mask | handle->wt_mask;
	wt_mask &= (1<<starpu_memory_nodes_get_count())-1;

	/* Note that it is possible that there is no valid copy of the data (if
	 * starpu_data_invalidate was called for instance). In that case, we do
	 * not enforce any write-through mechanism. */

	unsigned memory_node = replicate->memory_node;

	if (replicate->state != STARPU_INVALID && handle->current_mode & STARPU_W)
	if ((wt_mask & ~(1<<memory_node)))
		_starpu_write_through_data(handle, memory_node, wt_mask);

	uint32_t local_node = _starpu_get_local_memory_node();
	while (_starpu_spin_trylock(&handle->header_lock))
		_starpu_datawizard_progress(local_node, 1);

	/* Release refcnt taken by fetch_data_on_node */
	replicate->refcnt--;
	STARPU_ASSERT(replicate->refcnt >= 0);

	STARPU_ASSERT(handle->busy_count > 0);
	handle->busy_count--;
	_starpu_data_check_not_busy(handle);

	/* In case there was a temporary handle (eg. used for reduction), this
	 * handle may have requested to be destroyed when the data is released
	 * */
	unsigned handle_was_destroyed = handle->lazy_unregister;

	_starpu_notify_data_dependencies(handle);

	if (!handle_was_destroyed)
		_starpu_spin_unlock(&handle->header_lock);
}

static void _starpu_set_data_requested_flag_if_needed(struct _starpu_data_replicate *replicate)
{
// XXX : this is just a hint, so we don't take the lock ...
//	_STARPU_PTHREAD_SPIN_LOCK(&handle->header_lock);

	if (replicate->state == STARPU_INVALID)
	{
		unsigned dst_node = replicate->memory_node;
		replicate->requested[dst_node] = 1;
	}

//	_STARPU_PTHREAD_SPIN_UNLOCK(&handle->header_lock);
}

int starpu_prefetch_task_input_on_node(struct starpu_task *task, uint32_t node)
{
	unsigned nbuffers = task->cl->nbuffers;
	unsigned index;

	for (index = 0; index < nbuffers; index++)
	{
		starpu_data_handle_t handle = task->handles[index];
		enum starpu_access_mode mode = task->cl->modes[index];

		if (mode & (STARPU_SCRATCH|STARPU_REDUX))
			continue;

		struct _starpu_data_replicate *replicate = &handle->per_node[node];
		prefetch_data_on_node(handle, replicate, mode);

		_starpu_set_data_requested_flag_if_needed(replicate);
	}

	return 0;
}

static struct _starpu_data_replicate *get_replicate(starpu_data_handle_t handle, enum starpu_access_mode mode, int workerid, unsigned local_memory_node)
{
	if (mode & (STARPU_SCRATCH|STARPU_REDUX))
		return &handle->per_worker[workerid];
	else
		/* That's a "normal" buffer (R/W) */
		return &handle->per_node[local_memory_node];
}

int _starpu_fetch_task_input(struct _starpu_job *j, uint32_t mask)
{
	_STARPU_TRACE_START_FETCH_INPUT(NULL);

	int profiling = starpu_profiling_status_get();
	struct starpu_task *task = j->task;
	if (profiling && task->profiling_info)
		_starpu_clock_gettime(&task->profiling_info->acquire_data_start_time);

	struct starpu_buffer_descr *descrs = j->ordered_buffers;
	unsigned nbuffers = task->cl->nbuffers;

	unsigned local_memory_node = _starpu_get_local_memory_node();

	int workerid = starpu_worker_get_id();

	unsigned index;
	for (index = 0; index < nbuffers; index++)
	{
		int ret;
		starpu_data_handle_t handle = descrs[index].handle;
		enum starpu_access_mode mode = descrs[index].mode;

		struct _starpu_data_replicate *local_replicate;

		if (index && descrs[index-1].handle == descrs[index].handle)
			/* We have already took this data, skip it. This
			 * depends on ordering putting writes before reads, see
			 * _starpu_compar_handles */
			continue;

		local_replicate = get_replicate(handle, mode, workerid, local_memory_node);

		ret = fetch_data(handle, local_replicate, mode);
		if (STARPU_UNLIKELY(ret))
			goto enomem;
	}

	/* Now that we have taken the data locks in locking order, fill the codelet interfaces in function order.  */
	for (index = 0; index < nbuffers; index++)
	{
		starpu_data_handle_t handle = task->handles[index];
		enum starpu_access_mode mode = task->cl->modes[index];

		struct _starpu_data_replicate *local_replicate;

		local_replicate = get_replicate(handle, mode, workerid, local_memory_node);

		task->interfaces[index] = local_replicate->data_interface;

		if (mode & STARPU_REDUX)
		{
			/* If the replicate was not initialized yet, we have to do it now */
			if (!local_replicate->initialized)
				_starpu_redux_init_data_replicate(handle, local_replicate, workerid);
		}
	}

	if (profiling && task->profiling_info)
		_starpu_clock_gettime(&task->profiling_info->acquire_data_end_time);

	_STARPU_TRACE_END_FETCH_INPUT(NULL);

	return 0;

enomem:
	/* try to unreference all the input that were successfully taken */
	/* XXX broken ... */
	_STARPU_DISP("something went wrong with buffer %u\n", index);
	//push_codelet_output(task, index, mask);
	_starpu_push_task_output(j, mask);
	return -1;
}

void _starpu_push_task_output(struct _starpu_job *j, uint32_t mask)
{
	_STARPU_TRACE_START_PUSH_OUTPUT(NULL);

	int profiling = starpu_profiling_status_get();
	struct starpu_task *task = j->task;
	if (profiling && task->profiling_info)
		_starpu_clock_gettime(&task->profiling_info->release_data_start_time);

        struct starpu_buffer_descr *descrs = j->ordered_buffers;
        unsigned nbuffers = task->cl->nbuffers;

	int workerid = starpu_worker_get_id();
	unsigned local_memory_node = _starpu_get_local_memory_node();

	unsigned index;
	for (index = 0; index < nbuffers; index++)
	{
		starpu_data_handle_t handle = descrs[index].handle;
		enum starpu_access_mode mode = descrs[index].mode;

		struct _starpu_data_replicate *local_replicate;

		if (index && descrs[index-1].handle == descrs[index].handle)
			/* We have already released this data, skip it. This
			 * depends on ordering putting writes before reads, see
			 * _starpu_compar_handles */
			continue;

		local_replicate = get_replicate(handle, mode, workerid, local_memory_node);

		/* In case there was a temporary handle (eg. used for
		 * reduction), this handle may have requested to be destroyed
		 * when the data is released
		 * */
		unsigned handle_was_destroyed = handle->lazy_unregister;

		_starpu_release_data_on_node(handle, mask, local_replicate);
		if (!handle_was_destroyed)
			_starpu_release_data_enforce_sequential_consistency(task, handle);
	}

	if (profiling && task->profiling_info)
		_starpu_clock_gettime(&task->profiling_info->release_data_end_time);

	_STARPU_TRACE_END_PUSH_OUTPUT(NULL);
}

/* NB : this value can only be an indication of the status of a data
	at some point, but there is no strong garantee ! */
unsigned _starpu_is_data_present_or_requested(starpu_data_handle_t handle, uint32_t node)
{
	unsigned ret = 0;

// XXX : this is just a hint, so we don't take the lock ...
//	_STARPU_PTHREAD_SPIN_LOCK(&handle->header_lock);

	if (handle->per_node[node].state != STARPU_INVALID)
	{
		ret  = 1;
	}
	else
	{
		unsigned i;
		unsigned nnodes = starpu_memory_nodes_get_count();

		for (i = 0; i < nnodes; i++)
		{
			if (handle->per_node[node].requested[i] || handle->per_node[node].request[i])
				ret = 1;
		}

	}

//	_STARPU_PTHREAD_SPIN_UNLOCK(&handle->header_lock);

	return ret;
}

