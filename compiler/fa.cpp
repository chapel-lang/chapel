/* -*-Mode: c++;-*-
 Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "pattern.h"

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
static AType *anyclass_type = 0;

static OpenHash<AType *, ATypeOpenHashFns> cannonical_atypes;
static OpenHash<Setters *, SettersHashFns> cannonical_setters;
static OpenHash<SettersClasses *, SettersClassesHashFns> cannonical_setters_classes;
static OpenHash<ATypeFold *, ATypeFoldOpenHashFns> type_fold_cache;
static OpenHash<ATypeViolation *, ATypeViolationHashFuns> type_violation_hash;

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
static int application_constraints(PNode *p, EntrySet *es, AVar *fun, CreationSet *s, Vec<AVar *> &args);

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
    av->lvalue = new AVar(v, es);
    av->lvalue->is_lvalue = 1;
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
    vars.add(iv);
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

AType *
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
  update_in(v, make_AType(cs));
  return cs;
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
      return b->allsubtypes.set_in(a->type) != 0;
  }
  else
    return b->allsubtypes.set_in(a) != 0;
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

static void
fill_rets(EntrySet *es, int n) {
  es->rets.fill(n);
  for (int i = 0; i < n; i++)
    if (!es->rets.v[i])
      es->rets.v[i] = make_AVar(es->fun->sym->ret->var, es);
}

static int 
same_eq_classes(Setters *s, Setters *ss) {
  if (s == ss)
    return 1;
  if (!s || !ss)
    return 0;
  Vec<Setters *> sc1, sc2;
  forv_AVar(av, *s) {
    assert(av->setter_class);
    sc1.set_add(av->setter_class);
  }
  forv_AVar(av, *ss) {
    assert(av->setter_class);
    sc2.set_add(av->setter_class);
  }
  if (sc1.some_disjunction(sc2))
    return 0;
  return 1;
}

static int
edge_type_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  assert(e->args.n && es->args.n);
  if (!es->split) {
    forv_MPosition(p, e->match->fun->positions) {
      AVar *es_arg = es->args.get(p), *e_arg = e->args.get(p);
      if (!e_arg)
	continue;
      if (p->pos.n == 1 && p->pos.v[0] == int2Position(2) && es->fun->sym == sym_new_object) {
	if (es_arg->out != e_arg->out)
	  return -1;
      } else
	if (es_arg->out != e_arg->out)
	  return 0;
    }
    assert(es->rets.n == e->rets.n);
    for (int i = 0; i < es->rets.n; i++) {
      if (es->rets.v[i]->lvalue && e->rets.v[i]->lvalue)
	if (es->rets.v[i]->lvalue->out != e->rets.v[i]->lvalue->out)
	  return 0;
    }
  } else {
    AEdge **last = es->edges.last();
    for (AEdge **pee = es->edges.first(); pee < last; pee++) if (*pee) {
      AEdge *ee = *pee;
      if (!ee->args.n)	
	continue;
      forv_MPosition(p, e->match->fun->positions) {
	AVar *e_arg = e->args.get(p); 
	AVar *ee_arg = ee->args.get(p);
	if (!e_arg || !ee_arg)
	  continue;
	if (ee_arg->out != e_arg->out)
	  return 0;
      }
      assert(e->rets.n == ee->rets.n);
      for (int i = 0; i < e->rets.n; i++) {
	if (ee->rets.v[i]->lvalue && e->rets.v[i]->lvalue)
	  if (ee->rets.v[i]->lvalue->out != e->rets.v[i]->lvalue->out)
	    return 0;
      }
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
edge_sset_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  assert(e->args.n && es->args.n);
  if (!es->split) {
    forv_MPosition(p, e->match->fun->positions) {
      AVar *av = e->args.get(p);
      if (av)
	if (!sset_compatible(av, es->args.get(p)))
	  return 0;
    }
    assert(es->rets.n == e->rets.n);
    for (int i = 0; i < es->rets.n; i++)
      if (!sset_compatible(e->rets.v[i], es->rets.v[i]))
	return 0;
  } else {
    AEdge **last = es->edges.last();
    for (AEdge **pee = es->edges.first(); pee < last; pee++) if (*pee) {
      AEdge *ee = *pee;
      if (!ee->args.n)	
	continue;
      forv_MPosition(p, e->match->fun->positions) {
	AVar *eav = e->args.get(p), *eeav = ee->args.get(p);
	if (eav || eeav)
	  if (!sset_compatible(eav, eeav))
	    return 0;
      }
      assert(e->rets.n == ee->rets.n);
      for (int i = 0; i < e->rets.n; i++)
	if (!sset_compatible(ee->rets.v[i], ee->rets.v[i]))
	  return 0;
    }
  }
  return 1;
}

static int
edge_constant_compatible_with_entry_set(AEdge *e, EntrySet *es) {
  forv_MPosition(p, e->match->fun->positions) {
    AVar *av = es->args.get(p);
    if (av->var->clone_for_constants) {
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
set_entry_set(AEdge *e, EntrySet *es = 0) {
  EntrySet *new_es = es;
  if (!es) {
    new_es = new EntrySet(e->match->fun);
    e->match->fun->ess.add(new_es);
  }
  e->to = new_es;
  new_es->edges.put(e);
  if (!es) {
    forv_MPosition(p, e->match->fun->positions) {
      Sym *s = e->match->fun->arg_syms.get(p);
      AVar *av = make_AVar(s->var, new_es);
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
check_split(AEdge *e) {
  if (e->from && e->from->split) {
    Map<Fun *, AEdge *> *m = e->from->split->out_edge_map.get(e->pnode);
    if (m) {
      for (int i = 0; i < m->n; i++)
	if (m->v[i].key == e->match->fun) {
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
  AType *tt = type_intersection(v->in, v->restrict);
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
      update_in(vv, v->out);
  }
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
      if (v->sym->external)
	update_in(av, v->sym->type->abstract_type);
      if (v->sym->constant) // for constants, the abstract type is the concrete type
	update_in(av, make_abstract_type(v->sym));
      if (v->sym->symbol || v->sym->fun) 
	update_in(av, v->sym->abstract_type);
      if (v->sym->type_kind != Type_NONE)
	update_in(av, v->sym->type_sym->abstract_type);
    }
  }
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
    flow_vars(atv, iv);
    if (iv->var->sym->name)
      cs->var_map.put(iv->var->sym->name, iv);
  }
}

static AVar *
get_element_avar(CreationSet *cs) {
  AVar *elem = unique_AVar(element_var, cs);
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

static void
make_closure(AVar *result) {
  assert(result->contour_is_entry_set);
  CreationSet *cs = creation_point(result, sym_function);
  int add = !cs->vars.n;
  PNode *partial_application = result->var->def;
  EntrySet *es = (EntrySet*)result->contour;
  if (partial_application->prim) { // apply and period
    AVar *iv = unique_AVar(partial_application->rvals.v[1], cs);
    flow_var_to_var(make_AVar(partial_application->rvals.v[1], es), iv);
    if (add)
      cs->vars.add(iv);
    iv = unique_AVar(partial_application->rvals.v[3], cs);
    flow_var_to_var(make_AVar(partial_application->rvals.v[3], es), iv);
    if (add)
      cs->vars.add(iv);
  } else {
    for (int i = 0; i < partial_application->rvals.n; i++) {
      AVar *iv = unique_AVar(partial_application->rvals.v[i], cs);
      flow_var_to_var(make_AVar(partial_application->rvals.v[i], es), iv);
      if (add)
	cs->vars.add(iv);
    }
  }
}

// for send nodes, add simple constraints which do not depend 
// on the computed types (compare to add_send_edgse_pnodes)
static void
add_send_constraints(EntrySet *es) {
  forv_PNode(p, es->fun->fa_send_PNodes) {
    if (p->prim) {
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
	      if (j - start != p->prim->pos) {
		AVar *av = make_AVar(p->rvals.v[j], es), *res = make_AVar(p->lvals.v[0], es);
		av->arg_of_send.set_add(res);
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
    for (int i = 0; i < p->rvals.n; i++) {
      AVar *lhs = make_AVar(p->lvals.v[i], es), *rhs = make_AVar(p->rvals.v[i], es);
      flow_var_to_var(rhs, lhs);
      if (lhs->lvalue)
	flow_var_to_var(rhs, lhs->lvalue);
    }
  }
}

static AEdge *
make_AEdge(Match *m, PNode *p, EntrySet *from) {
  Fun *f = m->fun;
  Map<Fun *, AEdge *> *f2e = from->out_edge_map.get(p);
  if (!f2e)
    from->out_edge_map.put(p, (f2e = new Map<Fun *, AEdge *>()));
  AEdge *e = f2e->get(f);
  if (!e) {
    f2e->put(f, (e = new AEdge()));
    e->pnode = p;
    e->from = from;
  }
  if (!e->match)
    e->match = m;
  else {
    assert(e->match->fun == m->fun);
    forv_MPosition(p, e->match->fun->positions) {
      AType *t1 = e->match->filters.get(p), *t2 = m->filters.get(p);
      if (t1 && t2)
	e->match->filters.put(p, type_union(t1, t2));
      else if (!t1 && t2)
	e->match->filters.put(p, t2);
    }
  }
  from->out_edges.set_add(e);
  if (!e->in_edge_worklist) {
    e->in_edge_worklist = 1;
    edge_worklist.push(e);
  }
  return e;
}

// returns 1 if any are partial, 0 if some matched and -1 if none matched
static int
all_application_constraints(PNode *p, EntrySet *es, AVar *a0, Vec<AVar *> &args) {
  int incomplete = -2;
  forv_CreationSet(cs, *a0->out) if (cs)
    switch (application_constraints(p, es, a0, cs, args)) {
      case -1: if (incomplete < 0) incomplete = -1; break;
      case 0: if (incomplete < 0) incomplete = 0; break;
      case 1: incomplete = 1; break;
    }
  return incomplete;
}

static int
partial_application(PNode *p, EntrySet *es, CreationSet *cs, Vec<AVar *> &args) {
  AVar *result = make_AVar(p->lvals.v[0], es);
  AVar *fun = cs->vars.v[0];
  for (int i = cs->vars.n - 1; i >= 1; i--) {
    cs->vars.v[i]->arg_of_send.set_add(result);
    args.add(cs->vars.v[i]);
  }
  fun->arg_of_send.set_add(result);
  return all_application_constraints(p, es, fun, args);
}

static void
record_arg(AVar *a, Sym *s, Map<MPosition*,AVar*> &args, MPosition &p) {
  MPosition *cpnum = cannonicalize_mposition(p), *cpname = 0;
  if (a->var->sym->name && !is_const(a->var->sym)) {
    MPosition pp(p);
    pp.set_top(a->var->sym->name);
    cpname = cannonicalize_mposition(pp);
  }
  for (MPosition *cp = cpnum; cp; cp = cpname, cpname = 0) { 
    args.put(cp, a);
    if (s->pattern) {
      forv_CreationSet(cs, *a->out) {
	if (cs->sym == sym_tuple) {
	  assert(s->has.n == cs->vars.n);
	  p.push(1);
	  for (int i = 0; i < s->has.n; i++) {
	    record_arg(cs->vars.v[i], s->has.v[i], args, p);
	    p.inc();
	  }
	  p.pop();
	}
      }
    }
  }
}

static int
function_dispatch(PNode *p, EntrySet *es, AVar *a0, CreationSet *s, Vec<AVar *> &args) {
  Vec<AVar *> a;
  int partial = 0;
  a.add(a0);
  for (int j = args.n - 1; j >= 0; j--)
    a.add(args.v[j]);
  Vec<Match *> matches;
  AVar *send = make_AVar(p->lvals.v[0], es);
  pattern_match(fa, a, matches, send);
  forv_Match(m, matches) {
    if (m->fun->sym->args.n == a.n) {
      AEdge *ee = make_AEdge(m, p, es);
      if (!ee->args.n) {
	MPosition p;
	p.push(1);
	for (int i = 0; i < m->fun->sym->args.n; i++) {
	  record_arg(a.v[i], m->fun->sym->args.v[i], ee->args, p);
	  p.inc();
	}
      }
      if (!ee->rets.n) {
	for (int i = 0; i < p->lvals.n; i++)
	  ee->rets.add(make_AVar(p->lvals.v[i], ee->from));
      }
    } else 
      partial = 1;
    if (!p->next_callees)
      p->next_callees = new PNode::Callees;
    p->next_callees->funs.set_add(m->fun);
  }
  return matches.n ? partial : -1;
}

static int
application_constraints(PNode *p, EntrySet *es, AVar *a0, CreationSet *cs, Vec<AVar *> &args) {
  if (sym_function->allsubtypes.set_in(cs->sym) && cs->defs.n)
    return partial_application(p, es, cs, args);
  return function_dispatch(p, es, a0, cs, args);
}

void
type_violation(ATypeViolation_kind akind, AVar *av, AType *type, AVar *send, Vec<Fun *> *funs) {
  ATypeViolation *v = new ATypeViolation(akind, av, send);
  v = type_violation_hash.put(v);
  if (!v->type)
    v->type = type;
  else
    v->type = type_union(v->type, type);
  if (funs)
    v->funs = funs;  // overwrite is OK as funs is monotonic
  type_violations.set_add(v);
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
      av->arg_of_send.set_add(result);
      args.add(av);
    }
    AVar *a0 = make_AVar(p->rvals.v[0], es);
    a0->arg_of_send.set_add(result);
    if (all_application_constraints(p, es, a0, args) > 0)
      make_closure(result);
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
	Vec<AVar *> args;
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *fun = make_AVar(p->rvals.v[1], es);
	AVar *a1 = make_AVar(p->rvals.v[3], es);
	fun->arg_of_send.set_add(result);
	a1->arg_of_send.set_add(result);
	args.add(a1);
	if (all_application_constraints(p, es, fun, args) > 0)
	  make_closure(result);
	break;
      }
      case P_prim_period: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *obj = make_AVar(p->rvals.v[1], es);
	AVar *selector = make_AVar(p->rvals.v[3], es);
	int partial = 0;
	obj->arg_of_send.set_add(result);
	selector->arg_of_send.set_add(result);
	set_container(result, obj);
	forv_CreationSet(sel, *selector->out) if (sel) {
	  char *symbol = sel->sym->name; assert(symbol);
	  forv_CreationSet(cs, *obj->out) if (cs) {
	    AVar *iv = cs->var_map.get(symbol);
	    if (iv)
	      flow_vars_equal(iv, result);
	    else {
	      Vec<AVar *> args;
	      args.add(selector);
	      int res = application_constraints(p, es, obj, cs, args);
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
      case P_prim_assign: {
	AVar *result = make_AVar(p->lvals.v[0], es);
	AVar *ref = make_AVar(p->rvals.v[1], es);
	AVar *val = make_AVar(p->rvals.v[3], es);
	ref->arg_of_send.set_add(result);
	forv_CreationSet(cs, *ref->out) if (cs) {
	  assert(cs->vars.n);
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
	thing->arg_of_send.set_add(result);
	forv_CreationSet(cs, *thing->out) if (cs)
	  creation_point(result, cs->sym->type_sym); // recover original type
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

static void
analyze_edge(AEdge *e) {
  make_entry_set(e);
  forv_MPosition(p, e->match->fun->positions) {
    AVar *a = e->args.get(p), *b = e->to->args.get(p);
    if (!a)
      continue;
    AType *filter = e->match->filters.get(p);
    if (!filter)
      continue;
    flow_var_type_permit(b, filter);
    forv_CreationSet(cs, *filter) if (cs)
      cs->ess.set_add(e->to);
    flow_vars(a, b);
    if (p->pos.n > 1)
      set_container(b, e->to->args.get(p->parent));
  }
  creation_point(make_AVar(e->match->fun->sym->cont->var, e->to), sym_continuation);
  for (int i = 0; i < e->pnode->lvals.n; i++)
    flow_vars(e->to->rets.v[i], e->rets.v[i]);
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
  e->match->filters.put(cp, top_type);
  e->args.put(cp, make_AVar(sym_init->var, e->to));
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
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *xav = make_AVar(v, es);
      for (AVar *av = xav; av; av = av->lvalue)
	fa->css_set.set_union(*av->out);
    }
  }
  forv_CreationSet(cs, fa->css_set) if (cs) 
    fa->css.add(cs);
  // collect callees
  forv_Fun(f, fa->funs) {
    forv_PNode(pnode, f->fa_send_PNodes) {
      if (pnode->next_callees) {
	pnode->callees = pnode->next_callees;
	pnode->next_callees = 0;
	forv_Fun(f, pnode->callees->funs)
	  pnode->callees->positions.set_union(f->positions);
      }
    }
  }
  // print results
  if (verbose_level)
    fa_dump_types(fa, stdout);
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

static void
collect_avar_argument_type_violations(AVar *av) {
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
      for (AEdge **x = xes->edges.first(); x < last; x++) if (*x) {
	AEdge *ee = *x;
	if (!ee->args.n)	
	  continue;
	if (ee->pnode == p && ee->from == from)
	  goto Lfound;
      }
      continue;
    Lfound:
      t = type_diff(t, a->out);
    }
    t = type_minus_partial_applications(t);
    if (t != bottom_type)
      type_violation(ATypeViolation_SEND_ARGUMENT, av, t, 
		     make_AVar(p->lvals.v[0], from));
  }
}

// for each call site, check that all args are covered
static void
collect_argument_type_violations() {
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *xav = make_AVar(v, es);
      for (AVar *av = xav; av; av = av->lvalue)
	collect_avar_argument_type_violations(av);
    }
  }
}

static void
collect_notype() {
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      if (av->out == bottom_type)
	type_violation(ATypeViolation_NOTYPE, av, av->out, 0, 0);
    }
  }
}

static void
complete_pass() {
  collect_results();
  collect_argument_type_violations();
  collect_notype();
}

inline static void
subtype(Sym *s, Sym *ss, Vec<Sym *> &types) {
  assert(s != ss);
  if (s->subtypes.set_add(ss))
    ss->dispatch_order.add(s);
  types.set_add(s);
  types.set_add(ss);
}

static void
initialize_symbols() {
  Vec<Sym *> type_syms, types;
  forv_Sym(s, fa->pdb->if1->allsyms) {
    // symbols (selectors) have all applicable functions as subtypes
    if (s->symbol) {
      s->abstract_type = make_abstract_type(s);
      subtype(sym_symbol, s, types);
    }
    if (s->constant) {
      subtype(s->type, s, types);
      s->type_sym = s;
    }
    if (s->fun) {
      s->abstract_type = make_abstract_type(s);
      subtype(sym_function, s, types);
    }
    if (s->type && s->type_kind && s != s->type)
      subtype(s->type, s, types);
    forv_Sym(ss, s->implements)
      subtype(ss, s, types);
    forv_Sym(ss, s->constraints)
      subtype(ss, s, types);
    // functions are subtypes of the initial symbol in their pattern
    // which may be a constant or a constant contrainted variable
    if (s->fun && s->args.n) {
      Sym *a0 = s->args.v[0];
      if (a0->symbol)
	subtype(a0, s, types);
      if (a0->type && a0->type->symbol)
	subtype(a0->type, s, types);
    }
    if (s->type_kind) {
      s->abstract_type = make_abstract_type(s);
      types.set_add(s);
    }
    forv_Sym(ss, s->has)
      if (!ss->var)
	ss->var = new Var(ss);
    if (s->type_sym)
      type_syms.add(s);
  }
  forv_Sym(s, types) if (s) {
    if (!s->dispatch_order.n && s != sym_any) {
      if (s->meta && (s != sym_anyclass))
	subtype(sym_anyclass, s, types);
      else if (s->value && (s != sym_value))
	subtype(sym_value, s, types);
      else
	subtype(sym_any, s, types);
    }
  }
  // map type_syms
  forv_Sym(s, type_syms) {
    forv_Sym(ss, s->subtypes) if (ss) {
      assert(ss->type_sym);
      s->type_sym->subtypes.set_add(ss->type_sym);
    }
  }
  // compute allsubtypes
  forv_Sym(s, types) if (s) {
    s->allsubtypes.set_add(s);
    s->allsubtypes.set_union(s->subtypes);
  }
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, types) if (s) {
      forv_Sym(ss, s->allsubtypes) if (ss) {
	changed = s->allsubtypes.set_union(ss->allsubtypes) || changed;
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
  anyclass_type = make_abstract_type(sym_anyclass);
  edge_worklist.clear();
  send_worklist.clear();
  initialize_symbols();
  initialize_primitives();
  build_positions(fa);
  build_patterns(fa);
}

static void
initialize_pass() {
  type_violations.clear();
  type_violation_hash.clear();
  entry_set_done.clear();
  refresh_top_edge(fa->top_edge);
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

static int
is_es_recursive(EntrySet *es) {
  if (es->split)
    return es->split->backedges.n;
  return es->backedges.n;
}

static int
is_es_cs_recursive(EntrySet *es) {
  if (es->split)
    return es->split->es_cs_backedges.n;
  return es->es_cs_backedges.n;
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
      AVar *xav = make_AVar(v, es);
      for (AVar *av = xav; av; av = av->lvalue)
	forv_AVar(x, av->backward) if (x) {
	  if (!x->out->n)
	    continue;
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
	if (!x->out->n)
	  continue;
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
split_entry_set(AVar *av, int fsetters = 0) {
  EntrySet *es = (EntrySet*)av->contour;
  if (!fsetters ? is_es_recursive(es) : is_es_cs_recursive(es))
    return 0;
  Vec<AEdge *> do_edges;
  int nedges = 0;
  AEdge **last = es->edges.last();
  for (AEdge **ee = es->edges.first(); ee < last; ee++) if (*ee) {
    if (!(*ee)->args.n)	
      continue;
    nedges++;
    if (!fsetters) {
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
split_ess_type(Vec<AVar *> &confluences) {
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!av->is_lvalue) {
	if (is_formal_argument(av))
	  if (split_entry_set(av, 0))
	    analyze_again = 1;
      } else {
	AVar *aav = unique_AVar(av->var, av->contour);
	if (is_return_value(aav))
	  if (split_entry_set(aav, 0))
	    analyze_again = 1;
      }
    }
  }
  return analyze_again;
}

static int
split_ess_setters(Vec<AVar *> &confluences) {
  int analyze_again = 0;
  forv_AVar(av, confluences) {
    if (av->contour_is_entry_set) {
      if (!av->is_lvalue) {
	if (is_return_value(av))
	  if (split_entry_set(av, 1))
	    analyze_again = 1;
      } else {
	AVar *aav = unique_AVar(av->var, av->contour);
	if (is_formal_argument(aav))
	  if (split_entry_set(aav, 1))
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
  e->args.clear();
  e->rets.clear();
  e->match->filters.clear();
}

static void
clear_es(EntrySet *es) {
  AEdge **last = es->edges.last();
  for (AEdge **ee = es->edges.first(); ee < last; ee++) if (*ee)
    clear_edge(*ee);
  es->out_edges.clear();
  forv_MPosition(p, es->fun->positions)
    es->args.get(p)->restrict = bottom_type;
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
  forv_AVar(x, *s)
    if (x)
      s->sorted.add(x);
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
  setter_starters.set_to_vec();
}

static int
split_css(Vec<AVar *> &starters) {
  int analyze_again = 0;
  // build starter sets, groups of starters for the same CreationSet
  while (starters.n) {
    CreationSet *cs = starters.v[0]->creation_set;
    if (is_es_cs_recursive(cs))
      continue;
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
  if (split_ess_setters(setter_confluences))
    return 1;
  if (split_css(setter_starters))
    return 1;
  return 0;
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
// On a new partition off setters this function recomputes the equiv sets
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

static int
analyze_confluence(AVar *av, int fsetter = 0) {
  int setters_changed = 0;
  Vec<Setters *> ss;
  Vec<AVar *> *dir = fsetter ? &av->forward : &av->backward;
  forv_AVar(x, *dir) if (x) {
    assert(x->contour_is_entry_set);
    if (!fsetter && !x->out)
      continue;
    for (int i = 0; i < ss.n; i++) {
      forv_AVar(a, *ss.v[i]) if (a) {
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
  forv_AVar(x, *dir) if (x)
    setters_changed = update_setter(x->container, x) || setters_changed;
  return setters_changed;
}

static int
extend_analysis() {
  // initialize
  int analyze_again = 0;
  static int extensions = 0;
  compute_recursive_entry_sets();
  compute_recursive_entry_creation_sets();
  forv_EntrySet(es, fa->ess) 
    es->split = 0;
  forv_CreationSet(cs, fa->css) 
    cs->split = 0;
  Vec<AVar *> confluences;
  // 1) split EntrySet(s) based on type
  collect_es_type_confluences(confluences);
  analyze_again = split_ess_type(confluences);
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
    ++extensions;
    if (verbose_level) printf("extending analysis %d\n", extensions);
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
  return type_violations.n ? -1 : 0;
}

static Var *
info_var(AST *a, Sym *s) {
  if (!s)
    s = a->rval;
  if (!s) 
    s = a->sym;
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
  }
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

