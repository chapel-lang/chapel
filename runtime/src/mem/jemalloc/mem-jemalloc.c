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

#include "chplrt.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chplsys.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "error.h"


//
// Information about shared heaps gotten from the comm layer.
//
typedef struct shared_heap {
  void* base;
  size_t size;
  size_t cur_offset;
  pthread_mutex_t alloc_lock;
} shared_heap_t;

static shared_heap_t* heaps;

static int get_num_heaps(void) {
  static int num_heaps = -1;

  if (num_heaps < 0) {
    //
    // Initialize: without multiple NUMA domains we definitely don't
    // need multiple heaps.  Even with multiple NUMA domains we might
    // not need multiple heaps, if other constraints aren't satisfied.
    //
    void* heap_base;
    size_t heap_size;

    chpl_comm_desired_shared_heap(&heap_base, &heap_size);
    if (heap_base == NULL) {
      num_heaps = 0;
    } else {
      num_heaps = chpl_topo_getNumNumaDomains();
      if (num_heaps < 1) {
        num_heaps = 1;
      } else if (!chpl_mem_impl_alloc_localizes()) {
        num_heaps = 1;
      }
    }
  }

  return num_heaps;
}

static inline int get_nearby_heap(void) {
  if (get_num_heaps() == 1) {
    return 0;
  } else {
    int hpi = chpl_topo_getThreadLocality();
    if (!isActualSublocID(hpi) || hpi >= get_num_heaps()) {
      chpl_internal_error("unexpected heap index");
    }
    return hpi;
  }
}


// helper routine to initialize the table of heaps
static void setupLocalizedHeaps(void* heap_base, size_t heap_size) {
  const int num_heaps = get_num_heaps();
  int hpi;
  char* subchunk_base;
  size_t* subchunk_sizes;

  heaps = (shared_heap_t*) chpl_je_malloc(num_heaps * sizeof(*heaps));
  if (heaps == NULL) {
    chpl_internal_error("cannot allocate heaps");
  }

  if (num_heaps <= 1) {
    heaps[0].base = heap_base;
    heaps[0].size = heap_size;
    heaps[0].cur_offset = 0;
    if (pthread_mutex_init(&heaps[0].alloc_lock, NULL) != 0) {
      chpl_internal_error("cannot init chunk_alloc lock");
    }

    return;
  }

  subchunk_sizes =
    (size_t*) chpl_je_malloc(num_heaps * sizeof(*subchunk_sizes));
  if (subchunk_sizes == NULL) {
    chpl_internal_error("cannot allocate subchunk_sizes");
  }

  chpl_topo_setMemSubchunkLocality(heap_base, heap_size, true,
                                   subchunk_sizes);

  subchunk_base = (char*) heap_base;
  for (hpi = 0; hpi < num_heaps; hpi++) {
    heaps[hpi].base = subchunk_base;
    heaps[hpi].size = subchunk_sizes[hpi];
    heaps[hpi].cur_offset = 0;
    if (pthread_mutex_init(&heaps[hpi].alloc_lock, NULL) != 0) {
      chpl_internal_error("cannot init chunk_alloc lock");
    }
    subchunk_base += heaps[hpi].size;

    //
    // The chpl_topo_setMemSubchunkLocality() call above isn't
    // having the effect we desire, for reasons that are mostly but
    // not completely understood.  So for now, to get the locality
    // we want, touch the pages into existence while executing on
    // the desired NUMA domains.
    //
    chpl_topo_touchMemFromSubloc(heaps[hpi].base, heaps[hpi].size, true, hpi);
  }
}


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

  int hpi;
  void* cur_chunk_base = NULL;
  size_t cur_heap_size;

  // which heap?
  hpi = get_nearby_heap();

  // this function can be called concurrently and it looks like jemalloc
  // doesn't call it inside a lock, so we need to protect it ourselves
  pthread_mutex_lock(&heaps[hpi].alloc_lock);

  // compute our current aligned pointer into the shared heap
  //
  //   jemalloc 4.5.0 man: "The alignment parameter is always a power of two at
  //   least as large as the chunk size."
  cur_chunk_base = alignHelper(heaps[hpi].base, heaps[hpi].cur_offset,
                               alignment);

  // jemalloc 4.5.0 man: "If chunk is not NULL, the returned pointer must be
  // chunk on success or NULL on error"
  if (chunk && chunk != cur_chunk_base) {
    pthread_mutex_unlock(&heaps[hpi].alloc_lock);
    return NULL;
  }

  cur_heap_size = (uintptr_t)cur_chunk_base - (uintptr_t)heaps[hpi].base;

  // If there's not enough space on the heap for this allocation, return NULL
  if (size > heaps[hpi].size - cur_heap_size) {
    pthread_mutex_unlock(&heaps[hpi].alloc_lock);
    return NULL;
  }

  // Update the current pointer, now that we've past any early returns.
  heaps[hpi].cur_offset = cur_heap_size + size;

  // now that cur_heap_offset is updated, we can unlock
  pthread_mutex_unlock(&heaps[hpi].alloc_lock);

  // jemalloc 4.5.0 man: "Zeroing is mandatory if *zero is true upon entry."
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


// helper routine to get a mallctl value
#define DECLARE_GET_MALLCTL_VALUE(type) \
static type get_ ## type ##_mallctl_value(const char* mallctl_string) { \
  type value; \
  size_t sz; \
  sz = sizeof(value); \
  if (chpl_je_mallctl(mallctl_string, &value, &sz, NULL, 0) != 0) { \
    char error_msg[256]; \
    snprintf(error_msg, sizeof(error_msg), "could not get mallctl value for %s", mallctl_string); \
    chpl_internal_error(error_msg); \
  } \
  return value; \
}
DECLARE_GET_MALLCTL_VALUE(size_t);
DECLARE_GET_MALLCTL_VALUE(unsigned);
#undef DECLARE_GET_MALLCTL_VALUE


// get the number of arenas
static unsigned get_num_arenas(void) {
  return get_unsigned_mallctl_value("opt.narenas");
}

// initialize our arenas (this is required to be able to set the chunk hooks)
static void initialize_arenas(void) {
  unsigned narenas;
  unsigned arena;

  // for each non-zero arena, set the current thread to use it (this
  // initializes each arena). arena 0 is automatically initialized.
  //
  //   jemalloc 4.5.0 man: "If the specified arena was not initialized
  //   beforehand, it will be automatically initialized as a side effect of
  //   calling this interface."
  narenas = get_num_arenas();
  for (arena=1; arena<narenas; arena++) {
    if (chpl_je_mallctl("thread.arena", NULL, NULL, &arena, sizeof(arena)) != 0) {
      chpl_internal_error("could not change current thread's arena");
    }
  }

  // then set the current thread back to using arena 0
  arena = 0;
  if (chpl_je_mallctl("thread.arena", NULL, NULL, &arena, sizeof(arena)) != 0) {
      chpl_internal_error("could not change current thread's arena back to 0");
  }
}


// replace the chunk hooks for each arena with the hooks we provided above
static void replaceChunkHooks(void) {
  unsigned narenas;
  unsigned arena;

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
    snprintf(path, sizeof(path), "arena.%u.chunk_hooks", arena);
    if (chpl_je_mallctl(path, NULL, NULL, &new_hooks, sizeof(chunk_hooks_t)) != 0) {
      chpl_internal_error("could not update the chunk hooks");
    }
  }
}

// helper routines to get the number of size classes
static unsigned get_num_small_classes(void) {
  return get_unsigned_mallctl_value("arenas.nbins");
}

static unsigned get_num_large_classes(void) {
  return get_unsigned_mallctl_value("arenas.nlruns");
}

static unsigned get_num_small_and_large_classes(void) {
  return get_num_small_classes() + get_num_large_classes();
}

// helper routine to get an array of size classes for small and large sizes
static void get_small_and_large_class_sizes(size_t* classes) {
  unsigned class;
  unsigned small_classes;
  unsigned large_classes;

  small_classes = get_num_small_classes();
  for (class=0; class<small_classes; class++) {
    char ssize[128];
    snprintf(ssize, sizeof(ssize), "arenas.bin.%u.size", class);
    classes[class] = get_size_t_mallctl_value(ssize);
  }

  large_classes = get_num_large_classes();
  for (class=0; class<large_classes; class++) {
    char lsize[128];
    snprintf(lsize, sizeof(lsize), "arenas.lrun.%u.size", class);
    classes[small_classes + class] = get_size_t_mallctl_value(lsize);
  }
}

// helper routine to determine if an address is not part of any shared heap
static bool addressNotInAnyHeap(void* ptr) {
  const int num_heaps = get_num_heaps();
  uintptr_t u_ptr = (uintptr_t)ptr;
  int hpi;
  for (hpi = 0; hpi < num_heaps; hpi++) {
    uintptr_t u_base = (uintptr_t)heaps[hpi].base;
    uintptr_t u_top =  u_base + heaps[hpi].size;
    if (u_ptr >= u_base && u_ptr < u_top) {
      return false;
    }
  }
  return true;
}

// grab (and leak) whatever memory jemalloc got on it's own, that's not in
// our shared heap
//
//   jemalloc 4.5.0 man: "arenas may have already created chunks prior to the
//   application having an opportunity to take over chunk allocation."
//
// jemalloc grabs "chunks" from the system in order to store metadata and some
// internal data structures. However, these chunks are not allocated from our
// shared heap, so we need to waste whatever memory is left in them so that
// future allocations come from chunks that were provided by our shared heap
static void useUpMemNotInHeap(void) {
  unsigned class;
  unsigned num_classes = get_num_small_and_large_classes();
  size_t classes[num_classes];
  get_small_and_large_class_sizes(classes);

  // jemalloc has 3 class sizes. The small (a few pages) and large (up to chunk
  // size) objects come from the arenas, but huge (more than chunk size)
  // objects comes from a shared pool. We waste memory at every large and small
  // class size so that we can be sure there's no fragments left in a chunk
  // that jemalloc grabbed from the system. This way we know that any future
  // allocation, regardless of size, must have come from a chunk provided by
  // our shared heap. Once we know a specific class size came from our shared
  // heap, we can free the memory instead of leaking it.
  for (class=num_classes-1; class!=UINT_MAX; class--) {
    void* p = NULL;
    size_t alloc_size;
    alloc_size = classes[class];
    do {
      if ((p = chpl_je_malloc(alloc_size)) == NULL) {
        chpl_internal_error("could not use up memory outside of shared heap");
      }
    } while (addressNotInAnyHeap(p));
    chpl_je_free(p);
  }
}

// Have jemalloc use our shared heap. Localize the shared heap, initialize all
// the arenas, then replace the chunk hooks with our custom ones, and finally
// use up any memory jemalloc got from the system that's not in our shared
// heap.
static void initializeSharedHeap(void* heap_base, size_t heap_size) {
  setupLocalizedHeaps(heap_base, heap_size);

  initialize_arenas();

  replaceChunkHooks();

  useUpMemNotInHeap();
}


void chpl_mem_layerInit(void) {
  void* heap_base;
  size_t heap_size;

  chpl_comm_desired_shared_heap(&heap_base, &heap_size);
  if (heap_base != NULL && heap_size == 0) {
    chpl_internal_error("if heap address is specified, size must be also");
  }

  // If we have a shared heap, initialize our shared heap. This will take care
  // of initializing jemalloc. If we're not using a shared heap, do a first
  // allocation to allow jemalloc to set up:
  //
  //   jemalloc 4.5.0 man: "Once, when the first call is made to one of the
  //   memory allocation routines, the allocator initializes its internals"
  if (heap_base != NULL) {
    initializeSharedHeap(heap_base, heap_size);
  } else {
    void* p;
    if ((p = chpl_je_malloc(1)) == NULL) {
      chpl_internal_error("cannot init heap: chpl_je_malloc() failed");
    }
    chpl_je_free(p);
  }
}


void chpl_mem_layerExit(void) {
  const int num_heaps = get_num_heaps();
  int hpi;

  for (hpi = 0; hpi < num_heaps; hpi++) {
    if (heaps[hpi].base != NULL) {
      // ignore errors, we're exiting anyways
      (void) pthread_mutex_destroy(&heaps[hpi].alloc_lock);
    }
  }
}


chpl_bool chpl_mem_impl_alloc_localizes(void) {
  //
  // For now, we only NUMA-localize the comm layer desired shared heap
  // if we're using the ugni comm layer.  ugni is the simpler case
  // because it's only used on Cray X* compute blades which are a
  // real-memory architecture.  We'll tackle comm=gasnet later; that's
  // more complicated because GASNet is used on all sorts of systems,
  // some virtual-memory, and we can't just go touching all the pages.
  //
  return (get_num_heaps() > 1
          && strcmp(CHPL_LOCALE_MODEL, "flat") != 0
          && strcmp(CHPL_COMM, "ugni") == 0);
}
