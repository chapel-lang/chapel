/* -*-Mode: c++;-*-
 Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

static FA *fa = 0;

static AType *bottom_type = 0;
static AType *top_type = 0;
static AType *bool_type = 0;
static AType *size_type = 0;
static AType *anyint_type = 0;
static AType *anynum_type = 0;
static AType *fun_type = 0;
static AType *symbol_type = 0;
static AType *fun_symbol_type = 0;

typedef Vec<Map<char *,Vec<Fun *> *> *> TupleDispatchTable;
static Map<char *,Vec<Fun *> *> class_dispatch_table;
static Map<char *,Vec<Fun *> *> self_dispatch_table;
static Map<char *, TupleDispatchTable *> tuple_dispatch_table;
static OpenHash<AType *, ATypeOpenHashFns> cannonical_atypes;
static OpenHash<Setters *, SettersHashFns> cannonical_setters;
static OpenHash<SettersClasses *, SettersClassesHashFns> cannonical_setters_classes;
static OpenHash<ATypeFold *, ATypeFoldOpenHashFns> type_fold_cache;

static Var *element_var = 0;

static SList(AEdge, edge_worklist_link) edge_worklist;
static SList(AVar, send_worklist_link) send_worklist;
static Vec<EntrySet *> entry_set_done;
static Vec<ATypeViolation *> type_violations;

static AType *type_union(AType *a, AType *b);
static AType *type_diff(AType *a, AType *b);
static AType *type_intersection(AType *a, AType *b);
static AType *type_cannonicalize(AType *t);
static AType *make_AType(CreationSet *cs);

AVar::AVar(Var *v, void *acontour) : 
  var(v), contour(acontour), lvalue(0), in(bottom_type), out(bottom_type), restrict(top_type), 
  container(0), setters(0), setter_class(0), creation_set(0), in_send_worklist(0), 
  contour_is_entry_set(0)
{
}

AType::AType(AType &a) {
  hash = 0;
  top = 0;
  this->copy(a);
}

AType::AType(CreationSet *cs) {
  hash = 0;
  top = 0;
  set_add(cs);
}

static AVar *
unique_AVar(Var *v, void *contour) {
  assert(contour);
  AVar *av = v->avars.get(contour);
  if (av)
    return av;
  av = new AVar(v, contour);
  v->avars.put(contour, av);
  return av;
}

static AVar *
unique_AVar(Var *v, EntrySet *es) {
  assert(es);
  AVar *av = v->avars.get(es);
  if (av)
    return av;
  av = new AVar(v, es);
  v->avars.put(es, av);
  av->contour_is_entry_set = 1;
  if (v->sym->lvalue) {
    av->lvalue = new AVar(new Var(v->sym), es);
    av->lvalue->contour_is_entry_set = 1;
  }
  return av;
}

CreationSet::CreationSet(CreationSet *cs) {
  sym = cs->sym;
  clone_for_constants = cs->clone_for_constants;
  atype = NULL;
  forv_AVar(v, cs->vars) {
    AVar *iv = unique_AVar(v->var, this);
    if (iv->var->sym->name)
      var_map.put(iv->var->sym->name, iv);
  }
  equiv = 0;
  type = 0;
}

static EntrySet *
find_nesting_EntrySet(Fun *fn, EntrySet *e) {
  assert(!"incomplete");
  return (EntrySet*)GLOBAL_CONTOUR;
}

AVar *
make_AVar(Var *v, EntrySet *es) {
  if (v->sym->constant || v->sym->symbol || v->sym->in == es->fun->sym)
    return unique_AVar(v, es);
  if (!v->sym->in || v->sym->in->module || v->sym->type_kind)
    return unique_AVar(v, GLOBAL_CONTOUR);
  return unique_AVar(v, find_nesting_EntrySet(v->sym->in->fun, es));
}

static inline AVar *
make_AVar(Var *v, AEdge *e) { 
  return make_AVar(v, e->to); 
}

static AType *
make_AType(CreationSet *cs) {
  if (cs->atype)
    return cs->atype;
  return cs->atype = type_cannonicalize(new AType(cs));
}

static AType *
make_abstract_type(Sym *s) {
  s = unalias_type(s);
  AType *a = s->abstract_type;
  if (a)
    return a;
  CreationSet *cs = new CreationSet(s);
  return s->abstract_type = make_AType(cs);
}

static AType *
make_AType(Vec<CreationSet *> &css) {
  AType *t = new AType();
  forv_CreationSet(cs, css)
    t->set_add(cs);
  return type_cannonicalize(t);
}

AType *
AType::constants() {
  AType *t = new AType();
  forv_CreationSet(cs, *this)
    if (cs->sym->constant)
      t->set_add(cs);
  return type_cannonicalize(t);
}

static AType *
make_singular_AType(Sym *s, Var *v) {
  assert(s->symbol || s->fun || s->constant); // otherwise use make_AType(Var *v) above
  if (v->as_AType)
    return v->as_AType;
  CreationSet *cs = new CreationSet(s);
  return v->as_AType = make_AType(cs);
}

static void
update_in(AVar *v, AType *t) {
  AType *tt = type_union(v->in, t);
  if (tt != v->in) {
    assert(tt != top_type);
    v->in = tt;
    tt = type_intersection(v->in, v->restrict);
    if (tt != v->out) {
      assert(tt != top_type);
      v->out = tt;
      forv_AVar(vv, v->arg_of_send) if (vv) {
	if (!vv->in_send_worklist) {
	  vv->in_send_worklist = 1;
	  send_worklist.push(vv);
	}
      }
      forv_AVar(vv, v->forward) if (vv)
	update_in(vv, tt);
    }
  }
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

static void
flow_vars_equal(AVar *v, AVar *vv) {
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
    } else {
      assert(0);
    }
  }
}

static void
flow_vars(AVar *v, AVar *vv) {
  if (vv->lvalue && v->lvalue) {
    flow_var_to_var(v, vv);
    flow_var_to_var(vv->lvalue, v->lvalue);
  } else
    flow_var_to_var(v, vv);
}

// Initially create a unique creation set for each
// variable (location in program text).
static CreationSet *
creation_point(AVar *v, Sym *s) {
  CreationSet *cs = v->creation_set;
  if (cs)
    goto Ldone;
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
    cs->vars.add(iv);
    if (h->name)
      cs->var_map.put(h->name, iv);
  }
 Lfound:
  if (v->contour != GLOBAL_CONTOUR)
    cs->defs.add(v);
  v->creation_set = cs;
 Ldone:
  if (v->contour_is_entry_set)
    ((EntrySet*)v->contour)->creates.set_add(cs);
  update_in(v, make_AType(v->creation_set));
  return v->creation_set;
}

//  all float combos become doubles
//  all signed/unsigned combos become signed
//  all int combos below 32 bits become signed 32 bits, above become signed 64 bits
Sym *
coerce_num(Sym *a, Sym *b) {
  if (a->num_type == b->num_type) {
    if (a->num_index > b->num_index)
      return a;
    else
      return b;
  }
  if (b->num_type == IF1_NUM_TYPE_FLOAT) {
    Sym *t = b; b = a; a = t;
  }
  if (a->num_type == IF1_NUM_TYPE_FLOAT) {
    if (int_type_precision[b->num_type] <= float_type_precision[a->num_type])
      return a;
    if (int_type_precision[b->num_type] >= 32)
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
  return sym_int8;
}

AType *
type_num_fold(Prim *p, AType *a, AType *b) {
  (void) p; p = 0; // for now
  a = type_intersection(a, anynum_type);
  b = type_intersection(b, anynum_type);
  if (a->n == 1 && b->n == 1)
    return type_union(a->v[0]->sym->type->abstract_type, b->v[0]->sym->type->abstract_type)->top;
  ATypeFold f(p, a, b), *ff;
  if ((ff = type_fold_cache.get(&f)))
    return ff->result;
  AType *r = new AType();
  forv_CreationSet(cs, *a) if (cs)
    r->set_add(cs->sym->type->abstract_type->v[0]);
  forv_CreationSet(cs, *b) if (cs)
    r->set_add(cs->sym->type->abstract_type->v[0]);
  r = type_cannonicalize(r)->top;
  type_fold_cache.put(new ATypeFold(p, a, b, r));
  return r;
}

static void
qsort_pointers(void **left, void **right) {
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
    void  **i = left, **j = right - 1, **x = left + ((right - left) / 2);
    do {
      while (i <= x && *i < *x) i++;
      while (j >= x && *x < *j) j--;
      if (i < j) {
	void *t = *i;
	*i = *j;
	*j = t;
	i++; j--;
	continue;
      }
      break;
    } while (1);
    if (left < j) qsort_pointers(left, j + 1);
    if (i < right) qsort_pointers(i, right);
  }
}

#define NO_TOP ((Sym *)-1)
static AType *
type_cannonicalize(AType *t) {
  assert(!t->sorted.n);
  assert(!t->union_map.n);
  assert(!t->intersection_map.n);
  int nconsts = 0, rebuild = 0;
  forv_CreationSet(cs, *t) if (cs) {
    // strip out constants if the base type is included
    if (cs->sym->type->num_type && cs->sym != cs->sym->type) {
      CreationSet *base_cs = cs->sym->type->abstract_type->v[0];
      if (t->set_in(base_cs)) {
	rebuild = 1;
	continue;
      }
      nconsts++;
    }
    t->sorted.add(cs);
  }
  if (nconsts > num_constants_per_variable) {
    // compress constants into the base type
    rebuild = 1;
    for (int i = 0; i < t->sorted.n; i++)
      if (t->sorted.v[i]->sym->type->num_type && t->sorted.v[i]->sym != t->sorted.v[i]->sym->type) {
	CreationSet *base_cs = t->sorted.v[i]->sym->type->abstract_type->v[0];
	if (!t->set_in(base_cs)) {
	  t->sorted.v[i] = base_cs;
	} else {
	  t->sorted.v[i] = t->sorted.v[t->sorted.n - 1];
	  t->sorted.n--;
	}
      }
  }
  if (rebuild) {
    t->clear();
    t->set_union(t->sorted);
  }
  if (t->sorted.n > 1)
    qsort_pointers((void**)&t->sorted.v[0], (void**)t->sorted.end());
  unsigned int h = 0;
  for (int i = 0; i < t->sorted.n; i++)
    h = (uint)t->sorted.v[i] * open_hash_multipliers[i % 256];
  t->hash = h ? h : h + 1; // 0 is empty
  AType *tt = cannonical_atypes.put(t);
  if (!tt) tt = t;
  if (tt == t) {
    if (tt->sorted.n < 2)
      tt->top = tt;
    else {
      Sym *s = NO_TOP;
      forv_CreationSet(cs, tt->sorted) {
	if (s == NO_TOP)
	  s = cs->sym;
	if (s == cs->sym)
	  continue;
	else if (!cs->defs.n) {
	  if (s->type->num_type && cs->sym->type->num_type)
	    s = coerce_num(s->type, cs->sym);
	  else
	    goto Ldone;
	} else {
	Ldone:
	  tt->top = top_type;
	  goto Ltop_done;
	}
      }
      if (s == NO_TOP)
	tt->top = bottom_type;
      else
	tt->top = s->abstract_type;
    }
  Ltop_done:;
  }
  return tt;
}
#undef NO_TOP

static AType *
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
subtype_of(Sym *a, Sym *b) {
  if (a->constant) {
    if (a == b)
      return 1;
    else
      return b->subtypes.set_in(a->type) != 0;
  }
  else
    return b->subtypes.set_in(a) != 0;
}

static AType *
type_diff(AType *a, AType *b) {
  AType *r = new AType();
  forv_CreationSet(aa, *a) if (aa) {
    if (aa->defs.n && b->set_in(aa))
      continue;
    forv_CreationSet(bb, *b) if (bb && !bb->defs.n) {
      if (subtype_of(aa->sym, bb->sym))
	goto Lnext;
    }
    r->set_add(aa);
  Lnext:;
  }
  r = type_cannonicalize(r);
  return r;
}

static AType *
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
	  if (subtype_of(aa->sym, bb->sym)) {
	    r->set_add(aa);
	    goto Lnexta;
	  }
	}
      } else {
	if (bb->defs.n) {
	  if (subtype_of(bb->sym, aa->sym))
	    r->set_add(bb);
	} else {
	  if (subtype_of(aa->sym, bb->sym)) {
	    r->set_add(aa);
	    goto Lnexta;
	  } else if (subtype_of(bb->sym, aa->sym))
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

static int
entry_set_compatibility(AEdge *e, EntrySet *es) {
  int val = INT_MAX;
  assert(es->args.n == e->args.n); // FIXME
  for (int i = 0; i < es->args.n; i++) {
    AType *t = type_intersection(e->args.v[i]->out, e->dispatch_filters.v[i]);
    if (es->args.v[i]->out != t) {
      val -= 4;
      break;
    }
  }
  int n = es->rets.n;
  if (e->rets.n < n) n = e->rets.n;
  for (int i = 0; i < n; i++) {
    if (es->rets.v[i]->setter_class != e->rets.v[i]->setter_class) {
      val -= 2;
      break;
    }
  }
  if (e->fun->clone_for_constants) {
    for (int i = 0; i < es->args.n; i++) {
      if (es->args.v[i]->var->clone_for_constants) {
	Vec<CreationSet *> css;
	// stupid C++ type system
	((Vec<CreationSet*> *)es->args.v[i]->out)->set_disjunction(
	  *((Vec<CreationSet*> *)e->args.v[i]->out), css);
	forv_CreationSet(cs, css)
	  if (cs->sym->constant) {
	    val -= 1;
	    goto Lconst_set_diff;
	  }
      }
    }
  Lconst_set_diff:;
  }
  return val;
}

static void
set_entry_set(AEdge *e, EntrySet *es = 0) {
  EntrySet *new_es = es;
  if (!es) {
    new_es = new EntrySet(e->fun);
    e->fun->ess.add(new_es);
  }
  e->to = new_es;
  new_es->edges.put(e);
  if (!es) {
    forv_Sym(a, e->fun->sym->args) {
      if (!a->var)
	a->var = new Var(a);
      new_es->args.add(make_AVar(a->var, new_es));
      new_es->args.v[new_es->args.n - 1]->restrict = bottom_type;
    }
  }
}

static EntrySet *
find_best_entry_set(AEdge *e, EntrySet *split) {
  EntrySet *es = NULL;
  int val = -1;
  forv_EntrySet(x, e->fun->ess) {
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
check_split(AEdge *e) {
  if (e->from && e->from->split) {
    Map<Fun *, AEdge *> *m = e->from->split->out_edge_map.get(e->pnode);
    if (m) {
      for (int i = 0; i < m->n; i++)
	if (m->v[i].key == e->fun) {
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
make_entry_set(AEdge *e, EntrySet *split = 0) {
  if (e->to) return;
  if (check_split(e)) return;
  if (check_es_db(e)) return;
  EntrySet *es = find_best_entry_set(e, split);
  set_entry_set(e, es);
  if (!es)
    e->to->split = split;
}

static void
flow_var_type_permit(AVar *v, AType *t) {
  v->restrict = type_union(t, v->restrict);
  v->out = type_intersection(v->in, v->restrict);
  assert(v->out != top_type);
}

static inline void
flow_var_type_permit(AVar *v, Sym *s) { 
  flow_var_type_permit(v, make_abstract_type(s)); 
}

static void
add_var_constraints(EntrySet *es) {
  Fun *f = es->fun;
  forv_Var(v, f->fa_Vars) {
    AVar *av = make_AVar(v, es);
    if (v->sym->type && !v->sym->pattern) {
      if (!v->sym->external)
	av->restrict = v->sym->type->abstract_type;
      else
	update_in(av, v->sym->type->abstract_type);
      if (v->sym->constant) // for constants, the abstract type is the concrete type
	update_in(av, make_singular_AType(v->sym, v));
      if (v->sym->symbol || v->sym->fun) 
	update_in(av, make_singular_AType(v->sym, v));
      else if (v->sym->type_kind != Type_NONE)
	creation_point(av, unalias_type(v->sym)); // this is somewhat more powerful than is required...
    }
  }
}

static void
fill_rets(EntrySet *es, int n) {
  assert(n == 1);
  es->rets.fill(n);
  for (int i = 0; i < n; i++)
    if (!es->rets.v[i])
      es->rets.v[i] = make_AVar(es->fun->sym->ret->var, es);
}

static void
set_container(AVar *av, AVar *container) {
  assert(!av->container || av->container == container);
  av->container = container;
  if (av->lvalue)
    av->lvalue->container = container;
}

static void
prim_make(PNode *p, EntrySet *es, Sym *kind, int start = 1, int ref = 0) {
  AVar *container = make_AVar(p->lvals.v[0], es);
  CreationSet *cs = creation_point(container, kind);
  cs->vars.fill(p->rvals.n - start);
  int l = p->rvals.n - start;
  p->tvals.fill(l);
  for (int i = 0; i < l; i++) {
    Var *v = p->rvals.v[start + i];
    AVar *av = make_AVar(v, es);
    if (!p->tvals.v[i]) {
      Sym *s = if1_alloc_sym(fa->pdb->if1);
      s->lvalue = v->sym->lvalue;
      s->in = es->fun->sym;
      p->tvals.v[i] = new Var(s);
      es->fun->fa_all_Vars.add(p->tvals.v[i]);
    }
    Var *tv = p->tvals.v[i];
    if (!cs->vars.v[i])
      cs->vars.v[i] = unique_AVar(v, cs);
    AVar *iv = cs->vars.v[i];
    AVar *atv = make_AVar(tv, es);
    set_container(atv, container);
    flow_vars(av, atv);
    if (!ref && !v->sym->lvalue)
      flow_vars(atv, iv);
    else
      flow_vars_equal(atv, iv);
    if (iv->var->sym->name)
      cs->var_map.put(iv->var->sym->name, iv);
  }
}

static AVar *
get_element_avar(CreationSet *cs) {
  AVar *elem = unique_AVar(element_var, cs);
  if (!cs->added_element_var) {
    cs->added_element_var = 1;
    cs->vars.add(elem);
  }
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
      Sym *s = if1_alloc_sym(fa->pdb->if1);
      assert(!e->var->sym->lvalue);
      s->in = es->fun->sym;
      Var *v = new Var(s);
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
	e->arg_of_send.set_add(container);
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

// for send nodes, add simple constraints which do not depend 
// on the computed types (compare to add_send_edgse_pnodes)
static void
add_send_constraints(EntrySet *es) {
  forv_PNode(p, es->fun->fa_send_PNodes) {
    if (!p->prim) { // normal send
      AVar *res = make_AVar(p->lvals.v[0], es);
      forv_Var(v, p->rvals) 
	make_AVar(v, es)->arg_of_send.set_add(res);
    } else { // primitive
      int start = (p->rvals.v[0]->sym == sym_primitive) ? 1 : 0;
      // return constraints
      assert(p->lvals.n < 2);
      for (int i = 0; i < p->lvals.n; i++) {
	switch (p->prim->ret_types[i]) {
	  case PRIM_TYPE_ANY: break;
	  case PRIM_TYPE_BOOL: update_in(make_AVar(p->lvals.v[i], es), bool_type); break;
	  case PRIM_TYPE_SIZE: update_in(make_AVar(p->lvals.v[i], es), size_type); break;
	  case PRIM_TYPE_ANY_NUM_AB:
	  case PRIM_TYPE_A: {
	    for (int j = start; j < p->rvals.n; j++)
	      if (j - start != p->prim->pos) 
		make_AVar(p->rvals.v[j], es)->arg_of_send.set_add(make_AVar(p->lvals.v[0], es));
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
	case P_prim_vector:
	  prim_make_vector(p, es);
	  break;
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
    for (int i = 0; i < p->rvals.n; i++)
      flow_vars(make_AVar(p->rvals.v[i], es), make_AVar(p->lvals.v[i], es));
  }
}

static AEdge *
make_AEdge(Fun *f, PNode *p, EntrySet *from) {
  Map<Fun *, AEdge *> *m = from->out_edge_map.get(p);
  if (!m)
    from->out_edge_map.put(p, (m = new Map<Fun *, AEdge *>()));
  AEdge *e = m->get(f);
  if (!e) {
    m->put(f, (e = new AEdge()));
    e->fun = f;
    e->pnode = p;
    e->from = from;
  }
  from->out_edges.set_add(e);
  return e;
}

static int
approx_check_pattern(AVar *a, Sym *b, AVar *result) {
  a->arg_of_send.set_add(result);
  if (!b->type)
    return 1;
  AType *t = make_abstract_type(b->type);
  if (t == top_type)
    return 1;
  t = type_intersection(a->out, t);
  if (t == bottom_type)
    return 0;
  if (b->pattern) {
    forv_CreationSet(cs, *t) {
      if (cs->vars.n != b->has.n)
	continue;
      for (int i = 0; i < b->has.n; i++)
	if (b->has.v[i]->type) 
	  if (!approx_check_pattern(cs->vars.v[i], b->has.v[i], result))
	    goto Lcontinue;
      return 1;
    Lcontinue:;
    }
  } else
    return 1;
  return 0;
}

static void
add_funs_constraints(PNode *p, EntrySet *es, Vec<Fun *> *fns, Vec<AVar *> &a, Vec<AVar *> *ret = 0) {
  if (fns) {
    AVar *result = make_AVar(p->lvals.v[0], es);
    forv_Fun(f, *fns) {
      Vec<Sym *> *fargs = &f->sym->args;
      if (a.n == fargs->n) {
	for (int j = 0; j < a.n; j++) {
	  if (fargs->v[j]->type && 
	      (type_intersection(a.v[j]->out, make_abstract_type(fargs->v[j]->type)) == bottom_type))
	    goto Lnext;
	  if (fargs->v[j]->pattern && !approx_check_pattern(a.v[j], fargs->v[j], result))
	    goto Lnext;
	}
	AEdge *ee = make_AEdge(f, p, es);
	if (!ee->args.n)
	  ee->args.copy(a);
	if (!ee->rets.n) {
	  if (!ret)
	    for (int i = 0; i < p->lvals.n; i++)
	      ee->rets.add(make_AVar(p->lvals.v[i], ee->from));
	  else
	    ee->rets.copy(*ret);
	}
	if (!ee->in_edge_worklist) {
	  ee->in_edge_worklist = 1;
	  edge_worklist.push(ee);
	}
      }
    Lnext:;
    }
  }
}

static int 
special_dispatch(Vec<AVar *> &a) {
  return a.n == 2 && 
    a.v[0]->out->n == 1 && a.v[0]->out->v[0]->sym->symbol &&
    a.v[1]->out->n == 1 && a.v[1]->out->v[0]->sym == sym_tuple;
}

static int 
special_dispatch(Vec<Sym *> &a) {
  return a.n == 2 && a.v[0]->type->symbol && a.v[1]->type == sym_tuple;
}

static int application_constraints(PNode *p, EntrySet *es, AVar *fun, CreationSet *s, Vec<AVar *> &args);

static int
function_application(PNode *p, EntrySet *es, AVar *fun, CreationSet *s, Vec<AVar *> &args) {
  Sym *fs = s->sym;
  Fun *f = fs->fun;
  if (fs->args.n - 1 == args.n) {
    AEdge *ee = make_AEdge(f, p, es);
    if (!ee->args.n) {
      ee->args.add(fun);
      for (int j = args.n - 1; j >= 0; j--)
	ee->args.add(args.v[j]);
    }
    if (!ee->rets.n)
      for (int i = 0; i < p->lvals.n; i++)
	ee->rets.add(make_AVar(p->lvals.v[i], ee->from));
    if (!ee->in_edge_worklist) {
      ee->in_edge_worklist = 1;
      edge_worklist.push(ee);
    }
  } else if (fs->args.n - 1 > args.n)
    return 1;
  return 0;
}

static int
partial_application(PNode *p, EntrySet *es, CreationSet *s, Vec<AVar *> &args) {
  int incomplete = 0;
  // partial application... recurse
  AVar *result = make_AVar(p->lvals.v[0], es);
  PNode *partial_application = s->defs.v[0]->var->def;
  void *contour = s->defs.v[0]->contour;
  if (partial_application->prim->index == P_prim_apply) {
    AVar *av = unique_AVar(partial_application->rvals.v[2], contour);
    av->arg_of_send.set_add(result);
    args.add(av);
  } else { // standard application
    for (int i = 1; i < partial_application->rvals.n; i++) {
      AVar *av = make_AVar(partial_application->rvals.v[i], es);
      av->arg_of_send.set_add(result);
      args.add(av);
    }
  }
  AVar *fun = unique_AVar(partial_application->rvals.v[0], contour);
  forv_CreationSet(cs, *fun->out) if (cs)
    incomplete = application_constraints(p, es, fun, cs, args) || incomplete;
  return incomplete;
}

static int
function_dispatch(PNode *p, EntrySet *es, AVar *a0, CreationSet *s, Vec<AVar *> &args) {
  Vec<AVar *> a;
  int some = 0;
  a.add(a0);
  for (int j = args.n - 1; j >= 0; j--)
    a.add(args.v[j]);
  if (special_dispatch(a)) {
    // specialized dispatch: symbol(tuple)
    TupleDispatchTable *at = tuple_dispatch_table.get(a.v[0]->out->v[0]->sym->name);
    if (at) {
      CreationSet *tuple = a.v[1]->out->v[0];
      for(int i = 0; i < tuple->vars.n; i++) {
	if (i >= at->n)
	  break;
	forv_CreationSet(cs, *tuple->vars.v[i]->out) if (cs) {
	  if (cs->sym->symbol) {
	    Vec<Fun *> *fns = at->v[i]->get(cs->sym->name);
	    some = 1;
	    add_funs_constraints(p, es, fns, a);
	  }
	}
      }
    }
    if (some)
      return 0;
  } 
  // standard dispatch, on unique class (symbols, bare classes), and objects
  forv_CreationSet(cs, *a0->out) if (cs) {
    Vec<Fun *> *fns = 0;
    if (!cs->defs.n)
      fns = class_dispatch_table.get(cs->sym->name);
    else
      fns = self_dispatch_table.get(cs->sym->name);
    add_funs_constraints(p, es, fns, a);
  }
  return 0;
}

static int
application_constraints(PNode *p, EntrySet *es, AVar *a0, CreationSet *cs, Vec<AVar *> &args) {
  if (sym_function->subtypes.set_in(cs->sym)) {
    if (!cs->defs.n)
      return function_application(p, es, a0, cs, args);
    else
      return partial_application(p, es, cs, args);
  }
  return function_dispatch(p, es, a0, cs, args);
}

static void
type_violation(AVar *av, AType *type, PNode *pnode) {
  if (!av->violations)
    av->violations = new ViolationMap;
  ATypeViolation *v = av->violations->get(pnode);
  if (!v)
    av->violations->put(pnode, (v = new ATypeViolation(av, type, pnode)));
  else
    v->type = type_union(v->type, type);
  type_violations.set_add(v);
}

// for send nodes, add call edges and more complex constraints
// which depend on the computed types (compare to add_send_constraints)
static void
add_send_edges_pnode(PNode *p, EntrySet *es, int initial = 0) {
  if (!p->prim) {
    assert(p->lvals.n == 1);
    int incomplete = 0;
    Vec<AVar *> args;
    for (int i = p->rvals.n - 1; i > 0; i--)
      args.add(make_AVar(p->rvals.v[i], es));
    AVar *a0 = make_AVar(p->rvals.v[0], es);
    forv_CreationSet(cs, *a0->out) if (cs)
      incomplete = application_constraints(p, es, a0, cs, args) || incomplete;
    if (incomplete)
      creation_point(make_AVar(p->lvals.v[0], es), sym_function);
  } else {
    // argument and return constraints
    int n = p->prim->nargs < 0 ? -p->prim->nargs : p->prim->nargs;
    AVar *a = 0, *b = 0;
    int start = 0, iarg = 0;
    if (p->rvals.v[0]->sym == sym_primitive)
      start = 1;
    for (int i = start; i < p->rvals.n; i++) {
      if (i - start == p->prim->pos) continue;
      AVar *arg = make_AVar(p->rvals.v[i], es);
      // record violations
      if (type_diff(arg->out, p->prim->args.v[iarg]) != bottom_type)
	type_violation(arg, type_diff(arg->out, p->prim->args.v[iarg]), p); 
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
      if (p->prim->ret_types[i] == PRIM_TYPE_ANY_NUM_AB)
	update_in(make_AVar(p->lvals.v[i], es), type_num_fold(p->prim, a->out, b->out));
      if (p->prim->ret_types[i] == PRIM_TYPE_A)
	flow_vars(a, make_AVar(p->lvals.v[i], es));
    }
    // specifics
    switch (p->prim->index) {
      default: break;
      case P_prim_print: {
	AVar *ret = make_AVar(p->lvals.v[0], es);
	update_in(ret, make_abstract_type(sym_int));
	break;
      }
      case P_prim_vector:
	prim_make_vector(p, es);
	break;
      case P_prim_index: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *vec = make_AVar(p->rvals.v[1], es);
	Sym *index = p->rvals.v[2]->sym;
	vec->arg_of_send.set_add(result);
	set_container(result, vec);
	forv_CreationSet(cs, *vec->out) if (cs) {
	  if (cs->sym == sym_tuple) {
	    int i;
	    if (index->type && index->imm_int(&i) == 0 && i >= 0 && i < cs->vars.n)
	      flow_vars_equal(cs->vars.v[i], result);
	    else { // assume the worst
	      for (i = 0; i < cs->vars.n; i++)
		flow_vars_equal(cs->vars.v[i], result);
	    }
	  } else {
	    AVar *elem = get_element_avar(cs);
	    flow_vars_equal(elem, result);
	  }
	}
	break;
      }
      case P_prim_apply: {
	assert(p->lvals.n == 1);
	int incomplete = 0;
	Vec<AVar *> args;
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *fun = make_AVar(p->rvals.v[1], es);
	AVar *a1 = make_AVar(p->rvals.v[3], es);
	fun->arg_of_send.set_add(result);
	a1->arg_of_send.set_add(result);
	args.add(a1);
	forv_CreationSet(f, *fun->out) if (f)
	  incomplete = application_constraints(p, es, fun, f, args) || incomplete;
	if (incomplete)
	  creation_point(make_AVar(p->lvals.v[0], es), sym_function);
	break;
      }
      case P_prim_period: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *obj = make_AVar(p->rvals.v[1], es);
	AVar *selector = make_AVar(p->rvals.v[3], es);
	obj->arg_of_send.set_add(result);
	selector->arg_of_send.set_add(result);
	set_container(result, obj);
	forv_CreationSet(sel, *selector->out) if (sel) {
	  char *symbol = sel->sym->name; assert(symbol);
	  forv_CreationSet(cs, *obj->out) if (cs) {
	    AVar *iv = cs->var_map.get(symbol);
	    if (iv)
	      flow_vars_equal(iv, result);
	  }
	}
	break;
      }
      case P_prim_assign: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *ref = make_AVar(p->rvals.v[1], es);
	AVar *val = make_AVar(p->rvals.v[3], es);
	forv_CreationSet(cs, *ref->out) if (cs) {
	  AVar *av = cs->vars.v[0];
	  flow_vars(val, av);
	}
	flow_vars_equal(val, result);
	break;
      }
      case P_prim_deref: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *ref = make_AVar(p->rvals.v[2], es);
	ref->arg_of_send.set_add(result);
	set_container(result, ref);
	forv_CreationSet(cs, *ref->out) if (cs) {
	  AVar *av = cs->vars.v[0];
	  flow_vars_equal(av, result);
	}
	break;
      }
      case P_prim_new: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *thing = make_AVar(p->rvals.v[1], es);
	forv_CreationSet(cs, *thing->out) if (cs)
	  creation_point(result, cs->sym);
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
  forv_PNode(p, e->fun->fa_send_PNodes)
    add_send_edges_pnode(p, e->to, 1);
}

static inline int
is_fa_Var(Var *v) { 
  return v->sym->type || v->sym->aspect || v->sym->constant || v->sym->symbol;
}

static void
collect_Vars_PNodes(Fun *f) {
  f->fa_collected = 1;
  if (!f->entry)
    return;
  f->collect_Vars(f->fa_all_Vars, &f->fa_all_PNodes);
  forv_Var(v, f->fa_all_Vars)
    if (is_fa_Var(v))
      f->fa_Vars.add(v);
  Primitives *prim = f->pdb->if1->primitives;
  forv_PNode(p, f->fa_all_PNodes) {
    if (p->code->kind == Code_MOVE)
      f->fa_move_PNodes.add(p);
    f->fa_phi_PNodes.append(p->phi);
    f->fa_phy_PNodes.append(p->phy);
    if (p->code->kind == Code_SEND) {
      p->prim = prim->find(p);
      f->fa_send_PNodes.add(p);
      if (p->code && p->code->ast)
	p->code->ast->prim = p->prim;
    }
  }
}

// a is an actual argument, b is a formal argument
static void
pattern_match(AVar *a, AVar *b, EntrySet *es) {
  forv_CreationSet(cs, *a->out) if (cs) {
    Sym *s = b->var->sym;
    if (s->type && s->type != cs->sym)
      continue;
    if (s->pattern) {
      if (cs->vars.n < s->has.n) // must have enough to cover args
	continue;
      for (int i = 0; i < s->has.n; i++)
	if (s->has.v[i]->var) // if used
	  flow_vars(cs->vars.v[i], make_AVar(s->has.v[i]->var, es));
    }
  }
}

static AType *
check_pattern(AVar *a, Sym *b) {
  if (!b->type)
    return top_type;
  AType *bt = make_abstract_type(b->type); 
  if (bt == top_type) 
    return a->out;
  AType *t = type_intersection(a->out, bt);
  if (t == bottom_type)
    return 0;
  Vec<CreationSet *> css;
  if (b->pattern) {
    forv_CreationSet(cs, *t) {
      if (cs->vars.n != b->has.n)
	continue;
      for (int i = 0; i < b->has.n; i++)
	if (b->has.v[i]->type) 
	  if (!check_pattern(cs->vars.v[i], b->has.v[i]->type))
	    goto Lcontinue;
      css.add(cs);
    Lcontinue:;
    }
    if (css.n)
      return make_AType(css);
    else
      return 0;
  } else
    return t;
}

static void
analyze_edge(AEdge *e) {
  // compute restricted types valid for this dispatch
  for (int i = 0; i < e->args.n; i++) {
    if (e->fun->sym->args.v[i]->pattern) {
      AType *dispatch_filter = check_pattern(e->args.v[i], e->fun->sym->args.v[i]);
      if (!dispatch_filter)
	return;
      e->dispatch_filters.add(dispatch_filter);
    } else
      e->dispatch_filters.add(top_type);
  }
  make_entry_set(e);
  assert(e->args.n <= e->to->args.n);
  for (int i = 0; i < e->args.n; i++) {
    AVar *a = e->args.v[i], *b = e->to->args.v[i];
    flow_var_type_permit(b, e->dispatch_filters.v[i]);
    flow_vars(a, b);
    if (b->var->sym->pattern)
      pattern_match(a, b, e->to);
  }
  forv_AType(at, e->dispatch_filters)
    forv_CreationSet(cs, *at)
      cs->ess.set_add(e->to);
  creation_point(make_AVar(e->fun->sym->cont->var, e->to), sym_continuation);
  for (int i = 0; i < e->pnode->lvals.n; i++) {
    fill_rets(e->to, e->pnode->lvals.n);
    flow_vars(e->to->rets.v[i], e->rets.v[i]);
  }
  if (!entry_set_done.set_in(e->to)) {
    entry_set_done.set_add(e->to);
    if (!e->fun->fa_collected)
      collect_Vars_PNodes(e->fun);
    add_var_constraints(e->to);
    add_move_constraints(e->to);
    add_send_constraints(e->to);
    add_send_edges(e);
  }
}

static AEdge *
make_top_edge(Fun *top) {
  AEdge *e = new AEdge();
  e->fun = top;
  e->pnode = new PNode();
  make_entry_set(e);
  return e;
}

static inline int
is_formal_argument(AVar *av) {
  forv_AVar(v, av->backward) if (v) {
    if (v->arg_of_send.n)
      return 1;
  }
  return 0;
}

static inline int
is_return_value(AVar *av) {
  EntrySet *es = (EntrySet*)av->contour;
  forv_AVar(v, es->rets)
    if (v == av)
      return 1;
  return 0;
}

#include "fa_extras.cpp"

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
  fa->ess_set.move(entry_set_done);
  // collect css and css_set
  fa->css.clear();
  fa->css_set.clear();
  forv_Fun(f, fa->funs)
    forv_Var(v, f->fa_all_Vars)
      for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].value) 
	fa->css_set.set_union(*v->avars.v[i].value->out);
  forv_CreationSet(cs, fa->css_set) if (cs) 
    fa->css.add(cs);
  // print results
  if (verbose_level)
    fa_dump_types(fa, stdout);
}

// for each call site, check that all args are covered
static void
collect_type_violations() {
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      forv_AVar(c, av->arg_of_send) if (c) {
	PNode *p = c->var->def;
	if (p->prim) continue; // primitives handled elsewhere
	EntrySet *from = (EntrySet*)c->contour;
	AType *t = av->out;
	forv_AVar(a, av->forward) if (a) {
	  if (!a->contour_is_entry_set)
	    continue;
	  EntrySet *xes = (EntrySet*)a->contour;
	  AEdge **last = xes->edges.last();
	  for (AEdge **x = xes->edges.first(); x < last; x++) 
	    if (*x && (*x)->pnode == p && (*x)->from == from)
	      goto Lfound;
	  continue;
	Lfound:
	  t = type_diff(t, a->out);
	}
	if (t != bottom_type)
	  type_violation(av, t, p);
      }
    }
  }
}

static void
complete_pass() {
  collect_results();
  collect_type_violations();
}

static void
build_dispatch_table() {
  class_dispatch_table.clear();
  self_dispatch_table.clear();
  forv_Fun(f, fa->pdb->funs) {
    Sym *a = f->sym->args.v[0];
    if (special_dispatch(f->sym->args)) {
      int some = 0;
      TupleDispatchTable *at = tuple_dispatch_table.get(a->type->name);
      if (!at)
	tuple_dispatch_table.put(a->type->name, (at = new TupleDispatchTable));
      for (int i = 0; i < f->sym->args.v[1]->has.n; i++) {
	Sym *v = f->sym->args.v[1]->has.v[i];
	if (v->type->symbol) {
	  at->fill(i + 1);
	  if (!at->v[i])
	    at->v[i] = new Map<char *, Vec<Fun *> *>;
	  Vec<Fun *> *vf = at->v[i]->get(v->type->name);
	  if (!vf)
	    at->v[i]->put(v->type->name, (vf = new Vec<Fun *>));
	  vf->add(f);
	  some = 1;
	}
      }
    }
    if (a->type) {
      Map<char *,Vec<Fun *> *> *table = 
	f->sym->class_static ? &class_dispatch_table : &self_dispatch_table;
      Vec<Fun *> *vf = table->get(a->type->name);
      if (!vf) 
	table->put(a->type->name, (vf = new Vec<Fun *>));
      vf->add(f);
    }
  }
}

static void
initialize_symbols() {
  forv_Sym(s, fa->pdb->if1->allsyms) {
    // everything is a subtype of itself
    s->subtypes.set_add(s);
    // symbols (selectors) have all applicable functions as subtypes
    if (s->symbol) {
      s->abstract_type = make_abstract_type(s);
      s->subtypes.set_add(s);
      sym_symbol->subtypes.set_add(s);
      sym_any->subtypes.set_add(s);
    }
    // functions are subtypes of the initial symbol in their pattern
    if (s->fun && s->args.n && s->args.v[0]->symbol)
      s->args.v[0]->subtypes.set_add(s);
    if (s->fun && s->args.n) {
      Sym *a0 = s->args.v[0];
      if (a0->type && a0->type->symbol)
	a0->type->subtypes.set_add(s);
    }
    if (s->fun) {
      sym_function->subtypes.set_add(s);
      sym_any->subtypes.set_add(s);
    }
    if (s->type_kind) {
      s->abstract_type = make_abstract_type(s);
      sym_any->subtypes.set_add(s);
    }
    if (s->type) {
      if (s->type_kind)
	s->type->subtypes.set_add(s);
    }
    forv_Sym(ss, s->implements)
      ss->subtypes.set_add(s);
    forv_Sym(ss, s->has)
      if (!ss->var)
	ss->var = new Var(ss);
  }
  forv_Sym(s, fa->pdb->if1->allsyms) {
    if (s->constraints.n) {
      for (int i = 0; i < s->constraints.n; i++) {
	if (!i && s->type_kind == Type_UNKNOWN) {
	  s->subtypes.copy(s->constraints.v[i]->subtypes);
	  continue;
	}
	s->subtypes.set_intersection(s->constraints.v[i]->subtypes);
      }
    }
  }
}

static void
initialize_primitives() {
  forv_Prim(p, fa->pdb->if1->primitives->prims) {
    int n = p->nargs < 0 ? -p->nargs : p->nargs;
    for (int i = 0; i < n - 1; i++) {
      switch (p->arg_types[i]) {
	case PRIM_TYPE_ANY:		p->args.add(top_type); break;
	case PRIM_TYPE_SYMBOL:		p->args.add(symbol_type); break;
	case PRIM_TYPE_CONT:		p->args.add(make_abstract_type(sym_continuation)); break;
	case PRIM_TYPE_REF:		p->args.add(make_abstract_type(sym_ref)); break;
	case PRIM_TYPE_ANY_NUM_A:	p->args.add(anynum_type); break;
	case PRIM_TYPE_ANY_NUM_B:	p->args.add(anynum_type); break;
	case PRIM_TYPE_ANY_INT_A:	p->args.add(anyint_type); break;
	case PRIM_TYPE_ANY_INT_B:	p->args.add(anyint_type); break;
	default: assert(!"case");	break;
      }
    }
  }
}

static void
initialize() {
  element_var = new Var(if1_alloc_sym(
    fa->pdb->if1, if1_cannonicalize_string(fa->pdb->if1, "some element")));
  bottom_type = type_cannonicalize(new AType());
  top_type = make_abstract_type(sym_any);
  bool_type = make_abstract_type(sym_bool);
  size_type = make_abstract_type(sym_size);
  symbol_type = make_abstract_type(sym_symbol);
  fun_type = make_abstract_type(sym_function);
  fun_symbol_type = type_union(symbol_type, fun_type);
  anyint_type = make_abstract_type(sym_anyint);
  anynum_type = make_abstract_type(sym_anynum);
  edge_worklist.clear();
  send_worklist.clear();
  initialize_symbols();
  initialize_primitives();
  build_dispatch_table();
}

static void
initialize_pass() {
  type_violations.clear();
  entry_set_done.clear();
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
  forv_EntrySet(es, cs->ess) {
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
      es_succ->backedges.add(e);
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

static int
is_es_recursive(EntrySet *es) {
  if (es->split)
    return es->split->backedges.n;
  return es->backedges.n;
}

static int
is_es_cs_recursive(EntrySet *es) {
  if (es->split)
    return es->split->backedges.n;
  return es->backedges.n;
}

// make sure you take into account splits here!  what matters is the pre-split es/cs
static int
is_es_cs_recursive(CreationSet *cs) {
  if (cs->split)
    return cs->es_backedges.n;
  return cs->es_backedges.n;
}

static void
collect_es_type_confluences(Vec<AVar *> &type_confluences) {
  type_confluences.clear();
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      forv_AVar(x, av->backward) if (x) {
	if (type_diff(av->in, x->out) != bottom_type) {
	  type_confluences.set_add(av);
	  break;
	}
      }
    }
  }
  type_confluences.set_to_vec();
}

static void
collect_cs_type_confluences(Vec<AVar *> &type_confluences) {
  type_confluences.clear();
  forv_CreationSet(cs, fa->css) {
    forv_AVar(av, cs->vars) {
      forv_AVar(x, av->backward) if (x) {
	if (!av->contour_is_entry_set && av->contour != GLOBAL_CONTOUR) {
	  if (type_diff(av->in, x->out) != bottom_type) {
	    type_confluences.set_add(av);
	    break;
	  }
	}
      }
    }
  }
  type_confluences.set_to_vec();
}

static int
edge_type_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  assert(es->args.n == e->args.n); // FIXME
  for (int i = 0; i < es->args.n; i++)
    if (es->args.v[i]->out != e->args.v[i]->out)
      return 0;
  return 1;
}

static int
edge_sset_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  int n = es->rets.n;
  if (e->rets.n < n) n = e->rets.n;
  for (int i = 0; i < n; i++)
    if (es->rets.v[i]->setter_class != e->rets.v[i]->setter_class)
      return 0;
  return 1;
}

static int
split_entry_set(AVar *av, int fsset = 0) {
  EntrySet *es = (EntrySet*)av->contour;
  if (!fsset ? is_es_recursive(es) : is_es_cs_recursive(es))
    return 0;
  Vec<AEdge *> do_edges;
  int nedges = 0;
  AEdge **last = es->edges.last();
  for (AEdge **ee = es->edges.first(); ee < last; ee++) if (*ee) {
    nedges++;
    if (!fsset) {
      if (!edge_type_compatible_with_entry_set(*ee, es))
	do_edges.add(*ee);
    } else
      if (!edge_sset_compatible_with_entry_set(*ee, es))
	do_edges.add(*ee);
  }
  int first = do_edges.n == nedges ? 1 : 0;
  int split = 0;
  for (int i = first; i < do_edges.n; i++) {
    AEdge *e = do_edges.v[i];
    e->to = 0;
    es->edges.del(e);
    make_entry_set(e, es);
    if (e->to != es)
      split = 1;
  }
  return split;
}

static int
split_ess(Vec<AVar *> &confluences, int fsset = 0) {
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!fsset ? is_formal_argument(av) : is_return_value(av)) {
	if (split_entry_set(av, fsset))
	  analyze_again = 1;
      }
    }
  }
  return analyze_again;
}

static void
clear_avar(AVar *av) {
  av->in = bottom_type;
  av->out = bottom_type;
  av->setters = 0;
  av->setter_class = 0;
  av->restrict = top_type;
  av->backward.clear();
  av->forward.clear();
  av->arg_of_send.clear();
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
  e->dispatch_filters.clear();
}

static void
clear_es(EntrySet *es) {
  AEdge **last = es->edges.last();
  for (AEdge **ee = es->edges.first(); ee < last; ee++) if (*ee)
    clear_edge(*ee);
  es->out_edges.clear();
  forv_AVar(v, es->args)
    v->restrict = bottom_type;
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

static void 
clear_results() {
  clear_var(element_var);
  forv_Sym(s, fa->pdb->if1->allsyms) {
    if (s->var)
      clear_var(s->var);
    if (s->creators.n)
      forv_CreationSet(cs, s->creators)
	clear_cs(cs);
  }
  forv_Fun(f, fa->funs) {
    forv_Var(v, f->fa_all_Vars)
      clear_var(v);
    forv_Sym(v, f->sym->args)
      if (v->var) clear_var(v->var);
    if (f->sym->ret->var)
      clear_var(f->sym->ret->var);
    clear_var(f->sym->ret->var);
    clear_var(f->sym->cont->var);
  }
  forv_Fun(f, fa->funs)
    forv_EntrySet(es, f->ess) if (es)
      clear_es(es);
  for (int i = 0; i < cannonical_setters.n; i++)
    forc_List(Setters *, x, cannonical_setters.v[i].value)
      x->car->eq_classes = NULL;
}

static Setters *
setters_cannonicalize(Setters *s) {
  assert(!s->sorted.n);
  forv_AVar(x, *s) if (x) s->sorted.add(x);
  if (s->sorted.n > 1)
    qsort_pointers((void**)&s->sorted.v[0], (void**)s->sorted.end());
  uint h = 0;
  for (int i = 0; i < s->sorted.n; i++)
    h = (uint)s->sorted.v[i] * open_hash_multipliers[i % 256];
  s->hash = h ? h : h + 1; // 0 is empty
  Setters *ss = cannonical_setters.put(s);
  if (!ss) ss = s;
  return ss;
}

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

static int 
same_eq_classes(Setters *s, Setters *ss) {
  if (s == ss)
    return 1;
  if (!s || !ss)
    return 0;
  return s->eq_classes == ss->eq_classes;
}

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
  }
  setters_confluences.set_to_vec();
}

static void
collect_setter_confluences(Vec<AVar *> &setter_confluences, Vec<AVar *> &setter_starters) {
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      if (av->setters) {
	forv_AVar(x, av->forward) if (x) {
	  if (x->setters && !same_eq_classes(av->setters, x->setters)) {
	    setter_confluences.set_add(av);
	    break;
	  }
	}
	if (av->creation_set) {
	  forv_AVar(s, *av->setters) if (s) {
	    if (s->container->out->in(av->creation_set))
	      setter_starters.add(av);
	  }
	}
      }
    }
  }
  setter_confluences.set_to_vec();
}

static int
split_css(Vec<AVar *> &starters) {
  int analyze_again = 0;
  while (starters.n) {
    // build starter sets, groups of starters for the same CreationSet
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
    if (is_es_cs_recursive(cs))
      continue;
    // for each element of the starter set, find all compatible starters
    // and split off a new CreationSet for them
    while (starter_set.n) {
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
	CreationSet *new_cs = new CreationSet(cs);
	new_cs->defs.move(compatible_set);
	forv_AVar(v, new_cs->defs)
	  v->creation_set = new_cs;
	new_cs->split = cs;
	analyze_again = 1;
      }
    }
  }
  return analyze_again;
}

static int
split_for_setters() {
  Vec<AVar *> setter_confluences, setter_starters;
  collect_setter_confluences(setter_confluences, setter_starters);
  if (split_ess(setter_confluences, 1))
    return 1;
  if (split_css(setter_starters))
    return 1;
  return 0;
}

static void
split_eq_class(Setters *eq_class, Vec<AVar *> &diff) {
  Setters *new_class = new Setters, *old_class = new Setters;
  new_class->copy(diff);
  new_class->set_difference(*eq_class, diff);
  new_class = setters_cannonicalize(new_class);
  eq_class->set_difference(*new_class, diff);
  old_class->copy(diff);
  old_class = setters_cannonicalize(old_class);
  forv_AVar(x, *new_class) if (x)
    x->setter_class = new_class;
  forv_AVar(x, *old_class) if (x)
    x->setter_class = old_class;
}

static void
recompute_eq_classes(Vec<Setters *> &ss) {
  forv_Setters(s, ss) {
    SettersClasses *new_sc = s->eq_classes;
    if (!s->eq_classes)
      new_sc = new SettersClasses;
    forv_AVar(v, *s) if (v) {
      if (v->setter_class) {
	if (!s->eq_classes)
	  new_sc->set_add(v->setter_class);
      }  else {
	assert(!s->eq_classes || s->eq_classes->in(s));
	v->setter_class = s;
	if (!s->eq_classes)
	  new_sc->set_add(s);
      }
      if (!s->eq_classes) {
	new_sc = setters_classes_cannonicalize(new_sc);
	s->eq_classes = new_sc;
	new_sc->used_by.put(s);
      }
    }
    int changed = 0;
    forv_Setters(x, *s->eq_classes) if (x) {
      Vec<AVar *> diff;
      x->set_difference(*s, diff);
      if (diff.n) {
	split_eq_class(x, diff);
	changed = 1;
      }
    }
    if (changed) {
      new_sc = new SettersClasses;
      forv_AVar(v, *s) if (v)
	new_sc->set_add(v->setter_class);
      new_sc = setters_classes_cannonicalize(new_sc);
      s->eq_classes->used_by.del(s);
      s->eq_classes = new_sc;
      new_sc->used_by.put(s);
    }
  }
}

static int
analyze_confluence(AVar *av, int fsetter = 0) {
  int setters_changed = 0;
  Vec<Setters *> ss;
  forv_AVar(x, av->backward) if (x) {
    assert(x->contour_is_entry_set);
    for (int i = 0; i < ss.n; i++) {
      forv_AVar(a, *ss.v[i]) {
	if ((!fsetter && a->out == x->out) || 
	    (fsetter && same_eq_classes(a->setters, x->setters))) {
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
  forv_AVar(x, av->backward) if (x)
    setters_changed = update_setter(x->container, x) || setters_changed;
  return setters_changed;
}

static int
extend_analysis() {
  // initialize
  int analyze_again = 0;
  compute_recursive_entry_sets();
  compute_recursive_entry_creation_sets();
  forv_EntrySet(es, fa->ess) 
    es->split = 0;
  forv_CreationSet(cs, fa->css) 
    cs->split = 0;
  Vec<AVar *> confluences;
  // 1) split EntrySet(s) based on type
  collect_es_type_confluences(confluences);
  analyze_again = split_ess(confluences);
  if (!analyze_again) {
    collect_cs_type_confluences(confluences);
    forv_AVar(av, confluences)
      analyze_confluence(av);
    if (split_for_setters())
      analyze_again = 1;
  }
  while (!analyze_again) {
    // 2) compute setters for ivar imprecisions
    collect_cs_setter_confluences(confluences);
    int progress = 0;
    forv_AVar(av, confluences)
      progress = analyze_confluence(av, 1) || progress;
    // 3) stop if no progress
    if (!progress) break;
    // 4) split EntrySet(s) and CreationSet(s) for setter imprecisions
    if (split_for_setters()) {
      analyze_again = 1;
      break;
    }
  }
  if (analyze_again) {
    if (verbose_level) printf("extending analysis\n");
    clear_results();
    return 1;
  }
  return 0;
}

int
FA::analyze(Fun *top) {
  ::fa = this;
  initialize();
  top_edge = make_top_edge(top);
  do {
    initialize_pass();
    edge_worklist.push(top_edge);
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
  show_violations(fa, stderr);
  int untyped = show_untyped(this);
  return type_violations.n || untyped ? -1 : 0;
}

static Var *
info_var(AST *a, Sym *s) {
  if (!s) 
    s = a->sym;
  if (!s)
    s = a->rval;
  if (!s)
    return 0;
  if (a->pnodes.n) {
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
  } else
    if (s->var)
      return s->var;
  return 0;
}

// Given an AST node and a Sym, find the Sym which
// corresponds to the concrete (post-cloning) type of the
// variable corresponding to the Sym at that AST node.
Sym *
type_info(AST *a, Sym *s) {
  Var *v = info_var(a, s);
  if (v)
    return v->type;
  return 0;
}

// Given a function and an AST node, return the set of
// functions which could be called from that AST node.
void
call_info(Fun *f, AST *a, Vec<Fun *> &funs) {
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
    forv_CreationSet(cs, *av->out) {
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

// Given an AST node and a Sym, find the set of
// constants which could arrive at that point.
// make sure that there is not some dominating
// non-constant type.
int
constant_info(AST *a, Vec<Sym *> &constants, Sym *s) {
  constants.clear();
  Var *v = info_var(a, s);
  if (v)
    return constant_info(v, constants);
  return 0;
}

