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

#ifndef SOCL_MEM_OBJECTS_H
#define SOCL_MEM_OBJECTS_H

void mem_object_init(void);
void mem_object_store(cl_mem m);
void mem_object_release(cl_mem m);
cl_mem mem_object_fetch(const void * addr);

#endif /* SOCL_MEM_OBJECTS_H */
