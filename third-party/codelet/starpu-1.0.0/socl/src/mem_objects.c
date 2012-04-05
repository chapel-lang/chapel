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


#define mem_object_hash_key 257

static cl_mem p_mem_objects[mem_object_hash_key] = {NULL};
static volatile pthread_spinlock_t p_mem_objects_spinlock[mem_object_hash_key];

#define LOCK(i) pthread_spin_lock(&p_mem_objects_spinlock[i]);
#define UNLOCK(i) pthread_spin_unlock(&p_mem_objects_spinlock[i]);

void mem_object_init(void) {
  int i;
  for (i=0; i<mem_object_hash_key; i++) {
    pthread_spin_init(&p_mem_objects_spinlock[i], 0);
  }
}

static int mem_object_hash(const void * addr) {
  uintptr_t t = (uintptr_t)addr;
  uintptr_t t2 = t >> 4;
  uintptr_t t3 = t2 % mem_object_hash_key;
  return (int)t3;
}

void mem_object_store(cl_mem m) {
   int hash = mem_object_hash(m);

   LOCK(hash);

   m->prev = NULL;
   m->next = p_mem_objects[hash];
   if (p_mem_objects[hash] != NULL)
      p_mem_objects[hash]->prev = m;
   p_mem_objects[hash] = m;

   UNLOCK(hash);
}

void mem_object_release(cl_mem m) {

   int hash = mem_object_hash(m);

   LOCK(hash);

   if (m->prev != NULL)
     m->prev->next = m->next;
   if (m->next != NULL)
     m->next->prev = m->prev;

   if (p_mem_objects[hash] == m) {
      p_mem_objects[hash] = m->next;
   }

   UNLOCK(hash)
}

cl_mem mem_object_fetch(const void * addr) {
   int hash = mem_object_hash(*(cl_mem*)addr);

   LOCK(hash);

   cl_mem buf;
   for (buf = p_mem_objects[hash]; buf != NULL; buf = buf->next) {
      if (*(cl_mem*)addr == buf) {
         UNLOCK(hash);
         return buf;
      }
   }

   UNLOCK(hash);
   return NULL;
}

#undef LOCK
#undef UNLOCK
#undef mem_object_hash_key
