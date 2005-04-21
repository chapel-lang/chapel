#ifndef _INSERT_ELIDED_LOOPS_H_
#define _INSERT_ELIDED_LOOPS_H_

#include "traversal.h"
#include "expr.h"

class InsertElidedIndices : public Traversal {
 public:
  DefExpr* indices;
  InsertElidedIndices::InsertElidedIndices(DefExpr* init_indices);
  void preProcessExpr(Expr* expr);
};

class InsertElidedLoops : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};

#endif
