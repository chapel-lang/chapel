/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

Fun::Fun(PDB *apdb, Sym *asym, int aninit_function) {
  pdb = apdb;
  init_function = aninit_function;
  clone_for_constants = 1;
  fa_collected = 0;
  sym = asym;
  asym->fun = this;
  ast = sym->ast;
  cg_string = 0;
  if (verbose_level) 
    if (asym->name)
      printf("function %s\n", asym->name);
    else
      printf("function %d\n", asym->id);
  build_cfg();
  build_ssu();
  setup_ast();
}

void
Fun::setup_ast() {
  Vec<PNode *> nodes;
  collect_PNodes(nodes);
  forv_PNode(n, nodes) {
    if (n->code && n->code->ast)
      n->code->ast->pnodes.add(n);
  }
}

void
Fun::collect_PNodes(Vec<PNode *> &v) {
  Vec<PNode *> sv;
  if (!entry)
    return;
  v.add(exit);
  sv.set_add(exit);
  for (int i = 0; i < v.n; i++) {
    forv_PNode(p, v.v[i]->cfg_pred)
      if (sv.set_add(p))
	v.add(p);
  }
}

void
collect_Vars_PNode(PNode *n, Vec<Var *> &vars) {
  forv_Var(v, n->rvals)
    vars.set_add(v);
  forv_Var(v, n->lvals)
    vars.set_add(v);
  forv_PNode(p, n->phi)
    collect_Vars_PNode(p, vars);
  forv_PNode(p, n->phy)
    collect_Vars_PNode(p, vars);
}

void
Fun::collect_Vars(Vec<Var *> &vars, Vec<PNode *> *nodes) {
  Vec<PNode *> sv, v;
  if (!nodes) nodes = &v;
  if (!entry)
    return;
  nodes->add(exit);
  for (int i = 0; i < nodes->n; i++) {
    collect_Vars_PNode(nodes->v[i], vars);
    forv_PNode(p, nodes->v[i]->cfg_pred)
      if (sv.set_add(p))
	nodes->add(p);
  }
  forv_Sym(s, sym->args)
    vars.set_add(s->var);
  vars.set_add(sym->ret->var);
  vars.set_to_vec();
}

static PNode *
copy_pnode(PNode *node, Fun *f, VarMap &vmap) {
  PNode *n = new PNode(node->code);
  n->rvals.copy(node->rvals);
  n->lvals.copy(node->lvals);
  n->cfg_succ.copy(node->cfg_succ);
  n->cfg_pred.copy(node->cfg_pred);
  n->phi.copy(node->phi);
  n->phy.copy(node->phy);
  n->prim = node->prim;
  for (int i = 0; i < n->rvals.n; i++) {
    Var *v = n->rvals.v[i];
    if (v->sym->in == f->sym) {
      if (!(v = vmap.get(n->rvals.v[i]))) {
	v = n->rvals.v[i]->copy();
	vmap.put(n->rvals.v[i], v);
      }
    }
    n->rvals.v[i] = v;
  }
  for (int i = 0; i < n->lvals.n; i++) {
    Var *v = n->lvals.v[i];
    if (v->sym->in == f->sym) {
      if (!(v = vmap.get(n->lvals.v[i]))) {
	v = n->lvals.v[i]->copy();
	vmap.put(n->lvals.v[i], v);
      }
    }
    n->lvals.v[i] = v;
  }
  return n;
}

Fun *
Fun::copy() {
  Fun *f = new Fun();
  f->pdb = pdb;
  f->sym = sym;
  f->init_function = init_function;

  PNodeMap nmap;
  VarMap vmap;
  Vec<PNode *> nodes;

  forv_PNode(n, fa_all_PNodes) {
    PNode *p = copy_pnode(n, f, vmap);
    nodes.add(p);
    nmap.put(n, p);
    for (int i = 0; i < n->phi.n; i++) {
      PNode *p = copy_pnode(n->phi.v[i], f, vmap);
      nodes.add(p);
      nmap.put(n, p);
      p->phi.v[i] = p;
    }
    for (int i = 0; i < n->phy.n; i++) {
      PNode *p = copy_pnode(n->phy.v[i], f, vmap);
      nodes.add(p);
      nmap.put(n, p);
      p->phy.v[i] = p;
    }
  }
  forv_PNode(n, nodes) {
    for (int i = 0; i < n->cfg_succ.n; i++)
      n->cfg_succ.v[i] = nmap.get(n->cfg_succ.v[i]);
    for (int i = 0; i < n->cfg_pred.n; i++)
      n->cfg_pred.v[i] = nmap.get(n->cfg_pred.v[i]);
  }
  for (int i = 0; i < vmap.n; i++)
    if (vmap.v[i].key)
      vmap.v[i].value->def = nmap.get(vmap.v[i].value->def);
  f->fa_all_PNodes.copy(fa_all_PNodes);
  for (int i = 0; i < f->fa_all_PNodes.n; i++)
    f->fa_all_PNodes.v[i] = nmap.get(f->fa_all_PNodes.v[i]);
  f->fa_all_Vars.copy(fa_all_Vars);
  for (int i = 0; i < f->fa_all_Vars.n; i++) {
    Var *v = vmap.get(f->fa_all_Vars.v[i]);
    if (!v)
      f->fa_all_Vars.v[i] = f->fa_all_Vars.v[i];
    else
      f->fa_all_Vars.v[i] = v;
  }
  f->entry = nmap.get(entry);
  f->exit = nmap.get(exit);
  f->region = region->copy(nmap);
  f->ess.copy(ess);
  f->ast = ast ? ast->copy(&nmap) : 0;
  return f;
}

