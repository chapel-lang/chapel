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

/*** OLD ***/

#define _DECL_DOMAIN(rank) \
  typedef struct __domain##rank { \
    _dom_perdim dim[rank]; \
  } _domain##rank

_DECL_DOMAIN(1);
_DECL_DOMAIN(2);

void _init_domain_1D(_domain1*,
		     int lo, int hi, int str);
void _init_domain_2D(_domain2*,
		     int lo0, int hi0, int str0,
		     int lo1, int hi1, int str1);

/*** END OLD ***/

#endif
