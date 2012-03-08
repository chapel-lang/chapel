/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
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

#ifndef TIMING_H
#define TIMING_H

/*
 * _starpu_timing_init must be called prior to using any of these timing
 * functions.
 */

#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <common/config.h>
#include <starpu.h>

void _starpu_timing_init(void);
void _starpu_clock_gettime(struct timespec *ts);

#endif /* TIMING_H */


