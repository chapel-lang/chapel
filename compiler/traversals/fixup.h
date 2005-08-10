#ifndef _FIXUP_H_
#define _FIXUP_H_

#include "traversal.h"
#include "baseAST.h"
#include "vec.h"

class Fixup : public Traversal {
 public:
  bool verifyParents;
  Vec<Expr*> parentExprs;
  Vec<Stmt*> parentStmts;
  Vec<Symbol*> parentSymbols;
  Vec<Symbol*> defSymbols;

  Fixup::Fixup(bool init_verifyParents = false);
  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void postProcessExpr(Expr* expr);
  virtual void preProcessSymbol(Symbol* sym);
  virtual void postProcessSymbol(Symbol* sym);
  virtual void run(Vec<ModuleSymbol*>* modules);
};

void fixup(BaseAST* ast);
void fixup(BaseAST* ast, BaseAST* ref);

#endif
