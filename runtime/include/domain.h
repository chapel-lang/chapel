#ifndef _domain_H_
#define _domain_H_

typedef struct __dom_perdim {
  int lo;
  int hi;
  int str;
} _dom_perdim;

#define _DECL_DOMAIN(rank) \
  typedef struct __domain##rank { \
    _dom_perdim dim[rank]; \
  } _domain##rank

_DECL_DOMAIN(1);

#define _default_format_domain1 "%d..%d by %d"

void _init_domain_1D(_domain1*, int lo, int hi, int str);


#endif

