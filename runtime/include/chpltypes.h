#ifndef _chpltypes_H_
#define _chpltypes_H_

typedef void* _nilType;
extern _nilType nil;

typedef enum __boolean {
  false = 0,
  true = 1
} _boolean;

typedef long long _integer64;

typedef double _float64;

typedef struct __complex128 {
  _float64 re;
  _float64 im;
} _complex128;

typedef char* _string;

#define _init_string() \
  _glom_strings(1, "")

void _copy_string(_string* lhs, _string rhs);
char* _glom_strings(int numstrings, ...);

char* _chpl_tostring_boolean(_boolean x, char* format);
char* _chpl_tostring_integer(_integer64 x, char* format);
char* _chpl_tostring_float(_float64 x, char* format);
char* _chpl_tostring_complex(_complex128 x, char* format);

_string _chpl_string_concat(_string x, _string y);
_string _chpl_string_index(_string* x, int i);
_string _chpl_string_select(_string x, int low, int high);
_string _chpl_string_strided_select(_string x, int low, int high, int stride);
_boolean _chpl_string_equal(_string x, _string y);
_boolean _chpl_string_notequal(_string x, _string y);
_integer64 _chpl_string_length(_string x);

#endif
