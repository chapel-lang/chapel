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

// AIX needs _ALL_SOURCE
// in order to get us gai_strerror().
#ifndef _ALL_SOURCE
#define _ALL_SOURCE
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
typedef char c_char_t;
typedef FILE* c_file;

static inline c_file chpl_cstdin(void) { return stdin; }
static inline c_file chpl_cstdout(void) { return stdout; }
static inline c_file chpl_cstderr(void) { return stderr; }

#define c_nil NULL;

enum { EXTEND_ERROR_OFFSET = 1000 };
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+10000) };

static inline int chpl_err_eq(err_t a, err_t b) { return a == b; }
static inline int32_t chpl_err_to_int(err_t a) { return a; }
static inline err_t chpl_int_to_err(int32_t a) { return a; }


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

#define EXTEND_ERROR_NUM 3

#define PTR_ADDBYTES(ptr,nbytes) ((void*) ( ((unsigned char*)ptr) + nbytes))

#define PTR_DIFFBYTES(end_ptr,start_ptr) (((unsigned char*)end_ptr) - ((unsigned char*)start_ptr))

static inline int is_c_nil(void* x) { return x==NULL; }

#endif
