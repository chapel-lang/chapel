/*
 * Copyright (c) 2013-2014 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _OFI_LOCK_H_
#define _OFI_LOCK_H_

#include "config.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ofi_osd.h>


#ifdef __cplusplus
extern "C" {
#endif


int ofi_wait_cond(pthread_cond_t *cond, pthread_mutex_t *mut, int timeout_ms);


#if PT_LOCK_SPIN == 1

#define ofi_spin_t_ pthread_spinlock_t
#define ofi_spin_init_(lock) pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE)
#define ofi_spin_destroy_(lock) pthread_spin_destroy(lock)
#define ofi_spin_lock_(lock) pthread_spin_lock(lock)
#define ofi_spin_trylock_(lock) pthread_spin_trylock(lock)
#define ofi_spin_unlock_(lock) pthread_spin_unlock(lock)

#else

#define ofi_spin_t_ pthread_mutex_t
#define ofi_spin_init_(lock) pthread_mutex_init(lock, NULL)
#define ofi_spin_destroy_(lock) pthread_mutex_destroy(lock)
#define ofi_spin_lock_(lock) pthread_mutex_lock(lock)
#define ofi_spin_trylock_(lock) pthread_mutex_trylock(lock)
#define ofi_spin_unlock_(lock) pthread_mutex_unlock(lock)

#endif /* PT_LOCK_SPIN */

#if ENABLE_DEBUG

typedef struct {
	ofi_spin_t_ impl;
	int is_initialized;
	int in_use;
} ofi_spin_t;

static inline int ofi_spin_init(ofi_spin_t *lock)
{
	int ret;

	ret = ofi_spin_init_(&lock->impl);
	lock->is_initialized = !ret;
	lock->in_use = 0;

	return ret;
}

static inline void ofi_spin_destroy(ofi_spin_t *lock)
{
	int ret;

	assert(lock->is_initialized);
	lock->is_initialized = 0;
	ret = ofi_spin_destroy_(&lock->impl);
	assert(!ret);
}

static inline void ofi_spin_lock(ofi_spin_t *lock)
{
	int ret;

	assert(lock->is_initialized);
	ret = ofi_spin_lock_(&lock->impl);
	assert(!ret);
	lock->in_use++;
}

static inline int ofi_spin_trylock(ofi_spin_t *lock)
{
	int ret;

	assert(lock->is_initialized);
	ret = ofi_spin_trylock_(&lock->impl);
	if (!ret)
		lock->in_use++;

	return ret;
}

static inline void ofi_spin_unlock(ofi_spin_t *lock)
{
	int ret;

	assert(lock->in_use);
	assert(lock->is_initialized);
	lock->in_use--;
	ret = ofi_spin_unlock_(&lock->impl);
	assert(!ret);
}

static inline int ofi_spin_held(ofi_spin_t *lock)
{
	return lock->in_use;
}

static inline void ofi_spin_lock_noop(ofi_spin_t *lock)
{
	/* These non-op routines must be used only by single-threaded code*/
	assert(!lock->in_use);
	lock->in_use = 1;
}

static inline void ofi_spin_unlock_noop(ofi_spin_t *lock)
{
	assert(lock->in_use);
	lock->in_use = 0;
}

#else /* !ENABLE_DEBUG */

#  define ofi_spin_t ofi_spin_t_
#  define ofi_spin_init(lock) ofi_spin_init_(lock)
#  define ofi_spin_destroy(lock) ofi_spin_destroy_(lock)
#  define ofi_spin_lock(lock) ofi_spin_lock_(lock)
#  define ofi_spin_trylock(lock) ofi_spin_trylock_(lock)
#  define ofi_spin_unlock(lock) ofi_spin_unlock_(lock)
#  define ofi_spin_held(lock) true

static inline void ofi_spin_lock_noop(ofi_spin_t *lock)
{
	(void) lock;
}

static inline void ofi_spin_unlock_noop(ofi_spin_t *lock)
{
	(void) lock;
}

#endif

typedef void(*ofi_spin_lock_t)(ofi_spin_t *lock);
typedef void(*ofi_spin_unlock_t)(ofi_spin_t *lock);

static inline void ofi_spin_lock_op(ofi_spin_t *lock)
{
	ofi_spin_lock(lock);
}

static inline void ofi_spin_unlock_op(ofi_spin_t *lock)
{
	ofi_spin_unlock(lock);
}

static inline int ofi_spin_held_op(ofi_spin_t *lock)
{
	return ofi_spin_held(lock);
}

#define ofi_mutex_t_ pthread_mutex_t
#define ofi_mutex_init_(lock) pthread_mutex_init(lock, NULL)
#define ofi_mutex_destroy_(lock) pthread_mutex_destroy(lock)
#define ofi_mutex_lock_(lock) pthread_mutex_lock(lock)
#define ofi_mutex_trylock_(lock) pthread_mutex_trylock(lock)
#define ofi_mutex_unlock_(lock) pthread_mutex_unlock(lock)

#if ENABLE_DEBUG

typedef struct {
	ofi_mutex_t_ impl;
	int is_initialized;
	int in_use;
} ofi_mutex_t;

static inline int ofi_mutex_init(ofi_mutex_t *lock)
{
	int ret;

	ret = ofi_mutex_init_(&lock->impl);
	lock->is_initialized = !ret;
	lock->in_use = 0;

	return ret;
}

static inline void ofi_mutex_destroy(ofi_mutex_t *lock)
{
	int ret;

	assert(lock->is_initialized);
	lock->is_initialized = 0;
	ret = ofi_mutex_destroy_(&lock->impl);
	assert(!ret);
}

static inline void ofi_mutex_lock(ofi_mutex_t *lock)
{
	int ret;

	assert(lock->is_initialized);
	ret = ofi_mutex_lock_(&lock->impl);
	assert(!ret);
	lock->in_use++;
}

static inline int ofi_mutex_trylock(ofi_mutex_t *lock)
{
	int ret;

	assert(lock->is_initialized);
	ret = ofi_mutex_trylock_(&lock->impl);
	if (!ret)
		lock->in_use++;

	return ret;
}

static inline void ofi_mutex_unlock(ofi_mutex_t *lock)
{
	int ret;

	assert(lock->in_use);
	assert(lock->is_initialized);
	lock->in_use--;
	ret = ofi_mutex_unlock_(&lock->impl);
	assert(!ret);
}

static inline int ofi_mutex_held(ofi_mutex_t *lock)
{
	return lock->in_use;
}

static inline void ofi_mutex_lock_noop(ofi_mutex_t *lock)
{
	/* These non-op routines must be used only by single-threaded code*/
	assert(!lock->in_use);
	lock->in_use = 1;
}

static inline void ofi_mutex_unlock_noop(ofi_mutex_t *lock)
{
	assert(lock->in_use);
	lock->in_use = 0;
}

static inline int
ofi_pthread_wait_cond(pthread_cond_t *cond, ofi_mutex_t *lock, int timeout_ms)
{
	assert(lock->is_initialized);
	return ofi_wait_cond(cond, &lock->impl, timeout_ms);
}

#else /* !ENABLE_DEBUG */

#  define ofi_mutex_t ofi_mutex_t_
#  define ofi_mutex_init(lock) ofi_mutex_init_(lock)
#  define ofi_mutex_destroy(lock) ofi_mutex_destroy_(lock)
#  define ofi_mutex_lock(lock) ofi_mutex_lock_(lock)
#  define ofi_mutex_trylock(lock) ofi_mutex_trylock_(lock)
#  define ofi_mutex_unlock(lock) ofi_mutex_unlock_(lock)
#  define ofi_mutex_held(lock) true

static inline void ofi_mutex_lock_noop(ofi_mutex_t *lock)
{
	(void) lock;
}

static inline void ofi_mutex_unlock_noop(ofi_mutex_t *lock)
{
	(void) lock;
}

#define ofi_pthread_wait_cond(cond, mut, timeout_ms) \
	ofi_wait_cond(cond, mut, timeout_ms)

#endif

typedef void(*ofi_mutex_lock_t)(ofi_mutex_t *lock);
typedef void(*ofi_mutex_unlock_t)(ofi_mutex_t *lock);

static inline void ofi_mutex_lock_op(ofi_mutex_t *lock)
{
	ofi_mutex_lock(lock);
}

static inline void ofi_mutex_unlock_op(ofi_mutex_t *lock)
{
	ofi_mutex_unlock(lock);
}

static inline int ofi_mutex_held_op(ofi_mutex_t *lock)
{
	return ofi_mutex_held(lock);
}

static inline void ofi_nolock_lock_op(void *nolock)
{
	(void) nolock;
}

static inline void ofi_nolock_unlock_op(void *nolock)
{
	(void) nolock;
}

/* No way to verify, so return true to pass all asserts.
 * User should provide their own checks higher-up.
 */
static inline int ofi_nolock_held_op(void *nolock)
{
	return 1;
}


/*
 * Generic lock abstraction
 * Caller selects lock implementation at runtime
 */
enum ofi_lock_type {
	OFI_LOCK_MUTEX,		/* default */
	OFI_LOCK_SPINLOCK,
	OFI_LOCK_NOOP,
	OFI_LOCK_NONE,
};

typedef int  (*ofi_genlock_lockheld_t)(void *baselock);
typedef void (*ofi_genlock_lockop_t)(void *baselock);

struct ofi_genlock {
	enum ofi_lock_type	lock_type;
	union {
		ofi_mutex_t	mutex;
		ofi_spin_t	spinlock;
		void		*nolock;
	} base;

	ofi_genlock_lockheld_t	held;
	ofi_genlock_lockop_t	lock;
	ofi_genlock_lockop_t	unlock;
};

int ofi_genlock_init(struct ofi_genlock *lock,
		     enum ofi_lock_type lock_type);
void ofi_genlock_destroy(struct ofi_genlock *lock);

static inline int ofi_genlock_held(struct ofi_genlock *lock)
{
	return lock->held(&lock->base);
}

static inline void ofi_genlock_lock(struct ofi_genlock *lock)
{
	lock->lock(&lock->base);
}

static inline void ofi_genlock_unlock(struct ofi_genlock *lock)
{
	lock->unlock(&lock->base);
}

#ifdef __cplusplus
}
#endif

#endif /* _OFI_LOCK_H_ */
