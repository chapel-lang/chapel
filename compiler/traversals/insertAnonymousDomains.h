#ifndef _INSERT_ANONYMOUS_DOMAINS_H_
#define _INSERT_ANONYMOUS_DOMAINS_H_

#include "traversal.h"
#include "symscope.h"

class InsertAnonymousDomains : public Traversal {
 public:
  Stmt* currentStmt;
  SymScope* currentScope;
  void preProcessStmt(Stmt* stmt);
  void preProcessType(Type* type);
  void postProcessStmt(Stmt* stmt);
};

#endif
