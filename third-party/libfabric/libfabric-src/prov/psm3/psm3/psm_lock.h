/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef _PSMI_IN_USER_H
#error psm_lock.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_LOCK_H
#define _PSMI_LOCK_H

#ifndef PSMI_USE_PTHREAD_SPINLOCKS
#define PSMI_USE_PTHREAD_SPINLOCKS 0
#endif

#if PSMI_USE_PTHREAD_SPINLOCKS
typedef pthread_spinlock_t psmi_spinlock_t;

#define psmi_spin_init(lock)	  pthread_spin_init(lock, \
					PTHREAD_PROCESS_PRIVATE)
#define psmi_spin_destroy(lock)	pthread_spin_destroy(lock)
#define psmi_spin_lock(lock)	  pthread_spin_lock(lock)
#define psmi_spin_trylock(lock) pthread_spin_trylock(lock)
#define psmi_spin_unlock(lock)  pthread_spin_unlock(lock)
#else
typedef ips_atomic_t psmi_spinlock_t;
#define PSMI_SPIN_INVALID   2
#define PSMI_SPIN_LOCKED    1
#define PSMI_SPIN_UNLOCKED  0
#endif

/* psmi_lock_t structure */
typedef struct {

#ifdef PSMI_LOCK_IS_SPINLOCK
	psmi_spinlock_t lock;
#elif defined(PSMI_LOCK_IS_MUTEXLOCK_DEBUG)
	pthread_mutex_t lock;
	pthread_t lock_owner;
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
	const char *lock_owner_loc;
#endif
#elif defined(PSMI_LOCK_IS_MUTEXLOCK)
	pthread_mutex_t lock;
#endif
} psmi_lock_t;


#if PSMI_USE_PTHREAD_SPINLOCKS
#else
PSMI_ALWAYS_INLINE(int psmi_spin_init(psmi_spinlock_t *lock))
{
	ips_atomic_set(lock, PSMI_SPIN_UNLOCKED);
	return 0;
}

PSMI_ALWAYS_INLINE(int psmi_spin_trylock(psmi_spinlock_t *lock))
{
	if (ips_atomic_cmpxchg(lock, PSMI_SPIN_UNLOCKED, PSMI_SPIN_LOCKED)
			== PSMI_SPIN_UNLOCKED) {
		return 0;
	}

	return EBUSY;
}

PSMI_ALWAYS_INLINE(int psmi_spin_destroy(psmi_spinlock_t *lock))
{
	if (lock == NULL) {
		return EINVAL;
	}

	/* We could just do psmi_spin_trylock() here and dispense with the invalid state */
	if (ips_atomic_cmpxchg(lock, PSMI_SPIN_UNLOCKED, PSMI_SPIN_INVALID)
			== PSMI_SPIN_UNLOCKED) {
		return 0;
	}

	return EBUSY;
}

PSMI_ALWAYS_INLINE(int psmi_spin_lock(psmi_spinlock_t *lock))
{
	while (psmi_spin_trylock(lock) == EBUSY) {
	}
	return 0;
}

PSMI_ALWAYS_INLINE(int psmi_spin_unlock(psmi_spinlock_t *lock))
{
	atomic_set(lock, PSMI_SPIN_UNLOCKED);
	return 0;
}
#endif /* PSMI_USE_PTHREAD_SPINLOCKS */

PSMI_ALWAYS_INLINE(void psmi_init_lock(psmi_lock_t *lock))
{
#ifdef PSMI_LOCK_IS_SPINLOCK
	psmi_spin_init(&(lock->lock));
#elif defined(PSMI_LOCK_IS_MUTEXLOCK)
	pthread_mutex_init(&(lock->lock), NULL);
#elif defined(PSMI_LOCK_IS_MUTEXLOCK_DEBUG)
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
	pthread_mutex_init(&(lock->lock), &attr);
	pthread_mutexattr_destroy(&attr);
	lock->lock_owner = PSMI_LOCK_NO_OWNER;
#ifdef PSMI_LOCK_MUTEXLOCK_DEBUG_LOG_CONTENTION
	lock->lock_owner_loc = "NONE";
#endif
#endif
}

PSMI_ALWAYS_INLINE(void psmi_destroy_lock(psmi_lock_t *lock))
{
#ifdef PSMI_LOCK_IS_SPINLOCK
	int err;
	/* This will map to either pthread_spin_destroy() or our custom psmi_spin_destroy().
	 * Both their return values can be interpreted by strerror().
	 */
	if ((err = psmi_spin_destroy(&(lock->lock))) != 0) {
		_HFI_VDBG("Destroying spinlock failed: %s\n", strerror(err));
	}
	/* The same path for both the regular mutex and the debugging mutex */
#elif defined(PSMI_LOCK_IS_MUTEXLOCK) || defined(PSMI_LOCK_IS_MUTEXLOCK_DEBUG)
	int err;
	if ((err = pthread_mutex_destroy(&(lock->lock))) != 0) {
		/* strerror_r() may be a better choice here but it is tricky
		 * to reliably detect the XSI vs GNU version, and if hardcoded,
		 * may be inadvertently changed when tampering with headers/makefiles
		 * in the long run.
		 *
		 * This would result in incorrect operation: a segfault from
		 * derefencing the return value or failure to retrieve the
		 * error string.
		 *
		 * The C11's strerror_s may be an option here too.
		 */
		_HFI_VDBG("Destroying mutex failed: %s\n", strerror(err));
	}
#endif
}

PSMI_ALWAYS_INLINE(int psmi_sem_post(sem_t *sem, const char *name))
{
	if (sem_post(sem) == -1) {
		_HFI_VDBG("Semaphore %s: post failed\n", name ? name : "NULL" );
		return -1;
	}

	_HFI_VDBG("Semaphore %s: post succeeded\n", name ? name : "NULL");

	return 0;
}

PSMI_ALWAYS_INLINE(int psmi_sem_timedwait(sem_t *sem, const char *name))
{
	/* Wait 5 seconds for shm read-write lock to open */
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 5;

	if (sem_timedwait(sem, &ts) == -1) {
		_HFI_VDBG("Semaphore %s: Timedwait failed: %s (%d)\n",
				name ? name : "NULL", strerror(errno), errno );
		return -1;
	}

	_HFI_VDBG("Semaphore %s: Timedwait succeeded\n", name ? name : "NULL");

	return 0;
}

PSMI_ALWAYS_INLINE(int psmi_init_semaphore(sem_t **sem, const char *name,
					   mode_t mode, int value))
{
	*sem = sem_open(name, O_CREAT | O_EXCL, mode, value);
	if ((*sem == SEM_FAILED) && (errno == EEXIST)) {
		*sem = sem_open(name, O_CREAT, mode, value);
		if (*sem == SEM_FAILED) {
			_HFI_VDBG("Cannot open semaphore %s, errno=%d\n",
				  name, errno);
			return -1;
		}
	} else if (*sem == SEM_FAILED) {
		_HFI_VDBG("Cannot create semaphore %s, errno=%d\n", name, errno);
		return -1;
	}

	return 0;
}

/* C11 standard compiler built in atomic functions available in GCC 4.9 & later
 * (4.9 is a safe bet, some web sites claim 4.4 or 4.7 added these)
 * We keep it simple and just define the basics we need, C11 can do more.
 * On x86_64, var can be 4 or 8 byte int, unsigned or pointer types.
 * Some abiguity on support for 1 and 2 byte types (original icc did not).
 * INC and DEC return value after the math.
 * SEQ_CST is most conservative memory ordering, no extra cost on x86, but
 * may have more cost on other CPU architectures.
 */
#if __GNUC_PREREQ(4, 9)
#define PSM3_ATOMIC_DECL(type, var) type var
#define PSM3_ATOMIC_INC(var) __atomic_add_fetch(&(var), 1, __ATOMIC_SEQ_CST)
#define PSM3_ATOMIC_DEC(var) __atomic_sub_fetch(&(var), 1, __ATOMIC_SEQ_CST)
#define PSM3_ATOMIC_SET(var, val) __atomic_store_n((&(var), val, \
													 __ATOMIC_SEQ_CST)
#define PSM3_ATOMIC_GET(var) __atomic_load_n(&(var), __ATOMIC_SEQ_CST)
#else
/* older compiler built-in, 1st established with Intel Itanium icc */
#define PSM3_ATOMIC_DECL(type, var) type var
#define PSM3_ATOMIC_INC(var) __sync_add_and_fetch(&(var), 1)
#define PSM3_ATOMIC_DEC(var) __sync_sub_and_fetch(&(var), 1)
#define PSM3_ATOMIC_SET(var, val) (var) = (val)
#define PSM3_ATOMIC_GET(var) (var)
#endif

#endif /* _PSMI_LOCK_H */
