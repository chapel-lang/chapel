#ifndef _SYS_BASIC_H
#define _SYS_BASIC_H

// get endian.h htobe16, etc
#define _BSD_SOURCE

// get posix_memalign
#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 200112L
#define _POSIX_SOURCE 200112L
#define _FILE_OFFSET_BITS 64

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
