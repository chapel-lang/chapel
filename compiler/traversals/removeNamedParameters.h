#ifndef _REMOVE_NAMED_PARAMETERS_H_
#define _REMOVE_NAMED_PARAMETERS_H_

#include "traversal.h"

class RemoveNamedParameters : public Traversal {
  void postProcessExpr(Expr* expr);
};

void removeNamedParameters(void);

#endif
