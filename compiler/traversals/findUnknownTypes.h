#ifndef _FIND_UNKNOWN_TYPES_H_
#define _FIND_UNKNOWN_TYPES_H_

#include "traversal.h"

class FindUnknownTypes : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
  void postProcessStmt(Stmt* stmt);
  void preProcessSymbol(Symbol* sym);
  void preProcessExpr(Expr* expr);
};

class PrintStmts : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
};

#endif

