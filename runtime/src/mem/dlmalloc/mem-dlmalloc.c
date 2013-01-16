#include "chplrt.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpltypes.h"
#include "error.h"
#include "dlmalloc.h"

mspace chpl_dlmalloc_heap;

static void* saved_heap_start;
static size_t saved_heap_size;


void chpl_mem_layerInit(void) {
  chpl_comm_desired_shared_heap(&saved_heap_start, &saved_heap_size);
  if (!saved_heap_start || !saved_heap_size) {
    chpl_dlmalloc_heap = create_mspace(0, 1);
  } else {
    chpl_dlmalloc_heap = create_mspace_with_base(saved_heap_start, saved_heap_size, 1);
  }
}


void chpl_mem_layerExit(void) { }


void chpl_mem_layerActualSharedHeap(void** start_p, size_t* size_p) {
  *start_p = saved_heap_start;
  *size_p  = saved_heap_size;
}


