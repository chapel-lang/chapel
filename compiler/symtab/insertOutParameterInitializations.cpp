#include "insertOutParameterInitializations.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


InsertOutParameterInitializations::InsertOutParameterInitializations() {
  whichModules = MODULES_COMMON_AND_USER;
}


void InsertOutParameterInitializations::processSymbol(Symbol* sym) {
  ParamSymbol* arg = dynamic_cast<ParamSymbol*>(sym);

  if (!arg || !arg->init || arg->intent != PARAM_OUT) {
    return;
  }

  FnSymbol* fn = dynamic_cast<FnSymbol*>(arg->defPoint->sym);

  if (!fn) {
    INT_FATAL(sym, "Could not find function for ParamSymbol");
  }

  BlockStmt* body = dynamic_cast<BlockStmt*>(fn->body);

  if (!body) {
    body = new BlockStmt(fn->body);
    fn->body->replace(body);
  }

  Expr* assignExpr = new AssignOp(GETS_NORM, new Variable(arg), arg->init->copy());
  Stmt* initStmt = new ExprStmt(assignExpr);
  body->body->insertBefore(initStmt);
}
