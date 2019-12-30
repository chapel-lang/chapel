/*
 * Copyright 2004-2020 Cray Inc.
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

/* cstdlib memory function implementation */
#ifndef _chpl_mem_impl_H_
#define _chpl_mem_impl_H_

// Uses the system allocator
#include "chpl-mem-sys.h"

#if defined(__APPLE__)
#include <malloc/malloc.h>
#endif

static inline void* chpl_calloc(size_t n, size_t size) {
  return sys_calloc(n,size);
}

static inline void* chpl_malloc(size_t size) {
  return sys_malloc(size);
}

static inline void* chpl_memalign(size_t boundary, size_t size) {
  return sys_memalign(boundary, size);
}

static inline void* chpl_realloc(void* ptr, size_t size) {
  return sys_realloc(ptr,size);
}

static inline void chpl_free(void* ptr) {
  sys_free(ptr);
}

// malloc_good_size is OSX specific unfortunately. On other platforms just
// return minSize.
static inline size_t chpl_good_alloc_size(size_t minSize) {
#if defined(__APPLE__)
  return malloc_good_size(minSize);
#else
  return minSize;
#endif
}

#define CHPL_USING_CSTDLIB_MALLOC 1

#endif
