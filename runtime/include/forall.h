#ifndef _forall_H_
#define _forall_H_

#define _FORALL(ind, dom, d) \
  for (ind=dom.dim_info[d].lo; ind<=dom.dim_info[d].hi; ind+=dom.dim_info[d].str)

#define _FOR(ind, dom, d) \
  _FORALL(ind, dom, d)

#endif
