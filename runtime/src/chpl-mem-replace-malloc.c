/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

#ifndef CHPL_USING_CSTDLIB_MALLOC
#ifdef __GLIBC__
#define USE_GLIBC_MALLOC_HOOKS
#endif
#endif

#ifdef __GLIBC__
#include <malloc.h> // for memalign
#endif

static int heapInitialized = 0;

// declare symbol version of calloc/malloc/realloc in case
// we want pointers to them or we want to use linker scripts/weak symbols

// If we use weak symbols/linker scripts, for glibc we would need to define:
//  malloc, free, cfree, calloc, realloc,
//  memalign, posix_memalign, aligned_alloc,
//  malloc_usable_size.
// we could use ld's --wrap argument to achieve it.

// we always create symbols for calloc/malloc/realloc so that
// we could use a linker script or malloc hook.
#ifdef CHPL_WRAP_MALLOC
void* __real_calloc(size_t n, size_t size)
      CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;
void* __wrap_calloc(size_t n, size_t size)
      CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void* __real_malloc(size_t size)
      CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;
void* __wrap_malloc(size_t size)
      CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void* __real_memalign(size_t boundary, size_t size)
      CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;
void* __wrap_memalign(size_t boundary, size_t size)
      CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void* __real_realloc(void* ptr, size_t size)
      CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;
void* __wrap_realloc(void* ptr, size_t size)
      CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void __real_free(void* ptr);
void __wrap_free(void* ptr);


void* __wrap_calloc(size_t n, size_t size)
{
  if( heapInitialized == 0 ) return __real_calloc(n, size);
  printf("in __wrap_calloc\n");
  return chpl_calloc(n, size);
}

void* __wrap_malloc(size_t size)
{
  if( heapInitialized == 0 ) return __real_malloc(size);
  printf("in __wrap_malloc\n");
  return chpl_malloc(size);
}

void* __wrap_memalign(size_t boundary, size_t size)
{
  if( heapInitialized == 0 ) return __real_memalign(boundary, size);
  printf("in __wrap_memalign\n");
  return chpl_memalign(boundary, size);
}

void* __wrap_realloc(void* ptr, size_t size)
{
  if( heapInitialized == 0 ) return __real_realloc(ptr, size);
  printf("in __wrap_realloc\n");
  return chpl_realloc(ptr, size);
}

void __wrap_free(void* ptr)
{
  if( ! ptr ) return;
  if( heapInitialized == 0 ) {
    __real_free(ptr);
    return;
  }
  printf("in __wrap_free\n");
  chpl_free(ptr);
}

#endif

#ifdef USE_GLIBC_MALLOC_HOOKS

// GLIBC requests that we define __malloc_initialize_hook like this
//void (* __malloc_initialize_hook) (void) = chpl_mem_replace_malloc_if_needed;
// but that would result in chpl_mem_replace_malloc being called
// at program start (more or less) and not after the memory layer
// is initialized. We want to only use our custom allocator
// after the comms layer has created the communication-ready
// heap. An alternative would be to make these routines call
// the old allocator if heapInitialized==0. We'd have to do
// that if we used symbol remapping too (e.g. with ld --wrap).

static void * (*original_malloc)  (size_t, const void *);
static void * (*original_memalign)(size_t, size_t, const void *);
static void * (*original_realloc) (void *, size_t, const void *);
static void   (*original_free)    (void *, const void *);

static
void* chpl_malloc_hook(size_t size, const void* arg)
{
  if( heapInitialized == 0 ) return original_malloc(size, arg);
  printf("in chpl_malloc_hook\n");
  return chpl_malloc(size);
}

static
void* chpl_memalign_hook(size_t boundary, size_t size, const void* arg)
{
  if( heapInitialized == 0 ) return original_memalign(boundary, size, arg);
  printf("in chpl_memalign_hook\n");
  return chpl_memalign(boundary, size);
}

static
void* chpl_realloc_hook(void* ptr, size_t size, const void* arg)
{
  if( heapInitialized == 0 ) return original_realloc(ptr, size, arg);
  printf("in chpl_realloc_hook\n");
  return chpl_realloc(ptr,size);
}

static
void chpl_free_hook(void* ptr, const void* arg) {
  if( ! ptr ) return;
  if( heapInitialized == 0 ) {
    original_free(ptr, arg);
    return;
  }
  printf("in chpl_free_hook\n");
  chpl_free(ptr);
}

#endif


void chpl_mem_replace_malloc_if_needed_heap_inited(void) {
#ifdef CHPL_USING_CSTDLIB_MALLOC
  // do nothing, we're already using the C stdlib allocator.
#else
  // try using malloc hooks for glibc
  static int hooksInstalled = 0;
  if( hooksInstalled ) return;
  hooksInstalled = 1;

  printf("in chpl_mem_replace_malloc_if_needed\n");

#ifdef USE_GLIBC_MALLOC_HOOKS
  // glibc wants a memalign call
  // glibc: void *memalign(size_t boundary, size_t size);
  // dlmalloc: dlmemalign
  // tcmalloc: tc_memalign
  original_malloc = __malloc_hook;
  original_memalign = __memalign_hook;
  original_realloc = __realloc_hook;
  original_free = __free_hook;

  __malloc_hook = chpl_malloc_hook;
  __memalign_hook = chpl_memalign_hook;
  __realloc_hook = chpl_realloc_hook;
  __free_hook = chpl_free_hook;

#endif

  // We could do this too for Mac OS X if it mattered.
  // - include malloc/malloc.h
  // - replace functions in malloc_default_zone returned malloc_zone_t*
  // functions needed include a "size" function and a "valloc" function:
  // BSD: malloc_size
  // glibc: malloc_usable_size
  // dlmalloc: dlmalloc_usable_size
  // tcmalloc: tc_malloc_size
#endif

  // Record that the heap is ready.
  heapInitialized = 1;
}

