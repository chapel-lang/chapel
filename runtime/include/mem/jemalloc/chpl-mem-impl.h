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

/* jemalloc memory function implementation */
#ifndef _chpl_mem_impl_H_
#define _chpl_mem_impl_H_

#include "chpl-mem-jemalloc-prefix.h"

#include "jemalloc/jemalloc.h"


// Default configuration of jemalloc names the routines
// like je_malloc, je_free, etc; but the Chapel configuration
// uses the prefix chpl_je_. Use the CHPL_JE_ macro to
// paste the appropriate prefix (je_ or chpl_je_).
#define CHPL_JE_TOK_PASTE_INTERNAL(a, b) a ## b
#define CHPL_JE_TOK_PASTE(a, b) CHPL_JE_TOK_PASTE_INTERNAL(a,b)
#define CHPL_JE_(NAME) CHPL_JE_TOK_PASTE(CHPL_JEMALLOC_PREFIX, NAME)

#define CHPL_JE_MALLOCX CHPL_JE_(mallocx)
#define CHPL_JE_RALLOCX CHPL_JE_(rallocx)
#define CHPL_JE_DALLOCX CHPL_JE_(dallocx)
#define CHPL_JE_NALLOCX CHPL_JE_(nallocx)
#define CHPL_JE_MALLCTL CHPL_JE_(mallctl)


#define MALLOCX_NO_FLAGS 0
// Determine which arena to use. For large allocations (32 MiB) use a dedicated
// arena to reduce fragmentation
extern unsigned CHPL_JE_LG_ARENA;
static inline int CHPL_JE_MALLOCX_ARENA_FLAG(size_t size) {
  if (size >= ((size_t) 32 << 20)) {
    return MALLOCX_ARENA(CHPL_JE_LG_ARENA);
  }
  return MALLOCX_NO_FLAGS;
}


// jemalloc extended API requires non-0 sized allocations and non-NULL frees.
// Use a min size of 1 (instead of returning NULL) since our memory API
// interprets a NULL return as an out-of-memory sentinel
static inline size_t minSize(size_t size) {
  if (size == 0) {
    size = 1;
  }
  return size;
}

static inline void* chpl_calloc(size_t n, size_t size) {
  size_t nSize = minSize(n*size);
  return CHPL_JE_MALLOCX(nSize, MALLOCX_ZERO | CHPL_JE_MALLOCX_ARENA_FLAG(nSize));
}

static inline void* chpl_malloc(size_t size) {
  size = minSize(size);
  return CHPL_JE_MALLOCX(size, CHPL_JE_MALLOCX_ARENA_FLAG(size));
}

static inline void* chpl_memalign(size_t boundary, size_t size) {
  size = minSize(size);
  return CHPL_JE_MALLOCX(size, MALLOCX_ALIGN(boundary) | CHPL_JE_MALLOCX_ARENA_FLAG(size));
}

static inline void* chpl_realloc(void* ptr, size_t size) {
  if (ptr == NULL) {
    size = minSize(size);
    return CHPL_JE_MALLOCX(size, CHPL_JE_MALLOCX_ARENA_FLAG(size));
  }
  if (size == 0) {
    CHPL_JE_DALLOCX(ptr, MALLOCX_NO_FLAGS);
    return NULL;
  }
  return CHPL_JE_RALLOCX(ptr, size, CHPL_JE_MALLOCX_ARENA_FLAG(size));
}

static inline void chpl_free(void* ptr) {
  if (ptr != NULL) {
    CHPL_JE_DALLOCX(ptr, MALLOCX_NO_FLAGS);
  }
}

static inline size_t chpl_good_alloc_size(size_t minSize) {
  if (minSize == 0) {
    return 0;
  }
  return CHPL_JE_NALLOCX(minSize, MALLOCX_NO_FLAGS);
}

#endif
