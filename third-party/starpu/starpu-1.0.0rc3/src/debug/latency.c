/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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
#include <debug/starpu_debug_helpers.h>
#include <common/config.h>
#include <datawizard/coherency.h>

void _starpu_benchmark_ping_pong(starpu_data_handle_t handle,
			unsigned node0, unsigned node1, unsigned niter)
{
	/* We assume that no one is using that handle !! */
	unsigned iter;
	for (iter = 0; iter < niter; iter++)
	{
		int ret;

		_starpu_spin_lock(&handle->header_lock);
		handle->refcnt++;
		handle->busy_count++;
		_starpu_spin_unlock(&handle->header_lock);

		struct _starpu_data_replicate *replicate_0 = &handle->per_node[node0];
		ret = _starpu_fetch_data_on_node(handle, replicate_0, STARPU_RW, 0, 0, NULL, NULL);
		STARPU_ASSERT(!ret);
		_starpu_release_data_on_node(handle, 0, replicate_0);

		_starpu_spin_lock(&handle->header_lock);
		handle->refcnt++;
		handle->busy_count++;
		_starpu_spin_unlock(&handle->header_lock);

		struct _starpu_data_replicate *replicate_1 = &handle->per_node[node1];
		ret = _starpu_fetch_data_on_node(handle, replicate_1, STARPU_RW, 0, 0, NULL, NULL);
		STARPU_ASSERT(!ret);
		_starpu_release_data_on_node(handle, 0, replicate_1);
	}
}
