#ifndef _PRE_ANALYSIS_HACKS_H_
#define _PRE_ANALYSIS_HACKS_H_

#include "traversal.h"
#include "symbol.h"
#include "expr.h"

class PreAnalysisHacks : public Traversal {
public:
  void postProcessStmt(Stmt* stmt);
  void postProcessExpr(Expr* expr);
  void postProcessType(Type* type);
};

#endif
