
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#ifdef HAVE_GETPAGESIZE
# include <unistd.h>
#else
static QINLINE int getpagesize()
{
  return 4096;
}
#endif

#include <qthread/qthread-int.h> /* for uint_fast16_t */

#include "chpl-mem-impl.h"

void *qt_malloc(size_t size){
  return chpl_malloc(size);
}

void qt_free(void *ptr){
  chpl_free(ptr);
}

void *qt_calloc(size_t nmemb, size_t size) {
  return chpl_calloc(nmemb, size);
}

void *qt_realloc(void *ptr, size_t size) {
  return chpl_realloc(ptr, size);
}

/* local constants */
size_t _pagesize = 0;

void qt_internal_alignment_init(void) {
  _pagesize = getpagesize();
}

void *qt_internal_aligned_alloc(size_t        alloc_size,
                                     uint_fast16_t alignment) {
    return chpl_memalign(alignment, alloc_size);
}

void qt_internal_aligned_free(void         *ptr,
                                   uint_fast16_t alignment) {
    chpl_free(ptr);
}
