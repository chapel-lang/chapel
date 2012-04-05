/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
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

#ifndef __STARPU_INSERT_TASK_UTILS_H__
#define __STARPU_INSERT_TASK_UTILS_H__

#include <stdlib.h>
#include <stdarg.h>
#include <starpu.h>

size_t _starpu_insert_task_get_arg_size(va_list varg_list);
int _starpu_codelet_pack_args(size_t arg_buffer_size, char **arg_buffer, va_list varg_list);
int _starpu_insert_task_create_and_submit(char *arg_buffer, struct starpu_codelet *cl, struct starpu_task **task, va_list varg_list);

#endif // __STARPU_INSERT_TASK_UTILS_H__

