/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

int var_id = 1;

Var* 
Var::copy() {
  Var *v = new Var(sym);
  v->type = type;
  v->def = def;
  v->clone_for_constants = clone_for_constants;
  v->avars.copy(avars);
  return v;
}

int
compar_vars(const void *ai, const void *aj) {
  int i = (*(Var**)ai)->id;
  int j = (*(Var**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}
