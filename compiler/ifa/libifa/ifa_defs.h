/* -*-Mode: c++;-*-
  Copyright 2003-6 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _defs_H_
#define _defs_H_

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

#ifdef IFA_EXTERN
#define IFA_EXTERN_INIT(_x) = _x
#else
#define IFA_EXTERN_INIT(_x)
#define IFA_EXTERN extern
#endif

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
#define MALLOC malloc
#define REALLOC realloc
#define FREE free
class gc {
 public:
  bool unused;
};
#define MEM_INIT()
#endif
#endif

#define round2(_x,_n) ((_x + ((_n)-1)) & ~((_n)-1))
#define tohex1(_x) \
((((_x)&15) > 9) ? (((_x)&15) - 10 + 'A') : (((_x)&15) + '0'))
#define tohex2(_x) \
((((_x)>>4) > 9) ? (((_x)>>4) - 10 + 'A') : (((_x)>>4) + '0'))
#define numberof(_x) ((sizeof(_x))/(sizeof((_x)[0])))

typedef char int8;
typedef unsigned char uint8;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef short int16;
typedef unsigned short uint16;
/* typedef uint32 uint; * already part of most systems */
typedef float float32;
typedef double float64;
typedef struct { float32 r; float32 i; } complex32;
typedef struct { float64 r; float64 i; } complex64;

#define NUM_ELEMENTS(_x) (sizeof(_x)/sizeof((_x)[0]))

extern int ifa_verbose;
extern int ifa_debug;

#define dbg if (ifa_debug) printf
#define DBG(_x) do { if (ifa_debug) { _x; } } while (0);

#include "list.h"
#include "vec.h"
#include "map.h"
#include "ast.h"
#include "arg.h"
#include "if1.h"
#include "prim.h"
#include "builtin.h"
#include "fail.h"
#include "fa.h"
#include "var.h"
#include "pnode.h"
#include "region.h"
#include "fun.h"
#include "pdb.h"
#include "clone.h"
#include "cg.h"

#ifdef __CYGWIN__
// cygwin assert busted 
#undef assert
void myassert();
#define assert(_x) if (!(_x)) myassert()
#endif

#endif
