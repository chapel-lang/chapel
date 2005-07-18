#ifndef _FIND_ENCLOSING_SCOPE_VAR_USES_
#define _FIND_ENCLOSING_SCOPE_VAR_USES_

#include "traversal.h"
#include "symscope.h"
#include "symbol.h"
#include "expr.h"

class FindEnclosingScopeVarUses : public Traversal {
  SymScope* _encl_scope;
  Vec<Symbol*>* _var_uses_vec;
public:
  FindEnclosingScopeVarUses(SymScope* encl_scope);
  void postProcessExpr(Expr* expr);
  Vec<Symbol*>* getVarUses();
  
};

#endif
