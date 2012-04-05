/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
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
#include <starpu_profiling.h>
#include <profiling/profiling.h>

void starpu_bus_profiling_helper_display_summary(void)
{
	int long long sum_transferred = 0;

	fprintf(stderr, "\nData transfer statistics:\n");
	fprintf(stderr,   "*************************\n");

	int busid;
	int bus_cnt = starpu_bus_get_count();
	for (busid = 0; busid < bus_cnt; busid++)
	{
		int src, dst;

		src = starpu_bus_get_src(busid);
		dst = starpu_bus_get_dst(busid);

		struct starpu_bus_profiling_info bus_info;
		starpu_bus_get_profiling_info(busid, &bus_info);

		int long long transferred = bus_info.transferred_bytes;
		int long long transfer_cnt =  bus_info.transfer_count;
		double elapsed_time = starpu_timing_timespec_to_us(&bus_info.total_time);

		fprintf(stderr, "\t%d -> %d\t%.2lf MB\t%.2lfMB/s\t(transfers : %lld - avg %.2lf MB)\n", src, dst, (1.0*transferred)/(1024*1024), transferred/elapsed_time, transfer_cnt, (1.0*transferred)/(transfer_cnt*1024*1024));

		sum_transferred += transferred;
	}

	fprintf(stderr, "Total transfers: %.2lf MB\n", (1.0*sum_transferred)/(1024*1024));
}

void starpu_worker_profiling_helper_display_summary(void)
{
	double sum_consumed = 0.;
	int profiling = starpu_profiling_status_get();
	fprintf(stderr, "\nWorker statistics:\n");
	fprintf(stderr,   "******************\n");
	double overall_time = 0;

	int workerid;
	int worker_cnt = starpu_worker_get_count();
	for (workerid = 0; workerid < worker_cnt; workerid++)
	{
		struct starpu_worker_profiling_info info;
		starpu_worker_get_profiling_info(workerid, &info);
		char name[32];

		starpu_worker_get_name(workerid, name, sizeof(name));

		if (profiling)
		{
			double total_time = starpu_timing_timespec_to_us(&info.total_time) / 1000.;
			double executing_time = starpu_timing_timespec_to_us(&info.executing_time) / 1000.;
			double sleeping_time = starpu_timing_timespec_to_us(&info.sleeping_time) / 1000.;
			if (total_time > overall_time)
				overall_time = total_time;

			fprintf(stderr, "%-32s\n", name);
			fprintf(stderr, "\t%d task(s)\n\ttotal: %.2lf ms executing: %.2lf ms sleeping: %.2lf\n", info.executed_tasks, total_time, executing_time, sleeping_time);
			if (info.used_cycles || info.stall_cycles)
				fprintf(stderr, "\t%lu Mcy %lu Mcy stall\n", info.used_cycles/1000000, info.stall_cycles/1000000);
			if (info.power_consumed)
				fprintf(stderr, "\t%f J consumed\n", info.power_consumed);
		}
		else
		{
			fprintf(stderr, "\t%-32s\t%d task(s)\n", name, info.executed_tasks);
		}

		sum_consumed += info.power_consumed;
	}

	if (profiling)
	{
		const char *strval_idle_power = getenv("STARPU_IDLE_POWER");
		if (strval_idle_power)
		{
			double idle_power = atof(strval_idle_power); /* Watt */
			double idle_consumption = idle_power * overall_time / 1000.; /* J */

			fprintf(stderr, "Idle consumption: %.2lf J\n", idle_consumption);
			sum_consumed += idle_consumption;
		}
	}
	if (profiling && sum_consumed)
		fprintf(stderr, "Total consumption: %.2lf J\n", sum_consumed);
}
