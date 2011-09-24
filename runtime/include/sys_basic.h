#ifndef _SYS_BASIC_H
#define _SYS_BASIC_H

#ifndef _BSD_SOURCE
// get endian.h htobe16, etc
#define _BSD_SOURCE
#endif

// get posix_memalign
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE 200112L
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#ifdef __GNUC__
#define always_inline inline __attribute__((__always_inline__))
#else
#define always_inline inline
#endif

#define GASNETT_THREAD_SAFE 1
#define GASNETI_THREAD_SAFE 1

// If gasnet.h and gasnet_tools.h are both included,
// we're supposed to include gasnet.h first.
#ifdef PLEASE_INCLUDE_GASNET_H
#include "gasnet.h"
#endif


#ifdef _chplrt_H_
#ifndef PLEASE_INCLUDE_GASNET_TOOLS_H
#define PLEASE_INCLUDE_GASNET_TOOLS_H 1
#endif
#endif

#ifdef CHPL_GEN_CODE
#ifndef PLEASE_INCLUDE_GASNET_TOOLS_H
#define PLEASE_INCLUDE_GASNET_TOOLS_H 1
#endif
#endif

#ifdef CHPL_RUNTIME
#ifndef PLEASE_INCLUDE_GASNET_TOOLS_H
#define PLEASE_INCLUDE_GASNET_TOOLS_H 1
#endif
#endif


#ifdef PLEASE_INCLUDE_GASNET_TOOLS_H
// GASNet manuals say they want this to be the 1st thing included.
// If we're not compiling chapel codes, we'll fall back on
// e.g. __sync_fetch_add
#include "gasnet_tools.h"
#endif

#ifndef _GASNET_TOOLS_H
#include <limits.h> // for LONG_MAX.
// We make some wrappers using GCC intrinsics for easy C test cases.
typedef long gasnett_atomic_val_t;
typedef long gasnett_atomic_t;
#define GASNETT_ATOMIC_MAX LONG_MAX
#define GASNETT_ATOMIC_MB_PRE 0
#define GASNETT_ATOMIC_MB_POST 0
static always_inline
void gasnett_atomic_set(gasnett_atomic_t *p,
                        gasnett_atomic_val_t v,
                        int flags) {
  __sync_synchronize();
  *p = v;
  __sync_synchronize();
}
static always_inline
gasnett_atomic_val_t gasnett_atomic_read(gasnett_atomic_t *p, int flags) {
  gasnett_atomic_val_t ret;
  __sync_synchronize();
  ret = *p;
  __sync_synchronize();
  return ret;
}
static always_inline
gasnett_atomic_val_t gasnett_atomic_add(gasnett_atomic_t *p,
                                        gasnett_atomic_val_t op,
                                        int flags) {
  return __sync_add_and_fetch(p, op);
}

static always_inline
gasnett_atomic_val_t gasnett_atomic_subtract(gasnett_atomic_t *p,
                                             gasnett_atomic_val_t op,
                                             int flags) {
  return __sync_sub_and_fetch(p, op);
}
// Workaround gasnet tools.
#endif

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef int c_int;
typedef unsigned int c_uint;
typedef long c_long;
typedef unsigned long c_ulong;
typedef long long c_longlong;
typedef unsigned long long c_ulonglong;
typedef double c_double;
typedef void* c_ptr;
typedef int err_t;
typedef int fd_t;
typedef wchar_t c_wchar_t;
typedef char c_char_t;
typedef FILE* c_file;

static inline c_file chpl_cstdin(void) { return stdin; }
static inline c_file chpl_cstdout(void) { return stdout; }
static inline c_file chpl_cstderr(void) { return stderr; }

#define c_nil NULL;

enum { EXTEND_ERROR_OFFSET = 1000 };
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+10000) };

// EEOF 
enum { EEOF = (EXTEND_ERROR_OFFSET+0),
       ESHORT = (EXTEND_ERROR_OFFSET+1),
       EFORMAT = (EXTEND_ERROR_OFFSET+2),
     };
enum { EXTEND_ERROR_NUM = 3 };

#define PTR_ADDBYTES(ptr,nbytes) ((void*) ( ((unsigned char*)ptr) + nbytes))

#define PTR_DIFFBYTES(end_ptr,start_ptr) (((unsigned char*)end_ptr) - ((unsigned char*)start_ptr))

static inline int is_c_nil(void* x) { return x==NULL; }

#endif
