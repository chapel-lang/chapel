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

#ifndef __DRIVER_COMMON_H__
#define __DRIVER_COMMON_H__

#include <sys/time.h>
#include <starpu.h>
#include <core/jobs.h>
#include <common/utils.h>

void _starpu_driver_start_job(struct _starpu_worker *args, struct _starpu_job *j,
			      struct timespec *codelet_start, int rank, int profiling);
void _starpu_driver_end_job(struct _starpu_worker *args, struct _starpu_job *j, enum starpu_perf_archtype perf_arch,
			    struct timespec *codelet_end, int rank, int profiling);
void _starpu_driver_update_job_feedback(struct _starpu_job *j, struct _starpu_worker *worker_args,
					enum starpu_perf_archtype perf_arch,
					struct timespec *codelet_start, struct timespec *codelet_end, int profiling);

void _starpu_block_worker(int workerid, pthread_cond_t *cond, pthread_mutex_t *mutex);

#endif // __DRIVER_COMMON_H__
