#ifndef _VERIFY_AST_TYPE_H_
#define _VERIFY_AST_TYPE_H_

#include "traversal.h"

class VerifyASTType : public Traversal {
 public:
  VerifyASTType(void);

  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* sym);
  void preProcessType(Type* type);
};

#endif

