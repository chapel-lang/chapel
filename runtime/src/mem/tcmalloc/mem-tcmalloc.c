#include <errno.h>
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
  void* start;
  size_t size;

  chpl_comm_desired_shared_heap(&start, &size);

  if (start)
    chpl_warning("CHPL_MEM=tcmalloc ignores a starting heap address", 0, 0);

  if (!size) {
    char* p;

    if ((p = getenv("TCMALLOC_PREALLOCATED_HEAP")) != NULL) {
      char units;

      if (sscanf(p, "%zi%c", &size, &units) != 2
          || strchr("kKmMgG", units) == NULL) {
        printf("Cannot parse TCMALLOC_PREALLOCATED_HEAP environment variable; "
               "assume 4m\n");
        size = 4 * 1024 * 1024;
      }
      else {
        switch (units) {
        case 'k' : case 'K': size *= 1024;               break;
        case 'm' : case 'M': size *= 1024 * 1024;        break;
        case 'g' : case 'G': size *= 1024 * 1024 * 1024; break;
        }
      }
    }
  }

  if (!size)
    return;

  //
  // Do a first allocation, to allow tcmalloc to set up its internal
  // management structures.
  //
  {
    void* p;

    if ((p = tc_malloc(1)) == NULL)
      chpl_error("cannot init heap: tc_malloc() failed", 0, 0);
    tc_free(p);
  }

  //
  // Initialize our tcmalloc hugepage allocator.
  //
  saved_heap_base = tcmallocChapelInit_c(size, &saved_heap_size);
  if (saved_heap_base == NULL)
    chpl_error("cannot init heap: ChapelHugepageSysAllocatorInit_c() failed",
               0, 0);
}


void chpl_mem_layerExit(void) { }


void chpl_mem_layerActualSharedHeap(void** start_p, size_t* size_p)
{
  *start_p = saved_heap_base;
  *size_p  = saved_heap_size;
}


void* chpl_mem_layerAlloc(size_t size,
                          int32_t lineno, chpl_string filename)
{
  if (!heapInitialized)
    chpl_error("chpl_mem_allocMany called before the heap is initialized",
               lineno, filename);
  return tc_malloc(size);
}


void* chpl_mem_layerRealloc(void* memAlloc, size_t newChunk,
                            int32_t lineno, chpl_string filename)
{
  if (!heapInitialized)
    chpl_error("chpl_mem_realloc called before the heap is initialized",
               lineno, filename);
  return tc_realloc(memAlloc, newChunk);
}


void chpl_mem_layerFree(void* memAlloc,
                        int32_t lineno, chpl_string filename)
{
  if (!heapInitialized)
    chpl_error("chpl_mem_free called before the heap is initialized",
               lineno, filename);
  tc_free(memAlloc);
}
