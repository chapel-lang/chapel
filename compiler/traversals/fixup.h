#ifndef _FIXUP_H_
#define _FIXUP_H_

#include "traversal.h"
#include "baseAST.h"

class Fixup : public Traversal {
 public:
  bool verifyParents;
  Vec<Expr*> parentExprs;
  Vec<Stmt*> parentStmts;
  Vec<Symbol*> parentSymbols;
  Vec<Symbol*> defSymbols;

  Fixup::Fixup(bool initVerifyParents = false);
  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void postProcessExpr(Expr* expr);
  virtual void preProcessSymbol(Symbol* sym);
  virtual void postProcessSymbol(Symbol* sym);
  virtual void run(Vec<ModuleSymbol*>* modules);
};

void remove_help(BaseAST* ast);
void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Stmt* parentStmt,
                 Symbol* parentSymbol,
                 SymScope* parentScope);

#endif
