#ifndef QT_ALLOC_H
#define QT_ALLOC_H

#include <stdint.h>
#include <stdlib.h>

#include "qt_visibility.h"

void INTERNAL *qt_malloc(size_t size);

void INTERNAL qt_free(void *ptr);

#define MALLOC(sz) qt_malloc(sz)
#define FREE(ptr, sz) qt_free(ptr)

void INTERNAL *qt_calloc(size_t nmemb, size_t size);

void INTERNAL *qt_realloc(void *ptr, size_t size);

void INTERNAL *qt_internal_aligned_alloc(size_t alloc_size,
                                         uint_fast16_t alignment);
void INTERNAL qt_internal_aligned_free(void *ptr, uint_fast16_t alignment);

void INTERNAL qt_internal_alignment_init(void);

#ifdef __INTEL_COMPILER
#pragma warning(disable : 191)
#endif
extern size_t _pagesize;
#define pagesize ((size_t)_pagesize)
#endif // ifndef QT_ALLOC_H
/* vim:set expandtab: */
