#ifndef _TRANSFORM_LET_EXPRS_H_
#define _TRANSFORM_LET_EXPRS_H_

#include "traversal.h"
#include "baseAST.h"
#include "vec.h"

class TransformLetExprs : public Traversal {
 public:
  Vec<BaseAST*> lets;
  TransformLetExprs();
  void postProcessExpr(Expr* expr);
  virtual void run(Vec<ModuleSymbol*>* modules);
  void doTransformation(void);
};

#endif
