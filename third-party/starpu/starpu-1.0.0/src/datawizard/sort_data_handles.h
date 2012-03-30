/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
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

#ifndef __SORT_DATA_HANDLES_H__
#define __SORT_DATA_HANDLES_H__

#include <starpu.h>
#include <common/config.h>
#include <stdlib.h>

#include <stdarg.h>
#include <datawizard/coherency.h>
#include <datawizard/memalloc.h>

/* To avoid deadlocks, we reorder the different buffers accessed to by the task
 * so that we always grab the rw-lock associated to the handles in the same
 * order. */
void _starpu_sort_task_handles(struct starpu_buffer_descr descr[], unsigned nbuffers);

#endif // SORT_DATA_HANDLES
