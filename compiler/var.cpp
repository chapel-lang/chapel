/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

Var* 
Var::copy() {
  Var *v = new Var(sym);
  v->def = def;
  v->avars.copy(avars);
  return v;
}
