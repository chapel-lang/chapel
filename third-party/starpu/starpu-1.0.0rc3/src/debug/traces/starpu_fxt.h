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

#ifndef __STARPU__FXT_H__
#define __STARPU__FXT_H__

#include <starpu.h>
#include <starpu_config.h>
#include <common/config.h>

#ifdef STARPU_USE_FXT

#include <search.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <common/fxt.h>
#include <common/list.h>
#include "../mpi/starpu_mpi_fxt.h"
#include <starpu.h>
#include "../../../include/starpu_fxt.h"

void _starpu_fxt_dag_init(char *dag_filename);
void _starpu_fxt_dag_terminate(void);
void _starpu_fxt_dag_add_tag_deps(uint64_t child, uint64_t father);
void _starpu_fxt_dag_set_tag_done(uint64_t tag, const char *color);
void _starpu_fxt_dag_add_task_deps(unsigned long dep_prev, unsigned long dep_succ);
void _starpu_fxt_dag_set_task_done(unsigned long job_id, const char *label, const char *color);
void _starpu_fxt_dag_add_sync_point(void);

/*
 *	MPI
 */

int _starpu_fxt_mpi_find_sync_point(char *filename_in, uint64_t *offset, int *key, int *rank);
void _starpu_fxt_mpi_add_send_transfer(int src, int dst, int mpi_tag, size_t size, float date);
void _starpu_fxt_mpi_add_recv_transfer(int src, int dst, int mpi_tag, float date);
void _starpu_fxt_display_mpi_transfers(struct starpu_fxt_options *options, int *ranks, FILE *out_paje_file);

void _starpu_fxt_write_paje_header(FILE *file);

#endif // STARPU_USE_FXT

#endif // __STARPU__FXT_H__
