#ifndef _RESOLVE_OVERLOADED_OPERATORS_H_
#define _RESOLVE_OVERLOADED_OPERATORS_H_

#include "traversal.h"

class ResolveOverloadedOperators : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
