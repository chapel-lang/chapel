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

/* jemalloc memory function implementation */
#ifndef _chpl_mem_impl_H_
#define _chpl_mem_impl_H_

// jemalloc references malloc and friends internally so we need to quiet our
// warnings before including jemalloc.h
#undef malloc
#undef calloc
#undef realloc
#undef free
#undef _chpl_mem_warning_macros_h_


#include "jemalloc.h"

static inline void* chpl_calloc(size_t n, size_t size) {
  return je_calloc(n,size);
}

static inline void* chpl_malloc(size_t size) {
  return je_malloc(size);
}

static inline void* chpl_memalign(size_t boundary, size_t size) {
  void* ret = NULL;
  int rc;
  rc = je_posix_memalign(&ret, boundary, size);
  if( rc == 0 ) return ret;
  else return NULL;
}

static inline void* chpl_realloc(void* ptr, size_t size) {
  return je_realloc(ptr, size);
}

static inline void chpl_free(void* ptr) {
  je_free(ptr);
}

static inline size_t chpl_goodAllocSize(size_t minSize) {
  // TODO (EJR 12/17/15): can/should we use nallocx()? If so, this will require
  // using the extended API. Note that the extended API has undefined behavior
  // for allocations of size 0 so I think a size 0 check will have to be added
  // to all the above alloc/realloc calls. I'll probably wait till we have some
  // performance numbers before making that change to make sure there's no
  // negative impact of doing so.
  return minSize;
}

// TODO (EJR 12/17/15): if we end up using the extended API should we consider
// adding something like `chpl_sized_free()` sized deallocations that maps down
// to `sdallocx()`


// Now that we've defined our functions, turn the warnings back on.
#include "chpl-mem-warning-macros.h"

#endif
