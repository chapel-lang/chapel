#include <stdio.h>
#include <stdlib.h>
#include "chplmem.h"
#include "chplrt.h"

#undef malloc
#undef calloc
#undef free
#undef realloc

static void confirm(void* memory, char* description) {
  if (!memory) {
    fprintf(stderr, "***Error:  Out of memory allocating %s***\n", description);
    exit(0);
  }
}


void* _chpl_malloc(size_t number, size_t size, char* description) {
  size_t chunk = number * size;
  void* memory = malloc(chunk);
  confirm(memory, description);
  return memory;
}


/*
static void memzero(void* memory, size_t chunk) {
  size_t i;
  for (i = 0; i < chunk; i++) {
    *(char*)(memory + i) = 0;
  }
}
*/


void* _chpl_calloc(size_t number, size_t size, char* description) {
  void* memory = calloc(number, size);
  confirm(memory, description);
  return memory;
}


void _chpl_free(void* memory) {
  free(memory);
}


void* _chpl_realloc(void* memory, size_t number, size_t size) {
  size_t chunk = number * size;
  void* moreMemory = realloc(memory, chunk);
  confirm(moreMemory, "");
  return moreMemory;
}
