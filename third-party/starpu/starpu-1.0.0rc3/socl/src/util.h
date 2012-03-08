/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010,2011 University of Bordeaux
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

#ifndef SOCL_UTIL_H
#define SOCL_UTIL_H

int starpu_worker_get_range();

/**
 * Duplicate a memory area into a fresh allocated buffer
 * Consider using memdup or memdup_safe instead
 */
void * memdupa(const void *p, size_t size);

#define memdup(p, size) ((typeof(p))memdupa((const void*)p,size))
#define memdup_safe(p,size) (p == NULL ? NULL : memdup(p,size))

/**
 * Duplicate an array of pointers by performing a deep copy
 */
void ** memdup_deep_safea(const void **p, unsigned n, size_t size);

#define memdup_deep_safe(p,n,size) ((typeof(p))memdup_deep_safea((const void **)p,n,size))

/**
 * Duplicate an array of pointers by performing a deep copy
 * Sizes are different for each cell
 */
void ** memdup_deep_varsize_safea(const void **p, unsigned n, size_t * size);

#define memdup_deep_varsize_safe(p,n,size) ((typeof(p))memdup_deep_varsize_safea((const void **)p,n,size))

#endif /* SOCL_UTIL_H */
