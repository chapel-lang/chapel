#include "geysa.h"
#include "ast.h"
#include "sym.h"
#include "builtin.h"
#include "if1.h"

void AST::dump(FILE *fp, Fun *f) { }

void AST::graph(FILE *fp) { }

void
build_module(Sym *sym, Sym *fun) {
  sym->type = sym_module;
  sym->is_module = 1;
  fun->type = sym_function;
  fun->type_kind = Type_FUN;
  fun->type_sym = fun;
  sym->init = fun;
}

static void
new_builtin_symbol(Sym *&sym, char *name, char *builtin_name = 0) {
  if (!sym)
    sym = if1_make_symbol(if1, name);
  char *n = builtin_name ? builtin_name : name;
  if1_set_builtin(if1, sym, n);
}

void init_ast() {
  new_builtin_symbol(sym_primitive, "__primitive", "primitive");
  new_builtin_symbol(sym_reply, "reply");
  new_builtin_symbol(sym_make_tuple, "__make_tuple", "make_tuple");
  new_builtin_symbol(sym_make_vector, "__make_vector", "make_vector");
  new_builtin_symbol(sym_make_list, "__make_list", "make_list");
  new_builtin_symbol(sym_make_continuation, "__make_continuation", "make_continuation");
  new_builtin_symbol(sym_make_set, "__make_set", "make_set");
  new_builtin_symbol(sym_new, "__new", "new");
  new_builtin_symbol(sym_index_object, "__index_object", "index_object");
  new_builtin_symbol(sym_operator, "__operator", "operator");
  new_builtin_symbol(sym_destruct, "__destruct", "destruct");
  new_builtin_symbol(sym_meta_apply, "__meta_apply", "meta_apply");
  new_builtin_symbol(sym_period, ".", "period");
  new_builtin_symbol(sym_assign, "=", "assign");
}

static void
unalias_implements_constraints(IF1 *i) {
  // unalias
  forv_Sym(s, i->allsyms) {
    for (int x = 0; x < s->implements.n; x++)
      if (s->implements.v[x])
	s->implements.v[x] = unalias_type(s->implements.v[x]);
    if (s->constraints)
      for (int x = 0; x < s->constraints->n; x++)
	if (s->constraints->v[x])
	  s->constraints->v[x] = unalias_type(s->constraints->v[x]);
    s->type = unalias_type(s->type);
    s->in = unalias_type(s->in);
    if (s->type_kind != Type_NONE)
      s->type = s;
  }
  forv_Sym(s, i->allsyms) {
    Sym *us = unalias_type(s);
    if (s != us) {
      forv_Sym(ss, s->implements) if (ss) {
	assert(ss != us);
	us->implements.set_add(ss);
      }
    }
  }
}

static void 
closure_of_implements(IF1 *i) {
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, i->allsyms) {
      if (s->implements.n) {
	Vec<Sym *> supers(s->implements);
	forv_Sym(ss, supers) if (ss)
	  forv_Sym(sss, ss->implements) if (sss)
	    changed = s->implements.set_add(sss) || changed;
      }
    }
  }
  forv_Sym(s, i->allsyms)
    s->implements.set_to_vec();
}

static void
collect_includes(Sym *s, Vec<Sym *> &include_set, Vec<Sym *> &includes, Vec<Sym *> &in_includes) {
  if (!include_set.in(s) && in_includes.set_add(s)) {
    forv_Sym(ss, s->includes)
      collect_includes(ss, include_set, includes, in_includes);
    if (include_set.set_add(s))
      includes.add(s);
  }
}

static void
collect_include_vars(Sym *s, Sym *in = 0) {
  Vec<Sym *> saved;
  if (!in)
    saved.move(s->has);
  else
    in->has.append(s->has);
  forv_Sym(ss, s->includes)
    collect_include_vars(ss, in ? in : s);
  if (!in)
    s->has.append(saved);
}

static void
include_instance_variables(IF1 *i) {
  Vec<Sym *> include_set, includes;
  forv_Sym(s, i->allsyms) {
    Vec<Sym *> in_includes;
    if (s->includes.n)
      collect_includes(s, include_set, includes, in_includes);
  }
  forv_Sym(s, includes) 
    if (s->includes.n)
      collect_include_vars(s);
}

static void
set_value_for_value_classes(IF1 *i) {
  sym_value->is_value_class = 1;
  sym_anynum->is_value_class = 1;
  Vec<Sym *> implementers;
  forv_Sym(s, i->allsyms) {
    if (s->implements.n)
      implementers.add(s);
  }	
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, implementers)
      forv_Sym(ss, s->implements)
        if (ss->is_value_class && !s->is_value_class) { 
          changed = 1;
	  s->is_value_class = 1;
        }
  }
}

void
make_type_sym(Sym *s) {
  s->type_sym = new_Sym();
  s->type_sym->is_meta_class = 1;
  s->type_sym->in = s->in;
  s->type_sym->name = s->name;
  s->type_sym->type = s->type_sym;
  s->type_sym->ast = s->ast;
  s->type_sym->type_sym = s;
  s->type_sym->type_kind = Type_PRIMITIVE;
}

static void
make_type_syms(IF1 *i) {
  sym_anyclass->type_sym = sym_anyclass;
  sym_anyclass->is_meta_class = 1;
  forv_Sym(s, i->allsyms) {
    if (s->type_kind) {
      if (!s->type_sym)
	make_type_sym(s);
    }
  }
}

void
finalize_types(IF1 *i) {
  unalias_implements_constraints(i);
  closure_of_implements(i);
  include_instance_variables(i);
  // unalias builtin symbols
#define S(_n) sym_##_n = unalias_type(sym_##_n);
#include "builtin_symbols.h"
#undef S
  set_value_for_value_classes(i);
  make_type_syms(i);
}


