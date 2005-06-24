#ifndef _REPLACE_RETURNS_H_
#define _REPLACE_RETURNS_H_

#include "traversal.h"
#include "symbol.h"
#include "expr.h"

class ReplaceReturns : public Traversal {
  Symbol* _sym;
public:
  ReplaceReturns(Symbol* sym);
  void postProcessStmt(Stmt* stmt);
};

#endif
