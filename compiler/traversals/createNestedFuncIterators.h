#ifndef _CREATE_NESTED_FUNC_ITERATORS_
#define _CREATE_NESTED_FUNC_ITERATORS_

#include "traversal.h"
#include "expr.h"
#include "stmt.h"

class CreateNestedFuncIterators : public Traversal {
public :
  void postProcessExpr(Expr* expr);
  DefExpr* copyLoopBodyToNestedFuncDef(ForLoopStmt* fls);
  FnSymbol* copyIteratorDef(FnSymbol* old_iterator_sym);
};

#endif
