#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"
#include "tcmalloc.h"
#include "tcmalloc-interface.h"


static void*  saved_heap_base = NULL;
static size_t saved_heap_size = 0;


void chpl_mem_layerInit(void)
{
  void* addr;
  size_t size;

  chpl_comm_desired_shared_heap(&addr, &size);

  if (addr != NULL && size == 0)
    chpl_internal_error("if heap address is specified, size must be also");

  //
  // Do a first allocation, to allow tcmalloc to set up its internal
  // management structures.
  //
  {
    void* p;

    if ((p = tc_malloc(1)) == NULL)
      chpl_internal_error("cannot init heap: tc_malloc() failed");
    tc_free(p);
  }

  saved_heap_base = addr;
  saved_heap_size = size;

  //
  // Initialize our tcmalloc system allocator.
  //
  tcmallocChapelInit_c(addr, size);

  //
  // If the heap has to come from the memory supplied to us (say, in
  // order that the comm layer be able to depend on all allocations
  // having come from it), use up all the system memory tcmalloc had
  // acquired before we set up our own system allocator just now.
  // All allocations after this will come from the memory supplied
  // to us.
  //
  // Note that this can waste up to twice INITIAL_USE_UP_SIZE bytes
  // of the memory supplied to us, plus overhead.
  //
  if (addr != NULL) {
#define INITIAL_USE_UP_SIZE ((size_t) 4 * 1024)

    size_t size;
    char*  p;

    for (size = INITIAL_USE_UP_SIZE; size > 0; size /= 2) {
      do {
        p = tc_malloc(size);
      } while (p != NULL
               && (p < (char*) saved_heap_base
                   || p > (char*) saved_heap_base + saved_heap_size));

#undef INITIAL_USE_UP_SIZE
    }
  }
}


void chpl_mem_layerExit(void) { }


void chpl_mem_layerActualSharedHeap(void** addr_p, size_t* size_p)
{
  *addr_p = saved_heap_base;
  *size_p = saved_heap_size;
}


