#ifndef _APPLY_GETTERS_SETTERS_H_
#define _APPLY_GETTERS_SETTERS_H_

#include "traversal.h"

class ApplyGettersSetters : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
