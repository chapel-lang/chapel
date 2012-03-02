/* tcmalloc memory function implementation */

#include "tcmalloc.h"

static ___always_inline void* chpl_calloc(size_t n, size_t size) {
  return tc_calloc(n,size);
}

static ___always_inline void* chpl_malloc(size_t size) {
  return tc_malloc(size);
}

static ___always_inline void* chpl_realloc(void* ptr, size_t size) {
  return tc_realloc(ptr, size);
}

static ___always_inline void chpl_free(void* ptr) {
  tc_free(ptr);
}

