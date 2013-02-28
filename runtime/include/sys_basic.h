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
typedef int fd_t;
typedef FILE* c_file;
typedef int err_t;

static inline c_file chpl_cstdin(void) { return stdin; }
static inline c_file chpl_cstdout(void) { return stdout; }
static inline c_file chpl_cstderr(void) { return stderr; }


// TODO -- move this stuff to qio_error.h
#define c_nil NULL

struct qio_err_s {
  int code;
  const char* const_msg;
};
// qioerr is meant to store either an error code directly
// or a pointer to a struct err_s. There are several ways
// it could do that - for example, we could have already
// allocated memory for the system error codes... but here
// we just use pointers with the lowest bit set, since that
// would not be allowed for a pointer to a struct err_s
// because it must be aligned.
//
// NULL means no error.
typedef const struct qio_err_s* qioerr;

// on linux, normal error codes are < 255
enum { EXTEND_ERROR_OFFSET = 1000 };
// on linux, these are -1 to -100.
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+2000) };
enum { MAX_ERROR_OFFSET = GAI_ERROR_OFFSET+1000 };

static inline int32_t qio_err_to_int(qioerr a) {
  intptr_t num = (intptr_t) a;
  if( num == 0 ) return 0;
  if( num & 1 ) {
    // byte-aligned so can't be an error record.
    return num >> 1; // trim off the 1 we added to distinguish
  }
  return ((const struct qio_err_s*) a)->code;
}
static inline const char* qio_err_msg(qioerr a) {
  intptr_t num = (intptr_t) a;
  if( num == 0 ) return 0;
  if( num & 1 ) {
    // byte-aligned so can't be an error record.
    return NULL;
  }
  return ((struct qio_err_s*) a)->const_msg;
}

static inline qioerr qio_int_to_err(int32_t a) {
  intptr_t num = a;
  if( num != 0 ) {
    // add a 1 bit on the right to mark it as non-pointer.
    num <<= 1;
    num += 1;
  }
  return (qioerr) num;
}
static inline int qio_err_eq(qioerr a, qioerr b) {
  // Returns true if they have the same code.
  if( a == NULL && b == NULL ) return 1;
  if( (a == NULL) != (b == NULL) ) return 0;
  return qio_err_to_int(a) == qio_err_to_int(b);
}
static inline int qio_err_iserr(qioerr a) {
  return a != NULL;
}
static inline qioerr qio_mkerror_errno(void) {
  return qio_int_to_err(errno);
}
#define QIO_GET_CONSTANT_ERROR(ptr,code,note) { \
  static const struct qio_err_s qio_macro_tmp_err__ = {code, note}; \
  ptr = &qio_macro_tmp_err__; \
  assert( qio_err_to_int(ptr) == code ); \
}

// EEOF 
#ifndef EEOF
#define EEOF (EXTEND_ERROR_OFFSET+0)
#endif

#ifndef ESHORT
#define ESHORT (EXTEND_ERROR_OFFSET+1)
#endif

#ifndef EFORMAT
#define EFORMAT (EXTEND_ERROR_OFFSET+2)
#endif

// Make sure we have an EILSEQ (missing on XMT)
#ifndef EILSEQ
#define EILSEQ (EXTEND_ERROR_OFFSET+3)
#endif

// Make sure we have EOVERFLOW (missing on XMT)
#ifndef EOVERFLOW
#define EOVERFLOW (EXTEND_ERROR_OFFSET+4)
#endif

#define EXTEND_ERROR_NUM 5

// This could be done optionally only under Chapel.
typedef qioerr syserr;
static inline int chpl_macro_int_EEOF(void) { return EEOF; }
static inline int chpl_macro_int_ESHORT(void) { return ESHORT; }
static inline int chpl_macro_int_EFORMAT(void) { return EFORMAT; }
// end QIO error stuff

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
