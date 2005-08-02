#ifndef _CREATE_NESTED_FUNC_ITERATORS_
#define _CREATE_NESTED_FUNC_ITERATORS_

#include "traversal.h"
#include "expr.h"
#include "stmt.h"
#include "vec.h"
#include "symbol.h"
#include "alist.h"

class CreateNestedFuncIterators : public Traversal {
public :
  void postProcessExpr(Expr* expr);
  DefExpr* copyLoopBodyToNestedFuncDef(ForLoopStmt* fls, Vec<Symbol*>* encl_scope_var_uses);
  FnSymbol* copyIteratorDef(FnSymbol* old_iterato1r_sym);
  Vec<Symbol*>* getEnclosingScopeVarUses(ForLoopStmt* fls);
  AList<DefExpr>* addEnclVarFormals(FnSymbol* fn_sym, Vec<Symbol*>* encl_scope_var_uses,
                                    Map<BaseAST*,BaseAST*>* update_map);
  void addFuncActuals(ParenOpExpr* paren_op, Vec<Symbol*>* encl_scope_var_uses);
};

#endif
