/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#ifndef __DATA_CONCURRENCY_H__
#define __DATA_CONCURRENCY_H__

#include <core/jobs.h>

unsigned _starpu_submit_job_enforce_data_deps(struct _starpu_job *j);

void _starpu_notify_data_dependencies(starpu_data_handle_t handle);

unsigned _starpu_attempt_to_submit_data_request_from_apps(starpu_data_handle_t handle,
							  enum starpu_access_mode mode,
							  void (*callback)(void *), void *argcb);

#endif // __DATA_CONCURRENCY_H__

