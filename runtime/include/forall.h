#ifndef _forall_H_
#define _forall_H_

#define _FORALL_DIM(ind, lo, hi, str) \
  for (ind = lo; ind <= hi; ind += str)

#define _FORALL(ind, dom, d) \
  _FORALL_DIM(ind, dom.dim_info[d].lo, dom.dim_info[d].hi, dom.dim_info[d].str)

#define _FOR_DIM(ind, lo, hi, str) \
  _FORALL_DIM(ind, lo, hi, str)

#define _FOR(ind, dom, d) \
  _FORALL(ind, dom, d)

#endif
