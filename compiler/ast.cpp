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
