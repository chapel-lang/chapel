#include "expr.h"
#include "getstuff.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

GetStuff::GetStuff(int flags) :
  Traversal(false, false),
  getStmts(flags & GET_STMTS),
  getExprs(flags & GET_EXPRS),
  getSymbols(flags & GET_SYMBOLS),
  getTypes(flags & GET_TYPES)
{}


void GetStuff::preProcessStmt(Stmt* &stmt) {
  if (getStmts) {
    asts.add(stmt);
    stmts.add(stmt);
  }
}


void GetStuff::preProcessExpr(Expr* &expr) {
  if (getExprs) {
    asts.add(expr);
    exprs.add(expr);
  }
}


void GetStuff::preProcessSymbol(Symbol* &sym) {
  if (getSymbols) {
    asts.add(sym);
    symbols.add(sym);
  }
}


void GetStuff::preProcessType(Type* &type) {
  if (getTypes) {
    asts.add(type);
    types.add(type);
  }
}


void GetStuff::clear(void) {
  asts.clear();
  stmts.clear();
  exprs.clear();
  symbols.clear();
  types.clear();
}


GetStmts::GetStmts(void) :
  GetStuff(GET_STMTS)
{}


GetExprs::GetExprs(void) :
  GetStuff(GET_EXPRS)
{}


GetSymbols::GetSymbols(void) :
  GetStuff(GET_SYMBOLS)
{}


GetTypes::GetTypes(void) :
  GetStuff(GET_TYPES)
{}

