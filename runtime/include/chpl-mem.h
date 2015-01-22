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

#ifndef _chpl_mem_H_
#define _chpl_mem_H_

#ifndef LAUNCHER

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "arg.h"
#include "chpl-mem-hook.h"
#include "chpltypes.h"
#include "chpl-tasks.h"
#include "error.h"


/* The names and arguments for these functions are part
   of Chapel's user-facing interface because they are
   documented in a README doc/release
 */
// start public interface
static inline void* chpl_calloc(size_t n, size_t size);
static inline void* chpl_malloc(size_t size);
static inline void* chpl_memalign(size_t boundary, size_t size);
static inline void* chpl_realloc(void* ptr, size_t size);
static inline void chpl_free(void* ptr);
int chpl_posix_memalign(void** ptr, size_t alignment, size_t size);
void* chpl_valloc(size_t size);
void* chpl_pvalloc(size_t size);
// end public interface

// returns 0 if valid, EINVAL otherwise
int chpl_posix_memalign_check_valid(size_t alignment);

// runtime/include/mem/*/chpl-mem-impl.h defines
// chpl_calloc, chpl_malloc, chpl_realloc, chpl_free
// with the same signatures as the standard functions
// and no additional error checking.
#include "chpl-mem-impl.h"

void chpl_mem_init(void);
void chpl_mem_exit(void);

int chpl_mem_inited(void);


static inline
void* chpl_mem_allocMany(size_t number, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, c_string filename) {
  void* memAlloc;
  chpl_memhook_malloc_pre(number, size, description, lineno, filename);
  memAlloc = chpl_malloc(number*size);
  chpl_memhook_malloc_post(memAlloc, number, size, description,
                           lineno, filename);
  return memAlloc;
}

static inline
void* chpl_mem_alloc(size_t size, chpl_mem_descInt_t description,
                     int32_t lineno, c_string filename) {
  return chpl_mem_allocMany(1, size, description, lineno, filename);
}

static inline
void* chpl_mem_allocManyZero(size_t number, size_t size,
                             chpl_mem_descInt_t description,
                             int32_t lineno, c_string filename) {
  void* memAlloc;
  chpl_memhook_malloc_pre(number, size, description, lineno, filename);
  memAlloc = chpl_calloc(number, size);
  chpl_memhook_malloc_post(memAlloc, number, size, description,
                           lineno, filename);
  return memAlloc;
}

static inline
void* chpl_mem_calloc(size_t number, size_t size,
                      chpl_mem_descInt_t description,
                      int32_t lineno, c_string filename) {
  return chpl_mem_allocManyZero(number, size, description, lineno, filename);
}

static inline
void* chpl_mem_realloc(void* memAlloc, size_t size,
                       chpl_mem_descInt_t description,
                       int32_t lineno, c_string filename) {
  void* moreMemAlloc;

  chpl_memhook_realloc_pre(memAlloc, size, description,
                           lineno, filename);
  if (size == 0) {
    chpl_memhook_free_pre(memAlloc, lineno, filename);
    chpl_free(memAlloc);
    return NULL;
  }
  moreMemAlloc = chpl_realloc(memAlloc, size);
  chpl_memhook_realloc_post(moreMemAlloc, memAlloc, size, description,
                            lineno, filename);
  return moreMemAlloc;
}

static inline
void chpl_mem_free(void* memAlloc, int32_t lineno, c_string filename) {
  chpl_memhook_free_pre(memAlloc, lineno, filename);
  chpl_free(memAlloc);
}

// Provide a handle to instrument Chapel calls to memcpy.
static inline
void* chpl_memcpy(void* dest, const void* src, size_t num)
{
  assert(dest != src);
  return memcpy(dest, src, num);
}

// Query the allocator to ask for a good size to allocate that is at least
// minSize. One example where this is useful is to grow a vector while
// minimizing memory wasted by overallocation.
//
// If an allocator does not have the ability to get this information, minSize
// will be returned.
static inline size_t chpl_mem_goodAllocSize(size_t minSize) {
  return chpl_goodAllocSize(minSize);
}

// free a c_string_copy, no error checking.
// The argument type is explicitly c_string_copy, since only an "owned" string
// should be freed.
static inline
void chpl_rt_free_c_string_copy(c_string_copy *s, int32_t lineno, c_string filename)  {
  assert(*s!=NULL);
  chpl_mem_free((void *) *s, lineno, filename);
  *s = NULL;
}

// free a c_string (deprecated)
// This function is needed only because NewString.chpl uses the c_string type.
// c_strings are "unowned" so should not be freed, but NewString.chpl was written
// before this distinction was made.
static inline
void chpl_rt_free_c_string(c_string* s, int32_t lineno, c_string filename)
{
  // As far as the C compiler is concerned c_string and c_string_copy are the
  // same type, so no explicit cast is required.
  chpl_rt_free_c_string_copy(s, lineno, filename);
}

void chpl_mem_layerInit(void);
void chpl_mem_layerExit(void);
void* chpl_mem_layerAlloc(size_t, int32_t lineno, c_string filename);
void* chpl_mem_layerRealloc(void*, size_t, int32_t lineno, c_string filename);
void chpl_mem_layerFree(void*, int32_t lineno, c_string filename);

#else // LAUNCHER

#include <stdlib.h>
#include "arg.h"

#define chpl_mem_allocMany(number, size, description, lineno, filename)        \
  malloc((number)*(size))

#define chpl_mem_free(ptr, lineno, filename)        \
  free(ptr)

#endif // LAUNCHER

#endif
