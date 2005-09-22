#ifndef _INLINE_FUNCTIONS_
#define _INLINE_FUNCTIONS_

#include "traversal.h"
#include "symbol.h"
#include "map.h"
#include "expr.h"
#include "stmt.h"
#include "alist.h"

class InlineFunctions : public Traversal {
public:
  void postProcessExpr(Expr* expr);
  void run(Vec<ModuleSymbol*>* modules);
};
#endif
