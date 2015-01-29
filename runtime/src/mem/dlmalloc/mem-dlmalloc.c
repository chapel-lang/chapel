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


void chpl_mem_layerInit(void) {
  void*  heap_base;
  size_t heap_size;

  chpl_comm_desired_shared_heap(&heap_base, &heap_size);
  if (heap_base == NULL || heap_size == 0)
    chpl_dlmalloc_heap = create_mspace(0, 1);
  else
    chpl_dlmalloc_heap = create_mspace_with_base(heap_base, heap_size, 1);
}


void chpl_mem_layerExit(void) { }


