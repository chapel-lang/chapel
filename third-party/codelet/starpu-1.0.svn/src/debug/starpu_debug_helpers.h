/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#ifndef __STARPU_DEBUG_HELPERS_H__
#define __STARPU_DEBUG_HELPERS_H__

#include <starpu.h>
#include <starpu_config.h>
#include <starpu_util.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Perform a ping pong between the two memory nodes */
void _starpu_benchmark_ping_pong(starpu_data_handle_t handle, unsigned node0, unsigned node1, unsigned niter);

/* Display the size of different data structures */
void _starpu_debug_display_structures_size(void);

#ifdef __cplusplus
}
#endif

#endif // __STARPU_DEBUG_HELPERS_H__
