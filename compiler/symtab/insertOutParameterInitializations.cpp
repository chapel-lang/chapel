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
    if (fn->noparens && !fn->typeBinding) {
      USR_FATAL(fn, "Non-member functions must have parenthesized argument lists");
    }
  }

  ParamSymbol* arg = dynamic_cast<ParamSymbol*>(sym);

  if (!arg || !arg->defPoint->init || arg->intent != PARAM_OUT) {
    return;
  }

  FnSymbol* fn = dynamic_cast<FnSymbol*>(arg->defPoint->parentSymbol);

  if (!fn) {
    INT_FATAL(sym, "Could not find function for ParamSymbol");
  }

  BlockStmt* body = fn->body;

  Expr* assignExpr = new CallExpr(OP_GETSNORM, new Variable(arg), arg->defPoint->init->copy());
  Stmt* initStmt = new ExprStmt(assignExpr);
  body->body->insertAtHead(initStmt);
}
