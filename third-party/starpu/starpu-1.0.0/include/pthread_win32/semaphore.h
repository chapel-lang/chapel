/*
 * StarPU
 * Copyright (C) Université Bordeaux 1, CNRS 2010 (see AUTHORS file)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

/* This is a minimal pthread implementation based on windows functions.
 * It is *not* intended to be complete - just complete enough to get
 * StarPU running.
 */

#ifndef __STARPU_SEMAPHORE_H__
#define __STARPU_SEMAPHORE_H__

#include "pthread.h"

/**************
 * semaphores *
 **************/

typedef HANDLE sem_t;

static inline int sem_init(sem_t *sem, int pshared, unsigned int value) {
  (void)pshared;
  winPthreadAssertWindows(*sem = CreateSemaphore(NULL, value, MAXLONG, NULL));
  return 0;
}

static inline int do_sem_wait(sem_t *sem, DWORD timeout) {
  switch (WaitForSingleObject(*sem, timeout)) {
    default:
    case WAIT_FAILED:
      setSystemErrno();
      return -1;
    case WAIT_TIMEOUT:
      errno = EAGAIN;
      return -1;
    case WAIT_ABANDONED:
    case WAIT_OBJECT_0:
      return 0;
  }
}

#define sem_wait(sem) do_sem_wait(sem, INFINITE)
#define sem_trywait(sem) do_sem_wait(sem, 0)

static inline int sem_post(sem_t *sem) {
  winPthreadAssertWindows(ReleaseSemaphore(*sem, 1, NULL));
  return 0;
}

static inline int sem_destroy(sem_t *sem) {
  winPthreadAssertWindows(CloseHandle(*sem));
  return 0;
}

#endif /* __STARPU_SEMAPHORE_H__ */
