#include "geysa.h"
#include "prim.h"
#include "if1.h"
#include "builtin.h"
#include "pnode.h"
#include "var.h"
#include "ast.h"

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
    } else if (prim == prim_primitive) {
       char *name = c->rvals.v[1]->name;
       if (!name) name = c->rvals.v[1]->constant;
       if (!name) name = c->rvals.v[1]->imm.v_string;
       Prim *pp = prim_map[0][0].get(name);
       if (pp)
         return pp;
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
    } else if (prim == prim_primitive) {
      char *name = p->rvals.v[1]->sym->name;
      if (!name) name = p->rvals.v[1]->sym->constant;
      if (!name) name = p->rvals.v[1]->sym->imm.v_string;
      Prim *pp = prim_map[0][0].get(name);
      if (pp)
        return pp;
    }
    return prim;
  }
  return NULL;
}

