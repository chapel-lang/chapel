/* Boehm memory function implementation */

#include <gc.h>

static ___always_inline void* chpl_calloc(size_t n, size_t size) {
  return GC_MALLOC(n*size);
}

static ___always_inline void* chpl_malloc(size_t size) {
  return GC_MALLOC(size);
}

static ___always_inline void* chpl_realloc(void* ptr, size_t size) {
  return GC_REALLOC(ptr, size);
}

static ___always_inline void chpl_free(void* ptr) {
  GC_FREE(ptr);
}

