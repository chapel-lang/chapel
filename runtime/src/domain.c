#include "domain.h"

void _init_domain_1D(_domain1* newdom, int lo, int hi, int str) {
  newdom->dim[0].lo = lo;
  newdom->dim[0].hi = hi;
  newdom->dim[0].str = str;
}

