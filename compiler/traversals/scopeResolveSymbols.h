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
#include "map.h"
#include "symscope.h"
#include "symbol.h"

class ScopeResolveSymbols : public Traversal {
 public:
  Map<SymScope*,Vec<VarSymbol*>*>* defList;
  ScopeResolveSymbols();
  void postProcessExpr(Expr* expr);
};

#endif
