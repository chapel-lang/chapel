/*
  Copyright 2003-2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

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
  new_builtin_symbol(sym_index, "__index", "index");
  new_builtin_symbol(sym_operator, "__operator", "operator");
  new_builtin_symbol(sym_destruct, "__destruct", "destruct");
  new_builtin_symbol(sym_meta_apply, "__meta_apply", "meta_apply");
  new_builtin_symbol(sym_period, ".", "period");
  new_builtin_symbol(sym_assign, "=", "assign");
}
