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

#ifndef _chpl_mem_hook_H_
#define _chpl_mem_hook_H_

#ifndef LAUNCHER

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "arg.h"
#include "chpltypes.h"
#include "error.h"

// Need memory tracking prototypes for inlined memory routines
#include "chplmemtrack.h"

#ifdef __cplusplus
extern "C" {
#endif

// CHPL_MEMHOOKS_ACTIVE=1 will enable the memory hooks;
// CHPL_MEMHOOKS_ACTIVE will be set to 1 if CHPL_DEBUG is defined;
// or if CHPL_OPTIMIZE is not defined.
// If CHPL_OPTIMIZE is defined and CHPL_DEBUG is not defined,
// we set CHPL_MEMHOOKS_ACTIVE to chpl_memTrack, so that memory tracking
// can still be activated at run-time.
#ifndef CHPL_MEMHOOKS_ACTIVE

#ifdef CHPL_DEBUG
#define CHPL_MEMHOOKS_ACTIVE 1
#else
#ifdef CHPL_OPTIMIZE
#define CHPL_MEMHOOKS_ACTIVE chpl_memTrack
#else
#define CHPL_MEMHOOKS_ACTIVE 1
#endif
#endif

#endif

// Returns the starting number for memory descriptors for use by Chapel code.
static inline
chpl_mem_descInt_t chpl_memhook_md_num(void)
{
  return CHPL_RT_MD_NUM;
}


void chpl_memhook_check_pre(size_t number, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename);
void chpl_memhook_check_post(void* memAlloc,
                             chpl_mem_descInt_t description,
                             int32_t lineno, int32_t filename);


static inline
void chpl_memhook_malloc_pre(size_t number, size_t size,
                             chpl_mem_descInt_t description,
                             int32_t lineno, int32_t filename) {
  if (CHPL_MEMHOOKS_ACTIVE)
    chpl_memhook_check_pre(number, size, description, lineno, filename);
}


static inline
void chpl_memhook_malloc_post(void* memAlloc,
                              size_t number, size_t size,
                              chpl_mem_descInt_t description,
                              int32_t lineno, int32_t filename) {
  if (CHPL_MEMHOOKS_ACTIVE || memAlloc == NULL)
    chpl_memhook_check_post(memAlloc, description, lineno, filename);
  if (CHPL_MEMHOOKS_ACTIVE)
    chpl_track_malloc(memAlloc, number, size, description, lineno, filename);
}


static inline
void chpl_memhook_free_pre(void* memAlloc,
                           int32_t lineno, int32_t filename) {
  if (CHPL_MEMHOOKS_ACTIVE) {
    // call this one just to check heap is initialized.
    chpl_memhook_check_pre(0, 0, 0, lineno, filename);
    chpl_track_free(memAlloc, lineno, filename);
  }
}


static inline
void chpl_memhook_realloc_pre(void* memAlloc, size_t size,
                              chpl_mem_descInt_t description,
                              int32_t lineno, int32_t filename) {
  if (CHPL_MEMHOOKS_ACTIVE) {
    chpl_memhook_check_pre(1, size, description, lineno, filename);
    chpl_track_realloc_pre(memAlloc, size, description, lineno, filename);
  }
}


static inline
void chpl_memhook_realloc_post(void* moreMemAlloc, void* memAlloc,
                               size_t size,
                               chpl_mem_descInt_t description,
                               int32_t lineno, int32_t filename) {
  if (CHPL_MEMHOOKS_ACTIVE || moreMemAlloc == NULL)
    chpl_memhook_check_post(moreMemAlloc, description, lineno, filename);
  if (CHPL_MEMHOOKS_ACTIVE)
    chpl_track_realloc_post(moreMemAlloc, memAlloc, size, description,
                       lineno, filename);
}

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // LAUNCHER

#endif // _chpl_mem_hook_H_
