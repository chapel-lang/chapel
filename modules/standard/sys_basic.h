#ifndef _SYS_BASIC_H
#define _SYS_BASIC_H

#define _BSD_SOURCE
// get posix_memalign
#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 200112L
#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

typedef int c_int;
typedef unsigned int c_uint;
typedef long c_long;
typedef unsigned long c_ulong;
typedef long long c_longlong;
typedef unsigned long long c_ulonglong;
typedef uintptr_t c_ptr;
typedef int err_t;
typedef int fd_t;

#define c_nil NULL

#define EXTEND_ERROR_OFFSET 1000
#define GAI_ERROR_OFFSET (EXTEND_ERROR_OFFSET+10000)

// EEOF 
#define EEOF (EXTEND_ERROR_OFFSET+0) 
// ESHORT is -2
#define ESHORT (EXTEND_ERROR_OFFSET+1)
#define EXTEND_ERROR_NUM 2

#endif
