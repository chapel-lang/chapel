#ifndef _domain_H_
#define _domain_H_

typedef struct __dom_perdim {
  int lo;
  int hi;
  int str;
} _dom_perdim;

#define _INIT_DOMAIN_DIM(dom, dim, new_lo, new_hi, new_str) \
  dom.dim_info[dim].lo = new_lo; \
  dom.dim_info[dim].hi = new_hi; \
  dom.dim_info[dim].str = new_str

#endif
