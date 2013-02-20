#ifndef _SYS_BASIC_H
#define _SYS_BASIC_H

#ifndef _BSD_SOURCE
// get endian.h htobe16, valloc, etc
#define _BSD_SOURCE
#endif

#ifndef _DARWIN_C_SOURCE
// to get NI_MAXHOST or NI_MAXSERV
#define _DARWIN_C_SOURCE
#endif

// AIX needs _ALL_SOURCE
// in order to get us gai_strerror().
#ifndef _ALL_SOURCE
#define _ALL_SOURCE
#endif

// get posix_fallocate
// linux man says it needs _XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L
// get pread, pwrite
// linux man says it needs _XOPEN_SOURCE >= 500 || /* Since glibc 2.12: */ _POSIX_C_SOURCE >= 200809
// get readv, writev, preadv, pwritev, and wcwidth/wcswidth
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#ifdef __GNUC__
#define ___always_inline inline __attribute__((__always_inline__))
#else
#define ___always_inline inline
#endif

//#ifdef __MTA__
// MTA defines WCHAR_MAX in stdint and wchar,
// but avoids it if wchar is included before stdint.
//#include <wchar.h>
//#endif

// Ask a C++ compiler if it would please include e.g. INT64_MAX
#define __STDC_CONSTANT_MACROS

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

typedef char c_char;
typedef signed char c_schar;
typedef unsigned char c_uchar;
typedef short c_short;
typedef unsigned short c_ushort;
typedef int c_int;
typedef unsigned int c_uint;
typedef long c_long;
typedef unsigned long c_ulong;
typedef long long c_longlong;
typedef unsigned long long c_ulonglong;
typedef float c_float;
typedef double c_double;
typedef void* c_ptr;
typedef int err_t;
typedef err_t syserr;
typedef int fd_t;
typedef FILE* c_file;


static inline c_file chpl_cstdin(void) { return stdin; }
static inline c_file chpl_cstdout(void) { return stdout; }
static inline c_file chpl_cstderr(void) { return stderr; }

#define c_nil NULL

enum { EXTEND_ERROR_OFFSET = 1000 };
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+10000) };

static inline int chpl_err_eq(err_t a, err_t b) { return a == b; }
static inline int32_t chpl_err_to_int(err_t a) { return a; }
static inline err_t chpl_int_to_err(int32_t a) { return a; }


// EEOF 
#ifndef EEOF
#define EEOF (EXTEND_ERROR_OFFSET+0)
#endif
static inline int chpl_macro_int_EEOF(void) { return EEOF; }

#ifndef ESHORT
#define ESHORT (EXTEND_ERROR_OFFSET+1)
#endif
static inline int chpl_macro_int_ESHORT(void) { return ESHORT; }

#ifndef EFORMAT
#define EFORMAT (EXTEND_ERROR_OFFSET+2)
#endif
static inline int chpl_macro_int_EFORMAT(void) { return EFORMAT; }

// Make sure we have an EILSEQ (missing on XMT)
#ifndef EILSEQ
#define EILSEQ (EXTEND_ERROR_OFFSET+3)
#endif

// Make sure we have EOVERFLOW (missing on XMT)
#ifndef EOVERFLOW
#define EOVERFLOW (EXTEND_ERROR_OFFSET+4)
#endif

#define EXTEND_ERROR_NUM 5

#define PTR_ADDBYTES(ptr,nbytes) ((void*) ( ((unsigned char*)ptr) + nbytes))

#define PTR_DIFFBYTES(end_ptr,start_ptr) (((unsigned char*)end_ptr) - ((unsigned char*)start_ptr))

static inline c_int is_c_nil(void* x) { return x==NULL; }

// Define LLONG_MAX, ULLONG_MAX if it doesn't exist (should be in limits.h)
// This is needed for XMT.
#ifdef __MTA__
#ifndef LLONG_MAX
#define LLONG_MAX LONG_LONG_MAX
#endif
#ifndef ULLONG_MAX
#define ULLONG_MAX (2ULL*LLONG_MAX+1ULL)
#endif
#endif

// Define UINT32_MAX if it doesn't exist (needed for some C++ environments)
// (normally defined in stdint.h)
#ifndef UINT32_MAX
#define UINT32_MAX 0xffffffffu
#endif
#ifndef INT64_MAX
#define INT64_MAX 0x7fffffffffffffffll
#endif


#ifdef __cplusplus
// g++ supports restrict in C++ with the name __restrict. For other compliers,
// we just #define-out restrict.
#if defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define restrict __restrict
#else
#define restrict 
#endif
#endif


#endif
