#ifndef _RESOLVE_SYMBOLS_2_H_
#define _RESOLVE_SYMBOLS_2_H_

#include "baseAST.h"
#include "traversal.h"

class ResolveSymbols2 : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* symbol);
  void preProcessType(Type* type);
  void run(ModuleSymbol* moduleList);
};

#endif
