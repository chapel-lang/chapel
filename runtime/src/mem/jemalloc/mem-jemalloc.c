/*
 * Copyright 2004-2016 Cray Inc.
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

#include "chplrt.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpltypes.h"
#include "error.h"


// TODO it'd be great if there was some way for us to set the number of arenas.
// jemalloc's default is `4 * logicalCPUs` which is fine for fifo, but for
// qthreads and muxed, we know how many pthreads will be created at startup.
// The number of arenas can be set with je_malloc_conf, but that's a compile
// time constant. We can also set it with JE_MALLOC_CONF, but I think that has
// to be prior to program execution, I think by the time we get here, we're
// already too late. I was hoping we just had to set it before the first call
// to an allocation routine, but that doesn't appear to be the case.
//
// A hacky (but easy/straightforward) solution would be do modify jemalloc
// source. For qthreads we could get rid of the 4x multiplier and just default
// to the number of logical cpus. See jemalloc.c:1277. Might be worth testing
// the performance difference in a playground just to see if it's worth
// investigating further.

// TODO put these into a struct?
static void*  heap_base = NULL;
static size_t heap_size = 0;
static size_t cur_heap_offset = 0;
static pthread_mutex_t chunk_alloc_lock;


// compute aligned index into our shared heap, alignment must be a power of 2
static inline void* alignHelper(void* base_ptr, size_t offset, size_t alignment) {
  uintptr_t p;

  assert(alignment && ((alignment & (alignment -1)) == 0));
  p = (uintptr_t)base_ptr + offset;
  p = (p + alignment - 1) & ~(alignment - 1);
  return(void*) p;
}


// *** Chunk hook replacements *** //
// See http://www.canonware.com/download/jemalloc/jemalloc-latest/doc/jemalloc.html#arena.i.chunk_hooks

// Our chunk replacement hook for allocations (Essentially a replacement for
// mmap/sbrk.) Grab memory out of the shared heap and give it to jemalloc.
static void* chunk_alloc(void *chunk, size_t size, size_t alignment, bool *zero, bool *commit, unsigned arena_ind) {

  void* cur_chunk_base = NULL;
  size_t cur_heap_size;

  // this function can be called concurrently and it looks like jemalloc
  // doesn't call it inside a lock, so we need to protect it ourselves
  pthread_mutex_lock(&chunk_alloc_lock);

  // compute our current aligned pointer into the shared heap
  //
  //   jemalloc 4.0.4 man: "The alignment parameter is always a power of two at
  //   least as large as the chunk size."
  cur_chunk_base = alignHelper(heap_base, cur_heap_offset, alignment);

  // jemalloc 4.0.4 man: "If chunk is not NULL, the returned pointer must be
  // chunk on success or NULL on error"
  if (chunk && chunk != cur_chunk_base) {
    pthread_mutex_unlock(&chunk_alloc_lock);
    return NULL;
  }

  cur_heap_size = (uintptr_t)cur_chunk_base - (uintptr_t)heap_base;

  // If there's not enough space on the heap for this allocation, return NULL
  if (size > heap_size - cur_heap_size) {
    pthread_mutex_unlock(&chunk_alloc_lock);
    return NULL;
  }

  // Update the current pointer, now that we've past any early returns.
  cur_heap_offset = cur_heap_size + size;

  // now that cur_heap_offset is updated, we can unlock
  pthread_mutex_unlock(&chunk_alloc_lock);

  // jemalloc 4.0.4 man: "Zeroing is mandatory if *zero is true upon entry."
  if (*zero) {
     memset(cur_chunk_base, 0, size);
  }

  // Commit is not relevant for linux/darwin, but jemalloc wants us to set it
  *commit = true;

  return cur_chunk_base;
}

//
// Returning true indicates an opt-out of these hooks. For dalloc, this means
// that we never get memory back from jemalloc and we just let it re-use it in
// the future.
//
static bool null_dalloc(void *chunk, size_t size, bool committed, unsigned arena_ind) {
  return true;
}
static bool null_commit(void *chunk, size_t size, size_t offset, size_t length, unsigned arena_ind) {
  return true;
}
static bool null_decommit(void *chunk, size_t size, size_t offset, size_t length, unsigned arena_ind) {
  return true;
}
static bool null_purge(void *chunk, size_t size, size_t offset, size_t length, unsigned arena_ind) {
  return true;
}
static bool null_split(void *chunk, size_t size, size_t size_a, size_t size_b, bool committed, unsigned arena_ind) {
  return true;
}
static bool null_merge(void *chunk_a, size_t size_a, void *chunk_b, size_t size_b, bool committed, unsigned arena_ind) {
  return true;
}
// *** End chunk hook replacements *** //


// get the number of arenas
static size_t get_num_arenas(void) {
  size_t narenas;
  size_t sz;
  sz = sizeof(narenas);
  if (je_mallctl("opt.narenas", &narenas, &sz, NULL, 0) != 0) {
    chpl_internal_error("could not get number of arenas from jemalloc");
  }
  return narenas;
}

// initialize our arenas (this is required to be able to set the chunk hooks)
static void initialize_arenas(void) {
  size_t s_narenas;
  unsigned narenas;
  unsigned arena;

  // "thread.arena" takes an unsigned, but num_arenas is a size_t.
  s_narenas = get_num_arenas();
  if (s_narenas > (size_t) UINT_MAX) {
    chpl_internal_error("narenas too large to fit into unsigned");
  }
  narenas = (unsigned) s_narenas;

  // for each non-zero arena, set the current thread to use it (this
  // initializes each arena). arena 0 is automatically initialized.
  //
  //   jemalloc 4.0.4 man: "If the specified arena was not initialized
  //   beforehand, it will be automatically initialized as a side effect of
  //   calling this interface."
  for (arena=1; arena<narenas; arena++) {
    if (je_mallctl("thread.arena", NULL, NULL, &arena, sizeof(arena)) != 0) {
      chpl_internal_error("could not change current thread's arena");
    }
  }

  // then set the current thread back to using arena 0
  arena = 0;
  if (je_mallctl("thread.arena", NULL, NULL, &arena, sizeof(arena)) != 0) {
      chpl_internal_error("could not change current thread's arena back to 0");
  }
}


// replace the chunk hooks for each arena with the hooks we provided above
static void replaceChunkHooks(void) {
  size_t narenas;
  size_t arena;

  // set the pointers for the new_hooks to our above functions
  chunk_hooks_t new_hooks = {
    chunk_alloc,
    null_dalloc,
    null_commit,
    null_decommit,
    null_purge,
    null_split,
    null_merge
  };

  // for each arena, change the chunk hooks
  narenas = get_num_arenas();
  for (arena=0; arena<narenas; arena++) {
    char path[128];
    snprintf(path, sizeof(path), "arena.%zu.chunk_hooks", arena);
    if (je_mallctl(path, NULL, NULL, &new_hooks, sizeof(chunk_hooks_t)) != 0) {
      chpl_internal_error("could not update the chunk hooks");
    }
  }
}

static void useUpMemNotInHeap(void) {
  // grab (and leak) whatever memory jemalloc got on it's own, that's not in
  // our shared heap
  //
  //   jemalloc 4.0.4 man: "arenas may have already created chunks prior to the
  //   application having an opportunity to take over chunk allocation."
  //
  // Note that this will also allow jemalloc to initialize itself since
  char* p = NULL;
  do {
    // TODO use a larger value than size_t here (must be smaller than
    // "arenas.hchunk.0.size" though
    if ((p = je_malloc(sizeof(size_t))) == NULL) {
      chpl_internal_error("could not use up memory outside of shared heap");
    }
  } while ((p != NULL && (p < (char*) heap_base || p > (char*) heap_base + heap_size)));
}

// Have jemalloc use our shared heap. Initialize all the arenas, then replace
// the chunk hooks with our custom ones, and finally use up any memory jemalloc
// got from the system that's not in our shared heap.
static void initializeSharedHeap(void) {
  initialize_arenas();

  replaceChunkHooks();

  useUpMemNotInHeap();
}

void chpl_mem_layerInit(void) {
  void* heap_base_;
  size_t heap_size_;

  chpl_comm_desired_shared_heap(&heap_base_, &heap_size_);
  if (heap_base_ != NULL && heap_size_ == 0) {
    chpl_internal_error("if heap address is specified, size must be also");
  }

  // If we have a shared heap, initialize our shared heap. This will take care
  // of initializing jemalloc. If we're not using a shared heap, do a first
  // allocation to allow jemaloc to set up:
  //
  //   jemalloc 4.0.4 man: "Once, when the first call is made to one of the
  //   memory allocation routines, the allocator initializes its internals"
  if (heap_base_ != NULL) {
    heap_base = heap_base_;
    heap_size = heap_size_;
    cur_heap_offset = 0;
    if (pthread_mutex_init(&chunk_alloc_lock, NULL) != 0) {
      chpl_internal_error("cannot init chunk_alloc lock");
    }
    initializeSharedHeap();
  } else {
    void* p;
    if ((p = je_malloc(1)) == NULL) {
      chpl_internal_error("cannot init heap: je_malloc() failed");
    }
    je_free(p);
  }
}


void chpl_mem_layerExit(void) {
  if (heap_base != NULL) {
    // ignore errors, we're exiting anyways
    pthread_mutex_destroy(&chunk_alloc_lock);
  }
}
