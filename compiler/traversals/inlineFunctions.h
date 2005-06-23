#ifndef _INLINE_FUNCTIONS_
#define _INLINE_FUNCTIONS_

#include "traversal.h"
#include "symbol.h"
#include "map.h"
#include "expr.h"
#include "stmt.h"
#include "alist.h"

class InlineFunctions : public Traversal {
  Map<BaseAST*,BaseAST*>* _formal_to_actual_arg_map;
  DefStmt* createTempVariable(Type* type, Expr* init = NULL);
  void createFormalToActualArgMappings(FnCall* fn_call, AList<ParamSymbol>* formal_params);
public:
  InlineFunctions();
  void postProcessExpr(Expr* expr);
  bool isLeafFunction(FnSymbol* fs);
  virtual ~InlineFunctions();
};
#endif
