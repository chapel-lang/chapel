#ifndef _INSERT_UNION_CHECKS_H_
#define _INSERT_UNION_CHECKS_H_

#include "traversal.h"

class InsertUnionChecks : public Traversal {
 public:
  int writing;
  InsertUnionChecks::InsertUnionChecks(void);
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void postProcessExpr(Expr* expr);
};

#endif
