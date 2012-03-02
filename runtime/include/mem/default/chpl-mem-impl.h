/* default memory function implementation */

#include <stdlib.h>

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef _chpl_mem_warning_macros_h_

static ___always_inline void* chpl_calloc(size_t n, size_t size) {
  if( n == 0 || size == 0 ) return NULL;
  return calloc(n,size);
}

static ___always_inline void* chpl_malloc(size_t size) {
  return (size) ? malloc(size) : NULL;
}

static ___always_inline void* chpl_realloc(void* ptr, size_t size) {
  return realloc(ptr,size);
}

static ___always_inline void chpl_free(void* ptr) {
  free(ptr);
}

// Now that we've defined our functions, turn the warnings back on.
#include "chpl-mem-warning-macros.h"

