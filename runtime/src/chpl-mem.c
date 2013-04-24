//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"


static int heapInitialized = 0;


void chpl_mem_init(void) {
  chpl_mem_layerInit();
  heapInitialized = 1;
}


void chpl_mem_exit(void) {
  chpl_mem_layerExit();
}


int chpl_mem_inited(void) {
  return heapInitialized;
}


void chpl_mem_actualSharedHeap(void** start_p, size_t* size_p) {
  chpl_mem_layerActualSharedHeap(start_p, size_p);
}

