/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

Sym *
Scope::get_local(char *name) {
  Sym *s;
  if ((s = hash.get(name)))
    return s;
  forv_Scope(sc, dynamic)
    if ((s = sc->hash.get(name)))
      return s;
  return NULL;
}


Sym *
Scope::get(char *name) {
  Sym *s;
  if ((s = get_local(name)))
    return s;
  if (up)
    return up->get(name);
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
    s->next_sym = x->value;
    x->value = s;
  } else
    hash.put(name, s);
}
