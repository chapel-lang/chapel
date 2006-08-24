#ifndef _chplalloc_H_
#define _chplalloc_H_

#ifdef LEAK_DETECT
#define GC_DEBUG
#include "gc_cpp.h"
#define MALLOC(n) GC_MALLOC(n)
#define CALLOC(m,n) GC_MALLOC((m)*(n))
#define FREE(p) GC_FREE(p)
#define REALLOC(p,n) GC_REALLOC((p),(n))
#define CHECK_LEAKS() GC_gcollect()
#define MEM_INIT() GC_INIT()
#else
#ifdef USE_GC
#include "gc_cpp.h"
#define MALLOC GC_MALLOC
#define REALLOC GC_REALLOC
#define FREE(_x)
#define malloc dont_use_malloc_use_MALLOC_instead
#define relloc dont_use_realloc_use_REALLOC_instead
#define free dont_use_free_use_FREE_instead
#define MEM_INIT() GC_INIT()
#else
#include <stdlib.h>
class gc {
 public:
  bool unused;
};
#define MALLOC malloc
#define REALLOC realloc
#define FREE free
#define MEM_INIT()
#define GC_gcollect()
#endif
#endif



#endif
