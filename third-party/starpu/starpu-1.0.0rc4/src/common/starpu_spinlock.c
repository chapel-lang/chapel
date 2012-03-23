/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
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

#include <common/starpu_spinlock.h>
#include <common/config.h>
#include <common/utils.h>
#include <starpu_util.h>

int _starpu_spin_init(struct _starpu_spinlock *lock)
{
#ifdef STARPU_SPINLOCK_CHECK
//	memcpy(&lock->errcheck_lock, PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP, sizeof(PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP));
	int ret;
	ret = pthread_mutexattr_init(&lock->errcheck_attr);
	STARPU_ASSERT(!ret);

	ret = pthread_mutexattr_settype(&lock->errcheck_attr, PTHREAD_MUTEX_ERRORCHECK);
	STARPU_ASSERT(!ret);

	ret = pthread_mutex_init(&lock->errcheck_lock, &lock->errcheck_attr);
	return ret;
#else
#ifdef HAVE_PTHREAD_SPIN_LOCK
	int ret = pthread_spin_init(&lock->lock, 0);
	STARPU_ASSERT(!ret);
	return ret;
#else
	lock->taken = 0;
	return 0;
#endif
#endif
}

int _starpu_spin_destroy(struct _starpu_spinlock *lock STARPU_ATTRIBUTE_UNUSED)
{
#ifdef STARPU_SPINLOCK_CHECK
	pthread_mutexattr_destroy(&lock->errcheck_attr);
	return pthread_mutex_destroy(&lock->errcheck_lock);
#else
#ifdef HAVE_PTHREAD_SPIN_LOCK
	int ret = pthread_spin_destroy(&lock->lock);
	STARPU_ASSERT(!ret);
	return ret;
#else
	/* we don't do anything */
	return 0;
#endif
#endif
}

int _starpu_spin_lock(struct _starpu_spinlock *lock)
{
#ifdef STARPU_SPINLOCK_CHECK
	int ret = pthread_mutex_lock(&lock->errcheck_lock);
	STARPU_ASSERT(!ret);
	return ret;
#else
#ifdef HAVE_PTHREAD_SPIN_LOCK
	int ret = pthread_spin_lock(&lock->lock);
	STARPU_ASSERT(!ret);
	return ret;
#else
	uint32_t prev;
	do
	{
		prev = STARPU_TEST_AND_SET(&lock->taken, 1);
	}
	while (prev);
	return 0;
#endif
#endif
}

int _starpu_spin_checklocked(struct _starpu_spinlock *lock)
{
#ifdef STARPU_SPINLOCK_CHECK
	int ret = pthread_mutex_trylock(&lock->errcheck_lock);
	STARPU_ASSERT(ret != 0);
	return ret == 0;
#else
#ifdef HAVE_PTHREAD_SPIN_LOCK
	int ret = pthread_spin_trylock(&lock->lock);
	STARPU_ASSERT(ret != 0);
	return ret == 0;
#else
	STARPU_ASSERT(lock->taken);
	return !lock->taken;
#endif
#endif
}

int _starpu_spin_trylock(struct _starpu_spinlock *lock)
{
#ifdef STARPU_SPINLOCK_CHECK
	int ret = pthread_mutex_trylock(&lock->errcheck_lock);
	STARPU_ASSERT(!ret || (ret == EBUSY));
	return ret;
#else
#ifdef HAVE_PTHREAD_SPIN_LOCK
	int ret =  pthread_spin_trylock(&lock->lock);
	STARPU_ASSERT(!ret || (ret == EBUSY));
	return ret;
#else
	uint32_t prev;
	prev = STARPU_TEST_AND_SET(&lock->taken, 1);
	return (prev == 0)?0:EBUSY;
#endif
#endif
}

int _starpu_spin_unlock(struct _starpu_spinlock *lock STARPU_ATTRIBUTE_UNUSED)
{
#ifdef STARPU_SPINLOCK_CHECK
	int ret = pthread_mutex_unlock(&lock->errcheck_lock);
	STARPU_ASSERT(!ret);
	return ret;
#else
#ifdef HAVE_PTHREAD_SPIN_LOCK
	int ret = pthread_spin_unlock(&lock->lock);
	STARPU_ASSERT(!ret);
	return ret;
#else
	STARPU_RELEASE(&lock->taken);
	return 0;
#endif
#endif
}
