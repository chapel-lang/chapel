/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  Université Bordeaux 1
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

#ifndef __STARPU_DATA_CPY_H__
#define __STARPU_DATA_CPY_H__

#include <starpu.h>

int _starpu_data_cpy(starpu_data_handle_t dst_handle, starpu_data_handle_t src_handle,
			int asynchronous, void (*callback_func)(void*), void *callback_arg,
			int reduction, struct starpu_task *reduction_dep_task);

#endif // __STARPU_DATA_CPY_H__

