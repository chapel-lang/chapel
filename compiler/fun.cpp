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
  nmap = 0;
  vmap = 0;
  cg_string = 0;
  if (verbose_level > 1)
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
  forv_Var(v, n->tvals)
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

static void
copy_var(Var **av, Sym *fsym, VarMap &vmap) {
  Var *v = *av;
  if (v->sym->in == fsym) {
    if (!(v = vmap.get(*av))) {
      v = (*av)->copy();
      vmap.put(*av, v);
    }
  }
  *av = v;
}

static PNode *
copy_pnode(PNode *node, Fun *f, VarMap &vmap) {
  PNode *n = new PNode(node->code);
  n->rvals.copy(node->rvals);
  n->lvals.copy(node->lvals);
  n->tvals.copy(node->tvals);
  n->cfg_succ.copy(node->cfg_succ);
  n->cfg_pred.copy(node->cfg_pred);
  n->phi.copy(node->phi);
  n->phy.copy(node->phy);
  n->prim = node->prim;
  for (int i = 0; i < n->rvals.n; i++)
    copy_var(&n->rvals.v[i], f->sym, vmap);
  for (int i = 0; i < n->lvals.n; i++)
    copy_var(&n->lvals.v[i], f->sym, vmap);
  return n;
}

Fun *
Fun::copy() {
  Fun *f = new Fun();
  f->pdb = pdb;
  f->sym = sym;
  f->init_function = init_function;
  f->nmap = new PNodeMap;
  f->vmap = new VarMap;

  Vec<PNode *> nodes;

  forv_PNode(n, fa_all_PNodes) {
    PNode *p = copy_pnode(n, f, *f->vmap);
    nodes.add(p);
    f->nmap->put(n, p);
    for (int i = 0; i < n->phi.n; i++) {
      PNode *p = copy_pnode(n->phi.v[i], f, *f->vmap);
      nodes.add(p);
      f->nmap->put(n, p);
      p->phi.v[i] = p;
    }
    for (int i = 0; i < n->phy.n; i++) {
      PNode *p = copy_pnode(n->phy.v[i], f, *f->vmap);
      nodes.add(p);
      f->nmap->put(n, p);
      p->phy.v[i] = p;
    }
  }
  forv_PNode(n, nodes) {
    for (int i = 0; i < n->cfg_succ.n; i++)
      n->cfg_succ.v[i] = f->nmap->get(n->cfg_succ.v[i]);
    for (int i = 0; i < n->cfg_pred.n; i++)
      n->cfg_pred.v[i] = f->nmap->get(n->cfg_pred.v[i]);
  }
  f->args.copy(args);
  for (int i = 0; i < f->args.n; i++)
    copy_var(&f->args.v[i], f->sym, *f->vmap);
  f->rets.copy(rets);
  for (int i = 0; i < f->rets.n; i++)
    copy_var(&f->rets.v[i], f->sym, *f->vmap);
  for (int i = 0; i < f->vmap->n; i++)
    if (f->vmap->v[i].key)
      f->vmap->v[i].value->def = f->nmap->get(f->vmap->v[i].value->def);
  f->fa_all_PNodes.copy(fa_all_PNodes);
  for (int i = 0; i < f->fa_all_PNodes.n; i++)
    f->fa_all_PNodes.v[i] = f->nmap->get(f->fa_all_PNodes.v[i]);
  f->fa_all_Vars.copy(fa_all_Vars);
  for (int i = 0; i < f->fa_all_Vars.n; i++) {
    Var *v = f->vmap->get(f->fa_all_Vars.v[i]);
    if (v)
      f->fa_all_Vars.v[i] = v;
  }
  f->entry = f->nmap->get(entry);
  f->exit = f->nmap->get(exit);
  f->region = region->copy(*f->nmap);
  f->ess.copy(ess);
  f->ast = ast ? ast->copy(f->nmap) : 0;
  return f;
}

