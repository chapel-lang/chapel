#ifndef _SETUP_STMT_LINKS_H_
#define _SETUP_STMT_LINKS_H_

#include "traversal.h"
#include "vec.h"

class SetupStmtLinks : public Traversal {
 public:
  Vec<Stmt*> stmts;

  SetupStmtLinks::SetupStmtLinks();

  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);

  virtual void preProcessExpr(Expr* expr);
};

#endif
