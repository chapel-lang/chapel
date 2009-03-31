#ifndef _chpltypes_H_
#define _chpltypes_H_

#include <stdint.h>

#define nil 0
#define _nilType void*

#define nilRef 0
#define _nilRefType void*

#define _chpl_object void*
#define _chpl_value void*
#define chpl_opaque void*

// macros for specifying the correct C constant type
#define INT8( i)   (i)
#define INT16( i)  (i)
#define INT32( i)  (i ## L)
#define INT64( i)  (i ## LL)
#define UINT8( i)  (i ## U)
#define UINT16( i) (i ## U)
#define UINT32( i) (i ## UL)
#define UINT64( i) (i ## ULL)

typedef _Bool chpl_bool;
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
typedef struct __complex64 { _real32 re; _real32 im; } _complex64;
typedef struct __complex128 { _real64 re; _real64 im; } _complex128;
typedef const char*         chpl_string;
typedef int64_t              _symbol;


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

#define ascii(s) ((int8_t)(*s))

struct __chpl____wide_chpl_string;

//
// stopgap formatting
//
chpl_string _format(chpl_string format, ...)
  __attribute__((format(printf, 1, 2)));

char* chpl_glom_strings(int numstrings, ...);

chpl_bool string_contains(chpl_string x, chpl_string y);
chpl_string string_copy(chpl_string x, int32_t lineno, chpl_string filename);
chpl_string chpl_wide_string_copy(struct __chpl____wide_chpl_string* x, int32_t lineno, chpl_string filename);
chpl_string string_concat(chpl_string x, chpl_string y, int32_t lineno, chpl_string filename);
chpl_string string_index(chpl_string x, int i, int32_t lineno, chpl_string filename);
chpl_string string_select(chpl_string x, int low, int high, int32_t lineno, chpl_string filename);
chpl_string string_strided_select(chpl_string x, int low, int high, int stride, int32_t lineno, chpl_string filename);
int32_t chpl_string_compare(chpl_string x, chpl_string y);
int64_t string_length(chpl_string x);

int64_t real2int( _real64 f);       // return the raw bytes of the float
int64_t object2int( _chpl_object o);  // return the ptr

#endif
