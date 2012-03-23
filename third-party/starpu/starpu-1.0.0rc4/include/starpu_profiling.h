/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
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

#ifndef __STARPU_PROFILING_H__
#define __STARPU_PROFILING_H__

#include <starpu.h>
#include <errno.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define STARPU_PROFILING_DISABLE	0
#define STARPU_PROFILING_ENABLE		1

struct starpu_task_profiling_info
{
	/* Task submission */
	struct timespec submit_time;

	/* Scheduling overhead */
	struct timespec push_start_time;
	struct timespec push_end_time;
	struct timespec pop_start_time;
	struct timespec pop_end_time;

	/* Take input data */
	struct timespec acquire_data_start_time;
	struct timespec acquire_data_end_time;

	/* Execution */
	struct timespec start_time;
	struct timespec end_time;

	/* Release data */
	struct timespec release_data_start_time;
	struct timespec release_data_end_time;

	/* Callback */
	struct timespec callback_start_time;
	struct timespec callback_end_time;

	/* TODO add expected length, expected start/end ? */
	int workerid;

	uint64_t used_cycles;
	uint64_t stall_cycles;
	double power_consumed;
};

/* The timing is provided since the previous call to starpu_worker_get_profiling_info */
struct starpu_worker_profiling_info
{
	struct timespec start_time;
	struct timespec total_time;
	struct timespec executing_time;
	struct timespec sleeping_time;
	int executed_tasks;

	uint64_t used_cycles;
	uint64_t stall_cycles;
	double power_consumed;
};

struct starpu_bus_profiling_info
{
	struct timespec start_time;
	struct timespec total_time;
	int long long transferred_bytes;
	int transfer_count;
};

/* This function sets the ID used for profiling trace filename */
void starpu_set_profiling_id(int new_id);

/* This function sets the profiling status:
 * - enable with STARPU_PROFILING_ENABLE
 * - disable with STARPU_PROFILING_DISABLE
 * Negative return values indicate an error, otherwise the previous status is
 * returned. Calling this function resets the profiling measurements. */
int starpu_profiling_status_set(int status);

/* Return the current profiling status or a negative value in case there was an
 * error. */
int starpu_profiling_status_get(void);

/* Get the profiling info associated to a worker, and reset the profiling
 * measurements. If worker_info is NULL, we only reset the counters. */
int starpu_worker_get_profiling_info(int workerid, struct starpu_worker_profiling_info *worker_info);

int starpu_bus_get_count(void);
int starpu_bus_get_id(int src, int dst);
int starpu_bus_get_src(int busid);
int starpu_bus_get_dst(int busid);

int starpu_bus_get_profiling_info(int busid, struct starpu_bus_profiling_info *bus_info);

/* Some helper functions to manipulate profiling API output */
/* Reset timespec */
static inline void starpu_timespec_clear(struct timespec *tsp)
{
	tsp->tv_sec = 0;
	tsp->tv_nsec = 0;
}

/* Computes result = a + b */
static inline void starpu_timespec_add(struct timespec *a,
					struct timespec *b,
					struct timespec *result)
{
	result->tv_sec = a->tv_sec + b->tv_sec;
	result->tv_nsec = a->tv_nsec + b->tv_nsec;

	if (result->tv_nsec >= 1000000000)
	{
		++(result)->tv_sec;
		result->tv_nsec -= 1000000000;
	}
}

/* Computes res += b */
static inline void starpu_timespec_accumulate(struct timespec *result,
						struct timespec *a)
{
	result->tv_sec += a->tv_sec;
	result->tv_nsec += a->tv_nsec;

	if (result->tv_nsec >= 1000000000)
	{
		++(result)->tv_sec;
		result->tv_nsec -= 1000000000;
	}
}

/* Computes result = a - b */
static inline void starpu_timespec_sub(struct timespec *a,
					struct timespec *b,
					struct timespec *result)
{
	result->tv_sec = a->tv_sec - b->tv_sec;
	result->tv_nsec = a->tv_nsec - b->tv_nsec;

	if ((result)->tv_nsec < 0)
	{
		--(result)->tv_sec;
		result->tv_nsec += 1000000000;
	}
}

#define starpu_timespec_cmp(a, b, CMP)                          \
	(((a)->tv_sec == (b)->tv_sec) ? ((a)->tv_nsec CMP (b)->tv_nsec) : ((a)->tv_sec CMP (b)->tv_sec))

/* Returns the time elapsed between start and end in microseconds */
double starpu_timing_timespec_delay_us(struct timespec *start, struct timespec *end);
double starpu_timing_timespec_to_us(struct timespec *ts);

void starpu_bus_profiling_helper_display_summary(void);
void starpu_worker_profiling_helper_display_summary(void);

#ifdef __cplusplus
}
#endif

#endif /* __STARPU_PROFILING_H__ */
