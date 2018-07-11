
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

#include "chpl-mem.h"
#include "chpl-linefile-support.h"

void *qt_malloc(size_t size){
  return chpl_mem_alloc(size, CHPL_RT_MD_TASK_LAYER_UNSPEC,
                        0, CHPL_FILE_IDX_INTERNAL);
}

void qt_free(void *ptr){
  chpl_mem_free(ptr, 0, CHPL_FILE_IDX_INTERNAL);
}

void *qt_calloc(size_t nmemb, size_t size) {
  return chpl_mem_calloc(nmemb, size, CHPL_RT_MD_TASK_LAYER_UNSPEC,
                         0, CHPL_FILE_IDX_INTERNAL);
}

void *qt_realloc(void *ptr, size_t size) {
  return chpl_mem_realloc(ptr, size, CHPL_RT_MD_TASK_LAYER_UNSPEC,
                          0, CHPL_FILE_IDX_INTERNAL);
}

/* local constants */
size_t _pagesize = 0;

void qt_internal_alignment_init(void) {
  _pagesize = getpagesize();
}

void *qt_internal_aligned_alloc(size_t        alloc_size,
                                     uint_fast16_t alignment) {
  return chpl_mem_memalign(alignment, alloc_size, CHPL_RT_MD_TASK_LAYER_UNSPEC,
                           0, CHPL_FILE_IDX_INTERNAL);
}

void qt_internal_aligned_free(void         *ptr,
                                   uint_fast16_t alignment) {
  chpl_mem_free(ptr, 0, CHPL_FILE_IDX_INTERNAL);
}
