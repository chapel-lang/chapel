#include "insertOutParameterInitializations.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


InsertOutParameterInitializations::InsertOutParameterInitializations() {
  whichModules = MODULES_CODEGEN;
}


void InsertOutParameterInitializations::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->noParens && !fn->typeBinding) {
      USR_FATAL(fn, "Non-member functions must have parenthesized argument lists");
    }
  }

  ArgSymbol* arg = dynamic_cast<ArgSymbol*>(sym);

  if (!arg || !arg->defPoint->init || arg->intent != INTENT_OUT) {
    return;
  }

  FnSymbol* fn = dynamic_cast<FnSymbol*>(arg->defPoint->parentSymbol);

  if (!fn) {
    INT_FATAL(sym, "Could not find function for ArgSymbol");
  }

  BlockStmt* body = fn->body;

  Expr* assignExpr = new CallExpr(OP_GETSNORM, new SymExpr(arg), arg->defPoint->init->copy());
  Stmt* initStmt = new ExprStmt(assignExpr);
  body->body->insertAtHead(initStmt);
}
