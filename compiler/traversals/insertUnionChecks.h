#ifndef _INSERT_UNION_CHECKS_H_
#define _INSERT_UNION_CHECKS_H_

#include "traversal.h"

class InsertUnionChecks : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

#endif
