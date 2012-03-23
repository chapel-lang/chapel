/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#ifndef __STARPU_MPI_FXT_H__
#define __STARPU_MPI_FXT_H__

#include <starpu.h>
#include <common/config.h>
#include <common/fxt.h>

#define FUT_MPI_BARRIER		0x5201
#define FUT_MPI_ISEND		0x5202
#define FUT_MPI_IRECV_END	0x5203

#ifdef STARPU_USE_FXT
#define TRACE_MPI_BARRIER(rank, worldsize, key)	\
	FUT_DO_PROBE4(FUT_MPI_BARRIER, (rank), (worldsize), (key), syscall(SYS_gettid));
#define TRACE_MPI_ISEND(dest, mpi_tag, size)	\
	FUT_DO_PROBE4(FUT_MPI_ISEND, (dest), (mpi_tag), (size), syscall(SYS_gettid));
#define TRACE_MPI_IRECV_END(src, mpi_tag)	\
	FUT_DO_PROBE3(FUT_MPI_IRECV_END, (src), (mpi_tag), syscall(SYS_gettid));
#define TRACE
#else
#define TRACE_MPI_BARRIER(a, b, c)	do {} while(0);
#define TRACE_MPI_ISEND(a, b, c)	do {} while(0);
#define TRACE_MPI_IRECV_END(a, b)	do {} while(0);
#endif



#endif // __STARPU_MPI_FXT_H__
