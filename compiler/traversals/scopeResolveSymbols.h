/******************************************************************************
 *** Scope Resolve Symbols
 ***
 *** This traversal resolves unresolved symbols that are easy to
 *** resolve, i.e., top-level, non-functions.
 ***
 ***/

#ifndef _SCOPE_RESOLVE_SYMBOLS_H_
#define _SCOPE_RESOLVE_SYMBOLS_H_

#include "traversal.h"

class ScopeResolveSymbols : public Traversal {
 public:
  ScopeResolveSymbols();
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* sym);
};

#endif
