#ifndef _CHECK_IDS_H_
#define _CHECK_IDS_H_

#include "baseAST.h"
#include "traversal.h"

class CheckIDs : public Traversal {
 public:
  BaseAST** astlist;
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* symbol);
  void preProcessType(Type* type);

  void run(Stmt* program);
};

#endif
