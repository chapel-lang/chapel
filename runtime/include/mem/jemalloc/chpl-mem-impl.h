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

#define CHPL_JE_CALLOC CHPL_JE_(calloc)
#define CHPL_JE_MALLOC CHPL_JE_(malloc)
#define CHPL_JE_POSIX_MEMALIGN CHPL_JE_(posix_memalign)
#define CHPL_JE_REALLOC CHPL_JE_(realloc)
#define CHPL_JE_FREE CHPL_JE_(free)
#define CHPL_JE_NALLOCX CHPL_JE_(nallocx)
#define CHPL_JE_MALLCTL CHPL_JE_(mallctl)


#define MALLOCX_NO_FLAGS 0

static inline void* chpl_calloc(size_t n, size_t size) {
  return CHPL_JE_CALLOC(n,size);
}

static inline void* chpl_malloc(size_t size) {
  return CHPL_JE_MALLOC(size);
}

static inline void* chpl_memalign(size_t boundary, size_t size) {
  void* ret = NULL;
  int rc;
  rc = CHPL_JE_POSIX_MEMALIGN(&ret, boundary, size);
  if( rc == 0 ) return ret;
  else return NULL;
}

static inline void* chpl_realloc(void* ptr, size_t size) {
  return CHPL_JE_REALLOC(ptr, size);
}

static inline void chpl_free(void* ptr) {
  CHPL_JE_FREE(ptr);
}

static inline size_t chpl_good_alloc_size(size_t minSize) {
  if (minSize == 0) { return 0; }
  return CHPL_JE_NALLOCX(minSize, MALLOCX_NO_FLAGS);
}

#endif
