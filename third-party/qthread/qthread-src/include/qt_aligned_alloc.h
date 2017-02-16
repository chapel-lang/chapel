#ifndef HAVE_QT_ALIGNED_ALLOC_H

#include "qt_visibility.h"

void INTERNAL *qthread_internal_aligned_alloc(size_t        alloc_size,
                                              uint_fast16_t alignment);
void INTERNAL qthread_internal_aligned_free(void         *ptr,
                                            uint_fast16_t alignment);

void INTERNAL qthread_internal_alignment_init(void);

#ifdef __INTEL_COMPILER
# pragma warning (disable:191)
#endif
extern size_t _pagesize;
#define pagesize ((const size_t)_pagesize)
#endif // ifndef HAVE_QT_ALIGNED_ALLOC_H
/* vim:set expandtab: */
