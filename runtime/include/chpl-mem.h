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


// Some compilers support 'warn_unused_result' or 'acts like malloc'
// we put these on our memory layer implementations
#ifdef __GNUC__

#if __GNUC_PREREQ (3,4)
#define CHPL_ATTRIBUTE_WARN_UNUSED_RESULT __attribute__ ((__warn_unused_result__))
#else
#define CHPL_ATTRIBUTE_WARN_UNUSED_RESULT
#endif

#if __GNUC_PREREQ (2,96)
#define CHPL_ATTRIBUTE_MALLOC __attribute_malloc__
#else
#define CHPL_ATTRIBUTE_MALLOC
#endif

#else
#define CHPL_ATTRIBUTE_WARN_UNUSED_RESULT
#define CHPL_ATTRIBUTE_MALLOC
#endif


static ___always_inline void* chpl_calloc(size_t n, size_t size)
  CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

static ___always_inline void* chpl_malloc(size_t size)
  CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

static ___always_inline void* chpl_memalign(size_t boundary, size_t size)
  CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

static ___always_inline void* chpl_realloc(void* ptr, size_t size)
  CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

static ___always_inline void chpl_free(void* ptr);


// runtime/include/mem/*/chpl-mem-impl.h defines
// chpl_calloc, chpl_malloc, chpl_realloc, chpl_free
// with the same signatures as the standard functions
// and no additional error checking.
#include "chpl-mem-impl.h"

// If we use weak symbols/linker scripts, for glibc we would need to define:
//  malloc, free, cfree, calloc, realloc,
//  memalign, posix_memalign, aligned_alloc,
//  malloc_usable_size.
// we could use ld's --wrap argument to achieve it.

// we always create symbols for calloc/malloc/realloc so that
// we could use a linker script or malloc hook.
void* chpl_calloc_sym(size_t n, size_t size)
    CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void* chpl_malloc_sym(size_t size)
  CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void* chpl_memalign_sym(size_t boundary, size_t size)
  CHPL_ATTRIBUTE_MALLOC CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void* chpl_realloc_sym(void* ptr, size_t size)
  CHPL_ATTRIBUTE_WARN_UNUSED_RESULT;

void chpl_free_sym(void* ptr);


// This function sets up malloc hooks (if possible or useful)
void chpl_mem_replace_malloc_if_needed(void);


void chpl_mem_init(void);
void chpl_mem_exit(void);

int chpl_mem_inited(void);


static ___always_inline
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

static ___always_inline
void* chpl_mem_alloc(size_t size, chpl_mem_descInt_t description,
                     int32_t lineno, c_string filename) {
  return chpl_mem_allocMany(1, size, description, lineno, filename);
}

static ___always_inline
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

static ___always_inline
void* chpl_mem_calloc(size_t size, chpl_mem_descInt_t description,
                      int32_t lineno, c_string filename) {
  return chpl_mem_allocManyZero(1, size, description, lineno, filename);
}

static ___always_inline
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

static ___always_inline
void chpl_mem_free(void* memAlloc, int32_t lineno, c_string filename) {
  chpl_memhook_free_pre(memAlloc, lineno, filename);
  chpl_free(memAlloc);
}

// Provide a handle to instrument Chapel calls to memcpy.
static ___always_inline
void* chpl_memcpy(void* dest, const void* src, size_t num)
{
  assert(dest != src);
  return memcpy(dest, src, num);
}

// free a c_string_copy, no error checking.
// The argument type is explicitly c_string_copy, since only an "owned" string
// should be freed.
static ___always_inline
void chpl_rt_free_c_string_copy(c_string_copy *s, int32_t lineno, c_string filename)  {
  assert(*s!=NULL);
  chpl_mem_free((void *) *s, lineno, filename);
  *s = NULL;
}

// free a c_string (deprecated)
// This function is needed only because NewString.chpl uses the c_string type.
// c_strings are "unowned" so should not be freed, but NewString.chpl was written
// before this distinction was made.
static ___always_inline
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
