/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

Var* 
Var::copy() {
  Var *v = new Var(sym);
  v->type = type;
  v->def = def;
  v->clone_for_constants = clone_for_constants;
  v->avars.copy(avars);
  return v;
}
