#ifndef _INLINE_FUNCTIONS_
#define _INLINE_FUNCTIONS_

#include "traversal.h"
#include "symbol.h"
#include "map.h"
#include "expr.h"
#include "stmt.h"
#include "alist.h"

class InlineFunctions : public Traversal {
  bool _ok_to_inline;
  DefExpr* createTempVariable(Type* type, Expr* init = NULL);
  Map<BaseAST*,BaseAST*>* createFormalToActualArgMappings(CallExpr* fn_call, AList<DefExpr>* formal_args);
public:
  void postProcessExpr(Expr* expr);
  bool isFormalArgOut(ArgSymbol* p_sym);
  bool isFormalArgRef(ArgSymbol* p_sym);
  bool isTypeVar(ArgSymbol* p_sym);
  bool isCodegened(FnSymbol* fn_sym); 
};
#endif
