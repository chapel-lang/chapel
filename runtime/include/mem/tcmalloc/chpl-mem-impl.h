/* tcmalloc memory function implementation */

#include "tcmalloc.h"

static inline void* chpl_calloc(size_t n, size_t size) {
  return tc_calloc(n,size);
}

static inline void* chpl_malloc(size_t size) {
  return tc_malloc(size);
}

static inline void* chpl_realloc(void* ptr, size_t size) {
  return tc_realloc(ptr, size);
}

static inline void chpl_free(void* ptr) {
  tc_free(ptr);
}

