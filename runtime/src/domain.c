#include "domain.h"

_domain1 _init_domain_1D(int lo, int hi, int str) {
  _domain1 retval;

  retval.dim[0].lo = lo;
  retval.dim[0].hi = hi;
  retval.dim[0].str = str;

  return retval;
}

