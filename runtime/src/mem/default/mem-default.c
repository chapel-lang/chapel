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

void chpl_mem_layerInit(void) {
  void* start;
  size_t size;

  chpl_comm_desired_shared_heap(&start, &size);
  if (start || size)
    chpl_error("set CHPL_MEM to a more appropriate mem type", 0, 0);
}


void chpl_mem_layerExit(void) { }


void chpl_mem_layerActualSharedHeap(void** start_p, size_t* size_p) {
  *start_p = NULL;
  *size_p  = 0;
}


