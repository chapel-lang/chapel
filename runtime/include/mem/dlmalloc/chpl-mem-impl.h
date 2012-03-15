/* dlmalloc memory function implementation */

#include "dlmalloc.h"

extern mspace chpl_dlmalloc_heap;

static ___always_inline void* chpl_calloc(size_t n, size_t size) {
  return mspace_calloc(chpl_dlmalloc_heap,n,size);
}

static ___always_inline void* chpl_malloc(size_t size) {
  return mspace_malloc(chpl_dlmalloc_heap, size);
}

static ___always_inline void* chpl_realloc(void* ptr, size_t size) {
  return mspace_realloc(chpl_dlmalloc_heap, ptr, size);
}

static ___always_inline void chpl_free(void* ptr) {
  mspace_free(chpl_dlmalloc_heap, ptr);
}

