#include "astutil.h"
#include "expr.h"
#include "../traversals/fixup.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


void
verify() {
  Fixup* fixup = new Fixup(true);
  fixup->run(&allModules);

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts)
    ast->verify();
}
