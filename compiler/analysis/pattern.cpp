#include "geysa.h"
#include "pattern.h"
#include "pdb.h"
#include "if1.h"
#include "builtin.h"
#include "fun.h"
#include "pnode.h"
#include "fa.h"
#include "var.h"


// TODO
// handle context sensitive lookups, like localized pattern matching
//   - handle them with a unique symbol in a hidden first position to uniquify them
//   - the pnode will have multiple return values
// figure out what myclass does in Sather and Clu
// optimize case of filters with no restrictions (sym_any) to prevent
//   recomputation of dispatch functions
// used MPosition to handle destructuring of return values
// handle generalized structural types
// optimize case where only 1 function is possible (just check)

typedef Map<Fun *, Match *> MatchMap;
typedef Vec<Vec<Fun *> *> PartialMatches;

static ChainHash<MPosition *, MPositionHashFuns> cannonical_mposition;

MPosition::MPosition(MPosition &p) {
  pos.copy(p.pos);
  parent = p.parent;
}

MPosition *
cannonicalize_mposition(MPosition &p) {
  MPosition *cp = cannonical_mposition.get(&p);
  if (cp)
    return cp;
  cp = new MPosition;
  cp->pos.copy(p.pos);
  cannonical_mposition.put(cp);
  return cp;
}

static void
insert_fun(FA *fa, Fun *f, Sym *arg, Sym *s, MPosition &p) {
  MPosition *cpnum = cannonicalize_mposition(p), *cpname = 0;
  if (arg->name && !arg->is_constant && !arg->is_symbol) {
    MPosition pp(p);
    pp.set_top(arg->name);
    cpname = cannonicalize_mposition(pp);
  }
  for (MPosition *cp = cpnum; cp; cp = cpname, cpname = 0) { 
    if (fa->patterns->types_set.set_add(s))
      fa->patterns->types.add(s);
    if (!s->match_type) {
      s->match_type = new MType;
      fa->patterns->mtypes.add(s->match_type);
    }
    Vec<Fun *> *funs = s->match_type->funs.get(cp);
    if (!funs) {
      funs = new Vec<Fun *>;
      s->match_type->funs.put(cp, funs);
    }
    funs->set_add(f);
  }
}

static void
build_arg(FA *fa, Fun *f, Sym *a, MPosition &p) {
  if ((a->is_symbol || (a->type && a->type->is_symbol))) {
    Sym *sel = a->is_symbol ? a : a->type;
    insert_fun(fa, f, a, sel, p);
  } else
    insert_fun(fa, f, a, a->must_specialize ? a->must_specialize : sym_any, p);
  if (a->is_pattern) {
    p.push(1);
    forv_Sym(aa, a->has)
      build_arg(fa, f, aa, p);
    p.pop();
  }
  p.inc();
}

static void
build(FA *fa) {
  forv_Fun(f, fa->pdb->funs) {
    MPosition p;
    p.push(1);
    insert_fun(fa, f, f->sym, f->sym, p);
    forv_Sym(a, f->sym->has)
      build_arg(fa, f, a, p);
  }
}

static int
pattern_match_sym(FA *fa, Sym *s, CreationSet *cs, Vec<Fun *> *funs, Vec<Fun *> *partial_matches, 
		  MatchMap &match_map, MPosition *cp, Vec<Sym *> &done) 
{
  int found = 0;
  if (s->match_type) {
    Vec<Fun *> *fs = s->match_type->funs.get(cp);
    if (fs) {
      Vec<Fun *> ffs;
      if (partial_matches) {
	partial_matches->set_intersection(*fs, ffs);
	fs = &ffs;
      }
      forv_Fun(f, *fs) if (f) {
	found = 1;
	Sym *fun_arg = f->arg_syms.get(cp);
	if (fun_arg->is_pattern)
	  continue;
	Match *m = match_map.get(f);
	if (!m) {
	  m = new Match(f);
	  match_map.put(f, m); 
	}
	AType *t = m->filters.get(cp);
	if (!t) {
	  t = new AType; 
	  m->filters.put(cp, t);
	}
	t->set_add(cs);
      }
      funs->set_union(*fs);
    }
  }
  forv_Sym(ss, s->dispatch_order)
    if (done.set_add(ss))
      found = pattern_match_sym(fa, ss, cs, funs, partial_matches, match_map, cp, done) || found;
  return found;
}

static void
pattern_match_arg(FA *fa, AVar *a, PartialMatches &partial_matches, 
		  MatchMap &match_map, MPosition &p, AVar *send, Vec<MPosition *> *all_positions) 
{
  MPosition *cp = cannonical_mposition.get(&p);
  Vec<Fun *> *funs = new Vec<Fun *>;
  a->arg_of_send.set_add(send);
  if (cp && (!all_positions || all_positions->set_in(cp))) {
    forv_CreationSet(cs, *a->out) if (cs) {
      Vec<Sym *> done;
      Vec<Fun *> afuns;
      Sym *sym = cs->sym;
      if (a->var->sym->aspect) {
	if (!a->var->sym->aspect->implementors.in(cs->sym))
	  continue;
	sym = a->var->sym->aspect;
      }
      if (!pattern_match_sym(fa, sym, cs, &afuns, partial_matches.v[partial_matches.n-1], 
			     match_map, cp, done))
	continue;
      Vec<Fun *> pfuns;
      forv_Fun(f, afuns) if (f) {
	Sym *fun_arg = f->arg_syms.get(cp);
	if (fun_arg->is_pattern)
	  pfuns.set_add(f);
	else
	  funs->set_add(f);
      }
      if (cs->vars.n) {
	p.push(1);
	Vec<Fun *> *push_funs = new Vec<Fun *>(pfuns);
	partial_matches.add(push_funs);
	forv_AVar(av, cs->vars) {
	  if (av->var->sym->name && !av->var->sym->is_constant && !av->var->sym->is_symbol) {
	    MPosition pp(p);
	    pp.set_top(av->var->sym->name);
	    pattern_match_arg(fa, av, partial_matches, match_map, pp, send, all_positions);
	  } else
	    pattern_match_arg(fa, av, partial_matches, match_map, p, send, all_positions);
	  if (!partial_matches.v[partial_matches.n-1] ||
	      !partial_matches.v[partial_matches.n-1]->n)
	    break;
	  p.inc();
	}
	p.pop();
	if (partial_matches.v[partial_matches.n-1]) {
	  forv_Fun(f, *partial_matches.v[partial_matches.n-1]) if (f) {
	    Sym *fun_arg = f->arg_syms.get(cp);
	    if (fun_arg->has.n != cs->vars.n) 
	      continue;
	    Match *m = match_map.get(f);
	    if (!m) {
	      m = new Match(f);
	      match_map.put(f, m); 
	    }
	    AType *t = m->filters.get(cp);
	    if (!t) {
	      t = new AType; 
	      m->filters.put(cp, t);
	    }
	    t->set_add(cs);
	    funs->set_add(f);
	  }
	}
	partial_matches.n--;
      }
    }
  }
  partial_matches.v[partial_matches.n-1] = funs;
}

static int
best_match_sym(FA *fa, Sym *s, CreationSet *cs, Vec<Fun *> *funs, Vec<Fun *> *partial_matches, 
	       MatchMap &match_map, MPosition *cp, Vec<Sym *> &done) 
{
  if (s->match_type) {
    Vec<Fun *> *fs = s->match_type->funs.get(cp);
    if (fs) {
      Vec<Fun *> ffs;
      if (partial_matches) {
	partial_matches->set_intersection(*fs, ffs);
	fs = &ffs;
	if (fs->n) {
	  funs->set_union(*fs);
	  return 1;
	}
      }
    }
  }
  forv_Sym(ss, s->dispatch_order) 
    if (done.set_add(ss))
      if (best_match_sym(fa, ss, cs, funs, partial_matches, match_map, cp, done))
	return 1;
  return 0;
}

// for a given argument (a) at position (p) 
static void
best_match_arg(FA *fa, AVar *a, PartialMatches &partial_matches, 
	       MatchMap &match_map, MPosition &p, Vec<MPosition *> *all_positions,
	       int check_ambiguities = 0) 
{
  MPosition *cp = cannonical_mposition.get(&p);
  Vec<Fun *> *funs = new Vec<Fun *>;
  if (cp && (!all_positions || all_positions->set_in(cp))) {
    forv_CreationSet(cs, *a->out) if (cs) {
      Vec<Sym *> done;
      Vec<Fun *> afuns, pfuns;
      forv_Fun(f, *partial_matches.v[partial_matches.n-1]) if (f) {
	Sym *fun_arg = f->arg_syms.get(cp);
	if (fun_arg) {
	  if (fun_arg->is_pattern)
	    pfuns.set_add(f);
	  else
	    afuns.set_add(f);
	}
      }
      Sym *sym = cs->sym;
      if (a->var->sym->aspect) {
	if (!a->var->sym->aspect->implementors.in(cs->sym))
	  continue;
	sym = a->var->sym->aspect;
      }
      best_match_sym(fa, sym, cs, funs, &afuns, match_map, cp, done);
      if (cs->vars.n) {
	partial_matches.add(new Vec<Fun *>(pfuns));
	if (!check_ambiguities) {
	  p.push(1);
	  forv_AVar(av, cs->vars) {
	    if (av->var->sym->name && !av->var->sym->is_constant & !av->var->sym->is_symbol) {
	      MPosition pp(p);
	      pp.set_top(av->var->sym->name);
	      best_match_arg(fa, av, partial_matches, match_map, pp, all_positions, check_ambiguities);
	    } else
	      best_match_arg(fa, av, partial_matches, match_map, p, all_positions, check_ambiguities);
	    p.inc();
	  }
	} else {
	  p.push(cs->vars.n);
	  for (int i = cs->vars.n - 1; i >= 0; i--) {
	    AVar *av = cs->vars.v[i];
	    if (av->var->sym->name && !av->var->sym->is_constant & !av->var->sym->is_symbol) {
	      MPosition pp(p);
	      pp.set_top(av->var->sym->name);
	      best_match_arg(fa, av, partial_matches, match_map, pp, all_positions, check_ambiguities);
	    } else
	      best_match_arg(fa, av, partial_matches, match_map, p, all_positions, check_ambiguities);
	    p.dec();
	  }
	}
	p.pop();
	funs->set_union(*partial_matches.v[partial_matches.n-1]);
	partial_matches.n--;
      }
    }
  }
  partial_matches.v[partial_matches.n-1] = funs;
}

#if 0
static void
find_named_argument_matches(FA *fa, Vec<AVar *> args, PartialMatches &partial_matches, 
			    MatchMap &match_map, Vec<MPosition *> *all_positions) 
{
  MPosition p;
  p.push(1);
  forv_AVar(av, args) {
    pattern_match_arg(fa, av, partial_matches, match_map, p, send, all_positions);
    p.inc();
  }
}
#endif

// main dispatch entry point - given a vector of arguments return a vector of matches
int
pattern_match(FA *fa, Vec<AVar *> &args, Vec<Match *> &matches, AVar *send) {
  matches.clear();
  MatchMap match_map;
  PartialMatches partial_matches;
  Vec<MPosition *> *all_positions = NULL;
  // use summary information from previous analysis iterations to restrict the search
  if (send->var->def->callees) {
    partial_matches.add(new Vec<Fun *>(send->var->def->callees->funs));
    all_positions = &send->var->def->callees->arg_positions;
  } else
    partial_matches.add(NULL);
#if 0
  // find all named argument matches
  find_named_argument_matches(fa, args, partial_matches, match_map, send, all_positions);
  if (!partial_matches.v[0])
    return 0;
  build_positional_argument_maps(fa, args, partial_matches, match_map, send, all_positions);
#endif
  // find all matches
  {
    MPosition p;
    p.push(1);
    forv_AVar(av, args) {
      pattern_match_arg(fa, av, partial_matches, match_map, p, send, all_positions);
      p.inc();
    }
  }
  if (!partial_matches.v[0])
    return 0;
  Vec<Fun *> allfuns(*partial_matches.v[0]);
  // build all_positions if necessary
  if (!all_positions) {
    all_positions = new Vec<MPosition *>;
    forv_Fun(f, allfuns) if (f)
      all_positions->set_union(f->arg_positions);
  }
  // find best matches
  {
    MPosition p;
    p.push(1);
    forv_AVar(av, args) {
      best_match_arg(fa, av, partial_matches, match_map, p, all_positions);
      p.inc();
    }
  }
  Vec<Fun *> bestfuns(*partial_matches.v[0]);
  // check for ambiguities
  partial_matches.v[0]->copy(allfuns);
  {
    MPosition p;
    p.push(args.n);
    for (int i = args.n - 1; i >= 0; i--) {
      AVar *av = args.v[i];
      best_match_arg(fa, av, partial_matches, match_map, p, all_positions, true);
      p.dec();
    }
  }
  Vec<Fun *> finalfuns, reversebestfuns(*partial_matches.v[0]);
  partial_matches.v[0]->set_intersection(bestfuns, finalfuns);
  if (bestfuns.some_disjunction(reversebestfuns)) {
    Vec<Fun *> *ambiguities = new Vec<Fun *>;
    bestfuns.set_disjunction(reversebestfuns, *ambiguities);
    type_violation(ATypeViolation_DISPATCH_AMBIGUITY, args.v[0], args.v[0]->out, 
		   send, ambiguities);
  }
  // cannonicalize filter types
  forv_Fun(f, finalfuns) if (f) {
    Match *m = match_map.get(f);
    for (int i = 0; i < m->filters.n; i++)
      if (m->filters.v[i].key)
	m->filters.v[i].value = make_AType(*m->filters.v[i].value);
    matches.add(m);
  }
  return matches.n;
}

// build dispatch tables
void
build_patterns(FA *fa) {
  fa->patterns = new Patterns;
  build(fa);
}

// build a Fun::arg_position - positional and named argument Positions for an arg or pattern
static void
build_arg_position(Fun *f, Sym *a, MPosition &p, MPosition *parent = 0) {
  MPosition *cpnum = cannonicalize_mposition(p), *cpname = 0;
  if (a->name && !a->is_constant && !a->is_symbol) {
    MPosition pp(p);
    pp.set_top(a->name);
    cpname = cannonicalize_mposition(pp);
  }
  for (MPosition *cp = cpnum; cp; cp = cpname, cpname = 0) { 
    cp->parent = parent;
    f->arg_positions.add(cp);
    if (!a->var)
      a->var = new Var(a);
    f->arg_syms.put(cp, a);
    if (a->is_pattern) {
      MPosition pp(p);
      pp.push(1);
      forv_Sym(aa, a->has) {
	build_arg_position(f, aa, pp, cp);
	pp.inc();
      }
      pp.pop();
    }
  }
}

// build Fun::arg_positions - vec of positional and named argument Positions
// build Fun::args - map from Position's to Var's
void
build_arg_positions(FA *fa) {
  forv_Fun(f, fa->pdb->funs) {
    MPosition p;
    p.push(1);
    forv_Sym(a, f->sym->has) {
      build_arg_position(f, a, p);
      p.inc();
    }
    forv_MPosition(p, f->arg_positions) {
      Sym *s = f->arg_syms.get(p);
      f->args.put(p, s->var);
    }
    f->rets.add(f->sym->ret->var);
  }
}
