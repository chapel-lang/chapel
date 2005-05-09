#ifndef _VERIFY_H_
#define _VERIFY_H_

#include "traversal.h"
#include "../symtab/symtabTraversal.h"

class Verify : public Traversal {
 public:
  Vec<Symbol*>* syms;
  Verify();
  virtual void preProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  void run(ModuleSymbol* moduleList);
};

#endif
