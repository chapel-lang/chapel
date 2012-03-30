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

#ifndef __DATA_REQUEST_H__
#define __DATA_REQUEST_H__

#include <semaphore.h>
#include <datawizard/coherency.h>
#include <datawizard/copy_driver.h>
#include <common/list.h>
#include <common/starpu_spinlock.h>

struct _starpu_data_replicate;

struct _starpu_callback_list
{
	void (*callback_func)(void *);
	void *callback_arg;
	struct _starpu_callback_list *next;
};

LIST_TYPE(_starpu_data_request,
	struct _starpu_spinlock lock;
	unsigned refcnt;

	starpu_data_handle_t handle;
	struct _starpu_data_replicate *src_replicate;
	struct _starpu_data_replicate *dst_replicate;

	uint32_t handling_node;

	enum starpu_access_mode mode;

	struct _starpu_async_channel async_channel;

	unsigned completed;
	unsigned prefetch;
	int retval;

	/* The request will not actually be submitted until there remains
	 * dependencies. */
	unsigned ndeps;

	/* in case we have a chain of request (eg. for nvidia multi-GPU) */
	struct _starpu_data_request *next_req[STARPU_MAXNODES];
	/* who should perform the next request ? */
	unsigned next_req_count;

	struct _starpu_callback_list *callbacks;

#ifdef STARPU_USE_FXT
	unsigned com_id;
#endif
)

/* Everyone that wants to access some piece of data will post a request.
 * Not only StarPU internals, but also the application may put such requests */
LIST_TYPE(_starpu_data_requester,
	/* what kind of access is requested ? */
	enum starpu_access_mode mode;

	/* applications may also directly manipulate data */
	unsigned is_requested_by_codelet;

	/* in case this is a codelet that will do the access */
	struct _starpu_job *j;
	unsigned buffer_index;

	/* if this is more complicated ... (eg. application request) 
	 * NB: this callback is not called with the lock taken !
	 */
	void (*ready_data_callback)(void *argcb);
	void *argcb;
)

void _starpu_init_data_request_lists(void);
void _starpu_deinit_data_request_lists(void);
void _starpu_post_data_request(struct _starpu_data_request *r, uint32_t handling_node);
void _starpu_handle_node_data_requests(uint32_t src_node, unsigned may_alloc);
void _starpu_handle_node_prefetch_requests(uint32_t src_node, unsigned may_alloc);

void _starpu_handle_pending_node_data_requests(uint32_t src_node);
void _starpu_handle_all_pending_node_data_requests(uint32_t src_node);

int _starpu_check_that_no_data_request_exists(uint32_t node);

struct _starpu_data_request *_starpu_create_data_request(starpu_data_handle_t handle,
							 struct _starpu_data_replicate *src_replicate,
							 struct _starpu_data_replicate *dst_replicate,
							 uint32_t handling_node,
							 enum starpu_access_mode mode,
							 unsigned ndeps,
							 unsigned is_prefetch);

int _starpu_wait_data_request_completion(struct _starpu_data_request *r, unsigned may_alloc);

void _starpu_data_request_append_callback(struct _starpu_data_request *r,
					  void (*callback_func)(void *),
					  void *callback_arg);

void _starpu_update_prefetch_status(struct _starpu_data_request *r);
#endif // __DATA_REQUEST_H__
