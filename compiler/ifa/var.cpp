#include "defs.h"
#include "var.h"
#include "if1.h"

int var_id = 1;

Var::Var(Sym *s) : sym(s), type(NULL), mark(0), def(0), as_CreationSet(0), is_internal(0),
                   is_filtered(0), is_formal(0), cg_string(0) 
{
  id = var_id++; 
}

Var* 
Var::copy() {
  Var *v = new Var(sym);
  v->type = type;
  v->def = def;
  v->is_internal = is_internal;
  v->avars.copy(avars);
  return v;
}

void
pp(Var *v) {
  printf("(Var %d ", v->id);
  if1_dump_sym(stdout, v->sym);
  if (v->is_internal)
    printf(" INTERNAL ");
  if (v->is_filtered)
    printf(" FILTERED ");
  if (v->is_formal)
    printf(" FORMAL ");
  printf(")");
}
