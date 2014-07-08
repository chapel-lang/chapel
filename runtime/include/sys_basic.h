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

// Ask a C++ compiler if it would please include e.g. INT64_MAX
#define __STDC_CONSTANT_MACROS

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define PTR_ADDBYTES(ptr,nbytes) ((void*) ( ((unsigned char*)ptr) + nbytes))

#define PTR_DIFFBYTES(end_ptr,start_ptr) (((unsigned char*)end_ptr) - ((unsigned char*)start_ptr))

// Define LLONG_MAX, ULLONG_MAX if it doesn't exist (should be in limits.h)
#ifndef ULLONG_MAX
#define ULLONG_MAX (2ULL*LLONG_MAX+1ULL)
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
