#ifndef _chpltypes_H_
#define _chpltypes_H_

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


void _copy_string(_string* lhs, _string rhs);


#endif

