#ifndef _REMOVE_NESTED_FUNCTIONS_
#define _REMOVE_NESTED_FUNCTIONS_

#include "traversal.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "baseAST.h"

class RemoveNestedFunctions : public Traversal {
  Map<FnSymbol*,Vec<Symbol*>*>* _nested_func_args_map;
  Map<FnSymbol*,FnSymbol*>* _nested_func_sym_map;
public :
  RemoveNestedFunctions();
  void preProcessStmt(Stmt* stmt);
  void postProcessStmt(Stmt* stmt);
  void postProcessExpr(Expr* expr);
  static FnSymbol* hasEnclosingFunction(DefExpr* fn_def);
  Vec<Symbol*>* getEnclosingFuncVarUses(FnSymbol* fn_sym);
  void addNestedFuncFormals(Expr* expr, Vec<Symbol*>* encl_var_uses, FnSymbol* old_func_sym);
  void addNestedFuncActuals(CallExpr* paren_op, Vec<Symbol*>* encl_var_uses, FnSymbol* old_func_sym);
};

#endif
