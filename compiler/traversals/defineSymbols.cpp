#include "defineSymbols.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


/***
 ***  Define symbols from statements in scope
 ***/


DefineSymbols::DefineSymbols(SymScope* init_scope) {
  scope = init_scope;
}


void DefineSymbols::preProcessStmt(Stmt* stmt) {
  if (!scope) {
    return;
  }

  if (VarDefStmt* def = dynamic_cast<VarDefStmt*>(stmt)) {
    Symboltable::defineInScope(def->var, scope);
  }
}
