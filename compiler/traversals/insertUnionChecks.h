#ifndef _INSERT_UNION_CHECKS_H_
#define _INSERT_UNION_CHECKS_H_

#include "traversal.h"

//runtime type checking for union types

class InsertUnionChecks : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

#endif
