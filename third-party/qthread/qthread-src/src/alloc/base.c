#include <stdint.h>
#include <stdlib.h>

/* System Headers */
#include <unistd.h> /* for getpagesize() */

/* Internal Headers */
#include "qt_alloc.h"
#include "qt_asserts.h"

/* local constants */
size_t _pagesize = 0;

void *qt_malloc(size_t size) { return malloc(size); }

void qt_free(void *ptr) { free(ptr); }

void *qt_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

void *qt_realloc(void *ptr, size_t size) { return realloc(ptr, size); }

void qt_internal_alignment_init(void) { _pagesize = getpagesize(); }

void *qt_internal_aligned_alloc(size_t alloc_size,
                                uint_fast16_t alignment_small) {
  size_t alignment = alignment_small;
  // round alloc_size up to the nearest multiple of alignment
  // since that's required by the standard aligned_alloc
  // and the implementation on OSX actually enforces that.
  if (alignment) {
    alloc_size = ((alloc_size + (alignment - 1ull)) / alignment) * alignment;
  }
  return aligned_alloc(alignment, alloc_size);
}

void qt_internal_aligned_free(void *ptr, uint_fast16_t alignment) {
  qt_free(ptr);
}

/* vim:set expandtab: */
