#ifndef _METHODS_TO_FUNCTIONS_H_
#define _METHODS_TO_FUNCTIONS_H_

#include "baseAST.h"
#include "traversal.h"

class MethodsToFunctions : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* symbol);
  void preProcessType(Type* type);
  void run(ModuleSymbol* moduleList);
};

#endif
