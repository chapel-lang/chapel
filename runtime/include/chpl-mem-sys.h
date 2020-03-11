/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_mem_sys_H_
#define _chpl_mem_sys_H_

#include <stdlib.h>
#ifdef __GLIBC__
#include <malloc.h>  // get memalign
#endif

static inline void* sys_calloc(size_t n, size_t size) {
  return calloc(n, size);
}

static inline void* sys_malloc(size_t size) {
  return malloc(size);
}

static inline void* sys_memalign(size_t boundary, size_t size) {
#ifdef __GLIBC__
  return memalign(boundary, size);
#else
  void* ret = NULL;
  int rc;
  rc = posix_memalign(&ret, boundary, size);
  if( rc == 0 ) return ret;
  else return NULL;
#endif
}

static inline void* sys_realloc(void* ptr, size_t size) {
  return realloc(ptr, size);
}

static inline void sys_free(void* ptr) {
  free(ptr);
}

#endif
