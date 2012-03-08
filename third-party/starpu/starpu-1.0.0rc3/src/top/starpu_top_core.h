/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011 William Braik, Yann Courtois, Jean-Marie Couteyen, Anthony Roy
 * Copyright (C) 2011, 2012 Centre National de la Recherche Scientifique
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

struct starpu_task;

/*
 * Convert timespec to ms
 */
unsigned long long _starpu_top_timing_timespec_to_ms(const struct timespec *ts);

/*
 * This function returns 1 if starpu_top is initialized. 0 otherwise.
 */
int _starpu_top_status_get();

/*
 * This functions notify UI than the task has started or ended
 */
void _starpu_top_task_started(struct starpu_task *task,
			     int devid,
			     const struct timespec* ts);
void _starpu_top_task_ended(struct starpu_task *task,
			   int devid,
			   const struct timespec* ts );
/*
 * This functions notify UI than the task have been planed to
 * run from timestamp_begin to timestamp_end, on computation-core
 */
void __starpu_top_task_prevision_timespec(struct starpu_task *task,
					int devid,
					const struct timespec* start,
					const struct timespec* end);
void _starpu_top_task_prevision(struct starpu_task *task,
			       int devid, unsigned long long start,
			       unsigned long long end);


/****************************************************
***************** Callback function *****************
*****************************************************/

void _starpu_top_process_input_message(char *message);

