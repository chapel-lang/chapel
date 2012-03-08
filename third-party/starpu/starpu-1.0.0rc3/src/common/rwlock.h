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

#ifndef __RWLOCKS_H__
#define __RWLOCKS_H__

#include <stdint.h>
#include <starpu.h>

/* Dummy implementation of a RW-lock using a spinlock. */
struct _starpu_rw_lock {
	uint32_t busy;
	uint8_t writer;
	uint16_t readercnt;
};

/* Initialize the RW-lock */
void _starpu_init_rw_lock(struct _starpu_rw_lock *lock);

/* Grab the RW-lock in a write mode */
void _starpu_take_rw_lock_write(struct _starpu_rw_lock *lock);

/* Grab the RW-lock in a read mode */
void _starpu_take_rw_lock_read(struct _starpu_rw_lock *lock);

/* Try to grab the RW-lock in a write mode. Returns 0 in case of success, -1
 * otherwise. */
int _starpu_take_rw_lock_write_try(struct _starpu_rw_lock *lock);

/* Try to grab the RW-lock in a read mode. Returns 0 in case of success, -1
 * otherwise. */
int _starpu_take_rw_lock_read_try(struct _starpu_rw_lock *lock);

/* Unlock the RW-lock. */
void _starpu_release_rw_lock(struct _starpu_rw_lock *lock);

#endif
