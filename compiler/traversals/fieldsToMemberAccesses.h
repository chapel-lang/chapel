#ifndef _FIELDS_TO_MEMBERACCESSES_H_
#define _FIELDS_TO_MEMBERACCESSES_H_

#include "baseAST.h"
#include "traversal.h"

class FieldsToMemberAccesses : public Traversal {
 public:
  FieldsToMemberAccesses(void);

  void preProcessStmt(Stmt* stmt);
  void postProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
};

#endif
