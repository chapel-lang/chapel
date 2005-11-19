#ifndef _FIXUP_H_
#define _FIXUP_H_

#include "traversal.h"
#include "baseAST.h"
#include "vec.h"

class Fixup : public Traversal {
 public:
  bool verifyParents;
  bool insertHelper;  // true fixup on inserts, false fixup on removes
  Vec<Expr*> parentExprs;
  Vec<Stmt*> parentStmts;
  Vec<Symbol*> parentSymbols;
  Vec<Symbol*> defSymbols;

  Fixup::Fixup(bool initVerifyParents = false, bool initInsertHelper = true);
  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void postProcessExpr(Expr* expr);
  virtual void preProcessSymbol(Symbol* sym);
  virtual void postProcessSymbol(Symbol* sym);
  virtual void run(Vec<ModuleSymbol*>* modules);
};

void fixup(void);
void fixup(BaseAST* ast);
void fixup(BaseAST* ast, BaseAST* ref);
void removeHelper(BaseAST* ast);
void insertHelper(BaseAST* ast, ASTContext context);

#endif
