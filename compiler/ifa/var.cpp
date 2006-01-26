#include "defs.h"
#include "var.h"

int var_id = 1;

Var* 
Var::copy() {
  Var *v = new Var(sym);
  v->type = type;
  v->def = def;
  v->is_internal = is_internal;
  v->avars.copy(avars);
  return v;
}

