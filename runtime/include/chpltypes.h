#ifndef _chpltypes_H_
#define _chpltypes_H_

#include <stdint.h>

#define nil 0
#define _nilType void*

#define _chpl_object void*
#define _chpl_value void*

// macros for specifying the correct C constant type
#define INT8( i)   (i)
#define INT16( i)  (i)
#define INT32( i)  (i ## L)
#define INT64( i)  (i ## LL)
#define UINT8( i)  (i ## U)
#define UINT16( i) (i ## U)
#define UINT32( i) (i ## UL)
#define UINT64( i) (i ## ULL)

typedef char _chpl_bool;
#define false 0
#define  true 1

typedef signed char         _int8;
typedef short int           _int16;
typedef int                 _int32;
typedef long long int       _int64;
typedef unsigned char       _uint8;
typedef unsigned short int  _uint16;
typedef unsigned int        _uint32;
typedef unsigned long long  _uint64;
typedef float               _real32;
typedef double              _real64;
typedef float               _imag32;
typedef double              _imag64;
typedef struct __complex64 { _real32 re; _real32 im; } _complex64;
typedef struct __complex128 { _real64 re; _real64 im; } _complex128;
typedef const char*         _string;
typedef _int64              _symbol;


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

#define MIN_FLOAT32         (-FLT_MAX)
#define MIN_FLOAT64         (-DBL_MAX)
#define MAX_FLOAT32         FLT_MAX
#define MAX_FLOAT64         DBL_MAX

#define ascii(s) ((_int8)(*s))

#define string_copy(rhs, lineno, filename) (string_concat(rhs, "", lineno, filename))

//
// stopgap formatting
//
_string _format(_string format, ...)
  __attribute__((format(printf, 1, 2)));

char* _glom_strings(int numstrings, ...);

_chpl_bool string_contains(_string x, _string y);
_string string_concat(_string x, _string y, _int32 lineno, _string filename);
_string string_index(_string x, int i, _int32 lineno, _string filename);
_string string_select(_string x, int low, int high, _int32 lineno, _string filename);
_string string_strided_select(_string x, int low, int high, int stride, _int32 lineno, _string filename);
_chpl_bool string_equal(_string x, _string y);
_int64 string_length(_string x);

_int64 real2int( _real64 f);       // return the raw bytes of the float
_int64 object2int( _chpl_object o);  // return the ptr

#endif
