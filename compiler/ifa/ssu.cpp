/*

  Static Single Use Form 
    - from the Illinois Concert Compiler circa 1992
    - new implementation circa 2003
  
  References
  "The Program Structure Tree: Computing Control Regions in Linear Time"
    PLDI'94 by Johnson, Pearson, Pingali
*/

#include "geysa.h"
#include "ssu.h"
#include "region.h"
#include "fun.h"
#include "pnode.h"
#include "var.h"
#include "if1.h"

static void
print_classes_nodes(Vec<CQClass*> &cq_classes) {
  forv_CQClass(c, cq_classes) {
    printf("Node Class\n");
    forv_PNode(n, c->nodes)
      if1_dump_code(stdout, n->code, 2);
  }
}

static char *SPACES = "                                        ";
#define SP(_fp, _n) fputs(&SPACES[40-(_n)], _fp)

static void
print_regions_nodes(Region *r, int indent) {
  SP(stdout, indent); printf("Region\n");
  forv_PNode(n, r->nodes)
    if1_dump_code(stdout, n->code, indent + 3);
  forv_Region(rr, r->children)
    print_regions_nodes(rr, indent + 2);
}

static void
print_classes_edges(Vec<CQClass *> &v) {
  forv_CQClass(c, v) {
    printf("Edge Class\n");
    forv_UEdge(e, c->edges) {
      if (e->from->code) 
        if1_dump_code(stdout, e->from->code, 2);
      else printf("FAKE\n");
      printf("->"); 
      if (e->to->code) 
        if1_dump_code(stdout, e->to->code, 0);
      else printf("FAKE\n");
    }
  }
}

static void
print_pnode(PNode *n, char *s) {
  printf("  %s: used %d RVALS ", s, n->used);
  forv_Var(v, n->rvals) {
    printf("%p:", v);
    if1_dump_sym(stdout, v->sym);
  }
  printf(" LVALS ");
  forv_Var(v, n->lvals) {
    printf("%p:", v);
    if (v)
      if1_dump_sym(stdout, v->sym);
    else
      printf("NULL");
  }
  printf("\n");
}

static void
print_renamed(Fun *f) {
  Vec<PNode *> nodes;
  f->collect_PNodes(nodes);
  forv_PNode(n, nodes) {
    forv_PNode(p, n->phi) print_pnode(p, "phi");
    if1_dump_code(stdout, n->code, 0);
    if (n->rvals.n || n->lvals.n)
      print_pnode(n, "node");
    forv_PNode(p, n->phy) print_pnode(p, "phy");
  }
}

static void
print_ssu(Fun *f, Vec<CQClass*> cq_classes, Region *region, Vec<PNode *> nodes) {
  if (verbose_level > 2) {
    printf("%d cq classes\n", cq_classes.n);
    int phi = 0, phy = 0;
    forv_PNode(n, nodes) {
      phi += n->phi.n;
      phy += n->phy.n;
    }
    printf("%d phi nodes\n", phi);
    printf("%d phy nodes\n", phi);
  }
  if (verbose_level > 3) {
    print_classes_nodes(cq_classes);
    print_classes_edges(cq_classes);
    print_regions_nodes(region, 0);
    print_renamed(f);
  }
}

inline int 
UNode::p() { 
  return this == &pn->unode[1]; 
}

inline UNode *
UEdge::other(UNode *e) {
  if (to == from) {
    if (e == &to->unode[1])
      return &from->unode[0];
    else
      return &to->unode[1];
  } else { 
    int p = !e->p();
    if (e->pn == to)
      return &from->unode[p];
    else
      return &to->unode[p];
  }
}

// build DFS tree, collect and number nodes, record backedges
static void
undirected_dfs(PNode *n, int p, Vec<UNode *> &v, int &mark) {
  UNode *u = &n->unode[p], *uu = &n->unode[!p];
  u->dfs_num = mark++;
  v.add(u);
  if (uu->dfs_num < 0) {
    UEdge *e = new UEdge(n, n);
    u->dfs_children.push(e);
    uu->dfs_parent = e;
    undirected_dfs(n, !p, v, mark);
  }
  forv_UEdge(e, u->edges) {
    UNode *o = e->other(u);
    if (o->dfs_num < 0) {
      u->dfs_children.push(e);
      o->dfs_parent = e;
      undirected_dfs(e->other(u->pn), o->p(), v, mark);
    } else if (e != u->dfs_parent && e != uu->dfs_parent)
      u->backedges.push(e);
  }
}

// compute cycle equivalance classes
static void 
cycle_equiv(Vec<UNode *> &nodes) {
  forv_UNode(n, nodes) { 
    UNode* hi0 = NULL, *hi1 = NULL, *hi2 = NULL;
    forl_UEdge(e, n->backedges) {
      UNode *p = e->other(n);
      assert(p != n);
      if (!hi0 || p->dfs_num < hi0->dfs_num)
        hi0 = p;
    }
    forl_UEdge(e, n->dfs_children) {
      UNode *p = e->other(n);
      assert(p != n);
      if (!hi1 || p->hi->dfs_num < hi1->dfs_num) {
        if (!hi2 || hi1->dfs_num < hi2->dfs_num)
          hi2 = hi1;
        hi1 = p->hi;
      } else if (!hi2 || p->hi->dfs_num < hi2->dfs_num)
        hi2 = p->hi;
    }
    n->hi = !hi0 ? hi1 : !hi1 ? hi0 : hi0->dfs_num < hi1->dfs_num ? hi0 : hi1;
    forl_UEdge(e, n->dfs_children)
      n->blist.append(e->other(n)->blist);
    forl_UEdge(e, n->capping)
      n->blist.remove(e);
    forl_UEdge(e, n->backedges)
      if (e->other(n)->dfs_num > n->dfs_num) {
        n->blist.remove(e);
        if (!e->cq_class)
          e->cq_class = new CQClass();
      }
    forl_UEdge(e, n->backedges)
      if (e->other(n)->dfs_num <= n->dfs_num)
        n->blist.push(e);
    if (hi2 && (!hi0 || hi0->dfs_num > hi2->dfs_num)) {
      UEdge *c = new UEdge(n->pn, hi2->pn);
      hi2->capping.push(c);
      n->blist.push(c);
    }
    if (n->dfs_parent) {
      UEdge *t = n->blist.head;
      if (t->recent_size != n->blist.size) {
        t->recent_size = n->blist.size;
        t->recent_class = new CQClass();
      }
      n->dfs_parent->cq_class = t->recent_class;
      if (t->recent_size == 1)
        t->cq_class = n->dfs_parent->cq_class;
    }
  }
}

// order nodes and edges within cyclic equivalence classes by DFS on CFG
static void
order_cq(UEdge *e, Vec<CQClass *> &cq_classes) {
  cq_classes.set_add(e->cq_class);
  e->cq_class->edges.add(e);
  PNode *n = e->to;
  if (n->mark)
    return;
  n->mark = 1;
  UEdge *rep_edge = n->unode[0].dfs_parent;
  if (!rep_edge || (rep_edge->to != rep_edge->from))
    rep_edge = n->unode[1].dfs_parent;
  rep_edge->cq_class->nodes.add(n);
  for (int i = 0; i < n->cfg_succ.n; i++)
    order_cq(n->unode[1].edges.v[i], cq_classes);
}

// driver for building cycle equivalence classes
static void
build_cycle_equiv(Fun *f, UEdge *ee, Vec<CQClass*> &cq_classes) {
  Vec<UNode *> nodes;
  int mark = 0;
  undirected_dfs(f->entry, 0, nodes, mark);
  nodes.reverse();      // reverse DFS order
  cycle_equiv(nodes);
  for (int i = 0; i < f->entry->cfg_succ.n; i++)
    order_cq(f->entry->unode[1].edges.v[i], cq_classes);
  forv_UNode(n, nodes)
    n->pn->mark = 0;
  cq_classes.set_to_vec();
}

// compute control region nesting
static void 
nest_regions(PNode *n, Region *r) {
  if (!n->mark) {
    n->mark = 1;
    r->nodes.add(n);
    n->region = r;
    for (int i = 0; i < n->cfg_succ.n; i++) {
      UEdge *e = n->unode[1].edges.v[i];
      Region *r1 = e->entry, *r2 = e->exit, *rn = r;
      if (r == r1 || r == r2)
        rn = r->parent;
      if (r1 && r1 != r && r1 != rn) {
        r1->parent = rn;
        r1->depth = rn->depth + 1;
        rn->children.add(r1);
        rn = r1;
      }
      if (r2 && r2 != r && r2 != rn) {
        r2->parent = rn;
        r2->depth = rn->depth + 1;
        rn->children.add(r2);
        rn = r2;
      }
      nest_regions(e->to, rn);
    }
  }
}

static void
build_regions(Fun *f, Vec<PNode *> &nodes, Vec<CQClass*> &cq_classes) {
  f->region = new Region();
  forv_CQClass(c, cq_classes) {
    if (c->edges.n > 1) {
      UEdge *end = c->edges.v[c->edges.n - 1];
      Region *r = new Region(c->edges.v[0], end);
      c->edges.v[0]->entry = r;
      end->exit = r;
    }
  }
  nest_regions(f->entry, f->region);
  forv_PNode(n, nodes) {
    n->mark = 0;
    n->unode = NULL;
  }
}

static UEdge *
build_undirected_graph(Fun *f, Vec<PNode *> &nodes) {
  f->collect_PNodes(nodes);
  forv_PNode(n, nodes) {
    n->unode = new UNode[2];
    n->unode[0].pn = n;
    n->unode[1].pn = n;
    n->mark = 0;
  }
  forv_PNode(n, nodes) {
    forv_PNode(nn, n->cfg_pred) {
      UEdge *e = new UEdge(nn, n);
      n->unode[0].edges.add(e);
      nn->unode[1].edges.add(e);
    }
  }
  UEdge *ee = new UEdge(f->exit, f->entry);
  f->exit->unode[1].edges.add(ee);
  f->entry->unode[0].edges.add(ee);
  return ee;
}

static inline int
maybe_live(PNode *n, Var *v) {
  return n->live_vars->get(v) != 0;
}

static int
merge_live(PNode *n, PNode *p) {
  int changed = 0;
  forv_Var(v, *p->live_vars) if (v)
    if (!n->lvals_set.set_in(v))
      changed = !n->live_vars->put(v);
  return changed;
}

static void
approximate_liveness(Fun *f, Vec<PNode *> &nodes) {
  int changed = 1;
  forv_PNode(n, nodes) {
    n->live_vars = new BlockHash<Var *, PointerHashFns>;
    forv_Var(v, n->lvals)
      if (Var_is_local(v, f))
        n->lvals_set.set_add(v);
  }
  while (changed) {
    changed = 0;
    forv_PNode(n, nodes) {
      forv_PNode(p, n->cfg_succ)
        changed = merge_live(n, p) || changed;
      forv_Var(v, n->rvals)
        if (Var_is_local(v, f))
          changed = !n->live_vars->put(v) || changed;
    }
  }
}

static inline PNode *
new_phiphi(PNode *n, Var *v, int phy) {
  PNode *p = new PNode();
  p->rvals.add(v);
  if (phy)
    p->lvals.fill(n->cfg_succ.n);
  else {
    p->lvals.fill(1);
    p->rvals.fill(n->cfg_pred.n);
  }
  return p;
}

static int
place_one(Region *r, Var *v, int phy) {
  int placed = 0;
  forv_Region(rr, r->children) {
    placed = place_one(rr, v, phy) || placed;
    forv_PNode(n, rr->nodes) {
      if (!phy && n->lvals.in(v))
        placed = true;
      if (phy && n->rvals.in(v))
        placed = true;
    }
  }
  if (placed) {
    forv_PNode(n, r->nodes) {
      if (maybe_live(n, v)) {
        if (!phy && n->cfg_pred.n > 1)
          n->phi.add(new_phiphi(n, v, phy));
        if (phy && n->cfg_succ.n > 1)
          n->phy.add(new_phiphi(n, v, phy));
      }
    }
  }
  return placed;
}

static void
place_phi_phy(Fun *f) {
  Vec<Var *> vars;
  f->collect_Vars(vars);
  Vec<PNode *> nodes;
  f->collect_PNodes(nodes);
  approximate_liveness(f, nodes);
  forv_Var(v, vars) {
    if (Var_is_local(v, f)) {
      place_one(f->region, v, 0);
      place_one(f->region, v, 1);
    }
  }
  forv_PNode(n, nodes)
    n->live_vars = 0;
}

typedef Env<Var *, Var *> VarEnv;
typedef Vec<PNode **> EdgeSet;

static inline Var *
new_Var(Var *v, VarEnv &e, Fun *f) {
  if (!Var_is_local(v, f))
    return v;
  Var *vv = new Var(v->sym);
  e.put(v, vv);
  return vv;
}

static inline Var *
get_Var(Var *v, VarEnv &env, Fun *f) {
  if (!Var_is_local(v, f))
    return v;
  Var *vv = env.get(v);
  if (vv)
    return vv;
  return v;
}

static void
rename_edge(Fun *f, PNode *d, VarEnv &env, Vec<PNode *> &nset) {
  forv_PNode(p, d->phi)
    p->lvals.v[0] = new_Var(p->rvals.v[0]->sym->var, env, f);
  for (int i = 0; i < d->rvals.n; i++)
    d->rvals.v[i] = get_Var(d->rvals.v[i]->sym->var, env, f);
  for (int i = 0; i < d->lvals.n; i++)
    d->lvals.v[i] = new_Var(d->lvals.v[i]->sym->var, env, f);
  forv_PNode(p, d->phy)
    p->rvals.v[0] = get_Var(p->rvals.v[0]->sym->var, env, f);
  for (int i = 0; i < d->cfg_succ.n; i++) {
    if (d->cfg_succ.n != 1 && d->cfg_pred.n != 1)
      env.push();
    PNode *dd = d->cfg_succ.v[i];
    int di = dd->cfg_pred_index.get(d);
    forv_PNode(p, d->phy)
      p->lvals.v[i] = new_Var(p->rvals.v[0]->sym->var, env, f);
    forv_PNode(p, dd->phi)
      p->rvals.v[di] = get_Var(p->rvals.v[0]->sym->var, env, f);
    if (nset.set_add(dd))
      rename_edge(f, dd, env, nset);
    if (d->cfg_succ.n != 1 && d->cfg_pred.n != 1)
      env.pop();
  }
}

static void
rename_vars(Fun *f, Vec<PNode *> nodes) {
  forv_PNode(n, nodes) 
    for (int i = 0; i < n->cfg_pred.n; i++)
      n->cfg_pred_index.put(n->cfg_pred.v[i], i);
  VarEnv env;
  env.push();
  Vec<PNode *> nset;
  nset.add(f->entry);
  rename_edge(f, f->entry, env, nset);
}

static void
mark_used(PNode *n, List<PNode *> &worklist) {
  n->used = 1;
  forv_Var(v, n->rvals)
    if (!v->mark) {
      v->mark = 1;
      if (v->def)
        worklist.push(v->def);
    }
}

static int
has_sideeffects(PNode *n, Sym *fn_scope) {
  if (n->code && n->code->kind == Code_SEND)
    return 1;
  forv_Var (v, n->lvals)
    if (v && v->sym->in && v->sym->in != fn_scope)
      return 1;
  return 0;
}

static void
eliminate_unused_code(Fun *f, Vec<PNode *> &base_nodes) {
  Vec<PNode *> nodes;
  Vec<Var *> vars;
  List<PNode *> worklist;
  f->collect_Vars(vars);
  forv_Var(v, vars)
    v->mark = 0;
  forv_PNode(n, base_nodes) {
    n->used = 0;
    forv_PNode(p, n->phi) p->used = 0;
    forv_PNode(p, n->phy) p->used = 0;
  }
  nodes.copy(base_nodes);
  // collect worklist and set variable definitions
  for (int i = 0; i < nodes.n; i++) {
    PNode *n = nodes.v[i];
    forv_PNode(p, n->phi) {
      forv_Var(v, p->lvals) if (v) v->def = p;
      nodes.add(p); 
    }
    forv_PNode(p, n->phy) { 
      forv_Var(v, p->lvals) if (v) v->def = p;
      nodes.add(p); 
    }
    forv_Var(v, n->lvals) if (v) v->def = n;
    if (has_sideeffects(n, f->sym)) {
      n->used = 1;
      worklist.push(n);
    }
  }
  while (PNode *n = worklist.pop())
    mark_used(n, worklist);
  // remove unused phi/phy
  forv_PNode(n, base_nodes) {
    Vec<PNode *> vv;
    if (n->phi.n) {
      vv.move(n->phi);
      forv_PNode(p, vv)
        if (p->used) 
          n->phi.add(p);
    }
    if (n->phy.n) {
      vv.move(n->phy);
      forv_PNode(p, vv)
        if (p->used) 
          n->phy.add(p);
    }
  }
}

void
Fun::build_ssu() {
  if (!entry)
    return;
  Vec<CQClass*> cq_classes;
  Vec<PNode *> nodes;
  UEdge *entry_edge = build_undirected_graph(this, nodes);
  build_cycle_equiv(this, entry_edge, cq_classes);
  build_regions(this, nodes, cq_classes);
  place_phi_phy(this);
  rename_vars(this, nodes);
  eliminate_unused_code(this, nodes);
  print_ssu(this, cq_classes, region, nodes);
}
