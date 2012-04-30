/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
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

#ifndef __STARPU_MPI_DATATYPE_H__
#define __STARPU_MPI_DATATYPE_H__

#include <starpu_mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

int starpu_mpi_handle_to_datatype(starpu_data_handle_t data_handle, MPI_Datatype *datatype);
void *starpu_mpi_handle_to_ptr(starpu_data_handle_t data_handle);

#ifdef __cplusplus
}
#endif

#endif // __STARPU_MPI_DATATYPE_H__
