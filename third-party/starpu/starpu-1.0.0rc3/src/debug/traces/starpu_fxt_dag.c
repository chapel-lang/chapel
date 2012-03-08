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

#include <stdio.h>
#include <stdint.h>
#include <common/config.h>

#ifdef STARPU_USE_FXT

#include "starpu_fxt.h"

static FILE *out_file;
static unsigned cluster_cnt;

void _starpu_fxt_dag_init(char *out_path)
{
	if (!out_path)
	{
		out_file = NULL;
		return;
	}

	/* create a new file */
	out_file = fopen(out_path, "w+");
	if (!out_file)
	{
		fprintf(stderr,"error while opening %s\n", out_path);
		perror("fopen");
		exit(1);
	}
	cluster_cnt = 0;

	fprintf(out_file, "digraph G {\n");
	fprintf(out_file, "\tcolor=white\n");
	fprintf(out_file, "\trankdir=LR;\n");

	/* Create a new cluster */
	fprintf(out_file, "subgraph cluster_%u {\n", cluster_cnt);
	fprintf(out_file, "\tcolor=black;\n");
}

void _starpu_fxt_dag_terminate(void)
{
	if (!out_file)
		return;

	/* Close the last cluster */
	fprintf(out_file, "}\n");
	/* Close the graph */
	fprintf(out_file, "}\n");
	fclose(out_file);
}

void _starpu_fxt_dag_add_tag_deps(uint64_t child, uint64_t father)
{
	if (out_file)
		fprintf(out_file, "\t \"tag_%llx\"->\"tag_%llx\"\n",
			(unsigned long long)father, (unsigned long long)child);
}

void _starpu_fxt_dag_add_task_deps(unsigned long dep_prev, unsigned long dep_succ)
{
	if (out_file)
		fprintf(out_file, "\t \"task_%lx\"->\"task_%lx\"\n", dep_prev, dep_succ);
}

void _starpu_fxt_dag_set_tag_done(uint64_t tag, const char *color)
{
	if (out_file)
		fprintf(out_file, "\t \"tag_%llx\" [ style=filled, label=\"\", color=\"%s\"]\n",
			(unsigned long long)tag, color);
}

void _starpu_fxt_dag_set_task_done(unsigned long job_id, const char *label, const char *color)
{
	if (out_file)
		fprintf(out_file, "\t \"task_%lx\" [ style=filled, label=\"%s\", color=\"%s\"]\n", job_id, label, color);
}

void _starpu_fxt_dag_add_sync_point(void)
{
	if (!out_file)
		return;

	/* Close the previous cluster */
	fprintf(out_file, "}\n");

	cluster_cnt++;

	/* Create a new cluster */
	fprintf(out_file, "subgraph cluster_%u {\n", cluster_cnt);
	fprintf(out_file, "\tcolor=black;\n");
}

#endif /* STARPU_USE_FXT */
