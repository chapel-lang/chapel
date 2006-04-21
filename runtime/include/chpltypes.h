#ifndef _chpltypes_H_
#define _chpltypes_H_

typedef void* _nilType;
extern _nilType nil;
typedef void* _unspecifiedType;
extern _unspecifiedType _;

typedef enum __bool {
  false = 0,
  true = 1
} _bool;

typedef long long _int64;
typedef unsigned long long _uint64;
typedef double _float64;

typedef struct __complex128 {
  _float64 re;
  _float64 im;
} _complex128;

typedef char* _string;
typedef _int64 _symbol;

#define _init_string() _chpl_calloc(1, sizeof(char), "_init_string")

#define _copy_string(lhs, rhs) (lhs = _glom_strings(1, rhs))

//_string _copy_string(_string* lhs, _string rhs);
char* _glom_strings(int numstrings, ...);

char* _chpl_tostring_bool(_bool x, char* format);
char* _chpl_tostring_int(_int64 x, char* format);
char* _chpl_tostring_float(_float64 x, char* format);
char* _chpl_tostring_complex(_complex128 x, char* format);

_string _chpl_string_concat(_string x, _string y);
_string _chpl_string_index(_string x, int i);
_string _chpl_string_select(_string x, int low, int high);
_string _chpl_string_strided_select(_string x, int low, int high, int stride);
_bool _chpl_string_equal(_string x, _string y);
_bool _chpl_string_notequal(_string x, _string y);
_int64 _chpl_string_length(_string x);

#endif
