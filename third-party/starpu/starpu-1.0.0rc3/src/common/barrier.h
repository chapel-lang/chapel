/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
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

#ifndef __COMMON_BARRIER_H__
#define __COMMON_BARRIER_H__

#include <pthread.h>

struct _starpu_barrier
{
	int count;
	int reached_start;
	int reached_exit;
	pthread_mutex_t mutex;
	pthread_mutex_t mutex_exit;
	pthread_cond_t cond;
};

int _starpu_barrier_init(struct _starpu_barrier *barrier, int count);

int _starpu_barrier_destroy(struct _starpu_barrier *barrier);

int _starpu_barrier_wait(struct _starpu_barrier *barrier);

#if !defined(PTHREAD_BARRIER_SERIAL_THREAD)
#  define PTHREAD_BARRIER_SERIAL_THREAD -1
#  define pthread_barrier_t struct _starpu_barrier
#  define pthread_barrier_init(b,a,c) _starpu_barrier_init(b, c)
#  define pthread_barrier_destroy(b) _starpu_barrier_destroy(b)
#  define pthread_barrier_wait(b) _starpu_barrier_wait(b)
#endif /* !PTHREAD_BARRIER_SERIAL_THREAD */

#endif // __COMMON_BARRIER_H__
