#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
/* System Headers */
#if (HAVE_MEMALIGN && HAVE_MALLOC_H)
# include <malloc.h> /* for memalign() */
#endif
#ifdef HAVE_GETPAGESIZE
# include <unistd.h>
#else
static QINLINE int getpagesize()
{
  return 4096;
}
#endif

#include <qthread/qthread-int.h> /* for uintptr_t */

/* Internal Headers */
#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_debug.h"

/* local constants */
size_t _pagesize = 0;

void *qt_malloc(size_t size){
  return malloc(size);
}

void qt_free(void *ptr){
  free(ptr);
}

void *qt_calloc(size_t nmemb, size_t size) {
  return calloc(nmemb, size);
}

void *qt_realloc(void *ptr, size_t size) {
  return realloc(ptr, size);
}

void qt_internal_alignment_init(void)
{
    _pagesize = getpagesize();
}

void *qt_internal_aligned_alloc(size_t        alloc_size,
                                     uint_fast16_t alignment)
{
    void *ret;

    assert(alloc_size > 0);
    switch (alignment) {
        case 0:
            ret = MALLOC(alloc_size);
            break;
#if defined(HAVE_16ALIGNED_MALLOC)
        case 16:
        case 8:
        case 4:
        case 2:
            ret = MALLOC(alloc_size);
            break;
#endif
        default:
#if defined(HAVE_WORKING_VALLOC)
            if (alignment == pagesize) {
                ret = valloc(alloc_size);
                break;
            }
#elif defined(HAVE_PAGE_ALIGNED_MALLOC)
            if (alignment == pagesize) {
                ret = MALLOC(alloc_size);
                break;
            }
#endif
#if defined(HAVE_MEMALIGN)
            ret = memalign(alignment, alloc_size);
#elif defined(HAVE_POSIX_MEMALIGN)
            posix_memalign(&(ret), alignment, alloc_size);
#else
            {
                uint8_t *tmp = MALLOC((alloc_size + alignment - 1) + sizeof(void *));
                if (!tmp) { return NULL; }
                ret                 = (void *)(((uintptr_t)(tmp + sizeof(void *) + alignment - 1)) & ~(alignment - 1));
                *((void **)ret - 1) = tmp;
            }
            break;
#endif  /* if defined(HAVE_MEMALIGN) */
    }
    assert(ret);
    assert(((uintptr_t)ret & (alignment - 1)) == 0);
    return ret;
}

void qt_internal_aligned_free(void         *ptr,
                                   uint_fast16_t alignment)
{
    assert(ptr);
    switch (alignment) {
        case 0:
            qt_free(ptr);
            break;
#if defined(HAVE_16ALIGNED_MALLOC)
        case 16:
        case 8:
        case 4:
        case 2:
            qt_free(ptr);
            break;
#endif
        default:
#if defined(HAVE_WORKING_VALLOC) || defined(HAVE_PAGE_ALIGNED_MALLOC)
            if (alignment == pagesize) {
                qt_free(ptr);
                break;
            }
#endif
#if defined(HAVE_MEMALIGN) || defined(HAVE_POSIX_MEMALIGN)
            qt_free(ptr);
#else
            assert((uintptr_t)*((void **)ptr - 1) > 4096);
            qt_free(*((void **)ptr - 1));
#endif
    }
}

/* vim:set expandtab: */
