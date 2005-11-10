#ifndef _FIND_UNKNOWN_TYPES_H_
#define _FIND_UNKNOWN_TYPES_H_

#include "traversal.h"

class RemoveTypeVariableActuals : public Traversal {
  void postProcessExpr(Expr* expr);
};

class RemoveTypeVariableFormals : public Traversal {
  void preProcessSymbol(Symbol* sym);
};

class FindUnknownTypes : public Traversal {
 public:
  FindUnknownTypes(void);
  void preProcessSymbol(Symbol* sym);
};

#endif

