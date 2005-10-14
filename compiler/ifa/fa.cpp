#include "geysa.h"
#include "pattern.h"
#include "driver.h"
#include "prim.h"
#include "if1.h"
#include "builtin.h"
#include "pdb.h"
#include "fun.h"
#include "pnode.h"
#include "fa.h"
#include "ast.h"
#include "var.h"
#include "clone.h"
#include "graph.h"


#define CACHE_CALLEES           1

static int avar_id = 1;
static int creation_set_id = 1;
static int entry_set_id = 1;
int analysis_pass = 0;

static FA *fa = 0;

AType *bottom_type = 0;
AType *void_type = 0;
AType *null_type = 0;
AType *unknown_type = 0;
AType *top_type = 0;
AType *any_type = 0;
AType *bool_type = 0;
AType *size_type = 0;
AType *anyint_type = 0;
AType *anynum_kind = 0;
AType *fun_type = 0;
AType *symbol_type = 0;
AType *string_type = 0;
AType *fun_symbol_type = 0;
AType *anyclass_type = 0;

static ChainHash<AType *, ATypeChainHashFns> cannonical_atypes;
static ChainHash<Setters *, SettersHashFns> cannonical_setters;
static ChainHash<SettersClasses *, SettersClassesHashFns> cannonical_setters_classes;
static ChainHash<ATypeFold *, ATypeFoldChainHashFns> type_fold_cache;
static ChainHash<ATypeViolation *, ATypeViolationHashFuns> type_violation_hash;

static Var *element_var = 0;

static Que(AEdge, edge_worklist_link) edge_worklist;
static Que(AVar, send_worklist_link) send_worklist;
static Vec<EntrySet *> entry_set_done;
static Vec<ATypeViolation *> type_violations;

static int application(PNode *p, EntrySet *es, AVar *fun, CreationSet *s, Vec<AVar *> &args, 
                       Partial_kind partial);

static int 
get_avar_id() {
  return avar_id++;
}

AVar::AVar(Var *v, void *acontour) : 
  var(v), contour(acontour), lvalue(0), gen(0), in(bottom_type), out(bottom_type), 
  restrict(0), container(0), setters(0), setter_class(0), mark_map(0),
  creation_set(0), type(0), ivar_offset(0), in_send_worklist(0), contour_is_entry_set(0), 
  is_lvalue(0), is_dead(0)
{
  id = get_avar_id();
}

AType::AType(AType &a) {
  hash = 0;
  this->copy(a);
}

AType::AType(CreationSet *cs) {
  hash = 0;
  set_add(cs);
}

AVar *
unique_AVar(Var *v, void *contour) {
  assert(contour);
  AVar *av = v->avars.get(contour);
  if (av)
    return av;
  av = new AVar(v, contour);
  v->avars.put(contour, av);
  return av;
}

AVar *
unique_AVar(Var *v, EntrySet *es) {
  assert(es);
  AVar *av = v->avars.get(es);
  if (av)
    return av;
  av = new AVar(v, es);
  v->avars.put(es, av);
  av->contour_is_entry_set = 1;
  if (v->sym->is_lvalue) {
    av->lvalue = new AVar(v, es);
    av->lvalue->is_lvalue = 1;
    av->lvalue->contour_is_entry_set = 1;
  }
  return av;
}

CreationSet::CreationSet(Sym *s) : sym(s), clone_for_constants(0), added_element_var(0), atype(0), equiv(0) { 
  id = creation_set_id++;
}

CreationSet::CreationSet(CreationSet *cs) :
  added_element_var(0)
{
  sym = cs->sym;
  id = creation_set_id++;
  clone_for_constants = cs->clone_for_constants;
  atype = NULL;
  forv_AVar(v, cs->vars) {
    AVar *iv = unique_AVar(v->var, this);
    add_var_constraint(iv);
    vars.add(iv);
    if (iv->var->sym->name)
      var_map.put(iv->var->sym->name, iv);
  }
  equiv = 0;
  type = 0;
  sym->creators.add(this);
}

EntrySet::EntrySet(Fun *af) : fun(af), equiv(0) {
  id = entry_set_id++;
}

int
compar_creation_sets(const void *ai, const void *aj) {
  int i = (*(CreationSet**)ai)->id;
  int j = (*(CreationSet**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

int 
compar_avars(const void *ai, const void *aj) {
  int i = (*(AVar**)ai)->id;
  int j = (*(AVar**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

int 
compar_entry_sets(const void *ai, const void *aj) {
  int i = (*(EntrySet**)ai)->id;
  int j = (*(EntrySet**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

AVar *
make_AVar(Var *v, EntrySet *es) {
  if (v->sym->function_scope || v->is_internal) {
    if (es->fun && es->fun->sym->nesting_depth && v->sym->nesting_depth &&
        v->sym->nesting_depth != es->fun->sym->nesting_depth + 1) {
      assert(es->fun->sym->nesting_depth >= v->sym->nesting_depth);
      return unique_AVar(v, es->display.v[v->sym->nesting_depth-1]);
    }
    return unique_AVar(v, es);
  }
  assert(v->sym->global_scope);
  return unique_AVar(v, GLOBAL_CONTOUR);
}

static inline AVar *
make_AVar(Var *v, AEdge *e) { 
  return make_AVar(v, e->to); 
}

AType *
make_AType(CreationSet *cs) {
  if (cs->atype)
    return cs->atype;
  return cs->atype = type_cannonicalize(new AType(cs));
}

AType *
make_abstract_type(Sym *s) {
  s = unalias_type(s);
  AType *a = s->abstract_type;
  if (a)
    return a;
  CreationSet *cs = new CreationSet(s);
  return s->abstract_type = make_AType(cs);
}

AType *
make_AType(Vec<CreationSet *> &css) {
  AType *t = new AType();
  t->set_union(css);
  return type_cannonicalize(t);
}

AType *
AType::constants() {
  AType *t = new AType();
  forv_CreationSet(cs, this->sorted)
    if (cs->sym->constant)
      t->set_add(cs);
  return type_cannonicalize(t);
}

void
update_in(AVar *v, AType *t) {
  AType *tt = type_union(v->in, t);
  if (tt != v->in) {
    assert(tt && tt != top_type);
    v->in = tt;
    if (v->restrict)
      tt = type_intersection(v->in, v->restrict);
    if (tt != v->out) {
      assert(tt != top_type);
      v->out = tt;
      forv_AVar(vv, v->arg_of_send.asvec) {
        if (!vv->in_send_worklist) {
          vv->in_send_worklist = 1;
          send_worklist.enqueue(vv);
        }
      }
      forv_AVar(vv, v->forward) if (vv)
        update_in(vv, tt);
    }
  }
}

void
update_gen(AVar *v, AType *t) {
  if (v->gen) {
    AType *tt = type_union(v->gen, t);
    if (tt == v->gen)
      return;
    v->gen = tt;
  } else
    v->gen = t;
  update_in(v, v->gen);
}

static void
flow_var_to_var(AVar *a, AVar *b) {
  if (a == b)
    return;
  if (a->forward.in(b))
    return;
  a->forward.set_add(b);
  b->backward.set_add(a);
  update_in(b, a->out);
}

void
flow_vars(AVar *v, AVar *vv) {
  if (v->lvalue) {
    if (vv->lvalue) {
      flow_var_to_var(v, vv);
      flow_var_to_var(vv->lvalue, v->lvalue);
    } else {
      flow_var_to_var(v, vv);
      flow_var_to_var(vv, v->lvalue);
    }
  } else {
    if (vv->lvalue) {
      flow_var_to_var(v, vv);
      flow_var_to_var(vv->lvalue, v);
    } else
      flow_var_to_var(v, vv);
  }
}

void
flow_vars_assign(AVar *rhs, AVar *lhs) {
  flow_var_to_var(rhs, lhs);
  if (lhs->lvalue)
    flow_var_to_var(rhs, lhs->lvalue);
}


// Initially create a unique creation set for each
// variable (location in program text).
CreationSet *
creation_point(AVar *v, Sym *s) {
  CreationSet *cs = v->creation_set;
  if (cs) {
    assert(cs->sym == s);
    goto Ldone;
  }
  forv_CreationSet(x, s->creators) {
    cs = x;
    if (v->contour == GLOBAL_CONTOUR) {
      if (cs->defs.n)
        goto LnextCreationSet;
    } else {
      if (!cs->defs.n)
        goto LnextCreationSet;
      forv_AVar(av, cs->defs)
        if (av->var != v->var)
          goto LnextCreationSet;
    }
    goto Lfound;
  LnextCreationSet:;
  }
  // new creation set
  cs = new CreationSet(s);
  s->creators.add(cs);
  forv_Sym(h, s->has) {
    assert(h->var);
    AVar *iv = unique_AVar(h->var, cs);
    add_var_constraint(iv);
    cs->vars.add(iv);
    if (h->name)
      cs->var_map.put(h->name, iv);
  }
 Lfound:
  cs->defs.add(v);
  v->creation_set = cs;
 Ldone:
  if (v->contour_is_entry_set)
    ((EntrySet*)v->contour)->creates.set_add(cs);
  update_gen(v, make_AType(cs));
  return cs;
}

//  all float combos become doubles
//  all signed/unsigned combos become signed
//  all int combos below 32 bits become signed 32 bits, above become signed 64 bits
Sym *
coerce_num(Sym *a, Sym *b) {
  if (a->num_kind == b->num_kind) {
    if (a->num_index > b->num_index)
      return a;
    else
      return b;
  }
  if (b->num_kind == IF1_NUM_KIND_FLOAT) {
    Sym *t = b; b = a; a = t;
  }
  if (a->num_kind == IF1_NUM_KIND_FLOAT) {
    if (int_type_precision[b->num_kind] <= float_type_precision[a->num_kind])
      return a;
    if (int_type_precision[b->num_kind] >= 32)
      return sym_float32;
    return sym_float64;
  }
  // mixed signed and unsigned
  if (a->num_index >= IF1_INT_TYPE_64 || b->num_index >= IF1_INT_TYPE_64)
    return sym_int64;
  else if (a->num_index >= IF1_INT_TYPE_32 || b->num_index >= IF1_INT_TYPE_32)
    return sym_int32;
  else if (a->num_index >= IF1_INT_TYPE_16 || b->num_index >= IF1_INT_TYPE_16)
    return sym_int16;
  else if (a->num_index >= IF1_INT_TYPE_8 || b->num_index >= IF1_INT_TYPE_8)
    return sym_int8;
  return sym_bool;
}

AType *
type_num_fold(Prim *p, AType *a, AType *b) {
  (void) p; p = 0; // for now
  a = type_intersection(a, anynum_kind);
  b = type_intersection(b, anynum_kind);
  ATypeFold f(p, a, b), *ff;
  if ((ff = type_fold_cache.get(&f)))
    return ff->result;
  AType *r = new AType();
  forv_CreationSet(acs, a->sorted) {
    Sym *atype = acs->sym->type;
    forv_CreationSet(bcs, b->sorted) {
      Sym *btype = bcs->sym->type;
      r->set_add(coerce_num(atype, btype)->abstract_type->v[0]);
    }
  }
  r = type_cannonicalize(r);
  type_fold_cache.put(new ATypeFold(p, a, b, r));
  return r;
}

template<class C> void
qsort_by_id(C **left, C **right) {
 Lagain:
  if (right - left < 5) {
    for (C **y = right - 1; y > left; y--) {
      for (C **x = left; x < y; x++) {
        if (x[0]->id > x[1]->id) {
          C *t = x[0];
          x[0] = x[1];
          x[1] = t;
        } 
      } 
    }
  } else {
    C  **i = left + 1, **j = right - 1;
    C *x = *left;
    for (;;) {
      while (x->id < (*j)->id) j--;
      while (i < j && (*i)->id < x->id) i++;
      if (i >= j) break;
      C *t = *i;
      *i = *j;
      *j = t;
      i++; j--;
    }
    if (j == right - 1) {
      *left = *(right - 1);
      *(right - 1) = x;
      right--;
      goto Lagain;
    }
    if (left < j) qsort_by_id(left, j + 1);
    if (j + 2 < right) qsort_by_id(j + 1, right);
  }
}

template<class C> void
qsort_by_id(Vec<C *> &v) {
  if (v.n > 1)
    qsort_by_id(&v.v[0], v.end());
}

void
qsort_pointers(void **left, void **right) {
 Lagain:
  if (right - left < 5) {
    for (void **y = right - 1; y > left; y--) {
      for (void **x = left; x < y; x++) {
        if (x[0] > x[1]) {
          void *t = x[0];
          x[0] = x[1];
          x[1] = t;
        } 
      } 
    }
  } else {
    void  **i = left + 1, **j = right - 1, *x = *left;
    for (;;) {
      while (x < *j) j--;
      while (i < j && *i < x) i++;
      if (i >= j) break;
      void *t = *i;
      *i = *j;
      *j = t;
      i++; j--;
    }
    if (j == right - 1) {
      *left = *(right - 1);
      *(right - 1) = x;
      right--;
      goto Lagain;
    }
    if (left < j) qsort_pointers(left, j + 1);
    if (j + 2 < right) qsort_pointers(j + 1, right);
  }
}

AType *
type_cannonicalize(AType *t) {
  assert(!t->sorted.n);
  assert(!t->union_map.n);
  assert(!t->intersection_map.n);
  int consts = 0, rebuild = 0, nulls = 0;
  Vec<CreationSet *> nonconsts;
  forv_CreationSet(cs, *t) if (cs) {
    // strip out constants if the base type is included
    CreationSet *base_cs = 0;
    if (cs->sym->is_constant || (cs->sym->type->num_kind && cs->sym != cs->sym->type))
      base_cs = cs->sym->type->abstract_type->v[0];
    else if (cs->sym->type_kind == Type_TAGGED)
      base_cs = cs->sym->type->specializes.v[0]->abstract_type->v[0];
    if (base_cs) {
      if (t->set_in(base_cs)) {
        rebuild = 1;
        continue;
      }
      consts++;
      nonconsts.set_add(base_cs);
    } else { 
      if (cs->sym != sym_null)
        nonconsts.set_add(cs);
      else
        nulls = 1;
    }
    t->sorted.add(cs);
  }
  if (consts > fa->num_constants_per_variable)
    rebuild = 1;
  if (rebuild) {
    t->sorted.clear();
    t->sorted.append(nonconsts);
    t->clear();
    t->set_union(t->sorted);
  }
  if (t->sorted.n > 1)
    qsort_by_id(t->sorted);
  unsigned int h = 0;
  for (int i = 0; i < t->sorted.n; i++)
    h = (uint)(intptr_t)t->sorted.v[i] * open_hash_multipliers[i % 256];
  t->hash = h ? h : h + 1; // 0 is empty
  AType *tt = cannonical_atypes.put(t);
  if (!tt) tt = t;
  // compute "type" (without constants)
  if (nonconsts.n) {
    if (nulls || consts)
      tt->type = make_AType(nonconsts);
    else
      tt->type = tt;
  } else
    tt->type = bottom_type;
  return tt;
}

AType *
type_union(AType *a, AType *b) {
  AType *r;
  if ((r = a->union_map.get(b)))
    return r;
  if (a == b || b == bottom_type) {
    r = a;
    goto Ldone;
  }
  if (a == bottom_type) {
    r = b;
    goto Ldone;
  }
  {
    AType *ab = type_diff(a, b);
    AType *ba = type_diff(b, a);
    r = new AType(*ab);
    forv_CreationSet(x, *ba)
      if (x)
        r->set_add(x);
    forv_CreationSet(x, *a)
      if (x && b->in(x))
        r->set_add(x);
    r = type_cannonicalize(r);
  }
 Ldone:
  a->union_map.put(b, r);
  return r;
}

static inline int
subsumed_by(Sym *a, Sym *b) {
  if (b->type_kind == Type_LUB || a->is_symbol)
    return b->specializers.set_in(a->type) != 0;
  else return a == b || a->type == b;
}

AType *
type_diff(AType *a, AType *b) {
  AType *r;
  if ((r = a->diff_map.get(b)))
    return r;
  if (b == bottom_type) {
    r = a;
    goto Ldone;
  }
  r = new AType();
  forv_CreationSet(aa, *a) if (aa) {
    if (aa->defs.n && b->set_in(aa))
      continue;
    forv_CreationSet(bb, *b) if (bb && !bb->defs.n) {
      if (subsumed_by(aa->sym, bb->sym))
        goto Lnext;
    }
    r->set_add(aa);
  Lnext:;
  }
  r = type_cannonicalize(r);
 Ldone:
  a->diff_map.put(b, r);
  return r;
}

AType *
type_intersection(AType *a, AType *b) {
  AType *r;
  if ((r = a->intersection_map.get(b)))
    return r;
  if (a == b || a == bottom_type || b == top_type) {
    r = a;
    goto Ldone;
  }
  if (a == top_type || b == bottom_type) {
    r = b;
    goto Ldone;
  }
  r = new AType();
  forv_CreationSet(aa, *a) if (aa) {
    forv_CreationSet(bb, *b) if (bb) {
      if (aa->defs.n) {
        if (bb->defs.n) {
          if (aa == bb) {
            r->set_add(aa);
            goto Lnexta;
          }
        } else {
          if (subsumed_by(aa->sym, bb->sym)) {
            r->set_add(aa);
            goto Lnexta;
          }
        }
      } else {
        if (bb->defs.n) {
          if (subsumed_by(bb->sym, aa->sym))
            r->set_add(bb);
        } else {
          if (subsumed_by(aa->sym, bb->sym)) {
            r->set_add(aa);
            goto Lnexta;
          } else if (subsumed_by(bb->sym, aa->sym))
            r->set_add(bb);
        }
      }
    }
  Lnexta:;
  }
  r = type_cannonicalize(r);
 Ldone:
  a->intersection_map.put(b, r);
  return r;
}

static void
fill_rets(EntrySet *es, int n) {
  es->fun->rets.fill(n);
  es->rets.fill(n);
  for (int i = 0; i < n; i++)
    if (!es->rets.v[i]) {
      if (!i)
        es->rets.v[i] = make_AVar(es->fun->sym->ret->var, es);
      else {
        if (!es->fun->rets.v[i]) {
          Var *v = new Var(es->fun->sym->ret);
          es->fun->rets.v[i] = v;
          es->fun->fa_all_Vars.add(v);
        }
        es->rets.v[i] = make_AVar(es->fun->rets.v[i], es);
      }
    }
}

static int 
same_eq_classes(Setters *s, Setters *ss) {
  if (s == ss)
    return 1;
  if (!s || !ss)
    return 0;
  Vec<Setters *> sc1, sc2;
  forv_AVar(av, *s) if (av) {
    assert(av->setter_class);
    sc1.set_add(av->setter_class);
  }
  forv_AVar(av, *ss) if (av) {
    assert(av->setter_class);
    sc2.set_add(av->setter_class);
  }
  if (sc1.some_disjunction(sc2))
    return 0;
  return 1;
}

static int
edge_nest_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  if (!es->fun->sym->nesting_depth)
    return 1;
  if (es->fun->sym->nesting_depth >= e->from->fun->sym->nesting_depth) { // call down or same
    if (es->display.n) 
      for (int i = 0; i < e->from->display.n; i++)
        if (es->display.v[i] != e->from->display.v[i])
          return 0;
  } else { // call up
    if (es->display.n)
      for (int i = 0; i < es->fun->sym->nesting_depth; i++)
        if (es->display.v[i] != e->from->display.v[i])
          return 0;
  }
  return 1;
}

static int 
different_marked_args(AVar *a1, AVar *a2, int offset, AVar *basis = 0) {
  Vec<void *> marks1, marks2;
  AVar *basis1 = basis ? basis : a2;
  int found1 = 0, found2 = 0;
  if (a1->mark_map) {
    form_Map(MarkElem, x, *a1->mark_map) {
      if (basis1->mark_map) {
        int m = basis1->mark_map->get(x->key);
        if (m) {
          found1 = 1;
          if (m - offset == x->value)
            marks1.set_add(x->key);
        }
      }
    }
  }
  if (a2->mark_map) {
    form_Map(MarkElem, x, *a2->mark_map) {
      if (basis) {
        if (basis->mark_map) {
          int m = basis->mark_map->get(x->key);
          if (m) {
            found2 = 1;
            if (m - offset == x->value)
              marks2.set_add(x->key);
          }
        }
      } else {
        found2 = 1;
        marks2.set_add(x->key);
      }
    }
  }
  return found1 && found2 && marks1.some_disjunction(marks2);
}

static int
edge_type_compatible_with_edge(AEdge *e, AEdge *ee, EntrySet *es, int fmark = 0) {
  assert(e->args.n && ee->args.n);
  forv_MPosition(p, e->match->fun->arg_positions) {
    AVar *e_arg = e->args.get(p); 
    AVar *ee_arg = ee->args.get(p);
    if (!e_arg || !ee_arg)
      continue;
    if (!fmark) {
      if (ee_arg->out->type->n && e_arg->out->type->n && ee_arg->out->type != e_arg->out->type)
        return 0;
    } else {
      AVar *es_arg = es->args.get(p);
      if (different_marked_args(ee_arg, e_arg, 2, es_arg))
        return 0;
    }
  }
  if (e->rets.n != ee->rets.n)
    return 0;
  for (int i = 0; i < e->rets.n; i++) {
    if (ee->rets.v[i]->lvalue && e->rets.v[i]->lvalue) {
      if (!fmark) {
        if (ee->rets.v[i]->lvalue->out->type->n && e->rets.v[i]->lvalue->out->type->n &&
            ee->rets.v[i]->lvalue->out->type != e->rets.v[i]->lvalue->out->type)
          return 0;
      } else {
        if (different_marked_args(ee->rets.v[i]->lvalue, e->rets.v[i]->lvalue, 1,
                                  es->rets.v[i]->lvalue))
          return 0;
      }
    }
  }
  return 1;
}

static int
edge_type_compatible_with_entry_set(AEdge *e, EntrySet *es, int fmark = 0) {
  assert(e->args.n && es->args.n);
  if (!es->split) {
    forv_MPosition(p, e->match->fun->positional_arg_positions) {
      AVar *es_arg = es->args.get(p), *e_arg = e->args.get(p);
      if (!e_arg)
        continue;
      if (!fmark) {
        if (e_arg->out->type->n && es_arg->out->type->n && e_arg->out->type != es_arg->out->type)
          return 0;
      } else
        if (different_marked_args(e_arg, es_arg, 2))
          return 0;
    }
    if (es->rets.n != e->rets.n)
      return 0;
    for (int i = 0; i < e->rets.n; i++) {
      if (es->rets.v[i]->lvalue && e->rets.v[i]->lvalue) {
        if (!fmark) {
          if (es->rets.v[i]->lvalue->out->type->n && e->rets.v[i]->lvalue->out->type->n &&
              es->rets.v[i]->lvalue->out->type != e->rets.v[i]->lvalue->out->type)
            return 0;
        } else
          if (different_marked_args(es->rets.v[i]->lvalue, e->rets.v[i]->lvalue, 1))
            return 0;
      }
    }
  } else {
    AEdge **last = es->edges.last();
    for (AEdge **pee = es->edges.first(); pee < last; pee++) if (*pee) {
      AEdge *ee = *pee;
      if (!ee->args.n)  
        continue;
      if (!edge_type_compatible_with_edge(e, ee, es, fmark))
        return 0;
    }
  }
  return 1;
}

static
int sset_compatible(AVar *av1, AVar *av2) {
  if (!same_eq_classes(av1->setters, av2->setters))
    return 0;
  if (av1->lvalue && av2->lvalue)
    if (!same_eq_classes(av1->lvalue->setters, av2->lvalue->setters))
      return 0;
  return 1;
}

static int
edge_sset_compatible_with_edge(AEdge *e, AEdge *ee) {
  assert(e->args.n && ee->args.n);
  forv_MPosition(p, e->match->fun->arg_positions) {
    AVar *eav = e->args.get(p), *eeav = ee->args.get(p);
    if (eav && eeav)
      if (!sset_compatible(eav, eeav))
        return 0;
  }
  if (e->rets.n != ee->rets.n)
    return 0;
  for (int i = 0; i < e->rets.n; i++)
    if (!sset_compatible(e->rets.v[i], ee->rets.v[i]))
      return 0;
  return 1;
}

static int
edge_sset_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  assert(e->args.n && es->args.n);
  if (!es->split) {
    forv_MPosition(p, e->match->fun->positional_arg_positions) {
      AVar *av = e->args.get(p);
      if (av)
        if (!sset_compatible(av, es->args.get(p)))
          return 0;
    }
    if (es->rets.n != e->rets.n)
      return 0;
    for (int i = 0; i < es->rets.n; i++)
      if (!sset_compatible(e->rets.v[i], es->rets.v[i]))
        return 0;
  } else {
    AEdge **last = es->edges.last();
    for (AEdge **pee = es->edges.first(); pee < last; pee++) if (*pee) {
      AEdge *ee = *pee;
      if (!ee->args.n)  
        continue;
      if (!edge_sset_compatible_with_edge(e, ee))
        return 0;
    }
  }
  return 1;
}

static int
edge_constant_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  forv_MPosition(p, e->match->fun->positional_arg_positions) {
    AVar *av = es->args.get(p);
    if (av->var->sym->clone_for_constants) {
      AType css;
      av->out->set_disjunction(*e->args.get(p)->out, css);
      forv_CreationSet(cs, css)
        if (cs->sym->constant)
          return 0;
    }
  }
  return 1;
}

static int
entry_set_compatibility(AEdge *e, EntrySet *es) {
  int val = INT_MAX;
  if (e->match->fun->split_unique)
    return 0;
  if (!edge_nest_compatible_with_entry_set(e, es))
    return 0;
  switch (edge_type_compatible_with_entry_set(e, es)) {
    case 1: break;
    case 0: val -= 4; break;
    case -1: return 0;
  }
  if (!edge_sset_compatible_with_entry_set(e, es))
    val -= 2;
  if (e->match->fun->clone_for_constants)
    if (!edge_constant_compatible_with_entry_set(e, es))
      val -= 1;
  return val;
}

static void
update_display(AEdge *e, EntrySet *es) {
  if (es->fun->sym->nesting_depth > e->from->fun->sym->nesting_depth) { // call down
    if (es->display.n == 0)
      es->display.append(e->from->display);
    else
      for (int i = 0; i < e->from->display.n; i++)
        assert(es->display.v[i] == e->from->display.v[i]);
    es->display.add(e->from);
  } else if (es->fun->sym->nesting_depth == e->from->fun->sym->nesting_depth) { // same level
    if (es->display.n == 0)
      es->display.append(e->from->display);
    else
      for (int i = 0; i < e->from->display.n; i++)
        assert(es->display.v[i] == e->from->display.v[i]);
  } else { // call up
    if (es->display.n == 0)
      for (int i = 0; i < es->fun->sym->nesting_depth; i++)
        es->display.add(e->from->display.v[i]);
    else
      for (int i = 0; i < es->fun->sym->nesting_depth; i++)
        assert(es->display.v[i] == e->from->display.v[i]);
  }
}

static void
set_entry_set(AEdge *e, EntrySet *es = 0) {
  EntrySet *new_es = es;
  if (!es) {
    new_es = new EntrySet(e->match->fun);
    e->match->fun->ess.add(new_es);
  }
  e->to = new_es;
  new_es->edges.put(e);
  if (new_es->fun->sym->nesting_depth)
    update_display(e, new_es);
  if (!es) {
    forv_MPosition(p, e->match->fun->positional_arg_positions) {
      Var *v = e->match->fun->args.get(p);
      AVar *av = make_AVar(v, new_es);
      new_es->args.put(p, av);
    }
  }
  fill_rets(new_es, e->pnode->lvals.n);
}

static EntrySet *
find_best_entry_set(AEdge *e, EntrySet *split) {
  EntrySet *es = NULL;
  int val = -1;
  forv_EntrySet(x, e->match->fun->ess) {
    int v = entry_set_compatibility(e, x);
    if (v > 0 && v > val) {
      es = x;
      if (v == INT_MAX)
        return es;
      val = v;
    }
  }
  if (split)
    es = 0;
  return es;
}

static int
check_split(AEdge *e)  {
  if (!e->from)
    return 0;
  if (EntrySet *es = e->from->pending_es_backedge_map.get(e)) {
    set_entry_set(e, es);
    return 1;
  }
  if (e->from->split) {
    Map<Fun *, AEdge *> *m = e->from->split->out_edge_map.get(e->pnode);
    if (m) {
      for (int i = 0; i < m->n; i++)
        if (m->v[i].key == e->match->fun) {
          if (e->match->fun->split_unique) {
            set_entry_set(e);
          } else if (e->match->fun->nested_in == e->from->fun) {
            set_entry_set(e);
            e->to->split = m->v[i].value->to;
          } else
            set_entry_set(e, m->v[i].value->to);
          return 1;
        }
    }
  }
  return 0;
}

// check results of last session read from disk
static int
check_es_db(AEdge *e) {
  return 0;
}

static void 
make_entry_set(AEdge *e, EntrySet *split = 0, EntrySet *preference = 0) {
  if (e->to) return;
  if (check_split(e)) return;
  if (check_es_db(e)) return;
  EntrySet *es = 0;
  if (!split)
    es = find_best_entry_set(e, split);
  if (!es)
    es = preference;
  set_entry_set(e, es);
  if (!es)
    e->to->split = split;
}

void
flow_var_type_permit(AVar *v, AType *t) {
  if (!v->restrict)
    v->restrict = t;
  else
    v->restrict = type_union(t, v->restrict);
  AType *tt = type_intersection(v->in, v->restrict);
  if (tt != v->out) {
    assert(tt != top_type);
    v->out = tt;
    forv_AVar(vv, v->arg_of_send.asvec) {
      if (!vv->in_send_worklist) {
        vv->in_send_worklist = 1;
        send_worklist.enqueue(vv);
      }
    }
    forv_AVar(vv, v->forward) if (vv)
      update_in(vv, v->out);
  }
}

static inline void
flow_var_type_permit(AVar *v, Sym *s) { 
  flow_var_type_permit(v, make_abstract_type(s)); 
}

void
add_var_constraint(AVar *av) {
  Sym *s = av->var->sym;
  assert(s->type_kind != Type_VARIABLE);
  if (s->type && !s->is_pattern) {
    if (s->is_external && 
        (s->type->num_kind || s->type == sym_string || s == sym_nil))
      update_gen(av, s->type->abstract_type);
    if (s->is_constant) // for constants, the abstract type is the concrete type
      update_gen(av, make_abstract_type(s));
    if (s->is_symbol || s->is_fun) 
      update_gen(av, make_abstract_type(s));
    if (s->type_kind != Type_NONE)
      update_gen(av, make_abstract_type(s->meta_type));
  }
}

static void
add_var_constraints(EntrySet *es) {
  forv_Var(v, es->fun->fa_Vars)
    add_var_constraint(make_AVar(v, es));
}

void
set_container(AVar *av, AVar *container) {
  assert(!av->container || av->container == container);
  av->container = container;
  if (av->lvalue)
    av->lvalue->container = container;
}

void
fill_tvals(Fun *fn, PNode *p, int n) {
  p->tvals.fill(n);
  for (int i = 0; i < n; i++) {
    if (!p->tvals.v[i]) {
      Sym *s = new_Sym();
      s->function_scope = 1;
      s->in = fn->sym;
      p->tvals.v[i] = new Var(s);
      p->tvals.v[i]->is_internal = 1;
      s->var = p->tvals.v[i];
      fn->fa_all_Vars.add(p->tvals.v[i]);
    }
  }
}

void
prim_make(PNode *p, EntrySet *es, Sym *kind, int start, int ref) {
  AVar *container = make_AVar(p->lvals.v[0], es);
  CreationSet *cs = creation_point(container, kind);
  cs->vars.fill(p->rvals.n - start);
  int l = p->rvals.n - start;
  p->tvals.fill(l);
  for (int i = 0; i < l; i++) {
    Var *v = p->rvals.v[start + i];
    AVar *av = make_AVar(v, es);
    if (!p->tvals.v[i]) {
      Sym *s = new_Sym();
      s->function_scope = 1;
      s->is_lvalue = v->sym->is_lvalue;
      s->in = es->fun->sym;
      p->tvals.v[i] = new Var(s);
      s->var = p->tvals.v[i];
      es->fun->fa_all_Vars.add(p->tvals.v[i]);
    }
    Var *tv = p->tvals.v[i];
    if (!cs->vars.v[i])
      cs->vars.v[i] = unique_AVar(v, cs);
    AVar *iv = cs->vars.v[i];
    AVar *atv = make_AVar(tv, es);
    set_container(atv, container);
    flow_vars(av, atv);
    flow_vars(atv, iv);
    if (iv->var->sym->name)
      cs->var_map.put(iv->var->sym->name, iv);
  }
}

AVar *
get_element_avar(CreationSet *cs) {
  AVar *elem = unique_AVar(cs->sym->element->var, cs);
  cs->added_element_var = 1;
  return elem;
}

static void
prim_make_list(PNode *p, EntrySet *es) {
  AVar *container = make_AVar(p->lvals.v[0], es);
  CreationSet *cs = creation_point(container, sym_list);
  AVar *elem = get_element_avar(cs);
  for (int i = 0; i < p->rvals.n - 1; i++) {
    AVar *av = make_AVar(p->rvals.v[1 + i], es);
    set_container(av, container);
    flow_vars(av, elem);
  }
}

static void
vector_elems(int rank, PNode *p, AVar *ae, AVar *elem, AVar *container, int n = 0) {
  AVar *e = ae;
  if (!e->contour_is_entry_set) {
    p->tvals.fill(++n);
    assert(container->contour_is_entry_set);
    EntrySet *es = (EntrySet*)container->contour;
    if (p->tvals.v[n-1])
      e = make_AVar(p->tvals.v[n-1], es);
    else {
      Sym *s = new_Sym();
      s->function_scope = 1;
      assert(!e->var->sym->is_lvalue);
      s->in = es->fun->sym;
      Var *v = new Var(s);
      s->var = v;
      p->tvals.v[n-1] = v;
      es->fun->fa_all_Vars.add(v);
      e = make_AVar(v, es);
    }
    flow_vars(ae, e);
  }
  set_container(e, container);
  if (rank > 0) {
    forv_CreationSet(cs, *e->out) if (cs) {
      if (cs->sym != sym_tuple)
        flow_vars(e, elem);
      else {
        e->arg_of_send.add(container);
        forv_AVar(av, cs->vars)
          vector_elems(rank - 1, p, av, elem, container, n+1);
      }
    }
  } else
    flow_vars(e, elem);
}

static void
prim_make_vector(PNode *p, EntrySet *es) {
  AVar *container = make_AVar(p->lvals.v[0], es);
  CreationSet *cs = creation_point(container, sym_vector);
  AVar *elem = get_element_avar(cs);
  if (p->rvals.n > 2) {
    int rank = 0;
    p->rvals.v[1]->sym->imm_int(&rank);
    for (int i = 0; i < p->rvals.n - 2; i++) {
      Var *v = p->rvals.v[2 + i];
      AVar *av = make_AVar(v, es);
      vector_elems(rank, p, av, elem, container);
    }
  }
}

static void
make_closure_var(Var *v, EntrySet *es, CreationSet *cs, AVar *result, int add, int i) {
  AVar *iv = unique_AVar(v, cs);
  AVar *av = make_AVar(v, es);
  PNode *pn = result->var->def;
  if (!pn->tvals.v[i]) {
    pn->tvals.v[i] = new Var(v->sym);
    pn->tvals.v[i]->is_internal = 1;
    es->fun->fa_all_Vars.add(pn->tvals.v[i]);
  }
  AVar *cav = make_AVar(pn->tvals.v[i], es);
  flow_vars(av, cav);
  set_container(cav, result);
  flow_var_to_var(cav, iv);
  if (add)
    cs->vars.add(iv);
}

static void
make_closure(AVar *result) {
  assert(result->contour_is_entry_set);
  PNode *pn = result->var->def;
  CreationSet *cs = creation_point(result, sym_function);
  int add = !cs->vars.n;
  PNode *partial_application = result->var->def;
  EntrySet *es = (EntrySet*)result->contour;
  if (partial_application->prim) { // apply and period
    if (pn->prim == prim_setter && fa->setter_token) {
      pn->tvals.fill(4);
      make_closure_var(partial_application->rvals.v[4], es, cs, result, add, 0);
      make_closure_var(partial_application->rvals.v[3], es, cs, result, add, 1);
      make_closure_var(fa->setter_token->var, es, cs, result, add, 2);
      make_closure_var(partial_application->rvals.v[1], es, cs, result, add, 3);
    } else if (pn->prim == prim_period && fa->method_token) {
      pn->tvals.fill(3);
      make_closure_var(partial_application->rvals.v[3], es, cs, result, add, 0);
      make_closure_var(fa->method_token->var, es, cs, result, add, 1);
      make_closure_var(partial_application->rvals.v[1], es, cs, result, add, 2);
    } else {
      pn->tvals.fill(2);
      make_closure_var(partial_application->rvals.v[3], es, cs, result, add, 0);
      make_closure_var(partial_application->rvals.v[1], es, cs, result, add, 1);
    }
  } else {
    pn->tvals.fill(partial_application->rvals.n);
    for (int i = 0; i < partial_application->rvals.n; i++)
      make_closure_var(partial_application->rvals.v[i], es, cs, result, add, i);
  }
}

// for send nodes, add simple constraints which do not depend 
// on the computed types (compare to add_send_edgse_pnodes)
static void
add_send_constraints(EntrySet *es) {
  forv_PNode(p, es->fun->fa_send_PNodes) {
    if (p->prim) {
      int start = (p->rvals.v[0]->sym == sym_operator) ? 1 : 0;
      // return constraints
      for (int i = 0; i < p->lvals.n; i++) {
        int ii = i;
        if (p->prim->nrets < 0 || p->prim->nrets <= i)
          ii = -p->prim->nrets -1; // last
        switch (p->prim->ret_types[ii]) {
          case PRIM_TYPE_ANY: break;
          case PRIM_TYPE_BOOL: update_gen(make_AVar(p->lvals.v[i], es), bool_type); break;
          case PRIM_TYPE_SIZE: update_gen(make_AVar(p->lvals.v[i], es), size_type); break;
          case PRIM_TYPE_ANY_NUM_AB:
          case PRIM_TYPE_A: {
            for (int j = start; j < p->rvals.n; j++)
              if (j - start != p->prim->pos) {
                AVar *av = make_AVar(p->rvals.v[j], es), *res = make_AVar(p->lvals.v[0], es);
                av->arg_of_send.add(res);
              }
            break;
          }
          default: assert(!"case"); break;
        }
      }
      // specifics
      switch (p->prim->index) {
        default: break;
        case P_prim_reply:
          fill_rets(es, p->rvals.n - 2);
          for (int i = 2; i < p->rvals.n; i++) {
            AVar *r = make_AVar(p->rvals.v[i], es);
            flow_vars(r, es->rets.v[i - 2]);
          }
          break;
        case P_prim_tuple: prim_make(p, es, sym_tuple); break;
        case P_prim_list: prim_make_list(p, es); break;
        case P_prim_vector: prim_make_vector(p, es); break;
        case P_prim_continuation: prim_make(p, es, sym_continuation); break;
        case P_prim_set: prim_make(p, es, sym_set); break;
        case P_prim_ref: prim_make(p, es, sym_ref, 2, 1); break;
      }
    }
  }
}

static void
add_move_constraints(EntrySet *es) {
  Fun *f = es->fun;
  forv_PNode(p, f->fa_phi_PNodes) {
    AVar *vv = make_AVar(p->lvals.v[0], es);
    forv_Var(v, p->rvals)
      flow_vars(make_AVar(v, es), vv);
  }
  forv_PNode(p, f->fa_phy_PNodes) {
    AVar *vv = make_AVar(p->rvals.v[0], es);
    forv_Var(v, p->lvals)
      flow_vars(vv, make_AVar(v, es));
  }
  forv_PNode(p, f->fa_move_PNodes) {
    for (int i = 0; i < p->rvals.n; i++) {
      AVar *lhs = make_AVar(p->lvals.v[i], es), *rhs = make_AVar(p->rvals.v[i], es);
      if (lhs->lvalue && rhs->lvalue)
        flow_vars(rhs, lhs);
      else
        flow_vars_assign(rhs, lhs);
    }
  }
}

static AEdge *
get_AEdge(Fun *f, PNode *p, EntrySet *from) {
  Map<Fun *, AEdge *> *f2e = from->out_edge_map.get(p);
  if (!f2e)
    from->out_edge_map.put(p, (f2e = new Map<Fun *, AEdge *>()));
  AEdge *e = f2e->get(f);
  if (!e) {
    f2e->put(f, (e = new AEdge()));
    e->pnode = p;
    e->from = from;
    e->fun = f;
  }
  return e;
}

static AEdge *
make_AEdge(Match *m, PNode *p, EntrySet *from) {
  AEdge *e = get_AEdge(m->fun, p, from);
  if (!e->match)
    e->match = m;
  else {
    assert(e->match->fun == m->fun);
    forv_MPosition(p, e->match->fun->positional_arg_positions) {
      AType *t1 = e->match->formal_filters.get(p), *t2 = m->formal_filters.get(p);
      if (t1 && t2)
        e->match->formal_filters.put(p, type_union(t1, t2));
      else if (!t1 && t2)
        e->match->formal_filters.put(p, t2);
    }
  }
  from->out_edges.set_add(e);
  if (!e->in_edge_worklist) {
    e->in_edge_worklist = 1;
    edge_worklist.enqueue(e);
  }
  return e;
}

// returns 1 if any are partial, 0 if some matched and -1 if none matched
static int
all_applications(PNode *p, EntrySet *es, AVar *a0, Vec<AVar *> &args, Partial_kind partial) {
  int incomplete = -2;
  a0->arg_of_send.add(make_AVar(p->lvals.v[0], es));
  forv_CreationSet(cs, *a0->out) if (cs)
    switch (application(p, es, a0, cs, args, partial)) {
      case -1: if (incomplete < 0) incomplete = -1; break;
      case 0: if (incomplete < 0) incomplete = 0; break;
      case 1: incomplete = 1; break;
    }
  return incomplete;
}

static int
partial_application(PNode *p, EntrySet *es, CreationSet *cs, Vec<AVar *> args, Partial_kind partial) {
  AVar *result = make_AVar(p->lvals.v[0], es);
  assert(result->var->def == p);
  AVar *fun = cs->vars.v[0];
  for (int i = cs->vars.n - 1; i >= 1; i--) {
    cs->vars.v[i]->arg_of_send.add(result);
    args.add(cs->vars.v[i]);
  }
  return all_applications(p, es, fun, args, partial);
}

static void
record_arg(CreationSet *cs, AVar *a, Sym *s, AEdge *e, MPosition &p) {
  MPosition *cpnum = cannonicalize_mposition(p), *cpname = 0, cpname_p;
  if (positional_to_named(cs, a, p, &cpname_p))
    cpname = cannonicalize_mposition(cpname_p);
  for (MPosition *cp = cpnum; cp; cp = cpname, cpname = 0) { 
    e->args.put(cp, a);
    if (s->is_pattern) {
      AType *t = type_intersection(a->out, e->match->formal_filters.get(cp));
      forv_CreationSet(cs, *t) {
        assert(s->has.n == cs->vars.n);
        p.push(1);
        for (int i = 0; i < s->has.n; i++) {
          record_arg(cs, cs->vars.v[i], s->has.v[i], e, p);
          p.inc();
        }
        p.pop();
      }
    }
  }
}

int
function_dispatch(PNode *p, EntrySet *es, AVar *a0, CreationSet *s, Vec<AVar *> &args, 
                  Partial_kind partial) 
{
  Vec<AVar *> a;
  int partial_result = 0;
  a.add(a0);
  for (int j = args.n - 1; j >= 0; j--)
    a.add(args.v[j]);
  Vec<Match *> matches;
  AVar *send = make_AVar(p->lvals.v[0], es);
  if (pattern_match(a, send, partial, &matches)) {
    forv_Match(m, matches) {
      if (!m->partial && partial != Partial_ALWAYS) {
        AEdge *ee = make_AEdge(m, p, es);
        if (!ee->args.n) {
          MPosition p;
          p.push(1);
          for (int i = 0; i < m->fun->sym->has.n; i++) {
            record_arg(0, a.v[i], m->fun->sym->has.v[i], ee, p);
            p.inc();
          }
        }
        if (!ee->rets.n) {
          for (int i = 0; i < p->lvals.n; i++)
            ee->rets.add(make_AVar(p->lvals.v[i], ee->from));
        }
      } else 
        partial_result = 1;
#ifdef CACHE_CALLEES
      if (!p->next_callees)
        p->next_callees = new Callees;
      Fun *f = m->fun;
      p->next_callees->funs.set_add(f);
#endif
    }
  }
  return matches.n ? partial_result : -1;
}

static int
application(PNode *p, EntrySet *es, AVar *a0, CreationSet *cs, Vec<AVar *> &args, 
            Partial_kind partial) 
{
  if (sym_function->implementors.set_in(cs->sym) && cs->defs.n)
    return partial_application(p, es, cs, args, partial);
  return function_dispatch(p, es, a0, cs, args, partial);
}

void
type_violation(ATypeViolation_kind akind, AVar *av, AType *type, AVar *send, Vec<Fun *> *funs) {
  ATypeViolation *v = new ATypeViolation(akind, av, send);
  v = type_violation_hash.put(v);
  if (!v->type)
    v->type = type;
  else
    v->type = type_union(v->type, type);
  if (funs) {
    if (v->funs)
      v->funs->set_union(*funs);
    else
      v->funs = new Vec<Fun *>(*funs);
  }
  type_violations.set_add(v);
}

static void
destruct(AVar *ov, Var *p, EntrySet *es, AVar *result) {
  AVar *pv = make_AVar(p, es);
  flow_vars(ov, pv);
  if (p->sym->has.n) {
    AVar *violation = 0;
    forv_CreationSet(cs, *ov->out) if (cs) {
      if (p->sym->must_specialize->specializers.in(cs->sym)) {
        for (int i = 0; i < p->sym->has.n; i++) {
          AVar *av = NULL;
          int is_tuple = sym_tuple->specializers.set_in(cs->sym->type) != 0;
          if (!is_tuple && p->sym->has.v[i]->destruct_name)
            av = cs->var_map.get(p->sym->has.v[i]->destruct_name);
          else if (is_tuple && i < cs->vars.n)
            av = cs->vars.v[i];
          if (!av) {
            violation = make_AVar(p->sym->has.v[i]->var, es);
            goto Lviolation;
          }
          destruct(av, p->sym->has.v[i]->var, es, result);
        }
      } else {
      Lviolation:
        AVar *av = violation ? violation : ov;
        if (!av->var->sym->name && p->sym->name)
          av = pv;
        if (!av->var->sym->name && cs->vars.n < p->sym->has.n && p->sym->has.v[cs->vars.n]->name)
          av = make_AVar(p->sym->has.v[cs->vars.n]->var, es);
        if (!av->var->sym->name && cs->vars.n > p->sym->has.n && p->sym->has.n &&
            p->sym->has.v[p->sym->has.n-1]->name)
          av = make_AVar(p->sym->has.v[p->sym->has.n-1]->var, es);
        type_violation(ATypeViolation_MATCH, av, make_AType(cs), result);
      }
    }
  }
}

static int
get_tuple_index(AVar *index, int *i, int n) {
  if (index->var->sym->type && index->var->sym->imm_int(i) == 0) {
    *i -= fa->tuple_index_base;
    if (*i >= 0 && *i < n)
      return 1;
  }
  if (index->out->n == 1 && index->out->v[0]->sym->is_constant)
    if (index->out->v[0]->sym->imm_int(i) == 0) {
      *i -= fa->tuple_index_base;
      if (*i >= 0 && *i < n)
        return 1;
    }
  return 0;
}

// for send nodes, add call edges and more complex constraints
// which depend on the computed types (compare to add_send_constraints)
static void
add_send_edges_pnode(PNode *p, EntrySet *es) {
  if (!p->prim) {
    assert(p->lvals.n == 1);
    AVar *result = make_AVar(p->lvals.v[0], es);
    Vec<AVar *> args;
    for (int i = p->rvals.n - 1; i > 0; i--) {
      AVar *av = make_AVar(p->rvals.v[i], es);
      av->arg_of_send.add(result);
      args.add(av);
    }
    AVar *a0 = make_AVar(p->rvals.v[0], es);
    if (all_applications(p, es, a0, args, (Partial_kind)p->code->partial) > 0)
      make_closure(result);
  } else {
    // argument and return constraints
    int n = p->prim->nargs < 0 ? -p->prim->nargs : p->prim->nargs;
    AVar *a = 0, *b = 0;
    int start = 0, iarg = 0;
    if (p->rvals.v[0]->sym == sym_operator)
      start = 1;
    for (int i = start; i < p->rvals.n; i++) {
      if (i - start == p->prim->pos) continue;
      AVar *arg = make_AVar(p->rvals.v[i], es);
      // record violations
      if (type_diff(arg->out, p->prim->args.v[iarg]) != bottom_type)
        type_violation(ATypeViolation_PRIMITIVE_ARGUMENT, arg, 
                       type_diff(arg->out, p->prim->args.v[iarg]), 
                       make_AVar(p->lvals.v[0], es)); 
      switch (p->prim->arg_types[iarg]) {
        default: break;
        case PRIM_TYPE_ANY_NUM_A: a = arg; break;
        case PRIM_TYPE_ANY_NUM_B: b = arg; break;
        case PRIM_TYPE_ANY_INT_A: a = arg; break;
        case PRIM_TYPE_ANY_INT_B: b = arg; break;
      }
      if (i - start < n - 1) iarg++;
    }
    for (int i = 0; i < p->lvals.n; i++) {
      if (p->prim->ret_types[i] == PRIM_TYPE_ANY_NUM_AB) {
        AVar *res = make_AVar(p->lvals.v[i], es);
        // connect the flows, but prevent values to pass
        // so that splitting can attribute causality
        fill_tvals(es->fun, p, p->lvals.n);
        AVar *t = make_AVar(p->tvals.v[i], es);
        flow_var_type_permit(t, bottom_type);
        flow_vars(a, t);
        flow_vars(b, t);
        flow_vars(t, res);
        update_in(res, type_num_fold(p->prim, a->out, b->out));
      }
      if (p->prim->ret_types[i] == PRIM_TYPE_A)
        flow_vars(a, make_AVar(p->lvals.v[i], es));
    }
    AVar *result = p->lvals.n ? make_AVar(p->lvals.v[0], es) : 0;
    if (result)
      for (int i = 0; i < p->rvals.n; i++)
        make_AVar(p->rvals.v[i], es)->arg_of_send.add(result);
    // specifics
    switch (p->prim->index) {
      default: break;
      case P_prim_primitive: {
        char *name = p->rvals.v[1]->sym->name;
        if (!name) name = p->rvals.v[1]->sym->constant;
        if (!name) name = p->rvals.v[1]->sym->imm.v_string;
        if (!name)
          fail("bad primitive transfer function");
        RegisteredPrim *rp = fa->primitive_transfer_functions.get(name);
        if (!rp)
          fail("undefined primitive transfer function '%s'", 
               p->rvals.v[1]->sym->name);
        rp->fn(p, es);
        break;
      }
      case P_prim_meta_apply: {
        AVar *a1 = make_AVar(p->rvals.v[1], es);
        AVar *a2 = make_AVar(p->rvals.v[2], es);
        Sym *s;
        forv_CreationSet(cs1, *a1->out)
          forv_CreationSet(cs2, *a2->out)
            if (cs1->sym->is_meta_type && cs2->sym->is_meta_type && 
                (s = meta_apply(cs1->sym->meta_type, cs2->sym->meta_type)))
              update_gen(result, make_abstract_type(s));
            else
              type_violation(ATypeViolation_SEND_ARGUMENT, a1, a1->out, result);
        break;
      }
      case P_prim_destruct: {
        assert(p->rvals.n - 1 == p->lvals.n);
        for (int i = 0; i < p->lvals.n; i++) {
          AVar *av = make_AVar(p->rvals.v[i + 1], es);
          destruct(av, p->lvals.v[i], es, result);
          av->arg_of_send.add(result);
        }
        break;
      }
      case P_prim_vector:
        prim_make_vector(p, es);
        break;
      case P_prim_index_object: {
        AVar *vec = make_AVar(p->rvals.v[1], es);
        AVar *index = make_AVar(p->rvals.v[2], es);
        set_container(result, vec);
        forv_CreationSet(cs, vec->out->sorted) {
          if (sym_tuple->specializers.set_in(cs->sym->type)) {
            int i;
            if (get_tuple_index(index, &i, cs->vars.n))
              flow_vars(cs->vars.v[i], result);
            else
              for (int i = 0; i < cs->vars.n; i++) // assume the worst
                flow_vars(cs->vars.v[i], result);
          } else {
            AVar *elem = get_element_avar(cs);
            flow_vars(elem, result);
          }
        }
        break;
      }
      case P_prim_set_index_object: {
        AVar *vec = make_AVar(p->rvals.v[1], es);
        AVar *index = make_AVar(p->rvals.v[2], es);
        AVar *val = make_AVar(p->rvals.v[3], es);
        set_container(val, vec);
        forv_CreationSet(cs, *vec->out) if (cs) {
          if (sym_tuple->specializers.set_in(cs->sym->type)) {
            int i;
            if (get_tuple_index(index, &i, cs->vars.n))
              flow_vars(val, cs->vars.v[i]);
            else
              for (int i = 0; i < cs->vars.n; i++) 
                flow_vars(val, cs->vars.v[i]);
          } else {
            AVar *elem = get_element_avar(cs);
            flow_vars(val, elem);
          }
        }
        flow_vars(val, result);
        break;
      }
      case P_prim_apply: {
        assert(p->lvals.n == 1);
        Vec<AVar *> args;
        AVar *fun = make_AVar(p->rvals.v[1], es);
        AVar *a1 = make_AVar(p->rvals.v[3], es);
        args.add(a1);
        if (all_applications(p, es, fun, args, (Partial_kind)p->code->partial) > 0)
          make_closure(result);
        break;
      }
      case P_prim_period: {
        AVar *obj = make_AVar(p->rvals.v[1], es);
        AVar *selector = make_AVar(p->rvals.v[3], es);
        int partial = 0;
        set_container(result, obj);
        forv_CreationSet(sel, *selector->out) if (sel) {
          char *symbol = sel->sym->name; assert(symbol);
          forv_CreationSet(cs, *obj->out) if (cs) {
            if (cs == null_type->v[0])
              continue;
            AVar *iv = cs->var_map.get(symbol);
            if (iv)
              flow_vars(iv, result);
            else {
              Vec<AVar *> args;
              args.add(obj);
              if (fa->method_token)
                args.add(fa->method_token);
              int res = application(p, es, selector, cs, args, (Partial_kind)p->code->partial);
              if (res > 0)
                partial = 1;
              else if (res < 0)
                type_violation(ATypeViolation_MEMBER, selector, make_AType(cs), result);
            }
          }
        }
        if (partial)
          make_closure(result);
        break;
      }
      case P_prim_setter: {
        AVar *obj = make_AVar(p->rvals.v[1], es);
        AVar *selector = make_AVar(p->rvals.v[3], es);
        AVar *val = make_AVar(p->rvals.v[4], es);
        int partial = 0;
        p->tvals.fill(1);
        if (!p->tvals.v[0]) {
          Sym *s = new_Sym();
          s->function_scope = 1;
          p->tvals.v[0] = new Var(s);
          s->var = p->tvals.v[0];
          p->tvals.v[0]->is_internal = 1;
          es->fun->fa_all_Vars.add(p->tvals.v[0]);
        }
        AVar *tval = make_AVar(p->tvals.v[0], es);
        flow_vars(val, tval);
        set_container(tval, obj);
        forv_CreationSet(sel, *selector->out) if (sel) {
          char *symbol = sel->sym->name; assert(symbol);
          forv_CreationSet(cs, *obj->out) if (cs) {
            if (cs == null_type->v[0])
              continue;
            AVar *iv = cs->var_map.get(symbol);
            if (iv)
              flow_vars(tval, iv);
            else {
              Vec<AVar *> args;
              args.add(obj);
              if (fa->setter_token)
                args.add(fa->setter_token);
              args.add(val);
              int res = application(p, es, selector, cs, args, (Partial_kind)p->code->partial);
              if (res > 0)
                partial = 1;
              else if (res < 0)
                type_violation(ATypeViolation_MEMBER, selector, make_AType(cs), result);
            }
          }
        }
        if (partial)
          make_closure(result);
        flow_vars(val, result);
        break;
      }
      case P_prim_assign: {
        AVar *lhs = make_AVar(p->rvals.v[1], es);
        AVar *rhs = make_AVar(p->rvals.v[3], es);
        forv_CreationSet(cs, *lhs->out) if (cs) {
          if (cs->sym == sym_ref) {
            assert(cs->vars.n);
            AVar *av = cs->vars.v[0];
            flow_vars(rhs, av);
            flow_vars(rhs, result);
          } else {
            if (sym_anynum->specializers.in(cs->sym->type))
              update_in(result, cs->sym->type->abstract_type);
            else
              type_violation(ATypeViolation_MATCH, lhs, make_AType(cs), result);
          }
        }
        break;
      }
      case P_prim_deref: {
        AVar *ref = make_AVar(p->rvals.v[2], es);
        set_container(result, ref);
        forv_CreationSet(cs, *ref->out) if (cs) {
          AVar *av = cs->vars.v[0];
          flow_vars(av, result);
        }
        break;
      }
      case P_prim_new: {
        AVar *thing = make_AVar(p->rvals.v[1], es);
        forv_CreationSet(cs, *thing->out) if (cs)
          creation_point(result, cs->sym->meta_type); // recover original type
        break;
      }
      case P_prim_coerce: {
        assert(p->rvals.v[1]->sym->abstract_type);
        Sym *s = p->rvals.v[1]->sym;
        AVar *rhs = make_AVar(p->rvals.v[2], es);
        Vec<CreationSet *> css;
        forv_CreationSet(cs, *rhs->out)
          if (cs->sym->type == p->rvals.v[1]->sym)
            css.set_add(cs);
        if (css.n)
          update_gen(result, make_AType(css));
        else if (s->type->num_kind || s->type == sym_string || s->type->is_symbol)
          update_gen(result, s->abstract_type);
        break;
      }
      case P_prim_cast: {
        assert(!"implemented");
        break;
      }
    }
  }
}

static void
add_send_edges(AEdge *e) {
  forv_PNode(p, e->match->fun->fa_send_PNodes)
    add_send_edges_pnode(p, e->to);
}

static inline int
is_fa_Var(Var *v) { 
  return v->sym->type || v->sym->aspect || v->sym->is_constant || v->sym->is_symbol;
}

static void
collect_Vars_PNodes(Fun *f) {
  f->fa_collected = 1;
  if (!f->entry)
    return;
  f->collect_Vars(f->fa_all_Vars, &f->fa_all_PNodes);
  qsort_by_id(f->fa_all_Vars);
  qsort_by_id(f->fa_all_PNodes);
  forv_Var(v, f->fa_all_Vars)
    if (is_fa_Var(v))
      f->fa_Vars.add(v);
  Primitives *prim = if1->primitives;
  forv_PNode(p, f->fa_all_PNodes) {
    if (p->code->kind == Code_MOVE)
      f->fa_move_PNodes.add(p);
    f->fa_phi_PNodes.append(p->phi);
    f->fa_phy_PNodes.append(p->phy);
    if (p->code->kind == Code_SEND) {
      p->prim = prim->find(p);
      f->fa_send_PNodes.add(p);
    }
  }
  forv_Var(v, f->fa_all_Vars)
    if (v->sym->clone_for_constants)
      f->clone_for_constants = 1;
}

static AVar *
get_filtered(AEdge *e, MPosition *p, AVar *av) {
  AVar *filtered = e->filtered_args.get(p);
  if (!filtered) {
    Var *filtered_v = new Var(av->var->sym);
    filtered_v->is_internal = 1;
    filtered_v->is_filtered = 1;
    e->filtered_args.put(p, (filtered = unique_AVar(filtered_v, e->to)));
  }
  return filtered;
}

static void
analyze_edge(AEdge *e) {
  make_entry_set(e);
  form_MPositionAVar(x, e->args) {
    if (!x->key->is_positional())
      continue;
    MPosition *p = x->key;
    AVar *actual = x->value, *formal = make_AVar(e->to->fun->args.get(p), e->to),
      *filtered = get_filtered(e, p, formal);
    AType *filter = e->match->formal_filters.get(p);
    if (!filter)
      continue;
    flow_var_type_permit(filtered, filter);
    forv_CreationSet(cs, *filter) if (cs) 
      cs->ess.set_add(e->to);
    flow_vars(actual, filtered);
    flow_vars(filtered, formal);
    if (p->pos.n > 1)
      set_container(filtered, get_filtered(e, p->up, e->to->args.get(p->up)));
    else if (!actual->contour_is_entry_set && actual->contour != GLOBAL_CONTOUR) // closure
      set_container(filtered, make_AVar(e->pnode->rvals.v[0], e->from));
  }
  creation_point(make_AVar(e->match->fun->sym->cont->var, e->to), sym_continuation);
  for (int i = 0; i < e->pnode->lvals.n; i++)
    flow_vars(e->to->rets.v[i], e->rets.v[i]);
  int regular_rets = e->pnode->lvals.n;
  fill_rets(e->to, regular_rets + e->match->fun->out_positions.n);
  for (int o = 0; o < e->match->fun->out_positions.n; o++) {
    MPosition *p = e->match->fun->out_positions.v[o];
    p = p ? p : e->match->fun->out_positions.v[o];
    AVar *actual = e->args.get(p);
    flow_vars(e->to->rets.v[o + regular_rets], actual);
  }
  if (!entry_set_done.set_in(e->to)) {
    entry_set_done.set_add(e->to);
    if (!e->match->fun->fa_collected)
      collect_Vars_PNodes(e->match->fun);
    add_var_constraints(e->to);
    add_move_constraints(e->to);
    add_send_constraints(e->to);
    add_send_edges(e);
  }
}

static void
refresh_top_edge(AEdge *e) {
  MPosition p, *cp;
  p.push(1);
  cp = cannonicalize_mposition(p);
  e->match->formal_filters.put(cp, any_type);
  AVar *av = make_AVar(sym_init->var, e->to);
  e->args.put(cp, av);
  e->filtered_args.put(cp, av);
  update_gen(av, av->var->sym->abstract_type);
}

static AEdge *
make_top_edge(Fun *top) {
  AEdge *e = new AEdge();
  e->match = new Match(top);
  e->pnode = new PNode();
  make_entry_set(e);
  sym_init->var = new Var(sym_init);
  refresh_top_edge(e);
  return e;
}

static inline int
is_return_value(AVar *av) {
  EntrySet *es = (EntrySet*)av->contour;
  forv_AVar(v, es->rets)
    if (v == av)
      return 1;
  return 0;
}

static void
show_sym_name(Sym *s, FILE *fp) {
  if (s->name)
    fprintf(fp, "%s", s->name);
  else if (s->constant)
    fprintf(fp, "\"%s\"", s->constant);
  else
    fprintf(fp, "%d", s->id);
}

static void
show_type(Vec<CreationSet *> &t, FILE *fp) {
  Vec<Sym *> type;
  forv_CreationSet(cs, t) if (cs) {
    Sym *s = cs->sym;
    if (!verbose_level)
      s = s->type;
    type.set_add(s);
  }
  type.set_to_vec();
  qsort_by_id(type);
  fprintf(fp, "( ");
  forv_Sym(s, type) if (s) {
    show_sym_name(s, fp);
    fprintf(fp, " ");
  }
  fprintf(fp, ") ");
}

static void
show_sym(Sym *s, FILE *fp) {
  if (s->is_pattern) {
    fprintf(fp, "( ");
    forv_Sym(ss, s->has) {
      if (ss != s->has.v[0])
        fprintf(fp, ", ");
      show_sym(ss, fp);
    }
    fprintf(fp, ")");
  } else if (s->name)
    fprintf(fp, "%s", s->name);
  else if (s->constant)
    fprintf(fp, "\"%s\"", s->constant);
  else
    fprintf(fp, "_", s->constant);
  if (s->type && s->type->name)
    fprintf(fp, " = %s", s->type->name);
  else if (s->must_implement && 
           s->must_implement == s->must_specialize) {
    fprintf(fp, " : ");
    show_sym_name(s->must_implement, fp);
  } else if (s->must_implement) {
    fprintf(fp, " < ");
    show_sym_name(s->must_implement, fp);
  } else if (s->must_specialize && !s->must_specialize->is_symbol) {
    fprintf(fp, " @ ", s->must_specialize);
    show_sym_name(s->must_specialize, fp);
  }
}

static void
show_fun(Fun *f, FILE *fp) {
  if (f->line() > 0)
    fprintf(fp, "%s:%d: ", f->filename(), f->line());
  forv_Sym(s, f->sym->has) {
    show_sym(s, fp);
    if (s != f->sym->has.v[f->sym->has.n-1])
      fprintf(fp, ", ");
  }
  if (verbose_level)
    fprintf(fp, " id:%d", f->sym->id);
}

static void
show_atype(AType &t, FILE *fp) {
  fprintf(fp, "( ");
  forv_CreationSet(cs, t.sorted) if (cs) {
    show_sym_name(cs->sym, fp);
    fprintf(fp, " id:%d ", cs->id);
  }
  fprintf(fp, ") ");
}

void
fa_print_backward(AVar *v, FILE *fp = 0) {
  if (!fp) fp = stdout;
  Vec<AVar *> done, todo;
  todo.add(v);
  done.set_add(v);
  for (int i = 0; i < todo.n; i++) {
    v = todo.v[i];
    if (v->var) {
      if (v->var->sym) {
        if (v->var->sym->name)
          fprintf(fp, "%s %d\n", v->var->sym->name, v->var->sym->id);
        else
          fprintf(fp, "%d\n", v->var->sym->id);
      } else
        fprintf(fp, "VAR %p\n", v->var);
    } else
      fprintf(fp, "AVAR %p\n", v);
    show_atype(*v->out, fp); fprintf(fp, "\n");
    forv_AVar(vv, v->backward) if (vv) {
      if (!done.set_in(vv)) {
        todo.add(vv);
        done.set_add(vv);
      }
    }
  }
}

void
fa_dump_var_types(AVar *av, FILE *fp, int verbose = verbose_level) {
  Var *v = av->var;
  if (verbose < 2 && (!v->sym->name || v->sym->is_symbol))
    return;
  if (!v->sym->in)
    fprintf(fp, "::");
  else if (v->sym->in->name)
    fprintf(fp, "%s::", v->sym->in->name);
  else
    fprintf(fp, "%d::", v->sym->in->id);
  if (v->sym->name)
    fprintf(fp, "%s(%d) ", v->sym->name, v->sym->id);
  else
    fprintf(fp, "(%d) ", v->sym->id);
  if (v->sym->is_constant) {
    if (v->sym->constant)
      fprintf(fp, "\"%s\" ", v->sym->constant);
    else {
      fprintf(fp, "\"");
      fprint_imm(fp, v->sym->imm);
      fprintf(fp, "\" ");
    }
  }
  show_type(*av->out, fp);
  fprintf(fp, "\n");
}

void
fa_dump_types(FA *fa, FILE *fp) {
  Vec<Var *> gvars;
  forv_EntrySet(es, fa->ess) {
    Fun *f = es->fun;
    if (f->sym->name)
      fprintf(fp, "function %s (%d) ", f->sym->name, f->sym->id);
    else
      fprintf(fp, "function %d ", f->sym->id);
    fprintf(fp, "entry set with %d edges\n", es->edges.count());
    Vec<Var *> vars;
    f->collect_Vars(vars);
    forv_Var(v, vars) {
      if (v->sym->global_scope) {
        gvars.set_add(v);
        continue;
      }
      fa_dump_var_types(make_AVar(v, es), fp);
    }
  }
  gvars.set_to_vec();
  fprintf(fp, "globals\n");
  forv_Var(v, gvars)
    if (!v->sym->is_constant && !v->sym->is_symbol)
      fa_dump_var_types(unique_AVar(v, GLOBAL_CONTOUR), fp);
}

static void
show_illegal_type(FILE *fp, ATypeViolation *v) {
  AVar *av = v->av;
  if (av->var->sym->name)
    fprintf(fp, "'%s' ", av->var->sym->name);
  else if (verbose_level)
    fprintf(fp, "expr:%d ", av->var->sym->id);
  else
    fprintf(fp, "expression ");
  if (verbose_level) {
    fprintf(fp, "id:%d ", av->var->sym->id);
    if (av->out->n) {
      fprintf(fp, ": ");
      show_type(*av->out, fp);
    }
  }
  fprintf(fp, "illegal: ");
  show_type(*v->type->type, fp);
  fprintf(fp, "\n");
}

static int
compar_edge_id(const void *aa, const void *bb) {
  AEdge *a = (*(AEdge**)aa);
  AEdge *b = (*(AEdge**)bb);
  int i = 0, j = 0;
  if (a->pnode && a->pnode->lvals.n)
    i = make_AVar(a->pnode->lvals.v[0], a->to)->id;
  if (b->pnode && b->pnode->lvals.n)
    j = make_AVar(b->pnode->lvals.v[0], b->to)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

static void
show_call_tree(FILE *fp, PNode *p, EntrySet *es, int depth = 0) {
  depth++;
  if (depth > print_call_depth || !p->code)
    return;
  if (depth > 1 && p->code->filename() && p->code->line() > 0) {
    for (int x = 0; x < depth; x++)
      fprintf(fp, " ");
    fprintf(fp, "called from %s:%d", p->code->filename(), p->code->line());
    if (verbose_level && p->lvals.n)
      fprintf(fp, " send:%d", p->lvals.v[0]->sym->id);
    fprintf(fp, "\n");

  }
  Vec<AEdge*> edges;
  AEdge **last = es->edges.last();
  for (AEdge **x = es->edges.first(); x < last; x++) if (*x)
    edges.add(*x);
  qsort(edges.v, edges.n, sizeof(edges.v[0]), compar_edge_id);
  forv_AEdge(e, edges)
    show_call_tree(fp, e->pnode, e->from, depth);
}

void
show_avar_call_tree(FILE *fp, AVar *av) {
  EntrySet *es = (EntrySet*)av->contour;
  AEdge **last = es->edges.last();
  Vec<AEdge*> edges;
  for (AEdge **x = es->edges.first(); x < last; x++) if (*x)
    edges.add(*x);
  qsort(edges.v, edges.n, sizeof(edges.v[0]), compar_edge_id);
  forv_AEdge(e, edges)
    show_call_tree(fp, e->pnode, e->from, 1);
}

static void
show_candidates(FILE *fp, PNode *pn, Sym *arg0) {
  Vec<Fun *> *pfuns = pn->code->ast->visible_functions(arg0);
  if (!pfuns)
    return;
  Vec<Fun *> funs(*pfuns);
  funs.set_to_vec();
  qsort_by_id(funs);
  fprintf(fp, "note: candidates are:\n");
  forv_Fun(f, funs) {
    show_fun(f, fp);
    fprintf(fp, "\n");
  }
}

static int
compar_tv(const void *aa, const void *bb) {
  int i, j, x;
  ATypeViolation *a = (*(ATypeViolation**)aa);
  ATypeViolation *b = (*(ATypeViolation**)bb);
  IFAAST *aast = a->send ? a->send->var->def->code->ast : 0;
  if (!aast) aast = a->av->var->sym->ast;
  IFAAST *bast = b->send ? b->send->var->def->code->ast : 0;
  if (!bast) bast = b->av->var->sym->ast;
  if (!aast || !bast) {
    if (bast) return -1;
    if (aast) return 1;
    goto Lskip;
  }
  if (!aast->pathname() || !bast->pathname()) {
    if (bast->pathname()) return -1;
    if (aast->pathname()) return 1;
  } else {
    int x = strcmp(aast->pathname(), bast->pathname());
    if (x) return x;
  }
  i = aast->line();
  j = bast->line();
  x = (i > j) ? 1 : ((i < j) ? -1 : 0);
  if (x)
    return x;
 Lskip:
  if (a->kind < b->kind)
    return -1;
  if (b->kind < a->kind)
    return 1;
  if (a->av && b->av) {
    if (a->av->var && b->av->var) {
      if (a->av->var->sym && b->av->var->sym) {
        i = a->av->var->sym->id;
        j = b->av->var->sym->id;
        x = (i > j) ? 1 : ((i < j) ? -1 : 0);
        if (x)
          return x;
      }
      i = a->av->var->id;
      j = b->av->var->id;
      x = (i > j) ? 1 : ((i < j) ? -1 : 0);
      if (x)
        return x;
    }
    i = a->av->id;
    j = b->av->id;
    x = (i > j) ? 1 : ((i < j) ? -1 : 0);
    if (x)
      return x;
  }
  if (a->send && b->send) {
    i = a->send->id;
    j = b->send->id;
    x = (i > j) ? 1 : ((i < j) ? -1 : 0);
    if (x)
      return x;
  }
  return 0;
}

static void
show_violations(FA *fa, FILE *fp) {
  Vec<ATypeViolation *> vv;
  forv_ATypeViolation(v, type_violations) if (v)
    vv.add(v);
  qsort(vv.v, vv.n, sizeof(vv.v[0]), compar_tv);
  forv_ATypeViolation(v, vv) if (v) {
    if (v->send && v->send->var->def->code->line() > 0)
      fprintf(fp, "%s:%d: ", v->send->var->def->code->filename(), 
              v->send->var->def->code->line());
    else if (v->av->var->sym->ast && v->av->var->sym->line() > 0)
      fprintf(fp, "%s:%d: ", v->av->var->sym->filename(), 
              v->av->var->sym->line());
    else if (!v->av->contour_is_entry_set && v->av->contour != GLOBAL_CONTOUR) {
      CreationSet *cs = (CreationSet*)v->av->contour;
      fprintf(fp, "%s:%d: class %s:: ", 
              cs->sym->filename(), cs->sym->line(), cs->sym->name);     
    } else
      fprintf(fp, "error: ");
    switch (v->kind) {
      default: assert(0);
      case ATypeViolation_PRIMITIVE_ARGUMENT:
        fprintf(fp, "illegal primitive argument type ");
        show_illegal_type(fp, v);
        break;
      case ATypeViolation_SEND_ARGUMENT:
        if (v->av->var->sym->is_symbol &&
            v->send->var->def->rvals.v[0] == v->av->var) {
          fprintf(fp, "unresolved call '%s'", v->av->var->sym->name);
          if (verbose_level)
            fprintf(fp, " send:%d", v->send->var->sym->id);
          fprintf(fp, "\n");
          show_candidates(fp, v->send->var->def, v->av->var->sym);
        } else {
          fprintf(fp, "illegal call argument type ");
          show_illegal_type(fp, v);
        }
        break;
      case ATypeViolation_DISPATCH_AMBIGUITY:
        fprintf(fp, "error: ambiguous call '%s'", v->av->var->sym->name);
        if (verbose_level)
          fprintf(fp, " send:%d", v->send->var->sym->id);
        fprintf(fp, "\n");
        fprintf(fp, "note: candidates are:\n");
        forv_Fun(f, *v->funs) if (f) {
          show_fun(f, fp);
          fprintf(fp, "\n");
        }
        break;
      case ATypeViolation_MEMBER:
        if (v->av->out->n == 1)
          fprintf(fp, "unresolved member '%s'", v->av->out->v[0]->sym->name);
        else {
          fprintf(fp, "unresolved member\n");
          forv_CreationSet(selector, *v->av->out)
            fprintf(fp, "  selector '%s'\n", selector->sym->name);
        }
        if (v->type->n == 1)
          fprintf(fp, "  class '%s'\n", v->type->v[0]->sym->name ? v->type->v[0]->sym->name : 
                  "<anonymous>");
        else {
          fprintf(fp, "  classes\n");
          forv_CreationSet(cs, *v->type)
            fprintf(fp, "  class '%s'\n", cs->sym->name);
        }
        break;
      case ATypeViolation_MATCH:
        if (v->av->var->sym->name)
          fprintf(fp, "near '%s' unmatched type: ", v->av->var->sym->name);
        else
          fprintf(fp, "unmatched type: ");
        show_type(*v->type, fp);
        fprintf(fp, "\n");
        break;
      case ATypeViolation_NOTYPE:
        if (v->av->var->sym->name)
          fprintf(fp, "'%s' ", v->av->var->sym->name);
        else if (verbose_level)
          fprintf(fp, "expr:%d ", v->av->var->sym->id);
        else
          fprintf(fp, "expression ");
        fprintf(fp, "has no type\n");
        break;
      case ATypeViolation_BOXING:
        if (v->av->var->sym->name)
          fprintf(fp, "'%s' ", v->av->var->sym->name);
        else if (verbose_level)
          fprintf(fp, "expr:%d ", v->av->var->sym->id);
        else
          fprintf(fp, "expression ");
        fprintf(fp, "has mixed basic types:");
        show_type(*v->type, fp);
        fprintf(fp, "\n");
        break;
    }
    if (v->send)
      show_call_tree(fp, v->send->var->def, (EntrySet*)v->send->contour);
    else if (v->av->contour_is_entry_set)
      show_avar_call_tree(fp, v->av);
    else if (v->av->contour != GLOBAL_CONTOUR)
      show_call_tree(fp, ((CreationSet*)v->av->contour)->defs.first()->var->def,
                     (EntrySet*)((CreationSet*)v->av->contour)->defs.first()->contour, 1);
  }
}

static char *fn(char *s) {
  if (!s)
    return "<none>";
  char *filename = strrchr(s, '/');
  if (filename)
    return filename + 1;        
  return s;
}

void
log_var_types(Var *v, Fun *f) {
  if (!v->sym->name || v->sym->is_symbol || v->is_internal)
    return;
  if (!v->sym->in)
    log(LOG_TEST_FA, "::");
  else if (v->sym->in->name)
    log(LOG_TEST_FA, "%s::", v->sym->in->name);
  else
    log(LOG_TEST_FA, "%d::", v->sym->in->id);
  if (v->sym->name) {
    if (v->sym->line() > 0)
      log(LOG_TEST_FA, "%s(%s:%d) ", v->sym->name, fn(v->sym->filename()), v->sym->log_line());
    else
      log(LOG_TEST_FA, "%s ", v->sym->name);
  } else
    log(LOG_TEST_FA, "(%s:%d) ", fn(v->sym->filename()), v->sym->log_line());
  Vec<CreationSet *> css;
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    if (!f || f->ess.in(((EntrySet*)av->contour)))
      css.set_union(*av->out);
  }
  assert(css.n);
  log(LOG_TEST_FA, "( ");
  Vec<Sym *> syms;
  forv_CreationSet(cs, css) if (cs)
    syms.set_add(cs->sym->type);
  syms.set_to_vec();
  qsort_by_id(syms);
  forv_Sym(s, syms) {
    if (s->name)
      log(LOG_TEST_FA, "%s ", s->name);
    else if (s->constant)
      log(LOG_TEST_FA, "\"%s\" ", s->constant);
    else if (s->is_constant) {
      char c[128];
      sprint_imm(c, s->imm);
      log(LOG_TEST_FA, "\"%s\" ", c);
    }
    if (s->log_line())
      log(LOG_TEST_FA, "(%s:%d) ", fn(s->filename()), s->log_line());
  }
  log(LOG_TEST_FA, ")\n");
}

static void
collect_results() {
  // collect funs, ess and ess_set
  fa->funs.clear();
  fa->ess.clear();
  forv_EntrySet(es, entry_set_done) if (es) {
    fa->funs.set_add(es->fun);
    fa->ess.add(es);
  }
  fa->funs.set_to_vec();
  qsort_by_id(fa->funs);
  fa->ess_set.move(entry_set_done);
  // collect css and css_set
  fa->css.clear();
  fa->css_set.clear();
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *xav = make_AVar(v, es);
      for (AVar *av = xav; av; av = av->lvalue)
        fa->css_set.set_union(*av->out);
    }
  }
  forv_CreationSet(cs, fa->css_set) if (cs) 
    fa->css.add(cs);
  qsort_by_id(fa->css);
  // collect callees
  forv_Fun(f, fa->funs) {
    forv_PNode(pnode, f->fa_send_PNodes) {
      if (pnode->next_callees) {
        pnode->callees = pnode->next_callees;
        pnode->next_callees = 0;
        forv_Fun(x, pnode->callees->funs) if (x)
          pnode->callees->arg_positions.set_union(x->arg_positions);
      }
    }
  }
  // print results
  if (verbose_level)
    fa_dump_types(fa, stdout);
  if (fgraph_pass_contours) {
    char fn[2048];
    strcpy(fn, fa->fn);
    sprintf(fn + strlen(fn), ".%d", analysis_pass);
    graph_contours(fa, fn);
  }
}

static int
empty_type_minus_partial_applications(AType *a) {
  forv_CreationSet(aa, *a) if (aa) {
    if (aa->sym == sym_function && aa->defs.n)
      continue;
    if (aa == null_type->v[0])
      continue;
    return 0;
  }
  return 1;
}

static AType *
type_minus_partial_applications(AType *a) {
  AType *r = new AType();
  forv_CreationSet(aa, *a) if (aa) {
    if (aa->sym == sym_function && aa->defs.n)
      continue;
    r->set_add(aa);
  }
  r = type_cannonicalize(r);
  return r;
}

// for each call site, check that all args are covered
static void
collect_argument_type_violations() {
  forv_Fun(f, fa->funs) {
    forv_PNode(p, f->fa_send_PNodes) {
      if (p->prim) continue; // primitives handled elsewhere
      Vec<EntrySet *> ess;
      f->ess.set_intersection(fa->ess_set, ess);
      forv_EntrySet(from, ess) if (from) {
        FunAEdgeMap *m = from->out_edge_map.get(p);
        if (!m) {
          if (p->code->partial == Partial_NEVER) {
            forv_Var(v, p->rvals) {
              AVar *av = make_AVar(v, from);
              type_violation(ATypeViolation_SEND_ARGUMENT, av, av->out, 
                             make_AVar(p->lvals.v[0], from));
            }
          }
        } else {
          Vec<AVar *> actuals;
          form_Map(FunAEdgeMapElem, me, *m) {
            if (!from->out_edges.set_in(me->value))
              continue;
            form_MPositionAVar(x, me->value->args)
              if (x->key->is_positional())
                actuals.set_add(x->value);
          }
          forv_AVar(av, actuals) if (av) {
            AType *t = av->out;
            form_Map(FunAEdgeMapElem, me, *m) {
              AEdge *e = me->value;
              if (!from->out_edges.set_in(e))
                continue;
              form_MPositionAVar(x, me->value->args) {
                if (x->value != av)
                  continue;
                if (!x->key->is_positional())
                  continue;
                MPosition *p = x->key;
                AVar *filtered = e->filtered_args.get(p);
                if (filtered)
                  t = type_diff(t, filtered->out);
              }
            }
            if (!empty_type_minus_partial_applications(t)) {
              t = type_minus_partial_applications(t);
              type_violation(ATypeViolation_SEND_ARGUMENT, av, t, make_AVar(p->lvals.v[0], from));
            }
          }
        }
      }
    }
  }
}

static int
mixed_basics(AVar *av) {
  Vec<Sym *> basics;
  forv_CreationSet(cs, *av->out) if (cs) {
    Sym *b = to_basic_type(cs->sym->type);
    if (b)
      basics.set_add(b);
  }
  return basics.n > 1;
}

static void
collect_var_type_violations() {
  // collect NOTYPE violations
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      if (!av->var->is_internal && av->out == bottom_type && !is_Sym_OUT(av->var->sym))
        type_violation(ATypeViolation_NOTYPE, av, av->out, 0, 0);
    }
  }
  if (!fa->permit_boxing) {
    // collect BOXING violations
    forv_EntrySet(es, fa->ess) {
      forv_Var(v, es->fun->fa_all_Vars) {
        AVar *av = make_AVar(v, es);
        if (mixed_basics(av)) 
          type_violation(ATypeViolation_BOXING, av, av->out, 0, 0);
      }
    }
    forv_CreationSet(cs, fa->css) {
      forv_AVar(av, cs->vars) {
        if (mixed_basics(av)) 
          type_violation(ATypeViolation_BOXING, av, av->out, 0, 0);
      }
    }
  }
  if (fa->no_unused_instance_variables) {
    forv_CreationSet(cs, fa->css) {
      forv_AVar(av, cs->vars) {
        if (av->out == bottom_type)
          type_violation(ATypeViolation_NOTYPE, av, av->out, 0, 0);
      }
    }
  }
}

void 
initialize_Sym_for_fa(Sym *s) {
  if (s->is_symbol || s->is_fun || s->type_kind)
    s->abstract_type = make_abstract_type(s);
  if (s->is_fun || s->is_pattern || s->type_kind)
    forv_Sym(ss, s->has)
      if (!ss->var)
        ss->var = new Var(ss);
  if (s->type_kind && s->element)
    s->element->var = new Var(s->element);
}

static void
initialize_symbols() {
  forv_Sym(s, fa->pdb->if1->allsyms)
    initialize_Sym_for_fa(s);
}

static void
initialize_primitives() {
  forv_Prim(p, fa->pdb->if1->primitives->prims) {
    p->args.clear();
    int n = p->nargs < 0 ? -p->nargs : p->nargs;
    for (int i = 0; i < n - 1; i++) {
      switch (p->arg_types[i]) {
        case PRIM_TYPE_ALL:             p->args.add(top_type); break;
        case PRIM_TYPE_ANY:             p->args.add(any_type); break;
        case PRIM_TYPE_SYMBOL:          p->args.add(symbol_type); break;
        case PRIM_TYPE_CONT:            p->args.add(make_abstract_type(sym_continuation)); break;
        case PRIM_TYPE_REF:             p->args.add(make_abstract_type(sym_ref)); break;
        case PRIM_TYPE_ANY_NUM_A:       p->args.add(anynum_kind); break;
        case PRIM_TYPE_ANY_NUM_B:       p->args.add(anynum_kind); break;
        case PRIM_TYPE_ANY_INT_A:       p->args.add(anyint_type); break;
        case PRIM_TYPE_ANY_INT_B:       p->args.add(anyint_type); break;
        default: assert(!"case");       break;
      }
    }
  }
}

static void
initialize() {
  if1->callback->finalize_functions();
  element_var = new Var(new_Sym("some element"));
  bottom_type = type_cannonicalize(new AType());
  bottom_type->type = bottom_type;
  void_type = make_abstract_type(sym_void);
  unknown_type = make_abstract_type(sym_unknown);
  any_type = make_abstract_type(sym_any);
  top_type = type_union(any_type, void_type);
  bool_type = make_abstract_type(sym_bool);
  size_type = make_abstract_type(sym_size);
  symbol_type = make_abstract_type(sym_symbol);
  string_type = make_abstract_type(sym_string);
  fun_type = make_abstract_type(sym_function);
  fun_symbol_type = type_union(symbol_type, fun_type);
  anyint_type = make_abstract_type(sym_anyint);
  anynum_kind = make_abstract_type(sym_anynum);
  anyclass_type = make_abstract_type(sym_anyclass);
  null_type = make_abstract_type(sym_null);
  edge_worklist.clear();
  send_worklist.clear();
  initialize_symbols();
  initialize_primitives();
  build_arg_positions(fa);
  build_patterns(fa);
}

static void
initialize_pass() {
  type_violations.clear();
  type_violation_hash.clear();
  entry_set_done.clear();
  refresh_top_edge(fa->top_edge);
  if (fa->method_token)
    fa->method_token->out = make_abstract_type(fa->method_token->var->sym);
  if (fa->setter_token)
    fa->setter_token->out = make_abstract_type(fa->setter_token->var->sym);
}

enum { DFS_white = 0, DFS_grey, DFS_black };

static void
mark_es_backedges(EntrySet *es, Vec<EntrySet *> &nodes) {
  es->dfs_color = DFS_grey;
  forv_AEdge(e, es->out_edges) if (e) {
    if (e->to->dfs_color == DFS_white)
      mark_es_backedges(e->to, nodes);
    else {
      if (e->to->dfs_color == DFS_grey) {
        e->es_backedge = 1;
        e->to->backedges.add(e);
      }
    }
  }
  es->dfs_color = DFS_black;
}

static void
compute_recursive_entry_sets() {
  forv_EntrySet(es, fa->ess)
    es->dfs_color = DFS_white;
  Vec<EntrySet *> nodes;
  nodes.set_add(fa->top_edge->to);
  mark_es_backedges(fa->top_edge->to, nodes);
}

static void mark_es_cs_backedges(CreationSet *cs);
static void mark_es_cs_backedges(EntrySet *es);

static void
mark_es_cs_backedges(CreationSet *cs) {
  cs->dfs_color = DFS_grey;
  forv_EntrySet(es, cs->ess) if (es) {
    if (es->dfs_color == DFS_white)
      mark_es_cs_backedges(es);
    else if (es->dfs_color == DFS_grey)
      es->cs_backedges.add(cs);
  }
  cs->dfs_color = DFS_black;
}

static void
mark_es_cs_backedges(EntrySet *es) {
  es->dfs_color = DFS_grey;
  forv_AEdge(e, es->out_edges) if (e) {
    EntrySet *es_succ = e->to;
    if (es_succ->dfs_color == DFS_white)
      mark_es_cs_backedges(es_succ);
    else if (es_succ->dfs_color == DFS_grey) {
      e->es_cs_backedge = 1;
      es_succ->es_cs_backedges.add(e);
    }
  }
  forv_CreationSet(cs, es->creates) if (cs) {
    if (cs->dfs_color == DFS_white)
      mark_es_cs_backedges(cs);
    else if (cs->dfs_color == DFS_grey)
      cs->es_backedges.add(es);
  }
  es->dfs_color = DFS_black;
}

// recursion amongst EntrySets and the CreationSets 
// created within them, and the EntrySets "created"
// (as in restricted) by those CreationSets
static void
compute_recursive_entry_creation_sets() {
  forv_EntrySet(es, fa->ess)
    es->dfs_color = DFS_white;
  forv_CreationSet(cs, fa->css)
    cs->dfs_color = DFS_white;
  mark_es_cs_backedges(fa->top_edge->to);
}

int
is_es_recursive(EntrySet *es) {
  if (es->split)
    return es->split->backedges.n;
  return es->backedges.n;
}

static int
is_es_recursive(AEdge *e) {
  EntrySet *es = e->from->split ? e->from->split : e->from;
  forv_AEdge(ee, es->backedges)
    if (ee->pnode == e->pnode && ee->fun == e->fun)
      return 1;
  return 0;
}

int
is_es_cs_recursive(EntrySet *es) {
  if (es->split)
    return es->split->es_cs_backedges.n;
  return es->es_cs_backedges.n;
}

static int
is_es_cs_recursive(AEdge *e) {
  EntrySet *es = e->from->split ? e->from->split : e->from;
  forv_AEdge(ee, es->es_cs_backedges)
    if (ee->pnode == e->pnode && ee->fun == e->fun)
      return 1;
  return 0;
}

int
is_es_cs_recursive(CreationSet *cs) {
  if (cs->split)
    return cs->split->es_backedges.n;
  return cs->es_backedges.n;
}

static void
collect_es_type_confluences(Vec<AVar *> &confluences) {
  confluences.clear();
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *xav = make_AVar(v, es);
      for (AVar *av = xav; av; av = av->lvalue)
        forv_AVar(x, av->backward) if (x) {
          if (!x->out->type->n)
            continue;
          if (av->var->sym->clone_for_constants) {
            if (type_diff(av->in, x->out) != bottom_type) {
              confluences.set_add(av);
              break;
            }
          } else {
            if (x->out->type->n && type_diff(av->in->type, x->out->type) != bottom_type) {
              confluences.set_add(av);
              break;
            }
          }
        }
    }
  }
  confluences.set_to_vec();
  qsort_by_id(confluences);
  forv_AVar(x, confluences)
    log(LOG_SPLITTING, "type confluences %s %d\n", 
        x->var->sym->name ? x->var->sym->name : "", x->var->sym->id);
}

static void
collect_es_marked_confluences(Vec<AVar *> &confluences, Accum<AVar *> &acc) {
  confluences.clear();
  forv_AVar(xav, acc.asvec) {
    for (AVar *av = xav; av; av = av->lvalue)
      forv_AVar(x, av->backward) if (x) {
        if (!x->out->n)
          continue;
        if (different_marked_args(x, av, 1)) {
          confluences.set_add(av);
          break;
        }
      }
  }
  confluences.set_to_vec();
  qsort_by_id(confluences);
}

static void
collect_cs_type_confluences(Vec<AVar *> &confluences) {
  confluences.clear();
  forv_CreationSet(cs, fa->css) {
    forv_AVar(av, cs->vars) {
      if (!av->contour_is_entry_set && av->contour != GLOBAL_CONTOUR) {
        forv_AVar(x, av->backward) if (x) {
          if (!x->out->n)
            continue;
          if (av->var->sym->clone_for_constants) {
            if (type_diff(av->in, x->out) != bottom_type) {
              confluences.set_add(av);
              break;
            }
          } else {
            if (type_diff(av->in->type, x->out->type) != bottom_type) {
              confluences.set_add(av);
              break;
            }
          }
        }
      }
    }
  }
  confluences.set_to_vec();
  qsort_by_id(confluences);
}

static void
record_backedges(AEdge *e, EntrySet *es, Map<AEdge *, EntrySet *> &up_map) {
  form_Map(MapElemAEdgeEntrySet, m, up_map) {
    if (m->key->from == es)
      e->to->pending_es_backedge_map.put(get_AEdge(m->key->fun, m->key->pnode, e->to), m->value);
    else
      e->to->pending_es_backedge_map.put(m->key, m->value);
  }
  Vec<AEdge *> *backedges = &es->backedges;
  if (es->split)
    backedges = &es->split->backedges;
  forv_AEdge(ee, *backedges)
    if (ee->from == es)
      e->to->pending_es_backedge_map.put(get_AEdge(ee->fun, ee->pnode, e->to), e->to);
    else
      e->to->pending_es_backedge_map.put(ee, e->to);
}

static int
split_entry_set(AVar *av, int fsetters, int fmark = 0) {
  EntrySet *es = (EntrySet*)av->contour;
  Vec<AEdge *> do_edges;
  int nedges = 0, non_rec_edges = 0;
  AEdge **last = es->edges.last();
  Map<AEdge *, EntrySet *> pending_es_backedge_map;
  for (AEdge **ee = es->edges.first(); ee < last; ee++) if (*ee) {
    if (!(*ee)->args.n) 
      continue;
    nedges++;
    pending_es_backedge_map.map_union((*ee)->from->pending_es_backedge_map);
    if (!fsetters ? is_es_recursive(*ee) : is_es_cs_recursive(*ee))
      continue;
    non_rec_edges++;
    if (!fsetters) {
      if (!edge_type_compatible_with_entry_set(*ee, es, fmark))
        do_edges.add(*ee);
    } else
      if (!edge_sset_compatible_with_entry_set(*ee, es))
        do_edges.add(*ee);
  } 
  if (non_rec_edges == 1 && nedges != do_edges.n) 
    return 0;
  int first = do_edges.n == nedges ? 1 : 0;
  int split = 0;
  for (int i = first; i < do_edges.n; i++) {
    AEdge *e = do_edges.v[i];
    e->to = 0;
    e->filtered_args.clear();
    es->edges.del(e);
    make_entry_set(e, es);
    if (e->to != es) {
      record_backedges(e, es, pending_es_backedge_map);
      split = 1;
      log(LOG_SPLITTING, "SPLIT ES %d %s%s%s %d from %d -> %d\n", 
          es->id,
          fsetters ? "setters " : "",
          fmark ? "marks " : "",
          es->fun->sym->name ? es->fun->sym->name : "", es->fun->sym->id,
          e->pnode->lvals.v[0]->sym->id, e->to->id);
    }
  }
  return split;
}

static void
build_type_mark(AVar *av, CreationSet *cs, Vec<AVar *> &avset, int mark = 1) {
  Sym *tt = cs->sym->type;
  int m = av->mark_map ? av->mark_map->get(tt) : 0;
  if (!m) {
    if (!av->out->type->set_in(cs->sym->type->abstract_type->v[0]))
      return;
    if (!av->mark_map)
      av->mark_map = new MarkMap;
    av->mark_map->put(tt, mark);
  } else if (m > mark)
    av->mark_map->put(tt, mark);
  else if (m <= mark)
    return;
  forv_AVar(y, av->forward) if (y)
    if (avset.set_in(y))
      build_type_mark(y, cs, avset, mark + 1);
}

// To handle recursion, mark value*AVar distances from the nearest 
// AVar generating the value.  Dataflow is considered to be only
// from lower to higher distances for the purpose of splitting.
static void
build_type_marks(AVar *av, Accum<AVar *> &acc) {
  // collect all contributing nodes
  acc.add(av);
  forv_AVar(x, acc.asvec) {
    forv_AVar(y, x->backward) if (y)
      acc.add(y);
  }
  forv_AVar(x, acc.asvec) {
    forv_AVar(y, x->forward) if (y)
      acc.add(y);
  }
  // mark them
  forv_AVar(x, acc.asvec) {
    if (x->gen)
      forv_CreationSet(s, *x->gen) if (s) 
        build_type_mark(x, s, acc.asset);
  }
}

static void
build_setter_mark(AVar *av, AVar *x, Vec<AVar *> &avset, int mark = 1) {
  int m = av->mark_map ? av->mark_map->get(x) : 0;
  if (!m) {
    if (!av->setters->set_in(x))
      return;
    if (!av->mark_map)
      av->mark_map = new MarkMap;
    av->mark_map->put(x, mark);
  } else if (m > mark)
    av->mark_map->put(x, mark);
  else if (m <= mark)
    return;
  forv_AVar(y, av->backward) if (y)
    build_setter_mark(y, x, avset, mark + 1);
}

static void
build_setter_marks(AVar *av, Accum<AVar *> &acc) {
  // collect all contributing nodes
  acc.add(av);
  forv_AVar(x, acc.asvec) {
    forv_AVar(y, x->forward) 
      if (y && y->setters && y->setters->some_intersection(*av->setters))
        acc.add(y);
  }
  forv_AVar(x, acc.asvec) {
    forv_AVar(y, x->backward) 
      if (y && y->setters && y->setters->some_intersection(*av->setters))
        acc.add(y);
  }
  // mark them
  forv_AVar(x, acc.asvec) {
    if (x->setters->in(x))
      build_setter_mark(x, x, acc.asset);
  }
}

static void
clear_marks(Accum <AVar *> &acc) {
  forv_AVar(x, acc.asvec)
    x->mark_map = 0;
}

static void
clear_avar(AVar *av) {
  av->gen = 0;
  av->in = bottom_type;
  av->out = bottom_type;
  av->setters = 0;
  av->setter_class = 0;
  av->restrict = 0;
  av->backward.clear();
  av->forward.clear();
  av->arg_of_send.clear();
  av->mark_map = 0;
  if (av->lvalue)
    clear_avar(av->lvalue);
}

static void
clear_var(Var *v) {
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key)
    clear_avar(v->avars.v[i].value);
}

static void
clear_edge(AEdge *e) {
  e->es_backedge = 0;
  e->es_cs_backedge = 0;
  e->args.clear();
  e->rets.clear();
  e->match->formal_filters.clear();
  form_MPositionAVar(x, e->filtered_args)
    clear_avar(x->value);
}

static void
clear_es(EntrySet *es) {
  AEdge **last = es->edges.last();
  for (AEdge **ee = es->edges.first(); ee < last; ee++) if (*ee)
    clear_edge(*ee);
  es->out_edges.clear();
  es->backedges.clear();
  es->cs_backedges.clear();
  es->creates.clear();
}

static void
clear_cs(CreationSet *cs) {
  cs->ess.clear();      
  cs->es_backedges.clear();
  forv_AVar(v, cs->vars)
    clear_avar(v);
}

template <class F> void
foreach_var() {
  F::F(element_var);
  forv_Sym(s, fa->pdb->if1->allsyms)
    if (s->var)
      F::F(s->var);
  forv_Fun(f, fa->funs)
    forv_Var(v, f->fa_all_Vars)
      F::F(v);
}

struct ClearVarFn { static void F(Var *v) { 
  clear_var(v); 
} };

static void 
clear_results() {
  foreach_var<ClearVarFn>();
  forv_CreationSet(cs, fa->css)
    clear_cs(cs);
  forv_EntrySet(es, fa->ess)
    clear_es(es);
  for (int i = 0; i < cannonical_setters.n; i++)
    forc_List(Setters *, x, cannonical_setters.v[i].value)
      x->car->eq_classes = NULL;
  if (fa->method_token)
    clear_var(fa->method_token->var);
  if (fa->setter_token)
    clear_var(fa->setter_token->var);
}

static Setters *
setters_cannonicalize(Setters *s) {
  assert(!s->sorted.n);
  forv_AVar(x, *s)
    if (x)
      s->sorted.add(x);
  if (s->sorted.n > 1)
    qsort_pointers((void**)&s->sorted.v[0], (void**)s->sorted.end());
  uint h = 0;
  for (int i = 0; i < s->sorted.n; i++)
    h = (uint)(intptr_t)s->sorted.v[i] * open_hash_multipliers[i % 256];
  s->hash = h ? h : h + 1; // 0 is empty
  Setters *ss = cannonical_setters.put(s);
  if (!ss) ss = s;
  return ss;
}

#if 0
// Eventual Optimization
static SettersClasses *
setters_classes_cannonicalize(SettersClasses *s) {
  assert(!s->sorted.n);
  forv_Setters(x, *s) if (x) s->sorted.add(x);
  if (s->sorted.n > 1)
    qsort_pointers((void**)&s->sorted.v[0], (void**)s->sorted.end());
  uint h = 0;
  for (int i = 0; i < s->sorted.n; i++)
    h = (uint)s->sorted.v[i] * open_hash_multipliers[i % 256];
  s->hash = h ? h : h + 1; // 0 is empty
  SettersClasses *ss = cannonical_setters_classes.put(s);
  if (!ss) ss = s;
  return ss;
}
#endif

static int
update_setter(AVar *av, AVar *s) {
  Setters *new_setters = 0;
  if (av->setters) {
    if (av->setters->in(s))
      return 0;
    new_setters = av->setters->add_map.get(s);
    if (new_setters)
      goto Ldone;
  }
  new_setters = new Setters;
  if (av->setters)
    new_setters->copy(*av->setters);
  new_setters->add(s);
  new_setters = setters_cannonicalize(new_setters);
  if (av->setters)
    av->setters->add_map.put(s, new_setters);
 Ldone:
  av->setters = new_setters;
  forv_AVar(x, av->backward) if (x)
    update_setter(x, s);
  return 1;
}

static void
collect_cs_marked_confluences(Vec<AVar *> &confluences) {
  confluences.clear();
  forv_CreationSet(cs, fa->css) {
    forv_AVar(av, cs->vars) {
      forv_AVar(x, av->backward) if (x) {
        if (!x->mark_map)
          continue;
        if (!av->contour_is_entry_set && av->contour != GLOBAL_CONTOUR) {
          if (different_marked_args(av, x, 1)) {
            confluences.set_add(av);
            break;
          }
        }
      }
    }
  }
  confluences.set_to_vec();
  qsort_by_id(confluences);
}

static void
split_eq_class(Setters *eq_class, Vec<AVar *> &diff) {
  Setters *diff_class = new Setters, *remaining_class = new Setters;
  diff_class->set_union(diff);
  diff_class = setters_cannonicalize(diff_class);
  eq_class->set_difference(diff, *remaining_class);
  remaining_class = setters_cannonicalize(remaining_class);
  forv_AVar(x, *diff_class) if (x)
    x->setter_class = diff_class;
  forv_AVar(x, *remaining_class) if (x)
    x->setter_class = remaining_class;
}

// AVar->setter_class is the smallest set of setter AVars which
// are equivalent (have the same ->out and equivalent ->setters)
// On a new partition of setters this function recomputes the equiv sets
static void
recompute_eq_classes(Vec<Setters *> &ss) {
  forv_Setters(s, ss) {
    // build new class for unclassed setters
    Setters *new_s = NULL;
    forv_AVar(v, *s) if (v)
      if (!v->setter_class) {
        if (!new_s)
          new_s = new Setters;
        new_s->set_add(v);
      }
    if (new_s) {
      new_s = setters_cannonicalize(new_s);
      forv_AVar(v, *new_s) if (v)
        v->setter_class = new_s;
      // reparition existing classes
      forv_AVar(v, *s) if (v) {
        if (v->setter_class != new_s) {
          Vec<AVar *> diff;
          v->setter_class->set_difference(*s, diff);
          split_eq_class(v->setter_class, diff);
        }
      }
    }
  }
}

enum AKind { AKIND_TYPE, AKIND_SETTER, AKIND_MARK };

static int
analyze_confluence(AVar *av, int akind = AKIND_TYPE) {
  int setters_changed = 0;
  Vec<Setters *> ss;
  Vec<AVar *> *dir = akind == AKIND_SETTER ? &av->forward : &av->backward;
  forv_AVar(x, *dir) if (x) {
    assert(x->contour_is_entry_set);
    if (akind == AKIND_TYPE && !x->out->type->n) continue;
    if (akind == AKIND_MARK && !x->mark_map) continue;
    for (int i = 0; i < ss.n; i++) {
      forv_AVar(a, *ss.v[i]) if (a) {
        if ((akind == AKIND_TYPE && 
             (!a->out->type->n || !x->out->type->n || a->out->type == x->out->type)) || 
            (akind == AKIND_SETTER && same_eq_classes(a->setters, x->setters)) ||
            (akind == AKIND_MARK && !different_marked_args(x, a, 1))) 
        {
          ss.v[i]->set_add(x);
          goto Ldone;
        }
      }
    }
    ss.add(new Setters);
    ss.v[ss.n - 1]->set_add(x);
  Ldone:;
  }
  for (int i = 0; i < ss.n; i++)
    ss.v[i] = setters_cannonicalize(ss.v[i]);
  recompute_eq_classes(ss);
  forv_AVar(x, *dir) if (x && x->setter_class)
    setters_changed = update_setter(x->container, x) || setters_changed;
  return setters_changed;
}

static void
collect_setter_confluences(Vec<AVar *> &setter_confluences, Vec<AVar *> &setter_starters) {
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *xav = make_AVar(v, es);
      for (AVar *av = xav; av; av = av->lvalue)
        if (av->setters) {
          forv_AVar(x, av->forward) if (x) {
            if (x->setters && !same_eq_classes(av->setters, x->setters)) {
              setter_confluences.set_add(av);
              break;
            }
          }
          if (av->creation_set) {
            forv_AVar(s, *av->setters) if (s) {
              assert(s->setter_class);
              if (s->container->out->in(av->creation_set))
                setter_starters.set_add(av);
            }
          }
        }
    }
  }
  setter_confluences.set_to_vec();
  qsort_by_id(setter_confluences);
  setter_starters.set_to_vec();
  qsort_by_id(setter_starters);
}

static int
split_with_setter_marks(AVar *av) {
  Accum<AVar *> acc;
  build_setter_marks(av, acc);
  Vec<AVar *> confluences;
  collect_es_marked_confluences(confluences, acc);
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!av->is_lvalue) {
        AVar *aav = unique_AVar(av->var, av->contour);
        if (is_return_value(aav)) {
          if (split_entry_set(aav, 0, 1))
            analyze_again = 1;
        }
      } else {
        if (av->var->is_formal) {
          if (split_entry_set(av, 0, 1))
            analyze_again = 1;
        }
      }
    }
  }
  clear_marks(acc);
  return analyze_again;
}

static int
split_ess_setters(Vec<AVar *> &confluences) {
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!av->is_lvalue) {
        if (is_return_value(av)) {
          if (split_entry_set(av, 1))
            analyze_again = 1;
        } else if (!analyze_again)
          if (split_with_setter_marks(av))
            analyze_again = 1;
      } else {
        AVar *aav = unique_AVar(av->var, av->contour);
        if (aav->var->is_formal) {
          if (split_entry_set(aav, 1))
            analyze_again = 1;
        } else if (!analyze_again)
          if (split_with_setter_marks(av))
            analyze_again = 1;
      }
    } else if (!analyze_again)
      if (split_with_setter_marks(av))
        analyze_again = 1;
  }
  return analyze_again;
}

static int
split_css(Vec<AVar *> &astarters) {
  int analyze_again = 0;
  // build starter sets, groups of starters for the same CreationSet
  Vec<AVar *> starters;
  forv_AVar(av, astarters)
    starters.add(av);
  while (starters.n) {
    CreationSet *cs = starters.v[0]->creation_set;
    Vec<AVar *> save;
    Vec<AVar *> starter_set;
    forv_AVar(av, starters) {
      if (av->creation_set == cs)
        starter_set.add(av);    
      else
        save.add(av);
    }
    starters.move(save);
    while (starter_set.n > 1) {
      AVar *av = starter_set.v[0];
      Vec<AVar *> compatible_set;
      forv_AVar(v, starter_set) {
        if (same_eq_classes(v->setters, av->setters))
          compatible_set.set_add(v);
        else
          save.add(v);
      }
      starter_set.move(save);
      Vec<AVar *> new_defs;
      cs->defs.set_difference(compatible_set, new_defs);
      if (new_defs.n) {
        cs->defs.move(new_defs);
        cs->defs.set_to_vec();
        CreationSet *new_cs = new CreationSet(cs);
        new_cs->defs.move(compatible_set);
        new_cs->defs.set_to_vec();
        forv_AVar(v, new_cs->defs)
          v->creation_set = new_cs;
        new_cs->split = cs;
        analyze_again = 1;
        log(LOG_SPLITTING, "SPLIT CS %d %s %d -> %d\n", cs->id,
            cs->sym->name ? cs->sym->name : "", cs->sym->id, 
            new_cs->id);
      }
    }
  }
  return analyze_again;
}

static int
split_for_setters() {
  Vec<AVar *> setter_confluences, setter_starters;
  collect_setter_confluences(setter_confluences, setter_starters);
  if (split_ess_setters(setter_confluences))
    return 1;
  if (split_css(setter_starters))
    return 1;
  return 0;
}

static int
split_with_type_marks(AVar *av) {
  Accum<AVar *> acc;
  build_type_marks(av, acc);
  Vec<AVar *> confluences;
  collect_es_marked_confluences(confluences, acc);
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!av->is_lvalue) {
        if (av->var->is_formal) {
          if (split_entry_set(av, 0, 1))
            analyze_again = 1;
        }
      } else {
        AVar *aav = unique_AVar(av->var, av->contour);
        if (is_return_value(aav)) {
          if (split_entry_set(aav, 0, 1))
            analyze_again = 1;
        }
      }
    }
  }
  if (!analyze_again) {
    collect_cs_marked_confluences(confluences);
    forv_AVar(av, confluences)
      analyze_confluence(av, AKIND_MARK);
    if (split_for_setters())
      analyze_again = 1;
  }
  clear_marks(acc);
  return analyze_again;
}

static void
collect_cs_setter_confluences(Vec<AVar *> &setters_confluences) {
  setters_confluences.clear();
  forv_CreationSet(cs, fa->css) {
    forv_AVar(av, cs->vars) {
      forv_AVar(x, av->forward) if (x) {
        if (!av->contour_is_entry_set && av->contour != GLOBAL_CONTOUR) {
          if (!same_eq_classes(av->setters, x->setters)) {
            setters_confluences.set_add(av);
            break;
          }
        }
      }
    }
    if (cs->added_element_var) {
      AVar *av = get_element_avar(cs);
      forv_AVar(x, av->forward) if (x) {
        if (!av->contour_is_entry_set && av->contour != GLOBAL_CONTOUR) {
          if (!same_eq_classes(av->setters, x->setters)) {
            setters_confluences.set_add(av);
            break;
          }
        }
      }
    }
  }
  setters_confluences.set_to_vec();
}

static int
split_ess_type(Vec<AVar *> &confluences) {
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!av->is_lvalue) {
        if (av->var->is_formal)
          analyze_again = split_entry_set(av, 0) || analyze_again; 
      } else {
        AVar *aav = unique_AVar(av->var, av->contour);
        if (is_return_value(aav))
          analyze_again = split_entry_set(aav, 0) || analyze_again; 
      }
    }
    if (!analyze_again)
      analyze_again = split_with_type_marks(av) || analyze_again;
  }
  return analyze_again;
}

static void
clear_splits() {
  forv_EntrySet(es, fa->ess) 
    es->split = 0;
  forv_CreationSet(cs, fa->css) 
    cs->split = 0;
}

static int
extend_analysis() {
  int analyze_again = 0;
  compute_recursive_entry_sets();
  compute_recursive_entry_creation_sets();
  clear_splits();
  Vec<AVar *> confluences;
  // 1) split EntrySet(s) based on type
  collect_es_type_confluences(confluences);
  analyze_again = split_ess_type(confluences);
  if (!analyze_again) {
    collect_cs_type_confluences(confluences);
    forv_AVar(av, confluences)
      analyze_confluence(av, AKIND_TYPE);
    if (split_for_setters())
      analyze_again = 1;
  }
  while (!analyze_again) {
    // 2) compute setters for ivar imprecisions
    collect_cs_setter_confluences(confluences);
    int progress = 0;
    forv_AVar(av, confluences)
      progress = analyze_confluence(av, AKIND_SETTER) || progress;
    // 3) stop if no progress
    if (!progress) break;
    // 4) split EntrySet(s) and CreationSet(s) for setter imprecisions
    if (split_for_setters()) {
      analyze_again = 1;
      break;
    }
  }
  if (analyze_again) {
    ++analysis_pass;
    if (verbose_level) printf("extending analysis %d\n", analysis_pass);
    log(LOG_SPLITTING, "======== pass %d ========\n", analysis_pass);
    clear_results();
    return 1;
  }
  return 0;
}

struct SetVoidFn { static void F(Var *v) { 
  CreationSet *s = void_type->v[0];
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    if (av->out->in(s))
      av->out = void_type;
  }
} };

static void
set_void_lub_types_to_void() {
  foreach_var<SetVoidFn>();
}

struct RemoveNullFn { static void F(Var *v) { 
  CreationSet *s = null_type->v[0];
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    if (av->out != s->sym->abstract_type && av->out->in(s))
      av->out = type_diff(av->out, s->sym->abstract_type);
  }
} };

static void
remove_null_types() {
  foreach_var<RemoveNullFn>();
}

static void
complete_pass() {
  collect_results();
  collect_argument_type_violations();
  collect_var_type_violations();
}

int
FA::analyze(Fun *top) {
  ::fa = this;
  initialize();
  top_edge = make_top_edge(top);
  do {
    initialize_pass();
    edge_worklist.enqueue(top_edge);
    while (edge_worklist.head || send_worklist.head) {
      while (AEdge *e = edge_worklist.pop()) {
        e->in_edge_worklist = 0;
        analyze_edge(e);
      }
      while (AVar *send = send_worklist.pop()) {
        send->in_send_worklist = 0;
        add_send_edges_pnode(send->var->def, (EntrySet*)send->contour);
      }
    }
    complete_pass();
  } while (extend_analysis());
  set_void_lub_types_to_void();
  remove_null_types();
  if (fanalysis_errors)
    if1->callback->report_analysis_errors(type_violations);
  show_violations(fa, stderr);
  return type_violations.n ? -1 : 0;
}

static Var *
info_var(IFAAST *a, Sym *s) {
  if (!s)
    s = a->symbol();
  if (!s)
    return 0;
  if (a && a->pnodes.n) {
    forv_PNode(n, a->pnodes) {
      forv_Var(v, n->lvals)
        if (v->sym == s)
          return v;
      forv_Var(v, n->lvals)
        if (v->sym == s)
          return v;
      forv_Var(v, n->rvals)
        if (v->sym == s)
          return v;
    }
  }
  if (s->var)
    return s->var;
  return 0;
}

// Given an IFAAST node and a Sym, find the Sym which
// corresponds to the concrete (post-cloning) type of the
// variable corresponding to the Sym at that IFAAST node.
Sym *
type_info(IFAAST *a, Sym *s) {
  Var *v = info_var(a, s);
  if (v)
    return v->type;
  return 0;
}

// Given a function and an IFAAST node, return the set of
// functions which could be called from that IFAAST node.
void
call_info(Fun *f, IFAAST *a, Vec<Fun *> &funs) {
  funs.clear();
  forv_PNode(n, a->pnodes) {
    Vec<Fun *> *ff = f->calls.get(n);
    if (ff)
      funs.set_union(*ff);
  }     
  funs.set_to_vec();
}

// Given a variable return the vector of constants
// which that variable could take on.
// Returns 0 for no constants or non-constant (e.g. some integer).
int
constant_info(Var *v, Vec<Sym *> &constants) {
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    forv_CreationSet(cs, *av->out) if (cs) {
      if (cs->sym->constant)
        constants.set_add(cs->sym);
      else {
        constants.clear();
        return 0;
      }
    }
  }
  constants.set_to_vec();
  return constants.n;
}

// Given an IFAAST node and a Sym, find the set of
// constants which could arrive at that point.
// make sure that there is not some dominating
// non-constant type.
int
constant_info(IFAAST *a, Vec<Sym *> &constants, Sym *s) {
  constants.clear();
  Var *v = info_var(a, s);
  if (v)
    return constant_info(v, constants);
  return 0;
}

int
symbol_info(Var *v, Vec<Sym *> &symbols) {
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    forv_CreationSet(cs, *av->out) if (cs) {
      if (cs->sym->is_symbol)
        symbols.set_add(cs->sym);
      else {
        symbols.clear();
        return 0;
      }
    }
  }
  symbols.set_to_vec();
  return symbols.n;
}
