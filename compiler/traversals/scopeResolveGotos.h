#ifndef _SCOPE_RESOLVE_GOTOS_H_
#define _SCOPE_RESOLVE_GOTOS_H_

#include "traversal.h"
#include "stmt.h"

class ScopeResolveGotos : public Traversal {
 public:
  LabelStmt* currentLoop;
  ScopeResolveGotos();
  void preProcessStmt(Stmt* stmt);
};

#endif
