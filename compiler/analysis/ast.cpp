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
unalias_syms(IF1 *i) {
  forv_Sym(s, i->allsyms) {
    Sym *us = unalias_type(s);
    if (s != us) {
      forv_Sym(ss, s->specializes) if (ss) {
	assert(ss != us);
	us->specializes.add(ss);
      }
      forv_Sym(ss, s->includes) if (ss) {
	assert(ss != us);
	us->includes.add(ss);
      }
      forv_Sym(ss, s->implements) if (ss) {
	assert(ss != us);
	us->implements.add(ss);
      }
    }
  }
  forv_Sym(s, i->allsyms) {
    for (int x = 0; x < s->specializes.n; x++)
      s->specializes.v[x] = unalias_type(s->specializes.v[x]);
    for (int x = 0; x < s->includes.n; x++)
      s->includes.v[x] = unalias_type(s->includes.v[x]);
    for (int x = 0; x < s->implements.n; x++)
      s->implements.v[x] = unalias_type(s->implements.v[x]);
    if (s->must_specialize)
      s->must_specialize = unalias_type(s->must_specialize);
    if (s->must_implement)
      s->must_implement = unalias_type(s->must_implement);
    s->type = unalias_type(s->type);
    s->in = unalias_type(s->in);
    if (s->type_kind != Type_NONE) {
      assert(!s->type || s->type == s);
      s->type = s;
    }
  }
}

static void
implement(Sym *s, Sym *ss, Vec<Sym *> &types) {
  assert(s != ss);
  s->implementors.set_add(ss);
  types.set_add(s);
  types.set_add(ss);
}

static void
specialize(Sym *s, Sym *ss, Vec<Sym *> &types) {
  assert(s != ss);
  if (s->specializers.set_add(ss))
    ss->dispatch_order.add(s);
  types.set_add(s);
  types.set_add(ss);
}

static void
implement_and_specialize(Sym *s, Sym *ss, Vec<Sym *> &types) {
  implement(s, ss, types);
  specialize(s, ss, types);
}

void
build_type_hierarchy() {
  Vec<Sym *> type_syms, types;
  forv_Sym(s, if1->allsyms) {
    // functions implement and specialize symbols (selectors) of the same name
    //   this permits overloading of selectors with multiple functions
    if (s->is_symbol)
      implement_and_specialize(sym_symbol, s, types);
    // constants implement and specialize 'type'
    if (s->is_constant)
      implement_and_specialize(s->type, s, types);
    // functions implement and specialize of "function"
    if (s->is_fun)
      implement_and_specialize(sym_function, s, types);
    forv_Sym(ss, s->implements)
      implement(ss, s, types);
    forv_Sym(ss, s->specializes)
      specialize(ss, s, types);
    // functions implement and specializes of the initial symbol in their pattern
    // which may be a constant or a constant constrainted variable
    if (s->is_fun && s->has.n) {
      Sym *a = s->self ? s->has.v[1] : s->has.v[0];
      if (a->is_symbol && a->name == s->name)
	implement_and_specialize(a, s, types);
      else if (a->must_specialize && a->must_specialize->is_symbol && a->must_specialize->name == s->name)
	implement_and_specialize(a->must_specialize, s, types);
    }
    if (s->type_kind)
      types.set_add(s);
    if (s->type_sym)
      type_syms.add(s);
  }
  forv_Sym(s, types) if (s) {
    if (!s->dispatch_order.n && s != sym_any && s != sym_void) {
      if (s->is_meta_class && (s != sym_anyclass))
	implement_and_specialize(sym_anyclass, s, types);
      else if (s->is_value_class && (s != sym_value))
	implement_and_specialize(sym_value, s, types);
      else 
	implement_and_specialize(sym_any, s, types);
    }
  }
  // map subtyping and subclassing to type_syms
  forv_Sym(s, type_syms) if (!s->is_meta_class) {
    forv_Sym(ss, s->implementors) if (ss)
      s->type_sym->implementors.set_add(ss->type_sym);
    forv_Sym(ss, s->specializers) if (ss)
      s->type_sym->specializers.set_add(ss->type_sym);
  }
  forv_Sym(s, types) if (s) {
    s->implementors.set_add(s);
    s->specializers.set_add(s);
  }
  // compute implementors closure
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, types) if (s) {
      forv_Sym(ss, s->implementors) if (ss) {
	changed = s->implementors.set_union(ss->implementors) || changed;
      }
    }
  }
  // compute specializers closure
  changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, types) if (s) {
      forv_Sym(ss, s->specializers) if (ss) {
	changed = s->specializers.set_union(ss->specializers) || changed;
      }
    }
  }
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

static void
set_true_type_for_variables(IF1 *i) {
  forv_Sym(s, i->allsyms) {
    if (s->is_var && s->must_implement && s->must_implement->is_value_class)
      s->type = s->must_implement;
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
  unalias_syms(i);
  include_instance_variables(i);
#define S(_n) sym_##_n = unalias_type(sym_##_n);
#include "builtin_symbols.h"
#undef S
  set_value_for_value_classes(i);
  set_true_type_for_variables(i);
  make_type_syms(i);
}


