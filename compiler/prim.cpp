/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

Prim::Prim(int aindex, char *astring, char *aname, int anargs, int apos, 
	   PrimType *aarg_types, PrimType *aret_types, int options) {
  index = aindex;
  string = astring;
  name = aname;
  nargs = anargs;
  pos = apos;
  arg_types = aarg_types;
  ret_types = aret_types;
  nonfunctional = options & PRIM_NON_FUNCTIONAL;
}

Primitives::Primitives(IF1 *if1) {
  prim_init(this, if1);
}

Prim *
Primitives::find(Code *c) {
  if (c->kind != Code_SEND)
    return NULL;
  Sym *f = c->rvals.v[0];
  if (f->builtin) {
    Prim *prim = prim_map[0][0].get(f->builtin);
    if (prim == prim_primitive) {
      int nargs = c->rvals.n - 3;
      if (nargs < 0)
	nargs = 0;
      if (c->rvals.v[1]->type == sym_symbol)
	prim = prim_map[nargs][0].get(c->rvals.v[1]->name);
      else {
	assert(c->rvals.v[2]->type == sym_symbol);
	prim = prim_map[nargs][1].get(c->rvals.v[2]->name);
      }
      assert(prim);
    }
    return prim;
  }
  return NULL;
}

Prim *
Primitives::find(PNode *p) {
  Var *f = p->rvals.v[0];
  if (f->sym->builtin) {
    Prim *prim = prim_map[0][0].get(f->sym->builtin);
    if (prim == prim_primitive) {
      int nargs = p->rvals.n - 3;
      if (nargs < 0)
	nargs = 0;
      if (p->rvals.v[1]->sym->type == sym_symbol)
	prim = prim_map[nargs][0].get(p->rvals.v[1]->sym->name);
      else {
	assert(p->rvals.v[2]->sym->type == sym_symbol);
	prim = prim_map[nargs][1].get(p->rvals.v[2]->sym->name);
      }
      assert(prim);
    }
    return prim;
  }
  return NULL;
}
