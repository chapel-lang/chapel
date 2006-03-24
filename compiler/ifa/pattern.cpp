#include "defs.h"
#include "pattern.h"
#include "pdb.h"
#include "if1.h"
#include "builtin.h"
#include "fun.h"
#include "pnode.h"
#include "fa.h"
#include "var.h"
#include "ast.h"
#include "log.h"

// Key to names of position variables
//  ABCD 
//   A = a | f, actual or formal (MANDITORY)
//   B = c, cannonical (OPTIONAL) 
//   C = p, positional (OPTIONAL) 
//   D = p, position (MANDITORY) 
// Examples:
//  acpp - actual, cannonical, positional position
//  acnp - actual, cannonical, named position
//  ap - actual non-cannonical, positional or named position

int pattern_match_hits = 0, pattern_match_complete = 0, pattern_matches = 0;

class PMatch : public Match {
 public:
  Map<MPosition *, MPosition *> actual_to_formal_position;
  Map<MPosition *, AVar *> actuals;
  Map<MPosition *, Sym *> formal_dispatch_types;
  Map<MPosition *, AType *> actual_filters; // actual -> type, point-wise and includes named arguments
  Map<MPosition *, MPosition *> actual_named_to_positional;
  Map<MPosition *, MPosition *> formal_to_actual_position;
  Map<MPosition *, MPosition *> order_substitutions;  // formal position -> actual position
  Vec<MPosition *> default_args; // formal positions
  Map<Sym *, Sym *> generic_substitutions;
  Map<MPosition *, Sym *> coercion_substitutions; // formal position -> coercion symbol

  Match *cache_copy();

  PMatch(Fun *afun) : Match(afun) {}
};
#define forv_PMatch(_p, _v) forv_Vec(PMatch, _p, _v)

typedef Map<Fun *, PMatch *> PMatchMap;
typedef MapElem<Fun *, PMatch *> PMatchElem;

class MatchCacheEntry : public Vec<Match *> {
 public:
  int is_closure;
  Partial_kind partial;
  PNode *visibility_point;
  MapMPositionAType all_args;

  MatchCacheEntry(int ais_closure, Partial_kind apartial, PNode *avp) : 
    is_closure(ais_closure), partial(apartial), visibility_point(avp) {}
};
#define forv_MatchCacheEntry(_x, _v) forv_Vec(MatchCacheEntry, _x, _v)

class MatchCache : public Vec<MatchCacheEntry *> { };

class Matcher {
 public:
  AVar *send;
  AVar *arg0;
  IFAAST *ast;
  int is_closure;
  int is_error;
  Partial_kind partial;
  PMatchMap match_map;
  Vec<Fun *> function_values; // functions passed in directly and varargs functions
  Map<MPosition *, int> mapped_positions;
  MapMPositionAType all_args;

  // C++'s manditory redundant declarations, tedious++
  int pattern_match_sym(Sym *, MPosition *, Vec<Fun *> *, Vec<Fun *> &, int);
  void pattern_match_sym_internal(Sym *, MPosition *, Vec<Fun *> &, Vec<Sym *> &);
  void pattern_match_arg(Sym *, MPosition *, Vec<Fun *> &, Vec<Fun *> &, int);
  void build_positional_map(MPosition &, int, Vec<Fun *> **);
  void update_match_map(AVar *, CreationSet *, MPosition *, MPosition *, Vec<Fun *> &);
  void find_arg_matches(AVar *, MPosition &, MPosition *, MPosition *, Vec<Fun *> **, int, int);
  void find_all_matches(CreationSet *, Vec<AVar *> &, Vec<Fun *> **, MPosition &, int);
  void find_single_match(CreationSet *, Vec<AVar *> &, Vec<Fun *> **, MPosition &, int);
  void find_best_cs_match(Vec<CreationSet *> &, MPosition &p, Vec<Fun *> &, Vec<Fun *> &, int);
  void find_best_matches(Vec<AVar *> &, Vec<CreationSet *> &, Vec<Fun *> &, MPosition &, 
                         Vec<Fun *> &, int, int iarg = 0);
  int covers_formals(Fun *, Vec<CreationSet *> &, MPosition &, int);
  PMatch *build_PMatch(Fun *, PMatch *);
  void instantiation_wrappers_and_partial_application(Vec<Fun *> &);
  Fun *build(PMatch *m, Vec<Fun *> &matches);
  void generic_arguments(Vec<CreationSet *> &, MPosition &, Vec<Fun *> &, int);
  void set_filters(Vec<CreationSet *> &, MPosition &, Vec<Fun *> &, int);
  void cannonicalize_matches(Vec<Fun *> &, int, Partial_kind, Vec<Match *> &, PNode *);

  Matcher(AVar *send, AVar *arg0, int is_closure, Partial_kind partial);
};


static void log_dispatch_cs_match(Matcher &matcher, Vec<CreationSet *> &csargs, 
                                  MPosition &app, Vec<Fun *> &local_matches);
static void log_dispatch_matches(Matcher &matcher, Vec<PMatch *> &matches);
static void log_dispatch_funs(Matcher &matcher, Vec<Fun *> &funs);

static ChainHash<MPosition *, MPositionHashFuns> cannonical_mposition;
static Map<Sym *, Map<MPosition *, Vec<Fun *> *> *> sym_match_cache;

Match *
PMatch::cache_copy() {
  Match *m = new Match(fun);
  m->formal_filters.copy(formal_filters);
  m->visibility_points.copy(visibility_points);
  m->is_partial = is_partial;
  return m;
}

void
Match::merge(Match *m) {
  // Merge Matches
  assert(fun == m->fun);
  forv_MPosition(p, fun->positional_arg_positions) {
    AType *t1 = formal_filters.get(p), *t2 = m->formal_filters.get(p);
    if (t1 && t2)
      formal_filters.put(p, type_union(t1, t2));
    else if (!t1 && t2)
      formal_filters.put(p, t2);
  }
  visibility_points.set_union(m->visibility_points);
}

static Sym *
dispatch_type(Sym *a) {
  if (is_Sym_OUT(a)) 
    return sym_unknown_type;
  if (a->is_symbol)
    return a;
  if (a->type && a->type->is_symbol)
    return a->type;
  if (a->must_specialize && a->must_specialize->type_kind != Type_VARIABLE)
    return a->must_specialize;
  return sym_any;
}

void
MPosition::copy(MPosition &p) {
  pos.copy(p.pos);
  up = p.up;
  next = p.next;
  if (p.cp == CANNONICAL_MPOSITION)
    cp = &p;
  else
    cp = p.cp;
}

MPosition::MPosition(MPosition &p) {
  copy(p);
}

MPosition *
cannonicalize_mposition(MPosition &p) {
  if (p.cp)
    return p.cp;
  MPosition *cp = cannonical_mposition.get(&p);
  if (cp) {
    p.cp = cp;
    return cp;
  }
  cp = new MPosition;
  cp->pos.copy(p.pos);
  cannonical_mposition.put(cp);
  cp->cp = CANNONICAL_MPOSITION;
  if (cp->pos.n > 1) {
    MPosition pp(*cp);
    pp.pop();
    cp->up = cannonicalize_mposition(pp);
  }
  return cp;
}

static MPosition *
to_formal(MPosition *acp, PMatch *m) {
  MPosition *fcp = m->actual_to_formal_position.get(acp);
  MPosition *result = fcp ? fcp : acp;
  return result;
}

static MPosition *
to_actual(MPosition *fcp, PMatch *m) {
  MPosition *acp = m->formal_to_actual_position.get(fcp);
  MPosition *result = acp ? acp : fcp;
  return result;
}

static MPosition *
cannonicalize_to_formal(MPosition &ap, PMatch *m) {
  return to_formal(cannonical_mposition.get(&ap), m);
}

// Given type, cp, partial_matches, return funs, using done
void
Matcher::pattern_match_sym_internal(Sym *type, MPosition *fcp, Vec<Fun *> &funs, Vec<Sym *> &done) {
  if (type->match_type) {
    Vec<Fun *> *fs = type->match_type->funs.get(fcp);
    if (fs)
      funs.set_union(*fs);
  }
  forv_Sym(tt, type->dispatch_order)
    if (done.set_add(tt))
      pattern_match_sym_internal(tt, fcp, funs, done);
}

void
Matcher::pattern_match_arg(Sym *type, MPosition *cp, Vec<Fun *> &test, Vec<Fun *> &result, int facp) {
  Vec<PMatch *> matches;
  Vec<MPosition *> positions;
  forv_Fun(f, test) if (f) {
    if (facp) {
      PMatch *m = match_map.get(f);
      cp = facp ? to_formal(cp, m) : cp;
    }
    Sym *a = f->arg_syms.get(cp);
    if (!a) {
      if (f->is_varargs)
        result.set_add(f);
      continue;
    }
    Sym *m_type = dispatch_type(a);
    if (m_type->specializers.set_in(type))
      result.set_add(f);
  }
}

// Given type, cp, partial_matches, return funs, using done
int 
Matcher::pattern_match_sym(Sym *type, MPosition *acp, Vec<Fun *> *local_matches, 
                           Vec<Fun *> &funs, int out_of_position) {
  if (out_of_position) {
    pattern_match_arg(type, acp, *local_matches, funs, 1);
    return funs.n;
  }
  pattern_match_arg(type, acp, function_values, funs, 0);
  Vec<Sym *> done;
  Vec<Fun *> new_funs;
  Map<MPosition *, Vec<Fun *> *> *pfm = sym_match_cache.get(type);
  Vec<Fun *> *vfn = 0;
  if (pfm) {
    vfn = pfm->get(acp);
    if (vfn)
      goto Ldone;
  }
  vfn = new Vec<Fun *>;
  pattern_match_sym_internal(type, acp, *vfn, done);
  if (!pfm)
    sym_match_cache.put(type, (pfm = new Map<MPosition *, Vec<Fun *> *>));
  pfm->put(acp, vfn);
 Ldone:
  if (local_matches) {
    if (local_matches->n > vfn->n) {
      forv_Fun(f, *vfn) if (f)
        if (local_matches->set_in(f))
          funs.set_add(f);
    } else {
      forv_Fun(f, *local_matches) if (f)
        if (vfn->set_in(f))
          funs.set_add(f);
    }
  } else
    funs.set_union(*vfn);
  return funs.n;
}

Matcher::Matcher(AVar *asend, AVar *aarg0, int ais_closure, Partial_kind apartial) {
  send = asend;
  arg0 = aarg0;
  is_closure = ais_closure;
  is_error = 0;
  partial = apartial;
  ast = asend->var->def->code->ast;
}

void
Matcher::update_match_map(AVar *a, CreationSet *cs, MPosition *acp, MPosition *acpp,
                          Vec<Fun *> &new_matches) 
{
  forv_Fun(f, new_matches) if (f) {
    PMatch *m = match_map.get(f);
    if (!m) {
      m = new PMatch(f);
      match_map.put(f, m); 
    }
    AType *t = m->actual_filters.get(acp);
    if (!t) {
      t = new AType; 
      m->actual_filters.put(acp, t);
      if (acp != acpp)
        m->actual_named_to_positional.put(acp, acpp);
    }
    t->set_add(cs);
    m->actuals.put(acpp, a);
  }
}

int
positional_to_named(CreationSet *cs, AVar *av, MPosition &pp, MPosition *np) {
  if ((!cs || cs->sym == sym_tuple) && av->var->sym->arg_name) {
    np->copy(pp);
    np->set_top(av->var->sym->arg_name);
    return 1;
  }
  return 0;
}

// This is an approximation.  It should really take into account
// the specific CreationSet which contributed at each MPosition
static void
add_all_args(MapMPositionAType &m, MPosition *cp, AType *t) {
  AType *tt = m.get(cp);
  if (!tt)
    m.put(cp, t);
  else
    m.put(cp, type_union(tt, t));
}

void
Matcher::find_arg_matches(AVar *a, MPosition &ap, MPosition *acp, MPosition *acpp, 
                          Vec<Fun *> **partial_matches, int recurse, int out_of_position)
{
  a->arg_of_send.add(send);
  Vec<Fun *> funs;
  add_all_args(all_args, acpp, a->out);
  forv_CreationSet(cs, a->out->sorted) {
    Sym *sym = cs->sym;
    if (a->var->sym->aspect) {
      if (!a->var->sym->aspect->specializers.set_in(cs->sym) &&
          (cs->sym != sym_nil_type || !sym_object->specializers.set_in(a->var->sym->aspect)))
        continue;
      sym = a->var->sym->aspect;
    }
    Vec<Fun *> new_funs;
    if (!pattern_match_sym(sym, acp, *partial_matches, new_funs, out_of_position))
      continue;
    update_match_map(a, cs, acp, acpp, new_funs);
    if (recurse && cs->vars.n) { // for recursive ap == app
      Vec<Fun *> rec_funs, tfuns, *rec_funs_p = &rec_funs;
      tfuns.move(new_funs);
      forv_Fun(f, tfuns) if (f) {
        Sym *a = f->arg_syms.get(to_formal(acpp, match_map.get(f)));
        assert(a || f->is_varargs);
        if (a && a->is_pattern)
          rec_funs.set_add(f);
        else
          new_funs.set_add(f);
      }
      if (rec_funs.n) {
        find_all_matches(cs, cs->vars, &rec_funs_p, ap, out_of_position);
        funs.set_union(rec_funs);
      }
    }
    funs.set_union(new_funs);
  }
  if (!*partial_matches) {
    *partial_matches = new Vec<Fun *>(funs);
  } else {
    Vec<Fun *> saved_matches;
    saved_matches.move(**partial_matches);
    forv_Fun(f, saved_matches) if (f) {
      if (funs.set_in(f))
        (*partial_matches)->set_add(f);
      else if (acpp->up && !f->arg_syms.get(to_formal(acpp->up, match_map.get(f)))->is_pattern)
        (*partial_matches)->set_add(f); 
    }
    (*partial_matches)->set_union(funs);
  }
}

int
compar_last_position(const void *ai, const void *aj) {
  MPosition *i = *(MPosition**)ai;
  MPosition *j = *(MPosition**)aj;
  if (i->pos.n > j->pos.n)
    return 1;
  else if (i->pos.n < j->pos.n)
    return -1;
  int ii = Position2int(i->pos.v[i->pos.n-1]);
  int jj = Position2int(j->pos.v[j->pos.n-1]);
  return (ii > jj) ? 1 : ((ii < jj) ? -1 : 0);
}

void
Matcher::build_positional_map(MPosition &app, int nactuals, Vec<Fun *> **funs) {
  if (*funs) {
    forv_Fun(f, **funs) if (f) {
      PMatch *m = match_map.get(f);
      Vec<MPosition *> fcpps_for_fcnps, acpps_for_acnps, unused_acpps, unused_fcpps;
      // map and collect named argument positions (at this level)
      for (int i = 0; i < m->actual_filters.n; i++) {
        MPosition *acp = m->actual_filters.v[i].key;
        if (acp && !acp->last_is_positional() && acp->prefix_to_last(app)) {
          MPosition *acnp = acp;
          MPosition fnp;
          if (acp->up)
            fnp.copy(*to_formal(acp->up, m));
          fnp.push(acp->last());
          MPosition *fcnp = cannonicalize_mposition(fnp);
          MPosition *acpp = m->actual_named_to_positional.get(acnp);
          MPosition *fcpp = f->named_to_positional.get(fcnp);
          acpps_for_acnps.set_add(acpp);
          fcpps_for_fcnps.set_add(fcpp);
          m->actual_to_formal_position.put(acpp, fcpp);
          m->formal_to_actual_position.put(fcpp, acpp);
          if (acpp != fcpp)
            mapped_positions.put(acpp, 1);
        }
      }
      app.push(1);
      // collect actual positions not used by named arguments (at this level)
      for (int i = 0; i < nactuals; i++) {
        MPosition *acpp = cannonicalize_mposition(app);
        if (!acpps_for_acnps.set_in(acpp))
          unused_acpps.add(acpp);
        app.inc();
      }
      app.pop();
      // sort unused positions
      qsort(unused_acpps.v, unused_acpps.n, sizeof(void*), compar_last_position);
      // build a maps for actual positionals <-> formal positionals
      MPosition fpp(*cannonicalize_to_formal(app, m));
      fpp.push(1);
      for (int i = 0; i < unused_acpps.n; i++) {
        MPosition *fcpp = cannonicalize_mposition(fpp);
        while (fcpps_for_fcnps.set_in(fcpp)) {
          fpp.inc();
          fcpp = cannonicalize_mposition(fpp);
        }
        m->actual_to_formal_position.put(unused_acpps.v[i], fcpp);
        m->formal_to_actual_position.put(fcpp, unused_acpps.v[i]);
        if (unused_acpps.v[i] != fcpp)
          mapped_positions.put(unused_acpps.v[i], 1);
        fpp.inc();
      }
    }
  }
}

void
Matcher::find_all_matches(CreationSet *cs, Vec<AVar *> &args, Vec<Fun *> **partial_matches, 
                          MPosition &app, int out_of_position)
{
  // determine named arguments
  int some_named = 0;
  app.push(1);
  forv_AVar(av, args) {
    MPosition anp;
    if (positional_to_named(cs, av, app, &anp)) {
      MPosition *acpp = cannonicalize_mposition(app);
      if (acpp) {
        MPosition *acnp = cannonicalize_mposition(anp);
        find_arg_matches(av, anp, acnp, acpp, partial_matches, 0, out_of_position);
        some_named = 1;
      }
    }
    app.inc();
  }
  // build positional map
  app.pop();
  if (some_named)
    build_positional_map(app, args.n, partial_matches);
  // match all arguments
  app.push(1);
  forv_AVar(av, args) {
    MPosition *acpp = cannonicalize_mposition(app);
    if (acpp) {
      int local_out_of_position = out_of_position;
      if (!local_out_of_position) {
        if (mapped_positions.get(acpp))
          local_out_of_position = 1;
      }
      find_arg_matches(av, app, acpp, acpp, partial_matches, 1, local_out_of_position);
    }
    app.inc();
  }
  app.pop();
}

int
subsumes(PMatch *x, PMatch *y, MPosition &app, int nargs) {
  int result = 0, identical = 1;
  app.push(1);
  for (int i = 0; i < nargs; i++) {
    MPosition *acpp = cannonicalize_mposition(app);
    Sym *xtype = x->formal_dispatch_types.get(to_formal(acpp, x));
    Sym *ytype = y->formal_dispatch_types.get(to_formal(acpp, y));
    if (xtype == ytype)
      goto Lnext;
    identical = 0;
    if (xtype->specializers.set_in(ytype)) {
      if (ytype->specializers.set_in(xtype)) {
        result = 0;
        goto Lreturn;
      }
      if (result >= 0)
        result = 1;
      else {
        result = 0;
        goto Lreturn;
      }
    } else if (ytype->specializers.set_in(xtype)) {
      if (xtype->specializers.set_in(ytype)) {
        result = 0;
        goto Lreturn;
      }
      if (result <= 0)
        result = -1;
      else {
        result = 0;
        goto Lreturn;
      }
    }
  Lnext:
    app.inc();
  }
  if (identical && !result) {
    if (x->fun->sym->nesting_depth > y->fun->sym->nesting_depth)
      result = -1;
    if (x->fun->sym->nesting_depth < y->fun->sym->nesting_depth)
      result = 1;
  }
 Lreturn:
  app.pop();
  return result;
}

void
Matcher::set_filters(Vec<CreationSet *> &csargs, MPosition &app, Vec<Fun *> &matches, 
                     int top_level) 
{
  Vec<Fun *> mm;
  mm.move(matches);
  forv_Fun(f, mm) {
    PMatch *m = match_map.get(f);
    app.push(1);
    for (int i = 0; i < csargs.n; i++) {
      MPosition *fcpp = cannonicalize_to_formal(app, m);
      AType *t = m->formal_filters.get(fcpp);
      if (!t) {
        t = new AType; 
        m->formal_filters.put(fcpp, t);
      }
      t->set_add(csargs.v[i]);
      app.inc();
    }
    app.pop();
    if (top_level) {
      // re-verify filters after instantiation
      int ok = 1;
      forv_MPosition(p, f->positional_arg_positions) {
        AType *t = m->formal_filters.get(p);
        if (t) {
          Sym *m_type = dispatch_type(m->fun->arg_syms.get(p));
          Sym *actual = m->actuals.get(to_actual(p, m))->var->sym;
          Vec<CreationSet *> newt;
          forv_CreationSet(cs, *t) if (cs) {
            if (cs->sym == sym_nil_type && actual->aspect &&
                sym_object->specializers.set_in(actual->aspect)) {
              if (m_type->specializers.set_in(actual->aspect))
                newt.set_add(cs);
            } else 
              if (m_type == cs->sym || m_type->specializers.set_in(cs->sym->type))
                newt.set_add(cs);
          }
          if (!newt.n)
            ok = 0;
          t->move(newt);
        }
      }
      if (ok)
        matches.add(f);
    } else
      matches.add(f);
  }
}

PMatch *
Matcher::build_PMatch(Fun *f, PMatch *m) {
  PMatch *mm = match_map.get(f);
  if (!mm) {
    mm = new PMatch(f);
    match_map.put(f, mm);
  }
  assert(m != mm);
  // NOTE: ensure handling is correct destructuring
  mm->actuals.copy(m->actuals);
  mm->formal_dispatch_types.copy(m->formal_dispatch_types);
  mm->actual_filters.clear(); // not needed
  form_MPositionAType(x, m->formal_filters) {
    if (AType *t = mm->formal_filters.get(x->key))
      t->set_union(*x->value);
    else
      mm->formal_filters.put(x->key, x->value);
  }
  mm->actual_named_to_positional.clear(); // not needed
  mm->actual_to_formal_position.copy(m->actual_to_formal_position);
  mm->formal_to_actual_position.copy(m->formal_to_actual_position);
  mm->order_substitutions.copy(m->order_substitutions);
  mm->default_args.copy(m->default_args);
  mm->generic_substitutions.copy(m->generic_substitutions);
  mm->coercion_substitutions.copy(m->coercion_substitutions);
  mm->is_partial = m->is_partial;
  return mm;
}

template <class C>
class MapCacheHashFns : public gc {
 public:
  static unsigned int hash(C a) {
    unsigned int h = 0, x = 0;
    for (int i = 0; i < a->n; i++) if (a->v[i].key) {
      h += open_hash_multipliers[x++ % 256] * (uintptr_t)a->v[i].key;
      h += open_hash_multipliers[x++ % 256] * (uintptr_t)a->v[i].value;
    }
    return h;
  }
  static int equal(C a, C b) {
    for (int i = 0; i < a->n; i++)
      if (a->v[i].key && b->get(a->v[i].key) != a->v[i].value)
        return 0;
    for (int i = 0; i < b->n; i++)
      if (b->v[i].key && a->get(b->v[i].key) != b->v[i].value)
        return 0;
    return 1;
  }
};
typedef Map<MPosition *, Sym*> CoercionMap;
typedef Map<Sym *, Sym*> GenericMap;
typedef Map<MPosition *, MPosition*> OrderMap;
class CoercionCache : public gc { 
 public: HashMap<CoercionMap *, MapCacheHashFns<CoercionMap *>, Fun *> cache; };
class GenericCache : public gc { 
 public: HashMap<GenericMap *, MapCacheHashFns<GenericMap *>, Fun *> cache; };
class OrderCache : public gc { 
 public: HashMap<OrderMap *, MapCacheHashFns<OrderMap *>, Fun *> cache; };

class DefaultCacheHashFns  {
 public:
  static unsigned int hash(Vec<MPosition*> *a) {
    unsigned int h = 0, x = 0;
    forv_MPosition(p, *a)
      h += open_hash_multipliers[x++ % 256] * (uintptr_t)a;
    return h;
  }
  static int equal(Vec<MPosition *> *a,  Vec<MPosition *> *b) {
    if (a->n != b->n)
      return 0;
    for (int i = 0; i < a->n; i++)
      if (a->v[i] != b->v[i])
        return 0;
    return 1;
  }
};
class DefaultCache : public gc { public: HashMap<Vec<MPosition*> *, DefaultCacheHashFns, Fun *> cache; };

static void
fixup_maps(PMatch *m, Map<MPosition *, MPosition *> &formal_to_formal) {
  Map<MPosition *, MPosition *> a2f, f2a;
  a2f.move(m->actual_to_formal_position);
  f2a.move(m->formal_to_actual_position);
  form_MPositionMPosition(x, a2f) {
    if (MPosition *y = formal_to_formal.get(x->value))
      m->actual_to_formal_position.put(x->key, y);
  }
  form_MPositionMPosition(x, f2a) {
    if (MPosition *y = formal_to_formal.get(x->key))
      m->formal_to_actual_position.put(y, x->value);
  }
  Map<MPosition *, AType *> formal_filters;
  formal_filters.move(m->formal_filters);
  form_MPositionAType(x, formal_filters) {
    if (MPosition *y = formal_to_formal.get(x->key))
      m->formal_filters.put(y, x->value);
  }
}

static void
fixup_maps_for_defaults(PMatch *m, Fun *oldf, Vec<MPosition *> &defaults) {
  Vec<MPosition *> defaults_set;
  defaults_set.set_union(defaults);
  Map<MPosition *, MPosition *> formal_to_formal;
  forv_MPosition(p, oldf->positional_arg_positions) {
    // if that formal position (or any above) nolonger exists, skip it
    MPosition tmp(*p), newp;
    for (int i = p->pos.n; i <= 0; i--) {
      tmp.pos.n = i;
      tmp.cp = 0; // not cannonical
      if (defaults_set.set_in(cannonicalize_mposition(tmp)))
        goto Lskip;
    }
    // at each level, count those before which nolonger exist and
    // subtract from newp
    for (int i = 0; i < p->pos.n; i++) {
      int offset = 0;
      tmp.copy(*p);
      tmp.pos.n = i + 1;
      for (int j = 0; j < Position2int(p->pos.v[i]); j++) {
        tmp.pos.v[i] = int2Position(j + 1);
        tmp.cp = 0; // not cannonical
        if (defaults_set.set_in(cannonicalize_mposition(tmp)))
          offset++;
      }
      newp.push(int2Position(Position2int(p->pos.v[i]) - offset));
    }
    formal_to_formal.put(p, cannonicalize_mposition(newp));
  Lskip:;
  }
  fixup_maps(m, formal_to_formal);
}

Fun *
Matcher::build(PMatch *m, Vec<Fun *> &done_matches) {
  Fun *f = m->fun, *fnew;
  if (m->generic_substitutions.n) {
    GenericCache *c = f->generic_cache;
    if (!c) c = f->generic_cache = new GenericCache;
    if (!(fnew = c->cache.get(&m->generic_substitutions))) {
      f = if1->callback->instantiate_generic(f, m->generic_substitutions);
      c->cache.put(new GenericMap(m->generic_substitutions), f);
    } else
      f = fnew;
    if (done_matches.set_in(f))
      return 0;
    done_matches.set_add(f);
    m->generic_substitutions.clear();
    m = build_PMatch(f, m);
  }
  if (m->default_args.n) {
    qsort(m->default_args.v, m->default_args.n, sizeof(void*), compar_last_position);
    DefaultCache *c = f->default_cache;
    Fun *oldf = f;
    if (!c) c = f->default_cache = new DefaultCache;
    if (!(fnew = c->cache.get(&m->default_args))) {
      f = if1->callback->default_wrapper(f, m->default_args);
      c->cache.put(new Vec<MPosition*>(m->default_args), f);
    } else
      f = fnew;
    if (done_matches.set_in(f))
      return 0;
    done_matches.set_add(f);
    Vec<MPosition *> defaults;
    defaults.move(m->default_args);
    m = build_PMatch(f, m);
    fixup_maps_for_defaults(m, oldf, defaults);
  }
  if (m->coercion_substitutions.n) {
    CoercionCache *c = f->coercion_cache;
    if (!c) c = f->coercion_cache = new CoercionCache;
    if (!(fnew = c->cache.get(&m->coercion_substitutions))) {
      f = if1->callback->coercion_wrapper(f, m->coercion_substitutions);
      c->cache.put(new CoercionMap(m->coercion_substitutions), f);
    } else
      f = fnew;
    if (done_matches.set_in(f))
      return 0;
    done_matches.set_add(f);
    m->coercion_substitutions.clear();
    m = build_PMatch(f, m);
  }
  int order_change = 0;
  forv_MPosition(p, f->positional_arg_positions) {
    MPosition *acp = to_actual(p, m);
    if (p != acp)
      order_change = 1;
    m->order_substitutions.put(p, acp);
  }
  if (order_change) {
    OrderCache *c = f->order_cache;
    if (!c) c = f->order_cache = new OrderCache;
    if (!(fnew = c->cache.get(&m->order_substitutions))) {
      f = if1->callback->order_wrapper(f, m->order_substitutions);
      c->cache.put(new OrderMap(m->order_substitutions), f);
    } else
      f = fnew;
    if (done_matches.set_in(f))
      return 0;
    done_matches.set_add(f);
    Map<MPosition *, MPosition *> order_substitutions;
    order_substitutions.move(m->order_substitutions);
    m = build_PMatch(f, m);
    fixup_maps(m, order_substitutions);
  } else
    m->order_substitutions.clear();
  return f;
}

void 
Matcher::instantiation_wrappers_and_partial_application(Vec<Fun *> &matches) {
  Vec<Fun *> new_matches, complete, done_matches;
  done_matches.set_union(matches);
  forv_Fun(f, matches) {
    PMatch *m = match_map.get(f);
    if (!m->is_partial)
      f = build(m, done_matches);
    if (!f) 
      continue;
    if (!m->is_partial)
      complete.set_add(f);
    if (f->is_generic && !m->is_partial)
      fail("instantiation failure for function '%s'", f->sym->name ? f->sym->name : "<unknown>");
    new_matches.set_add(f);
  }
  complete.set_to_vec();
  new_matches.set_to_vec();
  if (complete.n > 1) {
    type_violation(ATypeViolation_DISPATCH_AMBIGUITY, arg0, arg0->out, send, 
                   new Vec<Fun *>(complete));
    is_error = 1;
  }
  matches.move(new_matches);
}

int
Matcher::covers_formals(Fun *f, Vec<CreationSet *> &csargs, MPosition &p, int top_level) {
  if (f->is_varargs)
    return 1;
  int result = 1;
  PMatch *m = match_map.get(f);
  Vec<MPosition *> formals;
  p.push(1);
  for (int i = 0; i < csargs.n; i++) {
    MPosition *cp = cannonicalize_to_formal(p, m);
    formals.set_add(cp);
    p.inc();
  }
  p.pop();
  for (int i = 0; i < f->arg_positions.n; i++) {
    MPosition *pp = f->arg_positions.v[i];
    if (pp->prefix_to_last(p) && pp->last_is_positional()) {
      if (formals.set_in(pp))
        continue;
      if (f->default_args.get(pp)) {
        m->default_args.add(pp);
        continue;
      }
      if (top_level && partial != Partial_NEVER)
        m->is_partial = 1;
      else
        result = 0;
    }
  }
  if (top_level && result) {
    // handle x.y(z) as ((#y, x), z) differently for paren vs. paren-less methods
    if (partial == Partial_OK && !f->is_eager)
      m->is_partial = 1;
    // ensure that lazy Partial_OKs are returned as closures
    if (partial == Partial_NEVER && f->is_lazy && !is_closure)
      result = 0;
    // ensure that eager functions are not called from a closure
    if (f->is_eager && is_closure)
      result = 0;
  }
  return result;
}

static int
unify_instantiated(Sym *generic_type, Sym *concrete_type, Map<Sym *, Sym *> &substitutions) {
  Sym *sym = concrete_type;
  while (sym != generic_type) {
    substitutions.map_union(sym->substitutions);
    sym = sym->instantiates;
    if (!sym)
      return 0;
  }
  return 1;
}

static int
make_generic_substitution(IFAAST *ast, Sym *generic_type, Sym *concrete_type, Map<Sym *, Sym *> &substitutions) {
  Map<Sym *, Sym *> subs;
  if (unify_instantiated(generic_type, concrete_type, subs)) {
    for (int i = 0; i < subs.n; i++)
      if (!make_generic_substitution(ast, subs.v[i].key, subs.v[i].value, substitutions))
        return 0;
  }
  Sym *old = substitutions.get(generic_type);    
  if (old && old != concrete_type) {
    // conflicting generic variable substitutions
    return 0;
  }
  substitutions.put(generic_type, concrete_type);
  return 1;
}

static int
unify_generic_type(Sym *formal, Sym *generic_type, Sym *concrete_value, Map<Sym *, Sym *> &substitutions,
                   IFAAST *ast) 
{
  Sym *concrete_type = concrete_value->type;
  if (concrete_type == sym_nil_type)
    return 0;
  if (formal->is_generic) {
    if (generic_type == concrete_value)
      return make_generic_substitution(ast, formal, concrete_value, substitutions);
    if (concrete_type->is_meta_type) {
      if (!generic_type->is_meta_type)
        return 0;
      Sym *generic = 0;
      int bind_to_value = 0;
      if (!if1->callback->formal_to_generic(formal, &generic, &bind_to_value))
        return 1;
      if (generic_type->specializers.set_in(concrete_type))
        return make_generic_substitution(ast, generic, concrete_value->meta_type, 
                                         substitutions);
      return 0;
    }
    if (generic_type->specializers.set_in(concrete_type)) {
      Sym *generic = 0;
      int bind_to_value = 0;
      if (!if1->callback->formal_to_generic(formal, &generic, &bind_to_value))
        return 1;
      return make_generic_substitution(ast, generic, bind_to_value ? concrete_value : concrete_type, 
                                       substitutions);
    }
    return 0;
  } else {
    Sym *type = substitutions.get(generic_type);
    if (!type) 
      type = substitutions.get(generic_type->meta_type);
    if (type == concrete_type)
      return 1;
    if (make_generic_substitution(ast, generic_type, concrete_type, substitutions))
      return 1;
    return 0;
  }
  if (generic_type == concrete_type)
    return 1;
#if 0
  // treat type variables as ?t
  if (generic_type->type_kind == Type_VARIABLE)
    return make_generic_substitution(ast, generic_type, concrete_type, substitutions);
#endif
  return 0;
}

static Sym *
get_generic_type(Sym *formal) {
  Sym *formal_type = formal->must_specialize;
  if (formal->is_generic) {
    if (formal_type)
      return formal_type;
    return formal->type;
  }
  if (formal_type && formal_type->type_kind == Type_VARIABLE)
    return formal_type;
  if (formal_type && formal_type->is_generic)
    return formal_type;
  if (formal->type && formal->type->type_kind == Type_VARIABLE)
    return formal->type;
  return 0;
}

static void
instantiate_formal_types(PMatch *m) {
  form_MPositionSym(x, m->formal_dispatch_types) {
    Sym *formal = m->fun->arg_syms.get(x->key);
    Sym *type = get_generic_type(formal);
    if (type) {
      Sym *generic = formal->is_generic ? formal : type;
      Sym *instantiated = if1->callback->instantiate(generic, m->generic_substitutions);
      if (instantiated)
        x->value = instantiated;
    }
  }
}

static int
generic_substitutions(PMatch **am, MPosition &app, Vec<CreationSet*> &args, IFAAST *ast) {
  MPosition local_app(app);
  PMatch *m = *am;
  if (!m->fun->is_generic)
    return 1;
  local_app.push(1);
  for (int i = 0; i < args.n; i++) {
    MPosition *acpp = cannonicalize_mposition(local_app);
    MPosition *fcpp = to_formal(acpp, m);
    AVar *a = m->actuals.get(acpp);
    CreationSet *cs = args.v[i];
    Sym *concrete_value = a->var->sym->aspect ? a->var->sym->aspect : cs->sym;
    Sym *formal = m->fun->arg_syms.get(fcpp);
    if (!formal && m->fun->is_varargs)
      goto Lnext; 
    {
      Sym *generic_type = get_generic_type(formal);
      if (generic_type) {
        if (!unify_generic_type(formal, generic_type, concrete_value, m->generic_substitutions, ast))
          return 0;
        instantiate_formal_types(m);
      }
    }
   Lnext:;
    local_app.inc();
  }
  if (m->fun->is_generic && !m->generic_substitutions.n && !m->is_partial)
    fail("unification failure for generic function '%s'", 
         m->fun->sym->name ? m->fun->sym->name : "<unknown>");
  return 1;
}

static void
coercion_uses(PMatch **am, MPosition &app, Vec<CreationSet*> &args) {
  PMatch *m = *am;
  app.push(1);
  for (int i = 0; i < args.n; i++) {
    MPosition *acpp = cannonicalize_mposition(app);
    MPosition *fcpp = to_formal(acpp, m);
    AVar *a = m->actuals.get(acpp);
    CreationSet *cs = args.v[i];
    Sym *concrete_type = a->var->sym->aspect ? a->var->sym->aspect : cs->sym;
    concrete_type = concrete_type->type;
    Sym *formal_type = m->formal_dispatch_types.get(fcpp);
    if (!formal_type && m->fun->is_varargs)
      goto LnextArg;
    {
      Sym *coerced_type = concrete_type->coerce_to(formal_type) ;
      if (coerced_type && concrete_type != coerced_type) {
        m->coercion_substitutions.put(fcpp, concrete_type);
        assert(coerced_type);
        m->formal_dispatch_types.put(fcpp, coerced_type);
      }
    }
   LnextArg:;
    app.inc();
  }
  app.pop();
}

// clear out values needed only while considering a particular csargs
static void
clear_matches(PMatchMap &match_map) {
  form_Map(PMatchElem, x, match_map) {
    x->value->order_substitutions.clear();
    x->value->default_args.clear();
    x->value->generic_substitutions.clear();
    x->value->coercion_substitutions.clear();
  }
}

void
Matcher::find_best_cs_match(Vec<CreationSet *> &csargs, MPosition &app, 
                            Vec<Fun *> &local_matches, 
                            Vec<Fun *> &result, int top_level)
{
  log_dispatch_cs_match(*this, csargs, app, local_matches);
  Vec<PMatch *> covered;
  // collect the matches which cover the argument CreationSets
  forv_Fun(f, local_matches) if (f) {
    PMatch *m = match_map.get(f);
    app.push(1);
    for (int i = 0; i < csargs.n; i++) {
      MPosition *acpp = cannonicalize_mposition(app);
      MPosition *fcpp = to_formal(acpp, m);
      AType *t = m->actual_filters.get(acpp);
      Sym *formal = m->fun->arg_syms.get(fcpp);
      if (!formal && f->is_varargs)
        goto LnextCoverArg;
      // is each actual handled
      if (!t || !t->set_in(csargs.v[i])) {
        app.pop();
        goto LnextFun;
      }
      {
        Sym *formal_type = dispatch_type(formal);
        assert(formal_type);
        if (formal->is_exact_match) {
          if (csargs.v[i]->sym != formal_type 
              && csargs.v[i]->sym->type != formal_type // HACK FOR pre-instantiation
              && csargs.v[i]->sym->type->meta_type != formal_type // HACK FOR pre-instantiation
            ) {
            app.pop();
            goto LnextFun;
          }
        }
        if (formal->is_this) {
          if (csargs.v[i]->sym == sym_nil_type) {
            app.pop();
            goto LnextFun;
          }
        }
        m->formal_dispatch_types.put(fcpp, formal_type);
      }
     LnextCoverArg:;
      app.inc();
    }
    app.pop();
    // do they cover all the formals
    if (covers_formals(f, csargs, app, top_level))
      covered.set_add(m);
  LnextFun:;
  }
  covered.set_to_vec();
  log(LOG_DISPATCH, "%d- cover_formals: %d\n", send->var->sym->id, covered.n);
  log_dispatch_matches(*this, covered);
  if (!covered.n)
    return;

  Vec<PMatch *> applicable;
  // record generic substitutions and point-wise applications
  for (int i = 0; i < covered.n; i++) {
    if (!generic_substitutions(&covered.v[i], app, csargs, ast))
      continue;
    coercion_uses(&covered.v[i], app, csargs);
    applicable.add(covered.v[i]);
  }

  Vec<PMatch *> unsubsumed, subsumed;
  // eliminate those which are subsumed by some other function
  for (int i = 0; i < applicable.n; i++) if (applicable.v[i]) {
    if (subsumed.set_in(applicable.v[i]))
      continue;
    for (int j = i + 1; j < applicable.n; j++) if (applicable.v[j]) {
      switch (subsumes(applicable.v[i], applicable.v[j], app, csargs.n)) {
        case -1: subsumed.set_add(applicable.v[j]); break;
        case 0: break;
        case 1: goto LnextApplicable;
      }
    }
    unsubsumed.add(applicable.v[i]);
  LnextApplicable:;
  }
  log(LOG_DISPATCH, "%d- unsubsumed: %d\n", send->var->sym->id, unsubsumed.n);
  log_dispatch_matches(*this, unsubsumed);

  // for those remaining, check for ambiguities
  Vec<Fun *> matches, similar, done;
  for (int i = 0; i < unsubsumed.n; i++) {
    PMatch *x = unsubsumed.v[i];
    if (done.set_in(x->fun))
      continue;
    similar.clear(); similar.add(x->fun);
    for (int j = i + 1; j < unsubsumed.n; j++) {
      PMatch *y = unsubsumed.v[j];
      if (done.set_in(y->fun))
        continue;
      app.push(1);
      for (int k = 0; k < csargs.n; k++) {
        Sym *xarg = x->fun->arg_syms.get(cannonicalize_to_formal(app, x));
        Sym *yarg = y->fun->arg_syms.get(cannonicalize_to_formal(app, y));
        Sym *xtype = dispatch_type(xarg);
        Sym *ytype = dispatch_type(yarg);
        if (xtype != ytype || xarg->is_pattern != yarg->is_pattern)
          goto LnextUnsubsumed;
        app.inc();
      }
      similar.add(y->fun); done.set_add(y->fun);
    LnextUnsubsumed:;
      app.pop();
    }
    log(LOG_DISPATCH, "%d- unambiguous: %d\n", send->var->sym->id, similar.n);

    // for similar functions recurse for patterns to check for subsumption and ambiguities
    app.push(1);
    for (int i = 0; i < csargs.n; i++) {
      Sym *arg = similar.v[0]->arg_syms.get(cannonicalize_to_formal(app, match_map.get(similar.v[0])));
      if (!arg && similar.v[0]->is_varargs)
        goto LnextSimilarArg; 
      if (arg->is_pattern) {
        Vec<CreationSet *> local_csargs;
        Vec<Fun *> local_result;
        find_best_matches(csargs.v[i]->vars, local_csargs, similar, app, local_result, 0);
        similar.copy(local_result);
      }
     LnextSimilarArg:;
      app.inc();
    }
    app.pop();
    matches.append(similar);
  }
  if (top_level)
    instantiation_wrappers_and_partial_application(matches);
  set_filters(csargs, app, matches, top_level);
  log(LOG_DISPATCH, "%d- destructure_level: %d matches: %d\n", send->var->sym->id, 
      app.pos.n, matches.n);
  qsort_by_id(matches);
  log_dispatch_funs(*this, matches);
  result.set_union(matches);
  clear_matches(match_map);
}

void
Matcher::find_best_matches(Vec<AVar *> &args, Vec<CreationSet *> &csargs, 
                           Vec<Fun *> &matches, MPosition &app, 
                           Vec<Fun *> &result, int top_level, int iarg) 
{
  if (iarg >= args.n)
    find_best_cs_match(csargs, app, matches, result, top_level);
  else {
    csargs.fill(iarg + 1);
    forv_CreationSet(cs, args.v[iarg]->out->sorted) {
      csargs.v[iarg] = cs;
      find_best_matches(args, csargs, matches, app, result, top_level, iarg + 1);
    }
  }
}

void
Matcher::cannonicalize_matches(Vec<Fun *> &partial_matches, int is_closure, Partial_kind partial, 
                               Vec<Match *> &matches, PNode *visibility_point) 
{
  partial_matches.set_to_vec();
  qsort_by_id(partial_matches);
  forv_Fun(f, partial_matches) {
    PMatch *pm = match_map.get(f);
    Match *m = pm->cache_copy();
    assert(m->fun == f);
    for (int i = 0; i < m->formal_filters.n; i++)
      if (m->formal_filters.v[i].key)
        m->formal_filters.v[i].value = make_AType(*m->formal_filters.v[i].value);
    m->visibility_points.set_add(visibility_point);
    matches.add(m);
  }
  if (!is_error) {
    MatchCache *mc = send->match_cache;
    if (!mc)
      mc = send->match_cache = new MatchCache;
    MatchCacheEntry *e = new MatchCacheEntry(is_closure, partial, visibility_point);
    mc->add(e);
    e->all_args.copy(all_args);
    forv_Match(m, matches)
      e->add(new Match(*m));
  }
}

static void 
log_dispatch_pattern_match(Vec<Fun *> *visible, Vec<Fun *> &fnvalues, AVar *send) {
  if (visible)
    log(LOG_DISPATCH, "%d- pass: %d visible: %d, fn values: %d\n", 
        send->var->sym->id, analysis_pass, visible->count(), fnvalues.count());
  else
    log(LOG_DISPATCH, "%d- pass: %d visible: ALL\n", 
        send->var->sym->id, analysis_pass);
}

static void
find_visible_functions(Vec<AVar *> &args, PNode *visibility_point, Vec<Fun *> **visible_functions, 
                       Vec<Fun *> &function_values) 
{
  AVar *aarg0 = args.v[0];
  *visible_functions = 0;
  Vec<Fun *> *visible = 0;
  // use summary information from previous analysis iterations to restrict the search
  if (aarg0->out->n == 1 && aarg0->out->v[0]->sym->fun) {
    function_values.set_add(aarg0->out->v[0]->sym->fun);
    *visible_functions = new Vec<Fun *>;
    return;
  } else {
    if (aarg0->out->n == 1) {
      visible = visibility_point->code->ast->visible_functions(aarg0->out->v[0]->sym);
    } else {
      forv_CreationSet(cs, aarg0->out->sorted) {
        if (cs->sym->fun)
          function_values.set_add(cs->sym->fun);
        else {
          Vec<Fun *> *v = visibility_point->code->ast->visible_functions(cs->sym);
          if (v) {
            if (!*visible_functions)
              *visible_functions = new Vec<Fun *>;
            (*visible_functions)->set_union(*v);
          }
        }
      }
    }
    Vec<Fun *> tmp;
    if (visible) // needs to be copied
      *visible_functions = new Vec<Fun *>;
    else {
      if (*visible_functions)
        tmp.move(**visible_functions);
      visible = &tmp;
    }
    forv_Fun(f, *visible) if (f)
      if (f->is_varargs)
        function_values.set_add(f);
      else
        (*visible_functions)->set_add(f);
  }
}

static int
incomplete_call(Vec<AVar *> &args, AVar *send) {
  forv_AVar(av, args) {
    if (!av->out->n) {
      av->arg_of_send.add(send);
      return 1;
    }
  }
  return 0;
}

static void
get_all_args(AVar *a, MapMPositionAType &all_args, Vec<MPosition *> &ps, MPosition &p) {
  MPosition *cp = cannonicalize_mposition(p);
  if (!ps.set_in(cp))
    return;
  add_all_args(all_args, cp, a->out);
  forv_CreationSet(cs, a->out->sorted) {
    for (int i = 0; i < cs->vars.n; i++) {
      p.push(i+1);
      get_all_args(cs->vars.v[i], all_args, ps, p);
      p.pop();
    }
  }
}

static void
get_all_args(Vec<AVar *> &args, MapMPositionAType &all_args, Vec<MPosition *> &ps) {
  MPosition p;
  p.push(1);
  forv_AVar(a, args) {
    get_all_args(a, all_args, ps, p);
    p.inc();
  }
}

static int
match_cache_hit(Vec<AVar *> &args, AVar *send, int is_closure, Partial_kind partial, 
                PNode *visibility_point, Vec<Match *> &matches) {
  MatchCache *m = send->match_cache;
  if (!m) 
    return 0;
  Vec<MatchCacheEntry *> entries;
  forv_MatchCacheEntry(e, *m) {
    if (e->is_closure != is_closure)
      continue;
    if (e->partial != partial)
      continue;
    if (e->visibility_point != visibility_point)
      continue;
    entries.add(e);
  }
  if (!entries.n)
    return 0;
  MapMPositionAType all_args;
  Vec<MPosition *> ps;
  forv_MatchCacheEntry(e, entries)
    form_MPositionAType(x, e->all_args)
      ps.set_add(x->key);
  get_all_args(args, all_args, ps);
  forv_MatchCacheEntry(e, entries) {
    form_MPositionAType(x, e->all_args)
      if (x->value != all_args.get(x->key))
        goto Lfail;
    log(LOG_DISPATCH, "%d- pass: %d hits: %d\n", 
        send->var->sym->id, analysis_pass, e->n);
    forv_Match(mm, *e) {
      log(LOG_DISPATCH, "%d- hit: %d\n", 
          send->var->sym->id, mm->fun->sym->id);
      matches.add(new Match(*mm));
    }
    return 1;
  Lfail:;
  }
  return 0;
}

//
// main dispatch entry point - given a vector of arguments return a vector of matches
//
int
pattern_match(Vec<AVar *> &args, AVar *send, int is_closure, Partial_kind partial, 
              PNode *visibility_point, Vec<Match *> &matches)
{
  pattern_matches++;
  if (incomplete_call(args, send))
    return 0;
  pattern_match_complete++;
  if (match_cache_hit(args, send, is_closure, partial, visibility_point, matches)) {
    pattern_match_hits++;
    return matches.n;
  }
  Matcher matcher(send, args.v[0], is_closure, partial);
  Vec<Fun *> *partial_matches = 0;
  find_visible_functions(args, visibility_point, &partial_matches, matcher.function_values);
  log_dispatch_pattern_match(partial_matches, matcher.function_values, send);
  MPosition app;
  matcher.find_all_matches(0, args, &partial_matches, app, 0);
  if (!partial_matches || !partial_matches->n)
    return 0;
  
  {
    MPosition app;
    Vec<CreationSet *> csargs;
    Vec<Fun *> result;
    matcher.find_best_matches(args, csargs, *partial_matches, app, result, 1);
    partial_matches->move(result);
    if (!partial_matches->n)
      return 0;
  }
  matcher.cannonicalize_matches(*partial_matches, is_closure, partial, matches, visibility_point);
  return matches.n;
}

//
// Build dispatch tables
//

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
  insert_fun(fa, f, a, dispatch_type(a), p);
  if (a->is_pattern) {
    p.push(1);
    forv_Sym(aa, a->has)
      build_arg(fa, f, aa, p);
    p.pop();
  }
  p.inc();
}

static void
build_patterns(FA *fa, Fun *f, int add) {
  MPosition p;
  p.push(1);
  insert_fun(fa, f, f->sym, f->sym, p);
  forv_Sym(a, f->sym->has)
    build_arg(fa, f, a, p);
  if (add)
    sym_match_cache.clear();
}

void
add_patterns(FA *fa, Fun *f) {
  build_patterns(fa, f, 1);
}

void
build_patterns(FA *fa) {
  fa->patterns = new Patterns;
  forv_Fun(f, fa->pdb->funs) 
    build_patterns(fa, f, 0);
}

//
// Build argument positions (MPosition)s for functions
// 

static MPosition *
build_arg_position(Fun *f, Sym *a, MPosition &fpp, MPosition *up) {
  MPosition *fcpp = cannonicalize_mposition(fpp), *fcnp = 0;
  if (up) {
    assert(!up->down || up->down == fcpp);
    up->down = fcpp;
  }
  if (a->name && !a->is_constant && !a->is_symbol) {
    MPosition fnp(fpp);
    fnp.set_top(a->name);
    fcnp = cannonicalize_mposition(fnp);
    f->named_to_positional.put(fcnp, fcpp);
  }
  for (MPosition *fcp = fcpp; fcp; fcp = fcnp, fcnp = 0) { 
    f->arg_positions.add(fcp);
    if (!a->var)
      a->var = new Var(a);
    a->var->is_formal = 1;
    f->arg_syms.put(fcp, a);
    if (fcp == fcpp) {
      if (is_Sym_OUT(a))
        f->out_positions.add(fcpp);
      f->positional_arg_positions.add(fcpp);
      if (a->is_pattern) {
        MPosition local_fpp(fpp);
        local_fpp.push(1);
        MPosition *prev = 0;
        forv_Sym(aa, a->has) {
          MPosition *next = 
            build_arg_position(f, aa, local_fpp, up);
          if (prev) {
            assert(!prev->next || prev->next == next);
            prev->next = next;
          }
          prev = next;
          up = 0;
          local_fpp.inc();
        }
        local_fpp.pop();
      }
    }
  }
  return fcpp;
}

MPosition *
build_arg_positions(Fun *f, MPosition *up) {
  MPosition fpp;
  MPosition *cp = cannonicalize_mposition(fpp);
  if (up) {
    assert(!up->down || up->down == cp);
    up->down = cp;
  }
  fpp.push(1);
  MPosition *prev = 0;
  forv_Sym(a, f->sym->has) {
    MPosition *next = 
      build_arg_position(f, a, fpp, up);
    if (prev) {
      assert(!prev->next || prev->next == next);
      prev->next = next;
    }
    prev = next;
    up = 0;
    fpp.inc();
  }
  forv_MPosition(p, f->positional_arg_positions) {
    Sym *s = f->arg_syms.get(p);
    f->args.put(p, s->var);
  }
  f->rets.add(f->sym->ret->var);
  return cp;
}

void
build_arg_positions(FA *fa) {
  forv_Fun(f, fa->pdb->funs)
    build_arg_positions(f);
}

void log_dispatch_cs_match(Matcher &matcher, Vec<CreationSet *> &csargs, 
                           MPosition &app, Vec<Fun *> &local_matches)
{
  if (!logging(LOG_DISPATCH)) return;
  log(LOG_DISPATCH, "%d- destructure_level: %d candidates: %d arguments: %d\n",
      matcher.send->var->sym->id, app.pos.n, local_matches.count(), csargs.n);
  for (int i = 0; i < csargs.n; i++) {
    Sym *s = csargs.v[i]->sym;
    log(LOG_DISPATCH, "%d- arg %d : ", matcher.send->var->sym->id, i);
    if (s->name)
      log(LOG_DISPATCH, "%s\n", s->name);
    else if (s->constant)
      log(LOG_DISPATCH, "%s\n", s->constant);
    else
      log(LOG_DISPATCH, "%d\n", s->id);
  }
}

void log_dispatch_funs(Matcher &matcher, Vec<Fun *> &funs) {
  if (!logging(LOG_DISPATCH)) return;
  log(LOG_DISPATCH, "%d- candidates:\n", matcher.send->var->sym->id);
  if (funs.n > 10) {
    log(LOG_DISPATCH, "%d- too many candidates: %d:\n", matcher.send->var->sym->id,
        funs.count());
    return;
  }
  for (int i = 0; i < funs.n; i++) {
    Sym *fn = funs.v[i]->sym;
    if (fn->filename() && fn->line() > 0)
      log(LOG_DISPATCH, "%d- %d: %s:%d Sym: %d AST_id: %d\n", matcher.send->var->sym->id, i + 1, 
          fn->filename(), fn->line(), fn->id, fn->ast_id());
    else
      log(LOG_DISPATCH, "%d- %d: _:-1 Sym: %d AST_id: %d\n", matcher.send->var->sym->id, i + 1, 
          fn->id, fn->ast_id());
  }
}

void log_dispatch_matches(Matcher &matcher, Vec<PMatch *> &matches) {
  if (!logging(LOG_DISPATCH)) return;
  Vec<Fun *> funs;
  forv_PMatch(m, matches) if (m)
    funs.add(m->fun);
  log_dispatch_funs(matcher, funs);
}


