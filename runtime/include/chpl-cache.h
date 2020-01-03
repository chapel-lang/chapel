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

#ifndef _chpl_cache_h_
#define _chpl_cache_h_

#include "chpltypes.h"
#include "chpl-atomics.h"
#include "chpl-comm.h" // to get HAS_CHPL_CACHE_FNS via chpl-comm-task-decls.h
#include "chpl-tasks.h"

#ifdef HAS_CHPL_CACHE_FNS
// This is a cache for remote data.

// Is the cache enabled? (set at compile time)
extern const int CHPL_CACHE_REMOTE;

static inline
int chpl_cache_enabled(void)
{
  // The remote cache uses thread local storage, so if tasks can migrate
  // between threads we lose out ability to correctly fence.
  return CHPL_CACHE_REMOTE && !chpl_task_canMigrateThreads();
}


// Initialize the remote data cache layer. 
void chpl_cache_init(void);
void chpl_cache_exit(void);

// If release is set, waits on any pending puts in the cache.
// If acquire is set, sets this task's last acquire fence to 
// the cache's current request number.
void chpl_cache_fence(int acquire, int release, int ln, int32_t fn);

// "acquire" barrier or fence -> discard pre-fetched GET values
static inline
void chpl_cache_acquire(int ln, int32_t fn)
{
  if (chpl_cache_enabled()) chpl_cache_fence(1, 0, ln, fn);
}
// "release" barrier or fence -> complete pending PUTs
static inline
void chpl_cache_release(int ln, int32_t fn)
{
  if (chpl_cache_enabled()) chpl_cache_fence(0, 1, ln, fn);
}


// These are the functions that the generated code should be eventually
// calling on a put or a get.
void chpl_cache_comm_put(void* addr, c_nodeid_t node, void* raddr,
                         size_t size, int32_t commID, int ln, int32_t fn);
void chpl_cache_comm_get(void *addr, c_nodeid_t node, void* raddr,
                         size_t size, int32_t commID, int ln, int32_t fn);
void chpl_cache_comm_prefetch(c_nodeid_t node, void* raddr,
                              size_t size, int32_t commID, int ln, int32_t fn);
void  chpl_cache_comm_get_strd(
                   void *addr, void *dststr, c_nodeid_t node, void *raddr,
                   void *srcstr, void *count, int32_t strlevels,
                   size_t elemSize, int32_t commID, int ln, int32_t fn);
void  chpl_cache_comm_put_strd(
                      void *addr, void *dststr, c_nodeid_t node, void *raddr,
                      void *srcstr, void *count, int32_t strlevels,
                      size_t elemSize, int32_t commID, int ln, int32_t fn);
void chpl_cache_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                                   size_t size, int32_t commID, int ln, int32_t fn);
void chpl_cache_comm_get_unordered(void *addr, c_nodeid_t node, void* raddr,
                                   size_t size, int32_t commID, int ln, int32_t fn);
void chpl_cache_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                      c_nodeid_t srcnode, void* srcaddr,
                                      size_t size, int32_t commID,
                                      int ln, int32_t fn);
void chpl_cache_comm_getput_unordered_task_fence(void);

// For debugging.
void chpl_cache_print(void);
void chpl_cache_assert_released(void);

#endif
// ifdef HAS_CHPL_CACHE_FNS


#endif

