#ifndef _COLLECT_ASTS_H_
#define _COLLECT_ASTS_H_

#include "baseAST.h"
#include "traversal.h"
#include "vec.h"

class CollectASTs : public Traversal {
 public:
  Vec<BaseAST*>* asts;
  CollectASTs(Vec<BaseAST*>* init_asts);
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
};

#endif
