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

#ifndef _chpl_mem_consistency_h_
#define _chpl_mem_consistency_h_

#include "chpl-atomics.h" // for memory_order

#include "chpl-cache.h" // for chpl_cache_release, chpl_cache_acquire

// These functions support memory consistency with the remote
// data cache. They do not need to do anything if the cache is
// not enabled.

// When do these fences need to be called?

// Release fence before doing a sync variable operation
// in case that sync variable operation triggers a remote
// task to read values we are supposed to have written.

// Acquire fence after doing a sync variable operation
// in case the sync variable indicated to us that it is
// now safe to read a remote value (that another task updated).

// When spawning a task (or 'on' clause), make sure that any pending
// writes have completed (so that when that task or 'on' clause
// does a 'get', the written value will be available).

// When joining with a task (or 'on' clause), make sure that
// any values written in that task will be reflected in the current
// task (so if the task changed a value, a subsequent 'get' will
// read the new value).

// When starting a task or 'on' clause, make sure that any cached
// values from before the task started (e.g. if a thread is reused)
// are not used again, since values written before the task started
// need to be available to 'get'.

// When completing a task or 'on' clause, make sure that any cached
// writes have completed, so that the acquire fence when joining with
// this task will allow the joined task to read values written by this
// task.

static inline
void chpl_rmem_consist_release(int ln, int32_t fn)
{
#ifdef HAS_CHPL_CACHE_FNS
  chpl_cache_release(ln, fn);
#endif
}

static inline
void chpl_rmem_consist_acquire(int ln, int32_t fn)
{
#ifdef HAS_CHPL_CACHE_FNS
  chpl_cache_acquire(ln, fn);
#endif
}


// These should just call chpl_cache_release or chpl_cache_acquire. They
// exist so that we have a single place to put any required memory consistency 
// operations/fences. 

static inline
//void chpl_atomic_rmem_fence_pre(memory_order order, int ln, int32_t fn) {
void chpl_rmem_consist_maybe_release(memory_order order, int ln, int32_t fn) {
  if(order==memory_order_acquire || order==memory_order_relaxed) {
    // do nothing
  } else {
    // for release or sequentially consistent, flush pending writes
    chpl_rmem_consist_release(ln, fn);
  }
}
static inline
//void chpl_atomic_rmem_fence_post(memory_order order, int ln, int32_t fn) {
void chpl_rmem_consist_maybe_acquire(memory_order order, int ln, int32_t fn) {
  if(order==memory_order_release || order==memory_order_relaxed) {
    // do nothing
  } else {
    // for acquire or sequentially consistent, do not reuse any cached values
    chpl_rmem_consist_acquire(ln, fn);
  }
}

static inline
//void chpl_atomic_rmem_fence(memory_order order, int ln, int32_t fn) {
void chpl_rmem_consist_fence(memory_order order, int ln, int32_t fn) {
  if(order==memory_order_relaxed) {
    // do nothing
  } else {
    int acquire = 1;
    int release = 1;
    if( order == memory_order_acquire ) {
      release = 0;
    } else if( order == memory_order_release ) {
      acquire = 0;
    }

    if( release ) chpl_rmem_consist_release(ln, fn);
    if( acquire ) chpl_rmem_consist_acquire(ln, fn);
    chpl_atomic_thread_fence(order);
  }
}


#endif
