/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"


void 
Scope::add_dynamic(Scope *s, Sym *sy) {
  dynamic.add(s);
  dynamic_container.add(sy);
}

Sym *
Scope::get_local(char *name, Sym **container) {
  Sym *s;
  if ((s = hash.get(name))) {
    if (container)
      *container = 0;
    return s;
  }
  for (int i = 0; i < dynamic.n; i++) {
    if ((s = dynamic.v[i]->hash.get(name))) {
      if (container)
	*container = dynamic_container.v[i];
      return s;
    }
  }
  return NULL;
}

Sym *
Scope::get(char *name, Sym **container) {
  Sym *s;
  if ((s = get_local(name, container)))
    return s;
  if (up)
    return up->get(name, container);
  return NULL;
}

Scope *
Scope::global() {
  Scope *s = this;
  while (s->up) s = s->up;
  return s;
}

Scope *
Scope::module() {
  Scope *s = this;
  while (s->up->up) s = s->up;
  return s;
}

void
Scope::put(char *name, Sym *s) {
  MapElem<char *, Sym *> e(name, NULL);
  MapElem<char *, Sym *> *x = hash.set_in(e);
  if (x) {
    x->value = s;
  } else
    hash.put(name, s);
}
