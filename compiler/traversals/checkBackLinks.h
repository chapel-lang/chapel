#ifndef _CHECK_BACK_LINKS_H_
#define _CHECK_BACK_LINKS_H_

#include "traversal.h"

class CheckBackLinks : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* symbol);

  void run(Stmt* program);
};

#endif
