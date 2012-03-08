/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011 William Braik, Yann Courtois, Jean-Marie Couteyen, Anthony Roy
 * Copyright (C) 2011 Centre National de la Recherche Scientifique
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

#include <starpu_top.h>
#include <top/starpu_top_message_queue.h>
#include <top/starpu_top_connection.h>
#include <top/starpu_top_core.h>
#include <core/task.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <common/timing.h>

/********************************************
 **************TASK RELATED FUNCTIONS********
 *******************************************/

void _starpu_top_task_started(struct starpu_task *task,
			     int devid,
			     const struct timespec *ts)
{
	unsigned long long taskid = _starpu_get_job_associated_to_task(task)->job_id;
	STARPU_ASSERT(_starpu_top_status_get());
	char *str = (char *) malloc(sizeof(char)*64);
	snprintf(str, 64,
				"START;%llu;%d;%llu\n",
				taskid,
				devid,
				_starpu_top_timing_timespec_to_ms(ts));

	_starpu_top_message_add(_starpu_top_mt, str);
}

void _starpu_top_task_ended(struct starpu_task *task,
			   int devid,
			   const struct timespec *ts)
{
	unsigned long long taskid = _starpu_get_job_associated_to_task(task)->job_id;
	(void) devid; //unused
	STARPU_ASSERT(_starpu_top_status_get());
	char *str = (char *) malloc(sizeof(char)*64);
	snprintf(str, 64,
				"END;%llu;%llu\n",
				taskid,
				_starpu_top_timing_timespec_to_ms(ts));

	_starpu_top_message_add(_starpu_top_mt, str);
}

void __starpu_top_task_prevision_timespec(struct starpu_task *task,
					int devid,
					const struct timespec* start,
					const struct timespec* end)
{
	_starpu_top_task_prevision(task,
				  devid,
				  _starpu_top_timing_timespec_to_ms(start),
				  _starpu_top_timing_timespec_to_ms(end));
}

void _starpu_top_task_prevision(struct starpu_task *task,
			       int devid,
			       unsigned long long start,
			       unsigned long long end)
{
	unsigned long long taskid = _starpu_get_job_associated_to_task(task)->job_id;
	STARPU_ASSERT(_starpu_top_status_get());
	struct timespec now;
	_starpu_clock_gettime(&now);
	char * str= (char *)malloc(sizeof(char)*200);
	snprintf(str, 128,
		 "PREV;%llu;%d;%llu;%llu;%llu\n",
		 taskid,
		 devid,
		 _starpu_top_timing_timespec_to_ms(&now),
		 start,
		 end);

	_starpu_top_message_add(_starpu_top_mt, str);
}
