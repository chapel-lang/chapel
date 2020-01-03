/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SYS_BASIC_H
#define _SYS_BASIC_H

#ifndef _BSD_SOURCE
// get endian.h htobe16, valloc, etc
#define _BSD_SOURCE
#endif

// to get NI_MAXHOST or NI_MAXSERV
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif
#ifndef _NETBSD_SOURCE
#define _NETBSD_SOURCE
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

#ifndef _DEFAULT_SOURCE
// Quiets warnings about _BSD_SOURCE being deprecated in glibc >= 2.20
// This define enables everything _BSD_SOURCE does (and more) with glibc >= 2.19
#define _DEFAULT_SOURCE
#endif

//
// The following breaks #include of "glob.h" with the Cray CCE
// compiler and also complicates things for the #inclusion of dirent.h
// with most PrgEnv-* options on Crays, as seen in chpldirent.h.
// Michael added this in order to permit the support of files larger than 4GB
// on 32-bit platforms.
// Note that it's necessary to set this flag for Ubuntu 14.04 32-bit to get a
// working preadv/pwritev, so we set that in a Makefile. One possibility
// would be to set it here for 32-bit platforms only (e.g. using
// __SIZEOF_POINTER__ == 4 which would work in GCC).
//
//#ifndef _FILE_OFFSET_BITS
//#define _FILE_OFFSET_BITS 64
//#endif

// Ask a C++ compiler if it would please include e.g. INT64_MAX
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

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
// g++ supports restrict in C++ with the name __restrict. For other compilers,
// we just #define-out restrict.
#if defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define restrict __restrict
#else
#define restrict 
#endif
#endif


#endif
