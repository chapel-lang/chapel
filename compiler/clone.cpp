/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

typedef Vec<Vec<CreationSet *> *> CSSS;

static FA *fa = 0;
static Vec<CreationSet *> css, css_set;
static Vec<AVar *> global_avars;

static void
initialize() {
  forv_Fun(f, fa->funs)
    forv_Var(v, f->fa_all_Vars) {
      for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].value) {
	css_set.set_union(*v->avars.v[i].value->out);
	if (v->avars.v[i].key && v->avars.v[i].value->contour == GLOBAL_CONTOUR)
	  global_avars.set_add(v->avars.v[i].value);
      }
  }
  global_avars.set_to_vec();
  forv_CreationSet(cs, css_set) if (cs) {
    css.add(cs);
    cs->equiv = new Vec<CreationSet *>();
    cs->equiv->add(cs);
  }
  forv_Fun(f, fa->funs) {
    f->ess.set_intersection(fa->ess_set);  // restrict to used entry sets
    forv_EntrySet(es, f->ess) if (es) {
      forv_AEdge(e, es->out_edges) if (e)
        f->called_ess.set_add(e->to);
      es->equiv = new Vec<EntrySet *>();
      es->equiv->add(es);
    }
    forv_EntrySet(es, f->ess) if (es) {
      forv_AEdge(e, es->out_edges) if (e)
        forv_AVar(a, e->args)
          f->called_css.set_union(*a->out);
    }
  }
}

// object inlining placeholder
static int
equivalent_es_ivars(EntrySet *a, EntrySet *b) {
  return 1;
}

// return the Sym of some basic type, fail if basics are mixed or with non basics
static Sym *
basic_type(FA *fa, AType *t, Sym *fail) {
  Sym non_basic;
  Sym *res = 0;
  forv_CreationSet(cs, *t) {
    if (fa->basic_types.set_in(cs->sym)) {
      if (!res)
	res = cs->sym;
      else
	res = fail;
    } else {
      if (!res)
	res = &non_basic;
      else
	if (res != &non_basic)
	  res = fail;
    }
  }
  return res;
}

// clone for unboxing of basic types
static int
equivalent_es_vars(Var *v, EntrySet *a, EntrySet *b) {
  AVar *va = make_AVar(v, a), *vb = make_AVar(v, b);
  if (va != vb) {
    if (basic_type(fa, va->out, (Sym*)-1) != basic_type(fa, vb->out, (Sym *)-2))
      return 0;
  }
  return 1;
}

// clone for different sets of call targets
static int
equivalent_es_pnode(PNode *n, EntrySet *a, EntrySet *b) {
  Map<Fun *, AEdge *> *va = a->out_edge_map.get(n);
  Map<Fun *, AEdge *> *vb = b->out_edge_map.get(n);
  if (!va || !vb)
    return 1;
  Vec<Vec<EntrySet *> *> ca, cb;
  for(int i = 0; i < va->n; i++)
    if (va->v[i].key)
      ca.set_add(va->v[i].value->to->equiv);
  for(int i = 0; i < vb->n; i++)
    if (vb->v[i].key)
      cb.set_add(vb->v[i].value->to->equiv);
  if (va->some_disjunction(*vb))
    return 0;
  return 1;
}

class ES_FN { public: static int inline equivalent(EntrySet *a, EntrySet *b); };

// The definition is:
// 1) variables have the same boxing
// 2) calls are to the same set of functions
// 3) all objects are created equivalent CreationSets
// 4) if constant cloning is used, all constants must be the same
// 5) '.' and ')' (cast) primitives must have unique symbols
inline int
ES_FN::equivalent(EntrySet *a, EntrySet *b) {
  if (a->fun->clone_for_constants) {
    for (int i = 0; i < a->args.n; i++)
      if (a->args.v[i]->var->clone_for_constants)
	if (a->args.v[i]->constant() != a->args.v[i]->constant())
	  return 0;
  }
  if (!equivalent_es_ivars(a, b))
    return 0;
  forv_Var(v, a->fun->fa_all_Vars)
    if (!equivalent_es_vars(v, a, b))
      return 0;
  forv_PNode(n, a->fun->fa_all_PNodes) {
    if (!equivalent_es_pnode(n, a, b))
      return 0;
    // clone for creation points of concrete types
    if (n->lvals.n) {
      AVar *av = make_AVar(n->lvals.v[0], a);
      AVar *bv = make_AVar(n->lvals.v[0], b);
      if (av->creation_set && bv->creation_set &&
	  make_AVar(n->lvals.v[0], a)->creation_set->equiv != 
	  make_AVar(n->lvals.v[0], b)->creation_set->equiv)
	return 0;
    }
    if (n->prim) {
      switch (n->prim->index) {
	default: break;
	case P_prim_period: {
	  assert(n->prim->arg_types[1] == PRIM_TYPE_SYMBOL);
	  AVar *av = make_AVar(n->rvals.v[3], a);
	  AVar *bv = make_AVar(n->rvals.v[3], b);
	  if (av->out->some_disjunction(*bv->out))
	    return 0;
	  break;
	}
	case P_prim_cast: {
	  assert(n->prim->arg_types[0] == PRIM_TYPE_SYMBOL);
	  AVar *av = make_AVar(n->rvals.v[2], a);
	  AVar *bv = make_AVar(n->rvals.v[2], b);
	  if (av->out->some_disjunction(*bv->out))
	    return 0;
	  break;
	}
      }
    }
  }
  return 1;
}

// this code was MUCH easier to write in Lisp
class AEDGE_FN { 
 public: 
  static int equivalent(AEdge *a, AEdge *b) { return a->send == b->send; }
};

class CS_SYM_FN { 
 public: 
  static int equivalent(CreationSet *a, CreationSet *b) { return a->sym == b->sym; }
};

class CS_EQ_FN { 
 public: 
  static int equivalent(CreationSet *a, CreationSet *b) { 
    return a->sym == b->sym && !a->not_equiv.set_in(b); 
  }
};

// C++'s answer to higher order functions: YUCK
template <class C, class FN>
static void
sets_by_f(Vec<C *> &aset, Vec< Vec<C *> *> &asetset) {
  asetset.clear();
  Vec<C *> done;
  forv_Vec(C, x, aset) if (x) {
    if (done.set_add(x)) {
      Vec<C *> *equiv = new Vec<C *>();
      equiv->set_add(x);
      forv_Vec(C, y, aset) if (y) {
	if (!done.set_in(y) && FN::equivalent(x, y)) {
	  forv_Vec(C, z, *equiv)
	    if (z && !FN::equivalent(z, y))
	      goto Lskip;
	  done.set_add(y);
	  equiv->set_add(y);
	}
      Lskip:;
      }
      asetset.add(equiv);
    }
  }
}

static void
collect_ess_from_css(Vec<CreationSet *> &css, Vec<EntrySet *> &ess) {
  Vec<CreationSet *> todo_css;
  todo_css.copy(css);
  todo_css.set_to_vec();
  for (int i = 0; i < todo_css.n; i++) {
    CreationSet *cs = todo_css.v[i];
    forv_AVar(av, cs->defs)
      if (av->contour_is_entry_set)
	ess.set_add((EntrySet*)av->contour);
      else if (av->contour != GLOBAL_CONTOUR)
	todo_css.add((CreationSet*)av->contour);
  }
}

static inline void
make_not_equiv(CreationSet *a, CreationSet *b) {
  a->not_equiv.set_add(b);
  b->not_equiv.set_add(a);
}

static void
determine_basic_clones() {
  Vec<Vec<CreationSet *> *> css_sets;
  sets_by_f<CreationSet, CS_SYM_FN>(css, css_sets);
  // clone for unboxing of basic types
  for (int i = 0; i < css_sets.n; i++) {
    for (int j = 0; j < css_sets.v[i]->n; j++) {
      if (!css_sets.v[i]->v[j])
	continue;
      for (int k = 0; k < j; k++) {
	if (!css_sets.v[i]->v[k])
	  continue;
	CreationSet *cs1 = css_sets.v[i]->v[j], *cs2 = css_sets.v[i]->v[k];
	// if tuples are different lengths
	if (cs1->vars.n != cs2->vars.n) {
	  make_not_equiv(cs1, cs2);
	  continue;
	}
	// for tuples, for each variable
	for (int v = 0; v < cs1->vars.n; v++) {
	  AVar *av1 = cs1->vars.v[v], *av2 = cs2->vars.v[v];
	  // if the boxing or basic type is different
	  if (basic_type(fa, av1->out, (Sym*)-1) != 
	      basic_type(fa, av2->out, (Sym*)-2)) {
	    make_not_equiv(cs1, cs2);
	    continue;
	  }
	  // if we are cloning for constants and the constants are different
	  if (av1->var->clone_for_constants &&
	      av1->constant() != av2->constant()) {
	    make_not_equiv(cs1, cs2);
	    continue;
	  }
	}
      }
    }
  }
}

// the core function: builds equiv_sets for entry sets and creation sets
static void
determine_clones() {
  Vec<CreationSet *> changed_css, last_changed_css;
  Vec<EntrySet *> changed_ess, last_changed_ess;
  changed_ess.copy(fa->ess);
  changed_css.copy(css);

  determine_basic_clones();

  // find fixed point
  while (changed_css.n) {
    last_changed_css.move(changed_css);
    Vec<EntrySet *> last_changed_css_ess;
    collect_ess_from_css(last_changed_css, last_changed_css_ess);
	
    // recompute entry set equivalence
    while (changed_ess.n || last_changed_css.n) {
      changed_ess.set_to_vec();
      last_changed_ess.move(changed_ess);
      forv_Fun(f, fa->funs) {
	if (f->ess.some_intersection(last_changed_ess) ||
	    f->called_ess.some_intersection(last_changed_ess) ||
	    f->called_css.some_intersection(last_changed_css) ||
	    f->ess.some_intersection(last_changed_css_ess) ||
	    f->called_ess.some_intersection(last_changed_css_ess)) 
	{
	  sets_by_f<EntrySet, ES_FN>(f->ess, f->equiv_sets);
	  forv_EntrySet(es, f->ess) if (es) {
	    Vec<EntrySet *> *myset = NULL;
	    for (int i = 0; i < f->equiv_sets.n; i++)
	      if (f->equiv_sets.v[i]->in(es)) {
		myset = f->equiv_sets.v[i];
		break;
	      }
	    if (myset->some_disjunction(*es->equiv)) {
	      changed_ess.set_union(*myset);
	      es->equiv = myset;
	    }
	  }
	}
      }
      last_changed_css.clear();
    }
    // recompute non equivalent creation set required for static dispatch
    forv_Fun(f, fa->funs) {
      forv_Vec(Vec<EntrySet *>, ess, f->equiv_sets) {
	Vec<AEdge *> calls_edges;
	Vec<Vec<AEdge *> *> edge_sets;
	forv_EntrySet(es, *ess) if (es)
	  calls_edges.set_union(es->out_edges);
	sets_by_f<AEdge, AEDGE_FN>(calls_edges, edge_sets);
	for (int i = 0; i < edge_sets.n; i++)
	  for (int j = 0; j < edge_sets.v[i]->n; j++)
	    for (int k = 0; k < j; k++) {
	      AEdge *e1 = edge_sets.v[i]->v[j], *e2 = edge_sets.v[i]->v[k];
	      if (e1 && e2 && e1->fun == e2->fun && e1->to->equiv != e2->to->equiv) {
		for (int a = 0; a < e1->args.n; a++) {
		  Vec<CreationSet *> d1, d2;
		  // this is why strict type systems bite
		  ((VecCreationSet*)e1->args.v[a]->out)->set_difference(
		    *((VecCreationSet*)e2->args.v[a]->out), d1);
		  ((VecCreationSet*)e2->args.v[a]->out)->set_difference(
		    *((VecCreationSet*)e1->args.v[a]->out), d2);
		  forv_CreationSet(c1, d1) {
		    forv_CreationSet(c2, d2) {
		      if (c1->equiv == c2->equiv) 
			make_not_equiv(c1, c2);
		    }
		  }
		}
	      }
	    }
      }
    }
    // recompute creation set equivalence from non-equivalence
    Vec<Vec<CreationSet *> *> css_sets;
    sets_by_f<CreationSet, CS_EQ_FN>(css, css_sets);
    for (int i = 0; i < css_sets.n; i++) {
      Vec<CreationSet *> *s = css_sets.v[i];
      forv_CreationSet(cs, *s) if (cs) {
	if (cs->equiv->some_disjunction(*s))
	  changed_css.set_add(cs);
	cs->equiv = css_sets.v[i];
      }
    }
  }
}

// sets cs->sym to the new concrete symbol
static void
define_concrete_types(CSSS &css_sets) {
  for (int i = 0; i < css_sets.n; i++) {
    Vec<CreationSet *> *eqcss = css_sets.v[i];
    Sym *sym = 0;
    forv_CreationSet(cs, *eqcss) if (cs) {
      if (!sym)
	sym = cs->sym;
      else if (sym != cs->sym)
	sym = (Sym *)-1;
    }
    // same sym
    if (sym != (Sym*)-1) {
      AVar *def = 0;
      forv_CreationSet(cs, *eqcss) if (cs) {
	forv_AVar(av, cs->defs) {
	  if (!def)
	    def = av;
	  else if (def != av)
	    def = (AVar *)-1;
	}
      }
      if (sym->type_kind == Type_PRIMITIVE || sym->fun) {
	forv_CreationSet(cs, *eqcss) if (cs)
	  cs->type = sym;
      } else if (sym == sym_tuple) {
	// tuples use record type
	sym = if1_alloc_sym(fa->pdb->if1);
	sym->type_kind = Type_RECORD;
	sym->incomplete = 1;
	forv_CreationSet(cs, *eqcss) if (cs)
	  cs->type = sym;
      } else {
	Sym *s = if1_alloc_sym(fa->pdb->if1);
	s->type_kind = sym->type_kind;
	s->incomplete = 1;
	forv_CreationSet(cs, *eqcss) if (cs)
	  cs->type = s;
      }
    } else {
      // if different sym use sum type
      sym = if1_alloc_sym(fa->pdb->if1);
      sym->type_kind = Type_SUM;
      sym->incomplete = 1;
      forv_CreationSet(cs, *eqcss) if (cs)
	cs->type = sym;
    }
  }
}

static void
resolve_concrete_types(CSSS &css_sets) {
  for (int i = 0; i < css_sets.n; i++) {
    Vec<CreationSet *> *eqcss = css_sets.v[i];
    Sym *sym;
    forv_CreationSet(cs, *eqcss) if (cs) {
      sym = cs->type;
      break;
    }
    if (sym->incomplete) {
      sym->incomplete = 0;
      switch (sym->type_kind) {
	case Type_NONE:
	case Type_UNKNOWN: 
	case Type_FUN:
	case Type_ALIAS:
	case Type_PRODUCT:
	case Type_PRIMITIVE:
	case Type_APPLICATION:
	  assert(!"bad case");
	  break;
	case Type_SUM: {
	  forv_CreationSet(cs, *eqcss) if (cs)
	    sym->has.set_add(cs->sym);
	  sym->has.set_to_vec();
	  break;
	}
	case Type_RECORD:
	case Type_VECTOR:
	case Type_REF:
	case Type_TAGGED: {
	  forv_CreationSet(cs, *eqcss) if (cs) {
	    sym->has.fill(cs->vars.n);
	    for (int i = 0; i < cs->vars.n; i++) {
	      AVar *av = cs->vars.v[i];
	      if (!sym->has.v[i])
		sym->has.v[i] = if1_alloc_sym(fa->pdb->if1);
	      forv_CreationSet(x, *av->out) if (cs)
		sym->has.v[i]->has.set_add(x->sym);
	    }
	  }
	  for (int i = 0; i < sym->has.n; i++) {
	    Sym *s = sym->has.v[i];
	    s->has.set_to_vec();
	    if (s->has.n == 1)
	      sym->has.v[i] = s->has.v[0];
	    else
	      s->type_kind = Type_SUM;
	  }
	  break;
	}
      }
    }
  }
}

static void
build_concrete_types() {
  CSSS css_sets;
  forv_CreationSet(cs, css)
    css_sets.set_add(cs->equiv);
  css_sets.set_to_vec();
  define_concrete_types(css_sets);
  resolve_concrete_types(css_sets);
}

static void
concretize_types(Fun *f) {
  forv_Var(v, f->fa_all_Vars) {
    Sym *sym = 0, *type = 0;
    for (int i = 0; i < v->avars.n; i++) {
      if (!v->avars.v[i].key)
	continue;
      forv_CreationSet(cs, *v->avars.v[i].value->out) {
	if (!sym)
	  sym = cs->type;
	else {
	  if (sym != cs->type) {
	    if (!type) {
	      type = if1_alloc_sym(fa->pdb->if1);
	      type->type_kind = Type_SUM;
	      type->has.set_add(sym);
	    }
	    type->has.set_add(cs->type);
	  }
	}
      }
    }
    if (!type)
      v->type = sym;
    else {
      type->has.set_to_vec();
      if (type->has.n == 1)
	v->type = type->has.v[0];
      else
	v->type = type;
    }
  }
}

static void
fixup_var(Var *v, Fun *f, Vec<EntrySet *> *ess) {
  AVarMap avs;
  for (int i = 0; i < v->avars.n; i++)
    if (v->avars.v[i].key && ess->set_in((EntrySet*)v->avars.v[i].key))
	  avs.put(v->avars.v[i].key, v->avars.v[i].value);
  v->avars.move(avs);
}

static void
fixup_clone(Fun *f, Vec<EntrySet *> *ess) {
  f->ess.copy(*ess);
  forv_EntrySet(es, f->ess) if (es)
    es->fun = f;
  // fixup local variables
  forv_Var(v, f->fa_all_Vars)
    if (v->sym->in == f->sym)
      fixup_var(v, f, ess);
  // fixup arguments and return values
  forv_EntrySet(es, *ess) if (es) {
    f->args.fill(es->args.n);
    f->rets.fill(es->rets.n);
  }
  forv_EntrySet(es, *ess) if (es) {
    for (int i = 0; i < es->args.n; i++) {
      if (!f->args.v[i]) {
	f->args.v[i] = es->args.v[i]->var->copy();
	f->fa_all_Vars.add(f->args.v[i]);
      }
      f->args.v[i]->avars.put(es, es->args.v[i]);
    }
    for (int i = 0; i < es->rets.n; i++) {
      if (!f->rets.v[i]) 
	f->rets.v[i] = es->rets.v[i]->var->copy();
      f->rets.v[i]->avars.put(es, es->rets.v[i]);
      f->fa_all_Vars.add(f->rets.v[i]);
    }
  }
}

static void
clone_functions() {
  Vec<Fun *> fs;
  fs.copy(fa->funs);
  forv_Fun(f, fs) {
    if (f->equiv_sets.n == 1) {
      fixup_clone(f, f->equiv_sets.v[0]);
      concretize_types(f);
    } else {
      for (int i = 0; i < f->equiv_sets.n; i++) {
	Fun *ff = (i == f->equiv_sets.n - 1) ? f : f->copy();
	fixup_clone(ff, f->equiv_sets.v[i]);
	concretize_types(ff);
	fa->funs.add(ff);
      }	
    }
  }
}

int
PDB::clone(FA *afa, Fun *top) {
  ::fa = afa;
  initialize();
  determine_clones();
  build_concrete_types();
  clone_functions();
  return 0;
}
