#ifndef _FIXUP_H_
#define _FIXUP_H_

#include "traversal.h"
#include "baseAST.h"
#include "vec.h"

class Fixup : public Traversal {
 public:
  Vec<Expr*> parentExprs;
  Vec<Stmt*> parentStmts;
  Vec<Symbol*> parentSymbols;
  int verify;

  Fixup::Fixup();
  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void postProcessExpr(Expr* expr);
  virtual void postProcessSymbol(Symbol* sym);
  void run(ModuleSymbol* moduleList);
};

void fixup_stmt(Stmt* stmt);
void fixup_expr(Expr* expr);
void call_fixup(BaseAST* ast);

#endif
