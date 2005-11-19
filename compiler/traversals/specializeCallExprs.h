#ifndef _SPECIALIZE_PAREN_OP_EXPRS_H_
#define _SPECIALIZE_PAREN_OP_EXPRS_H_

#include "traversal.h"

class SpecializeCallExprs : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
  void postProcessStmt(Stmt* stmt);
};

void specializeCallExprs(void);

#endif
