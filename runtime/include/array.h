#ifndef _array_H_
#define _array_H_

#include "chplmem.h"
#include "chpltypes.h" /* only needed while _DECL_ARRAY is instantiated here */
#include "domain.h"

#define _data_construct(t) NULL
#define _data_alloc(t, x, size, desc) x = _chpl_malloc(size, sizeof(t), desc)
#define _data_set(x, i, v) x[i] = v
#define _data_get(x, i) (x[i])

typedef struct __arr_perdim {
  int off;
  int blk;
} _arr_perdim;

#define _INIT_ARRAY(rank, arr, dom, elemtype) \
  { \
    int __dim; \
    arr.elemsize = sizeof(elemtype); \
    arr.domain = &dom; \
    arr.dim_info[rank-1].off = dom.dim_info[rank-1].lo; \
    arr.dim_info[rank-1].blk = 1; \
    for (__dim=rank-2; __dim>=0; __dim--) { \
      arr.dim_info[__dim].off = dom.dim_info[__dim].lo; \
      arr.dim_info[__dim].blk = arr.dim_info[__dim+1].blk \
        * ((dom.dim_info[__dim+1].hi - dom.dim_info[__dim+1].lo + 1)  \
           / dom.dim_info[__dim+1].str); \
    } \
    arr.size = arr.dim_info[0].blk \
      * ((dom.dim_info[__dim+1].hi - dom.dim_info[__dim+1].lo + 1) \
         / dom.dim_info[__dim+1].str); \
    arr.base = (elemtype*)_chpl_malloc(arr.size, sizeof(elemtype), "array " #arr); \
  }

#define _ACC1(arr, i0) \
  arr.base[((i0)-arr.dim_info[0].off)*arr.dim_info[0].blk]

#define _ACC2(arr, i0, i1) \
  arr.base[((i0)-arr.dim_info[0].off)*arr.dim_info[0].blk + \
           ((i1)-arr.dim_info[1].off)*arr.dim_info[1].blk]

#define _ACC3(arr, i0, i1, i2) \
  arr.base[((i0)-arr.dim_info[0].off)*arr.dim_info[0].blk + \
           ((i1)-arr.dim_info[1].off)*arr.dim_info[1].blk + \
           ((i2)-arr.dim_info[2].off)*arr.dim_info[2].blk]

#endif

