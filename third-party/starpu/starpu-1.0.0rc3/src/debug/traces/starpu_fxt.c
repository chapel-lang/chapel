/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010, 2011  Université de Bordeaux 1
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
#include <common/config.h>

#ifdef STARPU_USE_FXT
#include "starpu_fxt.h"
#include <inttypes.h>
#include <starpu_hash.h>

static char *cpus_worker_colors[STARPU_NMAXWORKERS] = {"/greens9/7", "/greens9/6", "/greens9/5", "/greens9/4",  "/greens9/9", "/greens9/3",  "/greens9/2",  "/greens9/1"  };
static char *cuda_worker_colors[STARPU_NMAXWORKERS] = {"/ylorrd9/9", "/ylorrd9/6", "/ylorrd9/3", "/ylorrd9/1", "/ylorrd9/8", "/ylorrd9/7", "/ylorrd9/4", "/ylorrd9/2",  "/ylorrd9/1"};
static char *opencl_worker_colors[STARPU_NMAXWORKERS] = {"/blues9/9", "/blues9/6", "/blues9/3", "/blues9/1", "/blues9/8", "/blues9/7", "/blues9/4", "/blues9/2",  "/blues9/1"};
static char *other_worker_colors[STARPU_NMAXWORKERS] = {"/greys9/9", "/greys9/8", "/greys9/7", "/greys9/6"};
static char *worker_colors[STARPU_NMAXWORKERS];

static unsigned opencl_index = 0;
static unsigned cuda_index = 0;
static unsigned cpus_index = 0;
static unsigned other_index = 0;

static void set_next_other_worker_color(int workerid)
{
	worker_colors[workerid] = other_worker_colors[other_index++];
}

static void set_next_cpu_worker_color(int workerid)
{
	worker_colors[workerid] = cpus_worker_colors[cpus_index++];
}

static void set_next_cuda_worker_color(int workerid)
{
	worker_colors[workerid] = cuda_worker_colors[cuda_index++];
}

static void set_next_opencl_worker_color(int workerid)
{
	worker_colors[workerid] = opencl_worker_colors[opencl_index++];
}

static const char *get_worker_color(int workerid)
{
	return worker_colors[workerid];
}

static unsigned get_colour_symbol_red(char *name)
{
	/* choose some colour ... that's disguting yes */
	uint32_t hash_symbol = starpu_crc32_string(name, 0);
	return (unsigned)starpu_crc32_string("red", hash_symbol) % 1024;
}

static unsigned get_colour_symbol_green(char *name)
{
	/* choose some colour ... that's disguting yes */
	uint32_t hash_symbol = starpu_crc32_string(name, 0);
	return (unsigned)starpu_crc32_string("green", hash_symbol) % 1024;
}

static unsigned get_colour_symbol_blue(char *name)
{
	/* choose some colour ... that's disguting yes */
	uint32_t hash_symbol = starpu_crc32_string(name, 0);
	return (unsigned)starpu_crc32_string("blue", hash_symbol) % 1024;
}

static float last_codelet_start[STARPU_NMAXWORKERS];
static char last_codelet_symbol[128][STARPU_NMAXWORKERS];

/* If more than a period of time has elapsed, we flush the profiling info,
 * otherwise they are accumulated everytime there is a new relevant event. */
#define ACTIVITY_PERIOD	75.0
static double last_activity_flush_timestamp[STARPU_NMAXWORKERS];
static double accumulated_sleep_time[STARPU_NMAXWORKERS];
static double accumulated_exec_time[STARPU_NMAXWORKERS];

LIST_TYPE(_starpu_symbol_name,
	char *name;
)

static struct _starpu_symbol_name_list *symbol_list;

LIST_TYPE(_starpu_communication,
	unsigned comid;
	float comm_start;
	float bandwidth;
	unsigned src_node;
	unsigned dst_node;
)

static struct _starpu_communication_list *communication_list;

/*
 * Paje trace file tools
 */

static FILE *out_paje_file;
static FILE *distrib_time;
static FILE *activity_file;

/*
 * Generic tools
 */

static float get_event_time_stamp(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	return (float)((ev->time-options->file_offset)/1000000.0);
}

static int nworkers = 0;

static int register_worker_id(unsigned long tid)
{
	int workerid = nworkers++;

	/* create a new key in the htable */
	char *tidstr = malloc(16*sizeof(char));
	sprintf(tidstr, "%lu", tid);

	ENTRY item;
		item.key = tidstr;
		item.data = (void *)(uintptr_t)workerid;

	ENTRY *res;
	res = hsearch(item, FIND);

	/* only register a thread once */
	STARPU_ASSERT(res == NULL);

	res = hsearch(item, ENTER);
	STARPU_ASSERT(res);

	return workerid;
}

static int find_worker_id(unsigned long tid)
{
	char tidstr[16];
	sprintf(tidstr, "%lu", tid);

	ENTRY item;
		item.key = tidstr;
		item.data = NULL;
	ENTRY *res;
	res = hsearch(item, FIND);
	if (!res)
		return -1;

	int id = (uintptr_t)(res->data);

	return id;
}

static void update_accumulated_time(int worker, double sleep_time, double exec_time, double current_timestamp, int forceflush)
{
	accumulated_sleep_time[worker] += sleep_time;
	accumulated_exec_time[worker] += exec_time;

	/* If sufficient time has elapsed since the last flush, we have a new
	 * point in our graph */
	double elapsed = current_timestamp - last_activity_flush_timestamp[worker];
	if (forceflush || (elapsed > ACTIVITY_PERIOD))
	{
		if (activity_file)
			fprintf(activity_file, "%d\t%f\t%f\t%f\t%f\n", worker, current_timestamp, elapsed, accumulated_exec_time[worker], accumulated_sleep_time[worker]);

		/* reset the accumulated times */
		last_activity_flush_timestamp[worker] = current_timestamp;
		accumulated_sleep_time[worker] = 0.0;
		accumulated_exec_time[worker] = 0.0;
	}
}

/*
 *	Initialization
 */

static void handle_new_mem_node(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	char *prefix = options->file_prefix;

	if (out_paje_file)
	{
		fprintf(out_paje_file, "7       %f	%"PRIu64"      Mn      %sp	%sMEMNODE%"PRIu64"\n", get_event_time_stamp(ev, options), ev->param[0], prefix, options->file_prefix, ev->param[0]);

		if (!options->no_bus)
			fprintf(out_paje_file, "13       %f bw %sMEMNODE%"PRIu64" 0.0\n", 0.0f, prefix, ev->param[0]);
	}
}

static void handle_worker_init_start(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	/*
	   arg0 : type of worker (cuda, cpu ..)
	   arg1 : memory node
	   arg2 : thread id
	*/
	char *prefix = options->file_prefix;

	if (out_paje_file)
	fprintf(out_paje_file, "7       %f	%s%"PRIu64"      T      %sMEMNODE%"PRIu64"       %s%"PRIu64"\n",
		get_event_time_stamp(ev, options), prefix, ev->param[3], prefix, ev->param[2], prefix, ev->param[3]);

	int devid = ev->param[1];
	int workerid = register_worker_id(ev->param[3]);

	char *kindstr = "";
	enum starpu_perf_archtype archtype = 0;

	switch (ev->param[0])
	{
		case _STARPU_FUT_APPS_KEY:
			set_next_other_worker_color(workerid);
			kindstr = "apps";
			break;
		case _STARPU_FUT_CPU_KEY:
			set_next_cpu_worker_color(workerid);
			kindstr = "cpu";
			archtype = STARPU_CPU_DEFAULT;
			break;
		case _STARPU_FUT_CUDA_KEY:
			set_next_cuda_worker_color(workerid);
			kindstr = "cuda";
			archtype = STARPU_CUDA_DEFAULT + devid;
			break;
		case _STARPU_FUT_OPENCL_KEY:
			set_next_opencl_worker_color(workerid);
			kindstr = "opencl";
			archtype = STARPU_OPENCL_DEFAULT + devid;
			break;
		default:
			STARPU_ABORT();
	}

	/* start initialization */
	if (out_paje_file)
	fprintf(out_paje_file, "10       %f     S      %s%"PRIu64"      I\n",
			get_event_time_stamp(ev, options), prefix, ev->param[3]);

	if (activity_file)
	fprintf(activity_file, "name\t%d\t%s %d\n", workerid, kindstr, devid);

	snprintf(options->worker_names[workerid], 256, "%s %d", kindstr, devid);
	options->worker_archtypes[workerid] = archtype;
}

static void handle_worker_init_end(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	char *prefix = options->file_prefix;

	if (out_paje_file)
	fprintf(out_paje_file, "10       %f     S      %s%"PRIu64"      B\n",
			get_event_time_stamp(ev, options), prefix, ev->param[0]);

	/* Initilize the accumulated time counters */
	int worker = find_worker_id(ev->param[0]);
	last_activity_flush_timestamp[worker] = get_event_time_stamp(ev, options);
	accumulated_sleep_time[worker] = 0.0;
	accumulated_exec_time[worker] = 0.0;
}

static void handle_worker_deinit_start(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	char *prefix = options->file_prefix;

	if (out_paje_file)
	fprintf(out_paje_file, "10       %f     S      %s%"PRIu64"      D\n",
			get_event_time_stamp(ev, options), prefix, ev->param[0]);
}

static void handle_worker_deinit_end(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	char *prefix = options->file_prefix;

	if (out_paje_file)
	fprintf(out_paje_file, "8       %f	%s%"PRIu64"	T\n",
			get_event_time_stamp(ev, options), prefix, ev->param[1]);
}

static void create_paje_state_if_not_found(char *name, struct starpu_fxt_options *options)
{
	struct _starpu_symbol_name *itor;
	for (itor = _starpu_symbol_name_list_begin(symbol_list);
		itor != _starpu_symbol_name_list_end(symbol_list);
		itor = _starpu_symbol_name_list_next(itor))
	{
		if (!strcmp(name, itor->name))
		{
			/* we found an entry */
			return;
		}
	}

	/* it's the first time ... */
	struct _starpu_symbol_name *entry = _starpu_symbol_name_new();
	entry->name = malloc(strlen(name));
	strcpy(entry->name, name);

	_starpu_symbol_name_list_push_front(symbol_list, entry);

	/* choose some colour ... that's disguting yes */
	unsigned hash_symbol_red = get_colour_symbol_red(name);
	unsigned hash_symbol_green = get_colour_symbol_green(name);
	unsigned hash_symbol_blue = get_colour_symbol_blue(name);

	uint32_t hash_sum = hash_symbol_red + hash_symbol_green + hash_symbol_blue;

	float red, green, blue;
	if (options->per_task_colour)
	{
		red = (1.0f * hash_symbol_red) / hash_sum;
		green = (1.0f * hash_symbol_green) / hash_sum;
		blue = (1.0f * hash_symbol_blue) / hash_sum;
	}
	else
	{
		/* Use the hardcoded value for execution mode */
		red = 0.0f;
		green = 0.6f;
		blue = 0.4f;
	}

	/* create the Paje state */
	if (out_paje_file)
	fprintf(out_paje_file, "6       %s       S       %s \"%f %f %f\" \n", name, name, red, green, blue);
}


static void handle_start_codelet_body(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	worker = find_worker_id(ev->param[1]);

	if (worker < 0) return;

	char *prefix = options->file_prefix;

	unsigned long has_name = ev->param[2];
	char *name = has_name?(char *)&ev->param[3]:"unknown";

	snprintf(last_codelet_symbol[worker], 128, "%s", name);

	float start_codelet_time = get_event_time_stamp(ev, options);
	last_codelet_start[worker] = start_codelet_time;

	create_paje_state_if_not_found(name, options);

	if (out_paje_file)
	fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      %s\n", start_codelet_time, prefix, ev->param[1], name);
}

static long dumped_codelets_count;
static struct starpu_fxt_codelet_event *dumped_codelets;

static void handle_end_codelet_body(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	worker = find_worker_id(ev->param[4]);
	if (worker < 0) return;

	char *prefix = options->file_prefix;

	float end_codelet_time = get_event_time_stamp(ev, options);

	size_t codelet_size = ev->param[1];
	uint32_t codelet_hash = ev->param[2];

	if (out_paje_file)
	fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      B\n", end_codelet_time, prefix, ev->param[3]);

	float codelet_length = (end_codelet_time - last_codelet_start[worker]);

	update_accumulated_time(worker, 0.0, codelet_length, end_codelet_time, 0);

	if (distrib_time)
	fprintf(distrib_time, "%s\t%s%d\t%ld\t%"PRIx32"\t%f\n", last_codelet_symbol[worker],
				prefix, worker, codelet_size, codelet_hash, codelet_length);

	if (options->dumped_codelets)
	{
		enum starpu_perf_archtype archtype = ev->param[3];

		dumped_codelets_count++;
		dumped_codelets = realloc(dumped_codelets, dumped_codelets_count*sizeof(struct starpu_fxt_codelet_event));

		snprintf(dumped_codelets[dumped_codelets_count - 1].symbol, 256, "%s", last_codelet_symbol[worker]);
		dumped_codelets[dumped_codelets_count - 1].workerid = worker;
		dumped_codelets[dumped_codelets_count - 1].archtype = archtype;

		dumped_codelets[dumped_codelets_count - 1].size = codelet_size;
		dumped_codelets[dumped_codelets_count - 1].hash = codelet_hash;
		dumped_codelets[dumped_codelets_count - 1].time = codelet_length;
	}
}

static void handle_user_event(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	unsigned long code = ev->param[0];

	char *prefix = options->file_prefix;

	worker = find_worker_id(ev->param[1]);
	if (worker < 0)
	{
		if (out_paje_file)
			fprintf(out_paje_file, "9       %f     event      %sp      %lu\n", get_event_time_stamp(ev, options), prefix, code);
	}
	else
	{
		if (out_paje_file)
			fprintf(out_paje_file, "9       %f     event      %s%"PRIu64"      %lu\n", get_event_time_stamp(ev, options), prefix, ev->param[1], code);
	}
}

static void handle_start_callback(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	worker = find_worker_id(ev->param[1]);
	if (worker < 0)
		return;

	if (out_paje_file)
		fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      C\n", get_event_time_stamp(ev, options), options->file_prefix, ev->param[1] );
}

static void handle_end_callback(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	worker = find_worker_id(ev->param[1]);
	if (worker < 0)
		return;

	if (out_paje_file)
		fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      B\n", get_event_time_stamp(ev, options), options->file_prefix, ev->param[1] );
}

static void handle_worker_status(struct fxt_ev_64 *ev, struct starpu_fxt_options *options, const char *newstatus)
{
	int worker;
	worker = find_worker_id(ev->param[1]);
	if (worker < 0)
		return;

	if (out_paje_file)
		fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      %s\n",
			get_event_time_stamp(ev, options), options->file_prefix, ev->param[1], newstatus);
}

static double last_sleep_start[STARPU_NMAXWORKERS];

static void handle_start_sleep(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	worker = find_worker_id(ev->param[0]);
	if (worker < 0) return;

	float start_sleep_time = get_event_time_stamp(ev, options);
	last_sleep_start[worker] = start_sleep_time;

	if (out_paje_file)
		fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      Sl\n",
			get_event_time_stamp(ev, options), options->file_prefix, ev->param[0]);
}

static void handle_end_sleep(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int worker;
	worker = find_worker_id(ev->param[0]);
	if (worker < 0) return;

	float end_sleep_timestamp = get_event_time_stamp(ev, options);

	if (out_paje_file)
		fprintf(out_paje_file, "10       %f	S      %s%"PRIu64"      B\n",
			end_sleep_timestamp, options->file_prefix, ev->param[0]);

	double sleep_length = end_sleep_timestamp - last_sleep_start[worker];

	update_accumulated_time(worker, sleep_length, 0.0, end_sleep_timestamp, 0);
}

static void handle_data_copy(void)
{
}

static void handle_start_driver_copy(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	unsigned src = ev->param[0];
	unsigned dst = ev->param[1];
	unsigned size = ev->param[2];
	unsigned comid = ev->param[3];

	char *prefix = options->file_prefix;

	if (!options->no_bus)
	{
		if (out_paje_file)
		{
			fprintf(out_paje_file, "10       %f     MS      %sMEMNODE%u      Co\n", get_event_time_stamp(ev, options), prefix, dst);
			fprintf(out_paje_file, "18       %f	L      %sp	%u	%sMEMNODE%u	com_%u\n", get_event_time_stamp(ev, options), prefix, size, prefix, src, comid);
		}

		/* create a structure to store the start of the communication, this will be matched later */
		struct _starpu_communication *com = _starpu_communication_new();
		com->comid = comid;
		com->comm_start = get_event_time_stamp(ev, options);

		com->src_node = src;
		com->dst_node = dst;

		_starpu_communication_list_push_back(communication_list, com);
	}

}

static void handle_end_driver_copy(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	unsigned dst = ev->param[1];
	unsigned size = ev->param[2];
	unsigned comid = ev->param[3];

	char *prefix = options->file_prefix;

	if (!options->no_bus)
	{
		if (out_paje_file)
		{
			fprintf(out_paje_file, "10       %f     MS      %sMEMNODE%u      No\n", get_event_time_stamp(ev, options), prefix, dst);
			fprintf(out_paje_file, "19       %f	L      %sp	%u	%sMEMNODE%u	com_%u\n", get_event_time_stamp(ev, options), prefix, size, prefix, dst, comid);
		}

		/* look for a data transfer to match */
		struct _starpu_communication *itor;
		for (itor = _starpu_communication_list_begin(communication_list);
			itor != _starpu_communication_list_end(communication_list);
			itor = _starpu_communication_list_next(itor))
		{
			if (itor->comid == comid)
			{
				float comm_end = get_event_time_stamp(ev, options);
				float bandwidth = (float)((0.001*size)/(comm_end - itor->comm_start));

				itor->bandwidth = bandwidth;

				struct _starpu_communication *com = _starpu_communication_new();
				com->comid = comid;
				com->comm_start = get_event_time_stamp(ev, options);
				com->bandwidth = -bandwidth;

				com->src_node = itor->src_node;
				com->dst_node = itor->dst_node;

				_starpu_communication_list_push_back(communication_list, com);

				break;
			}
		}
	}
}

static void handle_start_driver_copy_async(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	unsigned dst = ev->param[1];

	char *prefix = options->file_prefix;

	if (!options->no_bus)
		if (out_paje_file)
			fprintf(out_paje_file, "10       %f     MS      %sMEMNODE%u      CoA\n", get_event_time_stamp(ev, options), prefix, dst);

}

static void handle_end_driver_copy_async(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	unsigned dst = ev->param[1];

	char *prefix = options->file_prefix;

	if (!options->no_bus)
		if (out_paje_file)
			fprintf(out_paje_file, "10       %f     MS      %sMEMNODE%u      Co\n", get_event_time_stamp(ev, options), prefix, dst);
}

static void handle_memnode_event(struct fxt_ev_64 *ev, struct starpu_fxt_options *options, const char *eventstr)
{
	unsigned memnode = ev->param[0];

	if (out_paje_file)
	fprintf(out_paje_file, "10       %f     MS      %sMEMNODE%u      %s\n",
		get_event_time_stamp(ev, options), options->file_prefix, memnode, eventstr);
}

/*
 *	Number of task submitted to the scheduler
 */
static int curq_size = 0;

static void handle_job_push(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	float current_timestamp = get_event_time_stamp(ev, options);

	curq_size++;

	if (!options->no_counter && out_paje_file)
		fprintf(out_paje_file, "13       %f ntask %ssched %f\n", current_timestamp, options->file_prefix, (float)curq_size);

	if (activity_file)
	fprintf(activity_file, "cnt_ready\t%f\t%d\n", current_timestamp, curq_size);
}

static void handle_job_pop(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	float current_timestamp = get_event_time_stamp(ev, options);

	curq_size--;

	if (!options->no_counter && out_paje_file)
		fprintf(out_paje_file, "13       %f ntask %ssched %f\n", current_timestamp, options->file_prefix, (float)curq_size);

	if (activity_file)
		fprintf(activity_file, "cnt_ready\t%f\t%d\n", current_timestamp, curq_size);
}

static
void handle_update_task_cnt(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	float current_timestamp = get_event_time_stamp(ev, options);
	unsigned long nsubmitted = ev->param[0];
	if (activity_file)
	fprintf(activity_file, "cnt_submitted\t%f\t%lu\n", current_timestamp, nsubmitted);
}

static void handle_codelet_tag_deps(struct fxt_ev_64 *ev)
{
	uint64_t child;
	uint64_t father;

	child = ev->param[0];
	father = ev->param[1];

	_starpu_fxt_dag_add_tag_deps(child, father);
}

static void handle_task_deps(struct fxt_ev_64 *ev)
{
	unsigned long dep_prev = ev->param[0];
	unsigned long dep_succ = ev->param[1];

	/* There is a dependency between both job id : dep_prev -> dep_succ */
	_starpu_fxt_dag_add_task_deps(dep_prev, dep_succ);
}

static void handle_task_done(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	unsigned long job_id;
	job_id = ev->param[0];

	unsigned long has_name = ev->param[3];
	char *name = has_name?(char *)&ev->param[4]:"unknown";

        int worker;
        worker = find_worker_id(ev->param[1]);

	const char *colour;
	char buffer[32];
	if (options->per_task_colour)
	{
		snprintf(buffer, 32, "#%x%x%x",
			 get_colour_symbol_red(name)/4,
			 get_colour_symbol_green(name)/4,
			 get_colour_symbol_blue(name)/4);
		colour = &buffer[0];
	}
	else
	{
		colour= (worker < 0)?"#aaaaaa":get_worker_color(worker);
	}

	unsigned exclude_from_dag = ev->param[2];

	if (!exclude_from_dag)
		_starpu_fxt_dag_set_task_done(job_id, name, colour);
}

static void handle_tag_done(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	uint64_t tag_id;
	tag_id = ev->param[0];

	unsigned long has_name = ev->param[2];
	char *name = has_name?(char *)&ev->param[3]:"unknown";

        int worker;
        worker = find_worker_id(ev->param[1]);

	const char *colour;
	char buffer[32];
	if (options->per_task_colour)
	{
		snprintf(buffer, 32, "%.4f,%.4f,%.4f",
			 get_colour_symbol_red(name)/1024.0,
			 get_colour_symbol_green(name)/1024.0,
			 get_colour_symbol_blue(name)/1024.0);
		colour = &buffer[0];
	}
	else
	{
		colour= (worker < 0)?"0.0,0.0,0.0":get_worker_color(worker);
	}

	_starpu_fxt_dag_set_tag_done(tag_id, colour);
}

static void handle_mpi_barrier(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int rank = ev->param[0];

	STARPU_ASSERT(rank == options->file_rank);

	/* Add an event in the trace */
	if (out_paje_file)
	fprintf(out_paje_file, "9       %f     event      %sp      %d\n", get_event_time_stamp(ev, options), options->file_prefix, rank);
}

static void handle_mpi_isend(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int dest = ev->param[0];
	int mpi_tag = ev->param[1];
	size_t size = ev->param[2];
	float date = get_event_time_stamp(ev, options);

	_starpu_fxt_mpi_add_send_transfer(options->file_rank, dest, mpi_tag, size, date);
}

static void handle_mpi_irecv_end(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int src = ev->param[0];
	int mpi_tag = ev->param[1];
	float date = get_event_time_stamp(ev, options);

	_starpu_fxt_mpi_add_recv_transfer(src, options->file_rank, mpi_tag, date);
}

static void handle_set_profiling(struct fxt_ev_64 *ev, struct starpu_fxt_options *options)
{
	int status = ev->param[0];

	if (activity_file)
	fprintf(activity_file, "set_profiling\t%f\t%d\n", get_event_time_stamp(ev, options), status);
}

static void handle_task_wait_for_all(void)
{
	_starpu_fxt_dag_add_sync_point();
}

static
void _starpu_fxt_display_bandwidth(struct starpu_fxt_options *options)
{
	float current_bandwidth_per_node[STARPU_MAXNODES] = {0.0};

	char *prefix = options->file_prefix;

	struct _starpu_communication*itor;
	for (itor = _starpu_communication_list_begin(communication_list);
		itor != _starpu_communication_list_end(communication_list);
		itor = _starpu_communication_list_next(itor))
	{
		current_bandwidth_per_node[itor->src_node] +=  itor->bandwidth;
		if (out_paje_file)
		fprintf(out_paje_file, "13  %f bw %sMEMNODE%u %f\n",
				itor->comm_start, prefix, itor->src_node, current_bandwidth_per_node[itor->src_node]);

		current_bandwidth_per_node[itor->dst_node] +=  itor->bandwidth;
		if (out_paje_file)
		fprintf(out_paje_file, "13  %f bw %sMEMNODE%u %f\n",
				itor->comm_start, prefix, itor->dst_node, current_bandwidth_per_node[itor->dst_node]);
	}
}

/*
 *	Public functions
 */

static
void starpu_fxt_parse_new_file(char *filename_in, struct starpu_fxt_options *options)
{
	/* Open the trace file */
	int fd_in;
	fd_in = open(filename_in, O_RDONLY);
	if (fd_in < 0)
	{
	        perror("open failed :");
	        exit(-1);
	}

	static fxt_t fut;
	fut = fxt_fdopen(fd_in);
	if (!fut)
	{
	        perror("fxt_fdopen :");
	        exit(-1);
	}

	fxt_blockev_t block;
	block = fxt_blockev_enter(fut);

	/* create a htable to identify each worker(tid) */
	hcreate(STARPU_NMAXWORKERS);

	symbol_list = _starpu_symbol_name_list_new();
	communication_list = _starpu_communication_list_new();

	char *prefix = options->file_prefix;

	/* TODO starttime ...*/
	/* create the "program" container */
	if (out_paje_file)
	{
		fprintf(out_paje_file, "7      0.0 %sp      P      MPIroot       program%s \n", prefix, prefix);
		/* create a variable with the number of tasks */
		if (!options->no_counter)
		{
			fprintf(out_paje_file, "7     %f    %ssched   Sc    %sp     scheduler \n", 0.0, prefix, prefix);
			fprintf(out_paje_file, "13    0.0    ntask %ssched 0.0\n", prefix);
		}
	}

	struct fxt_ev_64 ev;
	while(1)
	{
		int ret = fxt_next_ev(block, FXT_EV_TYPE_64, (struct fxt_ev *)&ev);
		if (ret != FXT_EV_OK)
		{
			break;
		}

		switch (ev.code)
		{
			case _STARPU_FUT_WORKER_INIT_START:
				handle_worker_init_start(&ev, options);
				break;

			case _STARPU_FUT_WORKER_INIT_END:
				handle_worker_init_end(&ev, options);
				break;

			case _STARPU_FUT_NEW_MEM_NODE:
				handle_new_mem_node(&ev, options);
				break;

			/* detect when the workers were idling or not */
			case _STARPU_FUT_START_CODELET_BODY:
				handle_start_codelet_body(&ev, options);
				break;
			case _STARPU_FUT_END_CODELET_BODY:
				handle_end_codelet_body(&ev, options);
				break;

			case _STARPU_FUT_START_CALLBACK:
				handle_start_callback(&ev, options);
				break;
			case _STARPU_FUT_END_CALLBACK:
				handle_end_callback(&ev, options);
				break;

			case _STARPU_FUT_UPDATE_TASK_CNT:
				handle_update_task_cnt(&ev, options);
				break;

			/* monitor stack size */
			case _STARPU_FUT_JOB_PUSH:
				handle_job_push(&ev, options);
				break;
			case _STARPU_FUT_JOB_POP:
				handle_job_pop(&ev, options);
				break;

			/* check the memory transfer overhead */
			case _STARPU_FUT_START_FETCH_INPUT:
				handle_worker_status(&ev, options, "Fi");
				break;

			case _STARPU_FUT_START_PUSH_OUTPUT:
				handle_worker_status(&ev, options, "Po");
				break;

			case _STARPU_FUT_START_PROGRESS:
				handle_worker_status(&ev, options, "P");
				break;

			case _STARPU_FUT_END_FETCH_INPUT:
			case _STARPU_FUT_END_PROGRESS:
			case _STARPU_FUT_END_PUSH_OUTPUT:
				handle_worker_status(&ev, options, "B");
				break;

			case _STARPU_FUT_WORKER_SLEEP_START:
				handle_start_sleep(&ev, options);
				break;

			case _STARPU_FUT_WORKER_SLEEP_END:
				handle_end_sleep(&ev, options);
				break;

			case _STARPU_FUT_TAG:
				/* XXX */
				break;

			case _STARPU_FUT_TAG_DEPS:
				handle_codelet_tag_deps(&ev);
				break;

			case _STARPU_FUT_TASK_DEPS:
				handle_task_deps(&ev);
				break;

			case _STARPU_FUT_TASK_DONE:
				handle_task_done(&ev, options);
				break;

			case _STARPU_FUT_TAG_DONE:
				handle_tag_done(&ev, options);
				break;

			case _STARPU_FUT_DATA_COPY:
				if (!options->no_bus)
				handle_data_copy();
				break;

			case _STARPU_FUT_START_DRIVER_COPY:
				if (!options->no_bus)
				handle_start_driver_copy(&ev, options);
				break;

			case _STARPU_FUT_END_DRIVER_COPY:
				if (!options->no_bus)
				handle_end_driver_copy(&ev, options);
				break;

			case _STARPU_FUT_START_DRIVER_COPY_ASYNC:
				if (!options->no_bus)
				handle_start_driver_copy_async(&ev, options);
				break;

			case _STARPU_FUT_END_DRIVER_COPY_ASYNC:
				if (!options->no_bus)
				handle_end_driver_copy_async(&ev, options);
				break;

			case _STARPU_FUT_WORK_STEALING:
				/* XXX */
				break;

			case _STARPU_FUT_WORKER_DEINIT_START:
				handle_worker_deinit_start(&ev, options);
				break;

			case _STARPU_FUT_WORKER_DEINIT_END:
				handle_worker_deinit_end(&ev, options);
				break;

			case _STARPU_FUT_START_ALLOC:
				if (!options->no_bus)
				handle_memnode_event(&ev, options, "A");
				break;

			case _STARPU_FUT_START_ALLOC_REUSE:
				if (!options->no_bus)
				handle_memnode_event(&ev, options, "Ar");
				break;

			case _STARPU_FUT_START_MEMRECLAIM:
				handle_memnode_event(&ev, options, "R");
				break;

			case _STARPU_FUT_END_ALLOC:
			case _STARPU_FUT_END_ALLOC_REUSE:
			case _STARPU_FUT_END_MEMRECLAIM:
				if (!options->no_bus)
				handle_memnode_event(&ev, options, "No");
				break;

			case _STARPU_FUT_USER_EVENT:
				handle_user_event(&ev, options);
				break;

			case FUT_MPI_BARRIER:
				handle_mpi_barrier(&ev, options);
				break;

			case FUT_MPI_ISEND:
				handle_mpi_isend(&ev, options);
				break;

			case FUT_MPI_IRECV_END:
				handle_mpi_irecv_end(&ev, options);
				break;

			case _STARPU_FUT_SET_PROFILING:
				handle_set_profiling(&ev, options);
				break;

			case _STARPU_FUT_TASK_WAIT_FOR_ALL:
				handle_task_wait_for_all();
				break;

			default:
#ifdef STARPU_VERBOSE
				fprintf(stderr, "unknown event.. %x at time %llx WITH OFFSET %llx\n",
					(unsigned)ev.code, (long long unsigned)ev.time, (long long unsigned)(ev.time-options->file_offset));
#endif
				break;
		}
	}

	hdestroy();

	/* Close the trace file */
	if (close(fd_in))
	{
	        perror("close failed :");
	        exit(-1);
	}
}

/* Initialize FxT options to default values */
void starpu_fxt_options_init(struct starpu_fxt_options *options)
{
	options->per_task_colour = 0;
	options->no_counter = 0;
	options->no_bus = 0;
	options->ninputfiles = 0;
	options->out_paje_path = "paje.trace";
	options->dag_path = "dag.dot";
	options->distrib_time_path = "distrib.data";
	options->dumped_codelets = NULL;
	options->activity_path = "activity.data";
}

static
void starpu_fxt_distrib_file_init(struct starpu_fxt_options *options)
{
	dumped_codelets_count = 0;
	dumped_codelets = NULL;

	if (options->distrib_time_path)
	{
		distrib_time = fopen(options->distrib_time_path, "w+");
	}
	else
	{
		distrib_time = NULL;
	}
}

static
void starpu_fxt_distrib_file_close(struct starpu_fxt_options *options)
{
	if (distrib_time)
		fclose(distrib_time);

	if (options->dumped_codelets)
	{
		*options->dumped_codelets = dumped_codelets;
		options->dumped_codelets_count = dumped_codelets_count;
	}
}

static
void starpu_fxt_activity_file_init(struct starpu_fxt_options *options)
{
	if (options->activity_path)
		activity_file = fopen(options->activity_path, "w+");
	else
		activity_file = NULL;
}

static
void starpu_fxt_activity_file_close(void)
{
	if (activity_file)
		fclose(activity_file);
}

static
void starpu_fxt_paje_file_init(struct starpu_fxt_options *options)
{
	/* create a new file */
	if (options->out_paje_path)
	{
		out_paje_file = fopen(options->out_paje_path, "w+");
		if (!out_paje_file)
		{
			fprintf(stderr,"error while opening %s\n", options->out_paje_path);
			perror("fopen");
			exit(1);
		}

		_starpu_fxt_write_paje_header(out_paje_file);
	}
	else
	{
		out_paje_file = NULL;
	}
}

static
void starpu_fxt_paje_file_close(void)
{
	if (out_paje_file)
		fclose(out_paje_file);
}

static uint64_t starpu_fxt_find_start_time(char *filename_in)
{
	/* Open the trace file */
	int fd_in;
	fd_in = open(filename_in, O_RDONLY);
	if (fd_in < 0)
	{
	        perror("open failed :");
	        exit(-1);
	}

	static fxt_t fut;
	fut = fxt_fdopen(fd_in);
	if (!fut)
	{
	        perror("fxt_fdopen :");
	        exit(-1);
	}

	fxt_blockev_t block;
	block = fxt_blockev_enter(fut);

	struct fxt_ev_64 ev;

	int ret = fxt_next_ev(block, FXT_EV_TYPE_64, (struct fxt_ev *)&ev);
	STARPU_ASSERT (ret == FXT_EV_OK);

	/* Close the trace file */
	if (close(fd_in))
	{
	        perror("close failed :");
	        exit(-1);
	}
	return (ev.time);
}

void starpu_fxt_generate_trace(struct starpu_fxt_options *options)
{
	_starpu_fxt_dag_init(options->dag_path);
	starpu_fxt_distrib_file_init(options);
	starpu_fxt_activity_file_init(options);

	starpu_fxt_paje_file_init(options);

	if (options->ninputfiles == 1)
	{
		/* we usually only have a single trace */
		uint64_t file_start_time = starpu_fxt_find_start_time(options->filenames[0]);
		options->file_prefix = "";
		options->file_offset = file_start_time;
		options->file_rank = -1;

		starpu_fxt_parse_new_file(options->filenames[0], options);
	}
	else
	{
		unsigned inputfile;

		uint64_t offsets[64];

		/*
		 * Find the trace offsets:
		 *	- If there is no sync point
		 *		psi_k(x) = x - start_k
		 *	- If there is a sync point sync_k
		 *		psi_k(x) = x - sync_k + M
		 *		where M = max { sync_i - start_i | there exists sync_i}
		 * More generally:
		 *	- psi_k(x) = x - offset_k
		 */

		int unique_keys[64];
		int rank_k[64];
		uint64_t start_k[64];
		uint64_t sync_k[64];
		unsigned sync_k_exists[64];
		uint64_t M = 0;

		unsigned found_one_sync_point = 0;
		int key = 0;
		unsigned display_mpi = 0;

		/* Compute all start_k */
		for (inputfile = 0; inputfile < options->ninputfiles; inputfile++)
		{
			uint64_t file_start = starpu_fxt_find_start_time(options->filenames[inputfile]);
			start_k[inputfile] = file_start;
		}

		/* Compute all sync_k if they exist */
		for (inputfile = 0; inputfile < options->ninputfiles; inputfile++)
		{
			int ret = _starpu_fxt_mpi_find_sync_point(options->filenames[inputfile],
						&sync_k[inputfile],
						&unique_keys[inputfile],
						&rank_k[inputfile]);
			if (ret == -1)
			{
				/* There was no sync point, we assume there is no offset */
				sync_k_exists[inputfile] = 0;
			}
			else
			{
				if (!found_one_sync_point)
				{
					key = unique_keys[inputfile];
					display_mpi = 1;
					found_one_sync_point = 1;
				}
				else
				{
					if (key != unique_keys[inputfile])
					{
						fprintf(stderr, "Warning: traces are coming from different run so we will not try to display MPI communications.\n");
						display_mpi = 0;
					}
				}


				STARPU_ASSERT(sync_k[inputfile] >= start_k[inputfile]);

				sync_k_exists[inputfile] = 1;

				uint64_t diff = sync_k[inputfile] - start_k[inputfile];
				if (diff > M)
					M = diff;
			}
		}

		/* Compute the offset */
		for (inputfile = 0; inputfile < options->ninputfiles; inputfile++)
		{
			offsets[inputfile] = sync_k_exists[inputfile]?
						(sync_k[inputfile]-M):start_k[inputfile];
		}

		/* generate the Paje trace for the different files */
		for (inputfile = 0; inputfile < options->ninputfiles; inputfile++)
		{
			int filerank = rank_k[inputfile];

#ifdef STARPU_VERBOSE
			fprintf(stderr, "Handle file %s (rank %d)\n", options->filenames[inputfile], filerank);
#endif

			char file_prefix[32];
			snprintf(file_prefix, 32, "mpi_%d_", filerank);

			options->file_prefix = file_prefix;
			options->file_offset = offsets[inputfile];
			options->file_rank = filerank;

			starpu_fxt_parse_new_file(options->filenames[inputfile], options);
		}

		/* display the MPI transfers if possible */
		if (display_mpi)
			_starpu_fxt_display_mpi_transfers(options, rank_k, out_paje_file);
	}

	_starpu_fxt_display_bandwidth(options);

	/* close the different files */
	starpu_fxt_paje_file_close();
	starpu_fxt_activity_file_close();
	starpu_fxt_distrib_file_close(options);

	_starpu_fxt_dag_terminate();

	options->nworkers = nworkers;
}
#endif // STARPU_USE_FXT
