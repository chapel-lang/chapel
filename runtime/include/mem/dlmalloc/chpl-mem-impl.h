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

/* dlmalloc memory function implementation */

#include "dlmalloc.h"

extern mspace chpl_dlmalloc_heap;

static ___always_inline void* chpl_calloc(size_t n, size_t size) {
  return mspace_calloc(chpl_dlmalloc_heap,n,size);
}

static ___always_inline void* chpl_malloc(size_t size) {
  return mspace_malloc(chpl_dlmalloc_heap, size);
}

static ___always_inline void* chpl_realloc(void* ptr, size_t size) {
  return mspace_realloc(chpl_dlmalloc_heap, ptr, size);
}

static ___always_inline void chpl_free(void* ptr) {
  mspace_free(chpl_dlmalloc_heap, ptr);
}

