/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

#ifndef __STARPU_FXT_H__
#define __STARPU_FXT_H__

#include <starpu_perfmodel.h>

#define STARPU_FXT_MAX_FILES	64

struct starpu_fxt_codelet_event
{
	char symbol[256]; /* name of the codelet */
	int workerid;
	enum starpu_perf_archtype archtype;
	uint32_t hash;
	size_t size;
	float time;
};

struct starpu_fxt_options
{
	unsigned per_task_colour;
	unsigned no_counter;
	unsigned no_bus;
	unsigned ninputfiles;
	char *filenames[STARPU_FXT_MAX_FILES];
	char *out_paje_path;
	char *distrib_time_path;
	char *activity_path;
	char *dag_path;

	/* In case we are going to gather multiple traces (eg in the case of
	 * MPI processes), we may need to prefix the name of the containers. */
	char *file_prefix;
	uint64_t file_offset;
	int file_rank;

	/*
	 *	Output parameters
	 */

	char worker_names[STARPU_NMAXWORKERS][256]; 
	enum starpu_perf_archtype worker_archtypes[STARPU_NMAXWORKERS];
	int nworkers;

	/* In case we want to dump the list of codelets to an external tool */
	struct starpu_fxt_codelet_event **dumped_codelets;
	long dumped_codelets_count;
};

void starpu_fxt_options_init(struct starpu_fxt_options *options);
void starpu_fxt_generate_trace(struct starpu_fxt_options *options);

#endif /* __STARPU_FXT_H__ */
