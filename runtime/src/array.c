#include <stdlib.h>
#include <stdio.h>
#include "array.h"


void _init_array1_integer64(_array1_integer64* newarr, _domain1* dom) {
  int d;
  const int rank = 1;

  newarr->elemsize = sizeof(_integer64);
  newarr->domain = dom;
  newarr->dim[rank-1].off = dom->dim[rank-1].lo;
  newarr->dim[rank-1].blk = 1;
  for (d=rank-2; d>=0; d--) {
    newarr->dim[d].off = dom->dim[d].lo;
    newarr->dim[d].blk = newarr->dim[d+1].blk
                       * ((dom->dim[d+1].hi - dom->dim[d+1].lo + 1) 
                          / dom->dim[d+1].str);
  }
  newarr->size = newarr->dim[0].blk 
                 * ((dom->dim[d+1].hi - dom->dim[d+1].lo + 1)
                    / dom->dim[d+1].str);
  newarr->base = (_integer64*)malloc(newarr->size * sizeof(_integer64));
  if (newarr->base == NULL) {
    fprintf(stderr, "ERROR: out of memory");
  }
}


void _init_array2_float64(_array2_float64* newarr, _domain2* dom) {
  int d;
  const int rank = 2;

  newarr->elemsize = sizeof(_float64);
  newarr->domain = dom;
  newarr->dim[rank-1].off = dom->dim[rank-1].lo;
  newarr->dim[rank-1].blk = 1;
  for (d=rank-2; d>=0; d--) {
    newarr->dim[d].off = dom->dim[d].lo;
    newarr->dim[d].blk = newarr->dim[d+1].blk
                       * ((dom->dim[d+1].hi - dom->dim[d+1].lo + 1) 
                          / dom->dim[d+1].str);
  }
  newarr->size = newarr->dim[0].blk 
                 * ((dom->dim[d+1].hi - dom->dim[d+1].lo + 1)
                    / dom->dim[d+1].str);
  newarr->base = (_float64*)malloc(newarr->size * sizeof(_float64));
  if (newarr->base == NULL) {
    fprintf(stderr, "ERROR: out of memory");
  }
}

