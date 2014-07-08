#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h> /* for malloc() */
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
#include "qt_visibility.h"
#include "qt_aligned_alloc.h"
#include "qt_asserts.h"
#include "qt_debug.h"

/* local constants */
size_t _pagesize = 0;

void INTERNAL qthread_internal_alignment_init(void)
{
    _pagesize = getpagesize();
}

void INTERNAL *qthread_internal_aligned_alloc(size_t        alloc_size,
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

void INTERNAL qthread_internal_aligned_free(void         *ptr,
                                            uint_fast16_t alignment)
{
    assert(ptr);
    switch (alignment) {
        case 0:
            free(ptr);
            break;
#if defined(HAVE_16ALIGNED_MALLOC)
        case 16:
        case 8:
        case 4:
        case 2:
            free(ptr);
            break;
#endif
        default:
#if defined(HAVE_WORKING_VALLOC) || defined(HAVE_PAGE_ALIGNED_MALLOC)
            if (alignment == pagesize) {
                free(ptr);
                break;
            }
#endif
#if defined(HAVE_MEMALIGN) || defined(HAVE_POSIX_MEMALIGN)
            free(ptr);
#else
            assert((uintptr_t)*((void **)ptr - 1) > 4096);
            free(*((void **)ptr - 1));
#endif
    }
}

/* vim:set expandtab: */
