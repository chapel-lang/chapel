#ifndef _INSERT_ANONYMOUS_TYPES_H_
#define _INSERT_ANONYMOUS_TYPES_H_

#include "traversal.h"
#include "symscope.h"

class InsertAnonymousTypes : public Traversal {
 public:
  Stmt* currentStmt;
  SymScope* currentScope;
  void preProcessStmt(Stmt* stmt);
};

#endif
