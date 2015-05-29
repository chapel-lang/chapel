/*
 * Copyright 2004-2015 Cray Inc.
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // for ptrdiff_t
#include <sys/time.h> // for struct timeval
#include <complex.h>

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
typedef uintptr_t c_uintptr;
typedef intptr_t c_intptr;
typedef ptrdiff_t c_ptrdiff;

// C++ does not support c99 bools
#ifndef __cplusplus
typedef _Bool chpl_bool;
#else
typedef bool chpl_bool;
#endif

#define c_nil NULL
static inline chpl_bool is_c_nil(void* x) { return (chpl_bool)(x==c_nil); }
static inline void* c_pointer_return(void* x) { return x; }

typedef enum {
  CHPL_TYPE_chpl_bool,
  CHPL_TYPE_chpl_bool8,
  CHPL_TYPE_chpl_bool16,
  CHPL_TYPE_chpl_bool32,
  CHPL_TYPE_chpl_bool64,
  CHPL_TYPE_enum,
  CHPL_TYPE_int8_t,
  CHPL_TYPE_int16_t,
  CHPL_TYPE_int32_t,
  CHPL_TYPE_int64_t,
  CHPL_TYPE_uint8_t,
  CHPL_TYPE_uint16_t,
  CHPL_TYPE_uint32_t,
  CHPL_TYPE_uint64_t,
  CHPL_TYPE__real32,
  CHPL_TYPE__real64,
  CHPL_TYPE__imag32,
  CHPL_TYPE__imag64,
  CHPL_TYPE__complex64,
  CHPL_TYPE__complex128,
  CHPL_TYPE_chpl_string,
  CHPL_TYPE_wide_string,
  CHPL_TYPE__cfile,
  CHPL_TYPE_chpl_task_list_p,
  CHPL_TYPE__timevalue,
  CHPL_TYPE_chpl_sync_aux_t,
  CHPL_TYPE_chpl_single_aux_t,
  CHPL_TYPE_chpl_taskID_t,
  CHPL_TYPE__symbol,
  CHPL_TYPE_CLASS_REFERENCE,
  CHPL_TYPE_DONE
} chplType;

typedef struct _chpl_fieldType {
  chplType type;
  size_t offset;
} chpl_fieldType;

// This allocation of bits is arbitrary.
// Seemingly, 64 bits is enough to represent both the node_id and sublocale_id
// portions  of a locale ID, and an even split is a good first guess.
typedef int32_t c_nodeid_t;
#define FORMAT_c_nodeid_t PRId32
typedef int32_t c_sublocid_t;
#define FORMAT_c_sublocid_t PRId32
typedef int64_t c_localeid_t;

// These are special values that mean "no sublocale and "any sublocale".
#define c_sublocid_none_val -1
#define c_sublocid_any_val  -2

static const c_sublocid_t c_sublocid_none = c_sublocid_none_val;
static const c_sublocid_t c_sublocid_any  = c_sublocid_any_val;

#ifndef LAUNCHER

// The type for wide-pointer-to-void. This is used in the runtime in order to
// store and transmit global variable addresses. It is needed in order to make
// that code able to support packed multilocale pointers.

// We can't include chpl-locale-model.h until after we've defined the node and
// sublocale types and constants, so these cases are also responsible to
// include chpl-locale-model.h.  (note: moving it out of the #ifdef leads to
// problems building the launcher).

#ifdef CHPL_WIDE_POINTER_STRUCT
#include "chpl-locale-model.h"
typedef struct wide_ptr_s {
  chpl_localeID_t locale;
  void* addr;
} wide_ptr_t;
typedef wide_ptr_t* ptr_wide_ptr_t;
#else
// It's useful to have the type for a wide pointer-to-void.
// This is the packed pointer version (the other version would be
// {{node,subloc}, address}).
#ifdef CHPL_WIDE_POINTER_PACKED
#include "chpl-locale-model.h"
typedef void * wide_ptr_t;
typedef wide_ptr_t* ptr_wide_ptr_t;
#ifndef CHPL_WIDE_POINTER_NODE_BITS
#error Missing packed wide pointer definition CHPL_WIDE_POINTER_NODE_BITS
#endif

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
#endif

#endif

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


typedef int8_t chpl_bool8;
typedef int16_t chpl_bool16;
typedef int32_t chpl_bool32;
typedef int64_t chpl_bool64;

typedef void (*chpl_fn_p)(void*); // function pointer for runtime ftable
typedef int16_t chpl_fn_int_t;    // int type for ftable indexing

// It is tempting to #undef true and false and then #define them just to be sure
// they expand correctly, but future versions of the C standard may not allow this!
#ifndef false
#define false 0
#endif
#ifndef  true
#define  true 1
#endif

typedef float               _real32;
typedef double              _real64;
typedef float               _imag32;
typedef double              _imag64;
typedef float complex       _complex64;
typedef double complex      _complex128;
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

_complex128 buildComplex128(_real64 re, _real64 im);
_complex64 buildComplex64(_real32 re, _real32 im);

/* This should be moved somewhere else, but where is the question */
const char* chpl_get_argument_i(chpl_main_argument* args, int32_t i);

#include "chpl-string-support.h"

#endif
