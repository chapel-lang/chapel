#ifndef _SCOPE_RESOLVE_GOTOS_H_
#define _SCOPE_RESOLVE_GOTOS_H_

#include "traversal.h"
#include "stmt.h"

class ScopeResolveGotos : public Traversal {
 public:
  LabelStmt* preCurrentLoop;
  LabelStmt* postCurrentLoop;
  ScopeResolveGotos();
  void preProcessStmt(Stmt* stmt);
};

void scopeResolveGotos(void);

#endif
