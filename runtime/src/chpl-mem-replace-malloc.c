/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

// This file always declares this function at least.
void chpl_mem_replace_malloc_if_needed(void);

#define DEBUG_REPLACE_MALLOC 0

#ifndef CHPL_USING_CSTDLIB_MALLOC
// Don't declare anything unless we're not using C allocator.

struct system_allocated_ptr {
  struct system_allocated_ptr* next;
  void* ptr;
  size_t len;
};

static struct system_allocated_ptr* system_allocated_head;

// This version is for linker malloc replacements
static void track_system_allocated(
  void* ptr,
  size_t len,
  void * (*real_malloc) (size_t) )
{
  struct system_allocated_ptr* cur;
  cur = (struct system_allocated_ptr*)
            real_malloc(sizeof(struct system_allocated_ptr));
  cur->next = system_allocated_head;
  cur->ptr = ptr;
  cur->len = len;
  system_allocated_head = cur;
}

static int is_system_allocated(void* ptr_in, size_t *len_out)
{
  unsigned char* ptr = (unsigned char*) ptr_in;
  struct system_allocated_ptr* cur;
  for( cur = system_allocated_head;
       cur;
       cur = cur->next ) {
    unsigned char* start = cur->ptr;
    unsigned char* end = start + cur->len;
    if( start <= ptr && ptr < end ) {
      if( len_out ) *len_out = cur->len;
      return 1;
    }
  }
  return 0;
}


// Replace malloc (assuming it is weak symbols
// and __libc_malloc etc are defined, as with GLIBC)

void* __libc_calloc(size_t n, size_t size);
void* calloc(size_t n, size_t size);

void* __libc_malloc(size_t size);
void* malloc(size_t size);

void* __libc_memalign(size_t alignment, size_t size);
void* memalign(size_t alignment, size_t size);

void* __libc_realloc(void* ptr, size_t size);
void* realloc(void* ptr, size_t size);

void __libc_free(void* ptr);
void free(void* ptr);

int posix_memalign(void **memptr, size_t alignment, size_t size);

void* __libc_valloc(size_t size);
void* valloc(size_t size);

void* __libc_pvalloc(size_t size);
void* pvalloc(size_t size);

void* calloc(size_t n, size_t size)
{
  void* ret;
  if( !chpl_mem_inited() ) {
    ret = __libc_calloc(n, size);
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early calloc %p = system calloc(%#x)\n", ret, (int) (n*size));
    track_system_allocated(ret, n*size, __libc_malloc);
    return ret;
  }
  if( DEBUG_REPLACE_MALLOC ) 
    printf("in calloc\n");

  ret = chpl_calloc(n, size);

  if( DEBUG_REPLACE_MALLOC ) 
    printf("%p = chpl_calloc(%#x)\n", ret, (int) (n*size));

  return ret;
}

void* malloc(size_t size)
{
  void* ret;
  if( !chpl_mem_inited() ) {
    ret = __libc_malloc(size);
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early malloc %p = system malloc(%#x)\n", ret, (int) size);
    track_system_allocated(ret, size, __libc_malloc);
    return ret;
  }
  if( DEBUG_REPLACE_MALLOC ) 
    printf("in malloc\n");

  ret = chpl_malloc(size);

  if( DEBUG_REPLACE_MALLOC ) 
    printf("%p = chpl_malloc(%#x)\n", ret, (int) size);

  return ret;
}

void* memalign(size_t alignment, size_t size)
{
  if( !chpl_mem_inited() ) {
    void* ret = __libc_memalign(alignment, size);
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early memalign %p = system memalign(%#x)\n", ret, (int) size);
    track_system_allocated(ret, size, __libc_malloc);
    return ret;
  }
  if( DEBUG_REPLACE_MALLOC ) 
    printf("in memalign\n");

  return chpl_memalign(alignment, size);
}

void* realloc(void* ptr, size_t size)
{
  if( !chpl_mem_inited() ) {
    void* ret = __libc_realloc(ptr, size);
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early realloc %p = system realloc(%p,%#x)\n",
             ret, ptr, (int) size);
    track_system_allocated(ret, size, __libc_malloc);
    return ret;
  } else {
    void* ret = NULL;
    size_t allocated_len = 0;
    size_t copy_size;

    if( DEBUG_REPLACE_MALLOC )
      printf("in realloc(%p,%#x)\n", ptr, (int) size);

    // check to see if we're realloc'ing a pointer that was allocated
    // before the our allocator came up.
    if( is_system_allocated(ptr, &allocated_len) ) {
      if( DEBUG_REPLACE_MALLOC ) {
        printf("in realloc, ptr %p was system allocated to size %#x\n",
               ptr, (int) allocated_len);
      }

      // allocate some new memory on the Chapel heap
      ret = chpl_malloc(size);

      // copy the minimum of allocated_len and size
      // to handle realloc expanding or shrinking the allocation
      copy_size = allocated_len;
      if( size < copy_size ) copy_size = size;

      memcpy(ret, ptr, copy_size);

      // free the old pointer from the system heap.
      __libc_free(ptr);
      return ret;
    } else {
      return chpl_realloc(ptr, size);
    }
  }
}

void free(void* ptr)
{
  if( ! ptr ) return;
  if( DEBUG_REPLACE_MALLOC ) 
    printf("in free(%p)\n", ptr);
  // check to see if we're freeing a pointer that was allocated
  // before the our allocator came up.
  if( !chpl_mem_inited() || is_system_allocated(ptr, NULL) ) {
    if( DEBUG_REPLACE_MALLOC ) 
      printf("calling system free\n");
    __libc_free(ptr);
    return;
  }

  if( DEBUG_REPLACE_MALLOC ) 
    printf("calling chpl_free\n");
  chpl_free(ptr);
}

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
  int ret;
  if( !chpl_mem_inited() ) {
    *memptr = NULL;
    ret = chpl_posix_memalign_check_valid(alignment);
    if( ret ) return ret;
    *memptr = __libc_memalign(alignment, size);
    if( ! *memptr ) return ENOMEM;
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early posix_memalign %p = system posix_memalign(%#x)\n",
             *memptr, (int) size);
    track_system_allocated(*memptr, size, __libc_malloc);
    return 0;
  }

  if( DEBUG_REPLACE_MALLOC ) 
    printf("in posix_memalign\n");

  ret = chpl_posix_memalign(memptr, alignment, size);

  if( DEBUG_REPLACE_MALLOC ) 
    printf("%p = chpl_posix_memalign(%#x, %#x) returned %i\n",
           *memptr, (int) alignment, (int) size, ret);

  return ret;
}

void* valloc(size_t size)
{
  void* ret;
  if( !chpl_mem_inited() ) {
    ret = __libc_valloc(size);
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early valloc %p = system valloc(%#x)\n", ret, (int) size);
    track_system_allocated(ret, size, __libc_malloc);
    return ret;
  }
  if( DEBUG_REPLACE_MALLOC ) 
    printf("in valloc\n");

  ret = chpl_valloc(size);

  if( DEBUG_REPLACE_MALLOC ) 
    printf("%p = chpl_valloc(%#x)\n", ret, (int) size);

  return ret;
}

void* pvalloc(size_t size)
{
  if( !chpl_mem_inited() ) {
    void* ret = __libc_pvalloc(size);
    if( DEBUG_REPLACE_MALLOC ) 
      printf("in early pvalloc %p = system pvalloc(%#x)\n",
             ret, (int) size);
    track_system_allocated(ret, size, __libc_malloc);
    return ret;
  }
  if( DEBUG_REPLACE_MALLOC ) 
    printf("in pvalloc\n");

  return chpl_pvalloc(size);
}

// end of ifdef CHPL_USING_CSTDLIB_MALLOC
#endif



void chpl_mem_replace_malloc_if_needed(void) {
#ifdef CHPL_USING_CSTDLIB_MALLOC
  // do nothing, we're already using the C stdlib allocator.
#else
  // it would be possible to use glibc malloc
  // hooks like __malloc_hook but these are deprecated

  // We could do this too for Mac OS X if it mattered.
  // - include malloc/malloc.h
  // - replace functions in malloc_default_zone returned malloc_zone_t*
  // functions needed include a "size" function and a "valloc" function:
  // BSD: malloc_size
  // glibc: malloc_usable_size
  // jemalloc: malloc_usable_size
#endif
}

