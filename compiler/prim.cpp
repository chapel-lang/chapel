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
  if (c->ast && c->ast->prim)
    return c->ast->prim;
  Sym *f = c->rvals.v[0];
  if (f->builtin) {
    Prim *prim = prim_map[0][0].get(f->builtin);
    if (prim == prim_primitive) {
      int nargs = c->rvals.n - 3;
      if (nargs < 0)
	nargs = 0;
      if (c->rvals.v[1]->symbol)
	prim = prim_map[nargs][0].get(c->rvals.v[1]->name);
      else {
	char *n = c->rvals.v[2]->symbol ? c->rvals.v[2]->name : c->rvals.v[2]->builtin;
	assert(n);
	prim = prim_map[nargs][1].get(n);
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
      if (p->rvals.v[1]->sym->symbol)
	prim = prim_map[nargs][0].get(p->rvals.v[1]->sym->name);
      else {
	assert(p->rvals.v[2]->sym->symbol);
	prim = prim_map[nargs][1].get(p->rvals.v[2]->sym->name);
      }
      assert(prim);
    }
    return prim;
  }
  return NULL;
}

static int
compatible_type(PrimType pt, Sym *s) {
  switch (pt) {
    case PRIM_TYPE_ANY: return 1;
    case PRIM_TYPE_SYMBOL: if (s && s->type == sym_symbol) return 1; break;
    case PRIM_TYPE_REF: if (s && s->type == sym_ref) return 1; break;
    case PRIM_TYPE_CONT: if (s && s->type == sym_continuation) return 1; break;
    case PRIM_TYPE_ANY_NUM_A:
    case PRIM_TYPE_ANY_NUM_B: 
      if (s && s->type && s->type->num_type != IF1_NUM_TYPE_NONE) return 1; break;
    case PRIM_TYPE_ANY_INT_A:
    case PRIM_TYPE_ANY_INT_B: 
      if (s && s->type && 
	  (s->type->num_type == IF1_NUM_TYPE_INT || s->type->num_type == IF1_NUM_TYPE_UINT))
	return 1;
      break;
    default: assert(!"case"); break;
  }
  return 0;
}

Prim *
Primitives::find(AST *ast) {
  if (ast->n < 2)
    return 0;
  Prim *prim = NULL;
  if (ast->v[0]->sym && ast->v[0]->sym->builtin)
    prim = prim_map[0][0].get(ast->v[0]->sym->builtin);
  if (!prim) {
    int nargs = ast->n - 2;
    if (nargs < 0)
      nargs = 0;
    if (ast->v[0]->sym && ast->v[0]->sym->type == sym_symbol) {
      assert(nargs == 2);
      prim = prim_map[nargs][0].get(ast->v[1]->sym->name);
      if (!compatible_type(prim->arg_types[0], ast->v[1]->sym))
	return 0;
    }
    else {
      assert(ast->v[1]->sym && ast->v[1]->sym->symbol);
      prim = prim_map[nargs][1].get(ast->v[1]->sym->name);
      if (!prim)
	return 0;
      if (!compatible_type(prim->arg_types[0], ast->v[0]->sym))
	return 0;
      if (nargs > 0)
	if (!compatible_type(prim->arg_types[1], ast->v[2]->sym))
	  return 0;
    }
    assert(prim);
  }
  if (prim == prim_apply) // do not deal with apply at this level
    return 0;
  return prim;
}

