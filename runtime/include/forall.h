#ifndef _forall_H_
#define _forall_H_

#define _FORALL(ind, dom, d) \
  for (ind=dom.dim[d].lo; ind<=dom.dim[d].hi; ind+=dom.dim[d].str)

#define _FOR(ind, dom, d) \
  _FORALL(ind, dom, d)

#endif
