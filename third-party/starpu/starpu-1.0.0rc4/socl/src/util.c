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

#include "socl.h"

int starpu_worker_get_range() {
   int id = starpu_worker_get_id();
   int i, oid = 0;
   for (i=0; i<id; i++)
      if (starpu_worker_get_type(i) == STARPU_OPENCL_WORKER) oid++;

   return oid;
}

void * memdupa(const void *p, size_t size) {
	void * s = malloc(size);
	memcpy(s,p,size);
	return s;
}

void ** memdup_deep_safea(const void **p, unsigned n, size_t size) {
	void ** s = (void**)malloc(sizeof(void*) * n);
	unsigned i;
	for (i=0; i<n; i++) {
		s[i] = memdup_safe((void*)p[i], size);
	}
	return s;
}

void ** memdup_deep_varsize_safea(const void **p, unsigned n, size_t * size) {
	void ** s = (void**)malloc(sizeof(void*) * n);
	unsigned i;
	for (i=0; i<n; i++) {
		s[i] = memdup_safe((void*)p[i], size[i]);
	}
	return s;
}
