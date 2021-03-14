/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_cache_h_
#define _chpl_cache_h_

#include "chpltypes.h"
#include "chpl-atomics.h"
#include "chpl-comm.h" // to get HAS_CHPL_CACHE_FNS via chpl-comm-task-decls.h
#include "chpl-env.h"
#include "chpl-tasks.h"
#include "error.h"

#ifdef HAS_CHPL_CACHE_FNS
// This is a cache for remote data.

#ifdef __cplusplus
extern "C" {
#endif

// Is the cache supposed to be enabled? (set at compile time)
extern const int CHPL_CACHE_REMOTE;

#if defined(__SANITIZE_ADDRESS__)
#define CHPL_ASAN 1
#endif

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define CHPL_ASAN 1
#endif
#endif

#if !defined(CHPL_ASAN)
#define CHPL_ASAN 0
#endif

static inline
void chpl_cache_warn_if_disabled(void)
{
  if (CHPL_CACHE_REMOTE && chpl_nodeID == 0 &&
      !chpl_env_rt_get_bool("CACHE_QUIET", false)) {
    if (CHPL_ASAN) {
      chpl_warning("Disabling --cache-remote due to incompatibility with "
                   "AddressSanitizer (quiet with CHPL_RT_CACHE_QUIET=true)", 0, 0);
    } else if (chpl_task_canMigrateThreads()) {
      chpl_warning("Disabling --cache-remote because tasks can migrate "
                   "threads (quiet with CHPL_RT_CACHE_QUIET=true)", 0, 0);
    }
  }
}

static inline
int chpl_cache_enabled(void)
{
  // The remote cache is not compatible with ASan, and it uses thread local
  // storage, so if tasks can migrate between threads we lose our ability to
  // correctly fence.
  return CHPL_CACHE_REMOTE && !CHPL_ASAN && !chpl_task_canMigrateThreads();
}
#undef CHPL_ASAN


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
void chpl_cache_print_stats(void);
// just stores 0s in the cache; here to exercise the data structures
// returns 1 if the data was cached
int chpl_cache_mock_get(c_nodeid_t node, uint64_t raddr, size_t size);
void chpl_cache_invalidate(c_nodeid_t node, void* raddr, size_t size,
                           int ln, int32_t fn);

#ifdef __cplusplus
}
#endif

#endif
// ifdef HAS_CHPL_CACHE_FNS


#endif

