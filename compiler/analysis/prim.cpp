/* -*-Mode: c++;-*-
 Copyright 2003-2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "prim.h"
#include "if1.h"
#include "builtin.h"
#include "pnode.h"
#include "var.h"
#include "ast.h"
#include "parse_ast.h"

Prim::Prim(int aindex, char *astring, char *aname, int anargs, int apos, int anrets,
	   PrimType *aarg_types, PrimType *aret_types, int options) {
  index = aindex;
  string = astring;
  name = aname;
  nargs = anargs;
  pos = apos;
  nrets = anrets;
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
  if (f->is_builtin) {
    char *name = if1->builtins_names.get(f);
    Prim *prim = prim_map[0][0].get(name);
    if (prim == prim_operator) {
      int nargs = c->rvals.n - 3;
      if (nargs < 0)
	nargs = 0;
      if (c->rvals.v[1]->is_symbol)
	prim = prim_map[nargs][0].get(c->rvals.v[1]->name);
      else {
	char *n = c->rvals.v[2]->is_symbol ? c->rvals.v[2]->name : 0;
	if (!n)
	  n = if1->builtins_names.get(c->rvals.v[2]);
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
  if (f->sym->is_builtin) {
    char *name = if1->builtins_names.get(f->sym);
    Prim *prim = prim_map[0][0].get(name);
    if (prim == prim_operator) {
      int nargs = p->rvals.n - 3;
      if (nargs < 0)
	nargs = 0;
      if (p->rvals.v[1]->sym->is_symbol)
	prim = prim_map[nargs][0].get(p->rvals.v[1]->sym->name);
      else {
	assert(p->rvals.v[2]->sym->is_symbol);
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
      if (s && s->type && s->type->num_kind != IF1_NUM_KIND_NONE) return 1; break;
    case PRIM_TYPE_ANY_INT_A:
    case PRIM_TYPE_ANY_INT_B: 
      if (s && s->type && 
	  (s->type->num_kind == IF1_NUM_KIND_INT || s->type->num_kind == IF1_NUM_KIND_UINT))
	return 1;
      break;
    default: assert(!"case"); break;
  }
  return 0;
}

Prim *
Primitives::find(ParseAST *ast) {
  if (ast->children.n < 2)
    return 0;
  Prim *prim = NULL;
  if (ast->children.v[0]->sym && ast->children.v[0]->sym->is_builtin) {
    char *name = if1->builtins_names.get(ast->children.v[0]->sym);
    prim = prim_map[0][0].get(name);
  }
  if (!prim) {
    int nargs = ast->children.n - 2;
    if (nargs < 0)
      nargs = 0;
    if (ast->children.v[0]->sym && ast->children.v[0]->sym->is_symbol) {
      prim = prim_map[nargs][0].get(ast->children.v[0]->sym->name);
      if (!prim)
	return 0;
      if (!compatible_type(prim->arg_types[0], ast->children.v[1]->sym))
	return 0;
    }
    else {
      assert(ast->children.v[1]->sym && ast->children.v[1]->sym->is_symbol);
      prim = prim_map[nargs][1].get(ast->children.v[1]->sym->name);
      if (!prim)
	return 0;
      if (!compatible_type(prim->arg_types[0], ast->children.v[0]->sym))
	return 0;
      if (nargs > 0)
	if (!compatible_type(prim->arg_types[1], ast->children.v[2]->sym))
	  return 0;
    }
    assert(prim);
  }
  if (prim == prim_apply) // do not deal with apply at this level
    return 0;
  return prim;
}

