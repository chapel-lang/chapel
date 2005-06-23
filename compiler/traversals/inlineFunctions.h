#ifndef _INLINE_FUNCTIONS_
#define _INLINE_FUNCTIONS_

#include "traversal.h"
#include "symbol.h"
#include "map.h"
#include "expr.h"
#include "stmt.h"
#include "alist.h"

class InlineFunctions : public Traversal {
  DefStmt* createTempVariable(Type* type, Expr* init = NULL);
  Map<BaseAST*,BaseAST*>* createFormalToActualArgMappings(FnCall* fn_call, AList<ParamSymbol>* formal_params);
public:
  void postProcessExpr(Expr* expr);
  bool isLeafFunction(FnSymbol* fs);
  virtual ~InlineFunctions();
};
#endif
