#ifndef _FIND_ENCLOSING_SCOPE_VAR_USES_
#define _FIND_ENCLOSING_SCOPE_VAR_USES_

#include "traversal.h"
#include "symscope.h"
#include "symbol.h"
#include "expr.h"
#include "map.h"
#include "stmt.h"

class FindEnclosingScopeVarUses : public Traversal {
  SymScope* _encl_scope;
  Map<FnSymbol*,Vec<Symbol*>*>* _all_funcs_encl_var_uses_map;
  Vec<Symbol*>* _curr_var_uses_vec;
  Vec<FnSymbol*>* _in_process_fns;
public:
  FindEnclosingScopeVarUses(SymScope* encl_scope, Map<FnSymbol*,Vec<Symbol*>*>* all_funcs_encl_var_uses_map, 
                            Vec<FnSymbol*>* in_process_fns);
  void preProcessExpr(Expr* expr);
  void preProcessStmt(Stmt* stmt);
  void postProcessStmt(Stmt* stmt);
  Vec<Symbol*>* getVarUses();
  void checkIfEnclVar(Variable* var);
  void computeNestFuncEnclScopeVarUses(FnSymbol* fn_sym);
  Vec<Symbol*>* removeCurrScopeVars(FnSymbol* fn_sym);
};

#endif
