#ifndef _CHECK_SEMANTICS_H_
#define _CHECK_SEMANTICS_H_

#include "traversal.h"

class CheckSemantics : public Traversal {
  void preProcessExpr(Expr* expr);
};

#endif

