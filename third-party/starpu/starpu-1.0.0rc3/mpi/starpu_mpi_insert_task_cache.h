/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
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

#define _STARPU_MPI_CLEAR_SENT_DATA     0
#define _STARPU_MPI_CLEAR_RECEIVED_DATA 1

extern struct starpu_htbl32_node **sent_data;
extern struct starpu_htbl32_node **received_data;

void _starpu_mpi_clear_cache_request(starpu_data_handle_t data_handle, int rank, int mode);
