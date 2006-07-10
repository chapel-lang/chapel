#ifndef _chpltypes_H_
#define _chpltypes_H_

typedef void* _nilType;
extern _nilType nil;
typedef void* _unspecifiedType;
extern _unspecifiedType _;

/* macros for specifying the correct C constant type */
#define INT8( i)   (i)
#define INT16( i)  (i)
#define INT32( i)  (i ## L)
#define INT64( i)  (i ## LL)
#define UINT8( i)  (i ## U)
#define UINT16( i) (i ## U)
#define UINT32( i) (i ## UL)
#define UINT64( i) (i ## ULL)

typedef enum __bool {
  false = 0,
  true = 1
} _bool;

typedef char               _int8;
typedef short int          _int16;
typedef int                _int32;
typedef long long int      _int64;
typedef unsigned char      _uint8;
typedef unsigned short int _uint16;
typedef unsigned int       _uint32;
typedef unsigned long long _uint64;
typedef float              _float32;
typedef double             _float64;
typedef long double        _float128;

#define ascii(s) ((_int8)(*s))

typedef struct __complex128 {
  _float64 re;
  _float64 im;
} _complex128;

typedef char* _string;
typedef _int64 _symbol;

#define _init_string() _chpl_calloc(1, sizeof(char), "_init_string")

#define string_copy(rhs) (_glom_strings(1, rhs))

//_string _copy_string(_string* lhs, _string rhs);
char* _glom_strings(int numstrings, ...);

char* _chpl_tostring_bool(_bool x, char* format);
char* _chpl_tostring_int(_int64 x, char* format);
char* _chpl_tostring_float(_float64 x, char* format);
char* _chpl_tostring_complex(_complex128 x, char* format);

_string string_concat(_string x, _string y);
_string string_index(_string x, int i);
_string string_select(_string x, int low, int high);
_string string_strided_select(_string x, int low, int high, int stride);
_bool string_equal(_string x, _string y);
_int64 string_length(_string x);

#endif
