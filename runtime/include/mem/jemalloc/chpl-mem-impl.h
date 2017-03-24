/*
 * Copyright 2004-2017 Cray Inc.
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

// jemalloc.h references the token "malloc" (but not the actual function) and
// our warning macros mess up jemalloc's use of it.
#include "chpl-mem-no-warning-macros.h"
#include "jemalloc.h"
#include "chpl-mem-warning-macros.h"
#include "chpltypes.h"

#define MALLOCX_NO_FLAGS 0


static inline void* chpl_calloc(size_t n, size_t size) {
  return chpl_je_calloc(n,size);
}

static inline void* chpl_malloc(size_t size) {
  return chpl_je_malloc(size);
}

static inline void* chpl_memalign(size_t boundary, size_t size) {
  void* ret = NULL;
  int rc;
  rc = chpl_je_posix_memalign(&ret, boundary, size);
  if( rc == 0 ) return ret;
  else return NULL;
}

static inline void* chpl_realloc(void* ptr, size_t size) {
  return chpl_je_realloc(ptr, size);
}

static inline void chpl_free(void* ptr) {
  chpl_je_free(ptr);
}

static inline size_t chpl_good_alloc_size(size_t minSize) {
  if (minSize == 0) { return 0; }
  return chpl_je_nallocx(minSize, MALLOCX_NO_FLAGS);
}


chpl_bool chpl_mem_impl_alloc_localizes(void);

#define CHPL_MEM_IMPL_ALLOC_LOCALIZES() chpl_mem_impl_alloc_localizes()


// TODO (EJR 03/11/16): Can/should we consider using the extended API? See JIRA
// issue 190 (https://chapel.atlassian.net/browse/CHAPEL-190) for more info.

#endif
