#ifndef _METHODS_TO_FUNCTIONS_H_
#define _METHODS_TO_FUNCTIONS_H_

#include "baseAST.h"
#include "traversal.h"

class MethodsToFunctions : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

#endif
