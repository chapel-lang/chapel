#ifndef _INSERT_ANONYMOUS_TYPES_H_
#define _INSERT_ANONYMOUS_TYPES_H_

#include "traversal.h"

class InsertAnonymousTypes : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
};

#endif
