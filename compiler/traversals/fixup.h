#ifndef _FIXUP_H_
#define _FIXUP_H_

#include "traversal.h"
#include "vec.h"

class Fixup : public Traversal {
 public:
  Vec<Stmt*> exprParent;
  Vec<Symbol*> stmtParent;

  Fixup::Fixup();

  virtual void preProcessType(Type* type);
  virtual void postProcessType(Type* type);
  virtual void preProcessSymbol(Symbol* sym);
  virtual void postProcessSymbol(Symbol* sym);
  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);

};

#endif
