/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
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

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <common/config.h>
#include <core/workers.h>

/* Create a file that will contain StarPU's log */
void _starpu_open_debug_logfile(void);

/* Close StarPU's log file */
void _starpu_close_debug_logfile(void);

/* Write into StarPU's log file */
void _starpu_print_to_logfile(const char *format, ...);

/* Tell gdb whether FXT is compiled in or not */
extern int _starpu_use_fxt;

#endif // __DEBUG_H__
