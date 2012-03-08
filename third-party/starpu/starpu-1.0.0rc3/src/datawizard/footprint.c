/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#include <datawizard/footprint.h>
#include <starpu_hash.h>

uint32_t _starpu_compute_buffers_footprint(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, unsigned nimpl, struct _starpu_job *j)
{
	if (j->footprint_is_computed)
		return j->footprint;

	uint32_t footprint = 0;
	unsigned buffer;

	struct starpu_task *task = j->task;

	if (model && model->per_arch[arch][nimpl].size_base) {
		size_t size = model->per_arch[arch][nimpl].size_base(task, arch, nimpl);
		footprint = starpu_crc32_be_n(&size, sizeof(size), footprint);
	} else if (model && model->size_base) {
		size_t size = model->size_base(task, nimpl);
		footprint = starpu_crc32_be_n(&size, sizeof(size), footprint);
	} else {
		for (buffer = 0; buffer < task->cl->nbuffers; buffer++)
		{
			starpu_data_handle_t handle = task->handles[buffer];

			uint32_t handle_footprint = _starpu_data_get_footprint(handle);

			footprint = starpu_crc32_be(handle_footprint, footprint);
		}
	}

	j->footprint = footprint;
	j->footprint_is_computed = 1;

	return footprint;
}

uint32_t _starpu_compute_data_footprint(starpu_data_handle_t handle)
{
	uint32_t interfaceid = (uint32_t)starpu_handle_get_interface_id(handle);

	STARPU_ASSERT(handle->ops->footprint);

	uint32_t handle_footprint = handle->ops->footprint(handle);

	return starpu_crc32_be(handle_footprint, interfaceid);
}
