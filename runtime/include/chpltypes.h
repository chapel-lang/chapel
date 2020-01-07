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

#ifndef _chpltypes_H_
#define _chpltypes_H_

#include "sys_basic.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // for ptrdiff_t
#include <sys/time.h> // for struct timeval
#ifndef __cplusplus
#include <complex.h>
#endif

// C types usable from Chapel.
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
typedef void* c_void_ptr;
typedef void* c_fn_ptr;  // a white lie
typedef uintptr_t c_uintptr;
typedef intptr_t c_intptr;
typedef ptrdiff_t c_ptrdiff;

// C++ does not support c99 bools
#ifndef __cplusplus
typedef _Bool chpl_bool;
#else
typedef bool chpl_bool;
#endif

static inline void* c_pointer_return(void* x) { return x; }
static inline ptrdiff_t c_pointer_diff(void* a, void* b, ptrdiff_t eltSize) {
  return (((unsigned char*)a) - ((unsigned char*)b))/eltSize;
}

// This allocation of bits is arbitrary.
// Seemingly, 64 bits is enough to represent both the node_id and sublocale_id
// portions  of a locale ID, and an even split is a good first guess.
typedef int32_t c_nodeid_t;
#define PRI_c_nodeid_t PRId32
#define SCN_c_nodeid_t SCNi32
typedef int32_t c_sublocid_t;
#define PRI_c_sublocid_t PRId32
#define SCN_c_sublocid_t SCNi32
typedef int64_t c_localeid_t;

// These are special values that mean "no", "any", and "all sublocales",
// respectively.
#define c_sublocid_none_val -1
#define c_sublocid_any_val  -2
#define c_sublocid_all_val  -3

static const c_sublocid_t c_sublocid_none = c_sublocid_none_val;
static const c_sublocid_t c_sublocid_any  = c_sublocid_any_val;
static const c_sublocid_t c_sublocid_all  = c_sublocid_all_val;

static inline int isActualSublocID(c_sublocid_t subloc) {
  return subloc >= 0;
}

#ifndef LAUNCHER

// The type for wide-pointer-to-void. This is used in the runtime in order to
// store and transmit global variable addresses. It is needed in order to make
// that code able to support packed multilocale pointers.

// We can't include chpl-locale-model.h until after we've defined the node and
// sublocale types and constants, so these cases are also responsible to
// include chpl-locale-model.h.  (note: moving it out of the #ifdef leads to
// problems building the launcher).

#include "chpl-locale-model.h"
typedef struct wide_ptr_s {
  chpl_localeID_t locale;
  void* addr;
} wide_ptr_t;
typedef wide_ptr_t* ptr_wide_ptr_t;

#else
// Just don't define wide_ptr_t. That way, other programs
// (like the launcher) can still use chpltypes.h but anything
// using it that doesn't get a wide pointer definition will
// fail.
//
// We define ptr_wide_ptr_t however so some things, like qthreads
// builds using chpl-comm.h (which uses that type to declare the
// global variables registry), can continue to work.
typedef void* ptr_wide_ptr_t;
#endif // LAUNCHER

#define nil 0 
typedef void* _nilType;
typedef void* _nilRefType;
typedef void* _chpl_object;
typedef void* _chpl_value;
typedef void* chpl_opaque;

#define nilRef 0

// macros for specifying the correct C literal type
#define INT8( i)   ((int8_t)(INT8_C(i)))
#define INT16( i)  ((int16_t)(INT16_C(i)))
#define INT32( i)  ((int32_t)(INT32_C(i)))
#define INT64( i)  ((int64_t)(INT64_C(i)))
#define UINT8( i)  ((uint8_t)(UINT8_C(i)))
#define UINT16( i) ((uint16_t)(UINT16_C(i)))
#define UINT32( i) ((uint32_t)(UINT32_C(i)))
#define UINT64( i) ((uint64_t)(UINT64_C(i)))

#define REAL32(i) ((float)(i))
#define REAL64(i) ((double)(i))

#define COMMID( i)  ((int64_t)(INT64_C(i)))


typedef int8_t chpl_bool8;
typedef int16_t chpl_bool16;
typedef int32_t chpl_bool32;
typedef int64_t chpl_bool64;

typedef void (*chpl_fn_p)(void*); // function pointer for runtime ftable
typedef int16_t chpl_fn_int_t;    // int type for ftable indexing

// Function table names and information, for VisualDebug use
typedef struct _chpl_fn_info {
  const char *name;
  int fileno;
  int lineno;
} chpl_fn_info;

// It is tempting to #undef true and false and then #define them just to be sure
// they expand correctly, but future versions of the C standard may not allow this!
#ifndef __cplusplus
#ifndef false
#define false 0
#endif
#ifndef  true
#define  true 1
#endif
#endif

typedef float               _real32;
typedef double              _real64;
typedef float               _imag32;
typedef double              _imag64;
#ifndef __cplusplus
typedef float complex       _complex64;
typedef double complex      _complex128;
#endif
typedef int64_t             _symbol;

// macros for Chapel min/max -> C stdint.h or values.h min/max
#define MIN_INT8            INT8_MIN
#define MIN_INT16           INT16_MIN
#define MIN_INT32           INT32_MIN
#define MIN_INT64           INT64_MIN
#define MAX_INT8            INT8_MAX
#define MAX_INT16           INT16_MAX
#define MAX_INT32           INT32_MAX
#define MAX_INT64           INT64_MAX

#define MIN_UINT8           (0)
#define MIN_UINT16          (0)
#define MIN_UINT32          (0)
#define MIN_UINT64          (0)
#define MAX_UINT8           UINT8_MAX
#define MAX_UINT16          UINT16_MAX
#define MAX_UINT32          UINT32_MAX
#define MAX_UINT64          UINT64_MAX

// code gen just uses - MAX_FLOATxx #define MIN_FLOAT32         (-FLT_MAX)
//                                  #define MIN_FLOAT64         (-DBL_MAX)
#define MAX_FLOAT32         FLT_MAX
#define MAX_FLOAT64         DBL_MAX

int64_t real2int( _real64 f);       // return the raw bytes of the float
int64_t object2int( _chpl_object o);  // return the ptr

typedef int32_t chpl__class_id;

typedef struct chpl_main_argument_s {
  int64_t argc;
  const char **argv;
  int32_t return_value;
} chpl_main_argument;

#ifndef __cplusplus
static inline _complex128 _chpl_complex128(_real64 re, _real64 im) {
  return re + im*_Complex_I;
}
static inline _complex64 _chpl_complex64(_real32 re, _real32 im) {
  return re + im*_Complex_I;
}

static inline _real64* complex128GetRealRef(_complex128* cplx) {
  return ((_real64*)cplx) + 0;
}
static inline _real64* complex128GetImagRef(_complex128* cplx) {
  return ((_real64*)cplx) + 1;
}
static inline _real32* complex64GetRealRef(_complex64* cplx) {
  return ((_real32*)cplx) + 0;
}
static inline _real32* complex64GetImagRef(_complex64* cplx) {
  return ((_real32*)cplx) + 1;
}

/* 128 bit complex operators for LLVM use */
static inline _complex128 complexMultiply128(_complex128 c1, _complex128 c2) {
  return c1*c2;
}
static inline _complex128 complexDivide128(_complex128 c1, _complex128 c2) {
  return c1/c2;
}
static inline _complex128 complexAdd128(_complex128 c1, _complex128 c2) {
  return c1+c2;
}
static inline _complex128 complexSubtract128(_complex128 c1, _complex128 c2) {
  return c1-c2;
}
static inline _complex128 complexUnaryMinus128(_complex128 c1) {
  return -c1;
}

/* 64 bit complex operators for LLVM use */
static inline _complex64 complexMultiply64(_complex64 c1, _complex64 c2) {
  return c1*c2;
}
static inline _complex64 complexDivide64(_complex64 c1, _complex64 c2) {
  return c1/c2;
}
static inline _complex64 complexAdd64(_complex64 c1, _complex64 c2) {
  return c1+c2;
}
static inline _complex64 complexSubtract64(_complex64 c1, _complex64 c2) {
  return c1-c2;
}
static inline _complex64 complexUnaryMinus64(_complex64 c1) {
  return -c1;
}
#endif

/* This should be moved somewhere else, but where is the question */
const char* chpl_get_argument_i(chpl_main_argument* args, int32_t i);

#include "chpl-string-support.h"

#endif
