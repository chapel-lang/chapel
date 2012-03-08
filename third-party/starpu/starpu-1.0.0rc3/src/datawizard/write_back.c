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

#include <datawizard/datawizard.h>
#include <datawizard/write_back.h>
#include <core/dependencies/data_concurrency.h>

static void wt_callback(void *arg)
{
	starpu_data_handle_t handle = (starpu_data_handle_t) arg;

	_starpu_spin_lock(&handle->header_lock);
	_starpu_notify_data_dependencies(handle);
	_starpu_spin_unlock(&handle->header_lock);
}

void _starpu_write_through_data(starpu_data_handle_t handle, uint32_t requesting_node,
				uint32_t write_through_mask)
{
	if ((write_through_mask & ~(1<<requesting_node)) == 0)
	{
		/* nothing will be done ... */
		return;
	}

	/* first commit all changes onto the nodes specified by the mask */
	uint32_t node, max;
	for (node = 0, max = starpu_memory_nodes_get_count(); node < max; node++)
	{
		if (write_through_mask & (1<<node))
		{
			/* we need to commit the buffer on that node */
			if (node != requesting_node)
			{
				while (_starpu_spin_trylock(&handle->header_lock))
					_starpu_datawizard_progress(requesting_node, 1);

				/* We need to keep a Read lock to avoid letting writers corrupt our copy.  */
				STARPU_ASSERT(handle->current_mode != STARPU_REDUX);
				STARPU_ASSERT(handle->current_mode != STARPU_SCRATCH);
				handle->refcnt++;
				handle->busy_count++;
				handle->current_mode = STARPU_R;

				struct _starpu_data_request *r;
				r = _starpu_create_request_to_fetch_data(handle, &handle->per_node[node],
									 STARPU_R, 1, 1, wt_callback, handle);

			        /* If no request was created, the handle was already up-to-date on the
			         * node */
			        if (r)
				        _starpu_spin_unlock(&handle->header_lock);
			}
		}
	}
}

void starpu_data_set_wt_mask(starpu_data_handle_t handle, uint32_t wt_mask)
{
	handle->wt_mask = wt_mask;

	/* in case the data has some children, set their wt_mask as well */
	if (handle->nchildren > 0)
	{
		unsigned child;
		for (child = 0; child < handle->nchildren; child++)
			starpu_data_set_wt_mask(&handle->children[child], wt_mask);
	}
}
