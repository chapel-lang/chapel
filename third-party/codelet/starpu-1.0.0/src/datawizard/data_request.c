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

#include <starpu.h>
#include <common/config.h>
#include <datawizard/datawizard.h>

/* requests that have not been treated at all */
static struct _starpu_data_request_list *data_requests[STARPU_MAXNODES];
static struct _starpu_data_request_list *prefetch_requests[STARPU_MAXNODES];
static pthread_mutex_t data_requests_list_mutex[STARPU_MAXNODES];

/* requests that are not terminated (eg. async transfers) */
static struct _starpu_data_request_list *data_requests_pending[STARPU_MAXNODES];
static pthread_mutex_t data_requests_pending_list_mutex[STARPU_MAXNODES];

void _starpu_init_data_request_lists(void)
{
	unsigned i;
	for (i = 0; i < STARPU_MAXNODES; i++)
	{
		prefetch_requests[i] = _starpu_data_request_list_new();
		data_requests[i] = _starpu_data_request_list_new();
		_STARPU_PTHREAD_MUTEX_INIT(&data_requests_list_mutex[i], NULL);

		data_requests_pending[i] = _starpu_data_request_list_new();
		_STARPU_PTHREAD_MUTEX_INIT(&data_requests_pending_list_mutex[i], NULL);
	}
}

void _starpu_deinit_data_request_lists(void)
{
	unsigned i;
	for (i = 0; i < STARPU_MAXNODES; i++)
	{
		_STARPU_PTHREAD_MUTEX_DESTROY(&data_requests_pending_list_mutex[i]);
		_starpu_data_request_list_delete(data_requests_pending[i]);

		_STARPU_PTHREAD_MUTEX_DESTROY(&data_requests_list_mutex[i]);
		_starpu_data_request_list_delete(data_requests[i]);
		_starpu_data_request_list_delete(prefetch_requests[i]);
	}
}

/* this should be called with the lock r->handle->header_lock taken */
static void starpu_data_request_destroy(struct _starpu_data_request *r)
{
	unsigned node;

	/* If this is a write only request, then there is no source and we use
	 * the destination node to cache the request. Otherwise we store the
	 * pending requests between src and dst. */
	if (r->mode & STARPU_R)
	{
		node = r->src_replicate->memory_node;
	}
	else
	{
		node = r->dst_replicate->memory_node;
	}

	STARPU_ASSERT(r->dst_replicate->request[node] == r);
	r->dst_replicate->request[node] = NULL;
	//fprintf(stderr, "DESTROY REQ %p (%d) refcnt %d\n", r, node, r->refcnt);
	_starpu_data_request_delete(r);
}

/* handle->lock should already be taken !  */
struct _starpu_data_request *_starpu_create_data_request(starpu_data_handle_t handle,
							 struct _starpu_data_replicate *src_replicate,
							 struct _starpu_data_replicate *dst_replicate,
							 uint32_t handling_node,
							 enum starpu_access_mode mode,
							 unsigned ndeps,
							 unsigned is_prefetch)
{
	struct _starpu_data_request *r = _starpu_data_request_new();

	_starpu_spin_checklocked(&handle->header_lock);

	_starpu_spin_init(&r->lock);

	r->handle = handle;
	r->src_replicate = src_replicate;
	r->dst_replicate = dst_replicate;
	r->mode = mode;
	r->handling_node = handling_node;
	r->completed = 0;
	r->prefetch = is_prefetch;
	r->retval = -1;
	r->ndeps = ndeps;
	r->next_req_count = 0;
	r->callbacks = NULL;

	_starpu_spin_lock(&r->lock);

	/* Take a reference on the target for the request to be able to write it */
	dst_replicate->refcnt++;
	handle->busy_count++;

	if (mode & STARPU_R)
	{
		unsigned src_node = src_replicate->memory_node;
		dst_replicate->request[src_node] = r;
		/* Take a reference on the source for the request to be able to read it */
		src_replicate->refcnt++;
		handle->busy_count++;
	}
	else
	{
		unsigned dst_node = dst_replicate->memory_node;
		dst_replicate->request[dst_node] = r;
	}

	r->refcnt = 1;

	_starpu_spin_unlock(&r->lock);

	return r;
}

int _starpu_wait_data_request_completion(struct _starpu_data_request *r, unsigned may_alloc)
{
	int retval;
	int do_delete = 0;

	uint32_t local_node = _starpu_get_local_memory_node();

	do
	{
		_starpu_spin_lock(&r->lock);

		if (r->completed)
			break;

		_starpu_spin_unlock(&r->lock);

#ifndef STARPU_NON_BLOCKING_DRIVERS
		_starpu_wake_all_blocked_workers_on_node(r->handling_node);
#endif

		_starpu_datawizard_progress(local_node, may_alloc);

	}
	while (1);


	retval = r->retval;
	if (retval)
		_STARPU_DISP("REQUEST %p COMPLETED (retval %d) !\n", r, r->retval);


	r->refcnt--;

	/* if nobody is waiting on that request, we can get rid of it */
	if (r->refcnt == 0)
		do_delete = 1;

	_starpu_spin_unlock(&r->lock);

	if (do_delete)
		starpu_data_request_destroy(r);

	return retval;
}

/* this is non blocking */
void _starpu_post_data_request(struct _starpu_data_request *r, uint32_t handling_node)
{
//	_STARPU_DEBUG("POST REQUEST\n");

	/* If some dependencies are not fulfilled yet, we don't actually post the request */
	if (r->ndeps > 0)
		return;

	if (r->mode & STARPU_R)
	{
		STARPU_ASSERT(r->src_replicate->allocated);
		STARPU_ASSERT(r->src_replicate->refcnt);
	}

	/* insert the request in the proper list */
	_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_list_mutex[handling_node]);
	if (r->prefetch)
		_starpu_data_request_list_push_back(prefetch_requests[handling_node], r);
	else
		_starpu_data_request_list_push_back(data_requests[handling_node], r);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[handling_node]);

#ifndef STARPU_NON_BLOCKING_DRIVERS
	_starpu_wake_all_blocked_workers_on_node(handling_node);
#endif
}

/* We assume that r->lock is taken by the caller */
void _starpu_data_request_append_callback(struct _starpu_data_request *r, void (*callback_func)(void *), void *callback_arg)
{
	STARPU_ASSERT(r);

	if (callback_func)
	{
		struct _starpu_callback_list *link = (struct _starpu_callback_list *) malloc(sizeof(struct _starpu_callback_list));
		STARPU_ASSERT(link);

		link->callback_func = callback_func;
		link->callback_arg = callback_arg;
		link->next = r->callbacks;
		r->callbacks = link;
	}
}

/* This method is called with handle's header_lock taken */
static void starpu_handle_data_request_completion(struct _starpu_data_request *r)
{
	unsigned do_delete = 0;
	starpu_data_handle_t handle = r->handle;
	enum starpu_access_mode mode = r->mode;

	struct _starpu_data_replicate *src_replicate = r->src_replicate;
	struct _starpu_data_replicate *dst_replicate = r->dst_replicate;


#ifdef STARPU_MEMORY_STATUS
	enum _starpu_cache_state old_src_replicate_state = src_replicate->state;
#endif

	_starpu_spin_checklocked(&handle->header_lock);
	_starpu_update_data_state(handle, r->dst_replicate, mode);

#ifdef STARPU_MEMORY_STATUS
	if (src_replicate->state == STARPU_INVALID)
	{
		if (old_src_replicate_state == STARPU_OWNER)
			_starpu_handle_stats_invalidated(handle, src_replicate->memory_node);
		else
		{
			/* XXX Currently only ex-OWNER are tagged as invalidated */
			/* XXX Have to check all old state of every node in case a SHARED data become OWNED by the dst_replicate */
		}

	}
	if (dst_replicate->state == STARPU_SHARED)
		_starpu_handle_stats_loaded_shared(handle, dst_replicate->memory_node);
	else if (dst_replicate->state == STARPU_OWNER)
	{
		_starpu_handle_stats_loaded_owner(handle, dst_replicate->memory_node);
	}
#endif

#ifdef STARPU_USE_FXT
	uint32_t src_node = src_replicate->memory_node;
	uint32_t dst_node = dst_replicate->memory_node;
	size_t size = _starpu_data_get_size(handle);
	_STARPU_TRACE_END_DRIVER_COPY(src_node, dst_node, size, r->com_id);
#endif

	/* Once the request has been fulfilled, we may submit the requests that
	 * were chained to that request. */
	unsigned chained_req;
	for (chained_req = 0; chained_req < r->next_req_count; chained_req++)
	{
		struct _starpu_data_request *next_req = r->next_req[chained_req];
		STARPU_ASSERT(next_req->ndeps > 0);
		next_req->ndeps--;
		_starpu_post_data_request(next_req, next_req->handling_node);
	}

	r->completed = 1;

	/* Remove a reference on the destination replicate for the request */
	STARPU_ASSERT(dst_replicate->refcnt > 0);
	dst_replicate->refcnt--;
	STARPU_ASSERT(handle->busy_count > 0);
	handle->busy_count--;

	/* In case the source was "locked" by the request too */
	if (mode & STARPU_R)
	{
		STARPU_ASSERT(src_replicate->refcnt > 0);
		src_replicate->refcnt--;
		STARPU_ASSERT(handle->busy_count > 0);
		handle->busy_count--;
	}

	_starpu_data_check_not_busy(handle);

	r->refcnt--;

	/* if nobody is waiting on that request, we can get rid of it */
	if (r->refcnt == 0)
		do_delete = 1;

	r->retval = 0;

	/* In case there are one or multiple callbacks, we execute them now. */
	struct _starpu_callback_list *callbacks = r->callbacks;

	_starpu_spin_unlock(&r->lock);

	if (do_delete)
		starpu_data_request_destroy(r);

	_starpu_spin_unlock(&handle->header_lock);

	/* We do the callback once the lock is released so that they can do
	 * blocking operations with the handle (eg. release it) */
	while (callbacks)
	{
		callbacks->callback_func(callbacks->callback_arg);

		struct _starpu_callback_list *next = callbacks->next;
		free(callbacks);
		callbacks = next;
	}
}

/* TODO : accounting to see how much time was spent working for other people ... */
static int starpu_handle_data_request(struct _starpu_data_request *r, unsigned may_alloc)
{
	starpu_data_handle_t handle = r->handle;

	_starpu_spin_lock(&handle->header_lock);
	_starpu_spin_lock(&r->lock);

	struct _starpu_data_replicate *src_replicate = r->src_replicate;
	struct _starpu_data_replicate *dst_replicate = r->dst_replicate;

	enum starpu_access_mode r_mode = r->mode;

	STARPU_ASSERT(!(r_mode & STARPU_R) || src_replicate);
	STARPU_ASSERT(!(r_mode & STARPU_R) || src_replicate->allocated);
	STARPU_ASSERT(!(r_mode & STARPU_R) || src_replicate->refcnt);

	_starpu_spin_unlock(&r->lock);

	/* perform the transfer */
	/* the header of the data must be locked by the worker that submitted the request */

	r->retval = _starpu_driver_copy_data_1_to_1(handle, src_replicate,
						    dst_replicate, !(r_mode & STARPU_R), r, may_alloc);

	if (r->retval == -ENOMEM)
	{
		/* If there was not enough memory, we will try to redo the
		 * request later. */
		_starpu_spin_unlock(&handle->header_lock);
		return -ENOMEM;
	}

	if (r->retval == -EAGAIN)
	{
		/* The request was successful, but could not be terminted
		 * immediatly. We will handle the completion of the request
		 * asynchronously. The request is put in the list of "pending"
		 * requests in the meantime. */
		_starpu_spin_unlock(&handle->header_lock);

		_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_pending_list_mutex[r->handling_node]);
		_starpu_data_request_list_push_front(data_requests_pending[r->handling_node], r);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_pending_list_mutex[r->handling_node]);

		return -EAGAIN;
	}

	/* the request has been handled */
	_starpu_spin_lock(&r->lock);
	starpu_handle_data_request_completion(r);

	return 0;
}

void _starpu_handle_node_data_requests(uint32_t src_node, unsigned may_alloc)
{
	struct _starpu_data_request *r;
	struct _starpu_data_request_list *new_data_requests;

	/* take all the entries from the request list */
        _STARPU_PTHREAD_MUTEX_LOCK(&data_requests_list_mutex[src_node]);

	struct _starpu_data_request_list *local_list = data_requests[src_node];

	if (_starpu_data_request_list_empty(local_list))
	{
		/* there is no request */
                _STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[src_node]);

		return;
	}

	/* There is an entry: we create a new empty list to replace the list of
	 * requests, and we handle the request(s) one by one in the former
	 * list, without concurrency issues.*/
	data_requests[src_node] = _starpu_data_request_list_new();

	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[src_node]);

	new_data_requests = _starpu_data_request_list_new();

	/* for all entries of the list */
	while (!_starpu_data_request_list_empty(local_list))
	{
                int res;

		r = _starpu_data_request_list_pop_front(local_list);

		res = starpu_handle_data_request(r, may_alloc);
		if (res == -ENOMEM)
		{
			_starpu_data_request_list_push_back(new_data_requests, r);
		}
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_list_mutex[src_node]);
	_starpu_data_request_list_push_list_front(new_data_requests, data_requests[src_node]);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[src_node]);

	_starpu_data_request_list_delete(new_data_requests);
	_starpu_data_request_list_delete(local_list);
}

void _starpu_handle_node_prefetch_requests(uint32_t src_node, unsigned may_alloc)
{
	struct _starpu_data_request *r;
	struct _starpu_data_request_list *new_data_requests;
	struct _starpu_data_request_list *new_prefetch_requests;

	/* take all the entries from the request list */
        _STARPU_PTHREAD_MUTEX_LOCK(&data_requests_list_mutex[src_node]);

	struct _starpu_data_request_list *local_list = prefetch_requests[src_node];

	if (_starpu_data_request_list_empty(local_list))
	{
		/* there is no request */
                _STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[src_node]);
		return;
	}

	/* There is an entry: we create a new empty list to replace the list of
	 * requests, and we handle the request(s) one by one in the former
	 * list, without concurrency issues.*/
	prefetch_requests[src_node] = _starpu_data_request_list_new();

	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[src_node]);

	new_data_requests = _starpu_data_request_list_new();
	new_prefetch_requests = _starpu_data_request_list_new();

	/* for all entries of the list */
	while (!_starpu_data_request_list_empty(local_list))
	{
                int res;

		r = _starpu_data_request_list_pop_front(local_list);

		res = starpu_handle_data_request(r, may_alloc);
		if (res == -ENOMEM )
		{
			if (r->prefetch)
				_starpu_data_request_list_push_back(new_prefetch_requests, r);
			else
			{
				/* Prefetch request promoted while in tmp list*/
				_starpu_data_request_list_push_back(new_data_requests, r);
			}
			break;
		}
	}

	while(!_starpu_data_request_list_empty(local_list))
	{
		r = _starpu_data_request_list_pop_front(local_list);
		if (r->prefetch)
			_starpu_data_request_list_push_back(new_prefetch_requests, r);
		else
			/* Prefetch request promoted while in tmp list*/
			_starpu_data_request_list_push_back(new_data_requests, r);
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_list_mutex[src_node]);
	_starpu_data_request_list_push_list_front(new_data_requests, data_requests[src_node]);
	_starpu_data_request_list_push_list_front(new_prefetch_requests, prefetch_requests[src_node]);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[src_node]);

	_starpu_data_request_list_delete(new_data_requests);
	_starpu_data_request_list_delete(new_prefetch_requests);
	_starpu_data_request_list_delete(local_list);
}

static void _handle_pending_node_data_requests(uint32_t src_node, unsigned force)
{
//	_STARPU_DEBUG("_starpu_handle_pending_node_data_requests ...\n");
//
	struct _starpu_data_request_list *new_data_requests_pending = _starpu_data_request_list_new();

	_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_pending_list_mutex[src_node]);

	/* for all entries of the list */
	struct _starpu_data_request_list *local_list = data_requests_pending[src_node];
	data_requests_pending[src_node] = _starpu_data_request_list_new();

	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_pending_list_mutex[src_node]);

	while (!_starpu_data_request_list_empty(local_list))
	{
		struct _starpu_data_request *r;
		r = _starpu_data_request_list_pop_front(local_list);

		starpu_data_handle_t handle = r->handle;

		_starpu_spin_lock(&handle->header_lock);

		_starpu_spin_lock(&r->lock);

		/* wait until the transfer is terminated */
		if (force)
		{
			_starpu_driver_wait_request_completion(&r->async_channel);
			starpu_handle_data_request_completion(r);
		}
		else
		{
			if (_starpu_driver_test_request_completion(&r->async_channel))
			{
				/* The request was completed */
				starpu_handle_data_request_completion(r);
			}
			else
			{
				/* The request was not completed, so we put it
				 * back again on the list of pending requests
				 * so that it can be handled later on. */
				_starpu_spin_unlock(&r->lock);
				_starpu_spin_unlock(&handle->header_lock);

				_starpu_data_request_list_push_back(new_data_requests_pending, r);
			}
		}
	}
	_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_pending_list_mutex[src_node]);
	_starpu_data_request_list_push_list_back(data_requests_pending[src_node], new_data_requests_pending);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_pending_list_mutex[src_node]);

	_starpu_data_request_list_delete(local_list);
	_starpu_data_request_list_delete(new_data_requests_pending);
}

void _starpu_handle_pending_node_data_requests(uint32_t src_node)
{
	_handle_pending_node_data_requests(src_node, 0);
}

void _starpu_handle_all_pending_node_data_requests(uint32_t src_node)
{
	_handle_pending_node_data_requests(src_node, 1);
}

int _starpu_check_that_no_data_request_exists(uint32_t node)
{
	/* XXX lock that !!! that's a quick'n'dirty test */
	int no_request = _starpu_data_request_list_empty(data_requests[node]);
	int no_pending = _starpu_data_request_list_empty(data_requests_pending[node]);

	return (no_request && no_pending);
}


void _starpu_update_prefetch_status(struct _starpu_data_request *r)
{
	STARPU_ASSERT(r->prefetch > 0);
	r->prefetch=0;

	/* We have to promote chained_request too! */
	unsigned chained_req;
	for (chained_req = 0; chained_req < r->next_req_count; chained_req++)
	{
		struct _starpu_data_request *next_req = r->next_req[chained_req];
		if (next_req->prefetch)
			_starpu_update_prefetch_status(next_req);
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&data_requests_list_mutex[r->handling_node]);

	/* The request can be in a different list (handling request or the temp list)
	 * we have to check that it is really in the prefetch list. */
	struct _starpu_data_request *r_iter;
	for (r_iter = _starpu_data_request_list_begin(prefetch_requests[r->handling_node]);
	     r_iter != _starpu_data_request_list_end(prefetch_requests[r->handling_node]);
	     r_iter = _starpu_data_request_list_next(r_iter))
	{

		if (r==r_iter)
		{
			_starpu_data_request_list_erase(prefetch_requests[r->handling_node],r);
			_starpu_data_request_list_push_front(data_requests[r->handling_node],r);
			break;
		}
	}
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_requests_list_mutex[r->handling_node]);
}
