#ifndef _RESOLVE_SYMBOLS_H_
#define _RESOLVE_SYMBOLS_H_

#include "baseAST.h"
#include "traversal.h"

class ResolveSymbols : public Traversal {
 public:
  ResolveSymbols();
  void postProcessExpr(Expr* expr);
};

#endif
