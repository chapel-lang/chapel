#ifndef _INSERT_ELIDED_LOOPS_H_
#define _INSERT_ELIDED_LOOPS_H_

#include "traversal.h"

class InsertElidedIndices : public Traversal {
 public:
  Symbol* indices;
  bool insert;
  InsertElidedIndices::InsertElidedIndices(Symbol* init_indices);
  void preProcessExpr(Expr* expr);
  void postProcessExpr(Expr* expr);
};

class InsertElidedLoops : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};

#endif
