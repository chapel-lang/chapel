#include "domain.h"

/* BLC: These need to be generalized and/or generated */


void _init_domain_1D(_domain1* newdom, int lo, int hi, int str) {
  newdom->dim[0].lo = lo;
  newdom->dim[0].hi = hi;
  newdom->dim[0].str = str;
}


void _init_domain_2D(_domain2* newdom, int lo0, int hi0, int str0,
		     int lo1, int hi1, int str1) {
  newdom->dim[0].lo = lo0;
  newdom->dim[0].hi = hi0;
  newdom->dim[0].str = str0;
  newdom->dim[1].lo = lo1;
  newdom->dim[1].hi = hi1;
  newdom->dim[1].str = str1;
}

