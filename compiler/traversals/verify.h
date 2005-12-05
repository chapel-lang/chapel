#ifndef _VERIFY_H_
#define _VERIFY_H_

#include "traversal.h"

class Verify : public Traversal {
 public:
  virtual void preProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void run(Vec<ModuleSymbol*>* modules);
};

void verify(void);

#endif
