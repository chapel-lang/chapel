/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
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
#include <core/jobs.h>
#include <core/workers.h>
#include <datawizard/coherency.h>
#include <profiling/bound.h>
#include <debug/starpu_debug_helpers.h>

void _starpu_debug_display_structures_size(void)
{
	fprintf(stderr, "struct starpu_task\t\t%u bytes\t(%x)\n",
			(unsigned) sizeof(struct starpu_task), (unsigned) sizeof(struct starpu_task));
	fprintf(stderr, "struct _starpu_job\t\t%u bytes\t(%x)\n",
			(unsigned) sizeof(struct _starpu_job), (unsigned) sizeof(struct _starpu_job));
	fprintf(stderr, "struct _starpu_data_state\t%u bytes\t(%x)\n",
			(unsigned) sizeof(struct _starpu_data_state), (unsigned) sizeof(struct _starpu_data_state));
	fprintf(stderr, "struct _starpu_tag\t\t%u bytes\t(%x)\n",
			(unsigned) sizeof(struct _starpu_tag), (unsigned) sizeof(struct _starpu_tag));
	fprintf(stderr, "struct _starpu_cg\t\t%u bytes\t(%x)\n",
			(unsigned) sizeof(struct _starpu_cg), (unsigned) sizeof(struct _starpu_cg));
	fprintf(stderr, "struct _starpu_worker\t\t%u bytes\t(%x)\n",
			(unsigned) sizeof(struct _starpu_worker), (unsigned) sizeof(struct _starpu_worker));
}
