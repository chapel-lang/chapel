#ifndef _FIND_TYPE_VARIABLE_H_
#define _FIND_TYPE_VARIABLE_H_

#include "traversal.h"
#include "vec.h"
#include "type.h"

class FindVariableType : public Traversal {
 public:
  Vec<BaseAST*>* asts;
  bool found;
  FindVariableType(Vec<BaseAST*>* init_asts);
  void preProcessExpr(Expr* expr);
};

bool
functionContainsAnyAST(FnSymbol* fn, Vec<BaseAST*>* asts_set);

#endif
