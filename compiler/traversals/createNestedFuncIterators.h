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
  void postProcessStmt(Stmt* stmt);
  DefExpr* copyLoopBodyToNestedFuncDef(ForLoopStmt* fls, FnSymbol* iterator_sym);
  FnSymbol* copyIteratorDef(FnSymbol* old_iterato1r_sym);
  AList<Expr>* getIteratorCallsHelper(AList<Expr>* iterator_list, AList<Expr>* user_iterator_list);
  DefExpr* copyFuncHelper(char* new_name, AList<DefExpr>* copy_formals, BlockStmt* copy_body);
};

#endif
