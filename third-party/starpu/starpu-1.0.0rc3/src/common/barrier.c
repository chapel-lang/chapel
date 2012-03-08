/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010,2011  Centre National de la Recherche Scientifique
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

#include <common/barrier.h>
#include <common/utils.h>

int _starpu_barrier_init(struct _starpu_barrier *barrier, int count)
{
	barrier->count = count;
	barrier->reached_start = 0;
	barrier->reached_exit = 0;
	_STARPU_PTHREAD_MUTEX_INIT(&barrier->mutex, NULL);
	_STARPU_PTHREAD_MUTEX_INIT(&barrier->mutex_exit, NULL);
	_STARPU_PTHREAD_COND_INIT(&barrier->cond, NULL);
	return 0;
}

static
int _starpu_barrier_test(struct _starpu_barrier *barrier)
{
	/*
	 * Check whether any threads are known to be waiting; report
	 * "BUSY" if so.
	 */
        _STARPU_PTHREAD_MUTEX_LOCK(&barrier->mutex_exit);
        if (barrier->reached_exit != barrier->count)
	{
                _STARPU_PTHREAD_MUTEX_UNLOCK(&barrier->mutex_exit);
                return EBUSY;
        }
        _STARPU_PTHREAD_MUTEX_UNLOCK(&barrier->mutex_exit);
        return 0;
}

int _starpu_barrier_destroy(struct _starpu_barrier *barrier)
{
	int ret = _starpu_barrier_test(barrier);
	while (ret == EBUSY)
	{
		ret = _starpu_barrier_test(barrier);
	}
	_STARPU_DEBUG("reached_exit %d\n", barrier->reached_exit);

	_STARPU_PTHREAD_MUTEX_DESTROY(&barrier->mutex);
	_STARPU_PTHREAD_MUTEX_DESTROY(&barrier->mutex_exit);
	_STARPU_PTHREAD_COND_DESTROY(&barrier->cond);
	return 0;
}

int _starpu_barrier_wait(struct _starpu_barrier *barrier)
{
	int ret=0;

        // Wait until all threads enter the barrier
	_STARPU_PTHREAD_MUTEX_LOCK(&barrier->mutex);
	barrier->reached_exit=0;
	barrier->reached_start++;
	if (barrier->reached_start == barrier->count)
	{
		barrier->reached_start = 0;
		_STARPU_PTHREAD_COND_BROADCAST(&barrier->cond);
		ret = PTHREAD_BARRIER_SERIAL_THREAD;
	}
	else
	{
                _STARPU_PTHREAD_COND_WAIT(&barrier->cond,&barrier->mutex);
	}
	_STARPU_PTHREAD_MUTEX_UNLOCK(&barrier->mutex);

        // Count number of threads that exit the barrier
	_STARPU_PTHREAD_MUTEX_LOCK(&barrier->mutex_exit);
	barrier->reached_exit ++;
	_STARPU_PTHREAD_MUTEX_UNLOCK(&barrier->mutex_exit);

	return ret;
}
