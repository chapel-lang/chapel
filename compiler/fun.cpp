/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "pattern.h"
#include "fun.h"
#include "pnode.h"
#include "if1.h"
#include "ast.h"

Fun::Fun(PDB *apdb, Sym *asym, int aninit_function) {
  pdb = apdb;
  init_function = aninit_function;
  clone_for_constants = 1;
  execution_frequency = 0.0;
  fa_collected = 0;
  has_return = 0;
  sym = asym;
  asym->fun = this;
  ast = sym->ast;
  nmap = 0;
  vmap = 0;
  size = -1;
  cg_string = 0;
  if (verbose_level > 2)
    if (asym->name)
      printf("function %s\n", asym->name);
    else
      printf("function %d\n", asym->id);
  build_cfg();
  build_ssu();
  setup_ast();
}

int
compar_funs(const void *ai, const void *aj) {
  int i = (*(Fun**)ai)->id;
  int j = (*(Fun**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

void
Fun::setup_ast() {
  Vec<PNode *> nodes;
  collect_PNodes(nodes);
  forv_PNode(n, nodes) {
    if (n->code && n->code->ast)
      n->code->ast->pnodes.add(n);
  }
  nodes.clear();
  ast->propagate(&nodes);
}

void
Fun::collect_PNodes(Vec<PNode *> &v) {
  Vec<PNode *> sv;
  if (!entry)
    return;
  v.add(exit);
  sv.set_add(exit);
  for (int i = 0; i < v.n; i++)
    forv_PNode(p, v.v[i]->cfg_pred)
      if (sv.set_add(p))
	v.add(p);
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
  forv_MPosition(p, arg_positions)
    vars.set_add(args.get(p));
  forv_Var(v, rets)
    vars.set_add(v);
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
  PNode *n = new PNode();
  n->code = node->code;
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
  for (int i = 0; i < n->tvals.n; i++)
    copy_var(&n->tvals.v[i], f->sym, vmap);
  return n;
}

Fun *
Fun::copy() {
  Fun *f = new Fun();
  f->pdb = pdb;
  f->sym = sym;
  f->init_function = init_function;
  f->nmap = new Map<PNode *, PNode*>;
  f->vmap = new VarMap;

  Vec<PNode *> nodes;

  forv_PNode(n, fa_all_PNodes) {
    PNode *p = copy_pnode(n, f, *f->vmap);
    nodes.add(p);
    f->nmap->put(n, p);
    for (int i = 0; i < n->phi.n; i++) {
      PNode *nn = n->phi.v[i];
      PNode *pp = copy_pnode(nn, f, *f->vmap);
      nodes.add(pp);
      f->nmap->put(nn, pp);
      p->phi.v[i] = pp;
    }
    for (int i = 0; i < n->phy.n; i++) {
      PNode *nn = n->phy.v[i];
      PNode *pp = copy_pnode(nn, f, *f->vmap);
      nodes.add(pp);
      f->nmap->put(nn, pp);
      p->phy.v[i] = pp;
    }
  }
  forv_PNode(n, nodes) {
    for (int i = 0; i < n->cfg_succ.n; i++)
      n->cfg_succ.v[i] = f->nmap->get(n->cfg_succ.v[i]);
    for (int i = 0; i < n->cfg_pred.n; i++)
      n->cfg_pred.v[i] = f->nmap->get(n->cfg_pred.v[i]);
  }
  f->arg_syms.copy(arg_syms);
  f->arg_positions.copy(arg_positions);
  forv_MPosition(p, f->arg_positions) {
    Var *v = args.get(p);
    copy_var(&v, f->sym, *f->vmap);
    f->args.put(p, v);
  }
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

void
Fun::calls_funs(Vec<Fun *> &calls_funs) {
  calls_funs.clear();
  Vec<Vec<Fun *> *> calls_funs_vecs;
  calls.get_values(calls_funs_vecs);
  for (int i = 0; i < calls_funs_vecs.n; i++)
    calls_funs.set_union(*calls_funs_vecs.v[i]);
  calls_funs.set_to_vec();
}

void
Fun::called_by_funs(Vec<Fun *> &called_by) {
  called_by.clear();
  forv_CallPoint(c, called)
    called_by.set_add(c->fun);
  called_by.set_to_vec();
}
