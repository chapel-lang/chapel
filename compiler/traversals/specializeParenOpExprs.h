#ifndef _SPECIALIZE_PAREN_OP_EXPRS_H_
#define _SPECIALIZE_PAREN_OP_EXPRS_H_

#include "traversal.h"

class SpecializeParenOpExprs : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
  void postProcessStmt(Stmt* stmt);
};

#endif
