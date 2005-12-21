#ifndef _FIND_UNKNOWN_TYPES_H_
#define _FIND_UNKNOWN_TYPES_H_

#include "traversal.h"

class RemoveTypeVariableActuals : public Traversal {
  void postProcessExpr(Expr* expr);
};

class RemoveTypeVariableFormals : public Traversal {
  void postProcessExpr(Expr* expr);
};

void removeTypeVariableActuals(void);
void removeTypeVariableFormals(void);

#endif
