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

void chpl_mem_init(void) {
  chpl_mem_layerInit();
  heapInitialized = 1;

  // replace malloc calls with hooks
  // we have to do that AFTER the mem layer heap
  // is initialized.
  chpl_mem_replace_malloc_if_needed();
}


void chpl_mem_exit(void) {
  chpl_mem_layerExit();
}


int chpl_mem_inited(void) {
  return heapInitialized;
}

// declare symbol version of calloc/malloc/realloc in case
// we want pointers to them or we want to use linker scripts/weak symbols

void* chpl_calloc_sym(size_t n, size_t size)
{
  printf("in chpl_calloc_sym\n");
  return chpl_calloc(n,size);
}

void* chpl_malloc_sym(size_t size)
{
  printf("in chpl_malloc_sym\n");
  return chpl_malloc(size);
}

void* chpl_memalign_sym(size_t boundary, size_t size)
{
  printf("in chpl_memalign_sym\n");
  return chpl_memalign(boundary, size);
}

void* chpl_realloc_sym(void* ptr, size_t size)
{
  printf("in chpl_realloc_sym\n");
  return chpl_realloc(ptr,size);
}

void chpl_free_sym(void* ptr) {
  printf("in chpl_free_sym\n");
  chpl_free(ptr);
}

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

static
void* chpl_malloc_hook(size_t size, const void* unusued)
{
  printf("in chpl_malloc_hook\n");
  return chpl_malloc(size);
}

static
void* chpl_memalign_hook(size_t boundary, size_t size, const void* unused)
{
  printf("in chpl_memalign_hook\n");
  return chpl_memalign(boundary, size);
}

static
void* chpl_realloc_hook(void* ptr, size_t size, const void* unused)
{
  printf("in chpl_realloc_hook\n");
  return chpl_realloc(ptr,size);
}

static
void chpl_free_hook(void* ptr, const void* unused) {
  printf("in chpl_free_hook\n");
  chpl_free(ptr);
}

#endif


void chpl_mem_replace_malloc_if_needed(void) {
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
  __malloc_hook = chpl_malloc_hook;
  __free_hook = chpl_free_hook;
  __realloc_hook = chpl_realloc_hook;
  __memalign_hook = chpl_memalign_hook;

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
}

