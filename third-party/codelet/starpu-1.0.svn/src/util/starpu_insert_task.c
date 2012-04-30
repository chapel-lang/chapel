/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

/* This file provides an interface that is very similar to that of the Quark
 * scheduler from the PLASMA project (see http://icl.cs.utk.edu/plasma/). */

#include <starpu.h>
#include <common/config.h>
#include <stdarg.h>
#include <util/starpu_insert_task_utils.h>

void starpu_codelet_pack_args(char **arg_buffer, size_t *arg_buffer_size, ...)
{
	va_list varg_list;

	/* Compute the size */
	va_start(varg_list, arg_buffer_size);
        *arg_buffer_size = _starpu_insert_task_get_arg_size(varg_list);

	va_start(varg_list, arg_buffer_size);
	_starpu_codelet_pack_args(*arg_buffer_size, arg_buffer, varg_list);
}

void starpu_codelet_unpack_args(void *_cl_arg, ...)
{
	unsigned char *cl_arg = (unsigned char *) _cl_arg;
	unsigned current_arg_offset = 0;
	va_list varg_list;

	STARPU_ASSERT(cl_arg);
	va_start(varg_list, _cl_arg);

	/* We fill the different pointers with the appropriate arguments */
	unsigned char nargs = cl_arg[0];
	current_arg_offset += sizeof(char);

	unsigned arg;
	for (arg = 0; arg < nargs; arg++)
	{
		void *argptr = va_arg(varg_list, void *);
		size_t arg_size = *(size_t *)&cl_arg[current_arg_offset];
		current_arg_offset += sizeof(size_t);

		memcpy(argptr, &cl_arg[current_arg_offset], arg_size);
		current_arg_offset += arg_size;
	}

	va_end(varg_list);
	free(cl_arg);
}

int starpu_insert_task(struct starpu_codelet *cl, ...)
{
	va_list varg_list;

	/* Compute the size */
	size_t arg_buffer_size = 0;
	va_start(varg_list, cl);
        arg_buffer_size = _starpu_insert_task_get_arg_size(varg_list);

	va_start(varg_list, cl);
	char *arg_buffer;
	_starpu_codelet_pack_args(arg_buffer_size, &arg_buffer, varg_list);

	va_start(varg_list, cl);
        struct starpu_task *task = starpu_task_create();
	int ret = _starpu_insert_task_create_and_submit(arg_buffer, cl, &task, varg_list);

	if (ret == -ENODEV)
	{
		task->destroy = 0;
		starpu_task_destroy(task);
	}
        return ret;
}
