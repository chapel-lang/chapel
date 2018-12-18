/*
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

#ifndef _chpl_mem_array_H_
#define _chpl_mem_array_H_

#ifndef LAUNCHER

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chpl-mem-hook.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "error.h"


static inline
chpl_bool chpl_mem_size_justifies_comm_alloc(size_t size) {
  //
  // Don't try to use comm layer allocation unless the size is beyond
  // the comm layer threshold and enough pages to make localization
  // worthwhile.
  //
  return (size >= chpl_comm_regMemAllocThreshold()
          && size >= 2 * chpl_comm_regMemHeapPageSize());
}


static inline
void* chpl_mem_array_alloc(size_t nmemb, size_t eltSize, c_sublocid_t subloc,
                           chpl_bool* callAgain, void* repeat_p,
                           int32_t lineno, int32_t filename) {
  const size_t size = nmemb * eltSize;
  void* p;

  //
  // Temporarily, to support dynamic array registration, we accept a
  // couple of additional arguments.  On a first call, to allocate,
  // pass NULL for repeat_p.  This function will return a pointer to
  // the allocated memory and either true or false in *callAgain.  If
  // false then the allocation procedure is complete.  If true, then
  // after initializing the memory the caller should call us again,
  // repeating the original arguments and passing the pointer returned
  // by the first call in repeat_p.  We will then call the comm layer
  // post-alloc function.
  //
  if (repeat_p == NULL) {
    //
    // Allocate.
    //
    chpl_memhook_malloc_pre(nmemb, eltSize, CHPL_RT_MD_ARRAY_ELEMENTS,
                            lineno, filename);

    p = NULL;
    *callAgain = false;
    if (chpl_mem_size_justifies_comm_alloc(size)) {
      p = chpl_comm_regMemAlloc(size, CHPL_RT_MD_ARRAY_ELEMENTS,
                                lineno, filename);
      if (p != NULL) {
        *callAgain = true;
      }
    }

    if (p == NULL) {
      p = chpl_malloc(nmemb * eltSize);
    }

    chpl_memhook_malloc_post(p, nmemb, eltSize, CHPL_RT_MD_ARRAY_ELEMENTS,
                             lineno, filename);
  } else {
    //
    // do comm layer post-allocation, if we got the memory from there.
    //
    p = repeat_p;
    chpl_comm_regMemPostAlloc(p, size);
  }

  return p;
}


static inline
void* chpl_mem_wide_array_alloc(int32_t dstNode, size_t nmemb, size_t eltSize,
                                c_sublocid_t subloc,
                                chpl_bool* callAgain, void* repeat_p,
                                int32_t lineno, int32_t filename) {
  if (dstNode != chpl_nodeID)
    chpl_error("array vector data is not local", lineno, filename);
  return chpl_mem_array_alloc(nmemb, eltSize, subloc, callAgain, repeat_p,
                              lineno, filename);
}


static inline
void chpl_mem_array_free(void* p,
                         size_t nmemb, size_t eltSize,
                         int32_t lineno, int32_t filename) {
  //
  // If the size indicates we might have gotten this memory from the
  // comm layer then try to free it there.  If not, or if so but the
  // comm layer says it didn't come from there, free it in the memory
  // layer.
  //
  chpl_memhook_free_pre(p, lineno, filename);

  const size_t size = nmemb * eltSize;
  if (chpl_mem_size_justifies_comm_alloc(size)
      && chpl_comm_regMemFree(p, size)) {
    return;
  }

  chpl_free(p);
}


static inline
void chpl_mem_wide_array_free(int32_t dstNode, void* p,
                              size_t nmemb, size_t eltSize,
                              int32_t lineno, int32_t filename) {
  if (dstNode != chpl_nodeID)
    chpl_error("array vector data is not local", lineno, filename);
  chpl_mem_array_free(p, nmemb, eltSize, lineno, filename);
}

#endif // LAUNCHER

#endif
