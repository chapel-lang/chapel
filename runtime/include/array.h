#ifndef _array_H_
#define _array_H_

#include "chpltypes.h" // only needed while _DECL_ARRAY is instantiated here
#include "domain.h"

typedef struct __arr_perdim {
  int off;
  int blk;
} _arr_perdim;

#define _DECL_ARRAY(elemtype, rank) \
  typedef struct __array##rank##elemtype { \
    int elemsize; \
    int size; \
    elemtype* base; \
    elemtype* origin; \
    _domain##rank* domain; \
    _arr_perdim dim[rank]; \
  } _array##rank##elemtype
					   

#define _ACC1(arr, i0) \
  arr.base[((i0)-arr.dim[0].off)*arr.dim[0].blk]


#define _ACC2(arr, i0, i1) \
  arr.base[((i0)-arr.dim[0].off)*arr.dim[0].blk + \
           ((i1)-arr.dim[1].off)*arr.dim[1].blk] \

_DECL_ARRAY(_integer64, 1);
_DECL_ARRAY(_float64, 2);
void _init_array1_integer64(_array1_integer64* newarr, _domain1* dom);
void _init_array2_float64(_array2_float64* newarr, _domain2* dom);

#endif

