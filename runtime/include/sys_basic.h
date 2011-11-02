#ifndef _SYS_BASIC_H
#define _SYS_BASIC_H

#ifndef _BSD_SOURCE
// get endian.h htobe16, etc
#define _BSD_SOURCE
#endif

#ifndef _DARWIN_C_SOURCE
// to get NI_MAXHOST or NI_MAXSERV
#define _DARWIN_C_SOURCE
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
#define ___always_inline inline __attribute__((__always_inline__))
#else
#define ___always_inline inline
#endif

#ifndef CHPL_NO_GASNET

#define GASNETT_THREAD_SAFE 1
#define GASNETI_THREAD_SAFE 1

// If gasnet.h and gasnet_tools.h are both included,
// we're supposed to include gasnet.h first.
#ifdef PLEASE_INCLUDE_GASNET_H
#include "gasnet.h"
#include "gasnet_tools.h"
#endif

/*
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
*/

#ifdef PLEASE_INCLUDE_GASNET_TOOLS_H
// GASNet manuals say they want this to be the 1st thing included.
// If we're not compiling chapel codes, we'll fall back on
// e.g. __sync_fetch_add
#include "gasnet_tools.h"
#endif

// end of ifndef CHPL_NO_GASNET
#endif

//#include <features.h> including features.h not necessary for feature
// test macros.

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
//typedef wchar_t c_wchar_t; it's a bad idea to use wchar_t
// we use int32_t instead
typedef char c_char_t;
typedef FILE* c_file;

static inline c_file chpl_cstdin(void) { return stdin; }
static inline c_file chpl_cstdout(void) { return stdout; }
static inline c_file chpl_cstderr(void) { return stderr; }

#define c_nil NULL;

enum { EXTEND_ERROR_OFFSET = 1000 };
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+10000) };

//enum { ENOERR = 0 };

static inline int chpl_err_eq(err_t a, err_t b) { return a == b; }
static inline int32_t chpl_err_to_int(err_t a) { return a; }
static inline err_t chpl_int_to_err(int32_t a) { return a; }

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
